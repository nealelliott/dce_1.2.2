#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.8.2  1996/03/09  23:26:06  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:44  marty]
#
# Revision 1.1.8.1  1995/12/08  00:14:03  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:23 UTC  psn
# 	Enable rules for building dce_krpc.ext
# 
# 	HP revision /main/HPDCE02/6  1995/09/14  19:57 UTC  maunsell_c
# 	merge maunsell_WC_port_fixes to HPDCE02 mainline
# 	adds krpc_osi_mach.h to build
# 
# 	HP revision /main/HPDCE02/maunsell_WC_port_fixes/2  1995/09/14  18:58 UTC  maunsell_c
# 	Walnut Creek port - add krpc_osi_mach.h
# 
# 	HP revision /main/HPDCE02/5  1995/09/08  22:01 UTC  sommerfeld
# 	Correct one typo in prev. fix.
# 	[1995/09/08  21:31 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/2]
# 
# 	CHFts16270: Hide encryption entrypoints (sigh).
# 	[1995/09/08  17:39 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
# 
# 	HP revision /main/HPDCE02/maunsell_WC_port_fixes/1  1995/09/14  17:51 UTC  maunsell_c
# 	add krpc_osi_mach.h
# 
# 	HP revision /main/HPDCE02/4  1995/01/30  22:40 UTC  sommerfeld
# 	avoid version.o.
# 	use default rules in hp.kext.mk instead of spelling it out here.
# 	[1995/01/27  04:23 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
# 
# 	HP revision /main/HPDCE02/3  1994/09/21  14:50 UTC  tatsu_s
# 	Merged mothra up to dce1_1_b18.
# 	[1995/12/07  23:55:43  root]
# 
# Revision 1.1.6.3  1994/09/15  13:36:17  rsarbo
# 	build stubbed S12Y into KRPC kernel extension to
# 	restore printf functionality
# 	[1994/09/15  13:22:55  rsarbo]
# 
# 	add pickling stuff to dce_krpc.ext
# 
# 	HP revision /main/HPDCE02/2  1994/08/17  20:41 UTC  tatsu_s
# 	Merged from Hpdce02_01
# 
# Revision 1.1.6.2  1994/08/09  17:32:31  burati
# 	DFS/EPAC/KRPC/dfsbind changes
# 	[1994/08/09  17:23:45  burati]
# 
# Revision 1.1.6.1  1994/06/10  20:54:10  devsrc
# 	cr10871 - fix copyright
# 	[1994/06/10  14:59:55  devsrc]
# 
# Revision 1.1.4.3  1993/07/21  16:13:37  delgado
# 	Fixing reference to LD
# 	[1993/07/21  16:13:17  delgado]
# 
# Revision 1.1.3.4  1993/06/02  23:01:43  tmm
# 	Add version string.
# 	[1993/06/01  17:54:02  tmm]
# 
# Revision 1.1.4.2  1993/06/10  19:24:02  sommerfeld
# 	Initial HPUX RP version.
# 	Add stuff from the GAMERA rpc/kruntime/Makefile to support the
# 	dce_krpc.ext kernel extension (since it is HPUX specific).
# 	[1993/06/03  22:04:14  kissel]
# 
# 	Initial revision
# 	[1993/01/15  21:14:03  toml]
# 
# $EndLog$
#
# rpc/kruntime/HP800/machdep.mk for HPUX-specific RPC kernel rules
#

OTHERS		= dce_krpc.ext

ILIST		= ${OTHERS}
IDIR		= /ext/

# Include files to build and export, and their locations
INCLUDES        += krpc_osi_mach.h


krpc_osi_mach.h_EXPDIR          = /usr/include/dce/ker/


HP800_OBJS_SYS	= krpc_helper_mach.o rpc_config.o

KEXTENSIONS=dce_krpc.ext

dce_krpc.ext_OFILES = $(OBJS_SVC) $(OBJS_COM) $(OBJS_COMMON) $(OBJS_DCE) \
		  $(OBJS_DG) $(OBJS_IP) $(OBJS_MGMT) $(OBJS_NDR) $(OBJS_AUTH) \
		  $(OBJS_KRPC) $(OBJS_SYS) ${${TARGET_OS}_OBJS} \
		  ${${TARGET_MACHINE}_OBJS}

# hide entrypoints which could conceivably be used for encryption/decryption
HIDES = -h des_cbc_encrypt -h des_ecb_encrypt -h des_cbc_cksum \
		-h des_key_sched 				\
	-h sec_des_cbc_encrypt -h sec_des_ecb_encrypt -h sec_des_cbc_cksum \
		-h sec_des_key_sched -h rpc__krb_dg_encrypt \
		-h sec_krb_dg_build_message -h sec_krb_dg_decode_message_kern

# Force this bit of pipe support and pickling into the KRPC extension 
# for use by dfs_server.ext.
dce_krpc.ext_UNDEFS = -u rpc_ss_initialize_callee_pipe -u idl_es_encode_dyn_buffer ${HIDES}
dce_krpc.ext_LIBS=-lkdes -lkidl -lksec
dce_krpc.ext_KLDFLAGS=${LIBDIRS} -e dce_krpc_config

dce_krpc.ext:	${dce_krpc.ext_OFILES}
		${_LD_} -r -N -e dce_krpc_config ${dce_krpc.ext_UNDEFS} -o dce_krpc.ext \
			${dce_krpc.ext_OFILES} ${LIBDIRS} -lkdes -lkidl -lksec 
