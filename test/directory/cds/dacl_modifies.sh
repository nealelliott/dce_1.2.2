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
# $Log: dacl_modifies.sh,v $
# Revision 1.1.13.2  1996/03/11  01:44:45  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:15:38  marty]
#
# Revision 1.1.13.1  1995/12/08  22:09:31  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:44:07  root]
# 
# Revision 1.1.11.1  1993/10/13  21:23:31  pwang
# 	Replaced linktarget with CDS_linkTarget [OT#9107]
# 	[1993/10/13  19:41:06  pwang]
# 
# Revision 1.1.9.2  1993/06/17  21:02:45  sommerfeld
# 	removed hardcoded NLSPATH export
# 	[1993/05/11  21:29:10  smythe]
# 
# Revision 1.1.4.3  1993/02/11  19:54:29  jd
# 	Incorporate new dec code.
# 	[1993/02/10  14:15:23  jd]
# 
# Revision 1.1.4.2  1993/01/12  18:28:39  bbelch
# 	Embedding copyright notice
# 	[1993/01/07  01:12:43  bbelch]
# 
# 	Modified test so it would run.
# 	[1992/07/02  22:04:00  grober_j]
# 
# 	Modified test so it would run.
# 	[1992/07/02  22:04:00  grober_j]
# 
# Revision 1.1.2.3  1992/07/29  19:41:15  m_sawyer
# 	Updated dacl tests.
# 	[1992/07/28  20:33:42  m_sawyer]
# 
# 	    Loading drop Dce1_0_1b19
# 	[1992/06/16  05:12:48  root]
# 
# Revision 1.1.2.2  1992/07/29  14:54:04  m_sawyer
# 	Updated dacl tests.
# 	[1992/07/28  20:33:42  m_sawyer]
# 
# 	    Loading drop Dce1_0_1b19
# 	[1992/06/16  05:12:48  root]
# 
# Revision 1.1  1992/01/19  14:32:15  devrcs
# 	Initial revision
# 
# $EndLog$
#
# This script tests that update operations require only write access. 
#
# Make sure no default container or inital object acls exist on parent
# 
#
# This script tests that update operations require only write access.
#
# Make sure no default container or inital object acls exist on parent
# 

myname=`basename $0`

echo ""
echo "Create directory tree first "
echo ""

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp create dir /.:/dacl_modifies"
cdscp create dir /.:/dacl_modifies
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_modifies to skulk"
cdscp set dir /.:/dacl_modifies to skulk
echo "cdscp create obj /.:/dacl_modifies/obj1"
cdscp create obj /.:/dacl_modifies/obj1
echo "cdscp create link /.:/dacl_modifies/lnk1"
cdscp create link /.:/dacl_modifies/lnk1 CDS_LinkTarget /.:/dacl_modifies/obj1
echo "cdscp create dir /.:/dacl_modifies/dir1"
cdscp create dir /.:/dacl_modifies/dir1
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_modifies to skulk"
cdscp set dir /.:/dacl_modifies to skulk
echo "cdscp set dir /.:/dacl_modifies/dir1 to skulk"
cdscp set dir /.:/dacl_modifies/dir1 to skulk

echo ""
echo "Set access to everything but write and expect all to fail"
echo ""

echo "#"
echo "# acl_edit"
echo "#"

echo "acl_edit -e /.:/dacl_modifies/obj1 -m user:notroot:rdtc"
acl_edit -e /.:/dacl_modifies/obj1 -m user:notroot:rdtc
echo "acl_edit -e /.:/dacl_modifies/lnk1 -m user:notroot:rdtc"
acl_edit -e /.:/dacl_modifies/lnk1 -m user:notroot:rdtc
echo "acl_edit -e /.:/dacl_modifies/dir1 -m user:notroot:rdtc"
acl_edit -e /.:/dacl_modifies/dir1 -m user:notroot:rdtc
echo "acl_edit  /.:/dacl_modifies/dir1 -m user:notroot:rdtcia"
acl_edit  /.:/dacl_modifies/dir1 -m user:notroot:rdtcia
echo "acl_edit /.:/dacl_modifies -m user:notroot:rdtcia"
acl_edit /.:/dacl_modifies -m user:notroot:rdtcia

echo ""
echo "Try modifies - should fail"
echo ""

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_modifies to skulk"
cdscp set dir /.:/dacl_modifies to skulk
echo "cdscp set dir /.:/dacl_modifies/dir1 to skulk"
cdscp set dir /.:/dacl_modifies/dir1 to skulk
echo "cdscp set dir /.:/dacl_modifies/dir1 1.11.12.13.14.15 = 1011"
cdscp set dir /.:/dacl_modifies/dir1 1.11.12.13.14.15 = 1011
echo "cdscp set obj /.:/dacl_modifies/obj1 1.11.12.13.14.15 = 1011"
cdscp set obj /.:/dacl_modifies/obj1 1.11.12.13.14.15 = 1011
echo "cdscp set link /.:/dacl_modifies/lnk1 CDS_LinkTimeout \( 1999-12-31-23:59:59 00:00:00 \)"
cdscp set link /.:/dacl_modifies/lnk1 CDS_LinkTimeout \( 1999-12-31-23:59:59 00:00:00 \)
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_modifies to skulk"
cdscp set dir /.:/dacl_modifies to skulk
echo "cdscp set dir /.:/dacl_modifies/dir1 to skulk"
cdscp set dir /.:/dacl_modifies/dir1 to skulk
echo "cdscp remove dir /.:/dacl_modifies/dir1 1.11.12.13.14.15"
cdscp remove dir /.:/dacl_modifies/dir1 1.11.12.13.14.15
echo "cdscp remove obj /.:/dacl_modifies/obj1 1.11.12.13.14.15"
cdscp remove obj /.:/dacl_modifies/obj1 1.11.12.13.14.15 
echo "cdscp remove link /.:/dacl_modifies/lnk1 CDS_LinkTimeout"
cdscp remove link /.:/dacl_modifies/lnk1 CDS_LinkTimeout
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_modifies to skulk"
cdscp set dir /.:/dacl_modifies to skulk
echo "cdscp set dir /.:/dacl_modifies/dir1 to skulk"
cdscp set dir /.:/dacl_modifies/dir1 to skulk
echo "cdscp add dir /.:/dacl_modifies/dir1 1.11.12.13.14.15 = 1011"
cdscp add dir /.:/dacl_modifies/dir1 1.11.12.13.14.15 = 1011 
echo "cdscp add obj /.:/dacl_modifies/obj1 1.11.12.13.14.15 = 1011"
cdscp add obj /.:/dacl_modifies/obj1 1.11.12.13.14.15 = 1011
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_modifies to skulk"
cdscp set dir /.:/dacl_modifies to skulk
echo "cdscp set dir /.:/dacl_modifies/dir1 to skulk"
cdscp set dir /.:/dacl_modifies/dir1 to skulk
echo "cdscp remove dir /.:/dacl_modifies/dir1 1.11.12.13.14.15 = 1011"
cdscp remove dir /.:/dacl_modifies/dir1 1.11.12.13.14.15 = 1011
echo "cdscp remove obj /.:/dacl_modifies/obj1 1.11.12.13.14.15 = 1011"
cdscp remove obj /.:/dacl_modifies/obj1 1.11.12.13.14.15 = 1011
echo "cdscp remove dir /.:/dacl_modifies/dir1 1.11.12.13.14.15"
cdscp remove dir /.:/dacl_modifies/dir1 1.11.12.13.14.15 
echo "cdscp remove obj /.:/dacl_modifies/obj1 1.11.12.13.14.15"
cdscp remove obj /.:/dacl_modifies/obj1 1.11.12.13.14.15
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_modifies to skulk"
cdscp set dir /.:/dacl_modifies to skulk
echo "cdscp set dir /.:/dacl_modifies/dir1 to skulk"
cdscp set dir /.:/dacl_modifies/dir1 to skulk

#
# Set child is supposed to be an unsupported command.  However,
# as of b21 you could still modify existing attributes in
# child entries.  If you modify the CDS_ObjectUUID attribute
# though, you will be unable to delete the directory.
#

#echo "set child /.:/dacl_modifies/dir1 CDS_ObjectUUID 3334e634-8572-11ca-bec9-0 8002b1c8f1f"
#cdscp set child /.:/dacl_modifies/dir1 CDS_ObjectUUID 3334e634-8572-11ca-bec9-0 8002b1c8f1f

#
# Unsupported command
#

#echo "remove child /.:/dacl_modifies/dir1 CDS_ObjectUUID"
#cdscp remove child /.:/dacl_modifies/dir1 CDS_ObjectUUID

echo ""
echo "Set acls for write and control only.  Must retain read for cleanup"
echo ""

echo "#"
echo "# acl_edit"
echo "#"

echo "acl_edit -e /.:/dacl_modifies/obj1 -m user:notroot:crw"
acl_edit -e /.:/dacl_modifies/obj1 -m user:notroot:crw
echo "acl_edit -e /.:/dacl_modifies/lnk1 -m user:notroot:crw"
acl_edit -e /.:/dacl_modifies/lnk1 -m user:notroot:crw
echo "acl_edit -e /.:/dacl_modifies/dir1 -m user:notroot:crw"
acl_edit -e /.:/dacl_modifies/dir1 -m user:notroot:crw
echo "acl_edit  /.:/dacl_modifies/dir1 -m user:notroot:crw"
acl_edit  /.:/dacl_modifies/dir1 -m user:notroot:crw
echo "acl_edit /.:/dacl_modifies -m user:notroot:crw"
acl_edit /.:/dacl_modifies -m user:notroot:crw

echo ""
echo "Try modifies - should work"
echo ""

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_modifies to skulk"
cdscp set dir /.:/dacl_modifies to skulk
echo "cdscp set dir /.:/dacl_modifies/dir1 to skulk"
cdscp set dir /.:/dacl_modifies/dir1 to skulk
echo "cdscp set dir /.:/dacl_modifies/dir1 1.11.12.13.14.15 = 1011"
cdscp set dir /.:/dacl_modifies/dir1 1.11.12.13.14.15 = 1011 
echo "cdscp set obj /.:/dacl_modifies/obj1 1.11.12.13.14.15 = 1011"
cdscp set obj /.:/dacl_modifies/obj1 1.11.12.13.14.15 = 1011
echo "cdscp set link /.:/dacl_modifies/lnk1 CDS_LinkTimeout \( 1999-12-31-23:59:59 00:00:00 \)"
cdscp set link /.:/dacl_modifies/lnk1 CDS_LinkTimeout \( 1999-12-31-23:59:59 00:00:00 \)
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_modifies to skulk"
cdscp set dir /.:/dacl_modifies to skulk
echo "cdscp set dir /.:/dacl_modifies/dir1 to skulk"
cdscp set dir /.:/dacl_modifies/dir1 to skulk
echo "cdscp remove dir /.:/dacl_modifies/dir1 1.11.12.13.14.15"
cdscp remove dir /.:/dacl_modifies/dir1 1.11.12.13.14.15
echo "cdscp remove obj /.:/dacl_modifies/obj1 1.11.12.13.14.15"
cdscp remove obj /.:/dacl_modifies/obj1 1.11.12.13.14.15
echo "cdscp remove link /.:/dacl_modifies/lnk1 CDS_LinkTimeout"
cdscp remove link /.:/dacl_modifies/lnk1 CDS_LinkTimeout
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_modifies to skulk"
cdscp set dir /.:/dacl_modifies to skulk
echo "cdscp set dir /.:/dacl_modifies/dir1 to skulk"
cdscp set dir /.:/dacl_modifies/dir1 to skulk
echo "cdscp add dir /.:/dacl_modifies/dir1 1.11.12.13.14.15 = 1011"
cdscp add dir /.:/dacl_modifies/dir1 1.11.12.13.14.15 = 1011
echo "cdscp add obj /.:/dacl_modifies/obj1 1.11.12.13.14.15 = 1011"
cdscp add obj /.:/dacl_modifies/obj1 1.11.12.13.14.15 = 1011
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_modifies to skulk"
cdscp set dir /.:/dacl_modifies to skulk
echo "cdscp set dir /.:/dacl_modifies/dir1 to skulk"
cdscp set dir /.:/dacl_modifies/dir1 to skulk
echo "cdscp remove dir /.:/dacl_modifies/dir1 1.11.12.13.14.15 = 1011"
cdscp remove dir /.:/dacl_modifies/dir1 1.11.12.13.14.15 = 1011
echo "cdscp remove obj /.:/dacl_modifies/obj1 1.11.12.13.14.15 = 1011"
cdscp remove obj /.:/dacl_modifies/obj1 1.11.12.13.14.15 = 1011
echo "cdscp remove dir /.:/dacl_modifies/dir1 1.11.12.13.14.15"
cdscp remove dir /.:/dacl_modifies/dir1 1.11.12.13.14.15
echo "cdscp remove obj /.:/dacl_modifies/obj1 1.11.12.13.14.15"
cdscp remove obj /.:/dacl_modifies/obj1 1.11.12.13.14.15
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_modifies to skulk"
cdscp set dir /.:/dacl_modifies to skulk
echo "cdscp set dir /.:/dacl_modifies/dir1 to skulk"
cdscp set dir /.:/dacl_modifies/dir1 to skulk

#echo "set child /.:/dacl_modifies/dir1 CDS_ObjectUUID 3334e634-8572-11ca-bec9-0 8002b1c8f1f"
#cdscp set child /.:/dacl_modifies/dir1 CDS_ObjectUUID 3334e634-8572-11ca-bec9-0 8002b1c8f1f

#
# Remove child not currently supported.
#

#echo "remove child /.:/dacl_modifies/dir1 CDS_ObjectUUID"
#cdscp remove child /.:/dacl_modifies/dir1 CDS_ObjectUUID

echo ""
echo "Cleanup - acl's"
echo ""

echo "#"
echo "# acl_edit"
echo "#"

echo "acl_edit -e /.:/dacl_modifies/obj1 -m user:notroot:cwrd"
acl_edit -e /.:/dacl_modifies/obj1 -m user:notroot:cwrd
echo "acl_edit -e /.:/dacl_modifies/lnk1 -m user:notroot:cwrd"
acl_edit -e /.:/dacl_modifies/lnk1 -m user:notroot:cwrd
echo "acl_edit -e /.:/dacl_modifies/dir1 -m user:notroot:cwrd"
acl_edit -e /.:/dacl_modifies/dir1 -m user:notroot:cwrd
echo "acl_edit  /.:/dacl_modifies/dir1 -m user:notroot:cwrd"
acl_edit  /.:/dacl_modifies/dir1 -m user:notroot:cwrd
echo "acl_edit /.:/dacl_modifies -m user:notroot:cwrd"
acl_edit /.:/dacl_modifies -m user:notroot:cwrd

echo ""
echo "Cleanup - namespace"
echo ""

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_modifies to skulk"
cdscp set dir /.:/dacl_modifies to skulk
echo "cdscp set dir /.:/dacl_modifies/dir1 to skulk"
cdscp set dir /.:/dacl_modifies/dir1 to skulk
echo "cdscp delete object /.:/dacl_modifies/obj1"
cdscp delete object /.:/dacl_modifies/obj1
echo "cdscp delete link /.:/dacl_modifies/lnk1"
cdscp delete link /.:/dacl_modifies/lnk1
echo "cdscp delete directory /.:/dacl_modifies/dir1"
cdscp delete directory /.:/dacl_modifies/dir1
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_modifies to skulk"
cdscp set dir /.:/dacl_modifies to skulk
echo "cdscp delete directory /.:/dacl_modifies"
cdscp delete directory /.:/dacl_modifies
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk

