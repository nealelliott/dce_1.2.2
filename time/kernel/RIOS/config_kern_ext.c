/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: config_kern_ext.c,v $
 * Revision 1.1.4.2  1996/02/17  23:34:55  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:10:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:31  root]
 * 
 * Revision 1.1.2.2  1992/12/30  16:35:51  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:14:24  htf]
 * 
 * Revision 1.1  1992/01/19  15:33:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/* This code configures (SYS_CFGKMOD) an AIX kernel extension */

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
struct cfg_kmod  config_data;
char *err_msg, path_parm[1024];
int status;

if (argc != 3)
  {
  printf("usage:  config_kern_ext <pathname> <parameter to module>\n");
  return(-1);
  }
path_parm[1023] = 0;
strncpy(path_parm, argv[1], sizeof(path_parm)-1);

query_data.path = argv[1];                   /* kernel extension filename */
query_data.kmid = (mid_t)0;                  /* dummy */
query_data.libpath = NULL;                   /* dummy */
status = sysconfig(SYS_QUERYLOAD, &query_data, sizeof(query_data));


if ((status != 0) || (query_data.kmid == 0))
    printf("config_kern_ext:  cannot configure %s\n", path_parm);
else
  {
  config_data.kmid = query_data.kmid; 
  config_data.cmd = atoi(argv[2]);
  config_data.mdiptr = NULL;
  config_data.mdilen = 0;
  status = sysconfig(SYS_CFGKMOD, &config_data, sizeof(config_data));

  if (status == 0)
   printf("config_kern_ext:  successfully sent command %d to kernel module id %x\n", config_data.cmd, config_data.kmid);
  else
    {
    switch (errno)
      {
      case EACCES:
        err_msg = "insufficient authority, or \n             i/o error"; 
      break;
   
      case EFAULT:
        err_msg = "insufficient authority";
      break;
  
      case EINVAL:
        err_msg = "invalid kernel module id";
      break;
  
      default:
        err_msg = "unknown error";
      }
    printf("config_kern_ext:  %s\n", err_msg);
    }
  }
return(status);
}
