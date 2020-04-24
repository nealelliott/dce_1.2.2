/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ncl_environment.h,v $
 * Revision 1.1.8.2  1996/02/18  23:34:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:05  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:17:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:30  root]
 * 
 * Revision 1.1.6.3  1994/08/03  19:02:32  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:59:21  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:39:21  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:00  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:03:53  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:17:59  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:32:59  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:52:27  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/29  15:36:15  m_sawyer
 * 	remove unused #define's
 * 	[1992/04/29  15:35:29  m_sawyer]
 * 
 * Revision 1.1  1992/01/19  15:15:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _NCL_ENVIRONMENT_H
#define _NCL_ENVIRONMENT_H
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
 */

#ifndef nclenvironment
#define nclenvironment 1

#ifndef FILE
#include <stdio.h>
#endif

#ifndef MS_EXACT
#include <msgblk.h>
#endif

#ifndef DNS_CTS_LEN_MAX
#include <dce/dnsclerk.h>
#endif

#include <dce/dns_record.h>

#define MAX_MSG_KEY	47		/* longest key offered to msg utility */
#define MAX_OP_LINE	9999		/* longest line offered to op routines*/

#define UIDOPON		1		/* uid included in output messages */
#define UIDOPOFF	0		/* no uid line in output messages */

#define MSGFILENG	73		/* can not use msg file */

#define ENVIRONMENT_BLOCK   531
#define LOGGING_BLOCK	533
#define MESSAGE_BLOCK	534
#define WITH_BLOCK      535
#define ATTRIBUTE_NAME_BLOCK 536
#define ATOMIC_VALUE_BLOCK   537
#define SINGLE_VALUE_BLOCK   538
#define ENTITY_BLOCK	539

/* codes for the operation that appears in first line of reference header. */
/* all except the first and last are valid. */
/* if additions are needed, change OPLINENGHIGH and */
/* insert them in front of it. */
/* the set must be dense, from 0 to OPLINENGHIGH. */
/* the value 0 must be an error */
#define OPLINENGLOW  00
#define OPLINESET    01
#define OPLINESHOW   02
#define OPLINEADD    03
#define OPLINEREM    04
#define OPLINEENAB   05
#define OPLINEDISAB  06
#define OPLINEDO     07
#define OPLINEREG     8
#define OPLINEDEREG   9
#define OPLINEMOD    10
#define OPLINERD     11
#define OPLINEDIR    12
#define OPLINEDEF    13
#define OPLINEUNDEF  14
#define OPLINENGHIGH 15

/* codes for the kind of entity reported on the reference header entity line */
#define refhdr_min_valid_ent dnsDir
#define refhdr_dirent dnsDir
#define refhdr_objent dnsObj
#define refhdr_kident dnsChildPtr
#define refhdr_slent  dnsSLink
#define refhdr_clhent dnsCH
#define refhdr_nsent  6
#define refhdr_grpent 7
#define dnsRep        8
#define dnsMasterRep  9
#define dnsSecondRep 10
#define dnsReadRep   11
#define dnsExcludeRep 12
#define fileentity    13
#define cellentity    14
#define recurse       15  /* For optional 'recurse' keyword */
#define acl           16  /* for excluding DACLS            */
#define refhdr_max_valid_ent 16
/* it is safe to assume that 0 will never be used. */
/* we can not simply extend the list in dnsdef.h since that list is under */
/*	architectual control. */

struct refhdropkey
    {
    int unused_index;
    char *key;
    };

#define FMTD_TIME_LEN 21

#define MAGIC_YEAR 57

struct tout				/* time, not portable  */
    {
    int x1;
    int x2;
    };

struct toutblk				/* not portable */
    {
    struct tout exp;			/* expiration time */
    struct tout ext;			/* extension time */
    };

/* see DNA Phase V Unique Identifier Functional Specification. */
/* this was made by using version x1_0_4, 15 February 1989. */
/* all versions ov the spec are usually available in : */
/* files::net$arch:[approved] or [review] */

struct dnaUid
    {
    unsigned	timeLow:32;
    unsigned	timeMid:16;
    unsigned	timeHiAndVersion:16;
    unsigned	clockSeqHiAndReserved:8;
    unsigned	clockSeqLow:8;
    unsigned char node[6];
    };

struct entblk
    {
    int			kind_of_structure_code;
    int			version;
    struct entblk	*nxt;
    struct entblk	*prv;
    struct entblk	*head;
    int			blk_alloc;	/* true if block in allocated storage */
    int			uid_present;	/* true if uid stored next */
    struct dnaUid	entuid;		/* uid if known */
    int			cts_present;	/* true if creation time stamp next */
    Timestamp_t		cts;		/* creation time stamp if known */
    int			kind_of_entity;
    int			name_len;	/* length of entity name */
    unsigned char	*name_p;	/* ptr to opaque full entity name */
    int			name_alloc;	/* true if name in allocated storage */
    int			wild;		/* true if wildcard in name */
    dns_opq_sname       wildname;       /* filled if wildcard supplied */
    int			strname_present;/* true if string form available */
    int			strname_alloc;	/* true if string in malloced memory */
    unsigned char	*strname_p;	/* ptr to string full name if present */
    };


struct envblk
    {
    int			kind_of_structure_code;
    int			version;
    struct entblk	*defent;	/* default entity */
    struct accblk	*defacc;	/* default access */
    struct entblk	*defnsn;	/* default namespace */
    struct logblk	*log;		/* logging information */
    struct cmdblk	*initcmd;	/* the initial command block */
    struct cmdblk	*curcmd;	/* the current command block */
    int			curconf;	/* the current confidence */
    int			curtout;	/* the current timeout */
    int			uidopt;		/* include uid in msgs or not */
    int			ts_opt;		/* how to display timestamps */
    int			nick_opt;	/* how to display nicknames */
    };

struct singlevalueblk
    {
    int			kind_of_structure_code;
    int			version;
    int			blkalloc;	/* true if this blk allocated */
    int			valalloc;	/* true if value allocated */
    AtomicValue_t       *vp;	        /* ptr to value, or null */
    };

struct attrblk
    {
    int			kind_of_structure_code;
    int			version;
    struct attrblk	*fwd;		/* ptr to next block */
    struct attrblk	*bck;		/* ptr to prev block */
    struct attrblk	*head;		/* ptr to head block */
    int			blkalloc;	/* true if this block malloced */
    int			grpattr;	/* group attribute kind or indiv */
    dns_opq_sname_t     bigattr;	/* attr name of max length */
    int			relop;		/* the relational operator */
    int			cmprules;	/* how to compare the values */
    struct singlevalueblk *av;		/* the single value or 0 */
    };
/* later change the pointer to a single value to be a ptr to a value blk. */
/* have value block be set or single. set might be list of singles */

#ifdef vms

#define full_item(item, code, size, addr, ret_len) \
	item.dns$w_itm_code = code; \
	item.dns$w_itm_size = size; \
	item.dns$a_itm_address = addr; \
	item.dns$a_itm_ret_length = ret_len;

#define part_item(item, code, size, addr) \
	item.dns$w_itm_code = code; \
	item.dns$w_itm_size = size; \
	item.dns$a_itm_address = addr;

#endif /* vms */

#endif /* nclenvironment */
#endif  /* #ifndef _NCL_ENVIRONMENT_H */
