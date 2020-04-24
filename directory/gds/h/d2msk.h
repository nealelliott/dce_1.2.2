/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2msk.h,v $
 * Revision 1.1.10.2  1996/02/18  23:36:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:28  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:01:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:17  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:47:19  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:41:54  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:56:01  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:38:48  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:18:35  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:00  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:53:27  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:48:01  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:55:12  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:43:22  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:42:08  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:30:32  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:11:51  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  09:33:53  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  04:12:34  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:43:23  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D2MSK_H
#define _D2MSK_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d2msk_rcsid[] = "@(#)$RCSfile: d2msk.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:36:07 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton****************************************************************/
/*								      */
/* TYP		: INCLUDE					      */
/*								      */
/* NAME         : d2msk.h                                             */
/*								      */
/* AUTOR        : Volpers                                             */
/* DATUM        : 07.02.1991                                          */
/*								      */
/* KOMPONENTE	: DS						      */
/*								      */
/* DOK.-NR.	: DS-Designspezifikation (Ausgabe 1)		      */
/*								      */
/* PRD#/VERS.	:						      */
/*								      */
/* DESCRIPTION  : contains the typedefs for the data structure        */
/*                where the values from the masks will be written.    */
/*								      */
/* SYSTEMABHAENGIGKEIT:	 COMMON					      */
/*								      */
/*								      */
/* Vers.Nr. |  Datum  |	 Aenderungen		       | KZ | CR# FM# */
/*   1.0    | 07.02.91| Original                       |    |         */
/*datoff***************************************************************/

/* -------------------- function key constants ----------------------------- */

#define D20_MFKTKEYS	MAXFKT  /* max. no. of active function keys */
#define D20_MARKFKT	'x'	/* character used to mark a pressed function key */

/* ------------------------ mask operation codes --------------------------- */

#define D20_NO_OP	0x00	/* no operation code */
#define	D20_CSAY	0x01 	/* csay ()-function code */
#define D20_CGET	0x02	/* cget ()-function code */
#define D20_FKTX	0x04	/* cgetfkt ()-function code */
#define D20_EOMSK	0x08	/* end of mask code */
#define D20_NOERASE	0x80	/* don't erase screen before output the mask */
#define D20_MAX_LEADM      5    /* maximum number of masks to display after  */
				/* <FKTHELP> was pressed		     */


typedef struct {
	signed16   d20_fkt_id;
	signed16   d20_attribute;
	signed16   d20_line_no;
	signed16   d20_column_no;
	char	  *d20_ioinfo;
	char	  *d20_oformat;
	signed16  d20_iolen;
	Bool 	  d20_conv;
	Bool	  d20_next;
	} D20_omask;


typedef struct {
	signed16   d20_otype ;
	char	  *d20_msgptr ;
	} D20_opmsg ;


typedef struct
	{   D20_omask *mask;        /* mask which was displayed              */
            signed32    msg_set;    /* read a specific message set	     */
            signed32    number;     /* number of masks to read		     */
	    D20_omask *n_mask[D20_MAX_LEADM];   
				    /* sequence of masks to display          */
                                    /* (D20_omask *) 0 if last mask is	     */
				    /* displayed                             */
	} D20_help_array;

#define	D20_MAX_T61_MASK_FV	(24 * 80 * D20_T61_CONV_FACT)	
					/* maximum no. of T61 chars required */
					/* to store a mask field value.	     */

#define	D20_MAX_LOC_MASK_FV	(24 * 80)
					/* maximum no. of chars required to  */
					/* store a mask field value in local */
					/* character format.	  	     */

#endif /* _D2MSK_H */
