# glm
set(GLM_LIB_NAME "glm")
set(GLM_INC_PATH ${GLM_LIB_NAME})

add_library(${GLM_LIB_NAME} INTERFACE)
target_include_directories(${GLM_LIB_NAME}
    INTERFACE
    ${GLM_INC_PATH}
)

# entt
set(ENTT_LIB_NAME "entt")
set(ENTT_INC_PATH ${ENTT_LIB_NAME})

add_library(${ENTT_LIB_NAME} INTERFACE)
target_include_directories(${ENTT_LIB_NAME}
    INTERFACE
    ${ENTT_INC_PATH}
)