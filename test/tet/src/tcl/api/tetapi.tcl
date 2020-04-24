# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: tetapi.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:50:12  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:48  marty]
#
# Revision 1.1.4.1  1995/12/11  22:56:41  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:34:30  root]
# 
# Revision 1.1.2.6  1994/07/08  19:39:30  rousseau
# 	Fix problem with long output lines (CR 10707).
# 	[1994/07/08  19:39:20  rousseau]
# 
# Revision 1.1.2.5  1994/06/10  20:51:21  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:20:05  devsrc]
# 
# Revision 1.1.2.4  1994/05/18  20:54:57  rousseau
# 	Fix typos. Make smoe performance changes (CR 10673).
# 	[1994/05/18  20:54:36  rousseau]
# 
# Revision 1.1.2.3  1994/05/03  13:36:11  rousseau
# 	Fixed startup and cleanup function code (CR 10506).
# 	[1994/05/03  13:35:29  rousseau]
# 
# Revision 1.1.2.2  1994/03/07  21:35:25  rousseau
# 	Use '-c' option to sh for portability (CR 10085).
# 	[1994/03/07  21:35:10  rousseau]
# 
# Revision 1.1.2.1  1994/02/10  22:54:42  rousseau
# 	Initial version
# 	[1994/02/10  22:53:53  rousseau]
# 
# $EndLog$


# DESCRIPTION:
#	This file contains tcl functions for use with the tcl API.
#	It is sourced automatically by the tcl TCM.
#	In addition it should be sourced by test purposes that are written as
#	separate tcl scripts, by means of the tcl "source" command.
#
#	The following functions are provided:
#
#		tet_setcontext
#		tet_setblock
#		tet_infoline
#		tet_result
#		tet_delete
#		tet_reason
#

# set current context and reset block and sequence
# usage: tet_setcontext
proc tet_setcontext {} {
  global TET_CONTEXT TET_BLOCK TET_SEQUENCE

  set TET_CONTEXT [exec sh -c {(:)& echo $!}]
  set TET_BLOCK 1
  set TET_SEQUENCE 1
}


# increment the current block ID, reset the sequence number to 1
# usage: tet_setblock
proc tet_setblock {} {
  global TET_BLOCK TET_SEQUENCE

  incr TET_BLOCK
  set TET_SEQUENCE 1
}


# print an information line to the execution results file
# and increment the sequence number
# usage: tet_infoline args [...]
proc tet_infoline args {
  global TET_TPCOUNT TET_CONTEXT TET_BLOCK TET_SEQUENCE

  tet_output 520 "$TET_TPCOUNT $TET_CONTEXT $TET_BLOCK $TET_SEQUENCE" $args
  incr TET_SEQUENCE
}


# record a test result for later emmision to the execution results file
# by tet_tpend
# usage: tet_result result_name
# (note that a result name is expected, not a result code number)
proc tet_result {result_name} {
    global TET_TMPRES

    if {([catch {set result_name}]) || 
        ([string length $result_name] == 0)} {
        puts stderr "tet_result: no result name specified"
        exit
    }
    if {[tet_getcode $result_name]} {
        tet_error "tet_result: invalid result name $result_name"
        set result_name NORESULT
    }
    
    if {([catch {set TET_TMPRES}]) || 
        ([string length $TET_TMPRES] == 0)} {
        puts stderr "tet_result: TET_TMPRES is not defined"
        exit
    }
    
    set TMPRES_FD [open $TET_TMPRES a]
    puts $TMPRES_FD $result_name 
    close $TMPRES_FD
}


# mark a test purpose as deleted
# usage: tet_delete test_name reason [...]
proc tet_delete {test_name args} {
  global TET_DELETES

  if {([catch {set test_name}] == 1) || 
      ([string length $test_name] == 0)} {
    puts stderr "tet_delete: no test_name specified"
    exit
  }
  
  if {[string length $args] == 0} {
    tet_undelete $test_name
    return
  }

  if {[tet_reason $test_name]} {
    tet_undelete $test_name
  }

  if {([catch {set TET_DELETES}] == 1) || 
      ([string length $TET_DELETES] == 0)} {
    puts stderr "tet_delete: TET_DELETES is not defined"
    exit
  }
  
  exec echo "$test_name $args" >> $TET_DELETES
}


# print the reason why a test purpose has been deleted
# return 0 if the test purpose has been deleted, 1 otherwise
# usage: tet_reason test_name
proc tet_reason {test_name} {
    global TET_DELETES

    set fd [open $TET_DELETES r]
    
    while {![eof $fd]} {
	set line [gets $fd]
	
	if {[lindex $line 0] == $test_name} {
	    puts [lindex 1 $line]
	    close $fd
	    return 0
	}
    }
    
    close $fd
    return 1
}


# ******************************************************************

#
# "private" functions for internal use by the tcl API
# these are not published interfaces and may go away one day
#
proc tet_getcode {code_name} {
    global TET_ABORT TET_RESNUM TET_CODE TET_ABACTION
    
    set TET_ABORT NO
    set TET_RESNUM -1
    
    if {[catch {set TET_CODE}]} {
	puts stderr "tet_getcode: TET_CODE is not defined"
	exit
    }

    if {[catch {set code_name}]} {
	puts stderr "tet_getcode: no code name specified"
	exit
    }

    set fd [open $TET_CODE r]
    seek $fd 0
    
    while {![eof $fd]} {
	set line [gets $fd]
	if {[regexp ^# $line]} {
	    continue
	}

	if {[regexp ^$ $line]} {
	    continue
	}

	if {[lindex $line 1] == $code_name} {
	    set TET_RESNUM [lindex $line 0]
	    set TET_ABACTION [lindex $line 2]
	    break
	}
    }
    
    close $fd

    if {$TET_RESNUM == -1} {
	unset TET_ABACTION
	return 1
    }
    
    switch $TET_ABACTION {
	"" -
	Continue {
	    set TET_ABORT NO
	}
	Abort {
	    set TET_ABORT YES
	}
	default {
	    tet_error "invalid action field $TET_ABACTION in file $TET_CODE"
	    set TET_ABORT NO
	}
    }
    
    unset TET_ABACTION
    return 0
}


# tet_undelete - undelete a test purpose
proc tet_undelete {test_name} {
    global TET_DELETES
    
    set fd [open $TET_DELETES r]
    set tfd [open "/tmp/tet_deletes.tmp" w+]
    seek $fd 0
    
    while {![eof $fd]} {
	set line [gets $fd]
	
	if {[string first $test_name $line] == 0} {
	    continue;
	} else {
	    puts $tfd $line
	}
    } 
    
    close $fd
    close $tfd
    exec cp "/tmp/tet_deletes.tmp" $TET_DELETES
    exec rm "/tmp/tet_deletes.tmp"
}


# tet_error - print an error message to stderr and on TCM Message line
proc tet_error {args} {
    global TET_PNAME TET_RESFILE_FD TET_ACTIVITY

    puts stderr "$TET_PNAME: $args"
    
    if {[catch {set TET_ACTIVITY}]} {
	set TET_ACTIVITY 0
    }
    
    if {([catch {set TET_ACTIVITY}] == 1) || 
        ([string length $TET_ACTIVITY] == 0)} {
	set TET_ACTIVITY 0
    }
    
    puts $TET_RESFILE_FD "510|$TET_ACTIVITY|$args"
}


# tet_output - print a line to the execution results file
proc tet_output {args} {
    global TET_STDERR TET_RESFILE_FD TET_ACTIVITY

    if {[catch {set TET_ACTIVITY}]} {
	set TET_ACTIVITY 0
    }
    
    if {[string length [lindex $args 1]] > 0} {
	set tet_sp " "
    } else {
	set tet_sp ""
    }
    
    set line [format "%d|%s%s%s|%s" [lindex $args 0] $TET_ACTIVITY $tet_sp \
            [lindex $args 1] [lindex $args 2]]
    
    regsub -all \n $line " " line2
    set line $line2
    
    if {[string length $line] > 511} {
	puts stderr [format "warning: results file line truncated: prefix: %d|%s%s%s|" [lindex $args 0] $TET_ACTIVITY $tet_sp [lindex $args 1]]
	set line [string range $line 0 509]
    }
    
    puts $TET_RESFILE_FD "$line"
    
    if {([file exists $TET_STDERR]) && ([file size $TET_STDERR] > 0)} {
	tet_error "[exec cat $TET_STDERR]"
    }
}

