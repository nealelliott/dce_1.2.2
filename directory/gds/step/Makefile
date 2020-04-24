# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  13:23:47  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:12:18  marty]
#
# Revision 1.1.4.1  1995/12/08  17:45:56  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:24:30  root]
# 
# Revision 1.1.2.8  1994/09/19  08:39:40  marrek
# 	Fixes for OT12176 (wrong incore tables.)
# 	[1994/09/19  08:33:53  marrek]
# 
# Revision 1.1.2.7  1994/09/06  20:41:26  bowe
# 	Remove reference to ../../../dce/utils/svc [CR 12055]
# 	[1994/09/06  20:40:44  bowe]
# 
# Revision 1.1.2.6  1994/09/06  16:30:15  keutel
# 	OT 12055: dce_svc_log_get_arg will become obsolete
# 	[1994/09/06  16:10:27  keutel]
# 
# Revision 1.1.2.5  1994/07/06  15:08:15  marrek
# 	July 1994 code drop.
# 	[1994/07/06  12:02:55  marrek]
# 
# Revision 1.1.2.4  1994/06/21  14:48:02  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:44:48  marrek]
# 
# Revision 1.1.2.3  1994/06/10  21:15:48  devsrc
# 	cr10871 - fix copyright
# 	[1994/06/10  14:54:02  devsrc]
# 
# Revision 1.1.2.2  1994/05/10  16:03:04  marrek
# 	Bug fixes for April 1994 submission.
# 	[1994/05/02  13:22:03  marrek]
# 
# 	April 1994 code submission.
# 	[1994/04/27  13:10:51  marrek]
# 
# Revision 1.1.2.1  1994/03/23  15:46:58  keutel
# 	creation
# 	[1994/03/21  15:54:24  keutel]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:47 $
#

PROGRAMS        = gdsstep

gdsstep_IMODE    = 755

ILIST         = ${PROGRAMS}

IDIR          = /bin/

INCFLAGS      = -I../h -I../cisam/isam

.include <${RULES_MK}>

gdsstep_LIBS   = -lisam ${LIBS}
gdsstep_OFILES = gstep.o gstisam.o gstutil.o gstapdu.o \
		 gstasn1.o gstaval.o

