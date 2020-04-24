/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rdconf.h,v $
 * Revision 1.1.6.2  1996/02/18  23:07:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:35  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:42:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:19:44  root]
 * 
 * Revision 1.1.4.1  1994/06/10  20:49:32  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:19:23  devsrc]
 * 
 * Revision 1.1.2.2  1993/03/12  04:26:18  mhickey
 * 	Final checkin for initial submit to 1.0.2 backing tree
 * 
 * 	Defect 5887.
 * 	[1993/03/11  21:38:41  mhickey]
 * 
 * $EndLog$
 */

/*---*
 *---* rdconf.h - defines for data structures, bufsize and error return 
 *---*            values for the rdconf() routine used by some systests.
 *---*
 *---* ASSOCIATION
 *---*    This file is part of the DCE 1.0.2 System Tests
 *---*
 *---*    The DCE 1.0.2 system tests corresponds to the
 *---*    DCE1.0.2 Core Component System Test plan.  For more information on 
 *---*    the operation and goals of the DCE 1.0.2 system tests, see the test 
 *---*    plan. 
 *---*
 *---* FUNCTION
 *---*    Define values for the config file reading routine.
 *---* 
 *---* CALLED BY
 *---*    N.A.
 *---* 
 *---* CALLS INTO 
 *---*    Nothing
 *---*
 *---* PREREQUISITES
 *---*    None.
 *---*/

/*---*
 *---* Lets be idempotent!
 *---*/
#if !defined(_SYSTEST_READ_CONF_H)

#  define _SYSTEST_READ_CONF_H

#  if !defined(RD_CNF_INBUFSZ)
#     define RD_CNF_INBUFSZ 1024
#  endif  /*---* !defined(RD_CNF_INBUFSZ) *---*/

   /*------------------------------------------------------------------------*
    *---* Data type for configuration parameters.  Passed to rpcsec2_rd_conf()
    *------------------------------------------------------------------------*/
   typedef struct {
      char *param_spec,     /*---* the identifier for the param we need a
                             *---* value for 
                             *---*/
           *param_val;      /*---* the value we get for the param *---*/
   } param_spec_val_t, *param_spec_val_p_t;

   /*---* 
    *---* Definitions for return values from config file read routine 
    *---*/
#  define rdconf_ok        0   /*---* no problem *---*/
#  define rdconf_syntax    1   /*---* config file syntax error *---*/
#  define rdconf_cantopen  2   /*---* no config file *---*/
#  define rdconf_noparam   3   /*---* Could not match all param 
                                        *---* ids from config file
                                        *---*/
#  define rdconf_nospace   4   /*---* no space for internal array *--*/

#endif  /*---* !defined(_SYSTEST_READ_CONF_H) *---*/
