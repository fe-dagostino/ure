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

#ifndef URE_SCENE_NODE_H
#define URE_SCENE_NODE_H

#include "ure_common_defs.h"
#include "ure_scene_node_base.h"

#include "glm/glm.hpp"
#include <unordered_map>
#include <type_traits>

namespace ure {

class SceneNode : public SceneNodeBase
{
public:
  /***/
  SceneNode( const std::string& name, Object* pObject )
    : SceneNodeBase( "SceneNode", name, pObject )
  {}
  /***/
  virtual ~SceneNode()
  {}

  /**
   * @brief 
   * 
   * @param pSceneNode 
   * @return true 
   * @return false 
   */
  bool              add_scene_node( SceneNodeBase* pSceneNode );
  /** 
   */
  template<class derived_t>
    requires std::is_nothrow_convertible_v<derived_t*,SceneNodeBase*>  
  derived_t*        get_scene_node( const std::string& sNodeType, const std::string& sInstanceName ) noexcept
  {
    if ( sNodeType.empty() || sInstanceName.empty() )
      return nullptr;
    
    scene_node_maps_t::iterator  _iter = m_maps.find( sNodeType );
    if ( _iter == m_maps.end() )
      return nullptr;
    
    scene_node_map_t& rSceneNodeMap = *(_iter->second.get());

    scene_node_map_t::iterator _nodeIterator = rSceneNodeMap.find(sInstanceName);
    if ( _nodeIterator == rSceneNodeMap.end() )
      return nullptr;
    
    return static_cast<derived_t*>(_nodeIterator->second.get());
  }  

  /**
   * @param pCamera    pointer to the active camera it will provide
   *                   projection matrix.
   */
  virtual bool      render( const glm::mat4& mProjection, Camera* pCamera ) = 0;
    
protected:
  /***/
  virtual bool      on_add_scene_node( SceneNodeBase* ) { return true; };
    
protected:
  using scene_node_map_t  = std::unordered_map<std::string, std::unique_ptr<SceneNodeBase>>;
  //using scene_node_maps_t = std::unordered_map<std::string, std::unique_ptr<scene_node_map_t>>;
  using scene_node_maps_t = std::unordered_map<std::string, std::unique_ptr<scene_node_map_t>>;

  /***/
  const scene_node_map_t*   get_scene_node_map( const std::string& sNodeType ) const;
    
protected:
  scene_node_maps_t   m_maps;
};

}

#endif // URE_SCENE_NODE_H
