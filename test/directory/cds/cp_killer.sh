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
# $Log: cp_killer.sh,v $
# Revision 1.1.10.2  1996/03/11  01:44:12  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:15:16  marty]
#
# Revision 1.1.10.1  1995/12/08  22:09:02  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:43:49  root]
# 
# Revision 1.1.6.2  1994/06/02  17:17:17  proulx
# 	Remove subtree, foreign and negative tests from alltests list
# 	[1994/06/02  15:16:51  proulx]
# 
# Revision 1.1.3.2  1993/01/12  18:27:33  bbelch
# 	Embedding copyright notice
# 	[1993/01/07  01:11:48  bbelch]
# 
# Revision 1.1  1992/01/19  14:32:25  devrcs
# 	Initial revision
# 
# $EndLog$
#
###############################################################################
#
# killer.sh - Script to run the dns control program tests.
#
# revision history:
# -----------------
# 19-nov-90 mbs revised original 'killer.sh'
# 20-nov-90 mbs modified to use 'cp_test.sh'
# 29-nov-90 mbs pass command line options to cp_test.sh
# 10-dec-90 mbs add replica to the list of tests
# 24-may-91 mbs add server, clerk and clearinghouse to list of tests
# 29-may-91 mbs don't do the group tests
# 22-feb-94 jmp add test scripts: subtree, foreign, intercell, negative,
#	        and unauthorised
###############################################################################

# Some definitions

workdir=`pwd`
mytests=${workdir}/cp_killer.tests
mycomments=${workdir}/cp_killer.tmp
needfiles=./cp_test.sh

#
# Test script list. 
#
alltests="directory\
	 object\
	 softlink\
	 replica\
	 childpointer\
         server\
         clerk\
	 clearinghouse\
	 misc"
#
# See if we have everything we need to continue
#
for f in ${needfiles}
  do
    if [ ! -f $f ]; then
        echo "I can't find '$f' anywhere ... aborting test!!!"
        exit 1
    fi
  done
#
# Create our test file, it's a composite of all the other tests.
#
echo "gathering test lines ..."
echo "# cp_killer.tests" > ${mytests}
echo "#" >> ${mytests}
echo "# created by $0 on `date`" >> ${mytests}
echo "#" >> ${mytests}
#
#
#
for testname in ${alltests}
  do
    testfile=./cp_${testname}.tests
    echo "    ${testfile}"
    if [ -f ${testfile} ]; then
        echo "    #"             > ${mycomments}
        echo "    # ${testfile}" >> ${mycomments}
        echo "    #"             >> ${mycomments}
	cat ${mycomments} >> ${mytests}
        cat ${testfile} >> ${mytests}
	rm ${mycomments}
    else
	echo "${testfile} not found."
    fi
  done
#
# chain to 'cp_test.sh' to do the real work
#
echo "chaining to 'cp_test.sh'"
./cp_test.sh $@ killer







