#!/bin/bash 
#show errors in color red 
set -o pipefail
cl()("$@" 2>&1>&3|sed $'s,.*,\e[31m&\e[m,'>&2)3>&1

echo -e "\033[1;32m ---------------------- update and install packages ---------------------- \033[0m"
# disable linux/deb install pkgs
# cl sudo apt-get update
# cl sudo apt-get -y upgrade
# cl sudo apt-get -y install git doxygen gcc g++ make libwxgtk3.*-dev \
# libelf-dev freeglut3-dev cutecom gcc-avr avr-libc libopenal-dev libncurses-dev gtkwave \
# gedit cmake bison++ pulseview
# cl sudo apt-get -y install linux-headers-`uname -r` dkms 
# enable homebrew install pkgs
#brew install openal-soft wxwidgets gpsim
brew install -q cmake pkg-config coreutils #coreutils to use gnu cp for installation
brew install -q wxwidgets doxgen # for GUI
brew install -q glib # for gpsim headers
brew tap osx-cross/homebrew-avr # for simavr
brew install -q avrdude avr-binutils avr-gcc@12 libelf

cl pwd
cl mkdir -p build_all
cd build_all
cl pwd
echo -e "\033[1;32m ---------------------- download deps -------------------------------------\033[0m"
#git clone --depth=1 https://github.com/lcgamboa/picsim.git
git clone --depth=1 https://github.com/lcgamboa/lxrad.git
#git clone --depth=1 https://github.com/lcgamboa/tty0tty.git
git clone --depth=1 https://github.com/lcgamboa/simavr.git
#git clone --depth=1 https://github.com/lcgamboa/uCsim_picsimlab.git
#cl sudo apt-get -y install python3 libglib2.0-dev libpixman-1-dev libfdt-dev gpsim-dev gpsim \
#ninja-build meson libgcrypt-dev libslirp-dev
#git clone --depth=1 --no-single-branch https://github.com/lcgamboa/qemu.git	
echo -e "\033[1;32m ---------------------- build and install picsim ------------------------- \033[0m"
#cd picsim
#ln -sf include picsim
#cl git pull --no-rebase
#cl make clean;make -j$(sysctl -n hw.ncpu)
#cl sudo make install
#cd ..
echo -e "\033[1;32m ---------------------- build and install lxrad -------------------------- \033[0m"
cl pwd
cd lxrad
cl pwd
git pull --no-rebase
cl cp -f ../../lxrad-patch-files/Makefile.in lib/Makefile.in
cl cp -f ../../lxrad-patch-files/lxaudio.h include/lxaudio.h
cl cp -f ../../lxrad-patch-files/lxaudio.cc lib/lxaudio.cc
cl ./make_deps.sh
#cl ./configure --prefix=/usr/local
cl ./configure --prefix=/usr/local CC="g++ -std=c++17" CXX="g++ -std=c++17" LDFLAGS="-D_APPLE_ -framework OpenAL"
cl make clean;make -j$(sysctl -n hw.ncpu)
cl make install
#cl rm -rf /usr/local/lib/liblxrad.*
#cl cp -dvf lib/liblxrad.* /usr/local/lib/ #coreutils cp work with cp -dvf now
#cd .. # lxrad
cd .. # build_all
cl pwd
echo -e "\033[1;32m ---------------------- build and install tty0tty ------------------------ \033[0m"
#cd tty0tty/module
cd .. #back to main folder
cl pwd
cd tty0tty/pts
#git pull --no-rebase
cl make
cl cp tty0tty /usr/local/bin/
#cl sudo ./dkms-install.sh
#cl make clean;make -j$(sysctl -n hw.ncpu)
#cl sudo make install
#sudo usermod -a -G dialout `whoami`
#cl sudo modprobe tty0tty
cd ../../ # back to main folder
cl pwd
cd build_all
cl pwd
echo -e "\033[1;32m ---------------------- build and install simavr ------------------------- \033[0m"
cd simavr
cl pwd
git pull --no-rebase
cl make clean; PATH=$PATH:"/usr/local/opt/avr-gcc@12/bin" make build-simavr -j$(sysctl -n hw.ncpu) 
cl make install-simavr
cd ../ # back to build_all
cl pwd
echo -e "\033[1;32m ---------------------- build and install uCsim -------------------------- \033[0m"
cl pwd
#cd uCsim_picsimlab
#git pull --no-rebase
#cl ./config_linux.sh
#cl make clean;make -j$(sysctl -n hw.ncpu)
#cd picsimlab
#cl make clean;make -j$(sysctl -n hw.ncpu)
#cl make install
#cd ../../
cl pwd
echo -e "\033[1;32m ---------------------- build qemu  ---------------------- \033[0m"
cl pwd
#cd qemu
#cl git checkout picsimlab-stm32
#git pull --no-rebase
#cl ./build_libqemu-stm32.sh
#cd build
#cl strip libqemu-stm32.so
#cl install -d ../../../lib/qemu/
#cl cp libqemu-stm32.so ../../../lib/qemu/
#cd ..
#cl git checkout picsimlab-esp32
#cl ./build_libqemu-esp32.sh
#cd build
#cl strip libqemu-xtensa.so
#cl cp libqemu-xtensa.so ../../../lib/qemu/
#cl strip libqemu-riscv32.so
#cl cp libqemu-riscv32.so ../../../lib/qemu/
#cd ..
#cl install -d ../../lib/qemu/fw
#cl cp pc-bios/esp32-v3-rom*.bin pc-bios/esp32c3-rom.bin ../../lib/qemu/fw/
#cd ..
cl pwd
echo -e "\033[1;32m ---------------------- build and install picsimlab ---------------------- \033[0m"
cl pwd
#git pull --no-rebase
cd ../ # back to main
cl pwd
cd src
cl make clean;make -j$(sysctl -n hw.ncpu) -f Makefile.static install_app
#cl make install_app
cl pwd
cd ../ # back to main
# compile tools
cd tools
cd PinViewer/
cl make clean; make -j$(sysctl -n hw.ncpu) -f Makefile.static install_app
cd ../espmsim/
cl make clean; make -j$(sysctl -n hw.ncpu) -f Makefile.static install_app
cd ../srtank/
cl make clean; make -j$(sysctl -n hw.ncpu) -f Makefile.static install_app
cd ..
#cl make install_app
cl pwd
#cl make install
#user=`whoami`
#cl sudo usermod -a -G dialout $user
echo -e "\033[1;32m ---------------------- done! -------------------------------------------- \033[0m"
echo -e "\033[1;32m logout your session and login to use serial \033[0m"
#sudo ldconfig
#if xhost > /dev/null 2>&1 ; then
#cl picsimlab
#fi
