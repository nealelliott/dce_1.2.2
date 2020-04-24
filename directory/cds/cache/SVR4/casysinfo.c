/* 
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 *(c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
*/

/*
 * HISTORY
 * $Log: casysinfo.c,v $
 * Revision 1.1.12.2  1996/02/18  19:23:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:53  marty]
 *
 * Revision 1.1.12.1  1995/12/08  00:10:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:22  root]
 * 
 * Revision 1.1.10.2  1994/05/12  21:10:36  peckham
 * 	Add CA_mappoint().
 * 	[1994/05/12  19:12:42  peckham]
 * 
 * Revision 1.1.10.1  1994/03/12  21:55:20  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:46:17  peckham]
 * 
 * Revision 1.1.8.1  1993/10/05  21:33:00  ohara
 * 	add a compiler error, this should not build on SVR4, its is a place holder for the real code.
 * 	[1993/10/05  19:40:14  ohara]
 * 
 * Revision 1.1.6.2  1993/06/24  19:46:13  hinman
 * 	[hinman] - Save merged-in SNI version (these files have been freed of nasty code)
 * 	[1993/06/17  14:22:09  hinman]
 * 
 * Revision 1.1.3.3  1992/12/30  12:35:59  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:18:49  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  19:07:40  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:08:55  weisman]
 * 
 * $EndLog$
 */


#ifdef SNI_SVR4_TAINTED
   !!!Error - Missing tainted code
#endif /* SNI_SVR4_TAINTED */

char *
CA_mappoint (void)
{
    return(0);
}
