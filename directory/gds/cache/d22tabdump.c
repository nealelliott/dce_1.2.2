/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22tabdump.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:01  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:28:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:35  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:38  marrek
 * 	February 1994 code drop
 * 	[1994/02/17  16:42:21  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:55:05  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:47:04  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:15:46  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:41:56  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:14:16  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:31:24  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:38:16  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22tabdump.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:59 $";
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
/* NAME         : d22tabdump.c                                        */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 10.05.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d22_tab_dump ()                                               */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function (signal routine) dumps the informations of the          */
/*      following data structures contained in the DUA-cachehandler process: */
/*                                                                           */
/*            - D22_admin_cache  (DUA-cache administration table)            */
/*            - D22_desc_cache   (DUA-cache description table)               */
/*            - D22_config_cache (DUA-cache configuration table)             */
/*            - D22-hash_entry   (DUA-cache hash table)                      */
/*            - D22_iobj_entry   (DUA-cache 'internally stored'-table)       */
/*                                                                           */
/*      The informations are written in a special 'dump'-file in hex-format  */
/*      (in the order listed above). Evaluation of this informations must    */
/*      be done by a separate program. Dumping is only be done, if there     */
/*      exists an active DUA-cache. The dumping operation is interrupted     */ 
/*      without an error message if there is any error detected during       */
/*      this operation.                                                      */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*                    NONE                                                   */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    NONE                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    NONE                                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 25.11.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <dce/d2dir.h>
#include <d22cache.h>

static char sccsid[] = { "@(#)d22tabdump.c	4.3 91/07/08 (D AP 11)" } ;

/* --------------------- declaration of global data ------------------------ */

extern D22_admin_tab	d22_admtab ;

/* ------------------------------------------------------------------------- */

void d22_tab_dump () {
	int	fd ;
	char	fname[D27_LFILE_NAME] ;

	signal (SIGUSR2, d22_tab_dump) ;

	/* look for active DUA-cache */
	if (d22_admtab.d22_act_cache_id == D22_NONE)
		return ;

	/* open DUA-cache dump file */
	sprintf (fname, "%s%s%d", d22_admtab.d22_conftab->d22_base_path,
				  D22_DUMPFILE, d22_admtab.d22_act_cache_id) ;
	if ((fd = open (fname, O_WRONLY | O_CREAT | O_TRUNC, D22_FILEMODE)) == D22_ERROR)
		return ;

	/* dump DUA-cache administration information */
	if (write (fd, &d22_admtab, sizeof (D22_admin_tab)) == D22_ERROR) {
		close (fd) ;
		return ; 
	}

	/* dump DUA-cache description table */
	if (write (fd, d22_admtab.d22_desctab, sizeof (D22_desc_tab)) == D22_ERROR) {
		close (fd) ;
		return ; 
	}

	/* dump DUA-cache configuration table -> default values */
	if (write (fd, d22_admtab.d22_conftab, sizeof (D22_config_tab)) == D22_ERROR) {
		close (fd) ;
		return ; 
	}

	/* dump DUA-cache hash table */
	if (write (fd, d22_admtab.d22_hshtab, sizeof (D22_hash_entry) *
		   d22_admtab.d22_desctab->d22_hmax_entries) == D22_ERROR) {
		close (fd) ;
		return ; 
	}

	/* dump DUA-cache 'internally stored' table */
	write (fd, d22_admtab.d22_iobjtab, sizeof (D22_iobj_entry) *
	       d22_admtab.d22_desctab->d22_imax_entries) ;
	close (fd) ;
}
