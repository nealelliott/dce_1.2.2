#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: rpc.cds.3_sec_conf.sh,v $
# Revision 1.1.6.2  1996/03/11  02:45:55  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:08  marty]
#
# Revision 1.1.6.1  1995/12/11  22:42:58  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:20:05  root]
# 
# Revision 1.1.4.1  1994/06/10  20:50:37  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:19:45  devsrc]
# 
# Revision 1.1.2.2  1993/03/12  04:28:09  mhickey
# 	Initial check -in for the rpc.cds.3 system test.
# 	[1993/01/29  20:27:47  mhickey]
# 
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
. ${RPCCDS3_CONF:-rpc.cds.3.conf}
. rpc.cds.3_conf_util.sh
. rpc.cds.3_sec_util.sh


#
# main
# 
#
   echo ""
   echo "   BEGINNING Security Namespace Setup(rpc.cds.3_sec_conf.sh) @ `date`"
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
   
   echo "     Adding principals and accounts for $RPCCDS3_SRV_PRINC_NAME and $RPCCDS3_CLI_PRINC_NAME"

   #
   # Add principal for server
   #
   echo "     Adding Principal for $RPCCDS3_SRV_PRINC_NAME..."
   rpcsec2_sec_add_princ $RPCCDS3_SRV_PRINC_NAME "RPC/SEC Systest server principal"
 
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_ap_out" = "error" ]
   then
      #
      # print error and quit
      #
      echo "     ERROR: Could not create principal for $RPCCDS3_SRV_PRINC_NAME"
      exit 2
   fi
   echo "     Principal $RPCCDS3_SRV_PRINC_NAME created..."
   echo ""

   #
   # Add principal for client
   #
   echo "     Adding Principal for $RPCCDS3_CLI_PRINC_NAME..."
   rpcsec2_sec_add_princ $RPCCDS3_CLI_PRINC_NAME \
                                             "RPC/SEC Systest client principal"
 
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_ap_out" = "error" ]
   then
      #
      # print error and quit
      #
      echo "     ERROR: Could not create principal for $RPCCDS3_CLI_PRINC_NAME"
      exit 3
   fi
   echo "     Principal $RPCCDS3_CLI_PRINC_NAME created..."
   echo ""

   #
   # Create group for the server
   #
   echo "     Creating the server group ($RPCCDS3_SRV_GROUP_NAME)..."
   rpcsec2_sec_add_group $RPCCDS3_SRV_GROUP_NAME "RPC/SEC Systest Server Group"
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
   echo "     Created server group ($RPCCDS3_SRV_GROUP_NAME)..."
   echo ""

   #
   # Create an account for the server
   #
   echo "     Creating an account for $RPCCDS3_SRV_PRINC_NAME..."
   rpcsec2_sec_add_account server $RPCCDS3_SRV_PRINC_NAME  $RPCCDS3_SRV_GROUP_NAME $RPCCDS3_SRV_INIT_PW "RPC_SEC Systest server account"
   
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_aa_out" = "error" ]
   then
      #
      # report and quit with an error status
      #
      echo "     ERROR: Couldn't create account for $RPCCDS3_SRV_PRINC_NAME."
      exit 5
   fi
   echo "     Created account for $RPCCDS3_SRV_PRINC_NAME."
   echo ""

   #
   # Create an account for the client
   #
   echo "     Creating an account for $RPCCDS3_CLI_PRINC_NAME..."
   rpcsec2_sec_add_account client $RPCCDS3_CLI_PRINC_NAME  none  $RPCCDS3_CLI_INIT_PW "RPC_SEC Systest client account"
   
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_aa_out" = "error" ]
   then
      #
      # report and quit with an error status
      #
      echo "     ERROR: Could not create account for $RPCCDS3_CLI_PRINC_NAME."
      exit 5
   fi
   echo "     Created an account for $RPCCDS3_CLI_PRINC_NAME..."
   echo ""
   
   #
   # Create the keytab file for the server
   #
   echo "     Creating keytab and adding keys for $RPCCDS3_SRV_PRINC_NAME..."
   rpcsec2_sec_add_key $RPCCDS3_SRV_PRINC_NAME non-random \
                       $RPCCDS3_SRV_INIT_PW \
                       $RPCCDS3_SRV_KEYTAB_DIRPATH $RPCCDS3_SRV_KEYTAB_FN
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_ak_out" = "error" ] 
   then
      #
      # report...
      #
      echo "     ERROR: Couldn't add keys for $RPCCDS3_SRV_PRINC_NAME."
      #
      # and exit with an error status
      #
      exit 3
   fi
   echo "     Added keys and keytab file $RPCCDS3_SRV_KEYTAB_DIRPATH/$RPCCDS3_SRV_KEYTAB_FN for $RPCCDS3_SRV_PRINC_NAME"
   echo ""

   #
   # Create the keytab file for the client
   #
   echo "     Creating keytab and adding keys for $RPCCDS3_CLI_PRINC_NAME..."
   rpcsec2_sec_add_key $RPCCDS3_CLI_PRINC_NAME non-random \
                       $RPCCDS3_CLI_INIT_PW $RPCCDS3_CLI_KEYTAB_DIRPATH \
                       $RPCCDS3_CLI_KEYTAB_FN
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_ak_out" = "error" ] 
   then
      #
      # report...
      #
      echo "     ERROR: Couldn't add keys for $RPCCDS3_CLI_PRINC_NAME."
      #
      # and exit with an error status
      #
      exit 3
   fi
   echo "     Added keys and keytab file $RPCCDS3_CLI_KEYTAB_DIRPATH/$RPCCDS3_CLI_KEYTAB_FN for $RPCCDS3_CLI_PRINC_NAME"
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
   chmod 644 $RPCCDS3_CLI_KEYTAB_DIRPATH/$RPCCDS3_CLI_KEYTAB_FN > /dev/null 2>&1
   #
   # if we failed
   #
   if [ $? -ne 0 ]
   then
      # 
      # report error ...
      #
      echo "     ERROR: Couldn't change mode bits on $RPCCDS3_CLI_KEYTAB_DIRPATH/$RPCCDS3_CLI_KEYTAB_FN"
      #
      # exit with error status
      # 
      exit 6
   fi
   echo "     Changed mode bits on $RPCCDS3_CLI_KEYTAB_DIRPATH/$RPCCDS3_CLI_KEYTAB_FN"
   echo ""

   chmod 644 $RPCCDS3_SRV_KEYTAB_DIRPATH/$RPCCDS3_SRV_KEYTAB_FN >/dev/null 2>&1
   #
   # if we failed
   #
   if [ $? -ne 0 ]
   then
      # 
      # report error ...
      #
      echo "     ERROR: Couldn't change mode bits on $RPCCDS3_SRV_KEYTAB_DIRPATH/$RPCCDS3_SRV_KEYTAB_FN"
      #
      # exit with error status
      # 
      exit 6
   fi
   echo "     Changed mode bits on $RPCCDS3_SRV_KEYTAB_DIRPATH/$RPCCDS3_SRV_KEYTAB_FN"
   echo ""

   #
   # say bye
   #
   echo "   COMPLETED Security Namespace Setup(rpc.cds.3_sec_conf.sh) @ `date`"
   echo "---------------------------------------------------------------------------"
   exit 0
