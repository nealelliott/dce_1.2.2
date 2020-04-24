/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: err_list.c,v $
 * Revision 1.1.6.2  1996/02/18  19:28:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:53  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:08:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:28  root]
 * 
 * Revision 1.1.4.4  1994/08/03  19:00:48  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:43:35  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:36:49  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:34  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/04  20:06:00  peckham
 * 	Fixes for sams file cleanup
 * 	[1994/04/04  18:05:43  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  21:58:58  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:00:55  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:04:57  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:35:42  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:23:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
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


#include <cdscp.h>
#include <err_list_defs.h>	    /* definitions of error list */
#include <kind_of_structure_codes.h>	/* to identify structures */

struct err_blk *get_err_blk_ptr(void);


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      Initialize the list header.
**
**	Routine exists for generality and debugging convenience.
**	The first planned use could have been handled by having
**	initial values on the structure declaration.
**
**  FORMAL PARAMETERS:
**
**      address of list header
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      list header is initialized, ie no errors have occured
**
**  COMPLETION CODES:
**
**      none.
**	if the routine returns, it has worked.
**	problems are signalled. the most likely problem is a user error:
**	bad address of list header, resulting in access violation.
**
**  SIDE EFFECTS:
**
**      any list elements are lost.
**	before using this twice, delete all records in the error list
**--
**/

void
err_list_hdr_init (
	struct err_blk *p)
{
    ZERO_bytes (p, sizeof (struct err_blk));
    p->kind_of_structure_code = ERR_LIST_CODE;   /* identify the structure*/
    p->kind_of_element = HEADER_ELEMENT;
    p->fwd_ptr  = p;		    /* point to self */
    p->back_ptr = p;		    /* point to self */

    /* the following are not needed */
    /* they just make the header neat in a dump */
    p->size = sizeof(*p);

    return;				/* no value from void routine */
}

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      Insert a new error list element in front of a specified element
**
**  FORMAL PARAMETERS:
**
**      address of the new block to link in
**	address of an existing block for the list
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      links are changed
**
**  FUNCTION VALUE:
**
**      Pointer to the new list element if the function returns.
**	Typical reason for failure to return is access violation 
**	caused by supplying a bad parameter.
**
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

struct err_blk *
insert_err_list_element_before (
	struct err_blk *x,
	struct err_blk *p)
{
    /* the error list is a doubley linked list. */
    /* the header has the same format as each element */
    /* the x is the new element, the p is the old, established element */

    /* first, link the new block into the chain */
    x->fwd_ptr  = p;
    x->back_ptr = p->back_ptr;
    /* then link the preceeding and next elements to the new element */
    /* the order is important */
    (p->back_ptr)->fwd_ptr  = x;
    p->back_ptr = x;
    return(x);			/* address of the new block */
}

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      Insert a new error list element after a specified element
**
**  FORMAL PARAMETERS:
**
**	address of the new error list element
**      address of the existing error list element
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      links are changed
**
**  FUNCTION VALUE:
**
**      Pointer to the new list element if the function returns.
**	Typical reason for failure to return is access violation 
**	caused by supplying a bad parameter.
**
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

struct err_blk *
insert_err_list_element_after (
	struct err_blk *x,
	struct err_blk *p)
{
    /* same comments as for insert_err_list_element_before */
    x->fwd_ptr  = p->fwd_ptr;	/* link the new block into the chain */
    x->back_ptr = p;
    (p->fwd_ptr)->back_ptr = x;
    p->fwd_ptr = x;
    return(x);			/* address of the new block */
}

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      Create a new error list element
**
**  FORMAL PARAMETERS:
**
**      address of header block for the list
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      space for list element is allocated, links are changed
**
**  FUNCTION VALUE:
**
**      Pointer to the new list element if the function returns.
**	Typical reason for failure to return is access violation 
**	caused by supplying a bad parameter.
**
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

struct err_blk *
create_err_list_element (
	unsigned char *p_in,		/* address of the opaque full name */
	struct dnsFlagStat *flags,	/* API flags structure */
	char *func_name)		/* the id of fctn that failed */
{
    const int name_length = dnsGetOpqFullLen ((dns_opq_fname *)p_in);
    const int total_needed = sizeof(struct err_blk) + name_length - 1;
    struct err_blk *const x = (struct err_blk *)malloc(total_needed);

    if ( x == NULL)
	dce_svc_printf(CDSCP_ASKNOMEM_MSG);

    /* fill in the block */
    /* identify the structure as an error list element */
    x->kind_of_structure_code = ERR_LIST_CODE;
    x->kind_of_element = ERROR_ELEMENT;
    x->size = total_needed;
    x->api_function_name = func_name;
    COPY_bytes (flags, &x->flags, sizeof (struct dnsFlagStat));
        /* and move the name into the block */
    COPY_bytes(p_in, x->full_name, name_length);
    /* finally, link the new block into the chain of errors */
    insert_err_list_element_before(x, get_err_blk_ptr());
	/* if we ever need to create and insert after do this: */
	/* remove the insert from this routine */
	/* create two new routines, create_err_elem_after &before */
	/* change calls to this, to call one of them */
	/* each of them uses this and the appropriate insert rtn */
    return(x);			/* address of the new block */
}

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      Create a new error list element, for an error which does not 
**	neccessarily stem from an API error.  
**
**	This routine was written in an attempt to make DNSCP error handling 
**	somewhat more consistent, although to do that some kluges (sp?) are 
**	done here since assumptions had been made that the error list is only 
**	used for failed API calls.
**
**  FORMAL PARAMETERS:
**	name_p : Address of the full name of the entity which caused
**	         the error.  Note: this is NOT opaque.  
**		 NOTE: this can be null if no entity triggered error.(INPUT)
**	code   : The error code (a code which is found in the 
**		 dnsmsg.msg message file) (INPUT)
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      space for list element is allocated, links are changed
**
**  FUNCTION VALUE:
**
**      Pointer to the new list element if the function returns.
**	Typical reason for failure to return is access violation 
**	caused by supplying a bad parameter.
**
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

struct err_blk *
nonapi_err_list_element (
	unsigned char *name_p,		/* addr of the full name - NOT opaque */
	int	      code)		/* the error code */
{
    struct err_blk	*x;		/* address of new element */
    dns_opq_fname	oname;		/* opaque full name */
    int			name_length=0;	/* length of the opaque full name */
    int			total_needed;	/* amount of space to malloc */
    char    *y;				/* alias for x */


    if (!name_p)				 /* If not given entity name  */
	{
	name_length = sizeof (NULLFULLNAME_t);	 /* Get len of empty full name*/
	}					 /* If not given entity name  */
    else
	{					 /* Have entity name  */
	/*
	**  Convert the full name to opaque format, since that's what the error 
	**  display routines expect it to be (derror in ERR_REPORT.C).
	*/
	name_length = sizeof (oname);
	dnsCvtCDSFullToOpq(name_p, &oname, &name_length);
	}					 /* Have entity name  */

    total_needed = sizeof(struct err_blk) + 	 /* Add space for name   */
	    name_length - 1;         

    x = (struct err_blk *)
	malloc(total_needed);	/* Get space for one element. */
    if ( x == NULL)
	dce_svc_printf(CDSCP_ASKNOMEM_MSG);

    /* 
    ** Fill in the block 
    */
    y = (char *)x;			/* so blk_size does not get squared */

    /* 
    ** Identify the structure as an error list element 
    */
    x->kind_of_structure_code = ERR_LIST_CODE;
    x->kind_of_element = ERROR_ELEMENT;
    x->size = total_needed;
    x->api_function_name = "";	    /* Not concerned with API function /*

    /*
    **  Fill in only those fields of the flags structure which are needed
    **  later on for error reporting.  (Needed by derror function)
    */
    x->flags.fsLocStat = code;
    x->flags.fsPartialRes = 0;

    if (name_p)					 /* Have entity name  */
	{
	COPY_bytes(&oname, x->full_name, name_length);
	}
    else 
	{					 /* No entity name  */
	ZERO_bytes( x->full_name, name_length);
	}					 /* No entity name  */

    /* 
    ** Finally, link the new block into the chain of errors 
    */
    insert_err_list_element_before(x, get_err_blk_ptr());

    return(x);			/* address of the new block */
}

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      this function deletes the specified err_list element
**
**  FORMAL PARAMETERS:
**
**      address of the err_list element to be deleted
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      the err_list is modified to delete the element.
**
**  COMPLETION CODES:	none
**
**  SIDE EFFECTS:
**
**      [@description_or_none@]
**
**--
**/

void
delete_one_err_list_element (
	struct err_blk *p)
{
    /* link around the element in forward direction. */
    (p->back_ptr)->fwd_ptr = p->fwd_ptr;
    /* link around the element in backward direction. */
    (p->fwd_ptr)->back_ptr = p->back_ptr;

    /* return the space occupied by the element */
    free(p);
    
    return;				/* no return value from void routine */
}

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      delete_all_err_list_elements deletes all the elements of the 
**	error list.
**
**  FORMAL PARAMETERS:
**
**      p is a pointer to the list head.
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      see delete_one_err_list_element which is used repeatedly.
**
**  COMPLETION CODES: none
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

void
delete_all_err_list_elements (
	struct err_blk *p)
{
    struct err_blk *pp, *trash_pp;

    /* for each element, delete the element from the list. */
    pp = p->fwd_ptr;
    while (pp != p)
    {
        trash_pp = pp;                  /* set element to be removed */
        pp = pp->fwd_ptr;               /* point to next element */
	delete_one_err_list_element(trash_pp);
    }
    return;				/* no value from void routine */
}
