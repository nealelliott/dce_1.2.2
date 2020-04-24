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
 * $Log: testbuf_gram.y,v $
 * Revision 1.1.61.1  1996/10/02  17:25:35  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:45  damon]
 *
 * Revision 1.1.56.2  1994/06/09  14:01:26  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:36:41  annie]
 * 
 * Revision 1.1.56.1  1994/02/04  20:14:05  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:45  devsrc]
 * 
 * Revision 1.1.54.1  1993/12/07  17:19:40  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  14:03:09  jaffe]
 * 
 * $EndLog$
 */
/* testbuf.y a yacc-based parser description for buffer package testing */
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/us.h>
#include <dcedfs/osi_user.h>
#include <pwd.h>
#include <dcedfs/episode/logbuf.h>
#include "logrec.h"
#include "tran.h"

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#undef YYDEBUG
#define YYDEBUG 1

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/logbuf/testbuf_gram.y,v 1.1.61.1 1996/10/02 17:25:35 damon Exp $")

elbb_tranRec_t mapToTid _TAKES((u_int32));
void setTidMap _TAKES((u_int32, elbb_tranRec_t));
void openDevice _TAKES((char *, u_int32, struct async_device **, unsigned,
			unsigned));
void closeDevice _TAKES((struct async_device *));
void initBufferSystem _TAKES((void));
void initLogSystem _TAKES((void));
PRIVATE caddr_t CvtData _TAKES((char *, unsigned));
PRIVATE char *MakeDeviceName _TAKES((char *));
PRIVATE void InitializeData _TAKES((daddr_t, daddr_t, int,
				    struct async_device *));
PRIVATE void UnCvtData _TAKES((char *, char *, unsigned));

struct async_device *dataDeviceP, *logDeviceP;
extern unsigned lineNumber;
char *progname;
char *filename;
unsigned verbose = 1;
int failures = 0;

/* use these to determine if we are opening the log or data device special. */
#define USDEV_LOG 1
#define USDEV_DATA 2

#define Printf(x) printf("%s %-3d: ", filename, lineNumber), printf x, printf("\n")
#define Vprintf(x) if(verbose) Printf(x)


/*
 * the sizes of our "aggregate" & log (in transactions for the log).
 */
unsigned nLogTrans = 10;
unsigned nLogBlocks;
unsigned nDataBlocks = 20;
%}

%union {
    int val;
    char *str;
}

%token <val> INTEGER CHARDATA
%token <str> STRING
%token DEBUG DATA DATA2 MERGE END RSTRT STARTTRAN DUMPLOG FILL UZERO
%token CREATELOG OPENLOG CLOSE PAGECLEAR RECOVER FORCE SYNC SYNCF
%token INCLUDE COMMENT SET RECOVERDEBUG LOGBUFDEBUG TESTDEBUG LOGSIZE
%token COMPARE NCOMPARE QUIT OPENDATA YACCDEBUG PAGESET

%%

  operations: operations operation
    | operation
    ;

  operation:
      '\n'
    | pageClear '\n'
    | pageSet '\n'
    | set '\n'
    | createLog '\n'
    | openLog '\n'
    | close '\n'
    | openData '\n'
    | startTran '\n'
    | endTran '\n'
    | recover '\n'
    | dumplog '\n'
    | force '\n'
    | fill '\n'
    | uzero '\n'
    | merge '\n'
    | include '\n'
    | data '\n'
    | data2 '\n'
    | debug '\n'
    | rstrtDecl '\n'
    | echo '\n'
    | cmp '\n'
    | ncmp '\n'
    | sync '\n'
    | fsync '\n'
    | quit '\n'
    | error '\n'
;
 pageClear: PAGECLEAR INTEGER
  {
      /* pageclear page-number */
      /*    $1         $2      */

      InitializeData($2, $2, 0, dataDeviceP);

  }
  | PAGECLEAR INTEGER INTEGER
  {
      /* pageclear first-page-number last-page-number */
      /*   $1            $2                $3         */
      InitializeData($2, $3, 0, dataDeviceP);
  }

 pageSet: PAGESET CHARDATA INTEGER INTEGER
  {
      /* pageset fill-character first-page-number last-page-number */
      /*   $1       $2               $3                $4          */
      InitializeData($3, $4, $2, dataDeviceP);
  }

 set: SET LOGSIZE INTEGER INTEGER
  {
      /* set logsize n data blocks n log trans  */
      /* $1     $2            $3        $4      */
      Vprintf(("set log size to %d data blocks and %d trans",$3,$4));
      nDataBlocks = $3;
      nLogTrans = $4;
  }
     | SET RECOVERDEBUG INTEGER
  {
      extern int afsdb_episode_recover;

      Vprintf(("set recovery debug flags to %d",$3));
      afsdb_episode_recover = $3;
  }
     | SET LOGBUFDEBUG INTEGER
  {
      extern int afsdb_episode_logbuf;

      Vprintf(("set logbuf debug flags to %d",$3));
      afsdb_episode_logbuf = $3;
  }
     | SET TESTDEBUG INTEGER
  {
      Vprintf(("change verbose flag to %d",$3));
      verbose = $3;
  }
     | SET YACCDEBUG INTEGER
  {
      Printf(("change yydebug to %d", $3));
      yydebug = $3;
  }
     ;

 createLog: CREATELOG STRING INTEGER
  {
      /* createlog logdevname logdevblksize */
      /*   $1          $2          $3       */
      daddr_t *logBlocks;
      unsigned index;
      long code;
      dev_t devNum;

      if (!dataDeviceP) {
	  Printf(("createLog called but data device not active"));
	  exit(7);
      }

      /* we create a device so we can compute the correct log size */
      openDevice ($2, $3, &logDeviceP, USDEV_LOG, 10);
      nLogBlocks = elbl_MinLogSize(logDeviceP, nLogTrans);
      closeDevice (logDeviceP);
      logDeviceP = 0;

      /* now reopen with the REAL log size */
      openDevice ($2, $3, &logDeviceP, USDEV_LOG, nLogBlocks);

      Vprintf(("createlog on %s with blocksize %d using %d blocks",$2, $3,
	       nLogBlocks));

      /*
       * create a clean restart record
       */

      /* set log block array */
      logBlocks = (daddr_t *)osi_Alloc(sizeof(daddr_t) * nLogBlocks);
      for (index = 0; index < nLogBlocks; index++) {
	  logBlocks[index] = index;
      }
      code = elbb_LogCreate(logDeviceP, dataDeviceP, logBlocks, nLogBlocks, 1);
      afsl_MBZ(code);

      code = elbb_Shutdown(dataDeviceP);
      afsl_MBZ(code);

/*    closeDevice(dataDeviceP); */
      closeDevice(logDeviceP);
      /* dataDeviceP = */ logDeviceP = 0;

      /* done */
  }

 openLog: OPENLOG STRING INTEGER
  {

      /* createlog logdevname datadevname logdevblksize datadevblksize*/
      /* openlog device-name block-size */
      /*   $1      $2           $3      */
      Vprintf(("openlog on %s with blocksize %d using %d blocks",$2, $3,
	       nLogBlocks));
      openDevice($2, $3, &logDeviceP, USDEV_LOG, nLogBlocks);
      initLogSystem();

      /* done. */
  }

 openData: OPENDATA STRING INTEGER
  {
      /* opendata device-name block-size */
      /*   $1      $2           $3      */

      Vprintf(("opendata on %s with blocksize %d using %d blocks",$2, $3,
	       nDataBlocks));
      openDevice($2, $3, &dataDeviceP, USDEV_DATA, nDataBlocks);

      /* done. */
  }

 close: CLOSE
  {
      /* close */
      /*   $1  */

      dev_t devNum;
      long code;

      Vprintf(("close"));
      if (dataDeviceP) {
	  code = elbb_Shutdown(dataDeviceP);
	  afsl_MBZ(code);

	  closeDevice(dataDeviceP);
	  dataDeviceP = 0;
      } 
      if (logDeviceP) {
	  closeDevice(logDeviceP);
	  logDeviceP = 0;
      }
	  
  }

 startTran: STARTTRAN INTEGER
  {
      logTranRec_t tid;
      long code;

      Vprintf(("starting transaction %d",$2));
      code = elbb_StartTran("startTran", 0, dataDeviceP, &tid);
      afsl_MBZ(code);
      setTidMap($2, tid);
  }

 debug: DEBUG INTEGER STRING
  {
      /* debug tran_id debug_string */
      /*  $1     $2       $3        */
      u_int32 lsn, logFull;
      struct elbt_TranRec *tP;
      
      Vprintf(("debug transaction %d: %s",$2, $3));
      elbt_LookupTran(mapToTid($2), tP);
      elbl_Write(tP, LOGREC_DEBUG, strlen($3), $3);

  }

 data: DATA INTEGER  INTEGER INTEGER STRING 
  {
      /* data tran_id disk_block data_offset new_data */
      /*  $1    $2       $3         $4         $5     */
      struct buffer *bP;
      long code;

      Vprintf(("data modification @ (%d,%d) length %d",$3,$4,strlen($5)));
      code = elbb_Read(dataDeviceP, $3, bufLogged, &bP);
      afsl_MBZ(code);
      afsl_Assert(bP);

      code = elbb_Modify(bP, $4, $5, strlen($5), mapToTid($2), 1, &bP);
      afsl_MBZ(code);

      code = elbb_Release(bP, elbb_lazy);
      afsl_MBZ(code);

  }

 data2: DATA2 INTEGER INTEGER INTEGER INTEGER STRING STRING
  {
      /* this is used for setting the old value first */
      /* data2 tran_id data_length disk_block data_offset old_data new_data */
      /*  $1    $2       $3         $4         $5         $6        $7     */
      struct buffer *bP;
      long code;
      caddr_t data;

      Vprintf(("data modification @ (%d,%d) length %d",$4,$5,$3));
      code = elbb_Read(dataDeviceP, $4, bufLogged, &bP);
      afsl_MBZ(code);
      afsl_Assert(bP);

      if (strlen($6) != strlen($7)) {
	  yyerror("old data/new data size mismatch, cannot recover!");
	  exit(5);
      }

      data = osi_Alloc($3);
      UnCvtData(data, $6, $3);
      
      /* we munge the old data area */
      bcopy(data, bufferData(bP) + $5, $3);

      UnCvtData(data, $7, $3);
      
      /* now the modify call will capture the right OV/NV pair */
      code = elbb_Modify(bP, $5, data, $3, mapToTid($2), 1, &bP);
      afsl_MBZ(code);

      code = elbb_Release(bP, elbb_lazy);
      afsl_MBZ(code);

  }

 endTran: END INTEGER
  {
      /* end tran_id  */
      /*  $1    $2    */
      Vprintf(("end transaction %d",$2));
      elbb_EndTran(mapToTid($2));
  }

 rstrtDecl: RSTRT
  {
      /* rstrt tran_id data_length disk_block data_offset old_data new_data */
      /*  $1    $2       $3         $4         $5         $6       $7      */
  }

 recover: RECOVER
  {
      /* recover */
      /*   $1    */
      long code;
      daddr_t *logBlocks;
      u_int32 index, index2;

      Vprintf(("recover"));
      /* set log block array */
      
      logBlocks = (daddr_t *)osi_Alloc(sizeof(daddr_t) * nLogBlocks);
      for (index = 0; index < nLogBlocks; index++)
	  logBlocks[index] = index;

      /* call recovery */
      code = elbr_Recover(logDeviceP, dataDeviceP, logBlocks, nLogBlocks);
      Vprintf(("recover terminated with code %d", code));
      if (code)
	  com_err("elbr_Recover", code, "terminated");
      /* exit(code); */
  }

 dumplog: DUMPLOG
  {
  }

 force: FORCE INTEGER
  {
      struct elbt_TranRec *tP;

      elbt_LookupTran(mapToTid($2), tP);
      elbl_ForceTran(tP);
  }

 fill: FILL INTEGER INTEGER INTEGER CHARDATA INTEGER
  {
      /* fill tran_id disk_block data_offset pattern  length */
      /*  $1    $2       $3         $4         $5       $6   */
      struct buffer *bP;
      long code;

      code = elbb_Read(dataDeviceP, $3, bufData, &bP);
      afsl_MBZ(code);

      code = elbb_Fill(bP, $4, (unsigned char)$5, $6, 0, mapToTid($2),
			 1, &bP);
      afsl_MBZ(code);

      code = elbb_Release(bP, elbb_lazy);
      afsl_MBZ(code);
      
  }

 uzero: UZERO INTEGER INTEGER INTEGER INTEGER
  {
      /* uzero tran_id disk_block data_offset length */
      /*  $1      $2     $3           $4        $5   */
      struct buffer *bP;
      long code;

      code = elbb_Read(dataDeviceP, $3, bufData, &bP);
      afsl_MBZ(code);

      elbb_UndoZeroBuffer(bP, $4, $5, mapToTid($2), &bP);

      code = elbb_Release(bP, elbb_lazy);
      afsl_MBZ(code);

  }

 merge: MERGE INTEGER INTEGER
  {
      long code;

      code = elbt_ECMerge(mapToTid($2), mapToTid($3), 0);
      if (code) {
	  Vprintf(("elbt_ECMerge call failed with code %d",code));
	  com_err("elbt_ECMerge", code, " -- aborting");
	  exit(code);
      }
  }

 include: INCLUDE STRING
  {
      Vprintf(("including file %s",$2));
      (void)includeFile($2); /* reset lexer */
  }

 echo: COMMENT
  {
      /*
       * simply print out a placemarker - more for debugging testbuf
       * than anything else.
       */
      Vprintf(("echo"));
  }

 cmp: COMPARE INTEGER INTEGER STRING
  {   /* compare block#  offset string-to-compare */
      /*   $1     $2      $3         $4           */
     
      struct buffer *bP;
      long code;

      /* we must destroy left-over state to avoid time-warp */
      code = elbb_Kill(dataDeviceP, $2, 0, 0);
      afsl_MBZ(code);

      code = elbb_Read(dataDeviceP, $2, bufData, &bP);
      afsl_MBZ(code);
      afsl_Assert(bP);
      if (strncmp($4, bP->data + $3,strlen($4)) != 0) {
	  Printf(("compare FAILED for block %d, offset %d\n\twas: <%s>\n\tshould have been <%s>",$2,$3, bP->data + $3, $4));
	  failures++;
      } else {
	  Printf(("compare OK for block %d, offset %d",$2,$3));
      }

      code = elbb_Release(bP, elbb_lazy);
      afsl_MBZ(code);
  }

 ncmp: NCOMPARE INTEGER INTEGER STRING
  {   /* ncompare block#  offset string-to-compare */
      /*   $1     $2      $3         $4           */
      struct buffer *bP;
      long code;
      
      /* we must destroy left-over state to avoid time-warp */
      code = elbb_Kill(dataDeviceP, $2, 0, 0);
      afsl_MBZ(code);

      code = elbb_Read(dataDeviceP, $2, bufData, &bP);
      afsl_MBZ(code);
      afsl_Assert(bP);
      if (strncmp($4, bP->data + $3,strlen($4)) == 0) {
	  Printf(("ncompare FAILED for block %d, offset %d\n\twas: <%s>\n\tshould not have been <%s>",$2,$3, bP->data + $3, $4));
	  failures++;
      } else {
	  Printf(("ncompare OK for block %d, offset %d",$2,$3));
      }

      code = elbb_Release(bP, elbb_lazy);
      afsl_MBZ(code);
  }

 sync: SYNC
  {  /* sync */
     /*  $1  */
      elbb_SyncForce();
  }

 fsync: SYNCF INTEGER
  {
      /* fsync blockNum */
      /*   $1     $2    */
      long code;
      struct buffer *bP;
      daddr_t blockNum = $2;

      code = elbb_FsyncForce(dataDeviceP, 0, &blockNum, 1);
      afsl_MBZ(code);
  }

 quit: QUIT
  {
      Printf(("quit seen, terminating test"));
      exit(failures);
  }

%%

#include <stdio.h>

main(argc, argv)
     int argc;
     char **argv;
  {
      long code;
      extern FILE *yyin;

      progname = argv[0];
      if (argc != 2) {
	  fprintf(stderr,"usage: %s filename\n",progname);
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
      initialize_asy_error_table();
      
      initBufferSystem();

      yyparse();

      exit(failures);
  }

yyerror(s)
     char *s;
  {
      fprintf(stderr,"%s in file %s at line %d\n",s, filename, lineNumber);
  }


#define MAXLOCALTRANS (10)
static elbb_tranRec_t transactionMap[MAXLOCALTRANS];
/*
 * take an integer transaction # used by the script & convert
 * to the internal #
 */
elbb_tranRec_t mapToTid (tran)
     u_int32 tran;
  {
      return tran < MAXLOCALTRANS ? transactionMap[tran] : 0;
  }
/*
 * set the transaction #.  Use this to clear (set to zero)
 */
void setTidMap(tran, tid)
     u_int32 tran;
     elbb_tranRec_t tid;
  {
      afsl_Assert(tran < MAXLOCALTRANS);
      transactionMap[tran] = tid;
  }

/*
 * openDevice - open a log or data device.
 *
 * this is the code common to create and open.
 */
void openDevice(devName, blkSize, dP, devMinNum, nBlks)
     char *devName;
     u_int32 blkSize;
     struct async_device **dP;
     unsigned devMinNum;
     unsigned nBlks;
  {
      dev_t devNum;
      osi_off_t maxOffset;
      long code;

      afsl_Assert(dP);
      afsl_MBZ(*dP);

      code = asevent_InitDevice(US_MAJOR_DEV, devMinNum, blkSize, dP);
      if (code != 0) {
	  com_err("openDevice calling InitDevice", code, "");
	  exit(1);
      }
      afsl_Assert((*dP)->mindev == devMinNum);
      
      devNum = makedev(US_MAJOR_DEV, devMinNum);
      code = us_ioctl(devNum, EPIU_IOCTL_SET_DIR,"/");
      if (code != 0) {
	  com_err("openDevice setting directory", code, "");
	  exit(1);
      }

      code = us_ioctl(devNum, EPIU_IOCTL_SET_FILE, MakeDeviceName(devName));
      if (code != 0) {
	  com_err("openDevice setting file name", code, "");
	  exit(1);
      }
      
      code = us_open(devNum, 0);
      if (code != 0) {
	  com_err("openDevice calling us_open", code, "");
	  exit(1);
      }
      /*
       * our pseudo-device consists of nDataBlocks and nLogBlocks.
       * data is before log.
       */
      maxOffset = ((osi_off_t) nBlks) * ((osi_off_t) blkSize);
      code = us_ioctl(devNum, EPIU_IOCTL_SET_MAX_OFFSET, (caddr_t)&maxOffset);
      if (code != 0) {
	  com_err("openDevice setting max offset", code, "");
	  exit(1);
      }
      /* they should only be equal if they are both null */
      afsl_Assert((logDeviceP != dataDeviceP) || !logDeviceP);
  }

void closeDevice(dP)
  struct async_device *dP;
  {
      long code;
      dev_t devNum;

      code = asevent_CloseDevice(dP);
      afsl_MBZ(code);

      devNum = makedev(dP->majdev, dP->mindev);

      code = us_close(devNum, 0);
      afsl_MBZ(code);
  }

/*
 * initBufferSystem - initialize the buffer system.
 */
void initBufferSystem()
  {
      long code;
      struct elbb_params elbbParams;

      /*
       * initialize the buffer system; we don't need much memory.
       */
      elbbParams.byteCount = 1024 * 256;
      elbbParams.bufferPool = osi_Alloc(elbbParams.byteCount);
      code = elbb_Init (&elbbParams);
      if (code != 0) {
	  com_err("initBufferSystem elbb_Init", code, "");
	  exit(1);
      }

  }

/*
 * initLogSystem - initialize the log system.
 */
void initLogSystem()
  {
      long code;
      daddr_t *logBlocks;
      u_int32 index, index2;

      /* first, verify that both the data and log devices have been opened */
      if (!dataDeviceP) {
	  Printf(("initLogSystem called but data device not active"));
	  exit(7);
      }
      if (!logDeviceP) {
	  Printf(("initLogSystem called but log device not active"));
	  exit(9);
      }
	 
      /* set log block array */
      logBlocks = (daddr_t *)osi_Alloc(sizeof(daddr_t) * nLogBlocks);
      for (index = 0; index < nLogBlocks; index++)
	  logBlocks[index] = index;

      /* initialize log */
      code = elbb_LogInit (logDeviceP, dataDeviceP, logBlocks,
			     (unsigned)nLogBlocks);
      if (code != 0) {
	  com_err("initLogSystem elbb_LogInit", code, "");
	  exit(1);
      }
  }

/*
 * CvtData - convert hex representation of data to binary representation.
 * This call uses a static data area that is no longer valid upon a subsequent
 * call.
 */
PRIVATE void UnCvtData(outData, inData, length)
  OUT char *outData;
  IN char *inData;
  unsigned length;
  {
      char *cp;
      unsigned datum;

      cp = outData;

      while(length--) {
	  datum = (*inData < 'a') ? *inData - '0' : *inData - 'a';
	  inData++;
	  *cp = datum << 4;
	  datum = (*inData < 'a') ? *inData - '0' : *inData - 'a';
	  *cp += datum;
	  inData++;
	  cp++;
      }
      return;
  }

PRIVATE char *MakeDeviceName(shortName)
  char *shortName;
  {
      static char longName[1024];
      unsigned space = sizeof(longName), len;
      struct passwd *pw;

      len = strlen(shortName);
      afsl_Assert(space > len);
      strcpy(longName, shortName);
      space -= len;
      pw = getpwuid(getuid());
      afsl_Assert(pw);

      strcat(longName,".testbuf.");
      space -= 9;

      len = strlen(pw->pw_name);
      afsl_Assert(space >= len);
      strcat(longName, pw->pw_name);
      space -= len;

      return longName;
  }

PRIVATE void InitializeData (firstPage, lastPage, fillChar, devP)
  daddr_t firstPage;
  daddr_t lastPage;
  int fillChar;
  struct async_device *devP;
  {
      daddr_t index;
      struct buffer *bP;
      long code;

      if (firstPage > lastPage) {
	  Printf(("firstPage cannot be greater than lastPage"));
	  exit(1);
      }

      if (firstPage == lastPage) {
	  Vprintf(("initializing page %d",firstPage));
      } else {
	  Vprintf(("initializing pages %d to %d",firstPage, lastPage));
      }

      if ((firstPage > nDataBlocks) || (lastPage > nDataBlocks)) {
	  Printf(("specified range is out of bounds"));
	  exit(1);
      }

      for (index = firstPage; index <= lastPage; index++) {

	  code = elbb_ReadGeneral(dataDeviceP, index, 0, bufData, &bP);
	  afsl_MBZ(code);
	  afsl_Assert(bP);

	  code = elbb_Fill(bP, 0, fillChar, devP->blkSize, 0, 0, 0, &bP);
	  afsl_MBZ(code);

	  /* guarantee that zeroed bits are on the disk. */
	  code = elbb_Release(bP, elbb_sdel);
	  afsl_MBZ(code);
      }
  }

