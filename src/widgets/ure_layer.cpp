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

#include "widgets/ure_layer.h"


namespace ure {

namespace widgets {


Layer::Layer( ViewPort& rViewPort ) noexcept(true)
 : Widget(nullptr)
{
  int_t    x,y = 0;
  sizei_t  w,h = 0;
  
  rViewPort.get_area( x, y, w, h );
  
  set_position( x, y, false );
  set_size( w, h, false );
}

Layer::~Layer() noexcept(true)
{

}

bool_t  Layer::render( const glm::mat4& mvp ) noexcept(true)
{
  if ( is_visible() == false )
    return false;
  
  // @todo
  Recti cliRect( get_position().x, get_position().y, get_size().width, get_size().height );
  
  return draw( mvp, cliRect );
}

/// Implements GLWindowEvents
  
void_t  Layer::on_creating( [[maybe_unused]] Window* pWindow ) noexcept(true)
{}

void_t  Layer::on_created( [[maybe_unused]] Window* pWindow ) noexcept(true)
{}

void_t  Layer::on_position_changed( [[maybe_unused]] Window* pWindow, [[maybe_unused]] int_t iPosX, [[maybe_unused]] int_t iPosY ) noexcept(true)
{}

void_t  Layer::on_size_changed( [[maybe_unused]] Window* pWindow, [[maybe_unused]] sizei_t iWidth, [[maybe_unused]] sizei_t iHeight ) noexcept(true)
{}

void_t  Layer::on_fb_size_changed( [[maybe_unused]] Window* pWindow, [[maybe_unused]] sizei_t iWidth, [[maybe_unused]] sizei_t iHeight ) noexcept(true)
{}

void_t  Layer::on_close( [[maybe_unused]] Window* pWindow ) noexcept(true)
{}

void_t  Layer::on_refresh( [[maybe_unused]] Window* pWindow ) noexcept(true)
{
  if ( is_enabled() == false )
    return;
}

void_t  Layer::on_got_focus( [[maybe_unused]] Window* pWindow ) noexcept(true)
{}

void_t  Layer::on_lost_focus( [[maybe_unused]] Window* pWindow ) noexcept(true)
{}

void_t  Layer::on_iconified( [[maybe_unused]] Window* pWindow ) noexcept(true)
{
  if ( is_enabled() == false )
    return;
  
}

void_t  Layer::on_restored( [[maybe_unused]] Window* pWindow ) noexcept(true)
{
  if ( is_enabled() == false )
    return;
  
}

void_t  Layer::on_mouse_button_pressed( [[maybe_unused]] Window* pWindow, [[maybe_unused]] mouse_button_t button, [[maybe_unused]] int_t mods ) noexcept(true)
{
  if ( is_enabled() == false )
    return;
  
}

void_t  Layer::on_mouse_button_released( [[maybe_unused]] Window* pWindow, [[maybe_unused]] mouse_button_t button, [[maybe_unused]] int_t mods ) noexcept(true)
{
  if ( is_enabled() == false )
    return;
  
}

void_t  Layer::on_mouse_move( [[maybe_unused]] Window* pWindow, [[maybe_unused]] double_t dPosX, [[maybe_unused]] double_t dPosY ) noexcept(true)
{
  if ( is_enabled() == false )
    return;
  
}

void_t  Layer::on_mouse_enter( [[maybe_unused]] Window* pWindow ) noexcept(true)
{
  if ( is_enabled() == false )
    return;
  
}

void_t  Layer::on_mouse_leave( [[maybe_unused]] Window* pWindow ) noexcept(true)
{
  if ( is_enabled() == false )
    return;
  
}

void_t  Layer::on_mouse_scroll([[maybe_unused]] Window* pWindow, [[maybe_unused]] double_t dOffsetX, [[maybe_unused]] double_t dOffsetY ) noexcept(true)
{
  if ( is_enabled() == false )
    return;
  
}

void_t  Layer::on_key_released( [[maybe_unused]] Window* pWindow, key_t key, int_t iScanCode, word_t wMods ) noexcept(true)
{
  if ( is_enabled() == false )
    return;
 
  // Propagate event to all registered functions even 
  // there is no focus.
  signalKeyReleased.emit( pWindow, this, key, iScanCode, wMods );
}

void_t  Layer::on_key_pressed( Window* pWindow, key_t key, int_t iScanCode, word_t wMods ) noexcept(true)
{
  if ( is_enabled() == false )
    return;
  
  if ( key == key_t::KEY_TAB )
  {
    step_focus();
  }
  
  // Propagate event to all registered functions even 
  // there is no focus.
  signalKeyPressed.emit( pWindow, this, key, iScanCode, wMods );
}

void_t  Layer::on_key_repeated( Window* pWindow, key_t key, int_t iScanCode, word_t wMods ) noexcept(true)
{
  if ( is_enabled() == false )
    return;
  
  if ( key == key_t::KEY_TAB )
  {
    step_focus();
  }
  
  // Propagate event to all registered functions even 
  // there is no focus.
  signalKeyRepeated.emit( pWindow, this, key, iScanCode, wMods );
}

void_t  Layer::on_unicode_char( [[maybe_unused]] Window* pWindow, [[maybe_unused]] uint_t iCodePoint ) noexcept(true)
{
  if ( is_enabled() == false )
    return;
}

}

}
