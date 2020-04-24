# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.9.1  1996/10/17  18:18:43  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:50:12  damon]
#
# Revision 1.1.4.2  1994/08/10  19:22:58  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:17:15  annie]
# 
# Revision 1.1.4.1  1994/07/13  22:32:58  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:17:15  devsrc]
# 
# 	Delegation Functional Tests - Initial Submit
# 	[1994/06/08  18:30:18  delgado]
# 
# $EndLog$



IDLINCFLAGS = -I.
IDLFLAGS = -keep c_source

CFLAGS = ${USERCFLAGS}

#IDLOBJ = ${IDLFILES:.idl=.h}

IDLFILES=                dfsdlg_target.idl dfsdlg_int.idl

INCLUDES=                dfsdlg_target.h dfsdlg_int.h

ILIST=                   dfsdlg_client dfsdlg_int dfsdlg_target delegation.sh 

PROGRAMS =               dfsdlg_client dfsdlg_int dfsdlg_target

.if ${TARGET_OS} == "AIX"
LIBS = -ldauth -ldacl -ldacllfs -lncompat -licl -losi -ldce -lcom_err ${${TARGET_MACHINE}_LIBS}
.else
LIBS =    -ldauth -ldacl -lncompat -losi -ldce ${${TARGET_MACHINE}_LIBS}
.endif

dfsdlg_client_OFILES  =  dlg_client.o dfsdlg_int_cstub.o dlg_utils.o dfsdlg_target_cstub.o

dfsdlg_int_OFILES =      dlg_int.o dlg_utils.o dfsdlg_int_sstub.o dfsdlg_target_cstub.o

dfsdlg_target_OFILES =   dfsdlg_target_sstub.o dlg_target.o dlg_utils.o 



.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif


.include <${RULES_MK}>


