/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ross.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:35  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:12  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:41:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:31  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:53  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:37  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:45  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:04:06  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:18  keutel
 * 	creation
 * 	[1994/03/21  13:32:17  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROSS_H
#define _ROSS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _ross_rcsid[] = { "@(#) $RCSfile: ross.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:35 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1993 BY SIEMENS NIXDORF INFORMATIONSSYSTEME AG  */
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
/* NAME         : ross.h                                              */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 93.02.12                                            */
/*                                                                    */
/* PRD#/VERS.   : IROS - V03.00 for RTROS V03.00                      */
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
/* 03.00K0  | 93-02-12|  original                      |jm  |         */
/*          |         |                                |    |         */
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

/* ****************** ROS configuration constants ******************* */
/* ******************                             ******************* */

#define RO_maxEVENTBUFFER   2048       /* length of buffer for        */
				       /* events                      */

#define RO_maxExternals        5       /* Max subfragments in Bind/   */
				       /* Unbind/Abort udata          */

/* ****************** ROS non-error return codes ******************** */
/* ******************                            ******************** */

#define   RO_SUCES           0     /* Call was successful             */
#define   RO_NOT1ST          1     /* Call was successful, multiple   */
				   /* attached.                       */
#define   RO_WARNIG          2     /* Success, but information lost   */
#define   RO_NOEVNT          3     /* ro_cwait :  Not event           */
#define   RO_TIMOUT          4     /* ro_cwait :  Wait time elapsed   */

/* ****************** ROS error return codes ************************ */
/* ******************                        ************************ */

#define   RO_CALERR         -1     /* Formal error in a ROS call      */
#define   RO_PRMERR         -2     /* Permanent error in a ROS call   */
#define   RO_TMPERR        -99     /* Temporary error in a ROS call   */

/* ****************** ROS error classes ***************************** */
/* ******************                   ***************************** */

#define   RO_ILSTAT           1    /* ROS call not allowed at that    */
				   /* provider status                 */
#define   RO_ILVERS           2    /* Illegal ROS version             */
#define   RO_NOSPEC           3    /* Not specified unexpected error  */
#define   RO_PARERR           4    /* Parameter error in a ROS call   */
#define   RO_SYSERR           5    /* System function error in the    */
				   /* ROS provider                    */
#define   RO_OSSERR           6    /* Session error                   */
#define   RO_SYSOSS           7    /* Session system error            */
#define   RO_CMXERR           8    /* Transport system error          */
#define   RO_CHCKEY           9    /* Authorization or key diskette   */
				   /* access error.                   */


/* ****************** Range of values of r_wtime field ************** */
/* ****************** in RO_chwtpb (both inclusive).   ************** */

#define RO_CHECK           0       /* Only check, not wait            */
#define RO_INFIN       65535       /* Infinite wait                   */

/* ****************** Possible values of r_pmode field ************** */
/* ****************** of RO_apcary (Mode selector).    ************** */

#define RO_PNORM           0       /* Presentation Normal mode        */
#define RO_PSNIP           4       /* presentation Private mode       */

/* ****************** Possible values of r_asecif in   ************** */
/* ****************** RO_acpary (Application Service   ************** */
/* ****************** Element - ASE - Confiuration;    ************** */
/* ****************** List of possible ASEs).          ************** */
				   
#define RO_ACSE            2       /* ACSE identifier                 */
#define RO_ROSE            4       /* ROSE identifier                 */
#define RO_RTSE            8       /* RTSE identifier                 */

/* ****************** Supported Interface Versions     ************** */
/* ******************                                  ************** */
				   /* Support interface versions:     */
#define RO_VERSION       300       /* - ROS-V03.00                    */

/* ****************** Values of the r_option field of  ************** */
/* ****************** RO_lgonpb and RO_lgwrpb          ************** */

#define RO_LINTE               2   /* - logging of interface calls.   */
				   /*   (without ro_cwait)            */
#define RO_LACTI               4   /* - logging of protocol and       */
				   /*   protocol machines activites.  */
#define RO_LCAIT               8   /* - logging of ro_cwait calls if  */
				   /*   interface logging is switched */
				   /*   on.                           */

/* ****************** Value of the r_reclen field of  *************** */
/* ****************** RO_lgwrpb                       *************** */

#define RO_MAXREC           1200   /* - maximum length of an user     */
				   /*   logging record.               */

/* ****************** Value of the r_fmode  field of  *************** */
/* ****************** RO_lgonpb                       *************** */

#define RO_NEWFIL              0   /* - Create new logging file.      */
#define RO_EXTFIL              1   /* - Extened or create logging     */
				   /*   file.                         */

/* ****************** Boolean TRUE/FALSE              *************** */
/* ******************                                 *************** */

#define  RO_FALSE  0               /*    { RO_TRUE , RO_FALSE }       */
#define  RO_TRUE   1

/* ****************** Values of possible association  *************** */
/* ****************** classes                         *************** */

#define  RO_ACL1   1
#define  RO_ACL2   2
#define  RO_ACL3   3

/* ****************** Values of possible operation    *************** */
/* ****************** classes                         *************** */

#define  RO_OPC1   1
#define  RO_OPC2   2
#define  RO_OPC3   3
#define  RO_OPC4   4
#define  RO_OPC5   5

/* ****************** Values of r_mtype field in the  *************** */
/* ****************** user data related structures    *************** */

#define  RO_MEMORY 0               /* Memory handle of global heap    */
#define  RO_FILE   1
#ifdef RP0000_MicV0600
#define  RO_GLOHAN 2               /* Memory handle of global heap    */
#endif

/* ****************** ROS events announced by the      ************** */
/* ****************** 'check and wait' function        ************** */
/* ****************** (in r_evtype field of RO_evnthd) ************** */

#define  RO_NOEVET  1            /* No user event was available for   */
				 /* delivery, but internal events     */
				 /* were processed.                   */

#define  RO_TIMEUT  2            /* No user event was available for   */
				 /* delivery, and the check or wait   */
				 /* timer expired.                    */

#define  RO_RBINEV  3            /* Return Buffer Indication Event    */

#define  RO_BNINEV  10           /* Bind   Indication   Event         */
#define  RO_BNCNEV  11           /* Bind   Confirmation Event         */
#define  RO_UBINEV  12           /* Unbind Indication   Event         */
#define  RO_UBCNEV  13           /* Unbind Confirmation Event         */
#define  RO_ABINEV  14           /* Abort  Indication   Event         */

#define  RO_IVINEV  20           /* Invoke Indication Event           */
#define  RO_RSINEV  21           /* Result Indicaiton Event           */
#define  RO_ERINEV  22           /* Error  Indication Event           */
#define  RO_RUINEV  23           /* User-Reject     Indication Event  */
#define  RO_RPINEV  24           /* Provider-Reject Indication Event  */

	 /* --------- ROS events announced within the      ---------- */
	 /* --------- provider-reject-apdu-not-transferred ---------- */
	 /* --------- event                                ---------- */
#define  RO_IVRQEV  30           /* Invoke request is rejected        */
#define  RO_RSRQEV  31           /* Result request is rejected        */
#define  RO_ERRQEV  32           /* Error  request is rejected        */
#define  RO_RURQEV  33           /* User-Reject    is rejected        */

/* ****************** ROS function numbers used in the ************** */
/* ****************** r_fnctnb field of RO_fncthd      ************** */

#define  RO_ATACH    1           /* Attach                            */
#define  RO_DTACH    2           /* Detach                            */
#define  RO_CWAIT    3           /* Check or Wait                     */
#define  RO_PRBUF    4           /* Provide Buffer                    */
#define  RO_REDRQ    5           /* Redirect Request                  */
#define  RO_WAKE     6           /* Wake                              */
#define  RO_INFRM    7           /* Inform                            */
#define  RO_LOGON    9           /* Turn ONN logging                  */
#define  RO_LOGOF   10           /* Turn OFF logging                  */
#define  RO_LOGWR   11           /* Write user record to log-file.    */

#define  RO_BINRQ   20           /* Bind Request                      */
#define  RO_BINRS   21           /* Bind Response                     */
#define  RO_UBNRQ   22           /* Unbind Request                    */
#define  RO_UBNRS   23           /* Unbind Response                   */
#define  RO_ABORQ   24           /* Abort  Request                    */

#define  RO_INVRQ   30           /* Invoke Request                    */
#define  RO_RESRQ   31           /* Result Request                    */
#define  RO_ERRRQ   32           /* Error  Request                    */
#define  RO_RJURQ   33           /* User-Reject Request               */


/* ****************************************************************** */
/*                                                                    */
/*         Possible values of r_result in RO_bnrspb and RO_bncfpb     */
/*                                                                    */
/* ****************************************************************** */

/* Used in response */

#define RO_ACCEPT            0x00000700UL /* - Accepted               */
#define RO_UREJCT            0x00000710UL /* - Rejected permanent     */
					  /*   Reson not defined      */

#define RO_NoReason          0x00000001UL /* Defined reasons          */
#define RO_UnsupApplCtxt     0x00000002UL
#define RO_UnrecClgApTitle   0x00000003UL
#define RO_UnrecClgApInvocId 0x00000004UL
#define RO_UnrecClgAeQual    0x00000005UL
#define RO_UnrecClgAeInvocId 0x00000006UL
#define RO_UnrecCldApTitle   0x00000007UL
#define RO_UnrecCldApInvocId 0x00000008UL
#define RO_UnrecCldAeQual    0x00000009UL
#define RO_UnrecCldAeInvocId 0x0000000aUL

/* Used in confimation  */

	   /* ******************************************************* */
	   /* Note : The range of values that can appear as r_result  */
	   /*        of a RO_bncfev can be more than those that are   */
	   /*        provided with the RO_bnrspb. These additional    */
	   /*        values are values used within the product and    */
	   /*        may be used as diagnostic info by the user to    */
	   /*        get closer to the source of the error. The       */
	   /*        range of possible error values MAY be included   */
	   /*        in an annexure in a Programmer's Guide. However, */
	   /*        these values may change from version to version  */
	   /*        and therefore these Annexures MAY NOT be         */
	   /*        exhaustive.                                      */
	   /* ******************************************************* */

					      /* Accepted             */
#define RO_ISACCEPT(R)  (!(R & 0x00000010UL)) 




/* ****************************************************************** */
/*                                                                    */
/*     Possible values of r_ureasn Unbind Reason in RO_ubrspb         */
/*     UNBINDrsp  and RO_ubcfev UNBINDcnf.                            */
/*     Unbind result is always positive. The reason tells more about  */
/*     the status of the Responder at the time of the Unbind.         */
/* ****************************************************************** */

#define RO_NotDefined   0x00000700L
#define RO_NotFinished  0x00020700L

	   /* ******************************************************* */
	   /* Note : The range of values that can appear as r_ureasn  */
	   /*        of a RO_ubcfev can be more than those that are   */
	   /*        provided with the RO_ubrspb. These additional    */
	   /*        values are values used within the product and    */
	   /*        may be used as diagnostic info by the user.      */
	   /*        The range of possible values MAY be included in  */
	   /*        an annexure in a Programmer's Guide. However,    */
	   /*        these values may change from version to version  */
	   /*        and therefore these Annexures MAY NOT be         */
	   /*        exhaustive or up-to-date.                        */
	   /* ******************************************************* */

/* ****************************************************************** */
/*                                                                    */
/*                   Values of r_result of RO_abinev                  */
/*                                                                    */
/* ****************************************************************** */

#define RO_UABORT        0x00000000UL

	   /* ******************************************************* */
	   /* Note : If the value of r_result is equal to RO_UABORT   */
	   /*        then the source of abort is peer ROSE-user.      */
	   /*        For all other values, the source of abort is     */
	   /*        the provider (the word provider could include the*/
	   /*        ROPM and all underlying layers including those   */
	   /*        layers that are not implemented within the       */
	   /*        RTROS product (like CMX etc.)                    */
	   /* ******************************************************* */


/* ****************************************************************** */
/*                                                                    */
/*                    Remote operation reject data type and constants */
/*                                                                    */
/* ****************************************************************** */

/* ****************** Possible values of r_reason used in     ******* */
/* ****************** RO_rurqpb and indicated in RO_ruinev.   ******* */

#define       RO_RINDU 0x00010800 /* Invoke problem        */
#define       RO_RINUO 0x00010801
#define       RO_RINMA 0x00010802
#define       RO_RINRL 0x00010803
#define       RO_RINIR 0x00010804
#define       RO_RINUL 0x00010805
#define       RO_RINLR 0x00010806
#define       RO_RINUC 0x00010807
        

#define       RO_RREUI 0x00020800 /* Return result problem */
#define       RO_RRERU 0x00020801
#define       RO_RREMR 0x00020802

#define       RO_RERUI 0x00030800 /* Return error problem  */
#define       RO_RERRU 0x00030801
#define       RO_RERCE 0x00030802
#define       RO_RERXE 0x00030803
#define       RO_RERMP 0x00030804

/* ****************** Values which are indicated in a *************** */
/* ****************** provider reject indication.     *************** */

#define       RO_RPOUA 0x00000800 /* General problem       */
#define       RO_RPOMA 0x00000801
#define       RO_RPOBA 0x00000802

#define       RO_NTRAN 0x000008ff /* Not transfered        */

/* ****************** Values of r_inftyp in RO_infrm  *************** */
/* ******************                                 *************** */

#define       RO_GTDCS   0      /* Gets the Defined Context Set       */ 
#define       RO_GTPBF   1      /* Gets the number of provided buffers*/
#define       RO_GPRID   2      /* Gets the Presentation Context Ids  */
/* **************** Values for Ae title forms *********************** */
/* ****************                           *********************** */

#define       RO_AETF1   1     /* AE title form1 (RDN)                */
#define       RO_AETF2   2     /* AE title form2 (Objid)              */ 

/* *************** Values which are to ORed with Ae Title forms ***** */
/* *************** to enable the checking of Ae Title           ***** */

#define       RO_AETCK   0x0000  /* Check AE Title (default)          */
#define       RO_AETNC   0x0100  /* No Checking for AE Title          */



/* ****************************************************************** */
/*                                                                    */
/*                    ROS local syntax                                */
/*                                                                    */
/* ****************************************************************** */

				/* Local syntax of operation code     */
typedef struct {                /* and error code:                    */
  long           F *r_oploc;    /* --> local value                    */
  unsigned long  F *r_opglb;    /* --> global value                   */
} RO_opval;


				/* Local syntax of Application        */
				/* Entity Title (Logical Name of      */
				/* Application). It is OPTIONAL.      */

                                /* AE Title form1                     */
                                /* Attribute Value Assertion (AVA)    */
typedef struct {
  unsigned long F *r_avatyp;    /* AVA Type OBJID                     */
  long             r_avanum;    /* Number of elements in AVA array    */
  char          F *r_avaval;    /* AVA array                          */
}RO_avastr;

typedef struct {
  long             r_rdnnum;    /* number of RDN elements             */
  RO_avastr     F *r_rdnval;    /* Relative distinguished name        */
}RO_rdname;

typedef struct {
  long             r_aptnum;    /* Number of AP title                 */
  RO_rdname     F *r_aptnam;    /* name of RDNs                       */
  long             r_aeqnum;    /* Number of AE qualifier             */
  RO_avastr     F *r_aeqval;    /* AE qualifier values                */
}RO_aetfm1;

typedef struct {
  unsigned long  F *r_aptitl;   /* --> application process title      */
  long           F *r_aequal;   /* --> application entity qualifier   */
}RO_aetfm2;

typedef struct {
  long              r_aptype;   /* Ae title form                      */
  void           F *r_aetval;   /* AE tiltle structure pointer        */
}RO_apname; 

                                /* Invocation Id                      */
                                /* It is OPTIONAL parameter           */
                                /* If any of the pointers are NULL,   */
                                /* then that parameter is not present */
typedef struct {
  long          F *r_apivid;    /* AP Invocation Id                   */
  long          F *r_aeivid;    /* AE Invocation Id                   */
}RO_invcid;

								 
				/* Local syntax of a presentation     */
typedef struct {                /* address:                           */
  char           F *r_paddrf;   /* --> paddress reference             */
  unsigned long     r_paddln;   /* --> paddress length                */
} RO_pradd;
				/* Structures describing the          */
				/* application contexts which are     */
				/* involved in one AE: (I) - (IV))    */

typedef struct {                /* (I)  Abstract syntax list array:   */
  unsigned long F *r_absnam;    /* --> Abstract Syntax Name (Obj. ID) */
  unsigned int     r_idtnum;    /* --> Number of transfer syntaxes    */
				/*     specified in r_idtary.         */
  int           F *r_idtary;    /* --> Transfer Syntax IDs (array of) */
				/*     that are supported for this    */
				/*     Abstract Syntax Name.          */
} RO_absary;                                        

				/* (II) Element of array describing   */
typedef struct {                /*      transfer  syntaxes            */
  unsigned long F *r_trsnam;    /* --> Transfer Syntax Name (Obj. ID) */
} RO_trsary;

				/* (III) Application context list     */
typedef struct {                /*       array element:               */
  unsigned long F *r_apcnam;    /* --> Application Context Name       */
				/*     (Object Identifier)            */
  long             r_pmode;     /* --> Mode selector                  */
  unsigned int     r_asecif;    /* --> ASE configuration Bit Mask for */
				/*     all ASEs included in Appl Ctxt.*/
  unsigned int     r_idsnum;    /* --> Number of Abstract Syntaxes    */
				/*     specified in r_idsary.         */
  int          F  *r_idsary;    /* --> Abstract Syntax Ids (array of) */
				/*     belonging to this Appl. Ctxt.  */
} RO_apcary;
				/* (IV) Applicatation Context         */
typedef struct {                /*     description:                   */
  unsigned int     r_apcnum;    /* --> Number of elements in r_apcary */
				/*     array.                         */
  RO_apcary       F *r_apcary;  /* --> Pointer to an array describing */
				/*     all the supported Application  */
				/*     Contexts and associated info.  */

  unsigned int     r_absnum;    /* --> Number of elements in r_absary */
				/*     array.                         */
  RO_absary       F *r_absary;  /* --> Pointer to an array describing */
				/*     the entire range of abstract   */
				/*     syntaxes supported.            */

  unsigned int     r_trsnum;    /* --> Number of elements in r_trsary */
				/*     array.                         */
  RO_trsary       F *r_trsary;  /* --> Pointer to an array describing */
				/*     the entire range of transfer   */
				/*     syntaxes supported.            */
} RO_apclst;                    

				/*    ROSE user data description for  */
typedef struct {                /*    requests and responses:         */
  int                r_mtype;   /* --> memory type                    */
  char           F *r_fraref;   /* --> fragment reference             */
  unsigned long     r_fralen;   /* --> fragment length                */
} RO_sndata;

				/* Receipt buffer provided by         */
				/* ro_prvbuf for incoming events.     */
				/* Also buffer provided on outbound   */
				/* event for the data to be received  */
				/* on a subsequent associated inbound */
				/* event and indicated in RO_evfrag   */
typedef struct {
  int               r_mtype;    /* --> memory type                    */
  char           F *r_bufref;   /* --> buffer reference               */
  unsigned long     r_buflen;   /* --> buffer length                  */
} RO_buffer;

				/*    ROSE user data description for  */
typedef struct {                /*    indications and confirms:       */
  int               r_mtype;    /* <-- memory type                    */
  int               r_specfl;   /* <-- special flag                   */
  char           F *r_bufref;   /* <-- buffer reference               */
  unsigned long     r_buflen;   /* <-- buffer length                  */
  int               r_absid;    /* <-- abstrct syntax id              */
  unsigned long     r_datlen;   /* <-- data length                    */
} RO_evdata;

typedef struct {                /* <-> Sub fragment array             */
  int               r_absid;    /* <-> abstract syntax identifier     */
  unsigned long     r_sublen;   /* <-> sub fragment length            */
} RO_fraary;
				/*    User information of Bind/       */
				/*    Unbind/Abort request.           */
				/*    Each element in r_fraary (i.e.  */
				/*    each fragment) corresponds to   */
				/*    exactly one EXTERNAL ASN1 type. */
typedef struct {
  int               r_mtype;    /* --> memory type                    */
  char           F *r_fraref;   /* --> fragment reference             */
  unsigned long     r_fralen;   /* --> fragment total length          */
  unsigned long     r_fracnt;   /* --> number of sub fragments        */

  RO_fraary         r_fraary[RO_maxExternals]; /* -->  len & absid    */

} RO_snfrag;

				/* User information of Bind/Unbind    */
				/* and Abort indication (ACSE user    */
				/* data) and Invoke/Result/Error      */
				/* indication (ROSE user data).       */
				/* Each element in r_fraary (i.e.     */
				/* each fragment) corresponds to      */
				/* exactly one EXTERNAL ASN1 type.    */
				/* Note: The RO_buffer passed during  */ 
				/*       BIND/UNBIND request may be   */
				/*       returned back as RO_evfrag in*/ 
				/*       the corresponding confirm.   */
typedef struct {
  int               r_mtype;    /* <-- memory type                    */
  int               r_specfl;   /* <-- special flag                   */
  char           F *r_bufref;   /* <-- buffer reference               */
  unsigned long     r_buflen;   /* <-- buffer length                  */
  unsigned long     r_fracnt;   /* <-- number of sub fragments        */
				/*     If r_fracnt is 0 then no data  */
				/*     is in buffer.                  */
  RO_fraary         r_fraary[RO_maxExternals]; 
				/* <-- len & absid                    */
} RO_evfrag;                    


/* ****************************************************************** */
/*                                                                    */
/*                    Further ROS constructed data types              */
/*                                                                    */
/* ****************************************************************** */

				/* Provider reject indication event   */
typedef struct {                /* data, if reason = RO_NTRAN:        */
  int             r_evtype;     /* <-- send request type              */
  unsigned long   r_reason;     /* <-- user reject reason             */
  RO_opval        r_srval;      /* <-- send request oper. or error val*/
  RO_evdata       r_evdata;     /* <-- event data  description        */
} RO_prdata;

				/* element of presentation context    */
				/* array                              */
typedef struct {
  int          r_absid;         /* <-- Abstract syntax identifier     */
  int          r_trsid;         /* <-- Transfer syntax identifier     */
  long         r_pctid;         /* <-- Presentation context identifier*/
} RO_prcxt;


/* ****************************************************************** */
/*                                                                    */
/*                    ROS event parameter block structures            */
/*                                                                    */
/* ****************************************************************** */

/* ****************** ROS event header ****************************** */
/* ******************                  ****************************** */

typedef struct {                /* Common event header:               */
  void           F *r_extent;
  int               r_evtype;   /* <-- event code number              */
} RO_evnthd;

/* ****************** Local events ********************************** */
/* ******************              ********************************** */

typedef struct {                /* return buffer indication event:    */
  RO_evnthd       r_evnthd;     /* <-- event header                   */
  RO_buffer       r_buffer;     /* <-- buffer describtion             */
} RO_rbinev;

/* ****************** Association handling events ******************* */
/* ******************                             ******************* */

typedef struct {                /* bind indication event:             */
   RO_evnthd      r_evnthd;     /* <-- event header                   */
   unsigned long  r_aplid;      /* <-- application identifier         */
   unsigned long  r_assid;      /* <-- association identifier         */
   long           r_pid;        /* <-- process identifier             */
   int            r_ptcon;      /* <-- calling application context    */
   RO_apname      r_aetitl;     /* <-- calling application reference  */
   RO_invcid      r_invcid;     /* <-- calling AP&AE Invocation Ids   */
   RO_pradd       r_pradd;      /* <-- calling presentation address   */
   RO_apname      r_cldaet;     /* <-- called application reference   */
   RO_invcid      r_cldinv;     /* <-- called AP&AE Invocation Ids    */
   RO_pradd       r_cldpad;     /* <-- called presentation address    */
   RO_evfrag      r_evfrag;     /* <-- user receive data              */
} RO_bninev;

typedef struct {                /* bind confirmatation event:         */
  RO_evnthd       r_evnthd;     /* <-- event header                   */
  unsigned long   r_assid;      /* <-- association identifier         */
  unsigned long   r_result;     /* <-- accept or reject reason        */
  int             r_ptcon;      /* <-- responding application context */
  RO_apname       r_aetitl;     /* <-- responding application refernce*/
  RO_invcid       r_invcid;     /* <-- responding AP&AE Invocation Ids*/
  RO_pradd        r_pradd;      /* <-- responding presentation address*/
  RO_evfrag       r_evfrag;     /* <-- user receive data              */
} RO_bncnev;

typedef struct {                /* unbind indication event:           */
  RO_evnthd       r_evnthd;     /* <-- event header                   */
  unsigned long   r_assid;      /* <-- association identifier         */
  RO_evfrag       r_evfrag;     /* <-- user receive data              */
} RO_ubinev;

typedef struct {                /*  unbind confirmation event:        */
  RO_evnthd       r_evnthd;     /* <-- event Header                   */
  unsigned long   r_assid;      /* <-- association identifier         */
  unsigned long   r_ureasn;     /* <-- unbind response reason         */
  RO_evfrag       r_evfrag;     /* <-- user receive data              */
} RO_ubcnev;

typedef struct {                /* association abort indicat. event:  */
  RO_evnthd       r_evnthd;     /* <-- event header                   */
  unsigned long   r_assid;      /* <-- association identifier         */
  unsigned long   r_result;     /* <-- abort reason value (mask)      */
  unsigned long   r_adinfo;     /* <-- additional reason parameter    */
  RO_evfrag       r_evfrag;     /* <-- user receive data              */
} RO_abinev;

/* ****************** Remote operation events *********************** */
/* ******************                         *********************** */

typedef struct {                /* Invoke indication event:           */
  RO_evnthd         r_evnthd;   /* <-- event header                   */
  unsigned long     r_assid;    /* <-- association identifier         */
  RO_opval          r_opval;    /* <-- operation value                */
  long              r_invid;    /* <-- invoke identifier              */
  long           F *r_lnkid;    /* <-- linked identifier              */
  RO_evdata         r_evdata;   /* <-- user receive data              */
} RO_ivinev;

typedef struct {                /* Result indication event:           */
  RO_evnthd       r_evnthd;     /* <-- event header                   */
  unsigned long   r_assid;      /* <-- association identifier         */
  long            r_invid;      /* <-- invoke identifier              */
  RO_opval        r_opval;      /* <-- operation value                */
  RO_evdata       r_evdata;     /* <-- user receive data              */
} RO_rsinev;

typedef struct {                /* Error indication event:            */
  RO_evnthd       r_evnthd;     /* <-- event header                   */
  unsigned long   r_assid;      /* <-- association identifier         */
  RO_opval        r_erval;      /* <-- error value                    */
  long            r_invid;      /* <-- invoke identifier              */
  RO_evdata       r_evdata;     /* <-- user receive data              */
} RO_erinev;

typedef struct {                /* User reject indication` event:     */
  RO_evnthd       r_evnthd;     /* <-- event header                   */
  unsigned long   r_assid;      /* <-- association identifier         */
  unsigned long   r_reason;     /* <-- reject reason                  */
  long            r_invid;      /* <-- invoke identifier              */
} RO_ruinev;

typedef struct {                /* Provider reject indication event:  */
  RO_evnthd       r_evnthd;     /* <-- event header                   */
  unsigned long   r_assid;      /* <-- association identifier         */
  unsigned long   r_reason;     /* <-- reject reason                  */
  long        F  *r_invid;      /* <-- invoke identifer               */
} RO_rpinev;

/* ****************** Event union *********************************** */
/* ******************             *********************************** */

typedef union {
  RO_evnthd         r_evnthd;   /* event header                       */

  RO_rbinev         r_rbinev;   /* return buffer indication event     */
  RO_bninev         r_bninev;   /* bind indication event              */
  RO_bncnev         r_bncnev;   /* bind confirmation event            */
  RO_ubinev         r_ubinev;   /* unbind indication event            */
  RO_ubcnev         r_ubcnev;   /* unbind confirmation event          */
  RO_abinev         r_abinev;   /* abort indication event             */
  RO_ivinev         r_ivinev;   /* invoke indication event            */
  RO_rsinev         r_rsinev;   /* result indication event            */
  RO_erinev         r_erinev;   /* error indication event             */
  RO_ruinev         r_ruinev;   /* user reject indication event       */
  RO_rpinev         r_rpinev;   /* provider reject indication event   */

				/* Parameter block buffer.            */
				/* Denotes the maximum size of the    */
				/* union. All the pointers present    */
				/* within the event blocks in the     */
				/* union refer to memory within the   */
				/* space after the event block but    */
				/* within the r_evpbbuf array.        */
  char              r_evpbbuf[RO_maxEVENTBUFFER];
} RO_evtpb;
                

/* ****************************************************************** */
/*                                                                    */
/*                    ROS function parameter structures               */
/*                                                                    */
/* ****************************************************************** */

/* ****************** ROS function header *************************** */
/* ******************                     *************************** */

typedef struct {                /* function header:                   */
  int                r_cont;    /* --> continue flag                  */
  int                r_fnctnb;  /* --> function call identifier       */
  int                r_versnb;  /* --> version number                 */

  void            F *r_extent;  /* <-> extention pointer. Version     */
				/*     specific changes shall be      */
				/*     introduced here.               */
  int                r_errval;  /* <-- error value                    */
  int                r_erclas;  /* <-- error class                    */
  int                r_retcod;  /* <-- return code                    */
} RO_fncthd;

/* ****************** Local functions ******************************* */
/* ******************                 ******************************* */

typedef struct {                /* ro_atach:                          */
  RO_fncthd       r_hdinfo;     /* <-> function header                */
  unsigned long   r_aplid;      /* <-- application identifier         */
  long            r_pid;        /* --> process identifier             */
  RO_apname       r_aetitl;     /* --> self application reference     */
  RO_invcid       r_invcid;     /* --> self AP&AE Invocation Ids      */
  RO_pradd        r_pradd;      /* --> self presentation address      */
  RO_apclst       r_apclst;     /* --> application context description*/
} RO_attapb;

typedef struct {                /* ro_dtach:                          */
  RO_fncthd       r_hdinfo;     /* <-> function header                */
  unsigned long   r_aplid;      /* --> application identifier         */
} RO_detapb;

typedef struct {                /* ro_cwait:                          */
  RO_fncthd         r_hdinfo;   /* <-> function header                */
  unsigned long     r_wtime;    /* --> wait time                      */
  RO_evtpb       F *r_evntrf;   /* --> event buffer reference         */
} RO_chwtpb;

typedef struct {                /* ro_prbuf:                          */
  RO_fncthd       r_hdinfo;     /* <-> function header                */
  unsigned long   r_aplid;      /* --> application identifier         */
  RO_buffer       r_buffer;     /* --> receive buffer                 */
} RO_pvbfpb;

typedef struct {                /* ro_redrq:                          */
  RO_fncthd       r_hdinfo;     /* <-> function header                */
  unsigned long   r_assid;      /* --> association identifier         */
  long            r_pid;        /* --> process identifier             */
} RO_rdrqpb;

typedef struct {                /* ro_wake:                           */
  RO_fncthd       r_hdinfo;     /* <-> function header                */
  long            r_pid;        /* --> process identifier             */
} RO_wakepb;

typedef struct {                /* ro_inform (inftyp = RO_GTDCS):     */
  unsigned long      r_assid;   /* -->  Association ID                */
  int                r_panum;   /* -->  Numb. of array elements       */
  int                r_pcnum;   /* <--  Number of contexts found      */
  RO_prcxt           r_prcxt[RO_maxExternals];
				/* <--  Presentation context array    */
} RO_gtdcif;

typedef struct {                /* ro_inform (inftyp = RO_GTPBF):     */
  unsigned long   r_aplid;      /* -->  application identifier        */
  int             r_number;     /* <--  Number of appication specific */
				/*      buffers available.            */
} RO_gtpbif;

typedef struct {                /* ro_inform (inftyp = RO_GPRID):     */
  unsigned long      r_aplid;   /* -->  Application ID                */
  int                r_apcon;   /* -->  Application Context Id        */
  int                r_absid;   /* --> Abstract syntax identifier     */
  long               r_pctid;   /* <-- Presentation context identifier*/
  
} RO_gtprid;

						 
typedef struct {                /* ro_inform:                         */
  RO_fncthd      r_hdinfo;      /* <-> function header                */
  int            r_inftyp;      /* -->inform type                     */
  void        F *r_infval;      /* <->pointer to the inform type      */
				/* structure                          */
}RO_inform;                      

/* ****************** Association handling functions **************** */
/* ******************                                **************** */

typedef struct {                /* ro_binrq:                          */
  RO_fncthd       r_hdinfo;     /* <-> function header                */
  unsigned long   r_assid;      /* <-- association identifier         */
  unsigned long   r_aplid;      /* --> application identifier         */
  int             r_aclass;     /* --> association class              */
  int             r_apcon;      /* --> calling application context    */
  RO_apname       r_aetitl;     /* --> called application reference.  */
  RO_invcid       r_invcid;     /* --> called Ap&AE Invocation Ids    */
  RO_pradd        r_pradd;      /* --> called presentation address.   */
  RO_snfrag       r_sndfra;     /* --> user send data                 */
  RO_buffer       r_buffer;     /* --> user receive data              */
} RO_bnrqpb;

typedef struct {                /* ro_binrs:                          */
  RO_fncthd       r_hdinfo;     /* <-> function header                */
  unsigned long   r_assid;      /* --> association identifier         */
  int             r_aclass;     /* --> association class              */
  unsigned long   r_result;     /* --> accept or reject reason        */
  int             r_apcon;      /* --> responding application context */
  RO_apname       r_aetitl;     /* --> responding application refernce*/
  RO_invcid       r_invcid;     /* --> responding Ap&AE Invocation Ids*/
  RO_pradd        r_pradd;      /* --> responding presentation address*/
  RO_snfrag       r_sndfra;     /* --> user send data                 */
} RO_bnrspb;

typedef struct {                /* ro_ubnrq:                          */
  RO_fncthd       r_hdinfo;     /* <-> function header                */
  unsigned long   r_assid;      /* --> association identifier         */
  RO_snfrag       r_sndfra;     /* --> user send Ddta                 */
  RO_buffer       r_buffer;     /* --> user receive data              */
} RO_ubrqpb;

typedef struct {                /* ro_ubnrs:                          */
  RO_fncthd       r_hdinfo;     /* <-> function header                */
  unsigned long   r_assid;      /* --> association identifier         */
  unsigned long   r_ureasn;     /* --> Unbind response reason         */
  RO_snfrag       r_sndfra;     /* --> user send data                 */
} RO_ubrspb;

typedef struct {                /* ro_aborq:                          */
  RO_fncthd       r_hdinfo;     /* <-> function header                */
  unsigned long   r_assid;      /* --> association identifier         */
  RO_snfrag       r_sndfra;     /* --> user send data                 */
} RO_abrqpb;

/* ****************** Remote operation functions ******************** */
/* ******************                            ******************** */

typedef struct {                /* ro_invrq:                          */
  RO_fncthd         r_hdinfo;   /* <-> function header                */
  unsigned long     r_assid;    /* --> association identifer          */
  long              r_invid;    /* --> invoke identifier              */
  long           F *r_lnkid;    /* --> linked identifier              */
  int               r_oclass;   /* --> operation class                */
  RO_opval          r_opval;    /* --> operation value                */
  int               r_absid;    /* --> abstract syntax identifier     */
  RO_sndata         r_sndata;   /* --> user send data                 */
  RO_buffer         r_buffer;   /* --> user receive data              */
} RO_ivrqpb;

typedef struct {                /* ro_resrq:                          */
  RO_fncthd       r_hdinfo;     /* <-> function header                */
  unsigned long   r_assid;      /* --> association identifier         */
  long            r_invid;      /* --> invoke identifier              */
  RO_opval        r_opval;      /* --> operation value                */
  int             r_absid;      /* --> abstract syntax identifier     */
  RO_sndata       r_sndata;     /* --> user send data                 */
} RO_rsrqpb;

typedef struct {                /* ro_errrq:                          */
  RO_fncthd       r_hdinfo;     /* <-> function header                */
  unsigned long   r_assid;      /* --> association identifier         */
  long            r_invid;      /* --> invoke identifier              */
  RO_opval        r_erval;      /* --> error value                    */
  int             r_absid;      /* --> abstract syntax identifier     */
  RO_sndata       r_sndata;     /* --> user send data                 */
} RO_errqpb;

typedef struct {                /* ro_rjurq:                          */
   RO_fncthd      r_hdinfo;     /* <-> function header                */
   unsigned long  r_assid;      /* --> association identifier         */
   long           r_invid;      /* --> invoke identifier              */
   int            r_absid;      /* --> abstract syntax identifier     */
   unsigned long  r_reason;     /* --> reject reason                  */
 } RO_rurqpb;

/* ****************** ROS logging functions ************************* */
/* ******************                       ************************* */

typedef struct {                /* ro_logon:                          */
   RO_fncthd        r_hdinfo;   /* <-> function header                */
   char          F *r_fname;    /* --> logging file name              */
   int              r_fmode;    /* --> open mode                      */
   unsigned long    r_rcnum;    /* --> max. records to log in file    */
				/*     wraps around and overwrites    */
				/*     first record when this is      */
				/*     exceeded.                      */
   unsigned long    r_option;   /* --> logging options (constant for  */
				/*     the entire duration of logging)*/
 } RO_lgonpb;

typedef struct {                /* ro_logof:                          */
   RO_fncthd      r_hdinfo;     /* <-> function header                */
 } RO_lgofpb;

typedef struct {                /* ro_logwr:                          */
   RO_fncthd        r_hdinfo;   /* <-> function header                */
   unsigned long    r_option;   /* --> logging options (allows to     */
				/*     dynamically change the r_option*/
				/*     provided during log ON         */
   long             r_rectyp;   /* --> logging record type.           */
				/*     Should be >= 0 (user record)   */
   unsigned long    r_reclen;   /* --> logging record length          */
   char          F *r_recptr;   /* --> pointer to logging record      */
 } RO_lgwrpb;

/* ****************** RO_excom function ***************************** */
/* ******************                   ***************************** */

typedef union  {
  RO_fncthd       r_hdinfo;     /* Function header                    */
  RO_attapb       r_attapb;     /* Local functions                    */
  RO_detapb       r_detapb;
  RO_chwtpb       r_chwtpb;
  RO_pvbfpb       r_pvbfpb;    
  RO_rdrqpb       r_rdrqpb;
  RO_wakepb       r_wakepb;
  RO_inform       r_inform;

  RO_lgonpb       r_lgonpb;     /* Ros logging functions              */
  RO_lgofpb       r_lgofpb;
  RO_lgwrpb       r_lgwrpb;

  RO_bnrqpb       r_bnrqpb;     /* Association handling functions     */
  RO_bnrspb       r_bnrspb;
  RO_ubrqpb       r_ubrqpb;
  RO_ubrspb       r_ubrspb;
  RO_abrqpb       r_abrqpb;

  RO_ivrqpb       r_ivrqpb;     /* Remote operations functions        */
  RO_rsrqpb       r_rsrqpb;
  RO_errqpb       r_errqpb;
  RO_rurqpb       r_rurqpb;
} RO_excopb;


/* ****************************************************************** */
/*                                                                    */
/*                    Extern declaration of ROS function call's       */
/*                                                                    */
/* ****************************************************************** */

#ifdef __STDC__

int F P ro_excom (RO_excopb F *);/* Execute communication              */

int F P ro_atach (RO_attapb F *);/* Local functions                    */
int F P ro_dtach (RO_detapb F *);
int F P ro_cwait (RO_chwtpb F *);
int F P ro_prbuf (RO_pvbfpb F *);
int F P ro_redrq (RO_rdrqpb F *);
int F P ro_wake  (RO_wakepb F *);
int F P ro_infrm (RO_inform F *);

int F P ro_logon (RO_lgonpb F *);/* Ros logging functions              */
int F P ro_logof (RO_lgofpb F *);
int F P ro_logwr (RO_lgwrpb F *);

int F P ro_binrq (RO_bnrqpb F *);/* Association handling functions     */
int F P ro_binrs (RO_bnrspb F *);
int F P ro_ubnrq (RO_ubrqpb F *);
int F P ro_ubnrs (RO_ubrspb F *);
int F P ro_aborq (RO_abrqpb F *);

int F P ro_invrq (RO_ivrqpb F *);/* Remote operations functions        */
int F P ro_resrq (RO_rsrqpb F *);
int F P ro_errrq (RO_errqpb F *);
int F P ro_rjurq (RO_rurqpb F *);
#else

extern  int   ro_excom();       /* Execute communication              */

extern  int   ro_atach();       /* Local functions                    */
extern  int   ro_dtach();
extern  int   ro_cwait();
extern  int   ro_prbuf();
extern  int   ro_redrq();
extern  int   ro_wake();
extern  int   ro_infrm();


extern  int   ro_logon();       /* Ros logging functions              */
extern  int   ro_logof();
extern  int   ro_logwr();

extern  int   ro_binrq();       /* Association handling functions     */
extern  int   ro_binrs();
extern  int   ro_ubnrq();
extern  int   ro_ubnrs();
extern  int   ro_aborq();

extern  int   ro_invrq();       /* Remote operation functios          */
extern  int   ro_resrq();
extern  int   ro_errrq();
extern  int   ro_rjurq();

#endif

#endif	/* _ROSS_H */
