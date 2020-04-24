# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.4.2  1996/03/09  23:14:51  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:32:10  marty]
#
# Revision 1.1.4.1  1995/12/08  16:14:49  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:20:35  root]
# 
# Revision 1.1.2.5  1994/07/27  08:15:10  marrek
# 	Remove -lsocket.
# 	[1994/07/27  08:14:07  marrek]
# 
# Revision 1.1.2.4  1994/06/21  14:48:10  marrek
# 	June 1994 code submission.
# 	[1994/06/17  11:40:04  marrek]
# 
# Revision 1.1.2.3  1994/06/10  21:16:17  devsrc
# 	cr10871 - fix copyright
# 	[1994/06/10  14:54:21  devsrc]
# 
# Revision 1.1.2.2  1994/05/10  16:03:19  marrek
# 	Remove linking of libnsl.
# 	[1994/05/09  11:21:49  marrek]
# 
# 	April 1994 code submission.
# 	[1994/04/27  13:11:56  marrek]
# 
# Revision 1.1.2.1  1994/03/23  15:47:20  keutel
# 	creation
# 	[1994/03/21  15:54:23  keutel]
# 
# $EndLog$
#
# $RCSfile: machdep.mk,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:14:51 $
#

gdscstub_LIBS	= -ld24asn -ld2aasn -ldriv25 -lrosx -lcom -lrosx -lcom \
	   	  -lrtrosasn1 -ltpdu -ltsi \
		  -lDUA -lids -lm ${${TARGET_MACHINE}_LIBS}  \
		  -lxti -ldl -lNDS ${LIBS}
gdssstub_LIBS	= -ldriv25 -lrosx -lcom -lrosx -lcom -ltpdu \
	   	  -lrtrosasn1 -ltsi \
		  -lm ${${TARGET_MACHINE}_LIBS} \
		  -lxti	-ldl -lNDS ${LIBS}
