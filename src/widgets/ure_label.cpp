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

#include "widgets/ure_label.h"

namespace ure {

namespace widgets {

Label::Label( Widget* pParent )
 : Widget( pParent ), m_fgColor( 0.0f, 0.0f, 0.0f, 1.0f ), 
   m_eAlignment(wtaAutoResize), m_pText( nullptr )
{
}

bool  Label::set_label( Font* pFont, const std::wstring& sLabel, const glm::vec4& fgColor, WidgetTextAligment align ) noexcept
{
  if ( pFont == nullptr )
    return false;
  
  if  ( m_pText != nullptr )
  {
    delete m_pText;
    m_pText = nullptr;
  }
  
  m_pText = pFont->get_text( sLabel, fgColor );
  if ( m_pText == nullptr )
    return false;
  
  // Default Texture coordinates
  m_vTexCoord.clear();
  m_vTexCoord.reserve(4);
  m_vTexCoord.push_back( glm::vec2( 0.0f, 0.0f )  );
  m_vTexCoord.push_back( glm::vec2( 1.0f, 0.0f )  );
  m_vTexCoord.push_back( glm::vec2( 0.0f, 1.0f )  );
  m_vTexCoord.push_back( glm::vec2( 1.0f, 1.0f )  );  
  
  _updateVertices( align );
  
  return true;
}

bool    Label::on_widget_draw( [[maybe_unused]] const Recti& rect ) noexcept
{
  if ( m_pText != nullptr )
  {
    draw_text( m_vVertices, m_vTexCoord, *m_pText, URE_CLAMP_TO_EDGE, URE_CLAMP_TO_EDGE );
  }
  
  return true;
}

void_t  Label::on_widget_position_changed( [[maybe_unused]] const Position& pos ) noexcept
{
  _updateVertices( getAlignment() );
}

void_t  Label::on_widget_size_changed( [[maybe_unused]] const Size& size ) noexcept
{
  _updateVertices( getAlignment() );
}
  
bool_t  Label::on_widget_update_background_vertices() noexcept
{
  _updateVertices( getAlignment() );
  return false;  
} 

void_t  Label::_updateVertices( WidgetTextAligment align )
{
  m_eAlignment = align;
  
  if (m_pText == nullptr )
    return;
    
  // Remove all vertices
  m_vVertices.clear();
  
  // Default Vertices coordinates 
  Position   pos = get_position();
  Size       size(0,0);
  
  if (get_parent()!=nullptr)
  {
    pos += get_parent()->get_position();
  }
  
  switch ( getAlignment() )
  {
    case wtaAutoResize :
    {
      size = m_pText->get_texture()->get_size();
      
      // Resize client area without notifications
      set_size( size.width, size.height, false );
    }; break;
    case wtaFillClientArea :
    {
      size = get_size();
    }; break; 
    case wtaCentered:
    {
      Size cSize = get_size();
      size = m_pText->get_texture()->get_size();
      
      float_t x = (float_t)pos.x + ((float_t)cSize.width  / (float_t)2) - ( (float_t)size.width  / (float_t)2);  
      float_t y = (float_t)pos.y + ((float_t)cSize.height / (float_t)2) - ( (float_t)size.height / (float_t)2);
      
      pos.x = (int_t)x;
      pos.y = (int_t)y;
    }; break; 
      
  }
  
  m_vVertices.push_back( glm::vec2( pos.x             , pos.y               ) );
  m_vVertices.push_back( glm::vec2( pos.x + size.width, pos.y               ) );
  m_vVertices.push_back( glm::vec2( pos.x             , pos.y + size.height ) );
  m_vVertices.push_back( glm::vec2( pos.x + size.width, pos.y + size.height ) );
}

}

}
