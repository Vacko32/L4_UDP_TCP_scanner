cmake_minimum_required(VERSION 3.16)
project(MyProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/contrib)
include(Catch)
get_target_property(catch2_includes Catch2 INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(Catch2 SYSTEM INTERFACE ${catch2_includes})

enable_testing()

# Add src directory
add_subdirectory(src)
# Add tests directory
add_subdirectory(tests)
