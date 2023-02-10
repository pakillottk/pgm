cmake_minimum_required (VERSION 3.14)
project(pgm_sandbox)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

if (WIN32)
  # Non unicode windows api is broken...
  add_compile_definitions(UNICODE)
endif()

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
target_link_libraries(pgm_sandbox PRIVATE pgm::core pgm::platform pgm::renderer)

if(MSVC)
  set(WARN_LEVEL "-W4")
else()
  set(WARN_LEVEL "-Wall -Wextra -Wpedantic")
endif()
target_compile_options(${PROJECT_NAME} PRIVATE ${WARN_LEVEL})