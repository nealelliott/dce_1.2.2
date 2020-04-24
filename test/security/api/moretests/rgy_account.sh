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
# $Log: rgy_account.sh,v $
# Revision 1.1.8.2  1996/03/11  02:35:08  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:51  marty]
#
# Revision 1.1.8.1  1995/12/11  21:49:37  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:43  root]
# 
# Revision 1.1.4.2  1992/12/31  17:31:40  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:56:59  htf]
# 
# Revision 1.1.2.2  1992/03/27  20:58:17  bmw
# 	Modified test cases that were incorrect.
# 	[1992/03/27  20:51:06  bmw]
# 
# Revision 1.1  1992/01/19  04:08:13  devrcs
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
# src/test/security/api/moretests/rgy_account.sh, , dce_osf, dce.75d  
#   
#	Extracted	1/4/92 17:02:19
#	from delta on	12/2/91 15:49:00
#
#
# rgy account functions command line test script
#
# usage: rgy_account.sh [-d n] [-s]
#

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
    -s) SAVE="-s" ;;
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

echo
echo "Begin rgy_edit account tests"
echo


    rgy.sh $DEBUG $SAVE " " "Adding principal: bullwinkle" \
           "do principal" \
           "a bullwinkle 312 -f \"Bullwinkle J. Moose\""
    EXIT=$?



    rgy.sh $DEBUG $SAVE " " "Adding Group: rom" \
           "do group" \
           "a rom 202 -f \"Royal order of Mooses\""
    EXIT=$?


    rgy.sh $DEBUG $SAVE " " "Adding Org: jwe" \
           "do org" \
           "a jwe 302 -f \"J. Ward Enterprises\""
    EXIT=$?



   rgy.sh $DEBUG $SAVE "RA1" "Adding Account: bullwinkle,rom,jwe" \
           "do acc" \
           'a bullwinkle -g none -o none -pw mooseandsquirrel -mp -dce- -m "This is a test" -h //vw/finger -s /bin/ksh'

    EXIT=$?




    rgy.sh $DEBUG $SAVE "RA2" "Deleting Account: bullwinkle" \
           "do acc" \
           "del -p bullwinkle"
    EXIT=$?



    rgy.sh $DEBUG $SAVE " " "Deleting principal: bullwinkle" \
           "do principal" \
           "del bullwinkle"
    EXIT=$?



    rgy.sh $DEBUG $SAVE " " "Deleting Group: rom" \
           "do group" \
           "del rom"
    EXIT=$?


    rgy.sh $DEBUG $SAVE " " "Deleting Org: jwe" \
           "do org" \
           "del jwe"
    EXIT=$?

echo
echo "End rgy_edit account tests"
echo


exit $EXIT

#
# End script rgy_account.sh
#

