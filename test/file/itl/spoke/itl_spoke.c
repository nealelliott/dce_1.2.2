/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * HISTORY
 * $Log: itl_spoke.c,v $
 * Revision 1.1.314.1  1996/10/17  18:28:34  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:21  damon]
 *
 * Revision 1.1.309.1  1994/02/04  20:46:15  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:56  devsrc]
 * 
 * Revision 1.1.307.1  1993/12/07  17:45:19  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:23:01  jaffe]
 * 
 * $EndLog$
 */


#include <dce/rpc.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_utils.h>
#include <utils/cht_hash.h>
#include <utils/itl/defs.h>
#include <utils/itl/auto_spoke.h>
#include <utils/itl/itl_spoke.h>

typedef struct {
    char		* string_binding;
    handle_t		handle;
    cht_link_t		link;
} binding_entry_t;

typedef struct {
    pthread_t asyncThread;
    pthread_t joinThread;
    unsigned int waitTime;
    boolean_t completed;
} asyncJoinInfo_t;

    
#define	HASH_KEY(x)	((x)->string_binding)
#define	HASH_FUNC(x)	(BindingHash(x))
#define	HASH_EQUAL(x,y)	(strcmp(x,y) == 0)

PRIVATE int BindingHash ();
PRIVATE void BindingEnter _TAKES((char *, handle_t));

CHT_GENERATE_ROUTINES(PRIVATE, binding_, 32, binding_entry_t, link,
		      char *, HASH_KEY, HASH_FUNC, HASH_EQUAL)

#define	SET_RESULT(v)	    \
    itl_SetIntVariable("CommandResult", v, var_local)

#define GET_INT_ARG(name, val)			\
MACRO_BEGIN					\
    itl_PromptUser((name));			\
    (val) = itl_GetInteger(NULL);		\
MACRO_END

#define GET_STRING_ARG(name, val, lenP)		\
MACRO_BEGIN					\
    itl_PromptUser((name));			\
    (val) = (char *)itl_GetString(NULL, lenP);		\
MACRO_END				
    
int nhandles;
int nspokes;

boolean_t isHub;		/* true if ns_listen() is called */
boolean_t isSpoke;		/* true is ns_connect() && !isHub */ 

PRIVATE pthread_cond_t		itlSpokeCond;
PRIVATE pthread_mutex_t		itlSpokeMutex;

PRIVATE boolean_t itlSpoke_Cleanup();
PRIVATE void Spoke_DoCleanup();

itlSpoke_Listen(char *p)
{
    char 		* name;
    int 		len;
    int 		err;
    rpc_mgr_epv_t       epv; /* YUJS */

    if (p)
	name = p;
    else 
	GET_STRING_ARG("Connection Name: ", name, &len);

    err = ns_Listen(name, spoke_v1_0_s_ifspec, &epv);

    if (!err) {
	LOCK(&itlSpokeMutex);
	isHub = TRUE;
        itl_RegisterCleanupFun(Spoke_DoCleanup, name);
	UNLOCK(&itlSpokeMutex);
    }
    else {
        DCE_ERROR(err, "ns_Listen FAILED");
        SET_RESULT(1);
    }
    return FALSE;
}

itlSpoke_Connect(char * p)
{
    char 		* name;
    handle_t 		h;
    int 		err;
    int 		len;
    rpc_mgr_epv_t       epv;

    unsigned32          st;
    unsigned char       * string_binding;

    if (p)
	name = p;
    else 
	GET_STRING_ARG("Connection Name: ", name, &len);

    err = ns_Connect(name, spoke_v1_0_c_ifspec, &h);

    if (err) {
	DCE_ERROR(err, "ns_Connect FAILED");
	SET_RESULT(-1);
    } else {
	err = spoke_setup(h, spoke_v1_0_s_ifspec, &epv); 
	if (err) {
	    DCE_ERROR(err, "after call spoke_setup in itlSpoke_Connect FAILED");
	    SET_RESULT(-1);
	} else {
	    char var_buffer[1024];
	    unsigned char * str_binding;

	    rpc_binding_to_string_binding(h, (unsigned_char_p_t *)&str_binding, (unsigned32 *)&err);
	    if (err)
		DCE_ERROR(err, "rpc_binding_to_string_binding FAILED");
	    
	    LOCK(&itlSpokeMutex);

	    /* do the hub_handle[] thing */
	    sprintf(var_buffer, "ITL_hub[%d]", nhandles);
	    itl_SetStrVariable(var_buffer, (char *)str_binding, var_global);

	    SET_RESULT(nhandles);
	    nhandles++;

	    sprintf(var_buffer, "ITL_n_hub_handles", nhandles);
	    itl_SetIntVariable(var_buffer, nhandles, var_global);

	    isSpoke = (isHub) ? FALSE : TRUE;

	    BindingEnter((char *)str_binding, h);
	    UNLOCK(&itlSpokeMutex);
	}
    }
    return FALSE;    
}

void Spoke_DoCleanup(char *p)
{
    char *name;
    int len;

    if ( isHub == TRUE ) { 
      if (p)
         name = p;
      else 
	GET_STRING_ARG("Connection Name: ", name, &len);
      ns_Cleanup(name, spoke_v1_0_s_ifspec);
      auto_spokeCleanup(nspokes);
    }
    else
      itl_AdviseUser("Cleanup from HUB.\n");
}

/*
 *  itlSpoke_Cleanup()
 *    Here to satisfy the need for a ITL command function to return a
 *    boolean_t.  Some C compilers are being picky about this.
 */
boolean_t itlSpoke_Cleanup()
{
    Spoke_DoCleanup(NULL);
}

int itlSpoke_dce_login()
{
    char *princName;
    char *passwd;
    int len, ret;

    GET_STRING_ARG("Connection Name: ", princName, &len);
    GET_STRING_ARG("Passwd: ", passwd, &len);
    ret = itl_dce_login(princName, passwd);
    if ( ret ) { 
      DCE_ERROR(ret,"error: dce_login");
      return TRUE;
    }
    ITL_FREE(princName);
    ITL_FREE(passwd);
    return FALSE;
}

struct execute_args {
    handle_t		h;
    char		*str;
    char                *varName;
};

pointer_t ExecuteStringAsync(struct execute_args * args)
{
    error_status_t 	err;
    handle_t		h;
    unsigned32		result;

    err = spoke_execute_string(args->h, (unsigned char *) args->str, &result);
    if (err)
    {
	DCE_ERROR(err, "ExecuteStringAsync FAILED");
    }
    else
    {
	ITLU_ASSIGN_VARIABLE(args->varName, &result, itl_intType);	
    }

    ITL_FREE(args->varName);
    ITL_FREE(args->str);
    ITL_FREE(args);

    return((pointer_t) err);
}

ExecuteString(int async)
{
    char 		* p, * str, * varName;
    error_status_t	err;
    itl_var_t		* procVar;
    handle_t		h;
    int 		result;
    
    
    if (!(h = itlSpoke_GetHandle()))
	return 0;
    
    itl_PromptUser("Procedure name: ");
    p = itl_GetVariableName((boolean_t *)&err, NULL);
    procVar = itl_LocateVariable(p);
    if (!procVar) {
	itl_Error("No such variable: %s\n", p);
	ITL_FREE(p);
	return 0;
    }
    ITL_FREE(p);    
    itl_PromptUser("Result variable name: "); 
    varName = itl_GetVariableName((boolean_t *)&err, NULL);
    
    str = itl_RunnableVar(procVar);
    if (str) {
	if (async) {
	    pthread_t 		* threadP;
	    struct execute_args * args;
	    char                * asyncIdVar; 
	    int                 eof, wordSize;

	    itl_PromptUser("Async Id Variable Name: ");
	    /*
	     *  Check for the empty string ("").  If there then user is not
	     *  interested in keeping the async identifier.
	     */
	    asyncIdVar = itl_PeekString(&eof, &wordSize);
	    if (strcmp(asyncIdVar, ""))
	    {
		ITL_FREE(asyncIdVar);
		asyncIdVar = itl_GetVariableName((boolean_t *) &err, NULL);
	    }
	    else
	    {
		ITL_FREE(asyncIdVar);
		/*
		 *  Clear the "" from the input stream.
		 */
		asyncIdVar = itl_GetString(&eof, &wordSize);
	    }

	    ITL_NEW(args, struct execute_args);
	    ITL_NEW(threadP, pthread_t);
	    /*
	     *  Make a copy of the procedure string.  Otherwise a race
	     *  condition exists if this code is being executed in a loop,
	     *  that also defines a procedure.  The memory is free'd in the
	     *  ExecuteStringAsync code.
	     */
	    ITL_ALLOC(args->str, char *, (strlen(str) + 1));
	    args->h = h;
	    strcpy(args->str, str);
	    args->varName = varName;
	    if (pthread_create(threadP, pthread_attr_default,
			       (pthread_startroutine_t) ExecuteStringAsync,
			       args) == -1)
	    {
		itl_Error("pthread_create failed, errno = %d\n", errno);
	    }
	    else
	    {
		/*
		 *  If the user is un-interested in joining with this thread,
		 *  i.e. did not enter a variable name for the async id, then
		 *  detach the thread.  Memory is free'd below.
		 */
		if (!strcmp(asyncIdVar, ""))
		{
		    pthread_detach(threadP);
		}
		else 
		{
		    /*
		     *  Store the thead data address in an int variable, with
		     *  the name supplied by the user.
		     *  Yes, this is a hack.  ITL should support an address
		     *  variable.
		     *  The thread data will be de-allocated via the async_join
		     *  command.
		     */
		    ITLU_ASSIGN_VARIABLE(asyncIdVar, &threadP, itl_intType);
		}
	    }
	    if (!strcmp(asyncIdVar, ""))
	    {
		ITL_FREE(threadP);
		ITL_FREE(asyncIdVar);
	    }
	} else {

	    err = spoke_execute_string(h, (idl_char *) str, (unsigned32 *) &result);
	    
	    SET_RESULT(err);
	    if (err)
		DCE_ERROR(err, "ExecuteString FAILED");
	    else
		ITLU_ASSIGN_VARIABLE(varName, &result, itl_intType);

	    ITL_FREE(varName);
	}
    } else 
	itl_Error("The variable %s is not executable.\n", VAR_NAME(procVar));

    return FALSE;
}

JoinSyncPoint(int x)
{
    int			h;
    int			len;    
    char		* f;
    unsigned32		timeout;
    int			timedOut;
    int			err;
    binding_entry_t	* bvP;
    char		* hubHandleValue;
    itl_var_t		* hubHandleVar;
    
    if (isHub) {
	itl_Error("JoinSyncPoint not valid on Hub.\n");
	return FALSE;
    }
    if (!isSpoke) {
	itl_Error("JoinSyncPoint: don't know who the Hub is.\n");
	return FALSE;
    }
    
    GET_STRING_ARG("Syncpoint Name: ", f, &len);
    GET_INT_ARG("Timeout Value: ", timeout);

    hubHandleVar = itl_LocateVariable("ITL_hub[0]");
    if (!hubHandleVar) {
	itl_Error("Must execute connect before attempting to syncpoint\n.");
	return 0;
    }

    hubHandleValue = (char *) VAR_VALUE(hubHandleVar);

    bvP = binding_Lookup(hubHandleValue, CHT_LATCH_NONE);
    ASSERT(bvP);

    err = spoke_join_syncpoint(bvP->handle, f, timeout, &timedOut);
    if (err) 
	DCE_ERROR(err, "Syncpoint with Hub FAILED");

    if (!err && timedOut) {
	itl_Error(
	  "Waiting for Syncpoint \"%s\" exceeded timeout of %d seconds.\n",
		  f, timeout);
    }
    return FALSE;
}

/*
 *  AsyncJoinJoin() --
 *    This routine is called when an async_join is done with a valid timeout
 *    value, i.e. not -1.  It does the actual join to the thread started
 *    by rexec_async.
 */
PRIVATE pointer_t AsyncJoinJoin(arg)
  IN pthread_addr_t arg;
{
    pthread_addr_t result = 0;
    asyncJoinInfo_t *asyncInfo = (asyncJoinInfo_t *) arg;

    if (pthread_join(asyncInfo->asyncThread, &result) == -1)
    {
	itl_Error("pthread_join (AsyncJoinJoin) failed, errno = %d\n", errno);
	if (!result)
	    result = (pthread_addr_t) errno;
    }

    return((pointer_t) result);
}

/*
 *  AsyncJoinCancel() --
 *    This routine is called when an async_join is done with a valid timeout
 *    value, i.e. not -1.  It sleeps for the timeout value, and if the
 *    thread doing the join (see AsyncJoinJoin) is not completed it cancels.
 *    it.
 */
PRIVATE pointer_t AsyncJoinCancel(arg)
  IN pthread_addr_t arg;
{
    asyncJoinInfo_t *asyncInfo = (asyncJoinInfo_t *) arg;
    struct timespec sleepTime;

    sleepTime.tv_sec = asyncInfo->waitTime;
    sleepTime.tv_nsec = 0;
    pthread_delay_np(&sleepTime);
    if (!asyncInfo->completed)
	pthread_cancel(asyncInfo->joinThread);
    
    ITL_FREE(asyncInfo);
    return((pointer_t) 0);
}

/*
 *  JoinAsyncExec() --
 *    Joins with a thread that was created via the rexec_async command.
 *    It prompts for the value of a variable holding the thread data pointer.
 */
PRIVATE JoinAsyncExec(int x)
{
    pthread_t *threadP, *joinThreadP = (pthread_t *) 0;
    pthread_addr_t result = 0;
    int asyncId;
    unsigned32 timeout;
    asyncJoinInfo_t *asyncInfo;
    pthread_t joinThread, cancelThread;

    GET_INT_ARG("Async Identifier: ", asyncId);
    GET_INT_ARG("Timeout Value: ", timeout);
    threadP = (pthread_t *) asyncId;
    ASSERT(threadP);
    
    if (!isHub) {
	itl_Error("Async join not valid on a spoke.\n");
	return FALSE;
    }

    /*
     *  If the timeout value is -1 we wait for ever.
     */
    if (timeout == -1)
    {
	joinThreadP = threadP;
    }
    else
    {
	/*
	 *  Timed async wait.  (I know, sounds a little strange.)  Well
	 *  to accomplish this we do the following.
	 *
	 *    1) Start two threads.  One does the join to the thread
	 *       started by the call to rexec_async.  The second sleeps
	 *       the requested time.
	 *    2) This thread joins with the first thread in step one, i.e.
	 *       the one joining to the thread pointed to by threadP.
	 *
	 *  If the join to the thread in threadP is successful prior to the
	 *  timeout, then the second thread started in step one, just
	 *  wakes up and dies.  If the join does not complete, the second
	 *  thread started above wakes up and cancels the first thread
	 *  started in step one.
	 */
	ITL_NEW(asyncInfo, asyncJoinInfo_t);  /* free'd in AsyncJoinCancel */
	asyncInfo->completed = FALSE;
	asyncInfo->waitTime = timeout;
	asyncInfo->asyncThread = *threadP;
	if (pthread_create(&joinThread, pthread_attr_default,
			   (pthread_startroutine_t) AsyncJoinJoin,
			   (pthread_addr_t) asyncInfo) == 0)
	{
	    asyncInfo->joinThread = joinThread;
	    if (pthread_create(&cancelThread, pthread_attr_default,
			       (pthread_startroutine_t) AsyncJoinCancel,
			       (pthread_addr_t) asyncInfo) == 0)
	    {
		/*
		 *  Don't care what happens to the cancel thread.
		 */
		(void) pthread_detach(&cancelThread);
		joinThreadP = &joinThread;
	    }
	    else
	    {
		itl_Error("pthread_create (cancel thread) failed, errno = %d\n", errno);
	    }
	}
	else
	{
	    itl_Error("pthread_create (join thread) failed, errno = %d\n", 
		      errno);
	}
    }

    if (pthread_join(*joinThreadP, &result) == -1)
    {
	itl_Error("pthread_join failed, errno = %d\n", errno);
    }
    
    if ((int) result)
	itl_Error("Async join failed with error, %d\n", (int) result);

    return FALSE;
}

CreateAndWaitForSyncPoint(int x)
{
    int			expected;
    char		* f;
    int			len;
    unsigned32		timeout;
    int			timedOut;
    
    GET_STRING_ARG("Syncpoint Name: ", f, &len);
    GET_INT_ARG("Num Spokes: ", expected);    
    GET_INT_ARG("Timeout Value: ", timeout);

    spoke_create_syncpoint(f, expected, timeout, &timedOut);

    if (timedOut) {
	itl_Error(
	  "Creating syncpoint \"%s\" exceeded timeout of %d seconds.\n",
		  f, timeout);
    }
}

ReleaseSyncPoint(int x)
{
    char		* f;
    int			len;
    int			timedOut;
    
    GET_STRING_ARG("Syncpoint Name: ", f, &len);

    spoke_release_syncpoint(f, &timedOut);

    if (timedOut) {
	itl_Error("Releasing syncpoint \"%s\" timed out.\n", f);
    }
}

Print(int x)
{
    char 		* f;
    int			len;
    unsigned long	value;
    char		buffer[1024];
    
    GET_STRING_ARG("Format (printf format): ", f,  &len);

    GET_INT_ARG("Value to Print: ", value);

    sprintf(buffer,f,value);
    itl_Message("%s\n", buffer);
}

RemoteExit(int x)
{
    handle_t		h;
    
    if (h = itlSpoke_GetHandle())
	spoke_remote_exit(h);
}

PRIVATE itl_commandTable_t spoke_cmd_list[] = {
{
    "listen", itlSpoke_Listen, 0, FALSE, 2, 
    "listen for messages",
    "spoke", ""
    },

{
    "connect", itlSpoke_Connect, 0, FALSE, 2,
    "connect to server",
    "spoke", ""
    },

{
    "cleanup", itlSpoke_Cleanup, 0, FALSE, 2,
    "cleanup name space",
    "spoke", ""
},

{
    "rexec_sync", ExecuteString, 0, FALSE, 2, 
    "execute a string remotely (synchronous)",
    "spoke", "",
},

{
    "rexec_async", ExecuteString, 1, FALSE, 2, 
    "execute a string remotely (asynchronous)",
    "spoke", "",
},

{
    "sync_create", CreateAndWaitForSyncPoint, 0, FALSE, 2,
    "wait for all spokes to execute a join_sync command",
    "spoke", "",
},

{
    "sync_join", JoinSyncPoint, 0, FALSE, 2,
    "Pause execution util all spokes have joined this syncpoint",
    "spoke", "",
},

{
    "async_join", JoinAsyncExec, 0, FALSE, 2,
    "Wait for a previous rexec_async call to complete",
    "spoke", "",
},

{
    "sync_release", ReleaseSyncPoint, 0, FALSE, 2,
    "release all spokes to continue after syncpoint",
    "spoke", "",
},

{
    "print", Print, 0, FALSE, 2, 
    "print fmt variable",
    "spoke", "",
},

{
    "rexit", RemoteExit, 0, FALSE, 2,
    "cause a spoke to cease and desist",
    "spoke", "",
},

{
    "dce_login", itlSpoke_dce_login, 0, FALSE, 2,
    "login as a dfs user",
    "auto-spoke", "",
}
		
};

void SpokeConnectedCb(char * string_binding)
{
    unsigned char	var_buffer[100];
    int			n;
    int			st;
    handle_t		handle;
    
    rpc_binding_from_string_binding((unsigned_char_p_t) string_binding,
                   (rpc_binding_handle_t *) &handle, (unsigned32 *)&st);

    if (!st) {
	/* do the first call to prevent deadlocks */
	if ((st = spoke_remote_null(handle)) != rpc_s_ok) {
	    DCE_ERROR(st, "spoke_connected_cb: spoke_remote_null; FAILED");
            return;
	}
        itl_AdviseUser("SpokeConnectedCb %s\n", string_binding);
	
	LOCK(&itlSpokeMutex);

	/*
	 *  Set the comm timeout to be the maximum it can be.
	 */
        rpc_mgmt_set_com_timeout((rpc_binding_handle_t) handle, 
				 rpc_c_binding_max_timeout, 
				 (unsigned32 *) &st);
	if (st != rpc_s_ok) 
	    DCE_ERROR(st, "rpc_mgmt_set_com_timeout FAILED");
	
	sprintf((char *)var_buffer,"ITL_spoke[%d]", nspokes);
	itl_SetStrVariable((char *)var_buffer, string_binding, var_global);
	
	nspokes++;
	itl_SetIntVariable("ITL_nspokes", nspokes, var_global);

	BindingEnter(string_binding, handle);
	UNLOCK(&itlSpokeMutex);	
    }
    else  
        DCE_ERROR(st, "spoke_connected_cb: bad string binding; FAILED");
}
		
PRIVATE int BindingHash(char * s)
{
    char 		* cp, * dp;
    int			v = 0;

    /*
     * loop through string, looking for port number in [].  Once
     * found, compute a simple hash value based on the port #.
     */
    
    for (cp = s; *cp; cp++) {
	if (*cp == '[') {
	    for (; *cp != ']'; v += *cp++);
	    break;
	}
    }
    return v;
}

PRIVATE void BindingEnter(string_binding, handle)
  IN char *string_binding;
  IN handle_t handle;
{
    int 		st;
    binding_entry_t	* bP;

    if (!handle) {
	rpc_binding_from_string_binding((unsigned_char_p_t) string_binding, 
                  (rpc_binding_handle_t *) &handle,(unsigned32 *) &st);
	if (st) {
	    DCE_ERROR(st, "rpc_binding_from_string_binding FAILED");
	    return;
	}
    }

    ITL_NEW(bP, binding_entry_t);
    bP->string_binding = string_binding;
    bP->handle = handle;
    binding_Insert(bP, CHT_LATCH_NONE);
}

typedef struct {
    boolean_t (*func)();
    pointer_t *userDataP;
} forAll_data_t;

/*
 *  ForAllClients() --
 *    The purpose of this function is to hide the binding_entry_t data
 *    structure.
 */
PRIVATE boolean_t ForAllClients(bindingP, dataP)
  IN binding_entry_t *bindingP;
  IN pointer_t *dataP;
{
    forAll_data_t *userInfoP = (forAll_data_t *) dataP;

    return((userInfoP->func)(bindingP->handle, 
			     (pointer_t) bindingP->string_binding,
			     userInfoP->userDataP));
}

/*
 *  itlSpoke_ForAllClients() --
 *    This function will apply a caller supplied function against all
 *    known clients.
 *
 * ARGUMENTS:
 *  func - this function will be called with the rpc handle, an info data
 *         string (currently the string binding), and the user data.
 *  dataP - a pointer to any data you like.
 */
EXPORT boolean_t itlSpoke_ForallClients(func, dataP)
  IN boolean_t (*func)();
  IN pointer_t dataP;
{
    forAll_data_t userInfo;

    userInfo.func = func;
    userInfo.userDataP = dataP;

    return(binding_Forall(ForAllClients, (pointer_t) &userInfo, CHT_LATCH_BRACKET));
}

/*
 *  itlSpoke_GetHandleByString() --
 *    Get handle from a binding string for one of the spoke clients, and
 *    return it to the caller, after ensuring it is in the hash table.
 */
EXPORT handle_t itlSpoke_GetHandleByString(val)
    char *val;
{
    binding_entry_t *bindingP;
    handle_t returnHandle = NULL;

    LOCK(&itlSpokeMutex);
    bindingP = binding_Lookup(val, CHT_LATCH_NONE);

    if (bindingP)
	returnHandle = bindingP->handle;
    else
	itl_Error("Bad handle value %s\n", val);

    UNLOCK(&itlSpokeMutex);

    return(returnHandle);
}

/*
 *  itlSpoke_GetHandle() --
 *    Prompt the user for a handle for one of the spoke clients, and
 *    return it to the caller, after ensuring it is in the hash table.
 */
EXPORT handle_t itlSpoke_GetHandle()
{
    char *val;
    int handleLength;
    binding_entry_t *bindingP;
    handle_t returnHandle = NULL;

    GET_STRING_ARG("Handle: ", val, &handleLength);
    LOCK(&itlSpokeMutex);
    bindingP = binding_Lookup(val, CHT_LATCH_NONE);

    if (bindingP)
	returnHandle = bindingP->handle;
    else
	itl_Error("Bad handle value %s\n", val);

    UNLOCK(&itlSpokeMutex);

    return(returnHandle);
}

EXPORT void itlSpoke_AddCmds()
{

    pthread_cond_init(&itlSpokeCond, pthread_condattr_default);
    pthread_mutex_init(&itlSpokeMutex, pthread_mutexattr_default);
    
    spoke_mgr_init(SpokeConnectedCb);
    binding_Init();
    (void) itl_AddModuleCmds("spoke_client", spoke_cmd_list,
			sizeof(spoke_cmd_list)/sizeof(spoke_cmd_list[0]),
			     2, "hello world");
			     
}
