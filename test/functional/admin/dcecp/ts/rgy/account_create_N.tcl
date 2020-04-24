#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: account_create_N.tcl,v $
# Revision 1.1.6.4  1996/11/14  16:08:46  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  14:24:53  arvind]
#
# Revision /main/DCE_1.2.2/4  1996/10/21  18:22 UTC  truitt
# 	Merge test changes.
# 
# Revision /main/DCE_1.2.2/truitt_dcecp_pk7/1  1996/10/15  16:30 UTC  truitt
# 	CHFts20138: Add the ability to have pk passwords prompted for as opposed
# 	to forcing the user to enter them on the command line.
# 
# Revision /main/DCE_1.2.2/3  1996/08/20  16:33 UTC  truitt
# 	Merge test changes.
# 
# Revision /main/DCE_1.2.2/truitt_dcecp_pk2/1  1996/08/20  16:32 UTC  truitt
# 	Update pk tests.
# 
# Revision /main/DCE_1.2.2/2  1996/07/19  16:53 UTC  gilda
# 	This dcecp test is modified to meet the pk login project needs.
# 
# Revision /main/DCE_1.2.2/gilda_pk_dcecp/1  1996/07/19  16:44 UTC  gilda
# 	dcecp work for Public key login
# 
# Revision /main/DCE_1.2.2/1  1996/05/24  23:32 UTC  sommerfeld
# 	merge u2u  changes.
# 	[1996/05/06  22:13 UTC  burati  /main/DCE_1.2/2]
# 
# 	u2u work off Mothra, before 1.2 branch is available
# 	[1996/04/29  22:06 UTC  burati  /main/HPDCE02/mb_mothra8/1]
# 
# Revision 1.1.4.3  1996/03/11  02:18:30  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:55  marty]
# 
# Revision 1.1.4.2  1995/12/13  15:27:08  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/06/08  13:40 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts33/1  1995/06/08  13:39 UTC  truitt
# 	CHFts15424: Change tests to reflect new dced acl behavior.
# 
# 	HP revision /main/HPDCE02/2  1995/03/09  14:14 UTC  truitt
# 	Test bogus -attr value.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts18/1  1995/03/09  14:13 UTC  truitt
# 	CHFts14579: Test that bogus values for the -attr option
# 	are properly handled.
# 
# 	HP revision /main/HPDCE02/1  1994/12/12  18:26 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_msg_chk/1  1994/12/12  18:23 UTC  truitt
# 	Fix incorrect error message.
# 	[1995/12/11  15:54:07  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:21  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:36  melman]
# 
# Revision 1.1.2.5  1994/09/01  17:49:19  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:30  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:01  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:58  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:38:47  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:23:06  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:32  melman
# 	updating to new API path
# 	[1994/05/06  17:13:55  melman]
# 
# Revision 1.1.2.1  1994/04/15  19:24:17  kevins
# 	CR 9938 add account functional tests
# 	[1994/04/15  15:31:17  kevins]
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
proc account_create_1_N {} {
    # Identify the test
    tet_infoline account_create_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {account create kevin bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc account_create_2_N {} {
    # Identify the test
    tet_infoline account_create_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create} msg] $msg
    } compare_strings {1 {No account name.}}

    # Cleanup steps
}

proc account_create_3_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_3_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create kevin}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {
        list [catch {account create kevin -group none -organization none -password mine -mypwd $DCP_CLIENT_PW} msg] $msg
    } compare_strings {1 {User is not authorized to update record}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete kevin}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_create_4_N {} {
    # Identify the test
    tet_infoline account_create_4_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -acctvalid} msg] $msg
    } compare_strings {1 {The '-acctvalid' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_5_N {} {
    # Identify the test
    tet_infoline account_create_5_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -client foo} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc account_create_6_N {} {
    # Identify the test
    tet_infoline account_create_6_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -acctvalid 45} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc account_create_7_N {} {
    # Identify the test
    tet_infoline account_create_7_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -dupkey 45} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc account_create_8_N {} {
    # Identify the test
    tet_infoline account_create_8_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -expdate 67} msg] $msg
    } compare_strings {1 {Bad time string supplied.}}

    # Cleanup steps
}

proc account_create_9_N {} {
    # Identify the test
    tet_infoline account_create_9_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -forwardabletkt 89} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc account_create_10_N {} {
    # Identify the test
    tet_infoline account_create_10_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -goodsince 67} msg] $msg
    } compare_strings {1 {Bad time string supplied.}}

    # Cleanup steps
}

proc account_create_11_N {} {
    # Identify the test
    tet_infoline account_create_11_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -group} msg] $msg
    } compare_strings {1 {The '-group' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_12_N {} {
    # Identify the test
    tet_infoline account_create_12_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -home} msg] $msg
    } compare_strings {1 {The '-home' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_13_N {} {
    # Identify the test
    tet_infoline account_create_13_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -organization} msg] $msg
    } compare_strings {1 {The '-organization' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_14_N {} {
    # Identify the test
    tet_infoline account_create_14_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -postdatedtkt 34} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc account_create_15_N {} {
    # Identify the test
    tet_infoline account_create_15_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -proxiabletkt 45} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc account_create_16_N {} {
    # Identify the test
    tet_infoline account_create_16_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -pwdvalid 34} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc account_create_17_N {} {
    # Identify the test
    tet_infoline account_create_17_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -renewabletkt 34} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc account_create_18_N {} {
    # Identify the test
    tet_infoline account_create_18_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -server} msg] $msg
    } compare_strings {1 {The '-server' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_19_N {} {
    # Identify the test
    tet_infoline account_create_19_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -shell} msg] $msg
    } compare_strings {1 {The '-shell' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_20_N {} {
    # Identify the test
    tet_infoline account_create_20_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -stdtgtauth 45} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc account_create_21_N {} {
    # Identify the test
    tet_infoline account_create_21_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -maxtktlife foo} msg] $msg
    } compare_strings {1 {Bad relative time string supplied.}}

    # Cleanup steps
}

proc account_create_22_N {} {
    # Identify the test
    tet_infoline account_create_22_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo  -maxtktrenew foo} msg] $msg
    } compare_strings {1 {Bad relative time string supplied.}}

    # Cleanup steps
}

proc account_create_23_N {} {
    # Identify the test
    tet_infoline account_create_23_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -acctvalid} msg] $msg
    } compare_strings {1 {The '-acctvalid' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_24_N {} {
    # Identify the test
    tet_infoline account_create_24_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -client} msg] $msg
    } compare_strings {1 {The '-client' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_25_N {} {
    # Identify the test
    tet_infoline account_create_25_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -description} msg] $msg
    } compare_strings {1 {The '-description' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_26_N {} {
    # Identify the test
    tet_infoline account_create_26_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -dupkey} msg] $msg
    } compare_strings {1 {The '-dupkey' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_27_N {} {
    # Identify the test
    tet_infoline account_create_27_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -expdate} msg] $msg
    } compare_strings {1 {The '-expdate' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_28_N {} {
    # Identify the test
    tet_infoline account_create_28_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -forwardabletkt} msg] $msg
    } compare_strings {1 {The '-forwardabletkt' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_29_N {} {
    # Identify the test
    tet_infoline account_create_29_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -goodsince} msg] $msg
    } compare_strings {1 {The '-goodsince' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_30_N {} {
    # Identify the test
    tet_infoline account_create_30_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -group} msg] $msg
    } compare_strings {1 {The '-group' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_31_N {} {
    # Identify the test
    tet_infoline account_create_31_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -group none} msg] $msg
    } compare_strings {1 {Required organization name attribute not provided.}}

    # Cleanup steps
}

proc account_create_32_N {} {
    # Identify the test
    tet_infoline account_create_32_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -organization none} msg] $msg
    } compare_strings {1 {Required group name attribute not provided.}}

    # Cleanup steps
}

proc account_create_33_N {} {
    # Identify the test
    tet_infoline account_create_33_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -postdatedtkt} msg] $msg
    } compare_strings {1 {The '-postdatedtkt' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_34_N {} {
    # Identify the test
    tet_infoline account_create_34_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -proxiabletkt} msg] $msg
    } compare_strings {1 {The '-proxiabletkt' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_35_N {} {
    # Identify the test
    tet_infoline account_create_35_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -pwdvalid} msg] $msg
    } compare_strings {1 {The '-pwdvalid' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_36_N {} {
    # Identify the test
    tet_infoline account_create_36_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -renewabletkt} msg] $msg
    } compare_strings {1 {The '-renewabletkt' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_37_N {} {
    # Identify the test
    tet_infoline account_create_37_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -attribute {sever /user}} msg] $msg
    } compare_strings {1 {Supplied attribute 'sever' is illegal.}}

    # Cleanup steps
}

proc account_create_38_N {} {
    # Identify the test
    tet_infoline account_create_38_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -attribute {sell hsk}} msg] $msg
    } compare_strings {1 {Supplied attribute 'sell' is illegal.}}

    # Cleanup steps
}

proc account_create_39_N {} {
    # Identify the test
    tet_infoline account_create_39_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -stdtgtauth} msg] $msg
    } compare_strings {1 {The '-stdtgtauth' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_40_N {} {
    # Identify the test
    tet_infoline account_create_40_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -maxtktlife} msg] $msg
    } compare_strings {1 {The '-maxtktlife' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_41_N {} {
    # Identify the test
    tet_infoline account_create_41_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -maxtktrenew} msg] $msg
    } compare_strings {1 {The '-maxtktrenew' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_42_N {} {
    # Identify the test
    tet_infoline account_create_42_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo} msg] $msg
    } compare_strings {1 {Required group name attribute not provided.}}

    # Cleanup steps
}

proc account_create_43_N {} {
    # Identify the test
    tet_infoline account_create_43_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -group junk} msg] $msg
    } compare_strings {1 {Required organization name attribute not provided.}}

    # Cleanup steps
}

proc account_create_44_N {} {
    # Identify the test
    tet_infoline account_create_44_N: Check bogus stuff passed to -attribute.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -attribute} msg] $msg
    } compare_strings {1 {The '-attribute' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_45_N {} {
    # Identify the test
    tet_infoline account_create_45_N: Check bogus stuff passed to -attribute.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -attribute \"\"} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc account_create_46_N {} {
    # Identify the test
    tet_infoline account_create_46_N: Check bogus stuff passed to -attribute.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -attribute gumby} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc account_create_47_N {} {
    # Identify the test
    tet_infoline account_create_47_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -usertouser 45} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc account_create_48_N {} {
    # Identify the test
    tet_infoline account_create_48_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create foo -usertouser} msg] $msg
    } compare_strings {1 {The '-usertouser' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_49_N {} {
    # Identify the test
    tet_infoline account_create_49_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pkmechanism} msg] $msg
    } compare_strings {1 {The '-pkmechanism' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_50_N {} {
    # Identify the test
    tet_infoline account_create_50_N: Check that the attribute list syntax is invalid.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attr {pkmechanism}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc account_create_51_N {} {
    # Identify the test
    tet_infoline account_create_51_N: Check that missing option argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature} msg] $msg
    } compare_strings {1 {The '-pksignatureusage' option requires a character string value.}}

    # Cleanup steps
}

proc account_create_52_N {} {
    # Identify the test
    tet_infoline account_create_52_N: Check that the attribute list syntax is invalid.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {pokey}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc account_create_53_N {} {
    # Identify the test
    tet_infoline account_create_53_N: Check that the attribute list syntax is invalid. 

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {generatekey}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc account_create_54_N {} {
    # Identify the test
    tet_infoline account_create_54_N: Check that the attribute list syntax is invalid.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attr {pksignature {privatekeyfile}}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc account_create_55_N {} {
    # Identify the test
    tet_infoline account_create_55_N: Check that only options or an attribute list are allowed.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pkmechanism file -attr {pksignature {gen 0}}} msg] $msg
    } compare_strings {1 {Only options or an attribute list are allowed.}}

    # Cleanup steps
}

proc account_create_56_N {} {
    # Identify the test
    tet_infoline account_create_56_N: Check for an unrecognized argument.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {new gumby} {public goober}} msg] $msg
    } compare_strings {1 {Unrecognized argument 'public goober'.}}

    # Cleanup steps
}

proc account_create_57_N {} {
    # Identify the test
    tet_infoline account_create_57_N: Check that supplied attribute is unknown. 

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {{generate 101} {newpass blah} {badattr gumby}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'badattr' is unknown.}}

    # Cleanup steps
}

proc account_create_58_N {} {
    global TET_ROOT DCP_PK_DATA_1

    # Identify the test
    tet_infoline account_create_58_N: Check that supplied attribute is unknown.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attr "{pksignature {{public $TET_ROOT/$DCP_PK_DATA_1} {badattrib pokey}}} \
                     {group none} {acctvalid no}"} msg] $msg
    } compare_strings {1 {Supplied attribute 'badattrib' is unknown.}}

    # Cleanup steps
}

proc account_create_59_N {} {
    # Identify the test
    tet_infoline account_create_59_N: Check that supplied value for 'generatekey' is valid. 

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {{generate pokey} {newpass blah}}} msg] $msg
    } compare_strings {1 {Supplied value for 'generatekey' must be zero, a positive integer or 'default'.}}

    # Cleanup steps
}

proc account_create_60_N {} {
    # Identify the test
    tet_infoline account_create_60_N: Check that supplied value for 'generatekey' is valid.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attr {pksignature {{generate pokey} {newpass blah}}}} msg] $msg
    } compare_strings {1 {Supplied value for 'generatekey' must be zero, a positive integer or 'default'.}}

    # Cleanup steps
}

proc account_create_61_N {} {
    # Identify the test
    tet_infoline account_create_61_N: Check that supplied value for 'generatekey' is valid.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {generate -100}} msg] $msg
    } compare_strings {1 {Supplied value for 'generatekey' must not be a negative integer.}}

    # Cleanup steps
}

proc account_create_62_N {} {
    # Identify the test
    tet_infoline account_create_62_N: Check that supplied value for 'generatekey' is valid.  

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {generate -100}} msg] $msg
    } compare_strings {1 {Supplied value for 'generatekey' must not be a negative integer.}}

    # Cleanup steps
}

proc account_create_63_N {} {
    # Identify the test
    tet_infoline account_create_63_N: Check that supplied value for 'generatekey' is valid.  

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attr {pksignature {generate -100}}} msg] $msg
    } compare_strings {1 {Supplied value for 'generatekey' must not be a negative integer.}}

    # Cleanup steps
}

proc account_create_64_N {} {
    # Identify the test
    tet_infoline account_create_64_N: Check that supplied value for key generation attribute is valid.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {generate 999999999999}} msg] $msg
    } compare_strings {1 {Supplied value for key generation attribute must be less than 2147483647.}}

    # Cleanup steps
}

proc account_create_65_N {} {
    # Identify the test
    tet_infoline account_create_65_N: Check that supplied value for key generation attribute is valid.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attr {pksignature {generatekey 999999999999}}} msg] $msg
    } compare_strings {1 {Supplied value for key generation attribute must be less than 2147483647.}}

    # Cleanup steps
}

proc account_create_66_N {} {
    # Identify the test
    tet_infoline account_create_66_N: Check that the value for 'pkmechanism' is either 'file' or 'pkss'.

    # Setup steps

    # Run the test
    test {
        list [catch {account create pokey -pkmechanism gumby} msg] $msg
    } compare_strings {1 {The value for 'pkmechanism' must be either 'file' or 'pkss'.}}

    # Cleanup steps
}

proc account_create_67_N {} {
    # Identify the test
    tet_infoline account_create_67_N: Check that the value for 'pkmechanism' is either 'file' or 'pkss'.

    # Setup steps

    # Run the test
    test {
        list [catch {account create pokey -attr {pkmechanism gumby}} msg] $msg
    } compare_strings {1 {The value for 'pkmechanism' must be either 'file' or 'pkss'.}}

    # Cleanup steps
}

proc account_create_68_N {} {
    # Identify the test
    tet_infoline account_create_68_N: Check if supplied attribute was specified more than once.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {{generate 0} {generate 1024}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'generatekey' was specified more than once.}}

    # Cleanup steps
}

proc account_create_69_N {} {
    # Identify the test
    tet_infoline account_create_69_N: Check if supplied attribute was specified more than once.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {{private gumby} {private pokey}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'privatekeyfile' was specified more than once.}}

    # Cleanup steps
}

proc account_create_70_N {} {
    global TET_ROOT DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_create_70_N: Check if supplied attribute was specified more than once.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature "{publickey $TET_ROOT/$DCP_PK_DATA_1} \
                     {publickey $TET_ROOT/$DCP_PK_DATA_2}"} msg] $msg
    } compare_strings {1 {Supplied attribute 'publickeyfile' was specified more than once.}}

    # Cleanup steps
}

proc account_create_71_N {} {
    # Identify the test
    tet_infoline account_create_71_N: Check if supplied attribute was specified more than once.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {{oldpassphrase blah} {oldpassphrase blah2}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'oldpassphrase' was specified more than once.}}

    # Cleanup steps
}

proc account_create_72_N {} {
    # Identify the test
    tet_infoline account_create_72_N: Check if supplied attribute  was specified more than once.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {{new {long string}} {new shortstring}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'newpassphrase' was specified more than once.}}

    # Cleanup steps
}

proc account_create_73_N {} {
    global TET_ROOT DCP_PK_DATA_1

    # Identify the test
    tet_infoline account_create_73_N: Check if supplied attribute was specified more than once.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attr "{pksignature {{public $TET_ROOT/$DCP_PK_DATA_1} \
                     {public $TET_ROOT/$DCP_PK_DATA_1}}} {acctvalid no}"} msg] $msg
    } compare_strings {1 {Supplied attribute 'publickeyfile' was specified more than once.}}

    # Cleanup steps
}

proc account_create_74_N {} {
    global TET_ROOT DCP_PK_DATA_1

    # Identify the test
    tet_infoline account_create_74_N: Check that a public or private keyfile is not specified when
    tet_infoline account_create_74_N: 'generatekey' is also specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature "{gen 1024} {new pokey} {private $TET_ROOT/$DCP_PK_DATA_1}"} msg] $msg
    } compare_strings {1 {Cannot specify a public or private keyfile when 'generatekey' is also specified.}}

    # Cleanup steps
}

proc account_create_75_N {} {
    global TET_ROOT DCP_PK_DATA_1

    # Identify the test
    tet_infoline account_create_75_N: Check that a public or private keyfile is not specified when
    tet_infoline account_create_75_N: 'generatekey' is also specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature "{gen default} {new gumby} {public $TET_ROOT/$DCP_PK_DATA_1}"} msg] $msg
    } compare_strings {1 {Cannot specify a public or private keyfile when 'generatekey' is also specified.}}

    # Cleanup steps
}

proc account_create_76_N {} {
    # Identify the test
    tet_infoline account_create_76_N: Check that a public or private keyfile is not specified when
    tet_infoline account_create_76_N: 'generatekey' is also specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {{generatekey 101} {newpassphrase gumby_string} \
                     {publickeyfile /tmp/gumby} {privatekeyfile /tmp/pokey}}} msg] $msg
    } compare_strings {1 {Cannot specify a public or private keyfile when 'generatekey' is also specified.}}

    # Cleanup steps
}

proc account_create_77_N {} {
    global TET_ROOT DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_create_77_N: Check that a public or private keyfile is not specified when
    tet_infoline account_create_77_N: 'generatekey' is also specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account create pokey -attr "pksignature {{generatekey 1001} {new pokey} \
                     {publickeyfile $TET_ROOT/$DCP_PK_DATA_1} {privatekeyfile $TET_ROOT/$DCP_PK_DATA_2}}"} msg] $msg
    } compare_strings {1 {Cannot specify a public or private keyfile when 'generatekey' is also specified.}}

    # Cleanup steps
}

proc account_create_78_N {} {
    # Identify the test
    tet_infoline account_create_78_N: Check that both 'publickeyfile' and 'privatekeyfile' are specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {newpass pokey}} msg] $msg
    } compare_strings {1 {Must specify both 'publickeyfile' and 'privatekeyfile'.}}

    # Cleanup steps
}

proc account_create_79_N {} {
    # Identify the test
    tet_infoline account_create_79_N: Check that both 'publickeyfile' and 'privatekeyfile' are specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {{gen 0} {newpass pokey1}}} msg] $msg
    } compare_strings {1 {Must specify both 'publickeyfile' and 'privatekeyfile'.}}

    # Cleanup steps
}

proc account_create_80_N {} {
    # Identify the test
    tet_infoline account_create_80_N: Check that both 'publickeyfile' and 'privatekeyfile' are specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attr {pksignature {newpass pokey}}} msg] $msg
    } compare_strings {1 {Must specify both 'publickeyfile' and 'privatekeyfile'.}}

    # Cleanup steps
}

proc account_create_81_N {} {
    # Identify the test
    tet_infoline account_create_81_N: Check that both 'publickeyfile' and 'privatekeyfile' are specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attr {pksignature {{gen 0} {newpass pokey1}}}} msg] $msg
    } compare_strings {1 {Must specify both 'publickeyfile' and 'privatekeyfile'.}}

    # Cleanup steps
}

proc account_create_82_N {} {
    # Identify the test
    tet_infoline account_create_82_N: Check that both 'publickeyfile' and 'privatekeyfile' are specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {{new gumby} {privatekey gumby1}}} msg] $msg
    } compare_strings {1 {Must specify both 'publickeyfile' and 'privatekeyfile'.}}

    # Cleanup steps
}

proc account_create_83_N {} {
    # Identify the test
    tet_infoline account_create_83_N: Check that both 'publickeyfile' and 'privatekeyfile' are specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {{new gumby} {publickey pokey1}}} msg] $msg
    } compare_strings {1 {Must specify both 'publickeyfile' and 'privatekeyfile'.}}

    # Cleanup steps
}

proc account_create_84_N {} {
    # Identify the test
    tet_infoline account_create_84_N: Check that both 'publickeyfile' and 'privatekeyfile' are specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attr {pksignature {new gumby} {privatekeyfile goober}}} msg] $msg
    } compare_strings {1 {Must specify both 'publickeyfile' and 'privatekeyfile'.}}

    # Cleanup steps
}

proc account_create_85_N {} {
    # Identify the test
    tet_infoline account_create_85_N: Check that both 'publickeyfile' and 'privatekeyfile' are specified. 

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attr {pksignature {{new gumby} {publickeyfile goober}}}} msg] $msg
    } compare_strings {1 {Must specify both 'publickeyfile' and 'privatekeyfile'.}}

    # Cleanup steps
}

proc account_create_86_N {} {
    # Identify the test
    tet_infoline account_create_86_N: Check that supplied attribute 'oldpassphrase' is not used during a create operation. 

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {{gen 101} {old gumby} {new pokey}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'oldpassphrase' is illegal during a create operation.}}

    # Cleanup steps
}

proc account_create_87_N {} {
    # Identify the test
    tet_infoline account_create_87_N: Check that supplied attribute 'oldpassphrase' is not used during a create operation.                            

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {{gen 0} {old gumby} {new pokey}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'oldpassphrase' is illegal during a create operation.}}

    # Cleanup steps
}

proc account_create_88_N {} {
    # Identify the test
    tet_infoline account_create_88_N: Check that supplied attribute 'oldpassphrase' is not used during a create operation.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {{old gumby} {new pokey}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'oldpassphrase' is illegal during a create operation.}}

    # Cleanup steps
}

proc account_create_89_N {} {
    global TET_ROOT DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_create_89_N: Check that supplied attribute 'oldpassphrase' is not used during a create operation.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature "{oldpass gumby} {newpass pokey} \
                     {publickeyfile $TET_ROOT/$DCP_PK_DATA_1} {privatekeyfile $TET_ROOT/$DCP_PK_DATA_2}"} msg] $msg
    } compare_strings {1 {Supplied attribute 'oldpassphrase' is illegal during a create operation.}}

    # Cleanup steps
}

proc account_create_90_N {} {
    # Identify the test
    tet_infoline account_create_90_N: Check that supplied attribute 'oldpassphrase' is not used during a create operation.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attribute {pksignature {{gen 101} {old gumby} {new pokey}}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'oldpassphrase' is illegal during a create operation.}}

    # Cleanup steps
}

proc account_create_91_N {} {
    # Identify the test
    tet_infoline account_create_91_N: Check that supplied attribute 'oldpassphrase' is not used during a create operation.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attribute {pksignature {{gen 0} {old gumby} {new pokey}}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'oldpassphrase' is illegal during a create operation.}}

    # Cleanup steps
}

proc account_create_92_N {} {
    # Identify the test
    tet_infoline account_create_92_N: Check that supplied attribute 'oldpassphrase' is not used during a create operation.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attribute {pksignature {{old gumby} {new pokey}}}} msg] $msg
    } compare_strings {1 {Supplied attribute 'oldpassphrase' is illegal during a create operation.}}

    # Cleanup steps
}

proc account_create_93_N {} {
    global TET_ROOT DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_create_93_N: Check that supplied attribute 'oldpassphrase' is not used during a create operation.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attribute "pksignature {{oldpass gumby} {newpass pokey} \
                     {publickeyfile $TET_ROOT/$DCP_PK_DATA_1} {privatekeyfile $TET_ROOT/$DCP_PK_DATA_2}}"} msg] $msg
    } compare_strings {1 {Supplied attribute 'oldpassphrase' is illegal during a create operation.}}

    # Cleanup steps
}

proc account_create_94_N {} {
    global TET_ROOT DCP_PK_DATA_1

    # Identify the test
    tet_infoline account_create_94_N: Check that the same file is not used for public and private keys.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature "{newpass gumby} {public $TET_ROOT/$DCP_PK_DATA_1} \
                     {private $TET_ROOT/$DCP_PK_DATA_1}"} msg] $msg
    } compare_strings {1 {The same file cannot be used for public and private keys.}}

    # Cleanup steps
}

proc account_create_95_N {} {
    global TET_ROOT DCP_PK_DATA_1

    # Identify the test
    tet_infoline account_create_95_N: Check that the same file is not used for public and private keys.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attr "pksignature {{newpass gumby} {public $TET_ROOT/$DCP_PK_DATA_1} \
                     {private $TET_ROOT/$DCP_PK_DATA_1}}"} msg] $msg
    } compare_strings {1 {The same file cannot be used for public and private keys.}}

    # Cleanup steps
}

proc account_create_96_N {} {
    # Identify the test
    tet_infoline account_create_96_N: Check that file '/tmp/bad2' is not opened.  

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature {{gen 0} {newpass gumby} {privatekey /tmp/bad1} \
                     {public /tmp/bad2}}} msg] $msg
    } compare_strings {1 {Could not open file '/tmp/bad2'.}}

    # Cleanup steps
}

proc account_create_97_N {} {
    global TET_ROOT DCP_PK_DATA_1

    # Identify the test
    tet_infoline account_create_97_N: Check that file '/tmp/bad1 is not opened. 

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attr "pksignature {{gen 0} {new gumby} {privatekey /tmp/bad1} \
                     {publickeyfile $TET_ROOT/$DCP_PK_DATA_1}}"} msg] $msg
    } compare_strings {1 {Could not open file '/tmp/bad1'.}}

    # Cleanup steps
}

proc account_create_98_N {} {
    global TET_ROOT DCP_PK_DATA_1 DCP_PK_DATA_3

    # Identify the test
    tet_infoline account_create_98_N: Check that empty file is not readable. 

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -pksignature "{new gumby} {public $TET_ROOT/$DCP_PK_DATA_1} \
                     {private $TET_ROOT/$DCP_PK_DATA_3}"} msg] $msg
    } compare_strings "1 {Could not read from file '$TET_ROOT/$DCP_PK_DATA_3'.}"

    # Cleanup steps
}

proc account_create_99_N {} {
    global TET_ROOT DCP_PK_DATA_1 DCP_PK_DATA_3

    # Identify the test
    tet_infoline account_create_99_N: Check that empty file is not readable. 

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attr "pksignature {{new gumby} {public $TET_ROOT/$DCP_PK_DATA_1} \
                     {private $TET_ROOT/$DCP_PK_DATA_3}}"} msg] $msg
    } compare_strings "1 {Could not read from file '$TET_ROOT/$DCP_PK_DATA_3'.}"

    # Cleanup steps
}

proc account_create_100_N {} {
    # Identify the test
    tet_infoline account_create_100_N: The same 'newpass' must be used when two keys specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -group none -org none -password none -mypwd none \
                     -pksignature {{generate 800} {newpassphrase gumby}} \
                     -pkkeycipher {{generate 800} {newpassphrase pokey}}} msg] $msg
    } compare_strings {1 {Passphrases for all the public keys must be the same.}}

    # Cleanup steps
}

proc account_create_101_N {} {
    # Identify the test
    tet_infoline account_create_101_N: The same 'newpass' must be used when two keys specified.

    # Setup steps

    # Run the test
    test {
        list [catch {account create gumby -attr "{group none} {organization none} {password none} \
                     {mypwd -dce-} {pksignature {{generate 256} {newpassphrase gumby}}} \
                     {pkkeycipher {{generate 256} {newpassphrase pokey}}}"} msg] $msg
    } compare_strings {1 {Passphrases for all the public keys must be the same.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 ic38 ic39 ic40 ic41 ic42 ic43 ic44 ic45 ic46 ic47 ic48 ic49 ic50 ic51 ic52 ic53 ic54 ic55 ic56 ic57 ic58 ic59 ic60 ic61 ic62 ic63 ic64 ic65 ic66 ic67 ic68 ic69 ic70 ic71 ic72 ic73 ic74 ic75 ic76 ic77 ic78 ic79 ic80 ic81 ic82 ic82 ic83 ic84 ic85 ic86 ic87 ic88 ic89 ic90 ic91 ic92 ic93 ic94 ic95 ic96 ic97 ic98 ic99 ic100 ic101 "
set ic1 "account_create_1_N"
set ic2 "account_create_2_N"
set ic3 "account_create_3_N"
set ic4 "account_create_4_N"
set ic5 "account_create_5_N"
set ic6 "account_create_6_N"
set ic7 "account_create_7_N"
set ic8 "account_create_8_N"
set ic9 "account_create_9_N"
set ic10 "account_create_10_N"
set ic11 "account_create_11_N"
set ic12 "account_create_12_N"
set ic13 "account_create_13_N"
set ic14 "account_create_14_N"
set ic15 "account_create_15_N"
set ic16 "account_create_16_N"
set ic17 "account_create_17_N"
set ic18 "account_create_18_N"
set ic19 "account_create_19_N"
set ic20 "account_create_20_N"
set ic21 "account_create_21_N"
set ic22 "account_create_22_N"
set ic23 "account_create_23_N"
set ic24 "account_create_24_N"
set ic25 "account_create_25_N"
set ic26 "account_create_26_N"
set ic27 "account_create_27_N"
set ic28 "account_create_28_N"
set ic29 "account_create_29_N"
set ic30 "account_create_30_N"
set ic31 "account_create_31_N"
set ic32 "account_create_32_N"
set ic33 "account_create_33_N"
set ic34 "account_create_34_N"
set ic35 "account_create_35_N"
set ic36 "account_create_36_N"
set ic37 "account_create_37_N"
set ic38 "account_create_38_N"
set ic39 "account_create_39_N"
set ic40 "account_create_40_N"
set ic41 "account_create_41_N"
set ic42 "account_create_42_N"
set ic43 "account_create_43_N"
set ic44 "account_create_44_N"
set ic45 "account_create_45_N"
set ic46 "account_create_46_N"
set ic47 "account_create_47_N"
set ic48 "account_create_48_N"
set ic49 "account_create_49_N"
set ic50 "account_create_50_N"
set ic51 "account_create_51_N"
set ic52 "account_create_52_N"
set ic53 "account_create_53_N"
set ic54 "account_create_54_N"
set ic55 "account_create_55_N"
set ic56 "account_create_56_N"
set ic57 "account_create_57_N"
set ic58 "account_create_58_N"
set ic59 "account_create_59_N"
set ic60 "account_create_60_N"
set ic61 "account_create_61_N"
set ic62 "account_create_62_N"
set ic63 "account_create_63_N"
set ic64 "account_create_64_N"
set ic65 "account_create_65_N"
set ic66 "account_create_66_N"
set ic67 "account_create_67_N"
set ic68 "account_create_68_N"
set ic69 "account_create_69_N"
set ic70 "account_create_70_N"
set ic71 "account_create_71_N"
set ic72 "account_create_72_N"
set ic73 "account_create_73_N"
set ic74 "account_create_74_N"
set ic75 "account_create_75_N"
set ic76 "account_create_76_N"
set ic77 "account_create_77_N"
set ic78 "account_create_78_N"
set ic79 "account_create_79_N"
set ic80 "account_create_80_N"
set ic81 "account_create_81_N"
set ic82 "account_create_82_N"
set ic83 "account_create_83_N"
set ic84 "account_create_84_N"
set ic85 "account_create_85_N"
set ic86 "account_create_86_N"
set ic87 "account_create_87_N"
set ic88 "account_create_88_N"
set ic89 "account_create_89_N"
set ic90 "account_create_90_N"
set ic91 "account_create_91_N"
set ic92 "account_create_92_N"
set ic93 "account_create_93_N"
set ic94 "account_create_94_N"
set ic95 "account_create_95_N"
set ic96 "account_create_96_N"
set ic97 "account_create_97_N"
set ic98 "account_create_98_N"
set ic99 "account_create_99_N"
set ic100 "account_create_100_N"
set ic101 "account_create_101_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
