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
# $Log: dacl_testing.sh,v $
# Revision 1.1.14.2  1996/03/11  01:45:00  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:15:47  marty]
#
# Revision 1.1.14.1  1995/12/08  22:09:43  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:44:14  root]
# 
# Revision 1.1.12.1  1993/10/28  22:01:41  pwang
# 	Replaced -or with -o [OT#9288]
# 	[1993/10/28  22:01:16  pwang]
# 
# Revision 1.1.10.3  1993/08/26  21:54:09  pwang
# 	Reoved extra newline and charcter in usage message [OT#8083]
# 	[1993/08/26  21:43:19  pwang]
# 
# Revision 1.1.10.2  1993/06/17  21:04:44  sommerfeld
# 	make parameter list consistent
# 	[1993/05/20  18:59:39  smythe]
# 
# Revision 1.1.5.4  1993/02/24  13:21:13  jd
# 	Script cleanup
# 	[1993/02/24  13:14:05  jd]
# 
# Revision 1.1.5.3  1993/02/11  19:54:44  jd
# 	Incorporate new dec code.
# 	[1993/02/10  14:16:08  jd]
# 
# Revision 1.1.5.2  1993/01/12  18:28:59  bbelch
# 	Embedding copyright notice
# 	[1993/01/07  01:12:57  bbelch]
# 
# Revision 1.1.2.2  1992/05/12  19:29:54  weisman
# 		Corrected options to acl_edit for deleting initial container
# 		and initial objects.
# 	[1992/05/12  19:28:54  weisman]
# 
# Revision 1.1  1992/01/19  14:32:19  devrcs
# 	Initial revision
# 
# $EndLog$
#
# This script assumes the nameserver is called  and
#  
# that we are running as notroot.
#
# Usage:
#	dacl_testing.sh -ch1 <clearinghouse1> -ch2 <clearinghouse2>
#
# Both clearinghouses must exist and notroot needs at least write 
# and control access to both of them. Notroot also needs write
# access to the default clearinghouse in order to create
# subdirectories beneath it.
#
# Notroot also needs at least insert and read access to the root
# directory in order to be able to create directories beneath it and
# skulk the root.
#


for i in $*
do
    case $i in
        -ch1)  ch1=$2; shift;shift;;
        -ch2)  ch2=$2; shift;shift;;
    esac
done

if [ x${ch1} = x"" -o x${ch2} = x"" ]
then
    echo "$0 : -ch1 clearinghouse1 -ch2 clearinghouse2"
    exit
fi


echo ""
echo "Test Reads"
echo ""

dacl_reads.sh > dacl_reads.log 2>&1

echo ""
echo "Test Creates"
echo ""

dacl_creates.sh   > dacl_creates.log 2>&1

echo ""
echo "Test Deletes"
echo ""

dacl_deletes.sh > dacl_deletes.log 2>&1

echo ""
echo "Test Modifies"
echo ""

dacl_modifies.sh > dacl_modifies.log 2>&1

echo ""
echo "Test Replication"
echo ""

dacl_replicas.sh ${ch1} ${ch2} > dacl_replicas.log 2>&1

