#!/bin/sh
# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: rpc.cds.3_cds_conf.sh,v $
# Revision 1.1.6.2  1996/03/11  02:45:35  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:00  marty]
#
# Revision 1.1.6.1  1995/12/11  22:42:34  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:19:50  root]
# 
# Revision 1.1.4.1  1994/06/10  20:49:56  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:19:29  devsrc]
# 
# Revision 1.1.2.3  1993/03/12  18:55:21  mhickey
# 	Fixing bsubmit mystery meat
# 	[1993/03/12  18:50:36  mhickey]
# 
# Revision 1.1.1.2  1993/01/29  19:01:05  mhickey
# 	Initial check in for the rpc.cds.3 system test.
# 
# Revision 1.1.1.2  1992/12/03  15:36:15  mhickey
# 	Initial submit for the rpc.sec.2 system test.
# 
# 	Defect 5887.
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
. ${RPCCDS3_CONF:-rpc.cds.3.conf}
. rpc.cds.3_cds_util.sh



   
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
  # the RPCCDS3_SRV_CDS_NAME has the name of the entry in it, so strip it
  # off before passing the path to tstrpc_cds_makepath
  #
  echo "     CDS directory where server interface will reside is $RPCCDS3_SRV_CDS_NAME..."
  cds_name=`echo $RPCCDS3_SRV_CDS_NAME | awk -F'/' '{print $NF;}'`
  cds_path=`echo $RPCCDS3_SRV_CDS_NAME | sed -e "s/\/$cds_name//"`
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
  rpcsec2_cds_setacl $cds_path group:$RPCCDS3_SRV_GROUP_NAME:$RPCCDS3_SRV_CDS_DIR_ACL

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
  # now set the initial object acl on the directory to allow principles from
  # the server group to insert entries, etc.
  #
  echo "     Modifying initial object ACL on CDS directory $cds_path..."
  rpcsec2_cds_setacl $cds_path -io group:$RPCCDS3_SRV_GROUP_NAME:$RPCCDS3_SRV_CDS_IO_ACL

  #
  # if we failed
  #
  if [ "$rpcsec2_cds_sa_out" = "error" ]
  then
     #
     # report and exit with error status
     #
     echo "     ERROR: Could not set the initial object ACL on CDS directory $cds_path."
     exit 2
  fi
  echo "     Set directory inital object ACL on $cds_path..."
  echo ""

  #
  #
  # Create the necessary directories for the server calendar entries in the
  # namespace
  #
  # the RPCCDS3_SRV_OBJ_DIR has the name of the entry in it, so strip it
  # off before passing the path to tstrpc_cds_makepath
  #
  echo "     CDS directory where calendar objects will reside is $RPCCDS3_SRV_OBJ_DIR..."
  cds_path=$RPCCDS3_SRV_OBJ_DIR
  echo "     Creating $cds_path..."
  rpcsec2_cds_makepath $cds_path
  
  #
  # if we failed...
  #
  if [ "$rpcsec2_cds_mp_out" = "error" ]
  then
     echo "     ERROR: Can't create CDS directory ($cds_path) for calendar_objects."
     exit 1
  fi
  echo "     Created CDS directory $cds_path..."
  echo ""

  #
  # now set the acl on the directory to allow principles from the server
  # group to insert entries, etc.
  #
  echo "     Modifying ACL on CDS directory $cds_path..."
  rpcsec2_cds_setacl $cds_path group:$RPCCDS3_SRV_GROUP_NAME:$RPCCDS3_SRV_CDS_DIR_ACL

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
  # now set the inital object acl on the directory to allow principles from
  # the server group to insert entries, etc.
  #
  echo "     Modifying ACL on CDS directory $cds_path..."
  rpcsec2_cds_setacl $cds_path -io group:$RPCCDS3_SRV_GROUP_NAME:$RPCCDS3_SRV_CDS_IO_ACL

  #
  # if we failed
  #
  if [ "$rpcsec2_cds_sa_out" = "error" ]
  then
     #
     # report and exit with error status
     #
     echo "     ERROR: Could not set the inital object ACL on CDS directory $cds_path."
     exit 2
  fi
  echo "     Set directory inital object ACL on $cds_path..."
  echo ""

  #
  # say bye
  #
  echo "   COMPLETED CDS Namespace Setup(rpc.cds.3_cds_conf.sh)"
