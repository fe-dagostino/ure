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

#ifndef URE_RECT_H
#define URE_RECT_H

#include "ure_common_defs.h"

namespace ure {

template<typename data_t>
  requires std::is_integral_v<data_t> || std::is_floating_point_v<data_t>
struct rect_t final
{
public:
  /***/
  constexpr rect_t() noexcept 
    : left(0), top(0), right(0), bottom(0)
  {}
  /***/
  constexpr rect_t( data_t l, data_t t, data_t r, data_t b ) noexcept 
    : left(l), top(t), right(r), bottom(b)
  {}
  /***/
  constexpr rect_t( const rect_t& rect ) noexcept 
    : left(rect.left), top(rect.top), right(rect.right), bottom(rect.bottom)
  {}
  /***/
  constexpr ~rect_t() noexcept 
  {}

  /***/
  constexpr rect_t& operator=(const rect_t& other)
  { left = other.left; top = other.top; right = other.right; bottom = other.bottom; }
  /***/
  constexpr bool operator==(const rect_t& other) const
  { return ((left  == other.left ) && (top    == other.top   ) &&
            (right == other.right) && (bottom == other.bottom)); 
  }
  
public:
  data_t left;
  data_t top;
  data_t right;
  data_t bottom;
  
};

typedef rect_t<sizei_t>   Recti;

}

#endif // URE_RECT_H
