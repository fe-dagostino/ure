cmake_minimum_required(VERSION 3.16)
# Set the project name and language
project( examples
         LANGUAGES CXX C
)

# Add the temporary output directories to the library path to make sure that
# libure can be found, even if it is not installed system-wide yet.
LINK_DIRECTORIES( ${LIB_BINARY_DIR} )

set(  DEFAULT_LIBRARIES
      ${libname}${LIB_VERSION_SO}
)

option(URE_ENABLE_CPPCHECK    "Enable/Disable cppcheck"    OFF)

if(ENABLE_WASM)
option(URE_JS_ONLY            "Build with WASM=0"          OFF)
endif()

if ( URE_ENABLE_CPPCHECK )
  set(CMAKE_CXX_CPPCHECK "cppcheck")
endif()

add_executable( ure_ogl                               ure_ogl.cpp           )

target_link_libraries( ure_ogl                        ${DEFAULT_LIBRARIES}  )

if(ENABLE_WASM)
if(URE_JS_ONLY)
	message(STATUS "Setting compilation target to native JavaScript")
	set(CMAKE_EXECUTABLE_SUFFIX ".js")
	set_target_properties( ure_ogl PROPERTIES LINK_FLAGS "-s WASM=0 -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3=1 --preload-file ../resources@resources 
                                                        -s EXPORTED_FUNCTIONS='[_main]'")
else(URE_JS_ONLY)
	message(STATUS "Setting compilation target to WASM")
  set(CMAKE_EXECUTABLE_SUFFIX ".js")
	set_target_properties( ure_ogl PROPERTIES LINK_FLAGS "${URE_LINK_FLAGS} -s WASM=1 -s USE_WEBGL2=1 -s FULL_ES3=1 
                                                        -s EXPORTED_RUNTIME_METHODS=['UTF8ToString'] 
                                                        --preload-file ../resources@resources -s EXPORTED_FUNCTIONS='[_main]'")
endif(URE_JS_ONLY)
else(ENABLE_WASM)
  target_link_libraries( ure_ogl            ${EXT_LIBRARIES} )
  target_link_libraries( ure_ogl            ${CMAKE_DL_LIBS} )
endif(ENABLE_WASM)