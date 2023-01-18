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

#ifndef URE_PROGRAMS_COLLECTOR_H
#define URE_PROGRAMS_COLLECTOR_H

#include "ure_common_defs.h"
#include "ure_program.h"

#include <core/singleton.h>
#include <unordered_map>
#include <vector>
#include <string>

namespace ure {

#define DEFAULT_SHADER_PATH  "./shaders"

/**
 * Singleton used in order to store shared programs.
 */
class ProgramsCollector final: public core::singleton_t<ProgramsCollector>
{
  friend class singleton_t<ProgramsCollector>;
public:
  /***/
  inline void_t              set_shaders_path( const std::string& path ) noexcept
  { m_sShadersPath = path; }
  /***/
  inline const std::string&  get_shaders_path() const noexcept
  { return m_sShadersPath; }
  
  /**
   * @brief Find specified program if registered.
   *        NOTE: return a pointer not the ownership, this means that returned pointer 
   *              can be used but not released, since it will be released from the collector.
   * 
   * @param name       program name used to store a specified instance in the collector.
   * @return Program*  a pointer to the program registered with \param name or nullptr if
   *                   does not exist. 
   */
  Program*        find( const std::string& name ) noexcept;
  /**
   * @brief Check if specified program name is already present in the collector.
   * 
   * @param name   program name
   * @return true  if exist
   * @return false if not exist
   */
  inline bool     contains( const std::string& name ) const noexcept
  { return m_mapPrograms.contains(name); }
  /**
   * @brief Create a new item with specified name and attributes list.
   *        Ownership of new item will be kept by ProgramsColletor. 
   *        In order to return ownership to calling application you must 
   *        use detach() method.
   * 
   * @param name       both "<name>.vs" and "<name>.fs" are supposed to exists 
   *                   in the get_shaders_path() or creation wil fail.
   * @param attributes 
   * @return Program*  pointer to the object instance or to the existing object instance
   *                   param @name was alredy registered in the collector. 
   * @return nullptr   in case of failure.                  
   */
  Program*        create( const std::string& name, const std::vector< std::pair<int,std::string> >& attributes ) noexcept;

  /**
   * @brief Store specified program inside the collector, but only if does not exist already a 
   *        program with the same name.
   * 
   * @param name        unique identifier for the program 
   * @param pProgram    pointer to a Program. If the method return true ownership of this pointer will be
   *                    taken from the collector.
   * @return true       in case a program with \param name is not registered already.
   * @return false      in case a program with \param name is already in the container.
   */
  bool            attach( const std::string& name, Program* pProgram ) noexcept;
  /**
   * @brief Detach a program from the collector.
   *        This means that collector return ownership to the caller.
   * 
   * @param name        program name
   * @return Program*   pointer to specified program if found inside the collector.
   *                    If specified program name does not exist return value will be nullptr.
   */
  Program*        detach( const std::string& name ) noexcept;
  
protected:
  /***/
  void_t on_initialize() noexcept
  { m_sShadersPath = DEFAULT_SHADER_PATH; }
  /***/
  void_t on_finalize() noexcept
  {}

private:
  typedef std::unordered_map<std::string, std::unique_ptr<Program>>  map_programs_t;
  
private:
  std::string      m_sShadersPath;
  map_programs_t   m_mapPrograms;
  
};

}

#endif // URE_PROGRAMS_COLLECTOR_H
