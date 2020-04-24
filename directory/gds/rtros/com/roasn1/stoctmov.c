/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stoctmov.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:57  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:10  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:09  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:47:49  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:05  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:53  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:19  keutel
 * 	creation
 * 	[1994/03/21  13:16:25  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stoctmov.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:07 $" } ;
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
static char *sccsid = " @(#) stoctmov.c 1.1 93/11/29  ST_MAVROS";
/***************************************************************************\
* 									    *
* asn1st_octmov(hdl,endpos,s,l,smax,errmode)                                 *
* 									    *
* Move an asn1 encoded string into a character string.			    *
*    hdl = STORE at the position of the TYPE field of an ASN-1 octet string. *
*    endpos =  end of the ASN1-string in the STORE.                         *  
*    s  = points to the target character string.                            *
*    smax = length of target character string.                              *
*    l :  will contain the number of bytes in the decoded string ``s''.     *
*    errmode  1 : a error is reported if decoded value > smax, decoding is  *
*                 aborted.                                                  *
*             0 : if decoded value > smax the rest of the value is skipped, *
*                 s is filled up to smax. The rerurned value for l is the   *
*                 total length of the decoded value and not smax.           *
*                 No error is reported but can be detected if l > smax.     *
*
*    Return an error value:                                                 *
*       If the target string where the decoded value should be stored is    *
*       to small an error is set if errmode = 1, else the rest of the       *
*       value just skipped.                                                 *
*       All other decoding errors  are reported by a call to ASN1_CKERR     * 
*       and an error value is returned.                                     *
* 									    *
* Note that the string ``s'' is NOT null terminated.			    *
* 									    *
\***************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int PASCAL asn1st_octmov(
STOREHANDLE hdl,
long endpos,
char FAR *s,
long FAR *l,
long smax,
int  errmode)
{
	long posz2;
	long wh;
	long lz = 0;
	signed short int tag;
	int   m,sterr;

	*l = 0;
	m = (ro0077_stgetcp(hdl) & 32);              /* Constructed or not */
	if ( (sterr = asn1st_skiptype(hdl,endpos)) != 0 )
		return(sterr);
	if ( (sterr = asn1st_length(hdl,endpos,&posz2)) != 0 )
		return(sterr);
	if (m)
	   {if (posz2) endpos = posz2;
	    while (ro0076_stwhere(hdl) < endpos && (tag = ro0077_stgetcp(hdl))!=0 )
		{
		 if (tag !=4 && tag != 36)           /* only octet strings */
		   {                           /* set ASN1 error and leave */
							/* wrong tag field */
		    return(asn1st_errdecset(hdl,ASN1_ERR_WRTAG));
		   }
		 if ( (sterr = asn1st_octmov(hdl,endpos,(char FAR *)(s+*l),
			   (long FAR *)&lz,(long)(smax-*l),errmode)) != OK)
			return(sterr);
		 *l += lz;
		}
	    return(asn1st_close(hdl,endpos,posz2));
	}else{
		wh = ro0076_stwhere(hdl);
		if (posz2 == 0)
		    return(asn1st_errdecset(hdl,ASN1_ERR_LDEF));
				      /* Check length of the target string */
		if ((*l = posz2 - wh) > smax) {
					/* Length of targetstring to short */
			if (errmode) {
					       /* set ASN1 error and break */
			    return(asn1st_errdecset(hdl,ASN1_ERR_S2SH));
			}
			if (smax > 0) {
		      /* fill only rest of target string with OCTET STRING */
			    ro0071_stgetm(hdl,s,smax);
			}
					  /* positioning to next tag-field */
			sterr = ro0066_storepos(hdl,posz2);
			return(sterr);
		} else {
			/* copy decoded OCTET STRING to targed string */
			ro0071_stgetm(hdl,s,*l);
			return(ro0063_sterror(hdl));
		}
	}
}

