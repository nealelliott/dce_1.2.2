/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stdlparams.c,v $
 * Revision 1.1.2.2  1996/03/11  14:09:23  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:28  marty]
 *
 * Revision 1.1.2.1  1995/12/13  17:24:57  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:08 UTC  dat
 * 	New file for second XIDL drop for DCE 1.2.1
 * 	[1995/12/13  17:24:00  root]
 * 
 * Revision 1.1.2.1  1995/11/01  14:22:27  bfc
 * 	idl cleanup
 * 	[1995/11/01  14:21:25  bfc]
 * 
 * $EndLog$
 */
/******************************************************************************/
/* Created 31-MAY-1994 15:05:28.34  by DDIS Parse-Table Generator V1.0        */
/* Source: 31-MAY-1994 14:51:41.00  $MY_ROOT:[BLD.V1_1.LIBIDL.SRC]STDLPARAMS.DDIS;2  */
/******************************************************************************/

#ifdef vms
globaldef unsigned char stdlparams[] = {
#else
unsigned char stdlparams[] = {
#endif
/******************************************************************************/
/* Parse Table Header                                                         */
/******************************************************************************/
	255,   1,   0,   0,		/* Ident                            */
	 20,   0,   0,   0,		/* TABLE_BEGIN-stdlparams                       */
	236,   0,   0,   0,		/* ENTRY_BEGIN-stdlparams                       */
	244,   0,   0,   0,		/* VALUE_BEGIN-stdlparams                       */
	244,   0,   0,   0,		/* TABLE_END-stdlparams                       */
/******************************************************************************/
/* Parse Table Entries                                                        */
/******************************************************************************/
	 16,   0,   0,   0,	/* 0 K_T_Stdl_Params [UNIVERSAL 16]   */
	  0,			/*                          */
	  1,			/* SEQUENCE required        */
	  2,   0,		/*   type offset to 2       */
	  0,   0,		/* No default value         */
	  0,   0,		/*                          */

	  0,   0,   0,   0,	/* 1 DDIS$K_T_EOC                     */
	  0,			/*                          */
	  0,			/* SEQUENCE end             */
	  0,   0,		/*                          */
	  0,   0,		/*                          */
	  0,   0,		/*                          */

	128,   0,   0,   0,	/* 2 K_T_octet [CONTEXT 0]            */
	136,			/* OCTET STRING             */
	  5,			/* SEQUENCE OF              */
	  0,   0,		/*                          */
	  0,   0,		/* No default value         */
	  0,   0,		/*                          */

	129,   0,   0,   0,	/* 3 K_T_integer [CONTEXT 1]          */
	  6,			/* INTEGER                  */
	  5,			/* SEQUENCE OF              */
	  0,   0,		/*                          */
	  0,   0,		/* No default value         */
	  0,   0,		/*                          */

	130,   0,   0,   0,	/* 4 K_T_latin [CONTEXT 2]            */
	136,			/* OCTET STRING             */
	  5,			/* SEQUENCE OF              */
	  0,   0,		/*                          */
	  0,   0,		/* No default value         */
	  0,   0,		/*                          */

	131,   0,   0,   0,	/* 5 K_T_katakana [CONTEXT 3]         */
	136,			/* OCTET STRING             */
	  5,			/* SEQUENCE OF              */
	  0,   0,		/*                          */
	  0,   0,		/* No default value         */
	  0,   0,		/*                          */

	132,   0,   0,   0,	/* 6 K_T_kanji [CONTEXT 4]            */
	136,			/* OCTET STRING             */
	  5,			/* SEQUENCE OF              */
	  0,   0,		/*                          */
	  0,   0,		/* No default value         */
	  0,   0,		/*                          */

	133,   0,   0,   0,	/* 7 K_T_decimal_string [CONTEXT 5]   */
	136,			/* OCTET STRING             */
	  5,			/* SEQUENCE OF              */
	  0,   0,		/*                          */
	  0,   0,		/* No default value         */
	  0,   0,		/*                          */

	134,   0,   0,   0,	/* 8 K_T_octet_array [CONTEXT 6]      */
	136,			/* OCTET STRING             */
	  5,			/* SEQUENCE OF              */
	  0,   0,		/*                          */
	  0,   0,		/* No default value         */
	  0,   0,		/*                          */

	135,   0,   0,   0,	/* 9 K_T_stdl_array_type [CONTEXT 7]  */
	  0,			/*                          */
	  5,			/* SEQUENCE OF              */
	249, 255,		/*   type offset to 2       */
	  0,   0,		/* No default value         */
	  0,   0,		/*                          */

	136,   0,   0,   0,	/* 10 K_T_stdl_record_type [CONTEXT 8] */
	  0,			/*                          */
	  5,			/* SEQUENCE OF              */
	248, 255,		/*   type offset to 2       */
	  0,   0,		/* No default value         */
	  0,   0,		/*                          */

	159, 100,   0,   0,	/* 11 K_T_tx_rpc_context [CONTEXT 100] */
	  0,			/*                          */
	  5,			/* SEQUENCE OF              */
	  2,   0,		/*   type offset to 13      */
	  0,   0,		/* No default value         */
	  0,   0,		/*                          */

	  0,   0,   0,   0,	/* 12 DDIS$K_T_EOC                     */
	  0,			/*                          */
	  0,			/* SEQUENCE end             */
	  0,   0,		/*                          */
	  0,   0,		/*                          */
	  0,   0,		/*                          */

	128,   0,   0,   0,	/* 13 K_T_tx_ctx [CONTEXT 0]           */
	  6,			/* INTEGER                  */
	  1,			/* SEQUENCE required        */
	  0,   0,		/*                          */
	  0,   0,		/* No default value         */
	  0,   0,		/*                          */

	129,   0,   0,   0,	/* 14 K_T_xopen_ctx [CONTEXT 1]        */
	  0,			/*                          */
	  1,			/* SEQUENCE required        */
	  2,   0,		/*   type offset to 16      */
	  0,   0,		/* No default value         */
	  0,   0,		/*                          */

	  0,   0,   0,   0,	/* 15 DDIS$K_T_EOC                     */
	  0,			/*                          */
	  0,			/* SEQUENCE end             */
	  0,   0,		/*                          */
	  0,   0,		/*                          */
	  0,   0,		/*                          */

	128,   0,   0,   0,	/* 16 K_T_rollback_indicator [CONTEXT 0] */
	  6,			/* INTEGER                  */
	  1,			/* SEQUENCE required        */
	  0,   0,		/*                          */
	  0,   0,		/* No default value         */
	  0,   0,		/*                          */

	  0,   0,   0,   0,	/* 17 DDIS$K_T_EOC                     */
	  0,			/*                          */
	  0,			/* SEQUENCE end             */
	  0,   0,		/*                          */
	  0,   0,		/*                          */
	  0,   0,		/*                          */

/******************************************************************************/
/* Entry Vectors                                                              */
/******************************************************************************/

	  1,   0,   0,   0,		/* Entry vector count               */
	  0,   0,   0,   0,		/* Entry point - index 0 */

/******************************************************************************/
/* Value Vectors                                                              */
/******************************************************************************/
/******************************************************************************/
/* Value Table Entries                                                        */
/******************************************************************************/
/******************************************************************************/
/* Parse Table End                                                            */
/******************************************************************************/
   };
