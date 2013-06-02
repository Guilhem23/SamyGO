#! /usr/bin/env bash
#find-debuginfo.sh - automagically generate debug info and file list
#for inclusion in an rpm spec file.

RUNPATH=$(dirname $0)

DEBUGEDIT=$RUNPATH/debugedit
if [ ! -e $DEBUGEDIT ]; then
  echo "Unable to find $DEBUGEDIT!" >&2
  exit 1
fi
  
STRIP=$RUNPATH/eu-strip
if [ ! -e $STRIP ]; then
  STRIP=eu-strip
fi

if [ -z "$1" ] ; then BUILDDIR="."
  else BUILDDIR=$1
fi

if [ -z "$2" ]; then CROSSTARGETDIR=""
  else CROSSTARGETDIR=$2
  PATH=$CROSSTARGETDIR/../bin:$PATH
fi
  
if [ -z "$3" ]; then TARGETPREFIX=""
  else TARGETPREFIX=$3
fi

LISTFILE=$BUILDDIR/debugfiles.list
SOURCEFILE=$BUILDDIR/debugsources.list

debugdir="${RPM_BUILD_ROOT}/${CROSSTARGETDIR}/usr/lib/debug"

mkdir -p $debugdir
debugdir=$(cd $debugdir ; pwd)

echo -n > $SOURCEFILE

# Strip ELF binaries
for f in `find $RPM_BUILD_ROOT/${CROSSTARGETDIR} ! -path "${debugdir}/*.debug" -type f \( -perm -0100 -or -perm -0010 -or -perm -0001 \) -exec file {} \; | \
	sed -n -e 's/^\(.*\):[ 	]*.*ELF.*, not stripped/\1/p'`
do
	dn=$(dirname $f | sed -n -e "s#^$RPM_BUILD_ROOT##p" | sed -n -e "s#$CROSSTARGETDIR##p")
	bn=$(basename $f .debug).debug

	debugdn="${debugdir}${dn}"
	debugfn="${debugdn}/${bn}"
	[ -f "${debugfn}" ] && continue

	echo extracting debug info from $f
	$DEBUGEDIT -b "$RPM_BUILD_DIR" -d /usr/src/debug -l "$SOURCEFILE" "$f"

	# A binary already copied into /usr/lib/debug doesn't get stripped,
	# just has its file names collected and adjusted.
	case "$dn" in
	/usr/lib/debug/*) continue ;;
	esac

	mkdir -p "${debugdn}"
	if test -w "$f"; then
		$STRIP -f "${debugfn}" "$f" || :
	else
		chmod u+w "$f"
		$STRIP -f "${debugfn}" "$f" || :
		chmod u-w "$f"
	fi
done

mkdir -p ${RPM_BUILD_ROOT}/${CROSSTARGETDIR}/usr/src/debug
cat $SOURCEFILE | (cd $RPM_BUILD_DIR; LANG=C sort -z -u | cpio -pd0m ${RPM_BUILD_ROOT}/${CROSSTARGETDIR}/usr/src/debug)
# stupid cpio creates new directories in mode 0700, fixup
find ${RPM_BUILD_ROOT}/${CROSSTARGETDIR}/usr/src/debug -type d -print0 | xargs -0 chmod a+rx

find ${RPM_BUILD_ROOT}/${CROSSTARGETDIR}/usr/lib/debug -type f | sed -n -e "s#^$RPM_BUILD_ROOT##p" > $LISTFILE
find ${RPM_BUILD_ROOT}/${CROSSTARGETDIR}/usr/src/debug -mindepth 1 -maxdepth 1 | sed -n -e "s#^$RPM_BUILD_ROOT##p" >> $LISTFILE
