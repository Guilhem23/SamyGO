cmd_/opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/can/.install := perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a9/sources/kernel-headers/include/linux/can /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/can arm bcm.h error.h netlink.h raw.h; perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a9/sources/kernel-headers/include/linux/can /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/can arm ; for F in ; do echo "\#include <asm-generic/$$F>" > /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/can/$$F; done; touch /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/can/.install
