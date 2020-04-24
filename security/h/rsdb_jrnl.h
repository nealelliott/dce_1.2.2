/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_jrnl.h,v $
 * Revision 1.1.7.3  1996/02/18  23:00:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:45  marty]
 *
 * Revision 1.1.7.2  1995/12/08  17:28:13  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:14 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:35 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	changes for dynamic allocation of journal records
 * 	[1995/04/03  22:12 UTC  hondo  /main/SEC_migrate/1]
 * 
 * 	changes for import/export of rsdb data for dynamic allocation.
 * 	[1995/03/28  19:30 UTC  hondo  /main/hondo_mig1/1]
 * 
 * Revision 1.1.2.3  1994/08/29  15:51:31  hondo
 * 	OT 11919 --login activity idl change to support different address types
 * 	[1994/08/28  21:47:59  hondo]
 * 
 * Revision 1.1.2.2  1994/08/10  19:21:46  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:03  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  14:59:46  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:59:33  mdf]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1994
 * Unpublished work. All Rights Reserved.
 */
/*
 * Registry Server - journal information
 */

#ifndef rsdb_jrnl_h__included
#define rsdb_jrnl_h__included

#include <dce/rs_login_activity.h>
#include <rsdb_login.h>
#include <lm_login.h>

#define MAX_NUM_JRNL_ENTRIES 2
#define FOR_READ 1
#define FOR_UPDATE 2

#define GOOD_RECORD_FOUND(x) (x==good_record_found ? 1 : 0)
#define BAD_RECORD_FOUND(x) (x==bad_record_found ? 1 : 0)
#define good_record_found 1
#define bad_record_found 2
#define unknown_record_found 3
#define no_record_found 4
#define SENT_TO_MASTER  1

typedef enum jrn_item_type_t {
                    good_login, bad_login } j_item_type_t;

typedef struct {
    j_item_type_t           type;
    unsigned32              seqno;
    unsigned32              flags;
    short                   count;
    sec_timeval_sec_t       time;
    lm_address_t            origin;
    uuid_t                  reserved;
} jrnl_rec_t;

typedef struct rsdb_acct_journal_t{
    signed32            num_items;
    jrnl_rec_t          member[MAX_NUM_JRNL_ENTRIES];
} rsdb_acct_journal_t;


typedef struct {
    j_item_type_t           type;
    unsigned32              seqno;
    unsigned32              flags;
    short                   count;
    sec_timeval_sec_t       time;
    krb5_int32              origin_addrtype;
    krb5_int32              origin_length;
    unsigned32              origin_ip_addr;
    uuid_t                  reserved;
} db_jrnl_rec_t;

typedef struct rsdb_jrnl_rec_t{
    signed32               num_items;
    db_jrnl_rec_t          member[MAX_NUM_JRNL_ENTRIES];
} rsdb_jrnl_rec_t;

void rsdb_journal_create();

int rsdb_import_jrnl_to_rsdb(
    rsdb_acct_journal_t    *activity,
    rsdb_jrnl_rec_t	   *data,
    error_status_t         *st
);

void export_rsdb_to_jrnl(
    rsdb_acct_journal_t    *activity,
    rsdb_jrnl_rec_t	   *jrnl_rec,
    error_status_t         *st
);

void rsdb_store_journal_activity_by_id(
    rsdb_pvt_id_t      id,
    rsdb_acct_journal_t    *activity
);

boolean32 rsdb_get_journal_activity_by_id(
    rsdb_pvt_id_t      id,
    rsdb_acct_journal_t    *activity
);

void rsdb_purge_journal_activity_by_id(
    rsdb_pvt_id_t      id
);

void rsdb_get_journal_entry(
    rsdb_acct_journal_t    *rec,
    j_item_type_t           id,
    jrnl_rec_t              *entry,
    error_status_t          *st
);


#endif

