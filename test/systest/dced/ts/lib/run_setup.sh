#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: run_setup.sh,v $
# Revision 1.1.4.2  1996/03/11  02:38:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:13  marty]
#
# Revision 1.1.4.1  1995/12/11  21:55:10  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:55:18  root]
# 
# Revision 1.1.2.2  1994/08/10  19:23:20  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:17:36  annie]
# 
# Revision 1.1.2.1  1994/06/27  19:00:40  baum
# 	Initial checkin
# 	[1994/06/27  19:00:02  baum]
# 
# $EndLog$

NUM_PARAMS=$#

VTARLM=26
trap 'echo "run_setup.sh: Caught VTARLM "' $VTARLM 

if [[ -z $TET_ROOT ]]
then
  echo TET_ROOT not defined.
  exit 1
fi

if [ $NUM_PARAMS != 1 ]
then
  echo "Usage: run_setup.sh /.../CELL/HOST"
  exit 1
fi

$TET_ROOT/tools/tet_setup dcd_st_setup.tcl $1
