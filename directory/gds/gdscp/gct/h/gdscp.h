/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: gdscp.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:03  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:58:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:53  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:05  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:05  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:34  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:58:21  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:17:25  keutel
 * 	creation
 * 	[1994/03/21  12:55:58  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program				   */
/* File Name   : gdscp.h					   */
/* Description : This file contains all the error defines returned */
/*		 by gdscp.					   */
/* Date        : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _GDSCP_H
#define _GDSCP_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gdscp_rcsid[] = "@(#)$RCSfile: gdscp.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:15 $";
#endif

#define GC_SUCCESS    	   		     	     ((signed16)  0)

/*-- Defines for error returns ------------------------------*/
/* Defines for errors returned by XDS (defined here between -1 to -200) */
#define GC_T_DS_E_ADMIN_LIMIT_EXCEEDED               ((signed16) -1)
#define GC_T_DS_E_AFFECTS_MULTIPLE_DSAS              ((signed16) -2)
#define GC_T_DS_E_ALIAS_DEREFERENCING_PROBLEM        ((signed16) -3)
#define GC_T_DS_E_ALIAS_PROBLEM                      ((signed16) -4)
#define GC_T_DS_E_ATTRIBUTE_OR_VALUE_EXISTS          ((signed16) -5)
#define GC_T_DS_E_BAD_ARGUMENT                       ((signed16) -6)
#define GC_T_DS_E_BAD_CLASS                          ((signed16) -7)
#define GC_T_DS_E_BAD_CONTEXT                        ((signed16) -8)
#define GC_T_DS_E_BAD_NAME                           ((signed16) -9)
#define GC_T_DS_E_BAD_SESSION                        ((signed16) -10)
#define GC_T_DS_E_BAD_WORKSPACE                      ((signed16) -11)
#define GC_T_DS_E_BUSY                               ((signed16) -12)
#define GC_T_DS_E_CANNOT_ABANDON                     ((signed16) -13)
#define GC_T_DS_E_CHAINING_REQUIRED                  ((signed16) -14)
#define GC_T_DS_E_COMMUNICATIONS_PROBLEM             ((signed16) -15)
#define GC_T_DS_E_CONSTRAINT_VIOLATION               ((signed16) -16)
#define GC_T_DS_E_DIT_ERROR                          ((signed16) -17)
#define GC_T_DS_E_ENTRY_EXISTS                       ((signed16) -18)
#define GC_T_DS_E_INAPPROP_AUTHENTICATION            ((signed16) -19)
#define GC_T_DS_E_INAPPROP_MATCHING                  ((signed16) -20)
#define GC_T_DS_E_INSUFFICIENT_ACCESS_RIGHTS         ((signed16) -21)
#define GC_T_DS_E_INVALID_ATTRIBUTE_SYNTAX           ((signed16) -22)
#define GC_T_DS_E_INVALID_ATTRIBUTE_VALUE            ((signed16) -23)
#define GC_T_DS_E_INVALID_CREDENTIALS                ((signed16) -24)
#define GC_T_DS_E_INVALID_REF                        ((signed16) -25)
#define GC_T_DS_E_INVALID_SIGNATURE                  ((signed16) -26)
#define GC_T_DS_E_LOOP_DETECTED                      ((signed16) -27)
#define GC_T_DS_E_MISCELLANEOUS                      ((signed16) -28)
#define GC_T_DS_E_MISSING_TYPE                       ((signed16) -29)
#define GC_T_DS_E_MIXED_SYNCHRONOUS                  ((signed16) -30)
#define GC_T_DS_E_NAMING_VIOLATION                   ((signed16) -31)
#define GC_T_DS_E_NO_INFO                            ((signed16) -32)
#define GC_T_DS_E_NO_SUCH_ATTRIBUTE_OR_VALUE         ((signed16) -33)
#define GC_T_DS_E_NO_SUCH_OBJECT                     ((signed16) -34)
#define GC_T_DS_E_NO_SUCH_OPERATION                  ((signed16) -35)
#define GC_T_DS_E_NOT_ALLOWED_ON_NON_LEAF            ((signed16) -36)
#define GC_T_DS_E_NOT_ALLOWED_ON_RDN                 ((signed16) -37)
#define GC_T_DS_E_NOT_SUPPORTED                      ((signed16) -38)
#define GC_T_DS_E_OBJECT_CLASS_MOD_PROHIB            ((signed16) -39)
#define GC_T_DS_E_OBJECT_CLASS_VIOLATION             ((signed16) -40)
#define GC_T_DS_E_OUT_OF_SCOPE                       ((signed16) -41)
#define GC_T_DS_E_PROTECTION_REQUIRED                ((signed16) -42)
#define GC_T_DS_E_TIME_LIMIT_EXCEEDED                ((signed16) -43)
#define GC_T_DS_E_TOO_LATE                           ((signed16) -44)
#define GC_T_DS_E_TOO_MANY_OPERATIONS                ((signed16) -45)
#define GC_T_DS_E_TOO_MANY_SESSIONS                  ((signed16) -46)
#define GC_T_DS_E_UNABLE_TO_PROCEED                  ((signed16) -47)
#define GC_T_DS_E_UNAVAILABLE                        ((signed16) -48)
#define GC_T_DS_E_UNAVAILABLE_CRIT_EXT               ((signed16) -49)
#define GC_T_DS_E_UNDEFINED_ATTRIBUTE_TYPE           ((signed16) -50)
#define GC_T_DS_E_UNWILLING_TO_PERFORM               ((signed16) -51)
#define GC_T_UNKNOWN_XDS_ERROR			     ((signed16) -52)

/* Defines for errors returned by XOM (defined here between -201 to -300) */
#define GC_T_OM_ENCODING_INVALID		     ((signed16) -201)
#define GC_T_OM_FUNCTION_DECLINED		     ((signed16) -202)
#define GC_T_OM_FUNCTION_INTERRUPTED		     ((signed16) -203)
#define GC_T_OM_MEMORY_INSUFFICIENT		     ((signed16) -204)
#define GC_T_OM_NETWORK_ERROR			     ((signed16) -205)
#define GC_T_OM_NO_SUCH_CLASS			     ((signed16) -206)
#define GC_T_OM_NO_SUCH_EXCLUSION		     ((signed16) -207)
#define GC_T_OM_NO_SUCH_MODIFICATION		     ((signed16) -208)
#define GC_T_OM_NO_SUCH_OBJECT			     ((signed16) -209)
#define GC_T_OM_NO_SUCH_RULES			     ((signed16) -210)
#define GC_T_OM_NO_SUCH_SYNTAX			     ((signed16) -211)
#define GC_T_OM_NO_SUCH_TYPE			     ((signed16) -212)
#define GC_T_OM_NO_SUCH_WORKSPACE		     ((signed16) -213)
#define GC_T_OM_NOT_AN_ENCODING			     ((signed16) -214)
#define GC_T_OM_NOT_CONCRETE			     ((signed16) -215)
#define GC_T_OM_NOT_PRESENT			     ((signed16) -216)
#define GC_T_OM_NOT_PRIVATE			     ((signed16) -217)
#define GC_T_OM_NOT_THE_SERVICES		     ((signed16) -218)
#define GC_T_OM_PERMANENT_ERROR			     ((signed16) -219)
#define GC_T_OM_POINTER_INVALID		     	     ((signed16) -220)
#define GC_T_OM_SYSTEM_ERROR			     ((signed16) -221)
#define GC_T_OM_TEMPORARY_ERROR			     ((signed16) -222)
#define GC_T_OM_TOO_MANY_VALUES			     ((signed16) -223)
#define GC_T_OM_VALUES_NOT_ADJACENT		     ((signed16) -224)
#define GC_T_OM_WRONG_VALUE_LENGTH		     ((signed16) -225)
#define GC_T_OM_WRONG_VALUE_MAKEUP		     ((signed16) -226)
#define GC_T_OM_WRONG_VALUE_NUMBER		     ((signed16) -227)
#define GC_T_OM_WRONG_VALUE_POSITION		     ((signed16) -228)
#define GC_T_OM_WRONG_VALUE_SYNTAX		     ((signed16) -229)
#define GC_T_OM_WRONG_VALUE_TYPE		     ((signed16) -230)	
#define GC_T_OMX_CLASS_NOT_FOUND_IN_SCHEMA_FILE	     ((signed16) -231)	
#define GC_T_UNKNOWN_XOM_ERROR			     ((signed16) -232)

/* Defines for errors returned by XOM Conv. library (between -301 to -500) */
#define GC_T_OMX_CANNOT_READ_SCHEMA     	     ((signed16) -301)
#define GC_T_OMX_SCHEMA_NOT_READ	   	     ((signed16) -302)
#define GC_T_OMX_NO_START_OBJ_BLOCK	   	     ((signed16) -303)
#define GC_T_OMX_NO_END_OBJ_BLOCK	   	     ((signed16) -304)
#define GC_T_OMX_EMPTY_OBJ_BLOCK	   	     ((signed16) -305)
#define GC_T_OMX_OBJ_FORMAT_ERROR	   	     ((signed16) -306)
#define GC_T_OMX_DUPLICATE_OBJ_ABBRV    	     ((signed16) -307)
#define GC_T_OMX_DUPLICATE_OBJ_OBJ_ID   	     ((signed16) -308)
#define GC_T_OMX_NO_START_ATTR_BLOCK	   	     ((signed16) -309)
#define GC_T_OMX_NO_END_ATTR_BLOCK	   	     ((signed16) -310)
#define GC_T_OMX_EMPTY_ATTR_BLOCK	   	     ((signed16) -311)
#define GC_T_OMX_ATTR_FORMAT_ERROR	   	     ((signed16) -312)
#define GC_T_OMX_DUPLICATE_ATTR_ABBRV   	     ((signed16) -313)
#define GC_T_OMX_DUPLICATE_ATTR_OBJ_ID  	     ((signed16) -314)
#define GC_T_OMX_NO_START_CLASS_BLOCK   	     ((signed16) -315)
#define GC_T_OMX_NO_END_CLASS_BLOCK	   	     ((signed16) -316)
#define GC_T_OMX_EMPTY_CLASS_BLOCK	   	     ((signed16) -317)
#define GC_T_OMX_CLASS_FORMAT_ERROR     	     ((signed16) -318)
#define GC_T_OMX_NO_CLASS_NAME          	     ((signed16) -319)
#define GC_T_OMX_DUPLICATE_CLASS_BLOCK  	     ((signed16) -320)
#define GC_T_OMX_CLASS_BLOCK_UNDEFINED  	     ((signed16) -321)
#define GC_T_OMX_INVALID_ABBRV          	     ((signed16) -322)
#define GC_T_OMX_INVALID_OBJ_ID	   	     	     ((signed16) -323)
#define GC_T_OMX_INVALID_CLASS_NAME     	     ((signed16) -324)
#define GC_T_OMX_INVALID_SYNTAX     	   	     ((signed16) -325)
#define GC_T_OMX_MEMORY_INSUFFICIENT    	     ((signed16) -326)
#define GC_T_OMX_INVALID_PARAMETER	   	     ((signed16) -327)
#define GC_T_OMX_UNKNOWN_ABBRV	   		     ((signed16) -328)
#define GC_T_OMX_UNKNOWN_OBJ_ID	   		     ((signed16) -329)
#define GC_T_OMX_UNKNOWN_OMTYPE	   		     ((signed16) -330)
#define GC_T_OMX_MISSING_AVA  	   		     ((signed16) -331)
#define GC_T_OMX_MISSING_ABBRV	   		     ((signed16) -332)
#define GC_T_OMX_FORMAT_ERROR 	   		     ((signed16) -333)
#define GC_T_OMX_UNKNOWN_ERROR	   		     ((signed16) -334)
#define GC_T_OMX_MISSING_RDN_DELIMITER  	     ((signed16) -335)
#define GC_T_OMX_MISMATCHED_QUOTES      	     ((signed16) -336)
#define GC_T_OMX_MISSING_EQUAL_OPERATOR 	     ((signed16) -337)
#define GC_T_OMX_MISSING_ATTR_VALUE     	     ((signed16) -338)
#define GC_T_OMX_MISSING_ATTR_INFO      	     ((signed16) -339)
#define GC_T_OMX_MISSING_CLASS_START_OP 	     ((signed16) -340)
#define GC_T_OMX_MISSING_CLASS_END_OP   	     ((signed16) -341)
#define GC_T_OMX_MISSING_CLASS_VALUE    	     ((signed16) -342)
#define GC_T_OMX_MISSING_COMP_VALUE     	     ((signed16) -343)
#define GC_T_OMX_MISMATCHED_BRACKETS    	     ((signed16) -344)
#define GC_T_OMX_UNEXPECTED_OPERATOR    	     ((signed16) -345)
#define GC_T_OMX_WRONG_VALUE		   	     ((signed16) -346)
#define GC_T_OMX_UNKNOWN_KEYWORD	   	     ((signed16) -347)
#define GC_T_OMX_MISSING_OPERATOR	   	     ((signed16) -348)
#define GC_T_OMX_MISSING_COMPOUND_OP    	     ((signed16) -349)
#define GC_T_UNKNOWN_CONV_LIB_ERROR		     ((signed16) -350)

/* Defines for errors returned by GDSCP Translator (between -501 to -700) */
#define GC_T_CONFLICTING_OPTION			     ((signed16) -501)
#define GC_T_UNKNOWN_OPTION		     	     ((signed16) -502)
#define GC_T_MISSING_DIRECTORY_ID		     ((signed16) -503)
#define GC_T_MISSING_OBJECT_NAME	     	     ((signed16) -504)
#define GC_T_MISSING_ATTRIBUTE_INFO		     ((signed16) -505)
#define GC_T_MISSING_NEW_ATTRIBUTE_INFO		     ((signed16) -506)
#define GC_T_MISSING_DSA_NAME		     	     ((signed16) -507)
#define GC_T_MISSING_PSAP_ADDRESS	     	     ((signed16) -508)
#define GC_T_MISSING_USER_NAME	     	     	     ((signed16) -509)
#define GC_T_MISSING_USER_PASSWORD	     	     ((signed16) -510)
#define GC_T_MISSING_FILTER	     	     	     ((signed16) -511)
#define GC_T_MISSING_SCOPE_OF_SEARCH   	     	     ((signed16) -512)
#define GC_T_MISSING_TIME_LIMIT   	     	     ((signed16) -513)
#define GC_T_MISSING_SIZE_LIMIT   	     	     ((signed16) -514)
#define GC_T_MISSING_ARGUMENTS   	     	     ((signed16) -515)
#define GC_T_INVALID_DIRECTORY_ID   	     	     ((signed16) -516)
#define GC_T_INVALID_TIME_LIMIT   	     	     ((signed16) -517)
#define GC_T_INVALID_SIZE_LIMIT   	     	     ((signed16) -518)
#define GC_T_UNKNOWN_ARGUMENT		     	     ((signed16) -519)
#define GC_T_CANNOT_INITIALIZE_WORKSPACE	     ((signed16) -520)
#define GC_T_BIND_NOT_MADE	     		     ((signed16) -521)
#define GC_T_MISSING_VALUE	     		     ((signed16) -522)
#define GC_T_FORMAT_ERROR	     		     ((signed16) -523)
#define GC_T_MISMATCHED_QUOTES	     		     ((signed16) -524)
#define GC_T_INVALID_PARAMETER		     	     ((signed16) -525)
#define GC_T_TOO_MANY_ARGUMENTS		     	     ((signed16) -526)
#define GC_T_ENTRY_NOT_FOUND		     	     ((signed16) -527)
#define GC_T_SUBORDINATES_NOT_FOUND		     ((signed16) -528)
#define GC_T_MEMORY_INSUFFICIENT		     ((signed16) -529)
#define GC_T_REFERRAL_RETURNED		     	     ((signed16) -530)
#define GC_T_OPERATION_INCOMPLETE	     	     ((signed16) -531)
#define GC_T_AMBIGUOUS_OPTION	     	     	     ((signed16) -532)
#define GC_T_TOO_MANY_VALUES	     	     	     ((signed16) -533)
#define GC_T_MISSING_AUTHENTICATION_TYPE	     ((signed16) -534)
#define GC_T_UNKNOWN_GCP_ERROR		     	     ((signed16) -535)

/* Defines for errors returned by GDSCP Interpreter (between -701 to -800) */
#define GC_I_UNKNOWN_OPTION                          ((signed16) -701)
#define GC_I_UNKNOWN_ARGUMENT                        ((signed16) -702)
#define GC_I_INVALID_ARGUMENT                        ((signed16) -703)
#define GC_I_INVALID_PARAMETER                       ((signed16) -704)
#define GC_I_UNKNOWN_OPERATION                       ((signed16) -705)
#define GC_I_MISSING_OPERATION                       ((signed16) -706)
#define GC_I_TOO_MANY_ARGUMENTS                      ((signed16) -707)
#define GC_I_AMBIGUOUS_OPERATION                     ((signed16) -708)
#define GC_I_CANNOT_UNSET_VAR                        ((signed16) -709)
#define GC_I_MEMORY_INSUFFICIENT                     ((signed16) -710)

#endif  /* ifndef _GDSCP_H */
