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

#ifndef URE_WINDOW_EVENTS_H
#define URE_WINDOW_EVENTS_H

#include "ure_common_defs.h"

namespace ure {

class Window;

class WindowEvents 
{
public:

  enum class mouse_button_t : int_t {
    BUTTON_1      = 0,
    BUTTON_2      = 1,
    BUTTON_3      = 2,
    BUTTON_4      = 3,
    BUTTON_5      = 4,
    BUTTON_6      = 5,
    BUTTON_7      = 6,
    BUTTON_8      = 7,
    BUTTON_LAST   = BUTTON_8,
    BUTTON_LEFT   = BUTTON_1,
    BUTTON_RIGHT  = BUTTON_2, 
    BUTTON_MIDDLE = BUTTON_3,
  };

  enum class mouse_modifier_key_flag_t : int_t {
    SHIFT         = 0x0001,   /*  One or more Shift keys were held down. */
    CONTROL       = 0x0002,   /*  One or more Control keys were held down. */
    ALT           = 0x0004,   /*  One or more Alt keys were held down. */
    SUPER         = 0x0008,   /*  One or more Super keys were held down. */
    CAPS_LOCK     = 0x0010,   /*  Caps Lock key is enabled. */
    NUM_LOCK      = 0x0020    /*  Num Lock key is enabled. */
  };

  /***/
  WindowEvents() noexcept
  {}

  /***/
  virtual ~WindowEvents() noexcept
  {}
  
  /**
   * Called in just before windows creation.
   * It will be a nice place to see own window hints
   */
  virtual void_t  on_creating( [[maybe_unused]] Window* pWindow ) noexcept;
  /***/
  virtual void_t  on_created( [[maybe_unused]] Window* pWindow ) noexcept;
  /**
   * Called to notify new window position.
   */
  virtual void_t  on_position_changed( [[maybe_unused]] Window* pWindow, [[maybe_unused]] int_t iPosX, [[maybe_unused]] int_t iPosY ) noexcept
  {}
  /**
   * Called to notify new window size.
   */
  virtual void_t  on_size_changed( [[maybe_unused]] Window* pWindow, [[maybe_unused]] sizei_t iWidth, [[maybe_unused]] sizei_t iHeight ) noexcept
  {}
  /**
   * Called to notify new window size.
   */
  virtual void_t  on_fb_size_changed( [[maybe_unused]] Window* pWindow, [[maybe_unused]] sizei_t iWidth, [[maybe_unused]] sizei_t iHeight ) noexcept
  {}
  /**
   * Called when user attempt to close the window.
   */
  virtual void_t  on_close( [[maybe_unused]] Window* pWindow ) noexcept
  {}
  /**
   * Called when the window whose content needs to be refreshed.
   */
  virtual void_t  on_refresh( [[maybe_unused]] Window* pWindow ) noexcept
  {}
  /***/  
  virtual void_t  on_got_focus( [[maybe_unused]] Window* pWindow ) noexcept
  {}
  /***/
  virtual void_t  on_lost_focus( [[maybe_unused]] Window* pWindow ) noexcept
  {}
  /***/  
  virtual void_t  on_iconified( [[maybe_unused]] Window* pWindow ) noexcept
  {}
  /***/
  virtual void_t  on_restored( [[maybe_unused]] Window* pWindow ) noexcept
  {}
  /***/
  virtual void_t  on_mouse_button_pressed( [[maybe_unused]] Window* pWindow, [[maybe_unused]] mouse_button_t button, [[maybe_unused]] int_t mods ) noexcept
  {}
  /***/
  virtual void_t  on_mouse_button_released( [[maybe_unused]] Window* pWindow, [[maybe_unused]] mouse_button_t button, [[maybe_unused]] int_t mods ) noexcept
  {}
  /***/
  virtual void_t  on_mouse_move( [[maybe_unused]] Window* pWindow, [[maybe_unused]] double_t dPosX, [[maybe_unused]] double_t dPosY ) noexcept
  {}
  /***/
  virtual void_t  on_mouse_enter( [[maybe_unused]] Window* pWindow ) noexcept
  {}
  /***/
  virtual void_t  on_mouse_leave( [[maybe_unused]] Window* pWindow ) noexcept
  {}
  /***/
  virtual void_t  on_mouse_scroll( [[maybe_unused]] Window* pWindow, [[maybe_unused]] double_t dOffsetX, [[maybe_unused]] double_t dOffsetY ) noexcept
  {}
  /***/
  virtual void_t  on_key_released( [[maybe_unused]] Window* pWindow, [[maybe_unused]] int_t iKey, [[maybe_unused]] int_t iScanCode, [[maybe_unused]] word_t wMods ) noexcept
  {}
  /**
   * Default implementation will close widows when ESCAPE key went pressed.
   */
  virtual void_t  on_key_pressed ( [[maybe_unused]] Window* pWindow, [[maybe_unused]] int_t iKey, [[maybe_unused]] int_t iScanCode, [[maybe_unused]] word_t wMods ) noexcept;  
  /***/
  virtual void_t  on_key_repeated( [[maybe_unused]] Window* pWindow, [[maybe_unused]] int_t iKey, [[maybe_unused]] int_t iScanCode, [[maybe_unused]] word_t wMods ) noexcept 
  {}
  /***/
  virtual void_t  on_unicode_char( [[maybe_unused]] Window* pWindow, [[maybe_unused]] uint_t iCodePoint ) noexcept 
  {}

};

}

#endif // URE_WINDOW_EVENTS_H
