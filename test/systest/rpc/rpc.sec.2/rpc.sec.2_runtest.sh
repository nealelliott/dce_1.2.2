#/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: rpc.sec.2_runtest.sh,v $
# Revision 1.1.94.2  1996/03/11  02:46:44  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:27  marty]
#
# Revision 1.1.94.1  1995/12/11  22:44:02  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1994/08/04  15:12 UTC  bissen
# 	merge from HPDCE01
# 	[1995/12/11  22:20:43  root]
# 
# Revision 1.1.92.1  1994/02/22  18:39:01  ohara
# 	update ps options on SINIX
# 	[1994/02/18  16:09:44  ohara]
# 
# 	HP revision /main/HPDCE01/1  1994/01/27  21:26  bissen
# 	merge kk and hpdce01
# 
# Revision 1.1.4.2  1993/08/25  20:13:13  bissen
# 	Changes for TET
# 	[1993/08/25  20:12:15  bissen]
# 
# Revision 1.1.89.4  1993/10/20  19:37:01  ohara
# 	svr4 ref port
# 	[1993/10/20  19:36:16  ohara]
# 
# Revision 1.1.89.3  1993/10/18  13:40:14  cmckeen
# 	Used RSH_COMM for all remote shell calls.
# 
# 	For CR 7721:
# 
# 	Moved the code to create log and tmp directories outside the command
# 	line parsing and after the "hi" messages.  This way the directories
# 	will be create if the paths are specified in the configuration file.
# 	[1993/10/18  13:39:36  cmckeen]
# 
# Revision 1.1.89.2  1993/09/16  14:58:47  cmckeen
# 	Changed FAILURE to FAIL to match the tet_result code
# 	[1993/09/16  14:55:17  cmckeen]
# 
# Revision 1.1.89.1  1993/09/09  19:12:58  cmckeen
# 	 HP's TET'ized version
# 	[1993/09/09  19:11:55  cmckeen]
# 
# Revision 1.1.7.2  1993/08/16  14:36:31  eheller
# 	File tetified.
# 	[1993/07/23  17:45:41  eheller]
# 
# Revision 1.1.1.5  1993/05/04  22:22:15  mgm
# 	    Loading drop DCE1_0_2FNL
# 
# Revision 1.1.2.9  1993/03/02  17:59:01  mhickey
# 	Fixed logic error when list of failed startups is exhausted.
# 	[1993/03/02  17:57:50  mhickey]
# 
# Revision 1.1.2.8  1993/02/25  21:09:51  mhickey
# 	Actually put timing switch on client arg list.
# 	[1993/02/25  21:09:25  mhickey]
# 
# Revision 1.1.2.7  1993/02/05  15:22:58  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:44:12  cjd]
# 
# Revision 1.1.4.2  1993/02/04  22:13:24  cjd
# 	Embedded copyright notice
# 
# Revision 1.1.2.6  1993/01/22  20:44:47  mhickey
# 	Added code to get temp files removed regardless of value of
# 	CLEANUP, and to cleanup at end of boundary runs.  Also fixed
# 	the LAST bug in timed run arg parsing (I hope to whomever!).
# 	Fixed code for removing log and temp files so that no error
# 	is printed if they don't exist.
# 	[1993/01/20  20:05:15  mhickey]
# 
# Revision 1.1.2.4  1993/01/13  19:35:38  mhickey
# 	Added options to allow specification of the
# 	log dir, temp dir, and bin dir.
# 
# 	Fixed call to osf1 systems do workaround
# 	osf1 fd limit problem.
# 	[1993/01/13  19:35:22  mhickey]
# 
# Revision 1.1.3.4  1993/01/07  03:38:19  mhickey
# 	really, REALLY, REALLY fixed the hours vs passes problem,
# 	added  read of .profile to rsh to osf1 systems.
# 
# Revision 1.1.3.3  1993/01/06  22:01:24  mhickey
# 	added fixe for rsh to osf1.1.1
# 
# Revision 1.1.3.2  1992/12/10  16:20:07  mhickey
# 	fixed handling of minutes, hours, and passes
# 
# Revision 1.1.2.3  1992/12/03  20:12:11  mhickey
# 	Fixed a typo in a variable name in an echo.
# 	[1992/12/03  20:11:51  mhickey]
# 
# Revision 1.1.2.2  1992/12/03  19:53:01  mhickey
# 	Initial check in of the script that runs the rpc.sec.2
# 	system test.
# 
# 	Defect 5887.
# 	[1992/12/03  19:14:54  mhickey]
# 
# $EndLog$
# 
# rpc.sec.2_runtest.sh
# 
# ASSOCIATION
#    This file is part of the rpc.sec.2 DCE System Test.
# 
#    The rpc.sec.2 system test corresponds to test rpc.sec.2 in the DCE1.0.2
#    Core Component System Test plan.  For more information on the operation
#    and goals of the rpc.sec.2 system test, see the test plan, or the README
#    file in this directory.
#
#
# FUNCTION
#    Runs the rpc.sec.2 test program.  Specifically, this script starts the 
#    rpc.sec.2_srv program on a machine specified in the configuratoin file, or
#    via the -M option on the command line, then, after checking to make sure
#    that rcp and rsh to machines named as clients in the config file, or via
#    the -m option on the command line works, and that some required files
#    exist, begins to look for the maximum concurrent calls to the server.
#    This is done by taking the estimated max for concurrent clients (passed
#    on the command line via the -c option) as a starting point, and then:
#        1) for some number of tries (defined in the configuration file),
#             a) figure out, based on the number of clients to run and the
#                number of machines to run on, what the number of clients
#                per machine is.
#             b) start the calculated number of clients on each machine, 
#                logging to a file on the local machine.   The number of 
#                clients running on each protocol is determined by the value
#                supplied with the -u command line option.  This option 
#                specifies the percentage of datagram (ncadg_ip_udp) clients
#                to run.  As each client is started the sequence number, 
#                host, and protocol for the client is logged to a file.
#             c) Wait for an interval specified in the config file for the
#                the clients to startup, then grep for the string READY in 
#                each log.  If found, this means the client has established
#                it's identity, found a binding to the server, and is ready 
#                to make rpc calls.  If not found, it is assumed that the 
#                client could not get setup.  The grep output is parsed into 
#                a report of the state of the clients.
#             d) Create the sync file on each client machine.
#             e) Wait for the clients to execute the RPC's
#
#
# CALLED BY
#    The user.
#
# CALLS INTO
#    rpc.sec.2.conf (definitions)
#
# PREREQUISITES
#    Assumes a DCE cell is running.
#

#
# Initialize variables
#
# first get the defaults from the config file
#
 
 . $TET_ROOT/lib/xpg3sh/tetapi.sh
 . ${RPCSEC2_CONF:-rpc.sec.2.conf}
 . $RPCSEC2_BIN_DIRPATH/rpc.sec.2_conf_util.sh

 tet_setcontext

#
# set global variables we need 
#
# Sync file name
#

 rs2_rt_cli_sync_fn="/tmp/start_cli.$$"

#
# path to the rpc.sec.2 executables and scripts
#

 echo $RPCSEC2_BIN_DIRPATH
 rs2_rt_exe_path=$RPCSEC2_BIN_DIRPATH

#
# client keytab file path
#

 rs2_rt_cli_kytb_path=$RPCSEC2_CLI_KEYTAB_DIRPATH/$RPCSEC2_CLI_KEYTAB_FN

#
# temp directory for this scripts operational files 
#

 rs2_rt_tmp=$RPCSEC2_TEMP_DIRPATH

#
# Log file base
#

 rs2_rt_log_base=$RPCSEC2_LOG_DIRPATH

#
# this machines os type
#

 rs2_rt_local_os=`uname`

#
# ps options that make sure that the entire command string for each proc is 
# printed
#

 if [ "$rs2_rt_local_os" = "OSF1" -o "$rs2_rt_local_os" = "AIX" ]
  then
   rs2_rt_pswideopts=wax
 fi
 if [ "$rs2_rt_local_os" = "HP-UX" ]
  then
   rs2_rt_pswideopts=ef
 fi
 if ["$rs2_rt_local_os" = "SINIX-D" ]
   then
    rs2_rt_pswideopts=-ef
 fi

#
# default protection level
#

 rs2_rt_prot_lev=$RPCSEC2_PROT_LEVEL

#
# default authorization service
#

 rs2_rt_authn_svc="-a $RPCSEC2_AUTHN_SERVICE"

#
# Initialize some variables
#

 rs2_rt_srv_started=0
 rs2_rt_bndry_mode=""
 rs2_rt_cli_to_start=0
 rs2_rt_cli_mach=""
 rs2_rt_cli_p_mach=0
 rs2_rt_cli_fudge=0
 rs2_rt_cli_this_mach=0
 rs2_rt_cli_on_mach=0
 rs2_rt_udp_cli=0
 rs2_rt_tcp_cli=0
 rs2_rt_udp_platfm=""
 rs2_rt_udp_platfm=""
 rs2_rt_cli_started=0
 rs2_rt_cli_seq=1
 rs2_rt_localhost=`hostname`
 rs2_rt_no_bound=""
 rs2_rt_nelems=$RPCSEC2_CLI_ARRAY_ELEMS

#
# now set default values for what we might get on the command line
#
# Number of  clients to start
#

 NCLIENTS=$RPCSEC2_CLI_TO_RUN

#
# Whether to ignore client startup errors.  0 = don't ignore

 IGNORE=0

#
# Hours to run
#

 HOURS=$RPCSEC2_CLI_DEF_RUN_INTVL

#
# Minutes to run
#

 MINS=10

#
# Report interval
#

 REP_INT=$RPCSEC2_CLI_DEF_REP_INTVL

#
# Percent of clients to run with DUP protocol bindings
#

 UDPPCT=$RPCSEC2_UDP_PCT

#
# Passes to run  (mutually exclusive with HOURS and MINS)
#

 PASSES=0

#
# Machines on which to run clients
#

 CLIENTS=$RPCSEC2_CLI_MACHINES

#
# Whether or not to time the RPCs in the clients. 0 = don't time
#

 TIMING=""

#
# Whether or not to print debug info in the clients. "" = don't print
#

 DEBUG_CLI=""

#
# default config file name
#

 CONFIG=${RPCSEC2_CONF:-rpc.sec.2.conf}

#
# Whether or not to print debug info in the server. "" = don't print
#

 DEBUG_SRV=""

#
# Restart server if already running?
#

 RESTART_SRV=0

#
# cleanup log files?
#

 CLEANUP=1

##################################################################
##    Functions
##################################################################

 trap 'rs2_rt_killjobs; \
       rs2_rt_cleanup; \
       rs2_rt_print_bye; \
       exit 1' 2

#
# rs2_rt_killjobs
#
#   Kills the clients we have running in an orderly manner (ie: they can print
#   one last status line before exiting.
#

 rs2_rt_killjobs()
 {
  echo ""
  echo "   Beginning killing of client jobs at `date`"
  echo "   ------------------------------------------------"
  echo "     Looking for client jobs..."
  rs2_rt_kj_jobs=`ps $rs2_rt_pswideopts | grep rpc.sec.2_cli | grep -v "grep rpc.sec.2" | awk '{jobs=jobs " " $1;} END { print jobs;}'`
  if [ -n "$rs2_rt_kj_jobs" ]
   then
    echo "     Found the following jobs: $rs2_rt_kj_jobs..."
    echo "     Attempting to kill all jobs with SIGINT..."
    eval "kill -2 $rs2_rt_kj_jobs"
    rs2_rt_kj_jobs=`ps $rs2_rt_pswideopts | grep rpc.sec.2_cli | grep -v "grep rpc.sec.2" | awk '{jobs=jobs " " $1;} END { print jobs;}'`
   
    if [ "$rs2_rt_kj_jobs" != "" ]
     then
      echo "     Couldn't kill the following jobs with SIGINT: $rs2_rt_kj_jobs"
      echo "     Attempting to kill remaining jobs with SIGKILL..."
      eval "kill -9 $rs2_rt_kj_jobs"
      rs2_rt_kj_jobs=`ps $rs2_rt_pswideopts | grep rpc.sec.2_cli | grep -v "grep rpc.sec.2" | awk '{jobs=jobs " " $1;} END { print jobs;}'`
   
      if [ "$rs2_rt_kj_jobs" != "" ]
       then
        echo "     Cannot kill the following jobs: $rs2_rt_kj_jobs"
       else 
        echo "     All jobs killed..."
      fi
     else
      echo "     All jobs killed..."
    fi
   else
    echo "     No client jobs active..."
  fi
  echo "   --------------------------------------------------"
  echo "   Finished killing client jobs at `date`"
  echo ""
 }

#
# rs2_rt_print_hi
# 
#   prints a little banner, then prints the parameters we are running with
#

 rs2_rt_print_hi()
 {
  echo "Beginning DCE RPC/Security System Test Execution at `date`"
  echo "-------------------------------------------------------------------"
  echo ""
  echo "   Test execution parameters are:"
  echo "   ------------------------------"
  if [ "$rs2_rt_cli_mach" = "$rs2_rt_localhost" ]
   then
    echo "   - Running $rs2_rt_cli_to_start clients on: $rs2_rt_localhost"
   else
    echo "   - Running $rs2_rt_cli_to_start clients on: $rs2_rt_cli_mach"
    echo "     ($rs2_rt_cli_p_mach clients per machine, $rs2_rt_cli_fudge machines"
    echo "      will run one extra client)"
  fi
  echo "   - Running $rs2_rt_nudp UDP clients, $rs2_rt_ntcp TCP clients"
  pr_pct=`echo "$rs2_rt_udppct * 100; quit" | bc`
  echo "     (approximately ${pr_pct}% UDP clients)"
  if [ "$rs2_rt_bndry_mode" = "-b" ]
   then
    echo "   - Running in boundary mode"
   else
    if [ $PASSES -eq 0 ]
     then
      echo "   - Running for $HOURS hrs, $MINS mins, reports every $REP_INT mins"
     else
      echo "   - Running for $PASSES passes, reports every $REP_INT passes"
    fi
  fi
  svc_str=`echo $rs2_rt_authn_svc | awk '{print $2}'`
  echo "   - Client and server using authorization service $svc_str"
  echo "   - Client and server using protection level $rs2_rt_prot_lev"
  echo "   - Using configuration file $rs2_rt_conf_file"
  [ $IGNORE -eq 1 ] && echo "   - Ignoring errors in starting clients"
  [ $RESTART_SRV -eq 1 ] && echo "   - Will attempt to restart server if running"
  [ "$DEBUG_SRV" != "" ] && echo "   - Server is printing debug information"
  [ "$DEBUG_CLI" != "" ] && echo "   - Clients are printing debug information"
  [ "$TIMING" = "-t" ] && echo "   - Clients are timing RPC's"
  echo "   - Clients passing structs containing arrays with $rs2_rt_nelems elements"
  echo ""
  echo "   Executables:" 
  echo "   ------------"
  echo "   Expect executables to be in directory $rs2_rt_exe_path"
  echo ""
  echo "   Log Files:"
  echo "   ----------"
  [ $rs2_rt_srv_started -eq 1 ] && \
     echo "   - Server log file is $rs2_rt_srv_log_base"
  echo "   - Client log files are $rs2_rt_cli_log_base.*"
  [ $CLEANUP -eq 0 ] && \
     echo "   - Log files will be saved after test is run" ||\
     echo "   - Log files will be removed after test run"
  echo "   --------------------------------------------------"
  echo ""
  echo ""
 }

#
# rs2_rt_print_bye
#
#   prints a short ending banner
#
 
rs2_rt_print_bye()
 {
  echo "-------------------------------------------------------------------"
  echo "   DCE RPC/Security System Test Execution Completed at `date`"
 }

#
# rs2_rt_cleanup()
#

 rs2_rt_cleanup()
 {
 
 #
 # Tell the user
 #

  if [ $CLEANUP -eq 1 ]
   then
    echo ""
    echo "   Starting log cleanup at `date`"
    echo "   ------------------------------"
    if [ $rs2_rt_srv_started -eq 1 ]
     then
      echo "     Cleaning up server log $rs2_rt_srv_log_base"
      rm -f "$rs2_rt_srv_log_base"
      is_gone=`ls $rs2_rt_srv_log_base | grep "^-rw"`
      if [ "$isgone" != "" ] 
       then
        echo "    Could not remove $rs2_rt_srv_log_base"
       else
        echo "    $rs2_rt_srv_log_base removed."
      fi
    fi
    echo "     Cleaning up client log files $rs2_rt_cli_log_base.*"
    rs2_rt_cli_logs=`ls $rs2_rt_cli_log_base.* 2>> /dev/null`
    for i in $rs2_rt_cli_logs
     do
      rm -f $i
      isgone=`ls -l $i 2>> /dev/null | grep "^-rw"`
      if [ "$isgone" != "" ]
       then
        echo "        Could not remove $i"
       else
        echo "        $i removed."
      fi
     done
  fi
  echo ""
  echo "   Starting temp file cleanup at `date`"
  echo "   ------------------------------------"
  rs2_rt_tfiles=`ls $rs2_rt_tmp/rs2_*$$ 2>> /dev/null`
  for i in $rs2_rt_tfiles
   do
    rm -f $i
    isgone=`ls -l $i 2>> /dev/null | grep "^-rw"`
    if [ "$isgone" != "" ]
     then
      echo "        Could not remove $i"
     else
      echo "        $i removed."
     fi
   done
   echo "     Cleaning up sync files..."
   for i in $rs2_rt_cli_mach
    do
     if [ "$i" != "$rs2_rt_localhost" ]
      then
       RSH="$RSH_COMM $i"
      else
       RSH=
     fi
     $RSH rm -f $rs2_rt_cli_sync_fn
     isgone=`$RSH ls -l $rs2_rt_cli_sync_fn 2>> /dev/null | grep "^-rw"`
     if [ "$isgone" != "" ]
      then
       echo "        Could not remove $rs2_rt_cli_sync_fn on $i"
      else
       echo "        $rs2_rt_cli_sync_fn removed on $i."
     fi
    done
   
   echo "   ------------------------------------"
   echo "   Cleanup complete at `date`"
   echo ""
 }

#
# rs2_rt_usage
#
#   prints the usage message
#

 rs2_rt_usage()
 {
  echo ""
  echo "Usage: rpc.sec.2_runtest.sh -C <num> [-a authn_svc][-c <mach> -c <mach>...][-b][-d]"
  echo "           [-D] [-i <intrvl>] [-I] [-t] [-u <pct_udp>] [-f <config>]"
  echo "           [ [ [-h <hours>][-m <mins>] ] | [-p passes] ] "
  echo "           [-l prot_lev][-H][-R][-S][-s size] [-T path] [-B path]"
  echo "           [-L path]"
  echo "Where:"
  echo "       -a authn_svc specifies the authorization service the client"
  echo "                    and server are to use when comunicating.  Must"
  echo "                    be one of \"secret\" or \"none\"."
  echo "       -B path      specifies the path on each client machine where"
  echo "                    the rpc.sec.2 binaries are stored."
  echo "       -b           specifies boundary mode testing"
  echo "       -C <num>     specifies the number of clients to run"
  echo "       -c <mach>    specifies a machine on which to run a client"
  echo "       -d           specifies that client debug info is to be printed"
  echo "       -D           specifies that server debug info is to be printed"
  echo "       -f <config>  specifies the path to the configuration file"
  echo "       -H           prints this help message"
  echo "       -h <hours>   specifies hours to run"
  echo "       -I           specifies that client startup errors are to be"
  echo "                    ignored"
  echo "       -i <intrvl>  specifies the report interval.  This is in passes"
  echo "                    if the test is to run for a number of passes, or"
  echo "                    in minutes if the test is to run for a period of"
  echo "                    time"
  echo "       -L path      specifies the path to the log directory"
  echo "       -l <prot_lev> specifies the protection level to use for the 
  echo "                     client and server.  This must be one of "none", 
  echo "                     "conn", "call", "pkt", "integ", or "priv".  Note"
  echo "                     that unless you are sure of the protection level"
  echo "                     in use by the server that is running, you should"
  echo "                     also use the -R option to kill the existing"
  echo "                     server and restart one with the desired"
  echo "                     protection level."
  echo "       -m <mins>    specifies minutes to run"
  echo "       -p <passes>  specifies passes to run"
  echo "                    Note: -p cannot be specified if -h or -m is"
  echo "                          specified and vice-versa"
  echo "       -T path      specifies the path to the temp directory"
  echo "       -t           specifies that clients are to time RPC's"
  echo "       -u <pct_udp> specifies percentage of clients to be run using"
  echo "                    UDP bindings"
  echo "       -R           specifies that any running server is to be"
  echo "                    killed and a new server is to be started"
  echo "       -S           specifies that log files are to be saved"
  echo "       -s           specifies the size of the array passed in RPC's"
  echo ""
 }

#########################################################################
###  Main program
#########################################################################

#
# parse the command line
#

 VAL_NEEDED="NONE"
 for i in $*
  do
   case $i in 

    -*)
       if [ "$VAL_NEEDED" != "NONE" ]
        then
         echo "Missing value for $VAL_NEEDED"
         rs2_rt_usage
         tet_result FAIL
         exit 1
       fi

       case $i in 
          -a) # authn service
              VAL_NEEDED=AUTHN_SVC
                 ;;

          -B) # the path to the binaries on the clients
              # we accept this at face value since we have no
              # quick way to check
              VAL_NEEDED=BINDIR
                 ;;

          -b) # boundary mode
              if [ "$rs2_rt_no_bound" = "" ]
               then
                rs2_rt_bndry_mode=-b
                HOURS=0; MINS=0; PASSES=0
               else
                echo "can't specify -b with -h, -m or -p"
                rs2_rt_usage
                tet_result FAIL
                exit 1
              fi
                  ;;

          -C) # number of clients to run
              VAL_NEEDED=NCLI
                  ;;

          -c) # another client machine
              VAL_NEEDED=CLI_MACH
                  ;;

          -D) # debug the Server
              DEBUG_SRV=-d
                  ;;

          -d) # debug the clients
              DEBUG_CLI=-d
                  ;;

          -f) # config file path
              VAL_NEEDED=CONFIG_FN
                  ;;

          -H) # want usage
              rs2_rt_usage
              exit 0
                  ;;

          -h) # Hours to run
              hrs_prob=0
              if [ "$rs2_rt_bndry_mode" = "-b" ]
               then
                hrs_prob=1
                echo "can't specify -b with -h, -m or -p"
               elif [ "$rs2_rt_no_bound" = "-p" ]
                then
                 hrs_prob=1
                 echo "Can't specify -p with -h or -m"
              fi
              if [ $hrs_prob -eq 1 ]
               then
                rs2_rt_usage
                exit 1
              fi
              VAL_NEEDED=HOURS
              rs2_rt_no_bound="-h"
                  ;;

          -I) # Ignore client startup errors
              IGNORE=1
                  ;;

          -i) # report interval
              VAL_NEEDED=REP_INT
                  ;;

          -L) # the directory in which to store logs. 
              VAL_NEEDED=LOGDIR
                  ;;

          -l) # protection level
              VAL_NEEDED=PROT_LEV
                  ;;

          -m) # Minutes to run 
              min_prob=0
              if [ "$rs2_rt_bndry_mode" = "-b" ]
               then
                min_prob=1
                echo "can't specify -b with -h, -m or -p"
               elif [ "$rs2_rt_no_bound" != "-p" ]
                then
                 min_prob=1
                 echo "Can't specify -p with -h or -m"
             fi
             if [ $min_prob -eq 1 ]
              then
               rs2_rt_usage
               exit 1
             fi
             VAL_NEEDED=MINS
             rs2_rt_no_bound="-m"
                  ;;

          -p) # passes
              pass_prob=0
              if [ "$rs2_rt_bndry_mode" = "-b" ]
               then
                pass_prob=1
                echo "Can't specify -p with -h, -m or -p"
               elif [ "$rs2_rt_no_bound" != "" ]
                then
                 pass_prob=1
                 echo "Can't specify -p with -h or -m"
              fi
              if [ $pass_prob -eq 1 ]
               then
                rs2_rt_usage
                exit 1
              fi
              VAL_NEEDED=PASSES
              rs2_rt_no_bound="-p"
                  ;;

          -R) # Restart server
              RESTART_SRV=1
                  ;;

          -S) # Remove log files after running
              CLEANUP=0
                  ;;

          -s) # client array size for RPC's
              VAL_NEEDED=ELEMS
                  ;;

          -T) # directory for temporary files
              VAL_NEEDED=TMPDIR
                  ;;

          -t) # time RPC's
              TIMING=-t
                  ;;

          -u) # percent of clients to be run using udp protocol
              VAL_NEEDED=UDPPCT
                  ;;

          -*) # Huh?
              echo "Bad flag $i"
              rs2_rt_usage
              exit 1
                  ;;

       esac 
          ;;

      *) case $VAL_NEEDED in
       AUTHN_SVC) if [ "$i" != "none" -a "$i" != "none" ]
                   then
                    echo "Bad value ($i) for authorization service"
                    rs2_rt_usage
                    tet_result FAIL
                    exit 1
                   else
                    rs2_rt_authn_svc="-a $i"
                  fi
                  VAL_NEEDED="NONE"
                    ;;

            NCLI) NCLIENTS=$i
                  VAL_NEEDED="NONE"
                    ;;

          BINDIR) # we accept this at face value
                  rs2_rt_exe_path=$i
                  VAL_NEEDED="NONE"
                    ;;

        CLI_MACH) rs2_rt_cli_mach="$rs2_rt_cli_mach $i"
                  VAL_NEEDED="NONE"
                    ;;

       CONFIG_FN) CONFIG=$i
                  VAL_NEEDED="NONE"
                    ;;

         REP_INT) REP_INT=$i
                  VAL_NEEDED="NONE"
                    ;;

      HOURS|MINS) if [ "$rs2_rt_no_bound" = "-p" ] 
                   then
                    echo "Can't specify -h or -m with -p"
                    rs2_rt_usage
                    tet_result FAIL
                    exit 1
                   else
                    [ "$VAL_NEEDED" = "HOURS" ] && HOURS=$i || MINS=$i
                  fi
                  VAL_NEEDED="NONE"
                    ;;

           ELEMS) rs2_rt_nelems=$i
                  VAL_NEEDED="NONE"
                    ;;

   LOGDIR|TMPDIR) # if this does not exist
                  if [ "$VAL_NEEDED" = "TMPDIR" ]
                   then
                    type=temp
                   else
                    type=log
                  fi
                  if [ "$VAL_NEEDED" = "TMPDIR" ]
                   then
                    rs2_rt_tmp=$i
                   else
                    rs2_rt_log_base=$i
                  fi
                  VAL_NEEDED="NONE"
                    ;;

        PROT_LEV) if [ "$i" != "none" -a "$i" != "conn" -a "$i" != "call" -a "$i" != "pkt" -a "$i" != "integ" -a "$i" != "priv" ] 
                   then
                    echo "Bad value for protection level"
                    rs2_rt_usage
                    tet_result FAIL
                    exit 1
                   else
                    rs2_rt_prot_lev=$i
                  fi
                  VAL_NEEDED="NONE"
                    ;;

          PASSES) if [ "$rs2_rt_no_bound" = "-h" -o "$rs2_rt_no_bound" = "-m" ] 
                   then
                    echo "Can't specify -h or -m with -p"
                    rs2_rt_usage
                    tet_result FAIL
                    exit 1
                   else
                    PASSES=$i
                  fi
                  VAL_NEEDED="NONE"
                    ;;

          UDPPCT) UDPPCT=$i
                  VAL_NEEDED="NONE"
                    ;;

               *) # Huh
                  echo "Bad arguement $i"
                  rs2_rt_usage
                  tet_result FAIL
                  exit 1
                  ;;
         esac

           ;;
   esac
  done
 if [ "$VAL_NEEDED" != "NONE" ]
  then
   echo "missing arguement for $VAL_NEEDED option"
   rs2_rt_usage
   tet_result FAIL
   exit 2
 fi

#################################
## get our params set
#################################

#
# path to client executable
#

 rs2_rt_cli_path=$rs2_rt_exe_path/rpc.sec.2_cli

#
# path to server executable
#

 rs2_rt_srv_path=$rs2_rt_exe_path/rpc.sec.2_srv

#
# base for client log names
#

 rs2_rt_cli_log_base=$rs2_rt_log_base/cli_log$$

#
# base for server log name
#

 rs2_rt_srv_log_base=$rs2_rt_log_base/srv_log$$

#
# basename for info on clients we want to start
#

 rs2_rt_cliinfo=$rs2_rt_tmp/rs2_cliinfo$$

#
# basename for info on clients we actually have running
#

 rs2_rt_cliprocs=$rs2_rt_tmp/rs2_cliprocs$$

#
# basename for info on clients ready state
#

 rs2_rt_cli_rdy_stat=$rs2_rt_tmp/rs2_rdystat$$

#
# config file name
#

 rs2_rt_conf_file=$CONFIG

#
# machines to run on
#

 [ "$rs2_rt_cli_mach" = "" ] && rs2_rt_cli_mach=$CLIENTS

#
# number of clients to run
#

 rs2_rt_cli_to_start=$NCLIENTS

#
# percent of clients to run udp
#

 rs2_rt_udppct=$UDPPCT

#
# Calculate the number of total clients per machine
#

 rs2_rt_udppct=`echo "scale=2; $UDPPCT/100; quit" | bc`
 rs2_rt_nudp=`echo "($rs2_rt_cli_to_start * $rs2_rt_udppct)/1; quit" | bc`
 rs2_rt_ntcp=`expr $rs2_rt_cli_to_start - $rs2_rt_nudp`

#
# calculate the number of clients per machine, and the "fudge".  The fudge is
# the remainder of the division of total clients into machines.  An additional
# client is started on each machine until the fudge is gone.
#

 if [ "$rs2_rt_cli_mach" != "$rs2_rt_localhost" ]
  then

  #
  # Running on a more than one system. Do the calculation.
  #
  # get number of machines 
  #

   rs2_rt_nmach=`echo $rs2_rt_cli_mach | awk '{print NF}'`

  #
  # handle the case where there are more machines than clients to be run.
  # We loop through the client machines until we have the same number of
  # machines as clients to run
  #

   if [ $rs2_rt_nmach -gt $rs2_rt_cli_to_start ]
    then
     nm=1
     rs2_rt_cli_mach=""
     for i in $CLIENTS
      do
       rs2_rt_cli_mach="$rs2_rt_cli_mach $i"
       if [ $nm -eq $rs2_rt_cli_to_start ]
        then
         rs2_rt_nmach=$nm
         break
        else
         nm=`expr $nm + 1`
       fi
      done
   fi            

  #
  # get base number of clients per machine
  #

   rs2_rt_cli_p_mach=`echo "$rs2_rt_cli_to_start/$rs2_rt_nmach;quit" | bc`

  #
  # get the fudge
  #

   rs2_rt_cli_fudge=`echo "$rs2_rt_cli_to_start%$rs2_rt_nmach;quit" | bc`
  else

  #
  # for the local machine case, we just prime the pertinent variables
  #

   rs2_rt_cli_p_mach=$rs2_rt_cli_to_start
   rs2_rt_cli_fudge=0
 fi

#
# now say hi
#

rs2_rt_print_hi

#
# check log and tmp directories
#
 
 echo "   Looking for log and tmp directories..."
 for i in $rs2_rt_tmp $rs2_rt_log_base
  do 
   if [ ! -d $i ]
    then

    #
    # try to create it
    #

    echo "   Trying to create dir ($i)..."
    rpcsec2_conf_makepath $i
    if [ "$rpcsec2_conf_mp_out" = "error" ]
     then
      echo "   Couldn't create dir ($i)."
      rs2_rt_usage
      tet_result FAIL
      exit 1
     else
      echo "   Created dir ($i)..."
    fi
   fi
  done

#
# Start the server
#
 
 echo "   Looking for running server..."
 foundit=`ps $rs2_rt_pswideopts | grep rpc.sec.2_srv | \
          grep -v "grep rpc.sec.2_srv" | awk '{print $1}'`
 if [ "$foundit" != "" ]
  then
   if [ $RESTART_SRV -eq 1 ]
    then
     echo "   Found rpc.sec.2_srv running with PID $foundit.  Attempting to kill it..."
     eval "kill -9 $foundit"
     sleep 10
     foundit=`ps $rs2_rt_pswideopts | grep rpc.sec.2_srv | \
              grep -v "grep rpc.sec.2_srv" | awk '{print $1}'`
     if [ "$foundit" != "" ]
      then
       echo "   Could not kill PID $foundit"
      else
       echo "   Pre-existing server killed..."
     fi
    else
     echo "   Testing with existing server (PID $foundit)..."
   fi
 fi
 if [ "$foundit" = "" ]
  then   
   echo "   Starting the rpc.sec.2 test server..."
    $rs2_rt_srv_path -f $rs2_rt_conf_file $DEBUG_SRV -l $rs2_rt_prot_lev > $rs2_rt_srv_log_base 2>& 1 &
    LOOP=1
    while ( [ $LOOP -eq 1 ] )
     do
      sleep 30
      grep "Ready for requests" $rs2_rt_srv_log_base > /dev/null 2>&1
      if [ $? -eq 0 ]
       then
        echo "   Server is ready..."
        rs2_rt_srv_started=1
        LOOP=0
        continue
      fi
     done
 fi

#
# loop through the list of machines, starting clients on each as 
# dictated by the number of clients per machine and the fudge
#

 echo ""
 echo "   Beginning to set up client run information at `date`"
 rs2_rt_cli_infos=""
 for i in $rs2_rt_cli_mach
  do
  
  #
  # set base value for clients for this machine
  #
 
   rs2_rt_cli_this_mach=$rs2_rt_cli_p_mach

  #
  # set the RSH variable
  # 
  #   If we are starting remote clients and the current machine is not the
  #   local host, then RSH is set to be the preamble for a remote client 
  #   startup, else it is null.
  #
  #

   if [ "$i" != "$rs2_rt_localhost" ]
    then
     RSH="$RSH_COMM $i"

     #
     # test rsh access
     #

     good_acc=`$RSH echo true`
     if [ "$good_acc" != "true" ]
      then 
       echo "   ACCESS for machine $i DENIED"
       if [ $IGNORE -eq 1 ]
        then
         continue
        else
         tet_result FAIL
         exit 1
       fi
     fi
    else
     RSH=
   fi

  #
  # get the O/S type
  #

   rs2_rt_os=`$RSH uname`

  #
  # if we have fudge left, take one
  #

   if [ $rs2_rt_cli_fudge -gt 0 ]
    then
     rs2_rt_cli_this_mach=`expr $rs2_rt_cli_this_mach + 1`
     rs2_rt_cli_fudge=`expr $rs2_rt_cli_fudge - 1`
   fi
   
  #
  # initialize the per-machine client counter and seqence counter
  #

   rs2_rt_cli_on_mach=0
   rs2_rt_preloop_seq=$rs2_rt_cli_seq
   
  #
  # now loop until we have the right number of clients started on this
  # machine.
  #

   while( [ $rs2_rt_cli_on_mach -lt $rs2_rt_cli_this_mach ] )
    do

    #
    # figure out what protocol to use
    #
    # question one: what protocols are we running on this platform so far
    #

     rs2_rt_did_udp=`echo $rs2_rt_udp_platfm | grep $rs2_rt_os`
     rs2_rt_did_tcp=`echo $rs2_rt_tcp_platfm | grep $rs2_rt_os`
      
    #
    # second question: do we need tcp and want to run it on this platform
    #

     if [ $rs2_rt_tcp_cli -lt $rs2_rt_ntcp -a "$rs2_rt_did_tcp" = "" ]
      then

      #
      # we will start a tcp client
      #

       protocol=connection

      #
      # add this platform to the list
      #

       rs2_rt_tcp_platfm="$rs2_rt_tcp_platfm $rs2_rt_os"

      #
      # else if we need udp and have none on this platform
      #

       elif [ $rs2_rt_udp_cli -lt $rs2_rt_nudp -a "$rs2_rt_did_udp" = "" ]
        then

        #
        # need udp clients, and none started on this platform so far, 
        # so make the protocol datagram
        #

         protocol=datagram
 
        #
        # add this platform to the list
        #

         rs2_rt_udp_platfm="$rs2_rt_udp_platfm $rs2_rt_os"

        #
        # else we have all protocols running on all platforms w/i the bounds
        # of the number of clients of eah type we were to start, so just 
        # start what ever kind is needed.

         elif [ $rs2_rt_tcp_cli -lt $rs2_rt_ntcp ]
          then
           protocol=connection
          else
           protocol=datagram
     fi
    
    #
    # save the info on the client
    #

     echo "$rs2_rt_cli_seq $i $rs2_rt_os $protocol" >> $rs2_rt_cliinfo
     rs2_rt_cliinfos="$rs2_rt_cliinfos $rs2_rt_cli_seq:$i:$rs2_rt_os:$protocol"
     
    #
    # adjust counts
    #

     [ "$protocol" = "connection" ] && rs2_rt_tcp_cli=`expr $rs2_rt_tcp_cli + 1` || \
                                       rs2_rt_udp_cli=`expr $rs2_rt_udp_cli + 1`
    
     rs2_rt_cli_on_mach=`expr $rs2_rt_cli_on_mach + 1`
     rs2_rt_cli_seq=`expr $rs2_rt_cli_seq + 1`
    done
  done  
 echo "   Finished setting up client run information at `date`..."
 echo ""
 echo "   Beginning client startup at `date`"
 echo "   ---------------------------------------------------------"
 killjobs=0
 currhost=""
 LOOP=1
 for nextrec in $rs2_rt_cliinfos
  do

  #
  # split up the input line
  #

   nextrec=`echo $nextrec | awk -F':' '{print $1, $2, $3, $4}'`
   set $nextrec
   seq=$1
   host=$2
   ostype=$3
   protocol=$4
   [ "$host" != "$rs2_rt_localhost" ] && RSH="$RSH_COMM $host" || RSH=

  #
  # if we are doing things for a new host, do setup tasks
  #

   if [ "$host" != "$currhost" ]
    then
     currhost=$host

     #
     # try to remove the sync file
     #

     $RSH rm -f $rs2_rt_cli_sync_fn
     isgone=`$RSH ls -l $rs2_rt_cli_sync_fn 2>> /dev/null | grep "^-rw"`
      if [ "$isgone" != "" -a $IGNORE -eq 0 ]
       then
        echo "   Could not remove sync file ($rs2_rt_cli_sync_fn) on $host..."
        killjobs=1
        LOOP=0
        continue
      fi

     #
     # look for the config file
     #

     foundit=`$RSH ls -l $rs2_rt_exe_path/$rs2_rt_conf_file 2>> /dev/null| grep "^-rw"`
     if [ "$foundit" = "" -a $IGNORE -eq 0 ]
      then

      #
      # report and quit
      #

       echo "   Can't find config file ($rs2_rt_conf_file) on $host..."
       killjobs=1
       LOOP=0
       continue
     fi
  
     #
     # check to see if we can execute the client program
     #

     cando=`$RSH $rs2_rt_cli_path -X| grep "usage"`
     if [ "$cando" = "" -a $IGNORE -eq 0 ]
      then

      #
      # report and quit
      #

       echo "   Can't run client executable $rs2_rt_cli_path on $host..."
       killjobs=1
       LOOP=0
       continue
     fi
   fi
  
  #
  # at this point, we should be fairly sure that the job will run, so 
  # start her up
  #
  # set the passes or hours and minutes, report and run intervals as 
  # appropriate
  #

   if [ "$rs2_rt_bndry_mode" = "-b" ]
    then
     echo "   STARTing $protocol client $seq on $host ($ostype) running in boundary mode"
     REP_INT=
    else
     [ $PASSES -ne 0 ] && \
       { runstr="$PASSES passes"; repstr=passes; runint="-p $PASSES"; } || \
       { runstr="$HOURS hours and $MINS minutes"; repstr="minutes"; \
         runint="-h $HOURS -m $MINS"; }
     echo "   STARTing $protocol client $seq on $host ($ostype) running for $runstr at `date`,"
     echo "   reporting every $REP_INT $repstr..."
     REP_INT_ARG="-i $REP_INT"
   fi

  #
  # due to problems with rsh on OSF1 1.1.1 we need to special case the
  # startup of OSF1 clients
  #
   if [ "$ostype" = "OSF1" -a "$host" != "$rs2_rt_localhost" ]
    then
     $RSH "sh -c '. /.profile; ulimit -n 256; $rs2_rt_cli_path -s $seq $rs2_rt_bndry_mode -P $protocol -f $rs2_rt_exe_path/$rs2_rt_conf_file -S $rs2_rt_cli_sync_fn $TIMING -l $rs2_rt_prot_lev $DEBUG_CLI $runint -v $rs2_rt_nelems $REP_INT_ARG'" > $rs2_rt_cli_log_base.$seq 2>&1 &
     elif [ "$ostype" = "HP-UX" -a "$host" != "$rs2_rt_localhost" ]
      then
       $RSH '. /.profile; '"$rs2_rt_cli_path"' -s '"$seq"' '"$rs2_rt_bndry_mode"' -P '"$protocol"' -f '"$rs2_rt_exe_path/$rs2_rt_conf_file"' -S '"$rs2_rt_cli_sync_fn"' '"$TIMING"' -l '"$rs2_rt_prot_lev"' '"$DEBUG_CLI"' '"$runint"' -v '"$rs2_rt_nelems"' '"$REP_INT_ARG" > $rs2_rt_cli_log_base.$seq 2>&1 &
      else
       $RSH $rs2_rt_cli_path -s $seq $rs2_rt_bndry_mode -P $protocol -f $rs2_rt_exe_path/$rs2_rt_conf_file -S $rs2_rt_cli_sync_fn $TIMING -l $rs2_rt_prot_lev $DEBUG_CLI $runint -v $rs2_rt_nelems $REP_INT_ARG > $rs2_rt_cli_log_base.$seq 2>&1 &
   fi
  done

#
# get a list of the clients and corresponding sequence numbers 
#
 ps $rs2_rt_pswideopts | grep rpc.sec.2_cli | grep -v "grep rpc.sec.2_cli" |\
    awk '{ \
           if ($6 == "remsh" ) \
            { \
                if ($10 > max) \
                max=$10; \
                if (list[$10] == "") \
                   list[$10]= $1 " " $7 " "  $10 " "  $12;\
            }\
            else \
            {\
               if ($8 > max) \
                  max = $8; \
               if (list[$8] == "") \
                  list[$8]= $1 " " "local" " " $8 " " $12;\
            }\
         }\
         END {\
                for (i=1; i <= max; i++)\
                   print list[i];\
         }' > $rs2_rt_cliprocs

#
# Sleep to let the clients get started
#

 sleep $RPCSEC2_CLI_START_DELAY

#
# Check to see if the clients got started.  We do this by looking at the logs
# for the "READY to make" line.  This should be there if the client is ready,
# since the clients fflush the output after printing the line.  We then
# sort the list and run it through an awk script that looks for missing 
# sequence numbers (which indicate missing
# jobs).  If any are detected, then the awk script prints the list of missing
# sequence numbers.  If no missing sequence numbers are detected then the awk 
# script prints "okay".  The awk script expects a line of the form
# sequence=<numb> in the input file. This tells the awk script what the last 
# sequence number is.
#

 echo "sequence = `expr $rs2_rt_cli_seq - 1`" > $rs2_rt_cli_rdy_stat
 grep "READY to sta" $rs2_rt_cli_log_base.* | sort -n +1 >> $rs2_rt_cli_rdy_stat
 no_cli_seq=`awk -f $rs2_rt_exe_path/rpc.sec.2_chkcli.awk $rs2_rt_cli_rdy_stat`
 echo ""
 if [ "$no_cli_seq" = "okay" ]
  then
   echo "   Client startup verified.  All clients ready to make calls..."
   this_err=0  # never a sequence number 0
  else
   echo "   Client startup failed:"
   if [ $IGNORE -eq 0 ]
    then
     killjobs=1
   fi
   this_err=`echo $no_cli_seq | awk '{print $1}'`
   no_cli_seq=`echo $no_cli_seq | sed -e s/$this_err// -e 's/  / /'`
 fi

#
# tell the user what we are running
#

 index=1
 rng_strt=1
 rs2_rt_cli_this_mach=0
 udp_this_mach=0
 tcp_this_mach=0
 failed=""
 mach=""
 os=""
 echo ""
 echo "   Distribution of clients:"
 echo "   ---------------------------------------------------------"
 while ( [ $index -le $rs2_rt_cli_seq ] )
  do
   if [ $index -ne $rs2_rt_cli_seq ]
    then
     this_rec=`grep "^$index" $rs2_rt_cliinfo`
     set $this_rec
   fi
   if [ "$mach" = "" ]
    then
     mach=$2
     os=$3
     elif [ "$mach" != "$2" -o $index -eq $rs2_rt_cli_seq ]
      then
       echo ""
       echo "   $mach($os) Sequence numbers: $rng_strt-`expr $index - 1"
       echo "      Ready:"
       echo "             Clients: $rs2_rt_cli_this_mach"
       echo "                 UDP: $udp_this_mach"
       echo "                 TCP: $tcp_this_mach"
       echo "      Failed:"
       for i in $failed
        do
         outstr=`echo $i | awk -F':' '{print "seq:",$1, "  protocol:", $2}'`
         echo "           $outstr"
        done
      
       if [ $index -eq $rs2_rt_cli_seq ]
        then
         break
        else
         mach=$2
         os=$3
         udp_this_mach=0
         tcp_this_mach=0
         rng_strt=$index
         rs2_rt_cli_this_mach=0
         failed=""
       fi
   fi
   if [ $1 -eq $this_err ]
    then
     failed="$failed $1:$4"
     this_err=`echo $no_cli_seq | awk '{print $1}'`
     if [ "$this_err" != "" ]
      then
       no_cli_seq=`echo $no_cli_seq | sed -e s/$this_err// -e 's/  / /'`
      else
       this_err=0
     fi
    else
     rs2_rt_cli_this_mach=`expr $rs2_rt_cli_this_mach + 1`
     [ "$4" = "connection" ] && tcp_this_mach=`expr $tcp_this_mach + 1` ||\
                                udp_this_mach=`expr $udp_this_mach + 1`
   fi

   index=`expr $index + 1`
  done

 if [ $killjobs -eq 1 ]
  then
   rs2_rt_killjobs
   if [ $CLEANUP -ne 0 ]
    then
     rs2_rt_cleanup
   fi
   rs2_rt_print_bye
   tet_result FAIL
   exit 2
 fi

#
# create the sync_file 
#

 echo ""
 echo "   Starting sync file creation at `date`..."
 for i in $rs2_rt_cli_mach
  do
   if [ "$i" != "$rs2_rt_localhost" ]
    then
     RSH="$RSH_COMM $i"
    else
     RSH=
   fi
   $RSH touch $rs2_rt_cli_sync_fn >> /dev/null 2>& 1
   didit=`$RSH ls -l $rs2_rt_cli_sync_fn 2>> /dev/null| grep "^-rw"`
   if [ "$didit" = "" ]
    then
     echo "   Could not create $rs2_rt_cli_sync_fn on host $i"
     rs2_rt_killjobs
     rs2_rt_cleanup
     rs2_rt_print_bye
     tet_result FAIL
     exit 1
   fi
  done   
 echo "   Sync file creation complete at `date`"  
  
#
# if we are in boundary mode, we want to wait for the RPC's to happen, the
# time the clients sleep after finding the sync file is always going to be
# <= 2 * the value of the variable RPCSEC2_CLI_SYNC_DELAY.  We then figure the
# time the server will delay, add that in, and then add a 2 second fudge per
# client to this to make sure the RPC's happen.
#

 if [ "$rs2_rt_bndry_mode" = "-b" ]
  then

  #
  # Sleep to let the RPC's happen
  # 

   sleep `echo "($RPCSEC2_CLI_SYNC_DELAY * 2) + (($rs2_rt_cli_seq - 1) * (($rs2_rt_nelems / 3) + 2)) + 15; quit" | bc`

  #
  # now filter the results
  #
   results=`grep "elapsed" $rs2_rt_cli_log_base.* | awk -f $rs2_rt_exe_path/rpc.sec.2_gen_summ.awk`
   if [ "$results" = "No client information lines in input" ]
    then
     echo "   No results for boundary run"
     tet_result FAIL
     exit 1
   fi 
   echo "   Boundary mode run finished at    `date   `"
   echo "   -------------------------------------------------"
   set $results
   echo "   Results for $1 clients ($2 UDP, $3 TCP):"
   echo "      $4 clients SUCCEEDED ($5 UDP, $6 TCP)"

  #
  # do the required shift to get args 7-14 mapped into $1-$7
  #

   nclients=$1
   shift 6
   echo "      $1 clients FAILED ($4 UDP, $7 TCP, $2 too busy, $3 other)"
   echo "          $5 UDP clients got SERVER TOO BUSY"
   echo "          $6 UDP clients got other failures"
   echo "          $8 TCP clients got SERVER TOO BUSY"
   echo "          $9 TCP clients got other failures"

   echo ""
   if [ $1 -gt 0 ]
    then 
     echo "   Failed with $nclients clients"
    else
     echo "   Passes with $nclients clients"
   fi

   rs2_rt_cleanup
 fi

 . $TET_ROOT/lib/xpg3sh/tetapi.sh


