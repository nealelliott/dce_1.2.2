#!/opt/dcelocal/bin/dcecp
#
#
# FILENAME: functions
#
# COMPONENT NAME: dcecp  
#
# FUNCTIONS: 
#	This file contains the procedures used by the test
#       backup_restore_ch.tcl
#
# USAGE:
#	This file is sourced by the following dcecp scripts:
#	. backup_restore_ch.t
#
# PROCEDURES:
#	echo_msg
#	initlogs
#	
#
#
##################################################################################


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: echo_msg
#	Generic procedure to print a message to a file which it is specified by
#	the argument fl.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc echo_msg {msg log} {
   puts $log $msg
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: initlogs
# 	This procedure creates the working directories and 
#	initializes the pass_fail_log file.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc initlogs {} {

   global WORKDIR LOGDIR BACKDIR RESTDIR TEMPDIR PASS_FAIL_LOG testlog
   global TMPCACHE TMPCDS PASSDIR FAILDIR
   
   catch {exec mkdir -p $WORKDIR}
   catch {exec mkdir -p $PASSDIR}
   catch {exec mkdir $FAILDIR}
   catch {exec mkdir $BACKDIR}
   catch {exec mkdir $RESTDIR}
   catch {exec mkdir $TEMPDIR}
   catch {exec mkdir $TMPCDS}
   catch {exec mkdir $TMPCACHE}

   set time [exec date]
   exec touch $PASS_FAIL_LOG

   if {[catch {exec ls $PASS_FAIL_LOG}] == 0} {
      set testlog [open $PASS_FAIL_LOG w]
   } else {
      set testlog stdout
   }
   echo_msg "-log-----:" $testlog
   echo_msg "-log-----: Beginning date and time  $time" $testlog
   return 0
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: user_root
#	Is the username root?
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
# 	This procedure checks that hostmachine has been configured as a CDS server.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc getcdsserver {hname} {

   global testlog

   if { [catch {rpcentry show /.:/hosts/$hname/cds-server} msg ] == 0 } {
       echo_msg "-log---: $msg" $testlog
       echo_msg "-log---: $hname is a CDS server." $testlog
       return 0
   } else {
       echo_msg "-log---: $msg" $testlog
       echo_msg "-log---: $hname is not a CDS server." $testlog
       return 1
   }
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: getcdsclients
#	This procedure returns the list of CDS clients configured in the cell.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc getcdsclients {} {

    return [directory list /.:/hosts -simplename]
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: run_cmd
#	This procedure executes a dcecp command. It returns an error if failure
#       occurs.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc run_cmd {cmd} {

   global testlog
   set resultcode  [catch {uplevel $cmd} answer]

   if {$resultcode != 0} {
       echo_msg "-log---:" $testlog
       echo_msg "-log---: Command-> $cmd" $testlog
       echo_msg "-log---: Output-> $answer" $testlog
       echo_msg "-log---: Command $cmd failed." $testlog
       return 1
   } else {
	echo_msg "-log---:" $testlog
	echo_msg "-log---: Command-> $cmd" $testlog
	echo_msg "-log---: Output-> $answer" $testlog
	echo_msg "-log---: Command $cmd executed successfully" $testlog
	return 0
   }
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: cell_cdsds
#	This procedure finds out how many additional CDS servers were configured
#	in the cell.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc list_cell_cdsds {clientlist hostname} {

   regsub $hostname $clientlist "" sanshostname 

# If there is only one server in the cell, then just return.

   if { [llength $sanshostname] == 0 } {
       return 1
   }

   set clientsonly 0
   set servers     0
   foreach i $sanshostname {
      if [catch {rpcentry show /.:/hosts/$i/cds-server} msg] {
         incr clientsonly
      } else {
         incr servers
      }
   }
   return $servers
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: disable_cdsd
#	Disables the CDS server on the host machine.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc disable_cdsd {hostname} {

   global PSFLAGS DCEBIN testlog

   set waiting 0

   if { [ catch {exec ps $PSFLAGS | egrep cdsd | egrep -v egrep} ] } {
       echo_msg "-log---: The CDS daemon (cdsd) is not running on $hostname." $testlog
   } else {
       echo_msg "-log---: The CDS daemon (cdsd) is running on $hostname." $testlog
   }

# Eventually, this should be a call to dced.

   if { [catch {exec $DCEBIN/cdscp disable server} ] == 1 } {
       echo_msg "-log---: " $testlog
       echo_msg "-log---: Failed to disable the CDS server on $hostname." $testlog
       return 1
   } 

# Wait until cdsd goes away before it returns - but do not wait forever.
   
   while {[ catch {exec ps $PSFLAGS | egrep cdsd | egrep -v egrep}] == 0} {
       incr waiting
       if {$waiting >= 1000} {
           echo_msg "-log---: Failed to disable cdsd." $testlog
           return 1
       }
   }    
   return 0
}

#################################################################################
# PROCEDURE NAME: init_name_space
#	Make initial test entries in the name space.
#################################################################################

proc init_name_space {} {

  global testlog
# Make entries in the name space

   echo_msg "-log---: Create test directories and objects to backup ." $testlog
    
   run_cmd {directory list /.:}

   if {[run_cmd {directory list /.:/br_dir_1}] == 1 } {
        run_cmd {directory create /.:/br_dir_1}
        run_cmd {object create /.:/br_dir_1/br_obj_1}
   }

   if {[run_cmd {directory list /.:/br_dir_2}] == 1 } {
        run_cmd {directory create /.:/br_dir_2}
        run_cmd {object create /.:/br_dir_2/br_obj_2}
   }

   if {[run_cmd {directory list /.:/br_dir_3}] == 1 } {
        run_cmd {directory create /.:/br_dir_3}
        run_cmd {object create /.:/br_dir_3/br_obj_3}
   }

run_cmd {directory list /.:}

}
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: backup_ch
#	This procedure backs up the clearinghouse files to a directory specified
#	by the varialble BACKDIR. BACKDIR has been defined in the file datafile.tcl
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc backup_ch {} {

   global BACKDIR CHDIR CDSCACHE DCEETC testlog
   set copy_sts 0

   if { [catch {eval exec cp $CHDIR/cds_files $BACKDIR} msg] == 0 } {
      echo_msg "-log---: Copied $CHDIR/cds_files to $BACKDIR" $testlog
      echo_msg "-log---: Output -> $msg" $testlog
   } else {
      echo_msg "-log---: Failed to copy  $CHDIR/cds_files to $BACKDIR" $testlog
      echo_msg "-log---: Output -> $msg" $testlog
      incr copy_sts
   }

   set checkpoint_fs [glob $CHDIR/*_ch.checkpoint*]
   
   foreach cpfs $checkpoint_fs {
       if { [catch {exec cp $cpfs  $BACKDIR} msg] == 0 } {
          echo_msg "-log---: Copied $cpfs to $BACKDIR" $testlog
          echo_msg "-log---: Output -> $msg" $testlog
       } else {
          echo_msg "-log---: Failed to copy $cpfs to $BACKDIR" $testlog
          echo_msg "-log---: Output -> $msg" $testlog
          incr copy_sts
       }
    }

    if { [catch {exec cp $DCEETC/cds_attributes $BACKDIR} msg] == 0 } {
        echo_msg "-log---: Copied $DCEETC/cds_attributes to $BACKDIR" $testlog
        echo_msg "-log---: Output -> $msg" $testlog
    } else {
        echo_msg "-log---: Failed to copy $DCEETC/cds_attributes to $BACKDIR" $testlog
        echo_msg "-log---: Output -> $msg" $testlog
        incr copy_sts
    }

    set chtlog_fs [glob $CHDIR/*_ch.tlog*]

    foreach tlog $chtlog_fs { 
       if { [catch {exec cp $tlog $BACKDIR} msg] == 0 } {
           echo_msg "-log---: Copied $tlog to $BACKDIR" $testlog
           echo_msg "-log---: Output -> $msg" $testlog
       } else {
           echo_msg "-log---: Failed to copy $tlog to $BACKDIR" $testlog
           echo_msg "-log---: Output -> $msg" $testlog
           incr copy_sts
       }
    }

    set chver_fs [glob $CHDIR/*_ch.version]

    foreach chver $chver_fs {
       if { [catch {exec cp $chver $BACKDIR} msg] == 0 } {
           echo_msg "-log---: Copied $chver to $BACKDIR" $testlog
           echo_msg "-log---: Output -> $msg" $testlog
       } else {
           echo_msg "-log---: Failed to copy $chver to $BACKDIR" $testlog
           echo_msg "-log---: Output -> $msg" $testlog
           incr copy_sts
       }
    }

    if { [catch {exec cp $CDSCACHE/cds_cache.version $BACKDIR} msg] == 0 } {
        echo_msg "-log---: Copied $CDSCACHE/cds_cache.version $BACKDIR" $testlog
        echo_msg "-log---: Output -> $msg" $testlog
    } else {
        echo_msg "-log---: Failed to copy $CDSCACHE/cds_cache.version to $BACKDIR" $testlog
        echo_msg "-log---: Output -> $msg" $testlog
        incr copy_sts
    }

    set cache_fs [glob $CDSCACHE/cds_cache.0*]

    foreach cfs $cache_fs {
       if { [catch {exec cp $cfs $BACKDIR} msg] == 0} {
           echo_msg "-log---: Copied $cfs to $BACKDIR" $testlog
           echo_msg "-log---: Output -> $msg" $testlog
       } else { 
           echo_msg "-log---: Failed to copy $cfs to $BACKDIR" $testlog
           echo_msg "-log---: Output -> $msg" $testlog
           incr copy_sts
       }
    }
    return $copy_sts
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: remove_ch_files
#	This procedure removes all the files that were backed up. This is done
#	so that the restore operation can be checked.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc remove_ch_files {} {

   global CHDIR CDSCACHE DCEETC
   
   exec rm $CHDIR/cds_files
   exec rm [glob $CHDIR/*_ch.checkpoint*]
   exec rm $DCEETC/cds_attributes
   exec rm [glob $CHDIR/*ch.tlog*]
   exec rm [glob $CHDIR/*_ch.version]
   exec rm [glob $CDSCACHE/cds_cache.version]
   exec rm [glob $CDSCACHE/cds_cache.0*]
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: restart_cdsd
#	This procedure starts cdsd. 
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc restart_cdsd {} {
  
   global DCEBIN

   exec sh -c "$DCEBIN/cdsd 1>&2"
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: access_name_space
#	This procedure access lists directories and objects in the namespace.
#	It returns an error if the objects are not present.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc access_name_space {} {

    set total_failures 0

    run_cmd {directory list /.:}

    if {[run_cmd {directory list {/.:/br_dir_1 /.:/bk_dir_2 /.:/bk_dir_3}}] == 1} {
        incr total_failures
    }

    if {[run_cmd {object show {/.:/br_dir_1/br_obj_1 /.:/br_dir_2/br_obj_2 /.:/br_dir_3/br_obj_3}}] == 1} {
        incr total_failures
    }
    return $total_failures
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: delete_name_space_entries
#	This procedure deletes the namespace entries that were created in case of
#	failure.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc delete_name_space_entries {} {

    set total_failures 0
  
    if {[run_cmd {object delete {/.:/br_dir_1/br_obj_1 /.:/br_dir_2/br_obj_2 /.:/br_dir_3/br_obj_3}}] == 1} {
        incr total_failures
    }
  
    if {[run_cmd {directory delete {/.:/br_dir_1 /.:/br_dir_2 /.:/br_dir_3}}] == 1} {
        incr total_failures
    }
    return $total_failures
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: restore_ch
#	This procedure restores the clearinghouse files to the appropiate 
#	directories.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc restore_ch {} {

   global BACKDIR CHDIR CDSCACHE DCEETC CH CACHE testlog
   global TMPCDS TMPCACHE   
   set copy_sts 0


#NOTE: the exec command hungs!!
   
#   if { [catch {exec sh -c "cp -pR $CHDIR $TMPCDS; cp -pR $CDSCACHE $TMPCACHE"} msg] == 0} {
#       echo_msg "-log---: Saved the content of $CHDIR before doing the backup." $testlog
#       echo_msg "-log---: Output -> $msg" $testlog
#   } else {
#       echo_msg "-log---: Failed to make a copy of $CHDIR before starting the backup." $testlog
#       echo_msg "-log---: Exiting because don't want to overwrite the clearinghouse" $testlog
#       echo_msg "-log---: without a safe backup of the exisiting one." $testlog
#       return 1
#   }

   if {[catch {eval exec cp $BACKDIR/cds_files $CHDIR} msg] == 0 } {
      echo_msg "-log---: Copied $BACKDIR/cds_files to $CHDIR" $testlog
      echo_msg "-log---: Output -> $msg" $testlog
   } else {
      echo_msg "-log---: Failed to copy  $BACKDIR/cds_files to $CHDIR" $testlog
      echo_msg "-log---: Output -> $msg" $testlog
      incr copy_sts
   }

   set checkp_fs [glob $BACKDIR/*_ch.checkpoint*]

   foreach ckfs $checkp_fs {
      if {[catch {exec cp $ckfs $CHDIR} msg ] == 0 } {
          echo_msg "-log---: Copied $ckfs to $CHDIR" $testlog
          echo_msg "-log---: Output -> $msg" $testlog
       } else {
          echo_msg "-log---: Failed to copy $ckfs to $CHDIR" $testlog
          echo_msg "-log---: Output -> $msg" $testlog
          incr copy_sts
       }
    }

    if { [catch {exec cp $BACKDIR/cds_attributes $DCEETC} msg] == 0 } {
        echo_msg "-log---: Copied $BACKDIR/cds_attributes to $DCEETC" $testlog
        echo_msg "-log---: Output -> $msg" $testlog
    } else {
        echo_msg "-log---: Failed to copy $BACKDIR/cds_attributes to $DCEETC" $testlog
        echo_msg "-log---: Output -> $msg" $testlog
        incr copy_sts
    }

    set tlog_fs [glob $BACKDIR/*ch.tlog*]
    
    foreach tfs $tlog_fs {
       if { [catch {exec cp $tfs $CHDIR} msg ] == 0 } {
           echo_msg "-log---: copied $tfs to $CHDIR" $testlog
           echo_msg "-log---: output -> $msg" $testlog
       } else {
           echo_msg "-log---: Failed to copy $tfs to $CHDIR" $testlog
           echo_msg "-log---: Output -> $msg" $testlog
           incr copy_sts
       }
    }

    set chver_fs [glob $BACKDIR/*_ch.version]

    foreach chvfs $chver_fs {
       if { [catch {exec cp $chvfs $CHDIR} msg ] == 0 } {
           echo_msg "-log---: Copied $chvfs to $CHDIR" $testlog
           echo_msg "-log---: Output -> $msg" $testlog
       } else {
           echo_msg "-log---: Failed to copy $chvfs to $CHDIR" $testlog
           echo_msg "-log---: Output -> $msg" $testlog
           incr copy_sts
       }
    }

    if { [catch {exec cp $BACKDIR/cds_cache.version $CDSCACHE} msg] == 0 } {
        echo_msg "-log---: copied $BACKDIR/cds_cache.version $CDSCACHE" $testlog
        echo_msg "-log---: output -> $msg" $testlog
    } else {
        echo_msg "-log---: Failed to copy $BACKDIR/cds_cache.version to $CDSCACHE" $testlog
        echo_msg "-log---: Output -> $msg" $testlog
        incr copy_sts
    }

    set cache_fs [glob $BACKDIR/cds_cache.0*]

    foreach chfs $cache_fs {
       if { [catch {exec cp $chfs $CDSCACHE} msg] == 0} {
           echo_msg "-log---: Copied $chfs to $CDSCACHE" $testlog
           echo_msg "-log---: Output -> $msg" $testlog
       } else { 
           echo_msg "-log---: Failed to copy $chfs to $CDSCACHE" $testlog
           echo_msg "-log---: Output -> $msg" $testlog
           incr copy_sts
       }
    }


#    if { $copy_sts != 0} {
#       echo_msg "-log---: The restore operation failed." $testlog
#       echo_msg "-log---: The clearinghouse remains unchanged." $testlog 
#       if { [catch {exec sh -c "cp -pR $TMPCDS/cds $CH; cp -pR $TMPCACHE/cds $CACHE" } msg] == 0 } {
#           echo_msg "-log---: Something is wrong." $testlog
#           echo_msg "-log---: WARNING!! The clearinghouse might not be relaible." $testlog
#       }
#    } 

    return $copy_sts
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: modify_name_space
#	Modify the name space by deleating known directories and objects and
#	adding new one. 
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc modify_name_space {} {

    global testlog

    echo_msg "-log---: Delete directories and objects that were backed up." $testlog
    if {[delete_name_space_entries] == 0} { 
        echo_msg "-log---: Directores and object backed up were deleted successfully." $testlog
    } else {
        echo_msg "-log---: Directores and object backed up were not deleted successfully." $testlog
    }

    if {[run_cmd {directory list /.:/br_dir_99}] == 1 } {
        run_cmd {directory create /.:/br_dir_99}
    }

    if {[run_cmd {directory list /.:/br_dir_98}] == 1 } {
        run_cmd {directory create /.:/br_dir_98}
    }

    if {[run_cmd {directory list /.:/br_dir_97}] == 1 } {
        run_cmd {directory create /.:/br_dir_97}
    }
    return 0
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: get_cell_name
#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc get_cell_name {} {

    set cellname [exec getcellname]
    set cellname [string trim $cellname /...]

# The following command is here so that when the name of
# of a H-Cell is returned by the "getcellname" command the 
# / is changed to _ to create the name of the backup file.

    regsub -all / $cellname _ cellname
    return $cellname
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: ping_server
#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc ping_server {hname} {

   global PLATFORM

   if {$PLATFORM == "HP-UX"} {
      set PACKETSIZE 64
   } else {
      set PACKETSIZE 1
   }

   return [catch {exec ping $hname $PACKETSIZE 1 | grep -s icmp_seq}]
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: exit_proc
#	This procedure closes the log files, cleans up, and exits.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc exit_proc {eflag} {

   global testlog CLEANUP PASS_FAIL_LOG
   global PASSDIR FAILDIR

#   clean_name_space
   set time [exec date]
#   delete_name_space_entries
   echo_msg "-log---: Ending time $time." $testlog
   close $testlog 
   logout
   if {$CLEANUP == 1} {
       exec {rm -r $LOGDIR}
   }
   if {$eflag == 1} {
      exec sh -c "cp $PASS_FAIL_LOG $FAILDIR"
   } else {
      exec sh -c "cp $PASS_FAIL_LOG $PASSDIR"
   }
   exit 1
}


