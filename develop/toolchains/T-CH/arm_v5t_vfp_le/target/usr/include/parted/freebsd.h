/*
    libparted - a library for manipulating disk partitions
    Copyright (C) 2001 Free Software Foundation, Inc.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
*/

#ifndef PED_FREEBSD_H_INCLUDED
#define PED_FREEBSD_H_INCLUDED

#include <parted/parted.h>
#include <parted/device.h>

#include <sys/param.h>

#if !defined(__FreeBSD_version) && defined(__FreeBSD_kernel_version)
#define __FreeBSD_version __FreeBSD_kernel_version
#endif

#define FREEBSD_SPECIFIC(dev)	((FreeBSDSpecific*) (dev)->arch_specific)

typedef	struct _FreeBSDSpecific	FreeBSDSpecific;

struct _FreeBSDSpecific {
#if (__FreeBSD_version >= 500000) && (__FreeBSD_version < 600000)
	unsigned char channel;
	unsigned char device;
#endif
	int	fd;
};

extern PedArchitecture ped_freebsd_arch;

#endif /* PED_FREEBSD_H_INCLUDED */

