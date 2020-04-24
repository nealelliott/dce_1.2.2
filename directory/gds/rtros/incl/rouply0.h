/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rouply0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:20  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:37:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:26  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:59  keutel
 * 	OT 11620
 * 	[1994/08/19  13:49:09  keutel]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:07  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:49  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:43  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:59:00  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:54  keutel
 * 	creation
 * 	[1994/03/21  13:28:24  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROUPLY0_H
#define _ROUPLY0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rouply0_rcsid[] = { "@(#) $RCSfile: rouply0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:20 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1991                     */
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
/* NAME         : rouply0.h                                           */
/*                                                                    */
/* AUTHOR       : Johann Mueller, Harald Miersch                      */
/* DATE         : 91-12-13                                            */
/*                                                                    */
/* COMPONENT    : Structures and defines common to                    */
/*                - session,                                          */
/*                - presentation and                                  */
/*                - all application protocol machines                 */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : Common generation constants, types and values       */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rouply0.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|91-12-13| adaptation of ROS V2.5A00 K05  | hm |         */
/*           |92-06-16| Destinguished Names of ApTitle |    |         */
/*           |        |  and AeQualifier reorganized   | mr |         */
/* 03.00  K00|93-04-09| PdvHeader chg  for SeqOfExtern | vp |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/* ****************************************************************** */
/* ******************                                 *************** */
/* ****************** Product configuration constants *************** */
/* ******************                                 *************** */
/* ****************************************************************** */

/* ****************** Configuration constants related to rts.h ****** */
/* ******************                                          ****** */

#define RCro00_MaxObjIdComp      16   /* max. number of object identifier */
                                      /* components                       */
#define RCro01_MaxAbstrSyn        8   /* max. number of abstract syntaxes */
                                      /* (per application)                */
#define RCro02_MaxTransSyn        8   /* max. number of transfer syntaxes */
                                      /* (per application)                */
#define RCro03_MaxTransPerAbstr   5   /* max. number of transfer syntaxes */
                                      /* (per abstract syntax)            */
#define RCro04_MaxPresCont        5   /* max. number of presentation con- */
                                      /* texts (per association)          */

/* ****************** Other ASN.1 related      ********************** */
/* ****************** configuration constants  ********************** */

#define RCro0a_MaxPdvList         5   /* max. number of PDV-lists         */
#define RCro0b_MaxExternal        5   /* max. number of EXTERNALs         */
                                      /* (per embedding PDU)              */

#define RCro0c_MaxLenObjDescExt  16   /* max. length of OBJECT DESCRIPTOR */
                                      /* within EXTERNAL (rest is ignored)*/


/* ****************************************************************** */
/* ******************                                    ************ */
/* ****************** Definitions: ASN.1 type --> C type ************ */
/* ******************                                    ************ */
/* ****************************************************************** */

/* ****************** Object identifier value *********************** */
/* ******************                         *********************** */

typedef unsigned long int rTro00_vObjectId [RCro00_MaxObjIdComp + 1];

/* This type implements an object identifier value. The first element
   contains the number of components.
   If the first element contains NULL the object identifier value is
   "Not defined".
*/

typedef unsigned long int FAR * rTro01_pfvObjectId;

/* This type implements a pointer to the first element of the ObjId 
   type specified in rTro00_vObjectId.
*/

/* ****************** Store description ***************************** */
/* ******************                   ***************************** */

typedef struct rSro02_stStore {
   unsigned long  int    hsStore;
   unsigned long  int    ulOffset;
   unsigned long  int    ulNumb;
} rTro02_stStore;

/* Used in PDV nodes (PM specific PDV nodes and User-data PDV nodes).

   Describes a complete "Store" or a part within a Store ("Store Part")
   which represents an ASN.1 type.

   Store : Virtual data storage, intended for a maybe very long byte string
     with an a priori unknown size. Manipulated by using a set of Utility
     store functions (e.g. byte read, block read). The physical data location
     (memory or disk) is transparent for the user of the functions.
   Store Part : Part of a Store, identified by offset and length.

   In the case of a User-data PDV node, the ASN.1 type of the contained data
   is defined by a PDV header parameter (ANY, OCTET STRING or BIT STRING).

   Otherwise, the ASN.1 type of the contained data is defined by the concrete
   use of the Store description in the PDV node:
   - OCTET STRING
   - BIT STRING
   - OBJECT DESCRIPTOR.
   The Store data are the contents octets of the ASN.1 encoding.
   During encoding: Tag and length octets are encoded.
   During decoding: Tag and length octets are removed, the byte string
   is packed if a constructed type was transferred.

   - hsStore        : Store handle returned by the store utility
                      function "newstore".
   - ulOffset       : Store      : NULL.
                      Store Part : Offset where the string starts within
                                   the store (scale: "byte"). NULL if
                                   Store Part at the beginning of the Store.
   - ulNumb         : Length within the Store.
                      ANY, OCTET STRING, OBJECT DESCRIPTOR : scale "byte".
                      BIT STRING                           : scale "bit".
                      (Redundant information if not Store Part and
                      if not BIT STRING because the total Store length
                      in bytes is the return value of a Utility function.)
*/


/* ****************** Encoding choice value ************************* */
/* ****************** (PDV header)          ************************* */

/* Note:
   Do not change the values! Must be identical with the ASN.1
   CHOICE tags in the EXTERNAL and PDV-list definitions!
*/

#define RCro0m_ChoiceAny          0   /* choice ANY               */
#define RCro0n_ChoiceOctetString  1   /* choice OCTET STRING      */
#define RCro0o_ChoiceBitString    2   /* choice BIT STRING        */


/* ****************** PDV header ************************************ */
/* ******************            ************************************ */

typedef struct rSro03_stPdvHeader {
                                     /* To handle SEQ. OF EXTERNALs   */
   signed   long  int    lFraCount;     
   unsigned long  int    vPdvHandle[RCro0b_MaxExternal-1];

   signed   short int    sPmNumb;
   signed   short int    sAbstrSynId;
   signed   long  int    lEncodingChoice;
   signed   long  int    lType;
} rTro03_stPdvHeader;

/* Common header structure of a PDV node ("PDV header").
   Must be located always at the beginning of the PDV node memory block.
   Set by the corresponding PM (send case) or during decoding.

   (PDV node: Local representation of a presentation data value.)

   - sPmNumb        : Protocol machine number (see rodpat0.h).
                      A pseudo PM number indicating RTSE or ROSE user is
                      used if the PDV node is passed to/from the External
                      Interface ("User-data PDV node", see below).
   - sAbstrSynId    : Abstract syntax identifier (describes the named abstract
                      syntax the node belongs to).
   - lEncodingChoice: Encoding choice value of the PDV node:
                      - ANY
                      - OCTET STRING
                      - BIT STRING.
                      During encoding, the choice is only done in a
                      PDV-list or EXTERNAL. Otherwise only the value ANY
                      is allowed.
                      During decoding, OCTET STRING and BIT STRING are only
                      possible in a PDV-list or EXTERNAL. Otherwise only the
                      value ANY may appear.
                      Values see below.
                      Note: In the current implementation, only the value
                      ANY is allowed (for encoding and decoding, otherwise
                      abort)!
   - lType          : PM specific defined value.
                      In general: PDU type within the abstract syntax (local
                      representation of an ASN.1 choice tag).

*/

/* ****************** User-data PDV node **************************** */
/* ******************                    **************************** */

typedef struct rSro04_stUserData {
   rTro03_stPdvHeader    stPdvHeader;
   rTro02_stStore        stStore;
} rTro04_stUserData;

/* Used to pass user-data of the user of the product.

   (User-data: PPDU part, encoded in the used transfer syntax.)

   - stPdvHeader    : Common PDV node header structure (see definition above).
   - stStore        : Store description (see definition above).
*/


/* ****************** Mapping: ASN.1 types --> C types *************** */
/* ******************                                  *************** */

/*
  |-------------------|---------------------------------------------|
  | ASN.1 Type        | C Type                                      |
  |-------------------|---------------------------------------------|
  | NULL              | -                                           |
  |-------------------|---------------------------------------------|
  | BOOLEAN           | signed long int                             |
  |-------------------|---------------------------------------------|
  | INTEGER           | signed long int                             |
  |-------------------|---------------------------------------------|
  | BIT STRING        | (unsigned long int, unsigned long int) or   |
  |                   | (rTro02_stStore)                            |
  |-------------------|---------------------------------------------|
  | OCTET STRING      | (unsigned long int, char [MAX]) or          |
  |                   | (rTro02_stStore)                            |
  |-------------------|---------------------------------------------|
  | OBJECT IDENTIFIER | rTro00_vObjectId                            |
  |-------------------|---------------------------------------------|
  | OBJECT DESCRIPTOR | (unsigned long int, char [MAX]) or          |
  |                   | (rTro02_stStore)                            |
  |-------------------|---------------------------------------------|
  | SEQUENCE          | struct                                      |
  |-------------------|---------------------------------------------|
  | SET               | struct                                      |
  |-------------------|---------------------------------------------|
  | SEQUENCE OF       | unsigned long int, TYPE [MAX]               |
  |-------------------|---------------------------------------------|
  | SET OF            | unsigned long int, TYPE [MAX]               |
  |-------------------|---------------------------------------------|
  | CHOICE            | signed long int, union                      |
  |-------------------|---------------------------------------------|
  | ANY               | NEXT                                        |
  |-------------------|---------------------------------------------|
  | ANY DEFINED BY    | NEXT, union                                 |
  |-------------------|---------------------------------------------|
  | EXTERNAL          | NEXT                                        |
  |-------------------|---------------------------------------------|
  | PDV-list          | NEXT                                        |
  |-------------------|---------------------------------------------|
  | COMPONENTS OF     | CALL                                        |
  |-------------------|---------------------------------------------|

  MAX : Length defined on compile time
  TYPE: One of the types listed in table above
  NEXT: Memory handle to the next PDV node
  CALL: Direct call of an encoding or decoding function.

  Local representation of OPTIONAL (within SEQUENCE and SET):
  - OBJECT IDENTIFIER: First element of the array is NULL if absent.
  - ANY, ANY DEFINED BY, EXTERNAL, PDV-list: Memory handle is NULL if absent.
  - Otherwise: Additional boolean variable (signed long int), false if absent.
*/


/* ****************************************************************** */
/* ******************                                     *********** */
/* ****************** Session service related definitions *********** */
/* ******************                                     *********** */
/* ****************************************************************** */

/* Note:
   If the following session parameter values are the same as used in the
   OSS interface (file oss.h) a direct exchange of the values without
   any mapping in the Lower Interface is possible.
   If another session implementation shall be used or if the OSS inter-
   face changes, there must be a mapping in the Lower Interface.
*/

/* ****************** Addressing related       ********************** */
/* ****************** constant definitions.    ********************** */

#define RCro10_PresentationAddress   0
#define RCro11_Pselector             1
#define RCro12_SessionAddress        2
#define RCro13_Sselector             3
#define RCro14_TransportAddress      4
#define RCro15_Tselector             5
#define RCro16_LowerAddress          6

#define RCro1a_MaxLenPSelector   16   /* max. length of presentation      */
                                      /*                selector.         */
#define RCro1b_MaxLenSSelector   16   /* max. length of Session selector  */
#define RCro1c_MaxLenTSelector   10   /* max. length of Transport selector*/
#define RCro1d_MaxLenTSAP       244   /* max. length of Transport address */
#define RCro1e_MaxLenPSAP       292   /* max. length of presentation address */

/* ****************** Session protocol version ********************** */
/* ******************                          ********************** */

/* Note:
   Used by various PMs.
*/

#define RCro50_ProtVers1          1    /* session protocol version 1  */
#define RCro51_ProtVers2          2    /* session protocol version 2  */

/* ****************** Session requirements ************************** */
/* ******************                      ************************** */

/* Note:
   Do not change! Used as presentation protocol parameter values too.
*/

#define RCro52_HalfDuplex    0x0001
#define RCro53_FullDuplex    0x0002
#define RCro54_MinorSync     0x0008
#define RCro55_Activity      0x0040
#define RCro56_Exceptions    0x0200

/* ****************** Session token values ************************** */
/* ******************                      ************************** */

/* If the corresponding bit is set, this means:
   S-CONNECT request         : token to the acceptor
   S-CONNECT indication      : token at the acceptor
   S-TOKEN-PLEASE request    : token requested
   S-TOKEN-PLEASE indication : token requested
*/

#define RCro57_DataToken          1    /* data token              (bit 0) */
#define RCro58_MinorToken         4    /* synchronize-minor token (bit 2) */
#define RCro59_ActivityToken     16    /* major/activity token    (bit 4) */
#define RCro5a_ReleaseToken      64    /* release token           (bit 6) */

/* If the corresponding bit is set, this means:
   S-CONNECT request         : token to choose by the acceptor
   S-CONNECT indication      : token to choose by the acceptor
   S-CONNECT response        : token at the acceptor
   S-CONNECT confirm         : token at the acceptor
*/

#define RCro5b_DataChoice         2    /* data token              (bit 1) */
#define RCro5c_MinorChoice        8    /* synchronize-minor token (bit 3) */
#define RCro5d_ActivityChoice    32    /* major/activity token    (bit 5) */
#define RCro5e_ReleaseChoice    128    /* release token           (bit 7) */

/* ****************** Sync minor type ******************************* */
/* ******************                 ******************************* */

#define RCro5f_ExplicitConfirm    0    /* explicit confirm                */
#define RCro5g_OptionalConfirm    1    /* optional confirm                */


/* ****************** Sync point values ***************************** */
/* ******************                   ***************************** */

#define RCro5h_NoInitialSpsn     -1L   /* no value for Initial Sync Point */
                                       /* Serial Number (ISPSN) specified */

/* ****************** Session connection user identifier ************ */
/* ******************                                    ************ */

#define RCro5m_MaxRef            64    /* max. length of user and common  */
                                       /* reference  according to service */
                                       /* and protocol standard           */
#define RCro5n_MaxAddRef          4    /* max. length of additional refe- */
                                       /* rence according to service and  */
                                       /* protocol standard               */

typedef struct rSro50_stConnId {
   unsigned long  int     ulCallingSSuserRef;
   char                   vCallingSSuserRef [RCro5m_MaxRef];
   unsigned long  int     ulCalledSSuserRef;
   char                   vCalledSSuserRef [RCro5m_MaxRef];
   unsigned long  int     ulCommonReference;
   char                   vCommonReference [RCro5m_MaxRef];
   unsigned long  int     ulAdditionalReference;
   char                   vAdditionalReference [RCro5n_MaxAddRef];
} rTro50_stConnId;

/* ****************** Session activity identifier ******************* */
/* ******************                             ******************* */

#define RCro5o_MaxActivId         6    /* max. length of activity identi- */
                                       /* fier according to service and   */
                                       /* protocol standard               */

typedef struct rSro51_stActivId {
   unsigned long  int     ulActivId;
   char                   vActivId [RCro5o_MaxActivId];
} rTro51_stActivId;

/* ****************** Session reason values ************************* */
/* ******************                       ************************* */

/* Used in the primitives:
   S-ACTIVITY-INTERRUPT req/ind/rsp/cnf
   S-ACTIVITY-DISCARD   req/ind/rsp/cnf
   S-U-EXCEPTION req/ind
*/  

#define RCro5p_NonSpecificError   0    /* non-specific error              */
#define RCro5q_ReceiverOverflow   1    /* SS-user receiving ability       */
                                       /* jeopardized                     */
#define RCro5r_SequenceError      3    /* sequence error                  */
#define RCro5s_LocalUserError     5    /* local SS-user error             */
#define RCro5t_UnrecovProcError   6    /* unrecoverable procedural error  */
#define RCro5u_DemandDataToken  128    /* demand data token               */


/* Used in the primitive:
   S-P-EXCEPTION ind
*/  

#define RCro5v_NonSpecificError   0    /* non-specific error              */
#define RCro5w_ProtocolError      4    /* protocol error                  */

/* ****************************************************************** */
/* ******************                                          ****** */
/* ****************** Presentation service related definitions ****** */
/* ******************                                          ****** */
/* ****************************************************************** */

/* ****************** Presentation requirements ********************* */
/* ******************                           ********************* */

                                          /* Presentation funct. units:  */
#define RCro60_Kernel                 0   /*   - kernel                  */
#define RCro61_Context                1   /*   - context management      */
#define RCro62_Restore                2   /*   - context restoration     */

/* ****************** Abstract syntax definition list *************** */
/* ******************                                 *************** */

#define RCro6a_MinAbstrSynId          0   /* Do not change!              */
#define RCro6b_MaxAbstrSynId          (RCro01_MaxAbstrSyn - 1)

/* During the attach call for each abstract syntax a short identifier is
   defined by the products user. It is called the "abstract syntax identifier"
   or "AbsId" in short.
   AbsId's are numbers in the range defined above.
*/

typedef signed short int rTro60_vAbstrSynId [RCro04_MaxPresCont + 1];

/* Used in the P-CONNECT request and indication events.
   During establishment of an association a set of abstract syntaxes is
   negotiated. This set is passed to/from the PPM in an array defined
   above. The first element contains the number of abstract syntaxes
   followed by AbsId values. The number and sequence of AbsIds in the
   P-CONNECT indication event is the same as in the corresponding
   P-CONNECT request event.
   If the first element contains NULL no abstract syntaxes are negotiated.
*/

/* ****************** Abstract syntax result list ******************* */
/* ******************                             ******************* */

#define RCro6c_Acceptance             0
#define RCro6d_UserRejection          1
#define RCro6e_ReasonNotSpecified    -1
#define RCro6f_AbstrSynNotSupported  -2
#define RCro6g_TransSynNotSupported  -3
#define RCro6h_LocalDcsLimitExceeded -4

typedef signed short int rTro61_vAbstrSynResult [RCro04_MaxPresCont + 1];

/* Used in the P-CONNECT response and confirm events.
   The array contains the result of abstract syntax negotiation.
   The first value contains the number of abstract syntaxes which were
   proposed in the P-CONNECT request and indication events.
   The sequence of the result values is the same as the sequence of
   corresponding AbsId's in the P-CONNECT request and indication events.

   Note that the service standard not specifies the reasons for a provider
   rejection (result value ProviderReject). In our implementation all nega-
   tive signed values mean a provider reject.

   This array has to be evaluated before the evaluation of the abstract 
   syntax definition list, negotiated transfer syntax list or negotiated
   presentation context identifier list.

   If the first element contains NULL in the case of a P-CONNECT response
   event the abstract syntaxes which were proposed in the P-CONNECT indi-
   cation event are negotiated. (?)
*/

/* ****************** Negotiated transfer syntax list *************** */
/* ******************                                 *************** */

typedef signed short int rTro62_vTransSynId [RCro04_MaxPresCont + 1];

/* Used in the P-CONNECT indication and confirm events.
   Necessary because the user of the product encodes and decodes its
   user-data itself in the current implementation.
   The array contains the short identifiers of negotiated transfer syntaxes
   (TransId's).
   The first value contains the number of abstract syntaxes which were
   proposed in the P-CONNECT request and indication events.
   The sequence of the TransId's is the same as the sequence of cor-
   responding AbsId's in the P-CONNECT request and indication events.
*/

/* ****************** Negotiated presentation context identifier **** */
/* ****************** list                                       **** */

typedef signed long int rTro63_vPresContId [RCro04_MaxPresCont + 1];

/* Used in the P-CONNECT indication and confirm events.
   Necessary because the user of the product encodes and decodes its
   user-data itself in the current implementation (for RT-TRANSFER the
   user-data may be encoded as EXTERNAL).
   The array contains the negotiated presentation context identifiers.
   The first value contains the number of abstract syntaxes which were
   proposed in the P-CONNECT request and indication events.
   The sequence of the identifiers is the same as the the sequence of
   corresponding AbsId's in the P-CONNECT request and indication events.
*/

typedef signed short int   FAR *  rTro64_pfvAbstrSynId     ;
typedef signed short int   FAR *  rTro65_pfvAbstrSynResult ;
typedef signed short int   FAR *  rTro66_pfvTransSynId     ;
typedef signed long  int   FAR *  rTro67_pfvPresContId     ;

/* ****************** Default context representation **************** */
/* ******************                                **************** */

#define RCro6i_NoDefContext           (RCro6b_MaxAbstrSynId + 1)

#define RCro6j_MinTransSynId          0   /* Do not change!              */
#define RCro6k_MaxTransSynId          (RCro02_MaxTransSyn - 1)

/* During establishment of an association a default abstract syntax and
   a default transfer syntax may be negotiated. An AbsId and an TransId
   are passed to/from the PPM. If no default context shall be negotiated
   the AbsId value RCro6i_NoDefContext is used (the TransId is undefined
   in this case).
*/

/* ****************** Default context result ************************ */
/* ******************                        ************************ */

/* The values defined above for the abstract syntax result list are used.
   They are passed in the P-CONNECT response and confirm PM event.
   If no default context was proposed the value RCro6i_NoDefContext
   is used.
*/

/* ****************** Presentation provider abort reasons        **** */
/* ****************** (generated by encoding/decoding functions) **** */

#define RCro6l_UnrecognizedPpduParam  4  /* unrecognized PPDU parameter  */
#define RCro6m_UnexpectedPpduParam    5  /* unexpected PPDU parameter    */
#define RCro6n_InvalidPpduParam       6  /* invalid PPDU parameter value */
                                         /* use will be illustrated later*/

/* *********************************************************************** */
/* ******************                                ********************* */
/* ****************** Other service parameter values ********************* */
/* ******************                                ********************* */
/* *********************************************************************** */

/* ****************** Mode selector values ******************************* */
/* ******************                      ******************************* */

/* Note:
   Do not change! Used as presentation protocol parameter values too.
*/

#define RCro70_X400Mode1984       0
#define RCro71_NormalMode         1
#define RCro72_SNIprivate        -1

/* ****************** Application protocol values ************************ */
/* ****************** (X.410-1984-mode)           ************************ */

/* Note:
   Do not change! Used as RTSE protocol parameter values too.
*/

#define RCro73_P1ApplProt         1
#define RCro74_P1ApplProt88      12
#define RCro75_P3ApplProt         3

/* ****************** Application context identifier value *************** */
/* ****************** (X.410-1984-mode)                    *************** */

#define RCro76_NoApplContext     -1

/* Application context identifier: Private definition.
   Index in Application Context List.
   This list is a part of the Application Entity Invocation Context
   ("AEIcontext", see rouctr4.h). The list is defined only for normal
   mode applications.
*/

/* *********************************************************************** */
/* ****************** Application Entity Title *************************** */
/* ******************      (AeTitle)           *************************** */

/* - This structure can only be used in local heap memory.
   - The NULL-hande indicates that application entity title is absent.
   - The length of memory area is equal to "sizeof (rTro71_stAeTitleForm2)"
     if application entity title form is equal to AP-title-form2.
   - The length of memory area is equal to "sizeof (rTro70_stAeTitleForm1)"
     if application entity title form is equal to AP-title-form1.
   - The presentation of the AeTitles is taken pattern from the ISO 8650
     :1988 Technical Corregendum 1: 1990.
*/


#define RCro7f_MaxRDNSequence     5  /* max. sizes for the arrays          */
#define RCro7g_MaxRDN             5  /* of DistinguishedName (Form1)       */
/* #define RCro7h_MaxAttriValue	  5 by kc on 29-07-94   */
#define RCro7h_MaxAttriValue	  16


/* ******** AttributeValueAssertion (AVA) ******************************** */

typedef struct rSro68_stAVA {
    rTro00_vObjectId      vAVA_Type;  /* object identifier                 */
    signed   long  int    lAVA_Value; /* number of vAVA_Value[] elements   */
    char                  vAVA_Value [RCro7h_MaxAttriValue];
				      /* T61String                         */
} rTro68_stAVA;


/* ******** RelativeDistinguishedName (RDN)******************************* */

typedef struct rSro69_stRDN {
    signed   long  int    lRDN;       /* number of vRDN[] elements         */
    rTro68_stAVA          vRDN [RCro7g_MaxRDN];
				      /* RelativeDistinguishedName
					  ::= SET OF AVA                   */
} rTro69_stRDN;


/* ******** AeTitleForm1 (Distinguished Name) **************************** */

typedef struct rSro70_stAeTitleForm1 {
    signed   long  int    lAeTitleForm;   /* union member                  */
    signed   long  int    lApTitle;       /* 0 for no ApTitle or
					     number of vApTitle[] elements */
    rTro69_stRDN          vApTitle [RCro7f_MaxRDNSequence];
					  /* Name::= SEQUENCE OF RDN
					     the CHOICE isn't implemented  */
    signed   long  int    lAeQuali;       /* 0 for no AeQuali or
					     number of vAeQuali[] elements */
    rTro68_stAVA          vAeQuali [RCro7g_MaxRDN];
					  /* RelativeDistinguishedName
					      ::= SET OF AVA               */
} rTro70_stAeTitleForm1;


/* ******** AeTitleForm2 (Object Identifier) ***************************** */

typedef struct rSro71_stAeTitleForm2 {
    signed   long  int    lAeTitleForm;   /* union member                  */
    rTro00_vObjectId      oApTitleObjId;
    signed   long  int    bAeQualInt;     /* OPTIONAL                      */
    signed   long  int    lAeQualInt;
} rTro71_stAeTitleForm2;


/* ******** Application Entity Title (AeTitle) *************************** */

typedef union rUro72_unAeTitle {
    signed   long  int    lAeTitleForm;   /* union member                  */
    rTro70_stAeTitleForm1 stAeTitleForm1;
    rTro71_stAeTitleForm2 stAeTitleForm2;
} rTro72_unAeTitle;

/* values for the union member lAeTitleForm */
#define RCro78_RelDisName         1       /* AeTitleForm1 (DistName)       */
#define RCro77_ObjId              2       /* AeTitleForm2 (ObjectId)       */

/* *********************************************************************** */
/* ****************** Application invocation identifier ****************** */
/* ******************                                   ****************** */

typedef struct rSro74_stInvoc {
    signed   long  int    bApInvocId;        /* OPTIONAL */
    signed   long  int    lApInvocId;
    signed   long  int    bAeInvocId;        /* OPTIONAL */
    signed   long  int    lAeInvocId;
} rTro74_stInvoc;

/* *********************************************************************** */
/* ****************** Local syntax of ANY DEFINED BY ********************* */
/* ****************** implicit encoding information  ********************* */

/* These data types shall be used for the local syntax of ASN.1 types
   which appear in ANY DEFINED BY types (implicit encoding information).
*/

/* Note:
   Do not change the type values! Must be identical with the ASN.1 tags
   for INTEGER and OBJECT IDENTIFIER.
*/
                                      /* ANY DEFINED BY types         */
#define RC047a_DefinedByInteger   2   /* INTEGER                      */
#define RC047b_DefinedByObjectId  6   /* OBJECT IDENTIFIER            */

typedef union rUro75_unDefinedBy {
   signed   long  int      lInteger;
   rTro00_vObjectId        oObjectId;
} rTro75_unDefinedBy;

typedef struct rSro76_stDefinedBy {
   signed   long  int      lType;
   rTro75_unDefinedBy      unValue;
} rTro76_stDefinedBy;

/* *********************************************************************** */
/* ****************** Connection establishment result values ************* */
/* ******************                                        ************* */

#define RCro80_Accept          0x00000000
#define RCro81_UserReject      0x00ff0000

/* *********************************************************************** */
/* ****************** Connection Response result value in    ************* */
/* ****************** case of a redirection.                 ************* */
/* ****************** WARNING: Check only for Byte 0, High   ************* */
/* ******************          Nibble.                       ************* */

#define RCro85_Redirection     0x00000030


/* ****************** Connection release result values ******************* */
/* ******************                                  ******************* */

#define RCro90_Affirmative      0x00
#define RCro91_Negative         0x01

#endif	/* _ROUPLY0_H */
