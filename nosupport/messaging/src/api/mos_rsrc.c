/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 *  m o s _ r s r c . c
 *
 *  Client interfaces for API control and Resource Management to support the 
 *  Message Oriented Service Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 *
 * $Log: mos_rsrc.c,v $
 * Revision 1.9  1996/12/02  16:16:34  salamone
 * Add mos_rsrc_name_get() API
 *
 * Revision 1.8  96/11/20  14:48:23  14:48:23  salamone (Julie Salamone)
 * Fix header file declarations
 * 
 * Revision 1.7  96/10/29  15:08:25  15:08:25  salamone (Julie Salamone)
 * Added function descriptions
 * 
 * Revision 1.6  96/10/28  12:42:03  12:42:03  salamone (Julie Salamone)
 * Unauthenticated rsrc protectlvl
 * 
 * Revision 1.5  96/10/25  14:42:13  14:42:13  salamone (Julie Salamone)
 * Add rsrc protectlvl support
 * 
 * Revision 1.4  96/10/16  17:50:28  17:50:28  salamone (Julie Salamone)
 * Added mos_s_no_dflt_quemgr_cfg
 * 
 * Revision 1.3  96/10/16  17:05:40  17:05:40  salamone (Julie Salamone)
 * Added resource error codes
 * 
 * Revision 1.2  96/10/15  14:23:23  14:23:23  salamone (Julie Salamone)
 * Clean partial results error code
 * 
 * Revision 1.1  96/10/14  16:56:40  16:56:40  salamone (Julie Salamone)
 * Initial revision
 * 
 * $EndLog$
 * ______________________________________________________________________
 */


/* 
 * INCLUDE FILES  
 */
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dce_svc.h>
#include <dce/nbase.h>
#include <dce/assert.h>
#include <dce/aclif.h>
#include <dce/utc.h>
#include <dce/id_epac.h>

#include <mosif.h>
#include <mos_api.h>
#include <mos_proto.h>
#include <mos_base.h>
#include <mos_q.h>
#include <dcemosmsg.h>
#include <dcemosmac.h>
#include <dcemossvc.h>

/*
 * VARIABLES
 */
mos_rsrc_info_t *rsrc_list_head = NULL;     /* head item in resource list */
mos_rsrc_info_t *rsrc_list_tail = NULL;     /* tail item in resource list */
mos_rsrc_info_t null_rsrc_item = { 0 };     /* NULL resource item */


/*
 *  FUNCTION:
 *  m o s _ r s r c _ a t t a c h
 *
 *  OVERVIEW:
 *  Finds one existing resource by name in the namespace.
 *  A resource is a name in the namespace that may be a queue, 
 *  a Q-mgr, a destination peer application, a specific NSI entry, 
 *  group, or profile name.
 *
 *  INPUTS:
 *  name: 
 *                  Name where the search for resources begins.
 *                  This can be either NULL, a simple, global or
 *                  cell-relative name.
 *                  If NULL or a simple name is specified, the default
 *                  queue manager is used.
 *
 *  security_name:
 *                  Specifies either a principal name or the security group
 *                  name where the named resource is a member. In a message
 *                  queuing model, only a security group name is allowed.
 *                  In a message passing model, a sender knows what recipient
 *                  to trust and only has to specify a principal name, if
 *                  desired.
 *                  If NULL is specified, caller trusts CDS. Not recommended.
 *
 *  OUTPUTS:
 *  resource_handle:
 *                  A context handle to the attached resource.
 *
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_rsrc_attach(
                mos_rsrc_name_t         name,
                sec_rgy_name_t          security_name,
                mos_handle_t *          resource_handle,
                error_status_t *        status
               )
{
    mos_rsrc_info_t *           rsrc_item_p;
    mos_rsrc_info_t *           temp_rsrc_info_p;
    unsigned_char_t *           resolved_name = (unsigned_char_t *)NULL;
    unsigned_char_t *           unresolved_name = (unsigned_char_t *)NULL;
    unsigned_char_t *           expanded_name = (unsigned_char_t *)NULL;
    mos_rsrc_name_t             server_name;
    mos_rsrc_name_t             resource_name;
    error_status_t              temp_status;

    /*
     * Initialize output parameters
     */
    *resource_handle = NULL;
    CLEAR_STATUS(status);
    server_name[0] = '\0';
    resource_name[0] = '\0';

    /*
     * If the specified name is NULL, return a handle 
     * to the default queue manager.
     */
    if ((name == NULL) || (name[0] == '\0')) {
       mos_rsrc_dflt_quemgr_get(security_name,
                                server_name,
                                resource_handle,
                                status);
       return;   /* Return whether good or bad status */
    }


    /*
     * XXX Handle links - use routine in mos_util.c
     */

    /* 
     * Determine the resolved and unresolved components of the name.
     */
    rpc_ns_entry_expand_name(mos_c_dflt_name_syntax,
                             name, 
                             &expanded_name, 
                             status);
    if (BAD_STATUS(status)) {      /* Error due to incomplete name */
       /*
        * If simple name, then use default queue manager.
        * The default queue manager name is the server name.
        * And the simple name is the queue name.
        */
       rpc_string_free(&expanded_name, &temp_status);
       mos_rsrc_dflt_quemgr_get(security_name,
                                server_name,
                                (mos_handle_t *)NULL,
                                status);
       if (GOOD_STATUS(status)) {
          strcpy((char *)resource_name, (char *)name);
       } else {
          return;
       }
    } else {
        /*
         * The resolved name portion is the server name.
         * The unresolved name portion is the queue name.
         */
        rpc_ns_entry_inq_resolution(mos_c_dflt_name_syntax,
                                    expanded_name,
                                    &resolved_name, 
                                    &unresolved_name, 
                                    status);
        if (! STATUS_EQUAL(status, rpc_s_partial_results)) {
           if (BAD_STATUS(status)) {
               rpc_string_free(&expanded_name, &temp_status);
               return;
           }
        } 
        strcpy((char *)server_name, (char *)resolved_name);
        rpc_string_free(&resolved_name, &temp_status);
        strcpy((char *)resource_name, (char *)unresolved_name);
        rpc_string_free(&unresolved_name, &temp_status);
        rpc_string_free(&expanded_name, &temp_status);
    }
    CLEAR_STATUS(status); 

    /*
     * Determine if we are already attached to this specific resource
     * If not, create new resource.
     */
    if (mos__is_rsrc_found(server_name, resource_name, resource_handle)) {
       return;
    }

    /*
     * Allocate space for a new resource
     */
    mos__rsrc_info_create(&rsrc_item_p, status);
    if (BAD_STATUS(status)) {
       return;
    }
    strcpy((char *)rsrc_item_p->server_name, (char *)server_name);
    strcpy((char *)rsrc_item_p->resource_name, (char *)resource_name);

    /*
     * Determine if we already have this server's binding handle
     * If not, then bind.
     */
    if (mos__is_server_found(rsrc_item_p->server_name, &temp_rsrc_info_p)) {
        rsrc_item_p->server_handle = temp_rsrc_info_p->server_handle;
        strcpy((char *)rsrc_item_p->server_prin_name, 
               (char *)temp_rsrc_info_p->server_prin_name);
    } else {
        mos__bind_to_server(security_name, rsrc_item_p, status);
    }

    /*
     * Here we have a new resource so use default protection level.
     * And determine the authentication level to use
     */
    if (GOOD_STATUS(status)) {
        mos_rsrc_protectlvl_set(mos_c_protect_level_default, 
                                rsrc_item_p,
                                status);
    }

    /* 
     * If there is no unresolved name, then this is not a queue. 
     * Otherwise, talk to the server to see if queue exists.
     */
    if ((GOOD_STATUS(status)) &&
        (rsrc_item_p->resource_name[0] != '\0')) {
        mos_q_ping(rsrc_item_p->server_handle,
                   rsrc_item_p->resource_name,
                   &rsrc_item_p->que_id,
                   status);
    } /* End of determine if queue exists. */
    
    /*
     * Add new resource to list of resources and
     * return resource handle to caller 
     */
    if (GOOD_STATUS(status)) {
       mos__rsrc_info_append(rsrc_item_p);
       *resource_handle = (mos_handle_t)rsrc_item_p;
    }

    if (BAD_STATUS(status)) {
       free(rsrc_item_p);
    } 
    return;
     
} /* End of mos_rsrc_attach() routine */


/*
 *  FUNCTION:
 *  m o s _ r s r c _ d e t a c h
 *
 *  OVERVIEW:
 *  Releases one resource and any allocated memory within MOS 
 *  associated with the named resource.
 *
 *  INPUTS/OUTPUTS:
 *  resource_handle:
 *                  A context handle to the attached resource.
 *                  If the detach operation succeeds, this parameter
 *                  returns the value NULL.
 *
 *  OUTPUTS:
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_rsrc_detach(
                mos_handle_t *          resource_handle,
                error_status_t *        status
               )
{

   /*
    * Initialize output parameters
    */
   CLEAR_STATUS(status);

   /*
    * Validate handle
    */
   if ((resource_handle == NULL) || (*resource_handle == NULL)) {
       SET_STATUS(status, mos_s_rsrc_handle_inv);
       return;
   }

   /*
    * Discard resource
    */
   mos__rsrc_info_remove(*resource_handle,
                         status);

   if (GOOD_STATUS(status)) {
       *resource_handle = NULL;
   }
   return;

} /* End of mos_rsrc_detach() routine */


/*
 *  FUNCTION:
 *  m o s _ r s r c _ n a m e _ g e t
 *
 *  OVERVIEW:
 *  Returns the queue manager name, its security group name, its
 *  principal name, and if applicable the queue name itself for the 
 *  specified resource handle.
 *
 *  INPUTS:
 *  resource_handle:
 *                  A context handle to the attached resource.
 *
 *  OUTPUTS:
 *  security_name:
 *                  Security group name where the named resource is a member.
 *                  If NULL, name is not returned.
 *
 *  quemgr_prin_name:
 *                  Principal name of queue manager managing the named 
 *                  resource. If NULL, name is not returned.
 *
 *  quemgr_name:
 *                  Cell-relative name of queue manager managing the named
 *                  resource. If NULL, name is not returned.
 *
 *  que_name:
 *                  Simple name of queue if named resource is a queue.
 *                  If NULL, name is not returned.
 *
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_rsrc_name_get(
                mos_handle_t            resource_handle,
                sec_rgy_name_t          security_name,
                sec_rgy_name_t          quemgr_prin_name,
                mos_rsrc_name_t         quemgr_name,
                mos_rsrc_name_t         que_name,
                error_status_t *        status
               )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate handle
     */
    if (! mos__is_rsrc_handle_valid(FALSE, resource_handle, status)) {
       return;
    }

    if (security_name != NULL) {
       strcpy((char *)security_name, 
              (char *)resource_handle->security_name);
    }
    if (quemgr_prin_name != NULL) {
       strcpy((char *)quemgr_prin_name, 
              (char *)resource_handle->server_prin_name);
    }
    if (quemgr_name != NULL) {
       strcpy((char *)quemgr_name,     
              (char *)resource_handle->server_name);
    }
    if (que_name != NULL) {
       strcpy((char *)que_name,      
              (char *)resource_handle->resource_name);
    }

    return;
     
} /* End of mos_rsrc_name_get() routine */


/*
 *  FUNCTION:
 *  m o s _ r s r c _ d f l t _ q u e m g r _ g e t
 *
 *  OVERVIEW:
 *  Returns the default queue manager used by a MOS application.
 *
 *  The search order is:
 *      1) Determine if caller has previously obtained or
 *         set a default queue manager.
 *
 *      2) Environment variable:
 *         MOS_DFLT_QUEMGR_NAME
 *         This points to the default Q-mgr.
 *
 *      3) Per host profile or hostdata object:
 *         /.:/hosts/$HOSTNAME/msg.profile
 *         This contains a list of Q-mgrs for this host
 *         and their priority.
 *
 *      4) Per cell profile:
 *         /.:/cell-profile
 *         This contains an entry for the default Q-mgr.
 *
 *  INPUTS:
 *  security_name:
 *                  Security group name where the default queue manager
 *                  is a member.
 *                  If NULL is specified, caller trusts CDS. Not recommended.
 *
 *  OUTPUTS:
 *  name:
 *                  Cell-relative name of default queue manager.
 *
 *  resource_handle:
 *                  A context handle to the attached resource.
 *                  If NULL is specified, the resource handle is not
 *                  returned. Otherwise, it is the caller's responsibility
 *                  to release the resource and any allocated memory
 *                  within MOS via mos_rsrc_detach(). For example, the
 *                  caller may already have a resource handle to the
 *                  deafult queue manager.
 *
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_rsrc_dflt_quemgr_get(
                sec_rgy_name_t          security_name,
                mos_rsrc_name_t         name,
                mos_handle_t *          resource_handle,
                error_status_t *        status
               )
{
    mos_rsrc_info_t *    temp_rsrc_info_p;
    unsigned_char_t *    server_name;
    unsigned_char_t *    expanded_name = (unsigned_char_t *)NULL;
    error_status_t       temp_status;

    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);
    name[0] = '\0';

    /* 
     * Step 1:
     * Determine if caller has previously obtained or set a 
     * default queue manager.
     */
    if (mos__is_dflt_quemgr_found(&temp_rsrc_info_p)) {
        strcpy((char *)name, (char *)temp_rsrc_info_p->server_name);
        if (resource_handle)
            *resource_handle = (mos_handle_t)temp_rsrc_info_p;
        return;
    }

    /*
     * Step 2:
     * Check environment variable, MOS_DFLT_QUEMGR_NAME, that points 
     * to the default Q-mgr.
     */
    server_name = (unsigned_char_p_t)getenv("MOS_DFLT_QUEMGR_NAME");

    /*
     * XXX Step 3:
     * Check the per host profile or hostdata object:
     *         /.:/hosts/$HOSTNAME/msg.profile
     */

    /*
     * XXX Step 4:
     * Check the per cell profile: /.:/cell-profile
     */

    /*
     * Validate server name 
     */
    if (server_name == NULL) {
        SET_STATUS(status, mos_s_no_dflt_quemgr_cfg);  
        return;
    } 
    rpc_ns_entry_expand_name(mos_c_dflt_name_syntax,
                             server_name, 
                             &expanded_name, 
                             status);
    if (GOOD_STATUS(status)) {     
       /*
        * Return default queue manager name
        */
       strcpy((char *)name, (char *)expanded_name);

       /*
        * If requested, return default queue manager handle
        */
       if (resource_handle) {
          mos_rsrc_attach(server_name,
                          security_name,
                          resource_handle,
                          status);
          if (GOOD_STATUS(status)) {
             (*resource_handle)->dflt_que_mgr = TRUE;
          }
       } 
    }

    rpc_string_free(&expanded_name, &temp_status);
    return;
     
} /* End of mos_rsrc_dflt_quemgr_get() routine */


/*
 *  FUNCTION:
 *  m o s _ r s r c _ d f l t _ q u e m g r _ s e t
 *
 *  OVERVIEW:
 *  Sets the default queue manager used by a MOS application.
 *
 *  INPUTS:
 *  name:
 *                  Cell-relative name of default queue manager.
 *
 *  security_name:
 *                  Security group name where the default queue manager
 *                  is a member.
 *                  If NULL is specified, caller trusts CDS. Not recommended.
 *
 *  OUTPUTS:
 *  resource_handle:
 *                  A context handle to the attached resource.
 *                  If NULL is specified, the resource handle is not
 *                  returned. Otherwise, it is the caller's responsibility
 *                  to release the resource and any allocated memory
 *                  within MOS via mos_rsrc_detach(). For example, the
 *                  caller may already have a resource handle to the
 *                  deafult queue manager.
 *
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_rsrc_dflt_quemgr_set(
                mos_rsrc_name_t         name,
                sec_rgy_name_t          security_name,
                mos_handle_t *          resource_handle,
                error_status_t *        status
               )
{
    mos_rsrc_info_t *    temp_rsrc_info_p;

    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate specified resource name
     */
    if ((name == NULL) || (name[0] == '\0')) {
        SET_STATUS(status, mos_s_rsrcname_null);  
        return;
    }

    /*
     * Attach to new default queue manager
     */
    mos_rsrc_attach(name,
                    security_name,
                    resource_handle,   
                    status);
    if (BAD_STATUS(status)) {
       return;
    }

    /* 
     * Determine if caller has previously obtained or set a 
     * default queue manager.
     */
    if (mos__is_dflt_quemgr_found(&temp_rsrc_info_p)) {
        temp_rsrc_info_p->dflt_que_mgr = FALSE;
    }

    /*
     * Mark newly attached resource as the new default queue manager
     */
    (*resource_handle)->dflt_que_mgr = TRUE;
    return;

} /* End of mos_rsrc_dflt_quemgr_set() routine */


/*
 *  FUNCTION:
 *  m o s _ r s r c _ p r o t e c t l v l _ g e t 
 *
 *  OVERVIEW:
 *  Returns the protection level for communications made to a resource.
 *  This is not to be confused with end-to-end protection levels.
 *
 *  INPUTS:
 *  resource_handle:
 *                  A context handle to the attached resource.
 *
 *  OUTPUTS:
 *  protect_level:
 *                  Specifies how much of the information in messages
 *                  exchanged by principals is encrypted. Refer to
 *                  Section 2.2 for the list of valid protection level
 *                  constants for authenticated communications from
 *                  sender to attached resource.
 *
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_rsrc_protectlvl_get(
                        mos_handle_t            resource_handle,
                        unsigned32 *            protect_level,
                        error_status_t *        status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate handle
     */
    if (! mos__is_rsrc_handle_valid(FALSE, resource_handle, status)) {
       return;
    }

    *protect_level = resource_handle->protectlvl;

    return;

} /* End of mos_rsrc_protectlvl_get() routine */


/*
 *  FUNCTION:
 *  m o s _ r s r c _ p r o t e c t l v l _ s e t 
 *
 *  OVERVIEW:
 *  Sets and registers the protection level for communications made 
 *  to a resource.
 *  This is not to be confused with end-to-end protection levels.
 *
 *  INPUTS:
 *  protect_level:
 *                  Specifies how much of the information in messages
 *                  exchanged by principals is encrypted. Refer to
 *                  Section 2.2 for the list of valid protection level
 *                  constants for authenticated communications from
 *                  sender to attached resource.
 *
 *  INPUTS/OUTPUTS:
 *  resource_handle:
 *                  A context handle to the attached resource.
 *
 *  OUTPUTS:
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_rsrc_protectlvl_set(
                        unsigned32              protect_level,
                        mos_handle_t            resource_handle,
                        error_status_t *        status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate handle
     */
    if (! mos__is_rsrc_handle_valid(FALSE, resource_handle, status)) {
       return;
    }

    /*
     * Validate protection level
     */
    if (! mos__is_protectlvl_valid(protect_level, status)) {
       return;
    }

    /*
     * Determine the authentication level to use
     */
    mos__auth_info_set(resource_handle, protect_level, status);
    if (GOOD_STATUS(status)) {
        /*
         * Register binding's protection level with API.
         * Since "mos_c_protect_level_default" translates to other 
         * protection level values, obtain true protection level.
         */
        rpc_binding_inq_auth_info(resource_handle->server_handle,
                                  NULL,
                                  &resource_handle->protectlvl,
                                  NULL,
                                  NULL,
                                  NULL,
                                  status);
        if (STATUS_EQUAL(status, rpc_s_binding_has_no_auth)) {
           CLEAR_STATUS(status);
        }
    }
    return;

} /* End of mos_rsrc_protectlvl_set() routine */


/*
 *  FUNCTION:
 *  m o s _ _ b i n d _ t o _ s e r v e r 
 *
 *  OVERVIEW:
 *  Obtains binding handle to server via the name service. If security
 *  name is not NULL, the server's principal name is validated with the 
 *  security registry.
 *
 *  INPUTS:
 *  security_name:
 *                  Security group name where the default queue manager
 *                  is a member.
 *                  If NULL is specified, caller trusts CDS. Not recommended.
 *
 *  OUTPUTS:
 *  rsrc_item_p:
 *                  A context handle to the attached resource.
 *
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos__bind_to_server(
                     sec_rgy_name_t       security_name,
                     mos_rsrc_info_t *    rsrc_item_p,
                     error_status_t *     status
                    )
{
    rpc_ns_handle_t             context;
    unsigned_char_t *           server_prin;
    error_status_t              temp_status;

    /*
     * Initialize output 
     */
    CLEAR_STATUS(status);

    /* 
     * Using the resolved name, obtain partial binding handle from CDS.
     */
    rpc_ns_binding_import_begin(mos_c_dflt_name_syntax,
                                rsrc_item_p->server_name,
                                NULL,
                                NULL,
                                &context,
                                status);

    if (BAD_STATUS(status)) {
       return;
    } 

    for (;;) {
        rpc_ns_binding_import_next(context,
                                   &rsrc_item_p->server_handle,
                                   status);
        if (BAD_STATUS(status)) {
            rpc_ns_binding_import_done(&context, &temp_status);
            return;
        }

        /*
         * Resolve partial binding handle
         */
        rpc_ep_resolve_binding(rsrc_item_p->server_handle,
                               mos_q_v1_0_c_ifspec,
                               status);
        if (GOOD_STATUS(status)) {
            rpc_ns_binding_import_done(&context, &temp_status);
            break;
        }
        /*
         * Free binding handle resource NOT used as a fully
         * bound server handle.
         */
         rpc_binding_free(&rsrc_item_p->server_handle,
                          &temp_status);

     } /* End of loop until fully bound server handle is obtained */


    if (BAD_STATUS(status)) {
       return;
    }

    /*
     * If the message queuing model, authentication and
     * authorization information bewteen clients and Q-mgrs
     * are set implicitly due to ACLs on queues.
     */
    rpc_mgmt_inq_server_princ_name(rsrc_item_p->server_handle,
                                   rpc_c_authn_dce_secret,
                                   &server_prin,
                                   status);
    if (BAD_STATUS(status)) {
       return;
    } else {
       strcpy((char *)rsrc_item_p->server_prin_name, (char *)server_prin);
       rpc_string_free(&server_prin, &temp_status);
    }

    /* 
     * If "security_name" is NULL, caller trusts CDS. Not recommended.
     * Otherwise, validate server with the security registry.
     */
    if (! ((security_name == NULL) || (security_name[0] == '\0'))) {
       if (! mos_is_principal_valid(rsrc_item_p->server_prin_name, 
                                    security_name, 
                                    status)){
           return;
       }
    }

    return;

} /* End of mos__bind_to_server() routine */
   

/*
 *  FUNCTION:
 *  m o s _ _ a u t h _ i n f o _ s e t 
 *
 *  OVERVIEW:
 *  This routine ensures that the proper authentication information is
 *  set on the server's binding. Note that protection levels may vary.
 *
 *  INPUTS:
 *  rsrc_item_p:
 *                  A context handle to the attached resource.
 *
 *  new_protectlvl:
 *                  Specifies how much of the information in messages
 *                  exchanged by principals is encrypted. Refer to
 *                  Section 2.2 for the list of valid protection level
 *                  constants for authenticated communications from
 *                  sender to attached resource.
 *
 *  OUTPUTS:
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos__auth_info_set(
                   mos_rsrc_info_t *    rsrc_item_p,
                   unsigned32           new_protectlvl,
                   error_status_t *     status
                    )
{
    unsigned32                    current_protectlvl;

    CLEAR_STATUS(status);

    /*
     * Obtain the current protection level from the binding.
     * If binding has not been authenticated yet or the protection 
     * level comparsion fails, then set auth info again.
     */
    rpc_binding_inq_auth_info(rsrc_item_p->server_handle,
                              NULL,
                              &current_protectlvl,
                              NULL,
                              NULL,
                              NULL,
                              status);
    if (! STATUS_EQUAL(status, rpc_s_binding_has_no_auth)) {
       if (BAD_STATUS(status)) {
          return;
       }
    } 
    if ((GOOD_STATUS(status)) &&
        (current_protectlvl == new_protectlvl)) {
        return;
    }

    /*
     * Determine the authentication level to use
     */
    if (mos_is_logged_in()) {
        rpc_binding_set_auth_info(rsrc_item_p->server_handle,
                                  rsrc_item_p->server_prin_name,
                                  new_protectlvl,
                                  mos_c_authn_dce_secret,
                                  NULL,
                                  mos_c_authz_dce,
                                  status);
    } else {
        rpc_binding_set_auth_info(rsrc_item_p->server_handle,
                                  NULL,
                                  new_protectlvl,
                                  mos_c_authn_none,
                                  NULL,
                                  mos_c_authz_none,
                                  status);
    }

    return;

} /* End of mos__auth_info_set() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ r s r c _ f o u n d
 *
 *  OVERVIEW:
 *  Based on the server name and the resource name, determine if caller
 *  is already attached to this specific resource. If so, return the 
 *  handle. Otherwise, return a NULL handle.
 *
 *  INPUTS:
 *  server_name: 
 *                  Server NSI name.
 *
 *  resource_name:
 *                  Queue name. Could be NULL.
 *
 *  OUTPUTS:
 *  resource_handle:
 *                  A context handle to the attached resource.
 *
 *
 *  RETURNS:        TRUE  indicates resource was found.
 *                  FALSE indicates resource was not found.
 */
boolean
mos__is_rsrc_found(
                     mos_rsrc_name_t      server_name,
                     mos_rsrc_name_t      resource_name,
                     mos_rsrc_info_t **   resource_handle
                    )
{
   mos_rsrc_info_t *     rsrc_item_p;
   
   for (rsrc_item_p = rsrc_list_head; 
        rsrc_item_p; 
        rsrc_item_p = rsrc_item_p->next) {

     if ((! strcmp((char *)rsrc_item_p->server_name, 
                   (char *)server_name)) 
         &&
         (! strcmp((char *)rsrc_item_p->resource_name, 
                   (char *)resource_name))) {
        *resource_handle = rsrc_item_p;
        return(TRUE);
     } 
   } /* End of loop thru all resources */

   *resource_handle = NULL;
   return(FALSE);

} /* End of mos__is_rsrc_found() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ s e r v e r _ f o u n d
 *
 *  OVERVIEW:
 *  Based on the server name only, determine if caller is already attached 
 *  to this server. If so, return the handle. Otherwise, return a NULL handle.
 *
 *  INPUTS:
 *  server_name: 
 *                  Server NSI name.
 *
 *  OUTPUTS:
 *  server_rsrc_info:
 *                  A context handle to the attached resource.
 *
 *
 *  RETURNS:        TRUE  indicates server was found.
 *                  FALSE indicates server was not found.
 */
boolean
mos__is_server_found(
                     mos_rsrc_name_t      server_name,
                     mos_rsrc_info_t **   server_rsrc_info
                    )
{
   mos_rsrc_info_t *     rsrc_item_p;
   
   for (rsrc_item_p = rsrc_list_head; 
        rsrc_item_p; 
        rsrc_item_p = rsrc_item_p->next) {

     if ((strcmp((char *)rsrc_item_p->server_name, (char *)server_name)) == 0) {
        *server_rsrc_info = rsrc_item_p;
        return(TRUE);
     } 
   } /* End of loop thru all resources */

   *server_rsrc_info = NULL;
   return(FALSE);

} /* End of mos__is_server_found() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ d f l t _ q u e m g r _ f o u n d
 *
 *  OVERVIEW:
 *  Determine if caller is already attached to the default queue manager.
 *  If so, return the handle. Otherwise, return a NULL handle.
 *
 *  OUTPUTS:
 *  dflt_qmgr_info:
 *                  A context handle to the attached resource.
 *
 *
 *  RETURNS:        TRUE  indicates default queue manager was found.
 *                  FALSE indicates default queue manager was not found.
 */
boolean
mos__is_dflt_quemgr_found(
                          mos_rsrc_info_t **    dflt_qmgr_info
                         )
{
   mos_rsrc_info_t *     rsrc_item_p;
   
   for (rsrc_item_p = rsrc_list_head; 
        rsrc_item_p; 
        rsrc_item_p = rsrc_item_p->next) {

       if (rsrc_item_p->dflt_que_mgr) {
           *dflt_qmgr_info = rsrc_item_p;
           return(TRUE);
       } 
   } /* End of loop thru all resources */

   *dflt_qmgr_info = NULL;
   return(FALSE);

} /* End of mos__is_dflt_quemgr_found() routine */


/*
 *  FUNCTION:
 *  m o s _ _ s e r v e r _ r e f c n t _ g e t
 *
 *  OVERVIEW:
 *  Return the number of attached resources that use the same binding 
 *  handle.
 *
 *  INPUTS:
 *  server_name: 
 *                  Server NSI name.
 *
 *  OUTPUTS:
 *  references:
 *                  Number of attached resources that use the same binding
 *                  handle.
 *
 *  RETURNS:        None
 */
void
mos__server_refcnt_get(
                       mos_rsrc_name_t      server_name,
                       unsigned32 *         references
                      )
{
   mos_rsrc_info_t *     rsrc_item_p;
   
   *references = 0;

   for (rsrc_item_p = rsrc_list_head; 
        rsrc_item_p; 
        rsrc_item_p = rsrc_item_p->next) {

     if ((strcmp((char *)rsrc_item_p->server_name, (char *)server_name)) == 0) {
         (*references)++;
     } 
   } /* End of loop thru all resources */

} /* End of mos__server_refcnt_get() routine */


/*
 *  FUNCTION:
 *  m o s _ _ r s r c _ i n f o _ c r e a t e
 *
 *  OVERVIEW:
 *  Create and initialize a new resource item.
 *
 *  OUTPUTS:
 *  resource_item_p:
 *                  Pointer to a resource information structure.
 *
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos__rsrc_info_create(
                      mos_rsrc_info_t **  resource_item_p,
                      error_status_t *    status
                     )
{
    mos_rsrc_info_t *  rsrc_item_p;

    /*
     * Intialize output parameters
     */
    *resource_item_p = NULL;
    CLEAR_STATUS(status);

    /*
     * Allocate new resource
     */
    rsrc_item_p = (mos_rsrc_info_t *)malloc(sizeof(mos_rsrc_info_t));
    if (rsrc_item_p == NULL) {
       SET_STATUS(status, mos_s_no_memory);    
       return;
    }

    /*
     * Initialize resource
     */
    *rsrc_item_p = null_rsrc_item;   
    
    *resource_item_p = rsrc_item_p;
    return;

} /* End of mos__rsrc_info_create() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ r s r c _ h a n d l e _ v a l i d
 *
 *  OVERVIEW:
 *  Validate a resource handle. Some APIs require a queue name only and
 *  other APIs accept either server or queue names.
 *
 *  INPUTS:
 *  is_rsrc_queue:
 *                  If TRUE, resource must be a queue.
 *
 *  resource_item_p:
 *                  A context handle to the attached resource.
 *
 *  OUTPUTS:
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        
 *                  TRUE  indicates validation was successful.
 *                  FALSE indicates validation was not successful.
 */
boolean
mos__is_rsrc_handle_valid(
                          boolean             is_rsrc_queue,
                          mos_rsrc_info_t *   resource_item_p,
                          error_status_t *    status
                         )
{
   CLEAR_STATUS(status);

   /*
    * Good handle?
    */
   if (resource_item_p == NULL){
       SET_STATUS(status, mos_s_rsrc_handle_inv);  
       return(FALSE);
   } 

   /*
    * Is resource a queue?
    */
   if (is_rsrc_queue) {
       if (uuid_is_nil(&resource_item_p->que_id, status)) {
           SET_STATUS(status, mos_s_rsrc_not_queue);  
           return(FALSE);
       }
   }

   return(TRUE);

} /* End of mos__is_rsrc_info_valid() routine */


/*
 *  FUNCTION:
 *  m o s _ _ r s r c _ i n f o _ a p p e n d
 *
 *  OVERVIEW:
 *  Add a newly attached resource at the tail of the list of 
 *  attached resources.
 *
 *  INPUTS:
 *  resource_item_p:
 *                  A context handle to the attached resource.
 *
 *  RETURNS:        None
 */
void
mos__rsrc_info_append(
                      mos_rsrc_info_t *   resource_item_p
                     )
{
   /*
    * Append to list
    */
   if (rsrc_list_head == NULL) {
       rsrc_list_head = rsrc_list_tail = resource_item_p;
   } else {
       rsrc_list_tail->next = resource_item_p;
       rsrc_list_tail = resource_item_p;
   }

} /* End of mos__rsrc_info_append() routine */


/*
 *  FUNCTION:
 *  m o s _ _ r s r c _ i n f o _ r e m o v e
 *
 *  OVERVIEW:
 *  Find the resource in the list of attached resources and remove it. 
 *
 *  INPUTS:
 *  resource_item_p:
 *                  A context handle to the attached resource.
 *
 *  OUTPUTS:
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos__rsrc_info_remove(
                      mos_rsrc_info_t *   remove_item_p,
                      error_status_t *    status
                     )
{
   mos_rsrc_info_t *     rsrc_item_p;
   mos_rsrc_info_t *     prev_item_p;
   unsigned32            ref_cnt;
   
   /*
    * Initialize output 
    */
   CLEAR_STATUS(status);

   /*
    * Determine if server or queue exists
    */
   prev_item_p = NULL;
   for (rsrc_item_p = rsrc_list_head; 
        rsrc_item_p; 
        rsrc_item_p = rsrc_item_p->next) {

     if ((! strcmp((char *)rsrc_item_p->server_name, 
                   (char *)remove_item_p->server_name))  &&
         (uuid_equal(&rsrc_item_p->que_id, &remove_item_p->que_id, status))) {
        break;
     }
     prev_item_p = rsrc_item_p;    /* Save previous resource item */

   } /* End of loop thru all resources */

   if (rsrc_item_p == NULL) {
       SET_STATUS(status, mos_s_no_rsrc_attached);  
       return;
   }

   /*
    * Remove item from list
    */
   if (rsrc_item_p == rsrc_list_head)
       rsrc_list_head = rsrc_item_p->next;
   if (prev_item_p) 
       prev_item_p->next = rsrc_item_p->next;
   if (rsrc_item_p == rsrc_list_tail)
       rsrc_list_tail = prev_item_p;

   /*
    * Note that a binding handle may be shared amongst several resources.
    * If not used by other resources, release binding handle
    */
   mos__server_refcnt_get(rsrc_item_p->server_name, &ref_cnt);
   if (ref_cnt == 0) {
       rpc_binding_free(&rsrc_item_p->server_handle, status);
   } 

   /*
    * Release memory for the resource
    */
   free((void *)rsrc_item_p);
   return;

} /* End of mos__rsrc_info_remove() routine */

