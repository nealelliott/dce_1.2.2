/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxxso.h,v $
 * Revision 1.1.5.2  1996/03/11  13:24:34  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:56  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:48:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:43  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:46  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:55  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:43  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:51:53  marrek]
 * 
 * $EndLog$
 */
#ifndef _CMXXSO_H
#define _CMXXSO_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cmxxso_rcsid[] = "@(#)$RCSfile: cmxxso.h,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:34 $";
#endif

/* #ident	"@(#)cmxxso.h	311.7 93/08/24 CMX (NC1)" */

/*
 * COPYRIGHT (C) 1990 BY SIEMENS AG All rights reserved
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1993
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
 * Header file for XTI/TLI address mapping service in PCMX
 */

#define	ML_ADDR		1024	/* maximum address length	*/
typedef unsigned char	ui8;
typedef unsigned short	ui16;

/*
 * some helpful macros
 */
#define MIN(a,b) (((a) > (b)) ? (b) : (a))
#define MAX(a,b) (((a) < (b)) ? (b) : (a))


/*
 * Second parameter of TX_mn2nb and TX_pa2nb:
 *      Options to the address mapping service in TLV-coding
 *      (AddressMappingOptions)
 */
#define	AMO_SIZE	(sizeof (tx_opttlv))
#define	AMO_TLV_SIZE	128
typedef struct
{
   ui16		lng;		   /* length of the TLV part without lng*/
   ui8		tlv[AMO_TLV_SIZE]; /* TLV parts				*/
} tx_opttlv;

/*
 * one TLV-Part:
 *	ui8	option type
 *	ui8     length ov the valuefield
 *      ui8[]	value
 *
 * Option Type
 */
#define AMO_T_CFG     1 /* Options, null-terminated character string from   */
			/* cmxticfg file; the null byte is also counted in  */
			/* the length field                                 */
#define AMO_T_REDIR   2	/* PID for connection redirection		    */
#define AMO_T_MODE    3	/* 1 if listening TSAP				    */

/*
 * definitions for tpflag in the application structur
 */
#define TPF_NOCONNECT	0x0001
#define TPF_NODISCON	0x0002
#define TPF_ISOREASON	0x0008

/*
 * Reason mapping table; ends when values for xti and cmx are 0
 */
typedef struct
{
   int	xti;
   int	cmx;
} tx_reason;


/*
 * Type definition for the address mapping functions
 *
 * option functions currently not supported and not finally defined!
 */
typedef struct netbuf	  *(TX_mn2nb)(union cx_unisel *, tx_opttlv *);
typedef struct netbuf	  *(TX_pa2nb)(struct t_partaddr *, tx_opttlv *);
typedef struct t_partaddr *(TX_nb2pa)(struct netbuf *);
typedef struct netbuf	  *(TX_setopt)(char *);
typedef char		  *(TX_getopt)(struct netbuf *);
typedef void		   (TX_getinf)(int *, int *, tx_reason **);


typedef struct
{
   TX_mn2nb	*myname2netbuf;
   TX_pa2nb	*paddr2netbuf;
   TX_nb2pa	*netbuf2paddr;
   TX_setopt	*setopt;
   TX_getopt	*getopt;
   TX_getinf	*getinf;
} tx_am;

/* position of PAD digit in a DTE address				*/
/* defines for pv_mode							*/
#define PADM_LEAD	0x01	/* leadind pads, default trailing	*/
#define PADM_ZERO	0x02	/* pad with zeros, default 0x0f		*/

int tx_bcd_addr	(int		 pv_mode,
		 int		 pv_len,
		 unsigned char	*pv_bcdin,
		 unsigned char	*pv_bcdout);
#endif /* _CMXXSO_H */
