/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dts_macros.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:54  marty]
 *
 * Revision 1.1.4.1  1995/12/11  22:45:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:21:26  root]
 * 
 * Revision 1.1.2.2  1994/10/18  18:51:32  agd
 * 	expand copyright
 * 	[1994/10/18  17:39:55  agd]
 * 
 * Revision 1.1.2.1  1994/09/29  21:02:52  bhaim
 * 	submit dlgcfg002 test
 * 	[1994/09/29  21:01:28  bhaim]
 * 
 * $EndLog$
 */
#define BAD_STATUS(stp)           ((*stp) != error_status_ok)
#define CLEAR_STATUS(stp)         ((*stp) = error_status_ok)
#define STATUS_EQUAL(stp, st_val) ((*stp) == (st_val))


