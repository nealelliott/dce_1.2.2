/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: d20cupcach.c,v $
 * Revision 1.1.4.2  1996/02/18  19:41:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:20  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:18:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:22  root]
 * 
 * Revision 1.1.2.4  1994/09/06  12:24:36  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:12:19  keutel]
 * 
 * Revision 1.1.2.3  1994/07/06  15:06:32  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:11:55  marrek]
 * 
 * Revision 1.1.2.2  1994/06/09  18:44:36  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:19  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:14:20  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:09:43  marrek]
 * 
 * 	Created for April Submission.
 * 	[1994/04/26  12:49:21  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20cupcach.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 19:41:36 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*  PROGRAM: d20cupdate                                                      */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The program updates the content of the DUA-cache which is specified  */
/*      by a directory identifier. The updating is done by first ascertai-   */
/*      ning the names of all objects contained in the DUA-cache (DS-SEARCH).*/
/*      Then for every found object all attribute types resp. attribute      */
/*      values are read from the DUA-cache and then from a DSA. The two      */
/*      attribute information blocks are compared and the DUA-cache entry    */
/*      is updated (DS-ADD) if any difference between the two blocks is      */
/*      detected. A DUA-cache entry is deleted (DS-REMOVE) if there is no    */
/*      corresponding entry within a DSA.                                    */
/*      Notes: The two DUA-cache short form entries (used for referencing    */
/*      the 'local'-DSA and 'client'-information) are never updated by the   */
/*      DUA-cache update process.                                            */
/*      The short form entry used for referencing the available 'default'-   */
/*      DSA's is updated by comparing its 'see also'-information with the    */
/*      referenced DUA-cache entries and also with the entries read from a   */
/*      DSA. If a referenced 'default'-DSA entry is not available in the     */
/*      DUA-cache or available in the DUA-cache but not in a DSA, then this  */
/*      reference information is removed from the entry.                     */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      argv[1]     = directory identifier.                                  */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    None                                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                         DATE: 25.4.88     */
/*****************************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <dce/d2dir.h>
#include <d21iapl.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>

/* ------------------------ declaration of constants ----------------------- */

#define D20_1RBUFLNG	32000
#define D20_2RBUFLNG	16000
#define D20_3RBUFLNG	 5000
#define D20_MINCALLPAR	   2
#define D20_DEL		0x3A		/* locality <-> dist. name delimiter */

#define D20_1ERRTXT	"ERROR: environment variable 'DIRINFBAS' incorrect\n"
#define D20_2ERRTXT	"ERROR: can't open log-file '%s'\n"
#define D20_3ERRTXT	"ERROR: directory identifier missing\n"
#define D20_4ERRTXT	"ERROR: %s <- invalid directory identifier\n"
#define D20_5ERRTXT	"ERROR: DS-BIND failed (err = %d)\n"
#define D20_6ERRTXT	"ERROR: insufficient system memory\n"
#define D20_7ERRTXT	"ERROR: DS-SEARCH to DUA-cache failed (err = %d)\n"
#define D20_8ERRTXT	"ERROR: DS-READ to DUA-cache failed (err = %d ; obj = %s)\n"
#define D20_9ERRTXT	"ERROR: DS-REMOVE to DUA-cache failed (err = %d ; obj = %s)\n"
#define D20_10ERRTXT	"ERROR: DS-READ to DSA failed (err = %d ; obj = %s)\n"
#define D20_11ERRTXT	"ERROR: DS-ADD to DUA-cache failed (err = %d ; obj = %s)\n"
#define D20_12ERRTXT	"ERROR: No default-DSA entry exists in the DUA-cache\n"
#define D20_13ERRTXT	"ERROR: DS-UNBIND failed (err = %d)\n"

/* ------------------------ declaration of global data --------------------- */

char	d22_locmsg[80] ;
			/* variable used to store locality messages */

/* ------------------------ declaration of local data ---------------------- */

static D2_pbhead        d20_pbhead = {          /* standard header */
				D21V022 } ;

static D2_c_arg         d20_comarg = {          /* common arguments */
				(D2_extension *) NULL,
				{ D2_NOT_STARTED, 0 },
				0,
				(signed32) 0,
				D2_PRIO_LOW,
				D2_T_UNLIMITED,
				D2_S_UNLIMITED,
				D2_WORLD_SCOPE };

static D2_a_type	d20_a_type[D2_AT_MAX] ;

static D2_req_info      d20_req_info = {        /* requested information block */
				D2_R_VAL_TYP,
				0,
				d20_a_type } ;

static D21_p1           d20_bindpar = {         /* DS-BIND parameter block */
				&d20_pbhead,
				0,
				(D2_credent *) NULL,
				(D2_name_string) NULL,
				(D2_a_value *) NULL,
				(D2_name_string) NULL,
				0 } ;

static D21_p2           d20_unbindpar = {       /* DS-UNBIND parameter block */
				&d20_pbhead } ;

static D21_p3           d20_readpar = {         /* DS-READ parameter block */
				&d20_pbhead,
				&d20_comarg,
				0,
				(D2_name_string) NULL,
				&d20_req_info } ;

static D21_p5           d20_searchpar = {        /* DS-SEARCH parameter block */
				&d20_pbhead,
				&d20_comarg,
				0,
				(D2_name_string) NULL,
				D2_WHOLE_SUBTREE,
				FALSE,
				(D2_Fi_filter *) NULL,
				(D2_req_info *)&d20_req_info,
				(String) NULL,
				(D2_ei_info *) NULL,
				(D2_ref_inf *) NULL,
				(D2_name_string) NULL,
				0,
				TRUE,
				(D2_error *) NULL };

static D21_p7           d20_addpar = {          /* DS-ADD parameter block */
				&d20_pbhead,
				&d20_comarg,
				0,
				(D2_name_string) NULL,
				(D2_a_info *) NULL,
				(D2_error *) NULL } ;

static D21_p8           d20_rempar = {          /* DS_REMOVE parameter block */
				&d20_pbhead,
				&d20_comarg,
				0,
				(D2_name_string) NULL,
				(D2_error *) NULL } ;
				
/* prototype for main */
int main (int argc, char *argv[]);

/* ------------------------------------------------------------------------- */

int main (int argc, char *argv[]) {

	FILE		*fd ;
	signed32	d20_dirid ;
#ifdef notdef /* pdunne */
	signed16	d20_delcnt ;
#endif
	signed16	i, vcnt, d20_attrcnt, d20_valno ;
	char            install_dir[D27_LFILE_NAME];
	char            *instptr = install_dir ;
	char		lname[D27_LFILE_NAME] ;
	boolean		d20_update ;
	D2_name_string	d20_rdn ;
	D2_ei_info	*d20_dninfo ;
	D2_a_type	*d20_typeptr ;
	D2_ei_info	*cache_ei,	*dsa_ei;
	D2_a_info	*d20_cinfoptr, 	*d20_dinfoptr;
	D2_a_info	*d20_1atptr,   	*d20_2atptr,
			*d20_nmptr = (D2_a_info *) NULL ;
	D2_a_value	*d20_1valptr, 	*d20_2valptr, 	*d20_nmvalptr, 
			*d20_nm1valptr ;
	D2_c_res	com_res;

	setlocale(LC_ALL, "");

	/* get path name of directory system installation */

        sprintf(instptr, "%s%s", dcelocal_path, D27_CLIENT_DIR); 

	/* evaluate call parameter -> directory identifier */
	if (argc < D20_MINCALLPAR) {
		fprintf (stderr, D20_3ERRTXT) ;
		exit (1) ;
	}
	d20_dirid = atoi (argv[1]) ;
	if (d20_dirid < D2MINDIR_ID || d20_dirid > D2CACHE_MAXDIR_ID) {
		fprintf (stderr, D20_4ERRTXT, argv[1]) ;
		exit (1) ;
	}

	/* open log-file */
	sprintf (lname,"%s%s%s%d",instptr,D27_CACHE_DIR,D27_LOGFNM,getpid());
	if ((fd = fopen (lname, "w")) == NULL) {
		fprintf (stderr, D20_2ERRTXT, lname) ;
		exit (1) ;
	}

	/* do bind to default-DSA and implicitly to the DUA-cache */
	d20_comarg.d2_serv_cntrls = D2_NOREFERRAL | D2_USEDSA ;
	d20_bindpar.d21_1dir_id = (signed16) d20_dirid ;

	if (d21_1_bind (&d20_bindpar) == D2_ERROR) {
		fprintf (fd, D20_5ERRTXT, d20_pbhead.d2_errvalue) ;
		exit (1) ;
	}

	d20_searchpar.d21_5bind_id = 
	d20_addpar.d21_7bind_id = d20_rempar.d21_8bind_id = d20_dirid;
	d20_unbindpar.d21_2bind_id = d20_bindpar.d21_1bind_id ;

	/* get all default-DSA names from DUA-cache */
	d20_req_info.d2_r_no_at = 1 ;
	d20_a_type[0].d2_type = (Ob_id_string) D2_SEE_ALSO;
	d20_a_type[0].d2_typ_len = strlen (D2_SEE_ALSO);
	d20_comarg.d2_serv_cntrls = D2_NOREFERRAL | D2_DUACACHE ;
	d20_readpar.d21_3entry = (D2_name_string) D2_N_DEF_DSA ;
	d20_readpar.d21_3cres = &com_res;

#ifdef DEBUG
fprintf (stdout, "DS-READ to DUA-cache (%s)\n", D2_N_DEF_DSA) ;
#endif

	d20_readpar.d21_3bind_id = d20_dirid;
	if (d21_3_read (&d20_readpar) == D2_ERROR) {
		fprintf (fd, D20_8ERRTXT, d20_pbhead.d2_errvalue, 
		d20_readpar.d21_3entry) ;

		exit (1) ;
	}

	if (d20_readpar.d21_3res == (D2_ei_info *) NULL) {
		fprintf (fd, D20_12ERRTXT) ;
		exit (1) ;
	}

	d20_nmptr = d20_readpar.d21_3res->d2_ei_ai ;

#ifdef DEBUG
for (i = 0, d20_nmvalptr = d20_nmptr->d2_a_val; i < d20_nmptr->d2_a_no_val; i++)
	fprintf (stdout, "default-DSA from short form entry: %s\n", d20_nmvalptr++->d2_at_value.d2_oct_a_v) ;
#endif

	/* get all DUA-cache entries */
	d20_comarg.d2_serv_cntrls = D2_NOREFERRAL | D2_DUACACHE ;
	d20_searchpar.d21_5cres = &com_res;

#ifdef DEBUG
fprintf (stdout, "DS-SEARCH to DUA-cache\n") ;
#endif

	d20_req_info.d2_r_no_at = 0 ;
	if (d21_5_search (&d20_searchpar) == D2_ERROR) {
		fprintf (fd, D20_7ERRTXT, d20_pbhead.d2_errvalue) ;
		exit (1) ;
	}

	/* update informations about default-DSA's into the available attribute list */ 
	vcnt = d20_nmptr->d2_a_no_val ;
	d20_nmvalptr = d20_nmptr->d2_a_val ;
	for (i = 0; i < vcnt; d20_nmvalptr++, i++) {
		d20_dninfo = d20_searchpar.d21_5res ;
		do {
			if (strcmp (
			    (char *)(d20_nmvalptr->d2_at_value.d2_oct_a_v + 2),
			    (char *)(d20_dninfo->d2_ei_entry)) == 0)
				break ;
		} while ((d20_dninfo = d20_dninfo->d2_ei_next) != (D2_ei_info *) NULL) ;
		if (d20_dninfo == (D2_ei_info *) NULL) {
			d20_nmvalptr->d2_a_v_len = 0 ;
#ifdef DEBUG
fprintf (stdout, "removing '%s' from default-DSA attribute list\n", d20_nmvalptr->d2_at_value.d2_oct_a_v) ;
#endif
		}
	}

	/* update all DUA-cache entries by reading all attributes of
	   an entry from the DUA-cache and then from the default-DSA */
	
	d20_dninfo = d20_searchpar.d21_5res ;

	do {
		/* don't consider the short form entries of the default-
		   resp. local-DSA and client */
		if (strcmp ((char *)(d20_dninfo->d2_ei_entry), D2_N_DEF_DSA)
		    == 0 ||
		    strcmp ((char *)(d20_dninfo->d2_ei_entry), D2_N_LOC_DSA) 
		    == 0 ||
		    strcmp ((char *)(d20_dninfo->d2_ei_entry), D2_N_CLIENT) ==0)
			continue ;

		/* read all attributes of an object from the DUA-cache */
		d20_req_info.d2_r_no_at = D2_ALL_ATT_REQ ;
		d20_comarg.d2_serv_cntrls = D2_NOREFERRAL | D2_DUACACHE ;
		d20_readpar.d21_3entry = d20_dninfo->d2_ei_entry ;
		d20_readpar.d21_3cres = &com_res;
         	d20_readpar.d21_3bind_id = d20_dirid;
#ifdef DEBUG
fprintf (stdout, "DS-READ to DUA-cache (%s)\n", d20_dninfo->d2_ei_entry) ;
#endif

		if (d21_3_read (&d20_readpar) == D2_ERROR) {
			fprintf (fd, D20_8ERRTXT, d20_pbhead.d2_errvalue, 
					d20_dninfo->d2_ei_entry) ;
			exit (1) ;
		}

		/* examine the returned attribute types */
		cache_ei = d20_readpar.d21_3res;
		d20_cinfoptr = d20_1atptr = d20_readpar.d21_3res->d2_ei_ai ;
		d20_typeptr = d20_a_type ;
		d20_attrcnt = 0 ;

		do {
			*d20_typeptr++ = d20_cinfoptr->d2_a_type ;
			d20_attrcnt++ ;
		} while ((d20_cinfoptr = d20_cinfoptr->d2_a_next) != (D2_a_info *) NULL) ;

		/* read the corresponding attributes of the object from the DSA */
		d20_req_info.d2_r_no_at = d20_attrcnt ;
		d20_comarg.d2_serv_cntrls = D2_NOCACHE | D2_NOREFERRAL | 
					    D2_USEDSA | D2_DONT_STORE ;
		d20_readpar.d21_3cres = &com_res;
         	d20_readpar.d21_3bind_id = d20_bindpar.d21_1bind_id ;
#ifdef DEBUG
fprintf (stdout, "DS-READ to DSA (%s)\n", d20_dninfo->d2_ei_entry) ;
#endif

		if (d21_3_read (&d20_readpar) == D2_ERROR) {
			if (d20_pbhead.d2_errvalue == D2_NFOUND) {

				/* object doesn't exist within a DSA ->
				   remove it from the DUA-cache also */
				d20_comarg.d2_serv_cntrls = D2_NOREFERRAL | D2_DUACACHE ;
				d20_rempar.d21_8entry = d20_dninfo->d2_ei_entry ;

#ifdef DEBUG
fprintf (stdout, "DS-REMOVE to DUA-cache (%s)\n", d20_dninfo->d2_ei_entry) ;
#endif

				if (d21_8_remove_object (&d20_rempar) == D2_ERROR) {
					fprintf (fd, D20_9ERRTXT, d20_pbhead.d2_errvalue, 
						d20_dninfo->d2_ei_entry) ;
					exit (1) ;
				}

				/* if removed object is a default-DSA, delete */
				/* it also in the 'see also'-attribute of the */
				/* default-DSA short form entry */
				for (d20_nmvalptr = d20_nmptr->d2_a_val, i = 0;
					i < d20_nmptr->d2_a_no_val; d20_nmvalptr++, i++) {
					if (strcmp ((char *)(d20_nmvalptr->d2_at_value.d2_oct_a_v + 2),
					    (char *)(d20_dninfo->d2_ei_entry)) 
									== 0) {
#ifdef DEBUG
fprintf (stdout, "default-DSA '%s' into short entry list\n", d20_dninfo->d2_ei_entry) ;
#endif
						d20_nmvalptr->d2_a_v_len = 0 ;
						break ;
					}
				}
				continue ;	
			} else {
				fprintf (fd, D20_10ERRTXT, d20_pbhead.d2_errvalue, d20_dninfo->d2_ei_entry) ;
				exit (1) ;
			}
		}

		/* compare the two attribute information blocks */
		dsa_ei = d20_readpar.d21_3res;
		d20_cinfoptr = d20_1atptr ;
		d20_dinfoptr = d20_readpar.d21_3res->d2_ei_ai ;
		d20_update = FALSE ;

		do {
			d20_2atptr = d20_dinfoptr ;

			do {

				if (strncmp (
					(char *)(d20_1atptr->d2_a_type.d2_type),
					(char *)(d20_2atptr->d2_a_type.d2_type),
					d20_1atptr->d2_a_type.d2_typ_len) 
					== 0) {

					if (d20_1atptr->d2_a_no_val != d20_2atptr->d2_a_no_val) {
						d20_update = TRUE ;
						break ;
					}
					d20_1valptr = d20_1atptr->d2_a_val ;
					d20_2valptr = d20_2atptr->d2_a_val ;
					for (d20_valno = 0; d20_valno < d20_1atptr->d2_a_no_val;
					     d20_valno++, d20_1valptr++, d20_2valptr++) {
						if (d20_1valptr->d2_a_v_len != d20_2valptr->d2_a_v_len ||
						    d20_1valptr->d2_a_rep != d20_2valptr->d2_a_rep ||
						    !d22_001_attr_comp (d20_1valptr, d20_2valptr)) {
							d20_update = TRUE ;
							break ;
						}
					}
				}
			} while (!d20_update && 
				(d20_2atptr = d20_2atptr->d2_a_next) != (D2_a_info *) NULL);

			if (d20_update) {
				/* update DUA-cache entry */
				d20_comarg.d2_serv_cntrls = D2_NOREFERRAL | D2_DUACACHE | 
											D2_NORMAL_CLASS;

				d20_addpar.d21_7entry_name = d20_dninfo->d2_ei_entry;
				d20_addpar.d21_7_at_info = d20_dinfoptr;
				d20_rdn = d20_dninfo->d2_ei_entry + strlen (
					(char *)(d20_dninfo->d2_ei_entry));

#ifdef notdef /* pdunne */
				for (d20_delcnt = 0; 
					d20_rdn >= d20_dninfo->d2_ei_entry && d20_delcnt < 2;
					d20_rdn -- )

					if (*d20_rdn == D2_DL1)
						d20_delcnt++ ;

				d20_rdn++ ;
				if (d20_delcnt == 2)
					*d20_rdn++ = '\0' ;
#endif

#ifdef DEBUG
fprintf (stdout, "DS-ADD to DUA-cache (%s)\n", d20_dninfo->d2_ei_entry) ;
#endif
				if (d21_7_add_object (&d20_addpar) == D2_ERROR) {
					fprintf (fd, D20_11ERRTXT, d20_pbhead.d2_errvalue, d20_dninfo->d2_ei_entry) ;
					exit (1) ;
				}
				break ;
			}

		} while ((d20_1atptr = d20_1atptr->d2_a_next) != (D2_a_info *) NULL);

		/* free the read result space	*/
		free ((char *) dsa_ei);
		free ((char *) cache_ei);

	} while ((d20_dninfo = d20_dninfo->d2_ei_next) != (D2_ei_info *) NULL);

	/* 	update informations about default-DSA's into the 
		available attribute list 			*/ 
	vcnt = d20_nmptr->d2_a_no_val ;
	d20_nmvalptr = d20_nm1valptr = d20_nmptr->d2_a_val ;
		for (i = 0; i < vcnt; d20_nmvalptr++, i++) {
			if (d20_nmvalptr->d2_a_v_len > 0) {
				if (d20_nmvalptr != d20_nm1valptr) {
					d20_nm1valptr->d2_a_v_len = d20_nmvalptr->d2_a_v_len ;
					d20_nm1valptr->d2_at_value.d2_oct_a_v = d20_nmvalptr->d2_at_value.d2_oct_a_v ;
				}
				d20_nm1valptr++ ;
			} else
				d20_nmptr->d2_a_no_val-- ;
		}

		/* update the default-DSA short form entry in the DUA-cache if required */
		if (d20_nmptr->d2_a_no_val != vcnt) {
			if (d20_nmptr->d2_a_no_val == 0) {
				fprintf (fd, D20_12ERRTXT) ;
				exit (1) ;
			}

			d20_comarg.d2_serv_cntrls = D2_NOREFERRAL | D2_DUACACHE | 
							D2_NORMAL_CLASS ;
			d20_addpar.d21_7entry_name = (D2_name_string) D2_N_DEF_DSA;
			d20_addpar.d21_7_at_info = d20_nmptr;

	#ifdef DEBUG
	fprintf (stdout, "DS-ADD to DUA-cache (%s)\n", D2_N_DEF_DSA) ;
	#endif
			if (d21_7_add_object (&d20_addpar) == D2_ERROR) {
				fprintf(fd, D20_11ERRTXT, d20_pbhead.d2_errvalue, D2_N_DEF_DSA);
				exit (1) ;
			}
		}

	/* do unbind to default-DSA and implicitly to the DUA-cache */
	if (d21_2_unbind (&d20_unbindpar) == D2_ERROR)
		fprintf (fd, D20_13ERRTXT, d20_pbhead.d2_errvalue) ;
	fclose (fd) ;
}

