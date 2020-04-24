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
# $Log: rgy_member.sh,v $
# Revision 1.1.6.2  1996/03/11  02:35:24  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:58  marty]
#
# Revision 1.1.6.1  1995/12/11  21:49:50  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:49  root]
# 
# Revision 1.1.4.2  1992/12/31  17:32:14  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:57:33  htf]
# 
# Revision 1.1.2.2  1992/03/27  20:59:38  bmw
# 	Modified test cases that were incorrect.
# 	[1992/03/27  20:51:42  bmw]
# 
# Revision 1.1  1992/01/19  04:08:19  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# 
# 
# Copyright 1991 by Apollo Computer, Inc.
# 
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Apollo Computer, Inc not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Apollo Computer, Inc. makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
# 
#
# src/test/security/api/moretests/rgy_member.sh, , dce_osf, dce.75d  
#   
#	Extracted	1/4/92 17:02:20
#	from delta on	12/2/91 15:49:18
#
#
# rgy_edit add command line test script
#
#

 
# Usage:
# m[ember] [group_name|org_name [-a member_list] [-r member_list]]


#
# Parse command line here.  Obtain 1) the value of the -d switch, if specified;
# 2) the -s switch, if specified; 3) the numbered test case; 4) the test case 
# description string.  Note that command line ordering is not significant to this
# parser.
#

SAVE=""
DEBUG=""
DFLAG="0"

for LCV in "$@"
do
if [ "$LCV" = "-d" ]
then
  DFLAG="-d"
  DEBUG="-d 0"
  continue
fi
  if [ "$DFLAG" = "-d" ]
  then
    case $LCV in
      "1")  DEBUG="-d 1" ;;
      "2")  DEBUG="-d 2" ;;
      "3")  DEBUG="-d 3" ;;
      "32") DEBUG="-d 32" ;;
      "33") DEBUG="-d 33" ;;
      "34") DEBUG="-d 34" ;; 
      "63") DEBUG="-d 63" ;;
    esac
    DFLAG="0"
    continue
    fi
  case "$LCV" in
    -d*) DMASK=`echo $LCV | sed 's/^-d//'`
	 case $DMASK in
      "1")  DEBUG="-d 1" ;;
      "2")  DEBUG="-d 2" ;;
      "3")  DEBUG="-d 3" ;;
      "32") DEBUG="-d 32" ;;
      "33") DEBUG="-d 33" ;;
      "34") DEBUG="-d 34" ;; 
      "63") DEBUG="-d 63" ;;
	 esac  ;;
    -s) SAVE="-s"; SFLAG="1" ;;
#    *[1-9]*) CASEVAL=$LCV ;;
#    *[" "]*) LBLSTR=$LCV ;;
  esac
done

#
# Initialize program variables.
#

EXIT=0
CLEANUP=0
STATUS=0
SFLAG="0"

   
echo
echo "Begin rgy_edit member function tests"
echo


    rgy.sh $DEBUG $SAVE \
           -v "python *2003" \
           "RE SETUP" "Adding Group: python" \
           "do group" \
           "a python 2003 -f \"The comfy chair!\"" \
           "v"
    EXIT=$?


    rgy.sh $DEBUG $SAVE \
           "RE SETUP" "Adding Principal: rocky" \
           "do principal" \
           "a rocky"
    EXIT=$?


  rgy.sh $DEBUG $SAVE \
           -v "root, rocky, dce-ptgt" \
           "RE1" "Test add/list members: root, rocky, dce-ptgt to python" \
           "do group" \
           "mem python -a root" \
           "mem python -a rocky" \
           "mem python -a dce-ptgt" \
           "v python -m"
    EXIT=$?

  rgy.sh $DEBUG $SAVE \
           -vn "root, rocky, dce-ptgt" \
           "RE2" "Test delete members: root, rocky, dce-ptgt from python" \
           "do group" \
           "mem python -r root" \
           "mem python -r rocky" \
           "mem python -r dce-ptgt" \
           "v python -m"
    EXIT=$?


    rgy.sh $DEBUG $SAVE \
           "RE CLEANUP" "Deleting Principal: rocky" \
           "do principal" \
           "del rocky"
    EXIT=$?


    rgy.sh $DEBUG $SAVE \
           "RE CLEANUP" "Deleting Group: python" \
           "do group" \
           "del python" \
           "y"
    EXIT=$?

echo
echo "End rgy_edit member function tests"
echo


exit $EXIT

#
# End script rgy_member.sh
#
