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

#ifndef URE_APPLICATION_EVENTS_H
#define URE_APPLICATION_EVENTS_H

#include "ure_common_defs.h"

namespace ure {

class ApplicationEvents 
{
public:
  /***/
  virtual ~ApplicationEvents(){}

  /***/
  virtual void_t on_initialize() = 0;
  /***/
  virtual void_t on_initialized() = 0;
  /***/
  virtual void_t on_finalize() = 0;
  /***/
  virtual void_t on_finalized() = 0;
  /***/
  virtual void_t on_run() = 0;
  /***/
  virtual void_t on_initialize_error(/* @todo */) = 0;
  /***/
  virtual void_t on_error( int32_t error, std::string_view description ) = 0;
  /***/
  virtual void_t on_finalize_error(/* @todo */) = 0;
};

}

#endif // URE_APPLICATION_EVENTS_H
