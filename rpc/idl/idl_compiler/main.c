/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * @DEC_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: main.c,v $
 * Revision 1.1.8.2  1996/02/18  18:52:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:35  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:19:39  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/20  20:38 UTC  jrr
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/11/17  17:04 UTC  dat  /main/dat_xidl2/1]
 * 
 * 	oct 95 idl drop
 * 	[1995/10/23  00:44:03  bfc]
 * 	 *
 * 	may 95 idl drop
 * 	[1995/10/21  22:57:56  bfc]
 * 	 *
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:24:48  bfc]
 * 	 *
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/30  22:39 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:32 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/2  1994/10/28  18:56 UTC  mothra
 * 	ifdef hpdce_version.h so the setup pass which builds idl
 * 	does not try to use it.
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/03  16:32 UTC  tatsu_s
 * 	Merged mothra up to dce 1.1.
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/24  19:41  lmm
 * 	merge kk and hpdce01
 * 	[1995/12/07  21:12:15  root]
 * 
 * Revision 1.1.4.2  1993/07/07  20:00:46  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:33:11  ganni]
 * 
 * Revision 1.1.3.3  1993/05/26  14:16:41  tatsu_s
 * 	Removed HPDCE_VersionID.
 * 	Added #include <hpdce_version.h>.
 * 	[1993/05/26  14:13:41  tatsu_s]
 * 
 * Revision 1.1.3.2  1993/05/11  16:35:52  tatsu_s
 * 	Added HPDCE_VersionID.
 * 	[1993/05/11  16:30:19  tatsu_s]
 * 
 * 	Embedding copyright notice
 * 	[1993/01/03  14:36:06  bbelch]
 * 
 * $EndLog$
 */
/*
**  @OSF_COPYRIGHT@
**
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**
**  NAME
**
**      MAIN.C
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Mainline for the IDL compiler.
**
**  VERSION: DCE 1.0
**
*/

#include <nidl.h>
#include <driver.h>             /* Main driver defs */

long main (
#ifdef PROTO
    int argc, char **argv
#endif
);


long main
#ifdef PROTO
(
    int  argc,
    char **argv
)
#else
(argc, argv)
    int  argc;
    char **argv;
#endif
{
    if (!DRIVER_main(argc, argv))
        exit(pgm_error);

    exit (pgm_ok);

    return 1;
}
