/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sa_replist.c,v $
 * Revision 1.1.7.2  1996/03/11  13:27:30  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:21  marty]
 *
 * Revision 1.1.7.1  1995/12/08  18:02:40  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/01/30  18:00 UTC  burati
 * 	Fix for CHFts14136
 * 
 * 	HP revision /main/mb_mothra/1  1995/01/27  19:51 UTC  burati
 * 	Merging fix for chfts14136 (print comm status regardless of upd_info avail).
 * 	[1995/12/08  16:30:52  root]
 * 
 * Revision 1.1.4.4  1994/06/17  18:41:26  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:11  devsrc]
 * 
 * Revision 1.1.4.3  1994/06/02  21:57:20  mdf
 * 	Merged with changes from 1.1.4.2
 * 	[1994/06/02  21:53:46  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:13:29  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.4.3  1993/12/28  21:20:09  hanfei
 * 	take out one confusing message from sa_check_replica_seqno_hightest
 * 	[1993/12/21  21:01:26  hanfei]
 * 
 * Revision 1.1.4.2  1993/12/16  18:22:33  hanfei
 * 	add sa_check_replica_seqno_hightest for verifying become master
 * 	has the highest update seqno.
 * 	[1993/12/15  21:49:48  hanfei]
 * 
 * Revision 1.1.2.3  1993/09/15  15:36:37  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.4.2  1994/06/02  20:20:35  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:13:29  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.4.3  1993/12/28  21:20:09  hanfei
 * 	take out one confusing message from sa_check_replica_seqno_hightest
 * 	[1993/12/21  21:01:26  hanfei]
 * 
 * Revision 1.1.4.2  1993/12/16  18:22:33  hanfei
 * 	add sa_check_replica_seqno_hightest for verifying become master
 * 	has the highest update seqno.
 * 	[1993/12/15  21:49:48  hanfei]
 * 
 * Revision 1.1.2.3  1993/09/15  15:36:37  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.5  1993/03/09  18:32:43  burati
 * 	CR7258 Changed call to sec_rgy_site_bind_from_towerv() from old call
 * 	sec_rgy_site_bind_from_twr_vec() (now passes cell,site names).
 * 	[1993/03/09  18:20:37  burati]
 * 
 * Revision 1.1.2.4  1993/03/08  17:49:31  burati
 * 	CR7299 fix labels on full lrep to differentiate between the replica's
 * 	copy of the last_update info and the master's copy of that same data.
 * 	[1993/03/08  16:53:15  burati]
 * 
 * Revision 1.1.2.3  1993/02/20  23:21:06  burati
 * 	CR7284 Clear status in sa_bind_to_rep_name to avoid comparing it
 * 	with unitialized data and failing even though the op succeeded.
 * 	Make sure lprop doesn't print out uninitialized data...
 * 	[1993/02/19  23:55:30  burati]
 * 
 * Revision 1.1.2.2  1993/01/29  18:52:05  burati
 * 	Initial version
 * 	[1993/01/14  22:59:53  burati]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992, 1993
 * Unpublished work. All Rights Reserved.
 */

#include <stdio.h>
#include <macros.h>
#include <dce/uuid.h>
#include <dce/replist.h>
#include <sa_util.h>

/* Private copy of the current replica list */
static rs_replica_item_t	*replist = NULL;
static rs_replica_item_full_t	*freplist = NULL;
static unsigned32       	num_reps = 0;
static unsigned32       	num_freps = 0;
extern sec_rgy_handle_t         default_context;
    
/* Printable communication states */
static struct {
    char *state;
} rep_state[] = {
    { "bad state" },
    { "ok" },
    { "short failure" },
    { "long failure" },
    { "copying database" },
    { "bad software version" }
};

/* Printable propagation states */
static struct {
    char *state;
} prop_type_text[] = {
    { "bad state" },
    { "marked for initialization" },
    { "initializing" },
    { "ready for updates" },
    { "marked for deletion" }
};


/*
 * Retrieve a copy of the replica list
 */
PUBLIC void sa_refresh_replist(context, stp)
    sec_rgy_handle_t	context;
    error_status_t      *stp;
{
    uuid_t      marker;
    unsigned32	nreps = 50;		/* Try 50 first */
    
    /* Clean up previous replist if it exists */
    if (replist != NULL) {
	free(replist);
    }
	
    /* Can't do anything without being able to reset the marker */
    uuid_create_nil(&marker, stp);
    if (BAD_STATUS(stp)) {
	return;
    }

    replist = (rs_replica_item_p_t)
	malloc(nreps * sizeof(rs_replica_item_t));
    if (replist == NULL) {
	SET_STATUS(stp, sec_s_no_memory);
	return;
    }
    sec_rgy_replist_read(context, &marker, nreps, &num_reps, replist, stp);
    if (BAD_STATUS(stp)) {
	free(replist);
	replist = NULL;
	num_reps = 0;
	return;
    }
    if (num_reps > nreps) {
	free(replist);
	nreps = num_reps;
        replist = (rs_replica_item_p_t)
	    malloc(nreps * sizeof(rs_replica_item_t));
	if (replist == NULL) {
	    SET_STATUS(stp, sec_s_no_memory);
	    num_reps = 0;
	    return;
	}
	sec_rgy_replist_read(context, &marker, nreps, &num_reps, replist, stp);
	if (BAD_STATUS(stp)) {
	    free(replist);
	    replist = NULL;
	    num_reps = 0;
	}
    }
}


/*
 * Retrieve a copy of the full replica list
 */
PUBLIC void sa_refresh_full_replist(context, stp)
    sec_rgy_handle_t	context;
    error_status_t      *stp;
{
    uuid_t      marker;
    unsigned32	nreps = 50;		/* Try 50 first */
    
    /* Clean up previous replist if it exists */
    if (freplist != NULL) {
	free(freplist);
    }
	
    /* Can't do anything without being able to reset the marker */
    uuid_create_nil(&marker, stp);
    if (BAD_STATUS(stp)) {
	return;
    }

    freplist = (rs_replica_item_full_p_t)
	malloc(nreps * sizeof(rs_replica_item_full_t));
    if (freplist == NULL) {
	SET_STATUS(stp, sec_s_no_memory);
	return;
    }
    sec_rgy_replist_read_full(context,&marker,nreps,&num_freps,freplist,stp);
    if (BAD_STATUS(stp)) {
	free(freplist);
	freplist = NULL;
	num_freps = 0;
	return;
    }
    if (num_freps > nreps) {
	free(freplist);
	nreps = num_freps;
        freplist = (rs_replica_item_full_p_t)
	    malloc(nreps * sizeof(rs_replica_item_full_t));
	if (freplist == NULL) {
	    SET_STATUS(stp, sec_s_no_memory);
	    num_freps = 0;
	    return;
	}
	sec_rgy_replist_read_full(context, &marker, nreps, &num_freps,
            freplist, stp);
        if (BAD_STATUS(stp)) {
            free(freplist);
            freplist = NULL;
            num_freps = 0;
	}
    }
}


/*
 * sa_display_replist - Display replica list with optional uuid and addresses
 * 
 * Replica list must be paged in via sa_refresh_replist before this is called,
 * or the routine will just return, displaying nothing because there
 * is no replist to display.
 */
PUBLIC void sa_display_replist(luuid, laddr, lstate, stp)
    boolean32		luuid;
    boolean32		laddr;
    boolean32		lstate;
    error_status_t      *stp;
{
    int			i, j;
    unsigned_char_p_t	id;
    error_status_t	lst, ist;
    rs_replica_info_t   info;
    sec_rgy_handle_t	rgyh;
    sec_rgy_bind_auth_info_t auth_info;
    SETUP_DEFAULT_AUTH_INFO(auth_info);
    
    for (i = 0; ((i < num_reps) && (replist != NULL)); i++) {
        printf("\n%s", replist[i].rep_name);
        if (replist[i].master)
	    printf(" (master)");
	if (replist[i].deleted)
	    printf(" (marked for deletion)");
	printf("\n");
	if (luuid) {
	    uuid_to_string(&replist[i].rep_id, &id, &lst);
	    if (GOOD_STATUS(&lst)) {
		printf("%sInstance id:   %s%s\n", saINDENT, saINDENT, id);
		free(id);
	    }
	}
	if (laddr) {
	    rpc_binding_handle_t binding;
	    unsigned_char_p_t	 bstr;
	    for (j = 0;((j < replist[i].rep_twrs->num_towers) && laddr);j++) {
		rpc_tower_to_binding(
		    replist[i].rep_twrs->towers[j]->tower_octet_string,
		    &binding, &lst);
		if (GOOD_STATUS(&lst)) {
		    rpc_binding_to_string_binding(binding, &bstr, &lst);
		    if (GOOD_STATUS(&lst)) {
			if (laddr) {
			    if (j == 0) {
                                printf("%sAddresses:     %s%s\n", saINDENT,
                                    saINDENT, bstr);
			    } else {
				printf("%s%s%s     %s\n", saINDENT, saINDENT,
                                    saINDENT,bstr);
			    }
			}
			rpc_string_free(&bstr, &lst);
			rpc_binding_free(&binding, &lst);
		    }
		}
	    }
	}
	if (lstate) {
	    sec_rgy_site_bind_from_towerv(
		(rpc_tower_vector_p_t) replist[i].rep_twrs,
		(unsigned_char_p_t) "/.:", replist[i].rep_name, &auth_info,
		&rgyh, &ist);
	    if (GOOD_STATUS(&ist)) {
		sec_rgy_rep_admin_info(rgyh, &info, &ist);
		if (GOOD_STATUS(&ist))
                    sa_print_site_info(&info, true, false, NULL, NULL, &lst);
                sec_rgy_site_close(rgyh, &lst);
	    }
	    if (BAD_STATUS(&ist)) {
		fprintf(stderr, "%sState:         %s", saINDENT, saINDENT);
		msg_status_print("unavailable", ist);
	    }
	}
    }
}


/*
 * sa_display_replist_full - Display replist with optional uuid, addresses,
 * 			     communications state and propagation info.
 * 
 * Replica list must be paged in via sa_refresh_full_replist before this is
 * called, or the routine will just return, displaying nothing because there
 * is no replist to display.
 */
PUBLIC void sa_display_full_replist(luuid, laddr, lstate, lprop, stp)
    boolean32		luuid;
    boolean32		laddr;
    boolean32		lstate;
    boolean32		lprop;
    error_status_t      *stp;
{
    int			i, j;
    unsigned_char_p_t	id;
    rs_replica_info_t   info;
    sec_rgy_handle_t	rgyh;
    error_status_t	lst, ist;
    sec_rgy_bind_auth_info_t auth_info;
    SETUP_DEFAULT_AUTH_INFO(auth_info);
    
    for (i = 0; ((i < num_freps) && (freplist != NULL)); i++) {
        printf("\n%s", freplist[i].rep_name);
        if (freplist[i].master)
	    printf(" (master)");
	if (freplist[i].deleted)
	    printf(" (marked for deletion)");
	printf("\n");
	if (luuid) {
	    uuid_to_string(&freplist[i].rep_id, &id, &lst);
	    if (GOOD_STATUS(&lst)) {
		printf("%sInstance id: %s\n", saINDENT, id);
		free(id);
	    }
	}
	if (laddr) {
	    rpc_binding_handle_t binding;
	    unsigned_char_p_t	 bstr;
	    for (j=0;((j < freplist[i].rep_twrs->num_towers) && laddr);j++) {
		rpc_tower_to_binding(
		    freplist[i].rep_twrs->towers[j]->tower_octet_string,
		    &binding, &lst);
		if (GOOD_STATUS(&lst)) {
		    rpc_binding_to_string_binding(binding, &bstr, &lst);
		    if (GOOD_STATUS(&lst)) {
			if (laddr) {
			    if (j == 0)
                                printf("%sAddresses:               %s\n", saINDENT, bstr);
			    else
				printf("%s%s%s     %s\n",saINDENT,saINDENT,saINDENT,bstr);
			}
			rpc_string_free(&bstr, &lst);
			rpc_binding_free(&binding, &lst);
		    }
		}
	    }
	}
	if (lstate) {
	    sec_rgy_site_bind_from_towerv(
		(rpc_tower_vector_p_t) freplist[i].rep_twrs,
		(unsigned_char_p_t) "/.:", freplist[i].rep_name, &auth_info,
		&rgyh, &ist);
	    if (GOOD_STATUS(&ist)) {
		sec_rgy_rep_admin_info(rgyh, &info, &ist);
		if (GOOD_STATUS(&ist))
                    sa_print_site_info(&info, true, false, NULL, NULL, &lst);
                sec_rgy_site_close(rgyh, &lst);
	    }
	    if (BAD_STATUS(&ist)) {
		fprintf(stderr, "%s%sState:         ", saINDENT, saINDENT);
		msg_status_print("unavailable", ist);
	    }
	}
	if (lprop && (! freplist[i].master)) {
            printf("%sPropagation state:       %s\n", saINDENT,
                prop_type_text[freplist[i].prop_info.prop_type]);
	    if (freplist[i].prop_info.prop_type == rs_c_replica_prop_update) {
                if (! freplist[i].prop_info.last_upd_inited) {
                    printf("%sLast update time and seqno unavailable\n",
                        saINDENT);
                } else {
                    printf("%sLast update delivered:   %s", saINDENT,
                        ctime(&freplist[i].prop_info.last_upd_ts.sec));
                    printf("%sLast update's seqno:     %d.%d\n", saINDENT,
                        freplist[i].prop_info.last_upd_seqno.high,
                        freplist[i].prop_info.last_upd_seqno.low);
                    printf("%sNumber of outstanding updates: %d\n", saINDENT,
                        freplist[i].prop_info.num_updates);
                }
                printf("%sLast comm status:        ", saINDENT);
                sa_status_only(freplist[i].comm_info.last_status);
            }
        }
    }
}


PRIVATE boolean32 sa__find_rep_item(name, itemP)
    unsigned_char_p_t	name;
    rs_replica_item_t	**itemP;
{
    int	i;
    for (i = 0; ((i < num_reps) && (replist != NULL)); i++) {
	if (strcmp((char *)name, (char *)replist[i].rep_name) == 0) {
	    *itemP = &replist[i];
	    return true;
	}
    }
    return false;
}


PUBLIC void sa_bind_to_rep_name(rep_name, ctxtP, authenticated, stp)
    unsigned_char_p_t	rep_name;
    sec_rgy_handle_t	*ctxtP;
    boolean32		authenticated;
    error_status_t      *stp;
{
    rs_replica_item_t	*itemP;
    CLEAR_STATUS(stp);

    /* Is the name on the current replica list? */
    if (! sa__find_rep_item(rep_name, &itemP)) {

	/* If not, then refresh it and try again */
	sa_refresh_replist(default_context, stp);
	if (GOOD_STATUS(stp)) {
	    /* If it's still not found, then say so */
	    if (! sa__find_rep_item(rep_name, &itemP)) {
		SET_STATUS(stp, -1);
	    }
	}
    }
    
    /* If the name was found in the replica list then attempt to bind to it */
    if (GOOD_STATUS(stp)) {
        unsigned_char_p_t tcell, tsite;
	sec_rgy_bind_auth_info_t auth_info;
	if (authenticated) {
	    auth_info.info_type = sec_rgy_bind_auth_dce;
	    auth_info.tagged_union.dce_info.authn_level =
		rpc_c_authn_level_pkt_integrity;
	    auth_info.tagged_union.dce_info.authn_svc =
		rpc_c_authn_dce_private;
	    auth_info.tagged_union.dce_info.authz_svc =
		rpc_c_authz_dce;
	    auth_info.tagged_union.dce_info.identity = NULL;
	} else {
	    auth_info.info_type = sec_rgy_bind_auth_none;
	}
        sec_rgy_site_binding_get_info(default_context, &tcell, NULL, NULL,
            NULL, stp);
        tsite = (unsigned_char_p_t) malloc(sizeof(char) *
            (strlen((char *)tcell) + strlen((char *)rep_name) + 2));
        if (tsite == NULL) {
            SET_STATUS(stp, sec_s_no_memory);
        } else {
            sprintf((char *)tsite, "%s/%s", tcell, rep_name);
        }
        if (GOOD_STATUS(stp)) {
            sec_rgy_site_bind_from_towerv(
                (rpc_tower_vector_p_t)itemP->rep_twrs, tcell, tsite,
                &auth_info, ctxtP, stp);
            free((char *) tcell);
            free((char *) tsite);
        }
    }
}

PUBLIC boolean32 sa_get_rep_id_from_name(rep_name, rep_idP)
    unsigned_char_p_t	rep_name;
    uuid_p_t            rep_idP;
{
    rs_replica_item_t	*itemP;
    error_status_t      st;
    if (sa__find_rep_item(rep_name, &itemP)) {
        *rep_idP = itemP->rep_id;
	return true;
    } else {
        /* If not found, then refresh it and try again */
        sa_refresh_replist(default_context, &st);
        if (GOOD_STATUS(&st) && sa__find_rep_item(rep_name, &itemP)) {
            *rep_idP = itemP->rep_id;
            return true;
        } else
            return false;
    }
}


/*
 * sa_check_replica_seqno_highest
 *
 * Find out if info replica has the highest seqno in all replica,
 * if so, return true, else false.
 * 
 * Replica list must be paged in via sa_refresh_full_replist before this is
 * called, or the routine will just return, displaying nothing because there
 * is no replist to display.
 */
PUBLIC boolean32 sa_check_replica_seqno_hightest(stp)
    error_status_t      *stp;
{
    int			i;
    int                 mrecent_rep=-1;
    rs_replica_info_t   host_info;
    rs_replica_info_t   info;
    sec_rgy_handle_t	rgyh;
    error_status_t      ist;

    sec_rgy_bind_auth_info_t auth_info;
    SETUP_DEFAULT_AUTH_INFO(auth_info);

    CLEAR_STATUS(stp);

    sec_rgy_rep_admin_info(default_context, &host_info, stp);
    if(BAD_STATUS(stp)) {
	sa_error_print(*stp);
	return false;
    }

    /* find out which replica has the highest seqno */
    for (i = 0; ((i < num_reps) && (replist != NULL)); i++) {
	CLEAR_STATUS(&ist);
	if (!replist[i].deleted) {
	    /* get replica seqno */
	    sec_rgy_site_bind_from_towerv(
		(rpc_tower_vector_p_t) replist[i].rep_twrs,
		(unsigned_char_p_t) "/.:", replist[i].rep_name, &auth_info,
		&rgyh, &ist);
	    if (GOOD_STATUS(&ist)) {
		sec_rgy_rep_admin_info(rgyh, &info, &ist);
		if (GOOD_STATUS(&ist)) {
		    if ( mrecent_rep == -1 ) /* not yet find one that is newer */
		        if ( sa_util_seqno_cmp(&info.last_upd_seqno,
					       &host_info.last_upd_seqno) > 0 )
			    mrecent_rep = i;
			else;
		    else
		        if ( sa_util_seqno_cmp(&info.last_upd_seqno,
					       &host_info.last_upd_seqno) > 0 ) 
			    mrecent_rep = i;
			else;
		}
                sec_rgy_site_close(rgyh, &ist);
	    }
	    else {
		/* msg_status_print("unavailable", ist); */
	    }
	}
    }
    if (mrecent_rep >= 0) {
        printf("Replica %s has newer data.\n", replist[mrecent_rep].rep_name);
	if ( sa_ask_y_n() )
	    return true;
	else
	    return false;
    }
    else
        return true;
}


