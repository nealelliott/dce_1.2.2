/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsutil.c,v $
 * Revision 1.1.435.2  1996/02/18  00:05:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:16  marty]
 *
 * Revision 1.1.435.1  1995/12/08  00:21:44  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:34 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/2  1995/10/12  17:28 UTC  sommerfeld
 * 	Set fsMaybeMore bit if not bypassing the cache.
 * 	[1995/09/15  20:30 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/02  01:16 UTC  lmm
 * 	Merge allocation failure detection functionality into Mothra
 * 
 * 	HP revision /main/lmm_rpc_alloc_fail_detect/1  1995/04/02  00:20 UTC  lmm
 * 	add memory allocation failure detection
 * 	[1995/12/08  00:00:15  root]
 * 
 * Revision 1.1.433.4  1994/07/29  16:19:27  tom
 * 	Remove DNSPI_V2 ifdef.
 * 	[1994/07/28  22:34:32  tom]
 * 
 * Revision 1.1.433.3  1994/07/15  21:00:42  tom
 * 	Bug 10599 - Map DNS_SOCKET ("Error with socket") error from cdspi
 * 	to (a more helpful?) rpc_s_name_service_unavailable.
 * 	[1994/07/15  21:00:28  tom]
 * 
 * Revision 1.1.433.2  1994/02/08  21:41:00  mori_m
 * 	CR 9701:  Initial submission for RPC runtime I18N support
 * 	[1994/02/08  21:40:47  mori_m]
 * 
 * Revision 1.1.433.1  1994/01/21  22:38:50  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:29  cbrooks]
 * 
 * Revision 1.1.2.8  1993/03/05  19:21:10  weisman
 * 	       3-mar-93   dm      Handle new dnsReadAttrValue() return status code
 * 	                          DNS_UNKNOWNATTRIBUTE.
 * 	[1993/03/05  19:09:53  weisman]
 * 
 * Revision 1.1.2.7  1993/03/01  19:31:43  sommerfeld
 * 	[7351] Avoid excessive numbers of checks of RPC_ClassVersion
 * 	[1993/02/25  01:03:55  sommerfeld]
 * 
 * Revision 1.1.2.6  1993/02/24  16:57:38  mhickey
 * 	Fixed incorrect pointer dereferencing in ns__count_uuid_in_object_vec.
 * 	Was using a uuid * to index through an array of uuid **, so changed the
 * 	pointer variable to a uuid ** and changed the variable name and
 * 	usage accordingly.  Also folded a couple of if statements into
 * 	one to eliminate some duplicate lines of code.
 * 
 * 	Defect 3678.
 * 	[1993/02/23  17:45:03  mhickey]
 * 
 * Revision 1.1.2.5  1993/02/05  16:12:00  raizen
 * 	 26-jan-93 dr        Back out changes from Revision 1.1.2.2
 * 	                     Add code to init dns_flags from login context
 * 	[1993/02/04  23:21:20  raizen]
 * 
 * Revision 1.1.2.4  1993/01/03  23:54:17  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:11:11  bbelch]
 * 
 * Revision 1.1.2.3  1992/12/23  21:14:36  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:42:56  zeliff]
 * 
 * Revision 1.1.2.2  1992/12/21  19:21:11  wei_hu
 * 	      09-dec-92 raizen    Modify rpc_ns_set_authn to call new API routine
 * 	                          rpc_ns_mgmt_set_authn for compatibility [OT4854]
 * 	[1992/12/18  20:28:13  wei_hu]
 * 
 * Revision 1.1  1992/01/19  03:12:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      NSUTIL.C
**
**  FACILITY:
**
**      Remote procedure call (RPC) name service independent interface (NS)
**  
**  ABSTRACT:
**
**  Name Service Utilities module, containing utilities common to the NS
**  component routines.
**  
**
*/

#include <commonp.h>    /* Common internals for RPC runtime system     */
#include <com.h>        /* Externals for Common Services component     */
#include <ns.h>         /* Externals for Name Services component       */
#include <nsp.h>        /* Internals for Name Services component       */
#include <nsutil.h>     /* Externals for NS Utility sub-component      */
#include <nsentry.h>    /* Externals for NS Entry sub-component        */

#ifdef AUTH_KRB
#include <dce/sec_login.h> /* For call to sec_login_get_current_context */
#endif




/*
**++
**  ROUTINE NAME:       rpc__ns_check_name_syntax 
**
**  SCOPE:              PRIVATE - declared in nsutil.h
**
**  DESCRIPTION:
**
**  Checks the specified name_syntax against valid syntaxes
**
**  INPUTS:
**
**      name_syntax     The name syntax specified by the application.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:
**
**      status         Returns one of
**                          rpc_s_ok,
**                          rpc_s_unsupported_name_syntax,
**                          rpc_s_invalid_name_syntax
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      true      if successful.
**      false     if not successful.
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE boolean rpc__ns_check_name_syntax 
#ifdef _DCE_PROTO_
(
    unsigned32              name_syntax,
    unsigned32              *status
)
#else
(name_syntax, status)
unsigned32              name_syntax;
unsigned32              *status;
#endif
{
    CODING_ERROR (status);

    if (!((name_syntax == rpc_c_ns_syntax_dec_dns ) ||
        (name_syntax == rpc_c_ns_syntax_dce)  ||
        (name_syntax == rpc_c_ns_syntax_x500) ||
        (name_syntax == rpc_c_ns_syntax_internet_dns) ||
        (name_syntax == rpc_c_ns_syntax_uuid) ||
        (name_syntax == rpc_c_ns_syntax_unknown)))
    {
        *status = rpc_s_invalid_name_syntax;
        return false;
    }

    if ((name_syntax != rpc_c_ns_syntax_dec_dns) &&
	(name_syntax != rpc_c_ns_syntax_dce))
    {
        *status = rpc_s_unsupported_name_syntax;
        return false;
    }

    *status = rpc_s_ok;
    return true;
}



/*
**++
**  ROUTINE NAME:      rpc_ns_set_authn
**
**  SCOPE:             PUBLIC - declared in RPCPVT.IDL
**
**  DESCRIPTION:
**
**  Sets the authentication global variable to the desired setting.
**  This value is written to the appropriate field of the dns flags
**  in rpc__ns_init_dns_flags.
**
**  Although this routine's signature is in RPCPVT.IDL, this is not part
**  of the public interface.  It is available only to private parties
**  and other components of DCE.
**
**  INPUTS:             
**      authn           A boolean value specifying whether authentication 
**                      should be used for all subsequent name service calls.
**                      True means turn on authentication, false means
**                      no authentication.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            
**      previous_authn  A boolean value specifying the previous state of
**                      authn (in a per-thread context) True means authentication
**                      was on, false means authentication was off.
**
**      status          An integer value specifying the results of the
**                      ns_set_authn operation.
**                      One of:
**                          rpc_s_ok
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void rpc_ns_set_authn 
#ifdef _DCE_PROTO_
(
    boolean32   authn,
    boolean32   *previous_authn,
    unsigned32  *status
)
#else
(authn, previous_authn, status)
boolean32   authn;
boolean32   *previous_authn;
unsigned32  *status;
#endif
{
    rpc_thread_context_p_t  thread_context;

    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT ();

    /*
     *  Assign the value of the authentication state that is currently
     *  stored in the per-thread context to *previous_authn so that it
     *  can later be restored, and set the ns_authn_state to what the
     *  user passes us.
     */
    RPC_GET_THREAD_CONTEXT(thread_context, status);

    if (*status == rpc_s_ok)
    {
        *previous_authn = thread_context->ns_authn_state;
        thread_context->ns_authn_state = authn;
    }

    return;
}


/*
**++
**  ROUTINE NAME:       rpc__inqcntx_create
**
**  SCOPE:              PRIVATE - declared in nsutil.h.
**
**  DESCRIPTION:
**
**  Creates and initializes an inquiry context.
**
**  INPUTS:
**
**      nsentry         Name service representation of an entry name.
**
**      usage           An enumerated value specifying how the inquiry
**                      context is to be used.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:
**
**      inquiry_context A pointer to the newly created inquiry context.
**
**      status          The status of the inquiry context create operation.
**                      One of:
**                          rpc_s_ok,
**                          rpc_s_no_memory.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE rpc_ns_inq_rep_t *rpc__inqcntx_create 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t         nsentry,
    rpc_inqcntx_usage_t     usage,
    unsigned32              *status
)
#else
(nsentry, usage, status)
rpc_nsentry_p_t         nsentry;
rpc_inqcntx_usage_t     usage;
unsigned32              *status;
#endif
{
    unsigned32              attribute_class;
    rpc_ns_inq_rep_p_t      inquiry_context;


    CODING_ERROR (status);

    RPC_MEM_ALLOC (
        inquiry_context,
        rpc_ns_inq_rep_p_t,
        sizeof (rpc_ns_inq_rep_t), 
        RPC_C_MEM_INQ_REP,
        RPC_C_MEM_WAITOK);
    if (inquiry_context == NULL){
	*status = rpc_s_no_memory;
	return (NULL);
    }

    /*
     * Initialize common fields
     */
    inquiry_context->usage = usage;

    inquiry_context->nsentry = nsentry;

    /*
     * Use the global expiration age for determining
     * whether the name service cache needs to be refreshed.
     */
     
    inquiry_context->common.exp_age = rpc_g_ns_exp_age;
    
    switch (usage)
    {
        case rpc_e_group_member:
        {
            attribute_class = RPC_C_ATTR_GROUP;
            memset ((char *) &(inquiry_context->inq_specific.inqcntx_group), 
                0, sizeof (rpc_inqcntx_group_t));
            break;
        }
        
        case rpc_e_profile_element:
        {
            attribute_class = RPC_C_ATTR_PROFILE;
            memset ((char *) &(inquiry_context->inq_specific.inqcntx_profile), 
                0, sizeof (rpc_inqcntx_profile_t));
            break;
        }
        
        case rpc_e_tower_member:
        {
            attribute_class = RPC_C_ATTR_DNA_TOWERS;
            memset ((char *) &(inquiry_context->inq_specific.inqcntx_tower), 
                0, sizeof (rpc_inqcntx_tower_t));
            break;
        }

        case rpc_e_uuid_member:
        {
            attribute_class = RPC_C_ATTR_OBJECT_UUIDS;
            memset ((char *) &(inquiry_context->inq_specific.inqcntx_nsuuid), 
                    0, sizeof (rpc_inqcntx_nsuuid_t));
            break;
        }

	case rpc_e_codesets_member:
	{
            attribute_class = RPC_C_ATTR_CODESETS;
            memset ((char *) &(inquiry_context->inq_specific.inqcntx_codesets),
                    0, sizeof (rpc_inqcntx_codesets_t));
            break;
        }

    }

    inquiry_context->nsattr =
        RPC_NS_LOOKUP_ATTRIBUTE (rpc_c_ns_dec_dns, attribute_class);

    inquiry_context->dns_context = dnsGetHandle();

    if (inquiry_context->dns_context == NULL_DNS_HANDLE)
    {
        rpc__inqcntx_free (&inquiry_context, status);
        *status = rpc_s_no_memory;
        return (rpc_ns_inq_rep_p_t) NULL;
    }

    inquiry_context->classversion_checked = false;

    *status = rpc_s_ok;
    return inquiry_context;
}

/*
**++
**  ROUTINE NAME:       rpc__inqcntx_free
**
**  SCOPE:              PRIVATE - declared in nsutil.h.
**
**  DESCRIPTION:
**
**  Frees an inquiry context.  Note, the caller, being of a specific
**  usage of the inquiry context, is responsible for checking the 
**  validity of the inquiry context passed in from the PUBLIC user.
**
**  INPUTS:
**
**      inquiry_context Address of inquiry context to free.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:
**
**      status          The status of the inquiry context create operation.
**                      One of:
**                          rpc_s_ok,
**                          or status from a subsequent routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE void rpc__inqcntx_free 
#ifdef _DCE_PROTO_
(
    rpc_ns_inq_rep_p_t      *inquiry_context,
    unsigned32              *status
)
#else
(inquiry_context, status)
rpc_ns_inq_rep_p_t      *inquiry_context;
unsigned32              *status;
#endif
{
    CODING_ERROR (status);

    /*
     * Free the DNS Handle
     */
    if ((*inquiry_context)->dns_context != NULL_DNS_HANDLE)
    {
        dnsFreeHandle ((*inquiry_context)->dns_context);
    }

    /*
     * Free the nsentry structure.
     */
    rpc__nsentry_free (&((*inquiry_context)->nsentry), status);

    /*
     * Free the inquiry context.
     */
    RPC_MEM_FREE ((*inquiry_context), RPC_C_MEM_INQ_REP);

    *inquiry_context = NULL;
    *status = rpc_s_ok;
    return;
}


/*
**++
**  ROUTINE NAME:	rpc__ns_status_from_nserror
**
**  SCOPE:              PRIVATE - declared in nsutil.h
**
**  DESCRIPTION:
**
**  Converts a dns status code to an equivalent rpc status code if
**  applicable. If the dns status has no bearing on the rpc operation,
**  then we return the dns status directly.
**
**  INPUTS:
**
**      dns_flags       The DNS status structure.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:
**
**      rpc_status      The equivalent RPC status code or the 
**                      name service status code.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE void rpc__ns_status_from_nserror 
#ifdef _DCE_PROTO_
(
    struct dnsFlagStat      *dns_flags,
    unsigned32              *rpc_status
)
#else
(dns_flags, rpc_status)
struct dnsFlagStat      *dns_flags;
unsigned32              *rpc_status;
#endif
{
    /* 
     * Map "common" name service errors to our
     * own rpc errors.  Return other name service errors to
     * the user.  We are only concerned with returning the 
     * name service local status code, not the architected error.
     */
    switch (dns_flags->fsLocStat)
    {        
        case DNS_ACCESSDENIED:
            *rpc_status = rpc_s_no_ns_privilege;
            break;
            
        case DNS_INVALIDNAME:
            *rpc_status = rpc_s_incomplete_name;
            break;

        case DNS_ENTRYEXISTS:
            *rpc_status = rpc_s_entry_already_exists;
            break;

        case DNS_UNKNOWNENTRY:
            *rpc_status = rpc_s_entry_not_found;
            break;

        case DNS_NONSRESOURCES:
	case DNS_SOCKET:
            *rpc_status = rpc_s_name_service_unavailable;
            break;
        
        case DNS_SUCCESS_STALE:
        case DNS_NOTFND_STALE:
             *rpc_status = rpc_s_name_service_unavailable;
             break;

        case DNS_UNDERSPECIFIEDNAME:
            *rpc_status = rpc_s_underspecified_name;
            break;

        default:
            *rpc_status = dns_flags->fsLocStat;
            break;
    }

    return;
}


/*
**++
**  ROUTINE NAME:       rpc__ns_init_dns_flags
**
**  SCOPE:              PRIVATE - declared in nsutil.h
**
**  DESCRIPTION:
**
**  Initializes the dns flag structure to its default values.
**  Also determines the value of expiration_age and the
**  confidence value to be assigned.
** 
**  INPUTS:
**
**      dns_flags       The dns flag structure
**
**      exp_age         Expiration age of the NS local data (cache)
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none  
**      
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE void rpc__ns_init_dns_flags 
#ifdef _DCE_PROTO_
(
    struct dnsFlagStat      *dns_flags,
    unsigned32              exp_age
)
#else
(dns_flags, exp_age)
struct dnsFlagStat      *dns_flags;
unsigned32              exp_age;
#endif
{
    rpc_thread_context_p_t   thread_context;
    unsigned32               status;
#ifdef AUTH_KRB
    sec_login_handle_t       login_context;
    error_status_t           sec_status;
#endif

    /* 
     *  This next line of code sets the dns flag structure to its 
     *  default values. Some values of interest are:
     *    60 secs timeout
     *    asynchronous responses disabled
     *    authentication enabled
     */
    memset ((char *) dns_flags, 0, sizeof (struct dnsFlagStat));

    /*
     * If the expiration age specifies to bypass the local cache,
     * do this by using a medium confidence level. (This also
     * results in a cache update.)
     * 
     * Otherwise, use low confidence and the specified cache
     * expiration age.
     */
    if (exp_age == RPC_C_NS_BYPASS_CACHE)
    {
        dns_flags->fsConf = dnsConfMed;
    }
    else
    {
        dns_flags->fsConf = dnsConfLow;
        dns_flags->fsCacheTimeout = exp_age;
	/* If we're using the cache, set the MaybeMore bit so that 
	 * if we miss in the cache, CDS will refill more agressively
	 */
	dns_flags->fsMaybeMore = 1;
    }

    /* 
     * If using CDS set fsNameType to use the DCE strings
     * for API name arguments.
     *
     * Also, set the authentication fields to the proper setting:
     * if our global rpc_g_ns_authn is true, then set fsUnAuth and
     * fsTrustAll to zero, otherwise set them to one.
     */
     
    dns_flags->fsNameType = dnsStrDCE;

    RPC_GET_THREAD_CONTEXT(thread_context, &status);
    if (status == rpc_s_ok)
    {
        if (thread_context->ns_authn_state)
        {
#if defined(AUTH_KRB) && !defined(_KERNEL)
            sec_login_get_current_context (&login_context, &sec_status);
            if (sec_status != error_status_ok)
            {
                dns_flags->fsUnAuth = 1;
                dns_flags->fsTrustAll = 1;
            }
#endif
        }
        else
        {
            dns_flags->fsUnAuth = 1;
            dns_flags->fsTrustAll = 1;
        }
    }

}



/*
**++
**
**  ROUTINE NAME:       rpc__if_in_id_list
**
**  SCOPE:              PRIVATE - declared in nsutil.h
**
**  DESCRIPTION:
**      
**  Determines if a specified interface id is found in a given interface
**  list. If it is not found a new list element will be created and this
**  if id will be added to the list.
**
**  INPUTS:
**
**      if_id_list      The list of if id's to be searched.
**
**      if_id           The if id to be searched for.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_coding_error
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      true if the interface id is found in the list
**      false if the interface id is not found in the list
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE boolean rpc__if_in_id_list 
#ifdef _DCE_PROTO_
(
    rpc_list_p_t            if_id_list,
    rpc_if_id_p_t           if_id,
    unsigned32              *status
)
#else
(if_id_list, if_id, status)
rpc_list_p_t            if_id_list;
rpc_if_id_p_t           if_id;
unsigned32              *status;
#endif
{
    rpc_if_list_elt_p_t     if_list_element;
    

    CODING_ERROR (status);
    
    /*
     * get the first element from the interface id list
     */
    RPC_LIST_FIRST ((*if_id_list), if_list_element, rpc_if_list_elt_p_t);

    /*
     * for each element on this list...
     */
    while (if_list_element != NULL)
    {
        /*
         * compare the if uuid and version fields in the given interface id
         * to those in this list element
         */
        if (uuid_equal (&(if_id->uuid),
                &(if_list_element->if_id.uuid), status) &&
            if_id->vers_major == if_list_element->if_id.vers_major &&
            if_id->vers_minor == if_list_element->if_id.vers_minor)
        {
            *status = rpc_s_ok;
            return true;
        }

        /*
         * then get the next element on the list
         */
        RPC_LIST_NEXT (if_list_element, if_list_element, rpc_if_list_elt_p_t);
    }
    
    /*
     * if no match was found on the list, add this element to the list
     * (note: 'true' argument is for a blocking alloc)
     */
    if_list_element = (rpc_if_list_elt_p_t)
        (rpc__list_element_alloc (&rpc_g_free_if_list, true));
    if (if_list_element == NULL){
	*status = rpc_s_no_memory;
	return false;
    }

    if_list_element->if_id.uuid = if_id->uuid;
    if_list_element->if_id.vers_major = if_id->vers_major;
    if_list_element->if_id.vers_minor = if_id->vers_minor;

    RPC_LIST_ADD_TAIL ((*if_id_list), if_list_element, rpc_if_list_elt_p_t);

    *status = rpc_s_ok;    
    return false;
}

/*
**++
**  ROUTINE NAME:       rpc__ns_count_obj_uuids_in_vec 
**
**  SCOPE:              PRIVATE - declared in nsutil.h
**
**  DESCRIPTION:
**
**  Counts the number of non_NULL, non-NIL object uuids in the input vector.
**
**  INPUTS:
**  
**  object_uuid_vec         The object uuid vector which is input for the
**                          PUBLIC routines rpc_ns_binding_export or 
**                          rpc_ns_[mgmt_]binding_unexport. 
**
**  INPUT/OUTPUTS:          none
**
**  OUTPUTS:                none
**
**
**  IMPLICIT INPUTS:        none
**
**  IMPLICIT OUTPUTS:       none
**
**  FUNCTION VALUE:         returns a uuid count (unsigned32)
**
**  SIDE EFFECTS:           none
**
**--
*/

PRIVATE unsigned32 rpc__ns_count_obj_uuids_in_vec 
#ifdef _DCE_PROTO_
(
    uuid_vector_p_t     object_uuid_vec
)
#else
(object_uuid_vec)
uuid_vector_p_t     object_uuid_vec;
#endif
{
    unsigned32      obj_count;
    unsigned32      uuid_count;
    uuid_p_t        *uuid_pp;        /*---* uuid ** for indexing through
                                      *---* the uuid pointer array in the
                                      *---* uuid vector.
                                      *---*/
    unsigned32      local_status;

    obj_count = 0;

    /*
     * Set the obj_count equal to the number of non-NULL elements, non-NIL 
     * UUIDS in object_uuid_vec
     */

    if (object_uuid_vec != (uuid_vector_p_t) NULL)
    {
        /*
         * Initialize the pointer to uuid pointer (uuid_pp) to the beginning
         * of the array of pointers.
         */
        uuid_pp = object_uuid_vec->uuid;

        for ( uuid_count = 0; 
              uuid_count < object_uuid_vec->count; 
              uuid_count++, uuid_pp++ )
        {

            /* 
             * ... if the pointer is NULL or points to a NIL uuid...
             */
            if ( (*uuid_pp == (uuid_p_t) NULL) || 
                 ((uuid_is_nil (*uuid_pp, &local_status)) == true) )
            {
                /*---* then skip it! *---*/
                continue;
            }

            /* 
             * ... else it's a non-NULL pointer and non-NIL object uuid so 
             * increment count
             */
            obj_count++;
        }
    }

    return (obj_count);

}

/*
**++
**  ROUTINE NAME:       rpc__ns_check_rpc_class_version
**
**  SCOPE:              PRIVATE - declared in nsattr.h
**
**  DESCRIPTION:
**
**  Determines whether the client's class version matches
**  the name service entry's class version.
**
**  INPUTS:
**
**      nsentry         Name service representation of an entry name.
**
**      exp_age         Expiration age of the NS local data (cache).
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:
**
**      status          Returns the status code from the 
**                      operation. This status code is a value that
**                      indicates whether the routine completed
**                      successfully and, if not, why.
**                      Returns:
**                          rpc_s_ok,
**                          rpc_s_no_memory,
**                          rpc_s_not_rpc_entry,
**                      or status returned from rpc__ns_status_from_nserror().
**
**  IMPLICIT INPUTS:    rpc_ns_rpc_class_version
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE void rpc__ns_check_rpc_class_version 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t     nsentry,
    unsigned32          exp_age,
    unsigned32          *status
)
#else
(nsentry, exp_age, status)
rpc_nsentry_p_t     nsentry;
unsigned32          exp_age;
unsigned32          *status;
#endif
{
    int                 dns_status;
    rpc_nsattr_t        *nsclass_version;       

    dns_attrvalue_t     attr_value, *attr_value_p; 

    dns_handle          dns_context;
    struct dnsFlagStat  dns_flags;
    unsigned8           temp_vers_major;

    CODING_ERROR (status);

    /*
     * Lookup the RPC_C_ATTR_CLASS_VERSION in the global table.
     */
    nsclass_version = RPC_NS_LOOKUP_ATTRIBUTE  (rpc_c_ns_dec_dns, 
                                            RPC_C_ATTR_CLASS_VERSION);


    /*
     * If using DNSPI V3, use a local variable that
     * matches the structure the V3 interface expects for an attribute
     * value. Initialize fields.
     */

    attr_value.val_syntax = dns_byte;
    attr_value.val_u.val_byte.length = 0;
    attr_value.val_u.val_byte.byte_p = NULL;
    
    /*
     * Setup local pointer to attr_value
     */
    attr_value_p = &attr_value;
    

    /*
     * Read the attribute value to see if attr_type exists
     * for the name service entry. 
     */
    
    /*
     * First initialize dns flags. Use 
     * RPC_C_NS_BYPASS_CACHE for the
     * expiration_age to indicate 
     * we should bypass the cache.
     */
    rpc__ns_init_dns_flags (&dns_flags, exp_age);

    dns_context = dnsGetHandle();
    if (dns_context == (dns_handle) NULL)
    {
        *status = rpc_s_no_memory;
        return;
    }

    /*
     * When using the DNSPI V3 interface, use attr_value
     * and the V3 dnsReadAttrValue() otherwise, use attr_value and attr_value_len
     * and the V2 dnsAddAttrValue().
     */


    dns_status = dnsReadAttrValue (
                    (dns_full_name_t *)nsentry->name, dns_context, 
                    (dns_attr_name_t *)nsclass_version, dnsDirOrObj,
                    (dns_full_name_t *) NULL, &attr_value_p,
                    (dns_cts **) NULL, (unsigned char *) NULL,
                    (int *) NULL, &dns_flags);


    switch (dns_status)
    {
        case DNS_SUCCESS:
        
            /*
             * If successful, check to see if the major versions match.
             * Use attr_value_p for DNSPI V3 (CDS)
             * and attr_value for DNSPI V2.
             */
            memcpy ((unsigned8  *) &temp_vers_major,
                    (unsigned8  *) (attr_value_p->val_u.val_byte.byte_p),
                    sizeof(unsigned8));

            if (temp_vers_major != rpc_ns_rpc_class_version.vers_major)
            {
                *status = rpc_s_class_version_mismatch;
            }
            else
            {
                *status = rpc_s_ok;
            }
            break;

#ifdef DNS_UNKNOWNATTRIBUTE
        case DNS_UNKNOWNATTRIBUTE:
#endif
        case DNS_EMPTY:
                        
            /* 
             * A return status of DNS_EMPTY means the class version
             * attribute does not exist. 
             */
            *status = rpc_s_not_rpc_entry;
            break;
                        
        default:
            /*
             * Return an appropriate error.
             */
            rpc__ns_status_from_nserror (&dns_flags, status);
            break;
    }

    dnsFreeHandle (dns_context);

    return;

}



