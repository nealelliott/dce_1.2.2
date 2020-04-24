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
 * $Log: x500_att.cxx,v $
 * Revision 1.1.2.2  1996/11/13  20:29:56  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:23 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
 *
 * Revision 1.1.2.1  1996/10/03  20:26:12  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:28:07  arvind]
 * 
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:24 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  16:05 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 * Module:
 *
 * Purpose:
 *
 */


#ifndef NOT_DCE
#include "x500_att.h"
#else
#include <dce/x500_att.h>
#endif

extern "C" {
#include <stdio.h>
}

#ifndef NOT_DCE
extern "C" {
#include <dce/cdsclerk.h>
#include <dce/gssapi.h>
#include <xdscds.h>
}
#else
// Hack just enough to run the tests in a non-DCE environment.

#include "gssapi.h"

typedef struct {
    char * objid;
    char * tag;
    char * ident;
    int syntax;
    int matchingrule;
} attrent_t;

extern "C" attrent_t * cdsGetXAttrByObjID(unsigned char *);
extern "C" attrent_t * cdsGetXAttrByString(unsigned char *);

#define CES 1
#define CIS 2
#define PS 3
#define NS 4

#define CEM 1
#define CIM 2
#define PM 3
#define NM 4


#define OMP_O_DSX_TYPELESS_RDN "\x2B\x16\x01\x02\x02"

#endif

 
unsigned32 x500AttrToOID(const char * attr_name,
                         asn_objectidentifier & attr) {
// Convert an attribute name to an OID
    attrent_t * attribute;
    unsigned32 error;
#ifdef DEBUG
    fprintf(stderr, "DEBUG: x500AttrToOID(%s)", attr_name);
    fflush(stderr);
#endif
    attribute = cdsGetXAttrByString((unsigned char *)attr_name);
    if (attribute == NULL) return pkc_s_x500_unsupported_attribute;
    return attr.set_value(attribute->objid);
}

unsigned32 x500OIDToSyntax(const asn_objectidentifier & o,
                           value_syntax_type & syntax) {
// Return the value syntax associated with the given OID
    attrent_t * attribute;
    unsigned32 error;
    buffer_t OIDencoding;
    gss_OID oid_value = NULL;
    OM_uint32 min, maj;

    if (error = o.x500_display(OIDencoding)) return error;
    OIDencoding.append((unsigned char) 0); // Null-terminate the string
					   // 
    attribute = cdsGetXAttrByObjID(OIDencoding.data);

    if (attribute == NULL) {
        if ((error = o.get_value(oid_value)) != 0) return error;
        if ((oid_value->length == sizeof(OMP_O_DSX_TYPELESS_RDN)-1) &&
            (memcmp(oid_value->elements,
                    OMP_O_DSX_TYPELESS_RDN,
                    oid_value->length) == 0)) {
            syntax = printableStringSyntax;
	    maj = gssdce_release_oid(&min, &oid_value);
            return 0;
	};
        maj = gssdce_release_oid(&min, &oid_value);
        return pkc_s_x500_unsupported_attribute;
    };
    switch (attribute->syntax) {
        case CES: syntax = caseExactStringSyntax; break;
        case CIS: syntax = caseIgnoreStringSyntax; break;
        case PS: syntax = printableStringSyntax; break;
        case NS: syntax = numericStringSyntax; break;
    };    
    return 0;

}

#ifdef NOT_DCE
static attrent_t attrs[15] = {
{"2.5.4.6", "", "Country", CES, CEM},
{"2.5.4.6", "", "CountryName", CES, CEM},
{"2.5.4.6", "", "C", CES, CEM},
{"2.5.4.10", "", "Organization", PS, PM},
{"2.5.4.10", "", "OrganizationName", PS, PM},
{"2.5.4.10", "", "O", PS, PM},
{"2.5.4.8", "", "OrganizationalUnit", PS, PM},
{"2.5.4.8", "", "OU", PS, PM},
{"2.5.4.3", "", "CommonName", PS, PM},
{"2.5.4.3", "", "CN", PS, PM},

};

attrent_t * cdsGetXAttrByObjID(unsigned char * s) {
    attrent_t * ap = attrs;
    while (ap->objid != NULL) {
        if (strcmp((const char *)s, ap->objid) == 0) return ap;
        ap++;
    };
    return NULL;
}

attrent_t * cdsGetXAttrByString(unsigned char * s) {
    attrent_t * ap = attrs;
    while (ap->objid != NULL) {
        if (strcmp((const char *)s, ap->ident) == 0) return ap;
        ap++;
    };
    return NULL;
}
#endif

