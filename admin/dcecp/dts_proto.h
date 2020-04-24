/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dts_proto.h,v $
 * Revision 1.1.6.2  1996/02/18  23:31:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:45  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:21:20  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/07  18:20 UTC  truitt
 * 	Add prototype for new clock function.
 * 	[1995/12/08  17:56:01  root]
 * 
 * Revision 1.1.4.3  1994/07/19  18:09:57  rousseau
 * 	Added 'dts catalog' and 'clock compare' (CR 11249 11251).
 * 	[1994/07/19  18:08:58  rousseau]
 * 
 * Revision 1.1.4.2  1994/06/09  16:02:10  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:41  devsrc]
 * 
 * Revision 1.1.4.1  1994/02/02  19:43:54  rousseau
 * 	Changed to support help functions.
 * 	[1994/02/02  19:40:16  rousseau]
 * 
 * Revision 1.1.2.3  1993/11/15  00:29:40  rousseau
 * 	Removed prototype for dts time command.
 * 	Added prototypes for clock commands.
 * 	[1993/11/15  00:29:35  rousseau]
 * 
 * Revision 1.1.2.2  1993/11/11  14:31:05  rousseau
 * 	Protected file from duplicate inclusion with #ifndef _DTS_PROTO_H
 * 	[1993/11/11  14:30:51  rousseau]
 * 
 * Revision 1.1.2.1  1993/11/09  18:26:28  rousseau
 * 	Initial version.
 * 	[1993/11/09  18:26:24  rousseau]
 * 
 * $EndLog$
 */


#ifndef _DTS_PROTO_H
#define _DTS_PROTO_H

/* Prototypes */
int dts_activate        (ClientData, Tcl_Interp *, int, char **);
int dts_configure       (ClientData, Tcl_Interp *, int, char **);
int dts_catalog         (ClientData, Tcl_Interp *, int, char **);
int dts_deactivate      (ClientData, Tcl_Interp *, int, char **);
int dts_modify          (ClientData, Tcl_Interp *, int, char **);
int dts_show            (ClientData, Tcl_Interp *, int, char **);
int dts_stop            (ClientData, Tcl_Interp *, int, char **);
int dts_synchronize     (ClientData, Tcl_Interp *, int, char **);

int clock_set           (ClientData, Tcl_Interp *, int, char **);
int clock_show          (ClientData, Tcl_Interp *, int, char **);
int clock_compare       (ClientData, Tcl_Interp *, int, char **);
int clock_synchronize   (ClientData, Tcl_Interp *, int, char **);

#endif
