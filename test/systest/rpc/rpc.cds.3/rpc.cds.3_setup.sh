#!/bin/sh
# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: rpc.cds.3_setup.sh,v $
# Revision 1.1.9.2  1996/03/11  02:46:01  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:10  marty]
#
# Revision 1.1.9.1  1995/12/11  22:43:03  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:20:08  root]
# 
# Revision 1.1.6.1  1994/06/10  20:50:42  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:19:48  devsrc]
# 
# Revision 1.1.4.1  1993/10/14  14:27:18  cmckeen
# 	Changed references to rsh to rshsp
# 	[1993/10/14  14:26:59  cmckeen]
# 
# Revision 1.1.2.3  1993/03/12  18:56:28  mhickey
# 	Fixing bsubmit mystery meat
# 	[1993/03/12  18:52:05  mhickey]
# 
# Revision 1.1.1.2  1993/01/29  20:28:29  mhickey
# 	Initial check -in for the rpc.cds.3 system test.
# 
# Revision 1.1.1.2  1992/12/03  15:40:11  mhickey
# 	Initial check submit for rpc.cds.3 system test.
# 
# 	Defect 5887.
# 
# $EndLog$
# 
# rpc.cds.3_setup.sh
# 
# ASSOCIATION
#    This file is part of the rpc.cds.3 DCE System Test.
# 
#    The rpc.cds.3 system test corresponds to test rpc.sec.2 in the DCE1.0.2
#    Core Component System Test plan.  For more information on the operation
#    and goals of the rpc.cds.3 system test, see the test plan, or the README
#    file in this directory.
#
#
# FUNCTION
#    Calls shell functions and scripts to add CDS namespace entries and 
#    security namespace entries to the DCE cell that are necessary for the
#    running of the rpc.cds.3 system test.  The principal names added, cds
#    directories created, keytab file paths, etc. are all set in the file
#    rpc.cds.3.conf.  This file is also read by the client and server programs
#    at startup to get these same parameters.  For more information on what
#    is added, etc., See the comments in the rpc.cds.3_{sec,cds}_{conf,util}.sh
#    shell scripts.
#
# CALLED BY
#    The user.
#
# CALLS INTO
#    rpc.cds.3.conf (definitions)
#    rpc.cds.3.sec_util.sh (rpccds3_sec_cnf_chkvars)
#    rpc.cds.3_sec_conf.sh (security namespace setup)
#    rpc.cds.3.cds_util.sh (rpccds3_cds_cnf_chkvars)
#    rpc.cds.3_cds_conf.sh  (CDS namespace setup)
#
# PREREQUISITES
#    Assumes a DCE cell is running.
#

#
# include utilities and defines
#
. ${RPCCDS3_CONF:-rpc.cds.3.conf}
. rpc.cds.3_sec_util.sh
. rpc.cds.3_cds_util.sh

rc3_su_usage() 
{
      echo "Usage: rpc.cds.3_setup [-B path] [-f path][-l][-R mach][-r mach]"
      echo "  -B: path to binaries on remote machines"
      echo "  -f: path to config file on remote machines"
      echo "  -l: configure for local operation only."
      echo "  -R: create keytab files for servers on mach."
      echo "  -r: create keytab files for cient machines only.  mach"
      echo "      is added to the list of machines on which to create a"
      echo "      keytab file."
}

rc3_su_rem_cli_sys=$RPCCDS3_CLI_MACHINES
rc3_su_rem_srv_sys="$RPCCDS3_SRV_MACHINES"
rc3_su_rem_bindir=$RPCCDS3_BIN_DIRPATH
rc3_su_remconf=$RPCCDS3_BIN_DIRPATH/rpc.cds.3.conf

#
# main
# 
#
# set defaults for variables
   rc3_su_local_only=0     # set up remote machines if any specified
   rc3_su_remote_srv_only=0    # set up remote server machines if any specified
   rc3_su_remote_cli_only=0    # set up remote client machines if any specified
   rc3_su_probsys=""       # remote systems we couldn't setup

   # 
   # Say hi...
   #
   echo ""
   echo "            BEGINNING Setup for System Test rpc.cds3 "
   echo "---------------------------------------------------------------------"
   echo ""
   
   #
   # check for the variables needed by the security and CDS setup routines
   #
   echo "  Checking for variables needed for security setup..."
   rpcsec2_sec_cnf_chkvars
   #
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_cc_out" = "error" ]
   then
      #
      # report and error
      #
      echo "  ERROR: Variables needed for security setup are missing."
      rc3_su_usage
      exit 1
   fi
   echo "  All variables needed for security setup are defined."
   echo ""

   #
   # check for cds variables
   #
   echo "  Checking for variables needed for cds setup..."
   rpcsec2_cds_cnf_chkvars
   #
   # if we failed...
   #
   if [ "$rpcsec2_cds_cc_out" = "error" ]
   then 
      #
      # report and quit
      #
      echo "  ERROR: Variable definitions needed for CDS setup missing."
      rc3_su_usage
      exit 1
   fi
   echo "  All variables necessary for CDS setup are defined."
   echo ""

   # 
   # check for local flag
   #
   rc3_su_rep_rem_srv=0
   rc3_su_rep_rem_cli=0
   VAL_NEEDED="NONE"
   for i in $*
   do
    case $i in 
 
     -*)
        if [ "$VAL_NEEDED" != "NONE" ]
        then
           echo "  Missing value for $VAL_NEEDED"
           rc3_su_usage
           exit 1
        fi
        case $i in
           -B) VAL_NEEDED=BINDIR
               ;;
           -f) VAL_NEEDED=CONFPATH
               ;;
           -l) rc3_su_local_only=1
               echo "  Configuring for local testing only..."
               ;;
           -R) rc3_su_remote_srv_only=1
               if [ $rc3_su_rep_rem_srv -eq 0 ]
               then
                  echo "  Setting up remote (server) machines only..."
                  rc3_su_rep_rem_srv=1
               fi
               VAL_NEEDED=REM_SRV_SYS
               ;;
           -r) rc3_su_remote_cli_only=1
               if [ $rc3_su_rep_rem_cli -eq 0 ]
               then
                  echo "  Setting up remote (client) machines only..."
                  rc3_su_rep_rem_cli=1
               fi
               VAL_NEEDED=REM_SYS
               ;;
           -*) # Huh?
               echo "  Bad flag $i"
               rc3_su_usage
               exit 1
               ;;
        esac
        ;;

     *) case $VAL_NEEDED in 
       REM_SRV_SYS) if [ "$i" != "default" ]
                    then
                       echo "  Adding $i to list of remote server systems..."
                       if [ "$rc3_su_rem_srv_sys" = "$RPCCDS3_SRV_MACHINES" ]
                       then
                          rc3_su_rem_srv_sys=$i
                       else
                          rc3_su_rem_srv_sys="$rc3_su_rem_srv_sys $i"
                       fi
                    else
                       echo "  setting up default remote systems $rc3_su_rem_srv_sys..."
                    fi
                    VAL_NEEDED="NONE"
                    ;;
           REM_SYS) if [ "$i" != "default" ]
                    then
                       echo "  Adding $i to list of remote systems..."
                       if [ "$rc3_su_rem_cli_sys" = "$RPCCDS3_CLI_MACHINES" ]
                       then
                          rc3_su_rem_cli_sys=$i
                       else
                          rc3_su_rem_cli_sys="$rc3_su_rem_cli_sys $i"
                       fi
                    else
                       echo "  setting up default remote systems $rc3_su_rem_cli_sys..."
                    fi
                    VAL_NEEDED="NONE"
                    ;;
           BINDIR) rc3_su_rem_bindir=$i
                   echo "  using $rc3_su_rem_bindir as remote bin dir..."
                   VAL_NEEDED="NONE"
                    ;;
         CONFPATH) rc3_su_remconf=$i
                   echo "  using $rc3_su_remconf as remote config file..."
                   VAL_NEEDED="NONE"
                    ;;
                 *) # Huh
                   echo "  Bad arguement $i"
                   rc3_su_usage
                   exit 1
                   ;;
        esac
        ;;
   esac
 done
 if [ "$VAL_NEEDED" != "NONE" ]
 then
    echo "  missing arguement for $VAL_NEEDED option"
    rc3_su_usage
    exit 2
 fi

 if [ $rc3_su_remote_cli_only -eq 0  -a $rc3_su_remote_srv_only -eq 0 ]
 then
    #
    # do the security setup
    #
    echo "  Setting up security namespace..."
    rpc.cds.3_sec_conf.sh
    #
    # if we failed...
    #
    if [ $? -ne 0 ]
    then
       #
       # report and exit
       # 
       echo "   ERROR: Configuration of security namespace failed."
       exit 1
    fi
    echo "   Security namespace is setup."
    echo ""
  
    #
    # do the CDS setup
    #
    echo "  Setting up CDS namespace..."
    rpc.cds.3_cds_conf.sh
    #
    # if we failed...
    #
    if [ $? -ne 0 ]
    then
       #
       # report and exit
       # 
       echo "   ERROR: Configuration of CDS namespace failed."
       exit 1
    fi
    echo "   CDS namespace is setup."
    echo ""

    #
    # set the remote flags
    #
    if [ $rc3_su_local_only -eq 0 ]
    then
       rc3_su_remote_srv_only=1
       rc3_su_remote_cli_only=1
    fi
 fi   


 #
 # We are all set from this machine.  Now try to create the client keytab
 # on any other machines specified.
 #

 [ $rc3_su_remote_cli_only -eq 0 ] && rc3_su_rem_cli_sys="" 
 [ $rc3_su_remote_srv_only -eq 0 ] && rc3_su_rem_srv_sys="" 

 rc3_su_rem_sys=""
 if [ -n "$rc3_su_rem_cli_sys" -a $rc3_su_local_only -eq 0 -o -n "$rc3_su_rem_srv_sys" -a $rc3_su_local_only -eq 0 ]
 then
    echo "  Setting up remote machines..."
    #
    # for each machine in rc3_su_rem_sys
    #
    rc3_su_local_host=`hostname`

    #
    # if we are doing clients, add them to the list
    #
    if [ -n "$rc3_su_rem_cli_sys" -a $rc3_su_remote_cli_only ]
    then
       rc3_su_rem_sys="$rc3_su_rem_cli_sys"
    fi
    rem_type="client"

    if [ -n "$rc3_su_rem_srv_sys" -a $rc3_su_remote_srv_only ]
    then
       rc3_su_rem_sys="$rc3_su_rem_sys start-srv $rc3_su_rem_srv_sys"
    fi
    rem_type="client"
    for currmach in $rc3_su_rem_sys
    do
       if [ "$currmach" = "start-srv" ]
       then
          rem_type="server"
          continue
       fi
       #
       # try to create the keytab.  This file should be in a public 
       # directory.
       #
       echo "  Setting up $currmach for the rpc.cds.3 $rem_type..."
       if [ "$currmach" != "$rc3_su_local_host" ]
       then
          RSH="rshsp $currmach"
          #
          # test rsh access
          #
          good_acc=`$RSH echo true`
          if [ "$good_acc" != "true" ]
          then 
             echo "  ACCESS for machine $currmach DENIED"
             rc3_su_usage
             exit 1
          fi
       else
          echo "  skipping $currmach..."
          continue
       fi
 
       #
       # get the O/S type
       #
       rc3_su_os=`$RSH uname`

       #
       # make the call
       #
       if [ "$rc3_su_os" = "OSF1" ]
       then
          rshsp $currmach "ulimit -n 256; PATH=$PATH:$rc3_su_rem_bindir; export PATH; RPCCDS3_CONF=$rc3_su_remconf; export RPCCDS3_CONF;. rpc.cds.3_sec_addkey.sh $rem_type" > /tmp/rc3_su.$$ 2>&1
       else
          rshsp $currmach "PATH=$PATH:$rc3_su_rem_bindir; export PATH; RPCCDS3_CONF=$rc3_su_remconf; export RPCCDS3_CONF;. rpc.cds.3_sec_addkey.sh $rem_type" > /tmp/rc3_su.$$ 2>&1
       fi

       #
       # if we failed...
       #
       cat /tmp/rc3_su.$$
       rc3_su_remstat=`grep "Added keys" /tmp/rc3_su.$$ > /dev/null 2>&1`
       if [ $? -ne 0 ]
       then
          #
          # report and exit 
          #
          echo "  ERROR: Can't create $rem_type keytab on $currmach"
          echo "  Error(s) was/were:"
          cat /tmp/rc3_su.$$

          #
          # add to list of problem systems
          #
          rc3_su_probsys="$rc3_su_probsys $rem_type:$currmach"
       else
          #
          # did the copy, report success
          #            
          echo "  Created keytab for $rem_type on $currmach..."
       fi
       rm -f /tmp/rc3_su.$$
       echo ""
    done
    echo "  Remote setup complete."
    #
    # if we had any problems setting up the remote systems...
    #
    if [ -n "$rc3_su_probsys" ]
    then
       #
       # Reiterate the news about systems that we could not setup
       #
       echo "  Note that the following machines COULD NOT BE SETUP FOR"
       echo "  RUNNING THE TEST CLIENT OR SERVER:"
       echo "      $rc3_su_probsys"
    fi
 fi   #### END of if [ -n "$RPCCDS3_CLI_MACHINES...]  ###


   #
   # Say bye
   #
   echo ""
   echo "            COMPLETED Setup for System Test rpc.cds.3 "
   echo "---------------------------------------------------------------------"
   echo ""

### END of main ###

