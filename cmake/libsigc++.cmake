
FetchContent_Declare(
  sigc
  GIT_REPOSITORY https://github.com/libsigcplusplus/libsigcplusplus.git
  GIT_TAG        master # master
  GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(sigc)

include_directories( ${sigc_SOURCE_DIR} ${sigc_BINARY_DIR} )

if(hasParent)
  message( INFO " EXPORT SIGC++ SRC DIR: [${sigc_SOURCE_DIR}]" )
  message( INFO " EXPORT SIGC++ BIN DIR: [${sigc_BINARY_DIR}]" )
  # make both ${sigc_SOURCE_DIR} and ${sigc_BINARY_DIR} visible at parent scope.
  set( sigc_SOURCE_DIR ${sigc_SOURCE_DIR} PARENT_SCOPE )
  set( sigc_BINARY_DIR ${sigc_BINARY_DIR} PARENT_SCOPE )
endif(hasParent)