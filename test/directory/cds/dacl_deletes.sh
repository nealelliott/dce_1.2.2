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
# $Log: dacl_deletes.sh,v $
# Revision 1.1.94.2  1996/03/11  01:44:42  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:15:36  marty]
#
# Revision 1.1.94.1  1995/12/08  22:09:28  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:44:05  root]
# 
# Revision 1.1.92.1  1993/10/13  21:23:29  pwang
# 	Replaced linktarget with CDS_LinkTarget [OT#9107]
# 	[1993/10/13  19:37:30  pwang]
# 
# Revision 1.1.8.3  1993/08/26  21:54:05  pwang
# 	Fixed wrong echoed message [OT#8083]
# 	[1993/08/26  21:42:01  pwang]
# 
# Revision 1.1.8.2  1993/06/17  21:02:10  sommerfeld
# 	replaced unused and incorrect references to dir1
# 	[1993/05/11  21:09:18  smythe]
# 
# Revision 1.1.4.4  1993/02/24  13:21:07  jd
# 	Script cleanups.
# 	[1993/02/24  13:13:32  jd]
# 
# Revision 1.1.4.3  1993/02/11  19:54:23  jd
# 	Incorporate new dec code.
# 	[1993/02/10  14:15:07  jd]
# 
# Revision 1.1.4.2  1993/01/12  18:28:32  bbelch
# 	Embedding copyright notice
# 	[1993/01/07  01:12:39  bbelch]
# 
# 	Modified test so it would run.
# 	[1992/07/02  22:03:16  grober_j]
# 
# 	Modified test so it would run.
# 	[1992/07/02  22:03:16  grober_j]
# 
# Revision 1.1.2.3  1992/07/29  19:41:11  m_sawyer
# 	Updated dacl tests.
# 	[1992/07/28  20:33:21  m_sawyer]
# 
# 	    Loading drop Dce1_0_1b19
# 	[1992/06/16  05:12:46  root]
# 
# Revision 1.1.2.2  1992/07/29  14:53:54  m_sawyer
# 	Updated dacl tests.
# 	[1992/07/28  20:33:21  m_sawyer]
# 
# 	    Loading drop Dce1_0_1b19
# 	[1992/06/16  05:12:46  root]
# 
# Revision 1.1  1992/01/19  14:32:13  devrcs
# 	Initial revision
# 
# $EndLog$
#
# This script tests access for delete operations.
#
# Delete to the name or for objects, links and childptrs delete
# to the parent directory is all that is needed.
#
# Childptrs are deleted by the server as part of deleting a directory
# so it is the server that need access.
#
# Make sure the root directory has no initial container or inital
# object acls for us to inherit.
# 


echo ""
echo "Set up directory tree"
echo ""

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp create dir /.:/dacl_deletes"
cdscp create dir /.:/dacl_deletes
echo "cdscp set dir /.:/dacl_deletes to skulk"
cdscp set dir /.:/dacl_deletes to skulk
echo "cdscp create object /.:/dacl_deletes/obj1"
cdscp create object /.:/dacl_deletes/obj1
echo "cdscp create link /.:/dacl_deletes/lnk1"
cdscp create link /.:/dacl_deletes/lnk1 CDS_LinkTarget /.:/dacl_deletes/obj1
echo "cdscp set dir /.:/dacl_deletes to skulk"
cdscp set dir /.:/dacl_deletes to skulk
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk


echo ""
echo "Set directory access to admin, object and link contained therein to"
echo "everything but delete. Delete the object and link. Should work."
echo ""

echo "#"
echo "# acl_edit"
echo "#"

echo "acl_edit /.:/dacl_deletes -m user:notroot:rwtica"
acl_edit /.:/dacl_deletes -m user:notroot:rwtica
echo "acl_edit -e /.:/dacl_deletes/obj1 -m user:notroot:rwtc"
acl_edit -e /.:/dacl_deletes/obj1 -m user:notroot:rwtc
echo "acl_edit -e /.:/dacl_deletes/lnk1 -m user:notroot:rwtc"
acl_edit -e /.:/dacl_deletes/lnk1 -m user:notroot:rwtc

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp set dir /.:/dacl_deletes to skulk"
cdscp set dir /.:/dacl_deletes to skulk
echo "cdscp delete object /.:/dacl_deletes/obj1"
cdscp delete object /.:/dacl_deletes/obj1
echo "cdscp delete link /.:/dacl_deletes/lnk1"
cdscp delete link /.:/dacl_deletes/lnk1
echo "cdscp set dir /.:/dacl_deletes to skulk"
cdscp set dir /.:/dacl_deletes to skulk

echo ""
echo "Set directory access to everything but admin, object and link contained"
echo "therein to delete. Delete object and link. Should work."
echo ""

echo " Recreate the object and link"
echo ""

echo "cdscp create object /.:/dacl_deletes/obj1"
cdscp create object /.:/dacl_deletes/obj1
echo "cdscp create link /.:/dacl_deletes/lnk1"
cdscp create link /.:/dacl_deletes/lnk1 CDS_LinkTarget /.:/dacl_deletes/obj1
echo "cdscp set dir /.:/dacl_deletes to skulk"
cdscp set dir /.:/dacl_deletes to skulk

echo ""
echo " acl_edit the directory object and link"
echo ""

echo "acl_edit /.:/dacl_deletes -m user:notroot:rwdtic"
acl_edit /.:/dacl_deletes -m user:notroot:rwdtic
echo "acl_edit -e /.:/dacl_deletes/obj1 -m user:notroot:dc"
acl_edit -e /.:/dacl_deletes/obj1 -m user:notroot:dc
echo "acl_edit -e /.:/dacl_deletes/lnk1 -m user:notroot:dc"
acl_edit -e /.:/dacl_deletes/lnk1 -m user:notroot:dc

echo ""
echo "Delete the object and link."
echo ""

echo "cdscp set dir /.:/dacl_deletes to skulk"
cdscp set dir /.:/dacl_deletes to skulk
echo "cdscp delete object /.:/dacl_deletes/obj1"
cdscp delete object /.:/dacl_deletes/obj1
echo "cdscp delete link /.:/dacl_deletes/lnk1"
cdscp delete link /.:/dacl_deletes/lnk1
echo "cdscp set dir /.:/dacl_deletes to skulk"
cdscp set dir /.:/dacl_deletes to skulk

echo ""
echo "Set directory access to everything but admin, object and link contained"
echo "therein to everything but delete. Delete object and link. Should fail."
echo ""

echo "Recreate the object and link"
echo ""

echo "cdscp create object /.:/dacl_deletes/obj1"
cdscp create object /.:/dacl_deletes/obj1
echo "cdscp create link /.:/dacl_deletes/lnk1"
cdscp create link /.:/dacl_deletes/lnk1 CDS_LinkTarget /.:/dacl_deletes/obj1
echo "cdscp set dir /.:/dacl_deletes to skulk"
cdscp set dir /.:/dacl_deletes to skulk

echo ""
echo "acl_edit the directory object and link"
echo ""

echo "acl_edit /.:/dacl_deletes -m user:notroot:rwdtic"
acl_edit /.:/dacl_deletes -m user:notroot:rwdtic
echo "acl_edit -e /.:/dacl_deletes/obj1 -m user:notroot:rwtc"
acl_edit -e /.:/dacl_deletes/obj1 -m user:notroot:wrtc
echo "acl_edit -e /.:/dacl_deletes/lnk1 -m user:notroot:rwtc"
acl_edit -e /.:/dacl_deletes/lnk1 -m user:notroot:wrtc

echo ""
echo "Delete the object and link."
echo ""

echo "cdscp set dir /.:/dacl_deletes to skulk"
cdscp set dir /.:/dacl_deletes to skulk
echo "cdscp delete object /.:/dacl_deletes/obj1"
cdscp delete object /.:/dacl_deletes/obj1
echo "cdscp delete link /.:/dacl_deletes/lnk1"
cdscp delete link /.:/dacl_deletes/lnk1
echo "cdscp set dir /.:/dacl_deletes to skulk"
cdscp set dir /.:/dacl_deletes to skulk

echo ""
echo "Reset the object and link to delete and clean them out."
echo ""

echo "acl_edit -e /.:/dacl_deletes/obj1 -m user:notroot:rwdtc"
acl_edit -e /.:/dacl_deletes/obj1 -m user:notroot:rwdtc
echo "acl_edit -e /.:/dacl_deletes/lnk1 -m user:notroot:rwdtc"
acl_edit -e /.:/dacl_deletes/lnk1 -m user:notroot:rwdtc
echo "cdscp set dir /.:/dacl_deletes to skulk"
cdscp set dir /.:/dacl_deletes to skulk
echo "cdscp delete object /.:/dacl_deletes/obj1"
cdscp delete object /.:/dacl_deletes/obj1
echo "cdscp delete link /.:/dacl_deletes/lnk1"
cdscp delete link /.:/dacl_deletes/lnk1
echo "cdscp set dir /.:/dacl_deletes to skulk"
cdscp set dir /.:/dacl_deletes to skulk


echo ""
echo "Set the directory to everything but delete. Delete the directory."
echo "Should fail."
echo ""

echo "Acl_edit the directory."
echo ""

echo "acl_edit /.:/dacl_deletes -m user:notroot:rwtica"
acl_edit /.:/dacl_deletes -m user:notroot:rwtica

echo ""
echo "Delete the directory"
echo ""

echo "cdscp set dir /.:/dacl_deletes to skulk"
cdscp set dir /.:/dacl_deletes to skulk
echo "cdscp delete dir /.:/dacl_deletes"
cdscp delete dir /.:/dacl_deletes
echo "cdscp set dir /.:/dacl_deletes to skulk"
cdscp set dir /.:/dacl_deletes to skulk

echo ""
echo "Set the directory to delete. Delete the directory. Should work"
echo ""

echo "Acl_edit the directory."
echo ""

echo "acl_edit /.:/dacl_deletes -m user:notroot:rwdtcia"
acl_edit /.:/dacl_deletes -m user:notroot:rwdtcia

echo ""
echo "Delete the directory"
echo ""

echo "cdscp set dir /.:/dacl_deletes to skulk"
cdscp set dir /.:/dacl_deletes to skulk
echo "cdscp delete dir /.:/dacl_deletes"
cdscp delete dir /.:/dacl_deletes
