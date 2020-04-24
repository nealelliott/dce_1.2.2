/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPPsap.c,v $
 * Revision 1.1.6.2  1996/02/18  18:24:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:27  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:32:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:38  root]
 * 
 * Revision 1.1.4.1  1994/02/22  18:10:15  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:21:02  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:00:05  marrek]
 * 
 * Revision 1.1.2.2  1993/08/10  07:04:44  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:15:30  marrek]
 * 
 * Revision 1.1.4.4  1992/12/30  21:16:55  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:05:14  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/07  14:10:37  marrek
 * 	November 1992 code drop
 * 	Casts in strncpy()
 * 	[1992/12/07  14:09:45  marrek]
 * 
 * Revision 1.1.4.2  1992/12/04  17:38:39  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	Replace T_NO_NSAP by T_MAXNSAP
 * 	[1992/12/04  12:24:41  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:18:49  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:46:11  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsPPsap.c,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 18:24:52 $";
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
/*	@(#)dsPPsap.c	1.12 (Bull S.A)	4/23/92	*/

/**********************************************************************/
/*									  */
/*		dsPPsap.c						  */
/*									  */
/*	contains Presentation Address  parser routines			  */
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
#include <cmx_addr.h>
#include <osiaddr.h>
#include <dsPPsap.h>

static Sel_description sel_info[MAX_NO_SEL]	 = {
	{ T_PSAPINFO, T_PSELVERS, T_PSELSIZE },
	{ T_SSAPINFO, T_SSELVERS, T_SSELSIZE },
	{ T_TSAPINFO, T_TSELVERS, T_TSELSIZE }
};







/*****************************************************************************/
/*																			 */
/*	FUNCTION:  encode_psap_addr	()											 */
/*																			 */
/*	SHORT DESCRIPTION:														 */
/*	  The function encodes a PSAP-address from a presentation-selector,	a	 */
/*	  session-selector and a transport-selector, whereby each of them can	 */
/*	  be given optionally, and one or several (up to MAX_NSAP) NSAP-		 */
/*	  addresses.															 */
/*																			 */
/*	INPUT-PARAMETER:														 */
/*		p_sel		=  Pointer to a	structure (which contains the size of	 */
/*			   the presentation	(P)	selector and a pointer to the	 */
/*					   selector	value) or a	NULL-pointer, if this selector	 */
/*					   isn't present.										 */
/*		s_sel		=  Pointer to a	structure (which contains the size of	 */
/*			   the session (S) selector	and	a pointer to the		 */
/*					   selector	value) or a	NULL-pointer, if this selector	 */
/*					   isn't present.										 */
/*		t_sel		=  Pointer to a	structure (which contains the size of	 */
/*			   the transport (S) selector and a	pointer	to the		 */
/*					   selector	value) or a	NULL-pointer, if this selector	 */
/*					   isn't present.										 */
/*					   NOTE: The size of a selector	value must only	be		 */
/*					   supplied	if the syntax of the selector (see parameter */
/*					   sel_syntax) is of type BINARY. In case of the		 */
/*					   syntaxes	ASCII and ASCHEX the function expects a		 */
/*					   string value	from which the size	can	be determined	 */
/*					   automatically.										 */
/*																			 */
/*		no_nsap		=  No. of the supplied NSAP-address	(1 - MAX-NSAP).		 */
/*		nsap_info	=  Pointer to an array,	whereby	each element of	the		 */
/*					   array contains the size of a	NSAP-address and a		 */
/*					   pointer to the NSAP-value. The no. of array elements	 */
/*					   must	be at least	'no_nsap'.							 */
/*		psap_addr	=  Pointer to the area,	where the encoded PSAP-address	 */
/*					   should be stored.									 */
/*																			 */
/*	OUTPUT-PARAMETER:														 */
/*		*psap_addr	=  Encoded PSAP-address	(valid only, if	the	function	 */
/*					   return-value	is = 0).								 */
/*		*len		=  Size	of the encoded PSAP-address.					 */
/*																			 */
/*	RETURN-VALUE:															 */
/*		OM_SUCCESS	= PSAP-address encoded without any error.				 */
/*		> 0			=  An error	is occurred	during encoding	of the PSAP-	 */
/*					   address (in this	case, the returned value			 */
/*					   corresponds to the type of error).					 */
/*****************************************************************************/

OM_return_code
dsP_encode_psap_addr (
	OMX_workspace	workspace,
	Psap_info *p_sel,
	Psap_info	*s_sel,
	Psap_info *t_sel,
	OM_sint	no_nsap,
	Psap_info *nsap_info,
	xds_psap	*psap_addr,
	OM_sint	*len)
{
	Psap_info		 * sel_ptr[MAX_NO_SEL] ;
	Psap_info		 * nsap_ptr;
	 OM_sint	i, sel_size ;
	struct t_adrdesc *descptr, *desc1ptr ;


	/* verify no. of NSAP-addresses	*/
	if (no_nsap	 < 1	 ||	no_nsap	> T_MAXNSAP)
		return (OM_WRONG_VALUE_NUMBER) ;

	sel_ptr[0] = p_sel ;
	sel_ptr[1] = s_sel ;
	sel_ptr[2] = t_sel ;
	/* evaluate	the	length of the new psap string		*/
	/* and check the size of the selectors and nsaps	*/
	*len = 0;
	for	(i = 0;	i <		MAX_NO_SEL;	i++) {

		if ((sel_ptr[i]	 !=	(Psap_info *	) NULL)	 &&		(sel_ptr[i]->psap_infoval !=
			(char *)NULL)) {
			if (sel_ptr[i]->psap_infolng > sel_info[i].sel_size)
				return (OM_WRONG_VALUE_LENGTH )	;
			*len +=		sizeof (struct t_adrdesc ) +  ((sel_ptr[i]->psap_infolng -
				1) & ~3) + 4;
		}
	}

	for	(i = 0,		nsap_ptr = nsap_info; i	 < no_nsap;	i++,	nsap_ptr++)	 {
		if (nsap_ptr->psap_infolng > T_NSAPSIZE)
			return (OM_WRONG_VALUE_LENGTH) ;
		*len +=		sizeof (struct t_adrdesc ) +  ((nsap_ptr->psap_infolng -
			1) & ~3) + 4 ;
	}

	/* allocate	the	psap string	*/
	if ((*psap_addr	 = (xds_psap) OMP_XALLOC(workspace,	AL_STRING, ROUNDUP(workspace,
			*len + 1)))	 ==	0)
		return(OM_MEMORY_INSUFFICIENT);
	memset(*psap_addr, 0, (*len) + 1);


	descptr	 = (struct t_adrdesc *)	 * psap_addr;

	for	(i = 0;	i <		MAX_NO_SEL + no_nsap; i++) {
		if (i <		MAX_NO_SEL)	 {
			/* encode selector(s) into PSAP-address	*/
			if ((sel_ptr[i]	 !=	(Psap_info *	) NULL)	 &&	 (sel_ptr[i]->psap_infoval !=
				(char *)NULL))	 {
				sel_size = sel_ptr[i]->psap_infolng;
				/* initialize selector area	*/
				memset (descptr	 + 1, 0,	((sel_size -  1) &
					~3)	+ 4) ;

				/* encode selector value into PSAP-address */
				memcpy ((char *)(descptr + 1), (char *)sel_ptr[i]->psap_infoval,
					 sel_size) ;

				/* write selector header into PSAP-address */
				descptr->t_infotype	 = sel_info[i].sel_type	;
				descptr->t_infovers	 = sel_info[i].sel_vers	;
				descptr->t_infolng = sel_size ;
				descptr->t_translng	 = descptr->t_nextoff =
					sizeof (struct t_adrdesc ) + ((sel_size	 -
					1) &  ~3)	 + 4	;
			} else {
				continue ;
			} /* endif */

		} else {

			/* encode NSAP-address value into PSAP-address */
			memcpy (descptr	 + 1, nsap_info->psap_infoval, nsap_info->psap_infolng)	;
			/* write NSAP-address header into PSAP-address */
			descptr->t_infotype	 = T_NSAPINFO ;
			descptr->t_infovers	 = T_NSAPVERS ;
			descptr->t_infolng = nsap_info->psap_infolng ;
			descptr->t_translng	 = descptr->t_nextoff =	 sizeof	(struct	t_adrdesc )	+
				((nsap_info->psap_infolng
				-				1) & ~3) + 4 ;
			nsap_info++;
		}
		/* adjust PSAP-address header */
		desc1ptr = (struct t_adrdesc *)	 * psap_addr	;
		while (desc1ptr	 < descptr)	{
			desc1ptr->t_translng +=		descptr->t_translng	;
			desc1ptr = (struct t_adrdesc *)
			((char *) desc1ptr + desc1ptr->t_nextoff) ;
		}

		descptr	 = (struct t_adrdesc *)
		((char *) descptr +		descptr->t_nextoff)	;
	}

	return(OM_SUCCESS);
} /* end dsP_encode_psap_addr */


/*****************************************************************************/
/*																			 */
/*	FUNCTION:  dsP_decode_psap_addr	()										 */
/*																			 */
/*	SHORT DESCRIPTION:														 */
/*	  The function decodes a PSAP-address and returns the selector value(s)	 */
/*	  and also the available NSAP-address values.							 */
/*																			 */
/*	INPUT-PARAMETER:														 */
/*		p_sel		=  Pointer to a	structure which	contains the pointer to	 */
/*					   the location	at which the decoded presentation (P)	 */
/*					   selector	value should be	stored.						 */
/*		s_sel		=  Pointer to a	structure which	contains the pointer to	 */
/*					   the location	at which the decoded session (S)		 */
/*					   selector	value should be	stored.						 */
/*		t_sel		=  Pointer to a	structure which	contains the pointer to	 */
/*					   the location	at which the decoded transport (T)		 */
/*					   selector	value should be	stored.						 */
/*					   NOTE: The reference to the selector value inside	of	 */
/*					   the PSAP-address	instead	of the selector	value it-	 */
/*					   self	is returned	if the given memory	reference is a	 */
/*					   NULL-pointer. This will be only done	if the requested */
/*					   syntax of the selector (see parameter sel_syntax) is	 */
/*					   BINARY.												 */
/*		no_nsap		=  Pointer to a	variable where the no. of decoded NSAP-	 */
/*					   addresses should	be stored.							 */
/*		nsap_info	=  Pointer to an array which contains pointer to the	 */
/*					   location	at which the decoded NSAP-address values	 */
/*					   should be stored.									 */
/*					   NOTE: The reference to the NSAP-address value inside	 */
/*					   of the PSAP-address instead of the NSAP-address		 */
/*					   value itself	is returned	if the given memory			 */
/*					   reference is	a NULL-pointer.							 */
/*																			 */
/*		psap_addr	=  Pointer to the PSAP-address which should	be decoded.	 */
/*																			 */
/*	OUTPUT-PARAMETER:														 */
/*		*p_sel		=  Pointer to the decoded P-selector value or a	NULL-	 */
/*					   pointer,	if the selector	isn't present. Size	of the	 */
/*					   decoded P-selector.									 */
/*		*s_sel		=  Pointer to the decoded S-selector value or a	NULL-	 */
/*					   pointer,	if the selector	isn't present. Size	of the	 */
/*					   decoded P-selector.									 */
/*		*t_sel		=  Pointer to the decoded T-selector value or a	NULL-	 */
/*					   pointer,	if the selector	isn't present. Size	of the	 */
/*					   decoded P-selector.									 */
/*		*no_nsap	=  No. of the decoded NSAP-addresses.					 */
/*		*nsap_info	=  Size	of each	decoded	NSAP-address value and the		 */
/*					   pointers	to the locations, where	the	NSAP-values	are	 */
/*					   stored.												 */
/*	RETURN-VALUE:															 */
/*		0		= PSAP-address decoded without any error.				 */
/*		< 0			= An error is occurred during decoding of the PSAP-		 */
/*					  address (in this case, the returned value				 */
/*					  corresponds to the type of error).					 */
/*****************************************************************************/
OM_sint
dsP_decode_psap_addr (
	Psap_info	*p_sel,
	Psap_info *s_sel,
	Psap_info	*t_sel,
	OM_sint *no_nsap,
	Psap_info *nsap_info,
	xds_psap psap_addr)
{
	Psap_info		 * sel_ptr[MAX_NO_SEL] ;
	 OM_sint	i,  sel_size ;
	struct t_adrdesc *descptr ;

	descptr	 = (struct t_adrdesc *)	psap_addr ;
	*no_nsap = 0 ;

	/* decode selector(s) from PSAP-address	*/
	sel_ptr[0] = p_sel ;
	sel_ptr[1] = s_sel ;
	sel_ptr[2] = t_sel ;
	for	(i = 0;	i <		MAX_NO_SEL;	i++) {
		if (descptr->t_infotype	 ==	sel_info[i].sel_type) {
			/* verify the size of the selector */
			if ((sel_size =		descptr->t_infolng)	 > sel_info[i].sel_size)
				return (-1 ) ;

			/* return only reference to	the	selector */
			/* value inside	of the PSAP-address	*/
			sel_ptr[i]->psap_infoval = (char *)	(descptr +	1) ;
			sel_ptr[i]->psap_infolng = sel_size	;
			descptr	 = (struct t_adrdesc *)
			((char *) descptr +		descptr->t_nextoff)	;
		} else {
			sel_ptr[i]->psap_infoval = (char *)	NULL ;
			sel_ptr[i]->psap_infolng = 0 ;
		}
	}

	/* decode NSAP-address(es) from	PSAP-address */
	while (*no_nsap	 < T_MAXNSAP) {
		if (descptr->t_infotype	 !=	T_NSAPINFO)
			return (-1)	;
		if (descptr->t_infolng > T_NSAPSIZE)
			return (-1)	;

		/* return only reference to	the	NSAP-address */
		/* value inside	of the PSAP-address	*/
		nsap_info->psap_infoval	 = (char *)	(descptr	 + 1) ;
		nsap_info->psap_infolng	 = descptr->t_infolng ;
		(*no_nsap)++;
		nsap_info++;

		if (descptr->t_translng	 > descptr->t_nextoff) {
			descptr	 = (struct t_adrdesc *)
			((char *) descptr +		descptr->t_nextoff)	;
		} else {
			break ;
		} /* endif */
	}

	return ((*no_nsap >		0 && *no_nsap <= T_MAXNSAP)	 ? 0	 : -1) ;
} /* end dsP_decode_psap_addr */


/*****************************************************************************/
/*																			 */
/*	FUNCTION:  dsP_get_psap_length ( )										 */
/*																			 */
/*	SHORT DESCRIPTION:														 */
/*										 */
/*	dsP_get_psap_length	() returns the total length	of				 */
/*		a psap address							 */
/*										 */
/*	INPUT-PARAMETER:														 */
/*		psap_addr	=  Pointer to the PSAP-address				 */
/*										 */
/*	OUTPUT-PARAMETER:														 */
/*	>0		= psap length						 */
/*****************************************************************************/
OM_sint
dsP_get_psap_length(
	xds_psap psap_addr)
{
	struct t_adrdesc *descptr ;

	if (psap_addr) {
		descptr	 = (struct t_adrdesc *)	psap_addr ;
		return(descptr->t_translng);
	} else {
		return(0);
	} /* endif */
} /* end dsP_get_psap_length */
