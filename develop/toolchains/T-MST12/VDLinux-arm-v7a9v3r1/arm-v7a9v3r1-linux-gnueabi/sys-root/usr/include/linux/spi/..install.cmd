cmd_/opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/spi/.install := perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a9/sources/kernel-headers/include/linux/spi /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/spi arm spidev.h; perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a9/sources/kernel-headers/include/linux/spi /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/spi arm ; for F in ; do echo "\#include <asm-generic/$$F>" > /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/spi/$$F; done; touch /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/spi/.install
