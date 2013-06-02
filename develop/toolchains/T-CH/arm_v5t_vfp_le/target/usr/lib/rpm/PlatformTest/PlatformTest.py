#!/usr/bin/env python

import sys, os, commands, string, re, tempfile
from getopt import getopt

def printhelp():
	print """
Usage: %s (-c <cc> | --cc=<cc>)
          (-o <objdump> | --objdump=<objdump>)
          (-t <testfile> | --testfile=<testfile>

If cc and objdump are not supplied, an attempt will be made to determine
the appropriate values based on the environment. If this is not
possible, warnings will be issued.
""" % sys.argv[0]

## command line parsing

try:
	opts, args = getopt(sys.argv[1:], 'hc:o:t:', ['cc=', 'objdump=', 'testfile=', 'help'])
except getopt.GetoptError:
	printhelp()
	sys.exit(2)

cc = None
objdump = None
testfile = '/opt/montavista/config/PlatformTest/tests.c'

for o, a in opts:
	if o in ('-h', '--help'):	printhelp(); sys.exit(0)
	elif o in ('-c', '--cc'):	cc = a
	elif o in ('-o', '--objdump'):	objdump = a
	elif o in ('-t', '--testfile'):	testfile = a

if cc == None:
	if os.environ.has_key('CC'):
		cc = os.environ['CC']
	else:
		sys.stderr.write('WARNING: CC not specified on command line or environment; using default\n')
		cc = 'gcc'

if objdump == None:
	if os.environ.has_key('OBJDUMP'):
		objdump = os.environ['OBJDUMP']
	else:
		sys.stderr.write('WARNING: OBJDUMP not specified on command line or equivalent; using default\n')
		objdump = 'objdump'

## use mktemp to get a filename; gcc output to it
tfname = tempfile.mktemp('_PlatformTest.o')

s, o = commands.getstatusoutput(string.join((cc, '-c', testfile, '-o', tfname)))
if s != 0: raise RuntimeError, '%s returned error code %s' % (cc, s)

s, objdump_output = commands.getstatusoutput(string.join((objdump, '-s', tfname)))
if s != 0:
	os.remove(tfname)
	raise RuntimeError, '%s returned error code %s' % (objdump, s)

os.remove(tfname)

section_re = re.compile('^Contents of section (.*):$')
content_re = re.compile('^ [0-9a-f]{4} ([0-9a-f ]{35})  \S')

content = {}
curr_section = None
lines = string.split(objdump_output, "\012")
for LineNum in range(len(lines)):
	ALine = lines[LineNum]
	section_match = section_re.match(ALine)
	if section_match != None:
		curr_section = section_match.group(1)
		assert not content.has_key('curr_section'), 'two headers found for the same section'
		content[curr_section] = {}
		content[curr_section]['raw_hex'] = ''
		continue
	content_match = content_re.match(ALine)
	if content_match != None:
		assert curr_section != None, 'found content before section header'
		content[curr_section]['raw_hex'] = content[curr_section]['raw_hex'] + string.replace(content_match.group(1), ' ', '')

bigendian = None
if(content.has_key('PT_ENDIANNESS')):
	if content['PT_ENDIANNESS']['raw_hex'] == '00':
		bigendian = 0
	elif content['PT_ENDIANNESS']['raw_hex'] == '01':
		bigendian = 1

for AnItem in content.keys():
	rawhex = content[AnItem]['raw_hex']
	if(len(rawhex) % 2 == 1): raise RuntimeError, 'bad assumption that len will always be a power of two'
	content[AnItem]['raw_text'] = ''
	for AVal in range(0, len(rawhex), 2):
		content[AnItem]['raw_text'] = content[AnItem]['raw_text'] + chr(eval('0x%s' % content[AnItem]['raw_hex'][AVal:AVal+2]))
	if bigendian == None: continue ## insufficient info to do numeric conversions
	if len(content[AnItem]['raw_text']) in (1, 2, 4, 8):
		## do a uint conversion
		uintval = long(0)
		if bigendian == 0: ttcrange = range(len(content[AnItem]['raw_text']) - 1, -1, -1)
		else: ttcrange = range(len(content[AnItem]['raw_text']))
		for AVal in ttcrange:
			uintval = uintval * 256
			uintval = uintval + ord(content[AnItem]['raw_text'][AVal])
		content[AnItem]['uintval'] = uintval

def_re = re.compile('DEF_([^_]+)_(\S+)')
string_re = re.compile('DEF_STR_(\S+)')
numval_re = re.compile('DEF_(UCHAR|UINT)_(\S+)')

for AKey in content.keys():
	kmatch = def_re.match(AKey)
	if kmatch != None:
		if kmatch.group(1) in ('UCHAR', 'UINT'):
			kval = content[AKey]['uintval']
			try:
				print 'export %s=%d' % (kmatch.group(2), kval)
			except TypeError:
				print 'export %s=%s' % (kmatch.group(2), kval)
		elif kmatch.group(1) == 'STR':
			if (content[AKey].has_key('uintval')):
				strstart = int(content[AKey]['uintval'])
				if strstart == 0:
					print 'export %s=no' % (kmatch.group(2))
				else:
					print 'export %s=yes' % (kmatch.group(2))
