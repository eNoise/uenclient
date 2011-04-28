cmake_minimum_required(VERSION 2.6)
project(uenclient)
set (PROJECT_NAME_GLOBAL uenclient)
set (VERSION "0.1.0")
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include (CheckIncludeFile)

#Compiling flags
if (NOT MSVC)
    if (CMAKE_CROSSCOMPILING)
	if(WIN32)
		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
			set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,console")
		else(CMAKE_BUILD_TYPE STREQUAL "Debug")
			set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
		endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	endif(WIN32)
    endif (CMAKE_CROSSCOMPILING)
    #if (NOT APPLE)
    #    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--as-needed")
    #    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--as-needed")
    #endif (NOT APPLE)
    #set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -pipe -DNDEBUG")
    #set (CMAKE_CXX_FLAGS_RELEASE     "${CMAKE_CXX_FLAGS_RELEASE} -DQT_NO_DEBUG -pipe")
    #set (CMAKE_CXX_FLAGS_DEBUG       "${CMAKE_CXX_FLAGS_DEBUG} -UNDEBUG -g3 -pipe")
endif (NOT MSVC)


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


set(UEN_CPP    
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

	      src/bencoder.cpp
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


qt4_automoc(${UEN_CPP})
qt4_wrap_ui(UEN_UI forms/addtorrentform.ui)

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


#icon
if(MINGW AND WIN32)
  ADD_CUSTOM_COMMAND (OUTPUT
            "${CMAKE_CURRENT_BINARY_DIR}/uenclient_rc.o"
            COMMAND i686-pc-mingw32-windres
            -I"${CMAKE_CURRENT_SOURCE_DIR}/"
            -i "${CMAKE_CURRENT_SOURCE_DIR}/uenclient.rc"
            -o "${CMAKE_CURRENT_BINARY_DIR}/uenclient_rc.o"
            )
  set(UEN_CPP ${UEN_CPP} "${CMAKE_CURRENT_BINARY_DIR}/uenclient_rc.o")
endif(MINGW AND WIN32)

include_directories(
		    ${QT_INCLUDES} 
		    ${CMAKE_CURRENT_BINARY_DIR}
)
add_executable(uenclient ${UEN_CPP} ${UEN_UI})
target_link_libraries(uenclient ${UEN_LIBS})

#Install