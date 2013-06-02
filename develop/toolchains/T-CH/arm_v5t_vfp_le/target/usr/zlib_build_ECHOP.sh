
export HOST=arm-v7a8-linux-gnueabi
export PREFIX_DIR=/opt/VDLinux-armv7a8/arm-v7a8-linux-gnueabi/libc/usr
export BUILD=arm-linux
export INSTALL_DIR="$PREFIX_DIR"
export PKG_CONFIG_PATH="$INSTALL_DIR/lib/pkgconfig"
PREFIX=$PREFIX_DIR
export CC=arm-v7a8-linux-gnueabi-gcc 
LDSHARED="arm-v7a8-linux-gnueabi-gcc -shared -Wl,-soname,libz.so.1" \
./configure --shared  --prefix=$PREFIX
make
make install