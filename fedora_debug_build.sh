
cd win32build
mingw32-cmake -DCMAKE_BUILD_TYPE=Debug -DLOCAL_GLOOX=ON -DCMAKE_INSTALL_PREFIX=`pwd`/uenclient ../
mingw32-make -j4
mingw32-make install
cd uenclient
cp /usr/i686-pc-mingw32/sys-root/mingw/bin/{QtCored4.dll,QtGuid4.dll,QtNetworkd4.dll,QtScriptd4.dll,QtSvgd4.dll,QtXmld4.dll,libgcc_s_sjlj-1.dll,libstdc++-6.dll,zlib1.dll,libpng14-14.dll,libjpeg-62.dll,libtiff-3.dll} ./
