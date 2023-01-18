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

#include "ure_monitor.h"

namespace ure {

Monitor::Monitor( MonitorHandler handler ) noexcept
 : Object(), m_handler( handler )
{
  m_name     = glfwGetMonitorName( (GLFWmonitor*)m_handler );
  m_VideoMode = *glfwGetVideoMode ( (GLFWmonitor*)m_handler );
}

Monitor::Monitor( const Monitor& rMonitor ) noexcept
 : Object(), m_handler( rMonitor.m_handler ),
   m_name( rMonitor.m_name )
{
}

Monitor::~Monitor()
{
  
}

std::vector<VideoMode> Monitor::query_video_modes() const
{
  std::vector<VideoMode>  lstModes;
  
  int iCount = 0;
  const GLFWvidmode* pVideoModes = glfwGetVideoModes( (GLFWmonitor*)m_handler, &iCount );
  while ( iCount > 0 )
  {
    lstModes.push_back( *pVideoModes );
    
    pVideoModes++;
    iCount--;
  }
  
  return lstModes;
}

Monitor::monitor_map_t      Monitor::detect_monitors() noexcept
{
  monitor_map_t  monitors;
  
  int           iMonitorCount  = 0;
  GLFWmonitor** ppMonitors     = nullptr;
  
  ppMonitors = glfwGetMonitors( &iMonitorCount );
  for ( int i=0; i < iMonitorCount; i++ )
  {
    std::unique_ptr<Monitor> ptrMonitor( new Monitor( static_cast<MonitorHandler>(ppMonitors[i]) ) );
    
    if ( monitors.contains( ptrMonitor->name().c_str() ) == false )
    {
      monitors[ptrMonitor->name().c_str()] = std::move(ptrMonitor);
    }
  }

  return monitors;  
}  


}
