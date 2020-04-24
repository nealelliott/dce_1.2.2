/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sets_lib.c,v $
 * Revision 1.1.12.1  1996/08/09  12:00:48  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  15:54 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	Two changes:
 * 	1) set_insert_member optimizations (getting rid of old, unnecessary data)
 * 	from Janet.
 *
 * 	2) OT 12767 fix: In set_insert_member, acls weren't matching because of
 * 	padding problems.  As a result, the ACL list kept growing.  Quick fix
 * 	is to check the UIDs of the ACLs, and if they are the same, just return.
 * 	Also, changed comparison value in set_replace_value from "> AFTER" to
 * 	">= 0".
 *
 * 	[1996/02/18  19:36:43  marty  1.1.10.2]
 *
 * Revision 1.1.10.2  1996/02/18  19:36:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:24  marty]
 * 
 * Revision 1.1.10.1  1995/12/08  15:32:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:55  root]
 * 
 * Revision 1.1.6.4  1994/08/16  18:28:04  jd
 * 	drop from janet
 * 	[1994/08/13  16:06:58  jd]
 * 
 * Revision 1.1.6.3  1994/08/03  19:04:40  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:28:17  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:43:53  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:27  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:12:35  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:45:40  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  14:23:25  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:24:01  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/04  19:09:53  keegan
 * 	set_insert function did return wrongattributetype if doing an add
 * 	operation on a single valued attribute but not if doing a remove
 * 	operation.  Also, set_insert created a new empty set if user was
 * 	trying to remove a non-existing attribute... now set_insert returns
 * 	success without creating anything.
 * 	[1992/12/04  19:08:27  keegan]
 * 
 * Revision 1.1.2.3  1992/07/11  16:48:56  peckham
 * 	OT#4649: ACLs on ET_clearinghouses are AT_single, which causes
 * 	no history.  They vary in length and can shrink, and when they do,
 * 	they trigger non-functionality in set_shiftdata() which causes
 * 	the previous attribute to be overwritten, and then the set
 * 	doesn't pass muster in att_set_check().
 * 	[1992/07/11  16:47:54  peckham]
 * 
 * Revision 1.1.2.2  1992/03/22  22:34:09  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:36:14  weisman]
 * 
 * Revision 1.1  1992/01/19  15:25:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: sets_lib.c
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

#include <address.h>
#include <dce/assert.h>
#include <back.h>
#include <dbdef.h>
#include <find.h>
#include <names.h>
#include <sets.h>
#include <uid.h>


/*
 * For CDS builds, compare using normal fullname routines.
 * For DNS builds, compare fullnames ignoring wildcard semantics.
 */
# define COMPARE_ACE_NAMES(v1,v2) dnsCmpFull(v1,v2)


/*
 * Local prototypes
 */

static dns_status_t
set_shiftdata (
    const int		,
    byte_t *const	,
    const int		,
    db_data_t *const	,
    const int		);

static dns_status_t
set_insert_member (
    db_data_t *const	        ,
    DBSet_t		        *,
    const Update_t         *const,
    const int		         ,
    attribute_descriptor_t *const );

static dns_status_t
set_new (
    db_data_t *const	 ,
    DBSet_t		 *,
    const int	         ,
    const Update_t  *const,
    AttributeName_t *const,
    const unsigned     int);

#if defined(DCE_CDS_DEBUG)
# define SET_CHECK
#endif

#ifdef SET_CHECK
/*
 * val_set_check
 */
static void 
val_set_check (const DBSet_t *const set_p)
{
    register byte_t *next_p = (byte_t *)&set_p->ds_offp[set_p->ds_ocnt];
    register int i;
    byte_t *temp;

    ALIGN_CHECK(set_p);
    switch (set_p->ds_flag[0]) {
    case DS_present:
    case DSX_absent:
	break;
    default:
	dce_svc_printf(CDS_S_SERVER_SETBAD_A_MSG);
    }
    switch (set_p->ds_type[0]) {
    case AT_null:
    case AT_single:
    case AT_set:
	break;
    default:
	dce_svc_printf(CDS_S_SERVER_SETBAD_A_MSG);
    }
    if (0 < (i = set_p->ds_hcnt)) {
	next_p = ALIGN_PTR(next_p);

	do
	    next_p += sizeof(delete_holder_t);
	while (0 < --i);
    }
    for (i=0; i< set_p->ds_ocnt; i++) {
	const DBSetMember_t *const dm_p =
				(DBSetMember_t *)INDEX_DBSet(set_p,i);

	dce_assert(cds__svc_handle,
		   (next_p <= (byte_t *)dm_p));
	ALIGN_CHECK(dm_p);
	switch (dm_p->dm_flag[0]) {
	case UD_present:
	case UD_absent:
	    break;
	default:
	    dce_svc_printf(CDS_S_SERVER_SETBAD_A_MSG);
	}
	switch (dm_p->dm_type[0]) {
	case AT_null:
	case AT_single:
	case AT_set:
	    break;
	default:
	    dce_svc_printf(CDS_S_SERVER_SETBAD_A_MSG);
	}
	switch (dm_p->dm_valid[0]) {
	case BL_true:
	case BL_false:
	    break;
	default:
	    dce_svc_printf(CDS_S_SERVER_SETBAD_A_MSG);
	}
	switch (dm_p->dm_value_present[0]) {
	case BL_true:
	case BL_false:
	    break;
	default:
	    dce_svc_printf(CDS_S_SERVER_SETBAD_A_MSG);
	}
	next_p = SKIP_DBSetMember(dm_p);
    }
    temp = ALIGN_PTR(SKIP_DBSet(set_p));
    dce_assert(cds__svc_handle,
	       (next_p <= temp));
}

/*
 * att_set_check
 */
void 
att_set_check (const DBSet_t *const set_p)
{
    register byte_t *next_p = (byte_t *)&set_p->ds_offp[set_p->ds_ocnt];
    register int i;
    byte_t *temp;

    switch (set_p->ds_flag[0]) {
    case DS_present:
    case DSX_absent:
	break;
    default:
	dce_svc_printf(CDS_S_SERVER_SETBAD_A_MSG);
    }
    switch (set_p->ds_type[0]) {
    case AT_null:
    case AT_single:
    case AT_set:
	break;
    default:
	dce_svc_printf(CDS_S_SERVER_SETBAD_A_MSG);
    }
    if (0 < (i = set_p->ds_hcnt)) {
	next_p = ALIGN_PTR(next_p);

	do
	    next_p += sizeof(delete_holder_t);
	while (0 < --i);
    }
    for (i=0; i< set_p->ds_ocnt; i++) {
	const AttributeName_t *const an_p =
				(AttributeName_t *)INDEX_DBSet(set_p,i);
	const DBSet_t *const ds_p =
				(DBSet_t *)SKIP_AttributeName(an_p);

	dce_assert(cds__svc_handle,
		   (next_p <= (byte_t *)an_p));
	switch (an_p->an_flag[0]) {
	case SN_normal:
	case SN_quoted:
	case SN_binary:
	case SN_objectid:
	    break;
	default:
	    dce_svc_printf(CDS_S_SERVER_NAMEBAD_A_MSG);
	}
	ALIGN_CHECK(ds_p);
	val_set_check(ds_p);
	next_p = SKIP_DBSet(ds_p);
    }
    temp = ALIGN_PTR(SKIP_DBSet(set_p));
    dce_assert(cds__svc_handle,
	       (next_p <= temp));
}

static void 
db_data_check (const db_data_t *const rec_p)
{
    unsigned int temp;

    if (rec_p->data_p)
    {
	temp = LEN_DBSet(rec_p->data_p);
	dce_assert(cds__svc_handle,
		   (temp <= rec_p->data_buf_len));
    }
}
#else

# define val_set_check(set_p) (set_p)
# define db_data_check(rec_p) (rec_p)

#endif

/*
 * set_timestamp_set
 *      This routine updates an attribute which is itself
 * a timestamp.  It updates the record in memory, it doesn't
 * write it to disk.
 *
 * Input:
 *      dir_p = address of db_data
 *      ts_p = address of ts for value
 *      dns_p = address of attribute descriptor entry
 *	upd_p = address of update buffer
 *	len_p = address to return length of update
 *
 * Output: none
 *
 * Value:
 *      DNS_SUCCESS if updated
 *      DNS_NONSRESOURCES if can't do it
 *
 */
dns_status_t 
set_timestamp_set (db_data_t               *dir_p,
                   Timestamp_t             *ts_p,
                   attribute_descriptor_t  *dns_p,
                   Update_t                *upd_p,
                   int                     *len_p)
{
    byte_t		updbuf[sizeof(Update_u) - MAXATTRIBUTE +
				sizeof(Timestamp_u)];
    int			len;

    if (!upd_p)
	{
	len_p = &len;
	upd_p = (Update_t *)updbuf;
	}

    if (!dns_p->rep_rec)
	{
	*len_p = update_dns_build(upd_p, UD_present, ts_p, dns_p,
				  BL_true,sizeof(*ts_p), (byte_t *)ts_p);

        return(set_insert(dir_p, upd_p, dns_p, FALSE));
	}
    else
	{
	*len_p = update_dns_build(upd_p, UD_present, NullTimestamp,
				  dns_p, BL_true,
				  sizeof(*ts_p), (byte_t *)ts_p);

        return(set_replace_value(dir_p, upd_p, dns_p));
	}
}

/*
 * set_member
 *      Build a set with one value.
 *
 * Input:
 *       out_set_pp = address of address to return set to
 *       att_p = attribute pointer in neutral case
 *	 casemask = mask of attribute input case
 *       ts_p = address of ts
 *       data_p = address of value
 *       len    = len of value
 *       valuetype = type of value
 *       valpresent = is value present
 *       valstate = state of value
 *       type = type of set
 *
 * Output:
 *       updated output address returned to out_set_pp
 * Value:
 *        Length of attribute set
 */
dns_status_t 
set_member (DBSet_t          **out_set_pp,
            AttributeName_t  *att_p,
            unsigned int     casemask,
            Timestamp_t      *ts_p,
            byte_t           *data_p,
            int              len,
            int              valuetype,
            int              valpresent,
            int              valstate,
            int              type)
{
    AttributeName_t	*an_p = (AttributeName_t *)*out_set_pp;
    register DBSetMember_t *value_p;
    register DBSet_t	*set_p;

    /* Copy name and point past it */
    COPY_AttributeName(att_p, an_p);
    set_p = (DBSet_t *)SKIP_AttributeName(an_p);
    ALIGN_CHECK(set_p);

    INS8(set_p->ds_flag, DS_present);
    INS8(set_p->ds_type, type);
    COPY_Timestamp(ts_p, set_p->ds_ts);
    INS32(set_p->ds_casemask, casemask);
    set_p->ds_hcnt = 0;			/* no delete holders */

    if ((len == 0) && !data_p)
        {
        /* build an empty set */
        set_p->ds_ocnt = 0;
        value_p = (DBSetMember_t *)DATA_DBSet(set_p);
        }
    else
        {
        set_p->ds_ocnt = 1;

        /* Build value header */
        value_p = (DBSetMember_t *)ALIGN_PTR(DATA_DBSet(set_p));
        set_p->ds_offp[0] = DIFF_bytes(value_p, set_p);
        INS8(value_p->dm_flag, valstate);
	INS8(value_p->dm_valid, (valstate == DM_present));
        INS8(value_p->dm_type, type);
        INS8(value_p->dm_value_present, valpresent);
        COPY_Timestamp(ts_p, value_p->dm_ts);
	INS8(value_p->dm_valuetype, valuetype);
	value_p->dm_length = len;

        if (0 < len)
            COPY_bytes(data_p, value_p->dm_data, len);

        if ((type == AT_single) && (valstate != DM_present))
	    INS8(set_p->ds_flag, DSX_absent);

	value_p = (DBSetMember_t *)SKIP_DBSetMember(value_p);
        }

    set_p->ds_setl = DIFF_bytes(value_p, set_p);
    val_set_check(set_p);
    /* Return pointer for next set */
    *out_set_pp = set_p = (DBSet_t *)SKIP_DBSet(set_p);

    /* Return set length */
    return DIFF_bytes(set_p, an_p);
}

/*
 * set_valuetest
 *     This routine compares a value with the members of a set
 * and returns a pointer if a match is found or NULL.
 *
 * Input:
 *     set_p = address of the set
 *     len = size of input value
 *     testvalue_p = address of input value
 *     vtype = data type of value
 *     index_p = address of index to start with
 *     dns_p = address of known attribute
 *
 * Output:
 *     index_p = index of match
 *
 * Value:
 *      pointer to match or NULL
 */
DBSetMember_t 
*set_valuetest (DBSet_t                 *set_p,
                int                     len,
                byte_t                  *testvalue_p,
                int                     vtype,
                int                     *index_p,
                attribute_descriptor_t  *dns_p)
{
    DBSetMember_t	*member_p;
    byte_t		*val1_p, *val2_p;
    int			cnt, len2;

    cnt = set_p->ds_ocnt;
    for (; *index_p < cnt; ++(*index_p))
        {
        member_p = (DBSetMember_t *)INDEX_DBSet(set_p, *index_p);
        if (!EXT8(member_p->dm_value_present)) continue;
	if (EXT8(member_p->dm_valuetype) != vtype) continue;
	/* RE-set pointer */
	val1_p = (byte_t *)testvalue_p;
	val2_p = (byte_t *)member_p->dm_data;
        len2 = LEN_DBSetMemberData(member_p);

	switch ((int)vtype) {
        case VT_uuid:
	  if (EQ_ObjUID((ObjUID_t *)val1_p, (ObjUID_t *)val2_p))
	    return(member_p);
	  break;
	case VT_GroupMember:
	  if (EXT8(((GroupMember_t *)val2_p)->gm_isaGroup) ==
	      EXT8(((GroupMember_t *)val1_p)->gm_isaGroup))
	    if (dnsCmpFull(
		    (dns_opq_fname_t *)((GroupMember_t *)val2_p)->gm_member,
		    (dns_opq_fname_t *)((GroupMember_t *)val1_p)->gm_member)==0)
	      return(member_p);
	  break;
	case VT_FullName:
	  if (dnsCmpFull((dns_opq_fname_t *)val2_p,
			 (dns_opq_fname_t *)val1_p)==0)
	    return(member_p);
	  break;
	case VT_ParentPointer:
	  /* Only check nsid of fullname, one parent / namespace */
	  val2_p = ((ParentPointer_t *)val2_p)->pp_myName;
	  val1_p = ((ParentPointer_t *)val1_p)->pp_myName;
	  if (EQ_ObjUID(((FullName_t *)val2_p)->fn_root,
			((FullName_t *)val1_p)->fn_root))
	    return(member_p);
	  break;
	case VT_CHDirectory:
	  /* Only test for the id get this far */
	  if (EQ_ObjUID(((CHDirectoryPointer_t *)val1_p)->cp_dirID,
			((CHDirectoryPointer_t *)val2_p)->cp_dirID))
	    return(member_p);
	  break;
#ifdef VT_gdaPointer
	case VT_gdaPointer:
	  /* don't check timeout */
	  val1_p = ((gdaPointer_t *)val1_p)->gp_replica;
	  val2_p = ((gdaPointer_t *)val2_p)->gp_replica;
	  /* fall through to check ReplicaPointer */
#endif
	case VT_ReplicaPointer:
	  if (EQ_ObjUID(((ReplicaPointer_t *)val2_p)->rp_CHID,
			((ReplicaPointer_t *)val1_p)->rp_CHID)) {
	    /*
	     * don't check the clearinghouse's address
	     */
	    val2_p = SKIP_Set(((ReplicaPointer_t *)val2_p)->rp_addressHint);
	    val1_p = SKIP_Set(((ReplicaPointer_t *)val1_p)->rp_addressHint);
	    if (EXT8(val2_p) == EXT8(val1_p))
	      if (dnsCmpFull(
			     (dns_opq_fname_t *)SKIP_ReplicaType(val2_p),
			     (dns_opq_fname_t *)SKIP_ReplicaType(val1_p))==0)
		return(member_p);
	  }
	  break;
	case VT_byte:
	  if ((dns_p) && (dns_p->atype == DNS_CLASS)) {
	    if (dnsCmpSimple((dns_opq_sname_t *)val2_p,
			     (dns_opq_sname_t *)val1_p) == 0)
	      return(member_p);
	    else break;
	  }
	  /* Fall through */
	default:
	  if ((len == len2) && EQ_bytes(member_p->dm_data, testvalue_p, len))
	    return(member_p);
	  break;
	} /* End switch */
      } /* End for loop */
    return(NULL);
}

/*
 * set_dnstest
 *    This routine locates an attribute and tests its values against a user
 * supplied value.  It should only be used to test DNS attributes for
 * internal code compares.
 *
 * Input:
 *    buf_p = address of record
 *    len = length of attribute
 *    testvalue_p = address of value
 *    dns_p = address of attribute descripor
 *
 * Output:
 *    None
 *
 * Value:
 *    True if match found, False otherwise
 */
int 
set_dnstest (data_record_t           *buf_p,
             int                     len,
             byte_t                  *testvalue_p,
             attribute_descriptor_t  *dns_p)
{
    DBSet_t *set_p;
    int idx;

    /* First locate the attribute */
    if (find_attribute(buf_p, (AttributeName_t *)dns_p->aname,
		     &set_p, &idx) == DNS_SUCCESS)
	/* Test value with set members */
	for (idx = 0; ; idx++) {
	    DBSetMember_t *const sm_p = set_valuetest(set_p,
			len, testvalue_p, dns_p->vtype, &idx, dns_p);

	    if (!sm_p) break;
	    if (EXT8(sm_p->dm_valid)) return(TRUE);
	}
    return(FALSE);
}

/*
 * set_test
 *    This routine locates an attribute and tests its values against a user
 * supplied value. Compare use supplied values.
 *
 * Input:
 *    buf_p = address of record
 *    attribute_p = address of attribute simple name
 *    av_p = address of atomicvalue
 *
 * Output:
 *    None
 *
 * Value:
 *    True if match found, False otherwise
 */
int 
set_test (data_record_t    *buf_p,
          AttributeName_t  *attribute_p,
          AtomicValue_t    *av_p)
{
    DBSet_t		*set_p;
    int			idx;

    /* First locate the attribute */
    if (find_attribute(buf_p, attribute_p, &set_p, &idx) == DNS_SUCCESS)
	/* Test value with set members */
        for (idx = 0; ; idx++) {
	    DBSetMember_t *const sm_p =
			set_valuetest(set_p, EXT16(av_p->av_length),
				      av_p->av_value,
				      EXT8(av_p->av_valuetype),
				      &idx,
				      (attribute_descriptor_t *)0);
	    if (!sm_p) break;
            if (EXT8(sm_p->dm_valid)) return(TRUE);
	}

    return(FALSE);
}

/*
 * set_shiftdata
 *     This routine makes room for a new attribute value and
 * maybe a new attribute.
 *
 * Input:
 *     type = new attribute or just a new value.
 *     start_p = address to start moving
 *     len = length (+ or -) to shift data
 *     rec_p = address of data descriptor
 *     idx = In the case where we are adding an attribute, index
 *             is the offset for the new attribute.  If we are just
 *             increasing the space for an attribute, it is the
 *             index of the next attribute.
 *
 * Output:
 *    data shifted in record
 *    may be a new record allocated.
 * Value:
 *     DNS_SUCCESS - shift completed
 *     DNS_NONSRESOURCES - ran out of room
 */
static dns_status_t 
set_shiftdata (const int         type,
               byte_t *const     start_p,
               const int         len,
               db_data_t *const  rec_p,
               const int         idx)
{
    const DBSet_t	*oldset_p = (DBSet_t *)rec_p->data_p;
    const byte_t *const delete_p = DATA_DBSet_Delete(oldset_p);
    data_record_t	*newrec_p = rec_p->data_p;
    register DBSet_t	*newset_p;
    register byte_t	*split_p = start_p;
    register byte_t	*ptr_p;
    register int	cnt = oldset_p->ds_ocnt;
    register int	expand = 0;
    register int	oldlen;
    const int		move_len = ALIGN_LEN(len);
    int			oldsize, newsize;
    byte_t		*area_start, *area_end;

    ALIGN_CHECK(oldset_p);

    /* Make sure start_p is within the oldset_p data area */
    area_start = INDEX_DBSet(oldset_p, 0);
    area_end = SKIP_DBSet(oldset_p);
    dce_assert(cds__svc_handle,
	       ((area_start <= start_p) && (start_p <= area_end)));

    /* Include extra offset word if adding an attribute */
    if (type)
	{
	ptr_p = INDEX_DBSet(oldset_p, 0);
	if (0 < oldset_p->ds_hcnt)
	    ptr_p = (byte_t *)delete_p;
	if (ptr_p < (byte_t *)&oldset_p->ds_offp[cnt+1])
	    expand = ALIGN_LEN(sizeof(oldset_p->ds_offp[cnt]));
	}

    oldsize = LEN_DBSet(oldset_p);
    newsize = oldsize + expand + move_len;

    if (rec_p->data_buf_len < newsize)
        {
	const int new_buf_len = (newsize/BLOCK_SIZE+1)*BLOCK_SIZE;

	/* Allocate a multiple of a block */
        newrec_p = (data_record_t *)dns_malloc_perm(new_buf_len);
	if (!newrec_p)
            return(DNS_NONSRESOURCES);

        rec_p->data_buf_len = new_buf_len;
        /* copy header information first */
        COPY_bytes(rec_p->data_p, newrec_p, DIFF_bytes(delete_p, rec_p->data_p));
        }

    newset_p = (DBSet_t *)newrec_p;
    ALIGN_CHECK(newset_p);

    /* if we are shrinking, don't copy deleted data */
    if (move_len < 0)
	split_p -= move_len;

    /* copy information from split_p to SKIP_DBSet(oldset_p) */
    oldlen = DIFF_bytes(SKIP_DBSet(oldset_p), split_p);
    if (0 < oldlen)
	{
	ptr_p = SKIP_bytes(newset_p,
		DIFF_bytes(split_p, oldset_p) + expand + move_len);
	MOVE_bytes(split_p, ptr_p, oldlen);
	}
#ifdef DCE_CDS_DEBUG
    else
	dce_assert(cds__svc_handle,
		   (0 <= oldlen));
#endif

    /* copy information from top of offsets to start_p */
    ptr_p = (byte_t *)&oldset_p->ds_offp[cnt];
    oldlen = DIFF_bytes(start_p, ptr_p);
    if (0 < oldlen)
	{
	split_p = SKIP_bytes(newset_p,
			DIFF_bytes(ptr_p, oldset_p) + expand);
	MOVE_bytes(ptr_p, split_p, oldlen);
	}
#ifdef DCE_CDS_DEBUG
    else
	dce_assert(cds__svc_handle,
		   (0 <= oldlen));
#endif

    /* alter offsets to reflect new data positions */
    if (type)   /* adding an attribute */
	{
	/* idx -> new attribute */
        if (idx == cnt)
	    newset_p->ds_offp[cnt--] = newset_p->ds_setl + expand;
	else for (; idx < cnt; --cnt)
            newset_p->ds_offp[cnt] = newset_p->ds_offp[cnt-1]+expand+move_len;

	for (; 0 <= cnt; --cnt)
	    newset_p->ds_offp[cnt] += expand;

	newset_p->ds_ocnt++;
	}
    else	/* modifying an attribute */
	{
	/* idx -> following attribute */
        while (--cnt, (idx <= cnt))
            newset_p->ds_offp[cnt] += move_len;
	}

    /* update set length */
    newset_p->ds_setl += expand + move_len;
    db_data_check(rec_p);

    if (newrec_p != rec_p->data_p)
        {
        dns_free((char *)rec_p->data_p);
        rec_p->data_p = newrec_p;
        }

    return(DNS_SUCCESS);
}

/*
 * set_insert_member
 *     This routine is called to insert a new member into an
 * attribute set.
 *
 * There are 2 types of attributes: AT_single and AT_set
 * All attributes are stored in the DBSet_t structure.
 * In order to keep replicas up to date, enough state from
 * past transactions must be maintained so that replicas
 * can build the current state of an attribute.  (replicas
 * may not have seen all updates cause of failed propagations).
 * We never know the exact state of a replica, which updates
 * it has seen and which ones it missed.  After a skulk,
 * we can remove old state cause only then do we guarentee
 * that the replica has seen the updates. 
 * 
 * Members of a DBSet are added in timestamp order.  After a member
 * is inserted, we must update the state of other set members
 * cause updates may invalidate earlier updates.
 *    An update of type AT_single invalidates all previous 
 *         set members.
 *    An update to remove an attribute (type AT_set with 
 *         no value and an operation of UP_absent) invalidates 
 *         all previous set members.
 *    An update of type AT_set which adds or removes a value
 *         invalidates all previous occurances of the same value.
 *    An update that changes the set type, invalidates all
 *         previous set members.
 * 
 * Prior to DCE1.2, CDS kept ALL transactions.  It didn't clean
 * out invalidated members until after a skulk.  Because servers are
 * doing export/unexport operations on startup/shutdown this
 * can generate many updates of basically the same data.
 * add towers/ remove same towers/ add towers/ remove same towers.  If
 * skulks aren't completing cause of network connectivity,
 * server entries could grow quite large.  It turns out that
 * not all of the past transactions that have been invalidated
 * need to be maintained.  
 *
 *   Need to keep:
 *          1. The last AT_single update or AT_set remove attribute
 *                 (don't need both, only the last)
 *          2. The last update for a specific value unless
 *                 invalidated by above (1.)
 * Everything else that has been invalidated can be removed NOW.
 * 
 *
 * Input:
 *     rec_p = address of record descriptor
 *     set_p = address of attribute
 *     upd_p = address of update structure
 *     setindex = index of set
 *     dns_p = address of attribute descriptor
 *
 * Output:
 *     member inserted
 *
 * Value:
 *     DNS_SUCCESS
 *     DNS_NONSRESOURCES
 */
static dns_status_t 
set_insert_member (db_data_t *const               rec_p,
                   DBSet_t                        *set_p,
                   const Update_t *const          upd_p,
                   const int                      setindex,
                   attribute_descriptor_t *const  dns_p)
{
    DBSet_t		*main_p = (DBSet_t *)rec_p->data_p;
    DBSetMember_t	*oldvalue_p, *newvalue_p;
    DBSetMember_t	*value_p = NULL;
    byte_t              *val_p, *ptr_p;
    int			expand = 0, newlen, newindex, cnt, idx, totallen;
    int			valuePresent, valueLength;
    dns_status_t	status;
    byte_t              valueType;

    att_set_check(main_p);
    cnt = set_p->ds_ocnt;
    /* disect the update value */
    val_p = SKIP_AttributeSpecifier(upd_p->ud_attribute);
    valuePresent = GET8(val_p);
    valueLength = GET16(val_p);
    if (valuePresent == BL_false) {
	valueType = VT_none;
	valueLength = 0;
    } else
	valueType = GET8(val_p);

    /* Find where new value should go */
    find_ts_position(set_p, (Timestamp_t *)upd_p->ud_timestamp,
	&oldvalue_p, &newindex);

    if (valuePresent && dns_p) {
	/* OT 12767: This is a hack to fix the problem where
	 * acls aren't matching cause of padding.
	 * without writable copies, we don't need all the history
	 * associated with single_valued attributes anyway.
	 */
	if ((dns_p->vtype == VT_DACL) && newindex > 0) {
	    value_p = (DBSetMember_t *)INDEX_DBSet(set_p, newindex-1);
	    if (EQ_Timestamp(value_p->dm_ts, upd_p->ud_timestamp))
	      return(DNS_SUCCESS);
	}
    }

    /* if not first value */
    if (newindex != 0) {
      idx = newindex -1;
      value_p = (DBSetMember_t *)INDEX_DBSet(set_p, idx);

      /* If have this update, don't do anything */
      if ((EQ_Timestamp(value_p->dm_ts, upd_p->ud_timestamp)) &&
	  (EXT8(value_p->dm_value_present) == valuePresent) &&
	  (EXT8(upd_p->ud_operation) == EXT8(value_p->dm_flag)) &&
	  (EXT8(value_p->dm_type) == (EXT8(upd_p->ud_attribute)))) {
	
	if (valuePresent == BL_false) return(DNS_SUCCESS);

	/* See if value is the same */
	value_p = set_valuetest(set_p, valueLength, val_p, valueType, 
				&idx, dns_p);
	    
	if (value_p && (newindex-1 == idx))
	    return(DNS_SUCCESS);
      }
    }

    /* prepend enclosing DBSetMember */
    newvalue_p = (DBSetMember_t *)ALIGN_PTR(oldvalue_p);
    newlen = DIFF_bytes(SKIP_bytes(newvalue_p->dm_data, valueLength), oldvalue_p);
    if (newindex != cnt)
	newlen = ALIGN_LEN(newlen);

    /* see if no room for new offset */
    ptr_p = INDEX_DBSet(set_p, 0);
    if (0 < set_p->ds_hcnt)
	ptr_p = DATA_DBSet_Delete(set_p);
    if (ptr_p < (byte_t *)&set_p->ds_offp[cnt+1])
	expand = ALIGN_LEN(sizeof(set_p->ds_offp[cnt]));

    /*
     * Make additional room at oldvalue_p in rec_p
     * for additional offset and data
     */
    status = set_shiftdata(FALSE, (byte_t *)oldvalue_p,
		expand + newlen, rec_p, setindex+1);
    if (status != DNS_SUCCESS) return(status);

    /* Relocate set in new buffer */
    main_p = (DBSet_t *)rec_p->data_p;
    set_p = (DBSet_t *)INDEX_DBSet(main_p, setindex);
    set_p = (DBSet_t *)SKIP_AttributeName(set_p);
    ALIGN_CHECK(set_p);

    /* Additional shift within new space to add offset */
    if (expand) {
	ptr_p = INDEX_DBSet(set_p, 0);
	if (0 < set_p->ds_hcnt)
	    ptr_p = DATA_DBSet_Delete(set_p);

	MOVE_bytes(ptr_p, SKIP_bytes(ptr_p, expand),
	    DIFF_bytes(INDEX_DBSet(set_p, newindex), ptr_p));
    }
    /* Re-calculate offset positions */
    if (newindex == cnt) {
	set_p->ds_offp[cnt] = ALIGN_LEN(set_p->ds_setl + expand);

	--cnt;
    } else for (; newindex < cnt; --cnt)
	set_p->ds_offp[cnt] = set_p->ds_offp[cnt-1] + expand + newlen;

    for (; 0 <= cnt; --cnt)
        set_p->ds_offp[cnt] += expand;

    /* update set length */
    set_p->ds_setl += expand + newlen;
    db_data_check(rec_p);

    set_p->ds_ocnt++;

    /* Point to space for new member and fill it in */
    newvalue_p = (DBSetMember_t *)INDEX_DBSet(set_p, newindex);

    /* Build new member */
    INS8(newvalue_p->dm_flag, EXT8(upd_p->ud_operation));
    INS8(newvalue_p->dm_type, EXT8(upd_p->ud_attribute));
    INS8(newvalue_p->dm_value_present, valuePresent);
    INS8(newvalue_p->dm_valuetype, valueType);
    newvalue_p->dm_length = valueLength;
    if (valuePresent)
        MOVE_bytes(val_p, newvalue_p->dm_data, valueLength);

    COPY_Timestamp(upd_p->ud_timestamp, newvalue_p->dm_ts);

    /* Invalidate previous values if necessary */
    if ((EXT8(upd_p->ud_attribute) == AT_single) ||
         ((EXT8(upd_p->ud_operation) != UD_present) && !valuePresent)) {
      for (cnt=0; cnt < newindex; ++cnt) {
	DBSetMember_t *prevalue_p =
	    (DBSetMember_t *)INDEX_DBSet(set_p, cnt);
	INS8(prevalue_p->dm_valid, BL_false);
	
	/* 
	 * Can remove all previous history as long as these
	 * update types stay around
	 */
	INS8(prevalue_p->dm_flag, DM_remove);
      }
    }  else if (0 < newindex) { /* It is a set update */
      DBSetMember_t *prevalue_p =
	  (DBSetMember_t *)INDEX_DBSet(set_p, newindex-1);

      if (EXT8(prevalue_p->dm_type) == AT_single) {
	/* 
	 * Don't remove yet, need single update histories cause they
	 * cause previous values to be invalidated
	 */
	INS8(prevalue_p->dm_valid, BL_false);
      }

      /* See if value already exists */
      if (valuePresent) {

	for (idx = 0; ; idx++) {
	  value_p = set_valuetest(set_p, valueLength, val_p, 
				      valueType, &idx, dns_p);
	  /* If not there, go add it */
	  if (!value_p) break;
	      
	  if (newindex == idx) continue;

	  if (newindex > idx) {
	    INS8(value_p->dm_valid, BL_false);

	    /* If this is a set update also, flag to be removed 
	     * before skulk cause no history needed .  If it was
	     * a single update need to keep so deletes of
	     * previous values happens
	     */
	    if (EXT8(value_p->dm_type) == AT_set) 
		INS8(value_p->dm_flag, DM_remove);
	  } else { /* invalidate value we just added */
	    INS8(newvalue_p->dm_valid, BL_false);
	    
	    /* Can remove it cause this is a set update */
	    INS8(newvalue_p->dm_flag, DM_remove);
	  }
	}
      }
    }

    /* Determine what resulting set should contain */
    if (0 <= dnsCmpCTS((dns_cts *)upd_p->ud_timestamp,
			(dns_cts *)set_p->ds_ts))
        {
        COPY_Timestamp(upd_p->ud_timestamp, set_p->ds_ts);
        INS8(set_p->ds_type, EXT8(upd_p->ud_attribute));
	INS8(newvalue_p->dm_valid, EXT8(upd_p->ud_operation) == UD_present);
	INS8(set_p->ds_flag, DS_present);
        if (((EXT8(upd_p->ud_attribute) == AT_single) || !valuePresent)
		&& (EXT8(upd_p->ud_operation) != UD_present))
	    INS8(set_p->ds_flag, DSX_absent);
        }
    else if ((EXT8(upd_p->ud_attribute) == AT_single) ||
		(EXT8(upd_p->ud_operation) == UD_absent))
	{
	/* If update to a single and not the last one, nolonger valid */
        INS8(newvalue_p->dm_valid, BL_false);
	}
    else if (EXT8(set_p->ds_flag) != DS_present)
	{
        INS8(newvalue_p->dm_valid, BL_false);
	/* 
	 * Its a set update and another update removed the set, so don't need
	 * history
	 */
	INS8(newvalue_p->dm_flag, DM_remove);
	}
    else
        {
        cnt = set_p->ds_ocnt;
        for (++newindex; newindex < cnt; ++newindex)
            {
            DBSetMember_t *const dm_p = (DBSetMember_t *)INDEX_DBSet(set_p, newindex);

            /* Any later update to a single, wipes this out */
            if (EXT8(dm_p->dm_type) != AT_set)
                {
                INS8(newvalue_p->dm_valid, BL_false);
		/* 
		 * Its a set update and another update removed the set, 
		 * so don't need history
		 */
		INS8(newvalue_p->dm_flag, DM_remove);
	      }
            /* Any later delete attribute, wipes this out */
            else if (!EXT8(dm_p->dm_value_present) &&
			(EXT8(dm_p->dm_flag) == DM_absent))
                {
		  INS8(newvalue_p->dm_valid, BL_false);
		  INS8(newvalue_p->dm_flag, DM_remove);
                }
	  }
        INS8(newvalue_p->dm_valid, BL_true);
      }
    /* Get rid of values marked DM_remove 
     * don't need to check last value, it would not be marked remove 
     */
    totallen = 0;
    for (idx=0; idx < set_p->ds_ocnt-1; ) {

      ptr_p = SKIP_DBSet(set_p);

      value_p = (DBSetMember_t *)INDEX_DBSet(set_p, idx);
      if (EXT8(value_p->dm_flag) != DM_remove) {
	++idx;
	continue;
      }

      /* Get pointer to next value */
      newvalue_p = (DBSetMember_t *)INDEX_DBSet(set_p, idx+1);

      /* Save ptr to original first value */
      val_p = INDEX_DBSet(set_p, 0);

      newlen = DIFF_bytes(newvalue_p, value_p);

      /* See if we should shift offsets */
      expand = DIFF_bytes(val_p, &set_p->ds_offp[set_p->ds_ocnt-1]);
      if (expand != ALIGN_LEN(expand))
	  expand = 0;

      totallen += expand + newlen;


      /* Adjust offsets before idx */
      for (newindex =0; newindex <= idx; ++newindex)
	  set_p->ds_offp[newindex] -= expand;

      /* Adjust offsets after idx */
      for (newindex = idx+1; newindex < set_p->ds_ocnt-1; ++newindex) 
	  set_p->ds_offp[newindex] = set_p->ds_offp[newindex+1] - 
	      (expand + newlen);
      
      /* Update set count and length */
      --set_p->ds_ocnt;
      set_p->ds_setl -= (expand + newlen);

      /* Move rest of set over, overwritting last offset */
      MOVE_bytes(val_p, INDEX_DBSet(set_p, 0),
		 DIFF_bytes(value_p, val_p));

      /* Skip over value_p and move the rest */
      MOVE_bytes(newvalue_p, INDEX_DBSet(set_p, idx),
		 DIFF_bytes(ptr_p, newvalue_p));
    }

    /* If anything was removed, update main set */
    if (totallen != 0) {
      ptr_p = INDEX_DBSet(main_p, setindex+1);
      MOVE_bytes(ptr_p, ptr_p - totallen,
		 DIFF_bytes(SKIP_DBSet(main_p), ptr_p));

      /* Adjust mainset offsets */
      for (idx = setindex+1; idx < main_p->ds_ocnt; ++idx) 
	  main_p->ds_offp[idx] -= totallen;

      main_p->ds_setl -= totallen;
    }

    att_set_check(main_p);
    return(DNS_SUCCESS);
}

/*
 * set_new
 *    Build a new attribute set
 *
 * Input:
 *	rec_p = address of record descriptor
 *	set_p = address of where set should go
 *	set_index = index of new set
 *	upd_p = address of update packet
 *	neutral_name_p = attribute in neutral case
 *	casemask = case flags for difference between upd_p->upd_attribute and
 *		neutral_name_p
 *
 * Output:
 *    new attribute inserted
 *
 * Value:
 *    DNS_SUCCESS
 *    DNS_NONSRESOURCES
 */
static dns_status_t 
set_new (db_data_t *const        rec_p,
         DBSet_t                 *set_p,
         const int               set_index,
         const Update_t *const   upd_p,
         AttributeName_t *const  neutral_name_p,
         const unsigned int      casemask)
{
    DBSet_t		*main_p = (DBSet_t *)rec_p->data_p;
    byte_t		*val_p;
    int			valuePresent, valueLength, valueType;
    register int	newlen;
    dns_status_t	status;

    att_set_check(main_p);
    /* Collect update data */
    val_p = SKIP_AttributeSpecifier(upd_p->ud_attribute);
    valuePresent = GET8(val_p);
    valueLength = GET16(val_p);
    if (valuePresent == BL_false) {
	valueType = VT_none;
	valueLength = 0;
    } else
	valueType = GET8(val_p);

    /* prepend enclosing DBSetMember */
    newlen = ALIGN_LEN(sizeof(DBSetMember_overhead) + valueLength);

    /* prepend enclosing DBSet */
    newlen += ALIGN_LEN(DIFF_bytes(&set_p->ds_offp[1], set_p));

    /* prepend the AttributeName */
    newlen += ALIGN_LEN(LEN_AttributeName(neutral_name_p));

    /* new offset in enclosing set */
    newlen += sizeof(main_p->ds_offp[set_index]);

    status = set_shiftdata(TRUE, (byte_t *)set_p, newlen, rec_p, set_index);
    if (status != DNS_SUCCESS) return(status);

    /* Recalculate pointers, may be new buffer */
    main_p = (DBSet_t *)rec_p->data_p;
    if (set_index) {
	set_p = (DBSet_t *)INDEX_DBSet(main_p, set_index-1);
	set_p = (DBSet_t *)SKIP_AttributeName(set_p);
	ALIGN_CHECK(set_p);
	set_p = (DBSet_t *)SKIP_DBSet(set_p);
    } else
	set_p = (DBSet_t *)DATA_DBSet(main_p);

    /* align the DBSet following the AttributeName */
    set_p = (DBSet_t *)ALIGN_PTR(SKIP_bytes(set_p,
			LEN_AttributeName(neutral_name_p)));
    set_p = (DBSet_t *)SKIP_bytes(set_p, -LEN_AttributeName(neutral_name_p));
    main_p->ds_offp[set_index] = DIFF_bytes(set_p, main_p);

    /* Build a set with one value */
    (void)set_member(&set_p, neutral_name_p, casemask,
		(Timestamp_t *)upd_p->ud_timestamp, val_p, valueLength,
		valueType, valuePresent, EXT8(upd_p->ud_operation),
		EXT8(upd_p->ud_attribute));

    att_set_check(main_p);
    return(DNS_SUCCESS);
}

/*
 * set_replace_value
 *     The routine replaces the current value of an attriubte.  No history
 * need be maintained.  This is for per replica attributes and allupto.
 *
 * Input:
 *     rec_p = address of record descriptor
 *     upd_p = address of update structure
 *     dns_p = address of attribute in dns_attlist
 *
 * Output:
 *   Value replaced or new value added if it wasn't there
 * Value:
 *     DNS_SUCCESS
 *     DNS_NONSRESOURCES
 *
 */
dns_status_t 
set_replace_value (db_data_t               *rec_p,
                   Update_t                *upd_p,
                   attribute_descriptor_t  *dns_p)
{
    DBSet_t		*main_p = (DBSet_t *)rec_p->data_p;
    DBSet_t		*set_p;
    DBSetMember_t	*value_p;
    byte_t		*val_p;
    int			valuePresent, valueLength, valueType;
    int			set_index, oldlen, newlen;
    dns_status_t	status;

    att_set_check(main_p);
    /* Locate the attribute */
    status = find_attribute(rec_p->data_p, (AttributeName_t *)dns_p->aname,
	&set_p, &set_index);

    /* Gather update data */
    val_p = SKIP_AttributeSpecifier(upd_p->ud_attribute);
    valuePresent = GET8(val_p);
    valueLength = GET16(val_p);
    if (valuePresent == BL_false) {
	valueType = VT_none;
	valueLength = 0;
    } else
	valueType = GET8(val_p);
    /* If attribute doesn't exist and we want to add it, do it.  If we
     * we want to mark it absent, consider it done
     */
    if (status == DNS_UNKNOWNENTRY)
        if (EXT8(upd_p->ud_operation) == UD_present)
            return(set_new(rec_p, set_p, set_index, upd_p,
			(AttributeName_t *)dns_p->aname, dns_p->casemask));
        else
	    return(DNS_SUCCESS); /* Don't bother */

    if (!dns_p->rep_rec) /* Always replace pre replicas */
	{
        if (dnsCmpCTS((dns_cts *)set_p->ds_ts,
			(dns_cts *)upd_p->ud_timestamp) >= 0)
            return(DNS_SUCCESS);
	}
    else if (dns_p->atype == DNS_LASTUPDATE)
	/* Maximize value */
	{
	value_p = (DBSetMember_t *)DATA_DBSet(set_p);
	if (dnsCmpCTS((dns_cts *)value_p->dm_data,
			(dns_cts *)val_p) > 0)
	    return(DNS_SUCCESS);
	}

    oldlen = LEN_DBSet(set_p);

    /* prepend enclosing DBSetMember */
    newlen = sizeof(DBSetMember_overhead) + valueLength;

    /* prepend enclosing DBSet */
    newlen += ALIGN_LEN(DIFF_bytes(&set_p->ds_offp[1], set_p));

    if (newlen-oldlen)
	{
	status = set_shiftdata(FALSE, (byte_t *)set_p,
		    newlen-oldlen, rec_p, set_index+1);
	if (status != DNS_SUCCESS) return(status);

	/* Recalculate pointers, may be new buffer */
	main_p = (DBSet_t *)rec_p->data_p;
	}

    /* Recalculate pointer to attributename  */
    set_p = (DBSet_t *)INDEX_DBSet(main_p, set_index);
    (void)set_member(&set_p, (AttributeName_t *)dns_p->aname, dns_p->casemask,
		(Timestamp_t *)upd_p->ud_timestamp, val_p, valueLength,
		valueType, valuePresent, EXT8(upd_p->ud_operation),
		EXT8(upd_p->ud_attribute));

    att_set_check(main_p);
    return(DNS_SUCCESS);
}

/*
 * set_insert
 *     Apply an update to an attribute.
 *
 * Input:
 *     rec_p = address of record descriptor
 *     upd_p = address of update structure
 *     dns_p = address of attribute descriptor
 *     change_allowed = change of type allowed
 *
 * Output:
 *    new value inserted
 * Value:
 *     DNS_SUCCESS
 *     DNS_NONSRESOURCES
 *     DNS_WRONGATTRIBUTETYPE
 *
 */
dns_status_t 
set_insert (db_data_t               *rec_p,
            Update_t                *upd_p,
            attribute_descriptor_t  *dns_p,
            int                     change_allowed)
{
    AttributeName_u	newname;
    AttributeName_t	*newname_p;
    dns_status_t	status;
    DBSet_t		*set_p;
    unsigned int	casemask;
    int			set_index;
    int			i;

    /*
     * Make sure name is neutral case. We can't convert in place because
     * we need to record the original in transaction log.
     */
    if (!dns_p)
	{
	AttributeName_t *const oldname_p =
		(AttributeName_t *)SKIP_AttributeType(upd_p->ud_attribute);

	newname_p = (AttributeName_t *)newname;
        names_s_neutralize_case((SimpleName_t *)oldname_p,
                                (SimpleName_t *)newname_p);
	casemask = 0;
	/* Build mask */
	for (i = 0; i < EXT8(newname_p->an_length); ++i)
	    if (newname_p->an_name[i] != oldname_p->an_name[i])
		casemask += 1<<i;
	}
    else
	{
  	newname_p = (AttributeName_t *)dns_p->aname;
    	casemask = dns_p->casemask;
	/* Cannot change the type of a known attribute */
	if (EXT8(upd_p->ud_attribute) != dns_p->type)
	    return(DNS_WRONGATTRIBUTETYPE);
	/* No history is maintained here */
	if (EXT8(upd_p->ud_attribute) == AT_single)
	    if (dns_p->rep_rec || 
		(dns_p->ch_rec && (rec_p->data_type == ET_clearinghouse)) ||
		(dns_p->atype == DNS_ALLUPTO) || (dns_p->atype == DNS_UTS))
	      return(set_replace_value(rec_p, upd_p, dns_p));
        }

    /* Locate the attribute */
    status = find_attribute(rec_p->data_p, newname_p, &set_p,
             &set_index);

    /* If it doesn't exist, add it */
    if (status == DNS_UNKNOWNENTRY)
	{
	if (EXT8(upd_p->ud_operation) != UD_present)
	    {					 /* If have no operation just */
	    return (DNS_SUCCESS);		 /* return, don't add null set*/
	    }
        return(set_new(rec_p, set_p, set_index, upd_p, newname_p, casemask));
	}

    /*
     * Changing the type of a present attribute is not allowed
     * from the TA (whether operation is to remove or add attribute)
     */
    if ((EXT8(set_p->ds_type) != EXT8(upd_p->ud_attribute)) &&
	(EXT8(set_p->ds_flag) == DS_present) && !change_allowed)
        return(DNS_WRONGATTRIBUTETYPE);

    /* Insert update and determine validity */
    return(set_insert_member(rec_p, set_p, upd_p, set_index, dns_p));
}
