/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stoidint.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:01  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:15  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:12  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:47:54  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:10  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:47:02  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:23  keutel
 * 	creation
 * 	[1994/03/21  13:16:33  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stoidint.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:13 $" } ;
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

static char *sccsid = " @(#) stoidint.c 1.1 93/11/29  ST_MAVROS";

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int PASCAL asn1st_oiddec (
STOREHANDLE hdl,
long endpos,                                        /* limit of oid string */
unsigned long FAR *x)                          /* table where to decode to */
{       unsigned long int i=1, j=0;
	unsigned char byte;
	long l;
	int sterr;

	sterr = asn1st_lendec(hdl,endpos,&l);  /* get length of oid string */
	if (sterr) return sterr;
	while (l--){
		byte = (unsigned char)ro0070_stgetc(hdl); /* read oid value */
		j = (j << 7) | (byte & 0x7F);
		if ((byte & 0x80)==0) {
		     if (i == 1) {            /* first contains oid1 +oid2 */
			 switch(j/40)                    /* calculate oid1 */
			  { case 0: x[i++] = 0; break;
			    case 1: x[i++] = 1; break;
			    default:x[i++] = 2; break;
			  }
			 j -= 40*x[1];                   /* calculate oid2 */
		     }
		     x[i++] = j;
		     j = 0;
		     if ( i - 1 > RCro00_MaxObjIdComp)
			  return(asn1st_errdecset(hdl,ASN1_ERR_SOFOID));
		}
	}
	if (byte & 0x80) asn1st_errdecset(hdl,ASN1_ERR_OIDLEN);
		    /* first bit of last byte of an OBJECT ID must be zero */
	x[0] = i-1;               /* first table element is number of oids */
	return(ro0063_sterror(hdl));
}

int PASCAL asn1st_oidcod(
STOREHANDLE hdl,
unsigned long FAR *x)                   /* oid table from where to encode */
{       unsigned long j;
	int sterr,i,k,n,l;
	char v[200];             /* intermediate buffer for max 17 entries */
	char FAR *vptr; 
	long offset;

	vptr = v;
	l= (int)x[0];                                    /* number of oids */
	if ( l > RCro00_MaxObjIdComp)
	    return(asn1st_errdecset(hdl,ASN1_ERR_SOFOID));
	if (x[1] > 2 || x[1] < 2 && x[2] > 39)
	    return(asn1st_errdecset(hdl,ASN1_OIDSECBIG));

	j = x[1]*40 + x[2];                 /* oid1 + oid2 coded in 1 byte */
	i = 3;                                       /* start with 3rd oid */
	for(;;){
		if (j < (unsigned long) (1<<7))
			n = 1;
		else if (j < (unsigned long) (1<<14))
			n = 2;
		else if (j < (unsigned long) (1<<21))
			n = 3;
		else if (j < (unsigned long) (1<<29))
			n = 4;
		else    n = 5;
		for (k = n-1; k>=0; k--){
			vptr[k] = (char)(j&0x7F);
			j = j>>7;
		}
		for (k =0; k < n-1; k++)
			vptr[k] |= 0x80;
		vptr+=n;
		if (i <= l)
		    j = x[i++];
		else
		    break;
	}
	offset = (long)(vptr - (char FAR *)v);
	asn1st_lencod(hdl,offset);                /* encode length */
	sterr = ro0068_stputm(hdl,v,offset); /* copy buffer to store */
	return(sterr);
}
