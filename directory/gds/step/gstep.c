/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: gstep.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:55  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:26  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:46:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:42  root]
 * 
 * Revision 1.1.2.12  1994/10/05  15:39:23  keutel
 * 	OT 12481: multiple routing
 * 	[1994/10/05  14:37:48  keutel]
 * 
 * Revision 1.1.2.11  1994/09/19  08:39:41  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:34:05  marrek]
 * 
 * Revision 1.1.2.10  1994/09/07  15:11:07  keutel
 * 	OT 12055
 * 	[1994/09/07  13:16:50  keutel]
 * 
 * Revision 1.1.2.9  1994/09/06  20:41:27  bowe
 * 	Rework args to gds_gst_print_data() [CR 12055]
 * 	[1994/09/06  20:40:48  bowe]
 * 
 * Revision 1.1.2.8  1994/09/06  16:30:17  keutel
 * 	OT 12055: dce_svc_log_get_arg will become obsolete
 * 	[1994/09/06  16:10:36  keutel]
 * 
 * Revision 1.1.2.7  1994/08/24  12:15:07  keutel
 * 	OT 11802
 * 	[1994/08/23  16:05:57  keutel]
 * 
 * Revision 1.1.2.6  1994/07/19  14:49:01  marrek
 * 	HP warnings removed.
 * 	[1994/07/19  12:14:45  marrek]
 * 
 * Revision 1.1.2.5  1994/07/13  19:40:15  keutel
 * 	OT 11276: core dump in asn1_dump, ...
 * 	[1994/07/13  19:04:03  keutel]
 * 
 * Revision 1.1.2.4  1994/06/21  14:48:06  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:45:29  marrek]
 * 
 * Revision 1.1.2.3  1994/06/10  21:16:01  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:12  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:03:10  marrek
 * 	Use correct rcs id string.
 * 	[1994/05/02  14:47:48  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:22:59  marrek]
 * 
 * 	Bug fixes.
 * 	[1994/04/29  09:30:58  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:11:47  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:47:08  keutel
 * 	creation
 * 	[1994/03/21  15:55:10  keutel]
 * 
 * $EndLog$
 */
/**********************************************************************/
/*                                                                    */
/* Component    : GDS                                                 */
/*                                                                    */
/* File name    : gstep.c                                             */
/*                                                                    */
/* Description  : This file contains the main function of the logfile */
/*                evaluation tool gdsstep of the GDS component.       */
/*                                                                    */
/* Date         : 12/08/93                                            */
/*                                                                    */
/* Author       : E. Kraemer, BUBA NM12                               */
/*                                                                    */
/**********************************************************************/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gstep.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:55 $";
#endif

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <gds.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#if defined(_AIX)
#include <sys/select.h>
#endif

#include <locale.h>

#include <gstep.h>
#include <d2dir.h>
#include <d23ipc.h>
#include <d25stub.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

			/* constants for exit values                  */
#define GST_MSGTAB_ERR  1           /* message table error            */
#define GST_WRONG_USAGE 2           /* wrong usage                    */
#define GST_OPEN_ERROR  3           /* logfile could not be opened    */
#define GST_READ_ERROR  4           /* logfile could not be processed */
#define GST_SEEK_ERROR  5           /* entry point in logfile not     */
				    /* found                          */
#define GST_CLOSE_ERROR 6           /* logfile could not be closed    */

			/* constants for profile bits                 */
#define GST_SVC_VERSION (1 << 0)    /* SVC Version logging            */
#define GST_TIME_STAMP  (1 << 1)    /* Time stamp logging             */
#define GST_ATTRIBUTES  (1 << 2)    /* Attributes logging             */
#define GST_MESS_INDEX  (1 << 3)    /* Message index logging          */
#define GST_THREAD_ID   (1 << 4)    /* Thread-ID logging              */
#define GST_SUBCOMP     (1 << 5)    /* Subcomponent logging           */
#define GST_FILENAME    (1 << 6)    /* Source file name logging       */
#define GST_PROGNAME    (1 << 7)    /* Program name logging           */

#define GST_HEXDUMP     (1 << 8)    /* Hex dump logging of binary data*/

			/* constants for formatting hex dump          */
#define GST_BYTES_PER_LINE 16

			/* maximum number of tolerated errors         */
#define GST_MAX_ERR_COUNT   64

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

			/* table of strings for severity levels       */
extern const char *dce_svc_g_code_table[5];
			/* indicates prolog elements to be logged     */
unsigned32 gds_gst_profile   = 0L;

			/* RTROS-logging filter program */
char *rtrosfilter[] = { "/opt/dcelocal/bin/gdsrosstep", (char *) NULL } ;
			/* CMX-logging filter program */
char *cmxfilter[]   = { "/opt/dcelocal/bin/gdscmxl", "-Dv", (char *) NULL } ;

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

/* IMPORT */

extern int main(int argc, char *argv[]);

/* EXPORT */

void gds_gst_print_binary_data(unsigned32 size, pointer_t data);

/* STATIC */

static void gds_gst_process_logfile(dce_svc_log_handle_t handle,
	unsigned32 thread_id, unsigned32 max_level, char *subcomp,
	error_status_t *status);
static void gds_gst_print_prolog(dce_svc_log_prolog_t prolog);
static void gds_gst_print_data(dce_svc_log_prolog_t prolog,
						error_status_t *status);
static void gds_gst_print_data_element(const char *format, char convchar,
	unsigned32 size, pointer_t data);
static void gds_gst_print_stub_func_info (char *format, signed32 call_no,
					       unsigned32 size, pointer_t info);
static void gds_gst_print_stub_stev_info (char *format, signed32 call_no,
					       unsigned32 size, pointer_t info);
static void gds_gst_print_fd_info (char *format, unsigned32 size,
								pointer_t info);
static void gds_gst_print_filter_info (char *prog[], FILE **rqfp, FILE **rsfp,
			    boolean complete, unsigned32 size, pointer_t info) ;


/**********************************************************************/
/*                                                                    */
/* Function name: main                                                */
/*                                                                    */
/* Description  : The function openes the logfile and reads the log   */
/*                records up to the end of the file. For every read   */
/*                record a function is called, which has to prepare   */
/*                it for readability.                                 */
/*                                                                    */
/* Exit value   :                                                     */
/*      0                           logfile completely processed      */
/*      GST_WRONG_USAGE             wrong call                        */
/*      GST_OPEN_ERROR              logfile could not be opened       */
/*      GST_READ_ERROR              logfile could not be processed    */
/*      GST_SEEK_ERROR              entrypoint in logfile not found   */
/*      GST_CLOSE_ERROR             logfile could not be closed       */
/*                                                                    */
/**********************************************************************/

main(
    int argc,           /* IN    - count of arguments                 */
    char **argv)        /* IN    - list of arguments                  */

{                       /* main                                       */

  int flag;             /* command line flag                          */
			/* handle for the logfile                     */
  dce_svc_log_handle_t handle;
			/* indicates logfile offset
  off_t      log_offset = 0L; only if called from gdsfollowlog        */
			/* indicates maximum debug level              */
  unsigned32 max_level  = svc_c_debug9;
			/* indicates thread ID                        */
  unsigned32 thread_id  = 0L;
  char       *subcomp;  /* indicates subcomponent                     */

  error_status_t status;/* error status for SVC calls                 */

    /* set the locale */
  setlocale(LC_ALL, "");

    /* load message table */
  dce_msg_define_msg_table(gds_msg_table,
    sizeof(gds_msg_table) / sizeof(gds_msg_table[0]),&status);

  if                    /* message table cannot be loaded             */
    (status != msg_s_ok)
  {                     /* print error message                        */
    fprintf(stderr,"Cannot define message table, error 0x%lx\n",status);
    exit(GST_MSGTAB_ERR);
  }

  subcomp = NULL;
  while                 /* command line flags to parse                */
       ((flag = getopt(argc,argv,"vtfhl:s:i:o:")) != EOF)
  {                     /* check the command line flag                */
    switch(flag)
    {
      case 'v':
	gds_gst_profile |= GST_SVC_VERSION | GST_PROGNAME | GST_THREAD_ID |
	  GST_TIME_STAMP | GST_ATTRIBUTES | GST_MESS_INDEX | GST_SUBCOMP |
	  GST_FILENAME;
	break;
      case 't':
	gds_gst_profile |= GST_TIME_STAMP;
	break;
      case 'f':
	gds_gst_profile |= GST_FILENAME;
	break;
      case 'h':
	gds_gst_profile |= GST_HEXDUMP;
	break;
      case 'l':
	max_level = atol(optarg);
	if              /* debug level invalid                        */
	  ((signed32)max_level < svc_c_debug_off || svc_c_debug9 < max_level)
	{               /* print usage hint                           */
	  dce_fprintf(stderr,GST_INV_DEBUG_LEVEL,max_level);
	  exit(GST_WRONG_USAGE);
	}
	break;
      case 's':
	subcomp = optarg;
	break;
      case 'i':
	thread_id = atol(optarg);
	break;
/*  This option is set by gdsfollowlog
      case 'o':
	log_offset = atol(optarg);
	break;
*/
    }
  }
  argc -= optind;
  argv += optind;
  if                    /* wrong parameters                           */
    (argc < 1)
  {                     /* print usage hint                           */
    dce_fprintf(stderr,GST_NO_LOGFILE);
    exit(GST_WRONG_USAGE);
  }
  else
  {                     /* open logfile                               */
    dce_svc_log_open(argv[0],&handle,&status);
    if                  /* logfile could not be opened                */
      (status != svc_s_ok)
    {                   /* print status and exit                      */
      dce_fprintf(stderr,GST_LOGOPEN_ERR,status);
      exit(GST_OPEN_ERROR);
    }
    else
    {                   /* seek for entry point (only for gdsfollowlog)
      dce_svc_log_seek(handle,log_offset,&status);                    */
      if                /* entry point could not be found             */
	(status != svc_s_ok)
      {                 /* error exit                                 */
	dce_fprintf(stderr,GST_OFFSET_ERR,status);
	exit(GST_SEEK_ERROR);
      }
			/* process logfile                            */
      gds_gst_process_logfile(handle,thread_id,max_level,subcomp,&status);
      if                /* logfile could not be processed completely  */
	(status != svc_s_ok && status != svc_s_at_end)
      {                 /* error exit                                 */
	exit(GST_READ_ERROR);
      }
      dce_svc_log_close(handle,&status);
      if                /* logfile could not be closed                */
	(status != svc_s_ok)
      {                 /* print status and exit                      */
	dce_fprintf(stderr,GST_LOGCLOSE_ERR,status);
	exit(GST_CLOSE_ERROR);
      }
    }
  }

  exit(0);

}                       /* main                                       */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_process_logfile                             */
/*                                                                    */
/* Description  : The function reads the records of the logfile       */
/*                subsequently and prepares the content to a readable */
/*                form.                                               */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_process_logfile(
			/* INOUT - handle for logfile                 */
    dce_svc_log_handle_t handle,
			/* IN    - thread ID for thread to print      */
    unsigned32     thread_id,
			/* IN    - maximum debug level to print       */
    unsigned32     max_level,
			/* IN    - subcomponent to print              */
    char           *subcomp,
			/* INOUT - error status of function           */
    error_status_t *status)

{                       /* gds_gst_process_logfile                    */

			/* prolog for the records                     */
  dce_svc_log_prolog_t prolog;
			/* count of errors                            */
  signed16  err_count = 0;
			/* argument index                             */

  while                 /* still log entries to process               */
       (*status != svc_s_at_end && err_count < GST_MAX_ERR_COUNT)
  {                     /* read next record                           */
    dce_svc_log_get(handle,&prolog,status);
    if                  /* no error occurred                          */
      (*status == svc_s_ok)
    {                   /* check requirements                         */
      if                /* thread ID, debug level and subcomponent    */
			/* match                                      */
	((thread_id == 0L ||
	  thread_id == pthread_getunique_np(&prolog->thread_id)) &&
	 (prolog->attributes & svc__c_debugmask) <= max_level &&
	 (subcomp == NULL || !strcmp(subcomp,prolog->fac_name)))
      {                 /* print prolog and data                      */
	gds_gst_print_prolog(prolog);
	gds_gst_print_data(prolog,status);
      }
    }
    else if             /* end of file not reached                    */
	   (*status != svc_s_at_end)
    {                   /* print error message                        */
      err_count++;
      printf("Couldn't read log record, error_status: 0x%lx errno: %d\n",
	*status,errno);
    }
  }

  if                    /* error occurred                             */
    (err_count >= GST_MAX_ERR_COUNT)
  {                     /* print error message                        */
    dce_fprintf(stderr,GST_ERR_OVERFLOW);
  }

}                       /* gds_gst_process_logfile                    */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_prolog                                */
/*                                                                    */
/* Description  : The function prints prolog data as specified by     */
/*                setting of global variables.                        */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_prolog(
			/* IN    - prolog of logged record            */
    dce_svc_log_prolog_t prolog)

{                       /* gds_gst_print_prolog                       */

			/* memory for UTC ASCII time                  */
  char time_stamp[UTC_MAX_STR_LEN];

  if                    /* Serviceability version should be logged    */
    (gds_gst_profile & GST_SVC_VERSION)
  {                     /* print SVC version                          */
    printf("SVC Version: %d ",prolog->version);
  }

  if                    /* program name should be logged              */
    (gds_gst_profile & GST_PROGNAME)
  {                     /* print program name                         */
    printf("Program: %s ",prolog->progname);
  }

  if                    /* Thread ID should be printed                */
    (gds_gst_profile & GST_THREAD_ID)
  {                     /* print thread ID                            */
    printf("Thread ID: 0x%lx ",pthread_getunique_np(&prolog->thread_id));
  }

  if                    /* Logging time should be printed             */
    (gds_gst_profile & GST_TIME_STAMP)
  {                     /* print time stamp                           */
    (void)utc_asclocaltime(time_stamp,sizeof(time_stamp),&prolog->t);
    printf("Time stamp: %s ",time_stamp);
  }

  if                    /* Attributes should be printed               */
    (gds_gst_profile & GST_ATTRIBUTES)
  {                     /* print attributes                           */
    if                  /* attributes contain severity level          */
      (prolog->attributes & svc__c_mask)
    {                   /* print severity level                       */
      printf("Severity level: %s ",
	dce_svc_g_code_table[(prolog->attributes >> svc__c_shift) - 1]);
    }
    else
    {                   /* print debug level                          */
      printf("Debug level: %ld ",prolog->attributes);
    }
  }

  if                    /* Message index should be printed            */
    (gds_gst_profile & GST_MESS_INDEX)
  {                     /* print message index                        */
    printf("Message index: 0x%lx ",prolog->message_index);
  }

  if                    /* Subcomponent should be printed             */
    (gds_gst_profile & GST_SUBCOMP)
  {                     /* print subcomponent name                    */
    printf("Subcomponent: %s ",prolog->fac_name);
  }

  if                    /* File name should be printed                */
    (gds_gst_profile & GST_FILENAME)
  {                     /* print source file name                     */
    printf("Sourcefile: %s Line: %d ",prolog->file,prolog->line);
  }

  if                    /* any prolog information should be printed   */
    (gds_gst_profile != 0L)
  {                     /* print newline                              */
    printf("\n");
  }

}                       /* gds_gst_print_prolog                       */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_data                                  */
/*                                                                    */
/* Description  : The function prints the logged data as specified    */
/*                by the message.                                     */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_data(
			/* IN    - prolog of logged record            */
    dce_svc_log_prolog_t prolog,
			/* INOUT - error status of function           */
    error_status_t *status)

{                       /* gds_gst_print_data                         */

  char *format;         /* format string for printf calls             */
  char *fel;            /* format element in format string            */
  char *dollar;         /* dollar character in format element         */
  char *eof;            /* end of format string                       */
  char convchar;        /* conversion character of format string      */
  char savchar;         /* saves character of format string           */
  pointer_t  data;      /* data from logfile                          */
  unsigned32 size;      /* size of data from logfile                  */
			/* number of subordinate function call        */
  boolean nline = TRUE;	/* print new line at end of log entry         */

  static FILE	*rtrosrq_fp, *rtrosrs_fp ;
  static FILE	*cmxrq_fp, *cmxrs_fp ;

  register signed16 call_nr = 0;
  unsigned32 arg_index = 0;

  format     = prolog->message_text;

  if                    /* performance message is logged with         */
			/* subcomponent 'general'                     */
    (GDS_S_FCT_IS_START_READ <= prolog->message_index &&
     prolog->message_index <= GDS_S_INT_SEARCH_COMPL &&
     strcmp(prolog->fac_name,(char *)gds_svc_table[GDS_S_GENERAL].sc_name)
     == 0)
  {                     /* search for newline character               */
    if                  /* newline embedded in format string          */
      ((eof = strchr(format,'\n')) != NULL)
    {                   /* replace it by end of string                */
      *eof = '\0';
    }
  }
  while                 /* still message text to process              */
       (*format != '\0')
  {                     /* find next format element                   */
    if                  /* no format element present                  */
      ((fel = strchr(format,'%')) == NULL)
    {                   /* just print the rest of the message         */
      printf(format);
      break;
    }
    else
    {                   /* find conversion character                  */
      if                /* no conversion character present            */
	((eof = strpbrk(fel + 1,"bcCdiouxXeEfgGsSnp%")) == NULL)
      {                 /* print error message                        */
	dce_printf(GST_CONVCHAR_MISSING,prolog->message_index);
	printf("\n");
	break;
      }
      else
      {                 /* assign conversion character                */
	convchar = *eof++;
	savchar  = *eof;
	*eof     = '\0';
	if              /* dollar embedded in format element          */
	  ((dollar = strchr(fel + 1,'$')) != NULL)
	{               /* overwrite format element                   */
	  strcpy(fel + 1,dollar + 1);
	}
	gds_dce_svc_log_get_arg(prolog,&arg_index,&data,&size,status);
	if              /* error occurred                             */
	  (*status != svc_s_ok)
	{               /* print error message                        */
	  dce_printf(GST_LOGREAD_ERR,*status);
	  printf("\n");
	  break;
	}
	else
	{               /* check whether output is in hex             */
	  if            /* binary data must be prepared as hex dump   */
	    (gds_gst_profile & GST_HEXDUMP)
	  {             /* just print data element                    */
	    gds_gst_print_data_element(format,convchar,size,data);
	  }
	  else
	  {             /* check message index                        */
	    switch(prolog->message_index)
	    {
	      case GDS_S_ENTRY_OPERATION:
	      case GDS_S_EXIT_OPERATION:
		gds_gst_print_op_id(format,size,data);
		break;
	      case GDS_S_ILL_SCHEMA_NAME:
	      case GDS_S_ENTRY_FOUND:
	      case GDS_S_ALIAS_SUB_SEARCH:
	      case GDS_S_ALIAS_FOUND:
	      case GDS_S_ACC_POINT_FOUND:
	      case GDS_S_TARGET_DSA:
	      case GDS_S_FMATCH_ST_ROOT:
		gds_gst_print_entry(format,size,data);
		break;
	      case GDS_S_ILL_DN_INTRPRTN:
		gds_gst_print_int_and_name(format,call_nr,size,data);
		break;
	      case GDS_S_LOC_NRS_BROKEN:
	      case GDS_S_INT_SEARCH_COMPLETE:
		gds_gst_print_rv_int_search(format,size,data);
		break;
	      case GDS_S_FAT_CHECK:
		gds_gst_print_rv_fa_check(format,size,data);
		break;
	      case GDS_S_ACL_CHECK:
		gds_gst_print_rv_acl_check(format,size,data);
		break;
	      case GDS_S_IND_BO_TYPE:
		gds_gst_print_bo_filter(format,call_nr,size,data);
		break;
	      case GDS_S_IND_AT_TYPE:
		gds_gst_print_filter_type(format,call_nr,size,data);
		break;
	      case GDS_S_IND_NAM_AT_TYPE:
	      case GDS_S_UNKNOWN_ATYPE_IN_FILTER:
		gds_gst_print_name_filter(format,size,data);
		break;
	      case GDS_S_WR_REP_IN_FILTER:
		gds_gst_print_type_and_rep(format,call_nr,size,data);
		break;
	      case GDS_S_WR_POS_IN_FILTER:
		gds_gst_print_type_and_pos(format,call_nr,size,data);
		break;
	      case GDS_S_WR_MATCH:
		gds_gst_print_int_and_syntax(format,call_nr,size,data);
		break;
	      case GDS_S_WR_AVLENGTH:
		gds_gst_print_rep_len_value(format,call_nr,size,data);
		break;
	      case GDS_S_DSACONF:
		gds_gst_print_dsa_configuration(format,call_nr,size,data);
		break;
	      case GDS_S_ISSTART_PAR:
		gds_gst_print_keydesc(format,call_nr,size,data);
		break;
	      case GDS_S_APDU_REQUEST:
		gds_gst_print_apdu_request(format,call_nr,size,data);
		break;
	      case GDS_S_APDU_RESULT:
		gds_gst_print_apdu_result(format,call_nr,size,data);
		break;
	      case GDS_S_APDU_ERROR:
		gds_gst_print_apdu_error(size,data);
		break;
	      case GDS_S_SELEVINFO_IN:
	      case GDS_S_SELEVINFO_OUT:
		gds_gst_print_fd_info(format, size,data);
		break;
	      case GDS_S_EXECACT_ENTRY:
		gds_gst_print_stub_stev_info(format, call_nr,size,data);
		break;
	      case GDS_S_EXECACTION_INFO:
		gds_gst_print_stub_func_info(format, call_nr,size,data);
		break;
	      case GDS_S_RTROS_LOG:
		gds_gst_print_filter_info(rtrosfilter, &rtrosrq_fp, &rtrosrs_fp,
				    (call_nr == 1) ? TRUE : FALSE, size, data) ;
		nline = FALSE ;
		break;
	      case GDS_S_CMX_FENTRY:
		gds_gst_print_filter_info(cmxfilter, &cmxrq_fp, &cmxrs_fp,
							        TRUE, 0, NULL) ;
		nline = FALSE ;
		break;
	      case GDS_S_CMX_LOG:
		gds_gst_print_filter_info(cmxfilter, &cmxrq_fp, &cmxrs_fp,
							    FALSE, size, data) ;
		break;
	      case GDS_S_DUA_D_N:
		gds_gst_print_int_and_name(format,call_nr,size,data);
		break;
	      case GDS_S_DUA_SERVICE_CONTROLS:
		gds_gst_print_options_serv_control(*(signed32 *)data);
		break;
	      case GDS_S_DUA_TIME_LIMIT:
		gds_gst_print_time_limit(*(signed16 *)data);
		break;
	      case GDS_S_DUA_SIZE_LIMIT:
		gds_gst_print_size_limit(*(signed16 *)data);
		break;
	      case GDS_S_ASN1_ENCODED_MESSAGE:
		gds_gst_print_asn1_encoded_msg(size,data);
		break;
	      default:
		gds_gst_print_data_element(format,convchar,size,data);
		break;
	    }
	    call_nr++;
	  }
	}
	format = eof;
	*eof   = savchar;
      }
    }
  }
  if (nline)
    printf("\n");
  else
    nline = TRUE ;

}                       /* gds_gst_print_data                         */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_data_element                          */
/*                                                                    */
/* Description  : The function prints one data element as specified   */
/*                by a format string                                  */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_data_element(
    const char *format, /* IN    - format string for printf           */
    char       convchar,/* IN    - conversion character for element   */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_data_element                 */

  char *form_elm;       /* pointer to format element                  */
  long n_char;          /* number of printed characters               */

  switch(convchar)
  {                     /* check conversion character                 */
    case 'b':           /* binary data                                */
      form_elm  = strchr(format,'%');
      *form_elm = '\0';
      printf(format);
      printf("\n");
      gds_gst_print_binary_data(size,data);
      printf("\n");
      *form_elm = '%';
      break;
    case 'c':           /* single characters                          */
    case 'C':
      printf(format,*(char *)data);
      break;
    case 'd':           /* integer                                    */
    case 'i':
    case 'o':
    case 'u':
    case 'x':
    case 'X':
      printf(format,size == sizeof(signed16) ? *(signed16 *)data :
	*(signed32 *)data);
      break;
    case 'f':           /* floating point numbers                     */
    case 'e':
    case 'E':
    case 'g':
    case 'G':
      printf(format,size == sizeof(float) ? *(float *)data :
        *(double *)data);
      break;
    case 's':
    case 'S':
			/* print data casted to string                */
      printf(format,(char *)data);
      break;
    case 'p':           /* pointer data                               */
      printf(format,(void *)data);
      break;
    case 'n':           /* no data conversion                         */
      printf(format,&n_char);
      break;
    case '%':           /* just print format string                   */
      printf(format,(char *)data);
      break;
    default:            /* can never happen                           */
      fprintf(stderr,"illegal conversion character '%c' detected\n",
	      convchar);
      break;
  }

}                       /* gds_gst_print_data_element                 */

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_binary_data                           */
/*                                                                    */
/* Description  : The function prints one element of binary data      */
/*                in hexadecimal dump format                          */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_binary_data(
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_binary_data                  */

  signed32 max_line;    /* maximum number of lines to print           */
  signed32 rest_line;   /* number of bytes in last incomplete line    */

  register signed32 line;           /* current line number            */
  register byte     *b;             /* current byte to print          */
  register signed16 i;  /* current position of byte in line           */

  max_line  = (signed32)size / GST_BYTES_PER_LINE;
  rest_line = (signed32)size % GST_BYTES_PER_LINE;

  for                   /* all lines of data                          */
     (line = 0; line < max_line + (rest_line > 0); line++)
  {                     /* prepare and print the line                 */
			/* print position in data string              */
    printf("%6x  ",line * GST_BYTES_PER_LINE);
    for                 /* all bytes in line                          */
       (i = 0, b = (byte *)data + line * GST_BYTES_PER_LINE;
	i < GST_BYTES_PER_LINE;
	i++, b++)
    {                   /* print the byte hexadecimal                 */
      if                /* byte is valid                              */
	(line < max_line || i < rest_line)
      {                 /* print the byte                             */
	printf("%02x ",(int)*b);
      }
      else
      {                 /* print a dummy byte                         */
	printf("   ");
      }
    }
    printf("    |");
    for                 /* all bytes in line                          */
       (i = 0, b = (byte *)data + line * GST_BYTES_PER_LINE;
	i < GST_BYTES_PER_LINE;
	i++, b++)
    {                   /* check if byte is printable                 */
      if                /* byte is valid and printable                */
	((line < max_line || i < rest_line) &&
	 0x20 <= (signed16)*b && (signed16)*b < 0x7F)
      {                 /* print the character                        */
	printf("%c",(char)*b);
      }
      else
      {                 /* print a dummy character                    */
	printf(" ");
      }
    }
    printf("|\n");
  }

}                       /* gds_gst_print_binary_data                  */


/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_stub_stev_info                        */
/*                                                                    */
/* Description  : The function prints stub state/event table          */
/*                information                                         */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

			/* STUB - state/event table information */
static struct sinfo {
	int	state ;
	char	*stname ;
} stateinfo[] = {
/* 1*/  D25_IRINIT,           "IPC-/ROS-initialization",
/* 2*/  D25_IRIDLE,           "IPC-/ROS-idle",
/* 3*/  D25_IRASSOCIATION,    "IPC-/ROS-association",
/* 4*/  D25_IR2ABORTED,       "ROS-aborted 2",
/* 5*/  D25_IBIND_PENDING,    "IPC-bind pending",
/* 6*/  D25_IINVOKE_PENDING,  "IPC-invoke pending",
/* 7*/  D25_IUNBIND_PENDING,  "IPC-unbind pending",
/* 8*/  D25_RBIND_PENDING,    "ROS-bind pending",
/* 9*/  D25_RINVOKE_PENDING,  "ROS-invoke pending",
/*10*/  D25_R1UNBIND_PENDING, "ROS-unbind pending 1",
/*11*/  D25_R2UNBIND_PENDING, "ROS-unbind pending 2",
/*12*/  D25_R1ABORTED,        "ROS-aborted 1",
/*13*/  D25_ICINVOKE_PENDING, "DUA-cache update pending",
/*14*/  0,	              "???" } ;

static struct einfo {
	int event ;
	char	*evname ;
} eventinfo[] = {
/* 1*/  D25_IBIND_EV,    "IPC-bind",
/* 2*/  D25_IUNBIND_EV,  "IPC-unbind",
/* 3*/  D25_IINVOKE_EV,  "IPC-invoke (END)",
/* 4*/  D25_IMINVOKE_EV, "IPC-invoke (MORE)",
/* 5*/  D25_IRESULT_EV,  "IPC-result (END)",
/* 6*/  D25_IMRESULT_EV, "IPC-result (MORE)",
/* 7*/  D25_IERROR_EV,   "IPC-error",
/* 8*/  D25_IREJECT_EV,  "IPC-reject",
/* 9*/  D25_IRETBUF_EV,  "IPC-return buffer",
/*10*/  D25_IABORT_EV,   "IPC-abort",
/*11*/  D25_RBIND_EV,    "ROS-bind request",
/*12*/  D25_RCFBIND_EV,  "ROS-bind confirmation",
/*13*/  D25_RRJBIND_EV,  "ROS-bind reject",
/*14*/  D25_RUNBIND_EV,  "ROS-unbind request",
/*15*/  D25_RCFUBIND_EV, "ROS-unbind confirmation",
/*16*/  D25_RINVOKE_EV,  "ROS-invoke",
/*17*/  D25_RRESULT_EV,  "ROS-result",
/*18*/  D25_RERROR_EV,   "ROS-error",
/*19*/  D25_RUSRREJ_EV,  "ROS-rejected by user",
/*20*/  D25_RPROVREJ_EV, "ROS-rejected by provider",
/*21*/  D25_RRETBUF_EV,  "ROS-return buffer",
/*22*/  D25_RABORT_EV,   "ROS-abort",
/*23*/  D25_IEXCPT_EV,   "IPC-exception",
/*24*/  D25_REXCPT_EV,   "ROS-exception",
/*25*/  0,	         "???" } ;

static void gds_gst_print_stub_stev_info (char *format, signed32 call_no,
					      unsigned32 size, pointer_t info) {
    signed32		i ;
    struct sinfo	*sinfoptr ;
    struct einfo	*einfoptr ;

    /* remove format element from format string */
    format[strlen (format) - 2] = '\0' ;
    switch (call_no) {
    case 0:
	for (i = 0, sinfoptr = stateinfo;
		  i < sizeof (stateinfo)/sizeof (struct sinfo); i++, sinfoptr++)
	    if (sinfoptr->state == *(signed32 *) info)
		break ;
	if (i == sizeof (stateinfo)/sizeof (struct sinfo))
	    sinfoptr-- ;
	printf ("%s%s (%d)", format, sinfoptr->stname, *(signed32 *) info) ;
	break ;
    case 1:
	for (i = 0, einfoptr = eventinfo;
		  i < sizeof (eventinfo)/sizeof (struct einfo); i++, einfoptr++)
	    if (einfoptr->event == *(signed32 *) info)
		break ;
	if (i == sizeof (eventinfo)/sizeof (struct einfo))
	    einfoptr-- ;
	printf ("%s%s (%d)", format, einfoptr->evname, *(signed32 *) info) ;
	break ;
    case 2:
	if (*(signed32 *) info != D25_UNUSED)
	    printf ("%s%d", format, *(signed32 *) info) ;
	break ;
    case 3:
	if (*(signed32 *) info != D25_UNUSED)
	    printf ("%s%d", format, *(signed32 *) info) ;
	break ;
    default:
	break ;
    }
}

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_stub_func_info                        */
/*                                                                    */
/* Description  : The function prints stub function call information  */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

			/* STUB - function call information */
char *funcinfo[] =
/* 1*/  { "ROS-attach",
/* 2*/    "ROS-detach",
/* 3*/    "ROS-bind request",
/* 4*/    "ROS-bind response",
/* 5*/    "ROS-unbind request",
/* 6*/    "ROS-unbind response",
/* 7*/    "ROS-invoke request",
/* 8*/    "ROS-result request",
/* 9*/    "ROS-return error request",
/*10*/    "ROS-rejected by user",
/*11*/    "ROS-abort request",
/*12*/    "ROS-provide buffer",
/*13*/    "ROS-get transfer syntax",
/*14*/    "IPC-attach server",
/*15*/    "IPC-detach server",
/*16*/    "IPC-attach client",
/*17*/    "IPC-detach client",
/*18*/    "IPC-bind",
/*19*/    "IPC-unbind",
/*20*/    "IPC-invoke",
/*21*/    "IPC-wait for invoke",
/*22*/    "IPC-result",
/*23*/    "IPC-wait for result",
/*24*/    "IPC-return buffer",
/*25*/    "IPC-abort",
/*26*/    "get own PSAP-address",
/*27*/    "get internally used resources",
/*28*/    "release internally used resources",
/*29*/    "release ROS-send/receive buffer",
/*30*/    "check internally used resources",
/*31*/    "Register process at IPC",
/*32*/    "Remove registered process from IPC",
/*33*/    "ASN.1 initialization",
/*34*/    "ASN.1 encoding",
/*35*/    "ASN.1 decoding",
/*36*/	  "save DUA-cache update invoke data",
/*37*/	  "remove DUA-cache update job",
/*38*/	  "save DUA-cache update result data",
/*39*/	  "restore IPC-environment",
/*40*/	  "continue DUA-cache update",
/*41*/	  "???" } ;

static void gds_gst_print_stub_func_info (char *format, signed32 call_no,
					      unsigned32 size, pointer_t info) {
    /* remove format element from format string */
    format[strlen (format) - 2] = '\0' ;
    switch (call_no) {
    case 0:
	if (*(signed32 *) info < 0 ||
			 *(signed32 *) info > sizeof (funcinfo)/sizeof (char *))
	    *(signed32 *) info = (sizeof (funcinfo)/sizeof (char *)) - 1 ;

	printf ("%s%s", format, funcinfo[*(signed32 *) info]) ;
	break ;
    case 1:
    case 2:
	printf ("%s%d", format, *(signed32 *) info) ;
	break ;
    default:
	break ;
    }
}

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_fd_info                               */
/*                                                                    */
/* Description  : The function prints file descriptor information     */
/*                used in a select system call                        */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_fd_info (char *format, unsigned32 size,
							       pointer_t info) {
    signed32 i, fndfd, maxfd = (signed32) size  * 8 ;

    /* remove format element from format string */
    format[strlen (format) - 2] = '\0' ;
    printf ("%s\n", format) ;
    /* print file descriptor information */
    for (i = 0, fndfd = 0; i < maxfd; i++) {
	if (FD_ISSET (i, (fd_set *) info)) {
	    printf ("%5d ", i) ;
	    fndfd++ ;
	}
	if (fndfd && (fndfd%12) == 0)
	    printf ("\n") ;
    }
    if (fndfd) {
	if ((fndfd%12) != 0)
	    printf ("\n") ;
    } else
	printf ("    ---\n") ;
}

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_filter_info                           */
/*                                                                    */
/* Description  : The function creates a filtering process, passes    */
/*                all associated logging records as a request to this */
/*                process and prints out the filtered information     */
/*                received.                                           */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void gds_gst_print_filter_info (char *proginfo[], FILE **filterrq_fp,
	FILE **filterrs_fp, boolean complete, unsigned32 size, pointer_t data) {
    int		filterrs_fds[2], filterrq_fds[2], rsinfo ;

    if (!*filterrq_fp) {
	/* create filtering process */

	/* establish communication pipes */
	if (pipe (filterrq_fds) == -1) {
	    perror ("pipe call failed") ;
	    exit (1) ;
	}
	if (pipe (filterrs_fds) == -1) {
	    perror ("pipe call failed") ;
	    exit (1) ;
    	}

	switch (fork ()) {
	case 0:
	    /* at this point we are in the filtering (child) process */
	    /* close unused pipe fd's */
	    close (filterrq_fds[1]) ;
	    close (filterrs_fds[0]) ;

	    /* redirect stdin and stdout to newly created pipe fd's */
	    close (0) ;
	    dup (filterrq_fds[0]) ;
	    close (filterrq_fds[0]) ;
	    close (1) ;
	    dup (filterrs_fds[1]) ;
	    close (filterrs_fds[1]) ;

	    /* disable I/O-buffering */
	    setbuf (stdin, NULL) ;
	    setbuf (stdout, NULL) ;

	    /* exec filtering process */
	    if (execv (proginfo[0], &proginfo[0]) == - 1) {
		perror ("execv failed") ;
		exit (1) ;
	    }
	    break ;
	case -1:
	    perror ("fork failed") ;
	    exit (1) ;
	    break ;
	default:
	    /* at this point we are still in the parent process */
	    /* close unused pipes */
	    close (filterrq_fds[0]) ;
	    close (filterrs_fds[1]) ;
	    if ((*filterrq_fp = fdopen (filterrq_fds[1], "w")) == (FILE *) NULL) {
		perror ("fopen failed") ;
		exit (1) ;
	    }	
	    if ((*filterrs_fp = fdopen (filterrs_fds[0], "r")) == (FILE *) NULL) {
		perror ("fopen failed") ;
		exit (1) ;
	    }	
	    /* disable I/O-buffering */
	    setbuf (*filterrq_fp, NULL) ;
	    setbuf (*filterrs_fp, NULL) ;
	    break ;
	}
    }

    /* send request to filtering process */
    if (size > 0) {
	if (fwrite (data, 1, size, *filterrq_fp) != size) {
	    perror ("fwrite failed in parent process") ;
	    exit (1) ;
	}
    }
    if (!complete)
	/* request is still incomplete */
	return ;

    /* get result from filtering process */
    while ((rsinfo = fgetc (*filterrs_fp)) != EOF && rsinfo != '\0') {
	fputc (rsinfo, stdout) ;
    }
    
    if (rsinfo == '\0')
	return ;

    if (ferror (*filterrs_fp)) {
	perror ("fgetc failed in parent process") ;
	exit (1) ;
    } else {
	/* EOF detected */
	fprintf (stderr, "EOF detected at result pipe\n") ;
	exit (1) ;
    }
}
