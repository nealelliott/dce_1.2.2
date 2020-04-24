/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cds_uuid_lib.c,v $
 * Revision 1.1.8.2  1996/02/18  19:32:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:05  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:20:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:38  root]
 * 
 * Revision 1.1.6.3  1994/06/30  19:16:23  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:58:22  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:40:31  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:03  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:05:37  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:24:19  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:53:20  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:06:15  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:50:17  weisman
 * 		New file from Digital (post 1.0)
 * 	[1992/03/22  20:57:10  weisman]
 * 
 * $EndLog$
 */
/*
 * Module: cds_uuid_lib.c
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
#include <string.h>
#include <dce/uuid.h>
#include <dce/dns_record.h>

void 
cds_uuid_create_nil (uuid_t *uuid_p)
{
  unsigned32 status;
  uuid_t id;

  uuid_create_nil(&id, &status);
  memcpy((char *)uuid_p, (char *)&id, sizeof(uuid_t));
}

int 
cds_uuid_equal (uuid_t  *uuid1_p,
                uuid_t  *uuid2_p)
{
  unsigned32 status=0;
  uuid_t id1,id2;

  /* Align them */
  memcpy((char *)&id1, (char *)uuid1_p, sizeof(uuid_t));
  memcpy((char *)&id2, (char *)uuid2_p, sizeof(uuid_t));
  return(uuid_equal(&id1, &id2, &status));
}

unsigned int 
cds_uuid_compare (uuid_t  *uuid1_p,
                  uuid_t  *uuid2_p)
{
  unsigned32 status=0;
  uuid_t id1,id2;

  /* Align them */
  memcpy((char *)&id1, (char *)uuid1_p, sizeof(uuid_t));
  memcpy((char *)&id2, (char *)uuid2_p, sizeof(uuid_t));
  return(uuid_compare(&id1, &id2, &status));
}

int 
cds_uuid_is_nil (uuid_t *uuid_p)
{
  unsigned32 status; 
  uuid_t id;

  /* Align */
  memcpy((char *)&id, (char *)uuid_p, sizeof(uuid_t));
  return(uuid_is_nil(&id, &status)); 
}
