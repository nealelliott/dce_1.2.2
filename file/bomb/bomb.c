/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bomb.c,v $
 * Revision 1.1.28.1  1996/10/02  17:03:12  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:38  damon]
 *
 * Revision 1.1.22.3  1994/07/13  22:19:11  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:17:28  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  17:55:03  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:53:47  mbs]
 * 
 * Revision 1.1.22.2  1994/06/09  13:51:42  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:25:15  annie]
 * 
 * Revision 1.1.22.2  1994/06/09  13:51:42  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:25:15  annie]
 * 
 * Revision 1.1.22.1  1994/02/04  20:05:56  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:06:28  devsrc]
 * 
 * Revision 1.1.20.1  1993/12/07  17:12:40  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  20:03:44  jaffe]
 * 
 * $EndLog$
 */

/*
 * Copyright (C) 1993 Transarc Corporation - All rights reserved
 */

#include <pthread.h>
#include <dcedfs/osi.h>
#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>

#include <bomb.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bomb/bomb.c,v 1.1.28.1 1996/10/02 17:03:12 damon Exp $")

#define COMMON_OFFSET		8
#define KERNEL_OFFSET		(COMMON_OFFSET)
#define SERVER_OFFSET		(COMMON_OFFSET + 1)

#define SET_BPDESC_OFFSET	0
#define SET_COMMAND_OFFSET	1

#define UNSET_BPNAME_OFFSET	0

#define APPEND_CHUNK_SIZE	32

static void	AppendString(char**, char*, unsigned*);
static int	CmdSetup(struct cmd_syndesc*, char*);
static int	DoSet(struct cmd_syndesc*, char*);
static int	DoUnset(struct cmd_syndesc*, char*);
static void	SetupSetSyntax(void);
static void	SetupSharedSyntax(struct cmd_syndesc*, char*);
static void	SetupUnsetSyntax(void);

#ifdef AFS_DEBUG
static int	DoTest(struct cmd_syndesc*, char*);
static void	SetupTestSyntax(void);
#endif

static int	kernel = 0;
static char*	server = NULL;


main(int argc, char* argv[])
{
    long	code;

    (void)cmd_SetBeforeProc(CmdSetup, NULL);
    SetupSetSyntax();
    SetupUnsetSyntax();

#ifdef AFS_DEBUG
    SetupTestSyntax();
#endif

    code = cmd_Dispatch(argc, argv);

    exit(code == 0 ? 0 : 1);
}	/* main() */


static int
CmdSetup(struct cmd_syndesc* csdP, char* rock)
{
    if (csdP->parms[KERNEL_OFFSET].items)
	kernel = 1;
    else if (csdP->parms[SERVER_OFFSET].items)
	server = csdP->parms[SERVER_OFFSET].items->data;
    return 0;
}	/* CmdSetup() */


static int
DoSet(struct cmd_syndesc* csdP, char* rock)
{
    bombPoint_t		bp;
    char*		bpDesc;
    char*		bpList = NULL;
    unsigned		bpListLen = 0;
    char		bpName[BOMB_MAX_NAME+1];
    long		code = 0;
    long		code2;
    char*		command = NULL;
    unsigned		commandLen = 0;
    cmd_dataIter_t	cmdIter;
    char*		cmdToken;

    if (!kernel && !server)
	for (cmdToken = cmdIter_FirstData(&cmdIter, SET_COMMAND_OFFSET, csdP);
	     cmdToken != NULL;
	     cmdToken = cmdIter_NextData(&cmdIter)) {

	    if (command != NULL)
		AppendString(&command, " ", &commandLen);

	    AppendString(&command, cmdToken, &commandLen);
	}
    
    for (bpDesc = cmdIter_FirstData(&cmdIter, SET_BPDESC_OFFSET, csdP);
	 bpDesc != NULL;
	 bpDesc = cmdIter_NextData(&cmdIter)) {

	if (code2 = bomb_ParseDesc(bpDesc, bpName, &bp))
	    (void)fprintf(stderr, "Failed to parse \"%s\": %s\n",
			  bpDesc, dfs_dceErrTxt(code2));
		
	if (command) {
	    if (code2)
		return code2;

	    if (bpList == NULL) {
		AppendString(&bpList, BOMB_ENV_VAR_NAME, &bpListLen);
		AppendString(&bpList, "=", &bpListLen);
	    } else {
		AppendString(&bpList, ";", &bpListLen);
	    }
	    AppendString(&bpList, bpDesc, &bpListLen);
	} else {
	    if (code2) {
		if (code == 0)
		    code = code2;
		continue;
	    }

	    if (kernel) {
		code2 = BOMB_KERNEL_SET(bpName, &bp);
	    } else {			/* Must be server */
		(void)fprintf(stderr,
			      "Server bomb points are not implemented yet\n");
		return ENOSYS;
	    }

	    if (code2) {
		(void)fprintf(stderr, "Failed to set %s: %s\n",
			      bpName, dfs_dceErrTxt(code2));
		if (code == 0)
		    code = code2;
		continue;
	    }
	}
    }

    if (command) {
	if (code = putenv(bpList)) {
	    (void)fprintf(stderr, "Failed to alter environment: %s\n",
			  dfs_dceErrTxt(errno));
	    return errno ? errno : -1;
	}

	if (code = system(command))
	    (void)fprintf(stderr, "Command failed; exit status was %#lx\n",
			  code);
    }

    if (bpList != NULL)
	free(bpList);

    return code;
}


#ifdef AFS_DEBUG
static int
DoTest(struct cmd_syndesc* csdP, char* rock)
{
    long	code;

    if (kernel) {
	code = BOMB_KERNEL_TEST();
    } else {			/* Must be server */
	(void)fprintf(stderr, "Server bomb points are not implemented yet\n");
	return ENOSYS;
    }

    (void)printf("Test returned, code = %ld: %s\n",
		 code, dfs_dceErrTxt(code));

    return code;
}
#endif	/* AFS_DEBUG */


static int
DoUnset(struct cmd_syndesc* csdP, char* rock)
{
    cmd_dataIter_t	bpIter;
    char*		bpName;
    long		code = 0;
    long		code2;

    for (bpName = cmdIter_FirstData(&bpIter, UNSET_BPNAME_OFFSET, csdP);
	 bpName != NULL;
	 bpName = cmdIter_NextData(&bpIter)) {

	if (kernel) {
	    code2 = BOMB_KERNEL_UNSET(bpName);
	} else {			/* Must be server */
	    (void)fprintf(stderr,
			  "Server bomb points are not implemented yet\n");
	    return ENOSYS;
	}

	if (code2) {
	    (void)fprintf(stderr, "Failed to unset %s: %s\n",
			  bpName, dfs_dceErrTxt(code2));
	    if (code == 0)
		code = code2;
	    continue;
	}
    }

    return code;
}


static void
AppendString(char** s1P, char* s2, unsigned* s1LenP)
{
    unsigned	allocLen = 0;
    unsigned	s2Len;

    s2Len = strlen(s2);

    if (*s1P == NULL) {
	allocLen = MAX(s2Len + 1, APPEND_CHUNK_SIZE);
	*s1P = malloc(allocLen);
	*s1P[0] = '\0';
    } else {
	long int	neededLen;

	neededLen = s2Len - (*s1LenP - (strlen(*s1P) + 1));
	if (neededLen > 0) {
	    allocLen = MAX(neededLen, APPEND_CHUNK_SIZE);
	    *s1P = realloc(*s1P, *s1LenP + allocLen);
	}
    }

    afsl_PAssertTruth(*s1P != NULL, ("Out of memory"));

    (void)strcat(*s1P, s2);
    *s1LenP += allocLen;
}


static void
SetupSetSyntax()
{
    struct cmd_syndesc*	csdP;

    csdP = cmd_CreateSyntax("set", DoSet, NULL, "set bomb point(s)");

    (void)cmd_AddParm(csdP, "-bpdesc", CMD_LIST, CMD_REQUIRED,
		      "bomb point description");
    (void)cmd_AddParm(csdP, "-command", CMD_LIST, CMD_REQUIRED|CMD_EXPANDS,
		      "shell command");
    SetupSharedSyntax(csdP, "-command");
}


static void
SetupSharedSyntax(struct cmd_syndesc* csdP, char* altSwitch)
{
    (void)cmd_Seek(csdP, COMMON_OFFSET);

    (void)cmd_AddParm(csdP, "-kernel", CMD_FLAG, CMD_REQUIRED, "");
    (void)cmd_AddParm(csdP, "-server", CMD_SINGLE, CMD_REQUIRED,
		      "server name");
    if (altSwitch)
	cmd_MakeAlternate(csdP, altSwitch, "-kernel");
    cmd_MakeAlternate(csdP, "-kernel", "-server");
}


#ifdef AFS_DEBUG
static void
SetupTestSyntax()
{
    struct cmd_syndesc*	csdP;

    csdP = cmd_CreateSyntax("test", DoTest, NULL, "test bomb point(s)");
    SetupSharedSyntax(csdP, NULL);
}
#endif	/* AFS_DEBUG */


static void
SetupUnsetSyntax()
{
    struct cmd_syndesc*	csdP;

    csdP = cmd_CreateSyntax("unset", DoUnset, NULL, "unset bomb point(s)");

    (void)cmd_AddParm(csdP, "-bpname", CMD_LIST, CMD_REQUIRED,
		      "bomb point name");
    SetupSharedSyntax(csdP, NULL);
}
