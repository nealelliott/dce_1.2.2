# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: simple06.rs,v $
# Revision 1.1.11.1  1996/10/02  17:26:58  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:33:38  damon]
#
# Revision 1.1.6.1  1994/06/09  14:02:42  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:37:43  annie]
# 
# Revision 1.1.2.3  1993/01/19  15:32:06  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:53:47  cjd]
# 
# Revision 1.1.2.2  1992/11/18  19:20:48  jaffe
# 	New file
# 	[1992/11/17  20:54:06  jaffe]
# 
# $EndLog$

mkdir foo1
mkdir 0775 foo2
mkdir bar1 bar2 bar3
mkdir 0364 tar1 tar2 tar3
mkdirs 5 how
mkdirs 5 0654 now
chmod 111 foo1
chmod 222 bar1 bar2 bar3
chmodpaths 5 333 how
