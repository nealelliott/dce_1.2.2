/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: sec_pvtkey_registered_keystores.c,v $
 * Revision 1.1.2.3  1996/10/15  20:53:48  arvind
 * 	OSF DCE 1.2.2 Drop 4.5
 * 	[1996/10/15  20:39:25  arvind]
 *
 * Revision 1.1.2.2  1996/10/04  16:26:28  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  16:25:48  arvind]
 * 
 * Revision /main/DCE_1.2.2/3  1996/10/02  18:05 UTC  aha
 * 	     Merge fix for OT CR 13637: invalid keystore on HP-UX
 * 
 * Revision /main/DCE_1.2.2/aha_pk9_5/1  1996/10/01  17:53 UTC  aha
 * 	     Add TERMINATE flag to last HPUX installed_keystore entry.  Change
 * 	     is conditionalized for HPUX only.
 * 
 * Revision /main/DCE_1.2.2/2  1996/09/23  21:36 UTC  arvind
 * 	          Conditionalize  HP build not to use pkss/pkc
 * 	          [1996/09/10  18:32 UTC  arvind  /main/arvind_pkss/3]
 * 
 * 	          Comment out pkss stuff
 * 	          [1996/09/09  19:28 UTC  cuti  /main/arvind_pkss/2]
 * 
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:45 UTC  arvind
 * 	          PKSS drop from DEC (DCE1.2.2)
 * 	          [1996/08/30  15:43 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/*
 * (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 * Module:
 *
 * Purpose:
 *
 */

#include <dce/nbase.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_psm_base.h>
#include "sec_pvtkey_registered_keystores.h"
#include "sec_pvtkey_pkss.h"
#include "sec_pvtkey_file.h"

/*
 * Each entry on the following table describes a single keystore 
 * implementation.  The flags field of each entry defines how
 * the entry is treated by search functions, as follows:
 * 
 * SEC_PVTKEY_SWITCH_FLAG_ENABLED - All keystores should have this flag set.
 *                                  Entries without this flag will be ignored.
 * SEC_PVTKEY_SWITCH_FLAG_TERMINATE - The last entry must have this flag
 *                                    set.  It terminates a search.
 * SEC_PVTKEY_SWITCH_FLAG_MATCHALL - Marks the entry as a default entry.
 *                                   If an earlier entry in the table did 
 *                                   not match the desired implementation UUID,
 *                                   this entry will be returned, if defaulting
 *                                   is permitted in the search.
 */

#ifdef HPUX
keystore_epv installed_keystore[MAX_KEYSTORES] = {
    {SEC_PVTKEY_KEYSTORE_MAGIC,
     SEC_PVTKEY_SWITCH_FLAG_ENABLED
     | SEC_PVTKEY_SWITCH_FLAG_TERMINATE
     | SEC_PVTKEY_SWITCH_FLAG_MATCHALL,
     &sec_psm_file_mechanism,
     "Local file",
     sec_pvtkey_file_open,
     sec_pvtkey_file_close,
     sec_pvtkey_file_capabilities,
     sec_pvtkey_file_store,
     sec_pvtkey_file_delete,
     sec_pvtkey_file_generate,
     sec_pvtkey_file_get,
     sec_pvtkey_file_update,
     sec_pvtkey_file_update_generate} 
     
};
#else
keystore_epv installed_keystore[MAX_KEYSTORES] = {
    {SEC_PVTKEY_KEYSTORE_MAGIC,
     SEC_PVTKEY_SWITCH_FLAG_ENABLED
     | SEC_PVTKEY_SWITCH_FLAG_MATCHALL,
     &sec_psm_file_mechanism,
     "Local file",
     sec_pvtkey_file_open,
     sec_pvtkey_file_close,
     sec_pvtkey_file_capabilities,
     sec_pvtkey_file_store,
     sec_pvtkey_file_delete,
     sec_pvtkey_file_generate,
     sec_pvtkey_file_get,
     sec_pvtkey_file_update,
     sec_pvtkey_file_update_generate} ,

     {SEC_PVTKEY_KEYSTORE_MAGIC,
     SEC_PVTKEY_SWITCH_FLAG_ENABLED
     | SEC_PVTKEY_SWITCH_FLAG_TERMINATE,
     &sec_psm_pkss_mechanism,
     "DCE Private Key Storage Service",
     sec_pvtkey_pkss_open,
     sec_pvtkey_pkss_close,
     sec_pvtkey_pkss_capabilities,
     sec_pvtkey_pkss_store,
     sec_pvtkey_pkss_delete,
     sec_pvtkey_pkss_generate,
     sec_pvtkey_pkss_get,
     sec_pvtkey_pkss_update,
     sec_pvtkey_pkss_update_generate}, 

};
#endif
