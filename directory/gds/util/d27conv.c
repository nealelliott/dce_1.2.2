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
 * $Log: d27conv.c,v $
 * Revision 1.1.6.3  1996/02/18  18:23:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:40  marty]
 *
 * Revision 1.1.6.2  1995/12/08  16:16:14  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/DCE1_1_WP/1  1995/04/26  19:42 UTC  jrr
 * 	Cast malloc returns to (char *) to make AIX happy.
 * 	[1995/12/08  15:21:21  root]
 * 
 * Revision 1.1.2.3  1994/06/21  14:48:31  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  12:10:06  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:18:49  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:17  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:16:07  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  15:04:26  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  10:02:21  marrek]
 * 
 * 	Bug fixes.
 * 	[1994/04/29  09:10:06  marrek]
 * 
 * 	Created for April Submission.
 * 	[1994/04/26  12:52:57  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27conv.c,v $ $Revision: 1.1.6.3 $ $Date: 1996/02/18 18:23:41 $";
#endif


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1994 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Component   : DCE 1.1 GDS Conversion Routines	               */
/*                                                                 */
/* File Name   : d27conv.c                                         */
/* Description : This file contains modules for                    */
/*      	     T.61 to LATIN conversions and back.               */
/* Date        : 15. April 1994                                    */
/* Author      : Wolfgang Schmid, SNI BU BA NM 123                 */
/*                                                                 */
/*-----------------------------------------------------------------*/

/*-- Includes -----------------------------------------------------*/
/*
 * The following piece of code should be used when the "private" conversion
 * routines are replaced by "ICONV":
 *
 * #include <iconv.h>
 */
#include <malloc.h>
#include <gds.h>
#include <d2dir.h>
#include <d27conv.h>
#include <d27char.h>


/*-- Local Global Variable(s) -------------------------------------*/
/*
 * The following piece of code should be used when the "private" conversion
 * routines are replaced by "ICONV":
 *
 * static iconv_t local_t61_cd; 	conversion descriptor for LOCAL
 *					code to T.61 conversions
 * static iconv_t t61_local_cd;		conversion descriptor for T.61 
 *					to LOCAL code conversions
 */

/*-- Exported Function(s) -----------------------------------------*/

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d27_init_conv_tables()                    */
/* Description         : This routine initializes the conversion   */
/*			 tables for LOCAL code to T61 and T61 to               */
/*			 LOCAL code conversions.                               */
/* Return Value        : D2_NOERROR 	initialization successful  */
/*			 D2_ERROR	initialization fails                       */
/* Author              : Wolfgang Schmid, SNI BU BA NM 123         */
/*                                                                 */
/*-----------------------------------------------------------------*/

d2_ret_val d27_init_conv_tables( void ) 
{
/*
 * The following piece of code should be used when the "private" conversion
 * routines are replaced by "ICONV":
 *
 * char *local_code;
 *
 * local_code = nl_langinfo(CODESET);
 *
 * if ((local_t61_cd = iconv_open("T.61", local_code)) == (iconv_t) NULL) {
 *	return(D2_ERROR);
 * }
 *
 * if ((t61_local_cd = iconv_open(local_code, "T.61")) == (iconv_t) NULL) {
 *	iconv_close(local_t61_cd);
 *	return(D2_ERROR);
 * }
 * return(D2_NOERROR);
 */

	OmCharInit();

	return(D2_NOERROR);
} /* end d27_init_conv_tables */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d27_close_conv_tables()                   */
/* Description         : This routine closes the conversion tables.*/
/* Return Value        : D2_NOERROR 	routine successful         */
/*			 D2_ERROR	routine fails              */
/* Author              : Wolfgang Schmid, SNI BU BA NM 123         */
/*                                                                 */
/*-----------------------------------------------------------------*/

d2_ret_val d27_close_conv_tables( void ) 
{
/*
 * The following piece of code should be used when the "private" conversion
 * routines are replaced by "ICONV":
 *
 * iconv_close(local_t61_cd);
 * iconv_close(t61_local_cd);
 * 
 * return(D2_NOERROR); 
 */

	return(D2_NOERROR);
} /* end d27_close_conv_tables */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d27_t61_to_local()		           */
/* Description         : This routine converts the string "inp_str"*/
/*			 (of size "inp_len") to the output string  */
/*			 "*out_str" (size is "*out_len").          */
/*			 Note: *out_len (as input parameter) holds */
/*			       the maximum length of the buffer    */
/*			       area where the converted string     */
/*			       should be generated. If in this case*/
/*			       *out_len is 0, no memory area was   */
/*			       allocated up so far; this routine   */ 
/*			       will allocate the memory area.      */
/*			       As output parameter it contains the */
/*			       actual length of the generated      */
/*			       output string.			   */
/* Return Value        : D2_NOERROR:	    conversion successful          */
/*			 otherwise: conversion failed              */
/* Author              : Wolfgang Schmid, SNI BU BA NM 123         */
/*                                                                 */
/*-----------------------------------------------------------------*/

d2_ret_val d27_t61_to_local(const char *inp_str, const unsigned32 inp_len, 
			  char **out_str, unsigned32 *out_len) 
{
/*
 * The following piece of code should be used when the "private" conversion
 * routines are replaced by "ICONV":
 *
 * signed32 outbytesleft, len;
 *
 * if (*out_len == 0) {
 *	memory are has not been allocated up so far; so try to allocate the
 *	memory for the converted string and in case is to little memory, then
 *	realloc
 *
 * 	len = *out_len = inp_len * D27_T61_CONV_FACT;		
 *	if ((*out_str = malloc(*out_len)) == NULL) {
 *		return (D2_ERROR);
 *	}
 *
 *	for ( ; ; ) {
 * 		outbytesleft = *out_len;
 * 		if (iconv(t61_local_cd, &inp_str, &inp_len, &out_str, 
 *			  &outbytesleft) != (size_t) -1) {
 *	 		*out_len -= outbytesleft; 
 *			if (*out_len < len) {
 *				*out_str = realloc(*out_str, *out_len);
 *				if (*out_str == NULL) {
 *					*out_len = 0;
 *					return(D2_ERROR);
 *				}
 *			} 
 *			return(D2_NOERROR);
 *		}
 *		else {
 *			if (errno == E2BIG) {
 *				free(*out_str);
 *				*out_len = 0;
 *				return(D2_ERROR);
 *			}
 *			else {
 *				*out_len += inp_len;
 *				len = *out_len;
 *				if ((*out_str = realloc(*out_str,
 *								 *out_len))
 *					      == NULL) {
 *					*out_len = 0;
 *					return(D2_ERROR);
 *				}
 *			}
 *		}
 *	} 
 * }
 * else {
 * 	outbytesleft = *out_len;
 * 	if (iconv(t61_local_cd, &inp_str, &inp_len, &out_str, &outbytesleft)
 *		     != (size_t) -1) {
 * 		*out_len -= outbytesleft; 
 *		return(D2_NOERROR);
 *	}
 *	else {
 *		*out_len = 0;
 *		return(D2_ERROR);
 *	}
 * }
 */

	d2_ret_val return_code = 0;
	unsigned32 len;

	if (*out_len == D2_NOERROR) {
		/*
		 * no memory area was provided
		 */
		len = *out_len = inp_len * D27_T61_CONV_FACT;		
		if ((*out_str = (char *)malloc( *out_len)) == NULL) {
			return_code = D2_ERROR;
			goto end;
		} /* endif */

		if (OmCharT61Iso(inp_str, inp_len, *out_str, (int *) out_len) != 0) {
			free(*out_str);
			*out_str = NULL;
			*out_len = 0;
			return_code = D2_ERROR;
			goto end;
		} /* endif */

		if (*out_len < len) {
			*out_str = (char *)realloc(*out_str, *out_len);
			if (*out_str == NULL) {
				*out_len = 0;
				return_code = D2_ERROR;
				goto end;
			} /* endif */
		} /* endif */
	} else {
		return_code = (OmCharT61Iso(inp_str, inp_len, *out_str, (int *) out_len));
	} /* endif */
end:
	return (return_code);
} /* end d27_t61_to_local */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d27_t61_to_local_code()	           */
/* Description         : This routine converts the string "inp_str"*/
/*			 (of size "inp_len") to the output string  */
/*			 "*out_str" (size is "*out_len").          */
/*			 Note: *out_len (as input parameter) holds */
/*			       the maximum length of the buffer    */
/*			       area where the converted string     */
/*			       should be generated.                */
/*			       As output parameter it contains the */
/*			       actual length of the generated      */
/*			       output string.			   */
/* Return Value     : D2_NOERROR:         conversion successful          */
/*			 otherwise: conversion failed              */
/* Author              : Wolfgang Schmid, SNI BU BA NM 123         */
/*                                                                 */
/* NOTE: This routine will retry the conversion if conversion      */
/*       failed due to some incomplete character at the end of the */
/*       input string. This behaviour is useful, if the input      */           /*       string has been truncated (as it happens in gdsditadm).   */ 
/*                                                                 */
/*-----------------------------------------------------------------*/

d2_ret_val d27_t61_to_local_code(const char *inp_str, unsigned32 inp_len, 
			       char *out_str, unsigned32 *out_len) 
{
/*
 * The following piece of code should be used when the "private" conversion
 * routines are replaced by "ICONV":
 *
 * size_t      inbytesleft;
 * size_t      outbytesleft;
 * const char *inbuf;
 * char       *outbuf;
 * size_t      rc; 
 * 
 * do {
 *	inbytesleft = inp_len;	
 *	outbytesleft = *out_len;
 *	inbuf = inp_str;
 *	outbuf = out_str;
 *      if ((rc = iconv(t61_local_cd, &inbuf, &inbytesleft, &outbuf, 
 *			&outbytesleft) == (size_t) -1) {
 * 		if (errno == EINVAL) {
 *				incomplete character at the end of the input
 *				buffer 
 *				--> try to convert again (with incomplete 
 *				    character dropped at the end)
 *			inp_len--;
 *		} 
 *		else {
 *			return(D2_ERROR); 
 *		}
 *      }
 *      else {
 *      	*out_len -= outbytesleft; 
 *	} 
 * } while (rc == (size_t) -1); 
 *
 * return(D2_NOERROR);
 */

	d2_ret_val return_code = D2_NOERROR;
	unsigned32 no_inv_char;
	unsigned32 prev_no_inv_char;
	unsigned32 no_part_char;

	/* no. of chars to be stripped from the end in case of error. */
	no_part_char = D27_T61_CONV_FACT - 1;
	prev_no_inv_char = 0;

	while ((no_inv_char = OmCharT61Iso(inp_str, inp_len, out_str, (int *) out_len)) != 0) {
		/* strip partial chars. from the end of the string. */
			
		/* char had not been stripped the first time  */
		/* and stripping didn't help.		      */
		if (prev_no_inv_char != 0 && no_inv_char != prev_no_inv_char - 1) {
			OmCharT61Iso(inp_str, inp_len + 1, out_str, (int *) out_len);
			return_code = D2_ERROR;
			goto end;
		} /* endif */

		/* all partial chars have been handled.	*/		
		if (no_part_char == 0) {
			return_code = D2_ERROR;
			goto end;
		} /* endif */

		/* first time char. is stripped or stripping */
		/* is helping and still some partial chars   */
		/* could be present.			     */
		prev_no_inv_char = no_inv_char;
		inp_len--;
		no_part_char--;	
	} /* endwhile */

end:
	return (return_code);
} /* end d27_t61_to_local_code */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d27_local_to_t61()		           */
/* Description         : This routine converts the string "inp_str"*/
/*			 (of size "inp_len") to the output string  */
/*			 "*out_str" (size is "*out_len").          */
/*			 Note: *out_len (as input parameter) holds */
/*			       the maximum length of the buffer    */
/*			       area where the converted string     */
/*			       should be generated. If in this case*/
/*			       *out_len is 0, no memory area was   */
/*			       allocated up so far; this routine   */ 
/*			       will allocate the memory area.      */
/*			       As output parameter it contains the */
/*			       actual length of the generated      */
/*			       output string.			   */
/* Return Value        : D2_NOERROR:		conversion successful      */
/*			 otherwise:	conversion failed	   */
/* Author              : Wolfgang Schmid, SNI BU BA NM 123         */
/*                                                                 */
/*-----------------------------------------------------------------*/

d2_ret_val d27_local_to_t61(const char *inp_str, const unsigned32 inp_len, 
			  char **out_str, unsigned32 *out_len) 
{
/*
 * The following piece of code should be used when the "private" conversion
 * routines are replaced by "ICONV":
 *
 * unsigned32 outbytesleft, len;
 *
 * if (*out_len == 0) {
 *	memory are has not been allocated so far; try to allocate the
 *	memory for the converted string and in case is too little memory, then
 *	realloc
 *
 * 	len = *out_len = inp_len * D27_T61_CONV_FACT;		
 *	if ((*out_str = malloc(*out_len)) == NULL) {
 *		return (D2_ERROR);
 *	}
 *
 *	for ( ; ; ) {
 * 		outbytesleft = *out_len;
 * 		if (iconv(local_t61_cd, &inp_str, &inp_len, &out_str, 
 *			  &outbytesleft) != (size_t) -1) {
 *	 		*out_len -= outbytesleft; 
 *			if (*out_len < len) {
 *				*out_str = realloc(*out_str, *out_len);
 *				if (*out_str == NULL) {
 *					*out_len = 0;
 *					return(D2_ERROR);
 *				}
 *			} 
 *			return(D2_NOERROR);
 *		}
 *		else {
 *			if (errno == E2BIG) {
 *				free(*out_str);
 *				*out_len = 0;
 *				return(D2_ERROR);
 *			}
 *			else {
 *				*out_len += inp_len;
 *				len = *out_len;
 *				if ((*out_str = realloc(*out_str,
 *								 *out_len))
 *					      == NULL) {
 *					*out_len = 0;
 *					return(D2_ERROR);
 *				}
 *			}
 *		}
 *	} 
 * }
 * else {
 * 	outbytesleft = *out_len;
 * 	if (iconv(local_t61_cd, &inp_str, &inp_len, &out_str, &outbytesleft)
 *		     != (size_t) -1) {
 * 		*out_len -= outbytesleft; 
 *		return(D2_NOERROR);
 *	}
 *	else {
 *		*out_len = 0;
 *		return(D2_ERROR);
 *	}
 * }
 */

	d2_ret_val return_code = D2_NOERROR;
	unsigned32 len;

	if (*out_len == 0) {
		/*
		 * no memory area was provided
		 */
		len = *out_len = inp_len * D27_T61_CONV_FACT;		
		if ((*out_str = (char *)malloc(*out_len)) == NULL) {
			return_code = D2_ERROR;
			goto end;
		} /* endif */
		if (OmCharIsoT61(inp_str, inp_len, *out_str, (int *) out_len) != 0) {
			free(*out_str);
			*out_str = NULL;
			*out_len = 0;
			return_code = D2_ERROR;
			goto end;
		} /* endif */
		if (*out_len < len) {
			*out_str = (char *)realloc(*out_str, *out_len);
			if (*out_str == NULL) {
				*out_len = 0;
				return_code = D2_ERROR;
				goto end;
			} /* endif */
		} /* endif */
	} else {
		if( OmCharIsoT61(inp_str, inp_len, *out_str, (int *) out_len) != 0 )
		    return_code = D2_ERROR;
		else
		    return_code = D2_NOERROR;
	} /* endif */
end:
	return (return_code);
} /* end d27_local_to_t61 */
