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
 * $Log: aud_evtsp.h,v $
 * Revision 1.1.19.2  1996/02/18  23:12:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:37:17  marty]
 *
 * Revision 1.1.19.1  1995/12/08  18:13:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:25:26  root]
 * 
 * Revision 1.1.17.4  1994/06/10  20:52:30  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:48:54  devsrc]
 * 
 * Revision 1.1.17.3  1994/04/21  22:05:52  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:59:38  cbrooks]
 * 
 * Revision 1.1.17.2  1994/01/21  22:10:17  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/21  21:07:27  rsalz]
 * 
 * $EndLog$
 */

#ifndef _AUD_EVTSP_H
#define _AUD_EVTSP_H 1

#define DTS_AUD_FIRST_EVT_NUMBER	0x0200
#define EVT_CREATE_CMD			0x0200
#define EVT_DELETE_CMD			0x0201
#define EVT_ENABLE_CMD			0x0202
#define EVT_DISABLE_CMD			0x0203
#define EVT_UPDATE_CMD			0x0204
#define EVT_CHANGE_CMD			0x0205
#define EVT_SYNCHRONIZE_CMD		0x0206
#define EVT_ADVERTISE_CMD		0x0207
#define EVT_UNADVERTISE_CMD		0x0208
#define EVT_SET_DEFAULT_CMD		0x0209
#define EVT_SET_ATTR_CMD		0x020A
#define EVT_SHOW_ATTR_CMD		0x020B
#define EVT_SHOW_ALL_CHARS_CMD		0x020C
#define EVT_SHOW_ALL_STATUS_CMD		0x020D
#define EVT_SHOW_ALL_CNTRS_CMD		0x020E
#define EVT_SHOW_LOC_SERVERS_CMD	0x020F
#define EVT_SHOW_GBL_SERVERS_CMD	0x0210
#define EVT_CONTACT_PROVIDER		0x0211
#define EVT_REQUEST_PROVIDER_TIME	0x0212
#define DTS_AUD_NUM_OF_EVTS		19

#endif /* _AUD_EVTSP_H */
