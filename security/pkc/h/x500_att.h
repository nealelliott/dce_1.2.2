/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: x500_att.h,v $
 * Revision 1.1.2.2  1996/11/13  20:40:15  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/17  00:19 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
 *
 * Revision 1.1.2.1  1996/10/04  19:52:01  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  19:51:27  arvind]
 * 
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:26 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  16:11 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/*
 * (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 */


#ifndef X500_ATTRIBUTES_
#define X500_ATTRIBUTES_

#ifndef __cplusplus
#error This file must be compiled with a C++ compiler
#endif

#ifdef NOT_DCE
#include "asn.h"
#else
#include <dce/asn.h>
#endif

enum value_syntax_type {unknownSyntax,
                        caseIgnoreStringSyntax,
                        caseExactStringSyntax,
			printableStringSyntax,
			numericStringSyntax};

unsigned32 x500AttrToOID(const char * attr_name,
                         asn_objectidentifier & attr);
// Convert an X500 attribute-type name string into an OID

unsigned32 x500OIDToSyntax(const asn_objectidentifier & o,
                           value_syntax_type & syntax);
// Convert an OID into an X500 attribute-type name string
			    

#endif


