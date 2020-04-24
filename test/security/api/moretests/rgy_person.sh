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
# $Log: rgy_person.sh,v $
# Revision 1.1.6.2  1996/03/11  02:35:29  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:01  marty]
#
# Revision 1.1.6.1  1995/12/11  21:49:55  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:52  root]
# 
# Revision 1.1.4.2  1994/08/04  16:16:09  mdf
# 	Hewlett Packard Security Code Drop
# 	[1994/07/28  11:41:22  mdf]
# 
# Revision 1.1.4.1  1994/07/15  15:03:24  mdf
# 	Hewlett Packard Security Code Drop
# 	[1994/07/14  17:24:43  mdf]
# 
# Revision 1.1.2.2  1992/12/31  17:32:31  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:57:51  htf]
# 
# Revision 1.1  1992/01/19  04:08:23  devrcs
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
# src/test/security/api/moretests/rgy_person.sh, , dce_osf, dce.75d  
#   
#	Extracted	1/4/92 17:02:21
#	from delta on	12/2/91 15:49:26
#
#
# rgy_edit person command line test script
#
# usage: rgy_person.sh  [-d n] [-s]
#


# a[dd] [principal_name [unix_num][-f fullname][-q quota][-al] ] |


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


    rgy.sh $DEBUG $SAVE "RP1" "Adding Person: rocky" \
           "do principal" \
           "a rocky  1002 -f \"Rocket J. Squirrel\""
    EXIT=$?


    rgy.sh $DEBUG $SAVE "RP2" "Changing Person rocky to bullwinkle" \
           "do principal" \
           "change rocky -n bullwinkle"
    EXIT=$?


#    rgy.sh $DEBUG $SAVE "RP3" "Changing Unix ID from 102 to 103" \
#           "do principal" \
#           "change bullwinkle -u 1003"
#    EXIT=$?
#
#
#    rgy.sh $DEBUG $SAVE "RP4" "Changing full name from Rocket J. Squirrel to Bullwinkle J. Moose" \
#           "do principal" \
#           "change bullwinkle -f \"Bullwinkle J. Moose\""
#    EXIT=$?
#

    rgy.sh $DEBUG $SAVE "RP5" "Deleting Entry: bullwinkle " \
           "do principal" \
           "del bullwinkle"
    EXIT=$?

    rgy.sh $DEBUG $SAVE -f "RP6" "Deleting non-existent entry: bullwinkle" \
           "do org" \
           "del bullwinkle"
    EXIT=$?


# Polymorphic PGOs
# Deliberate ordering, testing the four flavors of aggregate creation
# and destruction:  dir=>pgodir, pgo=>pgodir, pgodir=>dir, pgodir=>pgo

    rgy.sh $DEBUG $SAVE "RP7" "Adding Person: ab/bc" \
           "do principal" "a ab/bc"
    EXIT=$?

    rgy.sh $DEBUG $SAVE "RP8" "Adding Person: ab" \
           "do principal" "a ab"
    EXIT=$?

    rgy.sh $DEBUG $SAVE "RP9" "Adding Person: ab/bc/cd" \
           "do principal" "a ab/bc/cd"
    EXIT=$?

    rgy.sh $DEBUG $SAVE "RP10" "Deleting Person: ab/bc" \
           "do principal" "del ab/bc"
    EXIT=$?

    rgy.sh $DEBUG $SAVE "RP11" "Deleting Person: ab/bc/cd" \
           "do principal" "del ab/bc/cd"
    EXIT=$?

    rgy.sh $DEBUG $SAVE "RP12 " "Deleting Person: ab" \
           "do principal" "del ab"
    EXIT=$?

#
exit $EXIT

#
# End script rgy_principal.sh
#

