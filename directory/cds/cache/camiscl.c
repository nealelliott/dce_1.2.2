/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: camiscl.c,v $
 * Revision 1.1.6.2  1996/02/18  19:24:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:14  marty]
 *
 * Revision 1.1.6.1  1995/12/08  00:10:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:40  root]
 * 
 * Revision 1.1.4.5  1994/08/03  18:59:03  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:04:04  mccann]
 * 
 * Revision 1.1.4.4  1994/06/23  18:28:30  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:45:17  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  16:07:14  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:49  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:38:05  peckham
 * 	Remove dnstables.h dependency.
 * 	Also removed DNSRPC conditional. This seems to be
 * 	a very old conditional that is not defined,
 * 	even though it should be. Janet and I saw no
 * 	impact to re-introducing the additional check.
 * 	[1994/04/11  17:50:58  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  21:55:57  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:48:24  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:37:17  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:20:42  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:23:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: camiscl.c
 * Version: X02.13
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
#include <cache.h>
#include <ctype.h>

/*
 * convert simplename to upper case 
 */
void 
dns_sname_upper (SimpleName_t  *in_p,
                 SimpleName_t  *out_p)
{
  register byte_t *outname_p, *inname_p;
  register int pos, len;

  switch (EXT8(in_p->sn_flag)) {
  case SN_objectid:
  case SN_binary:
    /* Don't convert binary names */
    if (in_p != out_p)
      COPY_SimpleName(in_p, out_p);
    break;
  default:
    INS8(out_p->sn_flag, EXT8(in_p->sn_flag));
    INS8(out_p->sn_length, len = EXT8(in_p->sn_length));
    outname_p = out_p->sn_name;
    inname_p = in_p->sn_name; 
    for (pos=0; pos<len; outname_p++, inname_p++, pos++)
      if (islower(*inname_p))
	*outname_p = toupper(*inname_p);
      else
	*outname_p = *inname_p;
  }
}

/*
 * SetsEqual
 *   See if the members of 2 sets are equal.  Ignore timestamps
 * on members but retain ordering.
 */
int 
SetsEqual (Set_t  *set1_p,
           Set_t  *set2_p)
{
      MemberValue_t *mem1_p, *mem2_p;
      int i;

      if ((LEN_Set(set1_p) != LEN_Set(set2_p))  ||
	  (NUM_Set(set1_p) != NUM_Set(set2_p))) return(0);
	
      for (i=0; i<NUM_Set(set1_p); ++i) {
	mem1_p = (MemberValue_t *)INDEX_Set(set1_p, i);
	mem2_p = (MemberValue_t *)INDEX_Set(set2_p, i);
	if (EXT8(mem1_p->mv_valuetype) != EXT8(mem2_p->mv_valuetype))
	  return(0);
	if (LEN_MemberValue(mem1_p) != LEN_MemberValue(mem2_p)) return(0);
	if NE_bytes(mem1_p->mv_value, mem2_p->mv_value, 
		    EXT16(mem1_p->mv_length)) return(0);
      }
      return(1);
}











