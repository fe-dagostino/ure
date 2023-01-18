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

#ifndef URE_SCENE_CAMERA_NODE_H
#define URE_SCENE_CAMERA_NODE_H

#include "ure_common_defs.h"
#include "ure_camera.h"
#include "ure_scene_node_base.h"

#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

namespace ure {

class SceneCameraNode : public SceneNodeBase
{
public:
  /***/
  SceneCameraNode( const std::string& name, Camera* pCamera )
    : SceneNodeBase( "SceneCameraNode", name, pCamera )
  {}
  /***/
  virtual ~SceneCameraNode()
  {};
  
  /***/
  inline const Camera*   get_camera() const 
  { return static_cast<const Camera*>( get_object() ); }
  /***/
  inline Camera*         get_camera()
  { return dynamic_cast<Camera*>( get_object() ); }

  /***/  
  virtual bool render( [[__maybe_unused__]] const glm::mat4& mProjection, [[__maybe_unused__]] Camera* pCamera )
  { return true; };

protected:
  
};

}

#endif // URE_SCENE_CAMERA_NODE_H
