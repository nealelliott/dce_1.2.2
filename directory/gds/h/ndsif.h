/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndsif.h,v $
 * Revision 1.1.160.2  1996/02/18  23:36:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:56  marty]
 *
 * Revision 1.1.160.1  1995/12/08  16:02:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:36  root]
 * 
 * Revision 1.1.158.3  1994/07/06  15:07:36  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:16:37  marrek]
 * 
 * Revision 1.1.158.2  1994/06/09  18:46:27  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:32  devsrc]
 * 
 * Revision 1.1.158.1  1994/02/22  18:08:22  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:49:29  marrek]
 * 
 * Revision 1.1.156.2  1993/10/14  17:35:40  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:33:08  keutel]
 * 
 * Revision 1.1.156.1  1993/10/13  17:32:47  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  17:25:41  keutel]
 * 
 * $EndLog$
 */
#ifndef _NDSIF_H
#define _NDSIF_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _ndsif_rcsid[] = "@(#)$RCSfile: ndsif.h,v $ $Revision: 1.1.160.2 $ $Date: 1996/02/18 23:36:24 $";
#endif

#if !defined _XTI_H && !defined _TIUSER_H && !defined _SYS_TIUSER_H
#include <xti.h>
#endif
#include <netinet/in.h>
#include <dce/cmx_addr.h>

/* This header file provides all informations which are required */
/* to access the network directory service (NDS) from the user space */


	/* SUPPORTED ADDRESS MAPPING MODES */

#define NDS_OSITOXTI   (1 << 0)	/* map OSI-TSAP address(es) to XTI- */
				/* interface address(es) */
#define NDS_XTITOOSI   (1 << 1)	/* map XTI-interface address to OSI- */
				/* TSAP address */
#define NDS_CMXSEL     (1 << 2)	/* map OSI-TSAP address(es) to inter- */
				/* face specific OSI-address(es) */
#define NDS_OSITOSOCK  (1 << 3)	/* map OSI-TSAP address to SOCKET- */
				/* interface address */
#define NDS_SOCKTOOSI  (1 << 4)	/* map SOCKET-interface address to */
				/* OSI-TSAP address */
#define NDS_XTISEL     (1 << 5)	/* map OSI-TSAP address to no. of */
				/* supported XTI-service provider */
#define NDS_IPTOOSI    (1 << 6)	/* map internet address to OSI-address */


	/* IDENTIFIER OF SUPPORTED INTERFACES */

#define NDS_CMXSINFO	 (1 << 0)	/* CMX(S)-interface address info */
#define NDS_SOCKETINFO	 (1 << 3)	/* SOCKET-interface address info */
#define NDS_XTIINFO	 (1 << 4)	/* XTI-interface address info */


	/* SUPPORTED MAPPING RULES */

#define NDS_FMATCH	  1	/* map on first match */
#define NDS_HPRIO	  2	/* map on highest priority */

	/* FUNCTION RETURN VALUES AND NDS_ERRNO VALUES */

#define NDS_NOERR	0
#define NDS_ERR		-1
#define NDS_FATAL	-2

#define	NDS_TSAPERR	-51		/* invalid size of TSAP-address */
#define NDS_TSELERR	-52		/* invalid size of T-selector */
#define NDS_NSAPERR	-53		/* invalid size of NSAP-address or */
					/* invalid # of NSAP-addresses */
#define NDS_RULEPARERR	-54		/* wrong mapping rule parameter */
#define NDS_NORULESERR	-55		/* no matching rules available */
#define NDS_NOSELERR	-56		/* no NSAP-selection possible */
#define NDS_PROVERR	-57		/* invalid XTI-service provider */
					/* name or index */
#define NDS_BUFERR	-58		/* invalid output address buffer size */
#define NDS_IFDESCERR	-59		/* no interface description entry */
#define NDS_EXPRERR	-60		/* regular expression error */

	/* NDS FUNCTION PARAMETER STRUCTURES */

typedef struct {
      T_adrdesc		*nds_osi1addr ;		/* OSI-TSAP address 1 */
      T_adrdesc		*nds_osi2addr ;		/* OSI-TSAP address 2 */
      struct netbuf	nds_xti1addr ;		/* XTI-interface address 1 */
      struct netbuf	nds_xti2addr ;		/* XTI-interface address 2 */
      char		*nds_xtiprovnm ;	/* XTI-service provider name */
      int		nds_xtiprovidx ;	/* XTI-service provider index */
      int		nds_mprule ;		/* mapping rule */
      int		nds_errno ;		/* error reason */
} Nds_osixtiinfo ;

typedef struct {
      T_adrdesc		*nds_osi1addr ;		/* OSI-TSAP address 1 */
      T_adrdesc		*nds_osi2addr ;		/* OSI-TSAP address 2 */
      int		nds_iftypes ;		/* interface types */
      int		nds_mprule;		/* mapping rule */
      T_adrdesc		*nds_sel1addr ;		/* interface specific OSI- */
						/* TSAP address(es) 1 */
      T_adrdesc		*nds_sel2addr ;		/* interface specific OSI- */
						/* TSAP address 2 */
      int		nds_errno ;		/* error reason */
} Nds_cmxselinfo ;

typedef struct {
      T_adrdesc		*nds_osiaddr ;		/* OSI-TSAP address */
      struct sockaddr_in nds_sockaddr ;		/* SOCKET-interface address */
      int    		nds_ositselln ;		/* size of T-selector */
      char		nds_ositsel[T_TSELSIZE] ;/* T-selector value */
      int		nds_errno ;		/* error reason */
} Nds_osisockinfo ;

typedef struct {
      T_adrdesc		*nds_osiaddr ;		/* OSI-TSAP address */
      int		nds_noxtiprov ; 	/* no. of supported XTI- */
						/* service provider */
      int		nds_errno ;		/* error reason */
} Nds_xtiselinfo ;

typedef struct {
     struct in_addr	nds_ipaddr ;		/* IP-address */
     int    		nds_ositselln ;		/* size of T-selector */
     char		nds_ositsel[T_TSELSIZE] ;/* T-selector value */
     T_adrdesc		*nds_osiaddr ;		/* OSI-TSAP address */
     int		nds_errno ;		/* error reason */
} Nds_iposiinfo ;

typedef union {
     Nds_osixtiinfo	nds_mp1 ;
     Nds_cmxselinfo	nds_mp2 ;
     Nds_osisockinfo	nds_mp3 ;
     Nds_xtiselinfo	nds_mp4 ;
     Nds_iposiinfo	nds_mp5 ;
} Nds_mapinfo ;


	/* NDS FUNCTION PROTOTYPING */

extern int nds_map_addrinfo (const int, Nds_mapinfo *);
extern int (*NDS_map_addrinfo)(const int, Nds_mapinfo *); 

#endif /* _NDSIF_H */
