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

#ifndef URE_FREE_TYPE_FONT_H
#define URE_FREE_TYPE_FONT_H

#include "ure_common_defs.h"
#include "ure_font.h"

/**
 *  Forward declaration will avoid FT includes.
 */
struct FT_FaceRec_;
typedef struct FT_FaceRec_*  FT_Face;

namespace ure {

namespace font {

class FreeTypeFont final : public Font
{
private:
  FreeTypeFont()
    : m_face(nullptr), m_size(0,0), 
      m_left(0), m_top(0), m_right(0), m_bottom(0)
  {}
public:
  /***/
  FreeTypeFont( FT_Face face, const Size& size );
  /***/
  virtual ~FreeTypeFont();
  
  /**
   * Retrieve font size, in pixels, for both width and height.
   */
  virtual const Size&       get_size() const noexcept override;
  /**
   * Will set font size in pixels.
   * if only one values will be specified and the other one is zero,
   * it will be considered as a square size.
   */
  virtual void_t            set_size( const Size& size ) noexcept override;
  /***/
  virtual void_t            get_margins( sizei_t& left, sizei_t& top, sizei_t& right, sizei_t& bottom ) const noexcept override;
  /***/
  virtual void_t            set_margins( sizei_t  left, sizei_t  top, sizei_t  right, sizei_t  bottom ) noexcept override;
  /**
   * @param sText   widechar text used to generate GLText instance.
   * @param frColor foreground  color.
   */
  virtual Text*             get_text( const std::wstring& sText, const glm::vec4& frColor ) override;
 
protected:
  /***/
  FT_Face                   get_face() const noexcept
  { return m_face; }
  
private:
  FT_Face  m_face;
  Size     m_size;
  sizei_t  m_left, m_top, m_right, m_bottom;

  friend class FreeTypeFontLoader; // Required for the private constructor.
};

}

}

#endif // URE_FREE_TYPE_FONT_H
