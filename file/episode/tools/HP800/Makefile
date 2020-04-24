# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.9.1  1996/10/02  17:28:48  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:34:39  damon]
#
# Revision 1.1.4.1  1994/06/09  14:03:43  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:38:39  annie]
# 
# Revision 1.1.2.3  1993/01/21  18:24:06  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  14:14:13  zeliff]
# 
# Revision 1.1.2.2  1992/08/31  20:20:55  jaffe
# 	Transarc delta: mason-add-hp800-osi-routines 1.15
# 	  Selected comments:
# 	    This delta is misnamed - these are really porting fixes.
# 	    Makefile for the lwp assembly code.
# 	    Need to checkpoint to fix a different bug.
# 	    Checkpoint the first system where test_anode works.  Only one change is not
# 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
# 	    is -lBSD.  This should be fixed more generally.
# 	    Added lwputil.o to OFILES line
# 	    More HP-UX changes.
# 	    More HP-UX related changes.
# 	    Fix compile problem on the RIOS.
# 	    Fix more compilation problems.
# 	    Checkpoint.
# 	    Checpoint.
# 	    In this snap we turned all malloc() calls into osi_Alloc calls.
# 	    Also turned free calls into osi_Free.
# 	    HP-UX EOF detection fix.
# 	    Another checkpoint.
# 	    This is a checkpoint of the first working HP-UX Episode system.
# 	    Needed to add the vnops routines specific to HP-UX.
# 	    Fix compilation problems.
# 	[1992/08/30  02:52:35  jaffe]
# 
# $EndLog$
VPATH			= ${TARGET_MACHINE}
liblwp.a_OFILES		+= process.o lwputil.o
