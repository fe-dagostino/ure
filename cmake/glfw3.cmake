
set(GLFW_VERSION 3.4)

FetchContent_Declare(
                      glfw3
                      GIT_REPOSITORY https://github.com/glfw/glfw.git
                      GIT_TAG master
                    )

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw3)

include_directories( ${glfw_SOURCE_DIR} )
