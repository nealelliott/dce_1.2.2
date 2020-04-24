/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsprofile.c,v $
 * Revision 1.1.531.2  1996/02/18  00:05:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:14  marty]
 *
 * Revision 1.1.531.1  1995/12/08  00:21:38  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:34 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/25  21:41 UTC  lmm
 * 	Merge allocation failure detection cleanup work
 * 
 * 	HP revision /main/HPDCE02/lmm_alloc_fail_clnup/1  1995/05/25  21:02 UTC  lmm
 * 	allocation failure detection cleanup
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/02  01:16 UTC  lmm
 * 	Merge allocation failure detection functionality into Mothra
 * 
 * 	HP revision /main/lmm_rpc_alloc_fail_detect/1  1995/04/02  00:20 UTC  lmm
 * 	add memory allocation failure detection
 * 	[1995/12/08  00:00:11  root]
 * 
 * Revision 1.1.529.2  1994/07/29  16:19:25  tom
 * 	Add entry type to rpc__nsentry_create arguments. (OT 7945)
 * 	[1994/07/28  22:34:28  tom]
 * 
 * Revision 1.1.529.1  1994/01/21  22:38:45  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:24  cbrooks]
 * 
 * Revision 1.1.4.7  1993/01/08  21:47:40  weisman
 * 	Replace Grace Hsiao changes.
 * 	[1993/01/07  21:44:31  weisman]
 * 
 * Revision 1.1.4.3  1992/11/05  16:08:55  grace
 * 	    02-nov-92    ebm     In rpc_ns_profile_elt_remove(), return
 * 	                         rpc_s_profile_element_not_found when
 * 	                         rpc_s_no_more_elements or rpc_s_not_rpc_entry.
 * 	                         This augments bug fix 453.
 * 	    28-sep-92    gh      Remove rpc__ns_check_name_syntax() -- checked in
 * 	                         RPC_RESOLVE_SYNTAX macro.
 * 	    24-sep-92    dm      - Modify rpc_ns_profile_elt_remove and
 * 	                           rpc_ns_profile_elt_add to recognize the default
 * 	                           profile by either a NULL if_id or a nil if uuid.
 * 	                         - Modify rpc_ns_profile_elt_remove to properly
 * 	                           remove the default element.
 * 	                         - Modify rpc_ns_profile_elt_add to insure the
 * 	                           default profile's priority is 0.
 * 	                         - Modify rpc_ns_profile_elt_remove to return
 * 	                           rpc_s_profile_element_not_found only when
 * 	                           rpc_s_no_more_elements. Otherwise return the
 * 	                           status unchanged.
 * 	[1992/11/05  15:24:59  grace]
 * 
 * Revision 1.1.4.2  1992/10/13  20:57:04  weisman
 * 	    03-aug-92    gh      Fix memory leaks.
 * 	[1992/10/13  20:48:43  weisman]
 * 
 * Revision 1.1.2.2  1992/05/01  16:39:20  rsalz
 * 	  1-apr-92 ko        convert rpc__nsattr_remove_value of
 * 	                     rpc_s_not_found to rpc_s_profile_element_not_found
 * 	[1992/05/01  16:30:52  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:12:53  devrcs
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
**  NAME
**
**      nsprofile.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  RPC Name Service routines to create and manage profiles.
**
**
*/

#include <commonp.h>    /* Common internals for RPC Runtime system  */
#include <com.h>        /* Externals for Common Services component  */
#include <ns.h>         /* Externals for Name Services component    */
#include <nsp.h>        /* Internals for Name Services component    */
#include <nsentry.h>    /* Externals for NS Entry sub-component     */
#include <nsattr.h>     /* Externals for NS Attribute sub-component */
#include <nsuuid.h>     /* Externals for NS UUID sub-component      */
#include <nsutil.h>     /* Externals for NS Utility sub-component   */



/*
 * prototype declarations for locally defined routines
 */

INTERNAL void nsprofile_elt_free _DCE_PROTOTYPE_ ((
        rpc_nsprofile_elt_p_t   * /*nsprofile_elt*/,
        unsigned32              * /*status*/
    ));

INTERNAL rpc_nsprofile_elt_t *nsprofile_elt_from_profile_elt _DCE_PROTOTYPE_ ((
        unsigned32               /*member_name_syntax*/,
        unsigned_char_p_t        /*member_name*/,
        rpc_if_id_p_t            /*if_id*/,
        unsigned32               /*priority*/,
        unsigned_char_p_t        /*annotation*/,
        unsigned32              * /*status*/
    ));

INTERNAL void nsprofile_elt_to_profile_elt _DCE_PROTOTYPE_ ((
        unsigned32               /*member_name_syntax*/,
        rpc_nsprofile_elt_p_t    /*nsprofile_elt*/,
        rpc_profile_elt_t       * /*profile_elt*/,
        unsigned32              * /*status*/
    ));                    



/*
**++
**
**  ROUTINE NAME:       rpc_ns_profile_delete
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**      
**  Deletes a profile from the name service database.
**
**  INPUTS:
**
**      profile_name_syntax   The name service syntax used for the specified
**                      profile name.
**
**      profile_name    The entry in the namespace containing the profile
**                      attribute.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                      or status from a called routine.
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
**/

PUBLIC void rpc_ns_profile_delete 
#ifdef _DCE_PROTO_
(
    unsigned32              profile_name_syntax,
    unsigned_char_p_t       profile_name,
    unsigned32              *status
)
#else
(profile_name_syntax, profile_name, status)
unsigned32              profile_name_syntax;
unsigned_char_p_t       profile_name;
unsigned32              *status;
#endif
{
    rpc_nsentry_p_t         profile_nsentry;
    unsigned32              new_profile_name_syntax,
                            local_status;    

    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT ();
    
    /*
     * check the syntax that was given
     */
    new_profile_name_syntax = profile_name_syntax;
    RPC_RESOLVE_SYNTAX (new_profile_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }
    
    /*
     * create a name service representation of the profile name
     */
    profile_nsentry = rpc__nsentry_from_entry
        (new_profile_name_syntax, profile_name, status);
    if (*status != rpc_s_ok) return;

    /*
     * remove the profile attribute from this entry
     */
    rpc__nsattr_delete (profile_nsentry, RPC_C_ATTR_PROFILE, status);

    /*
     * free the nsentry that was created
     */
    rpc__nsentry_free (&profile_nsentry, &local_status);

    /*
     * Return the status from rpc__nsattr_delete
     */        
    return;
}


/*
**++
**
**  ROUTINE NAME:       rpc_ns_profile_elt_add
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**      
**  Adds an element to the profile attribute of the specified name service
**  entry.  Before adding a member to a profile, we try to remove
**  the specified profile element.  Otherwise the profile could contain 
**  the same element with different priorities and/or annotation.
**
**  INPUTS:
**
**      profile_name_syntax   The name service syntax used for the specified
**                      profile name.
**
**      profile_name    The entry in the namespace containing the profile
**                      attribute.
**
**      if_id           The interface identification (UUID and version numbers)
**                      to be added to the profile element.  If NULL, the 
**                      default profile element is added.
**
**      member_name_syntax  The name service syntax used for the specified
**                      member name.
**
**      member_name     The member name to be added to the profile element.
**
**      priority        An integer value (from 0-7) that specifies the
**                      relative lookup priority for this member.
**
**      annotation      A text string annotation stored with the interface
**                      entry.  If NULL, "\0" is stored.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_invalid_priority
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
**/

PUBLIC void rpc_ns_profile_elt_add 
#ifdef _DCE_PROTO_
(
    unsigned32              profile_name_syntax,
    unsigned_char_p_t       profile_name,
    rpc_if_id_p_t           if_id,
    unsigned32              member_name_syntax,
    unsigned_char_p_t       member_name,
    unsigned32              priority,
    unsigned_char_p_t       annotation,
    unsigned32              *status
)
#else
(profile_name_syntax, profile_name, if_id, member_name_syntax, member_name, priority, annotation, status)
unsigned32              profile_name_syntax;
unsigned_char_p_t       profile_name;
rpc_if_id_p_t           if_id;
unsigned32              member_name_syntax;
unsigned_char_p_t       member_name;
unsigned32              priority;
unsigned_char_p_t       annotation;
unsigned32              *status;
#endif
{
    rpc_nsentry_p_t         profile_nsentry = NULL;
    rpc_nsprofile_elt_p_t   nsprofile_elt = NULL;
    rpc_if_id_t             local_if_id,
                            *local_if_id_ptr = &local_if_id;
    unsigned_char_t         null_annotation[1];
    unsigned_char_p_t       local_annotation;
    unsigned32              new_profile_name_syntax,
                            new_member_name_syntax,
                            local_status;
    unsigned32              class_version_status;
        

    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT ();
    
    /*
     * check the range on the priority value (it's unsigned, so can't be < 0)
     */
    if (priority > RPC_C_NS_PRIORITY_MAX)
    {
        *status = rpc_s_invalid_priority;
        return;
    }
    
    /*
     * check to see if the default name and syntax are needed
     */
    new_profile_name_syntax = profile_name_syntax;
    RPC_RESOLVE_SYNTAX (new_profile_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    new_member_name_syntax = member_name_syntax;
    RPC_RESOLVE_SYNTAX (new_member_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * If the default profile element is being added
     * create the proper if_id.
     * Otherwise, copy specified if_id into local_if_id.
     */
    if ( (if_id == NULL) || uuid_is_nil (&(if_id->uuid), status) )
    {
        uuid_create_nil (&(local_if_id_ptr->uuid), status);
        local_if_id_ptr->vers_major = 0;
        local_if_id_ptr->vers_minor = 0;

        /*
         * Ignore the provided priority. The default profile element 
         * always has a priority of 0.
         */
        priority = 0;
    }
    else
    {
        *local_if_id_ptr = *if_id;
    }

    /*
     * Copy the annotation pointer to a local pointer if not NULL.
     */
    if (annotation != NULL)
    {
        local_annotation = annotation;
    }
    else
    {
        strcpy ((char *) null_annotation, "");
        local_annotation = null_annotation;
    }

    /*
     * create an nsentry from the profile name
     */
    profile_nsentry = rpc__nsentry_from_entry
        (new_profile_name_syntax, profile_name, status);
    if (*status != rpc_s_ok) 
    {
        return;
    }
    
    /*
     * Check the class version of the profile entry.
     */
    rpc__ns_check_rpc_class_version (profile_nsentry, RPC_C_NS_BYPASS_CACHE,
                                     &class_version_status);
    
    /*
     * If the entry does not exist, create it.
     */
    if (class_version_status == rpc_s_entry_not_found)
    {
        rpc__nsentry_create (profile_nsentry, RPC_C_NS_PROFILE, status);

        if (*status != rpc_s_ok)
        {
            goto CLEANUP;
        }   
    }

    /*
     * See if an element with the specified member name and 
     * interface identification is already in the profile by
     * trying to remove it. 
     *
     * A nil if-uuid in local_if_id_ptr tells the remove call that the
     * default element is desired.
     */
    rpc_ns_profile_elt_remove (new_profile_name_syntax, profile_name,
        local_if_id_ptr, new_member_name_syntax, member_name, status);

    if (!((*status == rpc_s_ok) ||
          (*status == rpc_s_profile_element_not_found) ||
          (*status == rpc_s_profile_not_found)))
    {
        goto CLEANUP;
    }

    /*
     * Create a name service rep of the profile element.
     */
    nsprofile_elt = nsprofile_elt_from_profile_elt (new_member_name_syntax,
        member_name, local_if_id_ptr, (unsigned32) priority, local_annotation, 
	status);

    if (*status != rpc_s_ok) 
    {
        goto CLEANUP;
    }

    /*
     * Add this element to the profile attribute.
     */
    rpc__nsattr_add_value (profile_nsentry, RPC_C_ATTR_PROFILE,
        (unsigned_char_p_t) (nsprofile_elt->octet_str),
        nsprofile_elt->octet_str_length, status);
    if (*status == rpc_s_ok) 
    {
        /* 
         * If the profile element has been successfully
         * added and the profile entry is not an RPC
         * name service entry, then label the entry as
         * an RPC entry.
         */
        if (class_version_status == rpc_s_not_rpc_entry)
        {
            rpc__nsentry_make_it_rpc (profile_nsentry,&local_status);
        }
    }

    /*
     * Free the name service rep of the profile element.
     */
    nsprofile_elt_free (&nsprofile_elt, &local_status);
    
CLEANUP:

    /*
     * Free things and return with status of last meaningful operation.
     */
    if (profile_nsentry != NULL)
    {
        rpc__nsentry_free (&profile_nsentry, &local_status);
    }

    return;
}


/*
**++
**
**  ROUTINE NAME:       rpc_ns_profile_elt_inq_begin
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**      
**  Creates an inquiry context for viewing the elements in a 
**  profile.
**
**  INPUTS:
**
**      profile_name_syntax   The name service syntax used for the specified
**                      profile name.
**
**      profile_name    The entry in the namespace containing the profile
**                      attribute.
**
**      inquiry_type    An integer value specifying the type of inquiry
**                      to perform on the profile.
**
**      if_id           The interface identification of the profile elements
**                      to be returned by rpc_ns_profile_elt_inq_next().
**
**      vers_option     Specifies how rpc_ns_profile_elt_inq_next() uses the
**                      if_id argument.
**
**      member_name_syntax   The name service syntax used for the member_name
**                      and the returned member_name in 
**                      rpc_ns_profile_elt_inq_next.
**
**      member_name     Specifies the member name looked for in profile
**                      elements in rpc_ns_profile_elt_inq_next().
**                      
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      inquiry_context Search context for subsequent inquiry operations.
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_profile_not_found
**                          rpc_s_invalid_inquiry_type
**                          rpc_s_invalid_vers_option
**                          rpc_s_invalid_name_syntax
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
**/

PUBLIC void rpc_ns_profile_elt_inq_begin 
#ifdef _DCE_PROTO_
(
    unsigned32              profile_name_syntax,
    unsigned_char_p_t       profile_name,
    unsigned32              inquiry_type,
    rpc_if_id_p_t           if_id,
    unsigned32              vers_option,
    unsigned32              member_name_syntax,
    unsigned_char_p_t       member_name,
    rpc_ns_handle_t         *inquiry_context,
    unsigned32              *status
)
#else
(profile_name_syntax, profile_name, inquiry_type, if_id, vers_option, 
 member_name_syntax, member_name, inquiry_context, status)
unsigned32              profile_name_syntax;
unsigned_char_p_t       profile_name;
unsigned32              inquiry_type;
rpc_if_id_p_t           if_id;
unsigned32              vers_option;
unsigned32              member_name_syntax;
unsigned_char_p_t       member_name;
rpc_ns_handle_t         *inquiry_context;
unsigned32              *status;
#endif
{
    unsigned_char_t         expanded_member_name [RPC_C_ENTRY_NAME_MAX];
    rpc_ns_inq_rep_p_t      inquiry_context_rep;
    unsigned32              new_profile_name_syntax,
                            new_member_name_syntax,
                            name_length;
    rpc_nsentry_p_t         profile_nsentry;
    unsigned32              local_status;


    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT ();

    /*
     * presume failure
     */
    *inquiry_context = NULL;

    /*
     * Check on validity of inquiry_type.
     */
    if (!((inquiry_type == rpc_c_profile_default_elt) ||
          (inquiry_type == rpc_c_profile_all_elts) ||
          (inquiry_type == rpc_c_profile_match_by_if)  ||
          (inquiry_type == rpc_c_profile_match_by_mbr) ||
          (inquiry_type == rpc_c_profile_match_by_both)))
    {
        *status = rpc_s_invalid_inquiry_type;
        return;
    }

    /*
     * If the interface identification is to be matched,
     * check on validity of vers_option.
     */
    if (((inquiry_type == rpc_c_profile_match_by_if) ||
         (inquiry_type == rpc_c_profile_match_by_both)) &&
         (!((vers_option == rpc_c_vers_all) ||
           (vers_option == rpc_c_vers_exact) ||
           (vers_option == rpc_c_vers_major_only) ||
           (vers_option == rpc_c_vers_compatible) ||
           (vers_option == rpc_c_vers_upto))))
    {
        *status = rpc_s_invalid_vers_option;
        return;
    }
    
    
    /*
     * Check the name and syntax to see if defaults are needed.
     */
    new_profile_name_syntax = profile_name_syntax;
    RPC_RESOLVE_SYNTAX (new_profile_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }
    
    /*
     * Create a name service representation of the profile name.
     */
    profile_nsentry = rpc__nsentry_from_entry
        (new_profile_name_syntax, profile_name, status);
    if (*status != rpc_s_ok) 
    {
        return;
    }

    /*
     * create a context for future inquiries
     */
    inquiry_context_rep = rpc__inqcntx_create 
        (profile_nsentry, rpc_e_profile_element, status);

    if (*status == rpc_s_ok)
    {
        /*
         * initialize the inquiry type field in the inquiry context
         */
        inquiry_context_rep->inq_specific.inqcntx_profile.inquiry_type =
                inquiry_type;

        /*
         * initialize the interface identification and
         * version option fields if match by if or both.
         */
        if ((inquiry_type == rpc_c_profile_match_by_if) ||
            (inquiry_type == rpc_c_profile_match_by_both))
        {
            inquiry_context_rep->inq_specific.inqcntx_profile.if_id =
                *if_id;

            inquiry_context_rep->
                inq_specific.inqcntx_profile.if_vers_option = vers_option;
        }

        /*
         * check the member syntax that was given and
         * initialize the member_name syntax field.
         */
        new_member_name_syntax = member_name_syntax;
        RPC_RESOLVE_SYNTAX (new_member_name_syntax, status);
        if (*status != rpc_s_ok)
        {
            rpc__inqcntx_free (&inquiry_context_rep, &local_status);
            return;
        }

        inquiry_context_rep->member_name_syntax = new_member_name_syntax;

        /*
         * expand the user specified name and copy into 
         * the member_name field if match by member or both.
         */
        if ((inquiry_type == rpc_c_profile_match_by_mbr) ||
                (inquiry_type == rpc_c_profile_match_by_both))
        {
                rpc__ns_expand_entry_name 
                    (member_name, expanded_member_name, &name_length, status);
                if (*status == rpc_s_ok)
                {
                    strcpy ((char *) inquiry_context_rep
                            ->inq_specific.inqcntx_profile.member_name,
                            (char*) expanded_member_name);
                }
                else
                {
                    rpc__inqcntx_free (&inquiry_context_rep, &local_status);
                    return;
                }
        }
    }
    else
    {
        /*
         * if not ok, free the nsentry and return with a NULL inquiry context
         */
        rpc__nsentry_free (&profile_nsentry, &local_status);
        return;
    }
    

    /*
     * Set the inquiry_context to the one created and 
     * return the valid status.
     */        
    *inquiry_context = (rpc_ns_handle_t) inquiry_context_rep;
    return;
}

/*
**++
**
**  ROUTINE NAME:       rpc_ns_profile_elt_inq_done
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**      
**  Frees the inquiry context used in viewing the elements
**  contained in a profile.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:
**
**      inquiry_context The inquiry context to be freed.
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_invalid_ns_handle
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
**/

PUBLIC void rpc_ns_profile_elt_inq_done 
#ifdef _DCE_PROTO_
(
    rpc_ns_handle_t         *inquiry_context,
    unsigned32              *status
)
#else
(inquiry_context, status)
rpc_ns_handle_t         *inquiry_context;
unsigned32              *status;
#endif
{
    
    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT ();

    /*
     * see if this is a valid inquiry context to be used here
     */
    if ((*inquiry_context == NULL) || 
        (((rpc_ns_inq_rep_p_t)*inquiry_context)->usage != 
            rpc_e_profile_element))
    {
        *status = rpc_s_invalid_ns_handle;
        return;
    }

    /*
     * free the inquiry context
     */
    rpc__inqcntx_free ((rpc_ns_inq_rep_p_t *) inquiry_context, status);

    return;
}

/*
**++
**
**  ROUTINE NAME:       rpc_ns_profile_elt_inq_next
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**      
**  Returns one element at a time from a profile.
**
**  INPUTS:
**
**      inquiry_context The context for making inquiries on this profile.
**
**  INPUTS/OUTPUTS:     
**
**
**  OUTPUTS:
**
**      if_id           The uuid and version numbers of an interface in
**                      this profile.
**
**      member_name     A profile element's member name.  If NULL on
**                      input, this argument is not returned.
**
**      priority        The profile element's priority.
**
**      annotation      A text string annotation stored for the profile
**                      element.  If no annotation is present in the profile
**                      element, the string "\0" is returned.
**                      If NULL on input, this argument is not returned.
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_invalid_ns_handle
**                          rpc_s_no_more_elements
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
**/

PUBLIC void rpc_ns_profile_elt_inq_next 
#ifdef _DCE_PROTO_
(
    rpc_ns_handle_t         inquiry_context,
    rpc_if_id_t             *if_id,
    unsigned_char_p_t       *member_name,
    unsigned32              *priority,
    unsigned_char_p_t       *annotation,
    unsigned32              *status
)
#else
(inquiry_context, if_id, member_name, priority, annotation, status)
rpc_ns_handle_t         inquiry_context;
rpc_if_id_t             *if_id;
unsigned_char_p_t       *member_name;
unsigned32              *priority;
unsigned_char_p_t       *annotation;
unsigned32              *status;
#endif
{
    unsigned8               *octet_str;
    boolean                 match;
    rpc_inqcntx_profile_p_t profile_context;
    

    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT ();

    /*
     * NULL output strings in case of failure.
     * Note, member_name and annotation are modify arguments,
     * so NULL only if caller wants them returned.
     */
    if (member_name != NULL)
    {
        *member_name = NULL;
    }

    if (annotation != NULL)
    {
        *annotation = NULL;
    }

    /*
     * see if this is a valid inquiry context to be used here
     */
    if ((inquiry_context == NULL) ||
        ((rpc_ns_inq_rep_p_t) (inquiry_context))->usage !=
            rpc_e_profile_element)
    {
        *status = rpc_s_invalid_ns_handle;
        return;
    }

    /*
     * get the profile specific context
     */
    profile_context = &(((rpc_ns_inq_rep_p_t) (inquiry_context))
        ->inq_specific.inqcntx_profile);
    
    /*
     * then start searching in this profile for the desired element 
     */
    for (;;)
    {
        /*
         * get a profile element attribute from the profile
         */
        rpc__nsattr_read_value ((rpc_ns_inq_rep_p_t) inquiry_context,
            (unsigned_char_p_t *) &octet_str,
            &(profile_context->nsprofile_elt.octet_str_length), status);

        /*
         * when there are no more members we'll return 
         * with the appropriate status
         */

        if (*status != rpc_s_ok) 
        {
            if (*status == rpc_s_no_more_members)
            {
                *status = rpc_s_no_more_elements;
            }
            return;
        }

        /*
         * Allocate a longword aligned address for the octet string 
         * and copy the data returned by DNS.  
         * DNS will and does return integer data on byte aligned addresses.
         */
        RPC_MEM_ALLOC (profile_context->nsprofile_elt.octet_str,
                       rpc_profile_elt_octet_str_p_t, 
                       profile_context->nsprofile_elt.octet_str_length,
                       RPC_C_MEM_NSPROFILE_OCTET_STR, RPC_C_MEM_WAITOK);
	if (profile_context->nsprofile_elt.octet_str == NULL){
	    *status = rpc_s_no_memory;
	    return;
	}
        memcpy ((char *)profile_context->nsprofile_elt.octet_str,
                (char *)octet_str, 
                profile_context->nsprofile_elt.octet_str_length);


        /*
         * convert the name service representation to a runtime profile
         */
        nsprofile_elt_to_profile_elt (
            ((rpc_ns_inq_rep_p_t) (inquiry_context))->member_name_syntax,
            &(profile_context->nsprofile_elt), &(profile_context->profile_elt),
            status);

        /*
         * free the allocated storage for the nsprofile_elt octet string
         */
        RPC_MEM_FREE (profile_context->nsprofile_elt.octet_str,
                      RPC_C_MEM_NSPROFILE_OCTET_STR);
        /*
         * Switch on the inquiry_type looking for the desired element.
         *
         * If a match is found, break out and copy the components.
         * If not match is found, continue with the loop until a
         * match is found or no more elements exist.
         */
        switch ((int)profile_context->inquiry_type)
        {
            case rpc_c_profile_default_elt:
                /*
                 * Match on the default profile element.
                 */
                match = uuid_is_nil (&(profile_context->profile_elt.if_id.uuid),
                                     status);
                if (*status != uuid_s_ok)
                {
                    return;
                }
                 
                if (!match)
                {
                    continue;
                }

                /*
                 * The default profile element is found.
                 * Exit switch and copy the element's components.
                 */
                break;

            case rpc_c_profile_match_by_if:
            case rpc_c_profile_match_by_mbr:
            case rpc_c_profile_match_by_both:
                /*
                 * Match profile element by if_id, or member_name, or both.
                 */
            
                /*
                 * Examine interface identification first.
                 */
                if ((profile_context->inquiry_type == 
                        rpc_c_profile_match_by_if) ||
                    (profile_context->inquiry_type == 
                        rpc_c_profile_match_by_both))
                {
                    
                    /*
                     * if this interface matches the criteria specified 
                     * in rpc_ns_profile_elt_inq_begin, then a match
                     * is found.
                     */
                    match = (rpc__if_id_compare (&(profile_context->if_id),
                        &(profile_context->profile_elt.if_id),
                        profile_context->if_vers_option, status));
        
                    if (*status != rpc_s_ok) 
                    {
                        return;
                    }

                    if (!match)
                    {
                        continue;
                    }
                }

                /*
                 * A match has been found with the interface.
                 * Now check the member_name of the element.
                 */
                if ((profile_context->inquiry_type == 
                        rpc_c_profile_match_by_mbr) ||
                    (profile_context->inquiry_type == 
                        rpc_c_profile_match_by_both))
                {
                    if ((strcmp ((char *) profile_context->member_name, 
                        (char *)(profile_context->profile_elt.name))) != 0)
                    {
                        /* 
                         * Member names do not match, continue searching.
                         */
                        continue;
                    }
                }

                /*
                 * A match interface, member_name, or both have been found.
                 */
                break;

            case rpc_c_profile_all_elts:
                /*
                 * No match criteria.
                 * Return all elements unconditional.
                 */
                break;

        } /* end switch */

        /* 
         * A match on the profile element is found.
         * Return all information to the caller.
         * The member_name and annotation are not return if
         * the caller specified a NULL address.
         */

        /*
         * copy interface identification
         */
        *if_id = profile_context->profile_elt.if_id;

        /*
         * copy priority
         */
        *priority = profile_context->profile_elt.priority;

        /*
         * Heap allocate storage for the member name to return
         * and copy the one obtained into it for the caller, if the
         * caller wants it.
         */
        if (member_name != NULL)
        {
            RPC_MEM_ALLOC (
            *member_name,
            unsigned_char_p_t,
            strlen ((char *)profile_context->profile_elt.name) + 1,
            RPC_C_MEM_STRING,
            RPC_C_MEM_WAITOK);
	    if (*member_name == NULL){
		*status = rpc_s_no_memory;
		return;
	    }
            /*
             * copy the member name
             */
        strcpy ((char *) *member_name,
                (char *) profile_context->profile_elt.name);
        }

        /*
         * Heap allocate storage for the annotation field to return
         * and copy the one obtained into it for the caller, if the
         * caller wants it.
         */
        if (annotation != NULL)
        {
            RPC_MEM_ALLOC (
                *annotation,
                unsigned_char_p_t,
                strlen ((char *)profile_context->profile_elt.annotation) + 1,
                RPC_C_MEM_STRING,
                RPC_C_MEM_WAITOK);
	    if (*annotation == NULL){
		RPC_MEM_FREE(*member_name, RPC_C_MEM_STRING);
		*status = rpc_s_no_memory;
		return;
	    }


            /*
             * strcpy copies the NUL character, so if empty in
             * the profile, "\0" (NUL) is returned, as specified
             * in the funtional specification.
             */
            strcpy ((char *) *annotation,
            (char *) profile_context->profile_elt.annotation);
        }

        /*  
         * Return matched element to caller.
         */
        *status = rpc_s_ok;
        return;

    } /* end for */
}


/*
**++
**
**  ROUTINE NAME:       rpc_ns_profile_elt_remove
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**      
**  Removes an element from a profile.  The entry (member_name) included
**  as a member in the profile element is not deleted.
**
**  INPUTS:
**
**      profile_name_syntax The name service syntax used for the specified
**                      profile name.
**
**      profile_name    The entry in the namespace containing the profile
**                      attribute from which to remove an element.
**
**      if_id           The interface identification (UUID and version numbers)
**                      of the profile element to be removed.
**                      A NULL value specifies the default profile member.
**
**      member_name_syntax The name service syntax used for the specified
**                      member name.
**
**      member_name     The name service entry name in the profile element
**                      to remove.
**
**
**  INPUTS/OUTPUTS:     none
**
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_profile_element_not_found
**                          errors from other called routines.
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
**/

PUBLIC void rpc_ns_profile_elt_remove 
#ifdef _DCE_PROTO_
(
    unsigned32              profile_name_syntax,
    unsigned_char_p_t       profile_name,
    rpc_if_id_p_t           if_id,
    unsigned32              member_name_syntax,
    unsigned_char_p_t       member_name,
    unsigned32              *status
)
#else
(profile_name_syntax, profile_name, if_id, member_name_syntax, member_name, status)
unsigned32              profile_name_syntax;
unsigned_char_p_t       profile_name;
rpc_if_id_p_t           if_id;
unsigned32              member_name_syntax;
unsigned_char_p_t       member_name;
unsigned32              *status;
#endif
{
    rpc_ns_handle_t         inquiry_context;
    rpc_if_id_t             inq_if_id,
                            local_if_id,
                            *search_if_id = &local_if_id;
    unsigned_char_p_t       annotation,
                            found_member_name;
    rpc_nsprofile_elt_p_t   nsprofile_elt;
    unsigned32              new_profile_name_syntax,
                            new_member_name_syntax,
                            search_for,
                            temp_status,
    			    priority;
    

        

    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT ();
    
    /*
     * check the name and syntax to see if defaults are needed
     */
    new_profile_name_syntax = profile_name_syntax;
    RPC_RESOLVE_SYNTAX (new_profile_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    new_member_name_syntax = member_name_syntax;
    RPC_RESOLVE_SYNTAX (new_member_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    if ( (if_id == NULL) || uuid_is_nil (&(if_id->uuid), status) )
    {
        /*
         * Set up a NIL UUID and inquiry type to search for 
         * the default profile element.
         */
        uuid_create_nil (&(search_if_id->uuid), status);
        search_if_id->vers_major = 0;
        search_if_id->vers_minor = 0;
        search_for = rpc_c_profile_default_elt;
    }
    else
    {
        /*
         * Otherwise, we're searching for both if_id and member_name.
         */
        *search_if_id = *if_id;
        search_for = rpc_c_profile_match_by_both;
    }
         
    /*
     * set up a search context to find the element to be removed
     */
    rpc_ns_profile_elt_inq_begin (new_profile_name_syntax, profile_name,
        search_for, search_if_id, rpc_c_vers_exact, new_member_name_syntax,
        member_name, &inquiry_context, status);

    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Because we may have to first read from the name service
     * (to find out what to remove), set the inquiry context
     * to specify a cache bypass. We want to make sure we have
     * up to date data for the delete operation.
     */
    rpc_ns_mgmt_handle_set_exp_age
        (inquiry_context, RPC_C_NS_BYPASS_CACHE, status);
    
    if (*status != rpc_s_ok)
    {
        return;
    }
    
    /*
     * get the element to be removed
     *
     * This code is called directly by an application as well as from
     * rpc_ns_profile_elt_add. Add calls this routine to see if we need
     * to replace an existing element. There are 2 reasons we'd replace 
     * an element:
     *
     * o  to change an element's priority and/or annotation
     * 
     *    In this case, the new element member-name is the same as
     *    the old. It isn't really necessary to have rpc_ns_profile_elt_inq_next
     *    return the member name - we've already got it as a routine input arg.
     *
     * o  to replace the default element
     *
     *    In this case, only the if-id (NULL or nil if-uuid) signifies the
     *    element of interest. However, when we go to remove the existing
     *    default, we can't be using the new default member name (from the 
     *    routine input arg member_name) since it's likely different than
     *    the current default's member name.
     * 
     * So here we have rpc_ns_profile_elt_inq_next return the found member 
     * name and use it to remove the element.
     */
    rpc_ns_profile_elt_inq_next
	(inquiry_context, &inq_if_id, &found_member_name, &priority, 
        &annotation, status);
            
    if (*status == rpc_s_ok)
    {
	/*
	 * create a nsprofile element necessary for the attribute removal
	 */
        nsprofile_elt = nsprofile_elt_from_profile_elt (
            new_member_name_syntax,
            found_member_name,
            search_if_id,
            priority,
            annotation,
            status);
    
        rpc_string_free (&annotation, &temp_status);

        if (*status == rpc_s_ok)
        {
            /*
             * remove this member
             */
            rpc__nsattr_remove_value (
                ((rpc_ns_inq_rep_p_t) (inquiry_context))->nsentry,
                RPC_C_ATTR_PROFILE,
                (unsigned_char_p_t) nsprofile_elt->octet_str,
	        nsprofile_elt->octet_str_length,
                status);

            if (*status == rpc_s_not_found)
            {
                *status = rpc_s_profile_element_not_found;
            }

            /*
             * free the allocated storage for the nsprofile_elt 
             */
            nsprofile_elt_free (&nsprofile_elt, &temp_status);
        }
    }
    else
    {
        /*
         * If the element wasn't found, return that info.
         * Otherwise we'll return whatever the status is.
         */
            if ((*status == rpc_s_no_more_elements) ||
                (*status == rpc_s_not_rpc_entry))
            {
                *status = rpc_s_profile_element_not_found;
            }
    }

    /*
     * free the inquiry context
     */
    rpc_ns_profile_elt_inq_done (&inquiry_context, &temp_status);   
            
    /*
     * Return with last meaningful status.
     */
    return;
}


/*
**++
**
**  ROUTINE NAME:       nsprofile_elt_free
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**      
**  Frees a name service representation of a profile element.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:
**
**      nsprofile_elt   A pointer to the profile element to be freed.
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
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
**/

INTERNAL void nsprofile_elt_free 
#ifdef _DCE_PROTO_
(
    rpc_nsprofile_elt_p_t   *nsprofile_elt,
    unsigned32              *status
)
#else
(nsprofile_elt, status)
rpc_nsprofile_elt_p_t   *nsprofile_elt;
unsigned32              *status;
#endif
{
    /*
     * free the profile octet string field and the nsprofile elt
     */
    RPC_MEM_FREE
        ((*nsprofile_elt)->octet_str, RPC_C_MEM_NSPROFILE_OCTET_STR);
    
    RPC_MEM_FREE (*nsprofile_elt, RPC_C_MEM_NSPROFILE_ELT);
    
    *nsprofile_elt = NULL;
    
    *status = rpc_s_ok;
    return;
}

/*
**++
**
**  ROUTINE NAME:       nsprofile_elt_from_profile_elt
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**      
**  Creates a name service representation of a profile element.
**
**  INPUTS:
**
**      member_name_syntax  The name service syntax used for the specified
**                      member name.  (Note, this has already been resolved.)
**
**      member_name     The profile element member name.
**
**      if_id           The UUID and version numbers of the interface
**                      in the profile.
**
**      priority        An integer value (from 0-7) that specifies the
**                      relative lookup priority for this member.
**
**      annotation      The text string annotation stored with the profile
**                      element.  
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_no_memory
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      A pointer to the name service representation of the profile element.
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL rpc_nsprofile_elt_t *nsprofile_elt_from_profile_elt 
#ifdef _DCE_PROTO_
(
    unsigned32              member_name_syntax,
    unsigned_char_p_t       member_name,
    rpc_if_id_p_t           if_id,
    unsigned32              priority,
    unsigned_char_p_t       annotation,
    unsigned32              *status
)
#else
(member_name_syntax, member_name, if_id, priority, annotation, status)
unsigned32              member_name_syntax;
unsigned_char_p_t       member_name;
rpc_if_id_p_t           if_id;
unsigned32              priority;
unsigned_char_p_t       annotation;
unsigned32              *status;
#endif
{
    rpc_nsentry_p_t         member_nsentry;
    rpc_nsprofile_elt_p_t   nsprofile_elt;
    rpc_nsuuid_p_t          nsif_uuid;
    unsigned_char_t         *temp_p;
    unsigned8               annotation_length;
    unsigned16              name_length_rep;
    unsigned32              octet_str_length;
    unsigned32              temp_status;
    
    CODING_ERROR (status);
    
    /*
     * create a name service rep of the member name
     */
    member_nsentry = rpc__nsentry_from_entry
        (member_name_syntax, member_name, status);
    if (*status != rpc_s_ok) 
    {
        return NULL;
    }

    /*
     * get annotation length and truncate to maximum 
     * (both counts include the NUL terminator
     */
    annotation_length = ( strlen ((char *) annotation) + 1);
    if (annotation_length > RPC_C_ANNOTATION_MAX)
    {
        annotation_length = RPC_C_ANNOTATION_MAX;
    }

    /*
     * compute the size of the nsprofile element
     */
    octet_str_length = 
        sizeof (rpc_nsuuid_t)           /* nsuuid	    */
        + sizeof (unsigned16)           /* if version major */
        + sizeof (unsigned16)           /* if version minor */
        + sizeof (unsigned8)            /* priority         */
        + sizeof (unsigned8)            /* annotation length */
        + annotation_length             /* annotation string (+ NUL) */
        + sizeof (unsigned16)           /* nsentry name length */
        + member_nsentry->name_length;  /* name (includes NUL) */

    /*
     * allocate space for the nsprofile element
     */
    RPC_MEM_ALLOC (
        nsprofile_elt,
        rpc_nsprofile_elt_p_t,
        sizeof (rpc_nsprofile_elt_t),
        RPC_C_MEM_NSPROFILE_ELT,
        RPC_C_MEM_WAITOK);
    if (nsprofile_elt == NULL){
        rpc__nsentry_free (&member_nsentry, &temp_status);
	*status = rpc_s_no_memory;
	return(NULL);
    }

        
    /*
     * allocate space for the profile element field
     */
    RPC_MEM_ALLOC (
        nsprofile_elt->octet_str,
        rpc_profile_elt_octet_str_p_t,
        octet_str_length,
        RPC_C_MEM_NSPROFILE_OCTET_STR,
        RPC_C_MEM_WAITOK);
    if (nsprofile_elt->octet_str == NULL){
	RPC_MEM_FREE(nsprofile_elt, RPC_C_MEM_NSPROFILE_ELT);
        rpc__nsentry_free (&member_nsentry, &temp_status);
	*status = rpc_s_no_memory;
	return(NULL);
    }

    /*
     * set the profile length field to the calculated size
     */
    nsprofile_elt->octet_str_length = octet_str_length;
    
    /*
     * create a name service representation of the interface uuid,
     * placing it in the proper endian representation, 
     * and copy it into the nsprofile_elt
     */
    nsif_uuid = rpc__nsuuid_from_uuid (&(if_id->uuid), status);

    if (*status != rpc_s_ok)
    {

        /* 
         * clean-up before return -- free ns profile element, the profile
         *                           octet string field and member nsentry
         */
        RPC_MEM_FREE (nsprofile_elt->octet_str, RPC_C_MEM_NSPROFILE_OCTET_STR);
        RPC_MEM_FREE (nsprofile_elt, RPC_C_MEM_NSPROFILE_ELT);
        rpc__nsentry_free (&member_nsentry, &temp_status);
            
        return NULL;
    }
    
    nsprofile_elt->octet_str->if_uuid = *nsif_uuid;
    
    /*
     * free the nsuuid
     */
    rpc__nsuuid_free (&nsif_uuid, status);
    
    /* 
     * copy the interface major and minor versions into the nsprofile_elt
     */
    nsprofile_elt->octet_str->if_vers_major = if_id->vers_major;
    nsprofile_elt->octet_str->if_vers_minor = if_id->vers_minor;

    /*
     * correct interface versions for difference in endian
     */
    RPC_RESOLVE_ENDIAN_INT16 (nsprofile_elt->octet_str->if_vers_major);
    RPC_RESOLVE_ENDIAN_INT16 (nsprofile_elt->octet_str->if_vers_minor);

    /*
     * copy the priority into the nsprofile_elt
     */
    nsprofile_elt->octet_str->priority = (unsigned8) priority;
    
    /*
     * Copy the annotation length and string into the nsprofile_elt.  
     * NUL the string.
     */
    nsprofile_elt->octet_str->count = (unsigned8) annotation_length;
    strncpy ((char *) (nsprofile_elt->octet_str->annotation),
            (char *) annotation, annotation_length-1);
    nsprofile_elt->octet_str->annotation[annotation_length-1] = '\0';

    /*
     * get address to where the name_length goes in the octet_str.
     */
    temp_p = (unsigned8 *) (nsprofile_elt->octet_str->annotation+
                        nsprofile_elt->octet_str->count);

    /*
     * Resolve name length to stored representation (little endian)
     * and copy to octet_str.
     * (nsentry->name_length includes the NUL in its count.)
     */
    name_length_rep = member_nsentry->name_length;
    RPC_RESOLVE_ENDIAN_INT16 (name_length_rep);

    memcpy ( (char *)temp_p, (char *)&name_length_rep, sizeof(unsigned16)); 

    /*
     * get address to where the name goes in the octet_str and copy it.
     */
    temp_p += sizeof(unsigned16);
    strcpy ((char *) temp_p, (char *) member_nsentry->name);

    /*
     * free the nsentry
     */
    rpc__nsentry_free (&member_nsentry, status);
    
    *status = rpc_s_ok;
    return nsprofile_elt;
}

/* 
**++
**
**  ROUTINE NAME:       nsprofile_elt_to_profile_elt
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**      
**  Converts a name service representation of a profile element to a runtime
**  representation.
**
**  INPUTS:
**
**      member_name_syntax  The name service syntax used for the runtime
**                      representation of the member name.
**
**      nsprofile_elt   The profile element in the name service which is
**                      to be converted.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      profile_elt     The runtime representation of the converted profile.
**
**      status          The result of the operation. One of:
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
**/

INTERNAL void nsprofile_elt_to_profile_elt 
#ifdef _DCE_PROTO_
(
    unsigned32              member_name_syntax,
    rpc_nsprofile_elt_p_t   nsprofile_elt,
    rpc_profile_elt_t       *profile_elt,
    unsigned32              *status
)
#else
(member_name_syntax, nsprofile_elt, profile_elt, status)
unsigned32              member_name_syntax;
rpc_nsprofile_elt_p_t   nsprofile_elt;
rpc_profile_elt_t       *profile_elt;
unsigned32              *status;
#endif
{
    rpc_nsentry_t           member_nsentry;
    unsigned32              name_length;
    unsigned_char_t         *temp_p;
    

    /*
     * start with an ok status - it might get changed
     */
    *status = rpc_s_ok;

    /*
     * copy a runtime rep of the interface uuid into the profile element
     * note, nsuuid_to_uuid corrects for difference in endian representations
     */
    rpc__nsuuid_to_uuid (&(nsprofile_elt->octet_str->if_uuid), 
            &(profile_elt->if_id.uuid), status);

    /*
     * copy the interface minor and minor version numbers
     */
    profile_elt->if_id.vers_major = nsprofile_elt->octet_str->if_vers_major;
    profile_elt->if_id.vers_minor = nsprofile_elt->octet_str->if_vers_minor;

    /*
     * correct interface versions for difference in endian
     */
    RPC_RESOLVE_ENDIAN_INT16 (profile_elt->if_id.vers_major);
    RPC_RESOLVE_ENDIAN_INT16 (profile_elt->if_id.vers_minor);

    /*
     * copy the priority
     */
    profile_elt->priority = nsprofile_elt->octet_str->priority;
    
    /*
     * copy the annotation string into the profile element
     * including its NUL terminator.
     */
    strcpy ((char *) (profile_elt->annotation),
            (char *) (nsprofile_elt->octet_str->annotation));

    /*
     * get address to the name and copy it into the nsentry.
     * skip over the name_length field. NOTE: When we use the name_length
     * in the future, remember to resolve to host endian representation.
     */
     
    temp_p =  nsprofile_elt->octet_str->annotation + 
              nsprofile_elt->octet_str->count;
                            
    member_nsentry.name = (temp_p + sizeof(unsigned16));

    /*
     * convert this form into the profile element
     */
    rpc__nsentry_to_entry (&member_nsentry, member_name_syntax,
        profile_elt->name, &name_length, status);

    return;
}    
