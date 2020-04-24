/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27prmsg.c,v $
 * Revision 1.1.10.2  1996/02/18  18:23:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:49  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:16:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:32  root]
 * 
 * Revision 1.1.8.3  1994/07/06  15:08:51  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:23:21  marrek]
 * 
 * Revision 1.1.8.2  1994/05/10  16:03:39  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:13:17  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  19:21:55  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:54:41  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:34:58  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  13:13:38  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  22:05:45  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:49:04  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  19:45:39  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:52:39  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:44:31  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27prmsg.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:23:51 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d27_printmsg ()                                               */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function writes a message in a predefined hex-format to a file.  */
/*      The format is as follows (one line shown):                           */
/*                                                                           */
/*                             . . .                                         */
/*  xxx   41 42 43 44 45 46 47 01 02 4a 4b 4c 4d 4e 4f 50 |ABCDEFG  JKLMNOP| */
/*                             . . .                                         */
/*                                                                           */
/*      All values of the message are represented in HD-format and ASCII-    */
/*      format (only if a value is in the range 0x20 - 0x7F, otherwise a     */
/*      blank is printed).                                                   */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      fd          = Descriptor of the file to which the message should be  */
/*                    written.                                               */
/*      p           = Pointer to the beginning of the message.               */
/*      size        = Size of the message.                                   */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    None                                                   */
/*                                                                           */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <ctype.h>

/*
 * Prototyping
 */
void d27_printmsg (FILE *fd, byte *p, signed32 size);

/* ------------------- DEFINITION OF CONSTANTS --------------------------- */

					/* hex dump printing formats */
#define HD_MVAL			16	/* # of hex-values per line */
#define HD_P1CNT		"%6X  "	/* size counting format */
#define HD_P2PRES		"%02X "	/* character presentation format */
#define HD_P3FILL		"   "	/* character filler format */
#define HD_P4DELS		"    |"	/* delimiter format (start of printable character area) */
#define HD_P5CHAR		"%c"	/* printable character format */
#define HD_P6NOCHAR		" "	/* no printable character format */
#define HD_P7DELE		"|\n"	/* delimiter format (end of printable character area) */

/* ------------------------------------------------------------------------- */

void d27_printmsg (FILE *fd, byte *p, signed32 size) {

    signed32	size1, size2 ;
    byte 	*p1 = p, *p2 = p + size ;

    if ((size2 = size % HD_MVAL) != 0)
	size += HD_MVAL - size2 ;
    fprintf (fd, HD_P1CNT, 0) ;
    for (size1 = 0; size1 <= size; size1++) {
	if (size1 > 0 && size1 % HD_MVAL == 0) {
	    fprintf (fd, HD_P4DELS) ;
	    for (size2 = 0; size2 < HD_MVAL; size2++, p1++)
		if (p1 < p2 && isprint ((int) *p1))
		    fprintf (fd, HD_P5CHAR, *p1) ;
		else
		    fprintf (fd, HD_P6NOCHAR) ;
	    fprintf (fd, HD_P7DELE) ;
	    if (size1 == size)
		break ;
	    fprintf (fd, HD_P1CNT, size1) ;
	}
	if (p < p2)
	    fprintf (fd, HD_P2PRES, *p++ & 0xFF) ;
	else
	    fprintf (fd, HD_P3FILL) ;
    }
}
