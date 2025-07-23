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
  using http_headers_t     = std::vector<const char*>; 
  using http_body_t        = std::string; 

  /***/
  resource_t() = delete;
  /***/
  constexpr resource_t( ResourcesFetcherEvents& events,
                        const std::string&      name,
                        const std::type_info&   type,
                        customer_request_t      cr,
                        const http_headers_t&   headers,
                        const http_body_t&      body,
                        bool                    verify_ssl
                      ) noexcept(true)
    : m_events(events), m_name(name), m_type(type), m_cr(cr),
      m_headers(headers), m_body(body), m_verify_ssl(verify_ssl)
  {}
  /***/ 
  constexpr ResourcesFetcherEvents& events() const noexcept(true)
  { return m_events; }
  /***/ 
  constexpr const std::string&     name() const noexcept(true)
  { return m_name; }
  /***/
  constexpr const std::type_info&  type() const noexcept(true)
  { return m_type; }
  /***/
  constexpr customer_request_t     cr() const noexcept(true)
  { return m_cr; }
  /***/
  constexpr const http_headers_t&  headers() const noexcept(true)
  { return m_headers; }
  /***/
  constexpr std::string_view       body() const noexcept(true)
  { return m_body; }
  /***/
  constexpr bool                   verify_ssl() const noexcept(true)
  { return m_verify_ssl; }

private:
  ResourcesFetcherEvents& m_events;
  const std::string       m_name;
  const std::type_info&   m_type;
  customer_request_t      m_cr;
  http_headers_t          m_headers;
  http_body_t             m_body;
  bool                    m_verify_ssl;
};

static void_t emscripten_download_succeeded(emscripten_fetch_t *fetch) noexcept(true)
{
  printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);

  if ( fetch->userData != nullptr ) 
  {
    std::unique_ptr<resource_t> _resource =  std::unique_ptr<resource_t>( reinterpret_cast<resource_t*>(fetch->userData) );

    _resource->events().on_download_succeeded( _resource->name(), _resource->cr(), _resource->type(), (const byte_t*)fetch->data, fetch->numBytes );

    if ( ResourcesFetcher::get_instance()->cancel( _resource->name(), _resource->cr() ) == false )
    {
      //@todo
    }

  }

  // The data is now available at fetch->data[0] through fetch->data[fetch->numBytes-1];
  emscripten_fetch_close(fetch); // Free data associated with the fetch.
}

static void_t emscripten_download_failed(emscripten_fetch_t *fetch) noexcept(true)
{
  printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);

  if ( fetch->userData != nullptr ) 
  {
    std::unique_ptr<resource_t> _resource =  std::unique_ptr<resource_t>( (resource_t*)fetch->userData );

    _resource->events().on_download_failed( _resource->name(), _resource->cr() );

    if ( ResourcesFetcher::get_instance()->cancel( _resource->name(), _resource->cr() ) == false )
    {
      //@todo
    } 
  }

  emscripten_fetch_close(fetch); // Also free data on failure.
}

bool_t ResourcesFetcher::fetch( ResourcesFetcherEvents& events, 
                                const std::string&      name, 
                                const std::type_info&   type, 
                                const std::string&      url, 
                                customer_request_t      cr,
                                const http_headers_t&   headers,
                                const http_body_t&      body,
                                bool                    verify_ssl
                              ) noexcept(true)
{
  if ( name.empty() || url.empty() )
    return false;

  std::string     _name = core::utils::format( "%s:%s", to_string_view(cr).data(), name.c_str() );
  std::lock_guard _mtx(m_mtx_fetch);

  /***/
  if ( m_fetching.contains( _name ) == true )
    return true;

  resource_t* _resource = new(std::nothrow)resource_t( events, name, type, cr, headers, body, verify_ssl );
  if ( _resource == nullptr )
  {
    return false;
  }

  /***/
  m_fetching.insert(_name);

  emscripten_fetch_attr_t attr;
  emscripten_fetch_attr_init(&attr);
  strcpy(attr.requestMethod, ResourcesFetcher::to_string_view(cr).data() );

  if ( _resource->headers().empty() == false )
  {
    attr.requestHeaders = _resource->headers().data();
  }

  if ( _resource->body().empty( ) == false )
  {
    attr.requestData     = _resource->body().data();
    attr.requestDataSize = _resource->body().length();
  }

  attr.userData   = _resource;
  attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
  attr.onsuccess  = emscripten_download_succeeded;
  attr.onerror    = emscripten_download_failed;
  emscripten_fetch(&attr, url.c_str() );

  return true;
} 

void_t ResourcesFetcher::on_initialize() noexcept(true)
{
}

void_t ResourcesFetcher::on_finalize() noexcept(true)
{
}

}
