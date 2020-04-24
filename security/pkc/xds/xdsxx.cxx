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
 * $Log: xdsxx.cxx,v $
 * Revision 1.1.2.1  1996/11/13  20:42:52  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  20:05:54  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/11/04  22:27 UTC  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  23:22 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:41 UTC  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:31 UTC  arvind  /main/arvind_capi/1]
 * 
 * $EndLog$
 */
/*
 * @DEC_COPYRIGHT@
 * Copyright (c) 1996
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 * 
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 * 
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 * 
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 * 
 * 
 * ABSTRACT:    Purpose: C++ interface to the XDS nameservice API
 * 
 *	
 * ENVIRONMENT:	portable
 * 
 * AUTHOR:
 *              John Wray 
 *		Distributed Processing Engineering (DPE)
 * 
 * DATE:	March 1996
 * 
 */


extern "C" {
#include <pthread.h>
#include <string.h>
#include <stdio.h>
}

#include <dce/xdsxx.h>

#ifdef DEC_XDS_DEBUG
#include <xom_debug.h>
extern "C" {
#include <xdsdec.h>
}
#endif

#include <dce/pkc_common.h>

OM_IMPORT(DS_C_ABANDON_FAILED)
OM_IMPORT(DS_C_ATTRIBUTE)
OM_IMPORT(DS_C_ATTRIBUTE_LIST)
OM_IMPORT(DS_C_ATTRIBUTE_ERROR)
OM_IMPORT(DS_C_ATTRIBUTE_PROBLEM)
OM_IMPORT(DS_C_COMMUNICATIONS_ERROR)
OM_IMPORT(DS_C_LIBRARY_ERROR)
OM_IMPORT(DS_C_NAME_ERROR)
OM_IMPORT(DS_C_REFERRAL)
OM_IMPORT(DS_C_SECURITY_ERROR)
OM_IMPORT(DS_C_SERVICE_ERROR)
OM_IMPORT(DS_C_SYSTEM_ERROR)
OM_IMPORT(DS_C_UPDATE_ERROR)
OM_IMPORT(DS_C_ENTRY_INFO_SELECTION)
OM_IMPORT(DS_C_ENTRY_MOD_LIST)
OM_IMPORT(DS_C_ENTRY_MOD)
OM_IMPORT(DS_BASIC_DIR_CONTENTS_PKG)
OM_IMPORT(DSX_GDS_PKG)


OM_workspace xds_t::workspace = NULL;

int xds_t::cdsIsSupported = 0;
int xds_t::gdsIsSupported = 0;

int OM_object_identifier_equal(const OM_object_identifier & o1,
                               const OM_object_identifier & o2) {
    if (o1.length != o2.length) return 0;
    return (memcmp(o1.elements, o2.elements, o1.length) == 0);
}


const char * xds_error_class_name(xds_error_class_t class_id) {
    switch (class_id) {
    case XDS_C_SUCCESS: return "SUCCESS";
    case XDS_C_BAD_STATUS: return "BAD_STATUS";
    case XDS_C_ABANDON_FAILED: return "ABANDON_FAILED";
    case XDS_C_ATTRIBUTE_ERROR: return "ATTRIBUTE_ERROR";
    case XDS_C_ATTRIBUTE_PROBLEM: return "ATTRIBUTE_PROBLEM";
    case XDS_C_COMMUNICATIONS_ERROR: return "COMMUNICATIONS_ERROR";
    case XDS_C_LIBRARY_ERROR: return "LIBRARY_ERROR";
    case XDS_C_NAME_ERROR: return "NAME_ERROR";
    case XDS_C_REFERRAL: return "REFERRAL";
    case XDS_C_SECURITY_ERROR: return "SECURITY_ERROR";
    case XDS_C_SERVICE_ERROR: return "SERVICE_ERROR";
    case XDS_C_SYSTEM_ERROR: return "SYSTEM_ERROR";
    case XDS_C_UPDATE_ERROR: return "UPDATE_ERROR";
    default: return "UNKNOWN_ERROR";
    };
}

const char * xds_error_code_name(OM_enumeration code_id) {
    switch (code_id) {
    case DS_E_ADMIN_LIMIT_EXCEEDED: return "ADMIN_LIMIT_EXCEEDED";
    case DS_E_AFFECTS_MULTIPLE_DSAS: return "AFFECTS_MULTIPLE_DSAS";
    case DS_E_ALIAS_DEREFERENCING_PROBLEM: return "ALIAS_DEREFERENCING_PROBLEM";
    case DS_E_ALIAS_PROBLEM: return "ALIAS_PROBLEM";
    case DS_E_ATTRIBUTE_OR_VALUE_EXISTS: return "ATTRIBUTE_OR_VALUE_EXISTS";
    case DS_E_BAD_ARGUMENT: return "BAD_ARGUMENT";
    case DS_E_BAD_CLASS: return "BAD_CLASS";
    case DS_E_BAD_CONTEXT: return "BAD_CONTEXT";
    case DS_E_BAD_NAME: return "BAD_NAME";
    case DS_E_BAD_SESSION: return "BAD_SESSION";
    case DS_E_BAD_WORKSPACE: return "BAD_WORKSPACE";
    case DS_E_BUSY: return "BUSY";
    case DS_E_CANNOT_ABANDON: return "CANNOT_ABANDON";
    case DS_E_CHAINING_REQUIRED: return "CHANING_REQUIRED";
    case DS_E_COMMUNICATIONS_PROBLEM: return "COMMUNICATIONS_PROBLEM";
    case DS_E_CONSTRAINT_VIOLATION: return "CONSTRAINT_VIOLATION";
    case DS_E_DIT_ERROR: return "DIT_ERROR";
    case DS_E_ENTRY_EXISTS: return "ENTRY_EXISTS";
    case DS_E_INAPPROP_AUTHENTICATION: return "INAPPROP_AUTHENTICATION";
    case DS_E_INAPPROP_MATCHING: return "INAPPROP_MATCHING";
    case DS_E_INSUFFICIENT_ACCESS_RIGHTS: return "INSUFFICIENT_ACCESS_RIGHTS";
    case DS_E_INVALID_ATTRIBUTE_SYNTAX: return "INVALID_ATTRIBUTE_SYNTAX";
    case DS_E_INVALID_ATTRIBUTE_VALUE: return "INVALID_ATTRIBUTE_VALUE";
    case DS_E_INVALID_CREDENTIALS: return "INVALID_CREDENTIALS";
    case DS_E_INVALID_REF: return "INVALID_REF";
    case DS_E_INVALID_SIGNATURE: return "INVALID_SIGNATURE";
    case DS_E_LOOP_DETECTED: return "LOOP_DETECTED";
    case DS_E_MISCELLANEOUS: return "MISCELLANEOUS";
    case DS_E_MISSING_TYPE: return "MISSING_TYPE";
    case DS_E_MIXED_SYNCHRONOUS: return "MIXED_SYNCHRONOUS";
    case DS_E_NAMING_VIOLATION: return "NAMING_VIOLATION";
    case DS_E_NO_INFO: return "NO_INFO";
    case DS_E_NO_SUCH_ATTRIBUTE_OR_VALUE: return "NO_CUSH_ATTRIBUTE_OR_VALUE";
    case DS_E_NO_SUCH_OBJECT: return "NO_SUCH_OBJECT";
    case DS_E_NO_SUCH_OPERATION: return "NO_SUCH_OPERATION";
    case DS_E_NOT_ALLOWED_ON_NON_LEAF: return "NOT_ALLOWED_ON_NON_LEAF";
    case DS_E_NOT_ALLOWED_ON_RDN: return "NOT_ALLOWED_ON_RDN";
    case DS_E_NOT_SUPPORTED: return "NOT_SUPPORTED";
    case DS_E_OBJECT_CLASS_MOD_PROHIB: return "OBJECT_CLASS_MOD_PROHIB";
    case DS_E_OBJECT_CLASS_VIOLATION: return "OBJECT_CLASS_VIOLATION";
    case DS_E_OUT_OF_SCOPE: return "OUT_OF_SCOPE";
    case DS_E_PROTECTION_REQUIRED: return "PROTECTION_REQUIRED";
    case DS_E_TIME_LIMIT_EXCEEDED: return "TIME_LIMIT_EXCEEDED";
    case DS_E_TOO_LATE: return "TOO_LATE";
    case DS_E_TOO_MANY_OPERATIONS: return "TOO_MANY_OPERATIONS";
    case DS_E_TOO_MANY_SESSIONS: return "TOO_MAnY_SESSIONS";
    case DS_E_UNABLE_TO_PROCEED: return "UNABLE_TO_PRECEED";
    case DS_E_UNAVAILABLE: return "UNAVAILABLE";
    case DS_E_UNAVAILABLE_CRIT_EXT: return "UNAVAILABLE_CRIT_EXT";
    case DS_E_UNDEFINED_ATTRIBUTE_TYPE: return "UNDEFINED_ATTRIBUTE_TYPE";
    case DS_E_UNWILLING_TO_PERFORM: return "UNWILLING_TO_PERFORM";
    default: return "UNKNOWN_ERROR_CODE";
    };
}


xds_status_t::xds_status_t(OM_object ds_status)
: om_translation_t(ds_status, 1) {
//    xds_status_t ** new_substatus;
    int i;
    unsigned32 error;    

    error_code = 0;
//    first_substatus = NULL;
//    next_substatus = NULL;
//    cur_substatus = NULL;
    attribute_oid.length = 0;
    attribute_oid.elements = NULL;

    ds_status = get_obj();

#ifdef XDS_DEBUG
fprintf(stderr, "xds_status::xds_status(%p)\n", ds_status);
#endif

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "xds_status::xds_status(0x%lx)", (unsigned long)ds_status));

    if (ds_status == NULL) {
// Success!
        error_class = XDS_C_SUCCESS;
        return;
    };

    if (ds_status->type != OM_CLASS) {
#ifdef XDS_DEBUG
fprintf(stderr,
"Type = %d, should be %d (OM_CLASS)\n",
ds_status->type,
OM_CLASS);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Type = %d, should be %d (OM_CLASS)",
                      ds_status->type,
                      OM_CLASS));
                      
        error_class = XDS_C_BAD_STATUS;
        return;
    };
#ifdef XDS_DEBUG
fprintf(stderr, "DS_status.class = OM_CLASS\n");
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "DS_status.class = OM_CLASS"));

    if ((ds_status->syntax & OM_S_SYNTAX) != OM_S_OBJECT_IDENTIFIER_STRING) {
#ifdef XDS_DEBUG
fprintf(stderr,
"SYntax = %d, should be %d (OM_S_OBJECT_IDENTIFIER_STRING)\n",
(ds_status->syntax & OM_S_SYNTAX),
OM_S_OBJECT_IDENTIFIER_STRING);
#endif

        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                  "Syntax = %d, should be %d (OM_S_OBJECT_IDENTIFIER_STRING)",
                      (ds_status->syntax & OM_S_SYNTAX),
                      OM_S_OBJECT_IDENTIFIER_STRING));

        error_class = XDS_C_BAD_STATUS;
        return;
    };
#ifdef XDS_DEBUG
fprintf(stderr, "DS_status.syntax = OM_S_OID\n");
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "DS_status.syntax = OM_S_OID"));

    if (OM_object_identifier_equal(ds_status->value.string,
                                   DS_C_ABANDON_FAILED)) {
        error_class = XDS_C_ABANDON_FAILED;
#ifdef XDS_DEBUG
fprintf(stderr, "DS_status.classValue = ABANDON_FAILED\n");
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                      "DS_status.classValue = ABANDON_FAILED"));

    } else if (OM_object_identifier_equal(ds_status->value.string,
                                   DS_C_REFERRAL)) {
// For the moment, we won't parse a DS_C_CONTINUATION_REFERENCE, as
// this error-class should never be returned by the XDS++ interface,
        error_class = XDS_C_REFERRAL;
#ifdef XDS_DEBUG
fprintf(stderr, "DS_status.classValue = REFERRAL\n");
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug3,
                      "DS_status.classValue = REFERRAL"));

    } else if (OM_object_identifier_equal(ds_status->value.string,
                                   DS_C_SECURITY_ERROR)) {
        error_class = XDS_C_SECURITY_ERROR;
#ifdef XDS_DEBUG
fprintf(stderr, "DS_status.classValue = SECURITY_ERROR\n");
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                      "DS_status.classValue = SECURITY_ERROR"));
    } else if (OM_object_identifier_equal(ds_status->value.string,
                                   DS_C_SERVICE_ERROR)) {
        error_class = XDS_C_SERVICE_ERROR;
#ifdef XDS_DEBUG
fprintf(stderr, "DS_status.classValue = SERVICE_ERROR\n");
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                      "DS_status.classValue = SERVICE_ERROR"));
    } else if (OM_object_identifier_equal(ds_status->value.string,
                                   DS_C_SYSTEM_ERROR)) {
        error_class = XDS_C_SYSTEM_ERROR;
#ifdef XDS_DEBUG
fprintf(stderr, "DS_status.classValue = SYSTEM_ERROR\n");
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                      "DS_status.classValue = SYSTEM_ERROR"));
    } else if (OM_object_identifier_equal(ds_status->value.string,
                                   DS_C_UPDATE_ERROR)) {
        error_class = XDS_C_UPDATE_ERROR;
#ifdef XDS_DEBUG
fprintf(stderr, "DS_status.classValue = UPDATE_ERROR\n");
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                      "DS_status.classValue = UPDATE_ERROR"));
    } else if (OM_object_identifier_equal(ds_status->value.string,
                                   DS_C_COMMUNICATIONS_ERROR)) {
        error_class = XDS_C_COMMUNICATIONS_ERROR;
#ifdef XDS_DEBUG
fprintf(stderr, "DS_status.classValue = COMMUNICATIONS_ERROR\n");
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                      "DS_status.classValue = COMMUNICATIONS_ERROR"));
    } else if (OM_object_identifier_equal(ds_status->value.string,
                                   DS_C_LIBRARY_ERROR)) {
        error_class = XDS_C_LIBRARY_ERROR;
#ifdef XDS_DEBUG
fprintf(stderr, "DS_status.classValue = LIBRARY_ERROR\n");
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                      "DS_status.classValue = LIBRARY_ERROR"));
    } else if (OM_object_identifier_equal(ds_status->value.string,
                                   DS_C_NAME_ERROR)) {
        error_class = XDS_C_NAME_ERROR;
#ifdef XDS_DEBUG
fprintf(stderr, "DS_status.classValue = NAME_ERROR\n");
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                      "DS_status.classValue = NAME_ERROR"));
    } else if (OM_object_identifier_equal(ds_status->value.string,
                                   DS_C_ATTRIBUTE_ERROR)) {
        error_class = XDS_C_ATTRIBUTE_ERROR;
#ifdef XDS_DEBUG
fprintf(stderr, "DS_status.classValue = ATTRIBUTE_ERROR\n");
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                      "DS_status.classValue = ATTRIBUTE_ERROR"));
    } else {
#ifdef XDS_DEBUG
fprintf(stderr, "Unrecognized status type OID\n");
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug6,
                      "Unrecognized status type OID"));
        error_class = XDS_C_BAD_STATUS;
    };
    
// Now we need to parse the remaining elements (if any).

    i = 1;
    while (ds_status[i].type != OM_NO_MORE_TYPES) {
        switch (ds_status[i].type) {
        case DS_PROBLEM:
#ifdef XDS_DEBUG
fprintf(stderr, "Parsing a DS_PROBLEM Attribute\n");
fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "Parsing a DS_PROBLEM Attribute"));
            if ((ds_status[i].syntax & OM_S_SYNTAX) != OM_S_ENUMERATION) {
                error_code = -1;
                return;
            };
            error_code = ds_status[i].value.enumeration;
            break;
        case DS_OBJECT_NAME:
        case DS_MATCHED:
#ifdef XDS_DEBUG
fprintf(stderr, "Parsing a DS_MATCHED or DS_OBJECT_NAME Attribute\n");
fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Parsing a DS_MATCHED or DS_OBJECT_NAME Attribute"));
            if ((ds_status[i].syntax & OM_S_SYNTAX) != OM_S_OBJECT) {
#ifdef XDS_DEBUG
fprintf(stderr,
        "Error: Unexpected name syntax (%d)\n",
        (ds_status[i].syntax & OM_S_SYNTAX));
fflush(stderr);
#endif
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                              "Error: Unexpected name syntax (%d)",
                              (ds_status[i].syntax & OM_S_SYNTAX)));
                error_code = -1;
                return;
            };
            if (error = problem_name.set_value(ds_status[i].value.object.object)) {
#ifdef XDS_DEBUG
fprintf(stderr,
        "Error: Couldn't decode error object-name (%u)\n",
        error);
fflush(stderr);
#endif
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug4,
                              "Error: Couldn't decode error object-name (%u)",
                              error));
                error_code = -2;
                return;
            };
            break;
// Other error-types not yet handled in detail...
        };
        i++;
    };
#ifdef XDS_DEBUG
fprintf(stderr, "Done with status code!\n");
fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Done with status code!"));
}

xds_status_t::xds_status_t(void) {
    error_code = 0;
//    first_substatus = NULL;
//    next_substatus = NULL;
//    cur_substatus = NULL;
    attribute_oid.length = 0;
    attribute_oid.elements = NULL;
}

xds_status_t::~xds_status_t() {
#ifdef XDS_DEBUG
    fprintf(stderr, "xds_status_t::~xds_status_t()\n");
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "xds_status_t::~xds_status_t()"));
    if (attribute_oid.elements) {
#ifdef XDS_DEBUG
        fprintf(stderr, "xds_status_t::~xds_status_t(), freeing attribute_OID\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "xds_status_t::~xds_status_t(), freeing attribute_OID"));
        pkc_free(attribute_oid.elements);
    };
}

xds_status_t & xds_status_t::operator = (const xds_status_t & o) {
//    xds_status_t * substatus_ptr = o.first_substatus;
//    xds_status_t ** new_substatus;
//    cur_substatus = NULL;
//    next_substatus = NULL;

// Copy the chain of substatii...
//    first_substatus = NULL;
//    new_substatus = &first_substatus;
//    while (substatus_ptr) {
//        *new_substatus = new xds_status_t;
//        **new_substatus = *substatus_ptr;
//        new_substatus = &((*new_substatus)->next_substatus);
//        *new_substatus = NULL;
//        substatus_ptr = substatus_ptr->next_substatus;
//    };

// Copy the attribute OID...
    attribute_oid.length = o.attribute_oid.length;
    attribute_oid.elements = (unsigned char *)pkc_malloc(attribute_oid.length);
    memcpy(attribute_oid.elements, 
           o.attribute_oid.elements, 
           attribute_oid.length);

// Copy the name...
    if (o.problem_name.is_set()) problem_name = o.problem_name;

    return *this;
}

int xds_status_t::isSuccess(void) const {
    return error_class == XDS_C_SUCCESS;
}

//unsigned32 xds_status_t::get_first_attribute_problem(
//                            const xds_status_t * & status) const {
//    xds_status_t * var_this = (xds_status_t *)this;
//    status = first_substatus;
//    if (first_substatus == NULL) return pkc_s_no_more_items;
//    var_this->cur_substatus = first_substatus ->next_substatus;
//    return 0;
//}

//unsigned32 xds_status_t::get_next_attribute_problem(
//                            const xds_status_t * & status) const {
//    xds_status_t * var_this = (xds_status_t *)this;
//    status = cur_substatus;
//    if (cur_substatus == NULL) return pkc_s_no_more_items;
//    var_this->cur_substatus = cur_substatus ->next_substatus;
//    return 0;
//}

unsigned32 xds_status_t::get_attribute_oid(
                            const OM_object_identifier * & oid) const {
    oid = NULL;
    if (attribute_oid.length != 0) oid = &attribute_oid;
    else return pkc_s_no_value;
    return 0;
}

unsigned32 xds_status_t::get_name(const x500name * & name) const {
    name = NULL;
    if (problem_name.is_set()) name = &problem_name;
    else return pkc_s_no_value;
    return 0;
}




pthread_once_t xds_t::once_block = pthread_once_init;


static DS_feature features[] = {
    { OM_STRING(OMP_O_DS_BASIC_DIR_CONTENTS_PKG) , OM_TRUE },
    { OM_STRING(OMP_O_DSX_GDS_PKG) , OM_TRUE },
    { { (OM_uint32)0, (void *)0 }, OM_FALSE },
      };    

void xds_t::initialize_xds(void) {

    DS_status status;
        
#ifdef XDS_DEBUG
    fprintf(stderr, "Initializing XDS library\n");
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                  "Initializing XDS library"));

    workspace = ds_initialize();
    if (workspace == NULL) {
#ifdef XDS_DEBUG
        fprintf(stderr, "Error initializing XDS - ds_initialize failed\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Error initializing XDS - ds_initialize failed"));
        return;
    };
// Now we have to negotiate use of the BDCP and GDS packages...
    if (status = ds_version(features, workspace)) {
#ifdef XDS_DEBUG
        fprintf(stderr, "Error negotiating version\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Error negotiating XDS version"));
        return;
    };

    cdsIsSupported = (features[0].activated != 0);
    gdsIsSupported = (features[1].activated != 0);
    
}

xds_t::xds_t(void) {
    DS_status status;
    pkc_init_svc_handle();

    session = NULL;
    error_status = 0;
#ifdef XDS_DEBUG
    fprintf(stderr, "Creating an XDS instance...\n");
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Creating an XDS instance..."));
    pthread_once(&once_block, initialize_xds);
    if (error_status) {
#ifdef XDS_DEBUG
        fprintf(stderr, "Non-zero error-status, aborting initialization...\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Non-zero error-status (%d), aborting initialization",
		      error_status));
        return;
    };

    status = ds_bind(DS_DEFAULT_SESSION,
                     workspace,
                     &session);
    if (status != NULL) {
#ifdef XDS_DEBUG
        fprintf(stderr, "Error initializing XDS - ds_bind returned error\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Error initializing XDS - ds_bind returned error"));
        error_status = pkc_s_init_error;
        om_delete(status);    
    };
}

xds_t::~xds_t() {
    DS_status status;
#ifdef XDS_DEBUG
    fprintf(stderr, "Unbinding session\n");
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Unbinding session"));
    status = ds_unbind(session);
    if (status) {
#ifdef XDS_DEBUG
        fprintf(stderr, "Unbind failed\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Unbind failed"));
        om_delete(status);    
    };
}

xds_status_t * xds_t::list(const x500name & name, xds_list_result_t *& results) {
    OM_object om_name;
    unsigned32 error_status;
    OM_private_object results_return = NULL;
    OM_sint invoke_id_return;
    DS_status status;
    xds_status_t * status_ptr;
    OM_return_code om_return_code;
    OM_public_object public_results_return;

    if (error_status = name.xom_object(om_name)) {
#ifdef XDS_DEBUG
        fprintf(stderr,
                "Error %d converting X500 name to OM_object\n",
                error_status);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Error 0x%x converting X500 name to OM_object",
                      error_status));
    };

#ifdef XDS_DEBUG
fprintf(stderr, "DEBUG: Just converted name to OM_name:\n");
#ifdef DEC_XDS_DEBUG
parse_object(om_name);
#endif
fprintf(stderr, "\n\nCalling xds::list()...\n");
fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Calling xds::list()..."));
    status = ds_list(session,
                     DS_DEFAULT_CONTEXT,
                     om_name,
                     &results_return,
                     &invoke_id_return);
#ifdef XDS_DEBUG
    fprintf(stderr, "Returned from ds_list, status = %p\n", status);
    fflush(stderr);
#endif

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Returned from ds_list, status = %p",
                  status));

    status_ptr = new xds_status_t(status);

#ifdef XDS_DEBUG
    fprintf(stderr, "Translated status code\n");
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Translated status code"));
    
    if (status_ptr->isSuccess()) {
#ifdef XDS_DEBUG
        fprintf(stderr, "Translating results...\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Translating results..."));
        results = new xds_list_result_t(results_return);
    };
    if (results_return) {
#ifdef XDS_DEBUG
        fprintf(stderr, "Deleting OM-format results from list...\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Deleting OM-format results from list..."));
        om_delete(results_return);
    };
    if (status) {
#ifdef XDS_DEBUG
        fprintf(stderr, "Deleting OM-format status from list...\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Deleting OM-format status from list..."));
        om_delete(status);
    };
#ifdef XDS_DEBUG
    fprintf(stderr, "Leaving xds_t::list()\n");
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Leaving xds_t::list()"));
    return status_ptr;
}

xds_status_t * xds_t::modify(const x500name & name,
                             const xds_changes_t & changes) {


    OM_object om_name;
    OM_object om_changes;
    unsigned32 error_status;
    OM_sint invoke_id_return;
    DS_status status;
    xds_status_t * status_ptr;

    if (error_status = name.xom_object(om_name)) {
#ifdef XDS_DEBUG
        fprintf(stderr,
                "Error %d converting X500 name to OM_object\n",
                error_status);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Error %d converting X500 name to OM_object",
                      error_status));
	return NULL;
    };

    if (error_status = changes.build_xom_object(om_changes)) {
#ifdef XDS_DEBUG
        fprintf(stderr,
                "Error 0x%x converting changes to OM_object\n",
                error_status);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Error 0x%x converting changes to OM_object",
                      error_status));
	return NULL;
    };

#ifdef XDS_DEBUG
fprintf(stderr, "Modifiying object:\n");
#ifdef DEC_XDS_DEBUG
parse_object(om_name);
#endif
fprintf(stderr, "Using change-list:\n");
#ifdef DEC_XDS_DEBUG
parse_object(om_changes);
#endif
fflush(stderr);
#endif
    
    status = ds_modify_entry(session,
                             DS_DEFAULT_CONTEXT,
                             om_name,
                             om_changes,
                             &invoke_id_return);
    

    status_ptr = new xds_status_t(status);

#ifdef XDS_DEBUG
    fprintf(stderr, "Translated status code\n");
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Translated status code"));

    if (status) {
#ifdef XDS_DEBUG
        fprintf(stderr, "Deleting OM-format status...\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Deleting OM-format status..."));
        om_delete(status);
    };

    if ((error_status = changes.delete_xom_object()) != 0) {
#ifdef XDS_DEBUG
        fprintf(stderr,
                "Error %d deleting change_list object\n",
                error_status);
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Error 0x%x deleting change_list object",
                      error_status));
    };

    return status_ptr;
}

xds_status_t * xds_t::modify_rdn(const x500name & name,
                                 const RelativeDistinguishedName & new_rdn,
                                 int delete_old_rdn) {
    OM_object om_name;
    OM_object om_rdn;
    OM_boolean delete_flag;
    unsigned32 error_status;
    OM_private_object results_return = NULL;
    OM_sint invoke_id_return;
    DS_status status;
    xds_status_t * status_ptr;

    delete_flag = (delete_old_rdn?OM_TRUE:OM_FALSE);

    if (error_status = name.xom_object(om_name)) {
#ifdef XDS_DEBUG
        fprintf(stderr,
                "Error %d converting X500 name to OM_object\n",
                error_status);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Error 0x%x converting X500 name to OM_object",
                      error_status));
	return NULL;
    };

    if (error_status = new_rdn.xom_object(om_rdn)) {
#ifdef XDS_DEBUG
        fprintf(stderr,
                "Error %d converting RDN to OM_object\n",
                error_status);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Error 0x%x converting RDN to OM_object",
                      error_status));
	return NULL;
    };

    
    status = ds_modify_rdn(session,
                           DS_DEFAULT_CONTEXT,
                           om_name,
                           om_rdn,
                           delete_flag,
                           &invoke_id_return);
    

    status_ptr = new xds_status_t(status);

#ifdef XDS_DEBUG
    fprintf(stderr, "Translated status code\n");
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Translated status code"));

    if (status) {
#ifdef XDS_DEBUG
        fprintf(stderr, "Deleting OM-format status...\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Deleting OM-format status..."));
        om_delete(status);
    };

    return status_ptr;

}

xds_status_t * xds_t::read(const x500name & name,
                           const xds_selection_t & selection,
                           xds_read_result_t *& results) {
    OM_object om_name;
    unsigned32 error_status;
    OM_private_object results_return = NULL;
    OM_sint invoke_id_return;
    DS_status status;

    xds_status_t * status_ptr;
    OM_return_code om_return_code;
    OM_public_object public_results_return;
//    om_translation_t * trans;

    if (error_status = name.xom_object(om_name)) {
#ifdef XDS_DEBUG
        fprintf(stderr,
                "Error %d converting X500 name to OM_object\n",
                error_status);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Error 0x%x converting X500 name to OM_object",
                      error_status));
    };

#ifdef XDS_DEBUG
fprintf(stderr, "DEBUG: Just converted name to OM_name:\n");
#ifdef DEC_XDS_DEBUG
parse_object(om_name);
#endif
fprintf(stderr, "\n\nCalling xds::read()...\n");
fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Calling xds::read()"));


    status = ds_read(session,
                     DS_DEFAULT_CONTEXT,
                     om_name,
                     selection.xom_object,
                     &results_return,
                     &invoke_id_return);

    status_ptr = new xds_status_t(status);

#ifdef XDS_DEBUG
    fprintf(stderr, "Translated status code\n");
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Translated status code"));
    
    if (status_ptr->isSuccess()) {

#ifdef XDS_DEBUG
        fprintf(stderr, "Translating results...\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Translating results"));
        results = new xds_read_result_t(results_return);
    };
    if (results_return) {
#ifdef XDS_DEBUG
        fprintf(stderr, "Deleting OM-format results...\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Deleting OM-format results"));
        om_delete(results_return);
    };
    if (status) {
#ifdef XDS_DEBUG
        fprintf(stderr, "Deleting OM-format status...\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Deleting OM-format status"));
        om_delete(status);
    };
#ifdef XDS_DEBUG
    fprintf(stderr, "Leaving xds_t::read()\n");
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Leaving xds_t::read()"));
    return status_ptr;
}

xds_status_t * xds_t::remove_entry(const x500name & name) {

    OM_object om_name;
    unsigned32 error_status;
    OM_sint invoke_id_return;
    DS_status status;
    xds_status_t * status_ptr;

    if (error_status = name.xom_object(om_name)) {
#ifdef XDS_DEBUG
        fprintf(stderr,
                "Error %d converting X500 name to OM_object\n",
                error_status);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Error 0x%x converting X500 name to OM_object",
                      error_status));
    };

    status = ds_remove_entry(session,
                             DS_DEFAULT_CONTEXT,
                             om_name,
                             &invoke_id_return);

    status_ptr = new xds_status_t(status);

#ifdef XDS_DEBUG
    fprintf(stderr, "Translated status code\n");
    fflush(stderr);
#endif    
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Translated status code"));
    if (status) {
#ifdef XDS_DEBUG
        fprintf(stderr, "Deleting OM-format status...\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Deleting OM-format status"));
        om_delete(status);
    };
#ifdef XDS_DEBUG
    fprintf(stderr, "Leaving xds_t::remove_entry()\n");
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Leaving xds_t::remove_entry()"));
    return status_ptr;

}

xds_status_t * xds_t::add_entry(const x500name & name,
                                const xds_attribute_list_t & entry) {
    
    OM_object om_name;
    OM_object om_entry;
    unsigned32 error_status;
    OM_sint invoke_id_return;
    DS_status status;

    xds_status_t * status_ptr;
    OM_return_code om_return_code;

    if (error_status = name.xom_object(om_name)) {
#ifdef XDS_DEBUG
        fprintf(stderr,
                "Error %d converting X500 name to OM_object\n",
                error_status);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Error 0x%x converting X500 name to OM_object",
                      error_status));
    };

#ifdef XDS_DEBUG
fprintf(stderr, "DEBUG: Just converted name to OM_name:\n");
#ifdef DEC_XDS_DEBUG
parse_object(om_name);
#endif
#endif
    if (error_status = entry.build_xom_object(om_entry)) {
#ifdef XDS_DEBUG
        fprintf(stderr,
                "Error %d converting attribute-list to OM_object\n",
                error_status);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                     "Error 0x%x converting attribute-list to OM_object",
                     error_status));
    };

#ifdef XDS_DEBUG
fprintf(stderr, "DEBUG: Just built an OM_entry object:\n");
#ifdef DEC_XDS_DEBUG
parse_object(om_entry);
#endif
fprintf(stderr, "\n\nCalling ds_add_entry()...\n");
fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Calling ds_add_entry()"));
    status = ds_add_entry(session,
                          DS_DEFAULT_CONTEXT,
                          om_name,
                          om_entry,
                          &invoke_id_return);

    status_ptr = new xds_status_t(status);

#ifdef XDS_DEBUG
    fprintf(stderr, "Translated status code\n");
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Translated status code"));
    
    if (status) {
#ifdef XDS_DEBUG
        fprintf(stderr, "Deleting OM-format status...\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Deleting OM-format status"));
        om_delete(status);
    };

    if ((error_status = entry.delete_xom_object()) != 0) {
#ifdef XDS_DEBUG
        fprintf(stderr,
                "Error %d deleting attribute_list object\n",
                error_status);
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Error 0x%x deleting attribute_list object",
                      error_status));
    };

#ifdef XDS_DEBUG
    fprintf(stderr, "Leaving xds_t::add_entry()\n");
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Leaving xds_t::add_entry()"));
    return status_ptr;
}


xds_status_t * xds_t::add_entry(const x500name & name) {
    
    OM_object om_name;
    OM_descriptor om_entry[2];
    unsigned32 error_status;
    OM_sint invoke_id_return;
    DS_status status;

    xds_status_t * status_ptr;
    OM_return_code om_return_code;

    if (error_status = name.xom_object(om_name)) {
#ifdef XDS_DEBUG
        fprintf(stderr,
                "Error %d converting X500 name to OM_object\n",
                error_status);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Error 0x%x converting X500 name to OM_object",
                      error_status));
    };

#ifdef XDS_DEBUG
fprintf(stderr, "DEBUG: Just converted name to OM_name:\n");
#ifdef DEC_XDS_DEBUG
parse_object(om_name);
#endif
#endif

    om_entry[0].type = OM_CLASS;
    om_entry[0].syntax = OM_S_OBJECT_IDENTIFIER_STRING;
    om_entry[0].value.string.length = OMP_LENGTH(DS_C_ATTRIBUTE_LIST);
    om_entry[0].value.string.elements = OMP_D_DS_C_ATTRIBUTE_LIST;

    om_entry[1].type = 0;
    om_entry[1].syntax = 0;
    om_entry[1].value.string.length = 0;
    om_entry[1].value.string.elements = NULL;

#ifdef XDS_DEBUG
fprintf(stderr, "\n\nCalling ds_add_entry()...\n");
fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Calling ds_add_entry()..."));
    status = ds_add_entry(session,
                          DS_DEFAULT_CONTEXT,
                          om_name,
                          om_entry,
                          &invoke_id_return);

    status_ptr = new xds_status_t(status);

#ifdef XDS_DEBUG
    fprintf(stderr, "Translated status code\n");
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Translated status code"));
    
    if (status) {
#ifdef XDS_DEBUG
        fprintf(stderr, "Deleting OM-format status...\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Deleting OM-format status"));
        om_delete(status);
    };

/*									    
    if ((error_status = entry.delete_xom_object()) != 0) {
#ifdef XDS_DEBUG
        fprintf(stderr,
                "Error %d deleting attribute_list object\n",
                error_status);
        fflush(stderr);
#endif
    };
*/
#ifdef XDS_DEBUG
    fprintf(stderr, "Leaving xds_t::add_entry()\n");
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Leaving xds_t::add_entry()"));
    return status_ptr;
}

xds_status_t * xds_t::search(const x500name & base,
                             subset_t subset,
                             const xds_filter_t & filter,
                             int search_aliases,
                             const xds_selection_t & selection,
                             xds_search_result_t *& results) {
    return NULL;
}


xds_search_result_t::~xds_search_result_t() {
}

om_translation_t::~om_translation_t() {
    if (copied_object != NULL) {
#ifdef XDS_DEBUG
        fprintf(stderr, "om_translation_t::~om_translation_t(), deleting copied_object\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
              "om_translation_t::~om_translation_t(), deleting copied_object"));
        om_delete(copied_object);
#ifdef XDS_DEBUG
        fprintf(stderr, "Deleted copied_object\n");
        fflush(stderr);
    } else {
        fprintf(stderr, "~om_translation_t: No copied object to delete\n");
        fflush(stderr);
#endif
    };
}

om_translation_t::om_translation_t(void) {
#ifdef XDS_DEBUG
        fprintf(stderr, "om_translation_t(void)\n");
        fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "om_translation_t(void)"));
    copied_object = NULL;
    original_object = NULL;
}

om_translation_t::om_translation_t(OM_object & obj, int force) {
    OM_return_code om_return_code;
    OM_value_position descriptor_count;

#ifdef XDS_DEBUG
        fprintf(stderr, "om_translation_t(%p)\n", obj);
        fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "om_translation_t(%p)", obj));

    copied_object = NULL;
    original_object = obj;

    if (obj == NULL) {
#ifdef XDS_DEBUG
        fprintf(stderr, "om_translation_t(void object), returning\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "om_translation_t(void object), returning"));
	return;
    };
    if (force || (obj->type == OM_PRIVATE_OBJECT)) {
#ifdef XDS_DEBUG
        fprintf(stderr,
                "om_translation_t(private object), translating via om_get\n");
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "om_translation_t(private object), translating via om_get"));
// We have to get a public copy of the object before we can parse it.
        om_return_code = om_get(obj,
                                OM_NO_EXCLUSIONS,
                                NULL,
                                0,
                                NULL,
                                NULL,
                                &copied_object,
                                &descriptor_count);
        if (om_return_code != OM_SUCCESS) {
#ifdef XDS_DEBUG
            fprintf(stderr,
                    "OM_get() returned error %d in om_translation_t\n",
                    om_return_code);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "OM_get() returned error 0x%x in om_translation_t",
                          om_return_code));
        };
        obj = copied_object;
#ifdef XDS_DEBUG
        fprintf(stderr, "Translated object is %p\n", copied_object);
        fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Translated object is %p", copied_object));
    };
}

xds_result_t::xds_result_t(OM_object obj) : om_translation_t(obj) {
    int i;

    error_status = 0;
    AliasWasDereferenced = 0;

    obj = get_obj();
#ifdef XDS_DEBUG
        fprintf(stderr, "xds_result_t::xds_result_t()\n");
        fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "xds_result_t::xds_result_t()"));
    
    if (obj == NULL) return;
    scan_object(obj);
}

void xds_result_t::scan_object(OM_object obj) {

    int i;
    i = 0;
#ifdef XDS_DEBUG
    fprintf(stderr, "Beginning SCAN_OBJECT for xds_result_t object(%p)\n", obj);
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Beginning SCAN_OBJECT for xds_result_t object(%p)", obj));
    while (obj[i].type != OM_NO_MORE_TYPES) {
        switch (obj[i].type) {
        case OM_CLASS:
#ifdef XDS_DEBUG
	    fprintf(stderr, "Found OM_CLASS tag (%d)\n", OM_CLASS);
	    fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
	                  "Found OM_CLASS tag (%d)", OM_CLASS));
            break;
        case DS_ALIAS_DEREFERENCED:
#ifdef XDS_DEBUG
            fprintf(stderr,
	            "Found DS_ALIAS_DEREFERENCED tag (%d)\n",
                    DS_ALIAS_DEREFERENCED);
            fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "Found DS_ALIAS_DEREFERENCED tag (%d)",
                          DS_ALIAS_DEREFERENCED));
            if ((obj[i].syntax & OM_S_SYNTAX) != OM_S_BOOLEAN) {
                error_status = pkc_s_conversion_error;
                return;
            };
            AliasWasDereferenced = (obj[i].value.boolean != 0);
#ifdef XDS_DEBUG
            fprintf(stderr,
	            "Value (C boolean): %d\n", AliasWasDereferenced);
            fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
	                  "Value (C boolean): %d", AliasWasDereferenced));
            break;
        default:
	    ;
#ifdef XDS_DEBUG
            fprintf(stderr,
                    "Unrecognized xds_result_t component, type = %d\n",
                    obj[i].type);
            fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                    "Unrecognized xds_result_t component, type = %d",
                    obj[i].type));
        };
        i++;
    };
#ifdef XDS_DEBUG
    fprintf(stderr, "Done SCAN_OBJECT for xds_result_t object(%p)\n", obj);
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Done SCAN_OBJECT for xds_result_t object(%p)", obj));
}

xds_list_result_t::~xds_list_result_t() {
    if (first_item) delete first_item;
}



unsigned32 xds_list_result_t::get_first_item(
                                const xds_list_info_item_t * & result) const {
    xds_list_result_t * var_this = (xds_list_result_t *)this;
    result = first_item;
    if (first_item == NULL) return pkc_s_no_more_items;
    var_this->cur_item = first_item->next_item;
    return 0;
}

unsigned32 xds_list_result_t::get_next_item(
                                const xds_list_info_item_t * & result) const {
    xds_list_result_t * var_this = (xds_list_result_t *)this;
    result = cur_item;
    if (cur_item == NULL) return pkc_s_no_more_items;
    var_this->cur_item = cur_item->next_item;
    return 0;
}


xds_list_info_item_t::~xds_list_info_item_t() {
    if (next_item) delete next_item;
}

xds_list_info_item_t::xds_list_info_item_t(OM_object obj) {
    int i = 0;

    error_status = 0;
    next_item = NULL;

#ifdef XDS_DEBUG
fprintf(stderr, "Constructing an xds_list_info_item_t from object %p\n", obj);
fprintf(stderr, "Beginning scan of xds_list_info_item_t (%p)\n", obj);
fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Constructing an xds_list_info_item_t from object %p", obj));
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Beginning scan of xds_list_info_item_t (%p)", obj));
    
    while (obj[i].type != OM_NO_MORE_TYPES) {
        switch (obj[i].type) {
        case OM_CLASS:
#ifdef XDS_DEBUG
	    fprintf(stderr, "Found OM_CLASS tag\n");
	    fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
	                  "Found OM_CLASS tag"));
            break;
        case DS_ALIAS_ENTRY:
#ifdef XDS_DEBUG
fprintf(stderr, "Found DS_ALIAS_ENTRY.  Decoding...\n");
fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "Found DS_ALIAS_ENTRY.  Decoding..."));
            if ((obj[i].syntax & OM_S_SYNTAX) != OM_S_BOOLEAN) {
#ifdef XDS_DEBUG
                fprintf(stderr, "Found syntax %d, expected OM_S_BOOLEAN\n",
                        obj[i].syntax & OM_S_SYNTAX);
#endif
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                              "Found syntax %d, expected OM_S_BOOLEAN",
                              obj[i].syntax & OM_S_SYNTAX));
                error_status = pkc_s_conversion_error;
                return;
            };
            entryIsAlias = (obj[i].value.boolean != 0);
            break;
        case DS_FROM_ENTRY:
#ifdef XDS_DEBUG
fprintf(stderr, "Found DS_FROM_ENTRY.  Decoding...\n");
fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "Found DS_FROM_ENTRY.  Decoding..."));
            if ((obj[i].syntax & OM_S_SYNTAX) != OM_S_BOOLEAN) {
#ifdef XDS_DEBUG
                fprintf(stderr, "Found syntax %d, expected OM_S_BOOLEAN\n",
                        obj[i].syntax & OM_S_SYNTAX);
#endif
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                              "Found syntax %d, expected OM_S_BOOLEAN",
                              obj[i].syntax & OM_S_SYNTAX));
                error_status = pkc_s_conversion_error;

                return;
            };
            wasFromEntry = (obj[i].value.boolean != 0);
            break;
        case DS_RDN:
#ifdef XDS_DEBUG
fprintf(stderr, "Found DS_RDN.  Decoding...\n");
fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "Found DS_RDN.  Decoding..."));
            if ((obj[i].syntax & OM_S_SYNTAX) != OM_S_OBJECT) {
#ifdef XDS_DEBUG
                fprintf(stderr, "Found syntax %d, expected OM_S_OBJECT\n",
                        obj[i].syntax & OM_S_SYNTAX);
#endif
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                              "Found syntax %d, expected OM_S_OBJECT",
                              obj[i].syntax & OM_S_SYNTAX));
                error_status = pkc_s_conversion_error;
                return;
            };
            error_status = rdn.set_value(obj[i].value.object.object);
            if (error_status) {
#ifdef XDS_DEBUG
                fprintf(stderr,
                        "RDN.set_value() returned %d\n",
                        error_status);
#endif
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                              "RDN.set_value() returned 0x%x",
                              error_status));
            };
            break;
	default:
#ifdef XDS_DEBUG
            fprintf(stderr,
               "Found unrecognized xds_list_info_item_t component, type = %d\n",
               obj[i].type);
            fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
               "Found unrecognized xds_list_info_item_t component, type = %d",
                          obj[i].type));
        };
        i++;
    };
#ifdef XDS_DEBUG
fprintf(stderr, "Finished scan of xds_list_info_item_t object (%p)\n", obj);
fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Finished scan of xds_list_info_item_t object (%p)", obj));
}

xds_list_result_t::xds_list_result_t(OM_object obj) : xds_result_t(obj) {
    int i;
    OM_object temp_obj;
    xds_list_info_item_t ** item_ptr = &first_item;

#ifdef XDS_DEBUG
fprintf(stderr, "Converting an xds_list_result_t\n");
fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Converting an xds_list_result_t"));

    obj = get_obj();

    temp_obj = obj;
    
    wasPartialOutcome = 0;
    first_item = NULL;
    cur_item = NULL;

    if (obj == NULL) {
#ifdef XDS_DEBUG
fprintf(stderr, "NULL object, returning...\n");
fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "NULL object, returning..."));
        return;
    };

    if (error_status) {
#ifdef XDS_DEBUG
fprintf(stderr, "Error converting xds_result_t parent, returning...\n");
fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Error converting xds_result_t parent, returning..."));
        return;
    };


#ifdef XDS_DEBUG
fprintf(stderr, "We have a valid results object to translate...\n");
fprintf(stderr, "Beginning scan for xds_list_result_t object(%p)\n", obj);
fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "We have a valid results object to translate..."));
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Beginning scan for xds_list_result_t object(%p)", obj));

    i = 0;
    while (obj[i].type != OM_NO_MORE_TYPES) {
        switch (obj[i].type) {
        case OM_CLASS:
#ifdef XDS_DEBUG
	    fprintf(stderr, "Found OM_CLASS tag (%d)\n", OM_CLASS);
	    fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "Found OM_CLASS tag (%d)\n", OM_CLASS));
            break;
        case DS_LIST_INFO:
#ifdef XDS_DEBUG
            fprintf(stderr,
                    "Found DS_LIST_INFO(%d).  Decoding...\n", DS_LIST_INFO);
            fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                         "Found DS_LIST_INFO(%d).  Decoding...", DS_LIST_INFO));

            if ((obj[i].syntax & OM_S_SYNTAX) != OM_S_OBJECT) {
#ifdef XDS_DEBUG
fprintf(stderr,
        "Unexpected object syntax (%d), expecting OM_S_OBJECT\n",
        obj[i].syntax);
fflush(stderr);
#endif
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Unexpected object syntax (%d), expecting OM_S_OBJECT",
                              obj[i].syntax));
                error_status = pkc_s_conversion_error;
                return;
            };
#ifdef XDS_DEBUG
            fprintf(stderr,
            "Re-directing object scan to 0 in sub-object and re-starting...\n");
	    {int j = i+1;
                fprintf(stderr,
		"The following entries within this object will be ignored:\n");
                if (obj[j].type == OM_NO_MORE_TYPES) {
		    fprintf(stderr, "   None\n");
		};
		while (obj[j].type != OM_NO_MORE_TYPES) {
		    fprintf(stderr, "   Type: %d\n", obj[j].type);
                   j++;
		};
            };
            fflush(stderr);
#endif
            obj = obj[i].value.object.object;
            xds_result_t::scan_object(obj);
            i = 0;
            continue;  // Continue (rather than break) avoids incrementing
		       // i, which has been zeroed in preparation for
		       // a scan restart.

        case DS_OBJECT_NAME:
#ifdef XDS_DEBUG
            fprintf(stderr,
                    "Found DS_OBJECT_NAME (%d).  Decoding...\n",
                    DS_OBJECT_NAME);
            fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "Found DS_OBJECT_NAME (%d).  Decoding...",
                          DS_OBJECT_NAME));
            if ((obj[i].syntax & OM_S_SYNTAX) != OM_S_OBJECT) {
#ifdef XDS_DEBUG
                fprintf(stderr,
                    "Unexpected object syntax (%d), expecting OM_S_OBJECT\n",
                    obj[i].syntax);
                fflush(stderr);
#endif
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                         "Unexpected object syntax (%d), expecting OM_S_OBJECT",
                              obj[i].syntax));
                error_status = pkc_s_conversion_error;
                return;
            };
            if (error_status =
                    object_name.set_value(obj[i].value.object.object)) {
#ifdef XDS_DEBUG
                fprintf(stderr, "name.set_value() returned %d\n", error_status);
                fflush(stderr);
#endif
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                              "name.set_value() returned 0x%x\n",
                              error_status));
                return;
            };
            break;
        case DS_PARTIAL_OUTCOME_QUAL:
#ifdef XDS_DEBUG
            fprintf(stderr,
                    "Found DS_PARTIAL_OUTCOME (%d).  Decoding...\n",
                    DS_PARTIAL_OUTCOME_QUAL);
            fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "Found DS_PARTIAL_OUTCOME (%d).  Decoding...\n",
                          DS_PARTIAL_OUTCOME_QUAL));
            wasPartialOutcome = 1;
            break;
        case DS_SUBORDINATES:
#ifdef XDS_DEBUG
            fprintf(stderr,
                    "Found DS_SUBORDINATES (%d).  Decoding...\n",
                    DS_SUBORDINATES);
            fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "Found DS_SUBORDINATES (%d).  Decoding...",
                          DS_SUBORDINATES));
            if ((obj[i].syntax & OM_S_SYNTAX) != OM_S_OBJECT) {
#ifdef XDS_DEBUG
                fprintf(stderr,
                       "Unexpected object syntax (%d), expecting OM_S_OBJECT\n",
                       obj[i].syntax);
                fflush(stderr);
#endif
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
                      "Unexpected object syntax (%d), expecting OM_S_OBJECT",
                              obj[i].syntax));
                error_status = pkc_s_conversion_error;
                return;
            };
#ifdef XDS_DEBUG
            fprintf(stderr,
                    "Beginning descent for subordinate object...\n");
            fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "Beginning descent for subordinate object..."));
            *item_ptr = new xds_list_info_item_t(obj[i].value.object.object);
#ifdef XDS_DEBUG
            fprintf(stderr,
                    "Got new xds_list_info_item_t (*item_ptr = %p)\n",
                    *item_ptr);
            fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Got new xds_list_info_item_t (*item_ptr = %p)",
                      *item_ptr));
#ifdef XDS_DEBUG
            fprintf(stderr,
                   "Returned from subordinate object parse\n");
            fprintf(stderr,
                    "Back in scan for xds_list_result_t object(%p)\n", obj);
            fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "Returned from subordinate object parse"));
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "Back in scan for xds_list_result_t object(%p)",
                          obj));
            if (error_status = (*item_ptr)->error_status) {
#ifdef XDS_DEBUG
                fprintf(stderr,
                        "info_item decode returned %d.  ABORTING PARSE.\n",
                        error_status);
                fflush(stderr);
#endif
                DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                            "info_item decode returned 0x%x.  ABORTING PARSE.",
                              error_status));
                return;
            };
#ifdef XDS_DEBUG
            fprintf(stderr, "xds_list_info_item_t created without error\n");
            fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "xds_list_info_item_t created without error"));
            item_ptr = &((*item_ptr)->next_item);
            break;
        default:
	    ;
#ifdef XDS_DEBUG
            fprintf(stderr,
                    "Unrecognized xds_list_result_t component, type = %d\n",
                    obj[i].type);
            fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                        "Unrecognized xds_list_result_t component, type = %d",
                          obj[i].type));
        };
        i++;
    };
#ifdef XDS_DEBUG
    fprintf(stderr, "Finished scan of xds_list_result_t(%p)\n", temp_obj);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Finished scan of xds_list_result_t(%p)", temp_obj));
#ifdef XDS_DEBUG
    if (obj != temp_obj) {
        fprintf(stderr,
                "At end of scan, current object was %p (not original object)\n",
                obj);
    };
    fflush(stderr);
#endif
    if (obj != temp_obj) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                 "At end of scan, current object was %p (not original object)",
                      obj));
    };
}


xds_read_result_t::~xds_read_result_t() {
    if (entry) delete entry;
}

xds_read_result_t::xds_read_result_t(OM_object obj) : xds_result_t(obj) {
    int i;

    entry = NULL;
    cur_attribute = NULL;

    obj = get_obj();
    
    if (obj == NULL) return;
    if (error_status) return;
    i = 0;
    while (obj[i].type != OM_NO_MORE_TYPES) {
        switch (obj[i].type) {
        case OM_CLASS:
            break;
        case DS_ENTRY:
#ifdef XDS_DEBUG
fprintf(stderr, "Converting a DS_ENTRY in the result of a ds_read()\n");
fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                         "Converting a DS_ENTRY in the result of a ds_read()"));
            if ((obj[i].syntax & OM_S_SYNTAX) != OM_S_OBJECT) {
                error_status = pkc_s_conversion_error;
                return;
            };
            entry = new xds_entry_t(obj[i].value.object.object);            
            break;
        default:
	    ;
#ifdef XDS_DEBUG
            fprintf(stderr,
                    "Unrecognized xds_read_result_t component, type = %d\n",
                    obj[i].type);
            fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                        "Unrecognized xds_read_result_t component, type = %d",
                          obj[i].type));
        };
        i++;
    };
#ifdef XDS_DEBUG
            fprintf(stderr,
                    "Done scan of xds_read_result_t object(%p)\n",
                    obj);
            fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Done scan of xds_read_result_t object(%p)",
                    obj));
}

xds_attribute_list_t::xds_attribute_list_t(void) : om_translation_t() {
    error_status = 0;
    first_attribute = NULL;
    xom_object = NULL;
}

unsigned32 xds_attribute_list_t::delete_xom_object(void) const {
    int i;
    int j;
    OM_object attribute;
    OM_object attributeValue;

    xds_attribute_list_t * var_this = (xds_attribute_list_t *) this;
    
    if (xom_object == NULL) return 0;
    i = 0;
    while (var_this->xom_object[i].type != OM_NO_MORE_TYPES) {
	if (var_this->xom_object[i].type == DS_ATTRIBUTES) {
	    attribute = var_this->xom_object[i].value.object.object;
	    j = 0;
	    while (attribute[j].type != OM_NO_MORE_TYPES) {
		if (attribute[j].type == DS_ATTRIBUTE_VALUES) {
		    switch (syntaxToClass(attribute[j].syntax)) {
		    case VC_CHARSTRING:
		    case VC_OCTETSTRING:
		    case VC_BITSTRING:
			attributeValue = attribute[j].value.object.object;
			pkc_free(attributeValue->value.string.elements);
			break;
		    default:
			break;
		    };
		};
		j++;
	    };
	    pkc_free(attribute);
	};
	i++;
    };
    pkc_free(var_this->xom_object);
    var_this->xom_object = NULL;
    return 0;
}

unsigned32 xds_attribute_list_t::build_xom_object(OM_object & o) const {
    xds_attribute_list_t * var_this = (xds_attribute_list_t *)this;
    xds_attribute_t * attrPtr = this->first_attribute;
    int attrCount = 0;
    xds_value_list_t * valuePtr;
    int valueCount;
    int i;
    int j;

    OM_object temp_object;

#ifdef XDS_DEBUG
fprintf(stderr, "DEBUG: xds_attribute_list_t::build_xom_object()\n");
fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "DEBUG: xds_attribute_list_t::build_xom_object()"));

    o = NULL;

// All class OIDs within the OM_object we're constructing aren't managed by
// the xom_object - if they're non-static, they belong to the corresonding
// xds_attribute_t.

    if (xom_object) {
#ifdef XDS_DEBUG
fprintf(stderr, "DEBUG: Already have an OM_object, deleting it\n");
fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Already have an OM_object, deleting it"));
	var_this->delete_xom_object();
    };
// Build a new OM_object representation of the entire attribute-list.
// First run along the list, counting elements...

    while (attrPtr != NULL) {
        attrCount++;
        attrPtr = attrPtr->next_attribute;        
    };

#ifdef XDS_DEBUG
fprintf(stderr, "DEBUG: %d attributes to encode\n", attrCount);
fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "%d attributes to encode\n", attrCount));

// Now attrCount contains the number of attributes.  Allocate an OM_object
// structure for them and encode...

    if ((var_this->xom_object =
            (OM_descriptor *)pkc_malloc(
                (attrCount + 2) * sizeof(OM_descriptor))) == NULL) {
        return pkc_s_nomem;
    };

    memset(var_this->xom_object,
           0,
           (attrCount + 2) * sizeof(OM_descriptor));

    var_this->xom_object[0].type = OM_CLASS;
    var_this->xom_object[0].syntax = OM_S_OBJECT_IDENTIFIER_STRING;
    var_this->xom_object[0].value.string.length =
        OMP_LENGTH(DS_C_ATTRIBUTE_LIST);
    var_this->xom_object[0].value.string.elements = OMP_D_DS_C_ATTRIBUTE_LIST;

    attrPtr = this->first_attribute;
    for (i=1; i<=attrCount; i++) {

#ifdef XDS_DEBUG
fprintf(stderr, "DEBUG: Building attribute %d\n", i);
fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Building attribute %d", i));

        var_this->xom_object[i].type = DS_ATTRIBUTES;
        var_this->xom_object[i].syntax = OM_S_OBJECT;

        valueCount = 0;
        valuePtr = attrPtr->first_attributeValue;

        while (valuePtr != NULL) {
            valueCount++;
            valuePtr = valuePtr->next_value;        
        };

#ifdef XDS_DEBUG
fprintf(stderr, "DEBUG: Attribute has %d values\n", valueCount);
fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Attribute has %d values", valueCount));

        if ((temp_object =
                (OM_descriptor *)pkc_malloc(
                    (valueCount + 3) * sizeof(OM_descriptor))) == NULL) {
            return pkc_s_nomem;
        };

        var_this->xom_object[i].value.object.object = temp_object;

        memset(temp_object,
               0,
               (valueCount + 3) * sizeof(OM_descriptor));

        temp_object[0].type = OM_CLASS;
        temp_object[0].syntax = OM_S_OBJECT_IDENTIFIER_STRING;
        temp_object[0].value.string.length =
            OMP_LENGTH(DS_C_ATTRIBUTE);
        temp_object[0].value.string.elements = OMP_D_DS_C_ATTRIBUTE;

	temp_object[1].type = DS_ATTRIBUTE_TYPE;
	temp_object[1].syntax = OM_S_OBJECT_IDENTIFIER_STRING;
	temp_object[1].value.string.length = attrPtr->attributeType.length;
	temp_object[1].value.string.elements = attrPtr->attributeType.elements;

        valuePtr = attrPtr->first_attributeValue;
        for (j=1; j<=valueCount; j++) {

#ifdef XDS_DEBUG
fprintf(stderr, "DEBUG: Adding value %d\n", j);
fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "DEBUG: Adding value %d", j));

	    temp_object[j+1].type = DS_ATTRIBUTE_VALUES;
	    temp_object[j+1].syntax = valuePtr->value->theValueSyntax;
	    switch (valuePtr->value->theValueType) {
	    case VC_NULL:
                break;
	    case VC_BOOLEAN:
                temp_object[j+1].value.boolean = (valuePtr->value->theBoolean
                                                  ?OM_TRUE
                                                  :OM_FALSE);
		break;
	    case VC_INTEGER:
                temp_object[j+1].value.integer = valuePtr->value->theInteger;
		break;
	    case VC_CHARSTRING:
	    case VC_OCTETSTRING:
	    case VC_BITSTRING:
                temp_object[j+1].value.string.length =
		                         valuePtr->value->theString.length;
                if ((temp_object[j+1].value.string.elements =
                        pkc_malloc(valuePtr->value->theString.length)
                    ) != NULL) {
                    memcpy(temp_object[j+1].value.string.elements,
                           valuePtr->value->theString.elements,
                           temp_object[j+1].value.string.length);
		} else {
		    return pkc_s_nomem;
                };
		break;
	    case VC_UNSUPPORTED:
	    default:
               return pkc_s_conversion_error;
	    };
	    valuePtr = valuePtr->next_value;
	};

        var_this->xom_object[i].type = DS_ATTRIBUTES;
    };        

    o = xom_object;
    return 0;

}

xds_attribute_list_t::xds_attribute_list_t(OM_object obj)
: om_translation_t(obj) {
    int i;
    xds_attribute_t ** attribute_ptr = &first_attribute;

    error_status = 0;
    first_attribute = NULL;
    xom_object = NULL;

    obj = get_obj();

    if (obj == NULL) return;

    i = 0;
    while (obj[i].type != OM_NO_MORE_TYPES) {
        switch (obj[i].type) {
        case OM_CLASS:
            break;
        case DS_ATTRIBUTES:
            if ((obj[i].syntax & OM_S_SYNTAX) != OM_S_OBJECT) {
                error_status = pkc_s_conversion_error;
                return;
            };
            *attribute_ptr = new xds_attribute_t(obj[i].value.object.object);
            if (error_status = (*attribute_ptr)->error_status) {
		return;
	    };
            attribute_ptr = &((*attribute_ptr)->next_attribute);
            break;
        default:
	    ;
#ifdef XDS_DEBUG
            fprintf(stderr,
                    "Unrecognized xds_attribute_list_t component, type = %d\n",
                    obj[i].type);
            fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                   "Unrecognized xds_attribute_list_t component, type = %d",
                          obj[i].type));
        };
        i++;
    };
#ifdef XDS_DEBUG
    fprintf(stderr,
            "Done scan of xds_attribute_list_t object(%p)\n",
            obj);
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Done scan of xds_attribute_list_t object(%p)",
                  obj));
}


unsigned32 xds_attribute_list_t::add_attribute(
                                      const OM_object_identifier & attr_type,
                                      const xds_value_list_t & values) {
    xds_attribute_t * attribute;

#ifdef XDS_DEBUG
fprintf(stderr, "xds_attribute_list_t::add_attribute\n");
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "xds_attribute_list_t::add_attribute"));

    attribute = new xds_attribute_t(attr_type, values);    

    attribute->next_attribute = first_attribute;
    first_attribute = attribute;
    return 0;

}

xds_attribute_t::xds_attribute_t(const OM_object_identifier & attr_type,
                                 const xds_value_list_t & values)
: om_translation_t() {

#ifdef XDS_DEBUG
fprintf(stderr, "DEBUG: xds_attribute_t::xds_attribute_t()\n");
fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "xds_attribute_t::xds_attribute_t()"));

    error_status = 0;
    next_attribute = NULL;
    first_attributeValue = NULL;
    attributeType.length = attr_type.length;
    if ((attributeType.elements = pkc_malloc(attr_type.length)) != NULL) {
        memcpy(attributeType.elements,
               attr_type.elements,
               attr_type.length);
    };

    values.copy(first_attributeValue);
}

unsigned32 xds_value_list_t::copy(xds_value_list_t * &newList) const {

#ifdef XDS_DEBUG
fprintf(stderr, "xds_value_list_t::copy()\n");
fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "xds_value_list_t::copy()"));

    newList = new xds_value_list_t;
    newList->value = new xds_value_t(*value);

    if (next_value) {
#ifdef XDS_DEBUG
fprintf(stderr, "Recursing for next value in list...\n");
fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Recursing for next value in list..."));
        next_value->copy(newList->next_value);
    } else {
#ifdef XDS_DEBUG
fprintf(stderr, "Copied a single value, returning...\n");
fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Copied a single value, returning..."));
        newList->next_value = NULL;
    };
    return 0;
}

xds_attribute_t::xds_attribute_t(OM_object obj) : om_translation_t(obj) {
    int i;
    xds_value_list_t ** attributeValue_ptr = &first_attributeValue;
    char* temp_str;

    next_attribute = NULL;
    first_attributeValue = NULL;
    attributeType.length = 0;
    attributeType.elements = NULL;
    error_status = 0;
    
    obj = get_obj();

    if (obj == NULL) return;

    i = 0;
    while (obj[i].type != OM_NO_MORE_TYPES) {
        switch (obj[i].type) {
        case OM_CLASS:
            break;
        case DS_ATTRIBUTE_TYPE:
            if ((obj[i].syntax & OM_S_SYNTAX) != OM_S_OBJECT_IDENTIFIER_STRING) {
                error_status = pkc_s_conversion_error;
                return;
            };
            attributeType.length = obj[i].value.string.length;
            attributeType.elements = 
                           (unsigned char *)pkc_malloc(attributeType.length);
            memcpy(attributeType.elements,
                   obj[i].value.string.elements,
                   attributeType.length);
            break;
        case DS_ATTRIBUTE_VALUES:
            
            *attributeValue_ptr = new xds_value_list_t(&obj[i]);
            attributeValue_ptr = &((*attributeValue_ptr)->next_value);
            break;
        default:
	    ;
#ifdef XDS_DEBUG
            fprintf(stderr,
                    "Unrecognized xds_attribute_t component, type = %d\n",
                    obj[i].type);
            fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                          "Unrecognized xds_attribute_t component, type = %d",
                          obj[i].type));
        };
        i++;
    };
#ifdef XDS_DEBUG
    fprintf(stderr,
            "Done scan of xds_attribute_t object (%p)\n",
            obj);
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Done scan of xds_attribute_t object (%p)",
                  obj));
}

xds_entry_t::~xds_entry_t() {
}

xds_entry_t::xds_entry_t(OM_object obj) : xds_attribute_list_t(obj) {
    int i;

    wasFromEntry = 0;
    if (obj == NULL) return;
    i = 0;
    while (obj[i].type != OM_NO_MORE_TYPES) {
        switch (obj[i].type) {
        case OM_CLASS:
            break;
        case DS_FROM_ENTRY:
            if ((obj[i].syntax & OM_S_SYNTAX) != OM_S_BOOLEAN) {
                error_status = pkc_s_conversion_error;
                return;
            };
            wasFromEntry = (obj[i].value.boolean != 0);
            break;
        case DS_OBJECT_NAME:
            if ((obj[i].syntax & OM_S_SYNTAX) != OM_S_OBJECT) {
                error_status = pkc_s_conversion_error;
                return;
            };
            if (error_status =
                    object_name.set_value(obj[i].value.object.object)) return;
            break;
        default:
	    ;
#ifdef XDS_DEBUG
            fprintf(stderr,
                    "Unrecognized xds_entry_t component, type = %d\n",
                    obj[i].type);
            fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug5,
                          "Unrecognized xds_entry_t component, type = %d",
                          obj[i].type));
        };
        i++;
    };
#ifdef XDS_DEBUG
    fprintf(stderr,
            "Done scan of xds_entry_t object (%p)\n",
            obj);
    fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Done scan of xds_entry_t object (%p)",
                  obj));
}

xds_attribute_list_t::~xds_attribute_list_t() {
    if (first_attribute) delete first_attribute;
    if (xom_object) delete_xom_object();
}

xds_attribute_t::~xds_attribute_t() {
    if (first_attributeValue) delete first_attributeValue;
    if (next_attribute) delete next_attribute;
    if (attributeType.elements) pkc_free(attributeType.elements);
}


xds_value_t::~xds_value_t() {
    if (theString.elements) pkc_free(theString.elements);
}

xds_value_t::xds_value_t(void) : om_translation_t() {
    theValueType = VC_NULL;
    theValueSyntax = OM_S_NULL;
    theString.elements = NULL;
    theString.length = 0;
    theInteger = 0;
    theBoolean = 0;
}

xds_value_t::xds_value_t(const xds_value_t & o) : om_translation_t() {
    theValueType = o.theValueType;
    theValueSyntax = o.theValueSyntax;
    theInteger = o.theInteger;
    if ((theString.length = o.theString.length) != 0) {
        if (theString.elements = pkc_malloc(theString.length)) {
            memcpy(theString.elements,
                   o.theString.elements,
                   theString.length);
        };
    } else {
        theString.elements = NULL;
    };
    theBoolean = o.theBoolean;
}

xds_value_t::xds_value_t(OM_object o) : om_translation_t(o) {

    o = get_obj();

#ifdef XDS_DEBUG
fprintf(stderr, "Creating an xds_value_t object, source:\n");
#ifdef DEC_XDS_DEBUG
parse_object(o);
#endif
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "Creating an xds_value_t object"));

    theString.length = 0;
    theString.elements = NULL;
    theValueType = syntaxToClass(o->syntax);
    theValueSyntax = o->syntax;
    switch (theValueType) {
    case VC_INTEGER:
        theInteger = o->value.integer; break;
    case VC_BOOLEAN:
        theBoolean = o->value.boolean; break;
    case VC_CHARSTRING:
    case VC_OCTETSTRING:
#ifdef XDS_DEBUG
fprintf(stderr, "Characterstring/octetstring value\n");
fprintf(stderr, "Length: %d\n", o->value.string.length);
fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Characterstring/octetstring value"));
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                      "Length: %d", o->value.string.length));
        if (o->value.string.length == OM_LENGTH_UNSPECIFIED) {
            theString.length = strlen((char *)(o->value.string.elements));
#ifdef XDS_DEBUG
fprintf(stderr, "RecalcLength: %d\n", theString.length);
fflush(stderr);
#endif
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                          "RecalcLength: %d", theString.length));
        } else {
            theString.length = o->value.string.length;
        };
        if (theString.elements = pkc_malloc(theString.length + 1)) {
	    memcpy(theString.elements,
		   o->value.string.elements,
		   theString.length);
	    ((char *)theString.elements)[theString.length] = 0;
	};
        break;
    case VC_BITSTRING:
        theString.length = o->value.string.length;
        theString.elements = pkc_malloc(o->value.string.length);
        memcpy(theString.elements,
               o->value.string.elements,
               theString.length);
        break;
    default:
#ifdef XDS_DEBUG
fprintf(stderr,
"Unsupported value-class (syntax = %d) in xds_value_t::xds_value_t()\n",
o->syntax);
fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug1,
       "Unsupported value-class (syntax = %d) in xds_value_t::xds_value_t()",
                      o->syntax));
    };
}


value_class_t syntaxToClass(OM_syntax s) {
    switch (s & OM_S_SYNTAX) {
    case OM_S_BOOLEAN : return VC_BOOLEAN;
    case OM_S_INTEGER : return VC_INTEGER;
    case OM_S_NULL : return VC_NULL;
//    case OM_S_BIT_STRING : return VC_BITSTRING;
    case OM_S_OCTET_STRING : 
    case OM_S_ENCODING_STRING:
    case OM_S_OBJECT_IDENTIFIER_STRING:
                return VC_OCTETSTRING;
    case OM_S_GENERAL_STRING :
    case OM_S_GENERALISED_TIME_STRING :
    case OM_S_GRAPHIC_STRING :
    case OM_S_IA5_STRING :
    case OM_S_NUMERIC_STRING :
    case OM_S_PRINTABLE_STRING :
    case OM_S_TELETEX_STRING :
    case OM_S_UTC_TIME_STRING :
    case OM_S_VIDEOTEX_STRING :
    case OM_S_VISIBLE_STRING :
        return VC_CHARSTRING;
    default: return VC_UNSUPPORTED;
    };
}

xds_selection_t::~xds_selection_t() {
    int i = 0;
    while (xom_object[i].type != OM_NO_MORE_TYPES) {
        if (xom_object[i].type == DS_ATTRIBUTES_SELECTED) {
            pkc_free(xom_object[i].value.string.elements);
            xom_object[i].value.string.elements = NULL;
            xom_object[i].value.string.length = 0;
        };
        i++;
    };           
    pkc_free(xom_object);
}

void xds_selection_t::select_all_attributes(void) {
    int i = 0;
    while ((xom_object[i].type != OM_NO_MORE_TYPES) &&
           (xom_object[i].type != DS_ALL_ATTRIBUTES)) i++;
    if (xom_object[i].type == OM_NO_MORE_TYPES) return; // Error!
    xom_object[i].value.boolean = OM_TRUE;
}

void xds_selection_t::select_specified_attributes(void) {
    int i = 0;
    while ((xom_object[i].type != OM_NO_MORE_TYPES) &&
           (xom_object[i].type != DS_ALL_ATTRIBUTES)) i++;
    if (xom_object[i].type == OM_NO_MORE_TYPES) return; // Error!
    xom_object[i].value.boolean = OM_FALSE;
}

void xds_selection_t::return_types_only(void) {
    int i = 0;
    while ((xom_object[i].type != OM_NO_MORE_TYPES) &&
           (xom_object[i].type != DS_INFO_TYPE)) i++;
    if (xom_object[i].type == OM_NO_MORE_TYPES) return; // Error!
    xom_object[i].value.boolean = DS_TYPES_ONLY;
}

void xds_selection_t::return_types_and_values(void) {
    int i = 0;
    while ((xom_object[i].type != OM_NO_MORE_TYPES) &&
           (xom_object[i].type != DS_INFO_TYPE)) i++;
    if (xom_object[i].type == OM_NO_MORE_TYPES) return; // Error!
    xom_object[i].value.boolean = DS_TYPES_AND_VALUES;
}

void xds_selection_t::clear_all_attributes(void) {
    int i = 0;

    while (xom_object[i].type != OM_NO_MORE_TYPES) {
        if (xom_object[i].type = DS_ATTRIBUTES_SELECTED) {
            pkc_free(xom_object[i].value.string.elements);
            xom_object[i].value.string.elements = NULL;
            xom_object[i].value.string.length = 0;
        };
        i++;
    };           

    xom_object = (OM_descriptor *)pkc_realloc(xom_object,
                                              4 * sizeof(OM_descriptor));
    xom_object[3].type = OM_NO_MORE_TYPES;
    xom_object[3].syntax = OM_S_NO_MORE_SYNTAXES;
    xom_object[3].value.string.length = 0;
    xom_object[3].value.string.elements = NULL;
}

void xds_selection_t::select_attribute(OM_object_identifier attr) {
    int i=0;
    while ((xom_object[i].type != OM_NO_MORE_TYPES) &&
           ((xom_object[i].type != DS_ATTRIBUTES_SELECTED) ||
            (xom_object[i].value.string.length != attr.length) ||
            (memcmp(xom_object[i].value.string.elements,
                    attr.elements,
                    attr.length) != 0))) i++;
    if (xom_object[i].type == OM_NO_MORE_TYPES) {
// We have to insert the new attribute
        xom_object =
            (OM_descriptor *)pkc_realloc(xom_object,
                                         (i+2) * sizeof(OM_descriptor));
        xom_object[i].type = DS_ATTRIBUTES_SELECTED;
        xom_object[i].syntax = OM_S_OBJECT_IDENTIFIER_STRING;
        xom_object[i].value.string.length = attr.length;
        xom_object[i].value.string.elements = pkc_malloc(attr.length);
        memcpy(xom_object[i].value.string.elements,
               attr.elements,
               attr.length);

        xom_object[++i].type = OM_NO_MORE_TYPES;
        xom_object[i].syntax = OM_S_NO_MORE_SYNTAXES;
        xom_object[i].value.string.length = 0;
        xom_object[i].value.string.elements = NULL;
    };
}

xds_selection_t::xds_selection_t(void) {
    xom_object = (OM_descriptor *)pkc_malloc(4 * sizeof(OM_descriptor));
    xom_object[0].type = OM_CLASS;
    xom_object[0].syntax = OM_S_OBJECT_IDENTIFIER_STRING;
    xom_object[0].value.string.length =
        OMP_LENGTH(DS_C_ENTRY_INFO_SELECTION);
    xom_object[0].value.string.elements =
        OMP_D_DS_C_ENTRY_INFO_SELECTION;

    xom_object[1].type = DS_ALL_ATTRIBUTES;
    xom_object[1].syntax = OM_S_BOOLEAN;
    xom_object[1].value.boolean = OM_TRUE;

    xom_object[2].type = DS_INFO_TYPE;
    xom_object[2].syntax = OM_S_ENUMERATION;
    xom_object[2].value.enumeration = DS_TYPES_AND_VALUES;
    
    xom_object[3].type = 0;
    xom_object[3].syntax = 0;
    xom_object[3].value.string.length = 0;
    xom_object[3].value.string.elements = NULL;

}


unsigned32 xds_value_list_t::add_value(OM_syntax s, long i) {
    xds_value_list_t * new_value_list;
    xds_value_t * new_value;
    unsigned32 error;
    new_value = new xds_value_t;

#ifdef XDS_DEBUG
fprintf(stderr,
	"xds_value_list_t::add_value(int %ld)\n", i);
fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "xds_value_list_t::add_value(int %ld)", i));

    if ((error = new_value->set_value(s, i)) != 0) {
#ifdef XDS_DEBUG
fprintf(stderr,
	"xds_value_list_t::add_value: xds_value_t::set_value returned %d\n",
	error);
fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
            "xds_value_list_t::add_value: xds_value_t::set_value returned 0x%x",
                      error));
	delete new_value;
	return error;
    };
    if (value) {
#ifdef XDS_DEBUG
fprintf(stderr,
      "xds_value_list_t::add_value: shifting current value to next position\n");
fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
       "xds_value_list_t::add_value: shifting current value to next position"));
	new_value_list = new xds_value_list_t;
	new_value_list->next_value = next_value;
	new_value_list->value = value;
	new_value_list->copied_object = copied_object;
	new_value_list->original_object = original_object;
	copied_object = NULL;
	original_object = NULL;
	next_value = new_value_list;
    } else {
#ifdef XDS_DEBUG
fprintf(stderr,
	"xds_value_list_t::add_value: List now contains one value\n");
fflush(stderr);
#endif
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                "xds_value_list_t::add_value: List now contains one value"));
    };
    value = new_value;
    return 0;
}

unsigned32 xds_value_list_t::add_value(OM_syntax s, const char * c) {
    xds_value_list_t * new_value_list;
    xds_value_t * new_value;
    unsigned32 error;
    new_value = new xds_value_t;
    if ((error = new_value->set_value(s, c)) != 0) {
	delete new_value;
	return error;
    };
    if (value) {
	new_value_list = new xds_value_list_t;
	new_value_list->next_value = next_value;
	new_value_list->value = value;
	new_value_list->copied_object = copied_object;
	new_value_list->original_object = original_object;
	copied_object = NULL;
	original_object = NULL;
	next_value = new_value_list;
    };
    value = new_value;
    return 0;
}

unsigned32 xds_value_list_t::add_value(OM_syntax s,
                                       const unsigned char * c,
                                       size_t l) {
    xds_value_list_t * new_value_list;
    xds_value_t * new_value;
    unsigned32 error;
    new_value = new xds_value_t;
    if ((error = new_value->set_value(s, c, l)) != 0) {
	delete new_value;
	return error;
    };
    if (value) {
	new_value_list = new xds_value_list_t;
	new_value_list->next_value = next_value;
	new_value_list->value = value;
	new_value_list->copied_object = copied_object;
	new_value_list->original_object = original_object;
	copied_object = NULL;
	original_object = NULL;
	next_value = new_value_list;
    };
    value = new_value;
    return 0;
}


xds_changes_t::xds_changes_t(void) {
    modification_count = 0;
    xom_object = NULL;
}

unsigned32 xds_changes_t::build_xom_object(OM_object & o) const {
    o = xom_object;
    return 0;
}

unsigned32 xds_changes_t::delete_xom_object(void) const {
    return 0;
}

xds_changes_t::~xds_changes_t() {
    int i;
    int j;
    OM_object change;
    
    if (xom_object == NULL) return;
// Delete the xom_object.  There are several 'deleteable' parts:
// 1) Any component with type DS_CHANGES contains a DS_CHANGE object
// 2) Any DS_CHANGE component with type DS_ATTRIBUTE_TYPE has a
//    string value that must be freed.
// 3) Any DS_CHANGE component with type DS_ATTRIBUTE_VALUES and a syntax
//    class of VC_CHARSTRING or VC_OCTETSTRING has a string value that
//    must be freed.
// 4) Any DS_CHANGE object must be freed
// 5) The xom_object itself must be freed.

    for (i=0; xom_object[i].type != OM_NO_MORE_TYPES; i++) {
	if (xom_object[i].type == DS_CHANGES) {
	    if ((change = xom_object[i].value.object.object) != NULL) {
		for (j=0; change[j].type != OM_NO_MORE_TYPES; j++) {
		    if (change[j].type == DS_ATTRIBUTE_TYPE) {
			pkc_free(change[j].value.string.elements);
		    } else if ((change[j].type = DS_ATTRIBUTE_VALUES) &&
                        ((syntaxToClass(change[j].syntax) == VC_CHARSTRING) ||
                         (syntaxToClass(change[j].syntax) == VC_OCTETSTRING)))
                    {
			pkc_free(change[j].value.string.elements);
		    };
		};
		pkc_free(change);
	    };
	};
    };
    pkc_free(xom_object);
    xom_object = NULL;
}

unsigned32 xds_changes_t::remove_attribute(
        const OM_object_identifier & attr_type) {
    return add_attr_value_list(attr_type,
                               NULL,
			       DS_REMOVE_ATTRIBUTE);

}

unsigned32 xds_changes_t::remove_values(
        const OM_object_identifier & attr_type,
        const xds_value_list_t & values) {
    return add_attr_value_list(attr_type,
                               &values,
			       DS_REMOVE_VALUES);

}

unsigned32 xds_changes_t::add_values(
        const OM_object_identifier & attr_type,
        const xds_value_list_t & values) {
    return add_attr_value_list(attr_type,
                               &values,
			       DS_ADD_VALUES);

}

unsigned32 xds_changes_t::add_attribute(
        const OM_object_identifier & attr_type,
        const xds_value_list_t & values) {

#ifdef XDS_DEBUG
fprintf(stderr,
"xds_changes_t::add_attribute(), calling add_attr_value_list\n");
fflush(stderr);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                "xds_changes_t::add_attribute(), calling add_attr_value_list"));

    return add_attr_value_list(attr_type,
                               &values,
			       DS_ADD_ATTRIBUTE);
}
			       

unsigned32 xds_changes_t::add_attr_value_list(
                             const OM_object_identifier & attr_type,
                             const xds_value_list_t * values,
                             OM_enumeration mod_type) {

//    OM_object entry;
    const xds_value_list_t * value_list_ptr;
    const xds_value_t * value_ptr;
    int value_count;


#ifdef XDS_DEBUG
fprintf(stderr,
        "add_attr_value_list(), current mod-count = %d\n",
        modification_count);
#endif
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_xds, svc_c_debug9,
                  "add_attr_value_list(), current mod-count = %d",
                  modification_count));

    modification_count++;

    xom_object =
        (OM_descriptor *)pkc_realloc(
            xom_object,
            (modification_count+2) * sizeof(OM_descriptor));
    xom_object[0].type = OM_CLASS;
    xom_object[0].syntax = OM_S_OBJECT_IDENTIFIER_STRING;
    xom_object[0].value.string.length =
        OMP_LENGTH(DS_C_ENTRY_MOD_LIST);
    xom_object[0].value.string.elements =
        OMP_D_DS_C_ENTRY_MOD_LIST;


    xom_object[modification_count+1].type = OM_NO_MORE_TYPES;
    xom_object[modification_count+1].syntax = OM_S_NO_MORE_SYNTAXES;
    xom_object[modification_count+1].value.string.length = 0;
    xom_object[modification_count+1].value.string.elements = NULL;

    xom_object[modification_count].type = DS_CHANGES;
    xom_object[modification_count].syntax = OM_S_OBJECT;

    xom_object[modification_count].value.object.object =
        (OM_descriptor *)pkc_malloc(4 * sizeof(OM_descriptor));

    memset(xom_object[modification_count].value.object.object,
           0,
           4 * sizeof(OM_descriptor));

    xom_object[modification_count].value.object.object[0].type =
        OM_CLASS;
    xom_object[modification_count].value.object.object[0].syntax =
        OM_S_OBJECT_IDENTIFIER_STRING;
    xom_object[modification_count].value.object.object[0].value.string.length =
        OMP_LENGTH(DS_C_ENTRY_MOD);
    xom_object[modification_count].value.object.object[0].value.string.elements =
        OMP_D_DS_C_ENTRY_MOD;

    xom_object[modification_count].value.object.object[1].type =
        DS_MOD_TYPE;
    xom_object[modification_count].value.object.object[1].syntax =
        OM_S_ENUMERATION;
    xom_object[modification_count].value.object.object[1].value.enumeration =
        mod_type;

    xom_object[modification_count].value.object.object[2].type =
        DS_ATTRIBUTE_TYPE;
    xom_object[modification_count].value.object.object[2].syntax =
        OM_S_OBJECT_IDENTIFIER_STRING;
    xom_object[modification_count].value.object.object[2].value.string.length =
        attr_type.length;
    if (xom_object[modification_count].value.object.object[2].value.string.elements
        = pkc_malloc(attr_type.length)) {
        memcpy(xom_object[modification_count].value.object.object[2].value.string.elements,
               attr_type.elements,
               attr_type.length);
    } else {
	return pkc_s_nomem;
    };

    if ((value_list_ptr = values) != NULL) {
	value_count = 0;
	while ((value_list_ptr != NULL) &&
	       ((value_ptr = value_list_ptr->value) != NULL)) {

	    value_count++;
	    if ((xom_object[modification_count].value.object.object =
                 (OM_descriptor *)pkc_realloc(
                         xom_object[modification_count].value.object.object,
                         (4 + value_count) * sizeof(OM_descriptor))
		) == NULL) return pkc_s_nomem;

	    xom_object[modification_count].value.object.object[3 + value_count]
		.type = OM_NO_MORE_TYPES;
	    xom_object[modification_count].value.object.object[3 + value_count]
		.syntax = OM_S_NO_MORE_SYNTAXES;
	    xom_object[modification_count].value.object.object[3 + value_count]
		.value.string.length = 0;
	    xom_object[modification_count].value.object.object[3 + value_count]
		.value.string.elements = NULL;

	    xom_object[modification_count].value.object.object[2 + value_count]
		.type = DS_ATTRIBUTE_VALUES;
	    xom_object[modification_count].value.object.object[2 + value_count]
		.syntax = value_ptr->theValueSyntax;
	    switch (value_ptr->theValueType) {
	    case VC_BOOLEAN:
		xom_object[modification_count].value.object.object[2 + value_count]
		    .value.boolean = (value_ptr->theBoolean?
				      OM_TRUE:OM_FALSE);
		break;
	    case VC_INTEGER:
		xom_object[modification_count].value.object.object[2 + value_count]
		    .value.integer = value_ptr->theInteger;
		break;
	    case VC_CHARSTRING:
	    case VC_OCTETSTRING:
		xom_object[modification_count].value.object.object[2 + value_count]
		    .value.string.length = value_ptr->theString.length;
		if ((xom_object[modification_count].value.object.object
			 [2 + value_count].value.string.elements =
			       pkc_malloc(value_ptr->theString.length)) == NULL)
		    return pkc_s_nomem;
		memcpy(xom_object[modification_count].value.object.object
			   [2 + value_count].value.string.elements,
		       value_ptr->theString.elements,
		       value_ptr->theString.length);
		break;
	    default:
		return pkc_s_conversion_error;
	    };


	    value_list_ptr = value_list_ptr->next_value;
	};
    };

    xom_object[modification_count].value.object.object[3 + value_count]
        .type = OM_NO_MORE_TYPES;
    xom_object[modification_count].value.object.object[3 + value_count]
        .syntax = OM_S_NO_MORE_SYNTAXES;
    xom_object[modification_count].value.object.object[3 + value_count]
        .value.string.length = 0;
    xom_object[modification_count].value.object.object[3 + value_count]
        .value.string.elements = NULL;
    
    return 0;


}

unsigned32 xds_value_t::set_value(OM_syntax s, long i) {
    switch (theValueType = syntaxToClass(s)) {
    case VC_INTEGER:
        theInteger = i;
        break;
    case VC_BOOLEAN:
        theBoolean = (i != 0);
        break;
    default:
        return pkc_s_conversion_error;
    };
    theValueSyntax = s;
    return 0;
}

unsigned32 xds_value_t::set_value(OM_syntax s, const char * c) {
    switch (theValueType = syntaxToClass(s)) {
    case VC_CHARSTRING:
        theString.length = strlen(c);
        if ((theString.elements = pkc_malloc(theString.length)) == NULL)
            return pkc_s_nomem;
        memcpy(theString.elements,
               c,
               theString.length);
        break;
    default:
        return pkc_s_conversion_error;
    };
    theValueSyntax = s;
    return 0;
}


unsigned32 xds_value_t::set_value(OM_syntax s,
                                  const unsigned char * c, size_t l) {
    switch (theValueType = syntaxToClass(s)) {
    case VC_CHARSTRING:
    case VC_OCTETSTRING:
        theString.length = l;
        if ((theString.elements = pkc_malloc(theString.length)) == NULL)
            return pkc_s_nomem;
        memcpy(theString.elements,
               c,
               theString.length);
        break;
    default:
        return pkc_s_conversion_error;
    };
    theValueSyntax = s;
    return 0;
}


unsigned32 xds_value_t::get_value_syntax(OM_syntax &s) const {
    s = theValueSyntax;
    return 0;
}

unsigned32 xds_value_t::get_value_class(value_class_t & s) const {
    s = theValueType;
    return 0;
}

unsigned32 xds_value_t::get_value(unsigned char * &s, size_t & l) const {
    if ((theValueType == VC_OCTETSTRING) ||
        (theValueType == VC_CHARSTRING)) {
        if (s = (unsigned char *)pkc_malloc(theString.length+1)) {
            memcpy(s, theString.elements, theString.length);
            l = theString.length;
            s[l] = 0;
            return 0;
        } else return pkc_s_nomem;
    } else return pkc_s_conversion_error;
}

unsigned32 xds_value_t::get_value(char * &s) const {
    if (theValueType == VC_CHARSTRING) {
        if (s = (char *)pkc_malloc(theString.length+1)) {
            memcpy(s, theString.elements, theString.length);
            s[theString.length] = 0;
            return 0;
        } else return pkc_s_nomem;
    } else return pkc_s_conversion_error;
}

unsigned32 xds_value_t::get_value(long & s) const {
    switch (theValueType) {
    case VC_INTEGER:
        s = theInteger;
        break;
    case VC_BOOLEAN:
        s = (theBoolean != 0);
        break;
    default:
        return pkc_s_conversion_error;
    };
    return 0;
}

xds_value_list_t::xds_value_list_t(void) : om_translation_t() {
    current_value = this;
    next_value = NULL;
    value = NULL;
}

xds_value_list_t::xds_value_list_t(OM_object o) : om_translation_t(o) {
    current_value = this;
    next_value = NULL;
    value = new xds_value_t(o);
}

xds_value_list_t::~xds_value_list_t() {
    delete value;
    if (next_value) delete next_value;
}

unsigned32 xds_value_list_t::get_first_value(const xds_value_t * & val) const {
    xds_value_list_t * var_this = (xds_value_list_t *)this;
    var_this->current_value = var_this;
    return get_next_value(val);
}

unsigned32 xds_value_list_t::get_next_value(const xds_value_t * & val) const {
    xds_value_list_t * var_this = (xds_value_list_t *)this;
    if (current_value == NULL) return pkc_s_no_value;
    if (current_value->value == NULL) return pkc_s_no_value;
    val = current_value->value;
    var_this->current_value = current_value->next_value;
    return 0;
}

unsigned32 xds_attribute_t::get_next_attribute(const xds_attribute_t * & attr)
const {
    attr = next_attribute;
    if (next_attribute == NULL) return pkc_s_no_value;
    return 0;
}

unsigned32 xds_attribute_t::get_type(const OM_object_identifier * & attr_type) 
const {
    attr_type = &attributeType;
    return 0;
}

unsigned32 xds_attribute_t::get_first_value(const xds_value_t * &s ) const {
    if (first_attributeValue == NULL) return pkc_s_no_value;
    return first_attributeValue->get_first_value(s);
}

unsigned32 xds_attribute_t::get_next_value(const xds_value_t * &s ) const {
    if (first_attributeValue == NULL) return pkc_s_no_value;
    return first_attributeValue->get_next_value(s);
}

unsigned32 xds_attribute_list_t::get_first_attribute(
                                        const xds_attribute_t * & att) const {
    if (first_attribute == NULL) return pkc_s_no_value;
    att = first_attribute;
    return 0;
}

unsigned32 xds_entry_t::get_object_name(const x500name * &name) const {
    if (object_name.is_set()) name = &object_name;
    else return pkc_s_no_value;
    return 0;
}

unsigned32 xds_read_result_t::get_object_name(const x500name *& name) const {
    if (entry == NULL) return pkc_s_no_value;
    return entry->get_object_name(name);
}

unsigned32 xds_read_result_t::get_first_attribute(
                                     const xds_attribute_t * & att) const {
    unsigned32 error;
    xds_read_result_t * var_this = (xds_read_result_t *)this;
    var_this->cur_attribute = NULL;
    if (entry == NULL) return pkc_s_no_value;
    error = entry->get_first_attribute(att);
    var_this->cur_attribute = att;
    return error;
}


unsigned32 xds_read_result_t::get_next_attribute(const xds_attribute_t * & att)
const {
    unsigned32 error;
    xds_read_result_t * var_this = (xds_read_result_t *)this;

    if (cur_attribute == NULL)
        return get_first_attribute(att);
    error = cur_attribute->get_next_attribute(att);
    var_this->cur_attribute = att;
    return error;
}

unsigned32 xds_list_info_item_t::get_rdn(
                             const RelativeDistinguishedName * & name) const {
    if (rdn.is_set()) name = &rdn;
    else return pkc_s_no_value;
    return 0;
}



