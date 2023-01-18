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

#ifndef URE_RESOURCES_COLLECTOR_H
#define URE_RESOURCES_COLLECTOR_H

#include "ure_object.h"
#include "ure_common_defs.h"

#include <core/singleton.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <type_traits>

namespace ure {

#define DEFAULT_RESOURCES_PATH  "./media"

class ResourcesCollector final: public core::singleton_t<ResourcesCollector>
{
  friend class singleton_t<ResourcesCollector>;
public:
  /***/
  inline void                set_resources_path( const std::string& path ) noexcept
  { m_sResourcesPath = path; }
  /***/
  inline const std::string&  get_resources_path() const noexcept
  { return m_sResourcesPath; }
  
  /**
   * @brief Find specified resiyrce if registered.
   *        NOTE: return a pointer not the ownership, this means that returned pointer 
   *              can be used but not released, since it will be released from the collector.
   * 
   * @param name       resource name used to store a specified instance in the collector.
   * @return Object*   a pointer to the resource registered with \param name or nullptr if
   *                   does not exist. 
   */
  template<class derived_t>
    requires std::is_nothrow_convertible_v<derived_t*,Object*>
  derived_t*       find( const std::string& name ) noexcept
  {
    map_resources_t::const_iterator  iter = m_mapResources.find( name );
    if ( iter == m_mapResources.end() )
      return nullptr;

    return static_cast<derived_t*>(iter->second.get());
  }

  /**
   * @brief Check if specified resource name is already present in the collector.
   * 
   * @param name   resource name
   * @return true  if exist
   * @return false if not exist
   */
  inline bool  contains( const std::string& name ) const noexcept
  { return m_mapResources.contains(name); }
  
  /**
   * @brief Store specified resource inside the collector, but only if does not exist already a 
   *        resource with the same name.
   * 
   * @param name        unique identifier for the resource 
   * @param pResource   pointer to a Resource(Object). If the method return true ownership of 
   *                    this pointer will be taken from the collector.
   * @return true       in case a resource with \param name is not registered already.
   * @return false      in case a resource with \param name is already in the container.
   */
  bool            attach( const std::string& name, Object* pResource ) noexcept;
  /**
   * @brief Detach a resource from the collector.
   *        This means that collector return ownership to the caller.
   * 
   * @param name        resource name
   * @return Object*    pointer to specified resource if found inside the collector.
   *                    If specified resource name does not exist return value will be nullptr.
   */
  Object*         detach( const std::string& name ) noexcept;
  
public:
  /***/
  void_t on_initialize() noexcept
  { m_sResourcesPath = DEFAULT_RESOURCES_PATH; }
  /***/
  void_t on_finalize() noexcept
  {}
  
private:
  typedef std::unordered_map<std::string, std::unique_ptr<Object>>  map_resources_t;
private:
  std::string      m_sResourcesPath;
  map_resources_t  m_mapResources;
  
};

}

#endif // URE_RESOURCES_COLLECTOR_H
