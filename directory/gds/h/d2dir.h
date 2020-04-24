/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2dir.h,v $
 * Revision 1.1.12.2  1996/02/18  23:36:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:20  marty]
 *
 * Revision 1.1.12.1  1995/12/08  16:01:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:09  root]
 * 
 * Revision 1.1.10.5  1994/08/10  08:25:33  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:14:31  marrek]
 * 
 * Revision 1.1.10.4  1994/06/21  14:47:18  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:51:39  marrek]
 * 
 * Revision 1.1.10.3  1994/05/10  15:55:58  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:38:41  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  15:18:32  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:30:55  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  18:52:23  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:47:15  marrek]
 * 
 * Revision 1.1.8.1  1993/10/04  17:52:54  keutel
 * 	dme changes
 * 	[1993/10/04  17:52:30  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  12:49:40  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:42:28  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:19:03  marrek
 * 	Fixes for OT 6566
 * 	[1993/02/02  09:39:07  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:41:37  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:30:12  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:11:33  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  09:33:35  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  04:09:32  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:43:03  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D2DIR_H
#define _D2DIR_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d2dir_rcsid[] = "@(#)$RCSfile: d2dir.h,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 23:36:02 $";
#endif

/*----------------------------------------------------------------------*/
/*									*/
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG		*/
/* All Rights Reserved							*/
/*									*/
/* File Name	: d2dir.h						*/
/*									*/
/* Description	: This module contains common definitions 		*/
/*		  used by gds, xds, xom and gda.			*/
/*									*/
/* Date		: December 03, 1987					*/
/*									*/
/* Author	: W. Schmid						*/
/*	(newer stuff: Jochen Keutel, SNI BA NM 123, Munich, Germany)	*/
/*									*/
/*----------------------------------------------------------------------*/

#include <dce/dce.h>

#define D2DIR_INCL             1        /* indicates that d2dir.h is  */
					/* included                   */
#define DIR_VERSION             "2.1"   /* defining version of the    */
					/* products                   */
					/* Directory - IDs            */
#define D2MINDIR_ID             1       /* min. directory ID          */
#define D2MAXDIR_ID             20      /* max. directory ID          */

#define D2CACHE_MAXDIR_ID       20
			    /**********************************************/
			    /* D2MINDIR_ID <= dir_id <= D2CACHE_MAXDIR_ID */
			    /*      --> G L O B A L  directory            */
			    /*                       configuration        */
			    /* D2CACHE_MAXDIR_ID > dir_id <= D2MAXDIR_ID  */
			    /*      --> L O C A L    directory            */
			    /*                       configuration        */
			    /**********************************************/

/******************************/
/* useful constants           */
/******************************/

#define D2_EOS    0x00                  /* end of string              */

#define D2_RDN_SEP     	'\001'          /* seperator between two RDNs */
					 /* of DN.		       */

#define D2_AVA_SEP	'\002'		/* seperator between two attr. */
					/* value assertions of a RDN.  */

					/* seperators between naming   */
					/* attr. type and value.       */
#define D2_T61_REP      '\003'          /* indicates T61 value         */
#define D2_PRTBL_REP    '\004'          /* indicates printable value   */
#define D2_NUM_REP      '\005'          /* indicates numeric value     */
#define D2_IA5_REP      '\006'          /* indicates IA5 value         */

#define D2_DL2		'\056'		/* object identifier delimiter */
					/* in a distinguished name */
#define D2_DL2S		"\056"

					/* format strings for         */
					/* Distinguished Name creation*/
					/* ATTENTION: for the last RDN*/
					/*            the last "^A"   */
					/*            must be cut off */

#define D2_RDN_FORMAT  "%c%.*s%c%.*s"
#define D2_RDNS_FORMAT  "%.*s%c%.*s"

/***************************/
/* interface specificators */
/***************************/

				      /* specifies ...                  */
#define D2_XDS_OPID      (0)          /* ... XDS functionality ...      */
#define D2_ADM_OPID     (50)          /* ... ADM functionality ...      */
				      /* ... for directory operations   */
#define D2_RES_OPID     (100)         /* ... to be added for results	*/
				      /* if you call ASN.1 encoding	*/
				      /* or decoding			*/
#define D2_REJ_OPID     (200)         /* ... to be added for rejects	*/
				      /* if you call ASN.1 encoding	*/
				      /* or decoding			*/

/**************************/
/* function return values */
/**************************/

typedef enum {D2_ERROR = -1 , D2_NOERROR} d2_ret_val;

/***********************/
/* ret_code values     */
/***********************/

#define D2_NO_ERR    0                  /* no error                   */
#define D2_TEMP_ERR  2                  /* temporary error            */
#define D2_CALL_ERR  1                  /* call error                 */
#define D2_PERM_ERR  3                  /* permanent error            */
#define D2_WARNING   4                  /* warning                    */

/************************/
/* err_class values     */
/************************/

#define D2_NOT_SPEC  0                  /* no further error           */
					/* specification              */
#define D2_PAR_ERR   1                  /* parameter error            */
#define D2_ILL_STAT  2                  /* invalid state              */
#define D2_ILL_VERS  7                  /* invalid version            */
#define D2_SYS_ERR   6                  /* system error               */
#define D2_REM_ERR   9                  /* remote error               */
#define D2_SEC_ERR   10			/* security error	      */
#define D2_INT_ERR   98                 /* internal error             */
#define D2_MESSAGE   99                 /* message (only to be used   */
					/* if ret_code = D2_WARNING)  */

/********************************/
/* global err_values            */
/********************************/

#define D2_DB_ERR    1                  /* database error             */
/*********************** Referral Errors  in pbhead *********************/

#define D2_REF_ERROR              100
					/* Referral returned for the */
					/* Base-object of the request*/

/*********************** Attribute Errors  in pbhead    *****************/

#define D2_ATT_ERROR                99  /* attr-err in pbhead_errvalue */

/********************************/
/* err_values                   */
/********************************/

/*********************** Directory User Agent Errors ******************/

#define D2_NO_BIND                 -1   /* invalid bind-ID            */
#define D2_DIR_UNKNOWN             -2   /* unknown directory-Id       */
#define D2_SHADOW_UPD              -3   /* invalid update of a shadow */
					/* entry                      */
#define D2_NO_SHADOW               -4   /* specified entry (DN) is no */
					/* shadow entry               */
#define D2_MEMORY_INSUFFICIENT     -5   /* available memory area      */
					/* exceeds                    */
#define D2_BAD_ARGUMENT            -6   /* if cstub didn't understand */
					/* the syntax                 */

/***********************   Communication errors ***********************/

#define D2_COMMUNICATION_ERROR     -7   /* error from underlying      */
					/* communication layers       */

/***********************   Tree function errors ***********************/

#define D2_PAR_NFOUND              -8   /* parent object of new object*/
					/* (--> "change_name()"       */
					/* doesn't exist              */
#define D2_NO_NEW_MASTER           -9   /* not all shadows are present*/
					/* on the new master DSA      */
					/* (--> "change_master()"     */

/***********************  Security errors returned by DUA *************/

#define D2_INVALID_AUTH_MECH	   -10  /* invalid auth. mechanism.   */

/**********************************************************************/

	/*************************/
	/* basic data types	 */
	/*************************/

typedef signed16	Bool;		/* TRUE | FALSE */
typedef byte *		Octet_string;	/* Octet string */
typedef char *		String;		/* Char string */

typedef String		IA5_string;
typedef String		Print_string;
typedef String		Numeric_string;
typedef String		UTC_time_string;

typedef Octet_string	T61_string;
typedef Octet_string	D2_name_string;
typedef Octet_string	Ob_id_string;
typedef Octet_string	Psap_string;
typedef Octet_string	Asn1_string;


/**********************************************************************/
/**********************************************************************/
/******************** ATTRIBUTE TYPES *********************************/
/**********************************************************************/
/**********************************************************************/
					  /* private attribute types */
					/* master knowledge attribute */
#define D2_MASTER_KNOWLEDGE  "\53\14\2\210\123\1\3\4\0"
					/* access control list        */
#define D2_ACL          "\53\14\2\210\123\1\3\4\1"
					/* time stamp in scheme object*/
#define D2_TIME_STAMP   "\53\14\2\210\123\1\3\4\2"
					/* shadowed by attribute      */
#define D2_SHADOWED_BY  "\53\14\2\210\123\1\3\4\3"
					/* structure rule table (SRT) */
#define D2_SRT          "\53\14\2\210\123\1\3\4\4"
					/* object class table (OCT)   */
#define D2_OCT          "\53\14\2\210\123\1\3\4\5"
					/* attribute description table*/
#define D2_AT           "\53\14\2\210\123\1\3\4\6"   /* (ADT) */

					/* indicates the position of  */
					/* attributes that can be     */
					/* stored in the cache        */
#define D2_CACHE_ATTR   "\53\14\2\210\123\1\3\4\7"
					/* NAMING: default DSA (only  */
					/*         in cache)          */
#define D2_DEFAULT_DSA  "\53\14\2\210\123\1\3\4\10"
					/* NAMING: local DSA (only in */
					/*         cache)             */
#define D2_LOCDSA       "\53\14\2\210\123\1\3\4\11"
					/* NAMING: client    (only in */
					/* cache)                     */
#define D2_CLIENT       "\53\14\2\210\123\1\3\4\12"
					/* Distinguished-Name-List    */
					/* Attribute                  */
#define D2_DNLIST       "\53\14\2\210\123\1\3\4\13"

					/* Shadowing job attr.        */
#define D2_SHADOWING_JOB  "\53\14\2\210\123\1\3\4\14"
					/* Cell-directory-service cell*/
					/* attribute                  */
#define D2_CDS_CELL     "\53\14\2\210\123\1\3\4\15"
					/* Cell-directory-service     */
					/* replica set attribute      */
#define D2_CDS_REPLICA  "\53\14\2\210\123\1\3\4\16"

/* security specific attributes.	*/

#define D2_PR_NM 	 "\53\14\2\210\123\1\3\4\17" 
					/* principal name */
#define D2_AUTH_MECH 	 "\53\14\2\210\123\1\3\4\20"
					/* authentication */
					/* mechanism      */
#define D2_DME_ALT_ADDR	 "\53\26\2\1\2\1\1"
					/* DME Alternate Address	*/
					/* attribute type		*/
					/* Decimal value is:		*/
					/* 1.3.22.2.1.2.1.1		*/

/**********************************************************************/
/**** values allowed for the attribute D2_TYPE_DSA   ******************/
/**********************************************************************/

#define D2_TYPE_DSA          "DSA-Type" /* type of DSA (for cache     */
					/* administration)            */

#define D2_1DEFAULT          "default"  /* the given DSA is the       */
					/* DEFAULT-DSA                */
#define D2_2LOCAL            "local"    /* the given DSA is the       */
					/* LOCAL-DSA                  */
#define D2_3DEFLOC           "default/local"
					/* the given DSA is the       */
					/* DEFAULT- and LOCAL-DSA     */
#define D2_4CLIENT           "client"   /* the given PSAP address is  */
					/* for the client             */

/**********************************************************************/
/**** values allowed for the attribute Principal Name *****************/
/**********************************************************************/

#define	D2_PR_NAME           "Principal Name"	/* For attribute list */
						/* in cache admin.    */


/**********************************************************************/
/**********************************************************************/
/******************** ATTRIBUTE VALUE LENGTHS *************************/
/**********************************************************************/
/**********************************************************************/

#define D2_OBJ_IDL_MAX      28          /* maximum length of object   */
					/* identifier                 */
#define D2_LOBJ_CLASS       D2_OBJ_IDL_MAX  /* object class attribute */

/**********************************************************************/
/**********************************************************************/
/******************** OBJECT CLASSES **********************************/
/**********************************************************************/
/**********************************************************************/

				    /* object class of schema object  */
#define D2_OSCHEMA      "\53\14\2\210\123\1\3\6\0"
#define D2_DME_NMO	 "\53\26\2\1\2\2\1"
					/* DME NMO Agent object class	*/
					/* Decimal value is:		*/
					/* 1.3.22.2.1.2.2.1		*/

/**********************************************************************/
/**********************************************************************/
/*********** EXTERNAL AUTHENTICATION MECHANISMS ***********************/
/**********************************************************************/
/**********************************************************************/

#define D2_DCE_EXT_CRED      "\53\14\2\210\123\1\3\203\20"

/**********************************************************************/
/**********************************************************************/
/******************** ATTRIBUTE SYNTAXES ******************************/
/**********************************************************************/
/**********************************************************************/

#define D2_GUIDE       1000 /* Search Guide Syntax                    */
#define D2_POST_AD_STX 1001 /* Postal Address syntax  (only for DSA)  */
#define D2_TLXNR_STX   1002 /* Telex-Number syntax    (only for DSA)  */
#define D2_TTXID_STX   1003 /* Teletex Terminal identifier syntax     */
#define D2_FAXNR_STX   1004 /* Faxnumber syntax                       */
#define D2_PR_DM_STX   1005 /* Preferred delivery method (Only for DSA)*/
#define D2_PSAP_STX    1006 /* Presentation Address syntax            */
#define D2_CERTIFICATE 1007 /* Certificate syntax                     */
#define D2_CERT_PAIR   1008 /* Certificate pair syntax                */
#define D2_CERT_LIST   1009 /* Certificate list syntax                */
#define D2_COUNTRY_STX 1010 /* Country-name syntax                    */
#define D2_PASSWD_STX  1011 /* Password-    syntax                    */
#define D2_ACCL       10000 /* Acccess control list                   */

/**********************************************************************/
/******************** ATTRIBUTE PRESENTATIONS *************************/
/**********************************************************************/

#define D2_T61    10002     /* Representation specifier for T61-string*/
#define D2_ASN1   10003     /* Representation for ASN1-string         */
#define D2_INT_LIST 10004   /* Representation for Integer List        */
#define D2_PRT_LIST 10005   /* Representation for printableString List*/
#define D2_T61_LIST 10006   /* Representation for T61String List      */
#define D2_ENUM   10007     /* Representation for enum		      */
#define D2_IA5    10008     /* Representation specifier for IA5-string*/
#define D2_T61_PR 10009     /* Representation specifier Choice of     */
			    /* T61, printable                         */
#define D2_T61_PR_LIST 10010/* Representation specifier Sequence of   */
			    /* Choice T61, printable                  */

/**********************************************************************/
/****** CONSTANTS FOR PRINTABLE-STRING VERIFICATION *******************/
/**********************************************************************/
#define D2_RG1          0x61        /* range a - z                    */
#define D2_RG2          0x7A

#define D2_RG3          0x41        /* range A - Z                    */
#define D2_RG4          0x5A

#define D2_RG9          0x30        /* range 0 -9                     */
#define D2_RG10         0x39

#define D2_BLANK        0x20

#define D2_ULDIFF       0x20        /* difference between lower and   */
				    /* upper case characters          */
#define D2_RGMIN        0x20        /* range ' ' - '~'                */
#define D2_RGMAX        0xFF


/**********************************************************************/
/**********************************************************************/
/******************* ACL-Handling *************************************/
/**********************************************************************/
/**********************************************************************/
			    /* interpretation of a distinguished name */
			    /* --> "D2_dnint"                         */
#define D2_SOBJ         0x30            /* '0' = Single Object        */
#define D2_SUBTREE      0x31            /* '1' = Root of Subtree      */
#define D2_GROUP        0x32            /* '2' = Group of objects     */
#define D2_IGNORE       0x33            /* '3' = ignore DN            */
#define D2_COPY         0x34            /* '4' = copy of other DSA    */


			    /* access rights                          */
			    /* --> "D2_aright"                        */
#define D2_MPUBLIC       0              /* Modify public    attributes*/
#define D2_RSTANDARD     1              /* Read   standard  attributes*/
#define D2_MSTANDARD     2              /* Modify standard  attributes*/
#define D2_RSENSITIVE    3              /* Read   sensitive attributes*/
#define D2_MSENSITIVE    4              /* Modify sensitive attributes*/

/**********************************************************************/
/*	Following are the valid object class kinds.		      */
/**********************************************************************/

				/* kinds of object classes               */
#define D2_ABSTRACT     0       /* indicates an abstract object class    */
#define D2_ALIAS        1       /* indicates an alias    object class    */
#define D2_AUXILIARY    2       /* indicates an auxiliary object class   */
#define D2_STRUCTURAL   3       /* indicates a structural object class   */

/**********************************************************************/
/**********************************************************************/

#define D2_NOACCESS_RIGHTS      5       /* number of access rights    */
#define D2_NOIDX_ACL            4       /* number of distinguished    */
					/* names per access rights    */
					/* (in ACL-list)              */
#define D2_ACLIDX_MAX          (D2_NOACCESS_RIGHTS * D2_NOIDX_ACL)
					/* total number of ACL-entries*/


/**********************************************************************/
/******************* Index-levels *************************************/
/**********************************************************************/

#define D2_MAX_ADM_IND_LEVEL    5       /* highest index level to     */
					/* administrate               */


/**********************************************************************/
/*  Facsimile Telephone Number    G3-Fax-NBPs bits                    */
/**********************************************************************/

#define D2_TWO_DIMENSIONAL      0x00000100
#define D2_FINE_RESOLUTION      0x00000200
#define D2_UNLIMITED_LENGTH     0x00100000
#define D2_B4_LENGTH            0x00200000
#define D2_A3_WIDTH             0x00400000
#define D2_B4_WIDTH             0x00800000
#define D2_UNCOMPRESSED         0x40000000
/**********************************************************************/

		/* range of "D2_attr_mod"                             */
#define D2_REP_AV    4                  /* REPlace Attribute-Value    */

		/* meaning of "Service Control" - bits of Standard    */
#define D2_NOCACHE          0x00000001  /* use master information     */
#define D2_PREF_CHAINING    0x00000002  /* prefer    chaining         */
#define D2_NOCHAINING       0x00000004  /* chaining prohibited        */
#define D2_DREFALIAS        0x00000008  /* don't dereference alias    */
#define D2_NOREFERRAL       0x00000010  /* return no referral         */
#define D2_LOCALSCOPE       0x00000020  /* local scope                */

		/* meaning of "private Service-Control-bits"          */
#define D2_DUACACHE         0x00000040  /* use DUA-Cache              */
#define D2_USEDSA           0x00000080  /* use DSA                    */
#define D2_DUAFIRST         0x00000100  /* use DUA-Cache first        */
				 /* CACHE STORAGE CLASSES             */
#define D2_RESIDENT_CLASS   0x00000200  /* store in resident class    */
#define D2_PRIVILEGED_CLASS 0x00000400  /* store in privileged class  */
#define D2_NORMAL_CLASS     0x00000800  /* store in standard class    */
#define D2_DONT_STORE       0x00001000  /* don't store in cache       */

#define D2_FILE_MODE        0x00002000  /* "append" results to        */
					/* existing file              */
#define D2_PREF_ADM_FUN     0x00004000  /* prefer adm. Op.-Id.s       */



/**********************************************************************/
/*      Authentication mechanism constants.                           */
/**********************************************************************/

#define D2_MAX_AUTH_MECHS       7

#define D2_ANONYMOUS		0
#define D2_SIMPLE		2
#define D2_DCE_AUTH		5
#define D2_STRONG		6
#define D2_SIMPLE_DCE		25
#define D2_SIMPLE_STRONG	26



/**********************************************************************/

		/* default values in common arguments                 */
#define D2_T_UNLIMITED          -1      /* no time limit              */
#define D2_S_UNLIMITED          -1      /* no size limit              */


		/* maximum lengths of RDN, etc.                       */
#define D2_NP_MAX           12          /* maximum number of RDNs     */
#define D2_NO_AVA	     3		/* maximum number of AVAs per */
					/* RDN.			      */
#define D2_AT_MAX          100          /* maximum number of attri-   */
					/* butes/object               */
#define D2_NP_LEN          128          /* maximum length of a RDN    */
#define D2_NP_ID_LEN        D2_OBJ_IDL_MAX  /* maximum string         */
					/* length of RDN identifier   */
#define D2_DNL_MAX        1024          /* maximum length of a        */
					/* distinguished name         */
#define D2_ATL_MAX        4096          /* maximum length of an       */
					/* attribute value            */
#define D2_L_AT_NAME        25          /* length of attribute name   */

/**********************************************************************/
/**********************************************************************/
/**************** In this part are all the defines ********************/
/**************** which are given by X.500 or by   ********************/
/**************** XDS (not allowed to change)      ********************/
/**********************************************************************/
/**********************************************************************/
		    /* The next error values are  */
		    /* XDS standard               */

		/* range of error type  (D2_error)                    */
#define D2_AT_PROBLEM       1
#define D2_NM_PROBLEM       2
#define D2_REF_PROBLEM      4

/*********************** Authentication Errors  ***********************/

#define D2_CREDENTIALS_INVALID     24   /* invalid credentials        */
					/* (unknown user or invalid   */
					/* password)                  */
#define D2_DSA_UNKNOWN             10   /* DSA to be contacted is     */
					/* unknown                    */
#define D2_INAPPR_AUTH             19   /* inappropriate auth.        */
					/* (not supported)            */
#define D2_INVALID_SIGNATURE       26   /* invalid signature of req.  */
#define D2_PROTECTION_REQUIRED     55   /* argument was not signed    */
					/* not from XDS               */

/*********************** Access Control Errors  ***********************/

#define D2_ACCESS_RIGHTS_INSUFFICIENT   21
					/* caller has no access rigths*/
					/* needed for request handling*/

/*********************** Attribute Errors in attr-problem   *************/

#define D2_ATTRIBUTE_MISSING        33  /* attribute not present      */
#define D2_CONSTRAINT_VIOLATION     16  /* implementation dependant   */
					/* restrictions hurt          */
					/* e. g. attribute length is  */
					/*       too long             */
#define D2_INAPPR_MATCH             20  /* inappropriate matching     */
#define D2_UNDEFINED_ATTR_TYPE      50  /* undefined attribute type   */
#define D2_ATSYNTAX_ERROR           22  /* wrong attribute syntax     */
#define D2_AT_VALUE_EXISTS           5  /* attribute value exists     */

/***********************    Name   Errors        **********************/

#define D2_NFOUND                   34  /* no entry found using search*/
					/* criterion                  */
#define D2_ON_ALIAS_NOT_ALLOWED     59  /* operation to be performed  */
					/* is not allowed on alias    */
					/* entries                    */
#define D2_NMSYNTAX_ERROR           56  /* wrong (naming) attribute   */
					/* syntax (not in XDS, this   */
					/* value must be mapped)      */
#define D2_ALIAS_ERROR               3  /* alias is orphan            */

/***********************   Service Errors       ***********************/

#define D2_TIME_LIMIT_EXCEEDED      43  /* service request couldn't   */
					/* be handled in the given    */
					/* time interval              */
#define D2_TOO_BUSY                 12  /* system is too busy         */
					/* (service request can't be  */
					/* handled)                   */
#define D2_NO_RESPONSE              48  /* a DSA needed for service   */
					/* request handling can't be  */
					/* reached                    */
#define D2_UNWILLING                58  /* a DSA needed for service   */
					/* request is unwilling to    */
					/* perform the request        */
#define D2_CHAINING_REQUIRED        14  /* chaining required although */
					/* prohibited by serv. ctrls  */
#define D2_UNABLE_TO_PROCEED        47  /* DSA could not proceed in   */
					/* name resolution            */
#define D2_INVALID_REFERENCE        25  /* invalid reference in       */
					/* operation progress         */
#define D2_ADM_LIM_EXCEEDED          1  /* administrative limit       */
					/* exceeded                   */
#define D2_LOOP_DETECTED            27  /* loop iun DSA was detected  */
#define D2_UNAV_CRIT_EXTENSION      49  /* Critical extension was     */
					/* required but not available */
#define D2_OUT_OF_SCOPE             41  /* no referrals within        */
					/* requested scope            */
#define D2_DIT_INCONSISTENT         17  /* Directory-Information-Tree */
					/* has become inconsistent    */

/***********************   Update  Errors       ***********************/

#define D2_ORPHAN                   36  /* there are still subordinate*/
					/* entries                    */
#define D2_ALREADY_EXISTS           18  /* the given entry already    */
					/* exists                     */
#define D2_NOT_ALLOWED_ON_RDN       37  /* the operation to be        */
					/* performed is not allowed   */
					/* on naming attributes       */
#define D2_NAMING_VIOLATION         31  /* naming violation           */
#define D2_OBJ_CLS_VIOLATION        40  /* object class violation     */
#define D2_AFFECTS_MULTIPLE_DSAS     2  /* operation affects multiple */
					/* DSAs (which is prohibited) */
#define D2_OBJCLS_MOD_PROHIBITED    39  /* operation attempted to     */
					/* modify object class attr.  */

/***********************   Abandon errors       ***********************/

#define D2_NO_SUCH_OPERATION        35  /* operation to be abandoned  */
					/* does not exist             */
#define D2_TOO_LATE                 44  /* response is available yet  */
#define D2_CANNOT_ABANDON           13  /* operation cannot be        */
					/* abandoned                  */
#define D2_ABANDONED                57  /* operation abandoned (for   */
					/* "receive_result" only)     */
					/* not from XDS               */
		    /* Attribute types which are given  */
		    /* by the X.500 standard            */

#define D2_OBJ_CLASS         "\125\4\0"  /* object class              */
#define D2_ALIASED_OBJECT    "\125\4\1"  /* aliased object name       */
#define D2_COMMON_NAME       "\125\4\3"  /* NAMING: common name       */
#define D2_SURNAME           "\125\4\4"
#define D2_SERIAL_NUMBER     "\125\4\5"
#define D2_COUNTRY           "\125\4\6"  /* NAMING: country           */
#define D2_LOCALITY          "\125\4\7"
#define D2_S_OR_PROV         "\125\4\10" /* stateOrProvinceName       */
#define D2_STREET_ADDRESS    "\125\4\11"
#define D2_ORG               "\125\4\12" /* NAMING: organization      */
#define D2_OU                "\125\4\13" /*NAMING: organizational unit*/
#define D2_TITLE             "\125\4\14"
#define D2_DESCRIPTION       "\125\4\15"
#define D2_S_GUIDE           "\125\4\16" /* SearchGuide */
#define D2_B_CATEGORY        "\125\4\17" /* businessCategory */
#define D2_POSTAL_ADDRESS    "\125\4\20"
#define D2_POSTAL_CODE       "\125\4\21"
#define D2_POST_BOX          "\125\4\22"  /* postOfficeBox    */
#define D2_PDO_NAME          "\125\4\23" /*physicalDeliveryOfficeName */
#define D2_TEL_NO            "\125\4\24"  /* telephone number         */
#define D2_TX_NO             "\125\4\25"  /* telex number             */
#define D2_TTX_ID            "\125\4\26" /*teletex terminal identifier*/
#define D2_FAC_TEL_NO        "\125\4\27" /*facsimile telephone number */
#define D2_X121_ADDRESS      "\125\4\30"
#define D2_ISDN_NUMBER       "\125\4\31"
#define D2_R_ADDRESS         "\125\4\32"  /* registeredAddress     */
#define D2_DES_INDICATOR     "\125\4\33"  /* destinationIndicator  */
#define D2_PREF_DEV_METHOD   "\125\4\34"  /*PreferredDeliveryMethod   */
#define D2_PSAP_ADDRESS      "\125\4\35"
#define D2_S_APP_CONTEXT     "\125\4\36" /*supportedApplicationContext*/
#define D2_MEMBER            "\125\4\37"
#define D2_OWNER             "\125\4\40"
#define D2_ROLE_OCCUPANT     "\125\4\41"
#define D2_SEE_ALSO          "\125\4\42"
#define D2_PWD               "\125\4\43"  /* user password              */

		    /* The object classes given by  */
		    /* the X.500 standard           */


#define D2_OTOP              "\125\6\0"  /* top                     */
#define D2_OALIAS            "\125\6\1"  /* alias                   */
#define D2_OCOUNTRY          "\125\6\2"  /* country                 */
#define D2_OLOCALITY         "\125\6\3"  /* locality                */
#define D2_OORG              "\125\6\4"  /* organization            */
#define D2_OOU               "\125\6\5"  /* organizational unit     */
#define D2_OPN               "\125\6\6"  /* person                  */
#define D2_OOPN              "\125\6\7"  /* organizational person   */
#define D2_OROLE             "\125\6\10" /* organizationalPerson    */
#define D2_OGROUP            "\125\6\11"  /* group of names         */
#define D2_RPN               "\125\6\12"  /* residentialPerson      */
#define D2_APR               "\125\6\13"  /* ApplicationProcesss    */
#define D2_OAE               "\125\6\14"  /* application entity     */
#define D2_ODSA              "\125\6\15"  /* DSA                    */
#define D2_ODEVICE           "\125\6\16"  /* device                 */

		    /* The object syntaxes given by  */
		    /* the X.500 standard            */

#define D2_ANY_STX    0     /* undefined syntax                       */
#define D2_DISTNAME   1     /* Distinguished name                     */
#define D2_OBJ_IDENT  2     /* object identifier syntax               */
#define D2_CEXACT     3     /* Case exact  string                     */
#define D2_CIGNORE    4     /* Case ignore string   (only for DSA)    */
#define D2_PRINTABLE  5     /* Case printable string                  */
#define D2_NUMERIC    6     /* Case numeric string                    */
#define D2_CIGN_LIST  7     /* Case ignore list (only for DSA)        */
#define D2_BOOLEAN    8     /* boolean syntax                         */
#define D2_INTEGER    9     /* integer syntax                         */
#define D2_OCTET      10    /* octet string                           */
#define D2_TIME       11    /* UTC-time                               */
#define D2_TELNR_STX  12    /* Telephone-number syntax  (Only for DSA)*/

		    /* Other values from X.500 standard  */

#define D2_ADDATT    0                  /* ADD ATTribute              */
#define D2_DELATT    1                  /* DELete ATTribute           */
#define D2_ADD_AV    2                  /* ADD Attribute-Value        */
#define D2_DEL_AV    3                  /* DELete Attribute-Value     */


		/* range of "D2_subset"                               */
#define D2_BASE_OBJECT      0           /* single object              */
#define D2_ONE_LEVEL        1           /* only nodes from next       */
					/* sublevel                   */
#define D2_WHOLE_SUBTREE    2           /* whole subtree              */


		/* range of "D2_ret_val"                              */
#define D2_R_TYP            0           /* return attribute type      */
#define D2_R_VAL_TYP        1           /* return attribute type and  */
					/* value                      */

#define D2_ALL_ATT_REQ     -1           /* all attributes are         */
					/* requested (to be used in   */
					/* "d2_r_no_at")              */


		/* range of "D2_a_opmod" and d2_c_mode in search crit */
#define D2_ITEM             0           /* "item"                     */
#define D2_AND              1           /* "and"                      */
#define D2_OR               2           /* "or"                       */
#define D2_NOT              3           /* "not"                      */

		/* range of "D2_a_match" and d2_mode in search crit   */
#define D2_EQUAL            0           /* complete matching required */
#define D2_SUBSTRINGS       1           /* matching by substrings     */
#define D2_GTEQ             2           /* matching rule greater or   */
					/* equal                      */
#define D2_LTEQ             3           /* matching rule less    or   */
					/* equal                      */
#define D2_PRESENT          4           /* only existence of attribute*/
					/* should be checked          */
#define D2_APPROX_MATCH     5           /* approximate match by using */
					/* phonetic extension         */

		/* range of "d2_fi_pos" (matching rule for substrings)*/
#define D2_INITIAL          0
#define D2_ANY              1
#define D2_FINAL            2


		/* range of "d2_ref_type" (type of reference)         */
#define D2_SUPERIOR         1
#define D2_SUBORDINATE      2
#define D2_CROSS            3
#define D2_NON_SPEC_SUBORDINATE 4

		/* range of "d2_nr_phase" (name resolution phase)     */
#define D2_NOT_STARTED      1
#define D2_PROCEEDING       2
#define D2_COMPLETED        3

		/* range of "d2_priority " (priority)                 */
#define D2_PRIO_LOW         0
#define D2_PRIO_MEDIUM      1
#define D2_PRIO_HIGH        2

		/* range of "d2_scope_ref" (scope of referrals)       */
#define D2_DMD_SCOPE        0
#define D2_COUNTRY_SCOPE    1
#define D2_WORLD_SCOPE      2

		/* range of "d2_limit_problem" (limit problem)        */
#define D2_TIME_LIMIT_EX    0
#define D2_SIZE_LIMIT_EX    1
#define D2_ADM_LIMIT_EX     2
#define D2_NO_LIMIT_EX      3


		    /* End of standard values  */

/**********************************************************************/
/****************** typedefs and defines ******************************/
/**************** for MHS X.400 information ***************************/
/**********************************************************************/

		    /* Attribute types which are given  */
		    /* by the X.400 standard            */

#define D2_MHS_DEL_CONT_LEN  "\126\5\2\0"/* MHS deliverable content */
						 /* length                  */
#define D2_MHS_DEL_CONT_TYP  "\126\5\2\1"/* MHS deliverable content */
						 /* type                    */
#define D2_MHS_DEL_EITS      "\126\5\2\2"/* MHS deliverable eits    */
#define D2_MHS_DL_MEMBERS    "\126\5\2\3"/* MHS user agent DL-members */
#define D2_MHS_DL_SUBM_PERM  "\126\5\2\4"/* MHS DL-submit permissions */
#define D2_MHS_MESS_STORE    "\126\5\2\5"/* MHS message store       */
#define D2_MHS_OR_ADDRESS    "\126\5\2\6"/* MHS OR-address          */
#define D2_MHS_PREF_DEL_M    "\126\5\2\7"/* MHS preferred delivery  */
					/* method                     */
#define D2_MHS_SUP_AUT_ACT   "\126\5\2\10 "/* MHS supported automatic */
						   /* actions                 */
#define D2_MHS_SUP_CONT_TYP  "\126\5\2\11" /* MHS supported content */
						   /* type                  */
#define D2_MHS_SUP_OPT_ATTR  "\126\5\2\12" /* MHS supported optional*/
						   /* attributes            */


		    /* The object classes given by  */
		    /* the X.400 standard           */


#define D2_MHS_DISTR_LIST    "\126\5\1\0"/* MHS distribution list   */
#define D2_MHS_M_STORE_CL    "\126\5\1\1"/* MHS message store       */
#define D2_MHS_MTA           "\126\5\1\2"/*MHS message transfer agent*/
#define D2_MHS_USER          "\126\5\1\3"/* MHS user                */
#define D2_MHS_USER_AGENT    "\126\5\1\4"/* MHS user agent          */

		    /* The attribute syntaxes given by  */
		    /* the X.400 standard               */

#define D2_MHS_DLSP_STX   100   /* MHS DL-submit-permission syntax    */
#define D2_MHS_ORADR_STX  101   /* MHS OR-Address syntax              */
#define D2_MHS_ORNAME_STX 102   /* MHS OR-name syntax                 */
#define D2_MHS_PR_DM_STX  103   /* MHS Preferred delivery method stx. */

		    /* Upper and lower bounds for       */
		    /* Terminal-type                    */

#define D2_MHS_T_T_LBOUND      3
#define D2_MHS_T_T_UBOUND      8



/**********************************************************************/
/********* Defines for the Strong Athentification Package *************/
/**********************************************************************/

/* Algorithm object identifiers	*/
#define CERT_ALG_ID_MAX_LEN	16		/* Maximal length of	*/
						/* the algorithm ob.id.	*/
#define CERT_RSA		 "\125\10\1\1"
						/* encryptionAlgorithm	*/
#define CERT_SQMOD_N		 "\125\10\2\1"
						/* hashAlgorithm	*/
#define CERT_SQMOD_NWITHRSA	 "\125\10\3\1"
						/* signatureAlgorithm	*/

/**********************************************************************/
/*************************** TYPEDEFs *********************************/
/**********************************************************************/

typedef signed16 D2_attr_mod;              /* ATTRIBUTE MODIFICATION     */


typedef signed16 D2_subset;                /* SEARCH COMMAND-OPTION      */


typedef signed16 D2_ret_val;               /* REQUESTED ATTRIBUTE VALUES */


typedef signed16 D2_a_opmod;               /* COMPARISON MODE FOR FILTER */


typedef signed16 D2_a_match;               /* MATCHING RULE              */

typedef struct {                        /* OBJECT IDENTIFIER          */
		    signed32             d2_typ_len;
		    Ob_id_string	d2_type;
	       } D2_obj_id;

typedef D2_obj_id   D2_a_type;          /* Attribute type             */

				/**************************************/
				/* REMARK: If the typedefs above will */
				/*         be changed, the alignments */
				/*         in the IPC-messages must be*/
				/*         checked again.             */
				/**************************************/

typedef char  D2_dnint;                 /* a flag indicating how the  */
					/* DN in ACL-list is to be    */
					/* interpreted                */


typedef signed16 D2_aright;                /* access rights              */

typedef struct
	{   signed16           version;        /* version of interface   */
	    signed16           ret_code;       /* return code            */
	    signed16           err_class;      /* error class            */
	    signed16           err_value;      /* error value            */
	} Pbhead;                           /* PBHEAD                 */

typedef struct
	{   signed16           d2_version;     /* version of interface   */
	    signed16           d2_retcode;     /* return code            */
	    signed16           d2_errclass;    /* error class            */
	    signed16           d2_errvalue;    /* error value            */
	} D2_pbhead;                        /* PBHEAD                 */


typedef struct D2_extension
  { struct  D2_extension       *d2_next_ext;/* next extension         */
	    signed16               d2_ext_id;  /* extension identifier   */
	    Bool                d2_critical;/* indicates whether      */
					    /* extension is critical  */
	/*    union               {            NOT IMPLEMENTED
				} d2_ext_item;                 */
	} D2_extension;             /* STANDARD EXTENSION             */


typedef struct D2_op_progress
	{   signed16               d2_nr_phase;/* name resolution phase  */
	    signed16               d2_next_rdn;/* number of next RDN to  */
					    /* be resolved            */
	} D2_op_progress;           /* OPERATION PROGRESS STRUCTURE   */


typedef struct
   {        D2_extension    *d2_extension;   /* Extension             */
	    D2_op_progress  d2_op_progress;  /* Operation progress    */
	    signed16           d2_al_rdn;       /* number of aliased RDNs*/
	    signed32            d2_serv_cntrls;  /* Service-Controls      */
	    signed16           d2_priority;     /* priority;             */
					     /* in V2 not used        */
	    signed16           d2_time_limit;   /* time limit in seconds */
	    signed16           d2_size_limit;   /* number of entries     */
					     /* to be returned        */
	    signed16           d2_scope_ref;    /* scope of referral     */
	    signed16		d2_sign_mech;	/* signature mechanism	*/
	    signed16		d2_prot_mech;	/* protection request	*/
	} D2_c_arg;                          /* COMMON-ARGUMENTS      */


typedef struct
	{
	    Bool            d2_al_deref;    /* indicates whether      */
					    /* object was dereferenced*/
	    byte *  d2_performer;   /* performer of operation */
					    /* (only important, when  */
					    /* results are signed)    */
	} D2_c_res;


typedef struct {
	signed32               d2_size;
	unsigned char   * d2_value;
} D2_str;

typedef struct {
	signed32 d2_ci_mode;
	D2_a_type     d2_ci_type;
} D2_crit_item;

typedef  struct {
	signed32           and_or_nr;
	struct D2_criteria   *and_or;
} D2_crit_set ;

typedef struct D2_criteria {
	signed32 d2_c_mode;
	union {
		D2_crit_set            *d2_ci_and_or ;
		D2_crit_item           *type;
		struct D2_criteria     *not;
	      }  d2_crit_val;
	} D2_criteria;

typedef struct guide {
	Bool            d2_obj_defined;
	D2_a_type       d2_obj;
	D2_criteria     d2_crit;
} D2_guide;

typedef struct  {
	D2_str   d2_terminal;
	D2_str   d2_controls;           /* optional   */
	D2_str   d2_graphics;           /* optional   */
	D2_str   d2_miscel;             /* optional   */
	D2_str   d2_pages;              /* optional   */
	D2_str   d2_privates;           /* optional   */
} D2_ttxid;

typedef struct Telex_Number {
	D2_str   d2_t_number;
	D2_str   d2_c_code;
	D2_str   d2_answer;
} D2_telex_number;

typedef struct faxNumber {          /* used for teletexTerminalId as well */
	D2_str           d2_tel_nr;
	Bool             d2_par_defined;
	signed32             d2_par;       /* used for bit string */
} D2_faxnumber;

typedef struct T61_seq {
	signed32                  d2_nb_item;
	D2_str             * d2_t61_item;
} D2_T61_seq;

		    /* structures needed for the        */
		    /* the X.400 standard               */

typedef struct D2_str_list {
	struct D2_str_list *d2_next;
	unsigned16             d2_rep;          /* D2_PRINTABLE or D2_T61 */
	D2_str             d2_str;          /* or D2_NUMERIC          */
} D2_str_list;

typedef struct {
	D2_str_list     *d2_admd_name;
	D2_str_list     *d2_common_name;
	D2_str_list     *d2_country_name;
	D2_str_list     *d2_domain_type1;
	D2_str_list     *d2_domain_type2;
	D2_str_list     *d2_domain_type3;
	D2_str_list     *d2_domain_type4;
	D2_str_list     *d2_domain_value1;
	D2_str_list     *d2_domain_value2;
	D2_str_list     *d2_domain_value3;
	D2_str_list     *d2_domain_value4;
	D2_str_list     *d2_generation;
	D2_str_list     *d2_given_name;
	D2_str_list     *d2_initials;
	Numeric_string  d2_isdn_number;
	Numeric_string  d2_isdn_subaddress;
	Numeric_string  d2_numeric_user_id;
	D2_str_list     *d2_org_name;
	D2_str_list     *d2_org_unit_name1;
	D2_str_list     *d2_org_unit_name2;
	D2_str_list     *d2_org_unit_name3;
	D2_str_list     *d2_org_unit_name4;
	D2_str_list     *d2_post_addr_details;
	T61_string      d2_post_addr_in_full;
	D2_str_list     *d2_post_code;
	D2_str_list     *d2_post_country_name;
	D2_str_list     *d2_post_deliv_pt_name;
	Print_string    d2_post_deliv_sys_name;
	D2_str_list     *d2_post_gen_deliv_addr;
	D2_str_list     *d2_post_locale;
	D2_str_list     *d2_post_office_box_nbr;
	D2_str_list     *d2_post_office_name;
	D2_str_list     *d2_post_office_nbr;
	D2_str_list     *d2_post_org_name;
	D2_str_list     *d2_post_patron_details;
	D2_str_list     *d2_post_patron_name;
	D2_str_list     *d2_post_street_addr;
	Psap_string     d2_presentation_addr;
	D2_str_list     *d2_prmd_name;
	D2_str_list     *d2_surname;
	Print_string    d2_terminal_id;
	signed32        d2_terminal_type;
	Numeric_string  d2_x121_addr;
	D2_str_list     *d2_post_addr_in_lines;
} D2_OR_address;


typedef struct {
	D2_name_string  d2_dir_name;
	D2_OR_address   *d2_or_address;
} D2_OR_name;                     /* Name, OR-Address or both != NULL */

typedef struct {
	unsigned16          d2_perm_type;
	union {
		 D2_OR_name      *d2_individual;
		 D2_OR_name      *d2_member_of_dl;
		 D2_OR_name      *d2_pattern_match;
		 D2_name_string  d2_member_of_group;
	      }           d2_choice;
} D2_dl_submit_perm;

/**********************************************************************/
/*       definitions used for Strong Authentication package	      */
/**********************************************************************/

typedef struct {			/* BIT STRING		      */
		signed32	d2_size;   /* count of bits in bitstring */
		unsigned char * d2_value;  /* pointer to bitstring */
	       } D2_bit_str;

typedef struct {			/* ALGORITHM IDENTIFIER */
		D2_obj_id	algorithm;
		signed32      * parameters; /* optional */
	       } Alg_id;

#define SIGNED(Type)					\
	typedef struct {				\
			Type		to_be_signed;	\
			Alg_id		alg;		\
			D2_bit_str	encr_str;	\
		       }

typedef struct {
		UTC_time_string	not_before;
		UTC_time_string	not_after;
	       } Validity;

typedef struct {
		Alg_id		algorithm;
		D2_bit_str	subject_key;
	       } Subj_public_key_info;

typedef struct {
		signed32		version;
		signed32		serial_number;
		Alg_id			signature;
		D2_name_string		issuer;
		Validity		validity;
		D2_name_string		subject;
		Subj_public_key_info	subj_public_key_info;
	       } Cert_unsigned;

SIGNED (Cert_unsigned) Certificate;

typedef struct {
		Certificate	* forward;
		Certificate	* reverse;
	       } Cert_pair;

typedef struct {
		Alg_id		signature;
		D2_name_string	issuer;
		signed32	user_cert;
		UTC_time_string	rev_date;
	       } Rev_cert_body;

typedef struct {
		signed32	no_rev_certs;
		Rev_cert_body * rev_cert;
		} Rev_certs_unsigned;

SIGNED (Rev_certs_unsigned) Rev_certs;

typedef struct {
		Alg_id		signature;
		D2_name_string	issuer;
		UTC_time_string	last_update;
		Rev_certs     * rev_certs;
	       } Cert_list_unsigned;

SIGNED (Cert_list_unsigned) Cert_list;

/**********************************************************************/
/*       common definitions for attribute types and values	      */
/**********************************************************************/

typedef struct
	{   signed16          d2_a_v_len; /* length of attribute value  */
					/* not valid for D2_guide        */
					/* not valid for D2_T61_seq      */
					/* not valid for D2_print_seq    */
					/* not valid for D2_telex_number */
					/* not valid for D2_faxnumber    */
	    unsigned16           d2_a_rep;
					/* representation-syntax of   */
					/* the attribute values       */
	    union
	     { T61_string        d2_t61_a_v;
	       Octet_string      d2_oct_a_v;
	       Print_string      d2_prt_a_v;
	       D2_name_string    d2_obj_name_a_v;
	       Ob_id_string      d2_obid_a_v;
	       Numeric_string    d2_num_a_v;
	       UTC_time_string   d2_utc_a_v;
	       Psap_string       d2_psap_a_v;
	       Asn1_string       d2_asn1_a_v;
	       IA5_string	 d2_ia5_a_v;
	       D2_guide        * d2_guide_a_v;  /* for serch guide */
	       D2_str_list     * d2_post_a_v; /* for Postal address */
	       D2_T61_seq      * d2_t61lst_a_v; /* for T61 list */
	       D2_ttxid        * d2_ttxid_a_v;  /* for TTX-ID */
	       D2_telex_number * d2_tlxnr_a_v;  /* telex */
	       D2_faxnumber    * d2_faxnr_a_v;  /* fax-number */
	       Certificate     * d2_cert_a_v;
	       Cert_pair       * d2_ctpair_a_v;
	       Cert_list       * d2_ctlist_a_v;
	       Bool              d2_bool_a_v;  /* d2_a_v_len has no meaning*/
	       signed32          d2_int_a_v;   /* d2_a_v_len has no meaning*/
	       signed32        * d2_intlist_a_v; /* d2_a_v_len =
						    sizeof(int) * nb_int */
	       D2_OR_address    *d2_or_addr_av; /* OR-Address         */
	       D2_OR_name       *d2_or_name_av; /* OR-name            */
	       D2_dl_submit_perm *d2_dl_s_p_av; /* DL-submit permission */
	     } d2_at_value;             /* attribute value            */
	} D2_a_value;                   /* ATTRIBUTE-VALUE            */

typedef struct
	{	D2_a_type 		d2_avatype; /* attr. type */
		D2_a_value 		d2_avaval; /* attr. value */
	} D2_ava;				/* ATTRIBUTE-VALUE-ASSERTION */

/* Filter from Version 2.01A up  **************************************/

typedef struct  D2_Fi_substr
	{
	    signed16             d2_fi_pos ;       /* position of substring */
						   /* for matching          */
	    D2_a_value           d2_fi_str_val ;   /* length & value of sub.*/
	    struct D2_Fi_substr  *d2_fi_next ;     /* pointer to next       */
	} D2_Fi_substr ;

typedef struct
	{
	    D2_a_match      d2_fi_match ;          /* matching rule         */
	    D2_a_type       d2_fi_type;            /* attribute type        */
	    union {D2_a_value    d2_fi_att_val;    /* length & value of att.*/
		   D2_Fi_substr  d2_fi_substrings; /* substring (used if    */
						   /* d2_fi_match =         */
						   /* D2_SUBSTRINGS         */
		  } d2_fi_val ;
	} D2_Fi_item;

typedef struct  D2_Fi_filter
	{
		D2_a_opmod      d2_fi_opmod;
		union { struct {
			signed16             d2_fi_no_fi ;
			struct D2_Fi_filter *d2_fi_filter ;
			       } fi_and_or ;
			struct D2_Fi_filter *d2_fi_not ;
			D2_Fi_item          d2_fi_item ;
		      } d2_fi_v ;
	} D2_Fi_filter ;


typedef struct D2_a_info
	{   struct D2_a_info   *d2_a_next;  /* pointer to next block  */
	    D2_a_type           d2_a_type;  /* attribute type         */
	    signed16            d2_a_no_val;/* number of values       */
	    D2_a_value         *d2_a_val;   /* values                 */
					    /* = pointer to first     */
					    /*  "D2_a_value"-structure*/
	} D2_a_info;               /* QUEUE OF ATTRIBUTE-INFORMATIONS */



typedef struct D2_l_info
	{   struct D2_l_info   *d2_l_next;  /* pointer to next RDN-   */
					    /* block                  */
	    D2_name_string      d2_l_rdn;   /* RDN                    */
	    Bool                d2_l_alias; /* alias-information      */
	    Bool                d2_l_fromentry;     /* master-inform. */
	} D2_l_info;                    /* QUEUE of RDNs              */

typedef struct D2_a_modinf
	{   struct D2_a_modinf *d2_m_next;  /* pointer to next modify-*/
					    /* attribute information  */
					    /* block                  */
	    D2_attr_mod         d2_m_mod;   /* kind of modification   */
	    D2_a_info           d2_m_attr;  /* attribute information  */
					    /* "next"-pointer not used*/
	} D2_a_modinf;      /* QUEUE OF MODIFY-ATTRIBUTE INFORMATIONS */


typedef struct D2_acc_point
	{   struct D2_acc_point *d2_next_ap;/* pointer to next block  */
	    D2_name_string      d2_dsa_ap;  /* DSA-Name               */
	    Psap_string         d2_adr_ap;  /* DSA-Adress             */
	} D2_acc_point;             /* QUEUE OF ACCESS POINTS         */


typedef struct D2_ref_inf
	{   struct D2_ref_inf   *d2_next_ref;   /* next referral      */
	    D2_name_string      d2_target;  /* target object          */
	    signed16            d2_al_rdns; /* number of aliased RDNs */
	    D2_op_progress      d2_op_progr;/* operation progress     */
	    signed16            d2_rdn_res; /* number of RDNs resolved*/
					    /* from supplied object   */
	    signed16            d2_ref_type;/* reference type         */
	    D2_acc_point        *d2_acc_point;  /* sequence of access-*/
					    /* points                 */
	} D2_ref_inf;               /* QUEUE OF REFERRAL-INFORMATIONS */


typedef struct D2_ei_info
	{   struct D2_ei_info  *d2_ei_next; /* pointer to next block  */
	    D2_name_string      d2_ei_entry;/* Distinguished Name     */
	    D2_a_info          *d2_ei_ai;   /* attribute informations */
					    /* (queue of attribute    */
					    /* information)           */
	    Bool                d2_ei_fromentry;   /* master-inform. */
	} D2_ei_info;            /* QUEUE OF ENTRY-INFORMATION-BLOCKS */


typedef struct
	{   D2_ret_val          d2_r_val_at;/* return Type / Value    */
	    signed16            d2_r_no_at; /* number of attribute    */
					    /* types                  */
	    D2_a_type          *d2_r_type_at;/* pointer to array of   */
					    /* attribute types        */
	} D2_req_info;                      /* REQUESTED INFORMATION  */


typedef struct
	{   signed16            d2_p_len;   /* length of password     */
	    Octet_string        d2_p_pw;    /* password               */
	} D2_password;                      /* PASSWORD               */


typedef struct
	{   D2_name_string      d2_c_name;  /* pointer to user name   */
	    D2_password        *d2_c_passwd;/* pointer to password    */
	} D2_credent;                       /* USER-CREDENTIALS       */


typedef struct D2_a_problem
  {  struct D2_a_problem       *d2_ap_next; /* pointer to next problem*/
	    signed16            d2_ap_prob; /* attribute problem      */
				                    /* range: Attribute-errors*/
	    D2_a_info           d2_ap_info;  /* pointer to attribute   */
			    /* info. (no. of val = 0; for attr. type only) */
	} D2_a_problem;                     /* ATTRIBUTE PROBLEM      */

typedef struct
	{   D2_name_string      d2_ae_name; /* pointer to object name */
	    D2_a_problem       *d2_ae_prob; /* pointer to attribute   */
					    /* problems               */
	} D2_a_error;                       /* ATTRIBUTE ERROR        */

typedef struct
	{   D2_name_string      d2_ne_match;/* pointer to matched obj.*/
	    signed16            d2_ne_prob; /* problem. range: Name   */
					    /* errors                 */
	} D2_n_error;                       /* NAME      ERROR        */

typedef struct
	{   signed16         d2_e_type;  /* error type             */
	    union
	    {
	      D2_a_error    *d2_e_atproblem;/* attribute problem      */
	      D2_ref_inf    *d2_e_referral; /* referral               */
	      D2_n_error    *d2_e_nmproblem;/* name      problem      */
	    } d2_e_problem;                 /* problem                */
	} D2_error;                         /* ERROR STRUCTURE        */


/**********************************************************************/
/* TYP          : C-MACRO                                             */
/* NAME         : D2_CHECK_MINMAX                                     */
/* AUTHOR       : Horn                                                */
/* DATE         : 21.01.86                                            */
/* SYNTAX       : D2_CHECK_MINMAX  (min, max, act)                    */
/* DESCRIPTION  : This Macro is used to check whether a given value   */
/*                is within given borders.                            */
/* INPUT PARAMETERS:                                                  */
/*                min   minimum value                                 */
/*                max   maximum value                                 */
/*                act   actual value                                  */
/* OUTPUT PARAMETERS: --                                              */
/* RETURNVALUE     :  != 0: value ok                                  */
/*                    == 0: value not ok                              */
/* STANDARD HEADER :  --                                              */
/* GLOBAL DATA (used):    --                                          */
/* GLOBAL DATA (changed): --                                          */
/* CALLED FUNCTIONS/PROGRAMS/MACROS: --                               */
/* REMARKS:        The Macro is to be used in that way:               */
/*                 if  (D2_CHECK_MINMAX (min, max, act))              */
/*                       { ** correct       ** }                      */
/*                 else  { ** not correct   ** }                      */
/*                                                                    */
/**********************************************************************/

#define D2_CHECK_MINMAX(min, max, act) (((min) <= (act)) && ((max) >= (act)))


/**********************************************************************/
/* TYP          : C-MACRO                                             */
/* NAME         : D2_ALIGN                                            */
/* AUTHOR       : Horn                                                */
/* DATE         : 18.01.88                                            */
/* SYNTAX       : D2_ALIGN (x)                                        */
/* DESCRIPTION  : This Macro can be used for alignment to double word.*/
/* INPUIT PARAMETER:                                                  */
/*                x     value (length) to be aligned                  */
/* OUTPUT PARAMETER:  aligned value (length)                          */
/* RETURNVALUE     :  --                                              */
/* STANDARD HEADER :  --                                              */
/* GLOBAL DATA (used):    --                                          */
/* GLOBAL DATA (changed): --                                          */
/* CALLED FUNCTIONS/PROGRAMS/MACROS: --                               */
/* REMARKS      :                                                     */
/*                                                                    */
/**********************************************************************/

#define D2_ALIGN(x)     (((x-1) & (~3)) + 4)

/**********************************************************************/
/*       MACROS for storing and loading short integers and longs      */
/**********************************************************************/
#define D2_STSHORT(x, y)    (x = y)
#define D2_LDSHORT(x)       x

#define D2_STLONG(x, y)     (x = y)
#define D2_LDLONG(x)        x

#define D2_ATTCMP(x,y)   ((x)->d2_typ_len != sizeof(y) - 1 ||           \
		      memcmp((x)->d2_type, (y), (x)->d2_typ_len) != 0)
			       /* compares two attribute types        */
			       /* 1.param: address of struct D2_a_type*/
			       /* 2.param: constant attribute type    */

/**********************************************************************/
/*       definitions used only by DME 				      */
/**********************************************************************/

typedef struct D2_alt_addr {
	D2_str d2_addr;
	struct {
		signed32 d2_num;
		D2_obj_id * d2_obj_id;} d2_ob_id_set;
	} D2_alt_addr;

#endif /* _D2DIR_H */
