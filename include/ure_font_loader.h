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

#ifndef URE_FONT_LOADER_H
#define URE_FONT_LOADER_H

#include "ure_common_defs.h"
#include "ure_font.h"

namespace ure {

class FontLoader 
{
public:
  /***/
  FontLoader()
    : m_initialized(false)
  {}
  /***/
  virtual ~FontLoader()
  {}
  
  /***/
  inline bool is_initialized() const noexcept
  { return m_initialized; }

  /***/
  inline const std::string& get_fonts_path( ) const noexcept
  { return m_sFontsPath; }
  /***/
  inline void_t             set_fonts_path( const std::string& sPath ) noexcept
  { m_sFontsPath = sPath; }
  
  /**
   * @param sFontFileName   
   */	
  virtual Font*      create_font( const std::string& sFontFileName, const Size& size ) = 0;
  /***/
  virtual bool       delete_font( Font*& font ) = 0;

protected:    
  bool          m_initialized;  
private:
  std::string   m_sFontsPath;
  
  friend class Font;
};

}

#endif // URE_FONT_LOADER_H
