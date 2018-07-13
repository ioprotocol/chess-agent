cmake_minimum_required(VERSION 3.10)
project(ChessAgent)

set(CMAKE_CXX_STANDARD 11)
set(OpenCV_DIR /usr/local/share/OpenCV)

find_package (PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
pkg_check_modules (GTKMM REQUIRED gtkmm-3.0)

include_directories (${GTKMM_INCLUDE_DIRS})
link_directories (${GTKMM_LIBRARY_DIRS})

include_directories(${OpenCV_DIR})

set(SOURCE_FILES main.cpp chessaction.cpp chessaction.h chesspiece.cpp chesspiece.h application_utils.h application_utils.cpp screenshot.cpp screenshot.h chesswindow.cpp chesswindow.h chessworker.cpp chessworker.h detection.h knndection.cpp knndection.h detection.cpp)

add_executable(ChessAgent ${SOURCE_FILES})

add_definitions (${GTKMM_CFLAGS_OTHER})
target_link_libraries (ChessAgent ${GTKMM_LIBRARIES} X11 pthread ${OpenCV_LIBS})