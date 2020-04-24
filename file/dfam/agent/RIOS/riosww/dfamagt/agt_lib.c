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
static char *RCSID_agt_lib_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_lib.c,v 1.1.2.2 1996/03/09 20:44:05 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_lib.c,v $
 * Revision 1.1.2.2  1996/03/09  20:44:05  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:01  marty]
 *
 * Revision 1.1.2.1  1996/02/16  06:26:02  dce-sdc
 * 	First Submit
 * 	[1996/02/16  06:25:27  dce-sdc]
 * 
 * Revision 1.1.2.7  96/01/31  04:51:19  04:51:19  ichikawa (T.Ichikawa)
 * 	Delete Debug segment.
 * 	[1996/01/31  04:51:08  ichikawa]
 * 
 * Revision 1.1.2.6  1996/01/12  04:34:55  kishi
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:34:46  kishi]
 * 
 * Revision 1.1.2.5  1995/10/26  00:50:54  kishi
 * 	Change Copyright.
 * 	[1995/10/26  00:50:45  kishi]
 * 
 * Revision 1.1.2.4  1995/10/13  09:08:20  kishi
 * 	Add header comment.
 * 	[1995/10/13  09:08:10  kishi]
 * 
 * Revision 1.1.2.3  1995/09/23  09:53:11  kishi
 * 	Add header comment.
 * 	[1995/09/23  08:31:33  kishi]
 * 
 * Revision 1.1.2.2  1995/09/22  04:26:16  kishi
 * 	NULL -> 0.
 * 	[1995/09/22  04:26:05  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:52:50  kishi
 * 	Initial revision
 * 	[1995/09/09  06:34:47  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#undef exit
#undef abort
#undef unlink
#undef close


#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <syslog.h>
#include <sys/socketvar.h>

#include "agt_agent.h"
#include "agt_lib.h"
#include "agt_errid.h"
#include "agt_msg.h"
#include "agt_login.h"

extern struct d_login_info     user_info;

/**************************************************************************/
/* ID	= char_to_num							  */
/* name = Convert string into integer					  */
/* func = Convert strings into long integer data type			  */
/* i/o  =    				                                  */
/* 	c:char *:IN:pointer to the first character in the string	  */
/*	n:long  :IN:number of characters				  */
/* return =								  */
/*	long type data							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
long char_to_num(char *c, int n)
{
    int i;
    long num;

    for (num = i = 0; i < n; c++, i++) {
        num = (num << 8) + (*c & 0xff);
    }

    return num;
}


/**************************************************************************/
/* ID	= long_to_char							  */
/* name = Convert long integer data type into string			  */
/* func = Convert long integer data type into string			  */
/* i/o  =    				                                  */
/*	c:char *:OUT:converted string					  */
/*  n:long  :IN :long integer type data					  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void long_to_char(char *c, long n)
{
    int i;

    for (i = 3; i >= 0; i--) {
        c[i] = n & 0x000000ff;
	n >>= 8;
    }
}


/**************************************************************************/
/* ID	= short_to_char							  */
/* name = Convert short data type into string	         		  */
/* func = Convert short data type into string	         		  */
/* i/o  =    				                                  */
/* 	c:char *:IN:pointer to the first character in the string	  */
/*	n:long  :IN:number of characters				  */
/* return =								  */
/*	long type data							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void short_to_char(char *c, short n)
{
    int i;

    for (i = 1; i >= 0; i--) {
        c[i] = n & 0x000000ff;
	n >>= 8;
    }
}


/**************************************************************************/
/* ID	= GetIPaddr							  */
/* name = Get IP-address from host name					  */
/* func = Get IP-address based on host name of the machine executing Agent*/
/* i/o  =    				                                  */
/*	host:char *         :IN :host name				  */
/*	addr:unsigned long *:OUT:IP-address				  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int GetIPaddr(char *host, unsigned long  *addr)
{
    struct hostent *hostdata;

    hostdata = gethostbyname(host);
    if (hostdata == NULL) {

	*addr = inet_addr(host);
	if (*addr == 0xffffffff) {
	    return -1;
	} else {
	    return 0;
	}
    } else {
	memcpy((char *)addr, hostdata->h_addr, 4);
	if (*addr == 0xffffffff) {
	    return -1;
	} else {
	    return 0;
	}
    }
}



/**************************************************************************/
/* ID	= agt_get_gwname						  */
/* name = Get host name from IP-address					  */
/* func = Get host name based on the IP-address				  */
/* i/o  =    				                                  */
/*	ipaddr:struct in_addr:IN:structure containing IP-address	  */	
/* return =								  */
/*	p:host name							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
char *agt_get_gwname(struct in_addr ipaddr)
{
    struct hostent *hostdata;
    char *ntoap;
    char *p;

    hostdata = gethostbyaddr((char *)&ipaddr,
			     sizeof(ipaddr), AF_INET);
    if (hostdata == NULL) {
	ntoap = inet_ntoa(ipaddr);
	if (*ntoap == 0) {
	    sprintf(msg.wkbuf, "%i,%08x", D_EID_20401, ipaddr.s_addr);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}
	p = (char *)malloc(strlen(ntoap)+1);
	if (p == NULL) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_20402, (strlen(ntoap)+1));
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02500, KDDS02500),strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}
	strcpy(p, ntoap);
    } else {
	p = (char *)malloc(strlen(hostdata->h_name)+1);
	if (p == NULL) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_20403, (strlen(hostdata->h_name)+1));
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02500, KDDS02500),strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}
	strcpy(p, hostdata->h_name);
    }

    return p;
}


/**************************************************************************/
/* ID 	= GetLong							  */
/* name = Convert string into long integer				  */
/* func = Convert string data into long integer data type		  */
/* i/o  =    				                                  */
/* 	str    :char *:IN:pointer to the first character in the string	  */
/*	longint:long *:OUT:long type data variable			  */
/* return =								  */
/*	0 :normal end							  */
/*	-1:abnormal end(memory overflow)				  */
/*	-2:abnormal end(wrong input data)				  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int GetLong(char *str, long  *longint)
{
    long val;
    char *endp;

    val = strtol(str, &endp, 0);
    if ((val == LONG_MAX) && (errno == ERANGE)) {
	return -1;
    }
    if ((val == LONG_MIN) && (errno == ERANGE)) {
	return -1;
    }
    if (*endp != 0) {
	return -2;
    }
    *longint = val;
    return 0;
}

/**************************************************************************/
/* ID	= agt_bare_exit							  */
/* name = Terminating a process						  */
/* func = Terminate a process						  */
/* i/o  =    				                                  */
/*	status:int:IN:termination status				  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_bare_exit(int status)
{

    exit(status);

}


/**************************************************************************/
/* ID	= agt_exit							  */
/* name = Exiting a process						  */
/* func = Close file descriptor for socket and then end process. Kill     */
/*        child processes when closing main process                       */
/* i/o  =    				                                  */	
/*	status:int:IN:exiting status					  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_exit(int status)
{
    if (agt.mainprocpid == getpid()) {
	kill(-(agt.mainprocpid), SIGKILL);
    }

    memset((char *)&user_info, 0, sizeof(user_info));
    agt_sleep(1);

    close(sck.gw_fd);
    close(sck.ux_fd);
    unlink(sck.myaddr);
    exit(status);
}


/**************************************************************************/
/* ID	= agt_abort							  */
/* name = Aborting process						  */
/* func = Close file descriptor for socket and then abort process. Kill   */
/*        child processes when closing main process                       */
/* i/o  = none				                                  */	
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_abort(void)
{
    if (agt.mainprocpid == getpid()) {
	kill(-(agt.mainprocpid), SIGKILL);
    }

    memset((char *)&user_info, 0, sizeof(user_info));
    agt_sleep(1);

    close(sck.gw_fd);
    close(sck.ux_fd);
    unlink(sck.myaddr);
    abort();
}


/**************************************************************************/
/* ID	= agt_abort_on_signal						  */
/* name = Transmission of signal					  */
/* func = Send SIGILL,SIGBUS,SIGSEGV,SIGSYS signals. Abort process after  */
/*        writing transmission information in the syslog file. 		  */
/* i/o  =   				                                  */
/*	sig :int	      :IN:signal namber				  */
/*  code:int	      :IN:status code					  */
/*	scp :struct sigcontext:IN:signal information			  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_abort_on_signal(int sig, int code, struct sigcontext *scp)
{
    char wkmsgbuf[40];
    char msgbuf[200];

    sprintf(wkmsgbuf, "");
    sprintf(msgbuf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), wkmsgbuf);
    syslog(LOG_ERR, msgbuf);
    sprintf(msgbuf, "            (%i,%i)\n", D_EID_20043, sig);
    syslog(LOG_ERR, msgbuf);

    agt_abort();
}

/**************************************************************************/
/* ID   = agt_sleep							  */
/* name = Process sleep							  */
/* func = Change the status of the process to SLEEP 			  */
/* i/o =								  */
/*	sec:unsigned long:IN:process sleep time				  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_sleep(unsigned long sec)
{
    struct timeval wait_tv;

    wait_tv.tv_sec = sec;
    wait_tv.tv_usec = 0;


    select(0, (int *)0, (int *)0, (int *)0, &wait_tv);
}

/*****************************************************************************/
/* ID	= agt_do_nothing						     */
/*                                                                           */
/* name = Dummy function                                                     */
/* func = Just a dummy function that does not do nothing at all.             */
/*                                                                           */
/* i/o    = none                                                             */
/* return = none                                                             */
/* note   = none                                                             */
/* date   = October 12, 1995  by Tetsuya Ichikawa                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
void agt_do_nothing(void)
{



}
