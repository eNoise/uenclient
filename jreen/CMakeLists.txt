project (jreen)
cmake_minimum_required (VERSION 2.6)

find_package(QCA2 REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR} 
		    ${PROJECT_SOURCE_DIR} 
		    ${QT_INCLUDES} 
		    ${QCA2_INCLUDE_DIR})
aux_source_directory(${PROJECT_SOURCE_DIR} jreen_srcs)
aux_source_directory(${PROJECT_SOURCE_DIR}/jdns jdns_srcs)
aux_source_directory(${PROJECT_SOURCE_DIR}/simplesasl simplesasl_srcs)
qt4_automoc(${jreen_srcs})
if (WIN32)
  add_definitions( -DWIN32 -D_WIN32)
endif (WIN32)
#configure_file( ${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_BINARY_DIR}/config.h.unix )

set (LINK STATIC)
add_library (jreen ${LINK} 
		    ${jreen_srcs} 
		    ${jdns_srcs} 
		    ${simplesasl_srcs})
target_link_libraries(jreen ${QCA2_LIBRARIES})
