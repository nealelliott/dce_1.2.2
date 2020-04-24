#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.16.6  1996/11/13  17:57:38  arvind
# 	CAPI drop from DEC
# 	[1996/10/16  20:23 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
#
# 	#
#
# 	Audit for 1.2.2
# 	[1996/06/26  20:43 UTC  arvind  /main/DCE_1.2.2/2]
#
# Revision 1.1.16.5  1996/10/15  20:45:35  arvind
# 	OSF DCE 1.2.2 Drop 4.5
# 	[1996/10/15  20:09:53  arvind]
# 
# Revision 1.1.16.4  1996/10/04  16:42:10  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/04  16:41:53  arvind]
# 
# Revision /main/DCE_1.2.2/5  1996/10/02  18:28 UTC  cuti
# 	Merge of cuti_pk_fix
# 
# Revision /main/DCE_1.2.2/cuti_pk_fix/1  1996/09/26  16:18 UTC  cuti
# 	Make international build work
# 
# Revision /main/DCE_1.2.2/4  1996/09/09  21:23 UTC  arvind
# 	PKSS drop from DEC (DCE1.2.2)
# 	[1996/08/30  15:38 UTC  arvind  /main/DCE_1.2.2/arvind_pkss/1]
# 
# 	Merge from cuti_pk_export to DCE_1.2.2
# 	[1996/06/27  19:32 UTC  cuti  /main/DCE_1.2.2/DCE122_PK/cuti_pk_export/3]
# 	#
# 	Merge out from DCE122
# 	#
# 	Add BSAFE_LIB to LIBS
# 	[1996/06/27  13:55 UTC  cuti  /main/DCE_1.2.2/DCE122_PK/cuti_pk_export/2]
# 	#
# 	Add psm
# 	[1996/06/19  19:52 UTC  cuti  /main/DCE_1.2.2/DCE122_PK/cuti_pk_export/1]
# 	#
# 	Merge to DCE122_PK
# 	[1996/06/10  14:58 UTC  aha  /main/DCE_1.2.2/DCE122_PK/1]
# 	#
# 	Changes for Public Key Login; remove int2krb5_int32 and krb5_int322int
# 	from EXPORT_KRB5_SYMS as the functions are no longer defined.
# 	[1996/06/07  17:45 UTC  aha  /main/DCE_1.2.2/aha_pk6/2]
# 	#
# 	Changes for Public Key Login; add krb5 symbols that are referenced by public key code.
# 	[1996/06/07  14:02 UTC  aha  /main/DCE_1.2.2/aha_pk6/1]
# 	#
# 	Audit for 1.2.2
# 	[1996/06/21  20:45 UTC  arvind  /main/arvind_audit122/2]
# 	#
# 	Audit for 1.2.2
# 	[1996/06/21  20:27 UTC  arvind  /main/arvind_audit122/1]
# 	#
# 	Merge krb5 work to DCE1.2.2 mainline
# 	[1996/05/29  20:28 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
# 	#
# 	Allow krb5_copy_creds and com_err to be exported.
# 	[1996/01/03  19:05 UTC  psn  /main/DCE_1.2/1]
# 	#
# 
# 	Merge krb5 work to DCE1.2.2 mainline
# 	[1996/05/29  20:28 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
# 
# 	Allow krb5_copy_creds and com_err to be exported.
# 	[1996/01/03  19:05 UTC  psn  /main/DCE_1.2/1]
# 
# 	Add additional symbols for export from the krb5 library
# 	[1995/11/16  21:39 UTC  dat  /main/jrr_1.2_mothra/1]
# 
# 	Update OSF copyright years
# 	[1996/03/11  13:13:44  marty]
# 
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/jrr_1.2_mothra/1  1995/11/16  21:39 UTC  dat
# 	Add additional symbols for export from the krb5 library
# 	[1995/12/08  16:27:22  root]
# 
# Revision /main/DCE_1.2.2/3  1996/06/30  21:59 UTC  cuti
# 	Merge from cuti_pk_export to DCE_1.2.2
# 
# Revision /main/DCE_1.2.2/DCE122_PK/cuti_pk_export/3  1996/06/27  19:32 UTC  cuti
# 	Merge out from DCE122
# 
# 	Add BSAFE_LIB to LIBS
# 	[1996/06/27  13:55 UTC  cuti  /main/DCE_1.2.2/DCE122_PK/cuti_pk_export/2]
# 
# 	Add psm
# 	[1996/06/19  19:52 UTC  cuti  /main/DCE_1.2.2/DCE122_PK/cuti_pk_export/1]
# 
# 	Merge to DCE122_PK
# 	[1996/06/10  14:58 UTC  aha  /main/DCE_1.2.2/DCE122_PK/1]
# 
# 	Changes for Public Key Login; remove int2krb5_int32 and krb5_int322int
# 	from EXPORT_KRB5_SYMS as the functions are no longer defined.
# 	[1996/06/07  17:45 UTC  aha  /main/DCE_1.2.2/aha_pk6/2]
# 
# 	Changes for Public Key Login; add krb5 symbols that are referenced by public key code.
# 	[1996/06/07  14:02 UTC  aha  /main/DCE_1.2.2/aha_pk6/1]
# 	Revision /main/DCE_1.2.2/2  1996/06/26  20:43 UTC  arvind
# 	Audit for 1.2.2
# 
# Revision /main/arvind_audit122/2  1996/06/21  20:45 UTC  arvind
# 	Audit for 1.2.2
# 	[1996/06/21  20:27 UTC  arvind  /main/arvind_audit122/1]
# 
# Revision /main/DCE_1.2.2/1  1996/05/30  21:14 UTC  mullan_s
# 	Merge krb5 work to DCE1.2.2 mainline
# 
# Revision /main/mullan_dce1.2.2_krb5_drop/1  1996/05/29  20:28 UTC  mullan_s
# 	Update OSF copyright years
# 	[1996/03/11  13:13:44  marty]
# 	#
# 
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/jrr_1.2_mothra/1  1995/11/16  21:39 UTC  dat
# 	Add additional symbols for export from the krb5 library
# 	[1995/12/08  16:27:22  root]
# 
# Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1  1996/05/03  19:55 UTC  mullan_s
# 	Allow krb5_copy_creds and com_err to be exported.
# 
# Revision /main/DCE_1.2/1  1996/01/03  19:05 UTC  psn
# 	Add additional symbols for export from the krb5 library
# 	[1995/11/16  21:39 UTC  dat  /main/jrr_1.2_mothra/1]
# 
# Revision 1.1.9.10  1994/06/21  14:40:59  marrek
# 	Remove _r libraries in June 1994 code submission.
# 	[1994/06/09  12:16:25  marrek]
# 
# Revision 1.1.9.9  1994/05/10  16:04:41  marrek
# 	libslog removed in April 1994 submission.
# 	[1994/04/27  14:33:43  marrek]
# 
# Revision 1.1.9.8  1994/04/18  00:14:51  pwang
# 	Loaded libdced.a into libdce OT#10158
# 	[1994/04/17  21:56:16  pwang]
# 
# Revision 1.1.9.7  1994/03/23  15:50:38  keutel
# 	GDS changes
# 	[1994/03/23  14:15:50  keutel]
# 
# Revision 1.1.9.4  1994/02/22  19:01:47  marrek
# 	gda_util doesn't have to be linked to shr.o
# 	[1994/02/21  13:39:32  marrek]
# 
# 	21sw ---> d21sw for gds/dua/switch
# 	[1994/02/21  11:41:10  marrek]
# 
# 	Changes for libdce.
# 	[1994/02/10  17:04:58  marrek]
# 
# Revision 1.1.9.3  1994/02/09  20:54:04  mori_m
# 	CR 9912:  Added libiconv reference.
# 	[1994/02/09  20:53:24  mori_m]
# 
# Revision 1.1.9.2  1994/01/27  22:09:49  bowe
# 	Added  ${libacldb.a:P} ${libdceutils.a:P} ${libdb44.a:P} to
# 	the list of libraries to be assembled into libdce.a.
# 	[1994/01/27  19:21:54  bowe]
# 
# Revision 1.1.9.1  1993/12/17  20:09:39  mario
# 	Added libsvc.a to LIBS.
# 	[1993/12/17  20:08:48  mario]
# 
# Revision 1.1.7.2  1993/10/19  22:11:19  rsarbo
# 	add new DFS i/f's (libdfsjunct.a) so CDS can understand
# 	configurable DFS junctions
# 	[1993/10/19  21:22:26  rsarbo]
# 
# Revision 1.1.7.1  1993/10/05  17:31:51  zeliff
# 	Defect 9041 - Fix references to libslog_r and libdua_r
# 	[1993/10/05  17:04:24  zeliff]
# 
# Revision 1.1.5.5  1993/02/02  21:40:53  tom
# 	Bug 6991 - fix build problem with libutc.
# 	[1993/02/02  21:36:52  tom]
# 
# Revision 1.1.5.3  1993/01/25  16:13:59  rsarbo
# 	fix NO_DFS builds on the RIOS
# 	[1993/01/25  16:13:36  rsarbo]
# 
# Revision 1.1.5.2  1992/12/30  17:16:59  zeliff
# 	Embedding copyright notice
# 	[1992/12/30  17:15:09  zeliff]
# 
# Revision 1.1.2.5  1992/07/02  21:42:53  zeliff
# 	Adding libslog.a to libdce.a
# 	[1992/07/02  21:41:21  zeliff]
# 
# Revision 1.1.2.4  1992/05/05  18:47:18  rec
# 	Added list of krb5 symbols that may be exported by
# 	libdce.a.  This list is created by adding the symbols
# 	to the list generated by makesym.sh.
# 	[1992/05/05  18:46:41  rec]
# 
# Revision 1.1.2.3  1992/02/04  22:02:46  mhickey
# 	Changes for the genereic libdce/Makefile
# 
# 	     1) have all of the directories where the libraries that make up libdce
# 	        are build in VPATH;
# 	     2) depend on the libraries that make up libdce (which will be found
# 	        in the object tree)
# 	     3) loop through the libraries, mapping symbols and unarchiving members
# 	     5) define LIBDCE_SOURCES_DEFINED
# 	[1992/02/04  21:59:20  mhickey]
# 
# Revision 1.1.3.4  1992/02/03  06:06:00  mhickey
# 	Final changes to make this work with a merged libdce/Makefile
# 
# 	Defect 2030
# 
# Revision 1.1.3.3  1992/01/31  18:25:12  mhickey
# 	Modified to take advantage of the rules in libdce/Makefile
# 	for visting subdirectories and building pieces.
# 
# Revision 1.1  1992/01/19  15:31:35  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# DCE 
# libdce Makefile
#
VPATH=${TARGET_MACHINE}:${SLIST}

SCRIPTS	= makesyms 
HOST_LD = xlC
RIOS_CPP_LIBS = -L/usr/lpp/xlC/lib -L/usr/lpp/xlC -lC -lm

.if !defined(NO_DFS)
DFS_AUX_LIBS = -lafssys -ldacl2acl -ldfsjunct
.else
DFS_AUX_LIBS = 
.endif

# list of symbols from krb5 that are
# allowed to be exported in libdce
EXPORT_KRB5_SYMS = krb5_cksumarray krb5_cc_default krb5_unparse_name \
                   krb5_get_default_realm krb5_build_principal \
                   krb5_read_password krb5_decode_generic \
		   krb5_rd_req_decoded krb5_free_ap_req krb5_mk_safe \
		   krb5_rd_priv krb5_free_address krb5_free_cred_contents \
		   krb5_free_tkt_authent krb5_init_ets krb5_parse_name \
		   AP_REQ_dec AP_REQ2krb5_ap_req AP_REQ_free error_message \
		   krb5_copy_creds com_err

EXTRA_SYMS       = afs_syscall

.if defined (USE_DES)

LIBS    = ${libcma.a:P} ${libsvc.a:P} ${libnck.a:P} ${libidl.a:P} \
	${libacldb.a:P} ${libdceutils.a:P} ${libdb44.a:P} ${libdced.a:P} \
	${libcds.a:P} ${libasn1.a:P} ${libsec.a:P} ${libpsm.a:P} \
        ${libpkssapi.a:P} ${libpk.a:P} ${libpkcrypto.a:P} \
	${libpkxds.a:P} ${libpkplcy.a:P} ${libpktrust.a:P} \
        ${libutc-${LIBUTC_MODE}.a:P} ${libdua.a:P} ${libXDS.a:P} \
        ${libXOM.a:P} ${libdce_conf.a:P} ${libd21sw.a:P} \
	${libd23.a:P} ${libd27.a:P} ${libosiaddr.a:P} ${libxoms.a:P} \
	${libdua_sec.a:P} ${libaudit.a:P} ${BSAFELIB} 
.else
LIBS    = ${libcma.a:P} ${libsvc.a:P} ${libnck.a:P} ${libidl.a:P} \
	${libacldb.a:P} ${libdceutils.a:P} ${libdb44.a:P} ${libdced.a:P} \
	${libcds.a:P} ${libasn1.a:P} ${libsec.a:P} ${libpsm.a:P} \
        ${libutc-${LIBUTC_MODE}.a:P} ${libdua.a:P} ${libXDS.a:P} \
        ${libXOM.a:P} ${libdce_conf.a:P} ${libd21sw.a:P} \
	${libd23.a:P} ${libd27.a:P} ${libosiaddr.a:P} ${libxoms.a:P} \
	${libdua_sec.a:P} 
.endif

libdce.a: ${LIBS} syms.imp makesyms
	${RM} ${_RMFLAGS_} *.o libdce.syms 
	for x in ${LIBS}; \
	do \
		./makesyms $$x >> libdce.syms; \
		ar -xv $$x; \
	done;	
	for y in ${EXPORT_KRB5_SYMS}; \
	do \
		echo $$y >> libdce.syms; \
	done;
	for y in ${EXTRA_SYMS}; \
	do \
		echo $$y >> libdce.syms; \
	done;
#	${HOST_LD} -v -o shr.o *.o -bloadmap:shr.o.map -bM:sre -bE:libdce.syms -bI:/lib/syscalls.exp -bI:${syms.imp:P} ${LIBDIRS} -T512 -H512 -lbsd -lc -lm -lcurses -lkrb5 -liconv ${DFS_AUX_LIBS} 2> shr.o.error
	${_LD_} -o shr.o *.o -bloadmap:shr.o.map -bM:sre -bE:libdce.syms -bI:/lib/syscalls.exp -bI:${syms.imp:P} ${LIBDIRS} ${RIOS_CPP_LIBS} -T512 -H512 -lbsd -lc -lm -lcurses -lkrb5 -liconv ${DFS_AUX_LIBS} 2> shr.o.error
	rm -f libdce.a
	ar crv libdce.a shr.o
	${RM} ${_RMFLAGS_} *.o

LIBDCE_SOURCES_DEFINED=
