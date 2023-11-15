#!/bin/sh

cd build_all/

cd lunasvg
rm -rf lunasvg
cd build
make clean
cd ..
cd ..
# uninstall lxrad
cd lxrad
rm -rf lunasvg
make clean
make uninstall
cd .. # back to build_all

# uninstall/rm simavr
cd simavr
make clean
DESTDIR="/usr/local"
rm -rf $DESTDIR/include/simavr
rm -rf $DESTDIR/lib/libsimavr.a
rm -rf $DESTDIR/lib/pkgconfig/simavr-avr.pc
rm -rf $DESTDIR/lib/pkgconfig/simavr.pc
rm -rf $DESTDIR/bin/simavr

# remove tty0tty
rm -rf /usr/local/bin/tty0tty

cd .. # back to main
# uninstall picsimlab
cd src
make clean
make uninstall

cd ../tools

cd PinViewer/
make clean
make uninstall
cd ..

cd espmsim/
make clean
make uninstall
cd ..

cd srtank/
make clean
make uninstall
cd ..

