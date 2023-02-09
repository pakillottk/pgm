cmake_minimum_required (VERSION 3.14)
project(pgm_sandbox)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

if (WIN32)
  # Non unicode windows api is broken...
  add_compile_definitions(UNICODE)
endif()

include_directories(${PGM_RENDERER_INCLUDE_DIRS})


if (WIN32)
    set(SOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/win32_main.cpp
    )
    add_executable(pgm_sandbox WIN32 ${SOURCES})
else()
    set(SOURCES 
        
    )
    add_executable(pgm_sandbox ${SOURCES})
endif()
target_link_libraries(pgm_sandbox PRIVATE pgm_renderer)
target_compile_options(pgm_sandbox PRIVATE "-Wall")