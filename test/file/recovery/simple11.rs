# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: simple11.rs,v $
# Revision 1.1.8.1  1996/10/17  18:34:33  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:57:11  damon]
#
# Revision 1.1.3.1  1994/07/13  22:32:17  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:15:46  devsrc]
# 
# 	moving files from src/file/episode to test/file
# 	[1994/03/22  20:40:41  delgado]
# 
# 	embedded copyright notice
# 	[1993/01/19  13:54:07  cjd]
# 
# 	New file
# 	[1992/11/17  21:00:57  jaffe]
# 
# $EndLog$

create foo1
create bar1 bar2 bar3
createfiles 5 foobar
chown 3200 foo1
chown 3290 bar1 bar2 bar3
chownpaths 3 3291 foobar
