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
target_link_libraries(pgm_sandbox pgm_renderer)
target_compile_options(pgm_sandbox PRIVATE "-Wall")