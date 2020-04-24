#!/bin/sh
# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: rpc.sec.2_cds_conf.sh,v $
# Revision 1.1.65.2  1996/03/11  02:46:26  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:19  marty]
#
# Revision 1.1.65.1  1995/12/11  22:43:42  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:20:31  root]
# 
# Revision 1.1.63.1  1993/09/09  19:12:43  cmckeen
# 	 HP's TET'ized version
# 	[1993/09/09  19:11:41  cmckeen]
# 
# Revision 1.1.5.2  1993/08/16  14:18:51  eheller
# 	File tetified.
# 	[1993/07/23  17:41:37  eheller]
# 
# Revision 1.1.1.5  1993/05/04  22:22:01  mgm
# 	    Loading drop DCE1_0_2FNL
# 
# Revision 1.1.2.4  1993/02/05  15:22:10  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:42:54  cjd]
# 
# Revision 1.1.3.2  1993/02/04  22:11:33  cjd
# 	Embedded copyright notice
# 
# Revision 1.1.2.3  1993/01/13  19:26:57  mhickey
# 	Fixed bad syntax in awk call, and added preference for
# 	an environmental variable value when reading the
# 	config file.
# 	[1993/01/13  19:18:48  mhickey]
# 
# Revision 1.1.2.2  1992/12/03  19:52:38  mhickey
# 	Initial submit for the rpc.sec.2 system test.
# 
# 	Defect 5887.
# 	[1992/12/03  15:36:15  mhickey]
# 
# $EndLog$
# 
# rpc.sec.2_cds_conf.sh
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
#   Sets up a directory in the CDS namespace into which the test server 
#   interface entry for the rpc.sec.2 system test will be exported,
#   and sets the ACL and initial object ACL on the directory to restrict use
#   to the server group created by rpc.sec.2_sec_conf.sh
#
# CALLED BY
#   Called by rpc.sec.2_setup.sh, the driver setup script.
# 
# CALLS INTO
#   Uses variable definitions from rpc.sec.2.conf
#   Uses functions from rpc.sec.2_cds_utils.sh.
#
# PREREQUISITES
#   Assumes a DCE cell with CDS is running on the system.
#        
#
# Include the configuration file.  The variable set therein that we care 
# about is
#   TSTRPC_SRV_CDS_NAME
#
. ${RPCSEC2_CONF:-rpc.sec.2.conf}
. $RPCSEC2_BIN_DIRPATH/rpc.sec.2_cds_util.sh



   
#
# main
#
  #
  # say hi
  #
  echo ""
  echo "   BEGINNING CDS Namespace Setup(rpc.sec.2_cds_conf.sh) @ `date`"
  echo "--------------------------------------------------------------------------"
  echo ""

  #
  # check to make sure all our variables are defined.  Note that the 
  # top level routine should handle this, but just in case...
  #
  echo "     Checking for necessary variable definitions..."
  rpcsec2_cds_cnf_chkvars
  #
  # if we failed...
  #
  if [ "$rpcsec2_cds_cc_out" = "error" ]
  then 
     #
     # report and quit
     #
     echo "     ERROR: Variable definitions needed for CDS setup missing."
     exit 1
  fi
  echo "     All variables necessary for CDS setup are defined."
  echo ""

  #
  # Create the necessary directories for the server interface entry in the
  # namespace
  #
  # the RPCSEC2_SRV_CDS_NAME has the name of the entry in it, so strip it
  # off before passing the path to tstrpc_cds_makepath
  #
  echo "     CDS directory where server interface will reside is $RPCSEC2_SRV_CDS_NAME..."
  cds_name=`echo $RPCSEC2_SRV_CDS_NAME | awk -F'/' '{print $NF;}'`
  cds_path=`echo $RPCSEC2_SRV_CDS_NAME | sed -e "s/\/$cds_name//"`
  echo "     Creating $cds_path..."
  rpcsec2_cds_makepath $cds_path
  
  #
  # if we failed...
  #
  if [ "$rpcsec2_cds_mp_out" = "error" ]
  then
     echo "     ERROR: Can't create CDS directory ($cds_path) for server interface."
     exit 1
  fi
  echo "     Created CDS directory $cds_path..."
  echo ""

  #
  # now set the acl on the directory to allow principles from the server
  # group to insert entries, etc.
  #
  echo "     Modifying ACL on CDS directory $cds_path..."
  rpcsec2_cds_setacl $cds_path group:$RPCSEC2_SRV_GROUP_NAME:$RPCSEC2_SRV_CDS_DIR_ACL

  #
  # if we failed
  #
  if [ "$rpcsec2_cds_sa_out" = "error" ]
  then
     #
     # report and exit with error status
     #
     echo "     ERROR: Could not set the ACL on CDS directory $cds_path."
     exit 2
  fi
  echo "     Set directory ACL on $cds_path..."
  echo ""

  #
  # say bye
  #
  echo "   COMPLETED CDS Namespace Setup(rpc.sec.2_cds_conf.sh) @ `date`"
  echo "--------------------------------------------------------------------------"
  exit 0



