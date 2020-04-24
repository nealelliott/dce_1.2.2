/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: gdsext.h,v $
 * Revision 1.1.10.2  1996/02/18  23:36:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:44  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:02:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:27  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:18:43  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:29:57  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:54:16  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:46:21  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:59:04  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:44:11  marrek]
 * 
 * Revision 1.1.4.4  1993/01/29  08:40:03  marrek
 * 	Change DS_MAX_DEF_RULE_NR to 17.
 * 	[1993/01/28  10:47:59  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:42:31  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:30:48  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:13:04  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  09:34:11  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  04:15:42  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:43:44  zeliff]
 * 
 * $EndLog$
 */
#ifndef _GDSEXT_H
#define _GDSEXT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gdsext_rcsid[] = "@(#)$RCSfile: gdsext.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:36:16 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYP		: INCLUDE					      */
/*								      */
/* NAME         : gdsext.h                                            */
/*								      */
/* AUTHOR       : SNI AP 11                                           */
/* DATE         : 25. 10. 1990                                        */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOK.-REF.    : DS-Design-Specification                             */
/*								      */
/* PRD#/VERS.	:						      */
/*								      */
/* DESCRIPTION  : This Include-File contains "typedefs" and "defines" */
/*                to be used for administration API.                  */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-10-25| Original                       | WS |         */
/*datoff **************************************************************/


					/* Directory - IDs            */
#define MINDIR_ID             1         /* min. directory ID          */
#define MAXDIR_ID             20        /* max. directory ID          */

					/* version numbers            */
#define DS_V02      10                  /* version-id 2.0             */
#define DS_V021     11                  /* version-id 2.1             */
#define DS_V022     12                  /* version-id 2.2             */
#define DS_V03      13                  /* version-id 3.0             */


typedef Octet_string            Name_string;
typedef Octet_string            ACL_string;

typedef  struct
	 { char     retcode;         /* Returncode                       */
	   char     errclass;        /* Errorclass                       */
	   signed16 errvalue;        /* Errorvalue                       */
	 } Errcode;

#define OBJ_IDL_MAX         28          /* maximum length of object   */
					/* identifier                 */
#define DNL_MAX             1024        /* maximum length of a        */
					/* distinguished name         */


/**************************/
/* function return values */
/**************************/
#define NOERROR     0                   /* function successful        */
#define ERROR      -1                   /* function fails             */


/*******************/
/* ret_code values */
/*******************/
#define NOERR    '0'                    /* no error                   */
#define CALLERR  '1'                    /* call error                 */
#define TEMPERR  '2'                    /* temporary error            */
#define PERMERR  '3'                    /* permanent error            */


/************************/
/* err_class values     */
/************************/
#define NOTSPEC  '0'                    /* no further error           */
					/* specification              */
#define PARERR   '1'                    /* parameter error            */
#define ILLSTAT  '2'                    /* invalid state              */
#define SESSION  '3'                    /* session error              */
#define RETRY    '4'
#define PENDING  '5'
#define SYSERR   '6'                    /* system error               */
#define ILLVERS  '7'                    /* invalid version            */
#define MBSERR   '8'                    /* MBS error                  */
#define DSAERR   '9'                    /* DSA error                  */
#define DHSERR   'A'                    /* DHS error                  */
#define DVSERR   'B'                    /* DVS error                  */
#define ROSERR   'C'                    /* ROS error                  */
#define MSSG     'D'                    /* MESSAGE                    */


/***********************/
/* global error values */
/***********************/
#define DS_NOT_SPEC        0   /* illegal version number                 */
#define DS_NO_BIND     -1   /* illegal session                        */
#define DS_NO_RESPONSE 48   /* no connection to DSA available         */
#define DS_TOO_BUSY    12   /* DSA is busy                            */
#define DS_ALLOC_ERROR -1001/* error occurred during memory allocation*/
			    /* (malloc)                               */
#define DS_NO_FILE     -300 /* file doesn't exist                     */
#define DS_FILE_ERR    -301 /* error while accessing file             */
#define DS_INV_SEARCH_SEL -302 /* declaration of base-object and      */
			    /* subset not correct                     */

#define DS_SEARCH_ERR   -200        /* SEARCH operation fails         */
#define DS_ADD_ERR      -201        /* ADD operation fails            */
#define DS_MOD_ERR      -202        /* MODIFY operation fails         */
#define DS_DEL_ERR      -203        /* DELETE operation fails         */


/**********************************/
/* error values of ACL management */
/**********************************/
#define DS_ACL_WRONG   -304 /* wrong format of ACL                    */
#define DS_WR_NO_DNS   -305 /* illegal number of DNs in ACL info block*/


/*************************************/
/* error values of Shadow management */
/*************************************/
#define DS_LAST_JOB         -306    /* last shadowing job reached     */
#define DS_UPD_JOB_FAILS    -307    /* update of shadowing job        */
				    /* attribute failed               */
#define DS_OVERLAY      -205        /* overlapping shadowing jobs     */
#define DS_INVTIME      -206        /* invalid update times of a      */
				    /* shadowing job                  */
#define DS_INVMODE      -207        /* invalid update mode of a       */
				    /* shadowing update job           */
				    /* or invalid mode for reading    */
				    /* shadowing jobs                 */
#define DS_DSA_NFOUND   -209        /* DSA not found                  */
#define DS_CRON_ERR	-210
#define DS_SHD_FILE_ERR -211        /* error while reading shadowing  */
				    /* job file                       */
#define DS_EOF 		-212
#define DS_DEST_LOC	-213	    /* in shadow creation destination */
				    /* DSA should not be local DSA.   */

/****************************************/                                    
/* Security error in shadow management. */                                    
/****************************************/                                    
#define DS_MASTB_IMP    -214        /* master bind not possible with these */ 
                                    /* DCE credentials or with this auth.  */ 
                                    /* mechanism.                          */ 

/***********************************/
/* service control parameters.	   */
/***********************************/
#define NOCACHE          0x00000001  /* use master information     */
#define LOCALSCOPE       0x00000020  /* local scope                */

/***********************************/
/* error values of tree management */
/***********************************/
#define DS_NFOUND        34         /* object not found               */
#define DS_PAR_NFOUND   -204        /* parent object not found        */
#define DS_ACCESS_RIGHTS_INSUFFICIENT   21      /* invalid access     */
						/* rights             */
#define DS_SERV_CTRLS_INVALID   -1004   /* wrong combination of       */
					/* service control bits       */
#define DS_NO_NEW_MASTER        -47     /* not all shadows are present*/
					/* on the new master DSA      */


/*************************************/
/* error values of schema management */
/*************************************/
#define DS_READ_FAILS  -101 /* error while reading schema             */
#define DS_ENT_NFOUND  -102 /* either AT, OCT or SRT entry doesn't    */
			    /* exists (depending on following values  */
			    /* of "upd_mode":                         */
			    /*          DEL_AT_ENTRY, MOD_AT_ENTRY    */
			    /*      or  DEL_OCT_ENTRY, MOD_OCT_ENTRY  */
			    /*      or  DEL_SRT_ENTRY, MOD_SRT_ENTRY) */
#define DS_SUP_NFOUND  -103 /* either superior OCT entry or SRT entry */
			    /* doesn't exist (depending on following  */
			    /* values of "upd_mode":                  */
			    /*          ADD_OCT_ENTRY, MOD_OCT_ENTRY  */
			    /*      or  ADD_SRT_ENTRY, MOD_SRT_ENTRY) */
#define DS_EXISTS      -104 /* either AT, OCT or SRT entry already    */
			    /* exists (depending on following values  */
			    /* of "upd_mode")                         */
			    /*          ADD_AT_ENTRY                  */
			    /*      or  ADD_OCT_ENTRY                 */
			    /*      or  ADD_SRT_ENTRY)                */
#define DS_OBJCLS_NFOUND -105
			    /* object class (referenced in SRT entry) */
			    /* doesn't exist (if "upd_mode" is        */
			    /*      ADD_SRT_ENTRY or MOD_SRT_ENTRY)   */
#define DS_OPT_ATTR_NFOUND -106
			    /* optional attribute (referenced in OCT  */
			    /* entry) doesn't exist (if "upd_mode" is */
			    /*      ADD_OCT_ENTRY or MOD_OCT_ENTRY)   */
#define DS_MAND_ATTR_NFOUND -107
			    /* mandatory attribute (referenced in OCT */
			    /* entry) doesn't exist (if "upd_mode" is */
			    /*      ADD_OCT_ENTRY or MOD_OCT_ENTRY)   */
#define DS_OPT_AND_MAND     -108
			    /* attribute is both a optional and a     */
			    /* mandatory one (if "upd_mode" is        */
			    /*      ADD_OCT_ENTRY or MOD_OCT_ENTRY)   */
#define DS_OBJID_NUNIQUE -109
			    /* either the object identifier of an AT  */
			    /* entry or of an OCT entry is not unique */
			    /* (depending on the following values of  */
			    /* "upd_mode":                            */
			    /*          ADD_AT_ENTRY, MOD_AT_ENTRY    */
			    /*      or  ADD_OCT_ENTRY, MOD_OCT_ENTRY) */
#define DS_TOO_MANY    -110 /* too many rules to generate a           */
			    /* complete DN structure (from ROOT to    */
			    /* the leaf node)                         */
			    /* (if "upd_mode" is ADD_SRT_ENTRY)       */
			    /*  or "upd_mode" is MOD_SRT_ENTRY)       */
#define DS_ORPHAN      -111 /* either OCT entry is still used as a    */
			    /* superior object class in another OCT   */
			    /* entry or                               */
			    /* SRT entry is still used as a parent    */
			    /* rule in another SRT entry              */
			    /* (relevant if "upd_mode" is either      */
			    /*     DEL_OCT_ENTRY or DEL_SRT_ENTRY)    */
#define DS_OCT_IN_USE  -112 /* OCT entry can't be deleted because     */
			    /* it is referenced in a SRT entry        */
			    /* (if "upd_mode" is DEL_OCT_ENTRY)       */
#define DS_AT_IN_USE   -113 /* AT entry can't be deleted because      */
			    /* it is referenced either in a SRT or    */
			    /* in OCT (if "upd_mode" is DEL_AT_ENTRY) */
#define DS_WR_FILENO   -114 /* wrong input for file number            */
#define DS_WR_ABBRV    -115 /* wrong input for an abbreviation        */
			    /* (must not contain ' ' and must not be  */
			    /* empty)                                 */
#define DS_WR_OBJ_ID_FORMAT -116
			    /* wrong format of object identifier      */
			    /* (either for attribute or object class  */
			    /* object identifier)                     */
#define DS_WR_NAME     -117 /* wrong input for either attribute name  */
			    /* or object class name (must not         */
			    /* contain ' ' and must not be empty)     */
#define DS_WR_BOUND    -118 /* wrong input for either upper or lower  */
			    /* bound                                  */
#define DS_WR_NO_RECUR -119 /* wrong input for number of recurring    */
			    /* attribute values                       */
#define DS_WR_SYNTAX   -120 /* wrong input for syntax                 */
#define DS_WR_ACL      -121 /* wrong input for access class           */
#define DS_WR_IND_LEV  -122 /* wrong input for index level            */
#define DS_WR_PHON     -123 /* wrong input for phonetical matching    */
#define DS_UPD_ERR     -124 /* schema update in the DSA fails         */
#define DS_FORM_ERR    -125 /* illegal format of SRT/OCT/AT while     */
			    /* reading schema from DSA 		      */
#define DS_ILL_OP      -126 /* illegal operation mode in "gds_upsh()" */
#define DS_NAME_NUNIQUE -127 /* either attribute name in an AT entry  */
			    /* or object class name in an OCT entry is*/
			    /* not unique                             */
			    /* (depending on the following values of  */
			    /* "upd_mode":                            */
			    /*          ADD_AT_ENTRY, MOD_AT_ENTRY    */
			    /*      or  ADD_OCT_ENTRY, MOD_OCT_ENTRY) */
#define DS_NAME_ATTR_NFOUND -128
			    /* Naming attribute (referenced in SRT    */
			    /* entry) doesn't exist (if "upd_mode" is */
			    /*      ADD_SRT_ENTRY or MOD_SRT_ENTRY)   */
#define DS_REPETITION -129  /* either an attribute is used several    */
			    /* times in an OCT entry(if "upd_mode" is */
			    /*      ADD_OCT_ENTRY or MOD_OCT_ENTRY)   */
			    /* or an object class  is used several    */
			    /* times in an SRT entry(if "upd_mode" is */
			    /*      ADD_SRT_ENTRY or MOD_SRT_ENTRY)   */
#define DS_NO_VALUE   -130  /* either no 	   attribute  for an  */
			    /* OCT entry is  specified (if "upd_mode" */
			    /* is   ADD_OCT_ENTRY or MOD_OCT_ENTRY)   */
			    /* or no object class  for an SRT entry   */
			    /* is specified (if "upd_mode" is         */
			    /*      ADD_SRT_ENTRY or MOD_SRT_ENTRY)   */
#define DS_NAME_ATTR_INVALID 	-131
			    /* Naming attribute does not belong to the*/
			    /* object classes referenced in SRT entry */
			    /* ("upd_mode" is ADD_SRT_ENTRY or        */
			    /* MOD_SRT_ENTRY)			      */
#define DS_OPT_MAND_NAME_ATTR	-133
			    /* optional or mandatory attribute is     */
			    /* naming attribute in SRT entry.         */ 
			    /* modification in OCT entry leads to a   */
			    /* situation when Naming attribute does   */
			    /* not belong to some object class in an  */
			    /* SRT entry.			      */
			    /* ("upd_mode" is MOD_OCT_ENTRY)	      */
#define DS_DEL_DEF_SCH		-134
			    /* entry of default schema can not be     */
			    /* deleted.				      */
			    /* "upd_mode" is DEL_SRT_ENTRY, or        */
			    /* DEL_OCT_ENTRY, or DEL_AT_ENTRY	      */
#define DS_MOD_DEF_SRT		-135
			    /* SRT entry of default schema can not be */
			    /* modified.			      */
			    /* "upd_mode" is MOD_SRT_ENTRY	      */ 
#define DS_MOD_OID_DEF_SCH	-136
			    /* obj. id. of attr. in default schema can*/
			    /* not be changed.			      */
			    /* "upd_mode" is MOD_AT_ENTRY or	      */ 
			    /* MOD_OCT_ENTRY.			      */
#define DS_MOD_SOBJ_DEF_OCT	-137
			    /* sup. obj. class of an object class in  */
			    /* default schema can not be changed.     */
			    /* "upd_mode" is MOD_OCT_ENTRY	      */ 
#define DS_MOD_MAND_DEF_OCT	-138
			    /* mand. attribute of an object class in  */
			    /* default schema can not be modified.    */
			    /* "upd_mode" is MOD_OCT_ENTRY	      */ 
#define DS_WR_RULE_NR		-139
			    /* wrong rule number (<=0)		      */
			    /* "upd_mode" is ADD_SRT_ENTRY	      */
#define DS_SIMILAR_RULE         -140 
			    /* too many rules which generate same     */
			    /* complete DN structure (from ROOT to    */
			    /* the leaf node)                         */
			    /* (if "upd_mode" is ADD_SRT_ENTRY)       */
			    /*  or "upd_mode" is MOD_SRT_ENTRY)       */
#define DS_MOD_SNTX_DEF_AT	-141
			    /* Syntax of attr. in default schema can  */
			    /* not be changed.			      */
			    /* "upd_mode" is MOD_AT_ENTRY or	      */ 
#define DS_MOD_CK_DEF_SCH	-142
			    /* object class kind of default schema    */
			    /* entry can not be modified.	      */
			    /* (if "upd_mode" is ADD_OCT_ENTRY)       */
			    /*  or "upd_mode" is MOD_OCT_ENTRY)       */
#define DS_WR_NAM_SNTX		-143
			    /* naming attribute must have case ignore */
			    /* syntax.				      */
#define DS_WR_NAM_RECR		-144
			    /* naming attribute must have +ve no. for */
			    /* recurring values.		      */
#define DS_MAND_SUP	        -145
			    /* mandatory attr. appears as mand. attr. */
			    /* in one of the superior object classes. */
#define DS_OPT_SUP	        -146
			    /* optional attr. appears as mand. or opt.*/
			    /* attr. in one of the sup. obj. classes. */
#define DS_WR_KIND_SCLASS	-147
			    /* wrong kind of object class as super    */
			    /* class.				      */
			    /* (if "upd_mode" is ADD_SRT_ENTRY)       */
			    /*  or "upd_mode" is MOD_SRT_ENTRY)       */
			    /* (if "upd_mode" is ADD_OCT_ENTRY)       */
			    /*  or "upd_mode" is MOD_OCT_ENTRY)       */
#define DS_NO_MORE_ALIAS	-148
			    /* Not more than 1 object class of ALIAS  */
			    /* kind can exist in schema.	      */
			    /* (if "upd_mode" is ADD_OCT_ENTRY)       */
			    /*  or "upd_mode" is MOD_OCT_ENTRY)       */
#define DS_INVALID_OC_KIND	-149
			    /* Object class kind does not belong to   */
			    /* set of valid object class kinds.	      */
			    /* (if "upd_mode" is ADD_OCT_ENTRY)       */
			    /*  or "upd_mode" is MOD_OCT_ENTRY)       */
#define DS_TOO_MANY_STR_SC	-150
			    /* more than 1 structural object classes  */
			    /* exist as direct super classes of a     */
			    /* structural object class.		      */
			    /* (if "upd_mode" is ADD_OCT_ENTRY)       */
			    /*  or "upd_mode" is MOD_OCT_ENTRY)       */
#define DS_MOD_SCH		-151
			    /* schema object class can not be modified*/
			    /*  or "upd_mode" is MOD_OCT_ENTRY)       */
#define DS_SCLASS_LOOP		-152
			    /* modification of object class generates */
			    /* loop in object class hierarchy.	      */
			    /*  or "upd_mode" is MOD_OCT_ENTRY)       */
#define DS_AUX_CONS_VIOL	-153
			    /* constraint violation: object class is  */
			    /* being used as aux. class.	      */
			    /*  or "upd_mode" is MOD_OCT_ENTRY)       */
#define DS_ONLY_STR_AUX		-154
			    /* only structural kind of object class   */
			    /* can have auxiliary classes.	      */
			    /* (if "upd_mode" is ADD_OCT_ENTRY)       */
			    /*  or "upd_mode" is MOD_OCT_ENTRY)       */
#define DS_AUX_AUX		-155
			    /* auxiliary object class should be of    */
			    /* AUXILIARY kind.			      */
			    /* (if "upd_mode" is ADD_OCT_ENTRY)       */
			    /*  or "upd_mode" is MOD_OCT_ENTRY)       */
#define DS_SCLASS_CONS_VIOL	-156
			    /* constraint violation: in usage as super*/
			    /* class.				      */
			    /*  or "upd_mode" is MOD_OCT_ENTRY)       */
#define DS_SRULE_STR		-157
			    /* a structure rule can have only STRUCTURAL */
			    /* kind of object class.		      */
			    /* (if "upd_mode" is ADD_SRT_ENTRY)       */
			    /*  or "upd_mode" is MOD_SRT_ENTRY)       */
#define DS_SRULE_CONS_VIOL	-158
			    /* constraint violation: in usage as object */
			    /* class in a SRULE.		      */
			    /*  or "upd_mode" is MOD_OCT_ENTRY)       */
#define DS_GTP_MISSING		-159
			    /* a STRUCTURAL object class does not have */
			    /* GTP as one of its super classes	       */
			    /*  or "upd_mode" is ADD_OCT_ENTRY)       */
			    /*  or "upd_mode" is MOD_OCT_ENTRY)       */
#define DS_PR_DM_STX_RECUR	-160
			    /* PR_DM_STX and MHS_PR_DM_STX syntaxes   */
			    /* can not be multi-valued.		      */
			    /*  or "upd_mode" is ADD_AT_ENTRY)        */
			    /*  or "upd_mode" is MOD_AT_ENTRY)        */

/********************************/
/* S C H E M A  H A N D L I N G */
/********************************/
					/* schema update operation    */
					/* modes                      */
#define ADD_AT_ENTRY    0
#define MOD_AT_ENTRY    1
#define DEL_AT_ENTRY    2
#define ADD_OCT_ENTRY   3
#define MOD_OCT_ENTRY   4
#define DEL_OCT_ENTRY   5
#define ADD_SRT_ENTRY   6
#define MOD_SRT_ENTRY   7
#define DEL_SRT_ENTRY   8

#define L_ABBRV         4               /* length of abbreviation for */
					/* SRT-, OCT- and AT-entries  */
					/* (incl. EOS)                */
#define L_MAX_NAME      26              /* max. length of the name of */
					/* a SRT-, OCT- and AT-entry  */
					/* (incl. EOS)                */
#define L_OBJ_ID        29              /* length of object identifier*/
					/* (represented as printable  */
					/* string; incl. EOS)         */

#define NO_NAM_ATTR	3		/* maximum no. of naming      */
					/* attrs. for a structure rule*/

#define NO_SUP_CLASS	10		/* maximum no. of super classes*/
					/* for an object class.	      */

#define NO_AUX_CLASS	10		/* maximum no. of auxiliary   */
					/* object classes for an object*/
					/* class.		      */

#define NO_MAND_ATTR    10              /* number of mandatory attr.  */
					/* in an OCT entry            */
#define NO_OPT_ATTR     50              /* number of optional attr.   */
					/* in an OCT entry            */


					/* access classes             */
#define PUBLIC          0
#define STANDARD        1
#define SENSITIVE       2

/**********************************************************************/
/**********************************************************************/
/******************** ATTRIBUTE SYNTAXES ******************************/
/**********************************************************************/
/**********************************************************************/

#define DS_GUIDE       1000 /* Search Guide Syntax                    */
#define DS_POST_AD_STX 1001 /* Postal Address syntax  (only for DSA)  */
#define DS_TLXNR_STX   1002 /* Telex-Number syntax    (only for DSA)  */
#define DS_TTXID_STX   1003 /* Teletex Terminal identifier syntax     */
#define DS_FAXNR_STX   1004 /* Faxnumber syntax                       */
#define DS_PR_DM_STX   1005 /* Preferred delivery method (Only for DSA)*/
#define DS_PSAP_STX    1006 /* Presentation Address syntax            */
#define DS_CERTIFICATE 1007 /* Certificate syntax                     */
#define DS_CERT_PAIR   1008 /* Certificate pair syntax                */
#define DS_CERT_LIST   1009 /* Certificate list syntax                */
#define DS_COUNTRY_STX 1010 /* Country-name syntax                    */
#define DS_PASSWD_STX  1011 /* Password-    syntax                    */
#define DS_ACCL       10000 /* Acccess control list                   */
#define DS_SH_BY_STX  10001 /* Shadowed by syntax                     */


		    /* The object syntaxes given by  */
		    /* the X.500 standard            */

#define DS_ANY_STX    0     /* undefined syntax                       */
#define DS_DISTNAME   1     /* Distinguished name                     */
#define DS_OBJ_IDENT  2     /* object identifier syntax               */
#define DS_CEXACT     3     /* Case exact  string                     */
#define DS_CIGNORE    4     /* Case ignore string   (only for DSA)    */
#define DS_PRINTABLE  5     /* Case printable string                  */
#define DS_NUMERIC    6     /* Case numeric string                    */
#define DS_CIGN_LIST  7     /* Case ignore list (only for DSA)        */
#define DS_BOOLEAN    8     /* boolean syntax                         */
#define DS_INTEGER    9     /* integer syntax                         */
#define DS_OCTET      10    /* octet string                           */
#define DS_TIME       11    /* UTC-time                               */
#define DS_TELNR_STX  12    /* Telephone-number syntax  (Only for DSA)*/


typedef char Abbrv[L_ABBRV] ;

typedef struct {
	    signed32 rule_nr;           /* rule number                */
	    signed32 par_rule_nr;       /* rule number of parent rule */
	    signed32 no_nam_attr;	/* number of naming attributes*/
	    Abbrv    nam_attr[NO_NAM_ATTR];         
					/* list of naming attributes  */
					/* attribute                  */
	    Abbrv    obj_class;		/* assigned object class.     */
} Srt;                  /* S T R U C T U R E  R U L E  T A B L E      */


typedef struct {
	    Abbrv    obj_abbrv;         /* abbreviation of object     */
					/* class                      */
	    signed32 no_sup_class;	/* no. of super classes.      */ 
	    Abbrv    sup_class[NO_SUP_CLASS];
					/* list of super classes.     */
	    char     obj_id[L_OBJ_ID];  /* object identifier for      */
					/* object class               */
	    char     obj_name[L_MAX_NAME]; /* object class name       */
	    signed32 obj_file_nr;       /* file number for object     */
					/* class                      */
	    signed32 obj_class_kind;	/* kind of object class.      */
	    signed32 no_aux_class;	/* no. of auxiliary object    */
					/* classes.		      */
	    Abbrv    aux_class[NO_AUX_CLASS];
					/* list of auxiliary object   */
					/* classes.		      */
	    signed32 no_must_attr;      /* number of mandatory        */
					/* attributes                 */
	    Abbrv    must_attr[NO_MAND_ATTR];
					/* list of mandatory attr.    */
	    signed32 no_may_attr;       /* number of optional         */
					/* attributes                 */
	    Abbrv    may_attr[NO_OPT_ATTR];
					/* list of optional attributes*/
} Oct;                  /* O B J E C T  C L A S S  T A B L E          */


typedef struct {
	    Abbrv    att_abbrv;         /* attribute abbreviation     */
	    char     att_obj_id[L_OBJ_ID]; /* object identifier for   */
					   /* attribute               */
	    char     att_name[L_MAX_NAME]; /* attribute name          */
	    signed32 att_lower;         /* lower bound                */
	    signed32 att_upper;         /* upper bound                */
	    signed32 att_no_recur;      /* number of recurring values */
	    signed32 att_syntax;        /* attribute syntax           */
	    signed32 att_acl;           /* access class of attribute  */
	    signed32 att_ind_lev;       /* index level of attribute in*/
					/* database                   */
	    signed32 att_phon;          /* a flag indicating whether  */
					/* phonetic comparison is     */
					/* allowed on the attribute   */
} At;                   /* A T T R I B U T E  T A B L E               */


typedef struct {
	    At       *at_entry;         /* AT entry to be added or    */
					/* modified                   */
	    signed32  no_at;            /* number of AT elements      */
	    At       *at;               /* AT                         */
	    signed32  no_srt;           /* number of SRT elements     */
	    Srt      *srt;              /* SRT                        */
					/* Srt info. is required only */
					/* for modify operation.      */
} Add_mod_at;           /* A D D / M O D I F Y  A T - E N T R Y       */


typedef struct {
	    char     *at_abbrv;         /* attribute abbreviation of  */
					/* attribute to be deleted    */
	    signed32  no_at;            /* number of AT elements      */
	    At       *at;               /* AT                         */
	    signed32  no_oct;           /* number of OCT elements     */
	    Oct      *oct;              /* OCT                        */
	    signed32  no_srt;           /* number of SRT elements     */
	    Srt     *srt;               /* SRT                        */
} Del_at;               /* D E L E T E  A T - E N T R Y               */


typedef struct {
	    Oct      *oct_entry;        /* OCT entry to be added      */
					/* or modified                */
	    signed32  no_at;            /* number of AT elements      */
	    At       *at;               /* AT                         */
	    signed32  no_oct;           /* number of OCT elements     */
	    Oct      *oct;              /* OCT                        */
	    signed32  no_srt;           /* number of SRT elements     */
	    Srt      *srt;              /* SRT                        */
} Add_mod_oct;          /* A D D / M O D I F Y  O C T - E N T R Y     */


typedef struct {
	    char     *oct_abbrv;        /* object class abbreviation  */
					/* of object class to be      */
					/* deleted                    */
	    signed32  no_oct;           /* number of OCT elements     */
	    Oct      *oct;              /* OCT                        */
	    signed32  no_srt;           /* number of SRT elements     */
	    Srt      *srt;              /* SRT                        */
} Del_oct;              /* D E L E T E  O C T - E N T R Y             */


typedef struct {
	    Srt      *srt_entry;        /* SRT entry to be added or   */
					/* modified                   */
	    signed32  no_at;            /* number of AT elements      */
	    At       *at;               /* AT                         */
	    signed32  no_oct;           /* number of OCT elements     */
	    Oct      *oct;              /* OCT                        */
	    signed32  no_srt;           /* number of SRT elements     */
	    Srt      *srt;              /* SRT                        */
} Add_mod_srt;          /* A D D / M O D I F Y  S R T - E N T R Y     */


typedef struct {
	    signed32 rule_nr;           /* structure rule no. to be   */
					/* deleted.		      */
	    signed32 no_srt;            /* number of SRT elements     */
	    Srt     *srt;               /* SRT                        */
} Del_srt;              /* D E L E T E  S R T - E N T R Y             */


typedef struct {
	    signed32         upd_mode;  /* update mode:               */
					/*     ADD_AT_ENTRY           */
					/*     MOD_AT_ENTRY           */
					/*     DEL_AT_ENTRY           */
					/*     ADD_OCT_ENTRY          */
					/*     MOD_OCT_ENTRY          */
					/*     DEL_OCT_ENTRY          */
					/*     ADD_SRT_ENTRY          */
					/*     MOD_SRT_ENTRY          */
					/*     DEL_SRT_ENTRY          */
	    union {
		Add_mod_at  *a_m_at;    /* "upd_mode" = ADD_AT_ENTRY  */
					/* "upd_mode" = MOD_AT_ENTRY  */
		Del_at      *d_at;      /* "upd_mode" = DEL_AT_ENTRY  */
		Add_mod_oct *a_m_oct;   /* "upd_mode" = ADD_OCT_ENTRY */
					/* "upd_mode" = MOD_OCT_ENTRY */
		Del_oct     *d_oct;     /* "upd_mode" = DEL_OCT_ENTRY */
		Add_mod_srt *a_m_srt;   /* "upd_mode" = ADD_SRT_ENTRY */
					/* "upd_mode" = MOD_SRT_ENTRY */
		Del_srt     *d_srt;     /* "upd_mode" = DEL_SRT_ENTRY */
	    } upd_val;
} S_upd_info;           /* S C H E M A  U P D A T E  I N F O          */


/**************************/
/* ACL -  H A N D L I N G */
/**************************/
			    /* access rights                          */
			    /* --> "aright"                           */
#define MPUBLIC       0                 /* Modify public    attributes*/
#define RSTANDARD     1                 /* Read   standard  attributes*/
#define MSTANDARD     2                 /* Modify standard  attributes*/
#define RSENSITIVE    3                 /* Read   sensitive attributes*/
#define MSENSITIVE    4                 /* Modify sensitive attributes*/


#define NOIDX_ACL     4                 /* number of distinguished    */
					/* names per access rights    */
					/* (in ACL-list)              */


typedef char  Dnint;                    /* a flag indicating how the  */
					/* DN in ACL-list is to be    */
					/* interpreted                */

typedef struct {
	    Name_string     dn;         /* Distinguished Name in ACL  */
	    Dnint           dn_int;     /* Distinguished Name inter-  */
					/* pretation                  */
} Obj;                                  /* ACL entry                  */


/********************************/
/* S H A D O W  H A N D L I N G */
/********************************/
				/* range of "t_mode" (-->"Upd_time")  */
#define UPD_HIGH        0               /* update times: minutes      */
#define UPD_MEDIUM      1               /* update times: hours        */
#define UPD_LOW_1       2               /* update times: day of week, */
					/*               + hours      */
#define UPD_LOW_2       3               /* update times: day of week, */
					/*               + hours      */
					/*      (two values for each) */


				/* range of "mode":                   */
				/* --> DS_GET_SHADOWING_JOB           */
#define READ_FIRST      0               /* reads first entry of the   */
					/* shadowing job file         */
#define READ_NEXT       1               /* reads next entry of the    */
					/* shadowing job file         */
#define TERMINATE       2               /* stops reading and closes   */
					/* the shadowing job file     */


				/* range of "upd_mode":               */
				/* --> DS_UPDATE_SHADOWING_JOB        */
#define ACTIVATE            0
#define DEACTIVATE          1
#define CHANGE_UPD_TIME     2
#define UPD_IMMEDIATELY     3
#define C_ACTIVATE          4   /* activate CACHE update job          */
#define C_DEACTIVATE        5   /* deactivate CACHE update job        */
#define C_CHANGE_UPD_TIME   6   /* change CACHE update job            */
#define C_UPD_IMMEDIATELY   7


				/* range of "state": (-->"Shd_job")   */
#define ACT_JOB     0x0001      /* active job in shadowing job file   */
#define NOT_ACT_JOB 0x0002      /* inactive job in shadowing job file */


				/* range of "mode"                    */
				/* (--> DS_GENERATE_SHADOWS)          */
				/* (--> DS_REMOVE_SHADOWS)            */
#define JOB_ONLY            0           /* creates/removes shadowing  */
					/* job only                   */
#define SHD_AND_JOB         1           /* creates/removes both       */
					/* shadows and shadowing job  */


				/* range of "subset"                  */
#define OBJECT              0           /* the object itself          */
#define OBJECT_SUBORDINATES 1           /* object and its subordinates*/


#define ALL_POSSIBLE_TIMES  -1          /* update times               */
					/*    at all possible values  */


typedef struct
	{   signed32 upd_t_mode;        /* mode of update time        */
					/* interpretation             */
	    signed32 upd_min;           /* only valid if "upd_t_mode" */
					/* is UPD_HIGH                */
					/* values: 5, 10, 15, 30      */
	    signed32 upd_hour;          /* only valid if "upd_t_mode" */
					/* is UPD_MEDIUM              */
					/* values: 1, 2, 4, 6, 12     */
	    signed32 upd_1d_of_week;    /* only valid if "upd_t_mode" */
					/* is UPD_LOW_1 or UPD_LOW_2  */
					/* values:  -1: every day of  */
					/*              the week      */
					/*      0 -  6: fixed day of  */
					/*              the week      */
					/*              0 = sunday,   */
	    signed32 upd_1hour;         /* only valid if "upd_t_mode" */
					/* is UPD_LOW_1 or UPD_LOW_2  */
					/* values: between 0..23      */
	    signed32 upd_2d_of_week;    /* only valid if "upd_t_mode" */
					/* is UPD_LOW_2               */
					/* values:                    */
					/*      0 -  6: fixed day of  */
					/*              the week      */
					/*              0 = sunday,   */
	    signed32 upd_2hour;         /* only valid if "upd_t_mode" */
					/* is UPD_LOW_2               */
					/* values: between 0..23      */
	} Upd_time;


typedef struct Subtree
	{   struct Subtree  *sub_next;  /* pointer to next subtree    */
					/* causing an error           */
	    Name_string      sub_name;  /* name of subtree            */
	    signed32         subset;    /* interpretation of subtree  */
	} Subtree;


typedef struct
	{   signed32 shd_state;         /* shadowing job state        */
	    signed32 shd_nr;            /* shadowing job number       */
	    Upd_time shd_time;          /* update times of shadowing  */
					/* job                        */
    /******************************************************************/
    /* additional parameters in case of DSA update jobs               */
    /******************************************************************/
	    char     shd_objname[DNL_MAX];  /* name of the object/   */
					    /* subtree to be updated  */
	    signed32 shd_subset;        /* object interpretation      */
	    char     shd_dsa[DNL_MAX];  /* name of the destination DSA*/
	    signed32 shd_idx_dsa;       /* index of DSA name in an    */
					/* internal list              */
	    signed32 shd_t_stamp;       /* time stamp of shadow       */
					/* creation                   */
	} Shd_job;


typedef struct
	{   signed32        serv_cntrls;     /* Service-Controls      */
	    signed16        priority;        /* priority;             */
					     /* in V2 not used        */
	    signed16        time_limit;      /* time limit in seconds */
	    signed16        size_limit;      /* number of entries     */
	} C_arg;                             /* COMMON-ARGUMENTS      */


/****************************/
/* T R E E  H A N D L I N G */
/****************************/
				/* range of "inherit"                 */
#define OLD_ACL      0
#define NEW_ACL      1

/********************************/
/* DEFAULT SCHEMA DEFINITIONS	*/
/********************************/

/********************************/
/* DEFAULT AT ENTRIES		*/
/********************************/

#define NO_DEF_AT	55	/* no. of entries in default At	*/

#define S_AT_ACL          "ACL"
#define S_AT_ACL_OBJ_ID   "43.12.2.1107.1.3.4.1"

#define S_AT_AON          "AON"
#define S_AT_AON_OBJ_ID   "85.4.1"

#define S_AT_AT           "AT"
#define S_AT_AT_OBJ_ID    "43.12.2.1107.1.3.4.6"

#define S_AT_BC           "BC"
#define S_AT_BC_OBJ_ID    "85.4.15"

#define S_AT_C            "C"
#define S_AT_C_OBJ_ID     "85.4.6"

#define S_AT_CDC          "CDC"
#define S_AT_CDC_OBJ_ID   "43.12.2.1107.1.3.4.13"

#define S_AT_CDR          "CDR"
#define S_AT_CDR_OBJ_ID   "43.12.2.1107.1.3.4.14"

#define S_AT_CN           "CN"
#define S_AT_CN_OBJ_ID    "85.4.3"

#define S_AT_DI           "DI"
#define S_AT_DI_OBJ_ID    "85.4.27"

#define S_AT_DSC          "DSC"
#define S_AT_DSC_OBJ_ID   "85.4.13"

#define S_AT_FTN          "FTN"
#define S_AT_FTN_OBJ_ID   "85.4.23"

#define S_AT_IIN          "IIN"
#define S_AT_IIN_OBJ_ID   "85.4.25"

#define S_AT_KNI          "KNI"
#define S_AT_KNI_OBJ_ID   "85.4.2"

#define S_AT_L            "L"
#define S_AT_L_OBJ_ID     "85.4.7"

#define S_AT_MDE          "MDE"
#define S_AT_MDE_OBJ_ID   "86.5.2.2"

#define S_AT_MDL          "MDL"
#define S_AT_MDL_OBJ_ID   "86.5.2.0"

#define S_AT_MDM          "MDM"
#define S_AT_MDM_OBJ_ID   "86.5.2.3"

#define S_AT_MDS          "MDS"
#define S_AT_MDS_OBJ_ID   "86.5.2.4"

#define S_AT_MDT          "MDT"
#define S_AT_MDT_OBJ_ID   "86.5.2.1"

#define S_AT_MEM          "MEM"
#define S_AT_MEM_OBJ_ID   "85.4.31"

#define S_AT_MK           "MK"
#define S_AT_MK_OBJ_ID    "43.12.2.1107.1.3.4.0"

#define S_AT_MMS           "MMS"
#define S_AT_MMS_OBJ_ID    "86.5.2.5"

#define S_AT_MOA           "MOA"
#define S_AT_MOA_OBJ_ID    "86.5.2.6"

#define S_AT_MPD           "MPD"
#define S_AT_MPD_OBJ_ID    "86.5.2.7"

#define S_AT_MSA           "MSA"
#define S_AT_MSA_OBJ_ID    "86.5.2.8"

#define S_AT_MSC           "MSC"
#define S_AT_MSC_OBJ_ID    "86.5.2.9"

#define S_AT_MSO           "MSO"
#define S_AT_MSO_OBJ_ID    "86.5.2.10"

#define S_AT_O            "O"
#define S_AT_O_OBJ_ID     "85.4.10"

#define S_AT_OCL          "OCL"
#define S_AT_OCL_OBJ_ID   "85.4.0"

#define S_AT_OCT          "OCT"
#define S_AT_OCT_OBJ_ID   "43.12.2.1107.1.3.4.5"

#define S_AT_OU           "OU"
#define S_AT_OU_OBJ_ID    "85.4.11"

#define S_AT_OWN          "OWN"
#define S_AT_OWN_OBJ_ID   "85.4.32"

#define S_AT_PA           "PA"
#define S_AT_PA_OBJ_ID    "85.4.16"

#define S_AT_PC           "PC"
#define S_AT_PC_OBJ_ID    "85.4.17"

#define S_AT_PDM          "PDM"
#define S_AT_PDM_OBJ_ID   "85.4.28"

#define S_AT_PDO          "PDO"
#define S_AT_PDO_OBJ_ID   "85.4.19"

#define S_AT_POB          "POB"
#define S_AT_POB_OBJ_ID   "85.4.18"

#define S_AT_PSA          "PSA"
#define S_AT_PSA_OBJ_ID   "85.4.29"

#define S_AT_RA           "RA"
#define S_AT_RA_OBJ_ID    "85.4.26"

#define S_AT_RO           "RO"
#define S_AT_RO_OBJ_ID    "85.4.33"

#define S_AT_SAC          "SAC"
#define S_AT_SAC_OBJ_ID   "85.4.30"

#define S_AT_SEA          "SEA"
#define S_AT_SEA_OBJ_ID   "85.4.34"

#define S_AT_SER          "SER"
#define S_AT_SER_OBJ_ID   "85.4.5"

#define S_AT_SG           "SG"
#define S_AT_SG_OBJ_ID    "85.4.14"

#define S_AT_SN           "SN"
#define S_AT_SN_OBJ_ID    "85.4.4"

#define S_AT_SPN          "SPN"
#define S_AT_SPN_OBJ_ID   "85.4.8"

#define S_AT_SRT          "SRT"
#define S_AT_SRT_OBJ_ID   "43.12.2.1107.1.3.4.4"

#define S_AT_STA          "STA"
#define S_AT_STA_OBJ_ID   "85.4.9"

#define S_AT_TIT          "TIT"
#define S_AT_TIT_OBJ_ID   "85.4.12"

#define S_AT_TN           "TN"
#define S_AT_TN_OBJ_ID    "85.4.20"

#define S_AT_TST          "TST"
#define S_AT_TST_OBJ_ID   "43.12.2.1107.1.3.4.2"

#define S_AT_TTI          "TTI"
#define S_AT_TTI_OBJ_ID   "85.4.22"

#define S_AT_TXN          "TXN"
#define S_AT_TXN_OBJ_ID   "85.4.21"

#define S_AT_UP           "UP"
#define S_AT_UP_OBJ_ID    "85.4.35"

#define S_AT_X1A          "X1A"
#define S_AT_X1A_OBJ_ID   "85.4.24"

/********************************/
/* DEFAULT OCT ENTRIES		*/
/********************************/

#define NO_DEF_OCT	22	/* no. of entries in default Oct	*/

#define S_OC_ALI               "ALI"
#define S_OC_ALI_OBJ_ID        "85.6.1"

#define S_OC_APE               "APE"
#define S_OC_APE_OBJ_ID        "85.6.12"

#define S_OC_APP               "APP"
#define S_OC_APP_OBJ_ID        "85.6.11"

#define S_OC_C                 "C"
#define S_OC_C_OBJ_ID          "85.6.2"

#define S_OC_DEV               "DEV"
#define S_OC_DEV_OBJ_ID        "85.6.14"

#define S_OC_DSA               "DSA"
#define S_OC_DSA_OBJ_ID        "85.6.13"

#define S_OC_GON               "GON"
#define S_OC_GON_OBJ_ID        "85.6.9"

#define S_OC_GTP               "GTP"
#define S_OC_GTP_OBJ_ID        "---"

#define S_OC_LOC               "LOC"
#define S_OC_LOC_OBJ_ID        "85.6.3"

#define S_OC_MDL               "MDL"
#define S_OC_MDL_OBJ_ID        "86.5.1.0"

#define S_OC_MMS               "MMS"
#define S_OC_MMS_OBJ_ID        "86.5.1.1"

#define S_OC_MTA               "MTA"
#define S_OC_MTA_OBJ_ID        "86.5.1.2"

#define S_OC_MUA               "MUA"
#define S_OC_MUA_OBJ_ID        "86.5.1.4"

#define S_OC_MUS               "MUS"
#define S_OC_MUS_OBJ_ID        "86.5.1.3"

#define S_OC_ORG               "ORG"
#define S_OC_ORG_OBJ_ID        "85.6.4"

#define S_OC_ORP               "ORP"
#define S_OC_ORP_OBJ_ID        "85.6.7"

#define S_OC_ORR               "ORR"
#define S_OC_ORR_OBJ_ID        "85.6.8"

#define S_OC_OU                "OU"
#define S_OC_OU_OBJ_ID         "85.6.5"

#define S_OC_PER               "PER"
#define S_OC_PER_OBJ_ID        "85.6.6"

#define S_OC_REP               "REP"
#define S_OC_REP_OBJ_ID        "85.6.10"

#define S_OC_SCH               "SCH"
#define S_OC_SCH_OBJ_ID        "43.12.2.1107.1.3.6.0"

#define S_OC_TOP               "TOP"
#define S_OC_TOP_OBJ_ID        "85.6.0"


#define DS_MIN_DEF_RULE_NR	1   /* minimum default rule nr.		 */
#define DS_MAX_DEF_RULE_NR	17  /* maximum default rule nr.		 */

#define APPEND_SUB		0	/* append subtree operation.	*/
#define CREATE_SHD		1	/* create shadow  operation.	*/
#define CHANGE_NAM		2	/* change name    operation.	*/
#define CHANGE_MAS		3	/* change master    operation.	*/

#endif /* _GDSEXT_H */
