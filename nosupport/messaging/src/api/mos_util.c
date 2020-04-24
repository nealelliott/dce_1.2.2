/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 *  m o s _ u t i l . c
 *
 *  Common interfaces used by the API, Q-mgr, and Admin to support the 
 *  Message Oriented Service Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 *
 * $Log: mos_util.c,v $
 * Revision 1.6  1996/11/23  11:19:21  salamone
 * Added mos_name_resolution()
 *
 * Revision 1.5  96/11/20  14:48:37  14:48:37  salamone (Julie Salamone)
 * Fix header file declarations
 * 
 * Revision 1.4  96/11/13  18:20:35  18:20:35  salamone (Julie Salamone)
 * Allow apps to use mos_datatype_ack
 * 
 * Revision 1.3  96/11/13  17:20:51  17:20:51  bowe (John Bowe)
 * Added ack_datatype.
 * 
 * Revision 1.2  1996/10/29  16:49:39  salamone
 * Added function descriptions and cleaned up compiler warnings
 *
 * Revision 1.1  96/10/14  16:56:55  16:56:55  salamone (Julie Salamone)
 * Initial revision
 * 
 * $EndLog$
 * ______________________________________________________________________
 */


/* 
 * INCLUDE FILES  
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dce_cf.h>
#include <dce/nbase.h>
#include <dce/assert.h>
#include <dce/dce_svc.h>
#include <dce/pgo.h>
#include <dce/sec_login.h>
#include <mos_proto.h>  

/*
 * INTERNAL PROTOTYPES
 */
void mos__string_list_free_ptrs(void (*)(void *), mos_string_list_t *);
void mos__string_list_free(void (*)(void *), mos_string_list_t **);
void mos__uuid_list_free_ptrs(void (*)(void *), mos_uuid_list_t *);
void mos__uuid_list_free(void (*)(void *), mos_uuid_list_t **);

/*
 * INITIALIZED DATA
 */
/* 
 * Well-known ACK datatype UUID for mos_ack_convert().  
 *    e7daf4c6-bd70-11cf-87df-0800092474cc 
 * Agrees with ../idl/mos_ack.idl. 
 */
uuid_t mos_datatype_ack = { 
  0xe7daf4c6, 0xbd70, 0x11cf, 0x87, 0xdf,
  {0x08, 0x00, 0x09, 0x24, 0x74, 0xcc}
};


/*
 *  FUNCTION:
 *  m o s  _ n a m e _ r e s o l u t i o n
 *
 *  OVERVIEW:
 *  Circumvent resolving named links within a name. Since named links to queue
 *  managers may have been created, rpc_ns_entry_inq_resolution() does not
 *  determine resolved and unresolved components of a name properly.
 *  Refer to OT 13614.
 *
 *  INPUTS:         
 *  entry_name:     Specified name to resolve.
 *
 *  OUTPUTS:        
 *  resolved:
 *                  Resolved portion of "entry_name".
 *                  The resolved name portion is the server name.
 *
 *  target:         CDS link name portion of "entry_name".
 *
 *  unresolved:
 *                  Unresolved portion of "entry_name".
 *                  The unresolved name portion is the queue name.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:
 *                  None
 */
void 
mos_name_resolution(
		    char *	        entry_name, 
		    char **	        resolved, 
		    char **	        target, 
		    char **	        unresolved,
		    error_status_t *	status
		    )
{
  unsigned_char_t *exp_name, *tmp_entry, *tmp_unresolved, *tmp_resolved;
  unsigned32 st;
  
  *resolved = *target = *unresolved = NULL;
  
  /* see if we can just use the rpc_ns_entry_inq-resolution() to get
     the right result, if no link is involved */
  rpc_ns_entry_inq_resolution(rpc_c_ns_syntax_default,
                              (unsigned_char_t *)entry_name,
                              (unsigned_char_t **)resolved,
                              (unsigned_char_t **)unresolved,
                              status);
  if (*status != rpc_s_partial_results) {
    /* either it is ok, or something wrong */
    return;
  }
  
  /* expand the name */
  rpc_ns_entry_expand_name(rpc_c_ns_syntax_default,
                           (unsigned_char_t *)entry_name,
                           &exp_name,
                           &st);
  /* now that we have partial result, we have to check whether the
     resolved part is part of the original name. If so, there is no
     link in it, the unsolved part is ok. Otherwise we have to do
     something to find the right unresolved part */
  if (!strncmp((char *)exp_name, *resolved, strlen(*resolved))) {
    return;
  }
  
  /* ok, we know link is in it. We check from right to left,
     taking away piece by piece until we get the link itself */
  *target = malloc(strlen(*resolved)+1);
  
  strcpy(*target, *resolved);
  rpc_string_free((unsigned_char_t **)unresolved, &st);
  rpc_string_free((unsigned_char_t **)resolved, &st);
  
  tmp_entry = (unsigned_char_t *)malloc(strlen((char *)exp_name)+1);
  strcpy((char *)tmp_entry, (char *)exp_name);
  
  tmp_unresolved = tmp_entry + strlen((char *)tmp_entry);
  while (1) {
    /* from right to left, find the first '/' from tmp_unresolved pos */
    while (*tmp_unresolved != '/') tmp_unresolved --;
    *tmp_unresolved = '\0';      
    rpc_ns_entry_inq_resolution(rpc_c_ns_syntax_default,
                                tmp_entry,
                                &tmp_resolved,
                                (unsigned_char_t **)unresolved,
                                status);
    if (*unresolved != NULL) 
      rpc_string_free((unsigned_char_t **)unresolved, &st);
    if (!strncmp((char *)tmp_entry, (char *)tmp_resolved, 
		 strlen((char *)tmp_resolved))) {
      /* we have found the link itself */
      *unresolved = malloc(strlen((char *)(tmp_unresolved+1))+1);
      strcpy(*unresolved, (char *)(tmp_unresolved+1));
      free((char *)tmp_entry);
      *resolved = malloc(strlen((char *)tmp_resolved)+1);
      strcpy(*resolved, (char *)tmp_resolved);
      rpc_string_free(&tmp_resolved, &st);
      *status = rpc_s_partial_results;
      return;
    }

    /* we have not found the link yet */
    *tmp_unresolved = '/';
    tmp_unresolved --;
  }
      
} /* End of mos_name_resolution() routine */


/*
 *  FUNCTION:
 *  m o s  _ i s _ l o g g e d _ i n
 *
 *  OVERVIEW:
 *  Determine if dce logged in or not.
 *
 *  INPUTS:         None
 *
 *  OUTPUTS:        None
 *
 *  RETURNS:
 *                  TRUE  indicates caller has logged in.
 *                  FALSE indicates caller has not logged in.
 */
boolean
mos_is_logged_in(void)
{
    sec_login_handle_t          login_context_h;
    error_status_t              status;

    sec_login_get_current_context(&login_context_h, &status);
    if (GOOD_STATUS(&status)) {
        return(TRUE);
    }

    return(FALSE);

} /* End of mos_is_logged_in() routine */


/*
 *  FUNCTION:
 *  m o s  _ i s _ p r i n c i p a l _ v a l i d 
 *
 *  OVERVIEW:
 *  Validates principal with the security registry to determine if member of 
 *  the security group he's supposed to be in. 
 *
 *  INPUTS:
 *  full_prin_name:
 *                  Specifies full principal name.
 *  group_name:
 *                  Assume group since support for message queueing model only.
 *                  Specifies the security group name where the named 
 *                  resource is a member. In a message queuing model, only a 
 *                  security group name is allowed.
 *                  In a message passing model, a sender knows what recipient
 *                  to trust and only has to specify a principal name, if
 *                  desired.
 *                  If NULL is specified, caller trusts CDS. Not recommended.
 *
 *  OUTPUTS:
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        
 *                  TRUE indicates that it's valid
 *                  FALSE indicates that it's not valid.
 */
boolean
mos_is_principal_valid(
                       sec_rgy_name_t          full_prin_name, 
                       sec_rgy_name_t          group_name,
                       error_status_t *        status
                       )
{
    sec_rgy_name_t      local_prin_name;  /* Local principal name */
    char *              cell_name;        /* Local cell name */
    sec_rgy_handle_t    rgy_handle;       /* Local registry binding */
    boolean             is_valid;

    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Obtain local cell name
     */
    dce_cf_get_cell_name(&cell_name, status);
    if (*status != dce_cf_st_ok){
       return(FALSE);
    }

    /*
     * Bind to local cell registry
     */
    sec_rgy_site_open(cell_name, &rgy_handle, status);
    if (BAD_STATUS(status)){
       free(cell_name);
       return(FALSE);
    }
    free(cell_name);    /* Got handle, free cell name */


    sec_id_parse_name(rgy_handle,    
                      full_prin_name,   /* Global (full) name of principal */
                      NULL, 
                      NULL,
                      local_prin_name,  /* Local name of principal */
                      NULL, 
                      status);
    if (BAD_STATUS(status)){
       return(FALSE);
    }

    /* 
     * And finally, find out from the registry whether that principal   
     * is a valid member of the specified group... 
     */
    is_valid = sec_rgy_pgo_is_member(rgy_handle,
                                     sec_rgy_domain_group,
                                     group_name,
                                     local_prin_name,
                                     status);
    if (BAD_STATUS(status)){
       return(FALSE);
    }

    return(is_valid);

} /* End of mos_is_secname_validate() routine */


/*
 *  FUNCTION:
 *  m o s _ s t r i n g _ l i s t _ f r e e _ p t r s
 *
 *  OVERVIEW:
 *  Free every string of the list, and then free the whole list.
 *  The string list itself pointed by string_list_p is not freed,
 *  but initialized.
 *
 *  INPUTS:
 *  string_list_p:
 *                  A pointer to a list of text strings.
 *
 *  RETURNS:        None
 */
void
mos_string_list_free_ptrs(
                          mos_string_list_t *string_list_p
                         )
{
    mos__string_list_free_ptrs(free, string_list_p);

    return;

} /* End of mos_string_list_free_ptrs() routine */


/*
 *  FUNCTION:
 *  m o s _ s t r i n g _ l i s t _ f r e e
 *
 *  OVERVIEW:
 *  Free every string of the list, and then free the whole list.
 *  Finally the *string_list_p itself is freed and set to NULL.
 *
 *  INPUTS/OUTPUTS:
 *  string_list_p:
 *                  A list of text strings.
 *                  Returns the value NULL.
 *
 *  RETURNS:        None
 */
void
mos_string_list_free(
                     mos_string_list_t **string_list_p
                    )
{

    mos__string_list_free(free, string_list_p);

    return;

} /* End of mos_string_list_free() routine */


/*
 *  FUNCTION:
 *  m o s _ _ s t r i n g _ l i s t _ f r e e _ p t r s
 *
 *  OVERVIEW:
 *  Call (*deallocate)() to free every string of the list, and then
 *  free the whole list. The string list itself pointed  by
 *  string_list is not freed, but initialized.
 *
 *  INPUTS:
 *  deallocate():
 *                  A pointer to a "free" function call to be used.
 *
 *  INPUTS/OUTPUTS:
 *  string_list_p:
 *                  A list of text strings.
 *
 *  RETURNS:        None
 */
void
mos__string_list_free_ptrs(
                           void                      (*deallocate)(void *ptr),
                           mos_string_list_t         *string_list 
                          )
{
    int i;

    if (EMPTY_LIST(string_list)) {
       return;
    }

    for (i=0; i<string_list->count; i++) {
        (*deallocate)(string_list->list[i]);
    }
    (*deallocate)(string_list->list);
    string_list->count = 0;
    string_list->list = NULL;

    return;

} /* End of mos__string_list_free_ptrs() routine */


/*
 *  FUNCTION:
 *  m o s _ _ s t r i n g _ l i s t _ f r e e
 *
 *  OVERVIEW:
 *  Call (*deallocate)() to free every string of the list, and
 *  then free the whole list. Finally the *string_list itself is
 *  freeed and set to NULL.
 *
 *  INPUTS:
 *  deallocate():
 *                  A pointer to a "free" function call to be used.
 *
 *  INPUTS/OUTPUTS:
 *  string_list_p:
 *                  A list of text strings.
 *                  Returns the value NULL.
 *
 *  RETURNS:        None
 */
void
mos__string_list_free(
                      void                   (*deallocate)(void *ptr),
                      mos_string_list_t     **string_list 
                     )
{

    if (*string_list) {
       mos__string_list_free_ptrs(deallocate, *string_list);
       (*deallocate)(*string_list);
       *string_list = NULL;
    }

    return;

} /* End of mos__string_list_free() routine */


/*
 *  FUNCTION:
 *  m o s _ u u i d _ l i s t _ f r e e _ p t r s
 *
 *  OVERVIEW:
 *  Free the uuid list, the uuid list itself pointed by uuid_list_p
 *  is not freed,  but initialized.
 *
 *  INPUTS/OUTPUTS:
 *  uuid_list_p:
 *                  A list of UUIDs.
 *
 *  RETURNS:        None
 */
void
mos_uuid_list_free_ptrs(
                        mos_uuid_list_t *uuid_list_p
                       )
{

    mos__uuid_list_free_ptrs(free, uuid_list_p);

    return;

} /* End of mos_uuid_list_free_ptrs() routine */


/*
 *  FUNCTION:
 *  m o s _ u u i d _ l i s t _ f r e e
 *
 *  OVERVIEW:
 *  Free the uuid list, and then the *uuid_list_p is freed and set
 *  to NULL.
 *
 *  INPUTS/OUTPUTS:
 *  uuid_list_p:
 *                  A list of UUIDs.
 *                  Returns the value NULL.
 *
 *  RETURNS:        None
 */
void
mos_uuid_list_free(
                   mos_uuid_list_t **uuid_list_p
                  )
{

    mos__uuid_list_free(free, uuid_list_p);

    return;

} /* End of mos_uuid_list_free() routine */


/*
 *  FUNCTION:
 *  m o s _ _ u u i d _ l i s t _ f r e e _ p t r s
 *
 *  OVERVIEW:
 *  Call (*deallocate)() to free the uuid list, the uuid list
 *  itself pointed by uuid_list is not freed,  but initialized.
 *
 *  INPUTS:
 *  deallocate():
 *                  A pointer to a "free" function call to be used.
 *
 *  INPUTS/OUTPUTS:
 *  uuid_list_p:
 *                  A list of UUIDs.
 *
 *  RETURNS:        None
 */
void
mos__uuid_list_free_ptrs(
                         void            (*deallocate)(void *ptr),
                         mos_uuid_list_t *uuid_list 
                        )
{
    if (EMPTY_LIST(uuid_list)) {
        return;
    }

    (*deallocate)(uuid_list->list);
    uuid_list->count = 0;
    uuid_list->list = NULL;

    return;

} /* End of mos__uuid_list_free_ptrs() routine */


/*
 *  FUNCTION:
 *  m o s _ _ u u i d _ l i s t _ f r e e
 *
 *  OVERVIEW:
 *  Call (*deallocate)() to free the uuid list, and then
 *  the *uuid_list is freed and set to NULL.
 *
 *  INPUTS:
 *  deallocate():
 *                  A pointer to a "free" function call to be used.
 *
 *  INPUTS/OUTPUTS:
 *  uuid_list_p:
 *                  A list of UUIDs.
 *                  Returns the value NULL.
 *
 *  RETURNS:        None
 */
void
mos__uuid_list_free(
                    void                 (*deallocate)(void *ptr),
                    mos_uuid_list_t      **uuid_list 
                   )
{

    if (*uuid_list) {
       uuid_list_free_ptrs(deallocate, *uuid_list);
       (*deallocate)(*uuid_list);
       *uuid_list = NULL;
    }

    return;

} /* End of mos__uuid_list_free() routine */

