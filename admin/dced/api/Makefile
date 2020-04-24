# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.5.2  1996/03/09  23:06:45  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:25:33  marty]
#
# Revision 1.1.5.1  1995/12/08  21:25:49  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/09/29  22:38 UTC  tatsu_s
# 	Submitted the fix for CHFts16512.
# 
# 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:15 UTC  tatsu_s
# 	Removed ../server from VPATH.
# 
# 	HP revision /main/HPDCE02/1  1995/07/11  21:52 UTC  mk
# 	Merge in change to CFLAGS from mk_dced_as_rpcd.
# 
# 	HP revision /main/mk_dced_as_rpcd/1  1995/07/11  21:49 UTC  mk
# 	Delete -g from CFLAGS.
# 	[1995/12/08  17:58:39  root]
# 
# Revision 1.1.3.21  1994/10/03  19:19:10  rsalz
# 	Remove references to server_decode (OT CR 11791).
# 	[1994/10/03  19:17:36  rsalz]
# 
# Revision 1.1.3.20  1994/08/25  16:31:58  rsalz
# 	Add dce_serer_register & co.
# 	[1994/08/25  16:28:59  rsalz]
# 
# Revision 1.1.3.19  1994/08/24  19:26:49  bowe
# 	Expunge dced_sesskey.o. [CR 11773]
# 	[1994/08/24  19:09:11  bowe]
# 
# Revision 1.1.3.18  1994/08/23  19:23:56  rsalz
# 	Add idlcompile convenience target.
# 	[1994/08/23  19:18:17  rsalz]
# 
# Revision 1.1.3.17  1994/08/20  18:05:01  bowe
# 	Added dce_server_sec.o,dced_convert_cstub.o. [CR 11791]
# 	[1994/08/20  17:59:40  bowe]
# 
# Revision 1.1.3.16  1994/08/03  22:06:24  pwang
# 	Turned on debug by now (i.e -g)
# 	[1994/08/03  21:56:39  pwang]
# 
# Revision 1.1.3.15  1994/07/28  21:55:33  bowe
# 	Added kmlocal.o, turned on STRICT_COMPILER_WARNINGS. [CR 11280]
# 	[1994/07/28  21:55:05  bowe]
# 
# Revision 1.1.3.14  1994/07/13  15:14:58  marty
# 	CR # 10017 - Set permissions on header files to 444 during install pass
# 	[1994/07/12  15:53:17  marty]
# 
# Revision 1.1.3.13  1994/06/09  16:02:45  devsrc
# 	cr10892 - fix copyright
# 	[1994/06/09  15:48:21  devsrc]
# 
# Revision 1.1.3.12  1994/05/12  19:30:18  rsalz
# 	Add some dce_server functions.
# 	Add and use dced_authorized
# 	More srvr implementation; export their interfaces
# 	[1994/05/12  19:26:17  rsalz]
# 
# Revision 1.1.3.11  1994/05/05  14:41:05  pwang
# 	Added dced_util.h
# 	[1994/05/05  14:00:49  pwang]
# 
# Revision 1.1.3.10  1994/04/18  00:14:52  pwang
# 	Since libdced.a is in libdce now, build libdced.a only
# 	if NO_SHARED_LIBRARIES is on.
# 	[1994/04/17  22:04:29  pwang]
# 
# Revision 1.1.3.9  1994/04/15  22:28:56  pwang
# 	Added dced_util.c
# 	[1994/04/15  21:27:24  pwang]
# 
# Revision 1.1.3.8  1994/04/14  20:08:25  rsalz
# 	Make server and API all compile.
# 	[1994/04/14  19:45:03  rsalz]
# 
# Revision 1.1.3.7  1994/03/18  19:59:02  rsalz
# 	Add ep IDL; standardize IDL compile lines (OT CR 9768)
# 	[1994/03/18  19:58:31  rsalz]
# 
# Revision 1.1.3.6  1994/03/11  00:07:21  pwang
# 	Build dcedhdmsg.c from here since libdce don't know how to build
# 	it from idl dir.
# 	[1994/03/11  00:00:48  pwang]
# 
# Revision 1.1.3.5  1994/03/09  20:43:02  pwang
# 	Merged with changes from 1.1.3.4
# 	[1994/03/09  20:42:56  pwang]
# 
# 	Built client stub code from here to make libdce
# 	Makefile happy
# 	[1994/03/09  20:39:39  pwang]
# 
# Revision 1.1.3.4  1994/03/07  20:36:35  rsalz
# 	Updating to second edition of functional spec.
# 	Moved ACF files to API directory.
# 	[1994/03/07  20:36:19  rsalz]
# 
# Revision 1.1.3.3  1994/02/28  18:38:21  pwang
# 	Added the missed dcedhdmsg.o to the library.
# 	[1994/02/28  18:31:00  pwang]
# 
# Revision 1.1.3.2  1994/02/28  17:21:05  pwang
# 	Initial Implementation.
# 	[1994/02/28  17:20:27  pwang]
# 
# $EndLog$
###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################

libdced_idlOFILES	= srvrconf_cstub.o srvrexec_cstub.o \
			hostdata_cstub.o rkeytab_cstub.o \
			secval_cstub.o dcedhdmsg.o \
			dced_convert_cstub.o
libdced_OFILES		= dced_binding.o dced_common.o \
			dced_server.o dced_hostdata.o \
			dce_server_sec.o dced_rkeytab.o dced_secval.o \
			dced_data.o register.o \
			dced_util_p.o dced_util.o kmlocal.o server.o \
			$(libdced_idlOFILES)

libdced.a_OFILES	= ${libdced_OFILES}

# The rest of the Makefile is "private" for building in the directory
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
#
# The following definitions are for shared libraries:
#   OBJECTS must be defined to pull in the common rules for compilation
#   LIBS    must be defined to prevent -ldce from appearing on compile lines
#
OBJECTS =
LIBS    =

#
#
# Don't want to build any '.a' here if we are using shared libraries
#
.if ${USE_SHARED_LIBRARIES} == 0
  # libraries to build
  LIBRARIES		= libdced.a
  EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}

  # things to install & export
  libdced.a_IDIR	= /usr/lib/
  libdced.a_EXPDIR	= /usr/lib/
.endif

# Search path for idl and sams file
VPATH	= ../idl

# Shared libraries to build
EXPSHLIB_TARGETS	= ${libdced_OFILES}

# Header and exported files
INCLUDES		= dced.h dced_data.h dced_util.h
HFILES 			= dcedimpl.h ${INCLUDES}

# install dir for libs & export dir for libs
IMODE			= 0444
ILIST 			= ${LIBRARIES} dced.h dced_data.h 
IDIR			= /share/include/dce/
EXPDIR			= /usr/include/dce/

# idl files and idl flags
IDLFILES		= hostdata.idl srvrconf.idl \
			  srvrexec.idl rkeytab.idl secval.idl \
			  dced_convert.idl
hostdata_ACF 		=
srvrconf_ACF		=
srvrexec_ACF		=
rkeytab_ACF		=
secval_ACF		=
dced_convert_ACF	=

IDLFLAGS                = -cepv -keep c_source
IDLINCFLAGS		= -I. -I../idl
IDLDATAFILES		= ${IDLFILES:S/.idl$/.h/g}
dced_convert.idl_IDLINCFLAGS	= -I.
dced_convert.idl_IDLFLAGS	=  -no_mepv -keep c_source

# sams file and target
SAMSFILES		= dhd.sams
SAMSFILES_INCLUDES	= dcedhdmsg.h
SAMS_TARGETS		= dcedhdmsg.c

# compiler flags
CFLAGS	 		= ${${TARGET_MACHINE}_CFLAGS}  \
			  ${STRICT_COMPILER_WARNINGS}
INCFLAGS		= -I. -I../idl

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>

idlcompile:	$(libdced_idlOFILES)

.endif
