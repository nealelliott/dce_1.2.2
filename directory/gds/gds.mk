#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: gds.mk,v $
# Revision 1.2.15.2  1996/03/09  23:13:45  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:04  marty]
#
# Revision 1.2.15.1  1995/12/08  15:55:41  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/08/29  18:37 UTC  mothra
# 	Fix OSF mistake with CC_OPT_LEVEL: should use CFLAGS
# 	[1995/12/08  15:11:20  root]
# 
# Revision 1.2.13.3  1994/03/23  15:14:44  keutel
# 	Merged with changes from 1.2.13.2
# 	[1994/03/23  15:13:07  keutel]
# 
# 	March 1994 code drop
# 	[1994/03/21  16:00:51  keutel]
# 
# Revision 1.2.13.2  1994/03/04  09:32:47  marrek
# 	Remove SVR4 specific part.
# 	[1994/03/03  09:53:09  marrek]
# 
# Revision 1.2.13.1  1994/02/22  18:46:44  marrek
# 	removed RIOS_LIBS for gds/stub
# 	[1994/02/21  15:17:48  marrek]
# 
# 	February 1994 code drop
# 	[1994/02/17  16:22:31  marrek]
# 
# 	Change OPT_LEVEL to CC_OPT_LEVEL.
# 	[1994/02/10  17:47:31  marrek]
# 
# 	Add OPT_LEVEL.
# 	[1994/02/10  17:22:01  marrek]
# 
# 	Update.
# 	[1994/02/10  17:18:39  marrek]
# 
# 	Code drop for dce1.1.
# 	[1994/02/08  08:50:12  marrek]
# 
# Revision 1.2.11.5  1993/10/20  14:47:30  ohara
# 	svr4 changes
# 	[1993/10/20  02:20:17  ohara]
# 
# Revision 1.2.11.4  1993/10/14  17:27:37  keutel
# 	October 1993 code drop
# 	[1993/10/14  17:26:08  keutel]
# 
# Revision 1.2.11.3  1993/10/13  17:32:19  keutel
# 	Merged with changes from 1.2.11.2
# 	[1993/10/13  17:26:28  keutel]
# 
# 	October 1993 code drop
# 	[1993/10/09  18:45:48  keutel]
# 
# Revision 1.2.11.2  1993/10/05  16:48:09  zeliff
# 	Defect 8954: removed RIOS_LIBS and HP800_LIBS from delupd stanza
# 	[1993/10/05  16:24:15  zeliff]
# 
# 	Defect 8955: removed RIOS_LIBS, HP800_LIBS from dsa stanza
# 	[1993/10/05  16:17:21  zeliff]
# 
# Revision 1.2.11.1  1993/10/04  19:05:42  keutel
# 	HP800_LIBS : libslog.a
# 	[1993/10/04  19:05:22  keutel]
# 
# Revision 1.2.8.2  1993/08/10  11:10:02  marrek
# 	adm_iapl change
# 	[1993/08/02  08:12:41  marrek]
# 
# 	deleted all stuff for dua: Jochen Keutel
# 	[1993/08/02  08:10:43  marrek]
# 
# Revision 1.2.5.3  1992/12/31  20:37:39  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:28:00  bbelch]
# 
# Revision 1.2.5.2  1992/09/29  21:11:24  devsrc
# 	[hinman@sni-usa.com] - No longer redefine rindex for RIOS in MAVROS
# 	and MAVCOD
# 	[1992/09/24  19:49:50  weisman]
# 
# 	[hinman@sni] - Check in merges from SNI
# 	[1992/09/14  20:25:59  treff]
# 
# Revision 9.5.1.6  92/09/09  12:01:39  hinman
# 	[hinman] - Add copyright message
# 
# Revision 9.5.2.2  92/09/09  10:45:49  hinman
# 	[hinman] - Add copyright text
# 
# Revision 9.5.1.5  92/09/08  11:44:01  hinman
# 	[hinman] - Changes for new structure of libXDS: put a cds-aware version
# 	into libdce, and make the freestanding one not cds-aware.
# 
# Revision 9.5.2.2  92/09/08  11:00:25  hinman
# 	[hinman] - Build a cds-aware, non-linking version of libdua
# 
# Revision 9.5.1.4  92/08/31  11:48:35  hinman
# 	[hinman] - Fold forward from hinman_osf_mergesb
# 
# Revision 9.5.2.2  92/08/31  11:35:36  hinman
# 	[hinman] - put dlopen() on a separate switch
# 
# Revision 9.5.1.3  92/08/31  10:35:42  hinman
# 	[hinman] - Define SNI_DLOPEN for dua
# 
# Revision 9.5.2.2  92/08/31  10:33:16  hinman
# 	[hinman] - Define SNI_DLOPEN for dua
# 
# Revision 9.5.1.2  92/08/21  15:49:50  hinman
# 	[hinman for greg] - Fold backward from DCE1TOP
# 
# Revision 8.1.1.17  92/08/21  13:46:30  hinman
# 	[hinman] - Fold forward from hinman_dce1sb
# 
# Revision 8.1.9.2  92/08/14  16:04:42  hinman
# 	[hinman] - Merged version\; compiles but does not work
# 
# Revision 9.5.1.1  92/07/24  10:19:18  hinman
# 	Create merge branch
# 
# Revision 9.5  92/07/24  10:19:15  hinman
# 	Update checkin
# 
# Revision 1.2.2.3  1992/06/02  03:42:19  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  21:40:25  zeliff]
# 
# Revision 1.2.2.2  1992/01/22  22:33:29  jim
# 	Changes needed for AIX platform.  libshth and libslog are needed to be
# 	linked because these symbols are not exported from libdce.a on AIX.
# 	[1992/01/22  22:17:40  jim]
# 
# Revision 1.2  1992/01/19  22:13:14  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#

.if !defined(_GDS_MK_)
_GDS_MK_=

############################################################################
# Definitions common to all GDS subcomponents.
############################################################################

RIOS_CFLAGS_COMMON      ?= -D_ALL_SOURCE

###########################################################################
# Definitions specific to GDS subdirectories.
###########################################################################

.if ${MAKESUB}=="directory/gds/"
.if ${TARGET_MACHINE} == "SVR4"
.ifdef SNI_CROSS_BUILD
# Only do libraries from dsa and osiadr, not executables
EXPLIB_SUBDIRS	= cacheadm ASN1 asn1 adm cache cisam dsa dua ipc osiadr oss rtros stub util gda xti 
SUBDIRS	= cacheadm asn1 adm bin cache conf ipc  msg ${TARGET_MACHINE} oss rtros stub gda 
.endif
.endif
.endif

.if ${MAKESUB}=="directory/gds/ASN1/lib_r/"
#
# This conditional is here to handle (temporarily) the non-idempotent
# timeb.h on ultrix.
#
.if ${TARGET_MACHINE} == "MIPS"
INCFLAGS        = -I${TARGET_MACHINE}
.endif
RIOS_CFLAGS	?= -Dunix ${RIOS_CFLAGS_COMMON}
.endif

.if ${MAKESUB}=="directory/gds/stub/"
RIOS_LDFLAGS	?= -bnodelcsect
.endif


############################################################################
# Defaults for standard target-specific definitions (in case they weren't
# defined above).
############################################################################

RIOS_CFLAGS     ?= ${RIOS_CFLAGS_COMMON}


############################################################################
# Defaults for standard definitions.  These may be overriden in some GDS
# subcomponent makefiles.
############################################################################

CFLAGS  ?= ${${TARGET_MACHINE}_CFLAGS}

CFLAGS += -DTHREADSAFE 

YFLAGS  ?= ${${TARGET_MACHINE}_YFLAGS}
LIBS ?= ${${TARGET_MACHINE}_LIBS:U-ldce}
.endif
