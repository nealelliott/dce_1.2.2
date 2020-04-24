# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.18.2  1996/03/09  23:26:11  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:49  marty]
#
# Revision 1.2.18.1  1995/12/08  00:14:21  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/11/23  18:01 UTC  sommerfeld
# 	kernel & misc makefile cleanup
# 	[1994/11/07  21:29 UTC  sommerfeld  /main/sommerfeld_mothmk_1/1]
# 
# Revision 1.2.14.5  1994/09/15  13:36:19  rsarbo
# 	build svc_krpc.o and dcerpcmsg.o into KRPC to restore
# 	minimal printf functionality with new S12Y interfaces
# 	[1994/09/15  13:20:42  rsarbo]
# 
# Revision 1.2.14.4  1994/09/07  15:48:33  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:30  marty]
# 
# Revision 1.2.14.3  1994/05/19  21:14:12  hopkins
# 	Serviceability work
# 	[1994/05/19  02:17:42  hopkins]
# 
# 	Serviceability:
# 	  add SVC_DEFS to CFLAGS.
# 	[1994/05/18  21:33:15  hopkins]
# 
# Revision 1.2.14.2  1994/01/21  22:31:34  cbrooks
# 	Code Cleanup
# 	[1994/01/21  22:29:14  cbrooks]
# 
# Revision 1.2.14.1  1993/12/07  17:54:40  mario
# 	Changed for ode2.3 - < and > becomes this except for RULES_MK
# 	[1993/12/07  17:25:28  mario]
# 
# Revision 1.2.11.1  1993/09/01  14:42:10  delgado
# 	Dont put -UKNCK -DKNCK on the same compile line - it does not work
# 	the way it used to on OSF1 (now works the way it should).
# 	[1993/09/01  14:41:58  delgado]
# 
# Revision 1.2.8.2  1993/06/10  19:26:10  sommerfeld
# 	Initial HPUX RP version.  Don't add OTHERS to EXPLIB_TARGETS
# 	by default.  This may not be correct in the long term.
# 	[1993/06/03  22:35:30  kissel]
# 
# Revision 1.2.4.4  1993/01/03  22:35:14  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:50:48  bbelch]
# 
# Revision 1.2.4.3  1992/12/23  19:37:10  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  14:48:57  zeliff]
# 
# Revision 1.2.4.2  1992/12/03  22:07:19  delgado
# 	Export kprc_helper_mach.h
# 	[1992/12/03  22:06:38  delgado]
# 
# Revision 1.2.2.5  1992/06/26  06:28:12  jim
# 	Added ../runtime/${TARGET_MACHINE} to vpath list.
# 	[1992/06/23  16:01:08  jim]
# 
# Revision 1.2.2.4  1992/05/04  21:00:42  rsalz
# 	Set OBJECT_FORMAT for OSF/1 pmax kernels
# 	[1992/05/04  20:59:12  rsalz]
# 
# Revision 1.2.2.3  1992/05/04  15:23:57  rsalz
# 	Add KERNEL_CC= line so that PMAX/machdep.mk sets right object format.
# 	[1992/05/04  15:23:34  rsalz]
# 
# Revision 1.2.2.2  1992/05/01  17:36:45  rsalz
# 	"Changed pas part of rpc6 drop."
# 	[1992/05/01  17:34:26  rsalz]
# 
# Revision 1.2  1992/01/19  22:13:32  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#  OSF DCE Version 1.0 
#
#  Copyright (c) 1991 by
#      Hewlett-Packard Company, Palo Alto, Ca. & 
#      Digital Equipment Corporation, Maynard, Mass.
#
#  NAME:
#
#      src/rpc/kruntime/Makefile for RPC kernel runtime library
#
#  FACILITY:
#
#      Remote Procedure Call (RPC) 
#
#  ABSTRACT:
#
#  Makefile for RPC kernel runtime library
#
#

VPATH		= ../sys_idl:${TARGET_MACHINE}:${TARGET_OS}_TEMPLATE:../runtime:../runtime/${TARGET_MACHINE}

INCFLAGS	= -I${TARGET_MACHINE} -I${TARGET_OS}_TEMPLATE -I../runtime
IDLINCFLAGS	= -I.			# Needed for ACF files
IDLFLAGS	= -keep c_source -cepv -no_mepv
LIBRARIES	= libknck.a
EXPDIR		= /usr/lib/
EXPLIB_TARGETS  = ${LIBRARIES:S/^/export_/g}
CFLAGS		= -DNCK -DKERNEL -D_KERNEL -DINET ${PROT_DEFS} ${AUTH_DEFS} \
		  ${NAF_DEFS} ${${TARGET_MACHINE}_CFLAGS} \
		  ${${TARGET_MACHINE}_KERNEL_CFLAGS} \
		  ${KERNELCFLAGS} \
		  ${SVC_DEFS}

STUB_CFLAGS  	= -UNCK -DKERNEL -D_KERNEL -DINET ${PROT_DEFS} ${AUTH_DEFS} \
		  ${NAF_DEFS} ${${TARGET_MACHINE}_CFLAGS} \
		  ${${TARGET_MACHINE}_KERNEL_CFLAGS}

conv_cstub.o_CFLAGS  = ${STUB_CFLAGS}
convc_cstub.o_CFLAGS = ${STUB_CFLAGS}
convc_sstub.o_CFLAGS = ${STUB_CFLAGS}
ep_cstub.o_CFLAGS    = ${STUB_CFLAGS}
mgmt_cstub.o_CFLAGS  = ${STUB_CFLAGS}
mgmt_sstub.o_CFLAGS  = ${STUB_CFLAGS}

# Include files to build and export, and their locations
INCLUDES	= exc_handling.h	  \
		  exc_handling_ids_krpc.h \
		  idl_ss_krpc.h		  \
		  krpc_helper.h		  \
		  krpc_helper_mach.h	  \
		  pthread.h		  \
		  pthread_exc.h

exc_handling.h_EXPDIR		= /usr/include/dce/ker/
exc_handling_ids_krpc.h_EXPDIR	= /usr/include/dce/ker/
idl_ss_krpc.h_EXPDIR		= /usr/include/dce/ker/
krpc_helper.h_EXPDIR		= /usr/include/dce/ker/
krpc_helper_mach.h_EXPDIR	= /usr/include/dce/ker/
pthread.h_EXPDIR		= /usr/include/dce/ker/
pthread_exc.h_EXPDIR		= /usr/include/dce/ker/

#
# serviceability remnants reqd in kernel
#
OBJS_SVC	= dcerpcmsg.o
OBJS_COM	= comauth.o combind.o comcall.o comcthd.o \
		  comif.o cominit.o cominit_ux.o comnaf.o comnet.o comobj.o \
		  comp.o comutil.o uuid.o
OBJS_COMMON     = rpctimer.o rpcclock.o rpcdbg.o rpcmutex.o rpclist.o \
		  rpcmem.o
OBJS_DG	 	= conv.o dg.o dgcall.o dgccall.o dgccallt.o dgcct.o \
		  dgclive.o dgclsn.o dgexec.o dgfwd.o dgglob.o dghnd.o \
		  dginit.o dglossy.o dglsn.o dgpkt.o dgrq.o dgscall.o  \
		  dgsct.o dgslive.o dgslsn.o dgsoc.o dgutl.o dgxq.o    \
		  ep_cstub.o conv_cstub.o convc_cstub.o convc_sstub.o
OBJS_IP	 	= ipnaf.o
OBJS_MGMT       = mgmt.o mgmt_cstub.o mgmt_sstub.o
OBJS_NDR	= ndrglob.o
OBJS_AUTH	= md5_krpc.o  sec_pkl_krpc.o pkl_krpc.o	  pac_util_krpc.o \
		  krbclt.o    krbcom.o    krbcrc.o    krbdgclt.o  krbdgcom.o \
                  krbdgsrv.o  krbreq.o    

#Kernel RPC specific modules
OBJS_KRPC       = comnlsn_krpc.o dce_error_krpc.o subr_krpc.o idl_ss_krpc.o \
                  sec_authn_krpc.o krpc_helper.o svc_krpc.o

#Operating-system-specific kernel RPC modules
OBJS_SYS	= comsoc_sys.o ipnaf_sys.o pthread_sys.o uuidsys.o \
		  ${${TARGET_MACHINE}_OBJS_SYS}

libknck.a_OFILES= $(OBJS_SVC) $(OBJS_COM) $(OBJS_COMMON) $(OBJS_DCE) \
		  $(OBJS_DG) $(OBJS_IP) $(OBJS_MGMT) $(OBJS_NDR) $(OBJS_AUTH) \
		  $(OBJS_KRPC) $(OBJS_SYS) ${${TARGET_OS}_OBJS} \
		  ${${TARGET_MACHINE}_OBJS}

AUTH_DEFS	?= -DAUTH_KRB -DAUTH_KRB_DG
PROT_DEFS	?= -DPROT_NCADG
NAF_DEFS	?= -DNAF_IP
SVC_DEFS	?= -DDCE_RPC_SVC

ipnaf_sys.o_CFLAGS	= ${CFLAGS} ${${TARGET_MACHINE}_ipnaf_sys}
krbreq.o_CFLAGS		= ${CFLAGS} ${${TARGET_MACHINE}_krbreq}


IDLFILES = conv.idl convc.idl ep.idl mgmt.idl


.if exists (${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>
