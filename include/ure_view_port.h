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

#ifndef URE_VIEW_PORT_H
#define URE_VIEW_PORT_H

#include "ure_object.h"
#include "ure_position.h"
#include "ure_size.h"
#include "ure_scene_graph.h"
#include "ure_transformations_matrix.h"

namespace ure {

class SceneGraph;

class ViewPort
{
public:
  /***/
  ViewPort() noexcept
    : m_ptrSceneGraph( nullptr ), m_pos( 0.0f, 0.0f ), m_size( 2, 2 ) 
  {
    m_matProjection = glm::mat4( 1.0f );
    reset();
  }

  /***/
  ViewPort( SceneGraph*& pSceneGraph, const glm::mat4& mProjection ) noexcept
    : m_ptrSceneGraph( nullptr ), m_pos( 0.0f, 0.0f ), m_size( 2, 2 )
  {
    set_scene(pSceneGraph);
    m_matProjection = mProjection;
    
    reset();
  }

  /***/
  ~ViewPort() noexcept
  {}

  /***/
  void  use() noexcept;
  /***/
  void  reset() noexcept;

  /**
   * GL_COLOR_BUFFER_BIT     Indicates the buffers currently enabled for color writing.
   * GL_DEPTH_BUFFER_BIT     Indicates the depth buffer.
   * GL_STENCIL_BUFFER_BIT   Indicates the stencil buffer.
   */
  void  clear_buffer( GLbitfield mask = GL_COLOR_BUFFER_BIT ) noexcept;

  /***/
  inline SceneGraph*  set_scene(SceneGraph*& pSceneGraph ) noexcept
  {
    SceneGraph* pRetVal = m_ptrSceneGraph.release();
    
    m_ptrSceneGraph.reset( pSceneGraph ); 
    pSceneGraph = nullptr;
    
    return pRetVal;
  }

  /***/
  inline SceneGraph*  get_scene() const noexcept
  { return m_ptrSceneGraph.get(); }

  /***/
  inline void         set_area( int_t x, int_t y, sizei_t width, sizei_t height ) noexcept
  {
    m_pos.x       = x;     m_pos.y       = y;
    m_size.width  = width; m_size.height = height;      
  }
  /***/
  inline void         get_area( int_t& x, int_t& y, sizei_t& width, sizei_t& height ) const noexcept
  {
    x      = m_pos.x;      y      = m_pos.y;
    width  = m_size.width; height = m_size.height;
  }

  /***/
  inline bool         render() noexcept
  {
    if ( m_ptrSceneGraph == nullptr )
      return false;
    
    return m_ptrSceneGraph->render( get_projection_matrix().get() );
  }

  /**
   * eg.
   * Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
   * glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
   */
  inline TMatrix&     get_projection_matrix() 
  { return m_matProjection; }

private:
  std::unique_ptr<SceneGraph> m_ptrSceneGraph;
  TMatrix                     m_matProjection;
  Position2D                  m_pos;
  Size                        m_size;
  
};

}

#endif // URE_VIEW_PORT_H
