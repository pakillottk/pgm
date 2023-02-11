cmake_minimum_required (VERSION 3.14)
project(platform)

option (BUILD_SHARED_LIBS OFF)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

if (WIN32)
  # Non unicode windows api is broken...
  add_compile_definitions(UNICODE)
endif()

# find_package(OpenGL REQUIRED)
# find_package(GLUT REQUIRED)
# include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

# find_package(GLEW REQUIRED)
# include_directories(${GLEW_INCLUDE_DIRS})

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