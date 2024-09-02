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
  /**
   * @param object     can be shared over different scene nodes.
  */
  SceneNodeBase( const std::string& node_type, const std::string& name, std::shared_ptr<Object> object ) noexcept(true)
    : Object(), m_node_type(node_type), m_name( name ), m_object( object )
  {}

  /***/
  virtual ~SceneNodeBase() noexcept(true)
  {}

  /***/
  inline constexpr const std::string& type() const noexcept(true)
  { return m_node_type; }

  /**
   * Return node instance name.
   */
  inline constexpr const std::string& name() const noexcept(true)
  { return m_name; }
  
  /***/
  inline bool_t has_object() const noexcept(true)
  { return (bool)m_object; }

  /***/
  template<class derived_t>
    requires std::is_nothrow_convertible_v<derived_t*,Object*>
  inline const std::shared_ptr<derived_t>  get_object() const noexcept(true)
  { return std::static_pointer_cast<derived_t>(m_object); }

  /***/
  template<class derived_t>
    requires std::is_nothrow_convertible_v<derived_t*,Object*>
  inline std::shared_ptr<derived_t> get_object() noexcept(true)
  { return std::static_pointer_cast<derived_t>(m_object); }

  inline bool_t has_animation() const noexcept(true)
  { return (bool)m_animation; }
  /***/
  Animation&    get_animation() noexcept(true)
  { return *m_animation; }
  /***/
  std::unique_ptr<Animation> set_animation( std::unique_ptr<Animation> animation ) noexcept(true)
  { 
    std::unique_ptr<Animation> prev_ani = std::move(m_animation);
    m_animation = std::move(animation); 
    return prev_ani;
  }
  
  /**
   * @param camera     pointer to the active camera it will provide the view matrix.
   */
  virtual bool_t    render( const glm::mat4& mProjection, const std::shared_ptr<Camera>& camera ) noexcept(true) = 0;
  
protected:
  /***/
  virtual void_t    on_set_animation( Animation* ) noexcept(true)
  {}
  
private:
  const std::string            m_node_type;    /* node type intended to be initialized with derived class name. */
  const std::string            m_name;         /* instance name */
  std::shared_ptr<Object>      m_object;
  std::unique_ptr<Animation>   m_animation;
  
};

}

#endif // URE_SCENE_NODE_BASE_H
