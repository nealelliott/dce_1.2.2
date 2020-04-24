/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_test.h,v $
 * Revision 1.1.4.2  1996/02/18  23:04:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:17  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:14:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:46:27  root]
 * 
 * Revision 1.1.2.5  1994/06/21  14:29:04  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  07:49:23  marrek]
 * 
 * Revision 1.1.2.4  1994/06/10  20:19:44  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:45  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/10  15:33:30  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  10:24:08  marrek]
 * 
 * Revision 1.1.2.2  1994/03/23  16:06:50  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  18:14:00  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:16:24  marrek
 * 	Beautification.
 * 	[1994/02/09  15:45:29  marrek]
 * 
 * 	Created for dce1.1 code drop.
 * 	[1994/02/09  11:30:55  marrek]
 * 
 * $EndLog$
 */
#ifndef _XT_TEST_H
#define _XT_TEST_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xt_test_rcsid[] = "@(#)$RCSfile: xt_test.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:04:20 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


int
main ( int argc, char *argv[]);
OM_sint xt_init(OM_sint argc, char *argv[]);
OM_sint xt_usage(char *pgmname);
OM_sint xt_dinit(void);
extern OM_boolean xt_interrupt_enabled;
extern OM_boolean xt_interrupt_automatic;
extern char    xt_interrupt_delay_string[];/*delay before cancel/signal in us */
#endif /* ifndef _XT_TEST_H */
