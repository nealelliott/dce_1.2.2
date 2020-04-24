/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omAlloc.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:27  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:38:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:41  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:20:42  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:08  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:16  marrek
 * 	Beautification.
 * 	[1994/02/09  15:40:01  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:58:37  marrek]
 * 
 * $EndLog$
 */
#ifndef _OMALLOC_H
#define _OMALLOC_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char omalloc_rcsid[] = "@(#)$RCSfile: omAlloc.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:13 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code	  mmgt_set_trace (void);

char	*mmgt_xcalloc(
	char *file,
	OM_sint	line,
	OM_sint	n,
	OM_sint	size
);
void	mmgt_xfree (
	char *file,
	OM_sint	line,
	char *buffer
);
char	*mmgt_xmalloc(
	char *file,
	OM_sint	line,
	OM_sint	size
);
char	*mmgt_xrealloc (
	char *file,
	OM_sint	line,
	char *old,
	OM_sint	size
);
void
omP_error(
    const char *fmt,
    ...
);
char	*
omP_xalloc_str(
	char *from,
	OM_sint	len);
#endif /* ifndef _OMALLOC_H */
