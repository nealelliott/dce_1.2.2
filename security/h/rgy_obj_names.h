/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rgy_obj_names.h,v $
 * Revision 1.1.9.1  1996/05/10  13:15:02  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/27  17:45 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/hanfei_dce1.2_b1/1  1996/02/02  18:33 UTC  hanfei
 * 	multi-cell group work: add defines for SEC_FOREIGN_PRINCIPAL_OBJ_NAME
 * 	[1996/05/09  20:48:56  arvind]
 *
 * Revision 1.1.4.2  1994/08/04  16:12:44  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:09:23  mdf]
 * 
 * Revision 1.1.4.1  1994/03/02  17:54:48  hanfei
 * 	Initial ERA drop
 * 	[1994/02/18  19:08:46  hanfei]
 * 
 * Revision 1.1.2.3  1992/12/29  13:07:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:43:26  zeliff]
 * 
 * Revision 1.1.2.2  1992/08/31  18:16:42  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  16:01:46  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:42:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rgy_obj_names.h
**
** Copyright (c) Hewlett-Packard Company 1992
** Unpublished work. All Rights Reserved.
**
*/
/* 
**   Registry Object Names
*/

#ifndef _rgy_obj_names_h__included_
#define _rgy_obj_names_h__included_
        
/* String constants for registry object names */
#define SEC_POLICY_OBJ_NAME         "policy"
#define SEC_POLICY_OBJ_NAME_UC      "Policy"
#define SEC_PRINCIPAL_OBJ_NAME      "principal"
#define SEC_PRINCIPAL_OBJ_NAME_UC   "Principal"
#define SEC_GROUP_OBJ_NAME          "group"
#define SEC_GROUP_OBJ_NAME_UC       "Group"
#define SEC_ORG_OBJ_NAME            "org"
#define SEC_ORG_OBJ_NAME_UC         "Org"
#define SEC_DIR_OBJ_NAME            "directory"
#define SEC_DIR_OBJ_NAME_UC         "Directory"
#define SEC_REPLIST_OBJ_NAME        "replist"
#define SEC_REPLIST_OBJ_NAME_UC     "Replist"
#define SEC_ATTR_SCHEMA_OBJ_NAME        "xattrschema"
#define SEC_ATTR_SCHEMA_OBJ_NAME_UC     "Xattrschema"
#define SEC_FOREIGN_PRINCIPAL_OBJ_NAME      "foreign_principal"
#define SEC_FOREIGN_PRINCIPAL_OBJ_NAME_UC   "Foreign_Principal"





#endif  /*  _rgy_obj_names_h__included_  */
