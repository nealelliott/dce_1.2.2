/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsp.h,v $
 * Revision 1.1.630.2  1996/02/18  22:56:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:44  marty]
 *
 * Revision 1.1.630.1  1995/12/08  00:21:36  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:34 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/09  19:18 UTC  tatsu_s
 * 	Submitted rfc31.0: Single-threaded DG client and RPC_PREFERRED_PROTSEQ.
 * 
 * 	HP revision /main/tatsu_s.st_dg.b0/2  1994/12/07  14:34 UTC  tatsu_s
 * 	Fixed group/profile members.
 * 
 * 	HP revision /main/tatsu_s.st_dg.b0/1  1994/12/05  18:54 UTC  tatsu_s
 * 	RPC_PREFERRED_PROTSEQ: Initial version.
 * 	[1995/12/08  00:00:10  root]
 * 
 * Revision 1.1.628.4  1994/07/29  16:19:24  tom
 * 	Remove DNSPI_V2 ifdef.
 * 	[1994/07/28  22:34:24  tom]
 * 
 * Revision 1.1.628.3  1994/02/08  21:40:12  mori_m
 * 	CR 9701:  Initial submission for RPC runtime I18N support
 * 	[1994/02/08  21:40:01  mori_m]
 * 
 * Revision 1.1.628.2  1994/01/26  22:12:50  tom
 * 	Don't bump RPC_C_ATTR_MAX up until codeset registry code has
 * 	been submitted.
 * 	[1994/01/26  21:52:21  tom]
 * 
 * Revision 1.1.628.1  1994/01/21  22:38:44  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:22  cbrooks]
 * 
 * Revision 1.1.4.9  1993/03/01  19:31:24  sommerfeld
 * 	[7351]
 * 	     Add two new fields to rpc_lkup_node_t and rpc_ns_inq_rep_t:
 * 	     classversion_checked and
 * 	     classversion_check_status.  These two extra fields allows
 * 	     the status of a previous call to rpc__ns_check_rpc_class_version
 * 	     for this inquiry context to be retained.  Retaining this allows
 * 	     for the RPC_ClassVersion attribute to only be read once for each
 * 	     namespace entry when calling rpc_ns_binding_lookup_next with this
 * 	     lookup node context.
 * 	[1993/02/25  01:03:16  sommerfeld]
 * 
 * Revision 1.1.4.8  1993/01/08  21:47:34  weisman
 * 	Resubmit Grace Hsiao changes.
 * 	[1993/01/07  21:46:16  weisman]
 * 
 * Revision 1.1.4.4  1992/11/16  22:20:19  zeliff
 * 	Re-submitting Grace Hsiao's changes.
 * 	[1992/11/16  22:19:12  zeliff]
 * 
 * Revision 1.1.4.2  1992/11/05  16:37:56  grace
 * 	     28-sep-92    gh      Add rpc__ns_check_name_syntax() for checking name
 * 	                          syntax in both RPC_RESOLVE_SYNTAX and
 * 	                          RPC_RESOLVE_NAME_AND_SYNTAX macros.
 * 	[1992/11/05  15:26:53  grace]
 * 
 * Revision 1.1.2.2  1992/05/01  16:39:08  rsalz
 * 	 3-mar-92    wh      DCE 1.0.1 merge.
 * 	23-jan-92    ebm     Remove dependency on CDS' cts attribute.
 * 	[1992/05/01  16:30:45  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:12:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _NSP_H
#define _NSP_H
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      NSP.H
**
**  FACILITY:
**
**      DECrpc Name Service independent Interface (NSI)
**
**  ABSTRACT:
**
**      Private name service runtime typedefs, constant definitions, etc.
**
**
*/


#include <dce/dnsclerk.h>
#include <nsinit.h>        /* Externals for NS Initialization sub-component */

/* 
 * 1-14-94 CBrooks 
 * we purposely include the file <nsutil.h> at the *end* of the file 
 * because the prototype for rpc__ns_check_name_syntax must be in 
 * scope for any other function that uses the following set of defines
 * that actually expand into code (another argument for inline functions
 * and/or C++!) However, nsutil.h depends on types declared in this 
 * file!. So, we take the easy way out, and include the prototype file 
 * at the end, once we're done with everything else
 */

/*
 * The value that indicates whether or not the Name Service has previously
 * been initialized. Its definition is in nsp.c.
 */

EXTERNAL boolean        rpc_ns_initialized;

/*
 *  MACROS & DEFINITIONS
 */

/*
 * The following macros check to see if NULL values were passed for
 * either the name service name or the name syntax, and if so obtain
 * the default values from the appropriate environment variables.
 */

#define RPC_RESOLVE_NAME_AND_SYNTAX(name, syntax, status)                     \
{                                                                             \
    *status = rpc_s_ok;                                                       \
    if ((name == NULL) || ((strlen ((char *) name)) == 0))                    \
    {                                                                         \
        char *tmp;                                                            \
                                                                              \
        if ((name = (unsigned_char_p_t) getenv ("RPC_DEFAULT_ENTRY")) == NULL)\
        {                                                                     \
            *status = rpc_s_no_env_setup;                                     \
        }                                                                     \
        else                                                                  \
        {                                                                     \
           if ((tmp = (char *) getenv ("RPC_DEFAULT_ENTRY_SYNTAX")) == NULL)  \
           {                                                                  \
               syntax = rpc_c_ns_syntax_dce;                                  \
           }                                                                  \
           else                                                               \
           {                                                                  \
               syntax = (unsigned32) (atoi (tmp));                            \
               if (!(rpc__ns_check_name_syntax (syntax, status)))             \
               {                                                              \
                   return;                                                    \
               }                                                              \
           }                                                                  \
        }                                                                     \
    }                                                                         \
    else                                                                      \
    {                                                                         \
        RPC_RESOLVE_SYNTAX (syntax, status);                                  \
    }                                                                         \
}

#define RPC_RESOLVE_SYNTAX(syntax, status)                                    \
{                                                                             \
    *status = rpc_s_ok;                                                       \
    if (syntax == rpc_c_ns_syntax_default)                                    \
    {                                                                         \
	char *tmp;                                                            \
        if ((tmp = (char *) getenv ("RPC_DEFAULT_ENTRY_SYNTAX")) == NULL)     \
        {                                                                     \
            syntax = rpc_c_ns_syntax_dce;                                     \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            syntax = (unsigned32) (atoi (tmp));                               \
            if (!(rpc__ns_check_name_syntax (syntax, status)))                \
            {                                                                 \
                return;                                                       \
            }                                                                 \
        }                                                                     \
    }                                                                         \
    else                                                                      \
    {                                                                         \
        if (!(rpc__ns_check_name_syntax (syntax, status)))                    \
        {                                                                     \
            return;                                                           \
        }                                                                     \
    }                                                                         \
}                                                 


/*
 * A macro which calls rpc__ns_init if it hasn't been called yet.
 */
#define RPC_NS_VERIFY_INIT()                                                 \
{                                                                             \
    if (rpc_ns_initialized == false)                                          \
    {                                                                         \
        RPC_VERIFY_INIT();                                                    \
        rpc__ns_init();                                                       \
    }                                                                         \
}

/* 
 * NS Constants 
 */

/*
 * Import/Lookup priorities for a profile element.
 */
#define RPC_C_NS_PRIORITY_MIN       ((unsigned32) 0)
#define RPC_C_NS_PRIORITY_MAX       ((unsigned32) 7)
#define RPC_C_NS_PRIORITY_DEFAULT   ((unsigned32) 0)

/* 
 * NS Attribute string constants
 */

#define RPC_C_NS_DNA_TOWERS         ((unsigned_char_p_t) "1.3.22.1.3.30")
#define RPC_C_NS_CLASS_VERSION      ((unsigned_char_p_t) "1.3.22.1.1.1")
#define RPC_C_NS_OBJECT_UUIDS       ((unsigned_char_p_t) "1.3.22.1.1.2")
#define RPC_C_NS_GROUP              ((unsigned_char_p_t) "1.3.22.1.1.3")
#define RPC_C_NS_PROFILE            ((unsigned_char_p_t) "1.3.22.1.1.4")
#define RPC_C_NS_CODESETS           ((unsigned_char_p_t) "1.3.22.1.1.5")


/* 
 * NS Attribute value string constant
 * (used for class value for object creation)
 */
#define RPC_C_NS_ENTRY              ((unsigned_char_p_t) "RPC_Entry")

/*
 * NS Attribute constants used to index into attribute table.
 */

/*
 * This macro is used to return the specific name service attribute
 * in opaque representation.
 *
 * Note, the name service indice is decremented by one since we want
 * rpc_ns_dec_dns to be the first entry, rather than the rpc_ns_none.
 */
#define RPC_NS_LOOKUP_ATTRIBUTE(name_service, attribute) \
    (&(rpc_g_attr_table[(name_service)-1][(attribute)].attr_name))

#define RPC_C_NS_MAX                1

#define RPC_C_ATTR_CLASS_VERSION    0
#define RPC_C_ATTR_DNA_TOWERS       1
#define RPC_C_ATTR_OBJECT_UUIDS     2
#define RPC_C_ATTR_GROUP            3
#define RPC_C_ATTR_PROFILE          4
#define RPC_C_ATTR_CODESETS         5
#define RPC_C_ATTR_MAX              6


/*
 * The max size of a name service entry name - this should be the
 * same as the DNS full name limit + 1 (to accomodate a NUL terminator)
 */

#define RPC_C_ENTRY_NAME_MAX        (DNS_STR_FNAME_MAX+1)

/*
 * The max size of a simple name used for attribute values - this should be
 * the same as the DNS simple name limit + 1 (to accomodate a NUL terminator)
 */

#define RPC_C_ATTR_NAME_MAX         (DNS_STR_SNAME_MAX+1)

/*
 * The (fixed) size of the annotation field in a profile element plus
 * one to accomodate the NUL terminator.
 * This is derived from the maximum size of the user specified part
 * of the interface name (17)
 */

#define RPC_C_ANNOTATION_MAX        18

/*
 * Define an internal name syntax constant to be used in the internal
 * name service routines. The value will depend on which version 
 * of DNSPI is being used.
 */
#if OSF_DCE == 1
#define RPC_C_NS_INTERNAL_NAME_SYNTAX rpc_c_ns_syntax_dce
#else
#define RPC_C_NS_INTERNAL_NAME_SYNTAX rpc_c_ns_syntax_dns
#endif

/* 
 * Lower limit (in seconds) of the global default for the
 * expiration age of local name service data is 8 hours.
 */

#define RPC_C_GLB_EXP_AGE_LOWER_LIMIT	28800

/* 
 * Upper limit (in seconds) of the global default for the
 * expiration age of local name service data is 12 hours.
 */

#define RPC_C_GLB_EXP_AGE_UPPER_LIMIT	43200

/*
 * The value that when passed as the
 * expiration_age to a name service call
 * indicates that name service data inquiries should go
 * directly to the server. This results in a local
 * data (or cache) update as a consequence.
 */

#define RPC_C_NS_BYPASS_CACHE 0

/* 
 * TYPE DEFINITIONS
 */


/*
 * Name Service Entry definitions
 */

typedef struct
{
    unsigned32              name_length;
    unsigned_char_p_t       name;
} rpc_nsentry_t, *rpc_nsentry_p_t;


/*
 * Name Service Attribute definitions
 */

typedef struct 
{
    unsigned char     name[RPC_C_ATTR_NAME_MAX];
} rpc_nsattr_t, *rpc_nsattr_p_t;

typedef struct
{   
    unsigned_char_p_t       attr_string;
    rpc_nsattr_t            attr_name;
} rpc_ns_attributes_t;

/*
 * Name Service Interface version definition
 */
typedef struct
{
    unsigned8               vers_major;
    unsigned8               vers_minor;
} rpc_ns_version_t;

/*
 * Name Service UUID Definitions
 */
typedef struct 
{
    uuid_t                  uuid;
} rpc_nsuuid_t, *rpc_nsuuid_p_t;


/*
 * Profile Definitions
 */

/*
 * interface list elements
 */
typedef struct
{
    rpc_list_t              link;
    rpc_if_id_t             if_id;
} rpc_if_list_elt_t, *rpc_if_list_elt_p_t;


/* 
 * Runtime Profile Representation
 */
typedef struct
{
    rpc_if_id_t             if_id;
    unsigned8               priority;
    unsigned_char_t         annotation[RPC_C_ANNOTATION_MAX];
    unsigned_char_t         name[RPC_C_ENTRY_NAME_MAX];
} rpc_profile_elt_t, *rpc_profile_elt_p_t;


/*
 * Name Service Profile Representation
 */

/* 
 * Note, this octet string structure is not the full representation.
 * This is due to the annotation field being variable length.
 * The actual layout is as follows:
 * 
 *
 *  rpc_nsuuid_t            if_uuid;        
 *  unsigned16              if_vers_major;
 *  unsigned16              if_vers_minor;
 *  unsigned8               priority;
 *  unsigned8               count;
 *  unsigned_char_t         annotation[count];
 *  unsigned16              name_length;
 *  unsigned_char_t         name[];         - ns entry name
 */

typedef struct
{
    rpc_nsuuid_t            if_uuid;        
    unsigned16              if_vers_major;
    unsigned16              if_vers_minor;
    unsigned8               priority;
    unsigned8               count;
    unsigned_char_t         annotation[1];
} rpc_profile_elt_octet_str_t, *rpc_profile_elt_octet_str_p_t;

typedef struct
{
    unsigned32              octet_str_length;
    rpc_profile_elt_octet_str_p_t octet_str;
} rpc_nsprofile_elt_t, *rpc_nsprofile_elt_p_t;


/*
 * types of searches for object UUID
 */
typedef enum
{
    rpc_e_obj_uuid_search_any,
    rpc_e_obj_uuid_search_match
} rpc_obj_search_t;


/*
 * types of searches for ns entry attributes
 */
typedef enum
{
    rpc_e_search_attr_start,
    rpc_e_search_attr_towers,
    rpc_e_search_attr_group,
    rpc_e_search_attr_profile,
    rpc_e_search_attr_codesets,
    rpc_e_search_attr_done
} rpc_search_attr_t;

/* 
 * Data Common to all name service handles.
 */

typedef struct
{
    unsigned32			exp_age; /* cache expiration age */
} rpc_ns_handle_common_t, *rpc_ns_handle_common_p_t;

/*
 * a lookup rep
 */
typedef struct
{
    rpc_ns_handle_common_t  common; /* Data common to ns handles */
    rpc_if_rep_p_t          if_spec;
    rpc_obj_search_t        obj_uuid_search;
    uuid_t                  obj_uuid;
    uuid_t                  obj_for_binding;
    rpc_ns_handle_t	    inq_cntx;
    unsigned32              max_vector_size;
    rpc_list_t              node_list;
    rpc_list_t              non_leaf_list;
    boolean                 first_entry_flag;
    rpc_ns_handle_t         eval_routines;
} rpc_lkup_rep_t, *rpc_lkup_rep_p_t;
    

/*
 * a lookup node list element
 */
typedef struct
{
    rpc_list_t              link;
    rpc_nsentry_t           *name;
    rpc_search_attr_t       type;
    unsigned32              mbrs_count;
    unsigned32              priority_group_count;
    rpc_list_t              mbrs_list;
    boolean		    classversion_checked;
    unsigned32		    classversion_check_status;
} rpc_lkup_node_t, *rpc_lkup_node_p_t;


/*
 * a lookup member list element
 */
typedef struct
{
    rpc_list_t              link;
    rpc_nsentry_t           *name;
    unsigned8               priority;
} rpc_lkup_mbr_t, *rpc_lkup_mbr_p_t;


/*
 * Type definitions used to form Inquiry Context structure.
 */

/*
 * Inquiry specific context for Groups.
 */
typedef struct
{
    rpc_nsentry_t  nsmember_name ;
    unsigned_char_t         member_name[RPC_C_ENTRY_NAME_MAX]; 
} rpc_inqcntx_group_t, *rpc_inqcntx_group_p_t;

/*
 * Inquiry specific context for Profiles.
 */
typedef struct
{
    unsigned32              inquiry_type;
    rpc_if_id_t             if_id;
    unsigned32              if_vers_option;
    unsigned_char_t         member_name[RPC_C_ENTRY_NAME_MAX];
    rpc_nsprofile_elt_t     nsprofile_elt;
    rpc_profile_elt_t       profile_elt;
} rpc_inqcntx_profile_t, *rpc_inqcntx_profile_p_t;

/*
 * Inquiry specific context for Towers.
 */
typedef struct
{
    twr_t                   *tower;
} rpc_inqcntx_tower_t, *rpc_inqcntx_tower_p_t;

/*
 * Inquiry specific context for uuids.
 */
typedef struct
{
    rpc_nsuuid_t            nsobj_uuid;
    uuid_t                  obj_uuid;
} rpc_inqcntx_nsuuid_t, *rpc_inqcntx_nsuuid_p_t;

/*
 * Inquiry specific context for codesets value.
 */
typedef struct
{
    unsigned32              value_length;
    unsigned_char_p_t       *value;
} rpc_inqcntx_codesets_t, *rpc_inqcntx_codesets_p_t;

/*
 * Inquiry Context Usage enumeration
 */
typedef enum 
{ 
    rpc_e_group_member, 
    rpc_e_profile_element, 
    rpc_e_tower_member,
    rpc_e_uuid_member,
    rpc_e_codesets_member
} rpc_inqcntx_usage_t;

/* 
 * Inquiry Context structure used for inquiries made on group members,
 * profile interfaces and members, tower members, and uuid members.
 */
typedef struct rpc_ns_inq_rep_s_t
{
    rpc_ns_handle_common_t  common;	    /* data common to ns handles */
    rpc_inqcntx_usage_t     usage;          /* inquiry usage */
    rpc_nsentry_t           *nsentry;       /* entry name in ns format */
    rpc_nsattr_t            *nsattr;        /* attribute name in ns format */
    unsigned32              member_name_syntax; /* ns syntax of member name */
    dns_handle              dns_context;    /* dns context handle */
    boolean		    classversion_checked;
    unsigned32		    classversion_check_status;
    union
    {
        rpc_inqcntx_group_t     inqcntx_group;
        rpc_inqcntx_profile_t   inqcntx_profile;
        rpc_inqcntx_tower_t     inqcntx_tower;
        rpc_inqcntx_nsuuid_t    inqcntx_nsuuid;
        rpc_inqcntx_codesets_t  inqcntx_codesets;
    } inq_specific;
} rpc_ns_inq_rep_t, *rpc_ns_inq_rep_p_t;

/*
 * Type definition for Import Context structure.
 */

typedef struct
{
    rpc_ns_handle_common_t	common; /* Data common to ns handles */
    rpc_binding_vector_t        *binding_vec;
    rpc_ns_handle_t		lookup_context;
} rpc_import_rep_t, *rpc_import_rep_p_t;


/*
 * All External referenced variables (defined in NSP.C)
 */

/* 
 * Dec DNS Name Service attributes table 
 */
EXTERNAL rpc_ns_attributes_t rpc_g_attr_table[RPC_C_NS_MAX][RPC_C_ATTR_MAX];

/*
 * Dec DNS Name Service Authentication support
 */


/*
 * NS class version number (DNS$CLASSVERSION)
 */
EXTERNAL dnsVersion_t rpc_ns_class_version;

/*
 * NS RPC Class Version structure
 */
 
EXTERNAL rpc_ns_version_t rpc_ns_rpc_class_version;

/*
 * Global ns mutex lock
 */
EXTERNAL rpc_mutex_t ns_lock;


/* 
 * Global expiration age (in seconds) for name service
 * local data refresh.
 */

EXTERNAL unsigned32 rpc_g_ns_exp_age;

/*
 * References to global free list descriptors 
 */

/*
 * The max sizes for the free list descriptors lists - if they get larger
 * than this memory will be returned to heap. Note that these values were
 * chosen by methods that are less than empirical.
 */
#define RPC_C_IF_LIST_MAX           10
#define RPC_C_MEMBERS_LIST_MAX      10
#define RPC_C_LOOKUP_NODES_LIST_MAX 10

EXTERNAL rpc_list_desc_t rpc_g_free_if_list;
EXTERNAL rpc_list_desc_t rpc_g_free_members;
EXTERNAL rpc_list_desc_t rpc_g_free_lookup_nodes;

#include <nsutil.h>

#endif /* _NSP_H */
