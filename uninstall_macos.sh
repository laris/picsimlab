#!/bin/sh

cd build_all/

# uninstall lxrad
cd lxrad
make uninstall
cd .. # back to build_all

# uninstall/rm simavr
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
make uninstall

cd ../tools

cd PinViewer/
make uninstall
cd ..

cd espmsim/
make uninstall
cd ..

cd srtank/
make uninstall
cd ..

