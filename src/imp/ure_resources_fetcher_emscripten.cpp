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

namespace ure {

/***/
class resource_t 
{
public:
  /***/
  resource_t() = delete;
  /***/
  constexpr resource_t( const std::string& name, const std::type_info& type )
    : m_name(name), m_type(type)
  {}
  /***/ 
  constexpr const std::string&     name() const noexcept
  { return m_name; }

  /***/
  constexpr const std::type_info&  type() const noexcept
  { return m_type; }

private:
  const std::string       m_name;
  const std::type_info&   m_type;

};


static void_t emscripten_download_succeeded(emscripten_fetch_t *fetch) 
{
  printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);

  if ( fetch->userData != nullptr ) 
  {
    std::unique_ptr<resource_t> resource =  std::unique_ptr<resource_t>( reinterpret_cast<resource_t*>(fetch->userData) );

    if ( ResourcesFetcher::get_instance()->events() != nullptr )
    {
      ResourcesFetcher::get_instance()->events()->on_download_succeeded( resource->name(), resource->type(), (const byte_t*)fetch->data, fetch->numBytes );
    }

    if ( ResourcesFetcher::get_instance()->cancel( resource->name() ) == false )
    {
      //@todo
    } 

  }
   
  // The data is now available at fetch->data[0] through fetch->data[fetch->numBytes-1];
  emscripten_fetch_close(fetch); // Free data associated with the fetch.
}

static void_t emscripten_download_failed(emscripten_fetch_t *fetch) 
{
  printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);

  if ( fetch->userData != nullptr ) 
  {
    std::unique_ptr<resource_t> resource =  std::unique_ptr<resource_t>( (resource_t*)fetch->userData );

    if ( ResourcesFetcher::get_instance()->events() != nullptr )
    {
      ResourcesFetcher::get_instance()->events()->on_download_failed( resource->name() );
    }

    if ( ResourcesFetcher::get_instance()->cancel( resource->name() ) == false )
    {
      //@todo
    } 
  }

  emscripten_fetch_close(fetch); // Also free data on failure.
}

bool ResourcesFetcher::fetch( const std::string& name, const std::type_info& type, const std::string& url ) noexcept
{
  if ( name.empty() || url.empty() )
    return false;

  std::lock_guard _mtx(m_mtx_fetch);

  /***/
  if ( m_fetching.contains( name ) == true )
    return true;

  resource_t* pResource = new(std::nothrow)resource_t( name, type );
  if ( pResource == nullptr )
  {
    return false;
  }

  /***/
  m_fetching.insert(name);

  emscripten_fetch_attr_t attr;
  emscripten_fetch_attr_init(&attr);
  strcpy(attr.requestMethod, "GET");
  attr.userData   = pResource;
  attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
  attr.onsuccess  = emscripten_download_succeeded;
  attr.onerror    = emscripten_download_failed;
  emscripten_fetch(&attr, url.c_str() );

  return true;
} 

void_t ResourcesFetcher::on_initialize() noexcept
{
}

void_t ResourcesFetcher::on_finalize() noexcept
{
}

}