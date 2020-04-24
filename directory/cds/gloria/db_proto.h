/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_proto.h,v $
 * Revision 1.1.2.1  1996/10/03  20:17:11  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:12:42  arvind]
 *
 * 	OSF DCE 1.2.2 Drop 4
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:18 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:36 UTC  arvind  /main/arvind_pkss/1]
 * 
 * 	Add gdb_compare* function prototypes.
 * 	[1996/07/10  18:45:48  zee]
 * 	 *
 * 
 * 	GLORIA (R1.2.2) revision.  (farrell, zee)
 * 	[1996/06/27  23:16:20  zee]
 * 
 * $EndLog$
 */
/*
 * @DEC_COPYRIGHT@
 * Copyright (c) 1990-1996
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 * 
 *
 * Module Description:
 *    Function prototypes for modules named db_*.c 
 */
#ifndef _DB_PROTO_H
#define _DB_PROTO_H

ch_info_t *
db_find_file(ObjUID_t *);


/*
 * db_cleanup.c
 */
void
db_cleanup_record (db_data_t    *,
                   Timestamp_t  *);


/*
 * db_common.c
 */
ch_info_t *
db_find_ch(ObjUID_t *);

void
build_return_data (db_data_t    *, 
		   byte_t        ,
		   ObjUID_t     *,
		   SimpleName_t *);

dns_status_t 
db_setup (ch_info_t  *);

dns_status_t 
db_create (ch_info_t *);

dns_status_t
db_enable (ch_info_t *);

dns_status_t 
write_chlastaddress (ch_info_t  *,
		     db_data_t  *,
                     Set_t      *);

dns_status_t 
db_disable (ObjUID_t *);

void
db_delete (ObjUID_t  *);

void
db_clear (ObjUID_t   *);

void 
db_close (ch_info_t  *);

void 
db_exit (nsgbl_ns_t  *);

dns_status_t
db_begin_operation (user_descriptor_t *);

dns_status_t
db_end_operation (user_descriptor_t *);

/*
 * Transaction begins and ends.
 */
void
db_txn_begin(ch_info_t *);

void
db_txn_commit(ch_info_t *);

void
db_txn_rollback(ch_info_t *);


/*
 * db_compare.c
 */
/*
 * CDS compare functions for GDB use
 */
int gdb_compare_cds_entry_key (int, char *, int, char *);
int gdb_compare_cds_fullname (int, char *, int, char *);
int gdb_compare_uuid (int, char *, int, char *);


/*
 * db_diags.c
 */
int
db_diag_dump_ch_info (
    int           ,
    ch_info_t    *,
    int           (*)(char	*,
                      void	*),
    void         *);

int
db_diag_dump_nsgbl_ns (
    int               ,
    nsgbl_ns_t       *,
    int              (*)(char	*,
                         void	*),
    void             *);

int
db_diag_set_force_back (
    ch_info_t *,
    int        );

int
db_diag_set_inhibit_back (
    ch_info_t *,
    int        );


/*
 * db_directory.c
 */
dns_status_t
db_dir_get_replica_state (db_data_t *);

dns_status_t
db_dir_get_replica_type (db_data_t *);

void
db_dir_get_version (db_data_t  *,
		    version_t  *);

dns_status_t
db_dir_set_state (db_data_t  *,
		  int         ,
		  Update_t   *,
		  int        *);

dns_status_t
db_dir_set_type (db_data_t  *,
		 int         ,
		 Update_t   *,
		 int        *);

dns_status_t
db_dir_insert_dir_store (ch_info_t  *,
			 FullName_t *,
			 ObjUID_t   *);

dns_status_t
db_dir_create (ch_info_t  *,
	       ObjUID_t   *,
	       FullName_t *,
	       db_data_t  *);

dns_status_t
db_dir_read (ch_info_t   *,
	     FullName_t  *,
	     db_data_t   *,
	     byte_t       );

dns_status_t
db_dir_update (ch_info_t *,
	       db_data_t *);

void
db_dir_delete (ch_info_t *,
	       db_data_t *);

dns_status_t 
db_dir_fullname_from_uuid(ch_info_t  *,
			  ObjUID_t   *,
			  FullName_t *);

dns_status_t 
db_dir_change_name(ch_info_t  *,
			  dir_info_t *,
			  FullName_t *);

dns_status_t 
db_dir_read_dir_store (ch_info_t *,
		       dir_info_t **,
		       FullName_t *,
		       ObjUID_t *);
dns_status_t 
db_dir_update_dir_store (ch_info_t *,
			 dir_info_t *,
			 FullName_t *,
			 ObjUID_t *);
dns_status_t 
db_dir_delete_dir_store (ch_info_t *,
		         FullName_t *,
			 ObjUID_t *);


/*
 * db_entry.c
 */
dns_status_t 
db_entry_create (ch_info_t    *,
		 db_data_t    *,
		 ObjUID_t     *,
                 SimpleName_t *);

dns_status_t 
db_entry_read (ch_info_t    *,
	       FullName_t   *,
	       byte_t        ,
	       db_data_t    *);

dns_status_t 
db_entry_read_next (ch_info_t    *,
		    ObjUID_t     *,
		    SimpleName_t *,
		    byte_t       ,
		    SimpleName_t *,
		    byte_t       *,
		    db_data_t    *);
dns_status_t 
db_entry_delete (ch_info_t	*,
		ObjUID_t	*,
		SimpleName_t	*,
		byte_t           );

dns_status_t 
db_entry_update (ch_info_t *,
		 db_data_t *,
		 db_data_t *);

/*
 * db_pseudo.c
 */
dns_status_t 
db_pseudo_create (ch_info_t               *,
                  struct user_descriptor  *,
                  version_t               *);
dns_status_t 
db_pseudo_chdirectory (ch_info_t  *,
                       ObjUID_t   *,
                       int         );

#endif  /* #ifndef _DB_PROTO_H */
