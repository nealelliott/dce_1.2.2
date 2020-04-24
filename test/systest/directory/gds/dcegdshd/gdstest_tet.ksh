#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: gdstest_tet.ksh,v $
# Revision 1.1.2.2  1996/03/09  20:50:47  marty
# 	Add OSF copyright
# 	[1996/03/09  20:28:48  marty]
#
# Revision 1.1.2.1  1995/12/11  21:57:49  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:57:30  root]
# 
# $EndLog$
#
# This is a TET shell test case which executes the GDS system tests.
# This calls the test dcegdshd.

tet_startup="startup"
tet_cleanup="clean_up"

iclist="ic1"
ic1="tp1"
tet_testlist="tp1"

#--------------------------------------------------------------------------
# remove_object().  Just to make sure remove all object entries in the DIB. 
#--------------------------------------------------------------------------
remove_local_remote_object()
{

   cat <<EOF > /tmp/gds_temp 
:directory id:2
:password:
:Country:
:Organization:
:Organizational unit:
:common name:
:options:Logon to the Default DSA
:**** Object Administration ****:
:Function:1
:**** Remove Object ****:
:Operation:2
:Object Type Number:5
:country:us
:organization:osf
:Organizational Unit:dce
:Common-Name:Jimbo
:**** Return ****:
:operation:00
:**** Administration ****:
:Function:0
:*** EOF ***:
EOF

   for REMOTEHOST in $CLIENT_LIST
   do
      rcp /tmp/gds_temp $REMOTEHOST:/tmp/gds_temp
      remsh $REMOTEHOST "export NLSPATH=/opt/dcelocal/nls/msg/en_US.ASCII/%N.cat;$DCELOCAL/bin/gdsditadm < /tmp/gds_temp > /dev/null"
      remsh $REMOTEHOST "rm -f /tmp/gds_temp"
   done

   $DCELOCAL/bin/gdsditadm < /tmp/gds_temp > /dev/null
   rm -f /tmp/gds_temp
}

#-------------------------------------------------------------------------
# remove_entire_gds_config().  This routine check whether GDS 
# processes running or not.  If running, deactivate them then
# delete directory id 2 and modify directory id 1
#-------------------------------------------------------------------------
remove_entire_gds_config()
{
   typeset EXPORT="export NLSPATH=/opt/dcelocal/nls/msg/en_US.ASCII/%N.cat"
   typeset CONF_FILE=$DCELOCAL/etc/gdsconfig
   typeset OLD_DIR=`pwd`

   # on local system

   cd $DCELOCAL/bin
   ./gdsdirinfo > /tmp/gds.local 2>&1
   if [ `grep -c errno /tmp/gds.local` -eq 0 ]; then
      # gds processes is running, deactivate them
          ./gdssysadm -fd -p > /dev/null
   fi
   if [ -r $CONF_FILE ]; then 
      if [ `grep -c "DIR-ID: 2" $CONF_FILE` -eq 1 ]; then
         # Directory id 2 is exist.  Go ahead and remove them
         ./gdssysadm -fc -d2 -m2 > /dev/null
      fi
      if [ `grep -c "DIR-ID: 1" $CONF_FILE` -eq 1 ]; then
         # Directory id 1 is exist.  Go ahead and change them
         ./gdssysadm -fc -d1 -m4 -c1 -C16 > /dev/null
      fi
   fi
   rm -f /tmp/gds.local
   myecho "Remove entire gds config on <$LOCALHOST>... done"

   #On remote systems

   for REMOTEHOST in $CLIENT_LIST; do
      remsh $REMOTEHOST "$EXPORT; cd $DCELOCAL/bin; ./gdsdirinfo" \
                                          > /tmp/gds.remote 2>&1
      if [ `grep -c errno /tmp/gds.remote` -eq 0 ]; then
         # gds processes is running, deactivate them
         remsh $REMOTEHOST "$EXPORT; cd $DCELOCAL/bin; ./gdssysadm -fd -p > /dev/null"
      fi

      remsh $REMOTEHOST "cat $CONF_FILE" > /tmp/gds.config 2>&1

      if [ `grep -c "DIR-ID: 2" /tmp/gds.config` -eq 1 ]; then
         # Directory id 2 is exist.  Go ahead and remove them
         remsh $REMOTEHOST "$EXPORT; cd $DCELOCAL/bin; ./gdssysadm -fc -d2 -m2 > /dev/null" 
      fi
      if [ `grep -c "DIR-ID: 1" /tmp/gds.config` -eq 1 ]; then
         # Directory id 1 is exist.  Go ahead and change them
         remsh $REMOTEHOST "$EXPORT; cd $DCELOCAL/bin; ./gdssysadm -fc -d1 -m4 -c1 -C16 > /dev/null"
      fi
  
      rm -f /tmp/gds.remote /tmp/gds.config
      myecho "Remove entire gds config on <$REMOTEHOST>... done"
   done
   cd $OLD_DIR
}

#------------------------------------------------------------------
# clean_up().  Clean all previous system test files left over 
#------------------------------------------------------------------
clean_up()
{
   myecho "Performing GDS Test clean-up"
   cd /
   myecho "Clean up previous system test object data and files ... "
   remove_local_remote_object

   #Remove entire GDS configuration if REMOVE_GDS_CONFIG="True"
   REMOVE_GDS_CONFIG=${REMOVE_GDS_CONFIG:-"True"}
   if [[ $REMOVE_GDS_CONFIG = "True" ]]; then
      remove_entire_gds_config
   fi

   if [ "`ls $STTMPDIR/dcegdshd.* | grep -c dcegdshd`" -ne 0 ]; then
      rm -rf $STTMPDIR/dcegdshd.*
   fi
   #clean up gds.temp created during check GDS of local and remote node
   if [ -r /tmp/gds.temp ]; then
      rm -f /tmp/gds.temp
   fi
   #Delete log files if flag is set to False
   SAVE_LOG_FILES=${SAVE_LOG_FILES:-"True"}
   if [[ $SAVE_LOG_FILES != "True" ]]; then
      rm -rf $LOG_DIR
   fi
   myecho "done cleaning up."
}

#-------------------------------------------------------------------------
# run_gds_setup().  Run GDS setup tool.
#-------------------------------------------------------------------------
run_gds_setup()
{
   if test -r ./worldSetup.gds 
   then
       tet_setblock
      ./worldSetup.gds
      if [[ $? != 0 ]]; then
        return 1
      else
        return 0
      fi
   else
      myecho "-------------------------------------------------------------"
      myecho " File ./worldSetup.gds does not exist."
      myecho " Can not setup GDS on both local and remote system."
      myecho "-------------------------------------------------------------"
      return 1
   fi
}

#-------------------------------------------------------------------------
# check_gds_setup().  Check to make sure GDS setup correctly
# This function expect 2 parameters: 
#    1. gdsconfig file
#    2. hostname
#-------------------------------------------------------------------------
check_gds_setup()
{
   # parameter 1 contains GDS configuration file name
   conf_file=${1}
   if [ "`grep \"DIR-ID: 1 \" $conf_file`" != "$DIR1" ]; then
      myecho "Wrong GDS system test setup on ${2}"
      myecho "Run GDS system test setup NOW."
      SETUP_FLAG=1
   else
      if [ "`grep \"DIR-ID: 2 \" $conf_file`" != "$DIR2" ]; then
         myecho "Wrong GDS system test setup on ${2}"
         myecho "Run GDS system test setup NOW."
         SETUP_FLAG=1
      else
         myecho "GDS setup on ${2} is OK."
      fi
   fi
}

#-------------------------------------------------------------------------
# check_gds_local_remote_setup().  Check local and remote configuration
#-------------------------------------------------------------------------
check_gds_local_remote_setup()
{
#set -x
   # on local system
   if [ -r $DCELOCAL/etc/gdsconfig ]; then
      cp $DCELOCAL/etc/gdsconfig /tmp/gds.temp1
      check_gds_setup /tmp/gds.temp1 $LOCALHOST
   else
      myecho "File $DCELOCAL/etc/gdsconfig on $LOCALHOST does not exist."
      myecho "Run GDS system test setup NOW."
      SETUP_FLAG=1
   fi 
   rm -f /tmp/gds.temp1
   
   # on remote system
   for REMOTEHOST in $CLIENT_LIST
   do
      remsh $REMOTEHOST "cat ${DCELOCAL}/etc/gdsconfig" > /tmp/gds.temp2 2>&1
      found=`grep -c "No such file" /tmp/gds.temp2`
      if [ $found -eq 1 ]; then
         myecho "File $DCELOCAL/etc/gdsconfig on $REMOTEHOST does not exist."
         myecho "Run GDS system test setup NOW."
         SETUP_FLAG=1
      else
         check_gds_setup /tmp/gds.temp2 $REMOTEHOST
      fi
      rm -f /tmp/gds.temp2
   done
}

#-------------------------------------------------------------------
# check_gds_local() check whether local GDS processes up or down.
# If down, start it up.
#-------------------------------------------------------------------
check_gds_local()
{
   # Local system
   $DCELOCAL/bin/gdsdirinfo > /tmp/gds.temp 2>&1
   match_local=`grep -c errno /tmp/gds.temp`
   rm -f /tmp/gds.temp
   if [ $match_local -eq 1 ]; then
      myecho "Current GDS processes on <$LOCALHOST> are down"
      myecho "Startup GDS on <$LOCALHOST> ... "
      cd $DCELOCAL/bin; gdssysadm -fA -p
      myecho "OK"
   else
      myecho "GDS on <$LOCALHOST> is OK"
   fi
}
#-------------------------------------------------------------------
# check_gds_remote() check whether remote GDS processes up or down.
# If down, start it up.
#-------------------------------------------------------------------
check_gds_remote()
{
   # Remote system
   for REMOTEHOST in $CLIENT_LIST
   do
      remsh $REMOTEHOST "export NLSPATH=/opt/dcelocal/nls/msg/en_US.ASCII/%N.cat;/opt/dcelocal/bin/gdsdirinfo" > /tmp/gds.temp 2>&1
      match_remote=`grep -c errno /tmp/gds.temp`
      if [ $match_remote -eq 1 ]; then
         myecho "Current GDS processes on <$REMOTEHOST> are down."
         myecho "Startup GDS on <$REMOTEHOST> ... "
         remsh $REMOTEHOST "export NLSPATH=/opt/dcelocal/nls/msg/en_US.ASCII/%N.cat;cd /opt/dcelocal/bin; gdssysadm -fA -p"
         myecho "OK"
      else
         myecho "GDS on <$REMOTEHOST> is OK"
      fi
   done
}

#-------------------------------------------------------------------
# startup() Main tet startup routine for the gds tests.
#-------------------------------------------------------------------
startup()
{
   myecho "Performing GDS test setup"

   #
   # build list of test machines
   #
   CELL_NAME=`/opt/dcelocal/bin/getcellname | awk -F/ '{ print $3}'`

   if [[ ! -f $TET_ROOT/systest/client_names_$CELL_NAME ]]; then
      myecho "File $TET_ROOT/systest/client_names_$CELL_NAME not found"
      myecho "This is a bad thing... GDS Test setup failed"
      for test_purp in $tet_testlist; do
         tet_delete $test_purp "setup() has failed"
      done
   else
      LOCALHOST=`hostname`
      MACHINES=`cat $TET_ROOT/systest/client_names_$CELL_NAME`
      integer c=1
      CLIENT_LIST=""
      for mach in $MACHINES; do
         if [[ "$mach" != "$LOCALHOST" ]] && (( c < 11 )); then
            CLIENT_LIST="$CLIENT_LIST $mach"
            ((c = c + 1))
         fi
      done
   fi
   myecho "Using machine $LOCALHOST as Initial DSA"
   myecho "Using machines $CLIENT_LIST as Other DSAs"
   export LOCALHOST CLIENT_LIST

   #
   # Variable setup
   #
   DIR1="DIR-ID: 1 CONF-TYPE: Clt/Srv-System SERVER-NO: 1 UPDATE: * DB-TYPE: C-isam CLIENTS: 16"
   DIR2="DIR-ID: 2 CONF-TYPE: Clt/Srv-System SERVER-NO: 1 UPDATE: * DB-TYPE: C-isam CLIENTS: 16"
   SETUP_FLAG=0
   TIME=5    #TIME can be 5, 10, 15, 30 minutes

   #Remove entire GDS configuration if REMOVE_GDS_CONFIG="True"
   REMOVE_GDS_CONFIG=${REMOVE_GDS_CONFIG:-"True"}
   if [[ $REMOVE_GDS_CONFIG = "True" ]]; then
      remove_entire_gds_config
   fi

   # Check for valid GDS Setup
   check_gds_local_remote_setup

   if [ $SETUP_FLAG -eq 1 ]
   then
      myecho "run_gds_setup in progress..."
      run_gds_setup
      if [[ $? != 0 ]] ; then
        myecho "GDS Test setup failed"
        for test_purp in $tet_testlist
        do
            tet_delete $test_purp "startup() has failed."
        done
      fi
  else
      # Make sure GDS processes are running   
      check_gds_local
      check_gds_remote
   fi
}


#-------------------------------------------------------------------
# tp1() Main TET test purpose for the gds tests.
#-------------------------------------------------------------------
tp1()
{
#
# Start run GDS system test
#
   myecho "Starting GDS test"

   DSA=/us/osf/dce/dsa/

   for n in $CLIENT_LIST; do
      SLAVE_LIST="$SLAVE_LIST -s$DSA$n"
   done

   tet_setblock
   ./dcegdshd -d${DSA}${LOCALHOST} ${SLAVE_LIST} -u${TIME}
   if [[ $? != 0 ]] 
   then
     tet_result FAIL
   else
     tet_result PASS
   fi

   myecho "------------------------------------------------------------------"
   myecho " TYPE: "
   myecho " more $LOG_DIR/dcegdshd/pass_fail_log"
   myecho " for more information about this system test."
   myecho "----------------------------------------------------------------------"
   myecho "GDS Test complete"
}


myecho()
{
	print $1
	tet_infoline $1
}

WS=`basename $0` ; export WS

. $TET_ROOT/profile.dcest
. ./gds.data
. ./gds.functions
. $TET_ROOT/lib/ksh/tcm.ksh





