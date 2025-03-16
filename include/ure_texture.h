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

  enum class lifecycle_t : int32_t {
    eRender,                        /* Lifecycle for the texture will be related to render(),
                                     * that means that texture will be created, used and areleased
                                     * during the render().
                                     */
    eObject,                        /* Lifecycle for the texture will be related to instance of
                                     * the object, so texture will be created and release only
                                     * when the instance will be destroiyed.
                                     */
  };

  enum class format_t : int32_t {
    eUndefined,
    eRGB  = URE_RGB,
    eRGBA = URE_RGBA
  };

  enum class type_t : int32_t {
    eUndefined,
    eUnsignedByte = URE_UNSIGNED_BYTE
  };

  /* Delete default constructor */
  constexpr Texture() = delete;

  Texture( sizei_t width, sizei_t height, format_t format, type_t type ) noexcept(true);

  /***/
  Texture( Image&& image, lifecycle_t lifecycle = lifecycle_t::eRender,
           int_t tws = URE_CLAMP_TO_EDGE,
           int_t twt = URE_CLAMP_TO_EDGE
         ) noexcept(true);

  /***/
  ~Texture() noexcept(true);

  /***/
  constexpr format_t    get_format() const noexcept(true)
  { return m_format; }

  /***/
  constexpr type_t      get_type() const noexcept(true)
  { return m_type; }

  /**
   * Return size in pixels.
   */
  constexpr const Size& get_size() const noexcept(true)
  { return m_size; }

  /**
   * @return pixels buffer length in bytes
   */
  constexpr uint32_t    get_length() const noexcept(true)
  { return m_length; }

  /***/
  constexpr uint8_t*    get_pixels() const noexcept(true)
  { return m_pixels; }

  /***/
  void_t                set_packing( int32_t param ) noexcept(true);
  /***/
  void_t                set_unpacking( int32_t param ) noexcept(true);

  /***/
  inline void_t         setParameterf( uint32_t target, uint32_t pname, GLfloat param ) noexcept(true);
  /***/
  inline void_t         setParameteri( uint32_t target, uint32_t pname, GLint param ) noexcept(true);

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
  void_t               render(  const std::vector<glm::vec2>& vertices, 
				                        const std::vector<glm::vec2>& texCoord,
				                        bool blend,
				                        enum_t target, int_t level, int_t uLocation, 
                                uint_t aVertices, uint_t aTexCoord,
				                        int_t  tws, int_t twt 
                              ) noexcept(true);
private:
  /***/
  bool_t tex_alloc() noexcept(true);
  /***/
  void_t tex_free() noexcept(true);
  /**/
  bool_t tex_create( enum_t target, int_t level, int_t tws = URE_CLAMP_TO_EDGE, int_t twt = URE_CLAMP_TO_EDGE ) noexcept(true);
  /***/
  bool_t tex_destroy() noexcept(true);

private:
  Size        m_size;
  format_t    m_format;
  type_t      m_type;
  lifecycle_t m_lifecycle;
  uint32_t    m_length;
  uint8_t*    m_pixels;
};

}

#endif // URE_TEXTURE_H
