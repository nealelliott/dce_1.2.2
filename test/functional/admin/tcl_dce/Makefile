# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.10.1  1996/06/04  22:05:58  arvind
# 	Merge OSF endgame to HP.
# 	[1996/03/14  19:29 UTC  psn  /main/DCE_1.2/2]
#
# 	Fix OT13354 as was done during OSF 1.2.1 Endgame.
# 	[1996/02/28  22:27 UTC  psn  /main/DCE_1.2/psn_1.2_endgame/1]
#
# Revision 1.1.8.4  1996/03/11  02:30:45  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:37:50  marty]
# 
# Revision 1.1.8.3  1996/02/23  16:40:58  root
# 	ENDGAME: Fix VPATH to find tclTest.o
# 	[1996/02/23  16:32:54  root]
# 
# Revision 1.1.8.2  1995/12/11  15:21:17  root
# 	Submit
# 
# 	HP revision /main/HPDCE02/2  1995/06/07  13:48 UTC  root
# 	build tcltest
# 
# 	HP revision /main/HPDCE02/2  1995/06/07  13:48 UTC  mothra
# 	build tcltest
# 
# 	HP revision /main/HPDCE02/1  1995/05/11  12:43 UTC  root
# 	fix IDIR
# 	[1995/12/11  14:34:16  root]
# 
# Revision 1.1.4.1  1994/10/26  21:03:13  mori_m
# 	CR 11259:  Added multi-bytes functional tests.
# 	[1994/10/26  20:51:56  mori_m]
# 
# Revision 1.1.2.1  1994/09/28  21:48:25  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:16:03  melman]
# 
# Revision 1.1.2.2  1994/06/10  20:16:33  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:39  devsrc]
# 
# Revision 1.1.2.1  1994/03/01  21:05:54  rousseau
# 	Initial version.
# 	[1994/03/01  21:01:53  rousseau]
# 
# $EndLog$

TCL_DIR		 = ${SOURCEBASE}/admin/tcl_dce:../../../../admin/tcl_dce
#TCL_TEST_DIR	 = ${TCL_DIR}/tests

VPATH		 = ${TCL_DIR}:../../../../admin/tcl_dce

SCRIPTS	 	 = all_tests \
		   append.test \
		   async.test \
		   case.test \
		   cd.test \
		   cmdinfo.test \
		   concat.test \
		   dcall.test \
		   defs \
		   dstring.test \
		   env.test \
		   error.test \
		   eval.test \
		   exec.test \
		   expr.test \
		   file.test \
		   for.test \
		   format.test \
		   glob.test \
		   history.test \
		   if.test \
		   incr.test \
		   info.test \
		   join.test \
		   lindex.test \
		   link.test \
		   linsert.test \
		   list.test \
		   llength.test \
		   lrange.test \
		   lreplace.test \
		   lsearch.test \
		   lsort.test \
		   misc.test \
		   open.test \
		   parse.test \
		   pid.test \
		   proc.test \
		   regexp.test \
		   rename.test \
		   scan.test \
		   set.test \
		   source.test \
		   split.test \
		   string.test \
		   switch.test \
		   trace.test \
		   unknown.test \
		   uplevel.test \
		   upvar.test \
		   while.test

PROGRAMS        = tcltest
OFILES          = tclTest.o
INCFLAGS        = -I${EXPORTBASE}/usr/include/dce -I../../../../admin/tcl_dce \
                  -I../../../../admin/tcl7.3
LIBS            = -ltcl ${${TARGET_MACHINE}_LIBS} -lm -ldce

DATAFILES	= ${SCRIPTS}
ILIST		= ${SCRIPTS} ${PROGRAMS}
IDIR            = /test/tet/functional/admin/tcl_dce/
SUBDIRS		= MULTIBYTES
DATA_SUBDIRS	= ${SUBDIRS}


.include <${RULES_MK}>

# Tcl's self test suite
tests: tcltest
	TCL_LIBRARY=${TCL_DIR}; export TCL_LIBRARY; \
	( cd ${TCL_DIR}; ./tcltest all_tests )

