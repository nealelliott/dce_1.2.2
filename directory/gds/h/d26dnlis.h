/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26dnlis.h,v $
 * Revision 1.1.10.2  1996/02/18  23:35:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:50  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:00:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:44  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:49:45  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:44:55  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:38:50  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:39:59  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:40:43  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:29:31  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:09:30  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  09:32:06  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  04:03:05  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:42:12  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D26DNLIS_H
#define _D26DNLIS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d26dnlis_rcsid[] = "@(#)$RCSfile: d26dnlis.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:35:42 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : INCLUDE                                             */
/*								      */
/* NAME         : d26dnlis.h                                          */
/*								      */
/* AUTHOR       : Schreck, D AP 11                                    */
/* DATE         : 29.03.89                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOK.-NR.     : Directory-Design-Specification                      */
/*								      */
/* PRD#/VERS.	:						      */
/*								      */
/* DESCRIPTION  : This Include-File contains "typedefs" and "defines" */
/*                used by the functions working with dnlist in file.  */
/*								      */
/* SYSTEM DEPENDENCIES:  SINIX C-ISAM                                 */
/*								      */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 29.03.89| Original                       | AS |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#define D26_I02_RECDNLISTL   (MAXKEYSIZE + D2_DNL_MAX + LONGSIZE)
				      /* record length of file for         */
				      /* distinguished name list           */

#define D26_I03_GET_CORE_INDEX    2   /* index of DN list in core =        */
				      /*    isrecnum - D26_GET_CORE_INDEX  */
				      /* isrecnum =                        */
				      /*    index of dn list in core       */
				      /*    + D26_GET_CORE_INDEX           */
				      /* (Note: The first isrecnum created */
				      /*        by C-ISAM V2.1, V3.1 is 1. */
				      /*        The first record of DN-List*/
				      /*        in file contains the time- */
				      /*        stamp of last update. So   */
				      /*        the first record containing*/
				      /*        a dist. name is the record */
				      /*        with isrecnum = 2 and the  */
				      /*        index of the first entry in*/
				      /*        dn list in core is 0 (there*/
				      /*        is no timestamp in the     */
				      /*        first entry in core).      */
				      /*        The dn with isrecnum = 2   */
				      /*        has the index 0 in dn list */
				      /*        in core.                   */
				      /*        The functions above are    */
				      /*        used to calculate isrecnum */
				      /*        and index to dn list in    */
				      /*        core.)                     */


				      /* valid values for input par. key   */
				      /* of function d26_i03_set_dnlistkey */
#define D26_I04_NO_KEY            0   /* no key (isrecnum)                 */
#define D26_I05_KEY_DNINTANDDN    1   /* dninterpretation and dn           */
#define D26_I06_KEY_OCCURRENCY    2   /* occurrency                        */
#define D26_I07_KEY_DN            3   /* dn (not possible at this time)    */
				      /* for further study                 */


/* offset to begin of dnlist record of datas for calculating the      */
/* position/begin of the datas in dnlist record                       */
/*                                                                    */
/* first record (record with time):                                   */
/* format: position I length     I type I meaning                     */
/*                  I (in bytes) I      I                             */
/*         ---------+------------+------+---------------------------- */
/*         0        I 1: CHARSIZEI Char I reason of last update       */
/*                  I            I      I (not used)                  */
/*                  I            I      I                             */
/*         1        I 1: CHARSIZEI Char I special character which     */
/*                  I            I      I never occurs in an          */
/*                  I            I      I D26_name_string (dist. name */
/*                  I            I      I syntax)                     */
/*                  I            I      I                             */
/*         2        I 4: sizeof  I Long I version number of DN-list   */
/*                  I    (Long)  I      I                             */
/*                  I            I      I                             */
/*         6        I 1: CHARSIZEI Char I EOS                         */
/*                  I            I      I                             */
/*         ...      I ...        I ...  I not used                    */
/*                  I            I      I                             */
/*         CHARSIZE I            I      I                             */
/*      + D2_DNL_MAXI            I      I                             */
/*      + 1         I 2:LONGSIZE I Long I -1 (negative value)         */
     /* is that +1 right ???*/
/*                  I            I      I (to fill this field C-isam  */
/*                  I            I      I "stlong" and to read this   */
/*                  I            I      I field C-isam "ldlong" must  */
/*                  I            I      I be used")                   */
/*                  I            I      I                             */
#define D26_I08_REASON            0   /* reason in first record       */
#define D26_I09_VERSION           2   /* version number of file       */
/*                                                                    */
/* other records with dn informations                                 */
/* format: position     I length      I type  I meaning               */
/*                      I (in bytes)  I       I                       */
/*         -------------+-------------+-------+-----------------------*/
/*         0            I 1: CHARSIZE I Char  I distinguished name    */
/*                      I             I       I interpretation        */
/*                      I             I       I                       */
/*         CHARSIZE     I D2_DNL_MAX  I Char  I distinguished name    */
/*                      I + 1         I-StringI terminated with EOS   */
/*                      I             I       I                       */
/*         CHARSIZE     I             I       I                       */
/*         + D2_DNL_MAX I             I       I                       */
/*         + 1          I 2: LONGSIZE I Long  I occurrency (no of     */
     /* is that +1 right ???*/
/*                      I             I       I references to dn)     */
/*                      I             I       I value >= 0            */
/*                      I             I       I C-isam functions      */
/*                      I             I       I "ldlong" (reads value)*/
/*                      I             I       I and "stlong" (stores  */
/*                      I             I       I value) must be used   */
/*                      I             I       I                       */
#define D26_I10_DNINT             0   /* dist. name interpretation         */
#define D26_I11_DNNAME          CHARSIZE
				      /* normed distinguished name         */
				      /* (terminated by EOS)               */
#define D26_I12_OCCURRENCY      (MAXKEYSIZE + D2_DNL_MAX)
				      /* number of references to dn        */

/* return_values for d26_i07_read_dnlistrec */
#define D26_I13_FOUND             1   /* wished dnlistrec was found        */
#define D26_I14_NOT_FOUND         2   /* wished record doesn't exist       */


#define D26_I15_SLEEP_SECS        1   /* no of seconds to sleep when a     */
				      /* record or file is locked          */


#define D26_I16_VERSION_RECNUM    1   /* isrecnum of "time record"         */
				      /* (no key, first record)            */

#define D26_I17_NO_DNINT_VAL      3   /* number of possible dn             */
				      /* interpretation values             */

/* I18 .. I20 reserved see also d26utilis.c */

#endif /* _D26DNLIS_H */
