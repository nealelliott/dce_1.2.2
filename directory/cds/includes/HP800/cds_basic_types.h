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
 * Revision 1.1.4.2  1996/02/18  23:33:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:39  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:13:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:49  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:38:22  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:41  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/12  21:11:29  peckham
 * 	Add threads paralell definitions:
 * 	  Machine page size _CDS_PAGE_SIZE_
 * 	  Threads default stack size _CDS_DEFAULT_STACK_
 * 	[1994/05/12  19:15:44  peckham]
 * 
 * Revision 1.1.2.1  1994/04/14  14:39:33  peckham
 * 	Split decnet_types into machine dependent cds_basic_types.h
 * 	Removed VMS, UNIX and DOSorOS2 conditionals.
 * 	[1994/04/11  19:56:58  peckham]
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
#if defined(__hppa)
# define _CDS_DEFAULT_STACK_		(_CDS_PAGE_SIZE_ * 15)
#else
# define _CDS_DEFAULT_STACK_		(_CDS_PAGE_SIZE_ * 8)
#endif

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


#if defined(__hp9000s700) || defined(__hp9000s800)

# define GET16(p)	\
        (field16)( *(((field8 *)(p))+1) | ((field16)*((field8 *)(p))) << 8 ); \
        (p) += sizeof(field16)
# define GET32(p)				\
          (field32)(*(((field8 *)(p))+3)        \
        | (*(((field8 *)(p))+2)<<8)     \
        | (*(((field8 *)(p))+1)<<16)    \
        | (*((field8 *)(p))<<24));      \
        (p) += sizeof(field32)
# define PUT16(p, v)	{ field16 tmp = (v); bcopy(&tmp, (p), 2); (p) += 2; }
# define PUT32(p, v)	{ field32 tmp = (v); bcopy(&tmp, (p), 4); (p) += 4; }
# define EXT8(p)	*(field8 *)(p)
# define EXT16(p)	(field16)(*(((field8 *)(p))+1) | (*(((field8 *)(p)))<<8))
# define EXT32(p)				\
          (field32)(*(((field8 *)(p))+3)        \
        | (*(((field8 *)(p))+2)<<8)     \
        | (*(((field8 *)(p))+1)<<16)    \
        | (*((field8 *)(p))<<24))
# define EXTRACT(b,t)	(*(t *)(b))
# define INS8(b,v)	(*(field8 *)(b) = (v))
# define INS16(p, v)	{ register field8 *q = (field8 *) (p); \
                          if ((int)q & 1) { \
                              register int w = (v); \
                              *(q+1) = w & 0xFF; w >>= 8; \
                              *q = w & 0xFF; \
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
                                  *(field16 *)(q+2) = w & 0xFFFF; w >>= 16; \
                                  *(field16 *)q = w & 0xFFFF; \
                                  break; \
                              default: \
                                  *(q+3) = w & 0xFF; w >>= 8; \
                                  *(field16 *)(q+1) = w & 0xFFFF; w >>= 16; \
                                  *q = w & 0xFF; \
                                  break; \
                          } \
                        }
# define INSERT(b,t,v)   { field32 tmp = (v); bcopy(&tmp, (b), sizeof(t)); }
#endif /* __hp9000s700 || __hp9000s800 */


#ifdef __hp9000s300
# define GET16(p)	*(field16 *)(p); (p) += sizeof(field16)
# define PUT16(p,v)	*(field16 *)(p) = (v); (p) += sizeof(field16)
# define GET32(p)	*(field32 *)(p); (p) += sizeof(field32)
# define PUT32(p,v)	*(field32 *)(p) = (v); (p) += sizeof(field32)
# define EXT8(b)	*(field8 *)(b))
# define EXT16(b)	*(field16 *)(b))
# define EXT32(b)	*(field32 *)(b))
# define EXTRACT(b,t)	(*(t *)(b))
# define INS8(b,v)	(*(field8 *)(b) = (v))
# define INS16(b,v)	(*(field16 *)(b) = (v))
# define INS32(b,v)	(*(field32 *)(b) = (v))
# define INSERT(b,t,v)	(*(t *)(b) = (v))
#endif /* __hp9000s300 */


/*
 * If 64 bits not needed
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
