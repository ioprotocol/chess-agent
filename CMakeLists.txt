cmake_minimum_required(VERSION 3.10)
project(ChessAgent)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_PREFIX_PATH D:/dev_qt/5.11.1/mingw53_32)
set(OpenCV_DIR D:/opencv/build/install)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
find_package(PkgConfig REQUIRED)
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

find_package(OpenCV REQUIRED)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5Concurrent CONFIG REQUIRED)

qt5_wrap_cpp(MOC_FILES mainwindow.cpp mainwindow.h colorfilterdialog.cpp colorfilterdialog.h)
qt5_wrap_ui(WRAP_FILES mainwindow.ui colorfilterdialog.ui)

set(RCC_FILES )

include_directories(${OpenCV_DIR})

file(GLOB SOURCE_HS_FILES "*.h")
file(GLOB SOURCE_CPPS_FILES "*.cpp")

add_executable(ChessAgent ${MOC_FILES} ${WRAP_FILES} ${SOURCE_HS_FILES} ${SOURCE_CPPS_FILES})

target_link_libraries (ChessAgent ${OpenCV_LIBS} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Concurrent)