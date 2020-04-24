/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27pfm.c,v $
 * Revision 1.1.6.2  1996/02/18  18:23:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:48  marty]
 *
 * Revision 1.1.6.1  1995/12/08  16:16:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:29  root]
 * 
 * Revision 1.1.4.3  1994/05/10  16:03:37  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  07:00:12  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/04  14:24:09  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:13:14  marrek]
 * 
 * Revision 1.1.4.2  1994/03/23  15:48:19  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:34:04  keutel]
 * 
 * Revision 1.1.4.1  1994/02/22  17:55:51  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:54:33  marrek]
 * 
 * Revision 1.1.2.2  1993/08/11  14:33:50  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  13:13:25  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27pfm.c,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 18:23:49 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : d27pfm.c <Utility-for performance measurements>     */
/*                                                                    */
/* AUTHOR       : E. Kraemer, AP 11                                   */
/* DATE         : 04.05.93                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    |04.05.93 |  Birth                         | ek |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <gds.h>

/*****  Operating System - Includes  *****/

#include <sys/times.h>

/*****  external Includes  *****/

#include <dce/d27util.h>
#include <d27svc.h>

/*****  internal Includes  *****/

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define D27_TTR_BLOCK   64      /* count of timetrace entries / block */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

typedef struct
	{
	  char   *file;                 /* source file name           */
	  int    line;                  /* line number of the source  */
	  unsigned32 message_index;     /* index of message to log    */
	  char       *d27_text;         /* text to be logged          */
	  signed32   d27_nr_access1;    /* 1. number of accesses to   */
					/* operating system           */
	  signed32   d27_nr_access2;    /* 2. number of accesses to   */
					/* operating system           */
	  clock_t d27_real_time;        /* real time                  */
	  struct tms d27_times;         /* user- and system time      */
	} D27_ttr_entry;

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/****** LOCAL ******/

static struct
	{
	  signed16 d27_no_ttr_alloc;/* number of allocated entries    */
				    /* number of used entries         */
	  signed16 d27_no_ttr_entries;
				    /* timetrace entries              */
	  D27_ttr_entry *d27_ttr_entry;
	} d27_time_trace;

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

static void d27_024_log_ttr_entry(D27_ttr_entry *ttr_entry_0,
    D27_ttr_entry *ttr_entry, int sub_comp);

/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d27_021_reset_time_trace   [reset_time_trace]       */
/*                                                                    */
/* AUTHOR       : Ernst Kraemer, DAP 11                               */
/* DATE         : 03.05.93                                            */
/*                                                                    */
/* DESCRIPTION  : The function frees the array of time traces         */
/*                and initializes the structure                       */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d27_021_reset_time_trace(void)

{                       /*  d27_021_reset_time_trace                  */

  if                    /* performance logging is enabled             */
    (DCE_SVC_DEBUG_ATLEAST(gds_svc_handle,GDS_S_PFM,svc_c_debug1))
  {                     /* check time trace entries                   */
    if                  /* time traces allocated                      */
      (d27_time_trace.d27_ttr_entry != NULL)
    {                   /* free time traces                           */
      free(d27_time_trace.d27_ttr_entry);
    }
    d27_time_trace.d27_ttr_entry      = NULL;
    d27_time_trace.d27_no_ttr_alloc   = 0;
    d27_time_trace.d27_no_ttr_entries = 0;
  }

}                       /*  d27_021_reset_time_trace                  */

/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d27_022_enter_time_trace   [enter_time_trace]       */
/*                                                                    */
/* AUTHOR       : Ernst Kraemer, DAP 11                               */
/* DATE         : 04.05.93                                            */
/*                                                                    */
/* DESCRIPTION  : The function fills the next timetrace structure.    */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d27_022_enter_time_trace(
    char       *file,   /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int        line,    /* IN    -  line number of source file        */
			/* IN    -  index of message to be logged     */
    unsigned32 message_index,
    char       *text,   /* IN    -  text data to be logged            */
			/* IN    -  first number of system accesses   */
    signed32   nr_access1,
			/* IN    -  second number of system accesses  */
    signed32   nr_access2)

{                       /*  d27_022_enter_time_trace                  */

  D27_ttr_entry *ttr_entry;         /* time trace pointer             */
  D27_ttr_entry ttrace;             /* time trace structure           */

  signed16 idx;         /* index of time trace entry                  */

  if                    /* performance logging is enabled             */
    (DCE_SVC_DEBUG_ATLEAST(gds_svc_handle,GDS_S_PFM,svc_c_debug1))
  {                     /* check whether enough timetrace structures  */
			/* are allocated                              */
    idx = d27_time_trace.d27_no_ttr_entries;
    if                  /* not enough timetrace structures allocated  */
      (idx == d27_time_trace.d27_no_ttr_alloc)
    {                   /* reallocate time trace entries              */
      if                /* no more memory available                   */
	((d27_time_trace.d27_ttr_entry = (D27_ttr_entry *)D27_REALLOC(
	  svc_c_sev_warning,d27_time_trace.d27_ttr_entry,
	  sizeof(D27_ttr_entry) *
	  (d27_time_trace.d27_no_ttr_alloc + D27_TTR_BLOCK))) != NULL)
      {                 /* reset number of allocated entries          */
	d27_time_trace.d27_no_ttr_alloc += D27_TTR_BLOCK;
      }
      else
      {                 /* return                                     */
	return;
      }
    }
    ttr_entry = d27_time_trace.d27_ttr_entry + idx;
    ttr_entry->d27_real_time  = times(&ttr_entry->d27_times);

    d27_time_trace.d27_no_ttr_entries++;
  }
  else
  {                     /* assign timetrace structure for general     */
			/* logging only                               */
    ttr_entry              = &ttrace;
  }

  ttr_entry->file           = file;
  ttr_entry->line           = line;
  ttr_entry->message_index  = message_index;
  ttr_entry->d27_text       = text;
  ttr_entry->d27_nr_access1 = nr_access1;
  ttr_entry->d27_nr_access2 = nr_access2;

  d27_024_log_ttr_entry((D27_ttr_entry *)NULL,ttr_entry,GDS_S_GENERAL);

}                       /*  d27_022_enter_time_trace                  */

/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d27_023_dump_time_trace   [dump_time_trace]         */
/*                                                                    */
/* AUTHOR       : Ernst Kraemer, DAP 11                               */
/* DATE         : 04.05.93                                            */
/*                                                                    */
/* DESCRIPTION  : The function dumps the whole timetrace structure    */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d27_023_dump_time_trace(void)

{                       /*  d27_023_dump_time_trace                   */

/* table header for time traces
static char *ttr_header = "comment:                       | real | user | sys  || dreal| duser| dsys |";
static char *underline  = "---------------------------------------------------------------------------";
*/

  D27_ttr_entry *ttr_entry_0;       /* time trace pointer of first    */
				    /* entry                          */
  D27_ttr_entry *ttr_entry;         /* time trace pointer             */

  signed16 i;                       /* loop variable                  */

  if                    /* performance logging is enabled             */
    (DCE_SVC_DEBUG_ATLEAST(gds_svc_handle,GDS_S_PFM,svc_c_debug1))
  {                     /* log timetrace structures                   */
    ttr_entry_0 = d27_time_trace.d27_ttr_entry;
    for                 /* all timetrace structures                   */
       (i = 0, ttr_entry = d27_time_trace.d27_ttr_entry + i;
	i < d27_time_trace.d27_no_ttr_entries; i++, ttr_entry++)
    {                   /* assign parameters for log                  */
      d27_024_log_ttr_entry(ttr_entry_0,ttr_entry,GDS_S_PFM);
    }
  }

}                       /*  d27_023_dump_time_trace                   */

/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d27_024_log_ttr_entry     [log time trace entry]    */
/*                                                                    */
/* AUTHOR       : Ernst Kraemer, DAP 11                               */
/* DATE         : 22.04.94                                            */
/*                                                                    */
/* DESCRIPTION  : The function logs one particular time trace entry   */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

static void d27_024_log_ttr_entry(
			/* IN    -  initial time trace entry          */
    D27_ttr_entry *ttr_entry_0,
			/* IN    -  time trace entry to log           */
    D27_ttr_entry *ttr_entry,
			/* IN    -  subcomponent to log               */
    int           sub_comp)

{                       /*  d27_024_log_ttr_entry                     */

  struct tms *times_0;              /* initial user and system times  */
  struct tms *times;                /* actual user and system times   */
  unsigned32 message_index;         /* message index to log           */
  char       *text;                 /* text data to be logged         */
  signed32   nr_access1;            /* first number of system accesses*/
  signed32   nr_access2;            /* second number of system acc.   */
  long       real_time = 0L;        /* real time to be logged         */
  long       user_time = 0L;        /* user time to be logged         */
  long       sys_time  = 0L;        /* system time to be logged       */

  if                    /* first time trace entry present             */
    (ttr_entry_0 != (D27_ttr_entry *)NULL)
  {                     /* assign times                               */
    times_0     = &ttr_entry_0->d27_times;
    times       = &ttr_entry->d27_times;
    real_time   = (long)(ttr_entry->d27_real_time -
      ttr_entry_0->d27_real_time);
    user_time   = (long)(times->tms_utime - times_0->tms_utime);
    sys_time    = (long)(times->tms_stime - times_0->tms_stime);
  }
  message_index = ttr_entry->message_index;
  text          = ttr_entry->d27_text;
  nr_access1    = ttr_entry->d27_nr_access1;
  nr_access2    = ttr_entry->d27_nr_access2;
  switch(message_index)
  {
    case GDS_S_FCT_IS_START_READ:
      DCE_SVC_LOG((gds_svc_handle,ttr_entry->file,ttr_entry->line,
	"%s%d%d%ld%ld%ld",sub_comp,svc_c_debug1,message_index,
	text,nr_access1,nr_access2,real_time,user_time,sys_time));
      break;
    case GDS_S_FCT_IS_WRITE:
    case GDS_S_FCT_IS_DELETE:
    case GDS_S_FCT_SENT_BLOCKS:
    case GDS_S_FCT_RECEIVED_BLOCKS:
      DCE_SVC_LOG((gds_svc_handle,ttr_entry->file,ttr_entry->line,
	"%s%d%ld%ld%ld",sub_comp,svc_c_debug1,message_index,
	text,nr_access1,real_time,user_time,sys_time));
      break;
    case GDS_S_IS_OPENED:
      DCE_SVC_LOG((gds_svc_handle,ttr_entry->file,ttr_entry->line,
	"%s%ld%ld%ld",sub_comp,svc_c_debug1,message_index,
	text,real_time,user_time,sys_time));
      break;
    default:
      DCE_SVC_LOG((gds_svc_handle,ttr_entry->file,ttr_entry->line,
	"%ld%ld%ld",sub_comp,svc_c_debug1,message_index,
	real_time,user_time,sys_time));
      break;
  }

}                       /*  d27_024_log_ttr_entry                     */
