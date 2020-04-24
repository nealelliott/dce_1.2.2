/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20getdsa.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:31  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:10:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:36  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:43:11  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:22:15  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:02:47  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:01:38  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:03  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:12:49  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:19:15  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:03:56  marrek]
 * 
 * Revision 1.1.4.4  1993/01/29  08:30:30  marrek
 * 	New syntax for d20_180_get_dsa().
 * 	[1993/01/28  10:12:23  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:44:18  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:58:01  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:34:53  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:27:08  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:44:45  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:36:38  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20getdsa.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:35 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20getdsa.c                                         */
/*								      */
/* AUTHOR       : Volpers,D AP 11                                     */
/* DATE         : 22.04.88                                            */
/*								      */
/* KOMPONENTE	: DS						      */
/*								      */
/* DOK.-NR.	: DS-Design-Spez.				      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.	: <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*								      */
/* HISTORY 	:						      */
/*								      */
/* Vers.Nr. |  Date   |	 Updates    		       | KZ | CR# FM# */
/*      0.1 | 15.04.88| Erstellung                     | hv |         */
/*								      */
/*datoff **************************************************************/

/**********************************************************************/
/*								      */
/*		       Modulidentification			      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*			  I N C	L U D E	S			      */
/*								      */
/**********************************************************************/

#include <gds.h>
#include <stdio.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d2shm.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern  char      *d20_2ifields[] ;  /* Input fields for mask 2       */
extern  D20_omask d20_2omask[] ;     /* Mask 2 */

extern D20_omask d20_qomask[];         /* Input fields for error mask */
extern char      *d20_errtab[];        /* Error mask                  */

extern char      d20_actopmsg[];
extern char      *d20_op[];

extern signed32  rbatch;
extern char     *d20_2_1opt;
extern char     *d20_2_2opt;

extern  D20_srt    *d2_p_d20srt;
				    /* pointer to SRT                      */
extern  signed32  d2_nd20srt ;      /* number of SRT elements              */
extern  signed32  fl3size;
extern  signed32  fl4size;

  signed32   d20_dsastr;            /* actual structure of dsa name       */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_180_get_dsa                                       */
/*								      */
/* AUTHOR:      Volpers   (D AP 11)                                   */
/* DATE:        22.04.88                                              */
/*								      */
/* SYNTAX:     signed16 d20_180_get_dsa(dsa_disname, p_env)	      */
/*                                                                    */
/*              char   *dsa_disname;                                  */
/*              char   *p_env;                                        */
/*								      */
/* DESCRIPTION: This function handles mask 2                          */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       no error                                      */
/*      -1      :       break is selected                             */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 signed16 d20_180_get_dsa(
   char  *dsa_disname,
   char  *p_env)

{	/*  d20_180_get_dsa                   */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

   signed16 rcnt; 
   int npar ;
   D20_omask   *varfld ;
   char        tree[50] ; /* field for storing unused return information */
   char   **varinput ;
   signed32  namestr;          /* actual structure of user- resp. dsa name */
   char   shfname[D20_SHFNMAX] ;  /* array for holding the path name of */
				       /* scheme files */
   char   shtstamp[D20_SUTCTIME] ;/* array for holding the time stamp of */
					/* the scheme */
   signed32  dsatmpstr ;             /* temporary structure dsa name */
   D20_srt    *srtptr ;
   FILE   *fdcm ;                 /* file descriptor of common scheme file */

static  Bool         first2loop = TRUE ;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/
/* get structure types of the 'logon user name' and the 'DSA name' */
/* from common scheme file */
sprintf (shfname, "%s%s", p_env, D20_CMFILE) ;
if ((fdcm = fopen (shfname, "r")) == (FILE *) NULL) 
    d20_disp_error(d20_errtab[D20_E56IDX]);

if ((npar = fscanf (fdcm, "%s%d%d", shtstamp, &namestr, &d20_dsastr)) < 1) 
	{
	fclose (fdcm) ;
	d20_disp_error(d20_errtab[D20_E56IDX]);
	}

fclose (fdcm) ;

if (npar < 3) 
	{
	/* no 'logon user name' structure selected -> search one from */
	/* scheme */
	for (srtptr = d2_p_d20srt, rcnt = 0; rcnt < d2_nd20srt; rcnt++,srtptr++)
		if (ds0_check_chgstruct((Octet_string)D2_PWD, sizeof(D2_PWD), srtptr))
		   break ;

	if (rcnt == d2_nd20srt)
		/* error -> no valid name structure found */
		d20_disp_error(d20_errtab[D20_E56IDX]);

	namestr = srtptr->rule_nr;

	/* no 'DSA name' structure selected -> search one from */
	/* scheme */
	for (srtptr = d2_p_d20srt, rcnt = 0; rcnt < d2_nd20srt; rcnt++,srtptr++)
		if (ds0_check_chgstruct((Octet_string)D2_PSAP_ADDRESS, 
					sizeof(D2_PSAP_ADDRESS), srtptr))
		  break ;

	if (rcnt == d2_nd20srt) 
		/* error -> no valid name structure found */
		d20_disp_error(d20_errtab[D20_E56IDX]);

	d20_dsastr = srtptr->rule_nr;
	 }

/* enter a specific DSA -> handle remote logon mask */
/* initialize variables used for mask 2 handling */

for (;;)
	{
	do
		{
		if (first2loop || *(signed32 *) d20_2ifields[DS_RE_OPT] == 
								DS_R_NAMESTROPT)
			{
			/* generate 'DSA name' structure within */
			/* variable partition of remote logon mask */
			varfld = &d20_2omask[D20_21POS] ;
			d20_gen_name_structure (d20_dsastr, &varfld, fl3size, 
								fl4size) ;

			first2loop = FALSE ;
			/* mark unused components of variable partition */
			while(varfld < &d20_2omask[D20_21POS + (D20_21CNT * 2)])
				varfld++->d20_fkt_id = D20_NO_OP ;
			}

		*d20_2ifields[DS_RE_BREAK] = DS_DEFAULT_VALUE ;

		d20_2omask[D20_22POS].d20_oformat = (rbatch) ? d20_2_2opt : 
								d20_2_1opt;
		d20_handlemask(d20_2omask);

		if (*d20_2ifields[DS_RE_BREAK] != DS_DEFAULT_VALUE ||
				*(signed32 *) d20_2ifields[DS_RE_OPT] == DS_R_END)
			return(D2_ERROR) ;

		if (*(signed32 *) d20_2ifields[DS_RE_OPT] == DS_R_NAMESTROPT)
			{
			/* new selection of 'DSA name' structure requested */
			for (;;)
				{
				/* get a new 'DSA name' structure */
				if (ds0_500_objects (TRUE, D20_OBJECT, 
						&dsatmpstr, tree) == D2_ERROR)
					{
					*d20_2ifields[DS_RE_BREAK] =
								D20_MARKFKT;
					return(D2_ERROR) ;
					}

				srtptr = d20_search_adm_srule( d2_nd20srt, 
							d2_p_d20srt, dsatmpstr);

				if (!ds0_check_chgstruct((Octet_string)D2_PSAP_ADDRESS,
					sizeof(D2_PSAP_ADDRESS), srtptr))
					{
					d20_qomask[D20_Q1POS].d20_ioinfo = 
							d20_errtab[D20_E58IDX];
					d20_handlemask (d20_qomask) ;
					} 
				else
					break;
				}

			if (*d20_2ifields[DS_RE_BREAK] != DS_DEFAULT_VALUE)
				return(D2_ERROR) ;

			d20_dsastr = dsatmpstr;
			/* store new structure info. into common scheme file */
			fdcm = fopen (shfname, "w") ;
			fprintf (fdcm, "%s %d %d\n", shtstamp, namestr, 
								d20_dsastr) ;
			fclose (fdcm) ;
			}

		} while (*(signed32 *) d20_2ifields[DS_RE_OPT] == DS_R_NAMESTROPT) ;

	if (*d20_2ifields[DS_RE_BREAK] != DS_DEFAULT_VALUE)
		/* go return to logon mask */
		return(D2_ERROR) ;

	/* generate the distinguished name of the DSA */
	sprintf (dsa_disname, "");
	varinput = &d20_2ifields[DS_RE_NAME] ;
	if (d20_gen_distname (d20_dsastr, &varinput, dsa_disname) == D2_ERROR)
		{
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E14IDX];
		d20_handlemask (d20_qomask) ;
		continue;
		} 

	break;	/* break from for loop.	*/
	}

return(D2_NOERROR);

} 	/* end ds20_180_get_dsa           */
