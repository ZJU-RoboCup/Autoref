cmake_minimum_required(VERSION 3.1)

project(Li LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

find_package(Qt5 COMPONENTS Core Quick Gui QuickControls2 REQUIRED)

include_directories(
    src
)
set(PROJECT_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
)
include_directories(${PROJECT_INCLUDE_DIRS})
file(GLOB CPPFILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/*.cpp"
)
set(RESOURCES
    Li.qrc
)
set(SOURCE
    ${CPPFILES}
    ${RESOURCES}
)
add_executable(${PROJECT_NAME} ${SOURCE})
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick Qt5::Gui Qt5::QuickControls2)
