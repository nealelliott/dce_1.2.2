/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPAcl.c,v $
 * Revision 1.1.6.2  1996/02/18  18:24:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:14  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:31:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:26  root]
 * 
 * Revision 1.1.4.1  1994/02/22  18:09:35  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:19:48  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:58:10  marrek]
 * 
 * Revision 1.1.2.2  1993/08/10  07:01:05  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:12:52  marrek]
 * 
 * Revision 1.1.4.2  1992/12/30  21:16:14  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:04:27  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:17:43  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:45:21  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsPAcl.c,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 18:24:31 $";
#endif

/***************************************************************
*															   *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG	1991   *
*				 ALL RIGHTS	RESERVED						   *
*															   *
***************************************************************/

/* ****************************************************************** */
/*																	  */
/*	 * COPYRIGHT   (C)	1990 BY	BULL S.A					  */
/*	 * All rights reserved											  */
/*	 *																  */
/*	 * This	software is	furnished under	licence	and	may	be used	only  */
/*	 * in accordance with the terms	of that	licence	and	with the	  */
/*	 * inclusion of	the	above copyright	notice.						  */
/*	 * This	software may not be	provided or	otherwise made available  */
/*	 * to, or used by, any other person. No	title to or	ownership of  */
/*	 * the software	is hereby transferred.							  */
/*																	  */
/* ****************************************************************** */
/*	@(#)dsPAcl.c	1.4	(Bull S.A) 2/11/92	*/

/**********************************************************************/
/*									  */
/*		xds_acl.c						  */
/*									  */
/*	contains Access	Control	List   parser routines			  */
/*									  */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <xom.h>
#include <d2dir.h>
#include <xdsiapl.h>
#include <xomip.h>
#include <alloc.h>
#include <xdsalloc.h>
#include <xdsgds.h>
#include <xdsdef.h>
#include <dsPAcl.h>

#define	OMP_VALID_INTERPRETATION(inter)	((inter	>= DSX_SINGLE_OBJECT) &&\
				 (inter	<= DSX_ROOT_OF_SUBTREE))

/*****************************************************************************/
/*																			 */
/*	FUNCTION:  dsP_decode_aci (	)											 */
/*																			 */
/*	SHORT DESCRIPTION:														 */
/*	  The function decodes an ACL_ITEM string and fill in an Aci_info		 */
/*	  structure.															 */
/*										 */
/*	INPUT-PARAMETER:														 */
/*		ac_i	=  Pointer to an Aci_info structure	which will be filled for */
/*				   the specific	ACL_ITEM with the ACL_ITEM length, the		 */
/*				   INTERPRETATION type,	and	a pointer to the dname string.	 */
/*		aci_p	=  Pointer to the ACL_ITEM which should	be decoded			 */
/*																			 */
/*	OUTPUT-PARAMETER:														 */
/*		*ac_i	=  Pointer to the decoded ACL_ITEM info: length,			 */
/*				   INTERPRETATION type,	dname pointer.						 */
/*		*aci_l	=  ACCESS CONTROL ITEM length								 */
/*		*aci_n	=  counter which is	incremented	if the ACL_ITEM	is			 */
/*				   to be considered	(INTERPRETATION	!= D2_IGNORE)			 */
/*																			 */
/*	RETURN-VALUE:															 */
/*		OM_SUCCESS	= ACI decoded without any error.						 */
/*		OM_WRONG_VALUE_SYNTAX  = unknown INTERPRETATION	type detected.		 */
/*										 */
/*****************************************************************************/

OM_return_code
dsP_decode_aci (
	Aci_info *ac_i,
	byte *aci_p,
	OM_sint	*aci_l,
	OM_sint	*aci_n)
{
	OM_sint	inter;
	byte	*acip;
	acip = aci_p;
	*aci_l = ac_i->ai_len =		1 +		strlen((char *)acip);
	inter =		ac_i->ai_inter = *acip;
	ac_i->ai_p = (char *)++acip;
	if (inter != D2_IGNORE)
		if (!OMP_VALID_INTERPRETATION(IAPL_TO_DSX_INTER(inter))) {
			return(OM_WRONG_VALUE_SYNTAX);
		} else {
			(*aci_n)++;
		} /* endif */
	return(OM_SUCCESS);
} /* end dsP_decode_aci	*/


/*****************************************************************************/
/*																			 */
/*	FUNCTION:  dsP_decode_acr (	)												 */
/*																			 */
/*	SHORT DESCRIPTION:														 */
/*	  The function decodes an ACR string and fill in an	array of D2_NOIDX_ACL*/
/*	  Aci_info structures. For each	of them, it	calls the dsP_decode_aci()		 */
/*	  function.																 */
/*										 */
/*	INPUT-PARAMETER:														 */
/*		ac_r	=  Pointer to an array,	whereby	each element of	the	array	 */
/*				   concerns	a specific ACL_ITEM	and	will be	filled with	the	 */
/*				   ACL_ITEM	length,	the	INTERPRETATION type, and a pointer	 */
/*				   to the dname	string.										 */
/*		acr_p	=  Pointer to the ACR which	should be decoded				 */
/*																			 */
/*	OUTPUT-PARAMETER:														 */
/*		*ac_r	=  Pointer to the decoded ACL_ITEM info: length,			 */
/*				   INTERPRETATION type,	dname pointer.						 */
/*		*acr_l	=  ACCESS CONTROL RIGHT	length								 */
/*		*acr_nval= counter which is	incremented	with the number	of			 */
/*				   significant ACL_ITEM	(INTERPRETATION	!= D2_IGNORE)		 */
/*																			 */
/*	RETURN-VALUE:															 */
/*		OM_SUCCESS	= ACR decoded without any error.						 */
/*		OM_WRONG_VALUE_SYNTAX  = unknown INTERPRETATION	type detected.		 */
/*										 */
/*****************************************************************************/

OM_return_code
dsP_decode_acr (
	Acr_info *ac_r,
	byte *acr_p,
	OM_sint	*acr_l,
	OM_sint	*acr_nval)
{
	OM_return_code	ret;
	OM_sint	j;
	OM_sint	ac_len;
	OM_sint	ac_nb;
	OM_sint	ai_len;
	OM_sint	nb;
	char	*acrp;
	Acr_info * acr;

	*acr_l = 0;
	*acr_nval =		0;
	acrp = (char *)acr_p;
	ac_len = ac_nb = 0;
	ac_r->ar_p = acrp;
	for	(j = 0,		acr	 = ac_r; j <	D2_NOIDX_ACL; j++) {
		ai_len = nb	 = 0;
		if ((ret = dsP_decode_aci(&(acr->ar_info[j]), (byte *)acrp,	&ai_len,
			 &nb))	 !=	OM_SUCCESS)
			return(ret);
		ac_nb += nb;
		ac_len += ai_len;
		acrp +=		ai_len;
	}
	acr->ar_len	 = ac_len;
	(*acr_nval)	 +=	ac_nb;
	*acr_l += ac_len;
	return(OM_SUCCESS);
} /* end dsP_decode_acr	*/


/*****************************************************************************/
/*																			 */
/*	FUNCTION:  dsP_decode_acl (	)												 */
/*																			 */
/*	SHORT DESCRIPTION:														 */
/*	  The function decodes an ACL string and fill in an	array of			 */
/*	  D2_NOACCESS_RIGHTS Acr_info structures.								 */
/*	  For each of them,	it calls the dsP_decode_acr() function.					 */
/*										 */
/*	INPUT-PARAMETER:														 */
/*		ac_r_l	=  Pointer to an array,	whereby	each element of	the	array	 */
/*				   concerns	a specific ACCESS RIGHT	:						 */
/*					  -	MODIFY PUBLIC										 */
/*					  -	READ STANDARD										 */
/*					  -	MODIFY STANDARD										 */
/*					  -	READ SENSITIVE										 */
/*					  -	MODIFY SENSITIVE									 */
/*				   and will	be filled with the number of significant		 */
/*				   ACL_ITEM	for	the	relative ACCESS	RIGHT, the global		 */
/*				   access right	string length, the pointer to the first		 */
/*				   character of	the	first ACL_ITEM and infos about each		 */
/*				   ACL_ITEM													 */
/*		acl_p	=  Pointer to the area where the ACL should	be decoded		 */
/*																			 */
/*	OUTPUT-PARAMETER:														 */
/*		*ac_r_l	=  Pointer to the decoded ACL info :						 */
/*				   for each	ACCESS RIGHT : length, number of significant	 */
/*				   ACL_ITEM, pointer to	the	first ACL_ITEM,					 */
/*				   and for each	ACL_ITEM : length, INTERPRETATION type,		 */
/*				   pointer to the dname	string.								 */
/*		*acl_l	=  ACCESS CONTROL LIST string length						 */
/*		*acl_nval= number of significant ACL_ITEM							 */
/*				   (INTERPRETATION != D2_IGNORE)							 */
/*																			 */
/*	RETURN-VALUE:															 */
/*		OM_SUCCESS	= ACL encoded without any error.						 */
/*		OM_WRONG_VALUE_SYNTAX  = unknown INTERPRETATION	type detected.		 */
/*										 */
/*****************************************************************************/

OM_return_code
dsP_decode_acl (
	Acr_info *ac_r_l[],
	byte *acl_p,
	OM_sint	*acl_l,
	OM_sint	*acl_nval)
{
	OM_return_code	ret;
	Acr_info * acl;
	OM_sint	i;
	OM_sint	ac_len;
	OM_sint	ac_nval;
	char	*aclp;

	*acl_l = 0;
	aclp = (char *)acl_p;
	*acl_nval =		0;
	for	(i = 0,		acl	 = *ac_r_l;	i <	D2_NOACCESS_RIGHTS;	i++, acl++)	 {
		acl->ar_p =		aclp;
		acl->ar_val_nb = 0;
		ac_nval	 = 0;
		ac_len = 0;
		if ((ret = dsP_decode_acr(acl, (byte *)aclp, &ac_len, &ac_nval)) !=
			OM_SUCCESS)
			return(ret);
		acl->ar_len	 = ac_len;
		acl->ar_val_nb = ac_nval;
		*acl_l += ac_len;
		*acl_nval += ac_nval;
		aclp +=		ac_len;
	}
	return(OM_SUCCESS);
} /* end dsP_decode_acl	*/


/*****************************************************************************/
/*																			 */
/*	FUNCTION:  dsP_encode_aci (	)											 */
/*																			 */
/*	SHORT DESCRIPTION:														 */
/*	  The function encodes an ACL_ITEM string from an Aci_info structure	 */
/*										 */
/*	INPUT-PARAMETER:														 */
/*		ac_i	=  Pointer to an Aci_info structure	which concerns a		 */
/*				   specific	ACL_ITEM										 */
/*				   and contains	the	ACL_ITEM length	(INTERPRETATION	char +	 */
/*				   dname length), the INTERPRETATION type, and a pointer to	 */
/*				   the dname string.										 */
/*		aci_p	=  Address of a	pointer	to the area	where the encoded		 */
/*				   ACL_ITEM	should be stored.								 */
/*																			 */
/*	OUTPUT-PARAMETER:														 */
/*		**aci_p	=  Encoded ACCESS CONTROL ITEM								 */
/*																			 */
/*	RETURN-VALUE:															 */
/*		OM_SUCCESS	= ACI encoded without any error.						 */
/*										 */
/*****************************************************************************/

OM_return_code
dsP_encode_aci (
	Aci_info *ac_i,
	byte **aci_p)
{
	byte	*acip;

	acip = (byte*) * aci_p;
	*acip++	 = ac_i->ai_inter;
	memcpy (acip, ac_i->ai_p , ac_i->ai_len	 - 1);
	*aci_p += ac_i->ai_len;
	return(OM_SUCCESS);
} /* end dsP_encode_aci	*/


/*****************************************************************************/
/*																			 */
/*	FUNCTION:  dsP_encode_acr (	)												 */
/*																			 */
/*	SHORT DESCRIPTION:														 */
/*	  The function encodes an ACL string from an array of D2_NOIDX_ACL		 */
/*	  Aci_info structures. For each	of them, it	calls the dsP_encode_aci()		 */
/*	  function.																 */
/*										 */
/*	INPUT-PARAMETER:														 */
/*		ac_r	=  Pointer to an array,	whereby	each element of	the	array	 */
/*				   concerns	a specific ACL_ITEM								 */
/*				   and contains	the	ACL_ITEM length	(INTERPRETATION	char +	 */
/*				   dname length), the INTERPRETATION type, and a pointer to	 */
/*				   the dname string.										 */
/*		acr_p	=  Address of a	pointer	to the area	where the encoded ACL	 */
/*				   should be stored.										 */
/*																			 */
/*	OUTPUT-PARAMETER:														 */
/*		**acr_p	=  Encoded ACCESS CONTROL RIGHT								 */
/*																			 */
/*	RETURN-VALUE:															 */
/*		OM_SUCCESS	= ACR encoded without any error.						 */
/*										 */
/*****************************************************************************/

OM_return_code
dsP_encode_acr (
	Acr_info *ac_r,
	byte **acr_p)
{
	OM_sint	j;
	OM_sint	ac_len;
	Acr_info	 * acr;
	byte	*acrp;

	acrp = (byte *)	 * acr_p;
	ac_len = 0;
	for	(j = 0,		acr	 = ac_r; j <	D2_NOIDX_ACL; j++) {
		dsP_encode_aci(&(acr->ar_info[j]), &acrp);
	}
	*acr_p = acrp;
	return(OM_SUCCESS);
} /* end dsP_encode_acr	*/


/*****************************************************************************/
/*																			 */
/*	FUNCTION:  dsP_encode_acl (	)												 */
/*																			 */
/*	SHORT DESCRIPTION:														 */
/*	  The function encodes an ACL string from an array of D2_NOACCESS_RIGHTS */
/*	  Acr_info structures. For each	of them, it	calls the dsP_encode_acr()		 */
/*	  function.																 */
/*										 */
/*	INPUT-PARAMETER:														 */
/*		ac_r_l	=  Pointer to an array,	whereby	each element of	the	array	 */
/*				   concerns	a specific ACCESS RIGHT	:						 */
/*					  -	MODIFY PUBLIC										 */
/*					  -	READ STANDARD										 */
/*					  -	MODIFY STANDARD										 */
/*					  -	READ SENSITIVE										 */
/*					  -	MODIFY SENSITIVE									 */
/*				   and contains	the	number of significant ACL_ITEM for the	 */
/*				   relative	ACCESS RIGHT, the global access	right string	 */
/*				   length, the pointer to the first	character of the first	 */
/*				   ACL_ITEM	and	an array of	D2_NOIDX_ACL Aci_info structures.*/
/*		acl_p	=  Address of a	pointer	to the area	where the encoded ACL	 */
/*				   should be stored.										 */
/*																			 */
/*	OUTPUT-PARAMETER:														 */
/*		**acl_p	=  Encoded ACCESS CONTROL LIST								 */
/*																			 */
/*	RETURN-VALUE:															 */
/*		OM_SUCCESS	= ACL encoded without any error.						 */
/*										 */
/*****************************************************************************/
OM_return_code
dsP_encode_acl (
	Acr_info	*ac_r_l[],
	byte **acl_p)
{
	OM_return_code	ret;
	Acr_info * acl;
	OM_sint	i;
	OM_sint	ac_len;
	byte	*aclp;

	aclp = (byte *)	 * acl_p;
	ac_len = 0;
	for	(i = 0,		acl	 = *ac_r_l;	i <	D2_NOACCESS_RIGHTS;	i++, acl++)	 {
		if ((ret = dsP_encode_acr(acl, &aclp)) != OM_SUCCESS)
			return(ret);
		ac_len += acl->ar_len;
	}
	return(OM_SUCCESS);
} /* end dsP_encode_acl	 */


/*****************************************************************************/
/*																			 */
/*	FUNCTION:  dsP_get_acl_length (	)										 */
/*																			 */
/*	SHORT DESCRIPTION:														 */
/*										 */
/*	dsP_get_acl_length () returns the total	length of an ACL string		 */
/*										 */
/*	INPUT-PARAMETER:														 */
/*		acl	  =	 Pointer to	the	ACL	string									 */
/*										 */
/*	OUTPUT-PARAMETER:														 */
/*	>0		= acl length						 */
/*										 */
/*****************************************************************************/
OM_sint
dsP_get_acl_length(xds_acl acl)
{
	OM_sint	i;
	OM_sint	len	 = 0;
	if (acl)
		for	(i = 0;	i <		D2_ACLIDX_MAX; i++)
			len	 +=	strlen(&((char *)acl)[len])	+ 1;
	return (len);
} /* end dsP_get_acl_length	*/
