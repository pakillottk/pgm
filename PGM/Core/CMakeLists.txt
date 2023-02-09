cmake_minimum_required (VERSION 3.14)
project(core)

set (PGM_LOGGING_LEVEL 3)
set (PGM_ASSERTS_ENABLED 
    $<$<CONFIG:DEBUG>: "PGM_ASSERTS_ENABLED" > 
    $<$<CONFIG:RELEASE>: "" >)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (WIN32)
  # Non unicode windows api is broken...
  add_compile_definitions(UNICODE)
endif()

# platform agnostic
set(
  SOURCES 
  Core/Core.cpp
)

# platform specific
if (WIN32)
  set(
    PLATFORM_SOURCES
  )
endif()

add_library(${PROJECT_NAME} ${PLATFORM_SOURCES} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PGM_ASSERTS_ENABLED} PGM_LOGGING_LEVEL=${PGM_LOGGING_LEVEL})

if(MSVC)
  set(WARN_LEVEL "-W4")
else()
  set(WARN_LEVEL "-Wall -Wextra -Wpedantic")
endif()
target_compile_options(${PROJECT_NAME} PRIVATE ${WARN_LEVEL})

add_library(pgm::${PROJECT_NAME} ALIAS ${PROJECT_NAME})