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

#ifndef URE_RESOURCES_FETCHER_EVENTS_H
#define URE_RESOURCES_FETCHER_EVENTS_H

#include "ure_common_defs.h"

namespace ure {

/**
 * 
 */
class ResourcesFetcherEvents 
{
public:
  /***/
  ResourcesFetcherEvents() noexcept
  {}
  /***/
  virtual ~ResourcesFetcherEvents() noexcept 
  {}

  virtual void_t    on_download_succeeded( [[maybe_unused]] const std::string& name, [[maybe_unused]] const std::type_info& type, [[maybe_unused]] const byte_t* data, [[maybe_unused]] uint_t length ) = 0;
  virtual void_t    on_download_failed   ( [[maybe_unused]] const std::string& name ) = 0;
};

}

#endif // URE_RESOURCES_FETCHER_EVENTS_H
