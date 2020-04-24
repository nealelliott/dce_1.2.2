/*
 * @@OSF_COPYRIGHT@@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_attr_schema.c,v $
 * Revision 1.1.11.3  1996/02/18  00:21:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:58  marty]
 *
 * Revision 1.1.11.2  1995/12/08  17:58:44  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/10/30  19:46 UTC  psn
 * 	Fix OSF OT 13160, CHFts16737
 * 	[1995/10/30  19:41 UTC  psn  /main/HPDCE02/psn_ot13160/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/09  11:46 UTC  mdf
 * 	Fixes defect.
 * 
 * 	HP revision /main/mdf_mothra_defects_0/1  1995/03/07  19:38 UTC  mdf
 * 	Closes CHFts14570 and CHFts14569.
 * 	[1995/12/08  17:22:07  root]
 * 
 * Revision 1.1.7.1  1994/10/17  18:18:26  cuti
 * 	Submit for Ann
 * 	[1994/10/14  16:16:28  cuti]
 * 
 * Revision 1.1.5.2  1994/09/23  23:24:15  ahop
 * 	implement rsdb_attr_sch_replace, rsdb_attr_sch_change_name_key,
 * 	clean up rsdb_attr_sch_get_id_by_name and rsdb_attr_sch_store_new
 * 	if trig_binding not present, don't alloc trig_binding for export
 * 
 * Revision 1.1.2.7  1994/08/08  14:23:46  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/04  19:24:10  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, fixed botched merge.
 * 
 * Revision 1.1.2.2  1994/05/11  19:32:54  ahop
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  21:47:59  ahop]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */


#include <dce/dce.h>
#include <dce/uuid.h>
#include <dce/sec_attr_util.h>		/* prototypes */
#include <dce/sec_attr_tools.h>		/* macros */
#include <dce/rs_attr_base.h>		/* types */

#include <stdio.h>
#include <rsdb_pvt.h>
#include <rsdb_attr_schema.h>		/* prototypes */
#include <rsdb_attr_pvt.h>		/* macros */
#include <rsdb.h>
#include <rsdb_name_util.h>
#include <rsdb_util.h>
#include <rs_util.h>
#include <rs_state.h>
#include <bstring.h>
#include <sec_svc.h>

/*
 * Local Global Variables
 */

PRIVATE rsdb_attr_schema_entry_t	*first = NULL;
PRIVATE rsdb_attr_schema_entry_t	*last = NULL;

/* Values for rsdb_attr_sch_flags_t:
 * Internal rsdb sch_entry flags for use by the back-end
 * schema entry import/export and storage code.
 */
PRIVATE unsigned32    rsdb_attr_sch_flag_none             = 0x00000000;
/* rsdb_attr_sch_flag_trig_binding:
 *      If SET, indicates that a trig_binding value
 *      is stored in this rsdb_attr_schema_entry_t.
 *      If NOT SET, a trig_binding is not present.
 */
PRIVATE unsigned32    rsdb_attr_sch_flag_trig_binding     = 0x00000001;


/*
 * Macros 
 */

#define RSDB_SCH_CUR_NUM_ENTRIES	(first->threads.alias)

#define SET_THREADS(item, id_value, next_and_prev)			\
	(item)->threads.id = (id_value);				\
	(item)->threads.next = (item)->threads.prev = (next_and_prev)

/* 
 * SCH_ENTRY_ALLOC: malloc P of length L.
 * If successful, memset buffer to zero.
 * Else, free all previously malloc-ed ptrs in S and return.
 * Calling routine should set "allocator" to rpc_ss_allocate
 * if the memory must be freed automatically by the server stub code.
 *
 * S is a pointer to a sec_attr_schema_entry_t
 * P is a pointer to be malloc-ed
 * L is the length to be malloc-ed
 * STP is a pointer to an error_status_t
 */
#define SCH_ENTRY_ALLOC(S,P,L,STP)	{		\
    if(!((P) = RSDB_ATTR_ALLOCATE(allocator,(L)))) {	\
	SET_STATUS(STP, sec_attr_no_memory);		\
	if(!(allocator)) {				\
	    sec_attr_util_sch_ent_free_ptrs((S));	\
	}						\
	return;						\
    } else {						\
	memset((P), NULL, (L));				\
    } }
	

/* 
 * PRIVATE OPERATIONS
 */

/*
 * If "prev" is true, set threads.prev of schema_entry 
 * identified by sch_entry_id to new_id.
 * If "prev" is false, set threads.next of schema_entry 
 * identified by sch_entry_id to new_id.
 */
PRIVATE void rsdb_attr_sch_reset(
    rsdb_pvt_id_t	sch_entry_id,
    rsdb_pvt_id_t	new_id,
    boolean32	    	prev,
    error_status_t	*st_p)
{
    rsdb_sequential_key_t	sequential_key;
    Pointer			data;
    int				data_len;
    rsdb_attr_schema_entry_t	*sch_entry_p;
    error_status_t		status;

    /* 
     * Update the global "first" or "last" if necessary.
     */
    if (prev) {
	if (sch_entry_id == last->threads.id) {
	    last->threads.prev = new_id;
	}
    } else {
	if (sch_entry_id == first->threads.id) {
	   first->threads.next = new_id; 
	}
    } 
    memset((Pointer) &sequential_key, NULL, sizeof(sequential_key)); 
    sequential_key.record_type = SEQUENTIAL_TYPE; 
    sequential_key.id = sch_entry_id; 

    rsdb_fetch(rsdb_attr_schema_db, (Pointer) &sequential_key, 
		sizeof(sequential_key), &data, &data_len, st_p); 

    if (BAD_STATUS(st_p)) {
        if (prev) {
            dce_svc_printf(SEC_ATTR_SCHEMA_CANT_RESET_MSG);
	} else {
            dce_svc_printf(SEC_ATTR_SCHEMA_CANT_RESET_MSG);
	}
    }

    /* Modify the threads field right in the database memory;
     * thus, no rsdb_store is required.
     */
    sch_entry_p = (rsdb_attr_schema_entry_t *)data; 
    if (prev) {
    	sch_entry_p->threads.prev = new_id; 
    } else {
    	sch_entry_p->threads.next = new_id; 
    }
}

/*
 * r s d b _ a t t r _ s c h _ u p d _ n u m _ e n t r i e s
 *
 * Update the stored "current number of schema entries."
 * If increment is TRUE, add one to the current number of entries.
 * If increment is FALSE, decrement the current number of entries.
 * Save the "first" dummy schema entry where this info is stored.
 * 
 */
PRIVATE void rsdb_attr_sch_upd_num_entries(
    boolean32	    	increment,
    error_status_t	*st_p)
{
    rsdb_attr_schema_entry_t	*sch_entry_p;
    rsdb_pvt_id_t		sch_entry_id;

    /* Modify the current number of entries in local copy 
     * and stored copy.  RSDB_SCH_CUR_NUM_ENTRIES is a field
     * in "first."
     */
    if(increment) {
	RSDB_SCH_CUR_NUM_ENTRIES += 1;
    } else {
	RSDB_SCH_CUR_NUM_ENTRIES -= 1;
    }
    rsdb_attr_sch_store_by_id(first->threads.id, first, st_p);
}



/* 
 * r s d b _ a t t r _ s c h _ p u r g e
 * 
 * Delete the schema entry and its key records
 * from the attr_schema database.
 *
 * Status Codes:
 * 	sec_attr_sch_entry_not_found 	Could not find schema entry.
 *	error_status_ok
 */

PRIVATE void rsdb_attr_sch_purge (
    rsdb_attr_schema_entry_t    *sch_entry_p,
    error_status_t	*st_p)
{
    rsdb_uid_key_t		uuid_key;
    rsdb_attr_name_key_t	attr_name_key;
    rsdb_sequential_key_t	sequential_key;

    memset((Pointer) &uuid_key, NULL,       sizeof(uuid_key));
    memset((Pointer) &attr_name_key, NULL,  sizeof(attr_name_key));
    memset((Pointer) &sequential_key, NULL, sizeof(sequential_key));

    /*
     * Keys
     */

    sequential_key.record_type 	= SEQUENTIAL_TYPE;
    sequential_key.id 		= sch_entry_p->threads.id;

    uuid_key.record_type	= UUID_TYPE;
    uuid_key.uid		= sch_entry_p->attr_id;

    attr_name_key.record_type	= ATTR_NAME_TYPE;
    attr_name_key.name_len	= sch_entry_p->attr_name_len;

    u_strncpy(attr_name_key.name, sch_entry_p->buf, 
		sch_entry_p->attr_name_len);

    rsdb_delete(rsdb_attr_schema_db, (Pointer) &sequential_key,
    	sizeof(sequential_key), st_p);

    if(GOOD_STATUS(st_p)) {
	rsdb_delete(rsdb_attr_schema_db, (Pointer) &uuid_key,
    		sizeof(uuid_key), st_p);
    }

    if(GOOD_STATUS(st_p)) {
	rsdb_delete(rsdb_attr_schema_db, (Pointer) &attr_name_key,
    		NAME_LEN(&attr_name_key), st_p);
    }
}


/* r s d b _ a t t r _ s c h _ a l l o c _ d b _ e n t r y
 *
 * Malloc an rsdb_attr_schema_entry_t buffer large enough to 
 * hold the fixed and variable length parts of the input 
 * sec_attr_schema_entry_t, pointed to by sec_sch_entry_p.
 * Set the schema entry length field in db_sch_entry_p.
 * Buffer allocated for db_sch_entry_p must be freed by caller.
 *
 * INPUT:
 *	sec_sch_entry_p - points to a sec_attr_schema_entry to
 *		be "measured for size."  It is assumed that this
 *		schema entry has already been checked for
 *		semantic correctness.
 * OUTPUT:
 *	db_sch_entry_p - an allocated buffer large enough to
 *		contain the input schema entry in rsdb format.
 *
 * Status Codes:
 *      sec_attr_no_memory    if malloc fails.
 *      error_status_ok                 otherwise.
 */
PRIVATE void rsdb_attr_sch_alloc_db_entry
    (
    sec_attr_schema_entry_t	*sec_sch_entry_p,	/* [in] */
    rsdb_attr_schema_entry_t	**db_sch_entry_p,	/* [out] */
    error_status_t		*st_p
    )
{
    int                 i, j;
    unsigned32		sch_entry_len;
    Pointer		new_buf;

    CLEAR_STATUS(st_p);
    *db_sch_entry_p = (rsdb_attr_schema_entry_t *)NULL;

    rsdb_attr_sch_calc_size(sec_sch_entry_p, &sch_entry_len, st_p);
    if (BAD_STATUS(st_p)) return;

    if(!(new_buf = malloc(sch_entry_len))) {
        SET_STATUS(st_p, sec_attr_no_memory);
        return;
    }
    *db_sch_entry_p = (rsdb_attr_schema_entry_t *) new_buf;
    memset((Pointer)(*db_sch_entry_p), NULL, sch_entry_len);
    /* store total length of db_sch_entry_p buffer. */
    (*db_sch_entry_p)->sch_entry_len = sch_entry_len;

    return;
}


/*
 * r s d b _ a t t r _ s c h _ c h a n g e _ n a m e _ k e y
 *
 * Retrieve the sequential_key stored with the key of the
 * old attr_name.  Store the sequential_key with the new attr_name 
 * and delete the old attr_name->sequential_key record.
 *
 * Status Codes:
 *	sec_attr_sch_entry_not_found	 if sch_entry not found 
 */
PRIVATE void rsdb_attr_sch_change_name_key (
	char			*old_attr_name,		/* [in] */
	char			*new_attr_name,		/* [in] */
	error_status_t 		*st_p)
{
    rsdb_attr_name_key_t        attr_name_key;
    Pointer			data;
    int				data_len;
    rsdb_pvt_id_t		sch_entry_id;

    CLEAR_STATUS(st_p);

    memset((Pointer) &attr_name_key, NULL, sizeof(attr_name_key));

    attr_name_key.record_type   = ATTR_NAME_TYPE;
    attr_name_key.name_len      = u_strlen(old_attr_name);
    u_strncpy(attr_name_key.name, old_attr_name, attr_name_key.name_len);

    rsdb_fetch(rsdb_attr_schema_db, (Pointer) &attr_name_key,
	NAME_LEN(&attr_name_key), &data, &data_len, st_p);

    if (BAD_STATUS(st_p)) {
	SET_STATUS(st_p, sec_attr_sch_entry_not_found);
	return;
    }

    /* save the schema entry id */
    memcpy(&sch_entry_id, data, data_len);

    /* delete the old record */
    rsdb_delete(rsdb_attr_schema_db, (Pointer) &attr_name_key,
    	NAME_LEN(&attr_name_key), st_p);

    /* Now store the new record */
    attr_name_key.name_len = u_strlen(new_attr_name);
    u_strncpy(attr_name_key.name, new_attr_name, attr_name_key.name_len);

    rsdb_store(rsdb_attr_schema_db, (Pointer) &attr_name_key,
    	NAME_LEN(&attr_name_key), (Pointer) &sch_entry_id,
    	sizeof(sch_entry_id), st_p);
}


/* r s d b _ a t t r _ s c h _ c a l c _ s i z e
 *
 * Calculate and return the size required for an 
 * rsdb_attr_schema_entry_t buffer large enough to 
 * hold the fixed and variable length parts of the input 
 * sec_attr_schema_entry_t, pointed to by sec_sch_entry_p.
 *
 * INPUT:
 *	sec_sch_entry_p - points to a sec_attr_schema_entry to
 *		be "measured for size."  It is assumed that this
 *		schema entry has already been checked for
 *		semantic correctness.
 * OUTPUT:
 *	size - number of bytes required for a buffer to store
 *		the data from the input schema entry in 
 *		an rsdb_attr_schema_entry_t.
 *
 * Status Codes:
 *      sec_attr_no_memory    if malloc fails.
 *      error_status_ok                 otherwise.
 */
PUBLIC void rsdb_attr_sch_calc_size
    (
    sec_attr_schema_entry_t	*sec_sch_entry_p,	/* [in] */
    unsigned32			*size,			/* [out] */
    error_status_t		*st_p
    )
{
    int                 i, j;
    unsigned32		sch_entry_len;

    CLEAR_STATUS(st_p);
    *size = 0;

    /* Note: Subtract 1 since one char
     * is already accounted for in the buf[VARYING]
     * field of rsdb_attr_schema_entry_t
     */
    sch_entry_len = sizeof(rsdb_attr_schema_entry_t) - 1;
    sch_entry_len += u_strlen(sec_sch_entry_p->attr_name);

    if(!(SA_ACL_MGR_SET_P(sec_sch_entry_p))) {
	SET_STATUS(st_p, sec_attr_bad_acl_mgr_set);
	return;
    }
    sch_entry_len += ( (SA_ACL_MGR_NUM(sec_sch_entry_p)) *
	sizeof(*(SA_ACL_MGR_INFO_P(sec_sch_entry_p, 0))) );

    if(SA_TRG_BND_INFO_P(sec_sch_entry_p)) {
	if(SA_TRG_BND_AUTH_INFO_TYPE(sec_sch_entry_p) == 
	    sec_attr_bind_auth_dce) {
	    sch_entry_len += 
		u_strlen(SA_TRG_BND_AUTH_SVR_PNAME_P(sec_sch_entry_p));
	}

	for(i = 0; i < SA_TRG_BND_NUM(sec_sch_entry_p); i++) {
	    sch_entry_len += sizeof(SA_TRG_BND_TYPE(sec_sch_entry_p, i));
	    switch(SA_TRG_BND_TYPE(sec_sch_entry_p, i)) {
	    case sec_attr_bind_type_string:
		sch_entry_len += sizeof(unsigned32); /* store string length */
		sch_entry_len += 
		    u_strlen(SA_TRG_BND_STRING_P(sec_sch_entry_p, i));
		break;
	    case sec_attr_bind_type_twrs:
		if(SA_TRG_BND_TWRSET_P(sec_sch_entry_p, i)) {
		    sch_entry_len += 
		        sizeof(SA_TRG_BND_TWRSET_COUNT(sec_sch_entry_p, i));
		    for(j = 0; 
		        j < SA_TRG_BND_TWRSET_COUNT(sec_sch_entry_p, i); j++) {
			/* store tower_length */
		        sch_entry_len += sizeof(unsigned32); 
		        sch_entry_len += 
			    SA_TRG_BND_TWR_LEN(sec_sch_entry_p, i, j);
		    }
		}
		break;
	    case sec_attr_bind_type_svrname:
		if(SA_TRG_BND_SVRNAME_P(sec_sch_entry_p, i)) {
		    sch_entry_len += 
			sizeof(SA_TRG_BND_SVRNAME_SYNTAX(sec_sch_entry_p, i));
		    sch_entry_len += sizeof(unsigned32); /* store string length */
		    sch_entry_len += 
		        u_strlen(SA_TRG_BND_SVRNAME_NAME_P(sec_sch_entry_p, i));
		}
		break;
	    } /* end switch */
	}
    }
    sch_entry_len += u_strlen(sec_sch_entry_p->scope);
    sch_entry_len += u_strlen(sec_sch_entry_p->comment);

    *size = sch_entry_len;
    return;
}


/* 
 * r s d b _ a t t r _ s c h _ c r e a t e
 *
 *  Create and store the schema entry list head and tail.
 *  This routine either succeeds or fails with fatal error.
 *  This operation is called in sec_create_db when creating
 *  the initial database; in this case, first and last are NULL.
 *  It may also be called in secd during re-initialization of
 *  a slave, in which case, first and last are already allocated.
 *
 */
PUBLIC void rsdb_attr_sch_create (
    void )
{
    size_t		length;
    error_status_t	st;

    /*
     * Instead of calling sizeof multiple times, just do it once
     * instead, for performace.
     */

    length = sizeof(rsdb_attr_schema_entry_t);

    /*
     * If the two global variables first, and last are 
     * equal to NULL then allocate space for them.
     */

    if(first == NULL && last == NULL) {
	MALLOC_OR_FATAL(rsdb_attr_schema_entry_t, first, length);
	MALLOC_OR_FATAL(rsdb_attr_schema_entry_t, last, length);
    }

    memset((Pointer) first, NULL, length);
    memset((Pointer) last,  NULL, length);

    SET_THREADS(first, START_OF_LIST, END_OF_LIST); /* See macro up above */
    SET_THREADS( last, END_OF_LIST, START_OF_LIST);

    first->sch_entry_len = last->sch_entry_len = length;

    rsdb_attr_sch_store_by_id(first->threads.id, first, &st);
    rsdb_attr_sch_store_by_id(last->threads.id, last, &st);
}


/* 
 * r s d b _ a t t r _ s c h _ i n i t
 *
 * Retrieve from database the schema entry list head and tail.
 * This routine either succeeds or fails with fatal error.
 */
PUBLIC void rsdb_attr_sch_init (
    void )
{
    error_status_t	st;

    if(!rsdb_attr_sch_get_by_id(START_OF_LIST, &first, &st)) {
        dce_svc_printf(SEC_ATTR_SCHEMA_CANT_LOOKUP_MSG);
    }
    if(!rsdb_attr_sch_get_by_id(END_OF_LIST, &last, &st)) {
        dce_svc_printf(SEC_ATTR_SCHEMA_CANT_LOOKUP_MSG);
    }
}


/*
 * r s d b _ a t t r _ s c h _ s t o r e _ b y _ i d 
 *
 * Store the input rsdb_attr_schema_entry_t in the ATTR_SCHEMA database.
 */
PUBLIC void rsdb_attr_sch_store_by_id(
    rsdb_pvt_id_t   		sch_entry_id,
    rsdb_attr_schema_entry_t	*sch_entry_p,
    error_status_t	  	*st_p)
{
    rsdb_sequential_key_t   sequential_key;

    memset((Pointer) &sequential_key, NULL, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = sch_entry_id;

    rsdb_replace(rsdb_attr_schema_db, (Pointer) &sequential_key,
	sizeof(sequential_key), (Pointer) sch_entry_p, 
	sch_entry_p->sch_entry_len, st_p);
}


/*
 * r s d b _ a t t r _ s c h _ g e t _ i d _ b y _ u u i d 
 *
 * Retrieve the sch_entry_id_p (sequential id) using the 
 * input attr_id as the key.
 * Return true if sch_entry_id id is found, false otherwise.
 *
 * Status Codes:
 *	sec_attr_sch_entry_not_found	 if seqential id not found
 */
PUBLIC boolean32 rsdb_attr_sch_get_id_by_uuid (
	uuid_t 			*attr_id_p,		/* [in] */
	rsdb_pvt_id_t		*sch_entry_id_p,	/* [out] */
	error_status_t 		*st_p)
{
    Pointer			data;
    int				data_len;
    rsdb_uid_key_t              uuid_key;

    CLEAR_STATUS(st_p);
    memset((Pointer) &uuid_key, NULL, sizeof(uuid_key));

    uuid_key.record_type        = UUID_TYPE;
    uuid_key.uid                = *attr_id_p;

    rsdb_fetch(rsdb_attr_schema_db, (Pointer) &uuid_key,
		sizeof(uuid_key), &data, 
		&data_len, st_p);

    if (BAD_STATUS(st_p)) {
	SET_STATUS(st_p, sec_attr_sch_entry_not_found);
	return false;
    }
    memcpy(sch_entry_id_p, data, data_len);
    return true;
}


/*
 * r s d b _ a t t r _ s c h _ g e t _ i d _ b y _ n a m e
 *
 * Retrieve the sch_entry_id_p (sequential id) using the 
 * input attr_name as the key.
 * Return true if sch_entry_id id is found, false otherwise.
 *
 * Status Codes:
 *	sec_attr_sch_entry_not_found	 if sch_entry not found 
 */
PUBLIC boolean32 rsdb_attr_sch_get_id_by_name (
	char			*attr_name,		/* [in] */
	rsdb_pvt_id_t		*sch_entry_id_p,	/* [out] */
	error_status_t 		*st_p)
{
    rsdb_attr_name_key_t        attr_name_key;
    Pointer			data;
    int				data_len;

    CLEAR_STATUS(st_p);

    memset((Pointer) &attr_name_key, NULL, sizeof(attr_name_key));

    attr_name_key.record_type   = ATTR_NAME_TYPE;
    attr_name_key.name_len      = u_strlen(attr_name);
    u_strncpy(attr_name_key.name, attr_name,
                attr_name_key.name_len);

    rsdb_fetch(rsdb_attr_schema_db, (Pointer) &attr_name_key,
		NAME_LEN(&attr_name_key), &data, &data_len, st_p);

    if (BAD_STATUS(st_p)) {
	SET_STATUS(st_p, sec_attr_sch_entry_not_found);
	return false;
    }
    memcpy(sch_entry_id_p, data, data_len);
    return true;
}


/*
 * r s d b _ a t t r _ s c h _ g e t _ b y _ i d 
 *
 * Retrieve the sch_entry using the input sch_entry_id as the key.
 * Return true if sch_entry found, false otherwise.
 * sch_entry_p must be freed by caller.
 *
 * Status Codes:
 *	sec_attr_sch_entry_not_found	 if entry not found with sch_entry_id
 *	sec_attr_no_memory   if malloc fails.
 */
PUBLIC boolean32 rsdb_attr_sch_get_by_id (
	rsdb_pvt_id_t 			sch_entry_id,
	rsdb_attr_schema_entry_t 	**sch_entry_p,
	error_status_t 			*st_p)
{
    rsdb_sequential_key_t	sequential_key;
    Pointer		 	data;
    int				data_len;

    CLEAR_STATUS(st_p);
    memset((Pointer) &sequential_key, NULL, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = sch_entry_id;

    rsdb_fetch(rsdb_attr_schema_db, (Pointer) &sequential_key,
		sizeof(sequential_key), &data, &data_len, st_p);

    if (BAD_STATUS(st_p)) {
	SET_STATUS(st_p, sec_attr_sch_entry_not_found);
	*sch_entry_p = (rsdb_attr_schema_entry_t *)NULL;
	return false;
    }
    if(!(*sch_entry_p = (rsdb_attr_schema_entry_t *)malloc(data_len))) {
	SET_STATUS(st_p, sec_attr_no_memory);
	return false;
    }
    memcpy((Pointer)(*sch_entry_p), data, data_len);
    return true;
}


/*
 * r s d b _ a t t r _ s c h _ g e t _ e x p o r t _ b y _ i d
 *
 * Return the sec_attr_schema_entry_t for the input 
 * sch_entry_id.  This call bypasses the malloc and memcpy 
 * of the rsdb_attr_schema_entry_t that is done in 
 * rsdb_attr_sch_get_by_id when an exported copy is really desired.
 *
 * Retrieve the sch_entry using the input sch_entry_id as the key.
 * Export the data to a sec_sch_entry_p.
 * Return true if sch_entry found, false otherwise.
 *
 * If allocator is not rpc_ss_allocate, caller 
 * must free allocated buffers by calling
 * sec_attr_util_sch_ent_free_ptrs(sec_sch_entry_p) when done.
 *
 * INPUT:
 *      sch_entry_id - seq id identifying sch_entry to retrieve.
 *      allocator - if NULL, use malloc for sec_sch_entry_p buffers and
 *              caller must free buffers.
 *              Set allocator to rpc_ss_allocate when calling from
 *              manager code called from a server stub so that
 *              memory is automatically freed.
 * OUTPUT
 *      sec_sch_entry_p - pointer to a sec_attr_schema_entry_t.  
 *		On output, internal pointers have been allocated
 *              and data copied in.
 *	next_sch_entry_id - sequential id of the next schema entry 
 *		in the list
 *
 * Status Codes:
 *	sec_attr_sch_entry_not_found	 if sch_entry not found with attr_id
 *	sec_attr_no_memory   if malloc fails.
 */
PUBLIC boolean32 rsdb_attr_sch_get_export_by_id (
    rsdb_pvt_id_t		sch_entry_id,			/* [in] */
    void                        *(*allocator)(unsigned32),      /* [in] */
    sec_attr_schema_entry_t 	*sec_sch_entry_p,		/* [out] */
    rsdb_pvt_id_t		*next_sch_entry_id,		/* [out] */
    error_status_t		*st_p
)
{
    rsdb_attr_schema_entry_t 	*db_sch_entry_p;
    rsdb_sequential_key_t	sequential_key;
    Pointer		 	data;
    int				data_len;

    CLEAR_STATUS(st_p);

    memset((Pointer) &sequential_key, NULL, sizeof(sequential_key));
    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = sch_entry_id;

    rsdb_fetch(rsdb_attr_schema_db, (Pointer) &sequential_key,
		sizeof(sequential_key), &data, &data_len, st_p);

    if (BAD_STATUS(st_p)) {
	SET_STATUS(st_p, sec_attr_sch_entry_not_found);
	return false;
    }
    db_sch_entry_p = (rsdb_attr_schema_entry_t *)data;
    *next_sch_entry_id = db_sch_entry_p->threads.next;

    rsdb_attr_sch_export(db_sch_entry_p, allocator, sec_sch_entry_p, st_p);
    if (BAD_STATUS(st_p)) {
        return false;
    }
    return true;
}


/*
 * r s d b _ a t t r _ s c h _ g e t _ b y _ u u i d 
 *
 * Retrieve the sch_entry using the input attr_id as the key.
 * Return true if sch_entry found, false otherwise.
 * sch_entry_p must be freed by caller.
 *
 * Status Codes:
 *	sec_attr_sch_entry_not_found	 if sch_entry not found with attr_id
 *	sec_attr_no_memory   if malloc fails.
 */
PUBLIC boolean32 rsdb_attr_sch_get_by_uuid (
	uuid_t 				*attr_id_p,	/* [in] */
	rsdb_attr_schema_entry_t 	**sch_entry_p,	/* [out] */
	error_status_t 			*st_p)
{
    rsdb_pvt_id_t		sch_entry_id;

    CLEAR_STATUS(st_p);
    *sch_entry_p = (rsdb_attr_schema_entry_t *)NULL;

    if(!(rsdb_attr_sch_get_id_by_uuid(attr_id_p, &sch_entry_id, st_p))) {
	return false;
    }
    return(rsdb_attr_sch_get_by_id(sch_entry_id, sch_entry_p, st_p));
}


/*
 * r s d b _ a t t r _ s c h _ g e t _ b y _ n a m e
 *
 * Retrieve the sch_entry using the input attr_name as the key.
 * Return true if sch_entry found, false otherwise.
 * sch_entry_p must be freed by caller.
 *
 * Status Codes:
 *	sec_attr_sch_entry_not_found	 if sch_entry not found 
 *	sec_attr_no_memory   if malloc fails.
 */
PUBLIC boolean32 rsdb_attr_sch_get_by_name (
	char 				*attr_name,	/* [in] */
	rsdb_attr_schema_entry_t 	**sch_entry_p,	/* [out] */
	error_status_t 			*st_p)
{
    rsdb_pvt_id_t		sch_entry_id;

    CLEAR_STATUS(st_p);
    *sch_entry_p = (rsdb_attr_schema_entry_t *)NULL;

    if(!(rsdb_attr_sch_get_id_by_name(attr_name, &sch_entry_id, st_p))) {
	return false;
    }
    return(rsdb_attr_sch_get_by_id(sch_entry_id, sch_entry_p, st_p));
}


/*
 * r s d b _ a t t r _ s c h _ s t o r e _ n e w
 *
 * Store the input rsdb_attr_schema_entry_t in the ATTR_SCHEMA database.
 * The value of sch_entry_id will contain the sequential key,
 * for the data that was stored.  
 *
 * Three records get stored for a new schema_entry:
 *	ATTR UUID -> SCH_ENTRY_ID
 *	ATTR NAME -> SCH_ENTRY_ID
 *	SCH_ENTRY_ID -> RSDB_ATTR_SCH_ENTRY_T
 *
 */
PUBLIC void rsdb_attr_sch_store_new(
    rsdb_attr_schema_entry_t	*sch_entry_p,	/* [in] */
    rsdb_pvt_id_t		*sch_entry_id,	/* [out] */
    error_status_t		*st_p)
{
    rsdb_uid_key_t		uuid_key;
    rsdb_attr_name_key_t	attr_name_key;
    rsdb_sequential_key_t	sequential_key;

    memset((Pointer) &uuid_key, NULL,       sizeof(uuid_key));
    memset((Pointer) &attr_name_key, NULL,  sizeof(attr_name_key));
    memset((Pointer) &sequential_key, NULL, sizeof(sequential_key));

    /*
     * Keys
     */

    sequential_key.record_type 	= SEQUENTIAL_TYPE;
    sequential_key.id 		= rsdb_new_sequential_id();
    sch_entry_p->threads.id 	= sequential_key.id;
    *sch_entry_id 		= sequential_key.id;

    uuid_key.record_type	= UUID_TYPE;
    uuid_key.uid		= sch_entry_p->attr_id;

    attr_name_key.record_type	= ATTR_NAME_TYPE;
    attr_name_key.name_len	= sch_entry_p->attr_name_len;

    u_strncpy(attr_name_key.name, sch_entry_p->buf, 
		sch_entry_p->attr_name_len);
    /*
     *  Thread the new schema entry on the sequential list.
     */

    sch_entry_p->threads.next = last->threads.id;
    sch_entry_p->threads.prev = last->threads.prev;

    rsdb_store(rsdb_attr_schema_db, (Pointer) &sequential_key,
    	sizeof(sequential_key), (Pointer) sch_entry_p, 
    	sch_entry_p->sch_entry_len, st_p);

    rsdb_store(rsdb_attr_schema_db, (Pointer) &uuid_key,
    	sizeof(uuid_key), (Pointer) sch_entry_id, 
    	sizeof(*sch_entry_id), st_p);

    rsdb_store(rsdb_attr_schema_db, (Pointer) &attr_name_key,
    	NAME_LEN(&attr_name_key), (Pointer) sch_entry_id,
    	sizeof(*sch_entry_id), st_p);

    /* 
     * Update threads of next and previous schema entries in list.
     */

    /* rsdb_attr_reset_next */
    rsdb_attr_sch_reset(sch_entry_p->threads.prev, 
	sch_entry_p->threads.id, false, st_p);

    /* rsdb_attr_reset_prev */
    rsdb_attr_sch_reset(sch_entry_p->threads.next,
	sch_entry_p->threads.id, true, st_p);

    /* Create the head of the list of attribute instances
     * of this type.
     */
    rsdb_attr_seq_list_create(sch_entry_p->threads.id, st_p);
}


/*
 * r s d b _ a t t r _ s c h _ i m p o r t
 *
 * Allocate db_sch_entry_p to contain enough space 
 * for all fixed and variable length data in sec_sch_entry_p.
 * Pack data from sec_sch_entry_p into db_sch_entry_p buffer.
 * db_sch_entry_p must be freed by caller.
 *
 * INPUT
 *	sec_sch_entry_p - points to semantically-checked sec_attr_schema_entry
 *		whose data is to be copied into db_sch_entry_p for storage.
 * OUTPUT
 *	db_sch_entry_p - points to an rsdb_attr_schema_entry buffer
 *		allocated by this function into which sch_entry data is copied.
 *
 * Status Codes:
 *
 */
PUBLIC void rsdb_attr_sch_import(
    sec_attr_schema_entry_t	*sec_sch_entry_p,
    rsdb_attr_schema_entry_t	**db_sch_entry_p,
    error_status_t  		*st_p)
{

    CLEAR_STATUS(st_p);

    if(!(SA_ACL_MGR_SET_P(sec_sch_entry_p))) {
	SET_STATUS(st_p, sec_attr_bad_acl_mgr_set);
	return;
    }

    /* Allocate the db_sch_entry_p buffer large enough to hold
     * the data in sec_sch_entry_p.  db_sch_entry_p is memset to NULL
     * except for the sch_entry_len field which is set to the size
     * of the allocated buffer.
     */
    rsdb_attr_sch_alloc_db_entry(sec_sch_entry_p, db_sch_entry_p, st_p);
    if(BAD_STATUS(st_p)) {
	return;
    }
    rsdb_attr_sch_import_info(sec_sch_entry_p, *db_sch_entry_p, st_p);

}

/*
 * r s d b _ a t t r _ s c h _ i m p o r t _ i n f o
 *
 * Pack data from sec_sch_entry_p into db_sch_entry_p buffer.
 *
 * INPUT
 *	sec_sch_entry_p - points to semantically-checked sec_attr_schema_entry
 *		whose data is to be copied into db_sch_entry_p for storage.
 * OUTPUT
 *	db_sch_entry_p - points to an rsdb_attr_schema_entry buffer
 *		allocated by this function into which sch_entry data is copied.
 *
 * Status Codes:
 *
 */

PUBLIC void rsdb_attr_sch_import_info(
    sec_attr_schema_entry_t	*sec_sch_entry_p,
    rsdb_attr_schema_entry_t	*db_sch_entry_p,
    error_status_t  		*st_p)
{
    long		i,j;
    Pointer		bufp;
    unsigned32		length;
    CLEAR_STATUS(st_p);

    (db_sch_entry_p)->attr_name_len    = u_strlen(sec_sch_entry_p->attr_name);
    (db_sch_entry_p)->attr_id          = sec_sch_entry_p->attr_id;
    (db_sch_entry_p)->attr_encoding    = sec_sch_entry_p->attr_encoding;
    (db_sch_entry_p)->acl_mgr_num      = SA_ACL_MGR_NUM(sec_sch_entry_p);
    (db_sch_entry_p)->schema_entry_flags = (sec_sch_entry_p)->schema_entry_flags;
    (db_sch_entry_p)->intercell_action = sec_sch_entry_p->intercell_action;
    (db_sch_entry_p)->trig_types       = sec_sch_entry_p->trig_types;
    (db_sch_entry_p)->db_flags         = rsdb_attr_sch_flag_none;

    /* Make sure there's bind_info present before dereferencing ptr */
    if(SA_TRG_BND_INFO_P(sec_sch_entry_p)) {
        (db_sch_entry_p)->db_flags |= rsdb_attr_sch_flag_trig_binding;
	(db_sch_entry_p)->trig_auth_info_type = 
	    SA_TRG_BND_AUTH_INFO_TYPE(sec_sch_entry_p);
	if(SA_TRG_BND_AUTH_INFO_TYPE(sec_sch_entry_p) == 
	    sec_attr_bind_auth_dce) {
	    (db_sch_entry_p)->trig_svr_princ_name_len =
		u_strlen(SA_TRG_BND_AUTH_SVR_PNAME_P(sec_sch_entry_p));
	    (db_sch_entry_p)->trig_protect_level =
		SA_TRG_BND_AUTH_PROT_LEVEL(sec_sch_entry_p);
	    (db_sch_entry_p)->trig_authn_svc =
		SA_TRG_BND_AUTH_AUTHN_SVC(sec_sch_entry_p);
	    (db_sch_entry_p)->trig_authz_svc =
		SA_TRG_BND_AUTH_AUTHZ_SVC(sec_sch_entry_p);
	}
	(db_sch_entry_p)->trig_binding_num = SA_TRG_BND_NUM(sec_sch_entry_p);
    }

    (db_sch_entry_p)->scope_len        = u_strlen(sec_sch_entry_p->scope);
    (db_sch_entry_p)->comment_len      = u_strlen(sec_sch_entry_p->comment);

    /*
     * Set bufp to point to the beginning of the variable
     * length data in buffer.  
     */
    bufp = (Pointer) (db_sch_entry_p)->buf;
    /* ATTR_NAME */
    RSDB_ATTR_COPY_ADV_TGT(bufp, sec_sch_entry_p->attr_name, 
	(db_sch_entry_p)->attr_name_len);

    /* ACL_MGR_SET */
    for(i = 0; i < (db_sch_entry_p)->acl_mgr_num; i++) {
	RSDB_ATTR_COPY_ADV_TGT(bufp, SA_ACL_MGR_INFO_P(sec_sch_entry_p, i),
	    sizeof(*(SA_ACL_MGR_INFO_P(sec_sch_entry_p, i))));
    }

    /* If binding info is present, import it */
    if(IS_SET((db_sch_entry_p)->db_flags, rsdb_attr_sch_flag_trig_binding)) {
        /* AUTH_INFO SVR_PRINC_NAME */
        if((db_sch_entry_p)->trig_auth_info_type == sec_attr_bind_auth_dce) {
	    RSDB_ATTR_COPY_ADV_TGT(bufp, 
	        SA_TRG_BND_AUTH_SVR_PNAME_P(sec_sch_entry_p),
	        (db_sch_entry_p)->trig_svr_princ_name_len);
        }

        /* TRIG_BINDINGS */
        for(i = 0; i < (db_sch_entry_p)->trig_binding_num; i++) {
	    /* BIND_TYPE */
	    RSDB_ATTR_COPY_ADV_TGT(bufp,
	        &(SA_TRG_BND_TYPE(sec_sch_entry_p, i)),
	        sizeof(SA_TRG_BND_TYPE(sec_sch_entry_p, i)));
	    switch(SA_TRG_BND_TYPE(sec_sch_entry_p, i)) {
	    case sec_attr_bind_type_string:
	        /* STRING_BINDING */
	        length = u_strlen(SA_TRG_BND_STRING_P(sec_sch_entry_p, i));
	        RSDB_ATTR_COPY_ADV_TGT(bufp, &length, sizeof(length));
	        RSDB_ATTR_COPY_ADV_TGT(bufp,
		    SA_TRG_BND_STRING_P(sec_sch_entry_p, i),
		    length);
	        break;
	    case sec_attr_bind_type_twrs:
	        /* TWR_SET */
	        RSDB_ATTR_COPY_ADV_TGT(bufp,
		    &(SA_TRG_BND_TWRSET_COUNT(sec_sch_entry_p, i)),
		    sizeof(SA_TRG_BND_TWRSET_COUNT(sec_sch_entry_p, i)));
	        for(j = 0; 
		    j < SA_TRG_BND_TWRSET_COUNT(sec_sch_entry_p, i); j++) {
		    RSDB_ATTR_COPY_ADV_TGT(bufp,
		        &(SA_TRG_BND_TWR_LEN(sec_sch_entry_p, i, j)),
		        sizeof(SA_TRG_BND_TWR_LEN(sec_sch_entry_p, i, j)));
		    RSDB_ATTR_COPY_ADV_TGT(bufp,
		        SA_TRG_BND_TWR_OCTETS(sec_sch_entry_p, i, j),
		        SA_TRG_BND_TWR_LEN(sec_sch_entry_p, i, j));
	        }
	        break;
	    case sec_attr_bind_type_svrname:
	        /* SVRNAME */
                if(SA_TRG_BND_SVRNAME_P(sec_sch_entry_p, i)) {
                    RSDB_ATTR_COPY_ADV_TGT(bufp,
                        &(SA_TRG_BND_SVRNAME_SYNTAX(sec_sch_entry_p, i)),
                        sizeof(SA_TRG_BND_SVRNAME_SYNTAX(sec_sch_entry_p, i)));
	            length = 
		        u_strlen(SA_TRG_BND_SVRNAME_NAME_P(sec_sch_entry_p, i));
	            RSDB_ATTR_COPY_ADV_TGT(bufp, &length, sizeof(length));
	            RSDB_ATTR_COPY_ADV_TGT(bufp,
		        SA_TRG_BND_SVRNAME_NAME_P(sec_sch_entry_p, i),
		        length);
	        }
	        break;
	    } /* end switch */
        }
    }

    /* SCOPE */
    RSDB_ATTR_COPY_ADV_TGT(bufp, sec_sch_entry_p->scope, 
	(db_sch_entry_p)->scope_len);

    /* COMMENT */
    RSDB_ATTR_COPY_ADV_TGT(bufp, sec_sch_entry_p->comment, 
	(db_sch_entry_p)->comment_len);

}

/*
 * r s d b _ a t t r _ s c h _ e x p o r t
 *
 * Unpack data from db_sch_entry_p into sec_sch_entry_p buffer.
 * Allocate space for variable length fields in sec_sch_entry_p.
 * Caller is responsible for freeing internal pointers by calling
 * sec_attr_util_sch_ent_free_ptrs(sec_sch_entry_p).
 *
 * INPUT:
 *	db_sch_entry_p - points to an rsdb_attr_schema_entry_t containing
 *		data to be exported into sec_sch_entry_p.
 *      allocator - if NULL, use malloc for sec_sch_entry_p buffers and
 *              caller must free buffers.
 *              Set allocator to rpc_ss_allocate when calling from
 *              manager code called from a server stub so that
 *              memory is automatically freed.
 *
 * OUTPUT:
 *	sec_sch_entry_p - pointer to an allocated sec_attr_schema_entry_t.
 *		On output, internal ptrs allocated and data copied in.
 *
 * ERRORS:
 *	sec_attr_cant_allocate_memory - no more memory.
 *
 */
PUBLIC void rsdb_attr_sch_export(
    rsdb_attr_schema_entry_t	*db_sch_entry_p,		/* [in] */
    void                        *(*allocator)(unsigned32),      /* [in] */
    sec_attr_schema_entry_t	*sec_sch_entry_p,		/* [out] */
    error_status_t  		*st_p)
{
    int			i, j;
    unsigned32		length;
    char		*bufp;	/* Pointer to variable length buffer */

    CLEAR_STATUS(st_p);
    if(!sec_sch_entry_p) {
	SET_STATUS(st_p, sec_attr_bad_param);
	return;
    }

    /* Note that SCH_ENTRY_ALLOC uses "allocator" to get memory */

    /* Initialize ptrs to zero to avoid marshalling problems on error */
    sec_sch_entry_p->attr_name = NULL;
    sec_sch_entry_p->acl_mgr_set = NULL;
    sec_sch_entry_p->trig_binding = NULL;
    sec_sch_entry_p->scope = NULL;
    sec_sch_entry_p->comment = NULL;

    sec_sch_entry_p->attr_id 		= db_sch_entry_p->attr_id;
    sec_sch_entry_p->attr_encoding	= db_sch_entry_p->attr_encoding;
    sec_sch_entry_p->schema_entry_flags = db_sch_entry_p->schema_entry_flags;
    sec_sch_entry_p->intercell_action 	= db_sch_entry_p->intercell_action;
    sec_sch_entry_p->trig_types 	= db_sch_entry_p->trig_types;
    /*
     * Set bufp to point to the beginning of the variable
     * length data in db_sch_entry_p.  As data is copied,
     * bufp is updated to point at the unread data.
     */
    bufp = db_sch_entry_p->buf;

    /* ATTR_NAME: Malloc and Copy, null-terminate it. */
    if(db_sch_entry_p->attr_name_len > 0) {
        SCH_ENTRY_ALLOC(sec_sch_entry_p,
	    sec_sch_entry_p->attr_name,
	    db_sch_entry_p->attr_name_len + 1, st_p);
	    RSDB_ATTR_COPY_NULL_ADV_SRC(sec_sch_entry_p->attr_name, bufp,
		    db_sch_entry_p->attr_name_len);
    }

    /* ACL_MGR_SET: Malloc and Copy */
    SCH_ENTRY_ALLOC(sec_sch_entry_p,
	SA_ACL_MGR_SET_P(sec_sch_entry_p),
	SA_ACL_MGR_SET_SIZE(db_sch_entry_p->acl_mgr_num), st_p);
    SA_ACL_MGR_NUM(sec_sch_entry_p) =  db_sch_entry_p->acl_mgr_num;
    for(i = 0; i < SA_ACL_MGR_NUM(sec_sch_entry_p); i++) {
	/* ACL_MGR_INFO: Malloc and Copy */
	SCH_ENTRY_ALLOC(sec_sch_entry_p, 
		SA_ACL_MGR_INFO_P(sec_sch_entry_p, i),
		sizeof(*(SA_ACL_MGR_INFO_P(sec_sch_entry_p, i))), st_p);
	RSDB_ATTR_COPY_ADV_SRC(SA_ACL_MGR_INFO_P(sec_sch_entry_p, i),
		bufp, sizeof(*(SA_ACL_MGR_INFO_P(sec_sch_entry_p, i))));
    }

    /* If trig_binding info was stored, export it */
    if(IS_SET((db_sch_entry_p)->db_flags, rsdb_attr_sch_flag_trig_binding)) {
        /* TRIG_BINDING: Malloc and Copy */
        SCH_ENTRY_ALLOC(sec_sch_entry_p, 
	    SA_TRG_BND_INFO_P(sec_sch_entry_p),
	    SA_BND_INFO_SIZE(db_sch_entry_p->trig_binding_num), st_p);
        SA_TRG_BND_AUTH_INFO_TYPE(sec_sch_entry_p) = 
	    db_sch_entry_p->trig_auth_info_type;
        if(SA_TRG_BND_AUTH_INFO_TYPE(sec_sch_entry_p) == 
	    sec_attr_bind_auth_dce) {
	    /* SVR_PRINC_NAME: Malloc, Copy, and null-terminate */
	    SCH_ENTRY_ALLOC(sec_sch_entry_p, 
		SA_TRG_BND_AUTH_SVR_PNAME_P(sec_sch_entry_p),
		db_sch_entry_p->trig_svr_princ_name_len + 1, st_p);
	    RSDB_ATTR_COPY_NULL_ADV_SRC(
		SA_TRG_BND_AUTH_SVR_PNAME_P(sec_sch_entry_p),
		bufp, db_sch_entry_p->trig_svr_princ_name_len);
	    SA_TRG_BND_AUTH_PROT_LEVEL(sec_sch_entry_p) = 
		db_sch_entry_p->trig_protect_level;
	    SA_TRG_BND_AUTH_AUTHN_SVC(sec_sch_entry_p) = 
		db_sch_entry_p->trig_authn_svc;
	    SA_TRG_BND_AUTH_AUTHZ_SVC(sec_sch_entry_p) = 
		db_sch_entry_p->trig_authz_svc;
        } 
        SA_TRG_BND_NUM(sec_sch_entry_p) = db_sch_entry_p->trig_binding_num;
        for(i = 0; i < SA_TRG_BND_NUM(sec_sch_entry_p); i++) {
	    RSDB_ATTR_COPY_ADV_SRC(&(SA_TRG_BND_TYPE(sec_sch_entry_p, i)),
		bufp, sizeof(SA_TRG_BND_TYPE(sec_sch_entry_p, i)));
	    switch(SA_TRG_BND_TYPE(sec_sch_entry_p, i)) {
	    case sec_attr_bind_type_string:
	        /* STRING_BINDING: Malloc, Copy, Null-terminate */
	        RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
	        SCH_ENTRY_ALLOC(sec_sch_entry_p, 
		    SA_TRG_BND_STRING_P(sec_sch_entry_p, i),
		    length + 1, st_p);
	        RSDB_ATTR_COPY_NULL_ADV_SRC(
		    SA_TRG_BND_STRING_P(sec_sch_entry_p, i),
		    bufp, length);
	        break;
	    case sec_attr_bind_type_twrs:
	        /* TWR_SET: Malloc, Copy */
	        /* retrieve twr_set count */
	        RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
	        SCH_ENTRY_ALLOC(sec_sch_entry_p, 
		    SA_TRG_BND_TWRSET_P(sec_sch_entry_p, i),
		    SA_TWR_SET_SIZE(length),
		    st_p);
	        SA_TRG_BND_TWRSET_COUNT(sec_sch_entry_p, i) = length;
	        for(j = 0; j < SA_TRG_BND_TWRSET_COUNT(sec_sch_entry_p, i);
		    j++) {
		    /* TWR_REF: Malloc, Copy */
		    RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
		    SCH_ENTRY_ALLOC(sec_sch_entry_p, 
		        SA_TRG_BND_TWR_P(sec_sch_entry_p, i, j),
		        SA_TWR_SIZE(length),
		        st_p);
		    SA_TRG_BND_TWR_LEN(sec_sch_entry_p, i, j) = length;
		    RSDB_ATTR_COPY_ADV_SRC(
		        SA_TRG_BND_TWR_OCTETS(sec_sch_entry_p, i, j),
		        bufp,
		        length);
	        }
	        break;
	    case sec_attr_bind_type_svrname:
                /* alloc the sec_attr_bind_svrname structure */
                SCH_ENTRY_ALLOC(sec_sch_entry_p,
                    SA_TRG_BND_SVRNAME_P(sec_sch_entry_p, i),
                    sizeof(*(SA_TRG_BND_SVRNAME_P(sec_sch_entry_p, i))), st_p);
                /* copy the name_syntax */
                RSDB_ATTR_COPY_ADV_SRC(
                    &(SA_TRG_BND_SVRNAME_SYNTAX(sec_sch_entry_p, i)),
                    bufp, 
                    sizeof(SA_TRG_BND_SVRNAME_SYNTAX(sec_sch_entry_p, i)));
	        /* get svrname length, alloc, Copy, Null-terminate */
	        RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
	        SCH_ENTRY_ALLOC(sec_sch_entry_p, 
		    SA_TRG_BND_SVRNAME_NAME_P(sec_sch_entry_p, i),
		    length + 1, st_p);
	        RSDB_ATTR_COPY_NULL_ADV_SRC(
		    SA_TRG_BND_SVRNAME_NAME_P(sec_sch_entry_p, i),
		    bufp, length);
	        break;
	    } /* end switch */
        }
    }

    /* SCOPE: Malloc, Copy, Null-terminate it. */
    if(db_sch_entry_p->scope_len > 0) {
        SCH_ENTRY_ALLOC(sec_sch_entry_p, sec_sch_entry_p->scope,
	    db_sch_entry_p->scope_len + 1, st_p);
        RSDB_ATTR_COPY_NULL_ADV_SRC(sec_sch_entry_p->scope, bufp,
	    db_sch_entry_p->scope_len);
    }

    /* COMMENT: Malloc, Copy, Null-terminate it. */
    if(db_sch_entry_p->comment_len > 0) {
        SCH_ENTRY_ALLOC(sec_sch_entry_p, sec_sch_entry_p->comment,
	    db_sch_entry_p->comment_len + 1, st_p);
        RSDB_ATTR_COPY_NULL_ADV_SRC(sec_sch_entry_p->comment, bufp,
	    db_sch_entry_p->comment_len);
    }
}


/*
 * r s d b _ a t t r _ s c h _ c u r s o r _ i n i t
 *
 * Initialize the cursor and return the current number
 * of schema entries in the schema identified by schema_name.
 *
 * INPUT:
 *	schema_name - name of the schema to be scanned with cursor
 *		(ignored since the registry has only one schema).
 *
 * OUTPUT:
 *	cur_num_entries - number of schema entries in "attr_schema"
 *
 *	cursor - initialized for attr_schema_scan operation at
 *		this server.
 *
 * ERRORS:
 *	sec_attr_bad_cursor
 *
 */
PUBLIC void rsdb_attr_sch_cursor_init(
    sec_attr_component_name_t   schema_name,            /* [in] */
    unsigned32                  *cur_num_entries,       /* [out] */
    rs_attr_cursor_t            *cursor,                /* [out] */
    error_status_t  		*st_p)
{
    CLEAR_STATUS(st_p);

    if(!cursor) {
	SET_STATUS(st_p, sec_attr_bad_cursor);
    } else {
	cursor->source = rs_state.rep_id;

	/* schema cursor object is not used since only
	 * one unambiguous schema exists.
	 */
	cursor->object = NULL;
	cursor->list = NULL;
	/* point to the first schema entry in the list */
	cursor->entry = first->threads.next;
	cursor->num_entries_left = RSDB_SCH_CUR_NUM_ENTRIES;
	cursor->valid = TRUE;
	if(cur_num_entries) {
	    *cur_num_entries = RSDB_SCH_CUR_NUM_ENTRIES;
	}
    } 
}


/*
 * r s d b _ a t t r _ s c h _ c u r s o r _ c h e c k
 *
 * If cursor is valid, make sure it originated at this server.
 * If not valid, initialize the cursor.
 * rs_attr_sch_* operations should call this before making
 * and rsdb_attr_sch_* query with the cursor.
 *
 * INPUT:
 *	schema_name - name of the schema to be scanned with cursor
 *		(ignored since the registry has only one schema).
 *
 *	cursor - initialized for attr_schema_scan operation at
 *		this server.
 *
 * ERRORS:
 *	sec_attr_bad_cursor
 *
 */
PUBLIC void rsdb_attr_sch_cursor_check(
    sec_attr_component_name_t   schema_name,            /* [in] */
    rs_attr_cursor_t            *cursor,                /* [in, out] */
    error_status_t  		*st_p)
{
    unsigned32                  cur_num_entries;

    CLEAR_STATUS(st_p);

    if(!cursor) {
	SET_STATUS(st_p, sec_attr_bad_cursor);
	return;
    }
    if(cursor->valid) {
	/* Error if cursor not initialized by this replica */
	if(!(uuid_equal(&(cursor->source), &(rs_state.rep_id), st_p))) {
	    SET_STATUS(st_p, sec_attr_bad_cursor);
	    return;
	}
    } else {
	rsdb_attr_sch_cursor_init(schema_name, 
		&cur_num_entries, cursor, st_p);
    }
}


/*
 * r s d b _ a t t r _ s c h _ a d d
 *
 * Add the new schema entry sec_sch_entry_p to the "attr_schema."
 * Increment the current number of schema entries.
 *
 * INPUT
 *	schema_name - name of schema into which sec_schema_entry_p
 *		should be added.  (Ignored since only one schema exists.)
 *	sec_sch_entry_p - points to semantically-checked sec_attr_schema_entry
 *		whose data is to be copied into db_sch_entry_p for storage.
 *
 * Status Codes:
 *	sec_attr_type_id_exists - attr_id already exists
 *	sec_attr_name_exists - attr_name already exists
 *
 */
PUBLIC void rsdb_attr_sch_add(
    sec_attr_component_name_t	schema_name,		/* [in] */
    sec_attr_schema_entry_t	*sec_sch_entry_p,	/* [in] */
    error_status_t  		*st_p)
{
    rsdb_attr_schema_entry_t    *db_sch_entry_p;
    rsdb_pvt_id_t		sch_entry_id;

    CLEAR_STATUS(st_p);

    if(rsdb_attr_sch_type_id_exists(&(sec_sch_entry_p->attr_id))) {
	SET_STATUS(st_p, sec_attr_type_id_exists);
	return;
    } else if(rsdb_attr_sch_name_exists((char *)(sec_sch_entry_p->attr_name))) {
	SET_STATUS(st_p, sec_attr_name_exists);
	return;
    }
    rsdb_attr_sch_import(sec_sch_entry_p, &db_sch_entry_p, st_p);
    if(BAD_STATUS(st_p)) {
	return;
    }

    /* When sch_entry acl_mgrs are added, inherit and store acl here
     * and store acl_id in db_sch_entry_p before storing sch_entry.
     */

    rsdb_attr_sch_store_new(db_sch_entry_p, &sch_entry_id, st_p);

    /* Increment the current number of schema entries.
     */
    rsdb_attr_sch_upd_num_entries(TRUE, st_p);

    free(db_sch_entry_p);
    return;
}


/*
 * r s d b _ a t t r _ s c h _ d e l e t e
 *
 * Delete the schema entry, identified by sch_entry_id, from the
 * attr_schema database and reset prev and next schema entry pointers.
 * Delete the sequential list of attributes associated with
 * this schema entry.
 *
 * INPUT
 *	schema_name - name of schema into which sec_schema_entry_p
 *		should be added.  (Ignored since only one schema exists.)
 *	attr_id - attribute type uuid identifying the schema entry
 *		to be deleted.
 *
 * Status Codes:
 *	sec_attr_sch_entry_not_found	if schema entry not found 
 *	error_status_ok		otherwise.
 */
PUBLIC void rsdb_attr_sch_delete(
    sec_attr_component_name_t   schema_name,	/* [in] */
    uuid_t			*attr_id_p,	/* [in] */
    error_status_t		*st_p)
{
    rsdb_attr_schema_entry_t	*sch_entry_p;

    CLEAR_STATUS(st_p);

    if(!(rsdb_attr_sch_get_by_uuid(attr_id_p, &sch_entry_p, st_p))) {
	return;
    }

    if(SA_SCH_FLAG_IS_SET_RESERVED(sch_entry_p)) {
	SET_STATUS(st_p, sec_attr_sch_reserved);
	return;
    }

    /* Delete all attribute instances of this type */
    rsdb_attr_seq_list_delete(sch_entry_p->threads.id);

    /* 
     * Delete from sequential chain.
     * Update threads of next and previous schema entries in list.
     */
    rsdb_attr_sch_reset(sch_entry_p->threads.prev,
	 sch_entry_p->threads.next, false, st_p);

    rsdb_attr_sch_reset(sch_entry_p->threads.next,
	sch_entry_p->threads.prev, true, st_p);

    /* Delete the sch_entry record and its uuid and name key records */
    rsdb_attr_sch_purge(sch_entry_p, st_p);

    free(sch_entry_p);

    /* Decrement the current number of schema entries.
     */
    rsdb_attr_sch_upd_num_entries(FALSE, st_p);
}


/*
 * r s d b _ a t t r _ s c h _ r e p l a c e
 *
 * Update the schema entry sec_sch_entry_p in the "attr_schema."
 *
 * INPUT
 *	schema_name - name of schema in which sec_schema_entry_p
 *		should be replaced.  (Ignored since only one schema exists.)
 *	sec_sch_entry_p - points to semantically-checked sec_attr_schema_entry
 *		whose data is to be copied into db_sch_entry_p for storage.
 *
 * Status Codes:
 *	sec_attr_type_id_exists - attr_id already exists
 *	sec_attr_name_exists - attr_name already exists
 *
 */
PUBLIC void rsdb_attr_sch_replace(
    sec_attr_component_name_t		schema_name,		/* [in] */
    sec_attr_schema_entry_t		*sec_sch_entry_p,	/* [in] */
    sec_attr_schema_entry_parts_t	modify_parts,		/* [in] */
    error_status_t  			*st_p)
{

    rsdb_attr_schema_entry_t    *db_sch_entry_old_p;
    rsdb_attr_schema_entry_t    *db_sch_entry_new_p;
    sec_attr_schema_entry_t	sec_sch_entry_old;
    char			*old_attr_name;
    error_status_t		tmp_status;
    rsdb_thread_hdr_t		save_header;

    CLEAR_STATUS(st_p);

    /* Look up old schema entry 
     * (don't forget to free db_sch_entry_old_p when done). 
     */
    if(!(rsdb_attr_sch_get_by_uuid(&(sec_sch_entry_p->attr_id),
	&db_sch_entry_old_p, st_p))) {
	return;
    }
    /* Save the header since it will be used when storing the
     * modified db sch_entry.
     */
    save_header = db_sch_entry_old_p->threads;

    /* export old db_entry to sec_sch_entry_old. 
     * (don't forget to free_ptrs in sec_sch_entry_old when done). 
     */
    rsdb_attr_sch_export(db_sch_entry_old_p, NULL, &sec_sch_entry_old, st_p);

    /* done with old db_entry */
    free(db_sch_entry_old_p);

    /* If rsdb_attr_sch_export failed, return */
    if(BAD_STATUS(st_p)) {
	return;
    }


    /* Overwrite fields in sec_sch_entry_old with modified fields
     * from input sec_sch_entry.  Then store the modified sec_sch_entry_old.
     *
     * To avoid extra allocation and data copies,
     * sec_sch_entry_old temporarily "borrows" data
     * pointers from the input sec_sch_entry for the store
     * operation.  These pointer fields are subsequently set to
     * NULL so that there's no attempt to free a data pointer
     * that's really "owned" by sec_sch_entry.
     */

    /* If name is being modified, check for uniqueness.
     * Store the new name key.  This change_name_key may have to
     * be undone if the subsequent sch_entry update fails.
     */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_name)) {
        if(rsdb_attr_sch_name_exists((char *)(sec_sch_entry_p->attr_name))) {
	    SET_STATUS(st_p, sec_attr_name_exists);
	    sec_attr_util_sch_ent_free_ptrs(&sec_sch_entry_old);
	    return;
	}
	rsdb_attr_sch_change_name_key((char *)sec_sch_entry_old.attr_name,
	    (char *)sec_sch_entry_p->attr_name, st_p);
	if(BAD_STATUS(st_p)) {
            sec_attr_util_sch_ent_free_ptrs(&sec_sch_entry_old);
            return;
        }
	/* Save the pointer to the old attr_name. */
	old_attr_name = (char *)sec_sch_entry_old.attr_name;

	/* temporarily set the attr_name ptr to the new name */
	sec_sch_entry_old.attr_name = sec_sch_entry_p->attr_name;
    }

    /* acl_mgr_set */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_acl_mgrs)) {
	/* Free the old acl_mgr_set, if one exists */
	sec_attr_util_sch_free_acl_mgrs(&sec_sch_entry_old);

	/* temporarily set the acl_mgr_set ptr to the new data */
	sec_sch_entry_old.acl_mgr_set = sec_sch_entry_p->acl_mgr_set;
    }

    /* unique flag - can only be changed to unset */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_unique)) {
	if(!SA_SCH_FLAG_IS_SET_UNIQUE(sec_sch_entry_p)) {
	    SA_SCH_FLAG_UNSET_UNIQUE(&sec_sch_entry_old);
	}
    }

    /* multi-inst flag - can only be changed to set */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_multi_inst)) {
	if(SA_SCH_FLAG_IS_SET_MULTI_INST(sec_sch_entry_p)) {
	    SA_SCH_FLAG_SET_MULTI_INST(&sec_sch_entry_old);
	}
    }

    /* reserved flag */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_reserved)) {
	if(SA_SCH_FLAG_IS_SET_RESERVED(sec_sch_entry_p)) {
	    SA_SCH_FLAG_SET_RESERVED(&sec_sch_entry_old);
	} else {
	    SA_SCH_FLAG_UNSET_RESERVED(&sec_sch_entry_old);
	}
    }

    /* apply_defaults flag */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_defaults)) {
	if(SA_SCH_FLAG_IS_SET_USE_DEFAULTS(sec_sch_entry_p)) {
	    SA_SCH_FLAG_SET_USE_DEFAULTS(&sec_sch_entry_old);
	} else {
	    SA_SCH_FLAG_UNSET_USE_DEFAULTS(&sec_sch_entry_old);
	}
    }

    /* intercell_action flag */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_intercell)) {
	sec_sch_entry_old.intercell_action =
	    sec_sch_entry_p->intercell_action;
    }

    /* trig_types flag not supported for update */

    /* trig_binding */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_trig_bind)) {
	/* Free the old trig_binding, if one exists */
	sec_attr_util_sch_free_binding(&sec_sch_entry_old);

	/* temporarily set the trig_binding ptr to the new data */
	sec_sch_entry_old.trig_binding = sec_sch_entry_p->trig_binding;
    }

    /* scope */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_scope)) {
	/* Free the old scope, if one exists */
	if(sec_sch_entry_old.scope) {
	    free(sec_sch_entry_old.scope);
	}

	/* temporarily set the scope ptr to the new data */
	sec_sch_entry_old.scope = sec_sch_entry_p->scope;
    }

    /* comment */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_comment)) {
	/* Free the old comment, if one exists */
	if(sec_sch_entry_old.comment) {
	    free(sec_sch_entry_old.comment);
	}

	/* temporarily set the comment ptr to the new data */
	sec_sch_entry_old.comment = sec_sch_entry_p->comment;
    }


    /* Import the modified schema entry.
     * Don't forget to free db_sch_entry_new_p when done.
     */
    rsdb_attr_sch_import(&sec_sch_entry_old, &db_sch_entry_new_p, st_p);

    /* Before freeing the internal ptrs of sec_sch_entry_old,
     * remove references to pointers that were copied from
     * sec_sch_entry_p, since they are the callers responsibility.
     */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_name)) {
	sec_sch_entry_old.attr_name = NULL;
    }
    /* acl_mgr_set */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_acl_mgrs)) {
	sec_sch_entry_old.acl_mgr_set = NULL;
    }
    /* trig_binding */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_trig_bind)) {
	sec_sch_entry_old.trig_binding = NULL;
    }
    /* scope */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_scope)) {
	sec_sch_entry_old.scope = NULL;
    }
    /* comment */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_comment)) {
	sec_sch_entry_old.comment = NULL;
    }

    /* If rsdb_attr_sch_import succeeds, copy the saved
     * header into the modified db_sch_entry, store it
     * then free it.
     */
    if(GOOD_STATUS(st_p)) {
        db_sch_entry_new_p->threads = save_header;
	rsdb_attr_sch_store_by_id(db_sch_entry_new_p->threads.id, 
	    db_sch_entry_new_p, st_p);
	free(db_sch_entry_new_p);
    }

    /* If rsdb_attr_sch_import or rsdb_attr_sch_store_by_id
     * returned a bad status and the name_key had been
     * changed, undo the name_key change,
     * retaining the original bad status in st_p.
     */
    if(BAD_STATUS(st_p) && FLAG_SET(modify_parts, sec_attr_schema_part_name)) {
	rsdb_attr_sch_change_name_key((char *)sec_sch_entry_p->attr_name,
	    old_attr_name, &tmp_status);
    }

    /* Clean up */
    sec_attr_util_sch_ent_free_ptrs(&sec_sch_entry_old);

    return;
}


/*
 * r s d b _ a t t r _ s c h _ l o o k u p _ b y _ u u i d
 *
 * Allocate internal buffers for and retrieve the schema entry
 * identified by attribute type uuid, attr_id.
 * Caller must free allocated buffers by calling:
 * sec_attr_util_sch_ent_free_ptrs(sec_sch_entry_p) when done.
 *
 * INPUT
 *	schema_name - name of schema into which sec_schema_entry_p
 *		should be added.  (Ignored since only one schema exists.)
 *	attr_id - attribute type_uuid identifying the schema entry
 *		to be retrieved.
 *      allocator - if NULL, use malloc for sec_sch_entry_p buffers and
 *              caller must free buffers.
 *              Set allocator to rpc_ss_allocate when calling from
 *              manager code called from a server stub so that
 *              memory is automatically freed.
 * OUTPUT:
 *	sec_sch_entry_p - pointer to an allocated sec_attr_schema_entry_t.
 *		On output, internal ptrs allocated and data copied in.
 *
 * Status Codes:
 *	sec_attr_sch_entry_not_found	if schema entry not found 
 *	error_status_ok		otherwise.
 */
PUBLIC void rsdb_attr_sch_lookup_by_uuid(
    sec_attr_component_name_t   schema_name,			/* [in] */
    uuid_t			*attr_id,			/* [in] */
    void                        *(*allocator)(unsigned32),      /* [in] */
    sec_attr_schema_entry_t     *sec_sch_entry_p,		/* [out] */
    error_status_t		*st_p)
{
    rsdb_attr_schema_entry_t	*db_sch_entry_p;

    CLEAR_STATUS(st_p);

    if(!(rsdb_attr_sch_get_by_uuid(attr_id, &db_sch_entry_p, st_p))) {
	return;
    }
    rsdb_attr_sch_export(db_sch_entry_p, allocator, sec_sch_entry_p, st_p);
    free(db_sch_entry_p);
    return;
}


/*
 * r s d b _ a t t r _ s c h _ l o o k u p _ b y _ n a m e
 *
 * Allocate internal buffers for and retrieve the schema entry
 * identified by attr_name.
 * Caller must free allocated ptrs by calling:
 * sec_attr_util_sch_ent_free_ptrs(sec_sch_entry_p) when done.
 *
 * INPUT
 *	schema_name - name of schema into which sec_schema_entry_p
 *		should be added.  (Ignored since only one schema exists.)
 *	attr_name - attribute name identifying the schema entry
 *		to be retrieved.
 *      allocator - if NULL, use malloc for sec_sch_entry_p buffers and
 *              caller must free buffers.
 *              Set allocator to rpc_ss_allocate when calling from
 *              manager code called from a server stub so that
 *              memory is automatically freed.
 * OUTPUT:
 *	sec_sch_entry_p - pointer to an allocated sec_attr_schema_entry_t.
 *		On output, internal ptrs allocated and data copied in.
 *
 * Status Codes:
 *	sec_attr_sch_entry_not_found	if schema entry not found 
 *	error_status_ok		otherwise.
 */
PUBLIC void rsdb_attr_sch_lookup_by_name(
    sec_attr_component_name_t   schema_name,			/* [in] */
    char			*attr_name,			/* [in] */
    void                        *(*allocator)(unsigned32),      /* [in] */
    sec_attr_schema_entry_t     *sec_sch_entry_p,		/* [out] */
    error_status_t		*st_p)
{
    rsdb_attr_schema_entry_t	*db_sch_entry_p;

    CLEAR_STATUS(st_p);

    if(!(rsdb_attr_sch_get_by_name(attr_name, &db_sch_entry_p, st_p))) {
	return;
    }
    rsdb_attr_sch_export(db_sch_entry_p, allocator, sec_sch_entry_p, st_p);
    free(db_sch_entry_p);
    return;
}


/*
 * r s d b _ a t t r _ s c h _ g e t _ n e x t
 *
 * Retrieve the schema entry indicated by the cursor
 * and update the cursor to point to the next entry.
 * Caller must free allocated ptrs by calling:
 * sec_attr_util_sch_ent_free_ptrs(sec_sch_entry_p) when done.
 * Cursor is assumed to have been checked with 
 * rsdb_attr_sch_cursor_check or initialized with 
 * rsdb_attr_sch_cursor_init before calling this operation.
 *
 *
 * INPUT:
 *	schema_name - name of the schema to be scanned with cursor
 *		(ignored since the registry has only one schema).
 *      allocator - if NULL, use malloc for sec_sch_entry_p buffers and
 *              caller must free buffers.
 *              Set allocator to rpc_ss_allocate when calling from
 *              manager code called from a server stub so that
 *              memory is automatically freed.
 *
 * INPUT/OUTPUT:
 *	cursor - initialized for attr_schema_scan operation at
 *		this server.
 *
 * OUTPUT:
 *	sec_sch_entry_p - pointer to an allocated sec_attr_schema_entry_t.
 *		On output, internal ptrs allocated and data copied in.
 *
 *	num_entries_left - a hint at the number of schema entries
 *		not yet seen.
 *
 * ERRORS:
 *	sec_attr_no_more_entries - cursor points to end of list
 *	sec_attr_bad_cursor
 *	sec_attr_no_memory
 *
 */
PUBLIC void rsdb_attr_sch_get_next(
    sec_attr_component_name_t   schema_name,            /* [in] */
    void                        *(*allocator)(unsigned32),      /* [in] */
    rs_attr_cursor_t            *cursor,                /* [in, out] */
    sec_attr_schema_entry_t     *sec_sch_entry_p,	/* [out] */
    unsigned32			*num_entries_left,	/* [out] */
    error_status_t  		*st_p)
{
    rsdb_pvt_id_t		next_sch_entry_id;

    CLEAR_STATUS(st_p);

    if ( cursor->valid && cursor->entry == END_OF_LIST) {
	SET_STATUS(st_p, sec_attr_no_more_entries);
	return;
    }

    if(!(rsdb_attr_sch_get_export_by_id(cursor->entry, allocator,
	sec_sch_entry_p, &next_sch_entry_id, st_p))) {
	return;
    }

    cursor->entry = next_sch_entry_id;

    /* keep track of number of entries left */
    (cursor->num_entries_left)--;
    *num_entries_left = cursor->num_entries_left;

    return;
}


/*
 * r s d b _ a t t r _ s c h _ g e t _ e n t r i e s
 *
 * Retrieve the schema entries for each of the attribute
 * instance types in attrs[].  Return the schema entries
 * in sch_list.  Cross_reference the attrs and schema
 * entries in xref[] in the following way:
 * Where i = index of attr_instance in attrs[], and
 * where j = index of schema entry in sch_list->entries[],
 * then xref[i] = j.
 *
 * If call fails and can be attributed to a problem with
 * an input attribute, set failure_index to indicate which
 * element in attrs[] caused the failure.
 * Set failure_index to -1 if failure can't be attributed
 * to a given attribute.
 *
 * Caller must free allocated buffers by calling, for
 * each p = &(sch_list->entries[i].sch_entry):
 * sec_attr_util_sch_ent_free_ptrs(p) when done.
 *
 * The following must also be free'd *sch_list and *xref.
 * 
 * INPUT
 *	schema_name - name of schema (Ignored for now)
 *	num_attrs - number of attribute instances in attrs[] array
 *	attrs - array of sec_attr_t attribute instances
 *	rgy_obj_info - (who knows!)
 * INPUT/OUTPUT:
 *	sch_list - Input: pointer to allocated rsdb_attr_sch_list_t 
 *		with size field set to indicate number of slots in list.
 *		Output: count is set to actual number of entries
 *		populated in entries[] subarray.
 *	xref - Input: an allocated rsdb_attr_sch_attr_xref_t 
 *		with number of elements equal to input "num_attrs."
 *		Output: array is populated with indices cross referencing
 *		the attrs[] and sch_list->entries[] arrays.
 *		
 * OUTPUT:
 *	failure_index - on failure, set to the position of 
 *		the problematic attribute in attrs[].
 *
 * Status Codes:
 *	sec_attr_sch_entry_not_found	if schema entry not found 
 *	sec_attr_no_more_entries	if all attributes requested,
 *				        but none found.
 *	error_status_ok		otherwise.
 */
PUBLIC void rsdb_attr_sch_get_entries(
    sec_attr_component_name_t   schema_name,		/* [in] */
    unsigned32			num_attrs,		/* [in] */
    sec_attr_t			attrs[],		/* [in] */
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,		/* [in] */
    rsdb_attr_sch_list_t	**sch_list,             /* [in, out] */
    signed32			**xref,			/* [in, out] */
    signed32			*failure_index,         /* [out] */
    error_status_t		*st_p
)
{
    rsdb_pvt_id_t		sch_entry_id;
    rsdb_pvt_id_t		next_sch_entry_id;
    rsdb_pvt_id_t		attr_list_id;
    rsdb_pvt_id_t		alt_attrs[1024];
    rsdb_max_attr_list_t	list;
    signed32			i, j, k, x, found, pos, use_alt_attrs;

    CLEAR_STATUS(st_p);
    *sch_list = NULL;
    *xref = NULL;
    *failure_index = -1;

    /* set j (current entry in sch_list->entries) to 0 */
    j = 0;

    /* 
     * If num_attrs is equal to zero, then we want to fetch
     * all of the possible attributes for this schema.
     */
    use_alt_attrs = 0;
    if (num_attrs == 0) {
	use_alt_attrs=1;
	attr_list_id = rgy_obj_info->attr_list_id;
	while (attr_list_id != END_OF_LIST)  {
	    if (!(rsdb_attr_list_get_by_id(attr_list_id, &list))) {
                dce_svc_printf(SEC_ATTR_CANT_GET_ATTRLIST_MSG);
	    }
	    for (i = 0; i < list.total_count; i++ ) {
		found = 0;
		for (k = 0; k < num_attrs; k++ ) {
		    if (!list.attr[i].inst_id.deleted) {
			if (list.attr[i].type_id == alt_attrs[k])
			    found = 1;
		    }
		}
	        if (!found) {
		    if (!list.attr[i].inst_id.deleted)
		        alt_attrs[num_attrs++] = list.attr[i].type_id;
		}
	    }
	    attr_list_id = list.next;
	}
    } 
    if ( num_attrs == 0 ) {
	SET_STATUS(st_p, sec_attr_no_more_entries);
	return;
    }
    /* 
     * Need to allocate appropriate amount of space 
     * for sch_list and xref.
     */
    RSDB_ATTR_MALLOC_ZERO((*sch_list),
        RSDB_ATTR_SCH_LIST_SIZE((*sch_list), num_attrs), st_p);

    if(BAD_STATUS(st_p)) {
	return;
    }

    (*sch_list)->size = num_attrs;
    (*sch_list)->count = 0;

    RSDB_ATTR_MALLOC_ZERO((*xref), num_attrs * sizeof(signed32), st_p);

    if(BAD_STATUS(st_p)) {
	return;
    }

    /* For each attribute, retrieve the schema entry if it
     * hasn't already been retrieved for a previous attribute.
     */
    for(i = 0; i < num_attrs && (*sch_list)->count < (*sch_list)->size; i++) {
	if (use_alt_attrs == 0 ) {
            /* translate the uuid to the smaller sch_entry_id */
            if(!(rsdb_attr_sch_get_id_by_uuid(&(attrs[i].attr_id),
   	         &sch_entry_id, st_p))) {
    	        *failure_index = i;
    	        break;
            }
	} else {
	    sch_entry_id = alt_attrs[i];
	}
        /* Check sch_list to see if sch_entry has already
         * been retrieved.  If so, cross-reference it and continue.
         */
        for(x = 0; x < (*sch_list)->count; x++) {
            if((*sch_list)->entries[x].sch_entry_id == sch_entry_id) {
                break;
            }
        }
        if((*sch_list)->entries[x].sch_entry_id == sch_entry_id) {
            /* more than one attribute of this type found in attrs[],
             * so set mv_update to true.
             */
            (*sch_list)->entries[x].flags.mv_update = 1;
            (*xref)[i] = x;
            /* sch_entry already stored, so skip past sch_entry retrieval */
            continue;
        }
        /* Need to retrieve sch_entry */
        (*sch_list)->entries[j].sch_entry_id = sch_entry_id;
        if(!(rsdb_attr_sch_get_export_by_id(
    	    (*sch_list)->entries[j].sch_entry_id, NULL,
    	    &((*sch_list)->entries[j].sch_entry), &next_sch_entry_id, st_p))) {
    	    *failure_index = i;
    	    break;
        }
        (*xref)[i] = j;
        ((*sch_list)->count)++;
        j++;
    }

    /* if ran out of space for sch_entries before 
     * evaluating all attrs, return error
     */
    if( i < num_attrs - 1 && (*sch_list)->count >= (*sch_list)->size ) {
	SET_STATUS(st_p, sec_s_pgmerr);
    }

    /* if unsuccessful, release resources before returning */
    if(BAD_STATUS(st_p)) {
	for(j = 0; j < (*sch_list)->count; j++) {
	    sec_attr_util_sch_ent_free_ptrs(&((*sch_list)->entries[j].sch_entry));
	}
    }
    return;
}

/*
 * r s d b _ a t t r _ s c h _ t y p e _ i d _ e x i s t s
 *
 * Return TRUE if attr_id matches the type uuid of an
 * existing schema entry in "attr_schema."  
 * Return FALSE otherwise.
 *
 * INPUT:
 *	attr_id - an attribute type uuid to be checked
 *		for existence in the attr_schema.
 */
PUBLIC boolean32 rsdb_attr_sch_type_id_exists(
    uuid_t			*attr_id)		/* [in] */
{
    rsdb_pvt_id_t		sch_entry_id;
    error_status_t		st;

    return(rsdb_attr_sch_get_id_by_uuid(attr_id, &sch_entry_id, &st));
}


/*
 * r s d b _ a t t r _ s c h _ n a m e _ e x i s t s
 *
 * Return TRUE if attr_name matches the attribute name of an
 * existing schema entry in "attr_schema."  
 * Return FALSE otherwise.
 *
 * INPUT:
 *	attr_name - an attribute name to be checked
 *		for existence in the attr_schema.
 */
PUBLIC boolean32 rsdb_attr_sch_name_exists(
    char			*attr_name)		/* [in] */
{
    rsdb_pvt_id_t		sch_entry_id;
    error_status_t		st;

    return(rsdb_attr_sch_get_id_by_name(attr_name, &sch_entry_id, &st));
}
