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

#include "ure_scene_graph.h"


#define GLM_FORCE_RADIANS
#define GLM_PRECISION_MEDIUMP_FLOAT
#include <glm/gtc/type_ptr.hpp>          // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale, glm::perspective

namespace ure {

void_t  SceneGraph::set_background( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha ) noexcept(true)
{
  m_red   = red;
  m_green = green;
  m_blue  = blue;
  m_alpha = alpha;

  ///////////////////
  // Reset background color
  glClearColor( m_red, m_green, m_blue, m_alpha );
}

void_t  SceneGraph::get_background( GLclampf& red, GLclampf& green, GLclampf& blue, GLclampf& alpha ) noexcept(true)
{
  red     = m_red;
  green   = m_green;
  blue    = m_blue;
  alpha   = m_alpha;
}

const SceneCameraNode*  SceneGraph::get_active_camera() const noexcept(true)
{
  const scene_node_map_t* mapCameras = get_scene_node_map( "SceneCameraNode" );
  if ( mapCameras == nullptr )
    return nullptr;
  
  for( auto& NodeBasePtr : *mapCameras )
  {
    const SceneCameraNode* pCameraNode = dynamic_cast<const SceneCameraNode*>(NodeBasePtr.second.get());
    if ( pCameraNode->get_camera()->is_active() )
      return pCameraNode;
  }

  return nullptr;
}
  
bool  SceneGraph::render( const glm::mat4& mProjection ) noexcept(true)
{
  const SceneCameraNode* pActiveCamera = get_active_camera();
  Camera*                pCamera       = (pActiveCamera!=nullptr)?(Camera*)pActiveCamera->get_camera():nullptr; 

  return render( mProjection, pCamera );
}

bool  SceneGraph::render( const glm::mat4& mProjection, Camera* pCamera ) noexcept(true)
{ 
  for ( auto n : m_vRender )
  {
    n->render( mProjection, pCamera );
  }
  
  return true; 
}
 
bool  SceneGraph::on_add_scene_node( SceneNodeBase* pSceneNode ) noexcept(true)
{
  m_vRender.push_back( pSceneNode );
  return true;
}

}
