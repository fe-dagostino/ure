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

#ifndef URE_SCENE_GRAPH_H
#define URE_SCENE_GRAPH_H

#include "ure_scene_node.h"
#include "ure_scene_camera_node.h"

#include <map>
#include <vector>

namespace ure {

class SceneGraph : public SceneNode
{
public:
  /***/
  SceneGraph()
    : SceneNode( "Root", nullptr ),
      m_red(0.0f), m_green(0.0f), m_blue(0.0f), m_alpha(1.0f)
  {
  }

  /***/
  ~SceneGraph() noexcept(true)
  { }
  
  /**
   * Return current active camera that will be used during render operation.
   * If there is no camera, return value will be nullptr and identity matrix
   * will be used for render.
   */
  const SceneCameraNode*  get_active_camera() const noexcept(true);
    
  /***/
  void    set_background( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha ) noexcept(true);
  /***/
  void    get_background( GLclampf& red, GLclampf& green, GLclampf& blue, GLclampf& alpha ) noexcept(true);
  
  /***/
  bool_t  render( const glm::mat4& mProjection ) noexcept(true);
  
protected:  
  /**
   * Iterate on all childs in order to render them.
   * 
   * @todo implements rendering order
   */
  virtual bool_t render( const glm::mat4& mProjection, Camera* pCamera ) noexcept(true);

  /***/
  virtual bool_t on_add_scene_node( SceneNodeBase* pSceneNode ) noexcept(true);
  
private:
  GLclampf   m_red, m_green, m_blue, m_alpha;  
  
  std::vector<SceneNodeBase*>    m_vRender;
};

}

#endif // URE_SCENE_GRAPH_H
