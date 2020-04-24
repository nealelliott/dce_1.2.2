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
# $Log: key_mgmt_setup.sh,v $
# Revision 1.1.7.2  1996/03/11  02:34:30  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:27  marty]
#
# Revision 1.1.7.1  1995/12/11  21:48:34  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:20  root]
# 
# Revision 1.1.5.1  1993/10/27  22:34:48  bmw
# 	CR 9274: delete /tmp/key_mgmt_test file for key_mgmt_set_key.tsh
# 	[1993/10/27  22:33:59  bmw]
# 
# Revision 1.1.3.4  1993/02/25  22:06:09  cuti
# 	Add "site -u" to rgy_edit commands to make sure working on master site.
# 	[1993/02/25  21:48:10  cuti]
# 
# Revision 1.1.3.3  1993/01/22  21:56:59  sekhar
# 	[OT 4887]  Remove principals and keys if already defined
# 	[1993/01/22  21:49:11  sekhar]
# 
# Revision 1.1.3.2  1992/12/31  17:29:04  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:54:37  htf]
# 
# Revision 1.1  1992/01/19  04:08:12  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# 

# Remove test_key* prinicpals and accounts which may be left
# in the registry from a previous setup or run of API tests.

echo ""
echo ".... Removing from registry (if present) the following principals "
echo ".... and their accounts: test_key1, test_key2, test_key3, test_key4 "
echo ""

cat > /tmp/rgy_edit.script << !
site -u
do p
del test_key1
del test_key2
del test_key3
del test_key4
quit
!
/opt/dcelocal/bin/rgy_edit </tmp/rgy_edit.script > /tmp/rgy_edit.out

echo ".... Adding principals and accounts : test_key1, test_key2, "
echo ".... test_key3, test_key4 "
echo ""

echo "site -u" > /tmp/rgy_edit.script
echo "do p" >>/tmp/rgy_edit.script
echo "add test_key1 -f \"test_key1\"" >>/tmp/rgy_edit.script
echo "do acc" >>/tmp/rgy_edit.script
echo "add test_key1 -g none -o none -mp -dce- -pw passwd1v1 -m \"This space for rent\"" >>/tmp/rgy_edit.script

echo "do p" >>/tmp/rgy_edit.script
echo "add test_key2 -f \"test_key2\"" >>/tmp/rgy_edit.script
echo "do acc" >>/tmp/rgy_edit.script
echo "add test_key2 -g none -o none -mp -dce- -pw passwd2v1 -m \"This space for rent\"" >>/tmp/rgy_edit.script

echo "do p" >>/tmp/rgy_edit.script
echo "add test_key3 -f \"test_key3\"" >>/tmp/rgy_edit.script
echo "do acc" >>/tmp/rgy_edit.script
echo "add test_key3 -g none -o none -mp -dce- -pw passwd3v1 -m \"This space for rent\"" >>/tmp/rgy_edit.script

echo "do p" >>/tmp/rgy_edit.script
echo "add test_key4 -f \"test_key4\"" >>/tmp/rgy_edit.script
echo "do acc" >>/tmp/rgy_edit.script
echo "add test_key4 -g none -o none -mp -dce- -pw passwd1v4 -m \"This space for rent\"" >>/tmp/rgy_edit.script

echo "q" >>/tmp/rgy_edit.script  


/opt/dcelocal/bin/rgy_edit </tmp/rgy_edit.script

rm -f /tmp/key_mgmt_test	# remove file for key_mgmt_set_key.tsh
