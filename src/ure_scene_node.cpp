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

#include "ure_scene_node.h"

namespace ure {

bool  SceneNode::add_scene_node( SceneNodeBase* pSceneNode )
{  
  const std::string& sNodeType       = pSceneNode->type();
  
  scene_node_maps_t::iterator  _iter = m_maps.find( sNodeType );
  if ( _iter == m_maps.end() )
  {
    m_maps[sNodeType] = std::make_unique<scene_node_map_t>();
    
    // When map is not present it will be added and then a recursive call to 
    // addSceneNode will add the node to it.
    return add_scene_node( pSceneNode );
  }
  
  scene_node_map_t& rSceneNodeMap = *_iter->second.get();
  
  // If a scene node with the same instance name is already present the function return false.
  if ( rSceneNodeMap.find(pSceneNode->name()) != rSceneNodeMap.end() )
    return false;

  // Scene Node will be registered and then function return TRUE 
  rSceneNodeMap[pSceneNode->name()] = std::unique_ptr<SceneNodeBase>(pSceneNode);

  return on_add_scene_node(pSceneNode);
}

const SceneNode::scene_node_map_t*   SceneNode::get_scene_node_map( const std::string& sNodeType ) const
{
  scene_node_maps_t::const_iterator  _iter = m_maps.find( sNodeType );
  if ( _iter == m_maps.end() )
    return nullptr;
  
  return (_iter->second.get());
}

}
