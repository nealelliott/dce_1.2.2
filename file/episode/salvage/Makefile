# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.141.1  1996/10/02  17:27:15  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:33:52  damon]
#
# Revision 1.1.136.5  1994/08/30  15:55:42  sasala
# 	DFS S12Y general cleanup - rename ftutil and tools cmd catalogs, add ftutil msgs to xaggr dfsexport, and episode salvage and vnops subcomponents
# 	[1994/08/30  00:19:22  sasala]
# 
# Revision 1.1.136.4  1994/08/17  16:08:33  ruby
# 	Implement partial S12Y (Internationalization) for DFS commands a daemons
# 	[1994/08/17  16:08:16  ruby]
# 
# 	Implement partial S12Y (Internationalization) for DFS commands and daemons
# 	[1994/08/12  18:02:13  ruby]
# 
# Revision 1.1.136.3  1994/06/09  14:02:56  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:37:58  annie]
# 
# Revision 1.1.136.2  1994/02/04  20:14:48  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:11:07  devsrc]
# 
# Revision 1.1.136.3  1994/06/09  14:02:56  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:37:58  annie]
# 
# Revision 1.1.136.2  1994/02/04  20:14:48  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:11:07  devsrc]
# 
# Revision 1.1.13.1  1993/12/07  17:20:28  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  14:05:32  jaffe]
# 
# Revision 1.1.7.7  1993/01/19  15:32:47  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:54:45  cjd]
# 
# Revision 1.1.7.6  1993/01/13  16:56:49  shl
# 	Transarc delta: jaffe-cleanup-2.3-compiler-warnings 1.8
# 	  Selected comments:
# 	    Fix compiler warnings found when moving to OSF/1 1.1.1.
# 	    remove lfp.
# 	    Remove the function dacl_SetOwnerControl.  It is not used.
# 	[1993/01/12  20:15:24  shl]
# 
# Revision 1.1.7.5  1992/11/24  16:55:59  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  16:25:11  bolinger]
# 
# Revision 1.1.7.4  1992/11/18  19:32:06  jaffe
# 	Transarc delta: cburnett-ot5890-rename-salvage.h-to-salveger.h 1.1
# 	  Selected comments:
# 	    Rename salvage/salvage.p.h to salvage/salvager.p.h to avoid having
# 	    two salvage.h include files.  There is also a salvage.h in the anode dir.
# 	    salvage.p.h to salvager.p.h
# 	Transarc delta: cburnett-ot6012-salvage-makefile-dependencies 1.1
# 	  Selected comments:
# 	    salvage_main.o and name.o need dependency rules for salvager.h.
# 	    This was found when the OSF pmake was used to build Episode.
# 	    Add dependencies as described above.
# 	[1992/11/17  21:15:17  jaffe]
# 
# Revision 1.1.7.3  1992/10/27  20:53:16  jaffe
# 	Transarc delta: jaffe-ot5497-fix-salvage-Makefile 1.1
# 	  Selected comments:
# 	    install both dfssal.cat and salvage.
# 	    put both PROGRAMS and OTHERS on the same ILIST line.
# 	[1992/10/27  13:05:57  jaffe]
# 
# Revision 1.1.7.2  1992/08/31  20:10:21  jaffe
# 	Sync with Transarc
# 	[1992/08/30  14:13:21  jaffe]
# 
# Revision 1.1.3.10  1992/06/23  13:55:22  jdp
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    User space binaries were being linked against -lbsd.  This library is
# 	    not present on all machine types.  Fixed this by adding a BSD_LIBS in
# 	    the machdep.mk file and using that instead.  Now, each architecture
# 	    can define its own set of BSD-compatibility libraries. (This is a
# 	    portability fix).
# 	    Use BSD_LIBS instead of -lbsd
# 	Transarc delta: ota-ot3609-no-msf-rules 1.1
# 	  Selected comments:
# 	    Fix problem with Episode makefiles wherein they didn't have a rule to
# 	    make the *.msf files.  Since this is a side effect of running
# 	    compile_et, just add this to the left-hand-side of that rule.
# 	    Also remove fifo_errors.et since this isn't used.
# 	    Change makefile.
# 	[1992/06/22  22:09:32  jdp]
# 
# Revision 1.1.3.9  1992/05/13  21:59:59  jdp
# 	Transarc delta: bab-ot2394-change-catalog-names 1.2
# 	  Files modified:
# 	    anode: Makefile; async: Makefile; logbuf: Makefile
# 	    salvage: Makefile
# 	  Selected comments:
# 	    The dfs error catalogs were generated with names that made them
# 	    dce_error_inq_text unable to locate them. (dce_error_inq_text pulls
# 	    a facility code out of the error code and uses this to construct a
# 	    filename in which to look for the error message.  Our (dfs) facility
# 	    code is such that our error catalog filenames must start with "dfs",
# 	    rather then "dce".)
# 	    Change occurrences of "dce...\.cat" to "dfs...\.cat"
# 	    ot 2394
# 	Transarc delta: cburnett-ot2714-episode_changes_for_aix32 1.1
# 	  Files modified:
# 	    async: us.h; salvage: Makefile
# 	    vnops: efs_vattr.c, efs_vfsops.c
# 	  Selected comments:
# 	    Changes to allow Episode to build on AIX 3.2 as well as
# 	    AIX 3.1.
# 	    use USERCFLAGS
# 	[1992/05/13  21:22:55  jdp]
# 
# Revision 1.1.3.8  1992/05/05  18:30:55  jdp
# 	delta jdp-epimerge-use-ode-makefiles 1.5
# 	[1992/05/04  22:44:44  jdp]
# 
# Revision 1.1.3.7  1992/04/29  02:30:23  garyf
# 	salvage_main.o,name.o depends on salvage.h
# 	fix ILIST
# 	[1992/04/29  02:29:22  garyf]
# 
# Revision 1.1.3.6  1992/04/07  19:07:50  mckeen
# 	dcesal.cat depends on sal_errs.msf which depends on salvage.h
# 	I think I have it right this time....
# 	[1992/04/07  16:47:48  mckeen]
# 
# 	Merged with mainline
# 	[1992/03/31  16:36:29  mckeen]
# 
# 	Changed dcesal.cat to be dependent on salvage.h not sal_errs.msf
# 	[1992/03/13  15:40:20  mckeen]
# 
# Revision 1.1.3.5  1992/02/03  16:23:43  treff
# 	Add -lbsd to LIBS.
# 	[1992/02/03  16:23:24  treff]
# 
# Revision 1.1.3.4  1992/01/25  20:46:36  zeliff
# 	dfs6.3 merge, part2
# 	[1992/01/25  20:03:58  zeliff]
# 
# $EndLog$

EXPDIR		= /usr/include/dcedfs/episode/
PROGRAMS 	= salvage

TECHNOLOGY=dfs

SAMSFILES = slv.sams
SAMS_TARGETS = dfsslvmsg.h dfsslvmsg.c dfsslvmac.h dfsslvsvc.c dfsslvsvc.h \
    dfsslv.cat

ILIST		= ${OTHERS} ${PROGRAMS}  dfsslv.cat
IDIR		= /bin/
dfsslv.cat_IDIR = /nls/msg/en_US.ASCII/

LIBS		= -lcom_err -ldir -lanode -ledsk -llogbuf -lasync \
		  -lxcred -ltools -llwp -lftutil -losi -lbomb -lafsutil \
		  -ldfskutil -ldfstab -lcmd -lafssys -lncompat \
		  -ldacl -ldacllfs -licl -ldce ${OS_LIBS}

OFILES		= dhops.o paths.o walk.o name.o salvage_main.o \
		  salvage_dhops.o aux_verify.o dfsslvsvc.o dfsslvmsg.o \
		  salvage_svc.o

CFLAGS          = ${USERCFLAGS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

name.o		: salvager.h

paths.o		: salvager.h slv.sams

walk.o		: salvager.h slv.sams

salvage_main.o  : salvager.h slv.sams

aux_verify.o	: salvager.h
