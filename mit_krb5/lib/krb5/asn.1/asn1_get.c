/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: asn1_get.c,v $
 * Revision 1.1.2.1  1996/06/05  20:33:46  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:37:26  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: asn1_get.c,v $
 * Revision 1.1.2.1  1996/06/05  20:33:46  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:37:26  arvind]
 *
 * Revision 1.1.1.2  1996/06/05  19:37:26  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 *
 * Revision /main/DCE_1.2.2/2  1996/05/30  21:18 UTC  mullan_s
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 
 * Revision /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1  1996/05/29  20:43 UTC  mullan_s
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1  1996/05/03  20:07 UTC  mullan_s
 * 	Add OSF/HP copyrights & RCS header.
 * 
 * $EndLog$
 */
/*
 * src/lib/krb5/asn.1/asn1_get.c
 * 
 * Copyright 1994 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#include "asn1_get.h"

asn1_error_code asn1_get_tag(buf, class, construction, tagnum, retlen)
     asn1buf * buf;
     asn1_class * class;
     asn1_construction * construction;
     asn1_tagnum * tagnum;
     int * retlen;
{
  asn1_error_code retval;
  
#ifdef OSF_DCE
  /* ANL - this was an inlined asn1buf_remains, but to get it
     to work with DCE, we need to go back to the modified
     version of the asn1buf_remains which was  modified */
 
  if (asn1buf_remains(buf) <= 0){   /* ANL */
#else
  if (buf == NULL || buf->base == NULL ||
      buf->bound - buf->next + 1 <= 0) {
#endif
      *tagnum = ASN1_TAGNUM_CEILING;
      return 0;
  }
  retval = asn1_get_id(buf,class,construction,tagnum);
  if(retval) return retval;
  retval = asn1_get_length(buf,retlen);
  if(retval) return retval;
  return 0;
}

asn1_error_code asn1_get_sequence(buf, retlen)
     asn1buf * buf;
     int * retlen;
{
  asn1_error_code retval;
  asn1_class class;
  asn1_construction construction;
  asn1_tagnum tagnum;

  retval = asn1_get_tag(buf,&class,&construction,&tagnum,retlen);
  if(retval) return retval;
  if(retval) return (krb5_error_code)retval;
  if(class != UNIVERSAL || construction != CONSTRUCTED ||
     tagnum != ASN1_SEQUENCE) return ASN1_BAD_ID;
  return 0;
}

/****************************************************************/
/* Private Procedures */

asn1_error_code asn1_get_id(buf, class, construction, tagnum)
     asn1buf * buf;
     asn1_class * class;
     asn1_construction * construction;
     asn1_tagnum * tagnum;
{
  asn1_error_code retval;
  asn1_tagnum tn=0;
  asn1_octet o;

#define ASN1_CLASS_MASK 0xC0
#define ASN1_CONSTRUCTION_MASK 0x20
#define ASN1_TAG_NUMBER_MASK 0x1F

  retval = asn1buf_remove_octet(buf,&o);
  if(retval) return retval;

  if(class != NULL)
    *class = (asn1_class)(o&ASN1_CLASS_MASK);
  if(construction != NULL)
    *construction = (asn1_construction)(o&ASN1_CONSTRUCTION_MASK);
  if((o&ASN1_TAG_NUMBER_MASK) != ASN1_TAG_NUMBER_MASK){
    /* low-tag-number form */
    if(tagnum != NULL) *tagnum = (asn1_tagnum)(o&ASN1_TAG_NUMBER_MASK);
  }else{
    /* high-tag-number form */
    do{
      retval = asn1buf_remove_octet(buf,&o);
      if(retval) return retval;
      tn = (tn<<7) + (asn1_tagnum)(o&0x7F);
    }while(tn&0x80);
    if(tagnum != NULL) *tagnum = tn;
  }
  return 0;
}

asn1_error_code asn1_get_length(buf, retlen)
     asn1buf * buf;
     int * retlen;
{
  asn1_error_code retval;
  asn1_octet o;

  retval = asn1buf_remove_octet(buf,&o);
  if(retval) return retval;
  if((o&0x80) == 0){
    if(retlen != NULL) *retlen = (int)(o&0x7F);
  }else{
    int num;
    int len=0;
    
    for(num = (int)(o&0x7F); num>0; num--){
      retval = asn1buf_remove_octet(buf,&o);
      if(retval) return retval;
      len = (len<<8) + (int)o;
    }
    if(retlen != NULL) *retlen = len;
  }
  return 0;
}
