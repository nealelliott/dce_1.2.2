/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: prspre.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:11  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:17:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:34  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:33  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:59:26  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:24  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:04  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:57  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:18:24  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:33:19  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:52:55  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:15:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _PRSPRE_H
#define _PRSPRE_H
/*
 * Program PRS, Module PRSPRE - Prefix file.
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 *
 * MODULE DESCRIPTION:
 *
 * Program PRS,  Module PRSPRE
 *
 * Prefix definitions, including binary file record and field definitions.
 *
 */

/*
 * General definitions.
 */

#define YES    1
#define NO     0
#define ERROR -1
#define FATAL -2

#define DEL '\177'

/*
 * File access definitions
 */
#ifdef vms
#define SYSTYPE YES		/* System type is known (VMS) */
#endif

#ifdef decus
#define SYSTYPE YES		/* System type is known (RSX) */
#endif

#ifdef unix
#define SYSTYPE YES		/* System type is known (UNIX) */
#endif

#ifndef SYSTYPE
#define other YES		/* System type is not known */
#endif


#ifdef other			/* For MS-DOS/ULTRIX binary file I/O, use ... */
#define FDECL FILE *		/*   standard file pointer */
#define FOPENB fopen		/*   standard fopen call */
#define FINP "rb"		/*     read binary */
#define FOUT "wb"		/*     write binary */
#define FGETCB fgetc		/*   standard fgetc call */
#define FPUTCB fputc		/*   standard fputc call */
#define FCLOSB fclose		/*   standard fclose call */
#define FDELET unlink		/*   standard unlink (delete) call */
#endif

#ifdef unix			/* For UNIX binary file I/O, use ... */
#define FDECL FILE *		/*   standard file pointer */
#define FOPENB fopen		/*   standard fopen call */
#define FINP "r"		/*     read */
#define FOUT "w"		/*     write */
#define FGETCB fgetc		/*   standard fgetc call */
#define FPUTCB fputc		/*   standard fputc call */
#define FCLOSB fclose		/*   standard fclose call */
#define FDELET unlink		/*   standard unlink (delete) call */
#endif

#ifdef decus			/* For RSX-11M binary file I/O, use ... */
#define FDECL FILE *		/*   standard file pointer */
#define FOPENB fopen		/*   standard fopen call */
#define FINP "rn"		/*     read non-text */
#define FOUT "wn"		/*     write non-text */
#define FGETCB getc		/*   DECUS-C getc call */
#define FPUTCB putc		/*   DECUS-C putc call */
#define FCLOSB fclose		/*   standard fclose call */
#define FDELET delete		/*   DECUS-C unlink (delete) call */
#endif

#ifdef vms			/* For VMS binary file I/O, use ... */
#define FDECL int		/*   integer file index */
#define FOPENB pr_open		/*   special open subroutine */
#define FINP "r"		/*     read */
#define FOUT "w"		/*     write */
#define FGETCB pr_getc		/*   special get char subroutine */
#define FPUTCB pr_putc		/*   special put char subroutine */
#define FCLOSB pr_close		/*   special close subroutine */
#define FDELET delete		/*   VMS unlink (delete) call */
#endif

/*
 * Parse table conversion definitions
 */

#if defined(other) || defined(unix)
#define INP_EXT	      ".txt"	/* Default extension for source files */
#define TMP_EXT	      ".tmp"	/* Default extension for temporary files */
#define OUT_EXT	      ".bin"	/* Default extension for output files */
#endif

#ifdef decus
#define INP_EXT	      ".TXT"	/* Default extension for source files */
#define TMP_EXT	      ".TMP"	/* Default extension for temporary files */
#define OUT_EXT	      ".BIN"	/* Default extension for output files */
#endif

#ifdef vms
#define INP_EXT	      ".TXT"	/* Default extension for source files */
#define TMP_EXT	      ".TMP"	/* Default extension for temporary files */
#define OUT_EXT	      ".BIN"	/* Default extension for output files */
#endif

#define INP_MAX		256	/* Max size of an input file record */
#define OUT_MAX		127	/* Max size of an output file record */

/*
 * Parse table source file definitions
 */

#define LBL_SIZ		15	/* Maximum length of an ELEMENT label */
#define LBL_NUM		1500	/* Maximum number of ELEMENT labels */

#define KEY_ELE		"ELEMENT"	/* ELEMENT keyword */
#define KEY_CHA		"CHARACTER"	/* CHARACTER keyword */
#define KEY_WOR		"WORD"		/* WORD keyword */
#define KEY_STR		"STRING"	/* STRING keyword */
#define KEY_OCT		"OCTAL"		/* OCTAL keyword */
#define KEY_DEC		"DECIMAL"	/* DECIMAL keyword */
#define KEY_HEX		"HEXADECIMAL"	/* HEXADECIMAL keyword */
#define KEY_FOR		"FORCE"		/* FORCE keyword */
#define KEY_GOS		"GOSUB"		/* GOSUB keyword */

#define SUB_PRO		"PROMPT"	/* PROMPT sub-keyword */
#define SUB_MIN		"MINIMUM"	/* MINIMUM sub-keyword */
#define SUB_MAX		"MAXIMUM"	/* MAXIMUM sub-keyword */
#define SUB_LEA		"LEADING"	/* LEADING sub-keyword */
#define SUB_TER		"TERMINATING"	/* TERMINATING sub-keyword */
#define SUB_CAL		"CALL"		/* CALL sub-keyword */
#define SUB_GOT		"GOTO"		/* GOTO sub-keyword */
#define SUB_NEX		"NEXT"		/* NEXT sub-keyword */
#define SUB_SUC		"SUCCESS"	/* SUCCESS sub-keyword */
#define SUB_ERR		"ERROR"		/* ERROR sub-keyword */

#define SWI_INC		"/INCLUDE"	/* /INCLUDE switch */
#define SWI_SKI		"/SKIP"		/* /SKIP switch */
#define SWI_KEE		"/KEEP"		/* /KEEP switch */

#define INT_SWI		"SWITCH"	/* SWITCH internal keyword */
#define INT_ELE		"TEST"		/* SWITCH TEST */
#define INT_LEA		"LEADING"	/* SWITCH LEADING */
#define INT_TER		"TERMINATING"	/* SWITCH TERMINATING */
#define INT_MAT		"MATCH"		/* SWITCH xxx MATCH */
#define INT_ALP		"ALPHABETIC"	/* SWITCH xxx MATCH ALPHABETIC */
#define INT_LOG		"LOGICAL"	/* SWITCH xxx MATCH LOGICAL */
#define INT_CAS		"CASE"		/* SWITCH xxx CASE */
#define INT_EXA		"EXACT"		/* SWITCH xxx CASE EXACT */
#define INT_GEN		"GENERAL"	/* SWITCH xxx CASE GENERAL */

/*
 * Parse table file "compiled" file definitions.
 *
 * Note that REC_ELE must be the lowest value, and REC_EOF must be the highest.
 * Also, no record id value may go below 0x80, or above 0x9F.
 */

#define PRS_VERSION	(char)2		/* Version */
#define PRS_EDIT	(char)0		/* Edit */

#define REC_ELE		(char)0x80	/* ELEMENT record */
#define REC_CHA		(char)0x81	/* CHARACTER test */
#define REC_WOR		(char)0x82	/* WORD test */
#define REC_STR		(char)0x83	/* STRING test */
#define REC_OCT		(char)0x84	/* OCTAL test */
#define REC_DEC		(char)0x85	/* DECIMAL test */
#define REC_HEX		(char)0x86	/* HEXADECIMAL test */
#define REC_GOS		(char)0x87	/* GOSUB test */
#define REC_FOR		(char)0x88	/* FORCE test */
#define REC_EOF		(char)0x8F	/* End of file */

#define CHR_SSP		(char)0x01	/* Separator between strings */
#define CHR_SEN		(char)0x00	/* End of strings (must be '\0') */

#define DEF_TRM		"se"		/* Default terminator string */

/*
 * Parse table "compiled" file record offsets.
 *
 * The record type (ELE_FND and XXX_FNC) must be at offset 0.
 * The record length (ELE_LEN and XXX_LEN) must be at offset 1.
 *
 * The offsets for OCT, DEC, and HEX records are combined under "NUM_xxx".
 *
 * The 3 bytes for CALL values must come immediately after the one byte
 * of offset to the CALL routine name string.
 */
#define ELE_FNC		0	/* Record type */
#define ELE_LEN		1	/* Record length (1-256) */
#define ELE_PRO		2	/* Offset in record for PROMPT string (1-256) */
#define ELE_CAL		3	/* Offset in record for CALL string (1-256) */
#define ELE_CA1		4	/* CALL value 1 (0-256) */
#define ELE_CA2		5	/* CALL value 2 (0-256) */
#define ELE_CA3		6	/* CALL value 3 (0-256) */
#define ELE_XXX		7	/* Basic length, excluding any strings */

#define XXX_FNC		0	/* Test type */
#define XXX_LEN		1	/* Record length (1-256) */
#define XXX_FLG		2	/* Flags */
#define XXX_SWT		3	/* Match and Case switches */
#define XXX_GOL		4	/* Goto table offset, low byte (1-65535) */
#define XXX_GOH		5	/* Goto table offset, high byte */

#define CHA_TST		6	/* Offset in record for string to test (1-256) */
#define CHA_TSL		7	/* Minimum match length (1-256) */
#define CHA_SLS		8	/* Offset in record for LEAD string (1-256) */
#define CHA_STS		9	/* Offset in record for TERM string (1-256) */
#define CHA_CAL		10	/* Offset in record for CALL string (1-256) */
#define CHA_CA1		11	/* CALL value 1 (0-256) */
#define CHA_CA2		12	/* CALL value 2 (0-256) */
#define CHA_CA3		13	/* CALL value 3 (0-256) */
#define CHA_XXX		14	/* Basic length, excluding any strings */

#define WOR_TST		6	/* Offset in record for string to test (1-256) */
#define WOR_TSL		7	/* Minimum match length (1-256) */
#define WOR_SLS		8	/* Offset in record for LEAD string (1-256) */
#define WOR_STS		9	/* Offset in record for TERM string (1-256) */
#define WOR_CAL		10	/* Offset in record for CALL string (1-256) */
#define WOR_CA1		11	/* CALL value 1 (0-256) */
#define WOR_CA2		12	/* CALL value 2 (0-256) */
#define WOR_CA3		13	/* CALL value 3 (0-256) */
#define WOR_XXX		14	/* Basic length, excluding any strings */

#define STR_TST		6	/* Offset in record for string to test (1-256) */
#define STR_TSL		7	/* Minimum match length (1-256) */
#define STR_TSM		8	/* Maximum match length (1-256) */
#define STR_SLS		9	/* Offset in record for LEAD string (1-256) */
#define STR_STS		10	/* Offset in record for TERM string (1-256) */
#define STR_CAL		11	/* Offset in record for CALL string (1-256) */
#define STR_CA1		12	/* CALL value 1 (0-256) */
#define STR_CA2		13	/* CALL value 2 (0-256) */
#define STR_CA3		14	/* CALL value 3 (0-256) */
#define STR_XXX		15	/* Basic length, excluding any strings */

#define NUM_VL1		6	/* Minimum value, low byte (0-longword value) */
#define NUM_VL2		7	/* Minimum value, next byte */
#define NUM_VL3		8	/* Minimum value, next byte */
#define NUM_VL4		9	/* Minimum value, high byte */
#define NUM_VM1		10	/* Maximum value, low byte (0-longword value) */
#define NUM_VM2		11	/* Maximum value, next byte */
#define NUM_VM3		12	/* Maximum value, next byte */
#define NUM_VM4		13	/* Maximum value, high byte */
#define NUM_SLS		14	/* Offset in record for LEAD string (1-256) */
#define NUM_STS		15	/* Offset in record for TERM string (1-256) */
#define NUM_CAL		16	/* Offset in record for CALL string (1-256) */
#define NUM_CA1		17	/* CALL value 1 (0-256) */
#define NUM_CA2		18	/* CALL value 2 (0-256) */
#define NUM_CA3		19	/* CALL value 3 (0-256) */
#define NUM_XXX		20	/* Basic length, excluding any strings */

#define GOS_GSL		6	/* Gosub table offset, low byte (1-65535) */
#define GOS_GSH		7	/* Gosub table offset, high byte */
#define GOS_SLS		8	/* Offset in record for LEAD string (1-256) */
#define GOS_STS		9	/* Offset in record for TERM string (1-256) */
#define GOS_CAL		10	/* Offset in record for CALL string (1-256) */
#define GOS_CA1		11	/* CALL value 1 (0-256) */
#define GOS_CA2		12	/* CALL value 2 (0-256) */
#define GOS_CA3		13	/* CALL value 3 (0-256) */
#define GOS_XXX		14	/* Basic length, excluding any strings */

#define FOR_CAL		6	/* Offset in record for CALL string (1-256) */
#define FOR_CA1		7	/* CALL value 1 (0-256) */
#define FOR_CA2		8	/* CALL value 2 (0-256) */
#define FOR_CA3		9	/* CALL value 3 (0-256) */
#define FOR_XXX		10	/* Basic length, excluding any strings */

#define FLG_GOT		0x07	/* Flag bits indicating what to do for GOTO */
#define FLG_GER		0x00	/* Return parse error to the caller (must be 0) */
#define	FLG_GOF		0x01	/* Use offset */
#define FLG_GNE		0x02	/* Goto the next ELEMENT in order */
#define FLG_GSU		0x04	/* Return parse success to the caller */

#define FLG_LEA		0x08	/* Flag bits for LEADING characters */
#define	FLG_LSK		0x00	/* Skip the leading character (must be 0) */
#define	FLG_LIN		0x08	/* Include it in the matched string */

#define FLG_TER		0x30	/* Flag bits for TERMINATING characters */
#define	FLG_TSK		0x00	/* Skip the terminal character (must be 0) */
#define	FLG_TIN		0x10	/* Include it in the matched string */
#define FLG_TNS		0x20	/* Let it be tested by next ELEMENT */

#define SWT_MEL		0x01	/* If 1, match alphabetics for CHA/STR tests */
#define SWT_MLE		0x02	/* If 1, match alphabetics for LEA chars */
#define SWT_MTE		0x04	/* If 1, match alphabetics for TER chars */

#define SWT_CEL		0x10	/* If 1, exact case for WOR/CHA/STR tests */
#define SWT_CLE		0x20	/* If 1, exact case for LEA chars */
#define SWT_CTE		0x40	/* If 1, exact case for TER chars */
#endif  /* #ifndef _PRSPRE_H */
