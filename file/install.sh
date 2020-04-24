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
# $Log: install.sh,v $
# Revision 1.1.9.1  1996/10/02  17:52:33  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:41:06  damon]
#
# Revision 1.1.4.1  1994/06/09  14:11:48  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:25:24  annie]
# 
# Revision 1.1.2.2  1993/01/19  16:05:30  cjd
# 	embedded copyright notice
# 	[1993/01/19  14:15:37  cjd]
# 
# Revision 1.1  1992/01/19  02:45:59  devrcs
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

PATH="${tools}:/usr/ucb:/bin:/usr/bin"
export PATH

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
TOSTAGE=${base}/install/${target_machine}
USER=`whoami`

export SOURCEBASE SOURCEDIR OBJECTDIR EXPORTBASE LIBDIRS TOSTAGE USER
INCDIRS="-I${EXPORTBASE}/usr/include"
export INCDIRS

#Build install directory structure
if [ -d ${base}/install ]
then
	true
else
	mkdir ${base}/install
fi
if [ -d ${TOSTAGE} ]
then
	true
else
	mkdir ${TOSTAGE}
fi

make -cF install_all 
