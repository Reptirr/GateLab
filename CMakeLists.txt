cmake_minimum_required(VERSION 4.2.)
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
        MultiScheme/include/MainView.h
        MultiScheme/src/MainView.cpp
        MultiScheme/include/PinItem.h
        Controller/include/BiMap.h
        Controller/include/Controller.h
        GateLogic/include/LogicPin.h
        GateLogic/include/LogicWire.h
        GateLogic/src/LogicPin.cpp
        GateLogic/src/LogicWire.cpp
        MultiScheme/include/ComponentItem.h
        MultiScheme/include/WireItem.h
        Controller/src/InputMapper.cpp
        Controller/include/InputMapper.h
        MultiScheme/include/UIConstants.h
        GateLogic/include/instant/LogicSource.h
        MultiScheme/include/instant/SourceItem.h
        MultiScheme/src/WireItem.cpp
        Controller/include/LogicComponentsFactory.h
        GateLogic/include/LogicUtils.h
        Controller/include/Mode.h
        IDEUI/include/GLToolBar.h
        IDEUI/include/MainWindow.h
        MultiScheme/include/WireLine.h
        MultiScheme/include/WireNode.h
        MultiScheme/include/WireEndpoint.h
        MultiScheme/src/WireNode.cpp
        MultiScheme/src/WireLine.cpp
        MultiScheme/include/Utils.h
        my_assert.h
        MultiScheme/include/MoveHelper.h
        GateLogic/include/instant/LogicNTransistor.h
        MultiScheme/include/instant/NTransistorItem.h
)

target_link_libraries(GateLabCore PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_include_directories(GateLabCore PUBLIC
        MultiScheme/include
        GateLogic/include
        Controller/include
        IDEUI/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(GateLab main.cpp)
target_link_libraries(GateLab PRIVATE GateLabCore)


if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(GateLab PROPERTIES WIN32_EXECUTABLE ON)
endif()

add_custom_target(dist
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_SOURCE_DIR}/dist"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:GateLab>" "${CMAKE_SOURCE_DIR}/dist"
        COMMAND "${WINDEPLOYQT}" --release --no-translations
        "${CMAKE_SOURCE_DIR}/dist/$<TARGET_FILE_NAME:GateLab>"
        COMMENT "Packaging GateLab into dist/"
        VERBATIM
)
add_dependencies(dist GateLab)



target_precompile_headers(GateLab PRIVATE
        <QWidget>
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