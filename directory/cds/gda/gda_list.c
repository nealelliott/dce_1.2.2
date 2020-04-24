/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_list.c,v $
 * Revision 1.1.6.2  1996/02/18  19:31:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:14  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:12:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:27  root]
 * 
 * Revision 1.1.4.4  1994/08/03  19:01:37  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:53:58  mccann]
 * 
 * Revision 1.1.4.3  1994/06/23  18:30:02  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:27:39  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:38:04  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:14  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:01:36  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:09:05  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:27:12  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:43:49  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:24:17  devrcs
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

#include <gda.h>
#include <errno.h>
#include <gda_common.h>
#include <gda_rwl.h>
#include <gda_list.h>
#include <gda_debug.h>
#include <gda_main.h>
 

/*
 * The l_el_init function failed to initialize the element. 
 */
#define	L_EL_INIT_FAIL		-24

/*
 * The l_insert_head function failed to insert the element. 
 */
#define	L_INSERT_HEAD_FAIL	-27


extern int x;

void 
l_init (list_t *list)
{
    int	status;
    list->head = (element_t *)NULL;


    DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " >l_init"));

    rwl_init(&list->rwl_link);

    DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <l_init"));

}

int 
l_free (list_t *list)
{
  int	status;


  DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " >l_free"));

  if ((status = write_lock(&list->rwl_link)) != OK)
      {
	return(L_FREE_FAIL);
      }

  if (list->head == (element_t *)NULL)
      {
	DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "l_free: head null"));

	rwl_free(&list->rwl_link);

      }
  else
      {
	DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
	        "l_free: HEAD_NONNULL"));

	write_unlock(&list->rwl_link);
	return(L_FREE_FAIL);
      }

     DCE_SVC_DEBUG((
         cds__svc_handle,
         cds_svc_gda,
         svc_c_debug8,
         " <l_free"));

     return(OK);
}


static int
l_el_init (element_t  **element,
           char       *data)
{
    element_t	*el;
    int		status;


      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " >l_el_init"));

	if((*element = (element_t *)malloc(sizeof(element_t))) ==
				(element_t *)NULL)
	{
              dce_svc_printf(CDS_S_ALLOC_A_MSG);
	}
	ALLOC(element_t, 1)

	el = *element;

	el->data_el = data;
	rwl_init(&el->rwl_link);
	rwl_init(&el->rwl_data);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug8,
        " <l_el_init"));

    return(OK);
}

static int
l_el_free (element_t *element)
{
    int status;


      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " >l_el_free"));

	rwl_free(&element->rwl_link);
	rwl_free(&element->rwl_data);

	free((void *)element);
	FREE(element_t, 1)

        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <l_el_free"));

	return(OK);
}

static int
l_insert_head (char    **user_el,
               list_t  *list)
{
    element_t	*el;
    int		status;


      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " >l_insert_head")); 

	if((status = l_el_init(&el, *user_el)) != OK)
	{

	    DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug8,
		    " <l_insert_head: return(L_INSERT_HEAD_FAIL)")); 

	    return(L_INSERT_HEAD_FAIL);
	}

    DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug5,
          "l_insert_head: after l_el_init"));

	*user_el = (char *)el;

	/* 
	 * el is not yet a member of the list --> it is not subject to a 
	 * locking order 
	 */
	if((status = read_lock(&el->rwl_link)) != OK)
	{
	    DCE_SVC_DEBUG((                  
		cds__svc_handle,
                cds_svc_gda,
                svc_c_debug1,
                "l_insert_head: Error status %d from read_lock",
                status)); 

	    return(L_INSERT_HEAD_FAIL);
	}

	/*
	 * list could go away 
	 */
	if((status = write_lock(&list->rwl_link)) != OK)
	{	
	    DCE_SVC_DEBUG((                  
		cds__svc_handle,
                cds_svc_gda,
                svc_c_debug1,
                "l_insert_head: Error status %d from write_lock",
                status));

	    return(L_INSERT_HEAD_FAIL);
	}

	if(list->head == (element_t *) NULL)
	{
	    list->head = el;
	    el->next_el = (element_t *)NULL;
	    el->prev_el = (element_t *)NULL;

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_insert_head: before list->rwl_link write unlock, head null"));

	    write_unlock(&list->rwl_link);
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_insert_head: after list->rwl_link write unlock, head null"));
	}	
	else
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_insert_head: before list->head->rwl_link write lock, head not null"));
	      
	    if((status = write_lock(&list->head->rwl_link)) != OK)
	    {
		DCE_SVC_DEBUG((                  
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug1,
		    "l_insert_head: Error status %d from write_lock",
		    status));

		return(L_INSERT_HEAD_FAIL);
	      }
		
	    list->head->prev_el = el;
	    el->next_el = list->head;
	    el->prev_el = (element_t *)NULL;
	    list->head = el;

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_insert_head: before list->rwl_link write unlock, head not null"));

	    write_unlock(&list->rwl_link);
		
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_insert_head: before list->rwl_link write unlock, head not null"));

	    write_unlock(&list->head->next_el->rwl_link);
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_insert_head: after list->head->rwl_link write unlock, head not null"));
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug8,
        " <l_insert_head"));

    return(OK);
}

/*
 * a list cannot be deleted during any operation 
 */
int 
l_delete (element_t  **element,
          list_t     *list)
{
    char 	*user_el;
    element_t	*el;
    element_t	*prev;
    element_t	*prev2;
    int		status;
    int		num_tries;


      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " >l_delete"));

	el = *element;
	user_el = el->data_el;

	read_unlock(&el->rwl_data);

	/*
	 * el cannot go away 
	 */
	if ((status = read_lock(&el->rwl_link)) != OK)
	{
	  return(L_DELETE_FAIL);
	}

	for(num_tries = 0; num_tries < 25; num_tries++)
	{
	  if(el->prev_el == (element_t *)NULL)
	      {
		DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_delete: prev null"));

		read_unlock(&el->rwl_link);

	    DCE_SVC_DEBUG((
		cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_delete: after el->rwl_link read unlock"));

	    if((status = write_lock(&list->rwl_link)) != OK)
	    {
	      DCE_SVC_DEBUG((
		        cds__svc_handle,
     		        cds_svc_gda,
		        svc_c_debug8,
		        " <l_delete: Error status %d from write_lock. return(L_DELETE_FAIL)",
			status));

	      return(L_DELETE_FAIL);
	    }

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_delete: after list->rwl_link write lock"));

	    /*
	     * everything can move around while locks are not held 
	     */
	    if(list->head != el)
	    {
		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_gda,
                    svc_c_debug5,
                    "l_delete: prev_el null: the world changed"));

		write_unlock(&list->rwl_link);

		/*	
		 * el cannot be deleted 
		 */
		if((status = read_lock(&el->rwl_link)) != OK)
		{
		    DCE_SVC_DEBUG((                  
			cds__svc_handle,
  		        cds_svc_gda,
		        svc_c_debug1,
		        "l_delete: Error status %d from read_lock - prev_el null: el link lock",
		        status));

		    return(L_DELETE_FAIL);
		}

		continue;
	    }

	    if((status = write_lock(&el->rwl_link)) != OK)
	    {

		DCE_SVC_DEBUG((                  
		    cds__svc_handle,
  		    cds_svc_gda,
		    svc_c_debug1,
		    "l_delete: Error status %d from write_lock - prev_el null: el link lock",
		    status));

		return(L_DELETE_FAIL);
	      }
	    
	    DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "l_delete: after el->rwl_link write lock"));

	    if((status = write_lock(&el->rwl_data)) != OK)
	    {
		DCE_SVC_DEBUG((                  
		    cds__svc_handle,
  		    cds_svc_gda,
		    svc_c_debug1,
		    "l_delete: Error status %d from write_lock - prev_el null: el data lock",
		    status));

		return(L_DELETE_FAIL);
	    }

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_delete: after el->rwl_data write lock"));

	    if(el->next_el != (element_t *)NULL)
	    {
		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_gda,
                    svc_c_debug5,
                    "l_delete: next non null"));

		if((status = write_lock(&el->next_el->rwl_link)) != OK)
		{

		    DCE_SVC_DEBUG((                  
		       cds__svc_handle,
  		       cds_svc_gda,
		       svc_c_debug1,
		       "l_delete: Error status %d from write_lock - prev_el null, next_el null: el->next_el link lock",
		       status));
		    return(L_DELETE_FAIL);
		  }

		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_gda,
                    svc_c_debug5,
                    "l_delete: after el->next_el->rwl_link write lock"));

		el->next_el->prev_el = (element_t *)NULL;

		write_unlock(&el->next_el->rwl_link);
		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_gda,
                    svc_c_debug5,
                    "l_delete: after el->next_el->rwl_link write unlock"));

	    }

	    list->head = el->next_el;

	    write_unlock(&list->rwl_link);
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_delete: after list->rwl_link write unlock"));
	}
	else
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_delete: prev non null"));

	    prev = el->prev_el;

	    read_unlock(&el->rwl_link);

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_delete: after el->rwl_data read unlock"));

	    /*	
	     * this write lock could fail because prev was just deleted 
	     */
	    if((status = write_lock(&(prev2 = el->prev_el)->rwl_link)) != OK)
	    {

		DCE_SVC_DEBUG((                  
		    cds__svc_handle,
  		    cds_svc_gda,
		    svc_c_debug1,
		    "l_delete: Error status %d from write_lock - prev_el non-null: el->prev_el link lock",
		    status));

		if (status == WRITE_LOCK_DELETED)
		{
		    /*
		     * el cannot be deleted 
		     */
		    if((status = read_lock(&el->rwl_link)) != OK)
		    {
			DCE_SVC_DEBUG((                  
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug1,
			    "l_delete: Error status %d from read_lock - prev_el null: el link lock",
			    status));
			
			return(L_DELETE_FAIL);
		    }

		    continue;
		}
		else
		{
		    return(L_DELETE_FAIL);
		}
	    }
	    
	    if(prev != prev2)
		x = x;
	    
	    /*	
	     * everything can move around while locks are not held 
	     */
	    if(el->prev_el != prev2)
	    {
		write_unlock(&prev2->rwl_link);
		/*
		 * el cannot be deleted 
		 */
		if((status = read_lock(&el->rwl_link)) != OK)
		{
		    DCE_SVC_DEBUG((                  
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug1,
			"l_delete: Error status %d from read_lock - prev_el null: el link lock",
			status));
		    return(L_DELETE_FAIL);
		}

		continue;
	    }

	    DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "l_delete: after el->prev_el->rwl_link write lock"));

	    if((status = write_lock(&el->rwl_link)) != OK)
	    {
		DCE_SVC_DEBUG((                  
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug1,
		    "l_delete: Error status %d from write_lock - prev_el non-null: el link lock",
		    status));

		return(L_DELETE_FAIL);
	    }

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_delete: after el->rwl_link write lock"));

	    if((status = write_lock(&el->rwl_data)) != OK)
	    {
		DCE_SVC_DEBUG((                  
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug1,
		    "l_delete: Error status %d from write_lock - prev_el non-null: el data lock",
		    status));

		return(L_DELETE_FAIL);
	      }

	    DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "l_delete: after el->rwl_data write lock"));


	    if(el->next_el != NULL)
	    {
		if((status = write_lock(&el->next_el->rwl_link)) != OK)
		{
		    DCE_SVC_DEBUG((                  
		        cds__svc_handle,
		        cds_svc_gda,
		        svc_c_debug1,
		        "l_delete: Error status %d from write_lock - prev_el non-null, next_el non-null: el->next_el link lock",
		        status));

		    return(L_DELETE_FAIL);
		  }

		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_gda,
                    svc_c_debug5,
                    "l_delete: after el->next_el->rwl_link write lock"));

		el->next_el->prev_el = el->prev_el;
		el->prev_el->next_el = el->next_el;

		write_unlock(&el->next_el->rwl_link);
	    }
	    else
	    {
		el->prev_el->next_el = (element_t *)NULL;
	    }

	    write_unlock(&el->prev_el->rwl_link);
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_delete: after el->prev_el->rwl_link write unlock"));
	}
	break;
    }

    if(num_tries > 25)
    {
	DCE_SVC_DEBUG((                  
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug1,
	    " <l_delete: number of tries exceded: return(L_DELETE_FAIL)"));

	return(L_DELETE_FAIL);
    }

    if((status = l_el_free(el)) != OK)
    {
	DCE_SVC_DEBUG((                  
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug1,
	    " <l_delete: Error status %d from l_el_free: return(L_DELETE_FAIL)",
	    status));

	return(L_DELETE_FAIL);
    }
    *element = (element_t *)user_el;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug8,
        " <l_delete: return(OK)"));
 
    return(OK);
}

element_t 
*l_next (element_t *element)
{
    element_t	*el;
    int		status;


      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " >l_next"));

	if(element->next_el == (element_t *)NULL)
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_next: element->next_el null"));

	    read_unlock(&element->rwl_link);
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_next: after element->rwl_link read unlock"));

	    return((element_t *)NULL);
	}

	el = element->next_el;
	
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug5,
          "l_next: before element->next_el->rwl_link read lock"));

	if((status = read_lock(&element->next_el->rwl_link)) != OK)
	{
	  DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug1,
		    " <l_next: Error status %d from read lock. return(L_NEXT_FAIL)",
		    status));

	  return((element_t *)L_NEXT_FAIL);
	}

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug5,
          "l_next: before element->rwl_link read unlock"));

	read_unlock(&element->rwl_link);
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <l_next"));

	return((element_t *)el);
}


element_t 
*l_first (list_t *list)
{
    element_t	*el;
    int		status;


      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " >l_first"));

	if((status = read_lock(&list->rwl_link)) != OK)
	{
	    DCE_SVC_DEBUG((                  
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug1,
	       "l_first: Error status %d from read_lock - list link lock",
		status));
	    return((element_t *)L_FIRST_FAIL);
	}

	if(list->head == (element_t *)NULL)
	{
	    DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "l_first: head null"));

	    read_unlock(&list->rwl_link);
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug5,
                "l_first: after list->rwl_link read lock"));

	    return((element_t *)NULL);
	}

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug5,
            "l_first: before list->head->rwl_link read lock"));

	if((status = read_lock(&list->head->rwl_link)) != OK)
	{
	    DCE_SVC_DEBUG((                  
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug1,
	       "l_first: Error status %d from read_lock - list head link lock",
		status));
	    return((element_t *)L_FIRST_FAIL);
	}

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug5,
            "l_first: before list->rwl_link read unlock"));

	el = list->head;

	read_unlock(&list->rwl_link);
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <l_first"));

	return((element_t *)el);
}


int 
l_release_link (element_t *element)
{
  int	status;
    

  DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " >l_release_link"));

  if ((status = read_lock(&element->rwl_data)) != OK)
      {
	DCE_SVC_DEBUG((                  
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug8,
	        "l_release_link: Error status %d from read_lock",
		status));
	    
	return(L_RELEASE_LINK_FAIL);
      }

  read_unlock(&element->rwl_link);
  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <l_release_link"));

  return(OK);
}


int 
l_release_el (element_t *element)
{
    int	status;


    DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " >l_release_el"));

    read_unlock(&element->rwl_link);
    DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <l_release_el"));

    return(OK);
}


int 
l_release_data (element_t *element)
{
    int	status;


      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " >l_release_data"));

	read_unlock(&element->rwl_data);
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <l_release_data"));

	return(OK);
}


int 
l_insert (char       **user_el,
          list_t     *list,
          element_t  *el_place)
{
    element_t	*el;
    int		status;


      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " >l_insert"));

	if(el_place == NULL)
	{
		status = l_insert_head(user_el, list);
		if (status != OK)
		    return(L_INSERT_FAIL);

		return(OK);
	}

	if((status = l_el_init(&el, *user_el)) != OK)
	{
	    DCE_SVC_DEBUG((                  
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug1,
	        "l_insert: Error status %d from l_el_init",
		status));

	    return(L_INSERT_FAIL);
	}

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug5,
            "l_insert: after l_el_init"));

	*user_el = (char *)el;

	/*
	 * since el is not yet a member of list, lock order does not matter
	 */
	if((status = read_lock(&el->rwl_link)) != OK)
	{
	    DCE_SVC_DEBUG((                  
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug1,
	       "l_insert: Error status %d from read_lock",
		status));
	    return(L_INSERT_FAIL);
	}

	/*		
	 * if this call fails, el_place may be deleted before we finish
	 * so fail before anything goes wrong 
	 */
	if((status = read_to_write_lock(&el_place->rwl_link)) != OK)
	{
	    DCE_SVC_DEBUG((                  
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug1,
	        "l_insert: Error status %d from read_to_write_lock - prev_el non-null: el_place link lock",
		status));

	    if(status == READ_TO_WRITE_LOCK_BLOCKED)
	    {
		return(L_INSERT_RETRY);
	    }
	    else
	    {
		return(L_INSERT_FAIL);
	    }
	}

	if(el_place->next_el != NULL)
	{
              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "l_insert: next_el non-null"));

		if((status = write_lock(&el_place->next_el->rwl_link)) != OK)
		{
		    DCE_SVC_DEBUG((                  
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug1,
			"l_insert: Error status %d from write_lock - next_el non-null:el_place->next_el link lock",
			status));

		    return(L_INSERT_FAIL);
		}


		el->prev_el = el_place;
		el->next_el = el_place->next_el;
		el_place->next_el->prev_el = el;
		el_place->next_el = el;

		write_unlock(&el_place->rwl_link);
		write_unlock(&el->next_el->rwl_link);

	}
	else
	{
              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "l_insert: next_el null"));

		el->prev_el = el_place;
		el->next_el = (element_t *)NULL;
		el_place->next_el = el;

		write_unlock(&el_place->rwl_link);

                DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_gda,
                    svc_c_debug5,
                    "l_insert: after el_place->rwl_link write unlock"));
	}

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " <l_insert: return(OK)"));

    return(OK);
}

void 
l_init_nl (list_t *list)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug8,
        "l_init_nl"));

    list->head = (element_t *)NULL;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug8,
        " <l_init_nl"));
}


int 
l_free_nl (list_t *list)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug8,
        " >l_free_nl"));

    if(list->head == (element_t *)NULL)
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <l_free_nl: head null: return(OK)"));

	return(OK);
    }
    else
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <l_free_nl: head NON null: return(L_FREE_NL_FAIL)"));

	return(L_FREE_NL_FAIL);
    }
}


static void
l_el_init_nl (element_t  **element,
              char       *data)
{
    element_t	*el;


      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " >l_el_init_nl"));

	if((*element = (element_t *)malloc(sizeof(element_t))) ==
				(element_t *)NULL)
              dce_svc_printf(CDS_S_ALLOC_A_MSG);
       ALLOC(element_t, 2)

       el = *element;

       el->data_el = data;

       DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_gda,
           svc_c_debug8,
           " <l_el_init_nl"));
}

static void 
l_el_free_nl (element_t *element)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug8,
        " >l_el_free_nl"));

    free((void *)element);
    FREE(element_t, 2)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug8,
        " <l_el_free_nl"));
}

void
l_insert_nl (char       **user_el,
             list_t     *list,
             element_t  *el_place)
{
    element_t	*el;
    int		status;


      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " >l_insert_nl"));

	l_el_init_nl(&el, *user_el);

        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug5,
            "l_insert_nl: after l_el_init_nl"));

	*user_el = (char *)el;

	if(el_place == (element_t *) NULL)
	{
              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "l_insert_nl: el_place null"));

		if(list->head == (element_t *) NULL)
		{
                      DCE_SVC_DEBUG((
                          cds__svc_handle,
                          cds_svc_gda,
                          svc_c_debug5,
                          "l_insert_nl: head null"));

			list->head = el;
			el->next_el = (element_t *)NULL;
			el->prev_el = (element_t *)NULL;
		}	
		else
		{
                      DCE_SVC_DEBUG((
                          cds__svc_handle,
                          cds_svc_gda,
                          svc_c_debug5,
                          "l_insert_nl: head not null"));

			el->next_el = list->head;
			el->prev_el = (element_t *)NULL;
			list->head->prev_el = el;
			list->head = el;
		}
	}
	else
	{
              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "l_insert_nl: el_place non-null"));

		el->next_el = el_place->next_el;
		el->prev_el = el_place;

		if(el_place->next_el != NULL)
			el_place->next_el->prev_el = el;

		el_place->next_el = el;
	}

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " <l_insert_nl"));

}

void 
l_delete_nl (element_t  **element,
             list_t     *list)
{
    char		*user_el;
    element_t	*el;
    
    DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " >l_delete_nl"));

	el = *element;
	user_el = el->data_el;

	if(el->prev_el == (element_t *)NULL)
	{
              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "l_delete_nl: prev null"));

		if(el->next_el != (element_t *)NULL)
		{
                      DCE_SVC_DEBUG((
                          cds__svc_handle,
                          cds_svc_gda,
                          svc_c_debug5,
                          "l_delete_nl: next non null"));

			el->next_el->prev_el = (element_t *)NULL;
		}

		list->head = el->next_el;
	}
	else if(el->next_el == (element_t *)NULL)
	{
              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "l_delete_nl: prev non null next null"));

		el->prev_el->next_el = (element_t *)NULL;
	}
	else
	{
              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "l_delete_nl: prev non null next non null"));

		el->next_el->prev_el = el->prev_el;
		el->prev_el->next_el = el->next_el;

	}
	l_el_free_nl(el);
	*element = (element_t *)user_el;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug8,
          " <l_delete_nl"));

}

element_t 
*l_next_nl (element_t *element)
{
    element_t	*el;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug8,
        " >l_next_nl"));

    if(element->next_el == (element_t *)NULL)
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <l_next_nl: element->next_el null"));

	return((element_t *)NULL);
    }

    el = element->next_el;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug8,
        " <l_next_nl"));

    return((element_t *)el);
}


element_t 
*l_first_nl (list_t *list)
{
    element_t	*el;


    DCE_SVC_DEBUG((
	cds__svc_handle,
        cds_svc_gda,
        svc_c_debug8,
        " >l_first_nl"));
    
    if(list->head == (element_t *)NULL)
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug5,
            " <l_first_nl: head null"));

	return((element_t *)NULL);
    }

    el = list->head;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug8,
        " <l_first_nl"));

    return((element_t *)el);
}





