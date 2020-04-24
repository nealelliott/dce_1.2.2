/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xdsimport.h,v $
 * Revision 1.1.78.2  1996/02/18  23:37:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:47  marty]
 *
 * Revision 1.1.78.1  1995/12/08  15:36:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:25  root]
 * 
 * Revision 1.1.76.4  1994/07/06  15:09:19  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:25:34  marrek]
 * 
 * Revision 1.1.76.3  1994/06/21  14:48:59  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:49:10  marrek]
 * 
 * Revision 1.1.76.2  1994/05/10  16:04:11  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:15:48  marrek]
 * 
 * Revision 1.1.76.1  1994/02/22  18:59:50  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:11:24  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:03:05  marrek]
 * 
 * Revision 1.1.74.1  1993/10/14  19:23:49  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:22:43  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  10:02:35  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  07:11:09  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:20:40  marrek]
 * 
 * Revision 1.1.4.2  1992/12/30  23:30:40  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:20:39  bbelch]
 * 
 * Revision 1.1.2.3  1992/06/30  21:00:34  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:41:55  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:16:34  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:43:34  melman]
 * 
 * $EndLog$
 */
#ifndef _XDSIMPORT_H
#define _XDSIMPORT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xdsimport_rcsid[] = "@(#)$RCSfile: xdsimport.h,v $ $Revision: 1.1.78.2 $ $Date: 1996/02/18 23:37:58 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : xdsimport.h                                         */
/*                                                                    */
/* AUTHOR       : V. Ryan                                             */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XDS                                                 */
/*                                                                    */
/* DOK.-REF.    : X/Open CAE Spec. API to Directory Services (XDS)    */
/*                X/Open CAE Spec. OSI-Abstract-Data Manip. API (XOM) */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : extern declarations for global variables,           */
/*                declaration of XDS object ids for internal use      */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   1.0    | 90-10-01| Original                       | VR |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/


/* Macro to initialise the syntax and value of an object identifier
 */
#define OMP_OID_DESC(type, oid_name) \
        { (type), OM_S_OBJECT_IDENTIFIER_STRING, \
         { { OMP_LENGTH(oid_name) , OMP_X_##oid_name } } }


/* Macro to make class constants available within a compilation unit
 */
#define OMP_IMPORT(class_name) \
                extern char OMP_X_##class_name []; \
                extern OM_string dsP_##class_name;


/* Macro to allocate memory for class constants within a compilation unit
 */
#define OMP_EXPORT(class_name) \
        char OMP_X_##class_name[] = OMP_O_##class_name; \
        OM_string dsP_##class_name = \
            { OMP_LENGTH(class_name), OMP_X_##class_name };




/* Object Identifier constants */

/* from:  xds.h */

OMP_IMPORT (DS_SERVICE_PKG)
OMP_IMPORT (DS_C_ABANDON_FAILED)
OMP_IMPORT (DS_C_ACCESS_POINT)
OMP_IMPORT (DS_C_ADDRESS)
OMP_IMPORT (DS_C_ATTRIBUTE)
OMP_IMPORT (DS_C_ATTRIBUTE_ERROR)
OMP_IMPORT (DS_C_ATTRIBUTE_LIST)
OMP_IMPORT (DS_C_ATTRIBUTE_PROBLEM)
OMP_IMPORT (DS_C_AVA)
OMP_IMPORT (DS_C_COMMON_RESULTS)
OMP_IMPORT (DS_C_COMMUNICATIONS_ERROR)
OMP_IMPORT (DS_C_COMPARE_RESULT)
OMP_IMPORT (DS_C_CONTEXT)
OMP_IMPORT (DS_C_CONTINUATION_REF)
OMP_IMPORT (DS_C_DS_DN)
OMP_IMPORT (DS_C_DS_RDN)
OMP_IMPORT (DS_C_ENTRY_INFO)
OMP_IMPORT (DS_C_ENTRY_INFO_SELECTION)
OMP_IMPORT (DS_C_ENTRY_MOD)
OMP_IMPORT (DS_C_ENTRY_MOD_LIST)
OMP_IMPORT (DS_C_ERROR)
OMP_IMPORT (DS_C_EXT)
OMP_IMPORT (DS_C_FILTER)
OMP_IMPORT (DS_C_FILTER_ITEM)
OMP_IMPORT (DS_C_LIBRARY_ERROR)
OMP_IMPORT (DS_C_LIST_INFO)
OMP_IMPORT (DS_C_LIST_INFO_ITEM)
OMP_IMPORT (DS_C_LIST_RESULT)
OMP_IMPORT (DS_C_NAME)
OMP_IMPORT (DS_C_NAME_ERROR)
OMP_IMPORT (DS_C_OPERATION_PROGRESS)
OMP_IMPORT (DS_C_PARTIAL_OUTCOME_QUAL)
OMP_IMPORT (DS_C_PRESENTATION_ADDRESS)
OMP_IMPORT (DS_C_READ_RESULT)
OMP_IMPORT (DS_C_REFERRAL)
OMP_IMPORT (DS_C_RELATIVE_NAME)
OMP_IMPORT (DS_C_SEARCH_INFO)
OMP_IMPORT (DS_C_SEARCH_RESULT)
OMP_IMPORT (DS_C_SECURITY_ERROR)
OMP_IMPORT (DS_C_SERVICE_ERROR)
OMP_IMPORT (DS_C_SESSION)
OMP_IMPORT (DS_C_SYSTEM_ERROR)
OMP_IMPORT (DS_C_UPDATE_ERROR)


/* from:  xdsbdcp.h */

OMP_IMPORT (DS_BASIC_DIR_CONTENTS_PKG)
OMP_IMPORT (DS_C_SEARCH_GUIDE)
OMP_IMPORT (DS_C_POSTAL_ADDRESS)
OMP_IMPORT (DS_C_TELEX_NBR)
OMP_IMPORT (DS_C_TELETEX_TERM_IDENT)
OMP_IMPORT (DS_C_FACSIMILE_PHONE_NBR)
OMP_IMPORT (DS_C_SEARCH_CRITERION)
OMP_IMPORT (DS_A_ALIASED_OBJECT_NAME)
OMP_IMPORT (DS_A_BUSINESS_CATEGORY)
OMP_IMPORT (DS_A_COMMON_NAME)
OMP_IMPORT (DS_A_COUNTRY_NAME)
OMP_IMPORT (DS_A_DESCRIPTION)
OMP_IMPORT (DS_A_DEST_INDICATOR)
OMP_IMPORT (DS_A_FACSIMILE_PHONE_NBR)
OMP_IMPORT (DS_A_INTERNAT_ISDN_NBR)
OMP_IMPORT (DS_A_KNOWLEDGE_INFO)
OMP_IMPORT (DS_A_LOCALITY_NAME)
OMP_IMPORT (DS_A_MEMBER)
OMP_IMPORT (DS_A_OBJECT_CLASS)
OMP_IMPORT (DS_A_ORG_NAME)
OMP_IMPORT (DS_A_ORG_UNIT_NAME)
OMP_IMPORT (DS_A_OWNER)
OMP_IMPORT (DS_A_PHONE_NBR)
OMP_IMPORT (DS_A_PHYS_DELIV_OFF_NAME)
OMP_IMPORT (DS_A_POST_OFFICE_BOX)
OMP_IMPORT (DS_A_POSTAL_ADDRESS)
OMP_IMPORT (DS_A_POSTAL_CODE)
OMP_IMPORT (DS_A_PREF_DELIV_METHOD)
OMP_IMPORT (DS_A_PRESENTATION_ADDRESS)
OMP_IMPORT (DS_A_REGISTERED_ADDRESS)
OMP_IMPORT (DS_A_ROLE_OCCUPANT)
OMP_IMPORT (DS_A_SEARCH_GUIDE)
OMP_IMPORT (DS_A_SEE_ALSO)
OMP_IMPORT (DS_A_SERIAL_NBR)
OMP_IMPORT (DS_A_STATE_OR_PROV_NAME)
OMP_IMPORT (DS_A_STREET_ADDRESS)
OMP_IMPORT (DS_A_SUPPORT_APPLIC_CONTEXT)
OMP_IMPORT (DS_A_SURNAME)
OMP_IMPORT (DS_A_TELETEX_TERM_ID)
OMP_IMPORT (DS_A_TELEX_NBR)
OMP_IMPORT (DS_A_TITLE)
OMP_IMPORT (DS_A_USER_PASSWORD)
OMP_IMPORT (DS_A_X121_ADDRESS)
OMP_IMPORT (DS_O_ALIAS)
OMP_IMPORT (DS_O_APPLIC_ENTITY)
OMP_IMPORT (DS_O_APPLIC_PROCESS)
OMP_IMPORT (DS_O_COUNTRY)
OMP_IMPORT (DS_O_DEVICE)
OMP_IMPORT (DS_O_DSA)
OMP_IMPORT (DS_O_GROUP_OF_NAMES)
OMP_IMPORT (DS_O_LOCALITY)
OMP_IMPORT (DS_O_ORG)
OMP_IMPORT (DS_O_ORG_PERSON)
OMP_IMPORT (DS_O_ORG_ROLE)
OMP_IMPORT (DS_O_ORG_UNIT)
OMP_IMPORT (DS_O_PERSON)
OMP_IMPORT (DS_O_RESIDENTIAL_PERSON)
OMP_IMPORT (DS_O_TOP)


/* from:  xdsgds.h */

OMP_IMPORT (DSX_GDS_PKG)
OMP_IMPORT (DSX_O_SCHEMA)
OMP_IMPORT (DSX_A_MASTER_KNOWLEDGE)
OMP_IMPORT (DSX_A_ACL)
OMP_IMPORT (DSX_A_TIME_STAMP)
OMP_IMPORT (DSX_A_SHADOWED_BY)
OMP_IMPORT (DSX_A_SRT)
OMP_IMPORT (DSX_A_OCT)
OMP_IMPORT (DSX_A_AT)
OMP_IMPORT (DSX_A_DEFAULT_DSA)
OMP_IMPORT (DSX_A_LOCAL_DSA)
OMP_IMPORT (DSX_A_CLIENT)
OMP_IMPORT (DSX_A_DNLIST)
OMP_IMPORT (DSX_A_SHADOWING_JOB)
OMP_IMPORT (DSX_C_GDS_SESSION)
OMP_IMPORT (DSX_C_GDS_CONTEXT)
OMP_IMPORT (DSX_C_GDS_ACL)
OMP_IMPORT (DSX_C_GDS_ACL_ITEM)


/* from:  xdscds.h */

OMP_IMPORT (DSX_A_CDS_Members)
OMP_IMPORT (DSX_A_CDS_GroupRevoke)
OMP_IMPORT (DSX_A_CDS_CTS)
OMP_IMPORT (DSX_A_CDS_UTS)
OMP_IMPORT (DSX_A_CDS_Class)
OMP_IMPORT (DSX_A_CDS_ClassVersion)
OMP_IMPORT (DSX_A_CDS_ObjectUUID)
OMP_IMPORT (DSX_A_CDS_Address)
OMP_IMPORT (DSX_A_CDS_Replicas)
OMP_IMPORT (DSX_A_CDS_AllUpTo)
OMP_IMPORT (DSX_A_CDS_Convergence)
OMP_IMPORT (DSX_A_CDS_InCHName)
OMP_IMPORT (DSX_A_CDS_ParentPointer)
OMP_IMPORT (DSX_A_CDS_DirectoryVersion)
OMP_IMPORT (DSX_A_CDS_UpgradeTo)
OMP_IMPORT (DSX_A_CDS_LinkTarget)
OMP_IMPORT (DSX_A_CDS_LinkTimeout)
OMP_IMPORT (DSX_A_CDS_CHName)
OMP_IMPORT (DSX_A_CDS_CHLastAddress)
OMP_IMPORT (DSX_A_CDS_CHUpPointers)
OMP_IMPORT (DSX_A_CDS_CHState)
OMP_IMPORT (DSX_UUID)
OMP_IMPORT (DSX_TYPELESS_RDN)
OMP_IMPORT (DSX_NORMAL_SIMPLE_NAME)
OMP_IMPORT (DSX_BINARY_SIMPLE_NAME)


/* from:  xdsmdup.h */

OMP_IMPORT (DS_MHS_DIR_USER_PKG)
OMP_IMPORT (DS_A_DELIV_CONTENT_LENGTH)
OMP_IMPORT (DS_A_DELIV_CONTENT_TYPES)
OMP_IMPORT (DS_A_DELIV_EITS)
OMP_IMPORT (DS_A_DL_MEMBERS)
OMP_IMPORT (DS_A_DL_SUBMIT_PERMS)
OMP_IMPORT (DS_A_MESSAGE_STORE)
OMP_IMPORT (DS_A_OR_ADDRESSES)
OMP_IMPORT (DS_A_PREF_DELIV_METHODS)
OMP_IMPORT (DS_A_SUPP_AUTO_ACTIONS)
OMP_IMPORT (DS_A_SUPP_CONTENT_TYPES)
OMP_IMPORT (DS_A_SUPP_OPT_ATTRIBUTES)
OMP_IMPORT (DS_O_MHS_DISTRIBUTION_LIST)
OMP_IMPORT (DS_O_MHS_MESSAGE_STORE)
OMP_IMPORT (DS_O_MHS_MESSAGE_TRANS_AG)
OMP_IMPORT (DS_O_MHS_USER)
OMP_IMPORT (DS_O_MHS_USER_AG)
OMP_IMPORT (DS_C_DL_SUBMIT_PERMS)

/* from xdssap.h */

OMP_IMPORT (DS_STRONG_AUTHENT_PKG)
OMP_IMPORT (DS_A_AUTHORITY_REVOC_LIST)
OMP_IMPORT (DS_A_CA_CERT)
OMP_IMPORT (DS_A_CERT_REVOC_LIST)
OMP_IMPORT (DS_A_CROSS_CERT_PAIR)
OMP_IMPORT (DS_A_USER_CERT)
OMP_IMPORT (DS_O_CERT_AUTHORITY)
OMP_IMPORT (DS_O_STRONG_AUTHENT_USER)
OMP_IMPORT (DS_C_ALGORITHM_IDENT)
OMP_IMPORT (DS_C_CERT)
OMP_IMPORT (DS_C_CERT_LIST)
OMP_IMPORT (DS_C_CERT_PAIR)
OMP_IMPORT (DS_C_CERT_SUBLIST)
OMP_IMPORT (DS_C_SIGNATURE)

/* from xdsdme.h */
OMP_IMPORT (DSX_A_ALTERNATE_ADDRESS)
OMP_IMPORT (DSX_O_DME_NMO_AGENT)

#endif /* _XDSIMPORT_H */
