#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: rpc.sec.2_sec_addkey.sh,v $
# Revision 1.1.82.2  1996/03/11  02:46:48  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:29  marty]
#
# Revision 1.1.82.1  1995/12/11  22:44:05  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1994/08/04  15:17 UTC  bissen
# 	merge from HPDCE01
# 	[1995/12/11  22:20:45  root]
# 
# Revision 1.1.80.2  1993/10/18  13:40:17  cmckeen
# 	Added path to find rpc.sec.2_conf_util.sh
# 	[1993/10/18  13:30:35  cmckeen]
# 
# Revision 1.1.80.1  1993/09/09  19:13:00  cmckeen
# 	 HP's TET'ized version
# 	[1993/09/09  19:11:57  cmckeen]
# 
# Revision 1.1.6.2  1993/08/16  14:38:07  eheller
# 	File tetified.
# 	[1993/07/23  17:46:12  eheller]
# 
# Revision 1.1.1.5  1993/05/04  22:22:17  mgm
# 	    Loading drop DCE1_0_2FNL
# 
# Revision 1.1.2.4  1993/02/25  20:16:31  mhickey
# 	Added support for creating client or server keytabs.
# 	[1993/02/25  18:55:27  mhickey]
# 
# Revision 1.1.2.3  1993/02/05  15:23:03  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:44:17  cjd]
# 
# Revision 1.1.4.2  1993/02/04  22:13:35  cjd
# 	Embedded copyright notice
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

. ${RPCSEC2_CONF:-rpc.sec.2.conf}
. $RPCSEC2_BIN_DIRPATH/rpc.sec.2_sec_util.sh
. $RPCSEC2_BIN_DIRPATH/rpc.sec.2_conf_util.sh

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
      rs2_ak_name=$RPCSEC2_CLI_PRINC_NAME
      rs2_ak_dir=$RPCSEC2_CLI_KEYTAB_DIRPATH   
      rs2_ak_fn=$RPCSEC2_CLI_KEYTAB_FN
      rs2_ak_pw=$RPCSEC2_CLI_INIT_PW
   else
      rs2_ak_name=$RPCSEC2_SRV_PRINC_NAME
      rs2_ak_dir=$RPCSEC2_SRV_KEYTAB_DIRPATH   
      rs2_ak_fn=$RPCSEC2_SRV_KEYTAB_FN
      rs2_ak_pw=$RPCSEC2_SRV_INIT_PW
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
   


