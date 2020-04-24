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
# $Log: sec_acl_setup.sh,v $
# Revision 1.1.4.2  1996/03/11  02:35:42  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:08  marty]
#
# Revision 1.1.4.1  1995/12/11  21:50:10  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:53:00  root]
# 
# Revision 1.1.2.3  1993/02/25  22:08:32  cuti
# 	Add "site -u" to rgy_edit commands to make sure working on master site.
# 	[1993/02/25  16:33:53  cuti]
# 
# Revision 1.1.2.2  1992/12/31  17:33:04  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:58:15  htf]
# 
# Revision 1.1  1992/01/19  04:08:04  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# 
echo "site -u" > /tmp/rgy_edit.script
echo "do p" >>/tmp/rgy_edit.script
echo "add flintstone -f \"Fred Flintstone\"" >>/tmp/rgy_edit.script
echo "do acc" >>/tmp/rgy_edit.script
echo "add flintstone -g none -o none -mp -dce- -pw yabadabado -m \"This space for rent\"" >>/tmp/rgy_edit.script
echo "q" >>/tmp/rgy_edit.script
/opt/dcelocal/bin/rgy_edit </tmp/rgy_edit.script

