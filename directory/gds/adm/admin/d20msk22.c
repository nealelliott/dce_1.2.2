/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20msk22.c,v $
 * Revision 1.1.727.2  1996/02/18  19:41:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:00  marty]
 *
 * Revision 1.1.727.1  1995/12/08  15:17:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:07  root]
 * 
 * Revision 1.1.725.3  1994/05/10  15:49:45  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:54:10  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:12:41  marrek]
 * 
 * Revision 1.1.725.2  1994/03/23  15:05:33  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:10:39  keutel]
 * 
 * Revision 1.1.725.1  1994/02/22  15:58:49  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:31:30  marrek]
 * 
 * Revision 1.1.723.2  1993/10/14  16:46:34  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  16:44:52  keutel]
 * 
 * Revision 1.1.723.1  1993/10/13  17:29:46  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  07:35:54  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  10:19:09  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:32:37  marrek]
 * 
 * Revision 1.1.4.4  1993/02/04  08:39:55  marrek
 * 	help_mask2 removed.
 * 	[1993/02/03  08:39:56  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:09:11  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:04:58  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:17:37  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:12:53  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:58:38  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:16:54  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20msk22.c,v $ $Revision: 1.1.727.2 $ $Date: 1996/02/18 19:41:16 $";
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
/* NAME         : d20msk22.c                                          */
/*								      */
/* AUTHOR       : Mueller V., SNI AP 11                               */
/* DATE         : 21.03.91                                            */
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
/*  0.1     |21.03.91 |  original                      |    |         */
/*datoff **************************************************************/

/*****************************************************************************/
/*                                                                           */
/*                 M A S K   D E F I N I T I O N   M O D U L E               */
/*                                                                           */
/*           (used by the administration of the directory system V2)         */
/*                                                                           */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  Author: V. Mueller                                    Date: 21.3.91      */
/*****************************************************************************/


#include <gds.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <cget.h>
#include <d2mhs_msk.h>
#include <d2spec_msk.h>

extern char d20_actopmsg[];
 char d20_attr_name[] = {  "                                        " } ;


/* ************************************************************************* */
/* ------------------------ WORKING M A S K -------------------------------- */
/* ************************************************************************* */

/* This mask doesn't use any nls file. What you write here is what will get! */

D20_omask d20_working_mask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"                      DIRECTORY SYSTEM",                    NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  12, 25, "Loading Message files...",    NULL, 0, FALSE, FALSE,
	/* This line is to put the cursor at the bottom of the screen:	*/
	D20_CSAY,    NORMAL,  23,  1, " ",                           NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K   DNSCELL ----------------------- */
/* ************************************************************************* */

static char d20_dnscell1field[D20_DC_FLD_LEN + 1];
static char d20_dnscell2field[D20_DC_FLD_LEN + 1];
static char d20_dnscell3field[D20_DC_FLD_LEN + 1];

static signed32  d20_dnscell4field = D2_REP_AV;

char *d20_dnscellifields[] = { d20_dnscell1field,  d20_dnscell2field,  d20_dnscell3field,
			       (char *) &d20_dnscell4field };

D20_omask d20_dnscellomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 21)             DIRECTORY SYSTEM",                    NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, d20_attr_name,                  NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, "namespace_UUID :",             NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 26, d20_dnscell1field,              "%-36s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11, 10, "root_dir_UUID :",               NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 26, d20_dnscell2field,              "%-36s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14, 10, "root_dir_name :",              NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 26, d20_dnscell3field,              "%-36s", 0, FALSE, FALSE,
	D20_NO_OP,   NORMAL, 20, 10, "Modification:",                NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 20, 24, (char *) &d20_dnscell4field,
	"%.4=Modify Value.3=Delete Value.2=Add Value   .a", 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K   DNSREP  ----------------------- */
/* ************************************************************************* */

static char d20_dnsrep2field[D20_DR_FLD_LEN + 1];
static char d20_dnsrep3afield[D20_DR_FLD_LEN + 1];
static char d20_dnsrep3bfield[D20_DR_FLD_LEN + 1];
static char d20_dnsrep4field[D20_DR_FLD_LEN + 1];
static char d20_dnsrep5field[D20_DR_FLD_LEN + 1];
static char d20_dnsrep6field[D20_DR_FLD_LEN + 1];
static char d20_dnsrep7field[D20_DR_FLD_LEN + 1];
static char d20_dnsrep8field[D20_DR_FLD_LEN + 1];

static signed32  d20_dnsrep1field = 1;
static signed32  d20_dnsrep9field = D2_REP_AV;

char *d20_dnsrepifields[] = { (char *) &d20_dnsrep1field, d20_dnsrep2field, d20_dnsrep3afield, d20_dnsrep3bfield,
			      d20_dnsrep4field, d20_dnsrep5field, d20_dnsrep6field,
			      d20_dnsrep7field, d20_dnsrep8field,
			      (char *) &d20_dnsrep9field };

D20_omask d20_dnsrepomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 22)             DIRECTORY SYSTEM",                    NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, d20_attr_name,                  NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, "replica_type :",               NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 25, (char *) &d20_dnsrep1field,
	"%.1=MASTER   .3=READ ONLY.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 10, 10, "chouse_cts :",                 NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 10, 25, d20_dnsrep2field,               "%-45s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 10, "chous_name :",                 NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 25, d20_dnsrep3afield,               "%-45s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 25, d20_dnsrep3bfield,               "%-45s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14, 13, "tower1:",                      NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 21, d20_dnsrep4field,               "%-45s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15, 13, "tower2:",                      NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 21, d20_dnsrep5field,               "%-45s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 16, 13, "tower3:",                      NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 16, 21, d20_dnsrep6field,               "%-45s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 17, 13, "tower4:",                      NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 17, 21, d20_dnsrep7field,               "%-45s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 18, 13, "tower5:",                      NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 18, 21, d20_dnsrep8field,               "%-45s", 0, FALSE, FALSE,
	D20_NO_OP,   NORMAL, 20, 10, "Modification:",                NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 20, 24, (char *) &d20_dnsrep9field, 
	"%.4=Modify Value.3=Delete Value.2=Add Value   .a", 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K   TTXID  ------------------------ */
/* ************************************************************************* */

static char d20_ttxid1field[D20_TT_TERM_FLD_LEN + 1];
static char d20_ttxid2field[(D20_TT_REST_FLD_LEN * D20_T61_CONV_FACT) + 1];
static char d20_ttxid3field[(D20_TT_REST_FLD_LEN * D20_T61_CONV_FACT) + 1];
static char d20_ttxid4field[(D20_TT_REST_FLD_LEN * D20_T61_CONV_FACT) + 1];
static char d20_ttxid5field[D20_TT_REST_FLD_LEN + 1];
static char d20_ttxid6field[D20_TT_REST_FLD_LEN + 1];

static signed32  d20_ttxid7field = D2_REP_AV;

char *d20_ttxidifields[] = { d20_ttxid1field, d20_ttxid2field, d20_ttxid3field,
			     d20_ttxid4field, d20_ttxid5field, d20_ttxid6field,
			     (char *) &d20_ttxid7field };

D20_omask d20_ttxidomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 23)             DIRECTORY SYSTEM",                    NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, d20_attr_name,                  NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, "Terminal :",                   NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 21, d20_ttxid1field,                "%-24s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 10, 10, "Controls :",                   NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 10, 21, d20_ttxid2field,                "%-45s", D20_TT_REST_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 12, 10, "Graphics :",                   NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 21, d20_ttxid3field,                "%-45s", D20_TT_REST_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 14, 10, "Miscel:",                      NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 21, d20_ttxid4field,                "%-45s", D20_TT_REST_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 16, 10, "Pages :",                      NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 16, 21, d20_ttxid5field,                "%-45s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 18, 10, "Privates :",                   NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 18, 21, d20_ttxid6field,                "%-45s", 0, FALSE, FALSE,
	D20_NO_OP,   NORMAL, 20, 10, "Modification:",                NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 20, 24, (char *) &d20_ttxid7field,
	"%.4=Modify Value.3=Delete Value.2=Add Value   .a", 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* ------------------------------  M A S K   TELEX ------------------------- */
/* ************************************************************************* */

static char d20_telex1field[D20_TL_NO_FLD_LEN + 1];
static char d20_telex2field[D20_TL_CC_FLD_LEN + 1];
static char d20_telex3field[D20_TL_ANS_FLD_LEN + 1];

static signed32  d20_telex4field = D2_REP_AV;

char *d20_telexifields[] = { d20_telex1field,  d20_telex2field,  d20_telex3field,
			     (char *) &d20_telex4field };

D20_omask d20_telexomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 24)             DIRECTORY SYSTEM",                    NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, d20_attr_name,                  NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, "Telex-Number :",               NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 25, d20_telex1field,                "%-14s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11, 10, "Country-Code :",               NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 25, d20_telex2field,                "%-4s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14, 10, "Answer :",                     NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 25, d20_telex3field,                "%-8s", 0, FALSE, FALSE,
	D20_NO_OP,   NORMAL, 20, 10, "Modification:",                NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 20, 24, (char *) &d20_telex4field,
	"%.4=Modify Value.3=Delete Value.2=Add Value   .a", 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ************************************************************************* */
/* ------------------------------  M A S K   T61  -------------------------- */
/* ************************************************************************* */

static char d20_t611field[(D20_POST_ADD_FLD_LEN * D20_T61_CONV_FACT) + 1];
static char d20_t612field[(D20_POST_ADD_FLD_LEN * D20_T61_CONV_FACT) + 1];
static char d20_t613field[(D20_POST_ADD_FLD_LEN * D20_T61_CONV_FACT) + 1];
static char d20_t614field[(D20_POST_ADD_FLD_LEN * D20_T61_CONV_FACT) + 1];
static char d20_t615field[(D20_POST_ADD_FLD_LEN * D20_T61_CONV_FACT) + 1];
static char d20_t616field[(D20_POST_ADD_FLD_LEN * D20_T61_CONV_FACT) + 1];

static signed32  d20_t617field = D2_REP_AV;

char *d20_t61ifields[] = { d20_t611field, d20_t612field, d20_t613field,
			   d20_t614field, d20_t615field, d20_t616field,
			   (char *) &d20_t617field };

D20_omask d20_t61omask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 25)            DIRECTORY SYSTEM",                     NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  5, 10, d20_attr_name,                  NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 13, "Part1:",                       NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 20, d20_t611field,                  "%-30s", D20_POST_ADD_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 10, 13, "Part2:",                       NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 10, 20, d20_t612field,                  "%-30s", D20_POST_ADD_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 12, 13, "Part3:",                       NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 20, d20_t613field,                  "%-30s", D20_POST_ADD_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 14, 13, "Part4:",                       NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 20, d20_t614field,                  "%-30s", D20_POST_ADD_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 16, 13, "Part5:",                       NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 16, 20, d20_t615field,                  "%-30s", D20_POST_ADD_FLD_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 18, 13, "Part6:",                       NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 18, 20, d20_t616field,                  "%-30s", D20_POST_ADD_FLD_LEN, TRUE, FALSE,
	D20_NO_OP,   NORMAL, 20, 10, "Modification:",                NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 20, 24, (char *) &d20_t617field,
	"%.4=Modify Value.3=Delete Value.2=Add Value   .a", 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ************************************************************************* */
/* ------------------------------  M A S K   FAX  -------------------------- */
/* ************************************************************************* */

static char d20_fax1field[D20_FX_NO_FLD_LEN + 1];

static signed32  d20_fax2field = FALSE;
static signed32  d20_fax3field = FALSE;
static signed32  d20_fax4field = FALSE;
static signed32  d20_fax5field = FALSE;
static signed32  d20_fax6field = FALSE;
static signed32  d20_fax7field = FALSE;
static signed32  d20_fax8field = FALSE;

static signed32  d20_fax9field = D2_REP_AV;

char *d20_faxifields[] = { d20_fax1field, (char *) &d20_fax2field, (char *) &d20_fax3field,
			   (char *) &d20_fax4field, (char *) &d20_fax5field,
			   (char *) &d20_fax6field, (char *) &d20_fax7field,
			   (char *) &d20_fax8field, (char *) &d20_fax9field };

D20_omask d20_faxomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 26)             DIRECTORY SYSTEM",                    NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  6,  8, d20_attr_name,                  NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, "Fax-Number :",                 NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 23, d20_fax1field,                  "%-45s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11, 12, "A3_Width:",                    NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 30, (char *) &d20_fax2field,  "%.1=N.2=Y.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 12, "B4_Length:",                   NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 30, (char *) &d20_fax3field,  "%.1=N.2=Y.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 12, "B4_Width:",                    NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 30, (char *) &d20_fax4field,  "%.1=N.2=Y.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14, 12, "Fine Resolution:",             NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 30, (char *) &d20_fax5field,  "%.1=N.2=Y.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15, 12, "Two Dimensional:",             NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 30, (char *) &d20_fax6field,  "%.1=N.2=Y.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 16, 12, "Uncompressed:",                NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 16, 30, (char *) &d20_fax7field,  "%.1=N.2=Y.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 17, 12, "Unlimited Length:",            NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 17, 30, (char *) &d20_fax8field,  "%.1=N.2=Y.a", 0, FALSE, FALSE,
	D20_NO_OP,   NORMAL, 20, 10, "Modification:",                NULL, 0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 20, 24, (char *) &d20_fax9field,
	"%.4=Modify Value.3=Delete Value.2=Add Value   .a", 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ************************************************************************* */
/* ------------------------------  M A S K   SHADOWED BY  ------------------ */
/* ************************************************************************* */

static char d20_sh1field[46];
static char d20_sh2field[46];
static char d20_sh3field[46];
static char d20_sh4field[46];
static char d20_sh5field[46];
static char d20_sh6field[46];
static char d20_sh7field[46];
static char d20_sh8field[46];
static char d20_sh1afield[]  = { "_" } ;
static char d20_sh3afield[]  = { "_" } ;
static char d20_sh5afield[]  = { "_" } ;
static char d20_sh7afield[]  = { "_" } ;

char *d20_shifields[] = { d20_sh1field, d20_sh2field, d20_sh3field, d20_sh4field,
			  d20_sh5field, d20_sh6field, d20_sh7field, d20_sh8field,
			  d20_sh1afield, d20_sh3afield, d20_sh5afield, d20_sh7afield};

D20_omask d20_shomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 27)            DIRECTORY SYSTEM",                     NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL, 0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  5, 10, d20_attr_name,                  NULL, 0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 13, "Distinguished Name (DN):",     NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 10, 13, d20_sh1field,                   "%-45s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 13, d20_sh2field,                   "%-45s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 13, d20_sh1afield,                  "%-2s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 13, d20_sh3field,                   "%-45s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 13, d20_sh4field,                   "%-45s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 13, d20_sh3afield,                  "%-2s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 16, 13, d20_sh5field,                   "%-45s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 17, 13, d20_sh6field,                   "%-45s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 13, d20_sh5afield,                  "%-2s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 19, 13, d20_sh7field,                   "%-45s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 20, 13, d20_sh8field,                   "%-45s", 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 13, d20_sh7afield,                  "%-2s", 0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_FKTX,    FKTSCD,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL, 0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* -----------------------  M A S K   Or-add. Types ------------------------ */
/* ************************************************************************* */

static signed32	d20_oradd1field;

char	*d20_oraddifields[] = { (char *)&d20_oradd1field };

D20_omask d20_oraddomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 28)            DIRECTORY SYSTEM",                     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL,
	    0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  4,  5, d20_attr_name,                  NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  5, 30, "OR Address Type: ",    	     NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  5, 47, (char *)&d20_oradd1field,		     
	"%.0=Mnemonic           .\
	   1=Numeric            .\
	   2=Structured postal  .\
	   3=Unstructured postal.\
	   4=Terminal           .a",
	    0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;



/* ************************************************************************* */
/* -----------------------  M A S K   Mnemonic Or-add.  -------------------- */
/* ************************************************************************* */

static char d20_moradd1field[D20_ORADD_CNT_FLD_LEN + 1];
static char d20_moradd2field[D20_ORADD_ADMD_FLD_LEN + 1];
static char d20_moradd3field[D20_ORADD_PRMD_FLD_LEN + 1];
static char d20_moradd4field[D20_ORADD_ORG_FLD_LEN + 1];
static char d20_moradd5field[D20_ORADD_OU1_FLD_LEN + 1];
static char d20_moradd6field[D20_ORADD_OU2_FLD_LEN + 1];
static char d20_moradd7field[D20_ORADD_OU3_FLD_LEN + 1];
static char d20_moradd8field[D20_ORADD_OU4_FLD_LEN + 1];
static char d20_moradd9field[D20_ORADD_CN_FLD_LEN +1 ];
static char d20_moradd10field[D20_ORADD_GN_FLD_LEN + 1];
static char d20_moradd11field[D20_ORADD_INIT_FLD_LEN + 1];
static char d20_moradd12field[D20_ORADD_SN_FLD_LEN + 1];
static char d20_moradd13field[D20_ORADD_GEN_FLD_LEN + 1];
static char d20_moradd14field[D20_ORADD_DT_FLD_LEN + 1];
static char d20_moradd15field[D20_ORADD_DT_FLD_LEN + 1];
static char d20_moradd16field[D20_ORADD_DT_FLD_LEN + 1];
static char d20_moradd17field[D20_ORADD_DT_FLD_LEN + 1];
static char d20_moradd18field[D20_ORADD_DV_FLD_LEN + 1];
static char d20_moradd19field[D20_ORADD_DV_FLD_LEN + 1];
static char d20_moradd20field[D20_ORADD_DV_FLD_LEN + 1];
static char d20_moradd21field[D20_ORADD_DV_FLD_LEN + 1];

static signed32  d20_moradd22field = D2_REP_AV;

char *d20_moraddifields[] = {d20_moradd1field, d20_moradd2field, 
	d20_moradd3field, d20_moradd4field, d20_moradd5field, d20_moradd6field,
	d20_moradd7field, d20_moradd8field, d20_moradd9field, d20_moradd10field,
	d20_moradd11field, d20_moradd12field, d20_moradd13field,
	d20_moradd14field, d20_moradd15field, d20_moradd16field,
	d20_moradd17field, d20_moradd18field, d20_moradd19field,
	d20_moradd20field, d20_moradd21field, (char *)&d20_moradd22field };
	 
D20_omask d20_moraddomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 29)            DIRECTORY SYSTEM",                     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL,
	    0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  4,  5, d20_attr_name,                  NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  5, 30, "OR Address Type: ",	     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  5, 47, "Mnemonic           ",    	     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  7,  3, "Country Name:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 17, d20_moradd1field,		     "%-3s",
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  7, 22, "ADMD Name:",		     NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 33, d20_moradd2field,               "%-16s",
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  7, 51, "PRMD Name:",                   NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 62, d20_moradd3field,		     "%-16s",
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  8,  3, "Org. Name:",                   NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 14, d20_moradd4field,		     "%-64s",
	    D20_ORADD_ORG_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  9,  3, "OU1:",                         NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9,  8, d20_moradd5field,		     "%-32s",
	    D20_ORADD_OU1_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  9, 41, "OU2:",                         NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 46, d20_moradd6field,		     "%-32s",
	    D20_ORADD_OU2_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 10,  3, "OU3:",                         NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 10,  8, d20_moradd7field,		     "%-32s",
	    D20_ORADD_OU3_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 10, 41, "OU4:",                         NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 10, 46, d20_moradd8field,		     "%-32s",
	    D20_ORADD_OU4_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 11,  3, "Com. Name:",                 NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 14, d20_moradd9field,		     "%-64s",
	    D20_ORADD_CN_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 12,  3, "Given Name:",                  NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 15, d20_moradd10field,		     "%-16s",
	    D20_ORADD_GN_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 12, 40, "Initials:",                    NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 50, d20_moradd11field,		     "%-5s",
	    D20_ORADD_INIT_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 13,  3, "Surname:",                     NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 12, d20_moradd12field,		     "%-40s",
	    D20_ORADD_SN_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 13, 54, "Generation:",                  NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 66, d20_moradd13field,		     "%-3s",
	    D20_ORADD_GEN_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 14,  3, "Domain Type1:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 17, d20_moradd14field,		     "%-8s",
	    D20_ORADD_DT_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 14, 40, "Domain Type2:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 54, d20_moradd15field,		     "%-8s",
	    D20_ORADD_DT_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 15,  3, "Domain Type3:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 17, d20_moradd16field,		     "%-8s",
	    D20_ORADD_DT_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 15, 40, "Domain Type4:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 54, d20_moradd17field,		     "%-8s",
	    D20_ORADD_DT_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 16,  3, "Domain Value1:",               NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 16, 18, d20_moradd18field,		     "%-60s",
	    D20_ORADD_DV_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 17,  3, "Domain Value2:",               NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 17, 18, d20_moradd19field,		     "%-60s",
	    D20_ORADD_DV_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 18,  3, "Domain Value3:",               NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 18, 18, d20_moradd20field,		     "%-60s",
	    D20_ORADD_DV_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 19,  3, "Domain Value4:",               NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 19, 18, d20_moradd21field,		     "%-60s",
	    D20_ORADD_DV_FLD_LEN, FALSE, FALSE,
	D20_NO_OP,  NORMAL,  21, 10, "Modification:",                NULL, 
	    0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 21, 24, (char *) &d20_moradd22field,
	    "%.4=Modify Value.3=Delete Value.2=Add Value   .a",
	    0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ************************************************************************* */
/* -----------------------  M A S K   Numeric Or-add.  --------------------- */
/* ************************************************************************* */

static char d20_noradd1field[D20_ORADD_CNT_FLD_LEN + 1];
static char d20_noradd2field[D20_ORADD_ADMD_FLD_LEN + 1];
static char d20_noradd3field[D20_ORADD_PRMD_FLD_LEN + 1];
static char d20_noradd4field[D20_ORADD_NUI_FLD_LEN + 1];
static char d20_noradd5field[D20_ORADD_DT_FLD_LEN + 1];
static char d20_noradd6field[D20_ORADD_DT_FLD_LEN + 1];
static char d20_noradd7field[D20_ORADD_DT_FLD_LEN + 1];
static char d20_noradd8field[D20_ORADD_DT_FLD_LEN + 1];
static char d20_noradd9field[D20_ORADD_DV_FLD_LEN + 1];
static char d20_noradd10field[D20_ORADD_DV_FLD_LEN + 1];
static char d20_noradd11field[D20_ORADD_DV_FLD_LEN + 1];
static char d20_noradd12field[D20_ORADD_DV_FLD_LEN + 1];

static signed32  d20_noradd13field = D2_REP_AV;

char *d20_noraddifields[] = {d20_noradd1field, d20_noradd2field, 
	d20_noradd3field, d20_noradd4field, d20_noradd5field, d20_noradd6field,
	d20_noradd7field, d20_noradd8field, d20_noradd9field, d20_noradd10field,
	d20_noradd11field, d20_noradd12field, (char *)&d20_noradd13field };
	 
D20_omask d20_noraddomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 30)            DIRECTORY SYSTEM",                     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL,
	    0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  4,  5, d20_attr_name,                  NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  5, 30, "OR Address Type: ",	     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  5, 47, "Numeric            ",          NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  7,  3, "Country Name:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 17, d20_noradd1field,		     "%-3s",
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  7, 22, "ADMD Name:",		     NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 33, d20_noradd2field,               "%-16s",
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  7, 51, "PRMD Name:",                   NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 62, d20_noradd3field,		     "%-16s",
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8,  3, "Numeric User Identifier:",     NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 28, d20_noradd4field,		     "%-32s",
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  9,  3, "Domain Type1:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 17, d20_noradd5field,		     "%-8s",
	    D20_ORADD_DT_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  9, 40, "Domain Type2:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 54, d20_noradd6field,		     "%-8s",
	    D20_ORADD_DT_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 10,  3, "Domain Type3:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 10, 17, d20_noradd7field,		     "%-8s",
	    D20_ORADD_DT_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 10, 40, "Domain Type4:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 10, 54, d20_noradd8field,		     "%-8s",
	    D20_ORADD_DT_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 11,  3, "Domain Value1:",               NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 18, d20_noradd9field,		     "%-60s",
	    D20_ORADD_DV_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 12,  3, "Domain Value2:",               NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 18, d20_noradd10field,		     "%-60s",
	    D20_ORADD_DV_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 13,  3, "Domain Value3:",               NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 18, d20_noradd11field,		     "%-60s",
	    D20_ORADD_DV_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 14,  3, "Domain Value4:",               NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 18, d20_noradd12field,		     "%-60s",
	    D20_ORADD_DV_FLD_LEN, FALSE, FALSE,
	D20_NO_OP,   NORMAL, 21, 10, "Modification:",                NULL,
	    0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 21, 24, (char *) &d20_noradd13field,
	    "%.4=Modify Value.3=Delete Value.2=Add Value   .a",
	    0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;



/* ************************************************************************* */
/* ------------------  M A S K   Structured postal Or-add.  ---------------- */
/* ************************************************************************* */

static char d20_soradd1field[D20_ORADD_CNT_FLD_LEN + 1];
static char d20_soradd2field[D20_ORADD_ADMD_FLD_LEN + 1];
static char d20_soradd3field[D20_ORADD_PRMD_FLD_LEN + 1];
static char d20_soradd4field[D20_ORADD_PCNT_FLD_LEN + 1];
static char d20_soradd5field[D20_ORADD_PC_FLD_LEN + 1];
static char d20_soradd6field[D20_ORADD_PAD_FLD_LEN + 1];
static char d20_soradd7field[D20_ORADD_PDPN_FLD_LEN + 1];
static char d20_soradd8field[D20_ORADD_PDSN_FLD_LEN + 1];
static char d20_soradd9field[D20_ORADD_PGDA_FLD_LEN + 1];
static char d20_soradd10field[D20_ORADD_PL_FLD_LEN + 1];
static char d20_soradd11field[D20_ORADD_POFBN_FLD_LEN + 1];
static char d20_soradd12field[D20_ORADD_POFNA_FLD_LEN + 1];
static char d20_soradd13field[D20_ORADD_POFNU_FLD_LEN + 1];
static char d20_soradd14field[D20_ORADD_PORGNA_FLD_LEN + 1];
static char d20_soradd15field[D20_ORADD_PPD_FLD_LEN + 1 ];
static char d20_soradd16field[D20_ORADD_PPN_FLD_LEN + 1 ];
static char d20_soradd17field[D20_ORADD_PSA_FLD_LEN + 1 ];

static signed32  d20_soradd18field = D2_REP_AV;

char *d20_soraddifields[] = {d20_soradd1field, d20_soradd2field, 
	d20_soradd3field, d20_soradd4field, d20_soradd5field, d20_soradd6field,
	d20_soradd7field, d20_soradd8field, d20_soradd9field, d20_soradd10field,
	d20_soradd11field, d20_soradd12field, d20_soradd13field,
	d20_soradd14field, d20_soradd15field, d20_soradd16field,
	d20_soradd17field, (char *)&d20_soradd18field };
	 
D20_omask d20_soraddomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 31)            DIRECTORY SYSTEM",                     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL,
	    0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  4,  5, d20_attr_name,                  NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  5, 30, "OR Address Type: ",	     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  5, 47, "Structured postal  ", 	     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  7,  3, "Country Name:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 17, d20_soradd1field,		     "%-3s",
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  7, 22, "ADMD Name:",		     NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 33, d20_soradd2field,               "%-16s",
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  7, 51, "PRMD Name:",                   NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 62, d20_soradd3field,		     "%-16s",
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8,  3, "Postal Country Name:",         NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 24, d20_soradd4field,		     "%-3s",
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 40, "Postal Code:",                 NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 53, d20_soradd5field,		     "%-16s",
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  9,  3, "Postal Address Details:",      NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 36, d20_soradd6field,		     "%-30s",
	    D20_ORADD_PAD_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 10,  3, "Postal Delivery Point Name:",  NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 10, 36, d20_soradd7field,		     "%-30s",
	    D20_ORADD_PDPN_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 11,  3, "Postal Delivery System Name:", NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 36, d20_soradd8field,		     "%-16s", 
	    D20_ORADD_PDSN_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 12,  3, "Postal General Delivery Address:", 
	     NULL, 0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 36, d20_soradd9field,		     "%-30s",
	    D20_ORADD_PGDA_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 13,  3, "Postal Locale:",               NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 36, d20_soradd10field,		     "%-30s",
	    D20_ORADD_PL_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 14,  3, "Postal Office Box Number:",    NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 36, d20_soradd11field,		     "%-30s",
	    D20_ORADD_POFBN_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 15,  3, "Postal Office Name:",          NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 36, d20_soradd12field,		     "%-30s",
	    D20_ORADD_POFNA_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 16,  3, "Postal Office Number:",        NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 16, 36, d20_soradd13field,		     "%-30s",
	    D20_ORADD_POFNU_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 17,  3, "Postal Organisation Name:",    NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 17, 36, d20_soradd14field,		     "%-30s",
	    D20_ORADD_PORGNA_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 18,  3, "Postal Patron Details:",       NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 18, 36, d20_soradd15field,		     "%-30s",
	    D20_ORADD_PPD_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 19,  3, "Postal Patron Name:",          NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 19, 36, d20_soradd16field,		     "%-30s",
	    D20_ORADD_PPN_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 20,  3, "Postal Street Address:",       NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 20, 36, d20_soradd17field,		     "%-30s",
	    D20_ORADD_PSA_FLD_LEN, FALSE, FALSE,
	D20_NO_OP,   NORMAL, 21, 10, "Modification:",                NULL,
	    0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 21, 24, (char *) &d20_soradd18field,
	    "%.4=Modify Value.3=Delete Value.2=Add Value   .a",
	    0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ************************************************************************* */
/* ------------------  M A S K   Unstructured postal Or-add.  -------------- */
/* ************************************************************************* */

static char d20_uoradd1field[D20_ORADD_CNT_FLD_LEN + 1];
static char d20_uoradd2field[D20_ORADD_ADMD_FLD_LEN + 1];
static char d20_uoradd3field[D20_ORADD_PRMD_FLD_LEN + 1];
static char d20_uoradd4field[D20_ORADD_PCNT_FLD_LEN + 1];
static char d20_uoradd5field[D20_ORADD_PC_FLD_LEN + 1];
static char d20_uoradd6field[D20_ORADD_PAIF1_FLD_LEN + 1];
static char d20_uoradd7field[D20_ORADD_PAIF2_FLD_LEN + 1];
static char d20_uoradd8field[D20_ORADD_PAIF3_FLD_LEN + 1];
static char d20_uoradd9field[D20_ORADD_PAIL_FLD_LEN + 1];
static char d20_uoradd10field[D20_ORADD_PAIL_FLD_LEN + 1];
static char d20_uoradd11field[D20_ORADD_PAIL_FLD_LEN + 1];
static char d20_uoradd12field[D20_ORADD_PAIL_FLD_LEN + 1];
static char d20_uoradd13field[D20_ORADD_PAIL_FLD_LEN + 1];
static char d20_uoradd14field[D20_ORADD_PAIL_FLD_LEN + 1];
static char d20_uoradd15field[D20_ORADD_PDSN_FLD_LEN + 1];

static signed32  d20_uoradd16field = D2_REP_AV;

char *d20_uoraddifields[] = {d20_uoradd1field, d20_uoradd2field, 
	d20_uoradd3field, d20_uoradd4field, d20_uoradd5field, d20_uoradd6field,
	d20_uoradd7field, d20_uoradd8field, d20_uoradd9field, d20_uoradd10field,
	d20_uoradd11field, d20_uoradd12field, d20_uoradd13field, 
	d20_uoradd14field, d20_uoradd15field, (char *)&d20_uoradd16field };
	 
D20_omask d20_uoraddomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 32)            DIRECTORY SYSTEM",                     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL,
	    0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  4,  5, d20_attr_name,                  NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  5, 30, "OR Address Type: ",	     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  5, 47, "Unstructured postal", 	     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  7,  3, "Country Name:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 17, d20_uoradd1field,		     "%-3s",
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  7, 22, "ADMD Name:",		     NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 33, d20_uoradd2field,               "%-16s",
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL,  7, 51, "PRMD Name:",                   NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 62, d20_uoradd3field,		     "%-16s",
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8,  3, "Postal Country Name:",         NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 24, d20_uoradd4field,		     "%-3s",
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 40, "Postal Code:",                 NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 53, d20_uoradd5field,		     "%-16s",
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9,  3, "Postal Address in Full:",      NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 27, d20_uoradd6field,		     "%-51s",
	    D20_ORADD_PAIF1_FLD_LEN, FALSE, TRUE,
	D20_CGET,  HALBHELL, 10,  3, d20_uoradd7field,		     "%-75s",
	    D20_ORADD_PAIF2_FLD_LEN, FALSE, TRUE,
	D20_CGET,  HALBHELL, 11,  3, d20_uoradd8field,		     "%-59s",
	    D20_ORADD_PAIF3_FLD_LEN, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12,  3, "Postal Address in Lines1:",      NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 29, d20_uoradd9field,		     "%-30s",
	    D20_ORADD_PAIL_FLD_LEN, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13,  3, "Postal Address in Lines2:",      NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 29, d20_uoradd10field,		     "%-30s",
	    D20_ORADD_PAIL_FLD_LEN, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14,  3, "Postal Address in Lines3:",      NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 29, d20_uoradd11field,		     "%-30s",
	    D20_ORADD_PAIL_FLD_LEN, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15,  3, "Postal Address in Lines4:",      NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 29, d20_uoradd12field,		     "%-30s",
	    D20_ORADD_PAIL_FLD_LEN, FALSE, FALSE,
	D20_CSAY,    NORMAL, 16,  3, "Postal Address in Lines5:",      NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 16, 29, d20_uoradd13field,		     "%-30s",
	    D20_ORADD_PAIL_FLD_LEN, FALSE, FALSE,
	D20_CSAY,    NORMAL, 17,  3, "Postal Address in Lines6:",      NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 17, 29, d20_uoradd14field,		     "%-30s",
	    D20_ORADD_PAIL_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 18,  3, "Postal Delivery System Name:", NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 18, 32, d20_uoradd15field,		     "%-16s",
	    D20_ORADD_PDSN_FLD_LEN, FALSE, FALSE,
	D20_NO_OP,   NORMAL, 21, 10, "Modification:",                NULL,
	    0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 21, 24, (char *) &d20_uoradd16field,
	    "%.4=Modify Value.3=Delete Value.2=Add Value   .a",
	    0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;



/* ************************************************************************* */
/* -----------------------  M A S K   Term Or-add.  ------------------------ */
/* ************************************************************************* */

static char d20_toradd1field[D20_ORADD_ISNO_FLD_LEN + 1];
static char d20_toradd2field[D20_ORADD_SUA_FLD_LEN + 1];

static signed32  d20_toradd3field;

static char d20_toradd4field[D20_ORADD_X121_FLD_LEN + 1];
static char d20_toradd5field[D20_ORADD_CNT_FLD_LEN + 1];
static char d20_toradd6field[D20_ORADD_ADMD_FLD_LEN + 1];
static char d20_toradd7field[D20_ORADD_PRMD_FLD_LEN + 1];
static char d20_toradd8field[D20_ORADD_TI_FLD_LEN + 1];

static signed32  d20_toradd9field;

static char d20_toradd10field[D20_ORADD_DT_FLD_LEN + 1];
static char d20_toradd11field[D20_ORADD_DT_FLD_LEN + 1];
static char d20_toradd12field[D20_ORADD_DT_FLD_LEN + 1];
static char d20_toradd13field[D20_ORADD_DT_FLD_LEN + 1];

static char d20_toradd14field[D20_ORADD_DV_FLD_LEN + 1];
static char d20_toradd15field[D20_ORADD_DV_FLD_LEN + 1];
static char d20_toradd16field[D20_ORADD_DV_FLD_LEN + 1];
static char d20_toradd17field[D20_ORADD_DV_FLD_LEN + 1];

static signed32  d20_toradd18field = D2_REP_AV;

char *d20_toraddifields[] = {d20_toradd1field, d20_toradd2field, 
	(char *)&d20_toradd3field, d20_toradd4field, d20_toradd5field, 
	d20_toradd6field, d20_toradd7field, d20_toradd8field, 
	(char *)&d20_toradd9field, d20_toradd10field, d20_toradd11field, 
	d20_toradd12field, d20_toradd13field, d20_toradd14field, 
	d20_toradd15field, d20_toradd16field, d20_toradd17field, 
	(char *)&d20_toradd18field };
	 
D20_omask d20_toraddomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 33)            DIRECTORY SYSTEM",                     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL,
	    0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  4,  5, d20_attr_name,                  NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  5, 30, "OR Address Type: ",	     NULL,
	    0, FALSE, FALSE,
        D20_CSAY,  HALBHELL,  5, 47, "Terminal           ", 	     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  7,  3, "ISDN Number:",                 NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 16, d20_toradd1field,		     "%-15s",
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8,  3, "ISDN Subaddress:",             NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 20, d20_toradd2field,		     "%-40s",
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9,  3, "Presentation Address:",        NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 25, (char *)&d20_toradd3field, 
	     "%.0=N.1=Y.a", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 10,  3, "X121 Address:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 10, 17, d20_toradd4field,		     "%-15s",
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 11,  3, "Country Name:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 17, d20_toradd5field,		     "%-3s",
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 11, 22, "ADMD Name:",		     NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 33, d20_toradd6field,               "%-16s",
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 11, 51, "PRMD Name:",                   NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 62, d20_toradd7field,		     "%-16s",
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 12,  3, "Terminal Identifier:",         NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 24, d20_toradd8field,		     "%-24s",
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 13,  3, "Terminal Type:",               NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 18, (char *)&d20_toradd9field,      
	    "%.0=            .3=TELEX       .4=TELETEX     .5=G3_FAX      .6=G4_FAX      .7=IA5_TERMINAL.8=VIDEOTEX    .a",
	    0, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 14,  3, "Domain Type1:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 17, d20_toradd10field,		     "%-8s",
	    D20_ORADD_DT_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 14, 40, "Domain Type2:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 54, d20_toradd11field,		     "%-8s",
	    D20_ORADD_DT_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 15,  3, "Domain Type3:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 17, d20_toradd12field,		     "%-8s",
	    D20_ORADD_DT_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 15, 40, "Domain Type4:",                NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 54, d20_toradd13field,		     "%-8s",
	    D20_ORADD_DT_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 16,  3, "Domain Value1:",               NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 16, 18, d20_toradd14field,		     "%-60s",
	    D20_ORADD_DV_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 17,  3, "Domain Value2:",               NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 17, 18, d20_toradd15field,		     "%-60s",
	    D20_ORADD_DV_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 18,  3, "Domain Value3:",               NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 18, 18, d20_toradd16field,		     "%-60s",
	    D20_ORADD_DV_FLD_LEN, FALSE, FALSE,
	D20_CSAY,  HALBHELL, 19,  3, "Domain Value4:",               NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 19, 18, d20_toradd17field,		     "%-60s",
	    D20_ORADD_DV_FLD_LEN, FALSE, FALSE,
	D20_NO_OP,   NORMAL, 21, 10, "Modification:",                NULL,
	    0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 21, 24, (char *) &d20_toradd18field,
	    "%.4=Modify Value.3=Delete Value.2=Add Value   .a",
	    0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ************************************************************************* */
/* -----------------------  M A S K   DL permission Types ------------------ */
/* ************************************************************************* */

static signed32	d20_dlperm1field;
 char	*d20_dlpermifields[] = {(char *)&d20_dlperm1field};

D20_omask d20_dlpermomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 34)            DIRECTORY SYSTEM",                     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL,
	    0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  4,  5, d20_attr_name,                  NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  5, 30, "DL Submit Permission Type: ",  NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  5, 57, (char *)&d20_dlperm1field,		     
	    "%.0=Individual     .\
	   1=Member Of DL   .\
	   2=Pattern Match  .\
	   3=Member Of Group.a",
	    0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;


/* ************************************************************************* */
/* -----------------------  M A S K   OR Name ------------------------------ */
/* ************************************************************************* */

static char	d20_orname1field[D20_ATVAL_FLD_LEN + 1];
static char	d20_orname2field[D20_ATVAL_FLD_LEN + 1];

static signed32	d20_orname3field;
static signed32	d20_orname4field = D2_REP_AV;
static signed32	d20_orname5field;

char		*d20_ornameifields[] = {d20_orname1field, d20_orname2field,
			(char *)&d20_orname3field, (char *)&d20_orname4field,
			(char *)&d20_orname5field };

D20_omask d20_ornameomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 35)            DIRECTORY SYSTEM",                     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL,
	    0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  4,  5, d20_attr_name,                  NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,    NORMAL,  5, 30, "DL Submit Permission Type: ", NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,  HALBHELL,  5, 57, (char *)&d20_orname5field,	     
	"%.0=Individual     .\
	   1=Member Of DL   .\
	   2=Pattern Match  .a",
	   0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, "Directory Name: ",   	     NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 26, d20_orname1field,   	     "%-45s",
	    D20_ATVAL_FLD_LEN, FALSE, TRUE,
	D20_CGET,  HALBHELL,  9, 26, d20_orname2field,   	     "%-45s",
	    D20_ATVAL_FLD_LEN, FALSE, FALSE,
	D20_CSAY,    NORMAL, 11, 10, "OR Address: ",    	     NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 11, 22, (char *)&d20_orname3field,  
	    "%.0=N.1=Y.a",
	    0, FALSE, FALSE,
	D20_NO_OP,   NORMAL,  21, 10, "Modification:",                NULL,
	    0, FALSE, FALSE,
	D20_NO_OP, HALBHELL,  21, 24, (char *) &d20_orname4field,
	    "%.4=Modify Value.3=Delete Value.2=Add Value   .a",
	    0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;

/* ************************************************************************* */
/* -----------------------  M A S K   Member Of Group ---------------------- */
/* ************************************************************************* */

static char	d20_mog1field[D20_ATVAL_FLD_LEN + 1];
static char	d20_mog2field[D20_ATVAL_FLD_LEN + 1];

static signed32	d20_mog3field = D2_REP_AV;

	char	*d20_mogifields[] = {d20_mog1field, d20_mog2field,
				     (char *)&d20_mog3field};
D20_omask d20_mogomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(mask 36)            DIRECTORY SYSTEM",                     NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL,
	    0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  4,  5, d20_attr_name,                  NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  5, 30, 
	    "DL Submit Permission Type: Member Of Group",  NULL,
	    0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 10, "Member Of Group: ",   	     NULL,
	    0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 27, d20_mog1field,		     "%-45s",
	    D20_ATVAL_FLD_LEN, FALSE, TRUE,
	D20_CGET,  HALBHELL,  9, 27, d20_mog2field,   	     	     "%-45s",
	    D20_ATVAL_FLD_LEN, FALSE, FALSE,
	D20_NO_OP,   NORMAL, 21, 10, "Modification:",                NULL,
	    0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 21, 24, (char *) &d20_mog3field,
	    "%.4=Modify Value.3=Delete Value.2=Add Value   .a",
	    0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL,
	    0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;




/* ************************************************************************* */
/* ---------------  Masks for security package follow. --------------------- */
/* ************************************************************************* */

/* ************************************************************************* */
/* -----------------------  M A S K   Certificate     ---------------------- */
/* ************************************************************************* */

/* IMPORTANT: If you change this mask, don't forget to check the defines */
/*            in d2spec_msk.h                                            */

static signed32	d20_cert1field;
static signed32	d20_cert2field;
static signed32	d20_cert3field;
static signed32	d20_cert4field;
static signed32	d20_cert5field;

static char	d20_cert6field[(D20_CERT_LINE_LEN * D20_T61_CONV_FACT) + 1];
static char	d20_cert7field[(D20_CERT_LINE_LEN * D20_T61_CONV_FACT) + 1];

static signed32	d20_cert8field;
static signed32	d20_cert9field;
static signed32	d20_cert10field;
static signed32	d20_cert11field;
static signed32	d20_cert12field;
static signed32	d20_cert13field;
static signed32	d20_cert14field;
static signed32	d20_cert15field;
static signed32	d20_cert16field;
static signed32	d20_cert17field;
static signed32	d20_cert18field;
static signed32	d20_cert19field;

static char	d20_cert20field[(D20_CERT_LINE_LEN * D20_T61_CONV_FACT) + 1];
static char	d20_cert21field[(D20_CERT_LINE_LEN * D20_T61_CONV_FACT) + 1];

static signed32	d20_cert22field;
static signed32	d20_cert23field;
static signed32	d20_cert24field = D2_DEL_AV;

	char	*d20_certifields[] = {
			(char *)&d20_cert1field, (char *)&d20_cert2field,
			(char *)&d20_cert3field, (char *)&d20_cert4field,
			(char *)&d20_cert5field,
			d20_cert6field, d20_cert7field,
			(char *)&d20_cert8field, (char *)&d20_cert9field,
			(char *)&d20_cert10field, (char *)&d20_cert11field,
			(char *)&d20_cert12field, (char *)&d20_cert13field,
			(char *)&d20_cert14field, (char *)&d20_cert15field,
			(char *)&d20_cert16field, (char *)&d20_cert17field,
			(char *)&d20_cert18field, (char *)&d20_cert19field,
			d20_cert20field, d20_cert21field,
			(char *)&d20_cert22field, (char *)&d20_cert23field,
			(char *)&d20_cert24field
		};

D20_omask d20_certomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(Mask 36)            DIRECTORY SYSTEM",                     NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL,
		0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  4, 10, d20_attr_name,                  NULL,
		0, FALSE, FALSE,
	D20_NO_OP,    NORMAL,  4, 65, (char *)&d20_cert1field,
		"%.0=Forward.1=Reverse.a",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  6,  5, "Version:",   	     	     NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  6, 14, (char *)&d20_cert2field,        "%-4d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  6, 50, "Serial Number:", 	     	     NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  6, 65, (char *)&d20_cert3field,        "%-10d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  7,  5, "Signature", 	     	     NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8,  6, "Algorithm:", 	     	     NULL,
		0, FALSE, FALSE,
	D20_CGET,    NORMAL,  8, 17, (char *)&d20_cert4field,
		"%.0=rsa           .1=sqMod-n       .2=sqMod-nWithRSA.a",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 50, "Parameter:", 	     	     NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  8, 61, (char *)&d20_cert5field,        "%-4d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9,  5, "Issuer:", 	     	     NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 25, d20_cert6field,   	     	     "%-45s",
		D20_CERT_LINE_LEN, TRUE, TRUE,
	D20_CGET,  HALBHELL, 10, 25, d20_cert7field,   	     	     "%-45s",
		D20_CERT_LINE_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 11,  5, "Validity", 	     	     NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12,  6, "Not Before", 	     	     NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 18, "Year:",                        NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 24, (char *)&d20_cert8field,        "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 27, "Month:", 	     	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 34, (char *)&d20_cert9field,     "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 37, "Day:", 	     	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 42, (char *)&d20_cert10field,    "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 45, "Hours:",                    NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 52, (char *)&d20_cert11field,    "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 55, "Minutes:",     	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 64, (char *)&d20_cert12field,    "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 67, "Seconds:",      	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 76, (char *)&d20_cert13field,    "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13,  6, "Not After", 	     	  NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 18, "Year:", 	     	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 24, (char *)&d20_cert14field,    "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 27, "Month:", 	     	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 34, (char *)&d20_cert15field,    "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 37, "Day:", 	     	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 42, (char *)&d20_cert16field,    "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 45, "Hours:",                    NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 52, (char *)&d20_cert17field,    "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 55, "Minutes:",     	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 64, (char *)&d20_cert18field,    "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 13, 67, "Seconds:",      	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 13, 76, (char *)&d20_cert19field,    "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14,  5, "Subject:", 	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 14, 25, d20_cert20field,   	  "%-45s",
		D20_CERT_LINE_LEN, TRUE, TRUE,
	D20_CGET,  HALBHELL, 15, 25, d20_cert21field,   	  "%-45s",
		D20_CERT_LINE_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 16,  5, "Subject Public Key Info",   NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 17,  6, "Algorithm Identifier",      NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 18,  7, "Algorithm:", 	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,    NORMAL, 18, 18, (char *)&d20_cert22field,
		"%.0=rsa           .1=sqMod-n       .2=sqMod-nWithRSA.a",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 18, 50, "Parameter:", 	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 18, 61, (char *)&d20_cert23field,    "%-4d",
		0, FALSE, FALSE,
	D20_NO_OP,   NORMAL, 21, 10, "Modification:",             NULL,
		0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 21, 24, (char *) &d20_cert24field,
		"%.3=Delete Value.a",
		0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;



/* ************************************************************************* */
/* -----------------------  M A S K   Certificate Pair --------------------- */
/* ************************************************************************* */

static signed32	d20_certp1field;
static signed32	d20_certp2field;
static signed32	d20_certp3field = D2_DEL_AV;

	char	*d20_certpifields[] = {
			(char *)&d20_certp1field, (char *)&d20_certp2field,
			(char *)&d20_certp3field
		};

D20_omask d20_certpomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(Mask 37)            DIRECTORY SYSTEM",                  NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                NULL,
		0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  4, 10, d20_attr_name,               NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  6, 15, "Forward Certificate:",   	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  6, 40, (char *)&d20_certp1field,
		"%.0=NO .1=YES.a",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  8, 15, "Reverse Certificate:", 	  NULL,
		0, FALSE, FALSE,
	D20_CGET,    NORMAL,  8, 40, (char *)&d20_certp2field,
		"%.0=NO .1=YES.a",
		0, FALSE, FALSE,
	D20_NO_OP,   NORMAL, 21, 10, "Modification:",             NULL,
		0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 21, 24, (char *) &d20_certp3field,
		"%.3=Delete Value.a",
		0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;



/* ************************************************************************* */
/* ----------------------- M A S K   Certificate List ---------------------- */
/* ************************************************************************* */

static signed32	d20_certl1field;
static signed32	d20_certl2field;

static char	d20_certl3field[(D20_CERT_LINE_LEN * D20_T61_CONV_FACT) + 1];
static char	d20_certl4field[(D20_CERT_LINE_LEN * D20_T61_CONV_FACT) + 1];

static signed32	d20_certl5field;
static signed32	d20_certl6field;
static signed32	d20_certl7field;
static signed32	d20_certl8field;
static signed32	d20_certl9field;
static signed32	d20_certl10field;
static signed32	d20_certl11field = 0;
static signed32	d20_certl12field = D2_DEL_AV;

	char	*d20_certlifields[] = {
			(char *)&d20_certl1field, (char *)&d20_certl2field,
			d20_certl3field, d20_certl4field,
			(char *)&d20_certl5field, (char *)&d20_certl6field, 
			(char *)&d20_certl7field, (char *)&d20_certl8field, 
			(char *)&d20_certl9field, (char *)&d20_certl10field, 
			(char *)&d20_certl11field, (char *)&d20_certl12field
		};

D20_omask d20_certlomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(Mask 38a)           DIRECTORY SYSTEM",                  NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                NULL,
		0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  4, 10, d20_attr_name,               NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  6,  5, "Signature", 	     	  NULL,	
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  6, 30, "Algorithm:", 	     	  NULL,	
		0, FALSE, FALSE,
	D20_CGET,    NORMAL,  6, 41, (char *)&d20_certl1field,
		"%.0=rsa           .1=sqMod-n       .2=sqMod-nWithRSA.a",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  7, 30, "Parameter:", 	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 41, (char *)&d20_certl2field,    "%-4d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9,  5, "Issuer:", 	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9, 30, d20_certl3field,             "%-45s",
		D20_CERT_LINE_LEN, TRUE, TRUE,
	D20_CGET,  HALBHELL, 10, 30, d20_certl4field,             "%-45s",
		D20_CERT_LINE_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 12,  5, "Last Update", 	     	  NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 18, "Year:", 	     	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 24, (char *)&d20_certl5field,    "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 27, "Month:", 	     	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 34, (char *)&d20_certl6field,    "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 37, "Day:", 	     	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 42, (char *)&d20_certl7field,    "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 45, "Hours:", 	     	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 52, (char *)&d20_certl8field,    "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 55, "Minutes:",     	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 64, (char *)&d20_certl9field,    "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 12, 67, "Seconds:",      	     	  NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 76, (char *)&d20_certl10field,   "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14,  5, "Revoked Certificates:",	  NULL,
		0, FALSE, FALSE,
	D20_CGET,    NORMAL, 14, 30, (char *)&d20_certl11field,	     
		"%.0=NO .1=YES.a",
		0, FALSE, FALSE,
	D20_NO_OP,   NORMAL, 20, 10, "Modification:",             NULL,
		0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 20, 24, (char *) &d20_certl12field,
		"%.3=Delete Value.a",
		0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL,	
		0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;




/* ************************************************************************* */
/* ----------------------- M A S K   Revoked Certificate ------------------- */
/* ************************************************************************* */

static signed32	d20_rcert1field;
static signed32	d20_rcert2field;

static char	d20_rcert3field[(D20_CERT_LINE_LEN * D20_T61_CONV_FACT) +1];
static char	d20_rcert4field[(D20_CERT_LINE_LEN * D20_T61_CONV_FACT) +1];

static signed32	d20_rcert5field;
static signed32	d20_rcert6field;
static signed32	d20_rcert7field;
static signed32	d20_rcert8field;
static signed32	d20_rcert9field;
static signed32	d20_rcert10field;
static signed32	d20_rcert11field;
static signed32	d20_rcert12field = 0;	/* NO	*/
static signed32	d20_rcert13field = D2_DEL_AV;

	char	*d20_rcertifields[] = {
			(char *)&d20_rcert1field, (char *)&d20_rcert2field,
			d20_rcert3field, d20_rcert4field,
			(char *)&d20_rcert5field, (char *)&d20_rcert6field, 
			(char *)&d20_rcert7field, (char *)&d20_rcert8field, 
			(char *)&d20_rcert9field, (char *)&d20_rcert10field, 
			(char *)&d20_rcert11field, (char *)&d20_rcert12field,
			(char *)&d20_rcert13field
		};

D20_omask d20_rcertomask[] = {
	D20_CSAY,    NORMAL,  2,  5,
	"(Mask 38b)           DIRECTORY SYSTEM",                     NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  2, 56, d20_actopmsg,                   NULL,
		0, FALSE, FALSE,
	D20_CSAY, UNDERLINE,  4, 10, d20_attr_name,                  NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  4, 43, "Revoked Certificate",          NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  6,  5, "Signature", 	     	     NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  6, 30, "Algorithm:", 	     	     NULL,
		0, FALSE, FALSE,
	D20_CGET,    NORMAL,  6, 41, (char *)&d20_rcert1field,
		"%.0=rsa           .1=sqMod-n       .2=sqMod-nWithRSA.a",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  7, 30, "Parameter:", 	     	     NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  7, 41, (char *)&d20_rcert2field,        "%-4d",	
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL,  9,  5, "Issuer:", 	     	     NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL,  9,  5, d20_rcert3field,  	     	     "%-45s",
		D20_CERT_LINE_LEN, TRUE, TRUE,
	D20_CGET,  HALBHELL, 10, 30, d20_rcert4field,  	     	     "%-45s",
		D20_CERT_LINE_LEN, TRUE, FALSE,
	D20_CSAY,    NORMAL, 12,  5, "User Certificate:",            NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 12, 30, (char *)&d20_rcert5field,        "%-10d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 14,  5, "Revocation Date",	     	     NULL,
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15, 18, "Year:", 	     	     	     NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 24, (char *)&d20_rcert6field,        "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15, 27, "Month:", 	     	     	     NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 34, (char *)&d20_rcert7field,        "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15, 37, "Day:", 	     	     	     NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 42, (char *)&d20_rcert8field,        "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15, 45, "Hours:", 	     	     	     NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 52, (char *)&d20_rcert9field,        "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15, 55, "Minutes:",     	     	     NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 64, (char *)&d20_rcert10field,        "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 15, 67, "Seconds:",      	     	     NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 15, 76, (char *)&d20_rcert11field,        "%-2d",
		0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 17,  5, "Other revoked Certificates:",   NULL,
		0, FALSE, FALSE,
	D20_CGET,  HALBHELL, 17, 33, (char *)&d20_rcert12field,
		"%.0=NO .1=YES.a",
		0, FALSE, FALSE,
	D20_NO_OP,   NORMAL, 20, 10, "Modification:",                NULL,
		0, FALSE, FALSE,
	D20_NO_OP, HALBHELL, 20, 24, (char *) &d20_rcert13field,
		"%.3=Delete Value.a",
		0, FALSE, FALSE,
	D20_FKTX,   FKTMENU,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_FKTX,    FKTDEL,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_FKTX,   FKTHELP,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_NO_OP,   FKTSCD,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_NO_OP,   FKTSCU,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_NO_OP,    FKTF1,  0,  0, NULL,                        NULL,
		0, FALSE, FALSE,
	D20_EOMSK,   NORMAL } ;



/* ************************************************************************* */
/* ----------------------- M A S K   DME attributes ------------------------ */
/* ************************************************************************* */
/* Content of this mask: alternate Address                                   */
/* Author              : Jean-Marc Bonnaudet                                 */
/* Date                : Wed Sep 15 11:36:54 MDT 1993                        */
/* ************************************************************************* */

/* IMPORTANT: If you change this mask, don't forget to check the defines */
/*            in d2spec_msk.h                                            */

 char	d20_dme_addr1[(DME_ADDRESS_FIRST_LINE_LEN * D20_T61_CONV_FACT) + 1];
 char	d20_dme_addr2[(DME_ADDRESS_LINE_LEN * D20_T61_CONV_FACT) + 1];
 char	d20_dme_addr3[(DME_ADDRESS_LINE_LEN * D20_T61_CONV_FACT) + 1];
 char	d20_dme_addr4[(DME_ADDRESS_LINE_LEN * D20_T61_CONV_FACT) + 1];
 char	d20_dme_addr5[(DME_ADDRESS_LINE_LEN * D20_T61_CONV_FACT) + 1];
 char	d20_dme_object_id[DME_MAX_OBJECT_ID][DME_OBJECT_ID_LEN + 1];

D20_omask d20_dmeomask[] = {
    D20_CSAY,    NORMAL,     2,  5,
    "(Mask xx)            DIRECTORY SYSTEM",                        NULL,
	0, FALSE, FALSE,
    D20_CSAY,    NORMAL,     2, 56, d20_actopmsg,                   NULL,
	0, FALSE, FALSE,
    D20_CSAY,    UNDERLINE,  5,  5, "alternate address:",           NULL,
	0, FALSE, FALSE,
    D20_CSAY,    NORMAL,     6,  5, "address:",                     NULL,
	0, FALSE, FALSE,
    D20_CGET,    NORMAL,     6, 15, d20_dme_addr1,                  "%-61s",
	DME_ADDRESS_FIRST_LINE_LEN, TRUE, TRUE,
    D20_CGET,    NORMAL,     7,  5, d20_dme_addr2,                  "%-71s",
	DME_ADDRESS_LINE_LEN, TRUE, TRUE,
    D20_CGET,    NORMAL,     8,  5, d20_dme_addr3,                  "%-71s",
	DME_ADDRESS_LINE_LEN, TRUE, TRUE,
    D20_CGET,    NORMAL,     9,  5, d20_dme_addr4,                  "%-71s",
	DME_ADDRESS_LINE_LEN, TRUE, TRUE,
    D20_CGET,    NORMAL,    10,  5, d20_dme_addr5,                  "%-71s",
	DME_ADDRESS_LINE_LEN, TRUE, FALSE,
    D20_CSAY,    NORMAL,    11,  5, "protocol:",                    NULL,
	0, FALSE, FALSE,
    D20_CSAY,    NORMAL,    12,  9, "object id. 1:",                NULL,
	0, FALSE, FALSE,
    D20_CGET,    NORMAL,    12, 30, d20_dme_object_id[0],           "%-28s",
	0, FALSE, FALSE,
    D20_CSAY,    NORMAL,    13,  9, "object id. 2:",                NULL,
	0, FALSE, FALSE,
    D20_CGET,    NORMAL,    13, 30, d20_dme_object_id[1],           "%-28s",
	0, FALSE, FALSE,
    D20_CSAY,    NORMAL,    14,  9, "object id. 3:",                NULL,
	0, FALSE, FALSE,
    D20_CGET,    NORMAL,    14, 30, d20_dme_object_id[2],           "%-28s",
	0, FALSE, FALSE,
    D20_CSAY,    NORMAL,    15,  9, "object id. 4:",                NULL,
	0, FALSE, FALSE,
    D20_CGET,    NORMAL,    15, 30, d20_dme_object_id[3],           "%-28s",
	0, FALSE, FALSE,
    D20_CSAY,    NORMAL,    16,  9, "object id. 5:",                NULL,
	0, FALSE, FALSE,
    D20_CGET,    NORMAL,    16, 30, d20_dme_object_id[4],           "%-28s",
	0, FALSE, FALSE,
    D20_CSAY,    NORMAL,    17,  9, "object id. 6:",                NULL,
	0, FALSE, FALSE,
    D20_CGET,    NORMAL,    17, 30, d20_dme_object_id[5],           "%-28s",
	0, FALSE, FALSE,
    D20_CSAY,    NORMAL,    18,  9, "object id. 7:",                NULL,
	0, FALSE, FALSE,
    D20_CGET,    NORMAL,    18, 30, d20_dme_object_id[6],           "%-28s",
	0, FALSE, FALSE,
    D20_CSAY,    NORMAL,    19,  9, "object id. 8:",                NULL,
	0, FALSE, FALSE,
    D20_CGET,    NORMAL,    19, 30, d20_dme_object_id[7],           "%-28s",
	0, FALSE, FALSE,
    D20_CSAY,    NORMAL,    20,  9, "object id. 9:",                NULL,
	0, FALSE, FALSE,
    D20_CGET,    NORMAL,    20, 30, d20_dme_object_id[8],           "%-28s",
	0, FALSE, FALSE,
    D20_CSAY,    NORMAL,    21,  9, "object id.10:",                NULL,
	0, FALSE, FALSE,
    D20_CGET,    NORMAL,    21, 30, d20_dme_object_id[9],           "%-28s",
	0, FALSE, FALSE,
    D20_FKTX,    FKTMENU,    0,  0, NULL,                        NULL,
	0, FALSE, FALSE,
    D20_FKTX,    FKTCR,      0,  0, NULL,                        NULL,
	0, FALSE, FALSE,
    D20_FKTX,    FKTDEL,     0,  0, NULL,                        NULL,	
	0, FALSE, FALSE,
    D20_FKTX,    FKTHELP,    0,  0, NULL,                        NULL,
	0, FALSE, FALSE,
    D20_NO_OP,   FKTSCD,     0,  0, NULL,                        NULL,
	0, FALSE, FALSE,
    D20_NO_OP,   FKTSCU,     0,  0, NULL,                        NULL,	
	0, FALSE, FALSE,
    D20_NO_OP,   FKTF1,      0,  0, NULL,                        NULL,
	0, FALSE, FALSE,
    D20_EOMSK,   NORMAL } ;
