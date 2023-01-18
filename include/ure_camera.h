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
  Camera( bool bActive );
  /***/
  virtual ~Camera();
  
  /**
   */
  inline bool                    is_active() const
  { return m_bActive; }
  /***/
  bool                           set_active( bool bActive );
  
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
  inline TMatrix&       get_view_matrix() noexcept
  { return m_matView; }
  inline const TMatrix& get_view_matrix() const noexcept
  { return m_matView; }
  inline void_t         set_view_matrix( const TMatrix& tmat ) noexcept
  { m_matView = tmat; }
  inline void_t         set_view_matrix( const glm::mat4& mat ) noexcept
  { m_matView = mat;  }
  
  /***/  
  const Position3D&     get_position() const
  { return m_vPosition; }
  /***/
  void                  set_position( const Position3D& vPos );
  
  /***/
  const Position3D&     get_center() const
  { return m_vCenter; }
  /***/
  void                  set_center( const Position3D& vCenter );

  /***/
  const Position3D&     get_up() const
  { return m_vUp; }
  /***/
  void                  set_up( const Position3D& vUp );
  
private:
  /***/
  void lookAt();
  
protected:
  bool         m_bActive;
  TMatrix      m_matView;
  
  Position3D   m_vPosition;  // Camera Position (eyes)
  Position3D   m_vCenter;    // Where the camera is pointed to
  Position3D   m_vUp;        // Top of the camera
};

}

#endif // URE_CAMERA_H
