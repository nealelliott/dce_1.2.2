/* Copyright (C) 1996, 1994 Transarc Corporation - All rights reserved. */

/**************************************************************
 * This file provides tests for low level utilities that are
 * used in tkm.
 *
 * The different subcommands test the following:
 *	individual token conflicts (types, ranges host)  - pair
 *	creation of list of conflicting tokens - list
 ***************************************************************/
#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>
#include "tkm_tokens.h"
#include "tkm_internal.h"
#include "tkm_conflicts.h"
#include "tkm_tokenlist.h"
#include "tkm_file.h"

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/tkm/RCS/tkm_test_units.c,v 1.4 1996/03/04 19:13:25 ota Exp $")

int break_in_debugger() {
     printf("Need debugger now!\n");
     while(1);
}

int PrintTypes()
{

    printf ("TKM_LOCK_READ -> %d\nTKM_LOCK_WRITE -> %d\nTKM_DATA_READ -> %d\nTKM_DATA_WRITE -> %d\nTKM_OPEN_READ -> %d\nTKM_OPEN_WRITE -> %d\nTKM_OPEN_SHARED -> %d\nTKM_OPEN_EXCLUSIVE -> %d\nTKM_OPEN_DELETE -> %d\nTKM_OPEN_PRESERVE -> %d\nTKM_STATUS_READ -> %d\nTKM_STATUS_WRITE -> %d\nTKM_NUKE -> %d\nTKM_SPOT_HERE -> %d\nTKM_SPOT_THERE -> %d\n",
	    TKM_TOKENTYPE_MASK(TKM_LOCK_READ),
	    TKM_TOKENTYPE_MASK(TKM_LOCK_WRITE),
	    TKM_TOKENTYPE_MASK(TKM_DATA_READ),
	    TKM_TOKENTYPE_MASK(TKM_DATA_WRITE),
	    TKM_TOKENTYPE_MASK(TKM_OPEN_READ),
	    TKM_TOKENTYPE_MASK(TKM_OPEN_WRITE),
	    TKM_TOKENTYPE_MASK(TKM_OPEN_SHARED),
	    TKM_TOKENTYPE_MASK(TKM_OPEN_EXCLUSIVE),
	    TKM_TOKENTYPE_MASK(TKM_OPEN_DELETE),
	    TKM_TOKENTYPE_MASK(TKM_OPEN_PRESERVE),
	    TKM_TOKENTYPE_MASK(TKM_STATUS_READ),
	    TKM_TOKENTYPE_MASK(TKM_STATUS_WRITE),
	    TKM_TOKENTYPE_MASK(TKM_NUKE),
	    TKM_TOKENTYPE_MASK(TKM_SPOT_HERE),
	    TKM_TOKENTYPE_MASK(TKM_SPOT_THERE));
}

int ReadTokenList(file, tokens, fileP)
    char *file;
    tkm_tokenList_t *tokens;
    tkm_file_t *fileP;
{
    FILE *tkf;
    char line[80];
    tkm_internalToken_p	granted;
    int id, host, type, low, high;

    if ((tkf = fopen(file,"r")) == NULL)
	return 1;
    while (fgets(line,80,tkf) != NULL){
	granted = (tkm_internalToken_p) osi_Alloc(sizeof(tkm_internalToken_t));
	sscanf(line, "%d%d%d%d%d", &id, &host, &type, &low, &high);
	AFS_hset64(granted->id, 0, id);
	granted->host = (struct hs_host *) host;
	granted->types = type;
	AFS_hset64(granted->range.lowBnd, 0, low);
	AFS_hset64(granted->range.upBnd, 0, high);
	granted->holder.file = fileP;
	tkm_AddToTokenList(granted,tokens);
    }
    return (0);
}

int CompareWithManyTokens(as)
    struct cmd_syndesc *as;
{
    tkm_tokenList_t *issued;
    tkm_internalToken_t requested;
    long 	revokeSet;
    long     sliceSet;
    char *issuedFile;
    int code;
    struct tkm_tokenConflictQ *conflicts;
    int max;
    long otherHostMask;
    afsFid fid;
    tkm_file_t *fileP;

    fid.Cell = tkm_localCellID;
    AFS_hset64(fid.Volume,0,1);
    fid.Vnode = 1;
    fid.Unique = 1;
    fileP = tkm_FindFile(&fid, 1 /* add */);

    issued = (tkm_tokenList_t *) osi_Alloc(sizeof(tkm_tokenList_t));
    tkm_TokenListInit(issued);
    requested.host = (struct hs_host *) ((cmd_IsParamPresent(as,0)) ?
		      atoi(cmd_FirstData(as,0)):0);
    requested.types = ((cmd_IsParamPresent(as,1)) ?
			atoi(cmd_FirstData(as,1)):
			(TKM_STATUS_READ | TKM_STATUS_WRITE));
    AFS_hset64(requested.range.lowBnd, 0, ((cmd_IsParamPresent(as,2)) ?
					   atoi(cmd_FirstData(as,2)):0));
    AFS_hset64(requested.range.upBnd, 0, ((cmd_IsParamPresent(as,3)) ?
					  atoi(cmd_FirstData(as,3)):10));
    issuedFile = cmd_FirstData(as,4);
    max = ((cmd_IsParamPresent(as,5)) ? atoi(cmd_FirstData(as,5)):50);
    if (ReadTokenList(issuedFile, issued, fileP)){
	printf("error reading token file\n");
	return(1);
    }
    if (issued->list == NULL)
	exit(1);
    TKM_LOCK_TOKEN_HOLDER(issued->list);
    conflicts = tkm_ConflictingTokens(issued, &requested,
				      &otherHostMask, max);
    printf("otherHostmask is %d\n", otherHostMask);
    if (conflicts != NULL){
	tkm_PrintTokenConflictQ(conflicts);
	tkm_DeleteConflictQ(conflicts);
    } else
	printf("No conflicts were found\n");
    tkm_FreeTokenList(issued);
}


int CompareTwoTokens(as)
    struct cmd_syndesc *as;
{
    tkm_internalToken_t issued;
    tkm_internalToken_t requested;
    long 	revokeSet;
    long     sliceSet;
    int code;

    issued.host = (struct hs_host *) ((cmd_IsParamPresent(as,0)) ?
				      atoi(cmd_FirstData(as,0)):0);
    issued.types = ((cmd_IsParamPresent(as,1)) ?
			atoi(cmd_FirstData(as,1)):
			(TKM_STATUS_READ | TKM_STATUS_WRITE));
    AFS_hset64(issued.range.lowBnd, 0,
	       ((cmd_IsParamPresent(as,2)) ? atoi(cmd_FirstData(as,2)) : 0));
    AFS_hset64(issued.range.upBnd, 0,
	       ((cmd_IsParamPresent(as,3)) ? atoi(cmd_FirstData(as,3)) : 10));
    requested.host = (struct hs_host *) ((cmd_IsParamPresent(as,4)) ?
		      atoi(cmd_FirstData(as,4)):1);
    requested.types = ((cmd_IsParamPresent(as,5)) ?
			   atoi(cmd_FirstData(as,5)):TKM_STATUS_READ);
    AFS_hset64(requested.range.lowBnd, 0,
	       ((cmd_IsParamPresent(as,6)) ? atoi(cmd_FirstData(as,6)) : 0));
    AFS_hset64(requested.range.upBnd, 0,
	       ((cmd_IsParamPresent(as,7)) ? atoi(cmd_FirstData(as,7)) : 10));
    code =  tkm_ExpRevokeReqd(&issued, &requested,
			   &revokeSet, &sliceSet);
    if (!code)
	printf("No revocation required\n");
    else {
	printf("Need to revoke types %d.", revokeSet);
	if (sliceSet != 0)
	    printf(" Can offer slice&dice for types %d.\n", sliceSet);
	else
	    printf("\n");
    }
}

main(argc, argv)
int argc;
char *argv[];
{
    struct cmd_syndesc *s;

    tkm_InitConflictTable();

    s = cmd_CreateSyntax("printtypes", PrintTypes, (char *)NULL,
				"print numeric values of token types");
    s = cmd_CreateSyntax("pair", CompareTwoTokens, (char *)NULL,
				"test conflicts for two tokens");

    (void)cmd_AddParm(s, "-ishost", CMD_SINGLE,
			   CMD_OPTIONAL, "host id of issued token (0 or 1)");
    (void)cmd_AddParm(s, "-istype", CMD_SINGLE,
			   CMD_OPTIONAL, "types of issued token");
    (void)cmd_AddParm(s, "-islow", CMD_SINGLE,CMD_OPTIONAL,
		      "byterange lower bound for issued token");
    (void)cmd_AddParm(s, "-isupper", CMD_SINGLE, CMD_OPTIONAL,
		      "byterange upper bound for issued token");
     (void)cmd_AddParm(s, "-reqhost", CMD_SINGLE, CMD_OPTIONAL,
		       "host id of requested token (0 or 1)");
    (void)cmd_AddParm(s, "-reqtype", CMD_SINGLE,
		      CMD_OPTIONAL, "types of requested token");
    (void)cmd_AddParm(s, "-reqlow", CMD_SINGLE, CMD_OPTIONAL,
		      "byterange lower bound for requested token");
    (void)cmd_AddParm(s, "-requpper", CMD_SINGLE, CMD_OPTIONAL,
		      "byterange upper bound for requested token");
    s = cmd_CreateSyntax("list", CompareWithManyTokens, (char *)NULL,
				"test conflicts for a token and a list of tokens");
     (void)cmd_AddParm(s, "-reqhost", CMD_SINGLE, CMD_OPTIONAL,
		       "host id of requested token (0 or 1)");
    (void)cmd_AddParm(s, "-reqtype", CMD_SINGLE,
		      CMD_OPTIONAL, "types of requested token");
    (void)cmd_AddParm(s, "-reqlow", CMD_SINGLE, CMD_OPTIONAL,
		      "byterange lower bound for requested token");
    (void)cmd_AddParm(s, "-requpper", CMD_SINGLE, CMD_OPTIONAL,
		      "byterange upper bound for requested token");
     (void)cmd_AddParm(s, "-issued", CMD_SINGLE,CMD_REQUIRED,
		       "file holding the description of the issued tokens list");
    cmd_Dispatch(argc, argv);
}

