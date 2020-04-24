#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: key_setup.sh,v $
# Revision 1.1.2.2  1996/03/11  02:45:20  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:07:54  marty]
#
# Revision 1.1.2.1  1995/12/11  22:01:00  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/jrr_1.2_mothra/1  1995/12/06  15:57 UTC  bell_d
# 	key_setup.sh creates bankd account for dcerpbnk_auth test
# 	[1995/12/11  20:59:54  root]
# 
# Revision 1.1.2.1  1994/09/28  21:47:59  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:15:39  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:44:55  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:33:13  devsrc]
# 
# Revision 1.1.2.1  1994/06/03  14:05:34  ganni
# 	create the principal needed for the tests through scripts
# 	[1994/06/03  14:03:20  ganni]
# 
# $EndLog$
#
#

echo "ktdel -p bankd -v 1" > /tmp/bnk_k.script
echo "del -p bankd" >> /tmp/bnk_k.script
echo "do p" >> /tmp/bnk_k.script
echo "del bankd" >> /tmp/bnk_k.script
echo "add bankd" >> /tmp/bnk_k.script
echo "do a" >> /tmp/bnk_k.script
echo "add bankd -pw bankd -mp $1 -g none -o none" >> /tmp/bnk_k.script
echo "ktadd -p bankd -pw bankd" >> /tmp/bnk_k.script
echo "quit" >> /tmp/bnk_k.script
/opt/dcelocal/bin/rgy_edit -update </tmp/bnk_k.script >/tmp/bnk_k.out

