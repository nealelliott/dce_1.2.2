/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_pvtkey_base.h,v $
 * Revision 1.1.2.3  1996/11/13  18:01:37  arvind
 * 	Correct argument name to bypass function.
 * 	[1996/11/05  02:17 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_4/1]
 *
 * Revision 1.1.2.2  1996/10/15  20:50:17  arvind
 * 	CHFts20143: pvtkey fetch bypass hooks.
 * 	[1996/10/04  22:22 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 * 
 * Revision 1.1.2.1  1996/07/08  20:19:43  arvind
 * 	OSF DCE1.2.2 Drop3
 * 	sec_pvtkey_base.h,v
 * 	Revision /main/DCE_1.2.2/1  1996/06/30  22:41 UTC  cuti
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 	[1996/07/08  19:50:25  arvind]
 * 
 * Revision /main/cuti_pk_export/1  1996/06/27  14:00 UTC  cuti
 * 	Create capability UUID
 * 
 * $EndLog$
 */
 

#ifndef _SEC_PVTKEY_BASE_H_
#define _SEC_PVTKEY_BASE_H_

#include <dce/sec_pvtkey.h>

extern sec_pk_pvtkey_capability SEC_PVTKEY_CAP_KEYGEN;
      /* 3b199b81-cb89-11cf-8a1e-08002b918ea1 */

extern sec_pk_pvtkey_capability SEC_PVTKEY_CAP_ANYALG;
      /* 3d2901e0-cb89-11cf-93d8-08002b918ea1 */

extern sec_pk_pvtkey_capability SEC_PVTKEY_CAP_KEYALG;
      /* 3db8f521-cb89-11cf-850c-08002b918ea1 */

extern sec_pk_pvtkey_capability SEC_PVTKEY_CAP_KEYDEL;
      /* 3e2c9ca0-cb89-11cf-8ed1-08002b918ea1 */

/* Bypass callback function type */

typedef error_status_t (*sec_pvtkey_get_bypass_fn_t) (
    unsigned char *princ, const uuid_t *key_uuid, uuid_t *out_uuid
);

void sec_pvtkey__era_fetch_bypass(unsigned char *cell,
				  sec_pvtkey_get_bypass_fn_t get);



#endif
