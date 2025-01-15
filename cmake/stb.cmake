
FetchContent_Declare(
  stb
  GIT_REPOSITORY    https://github.com/nothings/stb.git
  GIT_TAG           master # master
  GIT_SHALLOW       TRUE
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
)

FetchContent_MakeAvailable(stb)

add_library( stb INTERFACE ${stb_SOURCE_DIR} )
include_directories( ${stb_SOURCE_DIR} )
