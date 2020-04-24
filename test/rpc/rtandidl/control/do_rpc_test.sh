#!/bin/ksh 
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: do_rpc_test.sh,v $
# Revision 1.1.7.2  1996/03/11  02:22:04  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:57:15  marty]
#
# Revision 1.1.7.1  1995/12/11  19:37:20  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:19:43  root]
# 
# Revision 1.1.4.6  1994/09/02  18:46:42  tom
# 	Remove bogus setting of RPC_DEFAULT_ENTRY_SYNTAX.
# 	[1994/09/02  18:46:24  tom]
# 
# Revision 1.1.4.5  1994/06/29  21:00:07  ohara
# 	fix up the ps stuff for SINIX
# 	[1994/06/07  22:50:10  ohara]
# 
# Revision 1.1.4.4  1994/06/10  20:44:52  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:16:37  devsrc]
# 
# Revision 1.1.4.3  1994/04/07  22:56:31  tom
# 	Fix up improvements to work when user shell is csh.
# 	[1994/04/07  22:55:54  tom]
# 
# Revision 1.1.4.2  1994/03/07  20:50:04  bmw
# 	added test for HP-UX to kill server processes
# 	[1994/03/07  20:49:37  bmw]
# 
# Revision 1.1.4.1  1994/03/07  20:26:03  bmw
# 	added cleanup function and trap for signals
# 	[1994/03/07  20:25:06  bmw]
# 
# Revision 1.1.2.1  1993/11/02  22:02:05  tom
# 	Initial checkin.
# 	[1993/11/02  22:01:45  tom]
# 
# $EndLog$
#

# do_rpc_test
#
# You need to be a superuser (local root account) to run auth test.
# therefore, being a root account is mandatory to run this test to 
# guarantee the test suceessful in all situations..  
#   
# This script should be run with PROTOCOL set to CN and DG.
#
# Usage:
#    do_rpc_test include_directory tsh_file [debugging_flag]
#
# Examples:
#	do_rpc_test  /usr/include all.tsh 
#
#	do_rpc_test /sandbox/export/at386/usr/include all_ns.tsh 63
#
# The include directory is needed for all.tsh, cmd_line.tsh and no_code.tsh
# For other tests, a dummy directory can be given.
#

if [ $# -ne 2  -a $# -ne 3 ]
then
	echo 
	echo "usage: do_rpc_test include_dir suite.tsh [debugging-level]"
	echo
	exit 1
fi


cleanup()
{
	trap "exit 1" 1 2 3 15
	echo "Stopping servers"
	case `uname` in
		HP-UX*)
	 	ps -ef | awk '/server|sofserv|mgmt_ser/ {print $2}' | xargs kill > /dev/null 2>&1
		;;
		SINIX*)
		/bin/ps -ef | awk '/server|sofserv|mgmt_ser/ {print $2}' | xargs kill > /dev/null 2>&1
		;;
		*)
		ps -eo comm,pid | awk '/server|sofserv|mgmt_ser/ {print $2}' | xargs kill > /dev/null 2>&1
		;;
	esac


	echo "Removing NS entries"
	dce_login cell_admin -dce- << DCE_LOGIN_EOF
		# remove test directories
		cdscp > /dev/null << CDSCP_EOF
			del obj /.:/NSTEST_DIR/profile_test
			del obj /.:/NSTEST_DIR/profile_test_export1_8_dup
			del obj /.:/NSTEST_DIR/profile_test_export_1_1
			del obj /.:/NSTEST_DIR/profile_test_export1_1
			del dir /.:/NSTEST_DIR
CDSCP_EOF
DCE_LOGIN_EOF
}


testsh_search_dirs="-I../moretests -I."

echo "export environment variables"

echo "export SERVERHOST=`hostname`"
export SERVERHOST=`hostname`
echo "export SERVER_NAME=server"
export SERVER_NAME=server
echo "export SERVER_KEY=server"
export SERVER_KEY=server
echo "export CLIENT_NAME=client"
export CLIENT_NAME=client
echo "export CLIENT_KEY=client"
export CLIENT_KEY=client
echo "export CALLER_KEY=-dce- "
export CALLER_KEY=-dce- 

#
# Make sure the user picks a PROTCOL
#
if [ -z "$PROTOCOL" ]; then
	echo "The environment variable PROTOCOL must be set to either"
	echo "ncacn_ip_tcp or ncadg_ip_udp before running this script."
	exit 1
else
	echo ""
	echo "PROTOCOL=$PROTOCOL"
fi 

INCL=$1 
export INCL

tsh_file=$2 
log="${tsh_file%.tsh}_${PROTOCOL##*_ip_}.log"

if [ $3x != x ]
then
	debug_flag="-d $3"
else
	debug_flag=
fi


trap "cleanup; exit 1" 1 2 3 15
# start servers
./run_server 
# We run as cell_admin throughout.

echo "doing dce_login"
dce_login cell_admin -dce-  <<DCE_LOG_END

# Create NSTEST_DIR  

echo "Add ns test entry"
cdscp << CDSCP_EOF
cre dir /.:/NSTEST_DIR
q
CDSCP_EOF

echo "Run $tsh_file tests"
../testsh/testsh $debug_flag -l ${log} $testsh_search_dirs $tsh_file  

DCE_LOG_END

cleanup
