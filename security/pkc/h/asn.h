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
 * $Log: asn.h,v $
 * Revision 1.1.2.2  1996/11/13  20:36:15  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  23:23 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
 *
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:24 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
 *
 * Revision 1.1.2.1  1996/10/04  19:28:24  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  19:28:10  arvind]
 * 
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:25 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  16:08 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */

/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 * Module: asn.h
 *
 * Purpose:  This module contains the class definitions for the ASN.1
 *           BER encode/decode objects
 *
 *
 */



#ifndef ASN_
#define ASN_


#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
/* 
 * pthread needed as it might re-define read & write, so including it here
 * means that our users silently see the redefined versions.
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dce/nbase.h>
#include <dce/utc.h>
#include <dce/dcepkcmsg.h>
#include <dce/gssapi.h>

#ifdef __cplusplus
}
#endif


#define pkc_malloc_status

#ifdef __cplusplus
extern "C" 
unsigned32 pkc_adjust(unsigned char * &b,
                      size_t cur_size,
                      size_t new_size);
#endif

#ifdef __cplusplus
extern "C" 
#endif
void * pkc_malloc(size_t l);

#ifdef __cplusplus
extern "C" 
#endif
void * pkc_realloc(void * p, size_t l);

#ifdef __cplusplus
extern "C" 
#endif
void pkc_free(void * b);



#ifdef __cplusplus


#define DEFAULT_EXTENT 256


typedef enum {
    PUBLIC,
    SECURE
} secure_flag_t;

class r_buffer_t {
// A buffer descriptor (intended for read-only buffers)
public:
    unsigned char * orig_data;
    size_t orig_data_length;
    unsigned char * data;
    size_t data_length;
    r_buffer_t(void);
    void set(void);  // Take a snapshot of data/data_length for later restore
    void reset(void); // Restore data/data_length to previous settings
};

class buffer_t : public r_buffer_t {
protected:
    secure_flag_t security;
public:
    unsigned char * buffer;
    size_t buffer_length;
    buffer_t(secure_flag_t sec = PUBLIC);
    void secure(secure_flag_t sec = SECURE);
    void detach_buffer(void);
    ~buffer_t();
    unsigned32 clear(void);
    unsigned32 zero(void);
    unsigned32 empty(void);
    unsigned32 extend(size_t extra = 0);
    unsigned32 append(unsigned char c);
    unsigned32 append(const r_buffer_t & o);
    unsigned32 append(unsigned char * b, size_t l);
    unsigned32 append(char * b);
    unsigned32 append_dec(unsigned long i);
    unsigned32 append_dec(signed long i);
    unsigned32 append_hex(unsigned long i);
    unsigned32 append_hex(unsigned char i);

};

enum asn_tag_class {
    UNIVERSAL_CLASS = 0,
    APPLICATION_CLASS = 1,
    CONTEXT_CLASS = 2,
    PRIVATE_CLASS = 3
};

class asn_type_flags {
public:
    char constructed;
    char indefinite;
    asn_tag_class tag_class;
    asn_type_flags(void) {
        constructed = 0;
        indefinite = 0;
        tag_class = UNIVERSAL_CLASS;
    };
    int is_primitive(void) const { return !constructed; };
    int is_constructed(void) const { return constructed; };
    int is_definite(void) const { return !indefinite; };
    int is_indefinite(void) const { return indefinite; };
};

class asn_object {
friend class asn_constructed;
friend class asn_explicit;
protected:
    virtual unsigned32 check_default(void);
    asn_object * containing_object;

    buffer_t encode_buffer;
    char encoding_valid;
    char value_valid;
    char constant_object; // True for things like the NULL value

    char optional;
    char present;
    char isDefault;
    char defaultable;
    secure_flag_t security;

    char constructed;
    asn_tag_class tag_class;
    unsigned tag_value;

    unsigned32 parse_type_tag(unsigned char * &data,
                              size_t & data_length,
                              unsigned &tag_value,
                              asn_type_flags & flags);
    unsigned32 parse_type_tag(r_buffer_t & data_buffer,
                              unsigned &tag_value,
                              asn_type_flags & flags);

    unsigned32 parse_length(unsigned char * &data,
                            size_t & data_length,
                            size_t & value_length,
                            asn_type_flags &flags);
    unsigned32 parse_length(r_buffer_t & data_buffer,
                            size_t & value_length,
                            asn_type_flags &flags);

    int check_EOC(unsigned char * &data,
                  size_t & data_length);
    int check_EOC(r_buffer_t &data_buffer);

    virtual int check_type(unsigned char * &data,
                           size_t & data_length,
                           unsigned & ber_tag_value,
                           asn_type_flags &flags);
    int check_type(r_buffer_t &data_buffer,
                   unsigned & ber_tag_value,
                   asn_type_flags &flags) {
	return check_type(data_buffer.data,
                          data_buffer.data_length,
                          ber_tag_value,
                          flags);
    };

    virtual void invalidate_encoding(void);
// Derived classes should call this function whenever their encoding cache
// becomes invalid (for example, if the object value becomes invalid).
// however, the base-class value_changed() member function calls 
// invalidate_encoding, so many derived classes don't needd to explicitly
// call this function.

    virtual void value_changed(void);
// Derived classes should invoke this function whenever the value
// they represent changes.  This is used by the asn classes to
// maintain their encode/decode caches, and the various flags that
// are stored with each object.

    virtual unsigned32 decode_value_indefinite(
                            unsigned char * &data,
                            size_t & max_data_length);
    virtual unsigned32 decode_value_definite(
                            unsigned char * &data,
                            size_t & data_length) = 0;


    virtual unsigned32 encode_type_tag(buffer_t & data_buffer,
                                       unsigned tag_value,
                                       asn_type_flags flags);

    virtual unsigned32 encode_length(buffer_t & data_buffer,
                                     size_t value_length);

    virtual unsigned32 set_valid(char yesno = 1);

// Default assignment operator for asn_object hierarchy.  Note that in general
// there are better ways of copying than using the assignment operator because:
// 1) The generic assignment operator is very innefficient - it works by
//    calling write() on the source object, and read on the target object.
//    Member-wise copies (using get_value/set_value) are likely to be much 
//    more efficient.
// 2) If the source and destination object aren't of compatible types, the
//    assignment will fail with undefined results.  Since we choose
//    not to use exceptions, applications won't even be aware of such a
//    failure.
// Note that derived classes can't simply inherit this operator, as the 
// compiler will execute it and then execute default assignment operations
// for data members of derived classes, which will in general over-write
// the correct results that this operator has placed there.  Derived classes
// that want an assignment operator should declare one, and define it to
// be asn_object::operator=()
//
// Because of the complexities of inheriting this operator, I've decided to 
// make it private.  Equivalent functionality is available from the public 
// member function copy(), which assignment operators in derived classes could 
// invoke.  However, since the assignment operator can't signal an error,
// end-users should use copy() directly, rather than sacrificing
// error-checking for a slightly prettier syntax.

private:
    asn_object & operator = (const asn_object & o);

// Similarly, the copy constructor is dangerous, so make it privaate
private:
    asn_object(const asn_object & o, secure_flag_t sec);

public:

    virtual unsigned32 copy (const asn_object & o);
// A way to copy the value of another (type-compatible) asn_object
// into this one.

    int is_valid(void) const {
	return value_valid;
    };
    int is_set(void) const {
	return value_valid;
    };
    virtual void set_tag(unsigned tag) {
        tag_value = tag;
    };
    void set_tag_class(asn_tag_class theClass) {
        tag_class = theClass;
    };
    unsigned get_tag_value(void) const { return tag_value; };
    asn_tag_class get_tag_class(void) const { return tag_class; };
    virtual unsigned32 calculate_value(buffer_t & buffer) const = 0;
    virtual unsigned32 calculate_default_value(buffer_t & buffer);
    virtual unsigned32 hash(buffer_t & buffer) const;
    asn_object(secure_flag_t sec);
    virtual ~asn_object();
    int is_primitive(void) const { return !constructed; };
    int is_constructed(void) const { return constructed; };
    virtual unsigned32 make_default(void);
    virtual unsigned32 set_optional(char yesno = 1);
    virtual unsigned32 set_defaultable(void);
    virtual char is_present(void) const { return present; };
    virtual char is_optional(void) const { return optional; };
    virtual char is_default(void) const { return isDefault; };
    virtual char is_defaultable(void) const { return defaultable; };

    virtual unsigned32 read(r_buffer_t & data);
// Parse the supplied data.  The buffer is updated to
// describe the unparsed portions of the input data.

    virtual unsigned32 write(buffer_t & buffer) const;
// Append the DER value of the object to the buffer.


    virtual unsigned32 display(buffer_t & buffer) const;
// Append a human-readable display of the data to the buffer
    virtual int display_quals(buffer_t & buffer) const;
// Append a human-readable display of the object qualifiers.
// Called by display().  Returns 1 if value should be printed

    virtual int operator == (const asn_object & o) const;

    virtual int operator != (const asn_object & o) const {
	return !(*this == o);
    };


};


class asn_constructed : public asn_object {
protected:
    asn_object ** contained_object;
    size_t contained_object_count;
    asn_object * operator [] (size_t i) const;
    virtual void empty(void);
public:
    int find_child(asn_object * c);
    virtual size_t child_count(void) const { return contained_object_count; };
    asn_constructed(secure_flag_t sec) : asn_object(sec) {
        constructed = 1;
        contained_object = NULL;
        contained_object_count = 0;
    };
    virtual ~asn_constructed();
    virtual void register_child(asn_object * o);
};

class asn_primitive : public asn_object {
public:
    asn_primitive(secure_flag_t sec) : asn_object(sec) {
        constructed = 0;
    };
};

enum asn_int_value_type {
    SMALLINT, LARGEINT
};

class asn_integer : public asn_primitive {
    virtual unsigned32 check_default(void);
    void cons_int(void);
    signed32 default_data;
    signed32 data;
    unsigned char * data_buffer;
    size_t data_length;
    asn_int_value_type value_type;
public:
    asn_integer(secure_flag_t sec = PUBLIC) : asn_primitive(sec) {
        cons_int();
    };
    asn_integer & operator = (signed32 i);
    asn_integer & operator = (const asn_integer & o);
    virtual unsigned32 set_default_value(signed32 i);
    virtual unsigned32 set_value(signed32 i);
    virtual unsigned32 get_value(signed32 & i) const;
    virtual unsigned32 get_value(unsigned32 & i) const;
    virtual unsigned32 set_value(const unsigned char * b, size_t l);
    virtual unsigned32 get_value(unsigned char * &b, size_t & l) const;
    virtual unsigned32 calculate_value(buffer_t & buffer) const;
    virtual unsigned32 calculate_default_value(buffer_t & buffer);
    virtual unsigned32 decode_value_definite(
                            unsigned char * &data,
                            size_t & data_length);
    virtual unsigned32 display(buffer_t & buffer) const;
};



class asn_octetstring : public asn_object {
    virtual unsigned32 check_default(void);
    void cons_oct(void);
    unsigned char * data_buffer;
    size_t data_length;
    unsigned char * default_data_buffer;
    size_t default_data_length;
public:
    virtual void set_tag(unsigned tag) {
        tag_value = tag;
    };
    virtual ~asn_octetstring();
    asn_octetstring(secure_flag_t sec = PUBLIC) : asn_object(sec) {
        cons_oct();
    };
    virtual unsigned32 set_value(const unsigned char * b, size_t l);
    virtual unsigned32 get_value(unsigned char * &b, size_t & l) const;
    virtual unsigned32 set_default_value(const unsigned char * b, size_t l);
    virtual unsigned32 calculate_default_value(buffer_t & buffer);
    virtual unsigned32 calculate_value(buffer_t & buffer) const;
    virtual unsigned32 decode_value_definite(
                            unsigned char * &data,
                            size_t & data_length);
    virtual unsigned32 decode_value_indefinite(
                            unsigned char * &data,
                            size_t & data_length);
    virtual unsigned32 display(buffer_t & buffer) const;
};

class asn_boolean : public asn_object {
    virtual unsigned32 check_default(void);
    void cons_boole(void);
    char data;
    char default_data;
public:
    asn_boolean(secure_flag_t sec = PUBLIC) : asn_object(sec) {
        cons_boole();
    };
    asn_boolean & operator = (char c);
    virtual unsigned32 set_value(char c);
    virtual unsigned32 get_value(char & c) const;
    virtual unsigned32 calculate_value(buffer_t & buffer) const;
    virtual unsigned32 set_default_value(char c);
    virtual unsigned32 calculate_default_value(buffer_t & buffer);
    virtual unsigned32 decode_value_definite(
                            unsigned char * &data,
                            size_t & data_length);
    virtual unsigned32 display(buffer_t & buffer) const;
};

class asn_null : public asn_primitive {
    void cons_null(void);
public:
    asn_null(secure_flag_t sec = PUBLIC) : asn_primitive(sec) {
        cons_null();
    };
    virtual unsigned32 calculate_value(buffer_t & buffer) const;
    virtual unsigned32 decode_value_definite(
                            unsigned char * &data,
                            size_t & data_length);
    virtual unsigned32 display(buffer_t & buffer) const {
        buffer.append("<NULL>");
        return 0;
    };
};

inline size_t bit_to_byte_length(unsigned32 b) {
    return (b+7)/8;
}

inline unsigned char unused_bit_count(unsigned32 l) {
    return ((8 - (l%8)) & 0x07);
}

inline unsigned char used_bit_mask(unsigned32 b) {
    switch(b%8) {
    case 1: return 0x80; // 7 unused bits
    case 2: return 0xc0; // 6 bits unused
    case 3: return 0xe0; // 5 bits unused
    case 4: return 0xf0; // 4 bits unused
    case 5: return 0xf8; // 3 bits unused
    case 6: return 0xfc; // 2 bits unused
    case 7: return 0xfe; // 1 bit unused
    };
    return 0xff; // All bits used
}

class asn_bitstring : public asn_object {
    virtual unsigned32 check_default(void);
    void cons_bit(void);
    unsigned char * data_buffer;
    size_t data_length;
    unsigned char unused_bits;
    unsigned char * default_data_buffer;
    size_t default_data_length;
    unsigned char default_unused_bits;
public:
    virtual ~asn_bitstring();
    asn_bitstring(secure_flag_t sec = PUBLIC) : asn_object(sec) {
        cons_bit();
    };
    virtual unsigned32 set_value(const unsigned char * b, unsigned32 l);
    virtual unsigned32 set_default_value(const unsigned char * b, unsigned32 l);
    virtual unsigned32 get_value(unsigned char * &b, unsigned32 & l) const;
    virtual unsigned32 calculate_value(buffer_t & buffer) const;
    virtual unsigned32 calculate_default_value(buffer_t & buffer);
    virtual unsigned32 decode_value_definite(
                            unsigned char * &data,
                            size_t & data_length);
    virtual unsigned32 decode_value_indefinite(
                            unsigned char * &data,
                            size_t & data_length);
    virtual unsigned32 display(buffer_t & buffer) const;
};




class asn_objectidentifier : public asn_primitive {
    virtual unsigned32 check_default(void);
    void cons_oid(void);
    unsigned long * subidents;
    size_t subident_count;
    unsigned long * default_subidents;
    size_t default_subident_count;
    unsigned32 encode_subident(unsigned long v, buffer_t & buffer) const;
    unsigned32 decode_subident(unsigned char * &data,
                               size_t & data_length,
                               unsigned long & encoding) const;
public:
    virtual ~asn_objectidentifier();
    asn_objectidentifier(secure_flag_t sec = PUBLIC) : asn_primitive(sec) {
        cons_oid();
    };
    asn_objectidentifier(const unsigned long * b, 
                         size_t l, 
                         secure_flag_t sec = PUBLIC) : asn_primitive(sec) {
        cons_oid();
        set_value(b, l);
    };
    asn_objectidentifier & operator = (const asn_objectidentifier & o);
    virtual unsigned32 set_value(const char * stringVal);
    virtual unsigned32 set_value(const unsigned long * b, size_t l);
    virtual unsigned32 set_value(const asn_objectidentifier &o,
                                 const unsigned long * b,
                                 size_t l);
    virtual unsigned32 set_value(const asn_objectidentifier &o,
                                 unsigned long s);
    virtual unsigned32 set_default_value(const unsigned long * b, size_t l);
    virtual unsigned32 set_default_value(const asn_objectidentifier &o,
                                         const unsigned long * b,
                                         size_t l);
    virtual unsigned32 set_default_value(const asn_objectidentifier &o,
                                         unsigned long s);
    virtual unsigned32 get_value(unsigned long *& b, size_t & l) const ;

// The next routine returns the value as a gss_OID object.  Unlike all
// other get_value members, on entry, the parameter must be either NULL 
// or a valid gss_OID.
    virtual unsigned32 get_value(gss_OID & v) const ;

    virtual unsigned32 calculate_default_value(buffer_t & buffer);
    virtual unsigned32 calculate_value(buffer_t & buffer) const;
    virtual unsigned32 decode_value_definite(
                            unsigned char * &data,
                            size_t & data_length);
    virtual unsigned32 display(buffer_t & buffer) const;
    virtual unsigned32 x500_display(buffer_t & buffer) const;
    virtual int operator==(const asn_objectidentifier & o) const;
};


class asn_choice : public asn_constructed {
protected:
    size_t choice;
    virtual unsigned32 decode_value_definite(
                            unsigned char * &data,
                            size_t & data_length);
public:
    asn_choice(secure_flag_t sec = PUBLIC) : asn_constructed(sec) {
        choice = 0;
    };
    virtual unsigned32 set_value(size_t c) {
        if ((choice != c) || !value_valid) {
            choice = c;
            value_changed();
        };
        return 0;
    };
    virtual unsigned32 get_value(size_t & c) const;
    virtual unsigned32 calculate_value(buffer_t & buffer) const;
    virtual unsigned32 display(buffer_t & buffer) const;
    virtual unsigned32 read(r_buffer_t & data);
    virtual unsigned32 write(buffer_t & data) const;
};

#define STRING_SYNTAX_UNSPECIFIED 0
#define STRING_SYNTAX_C 1
#define STRING_SYNTAX_ISO646 2
#define STRING_SYNTAX_IA5 3
#define STRING_SYNTAX_T61 4


class asn_charstring : public asn_octetstring {
    int syntax;
    char * temp_string;
    void cons_chars(unsigned t) {
	syntax = STRING_SYNTAX_UNSPECIFIED;
        tag_value = t;
	temp_string = NULL;
    };
public:
    unsigned32 set_syntax(int s) { syntax = s; return 0; };
    asn_charstring(unsigned t, secure_flag_t sec = PUBLIC) 
    : asn_octetstring(sec) {
        cons_chars(t);
    };
    asn_charstring(unsigned t, const char * s, secure_flag_t sec = PUBLIC) 
    : asn_octetstring(sec) {
        cons_chars(t);
        set_value(s);
    };
    virtual ~asn_charstring();
    virtual unsigned32 set_value(const char * s, int translate = 1);
    virtual unsigned32 set_default_value(const char * s, int translate = 1);
    virtual unsigned32 get_value(char * &s, int translate = 1) const;
    virtual unsigned32 display(buffer_t & buffer) const;
    virtual unsigned32 x500_display(buffer_t & buffer) const;
    virtual unsigned32 read(r_buffer_t & data);
    virtual unsigned32 write(buffer_t & data) const;
};

class asn_conformantstring : public asn_charstring {
// A conformantstring is one that has no fixed ASN type.  It has a
// default type, that's used when a value is loaded by set_value(),
// but any character-string ASN1 type can be succesfully read into
// it, and it's actual type will change to conform.
protected:
    unsigned default_tag_value;
    void cons_conf(unsigned t) {
        default_tag_value = t;
    };
    virtual int check_type(unsigned char * &data,
                           size_t & data_length,
                           unsigned & ber_tag_value,
                           asn_type_flags &flags);
public:
    asn_conformantstring(unsigned t, secure_flag_t sec = PUBLIC) 
    : asn_charstring(t, sec) {
        cons_conf(t);
    };
    asn_conformantstring(unsigned t, char * s, secure_flag_t sec = PUBLIC) 
    : asn_charstring(t, s, sec) {
        cons_conf(t);
    };
    virtual unsigned32 set_value(const char * s, int translate = 1) {
        asn_charstring::set_tag(default_tag_value);
        return asn_charstring::set_value(s, translate);
    };
    virtual unsigned32 set_default_value(const char * s, int translate = 1) {
        return asn_charstring::set_default_value(s, translate);
    };

};

class asn_IA5string : public asn_charstring {
public:
    asn_IA5string(secure_flag_t sec = PUBLIC) 
    : asn_charstring(22, sec) { set_syntax(STRING_SYNTAX_IA5); };
};

class asn_T61string : public asn_charstring {
public:
    asn_T61string(secure_flag_t sec = PUBLIC) 
    : asn_charstring(20, sec) { set_syntax(STRING_SYNTAX_T61); };
};


class asn_utctime : public asn_charstring {
    unsigned century_start;
    char ignore_seconds;
public:
    asn_utctime(secure_flag_t sec = PUBLIC);
// Override the charstring set_value function as it would
// provide a way to break DER-encoding.
// Note that if you invoke the char * version of get_value, 
// you get no Y2K help.
    virtual unsigned32 set_value(const char * s) {
        return pkc_s_unimplemented;
    };
    virtual unsigned32 set_value(utc_t time);
    virtual unsigned32 get_value(utc_t & time) const;
    virtual unsigned32 set_value(unsigned year, 
                                 unsigned month,            // 1..12
                                 unsigned day,              // 1..31
                                 unsigned hour,             // 0..23
                                 unsigned minute,           // 0..59
                                 unsigned sec,              // 0..59
                                 int tz_hour = 0,           // -12..+12
                                 unsigned tz_min = 0);      // 0..59
// The tz_min is intepreted as adding to the magnitude of the tz_hour,
// and having the same sign as the tz_hour.  Thus a time-zone of 
// GMT-5:30 would be expressed as tz_hour = -5, and tz_min = 30
    virtual unsigned32 get_value(unsigned &year, 
                                 unsigned &month,
                                 unsigned &day,
                                 unsigned &hour,
                                 unsigned &minute,
                                 unsigned &sec,
                                 int &tz_hour,
                                 unsigned &tz_min) const ;
    virtual unsigned32 set_century_rollover(unsigned s);
// set_century_rollover is a hack to avoid the Y2K problem that is
// inherent with the UTCtime type.  UTCtime only stores two digits for
// the year portion of the date, and assumes 19XX as the interpretation
// of this.  Thus the basic type is incapable of expressing dates
// after 1999.  set_century_rollover(s) tells the class to treat
// parsed years before s as 20XX.  "s" should be in the range 0 to 99.

    virtual unsigned32 set_ignore_seconds(char t = 1);
// Tells the class whether to include seconds in the generated BER,
// or whether to ignore them.  Seconds are always supressed if they
// are zero.
};



class asn_sequence : public asn_constructed {
protected:
    virtual unsigned32 decode_value_definite(
                            unsigned char * &data,
                            size_t & data_length);
    virtual unsigned32 decode_value_indefinite(
                            unsigned char * &data,
                            size_t & data_length);

    void cons_seq(void);
    asn_object * operator [] (size_t i) const {
        return asn_constructed::operator[](i);
    }
    virtual void empty(void) { asn_constructed::empty(); };
public:
    asn_sequence(secure_flag_t sec = PUBLIC) 
    : asn_constructed(sec) {
        cons_seq();
    };
    virtual unsigned32 calculate_value(buffer_t & buffer) const;
    virtual unsigned32 display(buffer_t & buffer) const;
};



class asn_sortable : public asn_constructed {
protected:
    virtual void presort_elements(void) {};
    virtual void sort_elements(void);
    virtual int compare_objects(int i, int j) = 0;
// returns 1 if i > j, 0 if i = j, -1 if i < j
    virtual void swap_objects(int i, int j);
// swaps objects i & j
public:
    virtual unsigned32 write(buffer_t & buffer) const;
    asn_sortable(secure_flag_t sec) : asn_constructed(sec) {};
};

class asn_set : public asn_sortable {
protected:
    char * component_present;
    virtual unsigned32 decode_value_definite(
                            unsigned char * &data,
                            size_t & data_length);
    virtual unsigned32 decode_value_indefinite(
                            unsigned char * &data,
                            size_t & data_length);

    void cons_set(void);
    virtual unsigned32 set_child_present(int i);
    virtual unsigned32 set_child_absent(int i);
    virtual int compare_objects(int i, int j);
    virtual void swap_objects(int i, int j);
    virtual void register_child(asn_object * o) {
        asn_sortable::register_child(o);
        component_present =
            (char *)pkc_realloc(component_present, contained_object_count);
        component_present[contained_object_count-1] = 0;
    };
public:
    virtual ~asn_set();
    asn_set(secure_flag_t sec = PUBLIC) : asn_sortable(sec) {
        cons_set();
    };
    virtual unsigned32 calculate_value(buffer_t & buffer) const;
    virtual unsigned32 display(buffer_t & buffer) const;
    virtual int child_present(int i);
    virtual unsigned32 set_child_present(asn_object * c);
    virtual unsigned32 set_child_absent(asn_object * c);
};

// Unlike classes with fixed elements, sequence_of and set_of classes own
// their children - that is, they are dynamically created and deleted.
// The only way to access the children is via the class [] operator.

template<class elem_t> class asn_sequenceof : public asn_sequence {
protected:
    virtual unsigned32 decode_value_definite(
                            unsigned char * &data,
                            size_t & data_length) {

        r_buffer_t buffer;
        unsigned32 error;
        size_t the_child = 0;

        buffer.data = data;
        buffer.data_length = data_length;

        empty();  // Remove any current children

        while (buffer.data_length != 0) {
            register_child(new elem_t); // Create new children as needed.
            if (error = asn_sequence::operator[](the_child)->read(buffer)) {
                return error;
            };
            the_child++;
        }
        data = buffer.data;
        data_length = 0;
        return 0;
    };
    virtual unsigned32 decode_value_indefinite(
                            unsigned char * &data,
                            size_t & data_length) {
        r_buffer_t buffer;
        unsigned32 error;
        size_t the_child = 0;

        buffer.data = data;
        buffer.data_length = data_length;

        empty();  // Remove any current children

        while (buffer.data_length > 0) {
            if (check_EOC(buffer)) {
                data = buffer.data;
                data_length = buffer.data_length;
                return 0;
            };
            register_child(new elem_t); // Create new children as needed.
            if (error = asn_sequence::operator[](the_child)->read(buffer)) {
                return error;
            };
            the_child++;
        }
        return pkc_s_asn_no_more_data;
    };
public:
    virtual void empty(void) {
        int i;
        for (i=0; i<child_count();i++) {
            delete (*this)[i];
        };
        asn_sequence::empty();
    };
    asn_sequenceof(secure_flag_t sec = PUBLIC) : asn_sequence(sec) {};
    virtual ~asn_sequenceof() { empty(); };
    virtual elem_t * operator [] (size_t i) const {
        return (elem_t *)(asn_sequence::operator[](i));
    };
    virtual elem_t * new_child(void) {
        elem_t * the_child;
        the_child = new elem_t;
        register_child(the_child);
        return the_child;
    };
};




template<class elem_t> class asn_setof : public asn_set {
    buffer_t ** encoding;

protected:
    virtual void register_child(asn_object * o) {
        asn_set::register_child(o);
        component_present[contained_object_count-1] = 1;
    };
    virtual void presort_elements(void) {
// Called immediately before sorting the elements.  We have to run through 
// the elements and ask them to encode themeselves into the 
        int i;

        if (component_present == NULL)
            component_present = (char*)pkc_malloc(contained_object_count);

        if (encoding) pkc_free(encoding);
        encoding = 
            (buffer_t **)pkc_malloc(contained_object_count * sizeof(buffer_t *));

        for (i=0;i<contained_object_count;i++) {
            encoding[i] = new buffer_t(security);
            contained_object[i]->write(*encoding[i]);
// This has to be set so the set:: routines will do their stuff
            component_present[i] = 1; 
        };
    };
    virtual int compare_objects(int i, int j) {
// Compare the two encodings.  If the encodings differ within the length of 
// the smaller, then use lexicographic ordering; if not, then the shorter 
// string is smaller.
        int x;
        size_t len;
        len = ((encoding[i]->data_length < encoding[j]->data_length)
               ?encoding[i]->data_length : encoding[j]->data_length);
        for (x=0; x < len; x++) {
            if (encoding[i]->data[x] < encoding[j]->data[x]) return -1;
            else if (encoding[i]->data[x] > encoding[j]->data[x]) return 1;
        };
        if (encoding[i]->data_length < encoding[j]->data_length) return -1;
        else if (encoding[i]->data_length > encoding[j]->data_length) return 1;
        else return 0;
    };
    virtual void swap_objects(int i, int j) {
        buffer_t * temp;
        asn_sortable::swap_objects(i, j);
        temp = encoding[i];
        encoding[i] = encoding[j];
        encoding[j] = temp;
    };
    virtual unsigned32 decode_value_definite(
                            unsigned char * &data,
                            size_t & data_length) {

        r_buffer_t buffer;
        unsigned32 error;
        size_t the_child = 0;

        buffer.data = data;
        buffer.data_length = data_length;

        empty();  // Remove any current children

        while (buffer.data_length != 0) {
            register_child(new elem_t); // Create new children as needed.
            if (error = asn_set::operator[](the_child)->read(buffer)) {
                return error;
            };
            the_child++;
        }
        data = buffer.data;
        data_length = 0;
        return 0;
    };
    virtual unsigned32 decode_value_indefinite(
                            unsigned char * &data,
                            size_t & data_length) {
        r_buffer_t buffer;
        unsigned32 error;
        size_t the_child = 0;

        buffer.data = data;
        buffer.data_length = data_length;

        empty();  // Remove any current children

        while (buffer.data_length > 0) {
            if (check_EOC(buffer)) {
                data = buffer.data;
                data_length = buffer.data_length;
                return 0;
            };
            register_child(new elem_t); // Create new children as needed.
            if (error = asn_set::operator[](the_child)->read(buffer)) {
                return error;
            };
            the_child++;
        }
        return pkc_s_asn_no_more_data;
    };
public:
    virtual void empty(void) {
        int i;
        for (i=0; i<child_count();i++) {
            delete (*this)[i];
            if (encoding) delete encoding[i];
        };
        pkc_free(encoding); encoding = NULL;
        asn_set::empty();
    };
    virtual ~asn_setof() { empty(); };
    asn_setof(secure_flag_t sec = PUBLIC) : asn_set(sec) {
        encoding = NULL;
    };
    virtual elem_t * operator [] (size_t i) const {
        return (elem_t *)(asn_set::operator[](i));
    };
    virtual elem_t * new_child(void) {
        elem_t * the_child;
        the_child = new elem_t;
        register_child(the_child);
        return the_child;
    };
};


// asn_explicit is actually implemented as an implicitly tagged sequence.
// This construct results in correct encoding, and saves writing and 
// debugging any new code.  The only downside is that it's possible for
// an application to register multiple children of an asn_explicit, which 
// isn't legal ASN.1.  So don't do that :-)
class asn_explicit : public asn_sequence {
public:
    asn_explicit(asn_tag_class tagClass, 
                 unsigned tagValue, 
                 secure_flag_t sec = PUBLIC) : asn_sequence(sec) {
        set_tag(tagValue);
        set_tag_class(tagClass);
    };
    asn_explicit(unsigned tagValue,
                 secure_flag_t sec = PUBLIC) : asn_sequence(sec) {
	set_tag(tagValue);
        set_tag_class(CONTEXT_CLASS);
    };
    virtual char is_default(void) const {
        return contained_object[0]->is_default();
    };
    virtual char is_defaultable(void) const {
        return contained_object[0]->is_defaultable();
    };
    virtual unsigned32 check_default(void) {
        return contained_object[0]->check_default();
    };
    virtual unsigned32 calculate_default_value(buffer_t & buffer) {
        // We know the contained object is it's default value, so
        // just ask it to encode itself...
        return contained_object[0]->write(buffer);
    };
    virtual unsigned32 display(buffer_t & buffer) const;
};


// The class for ANY is a bit of a kludge, in that it doesn't really work
// for a true ANY value, only those values that are encoded in BER.
// It works by parsing the incoming BER stream, and stores the entire
// result as its value. Today, only definite-length-encoded ANY values
// are supported by the parser, although this restriction could be lifted
// fairly easily.

class asn_any : public asn_object {
    buffer_t value_encoding;
    buffer_t total_encoding;
    virtual int check_type(unsigned char * &data,
                           size_t & data_length,
                           unsigned & ber_tag_value,
                           asn_type_flags &flags);
public:
    asn_any(secure_flag_t sec = PUBLIC) : asn_object(sec) {
        value_encoding.empty();
        value_encoding.secure(security);
        total_encoding.secure(security);
    };
    virtual unsigned32 set_value(const asn_object & val);
    virtual unsigned32 get_value(unsigned char * &b, size_t & l) const;
    virtual unsigned32 decode_value_definite(
                            unsigned char * &data,
                            size_t & data_length) {
	value_encoding.empty();
        return value_encoding.append(data, data_length);
    };
    virtual unsigned32 calculate_value(buffer_t & buffer) const {
        return buffer.append(value_encoding);
    };
};

#endif
#endif
