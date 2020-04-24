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
 * $Log: tst_dtet_util.c,v $
 * Revision 1.1.4.2  1996/02/17  23:12:00  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:27  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:54:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:27:41  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:44:48  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:16:28  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/10  22:38:23  bhaim
 * 	Common TET functions
 * 	[1994/03/10  22:36:15  bhaim]
 * 
 * $EndLog$
 */


#include <dtet/tet_api.h>       /* DTET api header file */

/*
 *  ROUTINE NAME:       tst_tet_syncall
 *
 *  SCOPE:              PUBLIC (intended for use by DCE system tests)
 *
 *  INPUTS:
 *      syncpoint:      the syncronization point on which to syncronize.
 *      timeout:        the time to wait before the syncronization fails.
 *
 *  DESCRIPTION:
 *  tst_tet_syncall is a wrapper around tet_sync call that allows the user
 *  to syncronize on multi node. This routine will retrieve the list of
 *  slave node, and syncronize the salves from the mater node. It will
 *  also check to make sure that a proper syncronization has occurred.
 *
 */
int tst_tet_syncall(int syncpoint,int timeout)
{
  static int *syncnames;
  int retcode;
  int i, j = 0;
  static int num_slaves=-1;
  static int sysid;

/*
 * Sync all nodes to ensure LMS is up for next portion of test
 */

  if (num_slaves==-1) {
    num_slaves=tet_remgetlist(&syncnames);
    sysid=tet_remgetsys();

/*
 * Current bug in TET that the syncnames is not 0 terminated this can cause
 * the mesage dupilcate names in sync list
 */
    syncnames[num_slaves] = 0;

/*  On slave nodes, the syncnames array must contain a complete list of
 *  all the slave nodes minus the node you are running on.
 */

    if (sysid != 0) {
      for (i = 0; i < num_slaves; i++)
        if (syncnames[i] != sysid)
          syncnames[j++] = syncnames[i];

/* terminate the list */
      syncnames[j] = 0;
    }
  }


  retcode=tet_sync(syncpoint, syncnames, timeout);

  if (retcode != 0) {
    tet_infoline("Failed to syncronize");
    tet_result(TET_FAIL);
  }

  return(retcode);

}

