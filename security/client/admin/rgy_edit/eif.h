/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: eif.h,v $
 * Revision 1.1.9.2  1996/03/11  13:26:34  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:34  marty]
 *
 * Revision 1.1.9.1  1995/12/08  17:57:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:27  root]
 * 
 * Revision 1.1.6.2  1994/09/09  19:11:46  mob
 * 	Removing the temporary, incomplete ERA support from rgy_edit (OT 11652,11262)
 * 	[1994/09/09  00:02:34  mob]
 * 
 * Revision 1.1.6.1  1994/03/02  17:54:05  hanfei
 * 	Initial ERA drop
 * 	[1994/02/24  21:06:33  hanfei]
 * 
 * Revision 1.1.4.3  1992/12/29  12:38:40  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:34:45  zeliff]
 * 
 * Revision 1.1.4.2  1992/10/08  22:31:44  burati
 * 	CR5510 domain_text array too small for "principal"
 * 	[1992/10/08  21:49:17  burati]
 * 
 * Revision 1.1.2.2  1992/04/03  19:29:55  burati
 * 	CR2419 Add support for local registry.
 * 	[1992/03/31  20:38:14  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  eif.h V=11 03/02/92 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit general interface routines - public header file
**
*/

#ifndef EIF_H
#define EIF_H

#include <dce/rgynbase.h>

#include <ek_db.h>

/* Global Operation Handles */

    /* Person operations */
extern ek_op_handle_t P_get_by_name_h;
extern ek_op_handle_t P_get_by_num_h;
extern ek_op_handle_t P_change_h;
extern ek_op_handle_t P_add_h;
extern ek_op_handle_t P_delete_h;
extern ek_op_handle_t P_adopt_h;

    /* Group operations */
extern ek_op_handle_t G_get_by_name_h;
extern ek_op_handle_t G_get_by_num_h;
extern ek_op_handle_t G_change_h;
extern ek_op_handle_t G_add_h;
extern ek_op_handle_t G_delete_h;
extern ek_op_handle_t G_adopt_h;

    /* Org operations */
extern ek_op_handle_t O_get_by_name_h;
extern ek_op_handle_t O_get_by_num_h;
extern ek_op_handle_t O_change_h;
extern ek_op_handle_t O_add_h;
extern ek_op_handle_t O_delete_h;
extern ek_op_handle_t O_adopt_h;

    /* Membership operations */
extern ek_op_handle_t M_get_projlist_h;
extern ek_op_handle_t M_get_group_members_h;
extern ek_op_handle_t M_get_org_members_h;
extern ek_op_handle_t M_add_group_member_h;
extern ek_op_handle_t M_add_org_member_h;
extern ek_op_handle_t M_del_group_member_h;
extern ek_op_handle_t M_del_org_member_h;

    /* Acct operations */
extern ek_op_handle_t A_get_h;
extern ek_op_handle_t A_add_h;
extern ek_op_handle_t A_change_h;
extern ek_op_handle_t A_delete_h;

    /* LRGY operations */
extern ek_op_handle_t L_get_h;

    /* Policy operations */
extern ek_op_handle_t Pol_get_h;
extern ek_op_handle_t Pol_get_effective_h;
extern ek_op_handle_t Pol_set_h;

    /* Auth policy operations */
extern ek_op_handle_t Auth_pol_get_h;
extern ek_op_handle_t Auth_pol_get_effective_h;
extern ek_op_handle_t Auth_pol_set_h;

    /* Properties operations */
extern ek_op_handle_t Prop_rgy_get_h;
extern ek_op_handle_t Prop_rgy_set_h;

    /* Local registry properties operations */
extern ek_op_handle_t Prop_lrgy_get_h;
extern ek_op_handle_t Prop_lrgy_set_h;

    /* Override Policy operations */
extern ek_op_handle_t Override_get_h;
extern ek_op_handle_t Override_change_h;

    /* Cell operation */
extern ek_op_handle_t C_add_h;

/* for keeping track of the domain */
typedef enum { eif_domain_person,
	       eif_domain_group,
	       eif_domain_org,
	       eif_domain_acct,
 	       eif_domain_override
} eif_domain_t;


/*
** Current_domain is used to keep track of which domain the registry interface
** is operating in.  By default, it's the account domain
*/
extern eif_domain_t Current_domain;

/*
** Current_scope is used to keep track of which scope the registry interface
** is operating in.  By default, it's ""
*/
extern sec_rgy_name_t Current_scope[sec_rgy_domain_last];

/*
** keep track of whether or not the interface should auto-rebind. initially true
*/
extern boolean32  eif_auto_rebind;

/*
** indicates whether update verification should be performed.  true by default
*/
extern boolean32  eif_verify;

/* local error strings */
#define eif_acct_cant_delete      "Unable to delete acct(s) for"
#define eif_no_site_in_service    "Unable to contact the registry"
#define eif_no_update_site        "Unable to contact the master registry"
#define eif_cant_open_site        "Unable to open the registry at site"
#define eif_bad_default           "Invalid default"
#define eif_acct_invalid_id       "Invalid account id"

/* i/o strings */
#define eif_current_site          "Current site is:"
#define eif_site_changed          "Site changed to:"

/* Printable version of domain name */
typedef char eif_domain_name_t[10];

error_status_t eif_init (
#ifdef __STDC__
    void
#endif
);

#endif
