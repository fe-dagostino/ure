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
#include <core/unique_ptr.h>


//#include "GLShape.h"
#include <core/singleton.h>
#include "ure_monitor.h"


#include <map>

namespace ure {

class ApplicationEvents 
{
public:
  /***/
  virtual ~ApplicationEvents(){}

  /***/
  virtual void on_run() = 0;
  /***/
  virtual void on_initialize_error(/* @todo */) = 0;
  /***/
  virtual void on_error( int32_t error, const std::string& description ) = 0;
  /***/
  virtual void on_finalize_error(/* @todo */) = 0;
};

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
  ~Application() noexcept;

protected:
  /***/
  Application(  core::unique_ptr<ApplicationEvents> events, 
                const std::string& sShadersPath, 
                const std::string& sMediaPath  
             ) noexcept;
public:
  /***/
  void        run();

  /***/
  //static VOID init( ApplicationEvents* pEvents, const std::string& sShadersPath, const std::string& sMediaPath );
  /***/
  void        final();
  
  /***/
  ApplicationEvents*   set_events( ApplicationEvents* pEvents );
  /***/
  ApplicationEvents*   get_events()
  { return m_events.get(); }

  /***/
  inline bool quit() const noexcept
  { return m_quit; }
  /***/
  inline void quit( bool quit ) noexcept
  { m_quit = quit; }

  /***/
  void                 poll_events();
  /***/
  void                 wait_events();
  /***/ 
  std::string          get_version() const noexcept;
  
  /**
   * Return time in seconds since application initialization.
   */
  double               getTime() const;
  /***/
  void                 setTime( double dTime );
    
  /***/
  const Monitor*       get_monitor_by_name( const std::string& name ) noexcept;

public:
  /***/
  void     on_initialize();
  /***/
  void     on_finalize();

private:
  core::unique_ptr<ApplicationEvents>  m_events;
  mutable Monitor::monitor_map_t       m_mapMonitors;  
  bool                                 m_quit;
};




}

#endif // URE_APPLICATION_H
