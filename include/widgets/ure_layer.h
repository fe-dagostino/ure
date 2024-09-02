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

#ifndef URE_LAYER_H
#define URE_LAYER_H

#include "widgets/ure_widget.h"
#include "ure_view_port.h"
#include "ure_window_events.h"

namespace ure {

namespace widgets {

class Layer : public Widget, public WindowEvents
{
public:
  /***/
  Layer( ViewPort& rViewPort ) noexcept(true);
  /***/
  virtual ~Layer() noexcept(true);
    
  constexpr WindowEvents* get_windows_events() noexcept(true) 
  { return static_cast<WindowEvents*>(this); }

  /***/
  bool_t          render( const glm::mat4& mvp ) noexcept(true);
  
/// Implements WindowEvents
protected:
  
  /**
   * Called in just before windows creation.
   * It will be a nice place to see own window hints
   */
  virtual void_t  on_creating( Window* pWindow ) noexcept(true) override; 
  /***/
  virtual void_t  on_created( [[maybe_unused]] Window* pWindow ) noexcept(true) override;
  /**
   * Called to notify new window position.
   */
  virtual void_t  on_position_changed( Window* pWindow, int_t iPosX, int_t iPosY ) noexcept(true) override;
  /**
   * Called to notify new window size.
   */
  virtual void_t  on_size_changed( Window* pWindow, sizei_t iWidth, sizei_t iHeight ) noexcept(true) override;
  /**
   * Called to notify new window size.
   */
  virtual void_t  on_fb_size_changed( Window* pWindow, sizei_t iWidth, sizei_t iHeight ) noexcept(true) override;
  /**
   * Called when user attempt to close the window.
   */
  virtual void_t  on_close( Window* pWindow ) noexcept(true) override;
  /**
   * Called when the window whose content needs to be refreshed.
   */
  virtual void_t  on_refresh( Window* pWindow ) noexcept(true) override;
  /***/  
  virtual void_t  on_got_focus( Window* pWindow ) noexcept(true) override;
  /***/
  virtual void_t  on_lost_focus( Window* pWindow ) noexcept(true) override;
  /***/  
  virtual void_t  on_iconified( Window* pWindow ) noexcept(true) override;
  /***/
  virtual void_t  on_restored( Window* pWindow ) noexcept(true) override;
  /***/
  virtual void_t  on_mouse_button_pressed( Window* pWindow, mouse_button_t button, int_t mods ) noexcept(true) override;
  /***/
  virtual void_t  on_mouse_button_released( Window* pWindow, mouse_button_t button, int_t mods ) noexcept(true) override;
  /***/
  virtual void_t  on_mouse_move( Window* pWindow, double_t dPosX, double_t dPosY ) noexcept(true) override;
  /***/
  virtual void_t  on_mouse_enter( Window* pWindow ) noexcept(true) override;
  /***/
  virtual void_t  on_mouse_leave( Window* pWindow ) noexcept(true) override;
  /***/
  virtual void_t  on_mouse_scroll( Window* pWindow, double_t dOffsetX, double_t dOffsetY ) noexcept(true) override;
  /***/
  virtual void_t  on_key_released( Window* pWindow, key_t key, int_t iScanCode, word_t wMods ) noexcept(true) override;
  /**
   * Default implementation will close widows when ESCAPE key went pressed.
   */
  virtual void_t  on_key_pressed ( Window* pWindow, key_t key, int_t iScanCode, word_t wMods ) noexcept(true) override;  
  /***/
  virtual void_t  on_key_repeated( Window* pWindow, key_t key, int_t iScanCode, word_t wMods ) noexcept(true) override;
  /***/
  virtual void_t  on_unicode_char( Window* pWindow, uint_t iCodePoint ) noexcept(true) override;
  
private:
  
};

}

}

#endif // URE_LAYER_H
