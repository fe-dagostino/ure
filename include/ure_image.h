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

#ifndef URE_IMAGE_H
#define URE_IMAGE_H

#include "ure_handled_object.h"
#include "ure_size.h"

namespace ure {

/**
 */
class Image : public HandledObject
{
public:
  /**
   * Note: Both eFreeImage and eDevIL load the image in the same order 
   *       it has been stored, so if file format store the image from top
   *       to bottom or from bottom to top these two libraries don't take 
   *       care of it and image should be adjusted in rendering.
   *       eLibAV will load the image from top to bottom as for and 
   *       image viewer even the file format store the image in a different 
   *       manner. 
   *       This so eLibAV will be the best option if you don't knwo what
   *       kind of image will be loaded.
   */  
  enum class loader_t : int32_t {
    eStb,
    eFreeImage,
    eDevIL
  };

  enum class format_t : int32_t {
    eUndefined,
    eRGB  = URE_RGB,
    eRGBA = URE_RGBA
  };
  
  /***/
  Image() noexcept
    : m_size {0,0}, m_format(format_t::eRGBA), m_uiDataSize(0), m_pData(nullptr)
  {}

  /***/
  virtual ~Image();
  
  /**
   * Load and image using specified loader.
   */
  bool                     load( loader_t il, const std::string& filename ) noexcept;
  
  /***/
  constexpr format_t       get_format() const noexcept
  { return m_format; }
  /***/
  constexpr const Size&    get_size() const noexcept
  { return m_size; }
  /***/
  constexpr const uint8_t* get_data( uint32_t* datasize ) const noexcept
  { 
    if ( datasize != nullptr )
    { *datasize = m_uiDataSize; }

    return m_pData; 
  }

  /**
   * @brief Return data to the caller and empty current object.
   *        Caller will be in charge to release the data. 
   */
  uint8_t*                 detach( uint32_t* datasize ) noexcept;

public:
  Size     m_size; 
  format_t m_format;
  uint32_t m_uiDataSize;
  uint8_t* m_pData;

};

}

#endif // URE_IMAGE_H
