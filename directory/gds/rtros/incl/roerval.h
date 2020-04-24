/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roerval.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:39  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:35  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:34:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:15  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:36  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:12  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:13  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:12  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:17  keutel
 * 	creation
 * 	[1994/03/21  13:26:36  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROERVAL_H
#define _ROERVAL_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roerval_rcsid[] = { "@(#) $RCSfile: roerval.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:39 $" } ;
#endif

/* *********************************************************************** */
/*                                                                         */
/*  * COPYRIGHT  (C)                                                       */
/*  * Siemens Nixdorf Informationssysteme AG 1993                          */
/*  * All rights reserved                                                  */
/*  *                                                                      */
/*  * This software is furnished under licence and may be used only        */
/*  * in accordance with the terms of that licence and with the            */
/*  * inclusion of the above copyright notice.                             */
/*  * This software may not be provided or otherwise made available        */
/*  * to, or used by, any other person. No title to or ownership of        */
/*  * the software is hereby  transferred.                                 */
/*                                                                         */
/* *********************************************************************** */

/*daton ****************************************************************** */
/*                                                                         */
/* TYPE         : INCLUDE                                                  */
/*                                                                         */
/* NAME         : roerval.h                                                */
/*                                                                         */
/* AUTHOR       : Milind M. Aphale                                         */
/* DATE         : 93-06-17                                                 */
/*                                                                         */
/* COMPONENT    : RTROS error values                                       */
/*                                                                         */
/* PRD#/VERS.   : RTROS V-3.00                                             */
/*                                                                         */
/* DESCRIPTION  : Error codes                                              */
/*                                                                         */
/* Error names (identifiers):                                              */
/*                                                                         */
/* - Format is R_BBBBBB                                                    */
/*   - "R_"  = always fixed for all constants.                             */
/*   - A     = One alphabet identifying module to which name belongs.      */
/*             (not followed by all the modules - then A=B)                */
/*   - BBBBB = Five alphabets describing the error.                        */
/*                                                                         */
/*   Use of the "A" fields by the protocol machines:                       */
/*   ---------------------------|------------------|                       */
/*    Protocol machine / module | 'A' of error name|                       */
/*   ---------------------------|------------------|                       */
/*    Association Control       |      "A"         |                       */
/*    Reliable Transfer         |      "X"         |                       */
/*    Remote Operations         |      "O"         |                       */
/*    Presentation              |      "P"         |                       */
/*    Session                   |      "S"         |                       */
/*    Transport                 |      "T"         |                       */
/*    Timer                     |      "N"         |                       */
/*    Dispatcher                |      "D"         |                       */
/*    Utility                   |      "F"         |                       */
/*    Authorization disk access |      "C"         |                       */
/*    Common error values       |      "C"         |                       */
/*    External Interface(RTS)   |      "G"         |                       */
/*    External Interface(ROS)   |      "H"         |                       */
/*    Lower Interface(on OSS)   |      "I"         |                       */
/*    Lower Interface(on CMX)   |      "J"         |                       */
/*    Lower Interface(SOCKETS)  |      "K"         |                       */
/*    Upper Control             |      "U"         |                       */
/*    Lower Control             |      "L"         |                       */
/*    REMAINING                 |   undefined      |                       */
/*   ---------------------------|------------------|                       */
/*                                                                         */
/*                                                                         */
/* Error values:                                                           */
/*                                                                         */
/* - All error values are of type "signed short int" RTS-internally        */
/*   and of type "int" at the IRTS interface                               */
/*   Attention: Therefore, the values may not exceed the max. range        */
/*              of 32K-1 = 32767.                                          */
/*                                                                         */
/* - Conventions for representing error values:                            */
/*   - Decimal representation                                              */
/*   - Two digits represent the IRTS error class and return code           */
/*   - Format is WWWYZ                                                     */
/*                                                                         */
/*     WWW = Decimal value used for a continuos ordering.                  */
/*           Starts with 0, limited to 326 (caused by signed short)        */
/*           The 3rd digit is kept to be 0 when entering new modules.      */
/*     Y   = Error class                                                   */
/*           0   : Not specified                                           */
/*           1   : Parameter failure                                       */
/*           2   : Unknown state (wrong call sequence)                     */
/*           3   : Unknown version                                         */
/*           4   : System failure (operating system)                       */
/*           5   : Intern failure (RTS intern)                             */
/*           7   : Failure in OSS                                          */
/*           8   : Failure in CMX                                          */
/*           9   : Means "not used", e.g. key diskette errors              */
/*     Z   = Return code                                                   */
/*           1   : Temporary error                                         */
/*           2   : Permanent error                                         */
/*           3   : Call error / formal call error                          */
/*           9   : Warning                                                 */
/*                                                                         */
/*     Example: R_LMEMRY (local memory error) = 1042                       */
/*              OS system call error (4) and permanent error (2).          */
/*                                                                         */
/*------------------------------------------------------------------------ */
/*        Module                             Range ( values for www)       */
/*------------------------------------------------------------------------ */
/* Common error Values                                                     */
/* Authorization Disk Access                 00                            */
/*                                                                         */
/* Utility                                   10,20,30,40                   */
/* Dispatcher                                50,60,70,80                   */
/* Timer                                     90                            */
/*                                                                         */
/* Lower Interface ( COMMON )                100                           */
/* Lower Interface (OSS/CMX/Sockets)         110                           */
/* External Interface ( COMMON )             120,130,140,150               */
/* External Interfacr ( RTS )                160,170,180,190,200           */
/* External Interface ( ROS )                210,220                       */
/* Upper Control ( RTS )                     230                           */
/* Upper Control ( ROS )                     240                           */
/* Association Control (ACSE)                250                           */
/* Lower Control                             260                           */
/* Reliable Transfer                         270                           */
/* Remote operations                         280                           */
/* Presentation                              290,300                       */
/* Session                                   310                           */
/* Transport                                 320                           */
/*                                                                         */
/*------------------------------------------------------------------------ */
/*                                                                         */
/* SYSTEM DEPENDENCE :                                                     */
/*                                                                         */
/* HISTORY      : sccsid  =  @(#) roerval.h 1.1 93/11/29 RTROS-V3.00                   */
/*                                                                         */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM#      */
/*           |        |                                |    |              */
/* 3.00 K00  |93-06-17|  original                      |ma  |              */
/*           |        |                                |    |              */
/*           |        |                                |    |              */
/*           |        |                                |    |              */
/*datoff ***************************************************************** */

/* ********************* COMMON ERROR VALUES ************************ */
/* ******************* AUTHORIZATION DISK ERROR ********************* */
/* Range : 00 */

/* ****************** Authorization or key diskette access *************** */
/* ****************** errors                               *************** */

#define R_KEYINT            92      /* Check key internal error            */
#define R_KEYRUN           192      /* No running authorization            */
#define R_KEYFRM           292      /* SAFE file format error              */
#define R_KEYACC           392      /* SAFE file access error              */
#define R_KEYREA           492      /* SAFE file read   error              */
#define R_KEYREM           592      /* SAFE remote access error            */

/* *************************  UTILITY ******************************* */
/* Range : 10 ,20, 30,40 */

/* ****************** Common error values ******************************** */
/* ******************                     ******************************** */

#define R_LMEMRY          1042      /* Local memory error                  */
#define R_LALLCE          1142      /* Allocation error, local heap        */
#define R_LLOCKE          1242      /* Lock error, local heap              */
#define R_LULOKE          1342      /* Unlock error, local heap            */
#define R_LFREEE          1442      /* Free error, local heap              */
#define R_LRALLE          1542      /* Realloc error, local heap           */

#define R_GMEMRY          1642      /* Global memory error                 */
#define R_GALLCE          1742      /* Allocation error, global heap       */
#define R_GLOCKE          1842      /* Lock error, global heap             */
#define R_GULOKE          1942      /* Unlock error, global heap           */
#define R_GFREEE          2042      /* Free error, global heap             */
#define R_GRALLE          2142      /* Realloc error, global heap          */

#define R_WOBJVL          2202      /* Wrong object identifier value       */

/* ****************** Error values ROUTIL ******************************** */
/* ****************** (Utility)           ******************************** */

#define R_LENULL          2352      /* OBJCPY: length = 0                  */
#define R_MAXOBJ          2452      /* OBJCPY: wrong object id. size       */
#define R_DIFOBJ          2552      /* OBJCMP: object id.s different       */
#define R_LEVUNK          2652      /* LOGGING: level unknown              */
#define R_FICLER          2742      /* LOGGING: file close error           */
#define R_FICRER          2842      /* LOGGING: file creat error           */
#define R_FIOPER          2942      /* LOGGING: file open error            */
#define R_FISEER          3042      /* LOGGING: file seek error            */
#define R_FIWRER          3142      /* LOGGING: file write error           */
#define R_NOTSUP          3242      /* LOGGING: not supported call to      */
				    /*          openLog/closeLog           */

#define R_PARMER          3342      /* Invalid calling parameter           */
#define R_LENERR          3442      /* Invalid string length               */

#define R_FOPERR          3542      /* Error in fopen procedure            */
#define R_FCLERR          3642      /* Error in fclose procedure           */
#define R_FSEERR          3742      /* Error in fseek procedure            */
#define R_FTEERR          3842      /* Error in ftell procedure            */
#define R_FWRERR          3942      /* Error in fwrite procedure           */
#define R_FREERR          4042      /* Error in fclose procedure           */
#define R_FFLERR          4142      /* Error in fflush procedure           */
#define R_FLOCKE          4242      /* Error locking file                  */
#define R_FULOCK          4342      /* Error unlocking file                */

#define R_WRGTAG          4442      /* Wrong tag in decode                 */
#define R_OCTPAK          4542      /* OCTET STRING decode error           */

#define R_FNMERR          4642      /* Error in tmpnam procedure           */ 
#define R_FRMERR          4742      /* Error in remove procedure           */

#define R_FTSKID          4842      /* Invalid Current Task Id.            */

/* ************************* DISPATCHER ***************************** */
/* Range : 50,60 70, 80 */


#define R_AUPEN           5021      /* Event which announces the loss of   */
                                    /*  the association is still pending   */
                                    /*  (will be given during next         */
                                    /*  rt_event call)                     */
#define R_SPLFLG          5113      /* Special flag condition "true"       */

#define R_WAPLID          5213      /* Wrong application identifier        */
#define R_WASSOC          5313      /* Wrong association identifier        */

#define R_NASSOC          5452      /* Association not found               */
#define R_PMINVE          5552      /* PM already invoked                  */
#define R_PUSHER          5652      /* Error in push event                 */
#define R_NOSCBB          5752      /* Fatal error with SCB                */

#define R_DPPANC          5852      /* Dispatcher in panic state           */
                                    /*  after permanent error              */

#define R_NPSTCF          5952      /* Stack config. number unkown         */
#define R_NPMFCT          6052      /* No PM function for PM number        */
#define R_NCLFCT          6152      /* No PM clear function                */
#define R_NIFFCT          6252      /* No PM info function                 */
#define R_NPDVEN          6352      /* No PM encoding function             */
#define R_NPDVDE          6452      /* No PM decoding function             */
#define R_NPDVCF          6552      /* No PM clear PDV tree function       */
#define R_NAMLEN          6652      /* ACB name too long                   */
#define R_CONERR          6752      /* No concatenation allowed            */
#define R_IFARYS          6852      /* Array in inform-call too small      */
#define R_RDBUNT          6952      /* Unknown record type in RDB          */
#define R_ACBNOF          7052      /* ACB not found                       */
#define R_RDBINT          7152      /* Internal Recovery Management        */
                                    /*  failure                            */
#define R_RDBLEN          7252      /* Block size too short for data       */
                                    /*  written to RDB file                */
#define R_RDBWBK          7352      /* Wrong block number                  */
#define R_RDBREE          7452      /* Fatal read error RDB file           */
#define R_RDBW12          7552      /* Mismatch of read/write type or      */
                                    /*  wrong order of readinfo function   */
                                    /*  and read function                  */
#define R_RDBEND          7652      /* Read after readinfoRdb function     */
                                    /*  has returned EOF                   */
#define R_D410EN          7713      /*  No pm for 410 encode function      */
#define R_D410DE          7813      /*  No Pm for 410 decode cnf fun.      */
#define R_DSNIEN          7913      /*  No Pm for Sni PTS encod cnf. fun   */
#define R_DSNIDE          8013      /*  No Pm for Sni PTS decod cnf. fun   */
/* *************************** TIMER ********************************* */
/* Range : 90 */

#define R_TUNEVT          9052      /* Unknown event                       */

/* ****************** Lower Interface (Common) *********************** */ 
/* Range : 100 */

#define R_LLSQFL         10052      /* Local sequence flow error           */
#define R_LUNKEV         10152      /* Unknown event                       */
#define R_LNONET         10252      /* Network not started                 */
#define R_LNOADR         10352      /* No local address found              */
#define R_UNINPA         10452      /* Unknown incoming parameter          */
#define R_SDQNEM         10552      /* The send queue is not empty         */
#define R_STBUTS         10652      /* Store buffer too small              */
#define R_NOTIMP         10752      /* Not implemented                     */
#define R_NOENME         10852      /* Not enough memory in buffer         */
#define R_STOERR         10952      /* Error in a STORE function call      */


/* ************** Lower Interface (OSS/CMX/SOCKETS) ***************** */  
/* Range : 110 */

#define R_JNEICX         11052      /* Entity context does not exist */

#define R_UNOUPA         11152      /* OSS unknown outgoing parameter      */
#define R_ISYSOS         11242      /* Error on system call in OSS         */
#define R_IOSSER         11372      /* OSS error                           */
#define R_WAKEER         11472      /* Error on the s_wake call            */
#define R_ICMXER         11582      /* Error on transport system (CMX)     */
#define R_LSOCKE         11682      /* Error on sockets system             */
#define R_LNO1ST         11782      /* Error due to multiple attaches.     */

/* ****************** External Interface (Common) ******************* */ 
/* Range : 120,130,140,150 */

   /* *********** general errors **************************************** */
#define R_FCTCAL         12023      /* Wrong function call by RTROS user  */
                                    /* (not allowed at current RTROstate) */
#define R_NSUPVS         12113      /* Not supported VERSION               */
#define R_NSUPCA         12213      /* Not supported call in this version  */
#define R_PROCID         12342      /* Not able to compute process         */
                                    /*  task identifier                    */
#define R_TPBFCT         12442      /* Not able to compute tpdu call       */
                                    /*  back function pointer              */
#define R_NOBHPT         12513      /* No basic pointer or header pointer  */
				    /*  (pbhead)                           */

/* *********** address errors ******************************************** */
#define R_ADDLEN         12613      /* No presentation address length      */
                                    /*  in req/rsp call                    */
#define R_ADDREF         12713      /* No presentation address reference   */
                                    /*  in req/rsp call                    */
#define R_ADDCMP         12813      /* Length of presentation address      */
				    /*  buffer too small in ind/cnf        */

/* *********** application entity title errors *************************** */
#define R_ATITLE         12913      /* Wrong attribute for APL. TITLE      */
                                    /*  in req/rsp call                    */
#define R_OTITLE         13013      /* Wrong object id. for APL. TITLE     */
                                    /*  in req/rsp call                    */
#define R_RDNSRG         13113      /* REL. DIS. NAME not closed by '\0'   */
                                    /*  in req/rsp call                    */
#define R_QULSRG         13213      /* REL. DIS. NAME QUAL. not            */
                                    /*  closed by '\0'                     */
#define R_RDNCMP         13313      /* Length of REL. DIS. NAME buffer     */
				    /*  too small in ind/cnf call          */
#define R_RDQCMP         13413      /* Length of REL. DIS. NAME qualifier  */
				    /*  buffer too small in ind/cnf        */

/* *********** attach errors ********************************************* */

#define R_AAPCXE         13513      /* Too much APL. CONTEXTS or wrong     */
                                    /*  object identifier                  */

#define R_NUABEL         13613      /* Wrong ABSTRACT SYNTAX object id.    */
#define R_PAPCXE         13713      /* Wrong number of ABSTRACT SYNTAXES   */
                                    /*  in abs. syn. list                  */
#define R_NUTREL         13813      /* Wrong TRANSFER SYNTAX object id.    */
#define R_WTRSNM         13913      /* Wrong number of TRANSFER SYNTAXES   */
                                    /*  in trf. syn. list                  */

#define R_TRFIDX         14013      /* Wrong TRANSFER SYNTAX index         */
                                    /*  in pres. cxt. list                 */
#define R_MTPANU         14113      /* Wrong number of TRANSFER SYNTAXES   */
                                    /*  per abstract syntax                */
                                    /*  in pres. cxt. list                 */

#define R_PCXTNM         14213      /* Too much ABSTRACT SYNTAXES          */
                                    /*  in pres. cxt. list                 */

#define R_DPCXTN         14313      /* Too much ABSTRACT SYNTAXES          */
                                    /*  in abs. inc. list                  */
#define R_DNABSY         14413      /* Different number of ABSTRACT SYN.   */
                                    /*  in abs. syn. list,                 */
                                    /*     prs. cxt. list and              */
                                    /*     abs. inc. list                  */

/* *********** open req/rsp errors *************************************** */
#define R_W84_88         14513      /* Wrong MODE SELECTOR                 */

#define R_APLCXT         14613      /* Wrong index for the APL. CONTEXT    */

#define R_NOACSE         14742      /* ACSE abstract syntax required       */
#define R_ABSIDX         14813      /* Wrong ABSTRACT SYNTAX list          */
#define R_DFAABS         14913      /* Wrong DEFAULT ABSTRACT syntax       */
#define R_DFATRF         15013      /* Wrong DEFAULT TRANSFER syntax       */

#define R_RESULT         15113      /* Wrong RESULT                        */
#define R_ABSYRS         15213      /* Wrong SYNTAX RESULT                 */
#define R_DFARES         15313      /* Wrong DEFAULT PRES. CONTEXT RESULT  */

/* *********** open ind/cnf errors *************************************** */
#define R_SYNPTR         15413      /* No pointer for SYNTAX RESULT        */
#define R_ABSPTR         15513      /* No pointer for ABSTRACT SYNTAXES    */
#define R_TRFPTR         15613      /* No pointer for TRANSFER SYNTAXES    */
#define R_PRSPTR         15713      /* No pointer for PRES. CONTEXT Ident. */

/* *********** close req/rsp errors ************************************** */
#define R_REASON         15811      /* Wrong REASON                        */
#define R_ABSYIX         15913      /* Wrong ABSTRACT SYNTAX index         */

/* ***************** External Interface (RTS) *********************** */
/* Range : 160,170,180 ,190,200 */
/* *********** underlying errors ***************************************** */
#define R_WREVNT         16052      /* Wrong or undefined event coming     */
                                    /*  from Application Manager           */
#define R_RETDAT         16152      /* Failure in retrieve user data       */
                                    /*  by the store module                */
#define R_APLINV         16252      /* No appl. entity invocation context  */
#define R_PRSINV         16352      /* No pres. entity invocation context  */

#define R_NOA1ST         16409      /* Not first application at this        */
                                    /*  transport system                   */

/* *********** user data errors ****************************************** */
#define R_USRDAT         16513      /* Too long USER DATA length           */
                                    /*  in req/rsp call                    */
#define R_DATREF         16613      /* No reference pointer for USER DATA  */
				    /*  in req/rsp call                    */
#define R_DATCMP         16713      /* Length of USER DATA buffer too small*/
                                    /*  in ind/cnf call                    */

/* *********** attach errors ********************************************* */
#define R_WROFFR         16813      /* Wrong OFFER parameter or open req   */
                                    /*  after attach with OFFER = RESPONLY */




/* *********** open req/rsp errors *************************************** */
#define R_DIALOG         16913      /* Wrong DIALOG MODE                   */
#define R_INITIA         17013      /* Wrong TURN OWNER                    */
#define R_WINDOW         17113      /* Wrong WINDOW size                   */
#define R_CHECKP         17213      /* Wrong CHECKPOINT size               */
#define R_CHKWIN         17313      /* Wrong product CHECKPOINT*WINDOW size*/
#define R_84PROT         17413      /* wrong APL. PROTOCOL in 84-mode      */

#define R_ABSINC         17513      /* Wrong abstract syntax including     */
                                    /*  RTSE during attach or open req     */




/* *********** transfer errors ******************************************* */
#define R_WTIME          17613      /* Wrong WAIT TIME                     */

#define R_FILREF         17713      /* No FILE reference                   */
#define R_FILLEN         17813      /* No or wrong length of FILE name     */

/* *********** set recovery errors *************************************** */

#define R_RECSYL         17913      /* Recovery SYSTEM IDENTIFIER length   */
                                    /*  out of range                       */
#define R_RECSYI         18013      /* Wrong recovery SYSTEM IDENTIFIER    */
                                    /*  (null-pointer or wrong characters) */
#define R_RECMOD         18113      /* Wrong recovery MODE                 */

/* *********** inform errors ********************************************* */
#define R_INFTYP         18213      /* Wrong INFORMATION TYPE              */
#define R_INFARE         18313      /* No pointer for INFORMATION AREA     */

#define R_INFAPA         18413      /* No pointer for APPLICATION id. array*/
#define R_INFASA         18513      /* No pointer for ASSOCIATION id. array*/
#define R_INFACA         18613      /* No pointer for CORRESPONDING        */
                                    /*                APPLICATION id. array*/
#define R_INFADD         18713      /* No pointer for presentation ADDRESS */
#define R_INFTTL         18813      /* No pointer for APL. entity TITLE    */
#define R_INFAPC         18913      /* No pointer for APL. CONTEXT array   */
#define R_INFPRC         19013      /* No pointer for PRES. CONTEXT array  */
#define R_INFAPD         19113      /* No pointer for TRANSFER DATA (apdu) */
#define R_INFSYR         19213      /* No pointer for SYNTAX RESULT        */
#define R_INFASY         19313      /* No pointer for ABSTRACT SYNTAXES    */
#define R_INFTSY         19413      /* No pointer for TRANSFER SYNTAXES    */
#define R_INFPCI         19513      /* No pointer for PRES. CONTEXT IDENT. */

/* *********** status errors ********************************************* */
#define R_STINFO         19613      /* No pointer for INFORMATION array    */

#define R_STOTTL         19713      /* No pointer for OWN APL. entity TITLE*/
#define R_STPTTL         19813      /* No pointer for PARTNER APL. e. TITLE*/

#define R_STOADD         19913      /* No pointer for OWN pres. ADDRESS    */
#define R_STPADD         20013      /* No pointer for PARTNER pres. ADDRESS*/
#define R_XNOECB         20113      /* Ecb corresponding to Acb not found  */

/* ****************** External interface (ROS) ********************** */
/* Range : 210,220 */

#define  R_HNOABS      21041         /* No Abstarct Syntax present    */
#define  R_HWPMDE      21141         /* Wrong Mode value              */ 
#define  R_HWCONF      21241         /* Wrong Configuration value     */ 
#define  R_HCXTTS      21341         /* Exceeds Max. Transfer Syn.    */ 
#define  R_HACLAS      21441         /* Wrong Association Class       */
#define  R_HMTYPE      21541         /* Wrong memory type             */
#define  R_HFRCNT      21641         /* Fragment in Userdata exceeds limit */
#define  R_HWABSI      21741         /* Userdata Abs id wrong         */

#define  R_HOCLAS      21841         /* Wrong Operation Class         */
#define  R_HBUFER      21941         /* Buffer with  no refer. & len  */
#define  R_HNOSUP      22041         /* Not supported in the Current Ver */
#define  R_HRBUSY      22101         /* System  Busy                  */
#define  R_LOGERR      22241         /* Logging parameter error       */
#define  R_HRSTER      22341         /* Result Parameter error        */
#define  R_HRSNER      22441         /* Reason Parameter error        */
#define  R_HRRSER      22541         /* User Reject reason Param error*/
#define  R_HUDTLE      22641         /* User data length error        */
#define  R_HOTYPE      22741         /* r_otype error by deiver       */
#define  R_HWAETL      22841         /* wrong AE title                */
#define  R_HMOREE      22941         /* wrong r_more (TRUE)           */
#define  R_HWCONT      23541         /* wrong r_cont (TRUE)           */
#define  R_HWOPVL      23641         /* wrong Operation value         */
/* ****************** Upper Control (RTS) *************************** */
/* ****************** (Appl. Manager)     ******************************** */
/* Range : 230  'U'*/

#define R_ACUNEV         23052      /* Unknown incoming event              */
#define R_ACSTOR         23152      /* Store error                         */

/* ******************* Upper Control (ROS) ************************** */
/* Range : 240  'U'*/

#define  R_UBUFER      24013          /* Buffer not found             */
#define  R_USTERR      24152          /* Store error                  */

/* ****************** Association Control ( ACSE ) ******************* */
/* Range : 250 'A"*/

#define R_ALSQFL         25052      /* Local sequence flow error ACPM      */
#define R_AABSID         25152      /* Wrong abstract syntax id.           */
#define R_AUNKEV         25252      /* Unknown event in ACPM               */
#define R_AMODER         25352      /* Wrong mode (84, 88, private)        */
#define R_AAPLID         25452      /* Application id. is out of range     */
#define R_AAPLCX         25552      /* No application context              */
#define R_ASTERR         25652      /* Error in a store function call      */

/* ********************** Lower Control ****************************** */
/* Range : 260 */

/* ******************* Reliable Transfer ( RTSE ) ******************* */
/* Range : 270  'X'*/


#define R_FNIMPL         27052      /* Function not yet implemented        */
#define R_FINTER         27152      /* RTS panic: internal error           */
#define R_FLSQFL         27252      /* Local sequence flow error           */
#define R_FCLEAR         27352      /* Error during clearNodeRtpm          */
#define R_FUTIER         27452      /* Error from UTILITY module           */
#define R_FSTERR         27552      /* Error from STORE   module           */
#define R_FTPERR         27652      /* Error from TPDU    module           */
#define R_FENDER         27752      /* En- or decoding error               */

/* ******************* Remote Operations ( ROSE ) ******************** */
/* Range : 280 */

#define  R_OWSQFL      28022          /* Sequence flow error in roropm */
#define  R_ORELER      28152          /* Release Error                 */
#define  R_ONEVNT      28222          /* illegal in Event              */
#define  R_OROPME      28352          /* Internal Error ROPM           */
#define  R_OSTERR      28452          /* Store error                   */

/* ************************ Presentation ***************************** */
/* Range : 290,300  */

#define R_PLSQFL         29052      /* Local sequence flow error           */
#define R_PUNKEV         29152      /* Unknown event                       */
#define R_PFUNCU         29252      /* Wrong functional unit               */
#define R_PDFARS         29352      /* Wrong default context result        */
#define R_PUDATE         29452      /* PS-user data expected               */
#define R_PUDATN         29552      /* PS-user data not expected           */
#define R_POBJID         29652      /* Wrong object id. to getAbsId        */
#define R_PENTCX         29752      /* No pres. entity invoc. context      */
#define R_PANYNU         29852      /* Wrong ANY number to E/D Control     */
#define R_PCLEAN         29952      /* Error(s) during cleanUp             */
#define R_PCLPDV         30052      /* Error(s) during clearPdvTree        */
#define R_PSTERR         30152      /* Error in a STORE function call      */
#define R_PNIMPL         30252      /* Not yet implemented                 */
#define R_PDEPTS         30352      /* Decode Error  in PTS vp 24/04/93    */

/* ************************* Session ********************************** */
/* Range : 310 */

#define R_SUNKEV      31052         /* Unknown event                      */
#define R_SLSQFL      31152         /* Local sequence flow error          */ 
#define R_SINFER      31252         /* Internal error in inform fn.       */ 
#define R_SSTOER      31352         /* Store error in Session             */
#define R_SINTER      31452         /* Internal error in Spm/Product      */

/* ************************* Transport ******************************* */
/* Range : 320 */

#define  R_TUNKEV     32052
#define  R_TLSQFL     32152
#define  R_TSTERR     32252

#endif	/* _ROERVAL_H */
