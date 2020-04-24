/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_cleanup.c,v $
 * Revision 1.1.6.2  1996/02/18  19:35:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:12  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:29:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:48  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:03:51  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:20:55  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:43:04  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:27  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:10:33  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:39:59  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:20:16  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:20:25  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:25:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: db_cleanup.c
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
 */
#include <server.h>
#include <dbdef.h>
#include <uid.h>

/*
 * Local prototypes
 */
static byte_t *
cleanup_members (
    DBSet_t		*,
    byte_t		*,
    Timestamp_t		*);

/*
 * cleanup_members
 *     This routine purges absent members from attributes as
 * long as their timestamp is before allupto
 *
 * Input:
 *    oldset_p = pointer to current set
 *    newset_p = pointer to new set
 *    allupto_p = address of timestamp which bounds purges.
 */
static byte_t 
*cleanup_members (DBSet_t      *oldset_p,
                  byte_t       *new_p,
                  Timestamp_t  *allupto_p)
{
    DBSet_t *newset_p = (DBSet_t *)new_p;
    byte_t *newvalue_p;
    int new_index, newcnt;
    int old_index, oldcnt;

    ALIGN_CHECK(oldset_p);
    ALIGN_CHECK(newset_p);
    /* 
     * Determine the number of values which will remain in the
     * new set
     */
    oldcnt = oldset_p->ds_ocnt;
    newcnt = 0;
    for (old_index=0; old_index < oldcnt; ++old_index)
        {
        DBSetMember_t *oldvalue_p =
		(DBSetMember_t *)INDEX_DBSet(oldset_p, old_index);

        if ((EXT8(oldvalue_p->dm_valid)) ||
	    (dnsCmpCTS((dns_cts *)allupto_p, (dns_cts *)oldvalue_p->dm_ts) <= 0))
            /* Remove the value if marked absent before allupto */
            ++newcnt;
        }

    COPY_bytes(oldset_p, newset_p, DIFF_bytes(oldset_p->ds_offp, oldset_p));
    newset_p->ds_ocnt = newcnt;
    newset_p->ds_hcnt = 0;		/* no delete holders */

    /* Copy attributes and values */
    newvalue_p = DATA_DBSet(newset_p);
    new_index = 0;

    for (old_index = 0; old_index < oldcnt; old_index++)
        {
        DBSetMember_t *oldvalue_p =
		(DBSetMember_t *)INDEX_DBSet(oldset_p, old_index);

        /* Values marked absent before allupto can be removed */
        if (EXT8(oldvalue_p->dm_valid) ||
            (dnsCmpCTS((dns_cts *)allupto_p, (dns_cts *)oldvalue_p->dm_ts) <= 0))
	    {
	    /* Update index */
	    newvalue_p = ALIGN_PTR(newvalue_p);
	    newset_p->ds_offp[new_index] = DIFF_bytes(newvalue_p, newset_p);

	    /* Copy the value */
	    COPY_DBSetMember(oldvalue_p, newvalue_p);

	    new_index++; 
	    newvalue_p = SKIP_DBSetMember(newvalue_p);
	    }
        }

    /* Update set length */
    newset_p->ds_setl = DIFF_bytes(newvalue_p, newset_p);
    return(newvalue_p);
}

/*
 * db_cleanup_record
 *     This routine frees absent entries and purges absent space
 * 	from present entries.
 *
 * Input:
 *    rec_p = address of data descriptor
 *    allupto_p = address of timestamp which bounds purges.
 * 
 */
void 
db_cleanup_record (db_data_t    *rec_p,
                   Timestamp_t  *allupto_p)
{
    DBSet_t *old_mainset_p, *new_mainset_p;
    byte_t *new_p;
    int cnt, old_index, oldcnt;
    int new_index, newcnt, newhcnt = 0;

    /* Point at set of sets */
    old_mainset_p = (DBSet_t *)rec_p->data_p;
    ALIGN_CHECK(old_mainset_p);

    /* Has entry been deleted? */
    if (EXT8(old_mainset_p->ds_flag) != DS_present) return;

   /* 
    * Determine the number of attributes which will remain in the
    * new record 
    */
    oldcnt = old_mainset_p->ds_ocnt;
    newcnt = 0;
    for (old_index=0; old_index < oldcnt; ++old_index)
        {
	AttributeName_t *oldname_p =
		(AttributeName_t *)INDEX_DBSet(old_mainset_p, old_index);
	DBSet_t *oldset_p = (DBSet_t *)SKIP_AttributeName(oldname_p);

	ALIGN_CHECK(oldset_p);

        if ((EXT8(oldset_p->ds_flag) == DS_present) ||
		(dnsCmpCTS((dns_cts *)allupto_p, 
			   (dns_cts *)oldset_p->ds_ts) <= 0))
            /* Keep set if marked absent after allupto */
            newcnt++;
        }

    /* Build a new record */
    new_mainset_p = (DBSet_t *)dns_malloc_perm(rec_p->data_buf_len);
    if (!new_mainset_p) return;

    /* Copy stable part of record */
    ALIGN_CHECK(new_mainset_p);
    COPY_bytes(old_mainset_p, new_mainset_p,
	DIFF_bytes(old_mainset_p->ds_offp, old_mainset_p));

    new_mainset_p->ds_ocnt = newcnt;
    cnt = old_mainset_p->ds_hcnt;

    if (cnt)
	/* Have deleted record holders here, see if they can be removed */
	{
	delete_holder_t *delete_p =
		(delete_holder_t *)DATA_DBSet_Delete(old_mainset_p);

	new_p = DATA_DBSet_Delete(new_mainset_p);

	for (; 0 < cnt; delete_p++, --cnt)
          if (dnsCmpCTS((dns_cts *)delete_p->del_ts, (dns_cts *)allupto_p) > 0)
		{
		COPY_delete_holder(delete_p, new_p);
		new_p = SKIP_delete_holder(new_p);
		newhcnt++;
		}
	}
    else
	new_p = (byte_t *)&new_mainset_p->ds_offp[NUM_DBSet(new_mainset_p)];

    /* number of delete holders */
    new_mainset_p->ds_hcnt = newhcnt;

    /* Copy attributes and values */
    new_index = 0;
    for (old_index = 0; old_index < oldcnt; ++old_index)
        {
	AttributeName_t *oldname_p =
		(AttributeName_t *)INDEX_DBSet(old_mainset_p, old_index);
	DBSet_t *oldset_p = (DBSet_t *)SKIP_AttributeName(oldname_p);

	ALIGN_CHECK(oldset_p);

        /* Sets marked absent before allupto can be removed */
        if ((EXT8(oldset_p->ds_flag) == DS_present) ||
            (dnsCmpCTS((dns_cts *)allupto_p, (dns_cts *)oldset_p->ds_ts) <= 0))
	    {
	    /* DBSet following name must be aligned */
	    new_p = ALIGN_PTR(SKIP_bytes(new_p, LEN_AttributeName(oldname_p)));
	    new_p = SKIP_bytes(new_p, -LEN_AttributeName(oldname_p));

	    /* set offset to new entry */
	    new_mainset_p->ds_offp[new_index] = DIFF_bytes(new_p,new_mainset_p);

	    /* Copy name and skip past it */
	    COPY_AttributeName(oldname_p, new_p);
	    new_p = SKIP_AttributeName(new_p);

	    /* Cleanup absent values */
	    new_p = cleanup_members(oldset_p, new_p, allupto_p);
	    new_index++;
	    }
        }
    /* Update set length */
    new_mainset_p->ds_setl = DIFF_bytes(new_p, new_mainset_p);

    dns_free((char *)old_mainset_p); /* Release old buffer */
    rec_p->data_p = (data_record_t *)new_mainset_p;
}
