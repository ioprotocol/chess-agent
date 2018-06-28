cmake_minimum_required(VERSION 3.10)
project(ChessAgent)

set(CMAKE_CXX_STANDARD 11)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK3 REQUIRED gtk+-3.0)

include_directories (${GTK3_INCLUDE_DIRS})
link_directories (${GTK3_LIBRARY_DIRS})
set(SOURCE_FILES main.cpp screenshot-utils.h screenshot-utils.c screenshot-config.h screenshot-config.c)

add_executable(ChessAgent ${SOURCE_FILES})

add_definitions (${GTK3_CFLAGS_OTHER})
target_link_libraries (ChessAgent ${GTK3_LIBRARIES} X11)