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

#include "ure_renderer.h"
#include "core/utils.h"

namespace ure {


bool_t    Renderer::get_vendor  ( std::string& sVendor   ) const noexcept
{
  const GLubyte*  pRet = glGetString(GL_VENDOR);
  if ( pRet == nullptr )
    return false;
  
  sVendor = core::utils::format( "%s", pRet );

  return true;
}

bool_t    Renderer::get_renderer( std::string& sRenderer ) const noexcept
{
  const GLubyte*  pRet = glGetString(GL_RENDERER);
  if ( pRet == nullptr )
    return false;
  
  sRenderer = core::utils::format( "%s", pRet );

  return true;  
}
  
bool_t    Renderer::get_version( std::string& sVersion ) const noexcept
{
  const GLubyte*  pRet = glGetString(GL_VERSION);
  if ( pRet == nullptr )
    return false;
  
  sVersion = core::utils::format( "%s", pRet );

  return true;  
}

bool_t    Renderer::get_shader_version( std::string& sShaderVersion ) const noexcept
{
  const GLubyte*  pRet = glGetString(GL_SHADING_LANGUAGE_VERSION);
  if ( pRet == nullptr )
    return false;
  
  sShaderVersion = core::utils::format( "%s", pRet );

  return true;  
}

}