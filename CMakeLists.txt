cmake_minimum_required(VERSION 3.10)
project(ChessAgent)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(Qt5_DIR /home/xushy/qt5/5.11.1/gcc_64/lib/cmake/Qt5)
#set(RESOURCE_DIR resources/resources.qrc)
find_package(Qt5 REQUIRED Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

qt5_wrap_cpp(MOC mainwindow.h)
qt5_wrap_ui(UIC mainwindow.ui)
#qt5_add_resources(RCC resources.qrc)

set(CMAKE_CXX_STANDARD 11)
set(OpenCV_DIR /usr/local/share/OpenCV)

find_package (PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_DIR})

set(SOURCE_FILES main.cpp ${MOC} mainwindow.cpp ${UIC} chessaction.cpp chessaction.h chesspiece.cpp chesspiece.h application_utils.h application_utils.cpp screenshot.cpp screenshot.h detection.h knndection.cpp knndection.h detection.cpp)

add_executable(ChessAgent ${SOURCE_FILES})

target_link_libraries (ChessAgent X11 ${OpenCV_LIBS} Qt5::Widgets)