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
 * $Log: misc_proto.h,v $
 * Revision 1.1.2.1  1996/10/03  20:25:20  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:20:08  arvind]
 *
 * 	OSF DCE 1.2.2 Drop 4
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:18 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:36 UTC  arvind  /main/arvind_pkss/1]
 * 
 * 	GLORIA (R1.2.2) revision.  (farrell, zee)
 * 	[1996/06/27  23:16:30  zee]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1990-1996
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 * 
 */
/*
 * Function prototypes for CDS server modules (miscellaneous) 
 */

/* server_main.c */
void
server_shutdown(
    nsgbl_ns_t *);


/*
 * dns_service_ncl.c
 */

ch_info_t *
ch_info_malloc (
    nsgbl_ns_t		*);

void
ch_info_free (
    ch_info_t          *);

ch_info_t *
ch_info_next (
    nsgbl_ns_t		*,
    ch_info_t		*);

int 
dns_initialize (
    nsgbl_ns_t     *,
    SimpleName_t   *,
    ObjUID_t       *);

int 
dns_enable (
    nsgbl_ns_t     *);


/*
 * dns_service_rpc.c
 */
void 
cdscp_server_init (
    nsgbl_ns_t  *);

/*
 * get_parentUUID.c
 */
int
get_parentUUID (
    ch_info_t  *,
    FullName_t *,
    ObjUID_t   *,
    int         );


/*
 * mgmt_file.c
 */

void
sys_mgmt_file_load (
    nsgbl_ns_t		*);

dns_status_t
sys_mgmt_file_add (
    nsgbl_ns_t		*,
    char		*);

dns_status_t
sys_mgmt_create_new_file (
    nsgbl_ns_t		*);


/*
 * mgmt_init.c
 */
void
mgmt_init (
    nsgbl_ns_t		*);


/*
 * server_diags.c
 */
void *
server_diag_start (
    nsgbl_ns_t		*);

void
server_diag_stop (
    void		*);

int
server_diag_check_setting (
    diag_state_t *);

char *
server_diag_setting_ascii (
    diag_state_t );


/*
 * server_badclock.c
 */
int
RepairTimestamps_schedule (
    ch_info_t  *);

int
GetRepairStatus ();


/*
 * server_globals.c
 */

void
init_global_structures (
    void);

void
get_database_version (
    VersionNumber_t	*);

void
get_directory_version (
    VersionNumber_t	*);

void
set_directory_version (
    byte_t,
    byte_t);


void
get_server_version (
    VersionNumber_t     *);

void
get_ckpt_parameters (
    unsigned long	*,
    unsigned long	*);

void
get_v4_replica_ts (
    Timestamp_t         *);

void
get_ns_store_cache_size (
    unsigned int        *);

void
get_dir_store_cache_size (
    unsigned int        *);

void
get_dir_store_secondary_cache_size (
    unsigned int        *);

/*
 * rpc_dispatch.c
 */
error_status_t
server_get_client_creds (handle_t      ,
                       rpc_authz_cred_handle_t *,
                       sec_id_pac_t  *,
                       boolean32     *);

error_status_t
get_user_creds (handle_t            ,
              user_descriptor_t  *);


/*
 * unix_cds_net.c
 */

/* original nsi state for security bindings (formally 'BIND_PE_SITE') */
extern boolean32 orig_sec_nsi_state; 

dns_status_t
net_LocalName (
    ObjUID_t		*,
    FullName_t		*);

DBSet_t *
net_get_rpc_towers (
    void		*);

int
net_export_ch (
    ch_info_t		*);

int
net_re_export_ch (
    ch_info_t		*);

void
rpc_register_ch (
    void		*,
    ch_info_t		*);

void
rpc_unregister_ch (
    void		*,
    ObjUID_t		*);

dns_status_t
net_open (
    nsgbl_ns_t		*,
    void		**);

dns_status_t
net_close (
    void		*);


/*
 * unix_lib.c
 */
dns_status_t 
sys_bld_filename (
    FullName_t         *,
    char               *);

int 
sys_set_data_limit (
    int            );


/*
 * unix_towerset_build.c
 */

int
towerset_build (
    byte_t		*);


/*
 * update_lib.c
 */

int
update_delete_build (
    Update_t		*,
    Timestamp_t		*);

int
update_build (
    Update_t		*,
    int			,
    Timestamp_t		*,
    int			,
    AttributeName_t	*,
    unsigned int	*,
    int			,
    int			,
    int			,
    byte_t		*);

int
update_dns_build (
    Update_t		   *,
    int			   ,
    Timestamp_t		   *,
    attribute_descriptor_t *,
    int			   ,
    int			   ,
    byte_t		   *);
