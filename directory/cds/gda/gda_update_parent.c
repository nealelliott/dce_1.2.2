/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_update_parent.c,v $
 * Revision 1.1.12.1  1996/08/09  11:57:37  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  15:26 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/2]
 *
 * 	OT 12819 fix; added check for NULL pointer return in update_parent.
 * 	[1996/02/18  19:31:28  marty  1.1.10.2]
 *
 * Revision 1.1.10.2  1996/02/18  19:31:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:30  marty]
 * 
 * Revision 1.1.10.1  1995/12/08  15:13:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:38  root]
 * 
 * Revision 1.1.8.5  1994/08/03  19:01:44  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:53:47  mccann]
 * 
 * Revision 1.1.8.4  1994/06/23  18:30:10  mccann
 * 	sams cleanup drop1
 * 	[1994/06/23  18:15:43  mccann]
 * 
 * Revision 1.1.8.3  1994/06/09  18:38:13  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:27  devsrc]
 * 
 * Revision 1.1.8.2  1994/04/29  15:50:00  peckham
 * 	Use dcecdsmac.h macros for serviceability.
 * 	Use dcelocal_path in file names.
 * 	[1994/04/29  13:56:53  peckham]
 * 
 * Revision 1.1.8.1  1994/03/12  22:01:51  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:10:09  peckham]
 * 
 * Revision 1.1.6.3  1992/12/30  13:27:36  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:44:38  zeliff]
 * 
 * Revision 1.1.6.2  1992/09/29  19:13:21  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:12:29  weisman]
 * 
 * Revision 1.1.2.2  1992/03/22  21:58:16  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:24:38  weisman]
 * 
 * Revision 1.1  1992/01/19  15:24:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: gda_update_parent.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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

#include <gda.h>
#include <stddef.h>
#include <errno.h>
#include <netdb.h>

#include <dce/dce_cf.h>
#include <dce/rpc.h>
#include <tower.h>
#include <limits.h>
#include <dns_strings.h>

extern rpc_binding_vector_p_t gdad_bind_vector;

/* 
 * Local function Prototypes.
 */

static void
init_flags(
	   cdsFlagStat_t 	*);

static int
make_replica_ptr(
		 int		,
		 Set_t		*,
		 ObjUID_t	*,
		 ReplicaPointer_t *);

static int
cmp_towers(
	   Set_t		*,
	   Set_t		*);


static char gda_id[_DNS_MAXPATHLEN+1];

/* CDS_GDAPointers attribute */
static unsigned char rep_set[] = "1.3.22.1.3.52";

#define VAL_GP_P	val_u.val_byte.byte_p
#define VAL_GP_LEN	val_u.val_byte.length

/*
 * update_parent
 *   This function updates a the gdapointer attribute for this
 *  gda in the local cell.
 */
int 
update_parent (uuid_t *ch_id_p)
{
    cds_full_name_t *cellname_p;
    cds_attr_name_t *ap = (dns_attr_name_t *)rep_set;
    cds_handle_t handle;
    cds_attrvalue_t *value_p, value;
    gdaPointer_t gp_ptr_val, new_gp_ptr;
    ReplicaPointer_t *rp_p;
    Timeout_t *to_p;
    timespec_t inacc;
    reltimespec_t time;
    utc_t time_utc;
    byte_t my_towers[MAXATTRIBUTE];	/* same as ReplicaPointer_t */
    ObjUID_t my_chid;
    cdsFlagStat_t flags;
    int status;
    error_status_t dce_cf_status;

    
    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_gda,
		   svc_c_debug5,
		   " >update_parent"));
    
    dce_cf_get_cell_name((char **)&cellname_p, &dce_cf_status);
    if (dce_cf_status != dce_cf_st_ok) {
      dce_svc_printf(CDS_S_CFGETCELLNAME_MSG, (long)dce_cf_status);
      return(dce_cf_status);
    }
    /*
     * get my clearinghouse uuid from a file, exits if not found.
     */
    get_ObjUID(&my_chid);
    *ch_id_p = my_chid;

    /*
     * Read the gdaPointer set out of the directory.
     */
    handle = cdsGetHandle();

    if (handle == NULL_CDS_HANDLE)
    {
	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug2,
            " <update_parent: return(DNS_ERROR)"));

	return ( DNS_ERROR );
    }

    init_flags(&flags);
    status = cdsReadAttrValue(cellname_p, handle, ap, cdsDir,
			      NULL, &value_p, NULL, 
			      NULL, NULL, &flags);
    if (status == DNS_SUCCESS) 
    {
	/*
	 * Read through gdaPointers until we find one that matches
	 * my CH id
	 */

	do {
	    /*
	     * Copy the attribute value from the reply message
	     * so it doesn't get overwritten
	     */
	    COPY_gdaPointer(value_p->VAL_GP_P, &gp_ptr_val);
	    value.val_syntax = VT_gdaPointer;
	    value.VAL_GP_P = &gp_ptr_val;
	    value.VAL_GP_LEN = LEN_gdaPointer(value.VAL_GP_P);
	    rp_p = (ReplicaPointer_t *)gp_ptr_val.gp_replica;
	    if (EQ_ObjUID(rp_p->rp_CHID, &my_chid)) break;
	    status = cdsReadAttrValue(cellname_p, handle, ap, cdsDir,
				      NULL, &value_p, 
				      NULL, NULL, NULL, &flags);
	} while (status == CDS_SUCCESS);
      } 
    else if (status != CDS_EMPTY) 
	{
	  unsigned32 uuid_status;

	  /*
	   * ReadAttributeValue failed
	   */
	  cdsFreeHandle(handle);
	  uuid_create_nil(ch_id_p, &uuid_status);

	  dce_svc_printf(CDS_S_GDA_UPDATEPP_MSG, (long)flags.fsLocStat);
	  DCE_SVC_DEBUG((
			 cds__svc_handle,
			 cds_svc_gda,
			 svc_c_debug5,
			 " <update_parent: return(DNS_ERROR)"));
 
	  return(DNS_ERROR);
	}	

    dnsFreeHandle(handle);
    cds_LocalTowers ( gdad_bind_vector, my_towers );

    /*
     * If we found a matching CH id, see if the stored tower set matches
     * the local one. All done if it does; otherwise remove the current
     * gdaPointer from the set.
     */
    if (status == DNS_SUCCESS) 
	{
	  if (cmp_towers((Set_t *)my_towers,
			 (Set_t *)rp_p->rp_addressHint))
	      {
		DCE_SVC_DEBUG((
			       cds__svc_handle,
			       cds_svc_gda,
			       svc_c_debug7,
			       " <update_parent: return(DNS_SUCCESS)"));

		return(DNS_SUCCESS);
	      }
	  else 
	      {
		init_flags(&flags);
		dnsRmAttrValue(cellname_p, ap, dnsDir, &value,
			       (dns_cts *)NULL, &flags);
	      }
	}

    /*
     * Now update the replica set with an up-to-date replica pointer
     */
    to_p = (Timeout_t *)new_gp_ptr.gp_timeout;
    
    inacc.tv_sec = 0;
    inacc.tv_nsec = 0;
    time.tv_sec = 24*60*60;		/* check us every 24 hours */
    time.tv_nsec = 0;
    utc_gettime(&time_utc);
    COPY_utc(&time_utc, to_p->to_expiration);
    utc_mkbinreltime(&time_utc, &time, &inacc);
    COPY_utc(&time_utc, to_p->to_extension);
    
#ifdef RT_gda
    make_replica_ptr(RT_gda, (Set_t *)my_towers, &my_chid,
		     (ReplicaPointer_t *)new_gp_ptr.gp_replica);
#else
    make_replica_ptr(RT_readOnly, (Set_t *)my_towers, &my_chid,
		     (ReplicaPointer_t *)new_gp_ptr.gp_replica);
#endif
    value.val_syntax = VT_gdaPointer;
    value.VAL_GP_P = (byte_t *)&new_gp_ptr;
    value.VAL_GP_LEN = LEN_gdaPointer(value.VAL_GP_P);
    status = dnsAddAttrValue(cellname_p, ap, dnsSet, dnsDir, &value,
			     (dns_cts *)NULL, &flags);

    if (status != DNS_SUCCESS)
	dce_svc_printf(CDS_S_GDA_UPDATEPP_MSG, (long)flags.fsLocStat);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug5,
        " <update_parent: return(%d)",
	status));

    return(status);
}

static void 
init_flags (dnsFlagStat_t *flags)
{
    memset((char *) flags, 0, sizeof(dnsFlagStat_t));
    flags->fsConf = dnsConfMed;
    flags->fsNameType = dnsStrDCE;

}


int 
get_ObjUID (ObjUID_t *uid_p)
{
    char str_uid[100], *str_uid_p, *end_p;
    FILE *fp;
    error_status_t status;
    

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug7,
	" >get_ObjUID"));

    (void)sprintf(gda_id, CDS_GDA_ID_FILE, dcelocal_path);
	
    fp = fopen(gda_id, "r");
    if (fp == NULL) 
    {
	goto wrtn;
    }
    
    if (fgets(str_uid, sizeof(str_uid), fp) == NULL) 
    {
	goto ertn;
    }
      
    fclose(fp);

    if (end_p = strchr(str_uid, '\n')) 
    {
	*end_p = '\0';
    }
    
    uuid_from_string((unsigned char *)str_uid, uid_p, &status);
    if (status) 
    {
	goto ertn;
    }
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug7,
	" <get_ObjUID: return(DNS_SUCCESS)"));

    return(DNS_SUCCESS);

wrtn:

    fp = fopen(gda_id, "w");
    if (fp == NULL) 
    {
	goto ertn;
    }
    
    uuid_create((uuid_t *)uid_p, &status);
    uuid_to_string((uuid_t *)uid_p, (unsigned char **)&str_uid_p, &status);
    if (status) 
    {
	goto ertn;
    }
    
    fputs(str_uid_p, fp);
    free((char *)str_uid_p);
    putc('\n', fp);
    fclose(fp);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug7,
	" <get_ObjUID: return(DNS_SUCCESS)"));

    return(DNS_SUCCESS);

ertn:

    ZERO_ObjUID(uid_p);
    if (fp) 
    {
	fclose(fp);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug7,
	" <get_ObjUID"));

    dce_svc_printf(CDS_S_GDA_UUIDFILE_EB_MSG, gda_id, strerror(errno));
}


/*
 * cmp_towers
 *    checks that the two towers have the same members.  Ignores
 *    timestamps.
 *
 * Input:
 *     towerset1_p = pointer to first tower set
 *     towerset2_p = pointer to second tower set
 * Output:
 * 
 * Value:
 *     returns TRUE if the towers are equivalent except for
 *     order and timestamps.
 *
 * Side-effect:
 *     writes in the second towerset - marks things absent that match.
 */
static int 
cmp_towers (Set_t  *towerset1_p,
            Set_t  *towerset2_p)
{
    int                   num_mem1;
    int                   num_mem2;
    int                   i,j,found = 0;
    MemberValue_t        *member1_p;
    MemberValue_t        *member2_p;
    

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug7,
	" >cmp_towers"));

    num_mem1 = NUM_Set(towerset1_p);
    num_mem2 = NUM_Set(towerset2_p);

    for (i = 0; i < num_mem1; ++i) 
    {
        member1_p = (MemberValue_t *)INDEX_Set(towerset1_p, i);

        if (FLAG_IS_CLEAR(member1_p->mv_flag, MV_present))
	{
	    continue;
	}
	
        found = 0;
        for (j = 0; j < num_mem2; ++j) 
	{
            member2_p = (MemberValue_t *)INDEX_Set(towerset2_p, j);
            if (FLAG_IS_CLEAR(member2_p->mv_flag, MV_present))
	    {
		continue;
	    }
	    
	    if (EXT8(member1_p->mv_valuetype) != EXT8(member2_p->mv_valuetype))
	    {
		continue;
	    }
	    
            if (EXT16(member1_p->mv_length) != EXT16(member2_p->mv_length))
	    {
		continue;
	    }
	    
            if (NE_bytes(member2_p->mv_value, member1_p->mv_value,
			 EXT16(member1_p->mv_length)))
	    {
		continue;
	    }
	    
	    /*	
	     * mark the member in the second set absent 
	     */
	    CLEAR_FLAG(member2_p->mv_flag, MV_present);
            found = 1;
            break;
        }
	if (!found)
	{
	    /*
	     * not a match at all 
	     */
	    DCE_SVC_DEBUG((
	        cds__svc_handle,
		cds_svc_gda,
		svc_c_debug7,
		" <cmp_towers: not a match at all - return (found = %d)",
		found));

	    return (found);  
	}
	
    }

    /*
     * check for any present members in the second list 
     */
    for (j = 0; j < num_mem2; ++j) 
    {
        member2_p = (MemberValue_t *)INDEX_Set(towerset2_p, j);
        if (FLAG_IS_SET(member2_p->mv_flag, MV_present))
	{
	    found = 0;
	}
    }

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_gda,
	svc_c_debug7,
	" <cmp_towers: return (found = %d)",
	found));

    return (found);
}

static int 
make_replica_ptr (int               type,
                  Set_t             *towers,
                  ObjUID_t          *chid,
                  ReplicaPointer_t  *replica)
{
   register byte_t *p_p = replica->rp_addressHint;
   int nlen = sizeof(FullName_t);
   char hname[100];
   unsigned char chname[120];

   DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_gda,
	svc_c_debug7,
	" >make_replica_ptr"));

   COPY_ObjUID(chid, replica->rp_CHID);
   COPY_Set(towers, p_p);
   p_p = SKIP_Set(p_p);
   INS8(p_p, type);
   p_p = SKIP_ReplicaType(p_p);
   gethostname(hname, sizeof(hname));
   (void)sprintf((char *)chname, "/.:/%s_gda", hname);
   dnsCvtCDSFullToOpq(chname, (dns_full_name_t *)p_p, &nlen);

   DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_gda,
	svc_c_debug7,
	" <make_replica_ptr"));

   return(SKIP_FullName(p_p) - (byte_t *)replica);
}
