/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: eif_def.h,v $
 * Revision 1.1.6.2  1996/03/11  13:26:38  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:38  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:58:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:38  root]
 * 
 * Revision 1.1.4.2  1994/09/09  19:11:50  mob
 * 	Removing the temporary, incomplete ERA support from rgy_edit (OT 11652,11262)
 * 	[1994/09/09  00:06:55  mob]
 * 
 * Revision 1.1.4.1  1994/03/02  17:54:10  hanfei
 * 	Initial ERA drop
 * 	[1994/02/24  21:07:31  hanfei]
 * 
 * Revision 1.1.2.2  1992/12/29  12:38:59  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:11:57  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:45:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  eif_def.h V=11 08/21/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit general interface default handling - public header file
** 
*/


#ifndef EIF_DEF_H
#define EIF_DEF_H

#include <dce/nbase.h>

#include <ek_db.h>
#include <eif.h>

static char *defaults_file = ".rgy_editrc";

typedef unsigned long eif_default_id_t;

    /* use these symbolic identifiers to reference a particular default */
extern eif_default_id_t  eif_def_p_no_user_mod;
extern eif_default_id_t  eif_def_p_is_an_alias;
extern eif_default_id_t  eif_def_g_projlist_ok;
extern eif_default_id_t  eif_def_go_passwd;
extern eif_default_id_t  eif_def_pgo_fullname;
extern eif_default_id_t  eif_def_pgo_quota;
extern eif_default_id_t  eif_def_pgo_uid;
extern eif_default_id_t  eif_def_acct_passwd;
extern eif_default_id_t  eif_def_acct_misc;
extern eif_default_id_t  eif_def_acct_abbrev;
extern eif_default_id_t  eif_def_acct_homedir;
extern eif_default_id_t  eif_def_acct_shell;
extern eif_default_id_t  eif_def_acct_exp_date;
extern eif_default_id_t  eif_def_acct_passwd_valid;
extern eif_default_id_t  eif_def_acct_server;
extern eif_default_id_t  eif_def_acct_client;
extern eif_default_id_t  eif_def_acct_valid;
extern eif_default_id_t  eif_def_good_since_date;
extern eif_default_id_t  eif_def_max_ticket_lifetime;
extern eif_default_id_t  eif_def_max_renewable_lifetime;
extern eif_default_id_t  eif_def_acct_post_dated;
extern eif_default_id_t  eif_def_acct_forwardable;
extern eif_default_id_t  eif_def_acct_tgt;
extern eif_default_id_t  eif_def_acct_renewable;
extern eif_default_id_t  eif_def_acct_proxiable;
extern eif_default_id_t  eif_def_acct_dup_session_key;

/* eif_get_default
 * returns a pointer to static storage containing the character string value
 * for the default indicated by default_id, or 0 if the default id is bogus.
 * All defaults are in kernel format.  It's up to the interface to translate them
 * into something meaningful.
 */
char *eif_get_default (
#ifdef __STDC__
    eif_default_id_t default_id    /* [in] - id of desired default */
#endif
); 

/* eif_set_default
 *
 * sets the default identified by default_id to the character string value
 * in default_val.  Applies as many constraints as possible to the value.  The
 * stp parameter carries the status back out.  Returns true if the default_id
 * references a valid default value, false if the default id is bogus.  All 
 * defaults should be written in kernel format.  It's up to the interface
 * to perform any interface-to-kernel translations required before calling
 * eif_set_default.
 */ 
boolean32 eif_set_default ( 
#ifdef __STDC__ 
    eif_default_id_t  default_id,    /* [in]  - id of default being set */
    char              *default_val,  /* [in]  - value of default being set */
    error_status_t    *stp           /* [out] - status */
#endif
);

error_status_t eif_apply_add_defaults (
#ifdef __STDC__
    ek_op_handle_t  add_handle,
    eif_domain_t    domain
#endif
);

error_status_t eif_init_defaults (
#ifdef __STDC__
    void
#endif
);

error_status_t eif_save_defaults (
#ifdef __STDC__
    void
#endif
);

#endif

