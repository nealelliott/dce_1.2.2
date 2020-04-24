#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: account_generate_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:18:44  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:00  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:14  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/09  15:27 UTC  truitt
# 	Location of pwd_mgmt object changed.
# 
# 	HP revision /main/truitt_dcecp_chfts19/1  1995/03/09  15:25 UTC  truitt
# 	Location of pwd_mgmt handle changed.
# 	[1995/12/11  15:54:12  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:29  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:44  melman]
# 
# Revision 1.1.2.1  1994/09/16  18:21:08  kevins
# 	OT 12199 Initial Submission
# 	[1994/09/16  18:20:18  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc account_generate_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc account_generate_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc account_generate_7_P {} {
    global DCP_CLIENT DCP_CLIENT_PW
    # Identify the test
    tet_infoline account_generate_7_P: Check to see that account passwords are generated.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL -attribute {{pwd_val_type 3}  {pwd_mgmt_binding {{dce  /.:/pwd_strength pktprivacy secret name} {/.:/subsys/dce/pwd_mgmt/pwd_strength}}}}}] != 0} {return;}
    if {[dcp_wrap {group add none -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test { if { [catch {account generate $PRINCIPAL} msg] == 0} {
               if {[string length $msg] != 0} {
                   account create $PRINCIPAL -group none -org none \
                       -password $msg -mypwd $DCP_CLIENT_PW 
                   if {[lsearch [account catalog -simple] $PRINCIPAL] != -1 } {
                       list success
                   } else {list fail}
               } else {list fail}
           } else {list fail}
    } compare_strings success

    # Cleanup steps
    if {[dcp_wrap {principal delete $PRINCIPAL }] != 0} {return;}
}

proc account_generate_8_P {} {
    global DCP_CLIENT_PW 
    # Identify the test
    tet_infoline account_generate_8_P: Check to see that account passwords are generated.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL -attribute {{pwd_val_type 2}  {pwd_mgmt_binding {{dce  /.:/pwd_strength pktprivacy secret name} {/.:/subsys/dce/pwd_mgmt/pwd_strength}}}}}] != 0} {return;}
    if {[dcp_wrap {group add none -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test { if { [catch {account generate $PRINCIPAL} msg] == 0} {
               if {[string length $msg] != 0} {
                   account create $PRINCIPAL -group none -org none \
                       -password $msg -mypwd $DCP_CLIENT_PW 
                   if {[lsearch [account catalog -simple] $PRINCIPAL] != -1 } {
                       list success
                   } else {list fail}
               } else {list fail}
           } else {list fail}
    } compare_strings success

    # Cleanup steps
    if {[dcp_wrap {principal delete $PRINCIPAL }] != 0} {return;}
}

proc account_generate_9_P {} {
    global DCP_CLIENT_PW
    # Identify the test
    tet_infoline account_generate_9_P: Check to see that account passwords are generated.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group add none -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group none -org none -password fake -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { principal modify $PRINCIPAL -add {{pwd_val_type 3} \
               {pwd_mgmt_binding {{dce  /.:/pwd_strength pktprivacy \
               secret name} {/.:/subsys/dce/pwd_mgmt/pwd_strength}}}}
         if { [catch {account generate $PRINCIPAL} msg] == 0} {
             if {[string length $msg] != 0} {
                 account modify $PRINCIPAL -password $msg -mypwd $DCP_CLIENT_PW 
                 if {[lsearch [account catalog -simple] $PRINCIPAL] != -1 } {
                     list success
                 } else {list fail}
             } else {list fail}
         } else {list fail}
    } compare_strings success

    # Cleanup steps
    if {[dcp_wrap {principal delete $PRINCIPAL }] != 0} {return;}
}

proc account_generate_10_P {} {
    # Identify the test
    tet_infoline account_generate_10_P: Check to see that account passwords are generated.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL -attribute {{pwd_val_type 3}  {pwd_mgmt_binding {{dce  /.:/pwd_strength pktprivacy secret name} {/.:/subsys/dce/pwd_mgmt/pwd_strength}}}}}] != 0} {return;}

    # Run the test
    test { if { [catch {account generate $PRINCIPAL} msg] == 0} {
               if {[string length $msg] != 0} {
                   list success
               } else {list fail}
           } else {list fail}
    } compare_strings success

    # Cleanup steps
    if {[dcp_wrap {principal delete $PRINCIPAL }] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4"
set ic1 "account_generate_7_P"
set ic2 "account_generate_8_P"
set ic3 "account_generate_9_P"
set ic4 "account_generate_10_P"
set tet_startup account_generate_startup
set tet_cleanup account_generate_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
