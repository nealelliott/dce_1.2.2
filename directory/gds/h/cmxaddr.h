/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxaddr.h,v $
 * Revision 1.1.751.2  1996/02/18  23:35:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:19  marty]
 *
 * Revision 1.1.751.1  1995/12/08  15:59:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:19  root]
 * 
 * Revision 1.1.749.2  1994/06/21  14:47:01  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:40:26  marrek]
 * 
 * Revision 1.1.749.1  1994/02/22  18:07:34  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:42:54  marrek]
 * 
 * Revision 1.1.747.2  1993/10/14  17:27:42  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:26:22  keutel]
 * 
 * Revision 1.1.747.1  1993/10/13  17:32:28  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:10:27  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  12:14:20  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:37:20  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:39:13  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:28:44  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  03:49:31  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:40:55  zeliff]
 * 
 * $EndLog$
 */
#ifndef _CMXADDR_H
#define _CMXADDR_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cmxaddr_rcsid[] = "@(#)$RCSfile: cmxaddr.h,v $ $Revision: 1.1.751.2 $ $Date: 1996/02/18 23:35:24 $";
#endif

/* #ident "@(#)cmxaddr.h	311.3 93/07/28 CMX (NC1)" */

/*
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1991
 * 		 All Rights Reserved
 *
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */

/*
 * Structure definitions of local names (t_myname) and remote addresses
 * (t_partaddr) can be found in cmx.h (union t_address)
 *
 * Defines:
 * _HETERO	Makes the definition of the structures independent from any
 *              alignment, i.e. any compiler generates the same memory
 *              layout (this is very important if these structures are
 *              used at the communication interface of different platforms
 *              (e.g. BS2000- and SINIX-systems).
 *
 * Revision:
 * RK	(NC152)	02.07.91	Basic version CMX V3.1 (<- 4.4).
 */

/*
 * Note: To keep portability, all components (elements) of the structures
 * defined have to start at even addresses relative to the beginning of the
 * structure. This must not be maintained for char-elements but nevertheless
 * for char[]-elements.
 */

/***********************************************************************/
/*			CCP-specific part   			       */
/***********************************************************************/

/*
 * CCPs with NEA-Profiles
 */
#define CX_WANNEA	0x0001		/* CCP-Type for WAN1, WAN2 */
#define CX_LBS2NEA	0x0200		/* CCP-Type for C30 */
#define CX_STANEA	0x0020		/* CCP-Type for CCP-STA1 from V2.1 */
#define NEA_TYPE	64		/* Address type according to ITRANS */
#define NEA_STNAMESIZE	8
#define NEA_SELSIZE	NEA_STNAMESIZE
#define NEA_ADDRSIZE	(NEA_STNAMESIZE + 2)
#define MIG_NO		0
#define MIG_YES		1

struct nea_sel
{
#ifdef _HETERO
	unsigned char	cx_type[2];	/* = CX_WANNEA or CX_STANEA */
	unsigned char	ns_size[2];	/* = NEA_SELSIZE */
#else
	unsigned short	cx_type;	/* = CX_WANNEA or CX_STANEA */
	unsigned short	ns_size;	/* = NEA_SELSIZE */
#endif
	char		ns_stname[NEA_STNAMESIZE];
					/* Station name
					   in EBCDIC according to TRANSDATA-
					   rules, starting at the lowest
					   address (if necessary, the name
					   must be padded by trailing EBCDIC-
					   blanks) */
	char		ns_mig;		/* Migration service */
};

struct nea_addr
{
#ifdef _HETERO
	unsigned char	cx_type[2];	/* = CX_WANNEA or CX_STANEA */
	unsigned char	na_type[2];	/* = NEA_TYPE */
	unsigned char	na_size[2];	/* = NEA_ADDRSIZE */
#else
	unsigned short	cx_type;	/* = CX_WANNEA or CX_STANEA */
	unsigned short	na_type;	/* = NEA_TYPE */
	unsigned short	na_size;	/* = NEA_ADDRSIZE */
#endif
	char		na_stname[NEA_STNAMESIZE];
					/* Station name
					   in EBCDIC according to TRANSDATA-
					   rules, starting at the lowest
					   address (if necessary, the name
					   must be padded by trailing EBCDIC-
					   blanks) */
	char		na_reg;		/* Region number (binary) */
	char		na_pro;		/* Processor number multiplied by 8,
					   binary */
};

/*
 * CCPs with SBKA-Profiles
 */
#define CX_LANSBKA 	0x0002		/* CCP-Type for LAN2 */
#define CX_RFC1006	0x0004		/* CCP-Type for RFC1006 */
#define CX_WAN3SBKA	0x0008		/* CCP-Type for WAN3 */
#define CX_WANSBKA 	0x0010		/* CCP-Type for WAN4, WAN5, WAN6 */
#define CX_SDLCSBKA	0x0080		/* CCP-Type for STA1/SDLC */
#define CX_LANINET 	0x0100		/* CCP-Type for LAN1 */
#define CX_LOOPSBKA     0x0400          /* CCP-Type for CMX-LOCAL */
#define CX_TRSNASBKA    0x0800          /* CCP-Type for TRANSIT-LU6.2 */
#define CX_LANGWSBKA 	0x1000		/* CCP-Type for LAN2-WAN-LAN2 Gateway */
#define CX_LANBSBKA 	0x2000		/* CCP-Type for LAN2-LAN2 Bridge */
#define CX_OSITYPE	CX_LANSBKA	/* CCP-Type for OSITYPE */
#define CX_WANGWSBKA	0x4000		/* CCP-Type for WAN-LAN2 GATEWAY */
#define SBKA_TYPE	16		/* Address type according to ITRANS */
#define SBKA_SELSIZE	10		/* max. length of a T-Selector */
#define SBKA_ADDRSIZE	102		/* max. length of a T-Address */

struct sbka_sel
{
#ifdef _HETERO
	unsigned char   cx_type[2];     /* = CX_LANSBKA or ... */
	unsigned char	ks_size[2];	/* = Length of the T-Selector,
					   max. SBKA_SELSIZE */
#else
	unsigned short  cx_type;        /* = CX_LANSBKA or ... */
	unsigned short	ks_size;	/* Length of the T-Selector,
					   max. SBKA_SELSIZE */
#endif
	char		ks_rest[SBKA_SELSIZE];
					/* T-Selector field according to
					   SBKA-rules, starting at the
					   lowest address */
};

struct sbka_addr
{
#ifdef _HETERO
	unsigned char   cx_type[2];     /* = CX_LANSBKA  or ... */
	unsigned char	ka_type[2];	/* = SBKA_TYPE */
	unsigned char	ka_size[2];	/* = Length of the T-Address,
					   max. SBKA_ADDRSIZE */
#else
	unsigned short  cx_type;        /* = CX_LANSBKA  or ... */
	unsigned short	ka_type;	/* = SBKA_TYPE */
	unsigned short	ka_size;	/* Length of the T-Address,
					   max. SBKA_ADDRSIZE */
#endif
	char		ka_rest[SBKA_ADDRSIZE];
					/* T-Address field according to
					   SBKA-rules, starting at the
					   lowest address */
};

/*
 * CCPs for station interconnection
 */
#define CX_STA		0x0004		/* CCP-Type for STA1, STA2, STA3 */
#define STA_TYPE	240		/* Address type according to ITRANS */
#define STA_STNAMESIZE	8
#define STA_PNAMESIZE	8
#define STA_SELSIZE	(STA_STNAMESIZE + 2)
#define STA_ADDRSIZE	(STA_STNAMESIZE + STA_PNAMESIZE + 2)

struct sta_sel
{
#ifdef _HETERO
	unsigned char	cx_type[2];	/* = CX_STA */
	unsigned char	ss_size[2];	/* = STA_SELSIZE */
#else
	unsigned short	cx_type;	/* = CX_STA */
	unsigned short	ss_size;	/* = STA_SELSIZE */
#endif
	char		ss_stname[STA_STNAMESIZE];
					/* Station name in ASCII,
					   starting at the lowest address
					   (if necessary, the name must be
					   padded by trailing ASCII-blanks) */
	char		ss_term;	/* = 0 */
	char		ss_fill;	/* filler byte for structure
					   alignment */
};

struct sta_addr
{
#ifdef _HETERO
	unsigned char	cx_type[2];	/* = CX_STA */
	unsigned char	sa_type[2];	/* = STA_TYPE */
	unsigned char	sa_size[2];	/* = STA_ADDRSIZE */
#else
	unsigned short	cx_type;	/* = CX_STA */
	unsigned short	sa_type;	/* = STA_TYPE */
	unsigned short	sa_size;	/* = STA_ADDRSIZE */
#endif
	char		sa_pname[STA_PNAMESIZE];
					/* Processor name in ASCII,
					   consisting of a (3-digit) region
					   number, a slash (/), a (2-digit)
					   processor number and two ASCII-
					   blanks */
	char		sa_stname[STA_STNAMESIZE];
					/* Station name in ASCII,
					   starting at the lowest address
					   (if necessary, the name must be
					   padded by trailing ASCII-blanks) */
	char		sa_term;	/* = 0 */
	char		sa_fill;	/* filler byte for structure
					   alignment */
};

/*
 * Pseudo-CCP (SNA-Emulation) for SNA-interconnection
 */
#define CX_EMSNA	0x0040		/* CCP-Type for SNA Pseudo-CCP */
#define EMSNA_TYPE	128		/* Address type, ITRANS-irrelevant */
#define EMSNA_LUNAMESIZE 8
#define EMSNA_SELSIZE	(EMSNA_LUNAMESIZE + 2)
#define EMSNA_ADDRSIZE	EMSNA_SELSIZE

struct emsna_sel
{
#ifdef _HETERO
	unsigned char	cx_type[2];	/* = CX_EMSNA	*/
	unsigned char	es_size[2];	/* = EMSNA_SELSIZE */
#else
	unsigned short	cx_type;	/* = CX_EMSNA */
	unsigned short	es_size;	/* = EMSNA_SELSIZE */
#endif
	char		es_luname[EMSNA_LUNAMESIZE];
					/* LU-Name in EBCDIC,
					   starting at the lowest
					   address (if necessary, the name
					   must be padded by trailing EBCDIC-
					   blanks) */
	char		es_locadr;	/* SNA-Locaddr binary */
	char		es_fill;	/* filler byte for structure
					   alignment */
};

struct emsna_addr
{
#ifdef _HETERO
	unsigned char	cx_type[2];	/* = CX_EMSNA */
	unsigned char	ea_type[2];	/* = EMSNA_TYPE */
	unsigned char	ea_size[2];	/* = EMSNA_ADDRSIZE */
#else
	unsigned short	cx_type;	/* = CX_EMSNA */
	unsigned short	ea_type;	/* = EMSNA_TYPE */
	unsigned short	ea_size;	/* = EMSNA_ADDRSIZE */
#endif
	char		ea_luname[EMSNA_LUNAMESIZE];
					/* LU-Name in EBCDIC,
					   starting at the lowest
					   address (if necessary, the name
					   must be padded by trailing EBCDIC-
					   blanks) */
	char		ea_reg;		/* Region number binary */
	char		ea_pro;		/* Processor number multiplied by 8,
					   binary */
};

/***********************************************************************/
/*			CCP-independent part  			       */
/*		(excluding structure names in unions)	       	       */
/***********************************************************************/

/*
 * Constants
 */
#define CXS_SIZE	10		/* max. Length of a T-Selector */
#define CXA_SIZE	102		/* max. Length of a T-Address */
#define CXN_SELNUM	8		/* max. number of selectors in struct cx_myname */

/*
 * General structures for T-Selectors and T-Addresses
 */

struct cx_sel
{
#ifdef _HETERO
	unsigned char	cx_type[2];	/* CCP-Type */
	unsigned char	cxs_lng[2];	/* Length of the T-Selector */
#else
	unsigned short	cx_type;	/* CCP-Type */
	unsigned short	cxs_lng;	/* Length of the T-Selector */
#endif
	char		cxs_rest[CXS_SIZE];
					/* T-Selector field */
};

struct cx_addr
{
#ifdef _HETERO
	unsigned char	cx_type[2];	/* CCP-Type */
	unsigned char	cxa_ittype[2];	/* Address type according to ITRANS */
	unsigned char	cxa_lng[2];	/* Length of the address */
#else
	unsigned short	cx_type;	/* CCP-Type */
	unsigned short	cxa_ittype;	/* Address type according to ITRANS */
	unsigned short	cxa_lng;	/* Length of the Address */
#endif
	char		cxa_rest[CXA_SIZE];
					/* Address field */
};

/*
 * Unions for T-Selectors and T-Addresses
 */
union cx_unisel 			/* valid for all T-Selectors; max.
					   length is sizeof(struct cx_sel) ! */
{
	struct cx_sel	cxs;
	struct nea_sel	cxs_nea;
	struct sbka_sel cxs_sbka;
	struct sta_sel	cxs_sta;
	struct emsna_sel cxs_emsna;
};

union cx_uniaddr			/* valid for all T-Addresses; max.
					   length is sizeof(struct cx_addr) ! */
{
	struct cx_addr	cxa;
	struct nea_addr cxa_nea;
	struct sbka_addr cxa_sbka;
	struct sta_addr cxa_sta;
	struct emsna_addr cxa_emsna;
};

/*
 * Structure for local name
 */

struct cx_myname
{
#ifdef _HETERO
	unsigned char	cxn_selsize[2];	/* = sizeof(union cx_unisel) */
	unsigned char	cxn_abbr[4];	/* application-ID, must be
					   unambiguous in the local system */
#else
	unsigned short	cxn_selsize;	/* = sizeof(union cx_unisel) */
	unsigned short	cxn_abbr[2];	/* application-ID, must be
					   unambiguous in the local system */
#endif
	union cx_unisel cxn_sel[CXN_SELNUM];
};

/*
 * Some convenience macros
 */
#define CXS_FIXSIZE	(sizeof(struct cx_sel) - CXS_SIZE)
#define CXA_FIXSIZE	(sizeof(struct cx_addr) - CXA_SIZE)
#define CXN_PRESIZE	(sizeof(struct t_myname) - T_MNSIZE)
#define CXN_FIXSIZE	(sizeof(struct cx_myname) - \
				sizeof(union cx_unisel) * CXN_SELNUM)
#define CXP_PRESIZE	(sizeof(struct t_partaddr) - T_PASIZE)
#endif /* _CMXADDR_H */
