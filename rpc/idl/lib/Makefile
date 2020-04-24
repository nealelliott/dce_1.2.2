# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.15.2  1996/03/09  23:25:52  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:33  marty]
#
# Revision 1.2.15.1  1995/12/07  22:23:37  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/dat_xidl2/1  1995/11/17  17:05 UTC  dat
# 	Merge second XIDL drop for DCE 1.2.1
# 	[1995/12/07  21:13:35  root]
# 
# Revision 1.2.12.2  1994/08/18  22:09:26  hopkins
# 	Serviceability:
# 	  Set INCFLAGS for ctxeecli.o
# 	  Add -DDCE_RPC_SVC to CFLAGS
# 	[1994/08/18  21:47:09  hopkins]
# 
# Revision 1.2.12.1  1994/05/02  22:43:34  tom
# 	From DEC: Add binding callout function.
# 	 Add bindcall.o to user_ofiles.
# 	[1994/05/02  21:06:29  tom]
# 
# Revision 1.2.8.2  1993/07/07  20:04:14  ganni
# 	reduced stub idl sources
# 	[1993/07/07  19:35:07  ganni]
# 
# Revision 1.2.6.3  1993/01/03  22:11:56  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  14:38:39  bbelch]
# 
# Revision 1.2.6.2  1992/12/23  19:08:43  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  01:06:01  zeliff]
# 
# Revision 1.2.2.2  1992/05/11  17:51:08  mhickey
# 	Removed obsolete define of NEED_PROJECT_MAKE_DEFS
# 	and include of osf.dce.mk.  Fixes defect 2031.
# 	[1992/04/22  15:21:51  mhickey]
# 
# Revision 1.2  1992/01/19  22:13:29  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#  OSF DCE Version 1.0 
#
# src/idl/lib/Makefile for the IDL runtime library, libidl.a
#

###########################################################
# Define OBJECT files needed to build libidl.a
###  DO NOT PUT ANYTHING ELSE IN THIS SECTION
###########################################################
#
libidl_OFILES	= ${IDLLIB_USER_OFILES} ${IDLLIB_KERNEL_OFILES}

libidl.a_OFILES		= ${libidl_OFILES}

# Files needed by both KERNEL and USER space
IDLLIB_KERNEL_OFILES= alfrsupp.o allocate.o ctxeecli.o ctxeectx.o ctxerrtl.o \
		  eenodtbl.o ernodtbl.o marbfman.o ndrchars.o nbaseool.o \
		  ndrcharp.o nidlalfr.o pipesupp.o sscmaset.o sscmasrv.o \
		  ndrui.o ndrui2.o ndrui3.o ndrmi.o ndrmi2.o ndrmi3.o interpsh.o

# Files needed only by USER space
IDLLIB_USER_OFILES= eebool.o eebyte.o eechar.o eedouble.o eeenum.o eefloat.o \
		  erbool.o erbyte.o erchar.o erdouble.o erenum.o erfloat.o \
		  eehyper.o eelong.o eeshort.o eesmall.o eeuhyper.o \
		  erhyper.o erlong.o ershort.o ersmall.o eruhyper.o \
		  erulong.o erushort.o erusmall.o \
		  eeulong.o eeushort.o eeusmall.o \
		  ndrfloat.o autohndl.o cvt_glob.o ndrmi5.o ndrui5.o \
                  pickling.o bindcall.o

.if !defined(LIBDCE_LIBRARY_MAKEFILE)
#
# The following definitions are for shared libraries:
#   OBJECTS must be defined to pull in the common rules for compilation
#   LIBS    must be defined to prevent -ldce from appearing on compile lines
#
OBJECTS =
LIBS    =

#
# Don't want to build any '.a' here if we are using shared libraries
#
.if ${USE_SHARED_LIBRARIES} == 0
  # Library to build and export, and its location
  LIBRARIES		= libidl.a
.endif

EXPSHLIB_TARGETS	= ${libidl_OFILES}

EXPDIR			= /usr/lib/
CFLAGS			= ${${TARGET_MACHINE}_CFLAGS} -DMIA -DDCE_RPC_SVC

ctxeecli.o_INCFLAGS	= -I../../runtime

#
#standard set of rules
#
.include <${RULES_MK}>

ndrfloat.o: ndrfloat.c cvt.h cvt_pvt.h round.c \
      pkcray.c  pkibml.c  pkibms.c  pkieees.c  pkieeet.c  pkvaxf.c  pkvaxg.c \
      upkcray.c upkibml.c upkibms.c upkieees.c upkieeet.c upkvaxf.c upkvaxg.c

.endif
