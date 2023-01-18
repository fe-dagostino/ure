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

#ifndef URE_HANDLED_OBJECT_H
#define URE_HANDLED_OBJECT_H

#include "ure_object.h"

namespace ure {

class HandledObject : public Object
{
public:
  /***/
  HandledObject() noexcept
    : Object(), m_uiId(0)
  {}
  /***/
  HandledObject( const HandledObject& ho ) noexcept
    : Object(), m_uiId(ho.m_uiId)
  {}
  /***/
  HandledObject( HandledObject&& ho ) noexcept
    : Object(), m_uiId(ho.m_uiId)
  {}
  /***/
  virtual ~HandledObject() noexcept
  {}

  /**
   * Used to check identifier validity.
   */
  constexpr inline bool    is_valid() const noexcept
  { return (m_uiId!=0); }

  /**
   * Return object identifier.
   */
  constexpr inline uint_t  get_id() const noexcept
  { return m_uiId; }
protected:
  /**
   * Initialize \param @id value.
   */
  constexpr inline void_t  set_id( uint_t id )
  { m_uiId = id; }

private:
  uint_t   m_uiId;   
};

}

#endif // URE_HANDLED_OBJECT_H
