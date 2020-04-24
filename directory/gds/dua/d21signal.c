/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21signal.c,v $
 * Revision 1.1.4.2  1996/02/18  18:17:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:54:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:43  root]
 * 
 * Revision 1.1.2.1  1994/06/21  14:39:40  marrek
 * 	Created in June 94 code submission.
 * 	[1994/06/10  16:04:24  marrek]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: d21signal.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:17:44 $";
#endif

/*----------------------------------------------------------------------*/
/*									*/
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG		*/
/* All Rights Reserved							*/
/*									*/
/* File Name	: d21signal.c						*/
/*									*/
/* Description  : The module contains functions for signal handling	*/
/*									*/
/* Date         : July 08, 1994						*/
/*									*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

#include <gds.h>
#include <signal.h>

#include <d21dua.h>

static int	d21_sig_count = 0 ;
static sigset_t	d21_sig_omask ;

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_disable_signals				*/
/*									*/
/* Description	: This function is used to disable specific signals.	*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : July 08, 1994						*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d21_disable_signals (void)
{
const char function_name[] = "d21_disable_signals";
sigset_t d21_sig_nmask ;

BEGIN_d21_sig_count_LOCK
d21_sig_count++;

if (d21_sig_count <= 1)
  {
  sigemptyset (&d21_sig_nmask) ;
  sigaddset (&d21_sig_nmask, SIGHUP) ;
  sigaddset (&d21_sig_nmask, SIGQUIT) ;
  sigaddset (&d21_sig_nmask, SIGINT) ;
  sigaddset (&d21_sig_nmask, SIGTERM) ;

  /* disable some general signals to avoid process termination during */
  /* critical sections */
  if (sigprocmask (SIG_BLOCK, &d21_sig_nmask, &d21_sig_omask) == -1)
    {
    DUA_TRACE_SIGPROCMASK_FAILED
    }
  } /* end of : sig_count <= 1 */
END_d21_sig_count_LOCK

} /* end of d21_disable_signals */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_enable_signals				*/
/*									*/
/* Description	: This function is used to enable specific signals.	*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : July 08, 1994						*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d21_enable_signals (void)
{
const char function_name[] = "d21_enable_signals";
BEGIN_d21_sig_count_LOCK

d21_sig_count--;
if (d21_sig_count == 0)
  {
  /* restore saved signal mask */
  if (sigprocmask (SIG_SETMASK, &d21_sig_omask, (sigset_t *) NULL) == -1)
    {
    DUA_TRACE_SIGPROCMASK_FAILED
    }
  } /* end of : d21_sig_count == 0 */

END_d21_sig_count_LOCK

} /* end of d21_enable_signals */
