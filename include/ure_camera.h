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

#ifndef URE_CAMERA_H
#define URE_CAMERA_H

#include "ure_object.h"
#include "ure_position.h"
#include "ure_transformations_matrix.h"

#include "glm/glm.hpp"

namespace ure {


class Camera : public Object
{
public:
  
  /***/
  Camera( bool_t active ) noexcept(true)
    : Object(), m_active( active ), 
      m_camera_position( 0, 0, 0 ), 
      m_camera_center  ( 0, 0, 0 ), 
      m_camera_top     ( 0, 0, 0 )
  { lookAt(); }

  /***/
  constexpr virtual ~Camera() noexcept(true)
  {}
  
  /**
   */
  constexpr bool_t                is_active() const noexcept(true)
  { return m_active; }
  /***/
  constexpr bool_t                set_active( bool_t active ) noexcept(true)
  {
    bool_t    _old_value = m_active;
    m_active = active;
    return  _old_value;
  }

  /**
   * glm::vec3 cameraPosition = glm::vec3(4,3,3); // Camera is at (4,3,3), in World Space
   * glm::vec3 cameraTarget   = glm::vec3(0,0,0); // and looks at the origin
   * glm::vec3 upVector       = glm::vec3(0,1,0);  // Head is up (set to 0,-1,0 to look upside-down)
   *
   * glm::mat4 CameraMatrix = glm::lookAt(
   *                                         cameraPosition, // the position of your camera, in world space
   *                                         cameraTarget,   // where you want to look at, in world space
   *                                         upVector        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
   *                                     );
   */
  constexpr xform_matrix_t&       get_view_matrix() noexcept(true)
  { return m_view_matrix; }

  template<typename data_t>
    requires std::same_as<data_t,xform_matrix_t> || std::same_as<data_t,glm::mat4>
  constexpr void_t                set_view_matrix( const data_t& tmat ) noexcept(true)
  { m_view_matrix = tmat; }
  
  /***/  
  constexpr const Position3D&     get_position() const noexcept(true)
  { return m_camera_position; }
  /***/
  inline void_t                   set_position( const Position3D& camera_position ) noexcept(true)
  {
    m_camera_position = camera_position;
    lookAt();
  }

  /***/
  constexpr const Position3D&     get_center() const noexcept(true)
  { return m_camera_center; }
  /***/
  inline void_t                   set_center( const Position3D& camera_center ) noexcept(true)
  {
    m_camera_center   = camera_center;
    lookAt();
  }

  /***/
  constexpr const Position3D&     get_up() const noexcept(true)
  { return m_camera_top; }
  /***/
  inline void_t                   set_up( const Position3D& camera_top ) noexcept(true)
  {
    m_camera_top      = camera_top;
    lookAt();
  }

private:
  /***/
  inline void_t lookAt() noexcept(true)
  { m_view_matrix = glm::lookAt( m_camera_position, m_camera_center, m_camera_top ); }
  
protected:
  bool_t          m_active;
  xform_matrix_t  m_view_matrix;
  
  Position3D      m_camera_position;  // Camera Position (eyes)
  Position3D      m_camera_center;    // Where the camera is pointed to
  Position3D      m_camera_top;       // Top of the camera
};

using camera_ptr = std::shared_ptr<ure::Camera>;

}

#endif // URE_CAMERA_H
