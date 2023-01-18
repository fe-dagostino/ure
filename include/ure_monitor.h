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

#ifndef URE_MONITOR_H
#define URE_MONITOR_H

#include "ure_object.h"
#include "ure_size.h"
#include "ure_position.h"
#include <vector>
#include <unordered_map>

namespace ure {

class Window;

/***/
class Monitor : public Object
{
public:
  typedef std::unordered_map<const char*,std::unique_ptr<Monitor>> monitor_map_t;

private:
  /**
   * @param 
   */
  Monitor( MonitorHandler pMonitor ) noexcept;
public:  
  /***/
  Monitor( const Monitor& rMonitor ) noexcept;

  /***/
  virtual ~Monitor();

  /***/
  constexpr inline const std::string&          name() const noexcept
  { return m_name; }
  
  /**
   * Return current video mode.
   */
  constexpr inline const VideoMode&            video_mode() const noexcept
  { return m_VideoMode; }

  /**
   * Return the all available video modes supported by the monitor.
   */
  std::vector<VideoMode>    query_video_modes() const;  
  
  /***/
  static monitor_map_t      detect_monitors() noexcept;
protected:

private:
  MonitorHandler        m_handler;
  std::string           m_name;
  VideoMode             m_VideoMode;

  friend class Window;
};

}

#endif // URE_MONITOR_INFO_H
