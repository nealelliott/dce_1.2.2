/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: err_report.c,v $
 * Revision 1.1.10.1  1996/08/09  11:57:00  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  15:02 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	CDS bug fixes from DEC
 * 	[1995/11/02  14:24:03  marcoh  1.1.2.1]
 *
 * 	Added code to detect null fullname in derror.  Part of fix for
 * 	QAR 421.  Also see child/clerk_choose_ch.c and child/clerk_tree_walk.c
 * 	[1995/11/02  14:23:33  marcoh]
 *
 * 	DCE for Digital UNIX: populate from R1.1
 * 	[1995/11/01  15:23:34  marcoh]
 *
 * Revision 1.1.8.2  1996/02/18  19:28:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:54  marty]
 * 
 * Revision 1.1.8.1  1995/12/08  15:09:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:28  root]
 * 
 * Revision 1.1.6.5  1994/09/06  17:36:56  proulx
 * 	fix timestamps
 * 	[1994/09/06  17:28:45  proulx]
 * 
 * 	fix timestamps
 * 
 * 	fix timestamps
 * 
 * Revision 1.1.6.4  1994/08/03  19:00:49  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:50:02  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:29:31  mccann
 * 	sams cleanup drop1
 * 	[1994/06/23  18:13:05  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:36:50  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:36  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:58:59  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:01:01  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:05:03  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:35:49  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:47:35  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:21:28  weisman]
 * 
 * Revision 1.1  1992/01/19  15:22:36  devrcs
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
#include <err_list_defs.h>	/* for accumulating error data */
#include <options.h>		/* processing options */
#include <dce/dce_error.h>

static int derror(struct op_blk *,unsigned char *,char *,struct dnsFlagStat *);

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      emit_error_report : Print all the errors that have happened
**
**  FORMAL PARAMETERS:
**
**      [@description_or_none@]
**
**  IMPLICIT INPUTS:
**
**      [@description_or_none@]
**
**  IMPLICIT OUTPUTS:
**
**      [@description_or_none@]
**
**  {@function_value_or_completion_codes@}
**
**      [@description_or_none@]
**
**  SIDE EFFECTS:
**
**      [@description_or_none@]
**
**--
**/

void
emit_error_report(
struct err_blk *ep,			/* pointer to error list */
struct  op_blk *op)			/* pointer to output list */
{
    struct err_blk *pp;			/* to scan the error list */

   
    /* this can be called with ep pointing to the head of the list	*/
    /* or pointing into the list. As items near the front of the list	*/
    /* are used, the starting point is advanced. 			*/

    pp = ((ep->kind_of_element == HEADER_ELEMENT ) ? ep->fwd_ptr : ep );
    for ( ; pp->kind_of_element == ERROR_ELEMENT; pp=pp->fwd_ptr)
      {
	derror (op, pp->full_name, pp->api_function_name, &pp->flags);
	emit_blank_line(op);
      }
}

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      derror : emit_err_title	writes a title line for an error report
**
**  FORMAL PARAMETERS:
**
**      outbp	    pointer to the output block
**	msgbp	    pointer to message block
**	entity_name pointer to entity fullname or null indicating no entity 
**		    triggered problem
**	call	    ID of API call that failed
**	flags	    pointer to API flags structure
**
**  IMPLICIT INPUTS:
**
**      the correspondence between error code numbers and words defining 
** 	errors in file dnsmsg.msg
**
**  IMPLICIT OUTPUTS:
**
**      the line gets written
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

/* Report an cdsapi error */

static int 
derror(
	struct op_blk *outbp,		/* ptr to output block */
	unsigned char *entity_name,	/* ptr to opaque entity name or null */
	char *call_name,		/* API call that failed */
	struct dnsFlagStat *flags)	/* ptr to the API flags structure */
{
    char ent_name_string[DNS_STR_FNAME_MAX];
    char part[DNS_STR_FNAME_MAX];
    int code;				/* error code after standardizing it */

    if (!EMPTY_FullName(entity_name))
	{					 /* Had entity name  */
	format_fullname_for_print(	         /* cvt opaque name to string */
	    (FullName_t *)entity_name,	         /* ptr to opq entity fullname*/
	    ent_name_string);

	emit_line_fmt(outbp, CDSCP_ENTITY_ERROR,
		"Error on entity: %s\n", ent_name_string);
	}					 /* Had entity name  */

					/* Print out the API error text */
    code = flags->fsLocStat;		/* isolate the error code */

    dce_fprintf(stdout, code);
    fprintf(stdout, "\n");
    /* 
    **  For CDS, try to identify the failing routine.
    */

    emit_line_fmt(outbp, CDSCP_MSG_FUNCTION,
	"Function: %s\n", call_name);

#ifndef VMS
    if (flags->fsDNSStat == DNS_UNKNOWNENTRY - DNS_NAMESERVERERRORS)
    {
	if (EMPTY_FullName((FullName_t *)flags->fsPartialRes))
	  (void) strcpy (part, "Null Fullname");
	else
	  format_fullname_for_print((FullName_t *)flags->fsPartialRes, part);
	emit_line_fmt(outbp, CDSCP_PARTRESULT,
		"%s: partial results = %s\n", call_name, part);
    }
#endif  /* !VMS */

}
