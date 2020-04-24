/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: xdsxx.h,v $
 * Revision 1.1.2.2  1996/11/18  19:22:38  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/18  19:22:12  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/10/18  00:40 UTC  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/17  00:23 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:26 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  16:12 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 */

#ifndef XDSXX_
#define XDSXX_

#ifndef __cplusplus
#error "This file requires a C++ compiler"
#endif

extern "C" {
#include <pthread.h>
#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <xdscds.h>
#include <xdsgds.h>
}
#include <dce/x500name.h>

extern int OM_object_identifier_equal(const OM_object_identifier & o1,
                                      const OM_object_identifier & o2);


typedef enum {
    XDS_BASE_ONLY,
    XDS_ONE_LEVEL,
    XDS_SUBTREE
} subset_t;

typedef enum {
    XDS_C_SUCCESS, // Indicates a success
    XDS_C_BAD_STATUS, // Indicates a problem decoding a ds_status value
    XDS_C_ABANDON_FAILED,
    XDS_C_ATTRIBUTE_ERROR,
    XDS_C_ATTRIBUTE_PROBLEM,
    XDS_C_COMMUNICATIONS_ERROR,
    XDS_C_LIBRARY_ERROR,
    XDS_C_NAME_ERROR,
    XDS_C_REFERRAL, // Should never be returned to the user
    XDS_C_SECURITY_ERROR,
    XDS_C_SERVICE_ERROR,
    XDS_C_SYSTEM_ERROR,
    XDS_C_UPDATE_ERROR
} xds_error_class_t;


typedef enum {
    VC_UNSUPPORTED,
    VC_NULL,
    VC_BOOLEAN,
    VC_INTEGER,
    VC_CHARSTRING,
    VC_OCTETSTRING,
    VC_BITSTRING
} value_class_t;



value_class_t syntaxToClass(OM_syntax s);


const char * xds_error_class_name(xds_error_class_t class_id);
const char * xds_error_code_name(OM_enumeration code_id);

class om_translation_t {
protected:
    OM_public_object copied_object;
    OM_object original_object;
protected:
    OM_public_object get_obj(void) {
        if (copied_object != NULL) return copied_object;
        else return original_object;
    };
public:
    om_translation_t(void);
    om_translation_t(OM_object & obj, int force = 0);
    virtual ~om_translation_t();
};

class xds_value_t : public om_translation_t {
public:
    value_class_t theValueType;
    OM_syntax theValueSyntax;    
    long theInteger;
    OM_string theString;
    int theBoolean;    


    xds_value_t(void);
    xds_value_t(OM_object o);
    xds_value_t(const xds_value_t & o);
    virtual ~xds_value_t();

    virtual unsigned32 set_value(OM_syntax s, long i);
    virtual unsigned32 set_value(OM_syntax s, const char * c);
    virtual unsigned32 set_value(OM_syntax s, 
                                 const unsigned char * c, size_t l);
    virtual unsigned32 get_value_syntax(OM_syntax &s) const;
    virtual unsigned32 get_value_class(value_class_t & s) const;
// The next two methods allocate storage via pkc_malloc for their return 
// parameters...
    virtual unsigned32 get_value(unsigned char * &s, size_t & l) const;
    virtual unsigned32 get_value(char * &s) const;
    virtual unsigned32 get_value(long & s) const;
};

class xds_value_list_t : public om_translation_t {
    xds_value_list_t * current_value;
public:
    xds_value_t * value;
    xds_value_list_t * next_value;
    xds_value_list_t(void);
    xds_value_list_t(OM_object o);

    virtual unsigned32 add_value(OM_syntax s, long i);
    virtual unsigned32 add_value(OM_syntax s, const char * c);
    virtual unsigned32 add_value(OM_syntax s, 
                                 const unsigned char * c, size_t l);

    virtual ~xds_value_list_t();
    virtual unsigned32 copy(xds_value_list_t * &newList) const;
    virtual unsigned32 get_first_value(const xds_value_t * & val) const;
    virtual unsigned32 get_next_value(const xds_value_t * & val) const;
};

class xds_filter_t {
};

class xds_selection_t {
friend class xds_t;
    int all_attributes;
    OM_descriptor * xom_object;
public:
    xds_selection_t(void);
    virtual ~xds_selection_t();
    void select_all_attributes(void);
    void select_specified_attributes(void);
    void clear_all_attributes(void);
    void select_attribute(OM_object_identifier attr);
    void return_types_only(void);
    void return_types_and_values(void);
};


class xds_result_t : public om_translation_t {
    int AliasWasDereferenced;
public:
    unsigned32 error_status;
    xds_result_t(OM_object par);
    void scan_object(OM_object obj);
//    virtual ~xds_result_t();
    virtual int aliasDereferenced(void) const {return AliasWasDereferenced; }
};

class xds_attribute_t : public om_translation_t {
friend class xds_attribute_list_t;
friend class xds_changes_t;
    xds_attribute_t * next_attribute;
    OM_object_identifier attributeType;
    xds_value_list_t * first_attributeValue;
public:
    unsigned32 error_status;
    xds_attribute_t(OM_object apr);
    xds_attribute_t(const OM_object_identifier & attr_type,
                    const xds_value_list_t & values);
    virtual ~xds_attribute_t();
    virtual unsigned32 get_next_attribute(const xds_attribute_t * & attr) const;
    virtual unsigned32 get_type(const OM_object_identifier * & attr_type) const;
    virtual unsigned32 get_first_value(const xds_value_t * &s ) const;
    virtual unsigned32 get_next_value(const xds_value_t * &s ) const;
};


class xds_changes_t {
    int modification_count;
    OM_object xom_object;
    virtual unsigned32 add_attr_value_list(
                            const OM_object_identifier & attr_type,
                            const xds_value_list_t * values,
                            OM_enumeration mod_type);
                              
public:
    xds_changes_t(void);
    virtual ~xds_changes_t();
    virtual unsigned32 build_xom_object(OM_object & o) const;
    virtual unsigned32 delete_xom_object(void) const;
    virtual unsigned32 add_attribute(const OM_object_identifier & attr_type,
                                     const xds_value_list_t & values);
    virtual unsigned32 add_values(const OM_object_identifier & attr_type,
                                  const xds_value_list_t & values);
    virtual unsigned32 remove_attribute(const OM_object_identifier & attr_type);
    virtual unsigned32 remove_values(const OM_object_identifier & attr_type,
                                     const xds_value_list_t & values);
};


class xds_attribute_list_t : public om_translation_t {
    xds_attribute_t * first_attribute;
    OM_object xom_object;
public:
    unsigned32 error_status;
    xds_attribute_list_t(OM_object par);    
    xds_attribute_list_t(void);    
    virtual ~xds_attribute_list_t();    
    virtual unsigned32 build_xom_object(OM_object & o) const;
    virtual unsigned32 delete_xom_object(void) const;
    virtual unsigned32 get_first_attribute(const xds_attribute_t * & att) const;
    virtual unsigned32 add_attribute(const OM_object_identifier &attr_type,
                                     const xds_value_list_t & values);
};

class xds_entry_t : public xds_attribute_list_t {
    x500name object_name;
    int wasFromEntry;
public:
    xds_entry_t(OM_object par);    
    virtual ~xds_entry_t();    
    virtual unsigned32 get_object_name(const x500name * & name) const;
    virtual int fromEntry(void) const { return wasFromEntry; };
};

class xds_read_result_t : public xds_result_t {
    xds_entry_t * entry;
    const xds_attribute_t * cur_attribute;
public:
    xds_read_result_t(OM_object par);
    virtual ~xds_read_result_t();
    virtual unsigned32 get_object_name(const x500name *& name) const;
    virtual unsigned32 get_first_attribute(const xds_attribute_t * & att) const;
    virtual unsigned32 get_next_attribute(const xds_attribute_t * & att) const;
};


class xds_list_info_item_t {
friend class xds_list_result_t;
    xds_list_info_item_t * next_item;
    int entryIsAlias;
    int wasFromEntry;
    RelativeDistinguishedName rdn;
public:
    int isAlias(void) {return entryIsAlias; };
    int fromEntry(void) {return wasFromEntry; };
    unsigned32 error_status;
    xds_list_info_item_t(OM_object obj);
    virtual ~xds_list_info_item_t();
    virtual unsigned32 get_rdn(const RelativeDistinguishedName * & name) const;
};


class xds_list_result_t : public xds_result_t {
protected:
    x500name object_name;
    int wasPartialOutcome;
    xds_list_info_item_t * first_item;
    xds_list_info_item_t * cur_item;
public:
    xds_list_result_t(OM_object par);
    virtual ~xds_list_result_t();
    virtual unsigned32 get_object_name(const x500name * & name) const {
        if (object_name.is_set()) name = &object_name;
        else return pkc_s_no_value;
        return 0;
    };
    virtual int partialOutcome(void) const {
        return wasPartialOutcome;
    };
    virtual unsigned32 get_first_item(
                                const xds_list_info_item_t * & result) const;
    virtual unsigned32 get_next_item(
                                const xds_list_info_item_t * & result) const;
    
};

class xds_search_result_t : public xds_result_t {
public:
   xds_search_result_t(OM_object o) : xds_result_t(o) {};    
   virtual ~xds_search_result_t();
};

class xds_status_t : public om_translation_t {
protected:
    OM_object_identifier attribute_oid;
    x500name problem_name;
public:
    xds_status_t(OM_object ds_status);
    xds_status_t(void);
    virtual ~xds_status_t();
    virtual xds_status_t & operator=(const xds_status_t & o);
    virtual int isSuccess(void) const;
    virtual int isError(void) const { return !isSuccess(); };
    xds_error_class_t error_class;
    OM_enumeration error_code;    
    virtual unsigned32 get_attribute_oid(
                                    const OM_object_identifier * & oid) const;
    virtual unsigned32 get_name(const x500name * & name) const;
};

class xds_t {
private:
    static pthread_once_t once_block;
    static OM_workspace workspace;
    static void initialize_xds(void);
    OM_private_object session;
    static int cdsIsSupported;
    static int gdsIsSupported;
public:
    int cds_supported(void) { return cdsIsSupported; };
    int gds_supported(void) { return gdsIsSupported; };
    unsigned32 error_status;
    xds_t(void);
    virtual ~xds_t();

    virtual xds_status_t * add_entry(const x500name & name, 
                                     const xds_attribute_list_t & entry);
    virtual xds_status_t * add_entry(const x500name & name);
    virtual xds_status_t * list(const x500name & name, 
                                xds_list_result_t *& results);
    virtual xds_status_t * modify(const x500name & name, 
                                  const xds_changes_t & changes);
    virtual xds_status_t * modify_rdn(const x500name & name, 
                                      const RelativeDistinguishedName & new_rdn,
                                      int delete_old_rdn);
    virtual xds_status_t * read(const x500name & name,
                                const xds_selection_t & selection,
                                xds_read_result_t *& results);
    virtual xds_status_t * remove_entry(const x500name & name);
    virtual xds_status_t * search(const x500name & base,
                                  subset_t subset,
                                  const xds_filter_t & filter,
                                  int search_aliases,
                                  const xds_selection_t & selection,
                                  xds_search_result_t *& results);    
};

#endif
