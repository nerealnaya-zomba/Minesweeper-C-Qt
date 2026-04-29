cmake_minimum_required(VERSION 3.5)

project(MinesweeperProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.hpp
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

    qt_add_executable(MinesweeperProject
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        game.hpp game.cpp
        difficulty.hpp difficulty.cpp
        gamefield.hpp gamefield.cpp
        cell.hpp cell.cpp
        randommineplacement.hpp randommineplacement.cpp
        point.hpp point.cpp
        statistics.hpp statistics.cpp
        settings.hpp settings.cpp
        gametimer.hpp gametimer.cpp
        gamestate.hpp
        gamewindow.hpp gamewindow.cpp gamewindow.ui
        settingswindow.hpp settingswindow.cpp settingswindow.ui
        themestyles.hpp themestyles.cpp
        howtoplay.hpp howtoplay.cpp howtoplay.ui
        aboutprogram.hpp aboutprogram.cpp aboutprogram.ui
        statisticswindow.hpp statisticswindow.cpp statisticswindow.ui
        resources.qrc
        soundmanager.hpp soundmanager.cpp
        mineplacementstrategy.hpp
        nosafeplacement.hpp nosafeplacement.cpp
        clusteredplacement.cpp clusteredplacement.hpp
        attemptsplacementstrategy.hpp attemptsplacementstrategy.cpp
        mineplacementfactory.hpp mineplacementfactory.cpp
        windowswitcher.hpp
        strategy.hpp strategy.cpp
        customdifficultywindow.hpp customdifficultywindow.cpp customdifficultywindow.ui


    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MinesweeperProject APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MinesweeperProject SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MinesweeperProject
            ${PROJECT_SOURCES}
            game.hpp game.cpp
            difficulty.hpp difficulty.cpp
            gamefield.hpp gamefield.cpp
            cell.hpp cell.cpp
            mineplacer.hpp mineplacer.cpp
            point.hpp point.cpp
            statistics.hpp statistics.cpp
            settings.hpp settings.cpp
            gametimer.hpp gametimer.cpp
            gamestate.hpp
            gamewindow.hpp gamewindow.cpp gamewindow.ui
            settingswindow.hpp settingswindow.cpp settingswindow.ui
            themestyles.hpp themestyles.cpp
            howtoplay.hpp howtoplay.cpp howtoplay.ui
            aboutprogram.hpp aboutprogram.cpp aboutprogram.ui
            statisticswindow.hpp statisticswindow.cpp statisticswindow.ui
            translationmanager.hpp translationmanager.cpp
        )
    endif()
endif()

target_link_libraries(MinesweeperProject PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

if(WIN32)
    target_link_libraries(MinesweeperProject PRIVATE winmm)
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MinesweeperProject)
endif()
set_target_properties(MinesweeperProject PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MinesweeperProject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MinesweeperProject)
endif()
