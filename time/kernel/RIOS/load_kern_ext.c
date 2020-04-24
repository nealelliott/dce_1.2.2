/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: load_kern_ext.c,v $
 * Revision 1.1.4.2  1996/02/17  23:34:59  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:10  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:10:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:44  root]
 * 
 * Revision 1.1.2.2  1992/12/30  16:36:14  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:15:53  htf]
 * 
 * Revision 1.1  1992/01/19  15:33:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* This code installs (SYS_KLOAD) an AIX kernel extension */

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
struct cfg_load load_data;              /* structure to pass to sysconfig() */
char *err_msg;
int status;

if (argc != 2)
  {
  printf("usage:  load_kern_ext <pathname>\n");
  return(-1);
  }

load_data.path = argv[1];                    /* kernel extension filename */
load_data.kmid = (mid_t)0;                   /* dummy */
load_data.libpath = NULL;                         /* dummy */
status = sysconfig(SYS_KLOAD, &load_data, sizeof(load_data));

if (status == 0)
  printf("load_kern_ext:  successfully loaded %s, kernel module id %x\n", 
         argv[1], load_data.kmid);
else
  {
  switch (errno)
    {
    case EACCES:
      err_msg = "insufficient authority, or \n                not an ordinary file, or \n             file denies read-only permission";
    break;
 
    case EFAULT:
      err_msg = "insufficient authority, or i/o error accessing file";
    break;

    case ENOEXEC:
      err_msg = "invalid XCOFF format object file, or unable to resolve imported symbol";
    break;

    case EINVAL:
      err_msg = "XCOFF header damaged or incorrect for this machine";
    break;

    case ENOMEM:
      err_msg = "memory required exceeds system-imposed limit";
    break;

    case ETXTBSY:
      err_msg = "object file is open for write by process"; 
    break;

    default:
      err_msg = "unknown error";
    }
  printf("load_kern_ext:  %s\n", err_msg);
  }
return(status);
}
