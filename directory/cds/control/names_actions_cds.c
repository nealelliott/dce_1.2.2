/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: names_actions_cds.c,v $
 * Revision 1.1.12.2  1996/02/18  19:29:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:31  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:10:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:53  root]
 * 
 * Revision 1.1.10.4  1994/08/03  19:01:12  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:46:19  mccann]
 * 
 * Revision 1.1.10.3  1994/06/23  19:09:16  jd
 * 	Removed extern evtblk. It no longer exists
 * 	[1994/06/23  18:58:23  jd]
 * 
 * Revision 1.1.10.2  1994/06/09  18:37:31  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:28  devsrc]
 * 
 * Revision 1.1.10.1  1994/03/12  22:00:27  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:05:13  peckham]
 * 
 * Revision 1.1.8.1  1993/10/25  14:22:52  peckham
 * 	CR#9224: cdscp show cell core dump.
 * 	Change very old code to get the cellname via "/.:", not "/".
 * 	[1993/10/25  13:58:44  peckham]
 * 
 * Revision 1.1.6.3  1993/08/27  21:05:59  jd
 * 	(8202 ) Check the length of the last simple name against
 * 	DNS_STR_SNAME_MAX before the memcopy. Also make the size of
 * 	the local wildsimplename array 1 larger than DNS_STR_SNAME_MAX
 * 	to allow for the terminator.
 * 	[1993/08/27  19:26:08  jd]
 * 
 * Revision 1.1.6.2  1993/08/27  19:15:04  jd
 * 	(6501)Removed output of DceText error message from attributename. The
 * 	parser just says syntax error and the DceText outout was causing a
 * 	second error message to be output.
 * 	[1993/08/27  14:37:49  jd]
 * 
 * Revision 1.1.4.2  1992/12/30  13:08:13  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:39:49  zeliff]
 * 
 * Revision 1.1.2.3  1992/04/30  20:40:11  m_sawyer
 * 	fix 'with' processing
 * 	[1992/04/30  20:29:42  m_sawyer]
 * 
 * Revision 1.1.2.2  1992/03/22  21:52:24  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:22:51  weisman]
 * 
 * Revision 1.1  1992/01/19  15:22:41  devrcs
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

/* action routines for parsing cds names */

#include <cdscp.h>
#include <prsdef.h>

static unsigned char *np;		/* the full name, for other routines */
static unsigned char *tp;		/* move parts here and append \0 */
static unsigned char *ap;               /* Internal attribute name */
static unsigned char type;              /* Attributename value type */
static unsigned char *sp;               /* Internal simplename */
static unsigned char *wsp;              /* Internal wild simplename */

/* provide access to the name area via routine calls */

static int
    stash_name(
	       struct prs *cp,
	       int (*recognize_routine)(char *));

static int
    recognize_nsnwildfullname(
			      char *p);

unsigned char *
name_pointer (void)
{
  return(np);					/* point to the name area */
}

unsigned char *
aname_pointer (void)
{
  return(ap);					/* point to the name area */
}

int
aname_type (void)
{
  return((int)type);				/* attribute value type */
}

unsigned char *
sname_pointer (void)
{
  return(sp);					/* point to the name area */
}

unsigned char *
wild_sname_pointer (void)
{
  return(wsp);					/* point to the name area */
}

int
names_onetime_init (void)
{
  /* Allocate space for names */
  np =	(unsigned char *)malloc(DNS_OPQ_FNAME_MAX);
  tp =	(unsigned char *)malloc(DNS_STR_FNAME_MAX);
  ap =  (unsigned char *)malloc(DNS_OPQ_ANAME_MAX);
  sp =  (unsigned char *)malloc(DNS_OPQ_SNAME_MAX);
  wsp = (unsigned char *)malloc(DNS_OPQ_SNAME_MAX);
  return((np && tp && ap && sp && wsp) ? 1 : 0);
}

/* 
 * Find the end of the fullname. Names can be enclosed in single or
 * double quotes.  Single quotes should be used if the name
 * includes spaces.  Double qoutes and \ can be used to include
 * spaces, ', " or the wildcard characters.
 */
int 
scan_name (
     struct prs *cp,		/* ptr to parser context block */
     unsigned char *out_p)      /* Put new name, minus any quoting */	
{
  unsigned char *next_p;
  

  next_p = (unsigned char *)cp->cur;
  while ((*next_p != '\0') && (*next_p != ' ')) {
      if (*next_p == '\'') {
	  ++next_p;
	  /* Search for matching quote and accept anything in between */
	  while (*next_p != '\'') {
	      if (*next_p == '\0') return(0);
	      COPY_mbc_and_skip((char *)next_p, (char *)out_p);
	  } 
	  ++next_p;
	  continue;
      } else if (*next_p == '"') {
	  ++next_p;
	  while (*next_p != '"') {
	      if (*next_p == '\0') return(0);
	      /* Check for valid escapes and remove them from name */
	      if ((*next_p == '\\') &&
		  (*(next_p + 1) == '"'))
		++next_p;
	      COPY_mbc_and_skip((char *)next_p, (char *)out_p);
	  }	   
	  ++next_p;
	  continue;
      } if ((*next_p == '\\') && 
	    ((*(next_p + 1) == '\\') ||
	     (*(next_p + 1) == ' ')))
	/* check for escaping single characters */	
	++next_p;
      COPY_mbc_and_skip((char *)next_p, (char *)out_p);
  } /* End while */

  /* Null terminate new string */
  *out_p = '\0';

  /* Return size of string parsed */
  return(next_p - (unsigned char *)cp->cur);
}

/* Process next token as a simplename */
int
simplename (
     struct prs	*cp)		/* ptr to parser context block */
{
  int size;			
  int result = ACT_ERROR;	
  int simplesize = DNS_OPQ_SNAME_MAX;
  
  /* Search for end of name */
  size = scan_name(cp, tp);
  
  if (size > 0) {	
    if (cdsCvtSimpleToOpq (tp, 0, (cds_simple_name_t *)sp, &simplesize) ==
	DNS_SUCCESS) {
      /* Update command block for next token */
	cp->nxt = cp->cur + size;
	result = ACT_SUCCESS;	
      }
  }
  return(result);	
}

/* Process next token as a wild simplename */
int
wildsimplename (
     struct prs	*cp)		/* ptr to parser context block */
{
  int size;			
  int result = ACT_ERROR;	
  int simplesize = DNS_OPQ_SNAME_MAX;
  
  /* Search for end of name */
  size = scan_name(cp, tp);
  
  if (size > 0) {	
    if (cdsCvtSimpleToOpq(tp, 1, (cds_simple_name_t *)wsp, &simplesize) ==
	DNS_SUCCESS) {
      /* Update command block for next token */
	cp->nxt = cp->cur + size;
	result = ACT_SUCCESS;	
      }
  }
  return(result);	
}

/* Process next token as fullname without wildcards */
int
nowildfullname (
     struct prs	*cp)
{
  int size;	
  int result = ACT_ERROR;		
  int fullsize = DNS_OPQ_FNAME_MAX;
  
  /* Find end of name */
  size = scan_name(cp, tp);
  if (size > 0) {
    if (dnsCvtCDSFullToOpq(tp, (dns_opq_fname *)np, &fullsize) == 
	DNS_SUCCESS) {
      /* Update command block for next token */
      cp->nxt = cp->cur + size;	
      result = ACT_SUCCESS;	
    }
  }
  return(result);
}

/* Force next token to be root */
int
rootfullname (
     struct prs	*cp)
{
  int result = ACT_SUCCESS;
  int fullsize = DNS_OPQ_FNAME_MAX;
  
  if (dnsCvtCDSFullToOpq((unsigned char *)"/.:", (dns_opq_fname *)np, &fullsize)
				!= DNS_SUCCESS) 
      result = ACT_ERROR;	

  return(result);
}
/* Process next token as fullname with wildcards in last simple name */
int
wildfullname (
     struct prs	*cp)		/* ptr to parser context block */
{
  int size;	
  int result = ACT_ERROR;		
  int fullsize = DNS_OPQ_FNAME_MAX;
  unsigned char wildsimplename[DNS_STR_SNAME_MAX+1];
  int simplesize = DNS_OPQ_SNAME_MAX;
  int wildsize;
  int strsize;
  
  /* Find end of name */
  size = scan_name(cp, tp);

  if (size > 0) {
    char *end_p = (char *)tp;
    char *nextp = (char *)tp;
    /* 
     * Now find the last simplename and make sure it is a valid wildcarded
     * name.
     */
    while (*nextp) {
      if (nextp[0] == '\\') {
	if (nextp[1] != '\0')
	  nextp += LEN_mbc(nextp);
      } else if (*nextp == '/')
	end_p = nextp;

      nextp += LEN_mbc(nextp);
    }

    /* Get count of characters in last simplename */
    strsize = wildsize =  (char *)tp + strlen((char *)tp) - end_p;

    /* didn't find a terminating simplename */
    if (wildsize == 0) return(ACT_ERROR);
    
    /* Null terminate what is left */
    if (*end_p == '/') {
	*end_p++ = '\0';	
	--strsize;
    }

    if(strsize > DNS_STR_SNAME_MAX)
      return(ACT_ERROR);

    /* Copy simple name and terminator to its own buffer */
    memcpy(wildsimplename, end_p, strsize+1);

    /* If only name was wildcard, make a root, do this after copying wild
     * so don;'t collide
     */
    if (wildsize == size) {
	*tp = '/';
	*(tp+1) = '\0';	
    }

    if (dnsCvtCDSFullToOpq(tp, (dns_opq_fname *)np, &fullsize) == 
	DNS_SUCCESS) {
      if (dnsCvtCDSSimpleToOpq(wildsimplename, 1, (dns_opq_sname *)wsp, &simplesize) ==
	  DNS_SUCCESS) {

	/* Success if really was a wildcarded name */
	if ((EXT8(((SimpleName_t *)wsp)->sn_flag) == SN_cdswildcard) ||
	    (EXT8(((SimpleName_t *)wsp)->sn_flag) == SN_wildcard)) {

	  /* Update command block for next token */
	  cp->nxt = cp->cur + size;
	  result = ACT_SUCCESS;	
	}
      }
    }
  }
  return(result);
}
/* Process next token as an attributename */
int
attributename (
     struct prs	*cp)		/* ptr to parser context block */
{
  int size = DNS_OPQ_ANAME_MAX;
  int result = ACT_ERROR;
  unsigned char *end_p;
  int status;

  /* don't accept null attribute name */

  if (*cp->cur != '\0') {
    status = cdsCvtStrToObjID ( (unsigned char *)cp->cur, &type, 
				(unsigned char *)NULL,
				(int *)NULL, (dns_attr_name_t *)ap,
				&size, &end_p);
    if ( status == DNS_SUCCESS ) {
        cp->nxt = (char *)end_p;
        result = ACT_SUCCESS;	
    }
  }
  return(result);		
}

static int
stash_name (
    struct prs *cp,			/* ptr to parser context block */
    int (*recognize_routine)(char *))	/* ptr to proper recognize routine */
{
    int size;				/* number of characters parsed */
    int result;				/* value to be returned to parser */
    int cvt_code;			/* result of converting name */
    int fullsize;

    fullsize = CDS_OPQ_FNAME_MAX;
    result = ACT_ERROR;			/* assume parse will fail. */
    size =				/* are we looking at the right thing */
	(*recognize_routine)(	/* THIS LINE DIFFERS BY ROUTINE */
	    cp->cur);
    if (size > 0)			/* did we find it? */
	{				/* yes, we did */
	memcpy(tp, cp->cur, size);	/* have the data we want, */
	*(tp+size) = '\0';		/* made into a string */
	cvt_code = 
	    dnsCvtCDSFullToOpq(
		tp,			/* ptr to string form of name */
		(dns_opq_fname *)np,	/* where to put the opaque form */
		&fullsize);   		/* amount of room available for it */
	if (cvt_code == DNS_SUCCESS)	/* one last chance to fail */
	    {
	    cp->nxt = cp->cur + size;	/* tell where to resume */
	    result = ACT_SUCCESS;	/* report the good news */
	    }
	}
    return(result);			/* and away we go */
}


int
nsnwildfullname (
    struct prs	*cp)		/* ptr to parser context block */
{
    return(stash_name(cp, recognize_nsnwildfullname));
}

static int
recognize_nsnwildfullname (
char *p)
{
    char *p1 = p;

    while ((*p1 != ' ') && (*p1 != '\0')) /* find next whitespace */
	p1 += LEN_mbc(p1);
    return(p1 - p);  /* return length of current word */
}

