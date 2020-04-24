/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: inreal.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:32  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:44  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:45  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:43  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:23:57  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:53:11  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:31:39  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:59:35  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  16:35:03  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:52:05  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  16:55:58  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  14:43:26  marrek]
 * 
 * Revision 1.1.4.2  1992/10/12  21:06:15  sommerfeld
 * 	Change _HPUX_SOURCE to __hpux in #ifdef
 * 	[1992/10/05  16:13:00  sommerfeld]
 * 
 * 	Flush declarations of gcvt and strtod on HPUX.
 * 	[1992/09/30  21:26:04  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/06/01  20:17:20  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:18:25  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: inreal.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:41 $";
#endif

 
/* ****************************************************************** */
/*                                                                    */
/*   COPYRIGHT  (C)  1990 BY SIEMENS NIXDORF INFORMATIONSSYSTEME AG   */
/*                           AND INRIA                                */
/*   All rights reserved                                              */
/*                                                                    */
/*   This software is furnished under licence and may be used only    */
/*   in accordance with the terms of that licence and with the        */
/*   inclusion of the above copyright notice.                         */
/*   This software may not be provided or otherwise made available    */
/*   to, or used by, any other person. No title to or ownership of    */
/*   the software is hereby  transferred.                             */
/*                                                                    */
/* ****************************************************************** */

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <dce/asn1.h>


/*************************************************************************\
* 									  *
* Input and output functions for real are kept in a separate library, so  *
* that the loading with the math library is not mandatory in the general  *
* case.									  *
* 									  *
\*************************************************************************/

char * asn1_real_out(asn1z,d)
char * asn1z;
double d;
{
        return(asn1z +strlen(gcvt(d,12,asn1z)));
}
/*ARGSUSED*/
char * asn1_real_in(asn1z,asn1zm,x)
char * asn1z, * asn1zm;
double *x;
{	char * asn1z2;

	*x = strtod(asn1z,&asn1z2);
	while (asn1z2 > asn1z && !isdigit(*asn1z2))
		asn1z2--;
	return(asn1z2 +1);
}

double asn1_real_v_in(asn1z,asn1zm,asn1z2)
char * asn1z, * asn1zm, ** asn1z2;
{	double x;

	*asn1z2 = asn1_real_in(asn1z,asn1zm,&x);
	return(x);
}
