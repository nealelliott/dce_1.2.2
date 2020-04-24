# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.10.1  1996/10/02  16:57:33  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:03:34  damon]
#
# Revision 1.1.4.1  1994/06/09  13:49:52  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:23:20  annie]
# 
# Revision 1.1.2.3  1993/01/18  19:49:38  cjd
# 	Embedded copyright notice
# 	[1993/01/18  19:23:02  cjd]
# 
# Revision 1.1.2.2  1992/11/18  17:37:06  jaffe
# 	Transarc delta: jdp-ot5415-mount-aggregate-on-attach 1.4
# 	  Selected comments:
# 	    This delta contains the bulk of the changes necessary to have aggregates
# 	    mounted at attach time.  The purpose of this feature is to provide a
# 	    means by which exported filesets (with no local mount points) will be
# 	    sync'd periodically.  To accomplish this, we have created a new VFS type
# 	    (agfs).  Whenever an aggregate is exported, a local mount of the aggregate
# 	    is also done; then, when the sync daemon does its thing, the aggregate's sync
# 	    vfsop will be called.  The agfs sync then calls the new sync aggrop, which
# 	    in the case of Episode, calls efs_sync().  The UFS sync aggrop doen't do
# 	    anything, since we know the UFS filesystem is mounted, and so, its sync vfsop
# 	    will get called anyway.
# 	    This delta must be bundled with the corresponding deltas in the Episode and
# 	    test configurations.
# 	    This is the makefile for the AIX-specific agfs code.
# 	    Remove leftover junk from $OFILES.
# 	    Rearrange things slightly to make use of a vendor-supplied definition
# 	    of MOUNT_AGFS.
# 	    Fix bogus linker warnings about shared libraries when compiling .o files.
# 	[1992/11/17  19:29:41  jaffe]
# 
# $EndLog$
#

libagfs.a_OFILES	+= agfs_aixmount.o
