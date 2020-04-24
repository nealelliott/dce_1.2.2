/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: visit_cds.c,v $
 * Revision 1.1.12.1  1996/08/09  11:57:24  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  15:13 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	Fix for OT 12819 - check for NULL from cdsGetHandle.
 * 	[1996/02/18  19:30:41  marty  1.1.10.2]
 *
 * Revision 1.1.10.2  1996/02/18  19:30:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:00  marty]
 * 
 * Revision 1.1.10.1  1995/12/08  15:12:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:10  root]
 * 
 * Revision 1.1.8.6  1994/09/15  15:22:39  mccann
 * 	change CDS_CellAlias field strings
 * 	[1994/09/15  15:13:09  mccann]
 * 
 * Revision 1.1.8.5  1994/08/16  18:27:44  jd
 * 	drop from janet
 * 	[1994/08/13  16:05:08  jd]
 * 
 * Revision 1.1.8.4  1994/08/03  19:01:27  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:43:45  mccann]
 * 
 * Revision 1.1.8.3  1994/06/23  18:29:53  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:34:01  mccann]
 * 
 * Revision 1.1.8.2  1994/06/09  18:37:52  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:56  devsrc]
 * 
 * Revision 1.1.8.1  1994/03/12  22:01:22  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:07:38  peckham]
 * 
 * Revision 1.1.6.2  1993/08/16  12:31:23  jd
 * 	(3163) emit_1_attr_line wasn't checking for DNS_ERROR dnsReadAttrValue
 * 	nor was emit_some_attr_lines checking the result of emit_1_attr_line.
 * 	Both checks are now done. emit_1_attr_line queues the error. Checked
 * 	other calls to emit_1_attr_line for consistent check of return. They
 * 	are fine.
 * 	[1993/08/16  12:06:43  jd]
 * 
 * Revision 1.1.4.3  1992/12/30  13:26:43  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:42:58  zeliff]
 * 
 * Revision 1.1.4.2  1992/11/17  17:21:57  m_sawyer
 * 	Removed control-c checks.
 * 	[1992/11/17  15:15:17  m_sawyer]
 * 
 * Revision 1.1.2.3  1992/05/01  14:02:36  m_sawyer
 * 	fix 'with' processing
 * 	[1992/05/01  14:01:43  m_sawyer]
 * 
 * Revision 1.1.2.2  1992/03/22  21:54:47  weisman
 * 	           Big merge with post-1.0 development at Digital.
 * 	   [1992/03/22  20:23:36  weisman]
 * 
 * Revision 1.1  1992/01/19  15:23:02  devrcs
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
 * 
 */

#include <cdscp.h>
#include <cmdblk.h>
#include <assert.h>
#include <uc_clerk.h>

/*
 * prototypes for static functions
 */
static void cvt_to_dns_attrvalue_t
    (dns_attrvalue_t **,
     AtomicValue_t *);

static int emit_attr_lines
    (struct entblk *,
     struct attrblk *,
     struct envblk *,
     struct cmdblk *,
     struct op_blk *);

static int emit_some_attr_lines
    (int,
     struct entblk *,
     struct attrblk *,
     struct envblk *,
     struct cmdblk *,
     struct op_blk *);

static int emit_1_attr_line
    (struct entblk *,
     struct attrblk *,
     struct envblk *,
     struct cmdblk *,
     struct op_blk *);

static void expand_wildsname
    (struct entblk *,
     struct entblk *,
     dns_opq_fname *,
     dns_opq_sname *);

static int selected_ent
    (struct entblk  *,
     struct attrblk *,
     struct envblk  *,
     struct op_blk  *);

static int test_by_dnstest_pos
    (struct entblk  *,
     struct attrblk *,
     struct envblk  *);

static int bs_cmp
    (const void *,
     const void *);

static int related_entity
    (int);


struct knownattr
{
  int		index;		/* index into cds_attlist[] */
  void		(*outrtn)(struct op_blk *,char *,dns_attrvalue_t *);
};

static struct knownattr kattr[] =
{
  {DNA_TOWERS, emit_cds_tower},
  {DNS_CHLASTADDRESS, emit_cds_tower},
  {DNS_CHSTATE, emit_cds_chstate},
  {DNS_CLASS, emit_cds_sname},
  {DNS_CONVERGENCE, emit_cds_convergence},
  {DNS_INCHNAME, emit_cds_inch},
  {DNS_REPLICASTATE, emit_cds_repstate},
  {DNS_REPLICATYPE, emit_cds_reptype},
  {DNS_CELLALIASES, emit_cds_cellaliases},
  {-1, 0}
};	/* last entry > any attribute name */

#define NUMGBLATR (sizeof(kattr)/sizeof(struct knownattr))

dns_opq_fname partialRes; /* partial results from the API returned here */


struct dnsFlagStat *
init_flagstatus (
struct dnsFlagStat *fp,		/* ptr to the flags to be inited */
enum dnsConf	   conf,	/* confidence to use */
int		   *wait)	/* ptr to seconds to wait */
{
    dnsInitFlagStat(fp);

    fp->fsConf		= conf;		/* current confidence */
    fp->fsWait		= *wait;	/* current timeout wait */
    fp->fsDontCache	= (conf==dnsConfHi) ? 1 : 0;/* clerk to cache or not */
    fp->fsPartialRes	= &partialRes;  /* return partial results */
    fp->fsResLength	= sizeof(partialRes);
    return(fp);				/* maybe the caller wants to use it */
}

/* In v2 attributes in the kattr table are in alphabetic order.  In
 * v3, ordering should be by objectid, but this is now reflected in
 * kattr.  It is reflected in cds_attlist though.  So we will search
 * cds_attlist with a binary search, then find the corresponding entry
 * in kattr.  Note, kattr is a sub-set
 */
struct knownattr *
get_known_attribute (
dns_opq_sname *name_p,             /* Name to search for */
attribute_descriptor_t **dns_pp)   /* cds_attlist entry if found */
{
  int i;
  struct knownattr *found_p;

  found_p = (struct knownattr *)NULL;
  /* Binary search table */
  *dns_pp = bsearch(name_p,
                    (char *)dns_Attribute(0),
                    DNS_MAXATTRIBUTES,
                    sizeof(attribute_descriptor_t),
                    bs_cmp);

  /* Now map to knownattr table */
  if (*dns_pp)
    for (i=0; i<NUMGBLATR; ++i)
      if ((*dns_pp)->atype == kattr[i].index) {
	found_p = &kattr[i];
	break;
      }

  return(found_p);
}

static int
emit_1_attr_line (
struct entblk	*entbp,		/* ptr to entity */
struct attrblk	*atrbp,		/* ptr to an attribute block */
struct envblk	*envbp,		/* ptr to environment info */
struct cmdblk	*cmdbp,		/* ptr to command block */
struct op_blk	*outbp)		/* ptr to output block */
{
  int status;
  int worst;
  char stringattrname[DNS_STR_ANAME_MAX];
  unsigned char dottyattrname[DNS_STR_ANAME_MAX];
  attribute_descriptor_t *dns_p;
  int dottylen = sizeof(dottyattrname);
  int avail = sizeof(stringattrname);
  struct dnsFlagStat flags;
  dns_attrvalue_t *av_bufptr;
  dns_handle hp;
  int entType;
  dns_opq_fname *optchp;
  int numinset;


  if (cmdbp->func == SHOREPCMD)
    optchp = (dns_opq_fname *)entbp->nxt->name_p;
  else
    optchp = preferred_clh_pointer();

  entType = related_entity(entbp->kind_of_entity);

  worst = DNS_SUCCESS;
  init_flagstatus(&flags, (enum dnsConf)envbp->curconf,  &envbp->curtout);

  /* Get string name for display */
    status = dnsCvtObjIDToStr((dns_attr_name_t *)&atrbp->bigattr,
                              NULL, (unsigned char *)stringattrname, &avail,
                              dottyattrname, &dottylen);

  /* If no string translation (null string), use dotty string */
  if (strlen(stringattrname) == 0)
    memcpy(stringattrname, dottyattrname, dottylen);
  worst = baddest(status, worst);

  hp = dnsGetHandle();
  if (hp == NULL) {
      emit_line_fmt(outbp, CDSCP_NOGETHAND,
                    "Failed in a cdsGetHandle operation\n");
      worst = DNS_ERROR;
  }

  if (worst == DNS_SUCCESS) {
    for (numinset = 0, status = DNS_SUCCESS;
	 status == DNS_SUCCESS; numinset++ ) {
      status = 	dnsReadAttrValue((dns_opq_fname *)entbp->name_p, hp,
				 (dns_attr_name_t *)&atrbp->bigattr,
				 (enum dnsEntries)entType, optchp,
				 &av_bufptr, (dns_cts **)NULL,
                                      (unsigned char *)NULL,
                                      (int *)NULL, &flags);

      if (status == DNS_SUCCESS) {
	struct knownattr *found;/* ptr to proper item in table */

                found = get_known_attribute((dns_opq_sname *)&atrbp->bigattr,
                                            &dns_p);

	if (found)
	  (*found->outrtn)(outbp, stringattrname, av_bufptr);
	else
	  emit_cds_values(outbp, stringattrname, av_bufptr);
      } else if ((status == DNS_EMPTY) && (numinset == 0)) {
	/*  Display empty set text */
        emit_name_string(outbp, stringattrname, "<empty set>");
      } else if (status == DNS_ERROR) {
	create_err_list_element(entbp->name_p, &flags, "dnsReadAttrValue");
	if(hp != NULL)
	  dnsFreeHandle(hp);

	return(status);
      } else if (status != DNS_EMPTY) {
        emit_name_string(outbp, stringattrname, "");
        create_err_list_element(entbp->name_p, &flags, "dnsReadAttrValue");
      }
    } /* End for loop */
  }  /* End worst = success */

  if (status != DNS_EMPTY)
    worst = baddest(status, worst);

  /* Cleanup */
  if (hp != NULL)
    dnsFreeHandle(hp);

  return(worst);
}

static int
emit_some_attr_lines (
int		attrcode,	/* attribute group */
struct entblk	*entbp,		/* ptr to entity */
struct attrblk	*atrbp,		/* ptr to an attribute block */
struct envblk	*envbp,		/* ptr to environment info */
struct cmdblk	*cmdbp,		/* ptr to command block */
struct op_blk	*outbp)		/* ptr to output block */
{
  dns_handle hp;
  int result;
  int status;
  int atrcode;
  int *atp = &atrcode;
  struct sname *osnap;
  struct dnsFlagStat flags;
  int worst= DNS_SUCCESS;
  int entType;

  init_flagstatus(&flags, (enum dnsConf)envbp->curconf, &envbp->curtout);

  entType = related_entity(entbp->kind_of_entity);

  hp = dnsGetHandle();
  if (hp != NULL) {
    for (status = DNS_SUCCESS; status == DNS_SUCCESS;  ) {
      status = dnsEnumAttr((dns_opq_fname *)entbp->name_p, hp,
                                 (enum dnsEntries)entType,
                                 (enum dnsAttrType *)atp,
                                 (dns_attr_name_t **)&osnap,
                                 (unsigned char *)NULL,
			   (int *)NULL, &flags);
      if (status != DNS_EMPTY)
	worst = baddest(status, worst);	/* remember any bad news */
      if (status == DNS_SUCCESS) {
	memcpy(&atrbp->bigattr, osnap, DNS_OPQ_ANAME_MAX);
                result = emit_1_attr_line(entbp, atrbp, envbp, cmdbp, outbp);
	worst =   baddest(result, worst);
	if(worst == DNS_ERROR)
	  return(worst);
      }
      else if (status == DNS_ERROR)
              create_err_list_element(entbp->name_p, &flags,
                                      "dnsEnumAttr");
    }				/* end of for loop */
    /* we get here iff get handle worked */
    dnsFreeHandle(hp);
  } else {
    emit_line_fmt(outbp, CDSCP_NOGETHAND,
	"Failed in a dnsGetHandle operation\n");
    return DNS_ERROR;
  }

  return(worst);
}

static int
emit_attr_lines (
struct entblk	*entbp,		/* ptr to entity */
struct attrblk	*atrbp,		/* ptr to an attribute block */
struct envblk	*envbp,		/* ptr to environment info */
struct cmdblk	*cmdbp,		/* ptr to command block */
struct op_blk	*outbp)		/* ptr to output block */
{
    int status;
    int attrcode;
    struct attrblk workattr;

    /*
     * Make a copy of attribute block
     */
    workattr = *atrbp;
    workattr.fwd  = &workattr;
    workattr.bck  = &workattr;
    workattr.head = &workattr;
    workattr.blkalloc = 0;

    /*
     * Read one attribute or enumerate and read all attributes
     */
    if (atrbp->grpattr == EXPLICIT_ATTR)
    {
        status = emit_1_attr_line(entbp, &workattr, envbp, cmdbp, outbp);
    }
    else
    {
        workattr.grpattr = ALL_ATTR;
        status = emit_some_attr_lines(attrcode, entbp, &workattr,
                                      envbp, cmdbp, outbp);
        if ((status == DNS_SUCCESS) &&
            (entbp->kind_of_entity == dnsCH))
        {
            status = cp_server_cmd (SHOWSRVCLE, entbp, atrbp, envbp, outbp);
        }
    }

    return(status);
}

/* Support for showing multiple attributes.  Used by show replica and
 * show entity commands
 */
int
visit_attr_list (
struct entblk	*entbp,		/* ptr to entity */
struct attrblk	*atrbp,		/* ptr to head of attribute list */
struct envblk	*envbp,		/* ptr to environment info */
struct cmdblk	*cmdbp,		/* ptr to command block */
struct op_blk	*outbp)		/* ptr to output block */
{
    struct attrblk *abp;
    int status;
    int worst = DNS_SUCCESS;

    /*
     * Loop through list of attribute blocks
     */
    for (abp = atrbp->fwd; abp != atrbp; abp = abp->fwd)
    {
        status = emit_attr_lines(entbp, abp, envbp, cmdbp, outbp);

  worst = baddest(status, worst);
    }
  return(worst);
}

/*
 * This routine expands a wildcarded entry into a list of individual entries
 * to be worked on.
 * Currently only the shoent command comes through here
 */
int
visit_entlist (
struct entblk	*entbp,		/* ptr to head of entity list */
struct attrblk	*withp,		/* ptr to attribute filter block */
struct attrblk	*atrbp,		/* ptr to head of attribute list */
struct envblk	*envbp,		/* ptr to environment info */
struct cmdblk	*cmdbp,		/* ptr to command block */
struct op_blk	*outbp)		/* ptr to output block */
{
    struct entblk *p;			/* ptr to current entity block */
    struct entblk *hp;			/* ptr to head of entity list */
    int status;				/* result from called subroutine */
    int worst = DNS_SUCCESS;		/* the worst status seen so far */

    hp = entbp->head;			/* get to head, if not there now */
    p = hp->nxt;

    /*
     * If name contains a wildcard, expand it into a list of names
     */
    if (p->wild) {
        expand_wildsname(p->nxt, p, (dns_opq_fname *)p->name_p,
                         &p->wildname);
        p = p->nxt;
    }

    /*
     * list may grow , process each entry
     */
    for (; p != hp; p = p->nxt)
    {
        /*
         * See if this entity satisifies pre-conditions
         */
        if (selected_ent(p, withp, envbp, outbp))
        {
            emit_blank_line(outbp);
            emit_reference_header(outbp, p, CDSCP_REFHDR_OPLINE_SHOW, "SHOW");
            status = visit_attr_list(p, atrbp, envbp, cmdbp, outbp);
            worst = baddest(status, worst);
        }
    }   /* end of for loop */

    return(worst);
}

int
set_attr_line (
struct entblk	*entbp,		/* ptr to entity */
struct attrblk	*atrbp,		/* ptr to an attribute block */
struct envblk	*envbp,		/* ptr to environment info */
struct cmdblk	*cmdbp)		/* ptr to command block */
{
  int status;
  struct dnsFlagStat flags;
  enum dnsAttrType attrtype;
  dns_attrvalue_t *av_vp;
  int worst = DNS_SUCCESS;
  char *opname;
  int entType;

  init_flagstatus(&flags, (enum dnsConf)envbp->curconf, &envbp->curtout);

  entType = related_entity(entbp->kind_of_entity);

  /* Does user want to remove an entire attribute? */
  if (atrbp->av == (struct singlevalueblk *)NULL)   {
      opname = "dnsRmAttr";
      /* No way to tell what type of attribute it is, so try set first */
      status = dnsRmAttr((dns_opq_fname *)entbp->name_p,
			      (dns_attr_name_t *)&atrbp->bigattr,
			      (enum dnsAttrType)dnsSet,
			      (enum dnsEntries)entType, NULL, &flags);
      if ((status != DNS_SUCCESS) &&
	  (flags.fsLocStat == DNS_WRONGATTRIBUTETYPE))
	/* Try again as Single - no way to now in advance */
	status = dnsRmAttr((dns_opq_fname *)entbp->name_p,
			       (dns_attr_name_t *)&atrbp->bigattr,
			       (enum dnsAttrType)dnsSingle,
			       (enum dnsEntries)entType, NULL, &flags);
    } else {
      /* Convert into api atomicvalue */
      cvt_to_dns_attrvalue_t(&av_vp, atrbp->av->vp);

      if (cmdbp->func == REMSETCMD) {
	opname = "dnsRmAttrValue";
	status = dnsRmAttrValue((dns_opq_fname *)entbp->name_p,
			    (dns_attr_name_t *)&atrbp->bigattr,
			    (enum dnsEntries)entType, av_vp,
			    NULL, &flags);
      } else if ((cmdbp->func == SETENTCMD) &&
		 (entbp->kind_of_entity == (int)dnsDir) &&
		 (dnsCmpSimple((dns_opq_sname *)&atrbp->bigattr,
		    (dns_opq_sname *)dns_AttributeName(DNS_INCHNAME)) == 0)) {
	if (EXT8(atrbp->av->vp->av_value)) {
	  opname = "dnsAllowCH";
	  status = dnsAllowCH((dns_opq_fname *)entbp->name_p, &flags);
	} else {
	  opname = "dnsDisallowCH";
	  status = dnsDisallowCH((dns_opq_fname *)entbp->name_p, &flags);
	}
      } else {
	opname = "dnsAddAttrValue";
	if (cmdbp->func == ADDSETCMD)
	  attrtype = dnsSet;
	else			/* SETENTCMD */
	  attrtype = dnsSingle;
	status = dnsAddAttrValue((dns_opq_fname *)entbp->name_p,
				 (dns_attr_name_t *)&atrbp->bigattr, attrtype,
				 (enum dnsEntries)entType, av_vp,
				 NULL, &flags);
      }
    }

  /* Queue up an error report, if necessary */
  if (status != DNS_SUCCESS)
    create_err_list_element (entbp->name_p, &flags, opname);

  worst = baddest(worst, status);
  return(worst);
}

/* Process DIR commands */
int
enum_things (
struct entblk	*entbp,		/* ptr to entity */
struct attrblk	*withp,		/* ptr to attribute filter block */
struct envblk	*envbp,		/* ptr to environment info */
struct cmdblk	*cmdbp,		/* ptr to command block */
struct op_blk	*outbp)		/* ptr to output block */
{
    int status = DNS_SUCCESS;
    int numnames;
    dns_handle hp;
    dns_opq_fname *entp;
    dns_opq_sname *eachp;
    dns_opq_sname sname;
    dns_opq_fname dirname;
    int dirlen=sizeof(dirname);
    int simplelen=sizeof(sname);
    struct dnsFlagStat flags;
    char buf[999];
    struct entblk newent;
    dns_opq_fname newname;
    
    /* Make a dummy entity block to deal with with clause */
    ent_one_onetime(&newent, 0);
    newent.name_p = (unsigned char *)&newname;
    
    init_flagstatus(&flags, (enum dnsConf)envbp->curconf, &(envbp->curtout));
    
    entp = (dns_opq_fname *) entbp->name_p;
    
    if (entbp->wild)
    {
        COPY_SimpleName(wild_sname_pointer(), &sname);
    }
    else
    {
        /* Get count of names for scatter */
        numnames = dnsCountSimple(entp);
        
        /* Strip off last name */
        status = dnsScatter(entp, numnames - 1, &dirname, &dirlen,
                            &sname, &simplelen, NULL,
                            (int *)NULL);
        
        if (status != DNS_SUCCESS)
        {
            dns_status_clerk ( &flags, status );
            create_err_list_element((unsigned char *)entp, &flags, "dnsScatter");
            return(status);
        }
        entp = &dirname;
    }
    
    hp = dnsGetHandle();
    if (hp == NULL) {
        emit_line_fmt(outbp, CDSCP_NOGETHAND,
                      "Failed in a cdsGetHandle operation\n");
        status = DNS_ERROR;
    }
    
    /*
     * Determine which enumerate is required and emit
     * Keep going until an error is found or exhaust directory.
     */
    while (status == DNS_SUCCESS)
    {
        switch (cmdbp->func)
        {
            case DIRDIRCMD:
            {
                status = dnsEnumChild(entp, hp, &sname, &eachp,
                                      (unsigned char *)NULL,
                                      (int *)NULL, &flags);
                break;
            }
            case DIROBJCMD:
            {
                status = dnsEnumObj(entp, hp, NULL, &sname,
                                    (int *)NULL, &eachp,
                                    (unsigned char *)NULL,
                                    (int *)NULL,
                                    (dns_opq_sname **)NULL,
                                    (unsigned char *)NULL,
                                    (int *)NULL, &flags);
                break;
            }
            case DIRLNKCMD:
            {
                status = dnsEnumSLink(entp, hp, &sname, &eachp,
                                      (unsigned char *)NULL,
                                      (int *)NULL, &flags);
                break;
            }
            case DIRCLHCMD:
            {
                status = dnsEnumObj(entp, hp,
                                    (dns_opq_sname *)cds_classlist[DNS_CLEARINGHOUSE].cname,
                                    &sname, (int *)NULL, &eachp,
                                    (unsigned char *)NULL,
                                    (int *)NULL, (dns_opq_sname **)NULL,
                                    (unsigned char *)NULL, (int *)NULL,
                                    &flags);
                break;
            }
            case DIRGRPCMD:
            {
                status = dnsEnumObj(entp, hp,
                                    (dns_opq_sname *)cds_classlist[DNS_GROUP].cname,
                                    &sname, (int *)NULL, &eachp,
                                    (unsigned char *)NULL,
                                    (int *)NULL, (dns_opq_sname **)NULL,
                                    (unsigned char *)NULL, (int *)NULL,
                                    &flags);
                break;
            }
            default:
            {
                status = DNS_ERROR;
                break;
            }
        }
        
        if (status == DNS_SUCCESS)
        {
            int newlen = sizeof(newname);
            newent.kind_of_entity = entbp->kind_of_entity;
            
            /*
             * need to build an ent block so we can filter with clauses
             */
            dnsGather(entp, eachp, NULL, &newname, &newlen);
            if (selected_ent(&newent, withp, envbp, outbp))
            {
                format_simplename_for_print(buf, eachp);
                strcat(buf, "\n");
                emit_line(outbp, buf);
            }
        }
    } /* End for loop */
    
    /* Cleanup and process return */
    dnsFreeHandle(hp);
    return(status == DNS_EMPTY ? DNS_SUCCESS : status);
}

/*
 *
 *  This routine converts an unsigned counted character string to the
 *  new api format. Ie. it converts from v2api to v3api format.
 */
static void
cvt_to_dns_attrvalue_t (
dns_attrvalue_t **v3api_attrvalue_pp,
AtomicValue_t *value_p)
{
    static dns_attrvalue_t v3api_attrvalue;

    dns_TLV2AttrVal(EXT8(value_p->av_valuetype),
                    EXT16(value_p->av_length),
                    value_p->av_value, &v3api_attrvalue);

    *v3api_attrvalue_pp = &v3api_attrvalue;
}


/*
 * This routine performs an enumerate of a directory for entries stored
 * there.  It would be called SHOENT functions.
 */
static void
expand_wildsname (
    struct entblk *insrtp,		/* new blocks go ahead of here */
    struct entblk *entbp,		/* ptr to ent with wildcard */
    dns_opq_fname *leftpart,		/* ptr to full name left of wild */
    dns_opq_sname *name)		/* ptr to simple name to expand */
{
    struct entblk *newentp;		/* ptr to new entity block */
    int status;
    int result;
    int code;
    int   fullsize;
    dns_opq_fname fullresult;
    dns_handle handle;
    dns_opq_sname *pointer;		/* ptr to the enumerated name */
    struct dnsFlagStat fs;
    char *nameofenum;			/* error code for when an enum fails */
    struct envblk *envbp;		/* ptr to environment block */

    envbp = get_envblk_ptr();
    init_flagstatus(&fs, (enum dnsConf)envbp->curconf, &envbp->curtout);

    handle = dnsGetHandle();
    if (handle == NULL) {
        cds_status_clerk ( &fs, CDS_ERROR );
        create_err_list_element((unsigned char *)entbp->name_p, &fs, "cdsGetHandle");
        return;
    }
    init_flagstatus(&fs, (enum dnsConf)	envbp->curconf, &envbp->curtout);
    for (;;) {
      /* Perform enumeration based on kind of entry we are looking for */
      if ((enum dnsEntries)entbp->kind_of_entity == dnsDir) {
	nameofenum = "dnsEnumChild";	/* in case of error */
	status = dnsEnumChild(leftpart, handle, name, &pointer,
			      (unsigned char *)NULL, (int *)NULL, &fs);
      } else if ((enum dnsEntries)entbp->kind_of_entity == dnsObj) {
	nameofenum = "dnsEnumObj";	/* in case of error */
	status = dnsEnumObj(leftpart, handle, NULL, name,
			    (int *)NULL, &pointer, (unsigned char *)NULL,
			    (int *)NULL, (dns_opq_sname **)NULL,
			    (unsigned char *)NULL, (int *)NULL, &fs);
      } else if (entbp->kind_of_entity == refhdr_grpent) {
	nameofenum = "dnsEnumObj";	/* in case of error */
	status = dnsEnumGrp(leftpart, handle, name, &pointer,
			    NULL, NULL, &fs);
      } else if ((enum dnsEntries)entbp->kind_of_entity == dnsSLink) {
	  nameofenum = "dnsEnumSLink";
	  status = dnsEnumSLink(leftpart, handle, name, &pointer,
				NULL, NULL, &fs);
	} else if ((enum dnsEntries)entbp->kind_of_entity == dnsCH) {
	  nameofenum = "dnsEnumObj";	/* in case of error */
	  status = dnsEnumObj(leftpart, handle,
		      (dns_opq_sname *)cds_classlist[DNS_CLEARINGHOUSE].cname,
		      name, NULL, &pointer, NULL,
		      NULL, NULL, NULL, NULL, &fs);
	} else if ((enum dnsEntries)entbp->kind_of_entity == dnsChildPtr) {
	  nameofenum = "dnsEnumChild";	/* in case of error */
	  status = dnsEnumChild(leftpart, handle, name, &pointer,
			     NULL, NULL, &fs);
	} else {
	  dns_status_clerk ( &fs, status );
	  create_err_list_element(entbp->name_p, &fs, "dnsScatter");
	}

      /* the proper kind of enumeration has been done */
      if (status == DNS_SUCCESS) {
	fullsize = sizeof(fullresult);
	result = dnsGather(leftpart, pointer, (dns_opq_fname *)NullFullName,
			   &fullresult, &fullsize);
	if (result != DNS_SUCCESS)  {
	  dns_status_clerk ( &fs, status);
	  create_err_list_element((unsigned char *)leftpart, &fs, "dnsGather");
	} else {
	  /* Build a new entity block for latter processing */
	  newentp = (struct entblk *)copyent_new(entbp);
	  if (newentp) {
	    newentp->wild = 0;
	    code = move_name_to_entblk((unsigned char *)&fullresult, newentp);
	    if (code)
	      linkent_before(newentp, insrtp);
	    else {
	      dns_status_clerk ( &fs, CDS_ERROR );
	      create_err_list_element((unsigned char *)leftpart, &fs, "dnsGather");
	    }
	  } else {
	    dns_status_clerk ( &fs, CDS_ERROR );
	    create_err_list_element((unsigned char *)leftpart, &fs, "dnsGather");
	  }
	}
      }	else if (status == DNS_EMPTY) break;
      else  {
	/* Enumerate failed, report error */
	create_err_list_element(entbp->name_p, &fs, nameofenum);
	break;
      }
    } /* End for loop */

    /* Cleanup */
    dnsFreeHandle(handle);
    return;				/* no value from void routine */
}					/* end of expand_wildsname */

/*
 * Determine if this entity should be processed based on
 * what is in the with clause
 *
 */
static int
selected_ent (
struct entblk	*entbp,		/* ptr to entity */
struct attrblk	*withp,		/* ptr to attribute filter block */
struct envblk	*envbp,		/* ptr to environment info */
struct op_blk	*outbp)		/* ptr to output block */
{
    int test_result=TRUE;	/* result from subroutine */
    struct attrblk *p;		/* ptr to current with block */
    
    /*
     * Step through with list, any failure causes this entry to be
     * bypassed
     */
    for (p = withp->fwd; p != withp; p = p->fwd) {
        if (p->relop == RELOP_EQ) {	/* is there a with clause ? */
            test_result = test_by_dnstest_pos(entbp, p, envbp);
            if (!test_result) break;
        }
    } /* End for */
    return(test_result);
}

/*
 * Test if an entry and attribute have a specific value
 */
/* does WITH clause say use this? */
static int
test_by_dnstest_pos (
struct entblk	*entbp,		/* ptr to entity */
struct attrblk	*withp,		/* ptr to attribute filter block */
struct envblk	*envbp)		/* ptr to environment info */
{
    int status;
    struct dnsFlagStat flags;
    int entType;
    dns_attrvalue_t *attrvalue_p;

    /* Initialize for call */

    init_flagstatus(&flags, (enum dnsConf)envbp->curconf, &envbp->curtout);
    entType = related_entity(entbp->kind_of_entity);
    cvt_to_dns_attrvalue_t(&attrvalue_p, withp->av->vp);

    status = dnsTestAttrValue((dns_opq_fname *)	entbp->name_p,
			      (dns_attr_name_t *)&withp->bigattr,
			      (enum dnsEntries)entType,	attrvalue_p,
                              &flags);

    return(status == DNS_SUCCESS);
}

/*
 * Comparision routine for binary search off cds_attlist
   */
static int
bs_cmp (
    const void *arg1_p,
    const void *arg2_p)
{
    const dns_opq_sname *lookingfor = arg1_p;
    const attribute_descriptor_t *tableitem = arg2_p;
    dns_opq_sname *p;
    int code;

    /* Get name from table */
    p = (dns_opq_sname *)tableitem->aname;

    /* compare names and return results */
    code = dnsCmpSimple(lookingfor, p);
    return(code);
}

static int
related_entity (
int code)
{
    int goodcode;
    static int entity[] = {
	0,				/* invalid */
	(int)dnsDir,			/* directory */
	(int)dnsObj,			/* object */
	(int)dnsChildPtr,		/* child pointer */
	(int)dnsSLink,			/* soft link */
	(int)dnsCH,			/* clearinghouse */
	0,				/* namespace */
	(int)dnsObj,			/* group */
	(int)dnsDir,			/* replica */
	(int)dnsDir,			/* master replica */
	(int)dnsDir,			/* secondary replica */
	(int)dnsDir,			/* read-only replica */
	(int)dnsDir,			/* excluded replica */
	0};				/* invalid */

    goodcode = ((code >= (int)refhdr_min_valid_ent) &&
		(code <= (int)refhdr_max_valid_ent))
		? code : 0 ;
    return ( entity[goodcode] );
}

int					/* 0 to n, or -1 if none */
find_leftmost_wild (
    dns_opq_fname *fnp)			/* ptr to opq full name to examine */
{
    int return_code;			/* what we tell the caller */
    int numnames;			/* number of simple names in full */
    int skip;				/* number of simple names before wild */
    int status;				/* status of scatter operation */
    int len;				/* in/out, size of name buffer/ name */
    struct dnsFlagStat fs;		/* fake flags for error reports */
    unsigned char 
	name[DNS_OPQ_SNAME_MAX+2];	/* put each simple name here */
    struct envblk *envbp;		/* ptr to environment block */

    envbp = get_envblk_ptr();		/* needed only to init flagstat */
    numnames = 
	cdsCountSimple(fnp);		/* the number of simple names */
    /* we need to find the leftmost simple name with a wild card */
    init_flagstatus(			/* used only if scatter fails */
	    &fs,			/* ptr to flags */
	    (enum dnsConf)
		envbp->curconf,		/* current confidence */
	    &(envbp->curtout));		/* current timeout wait */
    for (skip=0; skip<numnames; skip++)	/* check each simple name in turn */
	{
	len = sizeof(name);
	status =
	    cdsScatter(			/* extract a simple name */
		fnp,			/* ptr to full name with wildcard */
		skip,			/* this many have been seen already */
		0,			/* dont care about left hand end */
		0,			/* or its length */
		(dns_opq_sname *)
		    &name[0],		/* put the selected simple name here */
		&len,			/* and its length here */
		0,			/* dont care about right hand end */
		0);			/* or its length */
	/* failure should be impossible, but just in case ... */
	if (status != DNS_SUCCESS)
	    {				/* try to report the error */
	    dns_status_clerk ( &fs, status );
	    create_err_list_element((unsigned char *)fnp, &fs, "dnsScatter");
	    skip = numnames;		/* force no wildcard found */
	    break;			/* and get out */
	    }
	if ((name[0] == 4) || (name[0] == 5))	/* *, %, or ... */
	    break;			/* found it, and skip tells where */
	}				/* end of for loop */
    return_code =
	((numnames < 0) || (skip >= numnames)) ? -1 : skip;
    return(return_code);
}
