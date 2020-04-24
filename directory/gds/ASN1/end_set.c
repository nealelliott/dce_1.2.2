/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: end_set.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:24  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:31  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:39  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:28  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:22:43  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:52:17  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:26:33  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:58:35  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  16:34:29  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:44  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  16:55:54  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  14:43:06  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:16:21  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:17:57  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: end_set.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:34 $";
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

/***************************************************************************\
* 									    *
* End set:								    *
* 									    *
* The X.500 authentication procedures are based on a ``unique ASN-1 BER	    *
* representation'', which is obtained by coding all boolean as an `FF'	    *
* octet, coding all length fields in the ``defined'' format (as in	    *
* ``close_d''), and also coding the SET and SET OF components in ``natural  *
* order'', i.e. such that the lowest components appear first.		    *
* 									    *
* The ``end set'' procedure applies this recommendation, by sorting the	    *
* elements of a sort. As the number of set elements is likely to be small,  *
* and as there is also a reasonable chance that the results from decoding   *
* and reencoding would be orderer, we did not bother doing better than a    *
* bubble sort.								    *
* 									    *
\***************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

struct set_element{
	asn1 start;
	int len;};

static int element_compare (struct set_element,struct set_element);

static int element_compare(a,b)
struct set_element a, b;
{	register l;
	register asn1 za = a.start, zb = b.start;

	l = (a.len < b.len)? a.len: b.len;

	while (l--){
		if (*za < *zb) return(0);
		if (*zb++ < *za++) return(-1);
	}
	return ((a.len == b.len)?0:-1);
}

asn1 asn1_unique_set(z1, z0)
asn1 z0, z1;
{	asn1 zs, z, zm;
	int l, n, i, test;
	struct set_element * x, y;
#ifdef DEBUG_END_SET
	asn1 z00 = z0-1;

	printf("Before asn1_set_end:\n");
	*z0 = 0x80;
	z1[0] = 0;
	z1[1] = 0;
	asn1_dump(stdout,z00,z1+2);
#endif
	z0++;
	if ((l = (z1 - z0))==0){
		*(z0 -1) = 0; return(z1);}
	zs = (asn1) malloc((unsigned)l);
	ASN1_MOVE(z0,zs,l);
	x = (struct set_element *)
	malloc((unsigned)128 * sizeof(struct set_element));

	z = zs; zm = zs + l; n = 0;

	while (z < zm){
		x[n].start = z;
		x[n].len = (z = asn1_skip(z,zm)) - x[n].start;
		n++;
		if ((n&127) == 0) 
			x = (struct set_element *) realloc((char *)x,
			(unsigned)(n+128)*sizeof(struct set_element));
	}
	for (test = 1; test;){
		test = 0;
		for (i=1; i<n; i++){
			if (element_compare(x[i-1],x[i])){
				y = x[i]; x[i] = x[i-1]; x[i-1] = y;
				test = 1;}
		}
	}
	z = asn1_lencod(z0-1, l);

	for (i =0; i<n; i++){
#ifdef DEBUG_END_SET
		printf("\nComponent %d is:\n", i);
		asn1_dump(stdout,x[i].start,x[i].start+x[i].len);
#endif
		ASN1_MOVE(x[i].start, z, x[i].len);
		z += x[i].len;
	}
	free((char *)x);
	free((char *)zs);
#ifdef DEBUG_END_SET
	asn1_dump(stdout,z00,z);
#endif
	return(z);
}

void asn1_x509_mode_on (void)
{
	asn1_end = asn1_defined_end;
	asn1_end_set = asn1_unique_set;
}
