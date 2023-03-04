cmake_minimum_required (VERSION 3.14)
project(pgm)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

option (BUILD_SHARED_LIBS OFF)

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

if (WIN32)
  # Non unicode windows api is broken...
  add_compile_definitions(UNICODE)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/vendor ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui) 

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(GLEW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew/include)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/PGM)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sandbox)