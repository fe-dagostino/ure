
# Emscripten Browser Clipboard Library

FetchContent_Declare(
  emsbc
  GIT_REPOSITORY    https://github.com/Armchair-Software/emscripten-browser-clipboard.git
  GIT_TAG           main # master
  GIT_SHALLOW       TRUE
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
)

FetchContent_MakeAvailable(emsbc)

add_library( emsbc INTERFACE ${emsbc_SOURCE_DIR} )
include_directories( ${emsbc_SOURCE_DIR} )
