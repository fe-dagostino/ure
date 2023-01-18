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

#include "ure_image.h"
#include "ure_utils.h"

#include <core/utils.h>

#include "images/images.h"


namespace ure {

typedef unsigned char* (*load_image)( const char* filename, unsigned int* size, int* width, int* height );

Image::~Image()
{
  if ( m_pData != nullptr )
  {
    free( m_pData );
    m_pData = nullptr;
  }
}

bool  Image::load( Image::loader_t il, const std::string& filename ) noexcept
{
  load_image load   = NULL; 

  if ( il == Image::loader_t::eStb )
  {
#ifdef _USE_STB
    load = stb_load;
#else  //_USE_STB
    ure::utils::log( "Image::load() - Use: cmake -DURE_USE_STB=ON in order to enable STB Image loader" );
    return false;
#endif //_USE_STB   
  }

  if ( il == Image::loader_t::eFreeImage )
  {
#ifdef _USE_FREEIMAGE
    load = fi_load;
#else  //_USE_FREEIMAGE
    ure::utils::log( "Image::load() - Use: cmake -DURE_USE_FREEIMAGE=ON in order to enable FreeImage loader" );
    return false;
#endif //_USE_FREEIMAGE   
  }
  
  if ( il == Image::loader_t::eDevIL )
  {
#ifdef _USE_DEVIL
    load = il_load;
#else  //_USE_DEVIL
    ure::utils::log( "Image::load() - cmake -DURE_USE_DEVIL=ON in order to enable DevIL loader" );
    return false;
#endif //_USE_DEVIL    
  }

  if ( load == NULL )
    return false;
  
  m_pData = load( filename.c_str(), &m_uiDataSize, &m_size.width, &m_size.height );
  if (m_pData==nullptr)
  {
    ure::utils::log( core::utils::format( "Image::load() - Failed to load Texture [%s]", filename.c_str() ) );
    return false;
  }
    
  ure::utils::log( core::utils::format( "Image::load() - Loaded Texture [%s] MEM BYTES [%d] W:[%d] x H:[%d] Pixel D:[32] Bits", filename.c_str(), m_uiDataSize, m_size.width, m_size.height ) );
  
  return true;
}

uint8_t*  Image::detach( uint32_t* datasize ) noexcept
{
  uint8_t* _pRetValue = m_pData;

  if ( datasize != nullptr )
  { *datasize = m_uiDataSize; }

  m_uiDataSize = 0;
  m_size       = {0,0}; 
  m_format     = format_t::eUndefined;
  m_pData      = nullptr;

  return _pRetValue;
}

}
