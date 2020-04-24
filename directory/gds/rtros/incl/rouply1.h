/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rouply1.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:22  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:37:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:29  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:09  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:50  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:45  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:59:05  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:57  keutel
 * 	creation
 * 	[1994/03/21  13:28:29  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROUPLY1_H
#define _ROUPLY1_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rouply1_rcsid[] = { "@(#) $RCSfile: rouply1.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:22 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1992                     */
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
/* NAME         : rouply1.h                                           */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-02-11                                            */
/*                                                                    */
/* COMPONENT    : Structures and defines common to                    */
/*                - session,                                          */
/*                - presentation and                                  */
/*                - all application protocol machines                 */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : PM service interface structures                     */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rouply1.h	1.2 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-02-11|  init                          |hm  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/* ****************************************************************** */
/* ******************                                  ************** */
/* ****************** Connect parameter substructures  ************** */
/* ******************                                  ************** */
/* ****************************************************************** */

/* ****************************************************************** */
/* ****************** Substructures, all PM's *********************** */
/* ******************                         *********************** */

/* ****************** Connect request, all PM's ********************* */
/* ******************                           ********************* */

typedef struct rSro4a_stReqAll {
   unsigned long  int     hCalledAddress;
   signed   short int     sSessProtVers;
   signed   long  int     lModeSelector;
} rTro4a_stReqAll;

/* This substructure is common for all CONNECT request events:

   - hCalledAddress  : Called address (Local heap memory handle).
   - sSessProtVers   : Session protocol version             (see rouply0.h).
                       Not used by the PPM.
   - lModeSelector   : Required mode (normal or X.410-1984) (see rouply0.h).
                       Not used by the SPM.
*/

/* ****************** Connect indication, all PM's ****************** */
/* ******************                              ****************** */

typedef struct rSro4b_stIndAll {
   unsigned long  int     hCallingAddress;
   unsigned long  int     hCalledAddress;
   signed   short int     sSessProtVers;
   signed   long  int     lModeSelector;
} rTro4b_stIndAll;

/* This substructure is common for all CONNECT indication events:

   - hCallingAddress : Calling address (Local heap memory handle).
   - sSessProtVers   : Session protocol version             (see rouply0.h).
                       Not used by the PPM.
   - lModeSelector   : Required mode (normal or X.410-1984) (see rouply0.h).
                       Not used by the SPM.
*/

/* ****************** Connect response and confirm, all PM's ******** */
/* ******************                                        ******** */

typedef struct rSro4c_stRspCnfAll {
   unsigned long  int     hRespondAddress;
   signed   short int     sSessProtVers;
} rTro4c_stRspCnfAll;

/* This substructure is common for all CONNECT response and confirm
   events:

   - hRespondAddress : Responding address (Local heap memory handle).
   - sSessProtVers   : Session protocol version             (see rouply0.h).
                       Not used by the PPM.
*/


/* ****************************************************************** */
/* ****************** Substructure, SPM ***************************** */
/* ******************                   ***************************** */

/* ****************** Connect request, indication, ****************** */
/* ****************** response and confirm, SPM    ****************** */

typedef struct rSro5a_stSpm {
   unsigned long  int     ulRequirements;
   signed   long  int     lSyncPoint;
   unsigned long  int     ulToken;
   unsigned long  int     ulQuality;
   rTro50_stConnId        stConnId;
} rTro5a_stSpm;

/* This substructure is passed to/from the SPM during a S-CONNECT event:

   - ulRequirements  : Session requirements               (see rouply0.h).
   - lSyncPoint      : Initial synchronization point serial number.
   - ulToken         : Initial assignment of tokens       (see rouply0.h).
   - ulQuality       : Quality of service
                       (not supported in OSS V2.0, filled with NULL).
   - stConnId        : Session connection identifier      (see rouply0.h).
*/


/* ****************************************************************** */
/* ****************** Substructures, PPM **************************** */
/* ******************                    **************************** */

/* ****************** Connect request, PPM ************************** */
/* ******************                      ************************** */

typedef struct rSro6a_stReqPpm {
   rTro60_vAbstrSynId     vAbstrSynId;
   signed   short int     sDefAbstrSynId;
   signed   short int     sDefTransSynId;
   unsigned long  int     ulRequirements;
} rTro6a_stReqPpm;

/* This substructure is passed to the PPM during a P-CONNECT request event:

   - vAbstrSynId     : Proposed abstract syntaxes         (see rouply0.h).
   - sDefAbstrSynId  : Proposed default abstract syntax   (see rouply0.h).
   - sDefTransSynId  : Proposed default transfer syntax   (see rouply0.h).
   - ulRequirements  : Required presentation requirements (see rouply0.h).
*/

/* ****************** Connect indication, PPM *********************** */
/* ******************                         *********************** */

typedef struct rSro6b_stIndPpm {
   rTro60_vAbstrSynId     vAbstrSynId;
   rTro61_vAbstrSynResult vAbstrSynResult;
   rTro62_vTransSynId     vTransSynId;
   rTro63_vPresContId     vPresContId;
   signed   short int     sDefAbstrSynId;
   signed   short int     sDefTransSynId;
   unsigned long  int     ulRequirements;
} rTro6b_stIndPpm;

/* This substructure is passed from the PPM during a P-CONNECT indication
   event:

   - vAbstrSynId     : Proposed abstract syntaxes         (see rouply0.h).
   - vAbstrSynResult : Result of abstract syntax negotiation
                       made by the local PPM              (see rouply0.h).
   - vTransSynId     : Negotiated transfer syntaxes       (see rouply0.h).
   - vPresContId     : Negotiated presentation context identifiers
                                                          (see rouply0.h).
   - sDefAbstrSynId  : Proposed default abstract syntax   (see rouply0.h).
   - sDefTransSynId  : Proposed default transfer syntax   (see rouply0.h).
   - ulRequirements  : Proposed presentation requirements (see rouply0.h).
*/

/* ****************** Connect response, PPM ************************* */
/* ******************                       ************************* */

typedef struct rSro6c_stRspPpm {
   rTro61_vAbstrSynResult vAbstrSynResult;
   signed   short int     sDefContResult;
   unsigned long  int     ulRequirements;
} rTro6c_stRspPpm;

/* This substructure is passed to the PPM during a P-CONNECT response
   event:

   - vAbstrSynResult : Abstract syntax result             (see rouply0.h).
   - sDefContResult  : Default context result             (see rouply0.h).
   - ulRequirements  : Used presentation requirements     (see rouply0.h).
*/

/* ****************** Connect confirm, PPM ************************** */
/* ******************                      ************************** */

typedef struct rSro6d_stCnfPpm {
   rTro61_vAbstrSynResult vAbstrSynResult;
   rTro62_vTransSynId     vTransSynId;
   rTro63_vPresContId     vPresContId;
   signed   short int     sDefContResult;
   unsigned long  int     ulRequirements;
} rTro6d_stCnfPpm;

/* This substructure is passed from the PPM during a P-CONNECT confirm
   event:

   - vAbstrSynResult : Abstract syntax result             (see rouply0.h).
   - vTransSynId     : Negotiated transfer syntaxes       (see rouply0.h).
   - vPresContId     : Negotiated presentation context identifiers
                                                          (see rouply0.h).
   - sDefContResult  : Default context result             (see rouply0.h).
   - ulRequirements  : Used presentation requirements     (see rouply0.h).
*/


/* ****************************************************************** */
/* ****************** Substructure, ACPM **************************** */
/* ******************                    **************************** */

/* ****************** Associate request,             **************** */
/* ****************** response and confirm, ACPM     **************** */

typedef struct rSro7a_stAcpm {
   signed   short int     sApplContextId;
   unsigned long  int	  hAeTitle;
   rTro74_stInvoc         stInvoc;
} rTro7a_stAcpm;

/* This substructure is passed to/from the ACPM during an A-ASSOCIATE
   event:

   - sApplContextId  : Application context identifier (normal mode)
                       or "NoApplContext" (X.410-1984 mode)
                                                          (see rouply0.h).
   - hAeTitle        : Called (request),                      or
                       responding (response, confirm) AE title
                                                          (see rouply0.h).
   - stInvoc         : Called (request),                      or
                       responding (response, confirm) AP invocation-
                       identifier and AE invocation-identifier
                                                          (see rouply0.h).
*/


typedef struct rSro7b_stIndAcpm {
   signed   short int     sApplContextId;
   unsigned long  int	  hAeTitle;
   rTro74_stInvoc         stInvoc;
   unsigned long  int	  hCldAeTitle;
   rTro74_stInvoc         stCldInvoc;
   
} rTro7b_stIndAcpm;

/* This substructure is passed to/from the ACPM during an A-ASSOCIATE
   event:

   - sApplContextId  : Application context identifier (normal mode)
                       or "NoApplContext" (X.410-1984 mode)
                                                          (see rouply0.h).
   - hAeTitle        : Calling AE Title                       or
                                                          (see rouply0.h).
   - stInvoc         : Calling AE & AP invocation-identifier     
                                                          (see rouply0.h).
   - hCldAeTitle     : Called AE Title                       or
                                                          (see rouply0.h).
   - stCldInvoc      : Called AE & AP invocation-identifier     
                                                          (see rouply0.h).
*/


/* ****************************************************************** */
/* ******************                                       ********* */
/* ****************** Complete connect parameter structures ********* */
/* ****************** (SPM, PPM, ACPM)                      ********* */
/* ******************                                       ********* */
/* ****************************************************************** */

/*
   N o t e:
   For the connect parameter structures of SPM, PPM and ACPM the  s a m e
   p h y s i c a l  memory block is used without any data copy.
   That's why the beginning of the PPM struct must be the SPM struct, and
   the beginning of the ACPM struct must be the PPM struct!
   In the case of an indication or confirm event the  S P M  allocates a
   memory block of the size of the corresponding  A C P M  struct!
*/

/* ****************************************************************** */
/* ****************** Complete structures, SPM ********************** */
/* ******************                          ********************** */

/* ****************** Connect request, SPM ************************** */
/* ******************                      ************************** */

typedef struct rSro5e_stConnReqSpm {
   rTro4a_stReqAll        stAll;
   rTro5a_stSpm           stSpm;
} rTro5e_stConnReqSpm;

/* This structure is passed to the SPM during a S-CONNECT request event:

   - stAll           : Substructure: common               (see above).
   - stSpm           : Substructure: session              (see above).
*/

/* ****************** Connect indication, SPM *********************** */
/* ******************                         *********************** */

typedef struct rSro5f_stConnIndSpm {
   rTro4b_stIndAll        stAll;
   rTro5a_stSpm           stSpm;
} rTro5f_stConnIndSpm;

/* This structure is passed from the SPM during a S-CONNECT indication
   event:

   - stAll           : Substructure: common               (see above).
   - stSpm           : Substructure: session              (see above).
*/

/* ****************** Connect response and confirm, SPM ************* */
/* ******************                                   ************* */

typedef struct rSro5g_stConnRspCnfSpm {
   rTro4c_stRspCnfAll     stAll;
   rTro5a_stSpm           stSpm;
} rTro5g_stConnRspCnfSpm;

/* This structure is passed to/from the SPM during a S-CONNECT response
   and S-CONNECT confirm event:

   - stAll           : Substructure: common               (see above).
   - stSpm           : Substructure: session              (see above).
*/


/* ****************************************************************** */
/* ****************** Complete structures, PPM ********************** */
/* ******************                          ********************** */

/* ****************** Connect request, PPM ************************** */
/* ******************                      ************************** */

typedef struct rSro6e_stConnReqPpm {
   rTro4a_stReqAll        stAll;
   rTro5a_stSpm           stSpm;
   rTro6a_stReqPpm        stPpm;
} rTro6e_stConnReqPpm;

/* This structure is passed to the PPM during a P-CONNECT request event:

   - stAll           : Substructure: common               (see above).
   - stSpm           : Substructure: session              (see above).
   - stPpm           : Substructure: presentation         (see above).
*/

/* ****************** Connect indication, PPM *********************** */
/* ******************                         *********************** */

typedef struct rSro6f_stConnIndPpm {
   rTro4b_stIndAll        stAll;
   rTro5a_stSpm           stSpm;
   rTro6b_stIndPpm        stPpm;
} rTro6f_stConnIndPpm;

/* This structure is passed from the PPM during a P-CONNECT indication
   event:

   - stAll           : Substructure: common               (see above).
   - stSpm           : Substructure: session              (see above).
   - stPpm           : Substructure: presentation         (see above).
*/

/* ****************** Connect response, PPM ************************* */
/* ******************                       ************************* */

typedef struct rSro6g_stConnRspPpm {
   rTro4c_stRspCnfAll     stAll;
   rTro5a_stSpm           stSpm;
   rTro6c_stRspPpm        stPpm;
} rTro6g_stConnRspPpm;

/* This structure is passed to the PPM during a P-CONNECT response event:

   - stAll           : Substructure: common               (see above).
   - stSpm           : Substructure: session              (see above).
   - stPpm           : Substructure: presentation         (see above).
*/

/* ****************** Connect confirm, PPM ************************** */
/* ******************                      ************************** */

typedef struct rSro6h_stConnCnfPpm {
   rTro4c_stRspCnfAll     stAll;
   rTro5a_stSpm           stSpm;
   rTro6d_stCnfPpm        stPpm;
} rTro6h_stConnCnfPpm;

/* This structure is passed from the PPM during a P-CONNECT confirm event:

   - stAll           : Substructure: common               (see above).
   - stSpm           : Substructure: session              (see above).
   - stPpm           : Substructure: presentation         (see above).
*/


/* ****************************************************************** */
/* ****************** Complete structures, ACPM ********************* */
/* ******************                           ********************* */

/* ****************** Association request, ACPM ********************* */
/* ******************                           ********************* */

typedef struct rSro7e_stAssocReqAcpm {
   rTro4a_stReqAll        stAll;
   rTro5a_stSpm           stSpm;
   rTro6a_stReqPpm        stPpm;
   rTro7a_stAcpm          stAcpm;
} rTro7e_stAssocReqAcpm;

/* This structure is passed to the ACPM during an A-ASSOCIATE request event:

   - stAll           : Substructure: common               (see above).
   - stSpm           : Substructure: session              (see above).
   - stPpm           : Substructure: presentation         (see above).
   - stAcpm          : Substructure: ACSE                 (see above).
*/

/* ****************** Association indication, ACPM ****************** */
/* ******************                              ****************** */

typedef struct rSro7f_stAssocIndAcpm {
   rTro4b_stIndAll        stAll;
   rTro5a_stSpm           stSpm;
   rTro6b_stIndPpm        stPpm;
   rTro7b_stIndAcpm       stAcpm;
} rTro7f_stAssocIndAcpm;

/* This structure is passed from the ACPM during an A-ASSOCIATE indication
   event:

   - stAll           : Substructure: common               (see above).
   - stSpm           : Substructure: session              (see above).
   - stPpm           : Substructure: presentation         (see above).
   - stAcpm          : Substructure: ACSE                 (see above).
*/

/* ****************** Association response, ACPM ******************** */
/* ******************                            ******************** */

typedef struct rSro7g_stAssocRspAcpm {
   rTro4c_stRspCnfAll     stAll;
   rTro5a_stSpm           stSpm;
   rTro6c_stRspPpm        stPpm;
   rTro7a_stAcpm          stAcpm;
} rTro7g_stAssocRspAcpm;

/* This structure is passed to the ACPM during an A-ASSOCIATE response
   event:

   - stAll           : Substructure: common               (see above).
   - stSpm           : Substructure: session              (see above).
   - stPpm           : Substructure: presentation         (see above).
   - stAcpm          : Substructure: ACSE                 (see above).
*/

/* ****************** Association confirm, ACPM ********************* */
/* ******************                           ********************* */

typedef struct rSro7h_stAssocCnfAcpm {
   rTro4c_stRspCnfAll     stAll;
   rTro5a_stSpm           stSpm;
   rTro6d_stCnfPpm        stPpm;
   rTro7a_stAcpm          stAcpm;
} rTro7h_stAssocCnfAcpm;

/* This structure is passed from the ACPM during an A-ASSOCIATE confirm
   event:

   - stAll           : Substructure: common               (see above).
   - stSpm           : Substructure: session              (see above).
   - stPpm           : Substructure: presentation         (see above).
   - stAcpm          : Substructure: ACSE                 (see above).
*/


/* ****************************************************************** */
/* ******************                                     *********** */
/* ****************** Activity Resume parameter structure *********** */
/* ****************** (RTPM, SPM)                         *********** */
/* ******************                                     *********** */
/* ****************************************************************** */

typedef struct rSro5i_stActResume {
   rTro51_stActivId       stActivId;
   rTro51_stActivId       stOldActivId;
   signed   long  int     lSyncPoint;
   rTro50_stConnId        stOldConnId;
} rTro5i_stActResume;

/* This structure is passed to/from the SPM during a SACT_RESUME request
   and SACT_RESUME indication event (from/to the RTPM via the PPM):

   - stActivId       : Activity identifier                (see rouply0.h).
   - stOldActivId    : Old activity identifier            (see rouply0.h).
   - lSyncPoint      : Synchronization point serial number.
   - stOldConnId     : Old session connection identifier  (see rouply0.h).
*/

#endif	/* _ROUPLY1_H */
