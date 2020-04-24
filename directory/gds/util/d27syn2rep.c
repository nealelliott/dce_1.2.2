/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27syn2rep.c,v $
 * Revision 1.1.12.2  1996/02/18  18:24:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:57  marty]
 *
 * Revision 1.1.12.1  1995/12/08  16:17:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:47  root]
 * 
 * Revision 1.1.10.1  1994/02/22  19:22:06  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:55:53  marrek]
 * 
 * Revision 1.1.8.1  1993/10/14  19:19:10  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:16:22  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:05:35  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  14:41:09  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  13:15:03  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:20:39  marrek
 * 	Insert <pthread.h>
 * 	introduce D2_IA5
 * 	[1993/02/02  09:49:11  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  22:06:15  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:49:26  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  18:55:35  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  15:45:39  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:12:27  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:36:36  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27syn2rep.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 18:24:04 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/d27util.h>

/*****************************************************************************/
/*                                                                           */
/*  FUNCTION:  d27_401_syntax_to_rep                                         */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      This function returns the correct ASN.1 representation for a given   */
/*      attribute syntax.                                                    */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      >0          = ASN.1 representation value. Refer to d2dir.h.          */
/*      D2_ERROR    = An error    occurred.                                  */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: Melissa Johnston                               DATE: 28.11.90    */

signed16 d27_401_syntax_to_rep(
    signed16 syntax)    /* IN    -  syntax to convert                 */

{
  switch (syntax)
  {
    case D2_CEXACT:
    case D2_CIGNORE:
      return(D2_T61_PR);
    case D2_CIGN_LIST:
    case D2_POST_AD_STX:
      return(D2_T61_PR_LIST);
    case D2_ANY_STX:
    case D2_PASSWD_STX:
      return(D2_OCTET);
    case D2_COUNTRY_STX:
    case D2_TELNR_STX:
      return(D2_PRINTABLE);
    case D2_PR_DM_STX:
    case D2_MHS_PR_DM_STX:
      return(D2_INT_LIST);
    case D2_ACCL:
    case D2_BOOLEAN:
    case D2_DISTNAME:
    case D2_FAXNR_STX:
    case D2_GUIDE:
    case D2_INTEGER:
    case D2_NUMERIC:
    case D2_OBJ_IDENT:
    case D2_OCTET:
    case D2_PRINTABLE:
    case D2_IA5:
    case D2_PSAP_STX:
    case D2_TIME:
    case D2_TLXNR_STX:
    case D2_TTXID_STX:
    case D2_CERTIFICATE:
    case D2_CERT_PAIR:
    case D2_CERT_LIST:
    case D2_MHS_DLSP_STX:
    case D2_MHS_ORADR_STX:
    case D2_MHS_ORNAME_STX:
    case D2_ASN1:
      return(syntax);
    default:                    /* not supported                      */
	return(D2_ERROR);
  }
}				/* end d27_401_syntax_to_rep()        */
