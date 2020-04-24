#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: test_mvr.sh,v $
# Revision 1.1.11.2  1996/03/11  01:46:08  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:16:28  marty]
#
# Revision 1.1.11.1  1995/12/08  22:12:09  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:45:02  root]
# 
# Revision 1.1.8.5  1994/07/11  22:25:45  keutel
# 	OT 11247: oidt ---> ./oidt
# 	[1994/07/11  22:24:04  keutel]
# 
# Revision 1.1.8.4  1994/07/06  15:53:48  marrek
# 	Add version string in July 1994 code drop.
# 	[1994/07/06  15:41:48  marrek]
# 
# Revision 1.1.8.3  1994/05/10  15:32:28  marrek
# 	Merged with changes from 1.1.8.2
# 	[1994/05/10  15:31:30  marrek]
# 
# 	April 1994 code submission.
# 
# Revision 1.1.8.1  1994/02/22  17:45:28  marrek
# 	Code drop for dce1.1.
# 	[1994/02/07  11:56:03  marrek]
# 
# Revision 1.1.6.1  1993/10/27  15:23:07  zeliff
# 	OT9269: Fixes to make mavros test work as documented
# 	[1993/10/27  15:18:22  zeliff]
# 
# Revision 1.1.4.3  1993/02/18  04:30:27  treff
# 	2/17/93 DCT @ OSF
# 	Fixed it so it would work.  -I and -L lines,
# 	DELTA, "./test_mvr", etc.
# 	[1993/02/18  04:29:57  treff]
# 
# Revision 1.1.4.2  1993/01/04  17:03:10  htf
# 	Embedded copyright notice
# 	[1993/01/04  16:15:57  htf]
# 
# Revision 1.1.2.2  1992/06/11  21:28:39  melman
# 	New GDS test code drop.
# 	[1992/06/11  21:20:28  melman]
# 
# $EndLog$
#
# $RCSfile: test_mvr.sh,v $ $Revision: 1.1.11.2 $ $Date: 1996/03/11 01:46:08 $
#

# DELTA is correction of the time
DELTA="-delta +300"
echo "DELTA is $DELTA"

echo "running test_mvr ..."
./test_mvr -ber -hard 1000 \
	-load test.entries -lbad test.failures \
	-dtxt test.text_out -ltxt test.text_ref -diff test.dump \
	${DELTA} -utc time.test -copy -x509 test.x509 \
	<test.input >test.output 2>test.errlog

if [ -s test.errlog ]
then
echo "this is test.errlog ..."
cat test.errlog
else
echo "test.errlog is empty - as it should be"
fi

echo
echo "running oidt ..."
./oidt <oidt.input >oidt.diff

diff oidt.diff oidt.output >oidt.errlog 2>&1
if [ -s oidt.errlog ]
then
echo "this is the diff between the expected and the real output ..."
cat oidt.errlog
else
echo "no diff between the expected and the real output - as it should be"
fi

