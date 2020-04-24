/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscp_crekid.c,v $
 * Revision 1.1.12.1  1996/08/09  11:56:38  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  14:50 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	Fix for OT 12819 - check for cdsGetHandle returning NULL.
 * 	[1996/02/18  19:28:05  marty  1.1.10.2]
 *
 * Revision 1.1.10.2  1996/02/18  19:28:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:16  marty]
 * 
 * Revision 1.1.10.1  1995/12/08  15:07:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:53  root]
 * 
 * Revision 1.1.7.3  1994/08/03  19:00:27  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:39:02  mccann]
 * 
 * Revision 1.1.7.2  1994/06/09  18:36:30  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:12  devsrc]
 * 
 * Revision 1.1.7.1  1994/03/12  21:58:00  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:57:50  peckham]
 * 
 * Revision 1.1.4.3  1993/02/15  21:20:05  keegan
 * 	Fix bogus 'on behalf of' pointer, which caused a crash.
 * 	[1993/02/15  21:19:11  keegan]
 * 
 * Revision 1.1.4.2  1992/12/30  13:02:29  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:32:28  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:45:20  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:20:58  weisman]
 * 
 * Revision 1.1  1992/01/19  15:22:52  devrcs
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
 *
 */

/* dnscp_crekid.c is used to create a child pointer. */

#include <cdscp.h>
#include <uc_clerk.h>

#ifdef DCE_SEC
# include <dce/id_base.h>
#endif


/*
 * Locate any replica of a directory and build a set containing
 * that replica for later input to create child.
 */
static int
build_rpset(
    struct envblk *envbp,		/* ptr to environment block */
    dns_opq_fname *dirp,		/* ptr to opaque fullname of dir/kid */
    dns_opq_fname *clhp,		/* ptr to opqfullname clearinghouse */
    Set_t *set_p)
    {
    struct dnsFlagStat enum1flags;	/* in/out flags/status for enum */
    dns_handle enum1hp;			/* for continuity in enum loop */
    int enum1status;			/* the portable interface tells us */
    dns_attrvalue_t *repptr;		/* ptr to current replica */
    ReplicaPointer_t *rp_p;
    MemberValue_t *member_p;

    init_flagstatus(
	    &enum1flags,		/* ptr to the block to init */
	    envbp->curconf,		/* current confidence */
	    &(envbp->curtout));		/* current timeout */
    enum1hp = dnsGetHandle();		/* prepare to read replica set */
    if (enum1hp == NULL) {
        return DNS_ERROR;
    }
    enum1status = DNS_SUCCESS; 
    while (enum1status == DNS_SUCCESS) {
	enum1status =
	    dnsReadAttrValue(
		(dns_opq_fname *)
		    dirp,		/* ptr to the name of the child dir */
		enum1hp,		/* continuity handle */
                                        /* ptr to attr simple name */
                (dns_attr_name_t *)dns_AttributeName(DNS_REPLICAS),
		dnsDir,			/* kind of entity code */
		preferred_clh_pointer(),  /* use the specified clearinghouse */
		&repptr,		/* address of ptr to replica */
	        (dns_cts **)NULL,
		(unsigned char *)NULL,	/* feign interest in tower */
	       (int *)NULL,		/* and its size */
		&enum1flags);		/* ptr to in/out flags */
	/* If we got a replica, check for master */
	if (enum1status == DNS_SUCCESS)	
	    {
	    rp_p = (ReplicaPointer_t *)repptr->val_u.val_byte.byte_p;

	    /* Build a set with one member */
	    INS8(set_p->st_flag, ST_present);
	    INS16(set_p->st_offsetLength, sizeof(word_u));
	    INS16(set_p->st_valueLength, sizeof(SetMember_overhead) + 
		  LEN_ReplicaPointer(rp_p));
	    INS16(OFFSET_Set(set_p, 0), sizeof(Set_overhead) + sizeof(word_u));
	    member_p = (MemberValue_t *)INDEX_Set(set_p, 0);
	    INS8(member_p->mv_flag, MV_present);
	    dnsCreateCTS((dns_cts *)member_p->mv_timestamp);
	    INS16(member_p->mv_length, LEN_ReplicaPointer(rp_p));
	    INS8(member_p->mv_valuetype, dns_Attribute(DNS_REPLICAS)->vtype);
	    COPY_ReplicaPointer(rp_p, member_p->mv_value);
	    break;
	    }
	else if (enum1status != DNS_EMPTY)
	    {
	    create_err_list_element((unsigned char *)dirp, &enum1flags, "dnsReadAttrValue");
	    break;		       /* abnormal, bad way out of loop */
	    }			       /* there might be more reps we ignore */
      }				       /* end of first enum for loop */
    dnsFreeHandle(enum1hp);	       /* give back resources consumed */
    return(enum1status);
  }

int
crekid (
    struct entblk	*entbp,		/* ptr to head of entity list */
    struct attrblk	*atrbp,		/* ptr to head of attribute list */
    struct envblk	*envbp,		/* ptr to environment block */
    struct cmdblk	*cmdbp,		/* ptr to current command block */
    struct op_blk	*outbp)		/* ptr to head of output list */
{

    /* the first entity on the entity list is the child pointer */
    /* the second entity is the clearinghouse to use. */
    /* for now we do not use the attributes list */

    struct  entblk *ent1p;		/* ptr to the child pointer entity */
    struct  entblk *ent2p;		/* ptr to the clearinghouse entity */
    dns_opq_fname *dirp;		/* ptr to full name of directory */
    dns_opq_fname *clhp;		/* ptr to clearinghouse opaque name */
    struct dnsFlagStat flags;   	/* misc parameters for dns */
    dns_handle  ctshp;			/* for continuity in set reads loop */
    int status = DNS_ERROR;		/* status from pi */
    Set_t *set_p;
    dns_attrvalue_t *valueptr;

#ifdef DCE_SEC
    /* principal; this user */
    sec_id_foreign_t onbehalf_of,*on_p = &onbehalf_of;
#else
    dns_opq_fname onbehalf_of, *on_p = &onbehalf_of;
#endif

    ent1p = entbp->nxt;			/* point to child pointer entity blk */
    dirp  = (dns_opq_fname *)ent1p->name_p; /* point to directory name */
    ent2p = ent1p->nxt;			/* point to clearinghouse entity blk */
    clhp  = (dns_opq_fname *)ent2p->name_p; /* point to clearinghouse name */

    /* Allocate maximum storage for a set with one replicapointer member */
    set_p = (Set_t *)malloc(sizeof(Set_u) + sizeof(ReplicaPointer_u) +
			    sizeof(SetMember_u));
    if (!set_p) {
      emit_line_fmt(outbp, CDSCP_NOMEM_REPSET,
	  "No memory for building replica set.\n");
      return(DNS_ERROR);
    }

    init_flagstatus(
	    &flags,		        /* ptr to the block to init */
	    envbp->curconf,		/* current confidence */
	    &(envbp->curtout));		/* current timeout */
    ctshp = dnsGetHandle();		/* provide a buffer */
    if (ctshp == NULL) {
        emit_line_fmt(outbp, CDSCP_NOGETHAND,
                      "No memory to get handle to nameserver.\n");
        return DNS_ERROR;
    }
    status =  dnsReadAttrValue(
	    dirp,			/* ptr to the name of the child ptr */
	    ctshp,			/* continuity handle */
	    (dns_attr_name_t *)dns_AttributeName(DNS_OBJECTUID),
	    dnsDir,			/* kind of entity code */
	    preferred_clh_pointer(),	/* use the specified clearinghouse */
	    &valueptr,
	    (dns_cts_t **)NULL,		/* no interest in cts */
	    (unsigned char *)NULL,
	    (int *)NULL,
	     &flags);	        	/* ptr to in/out flags */
    /* need to free the handle on both paths of the following if */
    if (status == DNS_SUCCESS)		/* as it should be */
	{
	/* Create replica set with 1 member */
	if (build_rpset(envbp, dirp, clhp, set_p) == DNS_SUCCESS) {

	    /* build the principal name for the onbehalf_of field */
#ifdef  DCE_SEC
	    /* no user passed */
	    on_p = (sec_id_foreign_t *)NULL;
#else
            /*
             * CDS doesn't care about "onbehalf_of"
             */
            COPY_FullName(NullFullName, &onbehalf_of);
#endif


	    /* try to create the child pointer */
	    status =
		cdsCreateChild(
		    dirp,		/* ptr to the name of the dir */
		    on_p,		/* on behalf of */
		    &valueptr->val_u.val_uuid,
		    (dns_opq_fname *)	/* (silly anachronism) */
			set_p,      	/* ptr to replica set */
		    LEN_Set(set_p),	/* redundant size of the above */
		    NULL_CDS_ID,	
		    &flags);		/* ptr to in/out flags */
	    if (status != DNS_SUCCESS) {
		create_err_list_element(	/* report the problem */
		    (unsigned char *)dirp,	/* ptr to name of the entity */
		    &flags,		/* ptr to info re what went wrong */
		    "cdsCreateChild");	/* code for kind of operation */
		}
	}				/* end of got a set */
      } else {
	create_err_list_element(	/* report the problem */
	    (unsigned char *)dirp,	/* ptr to name of the entity */
	    &flags,	        	/* ptr to info re what went wrong */
	    "dnsReadAttrValue");		/* code for kind of operation */
      }
    dnsFreeHandle(ctshp);		/* give back the cts buffer */
    free(set_p);
    return ( status );
}
