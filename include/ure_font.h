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

#ifndef URE_FONT_H
#define URE_FONT_H

#include "ure_common_defs.h"
#include "ure_object.h"
#include "ure_size.h"

namespace ure {

class Text;

/**
 */
class Font : public Object
{
public:
  /***/
  Font()
  {}
  /***/
  virtual ~Font()
  {}
  
  /**
   * Retrieve font size, in pixels, for both width and height.
   */
  virtual const Size&     get_size() const = 0;
  /**
   * Will set font size in pixels.
   * if only one values will be specified and the other one is zero,
   * it will be considered as a square size.
   */
  virtual void_t          set_size( const Size& size ) = 0;
  
  /**
   * Retrieve current margins in pixels on each side.
   */
  virtual void_t          get_margins( sizei_t& left, sizei_t& top, sizei_t& right, sizei_t& bottom ) const = 0;
  /**
   * Will set margins on four sides.
   */
  virtual void_t          set_margins( sizei_t  left, sizei_t  top, sizei_t  right, sizei_t  bottom ) = 0;
  
  /**
   * @param sText   widechar text used to generate Text instance.
   * @param frColor foreground  color.
   */
  virtual Text*           get_text( const std::wstring& sText, const glm::vec4& frColor ) = 0;
  
private:
  
};

}

#endif // URE_FONT_H
