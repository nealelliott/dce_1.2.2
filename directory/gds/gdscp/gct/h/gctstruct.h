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
 * $Log: gctstruct.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:59  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:58:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:50  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:04  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:03  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:29  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:58:15  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:17:23  keutel
 * 	creation
 * 	[1994/03/21  12:55:51  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program				   */
/* File Name   : gctstruct.h					   */
/* Description : This file contains all the data type definitions  */
/*		 used by the functions provided by the GDSCP	   */
/*		 Translator.					   */
/* Date        : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _GCTSTRUCT_H
#define _GCTSTRUCT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gctstruct_rcsid[] = "@(#)$RCSfile: gctstruct.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:13 $";
#endif

/*-- Type definitions ------------------------------------------*/
/* To store information about the type of operation. */
typedef enum _gc_t_op_type
{
    GC_T_BIND      =  0,  /* To specify bind operation      		 */
    GC_T_COMPARE   =  1,  /* To specify compare operation   		 */
    GC_T_CREATE	   =  2,  /* To specify create operation    		 */
    GC_T_LIST	   =  3,  /* To specify list operation	    		 */
    GC_T_MODIFY	   =  4,  /* To specify modify operation    		 */
    GC_T_MOVETO	   =  5,  /* To specify moveto operation    		 */
    GC_T_READ	   =  6,  /* To specify read operation	    		 */
    GC_T_REMOVE	   =  7,  /* To specify remove operation    		 */
    GC_T_SEARCH	   =  8,  /* To specify search operation    		 */
    GC_T_MOD_DSC   =  9,  /* To specify modify service control operation */
    GC_T_SHOW_DSC  = 10,  /* To specify show service control operation   */
    GC_T_SHOW_ABBR = 11   /* To specify show abbreviations operation     */
} gc_t_op_type;

/*-- Defines ------------------------------------------------------*/
#define GC_T_MAX_ERR_STRING	 256  /* Maximum size of the error string    */
#define GC_T_MAX_FILE_NAME      1024  /* Maximum length of XOI schema file   */
#define GC_T_MAX_DN_STRING	1024  /* Maximum size of the DN string       */
#define GC_T_MAX_AINFO_STRING  20480  /* Maximum size of Attr.Info string    */
#define GC_T_MAX_ATTR_NAME	  64  /* Maximum size of Attribute Full Name */
#define GC_T_MAX_OBJID	          64  /* Maximum size of Obj.Id.string       */
#define GC_T_MAX_ABBRV            16  /* Maximum size of Abbreviations       */
#define GC_T_MAX_VAR              16  /* Maximum size for other variables    */

/* Defines for the various switches */
#define GC_T_ADD_ATTR_SWITCH	     		"-addattr"
#define GC_T_REQUEST_ALL_SWITCH	     		"-allattr"
#define GC_T_ATTR_INFO_SWITCH	     		"-attribute"
#define GC_T_AUTHENTICATION_SWITCH 		"-authentication"
#define GC_T_BASE_OBJ_SWITCH	     		"-baseobject"
#define GC_T_CACHE_SWITCH	     		"-cache"
#define GC_T_MOD_ATTR_SWITCH	     		"-changeattr"
#define GC_T_DIRID_SWITCH	     		"-dirid"
#define GC_T_DSA_SWITCH		     		"-dsa"
#define GC_T_FILTER_SWITCH	     		"-filter"
#define GC_T_NOALIASES_SWITCH	     		"-noaliases"
#define GC_T_SING_LEVEL_SWITCH	     		"-onelevel"
#define GC_T_PASSWD_SWITCH	     		"-password"
#define GC_T_STRUCTURED_RESULT_SWITCH  		"-pretty"
#define GC_T_PSAP_SWITCH	     		"-psap"
#define GC_T_MOD_RDN_SWITCH	     		"-rdn"
#define GC_T_DEL_ATTR_SWITCH	     		"-removeattr"
#define GC_T_SUBTREE_SWITCH	     		"-subtree"
#define GC_T_ATTR_TYPE_SWITCH	     		"-types"
#define GC_T_USER_NAME_SWITCH	     		"-user"

/*--- Defines for the switches used for the service controls ---------*/
#define GC_T_AUTOMATIC_CONTINUATION_SWITCH   	"-automaticcontinuation"
#define GC_T_CACHE_CLASS_SWITCH         	"-cacheclass"
#define GC_T_CHAINING_PROHIBITED_SWITCH  	"-chainingprohibited"
#define GC_T_DEFAULT_SWITCH          		"-default"
#define GC_T_DONT_DEREF_ALIAS_SWITCH 		"-dontdereferencealias"
#define GC_T_DONT_USE_COPY_SWITCH    		"-dontusecopy"
#define GC_T_USE_CACHE_SWITCH        		"-duacache"
#define GC_T_CACHE_FIRST_SWITCH      		"-duafirst"
#define GC_T_LOCAL_SCOPE_SWITCH      		"-localscope"
#define GC_T_PREFER_ADMIN_FUNCTIONS_SWITCH     	"-preferadmfunctions"
#define GC_T_PREF_CHAIN_SWITCH       		"-preferchain"
#define GC_T_PRIORITY_SWITCH     		"-priority"
#define GC_T_SCOPE_OF_REFERRAL_SWITCH     	"-scopeofreferral"
#define GC_T_SIZE_LIMIT_SWITCH       		"-sizelimit"
#define GC_T_TIME_LIMIT_SWITCH       		"-timelimit"
#define GC_T_USE_DSA_SWITCH          		"-usedsa"

#define GC_T_TRUE_STR   			"TRUE"
#define GC_T_FALSE_STR  			"FALSE"
#define GC_T_RESIDENT_STR			"RESIDENT"
#define GC_T_PRIVILEGE_STR			"PRIVILEGE"
#define GC_T_NORMAL_STR				"NORMAL"
#define GC_T_COUNTRY_STR			"COUNTRY"
#define GC_T_DMD_STR				"DMD"
#define GC_T_UNLIMITED_STR			"UNLIMITED"
#define GC_T_LOW_STR				"LOW"
#define GC_T_MEDIUM_STR				"MEDIUM"
#define GC_T_HIGH_STR	 			"HIGH"
#define GC_T_INFINITE_STR			"INFINITE"
#define GC_T_NONE_STR				"NONE"
#define GC_T_SIMPLE_STR				"SIMPLE"
#define GC_T_STRONG_STR				"STRONG"
#define GC_T_DCE_STR				"DCE"

#define GC_T_RDN_DELIMITER_OP  '/'  /* Delimiter between RDNs                */
#define GC_T_RDN_DELIMITER_STR "/"  /* RDN Delimiter string                  */
#define GC_T_ROOT_DN 	       "/"  /* Root DN				     */
#define GC_T_QUOTE_OP  	       '\'' /* Quote character               	     */
#define GC_T_ESCAPE_OP         '\\' /* Escape character               	     */
#define GC_T_HEX_OP            'x'  /* To specify hex values 		     */
#define GC_T_ATTR_EQUALITY_OP  '='  /* Equality operator for Attributes      */

#endif  /* ifndef _GCTSTRUCT_H */
