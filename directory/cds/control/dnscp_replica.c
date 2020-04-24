/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscp_replica.c,v $
 * Revision 1.1.7.2  1996/02/18  19:28:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:27  marty]
 *
 * Revision 1.1.7.1  1995/12/08  15:08:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:03  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:32  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:41:50  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:36:36  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:18  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:58:14  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:58:25  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:03:00  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:33:08  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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

#include <cdscp.h>
#include <assert.h>

extern int debug;

int
do_replica_cmd (
     int cmd,
     struct entblk	*entbp,		/* ptr to head of entity list */
     struct attrblk	*atrbp,		/* ptr to head of attribute list */
     struct envblk	*envbp,		/* ptr to environment block */
     struct cmdblk	*cmdbp,		/* ptr to current command block */
     struct op_blk	*outbp)		/* ptr to head of output list */
{
  unsigned char *dirP, *chP;
  enum dnsReplicaType rtype;
  struct dnsFlagStat flags;
  int status;
  unsigned char *tmpP;

/* 
 * the first entity on the entity list is the directory name
 * the 2nd is the clearinghouse name.
 */

  dirP = entbp->nxt->name_p; /* directory name */
  chP = entbp->nxt->nxt->name_p; /* clearinghouse name */
  init_flagstatus(
		    &flags,			/* ptr to the block to init */
		    (enum dnsConf)envbp->curconf,/* current confidence */
		    &envbp->curtout);		/* current timeout */
  if (cmd == CREREPCMD) { /* Add a replica */
    tmpP = reptyp_pointer();  /* replica type */
    rtype = (enum dnsReplicaType)*tmpP;
    status = dnsAddReplica((dns_opq_fname *)dirP, /* directory name */
		       (dns_opq_fname *)chP, /* clearinghouse to put it in */
		       rtype,	/* master, secondary, r/o */
		       &flags);
  }
  else {    /* Delete a replica */
    status= dnsRmReplica((dns_opq_fname *)dirP, /* directory name */
			   (dns_opq_fname *)chP, /* clearinghouse  */
			   &flags);
      }

    if (status != DNS_SUCCESS) {
      create_err_list_element (dirP,
		       &flags,
		       (cmd == CREREPCMD) ? "dnsAddReplica": "dnsRmReplica");

    }
  return (status);

}
