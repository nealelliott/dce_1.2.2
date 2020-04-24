/*
 *      SCCS:  @(#)ptab.h	1.2 (92/09/30) 
 *
 *	UniSoft Ltd., London, England
 *
 * (C) Copyright 1992 X/Open Company Limited
 *
 * All rights reserved.  No part of this source code may be reproduced,
 * stored in a retrieval system, or transmitted, in any form or by any
 * means, electronic, mechanical, photocopying, recording or otherwise,
 * except as stated in the end-user licence agreement, without the prior
 * permission of the copyright owners.
 *
 * X/Open and the 'X' symbol are trademarks of X/Open Company Limited in
 * the UK and other countries.
 */

/************************************************************************

SCCS:   	@(#)ptab.h	1.2 92/09/30 DTET release 1.0b
NAME:		ptab.h
PRODUCT:	DTET (Distributed Test Environment Toolkit)
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	per-process information table description

MODIFICATIONS:

************************************************************************/

/*
**	Per-process information table.
**
**	A process table element is allocated for each connection to
**	another process.
**	Storage for an element is allocated by ptalloc() and freed by ptfree().
**
**	Servers keep client process table elements in a global process table
**	which takes the form of a linked list.
**	An element is inserted in the table by ptadd() and removed by ptrm().
**	Clients sometimes keep server process table elements in
**	standard places rather than in the global process table.
**
**	For a process acting as a server, an element is allocated
**	automatically (by the transport-specific accept routine) each time an
**	incoming connection is accepted.
**	A pointer to the element is then passed to the server-specific
**	ss_newptab() function which may store it in some appropriate place -
**	usually in the global process table.
**
**	For a process acting as a client, an element must be allocated and
**	initialised before calling ts_connect().
**	ts_connect() then calls the client-specific ss_tsconnect() function
**	which should fill in transport-specific details such as the address
**	of the server process.
*/

/* remote process details structure */
struct remid {
	int re_sysid;		/* system id */
	long re_pid;		/* process id on that system */
	int re_ptype;		/* process type (in dtmsg.h) */
};

/*
**	process table structure - this is a linked list
**
**	the next and last pointers must be first so as to allow the
**	use of the llist routines to manipulate the table
*/

struct ptab {
	struct ptab *pt_next;		/* ptr to next element in list */
	struct ptab *pt_last;		/* ptr to prev element in list */
	long pt_magic;			/* magic number */
	int pt_state;			/* process state - see below */
	int pt_flags;			/* flags - see below */
	struct remid pt_rid;		/* remote sys/proc id */
	struct dtmsg pt_msg;		/* buffer for message i/o */
	int pt_mdlen;			/* size of pt_msg data buffer */
	int pt_savreq;			/* client saved request code */
	long pt_seqno;			/* client request sequence number */
	time_t pt_timeout;		/* timeout time */
	char *pt_tdata;			/* ptr to transport-specific data */
	char *pt_sdata;			/* ptr to server-specific data */
};

/* short names for pt_rid members */
#define ptr_sysid	pt_rid.re_sysid
#define ptr_pid		pt_rid.re_pid
#define ptr_ptype	pt_rid.re_ptype

/* short names for pt_msg members */
#define ptm_hdr		pt_msg.dm_hdr
#define ptm_magic	pt_msg.dm_magic
#define ptm_req		pt_msg.dm_req
#define ptm_rc		pt_msg.dm_rc
#define ptm_seq		pt_msg.dm_seq
#define ptm_sysid	pt_msg.dm_sysid
#define ptm_pid		pt_msg.dm_pid
#define ptm_ptype	pt_msg.dm_ptype
#define ptm_len		pt_msg.dm_len
#define ptm_data	pt_msg.dm_data

#define PT_MAGIC	0x50746142	/* ptab magic number */

/* process states */
#define PS_DEAD		1	/* dead process */
#define PS_IDLE		2	/* waiting for response from remote system */
#define PS_RCVMSG	3	/* receiving a message */
#define PS_SNDMSG	4	/* sending a message */
#define PS_PROCESS	5	/* processing a message */
#define PS_WAITSYNC	6	/* waiting for sync */
#define PS_CONNECT	7	/* connecting to remote system */

/* values for pt_flags (a bit field) */
#define PF_ATTENTION	00001		/* process needs servicing */
#define PF_INPROGRESS	00002		/* message i/o started */
#define PF_IODONE	00004		/* message i/o complete */
#define PF_IOERR	00010		/* error during message i/o */
#define PF_TIMEDOUT	00020		/* timeout expired */
#define PF_SERVER	00040		/* process is a server, not client */
#define PF_CONNECTED	00100		/* connected to server */
#define PF_LOGGEDON	00200		/* logged on */
#define PF_LOGGEDOFF	00400		/* logged off */
#define PF_RCVHDR	01000		/* receiving message header */
#define PF_NBIO		02000		/* using non-blocking i/o */
#define PF_SERVWAIT	04000		/* in si_servwait() */

/* timeout values for ts_poll() calls */
#define LONGDELAY	3600		/* if proc table is not empty */
#define SHORTDELAY	60		/* if proc table is empty */


/* extern function declarations */

#if __STDC__

extern void fiodead(struct ptab *);
extern struct ptab *getptbyptype(int, struct ptab *);
extern struct ptab *getptbysyspid(int, long);
extern struct ptab *getptbysysptype(int, int);
extern void op_fclose(struct ptab *);
extern void op_fopen(struct ptab *);
extern void op_gets(struct ptab *);
extern void op_puts(struct ptab *);
extern void ptadd(struct ptab *);
extern struct ptab *ptalloc();
extern void ptfree(struct ptab *);
extern void ptrm(struct ptab *);
extern char *rtoa(struct remid *);
extern void si_clientloop(struct ptab *, int);
extern void si_serverproc(struct ptab *);
extern void si_service(struct ptab *);
extern void si_servwait(struct ptab *, int);
extern void so_dead(struct ptab *);
extern int ti_logoff(struct ptab *, int);
extern int ti_logon(struct ptab *);
extern char *ti_msgbuf(struct ptab *, int);
extern int ti_talk(struct ptab *, int);

#else /* __STDC__ */

extern void fiodead();
extern struct ptab *getptbyptype();
extern struct ptab *getptbysyspid();
extern struct ptab *getptbysysptype();
extern void op_fclose();
extern void op_fopen();
extern void op_gets();
extern void op_puts();
extern void ptadd();
extern struct ptab *ptalloc();
extern void ptfree();
extern void ptrm();
extern char *rtoa();
extern void si_clientloop();
extern void si_serverproc();
extern void si_service();
extern void si_servwait();
extern void so_dead();
extern int ti_logoff();
extern int ti_logon();
extern char *ti_msgbuf();
extern int ti_talk();

#endif /* __STDC__ */

