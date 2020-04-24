#
# @OSF_FREE_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1993, 1992, 1991, 1990, 1996 Open Software Foundation, Inc.
# Permission is hereby granted to use, copy, modify and freely distribute
# the software in this file and its documentation for any purpose without
# fee, provided that the above copyright notice appears in all copies and
# that both the copyright notice and this permission notice appear in
# supporting documentation.  Further, provided that the name of Open
# Software Foundation, Inc. ("OSF") not be used in advertising or
# publicity pertaining to distribution of the software without prior
# written permission from OSF.  OSF makes no representations about the
# suitability of this software for any purpose.  It is provided "as is"
# without express or implied warranty.
#
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
# All Rights Reserved
#
#
# HISTORY
# $Log: osf.std.mk,v $
# Revision 1.2.64.3  1996/10/03  14:37:12  arvind
# 	Revised link-export for clearcase; this makes all links in one command
# 	and arranges for the link to *not* depend on the source file.
# 	[1996/09/17  03:13 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
#
# 	Merge from sommerfeld_pk_kdc to aha_pk9
# 	[1996/08/13  21:33 UTC  aha  /main/DCE_1.2.2/aha_pk9/1]
#
# 	fix LINK_STATIC (along with change in Makeconf)
# 	[1996/08/09  18:30 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc/1]
#
# 	Fix checking out of tools under ClearCase to 1.2 rules.
# 	[1996/03/07  01:01 UTC  psn  /main/DCE_1.2/5]
#
# 	Fix checking out of tools under ClearCase to 1.2 rules.
# 	[1996/03/06  21:00 UTC  psn  /main/DCE_1.2/4]
#
# 	Fix checking out of tools under ClearCase to 1.2 rules.
# 	[1996/03/06  20:14 UTC  psn  /main/DCE_1.2/3]
#
# 	Add the checking out of tools under ClearCase to 1.2 rules.
# 	[1996/03/06  19:48 UTC  psn  /main/DCE_1.2/2]
#
# 	Add rules for checking out the tools when built.
# 	[1996/03/06  19:46 UTC  psn  /main/DCE_1.2/psn_dce1.2_krb5/1]
#
# 	Update OSF copyright year
# 	[1996/03/09  22:41:06  marty]
#
# 	Submit OSF/DCE 1.2.1
# 	HP 	[1995/11/21  21:05 UTC  jrr  /main/jrr_1.2_mothra/1]
# 	[1996/10/03  13:21:21  arvind]
#
# Revision 1.2.64.2  1996/09/09  20:58:30  bfc
# 	Fix IDLFLAGS to look for imported .idl files in the corresponding
# 	source tree, not just the object tree.
# 	[1996/09/09  20:56:01  bfc]
# 
# Revision 1.2.64.1  1996/05/10  13:08:01  arvind
# 	Drop 1 of DCE 1.2.2 to OSF
# 
# 	HP revision /main/DCE_1.2/5  1996/03/07  01:01 UTC  psn
# 	Fix checking out of tools under ClearCase to 1.2 rules.
# 
# 	HP revision /main/DCE_1.2/4  1996/03/06  21:00 UTC  psn
# 	Fix checking out of tools under ClearCase to 1.2 rules.
# 
# 	HP revision /main/DCE_1.2/3  1996/03/06  20:14 UTC  psn
# 	Fix checking out of tools under ClearCase to 1.2 rules.
# 
# 	HP revision /main/DCE_1.2/2  1996/03/06  19:48 UTC  psn
# 	Add the checking out of tools under ClearCase to 1.2 rules.
# 
# 	HP revision /main/DCE_1.2/psn_dce1.2_krb5/1  1996/03/06  19:46 UTC  psn
# 	Add rules for checking out the tools when built.
# 
# 	HP revision /main/DCE_1.2/1  1996/01/03  18:49 UTC  psn
# 	Merge second XIDL drop for DCE 1.2.1
# 	[1995/11/17  17:00 UTC  dat  /main/dat_xidl2/1]
# 
# 	Remove -R flag for linking of shared libraries
# 	[1995/02/22  20:02:54  bfc]
# 	#
# 
# 	Add support for C++
# 	[1994/12/09  18:06:45  acf]
# 
# 	Add definition ALPHA_STRICT_COMPILER_WARNINGS
# 	[1994/10/28  20:44:03  acf]
# 
# 	DCE for DEC OSF/1 AXP: populate from OSF DCE R1.1
# 	[1994/10/28  20:43:03  acf]
# 
# Revision 1.2.45.14  1994/08/29  19:18:06  marty
# 	CR # 11454 - Remove extraneous echo statements
# 	[1994/08/29  15:35:13  marty]
# 
# Revision 1.2.45.13  1994/07/26  20:52:03  annie
# 	added VISTA - 11163
# 	[1994/07/26  20:51:32  annie]
# 
# Revision 1.2.45.12  1994/07/13  21:29:57  devsrc
# 	merged with DCE1.1
# 	[1994/07/11  15:17:25  devsrc]
# 
# 	merged with dce1.1 bl-10
# 	[1994/06/29  12:41:59  annie]
# 
# Revision 1.2.45.11  1994/05/23  23:13:26  sommerfeld
# 	back out the .tryinclude change.
# 	[1994/05/23  19:28:40  sommerfeld]
# 
# Revision 1.2.45.10  1994/05/20  22:03:32  sommerfeld
# 	Change include->tryinclude.
# 	[1994/05/18  19:36:58  sommerfeld]
# 
# 	SAMS cleanups (one target at a time)
# 	IDL cleanups (also one target at a time)
# 	Clearcasified export-via-symlinks
# 	local/global_args.mk overrides
# 	Generate stubbed _[cs]aux.c files
# 	[1994/05/17  18:59:16  sommerfeld]
# 
# Revision 1.2.45.9  1994/05/10  16:01:56  rsalz
# 	Remove +w1 from RIOS_STRICT_COMPILER_WARNINGS (OR CR 10594).
# 	[1994/05/10  14:51:27  rsalz]
# 
# Revision 1.2.45.8  1994/05/09  17:59:17  rsalz
# 	Add STRICT_COMPILER_WARNINGS variable (OT CR 10581).
# 	[1994/05/09  17:20:01  rsalz]
# 
# Revision 1.2.45.7  1994/04/26  20:19:48  rsalz
# 	Change IDL invocation so all flags come first (OT CR 10462)
# 	[1994/04/26  19:04:06  rsalz]
# 
# Revision 1.2.45.11  1994/05/23  23:13:26  sommerfeld
# 	back out the .tryinclude change.
# 	[1994/05/23  19:28:40  sommerfeld]
# 
# Revision 1.2.45.10  1994/05/20  22:03:32  sommerfeld
# 	Change include->tryinclude.
# 	[1994/05/18  19:36:58  sommerfeld]
# 
# 	SAMS cleanups (one target at a time)
# 	IDL cleanups (also one target at a time)
# 	Clearcasified export-via-symlinks
# 	local/global_args.mk overrides
# 	Generate stubbed _[cs]aux.c files
# 	[1994/05/17  18:59:16  sommerfeld]
# 
# Revision 1.2.45.9  1994/05/10  16:01:56  rsalz
# 	Remove +w1 from RIOS_STRICT_COMPILER_WARNINGS (OR CR 10594).
# 	[1994/05/10  14:51:27  rsalz]
# 
# Revision 1.2.45.8  1994/05/09  17:59:17  rsalz
# 	Add STRICT_COMPILER_WARNINGS variable (OT CR 10581).
# 	[1994/05/09  17:20:01  rsalz]
# 
# Revision 1.2.45.7  1994/04/26  20:19:48  rsalz
# 	Change IDL invocation so all flags come first (OT CR 10462)
# 	[1994/04/26  19:04:06  rsalz]
# 
# Revision 1.2.45.6  1994/02/22  17:13:12  rsalz
# 	Enable code to allow EXPORT_USING_CP to work (OT CR 5095).
# 	[1994/02/22  17:10:29  rsalz]
# 
# Revision 1.2.45.5  1993/12/13  21:31:45  mario
# 	replaced spaces with tab
# 	[1993/12/13  21:31:25  mario]
# 
# Revision 1.2.45.4  1993/12/13  14:39:16  mario
# 	Minor cosmetic changes
# 	[1993/12/13  14:38:41  mario]
# 
# Revision 1.2.45.3  1993/12/07  17:54:35  mario
# 	Changed for ode2.3 - < and > becomes this except for RULES_MK
# 	[1993/12/07  17:25:23  mario]
# 
# Revision 1.2.36.6  1993/10/26  15:52:43  rousseau
# 	Changed sams -omc rule to recognize CMP.cat dependencies.
# 	[1993/10/26  15:52:31  rousseau]
# 
# Revision 1.2.39.1  1993/09/21  21:38:29  tom
# 	Replace single $ with double $.
# 	[1993/09/21  20:48:53  tom]
# 
# Revision 1.2.36.5  1993/07/30  18:44:41  zeliff
# 	Merging dce1.0.3i and dce1.1i versions for dce1.1i
# 	[1993/07/30  18:44:19  zeliff]
# 
# Revision 1.2.34.3  1993/08/20  14:08:05  cjd
# 	took out xlc optimization
# 	[1993/08/20  14:07:40  cjd]
# 
# Revision 1.2.34.2  1993/06/10  19:19:49  sommerfeld
# 	Remove _COMMON_LDFLAGS_ from _SHLDFLAGS_ by default.
# 	[1993/04/05  17:02:18  sommerfe]
# 
# 	Merge in HP changes.
# 	[1993/04/02  16:52:02  sommerfe]
# 
# 	_[cs]stub.c generation depends on .h because .o generated from them does.
# 	clean up stub generation so that we don't confuse clearcase.
# 	idl should really be able to deal with -header /dev/null or -no_header
# 	or soemthing; currently -header /dev/null attempts to create /dev/null.h!!!
# 	[1993/04/02  19:15:20  sommerfeld]
# 
# Revision 1.2.13.10  1993/02/03  17:52:33  john
# 	Added the _PP_CCFLAGS_ variable which will be used in osf.obj.mk in the
# 	preprocessing section.
# 	[1993/02/03  15:47:27  john]
# 
# Revision 1.2.13.9  1992/12/31  16:48:07  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:52:18  zeliff]
# 
# Revision 1.2.13.8  1992/12/21  17:33:12  john
# 	Added code to allow seperate compiler and linkers.  If the CC_LD flag is set its value
# 	will be used as the linker instead of the value of _CC_.
# 	[1992/12/21  15:29:07  john]
# 
# Revision 1.2.13.7  1992/12/14  21:44:35  john
# 	Fixed the test for running the install pass to not use TOSTAGE.
# 	[1992/12/14  21:43:57  john]
# 
# Revision 1.2.13.6  1992/12/10  17:24:52  marty
# 	Added osc1.1.1 upgrade changes (mkcatdefs instead of xmkcatdefs).
# 	Also added fix for OT#5098 (install pass rebuilds unbuilt objects).
# 	[1992/12/03  16:36:20  marty]
# 
# Revision 1.2.13.4  1992/11/14  21:52:53  john
# 	Fixed the default for MB to -all -file .depend.
# 	[1992/11/14  21:52:14  john]
# 
# Revision 1.2.13.3  1992/11/06  21:25:42  john
# 	Modified the setting for DEFMDFLAGS.
# 	[1992/10/20  01:25:04  john]
# 
# Revision 1.2.13.2  1992/09/29  20:02:34  devsrc
# 	[OT 5373]  SNI/SVR4 Merge UPDATE: Moved  the line "PARSER_AID?=parser_aid"
# 	           to osf.dce.mk
# 	[1992/09/25  14:27:33  sekhar]
# 
# 	Removed MAVROS, MAVCOD, MSG, PRS from osf.std.mk since it is in
# 	osf.dce.mk.
# 	[1992/09/19  01:47:25  john]
# 
# 	Redo ci to allow bsubmit.
# 	[1992/09/19  00:59:01  john]
# 
# 	Add various cds command variable definitions; add EXPORT_SYMLINK rule; add
# 	SHLIBFLAGS that can be set for entire build
# 	[1992/09/11  15:20:56  rec]
# 
# Revision 1.2.5.8  1992/05/22  19:54:11  zeliff
# 	Defect 3683.  Add ${${TARGET_MACHINE}_IDLFLAGS} to _IDLFLAGS_.
# 	[1992/05/22  14:38:55  zeliff]
# 
# Revision 1.2.5.7  1992/05/12  17:30:45  mhickey
# 	Added targets and lists for the clobber_exp[orts] pass.
# 
# 	Defect 175.
# 	[1992/05/12  17:28:13  mhickey]
# 
# Revision 1.2.5.6  1992/05/01  13:06:03  john
# 	Removed the cp -pr for the RIOS.  Went back to exporting using
# 	tar.
# 	[1992/05/01  13:05:03  john]
# 
# Revision 1.2.5.5  1992/04/30  22:36:36  john
# 	Added the SETUP pass for building all DCE tools
# 	[1992/04/30  22:05:21  john]
# 
# 	Merged 1.2.5.2 into 1.2.4.4 to incorporate idl changes.
# 	[1992/04/14  14:48:20  john]
# 
# 	Converted all setup pass references of variable TOOLS to DEFTOOLBASE
# 	so that the local tools will be used before the server tools.
# 	[1992/04/14  13:49:08  john]
# 
# 	Added .if to use tar only if EXPORT_USING_TAR is defined; otherwise
# 	cp -p -R will be used.
# 	[1992/04/13  20:04:18  john]
# 
# 	Added the SETUP pass.
# 	[1992/04/10  14:26:24  john]
# 
# Revision 1.2.5.4  1992/04/16  14:42:07  marty
# 	Fix copyright marker and hardcoded copyright notices
# 	[1992/04/16  14:39:28  marty]
# 
# Revision 1.2.5.3  1992/04/14  20:19:58  marty
# 	Corrected context in which WRITEABLE_STRINGS variable was used in
# 	".if defined()" to WRITABLE_STRINGS_CC,  WRITABLE_STRINGS_LD,
# 	WRITABLE_STRINGS_AR,  and WRITABLE_STRINGS_RANLIB.
# 	[1992/04/14  20:19:29  marty]
# 
# Revision 1.2.5.2  1992/04/07  19:17:14  mckeen
# 	Moved definition of IDL to osf.dce.std.mk
# 	[1992/03/13  15:40:51  mckeen]
# 
# Revision 1.2  1992/01/19  22:13:22  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#

.if !defined(_OSF_STD_MK_)
_OSF_STD_MK_=

#
#  Default rule - All other rules appear after variable definitions
#
_default: build_all

#
#  Debugging entry for checking environment
#
print_env:
	@printenv

# semi-permanent local/global overrides for make options
# it is indended that these be created in sandboxes and never actually 
# submitted to the mainline; these are there to allow changes to command 
# line arguments without either (a) hacking existing makefiles, or 
# (b) putting these overrides in, either on the command line or in 
# the process environment.

.if exists(${MAKETOP}/global_args.mk)
.include "${MAKETOP}/global_args.mk"
.endif
.if exists(local_args.mk)
.include "local_args.mk"
.endif
#
#  Use this as a dependency for any rule which should always be triggered
#  (e.g. recursive makes).
#
ALWAYS=_ALWAYS_

#
#  Shortened for macro definitions, not to be used within a Makefile.
#
_T_M_=${TARGET_MACHINE}

#
#  Definitions for object file format - A_OUT, COFF or MACHO
#
${_T_M_}_OBJECT_FORMAT?=MACHO
OBJECT_FORMAT?=${${_T_M_}_OBJECT_FORMAT}

#
#  Definitions for archive file format - ASCARCH or COFF
#
${_T_M_}_ARCHIVE_FORMAT?=COFF
ARCHIVE_FORMAT?=${${_T_M_}_ARCHIVE_FORMAT}

#
#  Set defaults for input variables which are not already defined
#
DEF_CPFLAGS?=-p -r
DEF_RMFLAGS?=-f
DEF_ARFLAGS?=crl
DEF_MDFLAGS?=${MDFLAGS:U-all -file .depend}

ROOT_OWNER?=root
KMEM_GROUP?=kmem
TTY_GROUP?=tty
SETUID_MODE?=4711
SETGID_MODE?=2711

OWNER?=bin
IOWNER?=${OWNER}
GROUP?=bin
IGROUP?=${GROUP}
IMODE?=755

#
#  Program macros
#
AR?=whatar
AS?=as
AWK?=awk
CC?=whatcc
CHMOD?=chmod
CP?=cp
CTAB?=ctab
ECHO?=echo
GENCAT?=gencat
GENPATH?=genpath
LD?=whatld
LEX?=lex
LIBLOC?=libloc
LINT?=lint
LN?=ln
MAKEPATH?=makepath
MD?=md
MIG?=mig
MKCATDEFS?=mkcatdefs
MV?=mv
RANLIB?=whatranlib
RELEASE?=release
RM?=rm
SED?=sed
SORT?=sort
TAGS?=ctags
TAR?=tar
TOUCH?=touch
TR?=tr
XSTR?=xstr
YACC?=yacc
STATMET?=covcc

#
# DCE QA requirement for lint is with the -MA switch.
#
STATMETFLAGS=-thm
CLEARTOOL?=/usr/atria/bin/cleartool
CLEARAUDIT?=/usr/atria/bin/clearaudit
UNIFDEF?=unifdef

#
#  Define ${_T_M_}_VA_ARGV to be either VA_ARGV_IS_RECAST
#  to recast to char **, otherwise define VA_ARGV_IS_ROUTINE
#  If not defined here, we become VA_ARGV_UNKNOWN which should invoke
#  a #error directive where needed.
#
HP_M68K_VA_ARGV=VA_ARGV_IS_RECAST
HP300_VA_ARGV=VA_ARGV_IS_RECAST
IBMRT_VA_ARGV=VA_ARGV_IS_RECAST
MACII_VA_ARGV=VA_ARGV_IS_RECAST
MMAX_VA_ARGV=VA_ARGV_IS_RECAST
PMAX_VA_ARGV=VA_ARGV_IS_RECAST
SUN3_VA_ARGV=VA_ARGV_IS_RECAST
SUN4_VA_ARGV=VA_ARGV_IS_RECAST
SUN_VA_ARGV=VA_ARGV_IS_RECAST
VAX_VA_ARGV=VA_ARGV_IS_RECAST
AT386_VA_ARGV=VA_ARGV_IS_RECAST
RIOS_VA_ARGV=VA_ARGV_IS_RECAST
APOLLO68K_VA_ARGV=VA_ARGV_IS_RECAST
APOLLO88K_VA_ARGV=VA_ARGV_IS_RECAST
${_T_M_}_VA_ARGV?=VA_ARGV_UNKNOWN

#
#  Defined whether characters are sign or zero extended
#
HP_M68K_CHAR_EXTEND=CHARS_EXTEND_ZERO
HP300_CHAR_EXTEND=CHARS_EXTEND_SIGN
IBMRT_CHAR_EXTEND=CHARS_EXTEND_ZERO
MACII_CHAR_EXTEND=CHARS_EXTEND_SIGN
MMAX_CHAR_EXTEND=CHARS_EXTEND_ZERO
PMAX_CHAR_EXTEND=CHARS_EXTEND_ZERO
SUN3_CHAR_EXTEND=CHARS_EXTEND_SIGN
SUN4_CHAR_EXTEND=CHARS_EXTEND_SIGN
SUN_CHAR_EXTEND=CHARS_EXTEND_SIGN
VAX_CHAR_EXTEND=CHARS_EXTEND_SIGN
AT386_CHAR_EXTEND=CHARS_EXTEND_SIGN
RIOS_CHAR_EXTEND=CHARS_EXTEND_ZERO
APOLLO68K_CHAR_EXTEND=CHARS_EXTEND_ZERO
APOLLO88K_CHAR_EXTEND=CHARS_EXTEND_ZERO
${_T_M_}_CHAR_EXTEND?=CHARS_EXTEND_UNKNOWN

#
#  Include project specific information
#
.include <osf.${project_name}.mk>

#
#  C compiler variations
#
CCTYPE?=ansi
_CCTYPE_=${${.TARGET}_CCTYPE:U${CCTYPE}}

.if defined(CPLUSPLUS_CC)
_cplusplus_CC_=${CPLUSPLUS_CC}
.else
_cplusplus_CC_=CC
.endif
.if defined(HOST_CC)
_host_CC_=${HOST_CC}
.else
_host_CC_=cc
.endif
.if defined(ANSI_CC)
_ansi_CC_=${ANSI_CC}
.else
_ansi_CC_=cc
.endif
.if defined(TRADITIONAL_CC)
_traditional_CC_=${TRADITIONAL_CC}
.else
_traditional_CC_=cc
.endif
.if defined(WRITABLE_STRINGS_CC)
_writable_strings_CC_=${WRITABLE_STRINGS_CC}
.else
_writable_strings_CC_=cc
.endif
_CC_=${_${_CCTYPE_}_CC_}

.if defined(CC_LD)
_CC_LD_=${CC_LD}
.else
_CC_LD_=${_CC_}
.endif

.if defined(CPLUSPLUS_LD)
_cplusplus_LD_=$(CPLUSPLUS_LD)
.else
_cplusplus_LD_=ld
.endif
.if defined(HOST_LD)
_host_LD_=${HOST_LD}
.else
_host_LD_=ld
.endif
.if defined(ANSI_LD)
_ansi_LD_=${ANSI_LD}
.else
_ansi_LD_=ld
.endif
.if defined(TRADITIONAL_LD)
_traditional_LD_=${TRADITIONAL_LD}
.else
_traditional_LD_=ld
.endif
.if defined(WRITABLE_STRINGS_LD)
_writable_strings_LD_=${WRITABLE_STRINGS_LD}
.else
_writable_strings_LD_=ld
.endif
_LD_=${_${_CCTYPE_}_LD_}

.if defined(CPLUSPLUS_AR)
_cplusplus_AR_=${CPLUSPLUS_AR}
.else
_cplusplus_AR_=ar
.endif
.if defined(HOST_AR)
_host_AR_=${HOST_AR}
.else
_host_AR_=ar
.endif
.if defined(ANSI_AR)
_ansi_AR_=${ANSI_AR}
.else
_ansi_AR_?=ar
.endif
.if defined(TRADITIONAL_AR)
_traditional_AR_=${TRADITIONAL_AR}
.else
_traditional_AR_=ar
.endif
.if defined(WRITABLE_STRINGS_AR)
_writable_strings_AR_=${WRITABLE_STRINGS_AR}
.else
_writable_strings_AR_=ar
.endif
_AR_=${_${_CCTYPE_}_AR_}

.if defined(CPLUSPLUS_RANLIB)
_cplusplus_RANLIB_=${CPLUSPLUS_RANLIB}
.else
_cplusplus_RANLIB_=ranlib
.endif
.if defined(HOST_RANLIB)
_host_RANLIB_=${HOST_RANLIB}
.else
_host_RANLIB_=ranlib
.endif
.if defined(ANSI_RANLIB)
_ansi_RANLIB_=${ANSI_RANLIB}
.else
_ansi_RANLIB_?=ranlib
.endif
.if defined(TRADITIONAL_RANLIB)
_traditional_RANLIB_=${TRADITIONAL_RANLIB}
.else
_traditional_RANLIB_=ranlib
.endif
.if defined(WRITABLE_STRINGS_RANLIB)
_writable_strings_RANLIB_=${WRITABLE_STRINGS_RANLIB}
.else
_writable_strings_RANLIB_=ranlib
.endif
_RANLIB_=${_${_CCTYPE_}_RANLIB_}

# Stanza to set string warnings; by machine type.
HP800_STRICT_COMPILER_WARNINGS = +w1
AT386_STRICT_COMPILER_WARNINGS = -pedantic -Wchar-subscripts -Wcomment \
				-Wformat -Wid-clash-32 -Wimplicit \
				-Wmissing-prototypes -Wswitch -Wreturn-type \
				-Wstrict-prototypes -Wtraditional -Wunused
RIOS_STRICT_COMPILER_WARNINGS =
ALPHA_STRICT_COMPILER_WARNINGS = -std1
${_T_M_}_STRICT_COMPILER_WARNINGS?=
STRICT_COMPILER_WARNINGS?=${${_T_M_}_STRICT_COMPILER_WARNINGS}

.if defined (HOST_CFLAGS)
_host_CFLAGS_=${HOST_CFLAGS}
.else
_host_CFLAGS_=
.endif
_ansi_CFLAGS_=${_O_F_CFLAGS_} ${_CCDEFS_} ${_SHCCDEFS_}
_traditional_CFLAGS_=${_ansi_CFLAGS_}
_writable_strings_CFLAGS_=${_ansi_CFLAGS_}
_cplusplus_CFLAGS_=${_ansi_CFLAGS_}
_CC_CFLAGS_=${_${_CCTYPE_}_CFLAGS_}

.if defined (HOST_INCDIRS)
_host_INCDIRS_=${HOST_INCDIRS}
.else
_host_INCDIRS_=
.endif

# EXT_INCDIRS may be set in the rc files for "external" (e.g., outside 
# the main source tree) components.

_ansi_INCDIRS_=${INCDIRS} ${EXT_INCDIRS}
_traditional_INCDIRS_=${INCDIRS} ${EXT_INCDIRS}
_writable_strings_INCDIRS_=${INCDIRS} ${EXT_INCDIRS}
_cplusplus_INCDIRS_=${INCDIRS} ${EXT_INCDIRS}
_CC_INCDIRS_=${_${_CCTYPE_}_INCDIRS_}

.if defined(HOST_LDFLAGS)
_host_LDFLAGS_=${HOST_LDFLAGS}
.else
_host_LDFLAGS_=
.endif
.if defined(ANSI_LDFLAGS)
_ansi_LDFLAGS_=${ANSI_LDFLAGS}
.else
_ansi_LDFLAGS_=
.endif
.if defined(TRADITIONAL_LDFLAGS)
_traditional_LDFLAGS_=${TRADITIONAL_LDFLAGS}
.else
_traditional_LDFLAGS_=
.endif
.if defined(WRITABLE_STRINGS_LDFLAGS)
_writable_strings_LDFLAGS_=${WRITABLE_STRINGS_LDFLAGS}
.else
_writable_strings_LDFLAGS_=
.endif
.if defined(CPLUSPLUS_LDFLAGS)
_cplusplus_LDFLAGS_=${CPLUSPLUS_LDFLAGS}
.else
_cplusplus_LDFLAGS_=
.endif

_CC_LDFLAGS_=${_${_CCTYPE_}_LDFLAGS_}

_host_LIBDIRS_=
.if ${USE_SHARED_LIBRARIES}
_ansi_LIBDIRS_=${SHLIBDIRS} ${LIBDIRS} ${EXT_SHLIBDIRS} ${EXT_LIBDIRS}
.else
_ansi_LIBDIRS_=${LIBDIRS} ${EXT_LIBDIRS}
.endif
_traditional_LIBDIRS_=${_ansi_LIBDIRS_}
_writable_strings_LIBDIRS_=${_ansi_LIBDIRS_}
_cplusplus_LIBDIRS_=${_ansi_LIBDIRS_}
_CC_LIBDIRS_=${_${_CCTYPE_}_LIBDIRS_}

#
#  Compilation optimization level.  This should be set to whatever
#  combination of -O and -g flags you desire.  Defaults to -O.
#
#  Allow these flags to be overridden per target
#
.if defined(OPT_LEVEL)
_CC_OL_=${${.TARGET}_OPT_LEVEL:U${OPT_LEVEL}}
_LD_OL_=${${.TARGET}_OPT_LEVEL:U${OPT_LEVEL}}
.else
.if defined(DEBUG_ON)
CC_OPT_LEVEL?=-g
.else
CC_OPT_LEVEL?=-O
.endif
_CC_OL_=${${.TARGET}_OPT_LEVEL:U${${.TARGET}_CC_OPT_LEVEL:U${CC_OPT_LEVEL}}}
LD_OPT_LEVEL?=
_LD_OL_=${${.TARGET}_OPT_LEVEL:U${${.TARGET}_LD_OPT_LEVEL:U${LD_OPT_LEVEL}}}
.endif

#
#  Program flags for makefile, environment and command line args
#
_INCFLAGS_=\
	${${.TARGET}_INCARGS:U${INCARGS}}\
	${${.TARGET}_INCFLAGS:U${INCFLAGS}}\
	${${.TARGET}_INCENV:U${INCENV}}
_GENINC_=\
	${_CC_GENINC_} ${_INCFLAGS_:!${GENPATH} ${_INCFLAGS_}!}
_LIBFLAGS_=\
	${${.TARGET}_LIBARGS:U${LIBARGS}}\
	${${.TARGET}_LIBFLAGS:U${LIBFLAGS}}\
	${${.TARGET}_LIBENV:U${LIBENV}}
_GENLIB_=\
	${_CC_GENLIB_} ${_LIBFLAGS_:!${GENPATH} ${_LIBFLAGS_}!}
_LIBS_=	${${.TARGET}_LIBSENV:U${LIBSENV}}\
	${${.TARGET}_LIBS:U${LIBS}}\
	${${.TARGET}_LIBSARGS:U${LIBSARGS}} ${TARGET_LIBS}
_CCPLUSPLUSFLAGS_=\
	${_CC_OL_}\
	${${.TARGET}_CENV:U${CENV}}\
	${${.TARGET}_CFLAGS:U${CFLAGS}} ${TARGET_FLAGS}\
	${${.TARGET}_CARGS:U${CARGS}}\
	${_GENINC_} ${_CC_INCDIRS_}
_CCFLAGS_=\
	${_CC_CFLAGS_}\
	${_CC_OL_}\
	${${.TARGET}_CENV:U${CENV}}\
	${${.TARGET}_CFLAGS:U${CFLAGS}} ${TARGET_FLAGS}\
	${${.TARGET}_CARGS:U${CARGS}}\
	${_CC_NOSTDINC_} ${_GENINC_} ${_CC_INCDIRS_} ${_CC_PICLIB_}
# this variable is used when generating pre-processor files.  It will use
# the flags specified by ${.TARGET.o_*} instead of adding ${.TARGET.pp_*} to
# the individual Makefiles.
_PP_CCFLAGS_=\
	${_CC_CFLAGS_}\
	${_CC_OL_}\
	${${.TARGET:.pp=.o}_CENV:U${CENV}}\
	${${.TARGET:.pp=.o}_CFLAGS:U${CFLAGS}} ${TARGET_FLAGS}\
	${${.TARGET:.pp=.o}_CARGS:U${CARGS}}\
	${_CC_NOSTDINC_} ${_GENINC_} ${_CC_INCDIRS_} ${_CC_PICLIB_}
_COMMON_LDFLAGS_=\
	${_CC_LDFLAGS_}\
	${_LD_OL_}\
	${${.TARGET}_LDENV:U${LDENV}}\
	${${.TARGET}_LDFLAGS:U${LDFLAGS}} ${TARGET_LDFLAGS}\
	${${.TARGET}_LDARGS:U${LDARGS}}\
	${_CC_NOSTDLIB_} ${_GENLIB_} ${_CC_LIBDIRS_}
_LDFLAGS_=\
	${_CC_GLUE_} ${_COMMON_LDFLAGS_}

.if ${TARGET_OS} == "HPUX"
_SHLDFLAGS_=-b ${_CC_LIBDIRS_}
.if defined(LINK_STATIC)
TARGET_LINKSTYLE=STATIC
.else
TARGET_LINKSTYLE=DYNAMIC
.endif
.else
# OSF/1
_SHLDFLAGS_=\
	${${.TARGET}_SHLDFLAGS:U${SHLDFLAGS}} ${EXPORTS} ${_COMMON_LDFLAGS_}
.endif

_LFLAGS_=\
	${${.TARGET}_LENV:U${LENV}}\
	${${.TARGET}_LFLAGS:U${LFLAGS}}\
	${${.TARGET}_LARGS:U${LARGS}}
_YFLAGS_=\
	${${.TARGET}_YENV:U${YENV}}\
	${${.TARGET}_YFLAGS:U${YFLAGS}}\
	${${.TARGET}_YARGS:U${YARGS}}
_LINTFLAGS_=\
	${${.TARGET}_LINTENV:U${LINTENV}}\
	${${.TARGET}_LINTFLAGS:U${LINTFLAGS}}\
	${${.TARGET}_LINTARGS:U${LINTARGS}}\
	${_GENINC_} ${_CC_INCDIRS_}
#
#  STATMET flags
#
_STATMETFLAGS_=${${.TARGET}_STATMETENV:U${STATMETENV}} \
        ${${.TARGET}_STATMETFLAGS:U${STATMETFLAGS}} \
        ${${.TARGET}_STATMETARGS:U${STATMETARGS}} \
        -z ${_GCC_EXEC_PREFIX_}gcc \
        -- -B${_GCC_EXEC_PREFIX_} ${GLINE} ${_GENINC_} ${_CC_INCDIRS_}
_TAGSFLAGS_=\
	${${.TARGET}_TAGSENV:U${TAGSENV}}\
	${${.TARGET}_TAGSFLAGS:U${TAGSFLAGS}}\
	${${.TARGET}_TAGSARGS:U${TAGSARGS}}
_CPFLAGS_=\
	${${.TARGET}_CPFLAGS:U${DEF_CPFLAGS}}
_RMFLAGS_=\
	${${.TARGET}_RMFLAGS:U${DEF_RMFLAGS}}
_MIGFLAGS_=\
	-cpp ${_GCC_EXEC_PREFIX_}cpp\
	${${.TARGET}_MIGENV:U${MIGENV}}\
	${${.TARGET}_MIGFLAGS:U${MIGFLAGS}}\
	${${.TARGET}_MIGARGS:U${MIGARGS}}\
	${_CC_NOSTDINC_} ${_GENINC_} ${_CC_INCDIRS_}
_MDFLAGS_=\
	${${.TARGET}_DEF_MDFLAGS:U${DEF_MDFLAGS}} ${_GENINC_} ${_CC_INCDIRS_}
_IDLINCFLAGS_=\
	${${.TARGET}_IDLINCFLAGS} \
	${${.ALLSRC:T:M*.idl}_IDLINCARGS:U${IDLINCARGS}}\
	${${.ALLSRC:T:M*.idl}_IDLINCFLAGS:U${IDLINCFLAGS}}\
	${${.ALLSRC:T:M*.idl}_IDLINCENV:U${IDLINCENV}}
_GENIDLINC_=\
	${_IDLINCFLAGS_:!${GENPATH} ${_IDLINCFLAGS_}!}
_IDLFLAGS_=\
	${${TARGET_MACHINE}_IDLFLAGS} \
	${${.TARGET}_IDLFLAGS} \
	${${.ALLSRC:T:M*.idl}_IDLENV:U${IDLENV}}\
	${${.ALLSRC:T:M*.idl}_IDLFLAGS:U${IDLFLAGS}}\
	${${.ALLSRC:T:M*.idl}_IDLARGS:U${IDLARGS}}\
	${_GENIDLINC_} ${_CC_INCDIRS_}

# Probably won't get evaluated - no -I flags to sams
_SAMSINCFLAGS_= \
	${${.TARGET}_SAMSINCFLAGS} \
	${${.ALLSRC:T:M*.sams}_SAMSINCARGS:U${SAMSINCARGS}}\
	${${.ALLSRC:T:M*.sams}_SAMSINCFLAGS:U${SAMSINCFLAGS}}\
	${${.ALLSRC:T:M*.sams}_SAMSINCENV:U${SAMSINCENV}}
_GENSAMSINC_=\
	${_SAMSINCFLAGS_:!${GENPATH} ${_SAMSINCFLAGS_}!}
_SAMSFLAGS_=\
	${${TARGET_MACHINE}_SAMSFLAG} \
	${${.TARGET}_SAMSFLAGS}\
	${${.ALLSRC:T:M*.sams}_SAMSENV:U${SAMSENV}}\
	${${.ALLSRC:T:M*.sams}_SAMSFLAGS:U${SAMSFLAGS}}\
	${${.ALLSRC:T:M*.sams}_SAMSARGS:U${SAMARGS}}

#
#  Define these with default options added
#
_RELEASE_=${RELEASE_PREFIX}${RELEASE} ${RELEASE_OPTIONS}

#
#  Define binary targets
#
.if defined(PROGRAMS)
BINARIES+=${PROGRAMS}
.endif
.if defined(LIBRARIES)
BINARIES+=${LIBRARIES}
.endif
.if defined(SHARED_LIBRARIES)
BINARIES+=${SHARED_LIBRARIES}
.endif
.if defined(OBJECTS)
BINARIES+=${OBJECTS}
.endif

#
#  Definitions for build
#
.if defined(MIG_DEFS)
_MIG_HDRS_=${MIG_DEFS:.defs=.h}
_MIG_USRS_=${MIG_DEFS:.defs=User.c}
_MIG_SRVS_=${MIG_DEFS:.defs=Server.c}
.endif

#
#  Definitions for clean/rmtarget/clobber
#
_CLEAN_TARGET=${.TARGET:S/^clean_//}
.if !defined(CLEANFILES)
_CLEAN_DEFAULT_=\
	${_CLEAN_TARGET}.X\
	${OFILES:U${${_CLEAN_TARGET}_OFILES:U${_CLEAN_TARGET}.o}}\
	${${_CLEAN_TARGET}_GARBAGE:U${GARBAGE}}
_CLEANFILES_=\
	${CLEANFILES:U${${_CLEAN_TARGET}_CLEANFILES:U${_CLEAN_DEFAULT_}}}
.endif

_RMTARGET_TARGET=${.TARGET:S/^rmtarget_//}

_CLOBBER_TARGET=${.TARGET:S/^clobber_//}
_CLOBBER_DEFAULT_=\
	${_CLOBBER_TARGET}.X\
	${OFILES:U${${_CLOBBER_TARGET}_OFILES:U${_CLOBBER_TARGET}.o}}\
	${${_CLOBBER_TARGET}_GARBAGE:U${GARBAGE}}
_CLOBBERFILES_=${_CLOBBER_TARGET} \
	${CLEANFILES:U${${_CLOBBER_TARGET}_CLEANFILES:U${_CLOBBER_DEFAULT_}}}



#
#
#  Definitions for lint
#
_LINT_TARGET=${.TARGET:S/^lint_//}
.if !defined(LINTFILES)
_LINT_OFILES_=${OFILES:U${${_LINT_TARGET}_OFILES:U${_LINT_TARGET}.o}}
LINTFILES=${${_LINT_TARGET}_LINTFILES:U${_LINT_OFILES_:.o=.c}}
.endif

#
#  Definitions for statmet
#  STATMETFILES is a component makefile variable
#
_STATMET_TARGET=${.TARGET:S/^statmet_//}
.if !defined(STATMETFILES)
_STATMET_OFILES_=${OFILES:U${${_STATMET_TARGET}_OFILES:U${_STATMET_TARGET}.o}}
#
# Why changed the STATMETFILES definition ??  There is already a conditional
#   check earlier depending on whether STATMETFILES is defined.
#
STATMETFILES=${${_STATMET_TARGET}_STATMETFILES:U${_STATMET_OFILES_:.o=.M}}
.endif

#
#  Definitions for tags
#
_TAGS_TARGET=${.TARGET:S/^tags_//}
.if !defined(TAGSFILES)
_TAGS_OFILES_=${OFILES:U${${_TAGS_TARGET}_OFILES:U${_TAGS_TARGET}.o}}
TAGSFILES?=${${_TAGS_TARGET}_TAGSFILES:U${_TAGS_OFILES_:.o=.c}}
.endif

#
#  Definitions for setup
#
_SETUP_TARGET=${.TARGET:S/^setup_//}
_SETUPFILES_=${DEFTOOLBASE}/${_SETUP_TARGET}

#
#  Definitions for export
#
_EXPORT_TARGET=${.TARGET:S/^export_//}
_EXPDIR_=${${_EXPORT_TARGET}_EXPDIR:U${EXPDIR:U${${_EXPORT_TARGET}_IDIR:U${IDIR:U/_MISSING_EXPDIR_/}}}}
_EXPLINKS_=${${_EXPORT_TARGET}_EXPLINKS:U${EXPLINKS}}
_DO_EXPLINKS_=\
	(cd ${EXPORTBASE}${_EXPDIR_:H};\
	 ${RM} ${_RMFLAGS_} ${_EXPLINKS_}\
	 ${_EXPLINKS_:@.LINK.@; ${LN} ${_EXPORT_TARGET} ${.LINK.}@})
.if defined(EXPLINKS)
_MAKE_EXPLINKS_=${_DO_EXPLINKS_}
.else
_MAKE_EXPLINKS_=${${_EXPORT_TARGET}_EXPLINKS:U@true:D${_DO_EXPLINKS_}}
.endif
_EXPFILES_=${EXPORTBASE}${_EXPDIR_}${_EXPORT_TARGET}

#
#  Definitions for the sams idl pass
#
_SAMIDL_TARGET=${.TARGET:S/^samidl_//}

#
# Definitions for clobber export
#
_CLOBBER_EXPDIR_=${${_CLOBBER_EXP_TARGET}_EXPDIR:U${EXPDIR:U${${_CLOBBER_EXP_TARGET}_IDIR:U${IDIR:U/_MISSING_CLOBBER_EXPDIR_/}}}}
_CLOBBER_EXP_TARGET=${.TARGET:S/^clobber_exp_//}
_CLOBBER_EXP_FILES_=${EXPORTBASE}${_CLOBBER_EXPDIR_}${_CLOBBER_EXP_TARGET}

#
#  Definitions for install/release
#
_INSTALL_TARGET=${.TARGET:S/^install_//}
.if defined(TOSTAGE)

.if defined(NO_STRIP)
_NOSTRIP_=-nostrip
.else
_NOSTRIP_=${${_INSTALL_TARGET}_NOSTRIP:D-nostrip}
.endif

_IDIR_=${${_INSTALL_TARGET}_IDIR:U${IDIR:U/_MISSING_IDIR_/}}

.endif

#
#  Default single suffix compilation rules
#
.SUFFIXES:
#.if !defined(TOSTAGE)
.if empty(.TARGETS:Minstall_*)
.SUFFIXES: .o .s .pp .c .h .y .l .sh .csh .txt .uu .C .sams .msg .cat .M .cxx .hxx

.uu:
	uudecode ${.IMPSRC}
.endif


#
#  Default double suffix compilation rules
#
.if defined(MSGHDRS)
${MSGHDRS}: $${.TARGET:_msg.h=.msg}
	${MKCATDEFS} ${.TARGET:_msg.h=} ${.ALLSRC} > ${.TARGET:_msg.h=.cat.in}
	-${GENCAT} ${.TARGET:_msg.h=.cat} ${.TARGET:_msg.h=.cat.in}
	-${RM} ${_RMFLAGS_} ${.TARGET:_msg.h=.cat.in}
	-${RM} ${_RMFLAGS_} ${MSGLANGPATH}/${.TARGET:_msg.h=.cat}
	${MAKEPATH} ${MSGLANGPATH}/${.TARGET:_msg.h=.cat}
	${CP} ${.TARGET:_msg.h=.cat} ${MSGLANGPATH}/${.TARGET:_msg.h=.cat}
.endif

.if defined(CATFILES)
${CATFILES}: $${.TARGET:.cat=.msg}
	${MKCATDEFS} ${.TARGET:.cat=} ${.ALLSRC} > ${.TARGET}.in
	-${GENCAT} ${.TARGET} ${.TARGET}.in
	-${RM} ${_RMFLAGS_} ${.TARGET}.in
	-${RM} ${_RMFLAGS_} ${MSGLANGPATH}/${.TARGET}
	${MAKEPATH} ${MSGLANGPATH}/${.TARGET}
	${CP} ${.TARGET} ${MSGLANGPATH}/${.TARGET}
.endif

.if defined(CTABFILES)
${CTABFILES}: $${.TARGET:=.ctab}
	${CTAB} -i ${.ALLSRC} -o ${.TARGET}.X
	${MV} ${.TARGET}.X ${.TARGET}
.endif

.if defined(MIG_DEFS)
${_MIG_HDRS_}: $${.TARGET:.h=.defs}
	${MIG} ${_MIGFLAGS_} ${${.TARGET:.h=.defs}:P} \
		-server /dev/null -user /dev/null

${_MIG_USRS_}: $${.TARGET:S/User.c\$/.defs/}
	${MIG} ${_MIGFLAGS_} ${${.TARGET:S/User.c\$/.defs/}:P} \
		-header /dev/null -server /dev/null

${_MIG_SRVS_}: $${.TARGET:S/Server.c\$/.defs/}
	${MIG} ${_MIGFLAGS_} ${${.TARGET:S/Server.c\$/.defs/}:P} \
		-header /dev/null -user /dev/null
.endif

.if defined(IDLFILES)
${IDLFILES:.idl=.h}: \
		$${.TARGET:.h=.idl} \
		$${$${.TARGET:.h=}_ACF:D$${.TARGET:.h=.acf}}
	-${RM} ${_RMFLAGS_} ${.TARGET}
	${IDL} ${_IDLFLAGS_} -client none -server none ${${.TARGET:.h=.idl}:P}

${IDLFILES:.idl=_cstub.c}: \
		$${.TARGET:_cstub.c=.idl} \
		$${.TARGET:_cstub.c=.h} \
		$${$${.TARGET:_cstub.c=}_ACF:D$${.TARGET:_cstub.c=.acf}}
	-${RM} ${_RMFLAGS_} ${.TARGET:_cstub.c=_cstub.c}
	${IDL} ${_IDLFLAGS_} -server none -header /tmp/${.TARGET:_cstub.c=.h} ${${.TARGET:_cstub.c=.idl}:P}
	-${RM} ${_RMFLAGS_} /tmp/${.TARGET:_cstub.c=.h}

${IDLFILES:.idl=_sstub.c}: \
		$${.TARGET:_sstub.c=.idl} \
		$${.TARGET:_sstub.c=.h} \
		$${$${.TARGET:_sstub.c=}_ACF:D$${.TARGET:_sstub.c=.acf}}
	-${RM} ${_RMFLAGS_} ${.TARGET:_sstub.c=_sstub.c}
	${IDL} ${_IDLFLAGS_} -client none -header /tmp/${.TARGET:_sstub.c=.h} ${${.TARGET:_sstub.c=.idl}:P}
	-${RM} ${_RMFLAGS_} /tmp/${.TARGET:_sstub.c=.h}

${IDLFILES:.idl=_caux.c}: 
	rm -f ${.TARGET}
	echo "static int idl_aux_stub = 0;" >${.TARGET}

${IDLFILES:.idl=_saux.c}:
	rm -f ${.TARGET}
	echo "static int idl_aux_stub = 0;" >${.TARGET}

.endif

.if defined(SAMSFILES)
TECHNOLOGY?=dce
# generates message files - dts.sams => dcedtsmsg.h
${SAMSFILES:S/^/${TECHNOLOGY}/g:S/.sams/msg.h/g}: \
  $${.TARGET:S/^${TECHNOLOGY}//:S/msg.h/.sams/}
	${SAMS} ${_SAMSFLAGS_} -oh ${${.TARGET:S/^${TECHNOLOGY}//:S/msg.h$/.sams/}:P}

# Message table  dts.sams => dcedtsmsg.c
${SAMSFILES:S/^/${TECHNOLOGY}/g:S/.sams/msg.c/g}: \
  $${.TARGET:S/^${TECHNOLOGY}//:S/msg.c/.sams/}
	${SAMS} ${_SAMSFLAGS_} -ot ${${.TARGET:S/^${TECHNOLOGY}//:S/msg.c$/.sams/}:P}

# Generates the message file, gencat input - dcedts.msg =>dcedts.cat
${SAMSFILES:S/^/${TECHNOLOGY}/g:S/.sams/.cat/g}: \
  $${.TARGET:S/.cat/.msg/}
	${SAMS} ${_SAMSFLAGS_} -oc ${${.TARGET:S/^${TECHNOLOGY}//:S/.cat$/.sams/}:P}

# Generates the message file, gencat input - dts.sams => dcedts.msg dcedts.cat
${SAMSFILES:S/^/${TECHNOLOGY}/g:S/.sams/.msg/g}: \
  $${.TARGET:S/^${TECHNOLOGY}//:S/.msg/.sams/}
	${SAMS} ${_SAMSFLAGS_} -om ${${.TARGET:S/^${TECHNOLOGY}//:S/.msg$/.sams/}:P}

# Generates the serviceability files, dts.sams => dcedtssvc.c
${SAMSFILES:S/^/${TECHNOLOGY}/g:S/.sams/svc.c/g}: \
  $${.TARGET:S/^${TECHNOLOGY}//:S/svc.c/.sams/}
	${SAMS} ${_SAMSFLAGS_} -oS ${${.TARGET:S/^${TECHNOLOGY}//:S/svc.c$/.sams/}:P}

# Generates the serviceability files, dts.sams => dcedtssvc.h
${SAMSFILES:S/^/${TECHNOLOGY}/g:S/.sams/svc.h/g}: \
  $${.TARGET:S/^${TECHNOLOGY}//:S/svc.h/.sams/}
	${SAMS} ${_SAMSFLAGS_} -os ${${.TARGET:S/^${TECHNOLOGY}//:S/svc.h$/.sams/}:P}

# Generates the serviceability files, dts.sams => dcedtsmac.h

${SAMSFILES:S/^/${TECHNOLOGY}/g:S/.sams/mac.h/g}: \
  $${.TARGET:S/^${TECHNOLOGY}//:S/mac.h/.sams/}
	${SAMS} ${_SAMSFLAGS_} -ou ${${.TARGET:S/^${TECHNOLOGY}//:S/mac.h$/.sams/}:P}

.endif
#


#  Special rules
#

#
#  Use this as a dependency for any rule which should always be triggered
#  (e.g. recursive makes).
#
${ALWAYS}:

#
#  Include pass definitions for standard targets
#
.include <osf.${project_name}.passes.mk>

#
#  Compilation rules
#
all: build_all

build_all: $${_all_targets_}

comp_all: $${_all_targets_}

#
#  Clean up rules
#
clean_all: $${_all_targets_}
	-${RM} ${_RMFLAGS_} core

.if !empty(_CLEAN_TARGETS_:Mclean_*)
${_CLEAN_TARGETS_:Mclean_*}:
	-${RM} ${_RMFLAGS_} ${_CLEANFILES_}
.endif

rmtarget_all: $${_all_targets_}
	-${RM} ${_RMFLAGS_} core

.if !empty(_RMTARGET_TARGETS_:Mrmtarget_*)
${_RMTARGET_TARGETS_:Mrmtarget_*}:
	-${RM} ${_RMFLAGS_} ${_RMTARGET_TARGET}
.endif

clobber_all: $${_all_targets_}
	-${RM} ${_RMFLAGS_} core depend.mk

.if !empty(_CLOBBER_TARGETS_:Mclobber_*)
${_CLOBBER_TARGETS_:Mclobber_*}:
	-${RM} ${_RMFLAGS_} ${_CLOBBERFILES_}
.endif

#
#  Clobber export rules
#

.if !defined(EXPORTBASE)

clobber_exp_all: ${ALWAYS}
	@echo "You must define EXPORTBASE to do a ${.TARGET}"

.if !empty(_CLOBBER_EXP_TARGETS_:Mclobber_exp_*)
${_CLOBBER_EXP_TARGETS_:Mclobber_exp_*}: ${ALWAYS}
	@echo "You must define EXPORTBASE to do a ${.TARGET}"
.endif

.else

clobber_exp_all: $${_all_targets_}

.if !empty(_CLOBBER_EXP_TARGETS_:Mclobber_exp_*)
${_CLOBBER_EXP_TARGETS_:Mclobber_exp_*}:
	-${RM} ${_RMFLAGS_}  ${.TARGET:Mclobber_exp_*:S/^clobber_exp_//g:@_CLOBBER_EXP_TARGET@${_CLOBBER_EXP_FILES_}@}
.endif
.endif

#
#  Lint rules
#
lint_all: $${_all_targets_}

.if !empty(_LINT_TARGETS_:Mlint_*)
${_LINT_TARGETS_:Mlint_*}: $${LINTFILES}
	${LINT} ${_LINTFLAGS_} ${.ALLSRC}
.endif

#
#  Statmet rules
#  STATMETFILES is a component makefile variable
#
statmet_all: $${_all_targets_}

.if !empty(_STATMET_TARGETS_:Mstatmet_*)
${_STATMET_TARGETS_:Mstatmet_*}: $${STATMETFILES}
	echo ${STATMETFLES}
.endif
.c.M:
	${STATMET} ${_STATMETFLAGS_} ${.IMPSRC}
.s.M:
	${STATMET} ${_STATMETFLAGS_} ${.IMPSRC}

#
#  Tags rules
#
tags_all: $${_all_targets_}

.if !empty(_TAGS_TARGETS_:Mtags_*)
${_TAGS_TARGETS_:Mtags_*}: $${TAGSFILES}
	${TAGS} ${_TAGSFLAGS_} ${.ALLSRC}
.endif

#
#  Setup rules
#
setup_all: $${_all_targets_}
.if !empty(_SETUP_TARGETS_:Msetup_*)
${_SETUP_TARGETS_:Msetup_*}: $${_SETUPFILES_}
${_SETUP_TARGETS_:Msetup_*:S/^setup_//g:@_SETUP_TARGET@${_SETUPFILES_}@}:\
               $${.TARGET:T}
.if defined(ODE_UNDER_CLEARCASE)
.if !defined(NO_CHECKOUT)
	if cmp -s ${.TARGET} ${.ALLSRC} ; then echo "Already there!";   \
	else echo "Different!";                                         \
		if [ -f ${.TARGET} ] ; then ${CLEARTOOL} co -c checkout ${.TARGET} || true;  fi ;\
		${RM} -f ${.TARGET};                                    \
		${CLEARAUDIT} -c "${CP} -r ${.ALLSRC} ${.TARGET}";      \
	fi
.endif
.else
	-${RM} ${_RMFLAGS_} ${.TARGET}
	${MAKEPATH} ${.TARGET}
.if defined(EXPORT_USING_CP)
	${CP} ${_CPFLAGS_} ${.ALLSRC} ${.TARGET}
.else
	(cd ${.ALLSRC:H};${TAR} cfh - ${.ALLSRC:T}) | \
	(cd ${.TARGET:H};${TAR} xf -)
.endif
.if defined(SETUP_LINKS)
	${RM} ${_RMFLAGS_} ${DEFTOOLBASE}${SETUP_LINKS}	
	${LN} ${.TARGET} ${DEFTOOLBASE}${SETUP_LINKS}
.endif
.endif
.endif

#
#  Export rules
#
.if !defined(EXPORTBASE)

export_all: ${ALWAYS}
	@echo "You must define EXPORTBASE to do an ${.TARGET}"

.if !empty(_EXPORT_TARGETS_:Mexport_*)
${_EXPORT_TARGETS_:Mexport_*}: ${ALWAYS}
	@echo "You must define EXPORTBASE to do an ${.TARGET}"
.endif

.else

export_all: $${_all_targets_}

.if !empty(_EXPORT_TARGETS_:Mexport_*)
${_EXPORT_TARGETS_:Mexport_*}: $${_EXPFILES_}

.if defined(ODE_UNDER_CLEARCASE) && defined(EXPORT_USING_SYMLINKS)
_EXPFILES_=${EXPORTBASE}${_EXPDIR_}${_EXPORT_TARGET}
_EXPTARG_=${_EXPORT_TARGET:T}
_EXPORTTARGETLINK_=${EXPORTBASE}${_EXPDIR_}${_EXPORT_TARGET}
_EXPLINK_=(cd ${${_EXPORT_TARGET}:q:H}; ${MAKEPATH} ${_EXPORTTARGETLINK_}; rm -f ${_EXPORTTARGETLINK_}; ${LN} -s `pwd`/${_EXPORT_TARGET} ${_EXPORTTARGETLINK_});

.EXPORT_SAMIDL_:
	@${EXPSAMIDL_TARGETS:Mexport_*:S/^export_//g:@_EXPORT_TARGET@${_EXPLINK_}@}
	touch ${.TARGET}

.EXPORT_INCLUDES_:
	@${EXPINC_TARGETS:Mexport_*:S/^export_//g:@_EXPORT_TARGET@${_EXPLINK_}@}
	touch ${.TARGET}

.EXPORT_SHLIBS_:
	@${EXPSHLIB_TARGETS:Mexport_*:S/^export_//g:@_EXPORT_TARGET@${_EXPLINK_}@}	
	touch ${.TARGET}

.EXPORT_BINS_:
	@${EXPBIN_TARGETS:Mexport_*:S/^export_//g:@_EXPORT_TARGET@${_EXPLINK_}@}
	touch ${.TARGET}

.EXPORT_LIBS_:
	@${EXPLIB_TARGETS:Mexport_*:S/^export_//g:@_EXPORT_TARGET@${_EXPLINK_}@}
	touch ${.TARGET}

_EXPORT_SAMIDL_TARGETS_=.EXPORT_SAMIDL_ ${EXPSAMIDL_TARGETS:Nexport_*} ${EXPSAMIDL_TARGETS:Mexport_*:S/^export_//g} 
_EXPORT_EXPINC_TARGETS_=.EXPORT_INCLUDES_ ${EXPINC_TARGETS:Nexport_*} ${EXPINC_TARGETS:Mexport_*:S/^export_//g} ${IDLDATAFILES}
_EXPORT_EXPSHLIB_TARGETS_=.EXPORT_SHLIBS_ ${EXPSHLIB_TARGETS:Nexport_*} ${EXPSHLIB_TARGETS:Mexport_*:S/^export_//g}
_EXPORT_EXPBIN_TARGETS_=.EXPORT_BINS_ ${EXPBIN_TARGETS:Nexport_*} ${EXPBIN_TARGETS:Mexport_*:S/^export_//g} 
_EXPORT_EXPLIB_TARGETS_=.EXPORT_LIBS_ ${EXPLIB_TARGETS:Nexport_*} ${EXPLIB_TARGETS:Mexport_*:S/^export_//g}
.else

${_EXPORT_TARGETS_:Mexport_*:S/^export_//g:@_EXPORT_TARGET@${_EXPFILES_}@}:\
		$${.TARGET:T}
	-${RM} ${_RMFLAGS_} ${.TARGET}
	${MAKEPATH} ${.TARGET}
.if defined(EXPORT_USING_SYMLINKS) 
	(cd ${.ALLSRC:H}; ${LN} -s `pwd`/${.TARGET:T} ${.TARGET})
.else
.if defined(EXPORT_USING_CP)
	${CP} ${_CPFLAGS_} ${.ALLSRC} ${.TARGET}
.else
	(cd ${.ALLSRC:H}; ${TAR} cfh - ${.ALLSRC:T}) | \
	(cd ${.TARGET:H}; ${TAR} xf -)
.endif
	${.ALLSRC:T:@_EXPORT_TARGET@${_MAKE_EXPLINKS_}@}
.endif
.endif
.endif
.endif

#
#  Installation/release rules
#
.if !defined(TOSTAGE)

install_all: ${ALWAYS}
	@echo "You must define TOSTAGE to do an ${.TARGET}"

.if !empty(_INSTALL_TARGETS_:Minstall_*)
${_INSTALL_TARGETS_:Minstall_*}: ${ALWAYS}
	@echo "You must define TOSTAGE to do an ${.TARGET}"
.endif

.else

install_all: $${_all_targets_}

.if !empty(_INSTALL_TARGETS_:Minstall_*)

.if defined(FROMSTAGE)

${_INSTALL_TARGETS_:Minstall_*}: ${ALWAYS}
	${_RELEASE_} ${_NOSTRIP_}\
		-o ${${_INSTALL_TARGET}_IOWNER:U${IOWNER}}\
		-g ${${_INSTALL_TARGET}_IGROUP:U${IGROUP}}\
		-m ${${_INSTALL_TARGET}_IMODE:U${IMODE}}\
		-tostage ${TOSTAGE}\
		-fromstage ${FROMSTAGE}\
		${_IDIR_}${_INSTALL_TARGET}\
		${${_INSTALL_TARGET}_ILINKS:U${ILINKS}}

.else

${_INSTALL_TARGETS_:Minstall_*}: $${_INSTALL_TARGET}
	${_RELEASE_} ${_NOSTRIP_}\
		-o ${${_INSTALL_TARGET}_IOWNER:U${IOWNER}}\
		-g ${${_INSTALL_TARGET}_IGROUP:U${IGROUP}}\
		-m ${${_INSTALL_TARGET}_IMODE:U${IMODE}}\
		-tostage ${TOSTAGE}\
		-fromfile ${${_INSTALL_TARGET}:P}\
		${_IDIR_}${_INSTALL_TARGET}\
		${${_INSTALL_TARGET}_ILINKS:U${ILINKS}}

.endif

.endif

.endif

.endif
