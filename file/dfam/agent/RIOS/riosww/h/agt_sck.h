/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
 */
#ifdef HITACHI_REV_CHECK
# ifndef RCS_agt_sck_h_Included
# define RCS_agt_sck_h_Included
 static char *RCSID_agt_sck_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_sck.h,v 1.1.2.2 1996/03/11 14:06:14 marty Exp $";
# endif /* Not RCS_agt_sck_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_sck.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:14  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:58  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:31:35  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:31:07  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  05:53:13  05:53:13  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:53:03  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  02:35:01  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:34:52  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:38:48  kishi
 * 	Initial revision
 * 	[1995/09/09  07:20:32  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_SCK_H
#define _AGT_SCK_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>

#define D_LISTEN_MAX        5
#define D_SERVICE_ID        "dfa"
#define D_BIND_INTERVAL     15
#define D_BIND_MSG_INTERVAL 8


struct d_sck {
    int gw_fd;
    char *gwrcvdata;
    int gwrcvlen;
    int gwrcvbufsize;
    char *gwsnddata;
    int gwsndlen;
    int gwsndbufsize;

    int ux_fd;
    char *uxrcvdata;
    int uxrcvlen;
    int uxrcvbufsize;
    char *uxsnddata;
    int uxsndlen;
    int uxsndbufsize;

    char *myaddr;
    struct sockaddr_un from;
    int fromlen;
    struct sockaddr_un main_adr;
    int main_adrlen;
    struct sockaddr_un trace_adr;
    int trace_adrlen;
    struct sockaddr_un sysc_adr;
    int sysc_adrlen;
    struct sockaddr_un tmpl_adr;

    int tmpl_adrlen;


    int numremote;
    unsigned long *remoteip;
    int portnumber;
    unsigned long sdtimeout;
    unsigned long readtimeout;

    char *gwname;
    struct in_addr gwipaddr;
    int gwport;
    long gwconnid;
};

extern struct d_sck sck;

extern int agt_sck_open_inetsocket(struct sockaddr_in *ownaddr);
extern int agt_sck_open_unixsocket(char *ownaddr);

#endif
