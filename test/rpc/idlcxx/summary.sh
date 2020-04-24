#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: summary.sh,v $
# Revision 1.1.2.1  1996/09/09  16:30:25  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  16:20:11  bfc]
#
# $EndLog$
#
awk -f filter tet_xres
