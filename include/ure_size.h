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

#ifndef URE_SIZE_H
#define URE_SIZE_H

#include "ure_common_defs.h"

namespace ure {

template<typename data_t>
  requires std::is_integral_v<data_t> || std::is_floating_point_v<data_t>
struct size_t final
{
public:
  /***/
  constexpr size_t() noexcept(true) 
    : width(0), height(0)
  {}
  /***/
  constexpr size_t( const data_t& w, const data_t& h ) noexcept(true)
    : width(w), height(h)
  {}

  /***/
  constexpr ~size_t() noexcept(true)
  {}
  
  data_t width;
  data_t height;
};

typedef size_t<sizei_t>   Size;
typedef size_t<double_t>  Size_d;
}

#endif // URE_SIZE_H