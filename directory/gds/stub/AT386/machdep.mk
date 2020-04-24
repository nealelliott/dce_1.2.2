# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.4.2  1996/03/09  23:14:47  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:32:06  marty]
#
# Revision 1.1.4.1  1995/12/08  16:14:34  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:20:28  root]
# 
# Revision 1.1.2.3  1994/06/10  21:16:12  devsrc
# 	cr10871 - fix copyright
# 	[1994/06/10  14:54:17  devsrc]
# 
# Revision 1.1.2.2  1994/05/10  16:03:14  marrek
# 	April 1994 code submission.
# 	[1994/04/27  13:11:49  marrek]
# 
# Revision 1.1.2.1  1994/03/23  15:47:15  keutel
# 	creation
# 	[1994/03/21  15:54:20  keutel]
# 
# $EndLog$
#
# $RCSfile: machdep.mk,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:14:47 $
#

gdscstub_LIBS	= -ld24asn -ld2aasn -ldriv25 -lrosx -lcom -lrosx -lcom \
	   	  -lrtrosasn1 -ltpdu -ltsi \
		  -lDUA -lids -lm ${${TARGET_MACHINE}_LIBS}  \
		  -lxti -lNDS ${LIBS}
gdssstub_LIBS	= -ldriv25 -lrosx -lcom -lrosx -lcom -ltpdu \
	   	  -lrtrosasn1 -ltsi \
		  -lm ${${TARGET_MACHINE}_LIBS} \
		  -lxti -lNDS ${LIBS}
