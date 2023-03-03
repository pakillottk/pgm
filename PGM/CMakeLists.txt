cmake_minimum_required (VERSION 3.14)
project(pgm)

option (BUILD_SHARED_LIBS OFF)
option (WITH_OPENGL "Enable OpenGL" ON)

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

    ${CMAKE_CURRENT_SOURCE_DIR}/Application/Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Application/GUI/GUISystem.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Events/EventDispatcher.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/ECS/Scene/Scene.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS/ecs.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/GUI/GUI.cpp


    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/API/Buffers/VertexArray.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/API/Backend/OpenGL/OpenGlCommands.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/API/Backend/OpenGL/Buffers/VertexArrayImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/API/Backend/OpenGL/Shaders/OpenGlShader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/API/Backend/OpenGL/Textures/OpenGlTexture2d.cpp
)

# platform specific
if (WIN32)
  set(
    PLATFORM_SOURCES

    ${CMAKE_CURRENT_SOURCE_DIR}/Platform/Input/Win32/Keyboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Platform/Input/Win32/Mouse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Platform/Window/Win32/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Platform/Strings/Win32/Strings.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/API/Backend/OpenGL/Win32/OpenGlRenderContext.cpp
)
endif()

if(WITH_OPENGL)
    find_package(OpenGL REQUIRED)
    set(BACKEND_TARGETS ${BACKEND_TARGETS} ${OPENGL_LIBRARIES} glew_s)
    set(BACKEND_INCLUDES ${BACKEND_INCLUDES} ${GLEW_INCLUDE_DIRS})
endif()

add_library(${PROJECT_NAME} ${PLATFORM_SOURCES} ${SOURCES})
target_link_libraries(
    ${PROJECT_NAME} 
    
    PUBLIC 
        fmt::fmt 
        glm::glm 
        EnTT::EnTT

    PRIVATE
        ${BACKEND_TARGETS}
)
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PGM_ASSERTS_ENABLED} PGM_LOGGING_LEVEL=${PGM_LOGGING_LEVEL})
target_include_directories(${PROJECT_NAME} PRIVATE ${BACKEND_INCLUDES})
if(MSVC)
  set(WARN_LEVEL "-W4" "/permissive-")
else()
  set(WARN_LEVEL "-Wall" "-Wextra" "-Wpedantic")
endif()
target_compile_options(${PROJECT_NAME} PRIVATE ${WARN_LEVEL})

add_library(pgm::${PROJECT_NAME} ALIAS ${PROJECT_NAME})