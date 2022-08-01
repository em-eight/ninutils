
cmake_minimum_required(VERSION 3.5)

project(ninutils VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_library(rel src/rel.cpp)
target_include_directories(rel PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>  # <prefix>/include
)


add_executable(readrel src/readrel.cpp)
target_include_directories(readrel PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>  # <prefix>/include
)
target_link_libraries(readrel rel)