/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stnumber.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:55  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:09  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:08  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:47:48  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:05  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:50  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:18  keutel
 * 	creation
 * 	[1994/03/21  13:16:23  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stnumber.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:06 $" } ;
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
static char *sccsid = " @(#) stnumber.c 1.1 93/11/29  ST_MAVROS";
/************************************************************************\
*                                                                        *
* asn1st_number(hs,endpos)                                               *
*                                                                        *
* Returns the number of sub-components in a constructed ASN-1 field.     *
* The r/w-position of the Store is at the L-Field.                       *
* <endpos> must lie within the Store-limit.                              *
*                                                                        *
\************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

long PASCAL asn1st_number(
STOREHANDLE hdl,
long endpos)
{       long posz2,inpos;
	int i=0;

	inpos = ro0076_stwhere(hdl);

	if ( asn1st_length(hdl,endpos,&posz2))
		return(0);
	if (posz2){
		if (posz2 > endpos) 
			return(0);
		else    endpos = posz2;
	}
	while (!ro0063_sterror(hdl) && (ro0076_stwhere(hdl) < endpos)
			 && ro0077_stgetcp(hdl) != 0 ) {

		if (asn1st_skip(hdl,endpos) != 0) return((long)i);
		i++;
	}
	ro0066_storepos(hdl,inpos);
	return(i);
}
