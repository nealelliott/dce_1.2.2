#!/bin/ksh
# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: gds_sec_clean.sh,v $
# Revision 1.1.4.2  1996/03/11  02:34:19  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:39:43  marty]
#
# Revision 1.1.4.1  1995/12/11  15:24:30  root
# 	Submit
# 	[1995/12/11  14:35:41  root]
# 
# Revision 1.1.2.2  1994/08/19  14:54:40  keutel
# 	OT 11752
# 	[1994/08/19  14:02:18  keutel]
# 
# Revision 1.1.2.1  1994/07/06  15:10:18  marrek
# 	Created in July 1994 code drop.
# 	[1994/07/06  13:02:18  marrek]
# 
# $EndLog$
#
# $RCSfile: gds_sec_clean.sh,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:34:19 $
#
# Cleanup procedure for DCE_Security tests
#
export NLSPATH=/usr/lib/nls/msg/En_US/%N:/opt/dcelocal/nls/msg/en_US.ASCII/%N
export CL=/opt/dcelocal/var/adm/directory/gds/adm

reg_clean()
{
  echo "\nrgy_del.sh" >> $OUTDIR/gds_start.log
  ${TET_EXECUTE}/config/rgy_del.sh > /dev/null
  echo "dcecp dcecp_del.tcl" >> $OUTDIR/gds_start.log
  dcecp ${TET_EXECUTE}/config/dcecp_del.tcl > /dev/null
}

local_clean()
{
  echo "local_clean: Deconfigure GDS\n" >> $OUTDIR/gds_start.log
  gdssysadm -fd -p                               # deactivate GDS
  gdssysadm -fc -m2 -d11 -p                      # delete dit 11 
  gdssysadm -fc -m2 -d12 -p                      # delete dit 12 
  gdssysadm -fA                                  # activate the GDS
}

reg_clean;
local_clean;
