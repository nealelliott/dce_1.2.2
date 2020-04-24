/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_cell.c,v $
 * Revision 1.1.4.2  1996/02/18  19:30:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:12:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:19  root]
 * 
 * Revision 1.1.2.3  1994/08/03  19:01:33  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:52:38  mccann]
 * 
 * Revision 1.1.2.2  1994/06/09  18:37:59  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:06  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/12  22:01:31  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:08:28  peckham]
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1993-1994. ALL RIGHTS RESERVED.
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

#include <gda.h>
#include <tower.h>
#include <dce/attributes.h>
#include <dce/dcecfgmsg.h>
#if defined(DCE_SEC)
#include <dce/sec_login.h>
#include <dce/rpc.h>
sec_login_handle_t login_context_h;
#endif

#define OUR_CELL_ROOT "/.:"

/*
 * Local function prototypes.
 */
static void
initflags (
             cdsFlagStat_t *);

	   
/*
 * Function: get cell info.
 * Gets cell (namespace) uuid, and all root replica info:
 * clearinghouse type, name, uuid, and towers.
 * Returns 0 on success, non-zero on error;
*/


int 
get_cell_info (char           *cell_uuid_string,
               ReplicaInfo_t  *replica_info,
               int            *num_replicas)
{
    int status;
    error_status_t error_status;
    int index, len;
    cdsFlagStat_t flags;
    cds_full_name_t opq_cellname;
    int opq_cellname_len = sizeof (opq_cellname);
    cds_attrvalue_t *bufptr;
    cds_handle_t hp;
    uuid_t tmp_uuid;
    unsigned_char_p_t tmp_uuid_string;
    ReplicaPointer_t *rp_p;
    Set_t *set_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " >get_cell_info"));

    /* 
     * Get our cell name and convert to opaque format
     */
    if ((status = cdsCvtFullToOpq ((unsigned char *)OUR_CELL_ROOT,
				   &opq_cellname,
				   &opq_cellname_len) != CDS_SUCCESS)) 
    {
	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug2,
            " <get_cell_info: error converting cell name to opaque fmt. return(status=%d)",
	    status));

	return (status);
    }
    hp = cdsGetHandle();
    
    if (hp == NULL_CDS_HANDLE)
    {
	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug2,
            " <get_cell_info: return(DNS_ERROR)"));

	return ( DNS_ERROR );
    }
    
    /*
     * Initialize flags 	
     */
    initflags(&flags);
    
    status = 
	cdsReadAttrValue(
			 &opq_cellname,
			 hp,
			 (cds_attr_name_t *)dns_AttributeName(DNS_OBJECTUID),
			 cdsDir, 
			 NULL,
			 &bufptr, 
			 (cds_cts_t **)NULL,
			 (unsigned char *)NULL,
			 (int *)NULL, 
			 &flags);
    
    if (status != CDS_SUCCESS) 
    {

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug2,
            " <get_cell_info: error from cdsReadAttrValue. return(status=%d)",
	    status));

	return (status);
    }
    
    /*
     * Align the uuid 	
     */
    memcpy (&tmp_uuid, &bufptr->val_u.val_uuid, sizeof (tmp_uuid));
    uuid_to_string (&tmp_uuid,
		    (unsigned_char_p_t *)&tmp_uuid_string,
		    &error_status);
    if (error_status == uuid_s_ok) 
    {
	strcpy (cell_uuid_string, (char *)tmp_uuid_string);
	rpc_string_free (&tmp_uuid_string, &error_status);
    }
    else 
    {
	*cell_uuid_string = '\0';

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug2,
            " <get_cell_info: error from uuid_to_string. return(status=%d)",
	    error_status));

	return (error_status);
    }
    
    cdsClearHandle(hp);
    
    /*
     * Now loop to read all replica pointers 	
     */
    index = *num_replicas = 0;
    while (status == CDS_SUCCESS) 
    {
	
	status = 
	    cdsReadAttrValue(
			     &opq_cellname,
			     hp,
			     (cds_attr_name_t *)dns_AttributeName(DNS_REPLICAS),
			     cdsDir, 
			     NULL,
			     &bufptr, 
			     (cds_cts_t **)NULL,
			     (unsigned char *)NULL,
			     (int *)NULL, 
			     &flags);
	
	if (( status == DNS_SUCCESS ) &&
	    ( bufptr->val_syntax == cds_ReplicaPointer )) 
	{
	    ++*num_replicas;
	    
	    /*
	     * Get replica pointer 
	     */
	    rp_p = (ReplicaPointer_t *)bufptr->val_u.val_byte.byte_p;
	    
	    /*
	     * get replica type 
	     */
	    replica_info[index].clh_type = 
		EXT8(FLD_ReplicaPointer_replicaType(rp_p));
	    /*
	     * Align and convert clearinghouse uuid 
	     */
	    memcpy (&tmp_uuid, &rp_p->rp_CHID, sizeof (tmp_uuid));
	    uuid_to_string (&tmp_uuid,
			    (unsigned_char_p_t *)&tmp_uuid_string,
			    &error_status);
	    if (error_status == uuid_s_ok) 
	    {
		strcpy (replica_info[index].clh_uuid, (char *)tmp_uuid_string);
		rpc_string_free (&tmp_uuid_string, &error_status);
	    }
	    else 
	    {
		*replica_info[index].clh_uuid = '\0';
		cdsFreeHandle(hp);
		
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug2,
		    " <get_cell_info: error from uuid_to_string. return(status=%d)",
		    error_status));
		
		return(error_status);
	    }
	    
	    /*	
	     * convert clearinghouse name 
	     */
	    len = sizeof (cds_full_name_t);
	    if ((status = 
		 cdsCvtFullToStr  (
				   (cds_full_name_t *)FLD_ReplicaPointer_CHName(rp_p),
				   (unsigned char *)replica_info[index].clh_name,
				   &len) != CDS_SUCCESS)) 
	    {
		cdsFreeHandle(hp);

		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug2,
		    " <get_cell_info: error coverting clearinghouse name. return(status=%d)",
		    status));

		return(status);
	    }
	    
	    set_p = (Set_t *)rp_p->rp_addressHint;
	    if ((status = cdsCvtTowerToStr(set_p,
					   (unsigned char **)&replica_info[index].towers))
		!= CDS_SUCCESS) 
	    {
		cdsFreeHandle(hp);

		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug2,
		    " <get_cell_info: error from cdsCvtTowerToStr. return(status=%d)",
		    status));

		return(status);
	    }
	}	
	++index;
    } /* end of while (status == CDS_SUCCESS).. */
    
    cdsFreeHandle(hp);
    if ((status == CDS_SUCCESS) || ((status == CDS_EMPTY) && (*num_replicas)))
    {
	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug2,
	    " <get_cell_info: return(0)"));

	return (0);
    }
    else 
    {
	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug2,
	    " <get_cell_info: error - return(status=%d)",
	    status));

	return(status);
    }
}



static cds_full_name_t partialRes;

static void 
initflags (struct cdsFlagStat *flags)
{
#ifdef DCE_SEC

    error_status_t sec_status;

#endif


   DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " >initflags"));

    ZERO_bytes (flags, sizeof (struct cdsFlagStat));
    flags->fsPartialRes = &partialRes;
    flags->fsResLength = sizeof(partialRes);
    flags->fsConf=cdsConfHi;
    flags->fsDontCache=1;
    flags->fsWait=5*60;
    flags->fsNameType = cdsOpqDNA;
    
#ifdef DCE_SEC

    sec_login_get_current_context(&login_context_h, &sec_status);
    if (sec_status == error_status_ok) 
    {
	flags->fsUnAuth = 0;                    /* use authenticated RPC */
	flags->fsTrustAll = 0;                  /* check server group */
    }
    else  
    {
	flags->fsUnAuth = 1;         /* unauthenticated RPC */
	flags->fsTrustAll = 1;       /* no check server group */
    }

#else

    flags->fsUnAuth = 1;                 /* default is no authentication */
    flags->fsTrustAll = 1;               /* trust any server */

#endif

   DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " <initflags"));
}


