/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dns_record.c,v $
 * Revision 1.1.8.2  1996/02/18  19:32:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:13  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:21:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:54  root]
 * 
 * Revision 1.1.6.5  1994/08/03  19:03:05  mccann
 * 	includes cleanup
 * 	[1994/08/01  19:40:37  mccann]
 * 
 * Revision 1.1.6.4  1994/06/30  19:16:29  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:58:46  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:40:49  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:12  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/29  15:52:03  peckham
 * 	Remove unnecessary conditionals.
 * 	[1994/04/29  13:59:55  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:05:50  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:25:11  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  13:53:44  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:07:00  zeliff]
 * 
 * Revision 1.1.4.2  1992/10/26  18:32:26  peckham
 * 	OT#4001: Remove cma_*_global() references.
 * 	[1992/10/26  18:30:48  peckham]
 * 
 * Revision 1.1.2.2  1992/03/22  22:15:47  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:30:18  weisman]
 * 
 * Revision 1.1  1992/01/19  15:19:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: dns_record.c
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
 *
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <dce/assert.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <dce/dns_record.h>
#include <dce/sys_time.h>
#include <dce/rpc.h>

DNS_CONST Time_t	 NullTime[1]		= { INITIALIZE_Time };

DNS_CONST Timeout_t	 NullTimeout[1]		= { INITIALIZE_Timeout };

DNS_CONST Node_t	 NullNode[1]		= { INITIALIZE_Node };

DNS_CONST utc_u	         Nullutc[1]	        = { INITIALIZE_utc };

DNS_CONST NULLSIMPLENAME_t EllipsisSimpleName[1] = { SN_ellipsis, 0 };

DNS_CONST NULLSET_t	 NullSet[1]		= { INITIALIZE_Set };

DNS_CONST NULLATTRIBUTE_t NullAttribute[1]	= { INITIALIZE_Attribute };

DNS_CONST byte_t	GlobalRoot[]		= { "\10\3..." };

int 
cds_record_init (void)
{
#ifdef	DCE_CDS_DEBUG

    dce_assert(cds__svc_handle,sizeof(Set_u) <= sizeof(SetMember_u));
    dce_assert(cds__svc_handle,MV_present == MH_present);
    dce_assert(cds__svc_handle,MVX_absent == MHX_absent);
    dce_assert(cds__svc_handle,MV_reserved == MH_reserved);

#endif

    return(TRUE);
}

/*
 * compare 2 DTSS utc values.  Since DNS packs bytes and
 * DTSS doesn't copy to real structures.
 */
int 
cds_utc_equal (byte_t  *utc1_p,
               byte_t  *utc2_p)
{
  utc_t utc1,utc2;
  enum utc_cmptype oper;

  /* Copy unaligned data to aligned */
  COPY_utc(utc1_p, &utc1);
  COPY_utc(utc2_p, &utc2);

  utc_cmpmidtime(&oper, &utc1, &utc2);
  if (oper == utc_equalTo) 
  {
      return(1);
  }

  return(0);
}

int 
cds_utc_greater (byte_t  *utc1_p,
                 byte_t  *utc2_p)
{
  utc_t utc1,utc2;
  enum utc_cmptype oper;

  /* Copy unaligned data to aligned */
  COPY_utc(utc1_p, &utc1);
  COPY_utc(utc2_p, &utc2);

  utc_cmpmidtime(&oper, &utc1, &utc2);
  if (oper == utc_greaterThan) 
  {
      return(1);
  }

  return(0);
}

int 
cds_utc_less (byte_t  *utc1_p,
              byte_t  *utc2_p)
{
  utc_t utc1,utc2;
  enum utc_cmptype oper;

  /* Copy unaligned data to aligned */
  COPY_utc(utc1_p, &utc1);
  COPY_utc(utc2_p, &utc2);

  utc_cmpmidtime(&oper, &utc1, &utc2);
  if (oper == utc_lessThan) 
  {
      return(1);
  }

  return(0);
}

void 
cds_utc_add (byte_t  *t1_p,
             byte_t  *t2_p,
             byte_t  *t3_p)
{
  utc_t t1,t2,t3;

  COPY_utc(t1_p, &t1);
  COPY_utc(t2_p, &t2);

  utc_addtime(&t3, &t2, &t1);
  COPY_utc(&t3, t3_p);
}

byte_t *
cds_pass_AttributeContents (const byte_t *const ptr)
{
    register const AttributeContents_t *const ac_p =
	(AttributeContents_t *)ptr;

    switch(EXT8(ac_p->ac_type)) 
    {
    case AT_null:
	return(SKIP_byte(ac_p->ac_type));

    case AT_single:
	return(SKIP_SetMember(ac_p->ac_single));

    case AT_set:
	return(SKIP_Set(ac_p->ac_set));
    }

    return((byte_t *)0);
}

byte_t *
cds_pass_AttributeSpecifier (const byte_t *const ptr)
{
    register const AttributeSpecifier_t *const as_p =
	(AttributeSpecifier_t *)ptr;

    switch(EXT8(as_p->as_type)) 
    {
    case AT_null:
	return(SKIP_byte(as_p->as_type));

    default:
	return(SKIP_AttributeName(as_p->as_name));
    }
}

byte_t *
cds_pass_NetworkAddress (const byte_t *ptr_p)
{
    register int cnt;

    if (cnt = *ptr_p++)
    {
	do 
	{
	    ptr_p = &ptr_p[1 + ptr_p[0]];
	    ptr_p = &ptr_p[1 + ptr_p[0]];
	} while (--cnt);
    }

    return((byte_t *)ptr_p);
}

byte_t *
cds_pass_Progress (const byte_t *ptr_p)
{
    ptr_p = SKIP_FullName(((Progress_t *)ptr_p)->pr_unresolvedName);
    ptr_p = SKIP_FullName(ptr_p);

    return(SKIP_Set(ptr_p));
}

byte_t *
cds_pass_ReplicaPointer (const byte_t *ptr_p)
{
    ptr_p = SKIP_Set(((ReplicaPointer_t *)ptr_p)->rp_addressHint);
    ptr_p = SKIP_ReplicaType(ptr_p);

    return(SKIP_FullName(ptr_p));
}

byte_t *
cds_pass_Update (const byte_t *ptr_p)
{	
    ptr_p = SKIP_AttributeSpecifier(((Update_t *)ptr_p)->ud_attribute);

    if (*ptr_p++ == BL_true)
	ptr_p = SKIP_AtomicValue(ptr_p);

    return((byte_t *)ptr_p);
}

byte_t *
cds_pass_UpdatePacket (const byte_t *ptr_p)
{
    register int cnt;

    ptr_p = SKIP_FullName(((UpdatePacket_t *)ptr_p)->up_entryName);
    ptr_p = SKIP_Timestamp(ptr_p);
    ptr_p = SKIP_EntryType(ptr_p);

    if (cnt = *ptr_p++)
    {
	do 
	{
	    ptr_p = SKIP_Update(ptr_p);
	} while (--cnt);
    }

    return((byte_t *)ptr_p);
}

byte_t *
cds_pass_WholeEntry (const byte_t *ptr_p)
{
    register int cnt;

    if (cnt = *ptr_p++)
    {
	do 
	{
	    ptr_p = SKIP_AttributeName(ptr_p);
	    ptr_p = SKIP_AttributeContents(ptr_p);
	} while (--cnt);
    }
    
    return((byte_t *)ptr_p);
}


