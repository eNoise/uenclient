project(uenclient)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(uenclient_SRCS uenclient.cpp main.cpp)
qt4_automoc(${uenclient_SRCS})
add_executable(uenclient ${uenclient_SRCS})
target_link_libraries(uenclient ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
