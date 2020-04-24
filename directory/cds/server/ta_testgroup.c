/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_testgroup.c,v $
 * Revision 1.1.6.2  1996/02/18  19:37:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:56  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:34:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:30  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:05:05  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:32:59  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:44:16  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:55  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:13:46  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:48:33  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:25:16  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:26:07  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:25:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module ta_testgroup.c
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
 */

#include <server.h>
#include <clerk.h>
#include <dbdef.h>
#include <find.h>
#include <names.h>
#include <security.h>
#include <sets.h>
#include <uid.h>

typedef struct loopdetector {
    struct loopdetector	*ld_next;
    ObjUID_t		*ld_uid_p;
    FullName_t		*ld_group_p;
    FullName_t		*ld_member_p;
} loopdetector_t;
#define MAX_LOOPS 100
static bytes_u loopbuf[sizeof(loopdetector_t)*MAX_LOOPS];
static loopdetector_t *first_ld_p;
static loopdetector_t *free_ld_p; 

/*
 * Local prototypes
 */
static dns_status_t
checkgrouploop (
    ObjUID_t		*,
    FullName_t		*,
    FullName_t		*);

static dns_status_t
setgrouploop (
    ObjUID_t		*,
    FullName_t		*,
    FullName_t		*);

static void
unsetgrouploop (
    ObjUID_t		*,
    FullName_t		*,
    FullName_t		*);

/*
 * initloop
 *    Initalize free list.
 *
 */
void 
initloop (void)
{
    loopdetector_t	*curld_p;
    int			cnt = MAX_LOOPS;

    first_ld_p = NULL;
    free_ld_p = (loopdetector_t *)loopbuf;
    curld_p = (loopdetector_t *)loopbuf;

    do
        curld_p->ld_next = &curld_p[1];
    while (curld_p++, --cnt);

    curld_p->ld_next = NULL;
}        

/* 
 * checkgrouploop
 *     Check if this transaction has already passed through this ta.
 * If so, we are probably in a loop.
 * 
 * Input:
 *     tid_p = address of transaction id
 *     group_p = address of group name
 *     member_p = address of member name
 *
 * Output:
 *     DNS_SUCCESS - not here
 *     DNS_POSSIBLECYCLE - found it ( and removed it )
 *
 */
static dns_status_t 
checkgrouploop (ObjUID_t    *tid_p,
                FullName_t  *group_p,
                FullName_t  *member_p)
{
    DEB_ASCII_BUF_ObjUID(tidBuf)
    DEB_ASCII_BUF_FullName(groupBuf)
    DEB_ASCII_BUF_FullName(memberBuf)
    loopdetector_t	*curld_p, *prvld_p;

    if (!first_ld_p)
        return(DNS_SUCCESS);

    curld_p = first_ld_p;
    prvld_p = NULL;

    while (curld_p)
        {
        if (EQ_ObjUID(curld_p->ld_uid_p, tid_p))
             if (dnsCmpFull((dns_opq_fname_t *)group_p, 
			    (dns_opq_fname_t *)curld_p->ld_group_p)==0)
                 if (dnsCmpFull((dns_opq_fname_t *)member_p, 
				 (dns_opq_fname_t *)curld_p->ld_member_p)==0)
		    {
                     /* The name is put in the list before
                        and searches are done for it, so 0 is legal */

                DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug5,
                    "  checkgrouploop(%s,%s,%s) match",
                    deb_ascii_ObjUID(tidBuf,tid_p),
                    deb_ascii_FullName(groupBuf,group_p),
                    deb_ascii_FullName(memberBuf,member_p)));

		    return(DNS_POSSIBLECYCLE);
		    }
        prvld_p = curld_p;
        curld_p = curld_p->ld_next;
        }                 

    return(DNS_SUCCESS);
}

/*
 * setgrouploop
 *     Insert a test in the list for possible loop detection.
 *
 * Input:
 *     tid_p = address of transaction id
 *     group_p = address of group
 *     member_p = address of member
 *
 */
static dns_status_t 
setgrouploop (ObjUID_t    *tid_p,
              FullName_t  *group_p,
              FullName_t  *member_p)
{
    DEB_ASCII_BUF_ObjUID(tidBuf)
    DEB_ASCII_BUF_FullName(groupBuf)
    DEB_ASCII_BUF_FullName(memberBuf)
    loopdetector_t	*curld_p;

    curld_p = free_ld_p;          

    if (!curld_p)
        return(DNS_NONSRESOURCES);

    free_ld_p = curld_p->ld_next;
    curld_p->ld_uid_p = tid_p;
    curld_p->ld_group_p = group_p;
    curld_p->ld_member_p = member_p;
    curld_p->ld_next = first_ld_p;
    first_ld_p = curld_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        "  setgrouploop(%s,%s,%s) entered",
        deb_ascii_ObjUID(tidBuf,tid_p),
        deb_ascii_FullName(groupBuf,group_p),
        deb_ascii_FullName(memberBuf,member_p)));

    return(DNS_SUCCESS);
}

/*
 * unsetgrouploop
 *    Remove an entry from the loop detect list
 *
 * Input:
 *    tid_p = address of tid of entry to remove
 *    group_p = address of group
 *    member_p = address of member
 *
 */
static void 
unsetgrouploop (ObjUID_t    *tid_p,
                FullName_t  *group_p,
                FullName_t  *member_p)
{
    DEB_ASCII_BUF_ObjUID(tidBuf)
    DEB_ASCII_BUF_FullName(groupBuf)
    DEB_ASCII_BUF_FullName(memberBuf)
    loopdetector_t	*curld_p, *prvld_p;

    if (!first_ld_p)
        return;

    curld_p = first_ld_p;
    prvld_p = NULL;

    while (curld_p)
        {
        if (EQ_ObjUID(curld_p->ld_uid_p, tid_p))
            if (dnsCmpFull((dns_opq_fname_t *)group_p, 
			    (dns_opq_fname_t *)curld_p->ld_group_p)==0)
                if (dnsCmpFull((dns_opq_fname_t *)member_p, 
				(dns_opq_fname_t *)curld_p->ld_member_p)==0)
                    {
                    if (!prvld_p)
                        first_ld_p = curld_p->ld_next;
                    else
                        prvld_p->ld_next = curld_p->ld_next;

                    curld_p->ld_next = free_ld_p;
                    free_ld_p = curld_p;

                DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug5,
                    "  unsetgrouploop(%s,%s,%s) removed",
                    deb_ascii_ObjUID(tidBuf,tid_p),
                    deb_ascii_FullName(groupBuf,group_p),
                    deb_ascii_FullName(memberBuf,member_p)));

                    break;
                    }
                prvld_p = curld_p;
                curld_p = curld_p->ld_next;
        }                 
}

/*
 * ta_testgroup
 *   This routine tests a group for membership.  It handles members which
 * are groups themselves.
 *
 * Input:
 *      user_p = address of user_descriptor
 *      progress_p = address of progress record
 *      member_p = member full name pointer
 *      direct_p = address of boolean
 *      detect = boolean for recurision 
 *      retlen_p = address to return length of results
 *
 * Function value:
 *      status
 *
 * Side effects: 
 *      Progress record updated
 */
dns_status_t 
ta_testgroup (user_descriptor_t  *user_p,
              Progress_t         *progress_p,
              FullName_t         *member_p,
              ObjUID_t           *detect_p,
              int                direct,
              int                *retlen_p)
{
    db_stream_t		*stream_p;
    DBSet_t		*set_p, *set2_p;
    DBSetMember_t	*value_p;
    GroupMember_t	*grmember_p;
    byte_t		*result_p,*direct_p;
    ObjUID_t		*newdetect_p;
    Timeout_t		*timeout_p;
    FullName_t		*group_p;
    int			cnt,pos,index;
    dns_status_t	retstatus, status;
    
    retstatus = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_ReadAttribute);
    if (retstatus != DNS_SUCCESS) return(retstatus);

    retstatus = walktree(user_p, ET_object, WALKTREE_LOOKUP_OP, progress_p, stream_p);
    if (retstatus != DNS_SUCCESS) return(retstatus);

    /* Want output in malloced area */
    timeout_p = (Timeout_t *)SKIP_Progress(progress_p);
    COPY_Timeout(NullTimeout, timeout_p);
    direct_p = SKIP_Timeout(timeout_p);
    newdetect_p = (ObjUID_t *)SKIP_BOOLEAN(direct_p);
    COPY_ObjUID(detect_p, newdetect_p);
    result_p = SKIP_ObjUID(newdetect_p);
    *retlen_p = SKIP_BOOLEAN(result_p) - (byte_t *)progress_p;

    /* Group is resolved name */
    group_p = (FullName_t *)SKIP_FullName(progress_p->pr_unresolvedName);
    if (ObjUID_is_nil(newdetect_p))
        id_new(newdetect_p);
    else /* see if we have been here before */
        retstatus = checkgrouploop(newdetect_p, group_p, member_p);

    if (retstatus != DNS_SUCCESS) goto group_done;

    /* Make sure the object is a group */
    if (!set_dnstest(stream_p->db_entry.data_p,
		LEN_ClassName(cds_classlist[DNS_GROUP].cname), 
		cds_classlist[DNS_GROUP].cname, dns_Attribute(DNS_CLASS)))
       {
       retstatus = DNS_NOTAGROUP;
       goto group_done;
       }

    /* check user access to object */
    retstatus = setgrouploop(newdetect_p, group_p, member_p);
    if (retstatus != DNS_SUCCESS) goto group_done;
    retstatus = security_check(user_p, stream_p, Test_check, ET_object, 
			       newdetect_p);

    unsetgrouploop(newdetect_p, group_p, member_p);

    if (retstatus == DNS_UNKNOWNENTRY)
        /* Rebuild resolved name for exception */
        resolved_null(progress_p);

    if (retstatus != DNS_SUCCESS) goto group_done;


    /* At this point retstatus == DNS_SUCCESS */     
    INS8(result_p, BL_false);
    if ((find_attribute(stream_p->db_entry.data_p,
		dns_AttributeName(DNS_MEMBER),
		&set_p, &index)) != DNS_SUCCESS) goto group_done;

    cnt = NUM_DBSet(set_p);
    for (pos = 0; pos < cnt; pos++)
        {
        value_p = (DBSetMember_t *)INDEX_DBSet(set_p, pos);
        if (!EXT8(value_p->dm_value_present) || !EXT8(value_p->dm_valid))
            continue;
        grmember_p = (GroupMember_t *)value_p->dm_data;
        if (EXT8(grmember_p->gm_isaGroup))
	    continue;
        if (!names_matchwild(member_p, (FullName_t *)grmember_p->gm_member))
            continue;

        INS8(result_p, BL_true);
        INS8(direct_p, BL_true);
        status = find_attribute(stream_p->db_entry.data_p, 
		dns_AttributeName(DNS_GROUPREVOKE), &set2_p, &index);
        if (status != DNS_SUCCESS)
            COPY_Timeout(NullTimeout, timeout_p);
        else
	    {
	    DBSetMember_t *dm_p = (DBSetMember_t *)DATA_DBSet(set2_p);
	    if (dm_p = find_last_value(set2_p))
	      COPY_Timeout(dm_p->dm_data, timeout_p);
	    else
	      COPY_Timeout(NullTimeout, timeout_p);
	    }
        goto group_done;
        } /* end for loop */

    /* All done, not a member. *result_p should be FALSE */
    if (direct)
	goto group_done;

    retstatus = setgrouploop(newdetect_p, group_p, member_p);
    if (retstatus != DNS_SUCCESS) goto group_done;

    /* Now check members which are groups themselves */
    cnt = NUM_DBSet(set_p);
    for (pos = 0; pos < cnt; pos++)
        {
        /* Stop processing if clearinghouse shutting */
        if (stream_p->db_file_p->file_ch_p->ch_state == dns_Broken)
            {
            retstatus = DNS_CLEARINGHOUSEDOWN;
            break;
            }
        value_p = (DBSetMember_t *)INDEX_DBSet(set_p, pos);
        if (!EXT8(value_p->dm_value_present) || !EXT8(value_p->dm_valid))
            continue;
        grmember_p = (GroupMember_t *)value_p->dm_data;
        if (!EXT8(grmember_p->gm_isaGroup))
	    continue;

	/* This value is in and out to clerk_testgroup */
	INS8(direct_p, BL_false);
	status = clerk_testgroup((FullName_t *)grmember_p->gm_member,
			member_p, (char *)direct_p, newdetect_p, timeout_p);
        if (status == DNS_POSSIBLECYCLE)
            {
            retstatus = DNS_POSSIBLECYCLE;
            break;
            }
        if (status != DNS_SUCCESS) continue;

        INS8(result_p, BL_true);
        INS8(direct_p, BL_false);
        if (find_attribute(stream_p->db_entry.data_p,
			dns_AttributeName(DNS_GROUPREVOKE),
			&set_p, &index) == DNS_SUCCESS)
	    {
	    DBSetMember_t *dm_p = (DBSetMember_t *)DATA_DBSet(set_p);

	    COPY_Timeout(min_timeout((Timeout_t *)dm_p, timeout_p), timeout_p);
	  }
	break;
      } /* End for loop */
group_done1:
    unsetgrouploop(newdetect_p, group_p, member_p);

group_done:
    db_close_stream(stream_p);
    return(retstatus);
}


