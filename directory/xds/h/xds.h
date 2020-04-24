/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xds.h,v $
 * Revision 1.1.103.2  1996/02/18  23:37:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:13  marty]
 *
 * Revision 1.1.103.1  1995/12/08  15:35:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:15  root]
 * 
 * Revision 1.1.101.1  1994/02/22  19:23:43  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:10:45  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:01:41  marrek]
 * 
 * Revision 1.1.99.1  1993/10/14  19:19:53  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:18:18  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  10:02:31  keutel]
 * 
 * Revision 1.1.8.2  1993/08/10  07:09:29  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:19:07  marrek]
 * 
 * Revision 1.1.6.3  1992/12/30  23:30:01  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:20:15  bbelch]
 * 
 * Revision 1.1.6.2  1992/11/30  08:10:55  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:47:06  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:15:53  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:42:58  melman]
 * 
 * $EndLog$
 */
#ifndef _XDS_H
#define _XDS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xds_rcsid[] = "@(#)$RCSfile: xds.h,v $ $Revision: 1.1.103.2 $ $Date: 1996/02/18 23:37:46 $";
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
/* NAME         : xds.h                                               */
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
/* DESCRIPTION  : xds.h as specified in XDS                           */
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

#ifndef XDS_HEADER
#define XDS_HEADER


/* DS package object identifier */

/* { iso(1) identifier-organization(3) icd-ecma(12) member-company(2) dec(1011) 
     xopen(28) dsp(0) } */

#define OMP_O_DS_SERVICE_PKG            "\x2B\x0C\x02\x87\x73\x1C\x00"


/* Typedefs */

typedef OM_private_object  DS_status;

typedef struct {
         OM_object_identifier   feature;
         OM_boolean             activated;
} DS_feature;


/* Function Prototypes */

DS_status ds_abandon(
         OM_private_object      session,
         OM_sint                invoke_id
);

DS_status ds_add_entry(
         OM_private_object      session,
         OM_private_object      context,
         OM_object              name,
         OM_object              entry,
         OM_sint                *invoke_id_return
);
 
DS_status ds_bind(
         OM_object              session,
         OM_workspace           workspace,
         OM_private_object      *bound_session_return
);
 
DS_status ds_compare(
         OM_private_object      session,
         OM_private_object      context,
         OM_object              name,
         OM_object              ava, 
         OM_private_object      *result_return,
         OM_sint                *invoke_id_return
);

OM_workspace ds_initialize(
         void
);
 
DS_status ds_list(
         OM_private_object      session,
         OM_private_object      context,
         OM_object              name,
         OM_private_object      *result_return,
         OM_sint                *invoke_id_return
);
 
DS_status ds_modify_entry(
         OM_private_object      session,
         OM_private_object      context,
         OM_object              name,
         OM_object              changes,
         OM_sint                *invoke_id_return
);
 
DS_status ds_modify_rdn(
         OM_private_object      session,
         OM_private_object      context,
         OM_object              name,
         OM_object              new_RDN,
         OM_boolean             delete_old_RDN,
         OM_sint                *invoke_id_return
);
 
DS_status ds_read(
         OM_private_object      session,
         OM_private_object      context,
         OM_object              name,
         OM_object              selection,
         OM_private_object      *result_return,
         OM_sint                *invoke_id_return
);

DS_status ds_receive_result(
         OM_private_object      session,
         OM_uint                *completion_flag_return,
         DS_status              *operation_status_return,
         OM_private_object      *result_return,
         OM_sint                *invoke_id_return
);

DS_status ds_remove_entry(
         OM_private_object      session,
         OM_private_object      context,
         OM_object              name,
         OM_sint                *invoke_id_return
);
 
DS_status ds_search(
         OM_private_object      session,
         OM_private_object      context,
         OM_object              name,
         OM_sint                subset,
         OM_object              filter,
         OM_boolean             search_aliases,
         OM_object              selection,
         OM_private_object      *result_return,
         OM_sint                *invoke_id_return
);
 
DS_status ds_shutdown(
         OM_workspace           workspace
);
 
DS_status ds_unbind(
         OM_private_object      session
);
 
DS_status ds_version(
         DS_feature             feature_list[],
         OM_workspace           workspace
);


/* Defined constants    */

/* Intermediate object identifier macro */

#define dsP_c(X)     (OMP_O_DS_SERVICE_PKG #X)

/* OM class names (prefixed DS_C_)      */

/* Every application program which makes use of a class or      */
/* other Object Identifier must explicitly import it into       */
/* every compilation unit (C source program) which uses it.     */
/* Each such class or Object Identifier name must be            */
/* explicitly exported from just one compilation unit.          */

/* In the header file, OM class constants are prefixed with     */
/* the OMP_O prefix to denote that they are OM classes.         */
/* However, when using the OM_IMPORT and OM_EXPORT macros,      */
/* the base names (without the OMP_O prefix) should be used.    */
/* For example:                                                 */
/*        OM_IMPORT(DS_C_AVA)                                   */

#define OMP_O_DS_C_ABANDON_FAILED               dsP_c(\x85\x3D)
#define OMP_O_DS_C_ACCESS_POINT                 dsP_c(\x85\x3E)
#define OMP_O_DS_C_ADDRESS                      dsP_c(\x85\x3F)
#define OMP_O_DS_C_ATTRIBUTE                    dsP_c(\x85\x40)
#define OMP_O_DS_C_ATTRIBUTE_ERROR              dsP_c(\x85\x41)
#define OMP_O_DS_C_ATTRIBUTE_LIST               dsP_c(\x85\x42)
#define OMP_O_DS_C_ATTRIBUTE_PROBLEM            dsP_c(\x85\x43)
#define OMP_O_DS_C_AVA                          dsP_c(\x85\x44)
#define OMP_O_DS_C_COMMON_RESULTS               dsP_c(\x85\x45)
#define OMP_O_DS_C_COMMUNICATIONS_ERROR         dsP_c(\x85\x46)
#define OMP_O_DS_C_COMPARE_RESULT               dsP_c(\x85\x47)
#define OMP_O_DS_C_CONTEXT                      dsP_c(\x85\x48)
#define OMP_O_DS_C_CONTINUATION_REF             dsP_c(\x85\x49)
#define OMP_O_DS_C_DS_DN                        dsP_c(\x85\x4A)
#define OMP_O_DS_C_DS_RDN                       dsP_c(\x85\x4B)
#define OMP_O_DS_C_ENTRY_INFO                   dsP_c(\x85\x4C)
#define OMP_O_DS_C_ENTRY_INFO_SELECTION         dsP_c(\x85\x4D)
#define OMP_O_DS_C_ENTRY_MOD                    dsP_c(\x85\x4E)
#define OMP_O_DS_C_ENTRY_MOD_LIST               dsP_c(\x85\x4F)
#define OMP_O_DS_C_ERROR                        dsP_c(\x85\x50)
#define OMP_O_DS_C_EXT                          dsP_c(\x85\x51)
#define OMP_O_DS_C_FILTER                       dsP_c(\x85\x52)
#define OMP_O_DS_C_FILTER_ITEM                  dsP_c(\x85\x53)
#define OMP_O_DS_C_LIBRARY_ERROR                dsP_c(\x85\x54)
#define OMP_O_DS_C_LIST_INFO                    dsP_c(\x85\x55)
#define OMP_O_DS_C_LIST_INFO_ITEM               dsP_c(\x85\x56)
#define OMP_O_DS_C_LIST_RESULT                  dsP_c(\x85\x57)
#define OMP_O_DS_C_NAME                         dsP_c(\x85\x58)
#define OMP_O_DS_C_NAME_ERROR                   dsP_c(\x85\x59)
#define OMP_O_DS_C_OPERATION_PROGRESS           dsP_c(\x85\x5A)
#define OMP_O_DS_C_PARTIAL_OUTCOME_QUAL         dsP_c(\x85\x5B)
#define OMP_O_DS_C_PRESENTATION_ADDRESS         dsP_c(\x85\x5C)
#define OMP_O_DS_C_READ_RESULT                  dsP_c(\x85\x5D)
#define OMP_O_DS_C_REFERRAL                     dsP_c(\x85\x5E)
#define OMP_O_DS_C_RELATIVE_NAME                dsP_c(\x85\x5F)
#define OMP_O_DS_C_SEARCH_INFO                  dsP_c(\x85\x60)
#define OMP_O_DS_C_SEARCH_RESULT                dsP_c(\x85\x61)
#define OMP_O_DS_C_SECURITY_ERROR               dsP_c(\x85\x62)
#define OMP_O_DS_C_SERVICE_ERROR                dsP_c(\x85\x63)
#define OMP_O_DS_C_SESSION                      dsP_c(\x85\x64)
#define OMP_O_DS_C_SYSTEM_ERROR                 dsP_c(\x85\x65)
#define OMP_O_DS_C_UPDATE_ERROR                 dsP_c(\x85\x66)
 

/* OM attribute names */

#define DS_ACCESS_POINTS                        ((OM_type) 701)
#define DS_ADDRESS                              ((OM_type) 702)
#define DS_AE_TITLE                             ((OM_type) 703)
#define DS_ALIASED_RDNS                         ((OM_type) 704)
#define DS_ALIAS_DEREFERENCED                   ((OM_type) 705)
#define DS_ALIAS_ENTRY                          ((OM_type) 706)
#define DS_ALL_ATTRIBUTES                       ((OM_type) 707)
#define DS_ASYNCHRONOUS                         ((OM_type) 708)
#define DS_ATTRIBUTES                           ((OM_type) 709)
#define DS_ATTRIBUTES_SELECTED                  ((OM_type) 710)
#define DS_ATTRIBUTE_TYPE                       ((OM_type) 711)
#define DS_ATTRIBUTE_VALUE                      ((OM_type) 712)
#define DS_ATTRIBUTE_VALUES                     ((OM_type) 713)
#define DS_AUTOMATIC_CONTINUATION               ((OM_type) 714)
#define DS_AVAS                                 ((OM_type) 715)
#define DS_CHAINING_PROHIB                      ((OM_type) 716)
#define DS_CHANGES                              ((OM_type) 717)
#define DS_CRIT                                 ((OM_type) 718)
#define DS_DONT_DEREFERENCE_ALIASES             ((OM_type) 719)
#define DS_DONT_USE_COPY                        ((OM_type) 720)
#define DS_DSA_ADDRESS                          ((OM_type) 721)
#define DS_DSA_NAME                             ((OM_type) 722)
#define DS_ENTRIES                              ((OM_type) 723)
#define DS_ENTRY                                ((OM_type) 724)
#define DS_EXT                                  ((OM_type) 725)
#define DS_FILE_DESCRIPTOR                      ((OM_type) 726)
#define DS_FILTERS                              ((OM_type) 727)
#define DS_FILTER_ITEMS                         ((OM_type) 728)
#define DS_FILTER_ITEM_TYPE                     ((OM_type) 729)
#define DS_FILTER_TYPE                          ((OM_type) 730)
#define DS_FINAL_SUBSTRING                      ((OM_type) 731)
#define DS_FROM_ENTRY                           ((OM_type) 732)
#define DS_IDENT                                ((OM_type) 733)
#define DS_INFO_TYPE                            ((OM_type) 734)
#define DS_INITIAL_SUBSTRING                    ((OM_type) 735)
#define DS_ITEM_PARAMETERS                      ((OM_type) 736)
#define DS_LIMIT_PROBLEM                        ((OM_type) 737)
#define DS_LIST_INFO                            ((OM_type) 738)
#define DS_LOCAL_SCOPE                          ((OM_type) 739)
#define DS_MATCHED                              ((OM_type) 740)
#define DS_MOD_TYPE                             ((OM_type) 741)
#define DS_NAME_RESOLUTION_PHASE                ((OM_type) 742)
#define DS_NEXT_RDN_TO_BE_RESOLVED              ((OM_type) 743)
#define DS_N_ADDRESSES                          ((OM_type) 744)
#define DS_OBJECT_NAME                          ((OM_type) 745)
#define DS_OPERATION_PROGRESS                   ((OM_type) 746)
#define DS_PARTIAL_OUTCOME_QUAL                 ((OM_type) 747)
#define DS_PERFORMER                            ((OM_type) 748)
#define DS_PREFER_CHAINING                      ((OM_type) 749)
#define DS_PRIORITY                             ((OM_type) 750)
#define DS_PROBLEM                              ((OM_type) 751)
#define DS_PROBLEMS                             ((OM_type) 752)
#define DS_P_SELECTOR                           ((OM_type) 753)
#define DS_RDN                                  ((OM_type) 754)
#define DS_RDNS                                 ((OM_type) 755)
#define DS_RDNS_RESOLVED                        ((OM_type) 756)
#define DS_REQUESTOR                            ((OM_type) 757)
#define DS_SCOPE_OF_REFERRAL                    ((OM_type) 758)
#define DS_SEARCH_INFO                          ((OM_type) 759)
#define DS_SIZE_LIMIT                           ((OM_type) 760)
#define DS_SUBORDINATES                         ((OM_type) 761)
#define DS_S_SELECTOR                           ((OM_type) 762)
#define DS_TARGET_OBJECT                        ((OM_type) 763)
#define DS_TIME_LIMIT                           ((OM_type) 764)
#define DS_T_SELECTOR                           ((OM_type) 765)
#define DS_UNAVAILABLE_CRIT_EXT                 ((OM_type) 766)
#define DS_UNCORRELATED_LIST_INFO               ((OM_type) 767)
#define DS_UNCORRELATED_SEARCH_INFO             ((OM_type) 768)
#define DS_UNEXPLORED                           ((OM_type) 769)


/* DS_Filter_Item_Type */

enum DS_Filter_Item_Type {
        DS_EQUALITY             =  0,
        DS_SUBSTRINGS           =  1,
        DS_GREATER_OR_EQUAL     =  2,
        DS_LESS_OR_EQUAL        =  3,
        DS_PRESENT              =  4,
        DS_APPROXIMATE_MATCH    =  5
};

/* DS_Filter_Type */

enum DS_Filter_Type {
        DS_ITEM =  0,
        DS_AND  =  1,
        DS_OR   =  2,
        DS_NOT  =  3
};

/* DS_Information_Type */

enum DS_Information_Type {
        DS_TYPES_ONLY           =  0,
        DS_TYPES_AND_VALUES     =  1
};

/* DS_Limit_Problem */

enum DS_Limit_Problem {
        DS_NO_LIMIT_EXCEEDED    =  -1,
        DS_TIME_LIMIT_EXCEEDED  =  0,
        DS_SIZE_LIMIT_EXCEEDED  =  1,
        DS_ADMIN_LIMIT_EXCEEDED =  2
};

/* DS_Modification_Type */

enum DS_Modification_Type {
        DS_ADD_ATTRIBUTE        =  0,
        DS_REMOVE_ATTRIBUTE     =  1,
        DS_ADD_VALUES           =  2,
        DS_REMOVE_VALUES        =  3
};

/* DS_Name_Resolution_Phase */

enum DS_Name_Resolution_Phase {
        DS_NOT_STARTED  =  1,
        DS_PROCEEDING   =  2,
        DS_COMPLETED    =  3
};

/* DS_Priority */

enum DS_Priority {
        DS_LOW          =  0,
        DS_MEDIUM       =  1,
        DS_HIGH         =  2
};

/* DS_Problem */

enum DS_Problem {
        DS_E_ADMIN_LIMIT_EXCEEDED               =  1,
        DS_E_AFFECTS_MULTIPLE_DSAS              =  2,
        DS_E_ALIAS_DEREFERENCING_PROBLEM        =  3,
        DS_E_ALIAS_PROBLEM                      =  4,
        DS_E_ATTRIBUTE_OR_VALUE_EXISTS          =  5,
        DS_E_BAD_ARGUMENT                       =  6,
        DS_E_BAD_CLASS                          =  7,
        DS_E_BAD_CONTEXT                        =  8,
        DS_E_BAD_NAME                           =  9,
        DS_E_BAD_SESSION                        = 10,
        DS_E_BAD_WORKSPACE                      = 11,
        DS_E_BUSY                               = 12,
        DS_E_CANNOT_ABANDON                     = 13,
        DS_E_CHAINING_REQUIRED                  = 14,
        DS_E_COMMUNICATIONS_PROBLEM             = 15,
        DS_E_CONSTRAINT_VIOLATION               = 16,
        DS_E_DIT_ERROR                          = 17,
        DS_E_ENTRY_EXISTS                       = 18,
        DS_E_INAPPROP_AUTHENTICATION            = 19,
        DS_E_INAPPROP_MATCHING                  = 20,
        DS_E_INSUFFICIENT_ACCESS_RIGHTS         = 21,
        DS_E_INVALID_ATTRIBUTE_SYNTAX           = 22,
        DS_E_INVALID_ATTRIBUTE_VALUE            = 23,
        DS_E_INVALID_CREDENTIALS                = 24,
        DS_E_INVALID_REF                        = 25,
        DS_E_INVALID_SIGNATURE                  = 26,
        DS_E_LOOP_DETECTED                      = 27,
        DS_E_MISCELLANEOUS                      = 28,
        DS_E_MISSING_TYPE                       = 29,
        DS_E_MIXED_SYNCHRONOUS                  = 30,
        DS_E_NAMING_VIOLATION                   = 31,
        DS_E_NO_INFO                            = 32,
        DS_E_NO_SUCH_ATTRIBUTE_OR_VALUE         = 33,
        DS_E_NO_SUCH_OBJECT                     = 34,
        DS_E_NO_SUCH_OPERATION                  = 35,
        DS_E_NOT_ALLOWED_ON_NON_LEAF            = 36,
        DS_E_NOT_ALLOWED_ON_RDN                 = 37,
        DS_E_NOT_SUPPORTED                      = 38,
        DS_E_OBJECT_CLASS_MOD_PROHIB            = 39,
        DS_E_OBJECT_CLASS_VIOLATION             = 40,
        DS_E_OUT_OF_SCOPE                       = 41,
        DS_E_PROTECTION_REQUIRED                = 42,
        DS_E_TIME_LIMIT_EXCEEDED                = 43,
        DS_E_TOO_LATE                           = 44,
        DS_E_TOO_MANY_OPERATIONS                = 45,
        DS_E_TOO_MANY_SESSIONS                  = 46,
        DS_E_UNABLE_TO_PROCEED                  = 47,
        DS_E_UNAVAILABLE                        = 48,
        DS_E_UNAVAILABLE_CRIT_EXT               = 49,
        DS_E_UNDEFINED_ATTRIBUTE_TYPE           = 50,
        DS_E_UNWILLING_TO_PERFORM               = 51
};

/* DS_Scope_Of_Referral */

enum DS_Scope_Of_Referral {
        DS_DMD          =  0,
        DS_COUNTRY      =  1
};

/* OM_object constants */

#define DS_DEFAULT_CONTEXT              ((OM_object) 0)
#define DS_DEFAULT_SESSION              ((OM_object) 0)
#define DS_OPERATION_NOT_STARTED        ((OM_object) 0)
#define DS_NO_FILTER                    ((OM_object) 0)
#define DS_NULL_RESULT                  ((OM_object) 0)
#define DS_SELECT_ALL_TYPES             ((OM_object) 1)
#define DS_SELECT_ALL_TYPES_AND_VALUES  ((OM_object) 2)
#define DS_SELECT_NO_ATTRIBUTES         ((OM_object) 0)
#define DS_SUCCESS                      ((DS_status) 0)
#define DS_NO_WORKSPACE                 ((DS_status) 1)
#define DS_INVALID_WORKSPACE            ((DS_status) 2)

/* ds_search subset */

#define DS_BASE_OBJECT          ( (OM_sint) 0)
#define DS_ONE_LEVEL            ( (OM_sint) 1)
#define DS_WHOLE_SUBTREE        ( (OM_sint) 2)

/* ds_receive_result completion_flag_return */

#define DS_COMPLETED_OPERATION          ( (OM_uint) 1)
#define DS_OUTSTANDING_OPERATIONS       ( (OM_uint) 2)
#define DS_NO_OUTSTANDING_OPERATION     ( (OM_uint) 3)

/* asynchronous operations limit (no asynchronous operations) */

#define DS_MAX_OUTSTANDING_OPERATIONS   0

/* asynchronous event posting */

#define DS_NO_VALID_FILE_DESCRIPTOR     -1


#endif  /* XDS_HEADER */

#endif /* _XDS_H */
