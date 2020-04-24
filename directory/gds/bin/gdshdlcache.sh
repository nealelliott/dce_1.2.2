#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: gdshdlcache.sh,v $
# Revision 1.1.10.2  1996/03/09  23:13:02  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:14  marty]
#
# Revision 1.1.10.1  1995/12/08  15:25:46  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:54:05  root]
# 
# Revision 1.1.8.1  1994/02/22  18:05:19  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:49:19  marrek]
# 
# Revision 1.1.6.2  1993/08/11  14:21:42  marrek
# 	July 1993 code drop.
# 	[1993/08/02  13:11:04  marrek]
# 
# Revision 1.1.4.3  1992/12/31  18:39:19  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:11:58  bbelch]
# 
# Revision 1.1.4.2  1992/10/05  16:43:53  marrek
# 	Insert #!/bin/sh .
# 	[1992/10/05  16:41:07  marrek]
# 
# Revision 1.1.2.2  1992/06/01  20:11:52  melman
# 	New GDS merged drop
# 	[1992/05/28  21:04:41  melman]
# 
# $EndLog$
#
# $RCSfile: gdshdlcache.sh,v $ $Revision: 1.1.10.2 $ $Date: 1996/03/09 23:13:02 $
#
#***************************************************************************
#
#        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991
#                             ALL RIGHTS RESERVED
#
#***************************************************************************

# shell script used to set the environment and to activate the
# 'cache upate'-program.
# input parameter:
#		$1 = directory identifier
#               $2 = <dce_local>; (path name of installation)
#
. $2/var/adm/directory/gds/.profile
$2/bin/gdscacheupd $1
