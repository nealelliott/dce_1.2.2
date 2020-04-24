/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20_adm.h,v $
 * Revision 1.1.10.2  1996/02/18  23:35:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:21  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:59:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:21  root]
 * 
 * Revision 1.1.8.6  1994/08/08  13:20:59  marrek
 * 	Fix for OT11045.
 * 	[1994/08/08  13:18:53  marrek]
 * 
 * Revision 1.1.8.5  1994/07/06  15:07:27  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:16:09  marrek]
 * 
 * Revision 1.1.8.4  1994/06/21  14:47:02  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:40:31  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:55:38  marrek
 * 	April 1994 submission.
 * 	[1994/04/29  10:52:16  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:18:16  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:29:23  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:07:35  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:43:05  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:15:16  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:37:32  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  20:39:22  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:28:48  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  19:08:28  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  10:43:23  marrek]
 * 
 * Revision 1.1.4.2  1992/10/05  12:58:52  marrek
 * 	Changes for bug 5205.
 * 	[1992/10/05  12:58:08  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  16:53:22  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:22:21  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:52:12  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:41:00  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D20_ADM_H
#define _D20_ADM_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d20_adm_rcsid[] = "@(#)$RCSfile: d20_adm.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:35:25 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*****************************************************************************/
/*                  HEADER-FILE fuer Directory Administration                */
/*                                                                           */
/*****************************************************************************/

#define D20_ADMIN_NLSFILE     "gdssysadm.cat"
#define D20_ADMINHELP_NLSFILE "gdssyshelp.cat"

#define MAXLEN		10	/* maximum length for malloc size in nls-file */
#define SLEEP_TIME	5

#define MINID           1
#define MAXID           20
#define MAXERRMSG       53
#define MAXCONFTYP	4

#define LOG_ON          "on"
#define	LOG_OFF		"off"
#define TRACE		"D2_TRACE"
#define LOGFNAME        "%s/adm/log_%d"

#define D2_LOCK		0	/* lock specific file */
#define D2_UNLOCK	1	/* unlock specific file */
#define LOCKACT        "%s/adm/.lock_act"
#define LOCKDB         "%s/adm/.lock_db"

#define NULLDEV		"/dev/null"
#define	SHELL		"/bin/sh -s 2>&1"
#define	REQCMDTRACE	"***REQUEST:\n%s\n"
#define	RESCMDTRACE	"***RESULT: %d\n"
#define FUNCMSG	 	"***FUNCTION-CALL:\n"
#define CMDBUF          800

#define	CLT_GLOBAL	0
#define	CLTSRV_GLOBAL	1

#define	MEDIUM_FILE	2	/* save/restore to/from file */

#define	UPDMSG		0	/* position of message 'Delta-update operation*/
				/* failed' in diradm_mask1		      */
#define POS_CONF_TYPE   1       /* position of config. type in conf2_mask     */

#define	EXIT1		(1 << 8)
#define	EXIT2		(2 << 8)
#define	EXIT3		(3 << 8)
#define	EXIT4		(4 << 8)
#define	EXIT5		(5 << 8)
#define	EXIT6		(6 << 8)
#define	EXIT7		(7 << 8)
#define	EXIT8		(8 << 8)
#define	EXIT9		(9 << 8)
#define	EXIT10		(10 << 8)
#define	EXIT11		(11 << 8)
#define	EXIT12		(12 << 8)
#define	EXIT13		(13 << 8)
#define	EXIT14		(14 << 8)
#define	EXIT15		(15 << 8)
#define	EXIT16		(16 << 8)
#define	EXIT17		(17 << 8)
#define	EXIT18		(18 << 8)
#define	EXIT19		(19 << 8)
#define	EXIT20		(20 << 8)
#define EXIT21          (21 << 8)
#define EXIT22          (22 << 8)
#define EXIT23          (23 << 8)
#define EXIT24          (24 << 8)
#define EXIT25          (25 << 8)
#define EXIT26          (26 << 8)
#define EXIT27          (27 << 8)
#define EXIT28          (28 << 8)
#define EXIT29          (29 << 8)
#define EXIT30          (30 << 8)
#define EXIT31          (31 << 8)
#define EXIT32          (32 << 8)
#define EXIT33          (33 << 8)
#define EXIT34          (34 << 8)
#define EXIT35          (35 << 8)
#define EXIT36          (36 << 8)
#define EXIT37          (37 << 8)
#define EXIT38          (38 << 8)
#define EXIT39          (39 << 8)
#define EXIT40          (40 << 8)
#define EXIT41          (41 << 8)
#define EXIT42          (42 << 8)
#define EXIT43          (43 << 8)
#define EXIT44          (44 << 8)
#define EXIT45          (45 << 8)

#define MAX_EXIT        EXIT45

				/* exit states of d20admin                    */
#define EXIT50          50      /* pressed key END, only without parameters   */
#define EXIT51          51      /* it is not possible to to change language   */
				/* and installation                           */
#define EXIT52          52      /* DIRINFBAS / LANG not set or wrong          */
#define EXIT53          53      /* language files not installed               */
#define EXIT54          54      /* directory system installation not available*/
#define EXIT55          55      /* error while allocating memory              */
#define EXIT56          56      /* you have left a maks with DEL              */
#define EXIT57          57      /* can't get user name                        */

#define AD              0
#define KO              1
#define AK              2
#define DE              3
#define SI              4
#define EI              5
#define WE              6
#define DS              7
#define TA              8
#define TE              9
#define TON             10
#define TOFF            11
#define SH              12

#define D20_WR_SYNTAX  "FATAL ERROR: wrong syntax (%s)\n"
#define D20_WR_CODE    "FATAL ERROR: illegal function code (%s)\n"
#define D20_WR_PARAM   "FATAL ERROR: parameter not allowed (%s)\n"
#define D20_WR_COMB    "FATAL ERROR: illegal parameter combination\n"

#define D20_LKEY       10   /* length of user key                   */
#define D20_LFILE      45   /* length of filename (save, restore)   */


			    /* minimum and maximum values for input */
			    /* parameters                           */
#define D20_1DBTYPE     1
#define D20_2DBTYPE     2

#define D20_1CONFTYPE   1
#define D20_2CONFTYPE   2

#define D20_1DIRID      1
#define D20_2DIRID      20

#define D20_1FORM_MEDIUM 0
#define D20_2FORM_MEDIUM 1

#define D20_1CONFMODE   1
#define D20_2CONFMODE   4

#define D20_1MEDIUM     0
#define D20_2MEDIUM     2

#define D20_1OPMODE     1
#define D20_2OPMODE     4

#define D20_1NOSERVER   1
#define D20_2NOSERVER   256

#define D20_1NOCLIENT   1
#define D20_2NOCLIENT   256
#define D20_3NOCLIENT   16

#define D20_1VOLUMENO   1
#define D20_2VOLUMENO   -1

			    /* parameters of "d20admin"-call          */
#define D20_ADMIN        'a'     /* constant abreviation for function AD     */
#define D20_CONF         'c'     /* constant abreviation for function KO     */
#define D20_ACT          'A'     /* constant abreviation for function AK     */
#define D20_DEACT        'd'     /* constant abreviation for function DE     */
#define D20_SAVE         's'     /* constant abreviation for function SI     */
#define D20_RESTORE      'r'     /* constant abreviation for function EI     */
#define D20_STATE        'i'     /* constant abreviation for function DS     */
#define D20_TR_ON        'T'     /* constant abreviation for function TA     */
#define D20_TR_OFF       't'     /* constant abreviation for function TE     */
#define D20_X_ON         'X'     /* constant abreviation for function TO     */
#define D20_X_OFF        'x'     /* constant abreviation for function TO     */
#define D20_SHELL        '!'     /* constant abreviation for function SH     */

#define D20_FUNC        'f'	/* always first parameter; describes function*/
#define D20_DBTYPE      'b'	/* database system			     */
#define D20_CONFTYPE    'c'     /* configuration type			     */
#define D20_DIRID       'd'     /* directory id				     */
#define D20_FORM_MEDIUM 'F'     /* formatting device			     */
#define D20_USERKEY     'k'     /* user key (for save/restore)		     */
#define D20_CONFMODE    'm'     /* configuration mode			     */
#define D20_MEDIUM      'M'     /* data medium type (for save/restore)	     */
#define D20_OPMODE      'o'     /* operation mode (for save/restore)	     */
#define D20_PRINTERR    'p'     /* suppress error messages		     */
#define D20_NOSERVER    's'     /* number of servers			     */
#define D20_NOCLIENT    'C'     /* number of clients			     */
#define D20_VOLUMENO    'v'     /* volume number (for save/restore)	     */
#define D20_XON         'X'     /* turns on trace for gdssysadm		     */
#define D20_SUPP_MASK   'D'     /* suppress output in mask		     */
#define D20_FILENAME    'n'     /* name of the file (for save/restore)       */
#define D20_AUTH_MECH	'A'	/* authentication mechanism used by DSA.     */
#define D20_CLHELP	'h'	/* help                                      */

#define D20_CLHELP_LINE_NR	7	/* Number of lines for the command   */
					/* line help			     */

				/* bit combinations for input values         */
#define D20C_DBTYPE      (1 << 1)
#define D20C_CONFTYPE    (1 << 2)
#define D20C_DIRID       (1 << 3)
#define D20C_FILENAME    (1 << 4)
#define D20C_FORM_MEDIUM (1 << 5)
#define D20C_USERKEY     (1 << 6)
#define D20C_CONFMODE    (1 << 7)
#define D20C_MEDIUM      (1 << 8)
#define D20C_OPMODE      (1 << 9)
#define D20C_PRINTERR    (1 << 10)
#define D20C_NOSERVER    (1 << 11)
/* Value no more used    (1 << 12) */
#define D20C_VOLUMENO    (1 << 13)
#define D20C_XON         (1 << 14)
#define D20C_NOCLIENT	 (1 << 15)
#define D20C_SUPP_MASK   (1 << 16)
#define D20C_AUTH_MECH	 (1 << 17)


#define D20_NOFIELDS   5    /* maximum number of input fields in a mask */

				/* positions of input first input field */
				/* and number fields that have to be set*/
				/* to D20_NO_OP                         */

				/* confpar_mask                       */
#define D20_P0CONFMODE  3
#define D20_N0CONFMODE  2

				/* conf1_mask                         */
#define D20_P1DIRID     0
#define D20_N1DIRID     3

				/* conf2_mask                         */
#define D20_P2CONFTYPE  0
#define D20_N2CONFTYPE  2

#define D20_P2CLIENT    2
#define D20_N2CLIENT    3

				/* conf3_mask                         */
#define D20_P3NOSERVER  0
#define D20_N3NOSERVER  2

#define D20_P3AUTHMECH    2
#define D20_N3AUTHMECH    2

				/* dirsi_mask                         */
#define D20_P4DIRID     3
#define D20_N4DIRID     3

#define D20_P4MEDIUM    6
#define D20_N4MEDIUM    2

				/* direi_mask                         */
#define D20_P5DIRID     3
#define D20_N5DIRID     3

#define D20_P5MEDIUM    6
#define D20_N5MEDIUM    2

				/* dirsiA_mask                        */
#define D20_P6USERKEY   0
#define D20_N6USERKEY   2

#define D20_P6FORM_MEDIUM   2
#define D20_N6MEDIUM        2

				/* dirsiF_mask                        */
#define D20_P7FILENAME  0
#define D20_N7FILENAME  2

#define D20_P7USERKEY  2
#define D20_N7USERKEY  2

				/* direiA_mask                        */
#define D20_P8USERKEY   0
#define D20_N8USERKEY   2

				/* direiF_mask                        */
#define D20_P9FILENAME  0
#define D20_N9FILENAME  2

#define D20_P9USERKEY   2
#define D20_N9USERKEY   2

#define POS_AD          3       /* position of function AD in diradm_mask1  */
#define POS_KO          4       /* position of function KO in diradm_mask1  */
#define POS_AK          5       /* position of function AK in diradm_mask1  */
#define POS_DE          6       /* position of function DE in diradm_mask1  */
#define POS_SI          7       /* position of function SI in diradm_mask1  */
#define POS_EI          8       /* position of function EI in diradm_mask1  */
#define POS_WE          9       /* position of function WE in diradm_mask1  */
#define POS_DS          2       /* position of function DS in diradm_mask2  */
#define POS_TA          3       /* position of function TA in diradm_mask2  */
#define POS_TE          4       /* position of function TE in diradm_mask2  */

#define NO_DIRADM1      7       /* number of function pos. in diradm_mask1  */
#define NO_DIRADM2      3       /* number of function pos. in diradm_mask2  */
#define NO_OF_FUNC      13      /* number of all possible functions         */

#define KCRE		1
#define	KDEL		2
#define KDIS		3
#define KCHG            4

#define L_ZEILE         100

#define CHECK_UPD       "for DIRID in \
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20\n\
do\nif [ \\( -s %s/dsa/dir$DIRID/update/SHDUPD_ERR \\) \
-o \\( -s %s/dsa/dir$DIRID/update/UPD_FATERR \\) ]\nthen\nexit 1\nfi\n\
done\nexit 0\n"

#define CHANGE_INST     "awk -F= '{if ($1 != \"DIRINFBAS\") print $0; \
else print \"DIRINFBAS=%s; export DIRINFBAS\"}' \
$HOME/.profile > $HOME/.profile.$$\n\
mv $HOME/.profile.$$ $HOME/.profile\nexit $?\n"

#define CHANGE_LANGUAGE "awk -F= '{if ($1 != \"LANG\") print $0; \
else print \"LANG=%s; export LANG\"}' \
$HOME/.profile > $HOME/.profile.$$\n\
mv $HOME/.profile.$$ $HOME/.profile\nexit $?\n"

#define PREPMASK        "[ -s %s/gdsconfig ] || { echo \"-1 0 0 0 0 -1\"; exit 1; }\n\
awk 'BEGIN { ENTRY_FOUND = 0 }\n\
$2 == %d { if ($4 == \"Client-System\") TYPE = 0\n\
if ($4 == \"Clt/Srv-System\") TYPE = 1\n\
if ($6 == \"-\") $6 = 2\n\
if ($8 == \"-\") UPDATE = 0; else UPDATE = 1\n\
if ($10 == \"-\" || $10 == \"C-isam\") DBTYP = 1\n\
if ($10 == \"informix\") DBTYP = 2\n\
printf\"%%d %%d %%d %%d %%d %%d\",TYPE,$6,UPDATE,DBTYP,$12,$14\n\
ENTRY_FOUND = 1; exit }\n\
END { if ( ENTRY_FOUND == 0) printf \"-1 0 0 0 0 -1\" }'  %s/gdsconfig"

#define FINDCMD	 	"find %s%s -atime +0 -name 'log_*' -exec rm -f {} \\;"
#define USERCMD         "awk -F= '$1 == \"D2_USER\" { printf \"%%s\", substr ($2, 0, index ($2, \";\") - 1); exit }' %s/.profile"
#define DISPLINENO	10
#define CONFISIZECMD    "cat %s/gdsconfig 2> /dev/null | wc -l"
#define DSAPATH         "%s%s/gdsdsa"
#define ADMPATH         "%s%s/gdsditadm"
#define ADMDIT          ". %s/.profile\ngdsditadm 2>> %s"
#define ADMCACHE        ". %s/.profile\ngdscacheadm 2>> %s"
#define CONFFKT         ". %s/.profile\ngdsconf %s %d %d %s %d %s %s %d %d %s %s %s 2>> %s"
#define ACTFKT          ". %s/.profile\ngdsstart %s %s %s 2>> %s >&2"
#define DEACTFKT        ". %s/.profile\ngdsdeact %s 2>> %s"
#define DBWRIT1FKT      ". %s/.profile\ngdsdbwrite %s %d 1 %s %d %s %s 2>> %s"
#define DBWRIT2FKT      ". %s/.profile\ngdsdbwrite %s %d 2 %d %d %s %s 2>> %s"
#define DBWRIT3FKT      ". %s/.profile\ngdsdbwrite %s %d 3 %s %s %s %s 2>> %s"
#define DBREAD1FKT      ". %s/.profile\ngdsdbread %s %d 1 %d %d %s %s 2>> %s"
#define DBREAD2FKT      ". %s/.profile\ngdsdbread %s %d 2 %s %s %s %s 2>> %s"
#define GETVOLCMD       "awk '$1 == \":nvol:\" { print $2; exit }' %s/tmp/vol%d.1 2>> %s"
#define GETKEYCMD       "awk '$1 == \":dkey:\" { print $2; exit }' %s/tmp/vol%d.1 2>> %s"
#define DSTATEFKT       ". %s/.profile\ngdsinfo %s %s %s 2>> %s"
#define DLOGFKT         ". %s/.profile\ngdslog %s %s 2>> %s"

#define FAIL_ERROR      "\nFATAL ERROR: No directory system installation available !\n"
#define FAIL_ERR1	"\nFATAL ERROR: Wrong universe !\n"
#define FAIL_ERR2	"\nFATAL ERROR: Can't get user name\n"
#define FAIL_ERR3	"\nFATAL ERROR: While accessing NLS file !!!\n"



/*-- Defines specifics for error messages -------------------------*/

/* Defines the index to use in the error table		*/
/* Error codes are in h/d27svc.h			*/

#define	IDX_INV_PAR_ERR		44
#define	IDX_INV_DIR_ID		45
#define	IDX_CHDIR_ERR		46
#define	IDX_FORK_ERR		47
#define	IDX_CHK_KEY_ERR		48
#define	IDX_INV_RES		44
#define	IDX_INV_PRIV		44
#define	IDX_INV_NORM		44
#define	IDX_INV_CPATH		44
#define	IDX_INV_SWITCH		44
#define	IDX_INV_VALUE		44
#define	IDX_INV_MXIASSOC	49
#define	IDX_INV_MXRASSOC	50
#define	IDX_NO_AUTH_MECH	51
#define	IDX_INV_AUTH_MECH	51
#define	IDX_INV_IVTIME		52

#define MAX_ERR_MESS_SIZE	1024

/* Process identifiers used for error messages	*/

#define GDSIPCINIT_IDX		1
#define GDSIPCCHK_IDX		2
#define GDSCACHE_IDX		3
#define GDSCSTUB_IDX		4
#define GDSSSTUB_IDX		5
#define GDSDSA_IDX		6

/* Process names for error messages		*/

#define GDSIPCINIT_NAME		"gdsipcinit: "
#define	GDSIPCCHK_NAME		"gdsipcchk: "
#define GDSCACHE_NAME		"gdscache: "
#define	GDSCSTUB_NAME		"gdscstub: "
#define GDSSSTUB_NAME		"gdssstub: "
#define GDSDSA_NAME		"gdsdsa: "



#endif /* _D20_ADM_H */
