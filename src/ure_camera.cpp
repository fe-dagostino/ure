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

#include "ure_camera.h"

#include <glm/gtc/type_ptr.hpp>          // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale, glm::perspective

namespace ure {

Camera::Camera( bool bActive )
 : Object(), m_bActive( bActive ), 
   m_vPosition( 0, 0, 0 ), m_vCenter( 0, 0, 0 ), m_vUp( 0, 0, 0 )
{
  lookAt();
}

Camera::~Camera()
{

}

bool  Camera::set_active( bool bActive )
{
  bool    _bPrevValue = m_bActive;
  
  m_bActive = bActive;
  
  return  _bPrevValue;
}

void  Camera::set_position( const glm::vec3& vPos )
{
  m_vPosition     = vPos;
  lookAt();
}

void  Camera::set_center( const glm::vec3& vCenter )
{
  m_vCenter       = vCenter;
  lookAt();
}

void  Camera::set_up( const glm::vec3& vUp )
{
  m_vUp           = vUp;
  lookAt();
}

void  Camera::lookAt()
{
  m_matView = glm::lookAt( m_vPosition, m_vCenter, m_vUp );
}

}
