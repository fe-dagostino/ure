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

#include "widgets/ure_widget.h"
#include "ure_view_port.h"


namespace ure {

namespace widgets {


Widget::Widget( Widget* pParent ) noexcept(true)
 : m_ebo( eboUndefined ), m_pos( 0, 0 ),
   m_size( 0, 0 ), m_visible( true ), m_enabled( true ),
   m_pParent( nullptr ), m_eBackground( NoBackground ), m_bkg_texture( nullptr )
{
  m_Focus = m_vChildren.end();
  
  set_parent( pParent );
}

Widget::~Widget() noexcept(true)
{
  // Release signals slot on parent.
  set_parent( nullptr );
}

bool  Widget::add_child( std::unique_ptr<Widget> widget ) noexcept(true)
{
  m_vChildren.push_back( std::move(widget) );
  m_vChildren.shrink_to_fit();

  // Update parent 
  widget->set_parent(this);
  
  // Iterator must be updated each time 
  // m_vChildren become changed
  m_Focus = m_vChildren.end();
  
  return true;
}

bool_t  Widget::has_focus() const noexcept(true)
{
  if ( m_pParent == nullptr )
    return false;

  if ( m_pParent->m_Focus != m_pParent->m_vChildren.end() )
  {
    if ( m_pParent->m_Focus->get() == this )
      return true;
  }

  return false;
}

void_t  Widget::set_position( int_t x, int_t y, bool notify ) noexcept(true)
{ 
  if ( (m_pos.x != x) || (m_pos.y != y) )
  {
    m_pos.x = x; 
    m_pos.y = y; 
  
    _updateBkVertices();
  }
  
  if ( notify == true )
  {
    on_widget_position_changed( m_pos );
  }
}
  
void_t  Widget::set_size( sizei_t width, sizei_t height, bool notify ) noexcept(true)
{ 
  if (( m_size.width != width ) || (m_size.height != height))
  {
    m_size.width  = width; 
    m_size.height = height; 

    _updateBkVertices();
  }
  
  if ( notify == true )
  {
    on_widget_size_changed( m_size );
  }
}  
  
void_t  Widget::set_background( std::shared_ptr<ure::Texture> texture, BackgroundOptions bo ) noexcept(true)
{ 
  m_bkg_texture = texture;     
  
  m_eBackground  = ImageBrush;
  
  if ( m_ebo != bo )
  {
    // Default Texture coordinates
    m_bkTexCoord.clear();
    m_bkTexCoord.reserve(4);
    switch ( bo )
    {
      case eboAsIs:
        m_bkTexCoord.push_back( glm::vec2( 0.0f, 1.0f )  );
        m_bkTexCoord.push_back( glm::vec2( 1.0f, 1.0f )  );
        m_bkTexCoord.push_back( glm::vec2( 0.0f, 0.0f )  );
        m_bkTexCoord.push_back( glm::vec2( 1.0f, 0.0f )  );
      break;
      case eboFlipHorizontal:
        m_bkTexCoord.push_back( glm::vec2( 1.0f, 1.0f )  );
        m_bkTexCoord.push_back( glm::vec2( 0.0f, 1.0f )  );
        m_bkTexCoord.push_back( glm::vec2( 1.0f, 0.0f )  );
        m_bkTexCoord.push_back( glm::vec2( 0.0f, 0.0f )  );
      break;
      case eboFlipVertical:
        m_bkTexCoord.push_back( glm::vec2( 0.0f, 0.0f )  );
        m_bkTexCoord.push_back( glm::vec2( 1.0f, 0.0f )  );
        m_bkTexCoord.push_back( glm::vec2( 0.0f, 1.0f )  );
        m_bkTexCoord.push_back( glm::vec2( 1.0f, 1.0f )  );
      break;
      case eboFlipBoth:
        m_bkTexCoord.push_back( glm::vec2( 1.0f, 0.0f )  );
        m_bkTexCoord.push_back( glm::vec2( 0.0f, 0.0f )  );
        m_bkTexCoord.push_back( glm::vec2( 1.0f, 1.0f )  );
        m_bkTexCoord.push_back( glm::vec2( 0.0f, 1.0f )  );
      break;
      default:
      break;
    }
    
    m_ebo = bo;
  }
}

bool_t  Widget::draw( const glm::mat4& mvp, const Recti& rect ) noexcept(true)
{
  if ( is_visible() == false )
    return false;
    
  Canvas::set_mvp( mvp );
  
  on_widget_begin_drawing( rect );
  
  if ( on_widget_draw_background( rect ) == true )
  {
    if ( m_eBackground != NoBackground )
    {
      if ( m_eBackground == SolidColor )
      {
        draw_rect( m_bkVertices, get_bk_color() );
      }
  
      if ( ( m_eBackground == ImageBrush ) && (m_bkg_texture.get() != nullptr) )
      {
        draw_rect( m_bkVertices, m_bkTexCoord, *m_bkg_texture.get(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
      }
    }
  }
  
  for( auto& w : m_vChildren )
  {
    w->draw( mvp, rect );
  }
  
  bool bRetVal = on_widget_draw( rect );
  
  on_widget_end_drawing( rect );
  
  return bRetVal;
}

void_t  Widget::on_widget_key_released( Window* pWindow, Layer* pLayer, key_t key, int_t iScanCode, word_t wMods ) noexcept(true)
{ 
  if (( has_focus() == false ) || ( is_enabled() == false ))
    return;
    
  signalKeyReleased.emit( pWindow, pLayer, key, iScanCode, wMods );   
} 
  
void_t  Widget::on_widget_key_pressed ( Window* pWindow, Layer* pLayer, key_t key, int_t iScanCode, word_t wMods ) noexcept(true)
{
  if (( has_focus() == false ) || ( is_enabled() == false ))
    return;
    
  handleDefaultKeys( key, iScanCode );
  
  signalKeyPressed.emit ( pWindow, pLayer, key, iScanCode, wMods ); 
} 

void_t  Widget::on_widget_key_repeated( Window* pWindow, Layer* pLayer, key_t key, int_t iScanCode, word_t wMods ) noexcept(true)
{ 
  if (( has_focus() == false ) || ( is_enabled() == false ))
    return;
  
  handleDefaultKeys( key, iScanCode );
  
  signalKeyRepeated.emit( pWindow, pLayer, key, iScanCode, wMods );  
} 

bool  Widget::step_focus() noexcept(true)
{
  if ( has_focusable() == false )
    return true;
  
  // Will restart from first widget
  if ( m_Focus == m_vChildren.end() )
  {
    m_Focus = m_vChildren.begin();
  }
  else
  {
    if ( (*m_Focus)->step_focus() == true )
      m_Focus++;
  }

  // Move to the first focusable item. It can be also the last item when the loop is ended.
  while ( ( m_Focus != m_vChildren.end()        ) && 
          (( (*m_Focus)->is_focusable() == false ) || 
	         ( (*m_Focus)->is_enabled()   == false ) ||
	         ( (*m_Focus)->is_visible()   == false ))
	)
  {
    m_Focus++;
  }
  
  if ( m_Focus == m_vChildren.end() )
    return step_focus();
  
  return false;
}

bool  Widget::has_focusable() noexcept(true)
{
  if ( m_vChildren.empty() )
    return false;
  
  for ( auto& w : m_vChildren )
    if ( w->is_focusable() == true )
      return true;
  
  return false;
}

// Called only in case current widget is both visible and enabled.
void_t  Widget::handleDefaultKeys( key_t key, [[maybe_unused]] int_t iScanCode ) noexcept(true)
{
  switch ( key )
  {
    case key_t::KEY_ENTER :  // ENTER KEY
    {
      // Raise on click event as for mouse when 
      signalOnClick.emit( this );
    }; break;
    default:
    break;
  }
}

void_t  Widget::_updateBkVertices( ) noexcept(true)
{
  if ( on_widget_update_background_vertices() == false )
    return;
    
  // Remove all vertices
  m_bkVertices.clear();
  m_bkVertices.reserve(4);
  
  // Default Vertices coordinates 
  Position   pos  = get_position();
  Size       size = get_size();
  
  if (get_parent()!=nullptr)
  {
    pos += get_parent()->get_position();
  }
  
  m_bkVertices.push_back( glm::vec2( pos.x             , pos.y + size.height ) );
  m_bkVertices.push_back( glm::vec2( pos.x + size.width, pos.y + size.height ) );
  m_bkVertices.push_back( glm::vec2( pos.x             , pos.y               ) );
  m_bkVertices.push_back( glm::vec2( pos.x + size.width, pos.y               ) );
}


void_t  Widget::set_parent( Widget* pParent ) noexcept(true)
{
  if ( m_pParent != nullptr )
  {
    slotKeyReleased.disconnect();
    slotKeyPressed.disconnect();
    slotKeyRepeated.disconnect();
  }
  
  m_pParent = pParent;
  
  if ( m_pParent != nullptr )
  {
    slotKeyReleased = m_pParent->signalKeyReleased.connect( sigc::mem_fun(*this, &Widget::on_widget_key_released) );
    slotKeyPressed  = m_pParent->signalKeyPressed.connect ( sigc::mem_fun(*this, &Widget::on_widget_key_pressed ) );
    slotKeyRepeated = m_pParent->signalKeyRepeated.connect( sigc::mem_fun(*this, &Widget::on_widget_key_repeated) );
  }
  
  _updateBkVertices();
}

}

}
