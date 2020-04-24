/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_dns.c,v $
 * Revision 1.1.5.2  1996/02/18  00:17:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:41  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:51:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:20:24  root]
 * 
 * Revision 1.1.2.5  1993/02/01  23:55:45  ahop
 * 	bl6e: malloc buffer for nsattr_value in rs_dns_attr_read_value
 * 	[1993/01/29  23:07:08  ahop]
 * 
 * Revision 1.1.2.4  1992/12/29  16:34:59  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:23:33  zeliff]
 * 
 * Revision 1.1.2.3  1992/12/18  22:11:50  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:49:07  sekhar]
 * 
 * Revision 1.1.3.2  1992/12/16  19:11:38  emartin
 * 	bl6c: fix 6482: call dnsFreeHandle before returning from rs_dns_attr_read_value
 * 
 * Revision 1.1.2.2  1992/08/31  18:24:16  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  17:53:00  sekhar]
 * 
 * $EndLog$
 */

#include <dce/rpc.h> 

#define DNS_CDS     1
#define DNS_V3API   1

#include <dce/dnsclerk.h>

#include <un_strng.h>

#include <rs_base.h>
#include <rs_dns.h>

/* 
 *  Constants and typedefs cloned from 
 *  rpc/runtime/nsp.h
 */

/*
 * The value that when passed as the
 * expiration_age to a name service call
 * indicates that name service data inquiries should go
 * directly to the server. This results in a local
 * data (or cache) update as a consequence.
 */
#define rs_dns_c_bypass_cache   0


#define rs_dns_c_attr_name_max  (DNS_STR_SNAME_MAX+1)

/*
 * Name Service Attribute definitions
 */
typedef struct 
{
    unsigned char     name[rs_dns_c_attr_name_max];
} rs_dnsattr_t, *rs_dnsattr_p_t;


/*
 * Prototypes for local routines
 */

PRIVATE void rs_dns_init_dns_flags (
#ifdef __STDC__
    struct dnsFlagStat      *dns_flags,
    unsigned32              exp_age
#endif
);

PRIVATE void rs_dns_rpc_status_from_dnserror (
#ifdef __STDC__
    struct dnsFlagStat      *dns_flags,
    unsigned32              *rpc_status
#endif
);



/*
**++
**  ROUTINE NAME:       rs_dns_attr_add_value
**
**  SCOPE:              PUBLIC
**
**  DESCRIPTION:
**
**  Cloned from rpc/runtime/nsattr.c/rpc__nsattr_add_value
** 
**  The routine adds a value to an attribute of the specified entry. If the
**  attribute does not already exists, this routine creates the attribute.
**
**  INPUTS:
**
**      ns_name             Name service representation of an entry name.
**      attr_name           Name of attribute.
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

PUBLIC void rs_dns_attr_add_value 
#ifndef __STDC__
    (ns_name, attr_name, nsattr_value, nsattr_value_len, status)
    unsigned_char_p_t   ns_name;
    unsigned_char_p_t   attr_name;
    unsigned_char_p_t   nsattr_value;
    unsigned32          nsattr_value_len;
    unsigned32          *status;
#else
  (
    unsigned_char_p_t   ns_name,
    unsigned_char_p_t   attr_name,
    unsigned_char_p_t   nsattr_value,
    unsigned32          nsattr_value_len,
    unsigned32          *status
  )
#endif
{
    int                 dns_status;
    rs_dnsattr_t        nsattr_name;    /* attribute in ns representation */
    struct dnsFlagStat  dns_flags;

#ifndef DNSPI_V2
    dns_attrvalue_t    local_nsattr_value; /* V3 DNSPI attribute value structure */ 
#endif

    if (strlen((char *) attr_name) > rs_dns_c_attr_name_max) {
        *status = rpc_s_string_too_long;
        return;
    }
    strcpy((char *) nsattr_name.name, (char *) attr_name);

#ifndef DNSPI_V2
    /*
     * If using DNSPI V3, use a local variable that
     * matches the structure the V3 interface expects for an attribute
     * value. Load the local variable with information in nsattr*
     * variables.
     */
     
    local_nsattr_value.val_syntax = dns_byte;
    local_nsattr_value.val_u.val_byte.length = (unsigned short)nsattr_value_len;
    local_nsattr_value.val_u.val_byte.byte_p = (void *)nsattr_value;
    
#endif

    /*
     * Add the value to the attribute.  The name service will
     * create the attribute if it does not already exist.
     */

    /*
     * First initialize dns flags. Use 
     * rs_c_ns_bypass_cache for the
     * expiration_age to indicate 
     * we should bypass the cache.
     */

    rs_dns_init_dns_flags (&dns_flags, rs_dns_c_bypass_cache);

    /* First determine whether the value already exists on the
     * entry. If the value exists, do nothing. Otherwise go ahead
     * and add the value. This is a performance enhancement.
     */
     
#ifndef DNSPI_V2
    /*
     * When using the DNSPI V3 interface, use local_nsattr_value
     * and the V3 dnsTestAttrValue() otherwise, use nsattr_value and nsattr_value_len
     * and the V2 dnsTestAttrValue()
     */

    dns_status = dnsTestAttrValue (
                    (dns_full_name_t *)ns_name, 
                    (dns_attr_name_t *)&nsattr_name, 
                    dnsObj, &local_nsattr_value,
                    &dns_flags);
#else

    dns_status = dnsTestAttrValue (
                    ns_name, (dns_attr_name_t *) &nsattr_name, 
                    dnsObj, nsattr_value, nsattr_value_len, &dns_flags);
#endif

    switch (dns_status)
    {
        case DNS_NOTFND:
            /*
             * The attribute value was not found. Add the value.
             * When using the DNSPI V3 interface, use local_nsattr_value
             * and the V3 dnsAddAttrValue() otherwise, use nsattr_value and 
             * nsattr_value_len and the V2 dnsAddAttrValue()
             */
#ifndef DNSPI_V2
    
            dns_status = dnsAddAttrValue (
                        (dns_full_name_t *) ns_name,   
                        (dns_attr_name_t *)&nsattr_name,dnsSingle,dnsObj,
                        &local_nsattr_value, (dns_cts *) NULL, &dns_flags);
#else
            dns_status = dnsAddAttrValue (
                        ns_name, (dns_attr_name_t *)&nsattr_name, 
                        dnsSingle, dnsObj,nsattr_value, nsattr_value_len, 
                        (dns_cts *) NULL, &dns_flags);

#endif                                                                 
    
            if (dns_status == DNS_SUCCESS)
            {
                *status = rpc_s_ok;
            }
            else
            {
                rs_dns_rpc_status_from_dnserror (&dns_flags, status);
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
            rs_dns_rpc_status_from_dnserror (&dns_flags, status);
            break;
    }

    
    return;
}

/*
**++
**  ROUTINE NAME:       rs_dns_attr_remove_value
**
**  SCOPE:              PUBLIC
**
**  DESCRIPTION:
**
**  Cloned from rpc/runtime/nsattr.c/rpc__nsattr_remove_value
** 
**  The routine adds a value to an attribute of the specified entry. If the
**  attribute does not already exists, this routine creates the attribute.
**
**  INPUTS:
**
**      ns_name             Name service representation of an entry name.
**      attr_name           Name of attribute.
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

PUBLIC void rs_dns_attr_remove_value 
#ifndef __STDC__
    (ns_name, attr_name, nsattr_value, nsattr_value_len, status)
    unsigned_char_p_t   ns_name;
    unsigned_char_p_t   attr_name;
    unsigned_char_p_t   nsattr_value;
    unsigned32          nsattr_value_len;
    unsigned32          *status;
#else
  (
    unsigned_char_p_t   ns_name,
    unsigned_char_p_t   attr_name,
    unsigned_char_p_t   nsattr_value,
    unsigned32          nsattr_value_len,
    unsigned32          *status
  )
#endif

{
    int                 dns_status;
    rs_dnsattr_t        nsattr_name;    /* attribute in ns representation */
    struct dnsFlagStat  dns_flags;

#ifndef DNSPI_V2
    dns_attrvalue_t    local_nsattr_value; /* V3 DNSPI attribute value structure */ 
#endif

    if (strlen((char *) attr_name) > rs_dns_c_attr_name_max) {
        *status = rpc_s_string_too_long;
        return;
    }
    strcpy((char *) nsattr_name.name, (char *) attr_name);

#ifndef DNSPI_V2

    /*
     * If using DNSPI V3, use a local variable that
     * matches the structure the V3 interface expects for an attribute
     * value. Load the local variable with information in nsattr*
     * variables.
     */

    local_nsattr_value.val_syntax = dns_byte;
    local_nsattr_value.val_u.val_byte.length = nsattr_value_len;
    local_nsattr_value.val_u.val_byte.byte_p = (void *)nsattr_value;

#endif

    /*
     * First initialize dns flags. Use 
     * rs_dns_c_bypass_cache for the
     * expiration_age to indicate 
     * we should bypass the cache.
     */
     
    rs_dns_init_dns_flags (&dns_flags, rs_dns_c_bypass_cache);

#ifndef DNSPI_V2
    /*
     * When using the DNSPI V3 interface, use local_nsattr_value
     * and the V3 dnsTestAttrValue() otherwise, use nsattr_value and nsattr_value_len
     * and the V2 dnsTestAttrValue()
     */

    dns_status = dnsTestAttrValue (
                    (dns_full_name_t *)ns_name, 
                    (dns_attr_name_t *)&nsattr_name, dnsObj, &local_nsattr_value,
                    &dns_flags);
#else

    dns_status = dnsTestAttrValue (
                    ns_name, (dns_attr_name_t *) &nsattr_name, 
                    dnsObj, nsattr_value, nsattr_value_len, &dns_flags);
#endif

    switch (dns_status)
    {
        case DNS_SUCCESS:

            /*
             * Now that we know the attribute value exists, 
             * remove it.
             */
             
#ifndef DNSPI_V2

    /*
     * When using the DNSPI V3 interface, use local_nsattr_value and
     * the V3 dnsRmAttrValue() otherwise, use nsattr_value and nsattr_value_len
     * and the V2 RmAddAttrValue().
     */
            dns_status = dnsRmAttrValue (
                            (dns_full_name_t *) ns_name, 
                            (dns_attr_name_t *) &nsattr_name, dnsObj,
                            &local_nsattr_value, (dns_cts *) NULL, &dns_flags);
#else
            dns_status = dnsRmAttrValue (
                            ns_name, (dns_attr_name_t *) &nsattr_name, 
                            dnsObj, nsattr_value,
                            nsattr_value_len, (dns_cts *) NULL, &dns_flags);
#endif
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
                rs_dns_rpc_status_from_dnserror (&dns_flags, status);
            }
            break;

        case DNS_NOTFND:
            /*
             * The attribute value was not found or the attribute
             * does not exists.  Return this fact to the caller.
             */
             *status = rpc_s_ok;
             break;

        default:
            /* 
             * Error occurred on dnsTestAttrValue.  It may be due
             * to the entry not being found, in which rpc_s_entry_not_found
             * will be returned.
             */
            rs_dns_rpc_status_from_dnserror (&dns_flags, status);
            break;

    }

    return;
}

/*
**++
**  ROUTINE NAME:       rs_dns_attr_read_value
**
**  SCOPE:              PUBLIC
**
**  DESCRIPTION:
**
**  Cloned from rpc/runtime/nsattr.c/rpc__nsattr_read_value
**
**  Reads a value from a name service attribute.
**
**  INPUTS:
**
**      ns_name             Name service representation of an entry name.
**      attr_name           Name of attribute.
**
**  INPUT/OUTPUTS:          none
**
**  OUTPUTS:
**
**      nsattr_value        Pointer to NS representation of the attribute value.
**                          Pointer must be freed by caller.
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

PUBLIC void rs_dns_attr_read_value 
#ifndef __STDC__
    (ns_name, attr_name, nsattr_value, nsattr_value_len, status)
    unsigned_char_p_t       ns_name;
    unsigned_char_p_t       attr_name;
    unsigned_char_p_t       *nsattr_value;
    unsigned32              *nsattr_value_len;
    unsigned32              *status;
#else
  (
    unsigned_char_p_t       ns_name,
    unsigned_char_p_t       attr_name,
    unsigned_char_p_t       *nsattr_value,
    unsigned32              *nsattr_value_len,
    unsigned32              *status
  )
#endif

{
    int                 dns_status;
    rs_dnsattr_t        nsattr_name;    /* attribute in ns representation */
    struct dnsFlagStat  dns_flags;
    dns_handle          dns_context;

#ifndef DNSPI_V2
                       /* V3 DNSPI attribute value structure */ 
    dns_attrvalue_t    local_nsattr_value, *local_nsattr_value_p; 
#endif                    

    if (strlen((char *) attr_name) > rs_dns_c_attr_name_max) {
        *status = rpc_s_string_too_long;
        return;
    }
    strcpy((char *) nsattr_name.name, (char *) attr_name);

#ifndef DNSPI_V2

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

#endif    

    /*
     * First initialize dns flags. Use 
     * rs_dns_c_bypass_cache for the
     * expiration_age to indicate 
     * we should bypass the cache.
     */

    rs_dns_init_dns_flags (&dns_flags, rs_dns_c_bypass_cache);

    dns_context = dnsGetHandle();
    if (dns_context == (dns_handle) NULL)
    {
        *status = rpc_s_no_memory;
        return;
    }

     /*************** comment out
     *
     * Check the entry's class version first.
     *
    rpc__ns_check_rpc_class_version (
                            inquiry_context->nsentry,
                            inquiry_context->common.exp_age, status );
    
    if (*status != rpc_s_ok)
    {
        dnsFreeHandle(dns_context);
        return;
    }
    **********************/

    /*
     * Read the attribute value using the dns_handle, ns_name,
     * and nsattr_name.
     */


    /*
     * When using the DNSPI V3 interface, use local_nsattr_value
     * and the V3 dnsReadAttrValue() otherwise, use nsattr_value and nsattr_value_len
     * and the V2 dnsReadAttrValue()
     */

#ifndef DNSPI_V2
    
    dns_status = dnsReadAttrValue (
                    (dns_full_name_t *) ns_name, 
                    dns_context, 
                    (dns_attr_name_t *)&nsattr_name, 
                    dnsDirOrObj, (dns_full_name_t *) NULL,
                    &local_nsattr_value_p, (dns_cts **) NULL,
                    (unsigned char *) NULL, (int *) NULL, &dns_flags);
#else

    dns_status = dnsReadAttrValue (
                    ns_name, 
                    dns_context, 
                    (dns_attr_name_t *) &nsattr_name, 
                    dnsObj, (dns_full_name_t *) NULL,
                    nsattr_value, (int *) nsattr_value_len, (dns_cts **) NULL,
                    (unsigned char *) NULL, (int *) NULL, &dns_flags);
#endif

    switch (dns_status)
    {
        case DNS_SUCCESS:
        {    
            *status = rpc_s_ok;
        
#ifndef DNSPI_V2
        
            /*
             * If using DNSPI_V3 interface, copy values in local_nsattr_value
             * structure to back original variables.
             */
             
            *nsattr_value_len = (unsigned32)(local_nsattr_value_p->val_u.val_byte.length);
            if(*nsattr_value_len) {
                if(!(*nsattr_value = (unsigned_char_p_t)malloc(*nsattr_value_len))) {
                    *status = sec_s_no_memory;
                } else {
                    bcopy(local_nsattr_value_p->val_u.val_byte.byte_p, *nsattr_value, *nsattr_value_len);
                }
            } else {
                *nsattr_value = (unsigned_char_p_t)NULL;
            }

#endif
            break;
        }
        case DNS_EMPTY:
            *status = rpc_s_no_more_members;
            break;

        default:
            rs_dns_rpc_status_from_dnserror (&dns_flags, status);
            break;
    }

    dnsFreeHandle(dns_context);
    return;
}


/*
**++
**  ROUTINE NAME:       rs_dns_init_dns_flags
**
**  SCOPE:              PRIVATE - declared in nsutil.h
**
**  DESCRIPTION:
**
**  Cloned from rpc/runtime/nsutil.c/rpc__ns_init_dns_flags
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

PRIVATE void rs_dns_init_dns_flags 
#ifndef __STDC__
    (dns_flags, exp_age)
    struct dnsFlagStat      *dns_flags;
    unsigned32              exp_age;
#else
  (
    struct dnsFlagStat      *dns_flags,
    unsigned32              exp_age
  )
#endif

{
    unsigned32               status;
        

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
    if (exp_age == rs_dns_c_bypass_cache)
    {
        dns_flags->fsConf = dnsConfMed;
    }
    else
    {
        dns_flags->fsConf = dnsConfLow;
        dns_flags->fsCacheTimeout = exp_age;
    }

    /* 
     * If using CDS set fsNameType to use the DCE strings
     * for API name arguments.
     *
     * Also, set the authentication fields to the proper setting:
     * if our global rpc_g_ns_authn is true, then set fsUnAuth and
     * fsTrustAll to zero, otherwise set them to one.
     */
     
#ifndef DNSPI_V2
    /******** Comment out.  
     * Assume that security server is always making authenticated 
     * calls

    dns_flags->fsNameType = dnsStrDCE;

    RPC_GET_THREAD_CONTEXT(thread_context, &status);
    if (status == rpc_s_ok)
    {
        if (! thread_context->ns_authn_state)
        {
            dns_flags->fsUnAuth = 1;
            dns_flags->fsTrustAll = 1;
        }
    }  

    ******* end comment out */
#endif

}

/*
**++
**  ROUTINE NAME:	rs_dns_rpc_status_from_dnserror
**
**  SCOPE:              PRIVATE - declared in nsutil.h
**
**  DESCRIPTION:
** 
**  Cloned from rpc/runtime/nsutil.c/rpc__ns_status_from_nserror
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

PRIVATE void rs_dns_rpc_status_from_dnserror 
#ifndef __STDC__
    (dns_flags, rpc_status)
    struct dnsFlagStat      *dns_flags;
    unsigned32              *rpc_status;
#else
  (
    struct dnsFlagStat      *dns_flags,
    unsigned32              *rpc_status
  )
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
            
#ifndef DNSPI_V2
        case DNS_INVALIDNAME:
#else
        case DNS_INVALIDNAME:
        case DNS_INVALID_NSNAME:
#endif
            *rpc_status = rpc_s_incomplete_name;
            break;

        case DNS_ENTRYEXISTS:
            *rpc_status = rpc_s_entry_already_exists;
            break;

        case DNS_UNKNOWNENTRY:
            *rpc_status = rpc_s_entry_not_found;
            break;

        case DNS_NONSRESOURCES:
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
