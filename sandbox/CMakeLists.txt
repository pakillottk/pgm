cmake_minimum_required (VERSION 3.14)
project(pgm_sandbox)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

add_executable(pgm_sandbox ${SOURCES})

target_link_libraries(
  pgm_sandbox 

  PRIVATE 
    pgm::application
)

if(MSVC)
  set(WARN_LEVEL "-W4")
else()
  set(WARN_LEVEL "-Wall" "-Wextra" "-Wpedantic")
endif()
target_compile_options(${PROJECT_NAME} PRIVATE ${WARN_LEVEL})