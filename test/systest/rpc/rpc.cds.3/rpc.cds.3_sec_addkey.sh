#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: rpc.cds.3_sec_addkey.sh,v $
# Revision 1.1.6.2  1996/03/11  02:45:53  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:07  marty]
#
# Revision 1.1.6.1  1995/12/11  22:42:57  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:20:04  root]
# 
# Revision 1.1.4.1  1994/06/10  20:50:36  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:19:44  devsrc]
# 
# Revision 1.1.2.3  1993/03/12  18:56:16  mhickey
# 	Fixing bsubmit mystery meat
# 	[1993/03/12  18:51:42  mhickey]
# 
# Revision 1.1.1.2  1993/01/29  20:27:33  mhickey
# 	Initial check -in for the rpc.cds.3 system test.
# 
# Revision 1.1.2.2  1993/01/13  20:03:58  mhickey
# 	Initial submit for the remote keytab file creatino script.
# 	[1993/01/13  20:03:38  mhickey]
# 
# $EndLog$
# 
# rpc.sec.2_sec_addkey.sh
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
#    Creates a client keytab file on a remote machine via unauthenticated 
#    rgy_edit session.
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

. ${RPCCDS3_CONF:-rpc.cds.3.conf}
. rpc.cds.3_sec_util.sh

#
# main
#
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

   if [ "$1" = "client" ]
   then
      rs2_ak_name=$RPCCDS3_CLI_PRINC_NAME
      rs2_ak_dir=$RPCCDS3_CLI_KEYTAB_DIRPATH   
      rs2_ak_fn=$RPCCDS3_CLI_KEYTAB_FN
      rs2_ak_pw=$RPCCDS3_CLI_INIT_PW
   else
      rs2_ak_name=$RPCCDS3_SRV_PRINC_NAME
      rs2_ak_dir=$RPCCDS3_SRV_KEYTAB_DIRPATH   
      rs2_ak_fn=$RPCCDS3_SRV_KEYTAB_FN
      rs2_ak_pw=$RPCCDS3_SRV_INIT_PW
   fi
   #
   # Create the keytab file for the client
   #
   echo "     Creating keytab and adding keys for $rs2_ak_name..."
   rpcsec2_sec_add_key $rs2_ak_name non-random \
                       $rs2_ak_pw \
                       $rs2_ak_dir $rs2_ak_fn
   #
   # if we failed...
   #
   if [ "$rpcsec2_sec_ak_out" = "error" ] 
   then
      #
      # report...
      #
      echo "     ERROR: Couldn't add keys for $rs2_ak_name."
      #
      # and exit with an error status
      #
      exit 3
   fi
   echo "     Added keys and keytab file $rs2_ak_dir/$rs2_ak_fn for $rs2_ak_name"
   echo ""
   


