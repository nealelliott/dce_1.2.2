/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscp_credir.c,v $
 * Revision 1.1.8.2  1996/02/18  19:28:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:14  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:07:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:51  root]
 * 
 * Revision 1.1.6.4  1994/08/03  19:00:26  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:38:51  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:29:30  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:33:26  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:36:28  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:11  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:57:57  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:57:43  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:02:22  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:32:22  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/01  15:40:07  m_sawyer
 * 	Don't use freed memory [in free_lists()].
 * 	[1992/06/01  15:38:55  m_sawyer]
 * 
 * Revision 1.1  1992/01/19  15:22:51  devrcs
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

/* do_dir_cmd:
 * Handles: create directory, delete directory, skulk directory,
 * and set to new epoch.
 */


#include <cdscp.h>

#define NEW 1
#define OLD 2

extern int debug;

/* Linked list used to contain old, new epoch information */

typedef struct ep {
  struct ep *link;
  int r_type;  /* Replica type */
  dns_opq_fname ch_name; /* clearinghouse name */
} epochDesc;

epochDesc *old_epoch_head = NULL; /* head of old epoch list */
epochDesc *old_epoch_tail = (epochDesc *) &old_epoch_head; /* tail of old epoch list */
epochDesc *new_epoch_head = NULL; /* head of new epoch list */
epochDesc *new_epoch_tail = (epochDesc *) &new_epoch_head; /* tail of new epoch list */

/*
 * Internal routines
 */
static int credir(unsigned char *,unsigned char *,enum dnsConf,int *);
static int deldir(unsigned char *,enum dnsConf,int *);
static int skulkdir(unsigned char *,enum dnsConf,int *);
static int new_epoch(struct entblk *,unsigned char *,enum dnsConf,int *,struct op_blk *);
static int get_new_epoch(struct entblk *);
static int get_old_epoch(unsigned char *,enum dnsConf,int *);
static int check_for_duplicates(char *);
static int check_master(void);
static int check_for_missing_exclusions(char *);
static int check_for_bad_exclusions(char *);
static int set_new_epoch(unsigned char *,enum dnsConf,int *);
static int free_lists(void);
static int enq(int,unsigned char *,int);


int
do_dir_cmd (
     int cmd,
     struct entblk	*entbp,		/* ptr to head of entity list */
     struct attrblk	*atrbp,		/* ptr to head of attribute list */
     struct envblk	*envbp,		/* ptr to environment block */
     struct cmdblk	*cmdbp,		/* ptr to current command block */
     struct op_blk	*outbp)		/* ptr to head of output list */

{
  unsigned char * dirP, * chP;
  int * tblk;
  enum dnsConf conf;

  dirP = entbp->nxt->name_p; /* directory name */
  chP = entbp->nxt->nxt->name_p; /* clearinghouse name */
  conf = (enum dnsConf)envbp->curconf;
  tblk = &envbp->curtout;
  switch (cmd) {
  case V1CREDIRCMD: /* Create dir */
    return (credir (dirP, chP, conf, tblk));
    break;
  case V1DELDIRCMD: /* Delete dir */
    return (deldir (dirP, conf, tblk));
    break;
  case SKULKCMD:
    return (skulkdir (dirP, conf, tblk));
    break;
  case NEWEPOCHCMD:
    return (new_epoch (entbp, dirP, conf, tblk, outbp));
  default: 
    return (DNS_ERROR);
  }
}

/*****************************************************************************/

static int
credir (
unsigned char * dirp, /* Directory name */
unsigned char * chp,  /* clearinghouse name */
enum dnsConf conf,
int *tblk)
{
    int status;
    struct dnsFlagStat flags, *ignore;	/* misc parameters for dns */

    ignore =				/* &flags is returned */
	init_flagstatus(
	    &flags,			/* ptr to the block to init */
	    conf,		/* current confidence */
	    tblk);		/* current timeout */
    status = 
	dnsCreateDir(
	    (dns_opq_fname *)dirp,	/* ptr to directory name */
	    (dns_opq_fname *)chp,	/* ptr to clearinghouse name, or 0 */
	    (dns_id_t *)NULL,
	    &flags);			/* ptr to in/out flag/status */


    if (status != DNS_SUCCESS) {

/* Queue up an error report */

      create_err_list_element (dirp,
			       &flags,
			       "dnsCreateDir");
    }

    return(status);			/* tell caller what happened */    
    }

/*****************************************************************************/

static int
deldir (
unsigned char * dirp, /* Directory name */
enum dnsConf conf,
int *tblk)
{
    int status;				/* the portable interface tells us */
    struct dnsFlagStat flags, *ignore;	/* misc parameters for dns */

    ignore =				/* &flags is returned */
	init_flagstatus(
	    &flags,			/* ptr to the block to init */
	    conf,		/* current confidence */
	    tblk);		/* current timeout */
    status = 
	dnsDelDir(			/* delete the specified directory */
	    (dns_opq_fname *)dirp,	/* ptr to directory name */
	    &flags);			/* ptr to in/out flag/status */

    if (status != DNS_SUCCESS) {
      create_err_list_element (dirp,
			       &flags,
			       "dnsDelDir");
    }
    return(status);			/* tell caller what happened */    
}

/*****************************************************************************/

static int
skulkdir (
unsigned char * dirP,
enum dnsConf conf,
int *tblk)
{
  struct dnsFlagStat flags, *ignore;
  int status;
  
  ignore =				/* &flags is returned */
    init_flagstatus(
		    &flags,		/* ptr to the block to init */
		    conf,		/* current confidence */
		    tblk);		/* current timeout */
  status = dnsSkulkDir ((dns_opq_fname *)dirP, &flags);

    if (status != DNS_SUCCESS) {
      create_err_list_element (dirP,
			       &flags,
			       "dnsSkulkDir");
    }

  return (status);
}

/*****************************************************************************/
/* Set directory to new epoch.

 * 1) Make sure there are no duplicate clearinghouses in the new epoch
 * 2) Make sure there is only one master in new epoch.
 * 3) Gather old epoch info (read dns$replicas attribute set from direct.)
 * 4) For each replica in old epoch, make sure it exists in the new epoch.
 * 5) For each replica of type 'EXCLUDE' in new epoch, make sure it exists
 *    in the old epoch.
 * 6) If above tests succeed, set new epoch.
*/

static int
new_epoch (
struct entblk *entbp, /* ptr to head of entity list */
unsigned char *dirP,  /* directory name */
enum dnsConf conf,    /* current confidence */
int *tblk, /* current timeout block */
struct op_blk *obp)   /* output block */
{
  char ch_name[DNS_STR_FNAME_MAX];
  int status;

  obp = get_op_blk_ptr();
/* Gather info about new epoch
*/
  get_new_epoch (entbp);

/* Check for duplicate clearinghouse names */

  if (check_for_duplicates (ch_name) == DNS_ERROR) {
    emit_line_fmt(obp, CDSCP_DUPCL, "Clearinghouse %s duplicated\n", ch_name);
    free_lists();
    return (DNS_ERROR);
  }

/* Check number of master replica types. */

  if (check_master () == DNS_ERROR) {
    emit_line_fmt(obp, CDSCP_TWO_MASTERS,
	"More than one master replica specified\n");
    free_lists();
    return (DNS_ERROR);
  }

/* Gather old epoch information */

  if (get_old_epoch (dirP, conf, tblk) == DNS_ERROR) {
    /* get_old_epoch reports errors */
    free_lists();
    return (DNS_ERROR);
  }

/* Check for missing clearinghouse exclusions in new epoch */

  if (check_for_missing_exclusions (ch_name) == DNS_ERROR) {
    emit_line_fmt(obp, CDSCP_MISEXCL,
	"Clearinghouse %s exclusion argument missing\n", ch_name);
    free_lists();
    return (DNS_ERROR);
  }

/* Check for incorrect exclusions in new epoch */

  if (check_for_bad_exclusions (ch_name) == DNS_ERROR) {
    emit_line_fmt(obp, CDSCP_NOREP,
	"No replica of specified directory exists at clearinghouse %s\n",
	ch_name);
    free_lists();
    return (DNS_ERROR);
  }

/* DNS_SUCCESS- set new epoch on directory */

  status = set_new_epoch (dirP, conf, tblk);
  free_lists (); /* release list memory */
  return (status);
}

/*****************************************************************************/

/* get_new_epoch: gather info specified in the new epoch; save in
 * linked list anchored at: 'new_head'.
*/

static int
get_new_epoch (
     struct entblk *entbp) /* ptr to head of entity list */
{
  struct entblk *p;

  p = entbp->nxt; /* ptr to 1st entity = directory name */

/* (entity list is circular) */

  while ((p = p->nxt) != entbp) { /* For each entity specified */
    /* Get only replica type entities, as there may be other stuff, too */
    switch (p->kind_of_entity) {
    case (dnsMasterRep):
    case (dnsSecondRep):
    case (dnsReadRep):
    case (dnsExcludeRep):
      enq (NEW, p->name_p, p->kind_of_entity); /* save name, type in queue */
      break;
    }
  }

}
/*****************************************************************************/

/* get_old_epoch: gather info about the existing epoch; save in
 * linked list anchored at: 'old_head'.
*/

static int
get_old_epoch (
     unsigned char * dirP, /* name of directory */
     enum dnsConf conf,    /* current confidence */
     int *tblk) /* current timeout block */

{
  struct dnsFlagStat flags, *ignore;
  dns_attrvalue_t *value;
  ReplicaPointer_t *rp_p;
  unsigned char *rtype_p, *name_p;
  dns_handle hp;
  int status = DNS_SUCCESS;

  ignore =		/* &flags is returned */
    init_flagstatus(
		    &flags,		/* ptr to the block to init */
		    conf,		/* current confidence */
		    tblk);		/* current timeout */

  if ((hp = dnsGetHandle()) == NULL) {
#ifdef DEBUG
    if (debug) printf ("dnsGetHandle failed\n");
#endif
    return (DNS_ERROR);
  }
  while (status == DNS_SUCCESS) {
    status = dnsReadAttrValue (
			    (dns_opq_fname *)dirP,
			    hp,
                            (dns_attr_name_t *)dns_AttributeName(DNS_REPLICAS),
			    dnsDir,
			    preferred_clh_pointer(), /* any/preferred CH */
			    &value,		/* returned value */
			   (dns_cts **) NULL, /* CTS */
			   (unsigned char *) NULL, /* CH tower */
			   (int *) NULL,	/* tower len */
			   &flags);
    if (status != DNS_SUCCESS) {
      if (status != DNS_EMPTY) {
	create_err_list_element (dirP, &flags, "dnsReadAttrValue");
	return (DNS_ERROR);
      }
      else return (DNS_SUCCESS);
    }
/* Extract the replica type and clearinghouse name from the attribute record.
 * (use macros from 'dns_record.h')
*/
    if (value->val_syntax != dns_ReplicaPointer) continue;
    rp_p = (ReplicaPointer_t *)value->val_u.val_byte.byte_p;
    rtype_p = (unsigned char *)FLD_ReplicaPointer_replicaType (rp_p);
    name_p  = (unsigned char *)FLD_ReplicaPointer_CHName (rp_p); /* and finally the name */
    enq (OLD, name_p, (int)(*rtype_p));
  }
  return (DNS_SUCCESS);
}

/************************************************************************/
/* Check new epoch for duplicate clearinghouse names 
 * Return DNS_ERROR on error.
*/

static int
check_for_duplicates (
     char * dup_name) /* OUTPUT: duplicate clh name or NULL */
     /* note: dup_name buffer must be DNS_STR_FNAME_MAX size */
{
  epochDesc *p, *p1;
  int len = DNS_STR_FNAME_MAX;

  for (p=new_epoch_head; p != NULL; p = p->link) {
    for (p1=new_epoch_head; p1 != NULL; p1 = p1->link) {
      if (p == p1) continue; /* don't compare against self */
      if (dnsCmpFull (&p->ch_name, &p1->ch_name) == 0) {
	  dnsCvtCDSFullToStr((dns_opq_fname *)&p->ch_name, (unsigned char *)dup_name, &len);
	  return (DNS_ERROR);
      }
    }
  }
  return (DNS_SUCCESS);
}

/************************************************************************/
/* Check for one and only one master
*/
static int
check_master (void)
{
  epochDesc *p;
  int mcnt = 0;

  for (p=new_epoch_head; p != NULL; p = p->link) {
    if (p->r_type == (int)dnsMasterRep) ++mcnt;
  }
  if (mcnt != 1) return (DNS_ERROR);	/* must find one and only one */
  else return (DNS_SUCCESS);

}
/************************************************************************/
/* Check new epoch for missing exclusions. 
 * Make sure each clearinghouse name mentioned in the old epoch
 * exists in the new epoch.
*/

static int
check_for_missing_exclusions (
char *ch_name) /* OUTPUT: name of missing exclusion (string format),
		or NULL, if none */
{
  epochDesc *op, *np;
  int match;
  int len = DNS_STR_FNAME_MAX;

  for (op=old_epoch_head; op != NULL; op = op->link) {
    match = FALSE;
    for (np=new_epoch_head; np != NULL; np = np->link) {
      if (dnsCmpFull (&op->ch_name, &np->ch_name) == 0) {
	match = TRUE;
	break; /* check next old ch name */
      }
    }
    if (match == FALSE) { /* found a missing exclusion */
      dnsCvtCDSFullToStr ((dns_opq_fname *)&op->ch_name, (unsigned char *)ch_name, &len);
      return (DNS_ERROR);
    }
  }
  return (DNS_SUCCESS);
}

/************************************************************************/
/* Check for improper exclusions in new epoch.
*/

static int
check_for_bad_exclusions (
     char * ch_name) /* OUTPUT: bad exclusion name or NULL */
{
  epochDesc *op, *np;
  int match, len = DNS_STR_FNAME_MAX;

  for (np=new_epoch_head; np != NULL; np = np->link) { /* for each new name */
    if (np->r_type == dnsExcludeRep) { /* if type = exclude */
      match = FALSE;
      for (op=old_epoch_head; op != NULL; op = op->link) { /* check each old name*/
	if (dnsCmpFull (&np->ch_name, &op->ch_name) == 0) {
	  match = TRUE;
	  break;
	}
      }
      if (match == FALSE) { /* found improper exclusion */
	dnsCvtCDSFullToStr ((dns_opq_fname *)&np->ch_name, (unsigned char *)ch_name, &len);
	return (DNS_ERROR);
      }
    }
  }
  return (DNS_SUCCESS);
}
/************************************************************************/
/* Set new epoch on directory
 * NOTE: all replica information specified in the list is assumed
 * to have been prevalidated according to rules specified above.
*/

static int
set_new_epoch (
unsigned char *dirP, /* the opaque full name of the directory */
enum dnsConf conf,    /* current confidence */
int *tblk) /* current timeout block */
{
  epochDesc *p, *np;
  unsigned char  *master_ch;
  unsigned char **secondary_ch, **secP;
  unsigned char **readonly_ch, **ronlyP;
  int cnt, status;
  struct dnsFlagStat flags, *ignore;

/* Count no. of replicas so we can allocate some arrays.
 * This seems like overkill, but I don't trust static sizing.
*/
  for (cnt=0, np=new_epoch_head; np != NULL; ++cnt, np = np->link) {}

/* each array is as large as the replica count */
  secondary_ch = (unsigned char **)malloc((cnt+1)*sizeof(char *));
  readonly_ch = (unsigned char **)malloc((cnt+1)*sizeof(char *));

/* Find the master 
 * Get the secondaries and read-onlys into arrays 
 */
  for (p=new_epoch_head, secP=secondary_ch, ronlyP=readonly_ch;
       p != NULL;
       p = p->link)
    {
      switch (p->r_type) {
      case (dnsMasterRep):
	master_ch = (unsigned char *)&p->ch_name;
	break;
      case (dnsSecondRep):
	*secP++ = (unsigned char *)&p->ch_name;
	break;
      case (dnsReadRep):
	*ronlyP++ = (unsigned char *)&p->ch_name;
	break;
      }
    }
  *secP = *ronlyP = NULL;

/* Initialize flags */

  ignore =		/* &flags is returned */
    init_flagstatus(
		    &flags,		/* ptr to the block to init */
		    conf,		/* current confidence */
		    tblk);		/* current timeout */

  status = dnsNewEpoch (
			(dns_opq_fname *)dirP,
			(dns_opq_fname *)master_ch,
			(dns_opq_fname **)secondary_ch,
			(dns_opq_fname **)readonly_ch,
			&flags
		      );

    if (status != DNS_SUCCESS) {
      create_err_list_element (dirP,
			       &flags,
			       "dnsNewEpoch");

    }
/* Free memory */
  free ((char *)secondary_ch);
  free ((char *)readonly_ch);
  return (status);
}


/************************************************************************/
/* Release the linked lists.
*/
static int
free_lists (void)
{
  epochDesc *p, *nxtp;

  if (new_epoch_head) {
    for (p=new_epoch_head, nxtp=p->link; p != NULL; p=nxtp) {
      nxtp = p->link;
      free ((char *)p);
    }
  }

  if (old_epoch_head) {
    for (p=old_epoch_head, nxtp=p->link; p != NULL; p=nxtp) {
      nxtp = p->link;
      free ((char *)p);
    }
  }
  old_epoch_head = NULL;
  old_epoch_tail = (epochDesc *) &old_epoch_head;
  new_epoch_head = NULL;
  new_epoch_tail = (epochDesc *) &new_epoch_head;
}

/************************************************************************/
/* Enqueue a replica spec to specified queue*/

static int
enq (
int queue,   /* queue id 'NEW' or 'OLD' */
unsigned char *name, /* clearinghouse name */
int type)  /* replica type */
{
  epochDesc *new;

  new = (epochDesc *) malloc (sizeof (epochDesc));
  new->r_type = type;
  new->link = NULL;
  COPY_FullName (name, (unsigned char *)&new->ch_name);
  if (queue == NEW) {
    new_epoch_tail->link = new;
    new_epoch_tail = new;
  }
  else {
    old_epoch_tail->link = new;
    old_epoch_tail = new;
  }
}
