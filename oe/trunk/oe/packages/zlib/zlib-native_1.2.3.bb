require zlib_${PV}.bb
ZLIB_EXTRA = ""
DEPENDS = "libtool-native"
FILESPATH = "${FILE_DIRNAME}/zlib-${PV}"

inherit native
