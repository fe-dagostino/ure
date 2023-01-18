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

#ifndef URE_WINDOW_H
#define URE_WINDOW_H

#include "ure_object.h"
#include "ure_renderer.h"
#include "ure_window_options.h"
#include "ure_window_events.h"
#include "ure_message.h"
#include "ure_size.h"
#include "ure_position.h"
#include <memory>

#include <mailbox.h>

namespace ure {

/**
 * Window class represent a Window on host operating system.
 * It can be a simple window or the whole screen when in full
 * screen mode.
 */
class Window : public Object
{
public:
  enum class WindowFlags : enum_t
  {
    eWindowShouldClose = 0x00000001
  };
  
  enum class ProcessingFlags : enum_t
  {
    epfCalling = 0x0000,    // Call will be executed by calling thread.    
    epfEnqueue = 0x0001,    // Call will be enqueued in order to be processed by @processMessage method
    epfWait    = 0x0002     // If specified will cause to wait for the message to be processed. This must be 
                            // used together with epfEnqueue
  };
  
  /***/
  Window();

  /***/
  virtual ~Window();
  
  /**
   * Initialize events interface in order to raise window events.
   * This function should be called before create in order to handle 
   * also creation related events.
   */
  bool                                  connect( WindowEvents* pEvents ) noexcept;
  /***/
  const std::vector<WindowEvents*>&     get_connections() const noexcept;
  
  /***/
  bool             create( std::unique_ptr<window_options> options, enum_t flags = static_cast<enum_t>(ProcessingFlags::epfCalling) );
  /***/
  void_t           make_context_current() noexcept;
  /***/
  const Renderer*  get_renderer() const noexcept
  { return m_ptrRenderer.get(); }

  /***/
  bool             show( enum_t flags = static_cast<enum_t>(ProcessingFlags::epfCalling) ) noexcept;
  /***/
  bool             hide( enum_t flags = static_cast<enum_t>(ProcessingFlags::epfCalling) ) noexcept;
  /**
   * Return true if windows is in windowed mode or false if window is in full screen mode. 
   */
  bool             is_windowed() const noexcept;
  /***/
  bool             set_title( const std::string& sTitle, enum_t flags = static_cast<enum_t>(ProcessingFlags::epfCalling) ) noexcept;
  /***/
  bool             set_position( const position_t<int_t>& position, enum_t flags = static_cast<enum_t>(ProcessingFlags::epfCalling) ) noexcept;
  /***/
  bool             set_size( const Size& size, enum_t flags = static_cast<enum_t>(ProcessingFlags::epfCalling) ) noexcept;
  /***/
  void_t           get_framebuffer_size( Size& size ) noexcept;
  /***/
  bool             show_normal( enum_t flags = static_cast<enum_t>(ProcessingFlags::epfCalling) ) noexcept;
  /***/
  bool             show_minimized( enum_t flags = static_cast<enum_t>(ProcessingFlags::epfCalling) ) noexcept;
  /**
   *  It is possible to select the minimum number of screen updates the driver should wait 
   *  before swapping buffers:
   */
  void_t           set_swap_interval( int iRefresh ) noexcept;
  /***/
  void_t           swap_buffers() noexcept;
  /***/
  void_t           close() noexcept;
  /***/
  bool             destroy( enum_t flags = static_cast<enum_t>(ProcessingFlags::epfCalling) ) noexcept;
  /***/
  bool             get_input_mode( int mode, int& value ) noexcept;
  /***/
  bool             set_input_mode( int mode, int value  ) noexcept;
  
  /***/
  bool             get_cursor_position( position_t<double>& position ) noexcept;
  /***/
  bool             set_cursor_position( const position_t<double>& position ) noexcept;
  
  /** 
   */
  bool             check( WindowFlags flags ) noexcept;
  
  /**
   * Return true if a message has been processed, false if message queue was empty.
   */
  bool             process_message() noexcept;
  
  /**
   */  
  bool             send_message( Message* pMessage ) noexcept;
  /**
   */
  bool             post_message( Message* pMessage ) noexcept;
  
protected:
  /***/
  void_t           set_window_hint( int_t iTarget, int_t iHint ) noexcept;
  
private:
  /***/
  static void_t pos_callback         (WindowHandler window, int_t xpos, int_t ypos );
  /***/
  static void_t size_callback        (WindowHandler window, int_t width, int_t height );
  /***/
  static void_t close_callback       (WindowHandler window );
  /***/
  static void_t refresh_callback     (WindowHandler window );
  /***/
  static void_t focus_callback       (WindowHandler window, int_t focus );
  /***/
  static void_t iconify_callback     (WindowHandler window, int_t iconified );
  /***/
  static void_t key_callback         (WindowHandler window, int_t key, int_t scancode, int_t action, int_t mods);
  /***/
  static void_t char_callback        (WindowHandler window, uint_t codepoint );
  /***/
  static void_t fbsize_callback      (WindowHandler window, int_t width, int_t height );
  /***/
  static void_t mouse_button_callback(WindowHandler window, int_t button, int_t action, int_t mods );
  /***/
  static void_t mouse_pos_callback   (WindowHandler window, double_t xpos, double_t ypos );
  /***/
  static void_t mouse_enter_callback (WindowHandler window, int_t entered );
  /***/
  static void_t mouse_scroll_callback(WindowHandler window, double_t xoffset, double_t yoffset );

private:
  /***/
  void_t  set_callbacks( bool bRegister );
  
protected:
  std::vector<WindowEvents*>       m_vEvents;
  
private:
  using mailbox_type = lock_free::mailbox<Message*, core::ds_impl_t::mutex, 0, 
                                          core::arena_allocator<core::node_t<Message*,false,true,false>, uint32_t, 1024, 1024, 0, 0, core::default_allocator<uint32_t>>
                                         >;

  std::unique_ptr<window_options>  m_ptrWinOptions;
  WindowHandler                    m_hWindow;
  std::unique_ptr<Renderer>        m_ptrRenderer;
  mailbox_type                     m_mbxMessages;  
};

}

#endif // URE_WINDOW_H