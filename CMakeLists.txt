
cmake_minimum_required(VERSION 3.5)

project(ninutils VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_library(dolrel SHARED src/rel.cpp src/symbols.cpp src/dol.cpp src/extra_info.cpp)
target_include_directories(dolrel PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>  # <prefix>/include
)

add_executable(readrel src/readrel.cpp)
target_include_directories(readrel PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>  # <prefix>/include
)
target_link_libraries(readrel dolrel)

add_executable(readdol src/readdol.cpp)
target_include_directories(readdol PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>  # <prefix>/include
)
target_link_libraries(readdol dolrel)

install(TARGETS dolrel DESTINATION lib)
install(TARGETS readrel readdol DESTINATION bin)