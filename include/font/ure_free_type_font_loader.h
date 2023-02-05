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

#ifndef URE_FREE_TYPE_FONT_LOADER_H
#define URE_FREE_TYPE_FONT_LOADER_H

#include "ure_common_defs.h"
#include "ure_font_loader.h"

#include <thread>

/**
 *  Forward declaration will avoid FT includes.
 */
struct FT_LibraryRec_;
typedef struct FT_LibraryRec_  *FT_Library;


namespace ure {

namespace font {

/***/
class FreeTypeFontLoader final : public FontLoader
{
public:
  /***/
  FreeTypeFontLoader() noexcept;
  /***/
  virtual ~FreeTypeFontLoader();
  /**
   * @brief Create a font object
   * 
   * @param sFontFileName  font file name 
   * @param size           size to be used for the font
   * @return Font*         a valid pointer to Font instance in case of sucsess, otherwise
   *                       nullprt in case of failure.
   */
  virtual Font*  create_font( const std::string& sFontFileName, const Size& size ) noexcept override;
  /**
   * @brief         Release a font instance.
   * 
   * @param font    font instance to delete. 
   * @return true   if \param font have been successful destoyed in such case also font will be updated to nullptr
   * @return false  in one of the following circumstances:
   *                - Loader instance not initialized correctly
   *                - delete_font() have been invoked in a different thread than the one used to create the instance.
   *                - \param font is nullptr
   *                - \param font do not match with type FreeTypeFont
   */
  virtual bool   delete_font( Font*& font ) noexcept override;

private:
  std::thread::id  m_tid;
  FT_Library       m_library;   /* handle to library     */

};

}

}

#endif // URE_FREE_TYPE_FONT_LOADER_H
