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
 * $Log: driver.h,v $
 * Revision 1.1.8.2  1996/02/18  23:45:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:05  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:16:49  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:03 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:11:12  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:47:53  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:43:29  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:56:39  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:24:08  bfc]
 * 
 * Revision 1.1.4.2  1993/07/07  19:58:14  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:31:34  ganni]
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
**  NAME:
**
**      driver.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**  Definitions for main "driver" module.
**
**  VERSION: DCE 1.0
*/

#ifndef DRIVER_H
#define DRIVER_H


boolean DRIVER_main(
#ifdef PROTO
    int argc,
    char **argv
#endif
);

void nidl_terminate (
#ifdef PROTO
    void
#endif
);

#if defined(MIA) && (defined(VMS) || defined(__osf__) || (defined(_ALPHA_) && defined(_MSDOS)) || defined(DUMPERS))
#include <ast.h>
extern boolean DRIVER_open_lang_hdr(
#ifdef PROTO
    boolean     *cmd_opt,       /* [in] Array of command option flags */
    void        **cmd_val,      /* [in] Array of command option values */
    FILE        **h_fid,        /*[out] Header file handle */
    AST_interface_n_t *int_p    /* [in] Ptr to interface node */
#endif
);
#endif

#endif /* DRIVER_H */
