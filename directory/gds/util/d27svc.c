/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27svc.c,v $
 * Revision 1.1.4.2  1996/02/18  18:24:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:55  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:17:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:45  root]
 * 
 * Revision 1.1.2.11  1994/10/05  15:39:25  keutel
 * 	OT 12481: multiple routing
 * 	[1994/10/05  14:37:58  keutel]
 * 
 * Revision 1.1.2.10  1994/09/30  12:08:11  keutel
 * 	OT 12360
 * 	[1994/09/30  10:01:41  keutel]
 * 
 * Revision 1.1.2.9  1994/09/19  08:39:43  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:31:09  marrek]
 * 
 * Revision 1.1.2.8  1994/08/24  12:15:08  keutel
 * 	OT 11802
 * 	[1994/08/23  15:35:12  keutel]
 * 
 * Revision 1.1.2.7  1994/07/13  13:33:55  keutel
 * 	OT 11265: delete first call of dce_svc_routing
 * 	[1994/07/12  22:33:43  keutel]
 * 
 * Revision 1.1.2.6  1994/07/08  18:03:31  keutel
 * 	OT 11156: call new function dce_svc_init
 * 	[1994/07/08  18:03:07  keutel]
 * 
 * Revision 1.1.2.5  1994/06/21  14:48:37  marrek
 * 	June 1994 code submission.
 * 	[1994/06/20  13:04:09  marrek]
 * 
 * Revision 1.1.2.4  1994/06/10  21:18:53  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:22  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/10  16:03:41  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  15:05:26  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  09:21:55  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/04  14:24:18  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:13:29  marrek]
 * 
 * Revision 1.1.2.2  1994/03/23  15:48:21  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:34:11  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:22:03  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:13:06  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:54:33  marrek]
 * 
 * $EndLog$
 */
/**********************************************************************/
/*                                                                    */
/* Component    : GDS                                                 */
/*                                                                    */
/* File name    : d27svc.c                                            */
/*                                                                    */
/* Description  : This file contains serviceability utilities for the */
/*                GDS component.                                      */
/*                                                                    */
/* Date         : 10/04/93                                            */
/*                                                                    */
/* Author       : E. Kraemer, BUBA NM12                               */
/*                                                                    */
/**********************************************************************/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27svc.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:24:00 $";
#endif

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <gds.h>
#include <dce/dcesvcmsg.h>

#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <search.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#include <dce/d27util.h>
#include <d23ipc.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define SVC_SEP_CHAR    ':'
#define D27_ROUTE_PATH_SPEC_CHAR 'P'
#define D27_DBG_ROUTE_SPEC_CHAR  'v'
#define D27_ROUTE_SPEC_CHAR      'w'

#define D27_SVC(args)   gds_svc_handle, file, line, args, GDS_S_GENERAL

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

/***   EXPORT               ****/

			    /* serviceability handle                  */
dce_svc_handle_t        gds_svc_handle;

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

static void d27_002_insert_path(char *route, char *path, char *where);

/**********************************************************************/
/*                                                                    */
/* Function name: d27_001_svc_init                                    */
/*                                                                    */
/* Description  : This initializes the serviceability functions for   */
/*                GDS components.                                     */
/*                                                                    */
/* Return value    :                                                  */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void d27_001_svc_init(
    int argc,           /* IN    - count of arguments                 */
    char **argv,        /* IN    - list of arguments                  */
    error_status_t *st) /* OUT   - error status                       */

{                       /* d27_001_svc_init                           */

  char *log_path;       /* path name of logging directory             */
			/* memory for routing specification string    */
  char *prog_name;      /* pointer to basename of calling program     */
  char where[100 + D27_LFILE_NAME];
  register int i;       /* loop variable                              */
  unsigned32 flags;

/* call dce_svc_init - it will do SVC intialization 
   (esp. rpc_sm_enable_allocate) */
/* flags are not yet used in dce_svc_init - so we set them to 0 */

  flags = 0;
  dce_svc_init(flags,st);
  if(*st != svc_s_ok)
  {
    return;
  }

  /* register the message table */
  dce_msg_define_msg_table(gds_msg_table,
			   sizeof(gds_msg_table)/sizeof(gds_msg_table[0]),
			   st);
  if (*st != msg_s_ok)
  {
    return;
  }

  prog_name = strrchr(argv[0],'/');
  prog_name = prog_name == NULL ? argv[0] : prog_name + 1;
  dce_svc_set_progname(prog_name,st);
  if(*st != svc_s_ok)
  {
    return;
  }

  gds_svc_handle = dce_svc_register(gds_svc_table,(idl_char *)"gds",st);
  if(*st != svc_s_ok)
  {
    return;
  }

  for                   /* all command line parameters                */
     (i = 1, log_path = ""; i < argc && *st == svc_s_ok; i++)
  {
    if                  /* label parameter found                      */
      (argv[i][0] == '-')
    {                   /* check label                                */
      switch((int)argv[i][1])
      {
	case D27_ROUTE_PATH_SPEC_CHAR:
	  log_path = argv[i] + 2;
	  break;
	case D27_DBG_ROUTE_SPEC_CHAR:
	  d27_002_insert_path(argv[i] + 1,log_path,where);
	  dce_svc_debug_routing((unsigned char *)where,st);
	  break;
	case D27_ROUTE_SPEC_CHAR:
	  d27_002_insert_path(argv[i] + 1,log_path,where);
	  dce_svc_routing((unsigned char *)where,st);
	  break;
	default:
	  break;        /* continue loop                              */
      }
    }
  }

}                       /* d27_001_svc_init                           */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_002_insert_path                                 */
/*                                                                    */
/* Description  : This inserts a path name into routing specifications*/
/*                                                                    */
/* Return value    :                                                  */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void d27_002_insert_path(
    char *route,        /* IN    - routing specification string       */
    char *path,         /* IN    - path name to insert                */
    char *where)        /* OUT   - modified routing specification     */

{                       /* d27_002_insert_path                        */

  char *filetype;       /* pointer to filetype segment                */
  char *filename;       /* pointer to filename segment                */

  strcpy(where,route + 1);
  if                    /* path is given                              */
    (strlen(path) > 0)
  {                     /* find beginning of filename segment         */
    if                  /* first separator present                    */
      ((filetype = strchr(where,SVC_SEP_CHAR)) != NULL)
    {                   /* find second separator                      */
      filetype++;
      if                /* second separator present                   */
	((filename = strchr(filetype,SVC_SEP_CHAR)) != NULL)
      {                 /* check routing type                         */
	if              /* debug routing is specified                 */
	  (*route == D27_DBG_ROUTE_SPEC_CHAR)
	{               /* find third separator                       */
	  filetype = filename + 1;
	  filename = strchr(filetype,SVC_SEP_CHAR);
	}
      }
      if                /* filename found                             */
	(filename != NULL)
      {                 /* check filetype                             */
	if              /* filetype is TEXTFILE or BINFILE            */
	  (strstr(filetype,"TEXTFILE") == filetype ||
	   strstr(filetype,"BINFILE") == filetype ||
	   strstr(filetype,"FILE") == filetype)
	{               /* insert pathname                            */
	  filename++;
	  strcpy(filename,path);
	  strcat(where,route + 1 + (filename - where));
	}
      }
    }
  }

  return;

}                       /* d27_002_insert_path                        */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_003_reset_routing                               */
/*                                                                    */
/* Description  : This resets the routing after a routing             */
/*                specification string is received from a distributed */
/*                command.                                            */
/*                                                                    */
/* Return value    :                                                  */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void d27_003_reset_routing(
			/* IN    - routing specification string       */
    char           *all_routes,
    error_status_t *st) /* OUT   - error status                       */

{                       /* d27_003_reset_routing                      */

			/* routing specification                      */
  char route[D23_DPARSIZE];
  int  spec_len;        /* length of one specification string         */
  int  scan_len;        /* length of scanned specification strings    */
  char *spec_char;      /* points to routing specification character  */
			/* path name of logging directory             */
  char log_path[D27_LFILE_NAME];
			/* memory for routing specification string    */
  char where[100 + D27_LFILE_NAME];

  strcpy(log_path,"");
  scan_len = 0;
  while                 /* routing specification is following         */
       (sscanf(all_routes + scan_len,"%s%n",route,&spec_len) > 0)
  {                     /* reset routing specification string         */
    scan_len += spec_len;
    spec_char = *route == '-' ? route + 1 : route;
    switch(*spec_char)
    {
      case D27_ROUTE_PATH_SPEC_CHAR:
	strcpy(log_path,spec_char + 1);
	break;
      case D27_DBG_ROUTE_SPEC_CHAR:
	d27_002_insert_path(spec_char,log_path,where);
	dce_svc_debug_routing((unsigned char *)where,st);
	if              /* debug routing failed                       */
	  (*st != svc_s_ok)
	{               /* issue error message                        */
	  dce_svc_printf(GDS_S_DBG_ROUTING_ERR_MSG,*st,spec_char + 1);
	}
	break;
      case D27_ROUTE_SPEC_CHAR:
	d27_002_insert_path(spec_char,log_path,where);
	dce_svc_routing((unsigned char *)where,st);
	if              /* routing failed                             */
	  (*st != svc_s_ok)
	{               /* issue error message                        */
	  dce_svc_printf(GDS_S_ROUTING_ERR_MSG,*st,spec_char + 1);
	}
	break;
      default:
	*st = svc_s_bad_routespec;
	dce_svc_printf(GDS_S_BAD_ROUTE_SPEC_MSG,*spec_char);
	break;
    }
  }

}                       /* d27_003_reset_routing                      */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_malloc                                          */
/*                                                                    */
/* Description  : This function calls malloc. In case of failure it   */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      != NULL         void *      pointer to allocated memory       */
/*      NULL                        malloc failed                     */
/*                                                                    */
/**********************************************************************/

void *d27_malloc(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    size_t len)         /* IN    -  size of memory to be allocated    */

{                       /* d27_malloc                                 */

  void *mem;            /* allocated memory                           */

  if                    /* malloc fails                               */
    ((mem = malloc(len)) == NULL)
  {                     /* issue log messages                         */
    dce_svc_printf(D27_SVC("%d"),attributes,GDS_S_NOMEMORY,len);
  }

  return(mem);

}                       /* d27_malloc                                 */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_calloc                                          */
/*                                                                    */
/* Description  : This function calls calloc. In case of failure it   */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      != NULL         void *      pointer to allocated memory       */
/*      NULL                        calloc failed                     */
/*                                                                    */
/**********************************************************************/

void *d27_calloc(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    size_t count,       /* IN    -  number of elements to be allocated*/
    size_t size)        /* IN    -  size of elements to be allocated  */

{                       /* d27_calloc                                 */

  void *mem;            /* allocated memory                           */

  if                    /* calloc fails                               */
    ((mem = calloc(count,size)) == NULL)
  {                     /* issue log messages                         */
    dce_svc_printf(D27_SVC("%d"),attributes,GDS_S_NOMEMORY,count * size);
  }

  return(mem);

}                       /* d27_calloc                                 */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_realloc                                         */
/*                                                                    */
/* Description  : This function calls realloc. In case of failure it  */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value :                                                     */
/*      != NULL         void *      pointer to allocated memory       */
/*      NULL                        realloc failed                    */
/*                                                                    */
/**********************************************************************/

void *d27_realloc(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    void *mem,          /* IN    -  memory to be reallocated          */
    size_t size)        /* IN    -  required length of memory         */

{                       /* d27_realloc                                */

  void *new_mem;        /* reallocated memory                         */

  if                    /* realloc fails                              */
    ((new_mem = realloc(mem,size)) == NULL)
  {                     /* issue log messages                         */
    dce_svc_printf(D27_SVC("%d"),attributes,GDS_S_NOMEMORY,size);
  }

  return(new_mem);

}                       /* d27_realloc                                */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_tsearch                                         */
/*                                                                    */
/* Description  : This function calls tsearch. In case of failure it  */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      != NULL         void *      pointer to found or inserted key  */
/*      NULL                        malloc failed                     */
/*                                                                    */
/**********************************************************************/

void *d27_tsearch(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    const void *key,    /* IN    -  key to search or insert in tree   */
    void **rootp,       /* INOUT -  root of binary tree               */
			/* IN    -  comparison function               */
    int (*compar)(const void *, const void *))

{                       /* d27_tsearch                                */

  void *tsearch_result; /* allocated memory                           */

  if                    /* tsearch fails                              */
    ((tsearch_result = tsearch(key,rootp,compar)) == NULL)
  {                     /* issue log messages                         */
    dce_svc_printf(D27_SVC("%d"),attributes,GDS_S_NOMEMORY,sizeof(void *));
  }

  return(tsearch_result);

}                       /* d27_tsearch                                */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_fopen                                           */
/*                                                                    */
/* Description  : This function calls fopen. In case of failure it    */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      != NULL         FILE *      file pointer to opened file       */
/*      NULL                        fopen failed                      */
/*                                                                    */
/**********************************************************************/

FILE *d27_fopen(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    const char *name,   /* IN    -  name of file to be opened         */
    const char *mode)   /* IN    -  open mode for file                */

{                       /* d27_fopen                                  */

  FILE *fp;             /* file pointer of file to be opened          */
  int  l_errno;         /* local errno                                */

  if                    /* fopen fails                                */
    ((fp = fopen(name,mode)) == NULL)
  {                     /* issue log messages                         */
    l_errno = errno;
    dce_svc_printf(D27_SVC("%s%s%d"),attributes,GDS_S_FOPEN_ERR,name,mode,
      l_errno);
  }

  return(fp);

}                       /* d27_fopen                                  */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_fclose                                          */
/*                                                                    */
/* Description  : This function calls fclose. In case of failure it   */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      != 0            int         function failed                   */
/*      0                           function terminates successfully  */
/*                                                                    */
/**********************************************************************/

int d27_fclose(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    FILE *fp)           /* IN    _  file pointer of file to be closed */

{                       /* d27_fclose                                 */

  int ret_value;        /* return value                               */
  int  l_errno;         /* local errno                                */

  if                    /* fclose fails                               */
    ((ret_value = fclose(fp)) != 0)
  {                     /* issue log messages                         */
    l_errno = errno;
    dce_svc_printf(D27_SVC("%d"),attributes,GDS_S_FCLOSE_ERR,l_errno);
  }

  return(ret_value);

}                       /* d27_fclose                                 */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_fflush                                          */
/*                                                                    */
/* Description  : This function calls fflush. In case of failure it   */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      EOF             int         function failed                   */
/*      0                           function terminates successfully  */
/*                                                                    */
/**********************************************************************/

int d27_fflush(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    FILE *fp)           /* IN    _  file pointer of file to be flushed*/

{                       /* d27_fflush                                 */

  int ret_value;        /* return value                               */
  int  l_errno;         /* local errno                                */

  if                    /* fflush fails                               */
    ((ret_value = fflush(fp)) != 0)
  {                     /* issue log messages                         */
    l_errno = errno;
    dce_svc_printf(D27_SVC("%d"),attributes,GDS_S_FFLUSH_ERR,l_errno);
  }

  return(ret_value);

}                       /* d27_fflush                                 */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_fpcheck                                         */
/*                                                                    */
/* Description  : This function calls ferror and feof. In case a file */
/*                error or end of file is detected it issues a log    */
/*                message and returns                                 */
/*                                                                    */
/* Return value    :                                                  */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void d27_fpcheck(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    FILE *fp)           /* IN    -  file pointer to be checked        */

{                       /* d27_fpcheck                                */

  int  l_errno;         /* local errno                                */

  if                    /* error has occurred                         */
    (ferror(fp) != 0)
  {                     /* issue log messages                         */
    l_errno = errno;
    dce_svc_printf(D27_SVC("%d"),attributes,GDS_S_FREAD_ERR,l_errno);
  }
  else if               /* end of file is reached                     */
	 (feof(fp) != 0)
  {                     /* issue log message                          */
    dce_svc_printf(D27_SVC(""),attributes,GDS_S_FEOF);
  }

}                       /* d27_fpcheck                                */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_fgets                                           */
/*                                                                    */
/* Description  : This function calls fgets. In case of failure it    */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      ptr             char *      string could be read in           */
/*      NULL                        error occurred                    */
/*                                                                    */
/**********************************************************************/

char *d27_fgets(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    char *ptr,          /* OUT   -  buffer for data to be read        */
    int n_bytes,        /* IN    -  number of bytes to read           */
    FILE *fp)           /* INOUT -  file pointer to read from         */

{                       /* d27_fgets                                  */

  char *buff;           /* buffer to return                           */

  buff = fgets(ptr,n_bytes,fp);

  d27_fpcheck(file,line,attributes,fp);

  return(buff);

}                       /* d27_fgets                                  */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_fread                                           */
/*                                                                    */
/* Description  : This function calls fread. In case of failure it    */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      >= 0            size_t      count of elements read            */
/*                                                                    */
/**********************************************************************/

size_t d27_fread(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    void *ptr,          /* IN    -  buffer for data to be read        */
    size_t size,        /* OUT   -  size of data element              */
    size_t nitems,      /* IN    -  number of data elements           */
    FILE *fp)           /* INOUT -  file pointer to read from         */

{                       /* d27_fread                                  */

  size_t ret_value;     /* return value                               */

  ret_value = fread(ptr,size,nitems,fp);

  d27_fpcheck(file,line,attributes,fp);

  return(ret_value);

}                       /* d27_fread                                  */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_fwrite                                          */
/*                                                                    */
/* Description  : This function calls fwrite. In case of failure it   */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      >= 0            size_t      count of elements written         */
/*                                                                    */
/**********************************************************************/

size_t d27_fwrite(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    const void *ptr,    /* IN    -  buffer for data to be written     */
    size_t size,        /* IN    -  size of data element              */
    size_t nitems,      /* IN    -  number of data elements           */
    FILE *fp)           /* INOUT -  file pointer to write to          */

{                       /* d27_fwrite                                 */

  size_t ret_value;     /* return value                               */
  int  l_errno;         /* local errno                                */

  ret_value = fwrite(ptr,size,nitems,fp);

  if                    /* error has occurred                         */
    (ferror(fp) != 0)
  {                     /* issue log messages                         */
    l_errno = errno;
    dce_svc_printf(D27_SVC("%d"),attributes,GDS_S_FWRITE_ERR,l_errno);
  }

  return(ret_value);

}                       /* d27_fwrite                                 */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_fprintf                                         */
/*                                                                    */
/* Description  : This function calls fprintf. In case of failure it  */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      >= 0            size_t      count of elements written         */
/*                                                                    */
/**********************************************************************/

int d27_fprintf(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    FILE *fp,           /* INOUT -  file pointer to write to          */
    const char * format,/* IN    -  format string                     */
    ... )               /* IN    -  variable parameters               */

{                       /* d27_fprintf                                */

  size_t ret_value;     /* return value                               */
  va_list args;         /* pointer to variable arguments              */
  int  l_errno;         /* local errno                                */

  va_start(args,format);
  ret_value = vfprintf(fp,format,args);
  va_end(args);

  if                    /* error has occurred                         */
    (ferror(fp) != 0)
  {                     /* issue log messages                         */
    l_errno = errno;
    dce_svc_printf(D27_SVC("%d"),attributes,GDS_S_FWRITE_ERR,l_errno);
  }

  return(ret_value);

}                       /* d27_fprintf                                */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_fork                                            */
/*                                                                    */
/* Description  : This function calls fork. In case of failure it     */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      >= 0            pid_t       function call successful          */
/*      -1                          fork failed                       */
/*                                                                    */
/**********************************************************************/

pid_t d27_fork(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes)

{                       /* d27_fork                                   */

  pid_t ret_value;      /* return value                               */
  int  l_errno;         /* local errno                                */

  ret_value = fork();

  if                    /* error has occurred                         */
    (ret_value == (pid_t)(-1))
  {                     /* issue log message                          */
    l_errno = errno;
    dce_svc_printf(D27_SVC("%d"),attributes,GDS_S_FORK_ERR,l_errno);
  }

  return(ret_value);

}                       /* d27_fork                                   */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_svcsigaction                                    */
/*                                                                    */
/* Description  : This function calls SIGACTION. In case of failure it*/
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*       0              int         function call successful          */
/*      -1                          sigaction failed                  */
/*                                                                    */
/**********************************************************************/

int d27_svcsigaction(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    int sig,            /* IN    -  signal to be handled              */
			/* IN    -  new setting of signal parameters  */
    const struct sigaction *act,
			/* OUT   -  old setting of signal parameters  */
    struct sigaction *oact)

{                       /* d27_svcsigaction                           */

  int ret_value;        /* return value                               */
  int  l_errno;         /* local errno                                */

  ret_value = SIGACTION(sig,act,oact);

  if                    /* error has occurred                         */
    (ret_value < 0)
  {                     /* issue log message                          */
    l_errno = errno;
    dce_svc_printf(D27_SVC("%d%d"),attributes,GDS_S_SIGHDL_ERR,sig,l_errno);
  }

  return(ret_value);

}                       /* d27_svcsigaction                           */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_wait                                            */
/*                                                                    */
/* Description  : This function calls wait. In case of failure it     */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      >= 0            pid_t       function call successful          */
/*      -1                          wait failed                       */
/*                                                                    */
/**********************************************************************/

pid_t d27_wait(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    int *stat_loc)      /* OUT   -  status of child process           */

{                       /* d27_wait                                   */

  pid_t ret_value;      /* return value                               */
  int  l_errno;         /* local errno                                */

  ret_value = wait(stat_loc);

  if                    /* error has occurred                         */
    (ret_value == (pid_t)(-1))
  {                     /* issue log message                          */
    l_errno = errno;
    dce_svc_printf(D27_SVC("%d"),attributes,GDS_S_WAIT_ERR,l_errno);
  }
  else if               /* child process exited normally              */
	 (WIFEXITED(*stat_loc))
  {                     /* issue log message                          */
    DCE_SVC_LOG((D27_SVC("%d"),svc_c_debug1,GDS_S_CHILD_EXIT,
      WEXITSTATUS(*stat_loc)));
  }
  else if               /* child process killed                       */
	 (WIFSIGNALED(*stat_loc))
  {                     /* issue log message                          */
    dce_svc_printf(D27_SVC("%d"),attributes,GDS_S_CHILD_SIGNAL,
      WTERMSIG(*stat_loc));
  }

  return(ret_value);

}                       /* d27_wait                                   */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_system                                          */
/*                                                                    */
/* Description  : This function calls system. In case of failure it   */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      != -1           int         function call successful          */
/*      -1                          fork or exec failed               */
/*                                                                    */
/**********************************************************************/

int d27_system(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    const char *cmd)    /* IN    -  name of the command to be executed*/

{                       /* d27_system                                 */

  int ret_value;        /* return value                               */
  int  l_errno;         /* local errno                                */

  ret_value = system(cmd);

  if                    /* error has occurred                         */
    (ret_value == -1)
  {                     /* issue log message                          */
    l_errno = errno;
    dce_svc_printf(D27_SVC("%s%d"),attributes,GDS_S_SYSTEM_ERR,cmd,l_errno);
  }
  else if               /* child process exited normally              */
	 (WIFEXITED(ret_value))
  {                     /* check exit status                          */
    if                  /* child exited with error                    */
      (WEXITSTATUS(ret_value) != 0)
    {                   /* issue log message                          */
      dce_svc_printf(D27_SVC("%d"),attributes,GDS_S_CHILD_EXIT,
	WEXITSTATUS(ret_value));
    }
  }
  else if               /* child process killed                       */
	 (WIFSIGNALED(ret_value))
  {                     /* issue log message                          */
    dce_svc_printf(D27_SVC("%d"),attributes,GDS_S_CHILD_SIGNAL,
      WTERMSIG(ret_value));
  }

  return(ret_value);

}                       /* d27_system                                 */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_execv                                           */
/*                                                                    */
/* Description  : This function calls execv. In case of failure it    */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*                      int         function failed                   */
/*                                                                    */
/**********************************************************************/

int d27_execv(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    const char *path,   /* IN    -  name of the command to be executed*/
    char *const *argv)  /* IN    -  pointer to command line arguments */

{                       /* d27_execv                                  */

  int ret_value;        /* return value                               */
  int  l_errno;         /* local errno                                */

  ret_value = execv(path,argv);

  l_errno = errno;
  dce_svc_printf(D27_SVC("%d"),attributes,GDS_S_EXECV_ERR,l_errno);

  return(ret_value);

}                       /* d27_execv                                  */
