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

#include "ure_application.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
# ifdef _USE_WEBSOCKETS
#  include <libwebsockets.h>
# endif
#endif

namespace ure {

void_t main_loop( void* arg )
{
  ApplicationEvents* event = static_cast<ApplicationEvents*>(arg);
  event->on_run();
}

void_t Application::run() noexcept(true)
{
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg ( main_loop, m_events.get(), 0, 1);
#else
  while ( exit() == false )
  {
    m_events->on_run();
#ifdef _USE_WEBSOCKETS
    /* Processing websockets messages */
    processing_ws();
#endif
  }
#endif
}

void_t Application::on_initialize() noexcept(true)
{
  on_initialize_wm();
#ifdef _USE_WEBSOCKETS
  on_initialize_ws();
#endif
}

void_t Application::on_finalize() noexcept(true)
{
  on_finalize_wm();
#ifdef _USE_WEBSOCKETS
  on_finalize_ws();
#endif
}

}
