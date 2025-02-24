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

#ifndef URE_WEBSOCKET_EVENTS_H
#define URE_WEBSOCKET_EVENTS_H

#include "ure_common_defs.h"
#include "ure_websocket_options.h"

namespace ure {

/**
 * 
 */
class WebSocketEvents 
{
public:
  /***/
  virtual bool_t    on_websocket_open   ( [[maybe_unused]] const std::string_view id ) noexcept(true) = 0;
  /***/
  virtual bool_t    on_websocket_close  ( [[maybe_unused]] const std::string_view id, uint16_t code ) noexcept(true) = 0;
  /***/
  virtual bool_t    on_websocket_message( [[maybe_unused]] const std::string_view id, websocket_options::ws_mode_t mode, const uint8_t* data, uint32_t length ) noexcept(true) = 0;
  /***/
  virtual bool_t    on_websocket_error  ( [[maybe_unused]] const std::string_view id ) noexcept(true) = 0;
};

}

#endif /* URE_WEBSOCKET_EVENTS_H */
