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
 * Revision 1.1.136.2  1996/02/18  22:57:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:10  marty]
 *
 * Revision 1.1.136.1  1995/12/08  00:22:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:52  root]
 * 
 * Revision 1.1.134.2  1994/06/10  20:54:59  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  15:00:25  devsrc]
 * 
 * Revision 1.1.134.1  1994/01/21  22:39:55  cbrooks
 * 	platform dependent NDR local representation
 * 	[1994/01/21  17:13:30  cbrooks]
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

#define NDR_LOCAL_INT_REP     ndr_c_int_little_endian
#define NDR_LOCAL_FLOAT_REP   ndr_c_float_ieee
#define NDR_LOCAL_CHAR_REP    ndr_c_char_ascii

#endif /* _NDR_REP_H */

/* 
 * HISTORY 
 * $Log: ndr_rep.h,v $
 * Revision 1.1.136.2  1996/02/18  22:57:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:10  marty]
 *
 * Revision 1.1.137.2  1996/02/18  22:16:10  marty
 * 	Update OSF copyright years
 *
 * Revision 1.1.136.1  1995/12/08  00:22:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:52  root]
 *
 * Revision 1.1.135.2  1995/12/08  00:00:52  root
 * 	Submit OSF/DCE 1.2.1
 *
 * Revision 1.1.134.2  1994/06/10  20:54:59  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  15:00:25  devsrc]
 *
 * Revision 1.1.135.2  1994/06/10  15:00:25  devsrc
 * 	cr10871 - fix copyright
 *
 * Revision 1.1.134.1  1994/01/21  22:39:55  cbrooks
 * 	platform dependent NDR local representation
 * 	[1994/01/21  17:13:30  cbrooks]
 *
 * Revision 1.1.1.2  1994/01/21  17:13:30  cbrooks
 * 	platform dependent NDR local representation
 *
 * $EndLog$
 */
