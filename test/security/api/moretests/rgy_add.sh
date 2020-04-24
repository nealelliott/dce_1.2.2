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
# $Log: rgy_add.sh,v $
# Revision 1.1.4.2  1996/03/11  02:35:11  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:51  marty]
#
# Revision 1.1.4.1  1995/12/11  21:49:39  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:44  root]
# 
# Revision 1.1.2.2  1992/12/31  17:31:44  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:57:03  htf]
# 
# Revision 1.1  1992/01/19  04:08:15  devrcs
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
# src/test/security/api/moretests/rgy_add.sh, , dce_osf, dce.75d  
#   
#	Extracted	1/4/92 17:02:19
#	from delta on	12/2/91 15:49:06
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
echo "Begin rgy_edit add object tests"
echo

    rgy.sh $DEBUG $SAVE \
           -v "rocky *1002" \
           "RA1" "Adding Person: rocky" \
           "do principal" \
           "a rocky  1002 -f \"Rocket J. Squirrel\"" \
           "v"
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "rom *2002" \
           "RA2" "Adding Group: rom" \
           "do group" \
           "a rom 2002 -f \"Royal order of Mooses\"" \
           "v"
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "jwe *3002" \
           "RA3" "Adding Org: jwe" \
           "do org" \
           "a jwe 3002 -f \"Jay Ward Enterprises\"" \
           "v"
    EXIT=$?

   rgy.sh $DEBUG $SAVE \
           -v "rocky \[rom jwe\]" \
           "RA4" "Adding Account: rocky,rom,jwe" \
           "do acc" \
           "a rocky -g rom -o jwe -mp -dce- -pw mooseandsquirrel -m \"This is a test\"" \
           "v"
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "bambam *1002" \
           "RA5" "Adding Person Alias: bambam" \
           "do principal" \
           "a bambam  1002 -al -f \"cute kid\"" \
           "v"
    EXIT=$?

echo
echo "End rgy_edit add object tests"
echo


exit $EXIT

#
# End script rgy_add.sh
#
