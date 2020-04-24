#!/opt/dcelocal/bin/dcecp
#
#
# FILENAME: admintest_env.tcl
#
# COMPONENT NAME: dcecp
#
# DESCRIPTION:
#       This file defines variables used by the tests DCPSAN002 
#	and DCPSAN003 
#
################################################################################

# OS dependent variables

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

# Location of the DCE binaries

set DCELOCAL    $env(DCELOCAL)
set DCEBIN      [concat $DCELOCAL/bin]
set DCEETC      [concat $DCELOCAL/etc]

# cell and env variables

set HOSTNAME	[exec hostname]
set CELLNAME	[exec $DCEBIN/getcellname]
set PID		[pid]

# set user identity

set LOGINNAME  [exec whoami]

# Location of temporay files and logs.

set WDIR	$env(STTMPDIR)   
set WORKDIR	[concat $WDIR\/$TESTNAME]
set CHBK        [concat $WORKDIR/ch_bk]
#set CHREST      [concat $WORKDIR/ch_rest]
set CHREST	/tmp/DCPSAN002/ch_bk

set BRDIR	[concat $env(STLOGDIR)/cdsfiles]
set TMPCH    	[concat $BRDIR/ch]
set TMPCDS	[concat $BRDIR/cache]

# Set log directory and file

set LDIR 	$env(STLOGDIR)
set LOGDIR	[concat $LDIR\/$TESTNAME\.$PID]
set PASSDIR	[concat $LOGDIR/pass]
set FAILDIR	[concat $LOGDIR/fail]
set PASS_FAIL_LOG [concat $LOGDIR/pass_fail_log]


# CDS admin and other directories

set CHDIR       [concat $DCELOCAL/var/directory/cds]
set CH          [concat $DCELOCAL/var/directory]
set CDSCACHE    [concat $DCELOCAL/var/adm/directory/cds]
set CACHE       [concat $DCELOCAL/var/adm/directory]

# Admin account and password

set CELLADMIN   cell_admin
set ADMIN_PW    -dce-

# clean up 
# 0 = no clean up
# 1 = clean up

set CEANUP 0

# exit on error flag
# 0 = exiting normally
# 1 = exiting on error

set EXIT_ON_ERROR 1
