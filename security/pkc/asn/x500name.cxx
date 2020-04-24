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
 * $Log: x500name.cxx,v $
 * Revision 1.1.2.2  1996/11/13  20:30:11  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  23:22 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
 *
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:23 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
 *
 * Revision 1.1.2.1  1996/10/03  20:26:24  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:28:45  arvind]
 * 
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:25 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  16:06 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 * Module: X500NAME.CXX
 *
 * Purpose: Classes to deal with DCE global names with X.500 GDS
 *          portions
 *
 */



extern "C" {
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <xom.h>
#include <xds.h>
#include <xdscds.h>
}

#define NAME_DEBUG

#include <dce/x500name.h>
#include <dce/x500_att.h>
#include "pkc_common.h"

#define CHAR_QUOTE '\\'
#define OPEN_QUOTE '\"'
#define CLOSE_QUOTE '\"'

#define GLOBAL_INTRODUCER "/.../"
#define LOCAL_INTRODUCER "/.:/"
#define ALT_GLOBAL_INTRODUCER "/"

#define DISPLAY_PREFIX_X500 "/"
#define DISPLAY_PREFIX_DCE "/.../"

OM_IMPORT(DS_C_DS_DN)
OM_IMPORT(DS_C_DS_RDN)
OM_IMPORT(DS_C_RELATIVE_NAME)
OM_IMPORT(DS_C_AVA)
OM_IMPORT(DSX_TYPELESS_RDN)

// For now, we're not doing any trimming.  Must fix this later.
int matchCaseIgnoreString(const unsigned char * v1, size_t l1,
                          const unsigned char * v2, size_t l2) {

    int i;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Comparing \"%.*s\" and \"%.*s\" as CIS",
                  l1, v1, l2, v2));

    if (l1 != l2) return 0;
    for (i=0; i<l1; i++) {
        if (toupper(v1[i]) != toupper(v2[i])) return 0;
    };
    return 1;
}

int matchCaseExactString(const unsigned char * v1, size_t l1,
                         const unsigned char * v2, size_t l2) {

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Comparing \"%.*s\" and \"%.*s\" as CES",
                  l1, v1, l2, v2));

    if (l1 != l2) return 0;
    return (strncmp((const char *)v1, (const char *)v2, l1) == 0);
}

int matchPrintableString(const unsigned char * v1, size_t l1,
                         const unsigned char * v2, size_t l2) {

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Comparing \"%.*s\" and \"%.*s\" as PS",
                  l1, v1, l2, v2));

    if (l1 != l2) return 0;
    return (strncmp((const char *)v1, (const char *)v2, l1) == 0);
}

int matchNumericString(const unsigned char * v1, size_t l1,
                       const unsigned char * v2, size_t l2) {

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Comparing \"%.*s\" and \"%.*s\" as NS",
                  l1, v1, l2, v2));

    if (l1 != l2) return 0;
    return (strncmp((const char *)v1, (const char *)v2, l1) == 0);
}

unsigned32 unsegment(const char * str,
                     char sep,
                     int remove_quotes,
		     char ** & result) {
// Split a segmented string into its segments.
// sep = separator character.
// Strings in result will be de-quoted if remove_quotes is non-zero

    int char_quote = 0;
    int in_quote = 0;
    int start;
    int done_segment;
    char buffer[512];
    int wp;
    int rp;
    int segment_count;

#define transcribe(x) if (wp < sizeof(buffer) - 1) buffer[wp++] = x; \
                      else buffer[sizeof(buffer)-1] = 0;

    result = NULL;
    start = 0;
    segment_count = 0;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Unsegment(%s), seperator = \"%c\"", str, sep));

    while (isspace(str[start])) start++;

    while (str[start] != 0) {
        segment_count++;
        done_segment = 0;
        wp = 0;
        rp = start;
        memset(buffer, 0, sizeof(buffer));
        while (!done_segment) {
            if (str[rp] == 0) {
                done_segment = 1;
                continue;
            } else if (char_quote) {
                transcribe(str[rp]);
                char_quote = 0;
                rp++;
                continue;
            } else if (str[rp] == CHAR_QUOTE) {
                if (!remove_quotes) transcribe(str[rp]);
                char_quote = 1; rp++;
                continue;
            } else if (in_quote) {
                if (str[rp] == CLOSE_QUOTE) {
                    if (!remove_quotes) transcribe(str[rp]);
                    in_quote = 0;
                    rp++;
                    continue;
                } else {
                    transcribe(str[rp]);
                    rp++;
                    continue;
                };
            } else if (str[rp] == OPEN_QUOTE) {
                if (!remove_quotes) transcribe(str[rp]);
                in_quote = 1;
                rp++;
                continue;
            } else if (str[rp] != sep) {
                transcribe(str[rp]);
                rp++;
                continue;
            } else {
                rp++;
                done_segment = 1;
                continue;
            };
        };
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      "Unsegment, found end of segment, segment: \"%s\"",
                      buffer));
        result = (char **)pkc_realloc(result,
                                       (segment_count+1) * sizeof(char *));
        result[segment_count-1] = (char *)pkc_malloc(strlen(buffer)+1);
        result[segment_count] = NULL;
        strcpy(result[segment_count-1], buffer);
        start = rp;


        while (isspace(str[start])) start++;
    };
    return 0;
}



int attributeMatch(const asn_objectidentifier & attrType,
                   const unsigned char * v1,
                   size_t l1,
                   const unsigned char * v2,
                   size_t l2) {
    value_syntax_type syntax;
    x500OIDToSyntax(attrType, syntax);
    switch (syntax) {
    case caseIgnoreStringSyntax:
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      "Comparing (caseIgnoreString)"));
        if (matchCaseIgnoreString(v1, l1, v2, l2)) return 1;
	else return 0;
    case caseExactStringSyntax:
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      "Comparing (caseExactString)"));
        if (matchCaseExactString(v1, l1, v2, l2)) return 1;
	else return 0;	
    case printableStringSyntax:
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      "Comparing (printableString)"));
        if (matchPrintableString(v1, l1, v2, l2)) return 1;
	else return 0;
    case numericStringSyntax:
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      "Comparing (numericString)"));
        if (matchNumericString(v1, l1, v2, l2)) return 1;
	else return 0;
    default:
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                      "ERROR: Don't know how to compare attributes."));
        {
            buffer_t buf;

            attrType.display(buf);
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                          "Attribute type: %.*s", buf.data_length, buf.data));
    
        }
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                      "Corresponding syntax: %d", syntax));
    }
    return -1;
}

unsigned32 x500_AttrValue::set_value(char * s) {
    return asn_charstring::set_value(s);
}

unsigned32 x500_AttrValue::hash(buffer_t & buffer) const {
    char * str;
    int i;
    char c;
    asn_charstring::get_value(str);
    for (i=0;str[i]!=0;i++) {
	c = str[i];
	if (c == ' ' || c == '\t') continue; // Collapse blanks
	if (islower(c)) c = toupper(c); // Upcase
	buffer.append(c);
    };
    return 0;
}

unsigned32 AttributeValueAssertion::display(buffer_t & buffer) const {
    unsigned char * attr_val;
    size_t attr_len;
    unsigned32 status;
    buffer_t temp_buf;
    if ( !is_valid() ) {
	buffer.append("<Unset-AVA>");
	return 0;
    };

    if (status = AttributeType.calculate_value(temp_buf) != 0) {
	buffer.append("<Unset-AttributeType>");
        buffer.append("=");
    } else {
        if ((temp_buf.data_length != OMP_LENGTH(DSX_TYPELESS_RDN)) ||
            (memcmp((unsigned char *)OMP_D_DSX_TYPELESS_RDN,
                    temp_buf.data,
                    temp_buf.data_length) != 0)) {
            AttributeType.x500_display(buffer);
            buffer.append("=");
        };
    };
    AttributeValue.x500_display(buffer);
    return 0;
}

unsigned32 AttributeValueAssertion::set_value(OM_object name) {
    AttributeValueAssertion * ava;
    unsigned32 error;
    int i;
    unsigned char * temp_ptr;
    size_t temp_length;
// A OM_object AVA is simply an {Attribute, value} pair...
    if (name[0].type != OM_CLASS) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                  "AVA[0].type = %d (expecting OM_CLASS)",
                  name[0].type));
	return pkc_s_bad_value;
    };
    if ((name[0].syntax & OM_S_SYNTAX) != OM_S_OBJECT_IDENTIFIER_STRING) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                 "AVA[0].syntax = %d (expecting OM_S_OBJECT_IDENTIFIER_STRING)",
                 name[0].syntax));
	return pkc_s_bad_value;
    };
    if (name[0].value.string.length != OMP_LENGTH(DS_C_AVA)) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                  "AVA[0].value.string.length wrong"));
	return pkc_s_bad_value;
    };
    if (memcmp(name[0].value.string.elements,
               OMP_D_DS_C_AVA,
               name[0].value.string.length) != 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                  "AVA[0].value.string.elements wrong (should be DS_C_AVA)"));
	return pkc_s_bad_value;
    };

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                  "AVA.set_value(), reading AttrType..."));
    if (name[1].type != DS_ATTRIBUTE_TYPE) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                  "AVA[1].type is %d (should be ATTRIBUTE_TYPE)",
                  name[1].type));
	return pkc_s_bad_value;
    };
    if ((name[1].syntax & OM_S_SYNTAX) != OM_S_OBJECT_IDENTIFIER_STRING) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                  "AVA[1].syntax is %d (should be OID_STRING)",
                  name[1].syntax));
	return pkc_s_bad_value;
    };

    temp_length = name[1].value.string.length;
    temp_ptr = (unsigned char *)name[1].value.string.elements;

    if (error = AttributeType.decode_value_definite(temp_ptr, temp_length)) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                  "Error %d decoding attribute-type", error));
	return error;
    };
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Read attribute type"));

    if (name[2].type != DS_ATTRIBUTE_VALUES) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                  "AVA[2].type is %d (should be ATTRIBUTE_VALUES)",
                  name[2].type));
	return pkc_s_bad_value;
    };
    if ((name[2].syntax & OM_S_SYNTAX) != OM_S_PRINTABLE_STRING &&
	(name[2].syntax & OM_S_SYNTAX) != OM_S_TELETEX_STRING) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                      "AVA[2].syntax is %d (should be PS or TS)",
                      name[2].syntax));
	return pkc_s_bad_value;
    }
    temp_length = name[2].value.string.length;
    temp_ptr = (unsigned char *)name[2].value.string.elements;
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "AVA.set_value(), reading attribute value..."));
    if (error = AttributeValue.decode_value_definite(temp_ptr, temp_length)) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      "AttributeValue.decode returned %d", error));
	return error;
    };
    if (name[3].type != OM_NO_MORE_TYPES) {
	return pkc_s_bad_value;
    };
    value_changed();
    return 0;
}

x500_AttrValue & x500_AttrValue::operator=(const x500_AttrValue & o) {
    unsigned char * ob;
    size_t ol;
    unsigned32 error;
    
    if ((error = o.asn_octetstring::get_value(ob, ol)) != 0) return *this;
    default_tag_value = o.default_tag_value;
    if ((error = asn_octetstring::set_value(ob, ol)) != 0) return *this;
    value_changed();
    return *this;
}


unsigned32 AttributeValueAssertion::set_value(char * str) {
    int i;
    int j;
    unsigned32 error;
    char buffer[512];
    unsigned char * temp_ptr;
    size_t temp_len;
    unsigned tag_type;
    
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "AVA::set_value(), hunting for '='..."));
    for (i=0; ; i++) {
        if (str[i] == 0) break;
        if (str[i] == '=') break;
    };
    if (i >= sizeof(buffer)-1) return pkc_s_asn_value_overflow;
    strncpy(buffer, str, i);
    buffer[i] = 0;
    if (str[i] == '=') {
        for (j=0;buffer[j]!='\0';j++) {
            if ((!isdigit(buffer[j]))
	        && (buffer[j] != '.')
		&& (!isspace(buffer[j]))) break;
	};
        if (buffer[j] == '\0') {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "Parsing numeric AttributeType \"%s\"",
                          buffer));
// This is a numeric OID
            if (error = AttributeType.set_value(buffer)) {
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                              "AttributeType.set_value() returned 0x%X",
                              error));
                return error;
            };
	} else {

            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "Attrname = \"%s\", setting Attrtype...",
			  buffer));
            if (error = x500AttrToOID(buffer, AttributeType)) {

                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                              "x500AttrToOID returned 0x%X", error));

                return error;
            };
        };
        strncpy(buffer, &str[i+1], sizeof(buffer));
    } else {
// This is a typeless RDN
	temp_ptr = (unsigned char *)OMP_D_DSX_TYPELESS_RDN;
	temp_len = OMP_LENGTH(DSX_TYPELESS_RDN);
	AttributeType.decode_value_definite(temp_ptr, temp_len);	
    };

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Attrval = \"%s\", Setting value...", buffer));

    tag_type = 19; // Default string type is PRINTABLE_STRING
    for (i=0; buffer[i] != 0; i++) {
	if (isdigit(buffer[i]) || isxdigit(buffer[i]))
	   continue;
	else switch (buffer[i]) {
	case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
	case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
	case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
	case 'Y': case 'Z':
	case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
	case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
	case 's': case 't': case 'u': case 'v': case 'w': case 'x':
	case 'y': case 'z':
        case ' ':
        case '\'':
	case '(':
	case ')':
	case '+':
	case ',':
	case '-':
	case '.':
	case '/':
	case ':':
	case '=':
	case '?':
	   continue;
        default:
	   break;
	};
// If we get here, we didn't execute a <continue> above, which means that
// a character outside PS's charset is present, therefore we have to
// use TELETEX_STRING instead.  Once we've decided this, break out of the
// loop.
        tag_type = 20;
        break;
    };
    AttributeValue.set_tag(tag_type);
    AttributeValue.set_value(buffer);
    value_changed();
    return 0;
}


unsigned32 AttributeValueAssertion::delete_xom_object(void) const {
// The dynamic portions of an AVA are:
// i) The type OID (at obj[1].value)
// ii) The AttributeValue portion (at obj[2].value)
// iii) The class object itself
    AttributeValueAssertion * this_var = (AttributeValueAssertion *)this;

    if (xom_obj == NULL) return 0;
    if (this_var->xom_obj[1].value.string.elements)
	pkc_free(this_var->xom_obj[1].value.string.elements);
	    // Free the type OID
    this_var->xom_obj[1].value.string.elements = NULL;
    if (this_var->xom_obj[2].value.string.elements)
	pkc_free(this_var->xom_obj[2].value.string.elements);
	    // Free the value string
    this_var->xom_obj[2].value.string.elements = NULL;
    pkc_free(this_var->xom_obj);
    this_var->xom_obj = NULL;
    return 0;
}

unsigned32 AttributeValueAssertion::xom_object(OM_object &result) const {
    int i;
    buffer_t type_buffer;
    char * string_value;
    AttributeValueAssertion * this_var = (AttributeValueAssertion *)this;

    if (xom_obj) delete_xom_object();
    this_var->xom_obj = (OM_descriptor *)pkc_malloc(sizeof(OM_descriptor) * 4);
    memset(this_var->xom_obj, 0, sizeof(OM_descriptor) * 4);
    this_var->xom_obj[0].type = OM_CLASS;
    this_var->xom_obj[0].syntax = OM_S_OBJECT_IDENTIFIER_STRING;
    this_var->xom_obj[0].value.string.length = OMP_LENGTH(DS_C_AVA);
    this_var->xom_obj[0].value.string.elements = OMP_D_DS_C_AVA;
    this_var->xom_obj[1].type = DS_ATTRIBUTE_TYPE;
    this_var->xom_obj[1].syntax = OM_S_OBJECT_IDENTIFIER_STRING;
    AttributeType.calculate_value(type_buffer); // Calculate_value allocates
						// storage for its output data
    this_var->xom_obj[1].value.string.length = type_buffer.data_length;
    this_var->xom_obj[1].value.string.elements = type_buffer.data;
    type_buffer.detach_buffer();
    this_var->xom_obj[2].type = DS_ATTRIBUTE_VALUES;
    switch (AttributeValue.get_tag_value()) {
    case 20 : this_var->xom_obj[2].syntax = OM_S_TELETEX_STRING; break;
    default : this_var->xom_obj[2].syntax = OM_S_PRINTABLE_STRING; break;
    };
    AttributeValue.get_value(string_value); // get_value just returns pointers
					    // to internal store, so wer have
					    // to copy it.
    this_var->xom_obj[2].value.string.length = strlen(string_value);
    this_var->xom_obj[2].value.string.elements =
	pkc_malloc(xom_obj[2].value.string.length+1);
    memcpy(this_var->xom_obj[2].value.string.elements,
           string_value,
	   xom_obj[2].value.string.length);
    ((unsigned char *)xom_obj[2].value.string.elements)
				    [xom_obj[2].value.string.length] = 0;
    result = xom_obj;
    return 0;
}



AttributeValueAssertion & AttributeValueAssertion::operator=(
                                   const AttributeValueAssertion & o) {
    delete_xom_object();
    AttributeType = o.AttributeType;
    AttributeValue = o.AttributeValue;
    value_changed();
    return *this;
}

unsigned32 AttributeValueAssertion::hash(buffer_t & buffer) const {
    return AttributeValue.hash(buffer);
}

int AttributeValueAssertion::operator==(const AttributeValueAssertion & o)
const {
    unsigned char * s1;
    unsigned char * s2;
    size_t l1;
    size_t l2;
    int res;
    
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Comparing two AVAs"));
    if (!(AttributeType == o.AttributeType)) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Attribute types differ"));
	return 0;
    };
    AttributeValue.asn_octetstring::get_value(s1, l1);
    o.AttributeValue.asn_octetstring::get_value(s2, l2);
    res = attributeMatch(AttributeType,
                          s1, l1, s2, l2);
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "AttributeMatch returned %d", res));
    return res;
}

unsigned32 RelativeDistinguishedName::set_value(OM_object name) {
    AttributeValueAssertion * ava;
    unsigned32 error;
    int i;

    empty();
// A OM_object RDN is simply an array of AVAs...
    if (name[0].type != OM_CLASS)
	return pkc_s_bad_value;
    if ((name[0].syntax & OM_S_SYNTAX) != OM_S_OBJECT_IDENTIFIER_STRING)
	return pkc_s_bad_value;
    if ((name[0].value.string.length != OMP_LENGTH(DS_C_DS_RDN)) &&
        (name[0].value.string.length != OMP_LENGTH(DS_C_RELATIVE_NAME)))
	return pkc_s_bad_value;
    if ((memcmp(name[0].value.string.elements,
	       OMP_D_DS_C_DS_RDN,
	       name[0].value.string.length) != 0) &&
        (memcmp(name[0].value.string.elements,
 	        OMP_D_DS_C_RELATIVE_NAME,
	        name[0].value.string.length) != 0))
	return pkc_s_bad_value;
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "RDN.set_value(), parsing AVAs"));
    for (i=1; name[i].type != OM_NO_MORE_TYPES; i++) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      "RDN.set_value(), AVA[%d]", i));
	if (name[i].type != DS_AVAS) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "RDN.set_value(), type = %d, expecting DS_AVAS",
                          name[i].type));
	    return pkc_s_bad_value;
	};
	if ((name[i].syntax & OM_S_SYNTAX) != OM_S_OBJECT) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "RDN.set_value(), syntax = %d, expecting OM_S_OBJECT",
                          name[i].syntax));
	    return pkc_s_bad_value;
        };
	ava = new_child();
        if (error = ava->set_value(name[i].value.object.object)) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "RDN.set_value(), ava->set_value returned %d",
                          error));
            return error;
	};
    };
    value_changed();
    return 0;
}


unsigned32 RelativeDistinguishedName::set_value(const char * par) {
    int i;
    unsigned32 error;
    char ** ava_string;
    AttributeValueAssertion * ava;

    empty();

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "RDN::set_value(%s), calling unsegment...", par));
    unsegment(par, ',', 1, ava_string);
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "DEBUG: Returned from unsegment"));
    if (ava_string == NULL) return pkc_s_asn_no_more_data;
    error = 0;
    for (i=0; ava_string[i]; i++) {
	if (!error) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                          "Segment[%d] : \"%s\"", i, ava_string[i]));
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "Generating new child..."));
	    ava = new_child();
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "parsing AVA"));
	    error = ava->set_value(ava_string[i]);
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "AVA.set_value() returned 0x%X", error));
	};
	pkc_free(ava_string[i]);
    };
    pkc_free(ava_string);
    if (error) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                      "DEBUG: Error is set (0x%X)", error));
        return error;
    };
    value_changed();
    return 0;
}

unsigned32 RelativeDistinguishedName::display(buffer_t & buffer) const {
    int i;
    if ( !is_valid() ) {
        buffer.append("<Unset-RDN>");
        return 0;
    };
    for (i=0; i<child_count(); i++) {
        (*this)[i]->display(buffer);
        if (i<child_count()-1) buffer.append(",");
    };
    return 0;
}


unsigned32 RelativeDistinguishedName::delete_xom_object(void) const {
    int i;
    RelativeDistinguishedName * this_var = (RelativeDistinguishedName *)this; 

    if (xom_obj == NULL) return 0;
    for (i=0; i < child_count(); i++) {
	(*this)[i]->delete_xom_object();
    };
    pkc_free(this_var->xom_obj);    
    this_var->xom_obj = NULL;
    return 0;
}

unsigned32 RelativeDistinguishedName::xom_object(OM_object &result) const {
    int i;
    unsigned32 error;
    RelativeDistinguishedName * this_var = (RelativeDistinguishedName *)this; 

    if (xom_obj) delete_xom_object();
    this_var->xom_obj = (OM_descriptor *)pkc_malloc(
				sizeof(OM_descriptor) * (child_count() + 2));
    memset(this_var->xom_obj, 0, sizeof(OM_descriptor) * (child_count() + 2));
    this_var->xom_obj[0].type = OM_CLASS;
    this_var->xom_obj[0].syntax = OM_S_OBJECT_IDENTIFIER_STRING;
    this_var->xom_obj[0].value.string.length = OMP_LENGTH(DS_C_DS_RDN);
    this_var->xom_obj[0].value.string.elements = OMP_D_DS_C_DS_RDN;
    for (i=0; i < child_count(); i++) {
	this_var->xom_obj[i+1].type = DS_AVAS;
	this_var->xom_obj[i+1].syntax = OM_S_OBJECT;
	error = (*this)[i]->xom_object(xom_obj[i+1].value.object.object);
	if (error) {
	    delete_xom_object();
	    result = NULL;
	    return error;
	};
    };
    this_var->xom_obj[i+1].type = 0;
    this_var->xom_obj[i+1].syntax = 0;
    this_var->xom_obj[i+1].value.string.length = 0;
    this_var->xom_obj[i+1].value.string.elements = NULL;
    result = xom_obj;
    
    return 0;
}


RelativeDistinguishedName & RelativeDistinguishedName::operator=(
                                       const RelativeDistinguishedName & o) {
    AttributeValueAssertion * ava;
    int i;
    
    delete_xom_object();
    empty();
// Copy the RDNs from this name into the new name.

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Copying an RDN of %d AVAs...", o.child_count()));
    for (i=0; i<o.child_count();i++) {
	ava = new_child();
	*ava = *(o[i]);
    };
    value_changed();
    return *this;
    
}				       

unsigned32 RelativeDistinguishedName::hash(buffer_t & buffer) const {
    int i;
    for (i=0;i<child_count(); i++) {
	(*this)[i]->hash(buffer);
    };    
    return 0;
}


int RelativeDistinguishedName::operator==(const RelativeDistinguishedName & o)
const {
    int i;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Comparing RDNs, lengths %d & %d",
                  child_count(), o.child_count()));
    if (child_count() != o.child_count()) return 0;
    for (i=0; i < child_count(); i++) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      "Comparing AVA[%d]", i));
	if (!(*(o[i]) == *((*this)[i]))) return 0;
    };    
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "RDNs match"));
    return 1;
}

unsigned32 x500name::set_value(OM_object name) {
    RelativeDistinguishedName * rdn;
    unsigned32 error;
    int i;

    empty();

// A OM_object name is simply an array of RDNs...
    if (name[0].type != OM_CLASS)
	return pkc_s_bad_value;
    if ((name[0].syntax & OM_S_SYNTAX) != OM_S_OBJECT_IDENTIFIER_STRING)
	return pkc_s_bad_value;
    if (name[0].value.string.length != OMP_LENGTH(DS_C_DS_DN))
	return pkc_s_bad_value;
    if (memcmp(name[0].value.string.elements,
               OMP_D_DS_C_DS_DN,
               name[0].value.string.length) != 0)
	return pkc_s_bad_value;
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "X500name.set_value(), parsing RDNs"));
    for (i=1; name[i].type != OM_NO_MORE_TYPES; i++) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      "X500name.set_value(), RDNs[%d]", i));
	if (name[i].type != DS_RDNS) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "Type = %d, expecting DS_RDNS", name[i].type));
	    return pkc_s_bad_value;
	};
	if ((name[i].syntax & OM_S_SYNTAX) != OM_S_OBJECT) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "Syntax = %d, expecting OM_S_OBJECT",
                          name[i].syntax & OM_S_SYNTAX));
	    return pkc_s_bad_value;
	};
        rdn = new_child();
        if (error = rdn->set_value(name[i].value.object.object)) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "Name.set_value(), rdn->set_value returned %d",
                          error));
            return error;
	};
    };
    value_changed();
    return 0;
}

unsigned32 x500name::set_value(const char * par) {
    int i;
    unsigned32 error;
    char ** rdn_string;
    RelativeDistinguishedName * rdn;

    empty();
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "name::set_value(%s), calling unsegment...", par));
    if (strncmp(par, 
                GLOBAL_INTRODUCER, 
                strlen(GLOBAL_INTRODUCER)) == 0) {
	par += strlen(GLOBAL_INTRODUCER);
    } else if (strncmp(par, 
               LOCAL_INTRODUCER, 
               strlen(LOCAL_INTRODUCER)) == 0) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug2,
                      "Cell-relative names not yet supported"));
	return pkc_s_unimplemented;
    } else if (strncmp(par,
                       ALT_GLOBAL_INTRODUCER, 
                       strlen(ALT_GLOBAL_INTRODUCER)) == 0) {
	par += strlen(ALT_GLOBAL_INTRODUCER);
    } else {
	return pkc_s_bad_value;
    };
    unsegment(par, '/', 0, rdn_string);
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "returned from unsegment"));
    if (rdn_string == NULL) return pkc_s_asn_no_more_data;
    error = 0;
    for (i=0; rdn_string[i]; i++) {
        if (error == 0) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "Segment[%d] : \"%s\"", i, rdn_string[i]));
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "Generating RDN..."));
            rdn = new_child();
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "Parsing RDN..."));
            if (error = rdn->set_value(rdn_string[i])) {
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                              "RDN.set_value() returned error 0x%X", error));
            };

            pkc_free(rdn_string[i]);
        };
    };
    pkc_free(rdn_string);
    if (error) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                      "Error is set (0x%X)", error));
        return error;
    };
    value_changed();
    return 0;
}

unsigned32 x500name::display(buffer_t & buffer,
                             int dce_format) const {
    int i;
    if ( !is_valid() ) {
	buffer.append("<Unset-DN>");
	return 0;
    };
    if (dce_format) {
	buffer.append(DISPLAY_PREFIX_DCE);
    } else {
	buffer.append(DISPLAY_PREFIX_X500);
    };
    for (i=0; i<child_count(); i++) {
	(*this)[i]->display(buffer);
	if (i<child_count()-1) buffer.append("/");
    };
    return 0;
}

unsigned32 x500name::delete_xom_object(void) const {
    int i;
    x500name * this_var = (x500name *)this; 

    if (xom_obj == NULL) return 0;
    for (i=0; i < child_count(); i++) {
	(*this)[i]->delete_xom_object();
    };
    pkc_free(this_var->xom_obj);    
    this_var->xom_obj = NULL;
    return 0;
}


unsigned32 x500name::xom_object(OM_object &result) const {
    int i;
    unsigned32 error;
    x500name * this_var = (x500name *)this; 

    if (xom_obj) delete_xom_object();
    this_var->xom_obj = (OM_descriptor *)pkc_malloc(
				sizeof(OM_descriptor) * (child_count() + 2));
    memset(this_var->xom_obj, 0, sizeof(OM_descriptor) * (child_count() + 2));
    this_var->xom_obj[0].type = OM_CLASS;
    this_var->xom_obj[0].syntax = OM_S_OBJECT_IDENTIFIER_STRING;
    this_var->xom_obj[0].value.string.length = OMP_LENGTH(DS_C_DS_DN);
    this_var->xom_obj[0].value.string.elements = OMP_D_DS_C_DS_DN;
    for (i=0; i < child_count(); i++) {
	this_var->xom_obj[i+1].type = DS_RDNS;
	this_var->xom_obj[i+1].syntax = OM_S_OBJECT;
	error = (*this)[i]->xom_object(xom_obj[i+1].value.object.object);
	if (error) {
	    delete_xom_object();
	    result = NULL;
	    return error;
	};
    };
    this_var->xom_obj[i+1].type = 0;
    this_var->xom_obj[i+1].syntax = 0;
    this_var->xom_obj[i+1].value.string.length = 0;
    this_var->xom_obj[i+1].value.string.elements = NULL;
    result = xom_obj;
    
    return 0;
}

x500name & x500name::operator=(const x500name & o) {
    buffer_t temp_buffer;    
    unsigned32 error;
    
    int i;
    RelativeDistinguishedName * rdn;

/* Below is the naive implementation of assignment.  It works, but is	    */
/* slow.								    */
/*
    if (error = o.write(temp_buffer)) return *this;
    if (error = read(temp_buffer)) return *this;
*/
/* Here's the fast version...						    */

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Copying a name of %d RDNs...", o.child_count()));

    empty();
    for (i=0; i<o.child_count();i++) {
	rdn = new_child();
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      "Copying RDN[%d]...", i));
	*rdn = *(o[i]);
    };
    value_changed();
    return *this;
}


unsigned32 x500name::make_ancestor(unsigned rdn_count, x500name ** par) const
{

    RelativeDistinguishedName * rdn;
    int i;
    
    if (rdn_count > child_count()) return pkc_s_insufficient_rdns;
    
    *par = new x500name;
// Copy all but the last RDN from this name into the new name.
//

//    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
//                  "Creating a name of %d RDNs...", child_count()-rdn_count));

    for (i=0; i<child_count()-rdn_count;i++) {
	rdn = (*par)->new_child();
	*rdn = *((*this)[i]);
    };
    (*par)->value_changed();
    return 0;
}


unsigned32 x500name::hash(buffer_t & buffer) const {
    int i;
    buffer.empty();    
    for (i=0;i<child_count(); i++) {
	(*this)[i]->hash(buffer);
    };    
    return 0;
}

unsigned32 x500name::make_parent(x500name ** par) const {
    return make_ancestor(1, par);
}


int x500name::operator==(const x500name & o) const {
    int i;
    if (child_count() != o.child_count()) return 0;
// Compare the RDNs last-to-first, as we're most likely to be called with
// names close in the name-sapce, and this way we're likely to detect
// non-matches quicker.
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Comparing two names of length %d", child_count()));
    for (i=child_count()-1; i>=0; i--) {
	if (!(*(o[i]) == *((*this)[i]))) return 0;
    };    
    return 1;
}

unsigned common_root_length(const x500name & n1,
                            const x500name & n2) {
    unsigned i;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Finding common root of names, lengths %d & %d",
                  n1.child_count(), n2.child_count()));
    for (i=0; i<n1.child_count() && i < n2.child_count(); i++) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      "Comparing RDN[%d]", i));
	if (!(*(n1[i]) == *(n2[i]))) return i;
    };    
    return i;
}

unsigned32 compare_names(const x500name & n1,
                         const x500name & n2,
                         char * names_equal,
                         unsigned * n1_to_common,
                         unsigned * n2_to_common) {

    unsigned root_length;
    unsigned n1_count = n1.child_count();
    unsigned n2_count = n2.child_count();
    unsigned n1_rem;
    unsigned n2_rem;

    root_length = common_root_length(n1,n2);
    n1_rem = n1_count - root_length;
    n2_rem = n2_count - root_length;

    if (n1_to_common) *n1_to_common = n1_rem;
    if (n2_to_common) *n2_to_common = n2_rem;
    if (names_equal) *names_equal = ((n1_rem == 0) && (n2_rem == 0));
    return 0;

}

int x500name::is_descendant(const x500name & o) const {
    unsigned rdn_count = o.child_count();
    unsigned common_rdn_count = common_root_length(*this, o);

    return (common_rdn_count == rdn_count);
}

int x500name::is_ancestor(const x500name & o) const {
    unsigned rdn_count = child_count();
    unsigned common_rdn_count = common_root_length(*this, o);

    return (common_rdn_count == rdn_count);
}

