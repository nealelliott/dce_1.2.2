/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20schem.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:06  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:17:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:11  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:05:38  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:10:57  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:58:57  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:32:10  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:23:33  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:33:32  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:09:21  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:05:14  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:18:36  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:13:49  marrek]
 * 
 * Revision 1.1.2.4  1992/06/30  20:54:30  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:33:25  melman]
 * 
 * Revision 1.1.2.3  1992/06/18  19:58:47  melman
 * 	Incorporated diffs from Norbert to help in configuring GDS for testing.
 * 	[1992/06/18  19:52:49  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  22:01:40  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:17:08  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20schem.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:25 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODULE                                              */
/*		  =========					      */
/* NAME         : d2schem.c                [scheme handling]          */
/*		  =========					      */
/* AUTHOR       : Sanjay Jain D AP 113                                */
/* DATE         : 05.02.91                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : DS-Design-Spec.                                     */
/*                LB  3255.43.04                                      */
/*			.					      */
/*								      */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*								      */
/* DESCRIPTION  : scheme handling:  memory reservation, scheme        */
/*                reading. see function desc. too.                    */
/*								      */
/* OPERATING-SYSTEM:                                                  */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. | Date    | Updates                        | KZ | CR# FM# */
/*   1.0    | 10.02.88| Original                       | Ri |         */
/*          |         |                                |    |         */
/*								      */
/*datoff **************************************************************/

/*####################################################################*/
/* AUSPRAEGUNG	dieser Datei:					      */
/*		  SINIX	  V2.0	MX2/MX4	V00  C-Comp. V00	      */
/*		  BS2000  V9					      */
/*								      */
/*####################################################################*/

#include <gds.h>

/**************	system-includes				 ****************/
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
 
/************** DS-external includes                     ****************/
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d21iapl.h>
#include <d2shm.h>
#include <d20proto.h>

/************** global variables  (import)                    ****************/
extern  signed16        d20_bid; /* current bind-id                     */

/* for new schema */
extern  Srt          *d2_p_srt ; /* pointer to Srt                      */
extern  Oct          *d2_p_oct ; /* pointer to Oct                      */
extern  At           *d2_p_at  ; /* pointer to At                       */

extern  signed32      d2_nsrt ;  /* number of Srt elements              */
extern  signed32      d2_nat  ;  /* number of At elements               */
extern  signed32      d2_noct ;  /* number of Oct elements              */

extern  D20_srt      *d2_p_d20srt ; /* pointer to Srt for object admin.    */
extern  D20_oct      *d2_p_d20oct ; /* pointer to Oct for object admin.    */
extern  D20_at       *d2_p_d20at  ; /* pointer to At for object admin.     */

extern  signed32     d2_nd20srt ;  /* number of Srt elements in admin. sch*/
extern  signed32     d2_nd20oct ;  /* number of Oct elements in admin. sch*/
extern  signed32     d2_nd20at  ;  /* number of At elements in admin. sch */

extern  char	*d20_errtab[] ;	     /* error message - table */
extern  D20_omask d20_msgomask[];
extern  D20_omask d20_qomask[] ;
extern  char    *d20_errmess[];
extern  signed32  d20_old_rule_nr;  /* rule number of structure that has   */
				    /* been selected before               */

/************** global variables  (export)                    ****************/
 
  char    new_utc_time[D20_SUTCTIME] ;
 
/************** external modules                   ***************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME  :      d20_204_read_scheme     [<short-name>]                */
/*								      */
/* AUTHOR:      U. Richter  D AP 113                                  */
/* DATE:        5.02.88                                               */
/*                                                                    */
/* DESCRIPTION: This module first reads the time stamp of DSA schema  */
/*		and compares this time stamp with the time stamp of   */
/*		object administration schema. If DSA schema is newer  */
/*		than object administration schema, then DSA schema is */
/*		read from DSA and a transformation   		      */
/*		is done on DSA schema to get object administration    */
/*		schema. This is kept in C structures and it is stored */
/*		in a local file. New time stamp is also stored in a   */
/*		file.						      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		char	*env;		 environment variable         */ 
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		D2_ERROR;       In case no error has occurred.	      */
/*		D2_NOERROR;	In case some error has occurred.      */
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
/*exoff ***************************************************************/

 d2_ret_val d20_204_read_scheme(
   char    *env)

{                     	/*  d20_204_read_scheme()               */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/
 D2_pbhead      head;

 D2_a_type      att_typ_arr[1]; /* requested attr. types to read      */

D21_p3		pb; 	/* parameter block for read operation */
D2_c_arg	c_arg;
D2_req_info	req_info;
D2_c_res	c_res;
D2_a_info	*at_info;
Errcode		ret_code;
FILE		*fdcom;
char            old_utc_time[D20_SUTCTIME] ;
char            shfname[D20_SHFNMAX] ;
D20_srt		*tmp_p_d20srt;
D20_oct		*tmp_p_d20oct;
D20_at		*tmp_p_d20at;
signed32	tmp_nd20srt;
signed32	tmp_nd20oct;
signed32	tmp_nd20at;

	/* actual structure of user- resp. dsa name */
signed32	namestr, dsastr ;	

FILE		*fdsch;
int		fdsch_int;
struct	flock	flock_struct;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* set parameters for reading TIMESTAMP of schema object */
head.d2_version = D21V022;
pb.d21_3pbhead = &head;

c_arg.d2_extension 		 = (D2_extension *)NULL;
c_arg.d2_op_progress.d2_nr_phase = D2_NOT_STARTED;
c_arg.d2_op_progress.d2_next_rdn = 0;
c_arg.d2_al_rdn 		 = 0;
c_arg.d2_serv_cntrls 		 = D2_NOREFERRAL | D2_USEDSA | D2_DREFALIAS | 
				   D2_DONT_STORE | D2_PREF_ADM_FUN;
c_arg.d2_priority 		 = D2_PRIO_LOW;
c_arg.d2_time_limit 		 = D2_T_UNLIMITED;
c_arg.d2_size_limit 		 = D2_S_UNLIMITED;
c_arg.d2_scope_ref 		 = D2_WORLD_SCOPE; 

pb.d21_3c_a = &c_arg;

pb.d21_3bind_id = d20_bid; 
pb.d21_3entry = (D2_name_string)D2_NAME_SCHEME;

att_typ_arr[0].d2_type 		=  (Ob_id_string)D2_TIME_STAMP;
att_typ_arr[0].d2_typ_len 	=  sizeof(D2_TIME_STAMP) - 1;

req_info.d2_r_val_at 		= D2_R_VAL_TYP;
req_info.d2_r_no_at 		= 1;
req_info.d2_r_type_at 		= att_typ_arr;

pb.d21_3r_info = &req_info;

pb.d21_3err = (D2_error *)NULL;
pb.d21_3cres = &c_res;

/* error in reading schema object */
if (d21_3_read(&pb) == D2_ERROR)
	{
    	if (pb.d21_3res != (D2_ei_info *) NULL)
    		free((char *) pb.d21_3res);

	d20_msgomask[D20_M1POS].d20_ioinfo = d20_errtab[D20_E59IDX] ;
	d20_handlemask (d20_msgomask) ;
	return( D2_NOERROR );
	}

at_info = pb.d21_3res->d2_ei_ai;

/* if attribute type is TIMESTAMP	*/
if (d20_cmp_objid(&(at_info->d2_a_type), sizeof(D2_TIME_STAMP),
			(Ob_id_string) D2_TIME_STAMP))
	{
	if (at_info->d2_a_no_val != 1)
	    {
    	    free((char *) pb.d21_3res);
	    d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E67IDX] ;
	    d20_handlemask (d20_qomask) ;
	    return( D2_ERROR );
	    }

	strncpy(new_utc_time,
		    (char *)at_info->d2_a_val->d2_at_value.d2_oct_a_v,
		    at_info->d2_a_val->d2_a_v_len) ;
	new_utc_time[at_info->d2_a_val->d2_a_v_len] = D2_EOS;
	}
else
	{
    	free((char *) pb.d21_3res);
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errmess[D20_E46IDX] ;
	d20_handlemask (d20_qomask) ;
	return( D2_ERROR );
	}                        

free((char *) pb.d21_3res);

/* open common scheme file and read scheme time stamp */
sprintf (shfname, "%s%s", env, D20_CMFILE) ;

if ((fdcom = fopen (shfname, "r")) == (FILE *) NULL) 
	{
        d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E71IDX] ;
	d20_handlemask (d20_qomask) ;
	return( D2_ERROR );
    	}

if (fscanf (fdcom, "%s %d %d", old_utc_time, &namestr, &dsastr) != 3)
	{
	fclose (fdcom);
	d20_disp_error(d20_errtab[D20_E56IDX]);
	}

fclose (fdcom);

/* compare scheme time stamp available within the common scheme file 	*/
/* with the time stamp of the schema object in a DSA 			*/

/* administration schema has been generated form the current DSA schema */
if (strcmp (old_utc_time, new_utc_time) == 0)
	return (D2_NOERROR) ;

/* read schema from DSA		*/
if (d21_21_rdsh(d20_bid,&ret_code,DS_V03, &d2_nsrt, &d2_p_srt, 
				          &d2_noct, &d2_p_oct,
				          &d2_nat, &d2_p_at) == D2_ERROR)
	{/* error reading scheme                */
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E59IDX] ;
	d20_handlemask (d20_qomask) ;
	return( D2_ERROR );
	}

/* save parameters of current object administration schema */
tmp_nd20srt = d2_nd20srt;
tmp_p_d20srt = d2_p_d20srt;
tmp_nd20oct = d2_nd20oct;
tmp_p_d20oct = d2_p_d20oct;
tmp_nd20at = d2_nd20at;
tmp_p_d20at  = d2_p_d20at;


/* transform DSA schema to new object administration schema 	*/
if (d20_sch_trans(d2_nat, d2_p_at, d2_noct, d2_p_oct, d2_nsrt, d2_p_srt, 
		  &d2_nd20at, &d2_p_d20at, &d2_nd20oct, &d2_p_d20oct, 
		  &d2_nd20srt, &d2_p_d20srt) == D2_ERROR)		  
	{
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errmess[D20_E41IDX];
	d20_handlemask (d20_qomask) ;

	/* reset parameters of DSA schema and free the space occupied by */
	/* DSA schema.							 */
	free((char *)d2_p_srt);
	free((char *)d2_p_oct);
	free((char *)d2_p_at);
	d2_nsrt = 0;
	d2_p_srt = (Srt *)NULL;
	d2_noct = 0;
	d2_p_oct = (Oct *)NULL;
	d2_nat = 0;
	d2_p_at = (At *)NULL;

	/* reset parameters of current object administration schema to */
	/* old parameters.					       */
	d2_nd20srt = tmp_nd20srt; 
	d2_p_d20srt = tmp_p_d20srt; 
	d2_nd20oct = tmp_nd20oct; 
	d2_p_d20oct = tmp_p_d20oct;
	d2_nd20at = tmp_nd20at;
	d2_p_d20at = tmp_p_d20at;

	return( D2_ERROR );
	}

/* get admin. scheme file name */
sprintf (shfname, "%s%s", env, D20_ADMSCH_FILE) ;

/* open admin. scheme file */
if ((fdsch = (FILE *)fopen(shfname,"w")) == (FILE *)0)
	{
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errmess[D20_E43IDX];
	d20_handlemask (d20_qomask) ;

	/* reset parameters of DSA schema 	*/
	free((char *)d2_p_srt);
	free((char *)d2_p_oct);
	free((char *)d2_p_at);
	d2_nsrt = 0;
	d2_p_srt = (Srt *)NULL;
	d2_noct = 0;
	d2_p_oct = (Oct *)NULL;
	d2_nat = 0;
	d2_p_at = (At *)NULL;

	/* reset parameters of current object administration schema to */
	/* old parameteres.					       */
	d2_nd20srt = tmp_nd20srt; 
	d2_p_d20srt = tmp_p_d20srt; 
	d2_nd20oct = tmp_nd20oct; 
	d2_p_d20oct = tmp_p_d20oct;
	d2_nd20at = tmp_nd20at;
	d2_p_d20at = tmp_p_d20at;

	return( D2_ERROR );
	}

fdsch_int = fileno(fdsch);

flock_struct.l_type = F_WRLCK;
flock_struct.l_whence = 0;
flock_struct.l_start = 0;
flock_struct.l_len = 0;

fcntl(fdsch_int, F_SETLKW, &flock_struct);

/* store object administration schema in a local file 	*/
if (d20_store_admsch(fdsch, d2_nd20at, d2_p_d20at, d2_nd20oct, d2_p_d20oct,
			  d2_nd20srt, d2_p_d20srt) == D2_ERROR)
	{
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errmess[D20_E43IDX];
	d20_handlemask (d20_qomask) ;

	/* reset parameters of DSA schema 	*/
	free((char *)d2_p_srt);
	free((char *)d2_p_oct);
	free((char *)d2_p_at);
	d2_nsrt = 0;
	d2_p_srt = (Srt *)NULL;
	d2_noct = 0;
	d2_p_oct = (Oct *)NULL;
	d2_nat = 0;
	d2_p_at = (At *)NULL;

	/* reset parameters of current object administration schema to */
	/* old parameteres.					       */
	d2_nd20srt = tmp_nd20srt; 
	d2_p_d20srt = tmp_p_d20srt; 
	d2_nd20oct = tmp_nd20oct; 
	d2_p_d20oct = tmp_p_d20oct;
	d2_nd20at = tmp_nd20at;
	d2_p_d20at = tmp_p_d20at;

	fclose(fdsch);	/* releases file lock also.	*/

	return( D2_ERROR );
	}

/* open common scheme file to store new time stamp */
sprintf (shfname, "%s%s", env, D20_CMFILE) ;
if ((fdcom = fopen (shfname, "w")) == (FILE *)NULL) 
	{
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E71IDX] ;
	d20_handlemask (d20_qomask) ;

	/* reset parameters of DSA schema and free the space occupied by */
	/* DSA schema.							 */
	free((char *)d2_p_srt);
	free((char *)d2_p_oct);
	free((char *)d2_p_at);
	d2_nsrt = 0;
	d2_p_srt = (Srt *)NULL;
	d2_noct = 0;
	d2_p_oct = (Oct *)NULL;
	d2_nat = 0;
	d2_p_at = (At *)NULL;

	/* reset parameters of current object administration schema to */
	/* old parameteres.					       */
	d2_nd20srt = tmp_nd20srt; 
	d2_p_d20srt = tmp_p_d20srt; 
	d2_nd20oct = tmp_nd20oct; 
	d2_p_d20oct = tmp_p_d20oct;
	d2_nd20at = tmp_nd20at;
	d2_p_d20at = tmp_p_d20at;

	fclose(fdsch);	/* releases file lock also.	*/

	return( D2_ERROR );
    	}

/* check if old namestr and dsastr are still valid.	*/
if (namestr <= d2_nd20srt && dsastr <= d2_nd20srt)
	if (ds0_check_chgstruct((Ob_id_string) D2_PWD, sizeof(D2_PWD), 
		(d2_p_d20srt + namestr - 1)) &&
		ds0_check_chgstruct((Ob_id_string) D2_PSAP_ADDRESS,
				sizeof(D2_PSAP_ADDRESS), 
		(d2_p_d20srt + dsastr - 1)))
		/*write new UTC-time & old namestr & dsastr into common file */
		fprintf (fdcom, "%s %d %d\n", new_utc_time, namestr, dsastr);

	else
		/* write new UTC-time into common scheme file */
		fprintf (fdcom, "%s\n", new_utc_time);
else
	/* write new UTC-time into common scheme file */
	fprintf (fdcom, "%s\n", new_utc_time);

fclose (fdcom) ;
fclose (fdsch);	/* releases file lock also.	*/

/* whole operation is successful release the space occupied by old object */
/* administration schema.						  */
free((char *)tmp_p_d20srt);
free((char *)tmp_p_d20oct);
free((char *)tmp_p_d20at);

d20_old_rule_nr = 0;
return( D2_NOERROR );

}                                /* end of d20_204_read_scheme()	*/
