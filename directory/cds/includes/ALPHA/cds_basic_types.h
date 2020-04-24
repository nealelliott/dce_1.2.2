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
 * Revision 1.1.4.2  1996/02/18  23:33:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:13:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:47  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:38:20  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:38  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/12  21:11:25  peckham
 * 	Add threads paralell definitions:
 * 	  Machine page size _CDS_PAGE_SIZE_
 * 	  Threads default stack size _CDS_DEFAULT_STACK_
 * 	[1994/05/12  19:15:35  peckham]
 * 
 * Revision 1.1.2.1  1994/04/14  14:39:29  peckham
 * 	Split decnet_types into machine dependent cds_basic_types.h
 * 	Removed VMS, UNIX and DOSorOS2 conditionals.
 * 	[1994/04/11  19:56:50  peckham]
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
typedef unsigned long int field64;	/* 64-bit message field */

/*
 * Macros to access 8 and 16-bit fields within messages.
 */
#define GET8(p)		(*(field8 *)(p)++)
#define PUT8(p,v)	(*(field8 *)(p)++ = (v))

/*
 * Little-endian aligned processors
 */
#define GET16(p)	\
	(field16)( *(field8 *)(p) | ((field16)*((field8 *)(p)+1)) << 8 ); \
	(p) += sizeof(field16)
#define GET32(p)			\
	  (field32)(*((field8 *)(p))	\
	| (*(((field8 *)(p))+1)<<8)	\
	| (((int)*(((field8 *)(p))+2))<<16)	\
	| (((int)*(((field8 *)(p))+3))<<24));	\
	(p) += sizeof(field32)
#define PUT16(p, v)	{ field16 tmp = (v); memmove((p), &tmp, 2); (p) += 2; }
#define PUT16S(p, v)	{ short tmp = (v); memmove((p), &tmp, 2); (p) += 2; }
#define PUT32(p, v)	{ field32 tmp = (v); memmove((p), &tmp, 4); (p) += 4; }
#define INSERT(b,t,v)	{ field32 tmp = (v); memmove((b), &tmp, sizeof(t)); }
#define INSERT16(b,v)	{ field16 tmp = (v); memmove((b), &tmp, sizeof(field16)); }
#define INSERT32(b,v)	{ field32 tmp = (v); memmove((b), &tmp, sizeof(field32)); }

#define INS8(p, v)	{ ((field8 *)(p))[0] = (v); }
#define INS16(p, v)	{ register field8 *q = (field8 *) (p); \
			  if ((int)q & 1) { \
			      register int w = (v); \
			      *q++ = w & 0xFF; w >>= 8; \
			      *q   = w & 0xFF; \
			  } else { \
			      *(field16 *)q = (v); \
			  } \
			}
#define INS32(p, v)	{ register field8 *q = (field8 *) (p); \
			  register int w = (v); \
			  switch ((int)q & 3) { \
			      case 0: \
				  *(field32 *)q = w; \
				  break; \
			      case 2: \
				  *(field16 *)q = w & 0xFFFF; w >>= 16; q += 2; \
				  *(field16 *)q = w & 0xFFFF; \
				  break; \
			      default: \
				  *q++ = w & 0xFF; w >>= 8; \
				  *(field16 *)q = w & 0xFFFF; w >>= 16; q += 2; \
				  *q   = w & 0xFF; \
				  break; \
			  } \
			}
#define EXT8(p)		*(field8 *)(p)
#define EXT16(p)	(field16)(*((field8 *)(p)) | (*(((field8 *)(p))+1)<<8))
#define EXT32(p)			\
	  (field32)(*((field8 *)(p))	\
	| (*(((field8 *)(p))+1)<<8)	\
	| (((int)*(((field8 *)(p))+2))<<16)	\
	| (((int)*(((field8 *)(p))+3))<<24))
#define INS64(p,v) { field64 val = (field64)(v); \
	((field8 *)(p))[0] = (val >>  0) & 0xFF; \
	((field8 *)(p))[1] = (val >>  8) & 0xFF; \
	((field8 *)(p))[2] = (val >> 16) & 0xFF; \
	((field8 *)(p))[3] = (val >> 24) & 0xFF; \
	((field8 *)(p))[4] = (val >> 32) & 0xFF; \
	((field8 *)(p))[5] = (val >> 40) & 0xFF; \
	((field8 *)(p))[6] = (val >> 48) & 0xFF; \
	((field8 *)(p))[7] = (val >> 56) & 0xFF; }
#define PUT64(p,v) { field64 val = (field64)(v); p += 8; \
	((field8 *)(p))[-8] = (val >>  0) & 0xFF; \
	((field8 *)(p))[-7] = (val >>  8) & 0xFF; \
	((field8 *)(p))[-6] = (val >> 16) & 0xFF; \
	((field8 *)(p))[-5] = (val >> 24) & 0xFF; \
	((field8 *)(p))[-4] = (val >> 32) & 0xFF; \
	((field8 *)(p))[-3] = (val >> 40) & 0xFF; \
	((field8 *)(p))[-2] = (val >> 48) & 0xFF; \
	((field8 *)(p))[-1] = (val >> 56) & 0xFF; }
#define EXT64(p) (field64) ( \
	  (field64)((field8 *)(p))[0] <<  0 \
	| (field64)((field8 *)(p))[1] <<  8 \
	| (field64)((field8 *)(p))[2] << 16 \
	| (field64)((field8 *)(p))[3] << 24 \
	| (field64)((field8 *)(p))[4] << 32 \
	| (field64)((field8 *)(p))[5] << 40 \
	| (field64)((field8 *)(p))[6] << 48 \
	| (field64)((field8 *)(p))[7] << 56 )
#define GET64(p) (field64) ( p += 8, \
	  (field64)((field8 *)(p))[-8] <<  0 \
	| (field64)((field8 *)(p))[-7] <<  8 \
	| (field64)((field8 *)(p))[-6] << 16 \
	| (field64)((field8 *)(p))[-5] << 24 \
	| (field64)((field8 *)(p))[-4] << 32 \
	| (field64)((field8 *)(p))[-3] << 40 \
	| (field64)((field8 *)(p))[-2] << 48 \
	| (field64)((field8 *)(p))[-1] << 56 )


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
