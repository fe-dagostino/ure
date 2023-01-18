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

#include "ure_programs_collector.h"
#include "ure_vertex_shader.h"
#include "ure_fragment_shader.h"
#include "ure_utils.h"

#include <core/utils.h>

namespace ure {

Program*   ProgramsCollector::find( const std::string& name ) noexcept
{
  map_programs_t::iterator  iter = m_mapPrograms.find( name );
  if ( iter == m_mapPrograms.end() )
    return nullptr;

  return iter->second.get();
}
    
Program*   ProgramsCollector::create( const std::string& name, const std::vector< std::pair<int,std::string> >& attributes ) noexcept
{
  Program* pProgram = find( name );
  if ( pProgram != nullptr )
    return pProgram;

  std::string                      _sVS = core::utils::format( "%s/%s.vs", get_shaders_path().c_str(), name.c_str() );
  std::string                      _sFS = core::utils::format( "%s/%s.fs", get_shaders_path().c_str(), name.c_str() );
  std::shared_ptr<VertexShader>    _vertexShader   = std::make_shared<VertexShader>();
  std::shared_ptr<FragmentShader>  _fragmentShader = std::make_shared<FragmentShader>();
  bool            _bVertexShader   = false;
  bool            _bfragmentShader = false;
    
  _bVertexShader   = _vertexShader->load_from_file( _sVS );
  if ( _bVertexShader == false )
  {
    std::string sLog;
    _vertexShader->get_logs(sLog);
    ure::utils::log( core::utils::format( "ERROR: Vertex Shader [%s]\n", sLog.c_str()) );
  }
    
  _bfragmentShader = _fragmentShader->load_from_file( _sFS );
  if ( _bVertexShader == false )
  {
    std::string sLog;
    _fragmentShader->get_logs(sLog);
    ure::utils::log( core::utils::format( "ERROR: Fragment Shader [%s]\n", sLog.c_str()) );
  }
    
  if ( ( _bVertexShader == true ) && ( _bfragmentShader == true ) )
  {
    pProgram = new(std::nothrow) Program();
  
    pProgram->attach_shaders( _vertexShader, _fragmentShader );

    for ( auto pair : attributes )
    {
      pProgram->bindAttribLocation( pair.first, pair.second.data() );
    }
    
    pProgram->link();

    //
    attach( name, pProgram );

    return pProgram;
  }

  return nullptr;
}

bool  ProgramsCollector::attach( const std::string& name, Program* pProgram ) noexcept
{
  map_programs_t::iterator  iter = m_mapPrograms.find( name );
  if ( iter != m_mapPrograms.end() )
    return false;
  
  m_mapPrograms[name] = std::unique_ptr<Program>(pProgram); 
  
  return true;
}

Program*  ProgramsCollector::detach( const std::string& name ) noexcept
{
  map_programs_t::iterator  iter = m_mapPrograms.find( name );
  if ( iter == m_mapPrograms.end() )
    return nullptr;
  
  Program* pProgram = iter->second.release();
  
  m_mapPrograms.erase(iter);
  
  return pProgram;
}

}
