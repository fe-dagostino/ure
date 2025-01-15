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

#include "ure_view_port.h"

#if defined(_IMGUI_ENABLED)
# include "imgui.h"
# include "imgui_impl_glfw.h"
# include "imgui_impl_opengl3.h"
#endif

namespace ure {

void ViewPort::use() noexcept
{
  glViewport( m_pos.x, m_pos.y, m_size.width, m_size.height );

#if defined(_IMGUI_ENABLED)
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
#endif  
}

void ViewPort::reset() noexcept
{
  int_t viewport[4];	
  glGetIntegerv(GL_VIEWPORT, viewport);
  
  set_area( viewport[0], viewport[1], viewport[2], viewport[3] );
}

void ViewPort::clear_buffer( GLbitfield mask ) noexcept
{
  glClear( mask );
}

bool_t       ViewPort::render() noexcept(true)
{
  if ( m_scene_graph == nullptr )
    return false;
  
  bool_t _retval = m_scene_graph->render( get_projection_matrix().get() );

#if defined(_IMGUI_ENABLED)
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

  return _retval;
}


}
