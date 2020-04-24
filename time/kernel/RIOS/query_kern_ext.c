/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: query_kern_ext.c,v $
 * Revision 1.1.4.2  1996/02/17  23:35:00  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:11  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:11:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:46  root]
 * 
 * Revision 1.1.2.2  1992/12/30  16:36:20  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:16:01  htf]
 * 
 * Revision 1.1  1992/01/19  15:33:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/* This code queries (SYS_QUERYLOAD) an AIX kernel extension */

#include <stdio.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysconfig.h>

extern int  sysconfig(int cmd, void *arg, int len);

main(argc, argv)
int argc; 
char *argv[];
{
struct cfg_load query_data;              /* structure to pass to sysconfig() */
char *err_msg, path_parm[1024];
int status;

if (argc != 2)
  {
  printf("usage:  query_kern_ext <pathname>\n");
  return(-1);
  }

path_parm[1023] = 0;
strncpy(path_parm, argv[1], sizeof(path_parm)-1);
query_data.path = argv[1];                   /* kernel extension filename */
query_data.kmid = (mid_t)0;                  /* dummy */
query_data.libpath = NULL;                   /* dummy */
status = sysconfig(SYS_QUERYLOAD, &query_data, sizeof(query_data));

if (status == 0)
  {
  if (query_data.kmid != 0)
    printf("query_kern_ext:  kernel module %s is loaded as id %x\n", 
           path_parm, query_data.kmid);
  else
    printf("query_kern_ext:  kernel module %s not loaded\n", path_parm);
  }
else
  {
  switch (errno)
    {
    case EACCES:
      err_msg = "insufficient authority";
    break;
 
    case EFAULT:
      err_msg = "insufficient authority, or i/o error accessing file";
    break;

    case EIO:
      err_msg = "i/o error";
    break;

    default:
      err_msg = "unknown error";
    }
  printf("query_kern_ext:  %s\n", err_msg);
  }
return(status);
}
