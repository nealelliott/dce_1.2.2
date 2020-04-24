/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: emit_cds_values.c,v $
 * Revision 1.1.8.2  1996/02/18  19:28:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:32  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:08:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:09  root]
 * 
 * Revision 1.1.6.5  1994/09/15  15:22:38  mccann
 * 	change CDS_CellAlias field strings
 * 	[1994/09/15  15:13:02  mccann]
 * 
 * Revision 1.1.6.4  1994/08/03  19:00:36  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:41:59  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:36:39  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:21  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/19  17:05:31  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:29:57  mccann]
 * 
 * Revision 1.1.6.1  1994/03/12  21:58:21  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:58:43  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:03:13  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:33:28  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/14  14:42:29  m_sawyer
 * 	Fix problem with printing of "small" negative numbers
 * 	[1992/05/14  14:41:39  m_sawyer]
 * 
 * Revision 1.1  1992/01/19  15:21:00  devrcs
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
 * MODIFICATION HISTORY:
 *
 */

#include <cdscp.h>
#include <cmdblk.h>
#include <dce/rpc.h>

static void F_small(struct op_blk *,char *,cds_attrvalue_t *);
static void F_short(struct op_blk *,char *,cds_attrvalue_t *);
static void F_long(struct op_blk *,char *,cds_attrvalue_t *);
static void F_uuid(struct op_blk *,char *,cds_attrvalue_t *);
static void F_timestamp(struct op_blk *,char *, cds_attrvalue_t *);
static void F_timeout(struct op_blk *,char *,cds_attrvalue_t *);
static void F_version(struct op_blk *,char *,cds_attrvalue_t *);
static void F_char(struct op_blk *,char *,cds_attrvalue_t *);
static void F_member(struct op_blk *,char *,cds_attrvalue_t *);
static void F_rp(struct op_blk *,char *,cds_attrvalue_t *);
static void F_pp(struct op_blk *,char *,cds_attrvalue_t *);
static void F_fullname(struct op_blk *,char *,cds_attrvalue_t *);
static void F_byte(struct op_blk *,char *,cds_attrvalue_t *);
static void F_chdirectory(struct op_blk *,char *,cds_attrvalue_t *);
static void F_gp(struct op_blk *,char *,cds_attrvalue_t *);
static void emit_tower_lines(struct op_blk *,int,unsigned char *,int);
static void emit_replicapointer_lines(struct op_blk *,ReplicaPointer_t *);
static void emit_timeout_lines(struct op_blk	*outbp, cds_timeout_t *);

static struct {
  int syntax;
  void (*routine)(struct op_blk *,char *,cds_attrvalue_t *);
  int manylines;
} value_display[] = {cds_none, 0, 0,
		     cds_small, F_small, 0,
		     cds_short, F_short, 0,
                     cds_long,  F_long, 0,
                     cds_uuid,  F_uuid, 0,
                     cds_Timestamp, F_timestamp, 0,
		     cds_Timeout, F_timeout, 1,
		     cds_Version, F_version, 0,
		     cds_char, F_char, 0,
		     cds_byte, F_byte, 0,
		     cds_ASN1, F_byte, 0,
		     cds_GroupMember, F_member, 1,
		     cds_ReplicaPointer, F_rp, 1,
		     cds_ParentPointer, F_pp, 1,
		     cds_gdaPointer, F_gp, 1,
		     cds_FullName, F_fullname, 0,
		     cds_CHDirectory, F_chdirectory, 1,
		     -1, 0, 0};

static void
F_small (
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
  int val = *((signed char *)&value_p->val_u.val_small);

  sprintf((char *)buf_p, "%1hd", val);
}

static void
F_short (
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
 int val = value_p->val_u.val_short;

  sprintf((char *)buf_p, "%1hd", val);
}

static void
F_long (
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
  int val = value_p->val_u.val_long;

  sprintf((char *)buf_p, "%1ld", val);
}

static void
F_uuid (
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
  fmt_uid_for_print((unsigned char *)&value_p->val_u.val_uuid, buf_p);
}

static void
F_version (
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
  format_classversion_for_print(buf_p,
				(ClassVersion_t *)&value_p->val_u.val_version);
}

static void
F_timestamp (
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
  format_ts((Timestamp_t *)&value_p->val_u.val_timestamp,
	    (unsigned char *)buf_p);
}

static void
F_fullname (
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
  format_fullname_for_print(value_p->val_u.val_byte.byte_p, buf_p);
}

static void
F_timeout (
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
  emit_timeout_lines(outbp, &value_p->val_u.val_timeout);
}

static void
F_member (
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
  emit_member_lines(outbp, value_p->val_u.val_byte.byte_p);
}

static void
F_rp (
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
  emit_replicapointer_lines(outbp, 
			    (ReplicaPointer_t *)value_p->val_u.val_byte.byte_p);
}

static void
F_pp(
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
  ParentPointer_t	*ppp;
  char valuebuf[100];

  ppp = (ParentPointer_t *)value_p->val_u.val_byte.byte_p;
  
  /* Print uuid */
  fmt_uid_for_print(ppp->pp_parentID, valuebuf);
  emit_attr_string(outbp, CDSCP_PARENTCTS_LINE, "Parent's UUID", valuebuf);

  emit_attr_string(outbp, CDSCP_PPTIMEOUT_LINE, "Timeout", ":");
  emit_timeout_lines(outbp, (cds_timeout_t *)&ppp->pp_timeout);

  emit_myname_line(outbp, ppp->pp_myName);
}

static void
F_gp (
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
  register gdaPointer_t *gp_p = (gdaPointer_t *)value_p->val_u.val_byte.byte_p;
  register ReplicaPointer_t *rp_p = (ReplicaPointer_t *)gp_p->gp_replica;

  emit_attr_string(outbp, CDSCP_PPTIMEOUT_LINE, "Timeout", ":");
  emit_timeout_lines(outbp, (cds_timeout_t *)&gp_p->gp_timeout);

  emit_replicapointer_lines(outbp, rp_p);
}

static void
F_chdirectory (
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
  CHDirectoryPointer_t  *chdirp = 
      (CHDirectoryPointer_t *)value_p->val_u.val_byte.byte_p;
  char valuebuf[CDS_STR_FNAME_MAX];
  
  /* Print directory UUID */ 
  fmt_uid_for_print(chdirp->cp_dirID,valuebuf);
  emit_attr_string(outbp, CDSCP_CHDIRCTS_LINE, "UUID of Directory", valuebuf);

  /* Print directory name */
  format_fullname_for_print((FullName_t *)chdirp->cp_directory, valuebuf);
  emit_attr_string(outbp, CDSCP_CHDIRNAME_LINE, "Name of Directory", valuebuf);

}

static void
F_char (
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
  memcpy(buf_p, value_p->val_u.val_char.char_p,
	 value_p->val_u.val_char.length);
  buf_p[value_p->val_u.val_char.length] = 0;
}

static void
F_byte (
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
  unsigned char *in_p = value_p->val_u.val_byte.byte_p;
  char *out_p = buf_p;
  int i = value_p->val_u.val_byte.length;

  if (0 < i)
    /* Convert each byte into hex pairs */
    do {
      sprintf(out_p, "%02.2x", *in_p++);
      out_p += 2;
    } while (--i);
}

/*
 * Convert attribute value to a displayable format.
 */
void
emit_cds_values (
     struct op_blk	*outbp,
     char		*name_p,
     cds_attrvalue_t    *value_p)
{
  char outbuf[10000];
  int i;
 
  /* Find valuetype in table */
  for (i=0; value_display[i].syntax != -1; ++i) {
    if (value_display[i].syntax == value_p->val_syntax) {
      if (value_display[i].routine) {
	/* This value, requires multiple lines, print header before dispatch */
	if (value_display[i].manylines) {
	  emit_name_string(outbp, name_p, ":");
	  (*value_display[i].routine)(outbp, outbuf, value_p);
	} else {
	  /* Only one line of output, format value then display */
	  (*value_display[i].routine)(outbp, outbuf, value_p);
	  emit_name_string(outbp, name_p, outbuf);
	}
      } else
	  emit_name_string(outbp, name_p, "");

      break;
    }/* End match syntax */
  }
}

void
emit_cds_sname (
     struct op_blk	*outbp,
     char		*name_p,
     cds_attrvalue_t    *value_p)
{
  char namebuf[CDS_STR_SNAME_MAX];
  cds_simple_name_t *const cds_name_p =
	(cds_simple_name_t *)value_p->val_u.val_byte.byte_p;
  int len = sizeof(namebuf);

  cdsCvtSimpleToStr(cds_name_p, (unsigned char *)namebuf, &len);
  emit_name_string(outbp, name_p, namebuf);
}

void
emit_cds_tower (
     struct op_blk	*outbp,
     char		*name_p,
     cds_attrvalue_t    *value_p)
{

  emit_name_string(outbp, name_p, ":");
  emit_tower_lines(outbp, 1, value_p->val_u.val_byte.byte_p,
		   value_p->val_u.val_byte.length);
}

/* Print cds_chstate - requires special processing for values */
void
emit_cds_chstate (
     struct op_blk	*outbp,
     char		*name_p,
     cds_attrvalue_t    *value_p)
{
    char outbuf[100];		/* build explanation of code here */

    format_chstate_for_print(outbuf, (int)value_p->val_u.val_small);
    emit_name_string(outbp, name_p, outbuf);
}

/* Print cds_convergence - requires special processing for values */
void
emit_cds_convergence (
     struct op_blk	*outbp,
     char		*name_p,
     cds_attrvalue_t    *value_p)
{
    char outbuf[100];		/* build explanation of code here */

    format_convergence_for_print(outbuf, (int)value_p->val_u.val_small);
    emit_name_string(outbp, name_p, outbuf);
}

/* Print cds_inchname - requires special processing for values */
void
emit_cds_inch (
     struct op_blk	*outbp,
     char		*name_p,
     cds_attrvalue_t    *value_p)
{
    char outbuf[100];		/* build explanation of code here */

    format_inch_for_print(outbuf, (int)value_p->val_u.val_small);
    emit_name_string(outbp, name_p, outbuf);
}

/* Print cds_replicatype - requires special processing for values */
void
emit_cds_reptype (
     struct op_blk	*outbp,
     char		*name_p,
     cds_attrvalue_t    *value_p)
{
    char outbuf[100];		/* build explanation of code here */

    format_reptyp_for_print(outbuf, (int)value_p->val_u.val_small);
    emit_name_string(outbp, name_p, outbuf);
}

/* Print cds_replicastate - requires special processing for values */
void
emit_cds_repstate (
     struct op_blk	*outbp,
     char		*name_p,
     cds_attrvalue_t    *value_p)
{
    char outbuf[100];		/* build explanation of code here */
    static const unsigned32 codes[] = {
	CDSCP_REPSTATE001, CDSCP_REPSTATE002, CDSCP_REPSTATE003,
	CDSCP_REPSTATE004, CDSCP_REPSTATE005, CDSCP_REPSTATE006};
    const int repstate = value_p->val_u.val_small;
    const unsigned32 code = ((1 <= repstate) && (repstate <= 6))
	? codes[repstate-1] : CDSCP_REPSTATE_BAD;

    cds_sprintf(outbuf, code,
		"<replica state value msg not found> value = %d", repstate);
    emit_name_string(outbp, name_p, outbuf);
}
/*
 * Display replica pointer
 */
static void
    emit_replicapointer_lines (
			       struct op_blk	*outbp,
			       ReplicaPointer_t *rp_p)	
{
  /* point to each field of the replica */
  unsigned char *const chidp = (unsigned char *)rp_p->rp_CHID;
  Set_t *const ahp = (Set_t *)rp_p->rp_addressHint;
  unsigned char *const rtp = (unsigned char *)FLD_ReplicaPointer_replicaType(rp_p);
  FullName_t *const np  = FLD_ReplicaPointer_CHName(rp_p);
  char valuebuf[CDS_STR_FNAME_MAX];	/* printable clearinghouse name here */
  
  /* Display clearinghouse uuid */
  fmt_uid_for_print(rp_p->rp_CHID, valuebuf);
  emit_attr_string(outbp,
		   CDSCP_REPCHCTS_LINE, "Clearinghouse UUID", valuebuf);
  
  /* Display tower */
  emit_towerset_lines(outbp, ahp);
  
  /* Display replica type */
  format_reptyp_for_print(valuebuf, *rtp);
  emit_attr_string(outbp,
		   CDSCP_REPTYPE_LINE, "Replica type", valuebuf);
  
  /* Display clearinghouse name */
  format_fullname_for_print((FullName_t *)np, valuebuf);
  emit_attr_string(outbp,
		   CDSCP_REPCHNAME_LINE, "Clearinghouse Name", valuebuf);
  
  return;				
}					
/*
 * Display a towerset
 */
void
    emit_towerset_lines (
			 struct op_blk	*outbp,
			 Set_t		*tsp)
{
    int members = NUM_Set(tsp);	      
    int i;

    /* Process each set member */
    for (i=0; i<members; i++)		
	{
	MemberValue_t *const smp = (MemberValue_t *)INDEX_Set(tsp, i);
	byte_t *const tp = smp->mv_value;
	const int len = EXT16(smp->mv_length);

	emit_tower_lines(outbp, i+1, tp, len);
	}
}
/* 
 * Display a tower, usually one member of a set 
 */
static void
    emit_tower_lines (
		      struct op_blk	*outbp,
		      int		n,		
		      unsigned char	*tp,		
		      int		size)		
{
  char **s_array;			
  int		  k;			
  int status1, status2;		

  /* 
   * warning: format_tower mallocs space for the strings it returns.
   * the caller must free the space when finished with it. 
   * Data is returned in s_array, which is a null terminated array 
   * of pointers to strings, one string per tower floor. The 
   * pointer array and the strings are contiguous in memory. 
   */

  status1 = validate_tower(tp, size);
  if ( status1 )			
      status2 = format_tower((char **)&s_array, (MemberValue_t *)tp);

      if ( status1 && ( status2 == CDS_SUCCESS )) {
      /* 
     * See if one or multiple floors.  
     * This only happens if we don't recognize the protocol sequence 
     */
    if ( s_array[1] != 0 ) {
      for (k=0; s_array[k] != 0; k++) {
	char tower_name[32];

	cds_sprintf(tower_name,
		    CDSCP_TOWERFLOOR, "Tower %2d Floor %d", n, k + 1);
	emit_name_string(outbp, tower_name, (char *)s_array[k]);
      }
    } else {
      emit_attr_string(outbp,
		       CDSCP_ONETOWERFLOOR, "Tower", (char *)s_array[0]);
    }
    free ((char *)s_array);
  } else {
    char buffer[5555];
    format(tp, size, buffer);
    emit_attr_string(outbp,
		     CDSCP_TOWERBAD, "Invalid Tower", buffer);
  }
  return;
}

static void
    emit_timeout_lines (
			struct op_blk	*outbp,
			cds_timeout_t 	*timeoutp)
{
    char valuebuf[1024];		/* construct formatted time here */
    byte *p;

    /* Process expiration first */
    /* zero time is a special case. */
    /* violate abstraction of utc, because approved way is even worse */
    p = (byte *)&timeoutp->expire;
    if (p[0] || p[1] || p[2] || p[3] || p[4] || p[5] || p[6] || p[7])
	format_utcabs_for_print(valuebuf, &timeoutp->expire);
    else
	strcpy(valuebuf, "0");

    emit_attr_string(outbp, CDSCP_EXP_LINE, "Expiration", valuebuf);


    /* Process extension */
    p = (byte *)&timeoutp->extend;
    if (p[0] || p[1] || p[2] || p[3] || p[4] || p[5] || p[6] || p[7])
        {
	size_t len = sizeof(valuebuf);	/* changed to amount used */
	/* Convert 128 bit utc to rel string */
	int status =
	    utc_ascreltime(valuebuf, len, &timeoutp->extend);
	if (status != 0)
	    /*
	     * This should probably not be an ifdef, the SNI_SVR4
	     * code is generic ANSI.
	     */
	    cds_strcpy(valuebuf, CDSCP_NORELTIME, "<?\?\?-??:??:??.?\?\?>");
      }
    else
	strcpy(valuebuf, "0");

    emit_attr_string(outbp, CDSCP_EXT_LINE, "Extension", valuebuf);
}
void
emit_cds_cellaliases (
     struct op_blk	*outbp,
     char		*buf_p,
     cds_attrvalue_t    *value_p)
{
    int code;				/* the group or principal code */
    GroupMember_t 	*memp = (GroupMember_t *)value_p->val_u.val_byte.byte_p;
    char valuebuf[DNS_STR_FNAME_MAX+1];		/* printable name built here */

    emit_name_string(outbp, buf_p, ":");
    format_fullname_for_print((FullName_t *)memp->gm_member, valuebuf);
    code = EXT8(memp->gm_isaGroup);		/* pick up the boolean code */

    if (code)			
	{			
	emit_attr_string(outbp, CDSCP_PRIMARY_LINE, "Primary", valuebuf);
	}
    else
	{			
	  emit_attr_string(outbp,
		CDSCP_ALIAS_LINE, "Alias", valuebuf);
	}
}




