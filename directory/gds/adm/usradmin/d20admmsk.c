/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20admmsk.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:07  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:21:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:59  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:43:56  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:25:25  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:18  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:14:39  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:03  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:04  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:52  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:38:28  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  11:01:43  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:41:43  marrek]
 * 
 * Revision 1.1.4.4  1993/02/04  08:40:19  marrek
 * 	help_mask array corrected.
 * 	[1993/02/03  08:42:36  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:11:04  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:07:44  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:40:28  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  09:14:42  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:01:51  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:45:51  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20admmsk.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:21 $";
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
/* NAME         : d20admmsk.c                                         */
/*								      */
/* AUTHOR       : Fischer, D AP 11                                    */
/* DATE         : 02.02.89                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  : Mask Definition Module                              */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*  0.1     |02.02.89 |  original                      |    |         */
/*datoff **************************************************************/

/*****************************************************************************/
/*    P R E P R O C E S S O R   I N S T R U C T I O N S                      */
/*****************************************************************************/

#include <gds.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d2msk.h>
#include <cget.h>
#include <d20_adm.h>


/*****************************************************************************/
/*     D E F I N I T I O N S                                                 */
/*****************************************************************************/

char    wahl[80] ;          /* in Login-Menuemask 1 */

			    /* arrays to suply gdsconfig */
char	*update[] = { "-", "'*'" } ;
char	*conftyp[] = { "Client-System", "Clt/Srv-System" } ;
char	*dbtype[] = { "-", "C-isam", "informix" } ;
char	*m_format[] = { "no", "yes" } ;

signed32 no_server = 2 ;     /* number of server-processes */
signed32 no_client = 16 ;    /* number of client proceses */
signed32 sprache ;           /* language */
signed32 m_conf_mode = 1 ;   /* configuration mode */
signed32 m_conf_typ ;        /* configuration type */
signed32 m_medium ;          /* storagemedium */
signed32 m_update ;          /* distribute update information */
signed32 m_dir_id = 1 ;      /* directory identifier */
signed32 m_authmech = D2_SIMPLE ;      /* security mechanism */
signed32 m_dbtype = 1 ;      /* database system */
signed32 m_form ;	     /* format medium ? */
signed32 m_opmode ;          /* operation code */
signed32 m_volumeno = 1 ;    /* number of the volumes */
Bool     m_print_err ;       /* flag for printing message */
			    /* user key (save and restore) */
char    m_userkey[D20_LKEY+1] = "__________";
char    m_filename[D20_LFILE+1] = "_____________________________________________";

/*****************************************************************************/
/*     1.Login-Menuemask                                                     */
/*****************************************************************************/

D20_omask   diradm_mask1[] = {
D20_NO_OP, INVERS, 24,43, "NOTE: Delta-update operation failed !",           NULL, 0, FALSE, FALSE,
D20_CSAY,  NORMAL,  2, 5, "diradm",                                          NULL, 0, FALSE, FALSE,
D20_CSAY,  INVERS,  2,32, "DIRECTORY SYSTEM V2.1",                                NULL, 0, FALSE, FALSE,
D20_CSAY,    WAHL,  5,14, "a - Administration of a directory system",        NULL, 0, FALSE, FALSE,
D20_CSAY,    WAHL,  7,14, "c - Configuration of a directory system",         NULL, 0, FALSE, FALSE,
D20_CSAY,    WAHL,  9,14, "A - Activation of a directory system",            NULL, 0, FALSE, FALSE,
D20_CSAY,    WAHL, 11,14, "d - Deactivation of a directory system",          NULL, 0, FALSE, FALSE,
D20_CSAY,    WAHL, 13,14, "s - Saving of local data on a floppy disk/streamer tape", NULL, 0, FALSE, FALSE,
D20_CSAY,    WAHL, 15,14, "r - Restoring of local data from a floppy disk/streamer tape", NULL, 0, FALSE, FALSE,
D20_CSAY,    WAHL, 17,14, "f - Further functions",                           NULL, 0, FALSE, FALSE,
D20_CSAY,  NORMAL, 23, 2, "Your selection! > ",                              NULL, 0, FALSE, FALSE,
D20_CGET, WAHLERG, 23,20, wahl,                                              "%1s", 0, FALSE, FALSE,
D20_FKTX,  FKTEND,  0, 0, NULL,                                           NULL, 0, FALSE, FALSE,
D20_FKTX, FKTMENU,  0, 0, NULL,                                           NULL, 0, FALSE, FALSE,
D20_FKTX,   FKTCR,  0, 0, NULL,                                           NULL, 0, FALSE, FALSE,
D20_FKTX, FKTHELP,  0, 0, NULL,                                           NULL, 0, FALSE, FALSE,
D20_EOMSK };

/*****************************************************************************/
/*     2.Login-Menuemask                                                     */
/*****************************************************************************/

D20_omask   diradm_mask2[] = {
 D20_CSAY,   NORMAL,  2, 5,  "diradm",                                       NULL, 0, FALSE, FALSE,
 D20_CSAY,   INVERS,  2,32,  "DIRECTORY SYSTEM V2.1",                             NULL, 0, FALSE, FALSE,
 D20_CSAY,   WAHL,    9,14,  "i - Display of directory system state informations", NULL, 0, FALSE, FALSE,
 D20_CSAY,   WAHL,   11,14,  "T - Activation of the 'trace'-system",         NULL, 0, FALSE, FALSE,
 D20_CSAY,   WAHL,   13,14,  "t - Deactivation of the 'trace'-system",       NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL, 23, 2,  "Your selection! > ",                           NULL, 0, FALSE, FALSE,
 D20_CGET,   WAHLERG,23,20,  wahl,                                           "%1s", 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTEND,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTMENU, 0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,  FKTHELP,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };

/*****************************************************************************/
/*     Mask to change language 						     */
/*****************************************************************************/

D20_omask   menupar_mask[] = {
 D20_CSAY,   NORMAL,  2, 5,  "menupar",                                      NULL, 0, FALSE, FALSE,
 D20_CSAY,   INVERS,  2,32,  "DIRECTORY SYSTEM",                             NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  2,61,  "Parameters",                                   NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL, 14,12,  "Which language do you prefer ?:",              NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL, 14,44,  (char *)&sprache,                               NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTEND,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTMENU, 0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };

/*****************************************************************************/
/*     Mask to display directory system statusinformation		     */
/*****************************************************************************/

D20_omask   info_mask[] = {
 D20_CSAY,   NORMAL,  2, 5,  "info",                                         NULL, 0, FALSE, FALSE,
 D20_CSAY,   INVERS,  2,32,  "DIRECTORY SYSTEM",                             NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  2,61,  "State information",                            NULL, 0, FALSE, FALSE,
 D20_EOMSK };

D20_omask   info1_mask[] = {
 D20_CSAY | D20_NOERASE,
             NORMAL, 24, 2,  "To continue please press CR",                  NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,  0, 0,  NULL,                                         NULL, 0, FALSE, FALSE,
 D20_EOMSK };

/*****************************************************************************/
/*     Mask to configure the directory system				     */
/*****************************************************************************/

D20_omask   confpar_mask[] = {
 D20_CSAY,   NORMAL,  2, 5,  "confpar",                                      NULL, 0, FALSE, FALSE,
 D20_CSAY,   INVERS,  2,32,  "DIRECTORY SYSTEM",                             NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  2,64,  "Configuration",                                NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  5, 8,  "Which configuration mode ?:",                  NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL,  5,36,  (char *)&m_conf_mode,                           NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTMENU, 0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTHELP, 0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };

/*****************************************************************************/
/*     1. Overlapping mask to configure the directory system		     */
/*****************************************************************************/

D20_omask   conf1_mask[] = {
 D20_CSAY | D20_NOERASE,
	     NORMAL,  7, 8,  "for which directory identifier should the operation", NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  8, 8,  "be done ? [1-20]:",                            NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL,  8,26,  (char *)&m_dir_id,                              NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTMENU, 0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,  FKTHELP,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };

/*****************************************************************************/
/*     1d. Overlapping mask to configure (delete) the directory system	     */
/*****************************************************************************/

D20_omask   conf1d_mask[] = {
 D20_CSAY | D20_NOERASE,
	     NORMAL,  7, 8,  "for which directory identifier should the operation", NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  8, 8,  "be done ? [2-20]:",                            NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL,  8,26,  (char *)&m_dir_id,                              NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTMENU, 0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,  FKTHELP,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };

/*****************************************************************************/
/*     2. Overlapping mask to configure the directory system		     */
/*****************************************************************************/

D20_omask   conf2_mask[] = {
 D20_CSAY | D20_NOERASE,
	     NORMAL, 10, 8,  "Which configuration type ?:",                  NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL, 10,36,  (char *)&m_conf_typ,                            NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL, 12, 8,  "How many clients have access to the directory",NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL, 13, 8,  "system at the same time ? [1-128]:",           NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL, 13,43,  (char *)&no_client,                             "%-3d", 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTMENU, 0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,  FKTHELP,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };

/*****************************************************************************/
/*     3. Overlapping mask to configure the directory system		     */
/*****************************************************************************/

D20_omask   conf3_mask[] = {
 D20_CSAY | D20_NOERASE,
	     NORMAL, 15, 8,  "How many server-processes should be established ? [1-64]:", NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL, 15,67,  (char *)&no_server,                             "%-3d", 0, FALSE, FALSE,
 D20_CSAY,  NORMAL, 19, 8,  "Which security mechanism should DSA support ?:",                    NULL, 0, FALSE, FALSE,
 D20_CGET,  NORMAL, 19,58,  (char *)&m_authmech,                              NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTMENU, 0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,  FKTHELP,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };

/*****************************************************************************/
/*     Empty mask to display configuration				     */
/*****************************************************************************/

D20_omask    conf_mask[] = {
 D20_CSAY,   NORMAL,  2, 5,  "confpar",                                      NULL, 0, FALSE, FALSE,
 D20_CSAY,   INVERS,  2,32,  "DIRECTORY SYSTEM",                             NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  2,64,  "Configuration",                                NULL, 0, FALSE, FALSE,
D20_EOMSK };

/*****************************************************************************/
/*     Mask to save the local database					     */
/*****************************************************************************/

D20_omask   dirsi_mask[] = {
 D20_CSAY,   NORMAL,  2, 5,  "diridw",                                       NULL, 0, FALSE, FALSE,
 D20_CSAY,   INVERS,  2,32,  "DIRECTORY SYSTEM",                             NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  2,64,  "save data",                                    NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  6,10,  "for which directory identifier do you want to", NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  7,10,  "save the directory system ? [1-20]:",          NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL,  7,46,  (char *)&m_dir_id,                              NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  9,10,  "On which medium do you want to save the data ?:", NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL,  9,58,  (char *)&m_medium,                              NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTMENU, 0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTHELP,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };

D20_omask   dirsiA_mask[] = {
 D20_CSAY | D20_NOERASE,
             NORMAL, 12,10,  "Backup password (optional):",                  NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL, 12,39,  m_userkey,                                      "%-10s", 0, FALSE, FALSE,
 D20_CSAY,   NORMAL, 14,10,  "{  you want to format the media volume(s) ?:", NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL, 14,55,  (char *)&m_form,                                NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTMENU, 0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTHELP,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };

D20_omask   dirsiF_mask[] = {
 D20_CSAY | D20_NOERASE,
             NORMAL, 12,10,  "Name of the file :",	                     NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL, 12,29,  m_filename,                                     "%-45s", 0, FALSE, FALSE,
 D20_CSAY,   NORMAL, 14,10,  "Backup password (optional):",                  NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL, 14,39,  m_userkey,                                      "%-10s", 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTMENU, 0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTHELP,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };

/*****************************************************************************/
/*     Empty mask to save the local database without displaying leading mask */
/*****************************************************************************/

D20_omask    dirsi0_mask[] = {
 D20_CSAY,   NORMAL,  2, 5,  "diridw",                                       NULL, 0, FALSE, FALSE,
 D20_CSAY,   INVERS,  2,32,  "DIRECTORY SYSTEM",                             NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  2,64,  "save data",                                    NULL, 0, FALSE, FALSE,
D20_EOMSK };

D20_omask   dirsi1_mask[] = {
 D20_CSAY | D20_NOERASE,
	     NORMAL, 23, 2,  "                                            ", NULL, 0, FALSE, FALSE,
 D20_CSAY,    NORMAL, 24, 2,  "                                           ", NULL, 0, FALSE, FALSE,
 D20_CSAY,    NORMAL, 17,10,  "Insert %2d. media volume and then press the", NULL, 0, FALSE, FALSE,
 D20_CSAY,    NORMAL, 18,10,  "Return-key to start the operation ? ",        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };


D20_omask   dirsi2_mask[] = {
 D20_CSAY | D20_NOERASE,
             NORMAL, 23, 2,  "Creating file list(s) ...",                    NULL, 0, FALSE, FALSE,
 D20_EOMSK };

D20_omask   dirsi3_mask[] = {
 D20_CSAY | D20_NOERASE,
	     NORMAL, 15,10,  "The backup requires %2d media volume(s).",     NULL, 0, FALSE, FALSE,
 D20_EOMSK };

D20_omask   dirsi4_mask[] = {
 D20_CSAY | D20_NOERASE,
	     NORMAL, 23, 2,  "Writing data to media volume %2d ...",          NULL, 0, FALSE, FALSE,
 D20_EOMSK };

D20_omask   dirsi4F_mask[] = {
 D20_CSAY | D20_NOERASE,
	     NORMAL, 23, 2,  "Writing data ...",          NULL, 0, FALSE, FALSE,
 D20_EOMSK };

/*****************************************************************************/
/*     Mask to restore a saved database					     */
/*****************************************************************************/

D20_omask   direi_mask[] = {
 D20_CSAY,   NORMAL,  2, 5,  "diridw",                                       NULL, 0, FALSE, FALSE,
 D20_CSAY,   INVERS,  2,32,  "DIRECTORY SYSTEM",                             NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  2,64,  "restore data",                                 NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  6,10,  "for which directory identifier do you want to restore", NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  7,10,  "the directory system ? [1-20]:",               NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL,  7,41,  (char *)&m_dir_id,                              NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  9,10,  "From which medium do you want to read the data ?:", NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL,  9,60,  (char *)&m_medium,                              NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTMENU, 0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTHELP,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };

D20_omask   direiA_mask[] = {
 D20_CSAY | D20_NOERASE,   
             NORMAL, 11,10,  "Backup password:",                             NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL, 11,27,  m_userkey,                                      "%-10s", 0, FALSE, FALSE,
 D20_CSAY,   NORMAL, 13,10,  "Attention: Your existing data will be overwritten !!!", NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTMENU, 0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTHELP,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };

D20_omask   direiF_mask[] = {
 D20_CSAY | D20_NOERASE,   
             NORMAL, 10,10,  "Name of the file:",                            NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL, 10,35,  m_filename,                                     "%-45s", 0, FALSE, FALSE,
 D20_CSAY,   NORMAL, 11,10,  "Backup password:",                             NULL, 0, FALSE, FALSE,
 D20_CGET,   NORMAL, 11,27,  m_userkey,                                      "%-10s", 0, FALSE, FALSE,
 D20_CSAY,   NORMAL, 13,10,  "Attention: Your existing data will be overwritten !!!", NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTMENU, 0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTHELP,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };


/*****************************************************************************/
/* Empty mask to restore the local database without displaying leading mask  */
/*****************************************************************************/

D20_omask    direi0_mask[] = {
 D20_CSAY,   NORMAL,  2, 5,  "diridw",                                       NULL, 0, FALSE, FALSE,
 D20_CSAY,   INVERS,  2,32,  "DIRECTORY SYSTEM",                             NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL,  2,64,  "restore data",                                 NULL, 0, FALSE, FALSE,
D20_EOMSK };

D20_omask   direi1_mask[] = {
 D20_CSAY | D20_NOERASE,
	     NORMAL, 23, 2,  "                                            ", NULL, 0, FALSE, FALSE,
 D20_CSAY,    NORMAL, 24, 2,  "                                           ", NULL, 0, FALSE, FALSE,
 D20_CSAY,    NORMAL, 17,10,  "Insert %2d. media volume and then press the",                   NULL, 0, FALSE, FALSE,
 D20_CSAY,    NORMAL, 18,10,  "Return-key to start the operation ? ",        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };


D20_omask   direi2_mask[] = {
 D20_CSAY | D20_NOERASE,
	     NORMAL, 15,10,  "The backup consists of %2d media volume(s).",       NULL, 0, FALSE, FALSE,
 D20_EOMSK };

D20_omask   direi3_mask[] = {
 D20_CSAY | D20_NOERASE,
	     NORMAL, 23, 2,  "Reading data from media volume %2d ...",       NULL, 0, FALSE, FALSE,
 D20_EOMSK };

D20_omask   direi3F_mask[] = {
 D20_CSAY | D20_NOERASE,
	     NORMAL, 23, 2,  "Reading data ...",       NULL, 0, FALSE, FALSE,
 D20_EOMSK };

D20_omask   dirsiei1F_mask[] = {
 D20_CSAY | D20_NOERASE,
	     NORMAL, 23, 2,  "                                            ", NULL, 0, FALSE, FALSE,
 D20_CSAY,    NORMAL, 24, 2,  "                                           ", NULL, 0, FALSE, FALSE,
 D20_CSAY,    NORMAL, 18,10,  "Press the Return-key to start the operation", NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTDEL,  0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,   0, 0,  NULL,                                        NULL, 0, FALSE, FALSE,
 D20_EOMSK };

/*****************************************************************************/
/*     Mask to display an error						     */
/*****************************************************************************/

D20_omask   error_mask[] = {
 D20_CSAY | D20_NOERASE,
	     NORMAL, 23, 2,  "                                            ", NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL, 23, 2,  "",                                             NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL, 24, 2,  "To continue please press CR",                  NULL, 0, FALSE, FALSE,
 D20_FKTX,   FKTCR,  0, 0,  NULL,                                         NULL, 0, FALSE, FALSE,
 D20_EOMSK };

/*****************************************************************************/
/*     Mask to remove an errortext					     */
/*****************************************************************************/

D20_omask   clear_mask[] = {
 D20_CSAY | D20_NOERASE,
	     NORMAL, 23, 2,  "                                            ", NULL, 0, FALSE, FALSE,
 D20_CSAY,   NORMAL, 24, 2,  "                                            ", NULL, 0, FALSE, FALSE,
 D20_EOMSK };

/*****************************************************************************/
/*     Error - messages                                                      */
/*****************************************************************************/

char    *err_mess[MAXERRMSG] ; 

/* ************************************************************************* */
/* ------------------- ARRAY FOR ALL HELP-MASKS ---------------------------- */
/* ************************************************************************* */

D20_help_array  help_masks1[] =
		{ { diradm_mask1, 1, 2, diradm_mask1, (D20_omask *) 0 },
		  { diradm_mask2, 3, 1, diradm_mask2, (D20_omask *) 0 },
		  { confpar_mask, 4, 1, confpar_mask, (D20_omask *) 0 },
		  { conf1d_mask, 5, 1, confpar_mask, conf1d_mask, (D20_omask *) 0 },
		  { conf1_mask, 6, 1, confpar_mask, conf1_mask, (D20_omask *) 0 },
		  { conf2_mask, 7, 1, confpar_mask, conf1_mask, conf2_mask, (D20_omask *) 0 },
		  { conf3_mask, 8, 1, confpar_mask, conf1_mask, conf2_mask, conf3_mask, (D20_omask *) 0 },
		  { dirsi_mask, 9, 1, dirsi_mask, (D20_omask *) 0 },
		  { direi_mask, 10, 1, direi_mask, (D20_omask *) 0 },
		  { dirsiA_mask, 11, 1, dirsi_mask, dirsiA_mask, (D20_omask *) 0 },
		  { dirsiF_mask, 12, 1, dirsi_mask, dirsiF_mask, (D20_omask *) 0 },
		  { direiA_mask, 13, 1, direi_mask, direiA_mask, (D20_omask *) 0 },
		  { direiF_mask, 14, 1, direi_mask, direiF_mask, (D20_omask *) 0 },
	  	  { (D20_omask *) 0 },
		} ;

/* ************************************************************************* */
/* --------------------------  H E L P - M A S K   ------------------------- */
/* ************************************************************************* */

D20_omask d20helpomask[] = {
	D20_CSAY,  NORMAL,  2, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL,  4, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL,  5, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL,  6, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL,  7, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL,  8, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL,  9, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 10, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 11, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 12, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 13, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 14, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 15, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 16, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 17, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 18, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 19, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 20, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 21, 5, NULL,        NULL, 0, FALSE, FALSE,
	D20_CSAY,  NORMAL, 23,12, NULL,        NULL, 0, FALSE, FALSE,
	D20_FKTX,  FKTDEL,  0, 0, NULL,        NULL, 0, FALSE, FALSE,
	D20_FKTX,  FKTCR,   0, 0, NULL,        NULL, 0, FALSE, FALSE,
	D20_FKTX,  FKTSCD,  0, 0, NULL,	  NULL, 0, FALSE, FALSE,
	D20_FKTX,  FKTSCU,  0, 0, NULL,	  NULL, 0, FALSE, FALSE,
	D20_EOMSK };

