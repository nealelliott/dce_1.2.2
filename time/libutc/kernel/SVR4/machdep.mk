# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# @SNI_COPYRIGHT@
# 
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.5.2  1996/03/09  23:31:38  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:47  marty]
#
# Revision 1.1.5.1  1995/12/08  18:12:18  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:25:05  root]
# 
# Revision 1.1.3.3  1992/12/30  20:41:53  htf
# 	Embedded copyright notice
# 	removed extra HISTORY and Log sections
# 	[1992/12/30  20:17:38  htf]
# 
# Revision 1.1.3.2  1992/09/29  21:27:50  devsrc
# 	[OT 5373]    SNI/SVR4 merge.
# 	[1992/09/18  18:13:10  sekhar]
# 
# Revision 1.1.2.2  1992/09/18  18:15:35  sekhar
# 	[OT 5373]    SNI/SVR4 merge.
# 	[1992/09/18  18:13:10  sekhar]
# 
# Revision 1.1.1.2  1992/09/18  18:13:10  sekhar
# 	[OT 5373]    SNI/SVR4 merge.
# 
# Revision 9.1  92/07/19  20:10:10  hinman
# 	Create split point
# 
# Revision 1.1.2.7  92/07/14  17:27:05  karr
# 	utc_gettime.o, utc_adjtime.o now depend upon .c files.
# 	[92/07/14  17:15:20  karr]
# 
# Revision 1.1.2.6  92/04/28  11:40:31  tikku
# 	Back to using sysent entries for the DTS system calls.
# 	[92/04/28  11:39:12  tikku]
# 
# Revision 1.1.2.5  92/04/17  11:03:10  tikku
# 	changed the .s files to .c files
# 	[92/04/17  11:02:30  tikku]
# 
# Revision 1.1.2.4  92/04/13  11:31:07  tikku
# 	Moved kern_utctime.c out of this directory. So all references
# 	to it in this file had to be removed.
# 	[92/04/13  11:29:49  tikku]
# 
# Revision 1.1.2.3  92/04/10  10:10:00  tikku
# 	Real initial version.
# 	[92/04/10  10:01:03  tikku]
# 
# Revision 1.1.2.2  92/04/06  09:10:02  tikku
# 	This is the real initial version!
# 	[92/04/06  09:00:59  tikku]
# 
# 	Initial version
# 	[92/04/03  15:40:14  tikku]
# 
# $EndLog$
# 

SVR4_libutc_OFILES = utc_gettime.o utc_adjtime.o

utc_gettime.o : utc_gettime.c
utc_adjtime.o : utc_adjtime.c
