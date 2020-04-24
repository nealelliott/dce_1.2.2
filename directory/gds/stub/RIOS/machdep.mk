# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.10.2  1996/03/09  23:14:50  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:32:09  marty]
#
# Revision 1.1.10.1  1995/12/08  16:14:45  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:20:33  root]
# 
# Revision 1.1.8.3  1994/05/10  16:03:18  marrek
# 	April 1994 code submission.
# 	[1994/04/27  13:11:53  marrek]
# 
# Revision 1.1.8.2  1994/03/23  15:47:19  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:33:49  keutel]
# 
# Revision 1.1.8.1  1994/02/22  19:21:52  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  11:53:08  marrek]
# 
# Revision 1.1.6.2  1993/08/11  08:18:18  marrek
# 	July 1993 code drop.
# 	[1993/08/02  12:17:31  marrek]
# 
# Revision 1.1.4.2  1992/12/31  22:03:15  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:47:24  bbelch]
# 
# Revision 1.1.2.2  1992/06/01  23:08:02  melman
# 	Created file with d24stub.o_CFLAGS and d25stub.o_CFLAGS and RIOS_CFLAGS
# 	[1992/06/01  19:08:05  melman]
# 
# $EndLog$
#
# $RCSfile: machdep.mk,v $ $Revision: 1.1.10.2 $ $Date: 1996/03/09 23:14:50 $
#

gdscstub_LIBS	= -ld24asn -ld2aasn -ldriv25 -lrosx -lcom -lrosx -lcom \
	   	  -lrtrosasn1 -ltpdu -ltsi \
		  -lDUA -lids -lm ${${TARGET_MACHINE}_LIBS}  \
		  -lxti -lNDS ${LIBS}
gdssstub_LIBS	= -ldriv25 -lrosx -lcom -lrosx -lcom -ltpdu \
	   	  -lrtrosasn1 -ltsi \
		  -lm ${${TARGET_MACHINE}_LIBS} \
		  -lxti -lNDS ${LIBS}

d24stub.o_CFLAGS	=  -DCSTUB -D_POSIX_SOURCE
d25stub.o_CFLAGS	=  -D_POSIX_SOURCE

RIOS_CFLAGS = -D_ALL_SOURCE
