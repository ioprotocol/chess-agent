cmake_minimum_required(VERSION 3.10)
project(ChessAgent)

set(CMAKE_CXX_STANDARD 11)
#set(Qt5_DIR /home/xushy/qt5/5.11.1/gcc_64/lib/cmake/Qt5)
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(Qt5_DIR /home/xsy/expand_disk1/qt5/5.11.1/gcc_64)
    set(OpenCV_DIR /usr/local/share/OpenCV)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_PREFIX_PATH D:/Qt/5.10.0/mingw53_32)
    set(OpenCV_DIR D:/opencv/build/install)
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")


set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)

qt5_wrap_cpp(MOC_FILES mainwindow.cpp mainwindow.h)
qt5_wrap_ui(WRAP_FILES mainwindow.ui)

set(RCC_FILES )

include_directories(${OpenCV_DIR} ${Qt5Core_DIR} ${Qt5Gui_DIR} ${Qt5Widgets_DIR})

file(GLOB SOURCE_HS_FILES "*.h")
file(GLOB SOURCE_CPPS_FILES "*.cpp")

add_executable(ChessAgent ${WRAP_FILES} ${SOURCE_HS_FILES} ${SOURCE_CPPS_FILES})

target_link_libraries (ChessAgent ${OpenCV_LIBS} Qt5::Widgets Qt5::Core Qt5::Gui)