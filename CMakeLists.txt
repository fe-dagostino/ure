cmake_minimum_required(VERSION 3.22)

set(libname "ure")

# Set the project name and language
project( ${libname}
         VERSION 0.1.0
         DESCRIPTION "Underlying Rendering Engine"
         LANGUAGES CXX C
)

# Check if current project has a parent where to export common variables
get_directory_property(hasParent PARENT_DIRECTORY)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ure_config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/ure_config.h)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" ) 
set_property(CACHE CMAKE_BUILD_TYPE   PROPERTY STRINGS Release Debug)

set(URE_WINDOWS_MANAGER  "glfw"    CACHE STRING "Windows, Context and Event Manager" )
set_property(CACHE URE_WINDOWS_MANAGER    PROPERTY STRINGS glfw glut sdl)

set(URE_BACKEND_RENDER   "gles"    CACHE STRING "Rendering system" )
set_property(CACHE URE_BACKEND_RENDER     PROPERTY STRINGS gles opengl2 opengl3 vulkan wgpu)


if ( URE_WINDOWS_MANAGER STREQUAL "" )
  message(FATAL_ERROR "URE_WINDOWS_MANAGER must be set to a valid value")
endif()

#### URE_WINDOWS_MANAGER ####

# GLFW
if ( URE_WINDOWS_MANAGER STREQUAL "glfw" )
  add_definitions(
    -D_GLFW_ENABLED
  )  
  set(ENABLE_GLFW         ON)

  if(hasParent)
    set( PARENT_DEFINITIONS "${PARENT_DEFINITIONS} -D_GLFW_ENABLED" )
  endif()

  option( URE_USE_IMGUI "Enable/Disable using ImGui" ON)
  if ( URE_USE_IMGUI )
    add_definitions(
      -D_IMGUI_ENABLED
    ) 

    set(ENABLE_IMGUI        ON) 

    if(hasParent)
      set( PARENT_DEFINITIONS "${PARENT_DEFINITIONS} -D_IMGUI_ENABLED" )
    endif()
  endif()
endif()

#### URE_BACKEND_RENDER ####

# GLES
if ( URE_BACKEND_RENDER STREQUAL "gles" )
  add_definitions(
    -D_GLES_ENABLED
  )

  set(ENABLE_GLES         ON)
  add_definitions( -D_NV_CARD_ )

  if(hasParent)
    set( PARENT_DEFINITIONS "${PARENT_DEFINITIONS} -D_GLES_ENABLED" )
  endif()
endif()

# OPENGL2
if ( URE_BACKEND_RENDER STREQUAL "opengl2" )
  add_definitions(
    -D_OGL2_ENABLED
  )  
  set(ENABLE_OGL2         ON)

  if(hasParent)
    set( PARENT_DEFINITIONS "${PARENT_DEFINITIONS} -D_OGL2_ENABLED" )
  endif()  
endif()

# OPENGL3
if ( URE_BACKEND_RENDER STREQUAL "opengl3" )
  add_definitions(
    -D_OGL3_ENABLED
  )  
  set(ENABLE_OGL3         ON)

  if(hasParent)
    set( PARENT_DEFINITIONS "${PARENT_DEFINITIONS} -D_OGL3_ENABLED" )
  endif()  
endif()

# VULKAN
if ( URE_BACKEND_RENDER STREQUAL "vulkan" )
  add_definitions(
    -D_VULKAN_ENABLED
  )  
  set(ENABLE_VULKAN       ON)

  if(hasParent)
    set( PARENT_DEFINITIONS "${PARENT_DEFINITIONS} -D_VULKAN_ENABLED" )
  endif()  
endif()

####
set(URE_CXX_MIN_VERSION "12.1")

# Emscripten
if (CMAKE_CXX_COMPILER MATCHES "em\\+\\+$" )
  set(URE_CXX_MIN_VERSION "3.0")
  set(ENABLE_WASM            ON)
  add_definitions(
    -D_WASM_ENABLED
  )
  
  if(hasParent)
    set( ENABLE_WASM        "${ENABLE_WASM}" PARENT_SCOPE )
    set( PARENT_DEFINITIONS "${PARENT_DEFINITIONS} -D_WASM_ENABLED" )
  endif()  
endif()

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS URE_CXX_MIN_VERSION)
  message(FATAL_ERROR "Minimum requirement for CXX is version ${URE_CXX_MIN_VERSION}. Current version is ${CMAKE_CXX_COMPILER_VERSION}")
endif()

####
## OTHER OPTIONS
#### 
option(URE_USE_STB        "Using STB image loader"         ON)
option(URE_USE_FREEIMAGE  "Build FreeImage loader"        OFF)
option(URE_USE_DEVIL      "Build DevIL loader"            OFF)
option(URE_USE_WEBSOCKETS "Enable websockets support"     OFF)

option(URE_BUILD_EXAMPLES "Enable/Disable examples build"  ON)
option(URE_BUILD_TESTS    "Enable/Disable tests build"    OFF)

if (NOT ENABLE_WASM)
  find_package(Freetype      REQUIRED)
  message(STATUS "FreeType:"                                      )
  message(STATUS "    version: ${FREETYPE_VERSION_STRING}"        )
  message(STATUS "    include: ${FREETYPE_INCLUDE_DIRS}"          )
  message(STATUS "             ${FREETYPE_INCLUDE_DIR_ft2build}"  )
  message(STATUS "             ${FREETYPE_INCLUDE_DIR_freetype2}" )
  message(STATUS "    libs   : ${FREETYPE_LIBRARIES}"             )

  find_package(CURL          REQUIRED)
  message(STATUS "libcurl:"                                       )
  message(STATUS "    version: ${CURL_VERSION_STRING}"            )
  message(STATUS "    include: ${CURL_INCLUDE_DIRS} "             )
  message(STATUS "    libs   : ${CURL_LIBRARIES}"                 )
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
message(STATUS "${PROJECT_SOURCE_DIR}")

include(FetchContent)

include(libsigc++)
include(lock-free)
include(glm)
if (NOT ENABLE_WASM)
  if (URE_USE_WEBSOCKETS)
    include(libwebsocket)
  endif()
endif()
if (URE_USE_STB)
  include(stb)
endif()

if (ENABLE_GLFW)
  if (NOT ENABLE_WASM)
    include(glfw3)
  else()
    set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
  endif()

  if (ENABLE_IMGUI)
    include(imgui)
  endif()
endif()


if (NOT ENABLE_WASM)
  include_directories( ${FREETYPE_INCLUDE_DIRS}                                 )
  include_directories( ${CURL_INCLUDE_DIRS}                                     )
endif()

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include                        )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/glad/include         )

# uncomment to set a default CXX standard for the external tools like clang-tidy and cppcheck
# and the targets that do not specify a standard.
# If not set, the latest supported standard for your compiler is used
# You can later set fine-grained standards for each target using `target_compile_features`
# Note: linking together projects compiled with different C++ standards may work, but
# it is not recommended because of possible issues with ABI
set(CMAKE_CXX_STANDARD 20)

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS         ON)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_C_STANDARD_REQUIRED    ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  set(  LIB_VERSION_SO        -${CMAKE_PROJECT_VERSION}-debug          )
  set(  LIB_VERSION_A         -${CMAKE_PROJECT_VERSION}-static-debug   )
else()
  set(  LIB_VERSION_SO        -${CMAKE_PROJECT_VERSION}                )
  set(  LIB_VERSION_A         -${CMAKE_PROJECT_VERSION}-static         )
endif()

set(  LIB_BINARY_DIR 
      ${CMAKE_CURRENT_BINARY_DIR}
   )

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  add_definitions(
                  -ggdb3
                  -D_DEBUG
  )

else()
  add_compile_options( -O3 )
endif()

if(MSVC)
  # warning level 4 and all warnings as errors
  add_compile_options(-D_CRT_SECURE_NO_WARNINGS /W4 /WX)
else()
  # lots of warning
  add_compile_options(-Wall -Wextra -pedantic)
  
  set( EXT_LIBRARIES
      m
      pthread
     )
endif()

if (ENABLE_WASM)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_FREETYPE=1 " )
  message("libure: CMAKE_CXX_FLAGS: [${CMAKE_CXX_FLAGS}]")

  set( URE_LINK_FLAGS "${URE_LINK_FLAGS} -s USE_FREETYPE=1"    )
  set( URE_LINK_FLAGS "${URE_LINK_FLAGS} -s FETCH=1"           )
  if ( ENABLE_GLFW )
    set( URE_LINK_FLAGS "${URE_LINK_FLAGS} -s USE_GLFW=3 -s GL_ENABLE_GET_PROC_ADDRESS" )
  endif()
  
  if (URE_USE_WEBSOCKETS)
    set( URE_LINK_FLAGS "${URE_LINK_FLAGS} -s WEBSOCKET_SUBPROTOCOL='text,binary'")
    set( URE_LINK_FLAGS "${URE_LINK_FLAGS} -s ASYNCIFY"       )
    set( URE_LINK_FLAGS "${URE_LINK_FLAGS} -lwebsocket.js"       )
  endif()

  if(hasParent)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" PARENT_SCOPE )
    set( URE_LINK_FLAGS  "${URE_LINK_FLAGS}"  PARENT_SCOPE )
  endif()

  message("libure: URE_LINK_FLAGS : [${URE_LINK_FLAGS}]")
endif()

if (URE_USE_STB)
  add_compile_definitions( _USE_STB )
endif (URE_USE_STB)

# Common libraries
list(APPEND EXT_LIBRARIES sigc-3.0                )
list(APPEND EXT_LIBRARIES lock-free::lock-free    )
list(APPEND EXT_LIBRARIES glm::glm                )

if (URE_USE_FREEIMAGE)
  add_compile_definitions( _USE_FREEIMAGE )
  list(APPEND EXT_LIBRARIES freeimage )
endif (URE_USE_FREEIMAGE)

if (URE_USE_DEVIL)
  add_compile_definitions( _USE_DEVIL )
  list(APPEND EXT_LIBRARIES IL )
endif (URE_USE_DEVIL)

if (URE_USE_WEBSOCKETS)
  add_compile_definitions( _USE_WEBSOCKETS )
  if(hasParent)
    set( PARENT_DEFINITIONS "${PARENT_DEFINITIONS} -D_USE_WEBSOCKETS" )
  endif()

  if (NOT ENABLE_WASM)
    list(APPEND EXT_LIBRARIES websockets )
  endif()
endif (URE_USE_WEBSOCKETS)

if (ENABLE_GLFW)
  list(APPEND EXT_LIBRARIES glfw )
endif()

if (ENABLE_IMGUI)
  list(APPEND EXT_LIBRARIES ImGui )
endif()

if (NOT ENABLE_WASM)
  list(APPEND EXT_LIBRARIES curl                  )
  list(APPEND EXT_LIBRARIES ${FREETYPE_LIBRARIES} )
endif()

if ( ENABLE_GLES )
  list(APPEND EXT_LIBRARIES GLESv2                )
endif()

if ( ENABLE_OGL2 OR ENABLE_OGL3 ) 
  list(APPEND EXT_LIBRARIES GL                    )
endif()

if ( ENABLE_VULKAN ) 
  
endif()


if (ENABLE_WASM)
  set( TARGET_IMP "emscripten" )
else()
  set( TARGET_IMP "generic"    )
endif()

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )

else()

endif()


file( GLOB 
      LIB_INCLUDE
      ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    )
file( GLOB 
      LIB_FONT_INCLUDE
      ${CMAKE_CURRENT_SOURCE_DIR}/include/font/*.h
    )    
file( GLOB 
      LIB_WIDGETS_INCLUDE
      ${CMAKE_CURRENT_SOURCE_DIR}/include/widgets/*.h
    )

file( GLOB 
      LIB_SRC
      ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    )
file( GLOB 
      LIB_FONT_SRC
      ${CMAKE_CURRENT_SOURCE_DIR}/src/font/*.cpp
    )
file( GLOB 
      LIB_WIDGETS_SRC
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/*.cpp
    )

if (URE_USE_STB)
  set( LIB_SRC ${LIB_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/src/images/stb.c )
endif (URE_USE_STB)

if (URE_USE_FREEIMAGE)
  set( LIB_SRC ${LIB_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/src/images/freeimage.c )
endif (URE_USE_FREEIMAGE)

if (URE_USE_DEVIL)
  set( LIB_SRC ${LIB_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/src/images/devil.c )
endif (URE_USE_DEVIL)

file( GLOB
      LIB_TARGET_IMP_SRC
      ./src/imp/*_${TARGET_IMP}.cpp
    )

file( GLOB
      LIB_WM_SRC
      ./src/wm/*_${URE_WINDOWS_MANAGER}.cpp
    )

set(  LIB_BE_SRC
      ./src/backend/ure_canvas_ogl.cpp
      ./src/backend/ure_program_ogl.cpp
      ./src/backend/ure_renderer_ogl.cpp
      ./src/backend/ure_scene_graph_ogl.cpp
      ./src/backend/ure_shader_object_ogl.cpp
      ./src/backend/ure_texture_ogl.cpp
      ./src/backend/ure_view_port_ogl.cpp
   )

if ( ENABLE_GLES )
set(  
      LIB_GLAD_SRC
      ./3rd-party/glad/src/gles2.c
   )
endif()

if ( ENABLE_OGL2 OR ENABLE_OGL3 ) 
set(  
      LIB_GLAD_SRC
      ./3rd-party/glad/src/gl.c
   )
endif()

if ( ENABLE_VULKAN ) 
set(  
      LIB_GLAD_SRC
      ./3rd-party/glad/src/vulkan.c
   )
endif()

#set(  PRG_DATE_SRC
#      ./submodules/date/src/tz.cpp
#   )

if(hasParent)
  set( PARENT_DEFINITIONS "${PARENT_DEFINITIONS}"        PARENT_SCOPE )
  set( PARENT_LIBS        "${libname}${LIB_VERSION_SO}"  PARENT_SCOPE )
endif()  

add_library   ( ${libname}${LIB_VERSION_SO}   SHARED ${LIB_SRC} ${LIB_TARGET_IMP_SRC} ${LIB_FONT_SRC} ${LIB_WIDGETS_SRC} ${LIB_WM_SRC} ${LIB_BE_SRC} ${LIB_GLAD_SRC} )
add_library   ( ure::ure        ALIAS ${libname}${LIB_VERSION_SO} )
add_library   ( ${libname}${LIB_VERSION_A}    STATIC ${LIB_SRC} ${LIB_TARGET_IMP_SRC} ${LIB_FONT_SRC} ${LIB_WIDGETS_SRC} ${LIB_WM_SRC} ${LIB_BE_SRC} ${LIB_GLAD_SRC} )
add_library   ( ure::ure_static ALIAS ${libname}${LIB_VERSION_A} )

target_link_libraries( ${libname}${LIB_VERSION_SO}  PUBLIC ${EXT_LIBRARIES} )
target_link_libraries( ${libname}${LIB_VERSION_A}   PUBLIC ${EXT_LIBRARIES} )
target_link_libraries( ${libname}${LIB_VERSION_SO}  PUBLIC ${CMAKE_DL_LIBS} )
target_link_libraries( ${libname}${LIB_VERSION_A}   PUBLIC ${CMAKE_DL_LIBS} )

target_include_directories( ${libname}${LIB_VERSION_SO} PUBLIC 
                            "${CMAKE_CURRENT_SOURCE_DIR}/include" 
                          )
target_include_directories( ${libname}${LIB_VERSION_A}  PUBLIC 
                            "${CMAKE_CURRENT_SOURCE_DIR}/include" 
                          )
include(GNUInstallDirs)
install( TARGETS ${libname}${LIB_VERSION_SO} ${libname}${LIB_VERSION_A} ARCHIVE DESTINATION lib )
install( FILES   ${LIB_INCLUDE} ${LIB_FONT_INCLUDE} ${LIB_WIDGETS_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} ) 

if ( URE_BUILD_EXAMPLES )
  add_subdirectory(examples)
endif()
