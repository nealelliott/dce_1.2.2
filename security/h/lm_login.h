/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: lm_login.h,v $
 * Revision 1.1.7.1  1996/07/08  18:33:02  arvind
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:52 UTC  psn  /main/DCE122_PK/1]
 *
 * 	Add prototypes for externally-called functions here.
 * 	Remove rcsid, which doesn't belong in a header file.
 * 	[1996/05/08  19:40 UTC  sommerfeld  /main/sommerfeld_CHFts17874/1]
 *
 * Revision 1.1.5.2  1996/02/18  22:58:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:57  marty]
 * 
 * Revision 1.1.5.1  1995/12/08  17:27:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:20  root]
 * 
 * Revision 1.1.3.2  1994/10/06  20:28:26  agd
 * 	expand copyright
 * 	[1994/10/06  14:27:31  agd]
 * 
 * Revision 1.1.3.1  1994/08/29  15:51:29  hondo
 * 	OT 11919 --login activity idl change to support different address types
 * 	[1994/08/28  21:47:42  hondo]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server -  Login activity management
 */

#ifdef __lm_login_h_included__
#else
#define __lm_login_h_included__

#include <krb5/hostaddr.h>
#include <dce/rpcbase.h>
#include <dce/sec_attr_base.h>

typedef enum lm_upd_type_t {
	lm_disable,
	lm_invalid_attempt,
	lm_valid_attempt,
	lm_reset } lm_upd_type_t;

typedef struct lm_address_t {
    krb5_int32 addrtype;
    krb5_int32 length;
    union{
        unsigned32              orig_ip_addr;
        unsigned char           string_binding[sec_rgy_name_max_len];
        sec_attr_twr_set_t      twr_set;
    } contents;
} lm_address_t;

typedef struct lm_upd_action_t {
    lm_upd_type_t      type;
    lm_address_t       origin;
} lm_upd_action_t;


void lm_get_origin_from_addr(
    lm_upd_action_t        *action,
    krb5_address          *from
);

void lm_update_login_info
(    krb5_principal 	   searchfor,       /* [in]  */
    lm_upd_action_t 	   *action,	    /* [in]  */
    error_status_t         *(status)       /* [out] */
);

#define REC (rec->member)
#define ACTION (action->origin)

#define GOOD_ADDRTYPE (good_entry->origin.addrtype)
#define GOOD_LENGTH (good_entry->origin.length)
#define GOOD_CONTENTS (good_entry->origin.contents)


#define BAD_ADDRTYPE (bad_entry->origin.addrtype)
#define BAD_LENGTH (bad_entry->origin.length)
#define BAD_CONTENTS (bad_entry->origin.contents)

#define NEW_GOOD (new_data->good_origin)
#define NEW_BAD (new_data->bad_origin)

#define LOGIN (login_activity)
#define LOGIN_GOOD (LOGIN->good_origin)
#define LOGIN_BAD (LOGIN->bad_origin)


#endif

