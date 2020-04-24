/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: absurd.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:11  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:05:35  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/28  23:54 UTC  dtruong
 * 	Merge to DCE 1.1
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/27  21:33  dtruong
 * 	merge kk and hpdce01
 * 	[1995/12/08  14:47:03  root]
 * 
 * Revision 1.1.4.5  1993/12/17  21:32:43  jake
 * 	GDS What string changes
 * 	[1993/12/17  21:32:28  jake]
 * 
 * Revision 1.1.4.4  1993/12/17  21:29:39  jake
 * 	GDS What string changes
 * 	[1993/12/17  21:27:31  jake]
 * 
 * Revision 1.1.4.3  1993/12/17  21:22:50  jake
 * 	GDS What string changes
 * 	[1993/12/17  21:11:23  jake]
 * 
 * Revision 1.1.4.2  1993/12/17  21:05:36  jake
 * 	GDS What string changes
 * 	[1993/12/17  21:04:00  jake]
 * 
 * 	GDS What string changes
 * 	[1993/12/17  21:02:44  jake]
 * 
 * Revision 1.1.2.5  1993/10/19  18:09:06  root
 * 	    Loading
 * 
 * Revision 1.1.6.2  1993/08/11  11:15:37  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:56:41  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:15:37  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:56:41  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:33:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:02  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:13:39  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:12:37  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$Id: absurd.c,v 1.1.10.2 1996/02/18 19:37:22 marty Exp $";
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

#include <dce/asn1.h>
/********************\
* 		     *
* Absurd ASN.1 type  *
* 		     *
\********************/
/*ARGSUSED*/
asn1 asn1_absurd_cod(asn1z,n,m,x)
asn1 asn1z;
void *x;
{
	return(asn1_null_cod(asn1z,n,m));
}

/*ARGSUSED*/
int asn1_absurd_len(n,x)
int n;
void *x;
{
	return(asn1_null_len(n));
}

asn1 asn1_absurd_dec(asn1z,asn1zm,x)
asn1 asn1z, asn1zm;
void **x;
{
	*x = 0;
	return(asn1_errdec(asn1z,ASN1_ERR_ABSURD));
}

/*ARGSUSED*/
asn1 asn1_absurd_lwc(asn1z,x)
asn1 asn1z;
void *x;
{
	return(asn1_null_lwc(asn1z));
}

asn1 asn1_absurd_lwd(asn1z,asn1zm,x)
asn1 asn1z, asn1zm;
void **x;
{
	*x = 0;
	/* SIGNAL ERROR */
	return(asn1_errdec(asn1z,ASN1_ERR_ABSURD));
}

/*ARGSUSED*/
asn1_string asn1_absurd_out(asn1z,x)
asn1_string asn1z;
void *x;
{
        return(asn1_null_out(asn1z));
}

/*ARGSUSED*/
int asn1_absurd_olen(x)
void *x;
{
	return(asn1_null_olen());
}

/*ARGSUSED*/
asn1_string asn1_absurd_in(asn1z,asn1zm,x)
asn1 asn1z, asn1zm;
void **x;
{
	*x = 0;
	return(asn1_err_in(asn1z,ASN1_ERR_ABSURD));
}

/*ARGSUSED*/
void  asn1_absurd_free(x)
void *x;
{
	return;
}

/*ARGSUSED*/
void asn1_absurd_err(x)
void **x;
{
	*x = 0;
	return;
}

/*ARGSUSED*/
void asn1_absurd_cpy(x, y)
void **x, *y;
{
	*x = 0;
	return;
}
 

static asn1_type_desc asn1_absurd_type_value = {
	asn1_absurd_cod,
	asn1_absurd_len,
	asn1_absurd_dec,
	asn1_absurd_lwc,
	asn1_absurd_lwd,
	asn1_absurd_out,
	asn1_absurd_olen,
	asn1_absurd_in,
	asn1_absurd_free,
	asn1_absurd_err,
	asn1_absurd_cpy};

asn1_type_desc * asn1_absurd_type = &asn1_absurd_type_value;
