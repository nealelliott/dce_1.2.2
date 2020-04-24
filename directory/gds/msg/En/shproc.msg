$ 
$ @OSF_COPYRIGHT@
$ COPYRIGHT NOTICE
$ Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
$ ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
$ src directory for the full copyright text.
$ 
$ HISTORY
$ $Log: shproc.msg,v $
$ Revision 1.1.10.2  1996/03/09  23:14:12  marty
$ 	Update OSF copyright year
$ 	[1996/03/09  22:31:31  marty]
$
$ Revision 1.1.10.1  1995/12/08  16:04:42  root
$ 	Submit OSF/DCE 1.2.1
$ 	[1995/12/08  15:15:51  root]
$ 
$ Revision 1.1.8.3  1994/05/10  15:56:57  marrek
$ 	April 1994 code submission.
$ 	[1994/04/27  12:41:42  marrek]
$ 
$ Revision 1.1.8.2  1994/03/23  15:19:10  keutel
$ 	March 1994 code drop
$ 	[1994/03/21  16:32:54  keutel]
$ 
$ Revision 1.1.8.1  1994/02/22  17:41:22  marrek
$ 	Code drop for dce1.1.
$ 	[1994/02/08  10:57:17  marrek]
$ 
$ Revision 1.1.6.3  1993/08/11  15:09:04  marrek
$ 	July 1993 code drop.
$ 	[1993/08/02  13:02:18  marrek]
$ 
$ Revision 1.1.6.2  1993/07/02  15:09:56  marrek
$ 	Changes for port to HP800.
$ 	[1993/07/02  14:38:32  marrek]
$ 
$ Revision 1.1.4.4  1993/01/25  10:14:22  marrek
$ 	Add comment leader
$ 	[1993/01/14  15:22:00  marrek]
$ 
$ $EndLog$
$
$ $RCSfile: shproc.msg,v $ $Revision: 1.1.10.2 $ $Date: 1996/03/09 23:14:12 $
$
$set 1 preload              $ /* d20conf: Display Configuration */
$                             /* *sccsid = @(#)shproc.msg	7.3 91/07/22 (K Sys AP 11) */
1 DIRECTORY-ID  CONFIGURATION TYPE  SERVER-PROC  CLIENTS  UPDATE  AUTH-MECH
2 %s%-10.2s%-24.16s%-10.2s%-9.3s%-7.4s%-7s
3 DIR-ID  CONFIGURATION-TYPE  SERV-PROC CLIENTS UPDATE AUTH-MECH
4 %s%-10.2s%-22.16s%-9.2s%-7.3s%-6.4s%-7s
5 yes
6 no
7 SIMPLE
8 DCE
9 STRONG
10 SIM+DCE
11 SIM+STR

$set 2 preload              $ /* d20info */
1  The directory system is active (existing processes ->):
2  DUA cache process
3  C-stub process
4  S-stub process(es)
5  DSA process(es)
6  IPC monitoring process
7  The directory system isn't active !
8  Status of the 'trace' system: active
9  Status of the 'trace' system: inactive
