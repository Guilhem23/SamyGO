# Kernel data not imported??? for what the hell we inherit module??? *Arris *
KERNEL_MAJOR_VERSION = "${MACHINE_KERNEL_VERSION}"

EXTRA_OECONF = "--with-kerneldir=${STAGING_KERNEL_DIR} ${DRIVER}"

PARALLEL_MAKE = ""

DRIVER ?= "--with-driver=serial --with-driver=mceusb"
# fails to build at the moment, re-enable later
#DRIVER_collie = "--with-driver=sa1100"
DRIVER_h3600 = "--with-driver=sa1100"
DRIVER_simpad = "--with-driver=sa1100"
DRIVER_nslu2 = "--with-driver=nslu2"
DRIVER_samygo = "--with-driver=mceusb"
