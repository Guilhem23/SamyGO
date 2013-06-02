
export HOST=arm_v5t_le
export PREFIX_DIR=/opt/arm_v5t_vfp_le/target/usr
export BUILD=arm-linux
export INSTALL_DIR="$PREFIX_DIR"
export PKG_CONFIG_PATH="$INSTALL_DIR/lib/pkgconfig"
PREFIX=$PREFIX_DIR
export CC=arm_v5t_le-gcc 
LDSHARED="arm_v5t_le-gcc -shared -Wl,-soname,libz.so.1" \
./configure --shared  --prefix=$PREFIX
make
make install
