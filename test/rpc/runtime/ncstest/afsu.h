/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: afsu.h,v $
 * Revision 1.1.6.2  1996/02/18  23:07:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:51  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:14:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:47:27  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:55:02  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:22:49  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  13:48:56  rsalz
 * 	Added as part of rpc6 drop.
 * 	[1992/04/30  23:22:59  rsalz]
 * 
 * Revision 1.2  1992/01/19  22:14:12  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
 */
/*
 *  OSF DCE Version 1.0 
 */

#include <afs.h>

#define PIPE_BUF_SIZE       (8*1024)
#define PIPE_LENGTH         (8*PIPE_BUF_SIZE)

globalref NameString_t ncstest_NameString_arg;
globalref PathString_t ncstest_PathString_arg;
globalref afsHyper ncstest_afsHyper_arg;
globalref afsHyper ncstest_afsHyper_arg;
globalref afsFid ncstest_afsFid_arg;
globalref afsStoreStatus ncstest_afsStoreStatus_arg;
globalref afsVolSync ncstest_afsVolSync_arg;
globalref afsToken ncstest_afsToken_arg;
globalref afsBulkVVs ncstest_afsBulkVVs_arg;
globalref afsFetchStatus ncstest_afsFetchStatus_arg;
globalref afsBulkVolIDs ncstest_afsBulkVolIDs_arg;


extern int 
    ncstest_afsTimeval_diff(),
    ncstest_afsHyper_diff(),
    ncstest_afsUUID_diff(),
    ncstest_afsFid_diff(),
    ncstest_afsFetchStatus_diff(),
    ncstest_afsStoreStatus_diff(),
    ncstest_afsToken_diff(),
    ncstest_afsVolSync_diff(),
    ncstest_afsPathString_diff(),
    ncstest_afsNameString_diff(),
    ncstest_afsBulkVVs_diff(),
    ncstest_afsBulkVolIDs_diff();

extern void ncstest_afs_BulkVVs_3_init();
extern void ncstest_afs_BulkVolIDs_3_init();

extern void ncstest_afstest_init();
