# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: basictest.rs,v $
# Revision 1.1.8.1  1996/10/17  18:33:55  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:56:50  damon]
#
# Revision 1.1.3.1  1994/07/13  22:31:40  devsrc
# 	moving files from src/file/episode to test/file
# 	[1994/03/22  20:40:17  delgado]
# 
# 	    delta bob-add-rcx 1.1
# 	    [1992/05/04  19:33:50  jdp]
# 	[1992/05/05  18:27:49  jdp]
# 
# Revision 1.1.8.1  1994/02/04  20:14:31  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:10:58  devsrc]
# 
# Revision 1.1.6.1  1994/01/28  20:43:40  annie
# 	expand OSF copyright
# 	[1994/01/28  20:42:15  annie]
# 
# 	expand OSF copyright
# 
# Revision 1.1.4.2  1993/04/14  21:16:32  jaffe
# 	Sync with Transarc Source
# 	[1993/04/14  21:03:39  jaffe]
# 
# $EndLog$

createfiles 300 abcdefg 
mkdirs 20 foo 
mvpaths 20 foo bar 
mkdir zoo
mvpaths 20 bar zoo/bar 
mvpaths 20 zoo/bar this-is-a-long-entry-name 
mvpaths 20 this-is-a-long-entry-name short 
mvpaths 20 short x 
mvpaths 20 x longer 
mkdir soup
mvpaths 20 longer soup/longer 
mvpaths 20 soup/longer zoo/longer 
mvpaths 20 zoo/longer soup/a 
