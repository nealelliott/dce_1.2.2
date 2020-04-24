/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: cds_basic_types.h,v $
 * Revision 1.1.4.2  1996/02/18  23:33:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:13:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:48  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:38:21  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:39  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/12  21:11:28  peckham
 * 	Add threads paralell definitions:
 * 	  Machine page size _CDS_PAGE_SIZE_
 * 	  Threads default stack size _CDS_DEFAULT_STACK_
 * 	[1994/05/12  19:15:39  peckham]
 * 
 * Revision 1.1.2.1  1994/04/14  14:39:31  peckham
 * 	Split decnet_types into machine dependent cds_basic_types.h
 * 	Removed VMS, UNIX and DOSorOS2 conditionals.
 * 	[1994/04/11  19:56:54  peckham]
 * 
 * $EndLog$
 */
#ifndef _BASIC_TYPES_H
#define _BASIC_TYPES_H
/*
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
 * Networks & Communications Software Engineering
 *
 */

/*
 * Restore old thread default stack sizes
 */

#define _CDS_PAGE_SIZE_			512
#define _CDS_DEFAULT_STACK_		(_CDS_PAGE_SIZE_ * 60)

/*
 * Opaque protocol access definitions
 */

typedef unsigned char field8;		/* 8-bit message field */
typedef unsigned short field16;		/* 16-bit message field */
typedef unsigned int field32;		/* 32-bit message field */
typedef field32 field64[2];		/* 64-bit message field */

/*
 * Macros to access 8 and 16-bit fields within messages.
 */
#define GET8(p)		(*(field8 *)(p)++)
#define PUT8(p,v)	(*(field8 *)(p)++ = (v))

#define GET16(p)	*(field16 *)(p); (p) += sizeof(field16)
#define PUT16(p,v)	*(field16 *)(p) = (v); (p) += sizeof(field16)
#define GET32(p)	*(field32 *)(p); (p) += sizeof(field32)
#define PUT32(p,v)	*(field32 *)(p) = (v); (p) += sizeof(field32)
#define EXT8(b)		(*(field8 *)(b))
#define EXT16(b)	(*(field16 *)(b))
#define EXT32(b)	(*(field32 *)(b))
#define EXTRACT(b,t)	(*(t *)(b))
#define INS8(b,v)	(*(field8 *)(b) = (v))
#define INS16(b,v)	(*(field16 *)(b) = (v))
#define INS32(b,v)	(*(field32 *)(b) = (v))
#define INSERT(b,t,v)	(*(t *)(b) = (v))

/*
 * 64 bits not needed
 */
#define INS64(p,v) { abort(); }
#define PUT64(p,v) { abort(); }
#define EXT64(p) (-1)
#define GET64(p) (-1)


/*
 * Counter access macros
 */
#define INC32(c)	((c) != (unsigned) 0xffffffff ? ++(c) : (c))
#define INC16(c)	((c) != (unsigned) 0xffff ? ++(c) : (c))
#define INC8(c)		((c) != (unsigned) 0xff ? ++(c) : (c))
#define ADD32(c,a)	(((c)+(a)) >= (c) ? ((c) += (a)) : ((c) = (unsigned) 0xffffffff))
#define ADD16(c,a)	(((c)+(a)) >= (c) ? ((c) += (a)) : ((c) = (unsigned) 0xffff))
#define ADD8(c,a)	(((c)+(a)) >= (c) ? ((c) += (a)) : ((c) = (unsigned) 0xff))

typedef unsigned char word[2];		/* 2 byte field */
typedef unsigned char longword[4];	/* 4 bytes field */

/* 
 * Macros to extract values transported as little endian 
 * Used in tower manipulation
 */

#define EXTL8(p) EXT8(p)
#define GETL8(p) GET8(p)
#define INSL8(p,v) INS8(p,v)
#define PUTL8(p,v) PUT8(p,v)

#if BYTE_ORDER == LITTLE_ENDIAN

# define EXTL16(p) EXT16(p)
# define GETL16(p) GET16(p)
# define INSL16(p,v) INS16(p,v)
# define PUTL16(p,v) PUT16(p,v)
# define EXTL32(p) EXT32(p)
# define GETL32(p) GET32(p)
# define INSL32(p,v) INS32(p,v)
# define PUTL32(p,v) PUT32(p,v)

#else

# define _swapS(v)   	( (((v) >> 8) & 0xFF) | (((v) & 0xFF) << 8) )
# define _swapQ(v)   	( (((v) >> 24) & 0xFF) | (((v) & 0xFF) << 24) | \
			  (((v) >> 8) & 0xFF00) | (((v) & 0xFF00) << 8) )
# define EXTL16(p) _swapS(EXT16(p))
# define GETL16(p) EXTL16(p); (p) += sizeof(field16)
# define INSL16(p,v) INS16(p, _swapS(v))
# define PUTL16(p,v) INS16(p, _swapS(v)); (p) += sizeof(field16)
# define EXTL32(p) _swapQ(EXT32(p))
# define GETL32(p) EXTL32(p); (p) += sizeof(field32)
# define INSL32(p,v) INS32(p, _swapQ(v))
# define PUTL32(p,v) INS32(p, _swapQ(v)); (p) += sizeof(field32)

#endif

#endif  /* #ifndef _BASIC_TYPES_H */
