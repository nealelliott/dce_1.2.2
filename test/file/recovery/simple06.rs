# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: simple06.rs,v $
# Revision 1.1.8.1  1996/10/17  18:34:27  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:57:06  damon]
#
# Revision 1.1.3.1  1994/07/13  22:32:09  devsrc
# 	moving files from src/file/episode to test/file
# 	[1994/03/22  20:40:35  delgado]
# 
# 	embedded copyright notice
# 	[1993/01/19  13:53:47  cjd]
# 
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
