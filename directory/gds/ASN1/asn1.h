/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: asn1.h,v $
 * Revision 1.2.12.2  1996/02/18  23:34:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:04  marty]
 *
 * Revision 1.2.12.1  1995/12/08  15:05:41  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/22  17:08 UTC  dtruong
 * 	Merge GDS onto HPDCE02
 * 	[1995/12/08  14:47:07  root]
 * 
 * Revision 1.2.9.4  1994/07/06  15:06:14  marrek
 * 	July 1994 code drop.
 * 	[1994/07/05  09:10:53  marrek]
 * 
 * Revision 1.2.9.3  1994/05/10  15:48:32  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:09  marrek]
 * 
 * Revision 1.2.9.2  1994/03/23  15:02:33  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:00:56  keutel]
 * 
 * Revision 1.2.9.1  1994/02/22  15:20:43  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:50:52  marrek]
 * 
 * Revision 1.2.7.2  1993/08/11  11:17:33  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:57:07  marrek]
 * 
 * Revision 1.2.5.4  1992/12/31  16:33:44  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:10  bbelch]
 * 
 * Revision 1.2.5.3  1992/11/27  16:55:30  marrek
 * 	November 1992 code drop
 * 	[1992/11/27  16:52:40  marrek]
 * 
 * Revision 1.2.5.2  1992/09/29  21:09:29  devsrc
 * 	[hinman@sni] - Check in merges from SNI
 * 	[1992/09/14  20:21:58  treff]
 * 
 * Revision 1.2.2.3  1992/06/01  20:13:50  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:12:50  zeliff]
 * 
 * Revision 1.2.2.2  1992/04/14  13:23:38  mishkin
 * 	     - Add HP-target stuff.
 * 	     - Fudge "sys/timeb.h" problem.
 * 	[1992/04/13  15:58:29  mishkin]
 * 
 * Revision 1.2  1992/01/19  22:13:00  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
#ifndef _ASN1_H
#define _ASN1_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char asn1_rcsid[] = "@(#)$RCSfile: asn1.h,v $ $Revision: 1.2.12.2 $ $Date: 1996/02/18 23:34:42 $";
#endif

/************************************************************************\
* 									 *
* Module:       ASN1   "@(#)asn1.h         1.0A05  91/10/15   MAVROS";   *
* 									 *
* Description:								 *
* This file contains data structures used to represent primitive 	 *
* asn1 types.								 *
* 									 *
* Reference:								 *
* ISO/DIS 8824   Specification of Abstract Syntax Notation One (ASN.1).	 *
* 									 *
\************************************************************************/

/**************************************************************************\
* 									   *
* CPU parametrisation							   *
* 									   *
* Depending of the CPU, we set a number of flags. These flags will direct  *
* the compilation of the ASN1 library, guaranteing the portability of the  *
* code. They are very useful for parameters like the definition of bit	   *
* string constants, and for the decoding af integers and reals.		   *
* 									   *
\**************************************************************************/
#if defined(mc68030)|| defined(mc68020)|| defined(mc68010) || defined(mc68000) || _ISP__M68K 
#define WORD 4
#define REALWORD 8
#if defined(mc68020) || defined(mc68030)
#define ANYBOUNDARY
#endif /* the old processors needed byte alignment! */
#define BIGENDIAN
#define IEEE_REAL
#else
#if defined(sparc) || defined(__pa_risc) || defined(__hppa) 
#define WORD 4
#define REALWORD 8
#define BIGENDIAN
#define IEEE_REAL
#else
#if defined(gould)
#define WORD 4
#define REALWORD 8
#define BIGENDIAN
#else 
#if defined(_IBMR2)
#define WORD 4
#define REALWORD 8
#define BIGENDIAN
#define ASN1_CHAR_IS_UNSIGNED
#else
#if defined(vax)
#define WORD 4
#define REALWORD 8
#define ANYBOUNDARY
#else
#if defined(ns16000) || defined(ns32000) || defined(nsc32000)
#define WORD 4
#define REALWORD 8
#else
#if defined(__i386__) || defined(i386)   /* MK 27.9.90 */
#define WORD 4
#define REALWORD 8
#define sinix		/* need to find a better way!!! */
#else
#if defined(mips)
#ifndef MIPSEL
#define BIGENDIAN
#define IEEE_REAL
#endif
#define WORD 4
#define REALWORD 8
#else
#if defined(__alpha)
#define WORD 8
#define REALWORD 8
#endif /* __alpha */
#endif /* mips */
#endif /* i386 */
#endif /* ns */
#endif /* vax */
#endif /* _IBMR2 */
#endif /* gould */
#endif /* sparc  */
#endif /* mc68020 */


#if defined(sun) || defined(_AIX) || defined(ultrix) || defined(bsd4_2) || defined(__OSF__)
#undef SYS5
#ifndef BERKELEY
#define BERKELEY
#endif
#endif

#ifdef sinix
#undef BERKELEY
#ifndef SYS5
#define SYS5
#endif
#endif

#ifndef CAST_FOR_SWITCH
#define CAST_FOR_SWITCH(x) ((int)(x))
#endif

#if (defined(sm90) || defined(sps7)) /* BULL DPX1000, DPX2000 */
#undef BERKELEY
#ifndef SYS5
#define SYS5
#endif
#endif


#if defined(STUPID_REAL) && defined(IEEE_REAL)
#undef IEEE_REAL
#endif /* this flag is only used for test programs */

/*************************\
* 			  *
* System parametrisation  *
* 			  *
\*************************/


#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/timeb.h>

#ifndef EOF
#include <stdio.h>
#endif /* EOF */

#define ASN1_MOVE(src,dst,len)	(void)memcpy((char *)(dst),(char *)(src),len)
#define ASN1_ZERO(dst,len)	(void)memset((char *)dst,0,len)
#define ASN1_CMP(src,dst,len)	memcmp((char *)(src),(char *)(dst),len)
#define ASN1_INDEX(x,c)		strchr(x,c)

#ifdef BERKELEY
#define ASN1_SWAP_SHORT(s)	(ntohs((short)(s)))
#define ASN1_SWAP_LONG(l)	(ntohl((long)(l)))
#endif

#ifndef ASN1_SWAP_LONG
#define ASN1_SWAP_LONG(l)	asn1_swap_long(l)
long asn1_swap_long (long);
#endif
#ifndef ASN1_SWAP_SHORT
#define ASN1_SWAP_SHORT(l)	asn1_swap_short(l)
short asn1_swap_short (short);
#endif

/**************************************\
* 				       *
* Macros for the thread safe library.  *
* 				       *
\**************************************/
#ifdef OSF_DCE
#include <krb5/osf_dce.h>
#ifndef ASN1_THREADED
#define ASN1_THREADED
#endif
#define ASN1_MALLOC_NAME(len)		malloc(len)
#define ASN1_FREE_NAME(add)		free(add)
#define ASN1_REALLOC_NAME(add,len) 	realloc(add,len)
#define ASN1_SEM_DECLARE(xx)		static pthread_mutex_t xx
#define ASN1_SEM_CREATE(xx,yy)		pthread_mutex_init((xx),(yy))
#define ASN1_SEM_LOCK(xx)		pthread_mutex_lock(xx)
#define ASN1_SEM_UNLOCK(xx)		pthread_mutex_unlock(xx)
#else
#define ASN1_MALLOC_NAME(len)		malloc(len)
#define ASN1_FREE_NAME(add)		free(add)
#define ASN1_REALLOC_NAME(add,len)	realloc(add,len)
#ifdef ASN1_THREADED /* This is used for testing the calls to lock.. */
#define ASN1_SEM_DECLARE(xx)		int xx
#define ASN1_SEM_CREATE(xx)  		xx = 0
#define ASN1_SEM_LOCK(xx)	if (xx++)\
				printf("\nSEM.LOCK: DEADLOCK %d.\n",xx);
#define ASN1_SEM_UNLOCK(xx)	if (--xx)\
				printf("\nSEM.UNLOCK: DEADLOCK %d.\n",xx);
#endif /* !ASN1_THREADED */
#endif /* !OSF_DCE */



/**********************\
* 		       *
*  Some useful types   *
* 		       *
\**********************/

typedef unsigned char asn1_octet, *asn1;

typedef struct {
	int	l;
	asn1	v;
} asn1_field;

typedef void * asn1_opaque;

typedef struct {
	int	type_id;
	void *	data;
} asn1_external;

typedef char * asn1_string;
typedef asn1_field asn1_oid;
#define asn1_octets_cmp(a,b) 	asn1_field_cmp(a,b)
#define asn1_oid_cmp(a,b) 	asn1_field_cmp(a,b)

/*******************************\
* 			        *
*  CODING AND DECODING MACROS   *
* 			        *
\*******************************/
#define asn1_type(z,m,n,x)	((m == -1)?(z=x,z+1):asn1_type_cod(z,m,n,0))

#define asn1_boolean_len(n,x)	(4)
#define asn1_boolean_free(x)	(0)
#define asn1_boolean_err(x)	
#define asn1_boolean_cpy(x,y)	(*x = y)
#define asn1_boolean_olen(x)	(5)

#define asn1_int_len(n,x)	(8)
#define asn1_int_dec(z1,z2,x)	asn1_intdec(asn1_skiptype(z1,z2),z2,x)
#define asn1_int_err(x)		
#define asn1_int_free(x)	(0)
#define asn1_int_cpy(x,y)	(*x = y)
#define asn1_int_olen(x)	(10)

#define asn1_real_len(n,x)	(12)
#define asn1_real_dec(z1,z2,x)	asn1_realdec(asn1_skiptype(z1,z2),z2,x)
#define asn1_real_free(x)	(0)
#define asn1_real_err(x)	(0)
#define asn1_real_cpy(x,y)	(*x = y)
#define asn1_real_olen(x)       (19)

#define asn1_bits_len(n,x,l)	(8+((l)+7)>>3)
#define asn1_bits_free(x,l)	(asn1_alloc_free((char *)x,1))
#define asn1_bits_err(x,l)	(*x = 0, *l = 0)

#define asn1_flags_len(n,x,l)	(8)
#define asn1_flags_err(x,l)	(0)
#define asn1_flags_free(x,y)	(0)
#define asn1_flags_cpy(x,lx,y,ly)	(*x = y)
#define asn1_flags_olen(x,lx)	(sizeof(int)+3)

#define asn1_octets_len(n,x,l)	(l + 8)
#define asn1_octets_free(x,l)	(asn1_alloc_free((char *)x,1))
#define asn1_octets_err(x,l)	(*x=0, *l=0)

#define asn1_oid_len		asn1_octets_len
#define asn1_oid_vcod		asn1_octets_vcod
#define asn1_oid_dec		asn1_octets_dec
#define asn1_oid_free		asn1_octets_free
#define asn1_oid_lwc		asn1_octets_lwc
#define asn1_oid_lwd		asn1_octets_lwd
#define asn1_oid_err		asn1_octets_err
#define asn1_oid_cpy		asn1_octets_cpy

#define asn1_chars_cod(z,m,n,x)	asn1_octets_cod(z,m,n,(asn1)x,strlen(x))
#define asn1_chars_len(n,x)	(strlen(x)+8)
#define asn1_chars_free(x)	(asn1_alloc_free(x,1))
#define asn1_chars_lwc(z,x)	asn1_octets_lwc(z,(asn1)x,strlen(x))
#define asn1_chars_err(x)	(*x = 0)

#define asn1_null_len(n)	(4)
#define asn1_null_free(x,y)	(0)
#define asn1_null_lwc(z)	(z)
#define asn1_null_lwd(z,zm)	(z)
#define asn1_null_err()		
#define asn1_null_cpy()		(0)
#define asn1_null_olen()	(4)

#define asn1_any_cod(z,m,n,x,l)	(ASN1_MOVE(x,z,l),z+l)
#define asn1_any_len(n,x,l)	(l)
#define asn1_any_free(x,l)	(asn1_alloc_free((char *)x,1))
#define asn1_any_lwc(z,x,l)	asn1_octets_lwc(z,x,l)
#define asn1_any_lwd(z,zm,x,l)	asn1_octets_lwd(z,zm,x,l)
#define asn1_any_err(x,l)	(*x=0, *l=0)
#define asn1_any_cpy(a,b,c,d)	asn1_octets_cpy(a,b,c,d)
#define asn1_any_in		asn1_octets_in
#define asn1_any_out		asn1_octets_out
#define asn1_any_olen		asn1_octets_olen

#define asn1_copy_len		asn1_any_len
#define asn1_copy_vcod		asn1_any_vcod
#define asn1_copy_dec		asn1_any_dec
#define asn1_copy_free		asn1_any_free
#define asn1_copy_lwc		asn1_any_lwc
#define asn1_copy_lwd		asn1_any_lwd
#define asn1_copy_err		asn1_any_err
#define asn1_copy_cpy		asn1_any_cpy
#define asn1_copy_in		asn1_any_in
#define asn1_copy_out		asn1_any_out
#define asn1_copy_olen		asn1_any_olen

/*
 * dce/nbase.h cannot be used for unsigned32, because it is created
 * after the setup_all build phase
 */
#if defined(__alpha)
#define ASN1_X509		((unsigned int)1<<31)
#else
#define ASN1_X509		((unsigned long)1<<31)
#endif /* __alpha */

/***********************************\
* 				    *
*  CODING AND DECODING PROCEDURES   *
* 				    *
\***********************************/
  asn1 
	asn1_binmov (asn1,asn1, asn1, int *),
	asn1_bitcod (asn1, asn1, int),
	asn1_bits_cod (asn1, int, int, asn1, int),
	asn1_bits_dec (asn1, asn1, asn1 *, int *),
	asn1_bits_lwc (asn1, asn1, int),
	asn1_bits_lwd (asn1, asn1, asn1 *, int *),
	asn1_boolean_cod (asn1, int, int, int),
	asn1_boolean_dec (asn1, asn1, int *),
	asn1_boolean_lwc (asn1, int),
	asn1_boolean_lwd (asn1, asn1, int *),
	asn1_chars_dec (asn1, asn1, char *),
	asn1_chars_lwd (asn1, asn1, char *),
	asn1_check (asn1, asn1),
	asn1_close (asn1, asn1, asn1),
	asn1_dump (FILE *, asn1, asn1),
	asn1_errdec (asn1, int),
	asn1_flagcod (asn1, int, int),
	asn1_flags_cod (asn1, int, int, int, int),
	asn1_flags_dec (asn1, asn1, int *, int),
	asn1_flags_lwc (asn1, asn1, int),
	asn1_flags_lwd (asn1, asn1, int *, int),
	asn1_int_cod (asn1, int, int, int),
	asn1_int_lwc (asn1, int),
	asn1_int_lwd (asn1, asn1, int *),
	asn1_intcod (asn1, int),
	asn1_intdec (asn1, asn1, int *),
	asn1_lencod (asn1, int),
	asn1_lendec (asn1, asn1, int *),
	asn1_length (asn1, asn1, asn1 *),
	asn1_move (asn1, asn1, asn1),
	asn1_null_cod (asn1, int, int),
	asn1_null_dec (asn1, asn1),
	asn1_octets_cod (asn1, int, int, asn1, int),
	asn1_octets_dec (asn1, asn1, asn1 *, int *),
	asn1_octets_lwc (asn1, asn1, int),
	asn1_octets_lwd (asn1, asn1, asn1 *, int *),
	asn1_octmov (asn1, asn1, asn1, int *),
	asn1_octpak (asn1, asn1, asn1),
	asn1_skip (asn1, asn1),
	asn1_skiptype (asn1, asn1),
	asn1_strmov (asn1, asn1, char *),
	asn1_stroct (asn1, asn1, asn1*, int *, int *),
	asn1_type_cod (asn1, int, int, int),
	asn1_unstack (asn1 *, int),
	asn1_realcod (asn1, double);
  
double	asn1_realval (asn1, asn1);
asn1	asn1_real_cod  (asn1, int, int, double);
asn1	asn1_real_lwc  (asn1, double);
asn1	asn1_real_lwd  (asn1, asn1, double *);

#define asn1_malloc(l)	asn1_alloc_malloc(l,(char *)0)
#define asn1_free(x)	asn1_alloc_free((char *)(x),1)
char *	asn1_alloc_malloc (unsigned int, char *);
char *	asn1_realloc (char *, unsigned int);
void	asn1_alloc_free (char *, int);
int	asn1_alloc_snap_shot  (char ***);

/***************************************\
* 				        *
*  Macros for the light weight syntax   *
* 				        *
\***************************************/
#ifdef ANYBOUNDARY
#ifdef BIGENDIAN
#define ASN1_LWINT(z) (*((int *)(z)))
#else
#define ASN1_LWINT(z) ASN1_SWAP_LONG(*((int *)(z)))
#endif
#else
#define ASN1_LWINT(z) ((((((z[0]<<8)+z[1])<<8)+z[2])<<8)+z[3])
#endif

#define asn1_skiplw(z) ((z) + ASN1_LWINT(z) + WORD)

#ifdef ANYBOUNDARY
#ifdef BIGENDIAN
#define ASN1_INTLW(z,i) (*((int *)(z)) = i)
#else
#define ASN1_INTLW(z,i) (*((int *)(z)) = htonl(i))
#endif
#else
#define ASN1_INTLW(z,i) (\
	z[0] = (i)>>24,	z[1] = (i)>>16,\
	z[2] = (i)>>8,	z[3] = (i),	(i))
#endif


#ifdef ANYBOUNDARY
#define ASN1_LW0(z) (*((int *)(z)) = 0, z+= WORD)
#else
#define ASN1_LW0(z) (*z++=0,*z++=0,*z++=0,*z++=0)
#endif

#if defined(IEEE_REAL) && defined(REAL_ALIGNMENT)
#define ASN1_REALLW(z,v) (*(double *)(z) = (v))
#define ASN1_LWREAL(z)   (*(double *)(z))
#else
#define ASN1_REALLW(z,v) asn1_reallw(z,(double)v)
#define ASN1_LWREAL(z)   asn1_lwreal(z)
void	asn1_reallw (asn1, double);
double	asn1_lwreal (asn1);
#endif

int     * asn1_real_ftcd	(int *,int **, double);
int 	* asn1_real_ftdc	(int *,int **, double *);
double	asn1_real_ftdec		(int *);

/********************************************************************\
* 								     *
* The following definitions are used by the type checking procedures *
* 								     *
\********************************************************************/
extern int 	asn1_diagnostic;
extern asn1	asn1_wrongbyte;
char * asn1_errmes (int);

#define ASN1_CKERR(x,z)		(asn1_errdec(z,x))
#define ASN1_ERR_IN(z,zm,diag)	((char *) asn1_errdec((asn1)(z),diag))
#define asn1_err_in(z,diag)	((char *) asn1_errdec((asn1)(z),diag))
#define asn1_err_ftlws(z,diag)	((int *) asn1_errdec((asn1)(z),diag))

#define ASN1_ERR_TAG	1	/* Tag length > limit */
#define ASN1_ERR_LLEN	2	/* Length's length > limit */
#define ASN1_ERR_LEN	3	/* Length > limit */
#define ASN1_ERR_SHORT	4	/* Value too short */
#define ASN1_ERR_EOC	5	/* Incorrect EOC format */
#define ASN1_ERR_BOUND 	6	/* EOC missing before limit */
#define ASN1_ERR_LDEF 	7	/* Undefined form for primitive */
#define ASN1_ERR_STRUCT	8	/* Structured form for primitive */
#define ASN1_ERR_MISS	9	/* Mandatory component missing */
#define ASN1_ERR_CHOICE	10	/* Incorrect choice component */
#define ASN1_ERR_BOOL	11	/* Incorrect boolean */
#define ASN1_ERR_NULL	12	/* Incorrect null component */
#define ASN1_TEXT_EXISTS 13	/* Object ID name already declared */
#define ASN1_OID_EXISTS	14	/* Object ID already declared */
#define ASN1_OIDEMPTY	15	/* empty object identifier */
#define ASN1_OIDBADTOP	16	/* bad top level object id */
#define ASN1_OIDUNDEF	17	/* undefined object identifier */
#define ASN1_OIDMISSSEC	18	/* missing second level component */
#define ASN1_OIDSECBIG	19	/* second component > 39 */
#define ASN1_OIDBADSEC	20	/* incorrect second component */
#define ASN1_OIDNUMCOMP	21	/* final components must be numeric */

#define ASN1_ERR_EQUAL	22	/* missing equal sign */
#define ASN1_ERR_OBRACKET 23	/* missing opening bracket */
#define ASN1_ERR_CBRACKET 24	/* missing closing bracket */
#define ASN1_ERR_HEXA	25	/* incorrect hexadecimal digit */
#define ASN1_ERR_ST_END	26	/* input end before end of string */
#define ASN1_ERR_MALLOC	27	/* cannot allocate memory */
#define ASN1_ERR_B_OR_H	28	/* string type must be B or H */
#define ASN1_ERR_BSTRING 29	/* incorrect binary string */
#define ASN1_ERR_STRING 30	/* string component expected */
#define ASN1_ERR_HSTRING 31	/* hexa string should be typed H */
#define ASN1_ERR_ST_EOL	32	/* end of line before end of string */
#define ASN1_ERR_FLAGS	33	/* unexpected bit string element */
#define ASN1_ERR_ABSURD 34	/* absurd data type */


/***************************************\
* 				        *
*  definitions for the coding control   *
* 				        *
\***************************************/

extern asn1 (*asn1_end) (asn1, asn1);
extern asn1 (*asn1_end_set) (asn1, asn1);

/****************************************\
* 					 *
* Definitions for the OBJECT ID library	 *
* 					 *
\****************************************/
asn1_oid 
	asn1_oid_get  (char *, int *),
	asn1_oid_copy (asn1_oid);
char * 	asn1_oid_put  (char *, asn1_oid);
char * 	asn1_oid_xput (char *, asn1_oid, int);

#define ASN1_OID_CCITT 		"ccitt"
#define ASN1_OID_ISO 		"iso"
#define ASN1_OID_ISO_CCITT 	"joint-iso-ccitt"

#define ASN1_OID_STANDARD	"standard"
#define ASN1_OID_REG_AUT		"registration-authority"
#define ASN1_OID_MEMBER		"member-body"
#define ASN1_OID_ID_ORG		"identified-organization"

#define ASN1_OID_RECOM		"recommendation"
#define ASN1_OID_QUESTION	"question"
#define ASN1_OID_ADMIN		"administration"
#define ASN1_OID_NET_OP		"network-operator"

/********************************************************\
* 							 *
* Macros and definitions for the input output routines.	 *
* 							 *
\********************************************************/
char
	*asn1_output_indent (char *, int),
	*asn1_output_open_bracket (char *),
	*asn1_skip_equal_sign (char *, char *),
	*asn1_skip_input (char *, char *),
	*asn1_skip_if_match (char *, char *, char *),
	*asn1_skip_input_and_count (char *, char *, int *),
	*asn1_skip_input_spaces (char *, char *), 
	*asn1_skip_input_string (char *, char *),
	*asn1_skip_input_separator (char *, char *), 
	*asn1_skip_input_hexa_string (char *, char *),
	*asn1_skip_input_quoted_string (char *, char *),
	*asn1_open_input (char *, char *),
	*asn1_close_input (char *, char *),
	*asn1_int_in (char *, char *, int *),
	*asn1_int_out (char *, int), 
	*asn1_boolean_in (char *, char *, int *),
	*asn1_boolean_out (char *, int),
	*asn1_real_in (char *, char *, double *),
	*asn1_real_out (char *,double),
	*asn1_null_in (char *, char *),
	*asn1_null_out (char *),
	*asn1_octets_in (char *, char *, asn1 *, int *),
	*asn1_octets_out (char *, asn1, int),
	*asn1_chars_in (char *, char *, char **),
	*asn1_chars_out (char *, char *),
	*asn1_oid_in (char *, char *, asn1 *, int *),
	*asn1_oid_out (char *, asn1, int),
	*asn1_bits_in (char *, char *, asn1 *, int *),
	*asn1_bits_out (char *, asn1, int),
	*asn1_flags_in (char *, char *, int *, int),
	*asn1_flags_out (char *, int, int);
  
int	asn1_input_number (char *, char *);
int	asn1_int_v_in (char *, char *, char **);
  
double asn1_real_v_in (char *, char *, char **);

extern int asn1_output_indentation;
extern int asn1_output_indent_len;

#define ASN1_OPENING_BRACKET	'<'
#define ASN1_CLOSING_BRACKET	'>'
#define ASN1_SEPARATOR		';'
#define ASN1_EQUAL_CHAR		'='

#define ASN1_OPEN_BRACKET(z)	(z = asn1_output_open_bracket(z))
#define ASN1_OPEN_BRACKET_CHOICE(z)	(asn1_output_indentation++,\
				 *z++ = ASN1_OPENING_BRACKET, *z++ = ' ')
#define ASN1_CLOSE_BRACKET(z)	(*z++ = ASN1_CLOSING_BRACKET,\
				asn1_output_indentation--)
#define ASN1_BRACKET_CL_LEN	(asn1_output_indent_len--, 1)
#define ASN1_BRACKET_OP_LEN	(asn1_output_indent_len++,\
				asn1_output_indent_len)
#define ASN1_BRACKET_LEN	(2)
#define ASN1_SEPARATE_LEN	(asn1_output_indent_len)
#define ASN1_SEPARATE(z)	(z = asn1_output_indent(z,ASN1_SEPARATOR))
#define ASN1_EQUAL_SIGN(z)	(*z++ = ASN1_EQUAL_CHAR, *z++ = ' ')
#define asn1_false		"FALSE"
#define asn1_true		"TRUE"
#define asn1_null		"NULL"

/*************************************\
* 				      *
* Support of the ANY DEFINED BY type  *
* 				      *
\*************************************/
typedef asn1 	(*asn1_decfunc) (asn1, asn1, void **);
typedef void 	(*asn1_errfunc) (void **x);
typedef void	(*asn1_freefunc) (void *x);
typedef asn1 	(*asn1_codfunc) (asn1, int, int, void *);
typedef int	(*asn1_lenfunc) (int, void *);
typedef asn1	(*asn1_lwdfunc) (asn1, asn1, void **);
typedef asn1	(*asn1_lwcfunc) (asn1, void *);
typedef asn1_string (*asn1_infunc) (asn1, asn1, void **);
typedef asn1_string (*asn1_outfunc) (asn1_string, void *);
typedef int	(*asn1_olenfunc) (void *);
typedef void	(*asn1_cpyfunc) (void **x, void *y);

typedef struct asn1_type_desc {
	asn1_codfunc	e_cod;
	asn1_lenfunc	t_len;
	asn1_decfunc	t_dec;
	asn1_lwcfunc	t_lwc;
	asn1_lwdfunc	t_lwd;
	asn1_outfunc	t_out;
	asn1_olenfunc	t_olen;
	asn1_infunc	t_in;
	asn1_freefunc	t_free;
	asn1_errfunc	t_err;
	asn1_cpyfunc	t_cpy;
} asn1_type_desc;

/***************************\
* 			    *
* Miscellaneous functions.  *
* 			    *
\***************************/

asn1 asn1_copy (asn1, int, int, asn1, int);
int asn1_number  (asn1, asn1);
int asn1_size  (asn1, asn1);
asn1 asn1_any_dec  (asn1, asn1, asn1 *, int *);
int asn1_load  (asn1 *, asn1 *, FILE *, FILE *, char *, int);
int asn1_load_item (int);
void asn1_load_memory  (int);
int asn1_load_tag (int);
void asn1_load_value  (asn1 *, int *, int);
int asn1_load_blank (void);
int asn1_load_comment (void);
void asn1_load_mess (char *);
void asn1_load_error  (int, char *);
int asn1_field_cmp  (asn1_field *, asn1_field *);
int asn1_bits_cmp  (asn1, int, asn1, int);
int asn1_flags_cmp  (int *, int, int *, int);
int asn1_fetch (FILE *, asn1 *, int *, int);
asn1 asn1_strdec (asn1, asn1, char *, int *);
int asn1_oid_int  (int, asn1, unsigned long *);
asn1 asn1_int_oid (asn1, unsigned long *, int);
asn1 asn1_regular_end  (asn1, asn1);
void asn1_std_mode_on (void);
asn1 asn1_defined_end  (asn1, asn1);
asn1 asn1_unique_set  (asn1, asn1);
void asn1_x509_mode_on  (void);
void asn1_output_indent_reset (void);
int asn1_count_size_of_line  (char *);
int asn1_inside_bracket  (char *);
int asn1_compare_name  (char *, char *);
int asn1_input_size  (char *, char *);
char * asn1_hexa_out ( char*, asn1, int);
int asn1_hexa_digit_in  (int, int *, int *);
char *asn1_hexa_in  (char *, char *, asn1 *, int *, int *, int *);
char * asn1_input_qstring  (char *, char *, asn1 *, int *);
char *asn1_binary_out  (char *, asn1, int);
int asn1_bits_olen  (asn1, int);
int asn1_octets_olen  (asn1, int);
int asn1_chars_olen  (char *);
int asn1_oid_olen  (asn1, int);
int asn1_boolean_v_in (char *, char *, char **);
void asn1_octets_cpy  (asn1 *, int *, asn1, int);
void asn1_bits_cpy  (asn1 *, int *, asn1, int);
void asn1_chars_cpy  (char **, char *);
int asn1_load_text  (char **, char **, FILE *, FILE *, char *,int);
asn1 asn1_absurd_cod  (asn1, int, int, void *);
int asn1_absurd_len  (int, void *);
asn1 asn1_absurd_dec  (asn1, asn1, void **);
asn1 asn1_absurd_lwc  (asn1, void *);
asn1 asn1_absurd_lwd  (asn1, asn1, void **);
asn1_string asn1_absurd_out  (asn1_string, void *);
int asn1_absurd_olen (void *);
asn1_string asn1_absurd_in  (asn1, asn1, void **);
void asn1_absurd_free (void *x);
void asn1_absurd_err (void **x);
void asn1_absurd_cpy (void **x, void *y);
int asn1_oid_decl  (char *, asn1_oid);
int look_for_oid  (asn1_oid oid);
int look_for_oid_part  (asn1_oid, int *);
int one_digit_more  (int *, int, int, int, int);
char * copy_to_blank (char *, char *);
char *numeric_part  (char *);

int asn1_utc_time  (char *, struct timeb *);
int asn1_gen_time  (char *, struct timeb *);
int asn1_time  (char *, int, struct timeb *);
int asn1_time_gen  (char *, int, struct timeb *);

#endif /* _ASN1_H */
