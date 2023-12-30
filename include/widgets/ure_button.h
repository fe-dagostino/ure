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

#ifndef URE_BUTTON_H
#define URE_BUTTON_H

#include "ure_common_defs.h"
#include "ure_label.h"

namespace ure {

namespace widgets {

/**
 */
class Button : public Label
{
public:

  /***/
  Button( Widget* pParent ) noexcept;
  /***/
  virtual ~Button() noexcept
  {}  
  
  /***/
  inline const std::wstring& get_caption() const noexcept
  { return Label::get_label();  }
  /***/
  inline const glm::vec4&    get_foreground() const noexcept
  { return Label::get_foreground(); }
  
  /***/
  bool                       set_caption( Font* pFont, const std::wstring& sCaption, const glm::vec4& fgColor, WidgetTextAligment align );
  
  /***/
  void_t                     set_focus( Texture* texture );
  
  /***/
  virtual bool               is_focusable() const noexcept override
  { return true; }
  
protected:
  /***/
  virtual bool    onDrawBackground( [[maybe_unused]] const Recti& rect ) noexcept override;
  /***/
  virtual bool    onDraw( [[maybe_unused]] const Recti& rect ) noexcept override;
  /***/
  virtual void_t  onPositionChanged( [[maybe_unused]] const Position& pos ) noexcept override;
  /***/
  virtual void_t  onSizeChanged( [[maybe_unused]] const Size& size ) noexcept override;

private:
  std::shared_ptr<Texture>  m_imFocus;
  
  std::vector<glm::vec2>    m_vVertices;
  std::vector<glm::vec2>    m_vTexCoord;    
};

}

}

#endif // URE_BUTTON_H
