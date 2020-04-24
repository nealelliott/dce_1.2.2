/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tnsx.h,v $
 * Revision 1.1.765.2  1996/02/18  23:36:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:03  marty]
 *
 * Revision 1.1.765.1  1995/12/08  16:02:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:43  root]
 * 
 * Revision 1.1.763.1  1994/02/22  18:08:32  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:50:35  marrek]
 * 
 * Revision 1.1.761.2  1993/10/14  17:35:48  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:33:34  keutel]
 * 
 * Revision 1.1.761.1  1993/10/13  17:32:58  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:10:22  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  13:08:51  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:46:23  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:43:45  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:31:23  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:52:06  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:39:34  melman]
 * 
 * $EndLog$
 */
#ifndef _TNSX_H
#define _TNSX_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char tnsx_rcsid[] = "@(#)$RCSfile: tnsx.h,v $ $Revision: 1.1.765.2 $ $Date: 1996/02/18 23:36:28 $";
#endif

/*
 * COPYRIGHT (C) 1987 BY SIEMENS AG
 * All rights reserved
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1990
 *               All Rights Reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */
/*  30.09.92    mm NC151        Konstanten fuer Routing Info => AU90     */

/*****************************************************************************/
/*									     */
/*									     */
/*			   HEADER-FILE  (tnsx.h)			     */
/*									     */
/*    for programming interface to Transport Name Service in SINIX           */
/*		 							     */
/*			       I C M X (TNS)				     */
/*									     */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/* Version: V3.0							     */
/*****************************************************************************/

/*
 * @(#)tnsx.h	311.1 92/02/12 	TNSX (DF 1)
 */

/* #ident "@(#)tnsx.h	311.2	92/09/30 TNSX (NC1)" */

/*****************************************************************************/
/*				CONSTANTS				     */
/*****************************************************************************/

/*
 * System parameter
 */
#define TS_NOBJECTS	4000	/* Max. number of objects */
#define TS_CMX_DIR_ID	1	/* Identifier for CMX directory */

/*****************************************************************************/
/*				NAMES AND NAME PARTS 			     */
/*****************************************************************************/

/*
 * Structure of name part
 * Values for identification of name parts
 * Values for maximal length of name parts
 */
typedef struct {
	long	ts_np_id ;	/* Identification of name parts */
#define TS_COUNTRY	66000	/* Country */
#define TS_ADMD		66001	/* Administration domain */
#define TS_PRMD		66002	/* Private domain */
#define TS_OU		66003	/* Organisation unit */
#define TS_PN		66004	/* Personal name */
	short	ts_np_length ;	/* Length of name part */
#define TS_LCOUNTRY	2	/* Country */
#define TS_LADMD	16	/* Administration domain */
#define TS_LPRMD	16	/* Private domain */
#define TS_LOU		10	/* Organisation unit */
#define TS_LPN		30	/* Personal name */
	char	*ts_np_value ;	/* Pointer to value */
} Ts_name_part ;

/*
 * Structure of the name of an object with properties (leaf)
 * Structure of the name of an object without properties (node)
 */
#define TS_NO_NP	5	/* Max. number of name parts */

typedef struct {
	short		ts_no_of_el ;
	Ts_name_part	ts_lf[TS_NO_NP] ;
} Ts_leaf_name ;

typedef struct {
	short		ts_no_of_el ;
	Ts_name_part	ts_nlf[TS_NO_NP - 1] ;
} Ts_non_leaf_name ;

/*****************************************************************************/
/*				PROPERTIES           			     */
/*****************************************************************************/

/*
 * Structure of a property
 * Structure of a filter
 * Values for the names of properties
 * Values for the type of properties
 * Maximal length of the values of a properties
 */
#define TS_MAX_NO_OF_PROPERTIES	20	/* max. number of properties */

typedef struct {
	short	ts_name ;
#define TS_EMPTYPROP	  0	/* terminator and indicator for empty list */
#define TS_TRANS	100	/* TRANSPORT ADDRES */
#define TS_NEABX	102	/* MIGRATION SERVICE */
#define TS_ROUT		103	/* ROUTING INFORMATION */
#define TS_GTYPE	105	/* DEVICE TYPE (e.g. DSS9750) */
#define TS_TRSYS	106	/* TRANSPORT SYSTEM (NEA, ISO, LAN, STA) */
#define TS_LNAME	107	/* LOCALE NAME (T-Selectors) */
#define TS_USER1PR	120	/* USER PROPERTY 1 */
#define TS_USER2PR	121     /* USER PROPERTY 2 */
#define TS_USER3PR	122     /* USER PROPERTY 3 */
	short	ts_type ;
#define TS_ITEM		0 
#define TS_GROUP	1	/* group property (not in version 1) */
	short	ts_length ;
#define TS_LPROP	200	/* maximal length of value */
	char	*ts_value ;
} Ts_property ;

typedef struct {
	short	ts_pr_name ;	/* cf. ts_name */
	short	ts_pr_length ;	/* cf. ts_length */
	char	*ts_pr_value ;	/* cf. ts_value */
} Ts_filter ;

/*
 * Values of properties
 * TRANSPORT ADDRES
 * 	see struct t_partaddr in cmx.h
 * LOCALER NAME (T-Selectoren)
 * 	see struct t_myname in cmx.h
 * MIGRATION SERVICE
 * 	Boolean variable with
 * 	TS_TRUE = MIGRATION SERVICE requested
 * 	TS_FALSE = MIGRATION SERVICE not requested
 * ROUTING INFORMATION
 *  neu: CC list encoded in char array (len = MAX_LEN_AU90 )
 *	alt: bitwise coded CC list Ts_route 
 * DEVICE TYPE
 *	Device type Ts_gtype 
 * TRANSPORT SYSTEM
 *	Type Ts_system 
 * USER PROPERTY[1-3]
 *	defined by user
 */
typedef short Ts_route;
#define MAX_LEN_AU91 12
#define MAX_LEN_AU90  2 + 16*MAX_LEN_AU91
typedef struct {char au90[MAX_LEN_AU90];
 }Ts_route_new;

typedef char Ts_gtype ;
#define TS_NOGTYPE	0	/* no device protocol */
#define TS_SS8110	1	/* Application (type 11) */
#define TS_DSS9750	2	/* 9750 DDT (type 57) */
#define TS_DRS8122	3	/* 8122 PT (type 43) */

typedef char Ts_system ;
#define TS_NEA		0	/* NEA transport system */
#define TS_ISO		1	/* ISO transport system */
#define TS_LAN		2	/* LAN transport system */
#define TS_STA		3	/* Station interconnection */

/*****************************************************************************/
/*				ERROR INFORMATIONS 			     */
/*****************************************************************************/

/*
 * The error information supplied with return value "TS_ERROR" 
 * in data type Ts_head is structured as follows:
 *	
 *  ts_retcode        ts_errclass       ts_errvalue
 * +-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+
 * |       o       | |       o       | |       o       |
 * +-+-+-+-|-+-+-+-+ +-+-+-+-+-+-+-+-+ +-+-+-+-|-+-+-+-+
 *         |                 |                 +--- ERROR VALUE
 *         |                 +--- ERROR CLASS
 *	   +--- ERROR TYPE
 *
 * ERROR VALUES correspond to ERROR CLASS
 */
#define TS_OK		 0	/* function call successful */
#define TS_ERROR	-1	/* function call failed */

/*
 * Structure of standard header
 */
typedef struct {
	short	ts_version ;	/* VERSION */
#define TS_1VER01	0x3000	/* Information retrieval */
#define TS_2VER01	0x3000	/* Information maintainance */
	short	ts_retcode ;	/* ERROR TYPE */
#define	TS_NOERR	  0	/* No error */
#define	TS_TEMPERR	 -1	/* Transient error */
#define TS_CALLERR	 -2	/* Call error */
#define TS_PERMERR	 -3	/* Permanenter error */
#define TS_APPERR	 -6	/* Error in DUA-Interface */
#define	TS_WARNING	-99	/* Warning */ 
	short	ts_errclass ;	/* ERROR CLASS */
#define TS_NOTSPEC	  0	/* Unspecified error */
#define TS_PARERR	  1	/* Parameter error */
#define TS_ILLVERS	  3	/* Invalid version */
#define TS_SYSERR	  4	/* System error */
#define TS_INTERR	  6	/* Internal error */
#define TS_MESSAGE	 99      /* Info */
	short	ts_errvalue ;	/* ERROR VALUE */
} Ts_head ;

/*
 * ERROR VALUES for ERROR CLASS TS_PARERR (1)
 */
#define	TS_DIRERR	 1	/* TS directory unknown */
#define	TS_NAMERR	 2	/* GLOBAL NAME not yet/already existing */
#define TS_ILLNAM	 3	/* GLOBAL NAME has incorrect syntax */
#define TS_STRERR	 4	/* Invalid value for "ts11str_def" */
#define TS_PROPER	 5	/* Property not yet/already existing */
#define TS_SCOPER	 6	/* Invalid value for "ts11scope" */
#define TS_MEMORY	 9	/* supplied memory insufficient */
#define TS_SIDERR	12	/* Invalid short id */
#define TS_ILLUSE	13	/* Short id is not allowed */
#define TS_OBJXST	15	/* Object is already existing */
#define TS_LENERR	16	/* Lenght error in path/property */

/*
 * ERROR VALUES for ERROR CLASS TS_ILLVERS (3)
 */
/*#define TS_NOTSPEC 	0 */

/*
 * ERROR VALUES bei ERROR CLASS TS_SYSERR (4)
 */
/* these are system errors as defined in errno.h */

/*
 * ERROR VALUES for ERROR CLASS TS_INTERR (6)
 */
#define	TS_TIMOUT	  7	/* No response from tnsxd */
#define TS_UPDERR	 14	/* TS directory full (overflow) */
#define TS_PROT		 20	/* Error in tnsxd protocol */
#define TS_NORQ		 21	/* tnsxd temporarily not available */
#define TS_EXEC		 30	/* exec (in child-process) failed */
#define TS_CHILDSIG	 31	/* Signal from child-process */
#define TS_DIRVERS	 32	/* Wrong version of directory */
#define TS_LFILE	100	/* Length error in file of TS directory */

/*
 * ERROR VALUES for ERROR CLASS TS_MESSAGE (99)
 */
#define TS_TIMLIM	 8	/* Time limit reached */
#define TS_LEAFNO	10	/* More GLOBAL NAMES than expected */
#define TS_CHLDNO	11	/* More children than expected */
 
/*****************************************************************************/
/*				PARAMETER AREA				     */
/*****************************************************************************/

/*
 * Basic data types
 */
typedef char	Ts_Bool ;
#define	TS_FALSE	0
#define TS_TRUE		1

typedef	short	Ts_str_def ;
#define TS_RESTRICTED 	0
#define TS_GENERAL 	1

typedef short	Ts_scope ;
#define TS_LOCAL 	0
#define TS_GLOBAL 	1

/*
 * Structure of a path name
 */
typedef struct {
	char	*ts_path ;
} Ts_pfname ;

/*
 * Parameter structure for function ts11_read_leafs()
 */
typedef struct {
	Ts_head		*ts_head ;
	short		ts11dir_id ;
	Ts_leaf_name	*ts11partial_name ;
	Ts_str_def	ts11str_def ;
	Ts_filter	*ts11filter ;
	Ts_scope	ts11scope ;
	Ts_pfname	ts11filename ;
	short		ts11time_limit ;
	char		*ts11npval ;
	short		ts11l_np_values ;
	short		ts11exp_no ;
	Ts_leaf_name	*ts11leafs ;
	short		ts11real_no ;
} Ts_P11 ;

/*
 * Parameterstructure for function ts12_read_children()
 */
typedef struct {
	Ts_head		*ts_head ;
	short		ts12dir_id ;
	Ts_non_leaf_name *ts12parent ;
	Ts_filter	*ts12filter ;
	char		*ts12npval ;
	short		ts12l_np_values ;
	short		ts12exp_no ;
	Ts_name_part	*ts12children ;
	short		ts12real_no ;
} Ts_P12 ;

/*
 * Parameter structure for function ts13_read_properties()
 */
typedef struct {
	Ts_head		*ts_head ;
	short		ts13dir_id ;
	Ts_leaf_name	*ts13leaf_name ;
	long		ts13short_id ;
	char		*ts13prval ;
	short		ts13l_pr_values ;
	Ts_property	*ts13prop ;
	Ts_leaf_name	*ts13dist_name ;
	char		*ts13npval ;
	short		ts13l_np_values ;
} Ts_P13 ;

/*
 * Parameter structure for function ts21_creat_non_leaf_entity()
 */
typedef struct {
	Ts_head		 *ts_head ;
	short		 ts21dir_id ;
	Ts_non_leaf_name *ts21parent ;
} Ts_P21 ;

/*
 * Parameter structure for function ts22_delete_non_leaf_entity()
 */
typedef struct {
	Ts_head		 *ts_head ;
	short		 ts22dir_id ;
	Ts_non_leaf_name *ts22parent ;
} Ts_P22 ;

/*
 * Parameter structure for function ts23_create_leaf_entity()
 */
typedef struct {
	Ts_head		 *ts_head ;
	short		 ts23dir_id ;
	Ts_non_leaf_name *ts23parent ;
	Ts_name_part	 *ts23dist_arc ;
	Ts_property	 *ts23prop ;
} Ts_P23 ;

/*
 * Parameter structure for function ts24_delete_leaf_entity()
 */
typedef struct {
	Ts_head		*ts_head ;
	short		ts24dir_id ;
	Ts_leaf_name	*ts24dist_name ;
} Ts_P24 ;

/*
 * Parameter structure for function ts25_add_property()
 */
typedef struct {
	Ts_head		*ts_head ;
	short		ts25dir_id ;
	Ts_leaf_name	*ts25dist_name ;
	long		ts25short_id ;
	Ts_property	*ts25prty ;
} Ts_P25 ;

/*
 * Parameter structure for function ts26_delete_property()
 */
typedef struct {
	Ts_head		*ts_head ;
	short		ts26dir_id ;
	Ts_leaf_name	*ts26dist_name ;
	long		ts26short_id ;
	short		ts26prid ;
} Ts_P26 ;

/*
 * Parameter structure for function ts_27_change_property()
 */
typedef struct {
	Ts_head		*ts_head ;
	short		ts27dir_id ;
	Ts_leaf_name	*ts27dist_name ;
	long		ts27short_id ;
	Ts_property	*ts27prty ;
} Ts_P27 ;

/*****************************************************************************/
/*	DEFINITION OF ICMX(TNS)-CALLS					     */
/*****************************************************************************/

short	ts11_read_leafs(Ts_P11 *parblk) ;
short	ts12_read_children(Ts_P12 *parblk) ;
short	ts13_read_properties(Ts_P13 *parblk) ;
short	ts21_create_non_leaf_entity(Ts_P21 *parblk) ;
short	ts22_delete_non_leaf_entity(Ts_P22 *parblk) ;
short	ts23_create_leaf_entity(Ts_P23 *parblk) ;
short	ts24_delete_leaf_entity(Ts_P24 *parblk) ;
short	ts25_add_property(Ts_P25 *parblk) ;
short	ts26_delete_property(Ts_P26 *parblk) ;
short	ts27_change_property(Ts_P27 *parblk) ;
#endif /* _TNSX_H */
