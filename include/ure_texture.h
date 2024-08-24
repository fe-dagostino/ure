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

#ifndef URE_TEXTURE_H
#define URE_TEXTURE_H

#include "ure_image.h"
#include "ure_handled_object.h"
#include "ure_size.h"

#include <vector>

namespace ure {

/***
 * 
 */
class Texture : public HandledObject
{
public:

  enum class format_t : int32_t {
    eUndefined,
    eRGB  = URE_RGB,
    eRGBA = URE_RGBA
  };

  enum class type_t : int32_t {
    eUndefined,
    eUnsignedByte = URE_UNSIGNED_BYTE
  };

  /***/
  constexpr Texture()
    : m_size( 0, 0 ), m_format( format_t::eUndefined ), m_type( type_t::eUndefined ), 
      m_length( 0 ), m_pixels( nullptr )
  {}

  Texture( sizei_t width, sizei_t height, format_t format, type_t type )
    : m_size( 0, 0 ), m_format( format_t::eUndefined ), m_type( type_t::eUndefined ), 
      m_length( 0 ), m_pixels( nullptr )
  { init( width, height, format, type, nullptr ); }

  /***/
  Texture( Image&& image )
  {
    if ( image.get_format() == Image::format_t::eRGBA )
    {
      m_size   = image.get_size();
      m_format = format_t::eRGBA;
      m_type   = type_t::eUnsignedByte;
      m_pixels = image.detach(&m_length);
    }
  }
  
  /***/
  ~Texture()
  {
    if ( m_pixels != nullptr )
    {
      free( m_pixels );
      m_pixels = nullptr;
    }
  }
  
  /***/
  constexpr format_t      get_format() const noexcept
  { return m_format; }
  
  /***/
  constexpr type_t        get_type() const noexcept
  { return m_type; }
  
  /**
   * Return size in pixels.
   */
  constexpr const Size&   get_size() const noexcept
  { return m_size; }

  /**
   * @return pixels buffer length in bytes
   */
  constexpr uint32_t      get_length() const noexcept
  { return m_length; }

  /***/
  constexpr uint8_t*      get_pixels() const noexcept
  { return m_pixels; }
  
  /***/
  void                    set_packing( int32_t param );
  /***/
  void                    set_unpacking( int32_t param );

  /***/
  inline void             setParameterf( uint32_t target, uint32_t pname, GLfloat param );
  /***/
  inline void             setParameteri( uint32_t target, uint32_t pname, GLint param );
  
  /**
   * Push image data, vertex coordinates and texture coordinates then draw it.
   * At current time all data will be sent to GPU each time render will be called.
   * 
   * @param target     Specifies the target texture of the active texture unit. 
   *                   Must be GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
   *                   GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 
   *                   GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 
   *                   or GL_TEXTURE_CUBE_MAP_NEGATIVE_Z.
   * @param level      Specifies the level-of-detail number. Level 0 is the base image 
   *                   level. Level n is the nth mipmap reduction image.
   */
  void                   render(  const std::vector<glm::vec2>& vertices, 
				                          const std::vector<glm::vec2>& texCoord,
				                          bool blend,
				                          enum_t target, int_t level, int_t uLocation, 
                                  uint_t aVertices, uint_t aTexCoord,
				                          int_t  tws, int_t twt 
                                );
private:
  /***/
  bool  init( sizei_t width, sizei_t height, format_t format, type_t type, uint8_t* pixels ) noexcept
  {
    if ( type != type_t::eUnsignedByte )
      return false;
  
    if ( m_pixels != nullptr )
    {
      free( m_pixels );
      m_pixels = nullptr;
    }

    m_size.width  = width;
    m_size.height = height;
    m_format      = format;
    m_type        = type;

    // Accepted values
    int bpp = 4;
    switch ( format )
    {
      case format_t::eRGB : bpp = 3; break;
      default:              bpp = 4; break;
    }
    
    m_length      = width*height*bpp;
    
    if ( pixels == nullptr ){
      m_pixels      = (uint8_t*)calloc( m_length, sizeof(uint8_t) );
    }
    else {
      m_pixels      = pixels;
      pixels        = nullptr;
    }

    if (m_pixels == nullptr)
      return false;
  
    return true;    
  }

private:
  Size      m_size;
  format_t  m_format;
  type_t    m_type;
  uint32_t  m_length;
  uint8_t*  m_pixels;
};

}

#endif // URE_TEXTURE_H
