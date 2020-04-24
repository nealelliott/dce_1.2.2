#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: makesyms.sh,v $
# Revision 1.1.4.2  1996/03/11  13:25:35  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:13:44  marty]
#
# Revision 1.1.4.1  1995/12/08  17:52:55  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:27:25  root]
# 
# Revision 1.1.2.2  1992/12/30  17:17:06  zeliff
# 	Embedding copyright notice
# 	[1992/12/30  17:15:14  zeliff]
# 
# Revision 1.1  1992/01/19  15:31:34  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#  
#  
#  OSF DCE Release 1.0 
#

dump -g $1 | sed -n -e 's/^[   ]*[0-9][0-9]*[   ]*\([^  .][^    ]*\)$/\1/p' 
