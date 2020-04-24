/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: input.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:30  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:42  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:44  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:41  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:23:45  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:53:03  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:30:39  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:59:22  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:35:00  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:52:02  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:17:09  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:18:20  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: input.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:40 $";
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
#include <dce/asn1.h>

/*************************************************************************\
* 									  *
* Formating control.							  *
* 									  *
* We have to manage an indentation system, so that each new line begins   *
* with a indentation count. This is done by maintaining an "indentation	  *
* counter", which is: 							  *
* 									  *
* . Increased when ``opening a bracket'', 				  *
* 									  *
* . Decreased when ``closing a bracket'', 				  *
* 									  *
* . Applied when ``separating two components''.				  *
* 									  *
\*************************************************************************/

int asn1_output_indentation = 0;
int asn1_output_indent_len = 2;
char * asn1_output_indent_value = "\t";

char * asn1_output_indent(z,c)
register char * z;
int c;
{	register i;
	register char * y;

	if (c) *z++ = c; 
	*z++ = '\n';
	for (i=0; i<asn1_output_indentation; i++)
		for(y = asn1_output_indent_value; *y; *z++ = *y++);
	return(z);
}

void asn1_output_indent_reset (void)
{
	asn1_output_indentation = 0;
	asn1_output_indent_len = 2;
}

char * asn1_output_open_bracket(asn1z)
char * asn1z;
{
	*asn1z++ = ASN1_OPENING_BRACKET;
	if (asn1_output_indentation && asn1_count_size_of_line(asn1z) > 7)
		asn1z = asn1_output_indent(asn1z,0);
	*asn1z++ = '\t';
	asn1_output_indentation++;
	return(asn1z);
}

int asn1_count_size_of_line(asn1z)
register char * asn1z;
{
	register n_char = 0, i, max_char = 80, n_tab = 0;

	asn1z--;
	do {	if (*asn1z == '\t'){
			n_char += 8;
			n_tab++;
			i = n_char + 8*(asn1_output_indentation-n_tab);
			if (max_char > i)
				max_char = i;
			if (n_char < max_char){
				for (i=0;i<8;i++){
					asn1z--;
					if (*asn1z < 32) break;
				}
			}
		}else if (*asn1z == '\n') break;
		else{	n_char++;
			asn1z--;
		}
	}while (n_char < max_char);
	n_char -= 8*asn1_output_indentation;
	return((n_char>0)?n_char:0);
}

/**************************************************************\
* 							       *
* asn1_inside_bracket(asn1z)				       *
* 							       *
* Returns TRUE if we have not yet reached the last component.  *
* 							       *
\**************************************************************/
int asn1_inside_bracket(asn1z)
char * asn1z;
{
	return(asn1z && ((*asn1z)!= ASN1_CLOSING_BRACKET));
}

/********************************\
* 				 *
* asn1_compare_name(asn1z,name)	 *
* 				 *
\********************************/
int asn1_compare_name(asn1z,name)
char * asn1z, *name;
{	register lname = strlen(name);

	return (asn1z != 0 && strncmp(asn1z,name,lname) == 0
	&& !isalnum(asn1z[lname]));
}

char * asn1_skip_if_match(asn1z, asn1zm, name)
char * asn1z, *asn1zm, *name;
{	register l = strlen(name);

	if (asn1z != 0 
	&& (asn1zm - asn1z) >= l
	&& strncmp(asn1z,name,l) == 0)
		return(asn1_skip_equal_sign(asn1z+l,asn1zm));
	else	return(asn1z);

}

/***********************\
* 		        *
* asn1_skip_equal_sign  *
* 		        *
\***********************/
char * asn1_skip_input_spaces(asn1z,asn1zm)
char * asn1z, * asn1zm;
{
	if (asn1z == 0)
		return(0);
	while ((*asn1z != 0) && (asn1z < asn1zm) && isspace(*asn1z)) asn1z++;
	return(asn1z);
}

char * asn1_skip_equal_sign(asn1z,asn1zm)
char * asn1z, * asn1zm;
{
	if ((asn1z = asn1_skip_input_spaces(asn1z,asn1zm))
	&& (*asn1z) && (asn1z < asn1zm) && (*asn1z == ASN1_EQUAL_CHAR))
		asn1z = asn1_skip_input_spaces(asn1z+1,asn1zm);
	else if (asn1z)
		asn1z = ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_EQUAL);
	return(asn1z);
}

/***************************************************************************\
* 									    *
* asn1_skip_input:							    *
* 									    *
* Skip a complete declaration. The declaration is composed of either a	    *
* value, or a name, an equal sign and a value. The value itself can be	    *
* either a simple value (string) or a set of declarations within brackets.  *
* 									    *
\***************************************************************************/
char * asn1_skip_input_and_count(asn1z,asn1zm,n)
char * asn1z, *asn1zm;
int *n;
{
	if (n) *n = 0;
	if (asn1z == 0 || asn1z >= asn1zm)
		return(0);

	asn1z = asn1_skip_input_spaces(asn1z,asn1zm);
	if (isalpha(*asn1z)){
		do asn1z++; 
		while (isalnum(*asn1z) || (*asn1z == '-'));

		asn1z = asn1_skip_input_spaces(asn1z,asn1zm);

		if (asn1z <asn1zm && *asn1z == ASN1_EQUAL_CHAR)
			asn1z = asn1_skip_input_spaces(asn1z+1,asn1zm);
	}

	if (*asn1z == ASN1_OPENING_BRACKET){
		asn1z = asn1_open_input(asn1z,asn1zm);
		while (asn1_inside_bracket(asn1z)){
			if (n) (*n)++;
			asn1z = 
			asn1_skip_input_and_count(asn1z,asn1zm,(int*)0);
		}
		asn1z = asn1_close_input(asn1z,asn1zm);
	}else{
		if (n) *n = 1;
		asn1z = asn1_skip_input_string(asn1z,asn1zm);
	}
	return(asn1_skip_input_separator(asn1z,asn1zm));
}

char * asn1_skip_input_separator(asn1z,asn1zm)
char * asn1z, *asn1zm;
{
	asn1z = asn1_skip_input_spaces(asn1z,asn1zm);
	if (asn1z && *asn1z == ASN1_SEPARATOR)
		return(asn1_skip_input_spaces(asn1z+1,asn1zm));
	else	return(asn1z);
}


char * asn1_skip_input(asn1z,asn1zm)
char * asn1z, *asn1zm;
{
	return(asn1_skip_input_and_count(asn1z,asn1zm,(int *)0));
}



int asn1_input_number(asn1z,asn1zm)
char * asn1z, *asn1zm;
{
	int n;

	(void) asn1_skip_input_and_count(asn1z,asn1zm,&n);
	return((n>0)?n:1);
}

/**********************\
* 		       *
* Skipping of strings  *
* 		       *
\**********************/

char * asn1_skip_input_string(asn1z,asn1zm)
char * asn1z, *asn1zm;
{	if (asn1z)
	while (asn1z < asn1zm && *asn1z != ASN1_CLOSING_BRACKET && *asn1z != ASN1_SEPARATOR){
		switch(*asn1z){
		case '\'':
			asn1z =
			asn1_skip_input_hexa_string(asn1z+1,asn1zm);
			break;
		case '"':
			asn1z =
			asn1_skip_input_quoted_string(asn1z+1,asn1zm);
			break;
		default:
			asn1z++;
			break;
		}
	}
	return(asn1z);
}

char * asn1_skip_input_quoted_string(asn1z,asn1zm)
register char * asn1z, *asn1zm;
{
	for (asn1z++;asn1z <asn1zm && *asn1z != '\"'; asn1z++)
		if (*asn1z == '\\') asn1z++;
	return((asn1z<asn1zm)?asn1z+1:0);
}

char * asn1_skip_input_hexa_string(asn1z,asn1zm)
register char * asn1z, *asn1zm;
{
	for (asn1z++;asn1z <asn1zm && *asn1z != '\''; asn1z++);
	return((asn1z<asn1zm)?asn1z+1:0);
}

int asn1_input_size(asn1z,asn1zm)
register char * asn1z, *asn1zm;
{	char * z;
	int l;

	if (asn1z != 0 && asn1z < asn1zm){
		if (z = asn1_skip_input_string(asn1z,asn1zm)){
			if (l = z - asn1z)
				return(l);
			else	return(1);
		}else	return(asn1zm - asn1z);
	}else	return(1);
}

/***************************************************************************\
* 									    *
* Opening and closing of sequences:					    *
* 									    *
* Sequences are supposed to start with an "open_bracket" character.	    *
* The "open sequence" routine places the current pointer after the first    *
* non blank character that follows the opening bracket.			    *
* 									    *
* The "close sequence" routines places the current pointer after the first  *
* non blank character that follows the closing bracket.			    *
* 									    *
\***************************************************************************/
char * asn1_open_input(asn1z,asn1zm)
char * asn1z, * asn1zm;
{
	if (asn1z = asn1_skip_input_spaces(asn1z,asn1zm)){
		if (*asn1z != ASN1_OPENING_BRACKET)
			return(ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_OBRACKET));
		return(asn1_skip_input_spaces(asn1z+1,asn1zm));
	}
	return(0);
}

char * asn1_close_input(asn1z,asn1zm)
char * asn1z, * asn1zm;
{
	for (;;asn1z++){
		if (asn1z == 0 || asn1z >= asn1zm || *asn1z == 0)
			return(0);
		if (*asn1z == ASN1_CLOSING_BRACKET)
			return(asn1_skip_input_spaces(asn1z+1,asn1zm));
	}
}

/******************************************\
* 					   *
* Hexadecimal format.			   *
* 					   *
* Used for bit strings and octet strings.  *
* 					   *
\******************************************/

char * asn1_hexa_out(asn1z,v,l)
char * asn1z;
asn1 v;
int l;
{	register int c, i;

	*asn1z++ = '\'';
	for (i=0;i<l;i++){
		c = (v[i] >> 4)&0x0F;
		*asn1z++ = ((c < 10)?'0':'A'-10)+c;
		c = v[i]&0x0F;
		*asn1z++ = ((c < 10)?'0':'A'-10)+c;
	}
	*asn1z++ = '\'';
	*asn1z++ = 'H';
	return(asn1z);
}

int asn1_hexa_digit_in(char_in, number_out, could_be_binary)
int char_in, *number_out, *could_be_binary;
{
	if (char_in == '0' || char_in == '1'){
		*number_out = char_in - '0'; return(0);
	}
	*could_be_binary = 0;
	if (char_in > '1' && char_in <= '9'){
		*number_out = char_in - '0'; return(0);
	}
	if (char_in >= 'A' && char_in <= 'F'){
		*number_out = char_in - 'A' +10; return(0);
	}
	if (char_in >= 'a' && char_in <= 'f'){
		*number_out = char_in - 'a' +10; return(0);
	}
	return(-1);
}

char * asn1_hexa_in(asn1z, asn1zm, v, l, could_be_binary, odd_number)
char * asn1z, * asn1zm;
asn1 *v;
int *l, *could_be_binary, *odd_number;
{	int number_out;

	if (asn1z == 0) return(0);
	*could_be_binary = 1;
	*odd_number = 0;
	for (;;){
		if (asn1z >= asn1zm || *asn1z == 0)
			return(ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_ST_END));

		if (*asn1z == '\'') return(asn1z+1);

		if (asn1_hexa_digit_in((int)*asn1z, 
		&number_out, could_be_binary))
			return(ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_HEXA));
		asn1z++;
		(*v)[*l] = number_out<<4;

		if (asn1z >= asn1zm || *asn1z == 0)
			return(ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_ST_END));

		if (*asn1z == '\''){
			(*l)++;
			*odd_number = 1;
			return(asn1z+1);
		}

		if (asn1_hexa_digit_in((int)*asn1z, 
		&number_out, could_be_binary))
			return(ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_HEXA));

		(*v)[*l] |= number_out;
		(*asn1z)++;
		(*l)++;
		if (*l % 1024){
			if ((*v = (asn1)asn1_realloc(
			(char *) *v,(unsigned)(*l + 1024)))==0)
			return(ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_MALLOC));
		}
	}
}

char * asn1_input_qstring(asn1z, asn1zm, v,l)
char * asn1z, * asn1zm;
asn1  *v;
int *l;
{
	for (;;){
		if (asn1z >= asn1zm || *asn1z == 0)
			return(ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_ST_END));
		switch (*asn1z){
		case '"':	return(asn1z+1);
		case '\n':
			return(ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_ST_EOL));
		case '\\':
			asn1z++;
			if (asn1z >= asn1zm || *asn1z == 0)
			return(ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_ST_END));
			switch (*asn1z){
			case 'n':	(*v)[*l] = '\n'; break;
			case 't':	(*v)[*l] = '\t'; break;
			case 'r':	(*v)[*l] = '\r'; break;
			default:	(*v)[*l] = *asn1z;
			}
			asn1z++;
			break;
		default:
			(*v)[*l] = *asn1z++;
			break;
		}
		(*l)++;
		if (((*l)%1024) == 0){
			if ((*v = (asn1)asn1_realloc((char *)*v,
			(unsigned)(*l + 1024)))==0)
			return(ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_MALLOC));
		}
	}

}


/***************************************************************************\
* 									    *
* Input and output routines for bits strings.				    *
* 									    *
* We will accept three formats:						    *
* 									    *
* * either a fully binary format:					    *
* 	'bbbbbbbbb'B							    *
*   which can be used to enter special strings with arbitrary bit numbers.  *
*   This format will always be choosen for "short" strings (<32 bits).	    *
* 									    *
* * or an hexadecimal format:						    *
* 	'xxxxxxxxxxxxxxx'H						    *
*   which can be used when the bit string contains 8*N bits,		    *
* 									    *
* * or a mixture of:							    *
* 	n * hexadecimal strings +					    *
* 	an optional terminal bit string.				    *
* This last format will be choosen for "long" strings. Each of the	    *
* initial hexa strings will contain at most 32 bytes; the terminal bit	    *
* string will contain at most 64 bits.					    *
* 									    *
* The same format can be used for ``flags''.				    *
* 									    *
\***************************************************************************/

char * asn1_bits_in(asn1z,asn1zm,v,l)
char * asn1z, *asn1zm;
asn1 *v;
int * l;
{	asn1 hexa_string;
	int hexa_len, could_be_binary, odd_number, bit_number, this_bit;
	int i,j,k;
	unsigned v_len = 0;

	*l = 0;
	if((*v = (asn1) asn1_malloc((unsigned)1024)) == 0
	||(hexa_string = (asn1) asn1_malloc((unsigned)1024)) == 0)
		return(ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_MALLOC));

	while (asn1z && asn1z < asn1zm && *asn1z){
		asn1z = asn1_skip_input_spaces(asn1z,asn1zm);
		if (asn1z == 0 || *asn1z != '\'')
			goto clean_before_exit;
		hexa_len = 0;
		if ((asn1z = 
		asn1_hexa_in(asn1z+1, asn1zm, &hexa_string, &hexa_len,
		&could_be_binary, &odd_number))== 0)
			goto clean_before_exit;

		switch (*asn1z){
		case 'H':
			asn1z++;
			if ((*v =(asn1)asn1_realloc((char *)*v,
			v_len+hexa_len+1))==0){
			asn1z = (ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_MALLOC));
			goto clean_before_exit;
			}
			if ((i=(*l & 0x07)) == 0){
				ASN1_MOVE(hexa_string,*v + v_len, hexa_len);
				v_len += hexa_len;
			}else{
				for (j=0; j<hexa_len; j++, v_len++){
					(*v)[v_len -1] |= hexa_string[j]>>i;
					(*v)[v_len] = hexa_string[j]<<(8-i);
				}
				if (odd_number && (i <= 4)) v_len--;
			}
			*l += 8*hexa_len - 4*odd_number;
			break;
		case 'B':
			asn1z++;
			if (could_be_binary == 0){
			asn1z = (ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_BSTRING));
			goto clean_before_exit;
			}
			bit_number = 2*hexa_len - odd_number;
			v_len = (*l + bit_number + 7)>>3;
			
			if ((*v = (asn1)asn1_realloc((char *)*v, v_len))==0){
			asn1z = (ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_MALLOC));
			goto clean_before_exit;
			}
			for (i=0, j = (*l&7), k = (*l)>>3; 
			i<bit_number; i++){
				this_bit = hexa_string[i>>1];
				if (i&1)
					this_bit &= 1;
				else	this_bit >>= 4;

				(*v)[k] |= (this_bit)<<(7-j);
				if (j == 7){
					j=0; k++; (*v)[k]=0;
				}else	j++;
			}
			*l += bit_number;
			break;
		default:
			asn1z = (ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_B_OR_H));
			goto clean_before_exit;
		}
	}
clean_before_exit:
	asn1_free((char *) hexa_string);
	return(asn1z);
}

char * asn1_binary_out(asn1z,v,l)
register char * asn1z;
register asn1 v;
int l;
{	register int c, j;

	*asn1z++ = '\'';
	while (l >0){
		c = *v++;
		for (j = 0; (j<8 && l); j++, l--, c<<=1)
			*asn1z++ = (c&0x80)?'1':'0';
	}
	*asn1z++ = '\''; *asn1z++='B';
	return(asn1z);
}

char * asn1_bits_out(asn1z,v,l)
char * asn1z;
asn1 v;
int l;
{	int i, n_char, max_char;

	n_char = asn1_count_size_of_line(asn1z);
	if ((max_char = (9 - asn1_output_indentation))>2)
		max_char *= 8;
	else	max_char = 16;
	if (n_char > max_char){
		asn1z = asn1_output_indent(asn1z,0);
		n_char = 0;
	}
	i = (max_char - n_char)>>1;
	while (l > 8*i){
		asn1z = asn1_hexa_out(asn1z,v,i);
		l -= 8*i;
		v += i;
		i = max_char>>1;
		if (l)
			asn1z = asn1_output_indent(asn1z,0);
	}

	if (l&0x07){
		i = max_char;
		while (l){
			if (l < i) i = l;
			asn1z = asn1_binary_out(asn1z, v, i);
			l -= i;
			v += (i>>3);
			if (l)
				asn1z = asn1_output_indent(asn1z,0);
		}
	}else{
		asn1z = asn1_hexa_out(asn1z,v, (l>>3));
	}
	return(asn1z);
}

/*ARGSUSED*/
int asn1_bits_olen(v,l)
asn1 v;
int l;
{
	return((l&0x07)?(l+10):((l>>3)+4));
}

char * asn1_flags_in(asn1z,asn1zm,v,l)
char * asn1z, *asn1zm;
int * v;
int l;
{
	asn1 bits_string;
	int nb_of_bits;

	*v = 0;
	if ((asn1z = asn1_bits_in(asn1z,asn1zm,&bits_string, &nb_of_bits))
	&& nb_of_bits){
		if (nb_of_bits > l) nb_of_bits = l;
		ASN1_MOVE((char*)bits_string,(char *)v,(nb_of_bits+7)>>3);
	}
	asn1_free((char *)bits_string);
	return(asn1z);	
}

char * asn1_flags_out(asn1z,v,l)
char * asn1z;
int v;
int l;
{
	return(asn1_bits_out(asn1z,(asn1)&v,l));
}

/****************************************************************************\
* 									     *
* Input and output routines for octet strings.				     *
* 									     *
* Octet strings will be displayed as a combination of three formats:	     *
* 									     *
* -either simple printable strings, without special characters,		     *
* 									     *
* -or quoted strings, using '\\' escapes,				     *
* 									     *
* -or hexa-decimal strings.						     *
* 									     *
* We can indeed use a combination of these formats. Note that we may have    *
* to print "long" octet strings, that dont fit on a single line. In this     *
* case, we have to break the value in sub-elements + apply the indentation   *
* to each of theses. The guessing of "how many elements" can take some 	     *
* time, together with alternatives like choice of the best splitting point,  *
* etc.									     *
* 									     *
* Character strings use the same routines as octet strings, with one major   *
* exception -- they need a terminating zero.				     *
* 									     *
\****************************************************************************/
char * asn1_octets_in(asn1z,asn1zm,v,l)
char * asn1z, *asn1zm;
asn1 * v;
int * l;
{	int could_be_binary, odd_number;

	if((*v = (asn1) asn1_malloc((unsigned)1024)) == 0)
		return(ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_MALLOC));
	*l = 0;

	while (asn1z && asn1z < asn1zm && *asn1z){
		if((asn1z = asn1_skip_input_spaces(asn1z,asn1zm))==0)
			goto clean_before_exit;

		switch(*asn1z){
		case '"':
			asn1z = asn1_input_qstring(asn1z+1, asn1zm,
				v,l);
			break;
		case '\'':
			if ((asn1z = 
			asn1_hexa_in(asn1z+1, asn1zm, v, l,
			&could_be_binary, &odd_number))== 0)
				goto clean_before_exit;
			if (asn1z >= asn1zm || *asn1z != 'H'){
			asn1z = ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_HSTRING);
			goto clean_before_exit;
			}
			asn1z++;
			break;
		default:
			if (!isalnum(*asn1z))
				goto clean_before_exit;
			{register int first_pass = *l;
			do {
				if (first_pass){
					(*v)[(*l)++] = ' ';
					first_pass = 0;
				}else	(*v)[(*l)++] = *asn1z++;
				if ((*l%1024) == 0){
				if((*v = (asn1)asn1_realloc
				((char *)*v, (unsigned)*l+1024))==0){
					asn1z=ASN1_ERR_IN(asn1z,
					asn1zm,ASN1_ERR_MALLOC);
					goto clean_before_exit;
				}}
			}while (isalnum(*asn1z) || *asn1z == '-'
			|| *asn1z == '.');
			}
			break;
		}
	}

clean_before_exit:
	if (*l && *v){
		if((*v = (asn1)asn1_realloc((char *)*v, (unsigned)*l))==0)
			asn1z=ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_MALLOC);
	}
	return(asn1z);
}

char * asn1_octets_out(asn1z,v,l)
char * asn1z;
asn1 v;
int l;
{	register int type_of_string=0, c, i, n_char=0, max_char;
	for (i=0; i<l; i++){
		c = v[i];
		if (isalnum(c)) continue;
		type_of_string = 1;
		if (c < 0x20){
			switch(c){
				case '\n':
				case '\t':
				case '\r': continue;
			default: break;
			}
		}else if (c < 0x7F) continue;
		type_of_string = 2;
		break;
	}
	n_char = asn1_count_size_of_line(asn1z);
	if ((max_char = (9 - asn1_output_indentation))>2)
		max_char *= 8;
	else	max_char = 16;
	if (n_char > max_char){
		asn1z = asn1_output_indent(asn1z,0);
		n_char = 0;
	}
	switch(type_of_string){
	case 0:
		while (l){
			i = max_char - n_char;
			if (i > l) i = l;
			n_char = 0;
			strncpy(asn1z,(char *)v,i);
			asn1z += i; v += i; l -= i;
			if (l)
				asn1z = asn1_output_indent(asn1z,0);
		}
		break;
	case 1:
		*asn1z++ = '"';
		for (i=0; i<l; i++){
			if (n_char > max_char){
				*asn1z++ = '"';
				asn1z = asn1_output_indent(asn1z,0);
				*asn1z++ = '"';
				n_char = 1;
			}else	n_char++;
			switch(v[i]){
			case '\n':
				*asn1z++ = '\\'; *asn1z++ = 'n';
				if (i == l) break;
				*asn1z++ = '"';
				asn1z = asn1_output_indent(asn1z,0);
				*asn1z++ = '"';
				n_char = 0;
				break;
			case '\t':
				n_char++;
				*asn1z++ = '\\'; *asn1z++ = 't'; break;
			case '\r':
				n_char++;
				*asn1z++ = '\\'; *asn1z++ = 'r'; break;
			default:
				*asn1z++ = v[i]; break;
			}
		}
		*asn1z++ = '"';
		break;
	case 2:
		while (l){
			i = (max_char - n_char)>>1;
			if (i > l) i = l;
			n_char = 0;
			asn1z = asn1_hexa_out(asn1z,v,i);
			v += i; l -= i;
			if (l)
				asn1z = asn1_output_indent(asn1z,0);
		}
		break;
	}
	*asn1z = 0;
	return(asn1z);
}

/*ARGSUSED*/
int asn1_octets_olen(v,l)
asn1 v;
int l;
{
	return(2*l + 3);
}

char * asn1_chars_in(asn1z,asn1zm,v)
char * asn1z, *asn1zm;
char **v;
{	int l;
	asn1 vv;

	if (asn1z = asn1_octets_in(asn1z,asn1zm,&vv,&l)){
		*v = (char *) asn1_realloc((char *)vv,(unsigned)(l+1));
		(*v)[l] = 0;
	}
	return(asn1z);
		
}

char * asn1_chars_out(asn1z,v)
char * asn1z;
char * v;
{
	return(asn1_octets_out(asn1z,(asn1)v,strlen(v)));
}

asn1_chars_olen(v)
char * v;
{
	return(asn1_octets_olen((asn1)v,strlen(v)));
}

/***************************************************************\
* 							        *
* Input and output routines for object identifiers.	        *
* This uses the ``object identifier'' routines. Beware of the   *
* initialisation of the OID tables!!!			        *
* 							        *
\***************************************************************/
char * asn1_oid_in(asn1z,asn1zm,v,l)
char * asn1z, *asn1zm;
asn1 *v;
int *l;
{	asn1_oid oid;
	char * s;
	int er;

	if (asn1z = asn1_chars_in(asn1z,asn1zm,&s)){
		oid = asn1_oid_copy(asn1_oid_get(s,&er));
		*v = oid.v; *l = oid.l;
		asn1_free(s);
	}
	return(asn1z);
}

char * asn1_oid_out(asn1z,v,l)
char * asn1z;
asn1 v;
int l;
{	asn1_oid oid;

	oid.v = v; oid.l = l;
	*asn1z++ = '"';
	asn1z += strlen(asn1_oid_put(asn1z,oid));
	*asn1z++ = '"';
	return(asn1z);
}

int asn1_oid_olen(v,l)
asn1 v;
int l;
{
	return(256);
}

/**************************************************************************\
* 									   *
* Miscellaneous routines for the handling of integer, booleans and nulls.  *
* 									   *
* For "int", a simple call to "sprintf";				   *
* For "boolean", a check for the "TRUE" and "FALSE" values;		   *
* For "NULL", a check for the "NULL" value.				   *
* 									   *
\**************************************************************************/
char * asn1_int_out(asn1z,l)
char * asn1z;
int l;
{
        sprintf(asn1z,"%d",l);
        return(asn1z + strlen(asn1z));

}

/*ARGSUSED*/
char * asn1_int_in(asn1z,asn1zm,l)
char * asn1z, * asn1zm;
int *l;
{	char * asn1z2;

	*l = strtol(asn1z,&asn1z2,0);
	return(asn1z2);
}

/*ARGSUSED*/
int asn1_int_v_in(asn1z,asn1zm,asn1z2)
char * asn1z, * asn1zm, ** asn1z2;
{	int l;

	l = strtol(asn1z,asn1z2,0);
	return(l);
}

char * asn1_boolean_in(asn1z,asn1zm,b)
char * asn1z, *asn1zm;
int * b;
{	register char * f = asn1_false, * t = asn1_true;

	if (asn1zm - asn1z >= 5 /* for FALSE */
	&&  asn1_compare_name(asn1z,f)){
		*b = 0;
		asn1z += strlen(f);
	}else if (asn1zm - asn1z >= 4 /* for TRUE */
	&&  asn1_compare_name(asn1z,t)){
		*b = 1;
		asn1z += strlen(t);
	}else{
		*b = 0;
		asn1z = ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_BOOL);
	}
	return(asn1z);
}


char * asn1_boolean_out(asn1z,b)
char * asn1z;
int b;
{
	return(asn1z + strlen(
	strcpy(asn1z,(b)?asn1_true:asn1_false)));
}

int asn1_boolean_v_in(asn1z,asn1zm,asn1z2)
char * asn1z, *asn1zm, ** asn1z2;
{	int b;

	*asn1z2 = asn1_boolean_in(asn1z,asn1zm,&b);
	return(b);
}

char * asn1_null_in(asn1z,asn1zm)
char * asn1z, *asn1zm;
{
	if (asn1zm - asn1z > 4
	&& asn1_compare_name(asn1z,"NULL")){
		asn1z += 4;
	}else{
		asn1z = ASN1_ERR_IN(asn1z,asn1zm,ASN1_ERR_NULL);
	}
	return(asn1z);
}


char * asn1_null_out(asn1z)
char * asn1z;
{
	strcpy(asn1z,"NULL");
	return(asn1z+4);
}
