# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: dcd_st_utils.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:38:00  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:11  marty]
#
# Revision 1.1.4.1  1995/12/11  21:55:04  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:55:13  root]
# 
# Revision 1.1.2.5  1994/10/05  15:26:09  fadden
# 	Add procedure to return successful command message output
# 	[1994/10/05  15:22:05  fadden]
# 
# Revision 1.1.2.4  1994/09/30  21:57:09  fadden
# 	Add procedures to display the commands before executing
# 	them and check and display the results after executing them.
# 
# 	Increase maintainability by adding routines to execute and
# 	display commands and check and report on results.
# 	Revision 1.1.2.3  1994/08/10  19:23:19  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:17:35  annie]
# 
# Revision 1.1.2.2  1994/07/26  21:41:45  fadden
# 	Modify parse_args to create new output file rather than appending
# 	Modify error reporting for use in standard system testing setup
# 	[1994/07/26  19:37:08  fadden]
# 
# Revision 1.1.2.1  1994/06/27  18:26:42  baum
# 	Fixed install path and moved setup and misc.tcl files to lib directory
# 	[1994/06/27  18:26:16  baum]
# 
# $EndLog$

#
# dcd_st_utils.tcl - Module containing common TCL routines for System
#		      Test Scripts.

set T_CELL_ADMIN 	cell_admin
set T_CELL_ADMIN_PWD 	-dce-

#
# For now, set TEST_PRINC to cell_admin.
#
set TEST_PRINC $T_CELL_ADMIN
set TEST_PRINC_PWD $T_CELL_ADMIN_PWD

#
# The WHO variable is specific to the ACL tests.  It is here because the 
# systest_setup.tcl script uses it to prep the dced ACLs.
#
set WHO			user:$TEST_PRINC

proc tet_infoline { string } {

 global RESULTS_FILE_HND TEST_NAME

 puts $RESULTS_FILE_HND "$TEST_NAME: $string"
}

#
#----------- Utility Procedures ---------------------------------------
#

proc tet_result { string } {

  global RESULTS_FILE_HND TEST_NAME

  puts $RESULTS_FILE_HND "$TEST_NAME: $string"
}

proc unwanted_fail_proc {} {
  uplevel {
    tet_infoline "FATAL: $cmd"
    tet_infoline "FATAL: Command failed when it should not have. ($msg)"
    systest_cleanup
  }
}

proc unwanted_success_proc {} {
  uplevel {
    tet_infoline "FATAL: $cmd"
    tet_infoline "FATAL: Command succeeded when ACL should have disallowed it"
    systest_cleanup
  }
}

proc not_bad_perms_proc {} {
  uplevel {
    tet_infoline "FATAL: $cmd"
    tet_infoline "FATAL: Command failed as expected but not due to permissions problems. ($msg)"
    systest_cleanup
  }
}

proc parse_args {} {
  uplevel {
    set TGT_CELL_HOST [lindex $argv 0]
    if { $argc != 2 } {
      if { $argc != 1 } {
        puts stdout "Usage: $argv0 /.:/hosts/TGT_HOST \[result_file\]"
        exit -1
      } else {
        puts stdout "Test output going to stdout"
        set RESULTS_FILE_HND stdout
      }
    } else {
      set RESULTS_FILE [lindex $argv 1]
      set RESULTS_FILE_HND [open $RESULTS_FILE w]
      if { $RESULTS_FILE_HND == -1 } {
        puts stdout "Could not open results file - $RESULTS_FILE"
        exit -1
      }
    }
  }  
}


# execute a command and catch errors
# force return from caller on error
proc doit {cmd} {

    tet_infoline "Executing $cmd"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
        tet_infoline "FATAL: $cmd"
        tet_infoline "FATAL: Command failed when it should not have. ($msg)"
	return -code error 1
    } else {
	return 0
    }
}

# execute a command and catch errors
# force return from caller on error
# return command's output message if no error
proc doit_ret_msg {cmd} {
    tet_infoline "Executing $cmd"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
        tet_infoline "FATAL: $cmd"
        tet_infoline "FATAL: Command failed when it should not have. ($msg)"
	return -code error 1
    } else {
	return $msg
    }
}

# execute a server ping command and catch errors
# ck response from ping - expect success
# do not force return from caller on error

proc doit_ping_success {cmd} {

    global PING_SUCCESS

    tet_infoline "Executing $cmd"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
        tet_infoline "FATAL: $cmd"
        tet_infoline "FATAL: Command failed when it should not have. ($msg)"
	return -1
    } else {
        set ret [string match *$PING_SUCCESS* $msg]
        if { $ret != 1 } { 
            tet_infoline "FATAL: $cmd"
            tet_infoline "FATAL: Command returned $msg when $PING_SUCCESS was expected"
	    return -1
        }
    }
    return 0
}

# execute a server ping command and catch errors
# ck response from ping - expect failure
# do not force return from caller on error
proc doit_ping_failure {cmd} {

    global PING_FAIL

    tet_infoline "Executing $cmd"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
        tet_infoline "FATAL: $cmd"
        tet_infoline "FATAL: Command failed when it should not have. ($msg)"
	return -1
    } else {
        set ret [string match *$PING_FAIL* $msg]
        if { $ret != 1 } { 
            tet_infoline "FATAL: $cmd"
            tet_infoline "FATAL: Command returned $msg when $PING_FAIL was expected"
	    return -1
        }
    }
    return 0
}

# execute command and catch return
# don't force return from caller
proc doit_noexit {cmd} {

    tet_infoline "Executing $cmd"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
        tet_infoline "FATAL: $msg"
	return -1
    } else {
	return 0
    }
}

# execute command and catch return
# don't force return from caller
proc doit_noexit_nofatal {cmd} {

    tet_infoline "Executing $cmd"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
        tet_infoline "Returned Error: $msg"
	return -1
    } else {
	return 0
    }
}
