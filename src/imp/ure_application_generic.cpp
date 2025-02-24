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

#ifdef _USE_WEBSOCKETS
# include <libwebsockets.h>
#endif

namespace ure {

#ifdef _USE_WEBSOCKETS

struct lws_context*            g_context =  nullptr;

extern struct lws_protocols    g_protocols[];


void_t Application::on_initialize_ws() noexcept(true)
{
  struct lws_context_creation_info info;
  memset(&info, 0, sizeof(info));

  info.port      = CONTEXT_PORT_NO_LISTEN; // we do not run any server
  info.protocols = g_protocols;
  info.gid       = -1;
  info.uid       = -1;

  g_context = lws_create_context(&info);
}

void_t Application::on_finalize_ws() noexcept(true)
{
  if ( g_context != nullptr )
  {
    lws_context_destroy( g_context );
  }
}

void_t Application::processing_ws() noexcept(true)
{
  lws_service(g_context, -1 );
}

#endif /* _USE_WEBSOCKETS */

}
