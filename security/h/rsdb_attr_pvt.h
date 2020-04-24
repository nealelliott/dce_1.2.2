/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_attr_pvt.h,v $
 * Revision 1.1.9.3  1996/02/18  23:00:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:29  marty]
 *
 * Revision 1.1.9.2  1995/12/08  17:27:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:43  root]
 * 
 * Revision 1.1.5.1  1994/10/17  18:18:10  cuti
 * 	delete unused MAX_LEN defines, reduce size of stored attr_list
 * 	[1994/10/14  16:15:20  cuti]
 * 
 * Revision 1.1.2.6  1994/09/22  20:08:13  cuti
 * 	Check NULL pointer and 0 length before do memcpy in macros.
 * 	[1994/09/22  02:55:00  cuti]
 * 
 * Revision 1.1.2.5  1994/08/08  14:36:27  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/08  14:35:50  mdf]
 * 
 * Revision 1.1.2.4  1994/07/15  22:04:56  baum
 * 	Added schema_entry_flags field
 * 	[1994/07/15  22:04:29  baum]
 * 
 * Revision 1.1.2.3  1994/06/17  18:42:04  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:42  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/11  19:07:04  ahop
 * 	hp_sec_to_osf_2 drop
 * 	 handle BND_SVRNAME, better binding checking
 * 	 support for get_effective
 * 	[1994/04/29  20:49:34  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 	 handle BND_SVRNAME, better binding checking
 * 	 support for get_effective
 * 
 * 	hp_sec_to_osf_2 drop
 * 	 handle BND_SVRNAME, better binding checking
 * 	 support for get_effective
 * 
 * $EndLog$
 */
/*
 *
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 *
 */
/*
 * Macros and types used by rsdb_attr.c, rsdb_attr_schema.c, rsdb_attr_srch.c
 * and callers of rsdb_attr code.
 */

#ifndef rsdb_attr_pvt_h__included
#define rsdb_attr_pvt_h__included

#include <dce/sec_attr_base.h>

#include <rsdb_pvt.h>



/*
 * Macro's 
 */
#define RSDB_ATTR_STRING_MAX_LEN	sec_rgy_name_max_len
#define RSDB_ATTR_PRINTSTRING_MAX_LEN	RSDB_ATTR_STRING_MAX_LEN

#define RSDB_ATTR_PRINTSTRING_OK(S)	( (S) && \
    u_strlen(S) <= RSDB_ATTR_PRINTSTRING_MAX_LEN )

#define MALLOC_OR_FATAL(type, item, length)				\
if (!(item = (type *) malloc(length))) 					\
{									\
	fatal("Could not allocate memory.");				\
}

/* Calling routines should set "allocator" to rpc_ss_allocate
 * if the memory must be freed automatically by the server stub code.
 */
#define RSDB_ATTR_ALLOCATE(allocator,size) \
        (allocator ? (*(allocator))((size)) : (*(malloc))((size)))

/*
 * RSDB_ATTR_MALLOC_ZERO: malloc P of length L.
 * If successful, memset buffer to zero.
 *
 * P is a pointer to be malloc-ed
 * L is the length to be malloc-ed
 * STP is a pointer to an error_status_t
 */
#define RSDB_ATTR_MALLOC_ZERO(P,L,STP) {		\
    if(!((P) = malloc(L))) {			\
        SET_STATUS(STP, sec_attr_no_memory);	\
    } else {					\
        memset((P), NULL, (L));			\
    } }

/*
 * RSDB_ATTR_COPY_ADV_SRC
 * memcpy from S into T for length L.
 * Advance pointer S by L.
 * (A temporary ptr is used for the pointer
 * arithmetic to ensure the pointer is
 * advanced L bytes, not L * size_of_other_structure.)
 *
 * T is the target pointer
 * S is the source pointer
 * L is the length to memcpy
 */
#define RSDB_ATTR_COPY_ADV_SRC(T,S,L)   {       \
    Pointer     ptr;                            \
    if ( (Pointer)(T) && (L) ) {                \
        memcpy((Pointer)(T), (Pointer)(S), (L));    \
        ptr = (Pointer)(S);                         \
        ptr += (L);                                 \
        (S) = ptr;                                  \
	}			                    \
    }

/*
 * RSDB_ATTR_COPY_NULL_ADV_SRC
 * memcpy from T into S for length L.
 * Null-terminate the string in T.
 * Advance pointer S by L.
 * (A temporary ptr is used for the pointer
 * arithmetic to ensure the pointer is
 * advanced L bytes, not L * size_of_other_structure.)
 *
 * T is the target pointer
 * S is the source pointer
 * L is the length to memcpy
 */
#define RSDB_ATTR_COPY_NULL_ADV_SRC(T,S,L)      {       \
    Pointer     ptr;                                    \
    memcpy((Pointer)(T), (Pointer)(S), (L));            \
    memset(((Pointer)(T))+(L), NULL, 1);                \
    ptr = (Pointer)(S);                                 \
    ptr += (L);                                         \
    (S) = ptr;                                          \
    }

/*
 * RSDB_ATTR_COPY_ADV_TGT
 * memcpy from S into T  for length L.
 * Advance pointer T by L.
 * (A temporary ptr is used for the pointer
 * arithmetic to ensure the pointer is
 * advanced L bytes, not L * size_of_other_structure.)
 *
 * T is the target pointer
 * S is the source pointer
 * L is the length to memcpy
 */
#define RSDB_ATTR_COPY_ADV_TGT(T,S,L)   {       \
    Pointer     ptr;                            \
    if ( (Pointer)(T) && (L) ) {                         \
        memcpy((Pointer)(T), (Pointer)(S), (L));    \
        ptr = (Pointer)(T);                         \
        ptr += (L);                                 \
        (T) = ptr;                                  \
    }                                               \
    }


/*
 * ATTRIBUTE and SCHEMA TYPES 
 */

/* SCHEMA ENTRY RELATIONS:
 *
 * The rsdb_attr_schema_entry_t can be retrieved, directly or
 * indirectly via sequential id, attribute name, or type_uuid:
 *
 * 	Record Type	Keys		Data
 *      -----------     ----		----
 *	SEQUENTIAL	attr_type_id	rsdb_schema_entry_t
 *	ATTR_NAME	attr_name	attr_type_id
 *	UUID		attr_type_uuid	attr_type_id
 */

/*
 *  r s d b _ a t t r _ n a m e _ k e y _ t
 *
 * Attribute Name Key - contains attribute name used as
 * the key to retrieve the schema entry sequential_key
 * used to retrieve the schema entry.
 */
typedef struct rsdb_attr_name_key_t {
    short           record_type;	/* ATTR_NAME_TYPE -> 'n' */
    long            name_len;
    sec_rgy_name_t  name;
} rsdb_attr_name_key_t;

/* R S D B _ A T T R _ S C H _ F L A G S _ T
 *
 * Internal rsdb sch_entry flags for use by the back-end 
 * schema entry import/export and storage code.
 * The flag constants are defined in rsdb_attr_schema.c as:
 *	const unsigned32 rsdb_attr_sch_flag_none = 0x00000000;
 *	const unsigned32 rsdb_attr_sch_flag_trig_binding = 0x00000001;
 */
typedef unsigned32  rsdb_attr_sch_flags_t;

/*
 * R S D B _ A T T R _ S C H E M A _ E N T R Y _ T
 *
 * Following is a diagram of the layout of variable data
 * in the rsdb schema_entry buf[] field.
 * Symbols:
 *	{field} - the value of field, never null-terminated, in
 *		  consecutive bytes.
 *	| - diagramatic separater indicates fields before and
 *	    after are concatenated with no space between them.
 *	<<comment>> - descriptive comment, not a part of stored data
 *
 * All "lengths" are stored as unsigned32s.
 *
 * {attr_name} | 
 * {acl_mgr_type1} | {q_perms1} | {u_perms1} | {t_perms1} | {d_perms1} |
 * ...
 * {acl_mgr_typeN} | {q_permsN} | {u_permsN} | {t_permsN} | {d_permsN} |
 * {auth_info_svr_princ_name} |
 * <<N trigger bindings are stored.  The format depends on binding type:>>
 * <<string_binding>>
 * {bind_type} | {str_bind_length} | {str_binding} |
 * <<svrname>>
 * {bind_type} | {svrname_length} | {svrname} |
 * <<twr_set>>
 * {bind_type} | {count} | {twr1_len} | {twr1_string} | ...
 *                  ...  | {twrN_len} | {twrN_string} |
 * {scope} |
 * {comment}
 */
 
typedef struct rsdb_attr_schema_entry_t {
    rsdb_thread_hdr_t      		threads;
    rsdb_pvt_id_t			acl_id;	/* acl for this entry */
    unsigned32         			sch_entry_len;
    unsigned32         			attr_name_len;
    uuid_t	 			attr_id;
    sec_attr_encoding_t			attr_encoding;
    unsigned32			 	acl_mgr_num;
    sec_attr_sch_entry_flags_t		schema_entry_flags;
    sec_attr_intercell_action_t		intercell_action;
    unsigned32 				trig_types;
    sec_attr_bind_auth_info_type_t	trig_auth_info_type;
    unsigned32				trig_svr_princ_name_len;
    unsigned32				trig_protect_level;
    unsigned32				trig_authn_svc;
    unsigned32				trig_authz_svc;
    unsigned32				trig_binding_num;
    unsigned32				scope_len;
    unsigned32         			comment_len;	
    rsdb_attr_sch_flags_t		db_flags;
    char         			buf[VARYING];
} rsdb_attr_schema_entry_t;



/* Attribute Instance Sequential ID 
 */
typedef struct rsdb_attr_inst_id_t {
    unsigned int    deleted:1;
    unsigned int    unused:1;
    unsigned int    id:30;
} rsdb_attr_inst_id_t;

/*
 *  r s d b _ a t t r _ i n s t _ t
 *
 *  threaded on sequential list for its attribute type
 *  threads.alias = attr_type_id (seq ID of sch_entry for this attr type)
 */
typedef struct rsdb_attr_inst_t {
    rsdb_thread_hdr_t   threads;
    unsigned32          attr_inst_len;  /* length of this buffer */
    sec_rgy_domain_t	domain;		/* domain of attr's object */
    rsdb_pvt_id_t       object_id;      /* object to which attr is attached */
    sec_attr_encoding_t encoding;
    char                buf[VARYING];
} rsdb_attr_inst_t;


/* ATTR_LIST - structure holds a list of sequential ids
 * identifying the attribute instances (rsdb_attr_inst_t)
 * (along with the seq ids of their attribute types)
 * attached to a given registry object (PGO, policy, replist, etc).
 * Attribute lists are stored by sequential key.
 *
 * To optimize for queries, the ids of multi-valued
 * instances are stored in consecutive slots in a single
 * attr_list for a given object.
 * (This consecutive storage optimization not yet implemented.)
 */
#define RSDB_MAX_ATTR_LIST_SIZE 10     /* Must fit in a short */

/* RSDB_ATTR_LIST_SIZE - L is a pointer to an rsdb_attr_list_t
 */
#define RSDB_ATTR_LIST_SIZE(L)       ( sizeof(rsdb_attr_list_t) + \
    (((L)->total_count - 1) * sizeof((L)->attr[0])) )

typedef struct rsdb_attr_list_element_t {
    rsdb_pvt_id_t       type_id;
    rsdb_attr_inst_id_t inst_id;
} rsdb_attr_list_element_t;

typedef struct rsdb_attr_list_t {
    rsdb_pvt_id_t               next;
    short                       total_count;    /* num slots used in attr[] */
    short                       count;          /* num valid elts in attr[] */
    rsdb_attr_list_element_t    attr[VARYING];
} rsdb_attr_list_t;

typedef struct rsdb_max_attr_list_t {
    rsdb_pvt_id_t               next;
    short                       total_count;    /* num slots used in attr[] */
    short                       count;          /* num valid elts in attr[] */
    rsdb_attr_list_element_t    attr[RSDB_MAX_ATTR_LIST_SIZE];
} rsdb_max_attr_list_t;


/* r s d b _ a t t r _ s c h _ l i s t _ f l a g s _ t
 *
 * Schema List Element Flags
 * flags.deleted - set to 0 initially when sch_entry added.
 *      set to 1 if this entry is removed from the list
 * flags.mv_update - Set to 0 if only one instance of this attr_type
 *	appears in the current attr_update request;
 *	Set to 1 if multiple instances of this attr_type appear
 *      in the current attr_update request.
 */
typedef struct rsdb_attr_sch_list_flags_t {
    unsigned int    deleted:1;
    unsigned int    mv_update:1;
    unsigned int    unused:30;
} rsdb_attr_sch_list_flags_t;


/* r s d b _ a t t r _ s c h _ l i s t _ e l e m e n t _ t  
 *
 * Arrays of these elements are created and passed by
 * rsdb operations that require the schema entries for
 * an associated array of attribute instances.
 * This structure is not stored; it's used only as a parameter.
 *
 * flags.deleted - set to 0 initially when sch_entry added.
 *	set to 1 if this entry is removed from the list
 * flags.mv_update - Set to 0 if only one instance of this attr_type
 *	appears in the current attr_update request;
 *	Set to 1 if multiple instances of this attr_type appear
 *      in the current attr_update request.
 * sch_entry_id - sequential id of this schema entry
 * sch_entry - the sec_attr_schema_entry_t.
 *
 */
typedef struct rsdb_attr_sch_list_element_t {
    rsdb_attr_sch_list_flags_t	flags;
    rsdb_pvt_id_t		sch_entry_id;
    sec_attr_schema_entry_t	sch_entry;
} rsdb_attr_sch_list_element_t;

/* 
 * r s d b _ a t t r _ s c h _ l i s t _ t  
 *
 * A list of rsdb_attr_sch_list_element_t structures for
 * use by rsdb operations to pass the schema entry info
 * that is associated with an array of attribute instances
 * being operated upon.  
 * This list is used only as a parameter; it is not stored.
 * This structure should be allocated for the correct number
 * of entries; don't make it an automatic variable since it
 * is so huge.
 * 
 * size - size of allocated "entries" array
 * count - number of sch_list_elements in "entries" array
 * entries - array containing rsdb_attr_sch_list_element_ts
 */
#define RSDB_ATTR_SCH_LIST_MAX_ENTRIES	1024

/* RSDB_ATTR_SCH_LIST_SIZE - L is a pointer to an rsdb_attr_sch_list_t.
 * N is number of entries required in the rsdb_attr_sch_list_t.
 */
#define RSDB_ATTR_SCH_LIST_SIZE(L,N)	( sizeof((L)->size) + \
    sizeof((L)->count) + ((N) * sizeof((L)->entries[0])) )

typedef struct rsdb_attr_sch_list_t {
    short                       	size;
    short                       	count;
    rsdb_attr_sch_list_element_t	entries[RSDB_ATTR_SCH_LIST_MAX_ENTRIES];
} rsdb_attr_sch_list_t;

/* 
 * r s d b _ a t t r _ s c h _ a t t r _ x r e f _ t
 *
 * This cross-reference table associates an attribute
 * (in a related array of attribute instances) with
 * a corresponding schema entry (in a related rsdb_attr_sch_list_t).
 * Where i = index of attr_instance in attrs[], and
 * where j = index of schema entry in sch_list.entries[],
 * then xref[i] = j.
 * 
 * This structure is used only as a parameter; it is not stored.
 * This structure should be allocated for the correct number
 * of entries; don't make it an automatic variable since it
 * is so huge.
 */
typedef signed32 rsdb_attr_sch_attr_xref_t[RSDB_ATTR_SCH_LIST_MAX_ENTRIES];

#endif /* rsdb_attr_pvt_h__included */
