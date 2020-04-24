/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stcopy.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:35  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:54  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:15:20  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:03:22  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:55  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:13  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:29:58  keutel
 * 	creation
 * 	[1994/03/21  13:15:47  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stcopy.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:45 $" } ;
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
static char *sccsid = " @(#) stcopy.c 1.1 93/11/29  ST_MAVROS";
/****************************************************************************\
*                                                                            *
* asn1st_copy(hdl,m,n,zz)                                                    *
*                                                                            *
* Copy an ASN1 encoded area pointed by <zz> (in memory) into the STORE <hdl>.*
* The T-field contains: m=mode/class, n=tagnumber (the structure bit is not  *
* influenced). Returns the error status.                                     *
*                                                                            *
\*****************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int PASCAL asn1st_copy(
STOREHANDLE hdl,
int m,
int n,
asn1 zz,
long l)
{       asn1 zz0;
	int sterr;
	if (m != -1){
		zz0 = zz;
		asn1st_type_cod(hdl,m,n,(int)(*zz&0x20));
		if ((zz = asn1_jumptype(zz, zz+l)) == 0){
		/* really use asn1_jumptype because zz is an area in memory */
		     sterr = ro0067_stputc(hdl,0);/* error in skipping type */
					/* length set to 0, no value copied */
		     sterr = asn1st_errdecset(hdl,ASN1_ERR_TAG);
		     return(sterr);
		}
		l -= (zz - zz0);
	}
	sterr = ro0068_stputm(hdl,(char FAR *)zz,(long) l);
	return(sterr);
}

/***********************************************************************\
* 								        *
* asn1_jumptype(z,zm)   (the same as asn1_skiptype)                     *
* 								        *
* Skip the type field: <z> points to the type field to be skipped. <zm> *
* points to the last byte in the buffer.  Returns a pointer to the      *
* following byte, or a null pointer in case of error. The following     *
* errors are detected:						        *
* 								        *
* . ASN1_ERR_TAG	The tag length exceeds the buffer limit.        *
* 								        *
\***********************************************************************/

asn1 PASCAL asn1_jumptype(
asn1 z,
asn1 zm)
{
	if (z == 0) return((asn1)0);
	if (((*z++)&0x1F) == 31)
		while ((z < zm) && ((*z++)&0x80));
	if (z < zm)
		return(z);
	else{
		return(0);
	}
}
