#rev_ver=`svn info|grep Revision|cut -d: -f2|awk '{print $1}'`

rm -fr win32build
mkdir win32build
cd win32build
mkdir uenclient
mingw32-cmake -DCMAKE_BUILD_TYPE=Release -DLOCAL_GLOOX=ON -DCMAKE_INSTALL_PREFIX=`pwd`/uenclient ../
mingw32-make -j4
mingw32-make install
cd uenclient
i686-pc-mingw32-strip uenclient.exe
cp /usr/i686-pc-mingw32/sys-root/mingw/bin/{QtCore4.dll,QtGui4.dll,QtNetwork4.dll,QtScript4.dll,libstdc++-6.dll,libgcc_s_sjlj-1.dll,zlib1.dll,libpng14-14.dll} ./
cd ../
zip -r uenclient-testbuild.zip uenclient/
