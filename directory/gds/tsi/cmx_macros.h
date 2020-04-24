/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmx_macros.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:08  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:35  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:47:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:03  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:16:29  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:28  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:08  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:50:28  marrek]
 * 
 * $EndLog$
 */
#ifndef _CMX_MACROS_H
#define _CMX_MACROS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cmx_macros_rcsid[] = "@(#)$RCSfile: cmx_macros.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:08 $";
#endif

/*
 * COPYRIGHT (C) 1987 BY SIEMENS AG
 * All rights reserved
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1990
 *               All Rights Reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */
/*
 *	macros for writing, reading longs an shorts
 *		the variable(s) starting at this adresses should be of
 *		the right type:
 *
 *		long in Short2Long, Long2Short, Long2Long,
 *			Long2Char and Char2Long.
 *		short in Short2Char and Char2Short.
 *
 *
 *		@(#)cmx_macros.h	311.6 93/07/26 TNSX (NC 1)
 */

/*	read long	*/
#if defined(T4600) || defined(WX200R) || defined(RWXXX) || defined(RM700R) || defined(AIX) || defined(HPUX) || defined(SUN)
#define Short2Long(p) \
	( (long) ((unsigned short *)&(p))[1] + \
	 ((long) ((unsigned short *)&(p))[0] << 16))
#else
#define Short2Long(p) \
	 (long) p
#endif

/*	write long	*/
#if defined(T4600) || defined(WX200R) || defined(RWXXX) || defined(RM700R) || defined(AIX) || defined(HPUX) || defined(SUN)
#define Long2Short(p,v)	\
	((unsigned short *)&(p))[1] = (unsigned short) v ;\
	((unsigned short *)&(p))[0] = (unsigned short) (v >> 16) ;
#else
#define Long2Short(p,v)	\
	p = v
#endif

/*	copy long in memory == Long2Short( , Short2Long( , ));	*/
#if defined(T4600) || defined(WX200R) || defined(RWXXX) || defined(RM700R) || defined(AIX) || defined(HPUX) || defined(SUN)
#define Long2Long(p,v) \
	memcpy( (void *)&(p), (void *)&(v), sizeof (long))
#else
#define Long2Long(p,v) \
	p = v
#endif

/*
 *	macros for writing and reading long and shorts from
 *	any address
 */

/*	 write long	*/
#if defined(T4600) || defined(WX200R) || defined(RWXXX) || defined(RM700R) || defined(AIX) || defined(HPUX) || defined(SUN)
#define	Long2Char(chr,lng) \
	((unsigned char *)&(chr))[3] = (unsigned char) ((lng)       & 0xff);\
	((unsigned char *)&(chr))[2] = (unsigned char) (((lng) >>  8) & 0xff); \
	((unsigned char *)&(chr))[1] = (unsigned char) (((lng) >> 16) & 0xff); \
	((unsigned char *)&(chr))[0] = (unsigned char) (((lng) >> 24) & 0xff)
#else
#define	Long2Char(chr,lng) \
	chr = lng
#endif

/*	 write short	*/
#if defined(T4600) || defined(WX200R) || defined(RWXXX) || defined(RM700R) || defined(AIX) || defined(HPUX) || defined(SUN)
#define	Short2Char(chr,sht) \
	((unsigned char *)&(chr))[1] = (unsigned char)  ((sht)      & 0xff); \
	((unsigned char *)&(chr))[0] = (unsigned char) (((sht) >> 8) & 0xff)
#else
#define	Short2Char(chr,sht) \
	chr = sht
#endif

/*	read long	*/
#if defined(T4600) || defined(WX200R) || defined(RWXXX) || defined(RM700R) || defined(AIX) || defined(HPUX) || defined(SUN)
#define Char2Long(chr) \
	((long)  ((unsigned char *)&(chr))[3]		+ \
	 (long) (((unsigned char *)&(chr))[2] <<  8)	+ \
	 (long) (((unsigned char *)&(chr))[1] << 16)	+ \
	 (long) (((unsigned char *)&(chr))[0] << 24))
#else
#define Char2Long(chr) \
	chr
#endif

/*	read short	*/
#if defined(T4600) || defined(WX200R) || defined(RWXXX) || defined(RM700R) || defined(AIX) || defined(HPUX) || defined(SUN)
#define Char2Short(chr) \
	((short)  ((unsigned char *)&(chr))[1]	+ \
	 (short) (((unsigned char *)&(chr))[0] <<  8))
#else
#define Char2Short(chr) \
	chr
#endif

/*
 * Silly definition for sigaction, because sometimes (HPUX and SCO)
 * the pointer is an integer.
 */
#if defined(HPUX) || defined(SCO)
#define SIG_PTR (int)
#else
#define SIG_PTR
#endif
#endif /* _CMX_MACROS_H */
