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
static char *RCSID_agt_sck_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_sck.c,v 1.1.2.2 1996/03/09 20:44:12 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_sck.c,v $
 * Revision 1.1.2.2  1996/03/09  20:44:12  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:07  marty]
 *
 * Revision 1.1.2.1  1996/02/16  05:54:07  dce-sdc
 * 	First Submit
 * 	[1996/02/16  05:53:36  dce-sdc]
 * 
 * Revision 1.1.2.4  96/01/12  04:50:18  04:50:18  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:49:47  kishi]
 * 
 * Revision 1.1.2.3  1995/10/26  01:03:10  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:03:00  kishi]
 * 
 * Revision 1.1.2.2  1995/09/23  09:51:44  kishi
 * 	Add header comment.
 * 	[1995/09/23  09:10:36  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:51:26  kishi
 * 	Initial revision
 * 	[1995/09/09  06:40:26  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <syslog.h>
#include <sys/socketvar.h>

#include "agt_agent.h"
#include "agt_errid.h"
#include "agt_sck.h"
#include "agt_msg.h"
#include "agt_parm.h"



struct d_sck sck = {
    -1,
    NULL,
    0,
    0,
    NULL,
    0,
    0,

    -1,
    NULL,
    0,
    0,
    NULL,
    0,
    0,

    NULL,
    {AF_UNIX},
    0,
    {AF_UNIX},
    0,
    {AF_UNIX},
    0,
    {AF_UNIX},
    0,
    {AF_UNIX},
    0,

    0,
    NULL,
    D_DFLT_PORTNO,
    D_DFLT_TCPSDT,
    D_DFLT_TCPRDT
};


/**************************************************************************/
/* ID	= agt_sck_open_inetsocket					  */
/* name = Open socket for AF_INET and address assignment		  */
/* func = Open socket to be used to communicate with gateway. Then assign */
/*        to agent address						  */
/* i/o =								  */
/*	ownaddr:struct sockaddr_in *:IN:pointer to agent address	  */
/* return =								  */
/*	sockfd:file identification number				  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_sck_open_inetsocket(struct sockaddr_in *ownaddr)
{
    int sockfd;
    int bind_err_count = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
	switch (errno) {
	case ENOBUFS:
	    sprintf(msg.wkbuf, "%i,%s,%i", D_EID_20011, inet_ntoa(ownaddr->sin_addr), ownaddr->sin_port);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02560, KDDS02560), strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_OUTOFRSC);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%s,%i,%i", D_EID_20012, errno, inet_ntoa(ownaddr->sin_addr), ownaddr->sin_port);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02560, KDDS02560), strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }


bind_retry:
    if (bind(sockfd, ownaddr, sizeof(struct sockaddr_in)) == -1) {
	char myaddress[128];

	switch (errno) {
	case EADDRINUSE:
	    if (bind_err_count == 0) {
		sprintf(msg.wkbuf, "%i", D_EID_20013);
		sprintf(myaddress, "%s.%i", inet_ntoa(ownaddr->sin_addr), ownaddr->sin_port);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02561, KDDS02561), myaddress, strerror(errno), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
	    }
	    bind_err_count++;
	    if (bind_err_count >= D_BIND_MSG_INTERVAL) {
		bind_err_count = 0;
	    }
	    agt_sleep(D_BIND_INTERVAL);
	    goto bind_retry;
	default:
	    sprintf(msg.wkbuf, "%i", D_EID_20013);
	    sprintf(myaddress, "%s.%i", inet_ntoa(ownaddr->sin_addr), ownaddr->sin_port);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02561, KDDS02561), myaddress, strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}
    }

    return sockfd;
}


/**************************************************************************/
/* ID	= agt_sck_open_inetsocket					  */
/* name = Open socket for AF_INET and address assignment		  */
/* func = Open socket to be used to communicate with gateway. Then assign */
/*        to agent address						  */
/* i/o =								  */
/*	ownaddr:struct sockaddr_in *:IN:pointer to agent address	  */
/* return =								  */
/*	sockfd:file identification number				  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_sck_open_unixsocket(char *ownaddr)
{
    struct sockaddr_un socadr;
    int sockfd;
    int adrlen;
    int len = 0;

    unlink(ownaddr);

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd == -1) {
	switch (errno) {
	case ENOBUFS:
	    sprintf(msg.wkbuf, "%i,%s,%s", D_EID_20014, errno, ownaddr);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02560, KDDS02560), strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_OUTOFRSC);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%s", D_EID_20015, errno, ownaddr);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02560, KDDS02560), strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }

    socadr.sun_family = AF_UNIX;
    strcpy(socadr.sun_path,ownaddr);

    len = sizeof(socadr.sun_len) + sizeof(socadr.sun_family) + strlen(socadr.sun_path) + 1;
    socadr.sun_len = len;
    if (bind(sockfd,&socadr,len) == -1){
	sprintf(msg.wkbuf, "%i,%i", D_EID_20016,errno);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02561, KDDS02561), ownaddr, strerror(errno), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }

    if (sck.myaddr != NULL) {
	free(sck.myaddr);
    }

    adrlen = strlen(ownaddr) + 1;
    sck.myaddr = (char *) malloc(adrlen);
    if (sck.myaddr == NULL) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_20017, adrlen);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    } else {
	strcpy(sck.myaddr, ownaddr);
    }

    return sockfd;
}
