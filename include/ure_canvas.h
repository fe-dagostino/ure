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

#ifndef URE_CANVAS_H
#define URE_CANVAS_H

#include "ure_object.h"
#include "ure_text.h"

#include <vector>

namespace ure {

class Program;

class Canvas : public Object
{
public:
  /***/
  Canvas() noexcept;
  /***/
  virtual ~Canvas() noexcept;
  
  /***/
  constexpr void_t           set_mvp( const glm::mat4& mvp ) noexcept
  { m_mvp = mvp;  }
  /***/
  constexpr const glm::mat4& get_mvp() const noexcept
  { return m_mvp; }
  
  /***/
  void_t  draw_points( const std::vector<glm::vec2>& points, const glm::vec4& color, float_t fThickness = 1.0f ) noexcept;
  /***/
  void_t  draw_lines ( const std::vector<glm::vec2>& points, const glm::vec4& color, float_t fThickness = 1.0f ) noexcept;
  /***/
  void_t  draw_rect  ( const std::vector<glm::vec2>& points, const glm::vec4& color ) noexcept;
  /***/ 
  void_t  draw_rect  ( const std::vector<glm::vec2>& vertices, const std::vector<glm::vec2>& texCoord, Texture& texture, int_t tws, int_t twt ) noexcept;
  /***/
  void_t  draw_text  ( const std::vector<glm::vec2>& vertices, const std::vector<glm::vec2>& texCoord, const Text& text, int_t tws, int_t twt ) noexcept;
  
private:
  /***/
  void_t  draw( enum_t mode, const std::vector<glm::vec2>& points, const glm::vec4& color, float_t fThickness ) noexcept;
  /***/
  void_t  draw( const std::vector<glm::vec2>& vertices, const std::vector<glm::vec2>& texCoord, const Text& text, int_t tws, int_t twt ) noexcept;
 
private:  
  glm::mat4     m_mvp;
  
};

}

#endif // URE_CANVAS_H
