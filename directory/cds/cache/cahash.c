/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cahash.c,v $
 * Revision 1.1.6.2  1996/02/18  19:24:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:06  marty]
 *
 * Revision 1.1.6.1  1995/12/08  00:10:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:35  root]
 * 
 * Revision 1.1.4.5  1994/08/03  18:59:02  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:04:02  mccann]
 * 
 * Revision 1.1.4.4  1994/06/23  18:28:25  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:43:57  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  16:07:08  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:44  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:37:59  peckham
 * 	Remove duplicate header includes.
 * 	[1994/04/11  17:39:22  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  21:55:47  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:47:46  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:36:55  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:20:05  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:23:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module cahash.c
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
#include <cache.h>
#include <dce/sys_time.h>
#include <uc_child.h>
#define HASH_LEN 25

static unsigned int
CAHash (
  unsigned char *string,
  int len);


/*
 * This routine produces a sum givin a string.  It divides the
 * string into 25 byte segments, adds the elements of each segment to
 * powers of 2 and totals the results.
 * Since it is called frequently, it attempts to optimize itself.
 */
static unsigned int 
CAHash (unsigned char  *string,
        int            len)
{
  int seg_cnt;			/* number of full segments */
  int seg_remainder;		/* bytes left in segment */
  register int j;		/* loop counter */
  register unsigned int total;
  register unsigned int *sum_p; 
  unsigned int sum[HASH_LEN];
  
  seg_cnt = len / HASH_LEN;
  seg_remainder = len % HASH_LEN;

  /* Optimize for short strings */
  if (seg_cnt == 0) {
    total = 0;
    for (j=0; j<seg_remainder; ++j)
      total += (*string++ << j);
    return(total);
  }

  /* 
   * The first time through just set up the sum array.
   * this reduces the overhead of initializing the array.
   */
  sum_p = sum;
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    
  *sum_p++ = *string++;    

  /* Now do cumulative */
  for (j=1; j<seg_cnt; ++j) {
    /* Repeat HASH_LEN times */
    sum_p = sum;
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
    *sum_p++ += *string++;    
  }

  /* Add in remaining bytes and compute total */
  total = 0;
  sum_p = sum;
  for (j=0; j<seg_remainder; ++j)
    total += ((*sum_p++ + *string++) << j);
  
  /* Add in powers not already covered */
  for (j=seg_remainder; j<HASH_LEN; ++j)
    total += (*sum_p++ << j);
  
  return(total);
}

/*
 * Get the hash index for a directory name 
 */
int 
CAHashDir (FullName_t *dirname_p)
{
  unsigned int total;
  
  /* Get hash value */
  total = CAHash((unsigned char *)dirname_p, LEN_FullName(dirname_p));
  
  /* Convert to array index */
  return(total % CAfixed_p->ca_dir_hash_len);
}

/*
 * Gett the hash index for a clearinghouse name 
 */
int 
CAHashCle (FullName_t *clename_p)
{
  unsigned int total;
  
  /* Get hash value */
  total = CAHash((unsigned char *)clename_p, LEN_FullName(clename_p));
  
  /* Convert to array index */
  return(total % CAfixed_p->ca_cle_hash_len);
}

/*
 * Get the hash index for a user name 
 */
int 
CAHashUser (cds_user_t user)
{
  unsigned int total;
  
  /* Get hash value */
#ifdef DCE_SEC
  total = CAHash (user+sizeof(field16), EXT16(user));
#else
  total = CAHash(user->sn_name, EXT8(user->sn_length));
#endif
  
  /* Convert to array index */
  return(total % CAfixed_p->ca_user_hash_len);
}

/*
 * Get the hash index for a namespace entry name 
 */
int 
CAHashName (FullName_t *name_p)
{
  unsigned int total;
  
  /* Get hash value */
  total = CAHash((unsigned char *)name_p, LEN_FullName(name_p));
  
  /* Convert to array index */
  return(total % CAfixed_p->ca_name_hash_len);
}

/*
 * Get the hash index for a softlink name 
 */
int 
CAHashLink (FullName_t *linkname_p)
{
  unsigned int total;
  
  /* Get hash value */
  total = CAHash((unsigned char *)linkname_p, LEN_FullName(linkname_p));
  
  /* Convert to array index */
  return(total % LINK_HASH_LEN);
}

/*
 * Get the hash index for a group/member name 
 */
int 
CAHashGroup (FullName_t  *group_p,
             FullName_t  *member_p)
{
  unsigned int total;
  
  
  /* Get hash value */
  total = CAHash((unsigned char *)group_p, LEN_FullName(group_p));
  total += CAHash((unsigned char *)member_p, LEN_FullName(member_p));
  
  /* Convert to array index */
  return(total % USERGROUP_HASH_LEN);
}

/*
 * Get the hash index for an attribute name 
 */
int 
CAHashAttr (AttributeName_t *attrname_p)
{
  unsigned int total;
  
  /* Get hash value */
  total = CAHash((unsigned char *)attrname_p, LEN_AttributeName(attrname_p));
  
  /* Convert to array index */
  return(total % CAfixed_p->ca_attr_hash_len);
}

/*
 * Hash time to get index into user table
 */
int 
CAHashAccess (Time_t *time_p)
{
  return(EXT32(time_p) % USERACCESS_HASH_LEN);
}
