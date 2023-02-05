cmake_minimum_required(VERSION 3.9)

set(libname "ure")

# Set the project name and language
project( ${libname}
         VERSION 0.1.0
         DESCRIPTION "Underlying Rendering Engine"
         LANGUAGES CXX C
)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h)

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
endif()

#### URE_BACKEND_RENDER ####

# GLES
if ( URE_BACKEND_RENDER STREQUAL "gles" )
  add_definitions(
    -D_GLES_ENABLED
  )

#  if ( ENABLE_GLFW )
#    add_definitions(
#      -DGLFW_INCLUDE_ES2  
#    )
#  endif()

  set(ENABLE_GLES         ON)
  add_definitions( -D_NV_CARD_ )
endif()

# OPENGL2
if ( URE_BACKEND_RENDER STREQUAL "opengl2" )
  add_definitions(
    -D_OGL2_ENABLED
  )  
  set(ENABLE_OGL2         ON)
endif()

# OPENGL3
if ( URE_BACKEND_RENDER STREQUAL "opengl3" )
  add_definitions(
    -D_OGL3_ENABLED
  )  
  set(ENABLE_OGL3         ON)
endif()

# VULKAN
if ( URE_BACKEND_RENDER STREQUAL "vulkan" )
  add_definitions(
    -D_VULKAN_ENABLED
  )  
  set(ENABLE_VULKAN       ON)
endif()

####

# Emscripten
if (CMAKE_CXX_COMPILER MATCHES "em\\+\\+$" )
  set(ENABLE_WASM         ON)
  add_definitions(
    -D_WASM_ENABLED
  )  
endif()

if ( ENABLE_GLFW )
  find_package(glfw3 3.3 REQUIRED)
endif()

####
## OTHER OPTIONS
#### 
option(URE_USE_STB        "Using STB image loader"         ON)
option(URE_USE_FREEIMAGE  "Build FreeImage loader"        OFF)
option(URE_USE_DEVIL      "Build DevIL loader"            OFF)

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

find_package(Git)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Update submodules with remote" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --remote -f
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive --remote -f : failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

#add_subdirectory(submodules/tclap)
add_subdirectory(submodules/libsigc++)
add_subdirectory(submodules/lock-free)
add_subdirectory(submodules/glm      )
#add_subdirectory(submodules/stb      ) 
#add_subdirectory(submodules/libcsv)
#add_subdirectory(submodules/libconfig)
#add_subdirectory(submodules/)

if (NOT ENABLE_WASM)
  include_directories( ${FREETYPE_INCLUDE_DIRS}                                 )
  include_directories( ${CURL_INCLUDE_DIRS}                                     )
endif()

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include                        )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/glad/include         )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/submodules/lock-free/include   )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/submodules/glm                 )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/submodules/stb                 )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/submodules/libsigc++           )
include_directories( ${CMAKE_CURRENT_BINARY_DIR}/submodules/libsigc++           ) # necessary for sigc++config.h
#include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/           )
#include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/backends/  )
#message (FATAL_ERROR "${CMAKE_CURRENT_BINARY_DIR}" )
link_directories( ${CMAKE_CURRENT_BINARY_DIR}/submodules/libsigc++           )

# uncomment to set a default CXX standard for the external tools like clang-tidy and cppcheck
# and the targets that do not specify a standard.
# If not set, the latest supported standard for your compiler is used
# You can later set fine-grained standards for each target using `target_compile_features`
# Note: linking together projects compiled with different C++ standards may work, but
# it is not recommended because of possible issues with ABI
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD   17)

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS         ON)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_C_STANDARD_REQUIRED    ON)


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
  message("libure: CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}") 

  set( URE_LINK_FLAGS "${URE_LINK_FLAGS} -s USE_FREETYPE=1"    )
  set( URE_LINK_FLAGS "${URE_LINK_FLAGS} -s FETCH=1"           )
  if ( ENABLE_GLFW )
    set( URE_LINK_FLAGS "${URE_LINK_FLAGS} -s USE_GLFW=3"      )
  endif()  
  
  get_directory_property(hasParent PARENT_DIRECTORY)
  if(hasParent)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" PARENT_SCOPE )
    set( URE_LINK_FLAGS  "${URE_LINK_FLAGS}"  PARENT_SCOPE )
  endif()

  message("libure: URE_LINK_FLAGS : ${URE_LINK_FLAGS}")
endif()

if (URE_USE_STB)
  add_definitions( -D_USE_STB )
endif (URE_USE_STB)

# Common libraries
set( EXT_LIBRARIES ${EXT_LIBRARIES} sigc-3.0 )

if (URE_USE_FREEIMAGE)
  add_definitions( -D_USE_FREEIMAGE )
  set( EXT_LIBRARIES ${EXT_LIBRARIES} freeimage )
endif (URE_USE_FREEIMAGE)

if (URE_USE_DEVIL)
  add_definitions( -D_USE_DEVIL )
  set( EXT_LIBRARIES ${EXT_LIBRARIES} IL )
endif (URE_USE_DEVIL)

if (ENABLE_GLFW)
  set( EXT_LIBRARIES ${EXT_LIBRARIES} glfw3 )
endif()

if (NOT ENABLE_WASM)
  set( EXT_LIBRARIES ${EXT_LIBRARIES} curl                  )
  set( EXT_LIBRARIES ${EXT_LIBRARIES} ${FREETYPE_LIBRARIES} )
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

#set(  PRG_GUI_SRC
#      ./submodules/imgui/imgui.cpp
#      ./submodules/imgui/imgui_demo.cpp
#      ./submodules/imgui/imgui_draw.cpp
#      ./submodules/imgui/imgui_tables.cpp
#      ./submodules/imgui/imgui_widgets.cpp
#  )

#set(  PRG_BACKEND_SRC
#    ./submodules/imgui/backends/imgui_impl_glfw.cpp
#    ./submodules/imgui/backends/imgui_impl_opengl3.cpp
#)


#set(  PRG_DATE_SRC
#      ./submodules/date/src/tz.cpp
#   )

#add_library( ${CMAKE_PROJECT_NAME}                   ${LIB_SRC} ${PRG_DATE_SRC} ${PRG_GUI_SRC} ${PRG_BACKEND_SRC} )
add_library   ( ${libname}${LIB_VERSION_SO}   SHARED ${LIB_SRC} ${LIB_TARGET_IMP_SRC} ${LIB_FONT_SRC} ${LIB_WIDGETS_SRC} ${LIB_WM_SRC} ${LIB_BE_SRC} ${LIB_GLAD_SRC} )
add_library   ( ${libname}${LIB_VERSION_A}    STATIC ${LIB_SRC} ${LIB_TARGET_IMP_SRC} ${LIB_FONT_SRC} ${LIB_WIDGETS_SRC} ${LIB_WM_SRC} ${LIB_BE_SRC} ${LIB_GLAD_SRC} )

target_link_libraries( ${libname}${LIB_VERSION_SO}  ${EXT_LIBRARIES} )
target_link_libraries( ${libname}${LIB_VERSION_A}   ${EXT_LIBRARIES} )
target_link_libraries( ${libname}${LIB_VERSION_SO}  ${CMAKE_DL_LIBS} )
target_link_libraries( ${libname}${LIB_VERSION_A}   ${CMAKE_DL_LIBS} )

target_include_directories( ${libname}${LIB_VERSION_SO} INTERFACE 
                            "${CMAKE_CURRENT_SOURCE_DIR}/include" 
                          )
target_include_directories( ${libname}${LIB_VERSION_A}  INTERFACE 
                            "${CMAKE_CURRENT_SOURCE_DIR}/include" 
                          )

if ( URE_BUILD_EXAMPLES )
  add_subdirectory(examples)
endif()
