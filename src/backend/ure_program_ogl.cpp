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

#include "ure_program.h"
#include "ure_vertex_shader.h"
#include "ure_fragment_shader.h"

namespace ure {

Program::Program( ) noexcept
{
  set_id( glCreateProgram( ) );
}

Program::~Program() noexcept
{
  glDeleteProgram( get_id() );
}

bool_t Program::attach_shaders( std::shared_ptr<VertexShader> pVertexShader, std::shared_ptr<FragmentShader> pFragmentShader )
{
  if (( pVertexShader == NULL ) || ( pFragmentShader == NULL ))
    return false;
  
  if ( pVertexShader->get_type() != ShaderObject::shader_t::eVertexShader )
    return false;
  
  if ( pFragmentShader->get_type() != ShaderObject::shader_t::eFragmentShader )
    return false;
  
  glAttachShader( get_id(), pVertexShader->get_id() );

  glAttachShader( get_id(), pFragmentShader->get_id() );
  
  m_vecVertex.push_back(pVertexShader);
  m_vecFragment.push_back(pFragmentShader);

  return true;
}
  
void_t Program::bindAttribLocation( GLuint index, const GLchar *name ) noexcept
{
  glBindAttribLocation( get_id(), index, name );
}

bool_t Program::link() noexcept
{
  // Link the program
  glLinkProgram( get_id() );

  if (is_linked())
    return true;
  
  return false;
}

void_t Program::use() noexcept
{
  glUseProgram( get_id() );
}

bool_t Program::is_linked() const noexcept
{
  // Check the link status
  return (query(GL_LINK_STATUS)>0);
}

bool_t Program::validate() const noexcept
{
  glValidateProgram( get_id() );

  return (query(GL_VALIDATE_STATUS)>0);
}

int_t Program::getActiveAttributes() const noexcept
{
  return query(GL_ACTIVE_ATTRIBUTES);
}

int_t Program::getActiveAttributeMaxLength() const noexcept
{
  return query(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);
}

int_t Program::getActiveUniforms() const noexcept
{
  return query(GL_ACTIVE_UNIFORMS);
}

int_t Program::getActiveUniformMaxLength() const noexcept
{
  return query(GL_ACTIVE_UNIFORM_MAX_LENGTH);
}

int_t Program::getAttachedShaders() const noexcept
{
  return query(GL_ATTACHED_SHADERS);
}

uint_t Program::query( GLenum e ) const noexcept
{
  int_t iValue;
  
  // Check the compile status
  glGetProgramiv(get_id(), e, &iValue);
  
  return iValue;
}

bool_t Program::get_logs( std::string& logs ) noexcept
{
  int_t iInfoLength = query(GL_INFO_LOG_LENGTH);
  if (iInfoLength <= 0)
    return false;
  
  char* pInfoLog = (char *)malloc(sizeof(char) * iInfoLength);
  
  glGetProgramInfoLog(get_id(), iInfoLength, NULL, pInfoLog);
  
  logs.clear();
  logs.append( pInfoLog, iInfoLength );

  free(pInfoLog);
  
  return true;
}

}
