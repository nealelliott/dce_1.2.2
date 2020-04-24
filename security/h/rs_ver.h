/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_ver.h,v $
 * Revision 1.1.11.1  1996/05/10  13:16:08  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:45 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/03/14  18:54 UTC  hanfei
 * 	define rs_c_software_vers_1_2_2 and increment rs_c_num_software_vers.
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:11 UTC  psn
 * 	Mergeout from HPDCE02 to SEC_migrate_merge.
 * 	[1995/05/13  02:41 UTC  greg  /main/SEC_migrate_merge/2]
 *
 * 	Merge for CHFts13996.
 * 	[1995/05/03  19:24 UTC  jrr  /main/HPDCE02/1]
 *
 * 	Fix secd's name string used for syslog entries (CHFts13996).
 * 	[1995/04/11  17:48 UTC  jrr  /main/jrr_sec_chfts13996/1]
 *
 * Revision 1.1.4.1  1994/07/15  14:59:42  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:16:59  mdf]
 * 
 * Revision 1.1.2.3  1993/02/26  23:48:30  emartin
 * 	em_bl6f: bump the version
 * 	[1993/02/22  22:51:10  emartin]
 * 
 * Revision 1.1.2.2  1992/12/29  13:25:46  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:15  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_ver.h V=4 04/29/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Version string constants
 * 
 */

#ifndef rs_version__h
#define rs_version__h

/*
 * What we call ourself when initializing kerberos
 * and writing syslog entries.
 */
static char      rs_syslog_name[]  = "secd";

/*
 * Unfortunately, the following need to be available to the client library
 * as well as the server process to rectify some pre-1.1 warranty 
 * patch software version weirdness.  The easy way out is to define
 * as static in the header file.  As this header file is included
 * in only a few places, we aren't chewing up too much redundant
 * storage.  We really would have preferred to put them in rs_ver_pvt.h.
 *
 * Things needed only by the server are in rs_ver_pvt.h.
 */

/* possible software versions 
 *
 * There should never be any need to remove 
 * any existing software version constants.
 * If, in the future, a server were no longer
 * to support "rs_c_software_vers_1_0_2" it is
 * necessary only to modify the definition of
 * "rs_c_min_software_vers" in rs_ver_pvt.h
 * appropriately and the software version code will
 * just "adapt".
 *
 * As of 1.1 the software versioning internals rely on the
 * the following.  (1) The defined software versions are 
 * monotonically increasing, and  (2) there are no
 * unused integers in the defined range of supported software
 * versions.
 */

#define rs_c_software_vers_unknown 0 
#define rs_c_software_vers_1_0_2   1   
#define rs_c_software_vers_1_1     2
#define rs_c_software_vers_1_2_2   3

/* total number of software revisions (including "unknown") */
#define rs_c_num_software_vers     4

static rs_sw_version_t   rs_1_0_2_version_id = "secd.dce.1.0.2";
static rs_sw_version_t   rs_1_1_version_id = "secd.dce.1.1";
static rs_sw_version_t   rs_1_2_2_version_id = "secd.dce.1.2.2";

#endif
