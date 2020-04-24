/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2shm.h,v $
 * Revision 1.1.10.2  1996/02/18  23:36:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:29  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:01:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:18  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:53:39  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:48:10  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:56:09  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:43:32  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:42:15  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:30:36  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  04:13:57  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:43:27  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D2SHM_H
#define _D2SHM_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d2shm_rcsid[] = "@(#)$RCSfile: d2shm.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:36:08 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton****************************************************************/
/*                                                                    */
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : d2shm.h      Directory-Typedefs fuer Schema V2      */
/*                                                                    */
/* AUTHOR       : Volpers D AP 11                                     */
/* DATE         : 21.01.1988                                          */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOK.-NR.     : DS-Design spezification                             */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : This Include-File contains data definitions and     */
/*                interfaces for scheme handling.                     */
/*                                                                    */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   1.0    | 20.08.87| Original                       |    |         */
/*datoff***************************************************************/

/**********************************************************************/
/**************** internnal constants *********************************/
/**********************************************************************/

#define D2_L_AT_NAME   25          /* max length of attribute name    */
#define D2_L_ACRONYM    3          /* max length of acronym           */
#define D2_NO_SUPCL    10          /* max no of superclasses of one   */
				   /* object class                    */
#define D2_NO_AUXCL    10          /* max no of auxiliary object      */
				   /* of one structural object class  */
#define D2_NO_MAND     10          /* max no of mandatory attributes  */
#define D2_NO_OPT      50          /* max no of optional attributes   */
				   /* corresponding to one oct entry  */


/*----------------------defines for ODT, ADT    ----------------------*/
/*----------------------elements in IA5 format  ----------------------*/

/* format definitions to read schema from  DSA */
#define D2_INFO_DELIM		' '
#define D2_FORMAT_SRT		"%d %d"
#define D2_NO_INFO_SRT		2
#define D2_FORMAT_OCT		"%s %s %s %d %d"
#define D2_NO_INFO_OCT		5
#define D2_ATT_SEPRATOR         ":"
#define D2_COLON                0x3a
#define D2_FORMAT_AT		"%s %s %s %d %d %d %d %d %d %d"
#define D2_NO_INFO_AT		10	

#define  D2_LC_SHORT        5  /* max. nr of positions of short in IA5*/
#define  D2_LC_LONG         10 /* max. nr of positions of long  in IA5*/
#define  D2_LC_BOOL         1  /* max. nr of positions of bool  in IA5*/
#define  D2_LC_MATCH        1  /* max. nr of positions of match in IA5*/
#define  D2_LC_D26_ACLASS   D2_LC_SHORT
			       /* max. nr of positions of D26_aclass  */
			       /* type in IA5.                        */
#define  D2_LC_DELIMITER    1  /* max. nr of positions of element     */
			       /* delimiter in IA5 format of SRT, ADT.*/

#define  D2_L_SRT_ATT   (D2_LC_SHORT + D2_LC_DELIMITER +         \
			 D2_LC_SHORT + D2_LC_DELIMITER +         \
			 D2_NO_AVA * (D2_L_ACRONYM + D2_LC_DELIMITER) + \
			 2 * D2_LC_DELIMITER + D2_L_ACRONYM)
			       /* max. nr of characters of SRT element*/
			       /* in printable format                 */

#define  D2_L_OCT_ATT   (D2_L_ACRONYM + D2_LC_DELIMITER +        \
			 D2_OBJ_IDL_MAX + D2_LC_DELIMITER +      \
			 D2_L_AT_NAME + D2_LC_DELIMITER +        \
			 D2_LC_SHORT + D2_LC_DELIMITER +         \
			 D2_LC_SHORT + D2_LC_DELIMITER +         \
			 (D2_L_ACRONYM + D2_LC_DELIMITER) * D2_NO_SUPCL + \
			 2 * D2_LC_DELIMITER +                   \
			 (D2_L_ACRONYM + D2_LC_DELIMITER) * D2_NO_AUXCL + \
			 2 * D2_LC_DELIMITER +                   \
			 (D2_L_ACRONYM + D2_LC_DELIMITER) * D2_NO_MAND + \
			 2 * D2_LC_DELIMITER +                   \
			 (D2_L_ACRONYM + D2_LC_DELIMITER) * D2_NO_OPT)
			       /* max. nr of characters of OCT element*/
			       /* in printable format.                */

#define  D2_L_AT_ATT    (D2_L_ACRONYM + D2_LC_DELIMITER +        \
			 D2_OBJ_IDL_MAX + D2_LC_DELIMITER +      \
			 D2_L_AT_NAME + D2_LC_DELIMITER +        \
			 D2_LC_SHORT + D2_LC_DELIMITER +         \
			 D2_LC_SHORT + D2_LC_DELIMITER +         \
			 D2_LC_SHORT + D2_LC_DELIMITER +         \
			 D2_LC_SHORT + D2_LC_DELIMITER +         \
			 D2_LC_SHORT + D2_LC_DELIMITER +         \
			 D2_LC_SHORT + D2_LC_DELIMITER +         \
			 D2_LC_SHORT + 1)
			       /* max. nr of characters of ADT element*/
			       /* in IA5 format.                      */

#define D2_INVALID_PRT_OBJ_ID   "---"   /* invalid object identifier */

#define D2_TOTAL_SCHEMA         1   /* flag to indicate complete schema  */
#define D2_DEF_SCHEMA           0   /* flag to indicate default schema   */


/* format definitions to read DSA scheme from a file */

#define  D2_S_MORE	    	5
#define  D2_O_MORE	    	5
#define  D2_A_MORE	    	20 

/* format definitions to store DSA scheme into a file */
#define  DSAAT_HEAD             "No_At: %d \n"
#define  DSAAT_FORMAT           "Abbrv: %s Obj_Id: %s Name: %s Lower-B: %d Upper-B: %d No-Rec: %d Syntax: %d ACL: %d Ind-Lev: %d Phon: %d \n"

#define	 DSAOCT_HEAD		"No-Oct: %d \n"
#define	 DSAOCT_FORMAT		"Abbrv: %s Obj-id: %s Name: %s Obj-class-kind: %d File-No: %d No-Sup: %d No-Aux: %d No-Mand: %d No-Opt: %d "

#define	 DSASRT_HEAD		"No-Srt: %d \n"
#define	 DSASRT_FORMAT		"Rule-No: %d Par-Rule-No: %d No-nam-attr: %d "

#define  D2_DSA_NUMAT           10 
#define  D2_DSA_NUMOCT          9 
#define  D2_DSA_NUMSRT          3

#define D20_SUTCTIME       14       /* max. size of the UTC-time string    */

#endif /* _D2SHM_H */
