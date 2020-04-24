/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22evapar.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:48  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:27  root]
 * 
 * Revision 1.1.8.5  1994/09/06  12:24:56  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:37  keutel]
 * 
 * Revision 1.1.8.4  1994/06/21  14:44:46  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  12:59:59  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:51:16  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:17:13  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:50  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:18:19  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:21  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:53:34  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:38:27  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:14:09  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:41:02  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:38  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:29:47  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:37:19  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22evapar.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:48 $";
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
/* NAME         : d22evapar.c                                         */
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
/*  FUNCTION:  d22_eval_param ()                                             */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function evaluates all parameter values by which a DUA-          */
/*      cachehandler process can be started (see design specification)       */
/*      and stores the evaluated values (-> DUA-cache default values)        */
/*      in a predefined table. There is no order at which the parameter      */
/*      must appear. The evaluated default values are verified for           */
/*      inconsistency and an error is returned if one is recognized.         */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      parc        = Number of parameters which must be evaluated.          */
/*      parv        = Pointer to the beginning of an array containing        */
/*                    'parc' pointers where each points to a parameter.      */
/*      admptr      = Pointer to the actual DUA-cache administration table.  */ 
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D22_OK      = The function has been performed successfully.          */
/*      > 0         = An internal error is occurred during operation (the    */
/*                    return value specifies the error).                     */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 16.11.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>


/* ----------------- declaration of global data ---------------------------- */

extern char	*optarg ;	/* reference to to actual option argument */
extern int	optind ;	/* index of the next option argument to be */
				/* processed */
extern int	opterr ;	/* 'print option error message'-flag */

extern boolean	d22_foreground ;/* test mode flag */

/* ------------------------------------------------------------------------- */

signed32 d22_eval_param (int parc, char *parv[], D22_admin_tab *admptr) {

    int			opt ;
    signed16		*p ;
    D22_config_tab	*conf = admptr->d22_conftab ;

    /* evaluate calling parameter */
    opterr = 0 ;
    while ((opt = getopt (parc, parv, "a:b:fh:i:m:n:p:P:r:v:w:")) != -1) {
	switch (opt) {
	case 'h':
	    /* evaluate max. no. of hash table entries */
	    conf->d22_hsh_max = atoi (optarg) ;
	    break ;
	case 'r':
	    switch ((int) *optarg) {
	    case 'm':
		/* evaluate max. no. of cacheable */
		/* RESIDENT directory objects */
		conf->d22_res_max = atoi (optarg + 1) ;
		break ;
	    case 'd':
		/* evaluate max. no. of removable */
		/* RESIDENT directory objects */
		conf->d22_rdel_no = atoi (optarg + 1) ;
		break ;
	    default:
		return (D27_INV_RES) ;
	    }
	    break ;
	case 'p':
	    switch ((int) *optarg) {
	    case 'm':
		/* evaluate max. no. of cacheable */
		/* PRIVILEGED directory objects */
		conf->d22_priv_max = atoi (optarg + 1) ;
		break ;
	    case 'd':
		/* evaluate max. no. of removable */
		/* PRIVILEGED directory objects */
		conf->d22_pdel_no = atoi (optarg + 1) ;
		break ;
	    default:
		return (D27_INV_PRIV) ;
	    }
	    break ;
	case 'n':
	    switch ((int) *optarg) {
	    case 'm':
		/* evaluate max. no. of cacheable */
		/* NORMAL directory objects */
		conf->d22_norm_max = atoi (optarg + 1) ;
		break ;
	    case 'd':
		/* evaluate max. no. of removable */
		/* NORMAL directory objects */
		conf->d22_ndel_no = atoi (optarg + 1) ;
		break ;
	    default:
		return (D27_INV_NORM) ;
	    }
	    break ;
	case 'i':
	    /* evaluate max. no. of internally hold */
	    /* directory objects */
	    conf->d22_int_max = atoi (optarg) ;
	    break ;
	case 'm':
	    /* evaluate max. size of all free memory areas */
	    conf->d22_mem_max = atoi (optarg) ;
	    break ;
	case 'a':
	    /* evaluate max. no. of free memory areas */
	    conf->d22_mem_no = atoi (optarg) ;
	    break ;
	case 'b':
	    /* evaluate path name for cache directory */
	    if (strlen (optarg) >= (size_t) D27_LINST_NAME) {
		return (D27_INV_CPATH) ;
	    }
	    sprintf (conf->d22_base_path, "%s%s", optarg, D27_CACHE_DIR) ;
	    break ;
	case 'f':
	    /* set process test mode flag */
	    d22_foreground = TRUE ;
	    break ;
	case 'P':
	case 'v':
	case 'w':
	    /* serviceability flags are ignored at this point */
	    break ;
	default:
	    return (D27_INV_SWITCH) ;
	}
    }		
    /* evaluate path name for cache directory from environment */
    sprintf (conf->d22_base_path, "%s%s%s", dcelocal_path, D27_CLIENT_DIR,
								D27_CACHE_DIR) ;

    /* check default-values for inconsistency */
    for (p = (signed16 *) conf ; p < (signed16 *) (conf->d22_base_path); p++)
	if (*p < 0) {
	    return (D27_INV_VALUE) ;
	}

    if ((conf->d22_res_max + conf->d22_priv_max + conf->d22_norm_max) >
							    conf->d22_hsh_max ||
	conf->d22_int_max >
		(conf->d22_res_max + conf->d22_priv_max + conf->d22_norm_max) ||
	conf->d22_int_max > D22_IOBJ_MAX ||
	conf->d22_rdel_no > conf->d22_res_max ||
	conf->d22_pdel_no > conf->d22_priv_max ||
				       conf->d22_ndel_no > conf->d22_norm_max) {
	return (D27_INV_VALUE) ;
    }
    return (D22_OK) ;
}
