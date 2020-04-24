/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2delupd.h,v $
 * Revision 1.1.10.2  1996/02/18  23:36:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:18  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:01:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:07  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:47:17  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  12:10:01  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:18:30  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:30:10  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:52:09  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:46:54  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:48:06  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:42:05  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:41:33  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:30:07  bbelch]
 * 
 * Revision 1.1.4.2  1992/10/05  13:07:35  marrek
 * 	Changes for bug 5205.
 * 	[1992/10/05  13:06:41  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  04:08:05  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:42:56  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D2DELUPD_H
#define _D2DELUPD_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d2delupd_rcsid[] = "@(#)$RCSfile: d2delupd.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:36:00 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYP		: INCLUDE					      */
/*								      */
/* NAME         : d2delupd.h                                          */
/*								      */
/* AUTHOR       : W. Schmid, D AP 11                                  */
/* DATE         : 25.1.1989                                           */
/*                                                                    */
/* DESCRIPTION  : This Include-File contains "typedefs" and "defines" */
/*                used for delta updates.                             */
/*								      */
/*datoff **************************************************************/

#include <dce/dce.h>
#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2dump.h>
/* (temporary) for util */
#include <d23apdu.h>


/* typedef used by d20_turn_shadowing_job()	*/
typedef enum {D2_ON = -1 , D2_OFF} d2_upd_status;


/* formats for D2_SHDLOG file */
/* format: - object name (terminated by @)     */
/*         - mk-index of superior              */
/*         - timestamp of logg                 */
/*         - pid of logging process (to make   */
/*           timestamp unique)                 */
/*         - operation id                      */
/*         - message length                    */
/*         - message (binary)                  */
					/* fprintf-format used to write*/
					/* the logging informations    */
					/* (message is not write by    */
					/* this format)                */
#define D2_WSHDLOG_FORM "%s@ %ld %ld:%d %d %ld "
					/* fscanf-format used to read  */
					/* the logging informations    */
					/* (message is not read by     */
					/* this format)                */
#define D2_RSHDLOG_FORM "%[^@]%*c %ld %ld%*c %d %d %ld%*c"
					/* No. of arguments of an     */
					/* log entry in the logg file */
					/* --> fscanf()               */
#define D2_RLOGPAR      6

/* formats for D21_SHD_JOBS file */
/* format: must be completed                      */
					/* printf-format used to store*/
					/* an update entry in the     */
					/* shadowing job file         */
#define D2_WJOB_FORM  "\n%d %d %d %d %d %d %d %d %d %s@ %d %s@ %ld %010ld"
					/* scanf-format used to read  */
					/* an update entry from the   */
					/* shadowing job file         */
#define D2_RJOB_FORM  "%d %d %d %d %d %d %d %d %d %[^@]%*c %d %[^@]%*c %ld %010ld"
					/* No. of arguments of an     */
					/* update entry in the        */
					/* shadowing job file         */
					/* --> fscanf()               */
#define D2_RJOBAR      14

					/* format for CACHE-JOB file  */
					/* handling                   */
#define D2_CACHE_JOB_FORM   "%d %d %d %d %d %d %d %d"
					/* No. of arguments of an     */
					/* update entry in the        */
					/* CACHE-JOB file             */
					/* --> fscanf()               */
#define D2_CACHE_JOB_PAR   8


#define D2_UPDATE_DIR   "/update"           /* name of directory for delta*/
					    /* update                     */
#define D2_ARCHIVE      "/update/archive"   /* name of the archive file:  */
					    /* the existence of this file */
					    /* indicates that the SHDLOG  */
					    /* files are to be archived   */
#define D2_SHDLOG       "/update/SHDLOG"    /* name of the logging file   */
#define D2_SAVE         "/update/SHDSAVE"   /* name of the save file      */
#define D2_LOGDAE       "/update/LOGDAEMON" /* name of the locking daemon */
#define D2_LOGJOBS      "/update/LOCKJOBS"  /* name of locking file to    */
					    /* synchronize accesses to    */
					    /* SHDJOBS file               */
#define D2_DSA_IDX      "/update/SHDDSA"    /* name of the special dsa    */
					    /* file                       */
#define D2_DSA_FONLY    "SHDDSA"            /* name of the special dsa    */
					    /* file                       */
#define D21_SHD_JOBS    "/update/SHDJOBS"   /* name of the shadowing job  */
					    /* file                       */
#define D21_TMP_SHD_JOBS "/update/TMPSHDJOBS"/* temporary shadowing job*/
					     /* file                   */
#define D21_CACHE_JOB   "/update/CACHEJOB"  /* name of the shadowing job  */
					    /* file for CACHE-JOB         */
#define D21_UPD_ERR      "/update/SHDUPD_ERR" /* error file of delta updates*/
#define D21_FUPD_ERR     "/update/UPD_FATERR" /* error file of delta updates*/
					      /* for fatal errors           */

#define D20_DAEMON       "/gdsdaemon"   /* name of the process which  */
					/* handles the update jobs    */
					/* contained in the shadowing */
					/* job file                   */
#define D20_CUP_PROC    "/gdshdlupd"    /* name of the shell script   */
					/* starting "D20_DAEMON"      */
#define D20_CUP_SCR     "/gdscrontab"   /* name of the shell script which */
					/* updates the 'cron'-table */
#define D20_EXEC        "/gdsexec"      /* name of program that executes */
					/* a given command               */
#define D20_CA_UP       "/gdshdlcache"  /* Progname for cache update      */

#define D21_MAX_LOG_SIZE	10000	/* max. size of the log-file used */
					/* to store fatal delta update errors */

#define D21_MAX_LOG_DATA	80	/* max. size of the log-data    */
#define D21_MAX_LOG_COMM	50	/* max. size of the log-comment */

#define REMOVE_ALL     10       /* removes all overlapping jobs       */
#define REMOVE         11       /* removes one single job             */


				/* counter for                        */
#define D21_NMIN       12       /* minutes                            */
#define D21_NHOUR      24       /* hours                              */
#define D21_NWDAY       7       /* days of week                       */
				/* (to be used for generation of      */
				/* "crontab" entry)                   */
				/* = number of distinguished values   */
				/* in jobfile                         */

				/* total number of components in time */
				/* occurrency array                   */
#define D21_T_OCC       (D21_NMIN+D21_NHOUR+D21_NWDAY+1)

				/* total number of components in time */
				/* occurrency array as sizeof int     */
#define D21_T_OFF       (D21_T_OCC * sizeof (int))

#define D21_NDIST_VAL   12      /* number of distinguished values     */
				/* in "crontab" entry                 */

#define D21_IDX_NOT_DEF (-101)  /* index not yet defined              */

#define D21_ROOT        "ROOT"  /* entry in shadowing job file        */
				/* representing subtree of ROOT       */


/* return-values of function update_dsa */
#define D2_UPD_ERR      (-1)    /* error on update of object in      */
				/* remote DSA                        */
				/* continue with next object of same */
				/* DSA                               */
#define D2_NEXT_DSA     (-2)    /* remote DSA not available          */
				/* continue with next DSA            */
#define D2_FATAL_ERR    (-3)    /* IPC error occurred                */
				/* do not continue working           */

/* defines for error file SHDUPD_ERR */
#define D2_A_OBJ         "add object"       /* OP-CODE: add object    */
#define D2_M_OBJ         "modify entry"     /* OP-CODE: modify entry  */
#define D2_M_RDN         "replace rdn"      /* OP-CODE: replace rdn   */
#define D2_R_OBJ         "remove object"    /* OP-CODE: remove object */
#define D21_UPDMSG	 "update error"	

/* --- scripts used to do some operations on the SHDERR-file --- */

	/* shell script used to search for a specific entry in the error-file */

#define SEARCHCMD "\
if [ -s %s%s ]\nthen\n\
awk ' BEGIN { LABELSIZE = 14 ; NOOBJ = 0 ; MATCH = 0 ; ACTION = 0 ; FIRST = 4 ; NOTFIRST = 8 }\n\
      $1 == \"TIME-STAMP:\"  { TIMPAR = substr ($0, LABELSIZE) }\n\
      $1 == \"DSA-NAME:\"    { DSAPAR = substr ($0, LABELSIZE) }\n\
      $1 == \"OBJECT-NAME:\" { OBJPAR = substr ($0, LABELSIZE) }\n\
      $1 == \"ACTION:\"      { ACTPAR = substr ($0, LABELSIZE) }\n\
      $1 ~ /---/ && DSAPAR == DSANAME && OBJPAR == OBJNAME { MATCH = 2 }\n\
      $1 ~ /---/ && MATCH == 2 && TIMPAR == TIMESTAMP  { NOOBJ++ ; ACTION = ACTPAR ; exit }\n\
      $1 ~ /---/ { MATCH = 0 }\n\
      END { \
	if (NOOBJ == 0) {\n\
		printf \"1\"\n\
	} else {\n\
		if (NOOBJ == 1) {\n\
			printf \"%%d\", FIRST + MATCH + ACTION\n\
		} else {\n\
			printf \"%%d\", NOTFIRST + MATCH + ACTION\n\
		}\n\
	}\n\
      }' \\\n\
      TIMESTAMP=\"%ld:%d\" \\\n\
      OBJNAME=\"%s\" \\\n\
      DSANAME=\"%s\" \\\n\
      %s%s 2> /dev/null\n\
else\necho 1\nfi\n"

	/* shell script used to remove an entry from the error-file */

#define DELCMD "\
awk ' BEGIN { LABELSIZE = 14 }\n\
      $1 == \"TIME-STAMP:\"  { TIMPAR = $0; TMSTAMP = $2 }\n\
      $1 == \"ERROR-TYPE:\"  { ERRTYP = $0 }\n\
      $1 == \"DIR.-ID:\"     { DIRID  = $0 }\n\
      $1 == \"DSA-NAME:\"    { DSAPAR = $0; DSAVAL = substr ($0, LABELSIZE) }\n\
      $1 == \"OBJECT-NAME:\" { OBJPAR = $0 }\n\
      $1 == \"OP-CODE:\"     { OPCODE = $0 }\n\
      $1 == \"ERROR-TIME:\"  { ERRTIM = $0 }\n\
      $1 == \"ERROR-CODE:\"  { ERRCOD = $0 }\n\
      $1 == \"ERROR-COUNT:\" { ERRCNT = $0 }\n\
      $1 == \"ACTION:\"      { ACTPAR = $0 }\n\
      $1 ~ /---/ && (TMSTAMP != TIMESTAMP || DSAVAL != DSANAME) { \\\n\
		print TIMPAR; print ERRTYP; print DIRID; print DSAPAR;\n\
		print OBJPAR; print OPCODE; print ERRTIM; print ERRCOD;\n\
		print ERRCNT; print ACTPAR; print $0 } ' \\\n\
      TIMESTAMP=\"%ld:%d\" \\\n\
      DSANAME=\"%s\" \\\n\
      %s%s 2> /dev/null > %s%s.$$\n\
      mv %s%s.$$ %s%s"

	/* shell script used to remove DSA-specific entries from the error-file */

#define DELDSACMD "\
awk 'BEGIN { LABELSIZE = 14 }\n\
      $1 == \"TIME-STAMP:\"  { TIMPAR = $0 }\n\
      $1 == \"ERROR-TYPE:\"  { ERRTYP = $0 }\n\
      $1 == \"DIR.-ID:\"     { DIRID  = $0 }\n\
      $1 == \"DSA-NAME:\"    { DSAPAR = $0; DSAVAL = substr ($0, LABELSIZE) }\n\
      $1 == \"OBJECT-NAME:\" { OBJPAR = $0 }\n\
      $1 == \"OP-CODE:\"     { OPCODE = $0 }\n\
      $1 == \"ERROR-TIME:\"  { ERRTIM = $0 }\n\
      $1 == \"ERROR-CODE:\"  { ERRCOD = $0 }\n\
      $1 == \"ERROR-COUNT:\" { ERRCNT = $0 }\n\
      $1 == \"ACTION:\"      { ACTPAR = $0 }\n\
      $1 ~ /---/ && DSAVAL != DSANAME { \\\n\
		print TIMPAR; print ERRTYP; print DIRID; print DSAPAR;\n\
		print OBJPAR; print OPCODE; print ERRTIM; print ERRCOD;\n\
		print ERRCNT; print ACTPAR; print $0 } ' \\\n\
      DSANAME=\"%s\" \\\n\
      %s%s 2> /dev/null > %s%s.$$\n\
      mv %s%s.$$ %s%s"

	/* shell script used to change an entry within the error-file */

#define CHANGECMD "\
awk ' $1 == \"TIME-STAMP:\"  { TIMPAR = $0; TMSTAMP = $2 }\n\
      $1 == \"ERROR-TYPE:\"  { ERRTYP = $0 }\n\
      $1 == \"DIR.-ID:\"     { DIRID  = $0 }\n\
      $1 == \"DSA-NAME:\"    { DSAPAR = $0 }\n\
      $1 == \"OBJECT-NAME:\" { OBJPAR = $0 }\n\
      $1 == \"OP-CODE:\"     { OPCODE = $0 }\n\
      $1 == \"ERROR-TIME:\"  { ERRTIM = $0 }\n\
      $1 == \"ERROR-CODE:\"  { ERRCOD = $0 }\n\
      $1 == \"ERROR-COUNT:\" { ERRCNT = $0; ERRORVAL = $2 }\n\
      $1 == \"ACTION:\"      { ACTPAR = $0 }\n\
      $1 ~ /---/ { print TIMPAR; print ERRTYP; print DIRID; print DSAPAR;\n\
		   print OBJPAR; print OPCODE }\n\
      $1 ~ /---/ && TMSTAMP != TIMESTAMP { print ERRTIM; print ERRCOD; print ERRCNT }\n\
      $1 ~ /---/ && TMSTAMP == TIMESTAMP { print \"ERROR-TIME:  \"ERRORTIME ;\n\
					   print \"ERROR-CODE:  \"ERRORCODE ;\n\
					   print \"ERROR-COUNT: \"ERRORVAL+1 }\n\
      $1 ~ /---/ { print ACTPAR; print $0 } ' \\\n\
      TIMESTAMP=\"%ld:%d\" \\\n\
      ERRORTIME=\"%d-%d-%d/%d:%02d\" \\\n\
      ERRORCODE=\"%d %d %d\" \\\n\
      %s%s 2> /dev/null > %s%s.$$\n\
      mv %s%s.$$ %s%s"

#define APPENDCMD	"\
TIME-STAMP:  %ld:%d\n\
ERROR-TYPE:  %s\n\
DIR.-ID:     %d\n\
DSA-NAME:    %s\n\
OBJECT-NAME: %s\n\
OP-CODE:     %s\n\
ERROR-TIME:  %d-%d-%d/%d:%02d\n\
ERROR-CODE:  %d %d %d\n\
ERROR-COUNT: %d\n\
ACTION:      %d\n\
-------------------------------------------------------------------------------\n"

/* Fatal error messages used by the 'delta update'-daemon */

#define D21_0FATFORM	"\n%s(line %3d) FATAL ERROR: "
#define D21_1FATMSG	"Usage: %s -d<dir-id> -m<op-mode> [-n<job-no>]"
#define D21_2FATMSG	"wrong switch syntax (%s)"
#define D21_3FATMSG	"unknown switch (%s)"
#define D21_4FATMSG	"illegal operation mode resp. directory identifier"
#define D21_5FATMSG	"no directory system installation found"
#define D21_6FATMSG	"%d - can't open lock-file (%s)"
#define D21_7FATMSG	"%d - can't lock lock-file"
#define D21_8FATMSG	"can't synchronize DSA-processes"
#define D21_9FATMSG	"%d - can't link SHDLOG-file (%s)"
#define D21_10FATMSG	"%d - can't unlink SHDLOG-file (%s)"
#define D21_11FATMSG	"can't resynchronize DSA-processes"
#define D21_12FATMSG	"%d - can't open temporary SHDLOG-file (%s)"
#define D21_13FATMSG	"%d - can't get file information above the SHDLOG-file"
#define D21_14FATMSG	"%d - can't open SHDLOG-SAVE file (%s)"
#define D21_15FATMSG	"can't get system memory"
#define D21_16FATMSG	"%d - can't open local DSA-name file (%s)"
#define D21_17FATMSG	"%d - can't read name of the local DSA"
#define D21_18FATMSG	"can't get DN-index of the local DSA"
#define D21_19FATMSG	"can't read header informations from SHDJOB-file"
#define D21_20FATMSG	"can't get DN-index from DSA-name (%s)"
#define D21_21FATMSG	"insufficient memory"
#define D21_22FATMSG	"%d - can't open temporary SHDJOB-file (%s)"
#define D21_23FATMSG	"%d - can't write header informations to temporary SHDJOB-file (%s)"
#define D21_24FATMSG	"%d - can't unlink SHDJOBS-file (%s)"
#define D21_25FATMSG	"%d - can't link temporary SHDJOBS-file (%s)"
#define D21_26FATMSG	"%d - can't unlink temporary SHDJOBS-file (%s)"
#define D21_27FATMSG	"%d - can't open SHDJOB file (%s)"
#define D21_28FATMSG	"%d - can't open DSA-specific file (%s)"
#define D21_29FATMSG	"can't execute search command"
#define D21_30FATMSG	"can't read search state"
#define D21_31FATMSG    "can't get DSA-name from DN-index %ld"
#define D21_32FATMSG	"update operations impossible (err-value = %d)"
#define D21_33FATMSG    "can't change error log-entry (timestamp = %ld)"
#define D21_34FATMSG	"%d - can't open error information file (%s)"
#define D21_35FATMSG	"%d - can't open temporary DSA-specific file (%s)"
#define D21_36FATMSG	"%d - can't unlink DSA-specific file (%s)"
#define D21_37FATMSG	"%d - can't link temporary DSA-specific file (%s)"
#define D21_38FATMSG	"%d - can't unlink DSA-specific file (%s)"
#define D21_39FATMSG	"can't remove unused entries from error log-file"
#define D21_40FATMSG	"can't realloc memory"
#define D21_41FATMSG	"%d - can't read log-message"
#define D21_42FATMSG	"can't read complete log-message (rcnt = %d)"
#define D21_43FATMSG	"%d - can't write entry to error log-file"
#define D21_44FATMSG    "%d - can't delete entry from error log-file (timestamp = %ld)"
#define D21_45FATMSG	"%d - can't open update directory (%s)"
#define D21_46FATMSG	"%d - can't attach client"

/* typedefs to read the data from file with the formats given above */

/* data of D21_SHD_JOBS */
typedef struct {
	signed32  d2_jstate;
	signed32  d2_jnr;
	Upd_time  d2_upd_t;
	char     *d2_jobjname;
	signed32  d2_jsubset;
	char     *d2_jdsaname;
	signed32  d2_jdsa_index;
	signed32  d2_jtimestamp;
} D2_jobentry;

/* data of D2_SHDLOG */
typedef struct {
	char     *d2_lobjname;            /* object name            */
	signed32  d2_lsupindex;           /* mk index of superior   */
	signed32  d2_ltimestamp;          /* log timestamp          */
	signed32  d2_llpid;               /* pid of logging process */
	signed32  d2_lopid;               /* operation id           */
	signed32  d2_lmsgsize;            /* message size           */
	char     *d2_lmsg;                /* log message            */
} D2_logentry;

/*************/
/* from dsa (temporary )*/
/*************/

/* from d262dnlis.c */

signed32 d26_i26_get_idx_from_file (boolean last_operation,
    D2_name_string dn, signed32 *dnlist_idx);
signed32 d26_i27_get_dn_from_file (boolean last_operation,
    signed32 dnlist_idx, D2_name_string dn);

#endif /* _D2DELUPD_H */
