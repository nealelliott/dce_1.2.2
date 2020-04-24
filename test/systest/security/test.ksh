# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: test.ksh,v $
# Revision 1.1.2.2  1996/03/09  20:52:31  marty
# 	Add OSF copyright
# 	[1996/03/09  20:30:05  marty]
#
# Revision 1.1.2.1  1995/12/11  22:48:10  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:22:48  root]
# 
# $EndLog$

#!/bin/ksh
cd /tmp/security/dceseact
/opt/dcelocal/bin/klist
kdestroy
if [ -f ./core ];then
    echo "core file found: moved to core$$"
    mv ./core ./core$$
fi
if [ -f ./cma_dump.log ];then
    echo "See cma_dump.log$ITERATION$$"
    mv ./cma_dump.log ./cma_dump.log$ITERATION$$
fi
exit
