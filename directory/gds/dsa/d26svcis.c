/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26svcis.c,v $
 * Revision 1.1.4.2  1996/02/18  19:46:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:48:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:02  root]
 * 
 * Revision 1.1.2.6  1994/10/05  15:39:15  keutel
 * 	OT 12481: multiple routing
 * 	[1994/10/05  14:36:58  keutel]
 * 
 * Revision 1.1.2.5  1994/07/06  15:07:05  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:13:52  marrek]
 * 
 * Revision 1.1.2.4  1994/06/21  14:45:56  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:34:25  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:01  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:48  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  15:13:32  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:27:20  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:20:54  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:10:55  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:52:11  marrek]
 * 
 * $EndLog$
 */
/**********************************************************************/
/*                                                                    */
/* Component    : GDS                                                 */
/*                                                                    */
/* File name    : d26svcis.c                                          */
/*                                                                    */
/* Description  : This file contains serviceability utilities for the */
/*                DSA in the GDS component.                           */
/*                                                                    */
/* Date         : 10/04/93                                            */
/*                                                                    */
/* Author       : E. Kraemer, BUBA NM12                               */
/*                                                                    */
/**********************************************************************/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26svcis.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 19:46:12 $";
#endif

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <gds.h>

#include <d26svcis.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define D26_SVC(args)   gds_svc_handle, file, line, args, GDS_S_GENERAL

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

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        FUNCTIONS           */
/*                                                                    */
/**********************************************************************/
extern int islock (int isfd);
extern int isunlock (int isfd);
extern int isrelease (int isfd);
extern int isuniqueid (int isfd, long *uniqid);
extern int isopen (char *filename, register int mode);
extern int isclose (int isfd);
extern int isread (int isfd, char *record, register int mode);
extern int isstart (int isfd, struct keydesc *keydesc, int keylen, char *record, register int mode);
extern int iswrite (int isfd, char *record);
extern int isrewrite (int isfd, char *record);
extern int isrewcurr (int isfd, char *record);
extern int isrewrec (int isfd, long recnum, char *record);
extern int isdelcurr (int isfd);
extern int isaddindex (int isfd, register struct keydesc *keydesc);
extern int isbuild (char *filename, int reclength, register struct keydesc *keydesc, register int mode);

/**********************************************************************/
/*                                                                    */
/* Function name: d26_isbuild                                         */
/*                                                                    */
/* Description  : This function calls isbuild. In case of failure it  */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      >= 0            int         C-ISAM file descriptor            */
/*      < 0                         isbuild failed                    */
/*                                                                    */
/**********************************************************************/

int d26_isbuild(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
			/* IN    -  name of C-ISAM file to be built   */
    const char *filename,
    int reclen,         /* IN    -  record length of C-ISAM file      */
			/* IN    -  description of primary key        */
    struct keydesc *keydesc,
    int mode)           /* IN    -  open mode for C-ISAM file         */

{                       /* d26_isbuild                                */

  int isfd;             /* C-ISAM file descriptor of file to be built */

  if                    /* isbuild fails                              */
    ((isfd = isbuild((char *)filename,reclen,keydesc,mode)) < 0)
  {                     /* issue log messages                         */
    dce_svc_printf(D26_SVC("%s%d%d%d"),attributes,GDS_S_ISBUILD_ERR,filename,
      reclen,mode,iserrno);
  }

  return(isfd);

}                       /* d26_isbuild                                */

/**********************************************************************/
/*                                                                    */
/* Function name: d26_isaddindex                                      */
/*                                                                    */
/* Description  : This function calls isaddindex. In case of failure  */
/*                it issues a log message and returns.                */
/*                                                                    */
/* Return value    :                                                  */
/*      >= 0            int         C-ISAM file descriptor            */
/*      < 0                         isaddindex failed                 */
/*                                                                    */
/**********************************************************************/

int d26_isaddindex(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    int isfd,           /* IN    -  C-ISAM file descriptor to add ind.*/
			/* IN    -  description of index to be added  */
    struct keydesc *keydesc)

{                       /* d26_isaddindex                             */

  int ret_value;        /* return value                               */

  if                    /* isbuild fails                              */
    ((ret_value = isaddindex(isfd,keydesc)) < 0)
  {                     /* issue log messages                         */
    dce_svc_printf(D26_SVC("%d"),attributes,GDS_S_ISADDINDEX_ERR,iserrno);
  }

  return(ret_value);

}                       /* d26_isaddindex                             */

/**********************************************************************/
/*                                                                    */
/* Function name: d26_isopen                                          */
/*                                                                    */
/* Description  : This function calls isopen. In case of failure it   */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      >= 0            int         C-ISAM file descriptor            */
/*      < 0                         isopen failed                     */
/*                                                                    */
/**********************************************************************/

int d26_isopen(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
			/* IN    -  name of C-ISAM file to be opened  */
    const char *filename,
    int mode)           /* IN    -  open mode for C-ISAM file         */

{                       /* d26_isopen                                 */

  int isfd;             /* C-ISAM file descriptor of file to be opened*/

  if                    /* isopen fails                               */
    ((isfd = isopen((char *)filename,mode)) < 0)
  {                     /* issue log messages                         */
    dce_svc_printf(D26_SVC("%s%d%d"),attributes,GDS_S_ISOPEN_ERR,filename,
      mode,iserrno);
  }

  return(isfd);

}                       /* d26_isopen                                 */

/**********************************************************************/
/*                                                                    */
/* Function name: d26_isclose                                         */
/*                                                                    */
/* Description  : This function calls isclose. In case of failure it  */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      0               int         function performs successfully    */
/*      < 0                         isclose failed                    */
/*                                                                    */
/**********************************************************************/

int d26_isclose(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    int isfd)           /* IN    -  C-ISAM file descriptor to close   */

{                       /* d26_isclose                                */

  int ret_value;        /* return value                               */

  if                    /* isclose fails                              */
    ((ret_value = isclose(isfd)) < 0)
  {                     /* issue log messages                         */
    dce_svc_printf(D26_SVC("%d"),attributes,GDS_S_ISCLOSE_ERR,iserrno);
  }

  return(ret_value);

}                       /* d26_isclose                                */

/**********************************************************************/
/*                                                                    */
/* Function name: d26_isuniqueid                                      */
/*                                                                    */
/* Description  : This function calls isuniqueid. In case of failure  */
/*                it issues a log message and returns.                */
/*                                                                    */
/* Return value    :                                                  */
/*      0               int         function performs successfully    */
/*      < 0                         isuniqueid has failed             */
/*                                                                    */
/**********************************************************************/

int d26_isuniqueid(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    int isfd,           /* IN    -  C-ISAM file descriptor            */
    long *uniqueid)     /* OUT   -  unique ID to be generated         */

{                       /* d26_isuniqueid                             */

  int ret_value;        /* return value                               */

  if                    /* isuniqueid returns error                   */
    ((ret_value = isuniqueid(isfd,uniqueid)) < 0)
  {                     /* issue log messages                         */
    dce_svc_printf(D26_SVC("%d"),attributes,GDS_S_ISUNIQUEID_ERR,iserrno);
  }

  return(ret_value);

}                       /* d26_isuniqueid                             */

/**********************************************************************/
/*                                                                    */
/* Function name: d26_isstart                                         */
/*                                                                    */
/* Description  : This function calls isstart. In case of failure it  */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      0               int         function performs successfully    */
/*      < 0                         isstart has failed                */
/*                                                                    */
/**********************************************************************/

int d26_isstart(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    int isfd,           /* IN    -  C-ISAM file descriptor            */
			/* INOUT -  C-ISAM key description            */
    struct keydesc *keydesc,
    int len,            /* IN    -  length of key to be used          */
    const char *record, /* IN    -  C-ISAM record containing key bytes*/
    int mode)           /* IN    -  C-ISAM start mode                 */

{                       /* d26_isstart                                */

  int ret_value;        /* return value                               */
  char key[MAXKEYSIZE]; /* key to be used                             */

  register short i;     /* loop variable                              */
  register short k_len; /* loop variable for key length               */
			/* pointer to keypart                         */
  register struct keypart *keypart;
  register char  *keyp; /* loop pointer to key                        */

  if                    /* at least debug level 3 is enabled          */
    (DCE_SVC_DEBUG_ATLEAST(gds_svc_handle,GDS_S_GENERAL,svc_c_debug3))
  {                     /* prepare parameters for logging             */
    for                 /* all key parts                              */
       (i = 0, keypart = keydesc->k_part, k_len = 0, keyp = key;
	i < keydesc->k_nparts && k_len < len; i++, keypart++)
    {                   /* copy key part into key                     */
      if                /* length of key part less than key part desc */
	(len - k_len < keypart->kp_leng)
      {                 /* copy rest of actual key                    */
	memcpy(keyp,record + keypart->kp_start,len - k_len);
	keyp  += len - k_len;
	k_len  = len;
      }
      else
      {                 /* copy complete key part                     */
	memcpy(keyp,record + keypart->kp_start,keypart->kp_leng);
	keyp  += keypart->kp_leng;
	k_len += keypart->kp_leng;
      }
    }

    DCE_SVC_LOG((D26_SVC("%b%b"),svc_c_debug3,GDS_S_ISSTART_PAR,
      sizeof(struct keydesc),(char *)keydesc,len,key));
  }

  if                    /* isstart returns error                      */
    ((ret_value = isstart(isfd,keydesc,len,(char *)record,mode)) < 0)
  {                     /* issue log messages                         */
    dce_svc_printf(D26_SVC("%d"),attributes,GDS_S_ISSTART_ERR,iserrno);
  }

  return(ret_value);

}                       /* d26_isstart                                */

/**********************************************************************/
/*                                                                    */
/* Function name: d26_isread                                          */
/*                                                                    */
/* Description  : This function calls isread. In case of failure it   */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      0               int         function performs successfully    */
/*      < 0                         isread has failed                 */
/*                                                                    */
/**********************************************************************/

int d26_isread(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    int isfd,           /* IN    -  C-ISAM file descriptor            */
    char *record,       /* OUT   -  record to be read                 */
    int mode)           /* IN    -  C-ISAM read mode                  */

{                       /* d26_isread                                 */

  int ret_value;        /* return value                               */

  if                    /* isread returns error                       */
    ((ret_value = isread(isfd,record,mode)) < 0)
  {                     /* issue log messages                         */
    dce_svc_printf(D26_SVC("%d%d"),attributes,GDS_S_ISREAD_ERR,mode,iserrno);
  }

  return(ret_value);

}                       /* d26_isread                                 */

/**********************************************************************/
/*                                                                    */
/* Function name: d26_iswrite                                         */
/*                                                                    */
/* Description  : This function calls iswrite. In case of failure it  */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      0               int         function performs successfully    */
/*      < 0                         iswrite has failed                */
/*                                                                    */
/**********************************************************************/

int d26_iswrite(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    int isfd,           /* IN    -  C-ISAM file descriptor            */
    const char *record) /* IN    -  record to be written              */

{                       /* d26_iswrite                                */

  int ret_value;        /* return value                               */

  if                    /* error has occurred                         */
    ((ret_value = iswrite(isfd,(char *)record)) < 0)
  {                     /* issue log messages                         */
    dce_svc_printf(D26_SVC("%d"),attributes,GDS_S_ISWRITE_ERR,iserrno);
  }

  return(ret_value);

}                       /* d26_iswrite                                */

/**********************************************************************/
/*                                                                    */
/* Function name: d26_isrewrite                                       */
/*                                                                    */
/* Description  : This function calls isrewrite. In case of failure it*/
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      0               int         function performs successfully    */
/*      < 0                         isrewrite has failed              */
/*                                                                    */
/**********************************************************************/

int d26_isrewrite(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    int isfd,           /* IN    -  C-ISAM file descriptor            */
    const char *record) /* IN    -  record to be rewritten            */

{                       /* d26_iswrite                                */

  int ret_value;        /* return value                               */

  if                    /* error has occurred                         */
    ((ret_value = isrewrite(isfd,(char *)record)) < 0)
  {                     /* issue log messages                         */
    dce_svc_printf(D26_SVC("%d"),attributes,GDS_S_ISWRITE_ERR,iserrno);
  }

  return(ret_value);

}                       /* d26_isrewrite                              */

/**********************************************************************/
/*                                                                    */
/* Function name: d26_isrewcurr                                       */
/*                                                                    */
/* Description  : This function calls isrewcurr. In case of failure it*/
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      0               int         function performs successfully    */
/*      < 0                         iswrite has failed                */
/*                                                                    */
/**********************************************************************/

int d26_isrewcurr(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    int isfd,           /* IN    -  C-ISAM file descriptor            */
    const char *record) /* IN    -  record to be written              */

{                       /* d26_isrewcurr                              */

  int ret_value;        /* return value                               */

  if                    /* error has occurred                         */
    ((ret_value = isrewcurr(isfd,(char *)record)) < 0)
  {                     /* issue log messages                         */
    dce_svc_printf(D26_SVC("%d"),attributes,GDS_S_ISWRITE_ERR,iserrno);
  }

  return(ret_value);

}                       /* d26_isrewcurr                              */

/**********************************************************************/
/*                                                                    */
/* Function name: d26_isrewrec                                        */
/*                                                                    */
/* Description  : This function calls isrewrec. In case of failure it */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      0               int         function performs successfully    */
/*      < 0                         iswrite has failed                */
/*                                                                    */
/**********************************************************************/

int d26_isrewrec(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    int isfd,           /* IN    -  C-ISAM file descriptor            */
    long rec_nr,        /* IN    -  C-ISAM record number              */
    const char *record) /* IN    -  record to be written              */

{                       /* d26_isrewrec                               */

  int ret_value;        /* return value                               */

  if                    /* error has occurred                         */
    ((ret_value = isrewrec(isfd,rec_nr,(char *)record)) < 0)
  {                     /* issue log messages                         */
    dce_svc_printf(D26_SVC("%d"),attributes,GDS_S_ISWRITE_ERR,iserrno);
  }

  return(ret_value);

}                       /* d26_isrewrec                               */

/**********************************************************************/
/*                                                                    */
/* Function name: d26_isdelcurr                                       */
/*                                                                    */
/* Description  : This function calls isdelcurr. In case of failure it*/
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      0               int         function performs successfully    */
/*      < 0                         isdelcurr has failed              */
/*                                                                    */
/**********************************************************************/

int d26_isdelcurr(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    int isfd)           /* IN    -  C-ISAM file descriptor            */

{                       /* d26_isdelcurr                              */

  int ret_value;        /* return value                               */

  if                    /* error has occurred                         */
    ((ret_value = isdelcurr(isfd)) < 0)
  {                     /* issue log messages                         */
    dce_svc_printf(D26_SVC("%d"),attributes,GDS_S_ISDELETE_ERR,iserrno);
  }

  return(ret_value);

}                       /* d26_isdelcurr                              */

/**********************************************************************/
/*                                                                    */
/* Function name: d26_islock                                          */
/*                                                                    */
/* Description  : This function calls islock. In case of failure it   */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      0               int         function performs successfully    */
/*      < 0                         islock failed                     */
/*                                                                    */
/**********************************************************************/

int d26_islock(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    int isfd)           /* IN    -  C-ISAM file descriptor to lock    */

{                       /* d26_islock                                 */

  int ret_value;        /* return value                               */

  if                    /* islock fails                               */
    ((ret_value = islock(isfd)) < 0)
  {                     /* issue log messages                         */
    dce_svc_printf(D26_SVC("%d"),attributes,GDS_S_ISLOCK_ERR,iserrno);
  }

  return(ret_value);

}                       /* d26_islock                                 */

/**********************************************************************/
/*                                                                    */
/* Function name: d26_isunlock                                        */
/*                                                                    */
/* Description  : This function calls isunlock. In case of failure it */
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      0               int         function performs successfully    */
/*      < 0                         isunlock failed                   */
/*                                                                    */
/**********************************************************************/

int d26_isunlock(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    int isfd)           /* IN    -  C-ISAM file descriptor to unlock  */

{                       /* d26_isunlock                               */

  int ret_value;        /* return value                               */

  if                    /* isunlock fails                             */
    ((ret_value = isunlock(isfd)) < 0)
  {                     /* issue log messages                         */
    dce_svc_printf(D26_SVC("%d"),attributes,GDS_S_ISRELEASE_ERR,iserrno);
  }

  return(ret_value);

}                       /* d26_isunlock                               */

/**********************************************************************/
/*                                                                    */
/* Function name: d26_isrelease                                       */
/*                                                                    */
/* Description  : This function calls isrelease. In case of failure it*/
/*                issues a log message and returns.                   */
/*                                                                    */
/* Return value    :                                                  */
/*      0               int         function performs successfully    */
/*      < 0                         isrelease failed                  */
/*                                                                    */
/**********************************************************************/

int d26_isrelease(
    char *file,         /* IN    -  name of source file, where the    */
			/*          function was called from          */
    int line,           /* IN    -  line number of source file        */
			/* IN    -  severity level of exception       */
    unsigned32 attributes,
    int isfd)           /* IN    -  C-ISAM file descriptor to release */

{                       /* d26_isrelease                              */

  int ret_value;        /* return value                               */

  if                    /* isrelease fails                            */
    ((ret_value = isrelease(isfd)) < 0)
  {                     /* issue log messages                         */
    dce_svc_printf(D26_SVC("%d"),attributes,GDS_S_ISRELEASE_ERR,iserrno);
  }

  return(ret_value);

}                       /* d26_isrelease                              */

