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
# $Log: dacl_reads.sh,v $
# Revision 1.1.13.2  1996/03/11  01:44:48  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:15:40  marty]
#
# Revision 1.1.13.1  1995/12/08  22:09:33  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:44:09  root]
# 
# Revision 1.1.11.1  1993/10/13  21:23:33  pwang
# 	Replaced linktarget with CDS_LinkTarget [OT#9107]
# 	[1993/10/13  20:41:35  pwang]
# 
# Revision 1.1.9.3  1993/08/26  21:54:01  pwang
# 	Fixed the truncated line [OT#8083]
# 	[1993/08/26  21:40:39  pwang]
# 
# Revision 1.1.9.2  1993/06/17  21:03:31  sommerfeld
# 	removed hardcoded NLSPATH export
# 	[1993/05/11  21:32:33  smythe]
# 
# Revision 1.1.4.3  1993/02/11  19:54:42  jd
# 	Incorporate new dec code.
# 	[1993/02/10  14:15:40  jd]
# 
# Revision 1.1.4.2  1993/01/12  18:28:44  bbelch
# 	Embedding copyright notice
# 	[1993/01/07  01:12:48  bbelch]
# 
# 	Modified test so it would run.
# 	[1992/07/02  22:04:20  grober_j]
# 
# 	Modified test so it would run.
# 	[1992/07/02  22:04:20  grober_j]
# 
# Revision 1.1.2.3  1992/07/29  19:41:19  m_sawyer
# 	Updated dacl tests.
# 	[1992/07/28  20:34:03  m_sawyer]
# 
# 	    Loading drop Dce1_0_1b19
# 	[1992/06/16  05:12:49  root]
# 
# Revision 1.1.2.2  1992/07/29  14:54:12  m_sawyer
# 	Updated dacl tests.
# 	[1992/07/28  20:34:03  m_sawyer]
# 
# 	    Loading drop Dce1_0_1b19
# 	[1992/06/16  05:12:49  root]
# 
# Revision 1.1  1992/01/19  14:32:16  devrcs
# 	Initial revision
# 
# $EndLog$
#
# This script tests that read operations require only read access. 
#
# Notroot needs at least insert and read access to the root directory
# in order to be able to create directories beneath it and skulk the
# root. Notroot also needs write access to the default clearinghouse
# in order to create directories under the parent.
#
# Make sure no default container or inital object acls exist on parent
# 

myname=`basename $0`

echo ""
echo "Create directory tree first"
echo ""

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp create dir /.:/dacl_reads"
cdscp create dir /.:/dacl_reads
echo "cdscp set dir /.:/dacl_reads to skulk"
cdscp set dir /.:/dacl_reads to skulk
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp create obj /.:/dacl_reads/obj1"
cdscp create obj /.:/dacl_reads/obj1
echo "cdscp create link /.:/dacl_reads/lnk1"
cdscp create link /.:/dacl_reads/lnk1 CDS_LinkTarget /.:/dacl_reads/obj1
echo "cdscp create dir /.:/dacl_reads/dir1"
cdscp create dir /.:/dacl_reads/dir1
echo "cdscp set dir /.:/dacl_reads to skulk"
cdscp set dir /.:/dacl_reads to skulk
echo "cdscp set dir /.:/dacl_reads/dir1 to skulk"
cdscp set dir /.:/dacl_reads/dir1 to skulk

#
# Need administer (or write, insert or delete) access in order to
# set dir to skulk.
#

echo ""
echo " Set acls to administer, read and control"
echo ""

echo "#"
echo "# acl_edit"
echo "#"

echo "acl_edit /.:/dacl_reads -m user:notroot:rcia"
acl_edit /.:/dacl_reads -m user:notroot:rcia
echo "acl_edit -e /.:/dacl_reads/obj1 -m user:notroot:cr"
acl_edit -e /.:/dacl_reads/obj1 -m user:notroot:cr
echo "acl_edit -e /.:/dacl_reads/lnk1 -m user:notroot:cr"
acl_edit -e /.:/dacl_reads/lnk1 -m user:notroot:cr
echo "acl_edit -e /.:/dacl_reads/dir1 -m user:notroot:cr"
acl_edit -e /.:/dacl_reads/dir1 -m user:notroot:cr
echo "acl_edit  /.:/dacl_reads/dir1 -m user:notroot:rcia"
acl_edit  /.:/dacl_reads/dir1 -m user:notroot:rcia

echo ""
echo "Try reads - should work"
echo ""

echo "#"
echo "# cdscp"
echo "#" 

echo "cdscp set dir /.:/dacl_reads to skulk"
cdscp set dir /.:/dacl_reads to skulk
echo "cdscp set dir /.:/dacl_reads/dir1 to skulk"
cdscp set dir /.:/dacl_reads/dir1 to skulk
echo "cdscp show dir /.:/dacl_reads"
cdscp show dir /.:/dacl_reads
echo "cdscp show child /.:/dacl_reads/dir1"
cdscp show child /.:/dacl_reads/dir1
echo "cdscp show obj /.:/dacl_reads/obj1"
cdscp show obj /.:/dacl_reads/obj1
echo "cdscp show link /.:/dacl_reads/lnk1"
cdscp show link /.:/dacl_reads/lnk1
echo "cdscp list dir /.:/dacl_reads/\*"
cdscp list dir /.:/dacl_reads/\*
echo "cdscp list child /.:/dacl_reads/\*"
cdscp list child /.:/dacl_reads/\*
echo "cdscp list obj /.:/dacl_reads/\*"
cdscp list obj /.:/dacl_reads/\*
echo "cdscp list link /.:/dacl_reads/\*"
cdscp list link /.:/dacl_reads/\*

echo ""
echo "Set to read only on parent directory and check lists"
echo ""

echo "#"
echo "# acl_edit"
echo "#"

echo "acl_edit -e /.:/dacl_reads/obj1 -m user:notroot:wdtc"
acl_edit -e /.:/dacl_reads/obj1 -m user:notroot:wdtc
echo "acl_edit -e /.:/dacl_reads/lnk1 -m user:notroot:wdtc"
acl_edit -e /.:/dacl_reads/lnk1 -m user:notroot:wdtc
echo "acl_edit -e /.:/dacl_reads/dir1 -m user:notroot:wdtc"
acl_edit -e /.:/dacl_reads/dir1 -m user:notroot:wdtc
echo "acl_edit  /.:/dacl_reads/dir1 -m user:notroot:wdtc"
acl_edit /.:/dacl_reads/dir1 -m user:notroot:wdtc

echo ""
echo "Try lists and expect them to work"
echo ""

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp set dir /.:/dacl_reads to skulk"
cdscp set dir /.:/dacl_reads to skulk
echo "cdscp set dir /.:/dacl_reads/dir1 to skulk"
cdscp set dir /.:/dacl_reads/dir1 to skulk
echo "cdscp list dir /.:/dacl_reads/\*"
cdscp list dir /.:/dacl_reads/\*
echo "cdscp list child /.:/dacl_reads/\*"
cdscp list child /.:/dacl_reads/\*
echo "cdscp list obj /.:/dacl_reads/\*"
cdscp list obj /.:/dacl_reads/\*
echo "cdscp list link /.:/dacl_reads/\*"
cdscp list link /.:/dacl_reads/\*

echo ""
echo "Try shows of dir1, obj1, and lnk1. Expect them to fail"
echo ""

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp show child /.:/dacl_reads/dir1"
cdscp show child /.:/dacl_reads/dir1
echo "cdscp show obj /.:/dacl_reads/obj1"
cdscp show obj /.:/dacl_reads/obj1
echo "cdscp show link /.:/dacl_reads/lnk1"
cdscp show link /.:/dacl_reads/lnk1

echo ""
echo "Set directory to everything but read"
echo ""

echo "#"
echo "# acl_edit"
echo "#"

echo "acl_edit /.:/dacl_reads -m user:notroot:wdtc"
acl_edit /.:/dacl_reads -m user:notroot:wdtc

echo ""
echo "Do reads - expect to fail"
echo ""

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp set dir /.:/dacl_reads to skulk"
cdscp set dir /.:/dacl_reads to skulk
echo "cdscp set dir /.:/dacl_reads/dir1 to skulk"
cdscp set dir /.:/dacl_reads/dir1 to skulk
echo "cdscp show dir /.:/dacl_reads"
cdscp show dir /.:/dacl_reads
echo "cdscp show child /.:/dacl_reads/dir1"
cdscp show child /.:/dacl_reads/dir1
echo "cdscp show obj /.:/dacl_reads/obj1"
cdscp show obj /.:/dacl_reads/obj1
echo "cdscp show link /.:/dacl_reads/lnk1"
cdscp show link /.:/dacl_reads/lnk1
echo "cdscp list dir /.:/dacl_reads/\*"
cdscp list dir /.:/dacl_reads/\* 
echo "cdscp list child /.:/dacl_reads/\*"
cdscp list child /.:/dacl_reads/\*
echo "cdscp list obj /.:/dacl_reads/\*"
cdscp list obj /.:/dacl_reads/\*
echo "cdscp list link /.:/dacl_reads/\*"
cdscp list link /.:/dacl_reads/\*

echo ""
echo "Cleanup"
echo ""

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp delete object /.:/dacl_reads/obj1"
cdscp delete object /.:/dacl_reads/obj1
echo "cdscp delete link /.:/dacl_reads/lnk1"
cdscp delete link /.:/dacl_reads/lnk1
echo "cdscp delete directory /.:/dacl_reads/dir1"
cdscp delete directory /.:/dacl_reads/dir1
echo "cdscp delete directory /.:/dacl_reads"
cdscp delete directory /.:/dacl_reads
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
