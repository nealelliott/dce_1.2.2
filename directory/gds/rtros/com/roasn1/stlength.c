/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stlength.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:54  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:08  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:07  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:47:47  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:04  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:47  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:17  keutel
 * 	creation
 * 	[1994/03/21  13:16:19  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stlength.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:04 $" } ;
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
static char *sccsid = " @(#) stlength.c 1.2 93/12/20  ST_MAVROS";
/*********************************************************************\
*                                                                     *
* asn1st_length(hdl,endpos,posz2)                                     *
*                                                                     *
* Decode the asn1 ``length'' field.                                   *
*       hdl    = STOREHANDLE  (r/w position is at the L-field         *
*       endpos = buffer limit                                         *
*    posz2= a return argument. Either the position of the first byte  *
*               following the ``value'' field, or zero if the         *
*               length is undefined.                                  *
* Returns a detected error (only in case of definite length).         *
*    0 means no error.                                                *
*    The following errors are detected:                               *
*                                                                     *
* . ASN1_ERR_TAG        The tag length exceeds the buffer limit.      *
* . ASN1_ERR_LLEN       The length's length exceeds the buffer limit. *
* . ASN1_ERR_LEN        The component size exceeds the buffer limit.  *
*                                                                     *
\*********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <memory.h>
#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int  PASCAL asn1st_length(
STOREHANDLE hdl,
long  endpos,
long FAR *posz2)
{
	int sterr;
	int k;
        long l;
	long inpos;

	inpos = ro0076_stwhere(hdl);

	if ( sterr = ro0063_sterror(hdl))
		return(sterr);

	if (inpos++ >= endpos)              /* set error in Store + Mavros */
	    return(asn1st_errdecset(hdl,ASN1_ERR_TAG));

	if ((l = ro0070_stgetc(hdl)) & 0x80){  /* Constructed or long form */
	     if ((k=(int)(l&0x7F))!=0){ /* k != 0-> long form, l_length stored in k */
		  l = 0;
		  inpos += k;
		  if (inpos > endpos)
					    /* set error in Store + Mavros */
		      return(asn1st_errdecset(hdl,ASN1_ERR_LLEN));
		  while (k--)
			 l = (l<<8) + (unsigned char)ro0070_stgetc(hdl);
	     }else{
		  *posz2 = 0;     /* Constructed form */
		  return(sterr);
	     }
	}
	if ((*posz2 = inpos + l) > endpos) {
					    /* set error in Store + Mavros */
	     return(asn1st_errdecset(hdl,ASN1_ERR_LEN));
	}
	return(ro0063_sterror(hdl));
}
/**************************************************************\
*                                                              *
* asn1st_close(hdl,posz2,limit)                                *
*                                                              *
* Tests that the component is correctly terminated.            *
* Detect the following errors:                                 *
*                                                              *
* . ASN1_ERR_SHORT      The value is smaller than expected.    *
* . ASN1_ERR_BOUND      The EOC is not within the limits       *
* . ASN1_ERR_EOC        The values of the EOC bytes are not 0  *
*                                                              *
\**************************************************************/
int PASCAL asn1st_close(
STOREHANDLE hdl,
long posz2,
long limit)
{
	long inpos;
	int sterr;

	inpos = ro0076_stwhere(hdl);
	sterr = ro0063_sterror(hdl);
	if (sterr) return(sterr);
	if (limit){
		if (inpos != posz2){
					    /* set error in Store + Mavros */
		   return(asn1st_errdecset(hdl,ASN1_ERR_SHORT));
		}
		return(0);
	}else{
		if ((posz2 - inpos) < 2){
					    /* set error in Store + Mavros */
		     return(asn1st_errdecset(hdl,ASN1_ERR_BOUND));
		}
		if (ro0070_stgetc(hdl) || ro0070_stgetc(hdl)){
					    /* set error in Store + Mavros */
		   return(asn1st_errdecset(hdl,ASN1_ERR_EOC));
		}
		return(0);
	}
}

/**************************************************************\
*                                                              *
* asn1st_getmatchz(hdl,poszm,matchz)                           *
*                                                              *
* Reads the next 6 Bytes (maximum length for Tag) into matchz  *
* If rest (= stwhere -poszm) is smaller than 6 only rest bytes *
* read.                                                        *
* After reading, the storeposition is moved to the old         *
* position.                                                    *
*                                                              *
* Returns value of first tagbyte;                              *
*                                                              *
* Detect the following errors:                                 *
*   ASN1_ERR_GETMA  if number of read bytes wrong              *
*                   indicates difference between poszm and     *
*                   real storelength,                          *
*                                                              *
\**************************************************************/
int PASCAL asn1st_getmatchz(
STOREHANDLE hdl,
long poszm,
unsigned char FAR * matchz)
{
    signed long int wh,rest;
    int i;
     /*   ASN1_ZERO(matchz,6);     */
	for(i=0 ;i < 6 ; i++)
	    matchz[i] = 0;
	wh = ro0076_stwhere(hdl);

	/* calculate rest length of ASN.1 string */
	rest = poszm - wh;
	if (rest > 0){
	    if (rest > 6) rest = 6;

	    /* read tag into matchz field */
	    if ( ro0071_stgetm(hdl, (char FAR *) matchz,rest) != rest){
		 asn1st_errdecset(hdl,ASN1_ERR_GETMA);
		 matchz[0] = 0;
		 return(0);
	    }
	    /* move to old position */
	    ro0066_storepos(hdl,wh);
	}

	return ( (int) matchz[0]);
}

/**************************************************************\
*                                                              *
* asn1st_checksetnum(hdl,comp,asn1set)                         *
*                                                              *
* STOREHANDLE hdl                                              *
* int  comp      : number of components                        *
* int  * asn1set : field for number of received elements       *
*                  of every component.                         *
* Checks if more than one element of every component is        *
* received.                                                    *
*                                                              *
* Returns ro0063_sterror                                       *
*                                                              *
* Detect the following errors:                                 *
*  ASN1_ERR_SETNUM  set if more than one element of one        *
*                   component is received.                     *
*                                                              *
\**************************************************************/

int PASCAL  asn1st_checksetnum(
STOREHANDLE hdl,
int comp,
int FAR * asn1set)
{
    int i;
    for (i=0; i<comp; i++)
	if (asn1set[i] > 1) {
	    return(asn1st_errdecset(hdl,ASN1_ERR_SETNUM));
	}
    return(ro0063_sterror(hdl));
}
