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
 * $Log: child_ctrl.c,v $
 * Revision 1.1.4.2  1996/02/18  00:23:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:53  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:07:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:42:45  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:18:30  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:57:53  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/22  06:19:40  baum
 * 	Initial Checkin
 * 	[1994/04/22  06:18:51  baum]
 * 
 * $EndLog$
 */
/*
 * Module:	dce_rdacl_svr_opers.c
 *
 * Facility:	Child management routines
 *
 * Abstract:	
 */

/*
 * Include Files
 */

#include <acl_test_utils.h>

int
child_start 
(
 FILE	**pp_child_hnd,
 char	*p_child_cmd,
 char	*p_mode,
 char	*p_expected_output,
 char	*p_actual_output
)
{

  int		ret, len;
  char		*p_error, *ptr;

  *pp_child_hnd = popen ( p_child_cmd, p_mode );
  if ( *pp_child_hnd == NULL )
  {
    p_error = strerror ( errno );
    sprintf ( p_actual_output, "Could not start %s (%s)\n", p_child_cmd, 
	      p_error );
    return 0;
  }
  
  return ( child_wait ( *pp_child_hnd, p_expected_output, p_actual_output ) );
}

int
child_wait
(
 FILE	*hnd,
 char	*p_expected_output,
 char	*p_actual_output
)
{

  int	ret, len;
  char	*ptr, *p_error;

  if ( p_expected_output == NULL )
    return 1;

  ptr = fgets ( p_actual_output, LONG_STRING_LEN, hnd );
  if ( ptr == NULL )
  {
    p_error = strerror ( errno );
    sprintf ( p_actual_output, "Could not read from child (%s)\n", p_error );
    return 0;
  }
  p_actual_output[ strlen(p_actual_output) - 1] = '\0';

  if ( !strcmp ( p_expected_output, ACCEPT_ANY_LINE ) )
    return 1;

  len = strlen ( p_actual_output ) - strlen ( p_expected_output );
  if ( len >= 0 && !strcmp ( &p_actual_output[len], p_expected_output) )
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void
child_close
(
 FILE *p_hnd
)
{
  if ( p_hnd != NULL )
    pclose ( p_hnd );
}
