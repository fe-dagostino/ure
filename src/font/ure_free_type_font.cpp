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

#include "font/ure_free_type_font.h"
#include "ure_text.h"
#include "ure_texture.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
#include FT_GLYPH_H

namespace ure {

namespace font {


FreeTypeFont::FreeTypeFont( FT_Face face, const Size& size )
 : Font(), m_face( face ),
   m_left(0), m_top(0), m_right(0), m_bottom(0)
{
  set_size( size );
}

FreeTypeFont::~FreeTypeFont()
{
  FT_Done_Face( m_face );
}

const Size& FreeTypeFont::get_size() const noexcept
{
  return m_size;
}

void_t      FreeTypeFont::set_size( const Size& size ) noexcept
{
  m_size = size;
  
  FT_Set_Pixel_Sizes( m_face, m_size.width, m_size.height );
  
  if ( m_size.width  == 0 )
    m_size.width  = m_size.height;

  if ( m_size.height == 0 )
    m_size.height = m_size.width;
}

void_t      FreeTypeFont::get_margins( sizei_t& left, sizei_t& top, sizei_t& right, sizei_t& bottom ) const noexcept
{
   left   = m_left;
   top    = m_top;
   right  = m_right;
   bottom = m_bottom;
}

void_t      FreeTypeFont::set_margins( sizei_t  left, sizei_t  top, sizei_t  right, sizei_t  bottom ) noexcept
{
   m_left   = left;
   m_top    = top;
   m_right  = right;
   m_bottom = bottom;
}

Text*       FreeTypeFont::get_text( const std::wstring& sText, const glm::vec4& frColor )
{
  if ( sText.empty() == true )
    return nullptr;
  
  std::vector<FT_Glyph>   glyphs;
  const wchar_t *         pBuffer = sText.c_str();
  std::size_t             iChars  = sText.length();
  FT_Error                error   = 0;
  Size                    size(0,0) ;
  
  while ( iChars-- )
  {
    FT_Glyph pGlyph = nullptr;
    
    error = FT_Load_Char ( m_face, *pBuffer++, FT_LOAD_RENDER );
    if ( error == 0 )
    {
      error = FT_Get_Glyph( m_face->glyph, &pGlyph );
      if ( error == 0 )
      {
        // convert to a bitmap (default render mode + destroying old)  
        if ( pGlyph->format != FT_GLYPH_FORMAT_BITMAP )                 
        {
          error = FT_Glyph_To_Bitmap( &pGlyph, FT_RENDER_MODE_NORMAL, 0, 1 );                          
	      }
	      
        // access bitmap content by typecasting                        
        FT_BitmapGlyph bmpGlyph = (FT_BitmapGlyph)pGlyph;  
	
        ///////////
        // Update bounding rect
	      size.width  += (pGlyph->advance.x >> 16);
        size.height = std::max( size.height, (GLsizei)bmpGlyph->bitmap.rows );
      
        glyphs.push_back( pGlyph );
      }
    }     
  } //while ( iChars-- )
  
  ///////////////////
  // Create new texture in order to store all characters
  Texture* pTexture = new(std::nothrow) Texture( size.width + m_left + m_right, size.height + m_top  + m_bottom, Texture::format_t::eRGBA, Texture::type_t::eUnsignedByte );
  if (pTexture==nullptr)
    return nullptr;
  
  uint8_t* pTextureData = pTexture->get_pixels();  
  
  sizei_t  width = pTexture->get_size().width;  
  sizei_t  xPos  = m_left;
  for ( auto g : glyphs )
  {
    // access bitmap content by typecasting                        
    FT_BitmapGlyph bmpGlyph = (FT_BitmapGlyph)g;
    FT_Bitmap&     bmp      = (bmpGlyph->bitmap);
    
    int yOffset = (size.height - bmpGlyph->top) + m_top;

    for ( unsigned int y = 0; y < bmp.rows; y++ )
    {
      memcpy( &(pTextureData[((y+yOffset)*width)+(xPos+bmpGlyph->left)]), &(bmp.buffer[y*bmp.width]), bmp.width );
    }
    
    xPos += (g->advance.x >> 16);
    
    // Release glyph data
    FT_Done_Glyph( g );
  }
  
  Text* pText = new(std::nothrow) Text( pTexture );
  if ( pText == nullptr )
  {
    delete pTexture;
    return nullptr;
  }
  
  // Set text foreground color
  pText->set_color( frColor );
  
  return pText;
}

}

}
