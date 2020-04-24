/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_acl_encode.c,v $
 * Revision 1.1.6.1  1996/10/03  14:43:23  arvind
 * 	Silence compiler warnings.
 * 	[1996/09/17  00:13 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.4.2  1996/03/11  13:26:01  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:05  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:54:46  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/08  23:30 UTC  jrr
 * 	Fix memory leak by using correct alloc/free for IDL es.
 * 	[1995/06/08  23:28 UTC  jrr  /main/jrr_sec_chfts15431/1]
 * 
 * Revision 1.1.2.4  1994/06/09  19:19:41  annie
 * 	cr10871 - expand copyright
 * 	[1994/06/09  19:19:05  annie]
 * 
 * 	cr10871 - expand copyright
 * 
 * 	cr10871 - expand copyright
 * 
 * Revision 1.1.2.3  1994/06/02  22:02:13  mdf
 * 	Merged with changes from 1.1.2.2
 * 	[1994/06/02  22:01:41  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:13:13  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.2.2  1994/06/02  20:20:24  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:13:13  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * $EndLog$
 */

/* sec_acl_encode.c
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */

#include <stdlib.h>
#include <dce/sec_acl_encode.h>
#include <macros.h>
#include <dce/exc_handling.h>
#include <dce/idl_es.h>
#include <dce/id_encode.h>


/*
 * if function pointer is NULL use rpc_ss_client_free
 */
#define DEALLOCATE(deallocator,ptr) \
  { if (deallocator) (*(deallocator))((ptr)); else rpc_ss_client_free((ptr)); }
  
/* convenience typedefs */
typedef idl_void_p_t (*allocate_fn_t)(idl_size_t size);
typedef void (*free_fn_t)(idl_void_p_t ptr);

#define SET_ALLOC_FREE(allocate,deallocate,stp) { \
    allocate_fn_t               old_allocate; \
    free_fn_t                   old_free; \
    volatile boolean32          swap_alloc_free; \
    volatile rpc_ss_thread_handle_t      thread_handle; \
    volatile idl_boolean                 enabled_allocator; \
    TRY \
        thread_handle = rpc_ss_get_thread_handle(); \
    CATCH(pthread_badparam_e); \
        thread_handle = NULL; \
    ENDTRY; \
    if (thread_handle == NULL) { \
        enabled_allocator = idl_true; \
        rpc_ss_enable_allocate(); \
    } \
    else \
        enabled_allocator = idl_false; \
    if (swap_alloc_free = ((allocate) && (deallocate))) { \
        rpc_sm_swap_client_alloc_free((allocate), (deallocate), \
				      &old_allocate, &old_free, (stp)); \
    } \
    if (STATUS_OK((stp)))

#define RESET_ALLOC_FREE \
    if (swap_alloc_free) { \
        rpc_sm_set_client_alloc_free(old_allocate, old_free, (stp)); \
    }; \
    if (enabled_allocator) { \
        rpc_ss_disable_allocate(); \
    } \
}


/*
 * There may be a better way of enforcing error_status rather
 * than exception handling as the error detection mechanism
 * for encoding routines, but using TRY/CATCH seems to work,
 * so encapsulate that model in macros.
 */
#define ENCODE_TRY(stp) { \
    volatile error_status_t ENCODE_TRY_st; \
    CLEAR_STATUS(&ENCODE_TRY_st); \
    TRY {

#define ENCODE_ENDTRY \
    } CATCH_ALL { \
        error_status_t ENCODE_ENDTRY_lst = error_status_ok; \
        /*  \
	 * if the exception has been mapped to a status value, use that \
	 * as our return status. Otherwise, set our return status \
	 * to rpc_s_unknown_error \
	 */ \
	 ENCODE_TRY_st = exc_get_status(THIS_CATCH, &ENCODE_ENDTRY_lst) \
	                     ? rpc_s_unknown_error  : ENCODE_ENDTRY_lst; \
    } ENDTRY; \
    *(stp) =  ENCODE_TRY_st; \
    }   


/*
 * sec_acl_entry_decode
 */
PUBLIC void sec_acl_entry_encode(
    idl_void_p_t        (*alloc)(idl_size_t size),     /* [in] */
    void                (*dealloc)(idl_void_p_t ptr),  /* [in] */
    sec_acl_entry_t     *entry,                        /* [in] */
    unsigned32          *num_bytes,                    /* [out] */
    idl_byte            **bytes,                       /* [out] */
    error_status_t      *stp                           /* [out] */
)
{
    idl_es_handle_t         h;
    error_status_t          est = error_status_ok;

    *stp = error_status_ok;

    idl_es_encode_dyn_buffer(bytes, num_bytes, &h, stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    SET_ALLOC_FREE(alloc, dealloc, &est) {
        ENCODE_TRY(&est) {
            dce_acl_entry_encode(h, entry);
        } ENCODE_ENDTRY;
    } RESET_ALLOC_FREE; 

    idl_es_handle_free(&h, stp);

    /*
     * If the status of the encoding operation was bad, we report
     * that back. otherwise we report back the status of the
     * handle free operation.
     */
    if (BAD_STATUS(&est)) {
	*stp = est;
    }
}


/*
 * sec_acl_entry_decode
 */
PUBLIC void sec_acl_entry_decode(
    idl_void_p_t        (*alloc)(idl_size_t size),     /* [in] */
    void                (*dealloc)(idl_void_p_t ptr),  /* [in] */
    unsigned32          num_bytes,                     /* [in] */
    idl_byte            *bytes,                        /* [in] */
    sec_acl_entry_t     *entry,                        /* [out] */
    error_status_t      *stp                           /* [out] */
)
{
    idl_es_handle_t h;
    error_status_t est = error_status_ok;

    idl_es_decode_buffer(bytes, num_bytes, &h, stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    SET_ALLOC_FREE(alloc, dealloc, &est) {
        ENCODE_TRY(&est) {
            dce_acl_entry_encode(h, entry);
        } ENCODE_ENDTRY;
    } RESET_ALLOC_FREE;

    idl_es_handle_free(&h, stp);

    /*
     * If the status of the encoding operation was bad, we report
     * that back. otherwise we report back the status of the
     * handle free operation.
     */
    if (BAD_STATUS(&est)) {
	*stp = est;
    }
}


/*
 * Desructor functions for decoded types
 */

PUBLIC void sec_acl_entry_decoded_data_free (
    void (*dealloc)(idl_void_p_t ptr),
    sec_acl_entry_t *entry
)
{
    if (entry) {
        switch (entry->entry_info.entry_type) {
        case sec_acl_e_type_user:
        case sec_acl_e_type_group:
        case sec_acl_e_type_foreign_other:
        case sec_acl_e_type_user_deleg:
        case sec_acl_e_type_group_deleg:
        case sec_acl_e_type_for_other_deleg:
            if (entry->entry_info.tagged_union.id.name)
                DEALLOCATE(dealloc, entry->entry_info.tagged_union.id.name);
            break;

        case sec_acl_e_type_foreign_user:
        case sec_acl_e_type_foreign_group:
        case sec_acl_e_type_for_user_deleg:
        case sec_acl_e_type_for_group_deleg:
            if (entry->entry_info.tagged_union.foreign_id.id.name)
                DEALLOCATE(dealloc, entry->entry_info.tagged_union.foreign_id.id.name);
            if (entry->entry_info.tagged_union.foreign_id.realm.name)
                DEALLOCATE(dealloc, entry->entry_info.tagged_union.foreign_id.realm.name);
            break;

        case sec_acl_e_type_extended:
            if (entry->entry_info.tagged_union.extended_info)
                DEALLOCATE(dealloc, entry->entry_info.tagged_union.extended_info);
            break;

        default:
            break;
        } /* switch entry_type */
    }
}
