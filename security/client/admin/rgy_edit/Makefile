# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.6.2  1996/03/11  13:26:32  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:14:32  marty]
#
# Revision 1.2.6.1  1995/12/08  17:57:44  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/11/15  21:21 UTC  mullan_s
# 	Merge.
# 	[1995/12/08  16:29:21  root]
# 
# Revision 1.2.4.4  1994/09/09  19:11:44  mob
# 	Removing the temporary, incomplete ERA support from rgy_edit (OT 11652,11262)
# 	[1994/09/08  23:52:14  mob]
# 
# Revision 1.2.4.3  1994/07/05  20:43:20  sommerfeld
# 	Don't include the rgy_edit_LD_OPT_LEVEL line
# 	[1994/07/05  20:42:53  sommerfeld]
# 
# Revision 1.2.4.2  1994/03/25  15:19:30  sekhar
# 	Merged with changes from 1.2.4.1
# 	[1994/03/25  15:18:56  sekhar]
# 
# 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
# 	[1994/03/25  15:16:29  sekhar]
# 
# Revision 1.2.4.1  1994/03/02  17:54:00  hanfei
# 	Initial ERA drop
# 	[1994/02/24  21:06:02  hanfei]
# 
# Revision 1.2.4.1  1994/03/02  17:54:00  hanfei
# 	Initial ERA drop
# 	[1994/02/24  21:06:02  hanfei]
# 
# Revision 1.2.2.2  1992/12/29  12:38:36  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:11:50  zeliff]
# 
# Revision 1.2  1992/01/19  22:13:42  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#  OSF DCE Version 1.0 

#   Makefile.ODE V=6 11/01/91 //littl/prgy/src/edrgy
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# /client/admin/rgy_edit Makefile
#

VPATH	= ../../rca:../../../h

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../../../h -I../../../krb5/include -I${EXPORTBASE}/usr/include/krb5

# Programs to build
PROGRAMS = rgy_edit 

# Files to install
ILIST = rgy_edit 

# Directory in which to install the files
IDIR = /bin/

rgy_edit_OFILES =	\
			eif.o        \
			eif_acct.o   \
			eif_def.o    \
			eif_pgo.o    \
			eif_prop.o   \
			eif_site.o   \
			tty.o        \
			tty_acct.o   \
			tty_conf.o   \
			tty_def.o    \
			tty_io.o     \
			tty_keytab.o \
			tty_pgo.o    \
			tty_prop.o   \
			tty_util.o   \
		 	ek_db.o    \
			ek_site.o  \
			ek_util.o  \
			form.o     \
			dcesadmsg.o

# Libraries the programs need to link with
LDIRS = -L. -L../../../utils
LIBS =  -ldce
	
rgy_edit_LIBS = ${LDIRS} -lsecutil ${LIBS} ${${TARGET_MACHINE}_LIBS} 


.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
