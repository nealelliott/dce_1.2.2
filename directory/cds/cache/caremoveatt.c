/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: caremoveatt.c,v $
 * Revision 1.1.9.2  1996/02/18  19:24:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:18  marty]
 *
 * Revision 1.1.9.1  1995/12/08  00:10:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:41  root]
 * 
 * Revision 1.1.6.5  1994/08/16  17:27:17  peckham
 * 	CR#11640: potential cache corruption and component crashes
 * 	due to bad fix for CR#10570.
 * 	Replace CALockClient() by CALockData(),
 * 	CAUnlockClient() by CAUnlockData().
 * 	[1994/08/16  15:30:40  peckham]
 * 
 * Revision 1.1.6.4  1994/06/23  18:28:32  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:46:20  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  16:07:16  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:51  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/14  14:38:07  peckham
 * 	Remove VMS and UNIX conditionals.
 * 	[1994/04/11  18:00:20  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  21:55:59  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:48:38  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:37:28  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:20:50  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:15:18  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:06:05  weisman]
 * 
 * Revision 1.1  1992/01/19  15:24:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: caremoveatt.c
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
#include <sys/time.h>
#include <attributes.h>
#include <dnsclerk.h>

/*
 * Remove an attribute from the cache.  This routine is called
 * after a successful modify attribute to invalidate the cache.
 */
void 
CARemoveAttribute (FullName_t       *entryname_p,
                   EntryType_t      *type_p,
                   AttributeName_t  *attname_p)
{
  DEB_ASCII_BUF_FullName(entrynameBuf)
  DEB_ASCII_BUF_EntryType(typeBuf)
  DEB_ASCII_BUF_AttributeName(attnameBuf)
  NameEntry_t *name_p;

  register AttrEntry_t *att_p;
  AttributeName_u attribute;
  register ValueEntry_t *val_p;
  FullName_u ename;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CARemoveAttribute(%s,%s,%s)",
      deb_ascii_FullName(entrynameBuf,entryname_p),
      deb_ascii_EntryType(typeBuf,type_p),
      deb_ascii_AttributeName(attnameBuf,attname_p)));

  /* Convert names to upper case */
  if (dns_normalize_fname(entryname_p, (FullName_t *)ename, sizeof(FullName_u))
	== DNS_SUCCESS) {
    CALockData();

    /* Locate entry in Name Hash */  
    name_p = CA_LookupName((FullName_t *)ename, type_p);
    if (name_p) {
	dns_attname_upper(attname_p, attribute);
	
	/* Get attribute in shared cache */
	att_p = CA_LookupAttr((AttributeName_t *)attribute);
	if (att_p)
	    for (val_p = CA_GET_AVPOINTER(name_p->nh_attributes.ll_flink);
		 val_p != (ValueEntry_t *)&name_p->nh_attributes;
		 val_p = CA_GET_AVPOINTER(val_p->av_collision.ll_flink))
		if (EQ_token(att_p->ah_token, val_p->av_token)) {

		    /*
		     * If name is locked down, release att_p
		     */
		    if (CA_locked(name_p)) CA_unlock(att_p);

		    /* Remove it from lists */
		    CA_CollisionRemove((CacheLinkedList_t *)val_p);
		    CA_LruRemove(val_p);

		    /* Add to free list */
		    CA_FreeListFree(val_p);
		    break;
		}
    }
    /* If modifying linktarget, remove from linkcache */
    if (EXT8(type_p->et_value) == ET_softlink)
      if (dnsCmpSimple((dns_opq_sname *)attname_p, 
	       (dns_opq_sname *)dns_AttributeName(DNS_LINKTARGET)) == 0)
	/* Remove entry from softlink cache */
	(void)CA_RemoveLink(entryname_p);
    
    CAUnlockData();
    
    }
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CARemoveAttribute(%s,%s,%s)",
      deb_ascii_FullName(entrynameBuf,entryname_p),
      deb_ascii_EntryType(typeBuf,type_p),
      deb_ascii_AttributeName(attnameBuf,attname_p)));
}
