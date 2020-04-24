/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stbinpak.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:28  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:49  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:05:05  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:04:46  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:52  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:02  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:29:50  keutel
 * 	creation
 * 	[1994/03/21  13:15:36  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stbinpak.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:40 $" } ;
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
static char *sccsid = " @(#) stbinpak.c 1.1 93/11/29  ST_MAVROS";
/***************************************************************************\
* 									    *
* In ASN-1, a bit string can be encoded following either the                *
* ``primitive'' format, i.e. as a string of bytes, or following the	    *
* ``structured'' format, i.e. as a sequence of strings. The latter 	    *
* case is hard to use for ``C'' programs. Hence, we will write some	    *
* procedures to ``pack'' all the substring in a single one. We don't 	    *
* want to perform memory allocation, thus we must reuse the existing	    *
* store. For that, we will start by the last substring, and we will         *
* concatenate in front of it the preceeding substrings, resulting	    *
* in a single string plus some unused bytes. Then, we will try to	    *
* restore some ASN-1 signification to the preceeding bytes; they will	    *
* be filled with the new length field, and possibly with some		    *
* null substrings.							    *
* 									    *
\***************************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

/**************************************************************************\
* 									   *
* asn1st_bitpak(hdl,endpos,target,freebits)                                *
* Moves the content of the ASN-1 string whose LENGTH is pointed		   *
* by the actual r/w position into a continuous field terminated by <target>*
* 									   *
* Note that the buffer limit <endpos> is never tested. It is supposed that *
* the procedure `strbit' provided thorough checking...                     *
* 									   *
\**************************************************************************/

long PASCAL asn1st_bitpak(
STOREHANDLE hdl,
long endpos,
long target,
int FAR *freebits)
{
  int i,j,k,byte;
  signed short int s, fbiterr=0;
  long posz0, posz2, posz3, wh;

  k = (int)asn1st_number(hdl,endpos);                /* number of subtypes */
  asn1st_length(hdl,endpos,&posz2);             /* get the length in posz2 */
  posz0 = ro0076_stwhere(hdl);                            /* here i am now */
  for (i=0; i<k; i++)   /* for each component on the first construct-level */
      {
       ro0066_storepos(hdl,posz0);                /* move to the beginning */
       for (j=1; j< k-i; j++)             /* the unpacking works backwards */
	    asn1st_skip(hdl,endpos);        /* skip over the single octets */
       s =(ro0077_stgetcp(hdl) & 32);          /* read the constructed bit */
       asn1st_skiptype(hdl,endpos);               /* skip over the T-field */
       if (s)                                         /* if subconstructed */
							      /* recursing */
	   target = asn1st_bitpak(hdl,endpos,target,freebits);
       else                                          /* not subconstructed */
	  {
	   asn1st_length(hdl,endpos,&posz2);             /* get the length */
	   posz3 = posz2;
	   if (*freebits == -1)     /* the freebits-byte may differ from 0 */
	      {                         /* only in the very last component */
	       *freebits=(int) ro0070_stgetc(hdl); /* read the content now */
	       if (*freebits > 7)                        /* encoding error */
		   fbiterr = 1;               /* signal for freebits-error */
	      }
	   else
	      {
	       byte = ro0070_stgetc(hdl);      /* only read to check for 0 */
	       if (byte != 0)                      /* ASN1 encoding error! */
		   fbiterr = 1;               /* signal for freebits-error */
	      }
	   wh = ro0076_stwhere(hdl);                   /* store where i am */
	   if (fbiterr)         /* if there was an error with the freebits */
	       return (asn1st_errdecset(hdl,ASN1_ERR_IBTS));
	   if (posz3 == target)                   /* check if last in line */
	       target = wh;                                 /* store where */
	   else                /* copy the values to the end of the string */
	      {
	       ro0081_stmove(hdl,target,posz3,posz3-wh,posz0);
	       target = target - posz3 + wh;
	      }
	  }
       }
  return (target);                                /* last storing position */
}
/***************************************************************************\
* 									    *
* asn1st_stroct(hdl,endpos,x,l,m,freebits)                                  *
* Pack a structured asn1 string.					    *
*     hdl= Store where the String lies (r/w-position at the L-field)        *
*  endpos= the usual buffer limit.                                          *
* 	x= will contain a pointer to the packed string.			    *
* 	l= will contain the length of the packed string.		    *
* 	m= will indicate whether the result is structured or not.	    *
* freebits= will contain the number of unused bits in the last byte.        *
* Returns the 'target' position.                                            *
* 									    *
\***************************************************************************/

long PASCAL asn1st_strbit(
STOREHANDLE hdl,
long endpos,
long FAR *x,
long FAR *l,
int FAR *m,
int FAR *freebits)
{
  long posz2, posz3, target;
  int err;

  posz3 = ro0076_stwhere(hdl);                         /* store where i am */
  if (asn1st_length(hdl,endpos,&posz2) != OK)       /* get the endposition */
      return(posz2);                                   /* fatal ASN1 error */
  target = posz2;                                /* that's the endposition */
  if (target ==0)                                     /* indefinite length */
     {
      target = ro0076_stwhere(hdl);                    /* here i stand now */
      if (target > endpos - 1)                       /* there should be at */
	 {                        /* least one byte left else report error */
	  asn1st_errdecset(hdl,ASN1_ERR_LEN); /* set error in Store + ASN1 */
	  return(target);
	 }
      while (ro0077_stgetcp(hdl))      /* while endposition is not reached */
	 {
	  err = asn1st_skip(hdl,endpos);             /* skip over next one */
	  if (err)  return (target);
	  target = ro0076_stwhere(hdl);                /* the new position */
	  if ((target+2) > endpos)       /* skip error or wrong string end */
	     {                                /* set error in Store + ASN1 */
	      asn1st_errdecset(hdl,ASN1_ERR_LEN);
	      return(posz2);
	     }
	 }
				 /* will be as:    target += 2;      */
      posz2 = target+2;          /*               posz2 = target;      */
     }
  ro0066_storepos(hdl,posz3);             /* move back to where we started */
  *x = asn1st_bitpak(hdl,endpos,target,freebits);
					 /* pack the values to the end and */
				       /* get the starting position of the */
							  /* 'new' V-field */
  *l = target - *x;                                      /* the new length */

   /*----------- NOT INSTALLED IN THE FIRST VERSION -----------------------
	if (*l < 128)
		l1 = 1;
	else	for(i= *l,l1=2;i>256;i=i>>8) l1++;
	if ((i = z2 - z3) < 128)
		l2 = 1;
	else	for(l2=2;i>256;i=i>>8) l2++;
	z = *x;
	if ((z-z3) > (l1 + l2)){
		*m = 1;
		if (*l < 128)
			*(--z) = *l;
		else{	for(i= *l;i<128;i=i>>8) *(--z) = i;
			*(--z) = l1 + 128;
		}
		*(--z) = 4;
		while ((z - z3) > (2 + l2)){
			*(--z) = 0; *(--z) = 4;}
	}else	*m = 0;
	while ((z - z3) > 128){
		*z-- = 0; *z-- = 4;}
	if ((z - z3) == 1)
		*z3 = (z2 - z);
	else{
		*z3 = l2 = (z - z3 -1);
		for(i=z2-z; l2; i=i>>8, l2--) *(--z) = i;
	}
   ----------- NOT INSTALLED IN THE FIRST VERSION -----------------------*/

  return(posz2);
}


int PASCAL asn1st_bitst_dec(
STOREHANDLE hdl,
long endpos,
long FAR *x,
long FAR *l)
{
  int s=1;
  long posz0,oldpos;
  signed short int m;
  int freebits =-1;        /* keeps value 'til first freebits-byte is read */

  m = ro0077_stgetcp(hdl);                          /* store the tag field */
  posz0=ro0076_stwhere(hdl);                           /* store where i am */

  if (asn1st_skiptype(hdl,endpos) == OK)            /* skip to the L-field */
     {
      if (m & 32)                                        /* if constructed */
	 {                                           /* pack subconstructs */
	  oldpos = asn1st_strbit(hdl,endpos,x,l,&s,&freebits);

   /*     if (s == 0)         */               /* if they're not constructed */
   /*        {
	      ro0066_storepos(hdl,posz0); */    /* move to the first T-field */
   /*         ro0067_stputc(hdl,m &~32);  */    /* clear the constructed bit */
   /*        }                            */

	  *l = ((*l)<<3) - freebits;                     /* length in bits */
	  ro0066_storepos(hdl,oldpos);        /* move back where I've been */
	 }
					   /* octet string not constructed */
      else if (asn1st_length(hdl,endpos,&posz0) == OK ) /* get endposition */
	      {
	       *x = ro0076_stwhere(hdl)+1;      /* string starts after the */
	       freebits = ro0070_stgetc(hdl);             /* freebits-byte */
	       if (freebits > 7)
					      /* set error in Store + ASN1 */
		   asn1st_errdecset(hdl,ASN1_ERR_IBTS);  /* wrong freebits */
	       if (posz0 && posz0 < endpos)      /* ckeck for valid length */
		  {                                    /* calculate length */
		   *l = ((posz0 - *x)<<3)- freebits;
		  }
	       else
					      /* set error in Store + ASN1 */
		   asn1st_errdecset(hdl,ASN1_ERR_LDEF);  /* wrong freebits */
	      }
     }
  return (ro0063_sterror(hdl));   /* something's wrong with the BIT STRING */
}                                                  /* if sterror(hdl) <> 0 */

