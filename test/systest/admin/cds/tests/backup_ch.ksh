#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: backup_ch.ksh,v $
# Revision 1.1.13.3  1996/03/11  02:36:50  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:38  marty]
#
# Revision 1.1.13.2  1995/12/11  21:53:32  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:54:32  root]
# 
# Revision 1.1.8.1  1994/10/25  17:58:22  petero
# 	fix path to functions
# 	[1994/10/25  14:18:03  petero]
# 
# Revision 1.1.6.1  1993/10/27  15:34:47  bmw
# 	CR 9214: strip domain from hostname to get correct clearinghouse name
# 	[1993/10/22  14:45:52  bmw]
# 
# Revision 1.1.2.5  1993/03/30  19:50:51  pellis
# 	Added a sleep to allow dtsd to go away after dce.clean.
# 	[1993/03/30  19:48:18  pellis]
# 
# Revision 1.1.2.4  1993/02/05  16:01:24  pellis
# 	Fixes for temporary directory creation, split server configuration
# 	checking and ksh echo bug.
# 	[1993/02/03  23:53:56  pellis]
# 
# Revision 1.1.2.3  1993/02/05  15:16:28  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:35:10  cjd]
# 
# Revision 1.1.4.2  1993/02/04  21:43:38  cjd
# 	Embedded copyright notice
# 
# Revision 1.1.2.2  1992/12/26  21:38:07  pellis
# 	Initial version of test.
# 	[1992/12/26  21:33:11  pellis]
# 
# $EndLog$
#
#   FILE_NAME: backup_ch.ksh
#
#   COMPONENT_NAME: dce.admin_test
#
#   FUNCTIONS:
#
#   USAGE:
#
#	backup_ch.ksh
#
#   DESCRIPTION:
#
#	This script does a backup of a CDS clearinghouse by backing up the
#	following four files to a specified directory:
#
#	DCELOCAL/var/directory/cds/cds_files
#	DCELOCAL/var/directory/cds/<ch_name>_ch.checkpoint*[0-9]
#	DCELOCAL/var/directory/cds/<ch_name>_ch.tlog*[0-9]
#	DCELOCAL/var/directory/cds/<ch_name>_ch.version
#
#	This script must be run on the master CDS server and all CDS clients
#	must be stopped before the script can be executed.
#
#===============================================================================
#
# Source the functions file
#
. ./functions
#
# Initialize Command Options
#
echo_msg "-log-----: Checking Command Usage (${0})"
#
if [ "$#" -gt 1 ] ; then
        echo_msg "-log-----: USAGE: ${0} or ${0} <Directory to Save CDS Clearinghouse To>"
        exit 1
fi
#
# Definitions
#
echo_msg "-log-----: Defining Variables (${0})"
#
TESTNAME=bkch
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
RESTARTDCE=0
DCELOCAL=/opt/dcelocal
TESTDIR=${TESTNAME}_${HOSTNAME}
WORKDIR=`pwd`
TEMPDIR=${WORKDIR}/tmp
LOGDIR=${WORKDIR}/log
BACKUPDIR=${WORKDIR}/backup
if [ "$#" -eq 1 ] ; then
	MASTERBAK=${1}
else
	MASTERBAK=${BACKUPDIR}/CH_${TESTNAME}_${HOSTNAME}_${PID}
fi
get_cell_name
NSNAME=${CELLNAME}\#${HOSTNAME}
STDOUT=${TEMPDIR}/${TESTNAME}_${HOSTNAME}_${PID}_STDOUT
STDERR=${TEMPDIR}/${TESTNAME}_${HOSTNAME}_${PID}_STDERR
#
# Make sure the temporary working directory exists
#
if [ ! -d ${TEMPDIR} ]; then
	echo_msg "-log-----: Creating ${TEMPDIR}...\c)"
	mkdir ${TEMPDIR}
	echo_msg "Done (${0})"
fi
#
# Abort test if it's not being run on the CDS Server
#
echo_msg "-log-----: Getting Name Of The CDS Server (${0})"
#
getcdsserver ${HOSTNAME}
if [ $? -ne 0 ] ; then
	echo_msg ""
	echo_msg "-error---: ${HOSTNAME} Is Not Running As The CDS Server, This Script"
	echo_msg "-error---: Can Only Be Executed On a CDS Server (${0})"
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
	echo_msg "-error---: All DCE Clients Must Be Stopped Before Running This Script (${0})"
	exit 1
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
################################################################################
#
# STOP DCE
#
dcestatus ${HOSTNAME}
if [ $? -eq 0 ] ; then
	RESTARTDCE=1
	echo_msg "-log-----: Stopping DCE On ${HOSTNAME} (${0})"
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
fi
#
################################################################################
#
# BACKUP MASTER CLEARINGHOUSE
#
echo_msg "-log-----: Backing Up The Master CDS Clearinghouse (${0})"
echo_msg "-log-----: "
echo_msg "-log-----: From     : ${DCELOCAL}"
echo_msg "-log-----: To       : ${MASTERBAK}"
echo_msg "-log-----: "
#
if [ -d ${BACKUPDIR} ] ; then
	if [ -d ${MASTERBAK} ] ; then
		echo_msg ""
		echo_msg "-log-----: Original Backup Directory Exists, Moving It \c"
		echo_msg "To: (${0})"
		echo_msg "-log-----:	${MASTERBAK}.old"
		mv ${MASTERBAK} ${MASTERBAK}.old
	fi
else
	echo_msg ""
	echo_msg "-log-----: Backup Directory (${BACKUPDIR}) Created (${0})"
	mkdir ${BACKUPDIR}
fi
#
#mkdir ${MASTERBAK}
#
backup_cds_ch ${NSNAME} ${DCELOCAL} ${MASTERBAK}
#
################################################################################
#
# START DCE
#
if [ ${RESTARTDCE} = 1 ] ; then
	echo_msg "-log-----: Starting DCE On ${HOSTNAME} (${0})"
	ksh /etc/rc.dce
	dcestatus ${HOSTNAME}
	if [ $? -eq 1 ] ; then
		echo_msg ""
		echo_msg "-error---: DCE Did Not Restart (${0})"
		exit 1
	fi
fi
#
################################################################################
#
# CLEANUP
#
echo_msg "-log-----: Backup of Master CDS Clearinghouse Completed, Doing Cleanup"
MESSAGE="Remove stdout temporary file"
COMMAND="rm -f ${STDOUT}" ; OPTIONS="" ; ESTATUS=0 ; exec_test
MESSAGE="Remove stderr temporary file"
COMMAND="rm -f ${STDERR}" ; OPTIONS="" ; ESTATUS=0 ; exec_test
#
################################################################################
#
# DONE
#
end_stats
stats
#
################################################################################
