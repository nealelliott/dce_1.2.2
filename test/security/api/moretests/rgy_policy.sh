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
# $Log: rgy_policy.sh,v $
# Revision 1.1.4.2  1996/03/11  02:35:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:02  marty]
#
# Revision 1.1.4.1  1995/12/11  21:49:56  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:53  root]
# 
# Revision 1.1.2.2  1992/12/31  17:32:38  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:57:55  htf]
# 
# Revision 1.1  1992/01/19  04:08:24  devrcs
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
# src/test/security/api/moretests/rgy_policy.sh, , dce_osf, dce.75d  
#   
#	Extracted	1/4/92 17:02:21
#	from delta on	12/2/91 15:49:30
#
#
# 
# Display/Modify the registry wide authorization policy
# Usage:
# au[thpolicy]
# 
# Usage:
# ce[ll] cellname [-ul unix_num] [-uf unix_num] [-gl gname] [-ol oname]
#                 [-gf gname] [-of oname] [-mp passwd]
#                 [-fa name] [-fp passwd]
#                 [-m misc_info] [-x account_expiration_date]
# 
#         OPTIONS:    -ul Unix number for the locally stored cell principal
#                     -uf Unix number for cell principal stored in foreign cell
#                     -gl group name for cell account stored in local cell
#                     -ol org name for cell account stored in local cell
#                     -gf group name for cell account stored in foreign cell
#                     -of org name for cell account stored in foreign cell
#                     -mp invoker's password
#                     -fa foreign cell administrator's name
#                     -fp foreign cell administrator's password
# 
#         DEFAULTS:   quota = 0; account_expiration_date = none
#                     unix_num = auto generate
# 
# All arguments that do not have default values are required.
# If any are missing, interactive mode is entered.
# 
# 
# Usage:
# pol[icy] [[org_name] [-al acct_lifespan] [-pl passwd_lifespan] [-pm passwd_min_len]
#                      [-px passwd_exp_date]|none] [-pa|-pna] [-ps-pns]]


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
echo "Begin policy function tests"
echo


    rgy.sh $DEBUG $SAVE \
           -v "Account lifespan: *2w6d" \
           "RP1" "Default Policy Test. Changing acct lifespan: forever -> 20 days " \
           "pol -al 20" \
           "po" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Account lifespan: *forever" \
           "RP2" "Default Policy Test. Restoring  acct lifespan: 20 days -> forever" \
           "pol -al forever" \
           "po" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Password lifespan: *2w6d" \
           "RP3" "Default Policy Test. Changing passwd lifespan: forever -> 20 days " \
           "pol -pl 20" \
           "po" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Password lifespan: *forever" \
           "RP4" "Default Policy Test. Restoring  passwd lifespan: 20 days -> forever" \
           "pol -pl forever" \
           "po" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Password min len: *20" \
           "RP5" "Default Policy Test. Changing passwd min len: 0 -> 20" \
           "pol -pm 20" \
           "po" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Password min len: *0" \
           "RP6" "Default Policy Test. Restoring  passwd min len: 20 -> 0" \
           "pol -pm 0" \
           "po" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Passwords MAY NOT be all spaces, *MAY NOT be all alphanumeric" \
           "RP7" "Default Policy Test. Changing passwd allowable characters: -pna -pns" \
           "pol -pna -pns" \
           "po" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Passwords MAY be all spaces, *MAY be all alphanumeric" \
           "RP8" "Default Policy Test. Restoring passwd allowable characters: -pa -ps" \
           "pol -pa -ps" \
           "po" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Account lifespan: *2w6d" \
           "RP9" "Policy Test on jwe. Changing acct lifespan: forever -> 20 days " \
           "pol jwe -al 20" \
           "pol jwe" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Account lifespan: *forever" \
           "RP10" "Policy Test on jwe. Restoring  acct lifespan: 20 days -> forever" \
           "pol jwe -al forever" \
           "pol jwe" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Password lifespan: *2w6d" \
           "RP11" "Policy Test on jwe. Changing passwd lifespan: forever -> 20 days " \
           "pol jwe -pl 20" \
           "pol jwe" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Password lifespan: *forever" \
           "RP12" "Policy Test on jwe. Restoring  passwd lifespan: 20 days -> forever" \
           "pol jwe -pl forever" \
           "pol jwe" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Password min len: *20" \
           "RP13" "Policy Test on jwe. Changing passwd min len: 0 -> 20" \
           "pol jwe -pm 20" \
           "pol jwe" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Password min len: *0" \
           "RP14" "Policy Test on jwe. Restoring  passwd min len: 20 -> 0" \
           "pol jwe -pm 0" \
           "pol jwe" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Passwords MAY NOT be all spaces, *MAY NOT be all alphanumeric" \
           "RP15" "Policy Test on jwe. Changing passwd allowable characters: -pna -pns" \
           "pol jwe -pna -pns" \
           "pol jwe" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Passwords MAY be all spaces, *MAY be all alphanumeric" \
           "RP16" "Policy Test on jwe. Restoring passwd allowable characters: -pa -ps" \
           "pol jwe -pa -ps" \
           "pol jwe" \
           "n" 
    EXIT=$?

echo
echo "End policy function tests"
echo

exit $EXIT

#
# End script policy.sh
#
