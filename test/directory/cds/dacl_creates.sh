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
# $Log: dacl_creates.sh,v $
# Revision 1.1.13.2  1996/03/11  01:44:40  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:15:34  marty]
#
# Revision 1.1.13.1  1995/12/08  22:09:24  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:44:03  root]
# 
# Revision 1.1.11.1  1993/10/13  21:23:27  pwang
# 	Replaced linktarget with CDS_LinkTarget [OT#9107]
# 	[1993/10/13  19:33:33  pwang]
# 
# Revision 1.1.9.2  1993/06/17  21:01:47  sommerfeld
# 	replaced hardcoded hostname with automatic lookup
# 	[1993/05/11  20:45:15  smythe]
# 
# Revision 1.1.4.4  1993/02/24  13:21:06  jd
# 	More of the same.
# 	[1993/02/24  00:20:22  jd]
# 
# 	Various script fix ups...
# 	[1993/02/23  23:03:15  jd]
# 
# Revision 1.1.4.3  1993/02/11  19:54:39  jd
# 	Incorporate new dec code.
# 	[1993/02/10  14:14:34  jd]
# 
# Revision 1.1.4.2  1993/01/12  18:28:27  bbelch
# 	Embedding copyright notice
# 	[1993/01/07  01:12:36  bbelch]
# 
# 	Modified the test so it would run.
# 	[1992/07/02  22:02:15  grober_j]
# 
# 	Modified the test so it would run.
# 	[1992/07/02  22:02:15  grober_j]
# 
# 	Modified the test so it would run.
# 	[1992/07/02  22:02:15  grober_j]
# 
# Revision 1.1.2.4  1992/07/29  19:41:08  m_sawyer
# 	Updated dacl tests.
# 	[1992/07/28  20:32:58  m_sawyer]
# 
# 	    Loading drop Dce1_0_1b19
# 	[1992/06/16  05:12:45  root]
# 
# Revision 1.1.2.3  1992/07/29  14:53:47  m_sawyer
# 	Updated dacl tests.
# 	[1992/07/28  20:32:58  m_sawyer]
# 
# 	    Loading drop Dce1_0_1b19
# 	[1992/06/16  05:12:45  root]
# 
# Revision 1.1.2.2  1992/07/28  21:01:15  m_sawyer
# 	Updated dacl tests.
# 	[1992/07/28  20:32:58  m_sawyer]
# 
# 	    Loading drop Dce1_0_1b19
# 	[1992/06/16  05:12:45  root]
# 
# Revision 1.1  1992/01/19  14:32:12  devrcs
# 	Initial revision
# 
# $EndLog$
#
# This script tests that create operations require only insert access
# to the directory and for directory create
#
# Usage:
#   dacl_creates <clearinghouse1> 
#
# Make sure no default container or inital object acls exist on parent
# Make sure notroot has at least control to clearinghouse before running
# this script
# 
#



echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp create dir /.:/dacl_creates"
cdscp create dir /.:/dacl_creates
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_creates to skulk"
cdscp set dir /.:/dacl_creates to skulk

echo "#"
echo "# Set access to everything but insert and expect all to fail"
echo "#"

echo "acl_edit /.:/dacl_creates -m user:notroot:rwdtca"
acl_edit /.:/dacl_creates -m user:notroot:rwdtca
HOSTNAME=`acl_edit /.:/dacl_creates -l | fgrep user:hosts/ | cut -d/ -f2`
echo "acl_edit /.:/dacl_creates -m user:hosts/HOSTNAME/cds-server:rwdc"
acl_edit /.:/dacl_creates -m user:hosts/$HOSTNAME/cds-server:rwdc


echo "#"
echo "# Try creates - should fail"
echo "#"

echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_creates to skulk"
cdscp set dir /.:/dacl_creates to skulk
echo "cdscp create object /.:/dacl_creates/obj1"
cdscp create object /.:/dacl_creates/obj1
echo "cdscp create link /.:/dacl_creates/lnk1"
cdscp create link /.:/dacl_creates/lnk1 CDS_LinkTarget /.:/dacl_creates/obj1
echo "cdscp create dir /.:/dacl_creates/dir1"
cdscp create dir /.:/dacl_creates/dir1

echo "#"
echo "# Add insert to directory - keep read to edit acls for later"
echo "#"

echo "acl_edit /.:/dacl_creates -m user:notroot:ric"
acl_edit /.:/dacl_creates -m user:notroot:ric

echo "#"
echo "# Create of object and link should work."
echo "#"
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_creates to skulk"
cdscp set dir /.:/dacl_creates to skulk
echo "cdscp create object /.:/dacl_creates/obj1"
cdscp create object /.:/dacl_creates/obj1
echo "cdscp create link /.:/dacl_creates/lnk1"
cdscp create link /.:/dacl_creates/lnk1 CDS_LinkTarget /.:/dacl_creates/obj1
echo "cdscp set dir /.:/dacl_creates to skulk"
cdscp set dir /.:/dacl_creates to skulk

echo "#"
echo "# Creation of directory should fail."
echo "#"

echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_creates to skulk"
cdscp set dir /.:/dacl_creates to skulk
echo "cdscp create dir /.:/dacl_creates/dir1"
cdscp create dir /.:/dacl_creates/dir1
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_creates to skulk"
cdscp set dir /.:/dacl_creates to skulk

echo "#"
echo "# Add insert and read for server"
echo "#"

echo "acl_edit /.:/dacl_creates -m user:hosts/HOSTNAME/cds-server:rwdic"
acl_edit /.:/dacl_creates -m user:hosts/$HOSTNAME/cds-server:rwdic

echo "#"
echo "# Try create again - should work"
echo "#"

echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_creates to skulk"
cdscp set dir /.:/dacl_creates to skulk
echo "cdscp create dir /.:/dacl_creates/dir1"
cdscp create dir /.:/dacl_creates/dir1
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_creates to skulk"
cdscp set dir /.:/dacl_creates to skulk
echo "cdscp set dir /.:/dacl_creates/dir1 to skulk"
cdscp set dir /.:/dacl_creates/dir1 to skulk

echo "#"
echo "# Cleanup - acl's"
echo "#"

echo "acl_edit /.:/dacl_creates -m user:notroot:rwdtcia"
acl_edit /.:/dacl_creates -m user:notroot:rwdtcia

echo "#"
echo "# Cleanup - namespace"
echo "#"

echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_creates to skulk"
cdscp set dir /.:/dacl_creates to skulk
echo "cdscp set dir /.:/dacl_creates/dir1 to skulk"
cdscp set dir /.:/dacl_creates/dir1 to skulk
echo "cdscp delete object /.:/dacl_creates/obj1"
cdscp delete object /.:/dacl_creates/obj1
echo "cdscp delete link /.:/dacl_creates/lnk1"
cdscp delete link /.:/dacl_creates/lnk1
echo "cdscp delete directory /.:/dacl_creates/dir1"
cdscp delete directory /.:/dacl_creates/dir1
echo "cdscp set dir /.:/dacl_creates to skulk"
cdscp set dir /.:/dacl_creates to skulk
echo "cdscp delete directory /.:/dacl_creates"
cdscp delete directory /.:/dacl_creates
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk

