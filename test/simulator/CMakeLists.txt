
cmake_minimum_required(VERSION 3.10)

find_package(Qt5 5.15 COMPONENTS Core Widgets Gui REQUIRED)

set(HEADERS
    Simulator.h
)

QT5_WRAP_CPP(SOURCES ${HEADERS})

add_executable(${PROJECT_NAME}
    ${SOURCES}
    Simulator.cpp
    main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    tones
)
