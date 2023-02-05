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

#include "font/ure_free_type_font_loader.h"
#include "font/ure_free_type_font.h"
#include "ure_utils.h"

#include <filesystem>
#include <typeinfo>

#include <core/utils.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H


namespace ure {

namespace font {

FreeTypeFontLoader::FreeTypeFontLoader() noexcept
{
  // Loader must be created / used and released from the same thread.
  m_tid = std::this_thread::get_id();

  FT_Error error = FT_Init_FreeType( &m_library );
  m_initialized  = ( error == 0 );
}

FreeTypeFontLoader::~FreeTypeFontLoader()
{
  if (m_initialized == false )
    return;

  if ( m_tid != std::this_thread::get_id() )
  {
    utils::err("FontLoader must be created, used and released within the same thread.");
  }
  
  FT_Error error = FT_Done_FreeType( m_library );
  if ( error != 0 )
  {
    utils::err("FreeTypeFontLoader failed to release resouces.");
  }
}
  
Font*  FreeTypeFontLoader::create_font( const std::string& sFontFileName, const Size& size ) noexcept
{
  if (m_initialized == false )
    return nullptr;

  // FontLoader must be created, used and released within the same thread.
  if ( m_tid != std::this_thread::get_id() )
  {
    utils::err("FontLoader must be created, used and released within the same thread.");
    return nullptr;
  }
  
  FT_Face      face  =  nullptr;
  std::string  sFont = core::utils::format( "%s/%s", get_fonts_path().c_str(), sFontFileName.c_str() );
  if ( std::filesystem::exists( sFont ) == false )
    return nullptr;
  
  FT_Error error = FT_New_Face( m_library, sFont.c_str(), 0, &face );
  if ( error != 0 )
  {
    return nullptr;
  }
      
  return new(std::nothrow) FreeTypeFont( face, size );
}

bool     FreeTypeFontLoader::delete_font( Font*& font ) noexcept
{
  if (m_initialized == false )
    return false;

  // FontLoader must be created, used and released within the same thread.
  if ( m_tid != std::this_thread::get_id() )
  {
    utils::err("FontLoader must be created, used and released within the same thread.");
    return false;
  }
  
  if ( font == nullptr )
    return false;
  
  const std::type_info& tiftf   = typeid(decltype(FreeTypeFont()));
  const std::type_info& tiparam = typeid(*font);

  if ( tiftf.name() != tiparam.name() )
    return false;
  
  delete font;
  font = nullptr;
  
  return true;  
}

}

}
