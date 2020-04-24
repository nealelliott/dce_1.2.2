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
# $Log: dacl_replicas.sh,v $
# Revision 1.1.14.2  1996/03/11  01:44:51  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:15:42  marty]
#
# Revision 1.1.14.1  1995/12/08  22:09:37  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:44:11  root]
# 
# Revision 1.1.12.1  1994/06/10  17:25:37  jd
# 	Clean up on the way out.
# 	[1994/06/10  17:19:18  jd]
# 
# Revision 1.1.10.3  1993/08/26  21:54:12  pwang
# 	Server princ need w perm to the CH where the replica is
# 	changed to read-only, not the CH that stores new master
# 	for "cdscp set dir to new epoch" [OT#8083]
# 	[1993/08/26  21:52:17  pwang]
# 
# Revision 1.1.10.2  1993/06/17  21:04:00  sommerfeld
# 	get hostname automatically
# 	retain some permissions on ch2 for cell integrity
# 	   since it may could be the default clearinghouse
# 	set server write access for correct ch (new master)
# 	[1993/05/20  17:51:22  smythe]
# 
# Revision 1.1.5.5  1993/02/24  13:21:09  jd
# 	Various script fix ups...
# 	[1993/02/23  23:03:56  jd]
# 
# Revision 1.1.5.4  1993/02/11  19:54:26  jd
# 	Incorporate new dec code.
# 	[1993/02/10  14:15:55  jd]
# 
# Revision 1.1.5.3  1993/02/01  21:33:08  hinman
# 	SVR4 changes
# 	[1993/02/01  01:12:52  bbelch]
# 
# Revision 1.1.5.2  1993/01/12  18:28:51  bbelch
# 	Embedding copyright notice
# 	[1993/01/07  01:12:52  bbelch]
# 
# Revision 1.1.2.3  1992/07/29  19:41:21  m_sawyer
# 	Updated dacl tests.
# 	[1992/07/28  20:34:03  m_sawyer]
# 
# 	    Loading drop Dce1_0_1b19
# 	[1992/06/16  05:12:49  root]
# 
# Revision 1.1.2.2  1992/07/29  14:54:19  m_sawyer
# 	Updated dacl tests.
# 	[1992/07/28  20:34:20  m_sawyer]
# 	Revision 1.1.1.2  1992/07/28  20:34:20  m_sawyer
# 	Updated dacl tests.
# 
# $EndLog$
#
# This script tests access to perform replication operations.
#
# usage:
#   dacl_replicas.sh <clearinghouse1> <clearinghouse2>
#
# Make sure no default container or inital object acls exist on parent
# Both clearinghouses must exist and notroot needs at least control
# access to both of them.
#

myname=`basename $0`

if [ $# -ne 2 ]; then
   echo "Usage: ${myname} <clearinghouse1 name without /.:> <clearinghouse2 name without /.:>"
   exit
fi

USERNAME="notroot"
# get hostname of ch2 server automatically
cdscp create directory /.:/dacl_ch2_dir clearinghouse /.:/$2
cdscp set dir /.: to skulk
HOSTNAME=`acl_edit /.:/dacl_ch2_dir -l | fgrep user:hosts/ | cut -d/ -f2`
cdscp delete directory /.:/dacl_ch2_dir
cdscp set dir /.: to skulk
echo "HOSTNAME = ${HOSTNAME}"

CDS_SERVER="hosts/${HOSTNAME}/cds-server"

export USERNAME HOSTNAME CDS_SERVER
echo "#"
echo "# acl_edit"
echo "#"
echo "acl_edit /.:/$1 -m user:${USERNAME}:wc"
acl_edit  /.:/$1 -m user:${USERNAME}:wc
echo "acl_edit /.:/$2 -m user:${USERNAME}:c"
acl_edit  /.:/$2 -m user:${USERNAME}:c

echo ""
echo "Creates top level directory first "
echo ""

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp create dir /.:/dacl_replicas clear /.:/$1"
cdscp create dir /.:/dacl_replicas clear /.:/$1
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_replicas to skulk"
cdscp set dir /.:/dacl_replicas to skulk

echo ""
echo "Set access to everything but admin and expect all to fail"
echo ""

echo "#"
echo "# acl_edit"
echo "#"

echo "acl_edit /.:/dacl_replicas -s user:${USERNAME}:rwditc"
acl_edit /.:/dacl_replicas -s user:${USERNAME}:rwditc

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp create replica /.:/dacl_replicas clea /.:/$2"
cdscp create replica /.:/dacl_replicas clea /.:/$2
echo "cdscp set dir /.:/dacl_replicas to new epoch mast /.:/$1"
cdscp set dir /.:/dacl_replicas to new epoch mast /.:/$1
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_replicas to skulk"
cdscp set dir /.:/dacl_replicas to skulk

echo ""
echo "Set access to admin and expect failure - server doesn't have access"
echo ""

echo "#"
echo "# acl_edit"
echo "#"

echo "acl_edit /.:/dacl_replicas -s user:${USERNAME}:rac"
acl_edit /.:/dacl_replicas -s user:${USERNAME}:rac

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp create replica /.:/dacl_replicas clea /.:/$2"
cdscp create replica /.:/dacl_replicas clea /.:/$2

echo ""
echo "Set server access to rwa but still expect create to fail"
echo ""

echo "#"
echo "# acl_edit"
echo "#"

echo "acl_edit /.:/dacl_replicas -m user:${CDS_SERVER}:rwa"
acl_edit /.:/dacl_replicas -m user:${CDS_SERVER}:rwa

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp create replica /.:/dacl_replicas clea /.:/$2"
cdscp create replica /.:/dacl_replicas clea /.:/$2

echo ""
echo "Add write for user to clear - should work"
echo ""

echo "#"
echo "# acl_edit"
echo "#"

# save other permissions since this could be default clearinghouse
echo "acl_edit /.:/$2 -m user:${USERNAME}:wrc"
acl_edit  /.:/$2 -m user:${USERNAME}:wrc
echo "acl_edit /.:/$1 -m user:${CDS_SERVER}:"
acl_edit  /.:/$1 -m user:${CDS_SERVER}:

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp create replica /.:/dacl_replicas clea /.:/$2"
cdscp create replica /.:/dacl_replicas clea /.:/$2
echo "cdscp set dir /.: to skulk"
cdscp set dir /.: to skulk
echo "cdscp set dir /.:/dacl_replicas to skulk"
cdscp set dir /.:/dacl_replicas to skulk
echo "cdscp set dir /.:/dacl_replicas to new epoch mast /.:/$1 read /.:/$2"
cdscp set dir /.:/dacl_replicas to new epoch mast /.:/$1 read /.:/$2

echo ""
echo "New epoch should fail changing masters"
echo ""

echo "cdscp set dir /.:/dacl_replicas to new epoch mast /.:/$2 read /.:/$1"
cdscp set dir /.:/dacl_replicas to new epoch mast /.:/$2 read /.:/$1

echo ""
echo "Set server access so new epoch changing masters succeeds"
echo ""

# need server write access to new master clearinghouse
echo "acl_edit /.:/$1 -m user:${CDS_SERVER}:wc"
acl_edit  /.:/$1 -m user:${CDS_SERVER}:wc

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp set dir /.:/dacl_replicas to new epoch mast /.:/$2 read /.:/$1"
cdscp set dir /.:/dacl_replicas to new epoch mast /.:/$2 read /.:/$1

echo ""
echo "Set access to everything but admin and expect remove to fail"
echo ""

echo "#"
echo "# acl_edit"
echo "#"

echo "acl_edit /.:/dacl_replicas -m user:${USERNAME}:rwdtic"
acl_edit /.:/dacl_replicas -m user:${USERNAME}:rwdtic
echo "acl_edit /.:/$1 -m user:${USERNAME}:c"
acl_edit  /.:/$1 -m user:${USERNAME}:c

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp delete replica /.:/dacl_replicas clear /.:/$1"
cdscp delete replica /.:/dacl_replicas clear /.:/$1

echo ""
echo "Set access to admin, but still fails cause of clearinghouse access"
echo ""

echo "#"
echo "# acl_edit"
echo "#"

echo "acl_edit /.:/dacl_replicas -m user:${USERNAME}:rac"
acl_edit /.:/dacl_replicas -m user:${USERNAME}:rac

echo "#"
echo "# cdscp"
echo "#"

echo "cdscp delete replica /.:/dacl_replicas clear /.:/$1"
cdscp delete replica /.:/dacl_replicas clear /.:/$1

echo ""
echo "Set clear access - should work"
echo ""

echo "#"
echo "# acl_edit"
echo "#"

echo "acl_edit /.:/$1 -m user:${USERNAME}:wc"
acl_edit  /.:/$1 -m user:${USERNAME}:wc

echo "#"
echo "# cdscp"
echo "#"

echo "acl_edit /.:/dacl_replicas -m user:${USERNAME}:rwdtac"
acl_edit /.:/dacl_replicas -m user:${USERNAME}:rwdtac
echo "cdscp delete repclica /.:/dacl_replicas clearinghouse /.:/$1"
cdscp delete replica /.:/dacl_replicas clearinghouse /.:/$1
cdscp set dir /.: to skulk
echo "cdscp delete directory /.:/dacl_replicas"
cdscp delete dir /.:/dacl_replicas
cdscp set dir /.: to skulk
