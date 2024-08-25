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

#include "ure_canvas.h"
#include "ure_programs_collector.h"

#include <glm/gtc/type_ptr.hpp>

namespace ure {

Canvas::Canvas() noexcept
{
  m_mvp = glm::mat4( 1.0f );
}

Canvas::~Canvas() noexcept
{
  
}
  
void_t  Canvas::draw_points( const std::vector<glm::vec2>& points, const glm::vec4& color, float_t fThickness ) noexcept
{
#if defined(_NV_CARD_)  
// Values are not part of ES Specs but still supported on NV cards
/* Shaders */
#define GL_VERTEX_PROGRAM_POINT_SIZE      0x8642
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A  

  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif
  
  draw( GL_POINTS, points, color, fThickness );
  
#if defined(_NV_CARD_)  
  glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif
}

void_t  Canvas::draw_lines( const std::vector<glm::vec2>& points, const glm::vec4& color, float_t fThickness ) noexcept
{
  glLineWidth( fThickness );
  
  draw( GL_LINE_STRIP, points, color, fThickness );
}

void_t  Canvas::draw_rect( const std::vector<glm::vec2>& points, const glm::vec4& color ) noexcept
{
  if ( points.size() != 4 )
    return;
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  
  draw( GL_TRIANGLE_STRIP, points, color, 1.0f );
  
  glDisable(GL_BLEND);
}

void  Canvas::draw_rect( const std::vector<glm::vec2>& vertices, const std::vector<glm::vec2>& texCoord, Texture& texture, int_t tws, int_t twt ) noexcept
{
  Program* pProgram = ProgramsCollector::get_instance()->find( "DefaultTexture" );
  if ( pProgram == nullptr )
  {
    std::vector< std::pair<int,std::string> >  attributes;
  
    attributes.push_back( std::pair<int,std::string>( 0, "a_v2Point" ) );
    attributes.push_back( std::pair<int,std::string>( 1, "a_v2TexCoord" ) );

    pProgram = ProgramsCollector::get_instance()->create( "DefaultTexture", attributes );
  }
  
  if ( pProgram == nullptr )
    return;
  
  pProgram->use();
  
  // Get the uniform location
  int_t MatrixID  = glGetUniformLocation( pProgram->get_id(), "u_m4MVP"     );
  int_t TextureID = glGetUniformLocation( pProgram->get_id(), "u_2dTexture" );
  
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(m_mvp) );
  
  texture.render( vertices, texCoord, true, GL_TEXTURE_2D, 0, TextureID, 0, 1, tws, twt );
}
  
void  Canvas::draw_text( const std::vector<glm::vec2>& vertices, const std::vector<glm::vec2>& texCoord, const Text& text, int_t tws, int_t twt ) noexcept
{
  draw( vertices, texCoord, text, tws, twt );
}

void  Canvas::draw( enum_t mode, const std::vector<glm::vec2>& points, const glm::vec4& color, float_t fThickness ) noexcept
{
  Program* pProgram = ProgramsCollector::get_instance()->find( "DefaultSolid" );
  if ( pProgram == nullptr )
  {
    std::vector< std::pair<int,std::string> >  attributes;
  
    attributes.push_back( std::pair<int,std::string>( 0, "a_v2Point" ) );
  
    pProgram = ProgramsCollector::get_instance()->create( "DefaultSolid", attributes );
  }

  if ( pProgram == nullptr )
    return;
  
  pProgram->use();
    
  // Get the uniform location
  int_t MatrixID    = glGetUniformLocation( pProgram->get_id(), "u_m4MVP"      );
  int_t ThicknessID = glGetUniformLocation( pProgram->get_id(), "u_fThickness" );
  int_t ColorID     = glGetUniformLocation( pProgram->get_id(), "u_v4Color"    );
  
  glUniformMatrix4fv(MatrixID   , 1, GL_FALSE, glm::value_ptr(m_mvp) );
  glUniform1f       (ThicknessID, fThickness                         );
  glUniform4fv      (ColorID    , 1, glm::value_ptr(color)           );  

  glEnableVertexAttribArray(0);
  glVertexAttribPointer    (0, 2, GL_FLOAT, GL_FALSE, 0, points.data() );

  glDrawArrays(mode, 0, points.size() ); 
  
  glDisableVertexAttribArray(0);
}

void   Canvas::draw( const std::vector<glm::vec2>& vertices, const std::vector<glm::vec2>& texCoord, const Text& text, int_t tws, int_t twt ) noexcept
{
  Program* pProgram = ProgramsCollector::get_instance()->find( "DefaultText" );
  if ( pProgram == nullptr )
  {
    std::vector< std::pair<int,std::string> >  attributes;
  
    attributes.push_back( std::pair<int,std::string>( 0, "a_v2Point"    ) );
    attributes.push_back( std::pair<int,std::string>( 1, "a_v2TexCoord" ) );
  
    pProgram = ProgramsCollector::get_instance()->create( "DefaultText", attributes );
  }
  
  if ( pProgram == nullptr )
    return;
  
  pProgram->use();
  
  // Get the uniform location
  int_t MatrixID  = glGetUniformLocation( pProgram->get_id(), "u_m4MVP"     );
  int_t TextureID = glGetUniformLocation( pProgram->get_id(), "u_2dTexture" );
  int_t ColorID   = glGetUniformLocation( pProgram->get_id(), "u_v4Color"   );
  
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(m_mvp) );
  glUniform4fv      ( ColorID, 1, glm::value_ptr(text.get_color()) );  
  
  text.get_texture()->render( vertices, texCoord, true, GL_TEXTURE_2D, 0, TextureID, 0, 1, tws, twt );
}

}
