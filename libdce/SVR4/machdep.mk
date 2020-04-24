# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
# All Rights Reserved
#
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.86.2  1996/03/11  13:25:36  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:13:45  marty]
#
# Revision 1.1.86.1  1995/12/08  17:53:05  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:27:29  root]
# 
# Revision 1.1.84.10  1994/07/06  15:09:31  marrek
# 	July 1994 code submission.
# 	[1994/07/04  13:26:16  marrek]
# 
# Revision 1.1.84.9  1994/06/21  14:41:00  marrek
# 	Remove _r libraries in June 1994 code submission.
# 	[1994/06/09  12:16:27  marrek]
# 
# Revision 1.1.84.8  1994/05/27  16:21:03  keutel
# 	OT 10782: add SNI Munich specific stuff
# 	[1994/05/26  10:35:36  keutel]
# 
# Revision 1.1.84.7  1994/05/10  16:04:42  marrek
# 	libslog removed in April 1994 submission.
# 	[1994/04/27  14:33:49  marrek]
# 
# Revision 1.1.84.6  1994/04/18  00:14:51  pwang
# 	Loaded libdced.a into libdce OT#10158
# 	[1994/04/17  21:57:45  pwang]
# 
# Revision 1.1.84.5  1994/03/23  15:50:39  keutel
# 	GDS changes
# 	[1994/03/23  14:15:53  keutel]
# 
# Revision 1.1.84.2  1994/02/02  22:05:52  ohara
# 	add libacldb.a libdceutils.a libdb44.a
# 	[1994/01/27  20:10:56  ohara]
# 
# Revision 1.1.84.1  1994/01/04  19:41:41  ohara
# 	add libsvc.a
# 	[1994/01/04  18:02:36  ohara]
# 
# Revision 1.1.82.2  1993/10/20  14:47:32  ohara
# 	use libxds not libxds_help
# 	[1993/10/17  20:10:22  ohara]
# 
# Revision 1.1.82.1  1993/10/15  21:19:07  ohara
# 	drop the _dce
# 	[1993/10/15  16:31:07  ohara]
# 
# Revision 1.1.6.2  1993/06/24  20:08:57  hinman
# 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
# 	[1993/06/17  14:37:41  hinman]
# 
# Revision 1.1.3.4  1993/02/01  20:27:19  hinman
# 	[hinman@sni] - Final merge before bsubmit
# 	[1993/01/31  15:59:13  hinman]
# 
# Revision 1.1.3.3  1992/12/30  17:17:08  zeliff
# 	Embedding copyright notice
# 	[1992/12/30  17:15:17  zeliff]
# 	Revision 1.1.4.2  1993/01/11  16:10:31  hinman
# 	[hinman] - Check in merged SNI version
# 
# Revision 9.1.3.6  92/11/18  11:46:15  steveg
# 	Add libdce.a back into build procedure
# 
# Revision 9.1.2.3  92/11/18  11:41:27  steveg
# 	Add libdce.a back in as part of the build
# 
# Revision 9.1.3.5  92/10/01  15:57:23  root
# 	Acceptance of OSF rev 1.1.3.2
# 
# Revision 1.1.3.2  1992/09/29  21:12:26  devsrc
# 	New file for SVR4 OSF merge
# 
# 	Submitted by Steve Gilbane
# 	[1992/09/11  15:41:07  rec]
# 
# $EndLog$
# 
# DCE 
# libdce Makefile
#
# This makefile builds libdce.so out of the LIBDCE_LIBS libraries 
# found in the export directory.  
#

#
# Define NO_BUILD_STATIC_LIBDCE to prevent building libdce.a
#
.if defined(NO_BUILD_STATIC_LIBDCE)
AR2		= /bin/echo > /dev/null
.else
ST_LIBDCE	= libdce.a
EXP_ST_LIBDCE	= export_libdce.a
AR2		= ${_AR_}
.endif

EXPLIB_TARGETS	= export_libdce.so ${EXP_ST_LIBDCE}
EXPDIR		= /usr/lib/
IDIR		= /usr/lib/
ILIST		= libdce.so ${ST_LIBDCE}
PARTIAL_LDFLAGS = -r
SHCCFLAGS	= -G
LIBDCE_SOURCES_DEFINED	= true

DCE_EXPORTBASE = ${EXPORTBASE}

# Now let's include the XDS libraries into libdce
LIBGDS_LIBS 	= libXDS.a libXOM.a libdua.a libd23.a libd27.a \
		  libosiaddr.a libxoms.a libd21sw.a libdua_sec.a
LIBDCE_LIBS	= libcma.a libnck.a libidl.a libsvc.a  libdced.a libcds.a \
		  libasn1.a libsec.a libkrb5.a \
		  libutc-${DTS_LIBUTC_MODE}.a libdce_conf.a \
		  libacldb.a libdceutils.a libdb44.a \
		  libsvr4.a

ALLLIBS_DCE	= ${LIBDCE_LIBS:S;^;${DCE_EXPORTBASE}/usr/lib/;g}
ALLLIBS_GDS	= ${LIBGDS_LIBS:S;^;${EXPORTBASE}/usr/lib/;g}
DEP_LIBS	= -lm_r ${SNILIBS} -lsocket_r -lnsl_r_i -lmalloc_r -lresolv_r -lucb_r -lc_r

libdce.a: libdce.so
	strip -x libdce.a
	ar -ts libdce.a

libdce.so: ${ALLLIBS_DCE} ${ALLLIBS_GDS}
	${RM} -f libdce.a; \
	cnt=1 ; \
	for x in ${ALLLIBS_DCE} ${ALLLIBS_GDS}; \
	do \
		echo Extracting $$x files; \
		f=`${_AR_} -t $$x`; \
		${_AR_} -x $$x; \
		${AR2} -qc libdce.a $$f; \
		${_LD_} ${PARTIAL_LDFLAGS} -o allofiles$${cnt}.o $$f; \
		${RM} -f $$f; \
		cnt=`expr $${cnt} + 1`; \
	done; 
	${_CC_} ${SHCCFLAGS} -o libdce.so allofiles*.o;

GARBAGE		= allofiles*.o
	
.include <${RULES_MK}>
