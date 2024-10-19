
FetchContent_Declare(
  lock-free
  GIT_REPOSITORY https://github.com/fe-dagostino/lock-free.git
  GIT_TAG        master # master
  GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(lock-free)
