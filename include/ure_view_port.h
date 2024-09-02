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
  ViewPort() noexcept(true)
    : m_scene_graph( nullptr ), m_projection_matrix( glm::mat4( 1.0f ) ),
      m_pos( 0.0f, 0.0f ), m_size( 2, 2 ) 
  {
    reset();
  }

  /***/
  ViewPort( std::unique_ptr<SceneGraph> scene_graph, const glm::mat4& projection_matrix ) noexcept(true)
    : m_scene_graph( std::move(scene_graph) ), m_projection_matrix( projection_matrix ),
      m_pos( 0.0f, 0.0f ), m_size( 2, 2 )
  {
    m_projection_matrix = projection_matrix;
    
    reset();
  }

  /***/
  ~ViewPort() noexcept(true)
  {}

  /***/
  void_t use() noexcept(true);
  /***/
  void_t reset() noexcept(true);

  /**
   * GL_COLOR_BUFFER_BIT     Indicates the buffers currently enabled for color writing.
   * GL_DEPTH_BUFFER_BIT     Indicates the depth buffer.
   * GL_STENCIL_BUFFER_BIT   Indicates the stencil buffer.
   */
  void_t clear_buffer( GLbitfield mask = GL_COLOR_BUFFER_BIT ) noexcept(true);

  /***/
  inline std::unique_ptr<SceneGraph>  set_scene( std::unique_ptr<SceneGraph> scene_graph ) noexcept(true)
  {
    std::unique_ptr<SceneGraph> old_sg = std::move(m_scene_graph);
    m_scene_graph = std::move(scene_graph);
    return old_sg;
  }

  /***/
  inline bool_t has_scene_graph() const noexcept(true)
  { return (m_scene_graph != nullptr); }

  /***/
  inline SceneGraph&  get_scene() const noexcept(true)
  { return *m_scene_graph; }

  /***/
  constexpr void_t    set_area( int_t x, int_t y, sizei_t width, sizei_t height ) noexcept(true)
  {
    m_pos.x       = x;     m_pos.y       = y;
    m_size.width  = width; m_size.height = height;      
  }
  /***/
  constexpr void_t    get_area( int_t& x, int_t& y, sizei_t& width, sizei_t& height ) const noexcept(true)
  {
    x      = m_pos.x;      y      = m_pos.y;
    width  = m_size.width; height = m_size.height;
  }
  /***/
  constexpr int_t   x()      const noexcept(true)
  { return m_pos.x; }
  /***/
  constexpr int_t   y()      const noexcept(true)
  { return m_pos.y; }
  /***/
  constexpr sizei_t width()  const noexcept(true)
  { return m_size.width; }
  /***/
  constexpr sizei_t height() const noexcept(true)
  { return m_size.height; }

  /***/
  inline bool_t     render() noexcept(true)
  {
    if ( m_scene_graph == nullptr )
      return false;
    
    return m_scene_graph->render( get_projection_matrix().get() );
  }

  /**
   * eg.
   * Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
   * glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
   */
  constexpr xform_matrix_t&   get_projection_matrix() noexcept(true)
  { return m_projection_matrix; }

private:
  std::unique_ptr<SceneGraph> m_scene_graph;
  xform_matrix_t              m_projection_matrix;
  Position2D                  m_pos;
  Size                        m_size;
  
};

}

#endif // URE_VIEW_PORT_H
