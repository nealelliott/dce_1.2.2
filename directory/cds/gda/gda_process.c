/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_process.c,v $
 * Revision 1.1.6.2  1996/02/18  19:31:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:23  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:12:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:31  root]
 * 
 * Revision 1.1.4.4  1994/08/03  19:01:40  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:52:35  mccann]
 * 
 * Revision 1.1.4.3  1994/06/23  18:30:05  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:27:50  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:38:08  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:19  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:01:41  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:09:24  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:27:21  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:44:04  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:24:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 */

/*
	process.c contains the code which dispatches the question from
	the gda common code to a gda conduit.
*/

#include <gda.h>
#include <gda_main.h>
#include <dce/assert.h>
#include <errno.h>

extern int x;

/*
	process determines which conduit down which the question should be
	sent initializes the conduit handler routine, sends the conduit the
	question and cleans up after an answer is returned.
*/
int 
process (question_el_t *element)
{
  int		status;
  int		ret_val;  /* return value from a conduit routine */

#define	ELEMENT	LIST_REF(question_el_t, element)

  DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >process"));

  /*
   * Kick out null unresolved names (usually when resolving /...) 
   */
  if (element->unresolved_in_q.fn_length == 0) 
      {
	DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug2,
                " <process: NULL name - return(DNS_UNKNOWNENTRY)"));

	return(DNS_UNKNOWNENTRY);
      }

  /*
   * Find out which conduit to send the question down 
   */
  if((status = choose_conduit(element)) != OK)
      {
	/*
	 * If no conduit can be found, return the progress record sent 
	 */
	DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug2,
                " <process: return(DNS_NOTSUPPORTED)"));

	return(DNS_NOTSUPPORTED);
      }

  if(element->conduit_q->single_threaded_c)
      {
	/*
	 * The conduit is single threaded 
	 * Grab the conduit lock 
	 */
	status = write_lock(&element->conduit_q->rwl_c);
	dce_assert(cds__svc_handle, (status == OK));
      }
  /*
   * initialize the conduit handler routine 
   */
  ret_val = (element->conduit_q->func_c
	     [element->routine_q][INIT_FUNCTION])(element);

  /*
   * place question in list 
   */
  if((status = l_insert((char **)&element, &question_list,
			(element_t *)NULL))!= OK)
      {
	/* Only happen if list alread deleted. */
	error_exit_th(-1);
      }
  /*
   * downgrade read lock on list to read lock on data in list 
   */
  if((status = l_release_link((element_t *)element)) != OK)
      {
	/* lock has been deleted, get out of here. */
	error_exit_th(-1);
      }
  
  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug5,
            "process: before conduit routine"));

  /*
   * call conduit handler routine 
   */
  ret_val = (ELEMENT->conduit_q->func_c
	     [ELEMENT->routine_q][PROCESS_FUNCTION])(ELEMENT);
  
  if(ELEMENT->conduit_q->single_threaded_c)
      {
	/*
	 * The conduit is single threaded 
	 */
	write_unlock(&ELEMENT->conduit_q->rwl_c);
      }
  
  /*
   * remove element from list 
   */
  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug5,
            "process: before l_delete"));

  if((status = l_delete((element_t **)&element, &question_list)) != OK)
      {
	/* Lock alread deleted */
	error_exit_th(-1);
      }	

  /*
   * destroy the conduit handler routine 
   */
  (void)(element->conduit_q->func_c
	 [element->routine_q][DESTROY_FUNCTION])(element);
  
  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <process"));

  /*
   * return to rpc routine 
   */
  
  return(ret_val);
#undef	EL
}


