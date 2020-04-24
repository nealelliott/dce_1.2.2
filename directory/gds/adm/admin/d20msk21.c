/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20msk21.c,v $
 * Revision 1.1.726.2  1996/02/18  19:41:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:56  marty]
 *
 * Revision 1.1.726.1  1995/12/08  15:17:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:04  root]
 * 
 * Revision 1.1.724.5  1994/08/01  15:31:46  marrek
 * 	Bug fixes for OT 11295, 11456, 11458, 11471.
 * 	[1994/08/01  15:29:44  marrek]
 * 
 * Revision 1.1.724.4  1994/05/10  15:49:42  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:12:24  marrek]
 * 
 * Revision 1.1.724.3  1994/03/23  15:05:31  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:10:22  keutel]
 * 
 * Revision 1.1.724.2  1994/02/22  15:58:46  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:31:02  marrek]
 * 
 * Revision 1.1.724.1  1994/01/13  09:18:01  marrek
 * 	Use revised help masks.
 * 	[1994/01/12  11:12:02  marrek]
 * 
 * Revision 1.1.722.2  1993/10/14  16:46:32  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  16:44:37  keutel]
 * 
 * Revision 1.1.722.1  1993/10/13  17:29:41  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  07:35:40  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  10:16:37  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:31:58  marrek]
 * 
 * Revision 1.1.4.5  1993/01/29  08:37:26  marrek
 * 	page up introduced for structure rule mask
 * 	[1993/01/28  10:39:05  marrek]
 * 
 * Revision 1.1.4.4  1993/01/25  10:12:12  marrek
 * 	Various bug fixes for mask handling.
 * 	[1993/01/25  10:06:51  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:09:03  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:04:50  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:16:37  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  08:53:47  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:56:45  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:16:39  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20msk21.c,v $ $Revision: 1.1.726.2 $ $Date: 1996/02/18 19:41:12 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20mask.c                                           */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 28.01.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  : Mask Definition Module                              */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*  0.1     |28.01.88 |  original                      |    |         */
/*  0.2     |15.12.88 | Eingabe der Attribut-Laenge    | WS |     56  */
/*          |         | korrigiert                     |    |         */
/*  0.3     |02.01.89 | Anlegen Attribute auf Hinzu-   | HV |     61  */
/*          |         | fuegen Attribute geaendert     |    |         */
/*  0.4     |12.01.89 | Fehlertext D20_E80IDX fuer     | WS |     56  */
/*          |         | Schema-Administration geaendert|    |         */
/*  0.5     |16.02.89 | Fehlerbehebung bei             | HV |     72  */
/*          |         | Schema-Administration          |    |         */
/*datoff **************************************************************/

/*****************************************************************************/
/*                                                                           */
/*                 M A S K   D E F I N I T I O N   M O D U L E               */
/*                                                                           */
/*           (used by the administration of the directory system V2)         */
/*                                                                           */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  Author: R. Horn                                       Date: 28.1.88      */
/*****************************************************************************/

#include <gds.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <cget.h>


extern D20_omask d20_dnscellomask[];
extern D20_omask d20_dnsrepomask[];
extern D20_omask d20_ttxidomask[];
extern D20_omask d20_telexomask[];
extern D20_omask d20_t61omask[];
extern D20_omask d20_faxomask[];
extern D20_omask d20_oraddomask[];
extern D20_omask d20_moraddomask[];
extern D20_omask d20_noraddomask[];
extern D20_omask d20_soraddomask[];
extern D20_omask d20_uoraddomask[];
extern D20_omask d20_toraddomask[];
extern D20_omask d20_dlpermomask[];
extern D20_omask d20_ornameomask[];
extern D20_omask d20_mogomask[];
extern D20_omask d20_dmeomask[];
extern D20_omask d20_certomask[];
extern D20_omask d20_certpomask[];
extern D20_omask d20_certlomask[];
extern D20_omask d20_rcertomask[];

/* ************************************************************************* */
/* --------------------------  H E L P - M A S K   ------------------------- */
/* ************************************************************************* */

D20_omask d20helpomask[] = {
	D20_CSAY,  NORMAL,  2, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL,  4, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL,  5, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL,  6, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL,  7, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL,  8, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL,  9, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 10, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 11, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 12, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 13, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 14, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 15, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 16, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 17, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 18, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 19, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 20, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 21, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 23,12, NULL,        NULL, 0, FALSE, FALSE,
	D20_FKTX,  FKTDEL,  0, 0, NULL,        NULL, 0, FALSE, FALSE,
	D20_FKTX,  FKTCR,   0, 0, NULL,        NULL, 0, FALSE, FALSE,
	D20_FKTX,  FKTSCD,  0, 0, NULL,	  NULL, 0, FALSE, FALSE,
	D20_FKTX,  FKTSCU,  0, 0, NULL,	  NULL, 0, FALSE, FALSE,
	D20_EOMSK };

			/* input values for update times              */

char *d20_fhour;

char *d20_fd_of_week;

/* ************************************************************************* */
/* ------------------------ O P E R A T I O N S ---------------------------- */
/* ************************************************************************* */

char d20_actopmsg[] = {  "                                        " } ;

	    /* A D M I N I S T R A T I O N   F U N C T I O N S */
char *d20_op[D20_NOP];

	    /* CACHE operations */
char *d20_cop[D20_NCOP];

	    /* B A S I C  A D M I N I S T R A T I O N */
D20_opmsg d20_opmsg[] = { (signed16)D20_UNKNOWN,    "                     ",
			  (signed16)D20_OBJECT,     "Add Object           ",
			  (signed16)D20_OBJECT,     "Remove Object        ",
			  (signed16)D20_OBJECT,     "Display Objects      ",
			  (signed16)D20_OBJECT,     "Display Objects      ",
			  (signed16)D20_OBJECT,     "Add Attribute        ",
			  (signed16)D20_OBJECT,     "Delete Attribute     ",
			  (signed16)D20_OBJECT,     "Modify Attribute     ",
			  (signed16)D20_ALIAS,      "Add Alias            ",
			  (signed16)D20_RDN,        "Modify RDN           ",
			  (signed16)D20_UNKNOWN,    "                     "
			} ;

	    /* S C H E M E  A D M I N I S T R A T I O N */
D20_opmsg d20_s_opmsg[] = { (signed16)D20_UNKNOWN,    "                     ",
			    (signed16)D20_UNKNOWN,    "Store Schema",
			    (signed16)D20_UNKNOWN,    "Load Schema",
			    (signed16)D20_UNKNOWN,    "Display SRT",	
			    (signed16)D20_UNKNOWN,    "Add SRT entry",
			    (signed16)D20_UNKNOWN,    "Delete SRT entry",
			    (signed16)D20_UNKNOWN,    "Modify SRT entry",	
			    (signed16)D20_UNKNOWN,    "Display OCT",
			    (signed16)D20_UNKNOWN,    "Add OCT entry",
			    (signed16)D20_UNKNOWN,    "Delete OCT entry",
			    (signed16)D20_UNKNOWN,    "Modify OCT entry",	
			    (signed16)D20_UNKNOWN,    "Display AT",
			    (signed16)D20_UNKNOWN,    "Add AT entry",	
			    (signed16)D20_UNKNOWN,    "Delete AT entry",
			    (signed16)D20_UNKNOWN,    "Modify AT entry",
			    (signed16)D20_UNKNOWN,    "                     "
			  } ;


	    /* D E L T A - U P D A T E - F U N C T I O N S            */
char *d20_dupd_op[D20_NDUPD_OP];

	    /* T R E E - H A N D L I N G - F U N C T I O N S         */
char *d20_tree_op[D20_NTREE_OP];

char *d20_disp[D20_NDISP];

/* ************************************************************************* */
/* ------------------------------  M A S K  1 ------------------------------ */
/* ************************************************************************* */

		    /* format strings for RDNs that will dynamically be */
		    /* changed                                          */
static char d20_1oform[12][6] = { "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s" };

static signed32  d20_13field = 0;
static char d20_14field[] = { "_" } ;
static char d20_15field[] = { "_" } ;

static char d20_11ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_12ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_13ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_14ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_15ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_16ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_17ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_18ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_19ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_1aofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_1bofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_1cofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;

static char d20_12field[(D20_1_PW_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" };

static char d20_11ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_12ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_13ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_14ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_15ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_16ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_17ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_18ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_19ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_1aifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_1bifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_1cifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;

char *d20_1ifields[] = { d20_12field,
			 (char *) &d20_13field,
			 d20_14field,  d20_11ifield, d20_12ifield,
			 d20_13ifield, d20_14ifield, d20_15ifield,
			 d20_16ifield, d20_17ifield, d20_18ifield,
			 d20_19ifield, d20_1aifield, d20_1bifield,
			 d20_1cifield, d20_15field } ;

D20_omask d20_1omask[] = {
	D20_CSAY,    NORMAL,  7,  7, "Password:",                       NULL, 0, FALSE, FALSE,
	D20_CGET,    DUNKEL,  7, 36, d20_12field,                       "%-32s", 0, FALSE, FALSE,
	D20_NO_OP,    NORMAL,  9,  7, d20_11ofield,			NULL, 0, FALSE, FALSE,
	D20_NO_OP,  HALBHELL,  9, 36, d20_11ifield,                      d20_1oform[0], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_NO_OP,    NORMAL, 10,  7, d20_12ofield,			NULL, 0, FALSE, FALSE,
	D20_NO_OP,  HALBHELL, 10, 36, d20_12ifield,                      d20_1oform[1], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_NO_OP,    NORMAL, 11,  7, d20_13ofield,			NULL, 0, FALSE, FALSE,
	D20_NO_OP,  HALBHELL, 11, 36, d20_13ifield,                      d20_1oform[2], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_NO_OP,    NORMAL, 12,  7, d20_14ofield,			NULL, 0, FALSE, FALSE,
	D20_NO_OP,  HALBHELL, 12, 36, d20_14ifield,                      d20_1oform[3], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_NO_OP,    NORMAL, 13,  7, d20_15ofield,			NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 13, 36, d20_15ifield,                      d20_1oform[4], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_NO_OP,    NORMAL, 14,  7, d20_16ofield,			NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 14, 36, d20_16ifield,                      d20_1oform[5], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_NO_OP,    NORMAL, 15,  7, d20_17ofield,			NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 15, 36, d20_17ifield,                      d20_1oform[6], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_NO_OP,    NORMAL, 16,  7, d20_18ofield,			NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 16, 36, d20_18ifield,                      d20_1oform[7], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_NO_OP,    NORMAL, 17,  7, d20_19ofield,			NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 17, 36, d20_19ifield,                      d20_1oform[8], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_NO_OP,    NORMAL, 18,  7, d20_1aofield,			NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 18, 36, d20_1aifield,                      d20_1oform[9], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_NO_OP,    NORMAL, 19,  7, d20_1bofield,			NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 19, 36, d20_1bifield,                      d20_1oform[10], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_NO_OP,    NORMAL, 20,  7, d20_1cofield,			NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 20, 36, d20_1cifield,                      d20_1oform[11], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 21, 10, "Options:",			NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 21, 19, (char *) &d20_13field, 
	"%.0=Logon to the Default-DSA.1=Logon to a Specific DSA . \
	2=Logon to the DUA-Cache  .3=Changing Name Structure .a", 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_14field,			NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_15field,                       NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;



/* ************************************************************************* */
/* ------------------ SECURITY LOGON M A S K  1 ---------------------------- */
/* ************************************************************************* */

static signed32	d20_msk_dir_id = 1;
static signed32	d20_sec_mech = 0;

char *d20_secifields[] = { (char *) &d20_msk_dir_id, (char *) &d20_sec_mech } ;

D20_omask d20_secomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 1)             DIRECTORY SYSTEM                           logon",
 NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  4, 48, "Directory-Identifier:",           NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  4, 70, (char *) &d20_msk_dir_id,          "%-2d", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  5,  7, "Security mechanism:",		NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  5, 39, (char *) &d20_sec_mech,		NULL, 0, FALSE, FALSE,
	D20_NO_OP,    NORMAL,21, 10, "Options: Logon to the DUA cache",	NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,			        NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                      	NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;



/* ************************************************************************* */
/* ------------------------------  M A S K  2 ------------------------------ */
/* ************************************************************************* */

		    /* format strings for RDNs that will dynamically be */
		    /* changed                                          */
static char d20_2oform[12][6] = { "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s" };

static signed32  d20_21field ;
static char d20_22field[] = { "_" } ;
static char d20_23field[] = { "_" } ;

static char d20_21ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_22ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_23ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_24ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_25ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_26ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_27ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_28ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_29ofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_2aofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_2bofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;
static char d20_2cofield[D20_ONM_NM_FLD_LEN + 1] = { "" } ;

static char d20_21ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_22ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_23ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_24ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_25ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_26ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_27ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_28ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_29ifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_2aifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_2bifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_2cifield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;

char *d20_2_1opt;
char *d20_2_2opt;

char *d20_2ifields[] = { (char *) &d20_21field,
			 d20_22field,  d20_21ifield, d20_22ifield,
			 d20_23ifield, d20_24ifield, d20_25ifield,
			 d20_26ifield, d20_27ifield, d20_28ifield,
			 d20_29ifield, d20_2aifield, d20_2bifield,
			 d20_2aifield, d20_23field } ;

D20_omask d20_2omask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 2)             DIRECTORY SYSTEM",                        NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, d20_actopmsg,                      NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  5,  5, "DSA-IDENTIFICATION:",             NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9,  7, d20_21ofield,                      NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 36, d20_21ifield,                      d20_2oform[0], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 10,  7, d20_22ofield,                      NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 10, 36, d20_22ifield,                      d20_2oform[1], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 11,  7, d20_23ofield,                      NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 36, d20_23ifield,                      d20_2oform[2], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 12,  7, d20_24ofield,                      NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 36, d20_24ifield,                      d20_2oform[3], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 13,  7, d20_25ofield,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 13, 36, d20_25ifield,                      d20_2oform[4], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 14,  7, d20_26ofield,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 14, 36, d20_26ifield,                      d20_2oform[5], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 15,  7, d20_27ofield,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 15, 36, d20_27ifield,                      d20_2oform[6], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 16,  7, d20_28ofield,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 16, 36, d20_28ifield,                      d20_2oform[7], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 17,  7, d20_29ofield,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 17, 36, d20_29ifield,                      d20_2oform[8], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 18,  7, d20_2aofield,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 18, 36, d20_2aifield,                      d20_2oform[9], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 19,  7, d20_2bofield,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 19, 36, d20_2bifield,                      d20_2oform[10], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 20,  7, d20_2cofield,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 20, 36, d20_2cifield,                      d20_2oform[11], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 21, 10, "Options:",                        NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 21, 19, (char *) &d20_21field, 
	"%.0=None                   .1=Changing Name Structure.a", 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_22field,                       NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_23field,                       NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K  3 ------------------------------ */
/* ************************************************************************* */

static char d20_31field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_32field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;

static signed32  d20_33field = 1 ;

static char d20_34field[] = { "_" } ;
static char d20_35field[] = { "_" } ;

char *d20_3ifields[] = { (char *)&d20_33field, d20_34field, d20_35field } ;

char *d20_3ofields[] = { d20_31field, d20_32field } ;

D20_omask d20_3omask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 3)             DIRECTORY SYSTEM",			NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, "ADMINISTRATION FUNCTIONS:",	NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9, 10, "0 Exit",                          NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11, 10, "1 Object Administration",         NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 10, "2 Schema Administration",         NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 10, "2 Cache Update         ",         NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15, 10, "3 Shadow Administration",         NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 17, 10, "4 Subtree Administration",        NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 19,  8, "Current DSA",                     NULL, 0, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 19, 20, d20_31field,                       "%-45s", D20_ATVAL_FLD_LEN, TRUE, TRUE,
	D20_CSAY,  HALBHELL, 20, 20, d20_32field,                       "%-45s", D20_ATVAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 23,  5, "Which function ?",		NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 23, 22, (char *)&d20_33field,              "%-1d", 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_34field,                       NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_35field,                       NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K  4 ------------------------------ */
/* ************************************************************************* */

static signed32   d20_41field = 1 ;
static char  d20_42field[] = { "_" } ;
static signed32   d20_43field = 0 ;
static char  d20_44field[] = { "_" } ;

char *d20_4ifields[] = { (char *) &d20_41field, d20_42field,
			 (char *) &d20_43field, d20_44field } ;

D20_omask d20_4omask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 4)             DIRECTORY SYSTEM",			NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, "OPERATIONS",			NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, " 0 Exit",                         NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9, 10, " 1 Add Object",                   NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 10, 10, " 2 Remove Object",                NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11, 10, NULL,                           NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 10, NULL,                           NULL, 0, FALSE, FALSE,
        D20_CSAY,    NORMAL, 13, 10, NULL,                           NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14, 10, NULL,                           NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15, 10, NULL,                           NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 16, 10, " 8 Add Alias",                    NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 17, 10, " 9 Modify RDN",                   NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  6, 40, "Entry-Type:",                     NULL, 0, FALSE, FALSE,
	D20_CGET,    NORMAL,  6, 52, (char *)&d20_43field,
				     "%.0=MASTER.1=SHADOW.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 23,  5, "Which operation ?",               NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL | CURSOR, 23, 23, (char *)&d20_41field,     "%-3d", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, d20_actopmsg,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_42field,			NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_44field,                       NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K  4a ----------------------------- */
/* ************************************************************************* */

static char d20_4a1field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_4a2field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_4a3field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_4a4field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_4a5field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_4a6field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_4a7field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_4a8field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_4a9field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_4aafield[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;

static char d20_4abfield[] = { "_" } ;
static char d20_4acfield[] = { "_" } ;

char *d20_4aifields[] = { d20_4a1field, d20_4a2field, d20_4a3field,
			  d20_4a4field, d20_4a5field, d20_4a6field,
			  d20_4a7field, d20_4a8field, d20_4a9field,
			  d20_4aafield, d20_4abfield, d20_4acfield };

D20_omask d20_4aomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 4a)            DIRECTORY SYSTEM",                        NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, "Display Local/Default-DSA",       NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, "Special DSA's",                   NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8,  9, "Local:",                          NULL, 0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  8, 18, d20_4a1field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, TRUE,
	D20_CSAY,  HALBHELL,  9, 18, d20_4a2field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 11,  9, "default:",                        NULL, 0, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 11, 18, d20_4a3field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, TRUE,
	D20_CSAY,  HALBHELL, 12, 18, d20_4a4field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,  HALBHELL, 14, 18, d20_4a5field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, TRUE,
	D20_CSAY,  HALBHELL, 15, 18, d20_4a6field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,  HALBHELL, 17, 18, d20_4a7field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, TRUE,
	D20_CSAY,  HALBHELL, 18, 18, d20_4a8field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,  HALBHELL, 20, 18, d20_4a9field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, TRUE,
	D20_CSAY,  HALBHELL, 21, 18, d20_4aafield,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_4abfield,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_4acfield,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ************************************************************************* */
/* ------------------------------  M A S K  5 ------------------------------ */
/* ************************************************************************* */

static signed32  d20_51field;
static char d20_52field[] = { "_" } ;
static char d20_53field[] = { "_" } ;
static char d20_54field[] = { "_" } ;

char *d20_5ifields[] = { (char *)&d20_51field, d20_52field, d20_53field,
			 d20_54field } ;

D20_omask d20_5omask[] = {
	D20_CSAY, UNDERLINE,  5,  8, "Object Type of Object",           NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  5,  8, "Object Type of Alias Object",     NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  5,  8, "Object Type of Aliased Object",   NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  5, 43, "Tree Structure",			NULL, 0, FALSE, FALSE,
	D20_NO_OP,   NORMAL,  8,  5, "00 ROOT",                         NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9,  5, NULL,                           "%-2s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9,  8, NULL,                           "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9, 43, NULL,                           "%-36s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 10,  5, NULL,                           "%-2s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 10,  8, NULL,                           "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 10, 43, NULL,                           "%-36s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11,  5, NULL,                           "%-2s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11,  8, NULL,                           "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11, 43, NULL,                           "%-36s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12,  5, NULL,                           "%-2s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12,  8, NULL,                           "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 43, NULL,                           "%-36s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13,  5, NULL,                           "%-2s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13,  8, NULL,                           "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 43, NULL,                           "%-36s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14,  5, NULL,                           "%-2s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14,  8, NULL,                           "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14, 43, NULL,                           "%-36s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15,  5, NULL,                           "%-2s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15,  8, NULL,                           "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15, 43, NULL,                           "%-36s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 16,  5, NULL,                           "%-2s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 16,  8, NULL,                           "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 16, 43, NULL,                           "%-36s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 17,  5, NULL,                           "%-2s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 17,  8, NULL,                           "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 17, 43, NULL,                           "%-36s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 18,  5, NULL,                           "%-2s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 18,  8, NULL,                           "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 18, 43, NULL,                           "%-36s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 19,  5, NULL,                           "%-2s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 19,  8, NULL,                           "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 19, 43, NULL,                           "%-36s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 20,  5, NULL,                           "%-2s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 20,  8, NULL,                           "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 20, 43, NULL,                           "%-36s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 23,  5, "Which object type ?",             NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 23, 25, (char *)&d20_51field,              "%-2d", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 5)             DIRECTORY SYSTEM",			NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, d20_actopmsg,                      NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  5,  8, "Structure Rule of New Object",    NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTSCD,  0,  0, d20_52field,			NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_53field,			NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_54field,                       NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTSCU,   0,  0, NULL,                       	NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K  6 ------------------------------ */
/* ************************************************************************* */

		    /* format strings for RDNs that will dynamically be */
		    /* changed                                          */
static char d20_6oform[12][6] = { "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s",
				  "%-25s" };

static signed32  d20_60field = 0 ;

static char d20_61field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" } ;
static char d20_62field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" } ;
static char d20_63field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" } ;
static char d20_64field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" } ;
static char d20_65field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" } ;
static char d20_66field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" } ;
static char d20_67field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" } ;
static char d20_68field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" } ;
static char d20_69field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" } ;
static char d20_610field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_611field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;
static char d20_612field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] = { "" } ;

static signed32  d20_613field = FALSE;

static char d20_614field[] = { "_" } ;
static char d20_615field[] = { "_" } ;
static char d20_616field[] = { "_" } ;
static char d20_617field[] = { "_" } ;

static signed32  d20_618field;

static signed32  d20_620field = FALSE;

char *d20_6ifields[] = { d20_61field,  d20_62field,  d20_63field,  d20_64field,
			 d20_65field,  d20_66field,  d20_67field,  d20_68field,
			 d20_69field,  d20_610field, d20_611field, d20_612field,
			 (char *) &d20_613field, d20_614field, d20_615field, 
			 d20_616field, d20_617field, (char *) &d20_618field , 
			 (char *)&d20_620field } ;

D20_omask d20_6omask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 6)             DIRECTORY SYSTEM",			NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, d20_actopmsg,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP,   NORMAL,  5,  5, "Object interpretation:",          NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL,  5, 36, (char *) &d20_60field,
				     "%.0=SINGLE OBJECT              .\
					1=OBJECT AND ITS SUBORDINATES.a", 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, "Object Name",                     NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, "Alias Name",                      NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, "Aliased Name",                    NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, "New Object Name",                 NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 36, d20_61field,                       d20_6oform[0], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  9, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 36, d20_62field,                       d20_6oform[1], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 10, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 10, 36, d20_63field,                       d20_6oform[2], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 11, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 36, d20_64field,                       d20_6oform[3], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 12, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 36, d20_65field,                       d20_6oform[4], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 13, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 36, d20_66field,                       d20_6oform[5], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 14, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 36, d20_67field,                       d20_6oform[6], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 15, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 36, d20_68field,                       d20_6oform[7], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 16, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 16, 36, d20_69field,                       d20_6oform[8], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 17, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 17, 36, d20_610field,                      d20_6oform[9], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 18, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 18, 36, d20_611field,                      d20_6oform[10], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 19, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 19, 36, d20_612field,                      d20_6oform[11], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 20,  6, "Object class:",                   NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 20, 20, (char *) &d20_618field,            NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 21,  6, "Auxiliary object class:",         NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 21, 30, (char *) &d20_620field,      "%.0=N.1=Y.a", 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, d20_614field,			NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, d20_614field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTSCD,  0,  0, d20_615field,			NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_616field,			NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_617field,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K  6a ----------------------------- */
/* ************************************************************************* */

static signed32  d20_6a1field = FALSE;
static signed32  d20_6a2field = FALSE;
static signed32  d20_6a3field = FALSE;
static signed32  d20_6a4field = FALSE;
static signed32  d20_6a5field = FALSE;
static char d20_6a6field[]  = { "_" } ;
static char d20_6a7field[]  = { "_" } ;

char *d20_6aifields[] = { (char *) &d20_6a1field, (char *) &d20_6a2field, (char *) &d20_6a3field,
			  (char *) &d20_6a4field, (char *) &d20_6a5field, d20_6a6field,
			  d20_6a7field };

D20_omask d20_6aomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 6a)            DIRECTORY SYSTEM",			NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,			NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, "Access Rights:",                  NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, "Modify Public:",                  NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 29, (char *) &d20_6a1field,      "%.0=N.1=Y.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9, 10, "Read Standard:",                  NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 29, (char *) &d20_6a2field,      "%.0=N.1=Y.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 10, 10, "Modify Standard:",                NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 10, 29, (char *) &d20_6a3field,      "%.0=N.1=Y.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11, 10, "Read Sensitive:",                 NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 29, (char *) &d20_6a4field,      "%.0=N.1=Y.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 10, "Modify Sensitive:",               NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 29, (char *) &d20_6a5field,      "%.0=N.1=Y.a", 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_6a6field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_6a7field,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K  6b ----------------------------- */
/* ************************************************************************* */

static char d20_6b1field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" };
static char d20_6b2field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" };

static signed32  d20_6b3field  = (signed32) (D2_IGNORE - '0');

static char d20_6b4field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" };
static char d20_6b5field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" };

static signed32  d20_6b6field  = (signed32) (D2_IGNORE - '0');

static char d20_6b7field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" };
static char d20_6b8field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" };

static signed32  d20_6b9field  = (signed32) (D2_IGNORE - '0');

static char d20_6bafield[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" };
static char d20_6bbfield[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  = { "" };

static signed32  d20_6bcfield  = (signed32) (D2_IGNORE - '0');

static char d20_6bdfield[]  = { "_" } ;
static char d20_6befield[]  = { "_" } ;
static char d20_6bffield[]  = { "_" } ;
static char d20_6bgfield[]  = { "_" } ;
static char d20_6bhfield[]  = { "_" } ;
static char d20_6bifield[]  = { "_" } ;

char *d20_opclass[D20_NOPCLASS];

char *d20_6bifields[] = { d20_6b1field, d20_6b2field, (char *) &d20_6b3field,
			  d20_6b4field, d20_6b5field, (char *) &d20_6b6field,
			  d20_6b7field, d20_6b8field, (char *) &d20_6b9field,
			  d20_6bafield, d20_6bbfield, (char *) &d20_6bcfield,
			  d20_6bdfield, d20_6befield, d20_6bffield,
			  d20_6bgfield, d20_6bhfield, d20_6bifield } ;

D20_omask d20_6bomask[] = {
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,			NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, "Access Right:",                   NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  6, 25, NULL,                           NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9, 13, "Distinguished Names (DNs):",      NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9, 60, "interpretation",                  NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 13, d20_6b1field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, TRUE,
	D20_CGET,  HALBHELL, 12, 13, d20_6b2field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, FALSE,
	D20_CGET,  HALBHELL, 12, 66, (char *) &d20_6b3field,   "%.0=OBJECT.1=SUBTREE.a", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 13, d20_6b4field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, TRUE,
	D20_CGET,  HALBHELL, 15, 13, d20_6b5field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, FALSE,
	D20_CGET,  HALBHELL, 15, 66, (char *) &d20_6b6field,   "%.0=OBJECT.1=SUBTREE.a", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 17, 13, d20_6b7field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, TRUE,
	D20_CGET,  HALBHELL, 18, 13, d20_6b8field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, FALSE,
	D20_CGET,  HALBHELL, 18, 66, (char *) &d20_6b9field,   "%.0=OBJECT.1=SUBTREE.a", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 20, 13, d20_6bafield,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, TRUE,
	D20_CGET,  HALBHELL, 21, 13, d20_6bbfield,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, FALSE,
	D20_CGET,  HALBHELL, 21, 66, (char *) &d20_6bcfield,   "%.0=OBJECT.1=SUBTREE.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 6b)            DIRECTORY SYSTEM",			NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, d20_6bdfield,			NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, d20_6bdfield,			NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_6befield,			NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, d20_6bffield,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_6bgfield,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, d20_6bhfield,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTF1,   0,  0, d20_6bifield,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;



/* ************************************************************************* */
/* ------------------------------  M A S K  7 ------------------------------ */
/* ************************************************************************* */

static char d20_71field[D20_ONM_NM_FLD_LEN + 1]
		    = { "_________________________" };
static char d20_72field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]
		    = { "_____________________________________________" };
static char d20_73field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]
		    = { "_____________________________________________" };

static char d20_74field[D20_ONM_NM_FLD_LEN + 1]
		    = { "_________________________" };
static char d20_75field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]
		    = { "_____________________________________________" };
static char d20_76field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]
		    = { "_____________________________________________" };

static char d20_77field[D20_ONM_NM_FLD_LEN + 1]
		    = { "_________________________" } ;
static char d20_78field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]
		    = { "_____________________________________________" };
static char d20_79field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]
		    = { "_____________________________________________" };

static char d20_710field[] = { "_" } ;
static char d20_711field[] = { "_" } ;
static char d20_712field[] = { "_" } ;
static char d20_713field[] = { "_" } ;
static char d20_714field[] = { "_" } ;
static char d20_715field[] = { "_" } ;

static char d20_71hfld[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT * 2) + 1]
= {"__________________________________________________________________________________________"};
static char d20_72hfld[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT * 2) + 1]
= {"__________________________________________________________________________________________"};
static char d20_73hfld[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT * 2) + 1]
= {"__________________________________________________________________________________________"};

char *d20_7ifields[] = { d20_71field,  d20_72field,  d20_73field,  d20_74field,
			 d20_75field,  d20_76field,  d20_77field,  d20_78field,
			 d20_79field,  d20_710field, d20_711field, d20_712field,
			 d20_713field, d20_714field, d20_715field } ;

char *d20_7hflds[] = { d20_71hfld,  d20_72hfld,  d20_73hfld };

/* conversion flag is set dynamically.	*/

D20_omask d20_7omask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 7)             DIRECTORY SYSTEM",			NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,			NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, "Attributes:",			NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, "Name :",				NULL, 0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  8, 18, d20_71field,                       "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9, 10, "Value:",				NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 18, d20_72field,                       "%-45s", D20_ATVAL_FLD_LEN, FALSE, TRUE,
	D20_CGET,  HALBHELL, 10, 18, d20_73field,                       "%-45s", D20_ATVAL_FLD_LEN, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 10, "Name :",				NULL, 0, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 12, 18, d20_74field,                       "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 10, "Value:",				NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 18, d20_75field,                       "%-45s", D20_ATVAL_FLD_LEN, FALSE, TRUE,
	D20_CGET,  HALBHELL, 14, 18, d20_76field,                       "%-45s", D20_ATVAL_FLD_LEN, FALSE, FALSE,
	D20_CSAY,    NORMAL, 16, 10, "Name :",				NULL, 0, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 16, 18, d20_77field,                       "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 17, 10, "Value:",				NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 17, 18, d20_78field,                       "%-45s", D20_ATVAL_FLD_LEN, FALSE, TRUE,
	D20_CGET,  HALBHELL, 18, 18, d20_79field,                       "%-45s", D20_ATVAL_FLD_LEN, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, d20_710field,			NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, d20_710field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTSCD,  0,  0, d20_711field,			NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_712field,			NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_713field,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, d20_714field,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTF1,   0,  0, d20_715field,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K  7a ----------------------------- */
/* ************************************************************************* */



static char d20_7a1field[D20_7A_PS_FLD_LEN + 1] =
		{ "___________________________________" };
static char d20_7a2field[D20_7A_SS_FLD_LEN + 1] =
		{ "___________________________________" };
static char d20_7a3field[D20_7A_TS_FLD_LEN + 1] =
		{ "_______________________" };
static char d20_7a4field[D20_7A_NA_FLD_LEN + 1] =
		{ "________________________________________________________" };
static char d20_7a5field[D20_7A_NA_FLD_LEN + 1] =
		{ "________________________________________________________" };
static char d20_7a6field[D20_7A_NA_FLD_LEN + 1] =
		{ "________________________________________________________" };
static char d20_7a7field[D20_7A_NA_FLD_LEN + 1] =
		{ "________________________________________________________" };
static char d20_7a8field[D20_7A_NA_FLD_LEN + 1] =
		{ "________________________________________________________" };

static char d20_7a9field[] = { "_" } ;
static char d20_7aafield[] = { "_" } ;
static char d20_7abfield[] = { "_" } ;
static char d20_7acfield[] = { "_" } ;
static char d20_7adfield[] = { "_" } ;

char *d20_7aifields[] = { d20_7a1field, d20_7a2field, d20_7a3field,
			  d20_7a4field, d20_7a5field, d20_7a6field,
			  d20_7a7field, d20_7a8field, d20_7a9field,
			  d20_7aafield, d20_7abfield, d20_7acfield,
			  d20_7adfield } ;

D20_omask d20_7aomask[] = {
	D20_CSAY, UNDERLINE,  5,  5, "P-Selector:",                     NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  5, 20, d20_7a1field,                      "%-35s", 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  7,  5, "S-Selector:",                     NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 20, d20_7a2field,                      "%-35s", 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  9,  5, "T-Selector:",                     NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 20, d20_7a3field,                      "%-23s", 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE, 12,  5, "Net-Address 1:",                  NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 20, d20_7a4field,                      "%-56s", 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE, 14,  5, "Net-Address 2:",                  NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 20, d20_7a5field,                      "%-56s", 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE, 16,  5, "Net-Address 3:",                  NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 16, 20, d20_7a6field,                      "%-56s", 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE, 18,  5, "Net-Address 4:",                  NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 18, 20, d20_7a7field,                      "%-56s", 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE, 20,  5, "Net-Address 5:",                  NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 20, 20, d20_7a8field,                      "%-56s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 7a)            DIRECTORY SYSTEM",			NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,			NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, d20_7a9field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_7aafield,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_7abfield,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, d20_7acfield,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTF1,   0,  0, d20_7adfield,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ************************************************************************* */
/* ------------------------------  M A S K  8 ------------------------------ */
/* ************************************************************************* */

static char d20_81field[D20_ONM_NM_FLD_LEN + 1]  ={ "" };

static char d20_82field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  ={ "" };
static char d20_83field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  ={ "" };
static char d20_84field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  ={ "" };
static char d20_85field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  ={ "" };

static char d20_86field[]  = { "_" } ;
static char d20_87field[]  = { "_" } ;
static char d20_88field[]  = { "_" } ;
static char d20_89field[]  = { "_" } ;

static char d20_81hfld[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT * 2) + 1] ={""};
static char d20_82hfld[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT * 2) + 1] ={""};

char *d20_8ifields[] = { d20_81field,  d20_82field,  d20_83field,  d20_84field,
			 d20_85field,  d20_86field,  d20_87field,  d20_88field,
			 d20_89field  } ;

char *d20_8hflds[] = { d20_81hfld,  d20_82hfld };

/* conversion flag is set dynamically.	*/

D20_omask d20_8omask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 8)             DIRECTORY SYSTEM",			NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,			NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  7,  8, "Attribute:",			NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9, 10, "Name :",				NULL, 0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  9, 21, d20_81field,                       "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11, 10, "Old Value:",			NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 21, d20_82field,                       "%-45s", D20_ATVAL_FLD_LEN, FALSE, TRUE,
	D20_CGET,  HALBHELL, 12, 21, d20_83field,                       "%-45s", D20_ATVAL_FLD_LEN, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14, 10, "New Value:",			NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 21, d20_84field,                       "%-45s", D20_ATVAL_FLD_LEN, FALSE, TRUE,
	D20_CGET,  HALBHELL, 15, 21, d20_85field,                       "%-45s", D20_ATVAL_FLD_LEN, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, d20_86field,			NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, d20_86field,			NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_87field,			NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTSCD,  0,  0, d20_88field,                       NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_89field,                       NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K  9 ------------------------------ */
/* ************************************************************************* */

static signed32 d20_91field = 1;
static char d20_92field[]  = { "_" } ;
static char d20_93field[]  = { "_" } ;

char *d20_9ifields[] = { (char *)&d20_91field, d20_92field, d20_93field } ;

D20_omask d20_9omask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 9)             DIRECTORY SYSTEM",			NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, d20_actopmsg,                      NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  4,  5, "Operations",			NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,	6, 13, "0 Exit",			NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,	7, 13, "1 Store Schema",		NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,	8, 13, "2 Load Schema",			NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,	9,  6, "(SRT): 3 Display SRT",		NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,      10, 13, "4 Add SRT entry",		NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,      11, 13, "5 Delete SRT entry",		NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,      12, 13, "6 Modify SRT entry",		NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,      13,  6, "(OCT): 7 Display OCT",		NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,      14, 13, "8 Add OCT entry",		NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,      15, 13, "9 Delete OCT entry",		NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,      16, 12, "10 Modify OCT entry",		NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,      17,  6, "(AT): 11 Display AT",		NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,      18, 12, "12 Add AT entry",		NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,      19, 12, "13 Delete AT entry",		NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,      20, 12, "14 Modify AT entry",		NULL, 0, FALSE, FALSE,
	D20_CSAY, NORMAL,      23,  5, "Which operation ?",		NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 23, 23, (char *)&d20_91field,              "%-2d", 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_92field,			NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_93field,                       NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K  10 ----------------------------- */
/* ************************************************************************* */

static char   d20_A1field[3+1];
static char   d20_A2field[25+1];
static char   d20_A3field[D2_SUPF_LEN];
static char   d20_A4field[D2_SUPL_LEN];
static char    d20_A5field[28+1];
static signed32    d20_A6field;
static signed32    d20_A7field      = 1;
static char   d20_A8field[D2_AUXF_LEN];
static char   d20_A9field[D2_AUXL_LEN];
static char   d20_A10field[D2_MANDF_LEN];
static char   d20_A11field[D2_MANDL_LEN];
static char   d20_A12field[D2_OPTF_LEN];
static char   d20_A13field[D2_OPTF_LEN];
static char   d20_A14field[D2_OPTF_LEN];
static char   d20_A15field[D2_OPTF_LEN];
static char   d20_A16field[D2_OPTF_LEN];
static char   d20_A17field[D2_OPTF_LEN];
static char   d20_A18field[D2_OPTL_LEN];
static char   d20_A19field[]   = { "_" } ;
static char   d20_A20field[]   = { "_" } ;
static char   d20_A21field[]   = { "_" } ;
static char   d20_A22field[]   = { "_" } ;
static char   d20_A23field[]   = { "_" } ;
static char   d20_A24field[]   = { "_" } ;

char *d20_Aifields[] = { d20_A1field, d20_A2field, d20_A3field, d20_A4field,
			 d20_A5field, (char *)&d20_A6field,(char *)&d20_A7field,
			 d20_A8field, d20_A9field, d20_A10field, d20_A11field, 
			 d20_A12field, d20_A13field, d20_A14field, d20_A15field,
			 d20_A16field, d20_A17field, d20_A18field, d20_A19field,
			 d20_A20field, d20_A21field, d20_A22field, d20_A23field,
			 } ;

D20_omask d20_Aomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 10)            DIRECTORY SYSTEM",			NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, d20_actopmsg,                      NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  4,  8, "Object Class Acronym:",           NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  4, 37, d20_A1field,             		"%-3s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  5,  8, "Object Class Name:",          	NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  5, 43, d20_A2field,             		"%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  6,  8, "Acronyms Of Super Classes:",      NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  6, 43, d20_A3field,             		"%-31s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 43, d20_A4field,             		"%-7s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8,  8, "Object Identifier:",              NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 43, d20_A5field,             		"%-28s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9,  8, "Object Class Kind:",     		NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 43, (char *) &d20_A6field,             
	"%.0=Abstract  .1=Alias     .2=Auxiliary .3=Structural.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 10,  8, "File No:",             		NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 10, 43, (char *) &d20_A7field,             "%-2d", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11,  8, "Auxiliary Object Classes:",       NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 43, d20_A8field,             		"%-31s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 43, d20_A9field,             		"%-7s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13,  8, "Mandatory Attributes:",           NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 43, d20_A10field,             		"%-31s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 43, d20_A11field,             		"%-7s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15,  8, "Optional Attributes:",            NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 43, d20_A12field,             		"%-31s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 16, 43, d20_A13field,             		"%-31s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 17, 43, d20_A14field,             		"%-31s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 18, 43, d20_A15field,             		"%-31s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 19, 43, d20_A16field,             		"%-31s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 20, 43, d20_A17field,             		"%-31s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 21, 43, d20_A18field,             		"%-7s", 0, FALSE, FALSE,
	D20_FKTX,    FKTSCU,  0,  0, d20_A19field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTSCD,  0,  0, d20_A20field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_A21field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, d20_A22field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, d20_A22field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_A23field,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTF1,   0,  0, d20_A24field,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K  11 ----------------------------- */
/* ************************************************************************* */

static char  d20_B1field[]  = { "---" };
static char  d20_B2field[]  = { "_________________________" };
static char  d20_B3field[]  = { "____________________________" } ;
static signed32   d20_B4field = 1;
static signed32   d20_B5field = 1;
static signed32   d20_B6field = 1;
static signed32   d20_B7field = 0;
static signed32   d20_B8field = 1;
static signed32   d20_B9field = 0;
static signed32   d20_Bafield = 0;
static char  d20_Bbfield[] = { "_" } ;
static char  d20_Bcfield[]  = { "_" } ;
static char  d20_Bdfield[]  = { "_" } ;
static char  d20_Befield[]  = { "_" } ;
static char  d20_Bffield[]  = { "_" } ;
static char  d20_Bgfield[]  = { "_" } ;

char *d20_Bifields[] = { d20_B1field, d20_B2field,
			 d20_B3field, (char *) &d20_B4field,
			 (char *) &d20_B5field, (char *) &d20_B6field,
			 (char *) &d20_B7field, (char *) &d20_B8field,
			 (char *) &d20_B9field, (char *) &d20_Bafield,
			 d20_Bbfield,d20_Bcfield,d20_Bdfield,d20_Befield,
			 d20_Bffield };

D20_omask d20_Bomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 11)            DIRECTORY SYSTEM",                        NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, d20_actopmsg,                      NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  5, 10, "Attribute Abbreviation:",         NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  5, 45, d20_B1field,             	        "%-3s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  6, 10, "Attribute Name:",                 NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  6, 45, d20_B2field,                       "%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  7, 10, "Object Identifier:",              NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 45, d20_B3field,             	        "%-28s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, "Lower Bound:",                    NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 45, (char *) &d20_B4field,             "%-4d", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9, 10, "Upper Bound:",                    NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 45, (char *) &d20_B5field,             "%-4d", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 10, 10, "Number of Recurring Values:",     NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 10, 45, (char *) &d20_B6field,             "%-4d", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11, 10, "Attribute Syntax:",		NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 45, (char *) &d20_B7field,		NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 10, "Access Class:",                   NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 45, (char *) &d20_B8field,
	"%.0=Public    .1=Standard  .2=Sensitive .a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 10, "Index Level:",                    NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 45, (char *) &d20_B9field,             "%-2d", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14, 10, "Phonetical Matching:",            NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 45, (char *) &d20_Bafield,             "%.0=N.1=Y.a", 0, FALSE, FALSE,
	D20_FKTX,    FKTSCU,  0,  0, d20_Bbfield,                       NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTSCD,  0,  0, d20_Bcfield,                       NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_Bdfield,                       NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, d20_Befield,                       NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, d20_Befield,                       NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_Bffield,                       NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTF1,   0,  0, d20_Bgfield,                       NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K  12 ----------------------------- */
/* ************************************************************************* */

static signed32   d20_C1field 		= 1; 
static signed32   d20_C2field    	= 0;
static char  d20_C3field[D2_NAM_LEN];
static char  d20_C4field[3+1];
static char  d20_C5field[]   		= { "_" } ;
static char  d20_C6field[]   		= { "_" } ;
static char  d20_C7field[]   		= { "_" } ;
static char  d20_C8field[]   		= { "_" } ;
static char  d20_C9field[]   		= { "_" } ;
static char  d20_C10field[]   		= { "_" } ;

char *d20_Cifields[] = { (char *)&d20_C1field, (char *)&d20_C2field, 
			 d20_C3field, d20_C4field, d20_C5field, d20_C6field, 
			 d20_C7field, d20_C8field, d20_C9field
		       };

D20_omask d20_Comask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 10)            DIRECTORY SYSTEM",			NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, d20_actopmsg,                      NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  6, 10, "Rule Number:",           	        NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  6, 45, (char *)&d20_C1field,     		"%-2d", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  7, 10, "Superior Rule Number:",          	NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 45, (char *)&d20_C2field,    		"%-2d", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, "Acronyms Of Naming Attrbutes:",   NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 45, d20_C3field,             		"%-11s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9, 10, "Object Class:",                 NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 45, d20_C4field,             		"%-3s", 0, FALSE, FALSE,
	D20_FKTX,    FKTSCU,  0,  0, d20_C5field,                       NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTSCD,  0,  0, d20_C6field,                       NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_C7field,                       NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, d20_C8field,                       NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, d20_C8field,                       NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_C9field,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTF1,   0,  0, d20_C10field,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ****************************************************************** */
/* ------------------------------  M A S K  13 ---------------------- */
/* ****************************************************************** */

static signed32   d20_D1field = 1 ;
static char  d20_D2field[] = { "_" } ;
static char  d20_D3field[] = { "_" } ;

D20_omask d20_Domask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 13)            DIRECTORY SYSTEM",                        NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, d20_actopmsg,                      NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, "OPERATIONS",			NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, "0 Exit",                          NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9, 10, "1 Create Shadows and Shadowing Job", NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 10, 10, "2 Create Shadowing Job",          NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11, 10, "3 Remove Shadows and Shadowing Job", NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 10, "4 Remove Shadowing Job",          NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 10, "5 Update Shadowing Job",          NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14, 10, "6 Display Shadowing Jobs",        NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15, 10, "7 Display Error Message File",    NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 16, 10, "8 Remove Update Error",           NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 23,  5, "Which operation ?",               NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 23, 23, (char *)&d20_D1field,              "%-1d", 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_D2field,                       NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_D3field,                       NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ****************************************************************** */
/* ------------------------------  M A S K  14.1 -------------------- */
/* ****************************************************************** */

static signed32   d20_E11field ;
static char  d20_E12field[] = "_" ;
static char  d20_E13field[] = "_" ;
static char  d20_E20field[] = "_" ;
static char  d20_E19field[] = "_" ;

static char  d20_E14field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  ={ "" };
static char  d20_E15field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  ={ "" };

static signed32   d20_E16field = OBJECT;

static char  d20_E17field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  ={ "" };
static char  d20_E18field[(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) + 1]  ={ "" };

char *d20_E11format;
char *d20_E12format;

char d20_m_empty[1] = "";

D20_omask d20_E1omask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 14)            DIRECTORY SYSTEM",                        NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, d20_actopmsg,                      NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  4,  5, "Object Name:",                    NULL, 0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  4, 30, d20_E14field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, TRUE,
	D20_CSAY,  HALBHELL,  5, 30, d20_E15field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  7,  5, "Object interpretation:",          NULL, 0, FALSE, FALSE,
	D20_CSAY, HALBHELL,   7, 30, (char *) &d20_E16field,
				     "%.0=SINGLE OBJECT              .\
					1=OBJECT AND ITS SUBORDINATES.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9,  5, "Target DSA:",                     NULL, 0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  9, 30, d20_E17field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, TRUE,
	D20_CSAY,  HALBHELL, 10, 30, d20_E18field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 12,  5, "Job:",                            NULL, 0, FALSE, FALSE,
	D20_CGET,   HALBHELL,12, 30, (char *) &d20_E11field,            NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_E12field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTMENU, 0,  0, d20_E19field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTCR,   0,  0, d20_E19field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_E13field,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, d20_E20field,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ****************************************************************** */
/* ------------------------------  M A S K  14.2 -------------------- */
/* ****************************************************************** */

static signed32   d20_E21field = UPD_HIGH;
static char  d20_E22field[] = "_" ;
static char  d20_E23field[] = "_" ;

D20_omask d20_E2omask[] = {
	D20_NOERASE | D20_CSAY,
		     NORMAL, 14,  5, "Update Frequency:",               NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 30, (char *) &d20_E21field,
				     "%.0=HIGH .\
					1=MEDIUM .\
					2=LOW .a", 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_E22field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTMENU, 0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTCR,   0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_E23field,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ****************************************************************** */
/* ------------------------------  M A S K  14a --------------------- */
/* ****************************************************************** */

static signed32   d20_Ea1field;
static char  d20_Ea2field[] = "_" ;
static char  d20_Ea3field[] = "_" ;
static char  d20_Ea4field[] = "_" ;
static char  d20_Ea5field[] = "_" ;


D20_omask d20_Eaomask[] = {
	D20_NOERASE | D20_CSAY,    
		     NORMAL, 16, 5, d20_m_empty,    			"%-58s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 16, 5, "Update Times:",            	NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 16, 30, (char *) &d20_Ea1field,            NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 17, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 18, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 19, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 20, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_Ea4field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTMENU, 0,  0, d20_Ea3field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTCR,   0,  0, d20_Ea3field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_Ea5field,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, d20_Ea2field,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ****************************************************************** */
/* ------------------------------  M A S K  14b --------------------- */
/* ****************************************************************** */

static signed32   d20_Eb1field = ZERO_HOUR;
static char  d20_Eb2field[] = "_" ;
static char  d20_Eb4field[] = "_" ;
static char  d20_Eb5field[] = "_" ;
static char  d20_Eb6field[] = "_" ;

D20_omask d20_Ebomask[] = {
	D20_NOERASE | D20_CSAY,    
		     NORMAL, 17, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 17, 8, "Hours      :",            		NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 17, 30, (char *) &d20_Eb1field,    	NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 18, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 19, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 20, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_Eb4field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTMENU, 0,  0, d20_Eb2field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTCR,   0,  0, d20_Eb2field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_Eb5field,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, d20_Eb6field,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ****************************************************************** */
/* ------------------------------  M A S K  14c --------------------- */
/* ****************************************************************** */

static signed32   d20_Ec1field = SUNDAY;
static signed32   d20_Ec2field = ZERO_HOUR;
static char  d20_Ec3field[] = "_" ;
static char  d20_Ec4field[] = "_" ;
static char  d20_Ec5field[] = "_" ;
static char  d20_Ec6field[] = "_" ;

D20_omask d20_Ecomask[] = {
	D20_NOERASE | D20_CSAY,    
		     NORMAL, 17, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 18, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 17, 8, "Day of Week:",            		NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 17, 30, (char *) &d20_Ec1field,    	NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 18, 8, "Hours      :",            		NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 18, 30, (char *) &d20_Ec2field,    	NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 19, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 20, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_Ec4field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTMENU, 0,  0, d20_Ec3field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTCR,   0,  0, d20_Ec3field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_Ec5field,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, d20_Ec6field,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ****************************************************************** */
/* ------------------------------  M A S K  14d --------------------- */
/* ****************************************************************** */

static signed32   d20_Ed1field = SUNDAY;
static signed32   d20_Ed2field = ZERO_HOUR;
static signed32   d20_Ed3field = SUNDAY;
static signed32   d20_Ed4field = ZERO_HOUR;
static char  d20_Ed5field[] = "_" ;
static char  d20_Ed6field[] = "_" ;
static char  d20_Ed7field[] = "_" ;
static char  d20_Ed8field[] = "_" ;

D20_omask d20_Edomask[] = {
	D20_NOERASE | D20_CSAY,    
		     NORMAL, 17, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 18, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 19, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 20, 8, d20_m_empty,    			"%-55s", 0, FALSE, FALSE,
	D20_CSAY,   NORMAL, 17, 8, "Day of Week:",            		NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 17, 30, (char *) &d20_Ed1field,    	NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 18, 8, "Hours      :",            		NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 18, 30, (char *) &d20_Ed2field,    	NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 19, 8, "Day of Week:",            		NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 19, 30, (char *) &d20_Ed3field,    	NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 20, 8, "Hours      :",            		NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 20, 30, (char *) &d20_Ed4field,    	NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_Ed5field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTMENU, 0,  0, d20_Ed8field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTCR,   0,  0, d20_Ed8field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, d20_Ed6field,                      NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, d20_Ed7field,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

char *d20_F_mints;
char *d20_F_hrs;
char *d20_F_days;
char *d20_F_job_state;
char *d20_F_act_mod;
char *d20_F_notact_mod;


/* ****************************************************************** */
/* ------------------------------  M A S K  14.3 -------------------- */
/* ****************************************************************** */

static signed32   d20_E31field = 6;
static char  d20_E32field[15] = { "______________" } ;
static char  d20_E33field[6] = { "_____" } ;
static char  d20_E34field[D20_ATVAL_FLD_LEN + 1]  = { "_____________________________________________" } ;
static signed32   d20_E35field = FALSE;

D20_omask d20_E3omask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 14)            DIRECTORY SYSTEM",                        NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, d20_actopmsg,                      NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  6,  5, "Object Name:",                    NULL, 0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  6, 30, d20_E14field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, TRUE,
	D20_CSAY,  HALBHELL,  7, 30, d20_E15field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  8,  5, "Target DSA:",                     NULL, 0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  8, 30, d20_E17field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, TRUE,
	D20_CSAY,  HALBHELL,  9, 30, d20_E18field,                      "%-45s", D20_ATVAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 10,  5, "Operation:",                      NULL, 0, FALSE, FALSE,
	D20_CSAY,   HALBHELL,10, 30, (char *) &d20_E31field,
	"%.6=ADD       .7=REMOVE    .8=MODIFY    .9=MODIFY RDN.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11,  5, "Error Time:",                     NULL, 0, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 11, 30, d20_E32field,                      "%-14s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12,  5, "Error Count:",                    NULL, 0, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 12, 30, d20_E33field,                      "%-5s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13,  5, "Error Code:",                     NULL, 0, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 13, 30, d20_E34field,                      "%-14s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14,  5, "Remove Update Error:",            NULL, 0, FALSE, FALSE,
	D20_CGET,   HALBHELL,14, 30, (char *) &d20_E35field,     "%.0=N.1=Y.a", 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTMENU, 0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTCR,   0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTSCD,  0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ****************************************************************** */
/* ------------------------------  M A S K  16 ---------------------- */
/* ****************************************************************** */

static signed32   d20_101field = 1 ;
static char  d20_102field[] = { "_" } ;
static char  d20_103field[] = { "_" } ;

D20_omask d20_10omask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 16)            DIRECTORY SYSTEM",                        NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, d20_actopmsg,                      NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, "OPERATIONS",			NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, "0 Exit",                          NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9, 10, "1 Save Subtree",                  NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 10, 10, "2 Append Subtree",                NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11, 10, "3 Copy Subtree",                  NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 10, "4 Change Name / Move Subtree",    NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 10, "5 Delete Subtree",                NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14, 10, "6 Change Master",                 NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15, 10, "7 Modify All Attributes",         NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 23,  5, "Which operation ?",               NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 23, 23, (char *)&d20_101field,             "%-1d", 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_102field,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTMENU, 0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTCR,   0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTHELP, 0,  0, d20_103field,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ****************************************************************** */
/* ------------------------------  M A S K  17a --------------------- */
/* ****************************************************************** */

static signed32   d20_11a1field = 0 ;

static char  d20_11a2field[D20_FN_FLD_LEN + 1] ={ "" };
static char  d20_11a3field[D20_FN_FLD_LEN + 1] ={ "" };

static char  d20_11a4field[] = { "_" } ;
static char  d20_11a5field[] = { "_" } ;

D20_omask d20_11aomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 17a)           DIRECTORY SYSTEM",                        NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, d20_actopmsg,                      NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  6, 10, "Source:",                         NULL, 0, FALSE, FALSE,
	D20_CGET,    NORMAL,  6, 22, (char *) &d20_11a1field,
	"%.0=MASTER DSA(s).1=BIND DSA     .2=SPECIFIC DSA .3=ANY DSA(s)   .a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, "File Name:",                      NULL, 0, FALSE, FALSE,
	D20_CGET,    NORMAL,  8, 22, d20_11a2field,                     "%-37s", 0, FALSE, FALSE,
	D20_CGET,    NORMAL,  9, 22, d20_11a3field,                     "%-37s", 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_11a4field,                     NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTMENU, 0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTCR,   0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTHELP, 0,  0, d20_11a5field,                     NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ****************************************************************** */
/* ------------------------------  M A S K  17b --------------------- */
/* ****************************************************************** */

static char  d20_11b1field[D20_FN_FLD_LEN + 1] ={ "" };
static char  d20_11b2field[D20_FN_FLD_LEN + 1] ={ "" };

static signed32   d20_11b3field = 1;
static signed32   d20_11b4field = 0;
static signed32   d20_11b5field = 1;
static char  d20_11b6field[] = { "_" } ;
static char  d20_11b7field[] = { "_" } ;

D20_omask d20_11bomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 17b)           DIRECTORY SYSTEM",                        NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, d20_actopmsg,                      NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  6, 10, "File Name:",                      NULL, 0, FALSE, FALSE,
	D20_CGET,    NORMAL,  6, 22, d20_11b1field,                     "%-37s", 0, FALSE, FALSE,
	D20_CGET,    NORMAL,  7, 22, d20_11b2field,                     "%-37s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9, 10, "Overwrite Existing Entries:",     NULL, 0, FALSE, FALSE,
	D20_CGET,    NORMAL,  9, 38, (char *) &d20_11b3field,           "%.0=No .1=Yes.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11, 10, "New Entries Protected by:",       NULL, 0, FALSE, FALSE,
	D20_CGET,    NORMAL, 11, 38, (char *) &d20_11b4field,
		     "%.1=ACL of the new parent.0=Original ACL         .a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 10, "Sink DSA:",                       NULL, 0, FALSE, FALSE,
	D20_CGET,    NORMAL, 13, 38, (char *) &d20_11b5field,
		     "%.1=BIND DSA    .2=SPECIFIC DSA.a", 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_11b6field,                     NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTMENU, 0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTCR,   0,  0, NULL,                           NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTHELP, 0,  0, d20_11b7field,                     NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K  18 --(English)------------------ */
/* ************************************************************************* */
static signed32  d20_121field = D20_12_OBJ_LIST;
static char d20_12afield[] = { "_" };
static char d20_12bfield[] = { "_" };
static char d20_12cfield[] = { "_" };
static char d20_12dfield[] = { "_" };
static char d20_12efield[] = { "_" };
static char d20_12ffield[] = { "_" };
static char d20_12gfield[] = { "_" };
static char d20_12hfield[] = { "_" };
static char d20_12f8field[] = { "_" };

char *d20_12ifields[] = { d20_12afield, d20_12bfield, d20_12cfield,
                          d20_12dfield, d20_12efield, d20_12ffield,
			  d20_12gfield, d20_12hfield, d20_12f8field };
static char d20_12bar[(D20_MAX_DISP_STR_LEN * D20_T61_CONV_FACT) + 1];
char *d20_12barmenu[] = { d20_12bar };
D20_omask d20_12omask[] = {
	D20_CSAY,    NORMAL,  2,  5, (char *)&d20_121field,		NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 46, d20_actopmsg,			NULL, 0, FALSE, FALSE,
	D20_CGET,    WAHLERG | DUNKEL, 2, 65, d20_12bar,                "%-75s", D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  5,  4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  6,  4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  7,  4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  8,  4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  9,  4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  10, 4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  11, 4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  12, 4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  13, 4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  14, 4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  15, 4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  16, 4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  17, 4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  18, 4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  19, 4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    WAHL,  20, 4, NULL,                             NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_FKTX,    FKTCR,  0, 0, d20_12afield,			NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL, 0, 0, d20_12bfield,			NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTF1,  0, 0, d20_12cfield,			NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTMENU, 0,0, d20_12dfield,			NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTPRINT,0,0, d20_12efield,			NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTSCD, 0, 0, d20_12ffield,			NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTSCU, 0, 0, d20_12gfield,			NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTHELP, 0, 0,d20_12hfield,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTF8, 0, 0,  d20_12f8field,                       NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K  18b --(English)------------------ */
/* ************************************************************************* */

static char d20_12b1field[D20_ONM_NM_FLD_LEN + 1];
static char d20_12b2field[D20_ONM_NM_FLD_LEN + 1];
static char d20_12b3field[D20_ONM_NM_FLD_LEN + 1];
static char d20_12b4field[D20_ONM_NM_FLD_LEN + 1];
static char d20_12b5field[D20_ONM_NM_FLD_LEN + 1];
static char d20_12b6field[] = { "_" };

char *d20_12bifields[] = { d20_12b1field, d20_12b2field, d20_12b3field,
                          d20_12b4field, d20_12b5field, d20_12b6field
			};

D20_omask d20_12bomask[] = {
	D20_CSAY,    NORMAL,  2,  5, NULL,				NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 46, d20_actopmsg,			NULL, 0, FALSE, FALSE,
	D20_CGET,    NORMAL,  5,  4, d20_12b1field,  			"%-25s", 0, FALSE, FALSE,
	D20_CGET,    NORMAL,  6,  4, d20_12b2field,  			"%-25s", 0, FALSE, FALSE,
	D20_CGET,    NORMAL,  7,  4, d20_12b3field,  			"%-25s", 0, FALSE, FALSE,
	D20_CGET,    NORMAL,  8,  4, d20_12b4field,  			"%-25s", 0, FALSE, FALSE,
	D20_CGET,    NORMAL,  9,  4, d20_12b5field,  			"%-25s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  20,60, "More:",                           NULL, 0, FALSE, FALSE,
	D20_CGET,    NORMAL,  20,65, d20_12b6field,  			"%-1s", 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ************************************************************************* */
/* ------------------------------  M A S K  19 ----------------------------- */
/* ************************************************************************* */

		    /* format strings for RDNs that will dynamically be */
		    /* changed                                          */
static char d20_13oform[12][6] = { "%-25s",
				   "%-25s",
				   "%-25s",
				   "%-25s",
				   "%-25s",
				   "%-25s",
				   "%-25s",
				   "%-25s",
				   "%-25s",
				   "%-25s",
				   "%-25s",
				   "%-25s" };

static char d20_131field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] ={ "" };
static char d20_132field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] ={ "" };
static char d20_133field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] ={ "" };
static char d20_134field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] ={ "" };
static char d20_135field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] ={ "" };
static char d20_136field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] ={ "" };
static char d20_137field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] ={ "" };
static char d20_138field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] ={ "" };
static char d20_139field[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] ={ "" };
static char d20_13afield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] ={ "" };
static char d20_13bfield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] ={ "" };
static char d20_13cfield[(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT) + 1] ={ "" };

static char d20_13dfield[]  = { "_" } ;
static char d20_13efield[]  = { "_" } ;
static char d20_13ffield[]  = { "_" } ;
static char d20_13gfield[]  = { "_" } ;
static char d20_13hfield[]  = { "_" } ;
static char d20_13jfield[]  = { "_" } ;

char *d20_13ofields[] = { d20_131field,  d20_132field,  d20_133field,
			  d20_134field,  d20_135field,  d20_136field,
			  d20_137field,  d20_138field,  d20_139field,
			  d20_13afield,  d20_13bfield,  d20_13cfield };

char *d20_13ifields[] = { d20_13dfield,  d20_13efield,  d20_13ffield,
			  d20_13gfield, d20_13hfield, d20_13jfield} ;

D20_omask d20_13omask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 6)             DIRECTORY SYSTEM",                        NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 48, d20_actopmsg,                      NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, "Object Name",                     NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 36, d20_131field,                      d20_13oform[0], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  9, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9, 36, d20_132field,                      d20_13oform[1], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 10, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 10, 36, d20_133field,                      d20_13oform[2], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 11, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11, 36, d20_134field,                      d20_13oform[3], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 12, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 36, d20_135field,                      d20_13oform[4], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 13, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 36, d20_136field,                      d20_13oform[5], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 14, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14, 36, d20_137field,                      d20_13oform[6], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 15, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15, 36, d20_138field,                      d20_13oform[7], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 16, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 16, 36, d20_139field,                      d20_13oform[8], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 17, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 17, 36, d20_13afield,                      d20_13oform[9], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 18, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 18, 36, d20_13bfield,                      d20_13oform[10], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 19, 10, NULL,				NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 19, 36, d20_13cfield,                      d20_13oform[11], D20_ONM_VAL_FLD_LEN, TRUE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, d20_13dfield,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTF1,   0,  0, d20_13efield,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTMENU, 0,  0, d20_13ffield,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTSCD,  0,  0, d20_13gfield,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTSCU,  0,  0, d20_13hfield,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTHELP, 0,  0, d20_13jfield,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTCR,   0,  0, d20_13ffield,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K  20 --(English)------------------ */
/* ************************************************************************* */

static char d20_14afield[]  = { "_" } ;
static char d20_14bfield[]  = { "_" } ;
static char d20_14cfield[]  = { "_" } ;
static char d20_14dfield[]  = { "_" } ;
static char d20_14efield[]  = { "_" } ;

D20_omask d20_14omask[] = {
	D20_CSAY,    NORMAL,  2,  5, NULL,                           NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 52, NULL,                           NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  5,  5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  6,  5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  7,  5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  8,  5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  9,  5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  10, 5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  11, 5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  12, 5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  13, 5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  14, 5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  15, 5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  16, 5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  17, 5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  18, 5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  19, 5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL,  20, 5, NULL,                           NULL, D20_MAX_DISP_STR_LEN, TRUE, FALSE,
	D20_FKTX,    FKTDEL,   0, 0, d20_14afield,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTPRINT, 0, 0, d20_14bfield,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTSCD,   0, 0, d20_14cfield,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTSCU,   0, 0, d20_14dfield,                      NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTHELP,  0, 0, d20_14efield,                      NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

char *d20_14oheader[D20_NERR_HEADER];
char *d20_bool[D20_NBOOL];


/* ************************************************************************* */
/* ------------------- ERROR MESSAGE TABLE --------------------------------- */
/* ************************************************************************* */

char *d20_errtab[D20_MAX_ERR_TAB];

char *d20_err_table[D20_MAX_ERR_TABLE];

char *d20_errmess[D20_MAX_ERR_MESS];

/* ************************************************************************* */
/* ------------------- ARRAY FOR ALL HELP-MASKS ---------------------------- */
/* ************************************************************************* */

D20_help_array  help_masks1[] =
		{ { d20_1omask, 1, 3, d20_1omask, (D20_omask *) 0 },
		  { d20_2omask, 4, 2, d20_2omask, (D20_omask *) 0 },
		  { d20_3omask, 6, 1, d20_3omask, (D20_omask *) 0 },
		  { d20_4omask, 7, 1, d20_4omask, (D20_omask *) 0 },
		  { d20_4aomask, 8, 1, d20_4aomask, (D20_omask *) 0 },
		  { d20_5omask, 9, 2, d20_5omask, (D20_omask *) 0 },
		  { d20_6omask, 11, 3, d20_6omask, (D20_omask *) 0 },
		  { d20_6aomask, 14, 1, d20_6aomask, (D20_omask *) 0 },
		  { d20_6bomask, 15, 2, d20_6bomask, (D20_omask *) 0 },
		  { d20_7omask, 17, 4, d20_7omask, (D20_omask *) 0 },
		  { d20_7aomask, 21, 1, d20_7aomask, (D20_omask *) 0 },
		  { d20_8omask, 22, 1, d20_8omask, (D20_omask *) 0 },
		  { d20_9omask, 23, 1, d20_9omask, (D20_omask *) 0 },
		  { d20_Aomask, 24, 6, d20_Aomask, (D20_omask *) 0 },
		  { d20_Bomask, 30, 7, d20_Bomask, (D20_omask *) 0 },
		  { d20_Comask, 37, 5, d20_Comask, (D20_omask *) 0 },
		  { d20_Domask, 42, 1, d20_Domask, (D20_omask *) 0 },
		  { d20_E1omask, 43, 5, d20_E1omask, (D20_omask *) 0 },
		  { d20_E2omask, 48, 1, d20_E1omask, d20_E2omask, (D20_omask *) 0 },
		  { d20_Eaomask, 49, 2, d20_E1omask, d20_E2omask, d20_Eaomask, (D20_omask *) 0 },
		  { d20_Ebomask, 51, 1, d20_E1omask, d20_E2omask, d20_Eaomask, d20_Ebomask, (D20_omask *) 0 },
		  { d20_Ecomask, 52, 2, d20_E1omask, d20_E2omask, d20_Eaomask, d20_Ecomask, (D20_omask *) 0 },
		  { d20_Edomask, 54, 2, d20_E1omask, d20_E2omask, d20_Eaomask, d20_Edomask, (D20_omask *) 0 },
		  { d20_E3omask, 56, 2, d20_E3omask, (D20_omask *) 0 },
		  { d20_10omask, 58, 4, d20_10omask, (D20_omask *) 0 },
		  { d20_11aomask, 62, 2, d20_11aomask, (D20_omask *) 0 },
		  { d20_11bomask, 64, 3, d20_11bomask, (D20_omask *) 0 },
		  { d20_12omask, 67, 1, d20_12omask, (D20_omask *) 0 },
		  { d20_13omask, 68, 1, d20_13omask, (D20_omask *) 0 },
		  { d20_14omask, 69, 1, d20_14omask, (D20_omask *) 0 },
		  { d20_dnscellomask, 70, 3, d20_dnscellomask, (D20_omask *) 0},
		  { d20_dnsrepomask, 73, 4, d20_dnsrepomask, (D20_omask *) 0 },
		  { d20_ttxidomask, 77, 3, d20_ttxidomask, (D20_omask *) 0 },
		  { d20_telexomask, 80, 2, d20_telexomask, (D20_omask *) 0 },
		  { d20_t61omask, 82, 2, d20_t61omask, (D20_omask *) 0 },
		  { d20_faxomask, 84, 2, d20_faxomask, (D20_omask *) 0 },
		  { d20_oraddomask, 86, 2, d20_oraddomask, (D20_omask *) 0 },
		  { d20_moraddomask, 88, 6, d20_moraddomask, (D20_omask *) 0 },
		  { d20_noraddomask, 94, 12, d20_noraddomask, (D20_omask *) 0 },
		  { d20_soraddomask, 106, 4, d20_soraddomask, (D20_omask *) 0 },
		  { d20_uoraddomask, 110, 6, d20_uoraddomask, (D20_omask *) 0 },
		  { d20_toraddomask, 116, 2, d20_toraddomask, (D20_omask *) 0 },
		  { d20_dlpermomask, 118, 1, d20_dlpermomask, (D20_omask *) 0 },
		  { d20_ornameomask, 119, 3, d20_ornameomask, (D20_omask *) 0 },
		  { d20_mogomask, 122, 3, d20_mogomask, (D20_omask *) 0 },
		  { d20_dmeomask, 125, 1, d20_dmeomask, (D20_omask *) 0 },
		  { d20_secomask, 131, 1, d20_secomask, (D20_omask *) 0 },
		  { d20_certomask, 126, 2, d20_certomask, (D20_omask *) 0 },
		  { d20_certpomask, 128, 1, d20_certpomask, (D20_omask *) 0 },
		  { d20_certlomask,  129, 1, d20_certlomask, (D20_omask *) 0 },
		  { d20_rcertomask, 130, 1, d20_rcertomask, (D20_omask *) 0 },
		  { (D20_omask *) 0 }
		};
