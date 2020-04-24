/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: repadm.c,v $
 * Revision 1.1.11.1  1996/10/03  14:47:46  arvind
 * 	mark as volatile variables live across setjmp
 * 	[1996/09/16  20:31 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.9.3  1996/02/18  00:07:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:28  marty]
 * 
 * Revision 1.1.9.2  1995/12/08  18:07:38  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/08/02  20:04 UTC  aha
 * 	CHFts15825: dced pe_site update thread does not rebind
 * 
 * 	HP revision /main/HPDCE02/aha_mothra8/1  1995/08/02  18:41 UTC  aha
 * 	CHFts15825: dced pe_site update thread doesn't rebind;
 * 	 add new API sec_rgy_rep_admin_info_any() that can bind to any working replica
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:00 UTC  greg
 * 	Merge client compatibility fixes from greg_sec_migrate2.
 * 	[1995/05/03  23:51 UTC  greg  /main/SEC_migrate_merge/2]
 * 
 * 	sec_bind_util_binding_resolve() --> sec_bind_util_resolve_binding().
 * 	[1995/05/03  21:55 UTC  greg  /main/SEC_migrate_merge/greg_sec_migrate2/2]
 * 
 * 	Move replication retry macros out into a common header file.
 * 	[1995/05/02  20:29 UTC  greg  /main/SEC_migrate_merge/greg_sec_migrate2/1]
 * 
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/20  21:02 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	More bugfixes sec_rgy_rep_admin_get_sw_vers().
 * 	[1995/02/28  22:33 UTC  greg  /main/SEC_migrate/3]
 * 
 * 	Fix some bugs in new software versioning routines.
 * 	[1995/02/22  19:57 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	Merge .../greg_sec_migrate/LATEST --> .../SEC_migrate/LATEST.
 * 	[1995/02/11  22:12 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	bugfixes in rsec_rgy_rep_admin_info_vers().
 * 	[1995/02/06  21:49 UTC  greg  /main/greg_sec_migrate/3]
 * 
 * 	Another crack at  API routines to support real
 * 	software versioning.
 * 	[1995/02/06  06:14 UTC  greg  /main/greg_sec_migrate/2]
 * 
 * 	Add software versioning support to
 * 	replica administration client API.
 * 	[1995/02/06  00:26 UTC  greg  /main/greg_sec_migrate/1]
 * 
 * Revision 1.1.5.1  1994/06/17  18:41:35  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:16  devsrc]
 * 
 * Revision 1.1.2.3  1993/03/02  22:27:01  burati
 * 	CR7400 Retry with reset handle on failure
 * 	[1993/03/02  22:14:30  burati]
 * 
 * Revision 1.1.2.2  1993/01/29  18:50:19  burati
 * 	Initial version
 * 	[1993/01/14  23:15:09  burati]
 * 
 * $EndLog$
 */

/* 
 * Copyright (c) Hewlett-Packard Company 1992, 1993, 1995
 * Unpublished work. All Rights Reserved.
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$ID: $";
#endif

#include <rca_pvt.h>  /* must be first cause it includes pbase.h */
#include <rgymacro.h>
#include <sec_bind_util.h>
#include <repadm.h>
#include <rs_repadm_1_1.h>
#include <rs_repadm.h>

/* 
 * Technically, we shouldn't be including the following in a libdce module,
 * but we need access to the global version  string constants that were
 * in use prior to the 1.1 warranty patch in order to rationalize
 * software version info received from pre-waranty-patch replicas
 * (both 1.1 and 1.0.2) so we break with tradition and use it here.
 */
#include <rs_ver.h> 
#include <rs_pvt.h> 

#define Crs_rep_admin_stop(CH, b)\
        (*rs_repadm_v1_1_c_epv.rs_rep_admin_stop)(HANDLE(CH), b)

#define Crs_rep_admin_maint(CH, b, c)\
        (*rs_repadm_v1_1_c_epv.rs_rep_admin_maint)(HANDLE(CH), b, c)

#define Crs_rep_admin_mkey(CH, b)\
        (*rs_repadm_v1_1_c_epv.rs_rep_admin_mkey)(HANDLE(CH), b)

#define Crs_rep_admin_info(CH, b, c)\
        (*rs_repadm_v1_1_c_epv.rs_rep_admin_info)(HANDLE(CH), b, c)

#define Crs_rep_admin_info_full(CH, b, c, d, e)\
        (*rs_repadm_v1_1_c_epv.rs_rep_admin_info_full)(HANDLE(CH), b, c, d, e)

#define Crs_rep_admin_destroy(CH, b)\
        (*rs_repadm_v1_1_c_epv.rs_rep_admin_destroy)(HANDLE(CH), b)

#define Crs_rep_admin_init_replica(CH, b, ST)\
        (*rs_repadm_v1_1_c_epv.rs_rep_admin_init_replica)(HANDLE(CH), b, ST)

#define Crs_rep_admin_change_master(CH, b, ST)\
        (*rs_repadm_v1_1_c_epv.rs_rep_admin_change_master)(HANDLE(CH), b, ST)

#define Crs_rep_admin_become_master(CH, ST)\
        (*rs_repadm_v1_1_c_epv.rs_rep_admin_become_master)(HANDLE(CH), ST)

#define Crs_rep_admin_become_slave(CH, ST)\
        (*rs_repadm_v1_1_c_epv.rs_rep_admin_become_slave)(HANDLE(CH), ST)

#define Crs_rep_admin_set_sw_rev(CH, a, ST)\
        if (sec_bind_util_resolve_binding(HANDLE(CH), rs_repadm_v1_2_c_ifspec, NULL, ST)) \
            (*rs_repadm_v1_2_c_epv.rs_rep_admin_set_sw_rev)(HANDLE(CH), a, ST)

#define Crs_rep_admin_get_sw_vers_info(CH, a, b, c, d, e, f, ST)\
        if (sec_bind_util_resolve_binding(HANDLE(CH), rs_repadm_v1_2_c_ifspec, NULL, ST))\
            (*rs_repadm_v1_2_c_epv.rs_rep_admin_get_sw_vers_info)\
                (HANDLE(CH), a, b, c, d, e, f, ST)


/*
 * Forward declarations of private functions.
 */

static void get_sw_vers_from_rep_info (
    rs_replica_info_t          *rep_info,
    unsigned32                 *num_sw_vers,
    rs_replica_sw_vers_info_t  **vers_info,
    unsigned32                 *current_sw_vers_offset,
    error_status_t             *stp
);

static void get_sw_vers_info_from_server (
    sec_rgy_handle_t           context,
    unsigned32                 *num_sw_vers,
    rs_replica_sw_vers_info_t  **sw_vers_info,
    unsigned32                 *current_sw_vers_offset,
    error_status_t             *status
);

/*
 *  s e c _ r g y _ r e p _ a d m i n  _ s t o p 
 *
 *  Stop the replica identified by this handle.
 */
void sec_rgy_rep_admin_stop
  (
    sec_rgy_handle_t    context,
    error_status_t      *status
  )
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_rep_admin_stop(context, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
}

/*
 *  s e c _ r g y _ r e p _ a d m i n _ m a i n t
 *
 *  Put replica in or out of maintenance mode.
 */
void sec_rgy_rep_admin_maint (
    sec_rgy_handle_t    context,
    boolean32           in_maintenance,
    error_status_t      *status
)
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_rep_admin_maint(context, in_maintenance, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
}

/*
 *  s e c _ r g y _ r e p _ a d m i n _ m k e y
 *
 *  Change the master key and re-encrypt the database
 */
void sec_rgy_rep_admin_mkey (
    sec_rgy_handle_t    context,
    error_status_t      *status
)
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_rep_admin_mkey(context, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
}


/*
 *  s e c _ r g y _ r e p _ a d m i n _ i n f o
 *
 *  Get basic information about a replica such
 *  as its state, uuid, latest update sequence
 *  number and timestamp, and whether it is the master.
 *  Also get the replica's information about the master's 
 *  uuid and the sequence number when the master was 
 *  designated.
 */
void sec_rgy_rep_admin_info (
    sec_rgy_handle_t    context,
    rs_replica_info_t   *rep_info,
    error_status_t      *status
)
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_rep_admin_info(context, rep_info, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
}

/*
 *  s e c _ r g y _ r e p _ a d m i n _ i n f o _ a n y
 *
 *  Get basic information about a replica such
 *  as its state, uuid, latest update sequence
 *  number and timestamp, and whether it is the master.
 *  Also get the replica's information about the master's 
 *  uuid and the sequence number when the master was 
 *  designated.
 *
 *  Unlike sec_rgy_rep_admin_info(), bind to any working
 *  replica.
 */
void sec_rgy_rep_admin_info_any (
    sec_rgy_handle_t    context,
    rs_replica_info_t   *rep_info,
    error_status_t      *status
)
{
    SETUP_RETRY(rca_op_read, status) {
        Crs_rep_admin_info(context, rep_info, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    RETRY_END;
}

/*
 *  s e c _ r g y _ r e p _ a d m i n _ i n f o _ f u l l
 *
 *  Get complete information about a replica such
 *  as its state, uuid, protocol towers, latest
 *  update sequence number and timestamp, and whether 
 *  it is the master.  
 *  Also get the replica's information about the master's 
 *  uuid, protocol towers, and the sequence number when 
 *  the master was designated.
 */
void sec_rgy_rep_admin_info_full (
    sec_rgy_handle_t        context,
    rs_replica_info_t       *rep_info,
    rs_replica_twr_vec_p_t  *rep_twrs,
    rs_replica_twr_vec_p_t  *master_twrs,
    error_status_t          *status
)
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_rep_admin_info_full(context, rep_info, rep_twrs,
                                master_twrs, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
    
    if (STATUS_OK(status)) {
	/* 
         * Rationalize the rep_info->compat_sw_rev field to account
         * for pre-warranty-patch 1.1 replicas, which reported 
         * a numeric value corresponding to 1.0.2 and a string
         * value (rep_info->sw_rev) corresponding to 1.1.  
	 * See sec_rgy_rep_admin_info_vers() comments for a more 
	 * complete description of why this bit of ugliness is necessary.
	 */
	if (strcmp(rep_info->sw_rev, rs_1_1_version_id) == 0) {
	    rep_info->compat_sw_rev = rs_c_software_vers_1_1;
	}
    }
}

/*
 *  s e c _ r g y _ r e p _ a d m i n _ r e s e t 
 *
 *  a drastic operation which tells a replica
 *  to destroy its database and exit
 */
void sec_rgy_rep_admin_destroy (
    sec_rgy_handle_t    context,
    error_status_t      *status
)
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_rep_admin_destroy(context, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
}

/*
 *  s e c _ r g y _ r e p _ a d m i n _ i n i t _ r e p l i c a
 *
 *  (re-)initialize the slave identified by "rep_id"
 *
 *  master-only operation.
 */
void sec_rgy_rep_admin_init_replica (
    sec_rgy_handle_t    context,
    uuid_p_t            rep_id,
    error_status_t      *status
)
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_rep_admin_init_replica(context, rep_id, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
}

/*
 *  s e c _ r g y _ r e p _ a d m i n _ c h a n g e _ m a s t e r
 *
 *  change the master to "new_master_id".
 *  The master gracefully passes its
 *  replica list state and propq to
 *  the new master.
 *
 *  master-only operation.
 */
void sec_rgy_rep_admin_change_master (
    sec_rgy_handle_t    context,
    uuid_p_t            new_master_id,
    error_status_t      *status
)
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_rep_admin_change_master(context, new_master_id, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
}

/* 
 *  s e c _ r g y _ r e p _ a d m i n _ b e c o m e _ m a s t e r
 *
 *  a drastic operation to make a slave
 *  become the master because the master
 *  has died.  
 *  Normally the rs_rep_admin_change_master 
 *  operation is used to designate a new 
 *  master; this operation can cause updates 
 *  to be lost.
 */
void sec_rgy_rep_admin_become_master (
    sec_rgy_handle_t    context,
    error_status_t      *status
)
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_rep_admin_become_master(context, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
}

/*
 *  s e c _ r g y _ r e p _ a d m i n _ b e c o m e _ s l a v e
 *
 *  a drastic operation to make a replica
 *  which thinks it's the master become a
 *  slave.
 */
void sec_rgy_rep_admin_become_slave (
    sec_rgy_handle_t    context,
    error_status_t      *status
)
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_rep_admin_become_slave(context, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
}

/*
 * New version(1.2) routines
 */

/* r e p _ a d m i n _ s e t _ s w _ r e v
 *
 * Master-only operation.
 *
 * Set the cell-wide software revision.
 */
void  sec_rgy_rep_admin_set_sw_vers (
    /* [in] */   sec_rgy_handle_t    context,
    /* [in] */   unsigned32          sw_rev,
    /* [out] */  error_status_t      *status
)
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_rep_admin_set_sw_rev(context, sw_rev, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
}


/* s e c _ r g y _ r e p _ a d m i n _ g e t _ s w _ v e r s
 * 
 * Any replica.
 *
 * Query replica regarding supported software
 * versions. 
 *
 * See detailed comments in rs_repadm.idl for an explanation
 * of how we use the wire operation.
 */
void  sec_rgy_rep_admin_get_sw_vers (
    /* [in] */   sec_rgy_handle_t            context,
    /* [out] */   unsigned32                 *num_sw_vers,
    /* [out] */   rs_replica_sw_vers_info_t  **sw_vers_info,
    /* [out] */   unsigned32                 *current_sw_vers_offset,
    /* [out] */   error_status_t             *status
)
{

    get_sw_vers_info_from_server(context, num_sw_vers, sw_vers_info, 
				 current_sw_vers_offset, status);

    if (STATUS_EQUAL(status, rpc_s_unknown_if)
    || STATUS_EQUAL(status, ept_s_not_registered)) {
	rs_replica_info_t          rep_info;
	rs_replica_twr_vec_p_t     rep_twrs;
	rs_replica_twr_vec_p_t     master_twrs;
	
	sec_rgy_rep_admin_info_full(context, &rep_info, &rep_twrs,
				    &master_twrs, status);
	
	if (STATUS_OK(status)) {
	    get_sw_vers_from_rep_info(&rep_info, num_sw_vers, sw_vers_info, 
				     current_sw_vers_offset, status);
	    /* Memory leak (rep_info, twrs) ??? */
	}
    }
}

    
/*
 *  s e c _ r g y _ r e p _ a d m i n _ i n f o _ v  e r s
 *
 *  Get complete information about a replica such
 *  as its state, uuid, protocol towers, latest
 *  update sequence number and timestamp, and whether 
 *  it is the master.  
 *  Also get the replica's information about the master's 
 *  uuid, protocol towers, and the sequence number when 
 *  the master was designated.
 *
 * *ALSO* get information about the range of
 * software versions supported by this replica.
 *
 */
void sec_rgy_rep_admin_info_vers (
    /* [in] */    sec_rgy_handle_t           context,
    /* [out] */   rs_replica_info_t          *rep_info,
    /* [out] */   rs_replica_twr_vec_p_t     *rep_twrs,
    /* [out] */   rs_replica_twr_vec_p_t     *master_twrs,
    /* [out] */   unsigned32                 *num_sw_vers,
    /* [out] */   rs_replica_sw_vers_info_t  **sw_vers_info,
    /* [out] */   unsigned32                 *current_sw_vers_offset,
    /* [out] */   error_status_t             *status
)
{
    rs_replica_sw_vers_info_t                *info_p;

    sec_rgy_rep_admin_info_full(context, rep_info, rep_twrs,
				master_twrs, status);
    if (BAD_STATUS(status)) return;

    get_sw_vers_info_from_server(context, num_sw_vers, &info_p, 
				 current_sw_vers_offset, status);

    if (STATUS_EQUAL(status, rpc_s_unknown_if)
    || STATUS_EQUAL(status, ept_s_not_registered)) {
	get_sw_vers_from_rep_info(rep_info, num_sw_vers, &info_p, 
				  current_sw_vers_offset, status);
    }

    *sw_vers_info = info_p;
}    


/*
 * Note well our use of a static local variable. Such use
 * depends on the assumption that reads and writes of unsigned32
 * values are atomic and therefore that we need not supply
 * concurrency control around such operations.
 *
 * The array of software versions returned from this routine
 * always reflects the complete range of software versions
 * supported by a replica.  The array is ordered from
 * earlist to latest version.  current_sw_vers_offset is the
 * index of the entry in the array containing the
 * version at which the replica is currently operating.
 */
static void get_sw_vers_info_from_server (
    sec_rgy_handle_t           context,
    unsigned32                 *num_sw_vers,
    rs_replica_sw_vers_info_t  **sw_vers_info,
    unsigned32                 *current_sw_vers_offset,
    error_status_t             *status
)
{
#define SW_VERS_INCREMENT 10

    static unsigned32          max_num_returned = SW_VERS_INCREMENT,
                               num_returned, min_sw_vers, max_sw_vers, cur_sw_vers;
    rs_replica_sw_vers_info_t  *info_p;
    volatile boolean32                  again;

    info_p = NULL;
    do {
	unsigned32  this_max = max_num_returned; /* !!! ATOMIC READ !!! */
	
	again = false;
	
	info_p = malloc(this_max * sizeof(*info_p));
	if (info_p == NULL) {
	    SET_STATUS(status, sec_s_no_memory);
	    break;
	}
	
	REP_SETUP_RETRY(rca_op_read, status) {
	    Crs_rep_admin_get_sw_vers_info(context, 
					   this_max,
					   &num_returned,
					   info_p,
					   &min_sw_vers,
					   &max_sw_vers,
					   &cur_sw_vers,
					   status);
	    NORMAL_CASE;
	    RETRY_CASE;
	}
	REP_RETRY_END;
	
	if (STATUS_OK(status) && (num_returned == this_max)) {
	    unsigned32  total_sw_vers =  max_sw_vers - min_sw_vers + 1;
	    
	    if (total_sw_vers > num_returned) {
		/* !!! ATOMIC WRITE !!! */
		max_num_returned += total_sw_vers - num_returned;
		again = true;
	    }
	}	    
	
	if (BAD_STATUS(status) || again) {
	    free(info_p);
	}

    } while (again);

    if (STATUS_OK(status)) {
	unsigned32  i;

	*sw_vers_info = info_p;
	*num_sw_vers = num_returned;
	
	/*
	 * Translate the wire notion of current software version
	 * (a true numeric software version) into the local notion
	 * of current software version (an index into an array of
	 * software versions).  Of course we're counting on
	 * a bug free server here--if the server were to return
         * a cur_sw_rev that lay outside of the range of
         * versions returned in info_p, we'd return
         * uninitialized garbage in the current_sw_rev parameter.
	 */
	for (i = 0; i < num_returned; i++) {
	    if (info_p[i].sw_vers == cur_sw_vers) {
		*current_sw_vers_offset = i;
		break;
	    }
	}
    } 
}

static void get_sw_vers_from_rep_info (
    rs_replica_info_t          *rep_info,
    unsigned32                 *num_sw_vers,
    rs_replica_sw_vers_info_t  **vers_info,
    unsigned32                 *current_sw_vers_offset,
    error_status_t             *stp
) 
{
    rs_replica_sw_vers_info_t *info_p;
    
    info_p =  malloc(sizeof(*info_p));
    if (info_p == NULL) {
	SET_STATUS(stp, sec_s_no_memory);
	return;
    }

    CLEAR_STATUS(stp);

    /*
     * copy version info from rep_info into first and only
     * element of sw_vers_info.
     */
    strcpy(&(info_p[0].sw_vers_printstring[0]), &(rep_info->sw_rev[0]));
    info_p[0].sw_vers = rep_info->compat_sw_rev;
    
    /* 
     * In the initial version of 1.1 that shipped from OSF
     * (prior to the warranty patch) the rs_version_id[] string 
     * constant in rs_ver.h was changed from "secd.dce.1.0.2"
     * to "secd.dce.1.1".  So the sw_rev field of the  rs_replica_info_t 
     * struct reflects the change in sw version. Unfortunately, the 
     * rs_c_state_compat_software_ver constant in rs_pvt.h was not
     * changed.  This means that the compat_sw_rev field of the rs_replica_info_t 
     * struct will always be one (1) regardless of whether the old replica 
     * is a true 1.0.2 replica or a pre-warranty-patch 1.1 replica. 
     *
     * We deal with this by testing the value of the sw_rev field against
     * the known string values for 1.0.2 and 1.1 replicas and assigning
     * the appropriate numeric software revision.  This reflects as closely 
     * as possible the reality of the situation.  A true 1.0.2 replica supports
     * only 1.0.2 functionality, and a pre-warranty-patch 1.1 replica is not capable 
     * of functioning in 1.0.2 mode, so it appears as only supporting 1.1 
     * functionality.   A post-warranty-patch 1.1 replica can function as
     * both a 1.0.2 and a 1.1 replica.  
     */
    if (strcmp(rep_info->sw_rev, rs_1_1_version_id) == 0) {
	rep_info->compat_sw_rev = info_p[0].sw_vers = rs_c_software_vers_1_1;
    }
    
    current_sw_vers_offset = 0;
    *num_sw_vers = 1;

    *vers_info = info_p;
}

