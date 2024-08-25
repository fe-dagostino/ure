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
#include <optional>

namespace ure {

#define DEFAULT_RESOURCES_PATH  "./media"

/**
 * 
 */
class ResourcesCollector final 
{
public:
  enum class resource_flag_t : word_t {
    EXPENDABLE = 0x0000,         /* Resource can be released or replaced in order to limit memory usage */  
    PRESERVE   = 0x0001,         /* Resource will not be removed if not explicity requested.            */
  };

  ResourcesCollector( const std::string_view& path = DEFAULT_RESOURCES_PATH ) noexcept(true)
    : m_resources_path(path)
  {}

  ~ResourcesCollector() noexcept(true)
  {}

  /***/
  constexpr void                set_resources_path( const std::string& path ) noexcept(true)
  { m_resources_path = path; }
  /***/
  constexpr const std::string&  get_resources_path() const noexcept(true)
  { return m_resources_path; }
  
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
  std::optional<std::shared_ptr<derived_t>>   find( const std::string& name ) noexcept(true)
  {
    map_resources_t::const_iterator  iter = m_mapResources.find( name );
    if ( iter == m_mapResources.end() )
      return std::nullopt;

    return std::static_pointer_cast<derived_t>( iter->second->_obj );
  }

  /**
   * @brief Check if specified resource name is already present in the collector.
   * 
   * @param name   resource name
   * @return true  if exist
   * @return false if not exist
   */
  inline bool_t  contains( const std::string& name ) const noexcept(true)
  { return m_mapResources.contains(name); }
  
  /**
   * @brief Store specified resource inside the collector, but only if does not exist already a 
   *        resource with the same name.
   * 
   * @param name             unique identifier for the resource 
   * @param resource         pointer to a Resource(Object). If the method return true ownership of 
   *                         this pointer will be taken from the collector.
   * @return true,nullptr    in case a resource with \param name is not already stored in the collector.
   * @return false,resource  in case the collector already contains a resource with \param name.
   */
  template<class derived_t, typename pointer_t = std::shared_ptr<derived_t> >
    requires std::is_nothrow_convertible_v<derived_t*,Object*> && 
             ( std::same_as<pointer_t,std::shared_ptr<derived_t>> || std::same_as<pointer_t,std::unique_ptr<derived_t>> )
  constexpr std::pair<bool,pointer_t>   attach(  const std::string& name, 
                                                 pointer_t resource, 
                                                 word_t flags = ure::word_t(ure::ResourcesCollector::resource_flag_t::EXPENDABLE) 
                                              ) noexcept(true)
  {
    if ( m_mapResources.contains( name ) == true )
      return std::make_pair(false, std::move(resource));
  
    m_mapResources[name] = std::make_unique<item_t>( flags, std::move(resource) ); 
  
    return std::make_pair(true, nullptr);
  }

  /**
   * @brief Detach a resource from the collector.
   *        This means that collector return ownership to the caller.
   * 
   * @param name        resource name
   * @return Object*    pointer to specified resource if found inside the collector.
   *                    If specified resource name does not exist return value will be nullptr.
   */
  template<class derived_t>
    requires std::is_nothrow_convertible_v<derived_t*,Object*>
  std::shared_ptr<derived_t>      detach( const std::string& name ) noexcept(true)
  {
    map_resources_t::iterator  iter = m_mapResources.find( name );
    if ( iter == m_mapResources.end() )
      return nullptr;
    
    return std::static_pointer_cast<derived_t>( m_mapResources.extract(iter).mapped()->_obj );
  }
 
private:
  struct item_t {
    item_t( word_t flags, std::shared_ptr<Object> obj )
    : _flags(flags), _obj(std::move(obj))
    {}
    
    word_t                   _flags;
    std::shared_ptr<Object>  _obj;
  };

  typedef std::unordered_map<std::string, std::unique_ptr<item_t>>  map_resources_t;
private:
  std::string      m_resources_path;
  map_resources_t  m_mapResources;
  
};

}

#endif // URE_RESOURCES_COLLECTOR_H
