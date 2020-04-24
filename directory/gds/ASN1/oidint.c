/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: oidint.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:53  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:27  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:49:00  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:20  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:28:03  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:55:37  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:47:22  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:02:14  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:36:53  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:13  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:19:45  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:19:51  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: oidint.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:00 $";
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

#include <dce/asn1.h>

int asn1_oid_int(l,v,x)
int l;
unsigned long *x;
asn1 v;
{	int i=0, j=0;

	while (l--){
		j = (j << 7)| (*v & 0x7F);
		if ((*v&0x80)==0){
			if (i == 0){
				switch(j/40){
				case 0:	x[i++] = 0; break;
				case 1:	x[i++] = 1; break;
				default:
					x[i++] = 2; break;}
				j -= 40*x[0];
			}
			x[i++] = j;
			j = 0;
		}
		v++;
	}
	return(i);
}

asn1 asn1_int_oid(v,x,l)
int l;
unsigned long *x;
asn1 v;
{	unsigned long j;
	int i, k, n;

	j = x[0]*40 + x[1]; i = 2;
	for(;;){
		if (j < (unsigned long) (1<<7))
			n = 1;
		else if (j < (unsigned long) (1<<14))
			n = 2;
		else if (j < (unsigned long) (1<<21))
			n = 3;
		else if (j < (unsigned long) (1<<29))
			n = 4;
		else	n = 5;
		for (k = n-1; k>=0; k--){
			v[k] = j&0x7F;
                        j = j>>7;
                     }
		for (k =0; k < n-1; k++)
			v[k] |= 0x80;
		v+=n;
		if (i < l)
			j = x[i++];
		else	break;
	}
  return(v);
}

