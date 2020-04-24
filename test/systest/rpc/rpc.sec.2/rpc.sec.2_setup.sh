#!/bin/ksh
# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: rpc.sec.2_setup.sh,v $
# Revision 1.1.75.2  1996/03/11  02:46:57  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:32  marty]
#
# Revision 1.1.75.1  1995/12/11  22:44:10  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1994/08/04  15:27 UTC  bissen
# 	merge from HPDCE01
# 
# 	HP revision /main/HPDCE01/1  1994/01/27  21:50  bissen
# 	merge kk and hpdce01
# 	[1995/12/11  22:20:48  root]
# 
# Revision 1.1.73.3  1993/10/18  13:40:18  cmckeen
# 	Fixed minor syntax problems.
# 
# 	Set rsh_type to either rsh or remsh depending on OS type.
# 	[1993/10/18  13:34:17  cmckeen]
# 
# Revision 1.1.73.2  1993/09/16  14:58:46  cmckeen
# 	Changed FAILURE to FAIL to match the tet_result code
# 	[1993/09/16  14:55:53  cmckeen]
# 
# Revision 1.1.73.1  1993/09/09  19:13:04  cmckeen
# 	 HP's TET'ized version
# 	[1993/09/09  19:12:01  cmckeen]
# 
# Revision 1.1.6.2  1993/08/16  14:42:08  eheller
# 	File tetified.
# 	[1993/07/23  17:47:41  eheller]
# 
# Revision 1.1.1.5  1993/05/04  22:22:20  mgm
# 	    Loading drop DCE1_0_2FNL
# 
# Revision 1.1.2.6  1993/02/25  18:30:55  mhickey
# 	Fixed usage message, added setup for servers, fixed logic so
# 	only servers or clients get setup, as specified.
# 	[1993/02/25  18:28:14  mhickey]
# 
# Revision 1.1.4.2  1993/02/04  22:13:56  cjd
# 	Embedded copyright notice
# 
# Revision 1.1.2.5  1993/02/05  15:23:12  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:44:33  cjd]
# 
# Revision 1.1.2.4  1993/01/14  20:09:38  mhickey
# 	Fixed incorrect remote bin path variable name,
# 
# 	Added read of /.profile to remote calls to
# 	non-OSF systems for nls info, and
# 
# 	Added print of output and correct check of
# 	status from the remote key creation.
# 	[1993/01/14  19:30:21  mhickey]
# 
# Revision 1.1.2.3  1993/01/13  19:46:53  mhickey
# 	Added options for specifying the remote machines to
# 	setup and that only remote client keytab files are
# 	to be created.
# 
# 	Added options for specifying the remote temp, log
# 	and bin dirs.
# 
# 	Changed remote setup from rcp of the client keytab to
# 	a rgy_edit session to create the client keytab on
# 	the remote machine.
# 	[1993/01/13  19:46:36  mhickey]
# 
# Revision 1.1.2.2  1992/12/03  19:53:21  mhickey
# 	Initial check submit for rpc.sec.2 system test.
# 
# 	Defect 5887.
# 	[1992/12/03  15:40:11  mhickey]
# 
# $EndLog$
# 
# rpc.sec.2_setup.sh
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
#    Calls shell functions and scripts to add CDS namespace entries and 
#    security namespace entries to the DCE cell that are necessary for the
#    running of the rpc.sec.2 system test.  The principal names added, cds
#    directories created, keytab file paths, etc. are all set in the file
#    rpc.sec.2.conf.  This file is also read by the client and server programs
#    at startup to get these same parameters.  For more information on what
#    is added, etc., See the comments in the rpc.sec.2_{sec,cds}_{conf,util}.sh
#    shell scripts.
#
# CALLED BY
#    The user.
#
# CALLS INTO
#    rpc.sec.2.conf (definitions)
#    rpc.sec.2.sec_util.sh (rpcsec2_sec_cnf_chkvars)
#    rpc.sec.2_sec_conf.sh (security namespace setup)
#    rpc.sec.2.cds_util.sh (rpcsec2_cds_cnf_chkvars)
#    rpc.sec.2_cds_conf.sh (CDS namespace setup)
#
# PREREQUISITES
#    Assumes a DCE cell is running.
#

#
# include utilities and defines
#

 . $TET_ROOT/lib/xpg3sh/tetapi.sh
 . rpc.sec.2.conf
 . rpc.sec.2_sec_util.sh
 . rpc.sec.2_cds_util.sh

 tet_setcontext

 rs2_su_usage() 
 {
  echo "Usage: rpc.sec.2_setup [-B path] [-f path][-l][-R [mach]][-r [mach]]"
  echo "  -B: path to binaries on remote machines"
  echo "  -f: path to config file on remote machines"
  echo "  -l: configure for local operation only."
  echo "  -R: create keytab files for servers on mach."
  echo "  -r: create keytab files for cient machines only."
  echo " NOTE: for the -r and -R options,  mach"
  echo "       is added to the list of machines on which to create a"
  echo "       keytab file."
 }

 rs2_su_rem_cli_sys=$RPCSEC2_CLI_MACHINES
 rs2_su_rem_srv_sys=$RPCSEC2_SRV_MACHINES
 rs2_su_rem_bindir=$RPCSEC2_BIN_DIRPATH
 rs2_su_remconf=$RPCSEC2_BIN_DIRPATH/rpc.sec.2.conf

#
# main
# 
#
# set defaults for variables

 rs2_su_local_only=0         # set up remote machines if any specified
 rs2_su_remote_srv_only=0    # set up remote server machines if any specified
 rs2_su_remote_cli_only=0    # set up remote client machines if any specified
 rs2_su_probsys=""           # remote systems we couldn't setup

# 
# Say hi...
#

 echo ""
 echo "            BEGINNING Setup for System Test rpc.sec.2 "
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
   rs2_su_usage
   tet_result FAIL
   return 1
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
   rs2_su_usage
   tet_result FAIL
   return 1
 fi
 echo "  All variables necessary for CDS setup are defined."
 echo ""

# 
# check for local flag
#

 rs2_su_rep_rem_srv=0
 rs2_su_rep_rem_cli=0
 VAL_NEEDED="NONE"
 for i in $*
  do
   case $i in 
 
     -*)
        if [ "$VAL_NEEDED" != "NONE" ]
         then
          echo "  Missing value for $VAL_NEEDED"
          rs2_su_usage
          tet_result FAIL
          return 1
        fi

        case $i in
           -B) VAL_NEEDED=BINDIR
               ;;

           -f) VAL_NEEDED=CONFPATH
               ;;

           -l) rs2_su_local_only=1
               echo "  Configuring for local testing only..."
               ;;

           -R) rs2_su_remote_srv_only=1
               if [ $rs2_su_rep_rem_srv -eq 0 ]
                then
                 echo "  Setting up remote (server) machines only..."
                 rs2_su_rep_rem_srv=1
               fi
               VAL_NEEDED=REM_SRV_SYS
               ;;

           -r) rs2_su_remote_cli_only=1
               if [ $rs2_su_rep_rem_cli -eq 0 ]
                then
                 echo "  Setting up remote (client) machines only..."
                 rs2_su_rep_rem_cli=1
               fi
               VAL_NEEDED=REM_SYS
               ;;

           -*) # Huh?
               echo "  Bad flag $i"
               rs2_su_usage
               tet_result FAIL
               return 1
               ;;

        esac
        ;;

     *) case $VAL_NEEDED in 
       REM_SRV_SYS) if [ "$i" != "default" ]
                    then
                       echo "  Adding $i to list of remote server systems..."
                       if [ "$rs2_su_rem_srv_sys" = "$RPCSEC2_SRV_MACHINES" ]
                       then
                          rs2_su_rem_srv_sys=$i
                       else
                          rs2_su_rem_srv_sys="$rs2_su_rem_srv_sys $i"
                       fi
                    else
                       echo "  setting up default remote systems $rs2_su_rem_srv_sys..."
                    fi
                    VAL_NEEDED="NONE"
                    ;;
           REM_SYS) if [ "$i" != "default" ]
                    then
                       echo "  Adding $i to list of remote systems..."
                       if [ "$rs2_su_rem_cli_sys" = "$RPCSEC2_CLI_MACHINES" ]
                       then
                          rs2_su_rem_cli_sys=$i
                       else
                          rs2_su_rem_cli_sys="$rs2_su_rem_cli_sys $i"
                       fi
                    else
                       echo "  setting up default remote systems $rs2_su_rem_cli_sys..."
                    fi
                    VAL_NEEDED="NONE"
                    ;;
           BINDIR) rs2_su_rem_bindir=$i
                   echo "  using $rs2_su_rem_bindir as remote bin dir..."
                   VAL_NEEDED="NONE"
                    ;;
         CONFPATH) rs2_su_remconf=$i
                   echo "  using $rs2_su_remconf as remote config file..."
                   VAL_NEEDED="NONE"
                    ;;
                 *) # Huh
                   echo "  Bad arguement $i"
                   rs2_su_usage
                   tet_result FAIL
                   return 1
                   ;;
        esac
        ;;
   esac
  done
 if [ "$VAL_NEEDED" != "NONE" ]
  then
   echo "  missing arguement for $VAL_NEEDED option"
   rs2_su_usage
   tet_result FAIL
   return 2
 fi

 if [ $rs2_su_remote_cli_only -eq 0  -a $rs2_su_remote_srv_only -eq 0 ]
  then
  
  #
  # do the security setup
  #

   echo "  Setting up security namespace..."
   rpc.sec.2_sec_conf.sh

  #
  # if we failed...
  #

   if [ $? -ne 0 ]
    then

    #
    # report and return
    # 
     echo "   ERROR: Configuration of security namespace failed."
     tet_result FAIL
     return 1
   fi
   echo "   Security namespace is setup."
   echo ""
  
  #
  # do the CDS setup
  #

   echo "  Setting up CDS namespace..."
   rpc.sec.2_cds_conf.sh

  #
  # if we failed...
  #

   if [ $? -ne 0 ]
    then

    #
    # report and return
    # 

     echo "   ERROR: Configuration of CDS namespace failed."
     tet_result FAIL
     return 1
   fi
   echo "   CDS namespace is setup."
   echo ""

  #
  # set the remote flags
  #

   if [ $rs2_su_local_only -eq 0 ]
    then
     rs2_su_remote_srv_only=1
     rs2_su_remote_cli_only=1
   fi
 fi   

#
# We are all set from this machine.  Now try to create the client keytab
# on any other machines specified.
#

 [ $rs2_su_remote_cli_only -eq 0 ] && rs2_su_rem_cli_sys="" 
 [ $rs2_su_remote_srv_only -eq 0 ] && rs2_su_rem_srv_sys="" 

 rs2_su_rem_sys=""
 if [ -n "$rs2_su_rem_cli_sys" -a $rs2_su_local_only -eq 0 -o -n "$rs2_su_rem_srv_sys" -a $rs2_su_local_only -eq 0 ]
  then
   echo "  Setting up remote machines..."

   #
   # for each machine in rs2_su_rem_*sys
   #

    rs2_su_local_host=`hostname`

   #
   # if we are doing clients, add them to the list
   #

    if [ -n "$rs2_su_rem_cli_sys" -a $rs2_su_remote_cli_only ]
     then
      rs2_su_rem_sys="$rs2_su_rem_cli_sys"
    fi

    if [ -n "$rs2_su_rem_srv_sys" -a $rs2_su_remote_srv_only ]
     then
      rs2_su_rem_sys="$rs2_su_rem_sys start-srv $rs2_su_rem_srv_sys"
    fi
    rem_type="client"

    for currmach in $rs2_su_rem_sys
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

      get_os_type=`uname`
      if [ "$get_os_type" = "HP-UX" ]
       then
        rsh_type="remsh"
       else
        rsh_type="rsh"
      fi
      echo "  Setting up $currmach for the rpc.sec.2 $rem_type..."
      if [ "$currmach" != "$rs2_su_local_host" ]
       then
        RSH="$rsh_type $currmach"

        #
        # test rsh access
        #

        good_acc=`$RSH echo true`
        if [ "$good_acc" != "true" ]
         then 
          echo "  ACCESS for machine $currmach DENIED"
          rs2_su_usage
          tet_result FAIL
          return 1
        fi
       else
        echo "  skipping $currmach..."
        continue
      fi
 
     #
     # get the O/S type
     #

      rs2_su_os=`$RSH uname`

     #
     # make the call
     #

      if [ "$rs2_su_os" = "OSF1" ]
       then
        $rsh_type $currmach "TET_ROOT=$TET_ROOT;export TET_ROOT;PATH=\$PATH:$rs2_su_rem_bindir; export PATH; RPCSEC2_CONF=$rs2_su_remconf; export RPCSEC2_CONF;. $rs2_su_rem_bindir/rpc.sec.2_sec_addkey.sh $rem_type" > /tmp/rs2_su.$$ 2>&1
      fi
      if [ "$rs2_su_os" = "HP-UX" ]
       then
        $rsh_type $currmach "TET_ROOT=$TET_ROOT;export TET_ROOT;"'PATH=$PATH:'"$rs2_su_rem_bindir"'; export PATH; RPCSEC2_CONF='"$rs2_su_remconf"'; export RPCSEC2_CONF; . '"$rs2_su_rem_bindir"'/rpc.sec.2_sec_addkey.sh '"$rem_type" > /tmp/rs2_su.$$ 2>&1
      fi
      if [ "$rs2_su_os" = "AIX" ]
       then
        $rsh_type $currmach "TET_ROOT=$TET_ROOT;export TET_ROOT;"'PATH='"$PATH"':'"$rs2_su_rem_bindir"'; export PATH; RPCSEC2_CONF='"$rs2_su_remconf"'; export RPCSEC2_CONF; . '"$rs2_su_rem_bindir"'/rpc.sec.2_sec_addkey.sh '"$rem_type" > /tmp/rs2_su.$$ 2>&1
      fi

     #
     # if we failed...
     #

      cat /tmp/rs2_su.$$
      rs2_su_remstat=`grep "Added keys" /tmp/rs2_su.$$ > /dev/null 2>&1`
      if [ $? -ne 0 ]
       then

       #
       # report and return 
       #

        echo "  ERROR: Can't create $rem_type keytab on $currmach"
        echo "  Error(s) was/were:"
        cat /tmp/rs2_su.$$

       #
       # add to list of problem systems
       #

        rs2_su_probsys="$rs2_su_probsys $rem_type:$currmach"
       else
     
       #
       # did the copy, report success
       #

        echo "  Created keytab for $rem_type on $currmach..."
      fi
      rm -f /tmp/rs2_su.$$
      echo ""
     done
    echo "  Remote setup complete."

    #
    # if we had any problems setting up the remote systems...
    #

    if [ -n "$rs2_su_probsys" ]
     then

     #
     # Reiterate the news about systems that we could not setup
     #

      echo "  Note that the following machines COULD NOT BE SETUP FOR"
      echo "  RUNNING THE TEST CLIENT OR SERVER:"
      echo "      $rs2_su_probsys"
    fi
 fi   #### END of if [ -n "$RPCSEC2_CLI_MACHINES...]  ###

#
# Say bye
#

 echo ""
 echo "            COMPLETED Setup for System Test rpc.sec.2 "
 echo "---------------------------------------------------------------------"
 echo ""

### END of main ###
