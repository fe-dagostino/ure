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

#ifndef URE_RESOURCES_FETCHER_H
#define URE_RESOURCES_FETCHER_H

#include "ure_common_defs.h"
#include "ure_resources_fetcher_events.h"

#include <core/singleton.h>
#include <core/unique_ptr.h>

#include <string>
#include <set>

namespace ure {

/**
 * 
 */
class ResourcesFetcher final: public core::singleton_t<ResourcesFetcher>
{
  friend class singleton_t<ResourcesFetcher>;
protected:

  /***/
  ResourcesFetcher() noexcept(true)
  {}

public:
  using http_headers_t     = std::vector<const char*>; 
  using http_body_t        = std::string; 

  enum class customer_request_t : enum_t
  {
    Get,
    Post,
    Put,
    Patch,
    Delete
  }; 

  static std::string_view to_string_view( customer_request_t cr )
  {
    switch (cr)
    {
      case customer_request_t::Get:
        return "GET";
      break;
      case customer_request_t::Post:
        return "POST";
      break;
      case customer_request_t::Put:
        return "PUT";
      break;
      case customer_request_t::Patch:
        return "PATCH";
      break;
      case customer_request_t::Delete:
        return "DELETE";
      break;
    }
    return "";
  }

  /***/
  bool_t            fetch ( ResourcesFetcherEvents&  events,
                            const std::string&       name,  
                            const std::type_info&    type,
                            const std::string&       url,
                            customer_request_t       cr,
                            const http_headers_t&    headers,
                            const http_body_t&       body,
                            bool                     verify_ssl = true
                          ) noexcept(true);
  /***/
  bool_t            cancel( std::string_view name ) noexcept(true)
  {
    std::lock_guard _mtx(m_mtx_fetch);
    
    if ( m_fetching.contains(name.data()) == false )
      return false;

    m_fetching.erase(name.data());

    return true;
  }
protected:
  /***/
  void_t  on_initialize() noexcept(true);

  /***/
  void_t  on_finalize() noexcept(true);
  
private:
  std::mutex                                m_mtx_fetch;
  std::set<std::string>                     m_fetching;
};

}

#endif // URE_RESOURCES_FETCHER_H
