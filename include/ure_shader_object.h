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

#ifndef URE_SHADER_OBJECT_H
#define URE_SHADER_OBJECT_H

#include "ure_handled_object.h"
#include <string>

namespace ure {

class ShaderObject : public HandledObject
{
public:

  enum class shader_t : int32_t {
    eFragmentShader = URE_FRAGMENT_SHADER,
    eVertexShader = URE_VERTEX_SHADER
  };

  /***/
  ShaderObject( shader_t eType );

  /***/
  virtual ~ShaderObject();

  /**
   * Loading shader from memory string and compile it.
   * return FALSE if compile operation has failed.
   */
  bool 		load( const std::string& source );
  /**
   * Loading shader from specified file and compile it.
   * return \false if load or compile operation has failed 
   * return \true if succeded.
   */
  bool 		load_from_file( const std::string& filename );
  
  /**
   * Return shader type vertex or fragment.
   */
  shader_t	get_type() const;
  /**
   * Return shader source code length.
   */
  uint32_t 	get_source_length() const;

  /**
   * Can be used to check compile status.
   */
  bool 		  is_compiled() const;
  
  /**
   * Can be used to retrieve log informations generated at 
   * load time.
   * Both in case of success or failure log could contains 
   * details for the reason of failure or just a confirmation message.
   * This function can be used at debug time in order to better understad
   * if there are errors and how to fix it. 
   */
  bool 		  get_logs( std::string& logs );
  
protected:
  
  /**
   * Query object values.
   */
  GLuint 	query( GLenum e ) const;  
  
private:
  
  friend class GLProgram;
};

}

#endif // URE_SHADER_OBJECT_H
