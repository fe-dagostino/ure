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

#include "ure_shader_object.h"

#include <filesystem>
#include <fstream>
#include <streambuf>

namespace ure {

ShaderObject::ShaderObject( shader_t eType )
{
  switch (eType)
  {
    case shader_t::eFragmentShader:
      set_id( glCreateShader( GL_FRAGMENT_SHADER ) );
    break;
    case shader_t::eVertexShader:
      set_id( glCreateShader( GL_VERTEX_SHADER ) );
    break;
    default:
      throw std::exception();
    break;
  }
}

ShaderObject::~ShaderObject()
{
  glDeleteShader( get_id() );
}

bool ShaderObject::load( const std::string& source )
{
  if ( is_valid() == false )
    return false;
  
  const char *c_str = source.c_str();

  // Load the shader source
  glShaderSource(get_id(), 1, &c_str, NULL);
  
  // Compile the shader
  glCompileShader(get_id());
    
  if(is_compiled())
    return true;
  
  return false;
}

bool ShaderObject::load_from_file( const std::string& filename )
{
  if ( filename.empty() )
    return false;

  std::filesystem::path src_filename(filename);

  if ( std::filesystem::exists(src_filename) == false )
    return false;

  std::ifstream  src_file(filename);
  std::string    src_shader( (std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>() );

  bool  _bRetVal = load( src_shader );
  
  return _bRetVal;
}

uint32_t ShaderObject::query( GLenum e ) const
{
  GLint iValue;
  
  // Check the compile status
  glGetShaderiv(get_id(), e, &iValue);
  
  return iValue;
}

ShaderObject::shader_t ShaderObject::get_type() const
{
  switch (query(GL_SHADER_TYPE))
  {
    case GL_FRAGMENT_SHADER: return shader_t::eFragmentShader;
    case GL_VERTEX_SHADER: return shader_t::eVertexShader;
  }

  throw std::exception();
}

bool ShaderObject::is_compiled() const
{
  // Check the compile status
  return (query(GL_COMPILE_STATUS)>0);
}

uint32_t ShaderObject::get_source_length() const
{
  return query(GL_SHADER_SOURCE_LENGTH);
}

bool  ShaderObject::get_logs( std::string& logs )
{
  GLint iInfoLength = query(GL_INFO_LOG_LENGTH);
  if (iInfoLength <= 0)
    return false;
  
  char* pInfoLog = (char *)malloc(sizeof(char) * iInfoLength);
  
  glGetShaderInfoLog(get_id(), iInfoLength, NULL, pInfoLog);
  
  logs.clear();
  logs.append( pInfoLog, iInfoLength );
  
  free(pInfoLog);
  
  return true;
}

}
