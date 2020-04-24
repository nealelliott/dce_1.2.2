/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dns_record.h,v $
 * Revision 1.1.14.2  1996/02/18  23:33:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:23  marty]
 *
 * Revision 1.1.14.1  1995/12/08  15:15:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:57  root]
 * 
 * 	More code cleanup
 * 
 * Revision 1.1.11.9  1994/06/30  19:15:37  mccann
 * 	sams cleanup drop 2
 * 	[1994/06/30  18:01:30  mccann]
 * 
 * Revision 1.1.11.8  1994/06/09  18:38:57  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:35  devsrc]
 * 
 * Revision 1.1.11.7  1994/05/12  21:11:50  peckham
 * 	Remove NBPG definitions (moved to cds_basic_types.h).
 * 	[1994/05/12  19:16:35  peckham]
 * 
 * Revision 1.1.11.6  1994/04/29  15:50:15  peckham
 * 	Remove unnecessary conditionals.
 * 	string_lib goes away!
 * 	[1994/04/29  13:57:21  peckham]
 * 
 * Revision 1.1.11.5  1994/04/19  17:06:12  mccann
 * 	Merge and try again
 * 	[1994/04/19  16:50:04  mccann]
 * 
 * Revision 1.1.11.4  1994/04/14  14:39:47  peckham
 * 	Split decnet_types into machine dependent cds_basic_types.h
 * 	Removed VMS, UNIX and DOSorOS2 conditionals.
 * 	[1994/04/11  19:57:46  peckham]
 * 
 * Revision 1.1.11.3  1994/04/07  10:22:22  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:18:07  kevins]
 * 
 * Revision 1.1.11.2  1994/03/12  22:02:51  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:14:11  peckham]
 * 
 * Revision 1.1.11.1  1994/02/02  21:02:29  rousseau
 * 	Prefixed include of cds_types.h with dce/ for CR 9836.
 * 	[1994/02/02  21:02:11  rousseau]
 * 
 * Revision 1.1.6.5  1992/12/30  13:29:55  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:48:02  zeliff]
 * 
 * Revision 1.1.6.4  1992/12/18  00:29:28  jd
 * 	    Alan Peckham, 17-Dec-1992
 * 	    OT#3523: Invalid attributes shown for entries
 * 	    This problem is many-fold. The initial architecture says
 * 	    that unknown attributes will return as a Set with the
 * 	    "present" flag off. 1) the handle routines don't
 * 	    distinguish this case. 2) There is no error code for this.
 * 	    3) during the conversion between internal and rpc formats
 * 	    the "present" flag is not maintained, always being reconstituted
 * 	    as being "on".
 * 	    Fix this by creating a new error PDU_UnknownAttribute and passing
 * 	    this back as an exception instead of the NullSet.
 * 	[1992/12/18  00:19:52  jd]
 * 
 * Revision 1.1.6.3  1992/10/13  18:08:03  marty
 * 	OSC1.1.1 upgrade.  Changes are osc1.0.4 compatible
 * 	[1992/10/13  17:05:53  marty]
 * 
 * Revision 1.1.7.2  1992/09/30  19:54:17  garyf
 * 	Merge up to latest (post SVR4) DCE1_0_2.
 * 	[1992/09/30  16:21:41  garyf]
 * 
 * Revision 1.1.6.2  1992/09/29  19:14:18  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:12:57  weisman]
 * 	Revision 1.1.3.2  1992/09/29  20:02:30  garyf
 * 	conditionalize out bcopy prototype for OSF1
 * 
 * Revision 1.1.2.4  1992/06/02  21:07:36  grober
 * 	Changed the ifdef __hpux so that it was not inside of a macro.
 * 	AIX does not like this.
 * 	[1992/06/02  20:17:11  grober]
 * 
 * Revision 1.1.2.3  1992/06/01  14:20:36  grober
 * 	Added hpux specific changes inside of #ifdef __hpux .
 * 	[1992/05/27  15:28:39  grober]
 * 
 * Revision 1.1.2.2  1992/03/22  22:03:30  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:26:11  weisman]
 * 
 * Revision 1.1  1992/01/19  15:17:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _DNS_RECORD_H
#define _DNS_RECORD_H
/*
 * Header dns_record.h
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
 *
 * NOTE:
 *	Changes in this file may require changes in
 *	dnsclerk.h, dnsclerk_cds.h, and cdsclerk.h
 */

#include <dce/dnsclerk_cds.h>
#include <dce/deb_ascii.h>

/*
 * Until all structures are being handled...
 */
char *
deb_ascii_rec (
    char *const       ,
    const void *const ,
    const char *const );
#define DEB_ASCII_LEN_rec DEB_ASCII_LEN_ptr
#define DEB_ASCII_BUF_rec(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_rec)
#define LOG_ASCII_BUF_rec(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_rec)

#define EllipsisSimpleName      cds_EllipsisSimpleName
#define NullAttribute           cds_NullAttribute
#define NullBinarySimpleName    cds_NullBinarySimpleName
#define NullFullName            cds_NullFullName
#define NullNode                cds_NullNode
#define NullSet                 cds_NullSet
#define NullSimpleName          cds_NullSimpleName
#define NullTime                cds_NullTime
#define NullTimeout             cds_NullTimeout
#define NullTimestamp           cds_NullTimestamp
#define Nullutc                 cds_Nullutc

#ifndef DIFF_bytes
# define DIFF_bytes(top,bot) (((byte_t *)(top)) - ((byte_t *)(bot)))
#endif

# ifndef VT_none
#  include <dce/cds_types.h>
# endif
# include <dce/utc.h>
# include <dce/uuid.h>
/*
 * The previous include file ends with two extern definitions.
 * If followed by a typedef, the mips compiler loses track of some
 * dbdef.h structure definitions and starts complaining
 * half-way into the server db_btree module. VERY obscure!
 */
struct dns_avoid_mysterious_compiler_bug {int shows_up_in_db_btree;};
/* PhaseV only */
#define VT_ACE              17

#ifndef	PUT8
# include <dce/cds_basic_types.h>
#endif	/* PUT8 */

#define DNS_CONST
#define DNS_GLOBAL
#define DNS_EXTERN extern

/*
 *
 */
#if (K_64_BITS <= LONG_BIT)
  typedef unsigned long mgmt_counter_t;
# define INC_MGMT_COUNTER(adr_p) {++(*(adr_p));}
# define EXPORT_MGMT_COUNTER(adr_p, av_p) \
    { (av_p)->av_valuetype = CP_counter64; \
      (av_p)->av_val.av_counter64 = *(adr_p); }
#elif (K_32_BITS <= LONG_BIT)
  typedef struct {
    unsigned long lo, hi;
  } mgmt_counter_t;
# define INC_MGMT_COUNTER(adr_p) {if (++(adr_p)->lo == 0) ++(adr_p)->hi; }
# define EXPORT_MGMT_COUNTER(adr_p, av_p) \
    { (av_p)->av_valuetype = CP_counter64; \
      (av_p)->av_val.av_counter64.low = (adr_p)->lo; \
      (av_p)->av_val.av_counter64.high = (adr_p)->hi; }
#else
    !!! Counters need at least 32 bit longs
#endif

/*
 *
 *    A R C H I T E C T U R A L   C O N S T A N T S
 *
 * Certain constants conflict with struct.h -
 * make conditional until struct.h goes away.
 */

#define SIMPLENAMEMAX		255
#define FULLNAMEMAX		402
#define ATTRIBNAMEMAX		31
#define MAXATTRIBUTE		4000
#define CLASSSIZE		31
#define UPDATEPACKETMAX		(16*1024)

# define MAJORVER		2	/* Major version of this software */
# define MINORVER		0	/* Minor version of this software */
# define MINVERSION              1       /* Minimum version supported */
# define MAXVERSION              2       /* Maximum version supported */
#define ECOVER			0	/* ECO version of this software */

#define MAXDATAGRAMSIZE		1400
#define MINDATAGRAMSIZE		256
#define NSMAXSIZE		(16*1024)

# define READ_BUF_SIZE           MAXATTRIBUTE+1024 /* internal read buffers */

#define NETWORKADDRESSMAX	200

/*
 * Identifiers:
 *
 *  Structure size is the maximum size the structure can take on.
 *   Because of variable sizes, NOT ALL ELEMENT OFFSETS ARE USEABLE.
 *
 *  The typedef <Name>_t is to be used for pointers and memory allocation,
 *   and allows access to the field names of the structure.
 *
 *  The typedef <Name>_u is to be used as a building block in structure
 *   definitions, and does not cause alignment.
 *
 *  A pointer to an element of the structure does not need the
 *   leading "&". This is due to declarations as arrays.
 *
 *  A null pointer to the structure is NULL_<Name>. This is especially
 *   useful for passing null pointer in subroutine calls.
 *
 *  To get the length of an element or structure,
 *   length = LEN_<Name>(any_p);
 *
 *  To move a pointer past the structure,
 *   octet_p = SKIP_<Name>(any_p);
 *
 *  To copy a structure,
 *   COPY_<Name>(from_any_p, to_any_p);
 *
 *  To compare two structures,
 *   EQ_<Name>(any1_p, any2_p)
 *   NE_<Name>(any1_p, any2_p)
 *
 *  To check if a structure has no elements or is Null,
 *   if (EMPTY_<Name>(any_p))
 *
 *  To index into a structure with multiple elements,
 *   element_p = INDEX_<Name>(any_p, index);
 *
 *  To get the number of elements in a structure,
 *   val = NUM_<Name>(any_p);
 *
 *  To access the offset to a given element,
 *   OFFSET_<Name>(any_p, index)
 *
 *  To get to the data area past the offsets,
 *   DATA_<Name>(any_p)
 *
 * Basic calls:
 *
 *	LEN_bytes(any_p, len)
 *	DIFF_bytes(top_p, bot_p)
 *	COPY_bytes(from_p, to_p, len)
 *	MOVE_bytes(from_p, to_p, len)
 *	ZERO_bytes(from_p, to_p, len)
 *	EQ_bytes(arg1_p, arg2_p, len)
 *	NE_bytes(arg1_p, arg2_p, len)
 *
 * There are several structures which have corresponding Null externals.
 *  Some of them require definition of seperate structs for declation
 *  of the short-form.
 *
 * NOTE: The introduction of data types is order-dependent.
 * They cannot be put in alphabetical order, much to my disappointment!
 */

/*
 * Everything should be built up from unsigned characters.
 */

typedef unsigned char byte_t;
typedef unsigned char byte_u[sizeof(byte_t)];
typedef unsigned char bytes_u;
# define LEN_byte(ptr) sizeof(byte_u)
#define SKIP_byte(ptr) (&((byte_t *)(ptr))[1])
#define COPY_byte(from,to) INS8((byte_t *)(to),EXT8((byte_t *)(from)))
#define EQ_byte(p1,p2) (EXT8(p1) == EXT8(p2))
#define GT_byte(p1,p2) (EXT8(p1) > EXT8(p2))
#define LT_byte(p1,p2) (EXT8(p1) < EXT8(p2))

char *
deb_ascii_byte (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_byte DEB_ASCII_LEN_u_8
#define DEB_ASCII_BUF_byte(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_byte)
#define LOG_ASCII_BUF_byte(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_byte)

# define LEN_bytes(ptr,len) (len)

#ifndef DIFF_bytes
# define DIFF_bytes(top,bot) (((byte_t *)(top)) - ((byte_t *)(bot)))
#endif

#define SKIP_bytes(ptr,len) (&((byte_t *)(ptr))[len])

char *
deb_ascii_bytes (
    char *const       ,
    const void *const ,
    int               );
#define DEB_ASCII_LEN_bytes 82
#define DEB_ASCII_BUF_bytes(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_bytes)
#define LOG_ASCII_BUF_bytes(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_bytes)

/*
 * NOTE: MOVE_bytes() is for overlapping copies.
 */
#include <string.h>

#if !defined(SNI_SVR4)
# define COPY_bytes(from,to,len) \
    (void)memcpy(((char *)(to)),((char *)(from)),((int)(len)))
#else
# define COPY_bytes(from,to,len) \
    (void)memmove(((char *)(to)),((char *)(from)),((int)(len)))
#endif
#define MOVE_bytes(from,to,len) \
    (void)memmove(((char *)(to)),((char *)(from)),((int)(len)))
#define ZERO_bytes(ptr,len) (void)memset(((char *)(ptr)),0, ((int)(len)))
#define EQ_bytes(p1,p2,len) \
    (memcmp(((char *)(p1)),((char *)(p2)),((int)(len))) == 0)
#define NE_bytes(p1,p2,len) \
    (memcmp(((char *)(p1)),((char *)(p2)),((int)(len))) != 0)

typedef unsigned char word_t;
typedef word_t word_u[2*sizeof(byte_t)];
#define LEN_word(ptr) LEN_bytes(ptr,sizeof(word_u))
#define SKIP_word(ptr) SKIP_bytes((ptr),LEN_word(ptr))
#define SKIP_words(ptr,len) (&((word_t *)(ptr))[len])
#define COPY_word(from,to) INS16((word_t *)(to),EXT16((word_t *)(from)))
#define EQ_word(p1,p2) (EXT16(p1) == EXT16(p2))
#define NE_word(p1,p2) (EXT16(p1) != EXT16(p2))

char *
deb_ascii_word (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_word DEB_ASCII_LEN_u_16
#define DEB_ASCII_BUF_word(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_word)
#define LOG_ASCII_BUF_word(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_word)

typedef unsigned char longword_t;
typedef longword_t longword_u[2*sizeof(word_u)];
#define LEN_longword(ptr) LEN_bytes(ptr,sizeof(longword_u))
#define SKIP_longword(ptr) SKIP_bytes((ptr),LEN_longword(ptr))
#define SKIP_longwords(ptr,len) (&((longword_t *)(ptr))[len])
#define COPY_longword(from,to) INS32((longword_t *)(to),EXT32((longword_t *)(from)))
#define EQ_longword(p1,p2) (EXT32(p1) == EXT32(p2))
#define NE_longword(p1,p2) (EXT32(p1) != EXT32(p2))

char *
deb_ascii_longword (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_longword DEB_ASCII_LEN_u_32
#define DEB_ASCII_BUF_longword(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_longword)
#define LOG_ASCII_BUF_longword(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_longword)

typedef unsigned char address_t;
typedef address_t address_u[sizeof(long)];
#define LEN_address(ptr) LEN_bytes(ptr,sizeof(address_u))
#define SKIP_address(ptr) SKIP_bytes((ptr),LEN_address(ptr))
#define SKIP_addresss(ptr,len) (&((address_t *)(ptr))[len])
#define COPY_address(from,to) INS32((address_t *)(to),EXT32((address_t *)(from)))
#define EQ_address(p1,p2) (EXT32(p1) == EXT32(p2))
#define NE_address(p1,p2) (EXT32(p1) != EXT32(p2))

char *
deb_ascii_address (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_address DEB_ASCII_LEN_ptr
#define DEB_ASCII_BUF_address(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_address)
#define LOG_ASCII_BUF_address(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_address)

/*
 * Access macros for byte flags
 */

#define SET_FLAG(ptr,flg) *((byte_t *)(ptr)) |= (flg)
#define CLEAR_FLAG(ptr,flg) *((byte_t *)(ptr)) &= ~(flg)
#define FLAG_IS_SET(ptr,flg) ((*((byte_t *)(ptr)) & (flg)) != 0)
#define FLAG_IS_CLEAR(ptr,flg) ((*((byte_t *)(ptr)) & (flg)) == 0)
/*
 * These two are to be phased out
 */
#define FLAG_SET(ptr,flg) ((*((byte_t *)(ptr)) & (flg)) != 0)
#define FLAG_CLEAR(ptr,flg) ((*((byte_t *)(ptr)) & (flg)) == 0)

#ifndef NO_ALIGN
/*
 * Alignment
 *
 * Not all compilers pad structures out, nor do they
 * align all variables. Here is (hopefully) a way
 * to get the system-dependent alignment preferences.
 *
 * The general case was taken out, since the mips -lmalloc
 * package only guarantees word alignment, and the compiler
 * forces double to doubleword alignment.
 */

#if 0
/* use ANSI C offsetof() to figure out alignment */
# ifndef offsetof
#  include <stddef.h>
# endif

  typedef struct {
    char	align_char;
    struct {		/* cause this to be aligned */
	union {
	    char	*align_pointer;
	    long	align_long;
	    double	align_double;
	} align_union;
    } align_struct;
  } align_t;

# define	ALIGN_SIZE (offsetof(align_t, align_struct))
#else
# define	ALIGN_SIZE sizeof(byte_t *)
#endif

#define ALIGN_MASK (ALIGN_SIZE - 1)

#define ALIGN_PTR(p) \
    ((byte_t *)NULL + (((byte_t *)(p) + ALIGN_MASK - (byte_t *)NULL) & ~ALIGN_MASK))

#define ALIGN_LEN(n) (((n) + ALIGN_MASK) & ~ALIGN_MASK)

#define ALIGN_CHECK(p) \
    { if (((byte_t*)(p) - (byte_t *)NULL) & ALIGN_MASK) abort(); }
/*             *** VMS alignment fixed at longwords ***     */
# define ALIGN_DECL(type,name) type name
#else
# define ALIGN_MASK 0
# define ALIGN_LEN(n) (n)
# define ALIGN_PTR(p) (p)
# define ALIGN_CHECK(p) (p)
# define ALIGN_DECL(type,name) type name;
#endif

/*
 *	A R C H I T E C T U R A L   S T R U C T U R E S
 *
 * BOOLEAN
 */

typedef struct {
	byte_u		bl_value;
#define BL_true		 1
#define BL_false	 0
} BOOLEAN_t;
typedef bytes_u BOOLEAN_u[1*sizeof(byte_u)];
#define LEN_BOOLEAN(ptr) LEN_bytes(ptr,sizeof(BOOLEAN_u))
#define SKIP_BOOLEAN(ptr) SKIP_byte(ptr)
#define COPY_BOOLEAN(from,to) COPY_byte((from),(to))
#define EQ_BOOLEAN(p1,p2) EQ_byte(p1,p2)
#define NE_BOOLEAN(p1,p2) NE_byte(p1,p2)

char *
deb_ascii_BOOLEAN (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_BOOLEAN (DEB_ASCII_LEN_byte+15)
#define DEB_ASCII_BUF_BOOLEAN(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_BOOLEAN)
#define LOG_ASCII_BUF_BOOLEAN(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_BOOLEAN)

/*
 * INTEGER
 */

typedef struct {
	longword_u	in_value;
} INTEGER_t;
typedef bytes_u INTEGER_u[sizeof(longword_u)];
#define LEN_INTEGER(ptr) LEN_bytes(ptr,sizeof(INTEGER_u))
#define SKIP_INTEGER(ptr) SKIP_longword((ptr))
#define COPY_INTEGER(from,to) COPY_longword((from),(to))
#define EQ_INTEGER(p1,p2) EQ_longword(p1,p2)
#define NE_INTEGER(p1,p2) NE_longword(p1,p2)

#define deb_ascii_INTEGER(b,p) deb_ascii_rec(b,p,"NULL INTEGER_t")
#define DEB_ASCII_LEN_INTEGER DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_INTEGER(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_INTEGER(nam) LOG_ASCII_BUF_rec(nam)

/*
 * Time - Universal time
 */

typedef struct {
	bytes_u		tm_value[8];
} Time_t;
#define INITIALIZE_Time 0,0,0,0,0,0,0,0
typedef bytes_u Time_u[8*sizeof(byte_u)];
#define LEN_Time(ptr) LEN_bytes(ptr,sizeof(Time_u))
#define SKIP_Time(ptr) SKIP_bytes((ptr),LEN_Time(ptr))
#define COPY_Time(from,to) COPY_bytes((from),(to),LEN_Time(from))
#define	ZERO_Time(ptr) ZERO_bytes((ptr),LEN_Time(ptr))

#define deb_ascii_Time(b,p) deb_ascii_rec(b,p,"NULL Time")
#define DEB_ASCII_LEN_Time DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_Time(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_Time(nam) LOG_ASCII_BUF_rec(nam)
/* Time comparison below */

/*
 * utc - utc_t defined by dtss in utc.h
 * If running under RPC/v3.0 then timeouts are
 * 2 utc fields, otherwise timeouts are 2 time fields
 */

#define INITIALIZE_utc 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
typedef bytes_u utc_u[sizeof(utc_t)];

# define LEN_utc(ptr) LEN_bytes(ptr,sizeof(utc_u))
# define COPY_utc(from,to) COPY_bytes((from),(to),LEN_utc(from))
# define SKIP_utc(ptr) SKIP_bytes((ptr),LEN_utc(ptr))
# define ZERO_utc(ptr) ZERO_bytes((ptr),LEN_utc(ptr))
# define EQ_utc(t1,t2) cds_utc_equal(t1,t2)
# define NE_utc(t1,t2) !cds_utc_equal(t1,t2)
# define LT_utc(t1,t2) cds_utc_less(t1,t2)
# define GT_utc(t1,t2) cds_utc_greater(t1,t2)
# define ADD_utc(t1,t2,t3) cds_utc_add(t1,t2,t3)

/*
 * Timeout
 */
  typedef struct {
        utc_u          to_expiration;
        utc_u          to_extension;
  } Timeout_t;       
  typedef bytes_u Timeout_u[2*sizeof(utc_u)];
#define INITIALIZE_Timeout INITIALIZE_utc , INITIALIZE_utc
#define EQ_Timeout(p1,p2) \
  (EQ_utc(((Timeout_t *)p1)->to_expiration, ((Timeout_t *)p2)->to_expiration) \
   && EQ_utc(((Timeout_t *)p1)->to_extension, ((Timeout_t *)p2)->to_extension))

#define NE_Timeout(p1,p2) \
  (NE_utc(((Timeout_t *)p1)->to_expiration, ((Timeout_t *)p2)->to_expiration) \
   || NE_utc(((Timeout_t *)p1)->to_extension, ((Timeout_t *)p2)->to_extension))

#define LEN_Timeout(ptr) LEN_bytes(ptr,sizeof(Timeout_u))
#define SKIP_Timeout(ptr) SKIP_bytes((ptr),LEN_Timeout(ptr))
#define COPY_Timeout(from,to) COPY_bytes((from),(to),LEN_Timeout(from))
#define	ZERO_Timeout(ptr) ZERO_bytes((ptr),LEN_Timeout(ptr))

#define deb_ascii_Timeout(b,p) deb_ascii_rec(b,p,"NULL Timeout_t")
#define DEB_ASCII_LEN_Timeout DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_Timeout(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_Timeout(nam) LOG_ASCII_BUF_rec(nam)

/*
 * 802 Node Address
 */

typedef struct {
	bytes_u		nd_value[6];
} Node_t;
typedef bytes_u Node_u[6*sizeof(byte_u)];
#define INITIALIZE_Node 0,0,0,0,0,0
#define LEN_Node(ptr) LEN_bytes(ptr,sizeof(Node_u))
#define SKIP_Node(ptr) SKIP_bytes((ptr),LEN_Node(ptr))
#define COPY_Node(from,to) COPY_bytes((from),(to),LEN_Node(from))
#define EQ_Node(p1,p2) EQ_bytes(p1,p2,LEN_Node(p1))
#define NE_Node(p1,p2) NE_bytes(p1,p2,LEN_Node(p1))

#define deb_ascii_Node(b,p) deb_ascii_rec(b,p,"NULL Node_t")
#define DEB_ASCII_LEN_Node DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_Node(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_Node(nam) LOG_ASCII_BUF_rec(nam)

/*
 * DNS Address Tower
 */

typedef struct {
	byte_u		na_cnt;
	bytes_u		na_rest[NETWORKADDRESSMAX - 1];
} NetworkAddress_t;
typedef bytes_u NetworkAddress_u[NETWORKADDRESSMAX*sizeof(byte_u)];
typedef bytes_u NetworkAddress_overhead[sizeof(NetworkAddress_u) - (NETWORKADDRESSMAX - 1)];
#define LEN_NetworkAddress(ptr) (SKIP_NetworkAddress(ptr)-(unsigned char *)(ptr))
#define SKIP_NetworkAddress(ptr) \
    cds_pass_NetworkAddress((const byte_t *)(ptr))
#define COPY_NetworkAddress(from,to) \
    COPY_bytes((from),(to),LEN_NetworkAddress(from))
#define EQ_NetworkAddress(p1,p2) EQ_bytes(p1,p2,LEN_NetworkAddress(p1))
#define NE_NetworkAddress(p1,p2) NE_bytes(p1,p2,LEN_NetworkAddress(p1))

#define deb_ascii_NetworkAddress(b,p) deb_ascii_rec(b,p,"NULL NetworkAddress_t")
#define DEB_ASCII_LEN_NetworkAddress DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_NetworkAddress(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_NetworkAddress(nam) LOG_ASCII_BUF_rec(nam)

/*
 * Timestamp
 */

typedef struct {
	Node_u		ts_node;
	Time_u		ts_time;
} Timestamp_t;
#define INITIALIZE_Timestamp INITIALIZE_Node , INITIALIZE_Time
typedef bytes_u Timestamp_u[sizeof(Node_u)+sizeof(Time_u)];
#define LEN_Timestamp(ptr) LEN_bytes(ptr,sizeof(Timestamp_u))
#define SKIP_Timestamp(ptr) SKIP_bytes((ptr),LEN_Timestamp(ptr))
#define COPY_Timestamp(from,to) COPY_bytes((from),(to),LEN_Timestamp(from))
#define	ZERO_Timestamp(ptr) ZERO_bytes((ptr),LEN_Timestamp(ptr))
#define EQ_Timestamp(p1,p2) EQ_bytes(p1,p2,LEN_Timestamp(p1))
#define NE_Timestamp(p1,p2) NE_bytes(p1,p2,LEN_Timestamp(p1))

char *
deb_ascii_Timestamp (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_Timestamp (DEB_ASCII_LEN_time_quad+18)
#define DEB_ASCII_BUF_Timestamp(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_Timestamp)
#define LOG_ASCII_BUF_Timestamp(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_Timestamp)

/*
 * ObjUID
 */
#define INITIALIZE_ObjUID 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  typedef uuid_t ObjUID_t;
  typedef bytes_u ObjUID_u[sizeof(uuid_t)];
#define LEN_ObjUID(ptr)  LEN_bytes(ptr,sizeof(ObjUID_u))
#define SKIP_ObjUID(ptr) SKIP_bytes((ptr),LEN_ObjUID(ptr))
#define COPY_ObjUID(from,to) COPY_bytes((from),(to),LEN_ObjUID(from))
#define ZERO_ObjUID(ptr) cds_uuid_create_nil((uuid_t *)(ptr))
#define EQ_ObjUID(p1,p2) cds_uuid_equal((uuid_t *)p1, (uuid_t *)p2)
#define NE_ObjUID(p1,p2) (!EQ_ObjUID(p1,p2))
#define ObjUID_is_nil(p1) cds_uuid_is_nil((uuid_t *)p1)
#define ObjUID_compare(p1,p2) cds_uuid_compare((uuid_t *)p1, (uuid_t *)p2)


char *
deb_ascii_ObjUID (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_ObjUID 49
#define DEB_ASCII_BUF_ObjUID(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_ObjUID)
#define LOG_ASCII_BUF_ObjUID(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_ObjUID)

/*
 * Confidence
 */

typedef struct {
	byte_u		co_value;
#define CO_low		 1
#define CO_medium	 2
#define CO_high		 3
} Confidence_t;
typedef bytes_u Confidence_u[sizeof(byte_u)];
#define LEN_Confidence(ptr) LEN_bytes(ptr,sizeof(Confidence_u))
#define SKIP_Confidence(ptr) SKIP_bytes((ptr),LEN_Confidence(ptr))
#define COPY_Confidence(from,to) COPY_bytes((from),(to),LEN_Confidence(from))

#define deb_ascii_Confidence(b,p) deb_ascii_rec(b,p,"NULL Confidence_t")
#define DEB_ASCII_LEN_Confidence DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_Confidence(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_Confidence(nam) LOG_ASCII_BUF_rec(nam)

/*
 * EntryType
 */

typedef struct {
	byte_u		et_value;
#define ET_directory	 1
#define ET_object	 2
#define ET_childPointer	 3
#define ET_softlink	 4
#define ET_clearinghouse 5
#define ET_anyDirectName 6
#define ET_firstLink     7
#define ET_dirOrObj	 8
} EntryType_t;
typedef bytes_u EntryType_u[sizeof(byte_u)];
#define LEN_EntryType(ptr) LEN_bytes(ptr,sizeof(EntryType_u))
#define SKIP_EntryType(ptr) SKIP_bytes((ptr),LEN_EntryType(ptr))
#define COPY_EntryType(from,to) COPY_bytes((from),(to),LEN_EntryType(from))

char *
deb_ascii_EntryType (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_EntryType (DEB_ASCII_LEN_byte+17)
#define DEB_ASCII_BUF_EntryType(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_EntryType)
#define LOG_ASCII_BUF_EntryType(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_EntryType)

/*
 * SimpleName
 */

typedef struct {
	byte_u		sn_flag;
#define SN_type		 (0xF<<0)
#define SN_null		  (0<<0)
#define SN_normal	  (1<<0)
#define SN_quoted	  (2<<0)
#define SN_binary	  (3<<0)
#define SN_wildcard	  (4<<0)
#define SN_ellipsis	  (5<<0)
#define SN_typed	  (6<<0)
#define SN_objectid	  (7<<0)
#define SN_cds            (8<<0)
#define SN_cdswildcard    (9<<0)
#define SN_reserved	 (0xF<<4)
	byte_u		sn_length;
	bytes_u		sn_name[SIMPLENAMEMAX];
} SimpleName_t;
typedef bytes_u SimpleName_u[(1+1+SIMPLENAMEMAX)*sizeof(byte_u)];
typedef bytes_u SimpleName_overhead[sizeof(SimpleName_u) - SIMPLENAMEMAX];
#define LEN_SimpleName(ptr) (sizeof(SimpleName_u) - SIMPLENAMEMAX + \
    EXT8(((SimpleName_t *)(ptr))->sn_length))
#define SKIP_SimpleName(ptr) (&((SimpleName_t *)(ptr)) \
    ->sn_name[EXT8(((SimpleName_t *)(ptr))->sn_length)])
#define COPY_SimpleName(from,to) COPY_bytes((from),(to),LEN_SimpleName(from))
#define	ZERO_SimpleName(ptr) COPY_SimpleName(NullSimpleName,(ptr))
#define EQ_SimpleName(p1,p2) EQ_bytes(p1,p2,LEN_SimpleName(p1))
#define NE_SimpleName(p1,p2) NE_bytes(p1,p2,LEN_SimpleName(p1))
#define EMPTY_SimpleName(ptr) (EXT8(((SimpleName_t *)(ptr))->sn_length) == 0)

typedef struct {
	byte_u		nsn_flag;
	byte_u		nsn_length;
} NULLSIMPLENAME_t;
typedef bytes_u NULLSIMPLENAME_u[2*sizeof(byte_u)];
#define INITIALIZE_SimpleName 0,0
#define INITIALIZE_BinarySimpleName 3,0

char *
deb_ascii_SimpleName (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_SimpleName 257
#define DEB_ASCII_BUF_SimpleName(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_SimpleName)
#define LOG_ASCII_BUF_SimpleName(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_SimpleName)

/*
 * EntryName
 */

#define EntryName SimpleName
#define EntryName_t SimpleName_t
#define EntryName_u SimpleName_u
#define EntryName_overhead SimpleName_overhead
#define LEN_EntryName LEN_SimpleName
#define SKIP_EntryName SKIP_SimpleName
#define COPY_EntryName COPY_SimpleName
#define ZERO_EntryName ZERO_SimpleName
#define EMPTY_EntryName EMPTY_SimpleName

#define deb_ascii_EntryName(b,p) deb_ascii_SimpleName(b,p)
#define DEB_ASCII_LEN_EntryName DEB_ASCII_LEN_SimpleName
#define DEB_ASCII_BUF_EntryName(nam) DEB_ASCII_BUF_SimpleName(nam)
#define LOG_ASCII_BUF_EntryName(nam) LOG_ASCII_BUF_SimpleName(nam)

/*
 * AttributeName
 */

typedef struct {
	byte_u		an_flag;
#define AN_type		 (0xF<<0)
#define AN_null		  (0<<0)
#define AN_normal	  (1<<0)
#define AN_quoted	  (2<<0)
#define AN_binary	  (3<<0)
#define AN_wildcard	  (4<<0)
#define AN_ellipsis	  (5<<0)
#define AN_reserved	 (0xF<<4)
	byte_u		an_length;
	bytes_u		an_name[ATTRIBNAMEMAX];
} AttributeName_t;
typedef bytes_u AttributeName_u[(1+1+ATTRIBNAMEMAX)*sizeof(byte_u)];
typedef bytes_u AttributeName_overhead[sizeof(AttributeName_u) - ATTRIBNAMEMAX];
#define NullAttributeName NullSimpleName
#define LEN_AttributeName(ptr) LEN_SimpleName(ptr)
#define SKIP_AttributeName(ptr) SKIP_SimpleName(ptr)
#define COPY_AttributeName(from,to) COPY_SimpleName(from,to)
#define	ZERO_AttributeName(ptr) ((AttributeName_t *)(ptr))->an_flag = \
    ((AttributeName_t *)(ptr))->an_length = 0, SN_null
#define EMPTY_AttributeName(ptr) EMPTY_SimpleName(ptr)

char *
deb_ascii_AttributeName (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_AttributeName 257
#define DEB_ASCII_BUF_AttributeName(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_AttributeName)
#define LOG_ASCII_BUF_AttributeName(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_AttributeName)

/*
 * ClassName
 */

typedef struct {
	byte_u		cn_flag;
	byte_u		cn_length;
	bytes_u		cn_name[CLASSSIZE];
} ClassName_t;
typedef bytes_u ClassName_u[(1+1+CLASSSIZE)*sizeof(byte_u)];
typedef bytes_u ClassName_overhead[sizeof(ClassName_u) - CLASSSIZE];
#define NullClassName NullSimpleName
#define LEN_ClassName(ptr) LEN_SimpleName(ptr)
#define SKIP_ClassName(ptr) SKIP_SimpleName(ptr)
#define COPY_ClassName(from,to) COPY_SimpleName(from,to)
#define ZERO_ClassName(ptr) ZERO_SimpleName(ptr)
#define EMPTY_ClassName(ptr) EMPTY_SimpleName(ptr)

#define deb_ascii_ClassName(b,p) deb_ascii_SimpleName(b,p)
#define DEB_ASCII_LEN_ClassName DEB_ASCII_LEN_SimpleName
#define DEB_ASCII_BUF_ClassName(nam) DEB_ASCII_BUF_SimpleName(nam)
#define LOG_ASCII_BUF_ClassName(nam) LOG_ASCII_BUF_SimpleName(nam)

/*
 * FullName
 */

typedef struct {
	ObjUID_u	fn_root;
	word_u		fn_length;
	bytes_u		fn_name[FULLNAMEMAX - (sizeof(word_u) + sizeof(ObjUID_u))];
} FullName_t;
typedef bytes_u FullName_u[FULLNAMEMAX*sizeof(byte_u)];
typedef bytes_u FullName_overhead[sizeof(ObjUID_u) + sizeof(word_u)];
#define LEN_FullName(ptr) (sizeof(ObjUID_u) + sizeof(word_u) + \
    EXT16(((FullName_t *)(ptr))->fn_length))
#define SKIP_FullName(ptr) (&((FullName_t *)(ptr)) \
    ->fn_name[EXT16(((FullName_t *)(ptr))->fn_length)])
#define COPY_FullName(from,to) COPY_bytes((from),(to),LEN_FullName(from))
#define	ZERO_FullName(ptr) COPY_bytes(NullFullName,(ptr),sizeof(NULLFULLNAME_u))
#define EQ_FullName(p1,p2) EQ_bytes(p1,p2,LEN_FullName(p1))
#define NE_FullName(p1,p2) NE_bytes(p1,p2,LEN_FullName(p1))
#define EMPTY_FullName(ptr) (EXT16(((FullName_t *)(ptr))->fn_length) <= 2)

typedef struct {
	ObjUID_u	nfn_root;
	word_u		nfn_length;
	NULLSIMPLENAME_u nfn_name;
} NULLFULLNAME_t;
typedef bytes_u NULLFULLNAME_u[sizeof(ObjUID_u)+sizeof(word_u)+sizeof(NULLSIMPLENAME_u)];

#if BYTE_ORDER == LITTLE_ENDIAN
# define INITIALIZE_FullName INITIALIZE_ObjUID, 2,0, INITIALIZE_SimpleName
#else
# define INITIALIZE_FullName INITIALIZE_ObjUID, 0,2, INITIALIZE_SimpleName
#endif

char *
deb_ascii_FullName (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_FullName 1023
#define DEB_ASCII_BUF_FullName(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_FullName)
#define LOG_ASCII_BUF_FullName(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_FullName)

/*
 * NameAndClass
 */

typedef struct {
	SimpleName_u	nc_name;
	ClassName_u	ncv_class;
} NameAndClass_t;
typedef bytes_u NameAndClass_u[sizeof(SimpleName_u)+sizeof(ClassName_u)];
#define LEN_NameAndClass(ptr) (SKIP_NameAndClass(ptr)-(byte_t *)(ptr))
#define SKIP_NameAndClass(ptr) SKIP_ClassName(SKIP_SimpleName(ptr))
#define COPY_NameAndClass(from,to) \
    COPY_bytes((from),(to),LEN_NameAndClass(from))

#define deb_ascii_NameAndClass(b,p) deb_ascii_rec(b,p,"NULL NameAndClass_t")
#define DEB_ASCII_LEN_NameAndClass DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_NameAndClass(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_NameAndClass(nam) LOG_ASCII_BUF_rec(nam)

/*
 * Atomic Value
 */

typedef struct {
	word_u		av_length;
	byte_u          av_valuetype;
	bytes_u		av_value[MAXATTRIBUTE];
} AtomicValue_t;
typedef bytes_u AtomicValue_u[sizeof(word_u)+(1+MAXATTRIBUTE)*sizeof(byte_u)];
typedef bytes_u AtomicValue_overhead[sizeof(AtomicValue_u) - MAXATTRIBUTE];
#define LEN_AtomicValue(ptr) (SKIP_AtomicValue(ptr)-(byte_t *)(ptr))
#define SKIP_AtomicValue(ptr) (&((AtomicValue_t *)(ptr)) \
    ->av_value[EXT16(((AtomicValue_t *)(ptr))->av_length)])
#define COPY_AtomicValue(from,to) COPY_bytes((from),(to),LEN_AtomicValue(from))

#define deb_ascii_AtomicValue(b,p) deb_ascii_rec(b,p,"NULL AtomicValue_t")
#define DEB_ASCII_LEN_AtomicValue DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_AtomicValue(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_AtomicValue(nam) LOG_ASCII_BUF_rec(nam)

/*
 * MemberHeader
 */

typedef struct {
	byte_u		mh_flag;
#define MH_present	 (1<<0)
#define MHX_absent	 (0<<0)		/* do not use in FLAG macros */
#define MH_reserved	 (0x3F<<1)
	Timestamp_u	mh_timestamp;
} MemberHeader_t;
typedef bytes_u MemberHeader_u[sizeof(byte_u)+sizeof(Timestamp_u)];
#define LEN_MemberHeader(ptr) LEN_bytes(ptr,sizeof(MemberHeader_u))
#define SKIP_MemberHeader(ptr) SKIP_bytes((ptr),LEN_MemberHeader(ptr))
#define COPY_MemberHeader(from,to) \
    COPY_bytes((from),(to),LEN_MemberHeader(from))

/*
 * SetMember
 */

typedef struct {
	MemberHeader_u	sm_header;
	AtomicValue_u	sm_value;
} SetMember_t;
typedef bytes_u SetMember_u[sizeof(MemberHeader_u)+sizeof(AtomicValue_u)];
typedef bytes_u SetMember_overhead[sizeof(SetMember_u) - MAXATTRIBUTE];
#define LEN_SetMember(ptr) (SKIP_SetMember(ptr)-(byte_t *)(ptr))
#define SKIP_SetMember(ptr) \
    SKIP_AtomicValue(((SetMember_t *)(ptr))->sm_value)
#define COPY_SetMember(from,to) COPY_bytes((from),(to),LEN_SetMember(from))

/*
 * MemberValue - special combination of MemberHeader and AtomicValue
 */

typedef struct {
	byte_u		mv_flag;
#define MV_present	 (1<<0)
#define MVX_absent	 (0<<0)		/* do not use in FLAG macros */
#define MV_reserved	 (0x3F<<1)
	Timestamp_u	mv_timestamp;
	word_u		mv_length;
	byte_u          mv_valuetype;
	bytes_u		mv_value[MAXATTRIBUTE];
} MemberValue_t;
typedef bytes_u MemberValue_u[sizeof(Timestamp_u)+sizeof(word_u)+(1+1+MAXATTRIBUTE)*sizeof(byte_u)];
typedef bytes_u MemberValue_overhead[sizeof(MemberValue_u) - MAXATTRIBUTE];
#define LEN_MemberValue(ptr) (SKIP_MemberValue(ptr)-(byte_t *)(ptr))
#define SKIP_MemberValue(ptr) \
    SKIP_AtomicValue(((MemberValue_t *)(ptr))->mv_length)
#define COPY_MemberValue(from,to) COPY_bytes((from),(to),LEN_MemberValue(from))

/*
 * Set
 */

typedef struct {
	byte_u		st_flag;
#define ST_present	 (1<<0)
#define STX_absent	 (0<<0)		/* do not use in FLAG macros */
#define ST_reserved	 (0x3F<<1)
	word_u		st_offsetLength;
	word_u		st_valueLength;
	word_u		st_offsets;
} Set_t;
typedef bytes_u Set_u[sizeof(byte_u)+3*sizeof(word_u)];
typedef bytes_u Set_overhead[sizeof(Set_u) - 2];
#define LEN_Set(ptr) (SKIP_Set(ptr)-(byte_t *)(ptr))
#define SKIP_Set(ptr) (&((Set_t *)(ptr))->st_offsets[ \
    EXT16(((Set_t *)(ptr))->st_offsetLength) + \
    EXT16(((Set_t *)(ptr))->st_valueLength)])
#define COPY_Set(from,to) COPY_bytes((from),(to),LEN_Set(from))
#define ZERO_Set(ptr) COPY_bytes(NullSet,(ptr),sizeof(NULLSET_u))
#define EMPTY_Set(ptr) (EXT16(((Set_t *)(ptr))->st_offsetLength) == 0)
#define NUM_Set(ptr) (EXT16(((Set_t *)(ptr))->st_offsetLength)/sizeof(word_u))
#define OFFSET_Set(ptr,idx) SKIP_bytes(((Set_t *)(ptr))->st_offsets,((idx)<<1))
#define DATA_Set(ptr) SKIP_bytes(((Set_t *)(ptr))->st_offsets, \
    EXT16(((Set_t *)(ptr))->st_offsetLength))

#define INDEX_Set(ptr,idx) SKIP_bytes((ptr),EXT16(OFFSET_Set((ptr),(idx))))
typedef struct {
	byte_u		stn_flag;
	word_u		stn_offsetLength;
	word_u		stn_valueLength;
} NULLSET_t;
typedef bytes_u NULLSET_u[sizeof(byte_u)+2*sizeof(word_u)];
#define INITIALIZE_Set STX_absent,{0,0},{0,0}

#define deb_ascii_Set(b,p) deb_ascii_rec(b,p,"NULL Set_t")
#define DEB_ASCII_LEN_Set DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_Set(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_Set(nam) LOG_ASCII_BUF_rec(nam)

/*
 * AttributeType
 */

typedef struct {
	byte_u		at_value;
#define AT_null		 1
#define AT_single	 2
#define AT_set		 3
} AttributeType_t;
typedef bytes_u AttributeType_u[sizeof(byte_u)];
#define LEN_AttributeType(ptr) LEN_bytes(ptr,sizeof(AttributeType_u))
#define SKIP_AttributeType(ptr) SKIP_bytes((ptr),LEN_AttributeType(ptr))
#define COPY_AttributeType(from,to) \
    COPY_bytes((from),(to),LEN_AttributeType(from))

char *
deb_ascii_AttributeType (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_AttributeType (DEB_ASCII_LEN_byte+21)
#define DEB_ASCII_BUF_AttributeType(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_AttributeType)
#define LOG_ASCII_BUF_AttributeType(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_AttributeType)

/*
 * AttributeSpecifier
 */

typedef struct {
	AttributeType_u	as_type;
	AttributeName_u	as_name;
} AttributeSpecifier_t;
typedef bytes_u AttributeSpecifier_u[sizeof(AttributeType_u)+sizeof(AttributeName_u)];
#define LEN_AttributeSpecifier(ptr) \
    (SKIP_AttributeSpecifier(ptr)-(byte_t *)(ptr))
#define SKIP_AttributeSpecifier(ptr) \
    cds_pass_AttributeSpecifier((const byte_t *)(ptr))
#define COPY_AttributeSpecifier(from,to) \
    COPY_bytes((from),(to),LEN_AttributeSpecifier(from))
#define FLD_AttributeSpecifier_AttributeName(ptr) \
    ((AttributeSpecifier_t *)(ptr))->as_name

#define deb_ascii_AttributeSpecifier(b,p) deb_ascii_rec(b,p,"NULL AttributeSpecifier_t")
#define DEB_ASCII_LEN_AttributeSpecifier DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_AttributeSpecifier(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_AttributeSpecifier(nam) LOG_ASCII_BUF_rec(nam)

/*
 * AttributeContents
 */

/*
 * Returning to Clerk (RC) defs for ReadAttributeResponse
 */
#define	RC_ONE_ATTRIBUTE 1
#define	RC_WHOLE_ENTRY	2

typedef struct {
	AttributeType_u ac_type;
/*	union {				*/
/*	    SetMember_u	acx_single;	*/
/*	    Set_u	acx_set;	*/
/*	} acx;				*/
	bytes_u		acx_union[sizeof(SetMember_u)];
#define ac_single acx_union
#define ac_set acx_union
} AttributeContents_t;
typedef bytes_u AttributeContents_u[sizeof(AttributeType_u)+sizeof(SetMember_u)];
typedef bytes_u AttributeContents_overhead[sizeof(AttributeContents_u) - sizeof(SetMember_u)];
#define LEN_AttributeContents(ptr) (SKIP_AttributeContents(ptr)-(byte_t *)(ptr))
#define SKIP_AttributeContents(ptr) cds_pass_AttributeContents((const byte_t *) ptr)
#define COPY_AttributeContents(from,to) \
    COPY_bytes((from),(to),LEN_AttributeContents(from))

typedef struct {
	AttributeType_u	nat_type;
} NULLATTRIBUTE_t;
typedef bytes_u NULLATTRIBUTE_u[sizeof(AttributeType_u)];
#define INITIALIZE_Attribute AT_null

#define deb_ascii_AttributeContents(b,p) deb_ascii_rec(b,p,"NULL AttributeContents_t")
#define DEB_ASCII_LEN_AttributeContents DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_AttributeContents(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_AttributeContents(nam) LOG_ASCII_BUF_rec(nam)

/*
 * WholeEntry
 */

typedef struct {
	byte_u		wh_numberOfAttribtues;
/*	union {					*/
/*	    struct whx_entry {			*/
/*		AttributeName_u     wh_name;	*/
/*		AttributeContents_u whv_value;	*/
/*	    } whx_entry;			*/
/*	} whx;					*/
	bytes_u		whx_union[sizeof(AttributeName_u) + sizeof(AttributeContents_u)];
#define wh_entry whx_union
} WholeEntry_t;
typedef bytes_u WholeEntry_u[sizeof(byte_u)+sizeof(AttributeName_u)+sizeof(AttributeContents_u)];
#define LEN_WholeEntry(ptr) (SKIP_WholeEntry(ptr)-(byte_t *)(ptr))
#define SKIP_WholeEntry(ptr) cds_pass_WholeEntry((const byte_t *)(ptr))
#define COPY_WholeEntry(from,to) COPY_bytes((from),(to),LEN_WholeEntry(from))

/*
 * ObjectClass
 */

#define ObjectClass ClassName
#define ObjectClass_t ClassName_t
#define ObjectClass_u ClassName_u
#define LEN_ObjectClass LEN_ClassName
#define SKIP_ObjectClass SKIP_ClassName
#define COPY_ObjectClass COPY_ClassName
#define ZERO_ObjectClass ZERO_ClassName

#define deb_ascii_ObjectClass(b,p) deb_ascii_ClassName(b,p)
#define DEB_ASCII_LEN_ObjectClass DEB_ASCII_LEN_ClassName
#define DEB_ASCII_BUF_ObjectClass(nam) DEB_ASCII_BUF_ClassName(nam)
#define LOG_ASCII_BUF_ObjectClass(nam) LOG_ASCII_BUF_ClassName(nam)

/*
 * VersionNumber
 */

typedef struct {
	byte_u		vn_major;
	byte_u		vn_minor;
} VersionNumber_t;
typedef bytes_u VersionNumber_u[2*sizeof(byte_u)];
#define LEN_VersionNumber(ptr) LEN_bytes(ptr,sizeof(VersionNumber_u))
#define SKIP_VersionNumber(ptr) SKIP_bytes((ptr),LEN_VersionNumber(ptr))
#define COPY_VersionNumber(from,to) \
    COPY_bytes((from),(to),LEN_VersionNumber(from))
#define EQ_VersionNumber(p1,p2) EQ_bytes(p1,p2,LEN_VersionNumber(p1))
#define GT_VersionNumber(p1,p2) \
    ( ( EXT8(p1->vn_major) > EXT8(p2->vn_major) ) ||        \
      ( ( EXT8(p1->vn_major) == EXT8(p2->vn_major) ) &&     \
        ( EXT8(p1->vn_minor) >  EXT8(p2->vn_minor) )        \
      )                                                     \
    )
#define LT_VersionNumber(p1,p2) \
    ( ( EXT8(p1->vn_major) < EXT8(p2->vn_major) ) ||        \
      ( ( EXT8(p1->vn_major) == EXT8(p2->vn_major) ) &&     \
        ( EXT8(p1->vn_minor) <  EXT8(p2->vn_minor) )        \
      )                                                     \
    )
#define ZERO_VersionNumber(p1) \
    ZERO_bytes((p1), sizeof(VersionNumber_u))

#define deb_ascii_VersionNumber(b,p) deb_ascii_rec(b,p,"NULL VersionNumber_t")
#define DEB_ASCII_LEN_VersionNumber DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_VersionNumber(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_VersionNumber(nam) LOG_ASCII_BUF_rec(nam)

/*
 * ClassVersion
 */

typedef struct {
	byte_u		cv_major;
	byte_u		cv_minor;
} ClassVersion_t;
typedef bytes_u ClassVersion_u[2*sizeof(byte_u)];
#define LEN_ClassVersion(ptr) LEN_bytes(ptr,sizeof(ClassVersion_u))
#define SKIP_ClassVersion(ptr) SKIP_bytes((ptr),LEN_ClassVersion(ptr))
#define COPY_ClassVersion(from,to) \
    COPY_bytes((from),(to),LEN_ClassVersion(from))

#define deb_ascii_ClassVersion(b,p) deb_ascii_rec(b,p,"NULL ClassVersion_t")
#define DEB_ASCII_LEN_ClassVersion DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_ClassVersion(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_ClassVersion(nam) LOG_ASCII_BUF_rec(nam)

/*
 * AccessControlEntry
 */

typedef struct {
	byte_u		ace_flags;
#define ACF_unAuthenticated (1<<0)
#define ACF_default	 (1<<1)
#define ACF_noPropagate	 (1<<2)
#define ACF_group	 (1<<3)
#define ACF_reserved	 (0xF<<4)
	byte_u		ace_rights;
#define ACR_read	 (1<<0)
#define ACR_write	 (1<<1)
#define ACR_delete	 (1<<2)
#define ACR_test	 (1<<3)
#define ACR_control	 (1<<4)
#define ACR_reserved	 (0x7<<5)
	FullName_u	ace_identifier;
} ACE_t;
typedef bytes_u ACE_u[2*sizeof(byte_u)+sizeof(FullName_u)];
typedef bytes_u ACE_overhead[sizeof(ACE_u) - sizeof(FullName_u)];
#define LEN_ACE(ptr) (SKIP_ACE(ptr)-(byte_t *)(ptr))
#define SKIP_ACE(ptr) \
    SKIP_FullName(((ACE_t *)(ptr))->ace_identifier)
#define COPY_ACE(from,to) COPY_bytes((from),(to),LEN_ACE(from))

#define deb_ascii_ACE(b,p) deb_ascii_rec(b,p,"NULL ACE_t")
#define DEB_ASCII_LEN_ACE DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_ACE(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_ACE(nam) LOG_ASCII_BUF_rec(nam)

/*
 * GroupMember
 */

typedef struct {
	BOOLEAN_u	gm_isaGroup;
	FullName_u	gm_member;
} GroupMember_t;
typedef bytes_u GroupMember_u[sizeof(BOOLEAN_u)+sizeof(FullName_u)];
typedef bytes_u GroupMember_overhead[sizeof(GroupMember_u) - sizeof(FullName_u)];
#define LEN_GroupMember(ptr) (SKIP_GroupMember(ptr)-(byte_t *)(ptr))
#define SKIP_GroupMember(ptr) \
    SKIP_FullName(((GroupMember_t *)(ptr))->gm_member)
#define COPY_GroupMember(from,to) COPY_bytes((from),(to),LEN_GroupMember(from))

/*
 * ReplicaState
 */

typedef struct {
	byte_u		rs_value;
#define RS_newDir	 1
#define RS_newReplica	 2
#define RS_on		 3
/* 
 * In DNS v1.0 we mistakingly defined dyingdir =5 and dyingreplica=4
 * The solution in V2.0 on Ultrix was to leave the defines as architected
 * #define RS_dyingDir	 4, #define RS_dyingReplica	 5
 * but to change the meaning of the labels in the code.  
 * This was very confusing,
 * so it is being fixed here and the code will do the correct thing for
 * each label.  
 * In either case the ARch. should be changed. to swap the values
 */
#define RS_dyingDir	 5    
#define RS_dyingReplica	 4
#define RS_dead		 6
} ReplicaState_t;
typedef bytes_u ReplicaState_u[sizeof(byte_u)];
#define LEN_ReplicaState(ptr) LEN_bytes(ptr,sizeof(ReplicaState_u))
#define SKIP_ReplicaState(ptr) SKIP_bytes((ptr),LEN_ReplicaState(ptr))
#define COPY_ReplicaState(from,to) \
    COPY_bytes((from),(to),LEN_ReplicaState(from))

/*
 * ReplicaType
 */

typedef struct {
	byte_u		rt_value;
#define RT_master	 1
#define RT_secondary	 2
#define RT_readOnly	 3
#define RT_gda		 4
} ReplicaType_t;
typedef bytes_u ReplicaType_u[sizeof(byte_u)];
#define LEN_ReplicaType(ptr) LEN_bytes(ptr,sizeof(ReplicaType_u))
#define SKIP_ReplicaType(ptr) SKIP_bytes((ptr),LEN_ReplicaType(ptr))
#define COPY_ReplicaType(from,to) COPY_bytes((from),(to),LEN_ReplicaType(from))

/*
 * ReplicaPointer
 */

typedef struct {
	ObjUID_u	rp_CHID;
	bytes_u         rp_addressHint[MAXATTRIBUTE]; /* Set_u */
	ReplicaType_u	rpv_replicaType;
	FullName_u	rpv_CHName;
} ReplicaPointer_t;
typedef bytes_u ReplicaPointer_u[sizeof(ObjUID_u)+MAXATTRIBUTE*sizeof(byte_u)+sizeof(ReplicaType_u)+sizeof(FullName_u)];
typedef bytes_u ReplicaPointer_overhead[sizeof(ReplicaPointer_u) -
    MAXATTRIBUTE - sizeof(FullName_u)];
#define LEN_ReplicaPointer(ptr) (SKIP_ReplicaPointer(ptr)-(byte_t *)(ptr))
#define SKIP_ReplicaPointer(ptr) \
    cds_pass_ReplicaPointer((const byte_t *)(ptr))
#define COPY_ReplicaPointer(from,to) \
    COPY_bytes((from),(to),LEN_ReplicaPointer(from))
#define	FLD_ReplicaPointer_replicaType(ptr) (ReplicaType_t *) \
    SKIP_Set(((ReplicaPointer_t *)(ptr))->rp_addressHint)
#define	FLD_ReplicaPointer_CHName(ptr) (FullName_t *) \
    SKIP_ReplicaType(SKIP_Set(((ReplicaPointer_t *)(ptr))->rp_addressHint))

#define deb_ascii_ReplicaPointer(b,p) deb_ascii_rec(b,p,"NULL ReplicaPointer_t")
#define DEB_ASCII_LEN_ReplicaPointer DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_ReplicaPointer(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_ReplicaPointer(nam) LOG_ASCII_BUF_rec(nam)

/*
 * Convergence
 */

typedef struct {
	byte_u		cg_value;
#define CG_low		 1
#define CG_medium	 2
#define CG_high		 3
} Convergence_t;
typedef bytes_u Convergence_u[sizeof(byte_u)];
#define LEN_Convergence(ptr) LEN_bytes(ptr,sizeof(Convergence_u))
#define SKIP_Convergence(ptr) SKIP_bytes((ptr),LEN_Convergence(ptr))
#define COPY_Convergence(from,to) COPY_bytes((from),(to),LEN_Convergence(from))

/*
 * ParentPointer
 */

typedef struct {
	ObjUID_u	pp_parentID;
	Timeout_u	pp_timeout;
	FullName_u	pp_myName;
} ParentPointer_t;
typedef bytes_u ParentPointer_u[sizeof(ObjUID_u)+sizeof(Timeout_u)+sizeof(FullName_u)];
typedef bytes_u ParentPointer_overhead[sizeof(ParentPointer_u) - sizeof(FullName_u)];
#define LEN_ParentPointer(ptr) (SKIP_ParentPointer(ptr)-(byte_t *)(ptr))
#define SKIP_ParentPointer(ptr) \
    SKIP_FullName(((ParentPointer_t *)(ptr))->pp_myName)
#define COPY_ParentPointer(from,to) \
    COPY_bytes((from),(to),LEN_ParentPointer(from))

/*
 * gdaPointer
 */

typedef struct {
	Timeout_u	gp_timeout;
	ReplicaPointer_u gp_replica;
} gdaPointer_t;
typedef bytes_u gdaPointer_u[sizeof(ObjUID_u)+MAXATTRIBUTE*sizeof(byte_u)+sizeof(ReplicaType_u)+sizeof(FullName_u)];
typedef bytes_u gdaPointer_overhead[sizeof(gdaPointer_u) -
    MAXATTRIBUTE - sizeof(FullName_u)];
#define LEN_gdaPointer(ptr) (SKIP_gdaPointer(ptr)-(byte_t *)(ptr))
#define SKIP_gdaPointer(ptr) \
    SKIP_ReplicaPointer(((gdaPointer_t *)(ptr))->gp_replica)
#define COPY_gdaPointer(from,to) \
    COPY_bytes((from),(to),LEN_gdaPointer(from))

/*
 * CHDirectoryPointer
 */

typedef struct {
	ObjUID_u	cp_dirID;
	FullName_u	cp_directory;
} CHDirectoryPointer_t;
typedef bytes_u CHDirectoryPointer_u[sizeof(ObjUID_u)+sizeof(FullName_u)];
#define LEN_CHDirectoryPointer(ptr) \
    (SKIP_CHDirectoryPointer(ptr)-(byte_t *)(ptr))
#define SKIP_CHDirectoryPointer(ptr) \
    SKIP_FullName(((CHDirectoryPointer_t *)(ptr))->cp_directory)
#define COPY_CHDirectoryPointer(from,to) \
    COPY_bytes((from),(to),LEN_CHDirectoryPointer(from))

/*
 * Structures used for DNS$SKULKSTATUS attribute.  Data is hung of the 
 * directory's DHE.  
 */
typedef struct ErrSet {
    longword_u		err_phase_offp;		 /* Offset to phase string */
    longword_u	 	err_setl;		 /* Total length of set  */
    word_u		err_ocnt;		 /* Number offsets in array */
    longword_u		err_offp[1];		 /* First offset into array */
    /* followed by array of strings */
} ErrSet_t;
typedef bytes_u ErrSet_u[ sizeof(longword_u) * 3 + sizeof(word_u)];
typedef bytes_u err_offp_u[ sizeof(longword_u)];

#define sk_none_yet  	0
#define sk_success   	1
#define sk_failure   	2
#define sk_in_progress	3
#define sk_ask_master	4
#define max_sk_strings sk_ask_master
extern char *sk_strings[];

typedef longword_t skulk_status_t;
typedef longword_u skulk_status_u;

typedef struct {
    skulk_status_u	sk_status;
    ErrSet_u		sk_info;
} SkulkInfoBlk_t;
typedef bytes_u SkulkInfoBlk_u[sizeof(skulk_status_u)+sizeof(ErrSet_u)];

/*
 * CHState
 */

typedef struct {
	byte_u	cs_value;
#define	CS_newCH	 1
#define	CS_on		 2
#define	CS_dyingCH	 3
} CHState_t;
typedef bytes_u CHState_u[sizeof(byte_u)];
#define LEN_CHState(ptr) LEN_bytes(ptr,sizeof(CHState_u))
#define SKIP_CHState(ptr) SKIP_byte(ptr)
#define COPY_CHState(from,to) COPY_byte(from,to)

/*
 * Progress
 */

typedef struct {
	byte_u		pr_flags;
#define PR_done		 (1<<0)
#define PR_up		 (1<<1)
#define PR_linked	 (1<<2)
#define PR_hitLink	 (1<<3)
#define PR_ignoreState	 (1<<4)
#define PR_directory	 (1<<5)
#define PR_reserved	 (0x3<<6)
	Timeout_u	pr_timeout;
	FullName_u	pr_unresolvedName;
	FullName_u	prv_resolvedName;
	Set_u		prv_CHList;
} Progress_t;
typedef bytes_u Progress_u[sizeof(byte_u)+sizeof(Timeout_u)+2*sizeof(FullName_u)+sizeof(Set_u)];
typedef bytes_u Progress_overhead[sizeof(Progress_u) - 2*sizeof(FullName_u) - sizeof(Set_u)];
#define LEN_Progress(ptr) (SKIP_Progress(ptr)-(byte_t *)(ptr))
#define SKIP_Progress(ptr) cds_pass_Progress((const byte_t *)(ptr))
#define COPY_Progress(from,to) COPY_bytes((from),(to),LEN_Progress(from))

#define deb_ascii_Progress(b,p) deb_ascii_rec(b,p,"NULL Progress_t")
#define DEB_ASCII_LEN_Progress DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_Progress(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_Progress(nam) LOG_ASCII_BUF_rec(nam)

/*
 * Update
 */

typedef struct {
	byte_u		ud_operation;
#define UD_present	 1
#define UD_absent	 2
	Timestamp_u	ud_timestamp;
	AttributeSpecifier_u ud_attribute;
	BOOLEAN_u	udv_valuePresent;
/*	union {				*/
/*	    AtomicValue_u udx_value;	* if valuePresent = TRUE */
/*	} udx;				*/
	bytes_u		udx_union[sizeof(AtomicValue_u)];
#define udv_value udx_union
} Update_t;
typedef bytes_u Update_u[sizeof(byte_u)+sizeof(Timestamp_u)+sizeof(AttributeSpecifier_u)+sizeof(BOOLEAN_u)+sizeof(AtomicValue_u)];
typedef bytes_u Update_overhead[sizeof(Update_u) - sizeof(AttributeSpecifier_u) - sizeof(AtomicValue_u)];
#define LEN_Update(ptr) (SKIP_Update(ptr)-(byte_t *)(ptr))
#define SKIP_Update(ptr) cds_pass_Update((const byte_t *)(ptr))
#define COPY_Update(from,to) COPY_bytes((from),(to),LEN_Update(from))

#define deb_ascii_Update(b,p) deb_ascii_rec(b,p,"NULL Update_t")
#define DEB_ASCII_LEN_Update DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_Update(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_Update(nam) LOG_ASCII_BUF_rec(nam)

/*
 * UpdatePacket
 */

typedef struct {
	FullName_u	up_entryName;
	Timestamp_u	upv_entryID;
	EntryType_u	upv_entrytype;
	byte_u		upv_updatecount;
	Update_u	upv_updateset[1];
} UpdatePacket_t;
typedef bytes_u UpdatePacket_u[sizeof(FullName_u)+sizeof(Timestamp_u)+sizeof(EntryType_u)+sizeof(byte_u)+1*sizeof(Update_u)];
typedef bytes_u UpdatePacket_overhead[sizeof(UpdatePacket_u) - sizeof(FullName_u) - 1*sizeof(Update_u)];
#define LEN_UpdatePacket(ptr) (SKIP_UpdatePacket(ptr)-(byte_t *)(ptr))
#define SKIP_UpdatePacket(ptr) cds_pass_UpdatePacket((const byte_t *)(ptr))
#define COPY_UpdatePacket(from,to) \
    COPY_bytes((from),(to),LEN_UpdatePacket(from))

#define deb_ascii_UpdatePacket(b,p) deb_ascii_rec(b,p,"NULL UpdatePacket_t")
#define DEB_ASCII_LEN_UpdatePacket DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_UpdatePacket(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_UpdatePacket(nam) LOG_ASCII_BUF_rec(nam)

/*
 * AuthenticationInfo
 */

typedef struct {
    	BOOLEAN_u	ai_authenticated;
    	FullName_u	ai_principal;
} AuthenticationInfo_t;
typedef bytes_u AuthenticationInfo_u[sizeof(BOOLEAN_u)+sizeof(FullName_u)];
#define LEN_AuthenticationInfo(ptr) \
    (SKIP_AuthenticationInfo(ptr)-(byte_t *)(ptr))
#define SKIP_AuthenticationInfo(ptr) \
    SKIP_FullName(((AuthenticationInfo_t *)(ptr))->ai_principal)
#define COPY_AuthenticationInfo(from,to) \
    COPY_bytes((from),(to),LEN_AuthenticationInfo(from))

/*
 * MessageType
 */

typedef struct {
	byte_u		mt_value;
#define MT_Request	 1
#define MT_Response	 2
#define MT_Exception	 3
#define MT_Advertise	 5
} MessageType_t;
typedef bytes_u MessageType_u[sizeof(byte_u)];
#define LEN_MessageType(ptr) LEN_bytes(ptr,sizeof(MessageType_u))
#define SKIP_MessageType(ptr) SKIP_bytes((ptr),LEN_MessageType(ptr))
#define COPY_MessageType(from,to) COPY_bytes((from),(to),LEN_MessageType(from))

/*
 * TID - Timestamp
 */

#define Tid_t Timestamp_t
#define Tid_u Timestamp_u
#define LEN_Tid(ptr) LEN_Timestamp(ptr)
#define SKIP_Tid(ptr) SKIP_Timestamp(ptr)
#define COPY_Tid(from,to) COPY_Timestamp(from,to)

/*
 * Operation
 */

typedef struct {
	byte_u		op_value;
	/* Clerk/Nameserver Protocol Operation Codes */
#define OP_Local                0       /* Local client call */
#define OP_CreateCTS            0       /* Local client call */
#define OP_GetInfo              24      /* make use of unused opcode */
#define OP_EnumerateAttributes	1	/* Client Call */
#define OP_ReadAttribute	2	/* Client Call, clerk */
#define OP_ModifyAttribute	3	/* Client Call */
#define OP_TestAttribute	4	/* Client Call */
#define OP_CreateObject		5	/* Client Call */
#define OP_EnumerateObjects	6	/* Client Call */
#define OP_DeleteObject		7	/* Client Call */
#define OP_CreateDirectory	8	/* Client Call */
#define OP_DeleteDirectory	9	/* Client Call */
#define OP_AddReplica		10	/* Client Call */
#define OP_RemoveReplica	11	/* Client Call */
#define OP_EnumerateChildren	12	/* Client Call */
#define OP_Skulk		13	/* Client Call, nameserver */
#define OP_CreateLink		14	/* Client Call */
#define OP_DeleteLink		15	/* Client Call */
#define OP_ResolveName		16	/* Client Call */
#define OP_EnumerateLinks	19	/* Client Call */
#define OP_TestGroup		17	/* Client Call, nameserver */
#define OP_AllowClearinghouses	27	/* Client Call */
#define OP_DisallowClearinghouses 28	/* Client Call */
#define OP_NewEpoch		29	/* Client Call */
#define OP_GetHandle		30
#define OP_FreeHandle		31
#define OP_ClearHandle		32
#define OP_CvtOpqFullToStr	33
#define OP_CvtStrFullToOpq	34
#define OP_CvtPhIVToPrincipal	35

	/* Directory Maintenance Protocol Operation Codes */
#define OP_CreateChildEntry	18	/* Nameserver Clerk Call */
#define OP_DeleteChildEntry	20	/* Nameserver Clerk Call */
#define OP_ReadReplica		21	/* Nameserver Clerk Call */
#define OP_ModifyReplica	22	/* Nameserver Clerk Call */
#define OP_LinkReplica		23	/* Nameserver Clerk Call */
#define OP_Combine		25	/* Nameserver Clerk Call */
#define OP_DoUpdate		26	/* Nameserver Clerk Call */
} Operation_t;
typedef bytes_u Operation_u[sizeof(byte_u)];
#define LEN_Operation(ptr) LEN_bytes(ptr,sizeof(Operation_u))
#define SKIP_Operation(ptr) SKIP_bytes((ptr),LEN_Operation(ptr))
#define COPY_Operation(from,to) COPY_bytes((from),(to),LEN_Operation(from))

/*
 * protocolVersion
 */

typedef struct {
	byte_u		pv_majorVersion;
	byte_u		pv_minorVersion;
	byte_u		pv_userMajor;
	byte_u		pv_userMinor;
} protocolVersion_t;
typedef bytes_u protocolVersion_u[4*sizeof(byte_u)];
#define LEN_protocolVersion(ptr) LEN_bytes(ptr,sizeof(protocolVersion_u))
#define SKIP_protocolVersion(ptr) \
    SKIP_bytes((ptr),LEN_protocolVersion(ptr))
#define COPY_protocolVersion(from,to) \
    COPY_bytes((from),(to),LEN_protocolVersion(from))

/*
 * DatagramRequestHeader
 */

typedef struct {
	protocolVersion_u dh_versionField;
	Timestamp_u	dh_transaction;
	byte_u		dh_sequence;
	byte_u		dh_totalLength;
	MessageType_u	dh_msgType;
	Operation_u	dh_operation;
	ObjUID_u	dh_clearinghouse;
} DatagramRequestHeader_t;
typedef bytes_u DatagramRequestHeader_u[sizeof(protocolVersion_u)+sizeof(Timestamp_u)+2*sizeof(byte_u)+sizeof(MessageType_u)+sizeof(Operation_u)+sizeof(ObjUID_u)];
#define LEN_DatagramRequestHeader(ptr) LEN_bytes(ptr,sizeof(DatagramRequestHeader_u))
#define SKIP_DatagramRequestHeader(ptr) \
    SKIP_bytes((ptr),LEN_DatagramRequestHeader(ptr))
#define COPY_DatagramRequestHeader(from,to) \
    COPY_bytes((from),(to),LEN_DatagramRequestHeader(from))

/*
 * ConnectUserData
 */

typedef struct {
	protocolVersion_u cd_versionField;
} ConnectUserData_t;
typedef bytes_u ConnectUserData_u[sizeof(protocolVersion_u)];
#define LEN_ConnectUserData(ptr) LEN_bytes(ptr,sizeof(ConnectUserData_u))
#define SKIP_ConnectUserData(ptr) \
    SKIP_bytes((ptr),LEN_ConnectUserData(ptr))
#define COPY_ConnectUserData(from,to) \
    COPY_bytes((from),(to),LEN_ConnectUserData(from))

/*
 * DisconnectUserData
 */

typedef struct {
	protocolVersion_u dd_versionField;
	byte_u		dd_discReason;
#define DD_IdleShutdown	 1
#define DD_BadAuthenticator 2
#define DD_BadCredentials 3
#define DD_InsufficientResources 4
#define DD_IncompatibleProtocol 5
#define DD_ProtocolViolation 6
} DisconnectUserData_t;
typedef bytes_u DisconnectUserData_u[sizeof(protocolVersion_u)+sizeof(byte_u)];
#define LEN_DisconnectUserData(ptr) LEN_bytes(ptr,sizeof(DisconnectUserData_u))
#define SKIP_DisconnectUserData(ptr) \
    SKIP_bytes((ptr),LEN_DisconnectUserData(ptr))
#define COPY_DisconnectUserData(from,to) \
    COPY_bytes((from),(to),LEN_DisconnectUserData(from))

/*
 * VCRequestHeader
 */

typedef struct {
	Tid_u		vh_transaction;
	MessageType_u	vh_msgType;
	Operation_u	vh_operation;
	ObjUID_u	vh_clearinghouse;
} VCRequestHeader_t;
typedef bytes_u VCRequestHeader_u[sizeof(Tid_u)+sizeof(MessageType_u)+sizeof(Operation_u)+sizeof(ObjUID_u)];
#define LEN_VCRequestHeader(ptr) LEN_bytes(ptr,sizeof(VCRequestHeader_u))
#define SKIP_VCRequestHeader(ptr) \
    SKIP_bytes((ptr),LEN_VCRequestHeader(ptr))
#define COPY_VCRequestHeader(from,to) \
    COPY_bytes((from),(to),LEN_VCRequestHeader(from))

/*
 * exceptionMessage
 */

typedef struct {
	longword_u	em_exceptionValue;
#define PDU_Success                     0
#define PDU_InvalidArgument		1
#define PDU_InvalidName			2
#define PDU_NoLocalResources		3
#define PDU_NoCommunication		4
#define PDU_AccessViolation		5
#define PDU_CannotAuthenticate		6
#define PDU_ConflictingArguments	7
#define PDU_TimeoutNotDone		8
#define PDU_TimeoutMaybeDone		9
#define PDU_EntryExists			11
#define PDU_UnknownEntry		12
#define PDU_NotSupported		13
#define PDU_NotImplemented		14
#define PDU_InvalidUpdate		15
#define PDU_UnknownClearinghouse	16
#define PDU_NotAReplica			17
#define PDU_AlreadyReplica		18
#define PDU_CrucialReplica		19
#define PDU_NotEmpty			20
#define PDU_NotLinked			21
#define PDU_PossibleCycle		22
#define PDU_DanglingLink		23
#define PDU_NotAGroup			24
#define PDU_ClearinghouseDown		25
#define PDU_badEpoch			26
#define PDU_BadClock			27
#define PDU_DataCorruption		28
#define PDU_WrongAttributeType		29
#define PDU_MoreThanOneReplica		30
#define PDU_CannotPutHere		31
#define PDU_OldSkulk			32
#define PDU_UntrustedCH			33
#define PDU_VersionSkew			34
#define PDU_AmbiguousNickname		35
#define PDU_UnknownNickname		36
#define PDU_NoLocal			37
#define PDU_NotNameserver		100
#define PDU_WrongState			101
#define PDU_BadNickname			102
#define PDU_LocalOnly			103
#define PDU_NotRoot			104
#define PDU_NotCHDirectory		105
#define PDU_RootLost			106
#define PDU_CannotUpgrade		107
#define PDU_UnderSpecifiedName		108
#define PDU_UnknownAttribute		109
#define PDU_Bug				999
/*	FullName_u	em_UnknownEntry;  */
} exceptionMessage_t;
typedef bytes_u exceptionMessage_u[sizeof(longword_u)];
#define LEN_exceptionMessage(ptr) LEN_bytes(ptr,sizeof(exceptionMessage_u))
#define SKIP_exceptionMessage(ptr) \
    SKIP_bytes((ptr),LEN_exceptionMessage(ptr))
#define COPY_exceptionMessage(from,to) \
    COPY_bytes((from),(to),LEN_exceptionMessage(from))

typedef enum {Off, Initial, On, Shut, Broken} ClerkStates;

/*
 * general version data
 */

typedef struct {
	byte_u		vr_major;
	byte_u		vr_minor;
} version_t;
typedef bytes_u version_u[2*sizeof(byte_u)];
#define LEN_version(ptr) LEN_bytes(ptr,sizeof(version_u))
#define SKIP_version(ptr) SKIP_bytes((ptr),LEN_version(ptr))
#define COPY_version(from,to) COPY_bytes((from),(to),LEN_version(from))

#define deb_ascii_version(b,p) deb_ascii_rec(b,p,"NULL version_t")
#define DEB_ASCII_LEN_version DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_version(nam) DEB_ASCII_BUF_rec(nam)
#define LOG_ASCII_BUF_version(nam) LOG_ASCII_BUF_rec(nam)

/*
 * A buffer for just outputting a line to the screen.
 */
#define LOG_ASCII_LEN_line 	512
#define LOG_ASCII_BUF_line(nam) LOG_ASCII_BUF(nam,LOG_ASCII_LEN_line)
#define DEB_ASCII_LEN_line	LOG_ASCII_LEN_line
#define DEB_ASCII_BUF_line(nam) DEB_ASCII_BUF(nam,DEB_ASCII_LEN_line)


/*
 * Time comparison and arithmetic services
 */
#define Low_Time(t) EXT32(t)
#define High_Time(t) EXT32(SKIP_bytes(t,4))

#define GT_Time(t1,t2) ((High_Time(t1) > High_Time(t2)) || \
    ((High_Time(t1) == High_Time(t2)) && (Low_Time(t1) > Low_Time(t2))))
#define GE_Time(t1,t2) ((High_Time(t1) > High_Time(t2)) || \
    ((High_Time(t1) == High_Time(t2)) && (Low_Time(t1) >= Low_Time(t2))))
#define EQ_Time(t1,t2) \
    ((High_Time(t1) == High_Time(t2)) && (Low_Time(t1) == Low_Time(t2)))
#define LE_Time(t1,t2) ((High_Time(t1) < High_Time(t2)) || \
    ((High_Time(t1) == High_Time(t2)) && (Low_Time(t1) <= Low_Time(t2))))
#define LT_Time(t1,t2) ((High_Time(t1) < High_Time(t2)) || \
    ((High_Time(t1) == High_Time(t2)) && (Low_Time(t1) < Low_Time(t2))))
#define NE_Time(t1,t2) \
    ((High_Time(t1) != High_Time(t2)) || (Low_Time(t1) != Low_Time(t2)))

/* t3 = t1 + t2 */
#define ADD_Time(t1,t2,t3) dns_qadd(t1,t2,t3)

/* t3 = t1 - t2 */
#define SUB_Time(t1,t2,t3) dns_qsub(t1,t2,t3)

/* t4 = l1 * l2 + l3 */
#define MUL_Time(l1,l2,l3,t4) \
    dns_emul(l1,l2,t4);t4+=l3

/* t1 = l2 * l3 + l4 */
#define DIV_Time(t1,l2,l3,l4) \
    dns_ediv(t1,l2,l3,l4)


/*
 * dns_record.c
 */
int
cds_record_init (
    void);

byte_t *
cds_pass_NetworkAddress (
    register const byte_t    *);

byte_t *
cds_pass_AttributeSpecifier (
    const byte_t      *const);

byte_t *
cds_pass_AttributeContents (
    const byte_t      *const);

byte_t *
cds_pass_WholeEntry (
    register const byte_t *);

byte_t *
cds_pass_ReplicaPointer (
    register const byte_t    *);

byte_t *
cds_pass_Progress (
    register const byte_t    *);

byte_t *
cds_pass_Update (
    register const byte_t    *);

byte_t *
cds_pass_UpdatePacket (
    register const byte_t    *);


DNS_EXTERN utc_u Nullutc[1];
DNS_EXTERN Time_t NullTime[1];
DNS_EXTERN Timeout_t NullTimeout[1];
DNS_EXTERN Node_t NullNode[1];
DNS_EXTERN Timestamp_t NullTimestamp[1];
DNS_EXTERN NULLSIMPLENAME_t NullSimpleName[1];
DNS_EXTERN NULLSIMPLENAME_t NullBinarySimpleName[1];
DNS_EXTERN NULLSIMPLENAME_t EllipsisSimpleName[1];
DNS_EXTERN NULLFULLNAME_t NullFullName[1];
DNS_EXTERN NULLSET_t NullSet[1];
DNS_EXTERN NULLATTRIBUTE_t NullAttribute[1];
DNS_EXTERN byte_t GlobalRoot[];
DNS_EXTERN ObjUID_u cdsWildRoot[1];


int
cds_uuid_is_nil (
     uuid_t *);

unsigned int
cds_uuid_compare (
     uuid_t *,
     uuid_t *);

int
cds_uuid_equal (
     uuid_t *,
     uuid_t *);

void
cds_uuid_create_nil (
     uuid_t *);

void
cds_utc_add (
     byte_t *,
     byte_t *,
     byte_t *);

int
cds_utc_less (
     byte_t *,
     byte_t *);

int
cds_utc_greater (
     byte_t *,
     byte_t *);

int
cds_utc_equal (
     byte_t *,
     byte_t *);

/*
 * possible not used anymore 
 */

void 
format_nodeid_for_print (
     unsigned char  *,
     unsigned char  *);

void 
format_ts (
     Timestamp_t    *,
     unsigned char  *);


#endif  /* #ifndef _DNS_RECORD_H */
