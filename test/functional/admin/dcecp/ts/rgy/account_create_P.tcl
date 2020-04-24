#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: account_create_P.tcl,v $
# Revision 1.1.6.4  1996/11/14  16:09:04  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	 account_create_P.tcl,v $
# 	Revision /main/DCE_1.2.2/5  1996/10/21  18:21 UTC  truitt
# 	Merge test changes.
# 	[1996/11/14  14:25:06  arvind]
#
# Revision /main/DCE_1.2.2/truitt_dcecp_pk7/1  1996/10/15  16:31 UTC  truitt
# 	CHFts20086: Add tests to check for newpassphrase consistency.
# 
# Revision /main/DCE_1.2.2/4  1996/09/11  18:49 UTC  truitt
# 	Merge test changes.
# 
# Revision /main/DCE_1.2.2/truitt_dcecp_pk5/1  1996/09/11  18:48 UTC  truitt
# 	Fix the uuid value of pkmechanism 'pkss'.
# 
# Revision /main/DCE_1.2.2/3  1996/08/20  16:35 UTC  truitt
# 	Merge test changes.
# 
# Revision /main/DCE_1.2.2/truitt_dcecp_pk2/1  1996/08/20  16:34 UTC  truitt
# 	Update pk tests.
# 
# Revision /main/DCE_1.2.2/2  1996/08/14  19:21 UTC  gilda
# 	dcecp work for PK login project
# 
# Revision /main/DCE_1.2.2/gilda_pk_dcecp/1 1996/08/14  18:37 UTC  gilda
# 	      dcecp work for Public key login
# 
# Revision /main/DCE_1.2.2/1  1996/05/24  23:32 UTC  sommerfeld
# 	merge u2u changes
# 	[1996/05/06  22:14 UTC  burati  /main/DCE_1.2/1]
# 
# 	u2u work off Mothra, before 1.2 branch is available
# 	[1996/04/29  22:07 UTC  burati  /main/mb_mothra8/1]
# 
# Revision 1.1.4.3  1996/03/11  02:18:33  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:56  marty]
# 
# Revision 1.1.4.2  1995/12/13  15:27:10  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:09  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:23  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:38  melman]
# 
# Revision 1.1.2.5  1994/09/01  17:49:20  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:32  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:02  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:59  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:38:48  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:23:11  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:34  melman
# 	updating to new API path
# 	[1994/05/06  17:13:57  melman]
# 
# Revision 1.1.2.1  1994/04/15  19:24:19  kevins
# 	CR 9938 add account functional tests
# 	[1994/04/15  15:31:18  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc account_create_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc account_create_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc account_create_45_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_45_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "description my_description" "group $GROUP" "organization $ORG" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]
    tet_infoline $LIST 

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL] description  
    } compare_strings "{my_description}"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_46_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_46_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "acctvalid no" "group $GROUP" "organization $ORG" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST  
        dcp_attrlist getval [account show $PRINCIPAL] acctvalid 
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_47_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_47_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "client no" "group $GROUP " "organization $ORG" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST  
        dcp_attrlist getval [account show $PRINCIPAL] client 
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_48_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_48_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "dupkey yes" "group $GROUP" "organization $ORG" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL] dupkey  
    } compare_strings yes

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_49_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_49_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "expdate 1999-04-13" "group $GROUP " "organization $ORG" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL] expdate 
    } compare_strings "1999-04-12-20:00:00.000-04:00I-----"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_50_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_50_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "forwardabletkt no" "group $GROUP" "organization $ORG" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL] forwardabletkt 
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_51_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_51_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "goodsince 1999-04-13" "group $GROUP" "organization $ORG" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL] goodsince
    } compare_strings "1999-04-12-20:00:00.000-04:00I-----"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_52_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_52_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "group $GROUP" "organization $ORG" "postdatedtkt yes" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL] postdatedtkt 
    } compare_strings yes

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_53_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_53_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "group $GROUP" "organization $ORG" "proxiabletkt yes" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL] proxiabletkt 
    } compare_strings yes

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_54_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_54_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "group $GROUP" "organization $ORG" "pwdvalid no" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL] pwdvalid  
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_55_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_55_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "group $GROUP" "organization $ORG" "renewabletkt no" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL] renewabletkt 
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_56_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_56_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "group $GROUP" "home hell" "organization $ORG" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL] home 
    } compare_strings hell

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_57_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_57_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "group $GROUP" "organization $ORG" "server no" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL] server 
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_58_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_58_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "group $GROUP" "organization $ORG" "shell help" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL] shell 
    } compare_strings help

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_59_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_59_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "group $GROUP" "organization $ORG" "stdtgtauth no" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL] stdtgtauth 
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_60_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_60_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "group $GROUP" "organization $ORG" "maxtktlife 21:0:0" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL -policies] maxtktlife
    } compare_strings "+0-21:00:00.000I-----"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_61_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_61_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "group $GROUP" "organization $ORG" "maxtktrenew 1:0:0" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"] 

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL -policies] maxtktrenew 
    } compare_strings "+0-01:00:00.000I-----"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_62_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_62_P: Check that we can create a list of accounts.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set PRINCIPAL1 [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append PRINCIPAL1 "foo"}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL1}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member "$PRINCIPAL $PRINCIPAL1"}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member "$PRINCIPAL $PRINCIPAL1"}] != 0} {return;}
    set LIST [list "group $GROUP" "organization $ORG" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create "$PRINCIPAL $PRINCIPAL1" -attribute $LIST
        dcp_found [account catalog -simplename] "$PRINCIPAL $PRINCIPAL1"
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {account delete "$PRINCIPAL $PRINCIPAL1"}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete "$PRINCIPAL $PRINCIPAL1"}] != 0} {return;}
}

proc account_create_63_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_63_P: Check that we can create an account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test { account create $PRINCIPAL -acctvalid no -client no -description "mine" -dupkey no \
           -expdate 1995-04-04 -forwardabletkt yes -goodsince 1995-04-04 -group $GROUP -home hell \
           -organization $ORG -postdatedtkt yes -proxiabletkt yes -pwdvalid no -renewabletkt yes \
           -server no -shell help -stdtgtauth no -maxtktlife 23:0:0 -maxtktrenew 1:0:0 \
           -passw $PRINCIPAL -mypwd $DCP_CLIENT_PW
        dcp_attrlist getval [account show $PRINCIPAL] home
    } compare_strings hell

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_64_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_64_P: Check that we can create an account with a list of attrs and u2u.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "usertouser yes" "group $GROUP" "organization $ORG" "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW"]

    # Run the test
    test { account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [account show $PRINCIPAL] usertouser 
    } compare_strings yes

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_65_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_65_P: Check that we can create an account with u2u no.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test { account create $PRINCIPAL -acctvalid no -client no -description "mine" -usertouser no \
           -expdate 1995-04-04 -forwardabletkt yes -goodsince 1995-04-04 -group $GROUP -home hell \
           -organization $ORG -postdatedtkt yes -proxiabletkt yes -pwdvalid no -renewabletkt yes \
           -server no -shell help -stdtgtauth no -maxtktlife 23:0:0 -maxtktrenew 1:0:0 \
           -passw $PRINCIPAL -mypwd $DCP_CLIENT_PW
        dcp_attrlist getval [account show $PRINCIPAL] home
    } compare_strings hell

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_66_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_66_P: Check that we can use 'pkmechanism' as an attribute with value 'file'. 

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test {account create $PRINCIPAL -description my_description -group $GROUP -organization $ORG \
          -pkmechanism file -password $PRINCIPAL -mypwd $DCP_CLIENT_PW
        dcp_attrlist getval [principal show $PRINCIPAL -all] DCEPKPrivateKeyStorage
    } compare_strings 8687c5b8-b01a-11cf-b137-0800090a5254

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_67_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_67_P: Check that we can use 'pkmechanism' as an attribute with value 'pkss'. 

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test {account create $PRINCIPAL -group $GROUP -organization $ORG \
          -pkmechanism pkss -password $PRINCIPAL -mypwd $DCP_CLIENT_PW 
        dcp_attrlist getval [principal show $PRINCIPAL -all] DCEPKPrivateKeyStorage
    } compare_strings 72053e72-b01a-11cf-8bf5-0800090a5254

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_68_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_68_P: Check that we can use 'pkmechanism' with -attribute with value 'file'.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "description my_description" "group $GROUP" "organization $ORG" \
                   "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW" "pkmechanism file"]
    tet_infoline $LIST 

    # Run the test
    test {account create $PRINCIPAL -attribute $LIST
        dcp_attrlist getval [principal show $PRINCIPAL -all] DCEPKPrivateKeyStorage
    } compare_strings 8687c5b8-b01a-11cf-b137-0800090a5254

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_69_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_69_P: Check that we can use 'pkmechanism' with -attribute with value 'pkss'.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    set LIST [list "description my_description" "group $GROUP" "organization $ORG" \
                   "password $PRINCIPAL" "mypwd $DCP_CLIENT_PW" "pkmechanism pkss"]
    tet_infoline $LIST 

    # Run the test
    test {account create $PRINCIPAL -attribute $LIST 
        dcp_attrlist getval [principal show $PRINCIPAL -all] DCEPKPrivateKeyStorage
    } compare_strings 72053e72-b01a-11cf-8bf5-0800090a5254

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_70_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_70_P: Check that 'generatekey' turned on and 'newpassphrase' specified. 

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test { account create $PRINCIPAL -description my_description -group $GROUP -organization $ORG \
           -password $PRINCIPAL -mypwd $DCP_CLIENT_PW -pksignature {{gen 1024} {new pokey}}
        dcp_attrlist getval [account show $PRINCIPAL] description 
    } compare_strings "{my_description}"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_71_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_71_P: Check that 'generatekey' turned on and 'newpassphrase' specified. 

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test { account create $PRINCIPAL -description my_description -group $GROUP -organization $ORG \
           -password $PRINCIPAL -mypwd $DCP_CLIENT_PW -pksignature {{gen default} {new pokey}}
        dcp_attrlist getval [account show $PRINCIPAL] description
    } compare_strings "{my_description}"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_72_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_72_P: Check that 'generatekey' turned on and 'newpassphrase' specified. 

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test { account create $PRINCIPAL -description my_description -group $GROUP -organization $ORG \
           -password $PRINCIPAL -mypwd $DCP_CLIENT_PW -pksignature {{gen 888} {new gumby}}
        dcp_attrlist getval [account show $PRINCIPAL] description  
    } compare_strings "{my_description}"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_73_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_73_P: Check that 'generatekey' turned on and 'newpassphrase' specified. 

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test { account create $PRINCIPAL -description my_description -group $GROUP -organization $ORG \
           -password $PRINCIPAL -mypwd $DCP_CLIENT_PW -pksignature {{gen default} {new gumby}}
        dcp_attrlist getval [account show $PRINCIPAL] description  
    } compare_strings "{my_description}"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_74_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_create_74_P: Check that we can use 'generatekey' off and both keyfiles, 'newpassphrase' specified.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test { account create $PRINCIPAL -description my_description -group $GROUP -organization $ORG \
           -pksignature "{public $TET_ROOT/$DCP_PK_DATA_1} {private $TET_ROOT/$DCP_PK_DATA_2} \
           {newpass pokey}" -password $PRINCIPAL -mypwd $DCP_CLIENT_PW 
        dcp_attrlist getval [account show $PRINCIPAL] description 
    } compare_strings "{my_description}"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_75_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_create_75_P: Check that we can use 'generatekey' off and both keyfiles, 'newpassphrase' specified.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    
    # Run the test
    test { account create $PRINCIPAL -description my_description -group $GROUP -organization $ORG \
           -pksignature "{generate 0} {public $TET_ROOT/$DCP_PK_DATA_1} {private $TET_ROOT/$DCP_PK_DATA_2} \
           {newpass pokey}" -password $PRINCIPAL -mypwd $DCP_CLIENT_PW 
        dcp_attrlist getval [account show $PRINCIPAL] description
    } compare_strings "{my_description}"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_76_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_create_76_P: Check that we can use 'generatekey' off and both keyfiles, 'newpassphrase' specified.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test { account create $PRINCIPAL -description my_description -group $GROUP -organization $ORG \
           -password $PRINCIPAL -mypwd $DCP_CLIENT_PW -pksignature "{public $TET_ROOT/$DCP_PK_DATA_1} \
           {private $TET_ROOT/$DCP_PK_DATA_2} {newpass pokey}"
        dcp_attrlist getval [account show $PRINCIPAL] description  
    } compare_strings "{my_description}"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_77_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_create_77_P: Check that we can use 'generatekey' off and both keyfiles, 'newpassphrase' specified.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test { account create $PRINCIPAL -description my_description -group $GROUP -organization $ORG \
           -password $PRINCIPAL -mypwd $DCP_CLIENT_PW -pksignature "{gen 0} {public $TET_ROOT/$DCP_PK_DATA_1} \
           {private $TET_ROOT/$DCP_PK_DATA_2} {newpass pokey}"
        dcp_attrlist getval [account show $PRINCIPAL] description  
    } compare_strings "{my_description}"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_78_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_78_P: If 'signature' and 'key cipher', both 'newpass' must match.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test { account create $PRINCIPAL -description my_description -group $GROUP -organization $ORG \
           -password $PRINCIPAL -mypwd $DCP_CLIENT_PW -pksignature {{gen 1024} {new gumby}} \
           -pkkeycipher {{gen 1024} {new gumby}}
        dcp_attrlist getval [account show $PRINCIPAL] description 
    } compare_strings "{my_description}"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_create_79_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_create_79_P: If 'signature' and 'key cipher', both 'newpass' must match.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test { account create $PRINCIPAL -attribute "{description my_description} {group $GROUP} \
           {organization $ORG} {password $PRINCIPAL} {mypwd $DCP_CLIENT_PW} \
           {pksignature {{gen 1024} {new gumby}}} \
           {pkkeycipher {{gen 1024} {new gumby}}}"
        dcp_attrlist getval [account show $PRINCIPAL] description 
    } compare_strings "{my_description}"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 "
set ic1 "account_create_45_P"
set ic2 "account_create_46_P"
set ic3 "account_create_47_P"
set ic4 "account_create_48_P"
set ic5 "account_create_49_P"
set ic6 "account_create_50_P"
set ic7 "account_create_51_P"
set ic8 "account_create_52_P"
set ic9 "account_create_53_P"
set ic10 "account_create_54_P"
set ic11 "account_create_55_P"
set ic12 "account_create_56_P"
set ic13 "account_create_57_P"
set ic14 "account_create_58_P"
set ic15 "account_create_59_P"
set ic16 "account_create_60_P"
set ic17 "account_create_61_P"
set ic18 "account_create_62_P"
set ic19 "account_create_63_P"
set ic20 "account_create_64_P"
set ic21 "account_create_65_P"
set ic22 "account_create_66_P"
set ic23 "account_create_67_P"
set ic24 "account_create_68_P"
set ic25 "account_create_69_P"
set ic26 "account_create_70_P"
set ic27 "account_create_71_P"
set ic28 "account_create_72_P"
set ic29 "account_create_73_P"
set ic30 "account_create_74_P"
set ic31 "account_create_75_P"
set ic32 "account_create_76_P"
set ic33 "account_create_77_P"
set ic34 "account_create_78_P"
set ic35 "account_create_79_P"

set tet_startup account_create_startup
set tet_cleanup account_create_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
