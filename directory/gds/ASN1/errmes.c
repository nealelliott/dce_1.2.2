/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: errmes.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:25  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:34  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:40  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:30  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:22:54  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:52:25  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:27:21  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:58:43  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:34:35  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:47  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:16:31  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:18:02  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: errmes.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:35 $";
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

/**************************************************************\
* 							       *
* asn1_errmes(x)					       *
* 							       *
* Returns a pointer to a character string explaining the ASN1  *
* diagnostic code ``x''					       *
* 							       *
\**************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

char * asn1_errmes(x)
int x;
{
	switch(x){
	case ASN1_ERR_TAG:	return("Tag length > limit");
	case ASN1_ERR_LLEN:	return("Length's length > limit");
	case ASN1_ERR_LEN:	return("Length > limit");
	case ASN1_ERR_SHORT:	return("Value too short");
	case ASN1_ERR_EOC:	return("Incorrect EOC format");
	case ASN1_ERR_BOUND :	return("EOC missing before limit");
	case ASN1_ERR_LDEF :	return("Undefined form for primitive");
	case ASN1_ERR_STRUCT:	return("Structured form for primitive");
	case ASN1_ERR_MISS:	return("Mandatory component missing");
	case ASN1_ERR_CHOICE:	return("Incorrect choice component");
	case ASN1_ERR_BOOL:	return("Incorrect boolean");
	case ASN1_ERR_NULL:	return("Incorrect null component");
	case ASN1_TEXT_EXISTS:	return("Object ID name already declared");
	case ASN1_OID_EXISTS:	return("Object ID already declared");
	case ASN1_OIDEMPTY:	return("empty object identifier");
	case ASN1_OIDBADTOP:	return("bad top level object id");
	case ASN1_OIDUNDEF:	return("undefined object identifier");
	case ASN1_OIDMISSSEC:	return("missing second level component");
	case ASN1_OIDSECBIG:	return("second component > 39");
	case ASN1_OIDBADSEC:	return("incorrect second component");
	case ASN1_OIDNUMCOMP:	return("final components must be numeric");
 	case ASN1_ERR_EQUAL:	return("missing equal sign");
	case ASN1_ERR_OBRACKET:	return("missing opening bracket");
	case ASN1_ERR_CBRACKET:	return("missing closing bracket");
	case ASN1_ERR_HEXA:	return("incorrect hexadecimal digit");
	case ASN1_ERR_ST_END:	return("input end before end of string");
	case ASN1_ERR_MALLOC:	return("cannot allocate memory");
	case ASN1_ERR_B_OR_H:	return("string type must be B or H");
	case ASN1_ERR_BSTRING:	return("incorrect binary string");
	case ASN1_ERR_STRING:	return("string component expected");
	case ASN1_ERR_HSTRING:	return("hexa string should be typed H");
	case ASN1_ERR_ST_EOL:	return("end of line before end of string");
	case ASN1_ERR_FLAGS:	return("unexpected bit string element");
	case ASN1_ERR_ABSURD:	return("absurd data type");
	default:		return("Unknown ASN1 error");
	}
}
