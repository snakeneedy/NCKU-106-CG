cmake_minimum_required(VERSION 3.4)
project(cghw2)

find_package(PkgConfig REQUIRED)
find_package(GLEW REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
pkg_search_module(GLFW REQUIRED glfw3)

# ext object loader
add_subdirectory(ext/tiny_obj_loader)
include_directories(ext/tiny_obj_loader)

# include gl directories
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp)
add_executable(cghw2 ${SOURCE_FILES})

target_link_libraries(cghw2 ${GLFW_LIBRARIES} ${GLFW_STATIC_LIBRARIES} ${GLEW_LIBRARIES})
target_link_libraries(cghw2 tiny_obj_loader_lib)
