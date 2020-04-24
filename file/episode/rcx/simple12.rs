# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: simple12.rs,v $
# Revision 1.1.11.1  1996/10/02  17:27:05  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:33:48  damon]
#
# Revision 1.1.6.1  1994/06/09  14:02:49  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:37:49  annie]
# 
# Revision 1.1.2.3  1993/01/19  15:32:21  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:54:12  cjd]
# 
# Revision 1.1.2.2  1992/11/18  19:30:17  jaffe
# 	New file
# 	[1992/11/17  21:01:38  jaffe]
# 
# $EndLog$

create foo1
create bar1 bar2 bar3
createfiles 5 foobar
chgrp 3200 foo1
chgrp 3290 bar1 bar2 bar3
chgrppaths 3 3291 foobar
