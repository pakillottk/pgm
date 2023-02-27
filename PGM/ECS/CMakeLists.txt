cmake_minimum_required (VERSION 3.14)
project(ecs)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene/Scene.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/ecs.cpp
)

# platform specific
if (WIN32)
  set(
    PLATFORM_SOURCES
  )
endif()

add_library(${PROJECT_NAME} ${PLATFORM_SOURCES} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC pgm::core EnTT::EnTT)

if(MSVC)
  set(WARN_LEVEL "-W4")
else()
  set(WARN_LEVEL "-Wall" "-Wextra" "-Wpedantic")
endif()
target_compile_options(${PROJECT_NAME} PRIVATE ${WARN_LEVEL})

add_library(pgm::${PROJECT_NAME} ALIAS ${PROJECT_NAME})