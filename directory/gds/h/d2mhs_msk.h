/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2mhs_msk.h,v $
 * Revision 1.1.10.2  1996/02/18  23:36:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:26  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:01:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:15  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:56:00  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:38:46  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:53:14  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:47:53  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:54:04  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:43:13  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:42:06  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:30:28  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  19:42:13  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:50:28  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:40:45  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D2MHS_MSK_H
#define _D2MHS_MSK_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d2mhs_msk_rcsid[] = "@(#)$RCSfile: d2mhs_msk.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:36:06 $";
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
/* NAME         : d2mhs_msk.h                                         */
/*								      */
/* AUTOR        : Volpers                                             */
/* DATUM        : 07.02.1991                                          */
/*								      */
/* KOMPONENTE	: DS						      */
/*								      */
/* DOK.-NR.	: DS-Designspezifikation (Ausgabe 1)		      */
/*								      */
/* PRD#/VERS.	:						      */
/*								      */
/* BESCHREIBUNG : enthaelt die Typedefs fuer die Datenstrukturen, in  */
/*                die die Werte aus Masken geschrieben werden.        */
/*								      */
/* SYSTEMABHAENGIGKEIT:	 COMMON					      */
/*								      */
/*								      */
/* Vers.Nr. |  Datum  |	 Aenderungen		       | KZ | CR# FM# */
/*   1.0    | 07.02.91| Original                       |    |         */
/*datoff***************************************************************/

/* ************************************************************************* */
/* -----------------------  M A S K   Or-add. Types ------------------------ */
/* ************************************************************************* */

#define DS_ORADD_TYPE	0

/******* Positions in Mnemonic mask. **********/
#define DS_ORADD_SCD_POS 	9
#define DS_ORADD_SCU_POS	10
#define DS_ORADD_F1_POS		11


/* ************************************************************************* */
/* ---------------------  M A S K   Oraddress field lengths ---------------- */
/* ************************************************************************* */

#define	D20_ORADD_CNT_FLD_LEN	3
#define	D20_ORADD_ADMD_FLD_LEN	16
#define	D20_ORADD_PRMD_FLD_LEN	16
#define	D20_ORADD_ORG_FLD_LEN	64
#define	D20_ORADD_OU1_FLD_LEN	32
#define	D20_ORADD_OU2_FLD_LEN	32
#define	D20_ORADD_OU3_FLD_LEN	32
#define	D20_ORADD_OU4_FLD_LEN	32
#define	D20_ORADD_CN_FLD_LEN	64
#define	D20_ORADD_GN_FLD_LEN	16
#define	D20_ORADD_INIT_FLD_LEN	5
#define	D20_ORADD_SN_FLD_LEN	40
#define	D20_ORADD_GEN_FLD_LEN	3
#define	D20_ORADD_DT_FLD_LEN	8
#define	D20_ORADD_DV_FLD_LEN	60

#define	D20_ORADD_NUI_FLD_LEN	32

#define	D20_ORADD_PCNT_FLD_LEN		3
#define	D20_ORADD_PC_FLD_LEN		16
#define	D20_ORADD_PAD_FLD_LEN		30
#define	D20_ORADD_PDPN_FLD_LEN		30
#define	D20_ORADD_PDSN_FLD_LEN		16
#define	D20_ORADD_PGDA_FLD_LEN		30
#define	D20_ORADD_PL_FLD_LEN		30
#define	D20_ORADD_POFBN_FLD_LEN		30
#define	D20_ORADD_POFNA_FLD_LEN		30
#define	D20_ORADD_POFNU_FLD_LEN		30
#define	D20_ORADD_PORGNA_FLD_LEN	30
#define	D20_ORADD_PPD_FLD_LEN		30
#define	D20_ORADD_PPN_FLD_LEN		30
#define	D20_ORADD_PSA_FLD_LEN		30

#define D20_ORADD_PAIF1_FLD_LEN		51
#define D20_ORADD_PAIF2_FLD_LEN		75
#define D20_ORADD_PAIF3_FLD_LEN		59
#define D20_ORADD_PAIL_FLD_LEN		30

#define	D20_ORADD_ISNO_FLD_LEN		15
#define	D20_ORADD_SUA_FLD_LEN		40
#define	D20_ORADD_X121_FLD_LEN		15
#define	D20_ORADD_TI_FLD_LEN		24

/* ************************************************************************* */
/* -----------------------  M A S K   Mnemonic Or-add.  -------------------- */
/* ************************************************************************* */

#define DS_MORADD_CNTRY		0
#define DS_MORADD_ADMD		1
#define DS_MORADD_PRMD		2
#define DS_MORADD_ORG	        3	
#define DS_MORADD_OU1	        4	
#define DS_MORADD_OU2	        5
#define DS_MORADD_OU3	        6	
#define DS_MORADD_OU4	        7	
#define DS_MORADD_CN		8	
#define DS_MORADD_GN		9
#define DS_MORADD_INTLS		10
#define DS_MORADD_SN		11
#define DS_MORADD_GEN		12
#define DS_MORADD_DT1		13
#define DS_MORADD_DT2		14
#define DS_MORADD_DT3		15
#define DS_MORADD_DT4		16
#define DS_MORADD_DV1		17
#define DS_MORADD_DV2		18
#define DS_MORADD_DV3		19
#define DS_MORADD_DV4		20
#define DS_MORADD_MOD		21

/******* Positions in Mnemonic mask. **********/
#define DS_MORADD_MOD_POS	47
#define DS_MORADD_SCD_POS	53
#define DS_MORADD_SCU_POS	54
#define DS_MORADD_F1_POS	55

/* ************************************************************************* */
/* -----------------------  M A S K   Numeric Or-add.  --------------------- */
/* ************************************************************************* */

#define DS_NORADD_CNTRY		0
#define DS_NORADD_ADMD		1
#define DS_NORADD_PRMD		2
#define DS_NORADD_NUI		3
#define DS_NORADD_DT1		4
#define DS_NORADD_DT2		5
#define DS_NORADD_DT3		6
#define DS_NORADD_DT4		7
#define DS_NORADD_DV1		8
#define DS_NORADD_DV2		9
#define DS_NORADD_DV3		10
#define DS_NORADD_DV4		11
#define DS_NORADD_MOD		12

/******* Positions in Numeric mask. **********/
#define DS_NORADD_MOD_POS	29
#define DS_NORADD_SCD_POS	35
#define DS_NORADD_SCU_POS	36
#define DS_NORADD_F1_POS	37

/* ************************************************************************* */
/* ------------------  M A S K   Structured postal Or-add.  ---------------- */
/* ************************************************************************* */

#define DS_SORADD_CNTRY		0
#define DS_SORADD_ADMD		1
#define DS_SORADD_PRMD		2
#define DS_SORADD_PCNTRY	3
#define DS_SORADD_PCODE		4
#define DS_SORADD_PAD		5
#define DS_SORADD_PDPN		6
#define DS_SORADD_PDSN		7
#define DS_SORADD_PGDA		8
#define DS_SORADD_PL		9
#define DS_SORADD_POBNR		10
#define DS_SORADD_PON		11
#define DS_SORADD_PONR		12
#define DS_SORADD_PORGN		13
#define DS_SORADD_PPD		14
#define DS_SORADD_PPN		15
#define DS_SORADD_PSA		16
#define DS_SORADD_MOD		17

/******* Positions in Structured Postal mask. **********/
#define DS_SORADD_MOD_POS	39
#define DS_SORADD_SCD_POS	45
#define DS_SORADD_SCU_POS	46
#define DS_SORADD_F1_POS	47

/* ************************************************************************* */
/* ------------------  M A S K   Unstructured postal Or-add.  -------------- */
/* ************************************************************************* */

#define DS_UORADD_CNTRY		0
#define DS_UORADD_ADMD		1
#define DS_UORADD_PRMD		2
#define DS_UORADD_PCNTRY	3
#define DS_UORADD_PCODE		4
#define DS_UORADD_PAIF		5
#define DS_UORADD_PAIL1	   	8
#define DS_UORADD_PAIL2	   	9
#define DS_UORADD_PAIL3		10
#define DS_UORADD_PAIL4		11
#define DS_UORADD_PAIL5		12
#define DS_UORADD_PAIL6		13
#define DS_UORADD_PDSN		14	
#define DS_UORADD_MOD		15	

/******* Positions in Unstructured postal mask. **********/
#define DS_UORADD_MOD_POS	33
#define DS_UORADD_SCD_POS	39
#define DS_UORADD_SCU_POS	40
#define DS_UORADD_F1_POS	41

/* ******S****************************************************************** */
/* -----------------------  M A S K   Term Or-add.  ------------------------ */
/* ************************************************************************* */

#define DS_TORADD_ISDNN		0
#define DS_TORADD_ISDNS		1
#define DS_TORADD_PA		2
#define DS_TORADD_X121		3	
#define DS_TORADD_CNTRY		4	
#define DS_TORADD_ADMD		5	
#define DS_TORADD_PRMD		6	
#define DS_TORADD_TI		7	
#define DS_TORADD_TT		8	
#define DS_TORADD_DT1		9	
#define DS_TORADD_DT2		10	
#define DS_TORADD_DT3		11	
#define DS_TORADD_DT4		12	
#define DS_TORADD_DV1		13	
#define DS_TORADD_DV2		14	
#define DS_TORADD_DV3		15	
#define DS_TORADD_DV4		16	
#define DS_TORADD_MOD		17	

/******* Positions in Terminal mask. **********/
#define DS_TORADD_MOD_POS	39
#define DS_TORADD_SCD_POS	45
#define DS_TORADD_SCU_POS	46
#define DS_TORADD_F1_POS	47

/*********** Constants for Types of OR Addresses *********/

#define D2_MORADD	0
#define D2_NORADD	1
#define D2_SORADD	2
#define D2_UORADD	3
#define D2_TORADD	4


/* ************************************************************************ */
/* -----------------------  M A S K   OR name .  -------------------------- */
/* ************************************************************************* */

#define	DS_ORN_DIRN1	0
#define	DS_ORN_DIRN2	1
#define	DS_ORN_ORA	2
#define	DS_ORN_MOD	3
#define	DS_ORN_DLST	4

/******* Positions in OR name mask. **********/

#define DS_ORN_DLST_POS  3
#define DS_ORN_MOD_POS	10
#define DS_ORN_SCD_POS	16
#define DS_ORN_SCU_POS	17
#define DS_ORN_F1_POS	18

/* ************************************************************************ */
/* -----------------------  M A S K   DL perm. types.  -------------------------- */
/* ************************************************************************* */
#define DS_DLPERM_TYPES	0

/******* Positions in DL perm mask. **********/
#define DS_DLPERM_SCD_POS	9
#define DS_DLPERM_SCU_POS	10	
#define DS_DLPERM_F1_POS	11	

/* ************************************************************************ */
/* -----------------------  M A S K   MOG .  -------------------------- */
/* ************************************************************************* */
#define DS_MOG_MOG1	0
#define DS_MOG_MOG2	1
#define DS_MOG_MOD	2

/******* Positions in Member of group (MOG) mask. **********/

#define DS_MOG_MOD_POS	7
#define DS_MOG_SCD_POS	13
#define DS_MOG_SCU_POS	14
#define DS_MOG_F1_POS	15

/* Len of postal address in full.		*/
#define D20_LEN_PAIF     (D20_ORADD_PAIF1_FLD_LEN + D20_ORADD_PAIF2_FLD_LEN + D20_ORADD_PAIF3_FLD_LEN + 1)


#define D20_LEN_ONE_PAIL	30
#define D20_LEN_ALL_PAIL	(6*D20_LEN_ONE_PAIL)

#define D20_MAX_STR_LIST	40	/* no. of str_list components in OR */
					/* address, including 6 components  */
					/* for post_addr_in_lines.	    */

/* Total no. of components in an OR addres.	*/
#define D20_MAX_ORADD_COMP	44

/* Max no. of OR addresses involved in an object.	*/
#define D20_MAX_ORADD		25

/* Max no. of OR names involved in an object.	*/
#define D20_MAX_ORNAME		25

/* Max no. of Distribution list submit permission structures involved 	*/
/* in an object.							*/
#define D20_MAX_DLSP		25

#endif /* _D2MHS_MSK_H */
