#!/opt/dce/local/bin/dcecp
#
# FILE_NAME: cds_common_procs.tcl
#
# USAGE: This files is sourced by the CDS dcecp admin scripts. 
#
# DESCRIPTION:
#
#	The procedures defined in this file are common to 
#	to all the CDS admin scripts using dcecp.
#	
# PREREQUISITES and SETUP:
#	
#
# DESCRIPTION:
#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: print_msg
#	This procedure prints a log message to a file or to stdout.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc print_msg { msg log} {

	puts $log $msg
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: initlogs
#	This procedure initialize the log file and creates any working directories
#	needed during the execution of this test.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc initlogs {} {

   global WORKDIR LOGDIR TMPCH TMPCDS CHBK PASS_FAIL_LOG logfile 
   global PASSDIR FAILDIR
   global TESTNAME

   catch {exec mkdir -p $WORKDIR}
   catch {exec mkdir -p $PASSDIR $FAILDIR}
   catch {exec mkdir -p $TMPCH $TMPCDS $CHBK}

   set time [exec date]

   exec touch $PASS_FAIL_LOG
   
   if { [file exists $PASS_FAIL_LOG] == 1 } {
      set logfile [open $PASS_FAIL_LOG w]
   } else {
      set logfile stdout
   }
   print_msg "-log----: " $logfile
   print_msg "-log----: Beginning execution of $TESTNAME" $logfile
   print_msg "-log----: The date and time are $time" $logfile
   return 0
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: user_root
#	This procedure checks that the username running this script is
#	root.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc user_root {} {

   global LOGINNAME

   if { $LOGINNAME == "root" } {
       return 0
   } else {
       return 1
   }
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: getcdsserver
#	This procedure verifies that the hostname is running a CDS server.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc getcdsserver {hname} {

   global logfile

   if { [catch {rpcentry show /.:/hosts/$hname/cds-server} msg ] == 0 } {
       print_msg "-log---: rpcentry show /.:/hosts/$hname/cds-server" $logfile 
       print_msg "-log---: $msg" $logfile
       print_msg "-log---: $hname is a CDS server." $logfile
       return 0
   } else {
       print_msg "-log---: $msg" $logfile
       print_msg "-log---: $hname is not a CDS server." $logfile
       return 1
   }
}


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: run_cmd
#       This procedure executes a dcecp command. If it fails, it returns an error.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc run_cmd {cmd} {

   global logfile
   set resultcode  [catch {uplevel $cmd} answer]

   if {$resultcode != 0} {
       print_msg "-log---:" $logfile
       print_msg "-log---: Command-> $cmd" $logfile
       print_msg "-log---: Output-> $answer" $logfile
       print_msg "-log---: Command $cmd failed." $logfile
       return 1
   } else {
        print_msg "-log---: ----" $logfile
        print_msg "-log---: Command-> $cmd" $logfile
        print_msg "-log---: Output-> $answer" $logfile
        print_msg "-log---: Command $cmd executed successfully" $logfile
        return 0
   }
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: init_name_space
#       Make initial test entries in the name space.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc init_name_space {} {

  global logfile
# Make entries in the name space

   print_msg "-log---: Calling procedure run_cmd to make entries in the name space." $logfile
    
   run_cmd {directory list /.:}

   if {[run_cmd {directory list /.:/dirtest01}] == 1 } {
        run_cmd {directory create /.:/dirtest01}
        run_cmd {object create /.:/dirtest01/obj01}
   }

   if {[run_cmd {directory list /.:/dirtest02}] == 1 } {
        run_cmd {directory create /.:/dirtest02}
        run_cmd {object create /.:/dirtest02/obj02}
   }

   if {[run_cmd {directory list /.:/dirtest03}] == 1 } {
        run_cmd {directory create /.:/dirtest03}
        run_cmd {object create /.:/dirtest03/obj03}
   }

run_cmd {directory list /.:}

}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: disable_cdsd
#       Disables the CDS server on the host.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc disable_cdsd {hostname} {

   global PSFLAGS LOGINNAME DCEBIN logfile

   set waiting 0

   if { [ catch {exec ps $PSFLAGS | egrep cdsd | egrep -v egrep} ] } {
       print_msg "-log---: The CDS daemon (cdsd) is not running on $hostname." $logfile
   } else {
       print_msg "-log---: The CDS daemon (cdsd) is running on $hostname." $logfile
   }

# Eventually, this should be a call to dced.

   if { [catch {exec $DCEBIN/cdscp disable server} ] == 1 } {
       print_msg "-log---: ---" $logfile
       print_msg "-log---: Failed to disable the CDS server on $hostname." $logfile
       return 1
   } 

# Wait until cdsd goes away before it returns - but do not wait forever.
   
   while {[ catch {exec ps $PSFLAGS | egrep cdsd | egrep -v egrep}] == 0} {
       incr waiting
       if {$waiting >= 1000} {
           print_msg "-log---: Failed to disable cdsd." $logfile
           return 1
       }
   }    
   return 0
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: backup_ch
#       This procedure backs up the clearinghouse files to a directory specified
#       by the varialble BACKDIR. BACKDIR has been defined in the file datafile.tcl
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc backup_ch {} {

   global CHBK CHDIR CDSCACHE DCEETC logfile
   set copy_sts 0

   if { [catch {eval exec cp $CHDIR/cds_files $CHBK} msg] == 0 } {
      print_msg "-log---: Copied $CHDIR/cds_files to $CHBK" $logfile
      print_msg "-log---: Output -> $msg" $logfile
   } else {
      print_msg "-log---: Failed to copy  $CHDIR/cds_files to $CHBK" $logfile
      print_msg "-log---: Output -> $msg" $logfile
      incr copy_sts
   }

   set checkpoint_fs [glob $CHDIR/*_ch.checkpoint*]
   
   foreach cpfs $checkpoint_fs {
       if { [catch {exec cp $cpfs  $CHBK} msg] == 0 } {
          print_msg "-log---: Copied $cpfs to $CHBK" $logfile
          print_msg "-log---: Output -> $msg" $logfile
       } else {
          print_msg "-log---: Failed to copy $cpfs to $CDSCH" $logfile
          print_msg "-log---: Output -> $msg" $logfile
          incr copy_sts
       }
    }

    if { [catch {exec cp $DCEETC/cds_attributes $CHBK} msg] == 0 } {
        print_msg "-log---: Copied $DCEETC/cds_attributes to $CHBK" $logfile
        print_msg "-log---: Output -> $msg" $logfile
    } else {
        print_msg "-log---: Failed to copy $DCEETC/cds_attributes to $CHBK" $logfile
        print_msg "-log---: Output -> $msg" $logfile
        incr copy_sts
    }

    set chtlog_fs [glob $CHDIR/*_ch.tlog*]

    foreach tlog $chtlog_fs { 
       if { [catch {exec cp $tlog $CHBK} msg] == 0 } {
           print_msg "-log---: Copied $tlog to $CHBK" $logfile
           print_msg "-log---: Output -> $msg" $logfile
       } else {
           print_msg "-log---: Failed to copy $tlog to $CHBK" $logfile
           print_msg "-log---: Output -> $msg" $logfile
           incr copy_sts
       }
    }

    set chver_fs [glob $CHDIR/*_ch.version]

    foreach chver $chver_fs {
       if { [catch {exec cp $chver $CHBK} msg] == 0 } {
           print_msg "-log---: Copied $chver to $CHBK" $logfile
           print_msg "-log---: Output -> $msg" $logfile
       } else {
           print_msg "-log---: Failed to copy $chver to $CHBK" $logfile
           print_msg "-log---: Output -> $msg" $logfile
           incr copy_sts
       }
    }

    if { [catch {exec cp $CDSCACHE/cds_cache.version $CHBK} msg] == 0 } {
        print_msg "-log---: Copied $CDSCACHE/cds_cache.version to $CHBK" $logfile
        print_msg "-log---: Output -> $msg" $logfile
    } else {
        print_msg "-log---: Failed to copy $CDSCACHE/cds_cache.version to $CHBK" $logfile
        print_msg "-log---: Output -> $msg" $logfile
        incr copy_sts
    }

    set cache_fs [glob $CDSCACHE/cds_cache.0*]

    foreach cfs $cache_fs {
       if { [catch {exec cp $cfs $CHBK} msg] == 0} {
           print_msg "-log---: Copied $cfs to $CHBK" $logfile
           print_msg "-log---: Output -> $msg" $logfile
       } else { 
           print_msg "-log---: Failed to copy $cfs to $CHBK" $logfile
           print_msg "-log---: Output -> $msg" $logfile
           incr copy_sts
       }
    }
    return $copy_sts
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: restore_ch
#       This procedure restores the CDS and clearinghouse files to the appropiate
#       directories.
#	This procedure expects to find the CDS and clearinghouse files in one 
#	directory and it restored to the appropiate directories.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc restore_ch {} {

   global CHREST CHDIR CDSCACHE DCEETC CH CACHE logfile
   global TMPCH TMPCDS
   set copy_sts 0


#NOTE: the exec command hungs!!

#   if { [catch {exec sh -c "cp -pR $CHDIR $TMPCH; cp -pR $CDSCACHE $TMPCDS"} msg] == 0} {
#       print_msg "-log---: Saved the content of $CHDIR before doing the backup." $logfile
#       print_msg "-log---: Output -> $msg" $filelog
#   } else {
#       print_msg "-log---: Failed to make a copy of $CHDIR before starting the backup." $logfile
#       print_msg "-log---: Exiting because don't want to overwrite the clearinghouse" $logfile
#       print_msg "-log---: without a safe backup of the exisiting one." $logfile
#       return 1
#   }

   if {[catch {eval exec cp $CHREST/cds_files $CHDIR} msg] == 0 } {
      print_msg "-log---: Copied $CHREST/cds_files to $CHDIR" $logfile
      print_msg "-log---: Output -> $msg" $logfile
   } else {
      print_msg "-log---: Failed to copy  $CHREST/cds_files to $CHDIR" $logfile
      print_msg "-log---: Output -> $msg" $logfile
      incr copy_sts
   }

   set checkp_fs [glob $CHREST/*_ch.checkpoint*]

   foreach ckfs $checkp_fs {
      if {[catch {exec cp $ckfs $CHDIR} msg ] == 0 } {
          print_msg "-log---: Copied $ckfs to $CHDIR" $logfile
          print_msg "-log---: Output -> $msg" $logfile
       } else {
          print_msg "-log---: Failed to copy $ckfs to $CHDIR" $logfile
          print_msg "-log---: Output -> $msg" $logfile
          incr copy_sts
       }
    }

    if { [catch {exec cp $CHREST/cds_attributes $DCEETC} msg] == 0 } {
        print_msg "-log---: Copied $CHREST/cds_attributes to $DCEETC" $logfile
        print_msg "-log---: Output -> $msg" $logfile
    } else {
        print_msg "-log---: Failed to copy $CHREST/cds_attributes to $DCEETC" $logfile
        print_msg "-log---: Output -> $msg" $logfile
        incr copy_sts
    }

    set tlog_fs [glob $CHREST/*ch.tlog*]

    foreach tfs $tlog_fs {
       if { [catch {exec cp $tfs $CHDIR} msg ] == 0 } {
           print_msg "-log---: copied $tfs to $CHDIR" $logfile
           print_msg "-log---: output -> $msg" $logfile
       } else {
           print_msg "-log---: Failed to copy $tfs to $CHDIR" $logfile
           print_msg "-log---: Output -> $msg" $logfile
           incr copy_sts
       }
    }

    set chver_fs [glob $CHREST/*_ch.version]

    foreach chvfs $chver_fs {
       if { [catch {exec cp $chvfs $CHDIR} msg ] == 0 } {
           print_msg "-log---: Copied $chvfs to $CHDIR" $logfile
           print_msg "-log---: Output -> $msg" $logfile
       } else {
           print_msg "-log---: Failed to copy $chvfs to $CHDIR" $logfile
           print_msg "-log---: Output -> $msg" $logfile
           incr copy_sts
       }
    }

    if { [catch {exec cp $CHREST/cds_cache.version $CDSCACHE} msg] == 0 } {
        print_msg "-log---: copied $CHREST/cds_cache.version $CDSCACHE" $logfile
        print_msg "-log---: output -> $msg" $logfile
    } else {
        print_msg "-log---: Failed to copy $CHREST/cds_cache.version to $CDSCACHE" $logfile
        print_msg "-log---: Output -> $msg" $logfile
        incr copy_sts
    }

    set cache_fs [glob $CHREST/cds_cache.0*]


    foreach chfs $cache_fs {
       if { [catch {exec cp $chfs $CDSCACHE} msg] == 0} {
           print_msg "-log---: Copied $chfs to $CDSCACHE" $logfile
           print_msg "-log---: Output -> $msg" $logfile
       } else {
           print_msg "-log---: Failed to copy $chfs to $CDSCACHE" $logfile
           print_msg "-log---: Output -> $msg" $logfile
           incr copy_sts
       }
    }

#    if { $copy_sts != 0} {
#       print_msg "-log---: The restore operation failed." $logfile
#       print_msg "-log---: The clearinghouse remains unchanged." $logfile
#       if { [catch {exec sh -c "cp -pR $TMPCH/cds $CH; cp -pR $TMPCDS/cds $CACHE" } msg] == 0 } {
#           print_msg "-log---: Something is wrong." $logfile
#           print_msg "-log---: WARNING!! The clearinghouse might not be relaible." $logfile
#       }
#    }

    return $copy_sts
}


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: restart_cdsd
#       This procedure starts cdsd. 
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc restart_cdsd {} {
  
   global DCEBIN

   exec sh -c "$DCEBIN/cdsd 1>&2"
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: exit_proc
#	This procedure exits the test and cleans up working directories.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc exit_proc {eflag} {
   global logfile
   global PASSDIR FAILDIR PASS_FAIL_LOG CLEANUP

   set time [exec date]
   print_msg "-log---: Ending time $time." $logfile
   close $logfile
   logout
   if {$eflag == 1} {
      exec sh -c "cp $PASS_FAIL_LOG $FAILDIR"
   } else {
      exec sh -c "cp $PASS_FAIL_LOG $PASSDIR"
   }
}




