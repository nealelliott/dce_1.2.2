/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2spec_msk.h,v $
 * Revision 1.1.757.2  1996/02/18  23:36:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:31  marty]
 *
 * Revision 1.1.757.1  1995/12/08  16:01:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:19  root]
 * 
 * Revision 1.1.755.2  1994/05/10  15:56:05  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:38:52  marrek]
 * 
 * Revision 1.1.755.1  1994/02/22  18:53:52  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:48:19  marrek]
 * 
 * Revision 1.1.753.2  1993/10/14  17:27:49  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:27:03  keutel]
 * 
 * Revision 1.1.753.1  1993/10/13  17:32:41  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:26:49  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  12:56:57  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:43:41  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:42:21  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:30:39  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  04:14:05  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:43:32  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D2SPEC_MSK_H
#define _D2SPEC_MSK_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d2spec_msk_rcsid[] = "@(#)$RCSfile: d2spec_msk.h,v $ $Revision: 1.1.757.2 $ $Date: 1996/02/18 23:36:09 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton****************************************************************/
/*								      */
/* TYP		: INCLUDE					      */
/*								      */
/* NAME         : d2spec_msk.h  Directory-Typedefs fuer Non-BS-Masken */
/*								      */
/* AUTOR        : Mueller V.                                          */
/* DATUM        : 02.03.1991                                          */
/*								      */
/* KOMPONENTE	: DS						      */
/*								      */
/* DOK.-NR.	: DS-Designspezifikation (Ausgabe 1)		      */
/*								      */
/* PRD#/VERS.	:						      */
/*								      */
/* BESCHREIBUNG : enthaelt die Typedefs fuer die Datenstrukturen, in  */
/*                die die Werte aus den speziellen Masken geschrieben */
/*                werden.                                             */
/*								      */
/* SYSTEMABHAENGIGKEIT:	 COMMON					      */
/*								      */
/*								      */
/* Vers.Nr. |  Datum  |	 Aenderungen		       | KZ | CR# FM# */
/*   1.0    | 02.03.91| Original                       |    |         */
/*datoff***************************************************************/

/*----------------------some constants for DNS  ----------------------*/

#define CTSLEN                 42
#define DNS_STR_FNAME_MAX    1023  /* DNS String Full Name Max size */
#define DS_X500_TOWER_LEN_MAX 500

#define D20_MAX_CELL         (2*(CTSLEN + 1) + (DNS_STR_FNAME_MAX + 1) + 4)
				   /* maximal length of DNS-Cell Attribute */

#define D20_MAX_REP          ((CTSLEN + 1) + (DNS_STR_FNAME_MAX + 1) + DS_X500_TOWER_LEN_MAX + 5)
				   /* maximal length of DNS-Replica Attribute */


					 /*   --- M A S K   DNS CELL ---   */
#define DS_DCELL_NSPCTS         0
#define DS_DCELL_RCTS           1
#define DS_DCELL_RNM            2
#define DS_DCELL_CHG            3

#define D20_DCELLMOD            9
#define D20_DCELLCHG           10
#define D20_DCELLMENU          11
#define D20_DCELLCR            12
#define D20_DCELLDEL           13
#define D20_DCELLHELP          14
#define D20_DCELLSCD           15
#define D20_DCELLSCU           16
#define D20_DCELLF1            17

#define	D20_DC_FLD_LEN		36

					 /*   --- M A S K   REPLICA ---    */
#define DS_REP_TYP              0
#define DS_REP_CHCTS            1
#define DS_REP_CHNMA            2
#define DS_REP_CHNMB            3
#define DS_REP_TW1              4
#define DS_REP_TW2              5
#define DS_REP_TW3              6
#define DS_REP_TW4              7
#define DS_REP_TW5              8
#define DS_REP_CHG              9

#define D20_REPMOD             20
#define D20_REPCHG             21
#define D20_REPMENU            22
#define D20_REPCR              23
#define D20_REPDEL             24
#define D20_REPHELP            25
#define D20_REPSCD             26
#define D20_REPSCU             27
#define D20_REPF1              28

#define	D20_DR_FLD_LEN		45	

					 /*   --- M A S K   TTXID ---      */
#define DS_TT_TERM              0
#define DS_TT_CONTR             1
#define DS_TT_GRAPH             2
#define DS_TT_MISCL             3
#define DS_TT_PAGES             4
#define DS_TT_PRIV              5
#define DS_TT_CHG               6

#define D20_TTMOD              15
#define D20_TTCHG              16
#define D20_TTMENU             17
#define D20_TTCR               18
#define D20_TTDEL              19
#define D20_TTHELP             20
#define D20_TTSCD              21
#define D20_TTSCU              22
#define D20_TTF1               23

#define	D20_TT_TERM_FLD_LEN	24
#define	D20_TT_REST_FLD_LEN	45

					 /*   --- M A S K   TELEX ---      */
#define DS_TL_TELEX             0
#define DS_TL_CODE              1
#define DS_TL_ANSW              2
#define DS_TL_CHG               3

#define D20_TLMOD               9
#define D20_TLCHG              10
#define D20_TLMENU             11
#define D20_TLCR               12
#define D20_TLDEL              13
#define D20_TLHELP             14
#define D20_TLSCD              15
#define D20_TLSCU              16
#define D20_TLF1               17

#define	D20_TL_NO_FLD_LEN	14
#define	D20_TL_CC_FLD_LEN	4
#define	D20_TL_ANS_FLD_LEN	8

					 /*   --- M A S K   T61   ---      */
#define DS_T61_CHG              6

#define D20_T61MOD             15
#define D20_T61CHG             16
#define D20_T61MENU            17
#define D20_T61CR              18
#define D20_T61DEL             19
#define D20_T61HELP            20
#define D20_T61SCD             21
#define D20_T61SCU             22
#define D20_T61F1              23


					 /*   --- M A S K   FAX   ---      */
#define DS_FX_NR                0
#define DS_FX_A3W               1
#define DS_FX_B4L               2
#define DS_FX_B4W               3
#define DS_FX_FINERES           4
#define DS_FX_TDIM              5
#define DS_FX_UNCOM             6
#define DS_FX_UNLIML            7
#define DS_FX_CHG               8

#define D20_FXMOD              19
#define D20_FXCHG              20
#define D20_FXMENU             21
#define D20_FXCR               22
#define D20_FXDEL              23
#define D20_FXHELP             24
#define D20_FXSCD              25
#define D20_FXSCU              26
#define D20_FXF1               27

#define	D20_FX_NO_FLD_LEN	45

					 /*   --- M A S K   SHADOWED BY -- */
#define D20_SHBPOS1             0
#define D20_SHBPOS2             1
#define D20_SHBPOS3             2
#define D20_SHBPOS4             3
#define D20_SHBPOS5             4
#define D20_SHBPOS6             5
#define D20_SHBPOS7             6
#define D20_SHBPOS8             7

#define D20_SHBMENU             8
#define D20_SHBCR               9
#define D20_SHBDEL             10
#define D20_SHBHELP            11
#define D20_SHBSCD             12
#define D20_SHBSCU             13
#define D20_SHBF1              14

					 /*   --- M A S K   SAP: CERT ---- */
#define D20_CERTSCD            58
#define D20_CERTSCU            59
#define D20_CERTF1             60

#define D20_MAX_CERT           10

					 /*   --- M A S K   SAP: CERTP --- */
#define D20_CERTPSCD           13
#define D20_CERTPSCU           14
#define D20_CERTPF1            15
#define D20_CERT_LINE_LEN      45

#define D20_MAX_CERTP          10

					 /*   --- M A S K   SAP: CERTL --- */
#define D20_CERTLSCD           32
#define D20_CERTLSCU           33
#define D20_CERTLF1            34

#define D20_MAX_CERTL          10

					 /*   --- M A S K   SAP: RCERT --- */
#define D20_RCERTSCD           35
#define D20_RCERTSCU           36
#define D20_RCERTF1            37

					/*----- M A S K   DME ----------*/
	/* no of address lines.			*/
#define DME_NO_ADDRESS_LINES		5
	/* Length of first line			*/
#define DME_ADDRESS_FIRST_LINE_LEN	61
	/* Length of lines Nr 2 to Nr 5		*/
#define DME_ADDRESS_LINE_LEN		71
	/* Maximum of objects identifiers	*/
#define DME_MAX_OBJECT_ID		10
	/* Size of an object identifier		*/
#define DME_OBJECT_ID_LEN		28
	/* Size of the address			*/
#define DME_ADDRESS_LEN			DME_ADDRESS_FIRST_LINE_LEN + ((DME_NO_ADDRESS_LINES - 1) * DME_ADDRESS_LINE_LEN)
	/* Maximal length of DME attribute in the administration	*/
/* UNUSED NOW
#define D20_MAX_DME									(DME_ADDRESS_LEN + (DME_MAX_OBJECT_ID * DME_OBJECT_ID_LEN) )
*/
	/* Indexes in the mask			*/
#define D20_DMESCD             34
#define D20_DMESCU             35
#define D20_DMEF1              36

/*----------------------some other constants    ----------------------*/
#define D20_SH_MAX              8  /* maximal number of shadowed-by values */

#define D20_MAX_NB_ITEM         6  /* maximal number of t61-items          */
#define D20_POST_ADD_FLD_LEN	30 /* size of one postal address field.    */

#define D20_MAX_INT            10  /* maximal number of integers           */
#define D20_MAX_LEN            50  /* maximal length of the input-field    */
				   /* for special attributes               */
#define D20_MAX_NO             10  /* maximal number of input-fields for   */
				   /* special attributes                   */


#endif /* _D2SPEC_MSK_H */
