
FetchContent_Declare(
  ImGui
  GIT_REPOSITORY    https://github.com/ocornut/imgui.git
  GIT_TAG           master # master
  GIT_SHALLOW       TRUE
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
)

FetchContent_MakeAvailable(ImGui)

if ( ENABLE_GLFW )
  message( INFO " EXPORT ImGui SRC DIR: [${imgui_SOURCE_DIR}]" )
  set ( IMGUI_SRC   ${imgui_SOURCE_DIR}/imgui.cpp 
                    ${imgui_SOURCE_DIR}/imgui_demo.cpp 
                    ${imgui_SOURCE_DIR}/imgui_draw.cpp
                    ${imgui_SOURCE_DIR}/imgui_tables.cpp
                    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  )
 
endif()

if ( ENABLE_GLES OR ENABLE_OGL3 )
  set ( IMGUI_BACKEND_SRC   ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp 
                            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
  )
endif()

add_library( ImGui   ${IMGUI_SRC} ${IMGUI_BACKEND_SRC} )
include_directories( ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends )

if ( ENABLE_GLES OR ENABLE_OGL3 )
  target_compile_definitions( ImGui PUBLIC IMGUI_IMPL_OPENGL_ES2 )
endif()

set_property(TARGET ImGui PROPERTY POSITION_INDEPENDENT_CODE ON)

if(hasParent)
  message( INFO " EXPORT ImGui SRC DIR: [${imgui_SOURCE_DIR}]" )
  message( INFO " EXPORT ImGui BIN DIR: [${imgui_BINARY_DIR}]" )
  # make both ${imgui_SOURCE_DIR} and ${imgui_BINARY_DIR} visible at parent scope.
  set( imgui_SOURCE_DIR ${imgui_SOURCE_DIR} PARENT_SCOPE )
  set( imgui_BINARY_DIR ${imgui_BINARY_DIR} PARENT_SCOPE )
endif(hasParent)