#!/bin/ksh
# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: gds_sec_start.sh,v $
# Revision 1.1.4.2  1996/03/11  02:34:21  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:39:45  marty]
#
# Revision 1.1.4.1  1995/12/11  15:24:32  root
# 	Submit
# 	[1995/12/11  14:35:42  root]
# 
# Revision 1.1.2.4  1994/08/24  15:15:47  keutel
# 	OT 11279
# 	[1994/08/24  14:13:47  keutel]
# 
# Revision 1.1.2.3  1994/08/19  14:54:41  keutel
# 	OT 11752
# 	[1994/08/19  14:01:54  keutel]
# 
# Revision 1.1.2.2  1994/08/18  12:22:59  keutel
# 	OT 11279
# 	[1994/08/17  15:34:32  keutel]
# 
# Revision 1.1.2.1  1994/07/06  15:10:21  marrek
# 	Created in July 1994 code drop.
# 	[1994/07/06  13:02:20  marrek]
# 
# $EndLog$
#
# $RCSfile: gds_sec_start.sh,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:34:21 $
# 
# Initial script for    
#
#   (1) GDS configuration and
#   (2) GDS administration (DSA names in DUA-Cache and in DSA's)
#
#
export NLSPATH=/usr/lib/nls/msg/En_US/%N:/opt/dcelocal/nls/msg/en_US.ASCII/%N
export CL=/opt/dcelocal/var/adm/directory/gds/adm

local_cache11_admin()
{
  echo "" >> $OUTDIR/gds_start.log
  echo "Local CACHE administration (DIR-IDs=11)" >> $OUTDIR/gds_start.log

  gdsditadm < ${TET_EXECUTE}/config/dsa11_init.bat > /dev/null

  if [ `grep -c 'ERROR:' $CL/tstfile` -gt 0 ]
  then
     echo "\ngdsditadm < ${TET_EXECUTE}/config/dsa11_init.bat"
     echo "              processed with ERROR"
     echo "Use  <vi $CL/tstfile>  for detailed information"
     echo "See also $OUTDIR/gds_start.log for history logging"
     echo ""
     exit
  fi
}

local_cache12_admin()
{
  echo "Local CACHE administration (DIR-IDs=12)" >> $OUTDIR/gds_start.log

  gdsditadm < ${TET_EXECUTE}/config/dsa12_init.bat > /dev/null

  if [ `grep -c 'ERROR:' $CL/tstfile` -gt 0 ]
  then
     echo "\ngdsditadm < ${TET_EXECUTE}/config/dsa12_init.bat"
     echo "              processed with ERROR"
     echo "Use  <vi $CL/tstfile>  for detailed information"
     echo "See also $OUTDIR/gds_start.log for history logging"
     echo ""
     exit
  fi
}

reg_setup()
{
#  echo "dce_login cell_admin <password> 2>/dev/null"
#  dce_login cell_admin $DCE_PASSWD 2>/dev/null
#  ksh
  echo "\ndcecp dcecp_add.tcl" >> $OUTDIR/gds_start.log
  dcecp ${TET_EXECUTE}/config/dcecp_add.tcl > /dev/null
  echo "rgy_add.sh" >> $OUTDIR/gds_start.log
  ${TET_EXECUTE}/config/rgy_add.sh > /dev/null
}

local_start()
{
  echo "local_start: Configure GDS" >> $OUTDIR/gds_start.log
  gdssysadm -fd -p                               # deactivate GDS
  gdssysadm -fc -m2 -d11 -p                      # delete dit 11 
  gdssysadm -fc -m2 -d12 -p                      # delete dit 12 
  gdssysadm -fc -m1 -d1 -c1 -C16 -p		 # configure dit 1 as client
  gdssysadm -fc -m1 -d11 -c2 -s2 -C16 -A25       # configure dit 11 
  gdssysadm -fc -m1 -d12 -c2 -s2 -C16 -A25       # configure dit 12 
  gdssysadm -fA                                  # activate the GDS


  # add client address into cache of dir. id 1

  gdscp -c "x500obj bind -dirid 1 -cache; \
            x500svc modify -preferadmfunctions TRUE -duacache TRUE -duafirst TRUE -usedsa FALSE -cacheclass RESIDENT -dontusecopy FALSE; \
            x500obj create 43.12.2.1107.1.3.4.10=CLIENT -attribute PSA={TS=Client,NA='TCP/IP!internet=127.0.0.1+port=31010'}"

  local_cache11_admin;
  local_cache12_admin;

  echo "Waiting 60 sec. for the DSA processes to come up" >> $OUTDIR/gds_start.log
  sleep 60

  echo "Adding objects in DSA-11" >> $OUTDIR/gds_start.log
  gdscp ${TET_EXECUTE}/config/dsa11_objects.tcl > /dev/null

  gdsditadm < ${TET_EXECUTE}/config/copy_subtree.bat > /dev/null

  if [ `grep -c 'ERROR:' $CL/tstfile` -gt 0 ]
  then
     echo "\ngdsditadm < ${TET_EXECUTE}/config/copy_subtree.bat"
     echo "              processed with ERROR"
     echo "Use  <vi $CL/tstfile>  for detailed information"
     echo "See also $OUTDIR/gds_start.log for history logging"
     echo ""
     exit
  fi

  echo "Adding objects in DSA-12" >> $OUTDIR/gds_start.log
  gdscp ${TET_EXECUTE}/config/dsa12_objects.tcl > /dev/null

  echo "Now, testing is going on ... " >> $OUTDIR/gds_start.log
}

reg_setup;
local_start;
