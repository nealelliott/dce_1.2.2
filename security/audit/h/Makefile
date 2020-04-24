# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# (c) Copyright International Business Machines Corp. 1994, 1996
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/07/08  18:26:25  arvind
# 	Audit for 1.2.2
# 	[1996/06/21  20:24 UTC  arvind  /main/arvind_audit122/1]
#
# 	Audit for 1.2.2
# 	[1996/03/09  23:27:03  marty  1.1.4.2]
#
# Revision 1.1.4.2  1996/03/09  23:27:03  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:42:34  marty]
# 
# Revision 1.1.4.1  1995/12/08  17:39:36  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:17:47  root]
# 
# Revision 1.1.2.4  1994/04/07  10:22:30  kevins
# 	CR 10304 Export some header files for cross component dependency issues
# 	[1994/04/07  10:18:15  kevins]
# 
# Revision 1.1.2.3  1994/01/14  21:48:29  zeliff
# 	OT9744:  Added EXPDIR because if you have IDIR, and no EXPDIR,
# 	then EXPDIR defaults to the IDIR value.
# 	[1994/01/14  21:45:29  zeliff]
# 
# Revision 1.1.2.2  1994/01/14  17:56:02  zeliff
# 	OT9744: change IDIR for header files
# 	[1994/01/14  17:50:26  zeliff]
# 
# Revision 1.1.2.1  1993/12/22  12:39:00  zeliff
# 	First version of code
# 	[1993/12/21  15:01:15  zeliff]
# 
# $EndLog$
#

# INCLUDES = audit.h dmn_threadp.h error_statusp.h \
#            eslp.h esl_filep.h pathsp.h evtp.h
INCLUDES = audit.h eslp.h
EXPDIR = /usr/include/dce/

ILIST = audit.h
IDIR = /share/include/dce/

.include <${RULES_MK}>
