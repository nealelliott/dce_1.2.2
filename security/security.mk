#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: security.mk,v $
# Revision 1.2.27.2  1996/07/08  18:37:38  arvind
# 	Merge to DCE 1.2.2
# 	[1996/06/21  19:31 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce_krb5_der_work/1]
#
# 	Add -DPOSIX_FILE_LOCKS to RIOS CFLAGS.
# 	[1996/05/24  23:16 UTC  sommerfeld  /main/DCE_1.2.2/1]
#
# 	Use ANSI C for RIOS in /security/server/rrs
# 	[1996/04/01  19:18 UTC  arvind  /main/DCE_1.2/2]
#
# 	Use ANSI C for RIOS in /security/server/rrs
# 	[1996/04/01  19:05 UTC  arvind  /main/DCE_1.2/arvind_checkout/1]
#
# 	Update OSF copyright year
# 	[1996/03/09  22:45:16  marty]
#
# 	Submit OSF/DCE 1.2.1
# 	HP 	[1995/11/08  15:38 UTC  dat  /main/HPDCE02/jrr_1.2_mothra/1]
#
# 	The +O3 optimize flag is not recognized on other platforms so confine to HP use only
# 	HP 	[1995/08/25  23:05 UTC  sommerfeld  /main/HPDCE02/3]
#
# 	CHFts16189: Allow special opt levels to be overridden.
# 	HP 	[1995/08/21  17:46 UTC  sommerfeld  /main/HPDCE02/2]
# 	[1996/07/08  18:09:33  arvind]
#
# Revision 1.2.27.1  1996/05/10  13:19:36  arvind
# 	Drop 1 of DCE 1.2.2 to OSF
# 
# 	HP revision /main/DCE_1.2/2  1996/04/01  19:18 UTC  arvind
# 	Use ANSI C for RIOS in /security/server/rrs
# 
# 	HP revision /main/DCE_1.2/arvind_checkout/1  1996/04/01  19:05 UTC  arvind
# 	Use ANSI C for RIOS in /security/server/rrs
# 
# 	HP revision /main/DCE_1.2/1  1996/01/03  19:15 UTC  psn
# 	The +O3 optimize flag is not recognized on other platforms so confine to HP use only
# 	[1995/11/08  15:38 UTC  dat  /main/HPDCE02/jrr_1.2_mothra/1]
# 
# 	CHFts16189: Allow special opt levels to be overridden.
# 	[1995/08/25  23:05 UTC  sommerfeld  /main/HPDCE02/3]
# 
# 	add more +O3's.
# 	[1995/08/01  19:20 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/2]
# 
# 	Use +O3 on inner loop crypto code (MD5, DES).
# 	[1995/07/30  19:15 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts15483/1]
# 
# 	International build kludge.
# 	[1995/01/27  03:48 UTC  sommerfeld  /main/sommerfeld_mothmk_2/1]
# 
# Revision 1.2.22.4  1994/09/16  16:03:51  rsalz
# 	Remove sec_clientd rules (OT CR 11488).
# 	[1994/09/15  19:39:39  rsalz]
# 
# Revision 1.2.22.3  1994/09/09  20:43:36  annie
# 	CR12111 - ld does not have -g option for all platforms unless you are an HP
# 	[1994/09/09  20:43:30  annie]
# 
# Revision 1.2.22.2  1994/09/08  20:52:51  annie
# 	cr 12111 - enable ability to turn -g for security component
# 	[1994/09/08  20:46:09  annie]
# 
# Revision 1.2.22.1  1994/05/17  20:11:14  ahop
# 	sec_clientd now requires -lkrb5 in RIOS_LIBS
# 	[1994/05/17  15:50:22  ahop]
# 
# Revision 1.2.20.2  1993/05/24  20:48:10  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:15:45  cjd]
# 
# Revision 1.2.18.2  1993/05/12  12:55:06  jd
# 	Initial 486 port.
# 	[1993/05/12  12:54:48  jd]
# 
# Revision 1.2.11.10  1993/03/05  18:30:45  frisco
# 	add support for sec_salvage_db
# 	[1993/03/03  20:06:34  frisco]
# 
# Revision 1.2.11.9  1993/02/01  20:36:26  hinman
# 	[hinman@sni] - Final merge before bsubmit
# 	[1993/01/31  16:14:59  hinman]
# 
# Revision 1.2.11.8  1993/01/26  18:06:11  sommerfeld
# 	[CR7011] Clean up USE_GETPW_R handling for HP800
# 	[1993/01/26  17:27:15  sommerfeld]
# 	Revision 1.2.16.2  1993/01/11  16:12:32  hinman
# 	[hinman] - Check in merged SNI version
# 
# Revision 1.2.11.7  1992/12/29  16:33:27  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  21:21:17  zeliff]
# 
# Revision 1.2.11.6  1992/12/04  21:31:12  burati
# 	CR5978 nonportable passwd struct code is now in machdep dir and has
# 	nonconflicting name now so we don't need -D_PWD_INCLUDED for HP800.
# 	[1992/12/04  16:45:44  burati]
# 
# Revision 1.2.11.5  1992/11/04  22:26:32  burati
# 	sec_admin is now built in the admin dir instead of server/bin
# 	[1992/11/04  21:16:19  burati]
# 
# Revision 1.2.11.4  1992/10/27  20:19:48  sekhar
# 	[OT 5590]  Added SECURITY_CFLAGS which sets DES_NEUTER when the USE_DES
# 	           is not defined (required for building an international version).
# 	[1992/10/27  15:46:53  sekhar]
# 
# Revision 1.2.11.3  1992/10/16  17:45:03  rsarbo
# 	moved file specific CFLAGS definitions to
# 	security/utils/Makefile
# 	[1992/10/16  17:38:11  rsarbo]
# 
# Revision 1.2.11.2  1992/09/29  21:20:08  devsrc
# 	Added a missing .endif
# 	[1992/09/18  16:05:32  sekhar]
# 
# 	Remove INTERMED_SHLIBS.
# 	[1992/09/15  20:17:06  sekhar]
# 
# 	[OT 5373]    SNI/SVR4 merge.
# 	[1992/09/11  21:36:01  sekhar]
# 
# Revision 1.2.2.8  1992/07/08  22:57:39  sekhar
# 	Defunct destoo and desneuter directories (replaced by crypto)
# 	[1992/07/08  22:57:23  sekhar]
# 
# Revision 1.2.2.7  1992/06/30  21:53:24  burati
# 	CR4516 HP/UX porting changes
# 	[1992/06/30  18:23:56  burati]
# 
# Revision 1.2.2.6  1992/06/01  20:40:47  sekhar
# 	*** empty log message ***
# 	[1992/06/01  19:51:32  sekhar]
# 
# Revision 1.2.6.2  1992/05/15  21:42:15  pato
# 	OT#2330 Don't define USE_DES unconditionally - ../Makeconf will set
# 	this variable; add SECURITY_CFLAGS
# 
# Revision 1.2.2.5  1992/04/23  14:48:55  burati
# 	Added passwd_import block.
# 	Removed unnecessary libs from APOLLO68K_LIBS
# 	[1992/04/23  01:54:56  burati]
# 
# Revision 1.2.2.4  1992/04/07  19:19:04  mckeen
# 	Only use traditional compiler on RIOS
# 	[1992/03/31  16:37:26  mckeen]
# 
# Revision 1.2.2.3  1992/01/22  23:29:26  jim
# 	Merge changes from DCE1.0 tape with defect fixes archive.  Some reordering
# 	necessary but all function and flags retained.
# 	[1992/01/22  23:28:15  jim]
# 
# Revision 1.2.2.2  1992/01/22  22:33:55  jim
# 	added krb5 to RIOS_LIBS because of build differences.
# 	[1992/01/22  21:50:24  jim]
# 
# Revision 1.2  1992/01/19  22:13:50  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#  OSF DCE Version 1.0 

# Copyright (c) Hewlett-Packard Company 1991, 1992
# Unpublished work. All Rights Reserved.
#
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
# All Rights Reserved
#
# 10-22-91; mhickey: Modified for the new model for building libdce.a.  This
#                    means that:
#                       - This makefile allows 'make' to pick up the
#                         definitions within it if it is being read as part
#                         of the parsing of the libdce.a Makefile; ie: the
#                         variable LIBDCE_LIBRARY_MAKEFILE is defined.
#
#                       - The '.if' conditions that determine the settings of
#                         variables for directories where pieces of libdce.a
#                         are built are conditional on the value of the
#                         variable LIBDCE_CURR_INCDIR if the variable
#                         LIBDCE_LIBRARY_MAKEFILE is set.
#
#                       - ${${TARGET_MACHINE}_OFILES} settings for directories
#                         where pieces of libdce.a are built are changed to
#                         ${${TARGET_MACHINE}_<libname>_OFILES}.
#
#

#
# This makefile needs to allow multiple accesses for the libdce build.  The
# multiple accesses should not be idempontent, since we want different settings
# on each access.
#
.if !defined(_SECURITY_MK_) || defined(LIBDCE_LIBRARY_MAKEFILE)
_SECURITY_MK_=

.if ${TARGET_MACHINE}=="RIOS"
CCTYPE=traditional
.endif

.if ${TARGET_MACHINE}=="RIOS" && ${MAKESUB}=="security/server/rrs/"
CCTYPE=ansi
.endif

.if ${MAKESUB}=="security/krb5/lib/krb5/"
RIOS_CFLAGS	?= -Dunix -D_BSD -DBERKELEY
.endif

.if ${MAKESUB}=="security/krb5/lib/mvr/"
RIOS_CFLAGS	?= -Dunix -D_BSD -DBERKELEY
.endif

#
# The condition on this directory needs to allow parsing when 'make' is
# building the library, and when the libdce Makefile is including this
# Makefile.
#
.if ${MAKESUB}=="security/utils/" || (defined(LIBDCE_LIBRARY_MAKEFILE) && ${LIBDCE_CURR_INCDIR} == "security/utils/")
RIOS_CFLAGS	?= -Dunix
RIOS_libsec_OFILES      ?= environment.o
HP800_libsec_OFILES     ?= environment.o
MIPS_CFLAGS     ?= -DCOMPAT_RPC5
VAX_CFLAGS      ?= -DCOMPAT_RPC5
.endif

.if ${MAKESUB}=="security/server/rsdb/"
RIOS_CFLAGS	?= -Dunix
VAX_CFLAGS	?= -Mg 
.endif

.if ${MAKESUB}=="security/server/rs/"
RIOS_CFLAGS	?= -Dunix 
VAX_CFLAGS	?= -Mg 

.endif

.if ${MAKESUB}=="security/server/bin/"
PMAX_LIBS       ?= -lbsd
RIOS_CFLAGS	?= -Dunix 
RIOS_LIBS	?= -lbsd -lkrb5
MIPS_LIBS	?= -li
VAX_LIBS	?= -li
APOLLO68K_LIBS  ?= -lintl
.endif

.if ${MAKESUB}=="security/server/sec_salvage_db/"
PMAX_LIBS       ?= -lbsd
RIOS_CFLAGS	?= -Dunix 
RIOS_LIBS	?= -lbsd -lkrb5
MIPS_LIBS	?= -li
VAX_LIBS	?= -li
APOLLO68K_LIBS  ?= -lintl
.endif

#
# The condition on this directory needs to allow parsing when 'make' is
# building the library, and when the libdce Makefile is including this
# Makefile.
#
.if ${MAKESUB}=="security/krb5/lib/krb5/" || (defined(LIBDCE_LIBRARY_MAKEFILE) && ${LIBDCE_CURR_INCDIR} == "security/krb5/lib/krb5/")
.if ${TARGET_MACHINE} == "HP800"
md5.o_OPT_LEVEL ?= +O3
ecb_encrypt.o_OPT_LEVEL ?= +O3
cbc_encrypt.o_OPT_LEVEL ?= +O3
cksum.o_OPT_LEVEL ?= +O3
make_sched.o_OPT_LEVEL ?= +O3
des_tables.o_OPT_LEVEL ?= +O3
pcbc_encrypt.o_OPT_LEVEL ?= +O3
.endif
RIOS_CFLAGS	?= -Dunix -D_BSD -DBERKELEY
MIPS_OFILES     = strdup.o
VAX_OFILES      = strdup.o
SEC_DROP_SYSLOG = false
.if ${TARGET_MACHINE} == "MIPS"
SEC_DROP_SYSLOG = true
.endif
.if ${TARGET_MACHINE} == "VAX"
SEC_DROP_SYSLOG = true
.endif
.if ${TARGET_MACHINE} == "SVR4"
SEC_DROP_SYSLOG = false
.endif
.endif

.if ${MAKESUB}=="security/krb5/comerr/"
.if ${TARGET_MACHINE} == "SVR4"
LIBS                    = -ll -lmalloc
.endif
.endif

.if ${MAKESUB}=="security/krb5/lib/kdb/"
RIOS_CFLAGS	?= -Dunix -D_BSD

.endif

.if ${MAKESUB}=="security/krb5/kdc/"
RIOS_CFLAGS	?= -Dunix -D_BSD

.if ${TARGET_MACHINE} == "VAX"
#
# SNI_SEC_MERGE:  Check this to make sure that the "VAX" target
#			machine is the right value.  we want to
#			make sure that this gets turned on only
#			for DEC Ultrix machines and does not get
#			turned on for others.  this fragment was
#			moved from krb5/kdc/Makefile because it
#			causing the SVR4 build to die.

#
# For DEC Ultrix:
# The correct syslog.h file lives in the src tree (I don't think it
# should be exported).  However, the export area must be searched first
# to make sure we get the real versions of the generated XXX_err.h files.
# To make the search happen the right way, we must modify INCDIRS.
#
XX != genpath -I../include
INCDIRS := ${INCDIRS} ${XX}
.endif

.endif

.if ${MAKESUB}=="security/krb5/clients/klist/"
PMAX_LIBS       ?= -lbsd
RIOS_CFLAGS	?= -Dunix -D_BSD
RIOS_LIBS	?= -lbsd -lkrb5
MIPS_LIBS	?= -li
VAX_LIBS	?= -li
APOLLO68K_LIBS  ?= -lintl
.ifdef DES_HIDDEN
klist_LIBS	= ../../../../libdce/bound_dce_nostrip.o ${LIBS}
.endif
.endif

.if ${MAKESUB}=="security/krb5/clients/kinit/"
PMAX_LIBS       ?= -lbsd
RIOS_CFLAGS	?= -Dunix -D_BSD
RIOS_LIBS	?= -lbsd -lkrb5
MIPS_LIBS	?= -li
VAX_LIBS	?= -li
APOLLO68K_LIBS  ?= -lintl
.ifdef DES_HIDDEN
kinit_LIBS	= ../../../../libdce/bound_dce_nostrip.o ${LIBS}
.endif
.endif

.if ${MAKESUB}=="security/krb5/clients/kdestroy/"
PMAX_LIBS       ?= -lbsd
RIOS_CFLAGS	?= -Dunix -D_BSD
RIOS_LIBS	?= -lbsd -lkrb5
MIPS_LIBS	?= -li
VAX_LIBS	?= -li
APOLLO68K_LIBS  ?= -lintl
.ifdef DES_HIDDEN
kdestroy_LIBS	= ../../../../libdce/bound_dce_nostrip.o ${LIBS}
.endif
.endif

.if ${MAKESUB}=="security/client/dce_login/"
PMAX_LIBS       ?= -lbsd
RIOS_CFLAGS	?= -Dunix 
RIOS_LIBS	?= -lbsd -lkrb5
MIPS_LIBS	?= -li
VAX_LIBS	?= -li
APOLLO68K_LIBS  ?= -lintl
.endif

.if ${MAKESUB}=="security/client/rca/"
RIOS_CFLAGS	?= -Dunix
.endif

.if ${MAKESUB}=="security/client/admin/rgy_edit/"
PMAX_LIBS       ?= -lbsd
RIOS_CFLAGS	?= -Dunix -D_NO_PROTO
RIOS_LIBS	?= -lbsd -lkrb5
MIPS_LIBS	?= -li
VAX_LIBS	?= -li
APOLLO68K_LIBS  ?= -lintl
.endif

.if ${MAKESUB}=="security/client/admin/acl_edit/svr_example/"
PMAX_LIBS       ?= -lbsd
RIOS_CFLAGS	?= -Dunix 
RIOS_LIBS	?= -lbsd -lkrb5
MIPS_LIBS	?= -li
VAX_LIBS	?= -li
APOLLO68K_LIBS  ?= -lintl
.endif

.if ${MAKESUB}=="security/client/admin/passwd_export/"
PMAX_LIBS       ?= -lbsd
RIOS_CFLAGS	?= -Dunix -D_NO_PROTO
RIOS_LIBS	?= -lbsd
MIPS_LIBS	?= -li
VAX_LIBS	?= -li
APOLLO68K_LIBS  ?= -lintl
.endif

.if ${MAKESUB}=="security/client/admin/passwd_import/"
PMAX_LIBS       ?= -lbsd
RIOS_CFLAGS	?= -Dunix -D_NO_PROTO
RIOS_LIBS	?= -lbsd -lkrb5
MIPS_LIBS	?= -li
VAX_LIBS	?= -li
APOLLO68K_LIBS  ?= -lintl
.endif

.if ${MAKESUB}=="security/client/admin/acl_edit/"
PMAX_LIBS       ?= -lbsd
RIOS_CFLAGS	?= -Dunix 
RIOS_LIBS	?= -lbsd -lkrb5
MIPS_LIBS	?= -li
VAX_LIBS	?= -li
APOLLO68K_LIBS  ?= -lintl
.endif

.if ${MAKESUB}=="security/client/admin/sec_admin/"
PMAX_LIBS       ?= -lbsd
RIOS_CFLAGS     ?= -Dunix 
RIOS_LIBS       ?= -lbsd -lkrb5
MIPS_LIBS       ?= -li
VAX_LIBS        ?= -li
APOLLO68K_LIBS  ?= -lintl
.endif

.if ${MAKESUB}=="security/helper/"
PMAX_LIBS       ?= -lbsd
RIOS_CFLAGS	?= -Dunix 
RIOS_LIBS	?= -lbsd
MIPS_LIBS	?= -li
VAX_LIBS	?= -li
APOLLO68K_LIBS  ?= -lintl
.endif

.if ${MAKESUB}=="security/client/acl/"
RIOS_CFLAGS	?= -Dunix 
.endif

.ifndef USE_DES
SECURITY_CFLAGS += -DDES_NEUTER
.endif

# added capabalities for security component to compile with
# the -g option 
.ifdef SEC_OPT_LEVEL 
# Only the HP platform's loader recognizes the -g switch
.if ${TARGET_MACHINE} == "HP800"
OPT_LEVEL=${SEC_OPT_LEVEL}
.else
CC_OPT_LEVEL=${SEC_OPT_LEVEL}
.endif
.endif


PMAX_CFLAGS += -DOSF -DHAS_STDLIB_H -DBERKELEY
AT386_CFLAGS += -DOSF -DHAS_STDLIB_H -DBERKELEY
APOLLO68K_CFLAGS += -W0,-std,-pic,-info,3 -DHAS_STDLIB_H
HP_PA_CFLAGS += -Dunix -DHAS_STDLIB_H -DOSF -D_REENTRANT -D_THREAD_SAFE -D__hp_osf -q no_use_dff -DUSE_GETPW_R
HP_PA_LIBS += -L/usr/lib -lpthreads -lmach -lc_r -lc
RIOS_CFLAGS += -DHAS_STDLIB_H -DPOSIX_FILE_LOCKS
VAX_CFLAGS += -DHAS_STDLIB_H 
MIPS_CFLAGS += -DHAS_STDLIB_H 
HP800_CFLAGS += -DPOSIX_FILE_LOCKS -DPOSIX_TERMIOS
.if (REENTRANT_CLIB)
HP800_CFLAGS += -DUSE_GETPW_R
.endif
.endif
