/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: dmn_aud_evtsp.h,v $
 * Revision 1.1.6.1  1996/07/08  18:25:06  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:23 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  22:57:30  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/18  22:57:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:23  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:38:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:17:32  root]
 * 
 * Revision 1.1.2.4  1994/10/04  21:23:23  weisz
 * 	CR 12167: api_filter test suite fails.
 * 	[1994/10/04  21:19:41  weisz]
 * 
 * Revision 1.1.2.3  1994/09/19  21:46:54  weisz
 * 	CR 12223: make client permissions checking consistent.
 * 	[1994/09/19  21:45:10  weisz]
 * 
 * Revision 1.1.2.2  1994/03/03  18:58:19  luan
 * 	CR 9773: Make consistent prior-inclusion checks in header files.
 * 	[1994/03/03  18:56:42  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:38:48  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:07  zeliff]
 * 
 * $EndLog$
 */

#ifndef _DMN_AUD_EVTSP_H
#define _DMN_AUD_EVTSP_H

#define AUDITD_AUD_FIRST_EVT_NUMBER 0x00000300
#define AUDITD_AUD_NUM_OF_EVTS      0x0000000b
#define EVT_DELETE_FILTER           0x300
#define EVT_SHOW_FILTER             0x301
#define EVT_LIST_FILTER             0x302
#define EVT_ADD_FILTER              0x303
#define EVT_REMOVE_FILTER           0x304

#define EVT_MODIFY_SSTRATEGY        0x305
#define EVT_MODIFY_STATE            0x306
#define EVT_REWIND                  0x307
#define EVT_STOP                    0x308

#define EVT_SHOW_SSTRATEGY          0x309
#define EVT_SHOW_STATE              0x30a

#endif /* _DMN_AUD_EVTSP_H */
