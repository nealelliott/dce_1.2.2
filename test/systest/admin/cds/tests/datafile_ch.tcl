#!/opt/dcelocal/bin/dcecp
#
#
# FILENAME: datafile_ch.tcl
#
# COMPONENT NAME: dcecp  
#
# DESCRIPTION:
#	This file defines and sets the variables used by 
#	the dcecp script backup_restore_ch.tcl
#
# USAGE: This file is sourced by DCPSAN001 .
#
# DESCRIPTION:
#        Define variables used by DCPSAN001.
#
################################################################################

# Define OS variables

set PLATFORM   [exec uname -s]

if { $PLATFORM == "HP-UX" } {
   set RSH remsh
   set PSFLAGS -ef
} elseif { $PLATFORM == "OSF1" } {
     set RSH rsh
     set PSFLAGS ax
} elseif { $PLATFORM == "AIX" } {
    set RSH rsh
    set PSFLAGS -ef
} else {
     set RSH rsh
     set PSFLAGS ax 

}

# Define PATHNAMES

set DCELOCAL	$env(DCELOCAL)
set DCEBIN	[concat $DCELOCAL\/bin]
set DCEETC	[concat $DCELOCAL\/etc]
set CHDIR	[concat $DCELOCAL\/var/directory/cds]
set CH		[concat $DCELOCAL\/var/directory]
set CDSCACHE	[concat $DCELOCAL\/var/adm/directory/cds]
set CACHE	[concat $DCELOCAL\/var/adm/directory]

# Set the clearinghouse name.

set HOSTNAME	[exec hostname]
set CELLNAME	[exec $DCEBIN/getcellname]

# User identity

set LOGINNAME  [exec whoami]

# directories and logs locations

set PID	     [pid]
set LOGNAME  DCPSAN001

set WDIR     $env(STTMPDIR)
set WORKDIR  [concat $WDIR\/$LOGNAME\.$PID]
set BACKDIR  [concat $WORKDIR\/ch_backup_to]
#set RESTDIR  [concat $WORKDIR\/ch_restore_from]
set RESTDIR  $BACKDIR
set TEMPDIR  [concat $WORKDIR\/tmp]
set TMPCDS   [concat $WORKDIR\/tmp\/clearinghouse]
set TMPCACHE [concat $WORKDIR\/tmp\/cache]

set LDIR 	$env(STLOGDIR)
set LOGDIR 	[concat $LDIR\/$LOGNAME\.$PID]
set PASSDIR	[concat $LOGDIR/pass]
set FAILDIR	[concat $LOGDIR/fail]
set PASS_FAIL_LOG  [concat $LOGDIR/pass_fail_log]

set testlog  0

# Cell admin information

set CELLADMIN	cell_admin
set ADMIN_PW	-dce-

# Clean up flag
# CLEANUP equals to 0 means do no remove log files.

set CLEANUP 0

# Exit on error flag
# 0 = exiting normaly 
# 1 = exiting on error

set EXIT_ON_ERROR 1



