/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20lngset.c,v $
 * Revision 1.1.11.2  1996/02/18  19:42:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:10  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:21:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:01  root]
 * 
 * Revision 1.1.9.2  1994/03/23  15:07:05  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:09  keutel]
 * 
 * Revision 1.1.9.1  1994/02/22  17:35:57  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:38:45  marrek]
 * 
 * Revision 1.1.7.2  1993/08/10  11:05:32  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:42:16  marrek]
 * 
 * Revision 1.1.5.4  1992/12/31  18:11:11  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:07:56  bbelch]
 * 
 * Revision 1.1.5.3  1992/12/17  23:12:20  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:07:40  tom]
 * 
 * Revision 1.1.5.2  1992/11/27  15:41:04  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:47:06  marrek]
 * 
 * Revision 1.1.3.3  1992/06/26  07:03:51  jim
 * 	Merge AIX 3.2 changes with the latest level.
 * 	[1992/06/22  21:25:17  jim]
 * 
 * Revision 1.1.3.2  1992/06/01  20:02:11  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:46:07  melman]
 * 	Revision 1.1.1.2  1992/05/29  19:02:24  jim
 * 	Added setlocale calls for AIX 3.2
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20lngset.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 19:42:24 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20language.c                                       */
/*								      */
/* AUTHOR       : Fischer, D AP 11                                    */
/* DATE         : 27.07.89                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/*datoff **************************************************************/

/*****************************************************************************/
/*    P R E P R O C E S S O R   I N S T R U C T I O N S                      */
/*****************************************************************************/

#include <gds.h>

#include <stdio.h>
#include <locale.h>
#include <nl_types.h>
#include <stdlib.h>

#include <d20proto.h>


/*****************************************************************************/
/*     D E F I N I T I O N S                                                 */
/*****************************************************************************/

int main(int argc, char *argv[])
{
signed32    msg_set;
signed32    msg_no;
char       *res;
char       *def_str = "DEFAULT";
nl_catd     nl_fd;

msg_set = atoi(argv[2]);
msg_no = atoi(argv[3]);

setlocale(LC_ALL,"");
if ((nl_fd = catopen(argv[1], 0)) == (nl_catd) -1)
{   fprintf(stderr, "catopen() failed\n");
    exit(1);
}

if ((res = catgets(nl_fd, msg_set, msg_no, def_str)) == def_str)
{   fprintf(stderr, "Error while reading NLS file; msg_set = %d msg_no = %d\n",
	    msg_set, msg_no);
    exit(1);
}

fprintf(stdout,"%s",res);
catclose(nl_fd);
exit(0);
}
