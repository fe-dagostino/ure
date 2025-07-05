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

#include "ure_window_events.h"
#include "ure_window.h"

#include "ure_config.h"


namespace ure {

void_t  WindowEvents::on_creating( [[maybe_unused]] Window* pWindow ) noexcept
{
#if defined(_GLES_ENABLED)  
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, URE_CONTEXT_VERSION_MAJOR    );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, URE_CONTEXT_VERSION_MINOR    );
  glfwWindowHint( GLFW_CLIENT_API           , GLFW_OPENGL_ES_API );
#endif  

#if defined(_OGL2_ENABLED)  
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, URE_CONTEXT_VERSION_MAJOR    );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, URE_CONTEXT_VERSION_MINOR    );
  glfwWindowHint( GLFW_CLIENT_API           , GLFW_OPENGL_ES_API );
#endif  

#if defined(_OGL3_ENABLED)  
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, URE_CONTEXT_VERSION_MAJOR    );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, URE_CONTEXT_VERSION_MINOR    );
  glfwWindowHint( GLFW_CLIENT_API           , GLFW_OPENGL_ES_API );
#endif  
}

#ifdef __EMSCRIPTEN__
EM_JS(void, resize_canvas, (), {
  js_resizeCanvas();
});
#endif

void_t  WindowEvents::on_created( [[maybe_unused]] Window* pWindow ) noexcept
{
#ifdef __EMSCRIPTEN__
  resize_canvas();
#endif
}

void_t WindowEvents::on_key_pressed( [[maybe_unused]] Window* pWindow, [[maybe_unused]] key_t key, [[maybe_unused]] int_t iScanCode, [[maybe_unused]] word_t wMods ) noexcept
{
}

void_t  WindowEvents::on_key_released( [[maybe_unused]] Window* pWindow, [[maybe_unused]] key_t key, [[maybe_unused]] int_t iScanCode, [[maybe_unused]] word_t wMods ) noexcept(true)
{
  if ( key == key_t::KEY_ESCAPE )
    pWindow->close();
}

}
