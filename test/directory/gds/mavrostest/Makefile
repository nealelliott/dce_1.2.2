#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.13.2  1996/03/11  01:46:05  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:16:27  marty]
#
# Revision 1.1.13.1  1995/12/08  22:11:19  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:44:47  root]
# 
# Revision 1.1.10.7  1994/09/02  12:50:12  keutel
# 	last fix was too quick
# 	[1994/09/02  10:32:42  keutel]
# 
# Revision 1.1.10.6  1994/09/01  14:50:08  keutel
# 	11991: remove h-files from ILIST
# 	[1994/09/01  12:40:38  keutel]
# 
# Revision 1.1.10.5  1994/06/22  12:17:53  zeliff
# 	ot11034: remove -lasn1 from LIBS lines
# 	[1994/06/22  12:17:43  zeliff]
# 
# Revision 1.1.10.4  1994/05/10  15:32:27  marrek
# 	Merged with changes from 1.1.10.3
# 	[1994/05/10  15:31:25  marrek]
# 
# 	April 1994 code submission.
# 
# Revision 1.1.10.2  1994/02/25  19:32:27  keutel
# 	OT 10018
# 	[1994/02/25  15:30:42  keutel]
# 
# 	OT 10012
# 
# Revision 1.1.10.1  1994/02/22  18:12:40  marrek
# 	Code drop for dce1.1.
# 	[1994/02/07  11:54:57  marrek]
# 
# Revision 1.1.8.1  1993/10/27  15:23:05  zeliff
# 	OT9269: Fixes to make mavros test work as documented
# 	[1993/10/27  15:18:21  zeliff]
# 
# Revision 1.1.6.3  1993/08/12  11:26:42  marrek
# 	July 1993 code drop.
# 	[1993/08/04  12:19:18  marrek]
# 
# Revision 1.1.6.2  1993/07/12  20:58:19  treff
# 	DCT @ OSF
# 	Make sure HP800 doesn't load -lbsd
# 	[1993/07/12  20:10:03  treff]
# 
# Revision 1.1.4.4  1993/02/18  04:27:49  treff
# 	2/17/93 DCT@OSF
# 	Fixed it to build, and run, on both platforms.
# 	Specifically fixed -I and -L lines, DELTA, library
# 	loading order, moving of ALL datafiles to obj tree.
# 	[1993/02/18  04:27:23  treff]
# 
# Revision 1.1.4.3  1993/01/04  17:02:25  htf
# 	Embedded copyright notice
# 	[1993/01/04  16:15:24  htf]
# 
# Revision 1.1.4.2  1992/11/02  15:32:54  root
# 	Corrected ILIST and commented out IDIR as IDIR is set in RULES_MK
# 	[1992/10/28  20:11:47  root]
# 
# Revision 1.1.2.6  1992/07/21  21:45:09  melman
# 	CR 4781: removed running of test_mvr from build
# 	[1992/07/21  21:43:00  melman]
# 
# Revision 1.1.2.5  1992/07/06  16:35:37  melman
# 	Added dependency so that test.h is needed by import1.o and attrdec.o.
# 	[1992/07/06  16:33:08  melman]
# 
# Revision 1.1.2.4  1992/06/12  21:20:17  melman
# 	Added ./ in front of test_mvr and oidt in rules at bottom of
# 	Makefile.  On the RIOS, . is not in the PATH.
# 	[1992/06/12  21:18:50  melman]
# 
# Revision 1.1.2.3  1992/06/11  21:51:16  melman
# 	New GDS test code drop.
# 	[1992/06/11  21:44:50  melman]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.13.2 $ $Date: 1996/03/11 01:46:05 $
#

PROGRAMS	= test_mvr oidt  

DATAFILES	= oidt.input oidt.output  test.dump test.input time.test \
		  test.x509 test.entries test.failures test.text_out \
		  test.text_ref test_mvr.sh

ILIST		= ${PROGRAMS} ${DATAFILES}

GARBAGE = test.h test.c test.make

test_mvr_LIBS     = -lm -ldce
test_mvr_OFILES = test.o test_mvr.o import1.o attrdec.o

test_mvr: test.h test.c

oidt_LIBS   = -ldce
oidt_OFILES = oidt.o 

test.h test.c:  test.mvr test_mvr.h
	${MAVROS} ${test.mvr:P} -m test -w -u -c -a -x 8 -d ${test_mvr.h:P} \
       	    -s ${CFILES} -i  import1.h
	
.include <${RULES_MK}>

