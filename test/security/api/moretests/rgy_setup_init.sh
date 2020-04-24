#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: rgy_setup_init.sh,v $
# Revision 1.1.6.2  1996/03/11  02:35:34  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:03  marty]
#
# Revision 1.1.6.1  1995/12/11  21:49:59  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:54  root]
# 
# Revision 1.1.4.2  1992/12/31  17:32:46  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:58:02  htf]
# 
# Revision 1.1.2.2  1992/05/19  20:30:33  cuti
# 	 Created tfor adding a signal account.
# 	[1992/05/19  20:28:53  cuti]
# 
# $EndLog$

#set -x




echo "do p" >> /tmp/rgy_edit.script
echo "add $1 -f \"$2\"" >> /tmp/rgy_edit.script
echo "do acc" >>/tmp/rgy_edit.script
echo "add $1 -g none -o none -mp -dce- -pw $3 " >>/tmp/rgy_edit.script

EXIT=$?

exit $EXIT
