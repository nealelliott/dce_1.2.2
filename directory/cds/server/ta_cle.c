/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_cle.c,v $
 * Revision 1.1.20.1  1996/08/09  12:01:01  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  15:56 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	OT 12932 fix: Export clearinghouse towers to clearinghouse object before
 * 	advertising the new clearinghouse in mgmt_create_cle().
 * 	*
 * 	Part of fix for OT 13208:
 * 	Modify file_back_inhibit use in file_info_t.  It now requires a signal
 * 	to wake the sleeping background thread when inhibit is turned off.
 * 	This to make explicit the ordering of the background run and the
 * 	ta_replica_add() during clearinghouse creation.  Also affects db_diags
 * 	[1996/02/18  19:36:48  marty  1.1.18.3]
 *
 * Revision 1.1.18.3  1996/02/18  19:36:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:29  marty]
 * 
 * Revision 1.1.18.2  1995/12/08  15:32:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:59  root]
 * 
 * Revision 1.1.14.1  1994/10/25  16:17:47  proulx
 * 	Don't enable the clearinghouse until db_enable has been done.
 * 	Poke the advertiser when turning a cleainghouse on.
 * 	All routines have a single exit.
 * 	Flesh out debugging.
 * 	[1994/10/25  15:06:53  proulx]
 * 
 * Revision 1.1.6.6  1994/08/16  18:28:06  jd
 * 	drop from janet
 * 	[1994/08/13  16:07:19  jd]
 * 
 * Revision 1.1.6.5  1994/08/03  19:04:43  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:28:30  mccann]
 * 
 * Revision 1.1.6.4  1994/06/30  19:18:19  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:49:12  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:43:57  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:30  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/25  19:57:24  ohara
 * 	add stdio.h
 * 	[1994/04/20  19:53:29  ohara]
 * 
 * Revision 1.1.6.1  1994/03/12  22:12:43  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:45:59  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:23:40  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:24:16  zeliff]
 * 
 * Revision 1.1.2.3  1992/03/22  22:34:54  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:36:25  weisman]
 * 
 * Revision 1.1.2.2  1992/01/22  23:02:28  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:18:06  melman]
 * 
 * $EndLog$
 */
 /*  ta_cle.c
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
 * Module Description
 *
 * Routines for clearinghouse operations
 *
 */

#include <server.h>
#include <clerk.h>
#include <events.h>
#include <names.h>
#include <uid.h>

#include <stdio.h>
#include <dce/dce_cf.h>



/* 
 * mgmt_create_cle
 *	This routine is called as the result of a management request
 *	to create a clearinghouse.
 *
 */
dns_status_t 
mgmt_create_cle (nsgbl_ns_t         *ns_p,
                 user_descriptor_t  *user_p,
                 nsgbl_ch_t         *ch_p,
                 FullName_t         *chname_p,
                 FullName_t         *replica_p,
                 int                replica_type,
                 version_t          *dir_version_p)
{
    static ClassVersion_t obj_version = {2,0};
    Timestamp_u		ts;
    FullName_t		*parent_p;	/* parent of CH object name */
    FullName_t		*rep_p;		/* replica pointer */
    FullName_t		*resolved_p;       
    Update_t		*upd_p;
    byte_t		att_value;
    byte_t		object_created = FALSE;  
    struct buffer {
	FullName_u	b_rep;
	FullName_u	b_parent;
	FullName_u	b_resolved;
	Update_u	b_upd;
    } *buf_p;
    int                 upd_len;
    dns_status_t        status;
    LOG_ASCII_BUF_FullName(fullnameBuf)
    LOG_ASCII_BUF_AttributeName(attrnameBuf)
    int thstatus;
    file_info_t         *file_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >mgmt_create_cle"));

    buf_p = (struct buffer *)dns_malloc(sizeof(struct buffer));
    if (!buf_p)
    {
	dce_svc_printf(CDS_S_SERVER_CRECLENOMEM_MSG);
	return (DNS_NONSRESOURCES);
    }


    /* set required pointers to the right addresses in the buffer */
    rep_p = (FullName_t *)buf_p->b_rep;
    parent_p = (FullName_t *)buf_p->b_parent;
    resolved_p = (FullName_t *)buf_p->b_resolved;
    upd_p = (Update_t *)buf_p->b_upd;

    /* fill in some of the fields of new clearinghouse structure */
    COPY_FullName(chname_p, &ch_p->ch_name);
    COPY_ObjUID(chname_p->fn_root, &ch_p->ch_nsuid);
    id_new(&ch_p->ch_uid);

    /* get namespace nickname and ns_uid to put into new CH structure */
    status = ta_cle_get_nsnickname(ch_p);
    if (status != DNS_SUCCESS) 
    {
      dns_free((char *)buf_p);
      return(status);
  }

    /*
     * replica info 
     */
    COPY_FullName(replica_p, rep_p);

    /*
     * get parent name 
     */
    names_stripright(&ch_p->ch_name, parent_p);
 
    /*
     * check if parent name contains a link 
     */
    if (mgmt_check_linked(parent_p, resolved_p) == DNS_SUCCESS)
	COPY_FullName(resolved_p, parent_p);

    /*
     * resolve replica name also 
     */
    if (mgmt_check_linked(rep_p, resolved_p) == DNS_SUCCESS)
	COPY_FullName(resolved_p, rep_p);
   
    /*
     * make sure replica does not violate invariant 
     * Only the root directory is allowed for CDS
     */
    if ((NE_FullName((dns_opq_fname *)parent_p, (dns_opq_fname *)rep_p)) ||
	 (NE_FullName((dns_opq_fname *)parent_p, 
		      (dns_opq_fname *)&ch_p->ch_cellname)))
	
    {
	dce_svc_printf(CDS_S_SERVER_INVARIANT_MSG);
	dns_free((char *)buf_p);

	return(DNS_ROOTLOST);
    }

    status = clerk_create_object(&ch_p->ch_name,
		(ClassName_t *)cds_classlist[DNS_CLEARINGHOUSE].cname,
		&obj_version, &ch_p->ch_uid, &ch_p->ch_cts);
    if (status != DNS_SUCCESS)
    {					 /* Failed to create object  */
        dce_svc_printf(CDS_S_SERVER_CRECLEOBJ_MSG, (long)status);
    }					 /* Failed to create object  */
    else
    {					 /* Created object, continue  */
	object_created = TRUE;

	status = clerk_skulk(parent_p);

	if (status != DNS_SUCCESS) {		 /* skulk failed  */
	  dce_svc_printf(CDS_S_SERVER_CRECLESKULK_MSG, (long)status);
	}
	else   { 			 /* skulk succeeded  */
	  /* 
	   * Test the cts of new object to verify its 
	   * existence after the skulk 
	   */
	  status = clerk_test_attribute_conf(&ch_p->ch_name,
			     ET_object,
	        	     dns_AttributeName(DNS_CTS),
		             sizeof(Timestamp_u), (byte_t *)&ch_p->ch_cts,
			     dns_Attribute(DNS_CTS)->vtype,
			     dnsConfMed);

	  if (status != DNS_SUCCESS) {
	    dce_svc_printf(CDS_S_SERVER_CLECRECTS_MSG, (long)status);
	  }

	  if (status == DNS_NOTFND)
	      {
		status = DNS_ENTRYEXISTS;
		object_created = FALSE;
		goto entry_exists;
	      }

	  if (status == DNS_SUCCESS) {
	    /* create pseudo directory */
	    status = db_create(ch_p, user_p, dir_version_p);
	    if (status != DNS_SUCCESS)
		goto entry_exists;
	    
	    /*
	     * Before enabling the clearinghouse, which will enable the
	     * background thread, set a flag so that the background thread 
	     * will wait until the clearinghouse is fully reliable.
	     * This is to guarantee that the background thread runs after
	     * ta_replica_add() is complete, even if there's a delay in 
	     * processing ta_replica_add().  Only need to worry about 
	     * restoring it except on successful completion of this procedure.
	     */
	    file_p = db_find_file(&ch_p->ch_uid);

	    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0) {
		dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
	    }
	    {
		file_p->file_back_inhibit = TRUE;
	    }
	    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0) {
		dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	    }

	    /* 
	     * Set the file state to be DB_OPEN 
	     */
	    status = db_enable(ch_p, TRUE);
	    if (status != DNS_SUCCESS)
		goto entry_exists;

	    if ((thstatus = dthread_lock(&(ch_p->ch_mutex))) < 0)
	    {
		dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
	    }


	      {      /* Beginning of the 'protected code' */

		ch_p->ch_state = dns_On;
		if ((thstatus = dthread_broadcast(&(ch_p->ch_state_changed))) < 0)
		{
		    dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
		}
	    } /* End of the 'protected code' */

	    if ((thstatus = dthread_unlock(&(ch_p->ch_mutex))) < 0)
	    {
		dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	    }

            /*
             * Export clearinghouse and Poke the advertiser
             */
	    if (!net_re_export_ch (ch_p)) {
	      goto entry_exists;
	    }
            adver_signal_new_info (ns_p->ns_advertiser);

	    /* add replica */
	    status = ta_replica_add(user_p, rep_p, replica_type);

	    /* log event if successful */
	    if (status == DNS_SUCCESS)
		{
		  dce_svc_printf(CDS_S_SERVER_CLEARINGHOUSECREATED_MSG,
			 deb_ascii_FullName(fullnameBuf,&ch_p->ch_name));

		}
	    /*
	     * Now, allow background to run.
	     */
	    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0) {
		dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
	    }
	    {
		file_p->file_back_inhibit = FALSE;
		if ((thstatus = dthread_broadcast(&(file_p->file_state_changed))) < 0) {
		    dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
		}
	    }
	    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0) {
		dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	    }    

	  } /* end if testattribute was successfully got */
	} /* if skulk successful */
      } /* end if object successfully created */

entry_exists: 

    if ((status != DNS_SUCCESS) && object_created)
    {
	clerk_delete_object(&ch_p->ch_name);
	mgmt_delete_ch(ns_p, user_p, ch_p);
    }

    if (buf_p)
	dns_free((char *)buf_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <mgmt_create_cle: @ end - return(status=%d)",status));

    return (status);
}

/*
 * ta_cle_get_nsnickname
 *
 * get the cellname
 *
 * Input:
 *    ch_p - clearinghouse structure to fill in 
 */
#define MYTEMPBUFSIZE 512

dns_status_t 
ta_cle_get_nsnickname (nsgbl_ch_t *ch_p)
{
    unsigned32		uuid_status;
    int                 cellnameLen;
    int                 len;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >ta_cle_get_nsnickname"));

    dce_cf_get_cell_name(&ch_p->ch_cellname_p, &uuid_status);
    if (uuid_status != dce_cf_st_ok) 
    {
      dce_svc_printf(CDS_S_CFGETCELLNAME_MSG, (long)uuid_status);
      return(uuid_status);
    }
    
    
    cellnameLen = sizeof(ch_p->ch_cellname);

    dnsCvtCDSFullToOpq((unsigned char *)ch_p->ch_cellname_p,
		       (dns_full_name_t *)&ch_p->ch_cellname, &cellnameLen);

    /* the HACK NSNICKNAME is the cellname */
    INS8(ch_p->ch_nsnickname.sn_flag, SN_cds);
    INS8(ch_p->ch_nsnickname.sn_length,
		len = strlen(ch_p->ch_cellname_p));
    COPY_bytes(ch_p->ch_cellname_p, ch_p->ch_nsnickname.sn_name, len);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <ta_cle_get_nsnickname: return(DNS_SUCCESS)"));

    return(DNS_SUCCESS);
}
