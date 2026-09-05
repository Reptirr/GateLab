cmake_minimum_required(VERSION 4.3)
project(GateLab)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/Program Files/Qt/6.11.0/mingw_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_library(GateLabCore STATIC
        GateLabCore/MultiScheme/include/MainView.h
        GateLabCore/MultiScheme/src/MainView.cpp
        GateLabCore/MultiScheme/include/PinItem.h
        GateLabCore/Controller/include/BiMap.h
        GateLabCore/Controller/include/Controller.h
        GateLabCore/GateLogic/include/LogicPin.h
        GateLabCore/GateLogic/include/LogicWire.h
        GateLabCore/GateLogic/src/LogicPin.cpp
        GateLabCore/GateLogic/src/LogicWire.cpp
        GateLabCore/MultiScheme/include/ComponentItem.h
        GateLabCore/MultiScheme/include/WireItem.h
        GateLabCore/Controller/src/InputMapper.cpp
        GateLabCore/Controller/include/InputMapper.h
        GateLabCore/MultiScheme/include/UIConstants.h
        GateLabCore/GateLogic/include/instant/LogicSource.h
        GateLabCore/MultiScheme/include/instant/SourceItem.h
        GateLabCore/MultiScheme/src/WireItem.cpp
        GateLabCore/Controller/include/LogicComponentsFactory.h
)

target_link_libraries(GateLabCore PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_include_directories(GateLabCore PUBLIC
        GateLabCore/MultiScheme/include
        GateLabCore/GateLogic/include
        GateLabCore/Controller/include
)

add_executable(GateLab main.cpp)
target_link_libraries(GateLab PRIVATE GateLabCore)

target_link_options(GateLabCore PRIVATE -fuse-ld=lld)
target_precompile_headers(GateLab PRIVATE
        <QWidget>
        <QObject>
        <QString>
        <QGraphicsScene>
        <QGraphicsItem>
)

enable_testing()
add_subdirectory(tests)


if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()