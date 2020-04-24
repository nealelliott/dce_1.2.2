#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: account_modify_N.tcl,v $
# Revision 1.1.6.3  1996/11/14  16:09:22  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  14:25:22  arvind]
#
# Revision /main/DCE_1.2.2/3  1996/10/21  18:19 UTC  truitt
# 	Merge test changes.
# 
# Revision /main/DCE_1.2.2/truitt_dcecp_pk7/1  1996/10/15  16:32 UTC  truitt
# 	CHFts20138: Add the ability to have pk passwords prompted for as opposed
# 	to forcing the user to enter them on the command line.
# 
# Revision /main/DCE_1.2.2/2  1996/08/20  16:38 UTC  truitt
# 	Merge test changes.
# 
# Revision /main/DCE_1.2.2/truitt_dcecp_pk2/1  1996/08/20  16:36 UTC  truitt
# 	Update pk tests.
# 
# Revision /main/DCE_1.2.2/1  1996/07/19  17:59 UTC  gilda
# 	account_modify_N.tcl is modified to meet the pk login needs.
# 
# Revision /main/gilda_pk_dcecp/1  1996/07/19  17:02 UTC  gilda
# 	dcecp work for Public key login
# 
# Revision 1.1.4.3  1996/03/11  02:18:46  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:01  marty]
# 
# Revision 1.1.4.2  1995/12/13  15:27:15  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:13  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:31  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:45  melman]
# 
# Revision 1.1.2.5  1994/09/01  17:49:23  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:36  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:05  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:03  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:38:51  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:23:30  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:36  melman
# 	updating to new API path
# 	[1994/05/06  17:14:01  melman]
# 
# Revision 1.1.2.1  1994/04/15  19:24:23  kevins
# 	CR 9938 add account functional tests
# 	[1994/04/15  15:31:21  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

## Test purposes
proc account_modify_1_N {} {
    # Identify the test
    tet_infoline account_modify_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify kevin bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc account_modify_2_N {} {
    # Identify the test
    tet_infoline account_modify_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify} msg] $msg
    } compare_strings {1 {No account name.}}

    # Cleanup steps
}

proc account_modify_3_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_3_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -group none -password foo -mypwd $DCP_CLIENT_PW} msg] $msg
    } compare_strings {1 {User is not authorized to update record}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_4_N {} {
    # Identify the test
    tet_infoline account_modify_4_N: Check that the '-pkmechanism' option has a character string value.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pkmechanism} msg] $msg
    } compare_strings {1 {The '-pkmechanism' option requires a character string value.}}

    # Cleanup steps
}

proc account_modify_5_N {} {
    # Identify the test
    tet_infoline account_modify_5_N: Check that the attribute list syntax is invalid.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {pkmechanism}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc account_modify_6_N {} {
    # Identify the test
    tet_infoline account_modify_6_N: Check that the '-pksignatureusage' option has a character string value.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature} msg] $msg
    } compare_strings {1 {The '-pksignatureusage' option requires a character string value.}}

    # Cleanup steps
}

proc account_modify_7_N {} {
    # Identify the test
    tet_infoline account_modify_7_N: Check that the attribute list syntax is invalid.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {pokey}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc account_modify_8_N {} {
    # Identify the test
    tet_infoline account_modify_8_N: Check that the attribute list syntax is invalid.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {generatekey}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc account_modify_9_N {} {
    # Identify the test
    tet_infoline account_modify_9_N: Check that the attribute list syntax is invalid.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {pksignature {privatekeyfile}}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc account_modify_10_N {} {
    # Identify the test
    tet_infoline account_modify_10_N: Check that only options or an attribute list are included.  

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pkmechanism file -change {pksignature {gen 0}}} msg] $msg
    } compare_strings {1 {Only options or an attribute list are allowed.}}

    # Cleanup steps
}

proc account_modify_11_N {} {
    # Identify the test
    tet_infoline account_modify_11_N: Check for unrecognized argument 'public goober'

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {new gumby} {public goober}} msg] $msg
    } compare_strings {1 {Unrecognized argument 'public goober'.}}

    # Cleanup steps
}

proc account_modify_12_N {} {
    # Identify the test
    tet_infoline account_modify_12_N: Check that supplied attribute 'badattr'.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{generate 101} {newpass blah} {badattr gumby}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'badattr' is unknown.}}

    # Cleanup steps
}

proc account_modify_13_N {} {
    # Identify the test
    tet_infoline account_modify_13_N: Check that supplied attribute 'badattrib'.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {{pksignature {{public gumby} {badattrib pokey}}} \
                     {group none} {acctvalid no}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'badattrib' is unknown.}}

    # Cleanup steps
}

proc account_modify_14_N {} {
    # Identify the test
    tet_infoline account_modify_14_N: Check that 

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{generate pokey} {newpass blah}}} msg] $msg
    } compare_strings {1 {Supplied value for 'generatekey' must be zero, a positive integer or 'default'.}}

    # Cleanup steps
}

proc account_modify_15_N {} {
    # Identify the test
    tet_infoline account_modify_15_N: Check the supplied value for 'generatekey'.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {pksignature {{generate pokey} {newpass blah}}}} msg] $msg
    } compare_strings {1 {Supplied value for 'generatekey' must be zero, a positive integer or 'default'.}}

    # Cleanup steps
}

proc account_modify_16_N {} {
    # Identify the test
    tet_infoline account_modify_16_N: Check the supplied value for 'generatekey'.
    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {generate -100}} msg] $msg
    } compare_strings {1 {Supplied value for 'generatekey' must not be a negative integer.}}

    # Cleanup steps
}

proc account_modify_17_N {} {
    # Identify the test
    tet_infoline account_modify_17_N: Check the supplied value for 'generatekey'.
    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {pksignature {generate -100}}} msg] $msg
    } compare_strings {1 {Supplied value for 'generatekey' must not be a negative integer.}}

    # Cleanup steps
}

proc account_modify_18_N {} {
    # Identify the test
    tet_infoline account_modify_18_N: Check the supplied value for key generation attribute. 
    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {generate 999999999999}} msg] $msg
    } compare_strings {1 {Supplied value for key generation attribute must be less than 2147483647.}}

    # Cleanup steps
}

proc account_modify_19_N {} {
    # Identify the test
    tet_infoline account_modify_19_N: Check the supplied value for key generation attribute. 

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {pksignature {generatekey 999999999999}}} msg] $msg
    } compare_strings {1 {Supplied value for key generation attribute must be less than 2147483647.}}

    # Cleanup steps
}

proc account_modify_20_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_20_N: Check the value for 'pkgenprivkey'. 

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -pkgenprivkey pokey} msg] $msg
    } compare_strings {1 {Supplied value for 'pkgenprivkey' must be zero, a positive integer or 'default'.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_21_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_21_N: Check the value for 'pkgenprivkey'.  

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
        -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -change {pkgenprivkey pokey}} msg] $msg
    } compare_strings {1 {Supplied value for 'pkgenprivkey' must be zero, a positive integer or 'default'.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_22_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_22_N: Check that supplied value for 'pkgenprivkey'.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -pkgenprivkey -100} msg] $msg
    } compare_strings {1 {Supplied value for 'pkgenprivkey' must not be a negative integer.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_23_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_23_N: Check that supplied value for 'pkgenprivkey'.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -change {pkgenprivkey -100}} msg] $msg
    } compare_strings {1 {Supplied value for 'pkgenprivkey' must not be a negative integer.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_24_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_24_N: Check that supplied value for key generation attribute is less than 2147483647.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -pkgenprivkey 99999999999} msg] $msg
    } compare_strings {1 {Supplied value for key generation attribute must be less than 2147483647.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_25_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_25_N: Check that supplied value for key generation attribute is less than 2147483647.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -change {pkgenprivkey 99999999999}} msg] $msg
    } compare_strings {1 {Supplied value for key generation attribute must be less than 2147483647.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_26_N {} {
    # Identify the test
    tet_infoline account_modify_26_N: Check that a password is not specified when 'pkgenprivkey' is also specified. 

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -password pokey -pkgenprivkey 101} msg] $msg
    } compare_strings {1 {Cannot specify a password when 'pkgenprivkey' is also specified.}}

    # Cleanup steps
}

proc account_modify_27_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_27_N: Check that a password is not specified when 'pkgenprivkey' is also specified.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -password pokey -pkgenprivkey default} msg] $msg
    } compare_strings {1 {Cannot specify a password when 'pkgenprivkey' is also specified.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_28_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_28_N: Check that a password  is not specified when 'pkgenprivkey' is also specified.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password pokey -mypwd -dce-}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -password pokey \
                     -mypwd -dce- -pkgenprivkey 101} msg] $msg
    } compare_strings {1 {Cannot specify a password when 'pkgenprivkey' is also specified.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_29_N {} {
    # Identify the test
    tet_infoline account_modify_29_N: Check that a password is not specified when 'pkgenprivkey' is also specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {{password pokey} {pkgenprivkey 101}}} msg] $msg
    } compare_strings {1 {Cannot specify a password when 'pkgenprivkey' is also specified.}}

    # Cleanup steps
}

proc account_modify_30_N {} {
    # Identify the test
    tet_infoline account_modify_30_N: Check that a password is not specified when 'pkgenprivkey' is also specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {{password pokey} {pkgenprivkey default}}} msg] $msg
    } compare_strings {1 {Cannot specify a password when 'pkgenprivkey' is also specified.}}

    # Cleanup steps
}

proc account_modify_31_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_31_N: Check that a password is not specified when 'pkgenprivkey' is also specified.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -change {{password pokey} {mypwd -dce-} {pkgenprivkey 101}}} msg] $msg
    } compare_strings {1 {Cannot specify a password when 'pkgenprivkey' is also specified.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_32_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_32_N: Check that the value for 'pkmechanism' is either 'file' or 'pkss'.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -pkmechanism pokey} msg] $msg
    } compare_strings {1 {The value for 'pkmechanism' must be either 'file' or 'pkss'.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_33_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_33_N: Check that the value for 'pkmechanism' is either 'file' or 'pkss'. 

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -change {pkmechanism pokey}} msg] $msg
    } compare_strings {1 {The value for 'pkmechanism' must be either 'file' or 'pkss'.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_34_N {} {
    # Identify the test
    tet_infoline account_modify_34_N: Check that supplied attribute 'generatekey' is not specified more than once.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{generate 0} {generate 1024}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'generatekey' was specified more than once.}}

    # Cleanup steps
}

proc account_modify_35_N {} {
    # Identify the test
    tet_infoline account_modify_35_N: Check that supplied attribute 'privatekeyfile' was not specified more than once. 

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{private pokey} {private pokey2}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'privatekeyfile' was specified more than once.}}

    # Cleanup steps
}

proc account_modify_36_N {} {
    # Identify the test
    tet_infoline account_modify_36_N: Check that supplied attribute 'publickeyfile' was specified more than once.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{publickey gumby} {publickey pokey}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'publickeyfile' was specified more than once.}}

    # Cleanup steps
}

proc account_modify_37_N {} {
    # Identify the test
    tet_infoline account_modify_37_N: Check that supplied attribute 'oldpassphrase' was specified more than once. 

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{oldpassphrase blah} {oldpassphrase blah2}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'oldpassphrase' was specified more than once.}}

    # Cleanup steps
}

proc account_modify_38_N {} {
    # Identify the test
    tet_infoline account_modify_38_N: Check that supplied attribute 'newpassphrase' was specified more than once. 

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{new {long string}} {new shortstring}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'newpassphrase' was specified more than once.}}

    # Cleanup steps
}

proc account_modify_39_N {} {
    # Identify the test
    tet_infoline account_modify_39_N: Check that supplied attribute 'publickeyfile' was specified more than once. 

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {{pksignature {{public gumby} {public pokey}}} {acctvalid no}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'publickeyfile' was specified more than once.}}

    # Cleanup steps
}

proc account_modify_40_N {} {
    # Identify the test
    tet_infoline account_modify_40_N: Check that a public or private keyfile is not used when 'generatekey' is also used.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{generatekey 111} {public pokey}}} msg] $msg
    } compare_strings {1 {Cannot specify a public or private keyfile when 'generatekey' is also specified.}}

    # Cleanup steps
}

proc account_modify_41_N {} {
    # Identify the test
    tet_infoline account_modify_41_N: Check that a public or private keyfile is not specified when 'generatekey' is specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{gen default} {public /tmp/gumby1} {private /tmp/gumby2}}} msg] $msg
    } compare_strings {1 {Cannot specify a public or private keyfile when 'generatekey' is also specified.}}

    # Cleanup steps
}

proc account_modify_42_N {} {
    global TET_ROOT DCP_PK_DATA_1

    # Identify the test
    tet_infoline account_modify_42_N: Check that a public or private keyfile is specified when 'generatekey' is specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature "{gen 1024} {new pokey} {private $TET_ROOT/$DCP_PK_DATA_1}"} msg] $msg
    } compare_strings {1 {Cannot specify a public or private keyfile when 'generatekey' is also specified.}}

    # Cleanup steps
}

proc account_modify_43_N {} {
    # Identify the test
    tet_infoline account_modify_43_N: Check that a public or private keyfile is not specifoed when 'generatekey' is specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{gen default} {new gumby} {public gumby}}} msg] $msg
    } compare_strings {1 {Cannot specify a public or private keyfile when 'generatekey' is also specified.}}

    # Cleanup steps
}

proc account_modify_44_N {} {
    # Identify the test
    tet_infoline account_modify_44_N: Check that a public or private keyfile is not specified when 'generatekey' is specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{generatekey 101} {newpassphrase gumby_string} \
                     {publickeyfile gumby} {privatekeyfile pokey}}} msg] $msg
    } compare_strings {1 {Cannot specify a public or private keyfile when 'generatekey' is also specified.}}

    # Cleanup steps
}

proc account_modify_45_N {} {
    global TET_ROOT DCP_PK_DATA_1

    # Identify the test
    tet_infoline account_modify_45_N: Check that a public or private keyfile is not specified when 'generatekey' is specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change "pksignature {{gen 111} {public $TET_ROOT/$DCP_PK_DATA_1}}"} msg] $msg
    } compare_strings {1 {Cannot specify a public or private keyfile when 'generatekey' is also specified.}}

    # Cleanup steps
}

proc account_modify_46_N {} {
    # Identify the test
    tet_infoline account_modify_46_N: Check that a public or private keyfile is not specified when 'generatekey' is specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {pksignature {{gen default} {public /tmp/gumby1} \
                     {private /tmp/gumby2}}}} msg] $msg
    } compare_strings {1 {Cannot specify a public or private keyfile when 'generatekey' is also specified.}}

    # Cleanup steps
}

proc account_modify_47_N {} {
    global TET_ROOT DCP_PK_DATA_1

    # Identify the test
    tet_infoline account_modify_47_N: Check that a public or private keyfile is specified when 'generatekey' is specified.  

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change "pksignature {{gen 1024} {new pokey} \
                     {privatekeyfile $TET_ROOT/$DCP_PK_DATA_1}}"} msg] $msg
    } compare_strings {1 {Cannot specify a public or private keyfile when 'generatekey' is also specified.}}

    # Cleanup steps
}

proc account_modify_48_N {} {
    # Identify the test
    tet_infoline account_modify_48_N: Check that a public or private keyfile is not specified when 'generatekey' is specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {pksignature {{gen default} {new gumby} {publickeyfile gumby}}}} msg] $msg
    } compare_strings {1 {Cannot specify a public or private keyfile when 'generatekey' is also specified.}}

    # Cleanup steps
}

proc account_modify_49_N {} {
    global TET_ROOT DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_49_N: Check that a public or private keyfile is not specified when 'generatekey' is specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change "pksignature {{generatekey 1001} {new pokey} \
                     {publickeyfile $TET_ROOT/$DCP_PK_DATA_1} {privatekeyfile $TET_ROOT/$DCP_PK_DATA_2}}"} msg] $msg
    } compare_strings {1 {Cannot specify a public or private keyfile when 'generatekey' is also specified.}}

    # Cleanup steps
}

proc account_modify_50_N {} {
    # Identify the test
    tet_infoline account_modify_50_N: Check that attribute 'oldpassphrase' and/or 'newpassphrase' is entered.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {generatekey 101}} msg] $msg
    } compare_strings {1 {Attribute 'oldpassphrase' and/or 'newpassphrase' not entered.}}

    # Cleanup steps
}

proc account_modify_51_N {} {
    # Identify the test
    tet_infoline account_modify_51_N: Check that attribute 'oldpassphrase' and/or 'newpassphrase' is entered.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {generatekey def} -acctvalid no} msg] $msg
    } compare_strings {1 {Attribute 'oldpassphrase' and/or 'newpassphrase' not entered.}}

    # Cleanup steps
}

proc account_modify_52_N {} {
    # Identify the test
    tet_infoline account_modify_52_N: Check that attribute 'oldpassphrase' and/or 'newpassphrase' is entered.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {pksignature {generatekey 101}}} msg] $msg
    } compare_strings {1 {Attribute 'oldpassphrase' and/or 'newpassphrase' not entered.}}

    # Cleanup steps
}

proc account_modify_53_N {} {
    # Identify the test
    tet_infoline account_modify_53_N: Check that attribute 'oldpassphrase' and/or 'newpassphrase' not entered. 

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {{pksignature {gen default}} {acctvalid no}}} msg] $msg
    } compare_strings {1 {Attribute 'oldpassphrase' and/or 'newpassphrase' not entered.}}

    # Cleanup steps
}

proc account_modify_54_N {} {
    # Identify the test
    tet_infoline account_modify_54_N: Check that attribute 'oldpassphrase' and/or 'newpassphrase' is entered.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {pksignature {gen 1001} {public gumby}}} msg] $msg
    } compare_strings {1 {Attribute 'oldpassphrase' and/or 'newpassphrase' not entered.}}

    # Cleanup steps
}

proc account_modify_55_N {} {
    # Identify the test
    tet_infoline account_modify_55_N: Check that attribute 'oldpassphrase' and/or 'newpassphrase' is entered. 

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{generate 0} {public gumby1}}} msg] $msg
    } compare_strings {1 {Attribute 'oldpassphrase' and/or 'newpassphrase' not entered.}}

    # Cleanup steps
}

proc account_modify_56_N {} {
    global TET_ROOT DCP_PK_DATA_1

    # Identify the test
    tet_infoline account_modify_56_N: Check that attribute 'oldpassphrase' and/or 'newpassphrase' is entered. 

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature "publickey $TET_ROOT/$DCP_PK_DATA_1"} msg] $msg
    } compare_strings {1 {Attribute 'oldpassphrase' and/or 'newpassphrase' not entered.}}

    # Cleanup steps
}

proc account_modify_57_N {} {
    # Identify the test
    tet_infoline account_modify_57_N: Check that attribute 'oldpassphrase' and/or 'newpassphrase' is entered.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{public /tmp/gumby1} {private /tmp/gumby2}}} msg] $msg
    } compare_strings {1 {Attribute 'oldpassphrase' and/or 'newpassphrase' not entered.}}

    # Cleanup steps
}

proc account_modify_58_N {} {
    # Identify the test
    tet_infoline account_modify_58_N: Check that attribute 'oldpassphrase' and/or 'newpassphrase' is entered.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {pksignature {{gen 0} {public gumby1}}}} msg] $msg
    } compare_strings {1 {Attribute 'oldpassphrase' and/or 'newpassphrase' not entered.}}

    # Cleanup steps
}

proc account_modify_59_N {} {
    # Identify the test
    tet_infoline account_modify_59_N: Check that attribute 'oldpassphrase' and/or 'newpassphrase' is entered.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {pksignature {public gumby1}}} msg] $msg
    } compare_strings {1 {Attribute 'oldpassphrase' and/or 'newpassphrase' not entered.}}

    # Cleanup steps
}

proc account_modify_60_N {} {
    # Identify the test
    tet_infoline account_modify_60_N: Check that attribute 'oldpassphrase' and/or 'newpassphrase' is entered.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {pksignature {{public gumby1} {private pokey1}}}} msg] $msg
    } compare_strings {1 {Attribute 'oldpassphrase' and/or 'newpassphrase' not entered.}}

    # Cleanup steps
}

proc account_modify_61_N {} {
    # Identify the test
    tet_infoline account_modify_61_N: Check that both 'publickeyfile' and 'privatekeyfile' are specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{new gumby} {privatekey gumby1}}} msg] $msg
    } compare_strings {1 {Must specify both 'publickeyfile' and 'privatekeyfile'.}}

    # Cleanup steps
}

proc account_modify_62_N {} {
    # Identify the test
    tet_infoline account_modify_62_N: Check that no action is specified by the public key attribute list.

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -change {pksignature {gen 0}}} msg] $msg
    } compare_strings {1 {No action specified by the public key attribute list.}}

    # Cleanup steps
}

proc account_modify_63_N {} {
    # Identify the test
    tet_infoline account_modify_63_N: Check that both 'publickeyfile' and 'privatekeyfile' are specified. 

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{new gumby} {privatekey gumby1}}} msg] $msg
    } compare_strings {1 {Must specify both 'publickeyfile' and 'privatekeyfile'.}}

    # Cleanup steps
}

proc account_modify_64_N {} {
    # Identify the test
    tet_infoline account_modify_64_N: Check that both 'publickeyfile' and 'privatekeyfile' are specified. 

    # Setup steps

    # Run the test
    test {
        list [catch {account modify gumby -pksignature {{new gumby} {publickey pokey1}}} msg] $msg
    } compare_strings {1 {Must specify both 'publickeyfile' and 'privatekeyfile'.}}

    # Cleanup steps
}

proc account_modify_65_N {} {
    # Identify the test
    tet_infoline account_modify_65_N: Check that both 'publickeyfile' and 'privatekeyfile' are specified.

    # Setup steps

    # Run the test 
    test {
        list [catch {account modify gumby -change {pksignature {{new gumby} {privatekey goober}}}} msg] $msg
    } compare_strings {1 {Must specify both 'publickeyfile' and 'privatekeyfile'.}}

    # Cleanup steps
}

proc account_modify_66_N {} {
    # Identify the test
    tet_infoline account_modify_66_N: Check that both 'publickeyfile' and 'privatekeyfile' are specified.

    # Setup steps

    # Run the test 
    test {
        list [catch {account modify gumby -change {pksignature {{new gumby} {publickeyfile goober}}}} msg] $msg
    } compare_strings {1 {Must specify both 'publickeyfile' and 'privatekeyfile'.}}

    # Cleanup steps
}

proc account_modify_67_N {} {
    global TET_ROOT DCP_PK_DATA_1

    # Identify the test
    tet_infoline account_modify_67_N: Check that the same file is not used for public and private keys.

    # Setup steps

    # Run the test 
    test {
        list [catch {account modify gumby -pksignature "{newpass pokey} {public $TET_ROOT/$DCP_PK_DATA_1} \
                     {private $TET_ROOT/$DCP_PK_DATA_1}"} msg] $msg
    } compare_strings {1 {The same file cannot be used for public and private keys.}}

    # Cleanup steps
}

proc account_modify_68_N {} {
    global TET_ROOT DCP_PK_DATA_1

    # Identify the test
    tet_infoline account_modify_68_N: Check that the same file are not used for public and private keys.

    # Setup steps

    # Run the test 
    test {
        list [catch {account modify gumby -change "pksignature {{new pokey} {public $TET_ROOT/$DCP_PK_DATA_1} \
                     {private $TET_ROOT/$DCP_PK_DATA_1}}"} msg] $msg
    } compare_strings {1 {The same file cannot be used for public and private keys.}}

    # Cleanup steps
}

proc account_modify_69_N {} {
    # Identify the test
    tet_infoline account_modify_69_N: Check that file '/tmp/bad2' can be opened.

    # Setup steps

    # Run the test 
    test {
        list [catch {account modify gumby -pksignature {{gen 0} {new pokey} {privatekey /tmp/bad1} \
                     {public /tmp/bad2}}} msg] $msg
    } compare_strings {1 {Could not open file '/tmp/bad2'.}}

    # Cleanup steps
}

proc account_modify_70_N {} {
    global TET_ROOT DCP_PK_DATA_1

    # Identify the test
    tet_infoline account_modify_70_N: Check that file '/tmp/bad1' can not be opened. 

    # Setup steps

    # Run the test 
    test {
        list [catch {account modify gumby -change "pksignature {{gen 0} {newpass pokey} \
                     {privatekey /tmp/bad1} {publickeyfile $TET_ROOT/$DCP_PK_DATA_1}}"} msg] $msg
    } compare_strings {1 {Could not open file '/tmp/bad1'.}}

    # Cleanup steps
}

proc account_modify_71_N {} {
    global TET_ROOT DCP_PK_DATA_1 DCP_PK_DATA_3

    # Identify the test
    tet_infoline account_modify_71_N: Check that empty file is not readable. 

    # Setup steps

    # Run the test 
    test {
        list [catch {account modify gumby -pksignature "{new pokey} {public $TET_ROOT/$DCP_PK_DATA_1} \
                     {private $TET_ROOT/$DCP_PK_DATA_3}"} msg] $msg
    } compare_strings "1 {Could not read from file '$TET_ROOT/$DCP_PK_DATA_3'.}"

    # Cleanup steps
}

proc account_modify_72_N {} {
    global TET_ROOT DCP_PK_DATA_1 DCP_PK_DATA_3

    # Identify the test
    tet_infoline account_modify_72_N: Check that empty file is not readable. 

    # Setup steps

    # Run the test 
    test {
        list [catch {account modify gumby -change "pksignature {{new pokey} {public $TET_ROOT/$DCP_PK_DATA_1} \
                     {private $TET_ROOT/$DCP_PK_DATA_3}}"} msg] $msg
    } compare_strings "1 {Could not read from file '$TET_ROOT/$DCP_PK_DATA_3'.}"

    # Cleanup steps
}

proc account_modify_73_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_73_N: Use pkgenprivkey on an invalid account.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -pkgenprivkey 1024 -mypwd $DCP_CLIENT_PW} msg] $msg
    } compare_strings {1 {Only DES keys allowed for this account}}

    # Cleanup steps
    if {[dcp_wrap {account delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_74_N {} {
    global DCP_CLIENT_PW DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline account_modify_74_N: An unauthorized user tries to modify the account.

    # Setup steps
    set RESIDUAL_NAME [string range $DCP_CELLNAME_ONE 5 end]
    set PRINCIPAL $DCP_CELLNAME_ONE/krbtgt/$RESIDUAL_NAME

    # Run the test
    test {
        list [catch {account modify $PRINCIPAL -pkgenprivkey 868 -mypwd $DCP_CLIENT_PW} msg] $msg
    } compare_strings {1 {User is not authorized to update record}}

    # Cleanup steps
}

proc account_modify_75_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_75_N: Use an incorrect combination of 'newpassphrase'.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -pksignature {{gen 256} {newpass gumby}} \
                     -pkkeycipher {{gen 256} {newpass pokey}}} msg] $msg
    } compare_strings {1 {Passphrases for all the public keys must be the same.}}

    # Cleanup steps
    if {[dcp_wrap {account delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_76_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_76_N: Use an incorrect combination of 'newpassphrase'.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW \
                   -pksignature {{gen 256} {newpass gumby}}}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -pkkeycipher {{gen 256} {newpass pokey}}} msg] $msg
    } compare_strings {1 {New passphrase doesn't match the other key pair's passphrase}}

    # Cleanup steps
    if {[dcp_wrap {account delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_77_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_77_N: Use an incorrect combination of 'newpassphrase'.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW \
                   -pkkeycipher {{gen 256} {newpass gumby}}}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -pksignature {{gen 256} {newpass pokey}}} msg] $msg
    } compare_strings {1 {New passphrase doesn't match the other key pair's passphrase}}

    # Cleanup steps
    if {[dcp_wrap {account delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_modify_78_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_78_N: Use an incorrect 'oldpassphrase'.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
                   -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW \
                   -pkkeycipher {{gen 256} {newpass gumby}}}] != 0} {return;}

    # Run the test
    test {
        list [catch {account modify $account_name -pkkeycipher {{gen 512} {oldpass pokey}}} msg] $msg
    } compare_strings {1 {BSAFE2: decryption failure}}

    # Cleanup steps
    if {[dcp_wrap {account delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 ic38 ic39 ic40 ic41 ic42 ic43 ic44 ic45 ic46 ic47 ic48 ic49 ic50 ic51 ic52 ic53 ic54 ic55 ic56 ic57 ic58 ic59 ic60 ic61 ic62 ic63 ic64 ic65 ic66 ic67 ic68 ic69 ic70 ic71 ic72 ic73 ic74 ic75 ic76 ic77 ic78 "
set ic1 "account_modify_1_N"
set ic2 "account_modify_2_N"
set ic3 "account_modify_3_N"
set ic4 "account_modify_4_N"
set ic5 "account_modify_5_N"
set ic6 "account_modify_6_N"
set ic7 "account_modify_7_N"
set ic8 "account_modify_8_N"
set ic9 "account_modify_9_N"
set ic10 "account_modify_10_N"
set ic11 "account_modify_11_N"
set ic12 "account_modify_12_N"
set ic13 "account_modify_13_N"
set ic14 "account_modify_14_N"
set ic15 "account_modify_15_N"
set ic16 "account_modify_16_N"
set ic17 "account_modify_17_N"
set ic18 "account_modify_18_N"
set ic19 "account_modify_19_N"
set ic20 "account_modify_20_N"
set ic21 "account_modify_21_N"
set ic22 "account_modify_22_N"
set ic23 "account_modify_23_N"
set ic24 "account_modify_24_N"
set ic25 "account_modify_25_N"
set ic26 "account_modify_26_N"
set ic27 "account_modify_27_N"
set ic28 "account_modify_28_N"
set ic29 "account_modify_29_N"
set ic30 "account_modify_30_N"
set ic31 "account_modify_31_N"
set ic32 "account_modify_32_N"
set ic33 "account_modify_33_N"
set ic34 "account_modify_34_N"
set ic35 "account_modify_35_N"
set ic36 "account_modify_36_N"
set ic37 "account_modify_37_N"
set ic38 "account_modify_38_N"
set ic39 "account_modify_39_N"
set ic40 "account_modify_40_N"
set ic41 "account_modify_41_N"
set ic42 "account_modify_42_N"
set ic43 "account_modify_43_N"
set ic44 "account_modify_44_N"
set ic45 "account_modify_45_N"
set ic46 "account_modify_46_N"
set ic47 "account_modify_47_N"
set ic48 "account_modify_48_N"
set ic49 "account_modify_49_N"
set ic50 "account_modify_50_N"
set ic51 "account_modify_51_N"
set ic52 "account_modify_52_N"
set ic53 "account_modify_53_N"
set ic54 "account_modify_54_N"
set ic55 "account_modify_55_N"
set ic56 "account_modify_56_N"
set ic57 "account_modify_57_N"
set ic58 "account_modify_58_N"
set ic59 "account_modify_59_N"
set ic60 "account_modify_60_N"
set ic61 "account_modify_61_N"
set ic62 "account_modify_62_N"
set ic63 "account_modify_63_N"
set ic64 "account_modify_64_N"
set ic65 "account_modify_65_N"
set ic66 "account_modify_66_N"
set ic67 "account_modify_67_N"
set ic68 "account_modify_68_N"
set ic69 "account_modify_69_N"
set ic70 "account_modify_70_N"
set ic71 "account_modify_71_N"
set ic72 "account_modify_72_N"
set ic73 "account_modify_73_N"
set ic74 "account_modify_74_N"
set ic75 "account_modify_75_N"
set ic76 "account_modify_76_N"
set ic77 "account_modify_77_N"
set ic78 "account_modify_78_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
