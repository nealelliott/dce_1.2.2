/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stoctpak.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:12  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:10  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:47:50  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:06  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:56  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:20  keutel
 * 	creation
 * 	[1994/03/21  13:16:30  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stoctpak.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:08 $" } ;
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
static char *sccsid = " @(#) stoctpak.c 1.1 93/11/29  ST_MAVROS";
/***************************************************************************\
*                                                                           *
* In ASN-1, an octet string can be encoded following either the             *
* ``primitive'' format, i.e. as a string of bytes, or following the         *
* ``structured'' format, i.e. as a sequence of strings. The latter          *
* case is hard to use for ``C'' programs. Hence, we will write some         *
* procedures to ``pack'' all the substring in a single one. We don't        *
* want to perform memory allocation, thus we must reuse the existing        *
* fields. For that, we will start by the last substring, and we will        *
* concatenate in front of it the preceeding substrings, resulting           *
* in a single string plus some unused bytes. Then, we will try to           *
* restore some ASN-1 signification to the preceeding bytes; they will       *
* be filled with the new length field, and possibly with some               *
* null substrings.                                                          *
*                                                                           *
\***************************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

/**************************************************************************\
*                                                                          *
* asn1_octpak(hdl,endpos,target)                                           *
* Moves the content of the ASN-1 string whose LENGTH is pointed            *
* by the actual r/w position into a continuous field terminated by <target>*
*                                                                          *
* Note that the buffer limit <endpos> is never tested. It is supposed that *
* the procedure `stroct' provided thorough checking...                     *
*                                                                          *
\**************************************************************************/

long PASCAL  asn1st_octpak(
STOREHANDLE hdl,
long endpos,
long target)
{
  int i,j,k;
  signed short int s;
  long posz0, posz2, posz3, wh;

  k = (int)asn1st_number(hdl,endpos);                /* number of subtypes */
  asn1st_length(hdl,endpos,&posz2);        /* get the endposition in posz2 */
  posz0 = ro0076_stwhere(hdl);                            /* here i am now */
  for (i=0; i<k; i++)   /* for each component on the first construct-level */
      {
       ro0066_storepos(hdl,posz0);      /* skipping starts always from the */
							  /* same position */
       for (j=1; j< k-i; j++)             /* the unpacking works backwards */
	    asn1st_skip(hdl,endpos);       /* skip over the single strings */
       s =(ro0077_stgetcp(hdl) & 32);          /* read the constructed bit */
       asn1st_skiptype(hdl,endpos);               /* skip over the T-field */
       if (s)                                         /* if subconstructed */
				/* move into the subconstructs recursively */
	   target = asn1st_octpak(hdl,endpos,target);
       else                                          /* not subconstructed */
	  {
	   asn1st_length(hdl,endpos,&posz2);             /* get the length */
	   posz3 = posz2;
	   wh = ro0076_stwhere(hdl);                   /* store where i am */
	   if (posz3 == target)           /* check if it's the last string */
	       target = wh;                                 /* store where */
	   else         /* copy the string to the start of the last string */
	      {
	       ro0081_stmove(hdl,target,posz3,posz3-wh,posz0);
	       target = target - posz3 + wh;
	      }
	  }
       }
  return (target);                                /* last storing position */
}
/***************************************************************************\
*                                                                           *
* asn1_stroct(hdl,endpos,x,l,m)                                             *
* Pack a structured asn1 string.                                            *
*     hdl= Store where the String lies (r/w-position at the L-field)        *
*  endpos= the usual buffer limit.                                          *
*       x= will contain the position of the packed string.                  *
*       l= will contain the length of the packed string.                    *
*       m= will indicate whether the result is structured or not.           *
* Returns the 'target' position.                                            *
*                                                                           *
\***************************************************************************/

long PASCAL asn1st_stroct(
STOREHANDLE hdl,
long endpos,
long FAR *x,
long FAR *l,
int FAR *m)
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
	  asn1st_errdecset(hdl,ASN1_ERR_LEN);
	  return(target);
	 }
      while (ro0077_stgetcp(hdl))      /* while endposition is not reached */
	 {
	  err = asn1st_skip(hdl,endpos);       /* skip over next component */
	  if (err)
	      return (target);
	  target = ro0076_stwhere(hdl);                /* the new position */
	  if ((target+2) > endpos)             /* wrong string termination */
	     {                                /* set error in Store + ASN1 */
	      asn1st_errdecset(hdl,ASN1_ERR_LEN);
	      return(posz2);
	     }
	 }
				       /* will be as:    target += 2;      */
      posz2 = target+2;              /*               posz2 = target;      */
     }
  ro0066_storepos(hdl,posz3);             /* move back to where we started */
  *x = asn1st_octpak(hdl,endpos,target); /* pack the values to the end and */
				       /* get the starting position of the */
							  /* 'new' V-field */
  *l = target - *x;                                      /* the new length */

   /*----------- NOT INSTALLED IN THE FIRST VERSION -----------------------
	if (*l < 128)
		l1 = 1;
	else    for(i= *l,l1=2;i>256;i=i>>8) l1++;
	if ((i = z2 - z3) < 128)
		l2 = 1;
	else    for(l2=2;i>256;i=i>>8) l2++;
	z = *x;
	if ((z-z3) > (l1 + l2)){
		*m = 1;
		if (*l < 128)
			*(--z) = *l;
		else{   for(i= *l;i<128;i=i>>8) *(--z) = i;
			*(--z) = l1 + 128;
		}
		*(--z) = 4;
		while ((z - z3) > (2 + l2)){
			*(--z) = 0; *(--z) = 4;}
	}else   *m = 0;
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

int PASCAL asn1st_octets_dec(
STOREHANDLE hdl,
long endpos,
long FAR *offset,
long FAR *len)
{
  int s=1;
  long posz0,oldpos;
  signed short int m;
  int sterr;

  m = ro0077_stgetcp(hdl);                            /* store the T-field */
  posz0=ro0076_stwhere(hdl);                           /* store where i am */
  if ( (sterr = asn1st_skiptype(hdl,endpos)) == OK) /* skip over the T-field */
     {
      if (m & 32)                                        /* if constructed */
	 {                                           /* pack subconstructs */
	  oldpos = asn1st_stroct(hdl,endpos,offset,len,&s);
	  sterr = ro0066_storepos(hdl,oldpos);   /* move back where I've been */
	 }
					   /* octet string not constructed */
      else if (asn1st_length(hdl,endpos,&posz0) == OK ) /* get endposition */
	      {
	       *offset = ro0076_stwhere(hdl);
	       if (posz0 && posz0 <= endpos )    /* check for valid length */
		  {
		   *len = posz0 - *offset;             /* calculate length */
		   sterr= ro0066_storepos(hdl,posz0); /* move to end of OCTET STR */
		  }
	       else
					    /* set error in Store + Mavros */
		   sterr = asn1st_errdecset(hdl,ASN1_ERR_LEN);
	      }
     }
  return (sterr);                               /* report the error status */
}
