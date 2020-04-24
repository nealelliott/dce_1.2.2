/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: filespec_actions.c,v $
 * Revision 1.1.6.2  1996/02/18  19:29:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:58  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:09:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:31  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:52  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:49:54  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:36:53  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:39  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:59:04  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:01:20  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:05:26  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:36:13  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:21:24  devrcs
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

#ifdef vms
#include <descrip.h>			/* vms descriptors */
#include <fscndef.h>			/* defs for filescan sys service */
#endif					/* end of extra vms header files */

#include <prsdef.h>			/* to use portable parser */
#include <filespec.h>
#include <op_list_defs.h>

#define FILESPEC_BLOCK 541		/* MOVE TO KIND_OF_STRUCTURE_CODES.H */


/* vms and ultrix versions have different names on purpose. */
/* this allows us to support cross system specs, later, if we want to. */
/* easy to add an ms-dos os/2 version, using a similar table */

int
recognize_unix_filespec (
    unsigned char	*);

int
recognize_vms_filespec (
    unsigned char	*);		/* just one place to keep a file spec */

/* other action routines move the info to a safe place. */
/* to find the file spec block, call get_fsblk_ptr */

static struct fsblk fsb;		/* all filespecs held here */

struct fsblk *
get_fsblk_ptr (void)
{
    return(&fsb);
}

void
fsblk_clrspec (
    struct fsblk	*fsbp)		/* ptr to file spec block */
{
    fsbp->fsp = 0;			/* prevent accidental access to it */
    fsbp->memcode = 0;			/* no longer malloced */
    fsbp->size = 0;			/* bytes in spec */
    fsbp->chars = 0;			/* chars in spec */
    return;				/* no value from void routine */
}

void
fsblk_init (
    struct fsblk	*fsbp)		/* ptr to file spec blk */
{
    fsbp->kind_of_structure_code = FILESPEC_BLOCK;
    fsbp->version = 1;
    fsbp->fwd     = fsbp;		/* reserved */
    fsbp->bck     = fsbp;		/* reserved */
    fsbp->top     = fsbp;		/* reserved */
    fsblk_clrspec(fsbp);		/* no pointers or lengths */
    return;				/* no value on void routine */
}					/* end of fsblk_init */

void
fsblk_freespec (
    struct fsblk	*fsbp)		/* ptr to file spec block */
{
    if ((fsbp->fsp) && (fsbp->memcode))
	{
	free(fsbp->fsp);		/* give back the space */
	fsblk_clrspec(fsbp);
	}
    return;				/* no value from void routine */
}

void
fsblk_copyspec (
    struct fsblk	*from,
    struct fsblk	*to)
{
    to->fsp = from->fsp;		/* copy the file spec pointer */
    to->memcode = from->memcode;	/* remember if it was malloced */
    to->size = from->size;
    to->chars = from->chars;
    return;				/* void return has no value */
}

void
fsblk_movespec (
    struct fsblk	*from,
    struct fsblk	*to)
{
    fsblk_copyspec(from, to);
    fsblk_clrspec(from);
}

/* save a filespec. or if av1 is 1, save a null string meaning terminal */
int
stash_filespec (
    struct prs	*prsp)			/* ptr to parser context block */
{
    int length;				/* number of chars recognized as spec*/
    int return_code;			/* what to tell the caller, parser */

    return_code = ACT_ERROR;		/* assume no good until all is ok */

#ifdef vms
    length = recognize_vms_filespec((unsigned char *)prsp->cur);
#else
    length = recognize_unix_filespec((unsigned char *)prsp->cur);
#endif

    if ((length > 0)||(prsp->av1 == 1))	/* was a filespec recognized ? */
	{				/* yes */
	prsp->nxt = prsp->cur + length;	/* tell parser how much was handled */
	/* save the results in the single file spec block */
	fsblk_init(&fsb);		/* fill in typical values */
	fsb.fsp =			/* get space for spec & \0 */
	    (unsigned char *)malloc(length+1);
	if (fsb.fsp)
	    {
	    memcpy(fsb.fsp, prsp->cur, length);	/* save the spec */
	    *(fsb.fsp+length) = '\0';	/* with a terminal null */
	    fsb.size = length;		/* remember its size */
	    fsb.chars = length;		/* the same until multi octet chars */
	    fsb.memcode = 1;		/* remember it is in malloced space */
	    return_code = ACT_SUCCESS;	/* ok. everything worked fine */
	    }
	}
    return(return_code);		/* tell parser what happened */
}

static int access_code;			/* see op_list_defs.h */

int
set_access(				/* terminal, extend, or new file */
    struct prs *pcbp)			/* ptr to parser context block */
{
    access_code = pcbp->av1;		/* remember value from parser */
    return(ACT_SUCCESS);		/* always success, parse continues */
}

int
save_tofile (
    struct prs *pcbp)			/* ptr to parser context block */
{
    int return_code;			/* what we tell the parser */
    struct op_blk *opbp;		/* ptr to an output block */
    struct f_blk  *fmtp;		/* ptr to format info */
    char *fsp;				/* ptr to file spec, or 0 */
    int len;				/* size of file spec, or 0 */

    return_code = ACT_ERROR;		/* fail unless everything works */
    fmtp = default_format_pointer();	/* find normal default format info */
    if (access_code == SYSTEMFILE)	/* what kind of file is it */
	{				/* TO TERMINAL */
	fsp = 0;			/* no ptr to filespec */
	len = 0;			/* so it is small */
	}
    else
	{				/* new file or append */
	fsp = (char *)fsb.fsp;		/* point at saved file spec */
	len = strlen(fsp);		/* size of filespec */
	}
	
    opbp =				/* try to make an output block */
	create_op_list_element(
	    fsp,			/* ptr to file spec, or null */
	    len,			/* size of the file spec, in bytes */
	    access_code,		/* to extend or not, etc */
	    0,				/* not open or closed */
	    fmtp);			/* ptr to formatting info */
    if (fsb.memcode) free(fsb.fsp);	/* give back borrowed memory */
    fsb.memcode = 0;			/* spec is not allocated */
    fsb.fsp = 0;			/* prevent accidental use */
    if (opbp)				/* will be 0 if create failed */
	{
	struct op_blk *ignorep;		/* ptr to inserted block */

	ignorep =
	    insert_op_list_element_before(
		opbp,			/* ptr to the new element to insert */
		get_op_blk_ptr());	/* ptr to what to put it before */
	return_code = ACT_SUCCESS;	/* it worked */
	}
    return(return_code);		/* failure, unless success */
}

int
atsigntest (				/* check for an at sign */
    struct prs *pcbp)			/* ptr to parser context block */
{					/* otherwise this would be in .cdt */
    if (*(pcbp->cur)=='@')
	{
	pcbp->nxt = pcbp->cur + 1;	/* tell parser where to resume */
	return(ACT_SUCCESS);
	}
    else
	return(ACT_ERROR);
}
