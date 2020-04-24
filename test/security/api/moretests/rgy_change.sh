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
# $Log: rgy_change.sh,v $
# Revision 1.1.4.2  1996/03/11  02:35:12  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:52  marty]
#
# Revision 1.1.4.1  1995/12/11  21:49:41  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/04/04  16:54 UTC  mdf
# 	For mothra as well.
# 	[1995/12/11  20:52:45  root]
# 
# Revision 1.1.2.2  1992/12/31  17:31:49  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:57:07  htf]
# 
# Revision 1.1  1992/01/19  04:08:16  devrcs
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
# src/test/security/api/moretests/rgy_change.sh, , dce_osf, dce.75d  
#   
#	Extracted	1/4/92 17:02:20
#	from delta on	12/2/91 15:49:10
#
#
# rgy_edit add command line test script
#
#


# Usage:
# c[hange] [principal_name [new_fields] [-al|-pr]]  |
#          [group_name  [new_fields] [-nl|-l]] |
#          [org_name    [new_fields]]
#      new_fields may include:
#          [-n new_name] [-f new_fullname]
#   
# c[hange]  [-p pname] [-g gname] [-o oname] [-np new_pname]
#                      [-ng new_group] [-no new_org]
#                      [[-rp | -pw password] -mp password] [-m misc_info]
#                      [-h homedir] [-s shell] [-pnv|-pv]
#                      [-x account_exp_date|none] [-anv|-av]]
#         OPTIONS:    -p      principal name
#                     -g      group name
#                     -o      org name
#                     -np     new principal name
#                     -ng     new group name
#                     -no     new org name
#                     -mp     my-password, for key validation
#                     -pw     new password for account
#                     -rp     generate random password
#                     -pv     password valid
#                     -pnv    password not valid
#                     -av     account valid
#                     -anv    account not valid
#         DEFAULTS:   Those fields that aren't explicitly changed
#                     will retain their previous values.
#         NOTES:      If any options are specified on the command line,
#                     rgy_edit will not prompt for fields, so all the
#                     required options would also have to be specified.
#                     To use interactive mode, do not specify any options.
#         REQUIRED:   If -pw or -rp are specified, then -mp must also be
#                     specified, to ensure the callers identity.


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
echo "Begin rgy_edit change object tests"
echo


    rgy.sh $DEBUG $SAVE \
           -v "barney *1002" \
           "RC1" "Changing Person: rocky -> barney" \
           "do principal" \
           "ch rocky  -n barney" \
           "v"
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Full name: *Barney A. Rubble" \
           "RC2" "Changing Person fullname: Rocket J. Squirrel -> Barney A. Rubble" \
           "do principal" \
           "ch barney  -f \"Barney A. Rubble\"" \
           "v barney -f"
    EXIT=$?


    rgy.sh $DEBUG $SAVE \
           -v "rocky *1002" \
           "RC3" "Restoring Person: barney -> rocky" \
           "do principal" \
           "ch barney -n rocky" \
           "v"
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Full name: *Rocket J. Squirrel" \
           "RC4" "Restoring Person fullname: Barney A. Rubble -> Rocket J. Squirrel " \
           "do principal" \
           "ch rocky  -f \"Rocket J. Squirrel\"" \
           "v -f"
    EXIT=$?



    rgy.sh $DEBUG $SAVE \
           -v "prehistory *2002" \
           "RC5" "Changing Group: rom -> prehistory" \
           "do group" \
           "ch rom  -n prehistory" \
           "v"
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Full name: *Prehistoric Preservation Society" \
           "RC6" "Changing Group fullname: Royal order of Mooses -> Prehistoric Preservation Society" \
           "do group" \
           "ch prehistory  -f \"Prehistoric Preservation Society\"" \
           "v -f"
    EXIT=$?


    rgy.sh $DEBUG $SAVE \
           -v "rom *2002" \
           "RC7" "Restoring Group: prehistory -> rom" \
           "do group" \
           "ch prehistory -n rom" \
           "v"
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Full name: *Royal order of Mooses" \
           "RC8" "Restoring Group fullname: Prehistoric Preservation Society -> Royal order of Mooses " \
           "do group" \
           "ch rom  -f \"Royal order of Mooses\"" \
           "v -f"
    EXIT=$?


    rgy.sh $DEBUG $SAVE \
           -v "hb *3002" \
           "RC9" "Changing Organization: jwe -> hb" \
           "do org" \
           "ch jwe  -n hb" \
           "v"
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Full name: *Hanna Barbera" \
           "RC10" "Changing Organization fullname: Jay Ward Enterprises -> Hanna Barbera" \
           "do org" \
           "ch hb  -f \"Hanna Barbera\"" \
           "v -f"
    EXIT=$?


    rgy.sh $DEBUG $SAVE \
           -v "jwe *3002" \
           "RC11" "Restoring Organization: hb -> jwe" \
           "do org" \
           "ch hb -n jwe" \
           "v"
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "Full name: *Jay Ward Enterprises" \
           "RC12" "Restoring Organization fullname: Hanna Barbera -> Jay Ward Enterprises " \
           "do org" \
           "ch jwe  -f \"Jay Ward Enterprises\"" \
           "v -f"
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "rocky \\[none jwe\\]:.*:1002:12:This is a test:/::" \
           "RC13" "Changing Account Group: rom -> none" \
           "do account" \
           "ch -p rocky -g rom -o jwe -ng none" \
           "v "
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "rocky \\[rom jwe\\]:.*:1002:2002:This is a test:/::" \
           "RC14" "Restoring Account Group: none -> rom" \
           "do account" \
           "ch -p rocky -g none -o jwe -ng rom" \
           "v "
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "rocky \\[rom none\\]:.*:1002:2002:This is a test:/::" \
           "RC15" "Changing Account Organization: jwe -> none" \
           "do account" \
           "ch -p rocky -g rom -o jwe -no none" \
           "v "
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "rocky \\[rom jwe\\]:.*:1002:2002:This is a test:/::" \
           "RC16" "Restoring Account Organization: none -> jwe" \
           "do account" \
           "ch -p rocky -g rom -o none -no jwe" \
           "v "
    EXIT=$?


    rgy.sh $DEBUG $SAVE \
           -v "rocky \\[rom jwe\\]:.*:1002:2002:This is a test:/::" \
           "RC17" "Changing Password: mooseandsquirrel -> borisdahlink" \
           "do account" \
           "ch -p rocky -g rom -o jwe -mp -dce- -pw borisdahlink" \
           "v rocky"
    EXIT=$?

    rgy.sh $DEBUG $SAVE \
           -v "rocky \\[rom jwe\\]:.*:1002:2002:This is a test:/::" \
           "RC18" "Restoring Password: borisdahlink -> mooseandsquirrel" \
           "do account" \
           "ch -p rocky -g rom -o jwe -mp -dce- -pw mooseandsquirrel " \
           "v rocky"
    EXIT=$?


    rgy.sh $DEBUG $SAVE \
           -v "Account is: NOT valid" \
           "RC19" "Setting account invalid" \
           "do account" \
           "ch -p rocky -g rom -o jwe -anv" \
           "v rocky -f"
    EXIT=$?


    rgy.sh $DEBUG $SAVE \
           -v "Account is: valid" \
           "RC20" "Restoring account to valid" \
           "do account" \
           "ch -p rocky -g rom -o jwe -av" \
           "v rocky -f"
    EXIT=$?


    rgy.sh $DEBUG $SAVE \
           -v "password is: NOT valid" \
           "RC19" "Setting password invalid" \
           "do account" \
           "ch -p rocky -g rom -o jwe -pnv" \
           "v rocky -f"
    EXIT=$?


    rgy.sh $DEBUG $SAVE \
           -v "password is: valid, was last changed:" \
           "RC20" "restoring password to valid" \
           "do account" \
           "ch -p rocky -g rom -o jwe -pv" \
           "v rocky -f"
    EXIT=$?

echo
echo "Begin rgy_edit change object tests"
echo

exit $EXIT

#
# End script rgy_change.sh
#
