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

#ifndef URE_SCENE_LAYER_NODE_H
#define URE_SCENE_LAYER_NODE_H

#include "ure_scene_node.h"
#include "ure_transformations_matrix.h"
#include "widgets/ure_layer.h"

namespace ure {

/**
 */
class SceneLayerNode : public SceneNode
{
public:
  /**
   * @param name    it must be an unique name for the scene where it can be
   *                addressed using this name.
   * @param layer   a pointer to Layer instance; this instance can be shared
   *                between more nodes.
   */
  SceneLayerNode( const std::string& name, widgets::Layer* pLayer )
   : SceneNode( name, pLayer )
  {
    // Init model matrix
    m_matModel = glm::mat4( 1.0f );
  }
  /***/
  virtual ~SceneLayerNode()
  {}  
  
  /***/  
  virtual bool_t          render( const glm::mat4& mProjection, Camera* pCamera ) noexcept(true) override; 
  
  /***/
  inline xform_matrix_t&       get_model_matrix() noexcept
  { return m_matModel; }
  /***/
  inline const xform_matrix_t& get_model_matrix() const noexcept
  { return m_matModel; }
  /***/
  inline void_t         set_model_matrix( const xform_matrix_t& tmat ) noexcept
  { m_matModel = tmat; }
  /***/
  inline void_t         set_model_matrix( const glm::mat4& mat ) noexcept
  { m_matModel = mat;  }  

private:
  xform_matrix_t   m_matModel;
};

}

#endif // URE_SCENE_LAYER_NODE_H
