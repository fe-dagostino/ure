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

#ifndef URE_WEBSOCKET_OPTIONS_H
#define URE_WEBSOCKET_OPTIONS_H

#include "ure_common_defs.h"
#include <string>

namespace ure {

class websocket_options
{
public:
  enum class ws_mode_t : std::size_t {
    text    = 0,
    binary
  };

  /***/
  constexpr websocket_options( websocket_options&& opts ) noexcept(true)
    : m_url(std::move(opts.m_url)), m_mode(opts.m_mode), 
      m_blocking(opts.m_blocking), m_raw(opts.m_raw)
  {}

  /***/
  constexpr websocket_options( const std::string& url, ws_mode_t mode, bool blocking = true, bool raw = false ) noexcept(true)
   : m_url(url), m_mode(mode), m_blocking(blocking), m_raw(raw)
  {}

  /***/
  ~websocket_options() noexcept(true)
  {}

  /***/
  constexpr const std::string_view      url() const noexcept(true)
  { return m_url; }

  /***/
  constexpr ws_mode_t                   mode() const noexcept(true)
  { return m_mode; }

  /***/
  constexpr bool                        blocking() const noexcept(true)
  { return m_blocking; }

  /***/
  constexpr bool                        raw() const noexcept(true)
  { return m_raw; }

private:
  const std::string    m_url;
  const ws_mode_t      m_mode;
  const bool           m_blocking;
  const bool           m_raw;
};

}

#endif /* URE_WEBSOCKET_OPTIONS_H */
