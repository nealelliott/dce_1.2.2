#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.813.1  1996/10/17  18:37:37  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:58:22  damon]
#
# Revision 1.1.808.1  1993/12/28  18:02:03  delgado
# 	use quotes for MAKETOP include instead of brackets
# 	[1993/12/28  17:58:44  delgado]
# 
# Revision 1.1.7.1  1993/09/28  21:03:46  gmd
# 	Removed unnecessary (and incorrect) IDIR variable
# 	so that files installed correctly.
# 	[1993/09/28  16:09:03  gmd]
# 
# Revision 1.1.4.7  1993/01/13  21:37:14  shl
# 	Transarc delta: comer-ot6251-move-icldump-into-file-tree 1.1
# 	  Selected comments:
# 	    This deltat moves the icldump command back into the file tree.  It
# 	    also adds a command parser interface.
# 	    Removed any trace of icldump.
# 	Transarc delta: comer-ot6297-icl-user-interface 1.5
# 	  Selected comments:
# 	    Added some functioanlity and a user interface to the ICL package.  The
# 	    current command is called dfstrace; the current interface is:
# 	    ./dfstrace: Commands are:
# 	    apropos         search by help text
# 	    clear           clear logs by logname or by event set
# 	    dump            dump DFS trace logs from the kernel
# 	    help            get help on commands
# 	    lslogs          list available logs
# 	    lssets          list available event sets
# 	    setlog          set the size of a log
# 	    setset          set state of event sets
# 	    Usage: dfstrace clear  [{-set <set_name>... | -log <log_name>... | -all}]
# 	    [-help]
# 	    Usage: dfstrace dump  [{-set <set_name>... | -all | -follow <log_name>}]
# 	    [-file <output_filename>]
# 	    [-sleep <seconds_between_reads>] [-help]
# 	    To get a continuous dump of a log, for example, do:
# 	    dfstrace -follow cmfx -sleep 5
# 	    Usage: dfstrace lslogs  [{-set <set_name>... | -log <log_name>... | -all}]
# 	    [-long][-help]
# 	    Lists logs, their sizes, and whether or not they are currently
# 	    allocated.
# 	    Usage: dfstrace lssets  [{-set <set_name>... | -all}] [-help]
# 	    Lists available sets and their states.
# 	    Usage: dfstrace setlog  -log <log_name> -buffersize <4-kilobyte_units>
# 	    Set log size.
# 	    Usage: dfstrace setset  {-activate | -deactivate | -release}
# 	    [{-set <set_name>... | -all}] [-help]
# 	    Change the state of an event set.  In active state, logging
# 	    occurs to log.  In inactive state, no loggin occurs but the
# 	    reference to the log persists.  In the released state, the
# 	    reference to the log is gone, so the log may be deallocated.
# 	    Took out icldump.
# 	    Misc bug fixes.
# 	    logp->logElements wasn't getting reset on clear.
# 	    By default icl tracing is turned on.  This change makes it a little
# 	    easier to turn it on and off when compiling.  The change adds 2 flags:
# 	    ICL_DEFAULT_ENABLED and ICL_DEFAULT_DISABLED.  There is an accompanying
# 	    -D in icl and icl.klib Makefiles, currently both enabled.
# 	[1993/01/13  15:01:58  shl]
# 
# Revision 1.1.4.6  1993/01/11  18:38:05  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:23:25  htf]
# 
# Revision 1.1.4.5  1992/12/09  21:34:16  jaffe
# 	Transarc delta: comer-ot6129-user-level-icl-dumping 1.5
# 	  Selected comments:
# 	    Added code to ICL trace logs at user level.  The guinea pig for this
# 	    change was dfsbind, also modified as part of this delta.  Most of the
# 	    actual tracing and some of the ICL work was done at IBM by Jean
# 	    Pehkonen.
# 	    Force the program to dump its log: kill -30 <pid>
# 	    Added dfsbind to EXPINC_SUBDIRS
# 	    In files where both osi.h and compat.h are included, the reference
# 	    to osi.h must be first.  In each of these files, osi.h was implicitly
# 	    included, so I added an explicit reference before compat.h.
# 	    Add ncompat and osi for the lock package.
# 	    Installed message catalog.
# 	[1992/12/07  13:55:42  jaffe]
# 
# Revision 1.1.4.4  1992/11/02  15:20:37  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:17:18  root]
# 
# Revision 1.1.4.3  1992/10/28  18:16:12  jaffe
# 	Transarc delta: fred-test-add-icldump 1.2
# 	  Selected comments:
# 	    Moved icldump command from source tree to test tree, since tests depend
# 	    upon it being there.
# 	    Added commands to build icldump program.
# 	    Too much multiplication.
# 	Transarc delta: rajesh-ot5367-salvage-recovery-authorization-check-tests 1.8
# 	  Selected comments:
# 	    To add authorization check tests for salvage/recovery. This is
# 	    the initial checkpoint.
# 	    Just a checkpoint.
# 	    Another checkpoint.
# 	    Checkpoint.
# 	    This delta version has been tested.
# 	    Changed permissions.
# 	    Update makefiles.
# 	    See above.
# 	    Fixed logical errors in the script pertaining to requiring read/write
# 	    access to aggregate to perform salvage and/or recovery, handling
# 	    of error counts generated in the AuthCheckTest.noroot script.
# 	    Fix a build-time error.
# 	[1992/10/28  15:07:54  jaffe]
# 
# Revision 1.1.4.2  1992/08/28  18:41:58  jaffe
# 	Transarc delta: jaffe-fix-test-102-1.6 1.2
# 	  Selected comments:
# 	    fixed extra spaces in the Makefile
# 	    export all scripts.
# 	Transarc delta: mcinerny-ot4865-install-tests-util-low-fs 1.1
# 	  Selected comments:
# 	    install test suite in export tree at export time
# 	    ditto
# 	[1992/08/28  12:37:58  jaffe]
# 
# Revision 1.1.2.2  1992/05/29  23:01:08  mason
# 	A working makefile
# 	[1992/05/29  19:27:40  mason]
# 
# $EndLog$
#
#
#

DATAFILES	= fs_GenerateFile fs_SetSysVar fs_FunctionsScript \
		  gen_FunctionsScript dce_AddUserPrincipal \
		  CreateLocalAccount DeleteLocalAccount 

SCRIPTS		= icldump

ILIST 		= ${DATAFILES} icldump
icldump_IMODE	= 0755



.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>

