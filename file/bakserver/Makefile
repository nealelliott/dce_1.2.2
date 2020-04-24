# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.23.1  1996/10/02  16:59:32  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:04:07  damon]
#
# Revision 1.1.17.3  1994/09/07  15:48:18  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:15  marty]
# 
# Revision 1.1.17.2  1994/06/09  13:50:50  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:24:13  annie]
# 
# Revision 1.1.17.1  1994/02/04  20:04:57  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:06:05  devsrc]
# 
# Revision 1.1.15.1  1993/12/07  17:11:57  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  12:46:56  jaffe]
# 
# Revision 1.1.11.4  1993/01/18  19:52:14  cjd
# 	Embedded copyright notice
# 	[1993/01/18  19:27:05  cjd]
# 
# Revision 1.1.11.3  1993/01/13  15:24:45  shl
# 	Transarc delta: vijay-ot6663-bak-remove-obsolete-files 1.1
# 	  Selected comments:
# 
# 	    Remove some obsolete files. Reorganize some of the header files which caused
# 	    some header files to go obsolete. Handle a merge error in budb.idl and also
# 	    in procs.c.
# 	[1993/01/12  17:42:31  shl]
# 
# Revision 1.1.11.2  1992/11/24  15:38:33  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  16:06:06  bolinger]
# 
# Revision 1.1.3.12  1992/06/30  21:42:53  mason
# 	Add the STUB_OPT_LEVEL variable to control optimization of stub files.
# 	[1992/06/30  21:27:49  mason]
# 
# Revision 1.1.3.11  1992/06/08  22:41:55  mason
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    Replaced -lbsd with ${BSD_LIBS} variable
# 	[1992/06/08  21:02:56  mason]
# 
# Revision 1.1.3.10  1992/05/20  19:43:54  mason
# 	Transarc delta: cfe-ot3085-fix-msf-dependencies 1.1
# 	  Files modified:
# 	    bak: Makefile; bakserver: Makefile; bosserver: Makefile
# 	    bubasics: Makefile; flserver: Makefile
# 	    flserver.klib: Makefile; fshost: Makefile; fsint: Makefile
# 	    ftserver: Makefile; host: Makefile; ncscompat: Makefile
# 	    ncsubik: Makefile; security/dacl: Makefile
# 	    security/dfsauth: Makefile; sysacl: Makefile; tkm: Makefile
# 	    tools/cmd: Makefile; update: Makefile; xcred: Makefile
# 	    xvolume: Makefile
# 	  Selected comments:
# 	    Make explicit the dependency that builds an .msf file from an .et file.
# 	    Add dependency to build .msf file.
# 	[1992/05/19  22:18:59  mason]
# 
# Revision 1.1.3.9  1992/05/05  19:00:20  rec
# 	Removed dependence on libkrb5.a by allowing libdce.a
# 	to export required symbol (krb5_cksumarray).
# 	[1992/05/05  18:53:26  rec]
# 
# Revision 1.1.3.8  1992/04/16  19:59:09  mckeen
# 	Removed definition of IDL
# 	[1992/04/15  19:07:51  mckeen]
# 
# Revision 1.1.3.7  1992/04/16  17:18:36  garyf
# 	fix .p.h rules
# 	[1992/04/16  17:12:37  garyf]
# 
# Revision 1.1.3.6  1992/04/14  03:09:22  mason
# 	The dfs error catalogs were generated with names that made them
# 	dce_error_inq_text unable to locate them. (dce_error_inq_text pulls
# 	a facility code out of the error code and uses this to construct a
# 	filename in which to look for the error message.  Our (dfs) facility
# 	code is such that our error catalog filenames must start with "dfs",
# 	rather then "dce".)
# 	Change occurrences of "dce...\.cat" to "dfs...\.cat"
# 	[1992/04/10  21:05:13  mason]
# 
# Revision 1.1.3.5  1992/04/07  20:26:48  rec
# 	Have -lkrb5 on link line depend on RIOS platform since
# 	it is not needed with a shared libdce on OSF/1.
# 	[1992/04/07  19:54:27  rec]
# 
# Revision 1.1.3.4  1992/01/30  14:58:01  carl
# 	add lib krb5 to libs list
# 	[1992/01/30  14:57:40  carl]
# 
# Revision 1.1.3.3  1992/01/24  01:48:15  devsrc
# 	Merging dfs6.3
# 	[1992/01/24  00:02:48  devsrc]
# 
# Revision 1.1.3.2  1992/01/23  19:04:08  zeliff
# 	Moving files onto the branch for dfs6.3 merge
# 	[1992/01/23  18:19:40  zeliff]
# 	Revision 1.1.1.2  1992/01/22  19:29:46  devsrc
# 	Bring in dfs6.3 sources
# 
# Revision 1.1  1992/01/19  02:54:25  devrcs
# 	Initial revision
# 
# $EndLog$
#

INCLUDES		= budb.h budb_errs.h

LIBRARIES		= libbudb.a
PROGRAMS        	= bakserver 

EXPDIR			= /usr/include/dcedfs/
libbudb.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS                  = dfsbdb.cat

ILIST                   = ${OTHERS} bakserver budb.h 
IDIR                    = /share/include/dcedfs/
dfsbdb.cat_IDIR         = /nls/msg/en_US.ASCII/
bakserver_IDIR          = /bin/

CFLAGS			= -D_VOPS -D_BSD_INCLUDES -Daiws -D__unix ${USERCFLAGS}

IDLFILES		= budb.idl

IDLINCFLAGS             = -I.
IDLFLAGS                = -keep c_source -no_mepv

# On some platforms the optimizer is very expensive relative to the
# actual gain.  We disable it on a per-platform basis.
OPT_LEVEL		+= ${STUB_OPT_LEVEL}

LIBS          		= -lbudb -lbubasics -lnubik -licl -lcom_err -lcmd \
			  -lafsutil -lncompat -ltpq -lbomb -lcommondata \
			  -ldacl -ldauth -ldacllfs -ldacl -ldauth -ldacllfs \
			  -lafssys -losi -ldce ${OS_LIBS}

libbudb.a_OFILES	= budb_cstub.o struct_ops.o 
bakserver_OFILES	= database.o db_alloc.o db_dump.o db_hash.o \
			  ol_verify.o budb_sstub.o dbs_dump.o \
			  db_lock.o db_text.o procs.o server.o

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

budb_errs.h budb_errs.msf: budb_errs.et
	${COMPILE_ET} ${budb_errs.et:P}

dfsbdb.cat: budb_errs.msf
	${GENCAT} $@ $>

