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

#ifndef URE_BUFFER_H
#define URE_BUFFER_H

#include "ure_common_defs.h"
#include <type_traits>

namespace ure {

template<typename data_t, typename data_size_t>
  requires std::is_unsigned_v<data_size_t>
class buffer_t 
{
public:
  /***/
  constexpr buffer_t() noexcept 
    : m_length(0), m_data(nullptr)
  {}

  /***/
  constexpr ~buffer_t() noexcept 
  {
    if (m_data!=nullptr)
    { 
      free(m_data); 
      m_data   = nullptr;
      m_length = 0;
    }
  }

  /***/
  constexpr data_size_t length() const noexcept
  { return m_length; }
  /**
   * @brief 
   * 
   * @param mem       pointer to a memory that have been allocated with malloc or calloc.
   * @param length    number of data_t elements  
   * @return data_t*  return previous pointer if present or nullptr.
   */
  constexpr data_t*     attach( data_t* mem, const data_size_t& length ) noexcept
  {
    data_t* retVal = m_data;
    m_data   = mem;
    m_length = length;
    return retVal;
  }
  constexpr data_t*     copy( const data_t* mem, const data_size_t& length ) noexcept
  {
    data_t*      retVal     = m_data;
    data_size_t  dataLength = sizeof(data_t) * length;
    m_data   = (data_t*)malloc( dataLength );
    memcpy( m_data, mem, dataLength );
    m_length = length;
    return retVal;
  }

  /***/
  constexpr data_t*     detach() noexcept 
  {
    data_t* retVal = m_data;
    m_data = nullptr;
    return retVal;
  }

private:  
  data_size_t     m_length;
  data_t*         m_data;
};


template<typename data_t, typename data_size_t>
class named_buffer_t : public buffer_t<data_t,data_size_t>
{
public:
  /***/
  named_buffer_t() = delete;
  /***/
  named_buffer_t( const std::string& name )
    : buffer_t<data_t,data_size_t>(), m_name( name ) 
  {}
  /***/
  named_buffer_t( std::string&& name )
    : buffer_t<data_t,data_size_t>(), m_name( std::move(name) ) 
  {}

  /***/
  constexpr const std::string& name() const noexcept
  { return m_name; }

private:
  const std::string     m_name;  
};

}

#endif // URE_BUFFER_H