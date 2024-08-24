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

#ifndef URE_RENDERER_H
#define URE_RENDERER_H

#include "ure_common_defs.h"

namespace ure {

class Window;

class Renderer final
{
private:
  /***/
  constexpr Renderer() noexcept
  {}

public:
  /***/
  constexpr ~Renderer() noexcept
  {}

  /**
   * Returns the company responsible for this Render implementation. This name does not 
   * change from release to release.
   * Return true if function succeeded, false otherwise
   */
  bool_t        get_vendor  ( std::string& sVendor   ) const noexcept;
  /**
   * Returns the name of the renderer. This name is typically specific to a particular 
   * configuration of a hardware platform. It does not change from release to release.
   * Return true if function succeeded, false otherwise
   */
  bool_t        get_renderer( std::string& sRenderer ) const noexcept;

  /**
   * Returns a version or release number of the form 
   * OpenGL<space>ES<space><version number><space><vendor-specific information>.
   * Return true if function succeeded, false otherwise
   */
  bool_t        get_version( std::string& sVersion ) const noexcept;
  /**
   * Returns a version or release number for the shading language of the form 
   * OpenGL<space>ES<space>GLSL<space>ES<space><version number><space><vendor-specific information>.
   * Return true if function succeeded, false otherwise
   */
  bool_t        get_shader_version( std::string& sShaderVersion ) const noexcept;
  
private:

  friend class Window;
};

}

#endif // #define URE_RENDERER_H

