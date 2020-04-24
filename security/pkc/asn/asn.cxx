/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, In c.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: asn.cxx,v $
 * Revision 1.1.2.2  1996/11/13  20:29:39  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  23:22 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
 *
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:23 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
 *
 * Revision 1.1.2.1  1996/10/03  20:25:58  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:27:38  arvind]
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
 * Module: ASN.CXX
 *
 * Purpose: ASN.1 encode/decode classes
 *
 */

#include <dce/asn.h>
#include <dce/pkc_codesets.h>

extern "C" {
#include <stdio.h>
#include "pkc_common.h"
}

int tag_to_default_syntax(unsigned tag) {
    switch (tag) {
    case 18: 
    case 19: 
        return STRING_SYNTAX_ISO646;
    case 20:
        return STRING_SYNTAX_T61;
    case 21:
        return STRING_SYNTAX_C;
            // VideotexString encoding (T.100/T.101) not supported
    case 22:
        return STRING_SYNTAX_IA5;
    case 25:
    case 27:
        return STRING_SYNTAX_C;
            // Grasphic and General string encodings not supported
        
    };
    return STRING_SYNTAX_C;
}

unsigned32 syntax_to_local(const unsigned char * s, size_t sl,
                           char * &d,
                           int syntax, unsigned tag) {
    int i;
    
    if (d) pkc_free(d);

    if (syntax == STRING_SYNTAX_UNSPECIFIED) {
	syntax = tag_to_default_syntax(tag);
    };
    switch(syntax) {
    case STRING_SYNTAX_C:
        d = (char *)pkc_malloc(sl+1);
        memcpy(d, s, sl);
        d[sl] = '\0';
        break;
    case STRING_SYNTAX_ISO646:
// Convert ISO646 string to local C string.
// For ASCII machines this is simple
        d = (char *)pkc_malloc(sl+1);
	for (i=0; i<sl; i++) d[i] = iso646_to_local(s[i]);
        d[sl] = '\0';
        break;
    case STRING_SYNTAX_IA5:
// Convert IA5 string to local C string.
        d = (char *)pkc_malloc(sl+1);
	for (i=0; i<sl; i++) d[i] = ia5_to_local(s[i]);
        d[sl] = '\0';
        break;
    case STRING_SYNTAX_T61:
// Convert T61 string to local C string.  
        d = (char *)pkc_malloc(sl+1);
	for (i=0; i<sl; i++) d[i] = t61_to_local(s[i]);
        d[sl] = '\0';
        break;
    default:
// Default behavior for unrecognized syntax is to just copy without
// interpretation
        d = (char *)pkc_malloc(sl+1);
        memcpy(d, s, sl);
        d[sl] = '\0';
        break;
    };
    return 0;
}

unsigned32 local_to_syntax(const char * s, buffer_t &d,
                           int syntax, unsigned tag) {
    int i;
        
    d.empty();
    if (syntax == STRING_SYNTAX_UNSPECIFIED) {
	syntax = tag_to_default_syntax(tag);
    };
    switch(syntax) {
    case STRING_SYNTAX_C:
        d.append((char *)s);
        break;
    case STRING_SYNTAX_ISO646:
	for (i=0; s[i]!='\0'; i++) d.append(local_to_iso646(s[i]));
        break;
    case STRING_SYNTAX_IA5:
	for (i=0; s[i]!='\0'; i++) d.append(local_to_ia5(s[i]));
        break;
    case STRING_SYNTAX_T61:
	for (i=0; s[i]!='\0'; i++) d.append(local_to_t61(s[i]));
        break;
    default:
        d.append((char *)s);
        break;
    };
    return 0;    
}




static int initialized = 0;


#define ASN_INIT \
if (!initialized) {pkc_init_svc_handle(); initialized = 1;}


r_buffer_t::r_buffer_t(void) {
    data = NULL; data_length = 0;
    orig_data = NULL; orig_data_length = 0;
}

void r_buffer_t::set(void) {
    orig_data = data;
    orig_data_length = data_length;
}

void r_buffer_t::reset(void) {
    data = orig_data;
    data_length = orig_data_length;
}
									

buffer_t:: buffer_t(secure_flag_t sec) {
    buffer_length = 0;
    buffer = NULL;
    security = sec;
}

void buffer_t::secure(secure_flag_t sec) {
    security = sec;
}

void buffer_t::detach_buffer(void) {
    buffer_length = 0;
    buffer = NULL;
}

buffer_t::~buffer_t() {
    if (buffer) {
        if (security == SECURE) {
            zero();
        };
        pkc_free(buffer);
    };
}

unsigned32 buffer_t::clear(void) {
    data_length = 0;
    data = buffer;
    return 0;
}

unsigned32 buffer_t::zero(void) {
    memset(buffer, 0, buffer_length);
    return 0;
}

unsigned32 buffer_t::empty(void) { 
    return clear(); 
}

unsigned32 buffer_t::extend(size_t extra) {
    size_t data_offset = data-buffer;
    if (extra < DEFAULT_EXTENT) extra = DEFAULT_EXTENT;
    if (security == SECURE) {
        unsigned char * temp_buffer =
                (unsigned char *)pkc_malloc(buffer_length+extra);
        if (temp_buffer != NULL) {
            memcpy(temp_buffer, buffer, buffer_length);
            zero();
        };
        pkc_free(buffer);
        buffer = temp_buffer;
    } else {
        buffer = (unsigned char *)pkc_realloc(buffer, buffer_length+extra);
    };
    if (buffer) {
        buffer_length+=extra;
        data = buffer+data_offset;
        return 0;
    } else {
        return pkc_s_nomem;
    };
}

unsigned32 buffer_t::append(unsigned char c) {
    unsigned32 error;
    if (data_length + 1 > buffer_length) {
        if (error = extend(1)) return error;
    };
    data[data_length++] = c;
    return 0;
}

unsigned32 buffer_t::append(const r_buffer_t & o) {
    unsigned32 error;
    if (data_length + o.data_length > buffer_length) {
        if (error = extend(o.data_length)) return error;
    };
    memcpy(data+data_length, o.data, o.data_length);
    data_length+=o.data_length;
    return 0;
}

unsigned32 buffer_t::append(unsigned char * b, size_t l) {
    unsigned32 error;
    r_buffer_t temp_buffer;
    temp_buffer.data = b;
    temp_buffer.data_length = l;
    append(temp_buffer);
    temp_buffer.data = NULL;
    temp_buffer.data_length = 0;
    return 0;
}

unsigned32 buffer_t::append(char * b) {
    unsigned char * bb;
    bb = (unsigned char *)b;
    return append(bb, strlen(b));
}

unsigned32 asn_object::make_default(void) {
    if (!is_defaultable()) return pkc_s_asn_no_default_value;
    isDefault = 1;
    return 0;
}


int asn_object::operator == (const asn_object & o) const {
    buffer_t ob1, ob2;
    write(ob1);
    o.write(ob2);
    return ((ob1.data_length == ob2.data_length) &&
            (memcmp(ob1.data, ob2.data, ob1.data_length) == 0));
}



#ifdef MALLOCTRACE
#ifndef MALLOCTRACE_OUT
#define MALLOCTRACE_OUT stdout
#endif
#endif


#ifdef MALLOCTRACE

void real_pkc_malloc_status(const char * s1,
                            int s2) {
    fprintf(MALLOCTRACE_OUT, "\n###STAT$%d$%s\n", s2, s1);
    fflush(MALLOCTRACE_OUT);
}
#endif


#ifdef MALLOCTRACE

void real_pkc_malloc_check(void * p,
                           const char * s1,
                           int s2) {
    fprintf(MALLOCTRACE_OUT, "\n###CHEK$%p$%d$%s\n", p, s2, s1);
    fflush(MALLOCTRACE_OUT);
}
#endif

#ifndef MALLOCTRACE

unsigned32 pkc_adjust(unsigned char * &b,
                      size_t cur_size,
                      size_t new_size) {
    if (cur_size == new_size) return 0;
    pkc_free(b);
    b = (unsigned char *)pkc_malloc(new_size);
    if (b) return 0;
    else return pkc_s_nomem;
}
#else
unsigned32 real_pkc_adjust(unsigned char * &b,
                           size_t cur_size,
                           size_t new_size,
			   const char * s1,
			   int s2) {
    if (cur_size == new_size) return 0;
    real_pkc_free(b, s1, s2);
    b = (unsigned char *)real_pkc_malloc(new_size, s1, s2);
    if (b) return 0;
    else return pkc_s_nomem;
}
#endif


#ifndef MALLOCTRACE
void * pkc_realloc(void * p, size_t l) {
    void * ptr;
    ptr =realloc(p, l);
    return ptr;
}
#else
void * real_pkc_realloc(void * p, size_t l, const char * s1, int s2) {
    void * ptr;
    fprintf(MALLOCTRACE_OUT, "\n###REAL$%p$%d$%d$%s\n", p, l, s2, s1);
    fflush(MALLOCTRACE_OUT);
#undef realloc
    ptr =realloc(p, l);
#define realloc(x,y) error! realloc() called
    fprintf(MALLOCTRACE_OUT, "\n###REA1$%p$%d$%d$%s\n", ptr, l, s2, s1);
    fflush(MALLOCTRACE_OUT);
    return ptr;
}
#endif

#ifndef MALLOCTRACE
void * pkc_malloc(size_t l) {
    void * ptr;
    ptr = malloc(l);
    return ptr;
}
#else
void * real_pkc_malloc(size_t l, const char * s1, int s2) {
    void * ptr;
    fprintf(MALLOCTRACE_OUT, "\n###ALOC$%d$%d$%s\n", l, s2, s1);
    fflush(MALLOCTRACE_OUT);
#undef malloc
    ptr = malloc(l);
#define malloc(x) error! malloc() called
    fprintf(MALLOCTRACE_OUT, "\n###ALO1$%p$%d$%d$%s\n", ptr, l, s2, s1);
    fflush(MALLOCTRACE_OUT);
    return ptr;
}
#endif



#ifndef MALLOCTRACE
void * pkc_calloc(size_t n, size_t l) {
    void * ptr;
    ptr = calloc(n, l);
    return ptr;
}
#else
void * real_pkc_calloc(size_t n, size_t l, const char * s1, int s2) {
    void * ptr;
    fprintf(MALLOCTRACE_OUT, "\n###ALOC$%d$%d$%s\n", l*n, s2, s1);
    fflush(MALLOCTRACE_OUT);
#undef calloc
    ptr = calloc(n,l);
#define calloc(x) error! calloc() called
    fprintf(MALLOCTRACE_OUT, "\n###ALO1$%p$%d$%d$%s\n", ptr, l*n, s2, s1);
    fflush(MALLOCTRACE_OUT);
    return ptr;
}
#endif

#ifndef MALLOCTRACE
void pkc_free(void * b) {
    free(b);
}
#else
void real_pkc_free(void * b, const char * s1, int s2) {
    fprintf(MALLOCTRACE_OUT, "\n###FREE$%p$%d$%s\n", b, s2, s1);
    fflush(MALLOCTRACE_OUT);
#undef free
    free(b);
#define free(x) error! free() called
    fprintf(MALLOCTRACE_OUT, "\n###FRE1$%d$%s\n", s2, s1);
    fflush(MALLOCTRACE_OUT);
}
#endif



asn_object & asn_object::operator = (const asn_object & o) {
    buffer_t buffer(security);
    o.write(buffer);
    read(buffer);
    return *this;
}

asn_object::asn_object(const asn_object & o, secure_flag_t sec) {

    ASN_INIT;

    security = sec;
    encoding_valid = 0;
    value_valid = 0;
    containing_object = NULL;
    constant_object = 0;
// Initially, objects are present, not optional, not default and
// not defaultable.  Initially values are invalid until the object
// is read or set.
    present = 1;
    optional = 0;
    isDefault = 0;
    defaultable = 0;
    encode_buffer.secure(security);

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Created asn_object(%lu) via copy constructor",
		  (unsigned long)this));
    copy(o);
}

unsigned32 asn_object::copy(const asn_object & o) {
    unsigned32 st;
    buffer_t buffer(security);
    if (st = o.write(buffer)) return st;
    return read(buffer);
}

unsigned32 asn_object::hash(buffer_t & buffer) const {
    buffer.empty();
    return write(buffer);
}

unsigned32 asn_object::set_defaultable(void) {
    defaultable = 1;
    set_optional();
    invalidate_encoding();
    return check_default();
}

asn_object::asn_object(secure_flag_t sec) {

    ASN_INIT;

    security = sec;
    encoding_valid = 0;
    value_valid = 0;
    containing_object = NULL;
    constant_object = 0;
// Initially, objects are present, not optional, not default and
// not defaultable.  Initially values are invalid until the object
// is read or set.
    present = 1;
    optional = 0;
    isDefault = 0;
    defaultable = 0;
    encode_buffer.secure(security);
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Created asn_object(%lu)", (unsigned long)this));
}

asn_object::~asn_object() {
}

unsigned32 asn_object::parse_type_tag(r_buffer_t &data_buffer,
                                      unsigned &tag_value,
                                      asn_type_flags & flags) {
    return parse_type_tag(data_buffer.data, 
                          data_buffer.data_length,
                          tag_value,
                          flags);
}

unsigned32 asn_object::parse_type_tag(unsigned char * &data,
                                      size_t & data_length,
                                      unsigned &tag_value,
                                      asn_type_flags & flags) {
    if (data_length < 1) return pkc_s_asn_no_more_data;
    flags.constructed = *data & 0x20;
    switch (*data & 0xc0) {
        case 0 : flags.tag_class = UNIVERSAL_CLASS; break;
        case 0x40 : flags.tag_class = APPLICATION_CLASS; break;
        case 0x80 : flags.tag_class = CONTEXT_CLASS; break;
        case 0xc0 : flags.tag_class = PRIVATE_CLASS; break;
    };
    if ((*data & 0x1f) == 0x1f) {
// High tag-number
        data++,data_length--;
        if (data_length < 1) return pkc_s_asn_no_more_data;
        tag_value = 0;
        while (*data & 0x80) {
            tag_value = tag_value << 7;
            tag_value += *data & 0x7f;
            data++, data_length--;
            if (data_length < 1) return pkc_s_asn_no_more_data;
        };
    } else {
// Low tag-number
        tag_value = *data & 0x1f;
    };
    data++,data_length--;
    return 0;
}

unsigned32 asn_object::parse_length(r_buffer_t & data_buffer,
                                    size_t & value_length,
                                    asn_type_flags &flags) {
    return parse_length(data_buffer.data,
                        data_buffer.data_length,
                        value_length,
                        flags);
}

unsigned32 asn_object::parse_length(unsigned char * &data,
                                    size_t & data_length,
                                    size_t & value_length,
                                    asn_type_flags &flags) {
    unsigned length_length;

    if (data_length < 1) return pkc_s_asn_no_more_data;
    flags.indefinite = 0;

    if (*data == 0x80) {
// Indefinite-length encoding
	if (flags.is_primitive()) return pkc_s_asn_bad_indefinite;
	flags.indefinite = 1;
    } else {
// Definite-length encoding
	if ((*data & 0x80) == 0) {
            value_length = (*data & 0x7f);
        } else {
            value_length = 0;
	    length_length = (*data &0x7f);
            while (length_length) {
                data++, data_length--;    
                if (data_length < 1) return pkc_s_asn_no_more_data;
                value_length = value_length << 8;
                value_length += *data;
		length_length--;
            };
        };
    };
    data++, data_length--;    
    return 0;
}

int asn_object::check_EOC(r_buffer_t &data_buffer) {
    return check_EOC(data_buffer.data,
                     data_buffer.data_length);
}

int asn_object::check_EOC(unsigned char * &data,
                          size_t & data_length) {
    if (data_length < 2) return 0;
    if (data[0] == 0 && data[1] == 0) {
        data += 2;
        data_length -= 2;
        return 1;
    } else return 0;
}



int asn_object::check_type(unsigned char * &data,
                           size_t & data_length,
                           unsigned & ret_ber_tag_value,
                           asn_type_flags & flags) {
    unsigned char * loc_data = data;
    size_t loc_data_length = data_length;
    asn_type_flags loc_flags = flags;
    unsigned ber_tag_value;
    unsigned char octet;
    
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "asn_object::check_type(0x%2.2X)", data[0]));

    if (data_length < 1) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug6,
                  "DataLength < 1 - no room for tag"));
	goto fail;
    };
    flags.constructed = *data & 0x20;

    switch (*data & 0xc0) {
        case 0 : flags.tag_class = UNIVERSAL_CLASS; break;
        case 0x40 : flags.tag_class = APPLICATION_CLASS; break;
        case 0x80 : flags.tag_class = CONTEXT_CLASS; break;
        case 0xc0 : flags.tag_class = PRIVATE_CLASS; break;
    };

    if (flags.tag_class != loc_flags.tag_class) goto fail;

    if ((*data & 0x1f) == 0x1f) {
// High tag-number

        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      "High (>= 31) tag"));

        data++,data_length--;  // Move pointers to first char of
			       // extended tag-field.

        ber_tag_value = 0;
        do {
            if (data_length < 1) {
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                          "DataLength < 1 - no more data for tag encoding"));
		goto fail;
	    };
	    octet = *data; // Save this octet for while() test
            ber_tag_value = ber_tag_value << 7;
            ber_tag_value += octet & 0x7fu;
            data++, data_length--;
        } while (octet & 0x80u);

    } else {
// Low tag-number
        ber_tag_value = *data & 0x1f;
        data++,data_length--;
    };
    if (ber_tag_value != tag_value) goto fail;

    ret_ber_tag_value = ber_tag_value;
    return 1;

fail:
// Restore the pointers so we don't consume what we couldn't parse
    data = loc_data;
    data_length = loc_data_length;
    flags = loc_flags;

    return 0;
}


void asn_object::invalidate_encoding(void) {
    if (encoding_valid) {
    	if (!constant_object) {
            encoding_valid = 0;
        };
        if (containing_object) containing_object->invalidate_encoding();
    };
}

void asn_object::value_changed(void) {
    invalidate_encoding();
    set_valid();
    if (is_defaultable()) check_default();
}



unsigned32 asn_object::decode_value_indefinite(unsigned char * &data,
                                               size_t & max_data_length) {
    return pkc_s_asn_not_supported;
}

unsigned32 asn_object::encode_type_tag(buffer_t & data_buffer,
                                       unsigned tag_value,
                                       asn_type_flags flags) {
// The flags argument is used only to determine the type-class, and
// whether the encoding is constructed or not.
	unsigned char tag_byte;
    unsigned long tag_mask = 0x7f;
    unsigned byte_count = 0;
    unsigned long loc_tag = tag_value;

	tag_byte = flags.tag_class << 6;
	if (flags.constructed) tag_byte += 32;
	if (tag_value <= 30) {
		tag_byte += tag_value;
		data_buffer.append(tag_byte);
	} else {
		tag_byte += 31;
        data_buffer.append(tag_byte);
        while (loc_tag) {
            loc_tag = loc_tag & ~tag_mask;
            byte_count++;
            tag_mask = tag_mask << 7;
        };
        while (byte_count) {
            tag_byte = (unsigned char)((tag_value & tag_mask) >> (7 * byte_count));
            byte_count--;
            if (byte_count) tag_byte |= 0x80;
            data_buffer.append(tag_value);
        };
    };
    return 0;
}

unsigned32 asn_object::encode_length(buffer_t & data_buffer,
                                     size_t value_length) {
    if (value_length <= 127) {
        data_buffer.append(value_length);
    } else {
        if (value_length <= 255) {
            data_buffer.append((unsigned char)0x81);
            data_buffer.append((unsigned char)value_length);
        } else if (value_length <= 0xfffful) {
            data_buffer.append((unsigned char)0x82);
            data_buffer.append(
                    (unsigned char)((value_length >> 8) & 0xfful));
            data_buffer.append(
                    (unsigned char)(value_length & 0xfful));
        } else if (value_length <= 0xfffffful) {
            data_buffer.append((unsigned char)0x83);
            data_buffer.append(
                    (unsigned char)((value_length >> 16) & 0xfful));
            data_buffer.append(
                    (unsigned char)((value_length >> 8) & 0xfful));
            data_buffer.append(
                    (unsigned char)(value_length & 0xfful));
        } else {
            data_buffer.append((unsigned char)0x84);
            data_buffer.append(
                    (unsigned char)((value_length >> 24) & 0xfful));
            data_buffer.append(
                    (unsigned char)((value_length >> 16) & 0xfful));
            data_buffer.append(
                    (unsigned char)((value_length >> 8) & 0xfful));
            data_buffer.append(
                    (unsigned char)(value_length & 0xfful));
        };
    };
    return 0;
}



unsigned32 asn_object::write(buffer_t & buffer) const {
    unsigned32 error;
    size_t original_offset = buffer.data - buffer.buffer;
    asn_type_flags type_flags;
    asn_object * var_this = (asn_object *)this;

    if (is_optional() && is_default()) return 0;
	// Suppress BER for default-values objects

// XXX
    if (is_optional() && !is_valid()) return 0;
	// Suppress BER for unset optional objects

    type_flags.constructed  = is_constructed();
    type_flags.indefinite = 0;
    type_flags.tag_class = tag_class;
    if (!encoding_valid) {
    	var_this->encode_buffer.clear();
	if (!is_default()) {
	    if (error = var_this->calculate_value(var_this->encode_buffer))
		return error;
	} else {
	    if (error = var_this->calculate_default_value(var_this->encode_buffer))
		return error;
	};
        var_this->encoding_valid = 1;
    };
    if (error = var_this->encode_type_tag(buffer,
					  var_this->tag_value,
					  type_flags))
        return error;
    if (error = var_this->encode_length(buffer,
					var_this->encode_buffer.data_length))
        return error;
    return buffer.append(encode_buffer);
}


unsigned32 asn_object::calculate_default_value(buffer_t & data) {
    return pkc_s_asn_no_default_value;
}

unsigned32 asn_object::read(r_buffer_t & data) {
    unsigned32 error;
    asn_type_flags type_flags;
    size_t value_length;
    type_flags.constructed  = is_constructed();
    type_flags.indefinite = 0;
    type_flags.tag_class = tag_class;

#ifdef DCE_DEBUG
	if (data.data_length >= 3) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "asn_object::read(starting %2.2X %2.2X %2.2X)",
                          data.data[0], data.data[1], data.data[2]));
	} else {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "asn_object::read(starting with %2.2X)",
                          data.data[0]));
	};
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      ".....: looking for tag value %2.2X, class %d",
                      tag_value, tag_class));
#endif

    if (check_type(data,
                   tag_value,
                   type_flags)) {

        tag_class = type_flags.tag_class;
        constructed = type_flags.constructed;

        if (error = parse_length(data,
                                 value_length,
                                 type_flags)) {

            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "asn_object::read() - parse_length failed(%x)",
                          error));
            return error;
	};
        if (type_flags.is_indefinite()) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                     "asn_object::read() - Decoding indefinite-length value"));
    	    error = decode_value_indefinite(data.data,
                                            data.data_length);
#ifdef DCE_DEBUG
	    if (error) {
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                              "asn_object::read() - dec_val_indef returned %x",
                              error));
            };
#endif

        } else {
            data.data_length -= value_length;
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                        "asn_object::read() - Decoding definite-length value"));
            error = decode_value_definite(data.data, value_length);
#ifdef DCE_DEBUG
	    if (error) {
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                              "asn_object::read() - dec_val_def returned 0x%x",
                              error));
            };
#endif
        };
	present = 1;
    	return error;
    } else {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      "asn_object::read(), check_type fail.  Is it optional?"));
    	if (is_optional()) {
    	    present = 0;
    	    if (is_defaultable()) {
        		isDefault = 1;
    	    };
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                          "asn_object::read(), optional -> OK"));
    	    return 0;
    	} else {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug6,
                          "asn_object::read(), ~optional -> fail"));
    	    return pkc_s_asn_wrong_type;
    	};
    };
}



int asn_object::display_quals(buffer_t & buffer) const {
    int v = is_valid();
    int o = is_optional();
    if (!v) {
	buffer.append("Invalid");
        if (o) {
            buffer.append("(optional)");
        };
        return 0;
    } else
    return 1;
}

unsigned32 asn_object::display(buffer_t & buffer) const {
    display_quals(buffer);    
    buffer.append("<ASN_OBJECT>");
    return 0;
}

unsigned32 asn_object::set_optional(char opt) {
    optional = opt;
    return 0;
}

unsigned32 asn_object::set_valid(char opt) {
    value_valid = opt;
    if (value_valid && containing_object)
	return containing_object->set_valid();
	// Any valid components within an object should make the object as a
	// whole valid.
    return 0;
}

unsigned32 asn_object::check_default() {
    return pkc_s_asn_no_default_value;
}



unsigned32 asn_integer::check_default(void) {
    if (is_defaultable()) {
        if (is_valid()) {
            isDefault = (data == default_data);
	} else {
// Defaultable unset objects take their default values
	    isDefault = 1;
	};
    } else isDefault = 0;
    return 0;
}

void asn_integer::cons_int(void) {
    tag_class = UNIVERSAL_CLASS;
    tag_value = 0x02;
    value_type = SMALLINT;
    data = 0;
    default_data = 0;
    data_buffer = NULL;
    data_length = 0;
}


asn_integer & asn_integer::operator = (signed32 i) {
    set_value(i);
    return *this;
}

asn_integer & asn_integer::operator = (const asn_integer & o) {
    unsigned char * data_buf;
    size_t data_len;
    signed32 val;
    
    switch (o.value_type) {
    case SMALLINT:
        if (o.get_value(val)) return *this;
	set_value(val);
	break;
    case LARGEINT:
	if (o.get_value(data_buf, data_len)) return *this;
	set_value(data_buf, data_len);
	break;
    default:
	break;
    };
    return *this;
}


unsigned32 asn_integer::calculate_value(buffer_t & buf) const {

    if (!is_valid()) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                      "Calculate_value() for unset asn_integer"));
        return pkc_s_asn_unset;
    };
    if (value_type == SMALLINT) {
        if (data >= 0) {
            if (data > 0x7ffffful)
                buf.append((unsigned char)((data >> 24) & 0xff));
            if (data > 0x7ffful)
                buf.append((unsigned char)((data >> 16) & 0xff));
            if (data > 0x7ful)
                buf.append((unsigned char)((data >> 8) & 0xff));
            buf.append((unsigned char)(data & 0xff));
        } else {
            if (((data & 0xff000000ul) != 0xff000000ul) ||
                ((data & 0x800000ul) == 0))
                buf.append((unsigned char)((data >> 24) & 0xff));
            if (((data & 0xffff0000ul) != 0xffff0000ul) ||
                ((data & 0x8000ul) == 0))
                buf.append((unsigned char)((data >> 16) & 0xff));
            if (((data & 0xffffff00ul) != 0xffffff00ul) ||
                ((data & 0x80) == 0))
                buf.append((unsigned char)((data >> 8) & 0xff));
            buf.append((unsigned char)(data & 0xff));
        };
    } else {
        int i;
        for (i = 0; i < data_length; i++) {
            buf.append(data_buffer[i]);
        };
    };
    return 0;
}


unsigned32 asn_integer::calculate_default_value(buffer_t & buf) {
    if (!is_defaultable()) return pkc_s_asn_no_default_value;
    if (default_data >= 0) {
	if (default_data > 0x7ffffful)
	    buf.append((unsigned char)((default_data >> 24) & 0xff));
	if (default_data > 0x7ffful)
	    buf.append((unsigned char)((default_data >> 16) & 0xff));
	if (default_data > 0x7ful)
	    buf.append((unsigned char)((default_data >> 8) & 0xff));
	buf.append((unsigned char)(default_data & 0xff));
    } else {
	if (((default_data & 0xff000000ul) != 0xff000000ul) ||
	    ((default_data & 0x800000ul) == 0))
	    buf.append((unsigned char)((default_data >> 24) & 0xff));
	if (((default_data & 0xffff0000ul) != 0xffff0000ul) ||
	    ((default_data & 0x8000ul) == 0))
	    buf.append((unsigned char)((default_data >> 16) & 0xff));
	if (((default_data & 0xffffff00ul) != 0xffffff00ul) ||
	    ((default_data & 0x80) == 0))
	    buf.append((unsigned char)((default_data >> 8) & 0xff));
	buf.append((unsigned char)(default_data & 0xff));
    };
    return 0;
}

unsigned32 asn_integer::set_value(signed32 i) {
    data = i;
    value_type = SMALLINT;
    value_changed();
    return 0;
}

unsigned32 asn_integer::set_default_value(signed32 i) {
    default_data = i;
    set_defaultable();
    return 0;
}


unsigned32 asn_integer::get_value(signed32 & i) const {
    if (is_default()) i = default_data;
    else {
        if (!is_valid()) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                          "Signed get_value() for unset asn_integer"));
            return pkc_s_asn_unset;
        };
        if (value_type != SMALLINT) return pkc_s_asn_value_overflow;
        i = data;
    };
    return 0;
}


unsigned32 asn_integer::get_value(unsigned32 & i) const {
    signed32 val;
    if (is_default()) val = default_data;
    else {
        if (!is_valid()) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                          "Unsigned get_value() for unset asn_integer"));
            return pkc_s_asn_unset;
        };
        if (value_type != SMALLINT) return pkc_s_asn_value_overflow;
        val = data;
    };
    if (val < 0) return pkc_s_asn_value_overflow;
    i = (unsigned32)val;
    return 0;
}

unsigned32 asn_integer::set_value(const unsigned char * b, size_t l) {
    unsigned32 error;
    value_type = LARGEINT;
    if (error = pkc_adjust(data_buffer, data_length, l))
        return error;
    data_length = l;
    memcpy(data_buffer, b, l);
    value_changed();
    return 0;
}


unsigned32 asn_integer::get_value(unsigned char * & b, size_t & l) const {
    if (is_default()) return pkc_s_asn_value_overflow;
    if (!is_valid()) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                      "Long get_value() for unset asn_integer"));
        return pkc_s_asn_unset;
    };
    if (value_type == SMALLINT) return pkc_s_asn_value_overflow;
    b = data_buffer;
    l = data_length;
    return 0;
}

unsigned32 asn_integer::decode_value_definite(
                            unsigned char * &b,
                            size_t & l) {
    unsigned32 error;
    int i;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Decoding an integer object"));

    if (l == 0) return pkc_s_asn_no_more_data;
    if (l > 4) {
        value_type = LARGEINT;
        error = pkc_adjust(data_buffer, data_length, l);
        if (error) return error;
	data_length = l;
        memcpy(data_buffer, b, l);
    } else {
        value_type = SMALLINT;
        data = 0;
        if (b[0] & 0x80) data = 0xfffffffful;

        for (i=0; i<l; i++) {
            data = data << 8;
            data = data | b[i];
        };
    };
    b += l;
    l = 0;
    value_changed();
    return 0;
}


unsigned32 asn_integer::display(buffer_t & buffer) const {
    if (display_quals(buffer)) {
        if (value_type == SMALLINT) {
       	    buffer.append("<INT>(");
            buffer.append_dec((signed long)data);
            buffer.append(")");
        } else buffer.append("<LARGE-INT>");
    } else buffer.append("<INT>");
    return 0;
}



// The data_buffer inside an octetstring has an extra trailing null byte.
// This allows the octetstring code to be trivially re-used for
// character-strings, saving a copy operation to add the null byte.
void asn_octetstring::cons_oct(void) {
    tag_class = UNIVERSAL_CLASS;
    tag_value = 4;
    constructed = 0; // Default, although we can cope with
                     // constructed octet-strings too.
    data_buffer = (unsigned char *)pkc_malloc(1);
    data_buffer[0] = 0;
    default_data_buffer = (unsigned char *)pkc_malloc(1);
    default_data_buffer[0] = 0;
    data_length = 0;
}

asn_octetstring::~asn_octetstring() {
    pkc_free(data_buffer);
    pkc_free(default_data_buffer);
}

unsigned32 asn_octetstring::check_default(void) {
    if ( is_defaultable() ) {
        if ( is_valid() ) {
            isDefault = (data_length == default_data_length) &&
                         (memcmp(data_buffer,
                                 default_data_buffer,
                                 data_length) == 0);
        } else {
            isDefault = 1;
	};
    } else isDefault = 0;
    return 0;
}

unsigned32 asn_octetstring::set_default_value(const unsigned char * b,
                                              size_t l) {
    unsigned32 error;
    if (error = pkc_adjust(default_data_buffer, default_data_length+1, l+1))
        return error;
    default_data_length = l;
    memcpy(default_data_buffer, b, l);
    default_data_buffer[l] = 0;
    return set_defaultable();
}

unsigned32 asn_octetstring::set_value(const unsigned char * b, size_t l) {
    unsigned32 error;
    if (error = pkc_adjust(data_buffer, data_length+1, l+1))
        return error;
    data_length = l;
    memcpy(data_buffer, b, l);
    data_buffer[l] = 0;
    value_changed();
    return 0;
}

unsigned32 asn_octetstring::get_value(unsigned char * &b, size_t & l) const {
    if (is_default()) {
        b = default_data_buffer;
        l = default_data_length;
    } else {
        if (!is_valid()) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                          "Get_value() for unset asn_octetstring"));
            return pkc_s_asn_unset;
        };
        b = data_buffer;
        l = data_length;
    };
    return 0;
}

unsigned32 asn_octetstring::calculate_value(buffer_t & buffer) const {
    r_buffer_t temp_buffer;
    if ( !is_valid() ) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                      "Calculate_value() for unset asn_octetstring"));
        return pkc_s_asn_unset;
    };
    temp_buffer.data = data_buffer;
    temp_buffer.data_length = data_length;
    buffer.append(temp_buffer);
    return 0;
}

unsigned32 asn_octetstring::calculate_default_value(buffer_t & buffer) {
    r_buffer_t temp_buffer;
    if (!is_defaultable()) return pkc_s_asn_no_default_value;
    temp_buffer.data = default_data_buffer;
    temp_buffer.data_length = default_data_length;
    buffer.append(temp_buffer);
    return 0;
}

unsigned32 asn_octetstring::decode_value_definite(
                                unsigned char * &b,
                                size_t & l) {
    unsigned32 error;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "OS::decode_value_definite(length=%d)", l));

    if (is_primitive()) {
        if (error = pkc_adjust(data_buffer, data_length+1, l+1))
           return error;
        data_length = l;
        memcpy(data_buffer, b, l);
	data_buffer[l] = 0;
        b += l; l=0;
    } else {

// This works by creating temporary octetstring objects and parsing them,
// and concatenating the results. Inefficient maybe, but it should work.
	asn_octetstring substring;

	buffer_t string_buffer;
	r_buffer_t input_buffer;
	unsigned char * substring_ptr;
	size_t substring_len;

//	substring.set_tag(tag_value); // Change the tag_value of the substring
//				      // to match our own.
//  Incorrect, apparently (x.209, section 23, example)
//
	string_buffer.clear();
	while (l > 0) {
	    input_buffer.data = b;
	    input_buffer.data_length = l;
	    error = substring.read(input_buffer);
	    if (error) {
		string_buffer.clear();
		return error;
	    };
	    b = input_buffer.data;
	    l = input_buffer.data_length;
// Append the string's value to the buffer we're building...
	    error = substring.get_value(substring_ptr,
					substring_len);
	    if (error) {
		string_buffer.clear();
		return error;
	    };
	    string_buffer.append(substring_ptr, substring_len);
	};
	if (error = pkc_adjust(data_buffer,
			       data_length+1,
			       string_buffer.data_length+1))
	    return error;
	data_length = string_buffer.data_length;
	memcpy(data_buffer, string_buffer.data, data_length);
	data_buffer[data_length] = 0;
    };
    constructed = 0; // Reset the object so that it generates primitive BER
    value_changed();
    return 0;
    
}


unsigned32 asn_octetstring::decode_value_indefinite(
                                unsigned char * &b,
                                size_t & l) {
    unsigned32 error;
// This works by creating temporary octetstring objects and parsing them,
// and concatenating the results. Inefficient maybe, but it should work.
    asn_octetstring substring;

    buffer_t string_buffer;
    r_buffer_t input_buffer;
    unsigned char * substring_ptr;
    size_t substring_len;
    int done = 0;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "OS::decode_value_indefinite(length=%d)",l));

//    substring.set_tag(tag_value); // Change the tag_value of the substring
//				  // to match our own.
// Again, apparently wrong.
// 
    string_buffer.clear();
    while (!done) {
        if (check_EOC(b,
                      l)) {
            done = 1;
        } else {
// Next component not EOC.  Parse an octet-string...
            input_buffer.data = b;
            input_buffer.data_length = l;
            error = substring.read(input_buffer);
            if (error) {
                string_buffer.clear();
                return error;
            };
            b = input_buffer.data;
            l = input_buffer.data_length;
// Append the string's value to the buffer we're building...
            error = substring.get_value(substring_ptr,
                                        substring_len);
            if (error) {
                string_buffer.clear();
                return error;
            };
            string_buffer.append(substring_ptr, substring_len);
        };
    };
    if (error = pkc_adjust(data_buffer,
                           data_length+1,
                           string_buffer.data_length+1))
        return error;
    data_length = string_buffer.data_length;
    memcpy(data_buffer, string_buffer.data, data_length);
    data_buffer[data_length] = 0;
    constructed = 0; // Reset the object so that it generates primitive BER
    value_changed();

    return 0;
}

char itox(unsigned char c) {
    if (c<=9) return c + '0';
    if (c<=0xf) return c + 'a';
    return '?';
}

unsigned32 asn_octetstring::display(buffer_t & buffer) const {
    int i;
    if (display_quals(buffer)) {
        buffer.append("<OCTETSTRING>( ");
        for (i=0;i<data_length;i++) {
            if (i%2) buffer.append((unsigned char)itox((data_buffer[i]>>4)&0xff));
            else {
                buffer.append((unsigned char)itox(data_buffer[i]&0xff));
                buffer.append(" ");
            };
        };
        buffer.append(")");
    } else {
        buffer.append("<OCTETSTRING>");
    };
    return 0;
}


void asn_boolean::cons_boole(void) {
    tag_class = UNIVERSAL_CLASS;
    tag_value = 1;
    data = 0;
    default_data = 0;
}


unsigned32 asn_boolean::check_default(void) {
    if ( is_defaultable() ) {
        if (is_valid() ) {
            isDefault = (data == default_data);
        } else {
            isDefault = 1;
        };
    } else isDefault = 0;
    return 0;
}

asn_boolean & asn_boolean::operator = (char c) {
    set_value(c);
    return *this;
}


unsigned32 asn_boolean::set_value(char c) {
    data = c;
    value_changed();
    return 0;
}

unsigned32 asn_boolean::set_default_value(char c) {
    default_data = c;
    set_defaultable();
    return 0;
}

unsigned32 asn_boolean::get_value(char & c) const {
    if (is_default()) c = (default_data != 0);
    else {
        if (!is_valid()) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                          "get_value() for unset asn_boolean"));
            return pkc_s_asn_unset;
        };
        c = (data != 0);  // re-normalize C's boolean representation
    };
    return 0;
}

unsigned32 asn_boolean::calculate_value(buffer_t & buf) const {
    if (!is_valid()) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                      "Calculate_value() for unset asn_boolean"));
        return pkc_s_asn_unset;
    };
    if (data) buf.append(0xff);
    else buf.append((unsigned char)0);
    return 0;
}

unsigned32 asn_boolean::calculate_default_value(buffer_t & buf) {
    if (!is_defaultable()) return pkc_s_asn_no_default_value;
    if (default_data) buf.append(0xff);
    else buf.append((unsigned char)0);
    return 0;
}

unsigned32 asn_boolean::decode_value_definite(
                            unsigned char * &b,
                            size_t & l) {
    if (l == 0) return pkc_s_asn_no_more_data;
    if (l != 1) return pkc_s_asn_invalid_value;

    data = (b[0]?0xff:0);
    b++, l=0;
    value_changed();
    return 0;
}

unsigned32 asn_boolean::display(buffer_t & buffer) const {
    if (display_quals(buffer)) {
	buffer.append("<BOOLEAN>(");
        if (data) buffer.append((unsigned char)'t');
        else buffer.append((unsigned char)'f');
	buffer.append(")");
    } else buffer.append("<BOOLEAN>");
    return 0;
}

void asn_null::cons_null(void) {
    tag_class = UNIVERSAL_CLASS;
    tag_value = 5;
    constant_object = 1;
    set_valid();  // NULL is always valid
}

unsigned32 asn_null::calculate_value(buffer_t & buf) const {
    return 0;
}

unsigned32 asn_null::decode_value_definite(
                            unsigned char * &b,
                            size_t & l) {
    if (l != 0) return pkc_s_asn_invalid_value;
    return 0;
}





asn_bitstring::~asn_bitstring() {
    pkc_free(data_buffer);
    pkc_free(default_data_buffer);
}

unsigned32 asn_bitstring::display(buffer_t & buffer) const {
    display_quals(buffer);
    buffer.append("<ASN_BITSTRING>");
    return 0;
}

void asn_bitstring::cons_bit(void) {
    tag_class = UNIVERSAL_CLASS;
    tag_value = 3;
    constructed = 0; // Default, although we can cope with
                     // constructed bit-strings too
    data_buffer = NULL;
    data_length = 0;
    unused_bits = 0;
    default_data_buffer = NULL;
    default_data_length = 0;
    default_unused_bits = 0;
}

unsigned32 asn_bitstring::check_default(void) {
    if ( is_defaultable() ) {
        if ( is_valid() ) {
            isDefault = (data_length == default_data_length) &&
                	 (unused_bits = default_unused_bits) &&
                         (memcmp(data_buffer,
                                 default_data_buffer,
                                 data_length) == 0);
        } else {
            isDefault = 1;
        };
    } else isDefault = 0;
    return 0;
}

unsigned32 asn_bitstring::set_value(const unsigned char * b, unsigned32 l) {
    unsigned32 error;
    if (error = pkc_adjust(data_buffer,
                           data_length,
                           bit_to_byte_length(l)))
        return error;
    data_length = bit_to_byte_length(l);
    memcpy(data_buffer, b, data_length);
    unused_bits = unused_bit_count(l);
    data_buffer[data_length-1] &= used_bit_mask(l);
    value_changed();
    return 0;
}

unsigned32 asn_bitstring::set_default_value(const unsigned char * b,
                                            unsigned32 l) {
    unsigned32 error;
    if (error = pkc_adjust(default_data_buffer,
                           default_data_length,
                           bit_to_byte_length(l)))
        return error;
    default_data_length = bit_to_byte_length(l);
    memcpy(default_data_buffer, b, default_data_length);
    unused_bits = unused_bit_count(l);
    default_data_buffer[data_length-1] &= used_bit_mask(l);
    set_defaultable();
    return 0;
}


unsigned32 asn_bitstring::get_value(unsigned char * &b, unsigned32 & l) const {
    if (is_default()) {
        b = default_data_buffer;
        l = (default_data_length * 8) - default_unused_bits;
    } else {
        if ( !is_valid() ) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                          "Getvalue() for unset asn_bitstring"));
            return pkc_s_asn_unset;
        };
        b = data_buffer;
        l = (data_length * 8) - unused_bits;
    };
    return 0;
}

unsigned32 asn_bitstring::calculate_value(buffer_t & buffer) const {
    r_buffer_t temp_buffer;
    if ( !is_valid() ) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                      "Calculate_value() for unset asn_bitstring"));
        return pkc_s_asn_unset;
    };
    buffer.append(unused_bits);
    temp_buffer.data = data_buffer;
    temp_buffer.data_length = data_length;
    buffer.append(temp_buffer);
    return 0;
}

unsigned32 asn_bitstring::calculate_default_value(buffer_t & buffer) {
    r_buffer_t temp_buffer;
    if (!is_defaultable()) return pkc_s_asn_no_default_value;
    buffer.append(default_unused_bits);
    temp_buffer.data = default_data_buffer;
    temp_buffer.data_length = default_data_length;
    buffer.append(temp_buffer);
    return 0;
}

unsigned32 asn_bitstring::decode_value_definite(
                                unsigned char * &b,
                                size_t & l) {
    unsigned32 error;
    if (is_primitive()) {
        if (l == 0) return pkc_s_asn_no_more_data;
        if (error = pkc_adjust(data_buffer, data_length, l-1))
            return error;
        unused_bits = b[0]; b++, l--;
        data_length = l;
        memcpy(data_buffer, b, l);
        data_buffer[data_length-1] &= used_bit_mask(8-unused_bits);
        b += l; l=0;
    } else {

// This works by creating temporary bitstring objects and parsing them,
// and concatenating the results. Inefficient maybe, but it should work.
        asn_bitstring substring;

        buffer_t string_buffer;
        r_buffer_t input_buffer;
        unsigned char * substring_ptr;
        unsigned32 substring_len;
        int substring_non_integral = 0;

        string_buffer.clear();
        while (l > 0) {
            if (substring_non_integral) return pkc_s_asn_invalid_value;
            // Only the last substring may contain a non-integral
            // number of octets.
            input_buffer.data = b;
            input_buffer.data_length = l;
            error = substring.read(input_buffer);
            if (error) {
                string_buffer.clear();
                return error;
            };
            b = input_buffer.data;
            l = input_buffer.data_length;
// Append the string's value to the buffer we're building...
            error = substring.get_value(substring_ptr,
                                        substring_len);
            if (error) {
                string_buffer.clear();
                return error;
            };
            if (substring.unused_bits != 0) substring_non_integral = 1;
            string_buffer.append(substring_ptr,
                                 bit_to_byte_length(substring_len));
        };
        unused_bits = substring.unused_bits;
        if (error = pkc_adjust(data_buffer,
                               data_length,
                               string_buffer.data_length))
            return error;
        data_length = string_buffer.data_length;
        memcpy(data_buffer, string_buffer.data, data_length);
    };
    constructed = 0; // Reset the object so that it generates primitive BER
    value_changed();
    return 0;

}


unsigned32 asn_bitstring::decode_value_indefinite(
                                unsigned char * &b,
                                size_t & l) {
    unsigned32 error;
// This works by creating temporary bitstring objects and parsing them,
// and concatenating the results. Inefficient maybe, but it should work.
    asn_bitstring substring;

    buffer_t string_buffer;
    r_buffer_t input_buffer;
    unsigned char * substring_ptr;
    unsigned32 substring_len;
    int done = 0;
    int substring_non_integral = 0;

    string_buffer.clear();
    while (!done) {
        if (check_EOC(b,
                      l)) {
            done = 1;
        } else {
// Next component not EOC.  Parse an octet-string...
            if (substring_non_integral) return pkc_s_asn_invalid_value;
            // Only the last substring may contain a non-integral
            // number of octets.
            input_buffer.data = b;
            input_buffer.data_length = l;
            error = substring.read(input_buffer);
            if (error) {
                string_buffer.clear();
                return error;
            };
            b = input_buffer.data;
            l = input_buffer.data_length;
// Append the string's value to the buffer we're building...
            error = substring.get_value(substring_ptr,
                                        substring_len);
            if (error) {
                string_buffer.clear();
                return error;
            };
            if (substring.unused_bits != 0) substring_non_integral = 1;
            string_buffer.append(substring_ptr,
                                 bit_to_byte_length(substring_len));
        };
    };
    unused_bits = substring.unused_bits;
    if (error = pkc_adjust(data_buffer,
                           data_length,
                           string_buffer.data_length))
        return error;
    data_length = string_buffer.data_length;
    memcpy(data_buffer, string_buffer.data, data_length);
    constructed = 0; // Reset the object so that it generates primitive BER
    value_changed();

    return 0;
}


asn_objectidentifier::~asn_objectidentifier() {
    pkc_free(subidents);
    pkc_free(default_subidents);
}

unsigned32 asn_objectidentifier::check_default(void) {
    if ( is_defaultable() ) {
        if ( is_valid() ) {
            isDefault = (subident_count == default_subident_count) &&
                         (memcmp(subidents,
                                 default_subidents,
                                 subident_count * sizeof(unsigned long)) == 0);
        } else {
            isDefault = 1;
        };
    } else isDefault = 0;
    return 0;
}

void asn_objectidentifier::cons_oid(void) {
    tag_class = UNIVERSAL_CLASS;
    tag_value = 6;
    subidents = NULL;
    subident_count = 0;
    default_subidents = NULL;
    default_subident_count = 0;
}

unsigned32 asn_objectidentifier::set_value(const unsigned long * b, size_t l) {
    if (subidents) pkc_free(subidents);
    subidents = (unsigned long *)pkc_malloc(l * sizeof(unsigned long));
    if (subidents == NULL) return pkc_s_nomem;
    memcpy(subidents, b, l * sizeof(unsigned long));
    subident_count = l;
    value_changed();
    return 0;
}

unsigned32 asn_objectidentifier::set_value(const char * stringValue) {
    const char * cptr;
    int i;
    char buf[16];
    int wp;
    
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "OID::SetValue(%s)", stringValue));

    if (subidents) pkc_free(subidents);

    for (subident_count=1, cptr=stringValue; *cptr != 0; cptr++) {
    	if (*cptr == '.') subident_count++;
    };
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "%d subidents", subident_count));
    subidents =
        (unsigned long *)pkc_malloc(subident_count * sizeof(unsigned long));
    if (subidents == NULL) return pkc_s_nomem;

    for (i=0, cptr = stringValue; i<subident_count; i++) {
        wp = 0;
        while (*cptr != 0 && *cptr != '.') {
            if (wp < sizeof(buf)-1) buf[wp++] = *cptr;
            cptr++;
        };
        buf[wp] = 0;
        sscanf(buf, "%lu",  &subidents[i]);
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      "int-subident[%d] = %lu", i, subidents[i]));
        if (*cptr) cptr++;
    }
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Got value"));
    value_changed();
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Returning from OID::setValue()"));
    return 0;
}

unsigned32 asn_objectidentifier::set_default_value(const unsigned long * b,
                                                   size_t l) {
    if (default_subidents) pkc_free(default_subidents);
    default_subidents = (unsigned long *)pkc_malloc(l * sizeof(unsigned long));
    if (default_subidents == NULL) return pkc_s_nomem;
    memcpy(default_subidents, b, l * sizeof(unsigned long));
    default_subident_count = l;
    set_defaultable();
    return 0;
}


unsigned32 asn_objectidentifier::set_value(const asn_objectidentifier &o,
                                           const unsigned long * b,
                                           size_t l) {
    if (subidents) pkc_free(subidents);
    subidents = (unsigned long *)pkc_malloc(
				    (l +o.subident_count)* sizeof(unsigned long));
    if (subidents == NULL) return pkc_s_nomem;
    memcpy(subidents, o.subidents, o.subident_count * sizeof(unsigned long));
    memcpy(&subidents[o.subident_count], b, l*sizeof(unsigned long));
    subident_count = l + o.subident_count;
    value_changed();
    return 0;
}

unsigned32 asn_objectidentifier::set_default_value(
                            const asn_objectidentifier &o,
                            const unsigned long * b,
                            size_t l) {
    if (default_subidents) pkc_free(default_subidents);
    default_subidents = (unsigned long *)pkc_malloc(
			    (l +o.subident_count)* sizeof(unsigned long));
    if (default_subidents == NULL) return pkc_s_nomem;
    memcpy(default_subidents,
           o.subidents,
	   o.subident_count * sizeof(unsigned long));
    memcpy(&default_subidents[o.subident_count], b, l*sizeof(unsigned long));
    default_subident_count = l + o.subident_count;
    set_defaultable();
    return 0;
}

unsigned32 asn_objectidentifier::set_value(const asn_objectidentifier &o,
                                           unsigned long s) {
    if (subidents) pkc_free(subidents);
    subidents = (unsigned long *)pkc_malloc(
				    (o.subident_count+1)* sizeof(unsigned long));
    if (subidents == NULL) return pkc_s_nomem;
    memcpy(subidents, o.subidents, o.subident_count * sizeof(unsigned long));
    subidents[o.subident_count] = s;
    subident_count = o.subident_count + 1;
    value_changed();
    return 0;
    
}

unsigned32 asn_objectidentifier::set_default_value(
                                          const asn_objectidentifier &o,
                                          unsigned long s) {
    if (default_subidents) pkc_free(default_subidents);
    default_subidents = (unsigned long *)pkc_malloc(
                             (o.subident_count+1)* sizeof(unsigned long));
    if (default_subidents == NULL) return pkc_s_nomem;
    memcpy(default_subidents,
           o.subidents,
           o.subident_count * sizeof(unsigned long));
    default_subidents[o.subident_count] = s;
    default_subident_count = o.subident_count + 1;
    set_defaultable();
    return 0;
    
}

unsigned32 asn_objectidentifier::get_value(unsigned long *& b, size_t & l) const {
    if (is_default()) {
        b = default_subidents;
        l = default_subident_count;
    } else {
	if ( !is_valid() ) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                          "Get_value() for unset asn_objectidentifier"));
            return pkc_s_asn_unset;
        };
        b = subidents;
        l = subident_count;
    };
    return 0;
}

unsigned32 asn_objectidentifier::get_value(gss_OID & vo) const {
    buffer_t encoded_value;
    unsigned32 res;
    OM_uint32 min_stat;
        
    if ( !is_valid() ) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                      "Get_value() for unset asn_objectidentifier"));
        return pkc_s_asn_unset;
    };
// A gss_OID value is a C descriptor pointing to malloced storage containing
// the "V" part of the ASN.1 TLV encoding - i.e. the thing that
// calculate_value produces.

    encoded_value.clear();
    if (res = calculate_value(encoded_value)) return res;

// If the gss_OID we've been passed has something in it already, free it.
    if (vo) {
	gssdce_release_oid(&min_stat, &vo);
    };

// Allocate the descriptor for the gss_OID value...
    vo = (gss_OID)pkc_malloc(sizeof(gss_OID_desc));
    if (vo == NULL) return pkc_s_nomem;

// Transfer responsibility for the OID encoding to the gss_OID object...
    vo->length = encoded_value.data_length;    
    vo->elements = encoded_value.data;    
    encoded_value.detach_buffer();

    return 0;
}

unsigned32 asn_objectidentifier::encode_subident(unsigned long v,
                                                 buffer_t & buffer) const {
    int octets;
    unsigned long mask;
    unsigned char value;
    if (v == 0) {
	buffer.append((unsigned char)0);
    } else {
	for (octets=1,mask=0x7f;
	     (v & ~mask) != 0;
	     octets++,mask=(mask<<7)|0x7f);
// Now octets contains the number of octets necessary to encode the value,
        while (octets) {
            value = (v >> ((octets - 1)* 7)) & 0x7f;
            if (octets != 1) value |= 0x80;
            buffer.append( value );
            octets--;
        };
    };
    return 0;
}

unsigned32 asn_objectidentifier::calculate_value(buffer_t & buffer) const {
    unsigned32 error;
    unsigned long encode_value;
    int i;
    
    encode_value = 40 * subidents[0] + subidents[1];
    error = encode_subident(encode_value, buffer);
    if (error) return error;
    for (i=2;i<subident_count;i++) {
	error = encode_subident(subidents[i], buffer);
	if (error) return error;
    };    
    return 0;
}

unsigned32 asn_objectidentifier::calculate_default_value(buffer_t & buffer) {
    unsigned32 error;
    unsigned long encode_value;
    int i;

    if (!is_defaultable()) return pkc_s_asn_no_default_value;
    
    encode_value = 40 * default_subidents[0] + default_subidents[1];
    error = encode_subident(encode_value, buffer);
    if (error) return error;
    for (i=2;i<subident_count;i++) {
	error = encode_subident(default_subidents[i], buffer);
	if (error) return error;
    };    
    return 0;
}

unsigned32 asn_objectidentifier::decode_subident(unsigned char *&data,
                                                 size_t & data_length,
                                                 unsigned long & encoding)
const {
    encoding = 0ul;
    while (data_length > 0) {
        encoding = encoding << 7;
        encoding |= *data & 0x7f;
        data_length--;
        if ((*data++ & 0x80) == 0) return 0;
    };
    return pkc_s_asn_no_more_data;
}

unsigned32 asn_objectidentifier::decode_value_definite(
                            unsigned char * &data,
                            size_t & data_length) {
    int i;
    int error;
    size_t s_c = 0;
    unsigned long sub_encoding;
// Count the number of subidents.  Since the last (and only the last)
// byte of each subident encoding must have zero as its most significant
// bit, counting the number of such bytes in the encoding gives us the
// number of subident eoncdings, which is one less that the number of
// subidents (since the first 2 subidents are squished into a single
// encoding).
    for (i=0;i<data_length;i++) {
        if ((data[i] & 0x80) == 0) s_c++;
    };
    subident_count = s_c + 1;
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "OID::dvd(), subident count = %d", subident_count));
// Now subident_count contains the new subident count.  Re-arrange the
// buffer and parse.
    if (subidents) pkc_free(subidents);
    if (
	(subidents = (unsigned long *)pkc_malloc(
                                    subident_count * sizeof(unsigned long)))
        == NULL) return pkc_s_nomem;
    if (error = decode_subident(data, data_length, sub_encoding))
        return error;
    if (sub_encoding <= 39) {
        subidents[0] = 0;
        subidents[1] = sub_encoding;
    } else if (sub_encoding <= 79) {
        subidents[0] = 1;
        subidents[1] = sub_encoding - 40;
    } else {
        subidents[0] = 2;
        subidents[1] = sub_encoding - 80;
    };
    for (i=2;i<subident_count;i++) {
	if (error = decode_subident(data, data_length, sub_encoding))
	    return error;
        subidents[i] = sub_encoding;
    };
    value_changed();
    return 0;
}

unsigned32 asn_objectidentifier::display(buffer_t & buffer) const {
    int i;
    if ( display_quals(buffer) ) {
        buffer.append("<OID>(");
        for (i=0;i<subident_count;i++) {
            buffer.append_dec(subidents[i]);
            if (i < subident_count-1) buffer.append('.');
        };
        buffer.append(")");
    } else {
        buffer.append("<OID>");
    };
    return 0;
}

unsigned32 asn_objectidentifier::x500_display(buffer_t & buffer) const {
    int i;
    if (is_valid()) {
        for (i=0;i<subident_count;i++) {
            buffer.append_dec(subidents[i]);
            if (i < subident_count-1) buffer.append('.');
        };
    } else {
        buffer.append("<UNSET-OID>");
        return pkc_s_asn_unset;
    };
    return 0;
}

asn_objectidentifier & asn_objectidentifier::operator=(
                                              const asn_objectidentifier &o) {
    if (subidents) pkc_free(subidents);
    subidents = (unsigned long *)pkc_malloc(
				    (o.subident_count)* sizeof(unsigned long));
    if (subidents == NULL) return *this;
    memcpy(subidents, o.subidents, o.subident_count * sizeof(unsigned long));
    subident_count = o.subident_count;
    value_changed();
    return *this;
}

int asn_objectidentifier::operator==(const asn_objectidentifier &o) const {
    int i;
    if ( !is_valid() || !o.is_valid() ) return 0;
    // Unset values aren't equal to anything

    if (subident_count != o.subident_count) return 0;

    for (i=0; i < subident_count; i++) {
	if (subidents[i] != o.subidents[i]) return 0;
    };
    return 1;
}

unsigned32 buffer_t::append_dec(unsigned long i) {
    char buffer[20];
    sprintf(buffer,"%lu",i);
    return append(buffer);
}

unsigned32 buffer_t::append_dec(signed long i) {
    char buffer[20];
    sprintf(buffer,"%ld",i);
    return append(buffer);
}

unsigned32 buffer_t::append_hex(unsigned long i) {
    char buffer[20];
    sprintf(buffer,"%lx",i);
    return append(buffer);
}

unsigned32 buffer_t::append_hex(unsigned char i) {
    char buffer[20];
    sprintf(buffer,"%2.2x",i);
    return append(buffer);
}

unsigned32 asn_choice::calculate_value(buffer_t & b) const {
    return pkc_s_asn_internal_error;
}

unsigned32 asn_choice::read(r_buffer_t & data) {
    int i;
    unsigned32 error;
    for (i=0;i<contained_object_count;i++) {
        error = contained_object[i]->read(data);
        if (error == pkc_s_asn_wrong_type) continue;
        if (error) return error;
        set_value(i);
        return 0;
    };
    return pkc_s_asn_wrong_type;

}

unsigned32 asn_choice::write(buffer_t & data) const {
    int i;
    unsigned32 error;

    if (is_optional() && !is_valid()) return 0;
	// Suppress BER for unset optional objects

    if ( !is_valid() ) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                      "Write() for unset asn_choice"));
        return pkc_s_asn_unset;
    };
    return contained_object[choice]->write(data);
}

unsigned32 asn_choice::display(buffer_t & buffer) const {
    int error;
    display_quals(buffer);
    buffer.append("<CHOICE>(");
    buffer.append_dec((long)choice);
    buffer.append("){");
    if (error = contained_object[choice]->display(buffer))
        return error;
    buffer.append('}');
    return 0;
}

unsigned32 asn_choice::decode_value_definite(
                            unsigned char * &data,
                            size_t & data_length) {
    return pkc_s_asn_internal_error;
}

unsigned32 asn_choice::get_value(size_t & c) const {
    if (!value_valid) return pkc_s_asn_unset;
    c = choice;
    return 0;
}

unsigned32 asn_charstring::set_value(const char * s, int translate) {
    unsigned32 error;
    if (!translate) {
        return asn_octetstring::set_value((const unsigned char *)s, strlen(s));
    } else {
	buffer_t temp;
	if ((error = local_to_syntax(s, temp, syntax, tag_value)) != 0)
	    return error;
        return asn_octetstring::set_value(temp.data, temp.data_length);
    };
}

unsigned32 asn_charstring::set_default_value(const char * s, int translate) {
    unsigned32 error;
    if (!translate) {
        return asn_octetstring::set_default_value((const unsigned char *)s,
                                                  strlen(s));
    } else {
	buffer_t temp;
	if ((error = local_to_syntax(s, temp, syntax, tag_value)) != 0)
	    return error;
        return asn_octetstring::set_default_value(temp.data,
                                                  temp.data_length);
    };
}

// For environments whose natice characterset is not ASCII,
// these routines can perform native <-> ISO conversions.  The current
// implementation is dumb about character sets.
unsigned32 asn_charstring::read(r_buffer_t & data) {
    return asn_octetstring::read(data);
}

unsigned32 asn_charstring::write(buffer_t & data) const {
    return asn_octetstring::write(data);
}
    
asn_charstring::~asn_charstring() {
    if (temp_string) pkc_free(temp_string);
}


unsigned32 asn_charstring::get_value(char * &s, int translate) const {
    unsigned32 error;
    unsigned char * b;
    size_t l;
    asn_charstring * this_var = (asn_charstring *)this;
    
    if (error = asn_octetstring::get_value(b,l)) return error;
    if (!translate) {
        s = (char *)b;
        return 0;
    } else {
	if (this_var->temp_string) pkc_free(this_var->temp_string);
	this_var->temp_string = NULL;
	if ((error = syntax_to_local(b, l,
                                     this_var->temp_string,
                                     syntax,
				     tag_value)) != 0) return error;
	s = this_var->temp_string;
        return 0;
    };
}
 
unsigned32 asn_charstring::display(buffer_t & buffer) const {
    char * s;
    unsigned32 error;
    if (display_quals(buffer)) {
        buffer.append("<CHARSTRING>(");
        if (error = get_value(s)) return error;
        buffer.append(s);
        buffer.append(")");
    } else {
        buffer.append("<CHARSTRING>");
    };
    return 0;
}

unsigned32 asn_charstring::x500_display(buffer_t & buffer) const {
    char * s;
    unsigned32 error;
    if (is_valid()) {
        if (error = get_value(s)) return error;
        buffer.append(s);
    } else {
        buffer.append("<UNSET-CHARSTRING>");
	return pkc_s_asn_unset;	
    };
    return 0;
}

asn_utctime::asn_utctime(secure_flag_t sec) : asn_charstring(23, sec) {
    set_syntax(STRING_SYNTAX_IA5);
    century_start = 0;
    ignore_seconds = 0;
}

unsigned32 asn_utctime::set_century_rollover(unsigned s) {
    century_start = s;
    return 0;
}

unsigned32 asn_utctime::set_ignore_seconds(char t) {
    ignore_seconds = t;
    return 0;
}

unsigned32 asn_utctime::set_value(utc_t time) {

    struct tm timetm;
    long tns;
    struct tm inacctm;
    long ins;
    long tdf;
    char tdf_negative;
    int tdf_hour;
    unsigned tdf_min;
    
    if (utc_anytime(&timetm, &tns, &inacctm, &ins, &tdf, &time)) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                    "Error converting utc_t in asn_utctime::set_value(utc_t)"));
	return pkc_s_asn_value_overflow;
    };

    
    if (tdf_negative = (tdf < 0)) tdf = - tdf;    

        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                      "tdf = %d seconds", tdf));

    tdf_hour = tdf / (60 * 60);
    tdf_min = tdf - (tdf_hour * 60 * 60);

    if (tdf_negative) tdf_hour = - tdf_hour;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "tdf = %d hours, %d seconds", tdf_hour, tdf_min));

        
    return set_value(timetm.tm_year,
                     timetm.tm_mon + 1,
                     timetm.tm_mday,
                     timetm.tm_hour,
                     timetm.tm_min,
                     timetm.tm_sec,
                     tdf_hour,
                     tdf_min);

}

unsigned32 asn_utctime::get_value(utc_t & time) const {
    unsigned year;
    unsigned month;
    unsigned day;
    unsigned hour;
    unsigned minute;
    unsigned second;
    int tz_hour;
    unsigned tz_minute;
    error_status_t status;
    struct tm timetm;
    long tns;
    struct tm inacctm;
    char tdf_negative;
    long ins;
    long tdf;
        
    if (status = get_value(year,
                           month,
                           day,
                           hour,
                           minute,
                           second,
                           tz_hour,
                           tz_minute)) return status;

    timetm.tm_year = year - 1900; // tm_time is 1900-based
    timetm.tm_mon = month - 1;
    timetm.tm_mday = day;
    timetm.tm_hour = hour;
    timetm.tm_min = minute;
    timetm.tm_sec = second;

    tns = 0;
    
    if (tdf_negative = (tz_hour < 0)) tz_hour = - tz_hour;    
    tdf = ((tz_hour * 60) + tz_minute) * 60;
    if (tdf_negative) tdf = - tdf;

    inacctm.tm_sec = 0;
    inacctm.tm_min = 0;
    inacctm.tm_hour = 0;
    inacctm.tm_mday = 0;
    inacctm.tm_mon = 0;
    inacctm.tm_year = 0;
    inacctm.tm_wday = 0;
    inacctm.tm_isdst = 0;
    ins = 0;

    if (utc_mkanytime(&time,
                      &timetm,
                      tns,
                      &inacctm,
                      ins,
                      tdf))
	return pkc_s_asn_value_overflow;
    return 0;
}

static unsigned days_in_month(unsigned month, unsigned year) {
    unsigned days_in_month_noleap[12] =
       {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    unsigned days_in_month_leap[12] =
       {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    unsigned * mdays;

    if (month > 12) return 0;
    if (month == 0) return 0;
    month --;

    mdays = ( ( ((year % 4) == 0) && !((year % 400) == 0) )
              ? days_in_month_leap
              : days_in_month_noleap );

    return mdays[month];
}

unsigned32 asn_utctime::set_value(unsigned year,
                                  unsigned month,
                                  unsigned day,
                                  unsigned hour,
                                  unsigned minute,
                                  unsigned sec,
                                  int tz_hour,
                                  unsigned tz_min) {
    char date_buffer[18];
    char tz_buf[5];
    int hour_correction;
    int min_correction;
    int leap_year;

    
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Converting time."));
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "       Year: %u, Month: %u, Day: %u",
                  year, month, day));
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "       Hour: %u, Min: %u, Sec: %u",
                  hour, minute, sec));
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "       TZHour: %d, TZMin: %u",
                  tz_hour, tz_min));
    while (year >= 100) year -= 100;
    if (month > 12) return pkc_s_asn_value_overflow;
    if (day > 31) return pkc_s_asn_value_overflow;
    if (hour > 23) return pkc_s_asn_value_overflow;
    if (minute > 59) return pkc_s_asn_value_overflow;
    if (sec > 59) return pkc_s_asn_value_overflow;
    if (tz_min > 59) return pkc_s_asn_value_overflow;

// Convert to UTC before storing...

    hour_correction = - tz_hour;
    if (tz_hour >= 0) min_correction = -tz_min;
    else min_correction = tz_min;
    
    hour += hour_correction;
    minute += min_correction;
    while (minute >= 60) {
        hour ++;
	minute -= 60;
    };

   while (hour >= 24) {
      day++;
      hour -= 24;
   };

   while (day > days_in_month(month, year)) {
      day -= days_in_month(month, year);
      month++;
      if (month > 12) {
	year++;
	month -= 12;
      };
   };

// PKIX requires that seconds are omitted if they are zero, and
// recommends omitting them always.
    if ((sec == 0) || ignore_seconds)
	sprintf(date_buffer,
		"%2.2u%2.2u%2.2u%2.2u%2.2u",
		year, month, day,
		hour, minute);
    else
	sprintf(date_buffer,
		"%2.2u%2.2u%2.2u%2.2u%2.2u%2.2u",
		year, month, day,
		hour, minute, sec);


    if ((tz_hour == 0) && (tz_min == 0)) {
	strcat(date_buffer, "Z");
    } else {
        if (tz_hour < 0) {
            strcat(date_buffer, "-");
            tz_hour = - tz_hour;
	} else {
            strcat(date_buffer, "+");
        };
	if (tz_hour > 23)
	    return pkc_s_asn_value_overflow;
	if (tz_min > 59)
	    return pkc_s_asn_value_overflow;
	sprintf(tz_buf, "%2.2u%2.2u", tz_hour, tz_min);
	strcat(date_buffer,tz_buf);
    };

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Result = \"%s\"",
                  date_buffer));
    return asn_charstring::set_value(date_buffer, 1); // Encode into IA5 when storing
}


unsigned32 asn_utctime::get_value(unsigned &year,
                                  unsigned &month,
                                  unsigned &day,
                                  unsigned &hour,
                                  unsigned &minute,
                                  unsigned &second,
                                  int &tz_hour,
                                  unsigned &tz_minute) const {
    char * s;
    unsigned32 error;
    char str_year[4];
    char str_month[4];
    char str_day[4];
    char str_hour[4];
    char str_minute[4];
    char str_second[4];
    char str_tz_hour[4];
    char str_tz_minute[4];
    int tz_negative = 0;

    if (error = asn_charstring::get_value(s, 1)) return error;
                                                 // Encode into local format

    str_year[2] = '\n';
    str_year[3] = 0;
    str_month[2] = '\n';
    str_month[3] = 0;
    str_day[2] = '\n';
    str_day[3] = 0;
    str_hour[2] = '\n';
    str_hour[3] = 0;
    str_minute[2] = '\n';
    str_minute[3] = 0;
    str_second[2] = '\n';
    str_second[3] = 0;
    str_tz_hour[2] = '\n';
    str_tz_hour[3] = 0;
    str_tz_minute[2] = '\n';
    str_tz_minute[3] = 0;

    str_year[0] = s[0];
    str_year[1] = s[1];
    str_month[0] = s[2];
    str_month[1] = s[3];
    str_day[0] = s[4];
    str_day[1] = s[5];
    str_hour[0] = s[6];
    str_hour[1] = s[7];
    str_minute[0] = s[8];
    str_minute[1] = s[9];
    str_second[0] = '0'; str_second[1] = '0';
    switch (s[10]) {
    case 'Z':
        str_tz_hour[0] = '0';
        str_tz_hour[1] = '0';
        str_tz_minute[0] = '0';
        str_tz_minute[1] = '0';
        break;
    case '-':
        tz_negative = 1;
    case '+':
        str_tz_hour[0] = s[11];
        str_tz_hour[1] = s[12];
        str_tz_minute[0] = s[13];
        str_tz_minute[1] = s[14];
        break;
    default:
        str_second[0] = s[10];
        str_second[1] = s[11];
        switch (s[12]) {
        case 'Z':
            str_tz_hour[0] = '0';
            str_tz_hour[1] = '0';
            str_tz_minute[0] = '0';
            str_tz_minute[1] = '0';
            break;
        case '-':
            tz_negative = 1;
        case '+':
            str_tz_hour[0] = s[13];
            str_tz_hour[1] = s[14];
            str_tz_minute[0] = s[15];
            str_tz_minute[1] = s[16];
            break;
        default:
            return pkc_s_asn_value_overflow;
        };
    };


    if (sscanf(str_year, "%u", &year) != 1) {
        return pkc_s_asn_value_overflow;
    };
    if (sscanf(str_month, "%u", &month) != 1) {
        return pkc_s_asn_value_overflow;
    };
    if (sscanf(str_day, "%u", &day) != 1) {
        return pkc_s_asn_value_overflow;
    };
    if (sscanf(str_hour, "%u", &hour) != 1) {
        return pkc_s_asn_value_overflow;
    };
    if (sscanf(str_minute, "%u", &minute) != 1) {
        return pkc_s_asn_value_overflow;
    };
    if (sscanf(str_second, "%u", &second) != 1) {
        return pkc_s_asn_value_overflow;
    };
    if (sscanf(str_tz_hour, "%d", &tz_hour) != 1) {
        return pkc_s_asn_value_overflow;
    };
    if (sscanf(str_tz_minute, "%u", &tz_minute) != 1) {
        return pkc_s_asn_value_overflow;
    };
    if (tz_negative) tz_hour = - tz_hour;
    year += 1900;
    if (year < century_start) {
        year += 100;
    };
    return 0;
}


void asn_sequence::cons_seq(void) {
    tag_class = UNIVERSAL_CLASS;
    tag_value = 16;
}

unsigned32 asn_sequence::decode_value_definite(
        unsigned char * &data,
        size_t & data_length) {
    int i;
    int ii;
    unsigned32 error;
    r_buffer_t buffer;

#ifdef DCE_DEBUG
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Sequence::decodeValueDefinite()"));
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Parsing %d bytes, starting", data_length));
    for (ii=0;ii<data_length && ii<10;ii++) {
       DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                     "%2.2X", data[ii]));
    };
#endif

    buffer.data = data;
    buffer.data_length = data_length;

    for (i=0;i<contained_object_count; i++) {

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Sequence:: reading element %d of %d...",
                  i, contained_object_count));

        if (error = (*this)[i]->read(buffer)) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug1,
                          "Element[%d].read() returned %d",
                          i, error));
            return error;
        };
    }
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Finished reading %d elements of sequence",
                  contained_object_count));
    value_changed();
    data = buffer.data;
    data_length = buffer.data_length;
    return 0;
}

unsigned32 asn_sequence::decode_value_indefinite(
        unsigned char * &data,
        size_t & data_length) {
    int i;
    unsigned32 error;
    r_buffer_t buffer;

    buffer.data = data;
    buffer.data_length = data_length;

    for (i=0;i<contained_object_count; i++) {
        if (error = (*this)[i]->read(buffer)) {
            return error;
        };
    }
    if (!check_EOC(buffer)) return pkc_s_asn_wrong_type;

    value_changed();
    data = buffer.data;
    data_length = buffer.data_length;
    return 0;
}

unsigned32 asn_sequence::calculate_value(buffer_t & buffer) const {
    int i;
    unsigned32 error;


    for (i=0;i<contained_object_count; i++) {
        if (error = (*this)[i]->write(buffer)) return error;
    }
    return 0;
}

unsigned32 asn_sequence::display(buffer_t & buffer) const {
    int i;
    unsigned32 error;
    display_quals(buffer);
    buffer.append("<SEQ> { ");
    for (i=0;i<contained_object_count; i++) {
        if (error = (*this)[i]->display(buffer)) return error;
        buffer.append(", ");
    }
    buffer.append("}");
    return 0;
}



void asn_set::cons_set(void) {
    tag_class = UNIVERSAL_CLASS;
    tag_value = 17;
    component_present = NULL;
}

unsigned32 asn_set::decode_value_definite(
        unsigned char * &data,
        size_t & data_length) {
    int i;
    unsigned32 error;
    r_buffer_t buffer;
    int found_element;

    buffer.data = data;
    buffer.data_length = data_length;

// If we've not already made a component_present array, do it now.
    component_present = (char * )pkc_realloc(component_present,
                                    contained_object_count * sizeof(char));

// Set all elements absent, and reset to default values.
    for (i=0;i<contained_object_count; i++) {
        component_present[i] = 0;
        if ((*this)[i]->is_defaultable()) (*this)[i]->make_default();
    };

    while (buffer.data_length != 0) {
    	found_element = 0;
        for (i=0;i<contained_object_count; i++) {
            if (!component_present[i]) {
                if (error = (*this)[i]->read(buffer)) {
                    if (error != pkc_s_asn_wrong_type) return error;
                } else {
                    if ((*this)[i]->is_present()) {
// Don't count absent default-valued elements as having been read.
                        component_present[i] = 1;
                        found_element = 1;
                        break;
        		    };
        		};
            };
        };
    	if (!found_element) return pkc_s_asn_wrong_type;
    };
    for (i=0;i<contained_object_count;i++)  {
        if (!component_present[i]) {
            if ((*this)[i]->is_default()) component_present[i] = 1;
            if (!component_present[i]) return pkc_s_asn_no_default_value;
        };
    };
    value_changed();
    data = buffer.data;
    data_length = buffer.data_length;
    return 0;
}

unsigned32 asn_set::decode_value_indefinite(
        unsigned char * &data,
        size_t & data_length) {
    int i;
    unsigned32 error;
    r_buffer_t buffer;
    int found_element;
    int done;

    buffer.data = data;
    buffer.data_length = data_length;

// If we've not already made a component_present array, do it now.
    component_present = (char * )pkc_realloc(component_present,
                                    contained_object_count * sizeof(char));

// Set all elements absent, and reset to default values.
    for (i=0;i<contained_object_count; i++) {
        component_present[i] = 0;
        if ((*this)[i]->is_defaultable()) (*this)[i]->make_default();
    };

    done = 0;
    while (!done) {
        if (buffer.data_length == 0) return pkc_s_asn_no_more_data;
        found_element = 0;
        if (check_EOC(buffer)) {
            done = 1;
            continue;
        };
        for (i=0;i<contained_object_count; i++) {
            if (!component_present[i]) {
                if (error = (*this)[i]->read(buffer)) {
                    if (error != pkc_s_asn_wrong_type) return error;
                } else {
                    if ((*this)[i]->is_present()) {
// Don't count absent default-valued elements as having been read.
                        component_present[i] = 1;
                        found_element = 1;
                        break;
                    };
                };
            };
        };
        if (!found_element) return pkc_s_asn_wrong_type;
        done = check_EOC(buffer);
    };
    for (i=0;i<contained_object_count;i++)  {
        if (!component_present[i]) {
            if ((*this)[i]->is_default()) component_present[i] = 1;
        };
        if (!component_present[i]) return pkc_s_asn_no_default_value;
    };
    value_changed();
    data = buffer.data;
    data_length = buffer.data_length;
    return 0;
}

unsigned32 asn_set::calculate_value(buffer_t & buffer) const {
    int i;
    unsigned32 error;
    asn_set * this_var = (asn_set *)this;

    if (component_present == NULL) return 0;
    this_var->sort_elements();
    for (i=0;i<contained_object_count; i++) {
	if (component_present[i]) {
	    if (error = (*this)[i]->write(buffer)) return error;
	};
    }
    return 0;
}

unsigned32 asn_set::display(buffer_t & buffer) const {
    int i;
    unsigned32 error;

    display_quals(buffer);
    buffer.append("<SET> { ");
    if (component_present)
	for (i=0;i<contained_object_count; i++) {
	    if (component_present[i]) {
		if (error = (*this)[i]->display(buffer)) return error;
                buffer.append(", ");
            };
        };
    buffer.append("}");
    return 0;
}

asn_set::~asn_set() {
    if (component_present) pkc_free(component_present);
}


int asn_set::child_present(int i) {
    if (i < 0 || i >= contained_object_count) return 0;
    if (component_present == NULL) return 0;
    return component_present[i];
}

unsigned32 asn_set::set_child_present(asn_object * c) {
    int child;
    if ((child = find_child(c)) < 0) return pkc_s_asn_out_of_range;
    return set_child_present(child);
}

unsigned32 asn_set::set_child_present(int i) {
    int j;
    if (i < 0 || i >= contained_object_count) return pkc_s_asn_out_of_range;
    if (component_present == NULL) {
	component_present =
	    (char *)pkc_malloc(contained_object_count * sizeof(char));
	for (j=0; i<contained_object_count; i++)
	    component_present[j] = (i != j);
    } else component_present[i] = 1;
    return 0;
}


unsigned32 asn_set::set_child_absent(asn_object * c) {
    int child;
    if ((child = find_child(c)) < 0) return pkc_s_asn_out_of_range;
    return set_child_absent(child);
}

unsigned32 asn_set::set_child_absent(int i) {
    int j;
    if (i < 0 || i >= contained_object_count) return pkc_s_asn_out_of_range;
    if (component_present == NULL) {
	component_present =
	    (char *)pkc_malloc(contained_object_count * sizeof(char));
	for (j=0; i<contained_object_count; i++)
	    component_present[j] = 0;
    } else component_present[i] = 0;
    return 0;
}


asn_object * asn_constructed::operator [] (size_t i) const {
    if (i >= contained_object_count) return NULL;
    else return contained_object[i];
}

asn_constructed::~asn_constructed() {
    int i;
    if (contained_object == NULL) {
        contained_object_count = 0;  // Although it should already be zero
        return;
    };
    for (i=0; i< contained_object_count; i++) {
        contained_object[i]->containing_object = NULL;
        // Orphan our children
    };
    pkc_free(contained_object);
    contained_object = NULL;
    contained_object_count = 0;
// The actual child objects aren't managed by an asn_constructed object;
// Only the array of pointers is deallocated when the constructed object
// is destroyed.
}

void asn_constructed::register_child(asn_object * o) {
    contained_object =
            (asn_object **)pkc_realloc(
                    contained_object,
                    (contained_object_count+1) * sizeof(asn_object *));
    contained_object[contained_object_count++] = o;
    o->containing_object = this;
    value_changed();
    set_valid(0); // Registering an additional child invalidates
}


void asn_constructed::empty(void) {
    contained_object_count = 0;
    if (contained_object) pkc_free(contained_object);
    contained_object = NULL;
}

int asn_constructed::find_child(asn_object * c) {
    int i;
    for (i=0; i<contained_object_count; i++)
	if (contained_object[i] == c) return i;
    return -1;
}

void asn_sortable::swap_objects(int i, int j) {
    asn_object * temp;
    temp = contained_object[i];	
    contained_object[i] = contained_object[j];	
    contained_object[j] = temp;
}

unsigned32 asn_sortable::write(buffer_t & buffer) const {
    asn_sortable * var_this = (asn_sortable *)this;
    var_this->sort_elements();
    return asn_constructed::write(buffer);
}

void asn_sortable::sort_elements(void) {
// Run through the elements and sort them.
// Use a bubble-sort, as this is trivial to program, and we won't have too
// many elements.

    int i;
    int done = 0;

    presort_elements();
    if (contained_object_count == 0) return;
    while (!done) {
	done = 1;
	for (i=0;i < contained_object_count-1; i++) {
	    if (compare_objects(i, i+1) > 0) {
		swap_objects(i, i+1);
		done = 0;
	    };
	};
    };
}

int asn_set::compare_objects(int i, int j) {
    unsigned ti;
    unsigned tj;
    ti = contained_object[i]->get_tag_value();	
    tj = contained_object[j]->get_tag_value();	
    if (ti < tj) return -1;
    else if (ti > tj) return 1;
    else return 0;
}

void asn_set::swap_objects(int i, int j) {
    char temp;
    asn_sortable::swap_objects(i,j);
    if (component_present) {
	temp = component_present[i];
	component_present[i] = component_present[j];
	component_present[j] = temp;
    };
}


int asn_conformantstring::check_type(unsigned char * &data,
                                     size_t & data_length,
                                     unsigned & ret_ber_tag_value,
                                     asn_type_flags & flags) {
    unsigned char * loc_data = data;
    size_t loc_data_length = data_length;
    asn_type_flags loc_flags = flags;
    unsigned ber_tag_value;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "ConformantString()::CheckType()"));
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Data starts: %2.2x %2.2x %2.2x",
                  (unsigned)data[0], (unsigned)data[1], (unsigned)data[2]));
    if (data_length < 1) goto fail;
    flags.constructed = *data & 0x20;

    switch (*data & 0xc0) {
        case 0 : flags.tag_class = UNIVERSAL_CLASS; break;
        case 0x40 : flags.tag_class = APPLICATION_CLASS; break;
        case 0x80 : flags.tag_class = CONTEXT_CLASS; break;
        case 0xc0 : flags.tag_class = PRIVATE_CLASS; break;
    };

    if (flags.tag_class != loc_flags.tag_class) goto fail;

    if ((*data & 0x1f) == 0x1f) {
// High tag-number
        data++,data_length--;
        if (data_length < 1) return pkc_s_asn_no_more_data;
        ber_tag_value = 0;
        while (*data & 0x80) {
            ber_tag_value = ber_tag_value << 7;
            ber_tag_value += *data & 0x7f;
            data++, data_length--;
            if (data_length < 1) goto fail;
        };
    } else {
// Low tag-number
        ber_tag_value = *data & 0x1f;
    };
    data++,data_length--;
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "ConformantString() tag from BER is %d",
                  ber_tag_value));
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Supported values are 19, 20, 22, 23"));

    if ((ber_tag_value != 19) &&
        (ber_tag_value != 20) &&
        (ber_tag_value != 22) &&
        (ber_tag_value != 23)) goto fail;

    ret_ber_tag_value = ber_tag_value;
    return 1;

fail:
    data = loc_data;
    data_length = loc_data_length;
    flags = loc_flags;

    return 0;
}

unsigned32 asn_any::set_value(const asn_object & val) {
    unsigned32 error;
    value_encoding.empty();
    total_encoding.empty();
    if (error = val.write(total_encoding)) return error;
    total_encoding.set();
    error = read(total_encoding);
    total_encoding.reset();
    return error;
}

unsigned32 asn_any::get_value(unsigned char * &b, size_t & l) const {
    unsigned32 error;
    ((asn_any*)this)->total_encoding.empty();
    if (error =
            ((asn_any*)this)->write(((asn_any*)this)->total_encoding))
        return error;
    b = total_encoding.data;
    l = total_encoding.data_length;
    return 0;
}

int asn_any::check_type(unsigned char * &data,
                        size_t & data_length,
                        unsigned & ret_ber_tag_value,
                        asn_type_flags & flags) {
    unsigned char * loc_data = data;
    size_t loc_data_length = data_length;
    asn_type_flags loc_flags = flags;
    unsigned ber_tag_value;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "ANY()::CheckType()"));
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "Data starts: %2.2x %2.2x %2.2x, length is %ld",
                  (unsigned)data[0], (unsigned)data[1],
                  (unsigned)data[2], (long)data_length));
    if (data_length < 1) goto fail;
    flags.constructed = *data & 0x20;

    switch (*data & 0xc0) {
        case 0 : flags.tag_class = UNIVERSAL_CLASS; break;
        case 0x40 : flags.tag_class = APPLICATION_CLASS; break;
        case 0x80 : flags.tag_class = CONTEXT_CLASS; break;
        case 0xc0 : flags.tag_class = PRIVATE_CLASS; break;
    };

//  if (flags.tag_class != loc_flags.tag_class) goto fail;

    if ((*data & 0x1f) == 0x1f) {
// High tag-number
        data++,data_length--;
        if (data_length < 1) return pkc_s_asn_no_more_data;
        ber_tag_value = 0;
        while (*data & 0x80) {
            ber_tag_value = ber_tag_value << 7;
            ber_tag_value += *data & 0x7f;
            data++, data_length--;
            if (data_length < 1) goto fail;
        };
    } else {
// Low tag-number
        ber_tag_value = *data & 0x1f;
    };
    data++,data_length--;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "ANY() tag from BER is %d, class = %d",
                  ber_tag_value, flags.tag_class));

    ret_ber_tag_value = ber_tag_value;
    return 1;

fail:
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_asn, svc_c_debug9,
                  "ANY::CheckType() failed"));
    data = loc_data;
    data_length = loc_data_length;
    flags = loc_flags;

    return 0;
}

unsigned32 asn_explicit::display(buffer_t & buffer) const {
        int i;
        unsigned32 error;
        unsigned long tagValue;
        asn_tag_class tagClass;

        display_quals(buffer);
        tagValue = get_tag_value();
        tagClass = get_tag_class();
        switch (tagClass) {
        case UNIVERSAL_CLASS:
            buffer.append("[UNIVERSAL "); break;
        case APPLICATION_CLASS:
            buffer.append("[APPLICATION "); break;
        case CONTEXT_CLASS:
            buffer.append("[CONTEXT "); break;
        case PRIVATE_CLASS:
            buffer.append("[PRIVATE "); break;
        };
        buffer.append_dec(tagValue);
        buffer.append("] { ");
    for (i=0;i<contained_object_count; i++) {
        if (error = (*this)[i]->display(buffer)) return error;
        buffer.append(", ");
    }
    buffer.append("}");
    return 0;
}
