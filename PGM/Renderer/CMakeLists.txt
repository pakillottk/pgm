cmake_minimum_required (VERSION 3.14)
project(renderer)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

option (BUILD_SHARED_LIBS OFF)

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

if (WIN32)
  # Non unicode windows api is broken...
  add_compile_definitions(UNICODE)
endif()

find_package(OpenGL REQUIRED)
# find_package(GLUT REQUIRED)
# include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

# find_package(GLEW REQUIRED)
# include_directories(${GLEW_INCLUDE_DIRS})

# platform agnostic
set(
    SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/API/Buffers/VertexArray.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/API/Backend/OpenGL/OpenGlCommands.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/API/Backend/OpenGL/Buffers/VertexArrayImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/API/Backend/OpenGL/Shaders/OpenGlShader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/API/Backend/OpenGL/Textures/OpenGlTexture2d.cpp
)

# platform specific
if (WIN32)
  set(
    PLATFORM_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/API/Backend/OpenGL/Win32/OpenGlRenderContext.cpp
  )
endif()

add_library(${PROJECT_NAME} ${PLATFORM_SOURCES} ${SOURCES})
target_link_libraries(
  ${PROJECT_NAME} 
  
  PUBLIC 
    pgm::core 
    pgm::platform

  PRIVATE
    ${OPENGL_LIBRARIES}
    glew_s
)

if(MSVC)
  set(WARN_LEVEL "-W4")
else()
  set(WARN_LEVEL "-Wall" "-Wextra" "-Wpedantic")
endif()
target_compile_options(${PROJECT_NAME} PRIVATE ${WARN_LEVEL})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIRS})

add_library(pgm::${PROJECT_NAME} ALIAS ${PROJECT_NAME})