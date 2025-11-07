
FetchContent_Declare(
  libwebsockets
  GIT_REPOSITORY https://github.com/warmcat/libwebsockets.git
  GIT_TAG        main #v4.4.1
  GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(libwebsockets)

set(LWS_STATIC_PIC                    ON)
set(LWS_WITHOUT_TESTAPPS              ON)
set(LWS_WITHOUT_TEST_CLIENT           ON)
set(LWS_WITHOUT_TEST_PING             ON)
set(LWS_WITHOUT_TEST_SERVER           ON)
set(LWS_WITHOUT_TEST_SERVER_EXTPOLL   ON)
set_property(TARGET ${LWS_LIBRARIES} PROPERTY POSITION_INDEPENDENT_CODE ON)

include_directories( ${libwebsockets_BINARY_DIR}/include )
link_directories   ( ${libwebsockets_BINARY_DIR}/lib     )

