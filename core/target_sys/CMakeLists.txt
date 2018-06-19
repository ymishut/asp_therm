project(itarget_sys)

add_library(${PROJECT_NAME} INTERFACE)

# for windows
target_include_directories(${PROJECT_NAME}
    INTERFACE
    ${PROJECT_SOURCE_DIR}/win
)