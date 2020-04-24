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
static char *RCSID_agt_stf_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_stf.c,v 1.1.2.2 1996/03/09 20:44:16 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_stf.c,v $
 * Revision 1.1.2.2  1996/03/09  20:44:16  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:10  marty]
 *
 * Revision 1.1.2.1  1996/02/16  05:44:12  dce-sdc
 * 	First Submit
 * 	[1996/02/16  05:43:45  dce-sdc]
 * 
 * Revision 1.1.2.4  96/01/12  04:56:03  04:56:03  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:55:43  kishi]
 * 
 * Revision 1.1.2.3  1995/10/26  01:09:47  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:09:40  kishi]
 * 
 * Revision 1.1.2.2  1995/09/23  09:50:53  kishi
 * 	Add header comment.
 * 	[1995/09/23  09:30:56  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:49:42  kishi
 * 	Initial revision
 * 	[1995/09/09  06:43:38  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/types.h>

#include "agt_agent.h"
#include "agt_errid.h"
#include "agt_stf.h"
#include "agt_msg.h"

static int agt__stat_write_line(int fd, char *bp, int sz);
static int agt__stat_read_line(int fd, char *bp, int sz);
static void agt__stat_lseek(int fd, off_t offset, int err_id);
static void agt__stat_lock(int fd, int err_id);
static void agt__stat_unlock(int fd, int err_id);


/**************************************************************************/
/* ID   = agt_stat_create_file						  */
/* name = Create operation information file 				  */
/* func = Open file specified in fn. If file does not exist, create with  */
/*        access mode specified in access				  */ 
/* i/o =								  */
/*	fn    :char *:IN:file name					  */
/*	omode :int   :IN:flag						  */
/*	access:mode_t:IN:file access mode				  */
/* return =								  */
/*	fd:file descriptor						  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_stat_create_file(char *fn, int omode, mode_t access)
{
    int fd;

open_retry:
    if ((fd = open(fn, omode, access)) == -1) {
	switch (errno) {
	case EACCES:
	case EAGAIN:
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02160, KDDS02160), strerror(errno));
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_UNKNOWN);
	    break;
	case EINTR:
	    goto open_retry;
	case ENFILE:
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02160, KDDS02160), strerror(errno));
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_OUTOFRSC);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%s,%i,%i", D_EID_20101, errno, fn, omode, access);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }

    return fd;
}


/**************************************************************************/
/* ID	= agt_stat_open_file						  */
/* name = Open operation information file 		    		  */
/* func = Open file specified in fn					  */
/* i/o =								  */
/*	fn    :char *:IN:file name					  */
/*	omode :int   :IN:flag						  */
/* return =								  */
/*	fd:file descriptor						  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_stat_open_file(char *fn, int omode)
{
    int fd;

open_retry:
    if ((fd = open(fn, omode)) == -1) {
	switch (errno) {
	case EACCES:
	case EAGAIN:
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02160, KDDS02160), strerror(errno));
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_UNKNOWN);
	    break;
	case EINTR:
	    goto open_retry;
	case ENFILE:
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02160, KDDS02160), strerror(errno));
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_OUTOFRSC);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%s,%i", D_EID_20102, errno, fn, omode);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }

    return fd;
}


/**************************************************************************/
/* ID	= agt_stat_add_entry						  */
/* name = Add entry information in operation information file		  */
/* func = Add agent process information in operation information file     */
/* i/o =								  */
/*	fd    :int   :IN:file descriptor of operation information file	  */
/*	id    :char *:IN:process identification				  */
/*	pid   :pid_t :IN:process-ID					  */
/*	dat   :char *:IN:date						  */
/*	tim   :char *:IN:time						  */
/*	remote:char *:IN:gateway name					  */
/*	user  :char *:IN:login user name				  */
/* return =								  */
/*	0 :normal end							  */
/*  -1:abnormal end							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_stat_add_entry(int fd, char *id, pid_t pid,
			char *dat, char *tim, char *remote, char *user)
{
#define TIME_MAX       15
    char buf[D_STAT_LINESZ+1];
    char tbuf[TIME_MAX + 1];
    char dbuf[TIME_MAX + 1];
    size_t timelen;
    time_t caltime;
    struct tm *loctime;


    if (fd == -1) {
	return;
    }


    if ((dat == NULL) || (tim == NULL)) {

	if ((caltime = time(NULL)) == -1) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_20103, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}
	loctime = localtime(&caltime);
	timelen = strftime(tbuf, TIME_MAX+1, "%H%M%S", loctime);
	timelen = strftime(dbuf, TIME_MAX+1, "%Y%m%d", loctime);

	if (tim != NULL) strcpy(tbuf, tim);
	if (dat != NULL) strcpy(dbuf, dat);
    } else {
	strcpy(tbuf, tim);
	strcpy(dbuf, dat);
    }

    sprintf(buf, "%s:%i:%s:%s:%s:%s\n", id, pid, dbuf, tbuf, remote, user);


    agt__stat_lock(fd, D_EID_20105);
    agt__stat_lseek(fd, -1, D_EID_20106);

    agt__stat_write_line(fd, buf, strlen(buf));

    agt__stat_unlock(fd, D_EID_20107);

}


/**************************************************************************/
/* ID   = agt_stat_delete_entry						  */
/* name = Delete entry information from operation information file	  */
/* func = Delete information from operation inforamtion file about stopped*/
/*        agent								  */
/* i/o =								  */
/*  fd :int  :IN:file descriptor of operation information file            */
/*  pid:pid_t:IN:process-ID						  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_stat_delete_entry(int fd, pid_t pid)
{
    char buf[D_STAT_LINESZ];
    int entrypid;
    ssize_t rlen;
    ssize_t rpos = 0;
    off_t wpos;
    int done_flag = D_FALSE;


    if (fd == -1) {
	return;
    }


    agt__stat_lock(fd, D_EID_20108);
    agt__stat_lseek(fd, 0, D_EID_20109);

    do {
	rlen = agt__stat_read_line(fd, buf, D_STAT_LINESZ);

	if (rlen > 0) {
	    switch (sscanf(buf, "%*c%*c%*c%*c:%i:", &entrypid)) {
	    case EOF:
		break;
	    case 1:
		if (entrypid == pid) {
		    wpos = rpos;
		    rpos += rlen;
		    while ((rlen =
			    agt__stat_read_line(fd, buf, D_STAT_LINESZ)) > 0) {

			agt__stat_lseek(fd, wpos, D_EID_20110);
			agt__stat_write_line(fd, buf, rlen);
			rpos += rlen;
			wpos += rlen;
			agt__stat_lseek(fd, rpos, D_EID_20111);
		    }
		    if (ftruncate(fd, wpos) == -1) {
			sprintf(msg.wkbuf, "%i", D_EID_20112);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02161, KDDS02161), "ftruncate", strerror(errno), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		    }
		    done_flag = D_TRUE;
		} else {
		    rpos += rlen;
		}
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i,%i,%i,%i", D_EID_20113, errno, rlen, rpos, wpos);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	}
    } while ((rlen > 0) && (!done_flag));

    agt__stat_unlock(fd, D_EID_20114);
}


/**************************************************************************/
/* ID	= agt__stat_write_line						  */
/* name = Write information on operation information file		  */
/* func = Write information specified in bp into the operation information*/
/*        file								  */
/* i/o =								  */
/*	fd:int   :IN:file descriptor of operation information file        */
/*  bp:char *:IN:information 						  */
/*  sz:int   :IN:sizez of bp						  */
/* return =								  */
/*	0 :normal end							  */
/*  -1:abnormal end							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__stat_write_line(int fd, char *bp, int sz)
{
    ssize_t wlen = 0;

    while (sz > 0) {
	if ((wlen = write(fd, bp, sz)) == -1) {
	    switch (errno) {
	    case EINTR:
		break;
	    case ENOSPC:
		sprintf(msg.wkbuf, "%s", agt.statfilename);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02161, KDDS02161), "write", strerror(errno), msg.wkbuf);
		AgentMsg(LOG_WARNING, msg.buf);
		agt_exit(D_EXIT_OUTOFRSC);
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i,%i", D_EID_20115, sz, wlen);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02161, KDDS02161), "write", strerror(errno), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	} else {
	    sz -= wlen;
	    bp += wlen;
	}
    }
}


/**************************************************************************/
/* ID   = agt__stat_read_line						  */
/* name = Read information from operation information file		  */
/* func = Read sz bytes from the operation inforamtion file		  */
/* i/o =								  */
/*	fd  :int   :file descriptor of operation information file         */
/*	bufp:char *:IN:buffer for the information to be read		  */
/*  sz  :int   :IN:size to be read					  */
/* return =                                                               */
/*	linelen:size read						  */
/*  0      :normal end							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__stat_read_line(int fd, char *bufp, int sz)
{
    ssize_t linelen = 0;
    ssize_t rlen;
    char *bp;
    int done = D_FALSE;

    if (sz <= 0) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_20116, sz);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }

    bp = bufp;

    do {
	switch (rlen = read(fd, bp, 1)) {
	case 1:
	    bp += rlen;
	    linelen += rlen;
	    if (*(bp-1) == '\n') {
		done = D_TRUE;
	    }
	    break;
	case 0:
	    break;
	default:
	    switch (errno) {
	    case EINTR:
		break;
	    default:
		sprintf(msg.wkbuf, "%i", D_EID_20117);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02161, KDDS02161), "read", strerror(errno), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	}
    } while (!((rlen == 0) || done));

    bufp[linelen] = '\0';

    return linelen;
}


/**************************************************************************/
/* ID	= agt__stat_lseek						  */
/* name = Seek information in operation information file		  */
/* func = Move read/write file pointer according to the offset specified  */
/* i/o =								  */
/*	fd    :int     :file descriptor of operation information file     */
/*  offset:off_t:IN:offset value					  */
/*  err_id:int  :IN:error-ID						  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__stat_lseek(int fd, off_t offset, int err_id)
{
    int rtn;

retry_lseek:
    if (offset == -1) {
	rtn = lseek(fd, 0, SEEK_END);
    } else {
	rtn = lseek(fd, offset, SEEK_SET);
    }

    if (rtn == -1) {
	switch (errno) {
	case EINTR:
	    goto retry_lseek;
	default:
	    sprintf(msg.wkbuf, "%i,%i", err_id, offset);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02161, KDDS02161), "lseek", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }
}

/**************************************************************************/
/* ID   = agt__stat_lock 						  */
/* name = Lock operation information file			 	  */	
/* func = Lock operation information file				  */
/* i/o =                                                                  */
/*  offset:off_t:IN:offset value					  */
/*  err_id:int  :IN:error-ID						  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__stat_lock(int fd, int err_id)
{
    struct flock lkdes;

    lkdes.l_type = F_WRLCK;
    lkdes.l_whence = SEEK_SET;
    lkdes.l_start = 0;
    lkdes.l_len = 0;

retry_setlkw:
    if (fcntl(fd, F_SETLKW, &lkdes) == -1) {
	switch (errno) {
	case EINTR:
	    goto retry_setlkw;
	case EMFILE:
	    sprintf(msg.wkbuf, "%s", agt.statfilename);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02161, KDDS02161), "fcntl", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_WARNING, msg.buf);
	    agt_exit(D_EXIT_OUTOFRSC);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%d", err_id,errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02161, KDDS02161), "fcntl", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }
}

/**************************************************************************/
/* ID   = agt__stat_unlock						  */
/* name = Unlock operation information file				  */
/* func = Unlock operation information file				  */
/* i/o =                                                                  */
/*  offset:off_t:IN:offset value					  */
/*  err_id:int  :IN:error-ID						  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__stat_unlock(int fd, int err_id)
{
    struct flock lkdes;

    lkdes.l_type = F_UNLCK;
    lkdes.l_whence = SEEK_SET;
    lkdes.l_start = 0;
    lkdes.l_len = 0;

retry_unsetlkw:
    if (fcntl(fd, F_SETLKW, &lkdes) == -1) {
	switch (errno) {
	case EINTR:
	    goto retry_unsetlkw;
	case EMFILE:
	    sprintf(msg.wkbuf, "%s", agt.statfilename);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02161, KDDS02161), "fcntl", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_WARNING, msg.buf);
	    agt_exit(D_EXIT_OUTOFRSC);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i", err_id);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02161, KDDS02161), "fcntl", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }
}


/**************************************************************************/
/* ID   = agt_stat_update_entry						  */
/* name = Update entry in operation information file			  */
/* func = Update entry in operation information file			  */
/* i/o =                                                                  */
/*	fd    :int      :file descriptor of operation information file    */
/*	pid   :pid_t :IN:process-ID					  */
/*	id    :char *:IN:process identification				  */
/*  remote:char *:IN:gateway name					  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_stat_update_entry(int fd, pid_t pid, char *id,
			   char *remote, char *user)
{
    char buf[D_STAT_LINESZ];
    int entrypid;
    ssize_t rlen;
    char *idstr;
    char *datestr;
    char *timestr;
    char *addrstr;
    char *userstr;
    int done_flag = D_FALSE;


    if ((fd == -1) ||
	((id == NULL) && (remote == NULL) && (user == NULL))) {


	return;
    }


    agt__stat_lock(fd, D_EID_20118);
    agt__stat_lseek(fd, 0, D_EID_20119);

    do {
	rlen = agt__stat_read_line(fd, buf, D_STAT_LINESZ);

	if (rlen > 0) {
	    switch (sscanf(buf, "%*c%*c%*c%*c:%i:", &entrypid)) {
	    case EOF:
		break;
	    case 1:
		if (entrypid == pid) {
		    idstr = strtok(buf, D_STAT_FS);
		    (void)strtok(NULL, D_STAT_FS);
		    datestr = strtok(NULL, D_STAT_FS);
		    timestr = strtok(NULL, D_STAT_FS);
		    addrstr = strtok(NULL, D_STAT_FS);
		    userstr = strtok(NULL, D_STAT_FS);

		    if (id != NULL) idstr = id;
		    if (remote != NULL) addrstr = remote;
		    if (user != NULL) userstr = user;

		    done_flag = D_TRUE;
		}
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i,%i", D_EID_20120, errno, rlen);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	}
    } while ((rlen > 0) && (!done_flag));

    agt__stat_unlock(fd, D_EID_20121);

    if (done_flag) {
	agt_stat_delete_entry(fd, pid);
	agt_stat_add_entry(fd, idstr, pid, datestr, timestr, addrstr, userstr);
    }
}


