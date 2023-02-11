cmake_minimum_required (VERSION 3.14)
project(platform)

option (BUILD_SHARED_LIBS OFF)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (WIN32)
  # Non unicode windows api is broken...
  add_compile_definitions(UNICODE)
endif()

# platform agnostic
set(
    SOURCES 

)

# platform specific
if (WIN32)
  set(
    PLATFORM_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Window/Win32/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Strings/Win32/Strings.cpp
  )
endif()

add_library(${PROJECT_NAME} ${PLATFORM_SOURCES} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC pgm::core)

if(MSVC)
  set(WARN_LEVEL "-W4")
else()
  set(WARN_LEVEL "-Wall" "-Wextra" "-Wpedantic")
endif()
target_compile_options(${PROJECT_NAME} PRIVATE ${WARN_LEVEL})

add_library(pgm::${PROJECT_NAME} ALIAS ${PROJECT_NAME})