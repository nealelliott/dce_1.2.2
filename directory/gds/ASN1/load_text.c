/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: load_text.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:44  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:11  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:53  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:05  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:26:39  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:54:33  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:41:35  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:01:04  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:36:18  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:52:49  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:18:57  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:19:19  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: load_text.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:52 $";
#endif

 
/* ****************************************************************** */
/*                                                                    */
/*   COPYRIGHT  (C)  1990 BY SIEMENS NIXDORF INFORMATIONSSYSTEME AG   */
/*                           AND INRIA                                */
/*   All rights reserved                                              */
/*                                                                    */
/*   This software is furnished under licence and may be used only    */
/*   in accordance with the terms of that licence and with the        */
/*   inclusion of the above copyright notice.                         */
/*   This software may not be provided or otherwise made available    */
/*   to, or used by, any other person. No title to or ownership of    */
/*   the software is hereby  transferred.                             */
/*                                                                    */
/* ****************************************************************** */

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <ctype.h>
#include <stdio.h>
#include <dce/asn1.h>

static FILE * asn1_in;
static FILE * asn1_er;
static int asn1_ln;
static unsigned asn1_text_area_len;
static char * asn1_text;
static char * asn1_fn;

/**************************************************************************\
* 									   *
* Input a text printout from a text file.				   *
* 									   *
* There are two kinds of texts: those enclosed between brackets and those  *
* which are not. The first type is terminated by the closing bracket. The  *
* secund type is terminated by a semi colon.				   *
* 									   *
* The most important difficulty is a proper handling of quoted strings.	   *
* Quoted strings can contain any kinds of characters, including quoted	   *
* quotes. For safety sake, they cannot contain a line feed.		   *
* 									   *
\**************************************************************************/

static int asn1_load_text_item (int, int);
static int asn1_load_text_until_quote (int);
static int asn1_text_memory (int);
static void asn1_load_text_error (int);

int asn1_load_text(zz, zm, in, err, fname, line)
char ** zz, ** zm, *fname;
FILE * in, * err;
int line;
{	int first_char, nb_of_chars;

	asn1_in = in;
	asn1_er = err;
	if ((asn1_fn = fname) == 0) asn1_fn = "-";
	asn1_ln = line;

	for (;;){
		if ((first_char = getc(asn1_in)) == EOF)
			return(-1);
		if (first_char == '\n') asn1_ln++;
		if (!isspace(first_char)) break;
	}
	asn1_text = malloc(asn1_text_area_len = 1024);
	asn1_text[0] = first_char;
	nb_of_chars = asn1_load_text_item(1,first_char == '<');
	asn1_text[nb_of_chars++] = 0;
	*zz = realloc(asn1_text,(unsigned)nb_of_chars);
	*zm = *zz + nb_of_chars -1;
	return (asn1_ln);
}

static asn1_load_text_item(nb_of_chars, waiting_for_bracket)
int nb_of_chars, waiting_for_bracket;
{	int c;

	for (;;){
		if ((c = getc(asn1_in)) == EOF){
			if(waiting_for_bracket){
				asn1_load_text_error(0);
				return(-1);
			}else	return(nb_of_chars);
		}
		if (asn1_text_memory(nb_of_chars)) return(-1);
		asn1_text[nb_of_chars++] = c;
		switch(c){
		case '<':
			nb_of_chars =
			asn1_load_text_item(nb_of_chars,1);
			break;
		case '>':
			if (waiting_for_bracket)
				return(nb_of_chars);
		case '"':
			nb_of_chars =
			asn1_load_text_until_quote(nb_of_chars);
			break;
		case '\n':
			asn1_ln++;
		case ';':
			if (waiting_for_bracket == 0)
				return(nb_of_chars);			
		default:
			continue;
		}
		if (nb_of_chars <0) return(nb_of_chars);
	}		
}

static int asn1_load_text_until_quote(nb_of_chars)
int nb_of_chars;
{	int c;

	for (;;){
		if ((c = getc(asn1_in)) == EOF){
			asn1_load_text_error(1);
			return(-1);
		}
		if (asn1_text_memory(nb_of_chars)) return(-1);
		asn1_text[nb_of_chars++] = c;
		switch(c){
		case '"':
			return(nb_of_chars);
		case '\n':
			asn1_load_text_error(1);
			return(-1);
		default:
			continue;
		}
	}		
}

static int asn1_text_memory(nb_of_chars)
int nb_of_chars;
{
	if (asn1_text_area_len <= nb_of_chars){
		asn1_text_area_len += 1024;
		if ((asn1_text = realloc(asn1_text,asn1_text_area_len))==0){
			asn1_load_text_error(3);
			return(-1);
		}
	}
	return(0);
}

static char * asn1_load_text_error_value[] = {
"unexpected end of file",
"end of file within quoted string",
"end of line within quoted string",
"not enough memory"};

static void asn1_load_text_error(i)
int i;
{
	fprintf(asn1_er,"\"%s\", line %d: %s\n",
	asn1_fn, asn1_ln,asn1_load_text_error_value[i]);
	if (asn1_text) free(asn1_text);
}


