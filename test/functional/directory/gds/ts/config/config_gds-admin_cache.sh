#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: config_gds-admin_cache.sh,v $
# Revision 1.1.4.2  1996/03/11  02:34:10  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:39:35  marty]
#
# Revision 1.1.4.1  1995/12/11  15:24:02  root
# 	Submit
# 	[1995/12/11  14:35:30  root]
# 
# Revision 1.1.2.4  1994/07/15  00:01:11  keutel
# 	OT 11312: removed some sleeps
# 	[1994/07/14  23:55:29  keutel]
# 
# Revision 1.1.2.3  1994/07/06  15:54:00  marrek
# 	Add version string in July 1994 code drop.
# 	[1994/07/06  15:42:12  marrek]
# 
# Revision 1.1.2.2  1994/06/10  20:20:47  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:08  devsrc]
# 
# Revision 1.1.2.1  1994/05/10  14:32:33  marrek
# 	Bug fixes for April 1994 submission.
# 	[1994/05/05  09:18:26  marrek]
# 
# 	Created in April 1994 submission.
# 	[1994/05/03  12:13:58  marrek]
# 
# $EndLog$
#
# $RCSfile: config_gds-admin_cache.sh,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:34:10 $
#
# Initial script for    
#
#   (1) GDS configuration and
#   (2) GDS administration (DSA names in DUA-Cache and in DSA's)
#
#
export NLSPATH=/usr/lib/nls/msg/En_US/%N:/opt/dcelocal/nls/msg/en_US.ASCII/%N
export CL=/opt/dcelocal/var/adm/directory/gds/adm

rgdssysadm()
{
  echo "rgdssysadm $*"
  remote_gdssysadm "192.35.18.2" root "$REMOTE_PASSWD" "$NLSPATH" "$1"
  if [ $? != 0 ]
  then
    echo "\nremote_gdssysadm returned with error - STOP\n"
    exit 1
  fi
}

rgdsditadm()
{
  echo "rgdsditadm ${1}"
  remote_gdsditadm "192.35.18.2" root "$REMOTE_PASSWD" "$NLSPATH" "$1"
  if [ $? != 0 ]
  then
    echo "\nremote_gdsditadm returned with error - STOP\n"
    exit 1
  fi
}

local_cache1c_admin()
{
  echo ""
  echo "CACHE administration (DIR-IDs=1c)"

  gdsditadm < ${TET_EXECUTE}/config/t.cacheinit1c > /dev/null

  if [ `grep -c 'ERROR:' $CL/tstfile` -gt 0 ]
  then
     echo "\ngdsditadm < ${TET_EXECUTE}/config/t.cacheinit1c"
     echo "              processed with ERROR"
     echo "Use  <vi $CL/tstfile>  for detailed information"
     echo ""
     exit
  fi
}

local_cache125_admin()
{
  echo ""
  echo "CACHE administration (DIR-IDs=1,2,5)"

  gdsditadm < ${TET_EXECUTE}/config/t.cacheinit125 > /dev/null

  if [ `grep -c 'ERROR:' $CL/tstfile` -gt 0 ]
  then
     echo "\ngdsditadm < ${TET_EXECUTE}/config/t.cacheinit125"
     echo "              processed with ERROR"
     echo "Use  <vi $CL/tstfile>  for detailed information"
     echo ""
     exit
  fi
}

local_dsa12_admin() 
{
  echo "DSA administration (dsa.chmaster)"

  gdsditadm < ${TET_EXECUTE}/config/dsa.chmaster > /dev/null

  if [ `grep -c 'ERROR:' $CL/tstfile` -gt 0 ]
  then
     echo "\ngdsditadm < ${TET_EXECUTE}/config/dsa.chmaster"
     echo "              processed with ERROR"
     echo "Use  <vi $CL/tstfile>  for detailed information"
     echo ""
     exit
  fi

  echo "DSA administration (dsa.dsa1)"

  gdsditadm < ${TET_EXECUTE}/config/dsa.dsa1 > /dev/null

  if [ `grep -c 'ERROR:' $CL/tstfile` -gt 0 ]
  then
     echo "\ngdsditadm < ${TET_EXECUTE}/config/t8.dsa1"
     echo "              processed with ERROR"
     echo "Use  <vi $CL/tstfile>  for detailed information"
     echo ""
     exit
  fi

  echo "DSA administration (dsa.dsa2)"

  gdsditadm < ${TET_EXECUTE}/config/dsa.dsa2 > /dev/null

  if [ `grep -c 'ERROR:' $CL/tstfile` -gt 0 ]
  then
     echo "\ngdsditadm < ${TET_EXECUTE}/config/dsa.dsa2"
     echo "              processed with ERROR"
     echo "Use  <vi $CL/tstfile>  for detailed information"
     echo ""
     exit
  fi
}

client_start() 
{
  gdssysadm -fd                                  # deactivate dsa
  gdssysadm -fc -m4 -d1 -c1 -C16                 # change configuration dit 1
  gdssysadm -fA                                  # activate the dsa

  local_cache1c_admin;
}

server_start () 
{
  rgdssysadm "-fd"                                 # deactivate server
  rgdssysadm "-fc -m2 -d2"                         # deconfigure dit 2 on server
  rgdssysadm "-fc -m2 -d5"                         # deconfigure dit 5 on server
  rgdssysadm "-fc -m1 -d2 -c2 -s1 -C16 -u0 -A2"    # configure dit 2 on server
  rgdssysadm "-fc -m1 -d5 -c2 -s1 -C16 -u0 -A2"    # configure dit 5 on server
  rgdssysadm "-fA"                                 # activate the server
  rgdsditadm "${TET_EXECUTE}/config/t.cacheinit2r"   # configure server dsas
  rgdsditadm "${TET_EXECUTE}/config/t.cacheinit5r"   # configure server dsas

  client_start;

  local_dsa12_admin;

  echo "\nNow, testing is going on ... \n"
}


local_start()
{
  gdssysadm -fd                                  # deactivate dsa
  gdssysadm -fc -m2 -d2                          # deconfigure dit 2 
  gdssysadm -fc -m2 -d5                          # deconfigure dit 4 
  gdssysadm -fc -m4 -d1 -c1 -C16
  gdssysadm -fc -m1 -d2 -c2 -s1 -C16 -u1 -A2     # configure dit 2 
  gdssysadm -fc -m1 -d5 -c2 -s1 -C16 -u1 -A2     # configure dit 5 
  gdssysadm -fA                                  # activate the server

  local_cache125_admin;

  echo "Waiting 60 sec. for coming up the DSA processes"
  sleep 60

  echo "Now, testing is going on ... "
}

case "$TET_GDS_CONFIG"
in
        1) server_start;;
	3) local_start;;
esac
