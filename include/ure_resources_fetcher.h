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

namespace ure {

/**
 * 
 */
class ResourcesFetcher final: public core::singleton_t<ResourcesFetcher>
{
  friend class singleton_t<ResourcesFetcher>;
public:
  /***/
  ResourcesFetcher() = delete;
  /***/
  ResourcesFetcher( core::unique_ptr<ResourcesFetcherEvents> events )
    : m_events( std::move(events) )
  {}

  /***/
  ResourcesFetcherEvents* events()
  { return m_events; }

  /***/
  bool                    fetch( const std::string& name, const std::type_info& type, const std::string& url );

protected:
  /***/
  void_t  on_initialize() noexcept;

  /***/
  void_t  on_finalize() noexcept;
  
private:
  core::unique_ptr<ResourcesFetcherEvents>  m_events;
};

}

#endif // URE_RESOURCES_FETCHER_H
