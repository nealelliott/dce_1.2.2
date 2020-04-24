/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY 
 * $Log: ndr_rep.h,v $
 * Revision 1.1.130.2  1996/02/18  22:57:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:04  marty]
 *
 * Revision 1.1.130.1  1995/12/08  00:22:41  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/02  18:23 UTC  tatsu_s
 * 	Merged HPDCE01 to Mothra.
 * 	[1995/12/08  00:00:47  root]
 * 
 * Revision 1.1.128.2  1994/06/10  20:54:53  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  15:00:19  devsrc]
 * 
 * Revision 1.1.128.1  1994/01/21  22:39:46  cbrooks
 * 	platform dependent NDR local representation
 * 	[1994/01/21  17:13:40  cbrooks]
 * 
 * $EndLog$
 */
/*
 * ndr_rep.h 
 * platform dependent (OS + Architecture) file split out from stubbase.h 
 * for DCE 1.1 code cleanup.
 * This file contains the architecture specific definitions of the 
 * local scaler data representation used 
 *
 * This file is always included as part of stubbase.h 
 */

#ifndef _NDR_REP_H 
#define _NDR_REP_H

#define NDR_LOCAL_INT_REP     ndr_c_int_big_endian
#define NDR_LOCAL_FLOAT_REP   ndr_c_float_ieee
#define NDR_LOCAL_CHAR_REP    ndr_c_char_ascii

/*
 * Machines on which 1, 2 and 4 byte fields are naturally aligned 
 * See ../idlddefs.h
 */
#define IDL_NATURAL_ALIGN_4

#endif /* _NDR_REP_H */
