/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsattr.c,v $
 * Revision 1.1.518.2  1996/02/18  00:04:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:00  marty]
 *
 * Revision 1.1.518.1  1995/12/08  00:21:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:59:57  root]
 * 
 * Revision 1.1.516.2  1994/07/29  16:19:15  tom
 * 	Remove DNSPI_V2 ifdef.
 * 	[1994/07/28  22:33:52  tom]
 * 
 * Revision 1.1.516.1  1994/01/21  22:38:26  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:58:51  cbrooks]
 * 
 * Revision 1.1.4.4  1993/03/01  19:30:24  sommerfeld
 * 	Add debug printf to watch RPC_ClassVersion check behavior.
 * 	[1993/02/25  18:48:06  sommerfeld]
 * 
 * 	[7351] remove unnecessary ifdef.
 * 	[1993/02/25  15:19:34  sommerfeld]
 * 
 * 	[7351] Avoid excessive numbers of checks of RPC_ClassVersion.
 * 	[1993/02/25  01:01:06  sommerfeld]
 * 
 * Revision 1.1.4.3  1993/01/03  23:53:28  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:09:58  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  21:13:12  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:41:43  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  16:38:39  rsalz
 * 	 3-mar-92    wh      DCE 1.0.1 merge.
 * 	21-jan-92    ebm     Change rpc__nsattr_read_resolution to reflect
 * 	                     successful status if the attribute read was
 * 	                     not found.
 * 	[1992/05/01  16:30:19  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:12:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**    NSATTR.C
**
**  FACILITY:
**
**    Remote procedure call (RPC) name service independent interface (NS)
**  
**  ABSTRACT:
**
**  Set of routines that create and manipulate the name service
**  representation of an attribute:
**  
**
**/

#include <commonp.h>
#include <com.h> 
#include <ns.h>              /* Private externals for other RPC components */
#include <nsp.h>             /* Private defs for Naming Service component */
#include <nsutil.h>          /* Private name service specific routines */
#include <nsattr.h>          /* Private name service attribute routines */


/*
**++
**  ROUTINE NAME:       rpc__nsattr_add_value
**
**  SCOPE:              PRIVATE - declared in nsattr.h
**
**  DESCRIPTION:
**
**  The routine adds a value to an attribute of the specified entry. If the
**  attribute does not already exists, this routine creates the attribute.
**
**  INPUTS:
**
**      nsentry             Name service representation of an entry name.
**      attr_type           Integer specifying an RPC attribute.
**      nsattr_value        Name service representation of the attribute value.
**      nsattr_value_len    Length of nsattr_value.
**
**  INPUT/OUTPUTS:          none
**
**  OUTPUTS:
**
**      status              Returns the status code from the add_value
**                          operation. This status code is a value that
**                          indicates whether the routine completed
**                          successfully and, if not, why.
**                          Returns:
**                            rpc_s_ok
**                            or a status from a subsequent called routine.
**
**  IMPLICIT INPUTS:        none
**
**  IMPLICIT OUTPUTS:       none
**
**  FUNCTION VALUE:         none
**
**  SIDE EFFECTS:           none
**
**--
*/

PRIVATE void rpc__nsattr_add_value 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t     nsentry,
    unsigned32          attr_type,
    unsigned_char_p_t   nsattr_value,
    unsigned32          nsattr_value_len,
    unsigned32          *status
)
#else
(nsentry, attr_type, nsattr_value, nsattr_value_len, status)
rpc_nsentry_p_t     nsentry;
unsigned32          attr_type;
unsigned_char_p_t   nsattr_value;
unsigned32          nsattr_value_len;
unsigned32          *status;
#endif
{
    int                 dns_status;
    rpc_nsattr_t        *nsattr_name;   /* attribute in ns representation */
    struct dnsFlagStat  dns_flags;

    dns_attrvalue_t    local_nsattr_value; /* V3 DNSPI attribute value structure */ 

    CODING_ERROR (status);

    nsattr_name = RPC_NS_LOOKUP_ATTRIBUTE  ( rpc_c_ns_dec_dns, attr_type);

    /*
     * If using DNSPI V3, use a local variable that
     * matches the structure the V3 interface expects for an attribute
     * value. Load the local variable with information in nsattr*
     * variables.
     */
     
    local_nsattr_value.val_syntax = dns_byte;
    local_nsattr_value.val_u.val_byte.length = (unsigned short)nsattr_value_len;
    local_nsattr_value.val_u.val_byte.byte_p = (void *)nsattr_value;
    

    /*
     * Add the value to the attribute.  The name service will
     * create the attribute if it does not already exist.
     */

    /*
     * First initialize dns flags. Use 
     * RPC_C_NS_BYPASS_CACHE for the
     * expiration_age to indicate 
     * we should bypass the cache.
     */

    rpc__ns_init_dns_flags (&dns_flags, RPC_C_NS_BYPASS_CACHE);

    /* First determine whether the value already exists on the
     * entry. If the value exists, do nothing. Otherwise go ahead
     * and add the value. This is a performance enhancement.
     */
     
    /*
     * When using the DNSPI V3 interface, use local_nsattr_value
     * and the V3 dnsTestAttrValue() otherwise, use nsattr_value and nsattr_value_len
     * and the V2 dnsTestAttrValue()
     */

    dns_status = dnsTestAttrValue (
                    (dns_full_name_t *)nsentry->name, 
                    (dns_attr_name_t *)nsattr_name, dnsObj, &local_nsattr_value,
                    &dns_flags);

    switch (dns_status)
    {
        case DNS_NOTFND:
            /*
             * The attribute value was not found. Add the value.
             * When using the DNSPI V3 interface, use local_nsattr_value
             * and the V3 dnsAddAttrValue() otherwise, use nsattr_value and 
             * nsattr_value_len and the V2 dnsAddAttrValue()
             */
    
            dns_status = dnsAddAttrValue (
                        (dns_full_name_t *) nsentry->name,   
                        (dns_attr_name_t *)nsattr_name,dnsSet,dnsObj,
                        &local_nsattr_value, (dns_cts *) NULL, &dns_flags);
    
            if (dns_status == DNS_SUCCESS)
            {
                *status = rpc_s_ok;
            }
            else
            {
                rpc__ns_status_from_nserror (&dns_flags, status);
            }
    
            break;
                        
        case DNS_SUCCESS:
            /*
             * The attribute value exists. Do nothing and return
             * success.
             */
            *status = rpc_s_ok;
            break;

        default: 
            /*
             * An error occurred on dnsTestAttrValue.
             * Return an appropriate error status.
             */
            rpc__ns_status_from_nserror (&dns_flags, status);
            break;
    }

    
    return;
}


/*
**++
**  ROUTINE NAME:       rpc__nsattr_delete
**
**  SCOPE:              PRIVATE - declared in nsattr.h
**
**  DESCRIPTION:
**
**  Delete an attribute and its values, if any.
**
**  INPUTS:
**
**      nsentry         Name service representation of an entry name.
**      attr_type       Integer specifying an RPC attribute.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:
**
**      status          Returns the status code from the attribute-delete
**                      operation. This status code is a value that
**                      indicates whether the routine completed
**                      successfully and, if not, why.
**                      Returns:
**                          rpc_s_ok,
**                      or other rpc status from rpc__ns_status_from_nserror().
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
*/
PRIVATE void rpc__nsattr_delete 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t     nsentry,
    unsigned32          attr_type,
    unsigned32          *status
)
#else
(nsentry, attr_type, status)
rpc_nsentry_p_t     nsentry;
unsigned32          attr_type;
unsigned32          *status;
#endif
{
    int                 dns_status;
    rpc_nsattr_t        *nsattr_name;   /* attribute in ns representation */
    struct dnsFlagStat  dns_flags;

    CODING_ERROR (status);


    /*
     * Lookup the ns opaque simple name in the global table.
     */
    nsattr_name = RPC_NS_LOOKUP_ATTRIBUTE  (rpc_c_ns_dec_dns, attr_type);

    /*
     * Delete the RPC attribute from the nameservice entry.
     */
     
    /*
     * First initialize dns flags. Use 
     * RPC_C_NS_BYPASS_CACHE for the
     * expiration_age to indicate 
     * we should bypass the cache.
     */

    rpc__ns_init_dns_flags (&dns_flags, RPC_C_NS_BYPASS_CACHE);
    dns_status = dnsRmAttr (
                    (dns_full_name_t *) nsentry->name, 
                    (dns_attr_name_t *)nsattr_name, dnsSet, dnsObj,
                    (dns_cts *) NULL, &dns_flags);
                                            
    if (dns_status == DNS_SUCCESS)
    {
        *status = rpc_s_ok;
    }
    else
    {
        rpc__ns_status_from_nserror (&dns_flags, status);
    }

    return;
}


/*
**++
**  ROUTINE NAME:       rpc__nsattr_exists
**
**  SCOPE:              PRIVATE - declared in nsattr.h
**
**  DESCRIPTION:
**
**  Determines whether  an attribute exists.
**
**  INPUTS:
**
**      nsentry         Name service representation of an entry name.
**      attr_type       Integer specifying an RPC attribute.
**      exp_age         Expiration age of the local name service data (cache).
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:
**
**      status          Returns the status code from the attr_exists
**                      operation. This status code is a value that
**                      indicates whether the routine completed
**                      successfully and, if not, why.
**                      Returns:
**                          rpc_s_ok,
**                          rpc_s_no_memory,
**                          rpc_s_not_found,
**                      or status returned from rpc__ns_status_from_nserror().
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      Returns true or false to indicate whether or not the 
**      specified attribute exists in the name space entry.
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE boolean rpc__nsattr_exists 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t     nsentry,
    unsigned32          attr_type,
    unsigned32          exp_age,
    unsigned32          *status
)
#else
(nsentry, attr_type, exp_age, status)
rpc_nsentry_p_t     nsentry;
unsigned32          attr_type;
unsigned32          exp_age;
unsigned32          *status;
#endif
{
    int                 dns_status;
    rpc_nsattr_t        *nsattr_name;       

    dns_attrvalue_t     attr_value, *attr_value_p; 

    dns_handle          dns_context;
    struct dnsFlagStat  dns_flags;

    CODING_ERROR (status);

    /*
     * Lookup the ns opaque simple name in the global table.
     */
    nsattr_name = RPC_NS_LOOKUP_ATTRIBUTE  (rpc_c_ns_dec_dns, attr_type);


    /*
     * If using DNSPI V3, use a local variable that
     * matches the structure the V3 interface expects for an attribute
     * value. Initialize field.
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
     * current exp_age for the
     * expiration_age to indicate 
     * we should bypass the cache.
     */
    rpc__ns_init_dns_flags (&dns_flags, exp_age );

    dns_context = dnsGetHandle();
    if (dns_context == (dns_handle) NULL)
    {
        *status = rpc_s_no_memory;
        return false;
    }

    /*
     * When using the DNSPI V3 interface, use attr_value
     * and the V3 dnsReadAttrValue() otherwise, use attr_value and attr_value_len
     * and the V2 dnsAddAttrValue().
     */


    dns_status = dnsReadAttrValue (
                    (dns_full_name_t *)nsentry->name, dns_context, 
                    (dns_attr_name_t *)nsattr_name, dnsDirOrObj,
                    (dns_full_name_t *) NULL, &attr_value_p,
                    (dns_cts **) NULL, (unsigned char *) NULL,
                    (int *) NULL, &dns_flags);


    dnsFreeHandle (dns_context);

    switch (dns_status)
    {
        case DNS_SUCCESS:
            *status = rpc_s_ok;
            return true;
            break;		/* NOTREACHED */

        case DNS_EMPTY:
            *status = rpc_s_not_found;
            return false;
            break;		/* NOTREACHED */

        default:
            /*
             * This case includes a DNS status of DNS_ERROR with
             * a flags value of DNS_UNKNOWN_ENTRY.  The called
             * routine maps DNS_UNKNOWN_ENTRY into rpc_s_entry_not_found.
             */
            rpc__ns_status_from_nserror (&dns_flags, status);
            return false;
            break;		/* NOTREACHED */
    }
}


/*
**++
**  ROUTINE NAME:      rpc__nsattr_read_resolution
**
**  SCOPE:             PRIVATE - declared in nsattr.h
**
**  DESCRIPTION:
**
**  rpc__nsattr_read_resolution attempts to read an attribute in order
**  to see if the entry name specified can be fully resolved.  The
**  results of the name resolution are passed back to the caller.
**
**  INPUTS:
**      nsentry         Name service representation of an entry name.
**      attr_type       Integer specifying an RPC attribute.
**
**  INPUT/OUTPUTS:      
**      resolved_name   A pointer to the portion of the entry name that
**                      was resolved by the name-service.
**
**  OUTPUTS:            
**      status          Returns the status code from the read_resolution
**                      operation.  This status code is a value that indicates
**                      whether the routine completed successfully and,
**                      if not, why.  
**                      One of:
**                          rpc_s_ok
**                          rpc_s_entry_not_found
**                          or some other status from a called routine.
**                      none
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

PRIVATE void rpc__nsattr_read_resolution 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t     nsentry,
    unsigned32          attr_type,
    unsigned_char_t     *resolved_name,
    unsigned32          *resolved_name_length,
    unsigned32          *status
)
#else
(nsentry, attr_type, resolved_name, resolved_name_length, status)
rpc_nsentry_p_t     nsentry;
unsigned32          attr_type;
unsigned_char_t     *resolved_name;
unsigned32          *resolved_name_length;
unsigned32          *status;
#endif
{
    int                 dns_status;
    rpc_nsattr_t        *nsattr_name;
    dns_full_name_t     partial_results;
    dns_handle          dns_context;
    struct dnsFlagStat  dns_flags;

    dns_attrvalue_t     local_nsattr_value,*local_nsattr_value_p; 
                    

    CODING_ERROR (status);

    /*
     * If using DNSPI V3, use a local variable,local_nsattr_value, that
     * matches the structure the V3 interface expects for an attribute
     * value. Initialize the structure. 
     */

     
    local_nsattr_value.val_syntax = dns_byte;
    local_nsattr_value.val_u.val_byte.length = 0;
    local_nsattr_value.val_u.val_byte.byte_p = NULL;

    /*
     * Setup local pointer to attr_value
     */
    local_nsattr_value_p = &local_nsattr_value;
    

    /*
     * Initialize the dns flags and set it up to return the
     * partial results (resolved and unresolved portions).
     * Set the cache expiration age to indicate 
     * we should bypass the cache.
     */

    rpc__ns_init_dns_flags (&dns_flags, RPC_C_NS_BYPASS_CACHE);
    dns_flags.fsPartialRes = &partial_results;

    dns_flags.fsResLength = DNS_FNAME_MAX;

    dns_context = dnsGetHandle();
    if (dns_context == (dns_handle) NULL)
    {
        *status = rpc_s_no_memory;
        return;
    }

    /*
     * Lookup the ns opaque simple name in the global table.
     */
    nsattr_name = RPC_NS_LOOKUP_ATTRIBUTE (rpc_c_ns_dec_dns, attr_type);

    /*
     * Read the attribute value using the dns_handle, nsentry,
     * and DNSPI_V3 local attribute structure.
     */
    dns_status = dnsReadAttrValue (
                    (dns_full_name_t *)nsentry->name, 
                    dns_context, (dns_attr_name_t *)nsattr_name, 
                    dnsDirOrObj, (dns_full_name_t *) NULL,
                    &local_nsattr_value_p, (dns_cts **) NULL,
                    (unsigned char *) NULL, (int *) NULL, &dns_flags);

    switch (dns_status)
    {
        case DNS_SUCCESS:
        case DNS_EMPTY:
            /* 
             * If read is successful, or attribute is not found, then we
             * set status to successful, since in both cases the entry 
             * name was fully resolvable.
             */
            *status = rpc_s_ok;
            break;

        default:
            rpc__ns_status_from_nserror (&dns_flags, status);
            break;
    }

    /*
     * If successful or entry was not found, return the resolution results.
     */
    if ((*status == rpc_s_ok) ||
        (*status == rpc_s_entry_not_found))
    {
        /*
         * If read was unsuccessful due to entry_not_found, then the 
         * partial results are in the same format that the entry name
         * was passed in (dns_full_name_t).
         * Otherwise, the entry given is fully resolvable -
         * DNS does not return any partial results.
         */
        if (*status == rpc_s_entry_not_found)
        {
            strcpy ((char *) resolved_name, (char *) dns_flags.fsPartialRes);
        }
        else
        {
            strcpy ((char *)resolved_name, (char *)nsentry->name);
        }

        /*
         * Calculate the length for both instances.
         * (CDS/DNS includes NUL in its string length.)
         */
        *resolved_name_length = strlen ((char *)resolved_name);
    }

    dnsFreeHandle (dns_context);

    /*
     * Return with status from the read attribute value operation.
     */
    return;
}


/*
**++
**  ROUTINE NAME:       rpc__nsattr_read_value
**
**  SCOPE:              PRIVATE - declared in nsattr.h
**
**  DESCRIPTION:
**
**  Reads a value from a name service attribute.
**
**  INPUTS:
**
**      inquiry_context     An inquiry context.
**
**  INPUT/OUTPUTS:          none
**
**  OUTPUTS:
**
**      nsattr_value        Pointer to NS representation of the attribute value.
**      nsattr_value_len    Length of the attribute value returned.
**      status              Returns the status code from the read-value
**                          operation. This status code is a value that
**                          indicates whether the routine completed
**                          successfully and, if not, why.
**                          Can be one of:
**                              rpc_s_ok
**                              rpc_s_no_more_members
**                          or status returned by rpc__ns_status_from_nserror().
**
**  IMPLICIT INPUTS:        none
**
**  IMPLICIT OUTPUTS:       none
**
**  FUNCTION VALUE:         none
**
**  SIDE EFFECTS:           none
**
**--
*/

PRIVATE void rpc__nsattr_read_value 
#ifdef _DCE_PROTO_
(
    rpc_ns_inq_rep_p_t      inquiry_context,
    unsigned_char_p_t       *nsattr_value,
    unsigned32              *nsattr_value_len,
    unsigned32              *status
)
#else
(inquiry_context, nsattr_value, nsattr_value_len, status)
rpc_ns_inq_rep_p_t      inquiry_context;
unsigned_char_p_t       *nsattr_value;
unsigned32              *nsattr_value_len;
unsigned32              *status;
#endif
{
    int                 dns_status;
    struct dnsFlagStat  dns_flags;

                       /* V3 DNSPI attribute value structure */ 
    dns_attrvalue_t    local_nsattr_value, *local_nsattr_value_p; 

    CODING_ERROR (status);


    /*
     * If using DNSPI V3, use a local variable,local_nsattr_value, that
     * matches the structure the V3 interface expects for an attribute
     * value. Initialize the structure. 
     */

    local_nsattr_value.val_syntax = dns_byte;
    local_nsattr_value.val_u.val_byte.length = 0;
    local_nsattr_value.val_u.val_byte.byte_p = NULL;

    /*
     * set local ptr
     */
    local_nsattr_value_p = &local_nsattr_value;


    /*
     * Initialize the dns flags and set it up to read
     * the entire entry, if space permits in the clerk's cache.
     * Use the cache expiration age set by the application in
     * the inquiry context.
     */
     
    rpc__ns_init_dns_flags (&dns_flags, inquiry_context->common.exp_age);
    dns_flags.fsMaybeMore = 1;


    /*
     * Check the entry's class version first.
     */
    /*
     * Only issue the rpc__ns_check_rpc_class_version call if
     * the classversion for this inquiry context has not
     * yet been checked.  If it has already been checked
     * just return the saved status from the inquiry context.
     */
    if (inquiry_context->classversion_checked)
    {
	RPC_DBG_PRINTF(rpc_e_dbg_general, 5,
	    ("(rpc__nsattr_read_value) skipping classversion check for %s\n",
	    inquiry_context->nsentry->name));
	*status = inquiry_context->classversion_check_status;
    }
    else 
    {
	RPC_DBG_PRINTF(rpc_e_dbg_general, 5,
	    ("(rpc__nsattr_read_value) doing classversion check for %s\n",
	    inquiry_context->nsentry->name));
	rpc__ns_check_rpc_class_version (
                                  inquiry_context->nsentry,
                                inquiry_context->common.exp_age, status );

        inquiry_context->classversion_check_status = *status;
        inquiry_context->classversion_checked = true;
    }
    
    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Read the attribute value using the dns_handle, nsentry,
     * and nsattr from the inquiry context.
     */


    /*
     * When using the DNSPI V3 interface, use local_nsattr_value
     * and the V3 dnsReadAttrValue() otherwise, use nsattr_value and nsattr_value_len
     * and the V2 dnsReadAttrValue()
     */

    
    dns_status = dnsReadAttrValue (
                    (dns_full_name_t *)inquiry_context->nsentry->name, 
                    inquiry_context->dns_context, 
                    (dns_attr_name_t *)inquiry_context->nsattr, 
                    dnsDirOrObj, (dns_full_name_t *) NULL,
                    &local_nsattr_value_p, (dns_cts **) NULL,
                    (unsigned char *) NULL, (int *) NULL, &dns_flags);

    switch (dns_status)
    {
        case DNS_SUCCESS:
        {    
        
        
            /*
             * If using DNSPI_V3 interface, copy values in local_nsattr_value
             * structure to back original variables.
             */
             
            *nsattr_value = (unsigned_char_p_t)(local_nsattr_value_p->val_u.val_byte.byte_p);
            *nsattr_value_len = (unsigned32)(local_nsattr_value_p->val_u.val_byte.length);
            *status = rpc_s_ok;
            break;
        }
        case DNS_EMPTY:
            *status = rpc_s_no_more_members;
            break;

        default:
            rpc__ns_status_from_nserror (&dns_flags, status);
            break;
    }

    return;
}


/*
**++
**  ROUTINE NAME:       rpc__nsattr_remove_value  
**
**  SCOPE:              PRIVATE - declared in nsattr.h
**
**  DESCRIPTION:
**
**  Removes a value from an attribute.
**
**  INPUTS:
**
**      nsentry             Name service representation of an entry name.
**      attr_type           Integer specifying an RPC attribute.
**      nsattr_value        Name service representation of the attribute value
**                          to remove.
**      nsattr_value_len    The length of nsattr_value.
**
**  INPUT/OUTPUTS:          none
**
**  OUTPUTS:
**
**      status              Returns the status code from the remove-value
**                          operation. This status code is a value that
**                          indicates whether the routine completed
**                          successfully and, if not, why.
**                          Returns:
**                              rpc_s_ok,
**                              rpc_s_not_found,
**                          or status returned from 
**                              rpc__ns_status_from_nserror().
**
**  IMPLICIT INPUTS:        none
**
**  IMPLICIT OUTPUTS:       none
**
**  FUNCTION VALUE:         none
**
**  SIDE EFFECTS:           none
**
**--
*/

PRIVATE void rpc__nsattr_remove_value 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t     nsentry,
    unsigned32          attr_type,
    unsigned_char_p_t   nsattr_value,
    unsigned32          nsattr_value_len,
    unsigned32          *status
)
#else
(nsentry, attr_type, nsattr_value, nsattr_value_len, status)
rpc_nsentry_p_t     nsentry;
unsigned32          attr_type;
unsigned_char_p_t   nsattr_value;
unsigned32          nsattr_value_len;
unsigned32          *status;
#endif
{
    int                 dns_status;
    rpc_nsattr_t        *nsattr_name;   /* attribute in ns representation */
    struct dnsFlagStat  dns_flags;
    
    dns_attrvalue_t    local_nsattr_value; /* V3 DNSPI attribute value structure */

    CODING_ERROR (status);


    /*
     * If using DNSPI V3, use a local variable that
     * matches the structure the V3 interface expects for an attribute
     * value. Load the local variable with information in nsattr*
     * variables.
     */

    local_nsattr_value.val_syntax = dns_byte;
    local_nsattr_value.val_u.val_byte.length = nsattr_value_len;
    local_nsattr_value.val_u.val_byte.byte_p = (void *)nsattr_value;


    /*
     * Lookup the RPC attribute in name service representation.
     */
    nsattr_name = RPC_NS_LOOKUP_ATTRIBUTE (rpc_c_ns_dec_dns, attr_type);

    /*
     * First initialize dns flags. Use 
     * RPC_C_NS_BYPASS_CACHE for the
     * expiration_age to indicate 
     * we should bypass the cache.
     */
     
    rpc__ns_init_dns_flags (&dns_flags, RPC_C_NS_BYPASS_CACHE);

    /*
     * When using the DNSPI V3 interface, use local_nsattr_value
     * and the V3 dnsTestAttrValue() otherwise, use nsattr_value and nsattr_value_len
     * and the V2 dnsTestAttrValue()
     */

    dns_status = dnsTestAttrValue (
                    (dns_full_name_t *)nsentry->name, 
                    (dns_attr_name_t *)nsattr_name, dnsObj, &local_nsattr_value,
                    &dns_flags);

    switch (dns_status)
    {
        case DNS_SUCCESS:

            /*
             * Now that we know the attribute value exists, 
             * remove it.
             */
             

    /*
     * When using the DNSPI V3 interface, use local_nsattr_value and
     * the V3 dnsRmAttrValue() otherwise, use nsattr_value and nsattr_value_len
     * and the V2 RmAddAttrValue().
     */
            dns_status = dnsRmAttrValue (
                            (dns_full_name_t *) nsentry->name, 
                            (dns_attr_name_t *) nsattr_name, dnsObj,
                            &local_nsattr_value, (dns_cts *) NULL, &dns_flags);
            if (dns_status == DNS_SUCCESS)
            {
                /*
                 * The attribute value was successfully removed.
                 */
                *status = rpc_s_ok;
            }
            else
            {
                /*
                 * An error occurred with the Remove.
                 */
                rpc__ns_status_from_nserror (&dns_flags, status);
            }
            break;

        case DNS_NOTFND:
            /*
             * The attribute value was not found or the attribute
             * does not exists.  Return this fact to the caller.
             */
             *status = rpc_s_not_found;
             break;

        default:
            /* 
             * Error occurred on dnsTestAttrValue.  It may be due
             * to the entry not being found, in which rpc_s_entry_not_found
             * will be returned.
             */
            rpc__ns_status_from_nserror (&dns_flags, status);
            break;

    }

    return;
}

