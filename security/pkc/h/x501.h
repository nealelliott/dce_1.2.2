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
 * $Log: x501.h,v $
 * Revision 1.1.2.2  1996/11/13  20:40:41  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  20:01:11  arvind]
 *
 * $EndLog$
 */
/*
 * HISTORY
 * CAPI drop from DEC
 * [1996/10/17  00:22 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
 * $Log: x501.h,v $
 * Revision 1.1.2.2  1996/11/13  20:40:41  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  20:01:11  arvind]
 *
 * Revision 1.1.3.2  1996/11/13  20:01:11  arvind
 * 	OSF DCE 1.2.2 Drop 5
 *
 * Revision 1.1.2.1  1996/10/04  19:54:51  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  19:54:40  arvind]
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



#ifndef X501_H_
#define X501_H_
#include <dce/asn.h>

// This isn't a complete X.501 implementation.  In fact, most of X.501 is
// to do with the definition of X.500 names, which are implemented in file
// x500name.h.  This header serves only to define the Attribute type, which 
// isn't needed in names, but is required by PKCS#8

class AttributeValue : public asn_any {
    
};

class asn_AttributeValueSet : public asn_setof<AttributeValue> {
};

class AttributeType : public asn_objectidentifier {
};

class Attribute : public asn_sequence {
public:
    AttributeType type;
    asn_AttributeValueSet values;
};

#endif
