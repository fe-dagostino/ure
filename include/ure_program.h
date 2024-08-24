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

#ifndef URE_PROGRAM_H
#define URE_PROGRAM_H

#include "ure_handled_object.h"

#include <string>
#include <memory>
#include <vector>

namespace ure {

class VertexShader;
class FragmentShader;

class Program final : public HandledObject
{
public:
  /***/
  Program() noexcept;
  /***/
  Program( const Program& program ) noexcept
   : HandledObject(program)
  {}
  /***/
  Program( Program&& program ) noexcept
   : HandledObject( std::move(program) )
  {}
  /***/
  ~Program() noexcept;

  /**
   * Loading shader from memory string and compile it.
   * return false if compile operation has failed.
   */
  bool_t 		attach_shaders( std::shared_ptr<VertexShader> pVertexShader, std::shared_ptr<FragmentShader> pFragmentShader );

  /**
   */
  void_t 		bindAttribLocation( uint_t index, const char_t *name ) noexcept;
  
  /***/
  bool_t 		link() noexcept;
  
  /**
   * Must be called on each render.
   * Mainly this function is called in the main gl loop.
   */
  void_t  use() noexcept;
  
  /**
   * Return TRUE if link process has been done successfully.
   */
  bool_t 	is_linked() const noexcept;

  /**
   * This function should be used only for debug; validation process 
   * will be slower. If validation will fails, detailed information will 
   * be available using the getLog() method.
   */
  bool_t  validate() const noexcept;
  
  
  int_t   getActiveAttributes() const noexcept;

  int_t   getActiveAttributeMaxLength() const noexcept;

  int_t   getActiveUniforms() const noexcept;

  int_t   getActiveUniformMaxLength() const noexcept;

  int_t   getAttachedShaders() const noexcept;
  
  /**
   * Can be used to retrieve log informations generated at 
   * load time.
   * Both in case of success or failure log could contains 
   * details for the reason of failure or just a confirmation message.
   * This function can be used at debug time in order to better understad
   * if there are errors and how to fix it. 
   */
  bool_t    get_logs( std::string& logs ) noexcept;
    
protected:
  /**
   * Query object values.
   */
  uint_t 	query( enum_t e ) const noexcept;  
  
private:
  std::vector<std::shared_ptr<VertexShader>>    m_vecVertex;
  std::vector<std::shared_ptr<FragmentShader>>  m_vecFragment;
};

}

#endif // URE_PROGRAM_H
