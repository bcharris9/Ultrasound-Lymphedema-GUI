cmake_minimum_required(VERSION 3.16)

project(Ultrasound_GUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
### IMPORT SERIALPORT
find_package(Qt6 REQUIRED COMPONENTS SerialPort)
#target_link_libraries(Ultrasound_GUI PRIVATE Qt6::SerialPort)
###

set(TS_FILES Ultrasound_GUI_en_US.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Ultrasound_GUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        csvwriter.h csvwriter.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Ultrasound_GUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Ultrasound_GUI SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Ultrasound_GUI
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(Ultrasound_GUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
###
target_link_libraries(Ultrasound_GUI PRIVATE Qt6::SerialPort)

#target_link_libraries(Ultrasound_GUI PRIVATE Qt6::Core Qt6::Gui Qt6::Bluetooth)

set(CUSTOM_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/macos/Info.plist")

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Ultrasound_GUI)
endif()
set_target_properties(Ultrasound_GUI PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_ICON_FILE "sketch.icns"  # still required for icon
    MACOSX_BUNDLE_INFO_PLIST ${CUSTOM_PLIST}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Ultrasound_GUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Ultrasound_GUI)
endif()
