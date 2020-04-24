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
 * $Log: rgy_sa_tclif.h,v $
 * Revision 1.1.6.3  1996/02/18  23:31:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:55  marty]
 *
 * Revision 1.1.6.2  1995/12/08  21:22:45  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/6  1995/09/11  14:09 UTC  truitt
 * 	Merge registry changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts46/1  1995/09/11  14:08 UTC  truitt
 * 	CHFts16100: Comprehensive clean-up of registry commands.
 * 
 * 	HP revision /main/HPDCE02/5  1995/05/15  17:24 UTC  truitt
 * 	Merge warranty patch work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts29/1  1995/05/15  17:23 UTC  truitt
 * 	Merge warranty patch work.
 * 
 * 	HP revision /main/HPDCE02/SEC_migrate_merge/2  1995/05/13  02:40 UTC  greg
 * 	Mergeout from HPDCE02 fto SEC_migrate_merge.
 * 
 * 	HP revision /main/HPDCE02/3  1995/03/30  17:47 UTC  truitt
 * 	Merge some warranty patch work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts23/1  1995/03/30  17:46 UTC  truitt
 * 	CHFts14675: New registry functionality to support warranty
 * 	patch work.
 * 
 * 	HP revision /main/HPDCE02/2  1994/12/20  14:13 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chk_03/1  1994/12/20  14:12 UTC  truitt
 * 	Remove duplicate definition.
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/08  17:29 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl2/1  1994/12/07  19:31 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl1/1  1994/12/05  14:48 UTC  hanfei
 * 	work for rgy checkpoint to be configurable
 * 	[1995/12/08  17:57:01  root]
 * 
 * Revision 1.1.2.7  1994/08/24  15:04:42  salamone
 * 	CR11838 - Add support for "registry set" command.
 * 	[1994/08/23  21:26:57  salamone]
 * 
 * Revision 1.1.2.6  1994/08/12  20:03:45  kevins
 * 	OT 9691 Add registry connect command
 * 	[1994/08/12  20:00:22  kevins]
 * 
 * Revision 1.1.2.5  1994/07/20  19:28:00  salamone
 * 	Add "registry verify" support.
 * 	[1994/07/20  19:18:19  salamone]
 * 
 * Revision 1.1.2.4  1994/06/20  21:03:00  salamone
 * 	Added "registry show" and "registry dump" support.
 * 	[1994/06/20  19:42:31  salamone]
 * 
 * Revision 1.1.2.3  1994/06/09  16:02:24  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:58  devsrc]
 * 
 * Revision 1.1.2.2  1994/06/02  15:55:10  salamone
 * 	Moved and renamed  rgy_modify() and rgy_show() functions in
 * 	rgy.c to rgy_sa_tclif.c
 * 	[1994/06/02  15:46:59  salamone]
 * 
 * Revision 1.1.2.1  1994/05/12  18:29:41  salamone
 * 	Initial version of rgy_sa_tclif.h
 * 	[1994/05/12  18:27:27  salamone]
 * 
 * $EndLog$
 */


/*
 * MODULE:
 * rgy_sa_tclif.h
 *
 * DESCRIPTION:
 * External declarations to support TCL interface for replication
 * information.
 *
 */

#ifndef _RGY_SA_TCLIF_H
#define _RGY_SA_TCLIF_H

/*
 * PROTOTYPES 
 */
int dcp_rgy_sa_catalog(ClientData, Tcl_Interp *, int, char **);
int dcp_rgy_sa_checkpoint(ClientData, Tcl_Interp *, int, char **);
int dcp_rgy_sa_connect(ClientData, Tcl_Interp *, int, char **);
int dcp_rgy_sa_delete(ClientData, Tcl_Interp *, int, char **);
int dcp_rgy_sa_designate(ClientData, Tcl_Interp *, int, char **);
int dcp_rgy_sa_destroy(ClientData, Tcl_Interp *, int, char **);
int dcp_rgy_sa_disable(ClientData, Tcl_Interp *, int, char **);
int dcp_rgy_sa_dump(ClientData, Tcl_Interp *, int, char **);
int dcp_rgy_sa_enable(ClientData, Tcl_Interp *, int, char **);
int dcp_rgy_sa_modify(ClientData, Tcl_Interp *, int, char **);
int dcp_rgy_sa_replace(ClientData, Tcl_Interp *, int, char **);
int dcp_rgy_sa_show(ClientData, Tcl_Interp *, int, char **);
int dcp_rgy_sa_stop(ClientData, Tcl_Interp *, int, char **);
int dcp_rgy_sa_synchronize(ClientData, Tcl_Interp *, int, char **);
int dcp_rgy_sa_verify(ClientData, Tcl_Interp *, int, char **);

#endif   /* _RGY_SA_TCLIF_H */
