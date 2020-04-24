#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: two_dsas.sh,v $
# Revision 1.1.4.2  1996/03/11  02:34:28  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:39:50  marty]
#
# Revision 1.1.4.1  1995/12/11  15:25:03  root
# 	Submit
# 	[1995/12/11  14:35:51  root]
# 
# Revision 1.1.2.4  1994/07/15  00:01:13  keutel
# 	OT 11312: removed some sleeps
# 	[1994/07/14  23:55:31  keutel]
# 
# Revision 1.1.2.3  1994/07/06  15:54:02  marrek
# 	Add version string in July 1994 code drop.
# 	[1994/07/06  15:42:17  marrek]
# 
# Revision 1.1.2.2  1994/06/10  20:20:49  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:10  devsrc]
# 
# Revision 1.1.2.1  1994/05/10  14:33:07  marrek
# 	Bug fixes for April 1994 submission.
# 	[1994/05/05  09:18:28  marrek]
# 
# 	Created in April 1994 submission.
# 	[1994/05/03  12:14:02  marrek]
# 
# $EndLog$
#
# $RCSfile: two_dsas.sh,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:34:28 $
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
  echo "Local CACHE administration (DIR-IDs=1)"

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
  echo "Local CACHE administration (DIR-IDs=1,2,5)"

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

local_cache34_admin() 
{
  echo "Local CACHE administration (DIR-ID=3)"

  gdsditadm <  ${TET_EXECUTE}/config/t.3cacheinit1 > /dev/null

  if [ `grep -c 'ERROR:' $CL/tstfile` -gt 0 ]
  then
     echo "\ngdsditadm < ${TET_EXECUTE}/config/t.3cacheinit1"
     echo "              processed with ERROR"
     echo "Use  <vi $CL/tstfile>  for detailed information"
     echo ""
     exit
  fi

  echo "Local CACHE administration (DIR-ID=4)"

  gdsditadm <  ${TET_EXECUTE}/config/t.4cacheinit1 > /dev/null

  if [ `grep -c 'ERROR:' $CL/tstfile` -gt 0 ]
  then
     echo "\ngdsditadm < ${TET_EXECUTE}/config/t.4cacheinit1"
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
  gdssysadm -fc -m2 -d3                          # deconfigure dit 3 on server
  gdssysadm -fc -m2 -d4                          # deconfigure dit 4 on server
  gdssysadm -fc -m1 -d3 -c2 -s1 -C16 -u0 -A2     # configure dit 3 on server
  gdssysadm -fc -m1 -d4 -c2 -s1 -C16 -u0 -A2     # configure dit 4 on server
  gdssysadm -fA                                  # activate the dsa

  local_cache1c_admin;
  local_cache34_admin;
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
  gdssysadm -fc -m2 -d3                          # deconfigure dit 3 
  gdssysadm -fc -m2 -d4                          # deconfigure dit 4 
  gdssysadm -fc -m2 -d5                          # deconfigure dit 4 
  gdssysadm -fc -m4 -d1 -c1 -C16
  gdssysadm -fc -m1 -d2 -c2 -s1 -C16 -u1 -A2     # configure dit 2 
  gdssysadm -fc -m1 -d3 -c2 -s1 -C16 -u0 -A2     # configure dit 3 
  gdssysadm -fc -m1 -d4 -c2 -s1 -C16 -u0 -A2     # configure dit 4 
  gdssysadm -fc -m1 -d5 -c2 -s1 -C16 -u1 -A2     # configure dit 5 
  gdssysadm -fA                                  # activate the server

  local_cache125_admin;
  local_cache34_admin;

  echo "Waiting 60 sec. for coming up the DSA processes"
  sleep 60

  local_dsa12_admin;

  echo "Now, testing is going on ... "
}

case "$TET_GDS_CONFIG"
in
        1) server_start;;
	3) local_start;;
esac
