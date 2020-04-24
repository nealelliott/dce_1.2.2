# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: simple09.rs,v $
# Revision 1.1.8.1  1996/10/17  18:34:31  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:57:09  damon]
#
# Revision 1.1.3.1  1994/07/13  22:32:15  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:15:45  devsrc]
# 
# 	moving files from src/file/episode to test/file
# 	[1994/03/22  20:40:39  delgado]
# 
# 	embedded copyright notice
# 	[1993/01/19  13:53:59  cjd]
# 
# 	New file
# 	[1992/11/17  20:58:48  jaffe]
# 
# $EndLog$

mkdir foo1
mkdir 111 foo2
mkdir bar1 bar2 bar3
mkdirs 5 how
rmdir foo1
rmdir bar3 foo2 bar1
rmdirs 3 how
