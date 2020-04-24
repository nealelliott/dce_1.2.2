/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2adm.h,v $
 * Revision 1.1.756.2  1996/02/18  23:35:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:12  marty]
 *
 * Revision 1.1.756.1  1995/12/08  16:01:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:02  root]
 * 
 * Revision 1.1.754.5  1994/07/06  15:07:34  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:16:32  marrek]
 * 
 * Revision 1.1.754.4  1994/06/21  14:47:14  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:41:24  marrek]
 * 
 * Revision 1.1.754.3  1994/05/10  15:55:56  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:38:23  marrek]
 * 
 * Revision 1.1.754.2  1994/03/23  15:18:28  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:30:06  keutel]
 * 
 * Revision 1.1.754.1  1994/02/22  18:51:44  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:46:39  marrek]
 * 
 * Revision 1.1.752.2  1993/10/14  17:27:48  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:26:58  keutel]
 * 
 * Revision 1.1.752.1  1993/10/13  17:32:39  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:26:41  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  12:46:39  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:41:42  marrek]
 * 
 * Revision 1.1.4.4  1993/01/29  08:39:41  marrek
 * 	Introduce DS_12_NO_FKT and DS_12_F8_POS
 * 	[1993/01/28  10:46:49  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:41:26  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:30:01  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:10:52  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  10:43:47  marrek]
 * 
 * Revision 1.1.2.4  1992/07/01  16:53:27  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:22:28  melman]
 * 
 * Revision 1.1.2.3  1992/06/30  20:59:46  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:40:50  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  04:07:54  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:42:49  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D2ADM_H
#define _D2ADM_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d2adm_rcsid[] = "@(#)$RCSfile: d2adm.h,v $ $Revision: 1.1.756.2 $ $Date: 1996/02/18 23:35:57 $";
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
/* NAME         : d2adm.h      Directory typedefs for non-BS-masks    */
/*								      */
/* AUTOR        : Volpers                                             */
/* DATUM        : 02.01.1988                                          */
/*								      */
/* KOMPONENTE	: DS						      */
/*								      */
/* DOK.-NR.	: DS-Designspezifikation (Ausgabe 1)		      */
/*								      */
/* PRD#/VERS.	:						      */
/*								      */
/* DESCRIPTION  : contains the typedefs for the data structures,      */
/*                where the values from the masks will be written     */
/*								      */
/* SYSTEMABHAENGIGKEIT:	 COMMON					      */
/*								      */
/*								      */
/* Vers.Nr. |  Datum  |	 Aenderungen		       | KZ | CR# FM# */
/*   1.0    | 20.08.87| Original                       |    |         */
/*datoff***************************************************************/


/* NLS file name */

#define D20_ADM_NLSFILE "gdsditadm.cat"
#define D20_ADMHELP_NLSFILE "gdsdithelp.cat"


			/* number of strings in different pointer arrays */
			/* in the mask module                            */
#define D20_NOP         8
#define D20_NCOP        3
#define D20_NOPMSG      11
#define D20_NS_OPMSG    16
#define D20_NDUPD_OP    9
#define D20_NTREE_OP    8
#define D20_NDISP       10
#define D20_NOPCLASS    5
#define D20_NERR_HEADER 4
#define D20_NATT_SYNTAX 6
#define D20_NBOOL       2

#define D20_M_SIZE      65      /* maximal number of fields to be saved */
				/* before displaying attributes         */
				/* (relevant masks: d20_certomask,	*/
				/* d20_6bomask, d20_7omask, d20_7aomask)*/


			/* max. number of attributes in attribute list */
			/* (for "scheme administration" = D20_AT_MAX,  */
			/*  otherwise = D2_AT_MAX)                     */
#define D20_AT_MAX      (10 * D2_AT_MAX)

			/* constants to be used in "d20_get_attribute" */
			/*                     and "d20_dnget"         */
#define D20_BEGIN                1        /* a sign whether the result */
					  /* of the directory call has */
					  /* to be returned to the     */
					  /* admin-interface starting  */
					  /* at the beginning of the   */
					  /* result block              */
#define D20_CONTINUE             2        /* parts ot the result block */
					  /* are already returned      */
#define D20_NEXT                 4        /* a sign that attributes for*/
					  /* the next DN in the queue  */
					  /* should be handled         */

			/* constants to be used in ds4_b02_fill_attr"*/
#define D20_AINSERT           0x01        /* sign for fill_attr      */
					  /* more attributes         */
#define D20_ATYPE_ONLY	      0x02	  /* sign for fill_attr      */
					  /* types only		     */
#define D20_ASTART            0x04        /* sign for fill_attr      */
					  /* first attributes        */
#define D20_AEND              0x08        /* sign for fill_attr      */
					  /* last attributes         */


#define D20_UNKNOWN          (D20_oct *)NULL   /* value for 	     */
					  /* undefined object class  */
					/* type to be looked for      */
					/* (--> MODIFY_ALL_ATTRIBUTES)*/
#define D20_OBJECT		 0	  /* operation mode -> gen.  */
#define D20_ALIAS		 1	  /* object resp. alias name */
#define D20__ALIASED             2        /* aliased name            */
#define D20_RDN                  3        /* generate RDN            */
#define D20_NEW_OBJ          	 4        /* for new object name     */
					  /* in case of MOD_RDN	     */


#define DS_DEFAULT_VALUE        '_'       /* Fill-character '_'      */
#define DS_TREE_LENGTH          37      /* namepart-length            */

#define DS_SIZE_HELP_MASK       22      /* size of helpmasks          */

#define DS_PSAP_LEN             80      /* max length of psap address */

#define D20_NPDEL	      0x21	/* DN part delimiter '!' */

					/* constants for dsa_type value */
#define D20_0_DSA_TYP           0       /* 0 = no DSA-Type given        */
#define D20_1_DSA_TYP           1       /* 1 = DSA-Typ = default        */
#define D20_2_DSA_TYP           2       /* 2 = DSA-Typ = local          */
#define D20_3_DSA_TYP           3       /* 3 = DSA-Typ = default/local  */
#define D20_4_DSA_TYP           4       /* 4 = DSA-Typ = client         */

#define D20_NO_PSAP             2       /* return if no PSAP for DSA-   */
					/* Typ  Client                  */
					   /*   --- M A S K   1 ---   */
#define DS_LO_PASS              0
#define DS_LO_OPTIONS           1
#define DS_LO_BREAK             2
#define DS_LO_NAME              3

/*	Security mask.	*/
#define DS_SEC_DIR		0
#define DS_SEC_SECM		1

#define DS_SEC_SECM_POS		3
#define DS_SEC_OPT_POS		5

#define DS_L_DEFDSAOPT		0
#define DS_L_SPEDSAOPT		1
#define DS_L_DCACHEOPT		2
#define DS_L_NAMESTROPT		3

/* ---- positions and sizes of variable output fields within mask 1 --------- */

#define D20_1PPOS	 0
#define D20_11POS	 2
#define D20_1OPOS       26	
#define D20_11CNT	12	/* in units of 2 */

#define	D20_1_PW_FLD_LEN	32

					  /*   --- M A S K   2 ---   */
#define DS_RE_OPT		0
#define DS_RE_BREAK             1
#define DS_RE_NAME              2

#define DS_R_NONE		0
#define DS_R_NAMESTROPT		1
#define DS_R_END                2


 /* ---- positions and sizes of variable output fields within mask 2 --------- */

#define D20_1PSAP        3       /* naming field of the PSAP-Address  */
#define D20_2PSAP        4       /* first input field of PSAP-Address */
#define D20_3PSAP        5       /* second input field of PSAP-Address */
#define D20_OR           6       /* 'or' field after PSAP-Address      */


#define D20_21POS        3
#define D20_21CNT	12	/* in units of 2 */
#define D20_22POS       28      /* position of "OPTIONS"             */


					  /*   --- M A S K   3 ---   */
					  /*  operation codes        */
#define DS__STOP            0
#define DS__BASIC_ADM       1
#define DS__SCHEME_ADM      2
#define DS__DELTA_UPDATE    3
#define DS__HANDLE_TREE     4
#define DS__CACHE_UPDATE    5             /* index for cache update      */
					  /* not the same as on the mask */
#define DS__REM_LOGON       6             /* index for remote logon    */
#define DS__LOGON           7             /* index for logon           */
#define DS__UNBIND          9             /* only if rbatch == TRUE    */

#define DS_SCH_OP_IDX       4
#define DS_CA_OP_IDX        5
#define DS_OP_MINIDX        6
#define DS_OP_MAXIDX       10
					  /*  input field positions  */
#define DS_OP_OPERATION 0                 /* Operation-field         */
#define DS_OP_BREAK     1                 /* Break-field             */

					  /*  output field positions */
#define DS_OP_1FLD      0                 /* current dsa field1      */
#define DS_OP_2FLD      1                 /*             field2      */

#define D20_31POS       3                 /* index of "ADMIN-text"   */
					  /* in mask (either for     */
					  /* DSA or CACHE            */
					  /* administration          */


					  /*   --- M A S K   4 ---   */
#define DS_C_DISP               5
#define DS_C_MINIDX            11 
#define DS_C_MAXIDX            11
#define DS_E1_TYPE             12       /* position of "entry type"  */
#define DS_E2_TYPE             13       /* position of input field   */
					/* for "entry type"          */

					/*  sign for Operations ...   */
#define DS_EXIT                 0
#define DS_READ_PROPERTY        0       /*  only a pseudo operation   */
#define DS_ADD_OBJECT           1
#define DS_DELETE_OBJECT        2
#define DS_DISPLAY_OBJECTS      3
#define DS_DISP_OBJ             4
#define DS_SPEC_DISP_OBJ        4
#define DS_ADD_ATTRIBUTE        5
#define DS_DELETE_ATTRIBUTE     6
#define DS_MODIFY_ATTRIBUTE     7
#define DS_OPMIN                0
#define DS_ADD_CLIENT_ADDRESS   5
#define DS_DELETE_DEFAULT_DSA   7
#define DS_DISP_CLIENT_ADDRESS  6

#define DS_ADD_ALIAS            8
#define DS_MODRDN             	9 

#define D20_DELETE_DEFAULT_DSA 15

#define DS_OPMAX                9         /* incl. DS_EXIT = 0       */
#define DS_OP_CACHEMAX          8 
#define DS_DUMP                12         /* only for dump/restore   */
					  /*  input field positions  */
#define DS_AF_FUNCTION  0                 /* Operation-field         */
#define DS_AF_BREAK     1                 /* Break-field             */
					  /* ... of Adm-Funct.-Msk.  */
#define DS_AF_E_TYPE    2                 /* Entry type field        */


					  /*   --- M A S K   4 a --- */
#define DS_SPLOC                0
#define DS_SP1LOC               1
#define DS_SP1DEF               2
#define DS_SP11DEF              3
#define DS_SP2DEF               4
#define DS_SP21DEF              5
#define DS_SP3DEF               6
#define DS_SP31DEF              7
#define DS_SP4DEF               8
#define DS_SP41DEF              9
#define DS_SPBREAK             10
					  /*   --- M A S K   5 ---   */
					  /*  input field positions  */
#define DS_OC_CLASS     0                 /*  Object-class-field     */
#define DS_OC_MORE      1                 /*  paging field           */
#define DS_OC_BREAK     2                 /*  break field            */
					  /* ... of Obj.-class-Mask  */

/* ---- positions and sizes of variable output fields within mask 5 --------- */

#define D20_51POS	 0
#define D20_510POS       0      /* object name                        */
#define D20_511POS       1      /* alias name                         */
#define D20_512POS       2      /* aliased name                       */
#define D20_513POS      45      /* new object name                    */
#define D20_51CNT        3
#define D20_5ROOT_POS    4
#define D20_52POS        5
#define D20_52CNT	12	/* in units of 3 */

					  /*  input field positions  */
					  /*  (variable partition -->*/
					  /*  relative to D20_52POS) */
#define DS_OCNUMBER     0                 /* Obj.-class-Number-field */
#define DS_OCNAME       1                 /* Obj.-class-Name  -field */
#define DS_OCTREE       2                 /* Obj.-class-tree  -field */
					  /* ... of Obj.-class-pos.  */


					  /*   --- M A S K   5a ---  */
#define DS_DO_EXEC      0                 /*  Execute field          */
#define DS_DO_MORE      1                 /*  paging field           */
#define DS_DO_BREAK     2                 /*  break field            */

/* ---- positions and sizes of variable output fields within mask 5a --------- */

#define D20_5A1POS	 3
#define D20_5A1CNT	12	/* in units of 2 */


					  /*   --- M A S K   6 ---   */
					  /*  input field positions  */
#define DS_OB1          0                 /*                         */
#define DS_OB2          1                 /*                         */
#define DS_OB3          2                 /*                         */
#define DS_OB4          3                 /*                         */
#define DS_OB5          4                 /* Object name - parts     */
#define DS_OB6          5                 /*                         */
#define DS_OB7          6                 /*                         */
#define DS_OB8          7                 /*                         */
#define DS_OB9          8                 /*                         */
#define DS_OB10         9                 /*                         */
#define DS_OB11        10                 /*                         */
#define DS_OB12        11                 /*                         */
#define DS_OB_EXEC     13                 /*  Execute field          */
#define DS_OB_MORE     14                 /*  paging (more) field    */
#define DS_OB_BREAK    15                 /*  break field            */
#define DS_OB_OBJCLASS 17                 /*  object class           */
#define DS_OB_AUXOCLASS 18                /*  Auxiliary object class */

/* ---- positions and sizes of variable output fields within mask 6 --------- */

#define D20_60POS        2      /* position for object interpretation */
#define D20_61POS        4
#define D20_61CNT	 2
#define D20_610POS       4      /* object name                        */
#define D20_611POS       5      /* alias name                         */
#define D20_612POS       6      /* aliased name                       */
#define D20_613POS       7      /* new object name                    */
#define D20_62POS        8
#define D20_62CNT       12      /* in units of 2 */
#define D20_64POS       32      /* Object-Class */
#define D20_65POS       34      /* Auxiliary object-Class */

#define	D20_ONM_VAL_FLD_LEN	25	/* in object name masks length of */
					/* naming attribute value field.  */

#define	D20_ONM_NM_FLD_LEN	25	/* in object name masks length of */
					/* naming attribute name field.	  */

#define D20_ATVAL_LEN           90      /* length of the attribute value. */

#define	D20_ATVAL_FLD_LEN	(D20_ATVAL_LEN/2)	
					/* field length for an attribute  */
					/* value.			  */

					  /*   --- M A S K  6a ---   */
					  /*  input field positions  */
#define DS_AC1_MOD_PUB   0                /*  modify public          */
#define DS_AC2_REA_STA   1                /*  read standard          */
#define DS_AC3_MOD_STA   2                /*  modify standard        */
#define DS_AC4_REA_SEN   3                /*  read sensitive         */
#define DS_AC5_MOD_SEN   4                /*  modify sensitive       */
#define DS_AC6_BREAK     5                /*  break                  */

					  /*   --- M A S K  6b ---   */
					  /*  input field positions  */
#define DS_DN1_NAME      0                /*  1. DN                  */
#define DS_DN2_NAME      1                /*  2. part of 1. DN       */
#define DS_DN3_TYP       2                /*  typ of 1. DN           */
#define DS_DN4_NAME      3                /*  2. DN                  */
#define DS_DN5_NAME      4                /*  2. part of 2. DN       */
#define DS_DN6_TYP       5                /*  typ of 2. DN           */
#define DS_DN7_NAME      6                /*  3. DN                  */
#define DS_DN8_NAME      7                /*  2. part of 3. DN       */
#define DS_DN9_TYP       8                /*  typ of 3. DN           */
#define DS_DN10_NAME     9                /*  4. DN                  */
#define DS_DN11_NAME    10                /*  2. part of 4. DN       */
#define DS_DN12_TYP     11                /*  typ of 4. DN           */
#define DS_DN13_BREAK   13                /*  break                  */
#define DS_DN14_MORE    14                /*  SCROLL_DOWN            */
#define DS_DN16_SCU     16                /*  SCROLL_UP              */
#define DS_DN17_F1      17                /*  funktion key F1        */

/* ---- positions and sizes of variable output fields within mask 6b --------- */

#define D20_6A1POS      21                 /* SCROLL DOWN (only for   */
					   /* DISPLAY_OBJECT)         */
#define D20_6A2POS      2                  /* position of "OPCLASS"   */
#define D20_6A3POS      23                 /* SCROLL UP (only for     */
					   /* DISPLAY_OBJECT)         */
#define D20_6A4POS      24                 /* function key F1 (only   */
					   /* for DISPLAY_OBJECT)     */

#define D20_6A5POS	7		   /* position of 1 intrp.    */
#define D20_6A6POS	10		   /* position of 2 intrp.    */
#define D20_6A7POS	13		   /* position of 3 intrp.    */
#define D20_6A8POS	16		   /* position of 4 intrp.    */


					  /*   --- M A S K   7 ---   */
					  /* attribute name position */
					  /* in the mask array       */
#define D20_EA1_POS     3
#define D20_EA2_POS     8
#define D20_EA3_POS     13
					  /*  input field positions  */
					  /* in the array of input   */
					  /* fields		     */
#define DS_EA1_NAME     0                 /* attribute name          */
#define DS_EA1_1VAL     1                 /* value of the attribute  */
#define DS_EA1_2VAL     2                 /* value of the attribute  */
#define DS_EA2_NAME     3                 /* attribute name          */
#define DS_EA2_1VAL     4                 /* value of the attribute  */
#define DS_EA2_2VAL     5                 /* value of the attribute  */
#define DS_EA3_NAME     6                 /* attribute name          */
#define DS_EA3_1VAL     7                 /* value of the attribute  */
#define DS_EA3_2VAL     8                 /* value of the attribute  */
#define DS_EA_EXEC      9                 /*  Execute field          */
#define DS_EA_MORE     10                 /*  paging field           */
#define DS_EA_BREAK    11                 /*  break field            */
#define DS_EA_SCU      13                 /*  SCROLL_UP              */
#define DS_EA_F1       14                 /*  function key F1        */

/* ---- positions and sizes of variable output fields within mask 7 --------- */

#define D20_EA_SCU_POS  23 		   /* SCROLL UP (only for     */
					   /* DISPLAY_OBJECT)         */
#define D20_EA_F1_POS	24                 /* function key F1 (only   */
					   /* for DISPLAY_OBJECT)     */
#define D20_EA_SCD_POS 	20                 /* SCROLL DOWN (only for   */
					   /* DISPLAY_OBJECT)         */
#define D20_EA_MORE_POS 25                 /* "more" field (only used */
					   /* if rbatch = TRUE)       */


					  /*   --- M A S K   7a ---  */
#define DS_SP_PSEL      0                 /*  presentation selector  */
#define DS_SP_SSEL      1                 /*  session selector       */
#define DS_SP_TSEL      2                 /*  transport selector     */
#define DS_SP_NET1      3                 /*  NSAP-address 1         */
#define DS_SP_NET2      4                 /*  NSAP-address 2         */
#define DS_SP_NET3      5                 /*  NSAP-address 3         */
#define DS_SP_NET4      6                 /*  NSAP-address 4         */
#define DS_SP_NET5      7                 /*  NSAP-address 5         */
#define DS_SP_MORE      8                 /*  more field to finish   */
#define DS_SP_BREAK     9                 /*  break field            */
#define DS_SP_SCU      11                 /*  SCROLL_UP              */
#define DS_SP_F1       12                 /*  function key F1        */

/* ---- positions and sizes of variable output fields within mask 7a -------- */

#define D20_SP1POS      23                 /* SCROLL UP (only for     */
					   /* DISPLAY_OBJECT)         */
#define D20_SP2POS      24                 /* function key F1 (only   */
					   /* for DISPLAY_OBJECT)     */
#define D20_SP3POS      18                 /* SCROLL DOWN (only for   */
					   /* DISPLAY_OBJECT)         */

#define D20_7A_PS_FLD_LEN       35
#define D20_7A_SS_FLD_LEN       35
#define D20_7A_TS_FLD_LEN       23
#define D20_7A_NA_FLD_LEN       56

					  /*   --- M A S K   8 ---   */
					  /*  input field positions  */
#define DS_MA_NAME      0                 /*  name field             */
#define DS_MA1OLD_VAL   1                 /*  old value field 1      */
#define DS_MA2OLD_VAL   2                 /*  old value field 2      */
#define DS_MA1NEW_VAL   3                 /*  new value field 1      */
#define DS_MA2NEW_VAL   4                 /*  new value field 2      */
#define DS_MA_EXEC      5                 /*  execute field          */
#define DS_MA_BREAK     6                 /*  break field            */
#define DS_MA_MORE      7                 /*  more field             */
					  /*  ... of mod. attr.-mask */

/* ---- positions and sizes of variable output fields within mask 8 --------- */

#define D20_81POS        4                 /* index of attribute name */
					   /* in mask 8 definition    */
#define D20_82POS       14                 /* index of FKTSCD field   */

#define	D20_83POS	 5		   /* old value position in mask */


					  /*   --- M A S K   9 ---   */
#define DS__MIN_OP          0
#define DS__MAX_OP          14 
					  /*  operation codes        */
#define DS__EXIT            0
#define DS__STORE           1
#define DS__LOAD            2 
#define DS__SRT_DISPLAY     3
#define DS__A_SRULE         4
#define DS__D_SRULE         5
#define DS__M_SRULE         6
#define DS__OCT_DISPLAY     7 
#define DS__A_OBJ           8 
#define DS__D_OBJ           9
#define DS__M_OBJ          10 
#define DS__AT_DISPLAY     11 
#define DS__A_ATTR         12 
#define DS__D_ATTR         13 
#define DS__M_ATTR	   14

					  /*  input field positions  */
#define DS_SCH_OP         0               /*  operation              */
#define DS_SCH_BREAK      1               /*  break                  */
					  /*  ... of scheme operation*/
					  /*      mask               */

/* ---- positions and sizes of variable output fields within mask 10 --------- */

#define D20_O_ABBRV_POS         2   	/* index of object class abbrv.	*/
#define D20_O_NAME_POS	  	4	/* index of object class name	*/
#define D20_O_SUP_POS		6	/* index of sup. obj. classes   */
#define D20_O_OBJID_POS		9	/* index of object id.		*/
#define D20_O_CKIND_POS		11	/* index of object class kind	*/
#define D20_O_FNO_POS		13	/* index of file number		*/
#define D20_O_AUX_POS		15	/* index of aux. object classes */
#define D20_O_MAND_POS         	18      /* index of mandatory attr.	*/ 
#define D20_O_OPT_POS           21 	/* index of optional attr.  	*/ 
#define D20_O_SCU_POS	  	29   	/* index of scroll up	*/
#define D20_O_F1_POS	 	35      /* index of F1 in the mask. */
 

#define D2_SUPF_LEN	32	    /* length of first sup. obj. class 	 */
				    /* field (including EOS) in OCT mask.*/	
#define D2_SUPL_LEN	8	    /* length of last sup. obj. class 	 */
				    /* field (including EOS) in OCT mask.*/	
#define D2_AUXF_LEN	32	    /* length of first aux. obj. class 	 */
				    /* field (including EOS) in OCT mask.*/	
#define D2_AUXL_LEN	8	    /* length of last aux. obj. class 	 */
				    /* field (including EOS) in OCT mask.*/	
#define D2_MANDF_LEN	32	    /* length of first mand. attr. field */
				    /* (including EOS) in OCT mask       */
#define D2_MANDL_LEN	8	    /* length of sec. mand. attr. field  */
				    /* (including EOS) in OCT mask       */
#define D2_OPTF_LEN	32	    /* length of first optl. attr. field */
				    /* (including EOS) in OCT mask       */
#define D2_OPTL_LEN     8	    /* length of last optl. attr. field  */
				    /* (including EOS) in OCT mask       */

#define D2_SUP_CNT	 2	    /* no. of fields for super classes   */
#define D2_AUX_CNT	 2	    /* no. of fields for aux. classes	 */
#define D2_MAND_CNT	 2	    /* no. of fields for mand. attributes*/
#define D2_OPT_CNT	 7  	    /* no. of fields for optl.           */
				    /* attributes		         */

					  /*   --- M A S K  10 ---   */
					  /*  input field positions  */
#define DS_O_ABBRV        0               /*  object class abbrev    */
#define DS_O_NAME         1               /*  object class name      */
#define DS_O_SUP          2               /*  superior obj. class.   */
					  /*  abbreviations.	     */
#define DS_O_OBJID        4               /*  object identifier of   */
					  /*  the object class       */
#define DS_O_CKIND	  5
#define DS_O_FNO	  6               /*  file no.		     */
#define DS_O_AUX	  7
#define DS_O_MAND         9		  /*  first field for mand.  */
					  /*  attr.		     */
#define DS_O_OPT	  11		  /*  first field for optional*/
					  /*  attr.		     */
#define DS_O_UP           18              /*  scroll up              */
#define DS_O_DOWN         19              /*  scroll down            */
#define DS_O_BREAK        20              /*  break                  */
#define DS_O_EXEC         21              /*  execute                */
					  /*  ... of OCT scheme mask */


/*---- positions and sizes of variable output fields within mask 11 --------- */

#define D20_B1POS         2                /* index of attribute abbrv */
#define D20_B2POS         4                /* index of attribute name  */
#define D20_B7POS        14                /* index of attribute syntax */
#define D20_B11POS       22                /* index of scroll up      */
#define D20_BF1POS	 28		   /* index of F1 in the mask. */

#define D20_B1CNT         9                /* in units of 2           */
#define D20_B2CNT         1                /* in units of 2           */
#define D20_B3CNT        10                /* in units of 2           */

					  /*   --- M A S K  11 ---   */
					  /*  input field positions  */
#define DS_A_ABBRV    	  0               /*  attribute abbreviation */
#define DS_A_NAME         1               /*  attribute name         */
#define DS_A_OBJ_ID       2               /*  object identifier      */
#define DS_A_LOWER        3               /*  lower bound on attr.   */
				          /*  value length	     */
#define DS_A_UPPER        4               /*  upper bound on attr.   */
					  /*  value length	     */
#define DS_A_NREC         5               /*  valid no. of recurring */
					  /*  values 		     */
#define DS_A_SYNTAX       6               /*  attribute syntax       */
#define DS_A_ACL          7               /*  ACL class              */
#define DS_A_INDEX        8               /*  inverted priority      */
#define DS_A_PHON         9               /*  phonetic matching      */
#define DS_A_UP          10               /*  scroll up              */
#define DS_A_DOWN        11               /*  scroll down            */
#define DS_A_BREAK       12               /*  break                  */
#define DS_A_EXEC        13               /*  execute                */
					  /*  ... of ADT scheme mask */

/*---- positions and sizes of variable output fields within mask 12 --------- */
#define D20_S_RULE_POS         2                /* index of rule no 	      */
#define D20_S_SUP_POS          4                /* index of superior rule no */
#define D20_S_NAM_POS	       6		/* index of naming attributes*/
						/* field.		     */
#define D20_S_OCLASS_POS       8                /* index of Object calss */
#define D20_S_SCU_POS	      10	        /* index of scroll up      */
#define D20_S_F1_POS	      16	        /* index of F1		   */

#define D20_S_FIELD_CNT        4                /* in units of 2           */
 
#define D2_NAM_LEN		12	  /* length of naming attrs. field */
					  /* (including EOS) in SRT mask.  */

					  /*   --- M A S K  12 ---   */
					  /*  input field positions  */
#define DS_S_RULE_NO		0	  /* index of rule no.	     */
#define DS_S_PAR_RULE_NO	1	  /* index of par rule no.   */
#define DS_S_NAM_ATTR_ABBRV	2 	  /* index of nam. attr. abbrv*/	
#define DS_S_OCLASS		3	  /* index of object class   */
#define DS_S_UP           4               /*  scroll up              */
#define DS_S_DOWN         5               /*  scroll down            */
#define DS_S_BREAK        6               /*  break                  */
#define DS_S_EXEC         7               /*  execute                */

				    /* D E L T A  U P D A T E S       */
#define DS__END             0
#define DS__CREATE_SHD      1
#define DS__CRE_JOB         2
#define DS__REMOVE_SHD      3
#define DS__REM_JOB         4
#define DS__UPD_SHD_JOB     5
#define DS__DISP_SHD_JOB    6
#define DS__DISP_UPD_ERR    7
#define DS__UPD_ERR         8


					    /* M A S K E  13          */
#define DS_D_OP            13
#define DS_D_BREAK         14

					    /* M A S K E  14.1        */
#define DS_E1_OBJ_NAME     3
#define DS_E1_OBJ_INTRP    6
#define DS_E1_DSA          8
#define DS_E1_JOB         11
#define DS_E1_BREAK       12 
#define DS_E1_CR_MENU     13 
#define DS_E1_FORWARD     16 

					    /* M A S K E  14.2        */
#define DS_E2_UPD_FREQ     1
#define DS_E2_BREAK        2

					    /* M A S K E  14.3        */
#define DS_E3_OBJ_NAME     3
#define DS_E3_DSA          6
#define DS_E3_OP           9
#define DS_E3_ERR_TIME    11
#define DS_E3_ERR_COUNT   13
#define DS_E3_ERR_CODE    15
#define DS_E3_UPD_ERR     17

					    /* M A S K E  14.A        */
#define DS_EA_UPD_TIME     2
#define DS_EA_EMPTY_LN     3
#define DS_EA_TBREAK       7
#define DS_EA_CR_MENU      8 
#define DS_EA_FORWARD     11 

					    /* M A S K E  14.B        */
#define DS_EB_HOURS        2
#define DS_EB_BREAK        6
#define DS_EB_CR_MENU      7 
#define DS_EB_FORWARD     10 
					    /* M A S K E  14.C        */
#define DS_EC_DOW          3
#define DS_EC_HOURS        5
#define DS_EC_BREAK        8
#define DS_EC_CR_MENU      9 
#define DS_EC_FORWARD      12 
					    /* M A S K E  14.D        */
#define DS_ED_DOW1         5
#define DS_ED_HOURS1       7
#define DS_ED_DOW2         9
#define DS_ED_HOURS2      11 
#define DS_ED_BREAK       12 
#define DS_ED_CR_MENU     13 
#define DS_ED_FORWARD     16 


				    /* function identiers in mask 16  */
#define DS__SAVE_TREE       1
#define DS__APPND_TREE      2
#define DS__COPY_TREE       3
#define DS__MOVE_TREE       4
#define DS__DEL_TREE        5
#define DS__CHNG_MASTER     6
#define DS__CHNG_ALL_ATTR   7


				    /* M A S K  16                    */
#define DS_10_OP            12
#define DS_10_BREAK         13


				/* constants for mask 11a             */
#define D20_M_DSA           0       /* MASTER DSA                     */
#define D20_B_DSA           1       /* BIND DSA                       */
#define D20_S_DSA           2       /* SPECIFIC DSA                   */


				    /* M A S K  17a                   */
#define DS_11A_DSA          3
#define DS_11A_1FILE        5
#define DS_11A_2FILE        6
#define DS_11A_BREAK        7


#define D20_SINK_DSA        0       /* SINK DSA                       */
#define D20_ORIG_DSA        1       /* original MASTER DSA            */


				    /* M A S K  17b                   */
#define DS_11B_1FILE        3
#define DS_11B_2FILE        4
#define DS_11B_OV_WR        6
#define DS_11B_ACL          8
#define DS_11B_DSA          10
#define DS_11B_BREAK        11

#define D20_FN_FLD_LEN 		37	/* file name field length.	*/

				    /* M A S K  18                    */
#define DS_12_NO_FKT           8
#define DS_12_USE	     0	    /* field containg type of list    */
				    /* object list / aux. object class*/
				    /* list.			      */
#define DS_12_F1_POS	     21	    /* position of F1 key in mask.    */
#define DS_12_F8_POS         27     /* position of F8 key in mask.    */

#define DS_12_FIRST          3      /* first data field               */

#define D20_12_OBJ_LIST	     	0   /* to select object list heading  */
				    /* in 12omask.		      */
#define D20_12_AUX_OCLASS_LIST	1   /* to select aux. object class list */
				    /* heading in 12omask.	      */
#define D20_12_ATTR_LIST	2   /* to select attributes           */	
				    /* heading in 12omask.	      */
#define D20_12_AT_LIST	        3   /* Summary of AT entries.	      */
				    /* heading in 12omask.	      */
#define D20_12_OCT_LIST	        4   /* Summary of OCT entries.	      */
				    /* heading in 12omask.	      */
#define D20_12_SRT_LIST	        5   /* Summary of SRT entries.	      */
				    /* heading in 12omask.	      */

#define D20_MAX_DISP_STR_LEN 75     /* maximal length of string to be */
				    /* displayed in summary list      */

				/* M A S K 18b			*/
#define	D20_12B_NO_ITEMS	5	
#define	D20_12B_START_POS	2	

				    /* M A S K  19                    */
#define DS_13_HELP            5
#define D20_13_NO_FIELDS     12     /* number of RDNs in d20_13omask  */
				    /* positions:                     */
#define DS_13_FIRST          4      /* first data field               */

				    /* position of function fields in */
				    /* mask 19                        */
#define DS_13_F1             28
#define DS_13_DOWN           30
#define DS_13_UP             31


				    /* M A S K  20                    */
				    /* positions:                     */
#define DS_14_NO_FIELDS       16    /* number of fields               */
#define DS_14_FIRST           2     /* first data field               */
#define DS_14_LAST            21    /* last data field                */




#define D20_Q1POS        2                  /* QUIT-mask              */
#define D20_M1POS        1                  /* Message-mask           */

#define	D20_ERR_MSG_LEN		(6 + 67)   /* Error: + error message length. */
#define	D20_CONT_MSG_LEN	24         /* Continue message length.       */

#define D20_MASK5        5
#define D20_MASK6        6
#define D20_MASK13       13

				    /* M A S K  18                    */
#define D20_EH_NOVWR    0       /* Not overwritten objects            */
#define D20_EH_OVWR     1       /* Overwritten  objects               */
#define D20_EH_NCHNG    2       /* No change    possible              */
#define D20_EH_NDEL     3       /* No delete    possible              */

#define	D20_T61_CONV_FACT	2	/* conversion factor to convert local */
					/* string to T61 chars. i.e max. no.  */
					/* T61 chars that can be generated for*/
					/* a local char.		      */	


#define DS_NP_VAL_LEN          	(D20_ONM_VAL_FLD_LEN * D20_T61_CONV_FACT)
#define DS_NP_NM_LEN		(D20_ONM_NM_FLD_LEN)


/* -------------------- error codes (Indices) --------------------------- */

#define D20_E00IDX	 0
#define D20_E01IDX	 1
#define D20_E02IDX	 2
#define D20_E03IDX	 3
#define D20_E04IDX	 4
#define D20_E05IDX	 5
#define D20_E06IDX	 6
#define D20_E07IDX	 7
#define D20_E08IDX	 8
#define D20_E09IDX	 9
#define D20_E10IDX	10
#define D20_E11IDX	11
#define D20_E12IDX	12
#define D20_E13IDX	13
#define D20_E14IDX	14
#define D20_E15IDX	15
#define D20_E16IDX	16
#define D20_E17IDX	17
#define D20_E18IDX	18
#define D20_E19IDX	19
#define D20_E20IDX	20
#define D20_E21IDX	21
#define D20_E22IDX	22
#define D20_E23IDX	23
#define D20_E24IDX	24
#define D20_E25IDX	25
#define D20_E26IDX	26
#define D20_E27IDX	27
#define D20_E28IDX      28
#define D20_E29IDX      29
#define D20_E30IDX      30
#define D20_E31IDX      31
#define D20_E32IDX      32
#define D20_E33IDX      33
#define D20_E34IDX      34
#define D20_E35IDX      35
#define D20_E36IDX      36
#define D20_E37IDX      37
#define D20_E38IDX      38
#define D20_E39IDX      39
#define D20_E40IDX      40
#define D20_E41IDX      41
#define D20_E42IDX      42
#define D20_E43IDX      43
#define D20_E44IDX      44
#define D20_E45IDX      45
#define D20_E46IDX      46
#define D20_E47IDX      47
#define D20_E48IDX      48
#define D20_E49IDX      49
#define D20_E50IDX      50
#define D20_E51IDX      51
#define D20_E52IDX      52
#define D20_E53IDX      53
#define D20_E54IDX      54
#define D20_E55IDX      55
#define D20_E56IDX      56
#define D20_E57IDX      57
#define D20_E58IDX      58
#define D20_E59IDX      59
#define D20_E60IDX      60
#define D20_E61IDX      61
#define D20_E62IDX      62
#define D20_E63IDX      63
#define D20_E64IDX      64
#define D20_E65IDX      65
#define D20_E66IDX      66
#define D20_E67IDX      67
#define D20_E68IDX      68
#define D20_E69IDX      69
#define D20_E70IDX      70
#define D20_E71IDX      71
#define D20_E72IDX      72
#define D20_E73IDX      73
#define D20_E74IDX      74
#define D20_E75IDX      75
#define D20_E76IDX      76
#define D20_E77IDX      77
#define D20_E78IDX      78
#define D20_E79IDX      79
#define D20_E80IDX      80
#define D20_E81IDX      81
#define D20_E82IDX      82
#define D20_E83IDX      83
#define D20_E84IDX      84
#define D20_E85IDX      85
#define D20_E86IDX      86
#define D20_E87IDX      87
#define D20_E88IDX      88
#define D20_E89IDX      89
#define D20_E90IDX      90
#define D20_E91IDX      91
#define D20_E92IDX      92
#define D20_E93IDX      93
#define D20_E94IDX      94
#define D20_E95IDX      95
#define D20_E96IDX      96
#define D20_E97IDX      97
#define D20_E98IDX      98
#define D20_E99IDX      99
#define D20_100IDX     100
#define D20_101IDX     101
#define D20_102IDX     102
#define D20_103IDX     103
#define D20_104IDX     104
#define D20_105IDX     105
#define D20_106IDX     106
#define D20_E105IDX     105
#define D20_E106IDX     106
#define D20_E107IDX     107
#define D20_E108IDX     108
#define D20_E109IDX     109
#define D20_E110IDX     110
#define D20_E111IDX     111
#define D20_E112IDX     112
#define D20_E113IDX     113
#define D20_E114IDX     114
#define D20_E115IDX     115
#define D20_E116IDX     116
#define D20_E117IDX     117
#define D20_E118IDX     118
#define D20_E119IDX     119
#define D20_E120IDX     120
#define D20_E121IDX	121
#define D20_E122IDX	122
#define D20_E123IDX	123
#define D20_E124IDX	124


/* ------------------------------------------------------------------------- */
#define D20_STSHORT(var,val)    var[0] = (unsigned char) (val) ; \
				var[1] = (unsigned char) (val >> 8);
#define D20_GTSHORT(var)       ((signed16) ((unsigned char) var[0] + \
					((unsigned char) var[1] << 8)))
/* ------------------------------------------------------------------------- */


/*----------------------maximum values          ----------------------*/
#define D20_382_NRFUN      5        /* nr of general functions             */
#define D20_383_NRLENATT   2        /* no. of requested attributes         */
				    /* "SCHEME"                            */


/*----------------------some other constants    ----------------------*/

#define D20_MAX_ERR_MESS        84      /* max. number of error messages  */
#define D20_MAX_ERR_TABLE	125
#define D20_MAX_ERR_TAB		107

#define	D20_MAXMSG		80	/* maximum lenght of message */
					/* (errormessage) */

#define D20_373_TIME_LIM   600

#ifdef TEST
	#define D20_CMFILE	"/adm/common.new"	/* scheme file name for common informations */
	#define D20_ADMSCH_FILE "/adm/admscheme.new" /* scheme file for administration */
#else
	#define D20_CMFILE	"/adm/common"	/* scheme file name for common informations */
	#define D20_ADMSCH_FILE "/adm/admscheme" /* scheme file for administration */
#endif

#define D20_SCHFILE     "/adm/newscheme"/* scheme file for update scheme operation */
#define D20_TSTOFILE    "/adm/tstfile"  /* test output file        */
#define D20_ADM         "/adm/"

					       /* path for config files    */
#define D20_COUNTRIES   "countries"            /* countries in ASCII-format*/


#define D20_SHFNMAX	    100		/* max. size of a scheme file name */
#define D20_FN_MAX          100         /* max. size of test file name  */

#define ALL_TIMES          99       /* all possible update times      */

#define ACTIVE          4
#define NOT_ACTIVE      5

/*	S C H E M A  F O R  A D M I N I S T R A T I O N 	*/

typedef struct {
	    At		at;
	    D2_obj_id   obj_id;
} D20_at;	/* ATTRIBUTE TABLE FOR ADMIN. */
	
typedef struct D20_oct {
	    Abbrv	 obj_abbrv;	/* abbreviation of object     */
				        /* class		      */	
	    D2_obj_id    obj_id;
					/* object identifier for      */
					/* object class               */
	    char         obj_name[L_MAX_NAME];
					/* object class name          */
	    signed32	 no_sup_class;	/* no. superior object class  */
					/* entries.		      */
	    struct D20_oct  **sup_class;  
					/* dpointer to superior object*/
					/* classes                    */
	    signed32	 no_aux_class;	/* no. of auxiliary object    */
					/* classes.		      */
	    struct D20_oct   *aux_class[NO_AUX_CLASS];   
					/* list of auxiliary object   */
					/* classes.		      */
	    signed32     no_must_attr;  /* number of mandatory attr.  */
					/* for object class and all   */
					/* it's superior classes      */
	    D20_at     **must_attr;     /* mandatory attributes       */
	    signed32     no_may_attr;   /* number of optional attr.   */
					/* for object class and all   */
					/* it's superior classes      */
	    D20_at     **may_attr;      /* optional attributes        */
} D20_oct;              /* O C T  for Administration                  */


typedef struct D20_srt {
	    signed32        rule_nr;   /* rule number                */
	    signed32	    par_rule_nr; /*parent rule number.	      */
	    struct D20_srt  *par_rule;  /* pointer to parent rule     */
	    signed32	    no_nam_attr;/* no. of naming attributes.  */
	    D20_at          *nam_attr[NO_NAM_ATTR]; 
					/* naming attributes	      */
	    signed32        no_obj_class; /* number of object classes*/
	    D20_oct         **obj_class;
					/* list of object classes     */
} D20_srt;              /* S R T  for Administration                  */



typedef struct
	{  D2_pbhead         *d20_Spbhead;    /* -> pointer to head   */
	   char              *d20_Senv;       /* -> installation      */
					      /*    environment       */
	} D20_pS;        /* S C H E M E   A D M I N I S T R A T I O N */


typedef struct
	{  D2_pbhead         *d20_Epbhead;    /* -> pointer to head   */
	   D20_srt           *d20_Esrt;       /* -> pointer to SRT    */
	   signed32           d20_Ensrt;      /* -> number of Entries */
					      /*    in the SRT        */
	   D20_oct           *d20_Eoct;       /* -> pointer to OCT    */
	   signed32           d20_Enoct;      /* -> number of Entries */
					      /*    in the OCT        */
	   D20_at            *d20_Eat;        /* -> pointer to AT     */
	   signed32           d20_Enat;       /* -> number of Entries */
					      /*    in the AT         */
	   char              *d20_Eenv;       /* -> installation      */
					      /*    environment       */
	   signed16           d20_Edir_id;    /* -> directory id      */
	   signed16           d20_Ebid;       /* -> bind id           */
	} D20_pE;           /* T R E E  H A N D L I N G               */
			    /* B A S I C  A D M I N I S T R A T I O N */
			    /* C A C H E    U P D A T E               */
			    /* D E L T A    U P D A T E               */


typedef struct
	{ signed16   obj_length;          /* maximal allowed length  */
	  signed16   obj_real_len;        /* object length           */
	  char       *attr_type;          /* pointer to attribute    */
					  /* type of the             */
					  /* naming attribute        */
	  D2_a_type  attr_id;		  /* attr. id of the naming  */
					  /* attribute		     */
	  char       *obj_name;           /* pointer to object name  */
	  unsigned16  rep;		  /* representation of the   */
					  /* naming attribute.	     */
	} Ds_obj_entry;


typedef struct
	{ D2_a_type   attr_id;            /* attribute type          */
	  Bool        hex_use;            /* hexa input or not       */
	  signed16    mod_type;           /* type of modification    */
	  D2_a_value  a_val;              /* attribute value         */
	} Ds_at_entry;


typedef	struct
	{    signed16      ds_length;   /* Property length            */
	     Octet_string  ds_value;	/* Property value             */
	} Ds_property;
 
#define	NAM_ATTR_OFFSET	3	/* offset of naming attributes (>1)	*/
				/* in mask 5omask (SRT tree display)	*/

typedef struct
     {  char     ds_numb[3];              /* rule number             */
	signed32 ds_no_nam_attr;	  /* no. of naming attributes.*/
	char     ds_nam_attr[NO_NAM_ATTR][DS_NP_NM_LEN + NAM_ATTR_OFFSET + 1];
					  /* array of naming attributes */
	char    ds_tree[DS_TREE_LENGTH];  /* tree structure for entry*/
     } Ds_empty_pos;

#define ADM_SCHEME 0
#define DSA_SCHEME 1

/* format definitions to store admin. scheme into a file */
#define  ADMAT_HEAD		"No_At: %d \n"
#define	 ADMAT_FORMAT		"Abbrv: %s Obj_Id: %s Name: %s Lower-B: %d Upper-B: %d No-Rec: %d Syntax: %d ACL: %d Ind-Lev: %d Phon: %d \n"

#define	 ADMOCT_HEAD		"No-Oct: %d No-Attr-Abbrv: %d No-Sup-Class-Abbrv: %d \n"

#define	 ADMOCT_FORMAT		"Abbrv: %s Obj-id: %s Name: %s No-Scls: %d No-Acls: %d No-Mand: %d No-Opt: %d "

#define	 ADMSRT_HEAD		"No-Srt: %d No-Obj-Class-Abbrv: %d \n"
#define	 ADMSRT_FORMAT		"Rule-No: %d Par-Rule-No: %d No-Nam-Att: %d No-Obj-Cls: %d " 

#define  D2_LC_SHORT        5  /* max. nr of positions of short in IA5*/
#define  D2_LC_LONG         10 /* max. nr of positions of long  in IA5*/

#define  D2_SCH_FIRST		0
#define  D2_SCH_NEXT		1
#define  D2_SCH_PREVIOUS	2

typedef struct
	{ D2_pbhead        *d20_6pbhead;    /* -> pointer standard-   */
					    /*    header              */
	  signed16     	    d20_6disp;      /* -> operation to be     */
					    /*    performed           */
	  signed32 	   d20_6nentries;   /* no. of entries involved in */
					    /* display.			  */
	  signed32	  *d20_6first;	    /* pointer to list of indices of */
					    /* selected entries for display. */
	  signed32	  *d20_6act;	    /* pointer to index of current */
					    /* selected entry for display. */
	} D20_p6;           /* D I S P L A Y   S C H E M E            */


			/* error values                               */
#define D2_E_SCH        -100        /* end of schema table reached    */
#define D2_B_SCH        -101        /* beginning of schema table reached   */
#define D2_NO_ELEM      -102        /* No element 		      */
#define D2_ILL_OP	-103        /* Illegal option 		      */


	/* definitions valid for MASK 12 */
#define D2_OCLASS_LEN   20          /* length of object class field      */
				    /* (including EOS) in SRT mask       */
#define D2_OCLASS_CNT	 2  	    /* no. of fields for Object classes  */
				    /* in SRT mask.			 */
				    /* attributes		         */

#define EVERY_HOUR 	1
#define EVERY_5_MIN	5
#define ZERO_HOUR	0
#define SUNDAY		0

#define D2_EXTERN_OBJ   0
#define D2_PARENT_OBJ   1
#define D2_REAL_OBJ     2

#define D2_ADM_RDN_SEP		'/'	/* rdn seperator in DN at the admin. */
					/* interface.			     */

#define D2_ADM_AVA_SEP		','	/* AVA seperator in DN at the admin. */
					/* interface.			     */

#define D2_ADM_TYPE_VAL_SEP	'='	/* Type and value of one of the AVAS */
					/* seperator in DN at the admin.     */
					/* interface.			     */

#define D20_BLANK		' '

#define	D20_TOO_MANY_ACR	1	/* too many acronyms at schema admin. */
					/* interface.			      */

#define	D20_TOO_BIG_ACR		2	/* too big acronym at schema admin.   */
					/* interface.			      */

#endif /* _D2ADM_H */
