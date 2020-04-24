/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: load_blank.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:43  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:09  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:53  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:02  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:26:26  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:54:24  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:40:49  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:00:54  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:36:13  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:52:46  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:18:50  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:19:14  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: load_blank.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:51 $";
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

#include <stdio.h>
#include <dce/asn1.h>

FILE * asn1_in;
FILE * asn1_er;
int asn1_ln;
unsigned asn1_zl;
asn1 asn1_zz;
char * asn1_fn;


/******************\
* 		   *
* asn1_load_blank  *
* 		   *
\******************/
int asn1_load_blank()
{	int c;

	for(;;){
		c = getc(asn1_in);
		if (c == '-')
			c = asn1_load_comment();
		if (c == '\n'){
			asn1_ln++; continue;}
		if (c == ' ' || c == '\t' || c == '\r')
			continue;
		return(c);
	}
}

asn1_load_comment()
{	int c;

	c = getc(asn1_in);
	if (c != '-'){
		if(ungetc(c,asn1_in) == EOF)
			return(EOF);
		else	return('-');
	}
	for (;;){
		c = getc(asn1_in);
		if (c == '-'){
			c = getc(asn1_in);
			if (c == '-'){
				return(getc(asn1_in));
			}
		}
		if (c == '\n' || c == EOF) return(c);
	}
}
	
/***********************\
* 		        *
* asn1_load_mess(s)     *
* asn1_load_error(x,s)  *
* 		        *
\***********************/
asn1_load_mess(s)
char * s;
{
	if (asn1_er)
		(void) fprintf(asn1_er,"\"%s\", line %d: %s\n",
		asn1_fn, asn1_ln, s);
	return;
}

asn1_load_error(x,s)
char x, *s;
{	int c;

	asn1_load_mess(s);
	for(;;) {
		c = getc(asn1_in);
		if (c == '\n'){
			asn1_ln++; return;}
		if (c == EOF || c == x) return;
	}
}
