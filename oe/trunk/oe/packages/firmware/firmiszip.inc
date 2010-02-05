DEPENDS += " unzip-native "
python do_unpack() {
	import re

	localdata = bb.data.createCopy(d)
	overrides = bb.data.getVar('OVERRIDES', localdata, 1)
	if not overrides:
		raise bb.build.FuncFailed('OVERRIDES not defined')
	bb.data.setVar('OVERRIDES', overrides+':'+bb.data.getVar('PN', localdata, 1), localdata)

	bb.data.update_data(localdata)

	src_uri = bb.data.getVar('SRC_URI', localdata)
	if not src_uri:
		return
	src_uri = bb.data.expand(src_uri, localdata)

	local = bb.data.expand(bb.fetch.localpath(src_uri, localdata), localdata)
	# dont need any parameters for extraction, strip them off
	local = re.sub(';.*$', '', local)
	bindir = bb.data.getVar('STAGING_BINDIR', localdata, 1)
	cmd = '%s/unzip %s' % (bindir, local)
	if not os.path.exists(bb.data.getVar('S', localdata, 1)):
		os.mkdir(bb.data.getVar('S', localdata, 1))
	os.chdir(bb.data.getVar('S', localdata, 1))
	bb.note("Unpacking %s to %s/" % (local, os.getcwd()))
	ret = os.system(cmd)
}
