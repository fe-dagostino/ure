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

#ifndef URE_APPLICATION_H
#define URE_APPLICATION_H

#include "ure_common_defs.h"
#include "ure_application_events.h"
#include <core/unique_ptr.h>

#include <core/singleton.h>
#include "ure_monitor.h"


#include <map>

namespace ure {

/**
 * @brief 
 * 
 */
class Application final : public core::singleton_t<Application>
{
private:
  friend class singleton_t<Application>;

  typedef std::map<const char*, MonitorHandler>  MonitorsMap;

public:
  /***/
  Application() = delete;
  /***/
  Application( const Application& ) = delete;
  /***/
  ~Application() noexcept(true);

protected:
  /***/
  Application(  core::unique_ptr<ApplicationEvents> events, 
                const std::string& sShadersPath 
             ) noexcept(true);
public:
  /***/
  void_t               run() noexcept(true);
  
  /***/
  constexpr ApplicationEvents* set_events( core::unique_ptr<ApplicationEvents> events ) noexcept(true)
  { 
    ApplicationEvents* pRetVal = events.release();
    m_events = std::move(events);  
    return pRetVal;
  }
  /***/
  constexpr ApplicationEvents* get_events() noexcept(true)
  { return m_events.get(); }

  /***/
  constexpr bool            exit() const noexcept(true)
  { return m_exit; }
  /***/
  constexpr void            exit( bool doexit ) noexcept(true)
  { m_exit = doexit; }

  /***/
  void_t                    poll_events() noexcept(true);
  /***/
  void_t                    wait_events() noexcept(true);
  /***/ 
  std::string               get_version() const noexcept(true);
  
  /**
   * Return time in seconds since application initialization.
   */
  double_t                  get_time() const noexcept(true);
  /***/
  void_t                    set_time( double_t dTime ) noexcept(true);
    
  /***/
  const Monitor*            get_monitor_by_name( std::string_view name ) noexcept(true);

public:
  /***/
  void_t     on_initialize() noexcept(true);
  /***/
  void_t     on_finalize() noexcept(true);

private:
  /***/
  void_t     on_initialize_wm() noexcept(true);
  /***/
  void_t     on_finalize_wm() noexcept(true);

  /***/
  void_t     on_initialize_ws() noexcept(true);
  /***/
  void_t     on_finalize_ws() noexcept(true);
  /***/
  void_t     processing_ws() noexcept(true);
  
private:
  core::unique_ptr<ApplicationEvents>  m_events;
  mutable Monitor::monitor_map_t       m_mapMonitors;  
  bool_t                               m_exit;
};

}

#endif // URE_APPLICATION_H
