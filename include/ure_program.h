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
  Program();
  /***/
  Program( const Program& program )
   : HandledObject(program)
  {}
  /***/
  Program( Program&& program )
   : HandledObject( std::move(program) )
  {}
  /***/
  ~Program();

  /**
   * Loading shader from memory string and compile it.
   * return false if compile operation has failed.
   */
  bool 		attach_shaders( std::shared_ptr<VertexShader> pVertexShader, std::shared_ptr<FragmentShader> pFragmentShader );

  /**
   */
  void 		bindAttribLocation( uint_t index, const char_t *name );
  
  /**
   */
  bool 		link();
  
  /**
   * Must be called on each render.
   * Mainly this function is called in the main gl loop.
   */
  void 		use();
  
  /**
   * Return TRUE if link process has been done successfully.
   */
  bool 		is_linked() const;

  /**
   * This function should be used only for debug; validation process 
   * will be slower. If validation will fails, detailed information will 
   * be available using the getLog() method.
   */
  bool 		validate() const;
  
  
  int_t   getActiveAttributes() const;

  int_t   getActiveAttributeMaxLength() const;

  int_t   getActiveUniforms() const;

  int_t   getActiveUniformMaxLength() const;

  int_t   getAttachedShaders() const;
  
  /**
   * Can be used to retrieve log informations generated at 
   * load time.
   * Both in case of success or failure log could contains 
   * details for the reason of failure or just a confirmation message.
   * This function can be used at debug time in order to better understad
   * if there are errors and how to fix it. 
   */
  bool    get_logs( std::string& logs );
    
protected:
  /**
   * Query object values.
   */
  uint_t 	query( enum_t e ) const;  
  
private:
  std::vector<std::shared_ptr<VertexShader>>    m_vecVertex;
  std::vector<std::shared_ptr<FragmentShader>>  m_vecFragment;
};

}

#endif // URE_PROGRAM_H
