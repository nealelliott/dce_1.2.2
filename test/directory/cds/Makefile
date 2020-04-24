#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.12.2  1996/03/11  01:43:42  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:14:51  marty]
#
# Revision 1.1.12.1  1995/12/08  22:08:26  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:43:24  root]
# 
# Revision 1.1.10.3  1994/09/07  15:48:40  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:36  marty]
# 
# Revision 1.1.10.2  1994/06/23  18:30:57  mccann
# 	remove subtree and inchname stuff
# 	[1994/06/23  12:52:00  mccann]
# 
# 	remove subtree and inchname stuff
# 
# Revision 1.1.10.1  1994/06/10  17:24:55  jd
# 	Added some things to the install list.
# 	[1994/06/10  14:54:25  jd]
# 
# Revision 1.1.8.3  1993/08/26  21:53:58  pwang
# 	Renamed dacl_start.sh to dacl_setup.sh to match with
# 	documentation [OT#8083]
# 	[1993/08/26  21:39:08  pwang]
# 
# Revision 1.1.8.2  1993/08/19  21:17:12  pwang
# 	Added the rule for cdstest_OFILES [OT#6773] [OT#7835]
# 	[1993/08/19  21:16:30  pwang]
# 
# Revision 1.1.4.6  1993/03/29  20:32:16  jd
# 	Make dcesx
# 	[1993/03/29  20:18:59  jd]
# 
# Revision 1.1.4.5  1993/02/24  13:20:52  jd
# 	Various script fix ups...
# 	[1993/02/23  23:02:13  jd]
# 
# Revision 1.1.4.4  1993/01/28  18:02:21  jd
# 	Removed cdstest.h copy from makefile. It's no longer
# 	referenced by anything in the test directory and is
# 	no longer required. A change to lbe/mk/osf.dce.passes.mk
# 	is also required to clear this bug.
# 	[1993/01/27  21:04:25  jd]
# 
# Revision 1.1.4.3  1993/01/12  18:25:55  bbelch
# 	Embedding copyright notice
# 	[1993/01/07  01:10:12  bbelch]
# 
# Revision 1.1.4.2  1992/11/02  15:32:49  root
# 	Add OTHERS and ILIST for install pass
# 	[1992/10/28  20:11:38  root]
# 
# Revision 1.1.2.2  1992/04/01  22:50:21  grober
# 	Added code necessary to allow cds_clerkserver_cstub.o and cds_types_caux.o
# 	to be built as part of cdstest.  The modifications to this Makefile may end
# 	up being a temporary solution if a cleaner solution can be found.
# 	[1992/04/01  22:03:41  grober]
# 
# Revision 1.1  1992/01/19  14:32:20  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#
#
# Makefile for test directory
#

EXPINC_SUBDIRS  = dcesx 
EXPLIB_SUBDIRS  = dcesx 
EXPBIN_SUBDIRS  = dcesx 
SUBDIRS         = dcesx 
DATA_SUBDIRS    = dcesx 


VPATH    = ../../../directory/cds/stubs

# programs to build
PROGRAMS = cdstest

# idl files
IDLFILES        = cds_clerkserver.idl cds_types.idl
cds_types_ACF =
cds_clerkserver_ACF =

cdstest_OFILES = cdstest.o \
                 cds_clerkserver_cstub.o cds_types_caux.o

DATAFILES	=	\
	cp_abuse.tests \
	cp_childpointer.tests \
	cp_clearinghouse.tests \
	cp_clerk.tests \
	cp_credir.tests \
	cp_directory.tests \
	cp_misc.tests \
	cp_misc_do.cdscp \
	cp_object.tests \
	cp_replica.tests \
	cp_server.tests \
	cp_softlink.tests
	

SCRIPTS  = \
	cp_killer.sh \
	cp_test.sh \
	cdstest.sh \
	dacl_creates.sh \
	dacl_deletes.sh \
	dacl_modifies.sh \
	dacl_reads.sh \
	dacl_replicas.sh \
	dacl_testing.sh \
	dacl_setup.sh

ILIST	= ${PROGRAMS} ${SCRIPTS} ${DATAFILES}

# Compiler flags for this dir
CFLAGS	 = -Dunix -DDEBUG -DDNS_RPC -DDNS_V3API -DDNS_CDS -DDCE_SEC -DDNS_CMA

# idl flags.  
IDLFLAGS        ?= -space_opt -keep c_source `genpath ${INCFLAGS}` ${INCDIRS}

RIOS_LIBS= -lm -lbsd
LIBS     = -ldce ${${TARGET_MACHINE}_LIBS}

INCFLAGS = -I../../../directory/cds/includes

IDLINCFLAGS = -I../../../directory/cds/stubs

.include <${RULES_MK}>

${cdstest_OFILES} : ${IDLFILES:.idl=.h}

GARBAGE = cds_clerkserver_cstub.c cds_types_caux.c \
	  cds_clerkserver.h cds_types.h
