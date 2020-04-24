/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * ID: $Id: itl_command.c,v 12.3 1995/10/19 21:00:40 blake Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 * FUNCTIONS:
 *    EXPORT   CmpTableCommands, HandleAproposCommand, PrintTableHelp, 
 *             itl_InitItl, itl_CleanupItl, itl_AddCmdTable, 
 *             itl_AddModuleCmds
 *    
 *    PRIVATE  PrintInfoRecord, PrintHelpTableEntry, PrintTableInfo, 
 *             PrintInfoMessage, HandleAssert, GetEnvCmd, HandleInfoCommand, 
 *             HandleEchoCommand, HandleNoopCommand, AproposMatchCommand, 
 *             MatchCommand, HandleBdeCommand, HandleSetVariable, 
 *             HandleShowVariable, HandleShellExec, LinkCmdTable, 
 *             MergeCmdTables, HashCmdTable
 *    
 *    extern   HandleSetItlTrace, HandleCrash, HandleTraceCommand, 
 *             HandleTraceHandleCommand
 *    
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1990, 1991
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 */

/*
 * Test Program:  itl_command.c
 *
 * Description:
 *     The code for the interface commands for the test program.
 *
 */


#include <string.h>   /* for strstr() in shellExec */
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

#ifdef ITL_FS_TEST
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/param.h>
#include <dcedfs/osi_net.h>
#endif /* ITL_FS_TEST */

#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_private.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_io.h>

extern int strncasecmp();

static char RCSid[] = "$Id: itl_command.c,v 12.3 1995/10/19 21:00:40 blake Exp $";

/**
 *  Help Info Structure
 **/
typedef struct {
    char *command;
    char *description;
    char *class;
    char *longDescr;
} itl_help_t;


EXPORT itl_commandTable_t * cmdStartP;
EXPORT itl_commandTable_t *commandIndex[26];
/*
 *  CommandMode - Each command defines the modes in which it is
 *  available - when matching a command name before execution or
 *  before providing help the command mode (defined in the table)
 *  is compared to this varaible (CommandMode).
 */
EXPORT unsigned long CommandMode = COMMAND_MODE_NORMAL;  /*  Default mode  */

PRIVATE itl_help_t *infoList = NULL;
PRIVATE int         infoSize = 0;

PRIVATE pthread_mutex_t itlInitMutex;

PRIVATE boolean_t itlIsInitialized = FALSE;

PRIVATE itl_commandTable_t *FindCommandEntry _TAKES((IN char *, IN boolean_t));

#ifdef ITL_FS_TEST

#define ITL_VAR_OS_TYPE         "ITL_SystemType"
#define ITL_VAR_PROCESS_ID      "ITL_ProcessId"
#define ITL_VAR_SYS_PAGE_SIZE   "ITL_SystemPageSize"
#define ITL_VAR_HOST_NAME       "ITL_HostName"

PRIVATE void SetFSTestingVariables _TAKES((void));

#endif  /* ITL_FS_TEST */

typedef struct cleanup_entry {
    void (*funP)();
    char * arg;
    struct cleanup_entry *next;
} cleanup_entry_t;

PRIVATE cleanup_entry_t *cleanupListP = NULL;

PRIVATE pthread_mutex_t itlCleanupMutex;

/* 
 * This routin is used to register the user defined cleanup function
 * as a node in the link list that will be executed by the function
 * itl_CleanupItl().
 */

EXPORT void itl_RegisterCleanupFun(funP, arg)
  IN void (*funP)();
  IN char * arg;
{
   cleanup_entry_t *newEntryP;

   ITL_NEW(newEntryP, cleanup_entry_t);
   newEntryP->funP = funP;
   newEntryP->arg = arg;
   pthread_mutex_lock(&itlCleanupMutex);
   newEntryP->next = cleanupListP;
   cleanupListP = newEntryP;
   pthread_mutex_unlock(&itlCleanupMutex);
}
/* 
 * itl_UnRegisterFun
 * It is just necessary to have Unregister function available
 * to deal with the case where cleaning up is harmful.
 */

EXPORT void itl_UnRegisterCleanupFun(funP, arg)
  IN void (*funP)();
  IN char * arg;
{
   int i;
   cleanup_entry_t *entryP, *tmpP;

   for(i=1,entryP=cleanupListP; entryP;i++,tmpP=entryP,entryP=entryP->next) {
      if ( entryP->funP == funP ) {
        pthread_mutex_lock(&itlCleanupMutex);
        if ( i == 1 ) { 
           cleanupListP = entryP->next;
        }
        else {
           tmpP->next = entryP->next;
        }
        pthread_mutex_unlock(&itlCleanupMutex);
        ITL_FREE(entryP); 
      }
   }
}


/*
 *  strcasestr: 
 *   returns a pointer to the first  occurrence  of  the
 *   pattern  string  s2  in  s1 ignorin case.   
 *   For example, if s1 is "string thing" and s2 is "ING",
 *   strcasestr() returns "ing thing".  If s2
 *   does not occur in s1, strstr() returns NULL.
 *   (See itl.h for declaration.)
 *
 */
SHARED char * strcasestr(str1, str2)
  IN char *str1;
  IN char *str2;
{
    int i;
    int len2 = (int) strlen(str2);
    int len1 = (int) strlen(str1);
    for (i=0; i <= (len1-len2); i++) {
	if (strncasecmp(str1+i, str2, len2) == 0) {
	    return(str1+i);
	}
    }
    return((char *)NULL);
}

#define HELP_LINE_CUTOFF 65
#define ITL_MAX_ALIAS_DEPTH 50

PRIVATE void PrintInfoString _TAKES((IN char *str));
PRIVATE void PrintInfoString(str)
  IN char *str;
{
    int i = 0;

    while (*str != '\0') {
	if (*str == '\n') {
	    fprintf(Cur_Out, "\n");
	    fflush(Cur_Out);
	    i = 0;
	} else if ((i > HELP_LINE_CUTOFF) && (*str == ' ') &&
		   (str[1] != ' ')) {
	    i = 0;
	    fprintf(Cur_Out, "\n");
	    fflush(Cur_Out);
	} else {
	    fprintf(Cur_Out, "%c", *str);
	    i++;
	}
	str++;
    }
    fprintf(Cur_Out, "\n");
    fflush(Cur_Out);
}

/*
 *  Print the help message in one entry of the command table.
 */
PRIVATE void PrintHelpTableEntry _TAKES((IN itl_commandTable_t *entry));
PRIVATE void PrintHelpTableEntry(entry)
  IN itl_commandTable_t *entry;
{
    if ((int) strlen(entry->command) < 16) {
	fprintf(Cur_Out, "%-15s - ", entry->command);
    } else {
	fprintf(Cur_Out, "%s - \n%18s", entry->command, " - ");
    }
    if (entry->function)
	fprintf(Cur_Out, "%s\n", entry->description);
    else
	fprintf(Cur_Out, "an alias for: %s\n", entry->description);
}


/*
 * Function: PrintTableInfo
 *  Print only those entries available in the current command mode
 *  and whose class contains the desired class.
 */
PRIVATE void PrintTableInfo _TAKES(( IN char *class));
PRIVATE void PrintTableInfo(class)
  IN char *class;
{
    itl_commandTable_t * tempCmdP;

    for(tempCmdP=cmdStartP; tempCmdP; tempCmdP = tempCmdP->nextCmdP){
	if ((tempCmdP->mode & CommandMode) &&
	    strcasestr(class, tempCmdP->class))
	    PrintHelpTableEntry(tempCmdP);
    }
    itl_Message("----------------------\n");
}


/*
 *  PrintInfoMessage:
 *    Print the info message associated with a specified topic.
 */
PRIVATE void PrintInfoMessage(topic)
  IN char *topic;
{
    int i;
    extern int strcasecmp();
    for (i=0; i < infoSize; i++) {
	if (strcasecmp(infoList[i].class, topic) == 0) {
	    PrintInfoString(infoList[i].description);
	}
    }
    itl_Message("\nThe commands related to %s include: \n", topic);
    PrintTableInfo(topic);
}



#define ITL_ELAPSED_TIME_CMD                                        \
{"elapsedTime", HandleElapsedTime, 0, FALSE,                        \
     COMMAND_MODE_ALL,                                              \
     "Return the elapsed time in msecs since the start of the test",\
     "misc",                                                        \
     ""}

/*
 *  Elapsed Time Command
 */
PRIVATE boolean_t HandleElapsedTime(param)
  IN int param;
{
    int elapsed = itlPrint_ElapsedTime();

    itl_AdviseUser("%d msecs have elapsed.\n", elapsed);
    itl_SetIntVariable("CommandResult", elapsed, var_local);

    return(FALSE);
}

#define ITL_ASSERT_CMD                                       \
{"assert", HandleAssert, 0, FALSE,                           \
     COMMAND_MODE_ALL,                                       \
     "Causes the program to exit if an expression is FALSE.",\
     "misc",                                                 \
     ""}

/*
 *  The assert command:
 *      Prompt for an integer expression and assert that it is not zero.
 */
PRIVATE boolean_t HandleAssert(param)
  IN int param;
{
    var_val_t *condVal;
    itl_PromptUser("Expression: ");
    condVal = itl_GetVarVal();
    if (!itl_VarValueToBoolean(condVal)) {
	itl_Message("Assertion failed, crashing!\n");
	itlIO_TraceCurrentLocation();
	itl_FreeVarVal(condVal);
	exit(5);
    }
    itl_FreeVarVal(condVal);
    return(FALSE);
}

#define SET_NAME(_name,_val) (void) itl_SetIntVariable(_name,_val,var_global)
PRIVATE void LoadTraceValues _TAKES((void));
PRIVATE void LoadTraceValues()
{
  static itlTraceValuesLoaded = FALSE;
  if (!itlTraceValuesLoaded) {
    SET_NAME("ITL_WARNING_TRACE", ITL_WARNING_TRACE);
    SET_NAME("ITL_DEBUG_TRACE",   ITL_DEBUG_TRACE);
    SET_NAME("ITL_VERBOSE_TRACE", ITL_VERBOSE_TRACE);
    SET_NAME("ITL_MEMORY_TRACE",  ITL_MEMORY_TRACE);
    SET_NAME("ITL_FILE_TRACE",	  ITL_FILE_TRACE);
    SET_NAME("ITL_VAR_TRACE",     ITL_VAR_TRACE);
    SET_NAME("ITL_LOCK_TRACE",    ITL_LOCK_TRACE);
    SET_NAME("ITL_IO_DEBUG",      ITL_IO_DEBUG);
    SET_NAME("ITL_IO_TRACE",      ITL_IO_TRACE);
    SET_NAME("ITL_EXP_TRACE",     ITL_EXP_TRACE);
    SET_NAME("ITL_SERVER_TRACE",  ITL_SERVER_TRACE);
    SET_NAME("ITL_ALL_TRACE",     ITL_ALL_TRACE);
    itlTraceValuesLoaded = TRUE;
  }
}
#undef SET_NAME

#define ITL_ADDITLTRACE_CMD				\
{"addItlTrace", HandleSetItlTrace, FALSE, FALSE,	\
     COMMAND_MODE_ALL,					\
     "add a trace to ITL's trace level.",		\
     "misc",						\
     ""}

#define ITL_SETITLTRACE_CMD				\
{"setItlTrace", HandleSetItlTrace, TRUE, FALSE,		\
     COMMAND_MODE_ALL,					\
     "set the trace level for ITL.",			\
     "misc",						\
     ""}

PRIVATE boolean_t HandleSetItlTrace _TAKES((IN int param));
PRIVATE boolean_t HandleSetItlTrace(param)
  IN int param;
{
    LoadTraceValues();
    itl_PromptUser("Tracing value: ");
    if (param)
	itl_traceLevel = GetInteger();
    else
	itl_traceLevel |= GetInteger();

    if (!isVerbose())
	itl_AdviseUser("Tracing is set to %x\n", itl_traceLevel);
    itl_Trace(ITL_VERBOSE_TRACE, "Tracing is set to %x\n", itl_traceLevel);
    return(FALSE);
}

#define ITL_INFO_CMD							\
{"info", HandleInfoCommand, 0, FALSE,					\
     COMMAND_MODE_ALL,							\
     "give help on a specific topic.",					\
     "misc",								\
     ""}

/*
 *  GetEnvCmd
 * 	get the value of an environment variable.
 */

PRIVATE boolean_t GetEnvCmd(param)
  IN int param;
{
    char *varName;
    itl_var_t *varP;
    int len;
    boolean_t eof;

    itl_PromptUser("Environment Variable's name: ");
    varName = itl_GetString(&len, &eof);
    varP = itl_GetEnv(varName, varName);
    itl_AdviseUser("%s = %s\n", VAR_NAME(varP), VAR_STR_VALUE(varP));
    ITL_FREE(varName);

    return(FALSE);
}
#define ITL_GETENV_CMD                        \
{"getEnv", GetEnvCmd, FALSE, FALSE,           \
     COMMAND_MODE_ALL,                        \
     "get a value of an environment variable",\
     "misc",                                  \
     ""}


PRIVATE boolean_t HandleInfoCommand(param)
  IN int param;
{
    char *topic;
    int len;
    boolean_t eof;

    itl_PromptUser("Topic (tran, var, misc): ");
    topic = itl_GetString(&eof, &len);
    if (eof)
	return(TRUE);
    PrintInfoMessage(topic);
    if ((strcasecmp(topic,"type") == 0) ||
	(strcasecmp(topic,"types") == 0)) {
	int i;
	/*
	 *  provide the list of all the types
	 */
	itl_Message("ITL currently supports the following value types:\n");
	for (i=0; ; i++) {
	    if (itlValue_TypeIsValid(i)) {
		fprintf(Cur_Out, "\t%s", itlValue_Type(i));
		if ((i+1)%5 == 0)
		    fprintf(Cur_Out, "\n");
	    } else {
		break;
	    }
	}
	fprintf(Cur_Out, "\n");
    }

    ITL_FREE(topic);
    return(FALSE);
}


#define ITL_CRASH_CMD			\
{"crash", HandleCrash, 0, TRUE,		\
     COMMAND_MODE_ALL,			\
     "Crash the test program quietly (without generating an error).", \
     "misc",				\
     ""}

boolean_t HandleCrash(param)
  IN int param;
{
    itl_Message("Test program is crashing %s (%d)...\n",
		!param ? "quietly": "with error",
		param); 
    exit(param);
    return(FALSE);
}


#define ITL_TRACE_CMD					\
{"itlTrace", HandleTraceCommand, 0, FALSE,			\
     COMMAND_MODE_ALL,					\
     "set the tracing level of the test program",	\
     "misc",						\
     ""}

#define ITL_TRACE(name)    \
        (void) setIntVariable(UTIL_STRING(name), name, var_global);

boolean_t HandleTraceCommand(param)
  IN int param;
{
    static boolean_t traceValuesLoaded = FALSE;
    if (!traceValuesLoaded){
	ITL_TRACE(ITL_DEBUG_TRACE);
	ITL_TRACE(ITL_VERBOSE_TRACE);
	ITL_TRACE(ITL_MEMORY_TRACE);
	ITL_TRACE(ITL_VAR_TRACE);
	ITL_TRACE(ITL_IO_TRACE);
	ITL_TRACE(ITL_IO_DEBUG);
	ITL_TRACE(ITL_EXP_TRACE);
	ITL_TRACE(ITL_EXCEPTION_TRACE);
	ITL_TRACE(ITL_ALL_TRACE);
	traceValuesLoaded = TRUE;
    }
    itl_PromptUser("Tracing level for the test program: ");
    itlIO_SetDebug(GetInteger());
    return(FALSE);
}


#define ITL_ECHO_COMMAND                                 \
{"echo", HandleEchoCommand, 0, FALSE,                    \
     COMMAND_MODE_ALL,                                   \
     "Echo the rest of this line interpreting variables",\
     "misc",                                             \
     ""}

#define ITL_COMMENT_COMMAND                                \
{"remark", HandleEchoCommand, 1, FALSE,                      \
     COMMAND_MODE_ALL,                                     \
     "Treat the rest of this line as a comment, ignore it",\
     "misc",                                               \
     ""}

#define ITL_ERROR_COMMAND                                          \
{"error", HandleEchoCommand, 2, FALSE,                             \
     COMMAND_MODE_ALL,                                             \
     "Report an error: the rest of this line is printed interpreting variables",\
     "misc",                                                       \
     "Report an error by printing the rest of the line interpreting\
variables.  This is similar to the echo command, however, it       \
sets the ITL error state which causes ITL to abort the current     \
script and exit to the upper most interactive level (or to         \
exit if called non-interactively)"}


PRIVATE boolean_t HandleEchoCommand(param)
  IN int param;
{
    char text[MAX_STRING_LENGTH];

    switch (param) {
      case 0:
      case 2:
        (void) itlIO_GetLine(text, NULL, NULL);
	if (param == 0)
	    itl_Message("%s\n", text);
	else
	    itl_Error("%s\n", text);

	break;
      case 1:
	itl_CommentLine();
	break;
    }
    return(FALSE);
}

#define ITL_WHILE_CMD					\
{"while", HandleNoopCommand, 0, TRUE,			\
     COMMAND_MODE_ALL,					\
     "Go back to the corresponding 'repeat' if the condition is TRUE", \
     "misc",						\
     ""}

#define ITL_QUIT_CMD					\
{"quit", HandleQuitCommand, 0, TRUE,			\
     COMMAND_MODE_ALL,					\
     "exit the current level of the test program",	\
     "misc",						\
     ""}

#define ITL_RETURN_CMD					\
{"return", HandleQuitCommand, 0, TRUE,			\
     COMMAND_MODE_ALL,					\
     "return from the current include or exec",		\
     "misc",						\
     ""}

#define ITL_ENDLOOP_CMD					\
{"endLoop", HandleNoopCommand, 0, TRUE,			\
     COMMAND_MODE_ALL,					\
     "marks the end of the loop body",			\
     "misc",						\
     ""}

PRIVATE boolean_t HandleNoopCommand(param)
  IN int param;
{
    return(FALSE);
}

PRIVATE boolean_t HandleQuitCommand(param)
  IN int param;
{
    itlIO_SetEarlyQuit(TRUE);
    return(FALSE);
}


PRIVATE int CmpTableCommands(ent1, ent2)
  IN const void *ent1, *ent2;
{
    extern int strcasecmp();
    return(strcasecmp(((itl_commandTable_t *)ent1)->command,
		      ((itl_commandTable_t *)ent2)->command));
}

#define COM_APROPOS_NO_MATCH    0
#define COM_APROPOS_CLASS_MATCH 1
#define COM_APROPOS_NAME_MATCH  2
#define COM_APROPOS_MATCH       3
/*
 *  AproposMatchCommand
 *  Match a command name for help purposes:
 *
 *  Return:
 *    COM_APROPOS_NO_MATCH - if there is no match.
 *    COM_APROPOS_CLASS_MATCH -  if the command appers in the class
 *		list of the entry
 *    COM_APROPOS_NAME_MATCH - if the command appears as a substring in
 *		the entrie's command name.
 *    COM_APROPOS_MATCH - if the command appears in the help message.
 */
PRIVATE int AproposMatchCommand(command, entry)
  IN char *command;
  IN itl_commandTable_t *entry;
{
    if (strcasestr(entry->command, command))
	return(COM_APROPOS_NAME_MATCH);
    else if (strcasestr(entry->class, command))
	return(COM_APROPOS_CLASS_MATCH);
    else if (strcasestr(entry->description, command))
	return(COM_APROPOS_MATCH);
    else
	return(COM_APROPOS_NO_MATCH);
}

#define COMMAND_NO_MATCH 0
#define COMMAND_PARTIAL_MATCH 1
#define COMMAND_FULL_MATCH 2
/*
 *  MatchCommand - Given a command name and a command entry,
 *  return:
 *     COMMAND_NO_MATCH - if there is no match
 *     COMMAND_PARTIAL_MATCH - if there is a partial match
 *     COMMAND_FULL_MATCH - it it is a full match
 */
PRIVATE int MatchCommand(command, entry)
  IN char *command;
  IN itl_commandTable_t *entry;
{
    int comLen = (int) strlen(command);
    /*
     *  If the command is not available in this mode, return no match
     */
    if ((comLen > (int) strlen(entry->command)) ||
	!(entry->mode & CommandMode))
	return(COMMAND_NO_MATCH);

    if (strncasecmp(command, entry->command, comLen) == 0) {
	if (comLen == (int) strlen(entry->command))
	    return(COMMAND_FULL_MATCH);
	else
	    return(COMMAND_PARTIAL_MATCH);
    } else {
	return(COMMAND_NO_MATCH);
    }
}

#define ITL_APROPOS_CMD					\
{"apropos", HandleAproposCommand, 0, FALSE,		\
     COMMAND_MODE_ALL,					\
     "lookup commands containing a specified string",	\
     "misc",						\
     ""}

/*
 *  HandleAproposCommand
 *    check the specified string against each of the commands in the
 *    command table and list all the ones that math.
 *   
 */
EXPORT boolean_t HandleAproposCommand(param)
  IN int param;
{
    itl_commandTable_t * tempCmdP;
    char *command;
    boolean_t eof;
    int len;
    boolean_t headerPrinted = FALSE;

    itl_PromptUser("apropos string: ");
    command = itl_GetString(&eof, &len);

    for(tempCmdP = cmdStartP; tempCmdP; tempCmdP = tempCmdP->nextCmdP){
	if ((tempCmdP->mode & CommandMode) &&
	    (AproposMatchCommand(command, tempCmdP)
	    != COM_APROPOS_NO_MATCH)) {
	    if (!headerPrinted)
		itl_Message("The following commands match %s\n", command);
	    headerPrinted = TRUE;
	    PrintHelpTableEntry(tempCmdP);
	}
    }
    itlExp_ShowVariables(command);
    ITL_UTIL_FREE(command);
    return(FALSE);
}

#define ITL_SLEEP_CMD						\
{"sleep", HandleDceCommand, 1, FALSE,				\
     COMMAND_MODE_ALL,						\
     "make the thread sleep for a number of miliSeconds.",	\
     "misc",							\
     ""}
     
#define ITL_YIELD_CMD						\
{"yield", HandleDceCommand, 0, FALSE,				\
     COMMAND_MODE_ALL,						\
     "yield the process by calling pthread_yield.",			\
     "misc",							\
     ""}

PRIVATE boolean_t HandleDceCommand(param)
  IN int param;
{
    struct timespec sleepTime;
    int mSecs;

    switch(param) {
      case 0:
	itl_Trace(ITL_IO_TRACE, "Yielding the processor\n");
	itl_AdviseUser("Yielding the processor.\n");
	pthread_yield();
	break;
      case 1:
	itl_PromptUser("Time (in milli-seconds): ");
	mSecs = GetInteger();
	sleepTime.tv_sec = mSecs / 1000;
	sleepTime.tv_nsec = (mSecs - (sleepTime.tv_sec * 1000)) * 1000;
	itl_Trace(ITL_IO_TRACE,
		  "Sleeping for %d secs and %d mSecs\n",
		  sleepTime.tv_sec, (sleepTime.tv_nsec / 1000));
	pthread_delay_np(&sleepTime);
	break;
      default:
	break;
    }
    return(FALSE);
}

#define ITL_SETVAR_CMD              \
{"set", HandleSetVariable, 0, FALSE,\
     COMMAND_MODE_ALL,              \
     "set a script variable",       \
     "var",                         \
     ""}

#define ITL_FETCHOP_CMD                                     \
{"fetch&op", HandleSetVariable, 1, FALSE,                   \
     COMMAND_MODE_ALL,                                      \
     "Perform a fetch & Op  operation on a script variable",\
     "var",                                                 \
     ""}

#define ITL_TESTANDSET_CMD                                   \
{"test&set", HandleSetVariable, 2, FALSE,                    \
     COMMAND_MODE_ALL,                                       \
     "Perform a test-and-set operation on a script variable",\
     "var",                                                  \
     ""}

PRIVATE boolean_t ReplaceFieldCmd _TAKES((
  IN  var_val_t *inValP,
  IN  char *fieldSpec
));
PRIVATE boolean_t ReplaceFieldCmd(inValP, fieldSpec)
  IN  var_val_t *inValP;
  IN  char *fieldSpec;
{
    var_val_t *newValP, *curValP;
    char *stringVal;
    int type;

    curValP = itlExp_GetNestedField(inValP, fieldSpec);
    if (!curValP)
	return(FALSE);

    itl_PromptUser("Type or Value for %s: ", fieldSpec);
    newValP = itl_GetVarVal();
    stringVal = ((VALUE_TYPE(newValP) == itl_stringType) ?
		 STR_VALUE_VAL(newValP) :
		 NULL);

    /*
     *  Get the type of the variable.
     *  If it is one of the known types, use the appropriate
     *  routine.  Otherwise, use the default.
     */
	    
    if (stringVal &&
	((type = itlValue_TypeByName(stringVal)) != -1)) {
      /*
       *  The token we read was a type definition.
       *  Call the type's read function to get the value.
       */
      itl_FreeVarVal(newValP);
      newValP = itlValue_Read(NULL, type);
    }

    itlExp_SetNestedField(inValP, fieldSpec, newValP);
    itl_FreeVarVal(newValP);
    itl_FreeVarVal(curValP);
    return(TRUE);
}

PRIVATE boolean_t HandleSetVariable(mode)
  IN int mode;
{
    char *varName;
    boolean_t varGlobal = VAR_GLOBAL_BY_DEFAULT;
    scope_t scope;
    boolean_t err;
    int len;
    boolean_t success = FALSE;
    extern int strcasecmp();
  
    itl_PromptUser("Variable Name: ");
    varName = itl_GetVariableName(&err, NULL);
    if (err) {
	itl_Error("EOF encountered while looking for a var name\n");
	return(TRUE);
    }

    itl_Trace(ITL_DEBUG_TRACE,
	      "%s variable using variable name %s\n",
	      (mode == 0 ? "set" :
	       (mode == 1 ? "fetch&Op" : "test&Set")), varName);

    if ((mode == 1) || (mode == 2)) {
	char *Op;
	var_val_t *value;
	char *dotLoc;

	itl_PromptUser("Operation: ");
	Op = itl_GetWord(&err, &len);

	dotLoc = strchr(varName, '.');
	itl_PromptUser("Value: ");
	value = itl_GetVarVal();

	if (mode == 1) {
	    if (itlExp_FetchAndOp(varName, value, Op)) {
		success = TRUE;
	    } else {
		itl_Error("Fetch&Op %s on var %s with value of type %s failed.\n",
			  Op,
			  varName,
			  itlValue_Type(VALUE_TYPE(value)));
	    }
	}
	else {
	    var_val_t *newValue;
	    itl_PromptUser("New Value: ");
	    newValue = itl_GetVarVal();
	    if (itlExp_TestAndSet(varName, value, Op, newValue)) {
		itl_AdviseUser("The variable %s was set.\n", varName);
		itl_SetIntVariable("CommandResult", TRUE, var_local);
		success = TRUE;
	    }
	    else {
		itl_SetIntVariable("CommandResult", FALSE, var_local);
	    }
	    itl_FreeVarVal(newValue);
	}

	itl_FreeVarVal(value);
	ITL_FREE(Op);
    }
    else {
	char *dotLocation = strchr(varName, '.');

	if (dotLocation) {
	    /*
	     *  We are setting one field of an existing variable
	     */
	    itl_var_t *varP = itl_LocateVariable(varName);

	    if (!varP) {
		itl_Error("Cannot assign a field of a nonexistent variable: %s\n",
			  varName);
	    } else {
		var_val_t *curValP;
		if (!itlExp_SetPermitted(varP, var_same)) {
		  success = FALSE;
		} else {
		  curValP = VAR_VALUE_REC(varP);
		  success = ReplaceFieldCmd(curValP, dotLocation+1);
		}
	    }
	} else {
	    /*
	     *  We are setting a 'normal' variable.
	     *  The user may specify an optional scope and an optional
	     *  variable type.
	     *  We read the next token using itl_GetVarVal and then
	     *  examine its value.  If it is a string and its value is
	     *  the token we are expecting (a scope or a type) we act
	     *  accordingly.  If it is not, we use that value.
	     *  The variable 'nextVal' holds the next token.
	     *  If 'nextVal' is a string value, stringVal contains its
	     *	string value, otherwise, stringVal is NULL.
	     */
	    var_val_t *nextVal;
	    char *stringVal;
	    int type;

	    itl_PromptUser("Scope, Type, or Value: ");
	    /*
	     *  Look at the next token and see if it is one of those key words
	     */
	    nextVal = itl_GetVarVal();
	    stringVal = ((VALUE_TYPE(nextVal) == itl_stringType) ?
			 STR_VALUE_VAL(nextVal) :
			 NULL);
	    if (stringVal &&
		((varGlobal = (strcasecmp(stringVal,"global") == 0)) ||
		 (strcasecmp(stringVal,"local") == 0))) {
		/*
		 *  The token we read was a scope definition.
		 *  Free the token (the var_val_t ptr) and read the
		 *  next token, which may be either a type specification
		 *  or a value.
		 */
		itl_FreeVarVal(nextVal);
		scope = varGlobal ?  var_global : var_local;
		itl_PromptUser("Type or Value: ");
		nextVal = itl_GetVarVal();
		stringVal = ((VALUE_TYPE(nextVal) == itl_stringType) ?
			     STR_VALUE_VAL(nextVal) :
			     NULL);
	    } else {
		/*
		 *  The user did not specify a scope.
		 */
		scope = var_same;
	    }

	    /*
	     *  Get the type of the variable.
	     *  If it is one of the known types, use the appropriate
	     *  routine.  Otherwise, use the default.
	     */
	    
	    if (stringVal &&
		((type = itlValue_TypeByName(stringVal)) != -1)) {
		/*
		 *  The token we read was a type definition.
		 *  Call the type's read function to get the value.
		 */
		var_val_t *value = itlValue_Read(NULL, type);
		if (value) {
		    success = (itl_SetVariable(varName, value, scope) ?
			       TRUE : FALSE);
		    itl_FreeVarVal(value);
		}  else {
		    itl_Error("The read function for type '%s' failed.\n",
			      itlValue_Type(type));
		}
	    } else {
		/*
		 *  The type was not specified.
		 *  Use the value we already read.
		 */
		success = (itl_SetVariable(varName, nextVal, scope) ?
			   TRUE : FALSE);
	    }
	    itl_FreeVarVal(nextVal);
	}
    }
    itl_AdviseUser("%s was %sset.\n",
		   varName,
		   success && ! itlIO_ParsingError() ? "" : "not ");

    ITL_FREE(varName);
    return(FALSE);
}

#define ITL_SHOW_CMD					\
{"show",  HandleShowVariable, 0, FALSE,			\
     COMMAND_MODE_ALL,					\
     "print the value of a script variable",		\
     "var",						\
     ""}

PRIVATE boolean_t HandleShowVariable(param)
  IN int param;
{
  char *varName;
  boolean_t err;
  
  itl_PromptUser("Variable Name: ");
  varName = itl_GetVariableName(&err, NULL);
  itl_ShowVariable(varName);
  ITL_FREE(varName);
  return(FALSE);
}
#define ITL_SHOWALL_CMD                                        \
{"showAll", itl_ShowAllVariables, 0, FALSE,                    \
     COMMAND_MODE_ALL,                                         \
     "list all the script variables and their current values.",\
     "var",                                                    \
     ""}

#define ITL_SHELL_EXEC_CMD                            \
{"shellExec", HandleShellExec, 1, FALSE,              \
     COMMAND_MODE_ALL,                                \
     "Execute shell commands in a sub-process.",      \
     "misc",                                          \
     "   Syntax:   shellExec <varName | string>\n\
Where <varName> is the name a variale containing either a string or an itl \
procedure.  If the parameter is not a variable name, it is taken as the \
script to be executed.\n\
  The command creates a file in /tmp called \
itl_ShellScript.%d.%d, where the first number is a file suffix and the \
second the process ID.  It writes the commands into that file \
and checks the environment variable SHELL to decide which shell to run.  \
Whether or not the execution succeeded, the file is removed.\n\n\
The command sets the CommandResult variable to the status returned from \
the shell.\n"\
}

/*
 * itl_HandleShellExec() --
 *    Processes the "shellExec" command.  It reads in a shell command
 *    string, writes it to a file, and then starts a sub-process (a shell)
 *    to execute the commands in the file.
 */
PRIVATE boolean_t HandleShellExec(arg)
  IN boolean_t arg;
{
    static unsigned int fileSuffix = 1;
    boolean_t retVal = TRUE;
    char *scriptName, *shellScript = NULL;
    unsigned int scriptLength;
    itl_var_t *scriptVar;
    boolean_t err, scriptAllocated = FALSE;

    /*
     *  Get "string" from user.  It can be a string, a variable that
     *  contains a string, a variable that contains a procedure, or
     *  just a quoted string.
     */
    itl_PromptUser("Command String: ");

    /*
     *  If the first character is a double quote ("), then its a string.
     *  Otherwise we assume it is a variable.
     */
    if (itl_NextCharacter() == '"')
    {
	if ((shellScript = itl_GetString(NULL, (int *) &scriptLength)) == "")
	{
	    itl_Error("The empty string is invalid.\n");
	    shellScript = NULL;
	}
	else
	{
	    scriptAllocated = TRUE;
	}
    }
    else
    {
	scriptName = itl_GetVariableName(&err, NULL);
	scriptVar = itl_LocateVariable(scriptName);
	if (scriptVar) 
	{
	    shellScript = itl_RunnableVar(scriptVar);
	    scriptLength = (int) strlen(shellScript) + 1;
	}
	else
	    itl_Error("The variable %s, does not exist.\n", scriptName);
	ITL_FREE(scriptName);
    }

    if (shellScript)
    {
	char fileName[64];   /* should be big enough */
	FILE *fileP;
	int childProcess;

	/*
	 *  Build up filename where shell commands will be placed.
	 *  Create the file and write in the shell commands.
	 */
	sprintf(fileName, "/tmp/itl_ShellScript.%d.%d", fileSuffix++,
		(int) getpid());
	if ((fileP = fopen(fileName, "w")) != (FILE *) 0)
	{
	    if (fwrite((pointer_t) shellScript, scriptLength, 1, fileP) == 1)
	    {
		/*
		 *  It appears that if you execute just a string without a 
		 *  newline and without passing the -c option to the C shell 
		 *  it won't execute.  So we print an additional newline to 
		 *  the shell script file.
		 */
		putc('\n', fileP);
		fclose(fileP);

		/*
		 *  Shell needs for file to have execute permision.
		 */
		if (!chmod(fileName, 0744))
		{
		    char *execShell;
		    char *args[4];
		    char shellArgs[4];   /* holds -cf options */ 
		    unsigned long sysErrorCode;
		    char shellCommand[128];
		    int retCode;

		    /*
		     *  Setup default shell arguments.
		     */
		    shellArgs[0] = '\0';

		    /*
		     *  Decide which shell to use.
		     */
		    if ((execShell = (char *) getenv("SHELL")) == NULL)
			execShell = "/bin/sh";    /* sh is default shell */
		    else
		    {
			/*
			 *  If the shell is the csh, then we have to 
			 *  pass in the -f option so that the users
			 *  .cshrc file does not get executed.  Some
			 *  systems will complain about background writes
			 *  to tty's.
			 */
			if (strstr(execShell, "csh") != (char *) 0)
			{
			    shellArgs[0] = '-';
			    shellArgs[1] = 'f';
			    shellArgs[2] = '\0';
			}
		    }

		    /*
		     *  Setup and execute the shell command.   The
		     *  system(3) function is not used, since it will issue
		     *  a wait call and block the entire process.  Not a 
		     *  good idea in a threaded environment.
		     */
		    if ((childProcess = fork()) == 0)
		    {
			if (shellArgs[0] != '\0')
			    retCode = execlp(execShell, execShell, shellArgs, 
					     fileName, NULL);
			else
			    retCode = execlp(execShell, execShell, fileName,
					     NULL);
			if (retCode)
			    itl_Error("execlp FAILED with errno, %d\n", errno);
		    }
		    else   /* parent process code */
		    {
			boolean_t childRunning = TRUE;
			struct timespec waitTime;
			pid_t waitStatus;

			/*
			 *  The assumption is that SIGCHLD will not kill
			 *  this process.  So we don't take any special
			 *  action for it.  We issue a non-blocking wait()
			 *  call.  If the child process has not completed
			 *  we sleep for some time and try again.
			 */
			waitTime.tv_sec = 5;
			waitTime.tv_nsec = 0;
			while (childRunning)
			{
			    pthread_delay_np(&waitTime);
			    if ((waitStatus = waitpid(childProcess, &retCode, 
						      WNOHANG)) == -1)
			    {
				if (errno != EINTR)
				{
				    itl_Error("waitpid failed with errno, %d\n",
					      errno);
				    break;
				}
			    }
			    else
			    {
				/*
				 *  If waitStatus = childProcess, the
				 *  child has completed.  Otherwise
				 *  waitStatus must be 0, and thus the
				 *  child is still running.
				 */
				if (waitStatus == (pid_t) childProcess)
				    childRunning = FALSE;
			    }
			}
			if (!childRunning)
			{
			    /*
			     *  If the exit status is not 0, then check to
			     *  see if the user wants to ignore this error
			     */
			    if (WIFEXITED(retCode))
				retCode = WEXITSTATUS(retCode);

			    if (retCode)
			    {
				itl_var_t *varP;
				void (*func)_TAKES((char *, ...));

				varP = itl_LocateVariable("ITL_IgnoreShellError");
				if (varP &&
				    (VAR_TYPE(varP) == itl_intType) &&
				    (VAR_INT_VALUE(varP) != 0))
				{
				    func = itl_AdviseUser;
				}
				else
				{
				    func = itl_Error;
				}

				(*func)("shellExec FAILED - Exit Status: 0x%x  Errno: %d\n", 
					retCode, errno);
			    }
			    else
			    {
				itl_AdviseUser("Shell commands succeeded.\n");
				retVal = FALSE;
			    }
			}
			itl_SetIntVariable("CommandResult", retCode, 
					   var_local);
		    }
		}
		else
		    itl_Error("Chmod FAILED\n");
	    }
	    else
	    {
		itl_Error("File write FAILED, ferror = %d\n", ferror(fileP));
		fclose(fileP);
	    }

	    (void) unlink(fileName);
	}
	else
	    itl_Error("File open FAILED, ferror =  %d\n", ferror(fileP));
    }
    else
	itl_Error("You must enter a quoted string, a string variable or procedure variable name.\n");

    if (scriptAllocated)
	ITL_FREE(shellScript);

    return(retVal);
}

EXPORT void itl_AddAlias(alias, command)
  IN char *alias;
  IN char *command;
{
    itl_commandTable_t *newCommandP = FindCommandEntry(alias, FALSE);

    if (newCommandP && !newCommandP->function) {
	/*
	 *  There is already an alias, replace it.
	 */
	ITL_FREE(newCommandP->description);
	ITL_ALLOC(newCommandP->description, char *, strlen(command) + 1);
	ITL_MEMCOPY(command, newCommandP->description, strlen(command) + 1);
    } else {
	ITL_NEW(newCommandP, itl_commandTable_t);
	newCommandP->function = NULL;
	newCommandP->breakCom = FALSE;
	newCommandP->class = "misc";
	ITL_ALLOC(newCommandP->command, char *, strlen(alias) + 1);
	ITL_MEMCOPY(alias, newCommandP->command, strlen(alias) + 1);
	newCommandP->mode = COMMAND_MODE_ALL;
	ITL_ALLOC(newCommandP->description, char *, strlen(command) + 1);
	ITL_MEMCOPY(command, newCommandP->description, strlen(command) + 1);
	itl_AddCmdTable(newCommandP, 1);
    }
}

PRIVATE boolean_t AliasCmd(param)
  IN int param;
{
    char *alias, *command;
    int len;
    boolean_t eof;
    itl_PromptUser("Alias: ");
    alias = itl_GetString(&eof, &len);
    if (len == 0) {
	itl_Error("Must specify a non-null alias");
	return(FALSE);
    }
    itl_PromptUser("Command name: ");
    command = itl_GetString(&eof, &len);
    itl_AddAlias(alias, command);
    ITL_FREE(command);
    ITL_FREE(alias);
    return(FALSE);
}

#define ITL_ALIAS_CMD        \
{"alias", AliasCmd, 0, FALSE,\
     COMMAND_MODE_ALL,       \
     "add a command alias",  \
     "misc",                 \
     ""}

PRIVATE boolean_t itlCommand_Help _TAKES((
  IN	int param
));
PRIVATE boolean_t itlCommand_Help(param)
  IN	int param;
{
    char *command;
    int len;

    ITL_GET_STRING("Command or \"\" for list: ", command, &len);
    /*
     *  The empty string, "", will have length 0.
     */
    if (len == 0) {
	PrintTableHelp(0);
    } else {
	itl_commandTable_t *commandP = FindCommandEntry(command, TRUE);
	if (commandP) {
	    PrintHelpTableEntry(commandP);
	    if (commandP->function) {
		if ((int) strlen(commandP->help) > 0) {
		    fprintf(Cur_Out, "\n");
		    PrintInfoString(commandP->help);
		}
	    }
	}
    }
    
}

#define ITL_HELP_CMD						\
{"help",  itlCommand_Help, 0, FALSE,				\
     COMMAND_MODE_ALL,						\
     "print help on a named command.",				\
     "misc",							\
     "\tSyntax:  help command\n\
- Provides help on the named command\n\
  If the user types ? or provides a Null command, the list of all \
available commands will be displayed."}

/*
 * Function: PrintTableHelp
 *  Print only those entries available in the current command mode.
 */
EXPORT boolean_t PrintTableHelp(param)
  IN int param;
{
    itl_commandTable_t * tempCmdP;

    for(tempCmdP=cmdStartP; tempCmdP; tempCmdP = tempCmdP->nextCmdP){
	if (tempCmdP->mode & CommandMode)
	    PrintHelpTableEntry(tempCmdP);
    }
    itl_Message("----------------------\n");
    itl_Message("Use the info command for more info on a specific topic\n");
    itl_Message("or type info help for more help.\n");
    return(FALSE);
}


EXPORT itl_commandTable_t commandTable[] = {
{"displayLines",  toggleDisplayLines, 0, FALSE,
     COMMAND_MODE_ALL,
     "toggle the mode which displays lines as they are processed",
     "misc",
     ""},
{"explicitMode", itlIO_SetInterfaceMode, TRUE, FALSE,
     COMMAND_MODE_ALL,
     "set explicit mode - for interface testing",
     "misc",
     ""},
{"normalMode", itlIO_SetInterfaceMode, FALSE, FALSE,
     COMMAND_MODE_ALL,
     "turn explicit mode off.",
     "misc",
     ""},
{"not_verbose", setVerbose,FALSE, FALSE,
     COMMAND_MODE_ALL,
     "set output mode to terse",
     "misc",
     ""},
{"verbose", setVerbose, TRUE, FALSE,
     COMMAND_MODE_ALL,
     "set output mode to verbose",
     "misc",
     ""},
ITL_CLOSE_BRACE_CMD,
ITL_ASSERT_CMD,
ITL_ELAPSED_TIME_CMD,
ITL_ADDITLTRACE_CMD,
ITL_SETITLTRACE_CMD,
ITL_APROPOS_CMD,
ITL_COLOOP_CMD,
ITL_LOOP_CMD,
ITL_CRASH_CMD,
ITL_ECHO_COMMAND,
ITL_COMMENT_COMMAND,
ITL_ERROR_COMMAND,
ITL_ENDLOOP_CMD,
ITL_EXEC_CMD,
ITL_THREAD_EXEC_CMD,
ITL_SHELL_EXEC_CMD,
ITL_HELP_CMD,
ITL_ALIAS_CMD,
ITL_INFO_CMD,
ITL_GETENV_CMD,
ITL_QUIT_CMD,
ITL_SLEEP_CMD,
ITL_TRACE_CMD,
ITL_WHILE_CMD,
ITL_YIELD_CMD,
ITL_IO_REPEAT_CMD,
ITL_IO_DO_CMD,
ITL_IF_THEN_ELSE_CMD,
ITL_IO_DUMP_DATA_CMD,
IO_HISTORY_COMMAND,
IO_INCLUDE_COMMAND,
IO_PATH_ENTRY,
IO_RANDOM_SEED_COMMAND,
/*
 *  Commands that handle script variables
 */
ITL_FETCHOP_CMD,
ITL_TESTANDSET_CMD,
ITL_SETVAR_CMD,
ITL_SHOW_CMD,
ITL_SHOWALL_CMD
};

PRIVATE itl_commandTable_t *FindCommandEntry(command, advise)
  IN char *command;
  IN boolean_t advise;
{
    itl_commandTable_t *firstCommand, *tempCmdP;
    itl_commandTable_t *matchList[25];
    int numMatches = 0;
    boolean_t fullMatch = FALSE;
    if (isalpha(*command)) {
	firstCommand = commandIndex[tolower(*command) - 'a'];
    }
    else {
	firstCommand = cmdStartP;
    }
    for(tempCmdP = firstCommand; tempCmdP; tempCmdP = tempCmdP->nextCmdP){
	int matchVal = MatchCommand(command, tempCmdP);
	if (matchVal == COMMAND_FULL_MATCH) {
	    /*
	     *  A full match was found.
	     *  Make this the only match and exit the loop.
	     */
	    numMatches = 1;
	    fullMatch = TRUE;
	    matchList[0] = tempCmdP;
	    break;
	} else if (matchVal == COMMAND_PARTIAL_MATCH) {
	    /*
	     *  A partial match was found, we keep up to 25 of them.
	     *  Since this is not a full match, keep looking.
	     */
	    if (numMatches < 25)
		matchList[numMatches] = tempCmdP;
	    numMatches++;
	}
    }
    if (numMatches == 1) {
	/*
	 *  There was exactly one match, return it.
	 *  If it was not a full match advise the user of the full
	 *  name of the command
	 */
	if (advise && !fullMatch)
	    itl_AdviseUser("(%s) -\n", matchList[0]->command);
	return(matchList[0]);
    } else if (numMatches == 0) {
	/*
	 *  There were no matches.
	 */
	return(NULL);
    } else {
	/*
	 *  The command is ambiguous, complain.
	 *  If there are not too many alternatives, enumerate them.
	 */
	if (advise) {
	    itl_Error("Ambiguous command: %s matches %d commands.\n", 
		      command, numMatches);
	    if (numMatches < 25) {
		int i;
		itl_Message("Possible matches: \n");
		for (i=0; i<numMatches; i++) {
		    itl_Message("... %s\n",
				matchList[i]->command);
		}
	    }
	}
	return(NULL);
    }
}

PRIVATE itl_commandTable_t *SearchForCommand _TAKES((IN char *command));
PRIVATE itl_commandTable_t *SearchForCommand(command)
  IN  char *command;
{
    int i;
    itl_commandTable_t *aliasHist[ITL_MAX_ALIAS_DEPTH];
    int cycleLen = 0;	/*  The length of an alias cycle if any	 */
    itl_commandTable_t *retVal;
    char *curCommand = command;
    for (i=0, curCommand = command;
	 i<ITL_MAX_ALIAS_DEPTH;
	 i++, curCommand = retVal->description) {
	retVal = FindCommandEntry(curCommand, TRUE);
	if (!retVal) {
	    itl_Error("No such command: %s\n", curCommand);
	    return(NULL);
	}
	if (retVal->function) {
	    /*
	     *  It is not an alias.
	     */
	    return(retVal);
	}
	/*
	 *  It is an alias, see if it is already too deep, otherwise, cont.
	 */
	aliasHist[i] = retVal;
	if ((i > 7) && (retVal == aliasHist[7])) {
	    cycleLen = i-7;
	    break;
	}
    }
    /*
     *  If we got here, we could not unfold the alias.
     *  If cycleLen is 0, there were too many aliases, otherwise, there
     *  was a cycle of length cycleLen
     */
    if (cycleLen) {
	int i;
	itl_Error("Found an alias cycle of length %d:\n", cycleLen);
	for (i=0;
	     (i < cycleLen) || (aliasHist[i] != aliasHist[i-cycleLen]);
	     i++) {
	    fprintf(Cur_Err, "%s -> ", aliasHist[i]->command);
	}
	fprintf(Cur_Err, "%s\n", aliasHist[i]->command);
    } else {
	itl_Error("Aliases nested too deep (max allowed %d)\n",
		  ITL_MAX_ALIAS_DEPTH);
    }
    return(NULL);
}


EXPORT char *itl_CommandLoop(promptStr)
  IN char *promptStr;
{
    itl_commandTable_t *commandEntP;
    /*
     *  Loop until a quit or synonym is found
     */
    for (;;) {
	char *command;
	int eof;

	if (itlIO_ParsingError()) {
	    /*
	     *  If we are either nested or not in interactive mode,
	     *  propogate the error to the previous level.
	     */
	    if(!itlIO_IsInteractive() ||
	       (itlIO_NestingLevel() > 0)) {
		return(NULL);
	    } else {
		/*  We are in interactive mode, the error may have
		 *  left us in some arbitrary place in the history buffer
		 *  Move to the end of the buffer
		 */
		itl_Message("An error occurred, skipping the rest of the buffer.\n");
		itlIO_MoveToEndOfBuffer();
		itl_SetErrorStatus(FALSE);
	    }
	}
	command = itl_GetCommand(promptStr, &eof);
	if (!command)
	    return(NULL);
	/*
	 *  Look for a command that matches the desired command
	 */
	commandEntP = SearchForCommand(command);
	ITL_FREE(command);
	if (commandEntP) {
	    (void) (*commandEntP->function)(commandEntP->parameter);
	    if (commandEntP->breakCom)
		break;
	}
    }
    /*
     *  Return the quit synonym that caused us to exit the loop
     */
    return(commandEntP->command);
}



PRIVATE void LinkCmdTable(curTable, curTableSize)
itl_commandTable_t * curTable;
int curTableSize;
{
    int i;

    for (i=0;i<curTableSize-1;i++){
	curTable[i].nextCmdP = &curTable[i+1];
    }
    curTable[curTableSize-1].nextCmdP = (itl_commandTable_t *) NULL;
}

#ifdef ITL_FS_TEST

/*
 *  SetFSTestingVariables() --
 *    Sets ITL variables that exposed data needed by the file system
 *    tests.
 */
PRIVATE void SetFSTestingVariables()
{
    char *sysType, *hostName, *realHostName, *dotPtr;
    int pageSize;
    var_val_t *newVarP;

#ifdef _AIX
    sysType = "aix";
#endif

#ifdef __OSF1__
    sysType = "osf1";
#endif

#ifdef SUNOS5
    sysType = "solaris";
#endif  /* SUNOS5 */

#ifdef SUNOS4
    sysType = "sunos";
#endif  /* SUNOS5 */

    (void) itl_SetStrVariable(ITL_VAR_OS_TYPE, sysType, var_global);

    /*
     *  Set other variables used by file system tests.
     */
    newVarP = itlStds_IntVal(getpid());
    assert(newVarP);
    if (!itl_SetVariable(ITL_VAR_PROCESS_ID, newVarP, var_local))
	itl_Error("Failed to set variable, %s\n", ITL_VAR_PROCESS_ID);
    ITL_FREE(newVarP);

#if defined(_SC_PAGE_SIZE)
    pageSize = (int) sysconf(_SC_PAGE_SIZE);
#elif defined (_SC_PAGESIZE)
    pageSize = (int) sysconf(_SC_PAGESIZE);
#else
    pageSize = getpagesize();
#endif    

    newVarP = itlStds_IntVal(pageSize);
    assert(newVarP);
    if (!itl_SetVariable(ITL_VAR_SYS_PAGE_SIZE, newVarP, var_local))
	itl_Error("Failed to set variable, %s\n", ITL_VAR_SYS_PAGE_SIZE);
    ITL_FREE(newVarP);

    ITL_ALLOC(hostName, char *, (MAXHOSTNAMELEN + 1));

    if(gethostname(hostName, MAXHOSTNAMELEN + 1))
    {
	*hostName = '\0';  /* on error set name to NULL */
	itl_Message("gethostname failed, setting %s to NULL.\n", 
		    ITL_VAR_HOST_NAME);
    }

    /*
     *  OSF/1 returns hostname as name.<blah>.  We only want the name.
     *  The code is so small I left it in for all machines.
     */
    if (dotPtr = strchr(hostName, '.'))
	*dotPtr = '\0';

    /*
     *  Possibly save a little memory.
     */
    ITL_ALLOC(realHostName, char *, (strlen(hostName) + 1));
    strcpy(realHostName, hostName);
    ITL_FREE(hostName);
    itl_SetStrVariable(ITL_VAR_HOST_NAME, realHostName, var_local);
}

#endif  /* ITL_FS_TEST */

/*
 *  InitOnce() --
 *    Fullfils the need on a pthreads implementation to ensure that the
 *    ITL initialization only occurs once.
 */
PRIVATE void InitOnce()
{
    pthread_mutex_init(&threadBlockMutex, pthread_mutexattr_default);
    pthread_mutex_init(&itlInitMutex, pthread_mutexattr_default);

    /*
     *  Initialize all internal modules.
     */
    if (itl_InitGlobal())
	exit(-1);
    if (itlIO_InternalInit())
	exit(-1);
    if (itlExp_Init())
	exit(-1);
    if (itlIO_Init())
	exit(-1);
    itlPrint_Init();

    cmdStartP = (itl_commandTable_t *) NULL;
    (void) itl_AddCmdTable(commandTable,
			   sizeof(commandTable) / sizeof(itl_commandTable_t));

    /*
     *  Initialize data structures.
     */
    initializeSpecialVars();

#ifdef ITL_FS_TEST

    /*
     *  Initialize variables needed for file system testing.
     */
    SetFSTestingVariables();

#endif  /* ITL_FS_TEST */

    itl_SetErrorStatus(FALSE);
}


EXPORT boolean_t itl_InitItl(interfaceMode, interactiveMode, serverPrefixP)
  IN boolean_t interfaceMode;
  IN boolean_t interactiveMode;
  IN char * serverPrefixP;
{
    static pthread_once_t itlOnce = pthread_once_init;

    if (pthread_once(&itlOnce, (pthread_initroutine_t) InitOnce) < 0)
    {
	/*
	 *  Use fprintf cause we are initializing, and ITL routines may
	 *  not be ready.
	 */
	fprintf(stderr, "pthread_once failed, errno = %d\n", errno);
	return(TRUE);
    }
    /*
     *  Must do additional initialization here, since you cannot pass 
     *  arguments via pthread_once.
     */
    if (!itlIsInitialized) 
    {
	pthread_mutex_lock(&itlInitMutex);
	if (!itlIsInitialized) 
	{
	    /*
	     *  The default is FALSE - Do not set the following two variables
	     *  Unless the default is being changed to avoid unnecesary output
	     */
	    if (interactiveMode)
		itl_SetInteractive(interactiveMode);
	    if (interfaceMode)
		itlIO_SetInterfaceMode(interfaceMode);

	    /*
	     *  Start a server thread to listen for connection requests
	     */
	    if (serverPrefixP) {
		itl_StartServerThread(serverPrefixP);
	    }
	    itlIsInitialized = TRUE;
	}
	pthread_mutex_unlock(&itlInitMutex);
    }

    return(FALSE);
}

/*
 *  itl_CleanupItl -   Cleanup the program for a clean exit
 */
EXPORT boolean_t itl_CleanupItl()
{
    cleanup_entry_t *entryP;

    for ( entryP = cleanupListP; entryP; entryP = entryP->next )
        (entryP->funP)(entryP->arg);

    itlExp_Cleanup();
    itlIO_Cleanup();

    return FALSE;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *      The following commands manipulate the command
 *	table -- allowing the users to add commands to
 *      to the table
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
/*
 *  MergeCmdTables:
 *      Given two command tables which are two sorted linked lists of 
 *      of commands - Merge them into one sorted list of commands.
 */
PRIVATE void MergeCmdTables(table1P, table2P)
itl_commandTable_t * table1P;
itl_commandTable_t * table2P;
{
    itl_commandTable_t ** previousPP;
    itl_commandTable_t * nextTable2P;

    previousPP = &cmdStartP;
    while (table1P && table2P){
	if (CmpTableCommands(table1P, table2P) > 0) {
	    nextTable2P = table2P->nextCmdP;
	    table2P->nextCmdP = table1P;
	    (*previousPP) = table2P;
	    previousPP = &table2P->nextCmdP;
	    table2P = nextTable2P;
	}
	else {
	    previousPP = &table1P->nextCmdP;
	    table1P = table1P->nextCmdP;
	}
    }

    if (table2P){
	(*previousPP) = table2P;
    }
}

/*
 *  HashCmdTable:
 *      Takes the command table (which is a sorted linked list), and
 *      generates a hash table keyed on the first letter of the command.
 */
PRIVATE void HashCmdTable()
{
    itl_commandTable_t *tableP = cmdStartP;
    int nextIndex = 1;
    commandIndex[0] = tableP;
    while (tableP){
	if (tolower(tableP->command[0]) < ('a' + nextIndex)) {
	    tableP = tableP->nextCmdP;
	    }
	else {
	    commandIndex[nextIndex] = tableP;
	    nextIndex++;
	    if (nextIndex == 26)
		break;
	}
    }
}

/*
 *   itl_AddCmdTable:  The exported routine for adding commands.
 *
 *   Given an array of commands and the length of the array,
 *   Add all the commands in the array to the current command table.
 */
EXPORT boolean_t itl_AddCmdTable(commandTableP, commandTableSize)
IN OUT itl_commandTable_t *commandTableP;
IN     int            commandTableSize;
{
    /*  
     *  We do not assume that the commands added are sorted.
     *  We first sort them, then link them into a linked list
     *  and finally merge them with the current table, which
     *  is already a sorted linked list of commands
     */

    qsort((void *)commandTableP,
          commandTableSize, 
	  sizeof(itl_commandTable_t),
	  CmpTableCommands);

    LinkCmdTable(commandTableP, commandTableSize);

    MergeCmdTables(cmdStartP, &commandTableP[0]);

    HashCmdTable();

    return FALSE;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *   itl_AddModuleCmds :
 *	Add module commands to the command table.
 *
 *	Parameters:
 *	name - The name of the new module (for info purposes)
 *	commandTableP - A pointer to the new commands.
 *	commandTableSize - The number of entries in the above table.
 *	moduleMask - The default mask to or in for this module.
 *	moduleInfo - The info message for this module.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT  boolean_t itl_AddModuleCmds(name, commandTableP, commandTableSize,
				   moduleMask, moduleInfo)
  IN char 		*name;
  IN itl_commandTable_t *commandTableP;
  IN int                 commandTableSize;
  IN int 		 moduleMask;
  IN char 		*moduleInfo;
{
    itl_AddCmdTable(commandTableP, commandTableSize);
    CommandMode |= moduleMask;

    if (moduleInfo) {
	infoSize ++;
	if (!infoList) {
	    ITL_ALLOC(infoList, itl_help_t *,
		      (infoSize * sizeof(itl_help_t)));
	    }
	else {
	    ITL_UTIL_REALLOC(infoList, itl_help_t *,
			     (infoSize * sizeof(itl_help_t)));
	}
	infoList[infoSize - 1].command = "";
	infoList[infoSize - 1].description = moduleInfo;
	infoList[infoSize - 1].class = name;
	infoList[infoSize - 1].longDescr = "";
    }
    return(TRUE);
}
