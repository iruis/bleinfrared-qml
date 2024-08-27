cmake_minimum_required(VERSION 3.16)

project(bleinfrared-qml VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 라즈비안 or 우분투에서 필요한 패키지
# qt6-base-dev qt6-serialport-dev qt6-declarative-dev
find_package(Qt6 6.2 REQUIRED COMPONENTS Quick QuickControls2 SerialPort)

qt_add_executable(bleinfrared-qml
    main.cpp
)

qt_add_qml_module(bleinfrared-qml
    URI BleInfrared
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES commander.h commander.cpp
        SOURCES settings.h settings.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(bleinfrared-qml PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.bleinfrared-qml
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(bleinfrared-qml
    PRIVATE Qt6::Quick Qt6::QuickControls2 Qt6::SerialPort
)

include(GNUInstallDirs)
install(TARGETS bleinfrared-qml
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/out)
