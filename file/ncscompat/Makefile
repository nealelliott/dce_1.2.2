# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.18.1  1996/10/02  17:54:10  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:42:01  damon]
#
# Revision 1.1.13.3  1994/08/17  15:15:14  ruby
# 	Implement partial S12Y (Internationalization) for DFS commands and daemons
# 	[1994/08/04  14:45:28  ruby]
# 
# Revision 1.1.13.2  1994/06/09  14:13:04  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:26:19  annie]
# 
# Revision 1.1.13.1  1994/02/04  20:22:31  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:14:50  devsrc]
# 
# Revision 1.1.11.1  1993/12/07  17:27:56  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:47:54  jaffe]
# 
# Revision 1.1.7.5  1993/01/19  16:07:48  cjd
# 	embedded copyright notice
# 	[1993/01/19  15:11:13  cjd]
# 
# Revision 1.1.7.4  1992/11/24  17:54:31  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  19:09:21  bolinger]
# 
# Revision 1.1.7.3  1992/10/27  21:15:00  jaffe
# 	Transarc delta: bab-ot5476-dfsauth-force-noauth-notify 1.3
# 	  Selected comments:
# 	    When it finds no default creds, the dfsauth package will now
# 	    complete initialization in unauthenticated mode.  This delta also
# 	    includes code to allow the bos command to take advantage of this
# 	    feature.
# 	    ot 5476
# 	    Moved error code constants into their own header file.  Also
# 	    added a return code for the use of rpc_locate_dfs_server() when the
# 	    dfsauth package forces noauth.
# 	    The first implementation wasn't really clean.  The place for the forced
# 	    noauth mode is not really in dfsauth package, but in the client of that
# 	    package.  The dfsauth package was, essentially restored; one routine was
# 	    added.  And the burden of retrying in noauth mode if no credentials were
# 	    found is moved to the end client.
# 	    Need to set noauth for the name space, even in some cases in which
# 	    noauth is not specified to rpc_locate_dfs_server.
# 	[1992/10/27  14:34:39  jaffe]
# 
# Revision 1.1.7.2  1992/08/31  20:06:49  jaffe
# 	Transarc delta: vijay-ot4694-dfs-servers-protect-against-cancel 1.1
# 	  Selected comments:
# 
# 	    This delta protects DFS servers from malicious cancels from client threads.
# 	    Here's a piece of mail from Beth Bottos explaining the problem.
# 	    Because of the way the RPC runtime interacts with threads, it is possible for a
# 	    malicious client to force the cancellation of a thread servicing an RPC within
# 	    a DFS server (all that is necessary is for the client to be running at least
# 	    two threads and to make an RPC with one thread and run pthread_cancel on the
# 	    thread making the RPC from the other thread).  This will, potentially,
# 	    deadlock many of our servers since a pending cancellation may be delivered when
# 	    a server holds any of the locks it uses internally.
# 	    Any cancellation that may be invoked on our RPC server threads can (in general)
# 	    be delivered during any call to any of the following pthread routines (these
# 	    are referred to as "cancelation points" in the pthreads documentation):
# 	    pthread_setasynccancel
# 	    pthread_testcancel
# 	    pthread_delay_np
# 	    pthread_join
# 	    pthread_cond_wait
# 	    pthread_cond_timedwait
# 	    The occurrence of pthread_cond_wait in this list is particularly bad for DFS
# 	    servers, since the implementations of the afslk_Obtain* routines use pthread_
# 	    cond_wait, and the afslk_Obtain* routines are used throughout our code base.
# 	    This means that, whenever we use one of these routines in an RPC server thread,
# 	    that thread becomes susceptible to cancellation, no matter what DFS locks it
# 	    might hold.
# 	    In order to protect our servers from this problem, we need to turn off cancel-
# 	    lation (saving the cancellation state when we were called) before obtaining any
# 	    of our own locks in our RPC management routines (the routines declared in .idl
# 	    files) and restore the old cancellation state before returning.  For all our
# 	    servers (with the possible exception of the backup server since it has long-
# 	    running manager routines?), this should not be a problem.  It should also not
# 	    be a real problem for the backup server if that server periodically tests for
# 	    the presence of pending cancels, using the pthread_testcancel routine, and
# 	    carefully exits (releasing any of its own locks) if a cancel is found to be
# 	    pending.
# 	    The following code is an example of how cancellation should be handled in our
# 	    manager routines (the code is merged from the BossvrCommonInit and
# 	    BossvrCommonTerm routines in the bosserver/bossvr_ncs_procs.c file)
# 	    long EXAMPLE_ManagerRoutine()
# 	    {
# 	    long        returnCode;
# 	    int savedCancelState;
# 	    if ((savedCancelState = pthread_setcancel(CANCEL_OFF)) != -1) {
# 	    /* real RPC manager service code goes here; rtnCode set appropriately*/
# 	    if (pthread_setcancel(savedCancelState) == -1) {
# 	    /* error handling appropriate to the server goes here; returnCode
# 	    set appropriately */
# 	    }
# 	    }
# 	    else {
# 	    /* error handling appropriate to the server goes here; returnCode set
# 	    appropriately */
# 	    return returnCcode;
# 	    }
# 	    Code of this form needs to be added to each our server implementations.
# 	    Add the new compat_handleCancel.h file.
# 	[1992/08/30  02:58:27  jaffe]
# 
# Revision 1.1.3.8  1992/05/20  19:56:42  mason
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
# 	[1992/05/20  11:40:14  mason]
# 
# Revision 1.1.3.7  1992/04/16  17:19:03  garyf
# 	fix .p.h rule
# 	[1992/04/16  17:13:06  garyf]
# 
# Revision 1.1.3.6  1992/04/14  04:08:59  mason
# 	Transarc delta: bab-ot2394-change-catalog-names 1.3
# 	  Files modified:
# 	    bak: Makefile; bakserver: Makefile; bosserver: Makefile
# 	    bubasics: Makefile; flserver: Makefile; fshost: Makefile
# 	    fsint: Makefile; ftserver: Makefile; host: Makefile
# 	    ncscompat: Makefile; ncsubik: Makefile
# 	    security/dacl: Makefile; security/dfsauth: Makefile
# 	    tkm: Makefile; tools/cmd: Makefile; update: Makefile
# 	    xcred: Makefile; xvolume: Makefile
# 	  Selected comments:
# 	    The dfs error catalogs were generated with names that made them
# 	    dce_error_inq_text unable to locate them. (dce_error_inq_text pulls
# 	    a facility code out of the error code and uses this to construct a
# 	    filename in which to look for the error message.  Our (dfs) facility
# 	    code is such that our error catalog filenames must start with "dfs",
# 	    rather then "dce".)
# 	    Change occurrences of "dce...\.cat" to "dfs...\.cat"
# 	    ot 2394
# 	    The sed script missed one occurrence of "dfs...\.cat"
# 	    the first time through because it was hidden by another
# 	    occurrence earlier on the line.
# 	[1992/04/13  16:07:34  mason]
# 
# Revision 1.1.3.5  1992/01/31  16:20:35  delgado
# 	Add modifications to build libdcedfs.a
# 	[1992/01/31  16:20:18  delgado]
# 
# Revision 1.1.3.4  1992/01/25  20:48:13  zeliff
# 	dfs6.3 merge, part2
# 	[1992/01/25  20:06:25  zeliff]
# 
# $EndLog$

libncompat.a_OFILES	= compat_rpcbind.o compat_authn.o \
			  compat_osi.o compat_err.o compat_cds.o \
			  compat_rpcVers.o compat_dfsnames.o compat_debug.o \
			  compat_serverdup.o compat_junct.o \
	                  compat_svc.o dfsncssvc.o dfsncsmsg.o

libdfsjunct_OFILES = compat_junct.o
libdfsjunct.a_OFILES = ${libdfsjunct_OFILES}

.if !defined(LIBDCEDFS_LIBRARY_MAKEFILE)
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
#
#
#

.if ${TARGET_OS} == "HPUX"
IN_SHLIB                = 1
.endif

INCLUDES		= compat.h \
			  compat_dfsnames.h compat_rpcVers.h \
			  compat_handleCancel.h compat_errs.h

LIBRARIES		= libncompat.a libdfsjunct.a

EXPDIR			= /usr/include/dcedfs/
libncompat.a_EXPDIR	= /usr/lib/
libdfsjunct.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}
EXPSHLIB_TARGETS        = ${libdfsjunct_OFILES}

OTHERS			= dfscmp.cat

TECHNOLOGY=dfs

SAMSFILES = ncs.sams
SAMS_TARGETS = dfsncsmsg.h dfsncsmsg.c dfsncsmac.h dfsncssvc.c dfsncssvc.h \
    dfsncs.cat

ILIST			= ${OTHERS} compat.h compat_dfsnames.h \
			  compat_rpcVers.h compat_handleCancel.h \
			  compat_errs.h  dfsncs.cat

IDIR			= /share/include/dcedfs/
dfsncs.cat_IDIR          = /nls/msg/en_US.ASCII/
dfscmp.cat_IDIR		= /nls/msg/en_US.ASCII/

CFLAGS			= -Dunix ${USERCFLAGS}

.if ${TARGET_OS}=="SUNOS5"
CFLAGS                  += -K PIC
.endif

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

compat_errs.h compat_errs.msf: compat_errs.et
	${COMPILE_ET} ${compat_errs.et:P}

dfscmp.cat: compat_errs.msf
	 ${GENCAT} $@ $>


.endif
.endif
