# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: simple07.rs,v $
# Revision 1.1.8.1  1996/10/17  18:34:28  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:57:07  damon]
#
# Revision 1.1.3.1  1994/07/13  22:32:11  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:15:43  devsrc]
# 
# 	moving files from src/file/episode to test/file
# 	[1994/03/22  20:40:37  delgado]
# 
# 	embedded copyright notice
# 	[1993/01/19  13:53:51  cjd]
# 
# 	New file
# 	[1992/11/17  20:55:45  jaffe]
# 
# $EndLog$

create foo1
create bar1 bar2 bar3
createfiles 5 foobar
chmod 0111 foo1
chmod 0222 bar1 bar2 bar3
chmodpaths 5 0333 foobar
