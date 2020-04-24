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
# $Log: rgy_delete.sh,v $
# Revision 1.1.4.2  1996/03/11  02:35:18  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:56  marty]
#
# Revision 1.1.4.1  1995/12/11  21:49:46  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:47  root]
# 
# Revision 1.1.2.2  1992/12/31  17:32:02  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:57:21  htf]
# 
# Revision 1.1  1992/01/19  04:08:18  devrcs
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
# src/test/security/api/moretests/rgy_delete.sh, , dce_osf, dce.75d  
#   
#	Extracted	1/4/92 17:02:20
#	from delta on	12/2/91 15:49:14
#
#
# rgy_edit add command line test script
#
#


# a[dd] [principal_name [unix_num][-f fullname][-q quota][-al] ] |
#       [group_name  [unix_num][-f fullname][-nl] ] |
#       [org_name    [unix_num][-f fullname] ]
#
# a[dd] [principal] 
#         [ -g group -o org -mp password [-pw password | -rp]
#         [-m misc_info] [-h homedir] [-s shell]
#         [-x account_exp_date|none] [-pv | -pnv] [-av | -anv] ]
#        OPTIONS:    -g      group name
#                    -o      org name
#                    -mp     my-password, for key validation
#                    -pw     new password for account
#                    -rp     generate random password
#                    -pv     password valid
#                    -pnv    password not valid
#                    -av     account valid
#                    -anv    account not valid
#        DEFAULTS:   -av -pv
#                    account_expiration_date = none
#                    homedir = /


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
echo "Begin delete object tests"
echo


    rgy.sh $DEBUG $SAVE "RD1" "Deleting Account: bullwinkle,rom,jwe" \
           "do acc" \
           "del -p rocky"
    EXIT=$?

    rgy.sh $DEBUG $SAVE "RD2" "Deleting Person: rocky" \
           "do principal" \
           "del rocky"
    EXIT=$?

    rgy.sh $DEBUG $SAVE "RD2" "Deleting Person Alias: bambam" \
           "do principal" \
           "del bambam"
    EXIT=$?

    rgy.sh $DEBUG $SAVE "RD3" "Deleting Group: rom" \
           "do group" \
           "del rom"
    EXIT=$?

    rgy.sh $DEBUG $SAVE "RD4" "Deleting Org: jwe" \
           "do org" \
           "del jwe"
    EXIT=$?

echo
echo "End delete object tests"
echo

exit $EXIT

#
# End script rgy_delete.sh
#
