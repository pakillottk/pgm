cmake_minimum_required (VERSION 3.14)
project(gui)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

option (BUILD_SHARED_LIBS OFF)

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

if (WIN32)
  # Non unicode windows api is broken...
  add_compile_definitions(UNICODE)
endif()

# platform agnostic
set(
    SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUI.cpp
)

# platform specific
if (WIN32)
  set(
    PLATFORM_SOURCES
  )
endif()

add_library(${PROJECT_NAME} ${PLATFORM_SOURCES} ${SOURCES})
target_link_libraries(
  ${PROJECT_NAME} 
  
  PUBLIC 
    pgm::core 
    pgm::platform
    pgm::renderer
)

if(MSVC)
  set(WARN_LEVEL "-W4")
else()
  set(WARN_LEVEL "-Wall" "-Wextra" "-Wpedantic")
endif()
target_compile_options(${PROJECT_NAME} PRIVATE ${WARN_LEVEL})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIRS})

add_library(pgm::${PROJECT_NAME} ALIAS ${PROJECT_NAME})