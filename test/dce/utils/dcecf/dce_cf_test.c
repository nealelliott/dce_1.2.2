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
 * $Log: dce_cf_test.c,v $
 * Revision 1.1.17.2  1996/02/18  00:23:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:19  marty]
 *
 * Revision 1.1.17.1  1995/12/08  22:08:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:43:22  root]
 * 
 * Revision 1.1.14.2  1994/06/10  20:18:50  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:12  devsrc]
 * 
 * Revision 1.1.14.1  1993/12/29  19:22:41  rsalz
 * 	Move dce_cf_XXX from config to dce/utils/dcecf (OT CR 9663).
 * 	[1993/12/29  18:37:30  rsalz]
 * 
 * $EndLog$
 */
/*
*/

/*  dce_cf_test.c V=1 05/23/91 //littl/prgy/src/dce_config
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
#include <stdio.h>
#include <dce/dce_cf_sts.h>

main(ac,av)
int ac;
char **av;
{
    char *name;
    int i, status;
    char routine[21];
    
    if (ac != 1)
    {
        fprintf(stderr, "Usage: %s\n", av[0]);
        exit(1);
    }

 for (i=0; i<2; i++)
 {
    if (i == 0)
        {
        strcpy(routine,"dce_cf_get_host_name");
    	dce_cf_get_host_name( &name, &status);
        }
    else
        {
        strcpy(routine,"dce_cf_get_cell_name");
    	dce_cf_get_cell_name( &name, &status);
        }
    
    if (status != dce_cf_st_ok)
    {
        fprintf(stderr, "%s: error %d in %s\n", av[0], status, routine);
        exit(1);
    }
    else
        printf("%s routine returns: %s\n",routine,name);
  }
}
