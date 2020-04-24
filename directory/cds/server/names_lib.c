/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: names_lib.c,v $
 * Revision 1.1.8.2  1996/02/18  19:36:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:55  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:31:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:29  root]
 * 
 * Revision 1.1.6.5  1994/08/03  19:04:23  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:31:21  mccann]
 * 
 * Revision 1.1.6.4  1994/06/09  18:43:36  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:09  devsrc]
 * 
 * Revision 1.1.6.3  1994/04/19  17:06:23  mccann
 * 	Merge and try again
 * 	[1994/04/19  16:51:15  mccann]
 * 
 * 	More code cleanup
 * 
 * Revision 1.1.6.2  1994/04/14  14:51:23  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:21:07  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:11:58  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:43:58  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:22:30  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:23:01  zeliff]
 * 
 * Revision 1.1.2.4  1992/04/30  12:38:45  peckham
 * 	Gak! I'm losing my touch.  We must break out of the for() loop
 * 	in dowild() if we run out of name, and *then* we can return FALSE.
 * 
 * 	Also while here, testing showed up another bug which I discovered
 * 	was reported in OT#2468 and OT#3071.  This bug was in names_s_compare()
 * 	where no '*' was found.  It was checking that both strings were the
 * 	same length, which is not true with escaped wildcard specs.  Instead,
 * 	check if the match used up all of the name string.
 * 
 * 	While I'm here, break down and insert the use of const everywhere.
 * 	Also some reformatting for clarity.
 * 	[1992/04/29  18:53:02  peckham]
 * 
 * Revision 1.1.2.3  1992/04/28  20:24:16  peckham
 * 	Incomplete fix. Routine dowild() was not returning a value on failure.
 * 	[1992/04/28  18:45:37  peckham]
 * 
 * Revision 1.1.2.2  1992/04/28  13:29:39  peckham
 * 	CR 2463 complains that t*1 matches t12. Bug in dowild() can't handle
 * 	names ending in other than '*'.
 * 	[1992/04/24  15:18:17  peckham]
 * 
 * Revision 1.1  1992/01/19  15:25:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: names_lib.c
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
 */

#include <server.h>
#include <ctype.h>
#include <names.h>

/*
 * Local prototypes
 */
static const byte_t *
findchar (
    int			,
    const byte_t	*,
    const unsigned int	,
    const unsigned int	);

static int
cmpnames (
    int			*const,
    const byte_t	*,
    int                 ,
    const byte_t	*,
    const unsigned int	 );

static const byte_t *
findstr (
    const byte_t	*,
    int			*const,
    const byte_t	*const,
    const int		,
    const unsigned int	);

static int
dowild (
    const byte_t	*,
    int			,
    const unsigned int	,
    const byte_t	*,
    int			,
    const unsigned int	);


/*
 * findchar
 *     This routine searches a string for a specific character.
 *
 * Input:
 *     len = length of string
 *     name = address of string
 *     flg = wildcard type
 *     thechar = character to search for
 *
 * Value:
 *     pointer to character if found, NULL otherwise
 */
static const byte_t 
*findchar (int                 len,
           const byte_t        *name_p,
           const unsigned int  flg,
           const unsigned int  thechar)
{
    for (; 0 < len; name_p++, --len) {
	if (*name_p == thechar)
	    return(name_p);
	else 
	  /* CDS wildcard characters can be escaped, i.e.
	   * \*, \\ and \? are treated as non-wildcard characters */
	  if ((name_p[0] == '\\') && (len >= 2)
	      && (flg == SN_cdswildcard)
	      && ((name_p[1] == '?')
		  || (name_p[1] == '*')
		  || (name_p[1] == '\\'))) {
	    name_p++;
	    --len;
	  }
    }
	  	    
    return(NULL);
}

/*
 * cmpnames
 *     This routine compares two strings. The second string
 *     can contain the single match wild character.
 *
 * Input:
 *     len1 = length of name1 
 *     name1_p = address of array of characters
 *     flg1 = name1 type
 *     len2 = length of name2
 *     name2_p = address of array of characters
 *     flg2 = name2 type
 *
 * Value:
 *     TRUE  if name1 = name2
 *     FALSE otherwise
 */
static int 
cmpnames (int *const          len1,
          const byte_t        *name1_p,
          int                 len2,
          const byte_t        *name2_p,
          const unsigned int  flg2)
{
    for (; ((0 < len2) && (0 < *len1)); 
	 name1_p++, name2_p++, --(*len1), --len2)
      if (*name2_p != '?') {
	  if (flg2 == SN_wildcard) {
	      char cc1 = islower(*name1_p) ? toupper(*name1_p) : *name1_p;
	      char cc2 = islower(*name2_p) ? toupper(*name2_p) : *name2_p;

	      if (cc1 != cc2)
		return(FALSE);
	  } else {
	      /* CDS wildcard characters can be escaped, i.e.
	       * \*, \\ and \? are treated as non-wildcard characters */
	      if ((name2_p[0] == '\\') && (len2 >= 2)
		  && ((name2_p[1] == '?')
		      || (name2_p[1] == '*')
		      || (name2_p[1] == '\\')))  {
		  name2_p++;
		  --len2;
	      }
	      if (*name2_p != *name1_p)
		return(FALSE);
	  }
      }

    /* If broke out of loop cause ran out of len1, didn't match */
    if (len2 > 0)
	return(FALSE);

    /* Made it through loop so must be equal */
    return (TRUE);
}

/*
 * findstr
 *     This routine searches one string for another.
 *     The pattern string can contain the single wild character.
 *
 * Input:
 *     name_p = address of string to search
 *     len_p = length of string, on output = length of matched string
 *     pattern_p = address of string to find
 *     patlen = length of string to find
 *     flg = nametype flg
 *
 * Value:
 *     pointer to match or NULL
 */
static const byte_t 
*findstr (const byte_t         *name_p,
          int *const           len_p,
          const byte_t *const  pattern_p,
          const int            patlen,
          const unsigned int   flg)
{
    byte_t *const endname_p = SKIP_bytes(name_p, *len_p);

    /* Loop through name looking for a match */
    for (; name_p <= endname_p; ++name_p, --(*len_p)) {
	int notused = *len_p;

	if (cmpnames(&notused, name_p, patlen, pattern_p, flg)) {
	    *len_p = *len_p - notused;
	    return(name_p);
	}
    }

    return(NULL);
}

/*
 * dowild
 *     This routine is called recursively to compare a string against
 * another with wildcards in it.  This routine is entered with name2_p 
 * pointing after a wildcard.  The wildcard can match any number of 
 * characters and this routine must try them all.
 * 
 *
 * Input:
 *     name1_p = address of non-wildcarded string
 *     len1 = length of remainder of 1st string
 *     flg1 = type of name1
 *     name2_p = address of remainder of wildcarded string
 *     len2 = length of 2nd string
 *     flg2 = type of name2
 *
 * Value:
 *     TRUE  if match
 *     FALSE otherwise
 */
static int 
dowild (const byte_t        *name1_p,
        int                 len1,
        const unsigned int  flg1,
        const byte_t        *name2_p,
        int                 len2,
        const unsigned int  flg2)
{
    int wildlen;
    register const byte_t *pattern_p;

    /* Collect groups of wildcards */
    for (;; name2_p++, --len2) {
	if (len2 <= 0)
	  return(TRUE); /* previous wildcard matches everything remaining */

	if (*name2_p != '*') 
	  break;
    }

    /* If there is nothing left in the first string, then it has been
     * equal to so far and is therfore less then the wildcarded string 
     */

    if (len1 <= 0)
	return(FALSE);

    /* Locate next '*' */
    pattern_p = findchar(len2, name2_p, flg2, '*');
    if (!pattern_p)
	wildlen = len2;
    else
	wildlen = pattern_p - name2_p;

    /* In between two '*' is a pattern that must be matched in the
       string.  It is possible that the first match is not the
       only match so this process must recurse until a match of the
       entire string is found or there is nothing left to compare
       against. */

    for (;0 < len1; name1_p++, --len1)
	{
	int matched = len1;

	/* find the first pattern match */
	pattern_p = findstr(name1_p, &matched, name2_p, wildlen, flg2);

	/* If pattern not there, defintly not equal */
	if (!pattern_p)
	    return(FALSE);

	/* Chalk everything before pattern up to previous '*' */
	len1 -= (pattern_p - name1_p);
	name1_p = pattern_p;

	if (len2 == wildlen) {
	    /* no more wild cards */
	    if (len1 == wildlen)
		return(TRUE);		/* exact match */
	} else {
	    /* more wild cards */
	    if (dowild(SKIP_bytes(name1_p, matched),
		       len1 - matched,
		       flg1,
		       SKIP_bytes(name2_p, 1 + wildlen),
		       len2 - 1 - wildlen,
		       flg2))
	      return(TRUE);
	}
    }
    return(FALSE);
}

/*
 * names_s_compare
 *     This routine compares two simple names and returns the results.
 * Wildcards may appear in the second name. Comparisons are done case-neutral
 * for quoted and normal names.  In CDS, names are case sensitive and
 * * and ? are valid characters, so with cdswildcard names these characters
 * are escaped with \ if they are NOT to be treated as wildcards.
 *
 * The neutral case is currently defined by:
 *      binary - no change
 *      simple, quoted - the upper case in the string library tables
 *      typed - normalized by routines in normal.c (X.500 support)
 *
 * Input:
 *     name1_p = address of first simple name
 *     name2_p = address of second simple name
 *
 * Value:
 *      TRUE if equal
 *      FALSE otherwise
 */
int 
names_s_compare (SimpleName_t  *name1_p,
                 SimpleName_t  *name2_p)
{
    register const byte_t *wild_string_p;
    int len1, len2;

    /* If no wildcards, do compare */
    if ((EXT8(name2_p->sn_flag) != SN_wildcard) &&
	(EXT8(name2_p->sn_flag) != SN_cdswildcard)) 
	return (dnsCmpSimple((dns_opq_sname *)name1_p,
				(dns_opq_sname *)name2_p) == 0);

    /* Binary and objectid names will only match an all wildcarded string */
    switch (EXT8(name1_p->sn_flag)) {
      case SN_binary:
      case SN_objectid:
	{
	register int checkLen, s_cnt = 0, q_cnt = 0;
	    
	if (checkLen = EXT8(name2_p->sn_length)) {
	    register byte_t *check_p = name2_p->sn_name;
		
	    do {
		switch (*check_p) {
		  case '?':			/* single character match */
		    q_cnt++;
		    break;
		  case '*':			/* multiple character match */
		    s_cnt++;
		    break;
		  default:
		    return(FALSE);
		}
	    } while (check_p++, --checkLen);
	}
	if (q_cnt > EXT8(name1_p->sn_length)) return(FALSE);
	if (q_cnt == EXT8(name1_p->sn_length)) return(TRUE);
	return(s_cnt > 0);
    }
    case SN_null:
    case SN_typed:
	/* X.500 typed name only matches single '*' */
	/* (HOW ABOUT MULTIPLE '*' ?) */
	return((EXT8(name2_p->sn_length) == 1) &&
	       (name2_p->sn_name[0] == '*'));
		
    }  /* End case */
    
    /* At this point name2 is sn_wildcard or sn_cdswildcard */

    len1 = EXT8(name1_p->sn_length);
    len2 = EXT8(name2_p->sn_length);

    /* Get pointer to '*' if any */
    wild_string_p = findchar ( len2, name2_p->sn_name, 
				EXT8(name2_p->sn_flag), '*');
    
    if (wild_string_p)   /* found a '*' */
      len2 = wild_string_p - name2_p->sn_name; /* get nbr characters before '*' */

    if (!cmpnames( &len1, name1_p->sn_name, 
	           len2, name2_p->sn_name, EXT8(name2_p->sn_flag)))
      return(FALSE);

    /* If we did NOT find an '*', the wildcard is '?'. The cmpnames above
     * should be considered succesful if and only if len1 used up !!!!!! */

    if (!wild_string_p)
      return(len1 == 0);

    /* We have at least one string wildcard so keep trying */
    return(dowild(SKIP_bytes(name1_p->sn_name, EXT8(name1_p->sn_length)-len1),
		  len1, EXT8(name1_p->sn_flag),
		  SKIP_byte(wild_string_p), EXT8(name2_p->sn_length) - len2 - 1,
		  EXT8(name2_p->sn_flag)));
  }
#ifdef LOWER_BUILD

/* names_lower
 *	Convert a full name to lower case
 *
 * Input:
 *	in_p = pointer to input name
 *	out_p = pointer to output name
 */
void 
names_lower (FullName_t  *in_p,
             FullName_t  *out_p)
{
    COPY_FullName(in_p, out_p);

    if (!EMPTY_FullName(out_p))
	{
	register SimpleName_t *sname_p = (SimpleName_t *)out_p->fn_name;

	while (TRUE)	/* this loop should be optimal... */
	    {
	    switch (EXT8(sname_p->sn_flag))
		{
		default:
		    dns_lower(sname_p->sn_name, sname_p->sn_name,
			    EXT8(sname_p->sn_length));
		case SN_binary:
		case SN_objectid:
		    sname_p = (SimpleName_t *)SKIP_SimpleName(sname_p);
		    continue;	/* the while (TRUE)... */
		case SN_null:
		    break;
		}
	    break;
	    }
	}
}
#endif


/* names_neutralize_case
 *
 * Convert a full name to a neutral case for comparisons.
 *
 * Since names are stored in neutral case, the neutral case determines
 * the collating sequence for the name.
 *
 * The neutral case is currently defined by the upper case in the string
 * library tables.
 *
 * Input:
 *	in_p = pointer to input name
 *	out_p = pointer to output name
 */
void 
names_neutralize_case (FullName_t  *in_p,
                       FullName_t  *out_p)
{
    COPY_FullName(in_p, out_p);

    if (!EMPTY_FullName(out_p))
	{
	register SimpleName_t *sname_p = (SimpleName_t *)out_p->fn_name;

	while (TRUE)	/* this loop should be optimal... */
	    {
	    switch (EXT8(sname_p->sn_flag))
		{
		default:
		    dns_upper(sname_p->sn_name,sname_p->sn_name,
			      EXT8(sname_p->sn_length));
		case SN_binary:
		case SN_cds:
		case SN_objectid:
		    sname_p = (SimpleName_t *)SKIP_SimpleName(sname_p);
		    continue;	/* the while (TRUE)... */
		case SN_null:
		    break;
		}
	    break;
	    }
	}
}

/* names_s_neutralize_case
 *
 * Convert a simple name to a neutral case for comparisons.
 *
 * Since names are stored in neutral case, the neutral case determines
 * the collating sequence for the name.
 *
 * The neutral case is currently defined by the upper case in the string
 * library tables.
 *
 * Input:
 *	in_p = pointer to input name
 *	out_p = pointer to output name
 */
void 
names_s_neutralize_case (SimpleName_t  *in_p,
                         SimpleName_t  *out_p)
{
    switch (EXT8(in_p->sn_flag))
	{
	default:
	    COPY_byte(in_p->sn_flag, out_p->sn_flag);
	    COPY_byte(in_p->sn_length, out_p->sn_length);
	    dns_upper(in_p->sn_name, out_p->sn_name,
		      EXT8(in_p->sn_length));
	    break;
	case SN_binary:
	case SN_objectid:
	    COPY_SimpleName(in_p, out_p);
	    break;
	}
}

/*
 * names_simple_casemask
 *
 * Apply a casemask to an attribute name - assumed to be in case-neutral
 * format.
 *
 * The neutral case is currently defined by the upper case in the string
 * library tables.  Reversal of this to the input name is done by accessing
 * the lower case equivalents in the string library.
 *	
 * Input:
 *	in_p = address of name in neutral case
 *	case_p = address of casemask
 *	out_p = address to return converted name
 */
void 
names_simple_casemask (SimpleName_t      *in_p,
                       SimpleNameMask_t  *case_p,
                       SimpleName_t      *out_p)
{
    switch (EXT8(in_p->sn_flag)) {
    case SN_normal:
    case SN_quoted:
	{
	register byte_t *in_name_p = in_p->sn_name;
	register byte_t *out_name_p = out_p->sn_name;
	register int length;
	register byte_t bits = 0;
	register int nbits = 0;
	int temp;

	temp = sizeof(*case_p);
	dce_assert(cds__svc_handle,
		   (temp == sizeof(bits)));

	/* if no casemask supplied, assume follows name */
	if (!case_p) case_p = SKIP_SimpleName(in_p);
     
	/* Move constant header */
	INS8(out_p->sn_flag, EXT8(in_p->sn_flag));
	INS8(out_p->sn_length, length = EXT8(in_p->sn_length));

	for (; 0 < length; in_name_p++, out_name_p++, --length)
	    {
	    if (--nbits <= 0) {
		bits = *case_p++;
		nbits = 8;
	    } else
		bits >>= 1;

	    if (bits & 1)
		*out_name_p = tolower(*in_name_p);
	    else
		*out_name_p = *in_name_p;
	    }
	}
	break;
    default:
	COPY_SimpleName(in_p, out_p);
	break;
    }
}

/*
 * names_fullmatch
 * Compare two full names disregarding case.
 *
 *
 * Input:
 *     fname1_p = address of full name
 *     fname2_p = address of full name
 *
 * Value:
 *     TRUE  if equal
 *     FALSE otherwise
 *
 */
int 
names_fullmatch (FullName_t  *fname1_p,
                 FullName_t  *fname2_p)
{
    return (dnsCmpFull((dns_opq_fname_t *)fname1_p,
			(dns_opq_fname_t *)fname2_p) == 0);
}

/*
 * names_new
 *     Build a full name from a simple name and UID. 
 *
 * Input:
 *     uid_p = address of uid
 *     sname_p = address of simple name
 *     fname_p = address to put results
 *
 *
 */
void 
names_new (ObjUID_t      *uid_p,
           SimpleName_t  *sname_p,
           FullName_t    *fname_p)
{
    register SimpleName_t *next_p = (SimpleName_t *)fname_p->fn_name;

    COPY_ObjUID(uid_p, fname_p->fn_root);

    if (sname_p)
	{
	/* Copy first simple name */
	COPY_SimpleName(sname_p, next_p);
	next_p = (SimpleName_t *)SKIP_SimpleName(next_p);
	}

    COPY_SimpleName(NullSimpleName, next_p);
    INS16(fname_p->fn_length,
	DIFF_bytes(next_p, fname_p->fn_name) + sizeof(NULLSIMPLENAME_u));
}

/*
 * names_pointlast 
 *      Return a pointer to the simple name preceeding the
 * last simple name of a fullname if there is one.  Otherwise point at
 * the last simple name.
 *
 * Input: 
 *      fname_p = address of fullname
 *
 * Value:
 *      pointer to simple name
 */
SimpleName_t 
*names_pointlast (FullName_t *fname_p)
{
    if (!EMPTY_FullName(fname_p))
	{
	register SimpleName_t *current_p = (SimpleName_t *)fname_p->fn_name;
	register SimpleName_t *previous_p;

	do
	    {
	    previous_p = current_p;
	    current_p = (SimpleName_t *)SKIP_SimpleName(previous_p);
	    }
	while (EXT8(current_p->sn_flag) != SN_null);

	return(previous_p);
	}
    else
	return((SimpleName_t *)fname_p->fn_name);
}

/*
 * names_stripright
 *
 *      Return a pointer to the simple name preceeding the
 * last simple name of a fullname.  The last simple name is nullname.
 *
 * Input: 
 *      fname_p = address of fullname
 *
 * output:
 *      parent_p = pointer to full name
 */
void 
names_stripright (FullName_t  *fname_p,
                  FullName_t  *parent_p)
{
    COPY_FullName(fname_p, parent_p);

    if (!EMPTY_FullName(parent_p))
	{
	register SimpleName_t *last_p = names_pointlast(parent_p);

	INS16(parent_p->fn_length,
		EXT16(parent_p->fn_length) - LEN_SimpleName(last_p));
	COPY_SimpleName(NullSimpleName, last_p);
	}
}

/*
 * names_append 
 *      Append a simple name to the end of a fullname.  
 *
 * Input: 
 *      fname_p = address of fullname
 *	sname_p = address of simplename
 *
 */
void 
names_append (SimpleName_t  *sname_p,
              FullName_t    *fname_p)
{
    register SimpleName_t *next_p;
    register int fnameLen = EXT16(fname_p->fn_length);

    /* Remove terminating NullSimpleName */
    if (0 < fnameLen)
	{
	fnameLen -= sizeof(NULLSIMPLENAME_u);
	INS16(fname_p->fn_length, fnameLen);
	}

    /* Append a simple name and new NullSimpleName */
    next_p = (SimpleName_t *)SKIP_FullName(fname_p);
    COPY_SimpleName(sname_p, next_p);
    COPY_SimpleName(NullSimpleName, SKIP_SimpleName(next_p));

    INS16(fname_p->fn_length,
	fnameLen + LEN_SimpleName(sname_p) + sizeof(NULLSIMPLENAME_u));
}

/*
 * names_count
 *      Return a count of simple names in a full name not including the
 * terminating nullname.
 *
 * Input: 
 *      fname_p = address of fullname
 *
 * Value:
 *      count
 */
int 
names_count (FullName_t *fname_p)
{
  return(dnsCountSimple((dns_opq_fname *)fname_p));
}

/* 
 * names_swap
 *     This routine moves a simple name from the beginning of
 * of the unresolved name to the end of the resolved name and
 * updates the full names appropriatly.  The total amount of
 * space used is not changed.  Anything following the resolved name
 * should be left unchanged.
 *
 * Input:
 *       unresolved_p = address of the unresolved name within progress record
 *
 * Value:
 *       pointer to resolved name in progress record
 */
FullName_t 
*names_swap (FullName_t *unresolved_p)
{
    SimpleName_u sname;
    FullName_t *resolved_p = (FullName_t *)SKIP_FullName(unresolved_p);
    register SimpleName_t *sname_p = (SimpleName_t *)unresolved_p->fn_name;
    register int unresolvedLen, resolvedLen, snameLen;

    /* if already the end, all done */
    if (EMPTY_FullName(unresolved_p))
	 return(resolved_p);

    unresolvedLen = EXT16(unresolved_p->fn_length);
    resolvedLen = EXT16(resolved_p->fn_length);
    snameLen = LEN_SimpleName(sname_p);

    /* Store new resolved simple name in safe place */
    COPY_bytes(sname_p, sname, snameLen);

    /* Update unresolved name length to reflect 1 less simple name */
    INS16(unresolved_p->fn_length, unresolvedLen -= snameLen);

    /* 
     * Slide unresolved name up to fill in space previously used by
     * new resolved name 
     */
    MOVE_bytes(SKIP_SimpleName(sname_p), unresolved_p->fn_name, unresolvedLen);

    /* Remove the NullSimpleName from the end of the resolved name */
    if (0 < resolvedLen)
	{
	resolvedLen -= sizeof(NULLSIMPLENAME_u);
	INS16(resolved_p->fn_length, resolvedLen);
	}

    /* Slide resolved name up so it immediatly follows the unresolved name */
    MOVE_bytes(resolved_p, SKIP_FullName(unresolved_p), LEN_FullName(resolved_p));

    /* Update resolved name pointer */
    resolved_p = (FullName_t *)SKIP_FullName(unresolved_p);
    sname_p = (SimpleName_t *)SKIP_FullName(resolved_p);

    /* 
     * A null simple name is already at the end, but we are
     * going to wipe it out and re-add it 
     */
    COPY_bytes(sname, sname_p, snameLen);
    COPY_SimpleName(NullSimpleName, SKIP_bytes(sname_p, snameLen));

    /* Update size of resolved name */
    INS16(resolved_p->fn_length,
	resolvedLen + snameLen + sizeof(NULLSIMPLENAME_u));

    return(resolved_p);
}     

/*
 * names_matchwild
 *     This routine compares a full name against a potentially
 * wildcard full name.  This routine is called in searching ACs.
 *
 * Input:
 *     name_1 = address of non-wildcarded name
 *     name_2 = address of potentially wildcarded name
 *
 * Value:
 *     True if match, False otherwise.
 */
int 
names_matchwild (FullName_t  *name1_p,
                 FullName_t  *name2_p)
{
    register SimpleName_t *sname1_p;
    register SimpleName_t *sname2_p;

    if (NE_ObjUID(name1_p->fn_root, name2_p->fn_root))
	return(FALSE);

    /* What if either is an abbreviated NullFullName ??? */

    sname1_p = (SimpleName_t *)name1_p->fn_name;
    sname2_p = (SimpleName_t *)name2_p->fn_name;

    while (EXT8(sname2_p->sn_flag) != SN_ellipsis)
	{

	/*
	 * Make certain that have not reached end of either name.
	 */
	if ((EXT8(sname1_p->sn_flag) == SN_null) &&
		(EXT8(sname2_p->sn_flag) == SN_null))
	    break;				/* Same fullname */
	else if ((EXT8(sname1_p->sn_flag) == SN_null) ||
		(EXT8(sname2_p->sn_flag) == SN_null))
	    return (FALSE);			/* Different fullname  */

	/* Check if names are not the same */
	if (!names_s_compare(sname1_p, sname2_p))
	    return(FALSE);

	sname1_p = (SimpleName_t *)SKIP_SimpleName(sname1_p);
	sname2_p = (SimpleName_t *)SKIP_SimpleName(sname2_p);
	}

    return(TRUE);
}
