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
# $Log: rgy_misc.sh,v $
# Revision 1.1.6.2  1996/03/11  02:35:26  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:00  marty]
#
# Revision 1.1.6.1  1995/12/11  21:49:52  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:51  root]
# 
# Revision 1.1.4.3  1992/12/31  17:32:23  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:57:42  htf]
# 
# Revision 1.1.4.2  1992/11/05  12:07:01  sekhar
# 	[OT 4827]  Fixed the scope cmd test (scope cmd is only valid within
# 	           principal domain).
# 	[1992/11/04  20:18:58  sekhar]
# 
# Revision 1.1.2.3  1992/05/20  22:44:56  sekhar
# 	Added comments for clarification.
# 	[1992/05/20  22:44:30  sekhar]
# 
# Revision 1.1.2.2  1992/03/27  21:00:09  bmw
# 	Modified test cases that were incorrect.
# 	[1992/03/27  20:52:15  bmw]
# 
# Revision 1.1  1992/01/19  04:08:21  devrcs
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
# src/test/security/api/moretests/rgy_misc.sh, , dce_osf, dce.75d  
#   
#	Extracted	1/4/92 17:02:21
#	from delta on	12/2/91 15:49:22
#
#
# rgy_edit add command line test script
#
#

# ado[pt] uuid-string principal_name [-u unix_num] [-f fullname] [-q quota] |
#                     group_name  [-u unix_num] [-f fullname] [-nl] |
#                     org_name    [-u unix_num] [-f fullname]
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
# Usage:
# def[aults]
# 
# Usage:
# do[main] [ p[rincipal] | g[roup] | o[rg] | a[ccount] ]
# 
# Add to Server Key Table
# Usage:
# kta[dd] -p <principal> [-r[egistry][-a]] [-f keyfile] [-pw password]
#     -r[egistry]     Update the key in the registry also.
#     -f keyfile      Optional keytable file name.
# 
# Delete from Server Key Table
# Usage:
# ktd[elete] -p <principal> -v <version-number> [-f keyfile]
#     -v version-num  Version number of the key to delete.
#     -f keyfile      Optional keytable file name.
# 
# List Server Key Table
# Usage:
# ktl[ist] [-p <principal>] [-f keyfile]
#     -p principal    Optional principal to list keys for.
#     -f keyfile      Optional keytable file name.
# 
# Usage:
# m[ember] [group_name|org_name [-a member_list] [-r member_list]]
# 
# Usage:
# l[ogin]  [user_name]
# 
# Usage:
# pol[icy] [[org_name] [-al acct_lifespan] [-pl passwd_lifespan] [-pm passwd_min_len]
#                      [-px passwd_exp_date]|none] [-pa|-pna] [-ps-pns]]
# 
# Usage:
# prop[erties]
# 
# Usage:
# sc[ope] [name]
# 
# Usage:
# si[te]  [[address] | -u[pdate] | -q[uery]]


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
echo "Begin rgy_edit miscellaneous function tests"
echo


    rgy.sh $DEBUG $SAVE \
           -v "Authorization Policy:" \
           "RM1" "Testing authpolicy function" \
           "au" \
           "n" 
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Account entries:" \
           "RM2" "Testing default function" \
           "def" \
           "n" 
    EXIT=$?

   rgy.sh $DEBUG $SAVE \
           -v "Properties for Registry at:" \
           "RM3" "Testing properties function" \
           "prop" \
           "n" 
    EXIT=$?

   rgy.sh $DEBUG $SAVE \
           -v "Properties for Registry at:" \
           "RM4" "Testing properties function" \
           "prop" \
           "n" 
    EXIT=$?

   rgy.sh $DEBUG $SAVE \
           -v "Scope for principal domain is rocky" \
           "RM5" "Testing scope function - principal domain" \
           "domain principal" \
           "scope rocky"  \
           "scope"
    EXIT=$?


    rgy.sh $DEBUG $SAVE \
           -v "Scope not set" \
           "RM6" "Testing scope function - account domain" \
           "scope rocky" \
           "scope"
    EXIT=$?

   rgy.sh $DEBUG $SAVE \
           -v "Current site is: registry server at" \
           "RM7" "Testing site display function" \
           "si"
    EXIT=$?

   rgy.sh $DEBUG $SAVE \
           -v "Current site is: registry server at" \
           "RM8" "Testing site update function" \
           "si -u"
    EXIT=$?

   rgy.sh $DEBUG $SAVE \
           -v "Current site is: registry server at" \
           "RM9" "Testing site query function" \
           "si -q"
    EXIT=$?


#
# Note: You must be logged in as a princicpal with registry modifying priveleges
#       for these to work.( e.g. if dce_config was used to configure the cell,
#       you must be logged in as whatever the celladmin variable in dce_config was 
#       set to at the time of configuration of the cell). 
#

    rgy.sh $DEBUG $SAVE \
           "RM SETUP" "Adding Principal and Account: rocky" \
           "do principal" \
           "a rocky" \
           "do acc" \
           'a rocky -g none -o none -pw rocky -mp -dce-'
    EXIT=$?

   rgy.sh $DEBUG $SAVE \
           -v "/rocky" \
           "RM10" "Testing ktadd/ktlist function" \
           "ktadd -p rocky -pw wowbullwinkle" \
           "ktl"
    EXIT=$?

   rgy.sh $DEBUG $SAVE \
           -vn "/rocky *1" \
           "RM11" "Testing ktdelete function" \
           "ktdel -p rocky -v 1" \
           "ktl"
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           "RM CLEANUP" "Deleting Principal and Account: rocky" \
           "do acc" \
           "del -p rocky" \
           "do principal" \
           "del rocky"
    EXIT=$?


echo
echo "End rgy_edit miscellaneous function tests"
echo

exit $EXIT

#
# End script rgy_misc.sh
#



