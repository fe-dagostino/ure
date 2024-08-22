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

#ifndef URE_POSITION_H
#define URE_POSITION_H

#include "ure_common_defs.h"

namespace ure {

template<typename data_t>
  requires std::is_integral_v<data_t> || std::is_floating_point_v<data_t>
struct position_t final
{
public:
  /***/
  constexpr position_t() noexcept 
    : x(0), y(0)
  {}
  /***/
  constexpr position_t( const data_t& xpos, const data_t& ypos ) noexcept 
    : x(xpos), y(ypos)
  {}

  /***/
  constexpr ~position_t() noexcept 
  {}

  /***/
  constexpr const position_t& operator+=( const position_t& pos ) noexcept
  { 
    x += pos.x; 
    y += pos.y;
    return *this;
  }
  
  data_t x;
  data_t y;
};

typedef position_t<sizei_t>  Position;
typedef glm::vec2            Position2D;
typedef glm::vec3            Position3D;

}

#endif // URE_POSITION_H