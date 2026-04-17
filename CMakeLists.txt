cmake_minimum_required(VERSION 3.15)
project(MyGameEngine VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include    # /Users/pavel/Desktop/WorkingProject/CplusWorks/AID/Practice1/dependencies/include #
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(GLFW QUIET glfw.3.4)
endif()

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/application/application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/gl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/logger/logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/states/MenuState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/states/PlayState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/states/PauseState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/states/StateManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/render/OpenGLAdapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ecs/World.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ecs/systems/RenderSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ecs/systems/AnimationSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ecs/systems/HierarchyUtils.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE /usr/local/include)

# Линковка библиотек
if(GLFW_FOUND)
    target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})
else()
    target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/library)
    target_link_libraries(${PROJECT_NAME} glfw.3.4)
endif()

target_link_libraries(${PROJECT_NAME}
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
)