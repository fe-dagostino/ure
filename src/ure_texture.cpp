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

Texture::Texture( sizei_t width, sizei_t height, format_t format, type_t type ) noexcept(true)
  : HandledObject(URE_INVALID_HANDLE),
    m_size( width, height ), m_format( format ), m_type( type ), m_lifecycle(lifecycle_t::eRender),
    m_length( 0 ), m_pixels( nullptr )
{ 
  tex_alloc();
}

Texture::Texture( Image&& image, lifecycle_t lifecycle,
                  int_t tws,
                  int_t twt
                ) noexcept(true)
  : HandledObject(URE_INVALID_HANDLE)
{
  if ( image.get_format() == Image::format_t::eRGBA )
  {
    m_size      = image.get_size();
    m_format    = format_t::eRGBA;
    m_type      = type_t::eUnsignedByte;
    m_pixels    = image.detach(&m_length);
    m_lifecycle = lifecycle;
    if ( lifecycle == lifecycle_t::eObject )
    {
      if ( tex_create( GL_TEXTURE_2D, 0, tws, twt ) == true )
      {
        /* In this condition lifecycle of the texture will be the same lifecycle of the object, this means
         * that the texture_id will be available in the GPU until the Texture instance will be delete.
         * In this specific condition we don't need anymore to keep the buffer allocated also in RAM.
         */
        tex_free();
      }
    }
  }
}

Texture::~Texture() noexcept(true)
{
  tex_free(); 
}

bool_t  Texture::tex_alloc() noexcept(true)
{
  if ( m_type != type_t::eUnsignedByte )
    return false;

  if ( m_pixels != nullptr )
  {
    free( m_pixels );
    m_pixels = nullptr;
  }
  // Accepted values
  int bpp = 4;
  switch ( m_format )
  {
    case format_t::eRGB : bpp = 3; break;
    default:              bpp = 4; break;
  }

  m_length      = m_size.width*m_size.height*bpp;

  if ( m_pixels == nullptr )
  {
    m_pixels = (uint8_t*)calloc( m_length, sizeof(uint8_t) );
  }

  if (m_pixels == nullptr)
    return false;

  return true;
}

void_t Texture::tex_free() noexcept(true)
{
  if ( m_pixels != nullptr )
  {
    free( m_pixels );
    m_pixels = nullptr;
  }
}


}
