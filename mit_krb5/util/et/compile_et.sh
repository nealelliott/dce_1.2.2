#   
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#  
# All Rights Reserved
#    
# HISTORY
# $Log: compile_et.sh,v $
# Revision 1.1.2.1  1996/06/05  21:11:40  arvind
# 	Merge from DCE_1.2
# 	[1996/06/05  21:08:18  arvind]
#
# $EndLog$
#  
#!/bin/sh
#
#
AWK=@AWK@
DIR=@DIR@

ROOT=`echo $1 | sed -e s/.et$//`
BASE=`echo $ROOT | sed -e 's;.*/;;'`

$AWK -f ${DIR}/et_h.awk outfile=${BASE}.h $ROOT.et
$AWK -f ${DIR}/et_c.awk outfile=${BASE}.c $ROOT.et
