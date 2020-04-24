%{
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: testtran_gram.y,v $
 * Revision 1.1.11.1  1996/10/02  17:25:39  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:47  damon]
 *
 * Revision 1.1.6.1  1994/06/09  14:01:28  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:36:44  annie]
 * 
 * Revision 1.1.4.5  1993/01/19  15:30:14  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:51:04  cjd]
 * 
 * Revision 1.1.4.4  1992/11/24  16:26:21  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:24:24  bolinger]
 * 
 * Revision 1.1.4.3  1992/09/25  16:21:03  jaffe
 * 	Sync with Transarc by cleaned duplicate HISTORY and Log info
 * 	[1992/09/24  17:49:18  jaffe]
 * 
 * Revision 1.1.4.2  1992/09/15  17:38:28  jaffe
 * 	Transarc delta: mason-ot4815-make-nested-tran-work-again 1.1
 * 	  Selected comments:
 * 	    Nested transactions stopped working because the interface to the log system
 * 	    stopped supporting the 'forced' option.
 * 	    Fixed so it compiles now.
 * 	Transarc delta: mason-tran-0033 1.11
 * 	  Selected comments:
 * 	    Checkpoint for the year-end.
 * 	    Intermediate snapshot - single threaded code now working correctly.
 * 	    SHOULD NOT BE USED IN PRODUCTION.
 * 	    Test language for transaction manager.
 * 	    Forced close to fix a different problem. This delta still does not work; do
 * 	    not use in production!
 * 	    Snapshot - passes all_tests.
 * 	    More work - now fixing bugs.
 * 	    Final round of testing; appears to be production quality now.
 * 	    Update internal development records.
 * 	    Found single-threaded case in TranForce which would block.
 * 	    Macro removed in delta 1.9 was used in multiple files.
 * 	    Found race condition where tran was referenced before initialization.
 * 	[1992/09/14  20:58:05  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/05  18:25:11  jdp
 * 	delta mason-tran-0033 1.11
 * 	[1992/05/04  18:01:47  jdp]
 * 
 * $EndLog$
 */
/* testgram.y a yacc-based parser description for testing the tran mgr */
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/us.h>
#include "logbuf.h"
#include "tran.h"

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/logbuf/testtran_gram.y,v 1.1.11.1 1996/10/02 17:25:39 damon Exp $")

elbt_TranRec_t tid;
int afsdb_episode_logbuf;

%}

%union {
    int val;
    char *str;
}

%token <val> INTEGER
%token <str> STRING
%token START END

%%

  operations: operations operation
    | operation
    ;

 operation: startTran '\n'
    | endTran '\n'
    | '\n'
    | error '\n'
    ;

 startTran: START
  {
      /* start */
      elbt_StartTran("testtran", 0, 0, 0, &tid);
      printf("started transaction %d\n", tid);
  }
;
  endTran: END
  {
      /* end */
      elbt_EndTran(tid);
      printf("ended transaction %d\n", tid);
  }
;
%%
#include <stdio.h>

char *progname;
char *filename;

main(argc, argv)
  int argc;
  char **argv;
  {
      long code;
      extern FILE *yyin;

      progname = argv[0];
      if (argc != 2) {
	  fprintf(stderr,"usage: %s <filename>\n",progname);
	  exit(1);
      }

      filename = osi_Alloc(strlen(argv[1]));
      strcpy(filename, argv[1]);
      yyin = fopen(filename,"r"); /* lex will use this file now */
      if (!yyin) {
	  fprintf(stderr,"could not open file %s\n",filename);
	  exit(1);
      }
      initialize_lgb_error_table();

      elbt_Init(0);

      yyparse();
  }

extern unsigned lineNumber;

yyerror(s)
  char *s;
  {
      fprintf(stderr, "%s in file %s at line %d\n", s, filename, lineNumber);
  }

SHARED int32 log_Write (tP, recType, dataLength, data)
  IN struct elbt_TranRec *tP;
  IN unsigned recType;
  IN u_int32 dataLength;
  IN caddr_t data;
  {
      return 0;
  }

SHARED int elbl_IsTranCommitted(tP)
  struct elbt_TranRec *tP;
  {
  }

SHARED void elbl_StartTran(dP, tP)
  struct async_device *dP;
  struct elbt_TranRec *tP;
  {
      tP->mapP = (opaque)1;
  }

SHARED void elbl_EndTran(tP)
  struct elbt_TranRec *tP;
  {
      tP->mapP = (opaque)0;
  }

SHARED int32 log_Force (dP, pageOffset)
     struct async_device *dP;
     u_int16 pageOffset;
  {
  }

