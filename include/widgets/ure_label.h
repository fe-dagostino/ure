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

#ifndef URE_LABEL_H
#define URE_LABEL_H

#include "ure_common_defs.h"
#include "ure_widget.h"
#include "ure_font.h"
#include "ure_text.h"

#include <string>

namespace ure {

namespace widgets {

/***/
enum WidgetTextAligment
{
  wtaAutoResize,              // Client Area will be resized as for text requirements 
  wtaFillClientArea,          // Fill Label Rectangle using to stretch/reduce text in both direction
  wtaCentered                 // Text will keep original aspect ratio and original dimensions and its
                              // position will be relative to client area center in client coordinates.
};
  

/**
 */
class Label : public Widget
{
public:
  /***/
  Label( Widget* pParent ) noexcept(true);
  /***/
  virtual ~Label() noexcept(true)
  {}
  
  /***/
  inline constexpr const std::wstring& get_label() const noexcept(true)
  { return m_sLabel;  }
  /***/
  inline constexpr const glm::vec4&    get_foreground() const noexcept(true)
  { return m_fgColor; }
  
  /***/
  bool_t                     set_label( Font* pFont, const std::wstring& sLabel, const glm::vec4& fgColor, WidgetTextAligment align ) noexcept(true);

protected:
  /***/
  virtual bool_t   on_widget_draw( [[maybe_unused]] const Recti& rect ) noexcept(true) override;
  /***/
  virtual void_t   on_widget_position_changed( [[maybe_unused]] const Position& pos ) noexcept(true) override;
  /***/
  virtual void_t   on_widget_size_changed( [[maybe_unused]] const Size& size ) noexcept(true) override;
  /***/
  virtual bool_t   on_widget_update_background_vertices() noexcept(true) override;

private:
  /***/
  inline WidgetTextAligment getAlignment() const noexcept(true)
  { return m_eAlignment; }
  
  /***/
  void_t                    _updateVertices( WidgetTextAligment align ) noexcept(true);

private:
  std::wstring              m_sLabel;
  glm::vec4                 m_fgColor;
  WidgetTextAligment        m_eAlignment;
  Text*                     m_pText;
  
  std::vector<glm::vec2>    m_vVertices;
  std::vector<glm::vec2>    m_vTexCoord;    
};

}

}

#endif // URE_LABEL_H
