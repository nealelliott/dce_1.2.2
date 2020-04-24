/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: op_list.c,v $
 * Revision 1.1.11.2  1996/02/18  19:29:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:33  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:10:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:54  root]
 * 
 * Revision 1.1.9.6  1994/08/03  19:01:13  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:46:15  mccann]
 * 
 * Revision 1.1.9.5  1994/06/23  19:09:17  jd
 * 	Removed extern evtblk. It no longer exists
 * 	[1994/06/23  18:58:26  jd]
 * 
 * Revision 1.1.9.4  1994/06/23  18:29:51  mccann
 * 	sams cleanup drop1
 * 	[1994/06/23  18:24:00  mccann]
 * 
 * Revision 1.1.9.3  1994/06/09  18:37:32  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:30  devsrc]
 * 
 * Revision 1.1.9.2  1994/04/04  20:06:14  peckham
 * 	Fixes for sams file cleanup
 * 	[1994/04/04  18:08:19  peckham]
 * 
 * Revision 1.1.9.1  1994/03/12  22:00:29  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:05:23  peckham]
 * 
 * Revision 1.1.7.2  1993/06/24  19:46:55  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:36:19  hinman]
 * 
 * Revision 1.1.4.3  1993/02/03  22:33:27  blurie
 * 	Fix for OT #7106.  For SVR4, add defintions needed to link
 * 	parser_aid when DNS_CMA has not been defined.
 * 	[1993/02/03  20:55:13  blurie]
 * 
 * Revision 1.1.4.2  1992/12/30  13:08:20  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:39:56  zeliff]
 * 
 * Revision 1.1.2.4  1992/05/06  16:37:21  m_sawyer
 * 	add #include <pthreads.h> for blocking i/o
 * 	[1992/05/06  15:58:42  m_sawyer]
 * 
 * Revision 1.1.2.3  1992/04/14  20:50:36  mfox
 * 	Fixes to allow CDS to compile cleanly with RIOS ANSI C compiler
 * 	[1992/04/14  19:40:34  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  21:53:00  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:23:02  weisman]
 * 
 * Revision 1.1  1992/01/19  15:22:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*************************************************************************
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
 *************************************************************************/


/*
**++
**  FACILITY:
**
**      DNS -- Distributed Name Service
**	NCL -- Network Control Language
**
**  ABSTRACT:
**
**      This module contain routines to manipulate and use the 
**	output list capability added to DNS$CONTROL as part of
**	the SHOW MAP command.
**
**	This package is designed for general use in any application
**	that needs to send output to several destinations.
**	It is specificly designed to be used with other modules that
**	support an NCL environment.
**
**  AUTHORS:
**
**      Chuck Bradley
**
**
**  CREATION DATE:     17-December-1987
**
**  MODIFICATION HISTORY:
**	Janet M. Mccann	3-8-89
**		Portablility changes
**	ceb 5/89 more portability and ncl changes
**	ceb 10/89 removed ties to dns
**
**  9-Jan-1992 by H. Tinkelenberg
**	Use DceText jacket routine rather than private routine get_dce_text.
**--
**/

#include <cdscp.h>
#include <kind_of_structure_codes.h>	/* to identify structures */
#include <dce/dce_error.h>

#define DEFAULT_LINES_PER_PAGE 66
#define VERY_LARGE_NUMBER 7654321
#define NORMAL_BOTTOM_MARGIN 4

static void set_top_of_one_form(struct op_blk *);

void
simple_header_routine (
	struct op_blk	*p)		/* points to a file element */
{
    emit_line(p, "\f");		    /* just a form feed */
    return;			    /* no return value on void routine */
}


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      Initialize an output block element .
**
**	Routine exists for generality and debugging convenience.
**	The first planned use could have been handled by having
**	initial values on the structure declaration.
**
**  FORMAL PARAMETERS:
**
**      address of block
**
**--
**/
void
init_op_list_element (
	struct op_blk	*p)
{
    p->kind_of_structure_code =
	OP_LIST_ELEMENT_CODE;	    /* identify the structure*/
    p->kind_of_element = FILE_ELEMENT;
    p->version  = 1;
    p->fwd_ptr  = p;		    /* point to self */
    p->back_ptr = p;		    /* point to self */
    p->head_ptr = p;		    /* this is the head */

    /* the following are not needed */
    /* they just make the block neat in a dump */
    p->file_ptr = UNDEFINED;
    p->file_access_code = UNDEFINED;
    p->file_status = UNDEFINED;
    p->format.file_attr = UNPAGED;
    p->format.width = DEFAULT_WIDTH;
    p->format.lines_per_page = DEFAULT_LINES_PER_PAGE;
    p->format.top_margin = DEFAULT_TOP_MARGIN;
    p->format.bottom_margin = DEFAULT_BOTTOM_MARGIN;
    p->format.lines_remaining = 0;
    p->format.page_number = 0;
    p->format.header_routine = DEFAULT_HEADER_ROUTINE;
    p->callback = NULL;			/* no callback */
    p->callparm = NULL;			/* no parameter on the call */
    p->spec_len = 0;
    p->spec_alloc = 0;
    p->spec = 0;
}

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      Initialize the list header.
**
**	Routine exists for generality and debugging convenience.
**	The first planned use could have been handled by having
**	initial values on the above structure declaration.
**
**  FORMAL PARAMETERS:
**
**      address of list header
**
**      list header is initialized, ie no output list elements.
**	use of the output routines will produce no output.
**
**      any list elements are lost.
**	the files they identify may be left open.
**
**--
**/
void
op_list_hdr_init (
	struct op_blk	*p)
{
    init_op_list_element(p);	    /* fill in most fields */
    p->kind_of_structure_code = OP_LIST_HEAD_CODE;   /* identify the structure*/
    p->kind_of_element = HEADER_ELEMENT;
}

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      Insert a new output list element in front of a specified element
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
struct op_blk *
insert_op_list_element_before (
	struct op_blk	*x,		/* ptr to element to insert */
	struct op_blk	*p)		/* ptr to element x will preceed */
{
    /* the output list is a doubley linked list. */
    /* the header has the same format as the front of each element */
    /* the x is the new element, the p is the old, established element */

    /* first, link the new block into the chain */
    x->fwd_ptr  = p;
    x->back_ptr = p->back_ptr;
    /* then link the preceeding and next elements to the new element */
    /* the order is important */
    (p->back_ptr)->fwd_ptr  = x;
    p->back_ptr = x;
    x->head_ptr = p->head_ptr;	/* copy ptr to head from block that has it */
    return(x);			/* address of the new block */
}

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      Insert a new output list element after a specified element
**
**  FORMAL PARAMETERS:
**
**	address of the new output list element
**      address of the existing output list element
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
struct op_blk *
insert_op_list_element_after (
	struct op_blk	*x,		/* ptr to element to insert */
	struct op_blk	*p)		/* ptr to block it goes after */
{
    /* same comments as for insert_op_list_element_before */
    x->fwd_ptr  = p->fwd_ptr;	/* link the new block into the chain */
    x->back_ptr = p;
    (p->fwd_ptr)->back_ptr = x;
    p->fwd_ptr = x;
    x->head_ptr = p->head_ptr;	/* copy ptr to head from block that has it */
    return(x);			/* address of the new block */
}

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      Create a new output list element
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
struct op_blk *
create_op_list_element (
	char		*p_in,		/* address of the file spec */
	int		spec_length,	/* length of the file spec */
	int		access_code,	/* becomes file_access_code */
	int		status_code,	/* becomes file_status */
	struct f_blk	*filefmt_info)	/* how to present info in the file */
{
    struct op_blk *x;		/* address of new element */
    char *y;			/* alias for x */
    int blk_size;		/* size of raw block, without file spec */
    int total_needed;		/* amount of space to request, includes spec */
    int last_char_pos;		/* offset to trailing \0 from head of block */
    char *plop_it_here;		/* where to put the trailing \0 */

    blk_size = sizeof(struct op_blk);	/* room for the block sans spec */
    total_needed = blk_size + spec_length + 1;	/* and with spec appended */
    last_char_pos = total_needed - 1;

    x =				/* get space for one element. tell where. */
	(struct op_blk *)malloc(total_needed);
    if ( x != NULL)
	{			/* malloc worked */
	/* fill in the block */
	y = (char *)x;			/* so blk_size does not get squared */
	/* identify the structure as an output list element */
	x->kind_of_structure_code = OP_LIST_ELEMENT_CODE;
	x->version = 1;
	x->kind_of_element = FILE_ELEMENT;
	x->memcode = 1;			/* blk is in malloc memory */
	x->file_access_code = access_code;
	x->file_status = status_code;
	x->file_ptr = UNDEFINED;
	x->format = *filefmt_info;
	x->callback = NULL;		/* no callback yet */
	x->callparm = NULL;		/* so no param for callback either */
	x->spec_len = spec_length;
	x->spec = y + blk_size;	/* char after fixed portion */
	/* and move the filespec into the block */
	memcpy(x->spec, p_in, spec_length); 
	/* and give it a terminator */
	plop_it_here = y + total_needed - 1;
	*plop_it_here = '\0';
	x->spec_alloc = 0;	/* space for spec not separately allocated */
	}			/* only pointers not filled in */
    return(x);			/* address of the new block, or 0 */
}

	/* the following two routines have identical logic. */
	/* they differ only in where the new block is inserted in the list. */
	/* the typical use is to insert before the head of the list. */
	/* that gives a fifo when used with forward traversal of the list. */
	/* insert after gives a stack behavior. */
	/* the routines return a pointer to the new block, or 0 */
	/* probable cause of 0 is failure of malloc */

struct op_blk *
create_op_list_element_before (
	char		*p_in,		/* ptr to file spec */
	int		spec_length,	/* length of file spec */
	int		access_code,	/* becomes file_access_code */
	int		status_code,	/* becomes file status */
	struct f_blk	*p_finfo,	/* info about the file/presentation */
	struct op_blk	*p_next)	/* address of element to insert before*/
{
    struct op_blk *newblk;		/* pointer to the new element */

    newblk = create_op_list_element(p_in, spec_length, 
		access_code, status_code, p_finfo);
    if (newblk)
	insert_op_list_element_before(newblk, p_next);
    return(newblk);
}

struct op_blk *
create_op_list_element_after (
	char		*p_in,		/* ptr to file spec */
	int		spec_length,	/* length of file spec */
	int		access_code,	/* becomes file_access_code */
	int		status_code,	/* becomes file status */
	struct f_blk	*p_finfo,	/* info about the file/presentation */
	struct op_blk	*p_prev)	/* address of element to insert after */
{
    struct op_blk *newblk;		/* pointer to the new element */

    newblk = create_op_list_element(p_in, spec_length, 
			access_code, status_code, p_finfo);
    if (newblk)
	insert_op_list_element_after(newblk, p_prev);
    return(newblk);
}


void
unlink_op_list_blk (
	struct op_blk	*p)		/* ptr to block to be unlinked */
{
    /* link around the element in forward direction. */
    (p->back_ptr)->fwd_ptr = p->fwd_ptr;
    /* link around the element in backward direction. */
    (p->fwd_ptr)->back_ptr = p->back_ptr;
    return;				/* no return value from void routine */
}


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      this function deletes the specified op_list element
**
**  FORMAL PARAMETERS:
**
**      address of the op_list element to be deleted
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      the op_list is modified to delete the element.
**	the file identified by the op_list element may be closed.
**	the space occupied by the op_list element may be released.
**	(see the comments for the numerous exceptions).
**
**  COMPLETION CODES:
**
**      normal successful completion
**	any problems are signalled.  there are many possible problems.
**	all problems are believed to be programming errors.
**
**  SIDE EFFECTS:
**
**      [@description_or_none@]
**
**--
**/
int
delete_one_op_list_element (
struct op_blk *p)
{
    /* close the file if it can be found and if it is open. */
    /* but only if it is not a process permanent file */
    close_one_file(p);			/* close it if it is open */

    /* give back the space occupied by the file spec if separately alloced */
    if (p->spec_alloc) free(p->spec);

    unlink_op_list_blk(p);

    /* return the space occupied by the element, if appropriate */
    if (p->memcode) free(p);		/* vaxc gives status. std c does not */

    return( DNS_SUCCESS );		/* normal successful completion */
}


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      delete_all_op_list_elements deletes all the elements of the 
**	output list.
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
**      see delete_one_op_list_element which is used repeatedly.
**
**--
**/
int
delete_all_op_list_elements (
	struct op_blk	*p)
{
    struct op_blk *pp, *trash_pp;
    int status;

    /* for each element, delete the element from the list. */

    status = DNS_SUCCESS;
    pp = p->fwd_ptr;
    while (pp != p)
    {
        trash_pp = pp;                  /* set element to be removed */
        pp = pp->fwd_ptr;               /* point to next element */

	if ( delete_one_op_list_element(trash_pp) != DNS_SUCCESS )
	    status = DNS_ERROR;		/* track failures */
    }
    return( status );
}


int					/* return DNS_SUCCESS or DNS_ERROR */
open_all_files (			/* open all files on the files list */
	struct op_blk *p)		/* pointer to head of the list */
{
struct op_blk *pp;
FILE *file_status;
char buf[dce_c_error_string_len];
int status;

status = DNS_SUCCESS;				/* no problems yet */
for (pp = p->fwd_ptr; pp != p; pp = pp->fwd_ptr)
    {
    if (((pp->file_access_code == NEWFILE) ||
	(pp->file_access_code == APPEND) ) &&
	(pp->file_status != FILE_OPEN))
	    {
	    file_status = NULL;
	    if ( pp->file_access_code == NEWFILE)
		file_status = fopen(pp->spec, "w");
	    else if (pp->file_access_code == APPEND)
		file_status = fopen(pp->spec, "a");

	    if	(file_status == NULL)
		{
		dce_printf (CDSCP_FILE_NOT_OPEN, pp->spec);
		pp->file_status = UNDEFINED;
		status = DNS_ERROR;
		delete_one_op_list_element(pp);		/* prevent writing */
		}
	    else
		{
		pp->file_status = FILE_OPEN;
		}
	    pp->file_ptr = file_status;  /* save the pointer to the file */
	    }
    else if (pp->file_access_code == SYSTEMFILE)
	{
	pp->file_status = FILE_OPEN;
	pp->file_ptr = stdout;
	}
    }				/* end of for loop that visits each filespec */
    return( status );
}


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      close all the output files
**
**  FORMAL PARAMETERS:
**
**      p   points to the head of the output list
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      none
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

void
close_all_files (
struct op_blk *p)
{
struct op_blk *pp;

for (pp = p->fwd_ptr; pp != p; pp = pp->fwd_ptr)    /* for each file */
    {
    close_one_file(pp);				    /* close it */
    }
}


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      close one output file
**
**  FORMAL PARAMETERS:
**
**      pp  pointer to a block in the output list
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      none
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/
int				/* return DNS_SUCCESS or DNS_ERROR */
close_one_file (
struct op_blk *pp)
{
    int file_status;
    char buf[dce_c_error_string_len];
    int status;

    buf[0] = '\0';
    if (((pp->file_access_code == NEWFILE) ||
	(pp->file_access_code == APPEND) ) &&
	(pp->file_status == FILE_OPEN))
	{
	fclose(pp->file_ptr);
	pp->file_status = FILE_CLOSED;
      }
    return ( DNS_SUCCESS );
}

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      emit a line to the outputs
**
**  FORMAL PARAMETERS:
**
**      p	pointer to head of the output list
**	data	pointer to the data to be written
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      stuff gets written
**
**  FUNCTION VALUE:
**
**      none
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

void
emit_line (
struct op_blk *p,
char *data)
{
    struct op_blk *pp;

    /* it the output block we are pointing at is a header, emit to all files */
    /* if it is an output element, write only to that element */
    /* this is a way to let us do file specific output in the middle of a */
    /* cycle of writing to all files */
    /* this hack/crock was added to allow page headers without adding */
    /* several more parameters to many, many calls. */
    /* the benefit is it helps keep the output routines looking consistant */

    if (p->file_ptr == NULL)
	{
	fputs(data, stdout);
	fflush(stdout);
	}
    else if (p->kind_of_element == HEADER_ELEMENT)
	{
	for (pp = p->fwd_ptr; pp != p; pp = pp->fwd_ptr)    
	    emit_one_copy_of_line(pp, data);
	}
    else
	{
	emit_one_copy_of_line(p, data);
	}
}


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      write the line on one output
**
**  FORMAL PARAMETERS:
**
**      pp	pointer to the output block
**	data	pointer to the data
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      data gets written
**
**  FUNCTION VALUE:
**
**      none	we can not detect errors in the output operation
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/
void
emit_one_copy_of_line (
struct op_blk *pp,
char *data)
{
    int emit_line_status;
    int len, used, togo;
    int result;				/* 1=write, 0=callback did it */

    {
    /* check for end of page, and start a new one if needed */
    /* we expect that stdout is the only file that might be non-paged */
    if (pp->format.lines_remaining <= pp->format.bottom_margin)
	{
	if (pp->format.lines_per_page != 0)	/* paged device or file */
	    {
	    /* some observations */
	    /* lines remaining has to be before emiting the headings */
	    /* each headings routine mightr put a different number of lines */
	    /* we will enter an infinite recursion loop if lines per page */
	    /* is too small to allow the biggest set of headers + 1 line */
	    /* fprintf(pp->file_ptr, "\f"); */
	    pp->format.lines_remaining = pp->format.lines_per_page;
	    if (pp->format.header_routine != NULL)
		(*pp->format.header_routine)(pp);
	    /* the headings routine is in a separate module */
	    /* it has to be tailored to each application */
	    /* the rest of this module might be reusable */
	    }
	else
	    {
	    /* this just keeps us out of this code most of the lines */
	    pp->format.lines_remaining = VERY_LARGE_NUMBER;
	    }
	}

    /* put the data to the file */
    if ( (pp->format.file_attr == PAGED) && (pp->format.width > 0) )
	{
	len = strlen(data);
	used = 0;
	togo = len;
	while (togo > pp->format.width)
	    {				/* write a full line of it */
	    /* use the callback if there is one, typically used for logging */
	    result = 1;
	    if (pp->callback)
		result = (*(pp->callback))(pp->callparm, pp, data+used);
	    /* write the line unless there was a callback and it said no */
	    if (result)
		{
		fwrite(data+used, sizeof(char), pp->format.width, pp->file_ptr);
		fputs("\n", pp->file_ptr);	/* advance to next line */
		}
	    pp->format.lines_remaining--;	/* another line used */
	    used += pp->format.width;
	    togo -= pp->format.width;
	    }
	/* no need to check for zero togo */
	/* use the callback if there is one, typically used for logging */
	result = 1;
	if (pp->callback)
	    result = (*(pp->callback))(pp->callparm, pp, data+used);
	/* write the line unless there was a callback and it said no */
	if (result)
	    {
	    emit_line_status = fprintf(pp->file_ptr, "%s", data+used);
	    pp->format.lines_remaining--;	/* count the lines printed */
	    }
	}
    else
	{
	/* use the callback if there is one, typically used for logging */
	result = 1;
	if (pp->callback)
	    result = (*(pp->callback))(pp->callparm, pp, NULL);
	/* write the line unless there was a callback and it said no */
	if (result)
	    {
	    emit_line_status = fprintf(pp->file_ptr, "%s", data);    
	    pp->format.lines_remaining--;	/* count the lines printed */
	    }
	}

    /* check that it got there */
    /* can not check properly. */
    /* status seems to be the number of characters written */
    if (emit_line_status < 0)		    /* # of % to fill in */
	{				    /* report the problem */
	dce_printf(CDSCP_BUG_EMIT_LINE, emit_line_status, data, pp->spec);
	}
    pp->format.lines_remaining--;		/* count the lines printed */

    }
}


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      emit one blank line to the outputs
**
**  FORMAL PARAMETERS:
**
**      p   points to the head of the output list
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      stuff gets written
**
**  FUNCTION VALUE:
**
**      none
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

void
emit_blank_line (
struct op_blk *p)
{			    /* special case justified by frequent use */
    emit_line(p, "\n");	    /* write one blank line */
}


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      return true if access code is valid 
**
**  FORMAL PARAMETERS:
**
**      code	the access code
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      true if code is valid, and false otherwise
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

int
valid_access_code (
int code)
{
    int answer;

    answer = (
	((code < MINIMUM_ACCESS_CODE) || (code > MAXIMUM_ACCESS_CODE)) ? 0 : 1);
    return(answer);
}


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      return true if status code is valid 
**
**  FORMAL PARAMETERS:
**
**      code	the status code
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      true if code is valid, and false otherwise
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

int
valid_status_code (
int code)
{
    int answer;

    answer = (
	((code < MINIMUM_STATUS_CODE) || (code > MAXIMUM_STATUS_CODE)) ? 0 : 1);
    return(answer);
}


/* this routine fills in a structure with constants, making a constant struct */
/* it exists only to hide the knowledge of the internals of the structure. */

void
set_default_format (
	struct f_blk	*p)
{
    p->file_attr	= UNPAGED;
    p->width		= DEFAULT_WIDTH;
    p->lines_per_page	= 0;
    p->top_margin	= 0;
    p->bottom_margin	= 0;
    p->lines_remaining	= 0;
    p->page_number	= 0;
    p->header_routine	= NULL;		/* none needed */
}


int					    /* 1 or 0 */
op_list_empty (				    /* return 1 if list is empty */
    struct op_blk *p)			    /* ptr to any block in the list */
{
    return( p->fwd_ptr == p );		    /* see if it points to itself */
}					    /* if so only one blk, so hdr */



/* this routine fills in a structure with constants, making a constant struct */
/* it exists only to hide the knowledge of the internals of the structure. */

void
set_default_stdout (
	struct op_blk	*p)
{
    static char stdoutname[] = "stdout";

    p->kind_of_structure_code	= OP_LIST_ELEMENT_CODE;
    p->version			= 1;
    p->kind_of_element		= FILE_ELEMENT;
    p->file_access_code		= SYSTEMFILE;
    p->file_status		= UNDEFINED;	    /* ??? */
    p->file_ptr			= stdout;
    p->spec			= stdoutname;
    p->spec_len			= sizeof(stdoutname);

    /* p->memcode, fwd, back, and head pointers not touched */
    }

static struct f_blk deffmt;         /* a default format block */
static struct op_blk defopb;        /* the default output block */


struct f_blk *
default_format_pointer (void)		/* let user find the block */
{					/* extra flexibility for future users */
    return(&deffmt);
}

struct op_blk *
default_op_blk_pointer (void)		/* let user find the block */
{					/* extra flexibility for future users */
    return(&defopb);
}

void
op_default_init (void)
{
    set_default_format(&deffmt);        /* establish formating rules */
    set_default_stdout(&defopb);        /* op_blk for stdout */
    defopb.memcode = 0;                 /* not allocated dynamically */
    return;				/* no value from void routine */
}

void
use_default_op_element (		/* add default element to output list */
    struct op_blk *listp,		/* ptr to head of output list */
    struct op_blk *blkp)		/* ptr to default output block */
{
    struct op_blk *ignorep;		/* value returned is known, unused */

    ignorep =
	insert_op_list_element_before(
	    blkp,			/* ptr to block to insert */
	    listp);			/* ptr to head of list */
}					/* before head is after all others */

void
use_default_op_list (			/* replace entire list by default */
    struct op_blk *p)			/* ptr to list head */
{
    delete_all_op_list_elements(p);	/* get rid of anything there now */
    use_default_op_element(p, &defopb);	/* insert the default output info */
    return;				/* no value from void routine */
} 


void
allow_default_op_list (			/* ensure there is at least one place */
    struct op_blk *p)			/* ptr to list head */
{
    if (op_list_empty(p))		/* how will we tell user of problems */
	{				/* the list is empty */
	use_default_op_element(p, &defopb);
	}				/* inserted the default output info */
    return;				/* no value from void routine */
} 


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      force next line to appear on a new page
**
**  FORMAL PARAMETERS:
**
**      p   ptr to head of the output block list or output block
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      none
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

void
new_page (
struct op_blk *p)
{
    /* if we are at a header block, start a new page on all outputs. */
    /* if at a file element, start a new page on that file only */

    struct op_blk *pp;

    if (p->kind_of_element == HEADER_ELEMENT)
	for (pp=p->fwd_ptr; pp != p; pp=pp->fwd_ptr) /* for each output file */
	    {
	    pp->format.lines_remaining = 0;    /* next line goes on new page */
	    }			    /* no page with headers and no data */
    else
	p->format.lines_remaining = 0;    /* next line goes on new page */

}


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      set lines remaining to indicate top of form
**
**  FORMAL PARAMETERS:
**
**      p	ptr to output list or to one block of it
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      none
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

void
set_top_of_form (
struct op_blk *p)
{
    struct op_blk *pp;

    if (p->kind_of_element == HEADER_ELEMENT)
	{
	for (pp=p->fwd_ptr; pp!=p; pp=pp->fwd_ptr)
	    set_top_of_one_form(pp);
	}
    else
	set_top_of_one_form(p);
}


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      set lines remaining to indicate top of one form
**
**  FORMAL PARAMETERS:
**
**      p	ptr to one output block
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      none
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

static void
set_top_of_one_form (
struct op_blk *p)
{
    if (p->file_access_code == SYSTEMFILE)
	{
	p->format.lines_remaining = DEFAULT_LINES_PER_PAGE;
	/* see if it is a device, and if so use the page parameter */
	}
    else
	{
	p->format.lines_remaining = DEFAULT_LINES_PER_PAGE;
	}
}
