/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: gsu.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:47  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:02:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:30  root]
 * 
 * Revision 1.1.2.3  1994/08/19  13:54:14  marrek
 * 	Improve testcode coverage OT11586.
 * 	[1994/08/19  11:45:23  marrek]
 * 
 * Revision 1.1.2.2  1994/08/11  13:05:36  marrek
 * 	Fix errors for interactive mode (ot11585).
 * 	[1994/08/10  13:06:51  marrek]
 * 
 * Revision 1.1.2.1  1994/06/21  14:39:41  marrek
 * 	Bug fix for June 1994 code submission.
 * 	[1994/06/16  15:45:12  marrek]
 * 
 * 	Created in June 94 code submission.
 * 	[1994/06/10  16:04:27  marrek]
 * 
 * $EndLog$
 */

#ifndef _GSU_H
#define _GSU_H

/**********************************************************************/
/*                                                                    */
/*  Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG         */
/*  All Rights Reserved                                               */
/*                                                                    */
/*  Project          : GDU - GDS Attribute Info Module                */
/*  File Name        : gsu.h                                          */
/*  Description      : This file contains all the data types and      */
/*		       defines used by the gdssetup modules.	      */
/*  Date             : Oktober 1993                                   */
/*  Author           : Eva Wilde, SNI BU BA KP13                      */
/*                                                                    */
/**********************************************************************/

#if !defined lint && defined GDS_RCS_ID
static char gsu_rcsid[] = "@(#)$RCSfile: gsu.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:18 $";
#endif

/***************************************************************************/
/*          Constants for configuration and initialisation                 */
/***************************************************************************/
#define GSU_DIR		 "/var/adm/directory/gds/adm/"
#define GSU_FILE	 "gsu_setup_file"
#define GSU_BLSIZE       100   /* size of blocks to restore of data        */
#define GSU_CONFIG         2   /* lengh of config-variable                 */
#define GSU_PSAP          85   /* lengh of all PSAP-address                */
#define GSU_DSA           90   /* lengh of all DSAs                        */
#define GSU_LOCALS         5   /* (y/n)lengh of variable local schema      */
#define GSU_DEFSCH         5   /* (y/n)lengh of variable default schema    */
#define GSU_LOC_DEF    	   5   /* (y/n)lengh of variable local=default     */
#define GSU_USERPSW      128   /* lengh of DSA password                    */
#define GSU_SECUR         15   /* lengh of variable security               */
#define GSU_PRINC         30   /* lengh of principal name                  */
#define GSU_ID            20   /* max. dir-id                              */
#define GSU_SERVER       256   /* max. no-of-server                        */
#define GSU_CLIENT       256   /* max. no-of-client                        */
#define GSU_ANZIN	  12   /* in steps.c max. Number of in_vect        */
#define GSU_LINE	  90   /* max. lengh of line for readline          */
#define GSU_MAXANZDSA	  10   /* max. no-of-dsas                          */
#define GSU_INPUT	  90   /* for history in readline                  */
#define GSU_LENINT	   5   /* lengh of GSU_SERVER or GSU_CLIENT        */
#define GSU_MAX_LANG	  20   /* length of language-dependent yes or not  */
#define GSU_COUNTRY	   6   /* length of country from local DSA         */

/***************************************************************************/
/*               Authentication mechanism constants                        */
/***************************************************************************/
#define GSU_ANONYMOUS	   0   /* Authentication mechanism constants       */
#define GSU_SIMPLE	   2
#define GSU_DCE_AUTH	   5
#define GSU_SIMPLE_DCE	   25
/***************************************************************************/

/***************************************************************************/
/*                Constants for end-mechanism                              */
/***************************************************************************/
#define GSU_EXIT_0	   0
#define GSU_EXIT_1	   1
#define GSU_EXIT_2	   2
#define GSU_EXIT_3	   3
#define GSU_EXIT_4	   4
#define GSU_EXIT_5	   5
#define GSU_EXIT_6	   6
#define GSU_EXIT_7	   7
#define GSU_EXIT_8	   8
#define GSU_EXIT_9	   9
#define GSU_EXIT_10	  10
#define GSU_EXIT_11	  11
#define GSU_EXIT_12	  12
#define GSU_EXIT_13	  13 
#define GSU_EXIT_14	  14 
#define GSU_EXIT_15	  15
#define GSU_EXIT_16	  16
#define GSU_EXIT_17	  17
#define GSU_EXIT_18	  18
#define GSU_EXIT_19	  19
#define GSU_EXIT_20	  20
#define GSU_EXIT_21	  21
#define GSU_EXIT_22	  22
#define GSU_EXIT_23	  23
#define GSU_EXIT_24	  24
#define GSU_EXIT_25	  25
#define GSU_EXIT_26	  26
#define GSU_EXIT_27	  27
#define GSU_EXIT_28	  28
#define GSU_EXIT_29	  29
#define GSU_EXIT_30	  30
#define GSU_EXIT_31	  31
#define GSU_EXIT_32	  32
#define GSU_EXIT_33	  33
#define GSU_EXIT_34	  34
#define GSU_EXIT_35	  35
#define GSU_EXIT_36	  36
#define GSU_EXIT_37	  37
#define GSU_EXIT_38	  38
#define GSU_EXIT_39	  39
#define GSU_EXIT_40	  40
#define GSU_EXIT_41	  41
#define GSU_EXIT_42	  42
#define GSU_EXIT_43	  43
#define GSU_EXIT_44	  44
#define GSU_EXIT_45	  45
#define GSU_EXIT_46	  46
#define GSU_EXIT_47	  47
#define GSU_EXIT_48	  48
#define GSU_EXIT_49	  49
#define GSU_EXIT_50	  50
#define GSU_EXIT_51	  51
#define GSU_EXIT_52	  52
#define GSU_EXIT_53	  53
#define GSU_EXIT_54	  54
#define GSU_EXIT_55	  55
#define GSU_EXIT_56	  56
#define GSU_EXIT_57	  57
#define GSU_EXIT_58	  58
#define GSU_EXIT_59	  59
#define GSU_EXIT_60	  60
#define GSU_EXIT_61	  61
#define GSU_EXIT_62	  62
#define GSU_EXIT_63	  63
#define GSU_EXIT_64	  64
#define GSU_EXIT_65	  65
#define GSU_EXIT_66	  66
#define GSU_EXIT_67	  67
#define GSU_EXIT_68	  68
#define GSU_EXIT_69	  69
#define GSU_EXIT_70	  70
#define GSU_EXIT_71	  71
#define GSU_EXIT_72	  72
#define GSU_EXIT_73	  73
#define GSU_EXIT_74	  74
#define GSU_EXIT_75	  75
#define GSU_EXIT_76	  76
#define GSU_EXIT_77	  77
#define GSU_EXIT_78	  78
#define GSU_EXIT_79	  79
#define GSU_EXIT_80	  80
#define GSU_EXIT_81	  81
#define GSU_EXIT_82	  82
#define GSU_EXIT_83	  83
#define GSU_EXIT_84	  84
#define GSU_EXIT_85	  85
#define GSU_EXIT_86	  86
#define GSU_EXIT_87	  87
#define GSU_EXIT_88	  88
#define GSU_EXIT_89	  89
#define GSU_EXIT_90	  90
#define GSU_EXIT_91	  91
#define GSU_EXIT_92	  92
#define GSU_EXIT_93	  93
#define GSU_EXIT_94	  94
#define GSU_EXIT_95	  95
#define GSU_EXIT_96	  96
#define GSU_EXIT_97	  97
#define GSU_EXIT_98	  98
#define GSU_EXIT_99	  99
#define GSU_EXIT_100	 100
#define GSU_EXIT_101	 101
#define GSU_EXIT_102	 102
#define GSU_EXIT_103	 103
#define GSU_EXIT_104	 104
#define GSU_EXIT_105	 105
#define GSU_EXIT_106	 106
#define GSU_EXIT_107	 107
#define GSU_EXIT_108	 108
#define GSU_EXIT_109	 109
#define GSU_EXIT_110	 110
#define GSU_EXIT_111	 111
#define GSU_EXIT_112	 112
#define GSU_EXIT_113	 113
#define GSU_EXIT_114	 114
#define GSU_EXIT_115	 115
#define GSU_EXIT_116	 116
#define GSU_EXIT_117	 117
#define GSU_EXIT_118	 118
#define GSU_EXIT_119	 119
#define GSU_EXIT_120	 120
#define GSU_EXIT_121	 121
#define GSU_EXIT_122	 122
#define GSU_EXIT_123	 123
#define GSU_EXIT_124	 124
#define GSU_EXIT_125	 125
#define GSU_EXIT_126	 126
#define GSU_EXIT_127	 127
#define GSU_EXIT_128	 128
#define GSU_EXIT_129	 129
#define GSU_EXIT_130	 130
/***************************************************************************/

extern int main(int, char **); 

/***************************************************************************/
/*   Structure for Additional-DSA's, Default-DSA's and Remote-DSA's        */
/***************************************************************************/
typedef struct Gsu_dsa
	        {
             	char dsaname[GSU_DSA + 1];
             	char psapadd[GSU_PSAP + 1];
             	struct Gsu_dsa * next;
             	} gsu_dsa;
/***************************************************************************/

#endif /* ifndef _GSU_H */
