# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: simple07.rs,v $
# Revision 1.1.11.1  1996/10/02  17:26:59  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:33:39  damon]
#
# Revision 1.1.6.1  1994/06/09  14:02:43  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:37:44  annie]
# 
# Revision 1.1.2.3  1993/01/19  15:32:08  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:53:51  cjd]
# 
# Revision 1.1.2.2  1992/11/18  19:20:54  jaffe
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
