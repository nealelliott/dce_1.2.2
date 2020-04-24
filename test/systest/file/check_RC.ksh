#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: check_RC.ksh,v $
# Revision 1.1.6.2  1996/03/11  02:43:41  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:07:14  marty]
#
# Revision 1.1.6.1  1995/12/11  21:59:19  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:58:38  root]
# 
# Revision 1.1.4.1  1994/06/10  20:49:07  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:19:03  devsrc]
# 
# Revision 1.1.2.2  1993/07/30  15:43:25  gmd
# 	Initial version - used by maxfile, maxdir
# 	and block_frag scripts.
# 	[1993/07/30  14:44:01  gmd]
# 
# $EndLog$
#################################################################
#
# Check Return Code and exit if failure
#
#################################################################
check_RC()
{
        RC="$1"
        COMMAND="$2"
        if [ "$RC" != "0" ]; then
                echo "$COMMAND FAILED"
                exit 1
        fi
}
