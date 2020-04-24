#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.409.1  1996/10/17  18:20:42  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:50:58  damon]
#
# Revision 1.1.404.3  1994/02/07  16:28:10  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:24  mckeen]
# 
# Revision 1.1.404.2  1994/02/04  20:41:00  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:22:38  devsrc]
# 
# Revision 1.1.402.1  1993/12/07  17:42:02  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  18:55:25  jaffe]
# 
# Revision 1.1.2.3  1993/01/11  18:27:20  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:01:42  htf]
# 
# Revision 1.1.2.2  1992/11/09  21:39:57  jaffe
# 	Transarc delta: jaffe-ot5942-make-test-tree-work 1.1
# 	  Selected comments:
# 	    Now that the test tree installs in dcetest/dce1.0, this delta
# 	    cleans up the Transarc files so that there are no extraneous additions
# 	    to the export tree, and so that all appropriate files are installed.
# 	    Add Makefile to drop appropriate files.
# 	[1992/11/09  19:03:31  jaffe]
# 
# $EndLog$
#

DATAFILES = all_tests.itl   clone.itl     clone00.itl   clone01.itl \
	    clone02.itl     clone03.itl   clone04.itl   create00.itl \
	    reclone.itl     reclone00.itl defaults.itl  destroy.itl \
	    destroy00.itl   destroy01.itl destroy02.itl destroy03.itl \
	    lock.itl	    lock00.itl \
            lock00_helper1.itl            lock00_helper2.itl \
            lock00_helper3.itl            lock01.itl \
	    reclone07.itl   reclone08.itl reclone01.itl reclone02.itl \
	    reclone03.itl   reclone04.itl reclone05.itl reclone06.itl \
	    setstatus00.itl setup.itl	  shutdown.itl \
	    clone_error.itl clone_crash.itl		destroy_error.itl \
	    reclone_error.itl		  bomb.itl

ILIST   = ${DATAFILES}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
