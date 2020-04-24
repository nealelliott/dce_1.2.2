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
 * Revision 1.1.4.2  1996/02/18  23:33:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:42  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:14:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:54  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:38:24  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:44  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/12  21:11:33  peckham
 * 	Add threads paralell definitions:
 * 	  Machine page size _CDS_PAGE_SIZE_
 * 	  Threads default stack size _CDS_DEFAULT_STACK_
 * 	[1994/05/12  19:15:52  peckham]
 * 
 * Revision 1.1.2.1  1994/04/14  14:39:37  peckham
 * 	Split decnet_types into machine dependent cds_basic_types.h
 * 	Removed VMS, UNIX and DOSorOS2 conditionals.
 * 	[1994/04/11  19:57:06  peckham]
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
#include <sys/param.h>

#define _CDS_PAGE_SIZE_			NBPG
#define _CDS_DEFAULT_STACK_		(_CDS_PAGE_SIZE_ * 8)

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

/*
 * Big-endian aligned processors
 */
# define EXT8(p)	  (* (field8 *)(p))
# define EXT16(p)	  (field16) ( \
			  (*((field8 *)(p)  )<< 8) \
			| (*((field8 *)(p)+1)    ))
# define EXT32(p) (field32) ( \
			  (*((field8 *)(p)  )<<24)	\
			| (*((field8 *)(p)+1)<<16)	\
			| (*((field8 *)(p)+2)<< 8)	\
			| (*((field8 *)(p)+3)    ))

# define GET16(p)	EXT16(p); (p) += sizeof(field16)
# define GET32(p)	EXT32(p); (p) += sizeof(field32)

# define PUT16(p, v)	{ field16 tmp = (v); \
			  field8 *tmp2 = (field8*) &tmp; \
			  *p++ = *tmp2++; \
			  *p++ = *tmp2; }
# define PUT32(p, v)	{ field32 tmp = (v); \
			  field8 *tmp2 = (field8*) &tmp; \
			  *p++ = *tmp2++; \
			  *p++ = *tmp2++; \
			  *p++ = *tmp2++; \
			  *p++ = *tmp2; }

# define INS8(p, v)	{ ((field8 *)(p))[0] = (v); }
# define INS16(p, v)	{ register field8 *q = (field8 *) (p); \
			  if ((int)q & 1) { \
			      register int w = (v); \
			      *q++ = (w & 0xFF00) >> 8; \
			      *q   = w & 0xFF; \
			  } else { \
			      *(field16 *)q = (v); \
			  } \
			}
# define INS32(p, v)	{ register field8 *q = (field8 *) (p); \
			  register int w = (v); \
			  switch ((int)q & 3) { \
			      case 0: \
				  *(field32 *)q = w; \
				  break; \
			      case 2: \
				  *(field16 *)q = (w & 0xFFFF0000) >>16; \
				  q += sizeof(field16); \
				  *(field16 *)q = w & 0xFFFF; \
				  break; \
			      default: \
				  *q++ = (w & 0xFF000000) >>24; \
				  *(field16 *)q = (w & 0x00FFFF00) >> 8; \
				  q += sizeof(field16); \
				  *q   = w & 0x000000FF; \
				  break; \
			  } \
			}


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
 * Also for time stamp manipulation, htink, 12/14/90
 * For VAX, MIPS and ALPHA they remain the same.
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
