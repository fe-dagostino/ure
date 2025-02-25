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

#ifndef URE_WEBSOCKET_H
#define URE_WEBSOCKET_H

#include "ure_common_defs.h"
#include "ure_websocket_events.h"
#include "ure_object.h"

#include <string>
#include <atomic>

namespace ure {

#ifdef _USE_WEBSOCKETS

/**
 * 
 */
class WebSocket : public Object
{
public:
  /***/
  WebSocket() = delete;
  /***/
  WebSocket( const std::string&   id,
             WebSocketEvents&     events, 
             websocket_options&&  options 
           ) noexcept(true)
    : m_id(id), m_events(events), m_options( std::move(options) ), m_data(nullptr), m_ready(false)
  {}

  /***/
  ~WebSocket()
  {
    release();
  }

  /***/
  constexpr std::string_view         id() const noexcept(true)
  { return m_id; }
  /***/
  bool_t                             is_ready() const noexcept(true)
  { return m_ready; }
  /***/
  constexpr WebSocketEvents&         events() noexcept(true)
  { return m_events; }
  /***/
  constexpr const websocket_options& options() const noexcept(true)
  { return m_options; }

  /***/
  bool_t open() noexcept(true);
  /***/
  bool_t send( const uint8_t* data, uint32_t length ) noexcept(true);
  /***/
  bool_t close() noexcept(true);

  /***/
  bool_t on_open() noexcept(true)
  {
    m_ready = true;
    return events().on_websocket_open( id() );
  }

  /***/
  bool_t on_close( uint16_t code ) noexcept(true)
  {
    m_ready = false;
    return events().on_websocket_close( id(), code );
  }
  
  /***/
  bool_t on_message( websocket_options::ws_mode_t mode, const uint8_t* data, uint32_t length ) noexcept(true)
  { return events().on_websocket_message( id(), mode, data, length ); }
  /***/
  bool_t on_error() noexcept(true)
  { return events().on_websocket_error( id() ); }

private:
  /***/
  void_t release() noexcept(true);

private:
  const std::string  m_id;
  WebSocketEvents&   m_events;
  websocket_options  m_options;
  void*              m_data;
  std::atomic_bool   m_ready;
};

#endif /* _USE_WEBSOCKETS */

}

#endif /* URE_WEBSOCKET_H */
