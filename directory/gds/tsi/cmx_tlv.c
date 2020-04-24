/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmx_tlv.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:09  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:36  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:47:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:04  root]
 * 
 * Revision 1.1.2.3  1994/07/06  15:08:25  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:21:34  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:16:30  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:29  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:10  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:50:33  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmx_tlv.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:09 $";
#endif

/*
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1992
 *               All Rights Reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */

/*
static char sccsid[] = "@(#)cmx_tlv.c	311.3	93/08/24	CMX (NC1)" ;
*/
#define CMX_TLV_C

#ifdef THREADSAFE
# include <pthread.h>
#endif

#include <stdio.h>
#include <limits.h>
#include "cmx_tlv.h"

/* functions prototypes   	*/

static int find_type (const uchar *, const int, const uchar);
int cxtlv_create  (const uchar, const int, const uchar *,
			uchar *, const int);
int cxtlv_add     (const uchar *, const int, const uchar *, const int,
			uchar *, const int);
int cxtlv_remove_elem  (const uchar *, const int, const uchar,
			uchar *, const int);
int cxtlv_remove_all (const uchar *, const int, const uchar,
			uchar *, const int);
int cxtlv_first   (const uchar *, const int, 
			uchar *, const int);
int cxtlv_rest    (const uchar *, const int, 
			uchar *, const int);
int cxtlv_number_of(const uchar *, const int);
int cxtlv_find_type (const uchar *, const int, const uchar, 
			uchar *, const int);
int cxtlv_find_tlv (const uchar *, const int, const uchar *, 
			uchar *, const int);
int cxtlv_replace (const uchar *, const int, const uchar, 
			const uchar *, uchar *, const int);


#ifdef KERNEL
#define MY_COPY(_pto,_pfrom,_length) \
	CX_MOVE((char *)(_pfrom),(char *)(_pto),(size_t)(_length))
#else
#define MY_COPY(_pto,_pfrom,_length) \
	memcpy((void *)(_pto),(void *)(_pfrom),(size_t)(_length))
#endif


#ifdef KERNEL
#define MY_COMPARE(_pt1,_pt2,_length) \
	cxtlv_kernel_cmp((const char *)(_pt1),(const char *)(_pt2),(size_t)(_length))
#else
#define MY_COMPARE(_pt1,_pt2,_length) \
	memcmp((const void *)(_pt1),(const void *)(_pt2),(size_t)(_length))
#endif


#ifdef KERNEL
/* 
 * The kernel compare functions have to be defined in this way
 * because in the kernel you can't use the memcmp function 
 * defined in string.h.
 * There's no test if the given length _length is correct, i.e.
 * if the length of one of the strings is smaller than the
 * given length, because when calling this functions there is the
 * condition that the two strings have the same length, in this
 * case the given length.
 */
static int cxtlv_kernel_cmp (const char *_pt1, const char *_pt2,
					size_t _length)
{
int i=0;

	while ( i < _length )
		{
		if ( _pt1[i]==_pt2[i] )
			i++;
		else return (-1);
		}
	return (0);
}
#endif

/* find_type: is only a help function for the functions 
 * cxtlv_remove_elem, cxtlv_remove_all and cxtlv_replace to check if 
 * the given type is in the given tlv_list; if it is present the 
 * returnvalue is the length of the found tlv-element;
 * error codes:  CX_WRONG_LENGTH
 *			CX_TYPE_NOT_FOUND 
 */

static int find_type (const uchar *tlv_list, const int length,
              const uchar find_type )
{
int proc_length = 0;	/* proc_length is the whole length of the	*/
			/* given tlv-list already processed		*/

	/* if length is negative, errorvalue is CX_WRONG_LENGTH         */
	if ( length<0 )
		return (CX_WRONG_LENGTH);

	/* searching for the first tlv-element with the given type	*/
	/* as long as length hasn't been reached			*/
	while ( CXTLV_TYPE(tlv_list+proc_length)!=find_type &&
		  proc_length<length )
		proc_length+=CXTLV_TLV_LENGTH(tlv_list+proc_length);

	/* if the interrupt condition was that proc_length reached	*/
	/* length, the given type was not found; errorvalue is		*/
	/* CX_TYPE_NOT_FOUND						*/
	if (proc_length >=length )
		return (CX_TYPE_NOT_FOUND);

	return (CXTLV_TLV_LENGTH(tlv_list+proc_length));
}



/* cxtlv_create: type, length and value are collected in the TLV-field
 * (length = maxlength); returnvalue is the length of the used field 
 * errorcodes:	CX_LENGTH_TOO_LONG
 *			CX_WRONG_LENGTH 
 *			CX_FIELD_TOO_SHORT
 */

int cxtlv_create (const uchar type, const int length,
                const uchar *value, uchar *result_tlv, const int max_length)
{
	/* if the int-value length is longer than an unsigned char	*/
	/* errorcode is CX_LENGTH_TOO_LONG				*/
	if ( length > UCHAR_MAX )
		return (CX_LENGTH_TOO_LONG);

	/* if length is negative, errorvalue is CX_WRONG_LENGTH		*/
	if ( length<0 )
		return (CX_WRONG_LENGTH);

	/* if the result-field is too short for the new tlv,            */
	/* errorvalue is CX_FIELD_TOO_SHORT				*/
	if (length+CX_VALUE_OFFSET > max_length)
		return (CX_FIELD_TOO_SHORT);

	/* type, length and value are collected in result_tlv		*/
	result_tlv[0]=type;
	result_tlv[1]=(uchar)length;
	if ( length>0 )
		MY_COPY((result_tlv+CX_VALUE_OFFSET),value,length);
	return (length+CX_VALUE_OFFSET);
}



/* cxtlv_add: appends the second TLV to the first TLV; return value is
 * the length used for the new TLV; the third TLV is the result TLV ;
 * errorcodes:	CX_WRONG_LENGTH 
 *			CX_FIELD_TOO_SHORT
 */

int cxtlv_add (const uchar *first_tlv, const int first_length,
             const uchar *sec_tlv, const int sec_length,
             uchar *result_tlv, const int max_length)
{
	/* if first_length or sec_length is negative           		*/
	/* errorvalue is CX_WRONG_LENGTH				*/
	if ( first_length<0 || sec_length<0 )
		return (CX_WRONG_LENGTH);

	/* if the result-field is too short for the new tlv,            */
	/* returnvalue is CX_FIELD_TOO_SHORT				*/
	if ( (first_length+sec_length) > max_length ||
	     (first_length+sec_length) < 0 ) /* test overflow		*/
		return (CX_FIELD_TOO_SHORT);

	/* if two empty tlv-lists should be added, the length of	*/
	/* the res_tlv is 0						*/
	else if (first_length==0 && sec_length==0)
		return (0);

	/* if one empty tlv-list is given,cxtlv_add copies the other    */
	/* tlv into the result_tlv and returns its length		*/
	else if ( first_length==0 )
		{
		MY_COPY(result_tlv,sec_tlv,sec_length);
		return (sec_length);
		}
	else if ( sec_length==0 )
		{
		MY_COPY(result_tlv,first_tlv,first_length);
		return (first_length);
		}
	/* else: the two tlv-lists are added as described		*/
	else  {
		MY_COPY(result_tlv,first_tlv,first_length);
		MY_COPY((result_tlv+first_length),sec_tlv,sec_length);
		return (first_length+sec_length);
		}
}



/* cxtlv_remove_elem: tlv-list is the given tlv, length is the whole   
 * length of this tlv-list, rem_type is the type of the tlv-   
 * element to be removed, new tlv is copied into result-tlv.      
 * Only the first tlv-element with the given type will be removed.
 * errorcodes:	CX_WRONG_LENGTH 
 *			CX_TYPE_NOT_FOUND
 *			CX_FIELD_TOO_SHORT
 */
 
int cxtlv_remove_elem (const uchar *tlv_list, const int length,
                const uchar rem_type, uchar *result_tlv,
                const int max_length)
{
int proc_length = 0,	/* proc_length is the whole length of the	*/
			/* given  tlv already "processed"		*/
    res_length = 0,	/* res_length is the whole length of the	*/
			/* result-tlv already "occupied"		*/
    act_length,		/* act_length is the whole length		*/
			/* of the "actual" tlv-element			*/
    length_of_found,	/* ret_value is the returnvalue of find;	*/
			/* it can be an errorcode or it is the		*/
			/* length of the found tlv-element		*/
    already_removed = 0;/* boolean value to check if wanted		*/
			/* tlv-element is already removed		*/

	length_of_found=find_type(tlv_list,length,rem_type);
	/* if there is an error in find ( CX_TYPE_NOT_FOUND, etc. )	*/
	/* the error will be also returned in cxtlv_remove_elem	*/
	if ( length_of_found< 0 )
		return (length_of_found);

	/* if the result-field is to short for the new tlv,		*/
	/* return value is CX_FIELD_TOO_SHORT				*/
	else if ( (length-length_of_found) > max_length)
		return (CX_FIELD_TOO_SHORT); 

	/* there are no more errors possible; now the tlv-list is	*/
	/* copied in the result-field without the found tlv		*/
	while ( proc_length < length )
		{
		act_length=CXTLV_TLV_LENGTH(tlv_list+proc_length);
		if (  CXTLV_TYPE(tlv_list+proc_length) != rem_type ||
                 already_removed  )
			{
			/* copy of one tlv-element in the result-field	*/
			MY_COPY((result_tlv+res_length),(tlv_list+proc_length),
         	             (act_length) ); 
			res_length+=act_length;
			}
		else	/* the given type is found and this element	*/
			/* will be skipped				*/
			already_removed=1;
		proc_length+=act_length;
		}
	return (res_length);
}



/* cxtlv_remove_all: tlv-list is the given tlv, length is the whole 
 * length of this tlv-list, rem_type is the type of the tlv-     
 * elments to be removed, new tlv is copied into result-tlv.
 * Every tlv-element with the given type will be removed.
 * errorcodes:	CX_WRONG_LENGTH 
 *			CX_TYPE_NOT_FOUND
 *			CX_FIELD_TOO_SHORT
 */
 
int cxtlv_remove_all (const uchar *tlv_list, const int length,
                    const uchar rem_type, uchar *result_tlv,
                    const int max_length)
{
int	proc_length = 0,/* proc_length is the whole length of the	*/
			/* given  tlv already "processed"		*/
	res_length = 0,	/* res_length is the whole length of the	*/
			/* result-tlv already "occupied"		*/
	act_length,     /* act_length is the whole length		*/
			/* of the "actual" tlv-element			*/
	length_of_found;/* ret_value is the returnvalue of find;	*/
			/* it can be an errorcode or it is the		*/
			/* length of the found tlv-element		*/

	length_of_found=find_type(tlv_list,length,rem_type);
	/* if there is an error in find ( CX_TYPE_NOT_FOUND, etc. )	*/
	/* the error will be also returned in cxtlv_remove_all		*/
	if ( length_of_found< 0 )
		return (length_of_found);

	while ( proc_length < length )
		{
		act_length=CXTLV_TLV_LENGTH(tlv_list+proc_length);
		if  ( CXTLV_TYPE(tlv_list+proc_length) != rem_type )
			{
			/* if the result-field is to short for the new	*/
			/* tlv, returnvalue is CX_FIELD_TOO_SHORT	*/
			if  ( (res_length+act_length) > max_length )
				return (CX_FIELD_TOO_SHORT);

			/* the actual tlv-element has not the rem_type	*/
			/* thus, the element is copied to the result_tlv*/
			MY_COPY((result_tlv+res_length),(tlv_list+proc_length),
				  act_length);
			res_length+=act_length;
			}
		proc_length+=act_length;
		}
	return (res_length);
}



/* cxtlv_first: in the result-field the first tlv-element of the
 * given tlv-list is copied, returnvalue is the length of this
 * tlv-element.
 * errorcodes:	CX_WRONG_LENGTH 
 *			CX_FIELD_TOO_SHORT
 */

int cxtlv_first   (const uchar *tlv_list, const int length, 
		       uchar *result_tlv, const int max_length)
{
int res_length;

	/* if length is negative, errorvalue is CX_WRONG_LENGTH		*/
	if ( length<0 )
		return (CX_WRONG_LENGTH);
	res_length=CXTLV_TLV_LENGTH(tlv_list);

	/* if the result-field is to short for the new tlv,		*/
	/* errorvalue is CX_FIELD_TOO_SHORT				*/
	if (res_length > max_length)
		return (CX_FIELD_TOO_SHORT);

	MY_COPY(result_tlv,tlv_list,res_length);
	return (res_length);
}



/* cxtlv_rest: the given tlv will be copied in the result-field
 * without the first tlv-element, return value is the length
 * of the result-field.
 * errorcodes:	CX_WRONG_LENGTH 
 *			CX_FIELD_TOO_SHORT
 */

int cxtlv_rest (const uchar *tlv_list, const int length, 
	     	    uchar *result_tlv, const int max_length)
{
int first_length, res_length;

	/* if length is negative, errorvalue is CX_WRONG_LENGTH		*/
	if ( length<0 )
		return (CX_WRONG_LENGTH);

	first_length=CXTLV_TLV_LENGTH(tlv_list);
	/* if the result-field is too short for the new tlv,		*/
	/* errorvalue is CX_FIELD_TOO_SHORT				*/
	res_length=length-first_length;
	if (res_length > max_length)
		return (CX_FIELD_TOO_SHORT);

	MY_COPY(result_tlv,(tlv_list+first_length),res_length);	
	return (res_length);
}



/* cxtlv_number_of: this function returns the number of tlv-elements
 * of the given tlv.
 * errorcodes:	CX_WRONG_LENGTH 
 */

int cxtlv_number_of(const uchar *tlv_list, const int length)
{
int	counter=0,	/* counter contains the actual number of	*/
			/* tlv-elements					*/
	proc_length = 0;/* proc_length is the whole length of the	*/
			/* given  tlv already "processed"		*/

	/* if length is negative, errorvalue is CX_WRONG_LENGTH		*/
	if ( length<0 )
		return (CX_WRONG_LENGTH);

	while ( proc_length < length )
		{
		counter++;
		proc_length += CXTLV_TLV_LENGTH(tlv_list+proc_length);
		}
	return (counter);	
}



/* cxtlv_find_type: the whole given tlv-list is searched for a tlv- 
 * element with the given type (find_type); the first tlv-
 * element found is written in the result-field and the return value
 * is the used length; 
 * if there's no such tlv-element, the return value is CX_TYPE_NOT_FOUND;
 * other errorcodes: CX_WRONG_LENGTH (of the given tlv-list)
 *                   CX_FIELD_TOO_SHORT (of the result_field)  
 */

int cxtlv_find_type (const uchar *tlv_list, const int length,
              const uchar find_type, uchar *result_tlv,
              const int max_length)
{
int proc_length = 0;	/* proc_length is the whole length of the	*/
			/* given tlv-list already processed		*/

	/* if length is negative, errorvalue is CX_WRONG_LENGTH		*/
	if ( length<0 )
		return (CX_WRONG_LENGTH);

	/* searching for the first tlv-element with the given type	*/
	/* as long as length hasn't being reached			*/
	while ( CXTLV_TYPE(tlv_list+proc_length)!=find_type &&
		  proc_length<length )
		proc_length+=CXTLV_TLV_LENGTH(tlv_list+proc_length);

	/* if the interrupt condition was that proc_length reached	*/
	/* length, the given type was not found; errorvalue is		*/
	/* CX_TYPE_NOT_FOUND						*/
	if (proc_length >=length )
		return (CX_TYPE_NOT_FOUND);

	/* if the result-field is to short for the new tlv,		*/
	/* error value is CX_FIELD_TOO_SHORT				*/
	if ( CXTLV_TLV_LENGTH(tlv_list+proc_length) > max_length )
		return (CX_FIELD_TOO_SHORT);

	MY_COPY(result_tlv,(tlv_list+proc_length),
		  CXTLV_TLV_LENGTH(tlv_list+proc_length) );
	return (CXTLV_TLV_LENGTH(tlv_list+proc_length));
}



/* cxtlv_find_tlv: the whole given tlv-list is searched for a tlv- 
 * element with the given tlv (same type and value); the first tlv-
 * element found is written in the result-field and the return value
 * is the used length; if there's no such tlv-element, the
 * return value is CX_TLV_NOT_FOUND;
 * other errorcodes: CX_WRONG_LENGTH (of the given tlv-list)
 *                   CX_FIELD_TOO_SHORT (of the result_field)  
 */

int cxtlv_find_tlv (const uchar *tlv_list, const int length,
			  const uchar *find_tlv, uchar *result_tlv,
			  const int max_length)
{
int proc_length = 0,	/* proc_length is the whole length of the	*/
			/* given tlv-list already processed		*/
    find_tlv_length=0;	/* find_tlv_length is the whole length of	*/
			/* the given tlv-list already processed		*/

	/* if length is negative, errorvalue is CX_WRONG_LENGTH		*/
	if ( length<0 )
		return (CX_WRONG_LENGTH);

	find_tlv_length = CXTLV_TLV_LENGTH(find_tlv); 

	while (proc_length < length )
		{	
		/* if type and length of the actual tlv-element are 	*/
		/* according to those of the given tlv-element ...	*/
		if ( CXTLV_TYPE(tlv_list+proc_length)==CXTLV_TYPE(find_tlv) &&
	         CXTLV_VAL_LENGTH(tlv_list+proc_length)==find_tlv_length )
			{

			/* if the values of the 2 tlv-elements are the same...*/
			if ( MY_COMPARE(CXTLV_VALUE(tlv_list+proc_length), 
			   CXTLV_VALUE(find_tlv),CXTLV_VAL_LENGTH(find_tlv))==0)
				{

				/* if the result-field is to short for the new*/
				/* tlv, errorvalue is CX_FIELD_TOO_SHORT*/
				if ( find_tlv_length > max_length )
					return (CX_FIELD_TOO_SHORT);

				/* the searched tlv-element has been found,*/
				/* and copied to the result-tlv		*/
				/* returnvalue is the length of the result tlv*/
				MY_COPY(result_tlv,(tlv_list+proc_length), 
				        find_tlv_length);
				return (find_tlv_length);
				}
			}
		proc_length+=CXTLV_TLV_LENGTH(tlv_list+proc_length);
		}

	/* the end of the while loop has been reached, that means	*/
	/* the searched tlv-element has not been found;			*/
	/* errorvalue CX_TLV_NOT_FOUND					*/
	return (CX_TLV_NOT_FOUND);
}



/* cxtlv_replace: tlv_list is searched for a tlv_element with     
 * repl_type, when found, this element will be removed and     
 * tlv_fill, als an element, will be filled in at this location 
 * errorcodes:	CX_WRONG_LENGTH 
 *			CX_FIELD_TOO_SHORT 
 *			CX_TYPE_NOT_FOUND
 */

int cxtlv_replace (const uchar *tlv_list, const int length,
                 const uchar repl_type, const uchar *tlv_fill,
                 uchar *result_tlv, const int max_length)
{
int	proc_length = 0,/* proc_length is the whole length of the	*/
			/* given  tlv already "processed"		*/
	res_length = 0,	/* res_length is the whole length of the	*/
			/* result-tlv already "occupied"		*/
	act_length,     /* act_length is the whole length		*/
			/* of the "actual" tlv-element			*/
	length_of_found,/* ret_value is the returnvalue of find;	*/
			/* it can be an errorcode or it is the		*/
			/* length of the found tlv-element		*/
	already_replaced = 0;/* boolean value to check if wanted	*/
			/* tlv-element is already replaced		*/

	length_of_found=find_type(tlv_list,length,repl_type);
	/* if there is an error in find_type (CX_TYPE_NOT_FOUND,etc. )	*/
	/* the error will be also returned in cxtlv_remove_elem		*/
	if ( length_of_found< 0 )
		return (length_of_found);

	/* if the result-field is to short for the new tlv,		*/
	/* returnvalue is CX_FIELD_TOO_SHORT				*/
	if ((length+CXTLV_TLV_LENGTH(tlv_fill)-length_of_found) >max_length)
		return (CX_FIELD_TOO_SHORT);

	while ( proc_length < length )
		{
		act_length=CXTLV_TLV_LENGTH(tlv_list+proc_length);
		if ( CXTLV_TYPE(tlv_list+proc_length) != repl_type ||
			already_replaced  )
			{
			/* the actual tlv-element has to be added to	*/
			/* the result-tlv 				*/
			MY_COPY((result_tlv+res_length),(tlv_list+proc_length),
				   act_length);
			res_length+=act_length;
			}
		else 	
			{
			/* the actual tlv-element is the element to be 	*/
			/* replaced; the tlv_fill will be added to the	*/
			/* result-tlv; already_replaced will be set to	*/
			/* 1, because only the first found element 	*/
			/* should be replaced				*/
			MY_COPY((result_tlv+res_length),tlv_fill,
				   CXTLV_TLV_LENGTH(tlv_fill) );	
			res_length+=CXTLV_TLV_LENGTH(tlv_fill);
			already_replaced = 1;
			}				
		proc_length+=act_length;
		}
	return (res_length);
}
