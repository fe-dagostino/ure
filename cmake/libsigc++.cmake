
FetchContent_Declare(
  sigc
  GIT_REPOSITORY https://github.com/libsigcplusplus/libsigcplusplus.git
  GIT_TAG        master # master
)

FetchContent_MakeAvailable(sigc)

message( INFO " SIGC++ SRC DIR: [${sigc_SOURCE_DIR}]" )
message( INFO " SIGC++ BIN DIR: [${sigc_BINARY_DIR}]" )

include_directories( ${sigc_SOURCE_DIR} ${sigc_BINARY_DIR} )

if(hasParent)
  # make both ${sigc_SOURCE_DIR} and ${sigc_BINARY_DIR} visible at parent scope.
  set( sigc_SOURCE_DIR ${sigc_SOURCE_DIR} PARENT_SCOPE )
  set( sigc_BINARY_DIR ${sigc_BINARY_DIR} PARENT_SCOPE )
endif(hasParent)