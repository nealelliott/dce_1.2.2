/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*  
 * HISTORY
 * $Log: icl_userint.h,v $
 * Revision 1.1.13.1  1996/10/02  17:52:29  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:41:04  damon]
 *
 * Revision 1.1.8.2  1994/06/09  14:11:46  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:25:23  annie]
 * 
 * Revision 1.1.8.1  1994/02/04  20:21:22  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:20  devsrc]
 * 
 * Revision 1.1.6.1  1994/01/28  20:43:43  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:18  annie]
 * 
 * Revision 1.1.4.2  1993/05/11  14:16:37  jaffe
 * 	Transarc delta: comer-db3378-add-rpc-interface-to-user-level-icl 1.2
 * 	  Selected comments:
 * 	    This delta implements an RPC interface to the user-level ICL tracing
 * 	    package.  The idea is that each server will export this interface to
 * 	    CDS under a meaningful entry.  For example, the flserver would export
 * 	    it under /.:/hosts/<hostname>/flserver by making the call:
 * 	    code = icl_ExportIF("flserver");
 * 	    Each command in the dfstrace suite now has a -cdsentry that, if
 * 	    specified, will cause dfstrace to connect to the specified server
 * 	    instead of the kernel.
 * 	    Added new #defines for -cdsentry argument.
 * 	    The dump file is now restricted to be in /opt/dcelocal/var/dfs/adm.
 * 	[1993/05/10  18:14:46  jaffe]
 * 
 * Revision 1.1.2.2  1993/01/13  18:08:57  shl
 * 	Transarc delta: comer-ot6297-icl-user-interface 1.5
 * 	  Selected comments:
 * 	    Added some functioanlity and a user interface to the ICL package.  The
 * 	    current command is called dfstrace; the current interface is:
 * 	    ./dfstrace: Commands are:
 * 	    apropos         search by help text
 * 	    clear           clear logs by logname or by event set
 * 	    dump            dump DFS trace logs from the kernel
 * 	    help            get help on commands
 * 	    lslogs          list available logs
 * 	    lssets          list available event sets
 * 	    setlog          set the size of a log
 * 	    setset          set state of event sets
 * 	    Usage: dfstrace clear  [{-set <set_name>... | -log <log_name>... | -all}]
 * 	    [-help]
 * 	    Usage: dfstrace dump  [{-set <set_name>... | -all | -follow <log_name>}]
 * 	    [-file <output_filename>]
 * 	    [-sleep <seconds_between_reads>] [-help]
 * 	    To get a continuous dump of a log, for example, do:
 * 	    dfstrace -follow cmfx -sleep 5
 * 	    Usage: dfstrace lslogs  [{-set <set_name>... | -log <log_name>... | -all}]
 * 	    [-long][-help]
 * 	    Lists logs, their sizes, and whether or not they are currently
 * 	    allocated.
 * 	    Usage: dfstrace lssets  [{-set <set_name>... | -all}] [-help]
 * 	    Lists available sets and their states.
 * 	    Usage: dfstrace setlog  -log <log_name> -buffersize <4-kilobyte_units>
 * 	    Set log size.
 * 	    Usage: dfstrace setset  {-activate | -deactivate | -release}
 * 	    [{-set <set_name>... | -all}] [-help]
 * 	    Change the state of an event set.  In active state, logging
 * 	    occurs to log.  In inactive state, no loggin occurs but the
 * 	    reference to the log persists.  In the released state, the
 * 	    reference to the log is gone, so the log may be deallocated.
 * 	    #defines for parameter indices.
 * 	    Misc bug fixes.
 * 	    logp->logElements wasn't getting reset on clear.
 * 	    By default icl tracing is turned on.  This change makes it a little
 * 	    easier to turn it on and off when compiling.  The change adds 2 flags:
 * 	    ICL_DEFAULT_ENABLED and ICL_DEFAULT_DISABLED.  There is an accompanying
 * 	    -D in icl and icl.klib Makefiles, currently both enabled.
 * 	Transarc delta: comer-ot6545-dfstrace-if-cleanup 1.2
 * 	  Selected comments:
 * 	    Removed unneeded -all options; misc other i/f cleanup.
 * 	    Fixed command option indices.
 * 	    Forgot to remove a printf.
 * 	Transarc delta: shl-add-copyrights-to-new-files 1.1
 * 	  Selected comments:
 * 	    Added OSF copyrights and other header stuff
 * 	[1993/01/12  21:20:46  shl]
 * 
 * $EndLog$
 */

#ifndef _ICL_USERINT_H__ENV_
#define _ICL_USERINT_H__ENV_

#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>

/* define common argument indices */
#define COMMON_ARG_CDSENTRY_INDEX 4

/* indices for dump command */
#define DUMP_ARG_SET_INDEX	0
#define DUMP_ARG_FOLLOW_INDEX	1
#define DUMP_ARG_FILE_INDEX	2
#define DUMP_ARG_SLEEP_INDEX	3
#define DUMP_ARG_LOG_INDEX      5
#define DUMP_ARG_RAW_INDEX      6

/* define argument indices for Showlog */
#define SHOWL_ARG_SET_INDEX	0
#define SHOWL_ARG_LOG_INDEX	1
#define SHOWL_ARG_LONG_INDEX	2

/* define argument indices for Showset */
#define SHOWS_ARG_SET_INDEX	0

/* define argument indices for Clear */
#define CLR_ARG_SET_INDEX	0
#define CLR_ARG_LOG_INDEX	1

/* define argument indices for set */
#define SET_ARG_SET_INDEX	0
#define SET_ARG_ACT_INDEX	1
#define SET_ARG_DEACT_INDEX	2
#define SET_ARG_FREE_INDEX	3

/* define argument indices for resize */
#define SSIZE_ARG_LOG_INDEX	0
#define SSIZE_ARG_SIZE_INDEX	1

#endif /* _ICL_USERINT_H__ENV_ */
