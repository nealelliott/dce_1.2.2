/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndslog.c,v $
 * Revision 1.1.180.2  1996/02/18  18:20:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:15  marty]
 *
 * Revision 1.1.180.1  1995/12/08  16:06:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:41  root]
 * 
 * Revision 1.1.178.3  1994/09/06  12:26:02  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:17:59  keutel]
 * 
 * Revision 1.1.178.2  1994/06/09  18:47:10  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:20  devsrc]
 * 
 * Revision 1.1.178.1  1994/02/22  16:10:33  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:58:40  marrek]
 * 
 * Revision 1.1.176.2  1993/10/14  17:36:04  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:34:51  keutel]
 * 
 * Revision 1.1.176.1  1993/10/13  17:33:33  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  16:51:05  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndslog.c,v $ $Revision: 1.1.180.2 $ $Date: 1996/02/18 18:20:36 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    module                                                         */
/*                                                                           */
/*   NAME:    ndslog.c                                                       */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains all logging resp. debugging functions used     */
/*       by the NDS-runtime functionality.                                   */
/*                                                                           */
/*       The following functions are available:                              */
/*                                                                           */
/*          * nds_log     - write logging information                        */
/*          * nds_dbg     - write debugging information                      */
/*          * nds_hexdump - write binary information as hex dump             */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 27.5.93     */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <ctype.h>
#include <ndscfg.h>

/* ------------------------------------------------------------------------- */

signed32 nds_log (char *file,		/* >  source file name */
		  signed32  line,	/* >  source file line number */
		  signed32  retval) {	/* >  error value */

    /* print logging message */
    fprintf (stderr, "%s: abort at line %d (return value = %d)\n", file, line,
								retval) ;
    fflush (stderr) ;
    return (retval) ;
}

/* ------------------------------------------------------------------------- */

signed32 nds_dbg (char *comment,	/* >  comment */
		  byte *addr,		/* >  reference to binary data */
		  signed32  size) {	/* >  size of binary data */

#ifdef IODEBUG
	if (comment)
	    fprintf (stderr, "\n(pid = %d): %s\n", getpid(), comment) ;
	nds_hexdump (stderr, addr, size) ;
	fflush (stderr) ;
#endif
	return ;
}

/* ------------------------------------------------------------------------- */

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  nds_hexdump ()                                                */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function writes a message in a predefined format to a file.      */
/*      The format is as follows (one line shown):                           */
/*                                                                           */
/*                             . . .                                         */
/*  xxx   41 42 43 44 45 46 47 01 02 4a 4b 4c 4d 4e 4f 50 |ABCDEFG  JKLMNOP| */
/*                             . . .                                         */
/*                                                                           */
/*      All values of the message are represented in HD-format and ASCII-   */
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

void nds_hexdump (FILE *fd, byte *p, signed32 size) {

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
