/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: std.c,v $
 * Revision 1.1.12.2  1996/02/18  19:42:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:25  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:22:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:16  root]
 * 
 * Revision 1.1.10.3  1994/06/21  14:44:04  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:25:57  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  15:07:11  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:16:33  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  18:35:21  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:45:05  marrek]
 * 
 * Revision 1.1.8.2  1993/08/11  10:10:41  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:30:21  marrek]
 * 
 * Revision 1.1.6.5  1992/12/31  18:11:51  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:08:51  bbelch]
 * 
 * Revision 1.1.6.4  1992/11/27  16:27:35  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  13:16:14  marrek]
 * 
 * Revision 1.1.6.3  1992/09/29  21:09:48  devsrc
 * 	[hinman@sni] - Check in merges from SNI
 * 	[1992/09/14  20:22:42  treff]
 * 
 * Revision 1.1.2.2  1992/06/01  20:03:34  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:49:49  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: std.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 19:42:35 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

#include <gds.h>
#include <stdlib.h>
#include <string.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

d2_ret_val copy_octet_string (
octet_string * orig,
octet_string * copy )
{
const char function_name[] = "copy_octet_string";
	if (orig->nb)
	{    if (orig->nb > copy->nb)
	     {    if (copy->nb == 0)
	       {    if ((copy->contents =
		     D27_MALLOC (svc_c_sev_warning,orig->nb)) == NULL)
			{
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
	       }
	       else
	       {    if ((copy->contents =
		      D27_REALLOC (svc_c_sev_warning,
					copy->contents, orig->nb)) == NULL)
			{
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
	       }
	       copy->nb = orig->nb;
	     }
	     memcpy ((char *)copy->contents, (char *)orig->contents, orig->nb);
	}
	return (D2_NOERROR);
}


