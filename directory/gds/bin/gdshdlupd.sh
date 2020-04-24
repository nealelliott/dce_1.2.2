#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: gdshdlupd.sh,v $
# Revision 1.1.10.2  1996/03/09  23:13:03  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:15  marty]
#
# Revision 1.1.10.1  1995/12/08  15:25:49  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:54:06  root]
# 
# Revision 1.1.8.2  1994/08/16  08:16:03  marrek
# 	Fix for delta update (ot 11617).
# 	[1994/08/12  11:59:53  marrek]
# 
# Revision 1.1.8.1  1994/02/22  18:05:20  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:49:26  marrek]
# 
# Revision 1.1.6.2  1993/08/11  14:22:37  marrek
# 	July 1993 code drop.
# 	[1993/08/02  13:11:11  marrek]
# 
# Revision 1.1.4.3  1992/12/31  18:39:21  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:12:02  bbelch]
# 
# Revision 1.1.4.2  1992/10/05  16:44:00  marrek
# 	Insert #!/bin/sh .
# 	[1992/10/05  16:41:15  marrek]
# 
# Revision 1.1.2.2  1992/06/01  20:11:58  melman
# 	New GDS merged drop
# 	[1992/05/28  21:04:45  melman]
# 
# $EndLog$
#
# $RCSfile: gdshdlupd.sh,v $ $Revision: 1.1.10.2 $ $Date: 1996/03/09 23:13:03 $
#
#***************************************************************************
#
#        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991
#                             ALL RIGHTS RESERVED
#
#***************************************************************************

# shell script used to set the environment and to activate the
# 'delta upate'-daemon.
# input parameter:
#		$1 = directory identifier
#               $2 = <dce_local>; (path name of installation)
#
. $2/var/adm/directory/gds/.profile
DIRID=$1
eval SVC_UPDAT_INFO=\"$SVC_UPDAT_ROUTING\"
$2/bin/gdsdaemon -d$1 -m2

# if you want to start the daemon with tracing switched on use this line:
#
# $2/bin/gdsdaemon $SVC_UPDAT_INFO -d$1 -m2
