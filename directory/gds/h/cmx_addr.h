/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * HISTORY
 * $Log: cmx_addr.h,v $
 * Revision 1.1.550.2  1996/02/18  23:35:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:17  marty]
 *
 * Revision 1.1.550.1  1995/12/08  15:59:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:18  root]
 * 
 * Revision 1.1.548.1  1994/02/22  18:07:33  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:42:44  marrek]
 * 
 * Revision 1.1.546.2  1993/10/14  17:27:41  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:26:19  keutel]
 * 
 * Revision 1.1.546.1  1993/10/13  17:32:24  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:10:31  keutel]
 * 
 * Revision 1.1.4.2  1993/08/10  12:13:15  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:37:10  marrek]
 * 
 * Revision 1.1.2.4  1993/01/28  14:58:23  marrek
 * 	Increase T_TSELSIZE to 32
 * 	[1993/01/28  12:13:59  marrek]
 * 
 * Revision 1.1.2.3  1992/12/31  20:39:00  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:28:40  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/04  18:09:33  marrek
 * 	November 1992 code drop
 * 	Creation
 * 	[1992/12/04  11:10:05  marrek]
 * 
 * $EndLog$
 */
#ifndef _CMX_ADDR_H
#define _CMX_ADDR_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cmx_addr_rcsid[] = "@(#)$RCSfile: cmx_addr.h,v $ $Revision: 1.1.550.2 $ $Date: 1996/02/18 23:35:23 $";
#endif

/*
 *   @(#)cmx_addr.h	311.3 93/08/24
 */
/*********************************************************************/
/*	NAMES AND ADDRESSES					     */
/*********************************************************************/

#ifndef _HETERO

struct t_myname {
	char	t_mnmode;	/* = T_MNMODE */
#define T_MNMODE 1
	char	t_mnres;	/* = 0 */
	short	t_mnlng;	/* Length of filled part of structure */
#define T_MNSIZE 120
	char	t_mn[T_MNSIZE];	/* Array for T-selectors */
};

struct t_partaddr {
	char	t_pamode;	/* = T_PAMODE */
#define T_PAMODE 2
	char	t_pares;	/* = 0 */
	short	t_palng;	/* Length of filled part of structure */
#define T_PASIZE 108
	char	t_pa[T_PASIZE];	/* Array for partner's TSAP address */
};

#else

struct t_myname {
	char	t_mnmode;	/* = T_MNMODE */
#define T_MNMODE 1
	char	t_mnres;	/* = 0 */
	char	t_mnlng[2];	/* Length of filled part of structure */
#define T_MNSIZE 120
	char	t_mn[T_MNSIZE];	/* Array for T-selectors */
};

struct t_partaddr {
	char	t_pamode;	/* = T_PAMODE */
#define T_PAMODE 2
	char	t_pares;	/* = 0 */
	char	t_palng[2];	/* Length of filled part of structure */
#define T_PASIZE 108
	char	t_pa[T_PASIZE];	/* Array for partner's TSAP address */
};

#endif	/* _HETERO */

/*
 * Future PSAP address
 * A PSAP address consists (in this order) of these components
 *  0-1 PSEL Presentation Selector (OPTIONAL)
 *  0-1 SSEL Session Selector (OPTIONAL)
 *    1 TSEL Transport Selector (MANDATORY)
 *  1-n NSAP Network Service Access Point Address (MANDATORY)
 * All components are described by struct t_adrdesc.
 *
 * These macros are silly, the definition is done for compatibility    
 * reasons only.
 */
#define T_GETINFO(s) (s)
		
#define T_SETINFO(s, v) (s = v)
		

struct t_adrdesc {
#define T_adrdesc	struct t_adrdesc
	char    t_infotype;     /* Type of described component */
#define	T_PSAPINFO	0x30	/* PSEL */
#define	T_SSAPINFO	0x20	/* SSEL */
#define T_TSAPINFO      0x10    /* TSEL */
#define T_NSAPINFO      0x40    /* NSAP address */
	char    t_infovers;     /* version of described component */
#define T_PSELVERS	 0	/* version no. of the presentation selector */
#define T_SSELVERS	 0	/* version no. of the session selector */
#define T_TSELVERS	 0	/* version no. of the transport selector */
#define T_NSAPVERS	 0	/* version no. of the NSAP-address */
	short   t_translng;     /* (rest)length of TOTAL information
				   including length and leading type field */
	short   t_infolng;      /* length of component described here */
#define T_PSELSIZE	16	/* max. size of a presentation selector */
#define T_SSELSIZE	16	/* max. size of a session selector */
#define T_TSELSIZE	32	/* max. size of a transport selector */
#define T_NSAPSIZE	20	/* max. size of a NSAP-address */
	short   t_nextoff;      /* byte offset to next component
				   including all fillers for alignment */
#define T_ALIGN4(x)	((((x) - 1) & ~3) + 4)
};


				/* max. size of a PSAP-address */
#define T_PSAPSIZE	(sizeof (struct t_adrdesc) + T_ALIGN4(T_PSELSIZE) + \
								     T_SSAPSIZE)
				/* max. size of a SSAP-address */
#define T_SSAPSIZE	(sizeof (struct t_adrdesc) + T_ALIGN4(T_SSELSIZE) + \
								     T_TSAPSIZE)
#define T_MAXNSAP	8	/* max. no. of NSAP-addresses */
				/* max. size of a TSAP-address */
#define T_TSAPSIZE	(sizeof (struct t_adrdesc) + T_ALIGN4(T_TSELSIZE) + \
			(T_MAXNSAP * (sizeof (struct t_adrdesc) + \
							 T_ALIGN4(T_NSAPSIZE))))


/*
 *   Example of the structure of a TSAP address including two NSAP addresses
 *
 *   |<---+---------------------------------------------->|
 *   |    |                                               |
 *   |<---|---+------>|<---+----------------------------->|
 *   |    |   |       |    |                              |
 *   +-+-+|+-+|+------+<---|---+------->|<---+----------->|
 *   |T|v|t|i|o| tsel |    |   |        |    |            |
 *   +-+-+-+|+-+------+-+-+|+-+|+-------+<---|---+------->|
 *          |  |<-+-->|N|v|t|i|o| nsap1 |    |   |        |
 *          |     |   +-+-+-+|+-+-------+-+-+|+-+|+-------+
 *          +-----+          |  |<--+-->|N|v|t|i|o| nsap2 |
 *         (1)               |      |   +-+-+-+|+-+-------+
 *                           +------+          |  |<--+-->|
 *                          (2)                |      |
 *                                             +------+
 *                                            (3)
 *      T       t_infotype      T_TSAPINFO
 *      v       t_infovers
 *      t(1)    t_translng
 *      i(1)    t_infolng       length of Transport Selector
 *      o(1)    t_nextoff       total length of Transport component
 *      tsel                    Transport Selector
 *      N       t_infotype      T_NSAPINFO
 *      v       t_infovers
 *      t(2)    t_translng
 *      i(2)    t_infolng       length of NSAP address 1
 *      o(2)    t_nextoff       total length of NSAP component 1
 *      nsap1                   NSAP address 1
 *      N       t_infotype      T_NSAPINFO
 *      v       t_infovers
 *      t(3)    t_translng
 *      i(3)    t_infolng       length of NSAP address 2
 *      o(3)    t_nextoff       total length of NSAP component 2
 *      nsap2                   NSAP address 2
 */
/*********************************************************************/

#endif /* _CMX_ADDR_H */
