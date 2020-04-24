/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stcmp.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:33  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:52  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:15:18  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:03:21  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:54  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:11  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:29:57  keutel
 * 	creation
 * 	[1994/03/21  13:15:44  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stcmp.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:44 $" } ;
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
static char *sccsid = " @(#) stcmp.c 1.1 93/11/29  ST_MAVROS";
/**************************************************************************\
*                                                                          *
* Comparaison and hashing procedures for the elementary data types.        *
* Note that the coresponding procedure for REAL are in the file            *
* "real.c", and that the file "weight.c" should contain the corresponding  *
* procedures for case dependant strings, and other substrings.             *
*                                                                          *
\**************************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <memory.h>

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int PASCAL asn1_int_cmp (
long a,
long b)
{
	return((a == b)?0:((a<b)?-1:1));
}

int PASCAL asn1_octets_cmp (
register asn1 v1,
register long l1,
register asn1 v2,
register long l2)
{       long i, l;

	if ((l = l1) > l2) l = l2;
	for (i=0; i<l; i++){
		if (v1[i] == v2[i]) continue;
		return ((v1[i] < v2[i])? -1:1);
	}
	return ((l1 == l2)?0:((l1 < l2)? -1:1));
}

int PASCAL asn1_boolean_cmp (
register int a,
register int b)
{
	return((a)?((b)?0:1):((b)?-1:0));
}

/************************************************************\
*                                                            *
* asn1_bits_cmp                                              *
*                                                            *
* Compares two `L,V' fields, where L is the number of BITS.  *
* Returns 0 if the two fields are identical, -1 otherwise.   *
*                                                            *
\************************************************************/

int PASCAL asn1_bits_cmp (
asn1 v1,
int l1,
asn1 v2,
int  l2)
{       register m,r,l;

	if (l1 != l2)
		return (-1);
	l = (l1>>3);
	if (l && ASN1_CMP(v1,v2,l))
		return(-1);
	if (r=(l1 - (l<<3))){
		r = 8-r; m=0;
		while (r--) {m = m<<1; m |= 1;}
		return(((v1[l]|m) != (v2[l]|m))?-1:0);
	}else   return(0);
}
