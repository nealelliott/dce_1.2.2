/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sysdep.c,v $
 * Revision 1.1.4.2  1996/02/17  23:59:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:09  marty]
 *
 * Revision 1.1.4.1  1995/12/07  22:37:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:18:34  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:34:02  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:48:39  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:33:48  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:14:19  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:00:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/****************************************************************/
/*  sysdep.c  - operating system dependencies                   */
/****************************************************************/

/*
 * ==========================================================================
 * Confidential and Proprietary.  Copyright 1987 by Apollo Computer Inc.,
 * Chelmsford, Massachusetts.  Unpublished -- All Rights Reserved Under
 * Copyright Laws Of The United States.
 *
 * Apollo Computer Inc. reserves all rights, title and interest with respect
 * to copying, modification or the distribution of such software programs and
 * associated documentation, except those rights specifically granted by Apollo
 * in a Product Software Program License, Source Code License or Commercial
 * License Agreement (APOLLO NETWORK COMPUTING SYSTEM) between Apollo and
 * Licensee.  Without such license agreements, such software programs may not
 * be used, copied, modified or distributed in source or object code form.
 * Further, the copyright notice must appear on the media, the supporting
 * documentation and packaging as set forth in such agreements.  Such License
 * Agreements do not grant any rights to use Apollo Computer's name or trademarks
 * in advertising or publicity, with respect to the distribution of the software
 * programs without the specific prior written permission of Apollo.  Trademark
 * agreements may be obtained in a separate Trademark License Agreement.
 * ==========================================================================
 */

#include <stdio.h>
#if defined(MSDOS) || defined(SYS5) || defined(vaxc)
#  include <string.h>
#else
#  include <strings.h>
#endif

#ifdef vms
# include <types.h>
# include <stat.h>
#else
# include <sys/types.h>
# include <sys/stat.h>
#endif

#include <rpc.h>

#include <sysdep.h>
#include <utils.h>

#ifndef MAX_INCLUSION_DEPTH
# define MAX_INCLUSION_DEPTH 10
#endif

#ifndef HASPOPEN
static int temp_count = 0;
static char *temp_names[MAX_INCLUSION_DEPTH];
char *sysdep_save_temp(old_name)
char *old_name;
{
#ifndef vms
        char new_name[64];
        char *new_name_ptr;
        sprintf(new_name,"NIDL%02d.TMP", temp_count);
        if (!(new_name_ptr = temp_names[temp_count] = alloc(strlen(new_name)+1)))
        {
                error("Couldn't allocate memory in sysdep_save_temp");
        }
        strcpy(temp_names[temp_count++], new_name);
        unlink(new_name);
        if(rename(old_name, new_name))
        {
                error("Couldn't rename temp file in sysdep_save_temp");
        }
        return(new_name_ptr);
#endif
}

void sysdep_cleanup_temp()
{
#ifndef vms
        int i;
        char *name;

        for(i = 0; i < temp_count; i++)
        {
                name = temp_names[i];
                free(temp_names[i]);
                temp_names[i] = (char *) 0;
                if(name)
                {
                        if(unlink(temp_names[i]))
                                error("Couldn't unlink temp file in sysdep_cleanup_temp");
                }
        }
        temp_count = 0;
#endif
}
#endif

#ifdef MSDOS
/* Make a legal unix file name from a DOS one */
void msdos_fix_filename(name)
char *name;
{
        while(*name)
        {
                if(*name == '\\')
                        *name = '/';
                name++;
        }
}
#endif
