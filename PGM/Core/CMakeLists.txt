cmake_minimum_required (VERSION 3.14)
project(core)

option (BUILD_SHARED_LIBS OFF)

set (PGM_LOGGING_LEVEL 4)
set (PGM_ASSERTS_ENABLED 
    $<$<CONFIG:DEBUG>: "PGM_ASSERTS_ENABLED" >)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (WIN32)
  # Non unicode windows api is broken...
  add_compile_definitions(UNICODE)
endif()

# platform agnostic
set(
  SOURCES 
  Events/EventDispatcher.cpp
)

# platform specific
if (WIN32)
  set(
    PLATFORM_SOURCES
  )
endif()

add_library(${PROJECT_NAME} ${PLATFORM_SOURCES} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt glm::glm)
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PGM_ASSERTS_ENABLED} PGM_LOGGING_LEVEL=${PGM_LOGGING_LEVEL})

if(MSVC)
  set(WARN_LEVEL "-W4")
else()
  set(WARN_LEVEL "-Wall" "-Wextra" "-Wpedantic")
endif()
target_compile_options(${PROJECT_NAME} PRIVATE ${WARN_LEVEL})

add_library(pgm::${PROJECT_NAME} ALIAS ${PROJECT_NAME})