/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: emit_ref_hdr.c,v $
 * Revision 1.1.13.1  1996/08/09  11:56:50  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  14:55 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	Fix for OT 12819 - check for NULL from cdsGetHandle.
 * 	[1996/02/18  19:28:48  marty  1.1.11.2]
 *
 * Revision 1.1.11.2  1996/02/18  19:28:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:47  marty]
 * 
 * Revision 1.1.11.1  1995/12/08  15:08:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:24  root]
 * 
 * Revision 1.1.8.3  1994/08/03  19:00:43  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:42:14  mccann]
 * 
 * Revision 1.1.8.2  1994/06/09  18:36:46  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:30  devsrc]
 * 
 * Revision 1.1.8.1  1994/03/12  21:58:45  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:59:50  peckham]
 * 
 * Revision 1.1.6.2  1993/08/02  20:01:58  jd
 * 	Took out the code just before the call to format_utcabs_for_print in
 * 	emit_refhdr_atline that converts GMT time to local time.
 * 	format_utcabs_for_print now contains the code to do the conversion.
 * 	[1993/08/02  18:45:03  jd]
 * 
 * Revision 1.1.2.2  1992/12/30  13:04:07  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:34:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:22:55  devrcs
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
#include <dns_malloc.h>

static void
emit_refhdr_opline (
	struct op_blk	*obp,		/* ptr to output list head */
	unsigned32	code,		/* which operation it is */
	char		*txt)
{
    emit_hdr(obp, code, txt);
    /* no return value. can not tell if the write worked. */
}
#define sn_of(x) SN_OVERHEAD+sizeof(x)+1

static void
emit_refhdr_uidline (
	struct op_blk	*obp,		/* ptr to output list head */
	struct entblk	*entp)		/* ptr to entity block */
{
    int outsize;			/* where dns should put size */
    dns_handle handle;
    unsigned char  *hp;			/* ptr to handle */
    struct dnaUid out, dummyout;
    int status;
    struct dnsFlagStat readattrflgs;
    struct dnaUid *uidp1 = &out;	/* ptr to uid internal form */
    char edited_uid[37];		/* big enough to add \0 */
    char *uidp2 = &edited_uid[0];	/* ptr to uid internal form */
    dns_attrvalue_t *uid_attrval;       /* ptr to uid attribute value */

    if ( ! entp->uid_present)		/* do we know the uid already? */
	{				/* no, so go get the info */
	dnsInitFlagStat(&readattrflgs);
	handle = dnsGetHandle();	/* allow iteration to read 1 dna uid */
        if (handle == NULL)
            {
                emit_line_fmt(obp, CDSCP_NOGETHAND,
                              "No memory to get handle to nameserver.\n");
                return;
            };
	outsize = sizeof(struct dnaUid);
	status = 
	    dnsReadAttrValue(
		(dns_opq_fname *)
		    (entp->name_p),	/* the opaque full name of the ent */
		handle,			/* ptr to handle */
                (dns_attr_name_t *)dns_AttributeName(DNS_OBJECTUID),
				        /* ptr to attr name, opaque */
		dnsObj,			/* look for a dns object */
		preferred_clh_pointer(),/* use preferred/any clearinghouse */
                &uid_attrval,           /* ptr to uid attribute value */
		0,			/* no interest in cts of uid */
		0,
		0,
		&readattrflgs);
        if (status == DNS_SUCCESS)
        {
            assert (uid_attrval->val_syntax == dns_uuid);
            assert (uid_attrval->val_u.val_byte.length == sizeof(out));
            COPY_bytes (uid_attrval->val_u.val_byte.byte_p,
                        &out,
                        uid_attrval->val_u.val_byte.length);
        }

	if (status != DNS_SUCCESS)
	    {				/* no. do fixup and continue */
	    out = dummyout;		/* pretend it worked */
	    create_err_list_element (entp->name_p,
				     &readattrflgs,
				     "dnsReadAttrValue");
	    }

	dnsFreeHandle(hp);
	entp->entuid = out;		/* stash the uid in entity block */
	entp->uid_present = 1;		/* now the uid is safely stashed */
	fmt_uid_for_print((unsigned char *)uidp1, uidp2);
	emit_hdr_string(obp, CDSCP_REFHDR_UIDLINE, "UID", uidp2);
	/* add code to allow for uid attr not present */
	}
}

static void
emit_refhdr_optuidline (
	struct op_blk	*obp,		/* ptr to output list head */
	struct envblk	*ebp,		/* ptr to environment block */
	struct entblk	*entp)		/* ptr to entity block */
{
    /* the following check shold be more complex. */
    /* consider also the kind of entity. only objects have objectuid. */

    /* modify envoronment block to have argc, argv, rtn to decide this */
 
    if ((ebp->uidopt) &&		/* is uid line desired in header? */
	    (entp->kind_of_entity==(int)refhdr_objent))	/* and meaningfull? */
	{				/* yes */
	emit_refhdr_uidline(obp, entp);	/* emit the line */
	}
}

void
emit_refhdr_atline (
	struct op_blk	*obp)		/* ptr to output list head */
{
    struct utc now;
    char fmtd_time[50];
    char *p;

    /* get current time and format it */
    utc_gettime ( &now );

#ifdef NOT_DEFINED

    /* the follwoing two-line crock is a sneaky way to covert to local  **
    ** time, compliments of Joe Comuzzi. We do this so we can use the   **
    ** format_utcabs_for_print call next to guarantee a compatible DCE  **
    ** date-and-time format.                                            */

    utc_localtime ( &time, &tns, &tm_inacc, &ins, &now );
    utc_mkgmtime ( &now, &time, tns, &tm_inacc, ins );

#endif

    format_utcabs_for_print(fmtd_time,  &now);

    /* scan the formatted string to surpress the milliseconds		*/

    p = strchr((char *)fmtd_time, '.');
    if (p)
        *p = '\0';

    emit_hdr_string(obp, CDSCP_REFHDR_TSLINE, "AT", fmtd_time);
}

static void
emit_refhdr_entline (
	struct op_blk	*obp,		/* ptr to output list head */
	struct entblk	*ebp)		/* ptr to current entity block */
{
    static char nokind[] = "UNKNOWN KIND OF ENTITY";
    static struct {
	unsigned32	code;
	char		*txt;
    } kindofent[] = {
	{ CDSCP_REFHDR_ENTLINE_NOKIND, nokind },
	{ CDSCP_REFHDR_ENTLINE_DIR, "DIRECTORY" },
	{ CDSCP_REFHDR_ENTLINE_OBJ, "OBJECT" },
	{ CDSCP_REFHDR_ENTLINE_KID, "CHILD" },
	{ CDSCP_REFHDR_ENTLINE_SL, "SOFTLINK" },
	{ CDSCP_REFHDR_ENTLINE_CLH, "CLEARINGHOUSE" },
	{ CDSCP_REFHDR_ENTLINE_NS, "NAMESPACE" },
	{ CDSCP_REFHDR_ENTLINE_GRP, "GROUP" },
	{ CDSCP_REFHDR_ENTLINE_REP, "REPLICA" },
	{ CDSCP_REFHDR_ENTLINE_MREP, "MASTER REPLICA" },
	{ CDSCP_REFHDR_ENTLINE_SREP, "SECONDARY REPLICA" },
	{ CDSCP_REFHDR_ENTLINE_RREP, "READ ONLY REPLICA" },
	{ CDSCP_REFHDR_ENTLINE_EREP, "EXCLUDED REPLICA" },
	{ CDSCP_REFHDR_ENTLINE_NOKIND, nokind },
	{ CDSCP_REFHDR_ENTLINE_CELL, "CELL" }
	};
    int limit = sizeof(kindofent)/sizeof(kindofent[0]);
    char stringarea[DNS_STR_FNAME_MAX]; 
    int idx = ebp->kind_of_entity;

    if (!((0 < idx) && ((int)refhdr_min_valid_ent <= idx)
	&& (idx <= refhdr_max_valid_ent) && (idx < limit)))
	idx = 0;
    format_fullname_for_print((FullName_t *)ebp->name_p, stringarea);
    emit_hdr_string(obp, kindofent[idx].code, kindofent[idx].txt, stringarea);
    /* no return value. can not tell if the write worked. */
    return;				/* no value from void routine */
}

void
emit_reference_header (
	struct op_blk	*obp,		/* ptr to output list head */
	struct entblk	*ebp,		/* ptr to current entity block */
	unsigned32	code,
	char		*txt)
/* need to add other parameters, probably */
{
    struct envblk *envp;		/* ptr to environment block */

    envp = (struct envblk *)get_envblk_ptr(); /* get ptr to environment block */

    emit_refhdr_opline(obp, code, txt);		/* operation line */
    emit_refhdr_entline(obp, ebp);		/* entity line */
    emit_refhdr_optuidline(obp, envp, ebp);	/* optional uid line */
    emit_refhdr_atline(obp);
}
