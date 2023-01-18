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

#ifndef URE_WINDOW_OPTIONS_H
#define URE_WINDOW_OPTIONS_H

#include "ure_common_defs.h"
#include "ure_position.h"
#include "ure_size.h"
#include <string>

namespace ure {

class window_options
{
public:
  /***/
  constexpr window_options(
                   const std::string&       monitor,
                   const std::string&       title,
                   const position_t<int_t>& position, 
                   const Size&              size 
                 ) noexcept
    : m_sMonitor(monitor), m_sTitle(title), 
      m_position(position),
      m_size(size)
  {}
  
  /***/
  constexpr ~window_options() noexcept
  {}
  
  /***/
  constexpr inline const std::string&   monitor() const noexcept
  { return m_sMonitor; }
  /***/
  constexpr inline const std
  ::string&   title() const noexcept
  { return m_sTitle;   }
  /***/
  constexpr inline int32_t              x() const noexcept
  { return m_position.x;    }
  /***/
  constexpr inline int32_t              y() const noexcept
  { return m_position.y;    }
  /***/
  constexpr inline sizei_t              width() const noexcept
  { return m_size.width;    }
  /***/
  constexpr inline sizei_t              height() const noexcept
  { return m_size.height;    }
  
private:
  const std::string       m_sMonitor;
  const std::string       m_sTitle;
  const position_t<int_t> m_position;
  const Size              m_size;
};

}

#endif // URE_WINDOWOPTIONS_H