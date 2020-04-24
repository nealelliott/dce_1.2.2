/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsExport.c,v $
 * Revision 1.1.79.2  1996/02/18  18:24:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:08  marty]
 *
 * Revision 1.1.79.1  1995/12/08  15:36:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:16  root]
 * 
 * Revision 1.1.77.3  1994/06/21  14:48:39  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:47:28  marrek]
 * 
 * Revision 1.1.77.2  1994/03/23  15:48:28  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:35:26  keutel]
 * 
 * Revision 1.1.77.1  1994/02/22  18:09:28  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:57:20  marrek]
 * 
 * Revision 1.1.75.1  1993/10/14  19:19:17  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:16:34  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:44:40  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  06:59:53  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:11:50  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  23:29:06  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:19:05  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:06:59  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:43:32  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  21:00:22  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:41:37  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:13:46  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:37:33  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsExport.c,v $ $Revision: 1.1.79.2 $ $Date: 1996/02/18 18:24:22 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYP          : MODUL                                               */
/*                                                                    */
/* NAME         : ds_export.c                                         */
/*                                                                    */
/* AUTHOR       : V. Ryan                                             */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XDS                                                 */
/*                                                                    */
/* DOK.-REF.    : X/Open Directory Services API Specification ver. 1.0*/
/*              : OSI Object Management API Specification ver. 2.0    */
/*                XDS/XOM Design Specification August 1990            */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : global variables                                    */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   1.0    | 90-10-01| Original                       | VR |         */
/*   1.1    | 90-11-07| drop 2                         | VR |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <xom.h>
#include <xomip.h>
#include <alloc.h>

#include <d2dir.h>
#include <d21iapl.h>
#include <xdsiapl.h>
#include <xds.h>
#include <xdsp.h>
#include <xdsbdcp.h>
#include <xdsgds.h>
#include <xdscds.h>
#include <xdsmdup.h>
#include <xdsdme.h>
#include <xdssap.h>

#include <xdsimport.h>
#include <xdscommon.h>


/* define globals */

dsP_wkspace_entry dsP_wkspace_table[DSP_MAX_WKSPACES]={0};
dsP_bind_entry    dsP_bind_table[DSP_MAX_SESSIONS]={{(OM_private_object)0,0}};
OM_uint32         dsP_def_auth_mech = DSX_DCE_AUTH;

#ifdef THREADSAFE
pthread_once_t   dsP_once = pthread_once_init;
pthread_mutex_t  dsP_wksptab_mutex;
pthread_mutex_t  dsP_bindtab_mutex;
#else
OM_sint          dsP_once = 1;
#endif




/* define Object Identifier constants */

/* from:  xds.h */

OMP_EXPORT (DS_SERVICE_PKG)
OMP_EXPORT (DS_C_ABANDON_FAILED)
OMP_EXPORT (DS_C_ACCESS_POINT)
OMP_EXPORT (DS_C_ADDRESS)
OMP_EXPORT (DS_C_ATTRIBUTE)
OMP_EXPORT (DS_C_ATTRIBUTE_ERROR)
OMP_EXPORT (DS_C_ATTRIBUTE_LIST)
OMP_EXPORT (DS_C_ATTRIBUTE_PROBLEM)
OMP_EXPORT (DS_C_AVA)
OMP_EXPORT (DS_C_COMMON_RESULTS)
OMP_EXPORT (DS_C_COMMUNICATIONS_ERROR)
OMP_EXPORT (DS_C_COMPARE_RESULT)
OMP_EXPORT (DS_C_CONTEXT)
OMP_EXPORT (DS_C_CONTINUATION_REF)
OMP_EXPORT (DS_C_DS_DN)
OMP_EXPORT (DS_C_DS_RDN)
OMP_EXPORT (DS_C_ENTRY_INFO)
OMP_EXPORT (DS_C_ENTRY_INFO_SELECTION)
OMP_EXPORT (DS_C_ENTRY_MOD)
OMP_EXPORT (DS_C_ENTRY_MOD_LIST)
OMP_EXPORT (DS_C_ERROR)
OMP_EXPORT (DS_C_EXT)
OMP_EXPORT (DS_C_FILTER)
OMP_EXPORT (DS_C_FILTER_ITEM)
OMP_EXPORT (DS_C_LIBRARY_ERROR)
OMP_EXPORT (DS_C_LIST_INFO)
OMP_EXPORT (DS_C_LIST_INFO_ITEM)
OMP_EXPORT (DS_C_LIST_RESULT)
OMP_EXPORT (DS_C_NAME)
OMP_EXPORT (DS_C_NAME_ERROR)
OMP_EXPORT (DS_C_OPERATION_PROGRESS)
OMP_EXPORT (DS_C_PARTIAL_OUTCOME_QUAL)
OMP_EXPORT (DS_C_PRESENTATION_ADDRESS)
OMP_EXPORT (DS_C_READ_RESULT)
OMP_EXPORT (DS_C_REFERRAL)
OMP_EXPORT (DS_C_RELATIVE_NAME)
OMP_EXPORT (DS_C_SEARCH_INFO)
OMP_EXPORT (DS_C_SEARCH_RESULT)
OMP_EXPORT (DS_C_SECURITY_ERROR)
OMP_EXPORT (DS_C_SERVICE_ERROR)
OMP_EXPORT (DS_C_SESSION)
OMP_EXPORT (DS_C_SYSTEM_ERROR)
OMP_EXPORT (DS_C_UPDATE_ERROR)


/* from:  xdsbdcp.h */


OMP_EXPORT (DS_BASIC_DIR_CONTENTS_PKG)
OMP_EXPORT (DS_C_SEARCH_GUIDE)
OMP_EXPORT (DS_C_POSTAL_ADDRESS)
OMP_EXPORT (DS_C_TELEX_NBR)
OMP_EXPORT (DS_C_TELETEX_TERM_IDENT)
OMP_EXPORT (DS_C_FACSIMILE_PHONE_NBR)
OMP_EXPORT (DS_C_SEARCH_CRITERION)
OMP_EXPORT (DS_A_ALIASED_OBJECT_NAME)
OMP_EXPORT (DS_A_BUSINESS_CATEGORY)
OMP_EXPORT (DS_A_COMMON_NAME)
OMP_EXPORT (DS_A_COUNTRY_NAME)
OMP_EXPORT (DS_A_DESCRIPTION)
OMP_EXPORT (DS_A_DEST_INDICATOR)
OMP_EXPORT (DS_A_FACSIMILE_PHONE_NBR)
OMP_EXPORT (DS_A_INTERNAT_ISDN_NBR)
OMP_EXPORT (DS_A_KNOWLEDGE_INFO)
OMP_EXPORT (DS_A_LOCALITY_NAME)
OMP_EXPORT (DS_A_MEMBER)
OMP_EXPORT (DS_A_OBJECT_CLASS)
OMP_EXPORT (DS_A_ORG_NAME)
OMP_EXPORT (DS_A_ORG_UNIT_NAME)
OMP_EXPORT (DS_A_OWNER)
OMP_EXPORT (DS_A_PHONE_NBR)
OMP_EXPORT (DS_A_PHYS_DELIV_OFF_NAME)
OMP_EXPORT (DS_A_POST_OFFICE_BOX)
OMP_EXPORT (DS_A_POSTAL_ADDRESS)
OMP_EXPORT (DS_A_POSTAL_CODE)
OMP_EXPORT (DS_A_PREF_DELIV_METHOD)
OMP_EXPORT (DS_A_PRESENTATION_ADDRESS)
OMP_EXPORT (DS_A_REGISTERED_ADDRESS)
OMP_EXPORT (DS_A_ROLE_OCCUPANT)
OMP_EXPORT (DS_A_SEARCH_GUIDE)
OMP_EXPORT (DS_A_SEE_ALSO)
OMP_EXPORT (DS_A_SERIAL_NBR)
OMP_EXPORT (DS_A_STATE_OR_PROV_NAME)
OMP_EXPORT (DS_A_STREET_ADDRESS)
OMP_EXPORT (DS_A_SUPPORT_APPLIC_CONTEXT)
OMP_EXPORT (DS_A_SURNAME)
OMP_EXPORT (DS_A_TELETEX_TERM_IDENT)
OMP_EXPORT (DS_A_TELEX_NBR)
OMP_EXPORT (DS_A_TITLE)
OMP_EXPORT (DS_A_USER_PASSWORD)
OMP_EXPORT (DS_A_X121_ADDRESS)
OMP_EXPORT (DS_O_ALIAS)
OMP_EXPORT (DS_O_APPLIC_ENTITY)
OMP_EXPORT (DS_O_APPLIC_PROCESS)
OMP_EXPORT (DS_O_COUNTRY)
OMP_EXPORT (DS_O_DEVICE)
OMP_EXPORT (DS_O_DSA)
OMP_EXPORT (DS_O_GROUP_OF_NAMES)
OMP_EXPORT (DS_O_LOCALITY)
OMP_EXPORT (DS_O_ORG)
OMP_EXPORT (DS_O_ORG_PERSON)
OMP_EXPORT (DS_O_ORG_ROLE)
OMP_EXPORT (DS_O_ORG_UNIT)
OMP_EXPORT (DS_O_PERSON)
OMP_EXPORT (DS_O_RESIDENTIAL_PERSON)
OMP_EXPORT (DS_O_TOP)



/* from:  xdsgds.h */

OMP_EXPORT (DSX_GDS_PKG)
OMP_EXPORT (DSX_O_SCHEMA)
OMP_EXPORT (DSX_A_MASTER_KNOWLEDGE)
OMP_EXPORT (DSX_A_ACL)
OMP_EXPORT (DSX_A_TIME_STAMP)
OMP_EXPORT (DSX_A_SHADOWED_BY)
OMP_EXPORT (DSX_A_SRT)
OMP_EXPORT (DSX_A_OCT)
OMP_EXPORT (DSX_A_AT)
OMP_EXPORT (DSX_A_DEFAULT_DSA)
OMP_EXPORT (DSX_A_LOCAL_DSA)
OMP_EXPORT (DSX_A_CLIENT)
OMP_EXPORT (DSX_A_DNLIST)
OMP_EXPORT (DSX_A_SHADOWING_JOB)
OMP_EXPORT (DSX_C_GDS_SESSION)
OMP_EXPORT (DSX_C_GDS_CONTEXT)
OMP_EXPORT (DSX_C_GDS_ACL)
OMP_EXPORT (DSX_C_GDS_ACL_ITEM)


/* from:  xdscds.h */

OMP_EXPORT (DSX_A_CDS_Members)
OMP_EXPORT (DSX_A_CDS_GroupRevoke)
OMP_EXPORT (DSX_A_CDS_CTS)
OMP_EXPORT (DSX_A_CDS_UTS)
OMP_EXPORT (DSX_A_CDS_Class)
OMP_EXPORT (DSX_A_CDS_ClassVersion)
OMP_EXPORT (DSX_A_CDS_ObjectUUID)
OMP_EXPORT (DSX_A_CDS_Address)
OMP_EXPORT (DSX_A_CDS_Replicas)
OMP_EXPORT (DSX_A_CDS_AllUpTo)
OMP_EXPORT (DSX_A_CDS_Convergence)
OMP_EXPORT (DSX_A_CDS_InCHName)
OMP_EXPORT (DSX_A_CDS_ParentPointer)
OMP_EXPORT (DSX_A_CDS_DirectoryVersion)
OMP_EXPORT (DSX_A_CDS_UpgradeTo)
OMP_EXPORT (DSX_A_CDS_LinkTarget)
OMP_EXPORT (DSX_A_CDS_LinkTimeout)
OMP_EXPORT (DSX_A_CDS_Towers)
OMP_EXPORT (DSX_A_CDS_CHName)
OMP_EXPORT (DSX_A_CDS_CHLastAddress)
OMP_EXPORT (DSX_A_CDS_CHUpPointers)
OMP_EXPORT (DSX_A_CDS_CHState)
OMP_EXPORT (DSX_UUID)
OMP_EXPORT (DSX_TYPELESS_RDN)
OMP_EXPORT (DSX_NORMAL_SIMPLE_NAME)
OMP_EXPORT (DSX_BINARY_SIMPLE_NAME)


/* from:  xdsmdup.h */

OMP_EXPORT (DS_MHS_DIR_USER_PKG)
OMP_EXPORT (DS_A_DELIV_CONTENT_LENGTH)
OMP_EXPORT (DS_A_DELIV_CONTENT_TYPES)
OMP_EXPORT (DS_A_DELIV_EITS)
OMP_EXPORT (DS_A_DL_MEMBERS)
OMP_EXPORT (DS_A_DL_SUBMIT_PERMS)
OMP_EXPORT (DS_A_MESSAGE_STORE)
OMP_EXPORT (DS_A_OR_ADDRESSES)
OMP_EXPORT (DS_A_PREF_DELIV_METHODS)
OMP_EXPORT (DS_A_SUPP_AUTO_ACTIONS)
OMP_EXPORT (DS_A_SUPP_CONTENT_TYPES)
OMP_EXPORT (DS_A_SUPP_OPT_ATTRIBUTES)
OMP_EXPORT (DS_O_MHS_DISTRIBUTION_LIST)
OMP_EXPORT (DS_O_MHS_MESSAGE_STORE)
OMP_EXPORT (DS_O_MHS_MESSAGE_TRANS_AG)
OMP_EXPORT (DS_O_MHS_USER)
OMP_EXPORT (DS_O_MHS_USER_AG)
OMP_EXPORT (DS_C_DL_SUBMIT_PERMS)


/* from xdsdme.h */

OMP_EXPORT (DSX_O_DME_NMO_AGENT)
OMP_EXPORT (DSX_A_ALTERNATE_ADDRESS)

/* from xdssap.h */

OMP_EXPORT (DS_STRONG_AUTHENT_PKG)
OMP_EXPORT (DS_A_AUTHORITY_REVOC_LIST)
OMP_EXPORT (DS_A_CA_CERT)
OMP_EXPORT (DS_A_CERT_REVOC_LIST)
OMP_EXPORT (DS_A_CROSS_CERT_PAIR)
OMP_EXPORT (DS_A_USER_CERT)
OMP_EXPORT (DS_O_CERT_AUTHORITY)
OMP_EXPORT (DS_O_STRONG_AUTHENT_USER)
OMP_EXPORT (DS_C_ALGORITHM_IDENT)
OMP_EXPORT (DS_C_CERT)
OMP_EXPORT (DS_C_CERT_LIST)
OMP_EXPORT (DS_C_CERT_PAIR)
OMP_EXPORT (DS_C_CERT_SUBLIST)
OMP_EXPORT (DS_C_SIGNATURE)
