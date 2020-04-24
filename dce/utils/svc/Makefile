#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.8.1  1996/06/04  21:45:03  arvind
# 	Fix compile warnings
# 	[1996/03/09  23:11:15  marty  1.1.6.2]
#
# Revision 1.1.6.2  1996/03/09  23:11:15  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:28:39  marty]
# 
# Revision 1.1.6.1  1995/12/08  21:37:25  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  18:09:20  root]
# 
# Revision 1.1.4.18  1994/09/07  21:04:58  bowe
# 	Added msginqcma.o [CR 11885]
# 	[1994/09/07  21:03:49  bowe]
# 
# Revision 1.1.4.17  1994/07/27  12:27:54  rsalz
# 	Turn off strict compiler warnings on HP to avoid lots of
# 	messages from their bug in <stdarg.h>
# 	[1994/07/27  03:39:28  rsalz]
# 
# 	Silence compiler warnings (OT CR 11455).
# 	[1994/07/27  03:33:27  rsalz]
# 
# Revision 1.1.4.16  1994/07/07  19:45:59  rsalz
# 	First part of removing SVC ACL Manager code (OT CR 10856).
# 	[1994/07/07  19:24:39  rsalz]
# 
# Revision 1.1.4.15  1994/06/09  16:05:50  devsrc
# 	cr10892 - fix copyright
# 	[1994/06/09  15:50:24  devsrc]
# 
# Revision 1.1.4.14  1994/05/27  18:27:03  bowe
# 	Set IDLINCFLAGS and INCFLAGS for building svcbin.h and svcbin_cstub.*
# 	(needs utctypes.{h,idl}) [CR 10797]
# 	[1994/05/27  18:26:35  bowe]
# 
# Revision 1.1.4.13  1994/05/26  18:51:34  bowe
# 	Added references to svcbin, svcdumplog.
# 	[1994/05/26  18:10:43  bowe]
# 
# Revision 1.1.4.12  1994/05/12  14:06:22  rsalz
# 	Don't append .cat to message catalog on HP (OT CR 10480).
# 	[1994/05/12  13:48:41  rsalz]
# 
# Revision 1.1.4.11  1994/04/25  21:41:59  salamone
# 	CR10430 - Export service.idl for dcecp
# 	[1994/04/25  21:41:24  salamone]
# 
# Revision 1.1.4.10  1994/04/19  20:01:52  rsalz
# 	Install dumplog in right place (OT CR 10382).
# 	Add a sample SVC routing file (OT CR 10397).
# 	[1994/04/19  16:39:28  rsalz]
# 
# Revision 1.1.4.9  1994/03/18  17:15:42  rsalz
# 	Fix sams flag so dcesvc.cat is generated (OT CR 10162).
# 	[1994/03/18  17:08:05  rsalz]
# 
# Revision 1.1.4.8  1994/03/16  03:50:13  rsalz
# 	More cleanups; get dumplog to build (OT CR 9823).
# 	[1994/03/15  20:05:50  rsalz]
# 
# 	Add service.acf (OT CR 9935); also Makefile cleanups.
# 	[1994/03/15  17:09:30  rsalz]
# 
# Revision 1.1.4.7  1994/01/28  18:03:15  bowe
# 	Comment out PROGRAMS (temporarily to fix rios build).
# 	[1994/01/28  18:02:14  bowe]
# 
# Revision 1.1.4.6  1994/01/24  22:01:50  bowe
# 	Set ILIST to the INCLUDES, not INCLUDES and LIBRARIES.
# 	Remove libsvc.a_IDIR, since that is not installed.
# 	[1994/01/24  21:56:57  bowe]
# 
# Revision 1.1.4.5  1994/01/18  23:17:33  bowe
# 	Set IDIR to the right value.
# 	[1994/01/18  23:16:32  bowe]
# 
# Revision 1.1.4.4  1994/01/03  19:35:04  rsalz
# 	Added dce_error.h from rpc/sys_idl.
# 	[1994/01/03  15:34:16  rsalz]
# 
# Revision 1.1.4.3  1993/12/20  20:50:42  rsalz
# 	Merge msg.sams into svc.sams
# 	[1993/12/20  20:50:18  rsalz]
# 
# Revision 1.1.4.2  1993/12/14  23:24:27  mario
# 	Added LIBRARIES and libsvc.a_EXPDIR so that libsvc.a gets placed in usr/lib
# 	[1993/12/14  23:24:15  mario]
# 
# Revision 1.1.4.1  1993/12/14  16:57:54  mario
# 	Changes required to get svc built in libdce
# 	[1993/12/14  16:57:22  mario]
# 
# Revision 1.1.2.4  1993/10/07  17:07:20  bowe
# 	Fixed type-o in target.  "dcesvcsmsg" changed to "dcesvcs.msg".
# 	[1993/10/07  16:59:14  bowe]
# 
# Revision 1.1.2.3  1993/08/25  20:17:59  rsalz
# 	Set IDIR, etc., so install pass works.
# 	Remove outdated manpage targets.
# 	[1993/08/25  20:17:09  rsalz]
# 
# Revision 1.1.2.2  1993/08/16  18:07:48  rsalz
# 	Initial release
# 	[1993/08/16  18:02:13  rsalz]
# 
# $EndLog$


###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
libsvc_OFILES	= ${MSGOFILES} ${SVCOFILES}

libsvc.a_OFILES	= ${libsvc_OFILES}

MSGOFILES	= \
	msgdeftable.o msggetdef.o msggetmsg.o msgget.o msgtable.o \
	msgtrans.o msgxpgcat.o msgpthread.o rpmatch.o msginqcma.o
SVCOFILES	= \
	dceassert.o dceprintf.o \
	svcprintf.o svcreg.o svcroute.o svcfilter.o svclog.o \
	svcpthread.o svcremimpl.o dcesvcmsg.o service_sstub.o \
	svcbin_cstub.o

# The rest of the makefile is "private" for use when building in this
# directory.
.if	!defined(LIBDCE_LIBRARY_MAKEFILE)

# The following definitions are for shared libraries:
#   OBJECTS must be defined to pull in the common rules for compilation
#   LIBS    must be defined to prevent -ldce from appearing on compile lines
OBJECTS	=
LIBS	=

# Don't want to build any '.a' here if we are using shared libraries
LIBRARIES	= libsvc.a
.if ${USE_SHARED_LIBRARIES} == 0
    EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
.endif

HP800_CFLAGS	= -DNO_CATALOG_SUFFIX
CFLAGS		+= ${${TARGET_MACHINE}_CFLAGS} ${STRICT_COMPILER_WARNINGS}
svcprintf.o_CFLAGS += ${${TARGET_MACHINE}_CFLAGS} \
			${STRICT_COMPILER_WARNINGS:S/+w1//}

PROGRAMS 	= svcdumplog
dcesvc_OFILES	= service_cstub.o
LIBS		= -ldce

EXPSHLIB_TARGETS= ${libsvc_OFILES}
EXPDIR          = /usr/include/dce/
libsvc.a_EXPDIR = /usr/lib/

IDIR		= /share/include/dce/
dcesvc.cat_IDIR	= /nls/msg/en_US.ASCII/
ILIST		= ${INCLUDES} ${PROGRAMS} dcesvc.cat routing
IMODE		= 444
svcdumplog_IMODE = 555
svcdumplog_IDIR	= /bin/
routing_IDIR	= /share/var/svc/
routing_IMODE	= 644

svcdumplog_OFILES = svcdumplog.o

INCLUDES	= assert.h dce_msg.h dce_svc.h \
		svcfilter.h svclog.h svcremote.h \
		dcesvcmsg.h service.h dce_error.h \
		svcbin.h

SAMIDL_INCLUDES	= service.idl service.h svcbin.idl svcbin.h
SAMS_TARGETS	= dcesvcmsg.h dcesvcmsg.c dcesvc.cat

IDLFILES	= service.idl svcbin.idl
IDLFLAGS	= -no_mepv -keep c_source
# Remove this when top-level Makefile visits time (or time/common?)
# before dce/utils/svc in the export pass.
svcbin.h_IDLINCFLAGS		= -I../../../time/common
svcbin_cstub.c_IDLINCFLAGS	= -I../../../time/common
svcbin_cstub.o_INCFLAGS		= -I../../../time/common
service_ACF	=
svcbin_ACF	=

.include <${RULES_MK}>

idlcompile:		service_cstub.o service_sstub.o svcbin_cstub.o

#${MSGOFILES}:	msgimpl.h dcesvcmsg.h
#${SVCOFILES}:	service.h svcimpl.h dcesvcmsg.h svcbin.h

dcesvc.msg:	svc.sams
	${SAMS} ${SAMSFLAGS} -om ${.ALLSRC}
dcesvcmsg.h:	svc.sams
	${SAMS} ${SAMSFLAGS} -oh ${.ALLSRC}
dcesvcmsg.c:	svc.sams
	${SAMS} ${SAMSFLAGS} -ot ${.ALLSRC}
dcesvc.cat:	svc.sams
	${SAMS} ${SAMSFLAG} -omc ${.ALLSRC}
.endif
