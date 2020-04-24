/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ek_db.h,v $
 * Revision 1.1.8.2  1996/03/11  13:26:53  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:49  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:59:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:07  root]
 * 
 * Revision 1.1.6.3  1994/09/09  19:11:58  mob
 * 	Removing the temporary, incomplete ERA support from rgy_edit (OT 11652,11262)
 * 	[1994/09/09  00:22:29  mob]
 * 
 * Revision 1.1.6.2  1994/07/15  14:58:22  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:13:58  mdf]
 * 
 * Revision 1.1.4.3  1992/12/29  12:39:57  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:35:40  zeliff]
 * 
 * Revision 1.1.4.2  1992/11/05  16:22:58  burati
 * 	CR4803 Changes for better error reporting on account changes
 * 	[1992/11/04  22:57:45  burati]
 * 
 * Revision 1.1.2.2  1992/04/03  19:54:39  burati
 * 	CR2419 Add support for local registry.
 * 	[1992/03/31  20:45:08  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  ek_db.h V=22 03/02/92 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit kernel
*/

#ifndef ek_db_h_included
#define ek_db_h_included


/* EDIT KERNEL OPERATIONS IN BRIEF
 * The edit kernel provides operations that read and write data in the
 * registry database.  Each operation has an associated handle.   A handle
 * is an opaque data structure that contains all the data fields associated with
 * an operation.  Data fields are written and read using kernel-supplied read
 * and write routines.  Data fields values are always character strings.  In some
 * cases, the kernel expects the strings to be in a certain format (this is
 *  analagous to typed data in strongly typed programming language).  If the
 * user writes a string  that is not in the correct format, the write operation
 *  returns a "bad format" error (e.g., see the flag field #defines below).  If
 * the field being written  has any constraints that the data does not meet the
 * data will be rejected even  though it is in the right  "format".
 * The steps to perform a kernel operation are.
 *     - obtain a handle for the desired operation
 *     - write any necessary/desired data fields  (check statuses on each write)
 *     - execute the operation (call do_op with the operation handle)
 *     - check status of operation
 *     - read returned data fields (if any)
 *
 * SUPPLYING DEFAULT VALUES
 *
 * The kernel tries very hard to prevent an interface from corrupting the
 * data in the registry server.  Calls to ek_write_field trigger a
 * validation routine for the data in the field being written.  How then, to
 * to allow existing values in the database to be easily used as defaults?
 *
 * A kernel change operation works as follows. (Assume an initialized operation)
 *
 * Any of the change operation's data fields may be written in any order.
 * Constraints will be applied.  When the key field is written, the kernel fills
 * any previously unwritten fields with data retrieved from the registry
 * database.  Fields that  were previously (successfully) written retain the
 * values that were written to them.
 *
 * If the change is on a fully qualified key, the interface writer can simply
 * write the key field first, and present the data filled in by the kernel
 * as the defaults for the remaining non-key fields. If the change key is a
 * wildcard (e.g. an account sid such as %.%.%) the interface writer can accept
 * values for each non-key field without displaying defaults, then lookup all
 * keys that match the wildcard and write them to the change key field
 * iteratively.
 */

#include <dce/lbase.h>
#include <dce/nbase.h>
#include <dce/rgynbase.h>

#include <Pointer.h>

#include <ek_glob.h>

typedef long int ek_op_id_t;       /* the type of an operation identifier */
typedef long int ek_field_id_t;    /* the type of a field specifier */
typedef Pointer  ek_op_handle_t;    /* operation handle type */


/* FUNCTION ek_get_op_handle
 *
 * given an operation id, returns an intialized handle for that operation.
 *
 * EXCEPTIONS:
 *          ek_null_handle - if handle allocation fails for any internal reason
 *          ek_bad_op      - if the operation identifier is undefined
 *
 * RETURNS:
 *          error_status_ok
*/
error_status_t ek_get_op_handle (
#ifdef __STDC__
    ek_op_handle_t  *op_handle,  /* OUT - pointer to an operation handle */
    ek_op_id_t      op_id        /* IN  - id of operation to get a handle for */
#endif
);

/* FUNCTION ek_free_op_handle
 *
 * frees all storage associated with an operation handle.  Subsequent attempts to
 * use the handle will result in disaster.
 */

void ek_free_op_handle (
#ifdef __STDC__
    ek_op_handle_t  *old_handle    /* IN/OUT - pointer to an operation handle */
#endif
);


/* FUNCTION ek_init_op
 *
 * Initializes all data fields associated with an operation.  Subsequent attempts
 * to read data from any field of an initialized operation will return
 * ek_field_no_data.
 *
 * Additional initializations specific to the operation may also occur (E.G.
 * query operation handles contain a marker that is an index into the registry
 * database.  Successive calls with the operation handle return the next item
 * in the database.  The marker is reset to the first item by the ek_init_op
 * call.
 *
 * EXCEPTIONS:
 *          ek_bad_op      - if the operation identifier is undefined
 */
error_status_t ek_init_op (
#ifdef __STDC__
   ek_op_handle_t  op_handle
#endif
);


/* FUNCTION ek_get_op_id
 *
 * returns the operation id of the operation to which a handle is allocated.
 * Disaster will strike if a bogus op_handle is supplied.
 */
ek_op_id_t ek_get_op_id (
#ifdef __STDC__
    ek_op_handle_t op_handle
#endif
);


/* FUNCTION ek_write_field
 *
 * Stores a value in a data field of an operation.  The operation is identified
 * in the operation handle. The user specifies which data field the value should
 * be stored in.  Arbitrarily long data strings can be written as long as they
 * meet the constraints on the field being written.
 *
 * EXCEPTIONS:
 *          ek_bad_field
 *          ek_field_readonly
 *
 * RETURNS:
 *          any validation status relevant to the field being written
*/

error_status_t  ek_write_field (
#ifdef __STDC__
    ek_op_handle_t  op_handle,     /* IN - the operation handle */
    ek_field_id_t   field,         /* IN - the field to be written */
    char            *new_value,    /* IN - the string value to be written */
    long int        new_value_len  /* IN - how long is new_value? */
#endif
);


/* FUNCTION ek_read_field
 *
 * reads a data value from an operation's data set.  The operation handle
 * identifies operation.  The caller specifies which field is read, and
 * supplies a buffer for the data to be written into.  The buffer length
 * variable specifies the buffer size. The data length parameter returns the
 * number of bytes that were in the requested field.  If the data length
 * exceeds the buffer length, the user buffer contains a truncated version of
 * the data.  If the data length does not exceed the buffer length, all of the
 *  field's data is present in the caller's buffer.  If buffer space is
 *  sufficient, the returned data is null terminated.
 *
 * EXCEPTIONS:
 *           ek_bad_field
 *           ek_field_no_data
 *
 * RETURNS:
 *           error_status_ok
 */
void  ek_read_field (
#ifdef __STDC__
    ek_op_handle_t  op_handle,     /* IN - the operation handle */
    ek_field_id_t   field,         /* IN - data field to be read */
    char            *buff,         /* OUT - data is read into this buffer */
    long int        buff_len,      /* IN  - how big is the buffer? */
    long int        *data_len      /* OUT - length of data in the field */
#endif
);


/* FUNCTION ek_field_written
 *
 *  Returns true if the field in question has been succesfully written via
 *  ek_write_field, false otherwise.  After an ek_init_op call, ek_field_written
 *  will return false for all fields.  If ek_field_written is called on an unused
 *  field, or with a bad field id, a kernel error is signaled.
 */

boolean32  ek_field_written (
#ifdef __STDC__
    ek_op_handle_t  op_handle,     /* IN - the operation handle */
    ek_field_id_t   field          /* IN - the field id */
#endif
);

/* FUNCTION ek_field_has_data
 *
 *  Returns true if the field in question has any data.  Differs form ek_field
 * written because it will return even if the field data was kernel-supplied
 * whereas ek_field written only returns true if the data was written by
 * a prior call to ek_write_field
 */

boolean32  ek_field_has_data (
#ifdef __STDC__
    ek_op_handle_t  op_handle,     /* IN - the operation handle */
    ek_field_id_t   field          /* IN - the field id */
#endif
);


/* FUNCTION ek_do_op
**
** takes an operation handle and performs the specified operation using the data
** from the handle's data fields.
**
** EXCEPTIONS:
**          ek_required_field
**          ek_bad_op_id
**
** RETURNS:
**          any status appropriate to the operation being performed
*/

error_status_t ek_do_op (
#ifdef __STDC__
    ek_op_handle_t op_handle   /* IN - the operation handle */
#endif
);


/* ek_key_add
**
**  Adds (sets) a key to a local key table.
**
*/
void ek_key_add (
#ifdef __STDC__
    char            *opt_keyfile,
    char            *principal,
    char            *key,
    boolean32       update_registry,
    error_status_t  *status
#endif
);

/* ek_key_delete
**
**  Deletes a key from a local key table.
**
*/
void ek_key_delete (
#ifdef __STDC__
    char            *opt_keyfile,
    char            *principal,
    unsigned32      key_vno,
    error_status_t  *status
#endif
);

/* ek_start_key_list
**
**  Start at the beginning of a key table
**
*/
void ek_start_key_list (
#ifdef __STDC__
    void            **cursor,
    char            *opt_keyfile,
    char            *principal,
    error_status_t  *status
#endif
);

/* ek_do_key_list
**
**  Get the next key from a key table
**
*/
void ek_do_key_list (
#ifdef __STDC__
    void            *cursor,
    char            **principal,
    unsigned32      *key_vno,
    error_status_t  *status
#endif
);

/* ek_end_key_list
**
**  Close a key table and get free cursor information
**
*/
void ek_end_key_list (
#ifdef __STDC__
    void            **cursor,
    error_status_t  *status
#endif
);

/* ek_login
**
** Log in to specified account
*/
boolean32 ek_login(
#ifdef __STDC__
    char            *login_name,
    char            *password,
    error_status_t  *status
#endif
);

/* ek_login_cleanup
 *
 * purge any remaining credentials established via
 * calls to ek_login.  Need only be called once--on exit.
 */
void  ek_login_cleanup(
#ifdef __STDC__
    void
#endif
);

/*
 * ek_acct_gen_passwd
 */
error_status_t ek_acct_gen_passwd(
    ek_op_handle_t      op_h,
    sec_passwd_type_t   *passwd_type,
    sec_passwd_rec_t    *passwd
);

/*
 * ek_acct_get_pwd_mgmt_info
 */
error_status_t ek_acct_get_pwd_mgmt_info (
    ek_op_handle_t      op_h,
    ek_pwd_val_type_t   *pwd_val_type
);

error_status_t ek_acct_reset_key_field(
#ifdef __STDC__
    ek_op_handle_t    op_h,
    sec_rgy_domain_t  domain
#endif
);

error_status_t ek_get_last_acct_chg_status(
#ifdef __STDC__
    ek_op_handle_t    op_h
#endif
);

void ek_do_lrgy_purge(
#ifdef __STDC__
    unsigned32      *num_purged,
    unsigned32      *num_tgts_purged,
    error_status_t  *status
#endif
);

boolean32 ek_do_lrgy_delete(
#ifdef __STDC__
    unsigned_char_p_t   name,
    error_status_t      *entry_status,
    error_status_t      *tgt_status
#endif
);

void ek_lrgy_entry_release_cursor(
#ifdef __STDC__
    ek_op_handle_t      op_handle,
    boolean32           *modified
#endif
);

/* KERNEL OPERATIONS AND DATA FIELDS
 *
 * Data field ids are actually zero origin offsets to a vector of fields stored
 * in an operation handle.  By convention, a key field definition is always the
 * last field in every set of data fields, so it can be used as a symbolic
 * sentinel value.   Do not change the definitions of field ids unless you know
 * exactly what you're doing,  otherwise, things could get real ugly.  The "n"
 * in the "ekn_" prefix indicates that  the definitions apply to data/operations
 * in/on the network registry database.
*/

/* Person, Group and Org operations and data */

    /* Common fields */
#define ekn_pgo_name              0
#define ekn_pgo_uuid              1   /* ek_uuid_buf_t */
#define ekn_pgo_unix_num          2   /* ek_unix_num_buf_t */
#define ekn_pgo_fullname          3   /* ek_fullname_buf_t */
#define ekn_pgo_is_reserved       4   /* ek_flag_buf_t */

    /* Person specific fields */
#define ekn_pgo_is_an_alias       5   /* ek_flag_buf_t */
#define ekn_pgo_quota             6

    /* Group_specific fields */
#define ekn_pgo_projlist_ok       7   /* ek_flag_buf_t */

#define ekn_pgo_key               8   /* ek_name_buf_t or ek_unix_num_buf_t,
				       * depending on the operation */

    /* These are used by the kernel but might conceivably be useful outside */
#define ekn_pgo_last_field        ekn_pgo_key
#define ekn_pgo_num_fields        (ekn_pgo_last_field + 1)

    /* Person Operations */
#define ekn_person_get_by_name 10
#define ekn_person_get_by_num  11
#define ekn_person_add         12
#define ekn_person_change      13
#define ekn_person_delete      14
#define ekn_person_adopt       15

    /* Group operations */
#define ekn_group_get_by_name  20
#define ekn_group_get_by_num   21
#define ekn_group_add          22
#define ekn_group_change       23
#define ekn_group_delete       24
#define ekn_group_adopt        25

    /* Organization operations */
#define ekn_org_get_by_name    30
#define ekn_org_get_by_num     31
#define ekn_org_add            32
#define ekn_org_change         33
#define ekn_org_delete         34
#define ekn_org_adopt          35


/* ACCOUNT Operation and Field Ids */


    /* Account data fields */
#define ekn_acct_pname                0   /* login name */
#define ekn_acct_gname                1
#define ekn_acct_oname                2
#define ekn_acct_key_parts            3
#define  ekn_acct_sid_person          4   /* person uuid */
#define  ekn_acct_sid_group           5   /* group uuid */
#define  ekn_acct_sid_org             6   /* org uuid */
#define  ekn_acct_unix_sid_person     7   /* person unix id */
#define  ekn_acct_unix_sid_group      8   /* group unix id */
#define  ekn_acct_unix_sid_org        9   /* org unix id */

    /* the following fields are in the user part of the account record.  The
     * changer and when changed fields are maintained by the registry and are
     * always readonly
     */
#define ekn_acct_user_plain_passwd    10
#define ekn_acct_user_enc_passwd      11
#define ekn_acct_user_my_plain_passwd 12
#define ekn_acct_user_my_enc_passwd   13
#define ekn_acct_user_passwd_version  14
#define ekn_acct_user_misc            15
#define ekn_acct_user_homedir         16
#define ekn_acct_user_shell           17
#define ekn_acct_user_passwd_dtm      18    /* password modification date-time */
#define ekn_acct_user_passwd_valid    19    /* flag field */

#define ekn_acct_admin_acct_server    20    /* flag field */
#define ekn_acct_admin_acct_client    21    /* flag field */
#define ekn_acct_admin_acct_valid     22    /* flag field */
#define ekn_acct_admin_creator        23   /* creator sid */
#define ekn_acct_admin_when_created   24   /* date-time account was created */
#define ekn_acct_admin_changer        25   /* last admin part changer sid*/
#define ekn_acct_admin_when_changed   26   /* date-time of admin part change */
#define ekn_acct_admin_exp_date       27   /* date-time account should expire */

    /* Account authorization flags */
#define ekn_acct_admin_auth_post_dated      28  /* Post-dated certificates     */
#define ekn_acct_admin_auth_forwardable     29  /* Forwardable certificates    */
#define ekn_acct_admin_auth_tgt             30  /* Certs via TGT authentication*/
#define ekn_acct_admin_auth_renewable       31  /* Allow Renewable certificates*/
#define ekn_acct_admin_auth_proxiable       32  /* Allow Proxiable certificates*/
#define ekn_acct_admin_auth_dup_session_key 33  /* Allow Duplicate Session keys*/
#define ekn_acct_admin_good_since_date      34  /* For TGT validation          */

    /* Auth policy, per-acct data  */
#define ekn_acct_max_ticket_lifetime        35
#define ekn_acct_max_renewable_lifetime     36

#define ekn_acct_pname_key                  37
#define ekn_acct_gname_key                  38
#define ekn_acct_oname_key                  39

#define ekn_acct_last_field                 ekn_acct_oname_key
#define ekn_acct_num_fields                 (ekn_acct_last_field + 1)

    /* Account operation ids */
#define ekn_acct_get         40
#define ekn_acct_add         41
#define ekn_acct_change      42
#define ekn_acct_delete      43


/* Membership data and operations */

    /* Membership data fields */
#define ekn_mem_name                0   /* person, group or org name */
#define ekn_mem_num_members         1   /* a long integer */
#define ekn_mem_key                 2   /* a person, group or org name */

#define ekn_mem_last_field          ekn_mem_key
#define ekn_mem_num_fields          (ekn_mem_last_field + 1)


    /* Membership Operation Ids */
#define ekn_mem_get_person_groups  50
#define ekn_mem_get_group_members  51
#define ekn_mem_get_org_members    52
#define ekn_mem_add_group_member   53
#define ekn_mem_add_org_member     54
#define ekn_mem_del_group_member   55
#define ekn_mem_del_org_member     56

/* Policy Data and Operations */

    /* Policy Field Ids */
#define ekn_policy_passwd_exp        0    /* date-time - password expiration date */
#define ekn_policy_passwd_life       1    /* int32 - days a password may remain
					      unchanged */
#define ekn_policy_acct_life         2    /* int32 - days an account valid? */
#define ekn_policy_passwd_min_len    3    /* int32 - minimum password length */
#define ekn_policy_passwd_no_spaces  4    /* flag - password cannot be all
					   * spaces */
#define ekn_policy_passwd_non_alpha  5    /* flag - password cannot be all
					   * alphanumeric */
#define ekn_policy_key               6

#define ekn_policy_last_field        ekn_policy_key
#define ekn_policy_num_fields        (ekn_policy_last_field + 1)

    /* Policy operations */
#define ekn_policy_get               60
#define ekn_policy_get_effective     61
#define ekn_policy_set               62


/* Registry properties operations and data - no key field required because
 * there's only logically) one registry
 */

#define ekn_prop_is_readonly       0  /* a flag field - if on then the current site is
					  readonly */
#define ekn_prop_auth_certificates_unbounded    1
#define ekn_prop_shadow_passwd                  2
#define ekn_prop_embedded_unix_id               3
#define ekn_prop_minimum_certificate_lifetime   4
#define ekn_prop_default_certificate_lifetime   5
#define ekn_prop_low_unix_id_person             6
#define ekn_prop_low_unix_id_group              7
#define ekn_prop_low_unix_id_org                8
#define ekn_prop_max_unix_id                    9
#define ekn_prop_realm                          10
#define ekn_prop_last_field                     ekn_prop_realm
#define ekn_prop_num_fields                     (ekn_prop_last_field + 1)

/* PROPERTY operations */

#define  ekn_prop_rgy_get 70
#define  ekn_prop_rgy_set 71

/* Local registry properties operations and data */

#define ekn_prop_lrgy_max_entries               0
#define ekn_prop_lrgy_num_entries               1
#define ekn_prop_lrgy_num_tgt_entries           2
#define ekn_prop_lrgy_lifespan                  3
#define ekn_prop_lrgy_last_field                ekn_prop_lrgy_lifespan
#define ekn_prop_lrgy_num_fields                (ekn_prop_lrgy_last_field + 1)

/* Local registry property operations */

#define  ekn_prop_lrgy_get 72
#define  ekn_prop_lrgy_set 73

/* Override policy operations and data */

#define ekn_override_policy_category  0
#define ekn_override_exclude_passwd   1
#define ekn_override_root_passwd      2
#define ekn_override_other_passwd     3
#define ekn_override_custom_data      4
#define ekn_override_key              5
#define ekn_override_last_field       ekn_override_key
#define ekn_override_num_fields       (ekn_override_last_field + 1)


#define ekn_override_get                        80
#define ekn_override_add                        81   /* not implemented */
#define ekn_override_change                     82
#define ekn_override_delete                     83   /* not implemented */

/* Authorization Policies (keyed by account) */
#define ekn_auth_policy_pname                   0
#define ekn_auth_policy_gname                   1
#define ekn_auth_policy_oname                   2
#define ekn_auth_policy_max_ticket_lifetime     3
#define ekn_auth_policy_max_renewable_lifetime  4
#define ekn_auth_policy_last_field              ekn_auth_policy_max_renewable_lifetime
#define ekn_auth_policy_num_fields              (ekn_auth_policy_last_field + 1)

#define ekn_auth_policy_get                     90
#define ekn_auth_policy_get_effective           91
#define ekn_auth_policy_set                     92

/* Cell operations and data */
#define ekn_cell_pname                          0
#define ekn_cell_lunix_num                      1
#define ekn_cell_funix_num                      2
#define ekn_cell_fullname                       3
#define ekn_cell_lgname                         4
#define ekn_cell_loname                         5
#define ekn_cell_fgname                         6
#define ekn_cell_foname                         7
#define ekn_cell_my_plain_passwd                8
#define ekn_cell_foreign_name                   9
#define ekn_cell_foreign_passwd                 10
#define ekn_cell_admin_exp_date                 11

#define ekn_cell_last_field                     ekn_cell_admin_exp_date
#define ekn_cell_num_fields                     (ekn_cell_last_field + 1)

#define ekn_cell_add                            100

    /* Local registry entry data fields */
#define ekn_lrgy_entry_cell                     0
#define ekn_lrgy_entry_princ                    1
#define ekn_lrgy_entry_acct_exp                 2
#define ekn_lrgy_entry_passwd_exp               3
#define ekn_lrgy_entry_identity_exp             4
#define ekn_lrgy_entry_passwd_valid             5
#define ekn_lrgy_entry_pw_name                  6
#define ekn_lrgy_entry_pw_passwd                7
#define ekn_lrgy_entry_pw_class                 8
#define ekn_lrgy_entry_pw_gecos                 9
#define ekn_lrgy_entry_pw_dir                   10
#define ekn_lrgy_entry_pw_shell                 11
#define ekn_lrgy_entry_pw_uid                   12
#define ekn_lrgy_entry_pw_gid                   13
#define ekn_lrgy_entry_pw_change                14
#define ekn_lrgy_entry_pw_expire                15
#define ekn_lrgy_entry_timestamp                16

#define ekn_lrgy_entry_last_field               ekn_lrgy_entry_timestamp
#define ekn_lrgy_entry_num_fields               (ekn_lrgy_entry_last_field + 1)

#define ekn_lrgy_entry_get                      110

#endif

