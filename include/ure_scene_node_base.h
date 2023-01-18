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

#ifndef URE_SCENE_NODE_BASE_H
#define URE_SCENE_NODE_BASE_H

#include "ure_common_defs.h"
#include "ure_object.h"
#include "ure_animation.h"

namespace ure {

class Camera;

/**
 */
class SceneNodeBase : public Object
{
public:
  /***/
  SceneNodeBase( const std::string& nodetype, const std::string& name, Object* pObject )
    : Object(), m_sNodeType(nodetype), m_sName( name ), m_ptrObject( pObject )
  {}

  /***/
  virtual ~SceneNodeBase()
  {}


  inline constexpr const std::string& type() const noexcept
  { return m_sNodeType; }

  /**
   * Return node instance name.
   */
  inline constexpr const std::string& name() const noexcept
  { return m_sName; }
  
  /***/
  inline const Object*       get_object() const
  { return m_ptrObject.get(); }
  /***/
  inline Object*             get_object() 
  { return m_ptrObject.get(); }

  /***/
  Animation*                 get_animation()
  { return m_animation.get(); }
  /***/
  std::unique_ptr<Animation> set_animation( std::unique_ptr<Animation> animation )
  { 
    std::unique_ptr<Animation> prev_ani = std::move(m_animation);
    m_animation = std::move(animation); 
    return prev_ani;
  }
  
  /**
   * @param pCamera    pointer to the active camera it will provide
   *                   projection matrix.
   */
  virtual bool    render( const glm::mat4& mProjection, Camera* pCamera ) = 0;
  
protected:
  /***/
  virtual void    on_set_animation( Animation* ) {}
  
private:
  const std::string            m_sNodeType;    // node type intended to be initialized with derived class name.
  const std::string            m_sName;        // instance name
  std::unique_ptr<Object>      m_ptrObject;
  std::unique_ptr<Animation>   m_animation;
  
};

}

#endif // URE_SCENE_NODE_BASE_H
