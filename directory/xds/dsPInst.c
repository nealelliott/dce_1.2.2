/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPInst.c,v $
 * Revision 1.1.6.2  1996/02/18  18:24:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:24  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:32:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:35  root]
 * 
 * Revision 1.1.4.3  1994/06/21  14:48:47  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:48:14  marrek]
 * 
 * Revision 1.1.4.2  1994/05/10  16:03:55  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:14:36  marrek]
 * 
 * Revision 1.1.4.1  1994/02/22  18:10:12  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:20:44  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:59:38  marrek]
 * 
 * Revision 1.1.2.2  1993/08/10  07:03:50  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:14:53  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:16:47  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:05:00  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:33:32  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:42:27  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:18:40  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:46:04  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsPInst.c,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 18:24:47 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/* ****************************************************************** */
/*                                                                    */
/*   * COPYRIGHT   (C)  1990 BY BULL S.A                      */
/*   * All rights reserved                                            */
/*   *                                                                */
/*   * This software is furnished under licence and may be used only  */
/*   * in accordance with the terms of that licence and with the      */
/*   * inclusion of the above copyright notice.                       */
/*   * This software may not be provided or otherwise made available  */
/*   * to, or used by, any other person. No title to or ownership of  */
/*   * the software is hereby transferred.                            */
/*                                                                    */
/* ****************************************************************** */
/*  @(#)dsPInst.c   1.15 (Bull S.A) 6/12/92 */

/*****************************************************************
 ** xds_instance.c -- XDS dependencies for omP_instance().  **
 **                             **
 ** Author : J.Zrihen, DASD/SSD.                **
 *****************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <xom.h>
#include <xomip.h>
#include <d2dir.h>
#include <xdsiapl.h>
#include <xdsbdcp.h>
#include <xmhdef.h>
#include <xdsimport.h>
#include <xdspackage.h>
#include <dsPInst.h>

/*
 *  HIERARCHICAL ORGANISATION OF THE OM CLASSES.
 * _____________________________________________
 *
 * Nota : Capital letters are concrete classes.
 * Example : 'PRESENTATION-ADDRESS' is an immediate subclass of
 *       the abstract class 'Address'.
 *
 *  XDS SERVICE PACKAGE OM Attributes :
 * -------------------------------------
 *
 * Object
 * - ACCESS-POINT
 * - Address
 *   - PRESENTATION-ADDRESS
 * - ATTRIBUTE
 *   - AVA
 *   - ENTRY-MOD
 *   - FILTER-ITEM
 * - ATTRIBUTE-LIST
 *   - ENTRY-INFO
 * - Common-Results
 *   - COMPARE-RESULT
 *   - LIST-INFO
 *   - READ-RESULT
 *   - SEARCH-INFO
 * - CONTEXT
 * - CONTINUATION-REF
 * - ENTRY-INFO-SELECTION
 * - ENTRY-MOD-LIST
 * - EXT
 * - FILTER
 * - LIST-INFO-ITEM
 * - LIST-RESULT
 * - Name
 *   - DS-DN
 * - OPERATION-PROGRESS
 * - PARTIAL-OUTCOME-QUAL
 * - Relative-Name
 *   - DS-RDN
 * - SEARCH-RESULT
 * - SESSION
 *
 *  XDS SERVICE Errors :
 * ----------------------
 *
 * Object
 * - ATTRIBUTE-ERROR
 * - CONTINUATION-REF
 *   - REFERRAL
 * - Error
 *   - ABANDON-FAILED
 *   - ATTRIBUTE-PROBLEM
 *   - COMMUNICATIONS-ERROR
 *   - LIBRARY-ERROR
 *   - NAME-ERROR
 *   - SECURITY-ERROR
 *   - SERVICE-ERROR
 *   - SYSTEM-ERROR
 *   - UPDATE-ERROR
 *   - NAME-ERROR
 *
 *  BDCP AND SAP PACKAGES OM Attributes :
 * ---------------------------------------
 *
 * Object
 * - ALGORITHM-IDENTIFIER       SAP
 * - CERTIFICATE-PAIR           SAP
 * - FACSIMILE-PHONE-NBR        BDCP
 * - POSTAL-ADDRESS         	BDCP
 * - SEARCH-CRITERION           BDCP
 * - SEARCH-GUIDE           	BDCP
 * - Signature              	SAP
 *   - CERTIFICATE          	SAP
 *   - CERTIFICATE_LIST         SAP
 *   - CERTIFICATE_SUBLIST      SAP
 * - TELETEX-TERM-IDENT         BDCP
 * - TELEX-NBR              	BDCP
 */
/*
 *  MDUP PACKAGE OM Attributes :
 * ---------------------------------------
 *
 * Object
 * - OR-Address                     MDUP
 *   - OR-NAME                      MDUP
 * - DL-SUBMIT-PERMS            MDUP
 */
/*
 * XDS Class Hierarchy definitions.
 */
#define ADDRESS(t)        ( (t) == XDS_PRESENTATION_ADDRESS)

#define ATTRIBUTE(t)      ( (t) == XDS_AVA  			|| \
							(t) == XDS_ENTRY_MOD 		|| \
						   	(t) == XDS_FILTER_ITEM		)

#define ATTR_LIST(t)      ( (t) == XDS_ENTRY_INFO		)

#define COMMON_RES(t)     ( (t) == XDS_COMPARE_RESULT 	|| \
							(t) == XDS_LIST_INFO 		|| \
						  	(t) == XDS_READ_RESULT   	|| \
						  	(t) == XDS_SEARCH_INFO		)

#define NAME(t)           ( (t) == XDS_DS_DN			)

#define RELATIVE_NAME(t)  ( (t) == XDS_DS_RDN			)

#define SIGNATURE(t)	  ( (t) == XDS_SIGNATURE		)

DEFINE_INSTANCE_MDUP_PACKAGE
/*
#define OR_ADDRESS(t)     ((t) == XMH_OR_NAME)
*/

/*
 * XDS Errors.
 */
#define CONT_REF(t) ((t) == XDS_REFERRAL)
#define ERROR(t)    ((t) == XDS_ABANDON_FAILED       || (t) == XDS_ATTRIBUTE_PROBLEM ||\
             (t) == XDS_COMMUNICATIONS_ERROR || (t) == XDS_LIBRARY_ERROR     ||\
             (t) == XDS_NAME_ERROR           || (t) == XDS_SECURITY_ERROR    ||\
             (t) == XDS_SERVICE_ERROR        || (t) == XDS_SYSTEM_ERROR      ||\
             (t) == XDS_UPDATE_ERROR)

/**
 ** dsP_instance_switch -- Check the class of "subject_type" is a subclass of "class_type".
 **/
OM_return_code
dsP_instance_switch(
const OM_workspace      workspace,
const OM_object         subject,
const OM_object_identifier  class,
const OMP_object_type   class_type,
const OMP_object_type   subject_type,
OM_boolean              *flag
)
{
    OM_return_code ret = OM_SUCCESS;
    /*
     * Check available classes for Packages.
     */
    switch (OMP_PACKAGE(class_type)) {
    case XDS_SERVICE_PACKAGE:
    case XDS_GDS_PACKAGE:
    case XDS_BDCP:
    case XMH_PACKAGE:
    case XDS_SAP_PACKAGE:
#ifdef  _MDUP_PKG_
    case XDS_MDUP:
#endif  /* _MDUP_PKG_ */

    /*
     * Check available subjects.
     */
        switch (OMP_PACKAGE(subject_type)) {
    /*
     * Service Package.
     */
        case XDS_SERVICE_PACKAGE:
            switch (OMP_CLASS(class_type)) {
            case XDS_ADDRESS:
                if (ADDRESS(OMP_CLASS(subject_type)))
                    *flag = OM_TRUE;
                break;

            case XDS_ATTRIBUTE:
                if (ATTRIBUTE(OMP_CLASS(subject_type)))
                    *flag = OM_TRUE;
                break;

            case XDS_ATTRIBUTE_LIST:
                if (ATTR_LIST(OMP_CLASS(subject_type)))
                    *flag = OM_TRUE;
                break;

            case XDS_COMMON_RESULTS:
                if (COMMON_RES(OMP_CLASS(subject_type)))
                    *flag = OM_TRUE;
                break;

            case XDS_NAME:
                if (NAME(OMP_CLASS(subject_type)))
                    *flag = OM_TRUE;
                break;

            case XDS_RELATIVE_NAME:
                if (RELATIVE_NAME(OMP_CLASS(subject_type)))
                    *flag = OM_TRUE;
                break;

            case XDS_CONTINUATION_REF:
                if (CONT_REF(OMP_CLASS(subject_type)))
                    *flag = OM_TRUE;
                break;

            case XDS_ERROR:
                if (ERROR(OMP_CLASS(subject_type)))
                    *flag = OM_TRUE;
                break;

            case XDS_ABANDON_FAILED:
            case XDS_ACCESS_POINT:
            case XDS_ATTRIBUTE_ERROR:
            case XDS_ATTRIBUTE_PROBLEM:
            case XDS_AVA:
            case XDS_COMMUNICATIONS_ERROR:
            case XDS_COMPARE_RESULT:
            case XDS_CONTEXT:
            case XDS_DS_DN:
            case XDS_DS_RDN:
            case XDS_ENTRY_INFO:
            case XDS_ENTRY_INFO_SELECTION:
            case XDS_ENTRY_MOD:
            case XDS_ENTRY_MOD_LIST:
            case XDS_EXT:
            case XDS_FILTER:
            case XDS_FILTER_ITEM:
            case XDS_LIBRARY_ERROR:
            case XDS_LIST_INFO:
            case XDS_LIST_INFO_ITEM:
            case XDS_LIST_RESULT:
            case XDS_NAME_ERROR:
            case XDS_OPERATION_PROGRESS:
            case XDS_PARTIAL_OUTCOME_QUAL:
            case XDS_PRESENTATION_ADDRESS:
            case XDS_READ_RESULT:
            case XDS_REFERRAL:
            case XDS_SEARCH_INFO:
            case XDS_SEARCH_RESULT:
            case XDS_SECURITY_ERROR:
            case XDS_SERVICE_ERROR:
            case XDS_SESSION:
            case XDS_SYSTEM_ERROR:
            case XDS_UPDATE_ERROR:
                break;

            default:
                return(OM_NO_SUCH_CLASS);
            } /* endswitch */
            break;

            /*
     * Siemens Service Package.
     */
        case XDS_GDS_PACKAGE:
            switch (OMP_CLASS(class_type)) {
            case XDS_CONTEXT:
                if ( OMP_CLASS(subject_type) == XDS_GDS_CONTEXT    )
                    *flag = OM_TRUE;
                break;

            case XDS_SESSION:
                if ( OMP_CLASS(subject_type) == XDS_GDS_SESSION    )
                    *flag = OM_TRUE;
                break;

            case XDS_ABANDON_FAILED:
            case XDS_ACCESS_POINT:
            case XDS_ADDRESS:
            case XDS_ATTRIBUTE:
            case XDS_ATTRIBUTE_ERROR:
            case XDS_ATTRIBUTE_LIST:
            case XDS_ATTRIBUTE_PROBLEM:
            case XDS_AVA:
            case XDS_COMMON_RESULTS:
            case XDS_COMMUNICATIONS_ERROR:
            case XDS_COMPARE_RESULT:
            case XDS_CONTINUATION_REF:
            case XDS_DS_DN:
            case XDS_DS_RDN:
            case XDS_ENTRY_INFO:
            case XDS_ENTRY_INFO_SELECTION:
            case XDS_ENTRY_MOD:
            case XDS_ENTRY_MOD_LIST:
            case XDS_ERROR:
            case XDS_EXT:
            case XDS_FILTER:
            case XDS_FILTER_ITEM:
            case XDS_LIBRARY_ERROR:
            case XDS_LIST_INFO:
            case XDS_LIST_INFO_ITEM:
            case XDS_LIST_RESULT:
            case XDS_NAME:
            case XDS_NAME_ERROR:
            case XDS_OPERATION_PROGRESS:
            case XDS_PARTIAL_OUTCOME_QUAL:
            case XDS_PRESENTATION_ADDRESS:
            case XDS_READ_RESULT:
            case XDS_REFERRAL:
            case XDS_RELATIVE_NAME:
            case XDS_SEARCH_INFO:
            case XDS_SEARCH_RESULT:
            case XDS_SECURITY_ERROR:
            case XDS_SERVICE_ERROR:
            case XDS_SYSTEM_ERROR:
            case XDS_UPDATE_ERROR:
                break;

            default:
                return(OM_NO_SUCH_CLASS);
            } /* endswitch */
            break;

            /*
     * BDCP Package.
     */
        case XDS_BDCP:
            switch (OMP_CLASS(class_type)) {
            case XDS_FACSIMILE_PHONE_NBR:
            case XDS_POSTAL_ADDRESS:
            case XDS_SEARCH_CRITERION:
            case XDS_SEARCH_GUIDE:
            case XDS_TELETEX_TERM_IDENT:
            case XDS_TELEX_NBR:
                break;

            default:
                return(OM_NO_SUCH_CLASS);
            } /* endswitch */
            break;
	/* 
	 * SAP Package
	 */
        case XDS_SAP_PACKAGE:
            switch (OMP_CLASS(class_type)) {
            case XDS_SIGNATURE:
				if (SIGNATURE(OMP_CLASS(subject_type)))
					*flag = OM_TRUE;
				break;
            case XDS_CERTIFICATE:
            case XDS_CERTIFICATE_LIST:
            case XDS_CERTIFICATE_PAIR:
            case XDS_CERTIFICATE_SUBLIST:
            case XDS_ALGORITHM_IDENT:
                break;

            default:
                return(OM_NO_SUCH_CLASS);
            } /* endswitch */
            break;
        case XMH_PACKAGE:
            switch (OMP_CLASS(class_type)) {
            case XMH_G3_FAX_NBPS:
                break;
            case XMH_TELETEX_NBPS:
                break;
			CASE_INSTANCE_MDUP_PACKAGE_CLOSURE_OBJECT  
#if 0
            case XMH_OR_ADDRESS:
                if (OR_ADDRESS(OMP_CLASS(subject_type)))
                    *flag =     OM_TRUE;
                break;
            case XMH_OR_NAME:
                break;
#endif /* 0 */
            default:
                return(OM_NO_SUCH_CLASS);
            } /* endswitch */
            break;

		CASE_INSTANCE_MDUP_PACKAGE 
#if 0
        case XDS_MDUP:
            switch (OMP_CLASS(class_type)) {
            case MHS_DLSP:
                break;

            default:
                return(OM_NO_SUCH_CLASS);
            } /* endswitch */
            break;
#endif /* 0 */

        default:
            return(OM_NO_SUCH_OBJECT);
        } /* endswitch */
        break;

#if 0
    case XOM_PACKAGE:
        return(OM_FUNCTION_DECLINED);
#endif /* 0 */

    default:
        return(OM_NO_SUCH_OBJECT);
    } /* endswitch */
end:
    return(ret);
} /* end dsP_instance_switch */
