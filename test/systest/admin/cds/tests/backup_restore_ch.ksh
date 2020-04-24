#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: backup_restore_ch.ksh,v $
# Revision 1.1.13.3  1996/03/11  02:36:54  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:39  marty]
#
# Revision 1.1.13.2  1995/12/11  21:53:33  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:54:33  root]
# 
# Revision 1.1.8.1  1994/10/25  17:59:04  petero
# 	fix path to functions
# 	[1994/10/25  14:18:35  petero]
# 
# Revision 1.1.6.1  1993/10/27  15:34:50  bmw
# 	CR 9214: strip domain name from hostname to get correct clearinghouse name
# 	[1993/10/22  14:47:17  bmw]
# 
# Revision 1.1.2.5  1993/03/30  19:50:55  pellis
# 	Added  several sleeps to allow dtsd to go away after dce.clean.
# 	[1993/03/30  19:48:52  pellis]
# 
# Revision 1.1.2.4  1993/02/05  16:01:36  pellis
# 	Several changes made to allow it to run in a split server cell and
# 	to cover up a ksh bug on OSF/1 1.1.1b5.
# 	[1993/02/03  20:23:55  pellis]
# 
# Revision 1.1.2.3  1993/02/05  15:16:32  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:35:15  cjd]
# 
# Revision 1.1.4.2  1993/02/04  21:43:43  cjd
# 	Embedded copyright notice
# 
# Revision 1.1.2.2  1992/12/26  21:38:13  pellis
# 	Initial version of test.
# 	[1992/12/26  21:33:21  pellis]
# 
# $EndLog$
#
#   FILE_NAME: backup_restore_ch.ksh
#
#   COMPONENT_NAME: dce.admin_test
#
#   FUNCTIONS:
#
#	check_ch()		- Check CDS Clearinghouse for test entries
#
#   USAGE:
#
#	backup_restore_ch.ksh
#
#   DESCRIPTION:
#
#	This test verifies that you can backup and restore a CDS clearinghouse
#	by backing up and restoring the following four files:
#
#	DCELOCAL/var/directory/cds/cds_files
#	DCELOCAL/var/directory/cds/<ch_name>_ch.checkpoint*[0-9]
#	DCELOCAL/var/directory/cds/<ch_name>_ch.tlog*[0-9]
#	DCELOCAL/var/directory/cds/<ch_name>_ch.version
#
#	This test must be run on the master CDS server and all CDS clients
#	must be stopped before the test can be executed.
#
#   DCE 1.0.2 TEST PLAN COVERAGE ( ++ = covered):
#
#	II)	CDS
#
#		A) Install
# ++			1) Server
#			2) Client
#		B) Configure
# ++			1) Initial CDS Server
#			2) Additional CDS Server(s)
#			3) CDS Client
#			4) Intercell
#				a) DNS
#					 i) show cell <cellname> as dns
#					ii) Update DNS Data File
#				b) GDS
#					 i) show cell <cellname> as gds
#					ii) Update DNS Data File
#		C) Create
#			1) Clearinghouse
#			2) Replica Read/Write
#			3) Replica Read Only
#		D) Disable/Enable
#			1) Master Replica
#				a) How Does the Client React When the Master
#				   Replica is Disabled?
#				b) How Does the Client React When the Master
#				   Replica is Reenabled?
#			2) Secondary Replica
#				a) How Does the Client React When the Secondary
#				   Replica is Disabled?
#				b) How Does the Client React When the Secondary
#				   Replica is Reenabled?
#			3) Modifying a Directory's Replica Set
#				a) How Does the Client React When a Replica is
#				   Excluded From the Replica Set?
#				b) How Does the Client React When a Replica is
#				   Reincluded in the Replica Set?
#		E) Update/Modify
#			1) Skulking a Directory
#				a) Modifying a Directory's Convergence
#			2) Modifying a Directory's Replica Set
#				a) Designating a New Master Replica
#					i) How Does the Client React When a New
#					   Master Replica is Designated?
#		F) Start/Stop
#			1) GDA
#				a) kill (Stop)
#					i) How Does the Client React When GDA is
#					   Stopped?
#				b) gdad (Start)
#					i) How Does the Client React When GDA is
#					   Restarted?
# ++		G) Backup
# ++			1) Clearinghouse
# ++			2) Namespace
# ++				a) Remote
#				b) By Replication
#		H) Cleanup
#		I) Monitoring
#		J) Reconfigure
#			1) Clearinghouse
#			2) Namespace
#				a) From Remote Backup
#				b) From Replica
#			3) Relocating a Clearinghouse
#				a) Disassociating a Clearinghouse from its Host
#				   Server
#				b) Copying the Clearinghouse Files to the Target
#				   Server
#				c) Starting the Clearinghouse on the Target
#				   Server
#		K) Unconfigure/Delete
#			1) Replica
#			2) Clearinghouse
#			3) Client
#				a) Remove All Namespace References to Client and
#				   Attempt to Reconfigure it
#		L) Crash Response
#			1) How Does the Client React When the Master Replica
#			   Goes Away During a Request?
#				a) Does the Client Switch to a Replica?
#			2) How Does the Server React When the Client Goes Away
#			   During a Request?
#			3) What Happens When a Skulk Update is Attempted When
#			   One Replica is Down?
#		M) Crash Recovery
# ++			1) From Remote Backup
#			2) From Replica
#		N) Compatibility
#			1) Use PMAX Namespace on RIOS
#			2) Use RIOS Namespace on PMAX
#			3) Use PMAX Clearinghouse on RIOS
#			4) Use RIOS Clearinghouse on PMAX
#			5) Use DCE 1.0.1 Namespace With DCE 1.0.2
#			6) Use DCE 1.0.1 Clearinghouse With DCE 1.0.2
#
#===============================================================================
#
# Source the functions file
#
. ./functions
#
# Definitions
#
echo_msg "-log-----: Defining Test Variables (${0})"
#
TESTNAME=bkrsch
COUNT=0
FAILED=0
HOSTNAME=`hostname | awk -F"." '{print \$1}'`
LOOPS=1
PASSED=0
PID=$$
PLATFORM=`uname -s`
LOGINNAME=`whoami`
PRINCIPAL=cell_admin
DCEPASSWORD=-dce-
DCELOCAL=/opt/dcelocal
TESTDIR=${TESTNAME}_${HOSTNAME}
WORKDIR=`pwd`
TEMPDIR=${WORKDIR}/tmp
LOGDIR=${WORKDIR}/log
BACKUPDIR=${WORKDIR}/backup
ORIGBAK=${BACKUPDIR}/orig_${TESTNAME}_${HOSTNAME}_${PID}
TESTBAK=${BACKUPDIR}/test_${TESTNAME}_${HOSTNAME}_${PID}
get_cell_name
CHNAME=${CELLNAME}\#${HOSTNAME}
STDOUT=${TEMPDIR}/${TESTNAME}_${HOSTNAME}_${PID}_STDOUT
STDERR=${TEMPDIR}/${TESTNAME}_${HOSTNAME}_${PID}_STDERR
#
# Initialize Command Options
#
echo_msg "-log-----: Checking Command Usage (${0})"
#
if [ "$#" -gt 0 ]
then
        echo_msg "-log-----: USAGE: $0"
        exit 1
fi
#
# Abort test if it's not being run on the CDS Server
#
echo_msg "-log-----: Getting Name Of The CDS Server (${0})"
#
getcdsserver ${HOSTNAME}
if [ $? -ne 0 ] ; then
	echo_msg ""
	echo_msg "-error---: ${HOSTNAME} Is Not Running As The CDS Server, This Test Can Only Be"
	echo_msg "-error---: Executed On a CDS Server (${0})"
	exit 1
fi
#
# Abort test if there are CDS Clients Running
#
echo_msg "-log-----: Getting The Names Of The CDS Clients If Any (${0})"
#
getcdsclients
STOPCLIENTS=0
for i in ${CDSCLIENTS} ; do
	if [ "${HOSTNAME}" != "${i}" ] ; then
		dcestatus ${i}
		if [ $? = 0 ] ; then
			getsecserver ${i}
			if [ $? = 1 ] ; then
				STOPCLIENTS=1
			fi
		fi
	fi
done
if [ ${STOPCLIENTS} = 1 ] ; then
	echo_msg ""
	echo_msg "-error---: All DCE Clients Must Be Stopped Before Running This Test (${0})"
	exit 1
fi
#
# Make sure the temporary working directory exists
#
if [ ! -d ${TEMPDIR} ]; then
	echo_msg "-log-----: Creating ${TEMPDIR}...\c)"
	mkdir ${TEMPDIR}
	echo_msg "Done (${0})"
fi
#
# Initialize the logfiles
#
initlogs ${TESTNAME}
#
# Create dce_login_noexec symbolic link if there isn't one
#
create_login_exec
#
# Define trap to cleanup after break
#
trap '	echo_msg ""
	echo_msg "-log-----: Stopping DCE On ${HOSTNAME} (${0})"
	ksh /etc/dce.clean
	unset KRB5CCNAME
	sleep 120
	dcestatus ${HOSTNAME}
	if [ $? -eq 0 ] ; then
		echo_msg ""
		echo_msg "-error---: The Following DCE Process(es) Did Not Stop: (${0})"
		echo_msg "${DCESTATUS}"
	fi
#
	echo_msg "-log-----: Restoring The Original CDS Clearinghouse (${0})"
	restore_cds_ch ${CHNAME} ${ORIGBAK} ${DCELOCAL}
#
	echo_msg "-log-----: Starting DCE On ${HOSTNAME} (${0})"
	ksh /etc/rc.dce
	dcestatus ${HOSTNAME}
	if [ $? -eq 1 ] ; then
		echo_msg ""
		echo_msg "-error---: DCE Did Not Restart (${0})"
		exit 1
	fi
#
	rm -f ${STDOUT}
	rm -f ${STDERR}
	rm -r ${TESTBAK}
	echo_msg "" | tee -a ${FULLLOG} | tee -a ${SHRTLOG}
	end_stats
	stats
	exit 1' 1 2 3 15
#
# Define Check Test Clearinghouse Function
#
check_ch()
{
#
if [ "${1}" = "fail" ] ; then
	WHEN=1
elif [ "${1}" = "pass" ] ; then
	WHEN=0
else
	echo_msg ""
	echo_msg "-error---: Invalid Option Specified (${0})"
fi
#
# login
#
do_login
#
# set up directory
#
MESSAGE="Check existence of test directory (list)"
COMMAND="cdscp list dir"
OPTIONS="/.:/${TESTDIR}"
ESTATUS=0
exec_test
#
MESSAGE="Check existence of test directory (show)"
COMMAND="cdscp show dir"
OPTIONS="/.:/${TESTDIR}"
ESTATUS=${WHEN}
exec_test
#
MESSAGE="Check existence of test object (list)"
COMMAND="cdscp list obj"
OPTIONS="/.:/${TESTDIR}/object"
ESTATUS=${WHEN}
exec_test
#
MESSAGE="Check existence of test object (show)"
COMMAND="cdscp show obj"
OPTIONS="/.:/${TESTDIR}/object"
ESTATUS=${WHEN}
exec_test
#
# Destroy Credentials
#
kdestroy
}
#
# Gather Starting Statistics
#
stats
#
################################################################################
#
# VALIDATE TEST CLEARINGHOUSE (SHOULD FAIL)
#
echo_msg "-log-----: Validating Test CDS Clearinghouse (${0})"
#
check_ch fail
#
################################################################################
#
# STOP DCE
#
echo_msg "-log-----: Stopping DCE On ${HOSTNAME} (${0})"
#
ksh /etc/dce.clean
unset KRB5CCNAME
sleep 120
dcestatus ${HOSTNAME}
if [ $? -eq 0 ] ; then
	echo_msg ""
	echo_msg "-error---: The Following DCE Process(es) Did Not Stop: (${0})"
	echo_msg "${DCESTATUS}"
	exit 1
fi
#
################################################################################
#
# BACKUP ORIGINAL CLEARINGHOUSE
#
echo_msg "-log-----: Backing Up The Original CDS Clearinghouse (${0})"
#
if [ -d ${BACKUPDIR} ] ; then
	if [ -d ${ORIGBAK} ] ; then
		echo_msg ""
		echo_msg "-log-----: Original Backup Directory Exists, Moving It \c"
		echo_msg "To: (${0})"
		echo_msg "-log-----:	${ORIGBAK}.old"
		mv ${ORIGBAK} ${ORIGBAK}.old
	fi
else
	echo_msg ""
	echo_msg "-log-----: Backup Directory (${BACKUPDIR}) Created (${0})"
	mkdir ${BACKUPDIR}
fi
#
mkdir ${ORIGBAK}
#
backup_cds_ch ${CHNAME} ${DCELOCAL} ${ORIGBAK}
#
################################################################################
#
# START DCE
#
echo_msg "-log-----: Starting DCE On ${HOSTNAME} (${0})"
#
ksh /etc/rc.dce
dcestatus ${HOSTNAME}
if [ $? -eq 1 ] ; then
	echo_msg ""
	echo_msg "-error---: DCE Did Not Restart (${0})"
	exit 1
fi
#
################################################################################
#
# CREATE TEST ENTRIES IN CLEARINGHOUSE
#
echo_msg "-log-----: Creating Test CDS Clearinghouse (${0})"
#
# login
#
do_login
#
# set up directory
#
MESSAGE="Create test directory in CDS clearinghouse"
COMMAND="cdscp create directory"
OPTIONS="/.:/${TESTDIR}"
ESTATUS=0
exec_test
#
MESSAGE="Set test directory Convergence to high"
COMMAND="cdscp set directory"
OPTIONS="/.:/${TESTDIR} CDS_Convergence = high"
ESTATUS=0
exec_test
#
MESSAGE="Enable skulking on test directory"
COMMAND="cdscp set directory"
OPTIONS="/.:/${TESTDIR} to skulk"
ESTATUS=0
exec_test
#
MESSAGE="Create test object in CDS clearinghouse"
COMMAND="cdscp create object"
OPTIONS="/.:/${TESTDIR}/object \
 	CDS_Class = object_class CDS_ClassVersion = 1.0"
ESTATUS=0
exec_test
#
# Destroy Credentials
#
kdestroy
#
################################################################################
#
# VALIDATE TEST CLEARINGHOUSE (SHOULD PASS)
#
echo_msg "-log-----: Validating Test CDS Clearinghouse (${0})"
#
check_ch pass
#
################################################################################
#
# STOP DCE
#
echo_msg "-log-----: Stopping DCE On ${HOSTNAME} (${0})"
#
ksh /etc/dce.clean
unset KRB5CCNAME
sleep 120
dcestatus ${HOSTNAME}
if [ $? -eq 0 ] ; then
	echo_msg ""
	echo_msg "-error---: The Following DCE Processes Did Not Stop: (${0})"
	echo_msg "${DCESTATUS}"
	exit 1
fi
#
################################################################################
#
# BACKUP TEST CLEARINGHOUSE
#
echo_msg "-log-----: Backing Up The Test CDS Clearinghouse (${0})"
#
if [ -d ${BACKUPDIR} ] ; then
	if [ -d ${TESTBAK} ] ; then
		echo_msg ""
		echo_msg "-log-----: Test Backup Directory Exists, Moving It \c"
		echo_msg "To: (${0})"
		echo_msg "-log-----:	${TESTBAK}.old"
		mv ${TESTBAK} ${TESTBAK}.old
	fi
else
	echo_msg ""
	echo_msg "-log-----: Backup Directory (${BACKUPDIR}) Created (${0})"
	mkdir ${BACKUPDIR}
fi
#
mkdir ${TESTBAK}
#
backup_cds_ch ${CHNAME} ${DCELOCAL} ${TESTBAK}
#
################################################################################
#
# DELETE CLEARINGHOUSE
#
echo_msg "-log-----: Removing The Test CDS Clearinghouse (${0})"
#
delete_cds_ch ${CHNAME} ${DCELOCAL}
#
################################################################################
#
# RESTORE ORIGINAL CLEARINGHOUSE
#
echo_msg "-log-----: Restoring The Original CDS Clearinghouse (${0})"
#
restore_cds_ch ${CHNAME} ${ORIGBAK} ${DCELOCAL}
#
################################################################################
#
# START DCE
#
echo_msg "-log-----: Starting DCE On ${HOSTNAME} (${0})"
#
ksh /etc/rc.dce
dcestatus ${HOSTNAME}
if [ $? -eq 1 ] ; then
	echo_msg ""
	echo_msg "-error---: DCE Did Not Restart (${0})"
	exit 1
fi
#
################################################################################
#
# VALIDATE TEST CLEARINGHOUSE (SHOULD FAIL)
#
echo_msg "-log-----: Validating Test CDS Clearinghouse (${0})"
#
check_ch fail
#
################################################################################
#
# STOP DCE
#
echo_msg "-log-----: Stopping DCE On ${HOSTNAME} (${0})"
#
ksh /etc/dce.clean
unset KRB5CCNAME
sleep 120
dcestatus ${HOSTNAME}
if [ $? -eq 0 ] ; then
	echo_msg ""
	echo_msg "-error---: The Following DCE Processes Did Not Stop: (${0})"
	echo_msg "${DCESTATUS}"
	exit 1
fi
#
################################################################################
#
# DELETE CLEARINGHOUSE
#
echo_msg "-log-----: Removing The Original CDS Clearinghouse (${0})"
#
delete_cds_ch ${CHNAME} ${DCELOCAL}
#
################################################################################
#
# RESTORE TEST CLEARINGHOUSE
#
echo_msg "-log-----: Restoring The Test CDS Clearinghouse (${0})"
#
restore_cds_ch ${CHNAME} ${TESTBAK} ${DCELOCAL}
#
################################################################################
#
# START DCE
#
echo_msg "-log-----: Starting DCE On ${HOSTNAME} (${0})"
#
ksh /etc/rc.dce
dcestatus ${HOSTNAME}
if [ $? -eq 1 ] ; then
	echo_msg ""
	echo_msg "-error---: DCE Did Not Restart (${0})"
	exit 1
fi
#
################################################################################
#
# VALIDATE TEST CLEARINGHOUSE (SHOULD PASS)
#
echo_msg "-log-----: Validating Test CDS Clearinghouse (${0})"
#
check_ch pass
#
################################################################################
#
# STOP DCE
#
echo_msg "-log-----: Stopping DCE On ${HOSTNAME} (${0})"
#
ksh /etc/dce.clean
unset KRB5CCNAME
sleep 120
dcestatus ${HOSTNAME}
if [ $? -eq 0 ] ; then
	echo_msg ""
	echo_msg "-error---: The Following DCE Processes Did Not Stop: (${0})"
	echo_msg "${DCESTATUS}"
	exit 1
fi
#
################################################################################
#
# DELETE CLEARINGHOUSE
#
echo_msg "-log-----: Removing The Test CDS Clearinghouse (${0})"
#
delete_cds_ch ${CHNAME} ${DCELOCAL}
#
################################################################################
#
# RESTORE ORIGINAL CLEARINGHOUSE
#
echo_msg "-log-----: Restoring The Original CDS Clearinghouse (${0})"
#
restore_cds_ch ${CHNAME} ${ORIGBAK} ${DCELOCAL}
#
################################################################################
#
# START DCE
#
echo_msg "-log-----: Starting DCE On ${HOSTNAME} (${0})"
#
ksh /etc/rc.dce
dcestatus ${HOSTNAME}
if [ $? -eq 1 ] ; then
	echo_msg ""
	echo_msg "-error---: DCE Did Not Restart (${0})"
	exit 1
fi
#
################################################################################
#
# VALIDATE TEST CLEARINGHOUSE (SHOULD FAIL)
#
echo_msg "-log-----: Validating Test CDS Clearinghouse (${0})"
#
check_ch fail
#
################################################################################
#
# REMINDER TO START DCE ON CLIENTS
#
if [ ! -z "${CDSCLIENTS}" ] ; then
	echo_msg "-log-----: The Following DCE Clients Can Now Restarted (${0})"
	for i in ${CDSCLIENTS} ; do
		if [ "${HOSTNAME}" != "${i}" ] ; then
			echo_msg "		${i}"
		fi
	done
	echo_msg ""
fi
#
################################################################################
#
# CLEANUP
#
echo_msg "-log-----: Test Completed, Doing Cleanup"
MESSAGE="Remove stdout temporary file"
COMMAND="rm -f ${STDOUT}" ; OPTIONS="" ; ESTATUS=0 ; exec_test
MESSAGE="Remove stderr temporary file"
COMMAND="rm -f ${STDERR}" ; OPTIONS="" ; ESTATUS=0 ; exec_test
MESSAGE="Remove original clearinghouse backup directory"
COMMAND="rm -r ${ORIGBAK}" ; OPTIONS="" ; ESTATUS=0 ; exec_test
MESSAGE="Remove test clearinghouse backup directory"
COMMAND="rm -r ${TESTBAK}" ; OPTIONS="" ; ESTATUS=0 ; exec_test
#
################################################################################
#
# DONE
#
end_stats
stats
#
################################################################################
