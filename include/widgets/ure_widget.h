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

#ifndef URE_WIDGET_H
#define URE_WIDGET_H

#include "ure_common_defs.h"
#include "ure_canvas.h"
#include "ure_position.h"
#include "ure_size.h"
#include "ure_rect.h"

#include <sigc++/sigc++.h>

namespace ure {

class ViewPort;
class Window;

namespace widgets {

class Layer;

class Widget : public Canvas, public sigc::trackable
{
public:
  using widgets_t = std::vector<std::unique_ptr<Widget>>;

  /***/
  enum BackgroundType
  {
    NoBackground,
    SolidColor,
    ImageBrush
  };
  /***/
  enum BackgroundOptions
  {
    eboUndefined,
    eboAsIs,
    eboFlipHorizontal,
    eboFlipVertical,
    eboFlipBoth
  };
  
  /**
   */
  Widget( Widget* pParent );
  /**
   */
  virtual ~Widget();
  
  /***/
  inline Widget*            get_parent() noexcept
  { return m_pParent; }
    
  /***/
  bool                      add_child( std::unique_ptr<Widget> widget ) noexcept;
  
  /***/
  inline widgets_t&         get_children() noexcept
  { return m_vChildren; }
  
  /**
   * @return TRUE if layer is visible.
   */
  inline bool               is_visible() const noexcept
  { return m_bVisible;     }
  /**
   * Set if layer is visible or not. When not visible render is disabled.
   */
  inline void_t             set_visible( bool visible ) noexcept
  { m_bVisible = visible; }  
  
  /**
   * @return TRUE if widget is enabled, so it will be receive envents
   *         like keys, mouse and so on.
   */
  inline bool               is_enabled() const noexcept
  { return m_bEnabled;     }
  /***/
  inline void_t             set_enabled( bool enabled ) noexcept
  { m_bEnabled = enabled;     }
  
  /**
   * @return TRUE if it is referred by parent window as current focused.
   * @return FALSE if the widget has no parent or focus is on an other widget.
   */
  bool                      has_focus() const noexcept;
  /**
   * Must be reimplemented at specialized class in order to
   * return a value that differs from default 'false'.
   * Return value should be 'true' if current widget or its children
   * can take the focus.
   */
  virtual bool              is_focusable() const noexcept
  { return false; }
  
  /***/
  inline void_t             set_position( const Position2D& pos ) noexcept
  {  set_position( pos.x, pos.y, true ); }
  /***/
  void_t                    set_position( int_t x, int_t y, bool notify ) noexcept;
  /***/
  inline const Position&    get_position() const noexcept
  { return m_pos; }
  
  /***/
  inline void_t             set_size( const Size& size ) noexcept
  { set_size( size.width, size.height, true ); }
  /***/
  void_t                    set_size( sizei_t width, sizei_t height, bool notify ) noexcept;  
  /***/
  inline const Size&        get_size() noexcept
  { return m_size; }
  
  /**
   */
  inline void_t             get_client_area( Recti& rect ) noexcept
  {
    rect.left  = get_position().x; rect.top    = get_position().y;
    rect.right = get_size().width; rect.bottom = get_size().height;
  }
  
  /***/    
  inline bool               has_background() const noexcept
  { return (m_eBackground!=NoBackground); }
  /***/
  inline void_t             set_background( const glm::vec4& cr )
  {  
    m_crBackground = cr;     
    m_eBackground  = SolidColor;
  }
  /**
   * @bo  usually image loaded from files needs a vertival flip in order to be shown as original.
   *      The reason is related to format type used to store image where images are store starting
   *      from the bottom line in the image.
   */
  void_t                          set_background( std::shared_ptr<ure::Texture> texture, BackgroundOptions bo );
  
  inline const glm::vec4&         get_bk_color() const
  { return m_crBackground;   }
  inline std::shared_ptr<Texture> get_bk_image() const
  { return m_bkg_texture;   }
  
  /***/
  bool                      draw( const glm::mat4& mvp, const Recti& rect );
  
public:
  using OnClick_Signal = sigc::signal<void_t(Widget*)>;
  
  OnClick_Signal            signalOnClick;
  
protected:
  /**
   * Default implementation will check if current widget has th focus and then will 
   * propagate the event to all child widgets.
   * Usual implementation propagate event only to focused child, that can be done
   * exending the class with a specialized event.
   */
  virtual void_t   on_key_released( Window* pGlWindow, Layer* pGlLayer, int_t iKey, int_t iScanCode, word_t wMods ) noexcept;
  /**
   * Default implementation will check if current widget has th focus and then will 
   * propagate the event to all child widgets.
   * Usual implementation propagate event only to focused child, that can be done
   * exending the class with a specialized event.
   */
  virtual void_t   on_key_pressed ( Window* pGlWindow, Layer* pGlLayer, int_t iKey, int_t iScanCode, word_t wMods ) noexcept;
  /**
   * Default implementation will check if current widget has th focus and then will 
   * propagate the event to all child widgets.
   * Usual implementation propagate event only to focused child, that can be done
   * exending the class with a specialized event.
   */
  virtual void_t   on_key_repeated( Window* pGlWindow, Layer* pGlLayer, int_t iKey, int_t iScanCode, word_t wMods ) noexcept;
  
protected:
  virtual void_t   on_begin_drawing( [[maybe_unused]] const Recti& rect ) noexcept
  {}
  /**
   * Called before drawing background. To hinibit default drawing return 'false'.
   */
  virtual bool     on_draw_background( [[maybe_unused]] const Recti& rect ) noexcept
  { return true; }
  /***/
  virtual bool     on_draw( [[maybe_unused]] const Recti& rect ) noexcept
  { return true; }
  /***/
  virtual void_t   on_end_drawing( [[maybe_unused]] const Recti& rect ) noexcept
  {}
  /***/
  virtual void_t   on_position_changed( [[maybe_unused]] const Position& pos ) noexcept
  {}
  /***/
  virtual void_t   on_size_changed( [[maybe_unused]] const Size& size ) noexcept
  {}

protected:
  /***/
  virtual bool     on_update_background_vertices() noexcept
  { return true; }
  
protected:
  /***/
  bool   step_focus();
  /***/
  bool   has_focusable();
  /**
   * NOTE: this method should be called only when current 
   *       widget is both visible and enabled.
   */
  void_t handleDefaultKeys( int_t iKey, int_t iScanCode );
  
private:

  /***/
  void_t _updateBkVertices() noexcept;

protected:
  using Key_Signal = sigc::signal<void_t(Window*, Layer*, int_t, int_t, word_t)>;
  
  Key_Signal   signalKeyReleased;
  Key_Signal   signalKeyPressed;
  Key_Signal   signalKeyRepeated;
  
  /***/
  void_t       set_parent( Widget* pParent );

// Used for disconnection.
private:
  sigc::connection   slotKeyReleased;
  sigc::connection   slotKeyPressed;
  sigc::connection   slotKeyRepeated;
  
private:
  BackgroundOptions             m_ebo;
  Position                      m_pos;
  Size                          m_size;
  bool                          m_bVisible;
  bool                          m_bEnabled;
  Widget*                       m_pParent;
  widgets_t                     m_vChildren;
  widgets_t::iterator           m_Focus;
  BackgroundType                m_eBackground;
  glm::vec4                     m_crBackground;
  std::shared_ptr<ure::Texture> m_bkg_texture;
  
protected:
  std::vector<glm::vec2>    m_bkVertices;
  std::vector<glm::vec2>    m_bkTexCoord;  
};

}

}

#endif // URE_WIDGET_H
