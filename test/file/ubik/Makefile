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
# Revision 1.1.611.1  1996/10/17  18:37:06  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:58:12  damon]
#
# Revision 1.1.606.4  1994/09/07  15:48:41  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:37  marty]
# 
# Revision 1.1.606.3  1994/02/07  16:28:25  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:37  mckeen]
# 
# Revision 1.1.606.2  1994/02/04  20:47:44  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:25:42  devsrc]
# 
# Revision 1.1.604.1  1993/12/07  17:46:42  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  19:42:21  jaffe]
# 
# Revision 1.1.4.7  1993/04/09  17:15:47  gmd
# 	Added READ_ME to ILIST via DATAFILES.
# 	[1993/04/08  23:28:52  gmd]
# 
# 	Removed unnecessary IDIR definition.
# 	Removed incorrect utst executable IDIR definitions.
# 	[1993/04/08  23:15:39  gmd]
# 
# Revision 1.1.4.6  1993/04/01  22:26:08  jaffe
# 	Remove udebug
# 	[1993/04/01  22:12:05  jaffe]
# 
# Revision 1.1.4.5  1993/04/01  22:01:22  jaffe
# 	Transarc delta: vijay-db3329-ubik-add-dfstrace 1.6
# 	  Selected comments:
# 	    Add ICL tracing to the ubik library. Also convert the locking macros into
# 	    function calls to assist in debugging and tracking any locking problems.
# 	    All components that link with the ubik library will now have to also link in
# 	    the ICL library.
# 	    All hex values have a 0x prefix in the trace dump.
# 	    Add a few casts in order to satisfy the compiler.
# 	[1993/04/01  21:49:10  jaffe]
# 
# Revision 1.1.4.4  1993/03/10  21:45:45  jaffe
# 	Transarc delta: vijay-ot7364-ubik-install-test-programs 1.2
# 	  Selected comments:
# 
# 	    Build the ubik test programs in the test tree instead of the source tree,
# 	    and install them. This way, people wishing to test ubik would be able to
# 	    get a hold of these programs. The programs include udebug, utst_server and
# 	    utst_client.
# 	    This delta is the first in the series of getting automated ubik tests to
# 	    work. The next delta will include some scripts to automatically setup
# 	    and execute some ubik test cases using the above servers.
# 	    add the ubik test program build and install.
# 	    Fix makefile dependencies for utst_server.o and utst_client.o
# 	[1993/03/10  20:19:49  jaffe]
# 
# Revision 1.1.4.3  1993/01/11  18:37:26  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:22:22  htf]
# 
# Revision 1.1.4.2  1992/11/02  15:36:13  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:17:00  root]
# 
# Revision 1.1.2.2  1992/05/21  12:52:49  jaffe
# 	Transarc delta: fred-merge-tests 1.1
# 	  Selected comments:
# 	    Created this delta to merge Phil Hirsch's final DFS test code changes with
# 	    the test code that the OSF had.  Hopefully, this means we and the OSF
# 	    should be looking at the same test code.
# 	    Changes to facilitate merge of local DFS test code with the OSF's.
# 	[1992/05/20  21:11:50  jaffe]
# 
# Revision 1.1  1992/01/19  03:18:52  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#
libutst.a_OFILES	= utst_proc_cstub.o utst_proc_sstub.o

INCLUDES		= utst_proc.h
LIBRARIES		= libutst.a
DATAFILES		= READ_ME HANDTEST \
			  utst_driver ubik.checklist
PROGRAMS		= utst_client utst_server 
EXPDIR			= /usr/include/dcedfs/
libutst.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}
utst_driver_IMODE	= 0755
ILIST			= ${DATAFILES} ${PROGRAMS}

CFLAGS			= -Dunix ${USERCFLAGS}

utst_server_OFILES	= utst_server.o
utst_client_OFILES	= utst_client.o


LIBS			= -lutst -lnubik -licl -lcmd -lafsutil -lncompat -ltpq\
			  -lbomb -lafsutil \
			  -ldacl -ldauth -ldacllfs -ldacl -ldauth -ldacllfs \
			  -losi -lcom_err -ldce  ${OS_LIBS} \
			  -lafssys

IDLFILES		= utst_proc.idl

IDLFLAGS		= -keep c_source -no_cpp -no_mepv 
IDLINCFLAGS		= -I. ${INCDIRS} -I../config

GARBAGE			= utst_proc_cstub.c utst_proc_sstub.c

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

utst_client.o: utst_proc.h
utst_server.o: utst_proc.h

