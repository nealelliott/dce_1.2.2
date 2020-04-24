/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dns_ncl_defs.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:21  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:15:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:54  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:15  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:57:45  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:38:56  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:29  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:02:48  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:13:55  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:29:47  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:47:48  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _DNS_NCL_DEFS_H
#define _DNS_NCL_DEFS_H
/*
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
 
#define DNS_ACCESS_DENIED 200           /* no privs                         */
#define DNS_NOT_SUPPORTED 202           /* unsupported                      */
#define DNS_INV_ARG 204                 /* invalid argument value           */
#define DNS_INV_STATE 206               /* wrong state                      */
#define DNS_NOSUCH_ATTR 208             /* non-existent atribute            */
#define DNS_FAILURE 210                 /* process failure                  */
#define DNS_REQ_ARG 212                 /* missing a required argument      */
#define DNS_ENTITY_NOEXIST 214          /* no such entity                   */
#define DNS_INSUFF_RESOURCES 216        /* insufficient resources           */
#define DNS_SOLICIT_FAILURE 218         /* WAN solicit failed               */
#define DNS_NET_CONN_FAILURE 220        /* Network connect failed           */
#define DNS_NET_IO_FAILURE 222          /* Network read or write failed     */
#define DNS_READONLY_ATTR 224           /* Attribute is read-only           */
#define DNS_NO_SUCH_VALUE 226           /* Value out of range               */
#define DNS_SOLICIT_NO_NEW_CH 228       /* Solicit OK but no new ch cached  */
/*                                                                          */
/* SET HEADER declaration - for attributes of data type 'SET of foo'        */
/*                                                                          */
#define NCL_INFO_HEADER_OVERHEAD 4
struct ncl_info_header {
    unsigned long int dns_count;        /* Number of items in the set       */
    } ;
/*                                                                          */
/* SET ELEMENT structure                                                    */
/*                                                                          */
#define NCL_SET_ELEMENT_OVERHEAD 4
struct ncl_set_element {
    unsigned long int dns_size;         /* Length of set item               */
    } ;
/*                                                                          */
/* Transfer buffer structure                                                */
/*                                                                          */
#define MAX_NCLTRANSBUF_DATA 1536
#define NCLTRANSBUF_LENGTH 1540         /* Structure size                   */
struct NclTransBuf {
    unsigned short int transbuf_w_size; /* The number of bytes of data      */
    unsigned short int transbuf_w_type; /* Data type                        */
    char transbuf_t_data [1536];        /* The data block                   */
    } ;
/*                                                                          */
/* NCL attribute type definitions                                           */
/*                                                                          */
#define DNS_TYPE_BINABSTIME 17
#define DNS_TYPE_BINRELTIME 19
#define DNS_TYPE_BOOLEAN 1
#define DNS_TYPE_COUNTER64 121
#define DNS_TYPE_DNA_UID 28
#define DNS_TYPE_ENUMERATION 201
#define DNS_TYPE_FileSpec 36
#define DNS_TYPE_FULLNAME 27
#define DNS_TYPE_ID802 34
#define DNS_TYPE_INTEGER32 111
#define DNS_TYPE_NSCTS 52
#define DNS_TYPE_SET 8
#define DNS_TYPE_SIMPLENAME 26
#define DNS_TYPE_TOWERSET 44
#define DNS_TYPE_UNSIGNED32 116
#define DNS_TYPE_VERSION 33
#define DNS_TYPE_OCTETSTRING 9
#define DNS_TYPE_ENDUSERSPEC 45
/*                                                                          */
#endif  /* #ifndef _DNS_NCL_DEFS_H */




