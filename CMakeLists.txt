cmake_minimum_required (VERSION 3.14)
project(pgm)

include(GNUInstallDirs)

set (CMAKE_CXX_STANDARD 20)
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

if (WIN32)
  # Non unicode windows api is broken...
  add_compile_definitions(UNICODE)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/renderer)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sandbox)