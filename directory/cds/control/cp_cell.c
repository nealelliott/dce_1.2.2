/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cp_cell.c,v $
 * Revision 1.1.10.2  1996/02/18  19:27:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:00  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:30  root]
 * 
 * Revision 1.1.8.5  1994/08/03  19:00:17  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:36:28  mccann]
 * 
 * Revision 1.1.8.4  1994/06/09  18:36:19  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:07:59  devsrc]
 * 
 * Revision 1.1.8.3  1994/04/19  17:05:24  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:25:39  mccann]
 * 
 * Revision 1.1.8.2  1994/03/22  19:59:56  griffin
 * 	HCell BL3 Support: Add/remove cellname
 * 	[1994/03/22  17:46:36  griffin]
 * 
 * Revision 1.1.8.1  1994/03/12  21:57:42  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:56:36  peckham]
 * 
 * Revision 1.1.6.2  1993/08/13  21:30:01  jd
 * 	(6498) In showcell don't double use the status work. After finding the
 * 	       the cell doesn't exist free the handle and return. Don't go to
 * 	       bottom of the function and trip over the double use of status.
 * 	       Everything in between the discovery of the non-existant cell and
 * 	       the bottom of the function was skipped in any case.
 * 	[1993/08/13  21:24:36  jd]
 * 
 * Revision 1.1.4.2  1992/12/30  13:01:36  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:31:15  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:40:38  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:19:36  weisman]
 * 
 * Revision 1.1  1992/01/19  15:20:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
 */

#include <cdscp.h>
#include <cmdblk.h>
#include <tower.h>
#include <dce/cprpcp.h>
#include <dce/cprpc_hcell.h>
#include <dce/rpc_cvt.h>

dns_status_t
showcell(
     int                type,           /* type of gda entry to display */
     struct entblk	*entbp,		/* ptr to entity */
     struct envblk	*envbp,		/* ptr to environment info */
     struct op_blk	*outbp)		/* ptr to output block */
     
{
  dns_status_t status;
  int i;
  int found = 0;
  dnsFlagStat_t flags;
  dns_attrvalue_t *bufptr;
  dns_handle_t hp;		
  char txtbuf[2*DNS_STR_FNAME_MAX];
  char uidbuf[DNS_STR_FNAME_MAX];
  unsigned char **str_array;
  ReplicaPointer_t *rp_p;
  Set_t *set_p;

  /* Get internal representation of local root */
  hp = dnsGetHandle();
  if (!hp)
    return ( DNS_ERROR );

  /* Initialize flags */
  init_flagstatus(&flags, (enum dnsConf)envbp->curconf,  &envbp->curtout);

  status = dnsReadAttrValue(
		(dns_opq_fname *)entbp->name_p, hp, 
		(dns_attr_name_t *)dns_AttributeName(DNS_OBJECTUID),
		dnsDir, preferred_clh_pointer(),
		&bufptr, (dns_cts **)NULL,
		(unsigned char *)NULL,(int *)NULL, &flags);

  if (status != DNS_SUCCESS) {
      create_err_list_element(entbp->name_p, &flags, "dnsReadAttrValue");
      dnsFreeHandle(hp);	/* Cleanup */
      return(status);
  } else {
      /* Remember the namespace UUID */

      fmt_uid_for_print((unsigned char *)&bufptr->val_u.val_uuid, uidbuf);
      
      dnsClearHandle(hp);

      /* Now loop to read all replica pointers */

      while (status == DNS_SUCCESS) {
	  status = dnsReadAttrValue(
			(dns_opq_fname *)entbp->name_p, hp, 
			(dns_attr_name_t *)dns_AttributeName(DNS_REPLICAS),
			dnsDir, preferred_clh_pointer(),
			&bufptr, (dns_cts **)NULL,
			(unsigned char *)NULL,(int *)NULL, &flags);

	  if (( status == DNS_SUCCESS ) &&
	      ( bufptr->val_syntax == dns_ReplicaPointer )) {

	      /* Insert namespace UUID */

	      strcpy ( &txtbuf[2], uidbuf);

	      /* count replicas	and parse replica pointer */

	      if ( found++ ) emit_blank_line ( outbp );

	      rp_p = (ReplicaPointer_t *)bufptr->val_u.val_byte.byte_p;

	      /* If X500 print in pieces, if Domain NS, concatonate all text */

	      if (type == SHOW500) {

		  emit_attr_string(outbp,
			CDSCP_X500_CELL1_LINE, "Namespace Uuid", &txtbuf[2]);

		  /* Expand clearinghouse uuid */

		  fmt_uid_for_print(rp_p->rp_CHID, txtbuf);
		  emit_attr_string(outbp,
			CDSCP_X500_CELL2_LINE, "Clearinghouse Uuid", txtbuf);

		  /* Expand clearinghouse name */

		  format_fullname_for_print(FLD_ReplicaPointer_CHName(rp_p), 
					    txtbuf);
		  emit_attr_string(outbp,
			CDSCP_X500_CELL3_LINE, "Clearinghouse Name", txtbuf);

		  /* Display master or readonly replica text 	*/

	          if (EXT8(FLD_ReplicaPointer_replicaType(rp_p)) == RT_master)
		      cds_sprintf(txtbuf, CDSCP_MASTER_REP, "  Master  ");
		  else
		      cds_sprintf(txtbuf, CDSCP_READONLY_REP, " Readonly ");

		  emit_attr_string( outbp,
			CDSCP_X500_CELL4_LINE, "Replica Type", txtbuf );

		  /* now get the protocol strings from the towers */

		  set_p = (Set_t *)rp_p->rp_addressHint;
                  if (dnsCvtTowerToStr(set_p,
				       (byte_t **)&str_array ) == DNS_SUCCESS) {
                    for ( i=0; str_array[i] != NULL; i++ ) {
                      emit_attr_string( outbp,
			CDSCP_X500_CELL5_LINE, "Tower", (char *)str_array[i] );
                    }
                  free ((char *)str_array);
                  }
	      } else {
		  txtbuf[0] = '1';
		  txtbuf[1] = ' ';

		  /* Add a blank and insert master or readonly replica text */

		  strcat(txtbuf, " ");

	          if (EXT8(FLD_ReplicaPointer_replicaType(rp_p)) == RT_master)
		      cds_strcat ( txtbuf, CDSCP_MASTER_REP, "  Master  " );
		  else
		      cds_strcat ( txtbuf, CDSCP_READONLY_REP, " Readonly " );

		  /* Expand clearinghouse name */

		  format_fullname_for_print (
			FLD_ReplicaPointer_CHName(rp_p), strchr(txtbuf, '\0'));

		  /* Add a blank and new null at the end */

		  strcat(txtbuf, " ");

		  /* Expand clearinghouse uuid */

		  fmt_uid_for_print(rp_p->rp_CHID, strchr(txtbuf, '\0'));
		  emit_attr_string(outbp,
			CDSCP_DNS_CELL_LINE, "TXT", txtbuf);
	      }
	  }			/* end of one replica */
      }				/* end read replica loop */
  }				/* end found objectuid */
	      
  if (( status == DNS_EMPTY ) || ( status == DNS_SUCCESS_STALE )) {

	/*  If no replcas found display empty set text */

	if ( !found ) emit_line_fmt(outbp, CDSCP_NOMASTER,
			"No master in directory replica set\n");
	else ( status = DNS_SUCCESS );

  } else {
	create_err_list_element(entbp->name_p, &flags, "dnsReadAttrValue");
  }

  dnsFreeHandle(hp);	/* Cleanup */
  
  return ( status );
}	

dns_status_t
cp_hiercell (
	 int                type,	/* type of gda entry to display */
	 struct entblk	    *entbp,	/* ptr to entity */
	 struct envblk	    *envbp,	/* ptr to environment info */
	 struct op_blk	    *outbp	/* ptr to output block */
	 )
{
    handle_t          rpc_binding;
    dns_handle_t      hp;
    dnsFlagStat_t     flags;
    dns_attrvalue_t  *bufptr;
    ReplicaPointer_t *rp_p;
    error_status_t    rpc_status;
    command_status_t  command_status;
    cds_FullName_t    alias_name;
    cds_FullName_t    master_chname;
    uuid_t            master_chid;
    unsigned char     opq_cellname[DNS_OPQ_FNAME_MAX];
    int               fullnamesize = DNS_OPQ_FNAME_MAX;
    dns_status_t      status = DNS_SUCCESS;

    /* Get internal representation of local root */
    status = dnsCvtCDSFullToOpq((unsigned char *) "/.:",
				(dns_opq_fname *) opq_cellname,
				&fullnamesize);
    if (status != DNS_SUCCESS) {
	    create_err_list_element(entbp->name_p, &flags, DNSREADATTRVALUE);
	    dnsFreeHandle(hp);	/* Cleanup */
	    return(status);
    }

    hp = dnsGetHandle();
    if (hp == NULL_DNS_HANDLE)
      return ( DNS_ERROR );

    /* Initialize flags - use high confidence */
    init_flagstatus(&flags, cdsConfHi, &envbp->curtout);

    /* Now loop to read all replica pointers and get master replica */

    while (status == DNS_SUCCESS) {

	status = dnsReadAttrValue(
		     (dns_opq_fname *)opq_cellname, hp,
		     (dns_attr_name_t *)dns_AttributeName(DNS_REPLICAS),
		     dnsDir, preferred_clh_pointer(),
		     &bufptr, (dns_cts **)NULL,
		     (unsigned char *)NULL,(int *)NULL, &flags);

	if (( status != DNS_SUCCESS ) ||
	    ( bufptr->val_syntax != dns_ReplicaPointer )) {
	    create_err_list_element(entbp->name_p, &flags, DNSREADATTRVALUE);
	    dnsFreeHandle(hp);	/* Cleanup */
	    return(status);
	}

	rp_p = (ReplicaPointer_t *)bufptr->val_u.val_byte.byte_p;
	if (EXT8(FLD_ReplicaPointer_replicaType(rp_p)) != RT_master) {
	    continue;
	}

	/* Align and convert clearinghouse uuid */
	memcpy (&master_chid, &rp_p->rp_CHID, sizeof (master_chid));

	/* Convert clearinghouse name for exporting via RPC */
	cdsExport_FullName (NULL,
			    FLD_ReplicaPointer_CHName(rp_p),
			    &master_chname);
	break;

    }  /* end while */

    if (( status == DNS_EMPTY ) || ( status == DNS_SUCCESS_STALE )) {
	create_err_list_element(entbp->name_p, &flags, DNSREADATTRVALUE);
	dnsFreeHandle(hp);	/* Cleanup */
	return(status);
    }

    /* Convert alias name for exporting via RPC */
    cdsExport_FullName (NULL,
			(FullName_t *) entbp->name_p,
			&alias_name);

    /*
     * get authenticated binding handle
     */
    rpc_binding = cprpc_get_binding_handle (1,
					    &master_chname,
					    &rpc_status);
    if (rpc_status != rpc_s_ok) {
        mgmt_error(rpc_status);
        return (rpc_status);
    }

    rpc_binding_set_object(rpc_binding, &master_chid, &rpc_status);
    if (rpc_status != rpc_s_ok) {
        mgmt_error(rpc_status);
        return (rpc_status);
    }

    switch (type) {
      case ADDCELLPREFERCMD:
      case ADDCELLALIASCMD:
      case ADDCELLCHILDCMD:
	rpc_status = cp_AddCellname(rpc_binding,
				    &alias_name,
				    type,
				    &master_chid,
				    &command_status);
	break;

      case REMCELLCMD:
	rpc_status = cp_RemoveCellname(rpc_binding,
				       &alias_name,
				       &master_chid,
				       &command_status);
	break;
      default:
	break;
    }  /* end switch */

    /*
     * Check for RPC error
     */
    if (rpc_status != rpc_s_ok) {
	if (type == REMCELLCMD) {
	    mgmt_error (rpc_status);
	}
	else {
	    mgmt_error(rpc_status);
	}
	return (rpc_status);
    }
    if (command_status != DNS_SUCCESS) {
	if (type == REMCELLCMD) {
	    mgmt_error(command_status);
	}
	else {
	    mgmt_error(command_status);
	}
	return (command_status);
    }

    dnsFreeHandle(hp);	/* Cleanup */
    return ( status );

}  /* end cp_hiercell */
