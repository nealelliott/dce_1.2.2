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
 * $Log: x500name.h,v $
 * Revision 1.1.2.2  1996/11/13  20:40:28  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/17  00:20 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
 *
 * Revision 1.1.2.1  1996/10/04  19:53:46  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  19:53:24  arvind]
 * 
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:26 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  16:11 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/*
 *
 * (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 */


#ifndef X500NAME__
#define X500NAME__

#ifndef __cplusplus
#error This file requires a C++ compiler
#endif

#ifdef NOT_DCE
#include "asn.h"
#include "x500_att.h"
#else
#include <dce/asn.h>
#include <dce/x500_att.h>
#endif

extern "C" {
#include "xom.h"
}

extern unsigned32 unsegment(const char * str,
                            char sep,
                            char ** & result);


extern int attributeMatch(const asn_objectidentifier & attrType,
                          const unsigned char * v1,
                          size_t l1,
                          const unsigned char * v2,
                          size_t l2);
// Compares two attribute values.  The AVAs must be of the same type (indicated
// attrType), and this type is used to select appropriate matching rules.
// The two {v,l} pairs are DER-encoded V portions of the TLV encoding of the
// attribute value.  The ASN.1 type of these values is as appropriate for
// the attrType.


extern int matchCaseIgnoreString(const unsigned char * v1, size_t l1,
                                const unsigned char * v2, size_t l2);

extern int matchCaseExactString(const unsigned char * v1, size_t l1,
                                const unsigned char * v2, size_t l2);

extern int matchPrintableString(const unsigned char * v1, size_t l1,
                                const unsigned char * v2, size_t l2);

extern int matchNumericString(const unsigned char * v1, size_t l1,
                              const unsigned char * v2, size_t l2);


class x500_AttrValue : public asn_conformantstring {
public:
    virtual unsigned32 set_value(char * s);
    x500_AttrValue(void) : asn_conformantstring(19) {};
    x500_AttrValue(unsigned t) : asn_conformantstring(t) {};
    x500_AttrValue(unsigned t, char * s) : asn_conformantstring(t, s) {};
    virtual x500_AttrValue & 
        operator=(const x500_AttrValue & o);
    virtual unsigned32 hash(buffer_t & buffer) const;
};

class AttributeValueAssertion : public asn_sequence {
    OM_object xom_obj;
public:
    asn_objectidentifier AttributeType;
    x500_AttrValue AttributeValue;
    virtual unsigned32 hash(buffer_t & buffer) const;
    AttributeValueAssertion(void) {
	xom_obj = NULL;
        register_child(&AttributeType);
        register_child(&AttributeValue);
    };
    virtual ~AttributeValueAssertion(void) {
	if (xom_obj) delete_xom_object();
    };
    int operator==(const AttributeValueAssertion & o) const;
    virtual AttributeValueAssertion & 
        operator=(const AttributeValueAssertion & o);
    virtual unsigned32 display(buffer_t & buffer) const;
    virtual unsigned32 set_value(char * str);
    virtual unsigned32 set_value(OM_object xom_object);
    virtual unsigned32 xom_object(OM_object & par) const;
    unsigned32 delete_xom_object(void) const;  
};

class RelativeDistinguishedName : public asn_setof<AttributeValueAssertion> {
    OM_object xom_obj;
public:
    RelativeDistinguishedName(void) {
	xom_obj = NULL;
    };
    virtual ~RelativeDistinguishedName() {
	if (xom_obj) delete_xom_object();
    };
    virtual unsigned32 hash(buffer_t & buffer) const;
    virtual RelativeDistinguishedName & 
        operator=(const RelativeDistinguishedName & o);
    virtual int operator==(const RelativeDistinguishedName & o) const;
    virtual unsigned32 set_value(const char * par);
    virtual unsigned32 set_value(OM_object xom_object);
    virtual unsigned32 display(buffer_t & buffer) const;
    virtual unsigned32 xom_object(OM_object & par) const;
    virtual unsigned32 delete_xom_object(void) const;  
};

class x500name : public asn_sequenceof<RelativeDistinguishedName> {
    OM_object xom_obj;
public:
    x500name(void) {
	xom_obj = NULL;
    };
    x500name & operator=(const x500name & o);
    virtual ~x500name() {
	if (xom_obj) delete_xom_object();
    };
    virtual unsigned32 hash(buffer_t & buffer) const;
    virtual int operator==(const x500name & o) const;
    virtual int operator!=(const x500name & o) const {
        return !(*this == o);
    };
    virtual unsigned32 set_value(const char * par);
    virtual unsigned32 set_value(OM_object xom_object);
    virtual unsigned32 display(buffer_t & buffer, int dce_format = 0) const;
    virtual unsigned32 make_parent(x500name ** par) const;
    virtual unsigned32 make_ancestor(unsigned rdn_count, x500name ** par) const;
    virtual unsigned32 xom_object(OM_object & par) const;
    virtual unsigned32 delete_xom_object(void) const;  
// Call the above method when you're finished with the OM_object associated 
// with the name.  Don't try to pass an OM_object that belongs to a name to
// om_delete(), as a name's OM_object is a client-generated public object,
// and om_delete() doesn't know what to do with those...
    virtual int is_descendant(const x500name & o) const;
// Returns true if this name is a descendant of the parameter
    virtual int is_ancestor(const x500name & o) const;
// Returns true if this name is an ancestor of the parameter
};



unsigned common_root_length(const x500name & n1,
                            const x500name & n2);

unsigned32 compare_names(const x500name & n1, 
                         const x500name & n2,
                         char * names_equal,
                         unsigned * n1_to_common,
                         unsigned * n2_to_common);
// Determines whether two names are equal, and how far (in terms of RDNs) each
// is from the common ancestor.

#endif
