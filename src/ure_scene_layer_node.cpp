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

#include "ure_scene_layer_node.h"
#include "ure_camera.h"

namespace ure {


  
bool SceneLayerNode::render( const glm::mat4& mProjection, Camera* pCamera )
{
  widgets::Layer* layer = get_object<widgets::Layer>();
  if ( layer == nullptr )
    return false;
  
  if ( layer->is_visible() == false )
    return false;

  // Retrieving camera (view) matrix from active camera.
  // If not present an identity matrix will be used.
  glm::mat4  mView = (pCamera==nullptr)?glm::mat4(1.0f):pCamera->get_view_matrix().get();
  
  glm::mat4  mvp =  mProjection * mView * m_matModel.get();
  if (get_animation() != nullptr)
    mvp *= get_animation()->get_matrix().get();

  return layer->render( mvp );
}

}
