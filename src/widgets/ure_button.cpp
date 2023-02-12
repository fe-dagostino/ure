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

#include "widgets/ure_button.h"

namespace ure {

namespace widgets {

Button::Button( Widget* pParent ) noexcept
 : Label( pParent )
{
}

bool    Button::set_caption( Font* pFont, const std::wstring& sCaption, const glm::vec4& fgColor, WidgetTextAligment align )
{
  return set_label( pFont, sCaption, fgColor, align  );
}
  
void_t  Button::set_focus( Texture* texture )
{
  m_imFocus.reset(texture);
}

bool    Button::onDrawBackground( [[maybe_unused]] const Recti& rect ) noexcept
{
  if ( has_focus() == true )
  {
    if ( m_imFocus != nullptr )
    {
      draw_rect( m_bkVertices, m_bkTexCoord, *m_imFocus, URE_CLAMP_TO_EDGE, URE_CLAMP_TO_EDGE );
    }
  }

  return true;
}

bool    Button::onDraw( const Recti& rect ) noexcept
{
  return Label::onDraw( rect );
}
  
void_t  Button::onPositionChanged( const Position2D& pos ) noexcept
{
  Label::onPositionChanged( pos );
}

void_t  Button::onSizeChanged( const Size& size ) noexcept
{
  Label::onSizeChanged( size );
}
  
}

}
