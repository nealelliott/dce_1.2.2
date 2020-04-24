/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: msgqd.c,v $
 * Revision 1.1.90.2  1996/02/17  23:29:17  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:04  marty]
 *
 * Revision 1.1.90.1  1995/12/11  22:51:04  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/05  18:55 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1995/12/11  22:23:41  root]
 * 
 * Revision 1.1.87.1  1994/02/23  21:44:10  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:42:32  pellis]
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  15:56  bissen
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.8.2  1993/07/09  16:11:51  root
 * 	Initial King Kong branch
 * 	[1993/07/09  16:11:23  root]
 * 
 * Revision 1.1.6.2  1993/04/23  15:57:52  sekhar
 * 	Merged Hp_dce1_0_1 changes
 * 	[1993/04/16  15:00:13  sekhar]
 * 
 * Revision 1.1.1.7  1993/03/11  22:16:19  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.4.2  1993/02/05  15:28:36  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:49:29  cjd]
 * 
 * Revision 1.1.2.2  1992/03/18  21:06:08  eperkins
 * 	Corrected includes
 * 	[1992/03/18  21:05:43  eperkins]
 * 
 * Revision 1.1  1992/01/19  14:34:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *
 * FILE_NAME: msgqd.c
 *
 * COMPONENT_NAME: dce.systest
 *
 * FUNCTIONS:	writeMsg()		* macro *
 *		newMsgnode()		* macro *
 *		newWaiters()		* macro *
 *		newType()		* macro *
 *		keepMsgMem()		* macro *
 *		keepWaitMem()		* macro *
 *		freeMemory()		* macro *
 *		showStatus()
 *		allocMem()
 *		msgInQueue()
 *		addToMainMsgList()
 *		waiterInQueue()
 *		findType()
 *		getMsg()
 *		putMsg()
 *		processMsg()
 *		main()
 *
 * ORIGINS: 27
 *
 *
 * (C) COPYRIGHT International Business Machines Corp. 1991
 * All Rights Reserved
 *
 *
 * USAGE:
 *	msgqd <msgqname>
 *
 *	msgqname	The pathname for the socket the daemon will listen on.
 *
 *
 * DESCRIPTION:
 *      What is this file?  Give the design, if appropriate.
 *
 * HISTORY: 
 * 06/24/91	Wayne Mock	Initial design/code
 * 08/30/91	Wayne Mock	Updated the documentation
 * 11/06/91	Wayne Mock	Implemented the non-blocking functionality
 *				for message receive requests.
 */

#include <msgq.h>
#include <signal.h>
#include <dce/dce.h>

/*
 * This structure forms the main message list.  All received messages
 * that are not being waited for get placed in this list.  The list is
 * order FIFO with the next and prev pointers.  It is ordered by type with
 * the nextType pointer.
 */
struct msgnode
{	struct imessage imsg;		/* the message sent by a client */
	struct msgnode* next;		/* the next msg in FIFO order */
	struct msgnode* prev;		/* the previous msg in FIFO order */
	struct msgnode* nextType;	/* the next message of the same type
					 * as this message 
					 */
};

/*
 * This structure forms a FIFO ordered list of processes waiting to receive a 
 * message.  It is associated with a type node in the type node list described
 * below.  There would be one of these lists for every message type being 
 * waited on.
 */
struct waiters
{	int sockfd;		/* The socket on which the client expects to
				 * to receive the message.
				 */
	struct waiters* next;	/* next waiter in the list */
};

/*
 * This structure forms the list of message types that have been requested by
 * or sent to other users.  Each node points to the beginning and end of its
 * type list in the main message list.  It also may point to a list of
 * processes waiting on messages of this type.
 */
struct typenode
{	long type;			/* message type this node represents */
	struct msgnode* mHead;		/* head of the message list of type */
	struct msgnode* mLast;		/* tail of the message list of type */
	int nMsgs;			/* number of msgs in the list */
	struct waiters* wHead;		/* head of the waiter list of type */
	struct waiters* wLast;		/* tail of the waiter list of type */
	int nWaiters;			/* number of waiters in the list */
	struct typenode* next;		/* list these structs together */
};



struct typenode* tHead = NULL;		/* head of the type list */
struct typenode* tLast = NULL;		/* tail of the type list */
struct msgnode* freeMsg = NULL;		/* list of used message buffers */
struct waiters* freeWait = NULL;	/* list of used waiter buffers */
struct typenode mainL;			/* the main message list header,
					 * type = 0.
					 */
int curWaiters = 0;			/* current number of waiter procs */
int maxWaiting = 0;			/* max number of waiters durinf 
					 *lifetimer 
					 */
int curMsgs = 0;			/* current number of messages in the
					 * main list.
					 */
int totMsgs = 0;			/* total num of msgs handled during 
					 * lifetime 
					 */
int maxQueued = 0;			/* max num of queued messages during 
					 * lifetime 
					 */
int curMsgKeep = 0;			/* number of messages in the free list
					 */
int maxMsgKeep = 0;			/* the longest length of the free
					 * message list.
					 */
int curWaitKeep = 0;			/* number of free waiter blocks */
int maxWaitKeep = 0;			/* longest length of the free waiter
					 * list.
					 */
int curTypes = 0;			/* current number of types queued up */
int totTypes = 0;			/* total number of types processed 
					 * during lifetime 
					 */
int maxTypes = 0;			/* max num of active types during 
					 * lifetime 
					 */
int totMallocs = 0;			/* total number of mallocs() during 
					 * lifetime 
					 */
int totFrees = 0;			/* the number of times free() was 
					 * called
					 */


/*****************************************************************************
 * writeMsg()
 *
 * DESCRIPTION
 *	This macro calls the send() and evaluates the return code, prints
 *	an error message if appropriate and closes the socket.
 *
 * ASSUMPTIONS
 *	none.
 *
 * INPUT PARAMETERS
 *	sk	The socket descriptor of type int.
 *	bf	The buffer of info to write to the socket of type char*.
 *
 * OUTPUT PARAMETERS
 *	none.
 *
 * RETURN VALUE
 *	none.
 *
 * SIDE EFFECTS
 *	The socket, sk, is closed after this macro executes.
 *
 ****************************************************************************/
#define writeMsg(sk,bf) \
	{	int rc; \
		rc = send( sk, (char*)(bf), IMSGSIZLESSBUF+(bf)->usize, 0 ); \
		if( rc != IMSGSIZLESSBUF+(bf)->usize ) \
		{	msgnError( "send failed to write imsgbuf", errno ); \
		} \
		close( sk ); \
	}
/* end writeMsg() */


/*****************************************************************************
 * newMsgnode()
 *
 * DESCRIPTION
 *	This macro creates a new struct msgnode and initializes its members.
 *	If there are free messages in the freeMsg list one of those are removed
 *	otherwise a new node is malloced from dynamic memory.  The node is
 *	placed at the end of a list whose last member is pv.
 *
 * ASSUMPTIONS
 *	none.
 *
 * INPUT PARAMETERS
 *	mb	Buffer containing an internal message buf of type 
 *		struct imessage.
 *	pv	A pointer to the previous node in the list that the new node
 *		is being created.  Its type is struct msgnode
 *
 * OUTPUT PARAMETERS
 *	p	The value of the newly allocated pointer of type struct msdnode
 *
 * RETURN VALUE
 *	none.
 *
 * SIDE EFFECTS
 *	A node may be removed from the freeMsg list
 *
 ****************************************************************************/
#define newMsgnode(p,mb,pv) \
	{	if( freeMsg != NULL ) \
		{	p = freeMsg; \
			freeMsg = freeMsg->next; \
			curMsgKeep--; \
		} \
		else \
		{	p=(struct msgnode*)allocMem(sizeof(struct msgnode)); \
		} \
		p->imsg = mb; 		/* client message buffer */ \
		p->next = NULL; \
		p->prev = pv; 		/* prev node in a list */ \
		p->nextType = NULL; \
		totMsgs++; \
		curMsgs++; \
		if( curMsgs > maxQueued ) \
		{	maxQueued = curMsgs; \
		} \
	}
/* end newMsgnode() */


		
/*****************************************************************************
 * newWaiters()
 *
 * DESCRIPTION
 *	Creates a new struct waiters and initializes its members.  Either by
 *	by removing a node from the freeWait list or malloc()ing one from
 *	dynamic memory.
 *
 * ASSUMPTIONS
 *	none.
 *
 * INPUT PARAMETERS
 *	sk	The socket descriptor of type int.
 *
 * OUTPUT PARAMETERS
 *	p	The value of the newly create pointer of type struct waiters.
 *
 * RETURN VALUE
 *	none.
 *
 * SIDE EFFECTS
 *	A node may be removed from the freeWait list.
 *
 ****************************************************************************/
#define newWaiters(p,sk) \
	{	if( freeWait != NULL ) \
		{	p = freeWait; \
			freeWait = freeWait->next; \
			curWaitKeep--; \
		} \
		else \
		{	p=(struct waiters*)allocMem(sizeof(struct waiters)); \
		} \
		p->sockfd = sk; \
		p->next = NULL; \
		curWaiters++; \
		if( curWaiters > maxWaiting ) \
		{	maxWaiting = curWaiters; \
		} \
	}
/* end newWaiters() */


/*****************************************************************************
 * newType()
 *
 * DESCRIPTION
 *	Malloc()s a new node of type struct typenode and initializes its
 *	members.
 *
 * ASSUMPTIONS
 *	none.
 *
 * INPUT PARAMETERS
 *	t	The value of the type member of type long.
 *
 * OUTPUT PARAMETERS
 *	p	The value of the newly create pointer of type struct typenode
 *
 * RETURN VALUE
 *	none.
 *
 * SIDE EFFECTS
 *	none.
 *
 ****************************************************************************/
#define newType(p,t) \
	{	p=(struct typenode*)allocMem(sizeof(struct typenode)); \
		p->type = t; \
		p->mHead = NULL; \
		p->mLast = NULL; \
		p->nMsgs = 0; \
		p->wHead = NULL; \
		p->wLast = NULL; \
		p->nWaiters = 0; \
		p->next = NULL; \
		totTypes++; \
		curTypes++; \
		if( curTypes > maxTypes ) \
		{	maxTypes = curTypes; \
		} \
	}
/* end newType() */


/*****************************************************************************
 * keepMsgMem()
 *
 * DESCRIPTION
 *	Saves the structure pointer to by p in the freeMsg list.  The list
 *	is kept in LIFO order.
 *
 * ASSUMPTIONS
 *	none.
 *
 * INPUT PARAMETERS
 *	p	The node to append to the head of the freeMsg list.
 *		Its of type struct imessage.
 *
 * OUTPUT PARAMETERS 
 *	none.
 *
 * RETURN VALUE
 *	none.
 *
 * SIDE EFFECTS
 *	The freeMsg list will be one node longer.
 *
 ****************************************************************************/
#define keepMsgMem(p) \
	{	p->next = freeMsg; \
		freeMsg = p; \
		curMsgKeep++; \
		if( curMsgKeep > maxMsgKeep ) \
		{	maxMsgKeep = curMsgKeep; \
		} \
	}
/* end keepMsgMem() */


/*****************************************************************************
 * keepWaitMem()
 *
 * DESCRIPTION
 *	Saves the structure pointer to by p in the freeWait list.  The list
 *	is kept in LIFO order.
 *
 * ASSUMPTIONS
 *	none.
 *
 * INPUT PARAMETERS
 *	p	The node to append to the head of the freeWait list.
 *		Its of type struct imessage.
 *
 * OUTPUT PARAMETERS
 *	none.
 *
 * RETURN VALUE
 *	none.
 *
 * SIDE EFFECTS
 *	The freeWait list will be one node longer.
 *
 ****************************************************************************/
#define keepWaitMem(p) \
	{	p->next = freeWait; \
		freeWait = p; \
		curWaitKeep++; \
		if( curWaitKeep > maxWaitKeep ) \
		{	maxWaitKeep = curWaitKeep; \
		} \
	}
/* end keepWaitMem() */


/*****************************************************************************
 * freeMemory()
 *
 * DESCRIPTION
 *	Frees the dynamic memory point to by p and keeps track of how many
 *	times free() has been called.
 *
 * ASSUMPTIONS
 *	none.
 *
 * INPUT PARAMETERS
 *	p	The memory pointer to free.  It can be any type.
 *
 * OUTPUT PARAMETERS
 *	none.
 *
 * RETURN VALUE
 *	none.
 *
 * SIDE EFFECTS
 *	totFrees is incremented by one.
 *
 ****************************************************************************/
#define freeMemory(p) \
	{	free( (char*)p ); \
		totFrees++; \
	}
/* end freeMemory() */


/*****************************************************************************
 * showStatus()
 *
 * DESCRIPTION
 *	Prints out current statical information, flushes stdout and
 *	exits.
 *
 * ASSUMPTIONS
 *	Only called when the program is exiting.
 *
 * INPUT PARAMETERS
 *	int sig		The signal that caused this routine to be called.
 *
 * OUTPUT PARAMETERS
 *	none.
 *
 * RETURN VALUE
 *	none.
 *
 * SIDE EFFECTS
 *	If called, the program will exit.
 *
 ****************************************************************************/
void
showStatus( sig )
int sig;
{	printf( "\n\nmsgqd statistics:\n"
		"curMsgs\t\t= %d\n"
		"totMsgs\t\t= %d\n"
		"maxQueued\t= %d\n"
		"curWaiters\t= %d\n"
		"maxWaiting\t= %d\n"
		"curMsgKeep\t= %d\n"
		"maxMsgKeep\t= %d\n"
		"curWaitKeep\t= %d\n"
		"maxWaitKeep\t= %d\n"
		"curTypes\t= %d\n"
		"totTypes\t= %d\n"
		"maxTypes\t= %d\n"
		"totMallocs\t= %d\n"
		"totFrees\t= %d\n",
			curMsgs, totMsgs, maxQueued, curWaiters, maxWaiting, 
			curMsgKeep, maxMsgKeep, curWaitKeep, 
			maxWaitKeep, curTypes, totTypes, maxTypes, 
			totMallocs, totFrees );
	fflush( stdout );

	exit( 0 );

} /* end showStatus() */


/*****************************************************************************
 * allocMem()
 *
 * DESCRIPTION
 *	Allocate memory of the specified size.  If enough memory is not
 *	available, exit the program with an appropriate message.
 *
 * INPUT PARAMETERS
 *	unsigned int size	Number of bytes to allocate.
 *
 * OUTPUT PARAMETERS
 *	none.
 *
 * RETURN VALUE
 *	char* 			Points to the newly allocated space.
 *
 ****************************************************************************/
void*
allocMem
#ifdef __NO_PROTOTYPES__
( size )
unsigned size;
#else
(
unsigned size
)
#endif
{	char* tp;
	
	totMallocs++;

	tp = (char*)malloc( size );
	if( tp == NULL )
	{	msgfError( "msgqd: malloc", errno );
	}

	return (void*)tp;

} /* end allocMem() */


/*****************************************************************************
 * msgInQueue()
 *
 * DESCRIPTION
 *	Removes the first node of the message list in the given type node.
 *	If the node is the head of the main list, the main list's head 
 *	pointer is updated.
 *
 * ASSUMPTIONS
 *	none.
 *
 * INPUT PARAMETERS
 *	struct typenode* typebuf	The header for the message list to
 *					search.
 *
 * OUTPUT PARAMETERS
 *	none.
 *
 * RETURN VALUE
 *	struct msgnode*		   	The first node in the message list
 *					if it exists, or NULL if it does not.
 *
 * SIDE EFFECTS
 *	The first node in the message list for the type is removed.
 *
 ****************************************************************************/
struct msgnode*
msgInQueue( typebuf )
struct typenode* typebuf;
{	struct msgnode* tp;

	if( typebuf->mHead == NULL )
	{	return NULL;
	}

	tp = typebuf->mHead;
	typebuf->mHead = typebuf->mHead->nextType;
	typebuf->nMsgs--;

	if( tp->prev == NULL )
	{	mainL.mHead = tp->next;
	}
	else
	{	tp->prev->next = tp->next;
	}
	if( tp->next != NULL )
	{	tp->next->prev = tp->prev;
	}
	curMsgs--;
	return tp;

} /* end msgBeginWaitedFor() */


/*****************************************************************************
 * addToMainMsgList()
 *
 * DESCRIPTION
 *	Creates a new struct msgnode and adds it to the end of the main
 *	message list.  The list is kept in FIFO order.
 *
 * ASSUMPTIONS
 *	none.
 *
 * INPUT PARAMETERS
 *	struct imessage* imsgbuf	The client message to put in the list.
 *
 * OUTPUT PARAMETERS
 *	none.
 *
 * RETURN VALUE
 *	struct msgnode*			The newly allocated node.
 *
 * SIDE EFFECTS
 *	A new node is added to the end of the main message list.
 *
 ****************************************************************************/
struct msgnode* 
addToMainMsgList( imsgbuf )
struct imessage* imsgbuf;
{	struct msgnode* new;

	if( mainL.mHead == NULL )
	{	newMsgnode( mainL.mHead, *imsgbuf, NULL );
		mainL.mLast = mainL.mHead;
	}
	else
	{	newMsgnode( new, *imsgbuf, mainL.mLast );
		mainL.mLast->next = new;
		mainL.mLast = new;
	}

	return mainL.mLast;

} /* end addToMainMsgList() */


/*****************************************************************************
 * waiterInQueue()
 *
 * DESCRIPTION
 *	Determines if a process is waiting for a message of the given type.
 *	Returns a pointer to the node of the waiter and removes it from the
 *	waiters list.
 *
 * ASSUMPTIONS
 *	none.
 *
 * INPUT PARAMETERS
 *	struct typenode* typebuf	The info about the type the waiter
 *					might be waiting on.
 *
 * OUTPUT PARAMETERS
 *	none.
 *
 * RETURN VALUE
 *	struct waiters*			The node if a process is waiting;
 *					otherwise, NULL.
 *
 * SIDE EFFECTS
 *	A waiter node may be removed from the waiters list of the given type.
 *
 ****************************************************************************/
struct waiters*
waiterInQueue( typebuf )
struct typenode* typebuf;
{	struct waiters* tp;

	if( typebuf->wHead == NULL )
	{	return NULL;
	}

	tp = typebuf->wHead;
	typebuf->wHead = typebuf->wHead->next;
	typebuf->nWaiters--;
	curWaiters--;
	return tp;

} /* end waiterInQueue() */

/*****************************************************************************
 * findType()
 *
 * DESCRIPTION
 *	Searches the type list for a matching type.  If a match is found
 *	the node is returned; otherwise a new node is added to the end of
 *	the list and that is returned.
 *
 * ASSUMPTIONS
 *	none.
 *
 * INPUT PARAMETERS
 *	long type	The type of node to look for.
 *
 * OUTPUT PARAMETERS
 *	none.
 *
 * RETURN VALUE
 *	struct typenode*	The node that matches the given type.
 *
 * SIDE EFFECTS
 *	A new node may be added to the type list.
 *
 ****************************************************************************/
struct typenode*
findType( type )
long type;
{	struct typenode* tp = tHead;
/*
 * nothing in the list, so create the first node.
 */
	if( tHead == NULL )
	{	newType( tHead, type );
		tLast = tHead;
		return tHead;
	}
/*
 * Must be something in the list.
 *
 * Search the list until a match is found or the end is reached.
 */
	while( tp != NULL )
	{	if( tp->type == type )
		{	break;
		}
		tp = tp->next;
	}
/*
 * No match was found so create a new node.
 */
	if( tp == NULL )
	{	newType( tp, type );
		tLast->next = tp;
		tLast = tp;
	}

	return tp;

} /* end findType() */


/*****************************************************************************
 * getMsg()
 *
 * DESCRIPTION
 *	The server's reaction to a RCV request.  If a message of the requested
 *	type exists, it is sent to the process waiting to received the message;
 *	otherwise, if is placed in a waiting list that corresponds to the
 *	requested type.
 *
 * ASSUMPTIONS
 *	none.
 *
 * INPUT PARAMETERS
 *	int sockfd			The socket to write the message to.
 *	struct imessage* imsgbuf	The type of message to receive.
 *
 * OUTPUT PARAMETERS
 *	none.
 *
 * RETURN VALUE
 *	none.
 *
 * SIDE EFFECTS
 *	none.
 *
 ****************************************************************************/
void
getMsg
#ifdef __NO_PROTOTYPES__
( sockfd, imsgbuf )
int sockfd;
struct imessage* imsgbuf;
#else
(
int sockfd,
struct imessage* imsgbuf
)
#endif
{	struct typenode* cur;
	struct msgnode* msg;
	struct waiters* new;

	cur = findType( imsgbuf->umsg.type );
	msg = msgInQueue( cur );
	if( msg != NULL )	/* found a message to give user */
	{	writeMsg( sockfd, &msg->imsg );
		keepMsgMem( msg );
	}
	else 	/* the user does not want to wait for a message */
	if( imsgbuf->flags & IPC_NOWAIT )
	{	imsgbuf->usize = -1;	/* this indicates no message */
		writeMsg( sockfd, imsgbuf );
	}
	else	/* make the user wait until a message is available */
	{	if( cur->wHead == NULL )
		{	newWaiters( cur->wHead, sockfd );
			cur->wLast = cur->wHead;
		}
		else
		{	newWaiters( new, sockfd );
			cur->wLast->next = new;
			cur->wLast = new;
		}
		cur->nWaiters++;
	}

} /* end getMsg() */


/*****************************************************************************
 * putMsg()
 *
 * DESCRIPTION
 *	The servers reaction to a SEND request.  The entire message contents
 *	is read into the buffer from the socket.  If a process is waiting
 *	for the message it is sent to it directly; otherwise, it is 
 *	placed into the queue for later retrival.
 *
 * ASSUMPTIONS
 *	The first part of the message was already read.
 *
 * INPUT PARAMETERS
 *	int sockfd			The socket where the message is 
 *					received.
 *	struct imessage* imsgbuf	The first part of the message.
 *
 * OUTPUT PARAMETERS
 *	none.
 *
 * RETURN VALUE
 *	none.
 *
 * SIDE EFFECTS
 *	none.
 *
 ****************************************************************************/
void
putMsg
#ifdef __NO_PROTOTYPES__
( sockfd, imsgbuf )
int sockfd;
struct imessage* imsgbuf;
#else
(
int sockfd,
struct imessage* imsgbuf
)
#endif
{	int rc;
	struct typenode* cur;
	struct waiters* waiter;
	struct msgnode* curMsg;

/*
 * the rest of the message is read from the socket.
 */
	rc = recv( sockfd, (char*)&imsgbuf->umsg.buf, 
			imsgbuf->usize - sizeof( imsgbuf->umsg.type ) , 0 );
	if( rc != imsgbuf->usize - sizeof( imsgbuf->umsg.type ) )
	{	msgnError( "recv failed to read imsgbuf->umsg", errno );
	}

/*
 * get the node from the type list that corresponds to this type of message.
 */
	cur = findType( imsgbuf->umsg.type );

/*
 * LOOP:  WARNING this loop is very strange.  
 * 
 *	  It is used for error recovery.
 *
 * 	  If the server has a waiter proc that dies, the send() will fail
 *	  with the EPIPE error because there is no one to read the info.
 *	  If the situation occurs the loop repeats.  This continues until
 *	  a working waiter is found or the message is placed in the queue.
 */
	while( 1 )
	{	waiter = waiterInQueue( cur );
	/*
	 * If there was a process waiting for the message, send it to that 
	 * process.
	 */
		if( waiter != NULL )
		{	rc = send( waiter->sockfd, (char*)imsgbuf, 
					IMSGSIZLESSBUF + imsgbuf->usize, 0 );
			close( waiter->sockfd );
			if( rc == -1 && errno == EPIPE )
			{	continue;	/* try the next waiter */
			}		
			if( rc != IMSGSIZLESSBUF + imsgbuf->usize )
			{	msgnError( "send failed to write imsgbuf", 
								errno );
			}
			totMsgs++;
			keepWaitMem( waiter );
			break; /* found a working waiter, stop looping */
		}
		else
	/*
	 * otherwise, place it into the queue.
	 */
		{	curMsg = addToMainMsgList( imsgbuf );
			if( cur->mHead == NULL )
			{	cur->mHead = curMsg;
			}
			else
			{	cur->mLast->nextType = curMsg;
			}
			cur->mLast = curMsg;
			cur->nMsgs++;
			break; /* nobody waiting, stop looping */
		}
	}

} /* end putMsg() */


/*****************************************************************************
 * processMsg()
 *
 * DESCRIPTION
 *	Reads the client request from the socket.  If the user is sending
 *	the message is either delivered to someone waiting on it or it is
 *	placed in the queue.  If the user is receiving a message it is
 *	either given the message or waits until the message in-question
 *	arrives.
 *
 * ASSUMPTIONS
 *	none.
 *
 * INPUT PARAMETERS
 *	int sockfd	The socket that is connected to the client.
 *
 * OUTPUT PARAMETERS
 *	none.
 *
 * RETURN VALUE
 *	none.
 *
 * SIDE EFFECTS
 *	none.
 *	
 ****************************************************************************/
void
processMsg
#ifdef __NO_PROTOTYPES__
( sockfd )
int sockfd;
#else
(
int sockfd
)
#endif
{	int rc;
	struct imessage imsgbuf;

/* 
 * only read the first IMSGSIZLESSBUF bytes of the message.  This is the only
 * amount of info neede for a RCV request and the rest is read if it is a
 * SND request.
 */
	rc = recv( sockfd, (char*)&imsgbuf, IMSGSIZLESSBUF, 0 );
	if( rc != IMSGSIZLESSBUF )
	{	msgnError( "recv failed to read imsgbuf", errno );
	}

	switch( imsgbuf.action )
	{
	case MSGSND:
		putMsg( sockfd, &imsgbuf );
		close( sockfd );
		break;
	case MSGRCV:
		getMsg( sockfd, &imsgbuf );
		break;
	default:
		msgfError( "bad message action", 0 );
		break;
	}
#ifdef DEBUG
	printf( "messages=%d\t\twaiters=%d\n", curMsgs, curWaiters );
#endif
} /* end processMsg() */


/*****************************************************************************
 * main()
 *
 * DESCRIPTION
 *	Initializes the main message list, sets up some sig handlers,
 *	Creates a socket for communications then listens on the socket
 *	for client requests.
 *
 ****************************************************************************/
int
main
#ifdef __NO_PROTOTYPES__
( argc, argv )
int argc;
char* argv[];
#else
(
int argc,
char* argv[]
)
#endif
{	int sockfd, newsockfd, rc, svLen, clLen;
	struct sockaddr_un svAddr, clAddr;

	if( argc != 2 )
	{	msgfError( "usage: msgdq <msgqname>", 0 );
	}

	if( setpgrp( 0, getpid() ) == -1 )	/* detach from parent */
	{	(void)perror( "setpgrp" );
	}

	mainL.mHead = NULL;
	mainL.wHead = NULL;

/*
 * catch these signals for proper cleanup
 */
	signal( SIGHUP, showStatus );
	signal( SIGINT, showStatus );
	signal( SIGQUIT, showStatus );
	signal( SIGTERM, showStatus );

/*
 * Open a socket ( a UNIX domain stream socket )
 */
	sockfd = socket( AF_UNIX, SOCK_STREAM, 0 );
	if( sockfd < 0 )
	{	msgfError( "socket", errno );
	}
/*
 * Bind out local address so that the client can send to us.
 */
	bzero( (char*)&svAddr, sizeof( svAddr ) );
	svAddr.sun_family = AF_UNIX;
	strcpy( svAddr.sun_path, argv[1] );
	svLen = strlen( svAddr.sun_path ) + sizeof( svAddr.sun_family );
	rc = unlink( argv[1] );
 	rc = bind( sockfd, (struct sockaddr*)&svAddr, svLen );
	if( rc < 0 )
	{	msgfError( "bind", errno );
	}

/*
 * start to listen for client requests.
 */
	rc = listen( sockfd, 5 );
	if( rc < 0 )
	{	msgfError( "listen", errno );
	}

/*
 * Wait for a connection from a client process.
 */
	while( TRUE )
	{	
		clLen = sizeof( clAddr );
		newsockfd = accept( sockfd, (struct sockaddr*)&clAddr, &clLen);
		if( newsockfd < 0 )
		{	msgnError( "accept", errno );
			continue;
		}
		processMsg( newsockfd );
	}

} /* end main() */
		

