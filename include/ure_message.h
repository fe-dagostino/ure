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

#ifndef URE_MESSAGE_H
#define URE_MESSAGE_H

#include "ure_common_defs.h"
#include "ure_object.h"
#include "core/event.h"

namespace ure {

class Window;

/**
 * Base class to be extended for managing messages at windows thread.
 * Each ure::Window have a message queue that can be processed in the main
 * loop using ure::Window.process_message(). In order to enqueue messages
 * both ure::Window.send_message() and ure::Window.post_message() can be used:
 * the first one will block exectuion of calling thread until the message
 * has been processed; instead the second one will enqueue the message
 * without to wait its execution.
 */
class Message
{
public:
  /**
   */
  Message( Window* pWindow );
  /***/
  virtual ~Message();
  /***/
  bool execute();
  /***/
  void wait();

protected:
  /**
   * Pure virtual function that will keep specific message 
   * implementation.
   */
  virtual bool on_execute() = 0;

private:
  /***/
  void event();
  /***/
  void signal();
  /***/
  bool release();
  
protected:
  Window* m_pWindow;
  
private:
  std::unique_ptr<core::event>   m_event;
  
  friend class Window;
};

}

#endif // URE_MESSAGE_H