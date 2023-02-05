/**************************************************************************************************
 * 
 * Copyright 2022 https://github.com/fe-dagostino
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this 
 * software and associated documentation files (the "Software"), to deal in the Software 
 * without restriction, including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
 * permit persons to whom the Software is furnished to do so, subject to the following 
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies 
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 *
 *************************************************************************************************/

#include "ure_resources_fetcher.h"

#include <mailbox.h>

#include <thread>
#include <future>
#include <memory>
#include <cstring>

#include <curl/curl.h>


namespace ure {

  /***/
  class resource_t 
  {
  public:
    /***/
    resource_t() = delete;
    /***/
    constexpr resource_t( const std::string& name, const std::string& url )
      : m_name(name), m_url(url)
    {}
    /***/ 
    constexpr const std::string&  name() const
    { return m_name; }
    /***/ 
    constexpr const std::string&  url() const
    { return m_url;  }

  private:
    const std::string m_name;
    const std::string m_url;
  };


using mailbox_type = lock_free::mailbox<resource_t*, core::ds_impl_t::mutex, 0, 
                                        core::arena_allocator<core::node_t<resource_t*,false,true,false>, uint32_t, 1024, 1024, 0, 0, core::default_allocator<uint32_t>>
                                        >;


static mailbox_type*       s_mbx  = nullptr;
static std::atomic<bool>   s_exit = false;
static std::thread         s_scheduler;


struct MemoryStruct {
  MemoryStruct()
  {
    memory = nullptr;   /* will be grown as needed by the realloc above */
    size   = 0;         /* no data at this point */
  }
  ~MemoryStruct()
  {
    if ( memory != nullptr )
    {
      free(memory);
      memory = nullptr;
      size   = 0;         /* no data at this point */
    }
  }

  char *memory;
  size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = static_cast<char*>(std::realloc(mem->memory, mem->size + realsize + 1));
  if(!ptr) {
    // out of memory! 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  std::memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

static void_t async_curl_download( resource_t* pResource )
{
  std::unique_ptr<resource_t>   resource    = std::unique_ptr<resource_t>(pResource);
  ResourcesFetcher&             fetcher     = *ResourcesFetcher::get_instance();
  CURL *                        easy_handle = curl_easy_init();

  struct MemoryStruct           chunk;
 
  if ( easy_handle == nullptr )
  {
    fetcher.events()->on_download_failed( resource->name() );
    return;
  }

  if ( 
        /* specify URL to get */
        (curl_easy_setopt(easy_handle, CURLOPT_URL            , resource->url().c_str()  ) != CURLE_OK) ||
        /* send all data to this function  */
        (curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION  , WriteMemoryCallback      ) != CURLE_OK) ||
        /* we pass our 'chunk' struct to the callback function */
        (curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA      , (void *)&chunk           ) != CURLE_OK) ||
        /* some servers do not like requests that are made without a user-agent field, so we provide one */  
        (curl_easy_setopt(easy_handle, CURLOPT_USERAGENT      , "libcurl-agent/1.0"      ) != CURLE_OK) ||
        (curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION , 1L                       ) != CURLE_OK) ||
        (curl_easy_setopt(easy_handle, CURLOPT_HTTPPROXYTUNNEL, 1L                       ) != CURLE_OK)
      )
  {
    fetcher.events()->on_download_failed( resource->name() );
  }
  else
  {
    /* get it! */
    CURLcode res = curl_easy_perform(easy_handle);
    /* check for errors */
    if(res != CURLE_OK) 
    {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

      fetcher.events()->on_download_failed( resource->name() );
    }
    else 
    {
      /*
      * Now, our chunk.memory points to a memory block that is chunk.size
      * bytes big and contains the remote file.
      *
      * Do something nice with it!
      */
      printf("%lu bytes retrieved\n", (unsigned long)chunk.size);

      fetcher.events()->on_download_succeeded( resource->name(), (const byte_t*)chunk.memory, chunk.size );
    }
  }

  /* cleanup curl stuff */
  curl_easy_cleanup(easy_handle);
}

static void_t th_requests_scheduler()
{
  while ( s_exit == false )
  {
    resource_t* pResource = nullptr;
    if ( s_mbx->read( pResource , 100 ) == core::result_t::eSuccess )
    {
      std::async( async_curl_download, pResource );
    }
  }
}

bool ResourcesFetcher::fetch( const std::string& name, const std::string& url )
{
  if ( name.empty() || url.empty() )
    return false;

  resource_t* pResource = new(std::nothrow)resource_t(name,url);
  if ( pResource == nullptr )
    return false;

  s_mbx->write( pResource );

  return true;
} 

void_t ResourcesFetcher::on_initialize() noexcept
{
  curl_global_init(CURL_GLOBAL_ALL);

  s_mbx       = new(std::nothrow) mailbox_type("Resource Requests Mailbox");
  if ( s_mbx != nullptr )
  {
    s_scheduler = std::thread(th_requests_scheduler);
  }
}

void_t ResourcesFetcher::on_finalize() noexcept
{
  s_exit = true;

  s_scheduler.join();

  if ( s_mbx != nullptr )
  {
    delete s_mbx;
    s_mbx = nullptr;
  }

  curl_global_cleanup();
}

}