cmake_minimum_required(VERSION 3.10)
project(ChessAgent)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_PREFIX_PATH D:/Qt/Qt5.11.1/5.11.1)
set(OpenCV_DIR D:/opencv/build/install)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(OpenCV CONFIG REQUIRED)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5Concurrent CONFIG REQUIRED)

set(RCC_FILES )

file(GLOB SOURCE_HS_FILES "*.h")
file(GLOB SOURCE_CPPS_FILES "*.cpp")

add_executable(ChessAgent ${SOURCE_HS_FILES} ${SOURCE_CPPS_FILES})

target_link_libraries (ChessAgent ${OpenCV_LIBS} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Concurrent)