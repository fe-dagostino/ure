
set(GLFW_VERSION 3.4)

FetchContent_Declare(
                      glfw
                      GIT_REPOSITORY https://github.com/glfw/glfw.git
                      GIT_TAG master
                    )

set(GLFW_BUILD_DOCS        OFF CACHE INTERNAL "" )
set(GLFW_BUILD_TESTS       OFF CACHE INTERNAL "" )
set(GLFW_BUILD_EXAMPLES    OFF CACHE INTERNAL "" )
set(GLFW_BUILD_WAYLAND      ON CACHE INTERNAL "" )
set(GLFW_BUILD_X11          ON CACHE INTERNAL "" )
set(GLFW_LIBRARY_TYPE   STATIC CACHE INTERNAL "" )
set(GLFW_INSTALL           OFF CACHE INTERNAL "" )
                    
FetchContent_MakeAvailable(glfw)
