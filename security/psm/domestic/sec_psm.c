/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: sec_psm.c,v $
 * Revision 1.1.2.5  1996/11/13  19:08:08  arvind
 * 	OSF DCE 1.2.2 Drop 5
 *
 * 	Merge of cuti_pk_fix
 *
 * 	Merge of cuti_pk_fix
 * 	[1996/10/02  18:28 UTC  cuti  /main/DCE_1.2.2/9]
 *
 * 	Make sec_psm_update_pub_key work
 * 	[1996/10/02  17:47 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/1]
 *
 * Revision 1.1.2.4  1996/10/15  20:53:18  arvind
 * 	OSF DCE 1.2.2 Drop 4.5
 * 	[1996/10/15  20:18:09  arvind]
 * 
 * Revision 1.1.2.3  1996/10/04  16:46:16  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  16:46:01  arvind]
 * 
 * Revision /main/DCE_1.2.2/9  1996/10/02  18:28 UTC  cuti
 * 	Merge of cuti_pk_fix
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_fix/1  1996/10/02  17:47 UTC  cuti
 * 	Make OID macro working
 * 
 * Revision /main/DCE_1.2.2/8  1996/09/16  20:31 UTC  aha
 * 	CHFts19896: public key version support
 * 
 * Revision /main/DCE_1.2.2/aha_pk9_3/1  1996/09/13  23:45 UTC  aha
 * 	Merge.
 * 
 * 	In sec_psm_put_pub_key(), pass public_key to sec_pubkey_store() even
 * 	if no data - this function requires storage of both public and
 * 	private keys, and since sec_pubkey_store() returns an error on an
 * 	empty key, let it handle the case.
 * 	[1996/08/15  19:17 UTC  aha  /main/DCE_1.2.2/aha_pk9/2]
 * 
 * 	Add more bsafe error status filter
 * 	[1996/08/12  18:57 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/2]
 * 
 * 	if pubkey has NULL data, don't call sec_pubkey_store in sec_spm_update_pub_key
 * 	[1996/08/08  18:22 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/1]
 * 
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 	[1996/06/30  22:41 UTC  cuti  /main/DCE_1.2.2/3]
 * 
 * 	Merge out from DCE122
 * 
 * 	Add +1 to malloc of strings
 * 	[1996/06/27  14:04 UTC  cuti  /main/DCE122_PK/cuti_pk_export/2]
 * 
 * 	Merge out from cuti_pk_new to cuti_pk_export
 * 
 * 	Bug fix on sec_psm_close()
 * 	[1996/06/14  15:10 UTC  cuti  /main/DCE12_PK/cuti_pk_new/7]
 * 
 * 	change vno to 0 for public key
 * 	[1996/06/10  15:43 UTC  cuti  /main/DCE12_PK/cuti_pk_new/6]
 * 
 * 	Change modulus_size to in/out
 * 	[1996/06/04  18:00 UTC  cuti  /main/DCE12_PK/cuti_pk_new/5]
 * 
 * 	deal with uninitialized vbl.
 * 	[1996/06/13  17:56 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/2]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/11  18:01 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * 	Change interface for public key domain, kvno.
 * 	[1996/08/09  17:13 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_kdc/2]
 * 
 * 	Don't store pubkey if NULL passed in.
 * 	Rototill to silence gcc warnings, and fix a few real bugs along the
 * 	way.
 * 	[1996/06/20  03:54 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_kdc/1]
 * 
 * 	deal with uninitialized vbl.
 * 	[1996/06/13  17:56 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/2]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * 	Merge CHFts17110 fix.
 * 	[1996/08/08  19:33 UTC  cuti  /main/DCE_1.2.2/4]
 * 
 * 	if pubkey has NULL data, don't call sec_pubkey_store in sec_spm_update_pub_key
 * 	[1996/08/08  18:22 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/1]
 * 
 * 	Merge mainline bugfix so that sec_pubkey_store is not called if
 * 	public_key is empty.
 * 	[1996/08/15  17:55 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/2]
 * 
 * 	Merge KDC pvtkey mechanism ERA support
 * 	[1996/08/14  12:52 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 * 
 * 	Add temporary switch for pvtkey mechanism
 * 	[1996/08/02  13:02 UTC  aha  /main/DCE_1.2.2/aha_pk8/1]
 * 
 * 	Use old_pubkey to restore pubkey
 * 	[1996/09/10  15:23 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/8]
 * 	Include dce/uuid.h
 * 	[1996/09/06  18:20 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/2]
 * 
 * 	In sec_psm_put_pub_key(), pass public_key to sec_pubkey_store() even
 * 	if no data - this function requires storage of both public and
 * 	private keys, and since sec_pubkey_store() returns an error on an
 * 	empty key, let it handle the case.
 * 	[1996/08/15  19:17 UTC  aha  /main/DCE_1.2.2/aha_pk9/2]
 * 
 * 	Add more bsafe error status filter
 * 	[1996/08/12  18:57 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/2]
 * 
 * 	if pubkey has NULL data, don't call sec_pubkey_store in sec_spm_update_pub_key
 * 	[1996/08/08  18:22 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/1]
 * 
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 	[1996/06/30  22:41 UTC  cuti  /main/DCE_1.2.2/3]
 * 
 * 	Merge out from DCE122
 * 
 * 	Add +1 to malloc of strings
 * 	[1996/06/27  14:04 UTC  cuti  /main/DCE122_PK/cuti_pk_export/2]
 * 
 * 	Merge out from cuti_pk_new to cuti_pk_export
 * 
 * 	Bug fix on sec_psm_close()
 * 	[1996/06/14  15:10 UTC  cuti  /main/DCE12_PK/cuti_pk_new/7]
 * 
 * 	change vno to 0 for public key
 * 	[1996/06/10  15:43 UTC  cuti  /main/DCE12_PK/cuti_pk_new/6]
 * 
 * 	Change modulus_size to in/out
 * 	[1996/06/04  18:00 UTC  cuti  /main/DCE12_PK/cuti_pk_new/5]
 * 
 * 	deal with uninitialized vbl.
 * 	[1996/06/13  17:56 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/2]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/11  18:01 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * 	Change interface for public key domain, kvno.
 * 	[1996/08/09  17:13 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_kdc/2]
 * 
 * 	Don't store pubkey if NULL passed in.
 * 	Rototill to silence gcc warnings, and fix a few real bugs along the
 * 	way.
 * 	[1996/06/20  03:54 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_kdc/1]
 * 
 * 	deal with uninitialized vbl.
 * 	[1996/06/13  17:56 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/2]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * 	Merge up from aha_pk8
 * 	[1996/08/28  19:36 UTC  aha  /main/DCE_1.2.2/aha_pk8_1/1]
 * 
 * 	Build default pvtkey_mechanism inline
 * 	[1996/08/15  20:54 UTC  aha  /main/DCE_1.2.2/aha_pk8/2]
 * 
 * 	Merge CHFts17110 fix.
 * 	[1996/08/08  19:33 UTC  cuti  /main/DCE_1.2.2/4]
 * 
 * 	if pubkey has NULL data, don't call sec_pubkey_store in sec_spm_update_pub_key
 * 	[1996/08/08  18:22 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/1]
 * 
 * 	Merge mainline bugfix so that sec_pubkey_store is not called if
 * 	public_key is empty.
 * 	[1996/08/15  17:55 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/2]
 * 
 * 	Merge KDC pvtkey mechanism ERA support
 * 	[1996/08/14  12:52 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 * 
 * 	Add temporary switch for pvtkey mechanism
 * 	[1996/08/02  13:02 UTC  aha  /main/DCE_1.2.2/aha_pk8/1]
 * 
 * 	In sec_psm_put_pub_key(), pass public_key to sec_pubkey_store() even
 * 	if no data - this function requires storage of both public and
 * 	private keys, and since sec_pubkey_store() returns an error on an
 * 	empty key, let it handle the case.
 * 	[1996/08/15  19:17 UTC  aha  /main/DCE_1.2.2/aha_pk9/2]
 * 
 * 	Add more bsafe error status filter
 * 	[1996/08/12  18:57 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/2]
 * 
 * 	if pubkey has NULL data, don't call sec_pubkey_store in sec_spm_update_pub_key
 * 	[1996/08/08  18:22 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/1]
 * 
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 	[1996/06/30  22:41 UTC  cuti  /main/DCE_1.2.2/3]
 * 
 * 	Merge out from DCE122
 * 
 * 	Add +1 to malloc of strings
 * 	[1996/06/27  14:04 UTC  cuti  /main/DCE122_PK/cuti_pk_export/2]
 * 
 * 	Merge out from cuti_pk_new to cuti_pk_export
 * 
 * 	Bug fix on sec_psm_close()
 * 	[1996/06/14  15:10 UTC  cuti  /main/DCE12_PK/cuti_pk_new/7]
 * 
 * 	change vno to 0 for public key
 * 	[1996/06/10  15:43 UTC  cuti  /main/DCE12_PK/cuti_pk_new/6]
 * 
 * 	Change modulus_size to in/out
 * 	[1996/06/04  18:00 UTC  cuti  /main/DCE12_PK/cuti_pk_new/5]
 * 
 * 	deal with uninitialized vbl.
 * 	[1996/06/13  17:56 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/2]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/11  18:01 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * 	Change interface for public key domain, kvno.
 * 	[1996/08/09  17:13 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_kdc/2]
 * 
 * 	Don't store pubkey if NULL passed in.
 * 	Rototill to silence gcc warnings, and fix a few real bugs along the
 * 	way.
 * 	[1996/06/20  03:54 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_kdc/1]
 * 
 * 	deal with uninitialized vbl.
 * 	[1996/06/13  17:56 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/2]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * 	Key version monotonically incresed in spm and feeds in to pvtkey and pubkey
 * 	[1996/09/10  15:38 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/9]
 * 
 * 	Add kdc_pk_init_mechansim
 * 	[1996/09/09  14:00 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/7]
 * 
 * 	Correct PKSS's uuid
 * 	[1996/09/04  12:30 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/6]
 * 
 * 	Retrieve pubkey before store it for later recovering
 * 	[1996/08/24  18:59 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/5]
 * 
 * 	domain_id should be a pointer
 * 	[1996/08/23  17:26 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/4]
 * 
 * 	Put key will try update if failed
 * 	[1996/08/14  17:33 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/3]
 * 
 * 	Add more bsafe error status filter
 * 	[1996/08/12  18:57 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/2]
 * 
 * 	if pubkey has NULL data, don't call sec_pubkey_store in sec_spm_update_pub_key
 * 	[1996/08/08  18:22 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/1]
 * 
 * Revision /main/DCE_1.2.2/aha_pk9_2/4  1996/09/13  18:09 UTC  aha
 * 	Change version number range from 0-255 to 1-255
 * 
 * Revision /main/DCE_1.2.2/aha_pk9_2/3  1996/09/11  17:33 UTC  aha
 * 	Include dce/uuid.h
 * 	[1996/09/06  18:20 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/2]
 * 
 * 	In sec_psm_put_pub_key(), pass public_key to sec_pubkey_store() even
 * 	if no data - this function requires storage of both public and
 * 	private keys, and since sec_pubkey_store() returns an error on an
 * 	empty key, let it handle the case.
 * 	[1996/08/15  19:17 UTC  aha  /main/DCE_1.2.2/aha_pk9/2]
 * 
 * 	Add more bsafe error status filter
 * 	[1996/08/12  18:57 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/2]
 * 
 * 	if pubkey has NULL data, don't call sec_pubkey_store in sec_spm_update_pub_key
 * 	[1996/08/08  18:22 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/1]
 * 
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 	[1996/06/30  22:41 UTC  cuti  /main/DCE_1.2.2/3]
 * 
 * 	Merge out from DCE122
 * 
 * 	Add +1 to malloc of strings
 * 	[1996/06/27  14:04 UTC  cuti  /main/DCE122_PK/cuti_pk_export/2]
 * 
 * 	Merge out from cuti_pk_new to cuti_pk_export
 * 
 * 	Bug fix on sec_psm_close()
 * 	[1996/06/14  15:10 UTC  cuti  /main/DCE12_PK/cuti_pk_new/7]
 * 
 * 	change vno to 0 for public key
 * 	[1996/06/10  15:43 UTC  cuti  /main/DCE12_PK/cuti_pk_new/6]
 * 
 * 	Change modulus_size to in/out
 * 	[1996/06/04  18:00 UTC  cuti  /main/DCE12_PK/cuti_pk_new/5]
 * 
 * 	deal with uninitialized vbl.
 * 	[1996/06/13  17:56 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/2]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/11  18:01 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * 	Change interface for public key domain, kvno.
 * 	[1996/08/09  17:13 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_kdc/2]
 * 
 * 	Don't store pubkey if NULL passed in.
 * 	Rototill to silence gcc warnings, and fix a few real bugs along the
 * 	way.
 * 	[1996/06/20  03:54 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_kdc/1]
 * 
 * 	deal with uninitialized vbl.
 * 	[1996/06/13  17:56 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/2]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * 	Merge up from aha_pk8
 * 	[1996/08/28  19:36 UTC  aha  /main/DCE_1.2.2/aha_pk8_1/1]
 * 
 * 	Build default pvtkey_mechanism inline
 * 	[1996/08/15  20:54 UTC  aha  /main/DCE_1.2.2/aha_pk8/2]
 * 
 * 	Merge CHFts17110 fix.
 * 	[1996/08/08  19:33 UTC  cuti  /main/DCE_1.2.2/4]
 * 
 * 	if pubkey has NULL data, don't call sec_pubkey_store in sec_spm_update_pub_key
 * 	[1996/08/08  18:22 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/1]
 * 
 * 	Merge mainline bugfix so that sec_pubkey_store is not called if
 * 	public_key is empty.
 * 	[1996/08/15  17:55 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/2]
 * 
 * 	Merge KDC pvtkey mechanism ERA support
 * 	[1996/08/14  12:52 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 * 
 * 	Add temporary switch for pvtkey mechanism
 * 	[1996/08/02  13:02 UTC  aha  /main/DCE_1.2.2/aha_pk8/1]
 * 
 * 	In sec_psm_put_pub_key(), pass public_key to sec_pubkey_store() even
 * 	if no data - this function requires storage of both public and
 * 	private keys, and since sec_pubkey_store() returns an error on an
 * 	empty key, let it handle the case.
 * 	[1996/08/15  19:17 UTC  aha  /main/DCE_1.2.2/aha_pk9/2]
 * 
 * 	Add more bsafe error status filter
 * 	[1996/08/12  18:57 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/2]
 * 
 * 	if pubkey has NULL data, don't call sec_pubkey_store in sec_spm_update_pub_key
 * 	[1996/08/08  18:22 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/1]
 * 
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 	[1996/06/30  22:41 UTC  cuti  /main/DCE_1.2.2/3]
 * 
 * 	Merge out from DCE122
 * 
 * 	Add +1 to malloc of strings
 * 	[1996/06/27  14:04 UTC  cuti  /main/DCE122_PK/cuti_pk_export/2]
 * 
 * 	Merge out from cuti_pk_new to cuti_pk_export
 * 
 * 	Bug fix on sec_psm_close()
 * 	[1996/06/14  15:10 UTC  cuti  /main/DCE12_PK/cuti_pk_new/7]
 * 
 * 	change vno to 0 for public key
 * 	[1996/06/10  15:43 UTC  cuti  /main/DCE12_PK/cuti_pk_new/6]
 * 
 * 	Change modulus_size to in/out
 * 	[1996/06/04  18:00 UTC  cuti  /main/DCE12_PK/cuti_pk_new/5]
 * 
 * 	deal with uninitialized vbl.
 * 	[1996/06/13  17:56 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/2]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/11  18:01 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * 	Change interface for public key domain, kvno.
 * 	[1996/08/09  17:13 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_kdc/2]
 * 
 * 	Don't store pubkey if NULL passed in.
 * 	Rototill to silence gcc warnings, and fix a few real bugs along the
 * 	way.
 * 	[1996/06/20  03:54 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_kdc/1]
 * 
 * 	deal with uninitialized vbl.
 * 	[1996/06/13  17:56 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/2]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_bugfix/9  1996/09/10  15:38 UTC  cuti
 * 	Key version monotonically incresed in spm and feeds in to pvtkey and pubkey
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_bugfix/7  1996/09/09  14:00 UTC  cuti
 * 	Add kdc_pk_init_mechansim
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_bugfix/6  1996/09/04  12:30 UTC  cuti
 * 	Correct PKSS's uuid
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_bugfix/5  1996/08/24  18:59 UTC  cuti
 * 	Retrieve pubkey before store it for later recovering
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_bugfix/4  1996/08/23  17:26 UTC  cuti
 * 	domain_id should be a pointer
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_bugfix/3  1996/08/14  17:33 UTC  cuti
 * 	Put key will try update if failed
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_bugfix/2  1996/08/12  18:57 UTC  cuti
 * 	Add more bsafe error status filter
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_bugfix/1  1996/08/08  18:22 UTC  cuti
 * 	if pubkey has NULL data, don't call sec_pubkey_store in sec_spm_update_pub_key
 * 
 * Revision /main/DCE_1.2.2/3  1996/06/30  22:41 UTC  cuti
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 
 * Revision /main/DCE122_PK/cuti_pk_export/3  1996/06/27  19:54 UTC  cuti
 * 	Merge out from DCE122
 * 
 * 	Add +1 to malloc of strings
 * 	[1996/06/27  14:04 UTC  cuti  /main/DCE122_PK/cuti_pk_export/2]
 * 
 * 	Merge out from cuti_pk_new to cuti_pk_export
 * 
 * 	Bug fix on sec_psm_close()
 * 	[1996/06/14  15:10 UTC  cuti  /main/DCE12_PK/cuti_pk_new/7]
 * 
 * 	change vno to 0 for public key
 * 	[1996/06/10  15:43 UTC  cuti  /main/DCE12_PK/cuti_pk_new/6]
 * 
 * 	Change modulus_size to in/out
 * 	[1996/06/04  18:00 UTC  cuti  /main/DCE12_PK/cuti_pk_new/5]
 * 	Revision /main/DCE_1.2.2/2  1996/06/13  21:30 UTC  sommerfeld
 * 	deal with uninitialized vbl.
 * 	[1996/06/13  17:56 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/2]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/06/11  18:01 UTC  aha
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 
 * Revision /main/DCE122_PK/1  1996/06/04  18:54 UTC  psn
 * 	DCE12_PK -> DCE122_PK
 * 
 * Revision /main/DCE12_PK/2  1996/05/28  18:00 UTC  cuti
 * 	Merge psm, pvtkey work
 * 
 * Revision /main/DCE12_PK/cuti_pk_new/4  1996/05/24  20:37 UTC  cuti
 * 	Update from review
 * 
 * Revision /main/DCE12_PK/cuti_pk_new/3  1996/05/24  19:24 UTC  cuti
 * 	Change local_algorithm_id to sec_pk_algorithm_id
 * 
 * Revision /main/DCE12_PK/cuti_pk_new/2  1996/05/23  18:42 UTC  cuti
 * 	Prepare for psm review.
 * 
 * $EndLog$
 */

#include <string.h>
#include <dce/uuid.h>
#include <malloc.h>
#include <dce/idlbase.h>
#include <rgymacro.h>
#include <dce/rgynbase.h>
#include <dce/sec_pvtkey.h>
#include <dce/sec_pvtkey_base.h>
#include <dce/sec_psm.h>
#include <sec_bsafe.h>
#include <dce/sec_pk.h>
#include <dce/sec_pubkey.h>


typedef struct sec_psm_context {
    void              *magic;
    char              *name;  /* canonical name */
    char              *pwd;
    unsigned32        mechanism_index;  
    sec_pk_mechanism_handle_t   mechanism_handle;
    sec_pk_domain_t   domain_id;
} sec_psm_context_t, sec_psm_context_p_t;

uuid_t sec_psm_pkss_mechanism = {
    /* 72053e72-b01a-11cf-8bf5-0800090a5254 */
    0x72053e72,
    0xb01a,
    0x11cf,
    0x8b,
    0xf5,
    {0x08, 0x00, 0x09, 0x0a, 0x52, 0x54}
};


uuid_t sec_psm_file_mechanism = {
    /* 8687c5b8-b01a-11cf-b137-0800090a5254 */
    0x8687c5b8,
    0xb01a,
    0x11cf,
    0xb1,
    0x37,
    {0x08, 0x00, 0x09, 0x0a, 0x52, 0x54}
};

uuid_t sec_psm_kdc_pk_init_mechanism = {
    /* adb48ed4-e94d-11cf-ab4b-08000919ebb5 */
    0xadb48ed4,
    0xe94d,
    0x11cf,
    0xab,
    0x4b,
    {0x08, 0x00, 0x09, 0x19, 0xeb, 0xb5}
};

/* RSA DSI's key */
idl_byte rsa_pkcs_oid[] =
{0x2a,0x86,0x48,0x86,0xf7,0x0d};

/* signature algorithm  */
idl_byte  md5_rsa_oid[] =
{0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x05};

/* RSA encryption algorithm */
idl_byte  rsa_enc_oid[] =
{0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x01};

#define OID(XX) \
sec_pk_algorithm_id_t XX = { sizeof(XX##_oid), (idl_byte *)&XX##_oid}

#define rsadsi OID(rsa_pkcs)
#define md5rsa OID(md5_rsa)
#define rsaenc OID(rsa_enc)

rsadsi; 
md5rsa; 
rsaenc;

/*
sec_pk_algorithm_id_t rsa_pkcs = {sizeof(rsa_pkcs_oid), (idl_byte *)&rsa_pkcs_oid};
sec_pk_algorithm_id_t md5_rsa = {sizeof(md5_rsa_oid), (idl_byte *)&md5_rsa_oid};
sec_pk_algorithm_id_t rsa_enc = {sizeof(rsa_enc_oid), (idl_byte *)&rsa_enc_oid};
*/

typedef unsigned32 sec_pk_algorithm_id_subtype;

#define dont_care   0
#define encryption  1
#define decryption  2


/* sec_psm_open
 * 
 * Open the personal security mechanism using password.  
 * 
 * Return status
 *   error_status_ok:    Success.
 *   other (non-zero):   sec_pvtkey_privileged
 *                       sec_pvtkey_no_more_memory
 *                       sec_psm_no_more_memory
 *
 * Input
 *
 *   name: Pointer to the user's canonical name within the 
 *         specified domain. 
 *   pwd: Pointer to the user's password.
 *   domain_id: Pointer to the application domain the user 
 *              is operating on.
 *
 * Output
 *
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data. 
 *
 */


error_status_t sec_psm_open(
    void               *name,
    idl_char           *pwd,
    sec_pk_domain_t    *domain_id,
    sec_psm_handle_t   *psm_handle
) {
    error_status_t  st;
    sec_psm_context_t  *psm_context = NULL;
    sec_pk_mechanism_handle_t pvtkey_handle;

    /* Initialize output data. */
    CLEAR_STATUS(&st);
    *psm_handle = NULL;

    /* open the pvtkey storage. */
    if (st = sec_pvtkey_open((idl_char *)name, domain_id, &pvtkey_handle)) {
	return(st);
    }

    /* Malloc the handle and fill in the contents. */
    if (!(psm_context = 
	  (sec_psm_context_t *)malloc(sizeof(sec_psm_context_t)))) {
        sec_pvtkey_close(pvtkey_handle);
	return(sec_psm_no_more_memory);
    }

    if (!(psm_context->name = (char *)malloc(strlen(name) + 1)) ) {
	free(psm_context);
	psm_context = NULL;
	sec_pvtkey_close(pvtkey_handle);
	return(sec_psm_no_more_memory);
    }
    strcpy(psm_context->name, (char *)name);

    psm_context->mechanism_handle = pvtkey_handle;
    psm_context->domain_id = *domain_id;
    if (pwd)
    {
        if (!(psm_context->pwd = (char *)malloc(strlen((char *)pwd) + 1))) {
                sec_pvtkey_close(pvtkey_handle);
            free(psm_context);
            psm_context = NULL;
            return(sec_psm_no_more_memory);
        }
        strcpy(psm_context->pwd, (char *)pwd);
    }
    else
    {
        psm_context->pwd=NULL;
    }    
 
    psm_context->magic = psm_context;
    *psm_handle = (sec_psm_handle_t *)psm_context;

    return st;

}

/* error: sec_psm_not_init
          sec_psm_invalid_handle
 */

sec_psm_context_t *  sec_psm_check_handle(
  sec_psm_handle_t  handle,
  error_status_t    *stp
)
{

    sec_psm_context_t  *psm_context = handle;

    CLEAR_STATUS(stp);

    /* check if handle is NULL, stop here, otherwise we'll just die */
    if (!handle) { /* handle is NULL */
        SET_STATUS(stp, sec_psm_not_init);
        return psm_context;
    }

    /* Make sure handle is not a garbage. */
    if (psm_context->magic != psm_context) {
        SET_STATUS(stp, sec_psm_invalid_handle);
    }
       
 
    return(GOOD_STATUS(stp) ? psm_context: NULL);

}

/* sec_psm_close
 * 
 * Close the personal security mechanism and cleanup the 
 * personal security context data.  It also ensures any
 * confidential information such as passwords or private
 * key are zeroed.
 * 
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_psm_not_init
 *         		sec_psm_invalid_handle
 * 	  		sec_psm_internal_error
 *	  		sec_pvtkey_invalid_handle
 *	  		sec_pvtkey_mechanism_not_init
 *
 * Input
 *
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data.  This handle should be 
 *               obtained through sec_psm_open().
 *
 */


error_status_t sec_psm_close(
   sec_psm_handle_t psm_handle
) {
    error_status_t st;
    sec_psm_context_t *psm_context = NULL;

    CLEAR_STATUS(&st);

    /* check handle */
    if (psm_context = sec_psm_check_handle(psm_handle, &st)) {
	if (psm_context->name) {
	    free(psm_context->name);
	    psm_context->name = NULL;
	}
	if (psm_context->pwd) {
	    memset(psm_context->pwd, 0,  strlen(psm_context->pwd));
	    free(psm_context->pwd);
	    psm_context->pwd = NULL;
	}

	/* close pvtkey */

	sec_pvtkey_close(psm_context->mechanism_handle);
	psm_context->mechanism_handle = NULL;
	psm_context->magic = NULL;
	free(psm_context);
	psm_context = NULL;
	return st;    
    }
    else {
	return(st);
    }
 
}

/* sec_psm_gen_pub_key
 *
 *
 * Generate public key according to key type.  The routine
 * allocates memeory for the data returned in the key
 * parameters.  Users should call sec_pk_data_free() to
 * deallocate that memory.
 *
 * Currently it only supports RSADSI's public key.
 *
 * Return status
 *   error_status_ok:    Success.
 *   other (non-zero):   sec_psm_wrong_pub_key_type 
 *                       sec_s_bsafe_alloc
 *                       sec_s_bsafe_mod_size_out_of_range
 *
 * Input
 *
 *    key_type: Only RSA_PKCS is supported.
 *
 *    modulus_bit_size: Desired key-length.  The length of the desired key.
 *                 	Interpretation of this parameter is algorithm-dependent;
 *                 	For RSA, it shall be interpreted as the bit-length of
 *                 	the key. 
 *
 * Output
 *
 *    private_key:  Pointer to the encoded private key structure of the
 *                  newly-generated key.  
 *    public_key: Pointer to the encoded public key structure of the
 *                 newly-generated key.
 *
 */

error_status_t sec_psm_gen_pub_key(
    sec_pk_algorithm_id_t   *key_type,        /* [in] */
    unsigned32              modulus_bit_size, /* [in] */
    sec_pk_gen_data_t       *seed,            /* [in], maybe internal data */
    sec_pk_data_t           *public_key,     /* [out] */
    sec_pk_data_t           *private_key     /* [out] */
){
    error_status_t  st;
    /* generate keys */

    if (memcmp(key_type, RSA_PKCS, sizeof(rsa_pkcs))) {
	return(sec_psm_wrong_pub_key_type);
    }

    st = sec_bsafe_gen_pub_key(modulus_bit_size, seed, 
			  public_key, private_key);
    
    return(st);

    

}

/* Fow now, I am not using OID or DER_OID */

error_status_t get_alg_id(
    sec_pk_algorithm_id_t         *signature_id,
    sec_pk_algorithm_id_subtype   subtype,
    sec_bsafe_alg_type_t          *alg_id)
{
  error_status_t st;

  CLEAR_STATUS(&st);

  if (!(memcmp(signature_id, &rsa_enc, sizeof(rsa_enc)))) {
      switch(subtype) {
      case encryption: *alg_id = sec_enc_rsa_pub; break;  /* public key encryption*/
      case decryption: *alg_id = sec_enc_rsa_priv; break;/* private key decryption */
      }
  } else if (!(memcmp(signature_id, &md5_rsa, sizeof(md5_rsa)))) {
    *alg_id = sec_sign_md5rsa;
  } else {
    st = sec_psm_unsupported_algorithm_id;
  }

    return(st);
}

/* sec_psm_sign_data
 * 
 * Compute the signature of the input data using the
 * signature algorithm specified in the arguments.
 *
 * The routine allocates memory for the data returned in 
 * the signature parameter.  Users should call 
 * sec_pk_data_free() to deallocate that memory.
 * 
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_psm_not_init
 *          		sec_psm_invalid_handle
 *                      sec_psm_unsupported_algorithm_id
 *	  		sec_pvtkey_invalid_handle
 *                      sec_pvtkey_mechanism_not_init
 *                      sec_pvtkey_internal_error
 *                      sec_pvtkey_invalid_password
 *                      sec_pvtkey_multiple_key_usages
 *
 *
 * Input
 *   
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data.  This handle should be 
 *               obtained through sec_psm_open().
 *   signature_alg_id: The ASN.1 DER-encoded object ID of the signature
 *                  algorithm, such as MD5WithRSAEncryption.
 *   key_usage: The usage of the private key which should be
 *              picked for this operation.
 *   data: Pointer to the ASN.1 DER-encoded data to be signed.
 *
 * Output
 *   
 *   signature: Pointer to a signature buffer pointer.
 *
 */


error_status_t sec_psm_sign_data(
    sec_psm_handle_t              psm_handle,
    sec_pk_algorithm_id_t         *signature_alg_id,
    sec_pk_usage_flags_t          key_usage,
    sec_pk_data_t                 *data,
    unsigned32                    *kvno,
    sec_pk_signed_t               *signature
)
{
    sec_pk_data_t    pvtkey;
    sec_psm_context_t *psm_context=NULL;
    sec_bsafe_alg_type_t alg_id;
    error_status_t   st;

    CLEAR_STATUS(&st);
    if (!(psm_context = sec_psm_check_handle(psm_handle, &st))) {
	return st;
    }
	

    if (!(st = sec_pvtkey_get(psm_context->mechanism_handle, 
			      (idl_char *)psm_context->pwd,
			      key_usage, kvno, &pvtkey, NULL))) {
 

      /* switch data to get alg_id from signature_id */
      if (!(st = get_alg_id(signature_alg_id, dont_care, &alg_id))) {

	st = sec_bsafe_sign_data(alg_id, (sec_pk_gen_data_t *)data, &pvtkey, signature);

      }
      /* zero and free private key memory */
      sec_pk_data_zero_and_free(&pvtkey);
    }

    return st;
}

/* sec_psm_verify_data
 * 
 * Verify the data.  Usually, you verify other people's signature.
 * 
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_psm_not_init
 *          		sec_psm_invalid_handle
 *                      sec_psm_unsupported_algorithm_id
 *          		sec_s_bsafe_decryption_failure (invalid key)
 *	  		sec_pk_e_domain_unsupported,
 *           		sec_pk_e_usage_unsupported,
 *           		sec_rgy_object_not_found,
 *           		sec_rgy_not_authorized,
 *           		sec_attr_unauthorized
 *
 * Input
 *
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data.  This handle should be
 *               obtained through sec_psm_open().
 *   signer_name: Pointer to the name of the principal had singed
 *                the data.
 *   signature_alg_id: The ASN.1 DER-encoded object ID of the signature
 *                  algorithm, such as MD5WithRSAEncryption.
 *   key_usage: The usage of the ;public key which should be
 *              picked for this operation.
 *   data: Pointer to the data to be verified
 *   signature: Pointer to the signature to be verified.
 *
 */

error_status_t sec_psm_verify_data(
    sec_psm_handle_t        psm_handle,
    sec_pk_domain_t         *signer_domain,
    void                    *signer_name,
    unsigned32              kvno,
    sec_pk_algorithm_id_t   *signature_alg_id,
    sec_pk_usage_flags_t    key_usage,
    sec_pk_data_t           *data,
    sec_pk_signed_t         *signature
){
    sec_psm_context_t *psm_context;
    sec_bsafe_alg_type_t alg_id;
    sec_pk_pubkey_t  pubkey;
    error_status_t   st;

    CLEAR_STATUS(&st);
    if (!(psm_context = sec_psm_check_handle(psm_handle, &st))) {
	return st;
    }


    if (signer_domain == NULL)
        signer_domain = &(psm_context->domain_id);
	
    if (GOOD_STATUS(&st)) {
   
	if (!(st = sec_pubkey_retrieve((idl_char *)signer_name,
				     signer_domain, 
				     key_usage, &kvno, &pubkey))) {

	    /* switch data to get alg_id from signature_id */
	  if (!(st = get_alg_id(signature_alg_id, dont_care, &alg_id))) {

	    st = sec_bsafe_verify_data(alg_id, (sec_pk_gen_data_t *)data, &pubkey, signature);
	    if ((st == sec_s_bsafe_input_data) || (st == sec_s_bsafe_input_len)) {
		st = sec_s_bsafe_decryption_failure;
	    }
	  }
	  sec_pk_data_free(&pubkey);
	   
	}
    }

    return st;

}

/* sec_psm_encrypt_data
 * 
 * Encrypt the data in the algorithm specified.  The routine
 * allocates memory for the data returned in the cipher_data
 * parameter.  Users should call sec_pk_data_free() to 
 * deallocate that memory.
 *
 * For reference implementation, only keyEncipherment key_usage
 * will be implemented.
 *
 * Usually you encrypt the data with other's public key.
 * 
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_psm_not_init
 *          		sec_psm_invalid_handle
 *                      sec_psm_unsupported_algorithm_id
 *	  		sec_pk_e_domain_unsupported,
 *           		sec_pk_e_usage_unsupported,
 *           		sec_rgy_object_not_found,
 *           		sec_rgy_not_authorized,
 *           		sec_attr_unauthorized
 *
 * Input
 *
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data.  This handle should be
 *               obtained through sec_psm_open().
 *   encryptee_name: Pointer to the name of the principal this data is
 *                   encrypted for.
 *   encryption_alg_id: The ASN.1 DER-encoded object ID of encryption
 *                   algorithm, such as RSA.
 *   key_usage: The usage of the public key this key pair 
 *              belongs to.
 *   clear_data: Pointer to the ASN.1 DER-encoded data to be encrypted.
 *
 * Output
 *
 *   cipher_data: Pointer to the encrypted output buffer.
 */

error_status_t sec_psm_encrypt_data(
    sec_psm_handle_t              psm_handle,
    sec_pk_domain_t               *encryptee_domain,
    void                          *encryptee_name,
    unsigned32                    *kvno,
    sec_pk_algorithm_id_t         *encryption_alg_id,
    sec_pk_usage_flags_t          key_usage,
    sec_pk_data_t                 *clear_data,
    sec_pk_data_t                 *cipher_data
)
{
    sec_psm_context_t *psm_context;
    sec_bsafe_alg_type_t alg_id;
    sec_pk_data_t    pubkey;
    error_status_t   st;
    sec_pk_gen_data_t       *seed=NULL;

    CLEAR_STATUS(&st);
    if (!(psm_context = sec_psm_check_handle(psm_handle, &st))) {
	return st;
    }

    if (encryptee_domain == NULL)
        encryptee_domain = &(psm_context->domain_id);
    

    if (!(st = sec_pubkey_retrieve((idl_char *)encryptee_name,
				 encryptee_domain,
				 key_usage, kvno, &pubkey))) {


	    /* switch data to get alg_id from signature_id */
      if (!(st = get_alg_id(encryption_alg_id, encryption, &alg_id))) {
	

	    st = sec_bsafe_encrypt_data(alg_id, seed, &pubkey, 
					(sec_pk_gen_data_t *)clear_data, 
					(sec_pk_gen_data_t *)cipher_data);
      }

      sec_pk_data_free(&pubkey);

    }

    return(st);

}

/* sec_psm_decrypt_data
 * 
 * Decrypt the mechanism-specific encrypted data. The routine
 * allocates memory for the data returned in the clear_data
 * parameter.  Users should call sec_pk_data_free() to 
 * deallocate that memory. 
 *
 * For reference implementation, only keyEncipherment key_usage
 * will be implemented.
 * 
 * Usually you decrypted the data with your own private key.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_psm_not_init
 *          		sec_psm_invalid_handle
 *                      sec_psm_unsupported_algorithm_id
 *	  		sec_s_bsafe_encryption_failure
 *	  		sec_pvtkey_invalid_handle
 *                      sec_pvtkey_mechanism_not_init
 *                      sec_pvtkey_internal_error
 *                      sec_pvtkey_invalid_password
 *                      sec_pvtkey_multiple_key_usages
 *
 * Input
 *
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data.  This handle should be
 *               obtained through sec_psm_open().
 *   encryption_alg_id: The ASN.1 DER-encoded object ID of encryption
 *                   algorithm, such as RSA.
 *   key_usage: The usage of the private key which should be
 *              picked for this operation.
 *   cipher_data: Pointer to encrypted cipher buffer.
 *
 *
 * Output
 *
 *   clear_data: Pointer to decrypted clear text buffer pointer.
 *
 */

error_status_t sec_psm_decrypt_data(
    sec_psm_handle_t              psm_handle,
    unsigned32                    kvno,
    sec_pk_algorithm_id_t         *encryption_alg_id,
    sec_pk_usage_flags_t          key_usage,
    sec_pk_data_t                 *cipher_data,
    sec_pk_data_t                 *clear_data
)
{
    sec_pk_data_t      pvtkey;
    sec_psm_context_t *psm_context=NULL;
    sec_bsafe_alg_type_t alg_id;
    error_status_t   st;

    CLEAR_STATUS(&st);
    if (!(psm_context = sec_psm_check_handle(psm_handle, &st))) {
	return st;
    }

    if (!(st = sec_pvtkey_get(psm_context->mechanism_handle,
			    (idl_char *)psm_context->pwd,
			    key_usage, &kvno, &pvtkey, NULL))) {
 
	/* switch data to get alg_id from signature_id */

      if (!(st = get_alg_id(encryption_alg_id, decryption, &alg_id))) {

	st = sec_bsafe_decrypt_data(alg_id, 
				    &pvtkey, 
				    (sec_pk_gen_data_t *)cipher_data, 
				    (sec_pk_gen_data_t *)clear_data);



        if ((st == sec_s_bsafe_input_data) || (st == sec_s_bsafe_input_len)) {
	    st = sec_s_bsafe_decryption_failure;
	}
      }
      sec_pk_data_zero_and_free(&pvtkey);
    }
	

    return(st);

}

/* sec_psm_put_pub_key
 * 
 * Store the public key pair with associated data into the personal 
 * security mechanism.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_psm_not_init
 *         		sec_psm_invalid_handle
 *	  		sec_pk_e_domain_unsupported,
 *           		sec_pk_e_usage_unsupported,
 *           		sec_rgy_object_not_found,
 *           		sec_rgy_not_authorized,
 *           		sec_attr_unauthorized
 *    			sec_pvtkey_invalid_handle
 *                      sec_pvtkey_mechanism_not_init
 *                      sec_pvtkey_no_more_memory
 *                      sec_pvtkey_internal_error
 *                      sec_pvtkey_same_domain_and_usage_key_already_exists.
 *
 * Input
 *
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data.  This handle should be
 *               obtained through sec_psm_open().
 *   pwd:  Pointer to the user's password.
 *   key_usage: The usage of the public key this key pair 
 *              belongs to.
 *   pvtkey: Pointer to the ASN.1 DER-encoded private key buffer.
 *   pubkey: Pointer to the ASN.1 DER-encoded public key buffer.
 *
 */

error_status_t sec_psm_put_pub_key(
    sec_psm_handle_t       psm_handle,
    idl_char               *pwd,
    sec_pk_usage_flags_t   key_usage,
    sec_pk_pubkey_t        *public_key,
    sec_pk_pvtkey_t        *private_key
) {
    sec_psm_context_t *psm_context=NULL;
    error_status_t st, pubkey_st;
    unsigned32 kvno=0, old_kvno=0;
    sec_pk_pubkey_t old_pubkey;

    sec_pk_data_init(&old_pubkey);
    CLEAR_STATUS(&st);
    if (!(psm_context = sec_psm_check_handle(psm_handle, &st))) {
	return st;
    }

    st = sec_pubkey_retrieve(psm_context->name, &(psm_context->domain_id),
				 key_usage,  &old_kvno, &old_pubkey);
      
    /* Supposedly, this should be an error if st is OK.  But we kind 
       of naking pub_pub_key and update_pub_key interchangeable */

    if (BAD_STATUS(&st)) { /* initial */
      kvno = 1;
    }
    else { /* monotonically increment */
      kvno = old_kvno + 1;
    }

    if (public_key) {
        /* Pass public_key even if no data, since this function
	 * requires both public_key and private_key, and sec_pubkey_store
	 * will return an appropriate error if public_key has no data
	 */
 

        if (st = sec_pubkey_store (psm_context->name, 
				   &(psm_context->domain_id), 
				   key_usage, &kvno,
				   public_key)) {
	      return st;
	}
    }
           
    if (private_key) {
	/* Store private key, */
	st = sec_pvtkey_store(psm_context->mechanism_handle, 
				  pwd, key_usage, kvno,
				  private_key, public_key);
    }

    if (BAD_STATUS(&st)) {
      /* If we got here, it means pubkey_store OK, however, pvtkey_store failed.
	 if there is an old pubkey, restore it for atomicity; otherwise there
         is no old pubkey.  In this case, we are supposed to remove newly stored
         pubkey, but
         key deletion is not supported. We do nothing here.  Fortunetely, to the
	 user, this operation is failed, supposedly users will reissue put key
         (or similar) command again.  If key deletion is supported, we'll remove
         newly stored pubkey
	 */
      if (old_pubkey.len != 0) { /* restore the old pubkey */
	pubkey_st = sec_pubkey_store (psm_context->name, 
			       &(psm_context->domain_id), 
			       key_usage, &old_kvno,
			       &old_pubkey);
      }
    }

    return st;

    
}

/* sec_psm_update_pub_key
 * 
 * Update the user's own public key pair or passphrase. The oldpassphrase is 
 * specified to authenticate the user updating the key.
 *
 * In the reference implementation, only a single version of a key
 * with a given key usage will be maintained.  Any old key version
 * will be overwritten.
 * 
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_psm_not_init
 *          		sec_psm_invalid_handle
 *	  		sec_pk_e_domain_unsupported,
 *           		sec_pk_e_usage_unsupported,
 *           		sec_rgy_object_not_found,
 *           		sec_rgy_not_authorized,
 *           		sec_attr_unauthorized
 *     			sec_pvtkey_invalid_handle
 *			sec_pvtkey_mechanism_not_init
 *			sec_pvtkey_private_key_is_not_supplied
 *			sec_pvtkey_new_password_required
 *			sec_pvtkey_no_more_memory
 *			sec_pvtkey_internal_error
 *			sec_pvtkey_no_matched_private_key
 *                      sec_pvtkey_Invalid_password.
 *
 * Input
 *
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data.  This handle should be
 *               obtained through sec_psm_open().
 *   oldpwd:  Pointer to the user's current password.
 *   newpwd:  Pointer to the user's new password.
 *   key_usage: The usage of the public key this key pair 
 *              belongs to.
 *   pvtkey: Pointer to the ASN.1 DER-encoded private key buffer.
 *   pubkey: Pointer to the ASN.1 DER-encoded public key buffer.
 *
 */

error_status_t sec_psm_update_pub_key(
    sec_psm_handle_t       psm_handle,
    idl_char               *oldpwd,
    idl_char               *newpwd,
    sec_pk_usage_flags_t   key_usage,
    sec_pk_pubkey_t        *public_key,
    sec_pk_pvtkey_t        *private_key
){

    sec_psm_context_t *psm_context = NULL;
    error_status_t st, pubkey_st;
    unsigned32 kvno, old_kvno=0;
    sec_pk_pubkey_t old_pubkey;

    sec_pk_data_init(&old_pubkey);
    CLEAR_STATUS(&st);
    if (!(psm_context = sec_psm_check_handle(psm_handle, &st))) {
	return st;
    }


    st = sec_pubkey_retrieve(psm_context->name, &(psm_context->domain_id),
				 key_usage,  &old_kvno, &old_pubkey);

    /* Supposedly, this should be an error if st is bad.  But we kind 
       of naking pub_pub_key and update_pub_key interchangeable */

    if (BAD_STATUS(&st)) { /* initial */
      kvno = 1;
    }
    else { /* monotonically increment */
      kvno = old_kvno + 1;
    }


    if (public_key && public_key->data) {
        if (st = sec_pubkey_store (psm_context->name, 
				   &(psm_context->domain_id), 
				   key_usage, &kvno, public_key))  {
	  return st;
	}
    }

    if (private_key) {
	/* Store private key, */
	st = sec_pvtkey_update(psm_context->mechanism_handle,
				   oldpwd, newpwd, 
				   key_usage, &kvno,
				   private_key, public_key);
    }


    if (BAD_STATUS(&st)) {
      /* If we got here, it means pubkey_store OK, however, pvtkey_update failed.
	 if there is an old pubkey, restore it for atomicity; No old pubkey
         should not happen here.  See put_pub_key for reference
	 */
      if (old_pubkey.len != 0) { /* restore the old pubkey */
	pubkey_st = sec_pubkey_store (psm_context->name, 
			       &(psm_context->domain_id), 
			       key_usage, &old_kvno,
			       &old_pubkey);
      }
    }


    return st;

}




