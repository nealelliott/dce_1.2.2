/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_procutils.c,v $
 * Revision 1.1.11.1  1996/10/02  17:58:40  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:07  damon]
 *
 * Revision 1.1.6.2  1994/06/09  14:15:52  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:15  annie]
 * 
 * Revision 1.1.6.1  1994/02/04  20:25:02  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:50  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:43:49  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:23  annie]
 * 
 * Revision 1.1.2.2  1993/01/13  18:17:39  shl
 * 	Transarc delta: kazar-ot6437-make-init-parent 1.1
 * 	  Selected comments:
 * 	    exiting threads weren't being wait3'd, and turned into zombies
 * 	    new file cntg dummy for osi_MakeInitChild
 * 	[1993/01/12  21:42:52  shl]
 * 
 * $EndLog$
 */
/*
 * Copyright (C) 1992 Transarc Corporation
 * All rights reserved.
 */

osi_MakeInitChild()
{
#ifdef KERNEL
    /* do something here? */
#endif /* KERNEL */
    return 0;
}
