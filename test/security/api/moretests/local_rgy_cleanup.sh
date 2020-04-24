#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: local_rgy_cleanup.sh,v $
# Revision 1.1.2.2  1996/03/09  20:50:27  marty
# 	Add OSF copyright
# 	[1996/03/09  20:28:34  marty]
#
# Revision 1.1.2.1  1995/12/11  21:48:42  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:23  root]
# 
# $EndLog$

# 
# Script to clean up after local_rgy.
#

cat > /tmp/rgy_edit.script << !
site -u
do p
del flintstone
del a
del abcdefghijklmnopqrstuvwxyz
del mishkin
del pato
del dineen
del wyant
del pcl
del emartin
del nacey
del dale
del markar
del greg
del burati
del ahop
del sommerfeld
del tbl
del frisco
del mk
del cuti
del roc
del kumar
del kathy
del kumar_k
del gream
del marcus_s
quit
!

/opt/dcelocal/bin/rgy_edit </tmp/rgy_edit.script
/bin/rm /tmp/rgy_edit.script

EXIT=$?
exit $EXIT

