inherit pkgconfig

require openssl.inc
SRC_URI[md5sum] = "a5cb5f6c3d11affb387ecf7a997cac0c"
SRC_URI[sha256sum] = "7131242042dbd631fbd83436f42aea1775e7c32f587fa4ada5a01df4c3ae8e8b"

PR = "${INC_PR}.0"

export OE_LDFLAGS="${LDFLAGS}"

SRC_URI += "file://configure-targets.patch;patch=1 \
            file://shared-libs.patch;patch=1 \
            file://debian.patch;patch=1 \
            file://oe-ldflags.patch;patch=1"

PARALLEL_MAKE = ""

DEFAULT_PREFERENCE = "1"
