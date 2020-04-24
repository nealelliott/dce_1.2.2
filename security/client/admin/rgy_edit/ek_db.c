/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ek_db.c,v $
 * Revision 1.1.13.2  1996/11/13  17:58:21  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/10/28  15:09 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/1]
 *
 * 	Fix duplicate enum name ("none") for pwd_val_type values.
 * 	[1996/05/24  23:14 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:44 UTC  hanfei  /main/DCE_1.2/2]
 *
 * 	coding style change
 * 	[1996/02/28  17:48 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/2]
 *
 * 	multi-cell group work: modify mem_key_validate() to accept global
 * 	principal name as valid.
 * 	[1996/01/31  16:41 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 *
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:46  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/10/18  21:38 UTC  rps  /main/HPDCE02/2]
 *
 * 	Merge.
 * 	[1995/12/08  16:30:02  root]
 *
 * 	merge
 * 	[1993/12/06  23:14:47  rps  1.1.7.2]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/02/23  22:23:00  burati  1.1.4.7]
 *
 * Revision 1.1.13.1  1996/05/10  13:13:24  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:44 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/02/28  17:48 UTC  hanfei
 * 	coding style change
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/01/31  16:41 UTC  hanfei
 * 	multi-cell group work: modify mem_key_validate() to accept global
 * 	principal name as valid.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:06 UTC  psn
 * 	CHFts16642 (cont) additional code move
 * 	[1995/10/17  19:29 UTC  rps  /main/HPDCE02/rps_moth4/2]
 * 
 * 	CHFts16642 intercell config broken
 * 	[1995/10/17  14:12 UTC  rps  /main/HPDCE02/rps_moth4/1]
 * 
 * 	Merge.
 * 	[1994/11/15  21:22 UTC  mullan_s  /main/HPDCE02/1]
 * 
 * 	Removing the temporary, incomplete ERA support from rgy_edit (OT 11652,11262)
 * 	[1994/09/09  00:17:06  mob]
 * 	 *
 * 
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:13:47  mdf]
 * 
 * 	merge
 * 	[1994/01/26  19:50  mdf  /main/HPDCE01/1]
 * 
 * 	[OT9488/CHFts09626] ek_cell_add:  ensure update site
 * 	[1993/12/02  16:06:07  rps]
 * 	 *
 * 
 * 	Loading drop DCE1_0_3b03
 * 
 * 	[1993/09/15  15:35:51  root  1.1.5.3]
 * 
 * Revision 1.1.8.4  1994/09/09  19:11:55  mob
 * 	Removing the temporary, incomplete ERA support from rgy_edit (OT 11652,11262)
 * 	[1994/09/09  00:17:06  mob]
 * 
 * Revision 1.1.8.3  1994/07/15  14:58:18  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:13:47  mdf]
 * 
 * Revision 1.1.6.1  1993/10/05  22:29:34  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  18:51:47  mccann]
 * 
 * Revision 1.1.4.7  1993/02/23  22:23:00  burati
 * 	CR7332 local rgy lifespan should be days, not hours.
 * 	[1993/02/23  21:47:01  burati]
 * 
 * Revision 1.1.4.6  1993/02/05  21:40:37  burati
 * 	In function ek_cell_add() use sec_passwd_str_max_len instead
 * 	of sec_passwd_str_t_size, to avoid writing past end of buffer
 * 	[1993/02/05  18:42:31  burati]
 * 
 * Revision 1.1.4.5  1992/12/29  12:39:45  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:35:28  zeliff]
 * 
 * Revision 1.1.4.4  1992/11/29  00:51:26  burati
 * 	CR3305 Enhancement to allow group aliases
 * 	[1992/11/28  22:25:29  burati]
 * 
 * Revision 1.1.4.3  1992/11/05  16:21:45  burati
 * 	CR5726 Don't allow negative quota's because it's too confusing.
 * 	CR4803 Improve error reporting on ek_acct_change
 * 	[1992/11/04  22:56:23  burati]
 * 
 * Revision 1.1.4.2  1992/10/22  22:29:32  burati
 * 	CR5763 Remove 0 length exclusion on passwd validations
 * 	[1992/10/22  17:49:22  burati]
 * 
 * Revision 1.1.2.5  1992/06/24  14:53:55  burati
 * 	CR4408 Fix uninitialized status vars.
 * 	[1992/06/23  21:59:55  burati]
 * 
 * Revision 1.1.2.4  1992/06/02  15:11:34  burati
 * 	Only save auth_plcy in form if it isn't already there.
 * 	[1992/06/02  02:46:45  burati]
 * 
 * 	CR3918 prevent loss of failure status when adding membership
 * 	[1992/06/01  22:26:25  burati]
 * 
 * Revision 1.1.2.3  1992/04/28  14:32:30  burati
 * 	CR2728 Initialize good_since_date to now, for cell accounts.
 * 	[1992/04/28  03:57:07  burati]
 * 
 * Revision 1.1.2.2  1992/04/03  19:47:27  burati
 * 	CR2419 Add support for local registry.
 * 	[1992/03/31  20:43:17  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  ek_db.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1993
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit edit kernel routines
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)ek_db.c	63 - 03/02/92";
#endif

#include <stdio.h>
#include <ctype.h>
#include <bstring.h>
#include <un_maloc.h>
#include <un_time.h>
#include <un_strng.h>
#include <u_str.h>
#include <rgymacro.h>
#include <sec_def_flags.h>

#include <dce/uuid.h>
#include <dce/rpc.h>
#include <dce/acct.h>
#include <dce/binding.h>
#include <dce/misc.h>
#include <dce/pgo.h>
#include <dce/policy.h>
#include <dce/sec_login.h>
#include <dce/keymgmt.h>
#include <dce/sec_pwd_mgmt.h>

#include <ek_db.h>

#include <form.h>
#include <eif.h>
#include <ek_error.h>
#include <ek_util.h>
#include <ek_types.h>
#include <ek_glob.h>
#include <ek_site.h>
#include <sec_lrgy.h>

/* Context handle retrieved by ek_site code */
extern sec_rgy_handle_t    context;

typedef struct ek_op_header_t {
    ek_op_id_t   operation;
} ek_op_header_t;

typedef struct ek_op_t {
    ek_op_header_t  op_header;          /* the operation the handle applies to */
    form_t          *op_form;           /* the operation's form */
} ek_op_t;

extern void exit (
#ifdef __STDC__
    int  status
#endif
);

/*
** Private prototypes
*/

/* MEMBERSHIP query operations.
**
** There are three kernel  membership queries: ekn_mem_get_person_groups,
** ekn_mem_get_group_members, and ekn_mem_get_org_members.  The operation handle
** field interface is  same for all three (ekn_mem_key, ekn_mem_name), as are the
** PRIVATE rgy_ data buffers.  The only differences are in the validation functions,
** which must perform a lookup in the appropriate domain in order to verify that the
** name in ekn_mem_key actually exists, and the domain on which the
** sec_rgy_pgo_get_members call is executed.
*/

#define MAX_MEMBERS 50  /* max members returned by a single sec_rgy_pgo_get_members
                         * call - it's completely arbitrary. MUST be used
                         * wherever the members array stored in the member
                         * query header is used in a sec_rgy_pgo_get_members call
                         */

/* The following header information is common to all membership query
 * operations
 */

typedef struct member_query_header_t {

        /* The data in the members array persists across invocations of the
         * GET_MEMBER  operations so it absolutely MUST be stored in the header.
         */
    sec_rgy_member_t  members[MAX_MEMBERS];
    signed32          num_returned; /* how many entries were returned in members? */

        /* the following variables are static across operation invocations on
           a per task basis, so we can't declare them in procedure bodies */
    signed32       num_members;   /* total number of members */
    boolean32      first_call;    /* is this the first call with this handle? -
                                   * must be initialized to true */
    long int       cur_member;    /* index into the members array */
    long int       num_read;      /* how many members have we read up to the
                                   * current invocation of do_get_members? Must
                                   * be initialized to zero.
                                   */
    sec_rgy_cursor_t  marker;
} member_query_header_t;


/*
 * Policy declarations
 */

typedef struct policy_header_t {
    ek_op_type_t    op_type;     /* ek_op_query, or ek_op_change */
    sec_rgy_name_t  name;
    sec_rgy_plcy_t  policy;      /* changes only */
} policy_header_t;

typedef struct auth_policy_header_t {
    ek_op_type_t            op_type;    /* ek_op_query, or ek_op_change */
    sec_rgy_login_name_t    name;
    sec_rgy_plcy_auth_t     policy;     /* changes only */
} auth_policy_header_t;


/*
** Properties declarations
*/

/* Unlike the rest of the kernel opertions the properties operations have no key
 * (there is only one registry).   The  model for a change form i.e., load the
 * non-key fields of a form with defaults when the key field is written,
 * preserving any data written by the interface before the key was written,
 * won't work.  Instead,  we do a lookup whenever a property handle is
 * initialized.
 */

typedef struct rgy_prop_header_t {
    ek_op_type_t           op_type;         /* only query or change */
    sec_rgy_properties_t   properties;
} rgy_prop_header_t;

/*
 * Local registry properties declarations
 */
typedef struct lrgy_prop_header_t {
    ek_op_type_t            op_type;        /* only query or change */
    sec_lrgy_hdr_t          properties;
    unsigned32              num_tgt_entries;
} lrgy_prop_header_t;


/*  Cell operation form header info */
typedef struct cell_header_t {
    ek_op_type_t    op_type;

    boolean32       admin_part_written;
    boolean32       user_part_written;
    boolean32       passwd_written;
    boolean32       my_passwd_written;

        /* the key is used by all operations */
    sec_rgy_login_name_t  key;
    sec_rgy_login_name_t  foreign_key;

        /* buffers */
    sec_rgy_acct_user_t   user_part;
    sec_rgy_acct_admin_t  admin_part;

        /* Only plaintext password updates using the next available key version
         * are currently possible via the edit kernel interface. If that
         * changes, some new header fields will be needed.  For now, the fields
         * of the header passwd rec are initialized accordingly, when the passwd
         * field is written.
         */
    sec_passwd_rec_t        passwd;
    sec_passwd_str_t        plain_passwd;           /* buffer for plain arm of
                                                     * passwd rec union */
    sec_passwd_type_t       key_type;               /* sec_passwd_des for now*/
    sec_passwd_rec_t        my_passwd;
    sec_passwd_str_t        my_plain_passwd;        /* buffer for plain arm of
                                                     * passwd rec union */
    sec_rgy_name_t          foreign_name;           /* Admin acct in foreign
                                                     * cell. */
    sec_passwd_str_t        foreign_passwd;         /* Password of admin acct
                                                     * in foreign cell */
    sec_rgy_pgo_item_t      item;
    sec_rgy_pgo_item_t      foreign_item;
} cell_header_t;


/*
 * Account operation declarations
 */

/*  Account operation form header info */
typedef struct acct_header_t {
    ek_op_type_t    op_type;

    boolean32       pname_key_written;
    boolean32       gname_key_written;
    boolean32       oname_key_written;
    boolean32       new_pname_key_written;
    boolean32       new_gname_key_written;
    boolean32       new_oname_key_written;
    boolean32       admin_part_written;
    boolean32       user_part_written;
    boolean32       plain_passwd_written;
    boolean32       passwd_written;
    boolean32       policy_written;

        /* 
         * information regarding the password management
         * policy of a principal. This information is for
         * internal purposes to figure out whether a new
         * password should be generated by the password
         * management server.
         */
    sec_pwd_mgmt_handle_t pwd_mgmt_h;
    ek_pwd_val_type_t     pwd_val_type;

        /* used only by queries */
    sec_rgy_cursor_t      marker;

        /* the key is used by all operations */
    sec_rgy_login_name_t  key;

        /* buffers */
    sec_rgy_acct_key_t    key_parts;
    sec_rgy_acct_user_t   user_part;
    sec_rgy_acct_admin_t  admin_part;

        /* only plaintext password updates using the next available key version
         * are currently possible via the edit kernel interface. If that
         * changes, some new header fields will be needed.  For now, the fields
         * of the header passwd rec are initialized accordingly, when the
         * passwd field is written.
         */
    sec_passwd_rec_t      passwd;               /* used for updates only */
    sec_passwd_rec_t      my_passwd;            /* used for updates only */
    sec_passwd_str_t      plain_passwd;         /* buffer for "plain" arm of
                                                 * passwd rec union */
    sec_passwd_str_t      my_plain_passwd;
    sec_passwd_version_t  passwd_version_number;/* always read only - assigned
                                                 * by the server when the
                                                 * password is changed
                                                 */
    sec_passwd_type_t     new_key_type;         /* sec_passwd_des, for now */
    sec_rgy_plcy_auth_t   policy;
    sec_rgy_login_name_t  new_key;              /* apply only to change ops */
    error_status_t        last_status;
} acct_header_t;

#define ACCT_KEY_WRITTEN(acct_header) \
            ((acct_header).pname_key_written && \
             (acct_header).gname_key_written && \
             (acct_header).oname_key_written)   \

#define ACCT_NEW_KEY_WRITTEN(acct_header) \
            ((acct_header).new_pname_key_written || \
             (acct_header).new_gname_key_written || \
             (acct_header).new_oname_key_written)   \

/*
 * Local registry declarations
 */

/*  lrgy acct entry operation form header info */
typedef struct _lrgy_entry_header_t {
    void                *cursor;
    sec_lrgy_entry_t    entry;
    sec_timeval_sec_t   timestamp;
} lrgy_entry_header_t;

/* default registry policy */

static sec_rgy_plcy_t Def_policy =
     { ek_live_forever,
       0,
       ek_live_forever,
       ek_live_forever,
       0 };

/* default registry auth policy */

static sec_rgy_plcy_auth_t Def_auth_policy =
     { ek_live_forever,
       ek_live_forever };

/* Begin routine definitions */

/*
 * read_rgy_policy
 */
PRIVATE error_status_t read_rgy_policy
#ifndef __STDC__
    (policy_rec)
    sec_rgy_plcy_t    *policy_rec;
#else
  (
    sec_rgy_plcy_t    *policy_rec
  )
#endif
{
    error_status_t  st;
    sec_rgy_name_t  null_name;

    null_name[0] = '\0';
    CLEAR_STATUS(&st);

    sec_rgy_plcy_get_info(context, null_name, policy_rec, &st);
    if (BAD_STATUS(&st))
        eku_translate_status(&st);
    return st;
}


PRIVATE error_status_t chk_pgo_exists
#ifndef __STDC__
    (name, domain)
    char                *name;
    sec_rgy_domain_t    domain;
#else
  (
    char                *name,
    sec_rgy_domain_t    domain
  )
#endif
{
    uuid_t  dummy_uuid;
    error_status_t st;

    sec_rgy_pgo_name_to_id(context, domain, (unsigned char *)name,
                            &dummy_uuid, &st);
    if (STATUS_EQUAL(&st,sec_rgy_object_not_found)) {
        switch (domain) {
        case sec_rgy_domain_person:
            SET_STATUS(&st, ek_err_person_does_not_exist);
            break;
        case sec_rgy_domain_group:
            SET_STATUS(&st, ek_err_group_does_not_exist);
            break;
        case sec_rgy_domain_org:
            SET_STATUS(&st, ek_err_org_does_not_exist);
            break;
        }
    } else
        eku_translate_status(&st);

    return st;
}


/* FUNCTION field_written
 *
 * Returns true when a field's status is form_field_valid_data, false otherwise.
 * Logically, true menas the interface has written a valid value to the field.
 * No error checking so the caller had better pass in a real form and
 * field_offset.
 */
PRIVATE boolean32 field_written
#ifndef __STDC__
    (form, field_offset)
    form_t               *form;
    form_field_offset_t  field_offset;
#else
  (
    form_t               *form,
    form_field_offset_t  field_offset
  )
#endif
{
    form_field_status_t  field_status;

    form_field_status(form, field_offset, &field_status);
    if (field_status == form_field_valid_data)
        return true;
    else
        return false;
}

/* CLEAR_BITSET
 * zeros out all the bits in an arbitrary (single integral) bitset.
 */
#define CLEAR_BITSET(bitset) ( (bitset) = 0 )


/* BITSET_COPY
 * copies bit settings from src_bitset to dest_bitset according to the settings
 * in mask, i.e. for each bit in src_bitset, if the corresponding bit in mask is
 * set then copy that bit from src_bitset to dest_bitset.
 *
 *     - zero all the masked bits in the dest_bitset leaving unmasked bits alone
 *     - zero all the unmasked bits in the src_bitset flags leaving masked bits
 *           alone
 *     - or the results  of 1 and 2 and store in dest_bitset
 */
#define BITSET_COPY(src_bitset, dest_bitset, mask) \
     ( (dest_bitset) = (((src_bitset) & (mask)) | ((dest_bitset) & ~(mask))) )


/******************************************************************************
 ******************************* OPERATION DEFINITIONS ************************
 ******************************************************************************/

/*
 * Here's where we get in to the specifics of operations.  For each operation
 * the following routines are defined.
 *
 * FORM ALLOCATION ROUTINE.
 *   A routine to allocate and initialize the operation's form.
 *
 * FORM CLEARING ROUTINE
 *  A routine that re-initializes the operation's form.  It's often possible to
 *  clear multiple forms with a single routine (e.g., all pgo name queries have
 *  the same state information and can share a single form clearing routine.
 *
 * ACTION ROUTINE
 *   A routine to perform the operation.  An action routine typically does the
 *   folowing things.
 *      - Checks any constraints that can't be checked on a field-by-field basis
 *      - Converts input field strings to rgy_types
 *      - Calls the appropriate rgy_ function(s)
 *      - Converts output fields from rgy_ types to strings.
 *
 * For each field in a kernel operation's form the following things are defined.
 *
 * INTERNAL DATA BUFFERS
 *  The field data sets passed to the kernel need to be converted to rgy_ types
 *  for dispatch to the registry client agent.  Buffers of the appropriate
 *  internal types are defined.  Care is taken to ensure that kernel operations
 *  behave well in a tasking environment.  Buffers that need not persist across
 *  invocations of an operation are local variables in the operation's action
 *  routine.  Data that must persist across operation invocations are stored in
 *  the operation's form header (e.g., sec_rgy_cursor_t markers).
 *
 * VALIDATION ROUTINES
 *  Validation routines can be supplied with each field.  If one is supplied,
 *  it is invoked whenever the field is written via write_field.  Validation
 *  routines enforce data integrity constraints that are applicable to the
 *  field being written. Constraints fall into to general categories: 1)
 *  constraints that can be verified using only the value in the data field
 *  (stand-alone constraints), and  2) constraints that require knowledge of
 *  values in an operation data field other than the one being written
 *  (intra-record constraints).
 *
 *  As an example of an intra-record constraint, consider an add account
 *  operation. Policy constraints on the password field of an account cannot be
 *  enforced unless the organization component of the account name is available.
 *  The organization component is part of the account key.  If the account key
 *  field has been written before the password field, the password validation
 *  routine can check all constraints.  If not, the password policy check must
 *  be performed by the action routine.  For this reason, action and validation
 *  routines must share knowledge about the validation "state" of fields with
 *  intra-record constraints (Or the action routine must simply re-apply all
 *  intra-record constraints every time it is invoked)
 *
 *  The validation routine interface defined by the  form.c  module is very 
 *  simple.  A validation routine accepts a form_t string and a string_length 
 *  and returns a error_status_t.  It is often possible, with additional 
 *  parameters, to collect the validation code for fields of a particular type 
 *  (e.g. pgo names) in a single routine that applies across all pgo operations.
 *  In these cases, the actual validation routines become simple wrappers 
 *  around the more general one.
 */

/*******************************************************************************
 * PGO Operations
 *******************************************************************************/
typedef struct pgo_header_t {
    sec_rgy_name_t      name;
    sec_rgy_cursor_t    marker;           /* queries only */
    sec_rgy_domain_t    domain;           /* person, group, or org */
    ek_op_type_t        op_type;          /* query. add, chang, or delete */
    sec_rgy_name_t      key;
    long                unix_num_key;     /* queries only */
    sec_rgy_pgo_item_t  item;             /* used by adds and changes */
    sec_rgy_pgo_flags_t flags;            /* need to keep actual value of the
                                           * projlist flag around until
                                           * ek_pgo_change is called so we
                                           * know if it's been toggled
                                           */
} pgo_header_t;

/* FUNCTION copy_unwritten_pgo_data
 *  Copies each field that hasn't been written by the interface from the src to
 *  the destination pgo item.  UUIDs are always copied.
 */
PRIVATE void copy_unwritten_pgo_data
(
    form_t              *form,
    sec_rgy_pgo_item_t  *src_item,
    sec_rgy_pgo_item_t  *dest_item
)
{
    sec_rgy_pgo_flags_t  writeset;   /* which pgo flags have been written */

        /* can't change uuids so just copy it */
    dest_item->id = src_item->id;

    if (!field_written(form, (form_field_offset_t)  ekn_pgo_unix_num))
        dest_item->unix_num = src_item->unix_num;

    if (!field_written(form, (form_field_offset_t) ekn_pgo_fullname)) {
        strcpy((char *)dest_item->fullname,
                (char *)src_item->fullname);
    }

    if (!field_written(form, (form_field_offset_t) ekn_pgo_quota)) {
        dest_item->quota = src_item->quota;
    }

    CLEAR_BITSET(writeset);
    if (field_written(form, (form_field_offset_t) ekn_pgo_is_an_alias))
        SET(writeset,sec_rgy_pgo_is_an_alias);

    if (field_written(form, (form_field_offset_t) ekn_pgo_projlist_ok))
        SET(writeset, sec_rgy_pgo_projlist_ok);

    if (field_written(form, (form_field_offset_t) ekn_pgo_is_reserved))
        SET(writeset, sec_rgy_pgo_is_required);
    BITSET_COPY(src_item->flags, dest_item->flags, ~writeset);

}

/* LOCAL FUNCTION pgo_query_unconvert
 * Takes a pgo_item containing the results of a pgo_query, a form containing
 * the fields for a person, group, or organization query, and a domain
 * specifier.  Converts the data in the pgo_item into string values and
 * copies those values to the appropriate fields in the form.
 */
PRIVATE void pgo_query_unconvert
  (
    form_t              *pgo_form,
    sec_rgy_domain_t    pgo_domain,
    sec_rgy_pgo_item_t  *pgo_item,
    sec_rgy_name_t      name
  )
{
        /* first the common fields */
    if (!field_written(pgo_form, (form_field_offset_t) ekn_pgo_name))
        form_write(pgo_form, ekn_pgo_name, name, strlen((char *)name),
                   false, &Dummy_st);
    if (!field_written(pgo_form, (form_field_offset_t) ekn_pgo_uuid))
        eku_dec_uuid(pgo_form, ekn_pgo_uuid, pgo_item->id);
    if (!field_written(pgo_form, (form_field_offset_t) ekn_pgo_unix_num))
        eku_dec_unix_num(pgo_form, ekn_pgo_unix_num, pgo_item->unix_num);
    if (!field_written(pgo_form, (form_field_offset_t) ekn_pgo_fullname))
        form_write(pgo_form, ekn_pgo_fullname, pgo_item->fullname,
                    strlen((char *)pgo_item->fullname), false, &Dummy_st);
    if (!field_written(pgo_form, (form_field_offset_t) ekn_pgo_is_reserved))
        eku_dec_pgo_flag(pgo_form, ekn_pgo_is_reserved, pgo_item->flags,
                          sec_rgy_pgo_is_required);

    switch (pgo_domain) {    /* now the domain specific fields */

    case sec_rgy_domain_person:
        if (!field_written(pgo_form, (form_field_offset_t) ekn_pgo_is_an_alias))
            eku_dec_pgo_flag(pgo_form, ekn_pgo_is_an_alias, pgo_item->flags,
                            sec_rgy_pgo_is_an_alias);
        if (!field_written(pgo_form, (form_field_offset_t) ekn_pgo_quota))
            if (pgo_item->quota == sec_rgy_quota_unlimited)
                form_write(pgo_form,ekn_pgo_quota,"unlimited",9,false,&Dummy_st);
            else
                eku_dec_int32(pgo_form, ekn_pgo_quota, pgo_item->quota);
        break;

    case sec_rgy_domain_group:
        if (!field_written(pgo_form, (form_field_offset_t) ekn_pgo_is_an_alias))
            eku_dec_pgo_flag(pgo_form, ekn_pgo_is_an_alias, pgo_item->flags,
                            sec_rgy_pgo_is_an_alias);
        if (!field_written(pgo_form, (form_field_offset_t) ekn_pgo_projlist_ok))
            eku_dec_pgo_flag(pgo_form, ekn_pgo_projlist_ok, pgo_item->flags,
                              sec_rgy_pgo_projlist_ok);
        break;
    }
}


/* PGO validation routines */

/* FUNCTION pgo_key_validate
 *
 * For adds the key must not exist.  For changes, the key must already
 * exist.  Required entries can't be deleted.
 * Query operations just require the basic check.
 *
 * returns
 *    ek_err_person_bad_name_chars
 *    ek_err_person_name_too_long
 *    ek_err_person_exists          - add of a person that already exists
 *    ek_err_person_does_not_exist  - change/delete of non-existent person
 *
 *    ek_err_group_bad_name_chars
 *    ek_err_group_name_too_long
 *    ek_err_group_exists           - add of a group that already exists
 *    ek_err_group_does_not_exist   - change/delete of non-existent group
 *
 *    ek_err_org_bad_name_chars
 *    ek_err_org_name_too_long
 *    ek_err_org_exists             - add of an org that already exists
 *    ek_err_org_does_not_exist     - change/delete of non-existent org
 *
 *    ek_err_reserved               - delete of reserved name
 */
PRIVATE error_status_t pgo_key_validate
#ifndef __STDC__
    ( pgo_form, name )
    form_t    *pgo_form;
    char      *name;
#else
  (
    form_t    *pgo_form,
    char      *name
  )
#endif
{
    error_status_t      st;
    sec_rgy_cursor_t    marker;
    pgo_header_t        header;
    sec_rgy_pgo_item_t  temp_item;
    sec_rgy_domain_t    domain;
    ek_op_type_t        op_type;
    boolean32           del_unknown = false;

       /* get the state info out of the header */
    form_read_header(pgo_form,  (Pointer)&header);

    op_type = header.op_type;
    domain = header.domain;

    if (op_type == ek_op_query) {
            /* zero-length names are a wildcard for "show everything" */
        if (strlen(name) == 0) {
            *(header.key) = '\0';
            form_write_header(pgo_form, (Pointer)&header);
            SET_STATUS(&st, error_status_ok);
            return st;
        }
    }

         /* first make sure the name meets basic restrictions */
    st = eku_chk_name(name, domain);

        /* if the name is bad or we're checking a query key then we're done */
    if ( BAD_STATUS(&st) )
        return st;
    else if (op_type == ek_op_query) {
        strcpy((char *)header.key, name);
        form_write_header(pgo_form, (Pointer)&header);
        return st;
    }

        /* try to find the named pgo item in the relevant domain - use a
         * temporary pgo item for the query results because we only overwrite
         * (i.e. supply default values for) those fields in the header item
         * that haven't already been written by the interface */
    sec_rgy_pgo_get_by_name(context, domain, (unsigned char *)name, &marker,
                         &temp_item, &st);

        /* how we interpret the results of the lookup depends on the operation */
    switch (op_type) {

    case ek_op_change:
    case ek_op_delete:
             /* changed/deleted object must be present in the requisite domain */
        if (STATUS_EQUAL(&st,sec_rgy_object_not_found)) {
            if (op_type != ek_op_delete) {
                switch (domain) {
                case sec_rgy_domain_person:
                    SET_STATUS(&st, ek_err_person_does_not_exist);
                    break;
                case sec_rgy_domain_group:
                    SET_STATUS(&st, ek_err_group_does_not_exist);
                    break;
                case sec_rgy_domain_org:
                    SET_STATUS(&st, ek_err_org_does_not_exist);
                    break;
                default:   /* sanity check */
                    SET_STATUS(&st, ek_kernel_failure);
                    break;
                }
            }
            else {
                SET_STATUS(&st, error_status_ok);
                del_unknown = true;
            }
        } else if (GOOD_STATUS(&st)) {
                /* cache the flag values in the header flag variable */
            header.flags = temp_item.flags;
        } else
            eku_translate_status(&st);
        break;

    case ek_op_add:
            /* added object can't already exist */
        if (STATUS_EQUAL(&st, sec_rgy_object_not_found))
            SET_STATUS(&st, error_status_ok);
        else if (GOOD_STATUS(&st)) {
            switch(domain) {
            case sec_rgy_domain_person:
                SET_STATUS(&st, ek_err_person_exists);
                break;
            case sec_rgy_domain_group:
                SET_STATUS(&st, ek_err_group_exists);
                break;
            case sec_rgy_domain_org:
                SET_STATUS(&st, ek_err_org_exists);
                break;
            default:   /* sanity check */
                SET_STATUS(&st, ek_kernel_failure);
                break;
            }
        } else
            eku_translate_status(&st);    /* just report anything else */
        break;
    }

    if (GOOD_STATUS(&st)) {
        switch (op_type) {

        case ek_op_delete:
            if (! del_unknown) {
                /* can't delete reserved names */
                if (FLAG_SET(temp_item.flags, sec_rgy_pgo_is_required))
                    SET_STATUS(&st, ek_err_reserved);
                else
                    /* load the delete form with the results of the name lookup */
                    pgo_query_unconvert(pgo_form, domain, &(temp_item),
                                        (unsigned_char_t *) name);
            }
            break;

        case ek_op_add:
            break;    /* nothing more to do */

        case ek_op_change:
            /* If the query succeeded, then the results become the default
             * values for any fields not previously written by the
             * interface
             */
            copy_unwritten_pgo_data(pgo_form, &temp_item, &header.item);

            /* write the query results to the change form so they're available to
             *   the interface as defaults (via ek_read_field)
             */
            pgo_query_unconvert(pgo_form, domain, &(header.item),
                                (unsigned_char_t *) name);
            break;

        }
    }

        /* if the key is valid, write the header info back to the form */
    if (GOOD_STATUS(&st)) {
            /* we've got the key in hand, so store it in the internal data set
               in the operation header */
        if (op_type == ek_op_add) {
            strcpy((char *)header.name, name);
        } else {
            strcpy((char *)header.key, name);
        }
        form_write_header(pgo_form, (Pointer)&header);
    }

    return st;
}


/* FUNCTION pgo_unix_num_validate
 *
 * Each add/change in a pgo domain requires checking that the unix num doesn't
 * already exist in the specified domain (except in the case of persons where
 * duplicate unix nums are okay as long as the added/changed entry is an alias -
 * this restriction can't be checked when the unix number field is written).
 * The domain is irrelevant when the op_type is ek_op_query,
 * and op_type = ek_op_delete doesn't make any sense at all.
 *
 * returns:
 *    ek_err_bad_unix_num    - if the unix number fails basic domain/range tests
 *    ek_err_unix_num_exists - if the unix num is already defined (groups and
 *                             orgs only)
 *    any status values related to communication or server failures.
 */
PRIVATE error_status_t pgo_unix_num_validate
#ifndef __STDC__
    ( pgo_form, unum_str )
    form_t    *pgo_form;
    char      *unum_str;
#else
  (
    form_t    *pgo_form,
    char      *unum_str
  )
#endif
{
    error_status_t    st;
    uuid_t            dummy_uuid;
    signed32          unum;
    pgo_header_t      header;
    sec_rgy_domain_t  domain;
    ek_op_type_t      op_type;

    SET_STATUS(&st, error_status_ok);

    form_read_header(pgo_form,  (Pointer)&header);
    domain = header.domain;
    op_type = header.op_type;

          /* unix_num field isn't used by delete operations */
    if (op_type == ek_op_delete) {
        SET_STATUS(&st, ek_kernel_failure);
        return st;
    }

    /* perform basic validation for query/add/change operations in all
     * domains.
     */
    if (!eku_chk_unix_num(unum_str, strlen(unum_str), &unum)) {
        SET_STATUS(&st, ek_err_bad_unix_num);
        return st;
    }

    if (op_type == ek_op_query) {  /* nothing more to do */
        header.unix_num_key = unum;
        form_write_header(pgo_form, (Pointer)&header);
        return st;
    }

        /* enforce the non-existence constraint */
    switch (domain) {
    case sec_rgy_domain_person:
    case sec_rgy_domain_group:
            /* alias capability means we can't enforce non-existence constraint
             * on a person unix num until the operation is performed
             */
        break;

    case sec_rgy_domain_org:

        sec_rgy_pgo_unix_num_to_id(context, domain, unum, &dummy_uuid, &st);

            /* should have gotten "object not found" */
        if (STATUS_EQUAL(&st, sec_rgy_object_not_found))
            SET_STATUS(&st, error_status_ok);
        else if (GOOD_STATUS(&st))
            SET_STATUS(&st, ek_err_unix_num_exists);
        else
            eku_translate_status(&st);    /* just report anything else */

        break;

    default:   /* sanity check */
        SET_STATUS(&st, ek_kernel_failure);
        break;
    }

    if (GOOD_STATUS(&st)) {
            /* store the unix num now while we have it */
        header.item.unix_num = unum;
        form_write_header(pgo_form, (Pointer)&header);
    }

    return st;
}


/*
 * FUNCTION quota_validate - Validate/update the quota field
 */
PRIVATE error_status_t quota_validate
#ifndef __STDC__
    (form, quota_str)
    form_t    *form;
    char      *quota_str;
#else
  (
    form_t    *form,
    char      *quota_str
  )
#endif
{
    error_status_t  st;
    pgo_header_t    header;
    signed32        quota;

    SET_STATUS(&st, error_status_ok);
    form_read_header(form,  (Pointer)&header);

    /* Quotas only make sense in the person domain */
    if (header.domain == sec_rgy_domain_person) {
        if (strcmp(quota_str, "unlimited") == 0) {
            header.item.quota = sec_rgy_quota_unlimited;
            form_write_header(form,  (Pointer)&header);
        } else {
            st = eku_chk_integer32 ( quota_str, strlen(quota_str), &quota );
            if (quota < 0) {
                SET_STATUS(&st, tty_input_out_of_range);
            }
            if (GOOD_STATUS(&st)) {
                header.item.quota = quota;
                form_write_header(form,  (Pointer)&header);
            }
        }
    }
    else {
        SET_STATUS(&st, ek_misc_bad_domain);
    }

    return st;
}


/* p_adopt_unum_validate
 *
 * adopting unix numbers is very much like adding unix numbers except that
 * an adopted unix number must not already exist.  In a person add, the
 * unix number may already exist if the person is an alias.  Rather than add
 * an "adopt" enumeration to the ek_op_type_t typedef, we simply supply a
 * special unix number validation routine fpr adopts.  It's the only special
 * one required.  All other fields can be validated using the standard add
 * validation routines.
 */
PRIVATE error_status_t adopt_unum_validate
#ifndef __STDC__
    (form, unum_str)
    form_t    *form;
    char      *unum_str;
#else
  (
    form_t    *form,
    char      *unum_str
  )
#endif
{
    error_status_t  st;
    uuid_t          dummy_uuid;
    signed32        unum;
    pgo_header_t    header;

    SET_STATUS(&st, error_status_ok);

     if (!eku_chk_unix_num(unum_str, strlen(unum_str), &unum)) {
        SET_STATUS(&st, ek_err_bad_unix_num);
        return st;
    }

     form_read_header(form,  (Pointer)&header);

        /* enforce the non-existence constraint */
    sec_rgy_pgo_unix_num_to_id(context, header.domain, unum, &dummy_uuid, &st);

        /* should have gotten "object not found" */
    if (STATUS_EQUAL(&st, sec_rgy_object_not_found))
        SET_STATUS(&st, error_status_ok);
    else if (GOOD_STATUS(&st))
        SET_STATUS(&st, ek_err_unix_num_exists);
    else
        eku_translate_status(&st);    /* just report anything else */

    if (GOOD_STATUS(&st)) {
            /* store the unix num now while we have it */
        header.item.unix_num = unum;
        form_write_header(form, (Pointer)&header);
    }

    return st;
}


/* FUNCTION pgo_uuid_validate
 *  UUIDs are explicit parameters only in adopt operations.  UUIDs being added
 *  must not exist in any pgo domain.
 *
 * returns:
 *    ek_err_bad_id    - if the uuid fails basic domain/range tests
 *    ek_err_id_exists - if the uuid is already defined in the any domain.
 *    any status values related to communication or server failures.
 */
PRIVATE error_status_t pgo_uuid_validate
#ifndef __STDC__
    (pgo_form, uuid_str)
    form_t    *pgo_form;
    char      *uuid_str;
#else
  (
    form_t    *pgo_form,
    char      *uuid_str
  )
#endif
{
    error_status_t      st;
    signed32            unum;
    pgo_header_t        header;
    sec_rgy_domain_t    domain;

    CLEAR_STATUS(&st);
    form_read_header(pgo_form,  (Pointer)&header);

      /* perform basic validation on the uuid string. */
    if (!eku_chk_uuid(uuid_str, &(header.item.id)))
        SET_STATUS(&st, ek_err_bad_id);
    else {
        for (domain = sec_rgy_domain_person; domain < sec_rgy_domain_last; domain++) {
            sec_rgy_pgo_id_to_unix_num(context, domain, &(header.item.id), &unum, &st);
            if (STATUS_EQUAL(&st, sec_rgy_object_not_found))
                SET_STATUS(&st, error_status_ok);
            else if (STATUS_EQUAL(&st, error_status_ok))
                SET_STATUS(&st, ek_err_id_exists);
            else
                eku_translate_status(&st);

            if (BAD_STATUS(&st))
                break;
        }
    }

    if (GOOD_STATUS(&st))
        form_write_header(pgo_form,  (Pointer)&header);

    return st;
}


/* FUNCTION pgo_name_validate
 *
 *  The ekn_pgo_name field is only writeable by change operations.
 *  The name field represents the new name the pgo entry will have if the change
 *  succeeds, therefore the name must not already exist in the specified domain.
 *  Name fields on reserved pgo items can't be changed, but we can't enforce that
 *  restriction unitl we have the entire record in hand.  The validation
 *  functions for each separate domain are wrappers around this one.
 *
 * returns:
 *    ek_err_person_bad_name_chars
 *    ek_err_person_name_too_long
 *    ek_err_person_exists
 *
 *    ek_err_group_bad_name_chars
 *    ek_err_group_name_too_long
 *    ek_err_group_exists
 *
 *    ek_err_org_bad_name_chars
 *    ek_err_org_name_too_long
 *    ek_err_org_exists
 */
PRIVATE error_status_t pgo_name_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    error_status_t        st;
    uuid_t                dummy_uuid;
    pgo_header_t          header;

    CLEAR_STATUS(&st);

    form_read_header(form,  (Pointer)&header);

    st = eku_chk_name(name, header.domain);

    if (GOOD_STATUS(&st)) {
        sec_rgy_pgo_name_to_id(context, header.domain, (unsigned_char_t *)name,
                            &dummy_uuid, &st);
            /* new (changed) name must be unique */
        if (STATUS_EQUAL(&st, sec_rgy_object_not_found)) {
            SET_STATUS(&st, error_status_ok);
        } else if (GOOD_STATUS(&st)) {
            switch (header.domain) {
            case sec_rgy_domain_person:
                SET_STATUS(&st, ek_err_person_exists);
                break;
            case sec_rgy_domain_group:
                SET_STATUS(&st, ek_err_group_exists);
                break;
            case sec_rgy_domain_org:
                SET_STATUS(&st, ek_err_org_exists);
                break;
            default:   /* sanity check */
                SET_STATUS(&st, ek_kernel_failure);
                break;
            }
        } else {
            eku_translate_status(&st);
        }
    }

    if (GOOD_STATUS(&st)) {
        strcpy((char *)(header.name), name);
        form_write_header(form,  (Pointer)&header);
    }

    return st;
}


/* FUNCTION g_projlist_flag_validate
 *
 * The projlist flag applies only to groups.
 *
 * returns:
 *    ek_bad_format
 */
PRIVATE error_status_t g_projlist_flag_validate
#ifndef __STDC__
    ( form, flag )
    form_t    *form;
    char      *flag;
#else
  (
    form_t    *form,
    char      *flag
  )
#endif
{
    pgo_header_t     header;
    boolean32       flag_set;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);

    if (flag_set)
        SET(header.item.flags, sec_rgy_pgo_projlist_ok);
    else
        UNSET(header.item.flags, sec_rgy_pgo_projlist_ok);

    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION is_an_alias_validate - The alias flag applies only to princ,groups.
 */
PRIVATE error_status_t is_an_alias_validate
#ifndef __STDC__
    ( form, flag )
    form_t    *form;
    char      *flag;
#else
  (
    form_t    *form,
    char      *flag
  )
#endif
{
    pgo_header_t    header;
    boolean32       flag_set;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);

    if (flag_set)
        SET(header.item.flags, sec_rgy_pgo_is_an_alias);
    else
        UNSET(header.item.flags, sec_rgy_pgo_is_an_alias);

    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION pgo_fullname_validate - The only restriction on fullnames is length
 */
PRIVATE error_status_t pgo_fullname_validate
#ifndef __STDC__
    ( form, fullname )
    form_t    *form;
    char      *fullname;
#else
  (
    form_t    *form,
    char      *fullname
  )
#endif
{
    pgo_header_t    header;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    if (strlen((char *)fullname) > ek_lim_max_fullname_len) {
        SET_STATUS(&st, ek_err_data_too_long);
        return st;
    }

    if (GOOD_STATUS(&st)) {
        form_read_header(form,  (Pointer)&header);
        strcpy((char *)(header.item.fullname), fullname);
        form_write_header(form,  (Pointer)&header);
    }

    return st;
}

/* PGO QUERY OPERATION DEFINITIONS */

/*
 * LOCAL FUNCTION clear_pgo_form
 *
 * All pgo query operation forms have the same state information in the header;
 * namely, the database marker.  That means a common subroutine can be used to
 * clear all of them.
 */
void clear_pgo_form
#ifndef __STDC__
    (pgo_form)
     form_t *pgo_form;
#else
  (
     form_t *pgo_form
  )
#endif
{
    pgo_header_t pgo_header;

    form_read_header(pgo_form, (void *)&pgo_header);
    if (pgo_header.op_type == ek_op_query)
        sec_rgy_cursor_reset(&(pgo_header.marker));
    form_write_header(pgo_form, (void *)&pgo_header);
    form_clear(pgo_form);
}


/*
 * LOCAL FUNCTION ek_get_pgo_by_name
 *
 * Invokes ek_pgo_name_lookup using the caller's form and domain.  If the lookup
 * succeeds, calls pgo_query_unconvert to place the results of the query lookup
 * into the form fields appropriate for the specified domain.
 */
PRIVATE error_status_t ek_get_pgo_by_name
#ifndef __STDC__
    (pgo_form)
     form_t       *pgo_form;
#else
  (
     form_t       *pgo_form
  )
#endif
{
    error_status_t  st;
    pgo_header_t    header;
    unsigned_char_p_t   scope;
    sec_rgy_name_t      key;

    SET_STATUS(&st, error_status_ok);
    form_read_header (pgo_form, (void *)&header);
    scope = Current_scope[header.domain];

    if (strlen((char *)header.key) == 0) {  /* get the next person */
        sec_rgy_pgo_get_next(context, header.domain, scope, &(header.marker),
                             &(header.item), header.name, &st);
    }
    else {    /* look up the named person */
        if (strlen((char *) scope) > 0) {   /* Need to prepend scope ? */
            sprintf((char *)key, "%s/%s", (char *)scope, (char *)header.key);
            strcpy((char *)header.key, (char *)key);
        }
        sec_rgy_pgo_get_by_name (context, header.domain, header.key,
                              &(header.marker), &(header.item), &st);
        if (GOOD_STATUS(&st))
            strcpy((char *)header.name, (char *)header.key);
    }

    if (GOOD_STATUS(&st)) {
        pgo_query_unconvert(pgo_form, header.domain, &(header.item), header.name);
        form_write_header (pgo_form, ((void *)&header));
    } else
        eku_translate_status(&st);

    return st;
}


/*
 * LOCAL FUNCTION ek_get_pgo_by_unix_num
 *
 * Invokes ek_pgo_num_lookup using the caller's form and domain.  If the lookup
 * succeeds, calls pgo_query_unconvert to place the results of the query lookup
 * into the form fields appropriate for the specified domain.
 */
PRIVATE error_status_t ek_get_pgo_by_unix_num
#ifndef __STDC__
    (pgo_form)
     form_t  *pgo_form;
#else
  (
     form_t  *pgo_form
  )
#endif
{
    error_status_t  st;
    pgo_header_t    header;
    unsigned_char_p_t   scope;

    form_read_header (pgo_form, (void *)&header);

    SET_STATUS(&st, error_status_ok);

    scope = Current_scope[header.domain];
    sec_rgy_pgo_get_by_unix_num (context, header.domain, scope,
                                 header.unix_num_key, true,
                                 &(header.marker), &(header.item),
                                 header.name, &st);
    if (GOOD_STATUS(&st))
        pgo_query_unconvert(pgo_form, header.domain, &(header.item), header.name);
     else
        eku_translate_status(&st);

    form_write_header (pgo_form, ((void *)&header));

    return st;
}


/* FUNCTION common_init_pgo_query_form
 *
 * Allocates a pgo query form and initializes non-key fields.
 * Basically, all the initializations that are the same for both name and unix
 * num queries
 *
 * returns: ek_null_handle - if form allocation fails
 */
PRIVATE void common_init_pgo_query_form
#ifndef __STDC__
    (pgo_query_form, pgo_domain, stp)
    form_t              **pgo_query_form;
    sec_rgy_domain_t    pgo_domain;
    error_status_t      *stp;
#else
  (
    form_t              **pgo_query_form,
    sec_rgy_domain_t    pgo_domain,
    error_status_t      *stp
  )
#endif
{
    form_bitset_t       attributes;
    form_field_offset_t cur_field;
    pgo_header_t        header;
    form_t              *new_form;

    new_form = form_alloc(ekn_pgo_num_fields, sizeof(pgo_header_t));

    if (new_form == form_null_form) {
        SET_STATUS(stp, ek_null_handle);
        return;
    }
    sec_rgy_cursor_reset(&(header.marker));
    header.domain  = pgo_domain;
    header.op_type = ek_op_query;
    form_write_header(new_form,  (Pointer)&header);

        /* most of the fields are readonly and have no validation function */
    attributes = (0 | attribute_readonly_mask);
    for (cur_field = ekn_pgo_name; cur_field < ekn_pgo_key; cur_field++)
        form_init_field(new_form, cur_field, attributes,
                         (form_validate_t)form_null_function);

    switch (pgo_domain) {
    case sec_rgy_domain_person:
        form_init_field(new_form, ekn_pgo_projlist_ok, attributes,
                         (form_validate_t)form_null_function);
        break;
    case sec_rgy_domain_group:
        break;
    case sec_rgy_domain_org:
        form_init_field(new_form, ekn_pgo_projlist_ok, attributes,
                         (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_pgo_is_an_alias, attributes,
                         (form_validate_t)form_null_function);
        break;
    }
    *pgo_query_form = new_form;
}


/*
 * LOCAL FUNCTION get_pgo_name_query_form
 *
 * Calls common_init_pgo_query_form to allocate a query form and initialize
 * all the non_key fields of the query form.  Initializes the the key field
 *  with the vaildation routine appropriate for name-keyed lookups.
 */
PRIVATE void get_pgo_name_query_form
#ifndef __STDC__
    (pgo_form, pgo_domain, stp)
    form_t              **pgo_form;    /* IN - pointer to the operation form */
    sec_rgy_domain_t    pgo_domain;    /* IN - the domain the pgo name query is on */
    error_status_t      *stp;
#else
  (
    form_t              **pgo_form,    /* IN - pointer to the operation form */
    sec_rgy_domain_t    pgo_domain,    /* IN - the domain the pgo name query is on */
    error_status_t      *stp
  )
#endif
{
    form_bitset_t  attributes;

    SET_STATUS(stp, error_status_ok);
        /* do the common pgo query form initializations */
    common_init_pgo_query_form(pgo_form, pgo_domain, stp);

        /* the key field is writable */
    attributes = (0 | attribute_readwrite_mask);
   form_init_field(*pgo_form, ekn_pgo_key, attributes, pgo_key_validate);
}


/*
 * LOCAL FUNCTION get_pgo_num_query_form
 * allocates and initiializes a form for a pgo get_by_num operation.
 */
PRIVATE void get_pgo_num_query_form
#ifndef __STDC__
    (pgo_form, pgo_domain, stp)
    form_t             **pgo_form;  /* IN - ptr to the operation form */
    sec_rgy_domain_t   pgo_domain;  /* IN - the domain the unum query is on */
    error_status_t     *stp;
#else
  (
    form_t             **pgo_form,  /* IN - ptr to the operation form */
    sec_rgy_domain_t   pgo_domain,  /* IN - the domain the unum query is on */
    error_status_t     *stp
  )
#endif
{
    form_bitset_t  attributes;

    SET_STATUS(stp, error_status_ok);

        /* do the common pgo query form initializations */
    common_init_pgo_query_form(pgo_form, pgo_domain, stp);

    if (BAD_STATUS(stp))
        return;

        /* the key field is writable and required */
    attributes = (0 | attribute_readwrite_mask | attribute_required_mask);
    form_init_field(*pgo_form, ekn_pgo_key, attributes, pgo_unix_num_validate);
}


/* PGO UPDATE OPERATIONS */

/* FUNCTION get_pgo_update_form
 *
 * allocates and initializes a form for a specified update operation in a
 * specified pgo domain.  All (used) fields of an add form are required.
 * All non-key fields of a pgo change form are optional (any unwritten fields
 * default to existingvalues which get assigned when the key field is
 * (successfully) written).  The key field of a change form is required.
 * UUIDs are assigned by the kernel on successfull adds.  UUID fields are not
 * writeable.
 */
PRIVATE get_pgo_update_form
#ifndef __STDC__
    ( pgo_form, domain, op_type, stp )
    form_t              **pgo_form;
    sec_rgy_domain_t    domain;
    ek_op_type_t        op_type;
    error_status_t      *stp;
#else
  (
    form_t              **pgo_form,
    sec_rgy_domain_t    domain,
    ek_op_type_t        op_type,
    error_status_t      *stp
  )
#endif
{
    form_bitset_t       attributes;
    form_t              *new_form;
    pgo_header_t        header;

    new_form = form_alloc(ekn_pgo_num_fields, sizeof(pgo_header_t));

    if (new_form == form_null_form) {
        SET_STATUS(stp, ek_null_handle);
        return;
    }

    header.op_type = op_type;
    header.domain = domain;
    header.item.flags = sec_rgy_pgo_projlist_ok;
    header.flags = sec_rgy_pgo_projlist_ok;

    form_write_header(new_form,  (Pointer)&header);

    switch (op_type) {

    case ek_op_add:
            /* common add form initializations */

            /* required common add fields */
        attributes = (0 | attribute_readwrite_mask | attribute_required_mask);
        form_init_field(new_form, ekn_pgo_fullname, attributes,
                         (form_validate_t)pgo_fullname_validate);

            /* the uuid is assigned by the kernel if the add is successful */
        attributes = (0 | attribute_readonly_mask);
        form_init_field(new_form, ekn_pgo_uuid, attributes,
                             (form_validate_t)form_null_function);

            /* not used in any pgo add operations */
        attributes = (0 | attribute_unused_mask);
        form_init_field(new_form, ekn_pgo_is_reserved, attributes,
                         (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_pgo_name, attributes,
                         (form_validate_t)form_null_function);

            /* domain specific add initializations */
        switch (domain) {

        case sec_rgy_domain_person:

                /* required person add fields */
            attributes = (0 | attribute_readwrite_mask
                          | attribute_required_mask);
            form_init_field(new_form, ekn_pgo_key, attributes,
                             (form_validate_t)pgo_key_validate);
            form_init_field(new_form, ekn_pgo_unix_num, attributes,
                             (form_validate_t)pgo_unix_num_validate);
            form_init_field(new_form, ekn_pgo_quota, attributes,
                             (form_validate_t)quota_validate);
            form_init_field(new_form, ekn_pgo_is_an_alias, attributes,
                             (form_validate_t)is_an_alias_validate);

                /* not used in a person add operation */
            attributes = (0 | attribute_unused_mask);
            form_init_field(new_form, ekn_pgo_projlist_ok, attributes,
                             (form_validate_t)form_null_function);
            break;

        case sec_rgy_domain_group:

                /* required group add fields */
            attributes = (0 | attribute_readwrite_mask
                          | attribute_required_mask);
            form_init_field(new_form, ekn_pgo_key, attributes,
                             (form_validate_t)pgo_key_validate);
            form_init_field(new_form, ekn_pgo_unix_num, attributes,
                             (form_validate_t)pgo_unix_num_validate);
            form_init_field(new_form, ekn_pgo_projlist_ok, attributes,
                             (form_validate_t)g_projlist_flag_validate);
            form_init_field(new_form, ekn_pgo_is_an_alias, attributes,
                             (form_validate_t)is_an_alias_validate);
            break;

        case sec_rgy_domain_org:

                /* required org add fields */
            attributes = (0 | attribute_readwrite_mask
                          | attribute_required_mask);
            form_init_field(new_form, ekn_pgo_key, attributes,
                             (form_validate_t)pgo_key_validate);
            form_init_field(new_form, ekn_pgo_unix_num, attributes,
                             (form_validate_t)pgo_unix_num_validate);

                /* not used in an org add operation */
            attributes = (0 | attribute_unused_mask);
            form_init_field(new_form, ekn_pgo_is_an_alias, attributes,
                             (form_validate_t)is_an_alias_validate);
            form_init_field(new_form, ekn_pgo_projlist_ok, attributes,
                             (form_validate_t)form_null_function);

            break;

        }    /* end  [switch (op_type) case ek_op_add: { switch (domain) }] */

        break;    /* end [switch (op_type) case ek_op_add:] */

    case ek_op_change:
            /* common change form initializations */

           /* optional change fields */
        attributes = (0 | attribute_readwrite_mask);
        form_init_field(new_form, ekn_pgo_fullname, attributes,
                         (form_validate_t)pgo_fullname_validate);

        attributes = (0 | attribute_readonly_mask);
        form_init_field(new_form, ekn_pgo_uuid, attributes,
                             (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_pgo_is_reserved, attributes,
                         (form_validate_t)form_null_function);


            /* domain specific pgo change form initializations */
        switch (domain) {
        case sec_rgy_domain_person:

                /* required for person change  operations */
            attributes = (0 | attribute_readwrite_mask
                          | attribute_required_mask);
            form_init_field(new_form, ekn_pgo_key, attributes,
                             (form_validate_t)pgo_key_validate);

                /* optional for person change operations */
            attributes = (0 | attribute_readwrite_mask);
            form_init_field(new_form, ekn_pgo_name, attributes,
                             (form_validate_t)pgo_name_validate);
            form_init_field(new_form, ekn_pgo_unix_num, attributes,
                             (form_validate_t)pgo_unix_num_validate);
            form_init_field(new_form, ekn_pgo_quota, attributes,
                             (form_validate_t)quota_validate);
            form_init_field(new_form, ekn_pgo_is_an_alias, attributes,
                             (form_validate_t)is_an_alias_validate);

                /* not used at all in a person add operation */
            attributes = (0 | attribute_unused_mask);
            form_init_field(new_form, ekn_pgo_projlist_ok, attributes,
                             (form_validate_t)form_null_function);
            break;

        case sec_rgy_domain_group:

                /* required for group change operation */
            attributes = (0 | attribute_readwrite_mask
                          | attribute_required_mask);
            form_init_field(new_form, ekn_pgo_key, attributes,
                             (form_validate_t)pgo_key_validate);

                /* optional for group change operation */
            attributes = (0 | attribute_readwrite_mask);
            form_init_field(new_form, ekn_pgo_name, attributes,
                             (form_validate_t)pgo_name_validate);
            form_init_field(new_form, ekn_pgo_unix_num, attributes,
                             (form_validate_t)pgo_unix_num_validate);
            form_init_field(new_form, ekn_pgo_projlist_ok, attributes,
                             (form_validate_t)g_projlist_flag_validate);
            form_init_field(new_form, ekn_pgo_is_an_alias, attributes,
                             (form_validate_t)is_an_alias_validate);
            break;


        case sec_rgy_domain_org:

                /* required for org change operation */
            attributes = (0 | attribute_readwrite_mask
                          | attribute_required_mask);
            form_init_field(new_form, ekn_pgo_key, attributes,
                             (form_validate_t)pgo_key_validate);

                /* optional for org change operation */
            attributes = (0 | attribute_readwrite_mask);
            form_init_field(new_form, ekn_pgo_name, attributes,
                             (form_validate_t)pgo_name_validate);
            form_init_field(new_form, ekn_pgo_unix_num, attributes,
                             (form_validate_t)pgo_unix_num_validate);

            form_init_field(new_form, ekn_pgo_is_an_alias, attributes,
                             (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_pgo_projlist_ok, attributes,
                             (form_validate_t)form_null_function);

            break;

        }    /* end [switch (op_type) case ek_op_change: { switch (domain) }] */

        break;     /* end [switch(op_type) case ek_op_change:] */

    case ek_op_delete:
            /* common pgo delete form initializations */

            /* read only pgo delete fields */
        attributes = (0 | attribute_readonly_mask);
        form_init_field(new_form, ekn_pgo_uuid, attributes,
                             (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_pgo_is_reserved, attributes,
                         (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_pgo_name, attributes,
                         (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_pgo_unix_num, attributes,
                         (form_validate_t)form_null_function);

            /* domain specific pgo change form initializations */
        switch (domain) {

        case sec_rgy_domain_person:

                /* required for person delete  operation */
            attributes = (0 | attribute_readwrite_mask
                          | attribute_required_mask);
            form_init_field(new_form, ekn_pgo_key, attributes,
                             (form_validate_t)pgo_key_validate);

            attributes = (0 | attribute_readonly_mask);
            form_init_field(new_form, ekn_pgo_is_an_alias, attributes,
                             (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_pgo_projlist_ok, attributes,
                             (form_validate_t)form_null_function);

            break;

        case sec_rgy_domain_group:

                /* required for group delete  operation */
            attributes = (0 | attribute_readwrite_mask
                          | attribute_required_mask);
            form_init_field(new_form, ekn_pgo_key, attributes,
                             (form_validate_t)pgo_key_validate);

            attributes = (0 | attribute_readonly_mask);
            form_init_field(new_form, ekn_pgo_projlist_ok, attributes,
                             (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_pgo_is_an_alias, attributes,
                             (form_validate_t)form_null_function);
            break;
        case sec_rgy_domain_org:

                /* required for org delete operation */
            attributes = (0 | attribute_readwrite_mask
                          | attribute_required_mask);
            form_init_field(new_form, ekn_pgo_key, attributes,
                             (form_validate_t)pgo_key_validate);

            attributes = (0 | attribute_unused_mask);
            form_init_field(new_form, ekn_pgo_is_an_alias, attributes,
                             (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_pgo_projlist_ok, attributes,
                             (form_validate_t)form_null_function);
            break;

        } /* end [switch (op_type) case ek_op_delete: { switch (domain) }] */

        break;  /* end [switch(op_type) case ek_op_delete:] */

    } /* end [switch (op_type)] */

        /* finally we return the newly initialized pgo update form */
    *pgo_form = new_form;
}

/*
 * FUNCTION clear_pgo_update_form
 */
PRIVATE void clear_pgo_update_form
#ifndef __STDC__
    (pgo_update_form)
     form_t        *pgo_update_form;
#else
  (
     form_t        *pgo_update_form
  )
#endif
{
    form_clear(pgo_update_form);
}

PRIVATE get_pgo_adopt_form
#ifndef __STDC__
    ( pgo_form, domain, stp )
    form_t              **pgo_form;
    sec_rgy_domain_t    domain;
    error_status_t      *stp;
#else
  (
    form_t              **pgo_form,
    sec_rgy_domain_t    domain,
    error_status_t      *stp
  )
#endif
{
    form_bitset_t       attributes;
    form_t              *new_form;
    pgo_header_t        header;

    new_form = form_alloc(ekn_pgo_num_fields, sizeof(pgo_header_t));

    if (new_form == form_null_form) {
        SET_STATUS(stp, ek_null_handle);
        return;
    }

    header.domain   = domain;
    header.op_type = ek_op_add;
    form_write_header(new_form,  (Pointer)&header);

        /* required common adopt fields */
    attributes = (0 | attribute_readwrite_mask | attribute_required_mask);
    form_init_field(new_form, ekn_pgo_fullname, attributes,
                     (form_validate_t)pgo_fullname_validate);
    form_init_field(new_form, ekn_pgo_uuid, attributes,
                     (form_validate_t)pgo_uuid_validate);

        /* not used in adopt operation */
    attributes = (0 | attribute_unused_mask);
    form_init_field(new_form, ekn_pgo_is_reserved, attributes,
                     (form_validate_t)form_null_function);
    form_init_field(new_form, ekn_pgo_name, attributes,
                     (form_validate_t)form_null_function);

        /* domain specific add initializations */
    switch (domain) {

    case sec_rgy_domain_person:

            /* required person adopt fields */
        attributes = (0 | attribute_readwrite_mask | attribute_required_mask);
        form_init_field(new_form, ekn_pgo_key, attributes,
                         (form_validate_t)pgo_key_validate);
        form_init_field(new_form, ekn_pgo_unix_num, attributes,
                         (form_validate_t)adopt_unum_validate);
        form_init_field(new_form, ekn_pgo_quota, attributes,
                         (form_validate_t)quota_validate);
        form_init_field(new_form, ekn_pgo_is_an_alias, attributes,
                         (form_validate_t)is_an_alias_validate);

            /* not used in a person adopt operation */
        attributes = (0 | attribute_unused_mask);
            form_init_field(new_form, ekn_pgo_projlist_ok, attributes,
                             (form_validate_t)form_null_function);
        break;

    case sec_rgy_domain_group:

        /* required group adopt fields */
        attributes = (0 | attribute_readwrite_mask | attribute_required_mask);
        form_init_field(new_form, ekn_pgo_key, attributes,
                         (form_validate_t)pgo_key_validate);
        form_init_field(new_form, ekn_pgo_unix_num, attributes,
                         (form_validate_t)adopt_unum_validate);
        form_init_field(new_form, ekn_pgo_projlist_ok, attributes,
                         (form_validate_t)g_projlist_flag_validate);
        form_init_field(new_form, ekn_pgo_is_an_alias, attributes,
                         (form_validate_t)is_an_alias_validate);
        break;

    case sec_rgy_domain_org:

        /* required org adopt fields */
        attributes = (0 | attribute_readwrite_mask | attribute_required_mask);
        form_init_field(new_form, ekn_pgo_key, attributes,
                         (form_validate_t)pgo_key_validate);
        form_init_field(new_form, ekn_pgo_unix_num, attributes,
                         (form_validate_t)adopt_unum_validate);

        /* not used in an org add operation */
        attributes = (0 | attribute_unused_mask);
        form_init_field(new_form, ekn_pgo_is_an_alias, attributes,
                         (form_validate_t)is_an_alias_validate);
        form_init_field(new_form, ekn_pgo_projlist_ok, attributes,
                         (form_validate_t)form_null_function);

        break;
    }

    *pgo_form = new_form;
}


/* FUNCTION ek_pgo_add
 *
 * All fields except the uuid and the attribute add are required, so if we've 
 * gotten here then we know the interface has supplied all other fields.  The 
 * validation routines have already stored the converted values in form header 
 * so we don't need to go to the form's fields at all.
 */
PRIVATE error_status_t ek_pgo_add
#ifndef __STDC__
    ( pgo_add_form )
    form_t  *pgo_add_form;
#else
  (
    form_t  *pgo_add_form
  )
#endif
{
    pgo_header_t    header;
    error_status_t  st;
    int             i;
    uuid_t          uuid_nil;

    SET_STATUS(&st, error_status_ok);
     
    form_read_header(pgo_add_form,  (Pointer)&header);

    sec_rgy_pgo_unix_num_to_id(context, header.domain, header.item.unix_num,
                                &(header.item.id), &st);

        /* if we're adding a princ/group then it's okay if the unix_num to uuid
         * translation turned up a match, but it's an error in any other
         * circumstances
         */
    if (GOOD_STATUS(&st)) {
        if (!(header.domain == sec_rgy_domain_person) &&
            !(header.domain == sec_rgy_domain_group))
            SET_STATUS(&st, ek_err_unix_num_exists);
    } else if (STATUS_EQUAL(&st, sec_rgy_object_not_found)) {
        SET_STATUS(&st, error_status_ok);
        uuid_create_nil(&header.item.id, &st);
    }

    if (GOOD_STATUS(&st)) {
            /* only rgy_create can create reserved items so we turn the
             * flag off
             */
        UNSET(header.item.flags, sec_rgy_pgo_is_required);

        sec_rgy_pgo_add(context, header.domain, header.name, &(header.item), &st);
            /* write the uuid into the uuid field so it's available to the
             * interface
             */
        eku_dec_uuid(pgo_add_form, ekn_pgo_uuid, header.item.id);
    }

    eku_translate_status(&st);
    return st;
}

/* FUNCTION ek_pgo_adopt
 *
 * All fields including the uuid are required, so if we've gotten here then we
 * know the interface has supplied all fields.  The validation routines have
 * already stored the converted values in form header so we don't need to go
 * to the form's fields at all.
 */
PRIVATE error_status_t ek_pgo_adopt
#ifndef __STDC__
    (pgo_form)
    form_t  *pgo_form;
#else
  (
    form_t  *pgo_form
  )
#endif
{
    pgo_header_t    header;
    uuid_t          dummy_uuid;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    form_read_header(pgo_form,  (Pointer)&header);

        /* unix number can't already exist on adopt */
    sec_rgy_pgo_unix_num_to_id(context, header.domain, header.item.unix_num,
                                &dummy_uuid, &st);
    if (GOOD_STATUS(&st)) {
        SET_STATUS(&st, ek_err_unix_num_exists);
    } else if (STATUS_EQUAL(&st, sec_rgy_object_not_found)) {
        SET_STATUS(&st, error_status_ok);
    }

    if (GOOD_STATUS(&st)) {
        /* only rgy_create can create reserved items so we turn the flag off */
        UNSET(header.item.flags, sec_rgy_pgo_is_required);
        sec_rgy_pgo_add(context, header.domain, header.name, &(header.item), &st);
    }

    if (BAD_STATUS(&st))
        eku_translate_status(&st);

    return st;
}


PRIVATE error_status_t ek_pgo_change
#ifndef __STDC__
    ( form )
    form_t        *form;
#else
  (
    form_t        *form
  )
#endif
{
    pgo_header_t          header;
    ek_name_buf_t         pname;
    long int              pname_len;
    boolean32             name_change /* = false */;
    error_status_t        st;

    SET_STATUS(&st, error_status_ok);


    form_read_header(form,  (Pointer)&header);

    name_change = field_written(form, (form_field_offset_t) ekn_pgo_name);

         /* APPLY INTRA-RECORD CONSTRAINTS */
    if (header.item.flags & sec_rgy_pgo_is_required) {

            /* can't change name of a required entry */
        if (name_change) {
            SET_STATUS(&st, ek_err_reserved);
            return st;
        }
            /* can't change unix number of a required entry */
        if (field_written(form, (form_field_offset_t) ekn_pgo_unix_num)) {
            SET_STATUS(&st, ek_err_reserved);
            return st;
        }

        /* can't toggle the projlist flag of required entry back on - we cached
         * the pgo flags in the form header when the key field was written so
         * we can tell if the projlist flag has been toggled without an extra
         * query
         */
        if (field_written(form, (form_field_offset_t) ekn_pgo_projlist_ok)) {
            if ((header.item.flags & sec_rgy_pgo_projlist_ok) &&
                !(header.flags &sec_rgy_pgo_projlist_ok)) {
                SET_STATUS(&st, ek_err_reserved);
                return st;
            }
        }

    }

    /*???? what does the server do if the unix number changes along with the
           is_an_alias flag? */

    /* if we get here then the changes meet all the intra-record constraints */
    sec_rgy_pgo_replace(context, header.domain, header.key, &header.item, &st);
    if (GOOD_STATUS(&st) && name_change) {
        sec_rgy_pgo_rename(context, header.domain, header.key, header.name, &st);
    }

    if (BAD_STATUS(&st)) {
         eku_translate_status(&st);
     }

    return st;
}

PRIVATE error_status_t ek_pgo_delete
#ifndef __STDC__
    ( form )
    form_t  *form;
#else
  (
    form_t  *form
  )
#endif
{
    pgo_header_t    header;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    form_read_header(form,  (Pointer)&header);
    sec_rgy_pgo_delete(context, header.domain, header.key, &st);
    if (BAD_STATUS(&st)) {
        eku_translate_status(&st);
    }

    return st;
}


/*******************************************************************************
* MEMBERSHIP Operations
********************************************************************************/

/*
 * FUNCTION mem_key_validate
 *
 * Validation routine for the key field of all memberhip operations.
 * Checks for bad characters and names that are too long.  If the name is okay,
 * checks to see if it exists in the appropriate domain.
 *
 * returns:  ek_err_bad_name
 *           ek_err_name_chars
 *           ek_err_name_too_long
 *           any status returned by the sec_rgy_pgo_get_by_name call
 */
PRIVATE error_status_t mem_key_validate
#ifndef __STDC__
    (form, key, keylen, domain)
    form_t            *form;
    char              *key;
    long int          keylen;
    sec_rgy_domain_t  domain;
#else
  (
    form_t            *form,
    char              *key,
    long int          keylen,
    sec_rgy_domain_t  domain
  )
#endif
{
    error_status_t st;
        /* place holders for sec_rgy_pgo_get_by_name call */
    sec_rgy_cursor_t   marker;
    sec_rgy_pgo_item_t item;

    st = eku_chk_name (key, domain);

    if (GOOD_STATUS(&st)) {    /* see if key exists in the appropriate domain */
        sec_rgy_cursor_reset(&marker);
	/* 
	 * Check if this is a global principal name, if so, skip the
	 * checking that verifying the principal exists locally.
	 */
	if ( ! ( domain == sec_rgy_domain_person && 
		strncmp(key, GLOBAL_DIR_ROOT, GLOBAL_DIR_ROOT_LEN ) == 0 ) ) { 
            sec_rgy_pgo_get_by_name(context, domain, (unsigned char *)key, 
			&marker, &item, &st);
	}

        if (STATUS_EQUAL(&st, sec_rgy_object_not_found)) {
            switch (domain) {
            case sec_rgy_domain_person:
                SET_STATUS(&st, ek_err_person_does_not_exist);
                break;
            case sec_rgy_domain_group:
                SET_STATUS(&st, ek_err_group_does_not_exist);
                break;
            case sec_rgy_domain_org:
                SET_STATUS(&st, ek_err_org_does_not_exist);
                break;
            }
        } else
            eku_translate_status(&st);
    }

    return st;
}


/* The domain-specific membership key validation routine wrappers */

    /* for the get_person_groups key */
PRIVATE error_status_t p_mem_key_validate
#ifndef __STDC__
    ( form, key )
    form_t    *form;
    char      *key;
#else
  (
    form_t    *form,
    char      *key
  )
#endif
{
    error_status_t st;
    st = mem_key_validate(form, key, strlen(key), sec_rgy_domain_person);
    return st;
}


    /* for the get_group_members key */
PRIVATE error_status_t g_mem_key_validate
#ifndef __STDC__
    (form, key)
    form_t    *form;
    char      *key;
#else
  (
    form_t    *form,
    char      *key
  )
#endif
{
    error_status_t st;
    st = mem_key_validate(form, key, strlen(key), sec_rgy_domain_group);
    return st;
}


    /* for the get_org_members key */
PRIVATE error_status_t o_mem_key_validate
#ifndef __STDC__
    (form, key)
    form_t      *form;
    char        *key;
#else
  (
    form_t      *form,
    char        *key
  )
#endif
{
    error_status_t st;
    st = mem_key_validate(form, key, strlen(key), sec_rgy_domain_org);
    return st;
}


/* The constraints on the member name field of a membership update operation
 * are identical to the constraints on the key field in the person domain, so
 * we just call the mem_key_validate routine with the appropriate domain
 * parameter
 */
PRIVATE error_status_t update_mem_name_validate
#ifndef __STDC__
    ( form, name )
    form_t      *form;
    char        *name;
#else
  (
    form_t      *form,
    char        *name
  )
#endif
{
    error_status_t st;
    st = mem_key_validate(form, name, strlen(name), sec_rgy_domain_person);
    return st;
}


/*
 * LOCAL FUNCTION get_member_query_form
 *
 * allocates and initiializes a form for membership query operations.  Which
 * validation function is assigned depends on domain of the membership operation.
 *
 * returns:   ek_null_handle    -   in the stp pointer when a form can't be
 *                                   llocated
 */
PRIVATE void get_member_query_form
#ifndef __STDC__
    (member_form, domain, stp)
    form_t              **member_form;   /* OUT - pointer to the operation form */
    sec_rgy_domain_t    domain;          /* IN  - the domain this membership query
                                            form is valid on */
    error_status_t      *stp;            /* OUT */
#else
  (
    form_t              **member_form,   /* OUT - pointer to the operation form */
    sec_rgy_domain_t    domain,          /* IN  - the domain this membership query
                                            form is valid on */
    error_status_t      *stp
  )
#endif
{
    form_bitset_t          attributes;
    member_query_header_t  member_hdr;
    form_t                 *new_form;

    SET_STATUS(stp, error_status_ok);
    new_form = form_alloc(ekn_mem_num_fields, sizeof(member_query_header_t));
    if (new_form == form_null_form)
        SET_STATUS(stp, ek_null_handle);
    else {
        sec_rgy_cursor_reset(&(member_hdr.marker));
        member_hdr.first_call = true;
        member_hdr.cur_member = 0;
        member_hdr.num_read = 0;
        member_hdr.num_returned = 0;
        member_hdr.num_members = 0;
        form_write_header(new_form, (void *)&member_hdr);

            /* the ekn_mem_key field is writable and required */
        attributes = (0 | attribute_readwrite_mask | attribute_required_mask);

        switch (domain) {
        case sec_rgy_domain_person:
            form_init_field(new_form, ekn_mem_key, attributes,
                            p_mem_key_validate);
            break;
        case sec_rgy_domain_group:
            form_init_field(new_form,ekn_mem_key,attributes,g_mem_key_validate);
            break;
        case sec_rgy_domain_org:
            form_init_field(new_form,ekn_mem_key,attributes,o_mem_key_validate);
            break;
        }

        /* the ekn_mem_name and ekn_mem_num_members fields are readonly
         * and have no validation function
         */
        attributes = (0 | attribute_readonly_mask);
            form_init_field(new_form, ekn_mem_name, attributes,
                             (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_mem_num_members, attributes,
                             (form_validate_t)form_null_function);
        *member_form = new_form;
    }
}


/*
 * LOCAL FUNCTION clear_member_query_form
 * initilizes header data, and clears all the data fields in the form.
 */
PRIVATE void clear_member_query_form
#ifndef __STDC__
    (member_form)
     form_t *member_form;
#else
  (
     form_t *member_form
  )
#endif
{
    member_query_header_t member_hdr;

    form_read_header(member_form, (void *)&member_hdr);
    sec_rgy_cursor_reset(&(member_hdr.marker));
    member_hdr.first_call = true;
    member_hdr.cur_member = 0;
    member_hdr.num_read = 0;
    member_hdr.num_returned = 0;
    member_hdr.num_members = 0;
    form_write_header(member_form, (void *)&member_hdr);
    form_clear(member_form);
}


/*
 * FUNCTION do_get_members
 *
 * If this is a first call to do_get_members, or we've read all the members
 * from the members array in the form header and there's still more members
 * left to get from the database, do a sec_rgy_pgo_get_members.  Otherwise return
 * the next member from the members array.  If there aren't any more members
 * to be had, and this isn't the first call, return ek_no_more_entries.
 */
PRIVATE error_status_t do_get_members
#ifndef __STDC__
    ( member_form, domain )
     form_t            *member_form;
     sec_rgy_domain_t  domain;
#else
  (
     form_t            *member_form,
     sec_rgy_domain_t  domain
  )
#endif
{
    error_status_t         st;
    member_query_header_t  member_hdr;
    boolean32              do_rgy_lookup;
    char                   integer32_buff[33];   /* enough for a null-terminated string
                                                    representation of an integer32 */
    sec_rgy_name_t             pgo_name;
    long int               pgo_namelen;

    SET_STATUS(&st, error_status_ok);

    form_read_header(member_form, (void *)&member_hdr);

    /* get the person name out of the ekn_mem_key field - we assume it's valid
     * because it passed through the ekn_mem_key validation routine, and we know
     * it's there because the ekn_mem_key field is required
     */
    form_read (member_form, ekn_mem_key, pgo_name, sizeof(pgo_name),
                &pgo_namelen);

    /* if this is the first invocation of the ekn_mem_get_person_groups
     * operation or we've exhausted the entries in the members buffer and
     * there's still more in the registry database then we need to do a rgy
     * lookup
     */
    do_rgy_lookup = false;
    if (member_hdr.first_call == true) {
        do_rgy_lookup = true;
        member_hdr.first_call = false;
    }
    else if ((member_hdr.num_returned == member_hdr.cur_member) &&
             (member_hdr.num_read < member_hdr.num_members)) {
        do_rgy_lookup = true;
        member_hdr.cur_member = 0;
    }

    if (do_rgy_lookup) {
        sec_rgy_pgo_get_members(context, domain, pgo_name,
                            &member_hdr.marker, MAX_MEMBERS, member_hdr.members,
                            &member_hdr.num_returned, &member_hdr.num_members,
                            &st);
        if (BAD_STATUS(&st))
            eku_translate_status(&st);
        else {
            if (member_hdr.num_members == 0)
                SET_STATUS(&st, ek_misc_no_members);

            /* convert the num_members return value to a string and store it
             * in the ekn_mem_num_members field
             */
            sprintf(integer32_buff, "%d", member_hdr.num_members);
            form_write(member_form, ekn_mem_num_members, integer32_buff,
                        strlen(integer32_buff), false, &Dummy_st);
        }
    }
    else if (member_hdr.num_read == member_hdr.num_members)
        /* have we exhausted all the entries in the members buffer? */
        SET_STATUS(&st, ek_misc_no_more_entries);

    if (GOOD_STATUS(&st)) {
        /* write next entry from the members buffer into the
         * ekn_mem_name field
         */
        form_write(member_form, ekn_mem_name,
                   member_hdr.members[member_hdr.cur_member],
                    strlen((char *)member_hdr.members[member_hdr.cur_member]),
                   false, &Dummy_st);
        member_hdr.cur_member++;
        member_hdr.num_read++;
    }

        /* write the header_info back into the form */
    form_write_header (member_form, ((void *)&member_hdr));

    return st;
}


/* Membership Update operations
 *
 *  There are two membership update operations, add and delete.  These operations
 *  are valid in the group and organization domains.
 */

/*
 * LOCAL FUNCTION get_member_update_form
 *
 * allocates and initiializes a form for membership update operations.  Which
 * validation routine is assigned to the ekn_mem_key key field depends on domain
 * of the membership operation.  The validation routine for the ekn_mem_name
 * field is the same for both adds and deletes in both domains.  The
 * ekn_mem_num_members field is unused.
 *
 * returns:   ek_null_handle    -   in the stp pointer when a form can't be
 *                                  allocated
 */
PRIVATE void get_member_update_form
#ifndef __STDC__
    ( member_form, domain, stp )
    form_t             **member_form; /* OUT - pointer to the operation form */
    sec_rgy_domain_t   domain;        /* IN  - domain of this membership operation */
    error_status_t     *stp;          /* OUT */
#else
  (
    form_t             **member_form, /* OUT - pointer to the operation form */
    sec_rgy_domain_t   domain,        /* IN  - domain of this membership operation */
    error_status_t     *stp
  )
#endif
{
    form_bitset_t  attributes;
    form_t         *new_form;

    SET_STATUS(stp, error_status_ok);
    new_form = form_alloc(ekn_mem_num_fields, form_null_header);
    if (new_form == form_null_form)
        SET_STATUS(stp, ek_null_handle);
    else {

            /* the key and name fields are writable and required */
        attributes = (0 | attribute_readwrite_mask | attribute_required_mask);

        switch (domain) {
            /* group and org domains have distinct key validation */
        default:
            SET_STATUS(stp, ek_kernel_failure);  /* kernel should know better */
            break;
        case sec_rgy_domain_group:
            form_init_field(new_form, ekn_mem_key, attributes,
                            g_mem_key_validate);
            break;
        case sec_rgy_domain_org:
            form_init_field(new_form, ekn_mem_key, attributes,
                            o_mem_key_validate);
            break;
        }

        /* the member name validation is the same for all member updates in all
         * domains
         */
        form_init_field(new_form, ekn_mem_name, attributes,
                        update_mem_name_validate);

            /* the ekn_mem_num_members field is unused */
        if (GOOD_STATUS(stp)) {
            attributes = (0 | attribute_unused_mask);
            form_init_field(new_form, ekn_mem_num_members, attributes,
                             (form_validate_t)form_null_function);

            *member_form = new_form;
        }
    }
}


/*
 * FUNCTION do_update_member
 *
 */
PRIVATE error_status_t do_update_member
#ifndef __STDC__
    ( member_form, domain, op_type )
    form_t              *member_form;
    sec_rgy_domain_t    domain;
    ek_op_type_t        op_type;
#else
  (
    form_t              *member_form,
    sec_rgy_domain_t    domain,
    ek_op_type_t        op_type
  )
#endif
{
    error_status_t  st;
    sec_rgy_name_t      go_name,
                    mem_name;
    long int        go_namelen,
                    mem_namelen;

    SET_STATUS(&st, error_status_ok);

    /* get the group/org name out of the ekn_mem_key field - we assume it's
     * valid  because it passed through the ekn_mem_key validation routine,
     * and we know  it's there because the ekn_mem_key field is required
     */
    form_read (member_form, ekn_mem_key, go_name, sizeof(go_name),
                &go_namelen);
    /* ditto for the ekn_mem_name field */
    form_read (member_form, ekn_mem_name, mem_name, sizeof(mem_name),
                &mem_namelen);

    switch (op_type) {

    case ek_op_add:
        sec_rgy_pgo_add_member(context, domain, go_name, mem_name, &st);
        break;
    case ek_op_delete:
        sec_rgy_pgo_delete_member(context, domain, go_name, mem_name, &st);
        break;
    default:
        SET_STATUS(&st, ek_kernel_failure);
        break;
    }
    eku_translate_status(&st);
    return st;
}


/******************************************************************************
 *           POLICY operations
 ******************************************************************************/

/*  FUNCTION policy_unconvert
 *
 * Takes a policy form and a sec_rgy_plcy_t record, converts the data in the
 * policy record to strings and stores the results in the policy form fields.
 */
PRIVATE void policy_unconvert
#ifndef __STDC__
    ( policy_form, policy_rec )
    form_t          *policy_form;
    sec_rgy_plcy_t  *policy_rec;
#else
  (
    form_t          *policy_form,
    sec_rgy_plcy_t  *policy_rec
  )
#endif
{
    if (policy_rec->passwd_exp_date == ek_never_expire)
        form_write(policy_form, ekn_policy_passwd_exp, ek_null_date,
                    strlen(ek_null_date), false, &Dummy_st);
    else
        eku_dec_date_time(policy_form, ekn_policy_passwd_exp,
                            policy_rec->passwd_exp_date);

    if (policy_rec->passwd_lifetime == ek_live_forever)
        form_write(policy_form, ekn_policy_passwd_life, ek_lifespan_infinite,
                          strlen(ek_lifespan_infinite), false, &Dummy_st);
    else
        eku_dec_lifespan(policy_form, ekn_policy_passwd_life,
                          policy_rec->passwd_lifetime);

    if (policy_rec->acct_lifespan == ek_live_forever)
        form_write(policy_form, ekn_policy_acct_life, ek_lifespan_infinite,
                          strlen(ek_lifespan_infinite), false, &Dummy_st);
    else
        eku_dec_lifespan(policy_form, ekn_policy_acct_life,
                          (long int)policy_rec->acct_lifespan);

    eku_dec_int32(policy_form, ekn_policy_passwd_min_len,
                   policy_rec->passwd_min_len);
    eku_dec_policy_pwd_flag(policy_form, ekn_policy_passwd_no_spaces,
                             policy_rec->passwd_flags,
                             sec_rgy_plcy_pwd_no_spaces);
    eku_dec_policy_pwd_flag(policy_form, ekn_policy_passwd_non_alpha,
                             policy_rec->passwd_flags,
                            sec_rgy_plcy_pwd_non_alpha);
}


/*  FUNCTION auth_policy_unconvert
 *
 * Takes an auth policy form and a sec_rgy_plcy_t record, converts the data in
 * the auth policy record to strings and stores the results in the auth_policy
 * form fields.
 */
PRIVATE void auth_policy_unconvert
#ifndef __STDC__
    ( auth_policy_form, auth_policy_rec )
    form_t              *auth_policy_form;
    sec_rgy_plcy_auth_t *auth_policy_rec;
#else
  (
    form_t              *auth_policy_form,
    sec_rgy_plcy_auth_t *auth_policy_rec
  )
#endif
{
    if (auth_policy_rec->max_ticket_lifetime == ek_live_forever)
        form_write(auth_policy_form, ekn_auth_policy_max_ticket_lifetime,
                    ek_lifespan_infinite, strlen(ek_lifespan_infinite),
                    false, &Dummy_st);
    else
        eku_dec_lifespan(auth_policy_form, ekn_auth_policy_max_ticket_lifetime,
                            auth_policy_rec->max_ticket_lifetime);

    if (auth_policy_rec->max_renewable_lifetime == ek_live_forever)
        form_write(auth_policy_form, ekn_auth_policy_max_renewable_lifetime,
                    ek_lifespan_infinite, strlen(ek_lifespan_infinite),
                    false, &Dummy_st);
    else
        eku_dec_lifespan(auth_policy_form,
                            ekn_auth_policy_max_renewable_lifetime,
                            auth_policy_rec->max_renewable_lifetime);
}


PRIVATE copy_policy_change_defaults
#ifndef __STDC__
    (form, default_policy, dest_policy)
    form_t          *form;
    sec_rgy_plcy_t  *default_policy;
    sec_rgy_plcy_t  *dest_policy;
#else
  (
    form_t          *form,
    sec_rgy_plcy_t  *default_policy,
    sec_rgy_plcy_t  *dest_policy
  )
#endif
{
    sec_rgy_plcy_pwd_flags_t  write_set = 0;   /* assume no flags written */

    if (!field_written(form, (form_field_offset_t) ekn_policy_passwd_exp))
        dest_policy->passwd_exp_date = default_policy->passwd_exp_date;

    if (!field_written(form, (form_field_offset_t) ekn_policy_passwd_life))
        dest_policy->passwd_lifetime = default_policy->passwd_lifetime;

    if (!field_written(form, (form_field_offset_t) ekn_policy_acct_life))
        dest_policy->acct_lifespan = default_policy->acct_lifespan;

    if (!field_written(form, (form_field_offset_t) ekn_policy_passwd_min_len))
        dest_policy->passwd_min_len = default_policy->passwd_min_len;

       /* construct the writeset for the passwd flags */
    if (field_written(form, (form_field_offset_t) ekn_policy_passwd_no_spaces))
        SET(write_set, sec_rgy_plcy_pwd_no_spaces);

    if (field_written(form, (form_field_offset_t) ekn_policy_passwd_non_alpha))
        SET(write_set, sec_rgy_plcy_pwd_non_alpha);

    BITSET_COPY(default_policy->passwd_flags, dest_policy->passwd_flags,
                ~write_set);
}


/*
 * FUNCTION policy_key_validate
 *
 * Checks for bad characters and names that are too long.  If the name is okay,
 * checks to see if it exists in the group domain.  Applies to organization
 * names for query and change operations.  On a change, we perform a lookup
 * to supply default values for all the non-key fields of the policy operation.
 * If the key is non-null we first see if there's any policy assigned to the
 * organization.  If there is, it becomes the default.  If not, the kernel
 * supplies the default settings.
 *
 * returns:  ek_err_bad_name
 *           ek_err_name_chars
 *           ek_err_name_too_long
 *           ek_err_org_does_not_exist
 *           any status returned by rgy_ calls
 */
PRIVATE error_status_t policy_key_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    error_status_t  st;
    policy_header_t header;
    sec_rgy_plcy_t  default_policy;

    SET_STATUS(&st, error_status_ok);

    form_read_header(form,  (Pointer)&header);

    if (strlen(name) != 0) {
        st = eku_chk_name(name, sec_rgy_domain_org);
        if (GOOD_STATUS(&st))
            /* Make sure the org really exists */
            st = chk_pgo_exists(name, sec_rgy_domain_org);
    }

    if (BAD_STATUS(&st))  return st;

    if (header.op_type == ek_op_change) {
        sec_rgy_plcy_get_info(context, (unsigned_char_t *)name,
                            &default_policy, &st);
        if (GOOD_STATUS(&st))
                /* use existing policy as the default */
            copy_policy_change_defaults(form, &default_policy, &header.policy);
        else if ((STATUS_EQUAL(&st, sec_rgy_object_not_found)) && (strlen(name) != 0)) {
                /* organization has no policy so the kernel makes it up */
            copy_policy_change_defaults(form, &Def_policy, &header.policy);
            SET_STATUS(&st, error_status_ok);
        } else
            eku_translate_status(&st);

        if (GOOD_STATUS(&st))   /* make the defaults visible to the interface */
            policy_unconvert(form, &(header.policy));
    }

    if (GOOD_STATUS(&st)) {
        strcpy((char *)header.name, name);
        form_write_header(form,  (Pointer)&header);
    }

    return st;
}


PRIVATE error_status_t policy_pwd_life_validate
#ifndef __STDC__
    ( form, pwd_life )
    form_t  *form;
    char    *pwd_life;
#else
  (
    form_t  *form,
    char    *pwd_life
  )
#endif
{
    policy_header_t  header;
    error_status_t    st;

    form_read_header(form,  (Pointer)&header);

    st = eku_chk_lifespan_days(pwd_life, strlen(pwd_life), &
                                (header.policy.passwd_lifetime));
    if (GOOD_STATUS(&st))
        form_write_header(form,  (Pointer)&header);

    return st;
}


PRIVATE error_status_t policy_acct_life_validate
#ifndef __STDC__
    ( form, acct_life )
    form_t  *form;
    char    *acct_life;
#else
  (
    form_t  *form,
    char    *acct_life
  )
#endif
{
    policy_header_t  header;
    error_status_t   st;

    form_read_header(form,  (Pointer)&header);

    st = eku_chk_lifespan_days(acct_life, strlen(acct_life),
                                &(header.policy.acct_lifespan));
    if (GOOD_STATUS(&st))
        form_write_header(form,  (Pointer)&header);

    return st;
}


PRIVATE error_status_t policy_pwd_exp_date_validate
#ifndef __STDC__
    (form, date)
    form_t  *form;
    char    *date;
#else
  (
    form_t  *form,
    char    *date
  )
#endif
{
    policy_header_t  header;
    error_status_t        st;

    SET_STATUS(&st, error_status_ok);
    form_read_header(form,  (Pointer)&header);
    st = eku_chk_date(date, strlen(date), &(header.policy.passwd_exp_date));
    if (GOOD_STATUS(&st))
        form_write_header(form,  (Pointer)&header);

    return st;
}


PRIVATE error_status_t policy_pwd_no_spaces_validate
#ifndef __STDC__
    ( form, flag )
    form_t  *form;
    char    *flag;
#else
  (
    form_t  *form,
    char    *flag
  )
#endif
{
    policy_header_t  header;
    boolean32        flag_set;
    error_status_t   st;

    form_read_header(form,  (Pointer)&header);
    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (GOOD_STATUS(&st)) {
        if (flag_set)
            SET(header.policy.passwd_flags, sec_rgy_plcy_pwd_no_spaces);
        else
            UNSET(header.policy.passwd_flags, sec_rgy_plcy_pwd_no_spaces);

        form_write_header(form,  (Pointer)&header);
    }

    return st;
}


PRIVATE error_status_t policy_pwd_non_alpha_validate
#ifndef __STDC__
    ( form, flag )
    form_t  *form;
    char    *flag;
#else
  (
    form_t  *form,
    char    *flag
  )
#endif
{
    policy_header_t header;
    boolean32       flag_set;
    error_status_t  st;

    form_read_header(form,  (Pointer)&header);
    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (GOOD_STATUS(&st)) {
        if (flag_set)
            SET(header.policy.passwd_flags, sec_rgy_plcy_pwd_non_alpha);
        else
            UNSET(header.policy.passwd_flags, sec_rgy_plcy_pwd_non_alpha);
        form_write_header(form,  (Pointer)&header);
    }

    return st;
}


PRIVATE error_status_t policy_pwd_min_len_validate
#ifndef __STDC__
    ( form, int_str )
    form_t  *form;
    char    *int_str;
#else
  (
    form_t  *form,
    char    *int_str
  )
#endif
{
    policy_header_t  header;
    error_status_t   st;

    form_read_header(form,  (Pointer)&header);
    st = eku_chk_integer32(int_str, strlen(int_str),
                           &(header.policy.passwd_min_len));
    if (GOOD_STATUS(&st)) {
        if ( (header.policy.passwd_min_len < 0) ||
             (header.policy.passwd_min_len > ek_lim_max_plain_passwd_len) )
            SET_STATUS(&st, ek_err_bad_passwd_len);
        else
            form_write_header(form,  (Pointer)&header);
    }

    return st;
}


PRIVATE error_status_t auth_plcy_max_ticket_validate
#ifndef __STDC__
    ( form, max_ticket )
    form_t  *form;
    char    *max_ticket;
#else
  (
    form_t  *form,
    char    *max_ticket
  )
#endif
{
    auth_policy_header_t    header;
    error_status_t          st;

    form_read_header(form,  (Pointer)&header);
    st = eku_chk_lifespan_hours(max_ticket, strlen(max_ticket),
                                &(header.policy.max_ticket_lifetime));
    if (GOOD_STATUS(&st))
        form_write_header(form,  (Pointer)&header);

    return st;
}


PRIVATE error_status_t auth_plcy_max_renewable_validate
#ifndef __STDC__
    ( form, max_renewable )
    form_t  *form;
    char    *max_renewable;
#else
  (
    form_t  *form,
    char    *max_renewable
  )
#endif
{
    auth_policy_header_t    header;
    error_status_t          st;

    form_read_header(form,  (Pointer)&header);
    st = eku_chk_lifespan_hours(max_renewable, strlen(max_renewable),
                                &(header.policy.max_renewable_lifetime));
    if (GOOD_STATUS(&st))
        form_write_header(form,  (Pointer)&header);

    return st;
}


PRIVATE error_status_t acct_auth_plcy_max_ticket_validate
#ifndef __STDC__
    (form, max_ticket)
    form_t  *form;
    char    *max_ticket;
#else
  (
    form_t  *form,
    char    *max_ticket
  )
#endif
{
    acct_header_t    header;
    error_status_t          st;

    form_read_header(form,  (Pointer)&header);
    st = eku_chk_lifespan_hours(max_ticket, strlen(max_ticket),
                                &(header.policy.max_ticket_lifetime));
    if (GOOD_STATUS(&st)) {
        header.policy_written = true;
        form_write_header(form,  (Pointer)&header);
    }

    return st;
}


PRIVATE error_status_t acct_auth_plcy_max_renewable_validate
#ifndef __STDC__
    (form, max_renew)
    form_t  *form;
    char    *max_renew;
#else
  (
    form_t  *form,
    char    *max_renew
  )
#endif
{
    acct_header_t    header;
    error_status_t          st;

    form_read_header(form,  (Pointer)&header);
    st = eku_chk_lifespan_hours(max_renew, strlen(max_renew),
                                &(header.policy.max_renewable_lifetime));
    if (GOOD_STATUS(&st)) {
        header.policy_written = true;
        form_write_header(form,  (Pointer)&header);
    }

    return st;
}


/* auth_plcy_name_validate
 *
 * The name field is only writeable by the change operation.
 * changed
 */
PRIVATE error_status_t auth_plcy_name_validate
#ifndef __STDC__
    ( form, name, domain )
    form_t            *form;
    char              *name;
    sec_rgy_domain_t  domain;
#else
  (
    form_t            *form,
    char              *name,
    sec_rgy_domain_t  domain
  )
#endif
{
    auth_policy_header_t    header;
    error_status_t          st;

    CLEAR_STATUS(&st);
    if (strlen(name) != 0) {
        st =  eku_chk_name(name, domain);
        /* make sure the person, group and org components are real */
        if (GOOD_STATUS(&st))
            st = chk_pgo_exists(name, domain);
        if (BAD_STATUS(&st)) return st;
    }

    form_read_header(form,  (Pointer)&header);
    switch (domain) {
    case sec_rgy_domain_person:
        strcpy((char *)header.name.pname, name);
        break;
    case sec_rgy_domain_group:
        strcpy((char *)header.name.gname, name);
        break;
    case sec_rgy_domain_org:
        strcpy((char *)header.name.oname, name);
        break;
    }

    if (GOOD_STATUS(&st)) {
        form_write_header(form,  (Pointer)&header);
    }

    return st;
}


/*
 * auth_plcy_pname_validate
 */
PRIVATE error_status_t auth_plcy_pname_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    return(auth_plcy_name_validate(form, name, sec_rgy_domain_person));
}


/*
 * auth_plcy_gname_validate
 */
PRIVATE error_status_t auth_plcy_gname_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    return(auth_plcy_name_validate(form, name, sec_rgy_domain_group));
}


/*
 * auth_plcy_pname_validate
 */
PRIVATE error_status_t auth_plcy_oname_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    return(auth_plcy_name_validate(form, name, sec_rgy_domain_org));
}


/*
 * LOCAL FUNCTION get_policy_form
 * Allocates and initializes a form for policy queries.
 *
 * returns:   ek_null_handle    -   in the stp pointer when a form can't be
 *                                  allocated
 */
PRIVATE void get_policy_form
#ifndef __STDC__
    ( policy_form, op_type, stp )
    form_t          **policy_form;  /* IN - pointer to the operation form */
    ek_op_type_t    op_type;        /* IN - query or change */
    error_status_t  *stp;           /* OUT - status */
#else
  (
    form_t          **policy_form,  /* IN - pointer to the operation form */
    ek_op_type_t    op_type,        /* IN - query or change */
    error_status_t  *stp
  )
#endif
{
    form_field_offset_t  cur_field;
    form_bitset_t        attributes;
    form_t               *new_form;
    policy_header_t      header;

    if (op_type == ek_op_add || op_type == ek_op_delete) {
        SET_STATUS(stp, ek_kernel_failure);
        return;
    }
    new_form = form_alloc(ekn_policy_num_fields, sizeof(policy_header_t));

    if (new_form == form_null_form) {
        SET_STATUS(stp, ek_null_handle);
        return;
    }

    SET_STATUS(stp, error_status_ok);

         /* the ekn_policy_key field is required */
    attributes = (0 | attribute_readwrite_mask | attribute_required_mask);
    form_init_field(new_form, ekn_policy_key, attributes, policy_key_validate);

    switch (op_type) {
    case ek_op_query:
        header.op_type = ek_op_query;

        /* the rest of the fields are readonly and have no validation function*/
        attributes = (0 | attribute_readonly_mask);
        for (cur_field = 0; cur_field < ekn_policy_key; cur_field++)
            form_init_field(new_form, cur_field, attributes,
                             (form_validate_t)form_null_function);
        break;
    case ek_op_change:
        header.op_type = ek_op_change;

        attributes = (0 | attribute_readwrite_mask);
        form_init_field(new_form, ekn_policy_passwd_exp, attributes,
                         (form_validate_t)policy_pwd_exp_date_validate);
        form_init_field(new_form, ekn_policy_passwd_life, attributes,
                         (form_validate_t)policy_pwd_life_validate);
        form_init_field(new_form, ekn_policy_acct_life, attributes,
                         (form_validate_t)policy_acct_life_validate);
        form_init_field(new_form, ekn_policy_passwd_min_len, attributes,
                         (form_validate_t)policy_pwd_min_len_validate);
        form_init_field(new_form, ekn_policy_passwd_no_spaces, attributes,
                         (form_validate_t)policy_pwd_no_spaces_validate);
        form_init_field(new_form, ekn_policy_passwd_non_alpha, attributes,
                         (form_validate_t)policy_pwd_non_alpha_validate);
        break;
    }

    form_write_header(new_form,  (Pointer)&header);
    *policy_form = new_form;
}


/*
 * LOCAL FUNCTION get_auth_policy_form
 * Allocates and initializes a form for auth policy queries.
 *
 * returns:   ek_null_handle    -   in the stp pointer when a form can't be
 *                                  allocated
 */
PRIVATE void get_auth_policy_form
#ifndef __STDC__
    ( auth_policy_form, op_type, stp )
    form_t          **auth_policy_form; /* IN pointer to the operation form */
    ek_op_type_t    op_type;            /* IN - query or change */
    error_status_t  *stp;               /* OUT - status */
#else
  (
    form_t          **auth_policy_form, /* IN pointer to the operation form */
    ek_op_type_t    op_type,            /* IN - query or change */
    error_status_t  *stp
  )
#endif
{
    form_field_offset_t  cur_field;
    form_bitset_t        attributes;
    form_t               *new_form;
    auth_policy_header_t header;

    if (op_type == ek_op_add || op_type == ek_op_delete) {
        SET_STATUS(stp, ek_kernel_failure);
        return;
    }
    new_form =
        form_alloc(ekn_auth_policy_num_fields, sizeof(auth_policy_header_t));

    if (new_form == form_null_form) {
        SET_STATUS(stp, ek_null_handle);
        return;
    }

    SET_STATUS(stp, error_status_ok);

         /* the ekn_auth_policy key fields are required */
    attributes = (0 | attribute_readwrite_mask | attribute_required_mask);
    form_init_field(new_form, ekn_auth_policy_pname, attributes, auth_plcy_pname_validate);
    form_init_field(new_form, ekn_auth_policy_gname, attributes, auth_plcy_gname_validate);
    form_init_field(new_form, ekn_auth_policy_oname, attributes, auth_plcy_oname_validate);

    switch (op_type) {
    case ek_op_query:
        header.op_type = ek_op_query;

        /* the rest of the fields are readonly and have no validation function*/
        attributes = (0 | attribute_readonly_mask);
        for (cur_field = 0; cur_field < ekn_auth_policy_pname; cur_field++)
            form_init_field(new_form, cur_field, attributes,
                             (form_validate_t)form_null_function);
        break;
    case ek_op_change:
        header.op_type = ek_op_change;

        attributes = (0 | attribute_readwrite_mask);
        form_init_field(new_form, ekn_auth_policy_max_ticket_lifetime, attributes,
                         (form_validate_t)auth_plcy_max_ticket_validate);
        form_init_field(new_form, ekn_auth_policy_max_renewable_lifetime, attributes,
                         (form_validate_t)auth_plcy_max_renewable_validate);
        break;
    }

    form_write_header(new_form,  (Pointer)&header);
    *auth_policy_form = new_form;
}


/*
 * LOCAL ROUTINE clear_auth_policy_form
 * Destroys the data in all non-key fields of an auth policy form.
 */
PRIVATE void clear_auth_policy_form
#ifndef __STDC__
    ( auth_plcy_form )
    form_t *auth_plcy_form;
#else
  (
    form_t *auth_plcy_form
  )
#endif
{
    form_field_offset_t cur_field;

    for (cur_field = ekn_auth_policy_max_ticket_lifetime;
            cur_field < auth_plcy_form->num_fields; cur_field++) {
        auth_plcy_form->fields[cur_field].data_len = 0;
        auth_plcy_form->fields[cur_field].field_status = form_field_no_data;
    }
}


/* FUNCTION ek_policy_get
 *
 * do a policy lookup using the name in the form header.  If we're after
 * organization policy (the name is non-zero length) the lookup may return
 * "object not found".  There are two possible reasons for this: 1) the
 * organization has no associated policy object in the database, or 2)
 * the organization doesn't exist at all.   Case 2 is highly unlikely because
 * the key validation routine has already checked for this case but as we
 * have no transaction capability it's remotely possible that the org got
 * deleted by a competing update process between the invocation of the
 * key validation routine and this get routine.  We double check when we
 * see an object not found and convert it to ek_err_org_does_not_exist
 * if case 2 is indeed the case.  If its case 1, we supply the default
 * organization policy.
 */
error_status_t ek_policy_get
#ifndef __STDC__
    ( policy_form )
    form_t  *policy_form;
#else
  (
    form_t  *policy_form
  )
#endif
{
    policy_header_t  header;
    error_status_t   st;

    SET_STATUS(&st, error_status_ok);

    form_read_header(policy_form,  (Pointer)&header);

    sec_rgy_plcy_get_info(context, header.name, &(header.policy), &st);
    if (GOOD_STATUS(&st)) {
        policy_unconvert(policy_form, &(header.policy));
    } else if (STATUS_EQUAL(&st, sec_rgy_object_not_found)) {
        st = chk_pgo_exists( (char *) header.name, sec_rgy_domain_org);
        if (GOOD_STATUS(&st)) {
            /* we've got an organization that has no associated policy object so
             * we give back the default policy
             */
            policy_unconvert(policy_form, &Def_policy);
            SET_STATUS(&st, error_status_ok);
        }
    } else
        eku_translate_status(&st);

    return st;
}


/*
 * FUNCTION ek_auth_policy_get
 * Do an auth policy lookup using the name in the form header.
 */
error_status_t ek_auth_policy_get
#ifndef __STDC__
    ( auth_policy_form )
    form_t  *auth_policy_form;
#else
  (
    form_t  *auth_policy_form
  )
#endif
{
    auth_policy_header_t    header;
    error_status_t          st;
    sec_rgy_cursor_t        marker;
    sec_rgy_login_name_t    name_result;
    sec_rgy_acct_key_t      key_parts;
    sec_rgy_sid_t           sid;
    sec_rgy_unix_sid_t      unix_sid;
    sec_rgy_acct_user_t     user_part;
    sec_rgy_acct_admin_t    admin_part;

    SET_STATUS(&st, error_status_ok);

    form_read_header(auth_policy_form,  (Pointer)&header);

    sec_rgy_auth_plcy_get_info(context, &header.name, &(header.policy), &st);
    if (GOOD_STATUS(&st)) {
        auth_policy_unconvert(auth_policy_form, &(header.policy));
        form_write_header(auth_policy_form, (Pointer)&header);
    } else if (STATUS_EQUAL(&st, sec_rgy_object_not_found)) {

        st =  eku_chk_name((char *)header.name.pname, sec_rgy_domain_person);
        if (GOOD_STATUS(&st)) {
            st =  eku_chk_name((char *)header.name.gname,sec_rgy_domain_group);
        }
        if (GOOD_STATUS(&st)) {
            st =  eku_chk_name((char *)header.name.oname, sec_rgy_domain_org);
        }
        if (BAD_STATUS(&st)) {
            return st;
        }

        sec_rgy_cursor_reset(&marker);
        sec_rgy_acct_lookup(context, &header.name, &marker, &name_result, &sid,
                        &unix_sid, &key_parts, &user_part, &admin_part, &st);

        if (GOOD_STATUS(&st)) {
            /* we've got an account that has no associated policy object so
             * we give back the default policy
             */
            auth_policy_unconvert(auth_policy_form, &Def_auth_policy);
            SET_STATUS(&st, error_status_ok);
        }
    } else
        eku_translate_status(&st);

    return st;
}


/*
 * FUNCTION ek_policy_get_effective
 * Gets the effective policy for an organization.
 */
error_status_t ek_policy_get_effective
#ifndef __STDC__
    ( policy_form )
    form_t   *policy_form;
#else
  (
    form_t   *policy_form
  )
#endif
{
    policy_header_t  header;
    error_status_t   st;

    form_read_header(policy_form,  (Pointer)&header);

    sec_rgy_plcy_get_effective(context, header.name,
                                  &(header.policy), &st);
    if (GOOD_STATUS(&st)) {
        policy_unconvert(policy_form, &(header.policy));
    } else if ( STATUS_EQUAL(&st, sec_rgy_object_not_found)) {
        SET_STATUS(&st, ek_err_org_does_not_exist);
    } else
        eku_translate_status(&st);

    return st;
}


/*
 * FUNCTION ek_auth_policy_get_effective
 * Gets the effective auth policy for an account.
 */
PRIVATE error_status_t ek_auth_policy_get_effective
#ifndef __STDC__
    ( auth_policy_form )
    form_t   *auth_policy_form;
#else
  (
    form_t   *auth_policy_form
  )
#endif
{
    auth_policy_header_t    header;
    error_status_t          st;

    form_read_header(auth_policy_form,  (Pointer)&header);

    sec_rgy_auth_plcy_get_effective(context, &header.name,
                                  &(header.policy), &st);
    if (GOOD_STATUS(&st)) {
        auth_policy_unconvert(auth_policy_form, &(header.policy));
    } else if ( STATUS_EQUAL(&st, sec_rgy_object_not_found)) {
        SET_STATUS(&st, ek_err_acct_does_not_exist);
    } else
        eku_translate_status(&st);

    return st;
}


/* FUNCTION ek_policy_set
 *
 * Sets policy for a given organization (or the registry, if the key is zero
 * length).  The registry server does not store a policy record for
 * organizations that have not explicitly had policy assigned to them. The
 * set_policy routine checks to see if the org in question has an associated
 * policy object.  If not, it checks the incoming policy against the default
 * policy, and only performs the policy update if there's a difference.
 */
error_status_t ek_policy_set
#ifndef __STDC__
    ( form )
    form_t  *form;
#else
  (
    form_t  *form
  )
#endif
{
    policy_header_t  header;
    error_status_t   st;
    sec_rgy_plcy_t     temp_pol;

    SET_STATUS(&st, error_status_ok);

    form_read_header(form,  (Pointer)&header);

    if (strlen((char *)header.name) != 0) {
        sec_rgy_plcy_get_info(context, header.name, &(temp_pol), &st);
        if (STATUS_EQUAL(&st, sec_rgy_object_not_found)) {
            /* make sure no one has deleted the org out from under us */
            st = chk_pgo_exists( (char *) header.name, sec_rgy_domain_org);
            if (STATUS_EQUAL(&st, error_status_ok))
                if ((header.policy.passwd_min_len  ==
                     Def_policy.passwd_min_len) &&
                    (header.policy.passwd_lifetime ==
                     Def_policy.passwd_lifetime) &&
                    (header.policy.passwd_exp_date ==
                     Def_policy.passwd_exp_date) &&
                    (header.policy.passwd_flags    ==
                     Def_policy.passwd_flags) &&
                    (header.policy.acct_lifespan   ==
                     Def_policy.acct_lifespan))

                    return st;   /* don't create new policy records for default
                                    policy */
        }
    }

    if (GOOD_STATUS(&st))
        sec_rgy_plcy_set_info(context, header.name, &(header.policy), &st);

    if (BAD_STATUS(&st))
        eku_translate_status(&st);

    return st;
}


/* FUNCTION ek_auth_policy_set
 *
 * Sets policy for a given account (or the registry, if the key is zero
 * length).  The registry server does not store a policy record for
 * accounts that have not explicitly had policy assigned to them. The
 * set_policy routine checks to see if the acct in question has an associated
 * policy object.  If not, it checks the incoming policy against the default
 * policy, and only performs the policy update if there's a difference.
 */
error_status_t ek_auth_policy_set
#ifndef __STDC__
    ( form )
    form_t  *form;
#else
  (
    form_t  *form
  )
#endif
{
    auth_policy_header_t    header;
    error_status_t          st;
    sec_rgy_plcy_auth_t     temp_pol;
    sec_rgy_cursor_t        marker;
    sec_rgy_login_name_t    name_result;
    sec_rgy_acct_key_t      key_parts;
    sec_rgy_sid_t           sid;
    sec_rgy_unix_sid_t      unix_sid;
    sec_rgy_acct_user_t     user_part;
    sec_rgy_acct_admin_t    admin_part;

    SET_STATUS(&st, error_status_ok);

    form_read_header(form,  (Pointer)&header);

    if (strlen((char *)header.name.pname) != 0) {
        sec_rgy_auth_plcy_get_info(context, &header.name, &(temp_pol), &st);
        if (STATUS_EQUAL(&st, sec_rgy_object_not_found)) {
            /* make sure no one has deleted the acct out from under us */
            sec_rgy_cursor_reset(&marker);
            sec_rgy_acct_lookup(context, &header.name, &marker, &name_result,
                    &sid, &unix_sid, &key_parts, &user_part, &admin_part, &st);

            if (STATUS_EQUAL(&st, error_status_ok))
                if ((header.policy.max_ticket_lifetime  ==
                     Def_auth_policy.max_ticket_lifetime) &&
                    (header.policy.max_renewable_lifetime ==
                     Def_auth_policy.max_renewable_lifetime))

                    return st;   /* don't create new policy records for default
                                    policy */
        }
    }

    if (GOOD_STATUS(&st))
        sec_rgy_auth_plcy_set_info(context, &header.name, &(header.policy), &st);

    if (BAD_STATUS(&st))
        eku_translate_status(&st);

    return st;
}


/*****************************************************************************
 * Local registry properties operations
 *****************************************************************************/

/* FUNCTION lrgy_prop_unconvert
 *
 * Converts internal local registry property types to strings and stores them
 * in the appropriate field of the lrgy properties form.
 */
PRIVATE void lrgy_prop_unconvert
#ifndef __STDC__
    ( form, prop, num_tgt_entries )
    form_t                  *form;
    sec_lrgy_hdr_t          *prop;
    unsigned32              *num_tgt_entries;
#else
  (
    form_t                  *form,
    sec_lrgy_hdr_t          *prop,
    unsigned32              *num_tgt_entries
  )
#endif
{
    eku_dec_int32(form, ekn_prop_lrgy_max_entries,
                        prop->max_entries);
    eku_dec_int32(form, ekn_prop_lrgy_num_entries,
                        prop->num_entries);
    eku_dec_int32(form, ekn_prop_lrgy_num_tgt_entries,
                        prop->num_entries);

    if (prop->lifespan == ek_live_forever) {
        form_write(form, ekn_prop_lrgy_lifespan, ek_lifespan_infinite,
                          strlen(ek_lifespan_infinite), false, &Dummy_st);
    } else {
        eku_dec_lifespan(form, ekn_prop_lrgy_lifespan, prop->lifespan);
    }
}


/*
 * FUNCTION lrgy_prop_max_entries_validate
 */
PRIVATE error_status_t lrgy_prop_max_entries_validate
#ifndef __STDC__
    (form, num_str, field)
    form_t              *form;
    char                *num_str;
    form_field_offset_t field;
#else
  (
    form_t              *form,
    char                *num_str,
    form_field_offset_t field
  )
#endif
{
    lrgy_prop_header_t  header;
    signed32            max_entries;
    error_status_t      st;

    CLEAR_STATUS(&st);

    st = eku_chk_integer32(num_str, strlen(num_str), &max_entries);

    if (GOOD_STATUS(&st)) {
        form_read_header(form,  (Pointer)&header);
        header.properties.max_entries = max_entries;
        form_write_header(form,  (Pointer)&header);
    }
    return st;
}


/*
 * FUNCTION lrgy_prop_lifespan_validate
 */
PRIVATE error_status_t lrgy_prop_lifespan_validate
#ifndef __STDC__
    ( form, lifespan_str )
    form_t  *form;
    char    *lifespan_str;
#else
  (
    form_t  *form,
    char    *lifespan_str
  )
#endif
{
    lrgy_prop_header_t      header;
    error_status_t          st;
    sec_timeval_period_t    lifespan;

    CLEAR_STATUS(&st);

    st = eku_chk_lifespan_days(lifespan_str, strlen(lifespan_str), &lifespan);

    if (GOOD_STATUS(&st)) {
        form_read_header(form,  (Pointer)&header);
        header.properties.lifespan = lifespan;
        form_write_header(form,  (Pointer)&header);
    }

    return st;
}


/* FUNCTION ek_get_lrgy_prop_form
 * returns:   ek_null_handle    -   in the stp pointer when a form can't be
 *                                    allocated
 */
PRIVATE void ek_get_lrgy_prop_form
#ifndef __STDC__
    ( prop_form, op_type, stp )
    form_t          **prop_form;
    ek_op_type_t    op_type;
    error_status_t  *stp;
#else
  (
    form_t          **prop_form,
    ek_op_type_t    op_type,
    error_status_t  *stp
  )
#endif
{
    form_field_offset_t    cur_field;
    form_bitset_t          attributes;
    form_t                 *new_form;
    lrgy_prop_header_t     header;

    SET_STATUS(stp, error_status_ok);

    new_form = form_alloc(ekn_prop_lrgy_num_fields, sizeof(header));

    if (new_form == form_null_form)
        SET_STATUS(stp, ek_null_handle);
    else {
        header.op_type = op_type;
        switch (op_type) {

        case ek_op_query:
                /* all fields are readonly */
            attributes = (0 | attribute_readonly_mask);
            for (cur_field = 0; cur_field < ekn_prop_lrgy_num_fields; cur_field++)
                form_init_field(new_form, cur_field, attributes,
                                 (form_validate_t)form_null_function);
            break;
        case ek_op_change:
            attributes = (0 | attribute_readwrite_mask);
            form_init_field(new_form, ekn_prop_lrgy_max_entries, attributes,
                             (form_validate_t)lrgy_prop_max_entries_validate);
            form_init_field(new_form, ekn_prop_lrgy_lifespan, attributes,
                             (form_validate_t)lrgy_prop_lifespan_validate);
            attributes = (0 | attribute_readonly_mask);
            form_init_field(new_form, ekn_prop_lrgy_num_entries, attributes,
                                (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_prop_lrgy_num_tgt_entries,
                                attributes,
                                (form_validate_t)form_null_function);
            break;
        }
    }
    form_write_header(new_form,  (Pointer)&header);

    *prop_form = new_form;
}


/* FUNCTION ek_init_lrgy_prop_form
 *
 * Clears data fields.  If this is a change operation, does a lookup and
 * loads the form with defaults from the registry database.
 */
PRIVATE error_status_t ek_init_lrgy_prop_form
#ifndef __STDC__
    ( prop_form )
    form_t  *prop_form;
#else
  (
    form_t  *prop_form
  )
#endif
{
    lrgy_prop_header_t header;
    error_status_t     st;

    CLEAR_STATUS(&st);
    form_clear(prop_form);

    form_read_header(prop_form,  (Pointer)&header);
    if (header.op_type == ek_op_change) {
        sec_lrgy_properties_get_info(&header.properties, &st);
        if (GOOD_STATUS(&st)) {
            sec_lrgy_properties_tgt_get_info(&header.num_tgt_entries, &st);
        }
        if (GOOD_STATUS(&st)) {
            lrgy_prop_unconvert(prop_form, &header.properties,
                                &header.num_tgt_entries);
            form_write_header(prop_form,  (Pointer)&header);
        } else {
            eku_translate_status(&st);
        }
    }

    return st;
}


/*
 * FUNCTION ek_lrgy_prop_get
 */
PRIVATE error_status_t ek_lrgy_prop_get
#ifndef __STDC__
    ( prop_form )
    form_t  *prop_form;
#else
  (
    form_t  *prop_form
  )
#endif
{
    lrgy_prop_header_t header;
    error_status_t     st;

    form_read_header(prop_form,  (Pointer)&header);

    sec_lrgy_properties_get_info(&header.properties, &st);
    sec_lrgy_properties_tgt_get_info(&header.num_tgt_entries, &st);

    if (GOOD_STATUS(&st)) {
        lrgy_prop_unconvert(prop_form, &header.properties,
                            &header.num_tgt_entries);
        form_write_header(prop_form,  (Pointer)&header);
    } else {
        eku_translate_status(&st);
    }

    return st;
}


/*
 * FUNCTION ek_lrgy_prop_set
 */
PRIVATE error_status_t ek_lrgy_prop_set
#ifndef __STDC__
    (prop_form)
    form_t *prop_form;
#else
  (
    form_t *prop_form
  )
#endif
{
    lrgy_prop_header_t  header;
    error_status_t      st;

    form_read_header(prop_form, (Pointer)&header);

    sec_lrgy_properties_set_info(&header.properties, &st);

    if (BAD_STATUS(&st)) {
        eku_translate_status(&st);
    }

    return st;
}


PUBLIC void ek_do_lrgy_purge
#ifndef __STDC__
    (num_purged, num_tgts_purged, status)
    unsigned32      *num_purged;
    unsigned32      *num_tgts_purged;
    error_status_t  *status;
#else
  (
    unsigned32      *num_purged,
    unsigned32      *num_tgts_purged,
    error_status_t  *status
  )
#endif
{
        /* Just do the purge; no pre-op validation yet */
    sec_lrgy_purge_expired(num_purged, num_tgts_purged, status);
}


PUBLIC boolean32 ek_do_lrgy_delete
#ifndef __STDC__
    (name, entry_status, tgt_status)
    unsigned_char_p_t   name;
    error_status_t      *entry_status;
    error_status_t      *tgt_status;
#else
  (
    unsigned_char_p_t   name,
    error_status_t      *entry_status,
    error_status_t      *tgt_status
  )
#endif
{
    sec_lrgy_entry_delete(name, entry_status);
    sec_lrgy_tgt_delete(name, tgt_status);
    if (GOOD_STATUS(entry_status) && GOOD_STATUS(tgt_status))
        return true;
    else
        return false;
}


/*****************************************************************************
 * PROPERTIES Operations
 *****************************************************************************/

/* FUNCTION rgy_prop_unconvert
 *
 * Converts internal registry property types to strings and stores them in the
 * appropriate field of the properties form
 */
PRIVATE void rgy_prop_unconvert
#ifndef __STDC__
    ( form, rgy_prop )
    form_t                  *form;
    sec_rgy_properties_t    rgy_prop;
#else
  (
    form_t                  *form,
    sec_rgy_properties_t    rgy_prop
  )
#endif
{
    eku_dec_property_flag(form, ekn_prop_is_readonly, rgy_prop.flags,
                            sec_rgy_prop_readonly);
    eku_dec_property_flag(form, ekn_prop_auth_certificates_unbounded,
                            rgy_prop.flags, sec_rgy_prop_auth_cert_unbound);
    eku_dec_property_flag(form, ekn_prop_shadow_passwd, rgy_prop.flags,
                           sec_rgy_prop_shadow_passwd);
    eku_dec_property_flag(form, ekn_prop_embedded_unix_id, rgy_prop.flags,
                           sec_rgy_prop_embedded_unix_id);

    eku_dec_unix_num(form, ekn_prop_low_unix_id_person,
                        rgy_prop.low_unix_id_person);
    eku_dec_unix_num(form, ekn_prop_low_unix_id_group,
                        rgy_prop.low_unix_id_group);
    eku_dec_unix_num(form, ekn_prop_low_unix_id_org,
                        rgy_prop.low_unix_id_org);
    eku_dec_unix_num(form, ekn_prop_max_unix_id,
                        rgy_prop.max_unix_id);
    eku_dec_lifespan(form, ekn_prop_minimum_certificate_lifetime,
                        rgy_prop.minimum_ticket_lifetime);
    eku_dec_lifespan(form, ekn_prop_default_certificate_lifetime,
                        rgy_prop.default_certificate_lifetime);
    form_write(form, ekn_prop_realm, rgy_prop.realm,
                strlen((char *) rgy_prop.realm), false, &Dummy_st);
}


/*
 * FUNCTION prop_is_readonly_validate
 */
PRIVATE error_status_t prop_is_readonly_validate
#ifndef __STDC__
    ( form, flag )
    form_t  *form;
    char    *flag;
#else
  (
    form_t  *form,
    char    *flag
  )
#endif
{
    rgy_prop_header_t  header;
    boolean32          flag_set;
    error_status_t     st;

    SET_STATUS(&st, error_status_ok);

    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);

    if (flag_set)
        SET(header.properties.flags, sec_rgy_prop_readonly);
    else
        UNSET(header.properties.flags, sec_rgy_prop_readonly);

    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION prop_shadow_passwd_validate
 */
PRIVATE error_status_t prop_shadow_passwd_validate
#ifndef __STDC__
    ( form, flag )
    form_t  *form;
    char    *flag;
#else
  (
    form_t  *form,
    char    *flag
  )
#endif
{
    rgy_prop_header_t  header;
    boolean32          flag_set;
    error_status_t     st;

    SET_STATUS(&st, error_status_ok);

    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);

    if (flag_set)
        SET(header.properties.flags, sec_rgy_prop_shadow_passwd);
    else
        UNSET(header.properties.flags, sec_rgy_prop_shadow_passwd);

    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION prop_unix_num_validate
 */
PRIVATE error_status_t prop_unix_num_validate
#ifndef __STDC__
    (form, unum_str, field)
    form_t              *form;
    char                *unum_str;
    form_field_offset_t field;
#else
  (
    form_t              *form,
    char                *unum_str,
    form_field_offset_t field
  )
#endif
{
    rgy_prop_header_t   header;
    signed32            unum;
    error_status_t      st;

    SET_STATUS(&st, error_status_ok);

    if (eku_chk_unix_num (unum_str, strlen(unum_str), &unum)) {

        form_read_header(form,  (Pointer)&header);
        switch (field) {
            case ekn_prop_low_unix_id_person:
                header.properties.low_unix_id_person = unum;
                break;
            case ekn_prop_low_unix_id_group:
                header.properties.low_unix_id_group = unum;
                break;
            case ekn_prop_low_unix_id_org:
                header.properties.low_unix_id_org = unum;
                break;
            case ekn_prop_max_unix_id:
                header.properties.max_unix_id = unum;
                break;
        }
        form_write_header(form,  (Pointer)&header);

    } else {
        SET_STATUS(&st, ek_err_bad_unix_num);
        return st;
    }

    return st;
}


PRIVATE error_status_t prop_luid_person_validate
#ifndef __STDC__
    (form, unum_str)
    form_t  *form;
    char    *unum_str;
#else
  (
    form_t  *form,
    char    *unum_str
  )
#endif
{
    return(prop_unix_num_validate(form, unum_str, ekn_prop_low_unix_id_person));
}


PRIVATE error_status_t prop_luid_group_validate
#ifndef __STDC__
    (form, unum_str)
    form_t  *form;
    char    *unum_str;
#else
  (
    form_t  *form,
    char    *unum_str
  )
#endif
{
    return(prop_unix_num_validate(form, unum_str, ekn_prop_low_unix_id_group));
}


PRIVATE error_status_t prop_luid_org_validate
#ifndef __STDC__
    (form, unum_str)
    form_t  *form;
    char    *unum_str;
#else
  (
    form_t  *form,
    char    *unum_str
  )
#endif
{
    return(prop_unix_num_validate(form, unum_str, ekn_prop_low_unix_id_org));
}


PRIVATE error_status_t prop_max_uid_validate
#ifndef __STDC__
    (form, unum_str)
    form_t  *form;
    char    *unum_str;
#else
  (
    form_t  *form,
    char    *unum_str
  )
#endif
{
    return(prop_unix_num_validate(form, unum_str, ekn_prop_max_unix_id));
}


/*
 * FUNCTION prop_min_tkt_validate
 */
PRIVATE error_status_t prop_min_tkt_validate
#ifndef __STDC__
    ( form, lifetime )
    form_t  *form;
    char    *lifetime;
#else
  (
    form_t  *form,
    char    *lifetime
  )
#endif
{
    rgy_prop_header_t  header;
    error_status_t     st;

    SET_STATUS(&st, error_status_ok);
    form_read_header(form,  (Pointer)&header);

    st = eku_chk_lifespan_minutes(lifetime, strlen(lifetime),
                                &(header.properties.minimum_ticket_lifetime));
    if (GOOD_STATUS(&st))
        form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION prop_def_tkt_validate
 */
PRIVATE error_status_t prop_def_tkt_validate
#ifndef __STDC__
    ( form, lifetime )
    form_t  *form;
    char    *lifetime;
#else
  (
    form_t  *form,
    char    *lifetime
  )
#endif
{
    rgy_prop_header_t  header;
    error_status_t     st;

    SET_STATUS(&st, error_status_ok);
    form_read_header(form,  (Pointer)&header);

    st = eku_chk_lifespan_hours(lifetime, strlen(lifetime),
                            &(header.properties.default_certificate_lifetime));
    if (GOOD_STATUS(&st))
        form_write_header(form,  (Pointer)&header);

    return st;
}


/* FUNCTION ek_get_rgy_prop_form
 * returns:   ek_null_handle    -   in the stp pointer when a form can't be
 *                                    allocated
 */
PRIVATE void ek_get_rgy_prop_form
#ifndef __STDC__
    ( prop_form, op_type, stp )
    form_t          **prop_form;
    ek_op_type_t    op_type;
    error_status_t  *stp;
#else
  (
    form_t          **prop_form,
    ek_op_type_t    op_type,
    error_status_t  *stp
  )
#endif
{
    form_field_offset_t    cur_field;
    form_bitset_t          attributes;
    form_t                 *new_form;
    rgy_prop_header_t      header;

    SET_STATUS(stp, error_status_ok);

    new_form = form_alloc(ekn_prop_num_fields, sizeof(header));

    if (new_form == form_null_form)
        SET_STATUS(stp, ek_null_handle);
    else {
        header.op_type = op_type;
        switch (op_type) {

        case ek_op_query:
                /* all fields are readonly */
            attributes = (0 | attribute_readonly_mask);
            for (cur_field = 0; cur_field < ekn_prop_num_fields; cur_field++)
                form_init_field(new_form, cur_field, attributes,
                                 (form_validate_t)form_null_function);
            break;
        case ek_op_change:
            /* all fields except realm are writeable and have a validation function */
            attributes = (0 | attribute_readwrite_mask);
            form_init_field(new_form, ekn_prop_is_readonly, attributes,
                             (form_validate_t)prop_is_readonly_validate);
            form_init_field(new_form, ekn_prop_shadow_passwd, attributes,
                             (form_validate_t)prop_shadow_passwd_validate);
            form_init_field(new_form, ekn_prop_low_unix_id_person, attributes,
                             (form_validate_t)prop_luid_person_validate);
            form_init_field(new_form, ekn_prop_low_unix_id_group, attributes,
                             (form_validate_t)prop_luid_group_validate);
            form_init_field(new_form, ekn_prop_low_unix_id_org, attributes,
                             (form_validate_t)prop_luid_org_validate);
            form_init_field(new_form, ekn_prop_max_unix_id, attributes,
                             (form_validate_t)prop_max_uid_validate);
            form_init_field(new_form, ekn_prop_minimum_certificate_lifetime,
                            attributes,(form_validate_t)prop_min_tkt_validate);
            form_init_field(new_form, ekn_prop_default_certificate_lifetime,
                            attributes,(form_validate_t)prop_def_tkt_validate);
            attributes = (0 | attribute_readonly_mask);
            form_init_field(new_form, ekn_prop_realm, attributes,
                                (form_validate_t)form_null_function);
            break;
        }
    }
    form_write_header(new_form,  (Pointer)&header);

    *prop_form = new_form;
}


/* FUNCTION ek_init_rgy_prop_form
 *
 * Clears data fields.  If this is a change operation, does a lookup and
 * loads the form with defaults from the registry database.
 */
PRIVATE error_status_t ek_init_rgy_prop_form
#ifndef __STDC__
    ( prop_form )
    form_t  *prop_form;
#else
  (
    form_t  *prop_form
  )
#endif
{
    rgy_prop_header_t  header;
    error_status_t     st;

    SET_STATUS(&st, error_status_ok);
    form_clear(prop_form);

    form_read_header(prop_form,  (Pointer)&header);
    if (header.op_type == ek_op_change) {
        sec_rgy_properties_get_info (context, &(header.properties), &st);

        if (GOOD_STATUS(&st)) {
            rgy_prop_unconvert(prop_form, header.properties);
            form_write_header(prop_form,  (Pointer)&header);
        } else {
            eku_translate_status(&st);
        }
    }

    return st;
}


/* FUNCTION ek_rgy_prop_get
 *
 * Calls  sec_rgy_properties_get_info. if the call is successful, stores
 * the results in prop_form.
 */
PRIVATE error_status_t ek_rgy_prop_get
#ifndef __STDC__
    ( prop_form )
    form_t  *prop_form;
#else
  (
    form_t  *prop_form
  )
#endif
{
    rgy_prop_header_t  header;
    error_status_t     st;

    form_read_header(prop_form,  (Pointer)&header);

    sec_rgy_properties_get_info (context, &(header.properties), &st);

    if (GOOD_STATUS(&st)) {
        rgy_prop_unconvert(prop_form, header.properties);
        form_write_header(prop_form,  (Pointer)&header);
    } else {
        eku_translate_status(&st);
    }

    return st;
}


/* FUNCTION ek_rgy_prop_set
 * Calls sec_rgy_properties_set_info. Report the status.
 */
PRIVATE error_status_t ek_rgy_prop_set
#ifndef __STDC__
    (prop_form)
    form_t *prop_form;
#else
  (
    form_t *prop_form
  )
#endif
{
    rgy_prop_header_t   header;
    error_status_t      st;

    form_read_header(prop_form,  (Pointer)&header);
    sec_rgy_properties_set_info(context, &(header.properties), &st);

    if (BAD_STATUS(&st)) {
        eku_translate_status(&st);
    }

    return st;
}


/********************************************************************************
 * ACCOUNT Operations
 ********************************************************************************/

/* the first lookup of an account will sometimes return "no more entries" even
 * if there are no matching entries at all.  This macro hides that fact
 */
#define ACCT_NOT_FOUND(st) ((STATUS_EQUAL(&st, sec_rgy_object_not_found)) || \
                            (STATUS_EQUAL(&st, sec_rgy_no_more_entries)))

PRIVATE void init_acct_header
#ifndef __STDC__
    ( header, op_type )
    acct_header_t  *header;
    ek_op_type_t   op_type;
#else
  (
    acct_header_t  *header,
    ek_op_type_t   op_type
  )
#endif
{
    header->op_type = op_type;

    if (op_type == ek_op_query) {
        sec_rgy_cursor_reset(&(header->marker));
    } else {
        header->pname_key_written = false;
        header->gname_key_written = false;
        header->oname_key_written = false;
        header->new_pname_key_written = false;
        header->new_gname_key_written = false;
        header->new_oname_key_written = false;
        header->admin_part_written = false;
        header->user_part_written = false;
        header->policy_written = false;
        header->plain_passwd_written = false;
        header->passwd_written = false;
        header->passwd.pepper = NULL;
        header->passwd.version_number = sec_passwd_c_version_none;
        PASSWD_TYPE(&header->passwd) = sec_passwd_plain;
        *(header->plain_passwd) = '\0';
        header->my_passwd.pepper = NULL;
        header->my_passwd.version_number = sec_passwd_c_version_none;
        PASSWD_TYPE(&header->my_passwd) = sec_passwd_plain;
        *(header->my_plain_passwd) = '\0';
        header->new_key_type = sec_passwd_none;
        header->last_status = error_status_ok;
        header->pwd_mgmt_h = NULL;
        header->pwd_val_type = no_policy;
    }
}


/* LOCAL FUNCTION acct_query_unconvert
 *
 * Takes an account user part,  and an account admin part containing the results
 * of an account query, and a form containing the fields for a account query.
 * Converts the data in the user and admin parts into strings and copies those
 * strings to the  appropriate fields in the form.
 */
PRIVATE void acct_query_unconvert
#ifndef __STDC__
    ( acct_form, acct_name, key_parts, sid,
        unix_sid, user_part, admin_part, policy)
    form_t                *acct_form;
    sec_rgy_login_name_t  *acct_name;
    sec_rgy_acct_key_t    key_parts;
    sec_rgy_sid_t         *sid;
    sec_rgy_unix_sid_t    *unix_sid;
    sec_rgy_acct_user_t   *user_part;
    sec_rgy_acct_admin_t  *admin_part;
    sec_rgy_plcy_auth_t   *policy;
#else
  (
    form_t                *acct_form,
    sec_rgy_login_name_t  *acct_name,
    sec_rgy_acct_key_t    key_parts,
    sec_rgy_sid_t         *sid,
    sec_rgy_unix_sid_t    *unix_sid,
    sec_rgy_acct_user_t   *user_part,
    sec_rgy_acct_admin_t  *admin_part,
    sec_rgy_plcy_auth_t   *policy
  )
#endif
{
    acct_header_t         header;
    form_read_header(acct_form,  (Pointer)&header);

    if (!field_written(acct_form, (form_field_offset_t) ekn_acct_pname))
        form_write(acct_form, ekn_acct_pname, acct_name->pname,
                   strlen((char *)acct_name->pname),
                   false, &Dummy_st);
    if (!field_written(acct_form, (form_field_offset_t) ekn_acct_gname))
        form_write(acct_form, ekn_acct_gname, acct_name->gname,
                   strlen((char *)acct_name->gname),
                   false, &Dummy_st);
    if (!field_written(acct_form, (form_field_offset_t) ekn_acct_oname))
        form_write(acct_form, ekn_acct_oname, acct_name->oname,
                   strlen((char *)acct_name->oname),
                   false, &Dummy_st);

        /* virtual data */
    if (!field_written(acct_form, (form_field_offset_t) ekn_acct_key_parts))
        eku_dec_acct_key_parts(acct_form, ekn_acct_key_parts, key_parts);
    if (!field_written(acct_form, (form_field_offset_t) ekn_acct_sid_person))
        eku_dec_uuid(acct_form, ekn_acct_sid_person, sid->person);
    if (!field_written(acct_form, (form_field_offset_t) ekn_acct_sid_group))
        eku_dec_uuid(acct_form, ekn_acct_sid_group, sid->group);
    if (!field_written(acct_form, (form_field_offset_t) ekn_acct_sid_org))
        eku_dec_uuid(acct_form, (form_field_offset_t) ekn_acct_sid_org,
                     sid->org);
    if (!field_written(acct_form,
                       (form_field_offset_t) ekn_acct_unix_sid_person))
        eku_dec_unix_num(acct_form, ekn_acct_unix_sid_person, unix_sid->person);
    if (!field_written(acct_form, (form_field_offset_t) ekn_acct_unix_sid_group))
        eku_dec_unix_num(acct_form, ekn_acct_unix_sid_group, unix_sid->group);
    if (!field_written(acct_form, (form_field_offset_t) ekn_acct_unix_sid_org))
        eku_dec_unix_num(acct_form, ekn_acct_unix_sid_org, unix_sid->org);

       /* the user part data */
    if (!field_written(acct_form, (form_field_offset_t) ekn_acct_user_misc))
        form_write(acct_form, ekn_acct_user_misc, user_part->gecos,
                    strlen((char *)user_part->gecos), false, &Dummy_st);
    if (!field_written(acct_form, (form_field_offset_t) ekn_acct_user_homedir))
        form_write(acct_form, ekn_acct_user_homedir, user_part->homedir,
                    strlen((char *)user_part->homedir), false, &Dummy_st);
    if (!field_written(acct_form, (form_field_offset_t) ekn_acct_user_shell))
        form_write(acct_form, ekn_acct_user_shell, user_part->shell,
                    strlen((char *)user_part->shell), false, &Dummy_st);

    form_write(acct_form, ekn_acct_user_enc_passwd, user_part->passwd,
               strlen((char *)user_part->passwd), false, &Dummy_st);

    eku_dec_int32(acct_form, ekn_acct_user_passwd_version,
                             (signed32) user_part->passwd_version_number);

    if (!field_written(acct_form,
                       (form_field_offset_t) ekn_acct_user_passwd_dtm))
        eku_dec_date_time(acct_form, ekn_acct_user_passwd_dtm,
                           user_part->passwd_dtm);
    if (!field_written(acct_form,
                       (form_field_offset_t) ekn_acct_user_passwd_valid))
        eku_dec_acct_user_flag(acct_form, ekn_acct_user_passwd_valid,
                                user_part->flags,
                               sec_rgy_acct_user_passwd_valid);

        /* the admin part data */
    if (!field_written(acct_form, (form_field_offset_t) ekn_acct_admin_creator))
        eku_dec_foreign_id(acct_form, ekn_acct_admin_creator,
                            admin_part->creator);
    if (!field_written(acct_form,
                       (form_field_offset_t)ekn_acct_admin_when_created))
        eku_dec_date_time(acct_form, ekn_acct_admin_when_created,
                           admin_part->creation_date);
    if (!field_written(acct_form, (form_field_offset_t) ekn_acct_admin_changer))
        eku_dec_foreign_id(acct_form, ekn_acct_admin_changer,
                            admin_part->last_changer);
    if (!field_written(acct_form,
                       (form_field_offset_t) ekn_acct_admin_when_changed))
        eku_dec_date_time(acct_form, ekn_acct_admin_when_changed,
                           admin_part->change_date);
    if (!field_written(acct_form, (form_field_offset_t) ekn_acct_admin_exp_date))
        if (admin_part->expiration_date == ek_never_expire)
            form_write(acct_form, ekn_acct_admin_exp_date, ek_null_date,
                        strlen(ek_null_date), false, &Dummy_st);
        else
            eku_dec_date_time(acct_form, ekn_acct_admin_exp_date,
                               admin_part->expiration_date);
    if (!field_written(acct_form,
                       (form_field_offset_t) ekn_acct_admin_acct_server))
        eku_dec_acct_admin_flag(acct_form, ekn_acct_admin_acct_server,
                                 admin_part->flags, sec_rgy_acct_admin_server);
    if (!field_written(acct_form,
                       (form_field_offset_t) ekn_acct_admin_acct_client))
        eku_dec_acct_admin_flag(acct_form, ekn_acct_admin_acct_client,
                                 admin_part->flags, sec_rgy_acct_admin_client);
    if (!field_written(acct_form,
                       (form_field_offset_t) ekn_acct_admin_acct_valid))
        eku_dec_acct_admin_flag(acct_form, ekn_acct_admin_acct_valid,
                                 admin_part->flags, sec_rgy_acct_admin_valid);
    if (!field_written(acct_form,
                       (form_field_offset_t)ekn_acct_admin_good_since_date))
        eku_dec_date_time(acct_form, ekn_acct_admin_good_since_date,
                           admin_part->good_since_date);

    /* Acct admin auth flags */
    if (!field_written(acct_form,
                        (form_field_offset_t) ekn_acct_admin_auth_post_dated))
        eku_dec_acct_admin_flag(acct_form, ekn_acct_admin_auth_post_dated,
                                admin_part->authentication_flags,
                                sec_rgy_acct_auth_post_dated);
    if (!field_written(acct_form,
                        (form_field_offset_t) ekn_acct_admin_auth_forwardable))
        eku_dec_acct_admin_flag(acct_form, ekn_acct_admin_auth_forwardable,
                                admin_part->authentication_flags,
                                sec_rgy_acct_auth_forwardable);
    if (!field_written(acct_form,
                        (form_field_offset_t) ekn_acct_admin_auth_tgt))
        eku_dec_acct_admin_flag(acct_form, ekn_acct_admin_auth_tgt,
                                admin_part->authentication_flags,
                                sec_rgy_acct_auth_tgt);
    if (!field_written(acct_form,
                        (form_field_offset_t) ekn_acct_admin_auth_renewable))
        eku_dec_acct_admin_flag(acct_form, ekn_acct_admin_auth_renewable,
                                admin_part->authentication_flags,
                                sec_rgy_acct_auth_renewable);
    if (!field_written(acct_form,
                        (form_field_offset_t) ekn_acct_admin_auth_proxiable))
        eku_dec_acct_admin_flag(acct_form, ekn_acct_admin_auth_proxiable,
                                admin_part->authentication_flags,
                                sec_rgy_acct_auth_proxiable);
    if (!field_written(acct_form,
                        (form_field_offset_t) ekn_acct_admin_auth_dup_session_key))
        eku_dec_acct_admin_flag(acct_form, ekn_acct_admin_auth_dup_session_key,
                                admin_part->authentication_flags,
                                sec_rgy_acct_auth_dup_skey);

        /* Auth policy data.
         * If operation is change and authpol doesn't exist, leave fields blank
         * else if null then defaulting to rgy auth plcy.
         */

    if (!field_written(acct_form,
                       (form_field_offset_t)ekn_acct_max_ticket_lifetime))
        if (policy)
            eku_dec_lifespan(acct_form, ekn_acct_max_ticket_lifetime,
                                    policy->max_ticket_lifetime);
        else if (header.op_type != ek_op_change)
            form_write(acct_form, ekn_acct_max_ticket_lifetime, "default-policy",
                        14, false, &Dummy_st);
    if (!field_written(acct_form,
                       (form_field_offset_t)ekn_acct_max_renewable_lifetime))
        if (policy)
            eku_dec_lifespan(acct_form, ekn_acct_max_renewable_lifetime,
                                    policy->max_renewable_lifetime);
        else if (header.op_type != ek_op_change)
            form_write(acct_form,ekn_acct_max_renewable_lifetime,"default-policy",
                        14, false, &Dummy_st);
}


/* copy_acct_change_defaults
 *
 *  For each field in the acct change form that hasn't been written via
 *  ek_write_field, apply the default values from the source parameters to
 *  the corresponding data fields in the form header.  We always copy admin
 *  part data that is supposedly maintained by the server, because it isn't
 *  really.
 */
PRIVATE void copy_acct_change_defaults
#ifndef __STDC__
    ( form, header, src_user_part, src_admin_part, src_key_parts)
    form_t                *form;
    acct_header_t         *header;
    sec_rgy_acct_user_t   *src_user_part;
    sec_rgy_acct_admin_t  *src_admin_part;
    sec_rgy_acct_key_t    *src_key_parts;
#else
  (
    form_t                *form,
    acct_header_t         *header,
    sec_rgy_acct_user_t   *src_user_part,
    sec_rgy_acct_admin_t  *src_admin_part,
    sec_rgy_acct_key_t    *src_key_parts
  )
#endif
{
    sec_rgy_acct_admin_flags_t  admin_writeset;
    sec_rgy_acct_user_flags_t   user_writeset;
    sec_rgy_acct_auth_flags_t   auth_writeset;

    if (!field_written(form, (form_field_offset_t) ekn_acct_key_parts))
        header->key_parts = *src_key_parts;

    if (!field_written(form, (form_field_offset_t) ekn_acct_user_plain_passwd)) {
        /* don't copy plain password - whether or not it gets set is determined
         * by the set_passwd flag in the client agent replace call, not by any
         * any value in the password field
         */
        header->user_part.passwd_dtm = src_user_part->passwd_dtm;
    }


    if (!field_written(form, (form_field_offset_t) ekn_acct_user_misc)) {
        strcpy((char *)(header->user_part.gecos), (char *)src_user_part->gecos);
    }

    if (!field_written(form, (form_field_offset_t) ekn_acct_user_homedir)) {
        strcpy((char *)(header->user_part.homedir),
                (char *)src_user_part->homedir);
    }

    if (!field_written(form, (form_field_offset_t) ekn_acct_user_shell)) {
        strcpy((char *)(header->user_part.shell),
                (char *)(src_user_part->shell));
    }

    if (!field_written(form, (form_field_offset_t) ekn_acct_admin_exp_date)) {
        header->admin_part.expiration_date = src_admin_part->expiration_date;
    }

    if (!field_written(form, (form_field_offset_t)
                        ekn_acct_admin_good_since_date)) {
        header->admin_part.good_since_date = src_admin_part->good_since_date;
    }

    /* copy old values to server maintained admin fields */
    header->admin_part.creator = src_admin_part->creator;
    header->admin_part.creation_date = src_admin_part->creation_date;
    header->admin_part.last_changer = src_admin_part->last_changer;
    header->admin_part.change_date = src_admin_part->change_date;


    CLEAR_BITSET(user_writeset);
    if (field_written(form, (form_field_offset_t) ekn_acct_user_passwd_valid))
        SET(user_writeset, sec_rgy_acct_user_passwd_valid);
    BITSET_COPY(src_user_part->flags, header->user_part.flags, ~user_writeset);

    CLEAR_BITSET(admin_writeset);
    if (field_written(form, (form_field_offset_t) ekn_acct_admin_acct_server))
        SET(admin_writeset, sec_rgy_acct_admin_server);
    if (field_written(form, (form_field_offset_t) ekn_acct_admin_acct_client))
        SET(admin_writeset, sec_rgy_acct_admin_client);
    if (field_written(form, (form_field_offset_t) ekn_acct_admin_acct_valid))
        SET(admin_writeset, sec_rgy_acct_admin_valid);
    BITSET_COPY(src_admin_part->flags, header->admin_part.flags,
                ~admin_writeset);

    CLEAR_BITSET(auth_writeset);
    if (field_written(form, (form_field_offset_t) ekn_acct_admin_auth_post_dated))
        SET(auth_writeset, sec_rgy_acct_auth_post_dated);
    if (field_written(form, (form_field_offset_t) ekn_acct_admin_auth_forwardable))
        SET(auth_writeset, sec_rgy_acct_auth_forwardable);
    if (field_written(form, (form_field_offset_t) ekn_acct_admin_auth_tgt))
        SET(auth_writeset, sec_rgy_acct_auth_tgt);
    if (field_written(form, (form_field_offset_t) ekn_acct_admin_auth_renewable))
        SET(auth_writeset, sec_rgy_acct_auth_renewable);
    if (field_written(form, (form_field_offset_t) ekn_acct_admin_auth_proxiable))
        SET(auth_writeset, sec_rgy_acct_auth_proxiable);
    if (field_written(form, (form_field_offset_t) ekn_acct_admin_auth_dup_session_key))
        SET(auth_writeset, sec_rgy_acct_auth_dup_skey);
    BITSET_COPY(src_admin_part->authentication_flags,
                header->admin_part.authentication_flags,
                ~auth_writeset);
}


/*
 * ek_acct_gen_passwd
 */
PUBLIC error_status_t ek_acct_gen_passwd (
    ek_op_handle_t      op_h,
    sec_passwd_type_t   *passwd_type,
    sec_passwd_rec_t    *passwd
)
{
    acct_header_t       header;
    error_status_t      st, lst;
    ek_op_t             *op;
    sec_passwd_rec_t    *pwd_p;
    unsigned32          num_pwds = 1,
                        num_returned;

    CLEAR_STATUS(&st);
    op = (ek_op_t *)op_h;
    form_read_header(op->op_form,  (Pointer)&header);

    if (*passwd_type == sec_passwd_des) {
        header.passwd.key.key_type = sec_passwd_des;
        sec_key_mgmt_gen_rand_key(rpc_c_authn_dce_private,
                                    (void *)NULL,
                                    (unsigned_char_p_t)header.key.pname,
                                    &header.passwd.key.key_type,
                                    sec_passwd_c_version_none,
                                    (void **) &pwd_p,
                                    &st);
        if (GOOD_STATUS(&st)) {
            header.passwd.version_number = sec_passwd_c_version_none;
            header.passwd.pepper         = NULL;
            memcpy(header.passwd.key.tagged_union.des_key,
                    pwd_p->key.tagged_union.des_key,
                    sec_passwd_c_des_key_size);
            sec_key_mgmt_free_key(pwd_p, &lst);
            header.passwd_written = true;
            header.plain_passwd_written = false;
            header.new_key_type = sec_passwd_des;
            form_write_header(op->op_form,  (Pointer)&header);
        }
    /*
     * NOTE : it is possible for the password management server
     * to generate passwords other than plaintext, but this can
     * easily be changed in the future if someone requests it.
     */
    } else if (*passwd_type == sec_passwd_plain) {
        sec_pwd_mgmt_gen_pwd(header.pwd_mgmt_h, num_pwds, &num_returned,
            passwd, &st);
    } else {
        SET_STATUS(&st, ek_kernel_failure);
    }

    return st;
}


/*
 * ek_acct_get_pwd_mgmt_info
 */
PUBLIC error_status_t ek_acct_get_pwd_mgmt_info (
    ek_op_handle_t    op_h,
    ek_pwd_val_type_t *pwd_val_type
)
{
    error_status_t        st;
    ek_op_t               *op;
    acct_header_t         header;
    sec_login_handle_t    your_lc;

    CLEAR_STATUS(&st);
    op = (ek_op_t *) op_h;
    form_read_header(op->op_form, (Pointer)&header);

    /* get current context */
    sec_login_get_current_context(&your_lc, &st);
    /* try using the default context */
    if (STATUS_EQUAL(&st, sec_login_s_no_current_context)) {
        your_lc = NULL;
        CLEAR_STATUS(&st);
    }

    /* setup handle */
    if (GOOD_STATUS(&st)) {
        sec_pwd_mgmt_setup(&header.pwd_mgmt_h, context, 
                           header.key, your_lc, NULL, &st);
    }

    /* get validation type */
    if (GOOD_STATUS(&st)) {
        sec_pwd_mgmt_get_val_type(header.pwd_mgmt_h, 
                (signed32 *) &header.pwd_val_type, &st);
    }

    if (GOOD_STATUS(&st)) {
        *pwd_val_type = header.pwd_val_type;
        form_write_header(op->op_form,  (Pointer)&header);
    }

    return st;
}


/*
 * ek_acct_reset_key_field
 */
PUBLIC error_status_t ek_acct_reset_key_field
#ifndef __STDC__
    ( op_h, domain )
    ek_op_handle_t    op_h;
    sec_rgy_domain_t  domain;
#else
  (
    ek_op_handle_t    op_h,
    sec_rgy_domain_t  domain
  )
#endif
{
    acct_header_t       header;
    error_status_t      st;
    ek_op_t             *op;

    CLEAR_STATUS(&st);
    op = (ek_op_t *)op_h;
    form_read_header(op->op_form,  (Pointer)&header);

    switch(domain) {
        case sec_rgy_domain_person:
            header.pname_key_written = false;
            break;
        case sec_rgy_domain_group:
            header.gname_key_written = false;
            break;
        case sec_rgy_domain_org:
            header.oname_key_written = false;
            break;
        default:
            SET_STATUS(&st, sec_rgy_bad_domain);
    }
    if (GOOD_STATUS(&st))
        form_write_header(op->op_form,  (Pointer)&header);
    return st;
}


/* FUNCTION acct_key_validate
 * checks the name for key for validity.  wildcards are ok.
 */
PRIVATE error_status_t acct_key_validate
#ifndef __STDC__
    ( form, name, domain )
    form_t            *form;
    char              *name;
    sec_rgy_domain_t  domain;
#else
  (
    form_t            *form,
    char              *name,
    sec_rgy_domain_t  domain
  )
#endif
{
    sec_rgy_login_name_t  name_result;
    sec_rgy_acct_user_t   user_part;
    sec_rgy_acct_admin_t  admin_part;
    sec_rgy_cursor_t      marker;
    sec_rgy_sid_t         sid;
    sec_rgy_unix_sid_t    unix_sid;
    sec_rgy_acct_key_t    key_parts;
    sec_rgy_plcy_auth_t   policy, *policy_p;
    acct_header_t         header;
    boolean32             wild_ok    = false;
    ek_op_type_t          op_type;
    error_status_t        st, tmp_st;

    SET_STATUS(&st, error_status_ok);
    form_read_header(form,  (Pointer)&header);
    op_type = header.op_type;
    policy_p = &policy;

    if (op_type == ek_op_query)
        wild_ok = true;    /* wildcards only acceptable on queries */

    if ( !(wild_ok && strlen(name) == 0) ) {
        st =  eku_chk_name(name, domain);
        if (BAD_STATUS(&st))  return st;
    }

    switch(domain) {
        case sec_rgy_domain_person:
            strcpy((char *)header.key.pname, name);
            header.pname_key_written = true;
            break;
        case sec_rgy_domain_group:
            strcpy((char *)header.key.gname, name);
            header.gname_key_written = true;
            break;
        case sec_rgy_domain_org:
            strcpy((char *)header.key.oname, name);
            header.oname_key_written = true;
            break;
        default:
            SET_STATUS(&st, ek_kernel_failure);
            return st;
    }

    if (op_type == ek_op_query) {   /* nothing more to do for queries */
        form_write_header(form,  (Pointer)&header);
        return st;
    }

    if (ACCT_KEY_WRITTEN(header)) {

        /* do a lookup on the acct key */
        sec_rgy_cursor_reset(&marker);
        sec_rgy_acct_lookup(context, &(header.key), &marker, &name_result,
                            &sid, &unix_sid, &key_parts, &user_part,
                            &admin_part, &st);
        if (GOOD_STATUS(&st) && !header.policy_written) {
            sec_rgy_auth_plcy_get_info(context,&(header.key),&policy,&tmp_st);
            if (STATUS_EQUAL(&tmp_st, sec_rgy_object_not_found)) {
                policy_p = NULL;
                header.policy_written = false;
            } else {
                header.policy = policy;
                header.policy_written = true;
            }
        }

        switch (op_type) {
        case ek_op_add:
            /* new acct must not already exist */
            if (ACCT_NOT_FOUND(st)) {
                SET_STATUS(&st, error_status_ok);
            } else if (GOOD_STATUS(&st)) {
                SET_STATUS(&st, ek_err_acct_exists);
            } else {
                eku_translate_status(&st);
            }

            /* originally planned to insure the appropriate group/org
               membership here but it's much easier to wait
               until the add is actually performed and check for
               the rgy_not_member_?*  status returns */
            break;

        case ek_op_delete:
        case ek_op_change:
            /* deleted/changed acct must already exist */
            if (ACCT_NOT_FOUND(st)) {
                SET_STATUS(&st, ek_err_acct_does_not_exist);
            } else {
                eku_translate_status(&st);
            }

            if (BAD_STATUS(&st))  return st;

            if (op_type == ek_op_change) {
                /* If the query succeeded, then the results become the default values
                 * for any non-key fields not previously written by the interface
                 */
                copy_acct_change_defaults(form, &header, &user_part, &admin_part,
                                          &key_parts);

                /* write the query results to the change form so they're available to
                 *  the interface as defaults (via ek_read_field)
                 */
                acct_query_unconvert(form, &header.key, header.key_parts, &sid,
                                     &unix_sid, &(header.user_part),
                                     &(header.admin_part), policy_p);
            } else {
                acct_query_unconvert(form, &header.key, key_parts,
                                     &sid, &unix_sid, &user_part, &admin_part,
                                     policy_p);
            }
        }
    }

    if (op_type == ek_op_add)
        switch(domain) {
            case sec_rgy_domain_person:
                st = chk_pgo_exists(name, sec_rgy_domain_person);
                break;
            case sec_rgy_domain_group:
                st = chk_pgo_exists(name, sec_rgy_domain_group);
                break;
            case sec_rgy_domain_org:
                st = chk_pgo_exists(name, sec_rgy_domain_org);
                break;
            default:
                SET_STATUS(&st, ek_kernel_failure);
                return st;
        }

    if (GOOD_STATUS(&st))
        form_write_header(form,  (Pointer)&header);
    return st;
}


/* FUNCTION acct_pname_key_validate
 * checks the pname key for validity.  wildcards are ok
 */
PRIVATE error_status_t acct_pname_key_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    return(acct_key_validate(form, name, sec_rgy_domain_person));
}


/* FUNCTION acct_gname_key_validate
 * checks the gname key for validity.
 */
PRIVATE error_status_t acct_gname_key_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    return(acct_key_validate(form, name, sec_rgy_domain_group));
}


/* FUNCTION acct_oname_key_validate
 * checks the oname key for validity.
 */
PRIVATE error_status_t acct_oname_key_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    return(acct_key_validate(form, name, sec_rgy_domain_org));
}


/* acct_name_validate
 * The name field is only writeable by the change operation.
 */
PRIVATE error_status_t acct_name_validate
#ifndef __STDC__
    ( form, name, domain )
    form_t            *form;
    char              *name;
    sec_rgy_domain_t  domain;
#else
  (
    form_t            *form,
    char              *name,
    sec_rgy_domain_t  domain
  )
#endif
{
    acct_header_t         header;
    error_status_t        st;

    st =  eku_chk_name(name, domain);

    /* make sure the person, group and org components are real */
    if (GOOD_STATUS(&st))
        st = chk_pgo_exists(name, domain);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);
    switch (domain) {
    case sec_rgy_domain_person:
        strcpy((char *)header.new_key.pname, name);
        break;
    case sec_rgy_domain_group:
        strcpy((char *)header.new_key.gname, name);
        break;
    case sec_rgy_domain_org:
        strcpy((char *)header.new_key.oname, name);
        break;
    }

    if (GOOD_STATUS(&st)) {
        switch (domain) {
        case sec_rgy_domain_person:
            header.new_pname_key_written = true;
            break;
        case sec_rgy_domain_group:
            header.new_gname_key_written = true;
            break;
        case sec_rgy_domain_org:
            header.new_oname_key_written = true;
            break;
        }
        form_write_header(form,  (Pointer)&header);
    }

    return st;
}


/*
 * acct_pname_validate
 */
PRIVATE error_status_t acct_pname_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    return(acct_name_validate(form, name, sec_rgy_domain_person));
}


/*
 * acct_gname_validate
 */
PRIVATE error_status_t acct_gname_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    return(acct_name_validate(form, name, sec_rgy_domain_group));
}


/*
 * acct_pname_validate
 */
PRIVATE error_status_t acct_oname_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    return(acct_name_validate(form, name, sec_rgy_domain_org));
}


/* acct_key_parts_validate
 *  Checks that the value passed in is of the proper kernel "type".
 *  The same restrictions apply to adds and changes.
 *
 *  returns:
 *     ek_bad_format       - if not the proper kernel type
 */
PRIVATE error_status_t acct_key_parts_validate
#ifndef __STDC__
    ( form, abbrev )
    form_t    *form;
    char      *abbrev;
#else
  (
    form_t    *form,
    char      *abbrev
  )
#endif
{
    acct_header_t   header;
    error_status_t  st;

    if (strlen(abbrev) != ek_lim_max_key_parts_len) {
        SET_STATUS(&st, ek_bad_format);
        return st;
    }
    if (*abbrev != *ek_acct_key_person) {
        SET_STATUS(&st, ek_bad_format);
        return st;
    }

    CLEAR_STATUS(&st);

    form_read_header(form,  (Pointer)&header);

    header.admin_part_written = true;
    if (*abbrev == *(ek_acct_key_person))
        header.key_parts = sec_rgy_acct_key_person;
    else if (*abbrev == *(ek_acct_key_group))
        header.key_parts = sec_rgy_acct_key_group;
    else if (*abbrev == *(ek_acct_key_org))
        header.key_parts = sec_rgy_acct_key_org;

    form_write_header(form,  (Pointer)&header);

    return st;
}


/* FUNCTION acct_my_plain_passwd_validate
 *
 * validates a plain text passwd using the policy in effect for the registry
 * as whole.  Organization policy  applies to passwords for accounts
 * with membership in the org, but we can't check that until the update operation
 * is actually invoked because we don't have the acct sid available until then.
 * zero length password means no passwd.  To keep the default passwd on a change
 * the interface should not write anything to the ekn_pgo_plain_passwd field.  To
 * change from an existing password to no password, a zero length string should
 * be written.
 *
 * returns
 *    ek_err_passwd_too_long
 *    ek_err_passwd_too_short
 *    ek_err_passwd_no_alpha
 *    ek_err_passwd_spaces
 */
PRIVATE error_status_t acct_my_plain_passwd_validate
#ifndef __STDC__
    ( form, passwd )
    form_t    *form;
    char      *passwd;
#else
  (
    form_t    *form,
    char      *passwd
  )
#endif
{
    sec_rgy_plcy_t  policy;
    acct_header_t   header;
    error_status_t  st;

    CLEAR_STATUS(&st);

    form_read_header(form,  (Pointer)&header);

    strcpy( (char *) header.my_plain_passwd, passwd);
    PASSWD_TYPE(&header.my_passwd) = sec_passwd_plain;

    form_write_header(form,  (Pointer)&header);

    return st;
}


/* FUNCTION acct_plain_passwd_validate
 *
 * validates a plain text passwd using the policy in effect for the registry
 * as whole.  Organization policy  applies to passwords for accounts
 * with membership in the org, but we can't check that until the update operation
 * is actually invoked because we don't have the acct sid available until then.
 * zero length password means no passwd.  To keep the default passwd on a change
 * the interface should not write anything to the ekn_pgo_plain_passwd field.  To
 * change from an existing password to no password, a zero length string should
 * be written.
 *
 * returns
 *    ek_err_passwd_too_long
 *    ek_err_passwd_too_short
 *    ek_err_passwd_no_alpha
 *    ek_err_passwd_spaces
 */
PRIVATE error_status_t acct_plain_passwd_validate
#ifndef __STDC__
    ( form, passwd )
    form_t    *form;
    char      *passwd;
#else
  (
    form_t    *form,
    char      *passwd
  )
#endif
{
    sec_rgy_plcy_t  policy;
    acct_header_t   header;
    error_status_t  st;

    st = read_rgy_policy(&policy);
    if (BAD_STATUS(&st)) return st;

    st = eku_chk_plain_passwd(passwd, strlen(passwd), policy, false);

    if (GOOD_STATUS(&st)) {
        form_read_header(form,  (Pointer)&header);

        strcpy( (char *) header.plain_passwd, passwd);
        PASSWD_TYPE(&header.passwd) = sec_passwd_plain;

        /* These initializations are fine as long as we only support
         * plaintext that is converted to a des key by the server using the
         * next available key version number. If we ever support more than this,
         * lots of things will have to change
         */
        header.passwd.pepper = NULL;     /* server assigns a prefix */
        header.passwd.version_number = sec_passwd_c_version_none;
        header.new_key_type = sec_passwd_des;

        header.plain_passwd_written = true;
        form_write_header(form,  (Pointer)&header);
    }

    return st;
}


PRIVATE error_status_t acct_misc_validate
#ifndef __STDC__
    ( form, misc )
    form_t  *form;
    char    *misc;
#else
  (
    form_t  *form,
    char    *misc
  )
#endif
{
    acct_header_t  header;
    error_status_t       st;

    SET_STATUS(&st, error_status_ok);

    if (strlen(misc) > ek_lim_max_pname_len) {
        SET_STATUS(&st, ek_err_misc_info_too_long);
    } else {
        form_read_header(form,  (Pointer)&header);
        strcpy((char *)header.user_part.gecos, misc);
        header.user_part_written = true;
        form_write_header(form,  (Pointer)&header);
    }

    return st;
}


PRIVATE error_status_t acct_homedir_validate
#ifndef __STDC__
    ( form, homedir )
    form_t  *form;
    char    *homedir;
#else
  (
    form_t  *form,
    char    *homedir
  )
#endif
{
    acct_header_t   header;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    if (strlen(homedir) > ek_lim_max_pname_len) {
        SET_STATUS(&st, ek_err_homedir_too_long);
    } else {
        form_read_header(form,  (Pointer)&header);
        strcpy((char *)header.user_part.homedir, homedir);
        header.user_part_written = true;
        form_write_header(form,  (Pointer)&header);
    }

    return st;
}


PRIVATE error_status_t acct_shell_validate
#ifndef __STDC__
    ( form, shell )
    form_t  *form;
    char    *shell;
#else
  (
    form_t  *form,
    char    *shell
  )
#endif
{
    acct_header_t   header;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    if (strlen(shell) > ek_lim_max_pname_len) {
        SET_STATUS(&st, ek_err_shell_too_long);
    } else {
        form_read_header(form,  (Pointer)&header);
        strcpy((char *)header.user_part.shell, shell);
        header.user_part_written = true;
        form_write_header(form,  (Pointer)&header);
    }

    return st;
}


/*
 * FUNCTION acct_passwd_valid_validate
 */
PRIVATE error_status_t acct_passwd_valid_validate
#ifndef __STDC__
    ( form, flag )
    form_t    *form;
    char      *flag;
#else
  (
    form_t    *form,
    char      *flag
  )
#endif
{
    acct_header_t   header;
    error_status_t  st;
    boolean32       flag_set;

    SET_STATUS(&st, error_status_ok);

    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);

    if (flag_set)
        SET(header.user_part.flags, sec_rgy_acct_user_passwd_valid);
    else
        UNSET(header.user_part.flags, sec_rgy_acct_user_passwd_valid);

    header.user_part_written = true;
    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION acct_admin_server_validate
 */
PRIVATE error_status_t acct_admin_server_validate
#ifndef __STDC__
    ( form, flag )
    form_t    *form;
    char      *flag;
#else
  (
    form_t    *form,
    char      *flag
  )
#endif
{
    acct_header_t   header;
    boolean32       flag_set;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form, (Pointer) &header);
    if (flag_set)
        SET(header.admin_part.flags, sec_rgy_acct_admin_server);
    else
        UNSET(header.admin_part.flags, sec_rgy_acct_admin_server);

    header.admin_part_written = true;
    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION acct_admin_client_validate
 */
PRIVATE error_status_t acct_admin_client_validate
#ifndef __STDC__
    ( form, flag )
    form_t    *form;
    char      *flag;
#else
  (
    form_t    *form,
    char      *flag
  )
#endif
{
    acct_header_t   header;
    boolean32       flag_set;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);
    if (flag_set)
        SET(header.admin_part.flags, sec_rgy_acct_admin_client);
    else
        UNSET(header.admin_part.flags, sec_rgy_acct_admin_client);

    header.admin_part_written = true;
    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION acct_admin_valid_validate
 */
PRIVATE error_status_t acct_admin_valid_validate
#ifndef __STDC__
    ( form, flag )
    form_t    *form;
    char      *flag;
#else
  (
    form_t    *form,
    char      *flag
  )
#endif
{
    acct_header_t   header;
    boolean32       flag_set;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);
    if (flag_set)
        SET(header.admin_part.flags, sec_rgy_acct_admin_valid);
    else
        UNSET(header.admin_part.flags, sec_rgy_acct_admin_valid);

    header.admin_part_written = true;
    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION acct_auth_post_dated_validate
 */
PRIVATE error_status_t acct_auth_post_dated_validate
#ifndef __STDC__
    ( form, flag )
    form_t    *form;
    char      *flag;
#else
  (
    form_t    *form,
    char      *flag
  )
#endif
{
    acct_header_t   header;
    boolean32       flag_set;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);
    if (flag_set)
        SET(header.admin_part.authentication_flags,
            sec_rgy_acct_auth_post_dated);
    else
        UNSET(header.admin_part.authentication_flags,
                sec_rgy_acct_auth_post_dated);

    header.admin_part_written = true;
    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION acct_auth_forwardable_validate
 */
PRIVATE error_status_t acct_auth_forwardable_validate
#ifndef __STDC__
    ( form, flag )
    form_t    *form;
    char      *flag;
#else
  (
    form_t    *form,
    char      *flag
  )
#endif
{
    acct_header_t   header;
    boolean32       flag_set;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);
    if (flag_set)
        SET(header.admin_part.authentication_flags,
            sec_rgy_acct_auth_forwardable);
    else
        UNSET(header.admin_part.authentication_flags,
                sec_rgy_acct_auth_forwardable);

    header.admin_part_written = true;
    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION acct_auth_tgt_validate
 */
PRIVATE error_status_t acct_auth_tgt_validate
#ifndef __STDC__
    ( form, flag )
    form_t    *form;
    char      *flag;
#else
  (
    form_t    *form,
    char      *flag
  )
#endif
{
    acct_header_t   header;
    boolean32       flag_set;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);
    if (flag_set)
        SET(header.admin_part.authentication_flags,
            sec_rgy_acct_auth_tgt);
    else
        UNSET(header.admin_part.authentication_flags,
                sec_rgy_acct_auth_tgt);

    header.admin_part_written = true;
    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION acct_auth_renewable_validate
 */
PRIVATE error_status_t acct_auth_renewable_validate
#ifndef __STDC__
    ( form, flag )
    form_t    *form;
    char      *flag;
#else
  (
    form_t    *form,
    char      *flag
  )
#endif
{
    acct_header_t   header;
    boolean32       flag_set;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);
    if (flag_set)
        SET(header.admin_part.authentication_flags,
            sec_rgy_acct_auth_renewable);
    else
        UNSET(header.admin_part.authentication_flags,
                sec_rgy_acct_auth_renewable);

    header.admin_part_written = true;
    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION acct_auth_proxiable_validate
 */
PRIVATE error_status_t acct_auth_proxiable_validate
#ifndef __STDC__
    ( form, flag )
    form_t    *form;
    char      *flag;
#else
  (
    form_t    *form,
    char      *flag
  )
#endif
{
    acct_header_t   header;
    boolean32       flag_set;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);
    if (flag_set)
        SET(header.admin_part.authentication_flags,
            sec_rgy_acct_auth_proxiable);
    else
        UNSET(header.admin_part.authentication_flags,
                sec_rgy_acct_auth_proxiable);

    header.admin_part_written = true;
    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION acct_auth_dup_skey_validate
 */
PRIVATE error_status_t acct_auth_dup_skey_validate
#ifndef __STDC__
    ( form, flag )
    form_t    *form;
    char      *flag;
#else
  (
    form_t    *form,
    char      *flag
  )
#endif
{
    acct_header_t   header;
    boolean32       flag_set;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    st = eku_chk_flag(flag, strlen(flag), &flag_set);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);
    if (flag_set)
        SET(header.admin_part.authentication_flags,
            sec_rgy_acct_auth_dup_skey);
    else
        UNSET(header.admin_part.authentication_flags,
                sec_rgy_acct_auth_dup_skey);

    header.admin_part_written = true;
    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION acct_exp_date_validate
 */
PRIVATE error_status_t acct_exp_date_validate
#ifndef __STDC__
    ( form, date )
    form_t    *form;
    char      *date;
#else
  (
    form_t    *form,
    char      *date
  )
#endif
{
    acct_header_t        header;
    sec_timeval_sec_t    clockh;
    error_status_t       st;

    SET_STATUS(&st, error_status_ok);

    st = eku_chk_date(date, strlen(date), &clockh);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);
    header.admin_part_written = true;
    header.admin_part.expiration_date = clockh;
    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION acct_good_since_date_validate
 */
PRIVATE error_status_t acct_good_since_date_validate
#ifndef __STDC__
    ( form, date )
    form_t    *form;
    char      *date;
#else
  (
    form_t    *form,
    char      *date
  )
#endif
{
    acct_header_t       header;
    sec_timeval_sec_t   clockh;
    error_status_t      st;
    time_t              tloc;
    ek_date_buf_t       timestr;

    SET_STATUS(&st, error_status_ok);

    /* Get the current time if 'now' is specified */
    if (strcmp(date, "now") == 0) {
        time(&tloc);
        strftime(timestr, sizeof(ek_date_buf_t), "%Y/%m/%d.%H:%M", localtime(&tloc));
        date = timestr;
    }

    st = eku_chk_date(date, strlen(date), &clockh);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);
    header.admin_part_written = true;
    header.admin_part.good_since_date = clockh;
    form_write_header(form,  (Pointer)&header);

    return st;
}


/* LOCAL FUNCTION get_acct_form
 * allocates and initiializes a form for a account operation.
 *
 * returns:   ek_null_handle    -   in the stp pointer when a form can't be
 *                                  allocated
 */
PRIVATE void get_acct_form
#ifndef __STDC__
    ( acct_form, op_type, stp )
    form_t          **acct_form;    /* IN - pointer to the operation form */
    ek_op_type_t    op_type;        /* IN - operation type */
    error_status_t  *stp;           /* OUT */
#else
  (
    form_t          **acct_form,    /* IN - pointer to the operation form */
    ek_op_type_t    op_type,        /* IN - operation type */
    error_status_t  *stp
  )
#endif
{
    form_field_offset_t  cur_field;
    form_bitset_t        attributes;
    acct_header_t        acct_header;
    form_t               *new_form;

    SET_STATUS(stp, error_status_ok);

    new_form = form_alloc(ekn_acct_num_fields, sizeof(acct_header_t));

    if (new_form == form_null_form) {
        SET_STATUS(stp, ek_null_handle);
        return;
    } else {
        init_acct_header(&acct_header, op_type);
        form_write_header(new_form,  (Pointer)&acct_header);
    }

    if (op_type == ek_op_query) {

            /* the key field is writable and required */
        attributes = (0 | attribute_readwrite_mask | attribute_required_mask);
        form_init_field(new_form, ekn_acct_pname_key , attributes,
                        acct_pname_key_validate);
        attributes = (0 | attribute_readwrite_mask | attribute_required_mask);
        form_init_field(new_form, ekn_acct_gname_key , attributes,
                        acct_gname_key_validate);
        attributes = (0 | attribute_readwrite_mask | attribute_required_mask);
        form_init_field(new_form, ekn_acct_oname_key,
                        attributes, acct_oname_key_validate);

        /* the rest of the fields are readonly and have no validation function */
        attributes = (0 | attribute_readonly_mask);
        for (cur_field = 0; cur_field < ekn_acct_pname_key; cur_field++)
            form_init_field(new_form, cur_field, attributes,
                             (form_validate_t)form_null_function);

            /* except for the plain password field, which is unused */
        attributes = (0 | attribute_unused_mask);
        form_init_field(new_form, ekn_acct_user_plain_passwd, attributes,
                         (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_acct_user_my_plain_passwd, attributes,
                         (form_validate_t)form_null_function);

    } else {
            /* common acct update field initializations */

            /* virtual fields, and server-maintained fields
             * are always readonly */
        attributes = (0 | attribute_readonly_mask);
        form_init_field(new_form, ekn_acct_sid_person, attributes,
                         (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_acct_sid_group, attributes,
                         (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_acct_sid_org, attributes,
                         (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_acct_unix_sid_person, attributes,
                         (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_acct_unix_sid_group, attributes,
                         (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_acct_unix_sid_org, attributes,
                         (form_validate_t)form_null_function);

        form_init_field(new_form, ekn_acct_user_enc_passwd, attributes,
                         (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_acct_user_my_enc_passwd, attributes,
                         (form_validate_t)form_null_function);

        form_init_field(new_form, ekn_acct_user_passwd_version, attributes,
                         (form_validate_t)form_null_function);

        form_init_field(new_form, ekn_acct_user_passwd_dtm, attributes,
                         (form_validate_t)form_null_function);

        /* server-maintained fields aren't writable in updates */
        form_init_field(new_form, ekn_acct_admin_creator, attributes,
                         (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_acct_admin_when_created, attributes,
                         (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_acct_admin_changer, attributes,
                         (form_validate_t)form_null_function);
        form_init_field(new_form, ekn_acct_admin_when_changed, attributes,
                         (form_validate_t)form_null_function);

        switch (op_type) {

        case ek_op_delete:
                /* only the key field is writeable - it's required */
            attributes = (0 | attribute_readwrite_mask |
                          attribute_required_mask);
            form_init_field(new_form, ekn_acct_pname_key, attributes,
                             acct_pname_key_validate);
            form_init_field(new_form, ekn_acct_gname_key, attributes,
                             acct_gname_key_validate);
            form_init_field(new_form, ekn_acct_oname_key, attributes,
                             acct_oname_key_validate);

                /* don't need a plain passwd for deletes */
            attributes = (0 | attribute_unused_mask);
            form_init_field(new_form, ekn_acct_user_plain_passwd, attributes,
                             (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_user_my_plain_passwd, attributes,
                             (form_validate_t)form_null_function);

                /* we'll give them the rest of the administerable fields back
                 * readonly, in case they're useful */
            attributes = (0 | attribute_readonly_mask);
            form_init_field(new_form, ekn_acct_pname, attributes,
                            (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_gname, attributes,
                            (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_oname, attributes,
                            (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_key_parts, attributes,
                            (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_key_parts, attributes,
                            (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_user_misc, attributes,
                            (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_user_homedir, attributes,
                            (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_user_shell, attributes,
                            (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_user_passwd_valid, attributes,
                            (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_admin_exp_date, attributes,
                            (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_admin_acct_server,
                            attributes, (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_admin_acct_client,
                            attributes, (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_admin_acct_valid,
                            attributes, (form_validate_t)form_null_function);
            form_init_field(new_form,ekn_acct_admin_good_since_date,
                            attributes, (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_max_ticket_lifetime,
                            attributes, (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_max_renewable_lifetime,
                            attributes, (form_validate_t)form_null_function);
            form_init_field(new_form,ekn_acct_admin_auth_post_dated,
                            attributes, (form_validate_t)form_null_function);
            form_init_field(new_form,ekn_acct_admin_auth_forwardable,
                            attributes, (form_validate_t)form_null_function);
            form_init_field(new_form,ekn_acct_admin_auth_tgt,
                            attributes, (form_validate_t)form_null_function);
            form_init_field(new_form,ekn_acct_admin_auth_renewable,
                            attributes, (form_validate_t)form_null_function);
            form_init_field(new_form,ekn_acct_admin_auth_proxiable,
                            attributes, (form_validate_t)form_null_function);
            form_init_field(new_form,ekn_acct_admin_auth_dup_session_key,
                            attributes, (form_validate_t)form_null_function);
            break;

        case ek_op_add:

                /* all the administerable fields are writable and required */
            attributes = (0 | attribute_readwrite_mask |
                          attribute_required_mask);
            form_init_field(new_form, ekn_acct_pname_key, attributes,
                            acct_pname_key_validate);
            form_init_field(new_form, ekn_acct_gname_key, attributes,
                            acct_gname_key_validate);
            form_init_field(new_form, ekn_acct_oname_key, attributes,
                            acct_oname_key_validate);
            form_init_field(new_form, ekn_acct_key_parts, attributes,
                            acct_key_parts_validate);
            form_init_field(new_form, ekn_acct_user_my_plain_passwd,attributes,
                            acct_my_plain_passwd_validate);
            form_init_field(new_form, ekn_acct_user_misc, attributes,
                            acct_misc_validate);
            form_init_field(new_form, ekn_acct_user_homedir, attributes,
                            acct_homedir_validate);
            form_init_field(new_form, ekn_acct_user_shell, attributes,
                            acct_shell_validate);
            form_init_field(new_form, ekn_acct_user_passwd_valid, attributes,
                            acct_passwd_valid_validate);
            form_init_field(new_form, ekn_acct_admin_exp_date, attributes,
                            acct_exp_date_validate);
            form_init_field(new_form, ekn_acct_admin_acct_server, attributes,
                            acct_admin_server_validate);
            form_init_field(new_form, ekn_acct_admin_acct_client, attributes,
                            acct_admin_client_validate);
            form_init_field(new_form, ekn_acct_admin_acct_valid, attributes,
                            acct_admin_valid_validate);
            form_init_field(new_form,ekn_acct_admin_good_since_date,attributes,
                            acct_good_since_date_validate);

            attributes = (0 | attribute_readwrite_mask);
                /* except passwd, sinca a random passwd may have been set */
            form_init_field(new_form, ekn_acct_user_plain_passwd, attributes,
                            acct_plain_passwd_validate);

                /* acct authorization flags */
            form_init_field(new_form, ekn_acct_admin_auth_post_dated,
                            attributes,
                            (form_validate_t) acct_auth_post_dated_validate);
            form_init_field(new_form, ekn_acct_admin_auth_forwardable,
                            attributes,
                            (form_validate_t) acct_auth_forwardable_validate);
            form_init_field(new_form, ekn_acct_admin_auth_tgt,
                            attributes,
                            (form_validate_t) acct_auth_tgt_validate);
            form_init_field(new_form, ekn_acct_admin_auth_renewable,
                            attributes,
                            (form_validate_t) acct_auth_renewable_validate);
            form_init_field(new_form, ekn_acct_admin_auth_proxiable,
                            attributes,
                            (form_validate_t) acct_auth_proxiable_validate);
            form_init_field(new_form, ekn_acct_admin_auth_dup_session_key,
                            attributes,
                            (form_validate_t) acct_auth_dup_skey_validate);

                /* optional auth policy fields */
            attributes = (0 | attribute_readwrite_mask);
            form_init_field(new_form, ekn_acct_max_ticket_lifetime, attributes,
                            (form_validate_t)acct_auth_plcy_max_ticket_validate);
            form_init_field(new_form, ekn_acct_max_renewable_lifetime, attributes,
                            (form_validate_t)acct_auth_plcy_max_renewable_validate);

            /* except the name field is unused */
            attributes = (0 | attribute_unused_mask);
            form_init_field(new_form, ekn_acct_pname, attributes,
                            (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_gname, attributes,
                            (form_validate_t)form_null_function);
            form_init_field(new_form, ekn_acct_oname, attributes,
                            (form_validate_t)form_null_function);
            break;

        case ek_op_change:

                /* the key field is required */
            attributes = (0 | attribute_readwrite_mask |
                            attribute_required_mask);
            form_init_field(new_form, ekn_acct_pname_key, attributes,
                            acct_pname_key_validate);
            form_init_field(new_form, ekn_acct_gname_key, attributes,
                            acct_gname_key_validate);
            form_init_field(new_form, ekn_acct_oname_key, attributes,
                            acct_oname_key_validate);

                /* the rest of the administerable fields are writable,
                 * but optional */
            attributes = (0 | attribute_readwrite_mask);
            form_init_field(new_form, ekn_acct_pname, attributes,
                            acct_pname_validate);
            form_init_field(new_form, ekn_acct_gname, attributes,
                            acct_gname_validate);
            form_init_field(new_form, ekn_acct_oname, attributes,
                            acct_oname_validate);
            form_init_field(new_form, ekn_acct_key_parts, attributes,
                            acct_key_parts_validate);
            form_init_field(new_form, ekn_acct_user_plain_passwd, attributes,
                            acct_plain_passwd_validate);
            form_init_field(new_form, ekn_acct_user_my_plain_passwd,attributes,
                            acct_my_plain_passwd_validate);
            form_init_field(new_form, ekn_acct_user_misc, attributes,
                            acct_misc_validate);
            form_init_field(new_form, ekn_acct_user_homedir, attributes,
                            acct_homedir_validate);
            form_init_field(new_form, ekn_acct_user_shell, attributes,
                            acct_shell_validate);
            form_init_field(new_form, ekn_acct_user_passwd_valid, attributes,
                            acct_passwd_valid_validate);
            form_init_field(new_form, ekn_acct_admin_exp_date, attributes,
                            acct_exp_date_validate);
            form_init_field(new_form, ekn_acct_admin_acct_server,attributes,
                            acct_admin_server_validate);
            form_init_field(new_form, ekn_acct_admin_acct_client,attributes,
                            acct_admin_client_validate);
            form_init_field(new_form, ekn_acct_admin_acct_valid, attributes,
                            acct_admin_valid_validate);
            form_init_field(new_form,ekn_acct_admin_good_since_date,attributes,
                            acct_good_since_date_validate);
            form_init_field(new_form, ekn_acct_max_ticket_lifetime, attributes,
                          (form_validate_t)acct_auth_plcy_max_ticket_validate);
            form_init_field(new_form, ekn_acct_max_renewable_lifetime,
                            attributes, (form_validate_t)
                            acct_auth_plcy_max_renewable_validate);

                /* acct authorization flags */
            form_init_field(new_form, ekn_acct_admin_auth_post_dated,
                            attributes,
                            (form_validate_t) acct_auth_post_dated_validate);
            form_init_field(new_form, ekn_acct_admin_auth_forwardable,
                            attributes,
                            (form_validate_t) acct_auth_forwardable_validate);
            form_init_field(new_form, ekn_acct_admin_auth_tgt,
                            attributes,
                            (form_validate_t) acct_auth_tgt_validate);
            form_init_field(new_form, ekn_acct_admin_auth_renewable,
                            attributes,
                            (form_validate_t) acct_auth_renewable_validate);
            form_init_field(new_form, ekn_acct_admin_auth_proxiable,
                            attributes,
                            (form_validate_t) acct_auth_proxiable_validate);
            form_init_field(new_form, ekn_acct_admin_auth_dup_session_key,
                            attributes,
                            (form_validate_t) acct_auth_dup_skey_validate);
            break;
        }
    }

    if (GOOD_STATUS(stp))
        *acct_form = new_form;
}


/* FUNCTION force_group_org_membership
 *
 * Adds the specified person to the specified group or org if they are not
 * already a  member. Returns true if the person was added AND was not already
 * a member,  false otherwise.
 *
 * NOTE: This function is meant to be used only with acct add/change operations.
 *       It does not report a particular failure status because it is assumed
 *       the  caller will eventually attempt an add.  By returning true the
 *       routine is telling the caller that the member add needs to be "undone".
 */
PRIVATE boolean32 force_group_org_membership
#ifndef __STDC__
    (context, p_name, go_name, domain, stp )
    sec_rgy_handle_t    context;
    char                *p_name;
    char                *go_name;
    sec_rgy_domain_t    domain;
    error_status_t      *stp;
#else
  (
    sec_rgy_handle_t    context,
    char                *p_name,
    char                *go_name,
    sec_rgy_domain_t    domain,
    error_status_t      *stp
  )
#endif
{
    boolean32       added_one = false, done, is_member;
    error_status_t  st;
    CLEAR_STATUS(stp);

    is_member = sec_rgy_pgo_is_member(context, domain,
                                        (unsigned_char_p_t) go_name,
                                        (unsigned_char_p_t) p_name, &st);
    if (BAD_STATUS(&st))
        return added_one;

    if (!is_member) {
        sec_rgy_pgo_add_member(context, domain, (unsigned_char_t *)go_name,
                                (unsigned_char_t *)p_name, stp);
        if (GOOD_STATUS(stp))
            added_one = true;
        else if (STATUS_EQUAL(stp, sec_rgy_not_authorized)) {
            switch(domain) {
                case sec_rgy_domain_group:
                    SET_STATUS(stp, ek_err_g_mbr_unauth);
                    break;
                case sec_rgy_domain_org:
                    SET_STATUS(stp, ek_err_o_mbr_unauth);
                    break;
            }
        }
    }

    return added_one;
}


/********************************************************************************
 * Local Registry Entry Operations
 ********************************************************************************/

/* LOCAL FUNCTION get_lrgy_entry_form
 * allocates and initiializes a form for a lrgy entry operation.
 *
 * returns:   ek_null_handle    -   in the stp pointer when a form can't be
 *                                  allocated
 */
PRIVATE void get_lrgy_entry_form
#ifndef __STDC__
    ( lrgy_entry_form, stp )
    form_t          **lrgy_entry_form;  /* IN - pointer to the operation form */
    error_status_t  *stp;               /* OUT */
#else
  (
    form_t          **lrgy_entry_form,  /* IN - pointer to the operation form */
    error_status_t  *stp
  )
#endif
{
    form_field_offset_t  cur_field;
    form_bitset_t        attributes;
    lrgy_entry_header_t  entry_header;
    form_t               *new_form;
    CLEAR_STATUS(stp);

    new_form = form_alloc(ekn_lrgy_entry_num_fields,
                            sizeof(lrgy_entry_header_t));
    if (new_form == form_null_form) {
        SET_STATUS(stp, ek_null_handle);
    } else {
            /* all fields are readonly */
        attributes = (0 | attribute_readonly_mask);
        for (cur_field = 0; cur_field < ekn_lrgy_entry_num_fields; cur_field++)
            form_init_field(new_form, cur_field, attributes,
                            (form_validate_t)form_null_function);
        form_write_header(new_form,  (Pointer)&entry_header);
    }

    if (GOOD_STATUS(stp)) {
        *lrgy_entry_form = new_form;
    }
}


/*
 * LOCAL FUNCTION clear_lrgy_entry_form
 */
PRIVATE error_status_t clear_lrgy_entry_form
#ifndef __STDC__
    ( form )
     form_t        *form;
#else
  (
     form_t        *form
  )
#endif
{
    lrgy_entry_header_t header;
    error_status_t      st;

    form_read_header(form,  (Pointer)&header);
    sec_lrgy_entry_cursor_init(&(header.cursor), &st);
    form_write_header(form,  (Pointer)&header);
    form_clear(form);
    return st;
}


/*
 * LOCAL FUNCTION lrgy_entry_query_unconvert
 */
PRIVATE void lrgy_entry_query_unconvert
#ifndef __STDC__
    ( lrgy_entry_form, entryP, timestampP )
    form_t                  *lrgy_entry_form;
    sec_lrgy_entry_t        *entryP;
    sec_timeval_sec_t       *timestampP;
#else
  (
    form_t                  *lrgy_entry_form,
    sec_lrgy_entry_t        *entryP,
    sec_timeval_sec_t       *timestampP
  )
#endif
{
    lrgy_entry_header_t     header;
    form_read_header(lrgy_entry_form,  (Pointer)&header);

    form_write(lrgy_entry_form, ekn_lrgy_entry_cell, entryP->cell,
               strlen((char *)entryP->cell), false, &Dummy_st);
    form_write(lrgy_entry_form, ekn_lrgy_entry_princ, entryP->principal,
               strlen((char *)entryP->principal), false, &Dummy_st);
    form_write(lrgy_entry_form, ekn_lrgy_entry_pw_name, entryP->pw_name,
               strlen((char *)entryP->pw_name), false, &Dummy_st);
    form_write(lrgy_entry_form, ekn_lrgy_entry_pw_passwd, entryP->pw_passwd,
               strlen((char *)entryP->pw_passwd), false, &Dummy_st);
    form_write(lrgy_entry_form, ekn_lrgy_entry_pw_class, entryP->pw_class,
               strlen((char *)entryP->pw_class), false, &Dummy_st);
    form_write(lrgy_entry_form, ekn_lrgy_entry_pw_gecos, entryP->pw_gecos,
               strlen((char *)entryP->pw_gecos), false, &Dummy_st);
    form_write(lrgy_entry_form, ekn_lrgy_entry_pw_dir, entryP->pw_dir,
               strlen((char *)entryP->pw_dir), false, &Dummy_st);
    form_write(lrgy_entry_form, ekn_lrgy_entry_pw_shell, entryP->pw_shell,
               strlen((char *)entryP->pw_shell), false, &Dummy_st);
    form_write(lrgy_entry_form, ekn_lrgy_entry_cell, entryP->cell,
               strlen((char *)entryP->cell), false, &Dummy_st);

    eku_dec_unix_num(lrgy_entry_form, ekn_lrgy_entry_pw_uid, entryP->pw_uid);
    eku_dec_unix_num(lrgy_entry_form, ekn_lrgy_entry_pw_gid, entryP->pw_gid);

    if (entryP->acct_exp == ek_never_expire) {
        form_write(lrgy_entry_form, ekn_lrgy_entry_acct_exp, ek_null_date,
                    strlen(ek_null_date), false, &Dummy_st);
    } else {
        eku_dec_date_time(lrgy_entry_form, ekn_lrgy_entry_acct_exp,
                            entryP->acct_exp);
    }
    if (entryP->passwd_exp == ek_never_expire) {
        form_write(lrgy_entry_form, ekn_lrgy_entry_passwd_exp, ek_null_date,
                    strlen(ek_null_date), false, &Dummy_st);
    } else {
        eku_dec_date_time(lrgy_entry_form, ekn_lrgy_entry_passwd_exp,
                            entryP->passwd_exp);
    }
    if (entryP->identity_exp == ek_never_expire) {
        form_write(lrgy_entry_form, ekn_lrgy_entry_identity_exp, ek_null_date,
                    strlen(ek_null_date), false, &Dummy_st);
    } else {
        eku_dec_date_time(lrgy_entry_form, ekn_lrgy_entry_identity_exp,
                            entryP->identity_exp);
    }
    eku_dec_date_time(lrgy_entry_form, ekn_lrgy_entry_pw_change, entryP->pw_change);
    if (entryP->acct_exp == ek_never_expire) {
        form_write(lrgy_entry_form, ekn_lrgy_entry_passwd_exp, ek_null_date,
                    strlen(ek_null_date), false, &Dummy_st);
    } else {
        eku_dec_date_time(lrgy_entry_form, ekn_lrgy_entry_pw_expire,
                            entryP->pw_expire);
    }
    eku_dec_date_time(lrgy_entry_form, ekn_lrgy_entry_timestamp, *timestampP);

    if (entryP->passwd_valid) {
        form_write(lrgy_entry_form, ekn_lrgy_entry_passwd_valid, "VALID", 5,
                    false, &Dummy_st);
    } else {
        form_write(lrgy_entry_form, ekn_lrgy_entry_passwd_valid, "NOT VALID",
                    9, false, &Dummy_st);
    }
}


/*
 * FUNCTION ek_lrgy_entry_get
 */
PRIVATE error_status_t ek_lrgy_entry_get
#ifndef __STDC__
    ( lrgy_entry_form )
    form_t *lrgy_entry_form;
#else
  (
    form_t *lrgy_entry_form
  )
#endif
{
    lrgy_entry_header_t header;
    sec_lrgy_entry_t    entry;
    sec_timeval_sec_t   timestamp;
    error_status_t      st;

    form_read_header(lrgy_entry_form,  (Pointer)&header);

    /* do the lookup */
    sec_lrgy_entry_get_next(header.cursor, &entry, &timestamp, &st);
    if (GOOD_STATUS(&st)) {
        lrgy_entry_query_unconvert(lrgy_entry_form, &entry, &timestamp);
        form_write_header(lrgy_entry_form, (Pointer)&header);
    }
    return st;
}


/*
 * FUNCTION ek_lrgy_entry_release_cursor
 */
PUBLIC void ek_lrgy_entry_release_cursor
#ifndef __STDC__
    ( op_handle, modified )
    ek_op_handle_t      op_handle;
    boolean32           *modified;
#else
  (
    ek_op_handle_t      op_handle,
    boolean32           *modified
  )
#endif
{
    lrgy_entry_header_t header;
    ek_op_t             *op;
    error_status_t      lst;

    op = (ek_op_t *)op_handle;
    form_read_header(op->op_form,  (Pointer)&header);
    sec_lrgy_entry_cursor_release(&(header.cursor), modified, &lst);
    form_write_header(op->op_form,  (Pointer)&header);
}


/********************************************************************************
 * Cell Operations
 ********************************************************************************/

/*
 * FUNCTION cell_my_passwd_validate
 */
PRIVATE error_status_t cell_my_passwd_validate
#ifndef __STDC__
    ( form, passwd )
    form_t    *form;
    char      *passwd;
#else
  (
    form_t    *form,
    char      *passwd
  )
#endif
{
    cell_header_t   header;
    error_status_t  st;

    CLEAR_STATUS(&st);
    form_read_header(form,  (Pointer)&header);

    strcpy((char *) header.my_plain_passwd, passwd);

    header.my_passwd_written = true;
    form_write_header(form,  (Pointer)&header);

    return st;
}


/*
 * FUNCTION cell_foreign_passwd_validate
 */
PRIVATE error_status_t cell_foreign_passwd_validate
#ifndef __STDC__
    ( form, passwd )
    form_t    *form;
    char      *passwd;
#else
  (
    form_t    *form,
    char      *passwd
  )
#endif
{
    cell_header_t   header;
    error_status_t  st;

    CLEAR_STATUS(&st);
    form_read_header(form,  (Pointer)&header);

    strcpy((char *) header.foreign_passwd, passwd);

    form_write_header(form,  (Pointer)&header);
    return st;
}


/*
 * FUNCTION cell_foreign_name_validate
 */
PRIVATE error_status_t cell_foreign_name_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    cell_header_t   header;
    error_status_t  st;

    CLEAR_STATUS(&st);
    form_read_header(form,  (Pointer)&header);

    /* Make sure it uses an absolute pathname to login to the foreign cell */
    if (*name != '/') {
        /* Prepend the cellname */
        sprintf((char *) header.foreign_name, "%s/%s",
                (char *) header.item.fullname, name);
    } else {
        strcpy((char *) header.foreign_name, name);
    }

    form_write_header(form,  (Pointer)&header);
    return st;
}


/*
 * FUNCTION cell_fullname_validate
 * The only restriction on fullnames is length.
 */
PRIVATE error_status_t cell_fullname_validate
#ifndef __STDC__
    ( form, fullname )
    form_t    *form;
    char      *fullname;
#else
  (
    form_t    *form,
    char      *fullname
  )
#endif
{
    cell_header_t    header;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    if (strlen((char *)fullname) > ek_lim_max_fullname_len) {
        SET_STATUS(&st, ek_err_data_too_long);
        return st;
    }

    if (GOOD_STATUS(&st)) {
        form_read_header(form,  (Pointer)&header);
        strcpy((char *)(header.item.fullname), fullname);
        form_write_header(form,  (Pointer)&header);
    }
    return st;
}


/* FUNCTION cell_unix_num_validate
 * returns:
 *    ek_err_bad_unix_num    - if the unix number fails basic domain/range tests
 *    ek_err_unix_num_exists - if the unix num is already defined (groups and
 *                             orgs only)
 *    any status values related to communication or server failures.
 */
PRIVATE error_status_t cell_unix_num_validate
#ifndef __STDC__
    ( cell_form, unum_str, local )
    form_t      *cell_form;
    char        *unum_str;
    boolean32   local;
#else
  (
    form_t      *cell_form,
    char        *unum_str,
    boolean32   local
  )
#endif
{
    error_status_t    st;
    uuid_t            dummy_uuid;
    signed32          unum;
    cell_header_t     header;
    ek_op_type_t      op_type;

    SET_STATUS(&st, error_status_ok);

    form_read_header(cell_form,  (Pointer)&header);
    op_type = header.op_type;

          /* unix_num field isn't used by delete operations */
    if (op_type == ek_op_delete) {
        SET_STATUS(&st, ek_kernel_failure);
        return st;
    }

    /* perform basic validation for query/add/change operations in all
     * domains.
     */
    if (!eku_chk_unix_num(unum_str, strlen(unum_str), &unum)) {
        SET_STATUS(&st, ek_err_bad_unix_num);
        return st;
    }

    /* Simple check if local, to make sure unix num doesn't exist */
    if (local) {
        sec_rgy_pgo_unix_num_to_id(context, sec_rgy_domain_person, unum,
                                    &dummy_uuid, &st);
        /* should have gotten "object not found" */
        if (STATUS_EQUAL(&st, sec_rgy_object_not_found))
            SET_STATUS(&st, error_status_ok);
        else if (GOOD_STATUS(&st))
            SET_STATUS(&st, ek_err_unix_num_exists);
        else
            eku_translate_status(&st);    /* just report anything else */
    }

    if (GOOD_STATUS(&st)) {
            /* store the unix num now while we have it */
        if (local)
            header.item.unix_num = unum;
        else
            header.foreign_item.unix_num = unum;
        form_write_header(cell_form,  (Pointer)&header);
    }

    return st;
}


PRIVATE error_status_t cell_lunix_num_validate
#ifndef __STDC__
    ( cell_form, unum_str )
    form_t    *cell_form;
    char      *unum_str;
#else
  (
    form_t    *cell_form,
    char      *unum_str
  )
#endif
{
    return(cell_unix_num_validate(cell_form, unum_str, true));
}


PRIVATE error_status_t cell_funix_num_validate
#ifndef __STDC__
    ( cell_form, unum_str )
    form_t    *cell_form;
    char      *unum_str;
#else
  (
    form_t    *cell_form,
    char      *unum_str
  )
#endif
{
    return(cell_unix_num_validate(cell_form, unum_str, false));
}


/*
 * cell_name_validate
 */
PRIVATE error_status_t cell_name_validate
#ifndef __STDC__
    ( form, name, domain, local )
    form_t              *form;
    char                *name;
    sec_rgy_domain_t    domain;
    boolean32           local;
#else
  (
    form_t              *form,
    char                *name,
    sec_rgy_domain_t    domain,
    boolean32           local
  )
#endif
{
    cell_header_t         header;
    error_status_t        st;

    st =  eku_chk_name(name, domain);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);
    switch (domain) {
    case sec_rgy_domain_person:
        strcpy((char *)header.key.pname, name);
        break;
    case sec_rgy_domain_group:
        if (local) {
            st = chk_pgo_exists(name, domain);
            if (GOOD_STATUS(&st))
                strcpy((char *)header.key.gname, name);
        } else
            strcpy((char *)header.foreign_key.gname, name);
        break;
    case sec_rgy_domain_org:
        if (local) {
            st = chk_pgo_exists(name, domain);
            if (GOOD_STATUS(&st))
                strcpy((char *)header.key.oname, name);
        } else
            strcpy((char *)header.foreign_key.oname, name);
        break;
    }

    if (GOOD_STATUS(&st)) {
        form_write_header(form,  (Pointer)&header);
    }

    return st;
}


PRIVATE error_status_t cell_pname_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    return(cell_name_validate(form, name, sec_rgy_domain_person, false));
}


PRIVATE error_status_t cell_lgname_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    return(cell_name_validate(form, name, sec_rgy_domain_group, true));
}


PRIVATE error_status_t cell_fgname_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    return(cell_name_validate(form, name, sec_rgy_domain_group, false));
}


PRIVATE error_status_t cell_loname_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    return(cell_name_validate(form, name, sec_rgy_domain_org, true));
}


PRIVATE error_status_t cell_foname_validate
#ifndef __STDC__
    ( form, name )
    form_t    *form;
    char      *name;
#else
  (
    form_t    *form,
    char      *name
  )
#endif
{
    return(cell_name_validate(form, name, sec_rgy_domain_org, false));
}


PRIVATE error_status_t cell_exp_date_validate
#ifndef __STDC__
    ( form, date )
    form_t    *form;
    char      *date;
#else
  (
    form_t    *form,
    char      *date
  )
#endif
{
    cell_header_t        header;
    sec_timeval_sec_t    clockh;
    error_status_t       st;
    CLEAR_STATUS(&st);

    st = eku_chk_date(date, strlen(date), &clockh);
    if (BAD_STATUS(&st)) return st;

    form_read_header(form,  (Pointer)&header);
    header.admin_part_written = true;
    header.admin_part.expiration_date = clockh;
    form_write_header(form,  (Pointer)&header);

    return st;
}


PRIVATE void init_cell_header
#ifndef __STDC__
    ( header, op_type )
    cell_header_t  *header;
    ek_op_type_t   op_type;
#else
  (
    cell_header_t  *header,
    ek_op_type_t   op_type
  )
#endif
{
    time_t  tloc;
    header->op_type = op_type;

    header->item.quota = header->foreign_item.quota = 0;
    header->item.flags = header->foreign_item.flags = sec_rgy_pgo_flags_none;

    header->admin_part_written = false;
    header->admin_part.flags = SEC_DEF_FLGS_ACCT_ADMIN_CELL;
    header->admin_part.authentication_flags = SEC_DEF_FLGS_ACCT_AUTH_CELL;
    time(&tloc);
    header->admin_part.good_since_date = (sec_timeval_sec_t) tloc;
    header->user_part_written = false;
    *header->user_part.gecos = '\0';
    *header->user_part.homedir = '\0';
    *header->user_part.shell = '\0';
    header->user_part.flags = SEC_DEF_FLGS_ACCT_USER_CELL;

    header->passwd_written = false;
    header->my_passwd_written = false;
    header->passwd.pepper = NULL;
    header->passwd.version_number = sec_passwd_c_version_none;
    header->my_passwd.pepper = NULL;
    header->my_passwd.version_number = sec_passwd_c_version_none;
    PASSWD_TYPE(&header->passwd) = sec_passwd_plain;
    PASSWD_TYPE(&header->my_passwd) = sec_passwd_plain;
    *(header->plain_passwd) = '\0';
    *(header->my_plain_passwd) = '\0';
}


/*
 * LOCAL FUNCTION clear_cell_form
 */
PRIVATE void clear_cell_form
#ifndef __STDC__
    ( form )
     form_t        *form;
#else
  (
     form_t        *form
  )
#endif
{
    cell_header_t  header;

    form_read_header(form,  (Pointer)&header);
    init_cell_header(&header, header.op_type);
    form_write_header(form,  (Pointer)&header);
    form_clear(form);
}


/*
 * LOCAL FUNCTION get_cell_form
 * allocates and initiializes a form for a cell operation.
 *
 * returns:   ek_null_handle    -   in the stp pointer when a form can't be
 *                                  allocated
 */
PRIVATE void get_cell_form
#ifndef __STDC__
    ( cell_form, op_type, stp )
    form_t          **cell_form;    /* IN - pointer to the operation form */
    ek_op_type_t    op_type;        /* IN - operation type */
    error_status_t  *stp;           /* OUT */
#else
  (
    form_t          **cell_form,    /* IN - pointer to the operation form */
    ek_op_type_t    op_type,        /* IN - operation type */
    error_status_t  *stp
  )
#endif
{
    form_field_offset_t  cur_field;
    form_bitset_t        attributes;
    cell_header_t        cell_header;
    form_t               *new_form;

    SET_STATUS(stp, error_status_ok);

    new_form = form_alloc(ekn_cell_num_fields, sizeof(cell_header_t));

    if (new_form == form_null_form) {
        SET_STATUS(stp, ek_null_handle);
        return;
    } else {
        init_cell_header(&cell_header, op_type);
        form_write_header(new_form,  (Pointer)&cell_header);
    }

        /* all the fields are writable and required */
    attributes = (0 | attribute_readwrite_mask |
                  attribute_required_mask);
    form_init_field(new_form, ekn_cell_pname, attributes,
                    cell_pname_validate);
    form_init_field(new_form, ekn_cell_fullname, attributes,
                    cell_fullname_validate);
    form_init_field(new_form, ekn_cell_lunix_num, attributes,
                    (form_validate_t)cell_lunix_num_validate);
    form_init_field(new_form, ekn_cell_funix_num, attributes,
                    (form_validate_t)cell_funix_num_validate);
    form_init_field(new_form, ekn_cell_lgname, attributes,
                    (form_validate_t)cell_lgname_validate);
    form_init_field(new_form, ekn_cell_fgname, attributes,
                    (form_validate_t)cell_fgname_validate);
    form_init_field(new_form, ekn_cell_loname, attributes,
                    (form_validate_t)cell_loname_validate);
    form_init_field(new_form, ekn_cell_foname, attributes,
                    (form_validate_t)cell_foname_validate);
    form_init_field(new_form, ekn_cell_my_plain_passwd, attributes,
                    (form_validate_t)cell_my_passwd_validate);
    form_init_field(new_form, ekn_cell_admin_exp_date, attributes,
                    (form_validate_t)cell_exp_date_validate);
    form_init_field(new_form, ekn_cell_foreign_name, attributes,
                    (form_validate_t)cell_foreign_name_validate);
    form_init_field(new_form, ekn_cell_foreign_passwd, attributes,
                    (form_validate_t)cell_foreign_passwd_validate);

    if (GOOD_STATUS(stp))
        *cell_form = new_form;
}


PRIVATE error_status_t add_new_cell_acct
#ifndef __STDC__
    ( rgyhandle, key, user_part, admin_part, my_passwd, passwd, new_key_type )
    sec_rgy_handle_t      rgyhandle;
    sec_rgy_login_name_t  *key;
    sec_rgy_acct_user_t   *user_part;
    sec_rgy_acct_admin_t  *admin_part;
    sec_passwd_rec_t      *my_passwd;
    sec_passwd_rec_t      *passwd;
    sec_passwd_type_t     new_key_type;
#else
  (
    sec_rgy_handle_t      rgyhandle,
    sec_rgy_login_name_t  *key,
    sec_rgy_acct_user_t   *user_part,
    sec_rgy_acct_admin_t  *admin_part,
    sec_passwd_rec_t      *my_passwd,
    sec_passwd_rec_t      *passwd,
    sec_passwd_type_t     new_key_type
  )
#endif
{
    sec_rgy_acct_key_t    abbrev = sec_rgy_acct_key_person;  /* only thing that works any more */
    error_status_t        st, undo_st;
    boolean32             undo_group_member_add, undo_org_member_add;
    sec_passwd_version_t  passwd_version_number;

    CLEAR_STATUS(&st);

        /* add person to group/org if necessary */
    undo_group_member_add = force_group_org_membership(rgyhandle, (char *) key->pname,
                                                       (char *)key->gname,
                                                       sec_rgy_domain_group, &st);
    if (BAD_STATUS(&st)) {
        return st;
    } else if (GOOD_STATUS(&st)) {
        undo_org_member_add = force_group_org_membership(rgyhandle, (char *) key->pname,
                                                         (char *)key->oname,
                                                         sec_rgy_domain_org, &st);
    }

    if (GOOD_STATUS(&st)) {
        sec_rgy_acct_add(rgyhandle, key, &abbrev, user_part, admin_part,
                            my_passwd, passwd, new_key_type,
                            &passwd_version_number, &st);
    }
    if (BAD_STATUS(&st)) {
           /* undo the group/org member adds if necessary */
        if (undo_group_member_add) {
            sec_rgy_pgo_delete_member(rgyhandle, sec_rgy_domain_group, key->gname,
                                  key->pname, &undo_st);
            if (BAD_STATUS(&undo_st)) {
                SET_STATUS(&st, ek_misc_acct_partial_op);
                return st;
            }
        }

        if (undo_org_member_add) {
            sec_rgy_pgo_delete_member(rgyhandle, sec_rgy_domain_org, key->oname,
                                  key->pname, &undo_st);
            if (BAD_STATUS(&undo_st)) {
                SET_STATUS(&st, ek_misc_acct_partial_op);
                return st;
            }
        }
    }
    return st;
}


PRIVATE error_status_t replace_cell_acct
#ifndef __STDC__
    ( rgy_h, key_p, user_part, admin_part, my_passwd, passwd_p, new_keytype )
    sec_rgy_handle_t            rgy_h;
    sec_rgy_login_name_t        *key_p;
    sec_rgy_acct_user_t         *user_part;
    sec_rgy_acct_admin_t        *admin_part;
    sec_passwd_rec_t            *my_passwd;
    sec_passwd_rec_t            *passwd_p;
    sec_passwd_type_t           new_keytype;
#else
  (
    sec_rgy_handle_t            rgy_h,
    sec_rgy_login_name_t        *key_p,
    sec_rgy_acct_user_t         *user_part,
    sec_rgy_acct_admin_t        *admin_part,
    sec_passwd_rec_t            *my_passwd,
    sec_passwd_rec_t            *passwd_p,
    sec_passwd_type_t           new_keytype
  )
#endif
{
    error_status_t              st, undo_st;
    sec_rgy_cursor_t            marker;
    sec_passwd_version_t        new_key_version;
    sec_rgy_acct_key_t          new_key_parts;
    sec_rgy_login_name_t        tmp_key, old_key;
    sec_rgy_acct_key_t          abbrev;
    boolean32                   undo_group_member_add,
                                undo_org_member_add;
    CLEAR_STATUS(&st);

        /* Temporary key to lookup the old acct with */
    strcpy((char *)tmp_key.pname, (char *)key_p->pname);
    tmp_key.gname[0] = tmp_key.oname[0] = '\0';

    sec_rgy_cursor_reset(&marker);
    sec_rgy_acct_lookup(rgy_h, &tmp_key, &marker, &old_key,
                        NULL, NULL, NULL, NULL, NULL, &st);

    /* if group or org is different, then rename old acct */
    if ((GOOD_STATUS(&st)) &&
        (strcmp((char *)key_p->gname, (char *)old_key.gname) ||
         strcmp((char *)key_p->gname, (char *)old_key.gname)) ) {

        /* add person to group/org if necessary */
        undo_group_member_add = force_group_org_membership(rgy_h,
                                                        (char *)key_p->pname,
                                                        (char *)key_p->gname,
                                                        sec_rgy_domain_group,
                                                        &st);
        if (BAD_STATUS(&st)) {
            return st;
        }
        undo_org_member_add = force_group_org_membership(rgy_h,
                                                        (char *)key_p->pname,
                                                        (char *)key_p->oname,
                                                        sec_rgy_domain_org,
                                                        &st);
        abbrev = sec_rgy_acct_key_person;
        if (GOOD_STATUS(&st))
            sec_rgy_acct_rename(rgy_h, &old_key, key_p, &abbrev, &st);
        if (BAD_STATUS(&st)) {
            if (undo_group_member_add) {
                sec_rgy_pgo_delete_member(rgy_h, sec_rgy_domain_group,
                                            key_p->gname, key_p->pname,
                                            &undo_st);
            }
            if (undo_org_member_add) {
                sec_rgy_pgo_delete_member(rgy_h, sec_rgy_domain_org,
                                            key_p->oname, key_p->pname,
                                            &undo_st);
            }
        }
    }
    if (GOOD_STATUS(&st)) {
        sec_rgy_acct_user_replace(rgy_h, key_p, user_part, true,
                                    my_passwd, passwd_p,
                                    new_keytype, &new_key_version, &st);
        if (GOOD_STATUS(&st)) {
            abbrev = sec_rgy_acct_key_person;
            sec_rgy_acct_admin_replace(rgy_h, key_p, &abbrev,
                                        admin_part, &st);
        }
    }
    return st;
}


/*
 * FUNCTION ek_cell_add
 */
PRIVATE error_status_t ek_cell_add
#ifndef __STDC__
    ( cell_form )
    form_t  *cell_form;
#else
  (
    form_t  *cell_form
  )
#endif
{
    cell_header_t               header;
    error_status_t              st, local_st;
    sec_rgy_plcy_t              policy;
    sec_rgy_bind_auth_info_t    auth_info;
    sec_rgy_handle_t            foreign_h;
    boolean32                   foreign_h_valid = false;
    sec_rgy_properties_t        properties;
    sec_login_handle_t          lcontext;
    boolean32                   lcontext_valid = false, reset_passwd;
    sec_login_auth_src_t        auth_src;
    sec_passwd_rec_t            my_passwd, passwd, *passwd_p;
    sec_passwd_str_t            tmp_passwd;

    CLEAR_STATUS(&st);
    form_read_header(cell_form,  (Pointer)&header);

        /* The principal for the foreign cell must exist in the local
         * cell's security database for sec_login to work, so do an
         * unauthenticated bind to the foreign cell, get the uuid
         * from its properties record and create the local principal.
         */
    auth_info.info_type = sec_rgy_bind_auth_none;
    sec_rgy_site_bind_update(header.item.fullname, &auth_info, &foreign_h, &st);
    if (GOOD_STATUS(&st)) {
        sec_rgy_properties_get_info(foreign_h, &properties, &st);
        if (GOOD_STATUS(&st)) {
            header.item.id = properties.realm_uuid;
            /* Add foreign principal to the local cell */
            sec_rgy_pgo_add(context, sec_rgy_domain_person, header.key.pname,
                            &header.item, &st);
            if (STATUS_EQUAL(&st, sec_rgy_name_exists)) {
                CLEAR_STATUS(&st);
            }
        }
        sec_rgy_site_close(foreign_h, &local_st);
    }

        /* Get the uuid of the local cell */
    if (GOOD_STATUS(&st)) {
        sec_rgy_properties_get_info (context, &properties, &st);
        if (GOOD_STATUS(&st)) {
            header.foreign_item.id = properties.realm_uuid;
            strcpy((char *) header.foreign_item.fullname,
                    (char *) properties.realm);
	}
    }

        /* prepare for the acct adds */
    if (GOOD_STATUS(&st)) {
        sprintf((char *)header.foreign_key.pname, "krbtgt/%s",
                (char *)(properties.realm + GLOBAL_DIR_ROOT_LEN));

        /* Generate a random password to be used for both cell accounts */
        header.passwd.key.key_type = sec_passwd_des;
        sec_key_mgmt_gen_rand_key(rpc_c_authn_dce_private,
                                    (void *)NULL,
                                    header.foreign_key.pname,
                                    &header.passwd.key.key_type,
                                    sec_passwd_c_version_none,
                                    (void **) &passwd_p,
                                    &st);

        header.my_passwd.key.key_type = sec_passwd_plain;
        header.passwd.key.tagged_union.plain = &(header.plain_passwd[0]);

            /* Add foreign acct to local cell */
        if (GOOD_STATUS(&st)) {
	    /* for bootstrapping, temporarily enable "foreign" logins */
	    header.admin_part.flags |= sec_rgy_acct_admin_valid;

            header.my_passwd.key.tagged_union.plain =
                                                &(header.my_plain_passwd[0]);
            st = add_new_cell_acct(context, &header.key, &(header.user_part),
                                    &(header.admin_part), &header.my_passwd,
                                    passwd_p, sec_passwd_des);
                /* If it exists, then replace it */
            if (STATUS_EQUAL(&st, sec_rgy_object_exists)) {
                st = replace_cell_acct(context, &header.key, &header.user_part,
                                        &header.admin_part, &header.my_passwd,
                                        passwd_p, sec_passwd_des);
            }
        }
    }

    if (GOOD_STATUS(&st)) {
	sec_rgy_acct_key_t abbrev = sec_rgy_acct_key_person;

        passwd.version_number = sec_passwd_c_version_none;
        passwd.pepper = NULL;
        passwd.key.key_type = sec_passwd_plain;

	/* password gets cleared, so make a copy; it's needed again later */
        strncpy((char *)tmp_passwd, (char *)header.foreign_passwd,
                sec_passwd_str_max_len);
        tmp_passwd[sec_passwd_str_max_len] = '\0';
        passwd.key.tagged_union.plain = &(tmp_passwd[0]);

        if (sec_login_setup_identity(header.foreign_name, sec_login_no_flags,
                                                             &lcontext, &st)) {
            if (sec_login_validate_identity(lcontext, &passwd,
                                            &reset_passwd, &auth_src, &st)) {
                if (auth_src != sec_login_auth_src_network) {
                    printf("Unable to obtain network credentials!\n");
                    return;
                }

                lcontext_valid = true;

                if (GOOD_STATUS(&st)) {
                    /* Do an authenticated bind to the foreign cell */
                    auth_info.info_type = sec_rgy_bind_auth_dce;
                    auth_info.tagged_union.dce_info.identity = lcontext;
                    auth_info.tagged_union.dce_info.authn_level =
                                            rpc_c_authn_level_pkt_integrity;
                    auth_info.tagged_union.dce_info.authn_svc =
                                            rpc_c_authn_dce_private;
                    auth_info.tagged_union.dce_info.authz_svc =
                                            rpc_c_authz_dce;
                    sec_rgy_site_bind_update(header.item.fullname, &auth_info,
                                            &foreign_h, &st);
                    if (GOOD_STATUS(&st)) {
                        foreign_h_valid = true;
                    }
                }
            }
        }

	/* re-disable "foreign" logins */
	header.admin_part.flags &= ~sec_rgy_acct_admin_valid;
	sec_rgy_acct_admin_replace(context, &header.key, &abbrev,
					&header.admin_part, &local_st);
	if (BAD_STATUS(&local_st)) {
	    printf("Warning: foreign cell account left valid\n");
	}
    }

        /* do the pgo add to the foreign secd */
    if (GOOD_STATUS(&st)) {
            /* Add the local principal to the foreign cell */
        sec_rgy_pgo_add(foreign_h, sec_rgy_domain_person,
                        header.foreign_key.pname,
                        &header.foreign_item, &st);
        if (STATUS_EQUAL(&st, sec_rgy_name_exists)) {
            CLEAR_STATUS(&st);
        }
    }

    if (GOOD_STATUS(&st)) {
        passwd.key.tagged_union.plain = header.foreign_passwd;

            /* Add local acct to foreign cell */
        st = add_new_cell_acct(foreign_h, &header.foreign_key,
                                &(header.user_part), &(header.admin_part),
                                &passwd, passwd_p, sec_passwd_des);
            /* If it exists, then replace it */
        if (STATUS_EQUAL(&st, sec_rgy_object_exists)) {
            st = replace_cell_acct(foreign_h, &header.foreign_key,
                                    &header.user_part, &header.admin_part,
                                    &passwd, passwd_p, sec_passwd_des);
        }
    }

    if (lcontext_valid) {
        sec_login_purge_context(&lcontext, &local_st);
    }
    if (foreign_h_valid) {
        sec_rgy_site_close(foreign_h, &local_st);
    }

    if (BAD_STATUS(&st))
        eku_translate_status(&st);
    else {
        form_write_header(cell_form,  (Pointer)&header);
    }

    return st;
}


/*
 * LOCAL FUNCTION clear_acct_form
 */
PRIVATE void clear_acct_form
#ifndef __STDC__
    ( form )
     form_t        *form;
#else
  (
     form_t        *form
  )
#endif
{
    acct_header_t  header;

    form_read_header(form,  (Pointer)&header);
    init_acct_header(&header, header.op_type);
    form_write_header(form,  (Pointer)&header);
    form_clear(form);
}


/* FUNCTION ek_acct_get
 *
 * Performs a sec_rgy_acct_lookup using the login name in the ekn_acct_key
 * field.  Writes the query results to the acct query forms fields.
 */
PRIVATE error_status_t ek_acct_get
#ifndef __STDC__
    ( acct_form )
    form_t *acct_form;
#else
  (
    form_t *acct_form
  )
#endif
{
    acct_header_t header;

    sec_rgy_login_name_t  login_name;
    sec_rgy_sid_t         rgy_sid;
    sec_rgy_unix_sid_t    unix_sid;
    sec_rgy_plcy_auth_t   *policy_p;
    error_status_t        st, tmp_st;

    form_read_header(acct_form,  (Pointer)&header);

    /* do the lookup */
    sec_rgy_acct_lookup(context, &(header.key), &(header.marker), &login_name, &rgy_sid,
                     &unix_sid, &(header.key_parts), &(header.user_part),
                     &(header.admin_part), &st);
    if (GOOD_STATUS(&st)) {
        sec_rgy_auth_plcy_get_info(context,&login_name,&header.policy,&tmp_st);
        if (! GOOD_STATUS(&tmp_st)) {
            policy_p = NULL;
            header.policy_written = false;
        } else {
            policy_p = &(header.policy);
            header.policy_written = true;
        }
    }

    if (GOOD_STATUS(&st)) {
        acct_query_unconvert(acct_form, &login_name,
                             header.key_parts,
                             &rgy_sid, &unix_sid, &(header.user_part),
                             &(header.admin_part), policy_p);
        form_write_header(acct_form,  (Pointer)&header);
    } else
        eku_translate_status(&st);

    return st;
}


PRIVATE error_status_t add_new_acct
#ifndef __STDC__
    ( key, abbrev, user_part, admin_part, my_passwd, passwd, new_key_type,
        passwd_version_number)
    sec_rgy_login_name_t  *key;
    sec_rgy_acct_key_t    *abbrev;
    sec_rgy_acct_user_t   *user_part;
    sec_rgy_acct_admin_t  *admin_part;
    sec_passwd_rec_t      *my_passwd;
    sec_passwd_rec_t      *passwd;
    sec_passwd_type_t     new_key_type;
    sec_passwd_version_t  *passwd_version_number;  /* [out] */
#else
  (
    sec_rgy_login_name_t  *key,
    sec_rgy_acct_key_t    *abbrev,
    sec_rgy_acct_user_t   *user_part,
    sec_rgy_acct_admin_t  *admin_part,
    sec_passwd_rec_t      *my_passwd,
    sec_passwd_rec_t      *passwd,
    sec_passwd_type_t     new_key_type,
    sec_passwd_version_t  *passwd_version_number
  )
#endif
{
    error_status_t        st, undo_st;
    boolean32             undo_group_member_add,
                          undo_org_member_add;
    CLEAR_STATUS(&st);

        /* add person to group/org if necessary */
    undo_group_member_add = force_group_org_membership(context,
                                (char *) key->pname, (char *)key->gname,
                                sec_rgy_domain_group, &st);
    if (BAD_STATUS(&st)) {
        return st;
    } else if (GOOD_STATUS(&st)) {
        undo_org_member_add = force_group_org_membership(context,
                                (char *) key->pname, (char *)key->oname,
                                sec_rgy_domain_org, &st);
    }

    if (GOOD_STATUS(&st)) {
        sec_rgy_acct_add(context, key, abbrev, user_part, admin_part,
                            my_passwd, passwd, new_key_type,
                            passwd_version_number, &st);
    }
    if (BAD_STATUS(&st)) {
           /* undo the group/org member adds if necessary */
        if (undo_group_member_add) {
            sec_rgy_pgo_delete_member(context, sec_rgy_domain_group, key->gname,
                                  key->pname, &undo_st);
            if (BAD_STATUS(&undo_st)) {
                SET_STATUS(&st, ek_misc_acct_partial_op);
                return st;
            }
        }

        if (undo_org_member_add) {
            sec_rgy_pgo_delete_member(context, sec_rgy_domain_org, key->oname,
                                  key->pname, &undo_st);
            if (BAD_STATUS(&undo_st)) {
                SET_STATUS(&st, ek_misc_acct_partial_op);
                return st;
            }
        }
    }

    return st;
}


/* FUNCTION ek_acct_add
 *
 * Gets the effective policy for the organization and applies it to the password
 * (if the password is not NULL).   If policy checks out okay, do the add.
 */
PRIVATE error_status_t ek_acct_add
#ifndef __STDC__
    ( acct_form )
    form_t  *acct_form;
#else
  (
    form_t  *acct_form
  )
#endif
{
    acct_header_t          header;
    error_status_t         st;
    sec_rgy_plcy_t         policy;
    ek_plain_passwd_buf_t  passwd;
    long int               passwd_len;

    CLEAR_STATUS(&st);
    form_read_header(acct_form,  (Pointer)&header);

    if (! (header.passwd_written || header.plain_passwd_written)) {
        return ek_required_field;
    }

    /* If not a random password, use the plain password */
    if (!header.passwd_written) {
        form_read(acct_form, ekn_acct_user_plain_passwd, passwd,
                    sizeof(passwd), &passwd_len);
            /* apply effective policy to non-zero length passwords */
        if (passwd_len != 0) {
            sec_rgy_plcy_get_effective(context, header.key.oname, &policy, &st);
            if (GOOD_STATUS(&st)) {
                st = eku_chk_plain_passwd(passwd, passwd_len, policy, false);
            } else {
                eku_translate_status(&st);
                return st;
            }
        }
        if (GOOD_STATUS(&st)) {
            header.passwd.key.tagged_union.plain = &(header.plain_passwd[0]);
        }
    }

        /* do the add */
    if (GOOD_STATUS(&st)) {
        header.my_passwd.key.tagged_union.plain = &(header.my_plain_passwd[0]);
        st = add_new_acct(&header.key, &(header.key_parts), &(header.user_part),
                          &(header.admin_part), &header.my_passwd, &header.passwd,
                          header.new_key_type, &(header.passwd_version_number));
    }

    if (GOOD_STATUS(&st)) {

        /* Add the auth policy data if it was specified */
        if (field_written (acct_form, ekn_acct_max_ticket_lifetime))
            sec_rgy_auth_plcy_set_info(context,&header.key,&header.policy,&st);

        /* pass back the abbreviation in the ekn_acct_abbrev field.  It may have
         * been changed by the server
         */
        eku_dec_acct_key_parts(acct_form, ekn_acct_key_parts, header.key_parts);

        /* pass back the server-assigned password version number */
        eku_dec_int32(acct_form, ekn_acct_user_passwd_version,
                      (signed32) header.passwd_version_number);
    }

    if (BAD_STATUS(&st))
        eku_translate_status(&st);
    else
        form_write_header(acct_form,  (Pointer)&header);

    return st;
}


/* 
 * ek_get_last_acct_chg_status
 */
PUBLIC error_status_t ek_get_last_acct_chg_status
#ifndef __STDC__
    (op_h)
    ek_op_handle_t op_h;
#else
    (ek_op_handle_t op_h)
#endif
{
    acct_header_t           header;
    ek_op_t                 *op;
    op = (ek_op_t *) op_h;
    form_read_header(op->op_form,  (Pointer)&header);
    return header.last_status;
}


/* FUNCTION ek_acct_change
 *
 * Gets the effective policy for the organization and applies it to the
 * password.  If policy checks out okay, do the change.
 */
PRIVATE error_status_t ek_acct_change
#ifndef __STDC__
    ( acct_form )
    form_t  *acct_form;
#else
  (
    form_t  *acct_form
  )
#endif
{
    acct_header_t           header;
    sec_rgy_plcy_t          policy;
    long int                passwd_len;
    sec_rgy_login_name_t    name_result, *lname;
    sec_rgy_acct_user_t     user_part;
    sec_rgy_acct_admin_t    admin_part;
    sec_rgy_cursor_t        marker;
    sec_rgy_sid_t           sid;
    sec_rgy_unix_sid_t      unix_sid;
    sec_rgy_acct_key_t      key_parts;
    error_status_t          undo_st, st;
    boolean32               undo_group_member_add = false,
                            undo_org_member_add = false, set_passwd;
    sec_rgy_acct_key_t      new_key_parts;

#define CLEANUP() {\
    eku_translate_status(&st); \
    header.last_status = st; \
    form_write_header(acct_form, (Pointer)&header); \
    if (undo_group_member_add) \
        sec_rgy_pgo_delete_member(context, sec_rgy_domain_group,\
            header.new_key.gname, header.new_key.pname, &undo_st);\
    if (undo_org_member_add) \
        sec_rgy_pgo_delete_member(context, sec_rgy_domain_org, \
            header.new_key.oname, header.new_key.pname, &undo_st); \
  }

    CLEAR_STATUS(&st);
    form_read_header(acct_form,  (Pointer)&header);
    header.last_status = error_status_ok;
    form_write_header(acct_form, (Pointer)&header);

        /* if no new data has been written, don't bother doing anything */
    if (!(header.user_part_written || header.admin_part_written ||
            (ACCT_NEW_KEY_WRITTEN(header)) || header.passwd_written ||
            header.policy_written || header.plain_passwd_written))
        return st;
    lname = &header.key;

    /* Attempt key changes first - so we don't change any non-key data
     * only to have the key change fail
     */
    if (ACCT_NEW_KEY_WRITTEN(header)) {

        /* do a lookup on the acct key */
        sec_rgy_cursor_reset(&marker);

        /*
         * any or all of the pname, gname, or oname components of the new account key may
         * have been written, so we need to provide the old key values for any that
         * weren't (otherwise, we chance doing a wildcarded lookup, or rename, and we
         *  certainly don't want to do that).
         */
        if (!header.new_pname_key_written) {
            strcpy( (char *) header.new_key.pname, (char *) header.key.pname);
        }
        if (!header.new_gname_key_written) {
            strcpy( (char *) header.new_key.gname, (char *) header.key.gname);
        }
        if (!header.new_oname_key_written) {
            strcpy( (char *) header.new_key.oname, (char *) header.key.oname);
        }

        sec_rgy_acct_lookup(context, &(header.new_key), &marker, &name_result,
                            &sid, &unix_sid, &key_parts, &user_part,
                            &admin_part, &st);

        /* changed acct name must not already exist */
        if (ACCT_NOT_FOUND(st)) {
            SET_STATUS(&st, error_status_ok);
        } else if (GOOD_STATUS(&st)) {
            SET_STATUS(&st, ek_err_acct_exists);
        }
        if (BAD_STATUS(&st)) {
            CLEANUP();
            return ek_misc_chg_name_fail;
        }

            /* add person to group/org if necessary */
        undo_group_member_add = force_group_org_membership(context,
                                                (char *)header.new_key.pname,
                                                (char *)header.new_key.gname,
                                                sec_rgy_domain_group, &st);
        if (BAD_STATUS(&st)) {
            CLEANUP();
            return ek_misc_chg_gmem_fail;
        }

        undo_org_member_add = force_group_org_membership(context,
                                                (char *)header.new_key.pname,
                                                (char *)header.new_key.oname,
                                                sec_rgy_domain_org, &st);
        if (BAD_STATUS(&st)) {
            CLEANUP();
            return ek_misc_chg_omem_fail;
        }

        new_key_parts = header.key_parts;
        sec_rgy_acct_rename(context, &header.key, &header.new_key,
                            &new_key_parts, &st);

        if (GOOD_STATUS(&st)) {
            /* The server may  have reassigned the shortest unique abbrev.
             * report this back by writing the server assigned value to the
             * ekn_acct_key_parts field
             */
            eku_dec_acct_key_parts(acct_form,ekn_acct_key_parts,new_key_parts);
        } else {
            CLEANUP();
            return ek_misc_chg_name_fail;
        }
        lname = &header.new_key;
    }

#undef CLEANUP
#define CLEANUP() {\
    eku_translate_status(&st); \
    header.last_status = st; \
    form_write_header(acct_form, (Pointer)&header); \
  }

    /* if the plain passwd field's been written, then check it against the
     * effective policy
     */
    if (header.plain_passwd_written) {
        set_passwd = true;
        header.passwd.key.tagged_union.plain = &(header.plain_passwd[0]);
        header.my_passwd.key.tagged_union.plain = &(header.my_plain_passwd[0]);
        passwd_len = u_strlen(PLAIN_PASSWD(&header.passwd));

            /* apply effective policy to non-zero length passwords */
        if (passwd_len != 0) {
            if (ACCT_NEW_KEY_WRITTEN(header)) {
                sec_rgy_plcy_get_effective(context, header.new_key.oname,
                                           &policy, &st);
            } else {
                sec_rgy_plcy_get_effective(context, header.key.oname,
                                           &policy, &st);
            }

            if (GOOD_STATUS(&st)) {
                st = eku_chk_plain_passwd((char *)PLAIN_PASSWD(&header.passwd),
                                          passwd_len, policy, false);
            }

            if (BAD_STATUS(&st)) {
                CLEANUP();
                return ek_misc_chg_passwd_fail;
            }
        }
    } else if (header.passwd_written) {
        set_passwd = true;
        header.my_passwd.key.tagged_union.plain = &(header.my_plain_passwd[0]);
    } else {
        set_passwd = false;
    }

    /* Only replace the admin/user data if it's been written.  replace
     * admin part first so we don't successfully replace user info and then
     * choke on the admin info because the user isn't authorized
     */
    if (header.admin_part_written) {
        if (!ACCT_NEW_KEY_WRITTEN(header)) {
            /* key parts might be changing even though the login name isn't.
             * if so we need to use the key parts field from the header.
             * If the login name ispart of the change, we just use the same
             * value for key_parts that came back in the rename operation
             */
            new_key_parts = header.key_parts;
        }
        sec_rgy_acct_admin_replace(context, lname, &new_key_parts,
                                   &(header.admin_part), &st);

        if (GOOD_STATUS(&st) && !ACCT_NEW_KEY_WRITTEN(header)) {
        /* The server may  have reassigned the shortest unique abbrev. Report
         * this by writing the server assigned value to the key_parts field.
         */
            eku_dec_acct_key_parts(acct_form, ekn_acct_key_parts, new_key_parts);
        }
        if (BAD_STATUS(&st)) {
            CLEANUP();
            return ek_misc_chg_admin_fail;
        }
    }

    if (GOOD_STATUS(&st)) {
        if (header.user_part_written)
            sec_rgy_acct_user_replace(context, lname,
                                        &(header.user_part),  false,
                                        &header.my_passwd, &header.passwd,
                                        header.new_key_type,
                                        &header.passwd_version_number, &st);
        if (BAD_STATUS(&st)) {
            CLEANUP();
            return ek_misc_chg_uinfo_fail;
        }
    }
    if (GOOD_STATUS(&st) && set_passwd) {
        sec_rgy_acct_passwd(context, lname, &header.my_passwd, &header.passwd,
                            header.new_key_type, &header.passwd_version_number,
                            &st);
        if (GOOD_STATUS(&st)) {
            eku_dec_int32(acct_form, ekn_acct_user_passwd_version,
                            (signed32) header.passwd_version_number);
        } else {
            CLEANUP();
            return ek_misc_chg_passwd_fail;
        }
    }

    /* Add the auth policy data if it was specified */
    if ((GOOD_STATUS(&st)) && (header.policy_written) &&
        ((field_written(acct_form, ekn_acct_max_ticket_lifetime)) ||
         (field_written(acct_form, ekn_acct_max_renewable_lifetime))) ) {
        sec_rgy_auth_plcy_set_info(context, lname,
                                    &header.policy, &st);
        if (STATUS_EQUAL(&st, sec_rgy_not_authorized)) {
            CLEANUP();
            return ek_err_auth_plcy_unauth;
        } else if (BAD_STATUS(&st)) {
            CLEANUP();
            return ek_misc_chg_auth_plcy;
        }
    }

    return st;
}


/*
 * FUNCTION ek_acct_delete - deletes an account.
 */
PRIVATE error_status_t ek_acct_delete
#ifndef __STDC__
    ( acct_form )
    form_t  *acct_form;
#else
  (
    form_t  *acct_form
  )
#endif
{
    acct_header_t   header;
    error_status_t  st;

    form_read_header(acct_form,  (Pointer)&header);
    sec_rgy_acct_delete(context, &(header.key), &st);
    if (BAD_STATUS(&st))
        eku_translate_status(&st);

    return st;
}

/********************************************************************************
 ******************************* GENERAL OPERATION ROUTINES *********************
 ********************************************************************************
 * The following routines deal with operations in general (handle allocation and
 * initialization, operation dispatch)
 */

/* to avoid having the interface check status values on every kernel call, the
 * kernel signal(s) kernel failures, and statuses raised by improper use of
 * kernel operations by the interface.
 */

#define SIGNAL_KERNEL_ERROR(status) \
    if (BAD_STATUS(&((status)))) { \
        switch((status)) { \
            case ek_null_handle:    \
            case ek_bad_format:     \
            case ek_required_field: \
            case ek_field_unused:   \
            case ek_field_readonly: \
            case ek_bad_field:      \
            case ek_bad_op_id:      \
            case ek_field_no_data:  \
            case ek_kernel_failure: \
                    fprintf(stderr, "FAULT 0x%x at line %d of file %s\n", status, __LINE__, __FILE__); \
                    ek_fatal((status)); \
                    break; \
            default: \
                    break; \
        } \
    }

PRIVATE void ek_fatal
#ifndef __STDC__
    ( status )
    error_status_t  status;
#else
  (
    error_status_t  status
  )
#endif
{
    exit(status);
}

/*
 * FUNCTION ek_get_op_handle
 *
 * Given an operation identifier, allocates an operation handle for a specified
 * operation.
 *
 * Returns: ek_bad_op_id
 *          ek_null_handle
 */
PUBLIC error_status_t ek_get_op_handle
#ifndef __STDC__
    ( op_handle, operation_id )
     ek_op_handle_t *op_handle;
     ek_op_id_t     operation_id;
#else
  (
     ek_op_handle_t *op_handle,
     ek_op_id_t     operation_id
  )
#endif
{
    error_status_t  st;
    ek_op_t         *new_op;

    SET_STATUS(&st, error_status_ok);

        /* allocate an operation handle */
    new_op = (ek_op_t *) malloc(sizeof(ek_op_t));

    if (new_op == 0)
        SET_STATUS(&st, ek_null_handle);
    else {
        switch (operation_id) {

        case ekn_person_get_by_name:
            get_pgo_name_query_form(&(new_op->op_form), sec_rgy_domain_person, &st);
            break;
        case ekn_person_get_by_num:
            get_pgo_num_query_form(&(new_op->op_form), sec_rgy_domain_person, &st);
            break;
        case ekn_person_add:
            get_pgo_update_form(&(new_op->op_form), sec_rgy_domain_person, 
                                ek_op_add, &st);
            break;
        case ekn_person_change:
            get_pgo_update_form(&(new_op->op_form), sec_rgy_domain_person, 
                                ek_op_change, &st);
            break;
        case ekn_person_delete:
            get_pgo_update_form(&(new_op->op_form), sec_rgy_domain_person, 
                                ek_op_delete, &st);
            break;
        case ekn_person_adopt:
            get_pgo_adopt_form(&(new_op->op_form), sec_rgy_domain_person, &st);
            break;

        case ekn_group_get_by_name:
            get_pgo_name_query_form(&(new_op->op_form), sec_rgy_domain_group, &st);
            break;
        case ekn_group_get_by_num:
            get_pgo_num_query_form(&(new_op->op_form),sec_rgy_domain_group, &st);
            break;
        case ekn_group_add:
            get_pgo_update_form(&(new_op->op_form), sec_rgy_domain_group, 
                                ek_op_add, &st);
            break;
        case ekn_group_change:
            get_pgo_update_form(&(new_op->op_form), sec_rgy_domain_group, 
                                ek_op_change, &st);
            break;
        case ekn_group_delete:
            get_pgo_update_form(&(new_op->op_form), sec_rgy_domain_group, 
                                ek_op_delete, &st);
            break;
        case ekn_group_adopt:
            get_pgo_adopt_form(&(new_op->op_form), sec_rgy_domain_group, &st);
            break;


        case ekn_org_get_by_name:
            get_pgo_name_query_form(&(new_op->op_form), sec_rgy_domain_org, &st);
            break;
        case ekn_org_get_by_num:
            get_pgo_num_query_form(&(new_op->op_form), sec_rgy_domain_org, &st);
            break;
        case ekn_org_add:
            get_pgo_update_form(&(new_op->op_form), sec_rgy_domain_org, 
                                ek_op_add, &st);
            break;
        case ekn_org_change:
            get_pgo_update_form(&(new_op->op_form), sec_rgy_domain_org, 
                                ek_op_change, &st);
            break;
        case ekn_org_delete:
            get_pgo_update_form(&(new_op->op_form), sec_rgy_domain_org, 
                                ek_op_delete, &st);
            break;
        case ekn_org_adopt:
            get_pgo_adopt_form(&(new_op->op_form), sec_rgy_domain_org, &st);
            break;


        case ekn_mem_get_person_groups:
            get_member_query_form(&(new_op->op_form), sec_rgy_domain_person, &st);
            break;
        case ekn_mem_get_group_members:
            get_member_query_form(&(new_op->op_form), sec_rgy_domain_group, &st);
            break;
        case ekn_mem_get_org_members:
            get_member_query_form(&(new_op->op_form), sec_rgy_domain_org, &st);
            break;

        case ekn_mem_add_group_member:
        case ekn_mem_del_group_member:
            get_member_update_form (&(new_op->op_form), sec_rgy_domain_group, &st);
            break;
        case ekn_mem_add_org_member:
        case ekn_mem_del_org_member:
            get_member_update_form (&(new_op->op_form), sec_rgy_domain_org, &st);
            break;

        case ekn_policy_get:
        case ekn_policy_get_effective:
            get_policy_form(&(new_op->op_form), ek_op_query, &st);
            break;
        case ekn_policy_set:
            get_policy_form(&(new_op->op_form), ek_op_change, &st);
            break;

        case ekn_auth_policy_get:
        case ekn_auth_policy_get_effective:
            get_auth_policy_form(&(new_op->op_form), ek_op_query, &st);
            break;
        case ekn_auth_policy_set:
            get_auth_policy_form(&(new_op->op_form), ek_op_change, &st);
            break;

        case ekn_prop_rgy_get:
            ek_get_rgy_prop_form(&(new_op->op_form), ek_op_query, &st);
            break;
        case ekn_prop_rgy_set:
            ek_get_rgy_prop_form(&(new_op->op_form), ek_op_change, &st);
            break;
        case ekn_prop_lrgy_get:
            ek_get_lrgy_prop_form(&(new_op->op_form), ek_op_query, &st);
            break;
        case ekn_prop_lrgy_set:
            ek_get_lrgy_prop_form(&(new_op->op_form), ek_op_change, &st);
            break;
        case ekn_acct_get:
            get_acct_form(&(new_op->op_form), ek_op_query, &st);
            break;
        case ekn_acct_add:
            get_acct_form(&(new_op->op_form), ek_op_add, &st);
            break;
        case ekn_acct_change:
            get_acct_form(&(new_op->op_form), ek_op_change, &st);
            break;
        case ekn_acct_delete:
            get_acct_form(&(new_op->op_form), ek_op_delete, &st);
            break;
        case ekn_lrgy_entry_get:
            get_lrgy_entry_form(&(new_op->op_form), &st);
            break;
        case ekn_cell_add:
            get_cell_form(&(new_op->op_form), ek_op_add, &st);
            break;
        default:
            SET_STATUS(&st, ek_bad_op_id);
            break;
        }
    }

    if (BAD_STATUS(&st)) {
        free( (char *) new_op);
        SIGNAL_KERNEL_ERROR(st);
    } else {
        new_op->op_header.operation = operation_id;
        *op_handle = (ek_op_handle_t)(new_op);
    }

    return st;
}


/*
 * FUNCTION ek_free_op_handle
 * Frees the storage for an operation handle.
 */
PUBLIC void ek_free_op_handle
#ifndef __STDC__
    (op_handle)
    ek_op_handle_t *op_handle;
#else
  (
    ek_op_handle_t *op_handle
  )
#endif
{
    ek_op_t  *free_op;

    free_op = (ek_op_t *)(*op_handle);
    form_release(free_op->op_form);
    free((char *)free_op);
}


/*
 * FUNCTION ek_init_op
 * Clears the data in all fields of an operation form.
 */
PUBLIC error_status_t ek_init_op
#ifndef __STDC__
    ( op_handle )
    ek_op_handle_t op_handle;
#else
  (
    ek_op_handle_t op_handle
  )
#endif
{
    error_status_t st;
    ek_op_t  *clear_op;

    SET_STATUS(&st, error_status_ok);
    clear_op = (ek_op_t *)op_handle;

    switch(clear_op->op_header.operation) {

    case ekn_person_get_by_name:
    case ekn_group_get_by_name:
    case ekn_org_get_by_name:
    case ekn_person_get_by_num:
    case ekn_group_get_by_num:
    case ekn_org_get_by_num:
        clear_pgo_form(clear_op->op_form);
        break;

    case ekn_person_add:
    case ekn_person_change:
    case ekn_person_delete:
    case ekn_person_adopt:
    case ekn_group_add:
    case ekn_group_change:
    case ekn_group_delete:
    case ekn_group_adopt:
    case ekn_org_add:
    case ekn_org_change:
    case ekn_org_delete:
    case ekn_org_adopt:
        clear_pgo_update_form(clear_op->op_form);
        break;

    case ekn_mem_get_person_groups:
    case ekn_mem_get_group_members:
    case ekn_mem_get_org_members:
        clear_member_query_form(clear_op->op_form);
        break;

    case ekn_mem_add_group_member:
    case ekn_mem_add_org_member:
    case ekn_mem_del_group_member:
    case ekn_mem_del_org_member:
        form_clear(clear_op->op_form);

    case ekn_policy_get:
    case ekn_policy_get_effective:
    case ekn_policy_set:
        form_clear(clear_op->op_form);
        break;

    case ekn_auth_policy_get:
    case ekn_auth_policy_get_effective:
        form_clear(clear_op->op_form);
        break;
    case ekn_auth_policy_set:
        clear_auth_policy_form(clear_op->op_form);
        st = ek_auth_policy_get(clear_op->op_form);
        break;

    case ekn_prop_rgy_get:
    case ekn_prop_rgy_set:
        st = ek_init_rgy_prop_form(clear_op->op_form);
        break;
    case ekn_prop_lrgy_get:
    case ekn_prop_lrgy_set:
        st = ek_init_lrgy_prop_form(clear_op->op_form);
        break;
    case ekn_acct_get:
    case ekn_acct_add:
    case ekn_acct_change:
    case ekn_acct_delete:
        clear_acct_form(clear_op->op_form);
        break;
    case ekn_lrgy_entry_get:
        st = clear_lrgy_entry_form(clear_op->op_form);
        break;
    case ekn_cell_add:
        clear_cell_form(clear_op->op_form);
        break;
    default:
        SET_STATUS(&st, ek_bad_op_id);
    }

    SIGNAL_KERNEL_ERROR(st);
    return st;
}


/*
 * FUNCTION ek_do_op
 * Dispatches an operation and it's data to the registry client agent.
 *
 * Returns: ek_bad_op_id
 *          ek_required_field
 *          any status returned by the dispatched operation
 */
PUBLIC error_status_t ek_do_op
#ifndef __STDC__
    ( op_handle )
    ek_op_handle_t  op_handle;
#else
  (
    ek_op_handle_t  op_handle
  )
#endif
{
    error_status_t       st;
    ek_op_t              *op;
    form_field_offset_t  cur_field;
    form_bitset_t        attributes;
    form_field_status_t  field_status;
    long int             num_fields;

    SET_STATUS(&st, error_status_ok);
    op = (ek_op_t *) op_handle;

        /* have all the fields required for the operation been written? */
    form_num_fields(op->op_form, &num_fields);

    for (cur_field = 0; cur_field < num_fields; cur_field++) {

        form_field_attributes(op->op_form, cur_field, &attributes);

        form_field_status(op->op_form, cur_field, &field_status);

        if ((attributes & attribute_required_mask) &&
            (field_status != form_field_valid_data))
            SET_STATUS(&st, ek_required_field);
    }

    if (GOOD_STATUS(&st)) {
        switch (op->op_header.operation) {

        case ekn_person_get_by_name:
            st = ek_get_pgo_by_name(op->op_form);
            break;
        case ekn_person_get_by_num:
            st = ek_get_pgo_by_unix_num(op->op_form);
            break;
        case ekn_person_add:
            st = ek_pgo_add(op->op_form);
            break;
        case ekn_person_change:
            st = ek_pgo_change(op->op_form);
            break;
        case ekn_person_delete:
            st = ek_pgo_delete(op->op_form);
            break;
        case ekn_person_adopt:
            st = ek_pgo_adopt(op->op_form);
            break;
        case ekn_group_get_by_name:
            st = ek_get_pgo_by_name(op->op_form);
            break;
        case ekn_group_get_by_num:
            st = ek_get_pgo_by_unix_num(op->op_form);
            break;
        case ekn_group_add:
            st = ek_pgo_add(op->op_form);
            break;
        case ekn_group_change:
            st = ek_pgo_change(op->op_form);
            break;
        case ekn_group_delete:
            st = ek_pgo_delete(op->op_form);
            break;
        case ekn_group_adopt:
            st = ek_pgo_adopt(op->op_form);
            break;

        case ekn_org_get_by_name:
            st = ek_get_pgo_by_name (op->op_form);
            break;
        case ekn_org_get_by_num:
            st = ek_get_pgo_by_unix_num (op->op_form);
            break;
        case ekn_org_add:
            st = ek_pgo_add(op->op_form);
            break;
        case ekn_org_change:
            st = ek_pgo_change(op->op_form);
            break;
        case ekn_org_delete:
            st = ek_pgo_delete(op->op_form);
            break;
        case ekn_org_adopt:
            st = ek_pgo_adopt(op->op_form);
            break;

        case ekn_mem_get_person_groups:
            st = do_get_members (op->op_form, sec_rgy_domain_person);
            break;
        case ekn_mem_get_group_members:
            st = do_get_members (op->op_form, sec_rgy_domain_group);
            break;
        case ekn_mem_get_org_members:
            st = do_get_members (op->op_form, sec_rgy_domain_org);
            break;
        case ekn_mem_add_group_member:
            st = do_update_member (op->op_form, sec_rgy_domain_group, ek_op_add);
            break;
        case ekn_mem_add_org_member:
            st = do_update_member (op->op_form, sec_rgy_domain_org, ek_op_add);
            break;
        case ekn_mem_del_group_member:
            st = do_update_member (op->op_form, sec_rgy_domain_group, ek_op_delete);
            break;
        case ekn_mem_del_org_member:
            st = do_update_member (op->op_form, sec_rgy_domain_org, ek_op_delete);
            break;

        case ekn_policy_get:
            st = ek_policy_get(op->op_form);
            break;
        case ekn_policy_get_effective:
            st = ek_policy_get_effective(op->op_form);
            break;
        case ekn_policy_set:
            st = ek_policy_set(op->op_form);
            break;

        case ekn_auth_policy_get:
            st = ek_auth_policy_get(op->op_form);
            break;
        case ekn_auth_policy_get_effective:
            st = ek_auth_policy_get_effective(op->op_form);
            break;
        case ekn_auth_policy_set:
            st = ek_auth_policy_set(op->op_form);
            break;

        case ekn_prop_rgy_get:
            st = ek_rgy_prop_get(op->op_form);
            break;
        case ekn_prop_rgy_set:
            st = ek_rgy_prop_set(op->op_form);
            break;
        case ekn_prop_lrgy_get:
            st = ek_lrgy_prop_get(op->op_form);
            break;
        case ekn_prop_lrgy_set:
            st = ek_lrgy_prop_set(op->op_form);
            break;
        case ekn_acct_get:
            st = ek_acct_get(op->op_form);
            break;
        case ekn_acct_add:
            st = ek_acct_add(op->op_form);
            break;
        case ekn_acct_change:
            st = ek_acct_change(op->op_form);
            break;
        case ekn_acct_delete:
            st = ek_acct_delete(op->op_form);
            break;

        case ekn_lrgy_entry_get:
            st = ek_lrgy_entry_get(op->op_form);
            break;

        case ekn_cell_add:
            st = ek_cell_add(op->op_form);
            break;

        default:
            SET_STATUS(&st, ek_bad_op_id);
            break;
        }
    }

    SIGNAL_KERNEL_ERROR(st);
    return st;
}


PUBLIC ek_op_id_t  ek_get_op_id
#ifndef __STDC__
    ( op_handle )
    ek_op_handle_t  op_handle;
#else
  (
    ek_op_handle_t  op_handle
  )
#endif
{
    return ((ek_op_t *)op_handle)->op_header.operation;
}


error_status_t  ek_write_field
#ifndef __STDC__
    ( op_handle, field, data, data_len )
    ek_op_handle_t  op_handle;
    ek_field_id_t   field;
    char            *data;
    long int        data_len;
#else
  (
    ek_op_handle_t  op_handle,
    ek_field_id_t   field,
    char            *data,
    long int        data_len
  )
#endif
{
    ek_op_t         *op;
    form_bitset_t   field_attributes;
    long int        num_fields;
    error_status_t  st;

    op = (ek_op_t *)op_handle;
    SET_STATUS(&st, error_status_ok);

    form_num_fields(op->op_form, &num_fields);
    if (!CHECK_FIELD(field, num_fields)) {
        SET_STATUS(&st, ek_bad_field);
    } else {
        form_field_attributes(op->op_form, field, &field_attributes);
        if (FIELD_UNUSED(field_attributes)) {
            SET_STATUS(&st, ek_field_unused);
        } else {
            if (FIELD_READONLY(field_attributes)) {
                SET_STATUS(&st,  ek_field_readonly);
            }
        }
    }

    if (GOOD_STATUS(&st))
        form_write (op->op_form, (form_field_offset_t)field, data, data_len,
                     true, &st);

    SIGNAL_KERNEL_ERROR(st);
    return st;
}


PUBLIC void ek_read_field
#ifndef __STDC__
    ( op_handle, field_id, user_buff, user_buffsize, data_len )
    ek_op_handle_t  op_handle;
    ek_field_id_t   field_id;
    char            *user_buff;
    long int        user_buffsize;
    long int        *data_len;
#else
  (
    ek_op_handle_t  op_handle,
    ek_field_id_t   field_id,
    char            *user_buff,
    long int        user_buffsize,
    long int        *data_len
  )
#endif
{
    ek_op_t              *op;
    form_bitset_t        field_attributes;
    form_field_status_t  field_status;
    long int             num_fields;
    error_status_t       st;

    SET_STATUS(&st, error_status_ok);
    op = (ek_op_t *)op_handle;

    form_num_fields(op->op_form, &num_fields);
    if (!CHECK_FIELD(field_id, num_fields)) {
        SET_STATUS(&st, ek_bad_field);
    } else {
        form_field_attributes(op->op_form, field_id, &field_attributes);
        if (FIELD_UNUSED(field_attributes)) {
            SET_STATUS(&st,  ek_field_unused);
        } else {
            form_field_status(op->op_form, field_id, &field_status);
            if (field_status == form_field_no_data) {
                SET_STATUS(&st, ek_field_no_data);
            }
        }
    }

    if (GOOD_STATUS(&st)){
        form_read(op->op_form, (form_field_offset_t)field_id, user_buff,
                   user_buffsize, data_len);
    }
    SIGNAL_KERNEL_ERROR(st);
}


PUBLIC boolean32  ek_field_written
#ifndef __STDC__
    ( op_handle, field_id )
    ek_op_handle_t  op_handle;     /* IN - the operation handle */
    ek_field_id_t   field_id;      /* IN - the field id */
#else
  (
    ek_op_handle_t  op_handle,     /* IN - the operation handle */
    ek_field_id_t   field_id
  )
#endif
{
    ek_op_t              *op;
    form_bitset_t        field_attributes;
    form_field_status_t  field_status;
    long int             num_fields;
    boolean32            written;
    error_status_t       st;

    SET_STATUS(&st, error_status_ok);
    written = false;

    op = (ek_op_t *)op_handle;

    form_num_fields(op->op_form, &num_fields);
    if (!CHECK_FIELD(field_id, num_fields)) {
        SET_STATUS(&st, ek_bad_field);
    } else {
        form_field_attributes(op->op_form, field_id, &field_attributes);
        if (FIELD_UNUSED(field_attributes)) {
            SET_STATUS(&st,  ek_field_unused);
        } else {
            form_field_status(op->op_form, field_id, &field_status);
            if (field_status == form_field_valid_data) {
                written = true;
            }
        }
    }
    SIGNAL_KERNEL_ERROR(st);

    return written;
}


PUBLIC boolean32  ek_field_has_data
#ifndef __STDC__
    ( op_handle, field_id )
    ek_op_handle_t  op_handle;     /* IN - the operation handle */
    ek_field_id_t   field_id;      /* IN - the field id */
#else
  (
    ek_op_handle_t  op_handle,     /* IN - the operation handle */
    ek_field_id_t   field_id
  )
#endif
{
    ek_op_t              *op;
    form_bitset_t        field_attributes;
    form_field_status_t  field_status;
    long int             num_fields;
    boolean32            has_data;
    error_status_t       st;

    SET_STATUS(&st, error_status_ok);
    has_data = false;

    op = (ek_op_t *)op_handle;

    form_num_fields(op->op_form, &num_fields);
    if (!CHECK_FIELD(field_id, num_fields)) {
        SET_STATUS(&st, ek_bad_field);
    } else {
        form_field_attributes(op->op_form, field_id, &field_attributes);
        if (FIELD_UNUSED(field_attributes)) {
            SET_STATUS(&st,  ek_field_unused);
        } else {
            form_field_status(op->op_form, field_id, &field_status);
            if (field_status != form_field_no_data) {
                has_data = true;
            }
        }
    }

    SIGNAL_KERNEL_ERROR(st);

    return has_data;
}


PUBLIC void ek_key_add
#ifndef __STDC__
    (opt_keyfile, principal, key, update_registry, stp)
    char            *opt_keyfile;
    char            *principal;
    char            *key;
    boolean32       update_registry;
    error_status_t  *stp;
#else
  (
    char            *opt_keyfile,
    char            *principal,
    char            *key,
    boolean32       update_registry,
    error_status_t  *stp
  )
#endif
{
    sec_passwd_rec_t        pwd, *pwd_p;
    sec_passwd_type_t       keytype = sec_passwd_des;
    sec_timeval_period_t    exp_time;
    unsigned32              next_key_vno;

    sec_key_mgmt_get_next_kvno(rpc_c_authn_dce_private,
                                (void *) opt_keyfile,
                                (unsigned_char_p_t) principal,
                                &keytype,
                                &pwd.version_number,
                                &next_key_vno,
                                stp);
    if (GOOD_STATUS(stp)) {
        pwd_p = &pwd;
        pwd.pepper = NULL;
        if (*key == '\0') {                 /* Generate a random key? */
            pwd.key.key_type = sec_passwd_des;
            sec_key_mgmt_gen_rand_key(rpc_c_authn_dce_private,
                                        (void *)opt_keyfile,
                                        (unsigned_char_p_t) principal,
                                        &pwd.key.key_type,
                                        next_key_vno,
                                        (void **) &pwd_p,
                                        stp);
        } else {
            pwd.key.key_type = sec_passwd_plain;
            pwd.key.tagged_union.plain = (unsigned_char_p_t) key;
        }
        if (GOOD_STATUS(stp)) {
            if (update_registry) {
                pwd.version_number = next_key_vno;
                sec_key_mgmt_change_key(rpc_c_authn_dce_private,
                                        (void *)opt_keyfile,
                                        (unsigned_char_p_t) principal,
                                        pwd_p->version_number, (void *) pwd_p,
                                        &exp_time, stp);
            } else {
                sec_key_mgmt_set_key(rpc_c_authn_dce_private,
                                        (void *)opt_keyfile,
                                        (unsigned_char_p_t) principal,
                                        pwd_p->version_number,
                                        (void *) pwd_p, stp);
            }
        }
    }
}


PUBLIC void ek_key_delete
#ifndef __STDC__
    (opt_keyfile, principal, key_vno, stp)
    char            *opt_keyfile;
    char            *principal;
    unsigned32      key_vno;
    error_status_t  *stp;
#else
  (
    char            *opt_keyfile,
    char            *principal,
    unsigned32      key_vno,
    error_status_t  *stp
  )
#endif
{
    sec_key_mgmt_delete_key(rpc_c_authn_dce_private, (void *)opt_keyfile,
                            (unsigned_char_p_t) principal, key_vno, stp);
}


PUBLIC void ek_start_key_list
#ifndef __STDC__
    (cursor, opt_keyfile, principal, stp)
    void            **cursor;
    char            *opt_keyfile;
    char            *principal;
    error_status_t  *stp;
#else
  (
    void            **cursor,
    char            *opt_keyfile,
    char            *principal,
    error_status_t  *stp
  )
#endif
{
    sec_passwd_type_t   keytype = sec_passwd_none;

    sec_key_mgmt_initialize_cursor(rpc_c_authn_dce_private,
                                    (void *) opt_keyfile,
                                    (unsigned_char_p_t) principal,
                                    (void *) &keytype, cursor, stp);
}


PUBLIC void ek_do_key_list
#ifndef __STDC__
    (cursor, principal, key_vno, stp)
    void            *cursor;
    char            **principal;
    unsigned32      *key_vno;
    error_status_t  *stp;
#else
  (
    void            *cursor,
    char            **principal,
    unsigned32      *key_vno,
    error_status_t  *stp
  )
#endif
{
    sec_key_mgmt_get_next_key(cursor, (unsigned_char_p_t *) principal,
                                key_vno, NULL, stp);
}


PUBLIC void ek_end_key_list
#ifndef __STDC__
    (cursor, stp)
    void            **cursor;
    error_status_t  *stp;
#else
  (
    void            **cursor,
    error_status_t  *stp
  )
#endif
{
    sec_key_mgmt_release_cursor(cursor, stp);
}


/*
 * Global storage for the current rgy_edit identity.
 */
static  sec_login_handle_t  Current_lc = NULL;

/*
 * ek_login - Log in to specified account
 */
PUBLIC  boolean32  ek_login
#ifndef __STDC__
    (login_name, password, stp)
    char            *login_name;
    char            *password;
    error_status_t  *stp;
#else
  (
    char            *login_name,
    char            *password,
    error_status_t  *stp
  )
#endif
{
    sec_login_handle_t      lcontext = NULL, prev_lcontext = Current_lc;
    boolean32               reset_passwd;
    sec_login_auth_src_t    auth_src;
    sec_passwd_rec_t        passwd;
    ek_site_name_t          site;
    ek_site_access_t        access_type;
    error_status_t          lst;

    passwd.version_number = sec_passwd_c_version_none;
    passwd.pepper = NULL;
    passwd.key.key_type = sec_passwd_plain;
    passwd.key.tagged_union.plain = (unsigned_char_p_t) password;

    if (sec_login_setup_identity((unsigned_char_p_t) login_name,
                                    sec_login_no_flags, &lcontext, stp)) {
        if (sec_login_validate_identity(lcontext, &passwd,
                                        &reset_passwd, &auth_src, stp)) {
            sec_login_set_context(lcontext, stp);
            if (GOOD_STATUS(stp)) {
                *stp = ek_site_get_current(site, &access_type);
                if (GOOD_STATUS(stp)) {
                    lst = ek_site_close();
                    *stp = ek_site_open_by_name(site, &access_type);
                }
            }
        }
    }

    if (GOOD_STATUS(stp)) {
        Current_lc = lcontext;
        if (prev_lcontext != NULL) {
            sec_login_purge_context(&prev_lcontext, &lst);
        }
        return true;
    } else {
        if (lcontext != NULL) {
            sec_login_purge_context(&lcontext, &lst);
        }
        return false;
    }
}

/*
 * cleanup any dangling login contexts resulting from rgy_edit logins
 */
void ek_login_cleanup
#ifndef __STDC__
   ( )
#else
  (
     void
  )
#endif
{
    error_status_t  st;

    if (Current_lc != NULL) {
        sec_login_purge_context(&Current_lc, &st);
    }
}
