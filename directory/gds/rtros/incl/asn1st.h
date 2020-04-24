/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: asn1st.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:27  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:26  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:33:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:54  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:26  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:02  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:01  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:56:32  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:59  keutel
 * 	creation
 * 	[1994/03/21  13:26:05  keutel]
 * 
 * $EndLog$
 */
#ifndef _ASN1ST_H
#define _ASN1ST_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _asn1st_rcsid[] = { "@(#) $RCSfile: asn1st.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:27 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*   COPYRIGHT  (C)  1990 BY SIEMENS NIXDORF INFORMATIONSSYSTEME AG   */
/*                           AND INRIA                                */
/*   All rights reserved                                              */
/*                                                                    */
/*   This software is furnished under licence and may be used only    */
/*   in accordance with the terms of that licence and with the        */
/*   inclusion of the above copyright notice.                         */
/*   This software may not be provided or otherwise made available    */
/*   to, or used by, any other person. No title to or ownership of    */
/*   the software is hereby  transferred.                             */
/*                                                                    */
/* ****************************************************************** */

	 /*********************************
	   *****  ###### Temporary inclusion for  DOS compilation
	 *********************************/
#ifndef __STDC__
#define __STDC__
#endif
/************************************************************************\
* asn1st.h, 91/10/15                                                     *
*                                                                        *
* Module:       ASN1ST                                                   *
*                                                                        *
* Description:                                                           *
* This file contains data structures used to represent primitive         *
* asn1 types.                                                            *
*                                                                        *
* Reference:                                                             *
* ISO/DIS 8824   Specification of Abstract Syntax Notation One (ASN.1).  *
*                                                                        *
*                                                                        *
* HISTORY      : sccsid  =  @(#)asn1st.h	1.2 94/01/26                                      *
*                                                                        *
* Vers.Nr.  |Date    |  changes                       |mark | CR# EM#    *
* 1.0      0|92-01-17|  original                      |gj,ht|            *
*           |        |                                |     |            *
\************************************************************************/

/**************************************************************************\
*                                                                          *
* CPU parametrisation                                                      *
*                                                                          *
* Depending of the CPU, we set a number of flags. These flags will direct  *
* the compilation of the ASN1 library, guaranteing the portability of the  *
* code. They are very useful for parameters like the definition of bit     *
* string constants, and for the decoding af integers and reals.            *
*                                                                          *
\**************************************************************************/


#if defined(mc68030)|| defined(mc68020)|| defined(mc68010) || defined(mc68000)
#define WORD 4
#define REALWORD 8
#if defined(mc68020) || defined(mc68030)
#define ANYBOUNDARY
#endif /* the old processors needed byte alignment! */
#define BIGENDIAN
#define IEEE_REAL
#else
#if defined(sparc)
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
#if defined(hppa)  || defined (_HPUX_SOURCE)
   /* PA RISC */
#ifndef hppa
#define hppa /* hppa is used also at other places */
#endif
#define ASN1_WORD_SZ 4
#define ASN1_REALWORD 8
#define BIGENDIAN
#define IEEE_REAL
#define ASN1_LONG_MASK 0xFFFF
#define ASN1_LONG_SHIFT 16
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
#if defined(i386)  || defined(i80386)  
#define WORD 4
#define REALWORD 8
#else
#if defined(mips)
#ifndef MIPSEL
#define BIGENDIAN
#define IEEE_REAL
#endif
#define WORD 4
#define REALWORD 8
#endif /* mips */
#endif /* i386 */
#endif /* ns */
#endif /* vax */
#endif /* hppa */
#endif /* _IBMR2 */
#endif /* gould */
#endif /* sparc  */
#endif /* mc68020 */


#ifdef R_G000_Windows  /* PORTING ON WINDOWS */
/* #include <windows.h>     */
#endif /* WINDOWS */

#if defined(sun) || defined(_AIX) || defined(ultrix) || defined(bsd4_2)
#undef SYS5
#ifndef BERKELEY
#define BERKELEY
#endif
#endif

#if defined(sinix) && defined(BERKELEY)
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
*                         *
* System parametrisation  *
*                         *
\*************************/


#if defined(__STDC__)
#include <stdlib.h>
#else
#ifdef SYS5
#include <string.h>
#ifndef M_KEEP
#include <malloc.h>
#endif /* M_KEEP */
#else
#ifdef BERKELEY
#include <strings.h>
#else
char * index(), * rindex(), * strncpy(), * strcpy();
#endif /* !BERKELEY */
#ifdef _AIX
#include <malloc.h>
#else
char * sprintf(), * malloc(), * realloc();
long strtol();
#endif  /* _AIX */
#endif /* !SYS5 */
#endif /* ! __STDC__ */

#ifndef VERY_STRANGE_PORT_OF_UNIX
#ifndef ultrix
#include <sys/types.h>
#endif
#include <sys/timeb.h>
#define ASN1_TIMEB_DEFINED
#endif

#ifndef EOF
#include <stdio.h>
#endif /* EOF */


#if defined(SYS5) || defined(__STDC__) || defined(sinix)
#include <routil0.h>
#ifdef R_G000_Windows  /* PORTING ON WINDOWS */
#define ASN1_MOVE(src,dst,len)  _hmemcpy((char HUGE *)(dst),(char HUGE  *)(src),len)
#define ASN1_CMP(src,dst,len)   ro0042_memcmp((char FAR *)(src),(char FAR *)(dst),(unsigned long int) len)
#define ASN1_ZERO(dst,len)      ro0043_memset((char FAR *)dst,0,(size_t)len)
#define ASN1_INDEX(x,c)         strchr((const char FAR *)x,(int)c)
#define ASN1_RINDEX(x,c)        strrchr((const char FAR *)x,(int)c)  
#else    /* R_G000_Windows */
#define ASN1_MOVE(src,dst,len)  (void)memcpy((char FAR *)(dst),(char FAR *)(src),len)
#define ASN1_ZERO(dst,len)      (void)ro0043_memset((char FAR *)dst,0,len)
#define ASN1_CMP(src,dst,len) ro0042_memcmp ((char FAR *)(src),(char FAR *)(dst),len)
#define ASN1_INDEX(x,c)         strchr(x,c)
#endif /* R_G000_Winndows */
#else
#define ASN1_INDEX(x,c)         index(x,c)
#ifdef BERKELEY
#define ASN1_MOVE(src,dst,len)  (void)bcopy((char FAR *)(src),(char FAR *)(dst),len)
#define ASN1_ZERO(dst,len)      (void)bzero((char FAR *)(dst),len)
#define ASN1_CMP(src,dst,len)   bcmp((char FAR *)(src),(char FAR *)(dst),len)
#else
#define ASN1_MOVE(src,dst,len)  asn1_bcopy((char FAR *)(src),(char FAR *)(dst),len)
#define ASN1_ZERO(dst,len)      asn1_bzero((char FAR *)(dst),len)
#define ASN1_CMP(src,dst,len)   asn1_bcmp((char FAR *)(src),(char FAR *)(dst),len)
void asn1_bcopy PROTO((char FAR *, char FAR *, int));
void asn1_bzero PROTO((char FAR *, int));
int  asn1_bcmp  PROTO((char FAR *, char FAR *, int));
#endif
#endif

#ifdef BERKELEY
#define ASN1_SWAP_SHORT(s)      (ntohs((short)(s)))
#define ASN1_SWAP_LONG(l)       (ntohl((long)(l)))
#endif

#ifndef ASN1_SWAP_LONG
#define ASN1_SWAP_LONG(l)       asn1_swap_long(l)
long asn1_swap_long PROTO((long));
#endif
#ifndef ASN1_SWAP_SHORT
#define ASN1_SWAP_SHORT(l)      asn1_swap_short(l)
short asn1_swap_short PROTO((short));
#endif

#ifdef BIGENDIAN
#define ASN1_BIT_CONSTANT(x,y) (1<<((8*sizeof(int))-(x)))
#else
#define ASN1_BIT_CONSTANT(x,y) (1<<(y))
#endif

/**************************************\
*                                      *
* Macros for the thread safe library.  *
*                                      *
\**************************************/
#ifdef OSF_DCE
#include <krb5/osf_dce.h>
#ifndef ASN1_THREADED
#define ASN1_THREADED
#endif
#define ASN1_MALLOC_NAME(len)           cma_malloc(len)
#define ASN1_FREE_NAME(add)             cma_free(add)
#define ASN1_REALLOC_NAME(add,len)      cma_realloc(add,len)
#define ASN1_SEM_DECLARE(xx)            static cma_t_mutex xx
#define ASN1_SEM_CREATE(xx)     {       cma_t_attr att; \
					cma_mutex_create(&(xx),(yy));}
#define ASN1_SEM_LOCK(xx)               cma_mutex_lock(xx)
#define ASN1_SEM_UNLOCK(xx)             cma_mutex_unlock(xx)
#else
#define ASN1_MALLOC_NAME(len)           malloc(len)
#define ASN1_FREE_NAME(add)             free(add)
#define ASN1_REALLOC_NAME(add,len)      realloc(add,len)
#ifdef ASN1_THREADED /* This is used for testing the calls to lock.. */
#define ASN1_SEM_DECLARE(xx)            int xx
#define ASN1_SEM_CREATE(xx)             xx = 0
#define ASN1_SEM_LOCK(xx)       if (xx++)\
				printf("\nSEM.LOCK: DEADLOCK %d.\n",xx);
#define ASN1_SEM_UNLOCK(xx)     if (--xx)\
				printf("\nSEM.UNLOCK: DEADLOCK %d.\n",xx);
#endif /* !ASN1_THREADED */
#endif /* !OSF_DCE */




/**********************\
*                      *
*  Some useful types   *
*                      *
\**********************/

typedef unsigned long STOREHANDLE;


typedef unsigned char asn1_octet, HUGE *asn1;
typedef char HUGE * asn1_pchar;


typedef struct {
	int     l;
	asn1    v;
} asn1_field;

typedef char HUGE * asn1_string;
typedef asn1_field asn1_oid;
typedef void HUGE * asn1_opaque;
typedef void NEAR * asn1_pvoid;
typedef unsigned long int FAR * asn1_pulong;

#ifdef RP0000_MicV0600
typedef unsigned long int  ulong;
#endif
/* ulong needs to be defined on HP machine also */
#ifdef RP0003_HP
typedef unsigned long int  ulong;
#endif

typedef struct {
	int     type_id;
	void *  data;
} asn1_external;


/********************************************\
*                                            *
*  CODING AND DECODING MACROS for ST MAVROS  *
*                                            *
\********************************************/
#define asn1_flags_cpy(pval,len,def,dum) *(pval) = def; \
					len = asn1st_flags_len(def);


/*******************************\
*                               *
*  CODING AND DECODING MACROS   *
*                               *
\*******************************/
#define asn1_type(z,m,n,x)      ((m == -1)?(z=x,z+1):asn1_type_cod(z,m,n,0))

#define asn1_boolean_cpy(x,y)   (*(x)= y)

#define asn1_int_dec(z1,z2,x)   asn1_intdec(asn1_skiptype(z1,z2),z2,x)
#define asn1_int_cpy(x,y)       (*(x) = y)

#define asn1_real_dec(z1,z2,x)  asn1_realdec(asn1_skiptype(z1,z2),z2,x)
#define asn1_real_cpy(x,y)      (*(x) = y)


#define asn1_flags_cmp(v1,l1, v2,l2)    (((v1) == (v2))?0:-1)


#define asn1_oid_vcod           asn1_octets_vcod
#define asn1_oid_dec            asn1_octets_dec
#define asn1_oid_cpy            asn1_octets_cpy
#define asn1_oid_cmp            asn1_octets_cmp

#define asn1_chars_cod(z,m,n,x) asn1_octets_cod(z,m,n,(asn1)x,strlen(x))

#define asn1_null_cpy()         (0)
#define asn1_null_cmp()         (0)

#define asn1_any_cod(z,m,n,x,l) (ASN1_MOVE(x,z,l),z+l)
#define asn1_any_len(n,x,l)     (l)
#define asn1_any_free(x,l)      (asn1_alloc_free((char *)x,1))
#define asn1_any_lwc(z,x,l)     asn1_octets_lwc(z,x,l)
#define asn1_any_lwd(z,zm,x,l)  asn1_octets_lwd(z,zm,x,l)
#define asn1_any_err(x,l)       (*(x)=0, *(l)=0)
#define asn1_any_cpy            asn1_octets_cpy
#define asn1_any_cmp            asn1_octets_cmp
#define asn1_any_hash           asn1_octets_hash
#define asn1_any_in             asn1_octets_in
#define asn1_any_out            asn1_octets_out
#define asn1_any_olen           asn1_octets_olen
#define asn1_any_ftcd           asn1_octets_ftcd
#define asn1_any_ftdc           asn1_octets_ftdc
#define asn1_any_ftln           asn1_octets_ftln

#define asn1_copy_len           asn1_any_len
#define asn1_copy_vcod          asn1_any_vcod
#define asn1_copy_dec           asn1_any_dec
#define asn1_copy_free          asn1_any_free
#define asn1_copy_lwc           asn1_any_lwc
#define asn1_copy_lwd           asn1_any_lwd
#define asn1_copy_err           asn1_any_err
#define asn1_copy_cpy           asn1_any_cpy
#define asn1_copy_cmp           asn1_octets_cmp
#define asn1_copy_hash          asn1_octets_hash
#define asn1_copy_in            asn1_any_in
#define asn1_copy_out           asn1_any_out
#define asn1_copy_olen          asn1_any_olen
#define asn1_copy_ftcd          asn1_octets_ftcd
#define asn1_copy_ftdc          asn1_octets_ftdc
#define asn1_copy_ftln          asn1_octets_ftln

#define ASN1_X509               (1<<31)

/***********************************\
*                                   *
*  CODING AND DECODING PROCEDURES   *
*                                   *
\***********************************/

/* Prototyping for asn1 STORE functions */
int   PASCAL   asn1st_flagcod PROTO((STOREHANDLE, unsigned long, long));
int PASCAL asn1st_flagdec PROTO((STOREHANDLE, long, unsigned long FAR *,
				 long FAR *));
int PASCAL asn1st_anystoredec PROTO((STOREHANDLE, long,
				  unsigned long int FAR * hstore));
int PASCAL asn1st_anystorecod PROTO((STOREHANDLE, unsigned long int));
int PASCAL asn1st_binmov PROTO((STOREHANDLE,long,char FAR*,long FAR*,
				long,unsigned short int FAR*));
int PASCAL asn1st_bitcod PROTO((STOREHANDLE,char FAR*,long ));
int PASCAL asn1st_bitstcod PROTO((STOREHANDLE,STOREHANDLE,long,long));
int PASCAL asn1st_bitst_dec PROTO((STOREHANDLE,long,long FAR*,long FAR*));

int PASCAL asn1st_close PROTO((STOREHANDLE, long, long));
int PASCAL asn1st_copy PROTO((STOREHANDLE, int, int, asn1, long));
int PASCAL asn1st_errdec PROTO((long, int));
int PASCAL asn1st_errdecset PROTO((STOREHANDLE,int));
int PASCAL asn1st_intcod PROTO((STOREHANDLE, long));
int PASCAL asn1st_intdec PROTO((STOREHANDLE, long, long FAR*));
int PASCAL asn1st_lencod PROTO((STOREHANDLE, long));
int PASCAL asn1st_lendec PROTO((STOREHANDLE ,long , long FAR*));
int PASCAL asn1st_length PROTO((STOREHANDLE, long, long FAR*));
int PASCAL asn1st_skip PROTO((STOREHANDLE, long));
int PASCAL asn1st_skipclose PROTO((STOREHANDLE, long , long));
int PASCAL asn1st_skiptype PROTO((STOREHANDLE, long));
int PASCAL asn1st_realcod PROTO((STOREHANDLE, double));
int PASCAL asn1st_regular_end PROTO((STOREHANDLE, long));
int PASCAL asn1st_oidcod PROTO((STOREHANDLE, unsigned long FAR *));
int PASCAL asn1st_oiddec PROTO((STOREHANDLE, long, unsigned long FAR*));
int PASCAL asn1st_octmov PROTO((STOREHANDLE hdl, long endpos, char FAR *s,
			     long FAR *l, long smax, int  errmode));
int PASCAL asn1st_octets_cod PROTO((STOREHANDLE,char FAR *,long));
int PASCAL asn1st_octets_dec PROTO((STOREHANDLE,long,long FAR*,long FAR*));

int PASCAL asn1st_deflenpos PROTO((STOREHANDLE));
int PASCAL asn1st_deflencod PROTO((STOREHANDLE,long)); 

int PASCAL asn1st_octstcod PROTO((STOREHANDLE,STOREHANDLE,long,long));
int PASCAL asn1st_getmatchz PROTO((STOREHANDLE, long , unsigned char FAR *));
int PASCAL asn1st_checksetnum PROTO((STOREHANDLE,int,int FAR *));

int PASCAL asn1st_load PROTO((STOREHANDLE,long FAR *,FILE *, FILE *, 
			      char FAR *, int));


long PASCAL asn1st_stroct PROTO((STOREHANDLE,long,long FAR *,
				 long FAR *,int FAR * ));
long PASCAL asn1st_strbit PROTO((STOREHANDLE,long,long FAR *,long FAR *,
				 int FAR *,int FAR * ));
long PASCAL asn1st_octpak PROTO((STOREHANDLE,long,long));
long PASCAL asn1st_bitpak PROTO((STOREHANDLE,long,long,int FAR *));
long PASCAL asn1st_number PROTO((STOREHANDLE,long));
long PASCAL asn1st_size   PROTO((STOREHANDLE,long));
long PASCAL asn1st_type_cod PROTO((STOREHANDLE, int, long, int));
  
/********************************************\
*                                            *
* Miscellaneous functions for STORE MAVROS.  *
*                                            *
\********************************************/

double PASCAL  asn1st_realval PROTO((STOREHANDLE, long));

unsigned char FAR * PASCAL asn1_check PROTO(( unsigned char FAR *, 
					       unsigned char FAR * ));

asn1 PASCAL asn1_lencod PROTO((asn1,int));

int PASCAL asn1st_flags_len PROTO((unsigned long int));
int PASCAL asn1st_swapint PROTO((unsigned long int FAR *));

asn1 PASCAL asn1_jumptype PROTO((asn1 ,asn1)); /* == asn1_skiptype()  */
int PASCAL asn1st_regular_end  PROTO((STOREHANDLE, long));
int PASCAL asn1st_copy PROTO((STOREHANDLE, int, int, asn1, long));


/********************************************\
*                                            *
* Prototyping for EXTERNAL, PDV-List, ANY    *
*                                            *
\********************************************/

/**********************************************************************/
/*                                                                    */
/*  TYPE MATCHING MACROS                                              */
/*                                                                    */
/**********************************************************************/

#define PDV_list_match(matchz) ((matchz)[0] == 48)
#define Components_of_set_match(matchz) ((matchz)[0] == 49)
#define RTS_External_match(matchz) ((matchz)[0] == 40)
#define Simply_encoded_data_match(matchz) (((matchz)[0]|32) == 36)

#define RTS_any_match(matchz) ((1)) /* normaly not used, because       */
				    /* ANY always taged                */


/**********************************************************************/
/*                                                                    */
/*  CODING AND DECODING PROCEDURES                                    */
/*                                                                    */
/**********************************************************************/

int PASCAL PDV_list_dec PROTO((STOREHANDLE,unsigned long,ulong FAR *,
			       void NEAR *));
int PASCAL PDV_list_cod PROTO((STOREHANDLE,int,int,ulong FAR *,
			       void NEAR  *));
int PASCAL PDV_list_dseq PROTO((STOREHANDLE,unsigned long int,
				ulong FAR *,void NEAR *));
int PASCAL PDV_list_ccod PROTO((STOREHANDLE,ulong FAR *,void NEAR *));

int PASCAL ROS_External_dec PROTO((STOREHANDLE hdl,unsigned long poszm,
	       ulong FAR * pNode, void NEAR * pContext));
int PASCAL RTS_External_dec PROTO((STOREHANDLE,unsigned long,
				   ulong FAR *,void NEAR *));
int PASCAL RTS_External_cod PROTO((STOREHANDLE,int,int,ulong FAR *,
				   void NEAR *));
int PASCAL RTS_External_dseq PROTO((STOREHANDLE,unsigned long int,
				    ulong FAR *,void NEAR *));
int PASCAL RTS_External_ccod PROTO((STOREHANDLE,ulong FAR *,void NEAR *));

int PASCAL Simply_encoded_data_dec PROTO((STOREHANDLE,unsigned long,
					  ulong FAR *,void NEAR *));
int PASCAL Simply_encoded_data_cod PROTO((STOREHANDLE,int,int,
					  ulong FAR *,void NEAR *));

int PASCAL Components_of_set_dec PROTO((STOREHANDLE,unsigned long,
				 ulong FAR *,long,void NEAR *));
int PASCAL Components_of_set_cod PROTO((STOREHANDLE,int,int,ulong FAR *,
				 long,void NEAR *));
int PASCAL Components_of_set_ccod PROTO((STOREHANDLE,ulong FAR *,long,
					 void NEAR *));

int PASCAL RTS_any_dec PROTO((STOREHANDLE, unsigned long, ulong FAR *,
			      int , void NEAR *));
int PASCAL RTS_any_cod PROTO((STOREHANDLE, int, int, ulong FAR *, 
			      int, void NEAR *));

int PASCAL RTS_anydef_dec PROTO((     
 STOREHANDLE                ,         
 unsigned long            ,           
 unsigned long FAR *      ,           
 rTro76_stDefinedBy FAR * ,           
 int                      ,           
 void NEAR *                ));       
                                      
int PASCAL RTS_anydef_cod PROTO((     
 STOREHANDLE             ,            
 int                     ,            
 int                     ,            
 unsigned long int FAR *             ,
 rTro76_stDefinedBy FAR *,            
 int                   ,              
 void NEAR  *            ));          
/* END STORE MAVROS prototyping */

#define asn1_malloc(l)  asn1_alloc_malloc(l,(char *)0)
#define asn1_free(x)    asn1_alloc_free((char *)(x),1)


char *  asn1_alloc_malloc PROTO((unsigned int, char *));
char *  asn1_realloc PROTO((char *, unsigned int));
void    asn1_alloc_free PROTO((char *, int));
int     asn1_alloc_snap_shot  PROTO((char ***));

int PASCAL asn1_int_cmp            PROTO((long, long));
int PASCAL asn1_octets_cmp         PROTO((asn1, long, asn1, long));
int PASCAL asn1_chars_cmp          PROTO((char FAR *, char FAR *));
int PASCAL asn1_boolean_cmp        PROTO((int, int));
/*        asn1_bits_cmp           PROTO((asn1, int, asn1, int)), */
int PASCAL asn1_boolean_hash       PROTO((int, int));
int PASCAL asn1_octets_hash        PROTO((int, asn1, int));
int PASCAL asn1_chars_hash         PROTO((int, char FAR *));
int PASCAL asn1_bits_hash          PROTO((int, asn1, int));
int PASCAL asn1_real_hash          PROTO((int, double));
int PASCAL asn1_real_cmp           PROTO((double, double));

/********************************************************************\
*                                                                    *
* The following definitions are used by the type checking procedures *
*                                                                    *
\********************************************************************/
extern int      asn1_diagnostic;
extern asn1     asn1_wrongbyte;

char FAR * PASCAL asn1_errmes PROTO((int));

#define ASN1_CKERR(x,z)         (asn1st_errdec(x,z))
#define ASN1_ERR_IN(z,zm,diag)  ((char FAR *) asn1_errdec((asn1)(z),diag))
#define asn1_err_in(z,diag)     ((char FAR *) asn1_errdec((asn1)(z),diag))
#define asn1_err_ftlws(z,diag)  ((int FAR *) asn1_errdec((asn1)(z),diag))


#define ASN1_ERR_TAG    1       /* Tag length > limit */
#define ASN1_ERR_LLEN   2       /* Length's length > limit */
#define ASN1_ERR_LEN    3       /* Length > limit */
#define ASN1_ERR_SHORT  4       /* Value too short */
#define ASN1_ERR_EOC    5       /* Incorrect EOC format */
#define ASN1_ERR_BOUND  6       /* EOC missing before limit */
#define ASN1_ERR_LDEF   7       /* Undefined form for primitive */
#define ASN1_ERR_STRUCT 8       /* Structured form for primitive */
#define ASN1_ERR_MISS   9       /* Mandatory component missing */
#define ASN1_ERR_CHOICE 10      /* Incorrect choice component */
#define ASN1_ERR_BOOL   11      /* Incorrect boolean */
#define ASN1_ERR_NULL   12      /* Incorrect null component */
#define ASN1_TEXT_EXISTS 13     /* Object ID name already declared */
#define ASN1_OID_EXISTS 14      /* Object ID already declared */
#define ASN1_OIDEMPTY   15      /* empty object identifier */
#define ASN1_OIDBADTOP  16      /* bad top level object id */
#define ASN1_OIDUNDEF   17      /* undefined object identifier */
#define ASN1_OIDMISSSEC 18      /* missing second level component */
#define ASN1_OIDSECBIG  19      /* second component > 39 */
#define ASN1_OIDBADSEC  20      /* incorrect second component */
#define ASN1_OIDNUMCOMP 21      /* final components must be numeric */

#define ASN1_ERR_EQUAL  22      /* missing equal sign */
#define ASN1_ERR_OBRACKET 23    /* missing opening bracket */
#define ASN1_ERR_CBRACKET 24    /* missing closing bracket */
#define ASN1_ERR_HEXA   25      /* incorrect hexadecimal digit */
#define ASN1_ERR_ST_END 26      /* input end before end of string */
#define ASN1_ERR_MALLOC 27      /* cannot allocate memory */
#define ASN1_ERR_B_OR_H 28      /* string type must be B or H */
#define ASN1_ERR_BSTRING 29     /* incorrect binary string */
#define ASN1_ERR_STRING 30      /* string component expected */
#define ASN1_ERR_HSTRING 31     /* hexa string should be typed H */
#define ASN1_ERR_ST_EOL 32      /* end of line before end of string */
#define ASN1_ERR_FLAGS  33      /* unexpected bit string element */
#define ASN1_ERR_ABSURD 34      /* absurd data type */

#define ASN1_ERR_COMPS  35      /* less components than expected */
#define ASN1_ERR_PTR    36      /* incorrect FTLWS pointer */

#define ASN1_ERR_EXTERNAL 37    /* Error in External type resolution */

/* ---> New error codes for STORE-MAVROS <---  */

#define ASN1_ERR_BSLEN    50    /* Length in BIT STRING 0 => no initial Octet */
#define ASN1_ERR_IBTS     51    /* "Initial octet" within constructed BIT  */
				/* STRINGs not 0 for the not last substring*/
				/* or "initial octet" greater than 7       */
#define ASN1_ERR_WRTAG    52    /* Wrong tag within BIT STRING or OCTET STR. */
#define ASN1_ERR_INT      53    /* Wrong Integer, length = 0 */
#define ASN1_ERR_GETMA    54    /* Error in asn1_getmatchz, difference */
				/* in poszm and real storelength */
#define ASN1_ERR_SETNUM   55    /*  more than one element of the same  */
				/*  SET component was received.        */
#define ASN1_ERR_WRSET    56    /* Wrong component  within SET or SEQUENCE */
				/* Not detected in MAVROS V2.2 because of  */
				/* using SET decoding functions also for   */
				/* "COMPONENTS OF" and skipping unexpected */
				/* components  */
#define ASN1_ERR_OIDLEN   57    /* in last byte of OID is bit 8 set        */

/* ------- Error because of local restrictions ---------------*/

#define ASN1_ERR_LOCSTART 70    /* first define value for local errors     */

#define ASN1_ERR_INTLEN   71    /* Wrong Integer length (max. 4 bytes)     */
#define ASN1_ERR_S2SH     72    /* Value of an OCTET STRING or BIT STRING  */
				/* is longer than target string            */
				/* S_SIZE (maxlen) in xx.mvr used          */
#define ASN1_ERR_SOFSZ    73    /* Number of components > Size of array for SEQ OF */
				/* S_SIZE (maxlen) in xx.mvr used          */
#define ASN1_ERR_SOFOID   74    /* Number of components in OBJECT ID greater*/
				/* than Size of array for OBJECT IDENTIFIER */

#define ASN1_ERR_LOCEND   80    /* last define value for local errors      */

#define STASN1ER         -25    /* Errornumber set in store if ASN.1 error */



/****************************************\
*                                        *
* Definitions for the OBJECT ID library  *
*                                        *
\****************************************/
asn1_oid PASCAL asn1_oid_get  PROTO((char FAR *, int FAR *));
asn1_oid PASCAL asn1_oid_copy PROTO((asn1_oid));
char FAR * PASCAL asn1_oid_put  PROTO((char FAR *, asn1_oid));
char FAR * PASCAL asn1_oid_xput PROTO((char FAR *, asn1_oid, int));

#define ASN1_OID_CCITT          "ccitt"
#define ASN1_OID_ISO            "iso"
#define ASN1_OID_ISO_CCITT      "joint-iso-ccitt"

#define ASN1_OID_STANDARD       "standard"
#define ASN1_OID_REG_AUT        "registration-authority"
#define ASN1_OID_MEMBER         "member-body"
#define ASN1_OID_ID_ORG         "identified-organization"

#define ASN1_OID_RECOM          "recommendation"
#define ASN1_OID_QUESTION       "question"
#define ASN1_OID_ADMIN          "administration"
#define ASN1_OID_NET_OP         "network-operator"

/*************************************\
*                                     *
* Support of the ANY DEFINED BY type  *
*                                     *
\*************************************/

typedef int     (*asn1_decfunc)(void); /* ST changed */
typedef void    (*asn1_errfunc)(void);
typedef void    (*asn1_freefunc)(void);
typedef int     (*asn1_codfunc)(void); /* ST changed */
typedef int     (*asn1_lenfunc)(void);
typedef asn1    (*asn1_lwdfunc)(void);
typedef asn1    (*asn1_lwcfunc)(void);
typedef asn1_string (*asn1_infunc)(void);
typedef asn1_string (*asn1_outfunc)(void);
typedef int     (*asn1_olenfunc)(void);
typedef void    (*asn1_cpyfunc)(void);
typedef int     (*asn1_cmpfunc)(void);
typedef int     (*asn1_hashfunc)(void);
typedef int FAR * (*asn1_ftdcfunc)(void);
typedef int FAR * (*asn1_ftcdfunc)(void);
typedef int     (*asn1_ftlnfunc)(void);
typedef int     (*asn1_ftfxfunc)(void);

typedef struct asn1_type_desc {
	asn1_codfunc    t_cod;
	asn1_lenfunc    t_len;
	asn1_decfunc    t_dec;
	asn1_ftcdfunc   t_ftcd;
	asn1_ftlnfunc   t_ftln;
	asn1_ftlnfunc   t_ftfx;
	asn1_ftdcfunc   t_ftdc;
	asn1_outfunc    t_out;
	asn1_olenfunc   t_olen;
	asn1_infunc     t_in;
	asn1_freefunc   t_free;
	asn1_errfunc    t_err;
	asn1_cpyfunc    t_cpy;
	asn1_cmpfunc    t_cmp;
	asn1_hashfunc   t_hash;
} asn1_type_desc;

/*********************************\
*                                 *
* The table of abstract syntaxes  *
*                                 *
\*********************************/
typedef struct {
	asn1_oid        *as_id;
	asn1_type_desc  *as_type;
	void *          as_data;
} asn1_abstract_syntax;

extern asn1_abstract_syntax asn1_syntax_list[];
extern int asn1_syntax_count;

/*************************************\
*                                     *
* The table of presentation contexts  *
*                                     *
\*************************************/
typedef struct {
	int as; /* rank of the abstract syntax */
	int id; /* identifier of the presentation context */
	int ts; /* local identifier of the selected transfer syntax    */
} asn1_one_def_ctx;

#ifdef INRIA
typedef struct {
	int                     defas;          /* default abstract syntax*/
	int                     defts;          /* default transf. syntax */
	int                     nb_ctx_dcs;     /* nb of ctx in the set */
	asn1_one_def_ctx        * dcs;          /* set of defined context */
	int                     * as_stack;     /* the stack of the abs. syn.*/
	int                     index_as_stack; /* top index in the stack */
	asn1_opaque             pres_info;      /* Presentation level info. */
} asn1_pres_connection;
#else
typedef struct {
	int nb_ctx_dcs;         /* Number of contexts in the set */
	asn1_one_def_ctx * dcs; /* The set of defined context */
	void * p_proto;         /* Presentation level information */
} asn1_pres_connection;
#endif

/**********************************\
*                                  *
* The transfer syntax identifiers  *
*                                  *
\**********************************/
#define    ASN1_BER           0
#define    ASN1_FTLWS         1


/************************************\
*                                    *
* Support of the EXTERNAL data type  *
*                                    *
\************************************/

asn1    PASCAL asn1_external_cod
	PROTO((asn1,int,int,int,void FAR *,asn1_pres_connection FAR *));
int     PASCAL asn1_external_len
	PROTO((int,int,void FAR *,asn1_pres_connection FAR *));
asn1    PASCAL asn1_external_dec
	PROTO((asn1,asn1,int FAR *,void FAR * FAR *,
	       asn1_pres_connection FAR *));
int FAR  * PASCAL asn1_external_ftcd
	PROTO((int FAR *,int FAR * FAR *,int,void FAR *,
	       asn1_pres_connection FAR *));
int     PASCAL asn1_external_ftln
	PROTO((int,void FAR *,asn1_pres_connection FAR *));
int     PASCAL asn1_external_ftfx
	PROTO((int,void FAR *,asn1_pres_connection FAR *));
int FAR * PASCAL asn1_external_ftdc
	PROTO((int FAR *,int FAR *,int FAR *,void FAR * FAR *,
	       asn1_pres_connection FAR *));
char FAR * PASCAL asn1_external_in
	PROTO((char FAR *,char FAR *,int FAR *,
	       void FAR * FAR *,asn1_pres_connection FAR *));
char FAR * PASCAL asn1_external_out
	PROTO((char FAR  *, int, void FAR *, asn1_pres_connection FAR *));
int     PASCAL asn1_external_olen
	PROTO((int,void FAR *,asn1_pres_connection FAR *));
void    PASCAL asn1_external_free
	PROTO((int,void FAR *,asn1_pres_connection FAR *));
void    PASCAL asn1_external_err
	PROTO((int FAR *,void FAR * FAR *, asn1_pres_connection FAR *));
void    PASCAL asn1_external_cpy
	PROTO((int FAR *,void FAR * FAR *,asn1_pres_connection FAR *,
	       int,void FAR *, asn1_pres_connection FAR *));
int     PASCAL asn1_external_cmp
	PROTO((int,void FAR *,asn1_pres_connection FAR *,int,void FAR *,
	       asn1_pres_connection FAR *));
int     PASCAL asn1_external_hash
	PROTO((int,int,void FAR *,asn1_pres_connection FAR *));

/*****************************************\
*                                         *
* Definition of the coding functions for  *
* the presentatio protocol.               *
*                                         *
\*****************************************/
asn1    PASCAL asn1_pres_value_cod      
	PROTO((asn1,int,int,void FAR *,asn1_pres_connection FAR *));
int     PASCAL asn1_pres_value_len      
	PROTO((int,int,void FAR *,asn1_pres_connection FAR *));
asn1    PASCAL asn1_pres_value_dec      
	PROTO((asn1,asn1,int,int,void FAR * FAR *,asn1_pres_connection FAR *));
int FAR * PASCAL asn1_pres_value_ftcd   
	PROTO((int FAR *,int FAR * FAR *,int,int,void FAR *,
	       asn1_pres_connection FAR *));
int     PASCAL asn1_pres_value_ftln     
	PROTO((int,int,void FAR *,asn1_pres_connection FAR *));
int FAR * PASCAL asn1_pres_value_ftdc   
	PROTO((int FAR *,int FAR *,int,int,void FAR * FAR *,
	       asn1_pres_connection FAR *));
int     PASCAL asn1_pres_octet_dec      
	PROTO((asn1,int,int,int,void FAR * FAR *,asn1_pres_connection FAR *));
int     PASCAL asn1_as_by_oid          PROTO((asn1,int));
int     PASCAL asn1_external_ts        PROTO((asn1,int));
void    PASCAL asn1_external_as 
	PROTO((int,int FAR *,int FAR *,asn1_pres_connection FAR *));
void    PASCAL asn1_external_ctx        
	PROTO((int FAR *,int FAR *,int,asn1_pres_connection FAR *));



/***************************\
*                           *
* Miscellaneous functions.  *
*                           *
\***************************/

int PASCAL asn1_load  PROTO((asn1 *, asn1 *, FILE *, FILE *,
			     char FAR *, int));
int  PASCAL asn1_load_item PROTO((int));
void PASCAL asn1_load_memory  PROTO((int));
int  PASCAL asn1_load_tag PROTO((int));
void PASCAL asn1_load_value  PROTO((asn1 *, int FAR *, int));
int  PASCAL asn1_load_blank PROTO((void));
int  PASCAL asn1_load_comment PROTO((void));
void PASCAL asn1_load_mess PROTO((char FAR *));
void PASCAL asn1_load_error  PROTO((int, char FAR *));
int  PASCAL asn1_field_cmp  PROTO((asn1_field FAR *, asn1_field FAR *));
int  PASCAL asn1_bits_cmp  PROTO((asn1, int, asn1, int));
int  PASCAL asn1_fetch PROTO((FILE *, asn1 FAR *, int FAR *, int));
asn1 PASCAL asn1_strdec PROTO((asn1, asn1, char FAR *, int FAR *));
int  PASCAL asn1_oid_int  PROTO((int, asn1, unsigned long FAR *));
asn1 PASCAL asn1_int_oid PROTO((asn1, unsigned long FAR *, int));
asn1 PASCAL asn1_fixed_end PROTO((asn1, asn1, int));
asn1 PASCAL asn1_uniquefixed_set PROTO((asn1, asn1, int));
asn1 PASCAL asn1_definedfixed_end PROTO((asn1, asn1, int));
void PASCAL asn1_std_mode_on PROTO((void));
asn1 PASCAL asn1_defined_end  PROTO((asn1, asn1));
asn1 PASCAL asn1_unique_set  PROTO((asn1, asn1));
void PASCAL asn1_x509_mode_on  PROTO((void));
void PASCAL asn1_output_indent_reset PROTO((void));
int  PASCAL asn1_count_size_of_line  PROTO((char FAR *));
int  PASCAL asn1_inside_bracket  PROTO((char FAR *));
int  PASCAL asn1_compare_name  PROTO((char FAR *, char FAR *));
int  PASCAL asn1_input_size  PROTO((char FAR *, char FAR *));
char FAR * PASCAL asn1_hexa_out PROTO(( char FAR *, asn1, int));
int  PASCAL asn1_hexa_digit_in  PROTO((int, int FAR *, int FAR *));
char FAR * PASCAL asn1_hexa_in  PROTO((char FAR *, char FAR *, asn1 *, 
			  int FAR *, int FAR *, int FAR *));
char FAR * PASCAL asn1_input_qstring  PROTO((char FAR *, char FAR *,
				    asn1 *, int FAR *));
char FAR * PASCAL asn1_binary_out  PROTO((char FAR *, asn1, int));
int PASCAL asn1_bits_olen  PROTO((asn1, int));
int PASCAL asn1_octets_olen  PROTO((asn1, int));
int PASCAL asn1_chars_olen  PROTO((char FAR *));
int PASCAL asn1_oid_olen  PROTO((asn1, int));
int PASCAL asn1_boolean_v_in PROTO((char FAR *, char FAR *, char FAR * FAR *));
void PASCAL asn1_octets_cpy  PROTO((asn1 *, int FAR *, asn1, int));
void PASCAL asn1_bits_cpy  PROTO((asn1 *, int FAR *, asn1, int));
void PASCAL asn1_chars_cpy  PROTO((char FAR * FAR *, char FAR *));
int  PASCAL asn1_load_text  PROTO((char FAR * FAR *, char FAR * FAR *,
				    FILE *, FILE *, char FAR *,int));
asn1 PASCAL asn1_absurd_cod  PROTO((asn1, int, int, void FAR *));
int  PASCAL asn1_absurd_len  PROTO((int, void FAR *));
asn1 PASCAL asn1_absurd_dec  PROTO((asn1, asn1, void FAR * FAR *));
asn1 PASCAL asn1_absurd_lwc  PROTO((asn1, void FAR *));
asn1 PASCAL asn1_absurd_lwd  PROTO((asn1, asn1, void FAR * FAR *));
asn1_string  PASCAL asn1_absurd_out  PROTO((asn1_string, void FAR *));
int PASCAL asn1_absurd_olen PROTO((void FAR *));
asn1_string PASCAL asn1_absurd_in  PROTO((asn1, asn1, void FAR * FAR *));
void PASCAL asn1_absurd_free PROTO((void FAR *x));
void PASCAL asn1_absurd_err PROTO((void  FAR * FAR * x));
void PASCAL asn1_absurd_cpy PROTO((void FAR * FAR * x, void FAR * y));
int PASCAL asn1_oid_decl  PROTO((char FAR  *, asn1_oid));
int PASCAL look_for_oid  PROTO((asn1_oid oid));
int PASCAL look_for_oid_part  PROTO((asn1_oid, int FAR *));
int PASCAL one_digit_more  PROTO((int FAR *, int, int, int, int));
char FAR * PASCAL copy_to_blank PROTO((char FAR *, char FAR *));
char FAR * PASCAL numeric_part  PROTO((char FAR *));

/*************************************************************************\
*                                                                         *
* Prototyping of the time functions is intentionally left incomplete, so  *
* that we dont have to include the definition of the "timeb" structure    *
* in "asn1.h".                                                            *
*                                                                         *
\*************************************************************************/
#ifdef ASN1_TIMEB_DEFINED
int PASCAL asn1_utc_time  PROTO((char FAR *, struct timeb FAR *));
int PASCAL asn1_gen_time  PROTO((char FAR *, struct timeb FAR *));
int PASCAL asn1_time  PROTO((char FAR *, int, struct timeb FAR *));
int PASCAL asn1_time_gen  PROTO((char FAR *, int, struct timeb FAR *));
#else
int PASCAL asn1_utc_time();
int PASCAL asn1_gen_time();
int PASCAL asn1_time();
int PASCAL asn1_time_gen();
#endif /* !ASN1_TIMEB_DEFINED */


/* read from append.h by kc on 24-04-93 */
/*
#define RTS_anydef_match(matchz) RTS_any_match(matchz)

int PASCAL RTS_anydef_dec PROTO((
 STOREHANDLE         ,
 unsigned long       ,
 unsigned long FAR *     ,
 rTro76_stDefinedBy FAR *,
 int                 ,
 void NEAR *              ));

int PASCAL RTS_anydef_cod PROTO((
 STOREHANDLE         ,
 int                 ,
 int                 ,
 ulong FAR *             ,
 rTro76_stDefinedBy FAR *,
 int                 ,
 void NEAR *              ));
 */


#endif	/* _ASN1ST_H */
