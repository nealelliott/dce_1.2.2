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
# $Log: rgy_org.sh,v $
# Revision 1.1.4.2  1996/03/11  02:35:28  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:00  marty]
#
# Revision 1.1.4.1  1995/12/11  21:49:54  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:51  root]
# 
# Revision 1.1.2.2  1992/12/31  17:32:27  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:57:46  htf]
# 
# Revision 1.1  1992/01/19  04:07:14  devrcs
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
# src/test/security/api/moretests/rgy_org.sh, , dce_osf, dce.75d  
#   
#	Extracted	1/4/92 16:40:58
#	from delta on	8/5/91 14:44:56
#
#
# rgy_edit organization command line test script
#
# usage: rgy_person.sh [-d n] [-s]
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
if [ "$LCV" = '-d' ]
then
  DFLAG='-d'
  DEBUG='-d 0'
  continue
fi
  if [ "$DFLAG" = '-d' ]
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


    rgy.sh $DEBUG $SAVE "RO1" "Adding Org: jwe" \
           "do org" \
           "a jwe 302 -f \"J. Ward Enterprises\""
    EXIT=$?


    rgy.sh $DEBUG $SAVE "RO2" "Changing Org jwe to mbg" \
           "do org" \
           "change jwe -n mbg"
    EXIT=$?


#    rgy.sh $DEBUG $SAVE "RO3" "Changing Unix ID from 302 to 303" \
#           "do org" \
#           "change mbg -u 303"
#    EXIT=$?
#
#
#    rgy.sh $DEBUG $SAVE "RO4" "Changing full name from J. Ward Enterprises to Mr. Big\'s Gang" \
#           "do org" \
#           "change mbg -f \"Mr. Big\'s Gang\""
#    EXIT=$?
#

    rgy.sh $DEBUG $SAVE "RO5" "Deleting Entry: mbg" \
           "do org" \
           "del mbg"
    EXIT=$?

    rgy.sh $DEBUG $SAVE -f "RO6" "Deleting non-existent entry: mbg" \
           "do org" \
           "del mbg"
    EXIT=$?


#
exit $EXIT

#
# End script rgy_org.sh
#

