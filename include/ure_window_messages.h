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

#ifndef URE_WINDOW_MESSAGES_H
#define URE_WINDOW_MESSAGES_H

#include "ure_message.h"

namespace ure {

/**
 * Specialized message class intended to be used for showing window if hidden.
 */
class ShowWindowMsg final : public Message
{
public:
  ShowWindowMsg( Window* pWindow ) noexcept
   : Message( pWindow )
  {}
  
  virtual ~ShowWindowMsg() noexcept
  {}
  
  virtual bool on_execute() noexcept override
  { return m_pWindow->show( static_cast<enum_t>(Window::processing_flag_t::epfCalling) ); }
};

/**
 * Specialized message class intended to be used for hidding the window.
 */
class HideWindowMsg final : public Message
{
public:
  HideWindowMsg( Window* pWindow ) noexcept
   : Message( pWindow )
  {}
  
  virtual ~HideWindowMsg() noexcept
  {}
  
  virtual bool on_execute() noexcept override
  { return m_pWindow->hide( static_cast<enum_t>(Window::processing_flag_t::epfCalling) ); }
};

/**
 * Specialized message class intended to be used for creating a new window.
 */
class CreateWindowMsg final : public Message
{
public:
  CreateWindowMsg( Window* pWindow, std::unique_ptr<window_options> options ) noexcept
    : Message( pWindow ), m_ptrWinOptions( std::move(options) )
  {}
  
  virtual ~CreateWindowMsg() noexcept
  {}
  
  virtual bool on_execute() noexcept override
  { return m_pWindow->create( std::move(m_ptrWinOptions), static_cast<enum_t>(Window::processing_flag_t::epfCalling) ); }
  
private:
  std::unique_ptr<window_options> m_ptrWinOptions;
};

/**
 * Specialized message class intended to be used for updating window title.
 */
class SetWindowTitleMsg final : public Message
{
public:
  SetWindowTitleMsg( Window* pWindow, const std::string& sTitle ) noexcept
   : Message( pWindow ), m_sTitle( sTitle )
  {}
  
  virtual ~SetWindowTitleMsg() noexcept
  {}
  
  virtual bool on_execute() noexcept override
  { return m_pWindow->set_title( m_sTitle, static_cast<enum_t>(Window::processing_flag_t::epfCalling) ); }
  
private:
  const std::string  m_sTitle;
};

/**
 * Specialized message class intended to be used for updating window position.
 */
class SetWindowPositionMsg final : public Message
{
public:
  SetWindowPositionMsg( Window* pWindow, const position_t<int_t>& position ) noexcept
   : Message( pWindow ), m_position(position)
  {}
  
  virtual ~SetWindowPositionMsg() noexcept
  {}
  
  virtual bool on_execute() noexcept override
  { return m_pWindow->set_position( m_position, static_cast<enum_t>(Window::processing_flag_t::epfCalling) ); }
  
private:
  const position_t<int_t> m_position;
};

/**
 * Specialized message class intended to be used for updating window size.
 */
class SetWindowSizeMsg final : public Message
{
public:
  SetWindowSizeMsg( Window* pWindow, const Size& size ) noexcept
   : Message( pWindow ), m_size(size)
  {}
  
  virtual ~SetWindowSizeMsg() noexcept
  {}
  
  virtual bool on_execute() noexcept override
  { return m_pWindow->set_size( m_size, static_cast<enum_t>(Window::processing_flag_t::epfCalling) ); }
  
private:
  const Size m_size;
};

/**
 * Specialized message class intended to be used to restore window.
 */
class ShowWindowNormalMsg final : public Message
{
public:
  ShowWindowNormalMsg( Window* pWindow ) noexcept
   : Message( pWindow )
  {}
  
  virtual ~ShowWindowNormalMsg() noexcept
  {}
  
  virtual bool on_execute() noexcept override
  { return m_pWindow->show_normal( static_cast<enum_t>(Window::processing_flag_t::epfCalling) ); }
};

/**
 * Specialized message class intended to be used to iconify window.
 */
class ShowWindowMinimizedMsg final : public Message
{
public:
  ShowWindowMinimizedMsg( Window* pWindow ) noexcept
   : Message( pWindow )
  {}
  
  virtual ~ShowWindowMinimizedMsg() noexcept
  {}
  
  virtual bool on_execute() noexcept override
  { return m_pWindow->show_minimized( static_cast<enum_t>(Window::processing_flag_t::epfCalling) ); }
};

/**
 * Specialized message class intended to be destroy window.
 */
class DestroyWindowMsg final : public Message
{
public:
  DestroyWindowMsg( Window* pWindow ) noexcept
   : Message( pWindow )
  {}
  
  virtual ~DestroyWindowMsg() noexcept
  {}
  
  virtual bool on_execute() noexcept override
  { return m_pWindow->destroy( static_cast<enum_t>(Window::processing_flag_t::epfCalling) ); }
};

}

#endif // URE_WINDOW_MESSAGES_H