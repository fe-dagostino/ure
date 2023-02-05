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

#ifndef URE_TEXT_H
#define URE_TEXT_H

#include "ure_texture.h"

namespace ure {

/**
 */
class Text 
{
public:
  /***/
  Text( Texture* pTexture )
    : m_texture( pTexture ), m_color( 0.0f, 0.0f, 0.0f, 1.0f )
  {}
  /***/
  virtual ~Text()
  {}
  
  /**
   * Default color is black.
   */
  inline  const glm::vec4& get_color() const
  { return m_color; }
  /***/
  inline  void             set_color( const glm::vec4& color )
  { m_color = color; }
  /***/
  inline  Texture*         get_texture() const
  { return m_texture.get(); }

private:
  std::unique_ptr<Texture>    m_texture;
  glm::vec4                   m_color;
};

}

#endif // URE_TEXT_H
