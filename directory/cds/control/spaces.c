/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spaces.c,v $
 * Revision 1.1.6.2  1996/02/18  19:30:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:57  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:11:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:08  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:01:24  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:43:55  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:49  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:52  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:01:09  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:07:15  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:10:03  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:42:14  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:22:17  devrcs
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
**************************************************************************/

#include <cdscp.h>

/*
**++
**  FACILITY:
**
**      This is a general purpose utility function
**
**  ABSTRACT:
**
**      spaces returns the address of a string of blanks or spaces.
**	that is, a string of X'20' or octal 40 characters,
**	terminated by a NULL, X'00' character.
**	This is called an ASCIZ string.
**
**	Use this function, for example, in a printf statement to 
**	put 3 leading blanks for each level in an outline:
**	printf("%s%s\n", spaces(3*level), topic);
**	C allows the use of a function returning the address of a string
**	in this context.
**
**	Before the first use of spaces, use spaces_init to initialize
**	the array of space characters used by the spaces function.
**--
**/

#include <stdio.h>			/* because of FILE in cmdblk.h */
/* the following includes should be in quotes instead of in <> */
#include <cmdblk.h>

#define    MAXSIZE (MAX_NCL_CMD+2)    

    /* set up all the blanks that are needed. */
    /* have it available throughout the module, but not outside the module */
    /* use spaces_init() to initialize it since C does not have a way to. */
    /* (can not use initialize, since size is unknown, due to MAXSIZE. */
static char all_spaces[MAXSIZE+1];


/*
**
** OWN STORAGE:  none
**
**/


/*
**
** EXTERNAL REFERENCES:  none
**
**/

/* 2 functions, spaces(number) and spaces_init() */

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      spaces returns the address of an ASCIZ string of spaces.
**
**	If the parameter is <0, it is treated as 0.
**	If the parameter is >MAXSIZE, it is treated as MAXSIZE.
**
**  FORMAL PARAMETERS:
**
**      number	    the number of spaces to provide the address of
**
**  IMPLICIT INPUTS:
**
**      This function uses one ASCIZ string of MAXSIZE spaces.
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      the address of the string
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

char *
spaces (
int number)	    /* the number of spaces requested */
{
int x;		    /* the number of spaces, after validation & adjustment */

    x = number;
    x = ( (x > MAXSIZE) ? MAXSIZE : x );    /* fix if too large */
    x = ( (x < 0) ? 0 : x );		    /* fix if too small */
    return(&all_spaces[MAXSIZE - x]);
}

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      spaces_init	creates the string of spaces used by the spaces rtn.
**
**  FORMAL PARAMETERS:
**
**      none
**
**  IMPLICIT INPUTS:
**
**      the array where the string is formed.
**
**  IMPLICIT OUTPUTS:
**
**      the ASCIZ string of spaces, properly terminated by a NULL.
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
spaces_init (void)
{
    char one_space = '\40';
    int i;				/* for loop control */

    for (i=0;i<MAXSIZE;i++) {all_spaces[i] = one_space;};
    all_spaces[MAXSIZE] = '\0';
}
