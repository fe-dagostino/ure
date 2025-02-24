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

#ifdef _USE_WEBSOCKETS

#include "ure_websocket.h"

#include <emscripten/emscripten.h>
#include <emscripten/websocket.h>

namespace ure {

EM_BOOL onopen( [[maybe_unused]] int eventType, [[maybe_unused]] const EmscriptenWebSocketOpenEvent *websocketEvent, void *userData) noexcept(true)
{
  return static_cast<WebSocket*>(userData)->on_open();

}

EM_BOOL onerror( [[maybe_unused]] int eventType, [[maybe_unused]] const EmscriptenWebSocketErrorEvent *websocketEvent, void *userData) noexcept(true)
{
  return static_cast<WebSocket*>(userData)->on_error();
}

EM_BOOL onclose( [[maybe_unused]] int eventType, [[maybe_unused]] const EmscriptenWebSocketCloseEvent *websocketEvent, void *userData) noexcept(true)
{
  return static_cast<WebSocket*>(userData)->on_close( websocketEvent->code );
}

EM_BOOL onmessage( [[maybe_unused]] int eventType, [[maybe_unused]] const EmscriptenWebSocketMessageEvent *websocketEvent, void *userData) noexcept(true)
{
  websocket_options::ws_mode_t mode = websocketEvent->isText?(websocket_options::ws_mode_t::text):(websocket_options::ws_mode_t::binary);
  return static_cast<WebSocket*>(userData)->on_message( mode, websocketEvent->data, websocketEvent->numBytes );
}

/***/
struct resource_t
{
  EMSCRIPTEN_WEBSOCKET_T   _ws;
};

bool WebSocket::open() noexcept(true)
{
  if (!emscripten_websocket_is_supported()) 
    return false;

  if ( m_data != nullptr )
    return false;

  m_ready = false;

  EmscriptenWebSocketCreateAttributes ws_attrs = {
                                                   options().url().data(),
                                                   nullptr,
                                                   EM_TRUE
                                                 };

  EMSCRIPTEN_WEBSOCKET_T ws = emscripten_websocket_new(&ws_attrs);

  m_data = new(std::nothrow)resource_t();
  if ( m_data == nullptr )
    return false;

  static_cast<resource_t*>(m_data)->_ws = ws;

  emscripten_websocket_set_onopen_callback   (ws, this, onopen   );
  emscripten_websocket_set_onerror_callback  (ws, this, onerror  );
  emscripten_websocket_set_onclose_callback  (ws, this, onclose  );
  emscripten_websocket_set_onmessage_callback(ws, this, onmessage);

  if ( options().blocking() == true )
  {
    uint16_t readyState = 0;
    do {
      emscripten_websocket_get_ready_state(ws, &readyState);
      emscripten_sleep(100);
    } while(readyState == 0);
  }

  return true;
}

bool_t WebSocket::send( const uint8_t* data, uint32_t length ) noexcept(true)
{
  unsigned short readyState = 0;
  
  emscripten_websocket_get_ready_state(static_cast<resource_t*>(m_data)->_ws, &readyState);
  if ( readyState == 0 )
    return false;
  
  EMSCRIPTEN_RESULT result = EMSCRIPTEN_RESULT_SUCCESS;

  switch (options().mode())
  {
    case websocket_options::ws_mode_t::text:
    {
      // Sends the given string of null-delimited UTF8 encoded text data to the connected server.
      result = emscripten_websocket_send_utf8_text( static_cast<resource_t*>(m_data)->_ws, reinterpret_cast<const char*>(data) );
    }; break;
  
    case websocket_options::ws_mode_t::binary:
    {
      // Sends the given block of raw memory data out to the connected server.
      result = emscripten_websocket_send_binary( static_cast<resource_t*>(m_data)->_ws, const_cast<typename std::remove_const<uint8_t>::type *>(data), length );
    }; break;

    default:
    break;
  }

  if (result != EMSCRIPTEN_RESULT_SUCCESS )
    return false;

  return true;
}

bool_t WebSocket::close() noexcept(true)
{
  if ( m_data == nullptr )
    return false;

  EMSCRIPTEN_RESULT result = emscripten_websocket_close( static_cast<resource_t*>(m_data)->_ws, 1000, "no reason" );
  if (result != EMSCRIPTEN_RESULT_SUCCESS )
    return false;

  result = emscripten_websocket_delete( static_cast<resource_t*>(m_data)->_ws );
  if (result != EMSCRIPTEN_RESULT_SUCCESS )
    return false;

  release();

  return true;
}

void_t WebSocket::release() noexcept(true)
{
  if ( m_data != nullptr )
  {
    delete static_cast<resource_t*>(m_data);
    m_data = nullptr;
  }
}

} /* namespace ure */

#endif /* _USE_WEBSOCKETS */