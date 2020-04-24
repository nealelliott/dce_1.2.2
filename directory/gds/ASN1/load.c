/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: load.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:42  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:06  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:52  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:00  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:26:14  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:54:16  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:40:01  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:00:45  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:36:05  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:52:43  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:18:44  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:19:10  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: load.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:50 $";
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

/***********************************************************************\
* 								        *
* asn1_load:							        *
* 								        *
* Load in memory an ASN-1 text in the same format as that produced by   *
* ``asn1_dump''							        *
* 								        *
\***********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <dce/asn1.h>

static FILE * asn1_in;
static FILE * asn1_er;
static asn1_ln;
static unsigned asn1_zl;
static asn1 asn1_zz;
static char * asn1_fn;

asn1_load(zz,zm,in,err,fname,line)
FILE * in, * err;
char * fname;
int line;
asn1 * zz, * zm;
{	int i;

	asn1_in = in;
	asn1_er = err;
	if ((asn1_fn = fname) == 0) asn1_fn = "-";
	asn1_ln = line;
	if ((i = asn1_load_blank())==EOF)
		return(-1);
	else	(void) ungetc(i,asn1_in);
	asn1_zz = (asn1) malloc(asn1_zl = 1024);
	i = asn1_load_item(0);
	if (i == 0) return(-1);
	*zz = (asn1) realloc((char *)asn1_zz,(unsigned)i);
	*zm = *zz + i;
	return (asn1_ln);
}

/****************************************************************\
* 								 *
* Syntax:							 *
* 								 *
* item = tag | tag value | tag '{' itemlist '}' | tag '{' '}'	 *
* 	| tag * item -- this for ASN.1 encoded octet strings.	 *
* 								 *
* itemlist = item | itemlist ',' item				 *
* 								 *
* valuelist = value | value '&' valuelist			 *
* 								 *
* value = `"` ascii_text `"` | `\`` hexa_text "\`16"		 *
* 								 *
\****************************************************************/
asn1_load_item(i)
int i;
{	int l=0, i0, c;
	asn1 v;

	asn1_load_memory(i+256);
	/* get tag */
	if ((i = asn1_load_tag(i0 = i))< 0){
		asn1_load_mess("Ignoring that element");
		return(i0);
	}
	/* look for next char */
	c = asn1_load_blank();
	switch(c){
	case '*':
		i0 = ++i;
		i = asn1_load_item(i);

		if ((l = i-i0) < 127){
			asn1 x = (asn1)malloc((unsigned)l);

			ASN1_MOVE(asn1_zz+i0, x, l);
			i = asn1_lencod(asn1_zz + i0 -1,l) - asn1_zz;
			ASN1_MOVE(x, asn1_zz+i,l);
			i += l;
			free((char *)x);
		}else	asn1_zz[i0-1] = l;
		break;
	case '\'':
	case '"':
		asn1_load_value(&v,&l,c); goto load_value;
	case ',':
	case '}':
		(void)ungetc(c,asn1_in);
	case EOF: /* empty item */
	load_value:
		asn1_load_memory(i+l+4);
		i = asn1_lencod(asn1_zz+i,l) - asn1_zz;
		if (l){
			ASN1_MOVE(v,asn1_zz + i, l);
			i += l;
			free((char *)v);
		}
		break;
	case '{':
		asn1_zz[i0] |= 32;
		i0 = ++i;
		c = asn1_load_blank();
		if (c != '}'){
			(void)ungetc(c,asn1_in);
			for (;;) {
			i = asn1_load_item(i);
			c = asn1_load_blank();
			if (c == ',')
				continue;
			break;
		}}
		if (c != '}'){
			asn1_load_error('}',"} or , expected");
		}
		if ((i - i0) > 127){
			asn1_load_memory(i+2);
			asn1_zz[i0-1] = 0x80;
			asn1_zz[i++] = 0;
			asn1_zz[i++] = 0;
		}else	asn1_zz[i0-1] = i - i0;
		break;
	default:
		asn1_load_error('\n',"Value expected");
		asn1_zz[i++] = 0;
	}
	return(i);
}

void asn1_load_memory(l)
int l;
{
	if (asn1_zl < l){
		asn1_zl = l + 1024;
		asn1_zz = (asn1) asn1_realloc((char *)asn1_zz, asn1_zl);
	}
}

/****************\
* 		 *
* asn1_load_tag	 *
* 		 *
\****************/
asn1_load_tag(i)
int i;
{	int c, class = 2, number, l;
	unsigned char x[9];
	char classtag[256];

	c = asn1_load_blank();
	if (c != '['){
		asn1_load_error('\n',"Tag expected");
		return(-1);
	}
	c = asn1_load_blank(); (void)ungetc(c,asn1_in);
	if (fscanf(asn1_in,"%[A-Za-z]",classtag)){
		switch (classtag[0]){
		case 'a':
		case 'A': class = 1; break;
		case 'p':
		case 'P': class = 4; break;
		case 'u':
		case 'U': class = 0; break;
		default:
			asn1_load_mess("Incorrect class selector");
			return(-1);
		}
	}
	if (fscanf(asn1_in,"%d",&number) == 0){
		asn1_load_mess("Incorrect tag number");
		number = 0;
	}
	if (number < 31)
		asn1_zz[i++] = (class<<6) + number;
	else{	asn1_zz[i++] = (class<<6) + 31;
		for (l=0;number;number = number>>7)
			x[l++] = number&0x7F;
		do {
			l--;
			asn1_zz[i++] = x[l] |((l)?128:0);
		} while (l);
	}
	if (asn1_load_blank() == ']'){
		return(i);
	}else{	asn1_load_error(']',"Incorrect tag delimiter");
		return(-1);
	}
}

/******************\
* 		   *
* asn1_load_value  *
* 		   *
\******************/
void asn1_load_value(v,lv,c)
asn1 * v;
int * lv, c;
{	int l, i;
	unsigned lvm = 0;
	asn1 x, y;
	unsigned char buf[256];

	*lv = 0;
	for(;;){ 
		switch (c) {
		case '\'':
			if (fscanf(asn1_in,"%[ 0-9a-fA-F]'16",buf) == 0){
				asn1_load_mess("Incorrect hexa value");
				return;
			}
			for (x=buf;*x;x++) if (*x == ' ') *x = '0';
			x = y = buf;
			while (*y){
				if(*(y+1)){
					(void)sscanf((char *)y,
					"%2x",&i); y+= 2;
				}else{	(void)sscanf((char *)y,
					"%x",&i); y++;}
				*x++ = i;
			}
			l = x - buf;
			break;
		case '"':
			if (fscanf(asn1_in,"%[^\"]\"16",buf) == 0){
				asn1_load_mess("Incorrect ascii value");
				return;
			}
			l = strlen((char *)buf);
			break;
		default:
			asn1_load_mess("Incorrect value");
			return;
		}
		if (l){
			if (lvm == 0)
				*v = (asn1) malloc(lvm = 1024);
			else if (*lv + l > lvm){
				lvm = ((*lv+l)|1023)+1;
				*v = (asn1) realloc((char*)(*v),lvm);
			}
			ASN1_MOVE(buf,*v + *lv, l);
			*lv += l;
		}
		c = asn1_load_blank();
		if (c != '&'){
			(void)ungetc(c,asn1_in);
			return;
		}else	c = asn1_load_blank();
	}
}

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
void asn1_load_mess(s)
char * s;
{
	if (asn1_er)
		(void) fprintf(asn1_er,"\"%s\", line %d: %s\n",
		asn1_fn, asn1_ln, s);
	return;
}

void asn1_load_error(x,s)
int x;
char *s;
{	int c;

	asn1_load_mess(s);
	for(;;) {
		c = getc(asn1_in);
		if (c == '\n'){
			asn1_ln++; return;}
		if (c == EOF || c == x) return;
	}
}
