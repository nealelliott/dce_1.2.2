/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: back_skulk.c,v $
 * Revision 1.1.12.1  1996/08/09  11:59:35  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  15:47 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	Incorporate fix for OT 13085: CDSD memory leaks.
 * 	[1996/02/18  19:35:33  marty  1.1.10.2]
 *
 * Revision 1.1.10.2  1996/02/18  19:35:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:51  marty]
 * 
 * Revision 1.1.10.1  1995/12/08  15:28:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:31  root]
 * 
 * Revision 1.1.6.9  1994/08/26  21:55:24  zee
 * 	In skulk_dir, add parameter in call to back_upgrade_directory_actions.
 * 	[1994/08/26  21:48:29  zee]
 * 
 * Revision 1.1.6.8  1994/08/03  19:03:36  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:19:04  mccann]
 * 
 * Revision 1.1.6.7  1994/07/08  16:17:00  zee
 * 	     HCell inheritance: In skulk_dir, call the back_hcell_inheritance
 * 	     routine to check if the parent name has changed and do the
 * 	     appropriate actions.
 * 	[1994/07/07  23:55:24  zee]
 * 
 * Revision 1.1.6.6  1994/06/30  19:18:05  mccann
 * 	     sams cleanup drop2
 * 	     [1994/06/30  18:47:35  mccann]
 * 
 * Revision 1.1.6.5  1994/06/09  18:42:53  devsrc
 * 	     cr10871 - expand copyright
 * 	     [1994/06/09  18:15:11  devsrc]
 * 
 * Revision 1.1.6.4  1994/05/06  16:04:16  zee
 * 	     During skulk of the root directory (with replica state on), call
 * 	     back_hcell_connections() to refresh any hcell connection this
 * 	     cell might have.
 * 	     [1994/05/05  21:19:15  zee]
 * 
 * Revision 1.1.6.3  1994/04/14  14:51:05  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:18:50  peckham]
 * 
 * Revision 1.1.6.2  1994/03/22  20:00:19  griffin
 * 	HCell BL3 support: Detect and modify softlinks and replica pointers when
 * 	preferred name of the cell has changed.
 * 	[1994/03/22  18:02:42  griffin]
 * 
 * Revision 1.1.6.1  1994/03/12  22:10:08  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:38:32  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:19:30  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:19:33  zeliff]
 * 
 * Revision 1.1.2.4  1992/06/30  20:17:51  peckham
 * 	OT#4482: Modified skulk to only release the database lock when
 * 	check_softlinks() is called (instead of doing a full directory release).
 * 	When the directory buffer is released, the entry update in
 * 	check_softlinks() cannot purge the entry of old data.
 * 	Besides, now the code matches the commentary.
 * 	OT#2114: uninitialized variables.
 * 	[1992/06/30  20:16:36  peckham]
 * 
 * Revision 1.1.2.3  1992/03/22  22:27:38  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:34:13  weisman]
 * 
 * Revision 1.1.2.2  1992/01/22  23:01:52  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:17:24  melman]
 * 
 * $EndLog$
 */
/*
 * Module back_skulk.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 *
 * MODULE DESCRIPTION:
 *
 * This module contains routines to schedule a skulk and perform
 * some local skulk activities.
 */

#include <server.h>
#include <dce/dce_msg.h>
#include <back.h>
#include <clerk.h>
#include <dbdef.h>
#include <events.h>
#include <find.h>
#include <names.h>
#include <security.h>
#include <sets.h>
#include <tlog.h>
#include <uid.h>
#include <hcell.h>
#include <replica.h>


#if defined(DCE_SEC)
# include <dce/aclbase.h>
# include <dpeacl.h>
#endif

/* Delta time to extend softlink timeout on error */
extern struct time_quad bkgdtimer;
extern utc_t bkgdtimer_utc;

#define MAX_NEWDIR_UPDATES 5  /* Must be at least 4 */

/*
 * Local prototypes
 */
dns_status_t
back_pseudodir_updates (
    db_stream_t         *,
    file_info_t         *,
    DBSet_t             *);

static dns_status_t
skulk_dyingdir (
    db_stream_t         *);

static void
convergence_updates (
    db_stream_t         *,
    int                 ,
    byte_t              *,
    byte_t              *);

#if defined(DCE_SEC)
static dns_status_t
acl_updates (
    db_stream_t         *,

    FullName_t          *,
    byte_t              *,
    int                 ,
    byte_t              *,
    int                 ,
    byte_t              *);

void
change_default (
    sec_acl_t           *,
    sec_acl_t           *);

#else
static void
acl_updates (
    db_stream_t         *,
    Set_t               *,
    byte_t              *);
#endif

static dns_status_t
skulk_newdir (
    db_stream_t         *);

static void
check_softlinks (
    db_stream_t         *);


/*
 * skulk_dyingdir
 *
 * This function tries to delete a child pointer.  It is called
 * with the directory read and locked.  It is the final step
 * in deleting a directory.
 *
 * Parameters:
 *      stream_p        (in)    - stream information ptr
 */
static dns_status_t
skulk_dyingdir (db_stream_t *stream_p)
{
    Update_u            update;
    tlog_dsc_t          tlog;
    dns_status_t        status;

    /*
     * try to delete child pointer
     */
    status = clerk_delete_child(
        (FullName_t *)stream_p->db_dir_hash_p->dir_name);

    /* May already have been deleted */
    if (status == DNS_UNKNOWNENTRY)
        status = DNS_SUCCESS;
    else if (status != DNS_SUCCESS)
        {
	  skulk_log(stream_p->db_dir_hash_p,
            cds_s_server_finalphase, cds_s_server_faildeletechild, NULL,
		    status);
        return(status);
        }

    /* If child is deleted, can set DNS$ReplicaState to dead */
    status = db_dir_set_state(stream_p, RS_dead, (Update_t *)update,
                &tlog.len);

    /* Log transaction */
    if (status == DNS_SUCCESS)
        {
        tlog.cnt = 1;
        tlog.buf_p = update;
        status = db_dir_update(stream_p, &tlog);
        }

    return(status);
}

/*
 * convergence_updates
 *
 * A new dirctory gets its dns$convergence value from its parent
 *
 * Parameters:
 *      stream_p        (in)    - stream info with dir read & locked
 *      valuelen        (in)    - len of value passed in
 *      value_p         (in)    - ptr to value
 *      tlog_buf_p      (in/out) - transaction log buffer
 */
static void
convergence_updates (db_stream_t  *stream_p,
                     int          valuelen,
                     byte_t       *value_p,
                     byte_t       *tlog_buf_p)
{
    tlog_dsc_t          tlog;                    /* transaction log update  */
    Timestamp_u         ts;                      /* current timestamp  */

    (void)ts_new((Timestamp_t *)ts); /* Ignore errors, use null */

    /* Set up update buffer */
    tlog.cnt = 1;
    tlog.buf_p = tlog_buf_p;
    tlog.len = update_dns_build((Update_t *)tlog_buf_p, UD_present,
                                (Timestamp_t *)ts,
                                dns_Attribute(DNS_CONVERGENCE), BL_true,
                                valuelen, value_p);

    /* Update dns$uts, no log required, it can be recreated */
    (void)set_timestamp_set(&stream_p->db_dir, (Timestamp_t *)ts,
                dns_Attribute(DNS_UTS), NULL, NULL);

    /* Update dns$lastupdate, no log required, it can be recreated */
    db_dir_set_lastupdate(stream_p->db_dir_hash_p, (Timestamp_t *)ts);

    /* Finally insert the single valued attribute */
    (void)set_insert(&stream_p->db_dir, (Update_t *)tlog_buf_p,
        dns_Attribute(DNS_CONVERGENCE), TRUE);

    /* Record transaction but don't write record yet */
    db_tlog_update(stream_p->db_file_p, OP_ModifyAttribute,
        (FullName_t *)stream_p->db_key,
        (ObjUID_t *)stream_p->db_dir.data_uid, ET_directory, &tlog);
}

#if defined(DCE_SEC)
/*
 *
 * change_default
 *   We are going to combine 2 acls with different default realms.  Means
 *   we need to change local types to foreign.  Since memory is
 *   cleaned up by the rpc runtime, we can point several entries at
 *   same data.
 *
 * Parameters:
 *      to_p    (in)
 *      from_p  (in)
 */
/*********************
**  DCE_SEC defined **
**********************/
void
change_default (sec_acl_t  *to_p,
                sec_acl_t  *from_p)
{

    int i;                                       /* index  */
    sec_acl_entry_t *entry_p;                    /* acl entry ptr  */
    unsigned32 status;                           /* ret stat from uuid fcn  */

    /* Already know the default realms are not the same */
    for (i=0; i<from_p->num_entries; ++i)
        {
        entry_p = &from_p->sec_acl_entries[i];
        switch (entry_p->entry_info.entry_type)
            {
            case sec_acl_e_type_user:
                entry_p->entry_info.entry_type = sec_acl_e_type_foreign_user;
                entry_p->entry_info.tagged_union.foreign_id.id =
                    entry_p->entry_info.tagged_union.id;
                entry_p->entry_info.tagged_union.foreign_id.realm =
                    from_p->default_realm;
                break;
            case sec_acl_e_type_group:
                entry_p->entry_info.entry_type = sec_acl_e_type_foreign_group;
                entry_p->entry_info.tagged_union.foreign_id.id =
                    entry_p->entry_info.tagged_union.id;
                entry_p->entry_info.tagged_union.foreign_id.realm =
                    from_p->default_realm;
                break;
            case sec_acl_e_type_other_obj:
                entry_p->entry_info.entry_type = sec_acl_e_type_foreign_other;
                entry_p->entry_info.tagged_union.foreign_id.id =
                    from_p->default_realm;
                break;
            case sec_acl_e_type_foreign_other:
                if (uuid_equal(&entry_p->entry_info.tagged_union.id.uuid,
                        &to_p->default_realm.uuid, &status))
                    entry_p->entry_info.entry_type = sec_acl_e_type_other_obj;
                break;
            case sec_acl_e_type_foreign_user:
                if (uuid_equal(
                    &entry_p->entry_info.tagged_union.foreign_id.realm.uuid,
                           &to_p->default_realm.uuid, &status))
                    {
                    entry_p->entry_info.entry_type = sec_acl_e_type_user;
                    entry_p->entry_info.tagged_union.id =
                        entry_p->entry_info.tagged_union.foreign_id.id;
                    }
                break;
            case sec_acl_e_type_foreign_group:
                if (uuid_equal(
                 &entry_p->entry_info.tagged_union.foreign_id.realm.uuid,
                           &to_p->default_realm.uuid, &status))
                    {
                    entry_p->entry_info.entry_type = sec_acl_e_type_group;
                    entry_p->entry_info.tagged_union.id =
                        entry_p->entry_info.tagged_union.foreign_id.id;
                    }
                break;
            case sec_acl_e_type_user_obj:
            case sec_acl_e_type_group_obj:
            case sec_acl_e_type_extended:
            case sec_acl_e_type_unauthenticated:
            case sec_acl_e_type_any_other:
            case sec_acl_e_type_mask_obj:
            default:
                break;
            } /* end switch */
        } /* End looping through acl entries */
}
/*
 * acl_updates
 *   Read the parent directories default container and initial objectcls
 *   and add them as my own.
 *   Dynamic memory for the wire acl is allocated by rpc_ss_allocate
 *   and will be freed automatically the the stub.  Since this
 *   routine may be called in the backgroud, rpc_ss_enable_allocate
 *   and rpc_ss_disable_allocate are used.
 *
 * Parameters:
 *      stream_p        (in/out)        - stream information ptr
 *      parent_p        (in/out)        - parent fullname ptr
 *      tlog_buf_p      (in/out)        - transaction log update buffer
 *      containerlen    (in/out)        -
 *      flatcontainer_p (in/out)        -
 *      objectlen       (in/out)        -
 *      flatobject_p    (in)            -
 *
 * Returns:
 *      DNS_SUCCESS on successful completion
 *      other dns error code otherwise
 */
static dns_status_t
acl_updates (db_stream_t  *stream_p,
             FullName_t   *parent_p,
             byte_t       *tlog_buf_p,
             int          containerlen,
             byte_t       *flatcontainer_p,
             int          objectlen,
             byte_t       *flatobject_p)
{
    tlog_dsc_t    tlog;                          /* transaction log storage  */
    Update_t      *upd_p;                        /* ptr to update block  */
    dns_status_t  status;                        /* current status  */
    int           len;                           /* current length  */
    int           i;                             /* index  */
    char          *flat_p;
    sec_acl_t     *new_p,
                  *old_p;
    Timestamp_t   *ts_p = NULL;                  /* update timestamp  */
    unsigned32    uidstatus;                     /* ret stat from uuid fcn  */

    /* Set up tlog buffer */
    tlog.cnt = 0;
    tlog.len = 0;
    tlog.buf_p = tlog_buf_p;
    upd_p = (Update_t *)tlog_buf_p;


    if (parent_p != NULL)
        {
        /* memory  is RPC mem and will be cleaned up at thread exit */
        status = dacl_lookup(parent_p, &flatcontainer_p, &containerlen,
                           sec_acl_type_default_container);
        }
    else if (flatcontainer_p == NULL)
        {
        status = DNS_NOTFND;
        }
    else
        {
        status = DNS_SUCCESS;
        }
    if (status == DNS_SUCCESS)
        {
        /*
         * First add my parents initial container acl as my
         * initial container acl
         */
        tlog.len += update_dns_build(upd_p, UD_present, NULL,
                                     dns_Attribute(DNS_DEFCONTAINERACL),
                                     BL_true, containerlen, flatcontainer_p);
        ++tlog.cnt;
        (void)set_insert(&stream_p->db_dir, upd_p,
                   dns_Attribute(DNS_DEFCONTAINERACL), TRUE);
        ts_p = (Timestamp_t *)upd_p->ud_timestamp;
        upd_p = (Update_t *)SKIP_Update(upd_p);

        /*
         * Locate my current object acl and combine it with my parents
         * initial container acl to create my new object acl.
         */
        if (dacl_get(&stream_p->db_dir, &new_p, DNS_OBJECTACL) !=
            DNS_SUCCESS)
            {

            /* Just use parents */
            tlog.len += update_dns_build(upd_p, UD_present, NULL,
                                         dns_Attribute(DNS_OBJECTACL), BL_true,
                                         containerlen, flatcontainer_p);
            }
        else
            {
            /*
             * Combine the 2.
             */
            if (dpe_acl_flat_to_wire((char *)flatcontainer_p, DPE_ACL_RPCMEM,
                                     &old_p) != dpe_acl_success)
                {
                status = DNS_NONSRESOURCES;
                }
            else
                {
                /* Combine */
                /* If default realms are not the same, need to make some
                 * adjustments
                 */
                if (!uuid_equal(&old_p->default_realm.uuid,
                                &new_p->default_realm.uuid, &uidstatus))
                    {
                    change_default(old_p, new_p);
                    }
                for (i=0; i<new_p->num_entries; ++i)
                    {
                    dacl_add_acl_entry(&new_p->sec_acl_entries[i], old_p,
                                       FALSE);
                    }
                if (dpe_acl_wire_to_flat(old_p, DPE_ACL_STDMEM,
                                         &flat_p, &len) != dpe_acl_success)
                    {
                    status = DNS_NONSRESOURCES;
                    }
                else
                    {
                    tlog.len += update_dns_build(upd_p, UD_present,
                                         NULL,
                                         dns_Attribute(DNS_OBJECTACL),
                                         BL_true, len, (byte_t *)flat_p);
                    free(flat_p);
                    }
                } /* End if dpe_acl_flat_to_wire == dpe_acl_success */
            } /* End if status != DNS_SUCCESS */

        /* If couldn't set up my acl, get out of here */
        if (status != DNS_SUCCESS) return(status);

        ++tlog.cnt;
        status = set_insert(&stream_p->db_dir, upd_p,
                            dns_Attribute(DNS_OBJECTACL), TRUE);
        ts_p = (Timestamp_t *)upd_p->ud_timestamp;
        upd_p = (Update_t *)SKIP_Update(upd_p);

        }  /* End if status == DNS_SUCCESS */
    else if (status == DNS_NOTFND)
        {
        status = DNS_SUCCESS;
        }
    /* Update failed */
    if (status != DNS_SUCCESS) return(status);

    /* Now get the default object acl from my parent */
    if (parent_p != NULL)
        {
        status = dacl_lookup(parent_p, &flatobject_p, &objectlen,
                           sec_acl_type_default_object);
        }
    else if (flatobject_p == NULL)
        {
        status = DNS_NOTFND;
        }
    if ((status != DNS_NOTFND) && (status != DNS_SUCCESS))
        {
        return(status);
        }
    if (status == DNS_SUCCESS)
        {
        /* Set up update buffer */
        tlog.len += update_dns_build(upd_p, UD_present, NULL,
                                         dns_Attribute(DNS_DEFOBJECTACL),
                                         BL_true, objectlen, flatobject_p);
        ++tlog.cnt;
        (void)set_insert(&stream_p->db_dir, upd_p,
                   dns_Attribute(DNS_DEFOBJECTACL), TRUE);
        ts_p = (Timestamp_t *)upd_p->ud_timestamp;
        upd_p = (Update_t *)SKIP_Update(upd_p);
        }

    if (tlog.cnt != 0)
        {
        /* Record updates in batch in transaction log */
        db_tlog_update(stream_p->db_file_p, OP_ModifyAttribute,
                       (FullName_t *)stream_p->db_key,
                       (ObjUID_t *)stream_p->db_dir.data_uid,
                       ET_directory, &tlog);
        (void)set_timestamp_set(&stream_p->db_dir, ts_p,
                          dns_Attribute(DNS_UTS), NULL, NULL);
        db_dir_set_lastupdate(stream_p->db_dir_hash_p, ts_p);
        }
    return(DNS_SUCCESS);
}
#endif /* DCE_SEC */

/************************
** End DCE_SEC defined **
*************************/


/* -------------------------------------------------------------------------- */

static dns_status_t
tmog_replica_set (
    db_stream_t *stream_p)

{
    dns_status_t      status;            /* Hold status */
    int               pass;              /* Count of passes through set */
    int               cnt;               /* Cardinality of replica set */
    int               index;             /* Index into replica set */
    DBSet_t          *set_p;             /* Current replica set */
    ReplicaPointer_t *rp_p;              /* A current replica pointer */
    FullName_t       *replica_ch_name_p; /* Holds current replica CH name */
    int               updates = 0;       /* Number of transmogrifications */
    byte_t           *tlog_buf_p;        /* Alloc'd tlog updates */
    tlog_dsc_t        tlog;              /* Transaction log update  */
    Update_t         *update_p;          /* Update work buffer */
    dns_status_t      tstatus;
    ReplicaPointer_t  new_rp;
    int               new_rp_len;
    byte_t            replicaType;
    Set_t            *addressHint_p;
    FullName_t        new_rp_name;
    Timestamp_u       ts;
    DBSet_t          *shadow_set_p;

    db_dir_release(stream_p);
    status = db_dir_read_for_update(stream_p);
    if (status != DNS_SUCCESS) {
        return(status);
    }

    status = find_attribute(stream_p->db_dir.data_p,
        dns_AttributeName(DNS_REPLICAS),
                &set_p, &index);
    if (status != DNS_SUCCESS) return (status);

    cnt = NUM_DBSet(set_p);

    /*
     * We're going to be diddling with the replica set, so we need to make
     * a copy of it first.
     */

    shadow_set_p = (DBSet_t *)dns_malloc_temp(LEN_DBSet(set_p));
    if (!shadow_set_p) {
        return (DNS_NONSRESOURCES);
    }
    COPY_DBSet(set_p, shadow_set_p);


    /*
     * Run through the replicaset twice.  First time, determine if there
     * are changes and calculate update packet sizes.  Second time, do the
     * transmogrifications.  Only do second pass if necessary.
     */

    for (pass = 0; pass < 2; pass ++) {

        for (index = 0; index < cnt; ++index) {
            DBSetMember_t *const av_p =
                       (DBSetMember_t *)INDEX_DBSet(shadow_set_p, index);

            if (!EXT8(av_p->dm_valid) || !EXT8(av_p->dm_value_present)) continue;
            rp_p = (ReplicaPointer_t *)av_p->dm_data;

            replica_parse(rp_p, NULL, &addressHint_p, NULL,
                          &replicaType,
                          &replica_ch_name_p);

            tstatus = transmogrify_cdsname(stream_p->db_file_p,
                                           replica_ch_name_p,
                                           &new_rp_name);

            if (tstatus == DNS_SUCCESS) {
                if (pass == 0) {
                    updates++;
                } else {
                    new_rp_len = replica_build(&new_rp,
                                               (ObjUID_t *)rp_p->rp_CHID,
                                               addressHint_p,
                                               replicaType,
                                               replica_ch_name_p);
                    status = ts_new((Timestamp_t *)ts);
                    if (status != DNS_SUCCESS) {
                        break;
                    }
                    tlog.len += update_dns_build(update_p,
                                                 UD_absent,
                                                 (Timestamp_t *)ts,
                                                 dns_Attribute(DNS_REPLICAS),
                                                 BL_true,
                                                 new_rp_len,
                                                 (byte_t *)&new_rp);
                    (void)set_insert(&stream_p->db_dir, update_p,
                          dns_Attribute(DNS_REPLICAS), TRUE);
                    tlog.cnt++;
                    update_p = (Update_t *)SKIP_Update(update_p);
                    new_rp_len = replica_build(&new_rp,
                                               (ObjUID_t *)rp_p->rp_CHID,
                                               addressHint_p,
                                               replicaType,
                                               &new_rp_name);
                    status = ts_new((Timestamp_t *)ts);
                    if (status != DNS_SUCCESS) {
                        break;
                    }
                    tlog.len += update_dns_build(update_p,
                                                 UD_present,
                                                 (Timestamp_t *)ts,
                                                 dns_Attribute(DNS_REPLICAS),
                                                 BL_true,
                                                 new_rp_len,
                                                 (byte_t *)&new_rp);
                    (void)set_insert(&stream_p->db_dir, update_p,
                          dns_Attribute(DNS_REPLICAS), TRUE);
                    tlog.cnt++;
                    update_p = (Update_t *)SKIP_Update(update_p);
                }
            }

        } /* End replica set loop */

        if (updates == 0) break;
        if (pass > 0) break;

        tlog_buf_p = (byte_t *)dns_malloc(sizeof(Update_u) * (2*updates));
        if (!tlog_buf_p) {
            dns_free(shadow_set_p);
            return (DNS_NONSRESOURCES);
        }

        /* Set up update buffer */
        tlog.cnt = 0;
        tlog.len = 0;
        tlog.buf_p = tlog_buf_p;
        update_p = (Update_t *)tlog_buf_p;

    } /* End "pass" loop */

    dns_free(shadow_set_p);

    if (updates > 0) {
        if (status == DNS_SUCCESS) {
            status = db_dir_update(stream_p, &tlog);
        }
        dns_free(tlog_buf_p);
    }

    return (status);

}

/* -------------------------------------------------------------------------- */

/*
 * skulk_newdir
 *
 * This function contacts the parent directory
 * and acquires the DNS$ACS, and DNS$Convergence attributes.
 * For a new directory, these parent attribute values are inherited.
 * The directory is turned on and skulking is complete.  The directory
 * is read in and locked on input.
 *
 * Parameters:
 *      stream_p (in/out)        - stream information
 *
 * Returns:
 *      DNS_SUCCESS on successful skulk of new directory
 *      DNS_NONSRESOURCES
 *      or other dns error otherwise
 */
static dns_status_t
skulk_newdir (db_stream_t *stream_p)
{
    byte_t              *tlog_buf_p;             /* alloc'd tlog updates */
    tlog_dsc_t          tlog;                    /* transaction log update  */
    FullName_u          parent;                  /* parent directory name  */
    Timestamp_u         context;                 /* context of updates ts  */
    byte_t              *buff_p;                 /* alloc'ed storage for read */
    dir_hash_entry_t    *hash_p;                 /* directory hash entry ptr */
    int                 len;                     /* update block byt size  */
    dns_status_t        status;                  /* current & exit status  */
    db_stream_t         *parentStream_p;         /* parent stream ptr  */

    status = ta_create_child_check(stream_p);
    if (status != DNS_SUCCESS) return(status);

    hash_p = stream_p->db_dir_hash_p;

    /* Build full name of parent */
    names_stripright((FullName_t *)hash_p->dir_name, (FullName_t *)parent);

    /* Allocate buffer for updates */
    tlog_buf_p = (byte_t *)dns_malloc(sizeof(Update_u)*MAX_NEWDIR_UPDATES);
    if (!tlog_buf_p) return (DNS_NONSRESOURCES);

    /* Allocate buffer for reading attributres, off of stack */
    buff_p = (byte_t *)dns_malloc(READ_BUF_SIZE);
    if (!buff_p)
        {
        dns_free((char *)buff_p);
        dns_free((char *)tlog_buf_p);
        return (DNS_NONSRESOURCES);
        }
#if !defined(DCE_SEC)

    /*
     * Get DNS$Convergence from the parent
     */
    status = clerk_read_attribute((FullName_t *)parent, ET_directory,
                dns_AttributeName(DNS_CONVERGENCE),
                buff_p, NullTimestamp, NULL);

    if (status == DNS_SUCCESS)
        {
        MemberValue_t *mv_p;

        mv_p = (MemberValue_t *)SKIP_byte(buff_p);
        convergence_updates(stream_p, EXT16(mv_p->mv_length),
                            mv_p->mv_value, tlog_buf_p);
        }
#else /* DCE_SEC */
    /* Optimmization to check if parent is already here.  Will greatly
     * reduce create_dir overhead for CDS
     */
    if ((status = db_get_stream(stream_p->db_file_p, &parentStream_p,
                           OP_ReadAttribute)) == DNS_SUCCESS)
        {
        COPY_FullName(parent, &parentStream_p->db_key);
        /* Parent is here */
        status = db_dir_read(parentStream_p);
        if (status != DNS_SUCCESS)
            db_free_stream(parentStream_p);
        }

    if (status == DNS_SUCCESS)
        {
        sec_acl_t *newO_p,*newC_p;
        byte_t *newOflat_p,*newCflat_p;
        int Olen,Clen;

        newOflat_p = newCflat_p = NULL;

        status = dacl_get(&parentStream_p->db_dir, &newC_p,
                          DNS_DEFCONTAINERACL);
        /* Storage for wire format will be cleaned up when thread exits */
        if ((status == DNS_SUCCESS) &&
            (dpe_acl_wire_to_flat(newC_p, DPE_ACL_STDMEM,(char **)&newCflat_p,
                                  &Clen) != dpe_acl_success))
            status = DNS_NONSRESOURCES;
        if ((status == DNS_SUCCESS) || (status == DNS_UNKNOWNENTRY))
            status = dacl_get(&parentStream_p->db_dir, &newO_p,
                            DNS_DEFOBJECTACL);
        if ((status == DNS_SUCCESS) &&
            (dpe_acl_wire_to_flat(newO_p, DPE_ACL_STDMEM,
                             (char **)&newOflat_p, &Olen) != dpe_acl_success))
            status = DNS_NONSRESOURCES;
        if ((status == DNS_SUCCESS) || (status == DNS_UNKNOWNENTRY))
            status = acl_updates(stream_p, NULL, tlog_buf_p,
                               Clen, newCflat_p, Olen, newOflat_p);
        if (newCflat_p)
            free((char *)newCflat_p);
        if (newOflat_p)
            free((char *)newOflat_p);

        /* Now locate convergence */
        if (status == DNS_SUCCESS)
            {
            DBSet_t *set_p;
            int index;
            DBSetMember_t *dm_p;

            if (find_attribute(parentStream_p->db_dir.data_p,
                               dns_AttributeName(DNS_CONVERGENCE),
                               &set_p, &index) == DNS_SUCCESS)
                {
                if ((dm_p = find_last_value(set_p)) != NULL)
                    convergence_updates(stream_p, dm_p->dm_length,
                                    dm_p->dm_data, tlog_buf_p);
                }
            }
        db_dir_release(parentStream_p);
        db_free_stream(parentStream_p);

        }  /* End if successfully got stream */
    else
        {
        status = acl_updates(stream_p, (FullName_t *)parent, tlog_buf_p,
                             0, NULL, 0, NULL);

        if (status == DNS_SUCCESS)
            /*
             * Get DNS$Convergence from the parent
             */
            status = clerk_read_attribute((FullName_t *)parent, ET_directory,
                                      dns_AttributeName(DNS_CONVERGENCE),
                                      buff_p, NullTimestamp, NULL);

        if (status == DNS_SUCCESS)
            {
            MemberValue_t *mv_p;

            mv_p = (MemberValue_t *)SKIP_byte(buff_p);
            convergence_updates(stream_p, EXT16(mv_p->mv_length),
                                mv_p->mv_value, tlog_buf_p);
            }
        }
#endif /* !DCE_SEC */

    if (status == DNS_SUCCESS)
        {
        DBSetMember_t *value_p;
        DBSet_t *pp_set_p;
        int index;
        /*
         * Record next 4 updates at once, update CDS_REPLICASTATE,
         * CDS_LASTSKULK and CDS_ALLUPTO.  CDS_ParentPointer is
         * logged to ensure parent id is saved from check_child
         */
        tlog.cnt = 4;
        tlog.buf_p = tlog_buf_p;
        status = db_dir_set_state(stream_p, RS_on,
                                (Update_t *)tlog.buf_p, &tlog.len);
        if (status != DNS_SUCCESS) goto cleanup;

        if (find_attribute(stream_p->db_dir.data_p,
                dns_AttributeName(DNS_PARENTPOINTER),
                &pp_set_p, &index) != DNS_SUCCESS)
        {
	  evl_AttributeMissing(stream_p->db_file_p->file_ch_p,
			       (FullName_t *)hash_p->dir_name, 
			       dns_AttributeName(DNS_PARENTPOINTER));
	  status = DNS_NAMESERVERBUG;
	  goto cleanup;
        }

        value_p = find_last_value(pp_set_p);
        if (ts_new((Timestamp_t *)context) != DNS_SUCCESS) goto cleanup;
        len = update_dns_build((Update_t *)SKIP_bytes(tlog.buf_p, tlog.len),
                             UD_present, (Timestamp_t *)context,
                             dns_Attribute(DNS_PARENTPOINTER),
                             BL_true, LEN_ParentPointer(value_p->dm_data),
                             value_p->dm_data);

        tlog.len += len;
        /* Generate new allupto and indicate skulk done */
        if (ts_new((Timestamp_t *)context) != DNS_SUCCESS) goto cleanup;
        (void)set_timestamp_set(&stream_p->db_dir, (Timestamp_t *)context,
                        dns_Attribute(DNS_LASTSKULK),
                        (Update_t *)SKIP_bytes(tlog.buf_p, tlog.len), &len);
        tlog.len += len;
        (void)set_timestamp_set(&stream_p->db_dir, (Timestamp_t *)context,
                        dns_Attribute(DNS_ALLUPTO),
                        (Update_t *)SKIP_bytes(tlog.buf_p, tlog.len), &len);
        tlog.len += len;
        status = db_dir_update(stream_p, &tlog);
        } /* if status == DNS_SUCCESS */

cleanup:
    dns_free((char *)tlog_buf_p);
    dns_free((char *)buff_p);

    return(status);
}



/* -------------------------------------------------------------------------- */

/*
 * check_softlinks
 *
 * This routine checks all the softlink entries for a directory.
 * Since it could take a while, the directory lock was released.
 * It makes sure the target of a softlink still exists.
 * It marks the link absent if either the target doesn't exist  or
 * it has timed out and the extension = 0.  The timeout is updated if
 * the target exists.
 *
 * Parameters:
 *      stream_p (in/out)       - stream infomation
 *
 */
static void
check_softlinks (db_stream_t *stream_p)
{
    bytes_u      update_buf[2*sizeof(Update_u) - 2*MAXATTRIBUTE +
                            sizeof(FullName_t) +
                            sizeof(Timeout_t)];  /* Max. 2 updates  */
    Update_t           *update_p;                /* update block ptr  */
    tlog_dsc_t          tlog;                    /* transaction log update  */
    Timeout_t           new_timeout;             /* checks link expiration  */
    Timestamp_u         newts;                   /* current timestamp  */
    utc_t               expire,extend;           /* expire - expiration ts  */
    DBSet_t             *main_p, *set_p;         /* working sets  */
    FullName_t          *target_p,               /* target of link ptr */
                        *newname_p;              /* resolved name ptr  */
    Timeout_t           *to_p;                   /* link timeout timestamp ptr*/
    int                 index;                   /* req'd for find_attribute  */
    int                 deleteit;                /* delete link flag  */
    int                 expired;                 /* link expired flag  */
    int                 target_changed;          /* flag cell name change */
    dns_status_t        status;                  /* current status  */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        ">entry_check_softlinks()"));

    tlog.cnt = 0;
    tlog.buf_p = update_buf;
    update_p = (Update_t *)update_buf;

    /* Allocate space for resolved name */
    newname_p = (FullName_t *)dns_malloc(sizeof(FullName_u));
    if (!newname_p) return;

    /* Set up key to find all softlink entries for this directory */
    names_new((ObjUID_t *)stream_p->db_dir.data_uid, NULL,
        (FullName_t *)stream_p->db_key);
    stream_p->db_entry.data_type = ET_softlink;

    /* Keep looping till error, or no more links */
    while ((status = db_entry_read_next_for_update(stream_p)) == DNS_SUCCESS)
    {
        main_p = (DBSet_t *)stream_p->db_entry.data_p->dbr_data;
        if (EXT8(main_p->ds_flag) != DS_present)
        {
            /* No more to do, get the next one */
            db_entry_release(stream_p);
            continue;
        }
        deleteit = FALSE;
        expired = FALSE;
        target_changed = FALSE;

        /* Check if link has timed out, not present means don't time out */
        if (find_attribute(stream_p->db_entry.data_p,
                dns_AttributeName(DNS_LINKTIMEOUT),
                &set_p, &index) == DNS_SUCCESS)
        {
            utc_t now;
            (void)utc_gettime(&now);
            COPY_utc(&now, new_timeout.to_expiration);

            /* Point at current timeout */
            to_p = (Timeout_t *)find_last_value(set_p)->dm_data;
            COPY_utc(to_p->to_expiration, &expire);
            COPY_utc(to_p->to_extension, &extend);

            /*
             * See if it has expired, 0 means it never expires, for v1.
             * Do byte compares cause all 0's illegal utc value
             */
            if (NE_bytes(NullTimeout, to_p, sizeof(NullTimeout)))
            {
                if (LT_utc((byte_t *)&expire, new_timeout.to_expiration))
                {
                    expired = TRUE;
                    /*
                     * If it has timed out and the extension is 0,
                     * deleteit.  Since real utc 0 is not all zeros
                     * cannot check value directly.
                     */
                    ADD_utc((byte_t *)&extend, new_timeout.to_expiration,
                          (byte_t *)&expire);
                    /* If they are equal, extension must be 0 */
                    if (EQ_utc((byte_t *)&expire, new_timeout.to_expiration))
                    {
                        deleteit = TRUE;
                    }
                }
            }
        }            /* End if find attribute dns_linktimeout */

        /* If it has expired, Make sure the target exists */
        if (expired && !deleteit)
        {
            /* If required attributes are missing, delete the soft link */
            if (find_attribute(stream_p->db_entry.data_p,
                        dns_AttributeName(DNS_LINKTARGET),
                        &set_p, &index) != DNS_SUCCESS)
            {
                deleteit = TRUE;
            }
            else
            {
                 /* Point at target full name */
                target_p = (FullName_t *)find_last_value(set_p)->dm_data;
                status = clerk_resolve_name(target_p, newname_p);
                if ((status == DNS_NOTLINKED) || (status == DNS_SUCCESS))
                {
                     /* It exists */
                     ADD_utc((byte_t *)&extend, new_timeout.to_expiration,
                                new_timeout.to_expiration);
                }
                else if (status == DNS_ACCESSDENIED)
                {
                    /* Wait a while before trying again */
                    /* need to add 'user_p' name when available thru
                     * stream_p
                     */
                    evl_SecurityFailure(
                        stream_p->db_file_p->file_ch_p->ch_nameserver,
                        target_p);

                    ADD_utc((byte_t *)&bkgdtimer_utc,
                        new_timeout.to_expiration, new_timeout.to_expiration);
                }
                else if (status == DNS_POSSIBLECYCLE)
                {             /* Delete the link */

                    evl_PossibleCycle(
                        stream_p->db_file_p->file_ch_p->ch_nameserver,
                        target_p); /* POSSIBLECYCLE_EVENT */

                    deleteit = TRUE;
                }
                else if ((status == DNS_DANGLINGLINK) ||
                         (status == DNS_UNKNOWNENTRY))
                {
                    deleteit = TRUE;
                }
                else /* Wait awhile for all other errors to go away */
                {
                     ADD_utc((byte_t *)&extend, (byte_t *)&bkgdtimer_utc,
                             new_timeout.to_expiration);
                }
            }                  /* if find dns_linktarget attribute */
        }

        /*
         * If we still plan to keep this softlink, then check the target
         * name to see if it requires transmogrification to the preferred
         * cell name.
         */

        if (!deleteit) {
            /*
             * Get the link target.  If it isn't there we have nothing
             * to do.  (When it expires, it will be deleted.)
             */
            status = find_attribute(stream_p->db_entry.data_p,
                        dns_AttributeName(DNS_LINKTARGET),
                        &set_p, &index);
            if (status == DNS_SUCCESS) {
                FullName_t  new_target_name;

                /* Retrieve target full name */
                target_p = (FullName_t *)find_last_value(set_p)->dm_data;
                status = transmogrify_cdsname(stream_p->db_file_p,
                                                  target_p,
                                                  &new_target_name);
                if (status == DNS_SUCCESS) {
                    /*
                     * Target name is a cell alias, but not preferred.  We
                     * transmogrify it to the preferred cell and update the
                     * database.
                     */
                    target_changed = TRUE;
                    tlog.len += update_dns_build(update_p, UD_present,
                        (Timestamp_t *)newts, dns_Attribute(DNS_LINKTARGET),
                        BL_true, LEN_FullName(&new_target_name),
                        (byte_t *)&new_target_name);
                    tlog.cnt++;
                    (void)set_insert(&stream_p->db_entry, update_p,
                        dns_Attribute(DNS_LINKTARGET), TRUE);
                    update_p = (Update_t *)SKIP_Update(update_p);
                }
            }
        }

        /*
         * Perform updates if necessary
         */

        if (deleteit || expired || target_changed) {

            /*
             * Updates are generated locally, so they won't
             * slip in behind allupto
             */

            if (ts_new((Timestamp_t *)newts) == DNS_SUCCESS) {
                if (deleteit)  {
                    /* Mark soft link absent */
                    main_p = (DBSet_t *)stream_p->db_entry.data_p->dbr_data;
                    INS8(main_p->ds_flag, DSX_absent);;
                    COPY_Timestamp(newts, main_p->ds_ts);

                    /* Delete contains no attribute name and type is NULL*/
                    tlog.len += update_delete_build(update_p,
                                 (Timestamp_t *)newts);
                    tlog.cnt++;
                } else {
                    /* Update timeout */
                    COPY_utc(&extend, new_timeout.to_extension);
                    tlog.len += update_dns_build(update_p, UD_present,
                        (Timestamp_t *)newts, dns_Attribute(DNS_LINKTIMEOUT),
                        BL_true, sizeof(new_timeout), (byte_t *)&new_timeout);
                    tlog.cnt++;
                    (void)set_insert(&stream_p->db_entry, update_p,
                        dns_Attribute(DNS_LINKTIMEOUT), TRUE);
                }

                /* Update timestamps */
                (void)set_timestamp_set(&stream_p->db_entry,
                        (Timestamp_t *)newts, dns_Attribute(DNS_UTS),
                        NULL, NULL);
                db_dir_set_lastupdate(stream_p->db_dir_hash_p,
                        (Timestamp_t *)newts);

                /* Update link and log transactions */
                (void)db_entry_update(stream_p, &tlog);

            } /* End if generated timestamp successfully */

        } /* End if (deleteit || expired) */

        db_entry_release(stream_p);

    } /* End loop until error and/or no more links */

    dns_free((char *)newname_p);
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        "<exit_check_softlinks()"));

}


/* -------------------------------------------------------------------------- */


/*
 * skulk_sched
 *
 * If a skulk is already running for this directory, use its
 * completion status, otherwise schedule a new skulk to run.
 * Incoming skulk requests are already individual threads.
 * They must either be allowed to continue of must wait
 * for an running thread to complete for the same directory.
 *
 * Parameters:
 *      stream_p (in/out)       - stream information
 *
 * Returns:
 *      DNS_SUCCESS on successful skulk
 *      other dns error otherwise
 */
dns_status_t
skulk_sched (db_stream_t *stream_p)
{
    dir_hash_entry_t        *hash_p     /* directory hash entry ptr  */
                                = stream_p->db_dir_hash_p;
    nsgbl_ch_t              *ch_p        /* clearinghouse information */
                                = stream_p->db_file_p->file_ch_p;
    dns_status_t            status;     /* status of skulk/exit stat */
    DEB_ASCII_BUF_FullName(namebuf)
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >skulk_sched(%s)",
        deb_ascii_FullName(namebuf,hash_p->dir_name)));

    INC_SRV_CTR(ch_p->ch_nameserver, ns_SkulksInitiated);

    if ((thstatus = dthread_lock(&(hash_p->dir_skulk_lock))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }
        /* Beginning of the 'protected code' */
        {
            if (hash_p->dir_skulk_in_progress)
            {
                /*
                 * Skulk of this directory is in progress.
                 * Wait for it to complete and use its status
                 * as this skulk's status.
                 */
                db_dir_release(stream_p);
                while (hash_p->dir_skulk_in_progress)
                {
                    if ((thstatus = dthread_wait(&(hash_p->dir_skulk_completed),
                                              &(hash_p->dir_skulk_lock))) < 0)
                    {
		      dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, 
				     (long)thstatus);
                    }
                }
                status = hash_p->dir_skulk_last_status;
            }
            else
            {
                /*
                 * No skulk in progress, so this thread gets to do the dirty
                 * work.   After marking that a skulk in in progress, do the
                 * skulk and record its status in the last status field (in
                 * case there are other threads waiting for the status), and
                 * broadcast that the skulk has completed (also for the waiting
                 threads benefit).
                 */
                hash_p->dir_skulk_in_progress = TRUE;
                if ((thstatus = dthread_unlock(&(hash_p->dir_skulk_lock))) < 0)
                {
		  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
                }
                {
                    status = skulk_dir(stream_p);
                    if (status == DNS_SUCCESS)
                    {
                        INC_SRV_CTR(ch_p->ch_nameserver, ns_SkulksCompleted);
                    }
                    else
                    {
                        evl_SkulkFailed(ch_p,
                              (FullName_t *)stream_p->db_dir_hash_p->dir_name,
                              status);  /* SKULKFAILED_EVENT */
                    }
                }

                if ((thstatus = dthread_lock(&(hash_p->dir_skulk_lock))) < 0)
                {
		  dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
                }

                hash_p->dir_skulk_last_status = status;
                hash_p->dir_skulk_in_progress = FALSE;

                if ((thstatus =
                     dthread_broadcast(&(hash_p->dir_skulk_completed))) < 0)
                {
		  dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, 
				 (long)thstatus);
                }
            } /* if no skulk in progress */
        }/*EndSERVER_LOCK*/

    /* End of the 'protected code' */
    if ((thstatus = dthread_unlock(&(hash_p->dir_skulk_lock))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <skulk_sched()"));

    return (status);
}


/* -------------------------------------------------------------------------- */


/*
 * skulk_dir
 *
 * This function starts the skulk of a directory.  It is entered with
 * the directory read and maybe locked
 *
 * Parameters:
 *      stream_p (in/out)       - stream information
 *
 * Returns:
 *      DNS_SUCCESS if all went well
 *      other dns error code otherwise
 *
 */
dns_status_t
skulk_dir (db_stream_t *stream_p)
{
    Update_u            update;                  /* update block storage  */
    tlog_dsc_t          tlog;                    /* transaction log update */
    bytes_u             mytower[READ_BUF_SIZE];  /* current address  */
    Timestamp_u         skulktime;               /* skulk timestamp  */
    dir_hash_entry_t    *hash_p;                 /* directory hash entry ptr  */
    int                 state,                   /* replica state (on, off...)*/
                        type;                    /* replica type */
    int                 changed;                 /* updates occurred flag  */
    dns_status_t        status;                  /* current status  */
    VersionNumber_u     min_version;             /* minimized replicas */
    int                 prev_func;               /* old stream access type */
    FullName_u          work_name;               /* Holds new directory name */
    int thstatus;
    LOG_ASCII_BUF_FullName( namebuf)

    state = db_dir_get_replica_state(stream_p->db_dir_hash_p);
    type = db_dir_get_replica_type(stream_p->db_dir_hash_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >skulk_dir(%s) state(%d) type(%d)",
        deb_ascii_FullName(namebuf,stream_p->db_dir_hash_p->dir_name),
        state,
        type));


    /* Perform local skulk on masters only */
    if (type == RT_master)
    {
        switch (state)
        {
            case RS_newDir:

                /* Need directory to be locked */
                if (!stream_p->db_dir.data_lock_p)
                    {
                    db_dir_release(stream_p);
                    status = db_dir_read_for_update(stream_p);
                    if (status != DNS_SUCCESS) return(status);
                    }
                /* Collect things from parent */
                status = skulk_newdir(stream_p);
                return(status);

            case RS_on:

               /*
                * Take care of any hierarchical cell connections that
                * we might have (this is done for the cell root directory
                * only).
                */

	       if (stream_p->db_dir_hash_p->dir_root) {
		   status = back_hcell_connections(stream_p);
		   DCE_SVC_DEBUG((cds__svc_handle,cds_svc_server,svc_c_debug6,
		       "  back_hcell_connections returned %d", status));
		   status = back_hcell_inheritance(stream_p);
		   DCE_SVC_DEBUG((cds__svc_handle,cds_svc_server,svc_c_debug6,
		       "  back_hcell_inheritance returned %d", status));
	       }

               /*
                * Check soft links in the directory
                * Since this could last a while, release the directory lock.
                */

                db_dir_release_lock(stream_p);
                check_softlinks(stream_p);

                /* Re-lock directory */
                db_dir_release(stream_p);
                status = db_dir_read_for_update(stream_p);
                if (status != DNS_SUCCESS)
                    {
                    skulk_log(stream_p->db_dir_hash_p,
			      cds_s_server_startphase, cds_s_server_failread, 
			      NULL, status);
                    return(status);
                    }
                break;

            case RS_dyingDir:
                /* Need directory to be locked */
                if (!stream_p->db_dir.data_lock_p)
                    {
                    db_dir_release(stream_p);
                    status = db_dir_read_for_update(stream_p);
                    if (status != DNS_SUCCESS) return(status);
                    }
                /* Remove child entry   */
                status = skulk_dyingdir(stream_p);
                return (status);
            case RS_dyingReplica:
                /* do nothing, background will cleanup later */
                return(DNS_SUCCESS);
        } /* End switch on replica state */

    }
    tlog.cnt = 1;
    tlog.buf_p = update;

    /*
     * Check our own address to make sure we're still where
     * we were the last time around
     */
    (void)towerset_build(mytower);
    changed = check_own_replica_ptr(stream_p, (Set_t *)mytower,
                        (Update_t *)update, &tlog.len);

    /* If any updates have been made, write them out */
    if (changed) {
        (void)db_dir_update(stream_p, &tlog);
    }

    /* Free locks */
    db_dir_release_lock(stream_p);

    /* Stop if clearinghouse shutting down */
    if (stream_p->db_file_p->file_state == DB_LOADED) {
        return(DNS_SUCCESS);
    }

    /* Generate proposed skulk timestamp */
    status = ts_new((Timestamp_t *)skulktime);
    if (status != DNS_SUCCESS)
    {
      skulk_log(stream_p->db_dir_hash_p, cds_s_server_precollectphase, 
		cds_s_server_failtsnew, NULL, status);
      return(status);
    }

    /*
     * Change our database access to read-only for the duration of
     * the collect phase.   This allows checkpoints to occur during this
     * potentially long phase.
     */

    status = db_modify_stream_function(stream_p, OP_ReadAttribute, &prev_func);
    /*
     * Collect all the updates
     */
    status = collect(stream_p, (Timestamp_t *)skulktime);
    if (status != DNS_SUCCESS)
    {
        skulk_log(stream_p->db_dir_hash_p,       /* Report error */
            cds_s_server_collectcombinephase, 0, NULL, status);
        return(status);
    }

    /*
     * Collect phase is done.  Revert to the original stream access mode.
     */

    status = db_modify_stream_function(stream_p, prev_func, &prev_func);
    if (status != DNS_SUCCESS)
    {
        skulk_log(stream_p->db_dir_hash_p,       /* DHE  */
		  cds_s_server_prespreadphase, cds_s_server_failswitchupdate,
		  NULL, status);
        return (status);
    }

    dce_assert(cds__svc_handle,
               (prev_func == OP_ReadAttribute));


    /* Re-lock directory record to set lastskulk */
    dns_free((char *)stream_p->db_dir.data_p);
    stream_p->db_dir.data_p = NULL;
    status = db_dir_read_for_update(stream_p);
    if (status != DNS_SUCCESS)
    {
        skulk_log(stream_p->db_dir_hash_p,cds_s_server_prespreadphase,
		  cds_s_server_failread, NULL, status);
        /* get rid of any return replica pointers stored in structure
         * use ring_count as that is the number of cells used
         */
        return(status);
    }

    /*
     * Record lastskulk so ta will not accept updates with timestamps
     * before this value.  If that happened they would not be skulked
     */
    (void)set_timestamp_set(&stream_p->db_dir, (Timestamp_t *)skulktime,
                dns_Attribute(DNS_LASTSKULK), (Update_t *)update, &tlog.len);
    status = db_dir_update(stream_p, &tlog);

    /* Free locks */
    db_dir_release_lock(stream_p);
    if (status != DNS_SUCCESS)
    {
        skulk_log(stream_p->db_dir_hash_p, cds_s_server_prespreadphase,
		  cds_s_server_failupdatels, NULL, status);
        /* get rid of any return replica pointers stored in structure
         * use ring_count as that is the number of cells used
         */
        return(status);
    }

    /*
     * Call spread to send out the updates to all clearinghouses
     * spread also returns whether or not the replica pointers have
     * changed since previous skulk
     */

    changed = FALSE;
    status = spread(stream_p, (Timestamp_t *)skulktime,
                    &changed, (VersionNumber_t *)min_version);

    /*
     * Get rid of any return replica pointers stored in structure
     * use ring_count as that is the number of cells used
     */

    if (status != DNS_SUCCESS)
    {
        skulk_log(stream_p->db_dir_hash_p,     
            cds_s_server_spreadphase, 0, NULL, status);
        return(status);
    }

     /* If replica set changed, repair it with parent */
    if (changed)
    {
        hash_p = stream_p->db_dir_hash_p;
        state = db_dir_get_replica_state(hash_p);
        if ((state != RS_dead) && (state != RS_dyingDir))
        {
            /* Get timestamp for updates, must be after lastskulk */
            utc_t  now;
            (void)utc_gettime(&now);
            /* Check replica set against child pointers */
            status = back_parentpointer_check(stream_p, &now);
            if (status != DNS_SUCCESS)
            {
                evl_CannotUpdateChildPointer( /* PARENTPOINTER_EVENT */
                        stream_p->db_file_p->file_ch_p->ch_nameserver,
                        (FullName_t *)hash_p->dir_name, status);

            }
        }
    }

    /*
     *  Check the current directory name.  If it isn't in the preferred
     *  cell, change it in the hash table and then update the parentpointer.
     */

    db_dir_release(stream_p);
    status = db_dir_read(stream_p);
    if (status != DNS_SUCCESS) {
        return(status);
    }


    status = transmogrify_cdsname(stream_p->db_file_p,
                                  (FullName_t *)stream_p->db_dir_hash_p->dir_name,
                                  (FullName_t *)work_name);
    if (status == DNS_SUCCESS) {
        utc_t  now;
        bytes_u work_buffer[READ_BUF_SIZE];
        if ((thstatus = dthread_lock(&(stream_p->db_dir_hash_p->dir_mutex))) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
        }
        /* Beginning of the 'protected code' */
            COPY_FullName(work_name, stream_p->db_dir_hash_p->dir_name);
        /* End of the 'protected code' */
        if ((thstatus = dthread_unlock(&(stream_p->db_dir_hash_p->dir_mutex))) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
        }
        (void)utc_gettime(&now);
        status = update_parentpointer(stream_p, work_buffer, &now, FALSE);
    }

    /*
     * Determine if we should transmogrify any replica pointers and
     * update them where appropriate.
     */

    status = tmog_replica_set(stream_p);

    if (status != DNS_SUCCESS) {
        return(status);
    }


    /*
     * See if we should crank up the version of the directory
     */

    db_dir_release(stream_p);
    back_upgrade_directory_actions(stream_p, (VersionNumber_t *) min_version,
				   ET_directory);

    /*
     * Set new AllUpTo
     */

    db_dir_release(stream_p);
    status = db_dir_read_for_update(stream_p);
    if (status != DNS_SUCCESS)
    {
      skulk_log(stream_p->db_dir_hash_p, cds_s_server_finalphase, 
		cds_s_server_failread, NULL, status);
      return(status);
    }

    (void)set_timestamp_set(&stream_p->db_dir, (Timestamp_t *)skulktime,
        dns_Attribute(DNS_ALLUPTO), (Update_t *)update, &tlog.len);

    status = db_dir_update(stream_p, &tlog);
    if (status != DNS_SUCCESS)
    {
      skulk_log(stream_p->db_dir_hash_p, cds_s_server_finalphase,
            cds_s_server_failupdateall, NULL, status);
        return(status);
    }

    /* Propagate allupto value to replicas */
    back_propagate_sched(stream_p, ET_directory, (Timestamp_t *)skulktime);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <skulk_dir()"));
    return(status);
}


/* -------------------------------------------------------------------------- */

/*
 * back_pseudodir_updates
 *
 *      Verifies CHCTS integrity between pseudo-directory and the
 *      clearinghouse object.
 *
 *      Update CDS_CHDirectories attribute on the clearinghouse object.
 *
 * input:
 *      Assumes stream has been opened with OP_ModifyAttribute and
 *      db_pseudo_read_for_update has been done.
 *
 * output:
 *      Releases lock whether or not it does updates pseudo directory.
 */
dns_status_t
back_pseudodir_updates (db_stream_t  *stream_p,
                        file_info_t  *file_p,
                        DBSet_t      *chset_p)
{
    nsgbl_ch_t          *ch_p                    /* clearinghouse infomation  */
                            = file_p->file_ch_p;
    Timestamp_u         update_ts;               /* timestamp of this update  */
    static ClassVersion_t obj_version = {{2},{0}};
    dns_status_t        return_status,           /* exit status  */
                        status,                  /* current status  */
                        save_status=DNS_SUCCESS; /* stat saved from loop  */
    int                 index,                   /* used in find_attribute  */
                        pos,                     /* position in set  */
                        len;                     /* byt siz of update block  */
    DBSet_t             *alluptoset_p;           /* DNS$ALLUPTO attr set ptr  */
    DBSetMember_t       *value_p;                /* value in DNS$ALLUPTO set  */
    Timestamp_t         *allupto_ts_p;           /* ptr to AllUpTo value  */
    dir_hash_entry_t    *hash_p;                 /* directory hash entry ptr  */
    CHDirectoryPointer_t chdir_data;             /* update packet data  */
    Update_u            update_block;            /* update block storage  */
    byte_t              flag;                    /* attr pres or absent flag */
    DEB_ASCII_BUF_Timestamp(timeworkbuf)
    DEB_ASCII_BUF_FullName( nameworkbuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >back_pseudodir_updates(%s)",
        deb_ascii_FullName(nameworkbuf,&ch_p->ch_name)));

    /* Make sure objectuid is okay */
    status = clerk_test_attribute(&ch_p->ch_name, ET_object,
                dns_AttributeName(DNS_OBJECTUID), sizeof(ch_p->ch_uid),
                (byte_t *)&ch_p->ch_uid, dns_Attribute(DNS_OBJECTUID)->vtype);

    if (status == DNS_UNKNOWNENTRY)
        {
        /* Attempt automatic re-creation */
        (void)clerk_create_object(&ch_p->ch_name,
                (ClassName_t *)cds_classlist[DNS_CLEARINGHOUSE].cname,
                &obj_version, &ch_p->ch_uid, NULL);
        /* Export rdacl interface to object */
        (void)net_export_ch(ch_p);
        }
    else if (status == DNS_NOTFND)
        {
        /* Try to update dns_objectuid of clearinghouse object */
        len = update_dns_build( (Update_t *)update_block,
                UD_present,
                (Timestamp_t *)NullTimestamp,
                dns_Attribute(DNS_OBJECTUID), BL_true,
                LEN_ObjUID(&ch_p->ch_uid), (byte_t *)&ch_p->ch_uid);

        (void)clerk_modify_attribute((FullName_t *)&ch_p->ch_name,
                                ET_object, (Update_t *)update_block, len);
        /* Fix up object for rdacl interface  - something changed */
        (void)net_re_export_ch(ch_p);
        }

    /* Save timestamp for recording as allupto if this update succeeds */
    (void)ts_new((Timestamp_t *)update_ts);

    /* Get current all up to time of pseudo directory.  */
    status = find_attribute(stream_p->db_dir.data_p,
            dns_AttributeName(DNS_ALLUPTO),
            &alluptoset_p, &index);
    if (status != DNS_SUCCESS)
        {
        allupto_ts_p = (Timestamp_t *)NullTimestamp;
        }
    else
        {
        value_p = (DBSetMember_t *)DATA_DBSet(alluptoset_p);
        allupto_ts_p = (Timestamp_t *)value_p->dm_data;
        }

    /*
     * In this case, must delete chdirectories attribute for the object
     * because allupto is null.
     */
    if (dnsCmpCTS((dns_cts *)NullTimestamp, (dns_cts *)allupto_ts_p) == 0)
        {
        len = update_dns_build( (Update_t *)update_block,
                UD_absent,
                (Timestamp_t *)NULL,
                dns_Attribute( DNS_CHDIRECTORIES),
                BL_false,
                0,
                (byte_t *)NULL);
        status = clerk_modify_attribute( (FullName_t *)&ch_p->ch_name,
                ET_object,
                (Update_t *)update_block,
                len);
        if (status != DNS_SUCCESS)
            {
            db_dir_release_lock( stream_p);
            return (status);
            }
        }

    for (pos = 0; pos < NUM_DBSet(chset_p); pos++)
        {                                        /* Begin loop directories  */
        DBSetMember_t *dbsetmem_p = (DBSetMember_t *)INDEX_DBSet(chset_p, pos);

        /* If directory changed before last skulk, no new data to update */
        if (dnsCmpCTS((dns_cts *)dbsetmem_p->dm_ts,
                         (dns_cts *)allupto_ts_p)  < 0)
            continue;

        /* Get DHE to find directory name */
        hash_p = db_hash_uid_ignore_state( file_p,
            (ObjUID_t *)dbsetmem_p->dm_data);
        if (!hash_p)
            {                                    /* Have no DHE  */
          DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_server,
              svc_c_debug7,
              "  back_pseudodir_updates, no hash entry for (%s) -> deleted",
              deb_ascii_FullName(nameworkbuf,&ch_p->ch_name)));
            /*
             * Reset allupto attribute to indicate that we must
             * reset the CHDIRECTORIES attribute.
             */
            if (NE_Timestamp(NullTimestamp, allupto_ts_p))
                {
                COPY_Timestamp( NullTimestamp, update_ts);
                break;
                }
            else
                {
                if (EXT8(dbsetmem_p->dm_flag) == DM_present)
                {
                    DCE_SVC_DEBUG((
                        cds__svc_handle,
                        cds_svc_server,
                        svc_c_debug7,
                        "back_pseudo_updates: lost replica has present value\n"));
                }
                continue;
            }                            /* End have lost replica  */
      }                                  /* Have no DHE  */


        /* Create update packet */
        COPY_FullName( hash_p->dir_name, chdir_data.cp_directory);
        COPY_ObjUID((ObjUID_t *)dbsetmem_p->dm_data,
              chdir_data.cp_dirID);

        /* Modify attribute */
        if (EXT8(dbsetmem_p->dm_flag) == DM_present)
            flag = UD_present;
        else
            flag = UD_absent;

        len = update_dns_build( (Update_t *)update_block,
                flag,
                (Timestamp_t *)NULL,
                dns_Attribute( DNS_CHDIRECTORIES),
                BL_true,
                LEN_CHDirectoryPointer( &chdir_data),
                (byte_t *)&chdir_data);

        status = clerk_modify_attribute( (FullName_t *)&ch_p->ch_name,
                ET_object,
                (Update_t *)update_block,
                len);
        if (status != DNS_SUCCESS)
            {
            save_status = status;
            }
        }                                        /* End loop directories  */

    /*
     * If not successful, just return, otherwise update allupto & purge
     * absent space from pseudo directory.
     */
    if (save_status != DNS_SUCCESS)
        {                                        /* Failed so  */
        return_status = save_status;
        db_dir_release_lock( stream_p);          /* allupto is unchanged.  */
        }
    else
        {                                        /* Had updates/noerrors  */
         /* Set new allupto */
        (void)update_dns_build((Update_t *)update_block, UD_present,
                NULL, dns_Attribute(DNS_ALLUPTO),
                BL_true, sizeof(update_ts), (byte_t *)update_ts);
        status = set_insert(&stream_p->db_dir, (Update_t *)update_block,
                dns_Attribute(DNS_ALLUPTO), TRUE);
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug7,
            "  back_pseudodir_updates-DNS$CHDirectories update (%s) status(%d)",
            deb_ascii_Timestamp(timeworkbuf,update_ts),
            status));
        db_cleanup_record(&stream_p->db_dir, (Timestamp_t *)update_ts);
        return_status = db_pseudo_update( stream_p, ((tlog_dsc_t *)0));
        }                                        /* Had updates  */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <back_pseudodir_updates - status(%d)",
        return_status));

    return (return_status);
}

/*
 * skulk_log
 *      Provide way for user to easily figure out why a skulk failed by.
 *
 * input:
 *      hash_p (in/out)  - addr of DHE of dir that had skulk running on it
 *      phasetype (in) - code of character string indicating what phase of
 *                         skulk was running when skulk failed ... NULL results
 *                         in a phase type of "In Skulk".
 *      explain (in)   - code of character string indicating what was
 *                         happening when err_stat was generated or NULL
 *      name_p (in)     = optional reason parameter
 *      err_stat (in)    - specific error status caused by skulk failure or NULL
 * Note that the last three input parameters may be NULL.  A NULL
 * is taken to mean there is no data for that parameter.
 *
 * returns:
 *      void.
 *
 */
void
skulk_log (
    dir_hash_entry_t    *hash_p,
    int                 phasetype,
    int                 reason,
    char                *name_p,
    dns_status_t        err_stat)
{
    LOG_ASCII_BUF_FullName( namebuf)
    unsigned32 status;
    unsigned char *explain_p,*phase_p;

    /*
     * Record type of skulk failure.
     */
    if (phasetype == 0)                            /* If no phase given,  */
        phasetype = cds_s_server_skulkphase;

    phase_p = dce_msg_get_msg(phasetype, &status);

    if (hash_p)                                  /* If have dir info  */
        {                                        /* Display in log file  */
        deb_ascii_FullName( namebuf, hash_p->dir_name);
        }
    else                                         /* No dir info  */
        {                                        /* just ensure no failure  */
        namebuf[ 0] = '\0';
        }

    if (reason != 0)
        {
	if (name_p)
	    explain_p = dce_sprintf(reason, name_p);
	else
	    explain_p = dce_msg_get_msg(reason, &status);
        dce_svc_printf(CDS_S_SERVER_SKULKSTAT1_MSG, namebuf, phase_p, 
		       explain_p, (long)err_stat);
	if (explain_p)
	    free(explain_p);
        }
    else
        {
        dce_svc_printf(CDS_S_SERVER_SKULKSTAT2_MSG, namebuf, phase_p, 
		       (long)err_stat);
        }

    if (phase_p)
	free(phase_p);

}
