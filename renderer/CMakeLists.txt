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
target_link_libraries(pgm_renderer glm entt)
target_compile_definitions(pgm_renderer PRIVATE PGM_LOGGING_LEVEL=3)
target_compile_options(pgm_renderer PRIVATE "-Wall")