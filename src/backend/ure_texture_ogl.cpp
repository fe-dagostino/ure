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

#include "ure_texture.h"


namespace ure {


Texture::~Texture()
{
  if ( m_pixels != nullptr )
  {
    free( m_pixels );
    m_pixels = nullptr;
  }
}

void  Texture::set_packing( int32_t param )
{
  glPixelStorei(GL_PACK_ALIGNMENT, param );
}

void  Texture::set_unpacking( int32_t param )
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, param );
}

void  Texture::setParameterf( uint32_t target, uint32_t pname, GLfloat param )
{ 
  glTexParameterf( target, pname, param ); 
}

void  Texture::setParameteri( uint32_t target, uint32_t pname, int32_t param )
{ 
  glTexParameteri( target, pname, param ); 
}

void  Texture::render(  const std::vector<glm::vec2>& vertices, const std::vector<glm::vec2>& texCoord, 
					              bool blend, enum_t target, int_t level, int_t uLocation, GLuint aVertices, GLuint aTexCoord,
					              int_t  tws, int_t twt
 					)
{
  if ( blend )
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  
  // Disable default 4 byte alignment
  set_unpacking( 1 );
    
  // Create Textures	    
  GLuint   _textureID;
  glGenTextures(1, &_textureID);
  set_id( _textureID );
  
  // texture 1 (poor quality scaling)
  glBindTexture(GL_TEXTURE_2D, get_id());   // 2d texture (x and y size)

  // Set the base map sampler to texture unit 0
  glUniform1i(uLocation, 0);
  
  // Set 2D texture rendering options
  setParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // cheap scaling when image smalled than texture
  setParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // cheap scaling when image bigger than texture
  setParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tws );
  setParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, twt );  
 
  // Send texture data to graphics card
  glTexImage2D(target, level, (GLint)get_format(), m_size.width, m_size.height, 0, (GLenum)get_format(), (GLenum)get_type(), m_pixels );

  glEnableVertexAttribArray(aTexCoord);
  glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, texCoord.data() );
    
  glEnableVertexAttribArray(aVertices);
  glVertexAttribPointer(aVertices, 2, GL_FLOAT, GL_FALSE, 0, vertices.data() );
  
  glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() ); 
  
  if ( blend )
  {
    glDisable(GL_BLEND);
  }
  
  glDeleteTextures(1, &_textureID);
  set_id( _textureID );
  
  glDisableVertexAttribArray(aTexCoord);
  glDisableVertexAttribArray(aVertices);
}

}
