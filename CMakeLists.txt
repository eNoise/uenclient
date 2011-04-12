cmake_minimum_required(VERSION 2.6)
project(uenclient)
set (PROJECT_NAME_GLOBAL uenclient)
set (VERSION "0.1.0")
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include (CheckIncludeFile)

option (LOCAL_GLOOX "Use local gloox lib" OFF)

find_package(Qt4 REQUIRED)

if (LOCAL_GLOOX)
  add_subdirectory(gloox)
  include_directories ("${PROJECT_SOURCE_DIR}")
  set (GLOOX_LIBRARIES "gloox")
else (LOCAL_GLOOX)
  find_package (Gloox REQUIRED)
  include_directories(${GLOOX_INCLUDE_DIR})
endif (LOCAL_GLOOX)


set(uenclient_SRCS 
	      src/downloadtorrent.cpp  
	      src/uenclient.cpp
	      src/chatdialog.cpp
	      src/chatuseritem.cpp
	      src/createtorrentdialog.cpp
	      src/helper.cpp
	      src/main.cpp)


qt4_automoc(${uenclient_SRCS})

include_directories(
		    ${QT_INCLUDES} 
		    ${CMAKE_CURRENT_BINARY_DIR})
add_executable(uenclient ${uenclient_SRCS})
target_link_libraries(uenclient 
				${QT_QTCORE_LIBRARY} 
				${QT_QTGUI_LIBRARY}
				${QT_QTNETWORK_LIBRARY}
				${GLOOX_LIBRARIES}
		      )
