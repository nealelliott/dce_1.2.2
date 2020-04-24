/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: sec_login_db.h,v $
 * Revision 1.1.2.2  1996/03/09  20:45:33  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:25:01  marty]
 *
 * Revision 1.1.2.1  1995/12/08  17:29:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:19  root]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1995
 * Unpublished work. All Rights Reserved.
 */
/*
 * sec_login_db.h : data type and interface definitions for 
 *                  sec_login_db.c
 */

/* multiple inclusion guard */
#ifndef sec_login_db__included
#define sec_login_db__included

#include <dce/dbif.h>

#include <Pointer.h>
#include <sec_login_pvt.h>
#include <macros.h>

#ifdef SNI_SVR4
#define DB_DATAFILE_SUFFIX ".db"
#else
#define DB_DATAFILE_SUFFIX ".data.db"
#endif

/*
 * Constructor macro for ".data.db" file
 */
#define DB_DATAFILE_NAME(dfname, ccache) \
    sec_util_string_init(&(dfname), "", \
             krb5_cc_get_name((ccache)), \
             DB_DATAFILE_SUFFIX)

/*
 * Convenience macros
 */
#define DB_V1_AUTHDATA(db_entry) ((db_entry)->contents.v1_authdata)
#define DB_V1_AUTHDATA_ARRAY(db_entry, index) \
		(DB_V1_AUTHDATA(db_entry))->ad[index]

/*
 * Pre DCE 1.1 fixed size login context data 
 */
typedef struct lc_fixed_data {
    context_state_t       state;
    boolean32             local;
    sec_login_auth_src_t  auth_src;
    unsigned32            cell_name_len;
    unsigned32            princ_name_len;

    /* fixed length kerberos data */
    unsigned32            pepper_len;
    int                   num_prev_caches;

    /* fixed length login info */
    policy_info_t         policy;
    login_info_state_t    info_state;
    sec_login_flags_t     flags;

       /* network identity */
    unsigned32            ppac_len;
        /* unix identity */
    boolean32             unix_info_valid;
    signed32              pw_uid;
    signed32              pw_gid;
    unsigned32            pw_quota;
    time_t                pw_change;
    time_t                pw_expire;
    unsigned32            num_unix_groups;
    unsigned32            pw_name_len;
    unsigned32            pw_passwd_len;
    unsigned32            pw_class_len;
    unsigned32            pw_gecos_len;
    unsigned32            pw_dir_len;
    unsigned32            pw_shell_len;
    unsigned32            var_len;
} lc_fixed_data_t;

/*
 * Structure that holds information about which types
 * of login context data and their contents were
 * fetched from the .db file
 */
typedef struct sec_login_db_data {
    unsigned32			num_fetched;
    unsigned32			num_not_fetched;
    sec_login_db_enum_t 	*not_fetched;
    sec_login_db_entry_t	*db_entry;
} sec_login_db_data_t;

/* Number of login context database types */
#define NUM_LC_DB_TYPES	2
extern sec_login_db_enum_t lc_db_types[];

/*
 * Private Function Prototypes
 */
PRIVATE void sec_login_db_copy_entries_from_lc(
    sec_login_context_t         *lcp,           /* [in]  */
    sec_login_db_enum_t         db_type,        /* [in]  */
    sec_login_db_entry_t        *db_entry,      /* [out] */
    error_status_t              *stp            /* [out] */
);

PRIVATE void sec_login_db_copy_entries_to_lc(
    sec_login_db_entry_t        *db_entry,      /* [in]  */
    sec_login_context_t         *lcp,           /* [out] */
    error_status_t              *stp            /* [out] */
);

PRIVATE void sec_db_free_authdata_contents(
    void                                (*dealloc)(idl_void_p_t ptr),
    sec_db_authdata_contents_t          *ac
);

PRIVATE void sec_db_copy_authdata_contents(
    sec_db_authdata_contents_t          *to_ac,    /* [out] */
    sec_db_authdata_contents_t          *from_ac,  /* [in]  */
    error_status_t                      *stp       /* [out] */
);

PRIVATE void sec_login_db_open_file(
    char                        *datafile,	/* [in]  */
    unsigned32                  flags,		/* [in]  */
    int                         mode,		/* [in]  */
    dce_db_convert_func_t       convert,	/* [in]  */
    dce_db_handle_t             *dbh,		/* [out] */
    error_status_t              *stp		/* [out] */
);

/*
 * Public Function Prototypes
 */
void sec_login_db_import_context_data(
    sec_login_context_t  	*lcp,      	/* [in]  */
    lc_fixed_data_t      	*fixed,    	/* [out] */
    Pointer              	*varying,  	/* [out] */
    sec_login_db_entry_t 	**db_entry,	/* [out] */
    error_status_t       	*stp       	/* [out] */
);
 
void sec_login_db_export_context_data(
    lc_fixed_data_t      	*fixed,    	/* [in]  */
    Pointer              	varying,   	/* [in]  */
    sec_login_db_data_t 	*db_data, 	/* [in]  */
    sec_login_context_t  	*lcp,      	/* [out] */
    error_status_t       	*stp       	/* [out] */
);

void sec_login_db_store_entries(
    char		 	*datafile, 	/* [in]  */
    unsigned32			num_db_entries, /* [in]  */
    sec_login_db_entry_t 	*db_entry, 	/* [in]  */
    error_status_t       	*stp       	/* [out] */
);

void sec_login_db_fetch_entries(
    char			*datafile,	/* [in]  */
    unsigned32			num_db_entries, /* [in]  */
    sec_login_db_enum_t         *db_types,      /* [in]  */
    sec_login_db_entry_t 	**db_entry,     /* [out] */
    unsigned32			*num_fetched,	/* [out] */
    unsigned32			*num_not_fetched, /* [out] */
    sec_login_db_enum_t		**not_fetched,	/* [out] */
    error_status_t       	*stp            /* [out] */
);

void sec_login_db_fetch_entry(
    char			*datafile,	/* [in]  */
    sec_login_db_entry_t	*db_entry,	/* [out] */
    error_status_t		*stp		/* [out] */
);

void sec_login_db_handle_fetch_entry(
    dce_db_handle_t             dbh,            /* [in]  */
    sec_login_db_entry_t        *db_entry,      /* [out] */
    error_status_t              *stp            /* [out] */
);

void sec_login_db_free_entries(
    void                        (*dealloc)(idl_void_p_t ptr),
    sec_login_db_entry_t        *db_entry,      /* [in/out] */
    unsigned32                  num_db_entries
);

void sec_login_db_free_entry_contents(
    void                        (*dealloc)(idl_void_p_t ptr),
    sec_login_db_entry_t        *db_entry,      /* [in/out] */
    unsigned32			authdata_index  /* [in] */
);

boolean32 sec_login_db_authdata_search(
    void                        *v1_authdata,   /* [in]  */
    void                        *server,        /* [in]  */
    unsigned32                  *index,         /* [out] */
    error_status_t              *stp            /* [out] */ 
);

void sec_login_db_search_and_store_entry(
    dce_db_handle_t		dbh,		/* [in]  */
    sec_login_db_enum_t		entry_type,	/* [in]  */
    void			*entry,		/* [in]  */
    boolean32			
	(*search) (void *, void *, unsigned32 *, error_status_t *), 
						/* [in]  */
    error_status_t		*stp		/* [out] */
);

void sec_login_db_open_and_lock(
    char			*datafile,	/* [in]  */
    unsigned32			flags,		/* [in]  */
    dce_db_handle_t		*dbh,		/* [out] */
    error_status_t		*stp		/* [out] */
);

void sec_login_db_unlock_and_close(
    dce_db_handle_t             *dbh,           /* [in]  */
    error_status_t              *stp            /* [out] */
);

#endif /* sec_login_db__included */
