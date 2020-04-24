# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:14:37  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:58  marty]
#
# Revision 1.1.4.1  1995/12/08  16:13:13  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:19:40  root]
# 
# Revision 1.1.2.4  1994/06/21  14:47:52  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:43:36  marrek]
# 
# Revision 1.1.2.3  1994/06/10  21:05:50  devsrc
# 	cr10871 - fix copyright
# 	[1994/06/10  14:48:59  devsrc]
# 
# Revision 1.1.2.2  1994/05/10  15:58:59  marrek
# 	April 1994 code submission.
# 	[1994/04/27  12:50:33  marrek]
# 
# Revision 1.1.2.1  1994/03/23  15:32:26  keutel
# 	creation
# 	[1994/03/21  13:25:58  keutel]
# 
# $EndLog$
#
#
#
# @SNI_COPYRIGHT@
#
# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:14:37 $
#

LIBRARIES		= libcomstep.a librtrosasn1.a libcom.a

.ifdef (${TARGET_MACHINE} == "HP800")
VPATH			= ../roacpm:../roasn1:../rodpat:../rolctr:../roprpm:../rostep:../rotime:${TARGET_MACHINE}:../routil
CFLAGS			+= -DRP0003_HP ${${TARGET_MACHINE}_CFLAGS}
.else
VPATH			= ../roacpm:../roasn1:../rodpat:../rolctr:../roprpm:../rostep:../rotime:../routil
CFLAGS			+= -DRC0008_LOG_V1 ${${TARGET_MACHINE}_CFLAGS}
.endif

libcomstep.a_EXPDIR	= /usr/lib/
librtrosasn1.a_EXPDIR	= /usr/lib/
libcom.a_EXPDIR		= /usr/lib/

EXPLIB_TARGETS  	= ${LIBRARIES:S/^/export_/g}

INCFLAGS		= -I../roincl   -I../../incl  -I../../../h

libcomstep.a_OFILES = \
        spacpm1.o  spacpm2.o  spacpmze.o spacpmzy.o spacpmzz.o routil31.o \
	spstep1.o  spstep3.o  spstep4.o  spstep5.o  routil0.o  routil4.o  \
	routil5.o  routil9.o  routil8.o  routil62.o routila.o  spprpm0.o  \
	spprpm1.o  spprpm2.o  spprpm3.o  spprpm4.o  spprpm5.o  splctr0.o  \
	sptime0.o  sputil0.o  spppmzx.o  spppmzy.o  spppmze.o  spppmzz.o  \
	rostor0.o  routil2.o  roppm02.o  spdpat0.o  spacpm0.o

librtrosasn1.a_OFILES = \
	stdeflen.o stdump.o   stend.o    sterror.o  stextern.o stintcod.o \
	stintdec.o stlencod.o stlendec.o stlength.o stnumber.o stcmp.o    \
	stoctmov.o stoctpak.o stoctscd.o stoidint.o stpres.o   stskip.o   \
	stskptyp.o sttypcod.o stanydef.o stbinmov.o stbinpak.o stbitcod.o \
	stbitscd.o stcopy.o

libcom.a_OFILES = \
	roacpm0.o  roacpm1.o  roacpmzy.o roacpmzz.o roacpmze.o roppm00.o \
	roppm03.o  roppm04.o  roppm05.o  roppm06.o  roppmzx.o  roppmzy.o \
	roppmzz.o  rotime0.o  rodpat1.o  roppm01.o  roppm02.o  roppmze.o \
	romoni00.o rostor0.o  rotime0.o  routil0.o  routil2.o  routil31.o \
	routil4.o  routil5.o  routil62.o rolctr0.o  routil8.o  routil9.o \
	routila.o  routilt.o 

.include <${RULES_MK}>
