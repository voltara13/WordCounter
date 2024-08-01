cmake_minimum_required(VERSION 3.16)
project(TestApp LANGUAGES CXX)

include(qt.cmake)
include_directories(qml-controllers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Quick 
        QuickControls2
)
qt_standard_project_setup()

set(QML_COMPONENTS
    qml-components/AppButton.qml
    qml-components/AppButtonPanel.qml
    qml-components/AppHistogram.qml
    qml-components/AppView.qml
    qml-components/main.qml
)

set(CONTROLLERS_HEADERS
    qml-controllers/WordCounterController.h
)

set(CONTROLLERS_SOURCES
    qml-controllers/WordCounterController.cpp
)

set(PROJECT_HEADERS
    utils/WorkerThread.h
)

set(PROJECT_SOURCES
    utils/WorkerThread.cpp
    main.cpp
)

if(QT_VERSION VERSION_LESS 6.2)
    list(APPEND PROJECT_SOURCES qml.qrc)
endif()

qt_add_executable(TestApp 
    ${PROJECT_HEADERS} 
    ${PROJECT_SOURCES}
)

target_link_libraries(TestApp PRIVATE Qt6::QuickControls2)
target_precompile_headers(TestApp PRIVATE Precompiled.h)

if(QT_VERSION VERSION_GREATER_EQUAL 6.2)
    qt_add_qml_module(TestApp
        URI testapp
        VERSION 1.0
        RESOURCE_PREFIX "/qt/qml/"
        QML_FILES
            ${QML_COMPONENTS}
        SOURCES 
            ${CONTROLLERS_HEADERS}
            ${CONTROLLERS_SOURCES}
    )
endif()

set_target_properties(TestApp
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(TestApp
    PUBLIC
        Qt::Quick
)

if(Qt6_FOUND)
    qt_import_qml_plugins(TestApp)
endif()
