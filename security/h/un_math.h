/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: un_math.h,v $
 * Revision 1.1.4.2  1996/02/18  23:01:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:02  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:30:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:35  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:28:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:16:23  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  un_math.h V=2 04/30/91 //nbs/dds_tools/src
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      prototypes for unix math library routines
** 
*/

#ifndef un_math_h__included
#define un_math_h__included

double atof ( 
#ifdef __STDC__
    char *nptr
#endif 
);

int atoi (
#ifdef __STDC__
    char *nptr
#endif
);

long atol (
#ifdef __STDC__
   char *nptr
#endif
);

#endif
