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
    constexpr resource_t( ResourcesFetcherEvents& events, const std::string& name, const std::type_info& type, const std::string& url ) noexcept(true)
      : m_events(events), m_name(name), m_type(type), m_url(url)
    {}
    /***/ 
    constexpr ResourcesFetcherEvents& events() const noexcept(true)
    { return m_events; }
    /***/ 
    constexpr const std::string&      name() const noexcept(true)
    { return m_name; }
    /***/ 
    constexpr const std::type_info&   type() const noexcept(true)
    { return m_type; }
    /***/ 
    constexpr const std::string&      url() const noexcept(true)
    { return m_url;  }

  private:
    ResourcesFetcherEvents& m_events;
    const std::string       m_name;
    const std::type_info&   m_type;
    const std::string       m_url;
  };


using mailbox_type = lock_free::mailbox<resource_t*, core::ds_impl_t::mutex, 0, 
                                        core::arena_allocator<core::node_t<resource_t*,false,true,false>, uint32_t, 1024, 1024, 0, 0, core::default_allocator<uint32_t>>
                                        >;


static mailbox_type*       s_mbx  = nullptr;
static std::atomic<bool_t> s_exit = false;
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

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) noexcept(true)
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

static void_t async_curl_download( resource_t* resource ) noexcept(true)
{
  std::unique_ptr<resource_t>   _resource    = std::unique_ptr<resource_t>(resource);
  CURL *                        _easy_handle = curl_easy_init();

  struct MemoryStruct           _chunk;
 
  if ( _easy_handle == nullptr )
  {
    _resource->events().on_download_failed( _resource->name() );

    if ( ResourcesFetcher::get_instance()->cancel( _resource->name() ) == false )
    {
      //@todo
    } 

    return;
  }

  if ( 
        /* specify URL to get */
        (curl_easy_setopt(_easy_handle, CURLOPT_URL            , _resource->url().c_str()  ) != CURLE_OK) ||
        /* send all data to this function  */
        (curl_easy_setopt(_easy_handle, CURLOPT_WRITEFUNCTION  , WriteMemoryCallback      ) != CURLE_OK) ||
        /* we pass our 'chunk' struct to the callback function */
        (curl_easy_setopt(_easy_handle, CURLOPT_WRITEDATA      , (void *)&_chunk           ) != CURLE_OK) ||
        /* some servers do not like requests that are made without a user-agent field, so we provide one */  
        (curl_easy_setopt(_easy_handle, CURLOPT_USERAGENT      , "libcurl-agent/1.0"      ) != CURLE_OK) ||
        (curl_easy_setopt(_easy_handle, CURLOPT_FOLLOWLOCATION , 1L                       ) != CURLE_OK) ||
        (curl_easy_setopt(_easy_handle, CURLOPT_HTTPPROXYTUNNEL, 1L                       ) != CURLE_OK)
      )
  {
    _resource->events().on_download_failed( _resource->name() );

    if ( ResourcesFetcher::get_instance()->cancel( _resource->name() ) == false )
    {
      //@todo
    } 
  }
  else
  {
    /* get it! */
    CURLcode res = curl_easy_perform(_easy_handle);
    /* check for errors */
    if(res != CURLE_OK) 
    {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

      _resource->events().on_download_failed( _resource->name() );

      if ( ResourcesFetcher::get_instance()->cancel( _resource->name() ) == false )
      {
        //@todo
      } 
    }
    else 
    {
      /*
      * Now, our chunk.memory points to a memory block that is chunk.size
      * bytes big and contains the remote file.
      *
      * Do something nice with it!
      */
      printf("%lu bytes retrieved\n", (unsigned long)_chunk.size);

      _resource->events().on_download_succeeded( _resource->name(), _resource->type(), (const byte_t*)_chunk.memory, _chunk.size );

      if ( ResourcesFetcher::get_instance()->cancel( _resource->name() ) == false )
      {
        //@todo
      } 

    }
  }

  /* cleanup curl stuff */
  curl_easy_cleanup(_easy_handle);
}

static void_t th_requests_scheduler() noexcept(true)
{
  while ( s_exit == false )
  {
    resource_t* pResource = nullptr;
    if ( s_mbx->read( pResource , 100 ) == core::result_t::eSuccess )
    {
      auto handle = std::async( async_curl_download, pResource );
    }
  }
}

bool_t ResourcesFetcher::fetch( ResourcesFetcherEvents& events, const std::string& name, const std::type_info& type, const std::string& url ) noexcept(true)
{
  if ( name.empty() || url.empty() )
    return false;

  std::lock_guard _mtx(m_mtx_fetch);

  /***/
  if ( m_fetching.contains( name ) == true )
    return true;

  resource_t* pResource = new(std::nothrow)resource_t( events, name, type, url);
  if ( pResource == nullptr )
  {
    return false;
  }

  /***/
  m_fetching.insert(name);

  s_mbx->write( pResource );

  return true;
} 

void_t ResourcesFetcher::on_initialize() noexcept(true)
{
  curl_global_init(CURL_GLOBAL_ALL);

  s_mbx       = new(std::nothrow) mailbox_type("Resource Requests Mailbox");
  if ( s_mbx != nullptr )
  {
    s_scheduler = std::thread(th_requests_scheduler);
  }
}

void_t ResourcesFetcher::on_finalize() noexcept(true)
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