
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: simple01.rs,v $
# Revision 1.1.11.1  1996/10/02  17:26:51  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:33:33  damon]
#
# Revision 1.1.6.1  1994/06/09  14:02:35  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:37:39  annie]
# 
# Revision 1.1.2.3  1993/01/19  15:31:51  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:53:26  cjd]
# 
# Revision 1.1.2.2  1992/11/18  19:19:56  jaffe
# 	Transarc delta: rajesh-ot5841-clean-up-rcx 1.11
# 	  Selected comments:
# 	    The RCX script langauage interface in terms of naming and
# 	    functionality available is not consistent. Need to introduce
# 	    consistency here.
# 	    Also, among the existing RCX scripts, some are essentially duplicates
# 	    of others. Hence remove duplicate scripts and reorganize existing
# 	    scripts.w
# 	    Also change the scripts to use the new interface if necessary.
# 	    In this version of delta remove duplicate scripts.
# 	    First step in reorganising scripts according to complexity of scripts.
# 	    Renamed copy of test04.rs
# 	    Final step (for now) in reorganising scripts according to complexity
# 	    of scripts.
# 	    Introduce consistency into the existing script language commands
# 	    in terms of naming and semantics. A checkpoint.
# 	    Completed and tested the implementation begun in last revision 1.4
# 	    for chmodfiles, rmfiles, rmdirs, mvfiles and mkdir.
# 	    Also developed some input rcx scripts for basic recovery tests, that
# 	    were actually developed for testing the new script language interface.
# 	    Add support for chown, chgrp, symlink, link, truncate, writes. Develop
# 	    additional input simple test scripts. Rename the earlier created "basic"
# 	    test scripts into "simple" scripts.
# 	    Due to change in command argument sequence
# 	    Lots and lots of fixes.
# 	    Changes made while testing on OSF1. Include some generic changes too.
# 	    Minor changes.
# 	    Some additional scripts.
# 	    Cannot mount filesets from the copy aggregate while the source aggregate is
# 	    still frozen as it can lead to a deadlock over the global buffer lock.
# 	    This version is tested and ready for export.
# 	    Includes work that should have been done under delta
# 	    rajesh-ot5837-extend-syscalls-exercised-by-rcx
# 	[1992/11/17  20:38:23  jaffe]
# 
# $EndLog$
createfiles 1000 foo 
