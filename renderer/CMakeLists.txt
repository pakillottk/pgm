cmake_minimum_required (VERSION 3.14)
project(pgm_renderer)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/window/win32/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/strings/win32/Strings.cpp
  )
endif()

add_library(pgm_renderer ${PLATFORM_SOURCES} ${SOURCES})
target_link_libraries(pgm_renderer PUBLIC glm::glm EnTT::EnTT fmt::fmt)
target_compile_definitions(pgm_renderer PRIVATE PGM_LOGGING_LEVEL=3)
target_compile_options(pgm_renderer PRIVATE "-Wall")