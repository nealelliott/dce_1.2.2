/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ros.h,v $
 * Revision 1.1.764.2  1996/02/18  23:36:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:01  marty]
 *
 * Revision 1.1.764.1  1995/12/08  16:02:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:40  root]
 * 
 * Revision 1.1.762.2  1994/03/23  15:18:44  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:09  keutel]
 * 
 * Revision 1.1.762.1  1994/02/22  18:08:30  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:50:24  marrek]
 * 
 * Revision 1.1.760.2  1993/10/14  17:35:45  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:33:31  keutel]
 * 
 * Revision 1.1.760.1  1993/10/13  17:32:55  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:15:53  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  13:07:29  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:46:03  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:43:35  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:31:18  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:51:53  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:39:27  melman]
 * 
 * $EndLog$
 */
#ifndef _ROS_H
#define _ROS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _ros_rcsid[] = { "@(#) $RCSfile: ros.h,v $ $Revision: 1.1.764.2 $ $Date: 1996/02/18 23:36:26 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1990 BY SIEMENS NIXDORF INFORMATIONSSYSTEME AG  */
/*  * All rights reserved                                             */
/*  *                                                                 */
/*  * This software is furnished under licence and may be used only   */
/*  * in accordance with the terms of that licence and with the       */
/*  * inclusion of the above copyright notice.                        */
/*  * This software may not be provided or otherwise made available   */
/*  * to, or used by, any other person. No title to or ownership of   */
/*  * the software is hereby  transferred.                            */
/*                                                                    */
/* ****************************************************************** */

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : ros.h                                               */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90.22.05                                            */
/*                                                                    */
/* COMPONENT    : ROS external interface include file                 */
/*                                                                    */
/* PRD#/VERS.   : IROS - V2.00 for RTROS V03.00                       */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/*                - Operating system:                                 */
/*                     SINIX  : -                                     */
/*                     BS2000 : -                                     */
/*                     MS-DOS : RP0000_MicV0600                       */
/*                                                                    */
/*                - Function prototyping:                             */
/*                     used   : __STDC__                              */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 2.00 K0  | 90-05-22|  original                      |jm  |         */
/*	    |	      | 			       |    |	      */
/*datoff ************************************************************ */

/* ****************** Declaration of FAR and PASCAL ****************** */
/* ****************** (MS-DOS only)                 ****************** */

#ifdef  RP0000_MicV0600
#define F far
#define P pascal
#else
#define F
#define P
#endif

/* ****************************************************************** */
/*                                                                    */
/*                    ROS constant definitions                        */
/*                                                                    */
/* ****************************************************************** */

/* ****************** ROS configuration constant ******************** */
/* ******************                            ******************** */

#define R_G002_maxAbstractSyntaxes  5  /* max. numbers of abstract    */
				       /* syntaxes involved in one    */
				       /* application context	      */

#define R_G003_maxAppliContexts     3  /* max. numbers of application */
				       /* contexts in one AE	      */
				       /* invocation.		      */

#define R_G004_maxEVENTBUFFER	 2048  /* length of buffer for        */
				       /* events		      */

/* ****************** ROS return codes ****************************** */
/* ******************                  ****************************** */

#define   R_SUCES  (R_int)  0      /* Call was successful             */
#define   R_PEND   (R_int)  1      /* Asynchr. funct. finished but    */
				   /* data transfer still running     */
#define   R_NOT1ST (R_int)  2      /* Call was successful, multiple   */
				   /* attached.                       */
#define   R_NOEVNT (R_int)  3      /* r_chwait :  Not event           */
#define   R_TIMOUT (R_int)  4      /* r_chwait :  Wait time runned    */
				   /*             off                 */

/* ****************** ROS error return codes ************************ */
/* ******************                        ************************ */

#define   R_CALERR (R_int) -1      /* Formal error in a ROS call      */
#define   R_PRMERR (R_int) -2      /* Permanent error in a ROS call   */
#define   R_TMPERR (R_int) -99     /* Temporary error in a ROS call   */

/* ****************** ROS error classes ***************************** */
/* ******************                   ***************************** */

#define   R_ILSTAT (R_int)    1    /* ROS call not allowed at that    */
				   /* provider status                 */
#define   R_ILVERS (R_int)    2    /* Illegal ROS version	      */
#define   R_NOSPEC (R_int)    3    /* Not specified unexpected error  */
#define   R_PARERR (R_int)    4    /* Parameter error in a ROS call   */
#define   R_SYSERR (R_int)    5    /* System function error in the    */
				   /* ROS provider                    */
#define   R_OSSERR (R_int)    6    /* Session error		      */
#define   R_SYSOSS (R_int)    7    /* Session system error	      */
#define   R_CMXERR (R_int)    8    /* Transport system error	      */
#define   R_CHCKEY (R_int)    9    /* Authorization or key diskette   */
				   /* access error.                   */

/* ****************** Further ROS constant definitions ************** */
/* ******************                                  ************** */

				   /* r_chwait, r_wtime parameter     */
				   /* constants:                      */
#define R_CHECK (R_unsign) 0       /* Only check, not wait            */
#define R_INFIN (R_unsign) 65535   /* Infinite wait                   */

				   /* r_attach, r_sfunit parameter    */
				   /* constants:                      */
#define R_SFUDX            0x0002  /* session full duplex unit        */

				   /* r_attach, r_pfunit parameter    */
				   /* constants:                      */
#define R_PKERN (R_unsign) 0       /* presentation kernel functional  */
				   /* unit                            */
#define R_PSNIP (R_unsign) 4	   /* presentation private mode       */
				   /* pseudo functional unit.	      */

				   /* r_attach, r_asecif parameter    */
				   /* constants:                      */
#define R_ACSE  (R_unsign) 2       /* ACSE identifier                 */
#define R_ROSE  (R_unsign) 4       /* ROSE identifier                 */
#define R_RTSE  (R_unsign) 8       /* RTSE identifier                 */

				   /* Support interface versions:     */
#define R_VERSION	   250	   /* - ROS-V2.50		      */

/* ****************** ROS logging definitions *********************** */
/* ******************                         *********************** */

#define R_LINTE  (R_unsign)    2   /* - logging of interface calls.   */
				   /*   (without r_cwait)             */
#define R_LACTI  (R_unsign)    4   /* - logging of protocol and       */
				   /*   protocol machines activites.  */
#define R_LCAIT  (R_unsign)    8   /* - logging of r_cwait calls if   */
				   /*   interface logging is switched */
				   /*   on.                           */
#define R_MAXREC (R_unsign) 1200   /* - maximum length of an user     */
				   /*   logging record.               */

#define R_NEWFIL               0   /* - Create new logging file.      */
#define R_EXTFIL               1   /* - Extened or create logging     */
				   /*	file.			      */

/* ****************************************************************** */
/*                                                                    */
/*                    ROS basic data types                            */
/*                                                                    */
/* ****************************************************************** */

/*
 * The following data types are dependent from the target computer.
 * They have to be altered with a ros portation, if an abstract
 * range can not be supported by its native range.
 *
	| native range | Ros type     |  abstract range               */
  
typedef unsigned short R_ushort;     /*  {      0 .. 255   }          */
typedef unsigned short R_unsign;     /*  {      0 .. 65535 }          */
typedef          short R_int;        /*  { -32768 .. 32767 }          */
typedef          int   R_idtype;     /*  Ros identification value     */
typedef 	 long  R_wclass;     /*  Wait classes (not supported) */

typedef          short R_bool;       /*  { R_TRUE , R_FALSE }         */
#define R_FALSE (R_bool) 0
#define R_TRUE  (R_bool) 1

typedef          short R_aclass;     /* Defined association classes   */
#define  R_ACL1 (R_aclass) 1
#define  R_ACL2 (R_aclass) 2
#define  R_ACL3 (R_aclass) 3

typedef          short R_oclass;     /* Defined operation classes     */
#define        R_OPC1 (R_oclass) 1
#define        R_OPC2 (R_oclass) 2
#define        R_OPC3 (R_oclass) 3
#define        R_OPC4 (R_oclass) 4
#define        R_OPC5 (R_oclass) 5

typedef 	   short R_accept;   /* Specification in an r_attach  */
#define        R_REJECT (R_accept) 0 /* whether an application has to */
#define        R_SELECT (R_accept) 1 /* accept an association request */
#define        R_GLOBAL (R_accept) 2 /* from other applications .     */

typedef            short R_mtype;    /* Types of user data parameter  */
#define        R_MEMORY (R_mtype)  0
#define        R_FILE   (R_mtype)  1

#ifdef RP0000_MicV0600
#define        R_GLOHAN (R_mtype)  2 /* Memory handle of global heap  */
#endif

typedef            short R_otype;    /* Types of ROS function         */
#define        R_SYNCH  (R_otype)  0 /* execution modes               */
#define        R_ACKSYN (R_otype)  1
#define        R_ACKASN (R_otype)  2
#define        R_ASYNCH (R_otype)  3

typedef            short R_evtype;   /* ROS events announced by the   */
#define        R_RBINEV (R_evtype) 0 /* 'check and wait' function     */
#define        R_FAINEV (R_evtype) 1
#define        R_BNINEV (R_evtype) 2
#define        R_BNCNEV (R_evtype) 3
#define        R_UBINEV (R_evtype) 4
#define        R_UBCNEV (R_evtype) 5
#define        R_ABINEV (R_evtype) 6
#define        R_IVINEV (R_evtype) 7
#define        R_RSINEV (R_evtype) 8
#define        R_ERINEV (R_evtype) 9
#define        R_RUINEV (R_evtype) 10
#define        R_RPINEV (R_evtype) 11

#define        R_IVRQEV (R_evtype) 12 /* ROS events anounced within   */
#define        R_RSRQEV (R_evtype) 13 /* the provider-reject-apdu-not-*/
#define        R_ERRQEV (R_evtype) 14 /* transfered event.            */
#define        R_RURQEV (R_evtype) 15

typedef        short R_fnctnb;       /* ROS function numbers which    */
#define        R_ATACH (R_fnctnb)  0 /* have to be used in the        */
#define        R_DTACH (R_fnctnb)  1 /* function header of a function */
#define        R_CWAIT (R_fnctnb)  2 /* parameter structure           */
#define        R_FRASD (R_fnctnb)  3
#define        R_FRARV (R_fnctnb)  4
#define        R_PRBUF (R_fnctnb)  5
#define        R_REDRQ (R_fnctnb)  6
#define        R_WAKE  (R_fnctnb)  7
#define        R_BINRQ (R_fnctnb)  8
#define        R_BININ (R_fnctnb)  9
#define        R_BINRS (R_fnctnb) 10
#define        R_BINCN (R_fnctnb) 11
#define        R_UBNRQ (R_fnctnb) 12
#define        R_UBNIN (R_fnctnb) 13
#define        R_UBNRS (R_fnctnb) 14
#define        R_UBNCN (R_fnctnb) 15
#define        R_ABORQ (R_fnctnb) 16
#define        R_INVRQ (R_fnctnb) 17
#define        R_INVIN (R_fnctnb) 18
#define        R_RESRQ (R_fnctnb) 19
#define        R_RESIN (R_fnctnb) 20
#define        R_ERRRQ (R_fnctnb) 21
#define        R_ERRIN (R_fnctnb) 22
#define        R_RJURQ (R_fnctnb) 23
#define        R_RJPIN (R_fnctnb) 24
#define        R_GTDCS (R_fnctnb) 26
#define        R_GTPBF (R_fnctnb) 27
#define        R_LOGON (R_fnctnb) 100
#define        R_LOGOF (R_fnctnb) 101
#define        R_LOGWR (R_fnctnb) 102

/* ****************************************************************** */
/*                                                                    */
/*		      Association result data type and constants      */
/*                                                                    */
/* ****************************************************************** */

typedef unsigned long int R_result;

#define R_ACCEPT (R_result) 0x0000 /* - Accepted, positive result.    */

				   /* - Not accepted, negative result,*/
				   /*   reason is :                   */

#define R_UREJCT (R_result) 0x0100 /*	 -- Reason is not defined.    */
#define R_NOREAS (R_result) 0x0101 /*	 -- No reason given.	      */
#define R_APCNAM (R_result) 0x0102 /*	 -- Application context name  */
				   /*	    not supported.	      */
#define R_CAGAPT (R_result) 0x0103 /*	 -- Calling-AP-title not      */
				   /*	     recognized.	      */
#define R_CADART (R_result) 0x0107 /*	 -- Called-AP-title not       */
				   /*	    recognized. 	      */

/* ****************************************************************** */
/*                                                                    */
/*		      Abort reason data type and constants	      */
/*                                                                    */
/* ****************************************************************** */

typedef unsigned long int R_areasn;

#define R_UABORT (R_areasn)   0x0000

/* ****************************************************************** */
/*                                                                    */
/*		      Remote operation reject data type and constants */
/*                                                                    */
/* ****************************************************************** */

typedef unsigned long int R_reason;

/* ****************** Values which are used in an user reject ******* */
/* ****************** request and indicated in an user reject ******* */
/* ****************** indication.			      ******* */

#define        R_RINDU (R_reason) 0x00010800 /* Invoke problem	      */
#define        R_RINUO (R_reason) 0x00010801
#define        R_RINMA (R_reason) 0x00010802
#define        R_RINRL (R_reason) 0x00010803
#define        R_RINIR (R_reason) 0x00010804
#define        R_RINUL (R_reason) 0x00010805
#define        R_RINLR (R_reason) 0x00010806
#define        R_RINUC (R_reason) 0x00010807

#define        R_RREUI (R_reason) 0x00020800 /* Return result problem */
#define        R_RRERU (R_reason) 0x00020801
#define        R_RREMR (R_reason) 0x00020802

#define        R_RERUI (R_reason) 0x00030800 /* Return error problem  */
#define        R_RERRU (R_reason) 0x00030801
#define        R_RERCE (R_reason) 0x00030802
#define        R_RERXE (R_reason) 0x00030803
#define        R_RERMP (R_reason) 0x00030804

/* ****************** Values which are indicated in a *************** */
/* ****************** provider reject indication.     *************** */

#define        R_RPOUA (R_reason) 0x00000800 /* General problem       */
#define        R_RPOMA (R_reason) 0x00000801
#define        R_RPOBA (R_reason) 0x00000802

#define        R_NTRAN (R_reason)    1	     /* Not used in this      */
#define        R_NPRED (R_reason)    2	     /* version 	      */

/* ****************************************************************** */
/*                                                                    */
/*                    ROS local syntax of ASN.1 types                 */
/*                                                                    */
/* ****************************************************************** */

				/* Local syntax of an object          */
typedef struct {                /* identifier:                        */
	R_unsign F *r_obidrf;	/* --> reference to components	      */
	R_unsign    r_obidnb;	/* --> number of components	      */
} R_objid;
				/* Local syntax of an object         */
				/* identifier to be received by a    */
				/* synchron function:                */
typedef struct {		/* (not supported)		     */
	R_unsign F *r_obidrf;	/* <-> reference to components	     */
	R_unsign    r_obidbf;	/* --> length of component buffer    */
	R_unsign    r_obidnb;	/* <-- number of components	     */
} R_robid;

				/* Local syntax of operation code    */
typedef struct {                /* and error code:                   */
	R_int	 F *r_oploc;	/* --> local value		     */
	R_objid     r_opglb;	/* --> global value		     */
} R_opval;
				/* Local syntax of operation code and*/
				/* error code to be received by a    */
				/* synchron function:                */
typedef struct {		/* (not supported)		     */
	R_int	 F *r_oploc;	/* <-- local value		     */
	R_robid     r_opglb;	/* <-> global value		     */
} R_ropval;

				/* Local syntax of presentation      */
				/* address and application process   */
typedef struct {                /* title:                            */
	char	 F *r_paddrf;	/* --> paddress reference	     */
	R_unsign    r_paddln;	/* --> paddress length		     */
	R_objid     r_aetitl;	/* --> application process title     */
} R_apname;
				/* Local syntax of a presentation    */
typedef struct {                /* address:                          */
	char	 F *r_paddrf;	/* --> paddress reference	     */
	R_unsign    r_paddln;	/* --> paddress length		     */
} R_pradd;
				/* Local syntax of a presentation    */
				/* address to be received by a       */
				/* synchron function:                */
typedef struct {		/* (not supported)		     */
	char	 F *r_paddrf;	/* <-> paddress reference	     */
	R_unsign    r_paddbf;	/* --> paddress buffer length	     */
	R_unsign    r_paddln;	/* <-- paddress length		     */
} R_rpadd;
				/* Structures describing the         */
				/* application contexts which are    */
				/* involved in one AE: (I) - (IV))   */

typedef struct {		/* (I)	Abstract syntax list array:  */
	R_int     r_objinx;     /* --> index within R_objary         */
	R_unsign  r_objnum;     /* --> number of objid components    */
	R_int     r_absid;      /* --> abstract syntax identifier    */
	R_unsign  r_mskid;      /* --> abstract syntax mask          */
				/*     identifier                    */
	R_int     r_idtinx;     /* --> Index to the transfer syntax  */
				/*     identifier list of this       */
				/*     abstract syntax within the    */
				/*     r_idtary                      */
	R_unsign  r_idtnum;     /* --> Number of transfer syntaxes   */
} R_absary;
				/* (II) Element of array describing  */
typedef struct {		/*	transfer  syntaxes	     */
	R_int	  r_objinx;	/* --> Index of the first component  */
				/*     of the objid of this transfer */
				/*     syntax  within r_objary.      */
	R_unsign  r_objnum;	/* --> Number of objid components of */
				/*     this transfer syntax.	     */
	R_int	  r_trsid;	/* --> Transfer syntax identifier,   */
				/*     used subsequently as short    */
} R_trsary;			/*     identifier.		     */

				/* (III) Application context list    */
typedef struct {		/*	 array element: 	     */
	R_int      r_apcinx;    /* --> index within R_objary         */
	R_unsign   r_objnum;    /* --> number of components          */
	R_unsign   r_sfunit;    /* --> session functional units      */
	R_unsign   r_pfunit;    /* --> presentation functional units */
	R_unsign   r_asecif;    /* --> ASE configuration id of non   */
				/*     ROSE ASE's.                   */
	R_int      r_idsinx;    /* --> index within r_idsary         */
	R_unsign   r_idsnum;    /* --> number of abstract syntaxes   */
} R_apcary;
				/* (IV) applicatation context	     */
typedef struct {                /*     describtion:                  */
	R_unsign   r_apcnum;    /* --> Number of entries within the  */
				/*     apllication context array     */
	R_unsign   r_absnum;    /* --> Number of entries within the  */
				/*     abstract syntax list array    */
	R_unsign   r_trsnum;    /* --> Number of transfer syntaxes   */
				/*     described in the transfer     */
				/*     syntax list array             */
	R_apcary  F *r_apcary;	/* --> Pointer to an array describing*/
				/*     basic aspects of the AC's of  */
				/*     an AE.                        */
	R_absary  F *r_absary;	/* --> Pointer to an array describing*/
				/*     the abstract syntaxes of this */
				/*     application contexts.         */
	R_int	  F *r_idsary;	/* --> Pointer to an array containing*/
				/*     all abstract syntax identifier*/
				/*     lists                         */
	R_trsary  F *r_trsary;	/* --> Pointer to an array describing*/
				/*     the transfer syntaxes         */
	R_int	  F *r_idtary;	/* --> Pointer to an array containing*/
				/*     all transfer syntax (trsid)   */
	R_unsign  F *r_objary;	/* --> Pointer to an array containing*/
				/*     all obj-id components.        */
} R_apclst;

				/*    ROSE user data and ACSE user   */
				/*    information if they have       */
typedef struct {                /*    to be sended:                  */
	R_mtype     r_mtype;	/* --> memory type		     */
	R_bool	    r_more;	/* --> more data flag		     */
	R_int	    r_absid;	/* --> abstract syntax identifier    */
	char	 F *r_fraref;	/* --> fragment reference	     */
	R_unsign    r_fralen;	/* --> fragment length		     */
} R_sndata;

				/*    ROSE user data and ACSE user   */
				/*    information if they have       */
typedef struct {                /*    to be received:                */
	R_mtype     r_mtype;	/* --> memory type		     */
	R_bool	    r_specfl;	/* <-- special flag		     */
	R_int	    r_absid;	/* <-- abstract syntax identifier    */
	char	 F *r_bufref;	/* <-> buffer reference 	     */
	R_unsign    r_buflen;	/* --> buffer length		     */
	R_unsign    r_datlen;	/* <-- data length		     */
} R_evdata;
				/*    ROSE user data and ACSE user   */
				/*    information if they have to    */
				/*    be received by an indication   */
				/*    function:                      */
typedef struct {		/* (not supported)		     */
	R_mtype     r_mtype;	/* --> memory type		     */
	R_bool	    r_more;	/* <-- more data flag		     */
	R_int	    r_absid;	/* <-- abstract syntax identifier    */
	char	 F *r_bufref;	/* <-> buffer reference 	     */
	R_unsign    r_buflen;	/* --> buffer length		     */
	R_unsign    r_fralen;	/* <-- fragment length		     */
} R_rvdata;

/* ****************************************************************** */
/*                                                                    */
/*      Data structures for private transfer syntaxes                 */
/*                                                                    */
/* ****************************************************************** */

			       /* element of presentation context     */
			       /* array 			      */
typedef struct {
	R_int  r_absid;        /* <-- Abstract syntax identifier      */
	R_int  r_trsid;        /* <-- Transfer syntax identifier      */
} R_prcnt;

/* ****************************************************************** */
/*                                                                    */
/*		      Further ROS constructed data types	      */
/*                                                                    */
/* ****************************************************************** */

/*
 * ROS receipt buffer, provider reject indication event data, if
 * reason is "not transfered" and wait time structure
 *
 */
				/* Receipt buffer provided by         */
typedef struct {                /* r_prvbuf:                          */
	R_mtype     r_mtype;	/* --> memory type		      */
	char	 F *r_bufref;	/* --> buffer reference 	      */
	R_unsign    r_buflen;	/* --> buffer length		      */
} R_buffer;

				/* Provider reject indication event   */
typedef struct {                /* data, if reason = R_NTRAN:         */
	R_evtype  r_evtype;     /* <-- send request type              */
	R_reason  r_reason;     /* <-- user reject reason             */
	R_bool    r_dflag;      /* <-- data flag                      */
	R_opval   r_srval;      /* <-- send request oper. or error val*/
	R_evdata  r_evdata;     /* <-- event data  description        */
} R_prdata;
				/* Wait function parameter            */
				/* in r_attach:                       */
typedef struct {		/* (not supported)		      */
	R_idtype    r_wstid;	/* --> waitstate identifier	      */
	R_wclass    r_wclass;	/* --> wait class		      */
	char	 F *r_uconwd;	/* --> user context word	      */
} R_wtinfo;

/* ****************************************************************** */
/*                                                                    */
/*                    ROS event parameter block structures            */
/*                                                                    */
/* ****************************************************************** */

/* ****************** ROS event header ****************************** */
/* ******************                  ****************************** */

typedef struct {                /* Common event header:               */
	R_idtype    r_subid;	/* <-- subsystem identifier	      */
	char	 F *r_uconwd;	/* <-- user context word	      */
	char	 F *r_evlink;	/* <-- next event link		      */
	R_evtype    r_evtype;	/* <-- subsyst. specific event code   */
} R_evnthd;

/* ****************** Local events ********************************** */
/* ******************              ********************************** */

typedef struct {                /* return buffer indication event:    */
	R_evnthd  r_evnthd;     /* <-- event header                   */
	R_buffer  r_buffer;     /* <-- buffer describtion             */
} R_rbinev;

typedef struct {                /* Fragment complete indication event:*/
	R_evnthd  r_evnthd;     /* <-- event header                   */
	R_idtype  r_assid;      /* <-- association identifier         */
	R_int     r_invid;      /* <-- invoke identifier              */
	R_rvdata  r_rvdata;     /* <-- user receive data              */
} R_fainev;

/* ****************** Association handling events ******************* */
/* ******************                             ******************* */

typedef struct {                /* bind indication event:             */
	R_evnthd  r_evnthd;     /* <-- event header                   */
	R_idtype  r_assid;      /* <-- association identifier         */
	R_idtype  r_aplid;      /* <-- application identifier         */
	R_idtype  r_pid;        /* <-- process identifier             */
	R_objid   r_ptcon;      /* <-- partners applic. context       */
	R_apname  r_ptnref;     /* <-- Reference of partner's applic. */
	R_evdata  r_evdata;     /* <-- user receive data              */
} R_bninev;

typedef struct {                /* bind confirmatation event:         */
	R_evnthd  r_evnthd;     /* <-- Event Header                   */
	R_idtype  r_assid;      /* <-- Association ID                 */
	R_result  r_result;	/* <-- accept or reject reason	      */
	R_objid   r_ptcon;      /* <-- partners applic. contontext    */
	R_pradd   r_pradd;      /* <-- paddress                       */
	R_objid   r_aetitl;     /* <-- application process title      */
	R_evdata  r_evdata;     /* <-- user receive data              */
} R_bncnev;

typedef struct {                /* unbind indication event:           */
	R_evnthd  r_evnthd;     /* <-- event header                   */
	R_idtype  r_assid;      /* <-- association identifier         */
	R_evdata  r_evdata;     /* <-- user receive data              */
} R_ubinev;

typedef struct {                /*  unbind confirmation event:        */
	R_evnthd  r_evnthd;     /* <-- event Header                   */
	R_bool    r_fail;       /* <-- operation failure              */
	R_idtype  r_assid;      /* <-- association identifier         */
	R_evdata  r_evdata;     /* <-- user receive data              */
} R_ubcnev;

typedef struct {                /* association abort indicat. event:  */
	R_evnthd  r_evnthd;     /* <-- event header                   */
	R_areasn  r_result;	/* <-- accept or reject reason	      */
	R_areasn  r_adinfo;	/* <-- additional reason parameter    */
	R_idtype  r_assid;      /* <-- association identifier         */
	R_evdata  r_evdata;     /* <-- user receive data              */
} R_abinev;

/* ****************** Remote operation events *********************** */
/* ******************                         *********************** */

typedef struct {                /* Invoke indication event:           */
	R_evnthd    r_evnthd;	/* <-- event header		      */
	R_opval     r_opval;	/* <-- operation value		      */
	R_idtype    r_assid;	/* <-- association identifier	      */
	R_int	    r_invid;	/* <-- invoke identifier	      */
	R_int	 F *r_lnkid;	/* <-- linked identifier	      */
	R_evdata    r_evdata;	/* <-- user receive data	      */
} R_ivinev;

typedef struct {                /* Result indication event:           */
	R_evnthd  r_evnthd;     /* <-- event header                   */
	R_idtype  r_assid;      /* <-- association identifier         */
	R_int     r_invid;      /* <-- invoke identifier              */
	R_opval   r_opval;      /* <-- operation value                */
	R_evdata  r_evdata;     /* <-- user receive data              */
} R_rsinev;

typedef struct {                /* Error indication event:            */
	R_evnthd  r_evnthd;     /* <-- event header                   */
	R_opval   r_erval;      /* <-- error value                    */
	R_idtype  r_assid;      /* <-- association identifier         */
	R_int     r_invid;      /* <-- invoke identifier              */
	R_evdata  r_evdata;     /* <-- user receive data              */
} R_erinev;

typedef struct {                /* User reject indication event:      */
	R_evnthd  r_evnthd;     /* <-- event header                   */
	R_reason  r_reason;     /* <-- reject reason                  */
	R_idtype  r_assid;      /* <-- association identifier         */
	R_int     r_invid;      /* <-- invoke ientifier               */
} R_ruinev;

typedef struct {                /* Provider reject indication event:  */
	R_evnthd  r_evnthd;     /* <-- event header                   */
	R_reason  r_reason;     /* <-- reject reason                  */
	R_bool    r_valid;      /* <-- valid invoke id flag           */
	R_idtype  r_assid;      /* <-- association identifier         */
	R_int     r_invid;      /* <-- invoke identifer               */
	R_prdata  r_prdata;     /* <-- provider reject data           */
} R_rpinev;

/* ****************** Event union *********************************** */
/* ******************             *********************************** */

typedef union {
	R_evnthd    r_evnthd;   /* event header                       */

	R_rbinev    r_rbinev;   /* return buffer indication event     */
	R_bninev    r_bninev;   /* bind indication event              */
	R_bncnev    r_bncnev;   /* bind confirmation event            */
	R_ubinev    r_ubinev;   /* unbind indication event            */
	R_ubcnev    r_ubcnev;   /* unbind confirmation event          */
	R_abinev    r_abinev;   /* abort indication event             */
	R_ivinev    r_ivinev;   /* invoke indication event            */
	R_rsinev    r_rsinev;   /* result indication event            */
	R_erinev    r_erinev;   /* error indication event             */
	R_ruinev    r_ruinev;   /* user reject indication event       */
	R_rpinev    r_rpinev;   /* provider reject indication event   */

				/* parameter block buffer with        */
				/* minimal maximal length.            */

	char r_evpbbuf[R_G004_maxEVENTBUFFER];
} R_evtpb;

/* ****************************************************************** */
/*                                                                    */
/*                    ROS function parameter structures               */
/*                                                                    */
/* ****************************************************************** */

/* ****************** ROS function header *************************** */
/* ******************                     *************************** */

typedef struct {                /* function header:                   */
	R_unsign  r_unitnb;     /* --> unit number                    */
	R_fnctnb  r_fnctnb;     /* --> function call identifier       */
	R_ushort  r_versnb;     /* --> version number                 */
	R_int     r_errval;     /* <-- error value                    */
	R_int     r_erclas;     /* <-- error class                    */
	R_int     r_retcod;     /* <-- return code                    */
} R_fncthd;

/* ****************** Local functions ******************************* */
/* ******************                 ******************************* */

typedef struct {                /* r_attach:                          */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_accept  r_accept;     /* --> accept requests                */
	R_wtinfo  r_wtinfo;     /* --> wait function information      */
	R_idtype  r_pid;        /* --> process identifier             */
	R_apname  r_aplref;     /* --> application reference          */
	R_idtype  r_aplid;      /* <-- application identifier         */
	R_apclst  r_apclst;     /* --> application context description*/
	R_objid   r_apcon;      /* --> application context            */
	R_pradd   r_pradd;      /* --> presention address             */
	R_objid   r_aetitl;     /* --> application process title      */
	R_sndata  r_sndata;     /* --> user send data                 */
} R_attapb;

typedef struct {                /* r_detach:                          */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_idtype  r_aplid;      /* --> application identifier         */
} R_detapb;

typedef struct {                /* r_chwait:                          */
	R_fncthd    r_hdinfo;	/* <-> function header		      */
	R_unsign    r_wtime;	/* --> wait time		      */
	R_idtype    r_wstid;	/* --> waitstate identifier	      */
	R_wclass    r_wclset;	/* --> wait class set		      */
	R_bool	    r_eflag;	/* --> event number flag	      */
	R_ushort    r_evnum;	/* <-- real event number	      */
	R_unsign    r_evlen;	/* --> event buffer length	      */
	R_evtpb  F *r_evntrf;	/* --> event buffer reference	      */
} R_chwtpb;
				/* r_frasnd (not supported)	      */
typedef struct {		/*				      */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_idtype  r_assid;      /* --> association identifier         */
	R_sndata  r_sndata;     /* --> user send data                 */
} R_fasdpb;
				/* r_frarcv (not supported)	      */
typedef struct {		/*				      */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_otype   r_otype;      /* --> operation type                 */
	R_idtype  r_assid;      /* --> association identifier         */
	R_rvdata  r_rvdata;     /* <-> user receive data              */
} R_farvpb;

typedef struct {                /* r_prvbuf:                          */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_idtype  r_aplid;      /* --> application identifier         */
	R_buffer  r_buffer;     /* --> receive buffer                 */
} R_pvbfpb;

typedef struct {                /* r_redrq:                           */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_otype   r_otype;      /* --> operation type                */
	R_idtype  r_assid;      /* --> association identifier         */
	R_idtype  r_pid;        /* --> process identifier             */
	R_objid   r_ptcon;      /* --> partner applic. context id.    */
	R_apname  r_ptnref;     /* --> reference of partners applic.  */
	R_sndata  r_sndata;     /* --> user send data                 */
} R_rdrqpb;

typedef struct {                /* r_wake:                            */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_idtype  r_pid;        /* --> process identifier             */
} R_wakepb;

typedef struct {              /* r_gtdcs:                             */
	R_fncthd  r_hdinfo;   /* <--> Function Header                 */
	R_bool    r_cont;     /* -->  Continue Flag                   */
	R_idtype  r_assid;    /* -->  Association ID                  */
	R_unsign  r_pcnum;    /* <--  Number of presentation contexts */
	R_prcnt   r_prcnt
		  [R_G003_maxAppliContexts   *	  /* <-- Presentation */
		   R_G002_maxAbstractSyntaxes];   /*	 context array*/

} R_getdpb;

typedef struct {              /* r_gtpbf:                             */
	R_fncthd  r_hdinfo;   /* <--> Function Header                 */
	R_bool    r_cont;     /* -->  Continue Flag                   */
	R_idtype  r_aplid;    /* -->  application identifier          */
	R_unsign  r_number;   /* <--  Number of appication specific   */
			      /*      not used.                       */
} R_getpbf;

/* ****************** Association handling functions **************** */
/* ******************                                **************** */

typedef struct {                /* r_binreq:                          */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_otype   r_otype;      /* --> operation type                 */
	R_aclass  r_aclass;     /* --> association class              */
	R_wtinfo  r_wtinfo;     /* --> wait function information      */
	R_idtype  r_assid;      /* <-- association identifier         */
	R_objid   r_apcon;      /* --> application context            */
	R_idtype  r_aplid;      /* --> application identifier         */
	R_apname  r_ptnref;     /* --> reference of partner's applic. */
	R_sndata  r_sndata;     /* --> user send data                 */
	R_evtype  r_evtype;     /* <-- reply type                     */
	R_result  r_result;	/* <-- accept or reject reason	      */
	R_robid   r_ptcon;      /* <-> partner's applic. context      */
	R_rpadd   r_rpadd;      /* <-> responding padress             */
	R_robid   r_ratitl;     /* <-> respond. applic. process title */
	R_evdata  r_evdata;     /* <-> user receive data              */
} R_bnrqpb;

typedef struct {                /* r_binres:                          */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_otype   r_otype;      /* --> operation type                 */
	R_wtinfo  r_wtinfo;     /* --> wait function information      */
	R_idtype  r_assid;      /* --> association identifier         */
	R_result  r_result;	/* --> accept or reject reason	      */
	R_objid   r_apcon;      /* --> application context            */
	R_pradd   r_pradd;      /* --> paddress                       */
	R_objid   r_aetitl;     /* --> application process title      */
	R_sndata  r_sndata;     /* --> user send data                 */
} R_bnrspb;
				/* r_bincon: (not supported)	      */
typedef struct {		/*				      */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_otype   r_otype;      /* --> operation type                 */
	R_idtype  r_assid;      /* --> association identifier         */
	R_rvdata  r_rvdata;     /* <-> user receive data              */
} R_bncnpb;

				/* r_binind: (not supported)	      */
typedef R_bncnpb  R_bninpb;	/*				      */

typedef struct {                /* r_ubnreq:                          */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_otype   r_otype;      /* --> operation type                 */
	R_idtype  r_assid;      /* --> association identifier         */
	R_sndata  r_sndata;     /* --> user send Ddta                 */
	R_evtype  r_evtype;     /* <-- reply type                     */
	R_bool    r_fail;       /* <-- operation failure              */
	R_result  r_result;	/* <-- accept or reject reason	      */
	R_evdata  r_evdata;     /* <-> user receive data              */
} R_ubrqpb;

typedef struct {                /* r_ubnres:                          */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_otype   r_otype;      /* --> operation type                 */
	R_bool    r_fail;       /* --> operation failure              */
	R_idtype  r_assid;      /* --> association identifier         */
	R_sndata  r_sndata;     /* --> user send data                 */
} R_ubrspb;

typedef struct {                /* r_ubncnf:                          */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_otype   r_otype;      /* --> operation type                 */
	R_idtype  r_assid;      /* --> association identifier         */
	R_rvdata  r_rvdata;     /* <-> user receive data              */
} R_ubcnpb;
				/* r_ubnind: (not supported)	      */
typedef R_ubcnpb  R_ubinpb;	/*				      */

typedef struct {                /* r_aboreq:                          */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_otype   r_otype;      /* --> operation type                 */
	R_idtype  r_assid;      /* --> association identifier         */
	R_sndata  r_sndata;     /* <-> user receive data              */
} R_abrqpb;

/* ****************** Remote operation functions ******************** */
/* ******************                            ******************** */

typedef struct {                /* r_invreq:                          */
	R_fncthd    r_hdinfo;	/* <-> function header		      */
	R_bool	    r_cont;	/* --> continue flag		      */
	R_otype     r_otype;	/* --> operation type		      */
	R_oclass    r_oclass;	/* --> operation class		      */
	R_opval     r_opval;	/* --> operation value		      */
	R_idtype    r_assid;	/* --> association identifer	      */
	R_int	    r_invid;	/* --> invoke identifier	      */
	R_int	 F *r_lnkid;	/* --> linked identifier	      */
	R_sndata    r_sndata;	/* --> user send data		      */
	R_evtype    r_evtype;	/* <-- reply type		      */
	R_reason    r_reason;	/* <-- reject reason		      */
	R_int	    r_result;	/* <-- accept or reject reason	      */
	R_ropval    r_erval;	/* <-> error value		      */
	R_evdata    r_evdata;	/* <-> user receive data	      */
} R_ivrqpb;
				/* r_invind: (not supported)	      */
typedef struct {		/*				      */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_otype   r_otype;      /* --> operation type                 */
	R_idtype  r_assid;      /* --> association identifier         */
	R_int     r_invid;      /* --> invoke identifier              */
	R_rvdata  r_rvdata;     /* --> user receive data              */
} R_ivinpb;

typedef struct {                /* r_resreq:                          */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_otype   r_otype;      /* --> operation type                 */
	R_idtype  r_assid;      /* --> association identifier         */
	R_int     r_invid;      /* --> invoke identifier              */
	R_opval   r_opval;      /* --> operation value                */
	R_sndata  r_sndata;     /* --> user send data                 */
} R_rsrqpb;

				/* r_resind: (not supported)	      */
typedef R_ivinpb  R_rsinpb;	/*				      */

typedef struct {                /* r_errreq:                          */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_otype   r_otype;      /* --> operation type                 */
	R_idtype  r_assid;      /* --> association identifier         */
	R_opval   r_erval;      /* --> error value                    */
	R_int     r_invid;      /* --> invoke identifier              */
	R_sndata  r_sndata;     /* --> user send data                 */
} R_errqpb;
				/* r_errind: (not supported)	      */
typedef R_ivinpb  R_erinpb;	/*				      */

typedef struct {                /* r_rjureq:                          */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_otype   r_otype;      /* --> operation type                 */
	R_reason  r_reason;     /* --> reject reason                  */
	R_idtype  r_assid;      /* --> association identifier         */
	R_int     r_invid;      /* --> invoke identifier              */
 } R_rurqpb;
				/* r_rjpind: (not supported)	      */
typedef struct {		/*				      */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
	R_idtype  r_assid;      /* --> association identifier         */
	R_int     r_invid;      /* --> invoke identifier              */
	R_rvdata  r_rvdata;     /* --> user receive data              */
} R_rpinpb;

/* ****************** ROS logging functions ************************* */
/* ******************                       ************************* */

typedef struct {                /* r_logon:                           */
	R_fncthd    r_hdinfo;	/* <-> function header		      */
	R_bool	    r_cont;	/* --> continue flag		      */
	char	 F *r_fname;	/* --> logging file name	      */
	R_int	    r_fmode;	/* --> open mode		      */
	R_int	    r_rcnum;	/* --> max. records to log in file    */
	R_unsign    r_option;	/* --> logging options		      */
 } R_lgonpb;

typedef struct {                /* r_logof:                           */
	R_fncthd  r_hdinfo;     /* <-> function header                */
	R_bool    r_cont;       /* --> continue flag                  */
 } R_lgofpb;

typedef struct {                /* r_logwr:                           */
	R_fncthd    r_hdinfo;	/* <-> function header		      */
	R_bool	    r_cont;	/* --> continue flag		      */
	R_unsign    r_option;	/* --> logging options		      */
	R_unsign    r_rectyp;	/* --> logging record type	      */
	R_unsign    r_reclen;	/* --> logging record length	      */
	char	 F *r_recptr;	/* --> pointer to logging record      */
 } R_lgwrpb;

/* ****************** R_excom function ****************************** */
/* ******************                  ****************************** */

typedef union  {
	R_fncthd  r_hdinfo;     /* Function header                   */
	R_attapb  r_attapb;     /* Local functions                   */
	R_detapb  r_detapb;
	R_chwtpb  r_chwtpb;
	R_pvbfpb  r_pvbfpb;
	R_rdrqpb  r_rdrqpb;
	R_wakepb  r_wakepb;
	R_getdpb  r_getdpb;
	R_getpbf  r_getpbf;
	R_bnrqpb  r_bnrqpb;     /* Association handling functions    */
	R_bnrspb  r_bnrspb;
	R_ubrqpb  r_ubrqpb;
	R_ubrspb  r_ubrspb;
	R_abrqpb  r_abrqpb;
	R_ivrqpb  r_ivrqpb;     /* Remote operations functions         */
	R_rsrqpb  r_rsrqpb;
	R_errqpb  r_errqpb;
	R_rurqpb  r_rurqpb;
	R_lgonpb  r_lgonpb;     /* Ros logging functions             */
	R_lgofpb  r_lgofpb;
	R_lgwrpb  r_lgwrpb;
} R_excopb;
/* ****************************************************************** */
/* for DIR add these two #defs                                        */
/* ****************************************************************** */
#define    r_callback         t_callback
#define    r_setopt           t_setopt

/* ****************************************************************** */
/*                                                                    */
/*                    Extern declaration of ROS function call's       */
/*                                                                    */
/* ****************************************************************** */

#ifdef __STDC__

R_int F P r_excom(R_excopb F *); /* Execute communication	     */
				 /* (not supported		     */

R_int F P r_atach(R_attapb F *); /* Local functions		     */
R_int F P r_dtach(R_detapb F *);
R_int F P r_cwait(R_chwtpb F *);
R_int F P r_frasd(R_fasdpb F *);
R_int F P r_frarv(R_farvpb F *);
R_int F P r_prbuf(R_pvbfpb F *);
R_int F P r_redrq(R_rdrqpb F *);
R_int F P r_wake (R_wakepb F *);
R_int F P r_gtdcs(R_getdpb F *);
R_int F P r_gtpbf(R_getpbf F *);

R_int F P r_binrq(R_bnrqpb F *); /* Association handling functions   */
R_int F P r_binin(R_bninpb F *);
R_int F P r_binrs(R_bnrspb F *);
R_int F P r_bincn(R_bncnpb F *);
R_int F P r_ubnrq(R_ubrqpb F *);
R_int F P r_ubnin(R_ubinpb F *);
R_int F P r_ubnrs(R_ubrspb F *);
R_int F P r_ubncn(R_ubcnpb F *);
R_int F P r_aborq(R_abrqpb F *);

R_int F P r_invrq(R_ivrqpb F *); /* Remote operations functions	     */
R_int F P r_invin(R_ivinpb F *);
R_int F P r_resrq(R_rsrqpb F *);
R_int F P r_resin(R_rsinpb F *);
R_int F P r_errrq(R_errqpb F *);
R_int F P r_errin(R_erinpb F *);
R_int F P r_rjurq(R_rurqpb F *);
R_int F P r_rjpin(R_rpinpb F *);

R_int F P r_logon(R_lgonpb F *); /* Ros logging functions	     */
R_int F P r_logof(R_lgofpb F *);
R_int F P r_logwr(R_lgwrpb F *);

#else

extern	R_int	r_excom();	/* Execute communication	     */
				/*  (not supported		     */

extern  R_int   r_atach();      /* Local functions                   */
extern  R_int   r_dtach();
extern  R_int   r_cwait();
extern  R_int   r_frasd();
extern  R_int   r_frarv();
extern  R_int   r_prbuf();
extern  R_int   r_redrq();
extern  R_int   r_wake();
extern  R_int   r_gtdcs();
extern  R_int   r_gtpbf();

extern  R_int   r_binrq();      /* Association handling functions    */
extern  R_int   r_binin();
extern  R_int   r_binrs();
extern  R_int   r_bincn();
extern  R_int   r_ubnrq();
extern  R_int   r_ubnin();
extern  R_int   r_ubnrs();
extern  R_int   r_ubncn();
extern  R_int   r_aborq();

extern  R_int   r_invrq();      /* Remote operation functios         */
extern  R_int   r_invin();
extern  R_int   r_resrq();
extern  R_int   r_resin();
extern  R_int   r_errrq();
extern  R_int   r_errin();
extern  R_int   r_rjurq();
extern  R_int   r_rjpin();

extern  R_int   r_logon();      /* Ros logging functions             */
extern  R_int   r_logof();
extern  R_int   r_logwr();

#endif

#endif	/* _ROS_H */
