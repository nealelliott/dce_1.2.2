#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: rpc.sec.2_sec_conf.sh,v $
# Revision 1.1.73.2  1996/03/11  02:46:51  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:30  marty]
#
# Revision 1.1.73.1  1995/12/11  22:44:06  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:20:46  root]
# 
# Revision 1.1.71.1  1993/09/09  19:13:01  cmckeen
# 	 HP's TET'ized version
# 	[1993/09/09  19:11:58  cmckeen]
# 
# Revision 1.1.6.2  1993/08/16  14:39:45  eheller
# 	File tetified.
# 	[1993/07/23  17:46:35  eheller]
# 
# Revision 1.1.1.5  1993/05/04  22:22:18  mgm
# 	    Loading drop DCE1_0_2FNL
# 
# Revision 1.1.2.5  1993/02/25  20:16:35  mhickey
# 	Added support for installing server keytab files on remote
# 	machines.  This means not randomizing the server key so that
# 	the sec_addkey.sh program can know what to use for a
# 	passwd.
# 	[1993/02/25  18:57:30  mhickey]
# 
# Revision 1.1.2.4  1993/02/05  15:23:05  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:44:22  cjd]
# 
# Revision 1.1.4.2  1993/02/04  22:13:41  cjd
# 	Embedded copyright notice
# 
# Revision 1.1.2.3  1993/01/13  19:25:28  mhickey
# 	Modified call to rpcsec2_sec_add_key to reflect change in
# 	parameters.
# 
# 	Added preference for environment variable definition of
# 	config file name.
# 	[1993/01/13  19:25:07  mhickey]
# 
# Revision 1.1.2.2  1992/12/03  19:53:08  mhickey
# 	Initial check submit for rpc.sec.2 system test.
# 
# 	Defect 5887.
# 	[1992/12/03  15:40:00  mhickey]
# 
# $EndLog$
# 
# rpc.sec.2_sec_conf.sh
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
#    Adds necessary principals, groups, and accounts to the security 
#    namespace.  Creates a server keytab file, and with user intervention,
#    adds an account for the server principal to the password file on the 
#    local machine and chowns the keytab file to be owned by the server.
#
# CALLED BY
#    Called by rpc.sec.2_setup.sh via while setting up the DCE cell for 
#    running the rpc.sec.2 system test.
#
# CALLS INTO
#    rpc.sec.2.conf (definitions)
#    rpc.sec.2_conf_util.sh (rpcsec2_conf_check_auth)
#    rpc.sec.2_sec_util.sh  (rpcsec2_sec_add_*).
#
# PREREQUISITES
#   Assumes a DCE cell with security service running.
#

#
# include utilities and defines
#
. rpc.sec.2.conf
. rpc.sec.2_conf_util.sh
. rpc.sec.2_sec_util.sh


#
# main
# 
#
   echo ""
   echo "   BEGINNING Security Namespace Setup(rpc.sec.2_sec_conf.sh) @ `date`"
   echo "---------------------------------------------------------------------"
   echo ""
   echo "     Verifying user identity..."
   rpcsec2_conf_check_auth cell_admin
   #
   # if user is not who s/he needs to be...
   #
   if [ "$rpcsec2_conf_ca_out" = "error" ]
   then
      echo "     ERROR: User identity not correct."
      exit 1
   fi
   echo "     User identity confirmed."
   echo ""
   
   #
   # check for variables we need. This should be done by the top level 
   # routine, but just in case...
   #
   echo "     Checking for variable definitions for security setup..."
   rpcsec2_sec_cnf_chkvars
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_cc_out" = "error" ]
   then
      #
      # report and error
      #
      echo "     ERROR: Variables needed for security setup are missing."
      exit 1
   fi
   echo "     All variables needed for security setup are defined."
   echo ""
   
   echo "     Adding principals and accounts for $RPCSEC2_SRV_PRINC_NAME and $RPCSEC2_CLI_PRINC_NAME"

   #
   # Add principal for server
   #
   echo "     Adding Principal for $RPCSEC2_SRV_PRINC_NAME..."
   rpcsec2_sec_add_princ $RPCSEC2_SRV_PRINC_NAME "RPC/SEC Systest server principal"
 
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_ap_out" = "error" ]
   then
      #
      # print error and quit
      #
      echo "     ERROR: Could not create principal for $RPCSEC2_SRV_PRINC_NAME"
      exit 2
   fi
   echo "     Principal $RPCSEC2_SRV_PRINC_NAME created..."
   echo ""

   #
   # Add principal for client
   #
   echo "     Adding Principal for $RPCSEC2_CLI_PRINC_NAME..."
   rpcsec2_sec_add_princ $RPCSEC2_CLI_PRINC_NAME \
                                             "RPC/SEC Systest client principal"
 
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_ap_out" = "error" ]
   then
      #
      # print error and quit
      #
      echo "     ERROR: Could not create principal for $RPCSEC2_CLI_PRINC_NAME"
      exit 3
   fi
   echo "     Principal $RPCSEC2_CLI_PRINC_NAME created..."
   echo ""

   #
   # Create group for the server
   #
   echo "     Creating the server group ($RPCSEC2_SRV_GROUP_NAME)..."
   rpcsec2_sec_add_group $RPCSEC2_SRV_GROUP_NAME "RPC/SEC Systest Server Group"
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_ag_out" = "error" ]
   then
      #
      # report and quit with error status
      #
      echo "     ERROR: Could not create server group..."
      exit 4
   fi
   echo "     Created server group ($RPCSEC2_SRV_GROUP_NAME)..."
   echo ""

   #
   # Create an account for the server
   #
   echo "     Creating an account for $RPCSEC2_SRV_PRINC_NAME..."
   rpcsec2_sec_add_account server $RPCSEC2_SRV_PRINC_NAME  $RPCSEC2_SRV_GROUP_NAME $RPCSEC2_SRV_INIT_PW "RPC_SEC Systest server account"
   
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_aa_out" = "error" ]
   then
      #
      # report and quit with an error status
      #
      echo "     ERROR: Couldn't create account for $RPCSEC2_SRV_PRINC_NAME."
      exit 5
   fi
   echo "     Created account for $RPCSEC2_SRV_PRINC_NAME."
   echo ""

   #
   # Create an account for the client
   #
   echo "     Creating an account for $RPCSEC2_CLI_PRINC_NAME..."
   rpcsec2_sec_add_account client $RPCSEC2_CLI_PRINC_NAME  none  $RPCSEC2_CLI_INIT_PW "RPC_SEC Systest client account"
   
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_aa_out" = "error" ]
   then
      #
      # report and quit with an error status
      #
      echo "     ERROR: Could not create account for $RPCSEC2_CLI_PRINC_NAME."
      exit 5
   fi
   echo "     Created an account for $RPCSEC2_CLI_PRINC_NAME..."
   echo ""
   
   #
   # Create the keytab file for the server
   #
   echo "     Creating keytab and adding keys for $RPCSEC2_SRV_PRINC_NAME..."
   rpcsec2_sec_add_key $RPCSEC2_SRV_PRINC_NAME non-random \
                       $RPCSEC2_SRV_INIT_PW \
                       $RPCSEC2_SRV_KEYTAB_DIRPATH $RPCSEC2_SRV_KEYTAB_FN
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_ak_out" = "error" ] 
   then
      #
      # report...
      #
      echo "     ERROR: Couldn't add keys for $RPCSEC2_SRV_PRINC_NAME."
      #
      # and exit with an error status
      #
      exit 3
   fi
   echo "     Added keys and keytab file $RPC_SRV_KEYTAB_DIRPATH/$RPCSEC2_SRV_KEYTAB_FN for $RPCSEC2_SRV_PRINC_NAME"
   echo ""

   #
   # Create the keytab file for the client
   #
   echo "     Creating keytab and adding keys for $RPCSEC2_CLI_PRINC_NAME..."
   rpcsec2_sec_add_key $RPCSEC2_CLI_PRINC_NAME non-random \
                       $RPCSEC2_CLI_INIT_PW $RPCSEC2_CLI_KEYTAB_DIRPATH \
                       $RPCSEC2_CLI_KEYTAB_FN
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_ak_out" = "error" ] 
   then
      #
      # report...
      #
      echo "     ERROR: Couldn't add keys for $RPCSEC2_CLI_PRINC_NAME."
      #
      # and exit with an error status
      #
      exit 3
   fi
   echo "     Added keys and keytab file $RPC_CLI_KEYTAB_DIRPATH/$RPCSEC2_CLI_KEYTAB_FN for $RPCSEC2_CLI_PRINC_NAME"
   echo ""
   
   #
   # change the permissions on the keytab files, but first let the user 
   # know that this is not the right way to operate.
   #
   echo "     Changing permissions for the keytab files..."
   echo "                    NOTE"
   echo "     Changing permissions for the keytab files to 644.  This is NOT"
   echo "     the recommended mode for security keytab files.  The keytab"
   echo "     files are protected via regular UNIX security (ie: file mode"
   echo "     bits, and access to the files should be as restrictive as is"
   echo "     reasonable." 
   echo "     For a real application, a unix account would be created for each"
   echo "     principal that owns a keytab file, and then access to the keytab"
   echo "     files would be restricted in some way akin to the following:"
   echo "         1) add a group to the groups file, the only members of which"
   echo "            being the principal uid and any uids representing users"
   echo "            authorized to start the client or server that owns the"
   echo "            keytab file.  The keytab file would be owned by the"
   echo "            client or server uid, and the permissions would be set"
   echo "            to 640 (read/write for owner, read for group)."
   echo "         2) the keytab file is owned by the client or server uid,"
   echo "            the permissions on the file are set to be 600 (read/write"
   echo "            for owner, no access for others), and the file mode bits"
   echo "            on the client or server program that will be accessing"
   echo "            the keytab file are set to be 4755 (read/write/execute"
   echo "            for owner, read/execute for group and others, and setuid"
   echo "            to owner on execution."
   echo "     In the interests of simplifying the administration of the"
   echo "     rpc.sec.2 systest, however, we let everyone read the keytabs."
   chmod 644 $RPCSEC2_CLI_KEYTAB_DIRPATH/$RPCSEC2_CLI_KEYTAB_FN > /dev/null 2>&1
   #
   # if we failed
   #
   if [ $? -ne 0 ]
   then
      # 
      # report error ...
      #
      echo "     ERROR: Couldn't change mode bits on $RPCSEC2_CLI_KEYTAB_DIRPATH/$RPCSEC2_CLI_KEYTAB_FN"
      #
      # exit with error status
      # 
      exit 6
   fi
   echo "     Changed mode bits on $RPCSEC2_CLI_KEYTAB_DIRPATH/$RPCSEC2_CLI_KEYTAB_FN"
   echo ""

   chmod 644 $RPCSEC2_SRV_KEYTAB_DIRPATH/$RPCSEC2_SRV_KEYTAB_FN >/dev/null 2>&1
   #
   # if we failed
   #
   if [ $? -ne 0 ]
   then
      # 
      # report error ...
      #
      echo "     ERROR: Couldn't change mode bits on $RPCSEC2_SRV_KEYTAB_DIRPATH/$RPCSEC2_SRV_KEYTAB_FN"
      #
      # exit with error status
      # 
      exit 6
   fi
   echo "     Changed mode bits on $RPCSEC2_SRV_KEYTAB_DIRPATH/$RPCSEC2_SRV_KEYTAB_FN"
   echo ""

   #
   # say bye
   #
   echo "   COMPLETED Security Namespace Setup(rpc.sec.2_sec_conf.sh) @ `date`"
   echo "---------------------------------------------------------------------------"
   exit 0
