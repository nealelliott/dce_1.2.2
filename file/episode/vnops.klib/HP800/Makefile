#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.9.1  1996/10/02  17:42:46  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:35:10  damon]
#
# Revision 1.1.4.1  1994/06/09  14:04:34  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:39:17  annie]
# 
# Revision 1.1.2.4  1993/01/21  18:25:20  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  16:46:36  zeliff]
# 
# Revision 1.1.2.3  1992/09/25  16:28:37  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:51:47  jaffe]
# 
# Revision 1.1.2.2  1992/08/31  20:49:18  jaffe
# 	Transarc delta: mason-add-hp800-osi-routines 1.15
# 	  Selected comments:
# 	    This delta is misnamed - these are really porting fixes.
# 	    Need to checkpoint to fix a different bug.
# 	    Checkpoint the first system where test_anode works.  Only one change is not
# 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
# 	    is -lBSD.  This should be fixed more generally.
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
# 	    HP changes.
# 	    Needed to add the vnops routines specific to HP-UX.
# 	    Fix compilation problems.
# 	[1992/08/30  03:04:24  jaffe]
# 
# Revision 1.1.1.2  1992/08/30  03:04:24  jaffe
# 	Transarc delta: mason-add-hp800-osi-routines 1.15
# 	  Selected comments:
# 	    This delta is misnamed - these are really porting fixes.
# 	    Need to checkpoint to fix a different bug.
# 	    Checkpoint the first system where test_anode works.  Only one change is not
# 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
# 	    is -lBSD.  This should be fixed more generally.
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
# 	    HP changes.
# 	    Needed to add the vnops routines specific to HP-UX.
# 	    Fix compilation problems.
# 
# 	$TALog: Makefile,v $
# 	Revision 1.3  1994/11/01  21:32:54  cfe
# 	Bring over the changes that the OSF made in going from their DCE 1.0.3
# 	release to their DCE 1.1 release.
# 	[from r1.2 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
# 
# 	Revision 1.1  1992/06/21  15:09:44  mason
# 	This is a checkpoint of the first working HP-UX Episode system.
# 
# 	HP changes.
# 	[added by delta mason-add-hp800-osi-routines, revision 1.13]
# 
# $EndLog$

CFLAGS += -DMOUNT_EFS=5
libkefsops.a_OFILES += efs_mount.o efs_hpux_subr.o
