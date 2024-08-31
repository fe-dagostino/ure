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

#ifndef URE_TRANSFORMATIONS_MATRIX_H
#define URE_TRANSFORMATIONS_MATRIX_H

#include "ure_common_defs.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace ure {

/**
 */
class transformation_matrix_t
{
public:
  /***/
  constexpr inline transformation_matrix_t( ) noexcept(true)
    : m_matrix( glm::mat4( 1.0f) )
  {}
  /***/
  constexpr inline transformation_matrix_t( const transformation_matrix_t& tmat ) noexcept(true)
    : m_matrix( tmat.m_matrix )
  {}
  /***/
  constexpr inline transformation_matrix_t( transformation_matrix_t&& tmat ) noexcept(true)
    : m_matrix( std::move(tmat.m_matrix) )
  {}
  /***/
  constexpr inline transformation_matrix_t( const glm::mat4& mat ) noexcept(true)
    : m_matrix( mat )
  {}
  /***/
  constexpr inline transformation_matrix_t( glm::mat4&& mat ) noexcept(true)
    : m_matrix( std::move(mat) )
  {}

  /***/
  constexpr inline ~transformation_matrix_t() noexcept(true)
  {}
  
  /***/
  constexpr inline const glm::mat4&  get() const noexcept(true)
  { return m_matrix; }
  
  /***/
  constexpr inline transformation_matrix_t&  operator = ( const transformation_matrix_t& tmat ) noexcept(true)
  { 
    m_matrix = tmat.m_matrix; 
    return *this;
  }
  /***/
  constexpr inline transformation_matrix_t&  operator = ( transformation_matrix_t&& tmat ) noexcept(true)
  { 
    m_matrix = std::move(tmat.m_matrix); 
    return *this;
  }

  /***/
  constexpr inline glm::mat4&  operator = ( const glm::mat4& mat ) noexcept(true)
  { 
    m_matrix = mat; 
    return m_matrix;
  }
  /***/
  constexpr inline glm::mat4&  operator = ( glm::mat4&& mat ) noexcept(true)
  { 
    m_matrix = std::move(mat); 
    return m_matrix;
  }
  
  /***/
  inline void_t  scale( double_t xs, double_t ys, double_t zs ) noexcept(true)
  { m_matrix = glm::scale( m_matrix, glm::vec3(xs,ys,zs) );  }
  /***/
  inline void_t  translate( double_t x, double_t y, double_t z ) noexcept(true)
  { m_matrix = glm::translate( m_matrix, glm::vec3(x,y,z) ); }
  /***/
  inline void_t  rotateX( float_t angle ) noexcept(true)
  { m_matrix = glm::rotate( m_matrix, angle, glm::vec3(1.0f,0.0f,0.0f) ); }
  /***/
  inline void_t  rotateY( float_t angle ) noexcept(true)
  { m_matrix = glm::rotate( m_matrix, angle, glm::vec3(0.0f,1.0f,0.0f) ); }
  /***/
  inline void_t  rotateZ( float_t angle ) noexcept(true)
  { m_matrix = glm::rotate( m_matrix, angle, glm::vec3(0.0f,0.0f,1.0f) ); }  
  
private:
  glm::mat4         m_matrix;
};

typedef transformation_matrix_t xform_matrix_t;

}

#endif // URE_TRANSFORMATIONS_MATRIX_H
