
FetchContent_Declare(
  sigc
  GIT_REPOSITORY https://github.com/libsigcplusplus/libsigcplusplus.git
  GIT_TAG        master # master
)

FetchContent_MakeAvailable(sigc)

include_directories( ${sigc_SOURCE_DIR} ${sigc_BINARY_DIR} )
