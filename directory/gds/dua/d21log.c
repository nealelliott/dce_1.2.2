/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21log.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:44  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:54:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:05  root]
 * 
 * Revision 1.1.8.5  1994/09/19  08:39:21  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:32:08  marrek]
 * 
 * Revision 1.1.8.4  1994/07/08  18:03:30  keutel
 * 	OT 11156: call new function dce_svc_init
 * 	[1994/07/08  18:03:06  keutel]
 * 
 * Revision 1.1.8.3  1994/06/21  14:46:43  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:39:12  marrek]
 * 
 * Revision 1.1.8.2  1994/05/10  15:53:57  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  07:29:21  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:20:56  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:32:14  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:06:00  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:33:01  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:18:55  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:55:35  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:15:42  marrek
 * 	Insert <pthread.h>, <stdio.h>, <unistd.h> and <string.h>
 * 	use d21_l01_init()
 * 	[1993/02/02  09:22:31  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:47:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:25:10  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:31:37  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:38:26  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  16:52:57  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:21:47  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:26:03  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:37:34  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21log.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:03 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*
* daton **************************************************************
*                                                                    *
* NAME         :  d21log.c                                           *
*                                                                    *
* AUTHOR       :  J.Forster                                          *
* DATE         :  22.04.88                                           *
*                                                                    *
* DESCRIPTION  :  Logging functions                                  *
*                                                                    *
* datoff *************************************************************
*/

#include <gds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <d21dua.h>
#include <dce/dcesvcmsg.h>

/*
**********************************************************************
*                                                                    *
*       D E K L A R A T I O N of D A T A 			     *
*                                                                    *
**********************************************************************
*/

static  Bool             init_log=FALSE;

/* local functions */

static void d21_00_do_init_once(void);
static void d21_L017_init_svc(void);

/*-----------------------------------------------------------------------*/
/*
	d21_00_do_init_once is called once only from d21_L01_init,
	which is called many times from all over the place.
	The flag init_log remains, to allow a quick return to
	a non-threaded version if necessary.
*/

static void d21_00_do_init_once(void)
{
#ifdef THREADSAFE
    char function_name[] = "d21_00_do_init_once";
#endif

if (init_log == FALSE)
  {
  d21_L017_init_svc();

#ifdef THREADSAFE
  if (D00_dua_protect_init() != 0)
    {
    DUA_TRACE_PROTECT_INIT_FAILED("dua")
    } /* end if */

  if (D00_gds_sec_protect_init() != 0)
    {
    DUA_TRACE_PROTECT_INIT_FAILED("gds_sec")
    } /* end if */

  #ifdef OSFCDS
    if (D00_switch_protect_init() != 0)
      {
      DUA_TRACE_PROTECT_INIT_FAILED("switch")
      } /* end if */
  #endif /* OSFCDS */
#endif /* THREADSAFE */

  init_log = TRUE;
  } /* end if */

} /* end d21_00_do_init_once */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name: d21_L01_init						*/
/*									*/
/* Description	: This function initializes the trace system and the	*/
/*			mutexes.					*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : April 5, 1994						*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d21_L01_init(void)
{
#ifdef THREADSAFE
	static pthread_once_t once_block = pthread_once_init;

	pthread_once(&once_block,d21_00_do_init_once);
#else
	d21_00_do_init_once();
#endif

} /* end d21_L01_init */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name: d21_L02_trace_bind_input				*/
/*									*/
/* Description	: This function traces IAPL input arguments		*/
/*			(for bind operation)				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : April 5, 1994						*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d21_L02_trace_bind_input(
  D21_p1 * parblk)
{
}

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name: d21_L03_trace_unbind_input				*/
/*									*/
/* Description	: This function traces IAPL input arguments		*/
/*			(for unbind operation)				*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : April 5, 1994						*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d21_L03_trace_unbind_input(
  D21_p2 * parblk)
{
}

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name: d21_L04_trace_iapl_input				*/
/*									*/
/* Description	: This function traces IAPL input arguments.		*/
/*			(object name, common arguments,...)		*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : April 5, 1994						*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d21_L04_trace_iapl_input(
  D2_name_string obj_name,
  D2_c_arg * com_arg)
{
if (obj_name != NULL)
  {
  DUA_TRACE_D_N(obj_name)
  }
else
  {
  DUA_TRACE_D_N("")
  }

DUA_TRACE_C_ARG(com_arg)
}

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name: d21_L017_init_svc					*/
/*									*/
/* Description	: This function initializes the serviceability.		*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : April 5, 1994						*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/


static void d21_L017_init_svc(void)
{
error_status_t svc_st;
unsigned32 flags;

/* call dce_svc_init - it will do SVC intialization 
   (esp. rpc_sm_enable_allocate) */
/* flags are not yet used in dce_svc_init - so we set them to 0 */

flags = 0;
dce_svc_init(flags,&svc_st);
if (svc_st != svc_s_ok)
  {
  fprintf(stderr,"Unable to initialize serviceability, status: 0x%lx\n",
		svc_st);
  }

/* register message table */
dce_msg_define_msg_table(gds_msg_table,
			 sizeof(gds_msg_table)/sizeof(gds_msg_table[0]),
			 &svc_st);
if (svc_st != msg_s_ok)
  {
  fprintf(stderr, "Unable to register message table, status: 0x%lx\n", svc_st);
  }

/* register at serviceability */
gds_svc_handle = dce_svc_register(gds_svc_table,(idl_char *)"gds",&svc_st);

if (svc_st != svc_s_ok)
  {
  fprintf(stderr,"Unable to register at serviceability, status: 0x%lx\n",
		svc_st);
  }

} /* end of d21_L017_init_svc */

