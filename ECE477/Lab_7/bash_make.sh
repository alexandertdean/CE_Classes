#!/bin/bash
# Code to download, patch, and build avrdude 6.3 on OpenSuse
# Run as regular user 
# 
export PREFIX=/usr/local
cd
sudo zypper remove avrdude
sudo zypper install wget gcc libevent-devel openssl-devel gtk2-devel libuuid-devel sqlite-devel libjansson-devel intltool cmake qt-devel fuse-devel libtool vala gcc-c++ libcurl-devel 

sudo zypper install make automake autoconf flex bison libusb-compat-devel libusb-1_0-devel libX11-devel libftdi1-devel subversion

wget http://download.savannah.gnu.org/releases/avrdude/avrdude-doc-6.3.pdf

mkdir avrdude
cd avrdude

svn co svn://svn.sv.gnu.org/avrdude/tags/RELEASE_6_3
#wget http://download.savannah.gnu.org/releases/avrdude/avrdude-6.3.tar.gz
#gunzip -c avrdude-6.3.tar.gz |tar xf -
cd RELEASE_6_3
wget http://savannah.nongnu.org/bugs/download.php?file_id=32171
patch < download.php\?file_id\=32171
./bootstrap
mkdir obj-avr
cd obj-avr
sudo ../configure --prefix=$PREFIX
sudo make
sudo make install
