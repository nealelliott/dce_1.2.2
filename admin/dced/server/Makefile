#
# @OSF_COPYRIGHT@
# Copyright (c) 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.2  1996/10/03  17:51:04  arvind
# 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
# 	[1996/06/11  17:58 UTC  aha  /main/DCE_1.2.2/1]
#
# 	Flush spurious deps.. .o's only depend on .h's, not stub .o's
# 	[1996/08/09  17:57 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_kdc/1]
#
# 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
# 	[1996/06/10  14:58 UTC  aha  /main/DCE122_PK/2]
#
# 	Merge to DCE122_PK
# 	[1996/06/06  19:00 UTC  aha  /main/DCE122_PK/aha_pk6/1]
#
# 	Add -I../../../security/client/rca to sv_clientd.o_INCFLAGS to allow
# 	rca_pvt.h to be included.
# 	[1996/06/04  18:52 UTC  psn  /main/DCE122_PK/1]
#
# 	Merge sec_pubkey and rsec_pk_cache
# 	[1996/05/07  20:08 UTC  aha  /main/DCE_1.2/aha_pk2/1]
#
# 	Add rsec_pk_cache.idl with -cepv
# 	[1996/01/03  19:37 UTC  psn  /main/DCE_1.2/1]
#
# Revision 1.1.6.1  1996/07/08  18:20:40  arvind
# 	eliminate ClearCase makefile churn for `idlcompile' target..
# 	[1996/06/11  21:33 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
# 
# 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
# 	[1996/06/10  14:58 UTC  aha  /main/DCE122_PK/2]
# 
# 	Merge to DCE122_PK
# 	[1996/06/06  19:00 UTC  aha  /main/DCE122_PK/aha_pk6/1]
# 
# 	Add -I../../../security/client/rca to sv_clientd.o_INCFLAGS to allow
# 	rca_pvt.h to be included.
# 	[1996/06/04  18:52 UTC  psn  /main/DCE122_PK/1]
# 
# 	Merge sec_pubkey and rsec_pk_cache
# 	[1996/05/07  20:08 UTC  aha  /main/DCE_1.2/aha_pk2/1]
# 
# 	Add rsec_pk_cache.idl with -cepv
# 	[1996/01/03  19:37 UTC  psn  /main/DCE_1.2/1]
# 
# 	Remove DES_HIDDEN stuff for OSF DCE 1.2.1 drop
# 	[1995/11/16  21:10 UTC  psn  /main/HPDCE02/jrr_1.2_mothra/1]
# 
# 	Remove -g option.
# 	[1995/10/19  14:06 UTC  jss  /main/HPDCE02/14]
# 
# 	Submitted the fix for CHFts16512.
# 	[1995/09/29  22:37 UTC  tatsu_s  /main/HPDCE02/13]
# 
# 	Initial revision: dced splitting (libdcedpvt).
# 	[1995/09/19  15:30 UTC  tatsu_s  /main/HPDCE02/tatsu_s.dced_local.b0/1]
# 
# 	change OTHERS to PROGRAMS
# 	[1995/07/27  22:13 UTC  root  /main/HPDCE02/12]
# 
# 	change OTHERS to PROGRAMS
# 	[1995/07/27  21:59 UTC  mothra  /main/HPDCE02/12]
# 
# 	fix build of domestic dced to install as dced.Dom
# 	[1995/07/27  02:22 UTC  mothra  /main/HPDCE02/11]
# 
# 	Merge in fix for CHFts15276 from mk_mothra_dced/3.
# 	[1995/06/05  16:44 UTC  mk  /main/HPDCE02/10]
# 
# 	Turn off -g compilation.
# 	[1995/05/16  21:31 UTC  mk  /main/HPDCE02/mk_mothra_dced/3]
# 
# 	Submitted the fix for OT12494/CHFts13959 .
# 	[1995/05/31  13:25 UTC  tatsu_s  /main/HPDCE02/9]
# 
# 	Added dced_acl_patcher.
# 	[1995/05/26  18:47 UTC  tatsu_s  /main/HPDCE02/tatsu_s.dced_security.b0/1]
# 
# 	Merge for CHFts13120
# 	[1995/04/04  17:25 UTC  mdf  /main/HPDCE02/8]
# 
# 	Merge in changes from mk_mothra_dced/2.
# 	[1995/01/13  20:08 UTC  mk  /main/HPDCE02/6]
# 
# 	Enable LLB and RRPC support.
# 	[1995/01/13  19:59 UTC  mk  /main/HPDCE02/mk_mothra_dced/2]
# 
# 	Merge in changes from mk_mothra_dced/1.
# 	[1994/12/07  17:06 UTC  mk  /main/HPDCE02/5]
# 
# 	Add rutc.idl to IDLFILES, rutc_sstub.o to COMPILED_IDL, and rutcops.c
# 	to COMPILED_OTHER.
# 	[1994/11/28  22:45 UTC  mk  /main/HPDCE02/mk_mothra_dced/1]
# 
# 	merge to HPDCE02 branch
# 	[1994/12/05  20:01 UTC  hopkins  /main/HPDCE02/4]
# 
# 	Turn of ld optimization until understand bug.
# 	[1994/11/28  20:14 UTC  hopkins  /main/HPDCE02/hopkins_eslit/1]
# 
# 	merge from Hpdce02_02
# 	[1994/09/22  16:00 UTC  jane  /main/HPDCE02/3]
# 
# 	Merge from Hpdce02_01
# 	[1994/08/19  21:32 UTC  pulsinelli  /main/HPDCE02/2]
# 
# 	fix build order to get header files to build before dced
# 	[1994/07/19  21:01 UTC  mothra  /main/HPDCE02/1]
# 
# Revision 1.1.4.2  1996/03/09  23:07:08  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:25:53  marty]
# 
# Revision 1.1.4.1  1995/12/08  21:27:49  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:10 UTC  psn
# 	Remove DES_HIDDEN stuff for OSF DCE 1.2.1 drop
# 
# 	HP revision /main/HPDCE02/14  1995/10/19  14:06 UTC  jss
# 	Remove -g option.
# 
# 	HP revision /main/HPDCE02/13  1995/09/29  22:37 UTC  tatsu_s
# 	Submitted the fix for CHFts16512.
# 
# 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:30 UTC  tatsu_s
# 	Initial revision: dced splitting (libdcedpvt).
# 
# 	HP revision /main/HPDCE02/12  1995/07/27  22:13 UTC  root
# 	change OTHERS to PROGRAMS
# 
# 	HP revision /main/HPDCE02/12  1995/07/27  21:59 UTC  mothra
# 	change OTHERS to PROGRAMS
# 
# 	HP revision /main/HPDCE02/11  1995/07/27  02:22 UTC  mothra
# 	fix build of domestic dced to install as dced.Dom
# 
# 	HP revision /main/HPDCE02/10  1995/06/05  16:44 UTC  mk
# 	Merge in fix for CHFts15276 from mk_mothra_dced/3.
# 
# 	HP revision /main/HPDCE02/mk_mothra_dced/3  1995/05/16  21:31 UTC  mk
# 	Turn off -g compilation.
# 
# 	HP revision /main/HPDCE02/9  1995/05/31  13:25 UTC  tatsu_s
# 	Submitted the fix for OT12494/CHFts13959 .
# 
# 	HP revision /main/HPDCE02/tatsu_s.dced_security.b0/1  1995/05/26  18:47 UTC  tatsu_s
# 	Added dced_acl_patcher.
# 
# 	HP revision /main/HPDCE02/8  1995/04/04  17:25 UTC  mdf
# 	Merge for CHFts13120
# 
# 	HP revision /main/HPDCE02/7  1995/02/02  18:58 UTC  sommerfeld
# 	Fix international build
# 	[1995/02/02  18:43 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_4/1]
# 
# 	HP revision /main/HPDCE02/6  1995/01/13  20:08 UTC  mk
# 	Merge in changes from mk_mothra_dced/2.
# 
# 	HP revision /main/HPDCE02/mk_mothra_dced/2  1995/01/13  19:59 UTC  mk
# 	Enable LLB and RRPC support.
# 
# 	HP revision /main/HPDCE02/5  1994/12/07  17:06 UTC  mk
# 	Merge in changes from mk_mothra_dced/1.
# 
# 	HP revision /main/HPDCE02/mk_mothra_dced/1  1994/11/28  22:45 UTC  mk
# 	Add rutc.idl to IDLFILES, rutc_sstub.o to COMPILED_IDL, and rutcops.c
# 	to COMPILED_OTHER.
# 
# 	HP revision /main/HPDCE02/4  1994/12/05  20:01 UTC  hopkins
# 	merge to HPDCE02 branch
# 
# 	HP revision /main/HPDCE02/hopkins_eslit/1  1994/11/28  20:14 UTC  hopkins
# 	Turn of ld optimization until understand bug.
# 
# 	HP revision /main/HPDCE02/3  1994/09/22  16:00 UTC  jane
# 	merge from Hpdce02_02
# 	[1995/12/08  17:59:53  root]
# 
# Revision 1.1.2.19  1994/08/28  01:07:24  rsalz
# 	Set ILIST right (OT CR 11930).
# 	[1994/08/28  01:05:33  rsalz]
# 
# Revision 1.1.2.18  1994/08/26  20:15:10  bowe
# 	Added dced_svc.o [CR 11725]
# 	Rename dced-postproc.sh dcecf_postproc [CR 11705]
# 	[1994/08/26  20:11:01  bowe]
# 
# Revision 1.1.2.17  1994/08/25  20:14:14  bowe
# 	Added dced-postproc.sh [CR 11705]
# 	[1994/08/25  20:13:18  bowe]
# 
# 	HP revision /main/HPDCE02/2  1994/08/19  21:32 UTC  pulsinelli
# 	Merge from Hpdce02_01
# 
# Revision 1.1.2.16  1994/08/15  21:18:28  bowe
# 	Do not use DCEPATHS. [CR 11656]
# 	[1994/08/15  21:17:27  bowe]
# 
# Revision 1.1.2.15  1994/07/29  19:56:00  rsalz
# 	Add copy.c, split out from misc.c
# 	[1994/07/29  19:36:05  rsalz]
# 
# Revision 1.1.2.14  1994/07/06  20:17:16  pwang
# 	Removed free.o [OT#11003]
# 	[1994/07/06  20:09:43  pwang]
# 
# Revision 1.1.2.13  1994/06/30  19:36:56  ohara
# 	use CC_OPT_LEVEL, not OPT_LEVEL  since some LD don't take -g
# 	[1994/06/27  20:29:02  ohara]
# 
# 	HP revision /main/HPDCE02/1  1994/07/19  21:01 UTC  mothra
# 	fix build order to get header files to build before dced
# 
# Revision 1.1.2.12  1994/06/16  19:35:03  rsalz
# 	Add free (temporary until integrated into dced API).
# 	[1994/06/16  19:31:34  rsalz]
# 
# Revision 1.1.2.11  1994/06/10  21:51:11  pwang
# 	The integration of sec_clientd stuff requires to link with
# 	bsd and krb3 libraries on RIOS [OT#10931]
# 	[1994/06/10  21:50:51  pwang]
# 
# Revision 1.1.2.10  1994/06/06  19:44:56  bowe
# 	Added sv_preauth stuff.
# 	[1994/06/06  19:39:07  bowe]
# 
# Revision 1.1.2.9  1994/05/26  19:42:04  rsalz
# 	EP functionality.
# 	[1994/05/26  19:35:32  rsalz]
# 
# 	Checkpointing.
# 	[1994/05/18  19:32:51  rsalz]
# 
# Revision 1.1.2.8  1994/05/10  20:15:05  rsalz
# 	Cleanup and more functionality
# 	[1994/05/10  20:10:33  rsalz]
# 
# Revision 1.1.2.7  1994/05/05  20:07:24  rsalz
# 	Initial server functionality drop.
# 	[1994/05/05  20:00:52  rsalz]
# 
# Revision 1.1.2.6  1994/04/27  19:41:32  rsalz
# 	Add serviceability files and init
# 	[1994/04/27  19:36:48  rsalz]
# 
# 	Add lock manager.
# 	[1994/04/27  19:00:17  rsalz]
# 
# Revision 1.1.2.5  1994/04/18  00:14:53  pwang
# 	Removed -ldced since libdced.a is in libdce now.
# 	[1994/04/17  22:06:11  pwang]
# 
# Revision 1.1.2.4  1994/04/14  20:08:30  rsalz
# 	Make server and API all compile.
# 	[1994/04/14  19:45:08  rsalz]
# 
# Revision 1.1.2.3  1994/04/11  20:22:02  rsalz
# 	Final set of RFC revision fixes.
# 	[1994/04/11  20:08:22  rsalz]
# 
# Revision 1.1.2.2  1994/03/18  19:59:03  rsalz
# 	Add ep IDL; standardize IDL compile lines (OT CR 9768)
# 	[1994/03/18  19:58:33  rsalz]
# 
# Revision 1.1.2.1  1994/03/09  14:25:36  bowe
# 	Initial checkin
# 	[1994/03/09  14:23:55  bowe]
# 
# $EndLog$
#

COMPILED_IDL	= ep_sstub.o hostdata_sstub.o \
		rdaclif_sstub.o rdaclifv0_sstub.o rkeytab_sstub.o \
		roverride_sstub.o rs_attr_schema_sstub.o \
		rsec_cert_sstub.o rsec_login_sstub.o \
		secval_sstub.o srvrconf_sstub.o srvrexec_sstub.o \
                rutc_sstub.o rsec_pk_cache_sstub.o
COMPILED_OTHER	= main.o epops.o svops.o \
		sv_clientd.o sv_preauth.o sv_rover.o oemisc.o oeops.o \
		dced_svc.o pe_update.o \
                rutcops.o

SCRIPTS		= dcecf_postproc dced_acl_patcher

CFLAGS		+= ${STRICT_COMPILER_WARNINGS}
sv_preauth.o_CFLAGS += ${STRICT_COMPILER_WARNINGS:S/-Wid-clash-32//} -DOSF_DCE
INCFLAGS	= -I. -I../idl \
		-I../../../rpc/runtime \
		-I../../../rpc/runtime/${TARGET_MACHINE} \
		-I../../../security/h -I../../../security/idl
sv_clientd.o_INCFLAGS = -I. -I../idl \
		-I../../../rpc/runtime \
		-I../../../rpc/runtime/${TARGET_MACHINE} \
		-I../../../security/h -I../../../security/idl \
                -I../../../security/client/rca
epops.o_INCFLAGS= -I. -I../idl -I../libdcedpvt \
		-I../../../rpc/runtime \
		-I../../../rpc/runtime/${TARGET_MACHINE}
oeops.o_INCFLAGS= -I. -I../idl -I../libdcedpvt \
		-I../../../rpc/runtime \
		-I../../../rpc/runtime/${TARGET_MACHINE}
oemisc.o_INCFLAGS= -I. -I../idl -I../libdcedpvt \
		-I../../../rpc/runtime \
		-I../../../rpc/runtime/${TARGET_MACHINE}
lbops.o_INCFLAGS= -I. -I../idl -I../libdcedpvt \
		-I../../../rpc/runtime \
		-I../../../rpc/runtime/${TARGET_MACHINE}

dced_acl_patcher_IDIR = /etc/
INCLUDES	=
.if ${TARGET_MACHINE} == "RIOS"
LIBS	= -lbsd -lkrb5 -ldcedpvt -ldce
.else
LIBS	= -ldcedpvt -ldce
.endif

.if defined(DES_HIDDEN)
LIBS		:= ../../../libdce/bound_dce_nostrip.o ${LIBS}
.endif
PROGRAMS	= dced
ILIST		= ${PROGRAMS} ${SCRIPTS}
dced_OFILES	= $(COMPILED_IDL) $(COMPILED_OTHER) 

IDIR		= /bin/

ep.idl_ACF		=
IDLFLAGS		= -keep c_source
hostdata.idl_IDLFLAGS	= -keep c_source -no_mepv
rkeytab.idl_IDLFLAGS	= -keep c_source -no_mepv
rs_attr_schema.idl_IDLFLAGS	= -keep c_source -no_mepv
srvrconf.idl_IDLFLAGS	= -keep c_source -no_mepv
srvrexec.idl_IDLFLAGS	= -keep c_source -no_mepv
rdaclifv0.idl_IDLFLAGS	= -keep c_source -no_mepv
rdaclif.idl_IDLFLAGS	= -keep c_source -no_mepv
ep.idl_IDLFLAGS		= -keep c_source -no_mepv
rs_attr_schema.idl_IDLINCFLAGS	= -I../../../security/idl
rsec_pk_cache.idl_IDLINCFLAGS = -I../../../security/idl
rsec_pk_cache.idl_IDLFLAGS = -keep c_source -cepv

IDLFILES	= hostdata.idl rkeytab.idl secval.idl \
		srvrconf.idl srvrexec.idl rs_attr_schema.idl ep.idl \
		roverride.idl rsec_cert.idl rsec_login.idl \
		rdaclif.idl rdaclifv0.idl \
                rutc.idl rsec_pk_cache.idl
VPATH		= ../idl:../../../security/idl:../../../rpc/sys_idl

llb.idl_IDLFLAGS	= -keep c_source -no_mepv
rrpc.idl_IDLFLAGS	= -keep c_source -no_mepv
IDLFILES		+= llb.idl rrpc.idl
COMPILED_IDL		+= llb_sstub.o rrpc_sstub.o
COMPILED_OTHER		+= lbops.o ncsops.o
CFLAGS			+= -DWANT_LLB ${DCEPATHS}

.include <${RULES_MK}>

idlcompile:		$(COMPILED_IDL:S/_sstub.o/.h/g:S/_cstub.o/.h/g:M*.h)
	echo ${COMPILED_IDL} > idlcompile

$(COMPILED_OTHER):      idlcompile

rdaclif_sstub.o:	rdaclif_sstub.c
rdaclifv0_sstub.o:	rdaclifv0_sstub.c
rs_attr_schema_sstub.o:	rs_attr_schema_sstub.c
ep_sstub.o:		ep_sstub.c
ep_sstub.c:		ep.idl
llb_sstub.o:		llb_sstub.c
llb_sstub.c:		llb.idl
rrpc_sstub.o:		rrpc_sstub.c
rrpc_sstub.c:		rrpc.idl
rsec_pk_cache_sstub.o:  rsec_pk_cache_sstub.c
