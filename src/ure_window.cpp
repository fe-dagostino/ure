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

#include "ure_window.h"

namespace ure {

#ifdef __EMSCRIPTEN__
EM_JS(int, canvas_get_width, (), {
  return Module.canvas.width;
});

EM_JS(int, canvas_get_height, (), {
  return Module.canvas.height;
});
#endif


Window::Window()
 : m_glsl_version( "#version 100" ),
   m_ptrWinOptions(nullptr), 
   m_hWindow( nullptr ), 
   m_ptrRenderer( nullptr ),
   m_mbxMessages( "Window Mailbox" ) 
{
}

Window::~Window()
{
  destroy();
}

bool  Window::connect( WindowEvents* pEvents ) noexcept
{
  events_type::iterator _iter = std::find(m_events.begin(), m_events.end(), pEvents);
  if ( _iter != m_events.end() )
    return false;
  
  m_events.push_back( pEvents );
  
  return true;
}

bool  Window::disconnect( WindowEvents* pEvents ) noexcept
{
  events_type::iterator _iter = std::find(m_events.begin(), m_events.end(), pEvents);
  if ( _iter == m_events.end() )
    return false;

  m_events.erase(_iter);

  return true;
}

const std::list<WindowEvents*>&   Window::get_connections() const noexcept
{
  return m_events;
}

#ifdef __EMSCRIPTEN__
void  Window::get_framebuffer_size( Size& size ) noexcept
{
  size.width  = canvas_get_width();
  size.height = canvas_get_height();
}
#endif

bool  Window::process_message() noexcept
{
  Message* pMessage = nullptr;

  core::result_t res = m_mbxMessages.read( pMessage, 0 );
  if ( res != core::result_t::eSuccess )
    return false;
    
  pMessage->execute();
  if ( pMessage->release() )
    delete pMessage;
  
  return true;
}

bool  Window::send_message( Message* pMessage ) noexcept
{
  if ( pMessage == nullptr )
    return false;

  pMessage->event();
  
  m_mbxMessages.write( pMessage );
  
  // Wait until event will be processed.
  pMessage->wait();
  
  delete pMessage;
  
  return true;
}

bool  Window::post_message( Message* pMessage ) noexcept
{
  if ( pMessage == nullptr )
    return false;
  
  m_mbxMessages.write( pMessage );
  
  return true;
}




}

