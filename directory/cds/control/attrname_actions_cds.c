/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: attrname_actions_cds.c,v $
 * Revision 1.1.10.2  1996/02/18  19:27:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:55  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:16  root]
 * 
 * Revision 1.1.8.11  1994/09/13  20:20:29  mccann
 * 	fix quoted strings and towers
 * 	[1994/09/13  20:19:46  mccann]
 * 
 * Revision 1.1.8.10  1994/09/06  17:36:55  proulx
 * 	fix timestamps
 * 	[1994/09/06  17:28:40  proulx]
 * 
 * 	fix timestamps
 * 
 * 	fix timestamps
 * 
 * 	fix timestamp
 * 
 * Revision 1.1.8.9  1994/08/18  14:34:49  mccann
 * 	change type
 * 	[1994/08/18  14:19:35  mccann]
 * 
 * Revision 1.1.8.8  1994/08/18  13:12:11  jd
 * 	Added a couple of casts.
 * 	[1994/08/18  13:02:43  jd]
 * 
 * Revision 1.1.8.7  1994/08/17  20:36:37  mccann
 * 	handle VT_byte attributes correctly
 * 	[1994/08/17  20:34:27  mccann]
 * 
 * Revision 1.1.8.6  1994/08/16  18:27:42  jd
 * 	drop from janet
 * 	[1994/08/13  16:05:00  jd]
 * 
 * Revision 1.1.8.5  1994/08/03  19:00:08  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:36:03  mccann]
 * 
 * Revision 1.1.8.4  1994/06/09  16:08:19  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:53:06  devsrc]
 * 
 * Revision 1.1.8.3  1994/05/12  21:10:59  peckham
 * 	Remove dnstables reference.
 * 	[1994/05/12  19:14:00  peckham]
 * 
 * Revision 1.1.8.2  1994/04/19  17:05:22  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:25:25  mccann]
 * 
 * Revision 1.1.8.1  1994/03/12  21:57:29  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:55:50  peckham]
 * 
 * Revision 1.1.6.1  1993/09/22  20:57:15  pwang
 * 	Advanced pcbp->maxcur to 'tower' string when a error is detected
 * 	in the string. [OT#6499]
 * 	[1993/09/22  20:56:39  pwang]
 * 
 * Revision 1.1.4.2  1992/12/30  13:01:08  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:30:36  zeliff]
 * 
 * Revision 1.1.2.5  1992/06/29  19:45:09  m_sawyer
 * 	Configure control program message facility to do exact
 * 	matching on message keys and make "adjustments" to accommodate
 * 	this change where necessary.
 * 	[1992/06/29  19:41:55  m_sawyer]
 * 
 * Revision 1.1.2.4  1992/04/30  20:35:21  m_sawyer
 * 	fix 'with' processing
 * 	[1992/04/30  20:27:12  m_sawyer]
 * 
 * Revision 1.1.2.3  1992/04/29  17:27:48  grober
 * 	Modified calls to sscanf to pass a short int rather than
 * 	an int since sscanf is looking for a short int.  This caused
 * 	problems creating user defined attributes of type short and small.
 * 	[1992/04/21  18:41:21  grober]
 * 
 * Revision 1.1.2.2  1992/03/22  21:38:01  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:11:47  weisman]
 * 
 * Revision 1.1  1992/01/19  15:22:47  devrcs
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
#include <ctype.h>
#include <assert.h>
#include <dce/utc.h>
#include <sys/time.h>
#include <prsdef.h>
#include <dce/rpc.h>
#include <dce/sys_time.h>
#include <dce/dce64.h>

static int savebyteattr(struct prs *, int type);
int *relop_pointer(void);

static struct attrblk ab;		/* only one of these */
static struct singlevalueblk svb1;	/* only one of these */
static AtomicValue_t av;                /* Contruct atomic value here */

/* Indicates special processing to parse routines */
static int IsA;

#define UNKNOWN         0
#define CONVERGENCE     1
#define INCH            2
#define TIMEOUT         3
#define VERSION         4
#define RPSTATE         5
#define RPTYPE          6
#define CHSTATE         7

/*
 *  Difference between smithsonian zero and gregorian zero
 *
 *  DTSS UTC base time is October 15, 1582 (Gregorian).
 *  DNS base time is November 17, 1858 (Smithsonian).
 */
#if (LONG_BIT  ==  K_64_BITS)
static time_quad_t base = 0x0135886AC7960000;
#else
static time_quad_t base = {0xC7960000u, 0x0135886Au};
#endif

/* Return pointer to static attrblk */
struct attrblk *
attrname_pointer ()
{
  return (&ab);
}

/* Test IsA */
int
istimeout (
     struct prs	*pcbp)
{
  return((IsA == TIMEOUT) ? ACT_SUCCESS : ACT_ERROR);
}

int
isconvergence (
     struct prs	*pcbp)
{
  return((IsA == CONVERGENCE) ? ACT_SUCCESS : ACT_ERROR);
}

int
isrptype (
     struct prs	*pcbp)
{
  return((IsA == RPTYPE) ? ACT_SUCCESS : ACT_ERROR);
}

int
isrpstate (
     struct prs	*pcbp)
{
  return((IsA == RPSTATE) ? ACT_SUCCESS : ACT_ERROR);
}

int
isinch (
     struct prs	*pcbp)
{
  return((IsA == INCH) ? ACT_SUCCESS : ACT_ERROR);
}

int
ischstate (
     struct prs       *pcbp)
{
  return((IsA == CHSTATE) ? ACT_SUCCESS : ACT_ERROR);
}

int
isversion (
     struct prs	*pcbp)
{
  return((IsA == VERSION) ? ACT_SUCCESS : ACT_ERROR);
}

/*
 * Handle quoting and escapes for input string
 * spaces and commas are terminators
 */
unsigned char *
scan_string (
     unsigned char *next_p,          /* current string */
     unsigned char *new_p,           /* new string */
     unsigned char *outlen_p)         /* 16 bits of length */
{

  unsigned char *out_p = new_p;

  /* Look for single quotes */
  if (*next_p == '\'') {
    next_p++;
    /* Search for matching quote and accept anything in between */
    while (*next_p != '\'') {
      if (*next_p == '\0') return(NULL);
      COPY_mbc_and_skip((char *)next_p, (char *)out_p);
    }
    next_p++;

  /* try for double quotes */
  } else if (*next_p == '\"') {
    next_p++;
    while (*next_p != '\"') {
      if (*next_p == '\0') return(NULL);
      /* Check for valid escapes and remove them from string */
      if ((*next_p == '\\') &&
	  ((next_p[1] == '\"') || (next_p[1] == '\\')))
        next_p++;
      COPY_mbc_and_skip((char *)next_p, (char *)out_p);
    }
    next_p++;

  /* Handle escaping spaces characters */
  } else
    while ((*next_p != '\0') && (*next_p != ' ') && (*next_p != ',')) {
	/* check for escaping single characters */
	if ((next_p[0] == '\\') &&
            ((next_p[1] == ' ') || (next_p[1] == ',') ||
             (next_p[1] == '\\')))		/* bug fix 4/15/93 */
	  next_p++;
	COPY_mbc_and_skip((char *)next_p, (char *)out_p);
    }

  INS16(outlen_p, out_p - new_p);
  return(next_p);
}

/*
 * Build an attrblk structure first in static storage, then allocate new space
 * and copy it onto a list of other attrblk
 */
int
save_any_attr (				/* put an attribute block on list */
    struct attrblk *habp,		/* ptr to head of attr list */
    unsigned char *snp,			/* ptr to attr simple name */
    AtomicValue_t *avp,			/* ptr to attr value as atomic value */
    int code,				/* kind of attribute code */
    int relop)				/* relational operator, usually = */
{
    int status;

    /* Initialize static attrblk */
    attrblk_init(&ab);

    /* Initialize static singlevalue block */
    init_singlevalueblk(&svb1);

    /* Flag as static storage */
    svb1.blkalloc = 0;
    svb1.valalloc =0;

    /* Build attrblk */
    ab.relop = relop;
    COPY_SimpleName(snp, &ab.bigattr);
    ab.grpattr = code;
    svb1.vp = avp;
    ab.av = (avp ? &svb1 : 0);		/* point to optional singlevalueblk */

    /* copy the block that is here into the list */
    status = copyattr_before(&ab, habp);
    return(status);
    }

/* Build attrblock for name only, no value */
int
save_named_attr (
     struct prs	*pcbp)
{
  int status;                         /* 1 if the subrtn was able to malloc */

  /* copy the block that is here into the list */
  status = save_any_attr(get_attrblk_ptr(), aname_pointer(),
			 NULL, EXPLICIT_ATTR, *relop_pointer());
  return (status ? ACT_SUCCESS : ACT_ERROR);
}

/* Save a version attribute in the attribute list */
int
saveverattr (
    struct prs *pcbp)
{
    unsigned char *p = (unsigned char *)majorminor_pointer();
    int status;

    /* Build Value */
    INS16(av.av_length, LEN_version(p));
    COPY_version(p, av.av_value);
    INS8(av.av_valuetype, VT_Version);

    /* Copy value into an attrblk */
    status = save_any_attr(get_attrblk_ptr(), aname_pointer(), &av,
			   EXPLICIT_ATTR, *relop_pointer());
    return(status ? ACT_SUCCESS : ACT_ERROR);
}

/* Save the class version attribute in the attribute list */
int
savecvattr (
    struct prs *pcbp)
{
    unsigned char *p = (unsigned char *)majorminor_pointer();
    int status;

    /* Build Value */
    INS16(av.av_length, LEN_version(p));
    COPY_version(p, av.av_value);
    INS8(av.av_valuetype, VT_Version);

    /* Copy value into an attrblk */
    status = save_any_attr(get_attrblk_ptr(),
			   (unsigned char *)dns_AttributeName(DNS_CLASSVERSION),
			   &av, EXPLICIT_ATTR, *relop_pointer());
    return(status ? ACT_SUCCESS : ACT_ERROR);
}

/* Save the classname attribute value */
int				/* normally ACT_SUCCESS */
savecnattr (
    struct prs *pcbp)
{
    ClassName_t *p = (ClassName_t *)sname_pointer();
    int status;
    int cn_len;

    /* Build atomic value consisting of a simplename */
    cn_len = EXT8(p->cn_length);
    if (cn_len > 31) return (ACT_ERROR);
    INS16(av.av_length, LEN_ClassName(p));
    INS8(av.av_valuetype, VT_byte);
    COPY_ClassName(p, av.av_value);

    status = save_any_attr(get_attrblk_ptr(),
			   (unsigned char *)dns_AttributeName(DNS_CLASS),
			   &av, EXPLICIT_ATTR, *relop_pointer());
    return(status ? ACT_SUCCESS : ACT_ERROR);
}

/* Save linktarget as attblk - called by createlink */
int
savelnktarget (
     struct prs *pbp)			/* ptr to parser context block */
{
  FullName_t *p = (FullName_t *)name_pointer(); /* ptr to the target name */
  int status;

  INS16(av.av_length, LEN_FullName(p));
  COPY_FullName(p, av.av_value);
  INS8(av.av_valuetype, VT_FullName);
  status = save_any_attr(get_attrblk_ptr(),
			 (unsigned char *)dns_AttributeName(DNS_LINKTARGET),
			 &av, EXPLICIT_ATTR, *relop_pointer());
  return(status ? ACT_SUCCESS : ACT_ERROR);
}

/* Save linktimeout - called during create link */
int
savelnktimeout (
     struct prs *pbp)
{
  dns_timeout_t *to_p;
  int status;

  to_p = (dns_timeout_t *)av.av_value;
  memcpy(&to_p->expire, abstime_pointer(), sizeof(utc_t));
  memcpy(&to_p->extend, deltatime_pointer(), sizeof(utc_t));
  INS16(av.av_length, sizeof(dns_timeout_t));
  INS8(av.av_valuetype, VT_Timeout);
  status = save_any_attr(get_attrblk_ptr(),
			 (unsigned char *)dns_AttributeName(DNS_LINKTIMEOUT),
			 &av, EXPLICIT_ATTR, *relop_pointer());
  return(status ? ACT_SUCCESS : ACT_ERROR);
}

/* Save a timeout in attrblk */
int
savetoattr (
     struct prs *pbp)
{
  dns_timeout_t *to_p;
  int status;

  to_p = (dns_timeout_t *)av.av_value;
  memcpy(&to_p->expire, abstime_pointer(), sizeof(utc_t));
  memcpy(&to_p->extend, deltatime_pointer(), sizeof(utc_t));
  INS16(av.av_length, sizeof(dns_timeout_t));
  INS8(av.av_valuetype, VT_Timeout);
  status = save_any_attr(get_attrblk_ptr(), aname_pointer(), &av,
			 EXPLICIT_ATTR, *relop_pointer());
  return(status ? ACT_SUCCESS : ACT_ERROR);
}

/* Save a small attribute value.  The value is passed in av1 */
int
save1byteattr (
     struct prs *pcbp)
{
  int status;

  INS8(av.av_value, pcbp->av1);
  INS16(av.av_length, sizeof(char));
  INS8(av.av_valuetype, VT_small);
  status = save_any_attr(get_attrblk_ptr(), aname_pointer(), &av,
			 EXPLICIT_ATTR, *relop_pointer());
  return(status ? ACT_SUCCESS : ACT_ERROR);
}

/*
 * Determine the attribute type and parse the input accordingly
 */
int
savecdsattr (
     struct prs *pcbp)
{
  dns_opq_sname *const att_p = (dns_opq_sname *)aname_pointer();
  attribute_descriptor_t *dns_p;
  int type = aname_type();

  /* See if it is a DNS attribute */
  get_known_attribute(att_p, &dns_p);

  /* Some CDS attributes and valuetypes are better handled by the parse
   * tables.  Set a flag to let them know
   */
  IsA = UNKNOWN;
  if (type == VT_Version) {
      IsA = VERSION;
      return(ACT_ERROR);
  } else if (type == VT_Timeout) {
      IsA = TIMEOUT;
      return(ACT_ERROR);
  }

  if (dns_p ) {
    if (dns_p->atype == DNS_CONVERGENCE) {
      IsA = CONVERGENCE;
      return(ACT_ERROR);
    } else if (dns_p->atype == DNS_INCHNAME) {
      IsA = INCH;
      return(ACT_ERROR);
    } else if (dns_p->atype == DNS_CLASS) {
      return(savesimpleattr(pcbp));
    } else if (dns_p->atype == DNS_REPLICASTATE) {
      IsA = RPSTATE;
      return(ACT_ERROR);
    } else if (dns_p->atype == DNS_REPLICATYPE) {
      IsA = RPTYPE;
      return(ACT_ERROR);
    } else if (dns_p->atype == DNS_CHSTATE) {
      IsA = CHSTATE;
      return(ACT_ERROR);
    }
  }

  switch (type) {
    case VT_ASN1:
    case VT_byte:
        if ((dns_p) && (dns_p->atype == DNA_TOWERS))
	  return(savetowerattr(pcbp));
	else
	  return(savebyteattr(pcbp, type));
    case VT_char:
	return(savecharattr(pcbp));
    case VT_small:
	return(savesmallattr(pcbp));
    case VT_short:
	return(saveshortattr(pcbp));
    case VT_long:
	return(savelongattr(pcbp));
    case VT_Timestamp:
	return(savetsattr(pcbp));
    case VT_uuid:
	return(saveuuidattr(pcbp));
    case VT_FullName:
	return(savenameattr(pcbp));
    case VT_none:
	return(savenoneattr(pcbp));
    default:
	return(savebyteattr(pcbp, (int)dns_byte));
	break;
    }
  return(ACT_ERROR);
}

int
savesmallattr (
     struct prs *pcbp)
{
  char *curp;
  long scanvalue = strtol(pcbp->cur, &curp, 10);
  char value = scanvalue;

  if ((pcbp->cur < curp)  && (value == scanvalue)) {
    int status;

    COPY_bytes(&value, av.av_value, sizeof(value));
    INS16(av.av_length, sizeof(value));
    INS8(av.av_valuetype, VT_small);
    status = save_any_attr(get_attrblk_ptr(), aname_pointer(), &av,
			   EXPLICIT_ATTR, *relop_pointer());
    /* Update pointer for parser */
    pcbp->nxt = curp;
    if (status)
      return(ACT_SUCCESS);
  }
  return(ACT_ERROR);
}

int
saveshortattr (
     struct prs *pcbp)
{
  char *curp;
  long scanvalue = strtol(pcbp->cur, &curp, 10);
  short int value = scanvalue;

  if ((pcbp->cur < curp) && (value == scanvalue)) {
    int status;

    COPY_bytes(&value, av.av_value, sizeof(short));
    INS16(av.av_length, sizeof(short));
    INS8(av.av_valuetype, VT_short);
    status = save_any_attr(get_attrblk_ptr(), aname_pointer(), &av,
			   EXPLICIT_ATTR, *relop_pointer());
    /* Update pointer for parser */
    pcbp->nxt = curp;
    if (status)
      return(ACT_SUCCESS);
  }
  return(ACT_ERROR);
}

int
savelongattr (
     struct prs *pcbp)
{
  char *curp;
  long value = strtol(pcbp->cur, &curp, 10);

  if (pcbp->cur < curp) {
    int status;

    COPY_bytes(&value, av.av_value, sizeof(long));
    INS16(av.av_length, sizeof(long));
    INS8(av.av_valuetype, VT_long);
    status = save_any_attr(get_attrblk_ptr(), aname_pointer(), &av,
			   EXPLICIT_ATTR, *relop_pointer());
    /* Update pointer for parser */
    pcbp->nxt = curp;
    if (status)
      return(ACT_SUCCESS);
  }
  return(ACT_ERROR);
}

int
savetsattr (
     struct prs *pcbp)
{
  unsigned64 newexpire_utc, inaccur_utc;
  time_quad_t newexpire;
  long tdf;
  char *nxt_p;
  int i;
  Node_u id;
  int status;

  /* parse time as utc, this routine updates the current pointer */
  if (parse_abstime(pcbp) != ACT_SUCCESS)
    return(ACT_ERROR);

  /* Convert base from gregorian to smithsonian */

  /* Break utc into componenets.  DTSS routine */
  utc_comptime(&newexpire_utc, &inaccur_utc, &tdf, abstime_pointer());
#if (LONG_BIT == 64)
	newexpire = newexpire_utc;
#else
	U64GET64(newexpire.tq_hi, newexpire.tq_lo, newexpire_utc);
#endif


  /* Convert gregorian values to  smithsonian  */
  if (!ZERO_time_quad(&newexpire))
    SUB_time_quad(&newexpire, &base, &newexpire);

  COPY_Time(&newexpire, ((Timestamp_t *)av.av_value)->ts_time);

  /* Now parse node id */
  nxt_p = pcbp->nxt;
  if (*nxt_p++ != '/') return(ACT_ERROR);

  for (i=0; i < sizeof(Node_u); ++i) {
    char nibbles[8], *nibble_p = nibbles, *end_p;
    long value;

    /* Move value to high and low niblet niblets */
    COPY_mbc_and_skip(nxt_p, nibble_p);
    COPY_mbc_and_skip(nxt_p, nibble_p);
    *nibble_p = '\0';
    value = strtol(nibbles, &end_p, 16);
    if ((end_p != nibble_p) || (value < 0) || (255 < value))
      return(ACT_ERROR);

    id[i] = value;
    if (*nxt_p == '-')
      nxt_p++;
  }

  COPY_Node(id, ((Timestamp_t *)av.av_value)->ts_node);
  INS16(av.av_length, sizeof(Timestamp_u));
  INS8(av.av_valuetype, VT_Timestamp);
  status = save_any_attr(get_attrblk_ptr(), aname_pointer(), &av,
			 EXPLICIT_ATTR, *relop_pointer());
  /* Update pointer for parser */
  pcbp->nxt = pcbp->cur+(nxt_p - pcbp->cur);

  return(status ? ACT_SUCCESS : ACT_ERROR);
}

int
savetowerattr (
     struct prs *pcbp)
{
  Set_t *set_p;
  MemberValue_t *member_p;

  char *binding[2];
  char *end_p = pcbp->nxt;
  char bindingstring[100];
  int len = 0;
  int status;

  /*
   * note: this routine expects to find pcbp->cur pointing to the
   * text string "tower" (or an appropriate abbreviation) and pcbp->nxt
   * pointing to the actual 'tower' string
   */

  /* Find end and length of 'tower' string */
  while ((*end_p != ' ') && (*end_p != '\0')) {
      len += LEN_mbc(end_p);
      end_p += LEN_mbc(end_p);
  }
  if (len == 0)
      return(ACT_ERROR);

  /* Save so string-to-tower routine can manipulate it */

  COPY_bytes(pcbp->nxt, bindingstring, len);
  bindingstring[len] = '\0';			/* Null terminate */
  binding[0] = bindingstring;			/* Array of pointers */
  binding[1] = NULL;

  if (dnsCvtStrToTower(binding, (char **)&set_p) == DNS_SUCCESS) {
      /* Have a set, find first member and convert to atomic value */
      member_p = (MemberValue_t *)DATA_Set(set_p);
      
      INS16(av.av_length, EXT16(member_p->mv_length));
      INS8(av.av_valuetype, VT_byte);
      COPY_bytes( (char *)member_p->mv_value, av.av_value, 
		 EXT16(member_p->mv_length));
      status = save_any_attr(get_attrblk_ptr(), aname_pointer(), &av,
			     EXPLICIT_ATTR, *relop_pointer());
      /* Update pointer for parser */
      pcbp->nxt += len;
      free(set_p);
  /* if we can't build a tower, we might have hex input defining the tower */

  /* since we only support RPC towers, hex tower input doesn't make sense
     anymore...

  } else if (( savebyteattr ( pcbp, VT_byte ) == ACT_SUCCESS ) &&
  ( validate_tower ( av.av_value, EXT16(av.av_length)))) {
	     status = save_any_attr ( get_attrblk_ptr(), aname_pointer(), &av,
	     EXPLICIT_ATTR, *relop_pointer());
	     */
  } else status = 0;

  return(status ? ACT_SUCCESS : ACT_ERROR);
}

int
savenameattr (
     struct prs *pcbp)
{
  int status;

  /* Parse name and convert to internal */
  status = nowildfullname(pcbp);

  if (status == ACT_SUCCESS) {
    COPY_FullName(name_pointer(), av.av_value);
    INS16(av.av_length, LEN_FullName(av.av_value));
    INS8(av.av_valuetype, VT_FullName);
    status = save_any_attr(get_attrblk_ptr(), aname_pointer(), &av,
			 EXPLICIT_ATTR, *relop_pointer());
  } else {
    /*
     * Advance the pcbp->maxcur so that it points to the right
     * place where the error is detected.
     */  
    pcbp->maxcur = pcbp->nxt;
    status = 0;
  }

  return(status ? ACT_SUCCESS : ACT_ERROR);
}

int
savesimpleattr (
     struct prs *pcbp)
{
  int status;

  /* Parse name and convert to internal */
  status = simplename(pcbp);

  if (status == ACT_SUCCESS) {
    COPY_SimpleName(sname_pointer(), av.av_value);
    INS16(av.av_length, LEN_SimpleName(av.av_value));
    INS8(av.av_valuetype, VT_byte);
    status = save_any_attr(get_attrblk_ptr(), aname_pointer(), &av,
			 EXPLICIT_ATTR, *relop_pointer());
  } else status = 0;
  return(status ? ACT_SUCCESS : ACT_ERROR);
}

int
saveuuidattr (
     struct prs *pcbp)
{
  unsigned char *nxt_p;
  unsigned32 status;
  unsigned_char_t uuidstring[100];
  int len;
  uuid_t uuid;

  /* find terminator */
  nxt_p =  (unsigned char *)strpbrk(pcbp->cur, " ,");
  if (nxt_p == NULL) {
    uuid_from_string((unsigned_char_t *)pcbp->cur, &uuid, &status);
    len = strlen(pcbp->cur);
  }  else {
    len = nxt_p - (unsigned char *)pcbp->cur;
    COPY_bytes(pcbp->cur, uuidstring, len);
    uuidstring[len] = '\0';
    uuid_from_string(uuidstring, &uuid, &status);
  }

  /* If it worked, update parser */
  if (status == uuid_s_ok) {

    COPY_bytes(&uuid, av.av_value, sizeof(uuid));
    INS16(av.av_length, sizeof(uuid));
    INS8(av.av_valuetype, VT_uuid);
    status = save_any_attr(get_attrblk_ptr(), aname_pointer(), &av,
			 EXPLICIT_ATTR, *relop_pointer());
    /* Update pointer for parser */
    pcbp->nxt = pcbp->cur+len;
  } else status = 0;
  return(status ? ACT_SUCCESS : ACT_ERROR);
}

int
savenoneattr (
     struct prs *pcbp)
{
  int status;

  INS8(av.av_valuetype, VT_none);
  status = save_any_attr(get_attrblk_ptr(), aname_pointer(), &av,
			 EXPLICIT_ATTR, *relop_pointer());
  return(status ? ACT_SUCCESS : ACT_ERROR);
}

int
savecharattr (
     struct prs *pcbp)
{
  unsigned char *nxt_p;
  int status;

  /* scan input string */
  nxt_p = scan_string((unsigned char *)pcbp->cur, av.av_value, av.av_length);

  if (!nxt_p || (EXT16(av.av_length) == 0) || (av.av_value == '\0'))
    return(ACT_ERROR);

  /* Update pointer for parser */
  pcbp->nxt = pcbp->cur + ((char *)nxt_p - pcbp->cur);

  INS8(av.av_valuetype, VT_char);
  status = save_any_attr(get_attrblk_ptr(), aname_pointer(), &av,
			 EXPLICIT_ATTR, *relop_pointer());
  return(status ? ACT_SUCCESS : ACT_ERROR);
}

static int
savebyteattr (
     struct prs *pcbp,
     int type)
{
  unsigned char *in_p = (unsigned char *)pcbp->nxt;
  unsigned char *out_p = av.av_value;
  int len = 0;
  int status;
  unsigned char *strend_p;

  /* As long as the input text appears to be hex data, convert it to
   * hex and store in av.av_value. The string must be totally hex, i.e.
   * the terminating character must be blank or null.
  */

  strend_p = in_p + strlen((char *)in_p);

  while (TRUE) {
    char *maybe_p = (char *)in_p;
    char nibbles[8], *nibble_p = nibbles, *end_p;
    long value;

    if ((in_p + 2*sizeof(char)) > strend_p) break;
    if (!isxdigit(*in_p) || (!isxdigit(*(in_p + sizeof(char))))) break;

    /* Move value to high and low niblet niblets */
    COPY_mbc_and_skip(maybe_p, nibble_p);
    COPY_mbc_and_skip(maybe_p, nibble_p);
    *nibble_p = '\0';
    value = strtol(nibbles, &end_p, 16);
    if ((end_p == nibbles) && (value == 0))
      break;
    if ((end_p != nibble_p) || (value < 0) || (255 < value))
      return(ACT_ERROR);

    *out_p++ = value;
    len++;
    in_p = (unsigned char *)maybe_p;
  }

  pcbp->nxt = (char *)in_p;		/* Update pointer for parser */
  INS16(av.av_length, len);		/* Needed by savetowerattr !!! */
  INS8(av.av_valuetype, type);

  if (( len == 0) ||
      ((in_p[0] != ' ') && (in_p[0] != '\0'))) return ( ACT_ERROR );

  status = save_any_attr(get_attrblk_ptr(), aname_pointer(), &av,
			 EXPLICIT_ATTR, *relop_pointer());
  return(status ? ACT_SUCCESS : ACT_ERROR);
}
