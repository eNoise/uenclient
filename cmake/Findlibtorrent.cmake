
FIND_PATH( LIBTORRENT_INCLUDE_DIR "libtorrent/torrent_info.hpp" )
FIND_LIBRARY( LIBTORRENT_LIBRARIES torrent-rasterbar )

if( LIBTORRENT_LIBRARIES AND LIBTORRENT_INCLUDE_DIR )
	message( STATUS "Found libtorrent: ${LIBTORRENT_LIBRARIES}" )
	set( LIBTORRENT_FOUND 1 )
else( LIBTORRENT_LIBRARIES AND LIBTORRENT_INCLUDE_DIR )
	message( STATUS "Could NOT find libtorrent" )
endif( LIBTORRENT_LIBRARIES AND LIBTORRENT_INCLUDE_DIR )