FetchContent_Declare(
  embc  GIT_REPOSITORY    https://github.com/fe-dagostino/emscripten-browser-clipboard.git
  GIT_TAG           main
  GIT_SHALLOW       TRUE
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
)

FetchContent_MakeAvailable(embc)

add_library( embc INTERFACE ${embc_SOURCE_DIR} )
include_directories( ${embc_SOURCE_DIR} )
