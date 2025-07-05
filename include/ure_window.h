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
#include <list>

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
  using events_type = std::list<WindowEvents*>;

  enum class window_flag_t : enum_t
  {
    eWindowShouldClose = 0x00000001
  };
  
  enum class processing_flag_t : enum_t
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
  bool_t             connect( WindowEvents* pEvents ) noexcept(true);
  /***/
  bool_t             disconnect( WindowEvents* pEvents ) noexcept(true);
  /***/
  const events_type& get_connections() const noexcept(true);
  
  /***/
  bool_t             create( std::unique_ptr<window_options> options, enum_t flags = static_cast<enum_t>(processing_flag_t::epfCalling) ) noexcept(true);
  /***/
  void_t             make_context_current() noexcept(true);
    /**
   * Return object identifier.
   */
  WindowHandler      get_handler() noexcept(true)
  { return m_hWindow; }

  operator WindowHandler() noexcept(true)
  { return m_hWindow; }
  /***/
  const Renderer*    get_renderer() const noexcept(true)
  { return m_ptrRenderer.get(); }

  /***/
  bool_t             show( enum_t flags = static_cast<enum_t>(processing_flag_t::epfCalling) ) noexcept(true);
  /***/
  bool_t             hide( enum_t flags = static_cast<enum_t>(processing_flag_t::epfCalling) ) noexcept(true);
  /**
   * Return true if windows is in windowed mode or false if window is in full screen mode. 
   */
  bool_t             is_windowed() const noexcept(true);
  /***/
  bool_t             set_title( const std::string& sTitle, enum_t flags = static_cast<enum_t>(processing_flag_t::epfCalling) ) noexcept(true);
  /***/
  bool_t             set_position( const position_t<int_t>& position, enum_t flags = static_cast<enum_t>(processing_flag_t::epfCalling) ) noexcept(true);
  /***/
  bool_t             set_size( const Size& size, enum_t flags = static_cast<enum_t>(processing_flag_t::epfCalling) ) noexcept(true);
  /***/
  void_t             get_framebuffer_size( Size& size ) noexcept(true);
  /***/
  bool_t             show_normal( enum_t flags = static_cast<enum_t>(processing_flag_t::epfCalling) ) noexcept(true);
  /***/
  bool_t             show_minimized( enum_t flags = static_cast<enum_t>(processing_flag_t::epfCalling) ) noexcept(true);
  /**
   *  It is possible to select the minimum number of screen updates the driver should wait 
   *  before swapping buffers:
   */
  void_t             set_swap_interval( int iRefresh ) noexcept(true);
  /***/
  void_t             swap_buffers() noexcept(true);
  /***/
  void_t             close() noexcept(true);
  /***/
  bool_t             destroy( enum_t flags = static_cast<enum_t>(processing_flag_t::epfCalling) ) noexcept(true);
  /***/
  bool_t             get_input_mode( int mode, int& value ) noexcept(true);
  /***/
  bool_t             set_input_mode( int mode, int value  ) noexcept(true);
  
  /***/
  bool_t             get_cursor_position( position_t<double>& position ) noexcept(true);
  /***/
  bool_t             set_cursor_position( const position_t<double>& position ) noexcept(true);
  
  /** 
   */
  bool_t             check( window_flag_t flags ) noexcept(true);
  
  /**
   * Return true if a message has been processed, false if message queue was empty.
   */
  bool_t             process_message() noexcept(true);
  
  /**
   */  
  bool_t             send_message( Message* pMessage ) noexcept(true);
  /**
   */
  bool_t             post_message( Message* pMessage ) noexcept(true);
  
protected:
  /***/
  void_t             set_window_hint( int_t iTarget, int_t iHint ) noexcept(true);
  
private:
  /***/
  static void_t pos_callback          (WindowHandler window, int_t xpos, int_t ypos );
  /***/
  static void_t size_callback         (WindowHandler window, int_t width, int_t height );
  /***/
  static void_t close_callback        (WindowHandler window );
  /***/
  static void_t refresh_callback      (WindowHandler window );
  /***/
  static void_t focus_callback        (WindowHandler window, int_t focus );
  /***/
  static void_t iconify_callback      (WindowHandler window, int_t iconified );
  /***/
  static void_t maximize_callback     (WindowHandler window, int_t maximized );
  /***/
  static void_t key_callback          (WindowHandler window, int_t key, int_t scancode, int_t action, int_t mods);
  /***/
  static void_t char_callback         (WindowHandler window, uint_t codepoint );
  /* not supported by emscripten 3.1.64 */
  static void_t char_mods_callback    (WindowHandler window, uint_t codepoint, int_t mods );
  /***/
  static void_t fbsize_callback       (WindowHandler window, int_t width, int_t height );
  /***/
  static void_t content_scale_callback(WindowHandler window, float_t xscale, float_t yscale);
  /***/
  static void_t mouse_button_callback (WindowHandler window, int_t button, int_t action, int_t mods );
  /***/
  static void_t mouse_pos_callback    (WindowHandler window, double_t xpos, double_t ypos );
  /***/
  static void_t mouse_enter_callback  (WindowHandler window, int_t entered );
  /***/
  static void_t mouse_scroll_callback (WindowHandler window, double_t xoffset, double_t yoffset );
  /***/
  static void_t drop_path_callback    (WindowHandler window, int_t path_count, const char_t* paths[] );

private:
  /***/
  void_t  set_callbacks( bool bRegister );
  
protected:
  events_type                      m_events;
  
private:
  using mailbox_type = lock_free::mailbox<Message*, core::ds_impl_t::mutex, 0, 
                                          core::arena_allocator<core::node_t<Message*,false,true,false>, uint32_t, 1024, 1024, 0, 0, core::default_allocator<uint32_t>>
                                         >;
  
  std::string                      m_glsl_version;
  std::unique_ptr<window_options>  m_ptrWinOptions;
  WindowHandler                    m_hWindow;
  std::unique_ptr<Renderer>        m_ptrRenderer;
  mailbox_type                     m_mbxMessages;  
};

}

#endif // URE_WINDOW_H