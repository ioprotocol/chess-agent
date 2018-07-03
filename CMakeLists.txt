cmake_minimum_required(VERSION 3.10)
project(ChessAgent)

set(CMAKE_CXX_STANDARD 11)
set(OpenCV_DIR /usr/local/share/OpenCV)

find_package (PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
pkg_check_modules (GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GLIB REQUIRED glib-2.0>=2.40)

include_directories (${GTK3_INCLUDE_DIRS})
link_directories (${GTK3_LIBRARY_DIRS})

include_directories(${OpenCV_DIR})

set(SOURCE_FILES main.cpp screenshot-utils.h screenshot-utils.c screenshot-config.h screenshot-config.c chess.cpp chess.h piece.cpp piece.h application-utils.h application-utils.c)

add_executable(ChessAgent ${SOURCE_FILES})

add_definitions (${GTK3_CFLAGS_OTHER})
target_link_libraries (ChessAgent ${GTK3_LIBRARIES} X11 pthread ${OpenCV_LIBS})