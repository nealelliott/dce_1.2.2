/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_hash.c,v $
 * Revision 1.1.6.2  1996/02/18  19:36:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:30  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:30:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:04  root]
 * 
 * Revision 1.1.4.5  1994/08/03  19:04:03  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:22:00  mccann]
 * 
 * Revision 1.1.4.4  1994/06/09  18:43:16  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:45  devsrc]
 * 
 * Revision 1.1.4.3  1994/04/14  14:51:13  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:19:51  peckham]
 * 
 * Revision 1.1.4.2  1994/03/22  20:00:25  griffin
 * 	HCell BL3 support: Support relative names for directory name hash
 * 	table lookup.   Resulting behavior is transparent to callers.
 * 	[1994/03/22  18:07:25  griffin]
 * 
 * Revision 1.1.4.1  1994/03/12  22:11:05  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:41:37  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:21:14  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:21:23  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:25:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: db_hash.c
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
 */

#include <server.h>
#include <dbdef.h>
#include <dnsnormal.h>
#include <find.h>
#include <names.h>
#include <sets.h>
#include <tlog.h>
#include <uid.h>
#include <hcell.h>
#include <ctype.h>

#include <dce/rpc.h>

/* -------------------------------------------------------------------------- */

/*
 * ROUTINE - db_hash_free
 *
 * Remove a directory from the hash tables.  This call can
 * only be made with a guarantee that no pointer
 * to the hash entries are held by other threads.  Threads
 * can be waiting though.
 *
 * If the clearinghouse file_p is provided, it is assumed that the associated
 * clearinghouse is being removed, and doesn't need updating.
 *
 * All surviving memory is released.
 *
 * Input:
 *      hash_p = pointer to entry to remove (unlocked)
 *      file_p = if not NULL, associated clearinghouse file (locked)
 */
dns_status_t
db_hash_free (dir_hash_entry_t  *hash_p,
              file_info_t       *file_p)
{
    DEB_ASCII_BUF_dir_hash_entry(hashBuf)
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >db_hash_free(%s,%s)",
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_file_info(fileBuf,file_p)));

    if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' pt 1 */
        { /* 1 */

        file_info_t *const pfile_p = hash_p->dir_file_info_p;

        /* Inform users that we are going away */
        hash_p->dir_state = dir_rundown;

        /* and wait for users to go away... */
        while (hash_p->dir_users)
        {
            if ((thstatus = dthread_wait(&(hash_p->dir_state_changed),
                                         &(hash_p->dir_mutex))) < 0)
            {
	      dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
              }
        }

        if (pfile_p)
        {/* 1st */
            /*
             * Remove directory from chdirectories
             * db_pseudo_chdirectory(file_p,
             *  (ObjUID_t *)hash_p->dir_uid, UD_absent);
             */
            if (pfile_p != file_p)
            {
                if ((thstatus = dthread_lock(&(pfile_p->file_mutex))) < 0)
                {
		  dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
                }
            }

                   { /* 2*/

                        dir_hash_entry_t *cur_p;
                        register unsigned int pos;

                        /* Remove entry from UID hash table */
                        pos = hash_p->dir_uid_index;

                        if ((cur_p = pfile_p->file_uid_table[pos]) == hash_p)
                            pfile_p->file_uid_table[pos] = hash_p->dir_uid_collision_p;
                        else
                        {
                            while (cur_p->dir_uid_collision_p != hash_p)
                                cur_p = cur_p->dir_uid_collision_p;
                            cur_p->dir_uid_collision_p = hash_p->dir_uid_collision_p;
                        }

                        /* Remove entry from name hash table */
                        pos = hash_p->dir_name_index;

                        if ((cur_p = pfile_p->file_name_table[pos]) == hash_p)
                            pfile_p->file_name_table[pos] = hash_p->dir_name_collision_p;
                        else
                        {
                            while (cur_p->dir_name_collision_p != hash_p)
                                cur_p = cur_p->dir_name_collision_p;

                            cur_p->dir_name_collision_p = hash_p->dir_name_collision_p;
                        }

                        /*
                         * If this is a root directory replica, then mark the clearinghouse
                         * as no longer storing the root directory.
                         */
                        if (hash_p->dir_root)
                            pfile_p->file_root_dir_present = FALSE;
                    }

                if (pfile_p != file_p)
                {
                    /* End of the 'protected code' pt 2 */
                    if ((thstatus = dthread_unlock(&(pfile_p->file_mutex))) < 0)
                    {
		      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, 
				     (long)thstatus);
                    }
                }
        }

    }
    /* End of the 'protected code' pt1 */

    if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, 
		     (long)thstatus);
    }

    /* Release directory record memory */
    if (hash_p->dir_p)
    {
        dns_free((char *)hash_p->dir_p);
    }

    /* Release btree memory */
    if (hash_p->dir_entry_p)
      db_btree_free(hash_p->dir_entry_p);

    /* Delete threads-related components */
    {
        if ((thstatus = dthread_delete_condition(&hash_p->dir_update_accessable)) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
        }
    }
    if ((thstatus = dthread_delete_mutex(&hash_p->dir_entry_locks_lock)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
    }

    if ((thstatus = dthread_delete_mutex(&hash_p->dir_skulk_lock)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
    }

    if ((thstatus = dthread_delete_condition(&hash_p->dir_skulk_completed)) <0)
    {
      dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
    }

    if ((thstatus = dthread_delete_mutex(&hash_p->dir_mutex)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
    }

    /* Free the entry itself */
    dns_free((char *)hash_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <db_hash_free(%p,%s) return(%s)",
        hash_p,
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_dns_status(statusBuf,DNS_SUCCESS)));

    return(DNS_SUCCESS);
}

/* -------------------------------------------------------------------------- */

/*
 * db_find_name_hash_index
 *      Hash a directory name and return its index in the hash table
 *
 * Input:
 *      name_p = address of directory full name
 */
unsigned int
db_find_name_hash_index (FullName_t *name_p)
{
    DEB_ASCII_BUF_FullName(nameBuf)
    register unsigned int result = 0;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_find_name_hash_index(%s)",
        deb_ascii_FullName(nameBuf,name_p)));

    /* If it is the root, we are done */
    if (!EMPTY_FullName(name_p))
        {
        SimpleName_t prev_temp, last_temp;
        register SimpleName_t *cur_p = (SimpleName_t *)name_p->fn_name;
        register SimpleName_t *last_p = (SimpleName_t *)NullSimpleName;
        register SimpleName_t *prev_p;
        int last_len, prev_len, max;
        register int i;

        /* Find last simple names */
        do
            {
            prev_p = last_p;
            last_p = cur_p;
            cur_p = (SimpleName_t *)SKIP_SimpleName(last_p);
            }
        while (EXT8(cur_p->sn_flag) != SN_null);

        /*
         * X.500 -
         * If either of the two simple names located is of type
         * SN_typed, then we must normalize them before the hashing
         * function is applied, so that all subsequent hashes for
         * any equivalent name return the same index. The code below
         * does this for all other name types by upcasing all bytes.
         * This doesn't work for typed names at all!
         */
        if (EXT8(prev_p->sn_flag) == SN_typed)
            {
            if (dns_normalize_sname(prev_p, &prev_temp, sizeof(prev_temp))
                                            != DNS_SUCCESS)
                goto leave_db_find_name_hash_index;
            prev_p = &prev_temp;
            }
        if (EXT8(last_p->sn_flag) == SN_typed)
            {
            if (dns_normalize_sname(last_p, &last_temp, sizeof(last_temp))
                                            != DNS_SUCCESS)
                goto leave_db_find_name_hash_index;
            last_p = &last_temp;
            }

        prev_len = EXT8(prev_p->sn_length);
        last_len = EXT8(last_p->sn_length);
        if ((max = last_len) < prev_len) max = prev_len;

        /*
         * Add the last two simple names together and multiply each
         * member by a power of 2 up to 25 characters
         */
        if (25 < max) max = 25;
        for (i = 0; i < max; i++)
            {
            register unsigned int next = 0;

            if (i < prev_len)
		next += islower(prev_p->sn_name[i])
			? toupper(prev_p->sn_name[i])
			: prev_p->sn_name[i];
            if (i < last_len)
		next += islower(last_p->sn_name[i])
			? toupper(last_p->sn_name[i])
			: last_p->sn_name[i];
            result += next << i;
            }
        result %= NAME_HASH_LEN;
        }

leave_db_find_name_hash_index:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_find_name_hash_index(%s) return(%u)",
        deb_ascii_FullName(nameBuf,name_p),
        result));
    return(result);
}

/* -------------------------------------------------------------------------- */

/*
 * db_hash_new_name
 *      Change the name in the hash table.  This is done because the name
 *      was found to be linked in check_parentpointer or it was just
 *      received in do_updates.  There may be
 *      outstanding pointer to the hash entry so the pointer must
 *      be maintained.
 *
 * Input:
 *      file_p = pointer to file information (unlocked)
 *      hash_p = pointer to hash entry (locked)
 *      name_p = pointer to new directory full name
 */
void
db_hash_new_name (file_info_t       *file_p,
                  dir_hash_entry_t  *hash_p,
                  FullName_t        *name_p)
{
    DEB_ASCII_BUF_dir_hash_entry(hashBuf)
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_FullName(nameBuf)
    unsigned int newindex, oldindex;
    dir_hash_entry_t *cur_p;
    FullName_t        rel_name;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_hash_new_name(%s,%s,%s)",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_FullName(nameBuf,name_p)));

    /* Obtain the relative name */
    cell_relative_fullname(file_p, name_p, &rel_name);

    /* Hash name to index */
    newindex = db_find_name_hash_index(&rel_name);

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {

        /* Remove current name */
        oldindex = hash_p->dir_name_index;

        if (file_p->file_name_table[oldindex] == hash_p)
            file_p->file_name_table[oldindex] = hash_p->dir_name_collision_p;
        else
            {
            cur_p = file_p->file_name_table[oldindex];
            while (cur_p->dir_name_collision_p != hash_p)
                cur_p = cur_p->dir_name_collision_p;
            cur_p->dir_name_collision_p = hash_p->dir_name_collision_p;
            }

        /* Replace name */
        COPY_FullName(name_p, hash_p->dir_name);
        COPY_FullName(&rel_name, hash_p->dir_relative_name);

        /* Now put in new position */
        hash_p->dir_name_index = newindex;
        hash_p->dir_name_collision_p = file_p->file_name_table[newindex];
        file_p->file_name_table[newindex] = hash_p;

        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, 
		     (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_hash_new_name(%s,%s,%s)",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_FullName(nameBuf,name_p)));
}

/* -------------------------------------------------------------------------- */

/*
 * db_hash_name
 * db_hash_name_int
 *
 *      Hash a directory name and return a pointer to the directory descriptor.
 *      If the directory is dead, return NULL.
 *
 * Input:
 *      file_p = pointer to file info (unlocked)
 *      name_p = pointer to directory full name
 *      isroot = boolean is name_p the root directory (normally false,
 *               used for special database load handling).
 */

static
dir_hash_entry_t *
db_hash_name_int (
    file_info_t         *file_p,
    FullName_t          *name_p,
    int                  isroot)
{
    DEB_ASCII_BUF_FullName(fullnameBuf)
    DEB_ASCII_BUF_FullName(relnameBuf)
    dir_hash_entry_t *hash_p;
    unsigned int      idx;
    FullName_t        rel_name;
    dns_status_t      status;
    int               thstatus;

    /* Obtain the relative name */
    if (isroot) {
        FullName_t empty_name;
        COPY_FullName(NullFullName, &empty_name);
        COPY_ObjUID(name_p->fn_root, empty_name.fn_root);
        status = cell_relative_fullname(file_p, &empty_name, &rel_name);
    } else {
        status = cell_relative_fullname(file_p, name_p, &rel_name);
    }

    if (status != CDS_SUCCESS) {
        return ((dir_hash_entry_t *) NULL);
    }

    /* Hash name to index */
    idx = db_find_name_hash_index(&rel_name);

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {

        /* !!!!!!!  Fix handles here !!!!! */
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug5,
            "  db_hash_name(%s) -> %s (%d)\n",
            deb_ascii_FullName(fullnameBuf, name_p),
            deb_ascii_FullName(relnameBuf, &rel_name), idx));


        /* Look through list for name match */
        hash_p = file_p->file_name_table[idx];

        /* Loop through collisions */
        while (hash_p)
        {
            if (names_fullmatch(&rel_name, 
                                (FullName_t *)hash_p->dir_relative_name))
            {
                /*
                 * Make sure directory is not dead.  We don't remove dead
                 * entries cause there may be outstanding pointer to them,
                 * but we don't have to return
                 */
                if (hash_p->dir_replica_state != RS_dead)
                    break;
            }
            hash_p = hash_p->dir_name_collision_p;
        }

        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    return(hash_p);
}


dir_hash_entry_t
*db_hash_name (file_info_t  *file_p,
               FullName_t   *name_p)
{
    return (db_hash_name_int(file_p, name_p, 0));
}


/* -------------------------------------------------------------------------- */

/*
 * db_find_uid_hash_index
 *      Hash a directory UID and return its index in the hash table
 *
 * Input:
 *      uid_p = address of uid
 */
unsigned int
db_find_uid_hash_index (ObjUID_t *uid_p)
{
    DEB_ASCII_BUF_ObjUID(uidBuf)
    register unsigned int result;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_find_uid_hash_index(%s)",
        deb_ascii_ObjUID(uidBuf,uid_p)));

    {
    unsigned32 status;
    uuid_t id;
    COPY_bytes(uid_p, &id, sizeof(id));
    result = uuid_hash(&id, &status) % UID_HASH_LEN;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_find_uid_hash_index(%s) return(%u)",
        deb_ascii_ObjUID(uidBuf,uid_p),
        result));

    return(result);

}

/* -------------------------------------------------------------------------- */

/*
 * db_hash_uid
 *      Hash a directory UID,
 *      Return pointer to dir hash entry.  Dead entries are not returned.
 *
 * Input:
 *      file_p = pointer to file info (unlocked)
 *      uid_p = pointer to directory uid
 */
dir_hash_entry_t
*db_hash_uid (file_info_t  *file_p,
              ObjUID_t     *uid_p)
{
    dir_hash_entry_t *hash_p;
    unsigned int idx;
    int thstatus;


    /* Get index into hash table */
    idx = db_find_uid_hash_index(uid_p);

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {

        hash_p = file_p->file_uid_table[idx];

        /* Loop through collisions */
        while (hash_p)
            {
            if (EQ_ObjUID(uid_p, hash_p->dir_uid))
                /*
                 * Make sure directory is not dead.  We don't remove dead
                 * entries cause there may be outstanding pointer to them,
                 * but we don't have to return
                 */
                if (hash_p->dir_replica_state != RS_dead)
                    break;

            hash_p = hash_p->dir_uid_collision_p;
            }

        }
        /* End of the 'protected code' */


    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    return(hash_p);
}

/* -------------------------------------------------------------------------- */

/*
 * db_hash_insert
 *      Add a directory to the hash table
 *
 * Input:
 *      file_p = pointer to file information block (unlocked)
 *      dir_p = directory in-memory structure (locked)
 *      type = type of record, DIR_REC_TYPE OR CH_REC_TYPE
 *
 * Output:
 *      return_hash_pp = address to return pointer to hash table
 */
dns_status_t
db_hash_insert (file_info_t         *file_p,
                directory_bucket_t  *dir_p,
                unsigned int        type,
                dir_hash_entry_t    **return_hash_pp)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_directory_bucket(dirBuf)
    DEB_ASCII_BUF_dir_hash_entry(hashBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    FullName_u          neutral_case_name;
    dir_hash_entry_t    *hash_p = NULL;
    FullName_t          *name_p = NULL;
    ObjUID_t            *uid_p = NULL;
    DBSet_t             *set_p;
    DBSetMember_t       *val_p;
    dns_status_t        status;
    int                 isroot = 0;
    int                 idx;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " >db_hash_insert(%s,%s,%d,%p)",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_directory_bucket(dirBuf,dir_p),
        type,
        return_hash_pp));

    /*
     * Find directory UID - in-memory structure of a directory
     * contains the same format as a data record passed to the caller.
     */


    if (find_attribute((data_record_t *)dir_p->dir_data,
             dns_AttributeName(DNS_OBJECTUID), &set_p, &idx) == DNS_SUCCESS)
        if ((val_p = find_last_value(set_p)) != NULL)
            uid_p = (ObjUID_t *)val_p->dm_data;
    /*
     * Find directory name, the in-memory structure of a directory
     * contains the same format as a data record passed to the caller.
     */
    if (type == ET_directory)
        {
        if (find_attribute((data_record_t *)dir_p->dir_data,
                dns_AttributeName(DNS_PARENTPOINTER),
                &set_p, &idx) == DNS_SUCCESS)
            if ((val_p = find_last_value(set_p)) != NULL)
                {
                ParentPointer_t *pp_p = (ParentPointer_t *)val_p->dm_data;

                name_p = (FullName_t *)pp_p->pp_myName;
                }
        }
    else
        if (find_attribute((data_record_t *)dir_p->dir_data,
                dns_AttributeName(DNS_CHNAME), &set_p, &idx) == DNS_SUCCESS)
            if ((val_p = find_last_value(set_p)) != NULL)
                name_p = (FullName_t *)val_p->dm_data;

    if (!name_p)
        {
        /*
         * The root directory should be the only entry without a name
         */
        name_p = (FullName_t *)neutral_case_name;
        COPY_FullName(&file_p->file_ch_p->ch_cellname, name_p);
        isroot = 1;
        }

    /* Make sure directory {uid|name} doesn't exist */
    if (db_hash_uid(file_p, uid_p))
        {
        status = DNS_ENTRYEXISTS;
        goto leave_db_hash_insert;
    }

    if (db_hash_name_int(file_p, name_p, isroot)) {
        status = DNS_ENTRYEXISTS;
        goto leave_db_hash_insert;
        }

    hash_p = (dir_hash_entry_t *)dns_malloc_perm(sizeof(dir_hash_entry_t));
    if (!hash_p)
        {
        status = DNS_NONSRESOURCES;
        goto leave_db_hash_insert;
    }

    /*
     * Initialize new entry
     */

    ZERO_bytes(hash_p, sizeof(dir_hash_entry_t));

    if ((thstatus = dthread_create_mutex(&hash_p->dir_mutex)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
    }

    hash_p->dir_p = dir_p;
    hash_p->dir_type = type;

    if ((thstatus = dthread_create_condition(&hash_p->dir_state_changed)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
    }

    if ((thstatus = dthread_create_condition(&hash_p->dir_update_accessable)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
    }

    hash_p->dir_update_access_owner = NULL;

    if ((thstatus = dthread_create_mutex(&hash_p->dir_entry_locks_lock)) < 0)
    { 
      dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
    }

    init_list(&hash_p->dir_entry_locks);

    if ((thstatus = dthread_create_mutex(&hash_p->dir_skulk_lock)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
    }

    hash_p->dir_skulk_in_progress = FALSE;

    if ((thstatus = dthread_create_condition(&hash_p->dir_skulk_completed)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
    }

    hash_p->dir_skulk_last_status = 0;
    hash_p->dir_prop_status = DNS_SUCCESS;      /* Init propagation status  */
    COPY_Timestamp(NullTimestamp, hash_p->dir_prop_time);

    /* Find replica state as stored in record */
    hash_p->dir_replica_state = RS_on;          /* default */

    if (find_attribute((data_record_t *)dir_p->dir_data,
            dns_AttributeName(DNS_REPLICASTATE), &set_p, &idx) == DNS_SUCCESS)
        {
        /*
         * DNS_REPLICASTATE is always a single valued set with no history
         */
        DBSetMember_t *member_p = (DBSetMember_t *)DATA_DBSet(set_p);

        hash_p->dir_replica_state = EXT8(member_p->dm_data);
        }

    /* Find replica type as stored in record */
    hash_p->dir_replica_type = RT_readOnly;                     /* default */

    if (find_attribute((data_record_t *)dir_p->dir_data,
            dns_AttributeName(DNS_REPLICATYPE), &set_p, &idx) == DNS_SUCCESS)
        {
        /*
         * DNS_REPLICATYPE is always a single valued set with no history
         */
        DBSetMember_t *member_p = (DBSetMember_t *)DATA_DBSet(set_p);

        hash_p->dir_replica_type = EXT8(member_p->dm_data);
        }

    COPY_Timestamp(NullTimestamp, hash_p->dir_lastupdate);
    hash_p->dir_root = isroot;
    COPY_ObjUID(uid_p, hash_p->dir_uid);
    COPY_FullName(name_p, hash_p->dir_name);
    if (isroot) {
        FullName_t empty_name;
        COPY_FullName(NullFullName, &empty_name);
        COPY_ObjUID(name_p->fn_root, empty_name.fn_root);
        cell_relative_fullname(file_p, &empty_name,
                               (FullName_t *)hash_p->dir_relative_name);
    } else {
        cell_relative_fullname(file_p, name_p,
                               (FullName_t *)hash_p->dir_relative_name);
    }

    if ((hash_p->dir_file_info_p = file_p) != NULL)
    { /* if N */

        if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
        }

          { /* Beginning of the 'protected code' */

           register unsigned int i;

            /*
             * If this is a root directory replica, then mark the clearinghouse
             * as storing the root directory (performance optimization
             * for certain routines).
             */
            if (hash_p->dir_root) {
                file_p->file_root_dir_present = TRUE;
                file_p->file_root_dir_p = hash_p;
            }

            /* Insert new entry in the hash table(s) */

            i = db_find_uid_hash_index(uid_p);
            hash_p->dir_uid_index = i;
            hash_p->dir_uid_collision_p = file_p->file_uid_table[i];
            file_p->file_uid_table[i] = hash_p;

            i = db_find_name_hash_index((FullName_t *)hash_p->dir_relative_name);
            hash_p->dir_name_index = i;
            hash_p->dir_name_collision_p = file_p->file_name_table[i];
            file_p->file_name_table[i] = hash_p;

       } /* End of the 'protected code' */

      if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
      }
    }

    status = DNS_SUCCESS;

leave_db_hash_insert:
    *return_hash_pp = hash_p;
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " <db_hash_insert(%s,%s,%d,%p->%s) return(%s)",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_directory_bucket(dirBuf,dir_p),
        type,
        return_hash_pp,
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}



/* -------------------------------------------------------------------------- */

/*
 * db_hash_locate
 *      Find the hash entry given and entry fullname.
 *
 * Input:
 *      file_p = pointer to file info (unlocked)
 *      key_name_p = pointer to fullname of directory entry.
 *
 */
dir_hash_entry_t
*db_hash_locate (file_info_t  *file_p,
                 FullName_t   *key_name_p)
{
    SimpleName_u holdname;
    SimpleName_t *name_p;
    dir_hash_entry_t *hash_p;

    /*
     *  First we check the UID at the beginning of the fullname.
     *  If this UID is the sames as the NSUID for the clearinghouse
     *  we are working on, then we hash the rest of the fullname.
     *
     *  If it doesn't equal the NSUID, then we assume that the UID in the
     *  fullname is a directory UID, and we'll hash that - ignoring the
     *  contents of the rest of the fullname.
     */

    if (EQ_ObjUID(key_name_p->fn_root, &file_p->file_ch_p->ch_nsuid))
        {
        /* Strip entry name off end of key */
        name_p = names_pointlast(key_name_p);
        COPY_SimpleName(name_p, holdname);
        INS16(key_name_p->fn_length,
            EXT16(key_name_p->fn_length) - LEN_SimpleName(name_p));
        COPY_SimpleName(NullSimpleName, name_p);

        /* Look up entire directory */
        hash_p = db_hash_name(file_p, key_name_p);

        /* Restore flags */
        COPY_SimpleName(holdname, name_p);
        INS16(key_name_p->fn_length,
            EXT16(key_name_p->fn_length) + LEN_SimpleName(name_p));
        }
   else
        {
        hash_p = db_hash_uid(file_p, (ObjUID_t *)key_name_p->fn_root);
        }

    return(hash_p);
}

/* -------------------------------------------------------------------------- */

/*
 * db_hash_uid_ignore_state
 *      Hash a directory UID,
 *      Return pointer to dir hash entry regardless of state, ie, return even
 *      if have dead entry.
 *
 * Input:
 *      file_p = pointer to file info (unlocked)
 *      uid_p = pointer to directory uid
 */
dir_hash_entry_t
*db_hash_uid_ignore_state (file_info_t  *file_p,
                           ObjUID_t     *uid_p)
{
    dir_hash_entry_t *hash_p;
    unsigned int idx;
    int thstatus;


    /* Get index into hash table */
    idx = db_find_uid_hash_index(uid_p);

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {

        hash_p = file_p->file_uid_table[idx];

        /* Loop through collisions */
        while (hash_p)
            {
            if (EQ_ObjUID(uid_p, hash_p->dir_uid))
                    break;

            hash_p = hash_p->dir_uid_collision_p;
            }

        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    return(hash_p);
}
