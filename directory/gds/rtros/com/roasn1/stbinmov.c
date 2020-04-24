/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stbinmov.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:27  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:47  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:02:25  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:02:07  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:51  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:45:58  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:29:48  keutel
 * 	creation
 * 	[1994/03/21  13:15:33  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stbinmov.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:37 $" } ;
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
static char *sccsid = " @(#) stbinmov.c 1.1 93/11/29  ST_MAVROS";
/******************************************************************\
*                                                                  *
* asn1st_binmov(hdl,endpos,s,l,freebits)                           *
*                                                                  *
* Move an asn1 encoded string into a character string.             *
*     hdl= Handle to the Store                                     *
*  endpos= the end of the ASN1-string in the store.                *
*       s= points to the target character string.                  *
*       l= will contain the number of bits in the string ``s''.    *
*    smax= length of target string                                 *
* freebits = number of unused bits in last byte                    *
*         ( has to be set 0 outside before calling this function   *
*           for the first time!! )                                 *
*                                                                  *
* Returns ERROR value .                                            *
* Note that the string ``s'' is NOT null terminated.               *
*                                                                  *
\******************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int PASCAL asn1st_binmov(
STOREHANDLE hdl,
long endpos,
char FAR * s,
long FAR * l,
long smax,
unsigned short int FAR * freebits)
{
	long lz=0,fb,wh;
	long posz2;
	int m,sterr;
	signed short int tag;
	*l = 0;
	if (*freebits)
	   {                                   /* set ASN1 error and leave */
	    return(asn1st_errdecset(hdl,ASN1_ERR_IBTS));
	   }
	m = (ro0077_stgetcp(hdl) & 32);        /* read the constructed bit */
	if ((sterr = asn1st_skiptype(hdl,endpos)) != 0 )
	    return(sterr);
	if ((sterr = asn1st_length(hdl,endpos,(long FAR *)&posz2)) != 0) /* get endposition */
	    return (sterr);                                  /* in posz2 */
	if (m == 0 )                                    /* not constructed */
	   {if (posz2 == 0 || posz2 > endpos)   /* not constr + indefinite */
		return(asn1st_errdecset(hdl,ASN1_ERR_LDEF));
	    wh = ro0076_stwhere(hdl);
	    *l = posz2 -1 -wh;
		   /* *l length of BIT STRING value (= encoded length - 1) */

	    if (*l > smax)             /* bit-string exceeds target string */
	       {                               /* set ASN1 error and leave */
		return(asn1st_errdecset(hdl,ASN1_ERR_S2SH));
	       }
	    if (*l < 0) /* BIT STRING must have initial octet, no length 0 */
		return(asn1st_errdecset(hdl,ASN1_ERR_BSLEN));

	    *freebits = ro0070_stgetc(hdl);         /* number of free bits */
							   /* in last byte */
	    if (*freebits >=8)
	       {                               /* set ASN1 error and leave */
		return(asn1st_errdecset(hdl,ASN1_ERR_IBTS));
	       }
	    if ( *l > 0 )
		 ro0071_stgetm(hdl,s,*l);            /* copy the bitstring */
	   }
	else                                      /* constructed bitstring */
	   {
	    if (posz2)
		endpos = posz2;
	    while (ro0076_stwhere(hdl) < endpos && (tag=ro0077_stgetcp(hdl))!=0)
	       {
		if (tag != 3  && tag != 35)             /* only bitstrings */
		   {                           /* set ASN1 error and leave */
		    return(asn1st_errdecset(hdl,ASN1_ERR_WRTAG));
		   }
		if ((sterr = asn1st_binmov(hdl,endpos, s+ *l,
				 (long FAR *)&lz,smax - *l,freebits)) != 0)
		    return(sterr);
		fb = (lz & 0x07);       /* remember if there are free bits */
		*l += (lz>>3);                          /* number of bytes */
		if (fb) (*l)++;                     /* add freebits byte */
	       }
	    sterr = asn1st_close(hdl,endpos,posz2);
	   }
	    *l = (*l)*8 - *freebits;       /* restore number of bits again */
	return(sterr);
}

/*
Flags decoding
*/
/*ARGSUSED*/

int PASCAL asn1st_flagdec(
STOREHANDLE hdl,
long endpos,
unsigned long int FAR * f,
long FAR * lf)
{

	int errval;
	unsigned short int freebits;

	freebits = (unsigned short) 0;

	errval = asn1st_binmov(hdl,endpos,(char FAR *) f,(long FAR *)lf,
		    (long)sizeof(long),(unsigned short int FAR *)&freebits);

       asn1st_swapint(f);
       return(errval);
}

int PASCAL asn1st_swapint(
unsigned long int FAR *pVal)
{
    unsigned char FAR *y = (unsigned char FAR *)pVal;
    unsigned char z;

    z = y[1];
    y[1] = y[2];
    y[2] = z;

    z = y[0];
    y[0] = y[3];
    y[3] = z;

    return(1);  /* warning : no return val on DOS */ 
}


/***********************************************************************/
/*                                                                     */
/* int asn1st_flags_len(defval)                                        */
/* unsigned long int defval;     value of the a default FLAG           */
/*                                                                     */
/* FLAG is short BIT STRING ( max 32 bits) with local representation   */
/* of a unsigned long                                                  */
/* The function calculates the length of the FLAG                      */
/* The length is the bit position of the last bit which is not null    */
/*                                                                     */
/***********************************************************************/

int PASCAL asn1st_flags_len(
unsigned long int defval)
{
int i;
unsigned long int map;

    for (i = 0; i < 32; i++){
	map = 1 << i;
	if (map & defval) break;
    }
return (32 - i);
}

