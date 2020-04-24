/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxliz.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:22  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:46  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:48:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:24  root]
 * 
 * Revision 1.1.2.3  1994/07/06  15:08:32  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:22:11  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:25  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:42  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:27  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:51:16  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmxliz.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:22 $";
#endif

/*
 * COPYRIGHT (C) 1987 BY SIEMENS AG  All rights reserved
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1992
 * 		 All Rights Reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */

#ifdef THREADSAFE
# include <pthread.h>
#endif

#include "cmx.h"

/*
static char sccsid[] = "@(#)cmxliz.c	311.3 92/10/28 CMX (NC1)" ;
static char lzstr[] = "@(#)!0#0@0%0&0@(#)" ;
*/
int	 t_cmxvers = T_LIBV40 ;
