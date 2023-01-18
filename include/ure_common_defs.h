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

#ifndef URE_COMMON_DEFS_H
#define URE_COMMON_DEFS_H

#include "config.h"

#ifdef __EMSCRIPTEN__
# include <emscripten.h>
#endif

#if defined(_GLFW_ENABLED)
# define GLFW_INCLUDE_NONE 
# include <GLFW/glfw3.h>

  namespace ure {

  typedef GLFWvidmode  VideoMode;
  typedef GLFWmonitor* MonitorHandler; 
  typedef GLFWwindow*  WindowHandler;

  }
#endif

/////////////////////////////
#define GLM_FORCE_RADIANS
#define GLM_PRECISION_MEDIUMP_FLOAT
#include <glm/glm.hpp>


#if defined(_GLES_ENABLED) || defined(_OGL2_ENABLED) || defined(_OGL3_ENABLED)

# if defined(_GLES_ENABLED)
#  include <glad/gles2.h>
# else
#  include <glad/gl.h>
# endif

#define URE_VERTEX_SHADER     GL_VERTEX_SHADER
#define URE_FRAGMENT_SHADER   GL_FRAGMENT_SHADER

#define URE_RGB               GL_RGB
#define URE_RGBA              GL_RGBA

#define URE_UNSIGNED_BYTE     GL_UNSIGNED_BYTE

namespace ure {
  using char_t     = GLchar;
  using enum_t     = GLenum;
  using int_t      = GLint;
  using uint_t     = GLuint;
  using float_t    = GLfloat;
  using double_t   = GLdouble;
  using sizei_t    = GLsizei;
  using word_t     = GLushort;
  using void_t     = GLvoid;
}

#endif

#if defined(_VULKAN_ENABLED)
# define GLAD_VULKAN_IMPLEMENTATION
# include <glad/vulkan.h>
#endif


/////////////////////////////

#include <iostream>
#include <string>
#include <memory>

#endif //URE_COMMON_DEFS_H