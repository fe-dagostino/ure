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

#include "ure_window.h"
#include "ure_monitor.h"
#include "ure_application.h"
#include "ure_window_messages.h"
#include "ure_utils.h"

#include <core/utils.h>

#if defined(_IMGUI_ENABLED)
# include "imgui.h"
# include "imgui_impl_glfw.h"
# include "imgui_impl_opengl3.h"
#endif


namespace ure {


bool_t Window::create( std::unique_ptr<window_options> options, enum_t flags ) noexcept(true)
{
  if ((m_hWindow != nullptr) || (options == nullptr))
    return false;

  m_ptrWinOptions = std::move(options);
  
  if ( flags & static_cast<enum_t>(processing_flag_t::epfEnqueue) )
  {
    bool_t bResult = true;
    
    Message* pMessage  = new CreateWindowMsg( this, std::move(options) );
    
    if ( flags & static_cast<enum_t>(processing_flag_t::epfWait) )
      bResult = send_message( pMessage );
    else
      bResult = post_message( pMessage );
    
    return bResult;
  }

  m_ptrRenderer = std::unique_ptr<Renderer>( new Renderer() );
  if ( m_ptrRenderer == nullptr )
    return false;

  // Full Screen monitor
  GLFWmonitor* pFsMonitor = nullptr;
  
  if ( ! m_ptrWinOptions->monitor().empty() )
  {
    pFsMonitor              = glfwGetPrimaryMonitor(); // Set Default
    const Monitor* pMonitor = Application::get_instance()->get_monitor_by_name( m_ptrWinOptions->monitor() );
    if ( pMonitor != nullptr )
    {
      pFsMonitor = (GLFWmonitor*)pMonitor->m_handler;
    }
   
    // Set Refresh rate accordingly with video mode max refresh rate
    glfwWindowHint( GLFW_REFRESH_RATE, 0 );
  }
  else
  {
    // For windowed mode initial state will be not visible.
    glfwWindowHint( GLFW_VISIBLE      , GL_FALSE           );
  }
  
  // Raise event for custom settings
  // in such event occurs the selection of OPENGL API
  // with default setting that can be overridden if needed
  for ( WindowEvents* e : get_connections() )
  {
    e->on_creating( this );
  }

  m_hWindow = glfwCreateWindow( 
                                m_ptrWinOptions->width(), m_ptrWinOptions->height(),
                                m_ptrWinOptions->title().c_str(),
                                pFsMonitor, 
                                nullptr
                              );

  if (!m_hWindow)
  {
    m_ptrRenderer.release();
    
    glfwTerminate();
    return false;
  }

  glfwSetWindowUserPointer( m_hWindow, this );

  set_callbacks(true);

  glfwMakeContextCurrent( m_hWindow );

#if defined(_GLES_ENABLED)
  m_glsl_version = "#version 100";
  int version = gladLoadGLES2(glfwGetProcAddress);
#else
  m_glsl_version = "#version 130";
  int version = gladLoadGL(glfwGetProcAddress);
#endif  
  if ( version != 0 )
  {
    ure::utils::log( core::utils::format( "GLAD GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version)) );
  }
  else 
  {
    ure::utils::log( "Failed to initialize OpenGL context" );

    m_ptrRenderer.release();
    
    glfwTerminate();
    return false;
  }

  // Only in windowed mode it is necessary to move window
  // and to make it visible after move.
  if ( pFsMonitor == nullptr )
  {
    glfwSetWindowPos( m_hWindow, m_ptrWinOptions->x(), m_ptrWinOptions->y() );
    
    glfwShowWindow( m_hWindow );
  }
  
  for ( WindowEvents* e : get_connections() )
  {
    e->on_created( this );
  }

/**
 * Note that initialization is immediately after the events 
 * since in case of EMSCRIPTED in the event we handle resizing of the canvas
 * used for drawing.
 */
#if defined(_IMGUI_ENABLED)
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  // @todo add option for docking branch
  //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(m_hWindow, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.

# ifdef __EMSCRIPTEN__
  ImGui_ImplGlfw_InstallEmscriptenCallbacks(m_hWindow, "#canvas");
# endif

  ImGui_ImplOpenGL3_Init( m_glsl_version.c_str() );
#endif  

  return true;
}

void_t Window::make_context_current() noexcept(true)
{
  assert( m_hWindow != nullptr );
  glfwMakeContextCurrent( m_hWindow );
}

bool_t Window::show( enum_t flags ) noexcept(true)
{
  if ( m_hWindow == nullptr )
    return false;

  if ( flags == static_cast<enum_t>(processing_flag_t::epfCalling) )
  {
    glfwShowWindow( m_hWindow );
    return true;
  }

  bool_t bResult = false;
  if ( flags & static_cast<enum_t>(processing_flag_t::epfEnqueue) )
  {
    Message* pMessage  = new ShowWindowMsg( this );
    
    if ( flags & static_cast<enum_t>(processing_flag_t::epfWait) )
      bResult = send_message( pMessage );
    else
      bResult = post_message( pMessage );
  }
  
  return bResult;
}

bool_t Window::hide( enum_t flags ) noexcept(true)
{
  if ( m_hWindow == nullptr )
    return false;

  if ( flags == static_cast<enum_t>(processing_flag_t::epfCalling) )
  {
    glfwHideWindow( m_hWindow );
    return true;
  }

  bool_t bResult = false;
  if ( flags & static_cast<enum_t>(processing_flag_t::epfEnqueue) )
  {
    Message* pMessage  = new HideWindowMsg( this );
    if ( flags & static_cast<enum_t>(processing_flag_t::epfWait) )
      bResult = send_message( pMessage );
    else
      bResult = post_message( pMessage );
  }
  
  return bResult;
}

bool_t Window::is_windowed() const noexcept(true)
{
  assert( m_hWindow != nullptr );
  return ( glfwGetWindowMonitor( m_hWindow ) == nullptr );
}

bool_t Window::set_title( const std::string& title, enum_t flags ) noexcept(true)
{
  if ( m_hWindow == nullptr )
    return false;

  if ( flags == static_cast<enum_t>(processing_flag_t::epfCalling) )
  {
    glfwSetWindowTitle( m_hWindow, title.c_str() );
    return true;
  }

  bool_t bResult = false;
  if ( flags & static_cast<enum_t>(processing_flag_t::epfEnqueue) )
  {
    Message* pMessage  = new SetWindowTitleMsg( this, title );
    
    if ( flags & static_cast<enum_t>(processing_flag_t::epfWait) )
      bResult = send_message( pMessage );
    else
      bResult = post_message( pMessage );
  }
  
  return bResult;
}

bool_t Window::set_position( const position_t<int_t>& position, enum_t flags ) noexcept(true)
{
  if ( m_hWindow == nullptr )
    return false;

  if ( flags == static_cast<enum_t>(processing_flag_t::epfCalling) )
  {
    glfwSetWindowPos( m_hWindow, position.x, position.y );
    return true;
  }
  
  bool_t bResult = false;
  if ( flags & static_cast<enum_t>(processing_flag_t::epfEnqueue) )
  {
    Message* pMessage  = new SetWindowPositionMsg( this, position );
    
    if ( flags & static_cast<enum_t>(processing_flag_t::epfWait) )
      bResult = send_message( pMessage );
    else
      bResult = post_message( pMessage );
  }
  
  return bResult;  
}

bool_t Window::set_size( const Size& size, enum_t flags ) noexcept(true)
{
  if ( m_hWindow == nullptr )
    return false;

  if ( flags == static_cast<enum_t>(processing_flag_t::epfCalling) )
  {
    glfwSetWindowSize( m_hWindow, size.width, size.height );
    return true;
  }

  bool_t bResult = false;
  if ( flags & static_cast<enum_t>(processing_flag_t::epfEnqueue) )
  {
    Message* pMessage  = new SetWindowSizeMsg( this, size );
    
    if ( flags & static_cast<enum_t>(processing_flag_t::epfWait) )
      bResult = send_message( pMessage );
    else
      bResult = post_message( pMessage );
  }
  
  return bResult;  
}

#ifndef __EMSCRIPTEN__
void_t Window::get_framebuffer_size( Size& size ) noexcept(true)
{
  assert( m_hWindow != nullptr );

  glfwGetFramebufferSize( m_hWindow, &size.width, &size.height );
}
#endif

bool_t Window::show_normal( enum_t flags ) noexcept(true)
{
  if ( m_hWindow == nullptr )
    return false;

  if ( flags == static_cast<enum_t>(processing_flag_t::epfCalling) )
  {
    glfwRestoreWindow( m_hWindow );
    return true;
  }

  bool_t bResult = false;
  if ( flags & static_cast<enum_t>(processing_flag_t::epfEnqueue) )
  {
    Message* pMessage  = new ShowWindowNormalMsg( this );
    
    if ( flags & static_cast<enum_t>(processing_flag_t::epfWait) )
      bResult = send_message( pMessage );
    else
      bResult = post_message( pMessage );
  }
  
  return bResult;
}

bool_t Window::show_minimized( enum_t flags ) noexcept(true)
{
  if ( m_hWindow == nullptr )
    return false;

  if ( flags == static_cast<enum_t>(processing_flag_t::epfCalling) )
  {
    glfwIconifyWindow( m_hWindow );
    return true;
  }
  
  bool_t bResult = false;
  if ( flags & static_cast<enum_t>(processing_flag_t::epfEnqueue) )
  {
    Message* pMessage  = new ShowWindowMinimizedMsg( this );
    
    if ( flags & static_cast<enum_t>(processing_flag_t::epfWait) )
      bResult = send_message( pMessage );
    else
      bResult = post_message( pMessage );
  }
  
  return bResult;
}

void_t Window::set_swap_interval( int_t iRefresh ) noexcept(true)
{
  assert( m_hWindow != nullptr );
  
  glfwSwapInterval(iRefresh);
}

void_t Window::swap_buffers() noexcept(true)
{
  assert( m_hWindow != nullptr );

  glfwSwapBuffers(m_hWindow);
}

void_t Window::close() noexcept(true)
{
  assert( m_hWindow != nullptr );

  glfwSetWindowShouldClose( m_hWindow, GL_TRUE );
}

bool_t Window::destroy( uint32_t flags ) noexcept(true)
{
  if ( m_hWindow == nullptr )
    return false;
  
  if ( flags == static_cast<enum_t>(processing_flag_t::epfCalling) )
  {
    m_ptrRenderer = std::move(nullptr);
  
    set_callbacks( false );
    
#if defined(_IMGUI_ENABLED)
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif

    glfwDestroyWindow(m_hWindow);
    m_hWindow = nullptr;
    
    return true;
  }

  bool_t bResult = false;
  if ( flags & static_cast<enum_t>(processing_flag_t::epfEnqueue) )
  {
    Message* pMessage  = new DestroyWindowMsg( this );
    
    if ( flags & static_cast<enum_t>(processing_flag_t::epfWait) )
      bResult = send_message( pMessage );
    else
      bResult = post_message( pMessage );
  }
  
  return bResult;
}

bool_t Window::get_input_mode( int_t mode, int_t& value ) noexcept(true)
{
  if ( m_hWindow == nullptr )
    return false;

  value = glfwGetInputMode( m_hWindow, mode );

  return true;
}

bool_t Window::set_input_mode( int_t mode, int_t value ) noexcept(true)
{
  if ( m_hWindow == nullptr )
    return false;

  glfwSetInputMode( m_hWindow, mode, value );
  
  return true;
}

bool_t Window::get_cursor_position( position_t<double>& position ) noexcept(true)
{
  if ( m_hWindow == nullptr )
    return false;

  glfwGetCursorPos( m_hWindow, &position.x, &position.y );

  return true;
}

bool_t Window::set_cursor_position( const position_t<double>& position ) noexcept(true)
{
  if ( m_hWindow == nullptr )
    return false;

  glfwSetCursorPos( m_hWindow, position.x, position.y );
  
  return true;
}
  
void_t Window::set_window_hint(int_t iTarget, int_t iHint) noexcept(true)
{
  glfwWindowHint( iTarget, iHint );
}
 
void_t Window::set_callbacks( bool_t bRegister )
{
  if ( bRegister )
  {
    glfwSetWindowPosCallback         ( m_hWindow, pos_callback           );  /* GLFW 3.0 */ 
    glfwSetWindowSizeCallback        ( m_hWindow, size_callback          );  /* GLFW 1.0 */
    glfwSetWindowCloseCallback       ( m_hWindow, close_callback         );  /* GLFW 2.5 */
    glfwSetWindowRefreshCallback     ( m_hWindow, refresh_callback       );  /* GLFW 2.5 */
    glfwSetWindowFocusCallback       ( m_hWindow, focus_callback         );  /* GLFW 3.0 */ 
    glfwSetWindowIconifyCallback     ( m_hWindow, iconify_callback       );  /* GLFW 3.0 */
    glfwSetWindowMaximizeCallback    ( m_hWindow, maximize_callback      );  /* GLFW 3.3 */
    glfwSetFramebufferSizeCallback   ( m_hWindow, fbsize_callback        );  /* GLFW 3.0 */
    glfwSetWindowContentScaleCallback( m_hWindow, content_scale_callback );  /* GLFW 3.3 */
    glfwSetMouseButtonCallback       ( m_hWindow, mouse_button_callback  );  /* GLFW 1.0 */
    glfwSetCursorPosCallback         ( m_hWindow, mouse_pos_callback     );  /* GLFW 3.0 */
    glfwSetCursorEnterCallback       ( m_hWindow, mouse_enter_callback   );  /* GLFW 3.0 */ 
    glfwSetScrollCallback            ( m_hWindow, mouse_scroll_callback  );  /* GLFW 3.0 */ 
    glfwSetKeyCallback               ( m_hWindow, key_callback           );  /* GLFW 1.0 */
    glfwSetCharCallback              ( m_hWindow, char_callback          );  /* GLFW 2.4 */
    glfwSetDropCallback              ( m_hWindow, drop_path_callback     );  /* GLFW 3.1 */
#ifndef __EMSCRIPTEN__
    glfwSetCharModsCallback          ( m_hWindow, char_mods_callback     );  /* GLFW 3.1 */
#endif
  } 
  else
  {
    glfwSetWindowPosCallback         ( m_hWindow, nullptr );  /* GLFW 3.0 */
    glfwSetWindowSizeCallback        ( m_hWindow, nullptr );  /* GLFW 1.0 */
    glfwSetWindowCloseCallback       ( m_hWindow, nullptr );  /* GLFW 2.5 */
    glfwSetWindowRefreshCallback     ( m_hWindow, nullptr );  /* GLFW 2.5 */
    glfwSetWindowFocusCallback       ( m_hWindow, nullptr );  /* GLFW 3.0 */ 
    glfwSetWindowIconifyCallback     ( m_hWindow, nullptr );  /* GLFW 3.0 */
    glfwSetWindowMaximizeCallback    ( m_hWindow, nullptr );  /* GLFW 3.3 */
    glfwSetFramebufferSizeCallback   ( m_hWindow, nullptr );  /* GLFW 3.0 */
    glfwSetWindowContentScaleCallback( m_hWindow, nullptr );  /* GLFW 3.3 */
    glfwSetMouseButtonCallback       ( m_hWindow, nullptr );  /* GLFW 1.0 */
    glfwSetCursorPosCallback         ( m_hWindow, nullptr );  /* GLFW 3.0 */
    glfwSetCursorEnterCallback       ( m_hWindow, nullptr );  /* GLFW 3.0 */
    glfwSetScrollCallback            ( m_hWindow, nullptr );  /* GLFW 3.0 */ 
    glfwSetKeyCallback               ( m_hWindow, nullptr );  /* GLFW 1.0 */
    glfwSetCharCallback              ( m_hWindow, nullptr );  /* GLFW 2.4 */
    glfwSetDropCallback              ( m_hWindow, nullptr );  /* GLFW 3.1 */ 
#ifndef __EMSCRIPTEN__
    glfwSetCharModsCallback          ( m_hWindow, nullptr );  /* GLFW 3.1 */ 
#endif
  }
}


// glfwSetMonitorCallback         /* GLFW 3.0 */
// glfwSetJoystickCallback        /* GLFW 3.2 */


bool_t Window::check( window_flag_t flags ) noexcept(true)
{
  if ( flags == window_flag_t::eWindowShouldClose )    
    return glfwWindowShouldClose( m_hWindow );
  
  return false;
}

///
void_t Window::pos_callback( WindowHandler window, int_t xpos, int_t ypos )
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));
  
  if ( pWindow == nullptr )
    return;

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    e->on_position_changed( pWindow, xpos, ypos );
  }
}

void_t Window::size_callback( WindowHandler window, int_t width, int_t height )
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));

  if ( pWindow == nullptr )
    return;

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    e->on_size_changed( pWindow, width, height );
  } 
}
 
void_t Window::close_callback( WindowHandler window )
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));

  if ( pWindow == nullptr )
    return;  

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    e->on_close( pWindow );
  }
}

void_t Window::refresh_callback( WindowHandler window )
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));

  if ( pWindow == nullptr )
    return;

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    e->on_refresh( pWindow );
  }    
}
 
void_t Window::focus_callback( WindowHandler window, int_t focus )
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));
  
  if ( pWindow == nullptr )
    return;

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    if ( focus == GL_TRUE )
      e->on_got_focus( pWindow );
    else
      e->on_lost_focus( pWindow );
  }    
}

void_t Window::iconify_callback( WindowHandler window, int_t iconified )
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));
  
  if ( pWindow == nullptr )
    return;

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    if ( iconified == GL_TRUE )
      e->on_iconified( pWindow );
    else
      e->on_restored( pWindow );
  }
}

void_t Window::maximize_callback    (WindowHandler window, int_t maximized )
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));
  
  if ( pWindow == nullptr )
    return;

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    if ( maximized == GL_TRUE )
      e->on_maximized( pWindow );
    else
      e->on_restored( pWindow );
  }

}

void_t Window::fbsize_callback( WindowHandler window, int_t width, int_t height )
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));

  if ( pWindow == nullptr )
    return;  

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    e->on_fb_size_changed( pWindow, width, height );
  }
}

void_t Window::content_scale_callback(WindowHandler window, float_t xscale, float_t yscale)
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));

  if ( pWindow == nullptr )
    return;  

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    e->on_content_scale( pWindow, xscale, yscale );
  }
}

void_t Window::mouse_button_callback( WindowHandler window, int_t button, int_t action, int_t mods )
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));

  if ( pWindow == nullptr )
    return;  

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    switch ( action )
    {
      case GLFW_PRESS:
        e->on_mouse_button_pressed( pWindow, WindowEvents::mouse_button_t(button), mods );
      break;
      case GLFW_RELEASE:
        e->on_mouse_button_released( pWindow, WindowEvents::mouse_button_t(button), mods );
      break;
    }
  }
}

void_t Window::mouse_pos_callback ( WindowHandler window, double_t xpos, double_t ypos )
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));
  
  if ( pWindow == nullptr )
    return;

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    e->on_mouse_move( pWindow, xpos, ypos );
  }
}

void_t Window::mouse_enter_callback ( WindowHandler window, int_t entered )
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));

  if ( pWindow == nullptr )
    return;

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    if ( entered == GL_TRUE )
      e->on_mouse_enter( pWindow );
    else
      e->on_mouse_leave( pWindow );
  }
}

void_t Window::mouse_scroll_callback ( WindowHandler window, double_t xoffset, double_t yoffset )
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));
  
  if ( pWindow == nullptr )
    return;

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    e->on_mouse_scroll( pWindow, xoffset, yoffset );
  }
}


void_t Window::key_callback( WindowHandler window, int_t key, int_t scancode, int_t action, int_t mods)
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));

  if ( pWindow == nullptr )
    return;

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    switch( action )
    {
      case GLFW_RELEASE:
        e->on_key_released( pWindow, static_cast<key_t>(key), scancode, mods );
      break;
      case GLFW_PRESS:
        e->on_key_pressed ( pWindow, static_cast<key_t>(key), scancode, mods );
      break;
      case GLFW_REPEAT:
        e->on_key_repeated( pWindow, static_cast<key_t>(key), scancode, mods );
      break;
    }
  }
}

void_t Window::char_callback( WindowHandler window, uint_t codepoint )
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));
  
  if ( pWindow == nullptr )
    return;

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    e->on_unicode_char( pWindow, codepoint );
  }
}

void_t Window::char_mods_callback( [[maybe_unused]] WindowHandler window, [[maybe_unused]] uint_t codepoint, [[maybe_unused]] int_t mods )
{
  Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer( window ));
  
  if ( pWindow == nullptr )
    return;

  for ( WindowEvents* e : pWindow->get_connections() )
  {
    e->on_unicode_char_mods( pWindow, codepoint, mods );
  }
}

void_t Window::drop_path_callback( [[maybe_unused]] WindowHandler window, [[maybe_unused]] int_t path_count, [[maybe_unused]] const char_t* paths[] )
{
  /* @todo */
}

}