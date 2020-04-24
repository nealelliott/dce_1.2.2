#!/bin/dcecp

# /bin/dcecp script meant to be sourced by dcecp scripts
#
# Functions:
#     parse values defined in cellconfig.data
#     parse arguments.  Any argument of the form xxx=yyy will be
#             executed as a variable assignment

# Set variables as defined in cellconfig.data
set fileId [open "| grep #define ./cellconfig.data | sort" r]

while {[gets $fileId line] >= 0} {
    set data [split $line]
    set [lindex $data 1] [join [lrange $data 2 end] " "]
}

close $fileId

# Set variables as defined on command line.
foreach arg $argv {
    if {[regexp {([^=]*)=(.*)} $arg match var val] == 1} {
	regsub -all "::" $val " " value
	set $var $value
    }
}

# do a dce login if a DCEUSER is supplied
if { [info exists DCEUSER] && [string compare $DCEUSER ""] != 0} {
    login $DCEUSER -password $DCEPASSWD
}

proc var_value {var} {
    eval upvar $var lvar
    if {[eval info exists lvar]} {
	return $lvar
    }
    return 0
}

proc look_for_error {} {
    set msg [eval $args]
    if {[regexp {\*error} $msg]} {
	error $msg
    }
    set msg
}

set ee_result 1
set ee_msg ""

proc ee {args} {
    global VERBOSE
    global ee_result
    global ee_msg

    while {[lsearch -regexp $args {^-}] == 0} {
	set curswitch [lindex $args 0]
	set args [lreplace $args 0 0]
	regsub {^-} $curswitch {} switch
	if {[string compare $switch "onerror"] == 0 ||
	    [string compare $switch "nomsg"] == 0} {
	    eval set $switch 1
	}
    }

    if { [string compare [var_value VERBOSE] "y"] == 0
	 && [var_value onerror] == 0} {
	puts "+ $args"
	flush stdout
    }
    if {[catch {eval $args} ee_msg]} {
	set ee_result 0
	if {[var_value onerror]} {
	    puts "+ $args"
	    flush stdout
	}
    } else {
	set ee_result 1
    }
    if [expr {[var_value nomsg] == 0 &&
	      [expr {[string compare [var_value VERBOSE] "y"] == 0
		     || [var_value onerror]}]}] {
	puts $ee_msg
	flush stdout
    }
    return $ee_result
}

proc ckerr {args} {
    global ee_result
    global ee_msg

    if {$ee_result != 1} {
	puts "*error $ee_msg: $args"
    }
}

proc touch {args} {
    foreach arg $args {
	if [catch {open $arg {RDWR CREAT}} fileid] {
	    puts stderr "Could not touch $arg: $fileid"
	} else {
	    close $fileid
	}
    }
}
	
