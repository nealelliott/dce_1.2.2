/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21spat.h,v $
 * Revision 1.1.10.2  1996/02/18  23:35:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:33  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:59:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:31  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:48:31  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:43:33  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:19:01  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:38:17  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:39:51  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:29:04  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  03:56:36  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:41:26  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D21SPAT_H
#define _D21SPAT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d21spat_rcsid[] = "@(#)$RCSfile: d21spat.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:35:33 $";
#endif

/*----------------------------------------------------------------------*/
/*									*/
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG		*/
/* All Rights Reserved							*/
/*									*/
/* File Name	: d21spat.h						*/
/*									*/
/* Description	: This module contains definitions used for conversion	*/
/*		  of special attributes in the DUA from IAPL to APDU	*/
/*		  and vice versa					*/
/*									*/
/* Date		: March 08, 1991					*/
/*									*/
/* Author	: G. Rustemeyer						*/
/*	(newer stuff: Jochen Keutel, SNI BA NM 123, Munich, Germany)	*/
/*									*/
/*----------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/* for ttxid                                               	      */
/*--------------------------------------------------------------------*/

#define D2_TERM    0                    /* for d2_terminal   */
#define D2_CTRL    1                    /* for d2_controls   */
#define D2_GRPH    2                    /* for d2_graphics   */
#define D2_MISC    3                    /* for d2_miscel     */
#define D2_PAGE    4                    /* for d2_pages      */
#define D2_PRIV    5                    /* for d2_privates   */


/*--------------------------------------------------------------------*/
/* structures for special attributes                                  */
/*--------------------------------------------------------------------*/

typedef struct {
	unsigned16            d2_str_type;
	unsigned16            d2_filler;
	signed16            d2_str_len;
	signed16            d2_offset;
} d21_ttxid;

typedef struct {
	signed16            d2_str_len;
	signed16            d2_offset;
} d21_faxnr;


typedef struct {
		 signed16 d21_offset;
		 signed16 d21_length;
	       } d21_obj_len;

typedef struct {
		 signed16 d21_choice;
		 signed16 d21_count;
	       } d21_crdesc;

typedef struct {
		 unsigned16 d21_match;
		 unsigned16 d21_filler;
		 signed16 d21_offset;
		 signed16 d21_length;
	       } d21_itdesc;

/*--------------------------------------------------------------------*/
/*  typedefs and defines for MHS X.400 information                    */
/*--------------------------------------------------------------------*/

/*--------------------------------*/
/* D21_OR_address_part tags       */
/*--------------------------------*/
#define D21_ORA_CTRY_N          1
#define D21_ORA_ADMD_N          2
#define D21_ORA_X121_A          3
#define D21_ORA_T_ID            4
#define D21_ORA_PRMD_N          5
#define D21_ORA_COM_N           6
#define D21_ORA_ORG_N           7
#define D21_ORA_NUM_ID          8
#define D21_ORA_SURNAME         9
#define D21_ORA_GIV_N           10
#define D21_ORA_INIT            11
#define D21_ORA_GEN             12
#define D21_ORA_ORGU_N1         13
#define D21_ORA_ORGU_N2         14
#define D21_ORA_ORGU_N3         15
#define D21_ORA_ORGU_N4         16
#define D21_ORA_DOM_T1          17
#define D21_ORA_DOM_V1          18
#define D21_ORA_DOM_T2          19
#define D21_ORA_DOM_V2          20
#define D21_ORA_DOM_T3          21
#define D21_ORA_DOM_V3          22
#define D21_ORA_DOM_T4          23
#define D21_ORA_DOM_V4          24
#define D21_ORA_P_D_S_N         25
#define D21_ORA_P_CTRY          26
#define D21_ORA_P_CODE          27
#define D21_ORA_P_O_N           28
#define D21_ORA_P_O_NUM         29
#define D21_ORA_P_A_D           30
#define D21_ORA_P_P_N           31
#define D21_ORA_P_ORG_N         32
#define D21_ORA_P_P_D           33
#define D21_ORA_P_A_F           34
#define D21_ORA_P_A_L           35
#define D21_ORA_P_S_A           36
#define D21_ORA_P_O_B_N         37
#define D21_ORA_P_G_D_A         38
#define D21_ORA_P_D_P_N         39
#define D21_ORA_P_LOC           40
#define D21_ORA_PRES_A          41
#define D21_ORA_ISDN_N          42
#define D21_ORA_ISDN_S          43
#define D21_ORA_T_T             44


typedef struct {
	signed16          d21_strl_off;
	signed16          d21_strl_len;
	unsigned16        d21_strl_rep;       /* D2_PRINTABLE or D2_T61 */
	unsigned16          d21_strl_fil;
} D21_str_list;
/* FOLLOWED BY: 1 string */


typedef struct {
	unsigned16        d21_ora_tag;       /* D21_ORA_ADMD_N ...      */
	unsigned16        d21_ora_fil;
	signed16          d21_ora_off;
	signed16          d21_ora_len;
} D21_OR_address_part;
/* FOLLOWED BY: 1 or more D21_str_list structures */
/*           or 1 string                          */
/*           or 1 integer                         */

/*--------------------------------*/
/* D21_OR_name tags               */
/*--------------------------------*/
#define D21_ORN_DN            1
#define D21_ORN_ORA           2


typedef struct {
	unsigned16        d21_orn_tag;       /* D21_ORN_dn ...          */
	unsigned16        d21_orn_fil;
	signed16          d21_orn_off;
	signed16          d21_orn_len;
} D21_OR_name;
/* FOLLOWED BY: 1 name-string                            */
/*           or 1 or more D21_OR_address_part structures */
/*           or both                                     */


/*--------------------------------*/
/* D21_DL_sub_perm tags           */
/*--------------------------------*/
#define D21_DLSP_INDIV           0
#define D21_DLSP_MEM_G           3
#define D21_DLSP_PATT            2
#define D21_DLSP_MEM_DL          1


typedef struct {
	unsigned16        d21_dlsp_tag;       /* D21_ORN_dn ...          */
	unsigned16        d21_dlsp_fil;
	signed16          d21_dlsp_off;
	signed16          d21_dlsp_len;
} D21_DL_sub_perm;
/* FOLLOWED BY: 1 name-string                            */
/*           or 1 or 2 D21_OR_name structures            */

/* prototyping for functions handling special attributes */

/* from d21spafm.c */

void d21_121_ttxid_fr_ipc ( char ** , char ** , Bool );
void d21_122_guide_fr_ipc ( char ** , char ** , Bool );
void d21_128_crit_fr_ipc ( char ** , char ** );
void d21_123_T61seq_fr_ipc ( char ** , char ** , Bool );
void d21_124_telexnr_fr_ipc ( char ** , char ** , Bool );
void d21_125_faxnr_fr_ipc ( char ** , char ** , Bool );
d2_ret_val d21_126_post_ad_fr_ipc ( char ** , char ** );

/* from d21spalm.c */

signed32 d21_111_ttxid_msglen ( D2_ttxid * );
signed32 d21_112_guide_msglen ( D2_guide * );
signed32 d21_113_T61seq_msglen ( D2_T61_seq * );
signed32 d21_114_telexnr_msglen ( D2_telex_number * );
signed32 d21_115_faxnr_msglen ( D2_faxnumber * );
signed32 d21_116_post_ad_msglen ( D2_str_list * );

/* from d21spals.c */

signed32 d21_131_ttxid_struct_len ( char * );
signed32 d21_132_guide_struct_len ( char * );
signed32 d21_133_T61seq_struct_len ( char * );
signed32 d21_134_telexnr_struct_len ( char * );
signed32 d21_135_faxnr_struct_len ( char * );
signed32 d21_136_post_ad_struct_len ( char * );

/* from d21spatm.c */

void d21_101_ttxid_to_ipc ( D2_ttxid * , char ** );
d2_ret_val d21_102_guide_to_ipc ( D2_guide * , char ** );
d2_ret_val d21_108_crit_to_ipc ( D2_criteria * , char ** );
void d21_103_T61seq_to_ipc ( D2_T61_seq * , char ** );
void d21_104_telexnr_to_ipc ( D2_telex_number * , char ** );
void d21_105_faxnr_to_ipc ( D2_faxnumber * , char ** );
d2_ret_val d21_106_post_ad_to_ipc ( D2_str_list * , char ** );


#endif /* _D21SPAT_H */
