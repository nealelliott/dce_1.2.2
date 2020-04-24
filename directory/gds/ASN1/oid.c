/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: oid.c,v $
 * Revision 1.2.10.2  1996/02/18  19:37:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:51  marty]
 *
 * Revision 1.2.10.1  1995/12/08  15:07:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:25  root]
 * 
 * Revision 1.2.8.3  1994/08/01  10:13:57  marrek
 * 	Remove true, false and boolean.
 * 	[1994/07/28  16:11:25  marrek]
 * 
 * Revision 1.2.8.2  1994/05/10  15:48:59  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:18  marrek]
 * 
 * Revision 1.2.8.1  1994/02/22  15:27:51  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:55:28  marrek]
 * 
 * Revision 1.2.6.2  1993/08/11  11:46:18  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:02:02  marrek]
 * 
 * Revision 1.2.4.3  1992/12/31  16:36:47  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:10  bbelch]
 * 
 * Revision 1.2.4.2  1992/11/27  16:56:16  marrek
 * 	November 1992 code drop
 * 	[1992/11/27  16:52:59  marrek]
 * 
 * Revision 1.2.2.3  1992/06/01  20:19:38  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:19:46  zeliff]
 * 
 * Revision 1.2.2.2  1992/04/14  13:27:16  mishkin
 * 	     Don't use non-standard pthread_initroutine_t.
 * 	[1992/04/13  15:59:12  mishkin]
 * 
 * Revision 1.2  1992/01/19  22:13:02  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: oid.c,v $ $Revision: 1.2.10.2 $ $Date: 1996/02/18 19:37:59 $";
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

#include <string.h>
#include <dce/dce.h>
#include <dce/asn1.h>

struct asn1_oid_entry{
	char * s;
	asn1_oid oid;
	unsigned thash;
	int tnext;
	int st;
	int son;
};

#define MAXHASH	128
int asn1_oid_thash[MAXHASH];

int asn1_oid_nb = -1;
int asn1_oid_first = -1;
struct asn1_oid_entry * asn1_oid_table;
#ifdef ASN1_THREADED
#include <pthread.h>
ASN1_SEM_DECLARE(asn1_oid_sem);
static void asn1_oid_init(void);
#endif

#define MAX_BUF_LEN 512
static unsigned oid_hash (char *);
static int look_for_text (unsigned, char *);
static asn1 digits_oid (asn1, char *);
static oid_digits (asn1, int, char *);

/***********************************************************************\
* 								        *
* asn1_oid_decl(s,oid)						        *
* char * s;							        *
* asn1_oid oid;							        *
* 								        *
* Declares in the oid table the oid value ``oid'' as being equivalent   *
* to the textual representation ``s''.				        *
* 								        *
\***********************************************************************/
asn1_oid_decl(s,oid)
char * s;
asn1_oid oid;
{	int i, x, *xst;
	unsigned h;
	struct asn1_oid_entry * e;
	int retval;

#ifdef ASN1_THREADED
	if (asn1_oid_nb == -1){
                asn1_oid_init();
	}
#endif

#ifdef ASN1_THREADED
        ASN1_SEM_LOCK(&asn1_oid_sem);
#endif
	if (look_for_text(h = oid_hash(s),s) >= 0){
		retval = (ASN1_TEXT_EXISTS);
		goto retpoint;
	}
	if((x = look_for_oid(oid)) >= 0
	&& asn1_oid_table[x].oid.l == oid.l){
		retval = (ASN1_OID_EXISTS);
		goto retpoint;
	}

	if (asn1_oid_nb == -1){
		for (i=0;i<MAXHASH;i++) asn1_oid_thash[i] = -1;
		asn1_oid_table = (struct asn1_oid_entry *)
		asn1_malloc(128 * sizeof(struct asn1_oid_entry));
	}else if ((asn1_oid_nb&127) == 127){
		asn1_oid_table = (struct asn1_oid_entry *)
		asn1_realloc((char *)asn1_oid_table,(unsigned)(
		(asn1_oid_nb+129) * sizeof(struct asn1_oid_entry)));
	}


	e = &asn1_oid_table[++asn1_oid_nb];
	e->s = asn1_malloc((unsigned)(strlen(s)+1));
	strcpy(e->s,s);
	e->oid = asn1_oid_copy(oid);
	e->thash = h;
	i = asn1_oid_thash[h%MAXHASH];
	asn1_oid_thash[h%MAXHASH] = asn1_oid_nb;
	e->tnext = i;
	xst = (x >= 0)? &asn1_oid_table[x].son : &asn1_oid_first;
	i = *xst;
	*xst = asn1_oid_nb;
	e->st = i;
	e->son = -1;
	xst = &e->st;

	while (i >= 0){
		if (asn1_oid_table[i].oid.l > oid.l && 
		ASN1_CMP(asn1_oid_table[i].oid.v,oid.v,oid.l) == 0){
			*xst = asn1_oid_table[i].st;
			asn1_oid_table[i].st = e->son;
			e->son = i;
		}else	xst = &asn1_oid_table[i].st;
		i = *xst;
	}
	retval = 0;

retpoint:
#ifdef ASN1_THREADED
	ASN1_SEM_UNLOCK(&asn1_oid_sem);
#endif
	return(retval);
}

/************************************************************************\
* 									 *
* char * asn1_oid_xput(buffer,oid,mode)					 *
* char * buffer;							 *
* asn1_oid oid;								 *
* int mode;								 *
* 									 *
* Prints the object id ``oid'' in the buffer ``buffer''. Returns the	 *
* pointer to ``buffer''. The index mode can take three values:		 *
* 									 *
* 0- Print in the shortest possible form.				 *
* 1- Print in a context indfependant format.				 *
* 2- Print in a context independant format, adding comments.		 *
* 									 *
* asn1_oid_put(buffer,oid) is equivalent to asn1_oid_xput(buffer,oid,0)	 *
* 									 *
\************************************************************************/
char * asn1_oid_xput(s,oid,mode)
char * s;
asn1_oid oid;
int mode;
{	int i, j, max;
	char * s0 = s;

	if (mode == 0 && (i = look_for_oid(oid)) >= 0){
		max = asn1_oid_table[i].oid.l;
	 	strcpy (s, asn1_oid_table[i].s);
		if (max == oid.l) return (s0);
		s += strlen (s);
	}else{
		for (max=0, i=0; max < oid.l;){
			i = (i<<7) + (oid.v[max]&0x7F);
			if ((oid.v[max++]&0x80)==0) break;
		}
		if (i < 40){
			strcpy (s, ASN1_OID_CCITT); strcat(s," ");
			switch(i){
			case 0:
				strcat (s, ASN1_OID_RECOM); break;
			case 1:
				strcat (s, ASN1_OID_QUESTION); break;
			case 2:
				strcat (s, ASN1_OID_ADMIN); break;
			case 3:
				strcat (s, ASN1_OID_NET_OP); break;
			default:
				sprintf(s+strlen(s),"%d",i);
			}
		}else if (i < 80) {
			i -= 40;
			strcpy(s, ASN1_OID_ISO); strcat(s," ");
			switch (i) {
			case 0:
				strcat (s, ASN1_OID_STANDARD); break;
			case 1:
				strcat (s, ASN1_OID_REG_AUT); break;
			case 2:
				strcat (s, ASN1_OID_MEMBER); break;
			case 3:
				strcat (s, ASN1_OID_ID_ORG); break;
			default:
				sprintf(s+strlen(s),"%d",i);
			}
		}else {
			sprintf(s,"%s ",ASN1_OID_ISO_CCITT);
			if (mode == 2 
			&& (max = 0,(j = look_for_oid_part(oid,&max)) != -1)){
				sprintf(s+strlen(s),"%s(%d)",
				asn1_oid_table[j].s, i-80);
			}else	sprintf(s+strlen(s),"%d",i-80);
		}
	}
	s += strlen(s);
	if (mode == 2){
		while (max < oid.l){
			*s++ = ' ';
			i = max;
			if ((j = look_for_oid_part(oid,&max)) != -1){
				strcpy(s, asn1_oid_table[j].s);
				s += strlen(asn1_oid_table[j].s);
				*s++ = '(';
			}
			oid_digits (oid.v + i, max -i, s);
			s += strlen(s);
			if (j != -1) 	*s++ = ')';
		}
		*s = 0;
	}else {
		*s++ = ' ';
		oid_digits (oid.v + max, oid.l - max, s);
	}
	return (s0);
}

char * asn1_oid_put(s,oid)
char * s;
asn1_oid oid;
{
	return(asn1_oid_xput(s,oid,0));
}


/***************************************************************************\
* 									    *
* asn1_oid asn1_oid_get(s,er)						    *
* char * s;								    *
* int * er;								    *
* 									    *
* Transforms the string ``s'' into an object id. The value of the object    *
* id will point to a static buffer, and will be overwrited by the next	    *
* call.  The routine ``asn1_oid_copy'' can be used to create allocate the   *
* necessary memory, and the macro ``asn1_oid_create'' do to so in a	    *
* single pass..  In case of error, the length and value components of	    *
* ``oid'' are set to zero, and the return argument ``er'' contains the	    *
* number of the error; it can be explained by the function ``asn1_ermes''.  *
* 									    *
\***************************************************************************/
asn1_oid asn1_oid_copy(oid)
asn1_oid oid;
{	asn1 x;

	if (oid.l == 0)
		oid.v = 0;
	else{	x = (asn1) asn1_malloc((unsigned)oid.l);
		ASN1_MOVE(oid.v,x,oid.l);
		oid.v = x;
	}
	return(oid);
}

asn1_oid asn1_oid_get(s,er)
char * s;
int * er;
{	char comp [MAX_BUF_LEN], *x, *y;
	asn1 z;
	static unsigned char oid_val[MAX_BUF_LEN];
	asn1_oid oid;
	int first, second;

	oid.l = 0;
	oid.v = (asn1)0;
	/* get the first component */
	x = copy_to_blank(comp,s);
	if (comp[0] == 0){
		*er = ASN1_OIDEMPTY;
		return(oid);
	}
	if ( (y = numeric_part(comp)) && strlen(y) != 1){
		*er = ASN1_OIDBADTOP;
		return(oid);
	}
	if (y){
		switch(*y){
		case '0': first = 0; break;
		case '1': first = 1; break;
		case '2': first = 2; break;
		default:
			*er = ASN1_OIDBADTOP;
			return(oid);
		};
	}else	if (strcmp(comp,ASN1_OID_CCITT) == 0) first = 0;
	else 	if (strcmp(comp,ASN1_OID_ISO) == 0) first = 1;
	else 	if (strcmp(comp,ASN1_OID_ISO_CCITT) == 0) first = 2;
	else	first = -1;

	if (first == -1){
		if ((second = look_for_text(oid_hash(comp),comp)) < 0){
			*er = ASN1_OIDUNDEF;
			return (oid);
		}
		ASN1_MOVE(asn1_oid_table[second].oid.v,
		oid_val,asn1_oid_table[second].oid.l);
		z = oid_val + asn1_oid_table[second].oid.l;
	}else{	x = copy_to_blank(comp,x);
		if (comp[0] == 0){
			*er = ASN1_OIDMISSSEC;
			return(oid);
		}
		if (y = numeric_part(comp)){
			if ((second = atoi(y)) > 39 && first != 2){
				*er = ASN1_OIDSECBIG;
				return(oid);
			}
		} else switch (first){
		case 0:
			if (strcmp(comp,ASN1_OID_RECOM) == 0) second = 0;
			else if (strcmp(comp,ASN1_OID_QUESTION) == 0) second = 1;
			else if (strcmp(comp,ASN1_OID_ADMIN) == 0) second = 2;
			else if (strcmp(comp,ASN1_OID_NET_OP) == 0) second = 3;
			else{	*er = ASN1_OIDBADSEC;
				return(oid);
			}
			break;
		case 1:
			if (strcmp(comp,ASN1_OID_STANDARD) == 0) second = 0;
			else if (strcmp(comp,ASN1_OID_REG_AUT) == 0) second = 1;
			else if (strcmp(comp,ASN1_OID_MEMBER) == 0) second = 2;
			else if (strcmp(comp,ASN1_OID_ID_ORG) == 0) second = 3;
			else{	*er = ASN1_OIDBADSEC;
				return(oid);
			}
			break;
		default:
			*er = ASN1_OIDNUMCOMP;
			return(oid);
		}
		first = first*40 + second;
		z = oid_val;
		if (first < 128){
			*z++ = first;
		}else if (first < 16384){
			*z++ = (first>>7)|0x80;
			*z++ = first&0x7f;
		}else if (first < 2097152){
			*z++ = (first>>14)|0x80;
			*z++ = (first>>7)|0x80;
			*z++ = first&0x7f;
		}else if (first < 268435456){
			*z++ = (first>>21)|0x80;
			*z++ = (first>>14)|0x80;
			*z++ = (first>>7)|0x80;
			*z++ = first&0x7f;
		}else{
			*z++ = (first>>28)|0x80;
			*z++ = (first>>21)|0x80;
			*z++ = (first>>14)|0x80;
			*z++ = (first>>7)|0x80;
			*z++ = first&0x7f;
		}
	}
	while (*x){
		x = copy_to_blank(comp,x);
		if (comp[0] == 0) break;
		if ((y = numeric_part(comp)) == 0){
			*er = ASN1_OIDNUMCOMP;
			return(oid);
		}
		z = digits_oid(z,y);
	}
	*er = 0;
	oid.l = z - oid_val;
	oid.v = oid_val;
	return (oid);
}

/**************\
* 	       *
* utilities..  *
* 	       *
\**************/
static unsigned oid_hash(s)
char * s;
{	unsigned h = 0;

	while (*s)
		h = (h<<8) + h + *s++;
	return(h);
}

static int look_for_text(h,s)
unsigned h;
char * s;
{	int i;

	if (asn1_oid_nb == -1) return(-1);
	i = asn1_oid_thash[h%MAXHASH];
	while (i != -1){
		if (asn1_oid_table[i].thash == h
		&&  strcmp(asn1_oid_table[i].s,s) == 0) break;
		i = asn1_oid_table[i].tnext;
	}
	return(i);
}

look_for_oid(oid)
asn1_oid oid;
{	int i, x = -1;

	i = asn1_oid_first;
	while (i >= 0){
		if (asn1_oid_table[i].oid.l <= oid.l && 
		ASN1_CMP(asn1_oid_table[i].oid.v,oid.v,
		asn1_oid_table[i].oid.l) == 0){
			x = i;
			if (asn1_oid_table[i].oid.l == oid.l) break;
			i = asn1_oid_table[i].son;
		}else	i = asn1_oid_table[i].st;
	}
	return(x);
}

look_for_oid_part(oid,max)
asn1_oid oid;
int * max;
{	int x;

	while (*max < oid.l && (oid.v[(*max)++]&0x80));
	oid.l = (*max);
	if ((x = look_for_oid(oid)) != -1
	&& asn1_oid_table[x].oid.l == oid.l)
	        return(x);
	else	return(-1);
}


/********************************************************************\
* 								     *
* one_digit_more:						     *
* 								     *
* adds the new item "x" within the int array "t";		     *
* 	"n": the number of elem within "t"			     *
* 	"x": the new item to be introduced			     *
* 	"m1": the decimal base of "x"				     *
* 	"m2": the decimal base of the result "t"		     *
* the array "t" contains the representation of x as an array	     *
* of digits in base "m2", but in reverse order (lower digit first).  *
* 								     *
* ex:	(from base 10 to base 128)				     *
*  *	input->	t:	85					     *
* 		n:	1					     *
* 		x:	7					     *
* 		m1:	10					     *
* 		m2:	128					     *
* 	output-> t:	89	6				     *
* 	returned  :	2					     *
* 								     *
* 	(6 * 128 + 89 = 857)					     *
* 								     *
\********************************************************************/
int one_digit_more(t,n,x,m1,m2)
int *t, n, x, m1, m2;
{
	register i;

	if (n == 0){
		n = 1;
		t[0] = x;
	}else{	for (i=0;i<n;i++)	t[i] *= m1;
		t[0] += x;
	}

	i =0;
	while (i<n){
		if (t[i] >= m2){
			if (i == (n-1))
				t[n++] = t[i]/m2;
			else	t[i+1] += t[i]/m2;
			t[i] = t[i]%m2;
		}
		i++;
	}
	return(n);
}

/************************************************************\
* 							     *
* digits_oid:						     *
* 							     *
* from a digital string to "asn1_object_id" representation.  *
* assumes that there is only one digital string.	     *
* 							     *
\************************************************************/
static asn1 digits_oid (oid, digits)
asn1 oid;
char * digits;
{
	int n;
	int t[32];

	n = 0;

	for (; *digits; digits++)
	{	n = one_digit_more (t, n, (int) *digits - '0', 10, 128);
	}

	while (n)
		*(oid++) = 0x80 | t [--n];
	*(oid -1) &= 0x7f;
	return (oid);
}

/**************************************************************\
* 							       *
* oid_digits:						       *
* 							       *
* from  "asn1_object_id" to digital string representation.     *
* "l" bytes from the input parameter "z" will be interpreted.  *
* 							       *
\**************************************************************/
static oid_digits(z,l,s)
asn1 z;
int l;
char * s;
{
	int i = 0, n;
	int t[32];

	while (i<l){
		n = 0;
		while(i<l){
			n = one_digit_more(t,n, (int) z[i]&0x7f,128,10);
			if ((z[i++]&0x80) == 0) break;
		}
		while (n)
			*s++ = '0' + t[--n];
		if (i<l) *s++ = ' ';
	}
	*s = 0;
}


/*****************************************************************************\
* 									      *
* copy_to_blank:							      *
* 									      *
* Extract a string delimitated by blanks characters. The string can be:	      *
* -	A character string,						      *
* -	A numeric string,						      *
* -	A combination in the xyz(1234) format.				      *
* In the latter case, we tolerate a blank between the last character and the  *
* parenthesis, or between the parenthesis and the digits.		      *
* 									      *
\*****************************************************************************/
char * copy_to_blank(x,y)
register char * x, * y;
{	register not_open = 1, not_closed = 0;

	do {	while (*y == ' ' || *y == '\t')	y++;
		while (*y && *y != ' ' && *y != '\t'){
			if (not_open){
				if (*y == '('){
					not_open = 0;
					not_closed = 1;
				}
			}else{
				if (*y == ')')
					not_closed = 0;
			}
			*x++ = *y++;
		}
		while (*y == ' ' || *y == '\t') y++;
	}	while ((not_open && *y == '(') || (not_closed && *y));
	*x = 0;
	return(y);
}

/**************************************************************************\
* 									   *
* Numeric_part:								   *
* 									   *
* Extract a numeric substring from an object ID component. The format	   *
* of the component may be:						   *
* 	99999								   *
* or	abcdef(99999).							   *
* If neither of these formats is present, returns 0. Otherwise, returns a  *
* pointer to the numeric substring.					   *
* 									   *
\**************************************************************************/
char * numeric_part(s)
register char * s;
{	register char * start = s;

	while (*s >= '0' && *s <= '9') s++;
	if (s > start && *s == 0) return(start);
	if ((start = (char *)ASN1_INDEX(start,'(')) == 0)
		return(0);
	if ((s = (char *)ASN1_INDEX(++start,')')) == 0)	
		return(0);
	if (s == start) return(0);
	*s = 0;
	for (s = start; *s; s++){
		if (*s < '0' || *s > '9'){
			*(start + strlen(start)) = ')';
			return(0);
		}
	}
	return(start);
}



#ifdef ASN1_THREADED

/*
**  ABSTRACT:
**
**  ASN1 Initialization module for re-entrant oid functions.
**  Creates mutex used to serialize access to critical data structures.
**
**
*/

static void asn1_oid_init_once ( void );

/*
 * The structure that defines the one-time initialization code. This
 * structure will be associated with alloc_init_once.
 */
static pthread_once_t asn1_oid_init_once_block = pthread_once_init;

/*
 * The value that indicates whether or not the asn1 memory allocation routines
 * are currently being initialized.
 */
static boolean        asn1_oid_init_in_progress = FALSE;

/*
 * The id of the thread that is executing (has executed) the asn1 alloc
 * initialization code.
 */
pthread_t      asn1_g_alloc_init_thread;

/*
**  ROUTINE NAME:       asn1_oid_init
**
**  SCOPE:              static - declared above
**
**  DESCRIPTION:
**      
**  asn1_oid_init() is called to create and intialize the mutex.
**  Prevent asn1_oid_init() (actually pthread_once() of 
**  asn1_oid_init_once()) recursive call deadlocks by the thread that is 
**  already performing the initialization (see asn1_oid_init_once()).
**
**/

static void asn1_oid_init(void)
{
    pthread_t       current_thread;
    

    if (asn1_oid_init_in_progress)
    {
        current_thread = pthread_self();
        
        if (pthread_equal(asn1_g_alloc_init_thread, current_thread))
        {
            /*
             * We're the thread in the middle of initialization 
             * (asn1_oid_init_once).
             * Assume it knows what it's doing and prevent a deadlock.
             */
            return;
        }
    }

    pthread_once (&asn1_oid_init_once_block, asn1_oid_init_once);
}


/*
**
**  ROUTINE NAME:       asn1_oid_init_once
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**      
**  asn1_oid_init_once() is invoked only once. It creates the mutex used to
**  serialize access to critical data sections. 
**
**/

static void asn1_oid_init_once(void)

{

    /*
     * These two operations (and their order) are critical to creating
     * a deadlock-safe environment for init processing.
     */
    asn1_g_alloc_init_thread = pthread_self();
    asn1_oid_init_in_progress = TRUE;

    ASN1_SEM_CREATE(&asn1_oid_sem, pthread_mutexattr_default);

    asn1_oid_init_in_progress = FALSE;
}
#endif /* ASN1_THREADED */
