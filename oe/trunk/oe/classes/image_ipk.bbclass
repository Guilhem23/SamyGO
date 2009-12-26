inherit rootfs_ipk

# We need to recursively follow RDEPENDS and RRECOMMENDS for images
BUILD_ALL_DEPS = "1"
do_rootfs[recrdeptask] = "do_package"

# Images are generally built explicitly, do not need to be part of world.
EXCLUDE_FROM_WORLD = "1"

USE_DEVFS ?= "0"

PACKAGE_ARCH = "${MACHINE_ARCH}"

def get_image_deps(d):
	import bb
	str = ""
	for type in (bb.data.getVar('IMAGE_FSTYPES', d, 1) or "").split():
		deps = bb.data.getVar('IMAGE_DEPENDS_%s' % type, d) or ""
		if deps:
			str += " %s" % deps
	return str

DEPENDS += "${@get_image_deps(d)}"

#
# Get a list of files containing device tables to create.
# * IMAGE_DEVICE_TABLE is the old name to an absolute path to a device table file
# * IMAGE_DEVICE_TABLES is a new name for a file, or list of files, seached
#   for in the BBPATH
# If neither are specified then the default name of files/device_table-minimal.txt
# is searched for in the BBPATH (same as the old version.)
#
def get_devtable_list(d):
	import bb
	devtable = bb.data.getVar('IMAGE_DEVICE_TABLE', d, 1)
	if devtable != None:
		return devtable
	str = ""
	devtables = bb.data.getVar('IMAGE_DEVICE_TABLES', d, 1)
	if devtables == None:
		devtables = 'files/device_table-minimal.txt'
	for devtable in devtables.split():
		str += " %s" % bb.which(bb.data.getVar('BBPATH', d, 1), devtable)
	return str

IMAGE_POSTPROCESS_COMMAND ?= ""

fakeroot fakeroot_do_rootfs () {
	do_rootfs
}

do_rootfs () {
	set -x
	rm -rf ${IMAGE_ROOTFS}

	mkdir -p ${IMAGE_ROOTFS}/dev
	for devtable in ${@get_devtable_list(d)}; do
		makedevs -r ${IMAGE_ROOTFS} -D $devtable
	done

	real_do_rootfs

	insert_feed_uris

	rm -f ${IMAGE_ROOTFS}${libdir}/ipkg/lists/oe
	
	${IMAGE_PREPROCESS_COMMAND}
		
	export TOPDIR=${TOPDIR}

	for type in ${IMAGE_FSTYPES}; do
		bbimage -n "${IMAGE_NAME}" -t "$type" -e "${FILE}"
	done

	${IMAGE_POSTPROCESS_COMMAND}
}

insert_feed_uris () {
	
	echo "Building feeds for [${DISTRO}].."
		
	for line in ${FEED_URIS}
	do
		# strip leading and trailing spaces/tabs, then split into name and uri
		line_clean="`echo "$line"|sed 's/^[ \t]*//;s/[ \t]*$//'`"
		feed_name="`echo "$line_clean" | sed -n 's/\(.*\)##\(.*\)/\1/p'`"
		feed_uri="`echo "$line_clean" | sed -n 's/\(.*\)##\(.*\)/\2/p'`"					
		
		echo "Added $feed_name feed with URL $feed_uri"
		
		# insert new feed-sources
		echo "src/gz $feed_name $feed_uri" >> ${IMAGE_ROOTFS}/etc/ipkg/${feed_name}-feed.conf
	done			
}
