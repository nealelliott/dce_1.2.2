/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: aban_cod.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:01  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:58:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:59  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:47:14  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:40:54  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:55:38  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:59:09  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:37:52  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:28:07  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  19:41:21  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:49:49  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:39:10  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: aban_cod.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:14 $";
#endif

#include <dce/asn1.h>
#include <dce/d2dir.h>
#include "d23apdu.h"

extern asn1 ids_abandon_arg_cod( );

d2a000_apdu_asn1(x, y, apdu, apdu_len, asn1_cod, err_apdu)
int x,y;
Octet * apdu;
int apdu_len;
asn1_field * asn1_cod;
Octet * err_apdu;
{

int z;

	z = *((Long *)(apdu + D23_S_HDR));

	if ((asn1_cod->v = malloc (ids_abandon_arg_len(-1, z))) == (asn1)NULL)
		{
		  printf ("Memory_error\n");
		  exit(1);
		 }

	asn1_cod->l = ids_abandon_arg_cod( asn1_cod->v, -1, 0, z) - asn1_cod->v;
}
