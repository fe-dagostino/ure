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

#include "ure_application.h"
#include "ure_programs_collector.h"
#include "ure_resources_collector.h"

#include "core/utils.h"
#include "images/images.h"

namespace ure {

static void error_callback(int32_t error, const char* description)
{
  if ( Application::is_valid() )
  {
    ApplicationEvents* pEvents = Application::get_instance()->get_events();
    
    if ( pEvents != nullptr )
    {
      pEvents->on_error( error, description );
    }
  }
}

Application::Application( 
                          core::unique_ptr<ApplicationEvents> events,
                          const std::string& sShadersPath 
                        ) noexcept(true)
  : m_events( std::move(events) ), m_exit(false)
{
  glfwSetErrorCallback(error_callback);  /* GLFW 3.3 */

  ProgramsCollector::initialize();
  ProgramsCollector::get_instance()->set_shaders_path( sShadersPath );
}

Application::~Application() noexcept(true)
{

}

void_t  Application::poll_events() noexcept(true)
{
  glfwPollEvents();
}

void_t  Application::wait_events() noexcept(true)
{
  glfwWaitEvents();
}

std::string  Application::get_version() const noexcept(true)
{
  int major, minor, rev;
  glfwGetVersion( &major, &minor, &rev );
  return core::utils::format( "%d.%d.%d", major, minor, rev );
}

double_t Application::get_time() const noexcept(true)
{
  return glfwGetTime();
}
  
void_t   Application::set_time( double_t dTime ) noexcept(true)
{
  glfwSetTime( dTime );
}

const Monitor* Application::get_monitor_by_name( std::string_view name ) noexcept(true)
{
  if ( name.empty() )
    return nullptr;
    
  // if map is empty it will be populated
  if ( m_mapMonitors.empty() )
    m_mapMonitors = Monitor::detect_monitors();
  
  if ( m_mapMonitors.contains( name.data() ) == false )
    return nullptr;

  return m_mapMonitors[name.data()].get();
}  

void_t  Application::on_initialize_wm() noexcept(true)
{
  if ( m_events != nullptr )
  {
    m_events->on_initialize();
  }

  if (!glfwInit())
  {
    if ( m_events != nullptr )
    {
       m_events->on_initialize_error();
    }
  }

#ifdef _USE_AVCPP
  if (!av_init())
  {
    if ( m_events != nullptr )
    {
      m_events->on_initialize_error();
    }
  }
#endif //_USE_AVCPP

#ifdef _USE_DEVIL
  if (!il_init())
  {
    if ( m_events != nullptr )
    {
      m_events->on_initialize_error();
    }
  }
#endif  //_USE_DEVIL

  //Reset time
  set_time(0.0f);

  if ( m_events != nullptr )
  {
    m_events->on_initialized();
  }
}

void_t  Application::on_finalize_wm() noexcept(true)
{
  if ( m_events != nullptr )
  {
    m_events->on_finalize();
  }

#ifdef _USE_AVCPP
  if (!av_final())
  {
    if ( m_events != nullptr )
    {
      m_events->on_finalize_error();
    }
  }
#endif //_USE_AVCPP

#ifdef _USE_DEVIL
  if (!il_final())
  {
    if ( m_events != nullptr )
    {
      m_events->on_finalize_error();
    }
  }
#endif  //_USE_DEVIL

  ProgramsCollector::get_instance()->finalize();

  glfwTerminate();

  if ( m_events != nullptr )
  {
    m_events->on_finalized();
  }  
}

}