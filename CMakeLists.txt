cmake_minimum_required(VERSION 3.20...4.3)
project(GateLab)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "no cmake_prefix_path. set it for qt 6.5 path")
endif()

find_package(Qt6 6.4.2 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_library(GateLabCore STATIC
        GLCore/MultiScheme/include/MainView.h
        GLCore/MultiScheme/src/MainView.cpp
        GLCore/MultiScheme/include/PinItem.h
        GLCore/Controller/include/Controller.h
        GLCore/GateLogic/include/LogicPin.h
        GLCore/GateLogic/include/LogicWire.h
        GLCore/GateLogic/src/LogicPin.cpp
        GLCore/GateLogic/src/LogicWire.cpp
        GLCore/MultiScheme/include/ComponentItem.h
        GLCore/MultiScheme/include/WireItem.h
        GLCore/Controller/src/InputMapper.cpp
        GLCore/Controller/include/InputMapper.h
        GLCore/MultiScheme/include/UIConstants.h
        GLCore/GateLogic/include/instant/LogicSource.h
        GLCore/MultiScheme/include/instant/SourceItem.h
        GLCore/MultiScheme/src/WireItem.cpp
        GLCore/Controller/include/LogicComponentsFactory.h
        GLCore/GateLogic/include/LogicUtils.h
        GLCore/Controller/include/Mode.h
        IDEUI/include/GLToolBar.h
        IDEUI/include/MainWindow.h
        GLCore/MultiScheme/include/WireLine.h
        GLCore/MultiScheme/include/WireNode.h
        GLCore/MultiScheme/include/WireEndpoint.h
        GLCore/MultiScheme/src/WireNode.cpp
        GLCore/MultiScheme/src/WireLine.cpp
        GLCore/MultiScheme/include/Utils.h
        my_assert.h
        GLCore/MultiScheme/include/MoveHelper.h
        GLCore/GateLogic/include/instant/LogicNTransistor.h
        GLCore/MultiScheme/include/instant/NTransistorItem.h
        GLCore/Controller/include/LogicController.h
        GLCore/Controller/include/UIController.h
        GLCore/Commands/include/ComponentCommands.h
        GLCore/Commands/include/GLCommand.h
        GLCore/Controller/include/ModeManager.h
        GLCore/Controller/include/GLCore.h
)

target_link_libraries(GateLabCore PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_include_directories(GateLabCore PUBLIC
        GLCore/MultiScheme/include
        GLCore/GateLogic/include
        GLCore/Controller/include
        IDEUI/include
        GLCore/Commands/include
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