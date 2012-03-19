# Default Setting
cmake_minimum_required(VERSION 2.6)
project(uenclient)
set (PROJECT_NAME_GLOBAL uenclient)
set (VERSION "0.1.0")
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include (CheckIncludeFile)


# Git Revision
execute_process(COMMAND ${PROJECT_SOURCE_DIR}/getrevision.sh
		OUTPUT_VARIABLE GIT_REVISION 
		OUTPUT_STRIP_TRAILING_WHITESPACE
		)
execute_process(COMMAND git describe
		OUTPUT_VARIABLE GIT_DESCRIBE 
		OUTPUT_STRIP_TRAILING_WHITESPACE
		)
add_definitions(-DGIT_REVISION="${GIT_REVISION}")
add_definitions(-DGIT_DESCRIBE="${GIT_DESCRIBE}")
add_definitions(-DGIT_VERSION="${VERSION}")


#Compiling flags
if (NOT MSVC)
    #enable c++0X
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

    #enable win32 defs
    if (WIN32)
      add_definitions(-DWIN32 -D_WIN32)
    endif (WIN32)

    if (CMAKE_CROSSCOMPILING)
	if(WIN32)
		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
			set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,console")
		else(CMAKE_BUILD_TYPE STREQUAL "Debug")
			set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
		endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	endif(WIN32)
    endif (CMAKE_CROSSCOMPILING)
endif (NOT MSVC)


# Settings
option (LOCAL_GLOOX "Use local gloox lib" ON)


#Find Packages
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
	      src/smilehandler.cpp
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


# Translations
file (GLOB UEN_TRANS translations/*.ts)
option (UPDATE_TRANSLATIONS "Update source translation translations/*.ts files (WARNING: make clean will delete the source .ts files! Danger!)")
if (UPDATE_TRANSLATIONS)
  qt4_create_translation(UEN_QM ${UEN_CPP} ${UEN_TRANS})
else (UPDATE_TRANSLATIONS)
  qt4_add_translation(UEN_QM ${UEN_TRANS})
endif (UPDATE_TRANSLATIONS) 
#add_custom_target (uenclient DEPENDS ${UEN_QM})

qt4_add_resources(UEN_RCC
				uenclient.qrc
)

set(UEN_LIBS
		${QT_QTCORE_LIBRARY} 
		${QT_QTGUI_LIBRARY}
		${QT_QTNETWORK_LIBRARY}
		${QT_QTXML_LIBRARY}
		${QT_QTSCRIPT_LIBRARY}
		${GLOOX_LIBRARIES}
)
# Some win32 libs
if(WIN32)
        set(UEN_LIBS ${UEN_LIBS} 
                      dnsapi
                      secur32
		      crypt32
		      wsock32
        )
endif(WIN32)


# Add windows icon to exe file
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


# Include Directory
include_directories(
		    ${QT_INCLUDES} 
		    ${CMAKE_CURRENT_BINARY_DIR}
)


# Compilation
add_executable(uenclient ${UEN_CPP} ${UEN_UI} ${UEN_QM} ${UEN_RCC})
target_link_libraries(uenclient ${UEN_LIBS})


# Directory setup
if(WIN32)
  set (BIN_DIR .)
else(WIN32)
  set (BIN_DIR bin)
endif(WIN32)

if (NOT SHARE_DIR)
  set (SHARE_DIR "share")
endif (NOT SHARE_DIR)

if (APPLE)
  set (CLIENT_DATA_DIR "${PROJECT_NAME_GLOBAL}.app")
elseif (WIN32)
  set (CLIENT_DATA_DIR ".")
else (APPLE)
  set (CLIENT_DATA_DIR "${CMAKE_INSTALL_PREFIX}/${SHARE_DIR}/${PROJECT_NAME_GLOBAL}")
endif (APPLE)
add_definitions(-DCLIENT_DATA_DIR="${CLIENT_DATA_DIR}")


# Instalation
install (TARGETS uenclient DESTINATION ${BIN_DIR})
install (DIRECTORY share/avatars/ DESTINATION "${CLIENT_DATA_DIR}/avatars")
install (DIRECTORY share/smiles/ DESTINATION "${CLIENT_DATA_DIR}/smiles")
install (DIRECTORY share/icons/ DESTINATION "${CLIENT_DATA_DIR}/icons")
install(FILES ${UEN_QM} DESTINATION "${CLIENT_DATA_DIR}/translations")
