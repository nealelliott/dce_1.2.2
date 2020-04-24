/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: unload_kern_ext.c,v $
 * Revision 1.1.4.2  1996/02/17  23:35:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:12  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:11:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:49  root]
 * 
 * Revision 1.1.2.2  1992/12/30  16:36:25  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:18:43  htf]
 * 
 * Revision 1.1  1992/01/19  15:33:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/* This code unloads (SYS_KULOAD) an AIX kernel extension */

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
struct cfg_load unload_data;           /* structure to pass to sysconfig() */
char *err_msg;
int status;

if (argc != 2)
  {
  printf("usage:  unload_kern_ext <kernel module id (hex)>\n");
  return(-1);
  }

unload_data.kmid = (void *)strtoul(argv[1], (char **) NULL, 16);
unload_data.path = NULL;
unload_data.libpath = NULL;                         /* dummy */
status = sysconfig(SYS_KULOAD, &unload_data, sizeof(unload_data));

if (status == 0)
  printf("unload_kern_ext:  successfully unloaded kernel module id %x\n", 
         unload_data.kmid);
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

    case EINVAL:
      err_msg = "extension not loaded or has load count of 0";
    break;

    default:
      err_msg = "unknown error";
    }
  printf("unload_kern_ext:  %s\n", err_msg);
  }
return(status);
}
