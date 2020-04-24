/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppmzx.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:52  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:13:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:20:08  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:06:07  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:19  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:15  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:52:15  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:50  keutel
 * 	creation
 * 	[1994/03/21  13:21:28  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roppmzx.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:57 $" } ;
#endif


#ifdef THREADSAFE
#include <pthread.h>
#endif

#include "rogenrl.h"
#include "rouply0.h"
#include "rostor0.h"
#include "roppm01.h"
#include "roppmzx.h"


/************************************************************************ */
/*                                                                        */
/*  signed short int PASCAL asn1st_matchctags(hdl,tag1,tag2,tag3)         */
/*                                                                        */
/*  tests if tag1 or tag 2 or tag 3  matches with the actual byte         */
/*  of the ASN1 string, hold in store.                                    */
/*                                                                        */
/*  Returnvalue : Error  < 0  if StoreError occures                       */
/*                0           if one of the input tags == actual byte     */
/*                RC041l_UnrecognizedPpdu   otherwise;                    */
/*                                                                        */
/*  Input tags with value 0 are ignored because a tag with value 0 is not */
/*  possible. If the actual byte is 0, RC041l_UnrecognizedPpdu is         */
/*  returned.                                                             */
/*                                                                        */
/************************************************************************ */
signed short int PASCAL asn1st_matchctags(
	STOREHANDLE hdl,
	int tag1,
	int tag2,
	int tag3)
{
    int sterr;
    unsigned char matchz;

    matchz = (unsigned char) ro0077_stgetcp(hdl);
					      /* Reading one byte for Tag */
    if ( (sterr = ro0063_sterror(hdl)) < 0 ) return sterr;

    if ( matchz == 0 )                        /* Test for Null in tagbyte */
	return (RC041l_UnrecognizedPpdu);

                                              /* Testing tags             */
    if ( ( (unsigned char)(matchz|32) == (unsigned char) tag1 ) ||
	 ( (unsigned char)(matchz|32) == (unsigned char) tag2 ) ||
	 ( (unsigned char)(matchz|32) == (unsigned char) tag3 ))
	 return (0);
    else return (RC041l_UnrecognizedPpdu);
}

