/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_attr_util.c,v $
 * Revision 1.1.10.3  1996/02/18  00:22:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:22  marty]
 *
 * Revision 1.1.10.2  1995/12/08  18:02:33  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/01/11  20:38 UTC  mdf
 * 	comment
 * 
 * 	HP revision /main/mdf_MOTHRA_0/1  1995/01/11  20:37 UTC  mdf
 * 	Fixed defect.
 * 	[1995/12/08  17:22:59  root]
 * 
 * Revision 1.1.6.1  1994/10/17  18:18:30  cuti
 * 	Add low-level free routines.
 * 	[1994/10/14  16:16:40  cuti]
 * 
 * Revision 1.1.2.10  1994/09/09  20:08:18  pwang
 * 	Allocated memory accordingly in case of printstring_array having
 * 	empty data in sec_attr_util_alloc_copy() [OT#11978]
 * 	[1994/09/09  20:08:03  pwang]
 * 
 * Revision 1.1.2.9  1994/08/16  10:36:52  mdf
 * 	Bug fixes from Hewlett Packard
 * 	[1994/08/15  19:51:54  mdf]
 * 
 * Revision 1.1.2.8  1994/07/15  15:03:09  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:24:15  mdf]
 * 
 * Revision 1.1.2.5  1994/06/17  18:43:03  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:20:19  devsrc]
 * 
 * Revision 1.1.2.4  1994/06/06  14:46:37  rsalz
 * 	Add sec_attr_enc_printstring_array case and error-reporting default
 * 	to sec_attr_util_alloc_copy (OT CR 10864).
 * 	[1994/06/06  14:44:11  rsalz]
 * 
 * Revision 1.1.2.3  1994/06/02  21:17:55  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:20:51  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.2.2  1994/05/11  19:34:29  ahop
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  21:50:26  ahop]
 * 
 * $EndLog$
 */
/*
 * @(#)$RCSfile: sec_attr_util.c,v $ $Revision: 1.1.10.3 $ (OSF) $Date: 1996/02/18 00:22:45 $
 */
/*
** Copyright (c) Hewlett-Packard Company 1994
** Unpublished work. All Rights Reserved.
**
**      Attribute utility functions
*/

#include <stdlib.h>
#include <dce/lbase.h>
#include <macros.h>
#include <sec_encode.h>
#include <dce/sec_attr_util.h>
#include <dce/sec_attr_tools.h>


/* PRIVATE routines */

/*
 * f r e e _ a t t r _ p t r s
 *
 * Free every non-null pointer in the attribute instance pointed to
 * by sec_attr_p, using the dellocator function passed in deallocator.
 * Note that this data structure may have been only partially initialized.
 * The sec_attr_p pointer itself is NOT freed.
 *
 * INPUT
 *      deallocate  - memory deallocation function ptr
 *
 * INPUT/OUTPUT:
 *      sec_attr_p - [in] points to an allocated and potentially
 *              initialized sec_attr_t.
 *              [out] internal ptrs freed and set to NULL.
 *
 */
static void free_attr_ptrs (
    void        (*deallocate)(void *ptr),
    sec_attr_t	*sec_attr_p
)
{
    long		i, j;

    if(!sec_attr_p) {
	return;
    }

    switch(SA_ATTR_ENCODING(sec_attr_p)) {
    case sec_attr_enc_any:
    case sec_attr_enc_void:
    case sec_attr_enc_integer:
    case sec_attr_enc_uuid:
	break;
    case sec_attr_enc_printstring:
	if(SA_ATTR_PRINTSTRING_P(sec_attr_p)) {
	    (*deallocate)(SA_ATTR_PRINTSTRING_P(sec_attr_p));
	    SA_ATTR_PRINTSTRING_P(sec_attr_p) = NULL;
	}
	break;
    case sec_attr_enc_printstring_array:
	if(SA_ATTR_STR_ARRAY_P(sec_attr_p)) {
	    for(i = 0; i < SA_ATTR_STR_ARRAY_NUM(sec_attr_p); i++) {
		if(SA_ATTR_STR_ARRAY_ELT_P(sec_attr_p, i)) {
		    (*deallocate)(SA_ATTR_STR_ARRAY_ELT_P(sec_attr_p, i));
		}
	    }
	    (*deallocate)(SA_ATTR_STR_ARRAY_P(sec_attr_p));
	    SA_ATTR_STR_ARRAY_P(sec_attr_p) = NULL;
	}
	break;
    case sec_attr_enc_bytes:
    case sec_attr_enc_confidential_bytes:
	if(SA_ATTR_BYTES_P(sec_attr_p)) {
	    (*deallocate)(SA_ATTR_BYTES_P(sec_attr_p));
	    SA_ATTR_BYTES_P(sec_attr_p) = NULL;
	}
	break;
    case sec_attr_enc_i18n_data:
	if(SA_ATTR_IDATA_P(sec_attr_p)) {
	    (*deallocate)(SA_ATTR_IDATA_P(sec_attr_p));
	    SA_ATTR_IDATA_P(sec_attr_p) = NULL;
	}
	break;
    case sec_attr_enc_attr_set:
	if(SA_ATTR_SET_P(sec_attr_p)) {
	    (*deallocate)(SA_ATTR_SET_P(sec_attr_p));
	    SA_ATTR_SET_P(sec_attr_p) = NULL;
	}
	break;
    case sec_attr_enc_binding:
	if(SA_ATTR_BND_INFO_P(sec_attr_p)) {
	    if(SA_ATTR_BND_AUTH_INFO_TYPE(sec_attr_p) == 
		sec_attr_bind_auth_dce) {
		if(SA_ATTR_BND_AUTH_SVR_PNAME_P(sec_attr_p)) {
		    (*deallocate)(SA_ATTR_BND_AUTH_SVR_PNAME_P(sec_attr_p));
		}
	    }
	    for(i = 0; i < SA_ATTR_BND_NUM(sec_attr_p); i++) {
		switch(SA_ATTR_BND_TYPE(sec_attr_p, i)) {
		case sec_attr_bind_type_string:
		    if(SA_ATTR_BND_STRING_P(sec_attr_p, i)) {
			(*deallocate)(SA_ATTR_BND_STRING_P(sec_attr_p, i));
		    }
		    break;
		case sec_attr_bind_type_twrs:
		    if(SA_ATTR_BND_TWRSET_P(sec_attr_p, i)) {
	    		for(j = 0; 
			    j < SA_ATTR_BND_TWRSET_COUNT(sec_attr_p, i); 
			    j++) {
			    if(SA_ATTR_BND_TWR_P(sec_attr_p, i, j)) {
				(*deallocate)(SA_ATTR_BND_TWR_P(sec_attr_p, i, j));
			    }
			}
			(*deallocate)(SA_ATTR_BND_TWRSET_P(sec_attr_p, i));
		    }
		case sec_attr_bind_type_svrname:
		    if(SA_ATTR_BND_SVRNAME_P(sec_attr_p, i)) {
		        if(SA_ATTR_BND_SVRNAME_NAME_P(sec_attr_p, i)) {
		            (*deallocate)(SA_ATTR_BND_SVRNAME_NAME_P(sec_attr_p, i));
		        }
			(*deallocate)(SA_ATTR_BND_SVRNAME_P(sec_attr_p, i));
		    }
		    break;
		} /* end switch on bind_type */
	    } /* end for each binding */
	    (*deallocate)(SA_ATTR_BND_INFO_P(sec_attr_p));
	    SA_ATTR_BND_INFO_P(sec_attr_p) = NULL;
	}
	break;
    default:
	break;
    } /* end encoding type switch */
}

/* PUBLIC routines */

/* s e c _ a t t r _ u t i l _ a l l o c _ c o p y
 *
 * Allocate the necessary subfields of the destination sec_attr_t and
 * copy the corresponding data from the source sec_attr_t.
 */
void sec_attr_util_alloc_copy(
    idl_void_p_t (*allocate)(idl_size_t size),
    sec_attr_t      *from,
    sec_attr_t      *to,
    error_status_t  *stp
)
{
    int i;

    CLEAR_STATUS(stp);

    if (!allocate) {
        allocate = malloc_shim;
    }
    CLEAR_STATUS(stp);

    to->attr_id = from->attr_id;
    to->attr_value.attr_encoding = from->attr_value.attr_encoding;
    switch (from->attr_value.attr_encoding) {
    case sec_attr_enc_void:
        break;
    case sec_attr_enc_printstring:
        to->attr_value.tagged_union.printstring = (unsigned_char_p_t) allocate(
            (strlen((char *)from->attr_value.tagged_union.printstring)+1) * 
	    sizeof(unsigned_char_t));
	strcpy( (char *)to->attr_value.tagged_union.printstring,
                (char *)from->attr_value.tagged_union.printstring);
        break;
    case sec_attr_enc_printstring_array: {
	sec_attr_enc_str_array_t *tosa, *fromsa;

	fromsa = from->attr_value.tagged_union.string_array;
	tosa   =   to->attr_value.tagged_union.string_array =
	    (sec_attr_enc_str_array_t *)
		allocate(sizeof(*tosa)
		    + ((fromsa->num_strings) ? 
		       (fromsa->num_strings - 1) * sizeof(*tosa->strings) : 0));
	tosa->num_strings = fromsa->num_strings;
	/* copy each string */
	for (i = 0; i < tosa->num_strings; i++) {
	    tosa->strings[i] = allocate(strlen((char *)fromsa->strings[i]) + 1);
	    strcpy((char *)tosa->strings[i], (char *)fromsa->strings[i]);
	}
	break;
    }
    case    sec_attr_enc_integer:
        to->attr_value.tagged_union.signed_int =
            from->attr_value.tagged_union.signed_int;
        break;
    case    sec_attr_enc_bytes:
    case    sec_attr_enc_confidential_bytes:
        to->attr_value.tagged_union.bytes = (sec_attr_enc_bytes_t *)
            allocate(sizeof(sec_attr_enc_bytes_t) +
                from->attr_value.tagged_union.bytes->length *sizeof(idl_byte));
        to->attr_value.tagged_union.bytes->length =
            from->attr_value.tagged_union.bytes->length;
        (void) memcpy(to->attr_value.tagged_union.bytes->data,
                        from->attr_value.tagged_union.bytes->data,
                        from->attr_value.tagged_union.bytes->length);
        break;
    case    sec_attr_enc_i18n_data:
        to->attr_value.tagged_union.idata = (sec_attr_i18n_data_t *)
            allocate(sizeof(sec_attr_i18n_data_t) +
                from->attr_value.tagged_union.idata->length *sizeof(idl_byte));
        to->attr_value.tagged_union.idata->codeset =
            from->attr_value.tagged_union.idata->codeset;
        to->attr_value.tagged_union.idata->length =
            from->attr_value.tagged_union.idata->length;
        (void) memcpy(to->attr_value.tagged_union.idata->data,
                        from->attr_value.tagged_union.idata->data,
                        from->attr_value.tagged_union.idata->length);
        break;
    case sec_attr_enc_uuid:
        to->attr_value.tagged_union.uuid =
            from->attr_value.tagged_union.uuid;
        break;
    case    sec_attr_enc_attr_set:
        to->attr_value.tagged_union.attr_set = (sec_attr_enc_attr_set_t *)
            allocate(sizeof(sec_attr_enc_attr_set_t) +
                from->attr_value.tagged_union.attr_set->num_members *
                sizeof(uuid_t));
        to->attr_value.tagged_union.attr_set->num_members =
            from->attr_value.tagged_union.attr_set->num_members;
        (void) memcpy(to->attr_value.tagged_union.attr_set->members,
                        from->attr_value.tagged_union.attr_set->members,
                        (from->attr_value.tagged_union.attr_set->num_members *
			sizeof(uuid_t)));
        break;
    case    sec_attr_enc_binding:

	/* Allocate the binding storage */
        to->attr_value.tagged_union.binding = (sec_attr_bind_info_t *)
            allocate(sizeof(sec_attr_bind_info_t) +
                from->attr_value.tagged_union.binding->num_bindings *
                sizeof(sec_attr_binding_t));
        to->attr_value.tagged_union.binding->num_bindings =
            from->attr_value.tagged_union.binding->num_bindings;

        /* Copy the auth info */
        to->attr_value.tagged_union.binding->auth_info.info_type =
            from->attr_value.tagged_union.binding->auth_info.info_type;
        switch(from->attr_value.tagged_union.binding->auth_info.info_type){
        case sec_attr_bind_auth_none:
            break;
        case sec_attr_bind_auth_dce:
            to->attr_value.tagged_union.binding->auth_info.tagged_union.dce_info.protect_level = from->attr_value.tagged_union.binding->auth_info.tagged_union.dce_info.protect_level;
            to->attr_value.tagged_union.binding->auth_info.tagged_union.dce_info.authn_svc = from->attr_value.tagged_union.binding->auth_info.tagged_union.dce_info.authn_svc;
            to->attr_value.tagged_union.binding->auth_info.tagged_union.dce_info.authz_svc = from->attr_value.tagged_union.binding->auth_info.tagged_union.dce_info.authz_svc;
            to->attr_value.tagged_union.binding->auth_info.tagged_union.dce_info.svr_princ_name = (unsigned_char_p_t) allocate(sizeof(unsigned_char_t) *
              (strlen((char *)from->attr_value.tagged_union.binding->auth_info.tagged_union.dce_info.svr_princ_name)+1));
            strcpy((char *)to->attr_value.tagged_union.binding->auth_info.tagged_union.dce_info.svr_princ_name, (char *)from->attr_value.tagged_union.binding->auth_info.tagged_union.dce_info.svr_princ_name);
        }

        /* Copy each binding */
        for (i=0; i<from->attr_value.tagged_union.binding->num_bindings; i++) {
          to->attr_value.tagged_union.binding->bindings[i].bind_type =
            from->attr_value.tagged_union.binding->bindings[i].bind_type;
          switch(from->attr_value.tagged_union.binding->bindings[i].bind_type){
          case sec_attr_bind_type_string:
            to->attr_value.tagged_union.binding->bindings[i].tagged_union.string_binding =
              (unsigned_char_p_t) allocate((strlen((char *)from->attr_value.tagged_union.binding->bindings[i].tagged_union.string_binding)+1) * sizeof(unsigned_char_t));
            strcpy((char *)to->attr_value.tagged_union.binding->bindings[i].tagged_union.string_binding, (char *)from->attr_value.tagged_union.binding->bindings[i].tagged_union.string_binding);
            break;
          case sec_attr_bind_type_twrs:
            SET_STATUS(stp, sec_rgy_not_implemented);
            return;
            break;
          case sec_attr_bind_type_svrname:
            to->attr_value.tagged_union.binding->bindings[i].tagged_union.svrname = (sec_attr_bind_svrname *) allocate(sizeof(sec_attr_bind_svrname));
            to->attr_value.tagged_union.binding->bindings[i].tagged_union.svrname->name_syntax = from->attr_value.tagged_union.binding->bindings[i].tagged_union.svrname->name_syntax;
            to->attr_value.tagged_union.binding->bindings[i].tagged_union.svrname->name = (unsigned_char_p_t) allocate((strlen((char *)from->attr_value.tagged_union.binding->bindings[i].tagged_union.svrname->name)+1) * sizeof(char));
            strcpy((char *)to->attr_value.tagged_union.binding->bindings[i].tagged_union.svrname->name, (char *)from->attr_value.tagged_union.binding->bindings[i].tagged_union.svrname->name);
            break;
          }
	}
        break;
    default:
            SET_STATUS(stp, sec_rgy_not_implemented);
    }
}


/*
 * s e c _ a t t r _ u t i l _  f r e e 
 *
 * Free all underlying storage in a sec_attr_t. 
 * The sec_attr_t itself is NOT freed.
 * 
 * If deallocaor is NULL, the appropriate
 * system deallocator function is used
 * (free()).
 *
 * INPUT:
 *       deallocate - memory deallocation function ptr
 *
 * INPUT/OUTPUT:
 *	sec_sch_entry_p - [in] points to an allocated and potentially
 *		initialized sec_attr_schema_entry_t.
 *		[out] internal ptrs freed.
 *
 */
void sec_attr_util_free(
    void (*deallocate)(void *ptr),
    sec_attr_t      *attr
)
{
    if (deallocate == NULL) {
        deallocate = free;
    }

    if (attr) {
	free_attr_ptrs(deallocate, attr);
    }

}


/*
 * s e c _ a t t r _ u t i l _ s c h _ f r e e _ a c l _ m g r s
 *
 * Free every non-null pointer in the acl_mgr_set field of
 * the schema entry pointed to by sec_sch_entry_p.
 * Set sec_sch_entry_p->acl_mgr_set to NULL.
 *
 * INPUT/OUTPUT:
 *	sec_sch_entry_p - [in] points to an allocated and potentially
 *		initialized sec_attr_schema_entry_t.
 *		[out] acl_mgr_set ptrs freed and set to NULL.
 *
 */
void sec_attr_util_sch_free_acl_mgrs(
    sec_attr_schema_entry_t	*sec_sch_entry_p
)
{
    long		i;

    if(!sec_sch_entry_p) {
	return;
    }
    if(sec_sch_entry_p->acl_mgr_set) {
	for(i = 0; i < SA_ACL_MGR_NUM(sec_sch_entry_p); i++) {
	    if(SA_ACL_MGR_INFO_P(sec_sch_entry_p, i)) {
		free(SA_ACL_MGR_INFO_P(sec_sch_entry_p, i));
	    }
	}
	free(sec_sch_entry_p->acl_mgr_set);
	sec_sch_entry_p->acl_mgr_set = NULL;
    }
}


/*
 * s e c _ a t t r _ u t i l _ s c h _ f r e e _ b i n d i n g
 *
 * Free every non-null pointer in the trig_binding field of
 * the schema entry pointed to by sec_sch_entry_p.
 * Set sec_sch_entry_p->trig_binding to NULL.
 *
 * INPUT/OUTPUT:
 *	sec_sch_entry_p - [in] points to an allocated and potentially
 *		initialized sec_attr_schema_entry_t.
 *		[out] trig_binding ptrs freed and set to NULL.
 *
 */
void sec_attr_util_sch_free_binding(
    sec_attr_schema_entry_t	*sec_sch_entry_p
)
{
    long		i, j;

    if(!sec_sch_entry_p) {
	return;
    }
    if(sec_sch_entry_p->trig_binding) {
	if(SA_TRG_BND_AUTH_INFO_TYPE(sec_sch_entry_p) == 
	    sec_attr_bind_auth_dce) {
	    if(SA_TRG_BND_AUTH_SVR_PNAME_P(sec_sch_entry_p)) {
		free(SA_TRG_BND_AUTH_SVR_PNAME_P(sec_sch_entry_p));
	    }
	}
	for(i = 0; i < SA_TRG_BND_NUM(sec_sch_entry_p); i++) {
	    switch(SA_TRG_BND_TYPE(sec_sch_entry_p, i)) {
	    case sec_attr_bind_type_string:
		if(SA_TRG_BND_STRING_P(sec_sch_entry_p, i)) {
		    free(SA_TRG_BND_STRING_P(sec_sch_entry_p, i));
		}
		break;
	    case sec_attr_bind_type_twrs:
		if(SA_TRG_BND_TWRSET_P(sec_sch_entry_p, i)) {
		    for(j = 0; 
			j < SA_TRG_BND_TWRSET_COUNT(sec_sch_entry_p, i);
			j++) {
			if(SA_TRG_BND_TWR_P(sec_sch_entry_p, i, j)) {
			    free(SA_TRG_BND_TWR_P(sec_sch_entry_p, i, j));
			}
		    }
		    free(SA_TRG_BND_TWRSET_P(sec_sch_entry_p, i));
		}
		break;
	    case sec_attr_bind_type_svrname:
		if(SA_TRG_BND_SVRNAME_P(sec_sch_entry_p, i)) {
		    if(SA_TRG_BND_SVRNAME_NAME_P(sec_sch_entry_p, i)) {
		        free(SA_TRG_BND_SVRNAME_NAME_P(sec_sch_entry_p, i));
		    }
		    free(SA_TRG_BND_SVRNAME_P(sec_sch_entry_p, i));
		}
		break;
	    default:
		break;
	    } /* end switch */
	}
	free(sec_sch_entry_p->trig_binding);
	sec_sch_entry_p->trig_binding = NULL;
    }
}


/*
 * s e c _ a t t r _ u t i l _ s c h _ e n t _ f r e e _ p t r s
 *
 * Free every non-null pointer in the schema entry pointed to
 * by sec_sch_entry_p.  Note that this data structure may
 * have been only partially initialized.  The sec_sch_entry_p
 * pointer itself is NOT freed.
 *
 * INPUT/OUTPUT:
 *	sec_sch_entry_p - [in] points to an allocated and potentially
 *		initialized sec_attr_schema_entry_t.
 *		[out] internal ptrs freed.
 *
 */
void sec_attr_util_sch_ent_free_ptrs(
    sec_attr_schema_entry_t	*sec_sch_entry_p
)
{
    if(!sec_sch_entry_p) {
	return;
    }

    if(sec_sch_entry_p->attr_name) {
	free(sec_sch_entry_p->attr_name);
	sec_sch_entry_p->attr_name = NULL;
    }

    sec_attr_util_sch_free_acl_mgrs(sec_sch_entry_p);

    sec_attr_util_sch_free_binding(sec_sch_entry_p);

    if(sec_sch_entry_p->scope) {
	free(sec_sch_entry_p->scope);
	sec_sch_entry_p->scope = NULL;
    }

    if(sec_sch_entry_p->comment) {
	free(sec_sch_entry_p->comment);
	sec_sch_entry_p->comment = NULL;
    }
}


/*
 * s e c _ a t t r _ u t i l _ s c h _ e n t _ f r e e 
 *
 * Free every non-null pointer in the schema entry pointed to
 * by sec_sch_entry_p.  Note that this data structure may
 * have been only partially initialized.  
 * Also free sec_sch_entry_p.
 *
 * INPUT/OUTPUT:
 *	sec_sch_entry_p - [in] points to an allocated and partially
 *		or fully initialized sec_attr_schema_entry_t.
 *		[out] all internal ptrs and self freed.
 *
 */
void sec_attr_util_sch_ent_free(
    sec_attr_schema_entry_t	**sec_sch_entry_p
)
{
    if(*sec_sch_entry_p) {
	sec_attr_util_sch_ent_free_ptrs(*sec_sch_entry_p);
	free(*sec_sch_entry_p);
	*sec_sch_entry_p = NULL;
    }
}


/*
 * s e c _ a t t r _ u t i l _ i n s t _ f r e e _ p t r s
 *
 * Free every non-null pointer in the attribute instance pointed to
 * by sec_attr_p.  Note that this data structure may
 * have been only partially initialized.  The sec_attr_p
 * pointer itself is NOT freed.
 *
 * INPUT/OUTPUT:
 *      sec_attr_p - [in] points to an allocated and potentially
 *              initialized sec_attr_t.
 *              [out] internal ptrs freed and set to NULL.
 *
 */
void sec_attr_util_inst_free_ptrs(
    sec_attr_t			*sec_attr_p
)
{
    if(!sec_attr_p) {
	return;
    }
    free_attr_ptrs(free, sec_attr_p);
}


/*
 * s e c _ a t t r _ u t i l _ i n s t _ f r e e
 *
 * Free every non-null pointer in the attribute instance pointed to
 * by sec_attr_p.  Note that this data structure may
 * have been only partially initialized.  
 * Also free *sec_attr_p and set it to NULL.
 *
 * INPUT/OUTPUT:
 *	sec_attr_p - [in] points to an allocated and partially
 *		or fully initialized sec_attr_t.
 *		[out] all internal ptrs and self freed.
 *
 */
void sec_attr_util_inst_free(
    sec_attr_t			**sec_attr_p
)
{
    if(*sec_attr_p) {
	sec_attr_util_inst_free_ptrs(*sec_attr_p);
	free(*sec_attr_p);
	*sec_attr_p = NULL;
    }
}

