cmd_/opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/tc_ematch/.install := perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a9/sources/kernel-headers/include/linux/tc_ematch /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/tc_ematch arm tc_em_cmp.h tc_em_meta.h tc_em_nbyte.h tc_em_text.h; perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a9/sources/kernel-headers/include/linux/tc_ematch /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/tc_ematch arm ; for F in ; do echo "\#include <asm-generic/$$F>" > /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/tc_ematch/$$F; done; touch /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/tc_ematch/.install
