cmd_/opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/netfilter_bridge/.install := perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a9/sources/kernel-headers/include/linux/netfilter_bridge /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/netfilter_bridge arm ebt_802_3.h ebt_among.h ebt_arp.h ebt_arpreply.h ebt_ip.h ebt_ip6.h ebt_limit.h ebt_log.h ebt_mark_m.h ebt_mark_t.h ebt_nat.h ebt_nflog.h ebt_pkttype.h ebt_redirect.h ebt_stp.h ebt_ulog.h ebt_vlan.h ebtables.h; perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a9/sources/kernel-headers/include/linux/netfilter_bridge /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/netfilter_bridge arm ; for F in ; do echo "\#include <asm-generic/$$F>" > /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/netfilter_bridge/$$F; done; touch /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/netfilter_bridge/.install
