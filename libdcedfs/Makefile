# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.16.2  1996/03/11  13:25:37  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:13:46  marty]
#
# Revision 1.1.16.1  1995/12/08  17:53:11  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:27:32  root]
# 
# Revision 1.1.12.2  1994/02/04  20:38:11  devsrc
# 	Merged with changes from 1.1.12.1
# 	[1994/02/02  20:40:16  devsrc]
# 
# 	Add libtpq.a to LIBS list
# 	[1993/12/09  21:35:54  jaffe]
# 
# Revision 1.1.12.1  1993/12/08  20:15:00  devobj
# 	Changed in order to build with ODE2.3 make
# 	[1993/12/08  20:14:41  devobj]
# 
# Revision 1.1.10.2  1993/10/21  20:47:17  rsarbo
# 	remove libdfsjunct.a since the i/f's are now contained in
# 	libncompat.a
# 	[1993/10/21  16:40:24  rsarbo]
# 
# Revision 1.1.10.1  1993/10/19  22:12:12  rsarbo
# 	Some of libncompat.a was split off into libdfsjunct.a.  Include
# 	that in libdcedfs as well as libncompat.a to prevent losing
# 	i/f's in libdcedfs.
# 	[1993/10/19  21:28:18  rsarbo]
# 
# Revision 1.1.8.2  1993/05/12  16:02:36  jaffe
# 	Add bomb library
# 	[1993/05/12  15:38:42  jaffe]
# 
# Revision 1.1.6.5  1993/04/02  21:41:45  rsarbo
# 	add libicl.a to link list
# 	[1993/04/02  17:45:11  rsarbo]
# 
# Revision 1.1.6.4  1993/01/14  23:21:32  rsarbo
# 	add libdfsncs.a to LIBS
# 	[1993/01/14  23:19:42  rsarbo]
# 
# Revision 1.1.6.3  1992/12/30  17:25:54  zeliff
# 	Embedding copyright notice
# 	[1992/12/30  17:15:22  zeliff]
# 
# Revision 1.1.2.2  1992/01/22  23:03:53  melman
# 	Adding changes from defect_fixes archive.
# 	[1992/01/22  22:20:28  melman]
# 
# $EndLog$
#
# libdcedfs Makefile
#

VPATH	= ../../export/${target_machine}/usr/lib:../../link/export/${target_machine}/usr/lib:${TARGET_MACHINE}

# Files to export
EXPLIB_TARGETS  = export_libdcedfs.a


LIBS	= ${libvolc.a:P} ${libfldb.a:P} ${libftserver.a:P} ${libbos.a:P} \
	  ${libafs4int.a:P} ${libcommondata.a:P} ${libncompat.a:P} \
	  ${libnubik.a:P} ${libdauth.a:P} ${libdacl.a:P} ${libdacllfs.a:P} \
	  ${librep.a:P} ${libcmd.a:P} ${libcom_err.a:P} ${libbomb.a:P} \
	  ${libafsutil.a:P} ${libafssys.a:P} ${libosi.a:P} ${libdfsncs.a:P} \
	  ${libicl.a:P} ${libtpq.a:P}

	
ILIST	=	libdcedfs.a
IDIR	=	/usr/lib/

# Directory in which to place exported files
EXPDIR = /usr/lib/

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
