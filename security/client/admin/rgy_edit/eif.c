/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: eif.c,v $
 * Revision 1.1.8.2  1996/03/11  13:26:33  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:33  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:57:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:25  root]
 * 
 * Revision 1.1.6.3  1994/09/09  19:11:46  mob
 * 	Removing the temporary, incomplete ERA support from rgy_edit (OT 11652,11262)
 * 	[1994/09/08  23:56:21  mob]
 * 
 * Revision 1.1.6.2  1994/05/11  16:43:51  ahop
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  20:43:45  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 
 * 	hp_sec_to_osf_2 drop
 * 
 * Revision 1.1.4.3  1992/12/29  12:38:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:34:40  zeliff]
 * 
 * Revision 1.1.4.2  1992/10/08  19:44:18  burati
 * 	CR4764 Fix access past array boundary of scope[]
 * 	[1992/09/24  21:48:31  burati]
 * 
 * Revision 1.1.2.2  1992/04/03  19:26:36  burati
 * 	CR2419 Add support for local registry.
 * 	[1992/03/31  20:37:36  burati]
 * 
 * Revision 1.1  1992/01/19  14:45:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  eif.c V=15 03/02/92 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit general interface routines
**
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)eif.c	15 - 03/02/92";
#endif

#include <macros.h>

#include <dce/misc.h>

#include <eif.h>
#include <eif_def.h>
#include <args.h>

/*
* Global operation handle definition
*/
    /* Person Operations */
PUBLIC ek_op_handle_t P_get_by_name_h;
PUBLIC ek_op_handle_t P_get_by_num_h;
PUBLIC ek_op_handle_t P_change_h;
PUBLIC ek_op_handle_t P_add_h;
PUBLIC ek_op_handle_t P_delete_h;
PUBLIC ek_op_handle_t P_adopt_h;

    /* Group operations */
PUBLIC ek_op_handle_t G_get_by_name_h;
PUBLIC ek_op_handle_t G_get_by_num_h;
PUBLIC ek_op_handle_t G_change_h;
PUBLIC ek_op_handle_t G_add_h;
PUBLIC ek_op_handle_t G_delete_h;
PUBLIC ek_op_handle_t G_adopt_h;

    /* Org operations */
PUBLIC ek_op_handle_t O_get_by_name_h;
PUBLIC ek_op_handle_t O_get_by_num_h;
PUBLIC ek_op_handle_t O_change_h;
PUBLIC ek_op_handle_t O_add_h;
PUBLIC ek_op_handle_t O_delete_h;
PUBLIC ek_op_handle_t O_adopt_h;

    /* Membership operations */
PUBLIC ek_op_handle_t M_get_projlist_h;
PUBLIC ek_op_handle_t M_get_group_members_h;
PUBLIC ek_op_handle_t M_get_org_members_h;
PUBLIC ek_op_handle_t M_add_group_member_h;
PUBLIC ek_op_handle_t M_add_org_member_h;
PUBLIC ek_op_handle_t M_del_group_member_h;
PUBLIC ek_op_handle_t M_del_org_member_h;

    /* Acct operations */
PUBLIC ek_op_handle_t A_get_h;
PUBLIC ek_op_handle_t A_add_h;
PUBLIC ek_op_handle_t A_change_h;
PUBLIC ek_op_handle_t A_delete_h;

    /* LRGY operations */
PUBLIC ek_op_handle_t L_get_h;

    /* Policy operations */
PUBLIC ek_op_handle_t Pol_get_h;
PUBLIC ek_op_handle_t Pol_get_effective_h;
PUBLIC ek_op_handle_t Pol_set_h;

    /* Auth policy operations */
PUBLIC ek_op_handle_t Auth_pol_get_h;
PUBLIC ek_op_handle_t Auth_pol_get_effective_h;
PUBLIC ek_op_handle_t Auth_pol_set_h;

    /* Properties operations */
PUBLIC ek_op_handle_t Prop_rgy_get_h;
PUBLIC ek_op_handle_t Prop_rgy_set_h;

    /* Local Registry Properties operations */
PUBLIC ek_op_handle_t Prop_lrgy_get_h;
PUBLIC ek_op_handle_t Prop_lrgy_set_h;

    /* Override Policy operations */
PUBLIC ek_op_handle_t Override_get_h;
PUBLIC ek_op_handle_t Override_change_h;

    /* Cell operation */
PUBLIC ek_op_handle_t C_add_h;

/*  Current_domain is used to keep track of which domain the registry interface
 * is operating in.  By default, it's the account domain
 */
PUBLIC eif_domain_t Current_domain = eif_domain_acct;

PUBLIC sec_rgy_name_t  Current_scope[sec_rgy_domain_last];

PUBLIC boolean32 eif_auto_rebind = true;  /* rebind on operation failure? */

PUBLIC boolean32 eif_verify = true;   /* verify selected updates? */


/*********************** Private functions *********************************/


/* FUNCTION eif_init
**
** Allocates global operation handles, and turns local overrides off
*/
error_status_t eif_init(
#ifdef __STDC__
    void
#endif
)
{
    error_status_t st;

    SET_STATUS(&st, error_status_ok);

    /* Default scope is the null string */
    Current_scope[sec_rgy_domain_person][0] = '\0';
    Current_scope[sec_rgy_domain_group][0]  = '\0';
    Current_scope[sec_rgy_domain_org][0]    = '\0';

        /* a get handle operation is purely local.  A failure is signal(led)
         * by the kernel, so we can ignore the status returns */

        /* Person operation handles */
    st = ek_get_op_handle(&P_get_by_name_h, ekn_person_get_by_name);
    st = ek_get_op_handle(&P_get_by_num_h, ekn_person_get_by_num);
    st = ek_get_op_handle(&P_change_h, ekn_person_change);
    st = ek_get_op_handle(&P_add_h, ekn_person_add);
    st = ek_get_op_handle(&P_delete_h, ekn_person_delete);
    st = ek_get_op_handle(&P_adopt_h, ekn_person_adopt);

        /* Group operation handles */
    st = ek_get_op_handle(&G_get_by_name_h, ekn_group_get_by_name);
    st = ek_get_op_handle(&G_get_by_num_h, ekn_group_get_by_num);
    st = ek_get_op_handle(&G_change_h, ekn_group_change);
    st = ek_get_op_handle(&G_add_h, ekn_group_add);
    st = ek_get_op_handle(&G_delete_h, ekn_group_delete);
    st = ek_get_op_handle(&G_adopt_h, ekn_group_adopt);

        /* Organization operation handles */
    st = ek_get_op_handle(&O_get_by_name_h, ekn_org_get_by_name);
    st = ek_get_op_handle(&O_get_by_num_h, ekn_org_get_by_num);
    st = ek_get_op_handle(&O_change_h, ekn_org_change);
    st = ek_get_op_handle(&O_add_h, ekn_org_add);
    st = ek_get_op_handle(&O_delete_h, ekn_org_delete);
    st = ek_get_op_handle(&O_adopt_h, ekn_org_adopt);

        /* Account operations */
    st = ek_get_op_handle(&A_get_h, ekn_acct_get);
    st = ek_get_op_handle(&A_add_h, ekn_acct_add);
    st = ek_get_op_handle(&A_change_h, ekn_acct_change);
    st = ek_get_op_handle(&A_delete_h, ekn_acct_delete);

        /* LRGY operations */
    st = ek_get_op_handle(&L_get_h, ekn_lrgy_entry_get);

        /* Membership operations */
    st = ek_get_op_handle(&M_get_projlist_h, ekn_mem_get_person_groups);
    st = ek_get_op_handle(&M_get_group_members_h, ekn_mem_get_group_members);
    st = ek_get_op_handle(&M_get_org_members_h, ekn_mem_get_org_members);
    st = ek_get_op_handle(&M_add_group_member_h, ekn_mem_add_group_member);
    st = ek_get_op_handle(&M_add_org_member_h, ekn_mem_add_org_member);
    st = ek_get_op_handle(&M_del_group_member_h, ekn_mem_del_group_member);
    st = ek_get_op_handle(&M_del_org_member_h, ekn_mem_del_org_member);

        /* Policy Operations */
    st = ek_get_op_handle(&Pol_get_h, ekn_policy_get);
    st = ek_get_op_handle(&Pol_get_effective_h, ekn_policy_get_effective);
    st = ek_get_op_handle(&Pol_set_h, ekn_policy_set);

        /* Auth policy Operations */
    st = ek_get_op_handle(&Auth_pol_get_h, ekn_auth_policy_get);
    st = ek_get_op_handle(&Auth_pol_get_effective_h, ekn_auth_policy_get_effective);
    st = ek_get_op_handle(&Auth_pol_set_h, ekn_auth_policy_set);

        /* Properties Operations */
    st = ek_get_op_handle(&Prop_rgy_get_h, ekn_prop_rgy_get);
    st = ek_get_op_handle(&Prop_rgy_set_h, ekn_prop_rgy_set);

        /* Local Registry Properties Operations */
    st = ek_get_op_handle(&Prop_lrgy_get_h, ekn_prop_lrgy_get);
    st = ek_get_op_handle(&Prop_lrgy_set_h, ekn_prop_lrgy_set);

        /* Cell operation */
    st = ek_get_op_handle (&C_add_h, ekn_cell_add);

        /* Initialize the defaults */
    st = eif_init_defaults();

    return st;
}
