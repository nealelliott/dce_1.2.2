#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: bomb_cat.sh,v $
# Revision 1.1.20.1  1996/10/02  17:03:34  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:04:40  damon]
#
# Revision 1.1.14.2  1994/06/09  13:51:45  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:25:19  annie]
# 
# Revision 1.1.14.1  1994/02/04  20:06:00  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:06:31  devsrc]
# 
# Revision 1.1.12.1  1993/12/07  17:12:43  jaffe
# 	New File from Transarc 1.0.3a
# 	[1993/12/02  20:05:03  jaffe]
# 
# $EndLog$

# Copyright (C) 1993 Transarc Corporation - All rights reserved
#
# $Header: /u0/rcs_trees/dce/rcs/file/bomb/bomb_cat.sh,v 1.1.20.1 1996/10/02 17:03:34 damon Exp $
#

(cat <<'EOF'				# Quote EOF so sh doesn't process input
changecom
define(BOMB_EXEC, $2)
define(BOMB_IF, if (0 /* Never execute */))
define(BOMB_ELSE, else if (0 /* Never execute */))
define(BOMB_POINT)
define(BOMB_RETURN)
define(BOMB_SET)
define(BOMB_SET_DESC)
define(BOMB_UNSET)
EOF

cat "$@") | m4 | tail +10		# Tail arg is number of m4 lines + 1
