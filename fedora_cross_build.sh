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
#cp /usr/i686-pc-mingw32/sys-root/mingw/bin/{boost_regex-gcc45-mt-1_44.dll,boost_program_options-gcc45-mt-1_44.dll,libsqlite3-0.dll,libgcc_s_sjlj-1.dll,libcurl-4.dll,libstdc++-6.dll,libcrypto-10.dll,libidn-11.dll,libssh2-1.dll,zlib1.dll,libiconv-2.dll,libintl-8.dll,libssl-10.dll,pthreadGC2.dll} ./
#i686-pc-mingw32-strip libstdc++-6.dll
#cd ../
#zip -r pichi-testbuild.zip pichi/