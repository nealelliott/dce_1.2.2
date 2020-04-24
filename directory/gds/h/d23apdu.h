/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23apdu.h,v $
 * Revision 1.1.752.2  1996/02/18  23:35:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:41  marty]
 *
 * Revision 1.1.752.1  1995/12/08  16:00:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:37  root]
 * 
 * Revision 1.1.750.1  1994/02/22  18:48:56  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:44:05  marrek]
 * 
 * Revision 1.1.748.2  1993/10/14  17:27:43  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:26:32  keutel]
 * 
 * Revision 1.1.748.1  1993/10/13  17:32:30  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:26:17  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  12:21:39  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:38:54  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:40:04  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:29:14  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:09:12  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  09:31:48  marrek]
 * 
 * Revision 1.1.2.3  1992/07/06  17:21:21  melman
 * 	CR 4327: unaligned read/writes on shadow update
 * 	[1992/07/06  17:20:24  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:59:46  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:41:39  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D23APDU_H
#define _D23APDU_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d23apdu_rcsid[] = "@(#)$RCSfile: d23apdu.h,v $ $Revision: 1.1.752.2 $ $Date: 1996/02/18 23:35:36 $";
#endif

/*----------------------------------------------------------------------*/
/*									*/
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG		*/
/* All Rights Reserved							*/
/*									*/
/* File Name	: d23apdu.h						*/
/*									*/
/* Description	: This module contains definitions for the APDU format	*/
/*		  used for the data-transfer via IPC			*/
/*									*/
/* Date		: April 06, 1990					*/
/*									*/
/* Author	: Praveen Gupta						*/
/*	(newer stuff: Jochen Keutel, SNI BA NM 123, Munich, Germany)	*/
/*									*/
/*----------------------------------------------------------------------*/

#include <dce/dce.h>
#include <dce/d2dir.h>

/*********** constant for NOT_DEFINED ************/

#define D2_NOT_DEFINED    (-2)         

/*****   Version-Number   *****/

#define	D23_APDUMIN	(-111) 	      /* APDU Minimum Version  */
#define D23_APDUV201A   (-110)        /* Version 2.01A         */
#define D23_APDUV22     (-109)        /* Version 2.2         */

#define D23_APDUMAX     D23_APDUV22  /* APDU Maximum Version  */

/*******  Protocol Version   *******/

#define D23_V1988       1            /*  Protocol Version for 1988-std. */

/*****   Operation-ID  defines   *****/

#define D23_READ        (1)           /* Operation-ID: DS_READ          */
#define D23_COMPARE     (2)           /* Operation-ID: DS_COMPARE       */
#define D23_ABANDON     (3)           /* Operation-ID: For ABANDON      */
#define	D23_LIST	(4)  	      /* Operation-ID: DS_LIST          */
#define D23_SEARCH      (5)           /* Operation-ID: DS_SEARCH        */
#define D23_ADDOBJT     (6)           /* Operation-ID: DS_ADD_OBJECT    */
#define D23_REMOBJT     (7)           /* Operation-ID: DS_REMOVE_OBJECT */
#define D23_MODENTR     (8)           /* Operation-ID: DS_MODIFY_ENTRY  */
#define D23_REPLRDN     (9)           /* Operation-ID: DS_REPLACE_RDN   */
#define D23_BIND        (10)          /* Operation-ID: DS_BIND          */
#define D23_UNBIND      (11)          /* Operation-ID: DS_UNBIND        */

/********** Operation-ID for Chaining   *********/

#define D23_CH_READ        (1)           /* Operation-ID: DS_READ          */
#define D23_CH_COMPARE     (2)           /* Operation-ID: DS_COMPARE       */
#define D23_CH_ABANDON     (3)           /* Operation-ID: For ABANDON      */
#define D23_CH_LIST        (4)           /* Operation-ID: DS_LIST          */
#define D23_CH_SEARCH      (5)           /* Operation-ID: DS_SEARCH        */
#define D23_CH_ADDOBJT     (6)           /* Operation-ID: DS_ADD_OBJECT    */
#define D23_CH_REMOBJT     (7)           /* Operation-ID: DS_REMOVE_OBJECT */
#define D23_CH_MODENTR     (8)           /* Operation-ID: DS_MODIFY_ENTRY  */
#define D23_CH_REPLRDN     (9)           /* Operation-ID: DS_REPLACE_RDN   */


#define D23_GBIND       (12)          /* temporary : has to be deleted later */
#define D23_DREAD       (12)          /* Operation-ID: to read PSAP and      */
				      /* authentication mechanisms supported */
				      /* by the target DSA.		     */
				      /* (used by gdsdaemon)		     */

/********** Error Codes in DAP          *********/

#define D23A_ATT_ERROR           1
#define D23A_NAME_ERROR          2
#define D23A_SERV_ERROR          3
#define D23A_REFERRAL            4
#define D23A_ABAN_ERROR          5
#define D23A_SEC_ERROR           6
#define D23A_ABAN_FAILED         7
#define D23A_UPDATE_ERROR        8

/********** Error Codes in DSP          *********/

#define D23S_ATT_ERROR           1
#define D23S_NAME_ERROR          2
#define D23S_SERV_ERROR          3
#define D23S_ABAN_ERROR          5
#define D23S_SEC_ERROR           6
#define D23S_ABAN_FAILED         7
#define D23S_UPDATE_ERROR        8
#define D23S_DSA_REFERRAL        9

/********** Reasons for ROS-reject      *********/

#define D23R_DUP_INVID           0      /* These constants are used as*/
#define D23R_UNREC_OPER          1      /* an index of an array. They */
#define D23R_MISTYPE_ARG         2      /* should not change          */
#define D23R_RESOURCE_LIMIT      3      /* deliberately               */
#define D23R_MRESULT		10
#define D23R_MERROR		15

/* the credential type          */

#define D23_NO_CRED            (-1)
#define D23_SIMPLE_CRED        ( 0)
#define D23_STRONG_CRED        ( 1)
#define D23_EXTERNAL_CRED      ( 2)

/*****   APDU-Tags  defines   *****/

#define	D23_MINTAG 	(65001)       /* Tag:  lowest used tag          */
				      /*       FDE9                     */
#define	D23_NAMTAG 	(65001)       /* Tag:  Begin D23_ds_name Block  */
				      /*       FDE9                     */
#define	D23_BONTAG 	(65002)       /* Tag:  Begin D23_ds_name Block  */
                                      /*       (for Base-Object Name)   */
				      /*       FDEA                     */
#define D23_ATTTAG      (65003)       /* Tag:  Begin D23_at_type Block  */
				      /*       FDEB                     */
#define D23_AVTAG      (65004)       /* Tag:  Begin D23_av_info Block  */
				      /*       FDEC                     */
#define	D23_ENTTAG 	(65007)       /* Tag:  Begin D23_en_info Block  */
				      /*       FDEF                     */
#define	D23_REFTAG 	(65009)       /* Tag:  Begin D23_rf_info Block  */
				      /*       FDF1                     */
#define	D23_REQTAG 	(65011)       /* Tag:  Begin D23_rq_info Block  */
				      /*       FDF3                     */
#define	D23_MODTAG 	(65013)       /* Tag:  Begin D23_md_info Block  */
				      /*       FDF5                     */
#define	D23_PATTAG 	(65015)       /* Tag:  Begin D23_ft1_info Block  */
				      /*       FDF7                     */
#define D23_FIITAG      (65016)       /* Tag:  Begin D23_fi1_info Block */
				      /*       FDF8                     */
#define D23_PWDTAG      (65017)       /* Tag:  Begin D23_pw_info Block  */
				      /*       FDF9                     */
#define D23_FISTAG      (65018)       /* Tag:  Begin D23_fs1_info Block */
				      /*       FDFA                     */
#define D23_BITTAG      (65019)       /* Tag:  Begin D23_bit_info Block  */
				      /*       FDFB                     */
#define D23_TIMTAG      (65020)       /* Tag:  Begin D23_tm_info Block  */
				      /*       FDFC                     */
#define D23_RQTAG       (65021)       /* Tag:  For requestor as ds_name  */
				      /* and for originator in chain-arg */
				      /*       FDFD                     */
#define D23_SECTAG      (65022)       /* Tag:  Begin D23_se_info Block  */
				      /*       FDFE                     */
#define D23_ACCTAG      (65025)       /* Tag:  Begin D23_acc_info Block  */
				      /*       FE01                     */
#define D23_SIMTAG      (65026)       /* Tag:  Begin D23_simcr_info Block  */
				      /*       FE02                     */
#define D23_STRTAG      (65027)       /* Tag:  Begin D23_strcr_info Block  */
				      /*       FE03                     */
#define D23_EXTTAG      (65028)       /* Tag:  Begin D23_extcr_info Block  */
				      /*       FE04                     */
#define D23_CRSTAG      (65029)       /* Tag:  Begin D23_com_res Block  */
				      /*       FE05                     */
#define D23_VALTAG      (65030)       /* Tag:  Begin D23_val_info Block  */
				      /*       FE06                     */
#define D23_SOITAG      (65031)       /* Tag:  Begin D23_subord_info Block  */
				      /*       FE07                     */
#define D23_OQTAG       (65032)       /* Tag:  Begin D23_part_oq Block  */
				      /*       FE08                     */
#define D23_CREFTAG     (65033)       /* Tag:  Begin D23_cross-ref Block */
				      /*       FE09                     */
#define D23_TRTAG       (65034)       /* Tag:  Begin trace info. Block */
				      /*       FE0A                     */
#define D23_DMTAG       (65035)       /* Tag: Begin Domain info Block */
				      /*       FE0B                     */
#define D23_ITAG        (65036)       /* Tag Search Arg. Block */
				      /*       FE0C                     */
#define D23_LTAG        (65037)       /* Tag Compare Result Block */
				      /*       FE0D                     */
#define D23_DRFTAG      (65038)       /* Tag DSA-REFERRAL Block */
				      /*       FE0E                     */
#define D23_APTAG       (65039)       /* Tag Attr.-Error Block */
				      /*       FE0F                     */
#define D23_CMTAG       (65040)       /* Tag D23_com_arg Block */
				      /*       FE10                     */
#define D23_CHTAG       (65041)       /* Tag Chaining Block */
				      /*       FE11                     */
#define D23_CHRTAG      (65042)       /* Tag Chaining Result Block */
				      /*       FE12                     */
#define D23_REPTAG      (65043)       /* Tag ReplaceRDN arg. Block */
				      /*       FE13                     */
#define D23_DOMTAG      (65044)       /* Tag Domain Info. Block         */
				      /*       FE14                     */
#define D23_ALGTAG      (65045)       /* Tag D23_alg_id                 */
				      /*       FE15                     */
#define D23_SIGTAG      (65046)       /* Tag D23_signature              */
				      /*       FE16                     */
#define D23_FWDTAG      (65047)       /* Tag D23_certificate forward    */
				      /*       FE17                     */
#define D23_REVTAG      (65048)       /* Tag D23_certificate reverse    */
				      /*       FE18                     */
#define D23_RVCTAG      (65049)       /* Tag D23_rev_cert               */
				      /*       FE19                     */
#define D23_EOMTAG	(65535)	      /* Tag:  End of Message           */
				      /*       FFFF                     */

/*
**********************************************************************
*                                                                    *
*       Types of external authentication			     *
*                                                                    *
**********************************************************************
*/

#define D23_DCE_AUTH	0	/* dce authentication.		*/

/*
**********************************************************************
*                                                                    *
*       T Y P E D E F  -  D E F I N I T I O N S                      *
*                                                                    *
**********************************************************************
*/

typedef struct
  {
    unsigned16       d23_eomtag;      /* Tag End of Message  */
    unsigned16       d23_eomfil;      /* Filler  */
    signed32         d23_eom_oqoff;   /* offset from D23_part_oq-tag to */
				      /* end-of-apdu for search & list only*/
  } D23_eom ;

#define D23_S_EOM   sizeof(D23_eom)

typedef struct                           /* ATTRIBUTE TYPE             */
  {
    unsigned16         d23_atttag;            /* Tag Attribute-Type Block */
    unsigned16         d23_afil;                  /*  Filler */
    signed16           d23_atoff;             /* Offset to next element  */
    signed16           d23_atlen;           /* length of attribute type  */
    /* attribute type */
  } D23_at_type;

#define D23_S_ATT   sizeof(D23_at_type)

typedef struct                          /*  Attribute-Value Block     */
  {
    signed16          d23_avlen;          /* length of Attribute Value  */
    signed16          d23_avoff;          /* Offset of Attribute Value  */
    unsigned16          d23_avrep;        /* representation of the      */
					  /* attribute value            */
    unsigned16          d23_avfil;        /* filler                     */
    /* attribute value */
  } D23_av_value;

#define D23_S_ATV   sizeof(D23_av_value)

	/* No D23_av_info block is used for att-type-only case  */

typedef struct                            /*  Attribute-Value Block  */
  {
    unsigned16           d23_avtag;          /*  Attribute-Value Tag  */
    signed16           d23_avnum;          /* No. of Values (at least 1)*/
    unsigned16           d23_asyntx;         /*  Attribute-syntax */
    unsigned16           d23_afil;           /*  Filler */
    /* structure D23_av_value */             /* Seq. of Attribute Value */
  } D23_av_info;


#define D23_S_AV   sizeof(D23_av_info)

typedef struct                              /*  Bit-Value Block       */
  {
    unsigned16         d23_btag;              /* Tag for Bit string     */
    unsigned16         d23_bfil;              /* Filler                 */
    signed16           d23_boff;              /* Offset to next Element */
					      /* in bytes               */
    signed16           d23_blen;              /* Length of Bit-Value in */
					      /* bits                   */
    /* bit value */                           /* Bit Value */
  } D23_bit_info;

#define D23_S_BIT   sizeof(D23_bit_info)

typedef struct                              /*  Time-Value Block  */
  {
    unsigned16           d23_tmtag;              /* Tag for Time string */
    unsigned16           d23_tmfil;                       /*  Filler  */
    signed16             d23_tmoff;              /* Offset to next Element */
    signed16             d23_tmlen;           /* Length of Time-Value */
    /* Time value */                           /* Time Value */
  } D23_tm_info;

#define D23_S_TM   sizeof(D23_tm_info)

typedef struct                              /*  Service-control Block  */
  {
    signed32                d23_scopt;      /*  Service-Controls-options  */
    signed16            d23_scpri;                 /* Priority */
    signed16            d23_sctlm;                 /*  Time-Limit (opt.)  */
				 /* (D2_T_UNLIMITED for being NOT_DEFINED)  */
    signed16            d23_scslm;                 /*  Size-Limit (opt.) */
				 /* (D2_S_UNLIMITED for being NOT_DEFINED)  */
    signed16            d23_scref;       /* scope of referrals (optional) */
				 /* (D2_WORLD for being NOT_DEFINED)  */
   } D23_serv_ctrl;

typedef struct                             /*  Operation-progress Block  */
  {
    unsigned16           d23_opnm_rs;           /*  NameResolutionPhase */
					    /* D2_NOT_STARTED for default */
    unsigned16           d23_op_nxtrdn;         /*  NextRDNToBeResolved (opt.)*/
				   /*( 0 for being NOT_DEFINED) */
  } D23_op_pr_info;

typedef struct                              /*  Common-Argument Block  */
  {
    unsigned16           d23_cmtag;              /*  Tag Common-arg Block  */
    signed16            d23_cmali;              /* aliased RDN's (optional) */
				   /*(D2_NOT_DEFINED for being NOT_DEFINED) */
    D23_serv_ctrl    d23_cmctr;              /*  Service-Controls  */
    D23_op_pr_info   d23_cmopr;              /*  Operation progress */
    /* structure D23_se_parm */             /*  Security Parameters (opt.)*/
    /* structure D23_ds_name */     /* Distinguished Name of requestor*/
				    /* (opt) requires a seperate tag  */
				    /* D23_RQTAG                      */
    /* structure D23_bit_info */    /* critical extensions (opt)      */
  } D23_com_arg;

#define D23_S_CARG   sizeof(D23_com_arg)

typedef struct                                 /*  Security Parameters */
  {
    unsigned16           d23_setag;             /* Tag Security info Block */
    unsigned16           d23_sefil;                            /*  Filler  */
    /* not yet implemented              */
  } D23_se_parm;

#define D23_S_SEP   sizeof(D23_se_parm)

typedef struct                          /*  Distinguished-Name Block  */
  {
    unsigned16           d23_dstag;         /* Tag Distinguished Name     */
    Bool             d23_dscomplete;    /* Complete flag              */
    signed16           d23_dsoff;         /* Offset to next Element     */
    signed16           d23_dslen;         /* Length of Name-Value       */
					/* 0 for ROOT                 */
    /* decoded part of name value */
    /* encoded part of name value (d23_av_value) */
  } D23_ds_name;

#define D23_S_NM   sizeof(D23_ds_name)

typedef struct                           /* Password Information Block */
  {
    unsigned16           d23_pwtag;             /* Tag Password-Inf. Block */
    unsigned16           d23_pwfil;                            /*  Filler  */
    signed16           d23_pwoff;              /* Offset to next Element */
    signed16           d23_pwlen;              /* Length of Passwd-Value */
    /* Password value */                             /* Password Value */
  } D23_pw_info;

#define D23_S_PW   sizeof(D23_pw_info)

/* typedef struct  */                    /*  Attribute-Information Block  */
/*  {  */
    /* structure D23_at_type */      /* Attribute Type  */
    /* structure D23_av_info */     /* Seq. of Attribute Values        */
				    /* in case of recurring attributes */
				    /* No D23_av_info block is used    */
				    /* for att-type-only case     */
/*  } D23_at_info;  */

typedef struct                            /*  Entry-Information Block  */
  {
    unsigned16           d23_entag;             /* Tag Entry-Inform. Block */
    Bool             d23_enmas;             /* from-Entry              */
    signed16 	     d23_enmaxat;        /* Max. No. of D23_at_info,   */
					 /* needed only for STUBS      */
    unsigned16           d23_enfil;          /* Filler */
    /* structure D23_ds_name */                /*  Distinguished Name  */
    /* structure D23_at_info */       /* Sequence of Attribute-Inform. */
  } D23_en_info;

#define D23_S_EN   sizeof(D23_en_info)

typedef struct                              /*  Common result Block  */
  {
    unsigned16        d23_crtag;
    Bool          d23_ali_deref;            /* Alias Deref.   */
    /*structure D23_ds_name   */            /* Requestor Block (opt.) */
    /* structure D23_se_parm */            /*  Security Parameters (opt.) */
  } D23_com_res;

#define D23_S_CRES   sizeof(D23_com_res)

typedef struct                              /*  Access-point Block  */
  {
    unsigned16           d23_acctag;             /* Tag Access-point Block */
    unsigned16           d23_accfil;             /*  Filler  */
    /* structure D23_ds_name */              /*  DSA - Name  */
    /* structure D23_av_value */             /*  DSA - PSAP Address  */
  } D23_acc_info;

#define D23_S_ACC   sizeof(D23_acc_info)

typedef struct
  {
   unsigned16           d23_rftag;               /* Tag Refr.-Inform. Block */
   signed16           d23_rf_alirdn;           /* No. of aliasedRDN (opt.)
				   (D2_NOT_DEFINED for being NOT_DEFINED) */
   signed16           d23_rf_rdnres;           /* No. of RDNs Resolved (opt.)
				   (D2_NOT_DEFINED for being NOT_DEFINED) */
   signed16           d23_rf_type;             /* Reference Type    (opt.)
				   (D2_NOT_DEFINED for being NOT_DEFINED) */
   D23_op_pr_info   d23_op_pr;               /* Operation Progress     */
   signed16 	    d23_nm_acc;              /* No. of access points */
   Bool             d23_rf_entry_only;       /* indicates whether     */
					     /* the subset parameter  */
					     /* must be reset         */
   /* structure D23_ds_name */               /* Name of Target-object  */
   /* structure D23_acc_info  */             /* access-points in sequence */
  } D23_rf_info;

#define D23_S_REF   sizeof(D23_rf_info)

typedef struct                            /*  Requ.-Information Block  */
  {
    unsigned16           d23_rqtag;             /* Tag Requ.-Inform. Block */
    unsigned16           d23_rqfil;                            /*  Filler  */
    D2_ret_val       d23_rqreq;               /*  Type of Information  */
    signed16            d23_rqnbr;           /* Number of Attribute-Types */
    /* structure D23_at_type */              /*  Attribute-Types  */
  } D23_rq_info;

#define D23_S_REQ   sizeof(D23_rq_info)

typedef struct                     /*  Modify-Entry Information Block  */
  {
    unsigned16           d23_mdtag;              /* Tag Modify-Entry Block */
    D2_attr_mod      d23_mdmod;               /*  Type of Modification */
    /* structure D23_at_type */               /*  Attribute type */
    /* structure D23_av_info */               /*  Attribute values */
					   /* present only if modification */
					   /* != D2_DELATT                 */
					   /* if modification == D2_REP_AV */
					   /* then there has to be an even */
					   /* no of values (pairs: old val */
					   /* new val)                     */
  } D23_md_info;


#define D23_S_MOD   sizeof(D23_md_info)

typedef struct                  /*  Filter Information Block */
  {
    unsigned16           d23_fp1tag;           /* Tag Filter-Pattern Block */
    D2_a_opmod       d23_fp1mod;            /* Choice of operation mode */
    signed16           d23_fp1count;               /*  count of filters  */
    unsigned16           d23_fp1filler;                         /* Filler  */
    /* Filter Information Block or */
    /* Filter Item Block       */
  } D23_ft1_info;

#define D23_S_FT1   sizeof(D23_ft1_info)

typedef struct                  /*  Filter Item Block (V 2.01A) */
  {
    unsigned16           d23_fi1tag;              /* Tag Filter Item Block */
    D2_a_match       d23_fi1match;                    /* matching rule */
    unsigned16           d23_fi1syntx;                /* Filter-Item syntax */
    unsigned16           d23_filler;                  /*  Filler */
    /* structure D23_at_type */               /*  Attribute type */
    /* Filter d23_av_value or */
    /* Filter Substrings Block */
  } D23_fi1_info;

#define D23_S_FI1   sizeof(D23_fi1_info)

typedef struct                          /*  Filter Substrings Block   */
  {
    unsigned16          d23_fs1tag;         /* Tag Filter Substrings Block*/
    signed16           d23_fs1position;    /* position of substring      */
    signed16          d23_fs1len;         /* length of substring        */
    signed16          d23_fs1off;         /* offset to next element     */
    unsigned16          d23_fs1rep;         /* representation of the      */
					/* substring value            */
    unsigned16          d23_fs1fil;         /* filler                     */
    /* substring value */
  } D23_fs1_info;
    /* Filter Substrings Block */

#define D23_S_FS1   sizeof(D23_fs1_info)

typedef struct                              /*   Validity  Block   */
  {
    unsigned16           d23_valtag;            /* Tag Validity Block */
    unsigned16           d23_valfil;            /* Filler */
    /* structure     d23_tm_info  */        /* time1 (opt.)*/
    /* structure     d23_tm_info  */        /* time2 (opt.)*/
    /* structure     d23_bit_info */        /* Random1 (opt.) */
    /* structure     d23_bit_info  */       /* Random2 (opt.)*/
   } D23_val_info;

#define D23_S_VAL   sizeof(D23_val_info)

typedef struct                              /*   Simple Cred. Block   */
  {
    unsigned16           d23_scrtag;        /* Tag Simple Credentials Block */
    unsigned16           d23_scrfil;        /* Filler */
    /* structure D23_val_info */        /*  Validity (opt.) */
    /* structure D23_ds_name */         /*  User Name  */
					/* D23_ds_name is not present for
					   Anonymous user */
    /* structure D23_pw_info */         /*  User Password (opt.) */
  } D23_simcr_info;

#define D23_S_SIMCR   sizeof(D23_simcr_info)

typedef struct                              /*   Strong Cred. Block   */
  {
    unsigned16           d23_strtag;        /* Tag Strong Credentials Block */
    unsigned16           d23_strfil;        /* Filler */
    signed16	     	 d23_strlen;	/* length of following ASN.1 encoded */
					/* security token.		     */
    signed16	     	 d23_stroff;	/* offset to next element.	     */
    /* ASN.1 encoded security token.	*/	
  } D23_strcr_info;

#define D23_S_SSCR   sizeof(D23_strcr_info)

typedef struct                              /*   External Cred. Block   */
  {
    unsigned16           d23_exttag;        /* Tag External Credentials Block */
    signed16	     d23_extkind;	/* kind of external method used   */
					/* until now DCE.		  */
    signed16	     d23_extlen;	/* length of following security   */
					/* token.			  */
    signed16	     d23_extoff;	/* offset to next element.	  */
    /* External Authenticated security token.	*/
  } D23_extcr_info;

#define D23_S_EXTCR   sizeof(D23_extcr_info)

typedef struct                              /*   DS_BIND Invoke-APDU   */
  {
    signed16           d23_Aversno;                /* Invoke-APDU Version */
    signed16          d23_Adirid;                 /*   Directory-ID   */
    unsigned16          d23_Afil;                   /* Filler */
    signed16          d23_Acrkind;                 /* Credential Kind */
						/*  D23_NO_CRED       */
						/*  D23_SIMPLE_CRED   */
						/*  D23_STRONG_CRED   */
						/*  D23_EXTERNAL_CRED */
    /* structure D23_simcr_info */              /*  Simple cred. */
    /* structure D23_strcr_info */              /*  Strong cred. */
    /* structure D23_extcr_info */              /*  External cred. */
    /* structure D23_av_value */                /*  DSA-PSAP-Address  */
    /* structure D23_eom */                     /* Tag End of Message */
  } D23_inbind;

#define D23_S_INB   sizeof(D23_inbind)

typedef struct                            /*   Invoke-APDU Header  */
  {
    signed16           d23_versno;                /* Invoke-Data Version */
    signed16           d23_filler;                /* Filler   */
    signed16          d23_dirid;                 /*   Directory-ID   */
    signed16          d23_bindid;                /*    Bind-ID     */
  } D23_header;

#define D23_S_HDR   sizeof(D23_header)

typedef struct                              /*   DS_BIND Result-APDU   */
  {
    D23_header      d23_Bhdr;                   /*   Invoke-Header APDU  */
    signed16          d23_Bversion;               /*   Protocol Version   */
    signed16           d23_Bcrkind;                /* Credential Kind */
						/* credential kind has to be */
						/* same as being given in    */
						/* inbind message */
    /* structure D23_simcr_info */              /*  Simple cred. */
    /* structure D23_strcr_info */              /*  Strong cred. */
    /* structure D23_extcr_info */              /*  External cred. */
    /* structure D23_eom */                      /* Tag End of Message */
  } D23_rsbind;

#define D23_S_RSB   sizeof(D23_rsbind)

typedef struct                            /*   DS_UNBIND Invoke-APDU   */
  {
    D23_header      d23_Chdr;                   /* APDU-Header        */
    D23_eom         d23_Ceom;                   /* Tag End of Message */
  } D23_inunbind;

#define D23_S_INUNB   sizeof(D23_inunbind)

typedef struct                            /*   DS_UNBIND Result-APDU   */
  {
    D23_eom         d23_Deom;                    /* Tag End of Message */
  } D23_rsunbind;

#define D23_S_RSUNB   sizeof(D23_rsunbind)

typedef struct                              /*   DS_READ Invoke-APDU   */
  {
    D23_header          d23_Ehdr;                   /*  APDU-Header  */
    /* structure D23_chain_arg */		 /* Chaining Args     */
						 /* present only for DSP */
    /* structure D23_com_arg */                  /*  Common-Arguments  */
    /* structure D23_ds_name */                     /*  Entry-Name  */
    /* structure D23_rq_info */             /*  Requested-Information  */
    /* structure D23_eom */                      /* Tag End of Message */
  } D23_inread;

#define D23_S_INR   sizeof(D23_inread)

/*
typedef struct  */                         /*   DS_READ Result-APDU    */
/* { */
    /* structure D23_chain_res */		/* Chaining Result in  */
						/* the case of DSP     */
    /* structure D23_en_info */                 /*  Entry-Information  */
    /* structure D23_com_res */                 /*  Common-Result info */
    /* structure D23_eom */                      /* Tag End of Message */
/* } D23_rsread;   */


typedef struct                             /*    DS_LIST Invoke-APDU   */
  {
    D23_header      d23_Ghdr;                /* Invoke-APDU Header */
    /* structure D23_chain_arg */		 /* Chaining Args     */
						 /* present only for DSP */
    /* structure D23_com_arg */                  /*  Common-Arguments  */
    /* structure D23_ds_name */                      /*   Parent-Node  */
    /* structure D23_eom */                      /* Tag End of Message */
  } D23_inlist;

#define D23_S_INL   sizeof(D23_inlist)

typedef struct                             /* Subordinate Block */
   {
     unsigned16         d23_sotag;              /* Tag Subordinate Block */
     unsigned16         d23_sofil;              /* Filler  */
     Bool           d23_soali;              /* Alias Entry */
     Bool           d23_somas;              /* From Entry */
     /* structure D23_ds_name */            /* RDN of subordinate */
    } D23_subord_info; /* Sequence of Subordinate Info. */

#define D23_S_SUBORD   sizeof(D23_subord_info)

typedef struct                              /* Part_oq Block  */
  {
    unsigned16         d23_oqtag;              /* Tag Part_oq Block */
    Bool           d23_oq_uncrit;          /* Uncritical Extention */
    signed16         d23_oq_limit;           /*  Limit Problem  (opt.)
				   (D2_NO_LIMIT_EX for being NOT_DEFINED) */
    unsigned16         d23_oqfil;              /* Filler */
    /* structure D23_rf_info */            /* Seq. of Referral-Inform. */
   } D23_part_oq;

#define D23_S_POQ   sizeof(D23_part_oq)

/*
typedef struct  */                        /*    DS_LIST Result-APDU    */ 
/* { */
    /* structure D23_chain_res */		/* Chaining Result in  */
						/* the case of DSP     */
    /* structure D23_ds_name */         /*  Base-Object Name (opt.) */
    /* structure D23_subord_info */     /* Seq. of Subordinate info.   */
    /* structure D23_part_oq */        /* Partial outcome qualifier (opt.)*/
    /* structure D23_com_res */                 /*  Common-Result info */
    /* structure D23_eom */                      /* Tag End of Message */
/* } D23_rslist;  */

typedef struct                             /*   DS_SEARCH Info  */
  {
    unsigned16          d23_Itag;		      /* Tag for Search-Info */
    unsigned16          d23_Ifiller;              /*  Filler  */
    Bool            d23_Iali;                  /* Search aliases       */
    D2_subset       d23_Iobjsub;              /*  Base-Object Subset   */
   } D23_srcharg;

#define D23_S_SCHARG            sizeof(D23_srcharg)

typedef struct                             /*   DS_SEARCH Invoke-APDU  */
  {
    D23_header      d23_Ihdr;                /* Invoke-APDU Header */
    /* structure D23_chain_arg */		 /* Chaining Args     */
						 /* present only for DSP */
    /* structure D23_com_arg */                  /*  Common-Arguments  */
    /* structure D23_srcharg */			 /*  Search information */
    /* structure D23_ds_name */                  /*  Base-Object Name  */
    /* structure D23_rq_info */             /*  Requested Information  */
    /* structure D23_ft1_info */           /* Filterpattern Information */
    /* structure D23_eom */                      /* Tag End of Message */
  } D23_insearch;

#define D23_S_INSRCH   sizeof(D23_insearch)

/*
typedef struct  */                        /*   DS_SEARCH Result-APDU   */
/* {  */
    /* structure D23_chain_res */		/* Chaining Result in  */
						/* the case of DSP     */
    /* structure D23_ds_name */            /*  Base-Object Name (opt.) */
    /* structure D23_en_info */      /* Sequence of Entry-Informations */
    /* structure D23_part_oq */        /* Partial outcome qualifier (opt.)*/
    /* structure D23_com_res */                 /*  Common-Result info */
    /* structure D23_eom */                      /* Tag End of Message */
/* } D23_rssearch;  */

typedef struct                           /*   DS_COMPARE Invoke-APDU   */
  {
    D23_header        d23_Khdr;                /* Invoke-APDU Header */
    /* structure D23_chain_arg */		 /* Chaining Args     */
						 /* present only for DSP */
    /* structure D23_com_arg */                  /*  Common-Arguments  */
    /* structure D23_ds_name */                       /*   Entry-Name  */
    /* structure D23_at_info */                /*  Compared Attribute  */
    /* structure D23_eom */                      /* Tag End of Message */
  } D23_incompare;

#define D23_S_INCOMP   sizeof(D23_incompare)

typedef struct
  {
    unsigned16          d23_Ltag;                    /* Compare Res_Info APDU */
    unsigned16          d23_Lfil;			 /*   Filler       */
    Bool            d23_Lmatsgn;                 /*   Match-Sign    */
    Bool            d23_Lfrmen;                  /*   FromEntry     */
   } D23_rscompinfo;                             /* Compare-Result Info */

#define D23_S_COMRS   sizeof(D23_rscompinfo)

/* typedef struct */                      /*   DS_COMPARE Result-APDU  */
/*{  */
    /* structure D23_chain_res */		/* Chaining Result in  */
						/* the case of DSP     */
    /* structure D23_rscompinfo; */             /* Compare-Result Info */
    /* structure D23_ds_name */               /* Distinguished Name    */
			 /* contains DN only if Alias was dereferenced */
			 /* contains never attribute information       */
    /* structure D23_com_res */                 /*  Common-Result info */
    /* structure D23_eom */                      /* Tag End of Message */
/* } D23_rscompare; */

typedef struct                              /* DS_ABANDON Invoke-APDU  */
  {
    D23_header          d23_Rhdr;                   /*  APDU-Header  */
    signed32                d23_Rinv_id;             /* invoke ID         */
    /* structure D23_eom */                      /* Tag End of Message */
  } D23_inabandon;

#define D23_S_INA   sizeof(D23_inabandon)

/* typedef */                             /* DS_ABANDON    Result-APDU */
/* { */
    /* structure D23_eom */                      /* Tag End of Message */
/* } D23_rsabandon;  */

typedef struct                            /* DS_ADD_OBJECT Invoke-APDU */
  {
    D23_header     d23_Mhdr;                /* Invoke-APDU Header */
    /* structure D23_chain_arg */		 /* Chaining Args     */
						 /* present only for DSP */
    /* structure D23_com_arg */                  /*  Common-Arguments  */
    /* structure D23_ds_name */             /*  DN of new Object       */
    /* structure D23_at_info */      /* Sequence of Attribute information  */
    /* structure D23_eom */                      /* Tag End of Message */
  } D23_inaddobjt;

#define D23_S_INADD   sizeof(D23_inaddobjt)

/* typedef */                             /* DS_ADD_OBJECT Result-APDU */
/* { */
    /* structure D23_chain_res */		/* Chaining Result in  */
						/* the case of DSP     */
    /* structure D23_eom */                      /* Tag End of Message */
/* } D23_rsaddobjt;  */

typedef struct                         /* DS_MODIFY-ENTRY Invoke-APDU  */
  {
    D23_header     d23_Nhdr;                /* Invoke-APDU Header */
    /* structure D23_chain_arg */		 /* Chaining Args     */
						 /* present only for DSP */
    /* structure D23_com_arg */                  /*  Common-Arguments  */
    /* structure D23_ds_name */                      /*   Entry-Name   */
    /* structure D23_md_info */             /*  Seq. of Attr.-Info.  */
    /* structure D23_eom */                    /*  End of Message Tag  */
  } D23_inmodentr;

#define D23_S_INMOD   sizeof(D23_inmodentr)

/* typedef   */                         /* DS_MODIFY_ENTRY Result-APDU */
/* { */
    /* structure D23_chain_res */		/* Chaining Result in  */
						/* the case of DSP     */
    /* structure D23_eom */                      /* Tag End of Message */
/* } D23_rsmodentr; */

typedef struct                           /* DS_REPLACE_RDN Information */
  {
    unsigned16          d23_reptag;			/* Tag REP_RDN Info. */
    Bool            d23_delordn;                /* DeleteOldRDN   */
    /* structure D23_ds_name */                      /*   Object-Name   */
    /* structure D23_ds_name */                 /*  New Rel.Dist.Name  */
   } D23_repinfo;	

#define D23_S_REP               sizeof(D23_repinfo)

typedef struct                           /* DS_REPLACE_RDN Invoke-APDU */
  {
    D23_header      d23_Ohdr;                /* Invoke-APDU Header */
    /* structure D23_chain_arg */		 /* Chaining Args     */
						 /* present only for DSP */
    /* structure D23_com_arg */                  /*  Common-Arguments  */
    /* structure D23_repinfo */			 /* REP_RDN Info      */
    /* structure D23_eom */                    /*  End of Message Tag  */
  } D23_in_replrdn;

#define D23_S_RPRDN   sizeof(D23_in_replrdn)

/* typedef  */                           /* DS_REPLACE_RDN Result-APDU */
/* { */
    /* structure D23_chain_res */		/* Chaining Result in  */
						/* the case of DSP     */
    /* structure D23_eom */                      /* Tag End of Message */
/* } D23_rs_replrdn; */

typedef struct                         /* DS_REMOVE_OBJECT Invoke-APDU */
  {
    D23_header    d23_Phdr;                /* Invoke-APDU Header */
    /* structure D23_chain_arg */		 /* Chaining Args     */
						 /* present only for DSP */
    /* structure D23_com_arg */                  /*  Common-Arguments  */
    /* structure D23_ds_name */                      /*   Entry-Name   */
    /* structure D23_eom */                    /*  End of Message Tag  */
  } D23_inremobjt;

#define D23_S_REMOBJ   sizeof(D23_inremobjt)

/* typedef  */                         /* DS_REMOVE_OBJECT Result-APDU */
/* { */
    /* structure D23_chain_res */		/* Chaining Result in  */
						/* the case of DSP     */
    /* structure D23_eom */                      /* Tag End of Message */
/* } D23_rsremobjt; */

typedef struct {                            /* Attribute-Problem APDU */
    unsigned16          d23_aptag;              /* D23_at_prob TAG */
    signed16           d23_apval;              /* Attbibute error code */
    /* structure D23_at_type */             /* Attrb.-type          */
    /* structure D23_av_info */             /* Attrb.-value (opt.,  */
					    /* at most 1)           */
   } D23_at_prob;                     

#define D23_S_ATPROB   sizeof(D23_at_prob)

typedef struct                                  /*  ERROR Result-APDU  */
  {
    signed16           d23_Zretcod;                      /*  Return-Code  */
    signed16           d23_Zerrcls;                      /*  Error-Class  */
    signed16           d23_Zerrval;                      /*  Error-Value  */
    signed16          d23_Zversion;                    /* Protocol version */
    /*  structure D23_ds_name */            /* Entry-name for Name-err */
					    /* or Attribute-error      */
    /* structure D23_at_prob */             /* Seq. of Attr. Problem  */
    /* structure D23_drf_info */            /* DSA-Referral-Information*/
    /* structure D23_rf_info */                    /* Referral-Inform. */
    /* signed32              */                 /* Invoke ID that could   */
					    /* not be abandoned       */
    /* structure D23_eom */                    /*  End of Message Tag  */
  } D23_rserror;

#define D23_S_RSERR   sizeof(D23_rserror)

			/* Invoke APDU of daemon process to get       */
			/* bind message (D23_inbind) to remote DSA    */
			/* from local DSA                             */
typedef struct
  {
     signed16          d23_Qversno;              /* Invoke-APDU Version  */
     signed16         d23_Qdirid;               /* Directory-ID         */
     /* structure D23_ds_name */              /* distinguished name   */
					      /* of remote DSA        */
     /* structure D23_eom     */              /* Tag End of message   */
  } D23_ingbind;

#define D23_S_GBND   sizeof(D23_ingbind)


/*****************************************************************
	    NEW APDU'S FOR DSP PROTOCOL
*****************************************************************/

typedef struct                      /* Cross-ref. Block */
   {
    unsigned16    d23_creftag;          /* Tag cross-ref Block */
    unsigned16    d23_crfil;            /* Filler   */
    /*  structure D23_ds_name */    /* Contex prefix */
    /*  structure D23_acc_info */   /* access point  */
    } D23_cross_ref;

#define D23_S_CREF   sizeof(D23_cross_ref)

typedef struct                      /* Trace info. Block  */
   {
    unsigned16    d23_trtag;            /* Tag trace info. */
    unsigned16    d23_trfil;            /* Filler  */
    D23_op_pr_info d23_op_prog;     /* Operation Progress */
    /* structure D23_ds_name */     /* DSA Name   */
    /* structure D23_ds_name */     /* Target-Object Name (Optional) */
    } D23_trace_info;

#define D23_S_TRACE   sizeof(D23_trace_info)

typedef struct                                 /*  Domain Parameters */
  {
    unsigned16           d23_dmtag;             /* Tag D23_dom_info Block */
    unsigned16           d23_dmfil;                            /*  Filler  */
    /* not yet implemented  */
  } D23_dom_info;

#define D23_S_DM   sizeof(D23_dom_info)

typedef struct                      /* Chaining Arg. info. Block  */
   {
    unsigned16      d23_chtag;          /* Tag Chaining Arg. */
    Bool        d23_al_deref;       /* aliasDereferenced Flag */
    signed16      d23_al_RDN;         /* aliasedRDNs Flag (opt.)
				   (D2_NOT_DEFINED for being NOT_DEFINED) */
    Bool	d23_entry_only;	    /* entryOnly   */
    Bool        d23_ret_cref;       /* returnCrossRefs   */
    signed16      d23_ref_type;       /* ReferenceType (Default superior) */
    signed16       d23_chtmlt;         /* Time-Limit  
				   (D2_T_UNLIMITED for being NOT_DEFINED) */
    D23_op_pr_info d23_op_prog;     /* Operation Progress */
    signed16      d23_max_tr;         /* Max no of Trace info. */
    /* structure D23_trace_info */  /* Sequence of Trace information   */
    /* structure D23_ds_name */     /* Originator with RQTAG (Opt.)  */
    /* structure D23_ds_name */     /* targetObject with BONTAG (Opt.) */
    /* structure D23_dom_info */     /* Domaininfo   (Optional) */
    /* structure D23_se_parm  */    /* Security Parameter (Optional) */
    /* ANY OPERATION ARGUMENTS */   /* Arguments of Read, Search & Modify
				     * Oper. Class identified by op_id of
				     * & prot_id of IPC-Table */
    } D23_chain_arg;

#define D23_S_CHARG   sizeof(D23_chain_arg)

typedef struct                      /* Chaining Result Block */
  {
    unsigned16      d23_chrtag;         /* Tag Chaining Result Block */
    unsigned16      d23_max_cref;       /* Max no. of cross refs. */
    /* structure D23_dom_info  */   /* Domain-info (optional) */
    /* structure D23_cross_ref */  /* Sequence of Cross Referrals */
    } D23_chain_res;

#define D23_S_CHRES   sizeof(D23_chain_res)

typedef struct
  {
    unsigned16      d23_drftag;
    signed16      d23_drf_max_tr;
    /* structure D23_rf_info */     /* Continuation Referance */
    /* structure D23_ds_name */     /* Context_prefix OPTIONAL  */
				    /* to be defined only if Con_ref points*/
				    /* to a subordinate/cross_ref */
    /* structure D23_trace_info */  /* Trace Information */
	} D23_drf_info;    /* DSA-REFERRAL */

#define D23_S_DREF   sizeof(D23_drf_info)

#define D23_NO_PARAMETERS  0
#define D23_INT_PARAMETERS 1

typedef struct
  {
    unsigned16      d23_algtag;         /* algorithm identifier tag       */
    signed16      d23_algpar;         /* indicates kind of parameters   */
    signed16      d23_algoff;         /* offset to parameters           */
    signed16      d23_alglen;         /* length of object identifier    */
    /* object identifier of algorithm   */
    /* parameters of the algorithm      */
  } D23_alg_id;                     /* algorithm identifier           */

#define D23_S_ALGID  sizeof(D23_alg_id)

typedef struct
  {
    unsigned16      d23_sigtag;         /* signature tag                  */
    unsigned16      d23_sigfil;         /* filler bytes                   */
    /* structure D23_alg_id             */
    /* structure D23_bit_info           */
  } D23_signature;                  /* algorithm identifier           */

#define D23_S_SIGNATURE sizeof(D23_signature)

typedef struct
  {
    unsigned16      d23_cert_tag;       /* forward or reverse tag         */
    unsigned16      d23_cert_fil;       /* filler bytes                   */
    /* signed32                                version                    */
    /* signed32                                serial number              */
    /* structure D23_alg_id                signature                  */
    /* structure D23_ds_name               issuer                     */
    /* structure D23_tm_info               not before                 */
    /* structure D23_tm_info               not after                  */
    /* structure D23_ds_name               subject                    */
    /* structure D23_alg_id                algorithm                  */
    /* structure D23_bit_info              subject_key                */
  } D23_certificate;                /* certificate                    */

#define D23_S_CERTIFICATE sizeof(D23_certificate)

typedef struct
  {
    unsigned16      d23_rc_tag;         /* revoked certificate tag        */
    unsigned16      d23_rc_fil;         /* filler bytes                   */
    signed32        d23_rc_ucert;       /* user certificate identifier    */
    /* structure D23_alg_id                signature                  */
    /* structure D23_ds_name               issuer                     */
    /* structure D23_tm_info               revocation date            */
  } D23_rev_cert;                   /* certificate                    */

#define D23_S_REV_CERT sizeof(D23_rev_cert)

#endif /* _D23APDU_H */
