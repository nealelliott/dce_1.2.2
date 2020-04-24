/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20logon.c,v $
 * Revision 1.1.924.2  1996/02/18  19:40:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:46  marty]
 *
 * Revision 1.1.924.1  1995/12/08  15:17:03  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1994/08/17  20:29 UTC  dtruong
 * 	Merge from Hpdce02_01
 * 	[1995/12/08  14:51:56  root]
 * 
 * Revision 1.1.922.5  1994/08/01  15:31:44  marrek
 * 	Bug fixes for OT 11295, 11456, 11458, 11471.
 * 	[1994/08/01  15:29:13  marrek]
 * 
 * Revision 1.1.922.4  1994/06/21  14:43:39  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  13:44:06  marrek]
 * 
 * 	HP revision /main/HPDCE02/2  1994/07/08  22:20 UTC  dtruong
 * 	Corrected build errors
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  16:34 UTC  dtruong
 * 	Merge to DCE 1.1
 * 
 * 	HP revision /main/HPDCE01/2  1994/03/16  21:35 UTC  dtruong
 * 	merge to main line
 * 
 * 	HP revision /main/HPDCE01/dtruong_mothra/2  1994/03/07  19:08 UTC  dtruong
 * 	Added routine to check hpterm
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/28  19:15  mgm
 * 	Merge hpdce01 and kk_final
 * 
 * Revision 1.1.7.4  1993/12/17  23:07:55  jake
 * 	GDS What string changes
 * 	[1993/12/17  23:07:34  jake]
 * 
 * Revision 1.1.7.3  1993/11/04  19:17:29  dtruong
 * 	<<<Clean up hp softkeys before exit.>>>
 * 	[1993/11/04  19:04:20  dtruong]
 * 
 * 	Insert check for bind_dsa before strcmp() is used.
 * 	[1993/08/17  08:57:55  marrek]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/30  08:29:53  marrek]
 * 
 * Revision 1.1.7.2  1993/10/27  20:58:32  dtruong
 * 	<<<Added softkeys for hp terminal>>>
 * 	[1993/10/27  20:54:24  dtruong]
 * 
 * 	180_get_dsa should not be part of cacheadm
 * 	[1993/01/28  10:32:07  marrek]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/31  13:04:29  bbelch]
 * 
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:07:05  tom]
 * 
 * 	November 1992 code drop
 * 	[1992/11/26  08:53:05  marrek]
 * 
 * Revision 1.1.2.4  1993/10/19  18:13:14  root
 * 	    Loading
 * 
 * Revision 1.1.920.2  1993/10/14  16:46:28  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  16:43:59  keutel]
 * 
 * Revision 1.1.920.1  1993/10/13  17:29:35  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  07:35:05  keutel]
 * 
 * Revision 1.1.3.3  1992/06/26  07:03:31  jim
 * 	Merge with the latest level in the tree.
 * 	[1992/06/22  21:19:14  jim]
 * 
 * Revision 1.1.3.2  1992/06/01  21:55:47  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:16:16  zeliff]
 * 	Revision 1.1.1.3  1992/06/05  20:13:35  jim
 * 	Merge AIX 3.2 changes with the latest gds code
 * 
 * Revision 1.1.4.2  1992/06/04  20:50:55  zeliff
 * 	Merging in AIX3.2 porting changes.
 * 
 * Revision 1.1.3.2  1992/06/01  21:55:47  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:16:16  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20logon.c,v $ $Revision: 1.1.924.2 $ $Date: 1996/02/18 19:40:59 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*		  =========					      */
/* NAME         : d2logon.c   [main module f. admin. process]         */
/*		  =========					      */
/* AUTHOR       : U. Richter  D AP 113                                */
/* DATE         : 20.01.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : DS-Design-Spec.                                     */
/*                LB  3255.43.04                                      */
/*			.					      */
/*								      */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*								      */
/* DESCRIPTION  : This is the main module for directory V2            */
/*                administration. Handles the logon mask and          */
/*                makes bind to read the scheme, reads scheme         */
/*                and makes bind to the medium to be admistrated.     */
/*								      */
/* OPERATING-SYSTEM:                                                  */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. | Date    | Updates                        | KZ | CR# FM# */
/*   1.0    | 20.01.88| Original                       | Ri |         */
/*   2.0    | 14.12.88| Fehler bei BIND                | hv |     52  */
/*								      */
/*datoff **************************************************************/

 
/*####################################################################*/
/* AUSPRAEGUNG	dieser Datei:					      */
/*		  SINIX	  V2.0	MX2/MX4	V00  C-Comp. V00	      */
/*		  BS2000  V9					      */
/*								      */
/*####################################################################*/



/**************	system-includes				 ****************/

#include <gds.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <errno.h>
 
/************** DS-external includes                     ****************/
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d21iapl.h>
#include <nl_types.h>
#include <cget.h>
#include <d20proto.h>
#include <d2delupd.h>
#include <d27conv.h>

/************** MA-includes                              ****************/
#include <magenex.h>
#include <malgcfe.h>
 
#include <d2shm.h>

#include <stdlib.h>

/**************	defines	 (incl.	makros)			      ***************/
#define  D20_L_RESTINST         40   /* 5 for pid, 5 for /adm/              */

			    /* maximal number of NLS strings          */
#define D20_NUPD_TIME   3
#define D20_NOPT        2
#define D20_NFORMAT     2
#define D20_NUPD1_TIME  3
#define D20_NJOB_FORMAT 3

			    /* message set number in NLS file for different*/
			    /* error tables                                */
#define D20S_ERR_TAB    15
#define D20S_ERR_TABLE  16
#define D20S_ERRMESS    17

			    /* return values of d20_check_params           */
#define D20_AUTH_NO_ERROR	0
#define D20_ANONYMOUS_ERR	1
#define D20_SIMPLE_ERR		2
#define D20_STRONG_ERR		3
#define D20_DCE_AUTH_ERR	4
#define D20_AUTH_ERROR		5

                           /* error value of d20_translate_auth_mech_param */
			   /* its value should be different as any authen- */
			   /* tication mechanism type (D2_ANONYMOUS,       */
			   /* D2_SIMPLE, D2_DCE_AUTH, D2_STRONG            */
#define D2_BAD_VALUE		-1

#define D20_PASSWD_LEN		80	/* length of password field */


/************** typedefs                                      ***************/
typedef struct
	{   signed32 msg_no;  /* number of NLS-strings to be read for a */
			    /* speciofic message set                  */
	    char **msg_str; /* pointer list for NLS-strings           */
	} D20_strings;

/************** global variables  (import)                    ****************/
extern  char    getdel[];
extern  char    getfill;

extern  char      *d20_1ifields[] ;  /* Inputfields for Mask 1              */
extern  char      *d20_secifields[]; /* Inputfields for security mask       */
extern  D20_omask d20_1omask[] ;     /* Mask 1 */
extern  D20_omask d20_secomask[] ;     /* Mask 1 */

extern  char      *d20_2ifields[] ;  /* Inputfields for Mask 2              */
extern  D20_omask d20_2omask[] ;     /* Mask 2 */

extern  char    *d20_3ifields[];     /* pointer-array f. input of mask3,    */
				     /* pointers already set.               */
extern  char    *d20_3ofields[];     /* pointer-array f. output of mask3,   */
				     /* pointers already set.               */
extern  D20_omask d20_3omask[];
				     /* structure array for output mask3,   */
				     /* data in array already set.          */

extern  char      *d20_5ifields[] ;  /* Inputfields for Mask 5              */

extern  D20_omask           d20_msgomask[];
extern  D20_omask           d20_qomask[];
				     /* structure array for error output    */
extern	char	*d20_hdlmsk_errmsg[];	/* Error messages for handlemask    */
extern  char	*d20_errtab[] ;
extern  char    d20_actopmsg[];
extern  char    *d20_op[];
extern  char    *d20_bool[];
extern  char    *d20_cop[];
extern  char    *d20_errmess[];

extern  String  d20_tfile;   /* file name for testfile */
extern  Bool    d20_cache;   /* administrate cache if TRUE            */
extern  Bool    d20_shadow;  /* administrate shadows if TRUE          */
extern  signed32     rbatch;
extern  Bool    d20_obj_list;           /* a flag indicating whether  */
					/* the ADMIN object structures*/
					/* are already built          */
extern  signed32   d20_init_rule_nr ;   /* default value of rule-     */
					/* number for changing name-  */
					/* structure                  */

extern  Bool    d20_call_cread;

extern  D20_omask d20_working_mask[];
extern  D20_omask d20_4omask[];
extern  D20_omask d20_4aomask[];
extern  D20_omask d20_5omask[];
extern  D20_omask d20_6omask[];
extern  D20_omask d20_6aomask[];
extern  D20_omask d20_6bomask[];
extern  D20_omask d20_7omask[];
extern  D20_omask d20_7aomask[];
extern  D20_omask d20_8omask[];
extern  D20_omask d20_9omask[];
extern  D20_omask d20_Aomask[];
extern  D20_omask d20_Bomask[];
extern  D20_omask d20_Comask[];
extern  D20_omask d20_Domask[];
extern  D20_omask d20_E1omask[];
extern  D20_omask d20_E2omask[];
extern  D20_omask d20_E3omask[];
extern  D20_omask d20_Eaomask[];
extern  D20_omask d20_Ebomask[];
extern  D20_omask d20_Ecomask[];
extern  D20_omask d20_Edomask[];
extern  D20_omask d20_10omask[];
extern  D20_omask d20_11aomask[];
extern  D20_omask d20_11bomask[];

extern  D20_omask d20_12omask[];  	/* GOE Nov 15 '89 */
extern  D20_omask d20_12bomask[];  	/* GOE Nov 15 '89 */
extern  D20_omask d20_13omask[];  	/* GOE Nov 21 '89 */
extern  D20_omask d20_14omask[];  	/* EK  Jan 04 '90 */
extern  D20_omask d20_dnscellomask[];
extern  D20_omask d20_dnsrepomask[];
extern  D20_omask d20_ttxidomask[];
extern  D20_omask d20_telexomask[];
extern  D20_omask d20_t61omask[];
extern  D20_omask d20_faxomask[];
extern  D20_omask d20_oraddomask[];
extern  D20_omask d20_moraddomask[];
extern  D20_omask d20_noraddomask[];
extern  D20_omask d20_soraddomask[];
extern  D20_omask d20_uoraddomask[];
extern  D20_omask d20_toraddomask[];
extern  D20_omask d20_dlpermomask[];
extern  D20_omask d20_ornameomask[];
extern  D20_omask d20_mogomask[];
extern	D20_omask d20_dmeomask[];	/* DME mask	*/

extern  D20_omask d20_certomask[];
extern  D20_omask d20_certpomask[];
extern  D20_omask d20_certlomask[];
extern  D20_omask d20_rcertomask[];

extern  D20_omask d20_frameomask[];
extern  char      *d20_err_table[];
extern  char      *d20_fd_of_week;
extern  char      *d20_fhour;
extern  D20_opmsg d20_opmsg[];
extern  D20_opmsg d20_s_opmsg[];
extern  char      *d20_dupd_op[];
extern  char      *d20_tree_op[];
extern  char      *d20_disp[];
extern  char      *d20_2_1opt;
extern  char      *d20_2_2opt;
extern  char      *d20_opclass[D20_NOPCLASS];
extern  char      *d20_E11format;
extern  char      *d20_E12format;
extern  char      *d20_F_mints;
extern  char      *d20_F_hrs;
extern  char      *d20_F_days;
extern  char      *d20_F_job_state;
extern  char      *d20_F_act_mod;
extern  char      *d20_F_notact_mod;
extern  char      *d20_14oheader[];
extern  char	  d20_attr_name[];

/* for help masks */
extern	nl_catd          nl_help_fd;

#ifdef __hpux
extern                   define_keys();
extern                   set_stty();
extern			 reset_hp_term();
extern			 term_type();
#endif


/*** Functions declarations ***/

static void d20_call(signed32 funselect, String p_env, signed16 opmax,
			Bool loc_dsa);
static signed32 d20_check_params(signed16 auth_mech,
			    D2_name_string user_name,
			    Octet_string user_pwd);
static signed32 d20_translate_auth_mech_param(char auth_mech_param);

#ifndef CACHE_ADMIN
static	signed32	d20_check_logon_dn(char *);
#endif

/************** global variables  (export)                  ****************/

/* for new schema */
  Srt          *d2_p_srt = (Srt *) NULL;
				    /* pointer to Srt                      */
  Oct          *d2_p_oct = (Oct *) NULL;
				    /* pointer to Oct                      */
  At           *d2_p_at  = (At *) NULL;
				    /* pointer to At                       */

  signed32           d2_nsrt = 0;  /* number of Srt elements              */
  signed32           d2_nat  = 0;  /* number of At elements               */
  signed32           d2_noct = 0;  /* number of Oct elements              */

  D20_srt      *d2_p_d20srt = (D20_srt *) NULL;
				    /* pointer to Srt for schema admin.    */
  D20_oct      *d2_p_d20oct = (D20_oct *) NULL;
				    /* pointer to Oct for schema admin.    */
  D20_at       *d2_p_d20at  = (D20_at *) NULL;

  signed32        d2_nd20srt = 0;  /* number of Srt elements              */
  signed32        d2_nd20at  = 0;  /* number of At elements               */
  signed32        d2_nd20oct = 0;  /* number of Oct elements              */

  signed32        wh_function; /* selected ADMIN-function             */
  signed16        d20_bid;     /* current bind-id                     */
  D2_c_arg        *d20_p_c_arg;/* pointer to common arguments         */
  signed16        d20_dir_id=1;/* directory identifier                */
  signed16        d20_auth_mech;/* authentication mechanism           */
  signed32        fl3size;
  signed32        fl4size;
  D2_credent      usercred;
  D2_credent     *p_usercred;
  Bool		d20_printable;



/************** globale variables  (module)                   ***************/

static    D2_c_arg        d20_com_arg;  /* common arguments                  */ 
static    char            d20_test_file[D20_FN_MAX]; /* file name for testfile */

 
static    D2_pbhead       err_inf;     /* errorinformation                   */

#ifndef CACHE_ADMIN
static    D2_name_string  d20_dsa  = (D2_name_string) NULL;
#endif
static    char           *d20_batchfile = (char *) NULL;
static    D2_name_string  d20_name = (D2_name_string) NULL;
static    Octet_string    d20_pwd  = (Octet_string) NULL;
static    signed32        d20_funselect = -1;
static    Bool            d20_params = FALSE;
					/* indicates whether administration*/
					/* was started with parameters     */



/*exon ****************************************************************/
/*								      */
/* TYPE:        MAIN                                                  */
/*								      */
/* NAME  :      dsadmin                 [<short-name>]                */
/*								      */
/* AUTHOR:      U. Richter  D AP 113                                  */
/* DATE:        14.01.88                                              */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/* STANDARD-HEADER:                                                   */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*exoff ***************************************************************/

int main(int argc,		/* nr. of parameters                   */
	char *argv[])		/* pointer-array of parameter strings  */
{
    /************** parameter lists and pointers to           ***************/
    char        disname_arr[ D2_DNL_MAX + 1 ];
				     /* distinguished name of user          */
    D21_p1      plbind;
    D2_password passwd;              /* length and string of password       */
    char        env_dir[D27_LFILE_NAME];
    String      p_env = env_dir ;
	
    char	shfname[D20_SHFNMAX] ;	/* array for holding the path name of */
					/* scheme files */	 
    char	shtstamp[D20_SUTCTIME] ;/* array for holding the time stamp of */
					/* the scheme */
    signed32	namestr, dsastr ;	/* actual structure of user- resp. dsa name */
    signed32    natmpstr ;   /* temporary structure of user- resp. dsa name */
    FILE	*fdcm ;			/* file descriptor of common scheme file */
    signed32    fl1size, fl2size ;
    signed16    rcnt;
    int		npar;
    D20_srt	*srtptr;	
    signed16    idx, opmax;
    signed32    funselect;
    Bool        bind_err;
    Bool        loc_dsa = FALSE;
    char		*dsa_name; /* distinguished name of dsa           */
    char        *pass_wd_str;

#ifdef __hpux
    signed32    terminal = 0;
#endif

    char	blank_pass_wd_str[D20_PASSWD_LEN];

#ifndef CACHE_ADMIN
    char        dsa_disname[ D2_DNL_MAX + 1 ];
    D20_omask	*varfld ;
    char	tree[50] ;		/* field for storing unused return information */
    char	**varinput ;
    signed16	ret_value;
    Bool	attr_name_fld;
    signed32	old_namestr = -1;
#endif

setlocale(LC_ALL, "");

/* inititialize conversion tables for Internationalization.	*/
if (d27_init_conv_tables() == D2_ERROR)
	d20_disp_error(d20_errtab[D20_E105IDX]);

#ifdef CACHE_ADMIN
	d20_cache = TRUE;
#else
	d20_cache = FALSE;
#endif

err_inf.d2_version = D21V022 ;

/* fill in default service controls */
d20_p_c_arg = &d20_com_arg ;
d20_p_c_arg->d2_serv_cntrls = D2_NOCACHE | D2_NOREFERRAL | D2_USEDSA | 
			      D2_DONT_STORE | D2_PREF_ADM_FUN;
d20_p_c_arg->d2_priority    = D2_PRIO_LOW ;
d20_p_c_arg->d2_size_limit  = D2_S_UNLIMITED ;
d20_p_c_arg->d2_time_limit  = D20_373_TIME_LIM ;

/* fill in bind-parameters */
plbind.d21_1pbhead = &err_inf ;
plbind.d21_1dir_id = d20_dir_id ;
usercred.d2_c_passwd = &passwd ;
plbind.d21_1name_dsa = (D2_name_string) NULL ;
plbind.d21_1addr_dsa = (D2_a_value *) NULL ;

getdel[0] = D2_EOS;
getfill = DS_DEFAULT_VALUE;
cgetinit() ;

if( argc == 1 )				/* No parameter			*/
    d20_handlemask( d20_working_mask );	/* Print a message:		*/
					/* "Load message files..."	*/

/* read masks from NLS-file */
if (d20_adm_read_mask_file() == D2_ERROR)
	{
#ifdef _AIX
	/* On IBM-AIX, catopen don't provide any error message	*/
	/* Print one here before to exit.			*/
	perror("NLS file");
#endif
	cgetterm();
	d27_close_conv_tables();
	exit(1);
	}

	sprintf(p_env, "%s%s", dcelocal_path, D27_CLIENT_DIR);

/* Now we know all messages of the catalogs.			*/
/* We can initialize the error message array for handlemask.	*/
/* d20_hdlmsk_errmsg is a global variable.			*/
d20_hdlmsk_errmsg[0]= d20_errtab[D20_103IDX];
d20_hdlmsk_errmsg[1]= d20_errtab[D20_104IDX];

if (getenv(D27_PRINTABLE) != (char *)NULL)
	d20_printable = TRUE;
else
	d20_printable = FALSE;
 
/* get local scheme from scheme files */
if (d20_lget_scheme (&d2_nd20at, &d2_p_d20at,
			    &d2_nd20oct,&d2_p_d20oct, 
			    &d2_nd20srt,&d2_p_d20srt) == D2_ERROR)
	d20_disp_error(d20_errtab[D20_E56IDX]);

/* analyse parameters of "d20adm" - call */
if (argc != 1)
	if (d20_read_params(argc, argv, &plbind) == D2_ERROR)
		{
		cgetterm();
		d27_close_conv_tables();
		exit(1);
		}

d20_tfile = d20_test_file;

if (d20_batchfile != (char *) NULL)
	sprintf(d20_tfile,"%s%s%s", p_env, D20_ADM, d20_batchfile);
else
	sprintf(d20_tfile,"%s%s", p_env, D20_TSTOFILE);

/* get structure types of the 'logon user name' and the 'DSA name' */
/* from common scheme file */

sprintf (shfname, "%s%s", p_env, D20_CMFILE) ;

if ((fdcm = fopen (shfname, "r")) == NULL)
	d20_disp_error(d20_errtab[D20_E56IDX]);

if ((npar = fscanf (fdcm, "%s%d%d", shtstamp, &namestr, &dsastr)) < 1) 
	{
	fclose (fdcm) ;
	d20_disp_error(d20_errtab[D20_E56IDX]);
	}

fclose (fdcm) ;

if (npar < 3) 
	{
	/* no 'logon user name' structure selected -> search one from */
	/* scheme */
	for (srtptr = d2_p_d20srt, rcnt = 0; rcnt < d2_nd20srt;
	     rcnt++,srtptr++)
		if (ds0_check_chgstruct((Octet_string)D2_PWD,
					sizeof(D2_PWD),
					srtptr))
			break ;

	if (rcnt == d2_nd20srt)
		/* error -> no valid name structure found */
		d20_disp_error(d20_errtab[D20_E56IDX]);

	namestr = srtptr->rule_nr;

	/* no 'DSA name' structure selected -> search one from */
	/* scheme */
	for (srtptr = d2_p_d20srt, rcnt = 0; rcnt < d2_nd20srt;
	     rcnt++,srtptr++)
		if (ds0_check_chgstruct((Octet_string)D2_PSAP_ADDRESS, 
					sizeof(D2_PSAP_ADDRESS), srtptr))
	      		break ;

	if (rcnt == d2_nd20srt)
		/* error -> no valid name structure found */
		d20_disp_error(d20_errtab[D20_E56IDX]);

	dsastr = srtptr->rule_nr;

	/* store new structure informations into common scheme file */
	fdcm = fopen (shfname, "w") ;

	if (fdcm != (FILE *) 0)
		{   
		fprintf (fdcm, "%s %d %d\n", shtstamp, namestr, dsastr) ;
	    	fclose (fdcm) ;
		}
	} 	/* end of npar < 3.	*/

/* initialize variables used for mask 1 and mask 2 handling */
pass_wd_str = d20_1omask[D20_1PPOS].d20_ioinfo;

memset((void *)blank_pass_wd_str, ' ', strlen(pass_wd_str));
blank_pass_wd_str[strlen(pass_wd_str)] = '\0';

fl1size = D20_ONM_NM_FLD_LEN;
fl2size = D20_ONM_VAL_FLD_LEN;
fl3size = D20_ONM_NM_FLD_LEN;
fl4size = D20_ONM_VAL_FLD_LEN;

natmpstr = namestr;

#if defined (__hpux)
        if ((terminal = term_type()) == 1) {
           set_stty();
           define_keys(1, 0, "   f1     HELP  ", "\033>");
           define_keys(2, 0, "   f2     MENU  ", "\012");
           define_keys(3, 0, "   f3    SCL-UP ", "\033S");
           define_keys(4, 0, "   f4    SCL-DN ", "\033T");
           define_keys(5, 0, "   f5    INS-CH ", "\033Q");
           define_keys(6, 0, "   f6    DEL-CH ", "\033P");
           define_keys(7, 0, "   f7    DEL-LN ", "\033M");
           define_keys(8, 0, "   f8    CANCEL ", "\003");
           fflush  (stdout);
        }
#endif

for (;;) 
	{
      	*d20_1ifields[DS_LO_PASS] = D2_EOS;

	/* program called without parameters */
      	if(d20_params == FALSE) 
      		{ /* handle logon mask */

		if (d20_handlemask(d20_secomask) == FKTDEL)
			d20_232_spec_exit () ;

#ifndef CACHE_ADMIN
		do 
			{    
			d20_obj_list = FALSE;

			/* generate 'logon user name' struct. within */
			/* variable partition of logon mask */
			
			switch (*(int *)d20_secifields[DS_SEC_SECM]) 
				{
				case D2_SIMPLE:
					/* user id. and password apear.	*/
					d20_1omask[D20_1PPOS].d20_ioinfo = 
								pass_wd_str;
					d20_1omask[D20_1PPOS + 1].d20_fkt_id = 
								D20_CGET;
					break;
				case D2_STRONG:
					/* user id. appears and passwd doesnt */
					d20_1omask[D20_1PPOS].d20_ioinfo = 
							blank_pass_wd_str;
					d20_1omask[D20_1PPOS + 1].d20_fkt_id = 
								D20_NO_OP;
					break;
				case D2_ANONYMOUS:
				case D2_DCE_AUTH:
					/*both user id. and passwd dont appear*/
					d20_1omask[D20_1PPOS].d20_ioinfo = 
							blank_pass_wd_str;
					d20_1omask[D20_1PPOS + 1].d20_fkt_id = 
								D20_NO_OP;
					break;
				}


			/* in case of SIMPLE or STRONG security mech */
			/* get the user name 			     */
			if (*(signed32 *)d20_secifields[DS_SEC_SECM] == 
			      D2_SIMPLE || 
			      *(signed32 *)d20_secifields[DS_SEC_SECM] == 
			      D2_STRONG)
				{
				if (old_namestr != namestr)
					{
					old_namestr = namestr;
					varfld = &d20_1omask[D20_11POS] ;
					d20_gen_name_structure (namestr, 
						&varfld, fl1size, fl2size) ;
					}
				}
			else
				{
				old_namestr = -1;
				varfld = &d20_1omask[D20_11POS] ;
				}

			/* mark unused components of variable part */
			attr_name_fld = TRUE;
			while(varfld < &d20_1omask[D20_11POS + 
						(D20_11CNT * 2)])
				{
				/* naming attribute name field */
				if (attr_name_fld == TRUE)
					{
					memset((void *)
						varfld->d20_ioinfo, ' ',
						(size_t)fl1size);
					attr_name_fld = FALSE;
					}
				else
					{
					/*naming attribute value field*/
					memset((void *)
						varfld->d20_ioinfo, ' ',
						(size_t)fl2size);
					attr_name_fld = TRUE;
					}

				varfld++->d20_fkt_id = D20_CSAY ;
				}

			/* display logon mask.		*/
			*d20_1ifields[DS_LO_BREAK] = DS_DEFAULT_VALUE ;
			d20_1omask[0].d20_fkt_id |= D20_NOERASE;

			d20_handlemask(d20_1omask);
			if (*d20_1ifields[DS_LO_BREAK] != DS_DEFAULT_VALUE)
				break;

			if (*(signed32 *) d20_1ifields[DS_LO_OPTIONS] == 
							DS_L_NAMESTROPT) 
				{
				/* new selection of 'logon user name' 	*/
				/* structure requested 			*/

				ret_value = D2_NOERROR;
				for (;;) 
					{
					strcpy(d20_actopmsg,d20_op[DS__LOGON]);

					*(signed32 *) d20_5ifields[DS_OC_CLASS] = 
					d20_init_rule_nr = namestr;

					/* get a new 'logon user name' struct */
					if ((ret_value = ds0_500_objects (
							FALSE,
							D20_OBJECT, &natmpstr, 
							tree)) == D2_ERROR)
						break ;

					srtptr = d20_search_adm_srule(
						d2_nd20srt, d2_p_d20srt, 
						natmpstr);

					if (ds0_check_chgstruct(
						(Octet_string)D2_PWD,
						sizeof(D2_PWD), srtptr))
					    break;

					/* selected structure is not allowed */
					d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_errtab[D20_E58IDX] ;
					d20_handlemask (d20_qomask) ;

					}

				d20_secomask[0].d20_fkt_id &= ~D20_NOERASE;
				d20_call_cread = FALSE;
				d20_handlemask(d20_secomask);
				d20_call_cread = TRUE;

				if (ret_value == D2_ERROR)
					continue ;

				namestr = natmpstr;
				/* store new structure informations into common scheme file */
				fdcm = fopen (shfname, "w") ;
				if (fdcm != (FILE *) 0)
					{   
					fprintf (fdcm, "%s %d %d\n", shtstamp, 
							namestr, dsastr) ;
				    	fclose (fdcm) ;
					}
				}
			} 
		while (*(signed32 *) d20_1ifields[DS_LO_OPTIONS] == DS_L_NAMESTROPT);

		if (*d20_1ifields[DS_LO_BREAK] != DS_DEFAULT_VALUE)
			{
			d20_secomask[0].d20_fkt_id |= D20_NOERASE;
			continue; /* to main for loop.	*/
			}
#endif

		/* evaluate directory identifier */
		d20_dir_id = (signed16) * ((signed32 *) d20_secifields[DS_SEC_DIR]);

#ifdef CACHE_ADMIN
		d20_auth_mech = D2_ANONYMOUS;
#else
		d20_auth_mech = *((signed32 *)d20_secifields[DS_SEC_SECM]);

		d20_cache = (*(signed32 *) d20_1ifields[DS_LO_OPTIONS] == 
					DS_L_DCACHEOPT) ? TRUE : FALSE ;

		if (d20_auth_mech == D2_SIMPLE || d20_auth_mech == D2_STRONG)
			{
			/* generate the distinguished name of the User */
			sprintf (disname_arr, "");
			varinput = &d20_1ifields[DS_LO_NAME] ;
			d20_gen_distname (namestr, &varinput, disname_arr) ;

			/* Check if a distinguished name is present */
			if(d20_check_logon_dn(disname_arr)
			    == D2_ERROR )
				{
				/* Write an error message: "Invalid    */
				/* user credentials (name, password) ! */
				/* (name, password) unknown !" */
				d20_qomask[D20_Q1POS].d20_ioinfo =
				    d20_errmess[6];
				d20_handlemask (d20_qomask) ;

				/* Return to logon mask */
				d20_secomask[0].d20_fkt_id |= D20_NOERASE;
				d20_1omask[0].d20_fkt_id &= ~D20_NOERASE;
				d20_call_cread = FALSE;
				d20_handlemask(d20_1omask);
				d20_call_cread = TRUE;

				continue ;
				}
			}
#endif
	      	}

#ifndef CACHE_ADMIN

	if(d20_params == FALSE && *(signed32 *) d20_1ifields[DS_LO_OPTIONS] == 
		DS_L_SPEDSAOPT || d20_params == TRUE && d20_dsa != 
		(D2_name_string) NULL && !strcmp((char *)d20_dsa,"SPECIFIC_DSA")) 
		{
		/* logon to a specific DSA -> handle remote logon mask */
		strcpy(d20_actopmsg,d20_op[DS__REM_LOGON]);

		ret_value = d20_180_get_dsa(dsa_disname, p_env);
		
		if (ret_value == D2_ERROR)
			{
			/* go return to logon mask */
			if (d20_params == FALSE)
				{
				d20_secomask[0].d20_fkt_id |= D20_NOERASE;

				d20_1omask[0].d20_fkt_id &= ~D20_NOERASE;
				d20_call_cread = FALSE;
				d20_handlemask(d20_1omask);
				d20_call_cread = TRUE;

				continue ;
				}
			else
				d20_232_spec_exit();
			}
		else
			plbind.d21_1name_dsa = (D2_name_string)&dsa_disname[1];

		} /* end of if statement.	*/

#endif

	if (d20_params == FALSE) /* program called without parameters    */
		{
		/* fill in bind-parameters */
		plbind.d21_1dir_id = d20_dir_id ;

		plbind.d21_1auth_mech = d20_auth_mech;

		switch ((int)plbind.d21_1auth_mech)
			{
			case D2_ANONYMOUS:
			case D2_DCE_AUTH:
				p_usercred = plbind.d21_1u_credent = 
						(D2_credent *) NULL;
				break;
 
			case D2_SIMPLE:
				/* generate user credentials (name etc.) */
				passwd.d2_p_len = 
					d20_strlen(d20_1ifields[DS_LO_PASS]);
				passwd.d2_p_pw = 
					(Octet_string) d20_1ifields[DS_LO_PASS];
				usercred.d2_c_name = 
					(D2_name_string)&disname_arr[1] ;

				p_usercred = plbind.d21_1u_credent = &usercred;
				break;

			case D2_STRONG:
				/* generate user credentials (name etc.) */
				passwd.d2_p_len = 0;
				passwd.d2_p_pw = (Octet_string) NULL;
				usercred.d2_c_name = 
						(D2_name_string)&disname_arr[1];

				p_usercred = plbind.d21_1u_credent = &usercred;
				break;

			}


		plbind.d21_1addr_dsa = (D2_a_value *) NULL ;

#ifndef CACHE_ADMIN
	    	if (*(signed32 *) d20_1ifields[DS_LO_OPTIONS] != DS_L_SPEDSAOPT) 
			{
			/* bind to cache or to default DSA */
			plbind.d21_1name_dsa = (D2_name_string) NULL ;
		    	} 
		else 
			{
			/* bind to a specific DSA, put dsa add. to bind block */
			plbind.d21_1name_dsa = (D2_name_string)&dsa_disname[1] ;
		    	}
#else
			plbind.d21_1name_dsa = (D2_name_string) NULL ;
#endif
		}

	d20_p_c_arg->d2_serv_cntrls = D2_NOCACHE | D2_NOREFERRAL | D2_USEDSA | 
				      D2_DONT_STORE | D2_PREF_ADM_FUN;

	if (d21_1_bind (&plbind) != D2_NO_ERR) 
		{
		bind_err = TRUE;
		/* error doing bind to default DSA */
		if (d20_cache == TRUE) 
			{
			/* bind to cache requested -> ingnore bind */
			/* error (except D2_DIR_UNKNOWN error)     */

			if (err_inf.d2_errvalue == D2_DIR_UNKNOWN)
				/* it is an error not to be ignored. */
				d20_qomask[D20_Q1POS].d20_ioinfo =
				d20_errmess[map_errnum(err_inf.d2_errvalue)];
			else
				d20_msgomask[D20_M1POS].d20_ioinfo = 
				d20_errtab[D20_E59IDX] ;

			if ((d20_params == TRUE) && 
		    		(err_inf.d2_errvalue == D2_DIR_UNKNOWN))
				{   
				cerase();
			    	cgetterm();
			    	fprintf(stderr, "%s\n", d20_qomask[
						D20_Q1POS].d20_ioinfo);
			    	fflush(stderr);
				d27_close_conv_tables();
			    	exit(1);
				}

			if (err_inf.d2_errvalue == D2_DIR_UNKNOWN)
				d20_handlemask (d20_qomask) ;
			else
				d20_handlemask (d20_msgomask) ;

			/* go return to logon mask */
			if (err_inf.d2_errvalue == D2_DIR_UNKNOWN)
				{
				if (rbatch)
			   		d20_232_spec_exit () ;
				else
					{
#ifndef CACHE_ADMIN
					d20_secomask[0].d20_fkt_id |= 
								D20_NOERASE ;

					d20_1omask[0].d20_fkt_id &= 
								~D20_NOERASE;
					d20_call_cread = FALSE;
					d20_handlemask(d20_1omask);
					d20_call_cread = TRUE;
#endif
			   		continue ;
					}
				}
			} 
		else 
			{
			d20_qomask[D20_Q1POS].d20_ioinfo =
				  d20_errmess[map_errnum(err_inf.d2_errvalue)];
			if (d20_params == TRUE)
				{   
				cerase();
			    	cgetterm();
			    	fprintf(stderr, "%s\n", 
					d20_qomask[D20_Q1POS].d20_ioinfo);
			    	fflush(stderr);
				d27_close_conv_tables();
			    	exit(1);
				}

			d20_handlemask (d20_qomask) ;
			/* go return to logon mask */
			if (rbatch)
		   		d20_232_spec_exit () ;
			else
				{
#ifndef CACHE_ADMIN
				d20_secomask[0].d20_fkt_id |= D20_NOERASE;

				d20_1omask[0].d20_fkt_id &= ~D20_NOERASE;
				d20_call_cread = FALSE;
				d20_handlemask(d20_1omask);
				d20_call_cread = TRUE;
#endif
			   	continue ;
				}
			}
		} 
	else 
		{
		bind_err = FALSE;

		/* save bind-id */
		d20_bid = plbind.d21_1bind_id ;
		dsa_name = (char *)plbind.d21_1ret_dsa;

		/* bind to DSA successful -> read scheme */
		if (d20_204_read_scheme (p_env) != D2_NOERROR)
			{
			d20__unbind(d20_bid);
#ifndef CACHE_ADMIN
			d20_secomask[0].d20_fkt_id |= D20_NOERASE;

			d20_1omask[0].d20_fkt_id &= ~D20_NOERASE;
			d20_call_cread = FALSE;
			d20_handlemask(d20_1omask);
			d20_call_cread = TRUE;
#endif
			continue ;
			}
		}

	/* prepare administration of cache */
	if (d20_cache) 
		{
		/* administration of cache requested -> unbind to DSA */
		if (bind_err == FALSE)
			d20__unbind(d20_bid);

		d20_p_c_arg->d2_serv_cntrls = D2_DUACACHE | D2_PREF_ADM_FUN;
		plbind.d21_1bind_id = d20_bid = d20_dir_id;
		/* no bind to DUA-CACHE will be performed */
		} 
	else 
		{
		/* checks if bind dsa is local dsa */
		loc_dsa = d20_check_bind_dsa(dsa_name,p_env,d20_dir_id);
		}

	if (d20_funselect < 0)
      		{
		*d20_3ifields[1] = DS_DEFAULT_VALUE ;

		if (d20_cache == TRUE)
			{
			if (*(signed32 *)d20_3ifields[0] > DS__SCHEME_ADM)
				*(signed32 *)d20_3ifields[0] = DS__BASIC_ADM;

			d20_3omask[DS_CA_OP_IDX].d20_fkt_id = D20_CSAY;
			d20_3omask[DS_SCH_OP_IDX].d20_fkt_id =D20_NO_OP;
			for (idx = DS_OP_MINIDX; idx <= DS_OP_MAXIDX; 
							idx++)
				d20_3omask[idx].d20_fkt_id = D20_NO_OP;

			opmax = DS__SCHEME_ADM;
			}
		else
			{
			d20_3omask[DS_CA_OP_IDX].d20_fkt_id = D20_NO_OP;
			d20_3omask[DS_SCH_OP_IDX].d20_fkt_id = D20_CSAY;

			for (idx = DS_OP_MINIDX; idx <= DS_OP_MAXIDX; 
								idx++)
				d20_3omask[idx].d20_fkt_id = D20_CSAY;

			opmax = DS__HANDLE_TREE;

			if(d20_gen_dnintf(dsa_name) != D2_NOERROR)
				d20_232_spec_exit();

			*d20_3ofields[DS_OP_2FLD] = D2_EOS;
			strncpy(d20_3ofields[DS_OP_1FLD], dsa_name,
				D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT);

			if ((int)strlen(dsa_name) > 
					D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT)
				/* fill second line                */
				strncpy(d20_3ofields[DS_OP_2FLD],
				dsa_name + 
				(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT),
				D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT);
			}

		/* mask 3 handling */
		for (;;) 
			{
			d20_handlemask (d20_3omask) ;

			if ((*d20_3ifields[1] != DS_DEFAULT_VALUE) ||
			   (*(signed32 *)d20_3ifields[0] == DS_EXIT))
				/* go return to logon mask */
				{   
				if (d20_cache == FALSE)
			    		{   
					d20__unbind(d20_bid);
			    		}

			    	if (d20_params == TRUE)
					d20_232_spec_exit();

			    	break ;
				}

			/* check selected function */
			funselect = *(signed32 *)d20_3ifields[0] ;
			d20_call(funselect,p_env,opmax,loc_dsa);
			if (funselect == DS__UNBIND && rbatch)
			    	break;
			}
	      	}
	else
		{
		opmax = d20_cache == TRUE ? DS__SCHEME_ADM :
					       DS__HANDLE_TREE;
		d20_call(d20_funselect,p_env,opmax,loc_dsa);
		d20_232_spec_exit();
	      	}

#ifndef CACHE_ADMIN
	d20_secomask[0].d20_fkt_id |= D20_NOERASE;

	d20_1omask[0].d20_fkt_id &= ~D20_NOERASE;
	d20_call_cread = FALSE;
	d20_handlemask(d20_1omask);
	d20_call_cread = TRUE;
#endif
	}

}



/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME  :      d20_232_spec_exit       [<short-name>]                */
/*								      */
/* AUTHOR:      U. Richter  D AP 113                                  */
/* DATE:        12.02.88                                              */
/*                                                                    */
/* DESCRIPTION: to get a defined exit ( shut screen functions )       */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/* STANDARD-HEADER:                                                   */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX	: */						    /**/
/*								      */
/*exoff****************************************************************/

signed16 d20_232_spec_exit(void)
{
	cerase() ;
	cgetterm() ;
	d27_close_conv_tables();
	exit (0) ;
}


/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_adm_read_mask_file()                              */
/*								      */
/* AUTHOR:      W. Schmid   D AP 11                                   */
/* DATE:        24.10.89                                              */
/*                                                                    */
/* DESCRIPTION: signed32 d20_adm_read_mask_file()                     */
/*                                                                    */
/*              This function reads the NLS file with all texts for   */
/*              masks and error messages.                             */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:         None                                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

signed32 d20_adm_read_mask_file(void)
{
char            *m_str;
nl_catd          nl_fd;
signed32         msg_set;
signed32         groesse;
signed32         i;

char             length[D2_LC_LONG];
char            *p_length = length;
char            *p_len[1];
static char     *upd_time[D20_NUPD_TIME];
static char     *upd1_time[D20_NUPD1_TIME];
static char     *opmsg[D20_NOPMSG];
static char     *s_opmsg[D20_NS_OPMSG];
static char     *opt[D20_NOPT];
static char     *format[D20_NFORMAT];
static char     *job_format[D20_NJOB_FORMAT];

static char     *d20_att_syntax[D20_NATT_SYNTAX];
static char	*d20_acl_intrp[1];

static D20_omask *mask[] =
		 { d20_1omask,
		   d20_2omask,
		   d20_3omask,
		   d20_4omask,
		   d20_4aomask,
		   d20_5omask,
		   d20_6omask,
		   d20_6aomask,
		   d20_6bomask,
		   d20_7omask,
		   d20_7aomask,
		   d20_8omask,
		   d20_9omask,
		   d20_Aomask,
		   d20_Bomask,
		   d20_Comask,
		   d20_Domask,
		   d20_E1omask,
		   d20_E2omask,
		   d20_Eaomask,
		   d20_Ebomask,
		   d20_Ecomask,
		   d20_10omask,
		   d20_11aomask,
		   d20_11bomask,
		   d20_qomask,
		   d20_msgomask,
		   d20_frameomask,
		   d20_12omask,
		   d20_13omask,
		   d20_14omask,
		   d20_dnscellomask,
		   d20_dnsrepomask,
		   d20_ttxidomask,
		   d20_telexomask,
		   d20_t61omask,
		   d20_faxomask,
		   d20_Edomask,
		   d20_E3omask,
		   d20_oraddomask,
		   d20_moraddomask,
		   d20_noraddomask,
		   d20_soraddomask,
		   d20_uoraddomask,
		   d20_toraddomask,
		   d20_dlpermomask,
		   d20_ornameomask,
		   d20_mogomask,
		   d20_certomask,
		   d20_certpomask,
		   d20_certlomask,
		   d20_rcertomask,
		   d20_12bomask,
		   d20_dmeomask,	/* DME mask	*/
		   d20_secomask,
		   (D20_omask *) 0
		   };

static D20_strings  nls_1strings[] =
			{ { D20_NUPD_TIME, upd_time },
			  { D20_NOP-1, &d20_op[1] },
			  { D20_NOPMSG, opmsg },
			  { D20_NCOP, d20_cop },
			  { D20_NS_OPMSG, s_opmsg },
			  { D20_NDUPD_OP-1, &d20_dupd_op[1] },
			  { D20_NTREE_OP-1, &d20_tree_op[1] },
			  { D20_NDISP, d20_disp },
			  { D20_NOPT, opt },
			  { D20_NOPCLASS, d20_opclass },
			  { D20_NFORMAT, format },
			  { D20_NUPD1_TIME, upd1_time },
			  { D20_NJOB_FORMAT, job_format },
			  { D20_MAX_ERR_TAB, d20_errtab },
			  { D20_MAX_ERR_TABLE, d20_err_table },
			  { D20_MAX_ERR_MESS, d20_errmess },
			  { 0, (char **) 0 }
			};

static D20_strings  nls_2strings[] =
			{ { D20_NERR_HEADER, d20_14oheader },
			  { D20_NATT_SYNTAX, d20_att_syntax },
			  { D20_NBOOL, d20_bool },
			  { 1, d20_acl_intrp },
			  { 0, (char **) 0 }
			};

if ((nl_fd = catopen(D20_ADM_NLSFILE, 0)) == (nl_catd) -1)
    return(D2_ERROR);

if ((nl_help_fd = catopen(D20_ADMHELP_NLSFILE, 0)) == (nl_catd) -1)
    return(D2_ERROR);

msg_set = 1;

if (d20_read_str(nl_fd, msg_set, 1, &p_length, p_len) == D2_ERROR)
{   catclose(nl_fd);
    return(D2_ERROR);
}
groesse = atoi(length);

if ((m_str = (char *)malloc((size_t)groesse)) == (char *) 0)
{   fprintf(stderr, "Error while allocating memory for reading NLS file\n");
    catclose(nl_fd);
    return(D2_ERROR);
}
msg_set++;

for (i = 0; nls_1strings[i].msg_no != 0; i++, msg_set++)
{   if (d20_read_str(nl_fd, msg_set, nls_1strings[i].msg_no, &m_str,
		     nls_1strings[i].msg_str) == D2_ERROR)
    {   catclose(nl_fd);
	return(D2_ERROR);
    }
}


d20_fhour = upd_time[0];
d20_fd_of_week = upd_time[2];

d20_op[0] = "";

for (i=0; i<D20_NOPMSG; i++)
{   d20_opmsg[i].d20_msgptr = opmsg[i];
}

for (i=0; i<D20_NS_OPMSG; i++)
{   d20_s_opmsg[i].d20_msgptr = s_opmsg[i];
}

d20_dupd_op[0] = "";

d20_tree_op[0] = "";

d20_2_1opt = opt[0];
d20_2_2opt = opt[1];

d20_E11format = format[0];
d20_E12format = format[1];

d20_F_mints = upd1_time[0];
d20_F_hrs = upd1_time[1];
d20_F_days = upd1_time[2];

d20_F_job_state = job_format[0];
d20_F_act_mod = job_format[1];
d20_F_notact_mod = job_format[2];

for (i=0; mask[i] != (D20_omask *) 0; i++, msg_set++)
{    if (d20_mask_init(nl_fd, msg_set, &m_str, mask[i]) == D2_ERROR)
     {   catclose(nl_fd);
	 return(D2_ERROR);
     }
}

d20_Ebomask[DS_EB_HOURS].d20_oformat = d20_fhour;

d20_Ecomask[DS_EC_DOW].d20_oformat = d20_fd_of_week;
d20_Ecomask[DS_EC_HOURS].d20_oformat = d20_fhour;

d20_Edomask[DS_ED_DOW1].d20_oformat = d20_fd_of_week;
d20_Edomask[DS_ED_HOURS1].d20_oformat = d20_fhour;
d20_Edomask[DS_ED_DOW2].d20_oformat = d20_fd_of_week;
d20_Edomask[DS_ED_HOURS2].d20_oformat = d20_fhour;

for (i = 0; nls_2strings[i].msg_no != 0; i++, msg_set++)
{   if (d20_read_str(nl_fd, msg_set, nls_2strings[i].msg_no, &m_str,
		     nls_2strings[i].msg_str) == D2_ERROR)
    {   catclose(nl_fd);
	return(D2_ERROR);
    }
}

d20_Bomask[D20_B7POS + 1].d20_oformat = d20_att_syntax[0];

if (rbatch)
	{
	d20_6bomask[D20_6A5POS].d20_oformat = 
	d20_6bomask[D20_6A6POS].d20_oformat = 
	d20_6bomask[D20_6A7POS].d20_oformat = 
	d20_6bomask[D20_6A8POS].d20_oformat = d20_acl_intrp[0];
	}

catclose(nl_fd);
return(D2_NOERROR);
}


/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_read_params()                                     */
/*								      */
/* AUTHOR:      E. Kraemer  D AP 11                                   */
/* DATE:        24.10.89                                              */
/*                                                                    */
/* DESCRIPTION: signed32 d20_read_params(argc, argv, plbind)          */
/*              int     argc;       number of arguments to be checked */
/*              char    *argv[];    arguments to be checked           */
/*              D21_p1  plbind;     parameter block for bind          */
/*								      */
/*              This function analyses all parameters and builds the  */
/*              parameter block for "DS_BIND".                        */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              int     argc;       number of arguments to be checked */
/*              char    *argv[];    arguments to be checked           */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*              D21_p1  plbind;     parameter block for bind          */
/*								      */
/* RETURNVALUE:         None                                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

signed32 d20_read_params(int argc, char *argv[], D21_p1 *plbind)
{
signed32      idx;
char    *str;
Bool     dirid_found = FALSE;

#ifndef CACHE_ADMIN
signed32	i;
static char	sp_dsa_name[D2_DNL_MAX + 1];
static char	usr_name[D2_DNL_MAX + 1];
#endif


			/* default value of the authentication mechanism */
plbind->d21_1auth_mech = D2_ANONYMOUS;


				/* analyse parameters                 */
for (idx = 1; idx < argc; idx++)
{   if (argv[idx][0] != '-')
    {                           /* batch output file assumed          */
	d20_batchfile = argv[idx];
    }
    else
    {   d20_params = TRUE;
	switch((int) argv[idx][1])
	{   case 'i':   for (str = argv[idx] + 2; *str != D2_EOS; str++)
			{   if (isdigit(*str) == 0)
			    {   if (d20_param_error(D20S_ERRMESS, D20_E02IDX)
						    == D2_ERROR)
				{   fprintf(stderr, "\nillegal DIR-ID\n");
				}
				return(D2_ERROR);
			    }
			}
			if (str == (argv[idx] + 2))
			{   if (d20_param_error(D20S_ERRMESS, D20_E02IDX)
						== D2_ERROR)
			    {   fprintf(stderr, "\nillegal DIR-ID\n");
			    }
			    return(D2_ERROR);
			}

			plbind->d21_1dir_id = d20_dir_id =
					      atoi(argv[idx] + 2);
			dirid_found = TRUE;
			break;
	    case 'o':   for (str = argv[idx] + 2; *str != D2_EOS; str++)
			{   if (isdigit(*str) == 0)
			    {   if (d20_param_error(D20S_ERR_TAB, D20_E00IDX + 1) == D2_ERROR)
				{   fprintf(stderr, "\nunknown operation\n");
				}
				return(D2_ERROR);
			    }
			}
			if (str == (argv[idx] + 2))
			{   if (d20_param_error(D20S_ERR_TAB, D20_E00IDX + 1) == D2_ERROR)
			    {   fprintf(stderr, "\nunknown operation\n");
			    }
			    return(D2_ERROR);
			}
			d20_funselect = atoi(argv[idx] + 2);
			break;

#ifndef CACHE_ADMIN
	    case 'd':   
    			plbind->d21_1name_dsa = (D2_name_string) NULL ;

			if (strcmp(argv[idx] + 2, "CACHE") == 0)
			{   d20_p_c_arg->d2_serv_cntrls = 
						D2_DUACACHE | D2_PREF_ADM_FUN;
			    d20_cache = TRUE;
			}
			else
				if (strcmp(argv[idx] + 2, "SPECIFIC_DSA") == 0)
					d20_dsa = (D2_name_string)(argv[idx]+2);
				else
					{
					strcpy(sp_dsa_name, argv[idx] + 2);
					if (ds0_508_check_name(sp_dsa_name) ==
								D2_NO_ERR)
    						plbind->d21_1name_dsa = 
						(D2_name_string)sp_dsa_name;
					else
						{
			    			fprintf(stderr,
						"\nWrong DN format !\n");
						return(D2_ERROR);
						}
					}
		
			break;
	    case 'p':   plbind->d21_1u_credent->d2_c_passwd->d2_p_len =
						       strlen(argv[idx] + 2);
			if ((d20_pwd = (Octet_string) malloc((size_t)
			  (plbind->d21_1u_credent->d2_c_passwd->d2_p_len+1)))
			  == (Octet_string)NULL)
			{   if (d20_param_error(D20S_ERR_TABLE, D20_E04IDX + 1) == D2_ERROR)
			    {   fprintf(stderr,"\ninternal error while allocating memory !\n");
			    }
			    return(D2_ERROR);
			}
			else
			{
			    strcpy((char *)d20_pwd,argv[idx] + 2);
			    plbind->d21_1u_credent->d2_c_passwd->d2_p_pw =
						    (Octet_string)d20_pwd;
			    for(i = 0; i < plbind->d21_1u_credent->
					     d2_c_passwd->d2_p_len; i++)
			    {   argv[idx][2 + i] = 'x';
			    }
			}
			break;
	    case 'u':   
			strcpy(usr_name, argv[idx] + 2);

			if (ds0_508_check_name(usr_name) == D2_NO_ERR)
				d20_name = plbind->d21_1u_credent->d2_c_name =
						(D2_name_string)usr_name;
			else
				{
				fprintf(stderr, "\nWrong DN format !\n");
				return(D2_ERROR);
				}
			break;
#endif

	    case 'A':		/* Parameter for authentication mechanism */
			switch(d20_translate_auth_mech_param(*(argv[idx] + 2)))
			    {			   /* Let's see the value */
			    /* Well, the specification doesn't allow any  */
			    /* value for an anonymous authentication.     */
			    /*
			    case D2_ANONYMOUS:
				plbind->d21_1auth_mech = D2_ANONYMOUS;
				break;
			    */
			    case D2_SIMPLE:
				plbind->d21_1auth_mech = D2_SIMPLE;
				break;
			    case D2_DCE_AUTH:
				plbind->d21_1auth_mech = D2_DCE_AUTH;
				break;
			    case D2_STRONG:
				plbind->d21_1auth_mech = D2_STRONG;
				break;
			default:		/* Bad parameter	*/
				plbind->d21_1auth_mech = D2_ANONYMOUS;
				if( d20_param_error( D20S_ERRMESS,
						     D20_E82IDX+1 )
				    == D2_ERROR )
				    fprintf(stderr,
			"Value for the authentication mechanism not valid.\n");
				if( d20_param_error( D20S_ERRMESS,
						     D20_E83IDX+1 )
				    == D2_ERROR )
				    fprintf(stderr,
"Use nothing for anonymous or 2 for simple or 6 for strong or 5 for DCE.\n");
				return(D2_ERROR);
				break;		/* unreachable	*/
			    }

			break;

	    default:    if (d20_param_error(D20S_ERR_TABLE, D20_E42IDX + 1) == D2_ERROR)
			{   fprintf(stderr, "\nunknown parameter !\n");
			}
			return(D2_ERROR);
			break;
	}
    }
}

if (d20_params == FALSE)
    return(D2_NOERROR);

if (dirid_found == FALSE)
{   if (d20_param_error(D20S_ERR_TABLE, D20_E41IDX + 1) == D2_ERROR)
    {   fprintf(stderr, "\nDIR-Id missing\n");
    }
    return(D2_ERROR);
}

switch( d20_check_params(plbind->d21_1auth_mech, d20_name, d20_pwd) )
{				/* In error case, print an error message */
    case D20_ANONYMOUS_ERR:
	if( d20_param_error( D20S_ERRMESS, D20_E78IDX+1 ) == D2_ERROR )
	    fprintf(stderr, "With an anonymous authentication, \
do not enter a user name or a password.\n");
	return(D2_ERROR);
	break;			/* unreachable	*/
    case D20_DCE_AUTH_ERR:
	if( d20_param_error( D20S_ERRMESS, D20_E79IDX+1 ) == D2_ERROR )
	    fprintf(stderr, "With DCE authentication, \
do not enter a user name or a password.\n");
	return(D2_ERROR);
	break;			/* unreachable	*/
    case D20_SIMPLE_ERR:
	if( d20_param_error( D20S_ERRMESS, D20_E80IDX+1 ) == D2_ERROR )
	    fprintf(stderr,
	"With simple authentication, enter a user name and a password.\n");
	return(D2_ERROR);
	break;			/* unreachable	*/
    case D20_STRONG_ERR:
	if( d20_param_error( D20S_ERRMESS, D20_E81IDX+1 ) == D2_ERROR )
	    fprintf(stderr,
	"With strong authentication, enter a user name but no password.\n");
	return(D2_ERROR);
	break;			/* unreachable	*/
    case D20_AUTH_NO_ERROR:	/* OK, correct combinaison of parameters */
	switch( plbind->d21_1auth_mech )
	{
	case D2_ANONYMOUS:
	case D2_DCE_AUTH:
	    plbind->d21_1u_credent = p_usercred = (D2_credent *)NULL;
	    break;
	case D2_SIMPLE:
	    /* This should have already a user name and a user password */
	    break;
	case D2_STRONG:
	    plbind->d21_1u_credent->d2_c_passwd->d2_p_len = 0;
	    break;
	default:
	    /* Not possible - or this is a programming error ! */
	    return(D2_ERROR);
	    break;
	}
	break;
    case D20_AUTH_ERROR:
    default:
	/* Not possible - or this is a programming error ! */
	return(D2_ERROR);
	break;			/* unreachable	*/
}

return(D2_NOERROR);
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_check_params                          */
/* Description         : Check the combination of the parameters   */
/*                       given at the command line.                */
/*                       According to the authentication           */
/*                       mechanism, user name and password are     */
/*                       required or not.                          */
/* Return Value        : D20_AUTH_NO_ERROR Combination correct     */
/*                       D20_DCE_AUTH_ERR  user name or password   */
/*                                         should not be entered   */
/*                       D20_SIMPLE_ERR    user name or password   */
/*                                         should be entered       */
/*                       D20_STRONG_ERR    user name should be     */
/*                                         entered, but not a      */
/*                                         password                */
/*                       D20_AUTH_ERROR    Other errors (there     */
/*                                         is a programming        */
/*                                         error if this occurs)   */
/* Author              : Jean-Marc Bonnaudet, SNI BU BA NM 123     */
/* Date                : Thu Feb 24 12:05:44 MET 1994              */
/*                                                                 */
/*-----------------------------------------------------------------*/

static signed32 d20_check_params(signed16 auth_mech,
			    D2_name_string user_name,
			    Octet_string user_pwd)
{
signed32 ret_val = D20_AUTH_NO_ERROR;

    switch(auth_mech)
    {
    case D2_ANONYMOUS:			/* no user name no password	*/
    case D2_DCE_AUTH:			/* no user name no password	*/
	if( user_name != (D2_name_string)NULL
	 || user_pwd  != (Octet_string)NULL)
	    ret_val = D20_DCE_AUTH_ERR;
	break;
    case D2_SIMPLE:			/* user name and password	*/
	if( user_name == (D2_name_string)NULL
	 || user_pwd  == (Octet_string)NULL )
	    ret_val = D20_SIMPLE_ERR;
	break;
    case D2_STRONG:			/* user name but no password	*/
	if( user_name == (D2_name_string)NULL
	 || user_pwd  != (Octet_string)NULL )
	    ret_val = D20_STRONG_ERR;
	break;
    default:				/* In this case, there is	*/
	ret_val = D20_AUTH_ERROR;	/* a programming error		*/
	break;
    }

return(ret_val);
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_translate_auth_mech_param             */
/* Description         : Translate the input value of the          */
/*                       authentication mechanism in the command   */
/*                       line parameter in a value used in gds     */
/* Return Value        : A value of authentication mechanism type  */
/*                       or D2_BAD_VALUE                           */
/* IMPORTANT           : Please take care that D2_BAD_VALUE has    */
/*                       an other value a an authentication        */
/*                       mechanism (D2_ANONYMOUS, D2_SIMPLE,       */
/*                       D2_DCE_AUTH, D2_STRONG)                   */
/* Author              : Jean-Marc Bonnaudet, SNI BU BA NM 123     */
/* Date                : Thu Feb 24 16:32:07 MEZ 1994              */
/*                                                                 */
/*-----------------------------------------------------------------*/

static signed32 d20_translate_auth_mech_param(char auth_mech_param)
{
signed32 ret_val;

    switch(auth_mech_param)
    {
	case '0': ret_val = D2_ANONYMOUS; break;
	case '2': ret_val = D2_SIMPLE;    break;
	case '6': ret_val = D2_STRONG;    break;
	case '5': ret_val = D2_DCE_AUTH;  break;
	default:  ret_val = D2_BAD_VALUE; break;
    }

    return( ret_val );
}



/*inon ****************************************************************/
/*								      */
/* TYPE:        void                                                  */
/*								      */
/* NAME  :      d20_call                [call]                        */
/*								      */
/* AUTHOR:      E. Kraemer  D AP 11                                   */
/* DATE:        24.10.89                                              */
/*                                                                    */
/* DESCRIPTION: static  void d20_call(funselect,p_env,opmax,loc_dsa)  */
/*								      */
/*              This function calls the selected administration       */
/*              function.                                             */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              funselect   signed32    number of selected function   */
/*              p_env       String      value of $DIRINFBAS           */
/*              loc_dsa     Bool                                      */
/*								      */
/* OUTPUT-PARAMETERS:   None                                          */
/*								      */
/* RETURNVALUE:         None                                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

static void d20_call(
	signed32 funselect,
	String   p_env,
	signed16 opmax,
	Bool     loc_dsa)
{                               /*  d20_call                          */
 
/* parameter lists */
D20_pS      plfktS;
D20_pE      plfktE;
signed16    rc;


if (funselect == DS__UNBIND && rbatch)
   { if (d20_cache == FALSE)
     { d20__unbind(d20_bid);
       return;
     }
   }
if (funselect >= DS__STOP &&
    funselect <= opmax)
{
/* distribute to selected function */
/* if '2' is selected and cache = TRUE       */
/* set funselect to CACHE_UPDATE             */
  if (funselect == DS__SCHEME_ADM &&
      d20_cache == TRUE)
      funselect = DS__CACHE_UPDATE;

  d20_shadow = FALSE;

  switch ((int) funselect) {
    case DS__STOP:
	if (d20_cache == FALSE)
	{   d20__unbind(d20_bid);
	}
	d20_232_spec_exit () ;
	break ;
    case DS__BASIC_ADM:
	break;
    case DS__SCHEME_ADM:
	plfktS.d20_Spbhead = &err_inf ;
	plfktS.d20_Senv   = p_env;
	break;
    case DS__DELTA_UPDATE:
	if (loc_dsa == FALSE)
	{				/* display error message	*/
	      d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E56IDX] ;
	      d20_handlemask (d20_qomask) ;
	      return;
	}
	break;
    case DS__HANDLE_TREE:
	break;
    case DS__CACHE_UPDATE:
	break;
    default:
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E00IDX] ;
	d20_handlemask (d20_qomask) ;
	return;
  }
  if (funselect != DS__SCHEME_ADM) {
    plfktE.d20_Epbhead = &err_inf ;
    plfktE.d20_Esrt    = d2_p_d20srt ;
    plfktE.d20_Ensrt   = d2_nd20srt;
    plfktE.d20_Eoct    = d2_p_d20oct ;
    plfktE.d20_Enoct   = d2_nd20oct;
    plfktE.d20_Eat     = d2_p_d20at;
    plfktE.d20_Enat    = d2_nd20at ;
    plfktE.d20_Eenv  = p_env;
    plfktE.d20_Edir_id = d20_dir_id;
    plfktE.d20_Ebid = d20_bid;
  }
}
else {
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E00IDX] ;
	d20_handlemask (d20_qomask) ;
	return;
     }
strcpy(d20_actopmsg,d20_op[funselect]);
wh_function = funselect;

/* call selected function */
switch(funselect)
    {
    case DS__STOP:
	rc = d20_232_spec_exit();
	break;
    case DS__BASIC_ADM:
	rc = d20_1_admin( &plfktE );
	break;

    #ifndef CACHE_ADMIN
    case DS__SCHEME_ADM:
	rc = d20_2_admin_scheme( &plfktS );
	break;
    case DS__HANDLE_TREE:
	rc = d20_E_tree_handling( &plfktE );
	break;
    case DS__DELTA_UPDATE:
	rc = d20_C_delta_update( &plfktE );
	break;
    #endif

    case DS__CACHE_UPDATE:
	rc = d20_D_cache_update( &plfktE );
	break;
    default:
	/* Should not happen ! */
	break;
    }

}                           /*  d20_call                              */



/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_disp_error(message)                               */
/*								      */
/* AUTHOR:      W. Schmid D AP 11                                     */
/* DATE:        24.10.89                                              */
/*                                                                    */
/* DESCRIPTION: void d20_disp_error(message)                          */
/*              char    *message;   error message to display          */
/*								      */
/*              This function will display a serious error and will   */
/*              terminate the process.                                */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              char    *message;   error message to display          */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:         None                                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

signed32 d20_disp_error(char *message)
{
    if (d20_params == FALSE)
    {   d20_qomask[D20_Q1POS].d20_ioinfo = message ;
	d20_handlemask (d20_qomask) ;
	d20_232_spec_exit () ;
    }
    else
    {   cerase();
	cgetterm();
	fprintf(stderr, "\n%s\n", message);
	fflush(stderr);
	d27_close_conv_tables();
#ifdef __hpux
	if ((term_type()) == 1) reset_hp_term();
#endif
	exit(1);
    }

    return(0);	/* unreachable	*/
}


/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_param_error(msg_set, msg_no)                      */
/*								      */
/* AUTHOR:      W. Schmid D AP 11                                     */
/* DATE:        24.10.89                                              */
/*                                                                    */
/* DESCRIPTION: d2_ret_val d20_param_error(msg_set, msg_no)           */
/*              signed32 msg_set;    message set in NLS-File          */
/*              signed32 msg_no;     message number in NLS-File       */
/*								      */
/*              This function will read the specified error message   */
/*              from the NLS file and will display it on the screeen. */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed32 msg_set;    message set in NLS-File          */
/*              signed32 msg_no;     message number in NLS-File       */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:                                                       */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

d2_ret_val d20_param_error(signed32 msg_set, signed32 msg_no)
{
char    *message;
nl_catd  nl_fd;
char    *def_str = "DEFAULT";
if ((nl_fd = catopen(D20_ADM_NLSFILE, 0)) == (nl_catd) -1)
    return(D2_ERROR);

if ((message = catgets(nl_fd, msg_set, msg_no, def_str)) == def_str)
{   fprintf(stderr, "Error while reading NLS file; msg_set = %d msg_no = %d\n",
	    msg_set, msg_no);
    return(D2_ERROR);
}

fprintf(stderr, "%s\n", message);
fflush(stderr);
catclose(nl_fd);

return(D2_NOERROR);
}


/*inon ****************************************************************/
/*								      */
/* TYPE:        Bool                                                  */
/*								      */
/* NAME  :      d20_check_bind_dsa()                                  */
/*								      */
/* AUTHOR:      V. Mueller  D AP 11                                   */
/* DATE:        15.01.90                                              */
/*                                                                    */
/* DESCRIPTION: Bool d20_check_bind_dsa(bind_dsa, path, dir_id)       */
/*              char     bind_dsa;  name of bind dsa                  */
/*              char    *path;      path of the installation          */
/*              signed16 dir_id;    directory id                      */
/*								      */
/*              This function checks if the bind dsa is the local dsa */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              char     bind_dsa;  name of bind dsa                  */
/*              char    *path;      path of the installation          */
/*              signed16 dir_id;    directory id                      */
/*								      */
/* OUTPUT-PARAMETERS:   None                                          */
/*								      */
/* RETURNVALUE:         None                                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

Bool d20_check_bind_dsa(char *bind_dsa, char *path, signed16 dir_id)

{
char filename[D27_LFILE_NAME];
char local_dsa[D2_DNL_MAX];	/* distinguished name of local DSA */
int  local_dsa_len;
FILE *fploc;
Bool return_value;

/* get filename */
sprintf(filename, "%s%s%s%s%d%s", dcelocal_path, D27_SERVER_DIR, D27_DSA_DIR,
				  D27_SUBDSA_DIR, dir_id, D27_DSANAME);

    if((fploc = fopen(filename, "r")) == (FILE * ) 0)	/* open of file     */
	return_value = FALSE;				/* file isn't there */
    else
    {
	if (fgets(local_dsa, D2_DNL_MAX - 1, fploc) == (char *) 0)
	    return_value = FALSE;			/* file empty       */
	else if (bind_dsa == (char *)NULL)
	    return_value = FALSE;
	else
	{
	    local_dsa_len = strlen(local_dsa);
	    if(local_dsa[local_dsa_len - 1] == '\n')
		{
		local_dsa_len--;
		local_dsa[local_dsa_len] = D2_EOS;
		}

	    return_value = d20_dsacmp(local_dsa, bind_dsa);
	}

	fclose(fploc);
    }

    return( return_value );

}


#ifndef CACHE_ADMIN

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_check_logon_dn()                      */
/* Description         : Check if the credentials are empty or not */
/* Return Value        : D2_NOERROR if values are present          */
/*                       D2_ERROR if fields are empty              */
/* Author              : Jean-Marc Bonnaudet, SNI BU BA NM 123     */
/*                                                                 */
/*-----------------------------------------------------------------*/

static signed32 d20_check_logon_dn(char *credential)
{
signed32 return_value;

    if( strlen(credential) )	return_value = D2_NOERROR;
    else			return_value = D2_ERROR;

    return( return_value );
}

#endif /* ifndef CACHE_ADMIN */


/* End of d20logon.c */
