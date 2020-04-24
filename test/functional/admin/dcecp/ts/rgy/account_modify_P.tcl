#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: account_modify_P.tcl,v $
# Revision 1.1.6.3  1996/11/14  16:09:34  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	 account_modify_P.tcl,v $
# 	Revision /main/DCE_1.2.2/6  1996/10/21  18:18 UTC  truitt
# 	Merge test changes.
# 	[1996/11/14  14:25:41  arvind]
#
# Revision /main/DCE_1.2.2/truitt_dcecp_pk7/1  1996/10/15  16:32 UTC  truitt
# 	CHFts20086: Add tests to check for newpassphrase consistency.
# 
# Revision /main/DCE_1.2.2/5  1996/09/11  18:51 UTC  truitt
# 	Merge test changes.
# 
# Revision /main/DCE_1.2.2/truitt_dcecp_pk5/1  1996/09/11  18:50 UTC  truitt
# 	Fix the uuid value of pkmechanism 'pkss'.
# 
# Revision /main/DCE_1.2.2/4  1996/09/03  17:08 UTC  truitt
# 	Merge test changes.
# 
# Revision /main/DCE_1.2.2/truitt_dcecp_pk3/1  1996/09/03  17:06 UTC  truitt
# 	Add additional checks to some of the pk tests.
# 
# Revision /main/DCE_1.2.2/3  1996/08/20  16:40 UTC  truitt
# 	Merge test changes.
# 
# Revision /main/DCE_1.2.2/truitt_dcecp_pk2/1  1996/08/20  16:38 UTC  truitt
# 	Update pk tests.
# 
# Revision /main/DCE_1.2.2/2  1996/08/14  19:21 UTC  gilda
# 	dcecp work for PK login project
# 
# Revision /main/DCE_1.2.2/gilda_pk_dcecp/1 1996/08/14  18:49 UTC  gilda 
# 	      dcecp work for Public key login
# 
# Revision /main/DCE_1.2.2/1  1996/05/24  23:32 UTC  sommerfeld
# 	merge u2u changes
# 	[1996/05/06  22:18 UTC  burati  /main/DCE_1.2/2]
# 
# 	u2u work off Mothra, before 1.2 branch is available
# 	[1996/04/29  22:07 UTC  burati  /main/HPDCE02/mb_mothra8/1]
# 
# Revision 1.1.4.3  1996/03/11  02:18:49  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:01  marty]
# 
# Revision 1.1.4.2  1995/12/13  15:27:16  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/05/01  14:34 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts26/1  1995/05/01  14:31 UTC  truitt
# 	Add some tests.
# 	[1995/12/11  15:54:14  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:32  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:47  melman]
# 
# Revision 1.1.2.5  1994/09/01  17:49:24  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:37  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:06  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:04  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:38:52  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:23:34  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:37  melman
# 	updating to new API path
# 	[1994/05/06  17:14:03  melman]
# 
# Revision 1.1.2.1  1994/04/15  19:24:24  kevins
# 	CR 9938 add account functional tests
# 	[1994/04/15  15:31:22  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc account_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc account_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc account_modify_4_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_4_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW }] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -acctvalid no
        dcp_attrlist getval [account show $PRINCIPAL] acctvalid
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_5_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_5_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW }] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -client no
        dcp_attrlist getval [account show $PRINCIPAL] client
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
}

proc account_modify_6_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_6_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW }] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -description help
        dcp_attrlist getval [account show $PRINCIPAL] description
    } compare_strings "{help}"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_7_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_7_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW }] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -dupkey yes
        dcp_attrlist getval [account show $PRINCIPAL] dupkey
    } compare_strings yes

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $ORG}] != 0} {return;}
}

proc account_modify_8_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_8_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW }] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -expdate 1995-01-01
        dcp_attrlist getval [account show $PRINCIPAL] expdate
    } compare_strings "1994-12-31-19:00:00.000-05:00I-----"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $ORG}] != 0} {return;}
}

proc account_modify_9_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_9_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -forwardabletkt no
        dcp_attrlist getval [account show $PRINCIPAL] forwardabletkt
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $ORG}] != 0} {return;}
}

proc account_modify_10_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_10_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -goodsince 1995-01-01
        dcp_attrlist getval [account show $PRINCIPAL] goodsince 
    } compare_strings "1994-12-31-19:00:00.000-05:00I-----"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $ORG}] != 0} {return;}
}

proc account_modify_11_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_11_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group add none -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -group none
        dcp_attrlist getval [account show $PRINCIPAL] group
    } compare_strings none

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group remove none -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_12_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_12_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -home hell
        dcp_attrlist getval [account show $PRINCIPAL] home
    } compare_strings hell

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_13_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_13_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -organization none
        dcp_attrlist getval [account show $PRINCIPAL] organization
    } compare_strings none

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization remove none -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_14_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_14_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -postdatedtkt no
        dcp_attrlist getval [account show $PRINCIPAL] postdatedtkt
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_15_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_15_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -proxiabletkt no
        dcp_attrlist getval [account show $PRINCIPAL] proxiabletkt
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_16_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_16_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -pwdvalid no 
        dcp_attrlist getval [account show $PRINCIPAL] pwdvalid
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_17_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_17_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -renewabletkt no
        dcp_attrlist getval [account show $PRINCIPAL] renewabletkt
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_18_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_18_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -server no
        dcp_attrlist getval [account show $PRINCIPAL] server
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_19_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_19_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -shell hell
        dcp_attrlist getval [account show $PRINCIPAL] shell
    } compare_strings hell

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_20_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_20_P: Check that we can create an account with a list of attributes.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -renewabletkt no
        dcp_attrlist getval [account show $PRINCIPAL] renewabletkt
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_21_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_21_P: Check that we can create a list of  accounts.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set PRINCIPAL1 [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append PRINCIPAL1 "foo"}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create "$PRINCIPAL $PRINCIPAL1"}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member "$PRINCIPAL $PRINCIPAL1"}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member "$PRINCIPAL $PRINCIPAL1"}] != 0} {return;}
    if {[dcp_wrap {account create "$PRINCIPAL $PRINCIPAL1" -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { account modify "$PRINCIPAL $PRINCIPAL1" -stdtgtauth no
        dcp_attrlist getval [account show $PRINCIPAL] stdtgtauth
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {account delete "$PRINCIPAL $PRINCIPAL1"}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete "$PRINCIPAL $PRINCIPAL1"}] != 0} {return;}
}

proc account_modify_22_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_22_P: Check that we can create an account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW }] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -maxtktlife 1:0:0
        dcp_attrlist getval [account show $PRINCIPAL -policies ] maxtktlife
    } compare_strings "+0-01:00:00.000I-----"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_23_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_23_P: Check that we can create an account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW }] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -maxtktrenew 1:0:0
        dcp_attrlist getval [account show $PRINCIPAL -policies] maxtktrenew
    } compare_strings "+0-01:00:00.000I-----"

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_24_P {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_24_P: Modify group and org tests

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create newgroup24}] != 0} {return;}
    if {[dcp_wrap {group add newgroup24 -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test {account modify $PRINCIPAL -group newgroup24 -home /.:
        dcp_attrlist getval [account show $PRINCIPAL] group
    } compare_strings newgroup24

    # Cleanup steps
    if {[dcp_wrap {group delete newgroup24}] != 0} {return;}
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_25_P {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_25_P: Modify group and org tests

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {organization create neworg25}] != 0} {return;}
    if {[dcp_wrap {organization add neworg25 -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test {account modify $PRINCIPAL -org neworg25 -home /.:
        dcp_attrlist getval [account show $PRINCIPAL] organization
    } compare_strings neworg25

    # Cleanup steps
    if {[dcp_wrap {organization delete neworg25}] != 0} {return;}
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_26_P {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_26_P: Modify group and org tests

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {organization create neworg26}] != 0} {return;}
    if {[dcp_wrap {organization add neworg26 -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test {account modify $PRINCIPAL -group $GROUP -org neworg26 -home /.:
        dcp_attrlist getval [account show $PRINCIPAL] organization
    } compare_strings neworg26

    # Cleanup steps
    if {[dcp_wrap {organization delete neworg26}] != 0} {return;}
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_27_P {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_27_P: Modify group and org tests

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create newgroup27}] != 0} {return;}
    if {[dcp_wrap {group add newgroup27 -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test {account modify $PRINCIPAL -group newgroup27 -org $ORG -home /.:
        dcp_attrlist getval [account show $PRINCIPAL] group
    } compare_strings newgroup27

    # Cleanup steps
    if {[dcp_wrap {group delete newgroup27}] != 0} {return;}
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_28_P {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_28_P: Modify group and org tests

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group add none -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {account modify $PRINCIPAL -group $GROUP -org $ORG -home /.:
        dcp_attrlist getval [account show $PRINCIPAL] group
    } compare_strings $GROUP

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group remove none -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_29_P {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_29_P: Modify group and org tests

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create newgroup29}] != 0} {return;}
    if {[dcp_wrap {group add newgroup29 -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create neworg29}] != 0} {return;}
    if {[dcp_wrap {organization add neworg29 -member $PRINCIPAL}] != 0} {return;}

    # Run the test
    test {account modify $PRINCIPAL -group newgroup29 -org neworg29 -home /.:
        dcp_attrlist getval [account show $PRINCIPAL] group
    } compare_strings newgroup29

    # Cleanup steps
    if {[dcp_wrap {organization delete neworg29}] != 0} {return;}
    if {[dcp_wrap {group delete newgroup29}] != 0} {return;}
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group remove none -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_30_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_30_P: Check that we can modify an account with a list of attrs and u2u.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW }] != 0} {return;}

    # Run the test
    test { account modify $PRINCIPAL -usertouser yes
        dcp_attrlist getval [account show $PRINCIPAL] usertouser
    } compare_strings yes

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $ORG}] != 0} {return;}
}

proc account_modify_31_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_31_P: Check that we can use 'pkmechanism' as an attribute with value 'file'.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW }] != 0} {return;}

    # Run the test
    test {account modify $PRINCIPAL -pkmechanism file 
        dcp_attrlist getval [principal show $PRINCIPAL -all] DCEPKPrivateKeyStorage
    } compare_strings 8687c5b8-b01a-11cf-b137-0800090a5254

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_32_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_32_P: Check that we can use 'pkmechanism' as an attribute with value 'file'.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW }] != 0} {return;}

    # Run the test
    test {account modify $PRINCIPAL -pkmechanism pkss 
        dcp_attrlist getval [principal show $PRINCIPAL -all] DCEPKPrivateKeyStorage
    } compare_strings 72053e72-b01a-11cf-8bf5-0800090a5254

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_33_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_33_P: Check that we can use 'pkmechanism' as an attribute with value 'file'.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW }] != 0} {return;}

    # Run the test
    test {account modify $PRINCIPAL -change {pkmechanism file}
        dcp_attrlist getval [principal show $PRINCIPAL -all] DCEPKPrivateKeyStorage
    } compare_strings 8687c5b8-b01a-11cf-b137-0800090a5254

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_34_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_34_P: Check that we can use 'pkmechanism' as an attribute with value 'file'.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW }] != 0} {return;}

    # Run the test
    test {account modify $PRINCIPAL -change {pkmechanism pkss}
        dcp_attrlist getval [principal show $PRINCIPAL -all] DCEPKPrivateKeyStorage
    } compare_strings 72053e72-b01a-11cf-8bf5-0800090a5254

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_35_P {} {
    global DCP_CLIENT_PW DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline account_modify_35_P: Modify the account with pkgenprivkey.
    tet_infoline account_modify_35_P: Existing principal has no DCEPK ERA values.

    # Setup steps
    set RESIDUAL_NAME [string range $DCP_CELLNAME_ONE 5 end]
    set PRINCIPAL $DCP_CELLNAME_ONE/krbtgt/$RESIDUAL_NAME
    set PKOLD ""
    set PKNEW ""
    set PKSTORE ""
    catch {principal modify $PRINCIPAL -remove {DCEPKAuthentication DCEPKPrivateKeyStorage} -types} msg
    catch {set PKOLD [attrlist getvalues [principal show $PRINCIPAL -all] -type DCEPKAuthentication]} msg

    # Run the test
    if {[catch {account modify $PRINCIPAL -pkgenprivkey 1024 -mypwd $DCP_CLIENT_PW} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $PRINCIPAL
        tet_infoline $msg
        tet_result FAIL
    } else {
        dcp_wrap {set PKNEW [attrlist getvalues [principal show $PRINCIPAL -all] \
                  -type DCEPKAuthentication]}
        tet_infoline Check that DCEPKAuthentication ERA was set correctly
        if {$PKOLD == $PKNEW} {
            tet_infoline PK Authentication information remained the same after modify
            tet_infoline Unable to modify the account
            tet_result FAIL
        } else {
            dcp_wrap {set PKSTORE [attrlist getvalues [principal show $PRINCIPAL -all] \
                      -type DCEPKPrivateKeyStorage]}
            tet_infoline Check that DCEPKPrivateKeyStorage ERA was set correctly
            if {$PKSTORE != "adb48ed4-e94d-11cf-ab4b-08000919ebb5"} {
                tet_infoline PK Storage type of $PKSTORE is incorrect
                tet_infoline Unable to modify the account
                tet_result FAIL
            } else {
                tet_infoline Able to correctly modify the account
                tet_result PASS
            }
        }
    }

    # Cleanup steps
}

proc account_modify_36_P {} {
    global DCP_CLIENT_PW DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline account_modify_36_P: Modify the account using pkgenprivkey.
    tet_infoline account_modify_36_P: Existing principal has DCEPK ERA values.

    # Setup steps
    set RESIDUAL_NAME [string range $DCP_CELLNAME_ONE 5 end]
    set PRINCIPAL $DCP_CELLNAME_ONE/krbtgt/$RESIDUAL_NAME
    set PKOLD ""
    set PKNEW ""
    set PKSTORE ""
    dcp_wrap {set PKOLD [attrlist getvalues [principal show $PRINCIPAL -all] -type DCEPKAuthentication]}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pkgenprivkey default -mypwd $DCP_CLIENT_PW} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        dcp_wrap {set PKNEW [attrlist getvalues [principal show $PRINCIPAL -all] \
                  -type DCEPKAuthentication]}
        tet_infoline Check that DCEPKAuthentication ERA was set correctly
        if {$PKOLD == $PKNEW} {
            tet_infoline PK Authentication information remained the same after modify
            tet_infoline Unable to modify the account
            tet_result FAIL
        } else {
            dcp_wrap {set PKSTORE [attrlist getvalues [principal show $PRINCIPAL -all] \
                      -type DCEPKPrivateKeyStorage]}
            tet_infoline Check that DCEPKPrivateKeyStorage ERA was set correctly
            if {$PKSTORE != "adb48ed4-e94d-11cf-ab4b-08000919ebb5"} {
                tet_infoline PK Storage type of $PKSTORE is incorrect
                tet_infoline Unable to modify the account
                tet_result FAIL
            } else {
                tet_infoline Able to correctly modify the account
                tet_result PASS
            }
        }
    }

    # Cleanup steps
}

proc account_modify_37_P {} {
    global DCP_CLIENT_PW DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline account_modify_37_P: Modify the account using pkgenprivkey.
    tet_infoline account_modify_37_P: Existing principal has DCEPK ERA values.

    # Setup steps
    set RESIDUAL_NAME [string range $DCP_CELLNAME_ONE 5 end]
    set PRINCIPAL $DCP_CELLNAME_ONE/krbtgt/$RESIDUAL_NAME
    set PKOLD ""
    set PKNEW ""
    set PKSTORE ""
    dcp_wrap {set PKOLD [attrlist getvalues [principal show $PRINCIPAL -all] -type DCEPKAuthentication]}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change "{pkgenprivkey 868} {mypwd $DCP_CLIENT_PW}"} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        dcp_wrap {set PKNEW [attrlist getvalues [principal show $PRINCIPAL -all] \
                  -type DCEPKAuthentication]}
        tet_infoline Check that DCEPKAuthentication ERA was set correctly
        if {$PKOLD == $PKNEW} {
            tet_infoline PK Authentication information remained the same after modify
            tet_infoline Unable to modify the account
            tet_result FAIL
        } else {
            dcp_wrap {set PKSTORE [attrlist getvalues [principal show $PRINCIPAL -all] \
                      -type DCEPKPrivateKeyStorage]}
            tet_infoline Check that DCEPKPrivateKeyStorage ERA was set correctly
            if {$PKSTORE != "adb48ed4-e94d-11cf-ab4b-08000919ebb5"} {
                tet_infoline PK Storage type of $PKSTORE is incorrect
                tet_infoline Unable to modify the account
                tet_result FAIL
            } else {
                tet_infoline Able to correctly modify the account
                tet_result PASS
            }
        }
    }

    # Cleanup steps
}

proc account_modify_38_P {} {
    global DCP_CLIENT_PW DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline account_modify_38_P: Modify the account using pkgenprivkey.
    tet_infoline account_modify_38_P: Existing principal has DCEPK ERA values.

    # Setup steps
    set RESIDUAL_NAME [string range $DCP_CELLNAME_ONE 5 end]
    set PRINCIPAL $DCP_CELLNAME_ONE/krbtgt/$RESIDUAL_NAME
    set PKOLD ""
    set PKNEW ""
    set PKSTORE ""
    dcp_wrap {set PKOLD [attrlist getvalues [principal show $PRINCIPAL -all] -type DCEPKAuthentication]}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change "{pkgenprivkey default} {mypwd $DCP_CLIENT_PW}"} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        dcp_wrap {set PKNEW [attrlist getvalues [principal show $PRINCIPAL -all] \
                  -type DCEPKAuthentication]}
        tet_infoline Check that DCEPKAuthentication ERA was set correctly
        if {$PKOLD == $PKNEW} {
            tet_infoline PK Authentication information remained the same after modify
            tet_infoline Unable to modify the account
            tet_result FAIL
        } else {
            dcp_wrap {set PKSTORE [attrlist getvalues [principal show $PRINCIPAL -all] \
                      -type DCEPKPrivateKeyStorage]}
            tet_infoline Check that DCEPKPrivateKeyStorage ERA was set correctly
            if {$PKSTORE != "adb48ed4-e94d-11cf-ab4b-08000919ebb5"} {
                tet_infoline PK Storage type of $PKSTORE is incorrect
                tet_infoline Unable to modify the account
                tet_result FAIL
            } else {
                tet_infoline Able to correctly modify the account
                tet_result PASS
            }
        }
    }

    # Cleanup steps
}

proc account_modify_39_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_39_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pkgenprivkey 0} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_40_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_40_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -password pokey -mypwd $DCP_CLIENT_PW -pkgenprivkey 0} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_41_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_41_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change {pkgenprivkey 0}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_42_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_42_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change "{password pokey} {mypwd $DCP_CLIENT_PW} {pkgenprivkey 0}"} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_43_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_43_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW -pksignature {{gen 1024} {newpass pokey}}}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature {{gen 1024} {oldpass pokey}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_44_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_44_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW -pksignature {{gen default} {newpass pokey}}}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature {{gen default} {oldpass pokey}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_45_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_45_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW -pksignature {{gen 1024} {newpass gumby}}}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change {pksignature {{gen 1024} {oldpass gumby}}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_46_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_46_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW -pksignature {{gen default} {newpass gumby}}}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change {pksignature {{gen default} {oldpass gumby}}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_47_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_47_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature {{gen 1001} {newpass pokey}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_48_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_48_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature {{gen default} {newpass pokey}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_49_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_49_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change {pksignature {{gen 1001} {newpass gumby}}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_50_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_50_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change {pksignature {{gen default} {newpass gumby}}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_51_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_51_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW -pksignature {{generate 1001} {newpass pokey}}}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature {{generate 1001} {oldpass pokey} {newpass pokey2}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_52_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_52_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG -password $PRINCIPAL \
                   -mypwd $DCP_CLIENT_PW -pksignature {{generate default} {newpass pokey}}}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature {{generate default} {oldpass pokey} {newpass pokey2}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_53_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_53_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG -password $PRINCIPAL \
                   -mypwd $DCP_CLIENT_PW -pksignature {{generate 1001} {newpass gumby}}}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change {pksignature {{generate 1001} {oldpass gumby} {newpass pokey}}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_54_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_54_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG -password $PRINCIPAL \
                   -mypwd $DCP_CLIENT_PW -pksignature {{generate default} {newpass gumby}}}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change {pksignature {{generate default} {oldpass gumby} \
                {newpass pokey}}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_55_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_55_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG -password $PRINCIPAL \
                   -mypwd $DCP_CLIENT_PW -pksignature "{public $TET_ROOT/$DCP_PK_DATA_1} \
                   {private $TET_ROOT/$DCP_PK_DATA_2} {newpass pokey}"}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature "{public $TET_ROOT/$DCP_PK_DATA_1} \
                {private $TET_ROOT/$DCP_PK_DATA_2} {oldpass pokey}"} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_56_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_56_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG -password $PRINCIPAL -mypwd $DCP_CLIENT_PW \
                   -pksignature "{generate 0} {public $TET_ROOT/$DCP_PK_DATA_1} {private $TET_ROOT/$DCP_PK_DATA_2} \
                   {newpass pokey}"}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature "{generate 0} {public $TET_ROOT/$DCP_PK_DATA_1} \
                {private $TET_ROOT/$DCP_PK_DATA_2} {oldpass pokey}"} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_57_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_57_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG -password $PRINCIPAL -mypwd $DCP_CLIENT_PW \
                   -pksignature "{public $TET_ROOT/$DCP_PK_DATA_1} {private $TET_ROOT/$DCP_PK_DATA_2} \
                   {newpass pokey}"}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change "pksignature {{public $TET_ROOT/$DCP_PK_DATA_1} \
                {private $TET_ROOT/$DCP_PK_DATA_2} {oldpass pokey}}"} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_58_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_58_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG -password $PRINCIPAL -mypwd $DCP_CLIENT_PW \
                   -pksignature "{gen 0} {public $TET_ROOT/$DCP_PK_DATA_1} {private $TET_ROOT/$DCP_PK_DATA_2} \
                   {newpass pokey}"}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change "pksignature {{gen 0} {public $TET_ROOT/$DCP_PK_DATA_1} \
                {private $TET_ROOT/$DCP_PK_DATA_2} {oldpass pokey}}"} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_59_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_59_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature "{public $TET_ROOT/$DCP_PK_DATA_1} \
                {private $TET_ROOT/$DCP_PK_DATA_2} {newpass pokey}"} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_60_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_60_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature "{generate 0} {public $TET_ROOT/$DCP_PK_DATA_1} \
                {private $TET_ROOT/$DCP_PK_DATA_2} {newpass pokey}"} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_61_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_61_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change "pksignature {{public $TET_ROOT/$DCP_PK_DATA_1} \
                {private $TET_ROOT/$DCP_PK_DATA_2} {newpass pokey}}"} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_62_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_62_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change "pksignature {{gen 0} {public $TET_ROOT/$DCP_PK_DATA_1} \
                {private $TET_ROOT/$DCP_PK_DATA_2} {newpass pokey}}"} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_63_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_63_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG -password $PRINCIPAL -mypwd $DCP_CLIENT_PW \
                   -pksignature "{public $TET_ROOT/$DCP_PK_DATA_1} {private $TET_ROOT/$DCP_PK_DATA_2} \
                   {newpass gumby}"}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature "{oldpass gumby} {newpass pokey} \
                {publickeyfile $TET_ROOT/$DCP_PK_DATA_1} {privatekeyfile $TET_ROOT/$DCP_PK_DATA_2}"} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_64_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_64_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG -password $PRINCIPAL -mypwd $DCP_CLIENT_PW \
                   -pksignature "{generate 0} {public $TET_ROOT/$DCP_PK_DATA_1} {private $TET_ROOT/$DCP_PK_DATA_2} \
                   {newpass gumby}"}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature "{generate 0} {oldpass gumby} {newpass pokey} \
                {publickeyfile $TET_ROOT/$DCP_PK_DATA_1} {privatekeyfile $TET_ROOT/$DCP_PK_DATA_2}"} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_65_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_65_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG -password $PRINCIPAL -mypwd $DCP_CLIENT_PW \
                   -pksignature "{public $TET_ROOT/$DCP_PK_DATA_1} {private $TET_ROOT/$DCP_PK_DATA_2} \
                   {newpass gumby}"}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change "pksignature {{oldpass gumby} {newpass pokey} \
                {publickeyfile $TET_ROOT/$DCP_PK_DATA_1} {privatekeyfile $TET_ROOT/$DCP_PK_DATA_2}}"} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_66_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_66_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG -password $PRINCIPAL -mypwd $DCP_CLIENT_PW \
                   -pksignature "{generate 0} {public $TET_ROOT/$DCP_PK_DATA_1} {private $TET_ROOT/$DCP_PK_DATA_2} \
                   {newpass gumby}"}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change "pksignature {{generate 0} {oldpass gumby} {newpass pokey} \
                {publickeyfile $TET_ROOT/$DCP_PK_DATA_1} {privatekeyfile $TET_ROOT/$DCP_PK_DATA_2}}"} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_67_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_67_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG -password $PRINCIPAL \
                   -mypwd $DCP_CLIENT_PW -pksignature {{gen default} {new pokey}}}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature {{oldpass pokey} {newpass pokey2}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_68_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_68_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG -password $PRINCIPAL -mypwd $DCP_CLIENT_PW \
                   -pksignature "{generate 0} {public $TET_ROOT/$DCP_PK_DATA_1} {private $TET_ROOT/$DCP_PK_DATA_2} \
                   {newpass pokey}"}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature {{generate 0} {oldpass pokey} {newpass pokey2}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_69_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_69_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG -password $PRINCIPAL -mypwd $DCP_CLIENT_PW \
                   -pksignature "{public $TET_ROOT/$DCP_PK_DATA_1} {private $TET_ROOT/$DCP_PK_DATA_2} \
                   {newpass gumby}"}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change {pksignature {{oldpass gumby} {newpass pokey}}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_70_P {} {
    global TET_ROOT DCP_CLIENT_PW DCP_PK_DATA_1 DCP_PK_DATA_2

    # Identify the test
    tet_infoline account_modify_70_P: Modify the account.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG -password $PRINCIPAL -mypwd $DCP_CLIENT_PW \
                   -pksignature "{generate 0} {public $TET_ROOT/$DCP_PK_DATA_1} {private $TET_ROOT/$DCP_PK_DATA_2} \
                   {newpass gumby}"}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change {pksignature {{generate 0} {oldpass gumby} {newpass pokey}}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_71_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_71_P: If 'signature' and 'key cipher', both 'newpass' must match.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature {{gen 528} {newpass gumby}} \
                -pkkeycipher {{gen 528} {newpass gumby}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_72_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_72_P: If 'signature' and 'key cipher', both 'newpass' must match.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -change {{pksignature {{gen 528} {newpass gumby}}} \
                {pkkeycipher {{gen 528} {newpass gumby}}}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_73_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_73_P: If 'signature' and 'key cipher', both 'newpass' must match.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW \
                   -pksignature {{generate 256} {newpass pokey}}}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pkkeycipher {{gen 528} {newpass pokey}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_modify_74_P {} {
    global DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_modify_74_P: If 'signature' and 'key cipher', both 'newpass' must match.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {account create $PRINCIPAL -group $GROUP -organization $ORG \
                   -password $PRINCIPAL -mypwd $DCP_CLIENT_PW \
                   -pkkeycipher {{generate 256} {newpass pokey}}}] != 0} {return;}

    # Run the test
    if {[catch {account modify $PRINCIPAL -pksignature {{gen 528} {newpass pokey}}} msg] != 0} {
        tet_infoline Unable to modify the account
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to modify the account
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {account delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 ic38 ic39 ic40 ic41 ic42 ic43 ic44 ic45 ic46 ic47 ic48 ic49 ic50 ic51 ic52 ic53 ic54 ic55 ic56 ic57 ic58 ic59 ic60 ic61 ic62 ic63 ic64 ic65 ic66 ic67 ic68 ic69 ic70 ic71 "
set ic1 "account_modify_4_P"
set ic2 "account_modify_5_P"
set ic3 "account_modify_6_P"
set ic4 "account_modify_7_P"
set ic5 "account_modify_8_P"
set ic6 "account_modify_9_P"
set ic7 "account_modify_10_P"
set ic8 "account_modify_11_P"
set ic9 "account_modify_12_P"
set ic10 "account_modify_13_P"
set ic11 "account_modify_14_P"
set ic12 "account_modify_15_P"
set ic13 "account_modify_16_P"
set ic14 "account_modify_17_P"
set ic15 "account_modify_18_P"
set ic16 "account_modify_19_P"
set ic17 "account_modify_20_P"
set ic18 "account_modify_21_P"
set ic19 "account_modify_22_P"
set ic20 "account_modify_23_P"
set ic21 "account_modify_24_P"
set ic22 "account_modify_25_P"
set ic23 "account_modify_26_P"
set ic24 "account_modify_27_P"
set ic25 "account_modify_28_P"
set ic26 "account_modify_29_P"
set ic27 "account_modify_30_P"
set ic28 "account_modify_31_P"
set ic29 "account_modify_32_P"
set ic30 "account_modify_33_P"
set ic31 "account_modify_34_P"
set ic32 "account_modify_35_P"
set ic33 "account_modify_36_P"
set ic34 "account_modify_37_P"
set ic35 "account_modify_38_P"
set ic36 "account_modify_39_P"
set ic37 "account_modify_40_P"
set ic38 "account_modify_41_P"
set ic39 "account_modify_42_P"
set ic40 "account_modify_43_P"
set ic41 "account_modify_44_P"
set ic42 "account_modify_45_P"
set ic43 "account_modify_46_P"
set ic44 "account_modify_47_P"
set ic45 "account_modify_48_P"
set ic46 "account_modify_49_P"
set ic47 "account_modify_50_P"
set ic48 "account_modify_51_P"
set ic49 "account_modify_52_P"
set ic50 "account_modify_53_P"
set ic51 "account_modify_54_P"
set ic52 "account_modify_55_P"
set ic53 "account_modify_56_P"
set ic54 "account_modify_57_P"
set ic55 "account_modify_58_P"
set ic56 "account_modify_59_P"
set ic57 "account_modify_60_P"
set ic58 "account_modify_61_P"
set ic59 "account_modify_62_P"
set ic60 "account_modify_63_P"
set ic61 "account_modify_64_P"
set ic62 "account_modify_65_P"
set ic63 "account_modify_66_P"
set ic64 "account_modify_67_P"
set ic65 "account_modify_68_P"
set ic66 "account_modify_69_P"
set ic67 "account_modify_70_P"
set ic68 "account_modify_71_P"
set ic69 "account_modify_72_P"
set ic70 "account_modify_73_P"
set ic71 "account_modify_74_P"

set tet_startup account_modify_startup
set tet_cleanup account_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
