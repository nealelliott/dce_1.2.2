/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: attributes.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:56  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:14:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:07  root]
 * 
 * Revision 1.1.4.8  1994/08/16  18:27:50  jd
 * 	drop from janet
 * 	[1994/08/13  16:06:09  jd]
 * 
 * Revision 1.1.4.7  1994/08/03  19:01:55  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:55:29  mccann]
 * 
 * Revision 1.1.4.6  1994/06/09  18:38:29  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:54  devsrc]
 * 
 * Revision 1.1.4.5  1994/05/06  16:04:02  zee
 * 	     HCell BL4 support: Add CDS_ParentCellPointers attribute.
 * 	[1994/05/05  21:03:51  zee]
 * 
 * Revision 1.1.4.4  1994/04/14  14:39:41  peckham
 * 	Split decnet_types into machine dependent cds_basic_types.h
 * 	Removed VMS, UNIX and DOSorOS2 conditionals.
 * 	[1994/04/11  19:57:14  peckham]
 * 
 * Revision 1.1.4.3  1994/04/07  10:22:17  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:18:01  kevins]
 * 
 * Revision 1.1.4.2  1994/03/22  19:59:58  griffin
 * 	HCell BL3 support: Add CDS_CellAliases attribute
 * 	[1994/03/22  17:48:46  griffin]
 * 
 * Revision 1.1.4.1  1994/03/12  22:02:05  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:11:21  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:28:07  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:45:19  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _ATTRIBUTES_H
#define _ATTRIBUTES_H
/*
 * Module attributes.h
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
 * Modifications:
 *
 *   9-MAR-1991 14:09  by Dave Griffin
 *      Fixed non-RPC constant ordering (was not alphabetical).
 *
 * Gen 22, 18-Jun-1991, Alan Peckham
 *	Add CDS_NSCellname.
 *
 * Gen 24, 2-Jul-1991, Pete Donahue
 *	Use DNS_EXTERN with v1v2 (VMS only).
 *
 * Gen 27, 22-Jul-1991, Linda Ferner Roberge
 *	Use DNS_EXTERN for cds_classlist and cds_attlist.
 *
 * Gen 29, 29-Aug-1991, Alan Peckham
 *	Add DNS_GDAPOINTERS for CDS.
 *
 * Gen 31, 27-Feb-1992, LR
 *	Add DNA_NODESYNONYM
 *
 * Gen 32, Oct 15 15:10:23, by Tamar R. Wexler
 *	Added DNS_SKULKSTATUS.
 *
 * Gen 33, Nov  6 16:45:44, by TRW
 *	Ok, now define dns_skulkstatus for CDS...
 *
 * Gen 34, 18-Jan-1992, by TRW
 *	Since adding DNS$SKULKSTATUS to CDS would require protocol changes and
 *	that is impossible for this version of CDS, pull all references to
 *	that attribute in CDS.
 */



#include <dce/dns_record.h>

/*
 * attribute set name datastructures and constants (does allocate space)
 * must be consistant with attribute_lib.c table ordering
 */

/* Object id order */
#define DNS_MEMBER              0  /* 1.3.22.1.3.10 */
#define DNS_GROUPREVOKE         1  /* 1.3.22.1.3.11 */
#define DNS_CTS			2  /* 1.3.22.1.3.12 */
#define DNS_UTS			3  /* 1.3.22.1.3.13 */
#define DNS_CLASS		4  /* 1.3.22.1.3.15 */
#define DNS_CLASSVERSION	5  /* 1.3.22.1.3.16 */
#define DNS_OBJECTUID           6  /* 1.3.22.1.3.17 */
#define DNS_REPLICAS		7  /* 1.3.22.1.3.19 */
#define DNS_ALLUPTO		8 /* 1.3.22.1.3.20 */
#define DNS_CONVERGENCE		9 /* 1.3.22.1.3.21 */
#define DNS_INCHNAME		10 /* 1.3.22.1.3.22 */
#define DNS_PARENTPOINTER	11 /* 1.3.22.1.3.23 */
#define DNS_DIRECTORYVERSION	12 /* 1.3.22.1.3.24 */
#define DNS_UPGRADETO		13 /* 1.3.22.1.3.25 */
#define DNS_LINKTARGET		14 /* 1.3.22.1.3.27 */
#define DNS_LINKTIMEOUT		15 /* 1.3.22.1.3.28 */
#define DNA_TOWERS		16 /* 1.3.22.1.3.30 */
#define DNS_CHNAME              17 /* 1.3.22.1.3.32 */
#define DNS_CHLASTADDRESS       18 /* 1.3.22.1.3.34 */
#define DNS_CHSTATE             19 /* 1.3.22.1.3.36 */
#define DNS_CHDIRECTORIES       20 /* 1.3.22.1.3.37 */
#define DNS_REPLICASTATE        21 /* 1.3.22.1.3.40 */
#define DNS_REPLICATYPE         22 /* 1.3.22.1.3.41 */
#define DNS_LASTSKULK           23 /* 1.3.22.1.3.42 */
#define DNS_LASTUPDATE          24 /* 1.3.22.1.3.43 */
#define DNS_RINGPOINTER         25 /* 1.3.22.1.3.44 */
#define DNS_EPOCH               26 /* 1.3.22.1.3.45 */
#define DNS_REPLICAVERSION      27 /* 1.3.22.1.3.46 */
#define DNS_NSCELLNAME          28 /* 1.3.22.1.3.48 */
#define DNS_OBJECTACL           29 /* 1.3.22.1.3.49 */
#define DNS_DEFOBJECTACL        30 /* 1.3.22.1.3.50 */
#define DNS_DEFCONTAINERACL     31 /* 1.3.22.1.3.51 */
#define DNS_GDAPOINTERS		32 /* 1.3.22.1.3.52 */
#define DNS_CELLALIASES         33 /* 1.3.22.1.3.53 */
#define DNS_PARENTCELLPOINTERS  34 /* 1.3.22.1.3.54 */
#define DNS_MAXATTRIBUTES	35 /* must = last one + 1*/

/* class names */
#define DNS_CLEARINGHOUSE	0
#define DNS_GROUP		1

/* external declarations to point at real tables - must be kept */
/* consistant with the attribute_table.h file */

typedef struct {
    byte_t	*v1;
    byte_t	*v2;
} v1v2_t;

extern v1v2_t v1v2[];

typedef struct class_descriptor
    {
    ClassName_u		cname;
    } class_descriptor_t;


extern class_descriptor_t cds_classlist[];

typedef struct attribute_descriptor
    {
    AttributeName_u	text;
    int			atype;
    unsigned int	casemask;
    unsigned char	type;		/* set_at, single_at */
    unsigned char       vtype;          /* Type of value */
    unsigned char       visable;        /* Is this attribute visable */
    unsigned char	delete;		/* can this attribute be deleted? */
    unsigned		dir_rec : 1;	/* type of records having this attrib */
    unsigned		child_rec : 1;
    unsigned		obj_rec : 1;
    unsigned		link_rec : 1;
    unsigned		ch_rec : 1;
    unsigned		rep_rec : 1;
    unsigned		change : 1 ;	/* Modifyable by non-nameserver */
    unsigned		namecheck : 1;	/* Flag if case insensitive value */
    unsigned short	min;
    unsigned short	max;
    AttributeName_u	aname;
    } attribute_descriptor_t;

extern attribute_descriptor_t cds_attlist[];

#define dns_Attribute(idx) (&cds_attlist[idx])
#define dns_AttributeName(idx) ((AttributeName_t *)cds_attlist[idx].aname)
#define dns_AttributeMask(idx) \
    ((AttributeNameMask_t *)&cds_attlist[idx].casemask)

#endif  /* #ifndef _ATTRIBUTES_H */
