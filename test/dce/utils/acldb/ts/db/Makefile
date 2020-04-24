# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  01:43:23  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:14:32  marty]
#
# Revision 1.1.4.1  1995/12/08  22:07:20  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:42:53  root]
# 
# Revision 1.1.2.4  1994/09/02  18:34:31  pwang
# 	Removed extra linked libraries [OT#11402]
# 	[1994/09/02  18:15:39  pwang]
# 
# Revision 1.1.2.3  1994/06/10  20:18:37  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:57:58  devsrc]
# 
# Revision 1.1.2.2  1994/05/19  14:53:34  baum
# 	Resync with recent DB & ACL API changes
# 	[1994/05/19  14:53:09  baum]
# 
# Revision 1.1.2.1  1994/04/07  19:26:58  baum
# 	INitial checkin
# 	[1994/04/07  19:26:44  baum]
# 
# $EndLog$

STANDARD_TETLIBS= -ltettcm -ltetapi


CFLAGS = ${${TARGET_MACHINE}_CFLAGS} -D _USE_TET 
# CFLAGS = ${${TARGET_MACHINE}_CFLAGS} 

IDLFILES = dbtest.idl
IDLFLAGS	= -keep c_source


PROGRAMS = dce_db_misc dce_db_delete dce_db_fetch dce_db_store dce_db_open \
	   dce_db_close dce_db_iter 

dce_db_misc_OFILES =  dbtest_cstub.o dce_db_misc.o db_test_utils.o
dce_db_open_OFILES =  dbtest_cstub.o dce_db_open.o db_test_utils.o
dce_db_close_OFILES =  dbtest_cstub.o dce_db_close.o db_test_utils.o
dce_db_store_OFILES =  dbtest_cstub.o dce_db_store.o db_test_utils.o
dce_db_fetch_OFILES =  dbtest_cstub.o dce_db_fetch.o db_test_utils.o
dce_db_delete_OFILES =  dbtest_cstub.o dce_db_delete.o db_test_utils.o
dce_db_iter_OFILES =  dbtest_cstub.o dce_db_iter.o db_test_utils.o

LIBS = -ldce ${STANDARD_TETLIBS} ${${TARGET_MACHINE}_LIBS} 

ILIST		= ${PROGRAMS}
IDIR		= /test/tet/functional/dce/utils/acldb/ts/db/
IMODE		= 755

.include <${RULES_MK}>

dbtest_cstub.o : dbtest.idl
dbtest.h : dbtest.idl
dce_db_store.o : dbtest.h db_test_utils.h
dce_db_delete.o : dbtest.h db_test_utils.h
dce_db_fetch.o : dbtest.h db_test_utils.h
dce_db_open.o : dbtest.h db_test_utils.h
dce_db_close.o : dbtest.h db_test_utils.h
dce_db_iter.o : dbtest.h db_test_utils.h
dce_db_misc.o : dbtest.h db_test_utils.h
db_test_utils.o : dbtest.h db_test_utils.h

