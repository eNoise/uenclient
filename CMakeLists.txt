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
	      src/loginform.cpp
	      src/uenclient.cpp
	      src/glooxsession.cpp
	      src/chatdialog.cpp
	      src/chatuseritem.cpp
	      src/privatechat.cpp
	      src/createtorrentdialog.cpp
	      src/helper.cpp
	      src/torrentdialog.cpp
	      src/addtorrentdialog.cpp

	      src/bencodeparser.cpp
	      src/connectionmanager.cpp
	      src/metainfo.cpp
	      src/peerwireclient.cpp
	      src/ratecontroller.cpp
	      src/filemanager.cpp
	      src/torrentclient.cpp
	      src/torrentserver.cpp
	      src/trackerclient.cpp

	      src/main.cpp)


qt4_automoc(${uenclient_SRCS})
qt4_wrap_ui(UEN_U forms/addtorrentform.ui)

set(UEN_LIBS
		${QT_QTCORE_LIBRARY} 
		${QT_QTGUI_LIBRARY}
		${QT_QTNETWORK_LIBRARY}
		${GLOOX_LIBRARIES}
)

#gloox fix
if(WIN32)
        set(UEN_LIBS ${UEN_LIBS} 
                      dnsapi
                      secur32
                      ws2_32
        )
endif(WIN32)


include_directories(
		    ${QT_INCLUDES} 
		    ${CMAKE_CURRENT_BINARY_DIR}
)
add_executable(uenclient ${uenclient_SRCS} ${UEN_U})
target_link_libraries(uenclient ${UEN_LIBS})

