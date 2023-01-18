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

#include "ure_resources_collector.h"


namespace ure {

bool      ResourcesCollector::attach( const std::string& name, Object* pResource ) noexcept
{
  map_resources_t::const_iterator  iter = m_mapResources.find( name );
  if ( iter != m_mapResources.end() )
    return false;
  
  m_mapResources[name] = std::unique_ptr<Object>( pResource ); 
  
  return true;  
}

Object*   ResourcesCollector::detach( const std::string& name ) noexcept
{
  map_resources_t::iterator  iter = m_mapResources.find( name );
  if ( iter == m_mapResources.end() )
    return nullptr;
  
  Object* pResource = iter->second.release();
  
  m_mapResources.erase(iter);
  
  return pResource;
}

}