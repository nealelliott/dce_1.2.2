#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: all.sh,v $
# Revision 1.1.10.1  1996/10/02  16:57:42  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:03:39  damon]
#
# Revision 1.1.4.1  1994/06/09  13:49:59  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:23:27  annie]
# 
# Revision 1.1.2.2  1993/01/18  19:50:03  cjd
# 	Embedded copyright notice
# 	[1993/01/18  19:23:40  cjd]
# 
# Revision 1.1  1992/01/19  02:46:02  devrcs
# 	Initial revision
# 
# $EndLog$
#

MACHINE="rios"
TARGET_MACHINE="RIOS"
target_machine="rios"

export MACHINE TARGET_MACHINE target_machine

srcdir=`pwd`
base=`expr "${srcdir}" : "\(.*\)/."`
if [ `basename "${srcdir}"` != "src" ]
then
	echo "Please move source tree into a \"src\" subdirectory"
	exit 1
fi

tools=${base}/tools/${target_machine}/bin


#
#  Contain search paths
#

CCTYPE=host
HOST_CC=/bin/cc

export CCTYPE HOST_CC

SITE="OSF"
OWNER="bin"
GROUP="bin"

export SITE OWNER GROUP

SOURCEBASE=${srcdir}
SOURCEDIR=""
OBJECTDIR=../obj/${target_machine}
EXPORTBASE=${base}/export/${target_machine}
LIBDIRS=-L${EXPORTBASE}/usr/lib

PATH="${EXPORTBASE}/usr/bin:${tools}:/usr/ucb:/bin:/usr/bin"
export PATH

export SOURCEBASE SOURCEDIR OBJECTDIR EXPORTBASE LIBDIRS
INCDIRS="-I${EXPORTBASE}/usr/include -I${EXPORTBASE}/usr/include/dce"
export INCDIRS

LANG=En_US
NLSPATH=${tools}/%N:${NLSPATH}
export LANG NLSPATH

make -cdF build_all 
