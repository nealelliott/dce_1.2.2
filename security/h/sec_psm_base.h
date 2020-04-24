/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_psm_base.h,v $
 * Revision 1.1.2.3  1996/11/13  18:01:24  arvind
 * 	Add/modify copyright info
 * 	[1996/11/12  17:43 UTC  arvind  /main/DCE_1.2.2/4]
 *
 * 	Add/modify copyright info
 * 	[1996/08/21  20:52 UTC  aha  /main/DCE_1.2.2/3]
 *
 * 	Merge initial portion of KDC pvtkey retrieval
 * 	[1996/08/14  12:52 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 *
 * 	Merge KDC pvtkey mechanism ERA support
 * 	[1996/08/02  13:02 UTC  aha  /main/DCE_1.2.2/aha_pk8/1]
 *
 * 	Add mechanism uuid for KDC private key retrieval from registry db
 * 	[1996/06/30  22:41 UTC  cuti  /main/DCE_1.2.2/2]
 *
 * Revision 1.1.2.2  1996/10/03  14:52:20  arvind
 * 	Merge initial portion of KDC pvtkey retrieval
 * 	[1996/08/14  12:52 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 * 
 * 	Merge KDC pvtkey mechanism ERA support
 * 	[1996/08/02  13:02 UTC  aha  /main/DCE_1.2.2/aha_pk8/1]
 * 
 * 	Add mechanism uuid for KDC private key retrieval from registry db
 * 	[1996/06/30  22:41 UTC  cuti  /main/DCE_1.2.2/2]
 * 
 * Revision 1.1.2.1  1996/07/08  20:19:32  arvind
 * 	OSF DCE1.2.2 Drop3
 * 	sec_psm_base.h,v
 * 	Revision /main/DCE_1.2.2/2  1996/06/30  22:41 UTC  cuti
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 
 * 	Move some variables to sec_psm.c
 * 	[1996/06/27  14:00 UTC  cuti  /main/DCE122_PK/cuti_pk_export/2]
 * 
 * 	Merge out from cuti_pk_new to cuti_pk_export
 * 
 * 	Merge PK work from DCE12_PK off of DCE_1.2 to DCE122_PK off of DCE_1.2.2.
 * 	[1996/06/04  18:54 UTC  psn  /main/DCE122_PK/1]
 * 
 * 	Merge psm, pvtkey work
 * 
 * 	User real ASN.1 algorithm_id
 * 	[1996/05/24  19:26 UTC  cuti  /main/cuti_pk_new/2]
 * 
 * 	Hold sec_psm.c's structure declaration
 * 
 * 	Add dce/ to include files name
 * 	[1996/06/14  15:09 UTC  cuti  /main/cuti_pk_new/5]
 * 
 * 	Remove ; after include
 * 	[1996/06/06  20:14 UTC  cuti  /main/cuti_pk_new/4]
 * 
 * 	Add sec_pvtkey.h
 * 	[1996/06/05  13:43 UTC  cuti  /main/cuti_pk_new/3]
 * 
 * 	User real ASN.1 algorithm_id
 * 	[1996/05/24  19:26 UTC  cuti  /main/cuti_pk_new/2]
 * 
 * 	Hold sec_psm.c's structure declaration
 * 	[1996/05/23  18:45 UTC  cuti  /main/cuti_pk_new/1]
 * 	Revision /main/DCE_1.2.2/1  1996/06/11  18:01 UTC  aha
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/07/08  19:49:20  arvind]
 * 
 * Revision /main/DCE122_PK/1  1996/06/04  18:54 UTC  psn
 * 	Merge PK work from DCE12_PK off of DCE_1.2 to DCE122_PK off of DCE_1.2.2.
 * 
 * Revision /main/DCE12_PK/1  1996/05/28  17:59 UTC  cuti
 * 	Merge psm, pvtkey work
 * 
 * 	User real ASN.1 algorithm_id
 * 	[1996/05/24  19:26 UTC  cuti  /main/cuti_pk_new/2]
 * 
 * 	Hold sec_psm.c's structure declaration
 * 	[1996/05/23  18:45 UTC  cuti  /main/cuti_pk_new/1]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1996
 * Unpublished work.  All Rights Reserved.
 *
 */

#ifndef _SEC_PSM_BASE_H_
#define _SEC_PSM_BASE_H_

/* The following private key storage mechanism are 
 * architectural.  They cannot be changed without
 * destroying interoperability.
 * 
 * The uuids are definded in sec_psm.c
 */

extern uuid_t sec_psm_pkss_mechanism;
    /* 72053e72-b01a-11cf-8bf5-0800090a5254 */

extern uuid_t sec_psm_file_mechanism;
    /* 8687c5b8-b01a-11cf-b137-0800090a5254 */

extern uuid_t sec_psm_kdc_pk_init_mechanism;
    /* adb48ed4-e94d-11cf-ab4b-08000919ebb5 */

extern sec_pk_algorithm_id_t rsa_pkcs;
extern sec_pk_algorithm_id_t md5_rsa;
extern sec_pk_algorithm_id_t rsa_enc;

#define RSA_PKCS &rsa_pkcs 
#define MD5RSA &md5_rsa
#define RSAENC &rsa_enc



#endif
