/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27obid.h,v $
 * Revision 1.1.10.2  1996/02/18  23:35:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:05  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:00:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:57  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:51:09  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:45:51  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:42:57  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:40:51  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:41:08  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:29:46  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  04:06:15  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:42:33  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D27OBID_H
#define _D27OBID_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d27obid_rcsid[] = "@(#)$RCSfile: d27obid.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:35:53 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

#ifdef PERMANENT
#define MAX_BUF_LEN           D2_LOBJ_CLASS
#define Boolean int

typedef char * string;

typedef struct {
	int		nb;
	unsigned char   * contents;
} octet_string;
#endif

typedef struct
	{
	  signed32    d2_nb_el;
	  unsigned32  *d2_subids;
	} d2t_oid;

/* Two representations are acceptable at the DUA to handle
 * the object identifiers:
 * . as a list of long integers,
 * . as an octets string already containing the ASN1 encoded ``V'' field
 */
typedef struct
	{
	  boolean   is_oid_ints;
	  union
	  {
	    asn1_oid    oid_asn1;
	    d2t_oid     oid_ints;
	  } contents;
	} dua_oid;

/*
 * definition of the attribute table
 */
typedef struct
	{
	  char *name;   /* att name used at the user interface        */
			/* index of the syntax of the values          */
	  signed32 syntax;
	  asn1_oid oid; /* object identifier (length, value)          */
	} att_typ;



#endif /* _D27OBID_H */
