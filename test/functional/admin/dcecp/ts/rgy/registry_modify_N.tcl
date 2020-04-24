#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_modify_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:22:58  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:37  marty]
#
# Revision 1.1.4.1  1995/12/11  15:12:57  root
# 	Submit
# 	[1995/12/11  14:30:52  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:25  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:49  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:32  melman]
# 
# Revision 1.1.2.7  1994/09/28  17:54:51  kevins
# 	OT 12376 modified registry unavailable messages
# 	[1994/09/28  17:51:56  kevins]
# 
# Revision 1.1.2.6  1994/09/20  15:49:14  salamone
# 	Fix authentication messages
# 	[1994/09/20  15:48:05  salamone]
# 
# Revision 1.1.2.5  1994/08/26  12:43:28  kevins
# 	OT 11598 added tests for registry -policies ops
# 	[1994/08/26  11:06:21  kevins]
# 
# Revision 1.1.2.4  1994/06/29  21:39:26  salamone
# 	Add -key option support.
# 	[1994/06/29  20:10:44  salamone]
# 
# Revision 1.1.2.3  1994/06/10  20:15:58  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:21  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  19:40:05  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:29:10  rousseau]
# 
# Revision 1.1.2.1  1994/05/11  13:48:02  kevins
# 	Initial submission
# 	[1994/05/11  13:40:24  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_modify_001_N {} {
    # Identify the test
    tet_infoline registry_modify_001_N: Check that at least one option must be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify foo} msg] $msg
    } compare_strings {1 {The command requires at least one option.}}

    # Cleanup steps
}

proc registry_modify_002_N {} {
    # Identify the test
    tet_infoline registry_modify_002_N: Check that only one argument can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify {foo foo2} -key} msg] $msg
    } compare_strings {1 {Only one registry replica is allowed.}}

    # Cleanup steps
}

proc registry_modify_003_N {} {
    # Identify the test
    tet_infoline registry_modify_003_N: Check for a bogus replica with the -key option.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify foo -key} msg] $msg
    } compare_strings {1 {Registry server 'foo' is unavailable.}}

    # Cleanup steps
}

proc registry_modify_004_N {} {
    # Identify the test
    tet_infoline registry_modify_004_N: Check for a bogus replica with an attribute change.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify foo -mingid 44} msg] $msg
    } compare_strings {1 {Registry replica specified is not the master.}}

    # Cleanup steps
}

proc registry_modify_005_N {} {
    # Identify the test
    tet_infoline registry_modify_005_N: Check for a bogus replica with a policy change.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify foo -hidepwd no} msg] $msg
    } compare_strings {1 {Registry replica specified is not the master.}}

    # Cleanup steps
}

proc registry_modify_006_N {} {
    # Identify the test
    tet_infoline registry_modify_006_N: Check that only one argument can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify foo foo2 -key} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo2'.}}

    # Cleanup steps
}

proc registry_modify_007_N {} {
    # Identify the test
    tet_infoline registry_modify_007_N: Check for bogus option specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify foo -} msg] $msg
    } compare_strings {1 {Ambiguous argument '-': -acctlife -change -deftktlife -hidepwd -key -maxtktlife -maxtktrenew -maxuid -mingid -minorgid -mintktlife -minuid -pwdalpha -pwdexpdate -pwdlife -pwdminlen -pwdspaces -version}}

    # Cleanup steps
}

proc registry_modify_008_N {} {
    # Identify the test
    tet_infoline registry_modify_008_N: Check for bogus option specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify foo -foo2} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo2'.}}

    # Cleanup steps
}

proc registry_modify_009_N {} {
    global DCP_CLIENT DCP_CLIENT_PW DCP_NOTROOT DCP_NOTROOT_PW

    # Identify the test
    tet_infoline registry_modify_009_N: Check for authentication to perform this operation.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {group add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {account create $DCP_NOTROOT -group none -organization none -password $DCP_NOTROOT_PW -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_NOTROOT -p $DCP_NOTROOT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry modify $DCP_RGY_SA_REP_NAME_MASTER -key} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {account delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_NOTROOT}] != 0} {return;}
}

proc registry_modify_010_N {} {
    # Identify the test
    tet_infoline registry_modify_010_N: Check that the -key option is mutually exclusive
    tet_infoline registry_modify_010_N: from an attribute change. 

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry modify $DCP_RGY_SA_REP_NAME_SLAVE1 -key -mingid 44} msg] $msg
    } compare_strings {1 {Mutually exclusive options entered.}}

    # Cleanup steps
}

proc registry_modify_011_N {} {
    # Identify the test
    tet_infoline registry_modify_011_N: Check that the -key option is mutually exclusive
    tet_infoline registry_modify_011_N: from a policy change. 

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry modify $DCP_RGY_SA_REP_NAME_SLAVE1 -key -hidepwd no} msg] $msg
    } compare_strings {1 {Mutually exclusive options entered.}}

    # Cleanup steps
}

proc registry_modify_012_N {} {
    # Identify the test
    tet_infoline registry_modify_012_N: Check the handling of the - option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -deftktlife} msg] $msg
    } compare_strings {1 {The '-deftktlife' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_013_N {} {
    # Identify the test
    tet_infoline registry_modify_013_N: Check the handling of the - option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -hidepwd} msg] $msg
    } compare_strings {1 {The '-hidepwd' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_014_N {} {
    # Identify the test
    tet_infoline registry_modify_014_N: Check the handling of the - option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -maxuid} msg] $msg
    } compare_strings {1 {The '-maxuid' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_015_N {} {
    # Identify the test
    tet_infoline registry_modify_015_N: Check the handling of the - option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -mingid} msg] $msg
    } compare_strings {1 {The '-mingid' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_016_N {} {
    # Identify the test
    tet_infoline registry_modify_016_N: Check the handling of the - option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -minorgid} msg] $msg
    } compare_strings {1 {The '-minorgid' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_017_N {} {
    # Identify the test
    tet_infoline registry_modify_017_N: Check the handling of the - option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -mintktlife} msg] $msg
    } compare_strings {1 {The '-mintktlife' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_018_N {} {
    # Identify the test
    tet_infoline registry_modify_018_N: Check the handling of the - option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -minuid} msg] $msg
    } compare_strings {1 {The '-minuid' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_019_N {} {
    # Identify the test
    tet_infoline registry_modify_019_N: Check the handling of the - option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -change} msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_020_N {} {
    # Identify the test
    tet_infoline registry_modify_020_N: Check the handling of the -acctlife option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -acctlife} msg] $msg
    } compare_strings {1 {The '-acctlife' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_021_N {} {
    # Identify the test
    tet_infoline registry_modify_021_N: Check the handling of the -maxtktrenew option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -maxtktrenew} msg] $msg
    } compare_strings {1 {The '-maxtktrenew' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_022_N {} {
    # Identify the test
    tet_infoline registry_modify_022_N: Check the handling of the -maxtktlif option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -maxtktlife} msg] $msg
    } compare_strings {1 {The '-maxtktlife' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_023_N {} {
    # Identify the test
    tet_infoline registry_modify_023_N: Check the handling of the -pwdalpha option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -pwdalpha} msg] $msg
    } compare_strings {1 {The '-pwdalpha' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_024_N {} {
    # Identify the test
    tet_infoline registry_modify_024_N: Check the handling of the -pwdexpdate option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -pwdexpdate} msg] $msg
    } compare_strings {1 {The '-pwdexpdate' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_025_N {} {
    # Identify the test
    tet_infoline registry_modify_025_N: Check the handling of the -pwdlife option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -pwdlife} msg] $msg
    } compare_strings {1 {The '-pwdlife' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_026_N {} {
    # Identify the test
    tet_infoline registry_modify_026_N: Check the handling of the -pwdminlen option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -pwdminlen} msg] $msg
    } compare_strings {1 {The '-pwdminlen' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_027_N {} {
    # Identify the test
    tet_infoline registry_modify_027_N: Check the handling of the -pwdspaces option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -pwdspaces} msg] $msg
    } compare_strings {1 {The '-pwdspaces' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_028_N {} {
    # Identify the test
    tet_infoline registry_modify_028_N: Check that unauthorized uses are trapped as errors.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry modify -minuid 5100} msg] $msg
    } compare_strings {1 {User is not authorized to update record}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_modify_029_N {} {
    # Identify the test
    tet_infoline registry_modify_029_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -minuid -1} msg] $msg
    } compare_strings {1 {The 'minuid' attribute must be a positive integer.}}

    # Cleanup steps
}

proc registry_modify_030_N {} {
    # Identify the test
    tet_infoline registry_modify_030_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -minuid foo} msg] $msg
    } compare_strings {1 {The 'minuid' attribute must be an integer.}}

    # Cleanup steps
}

proc registry_modify_031_N {} {
    # Identify the test
    tet_infoline registry_modify_031_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -mingid -1} msg] $msg
    } compare_strings {1 {The 'mingid' attribute must be a positive integer.}}

    # Cleanup steps
}

proc registry_modify_032_N {} {
    # Identify the test
    tet_infoline registry_modify_032_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -mingid foo} msg] $msg
    } compare_strings {1 {The 'mingid' attribute must be an integer.}}

    # Cleanup steps
}

proc registry_modify_033_N {} {
    # Identify the test
    tet_infoline registry_modify_033_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -deftktlife help} msg] $msg
    } compare_strings {1 {Bad relative time string supplied.}}

    # Cleanup steps
}

proc registry_modify_034_N {} {
    # Identify the test
    tet_infoline registry_modify_034_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -hidepwd oops} msg] $msg
    } compare_strings {1 {Unrecognized argument 'oops'.}}

    # Cleanup steps
}

proc registry_modify_035_N {} {
    # Identify the test
    tet_infoline registry_modify_035_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -maxuid -1} msg] $msg
    } compare_strings {1 {The 'maxuid' attribute must be a positive integer.}}

    # Cleanup steps
}

proc registry_modify_036_N {} {
    # Identify the test
    tet_infoline registry_modify_036_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -maxuid foo} msg] $msg
    } compare_strings {1 {The 'maxuid' attribute must be an integer.}}

    # Cleanup steps
}

proc registry_modify_037_N {} {
    # Identify the test
    tet_infoline registry_modify_037_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -minorgid -1} msg] $msg
    } compare_strings {1 {The 'minorgid' attribute must be a positive integer.}}

    # Cleanup steps
}

proc registry_modify_038_N {} {
    # Identify the test
    tet_infoline registry_modify_038_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -minorgid foo} msg] $msg
    } compare_strings {1 {The 'minorgid' attribute must be an integer.}}

    # Cleanup steps
}

proc registry_modify_039_N {} {
    # Identify the test
    tet_infoline registry_modify_039_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -mintktlife foo} msg] $msg
    } compare_strings {1 {Bad relative time string supplied.}}

    # Cleanup steps
}

proc registry_modify_040_N {} {
    # Identify the test
    tet_infoline registry_modify_040_N: Check that a registry replica must be specified
    tet_infoline registry_modify_040_N: when using the -key option.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -key} msg] $msg
    } compare_strings {1 {Registry replica not specified.}}

    # Cleanup steps
}

proc registry_modify_041_N {} {
    # Identify the test
    tet_infoline registry_modify_041_N: Check that the argument can not be specified
    tet_infoline registry_modify_041_N: as the local cell when using the -key option.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify /.: -key} msg] $msg
    } compare_strings {1 {Registry server '/.:' is unavailable.}}

    # Cleanup steps
}

proc registry_modify_042_N {} {
    # Identify the test
    tet_infoline registry_modify_042_N: Check that the argument can not be specified
    tet_infoline registry_modify_042_N: as a cell namewhen using the -key option.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry modify $DCP_CELLNAME_ONE -key} msg] $msg
    } compare_strings "1 {Registry server '$DCP_CELLNAME_ONE' is unavailable.}"

    # Cleanup steps
}

proc registry_modify_043_N {} {
    # Identify the test
    tet_infoline registry_modify_043_N: Check that a registry replica must be specified
    tet_infoline registry_modify_043_N: when using the -key option.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry modify $DCP_RGY_SA_REP_NAME_SLAVE1 -key jjjjjj} msg] $msg
    } compare_strings {1 {Unrecognized argument 'jjjjjj'.}}

    # Cleanup steps
}

proc registry_modify_044_N {} {
    # Identify the test
    tet_infoline registry_modify_044_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -acctlife foo} msg] $msg
    } compare_strings {1 {Supplied 'acctlife' is a bad date value.}}

    # Cleanup steps
}

proc registry_modify_045_N {} {
    # Identify the test
    tet_infoline registry_modify_045_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -mintktlife foo} msg] $msg
    } compare_strings {1 {Bad relative time string supplied.}}

    # Cleanup steps
}

proc registry_modify_046_N {} {
    # Identify the test
    tet_infoline registry_modify_046_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -maxtktrenew foo} msg] $msg
    } compare_strings {1 {Bad relative time string supplied.}}

    # Cleanup steps
}

proc registry_modify_047_N {} {
    # Identify the test
    tet_infoline registry_modify_047_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -pwdalpha foo} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc registry_modify_048_N {} {
    # Identify the test
    tet_infoline registry_modify_048_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -pwdexpdate foo} msg] $msg
    } compare_strings {1 {Bad 'pwdexpdate' date string supplied.}}

    # Cleanup steps
}

proc registry_modify_048_N {} {
    # Identify the test
    tet_infoline registry_modify_049_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -pwdlife foo} msg] $msg
    } compare_strings {1 {Supplied 'pwdlife' is a bad date value.}}

    # Cleanup steps
}

proc registry_modify_050_N {} {
    # Identify the test
    tet_infoline registry_modify_050_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -pwdminlen foo} msg] $msg
    } compare_strings {1 {Supplied 'pwdminlen' is not a positive integer.}}

    # Cleanup steps
}

proc registry_modify_051_N {} {
    # Identify the test
    tet_infoline registry_modify_051_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -maxuid foo} msg] $msg
    } compare_strings {1 {The 'maxuid' attribute must be an integer.}}

    # Cleanup steps
}

proc registry_modify_052_N {} {
    # Identify the test
    tet_infoline registry_modify_052_N: Check that missing arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -version} msg] $msg
    } compare_strings {1 {The '-version' option requires a character string value.}}

    # Cleanup steps
}

proc registry_modify_053_N {} {
    # Identify the test
    tet_infoline registry_modify_053_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -version gumby} msg] $msg
    } compare_strings {1 {The string 'gumby' is not a legal security software version.}}

    # Cleanup steps
}

proc registry_modify_054_N {} {
    # Identify the test
    tet_infoline registry_modify_054_N: Check that missing arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -change {version}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc registry_modify_055_N {} {
    # Identify the test
    tet_infoline registry_modify_055_N: Check that invalid arguments are traped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry modify -change {version gumby}} msg] $msg
    } compare_strings {1 {The string 'gumby' is not a legal security software version.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 ic38 ic39 ic40 ic41 ic42 ic43 ic44 ic45 ic46 ic47 ic48 ic49 ic50 ic51 ic52 ic53 ic54 ic55 "
set ic1 "registry_modify_001_N"
set ic2 "registry_modify_002_N"
set ic3 "registry_modify_003_N"
set ic4 "registry_modify_004_N"
set ic5 "registry_modify_005_N"
set ic6 "registry_modify_006_N"
set ic7 "registry_modify_007_N"
set ic8 "registry_modify_008_N"
set ic9 "registry_modify_009_N"
set ic10 "registry_modify_010_N"
set ic11 "registry_modify_011_N"
set ic12 "registry_modify_012_N"
set ic13 "registry_modify_013_N"
set ic14 "registry_modify_014_N"
set ic15 "registry_modify_015_N"
set ic16 "registry_modify_016_N"
set ic17 "registry_modify_017_N"
set ic18 "registry_modify_018_N"
set ic19 "registry_modify_019_N"
set ic20 "registry_modify_020_N"
set ic21 "registry_modify_021_N"
set ic22 "registry_modify_022_N"
set ic23 "registry_modify_023_N"
set ic24 "registry_modify_024_N"
set ic25 "registry_modify_025_N"
set ic26 "registry_modify_026_N"
set ic27 "registry_modify_027_N"
set ic28 "registry_modify_028_N"
set ic29 "registry_modify_029_N"
set ic30 "registry_modify_030_N"
set ic31 "registry_modify_031_N"
set ic32 "registry_modify_032_N"
set ic33 "registry_modify_033_N"
set ic34 "registry_modify_034_N"
set ic35 "registry_modify_035_N"
set ic36 "registry_modify_036_N"
set ic37 "registry_modify_037_N"
set ic38 "registry_modify_038_N"
set ic39 "registry_modify_039_N"
set ic40 "registry_modify_040_N"
set ic41 "registry_modify_041_N"
set ic42 "registry_modify_042_N"
set ic43 "registry_modify_043_N"
set ic44 "registry_modify_044_N"
set ic45 "registry_modify_045_N"
set ic46 "registry_modify_046_N"
set ic47 "registry_modify_047_N"
set ic48 "registry_modify_048_N"
set ic49 "registry_modify_048_N"
set ic50 "registry_modify_050_N"
set ic51 "registry_modify_051_N"
set ic52 "registry_modify_052_N"
set ic53 "registry_modify_053_N"
set ic54 "registry_modify_054_N"
set ic55 "registry_modify_055_N"
set tet_startup registry_modify_startup
set tet_cleanup registry_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
