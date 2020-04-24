/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20admsch.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:05  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:12:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:17  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:15  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:03:16  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:47  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:17:14  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:57:39  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:12:51  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:46:32  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:59:55  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:50:15  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:58:37  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:38:04  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:09:27  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20admsch.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:08 $";
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
/* NAME         : d20admsch.o [scheme administration interface]       */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 23.03.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
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
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*      0.1 | 23.03.88| Original                       | ws |         */
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
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d2shm.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                 D E C L A R A T I O N S        DATA                */
/*								      */
/**********************************************************************/

extern D20_omask  d20_msgomask[];       /*input fields of message mask*/
extern D20_omask  d20_qomask[];         /* input fields of error mask */

extern D20_omask  d20_9omask[];         /* scheme operation mask      */
extern char      *d20_9ifields[];       /* input fields of scheme     */
					/* operation mask             */

extern char      *d20_errtab[];         /* error table                */
extern char      *d20_errmess[];        /* error table                */
extern char      *d20_err_table[];      /* error table                */
extern char       d20_actopmsg[];       /* header info in ODT/ADT-mask*/
extern D20_opmsg  d20_s_opmsg[];        /* table of header infos      */
extern char      *d20_op[];             /* array of selectable        */
					/* operations                 */
#ifdef MORE
extern char       new_utc_time[];       /* scheme time stamp          */
#endif
extern signed16   d20_bid;              /* bind ID                    */

extern  	Srt	*d2_p_srt ;   /* pointer to SRT	      */	
extern  	Oct     *d2_p_oct ;   /* pointer to OCT	      */
extern  	At      *d2_p_at  ;   /* pointer to AT	      */

extern  	signed32 d2_nsrt ;     /* number of SRT elements     */
extern  	signed32 d2_noct ;     /* number of OCT elements     */
extern  	signed32 d2_nat  ;     /* number of AT elements      */

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

static  void  d20_release(char **srt, char **oct, char **at);

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_2_admin_scheme()                                  */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        23.03.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_2_admin_scheme(parblk)                   */
/*								      */
/*              D20_pS  *parblk;                                      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will display the mask to select the     */
/*              operation to be performed on scheme.                  */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D20_pS      *parblk;   				      */ 
/*								      */	
/*	{  D2_pbhead         *d20_Spbhead;     -> pointer to head     */
/*	   char              *d20_Senv;        -> installation        */
/*					          environment         */
/*	} D20_pS;        					      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:        No error occurred                          */
/*      D2_ERROR:          Error occurred                             */
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

 signed16 d20_2_admin_scheme(
   D20_pS *parblk)
 
{                               /*  d20_2_admin_scheme()              */

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


  	Srt	*tmp_psrule ; /* tmp pointer to SRT	      */	
  	Oct     *tmp_pobj ;   /* tmp pointer to OCT	      */
  	At      *tmp_pattr ;  /* tmp pointer to AT	      */

  	signed32 tmp_nsrule ;     /* number of SRT elements     */
  	signed32 tmp_nobj ;       /* number of OCT elements     */
  	signed32 tmp_nattr ;      /* number of AT elements      */

   Bool     modify = FALSE;      /* a flag indicating whether  */
					/* the scheme has been        */
					/* modified                   */

   Oct	obj;            	/* OCT-info to be handled     */
   At 	attr;           	/* AT-info to be handled     */
   Srt	srule;			/* structure rule to be handled */

   Bool     init;                   /* a flag indicating whether  */
					/* an input mask must be      */
					/* initialized                */

   S_upd_info	upd_info;	/* structure to store update  */
					/* information.		      */

   Add_mod_at	add_mod_at;	/* structure to store add/mod */
					/* attribute information.     */
   Del_at		del_at;		/* structure to store delete  */
					/* attribute information.     */

   Add_mod_oct	add_mod_oct;	/* structure to store add/mod */
					/* object class information.  */
   Del_oct		del_oct;	/* structure to store delete  */
					/* obj. class information.    */

   Add_mod_srt	add_mod_srt;	/* structure to store add/mod */
					/* structure rule information.*/
   Del_srt		del_srt;	/* structure to store delete  */
					/* structure rule information.*/

   Abbrv		abbrv;		/* to store attribute/object  */
					/* class abbreviation.	      */

   signed32		rule_nr;	/* to store structure rule    */
					/* number.		      */

   char     file[D20_SHFNMAX];
Errcode		ret_code;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if (d2_p_srt == (Srt *)NULL)
	{/* DSA schema has not been read till now.	*/
	 /* read DSA schema for schema administration.  */
	if (d21_21_rdsh(d20_bid,&ret_code,DS_V03,&d2_nsrt,&d2_p_srt,
					&d2_noct,&d2_p_oct,
					&d2_nat,&d2_p_at) == D2_ERROR) 
		{
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E59IDX] ;
		d20_handlemask (d20_qomask) ;
		return(D2_ERROR);
		}
	}

for                             /*  EVER                              */
    ( ; ; )
{                               /*  display scheme administration mask */
  strcpy(d20_actopmsg, d20_op[DS__SCHEME_ADM]);
  *d20_9ifields[DS_SCH_BREAK] = DS_DEFAULT_VALUE;

  d20_handlemask (d20_9omask);

  if                            /*  "break" was   selected            */
     (*d20_9ifields[DS_SCH_BREAK] != DS_DEFAULT_VALUE)
  {                             /*  release memory                    */
    d20_release((char **) &d2_p_srt, (char **) &d2_p_oct, (char **) &d2_p_at);
				/*  return(D2_NOERROR)                */
    return(D2_NOERROR);
  }
  else
  {                             /*  check selected operation          */
    if                          /*  operation invalid                 */
       (*(signed32 *)d20_9ifields[DS_SCH_OP] < DS__MIN_OP ||
			*(signed32 *)d20_9ifields[DS_SCH_OP] > DS__MAX_OP)
    {                           /*  display error message             */
      d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E00IDX] ;
      d20_handlemask (d20_qomask) ;
    }
    else
    {                           
      strcpy(d20_actopmsg,
	     d20_s_opmsg[*(signed32 *)d20_9ifields[DS_SCH_OP]].d20_msgptr);
      init = TRUE;
      switch (*(int *)d20_9ifields[DS_SCH_OP]) /*  operation is          */
      {
	case DS__EXIT:          /*  EXIT                              */
				/*  release memory                    */
		d20_release((char **) &d2_p_srt, (char **) &d2_p_oct,
			    (char **) &d2_p_at);
				/*  return(D2_NOERROR)                */
		return(D2_NOERROR);
	case DS__STORE:         /*  store scheme                      */
		   if (modify == TRUE)
			{
		  	sprintf(file, "%s%s", parblk->d20_Senv, D20_SCHFILE);
		  	if (d21_22_stsh(d20_bid,&ret_code,DS_V03,file,
						d2_nsrt, d2_p_srt,
						d2_noct,d2_p_oct,
						d2_nat,d2_p_at) == 
								D2_NOERROR)
		  		{/*  modify flag = D2_FALSE            */
		    		modify = FALSE;
		    		unlink(file);
				}
			else
				{
				/* error during store schema has occured.  */
				/* error could be DS_NO_RESPONSE or        */
				/* DS_NO_RESPONSE and DS_NO_FILE	   */
  				switch ((int) ret_code.errvalue)
					{
					/* no response from DSA	*/
					case DS_NO_RESPONSE : 
						d20_qomask[D20_Q1POS].
						d20_ioinfo =
						d20_errtab[D20_E12IDX];
						break;
					case DS_NO_FILE :
						d20_qomask[D20_Q1POS].
						d20_ioinfo =
						d20_errtab[D20_E71IDX];
						break;
					}

      				d20_handlemask (d20_qomask) ;
				}
			}
		break;
	case DS__LOAD:          /*  load scheme                       */
		/* save parameters of old schema */
		tmp_nsrule = d2_nsrt;
		tmp_psrule = d2_p_srt;
		tmp_nobj = d2_noct;
		tmp_pobj = d2_p_oct;
		tmp_nattr = d2_nat ;
		tmp_pattr = d2_p_at ;
		sprintf(file, "%s%s", parblk->d20_Senv, D20_SCHFILE);
		if (d20_027_load(file, &d2_nsrt, &d2_p_srt, 
				       &d2_noct, &d2_p_oct, 
		        	       &d2_nat, &d2_p_at) == D2_NOERROR)
			{
			/* release memory occupied by old schema */
			d20_release((char **) &tmp_psrule, (char **) &tmp_pobj,
				    (char **) &tmp_pattr);
	
			/* set the modify flag.		*/
		   	modify = TRUE;
			}
		else
			{
			/* restore old parameters of schema	*/
			d2_nsrt = tmp_nsrule ; 
			d2_p_srt = tmp_psrule ;
			d2_noct = tmp_nobj ;
			d2_p_oct = tmp_pobj ;
			d2_nat = tmp_nattr ;
			d2_p_at = tmp_pattr ;
			}
		break;

	case DS__SRT_DISPLAY:   /*  display SRT			      */
		d20_021_display_srt(d2_nsrt, d2_p_srt);
		break;

	case DS__A_SRULE:	/*  add a new structure rule          */
		/* add new structure rule	*/
		/* set upd_info structure	*/

		upd_info.upd_mode = ADD_SRT_ENTRY;
		upd_info.upd_val.a_m_srt = &add_mod_srt;
		upd_info.upd_val.a_m_srt->srt_entry = &srule;
		upd_info.upd_val.a_m_srt->no_at = d2_nat;
		upd_info.upd_val.a_m_srt->at = d2_p_at;
		upd_info.upd_val.a_m_srt->no_oct = d2_noct;
		upd_info.upd_val.a_m_srt->oct = d2_p_oct;
		upd_info.upd_val.a_m_srt->no_srt = d2_nsrt;
		upd_info.upd_val.a_m_srt->srt = d2_p_srt;

		/* get structure rule to be added         	*/
		/* loop until break is selected	      		*/
		while (d20_026_get_new_srule(init, DS__A_SRULE, &srule) == 
								D2_NOERROR)
			{/*  check new structure rule and update SRT */
		     	if (d20_032_update(&upd_info) == D2_NOERROR)
		  		{/*  set modify flag                   */
				/*  break                             */
		    		modify = TRUE;
				
				d2_nsrt = upd_info.upd_val.a_m_srt->no_srt;
				d2_p_srt = upd_info.upd_val.a_m_srt->srt ;

		    		break;
				}
		  	init = FALSE;
			}
		break;

	case DS__D_SRULE:	/*  delete a structure rule	      */
		/* set upd_info structure	*/

		upd_info.upd_mode = DEL_SRT_ENTRY;
		upd_info.upd_val.d_srt = &del_srt;
		upd_info.upd_val.d_srt->no_srt = d2_nsrt;
		upd_info.upd_val.d_srt->srt = d2_p_srt;

		/*  get structure rule no. to be deleted       		*/
		/* loop until break is selected				*/
		while (d20_025_get_srule(init, &(del_srt.rule_nr)) == 
								D2_NOERROR)
			{/*  check for structure rule and update SRT	*/
		     	if (d20_032_update(&upd_info) == D2_NOERROR)
		  		{/*  set modify flag                   */
				/*  break                             */
		    		modify = TRUE;
				
				d2_nsrt = upd_info.upd_val.d_srt->no_srt;

		    		break;
				}
		  	init = FALSE;
			}
		break;

	case DS__M_SRULE:	/*  modify an existing structure rule */
		/* set upd_info structure	*/

		upd_info.upd_mode = MOD_SRT_ENTRY;
		upd_info.upd_val.a_m_srt = &add_mod_srt;
		upd_info.upd_val.a_m_srt->srt_entry = &srule;
		upd_info.upd_val.a_m_srt->no_at = d2_nat;
		upd_info.upd_val.a_m_srt->at = d2_p_at;
		upd_info.upd_val.a_m_srt->no_oct = d2_noct;
		upd_info.upd_val.a_m_srt->oct = d2_p_oct;
		upd_info.upd_val.a_m_srt->no_srt = d2_nsrt;
		upd_info.upd_val.a_m_srt->srt = d2_p_srt;

		/*  loop to get structure rule no.	*/
		/* loop until break is selected		*/
		while (d20_025_get_srule(init, &rule_nr) == D2_NOERROR)
			{
			signed32	j;

			/* entry should not belong to default schema.	*/
			if ((rule_nr >= DS_MIN_DEF_RULE_NR) &&
					(       rule_nr <= DS_MAX_DEF_RULE_NR))
				{
				d20_qomask[D20_Q1POS].d20_ioinfo =
						d20_err_table[D20_E92IDX];
  				d20_handlemask (d20_qomask) ;
				init = FALSE;
				continue;
				}
	
			if ((j=d20_034_search_srule(rule_nr,d2_nsrt,d2_p_srt))
								== -1)
				{
				/* structure rule . does not exist in Srt   */
				d20_qomask[D20_Q1POS].d20_ioinfo =
						d20_err_table[D20_E57IDX];
  				d20_handlemask (d20_qomask) ;
				init = FALSE;
				continue;
				}

			/* structure rule no. found in Srt 	  */
			/* copy the Srt entry from SRT 	   	  */
			/* to the structure to be modified.	  */
			memcpy((char *)&srule,(char *)(d2_p_srt + j),
							sizeof(Srt));

			/*  get new structure rule		*/
			/* loop until break is selected	      	*/
			while (d20_026_get_new_srule(FALSE,
					DS__M_SRULE,&srule) == D2_NOERROR)
				{
				/* check new structure rule and update*/
				/* Srt				      */
	     			if (d20_032_update(&upd_info) == D2_NOERROR)
		  			{
					/* set modify flag          */
		    			modify = TRUE;
		    			break;
					}
				}
			break;	/* break from the get structure rule */
			        /* no. loop 			     */
			}
		break;

	case DS__OCT_DISPLAY:   /*  display OCT                       */
		d20_003_display_oct(d2_noct, d2_p_oct);
		break;

	case DS__A_OBJ:         
		/* add new object class		*/
		/* set upd_info structure	*/

		upd_info.upd_mode = ADD_OCT_ENTRY;
		upd_info.upd_val.a_m_oct = &add_mod_oct;
		upd_info.upd_val.a_m_oct->oct_entry = &obj;
		upd_info.upd_val.a_m_oct->no_at = d2_nat;
		upd_info.upd_val.a_m_oct->at = d2_p_at;
		upd_info.upd_val.a_m_oct->no_oct = d2_noct;
		upd_info.upd_val.a_m_oct->oct = d2_p_oct;
		upd_info.upd_val.a_m_oct->no_srt = d2_nsrt;
		upd_info.upd_val.a_m_oct->srt = d2_p_srt;

		/* get object class to be added         	*/
		/* loop until break is selected	      		*/
		while (d20_006_get_new_obj(init, DS__A_OBJ, &obj) == D2_NOERROR)
			{/*  check new object class and update OCT	*/
		     	if (d20_032_update(&upd_info) == D2_NOERROR)
		  		{/*  set modify flag                   */
				/*  break                             */
		    		modify = TRUE;
				
				d2_noct = upd_info.upd_val.a_m_oct->no_oct;
				d2_p_oct = upd_info.upd_val.a_m_oct->oct ;

		    		break;
				}
		  	init = FALSE;
			}
		break;

	case DS__D_OBJ:         /*  delete object class		*/
		/* set upd_info structure	*/

		upd_info.upd_mode = DEL_OCT_ENTRY;
		upd_info.upd_val.d_oct = &del_oct;
		upd_info.upd_val.d_oct->oct_abbrv = abbrv;
		upd_info.upd_val.d_oct->no_oct = d2_noct;
		upd_info.upd_val.d_oct->oct = d2_p_oct;
		upd_info.upd_val.d_oct->no_srt = d2_nsrt;
		upd_info.upd_val.d_oct->srt = d2_p_srt;

		/*  get object class abbreviation to be deleted       	*/
		/* loop until break is selected		*/
		while (d20_005_get_obj(init, abbrv) == D2_NOERROR)
			{/*  check object class and update OCT		*/
		     	if (d20_032_update(&upd_info) == D2_NOERROR)
		  		{/*  set modify flag                   */
				/*  break                             */
		    		modify = TRUE;
				
				d2_noct = upd_info.upd_val.d_oct->no_oct;

		    		break;
				}
		  	init = FALSE;
			}
		break;

	case DS__M_OBJ:		/* modify an object class	      */
		/* set upd_info structure	*/

		upd_info.upd_mode = MOD_OCT_ENTRY;
		upd_info.upd_val.a_m_oct = &add_mod_oct;
		upd_info.upd_val.a_m_oct->oct_entry = &obj;
		upd_info.upd_val.a_m_oct->no_at = d2_nat;
		upd_info.upd_val.a_m_oct->at = d2_p_at;
		upd_info.upd_val.a_m_oct->no_oct = d2_noct;
		upd_info.upd_val.a_m_oct->oct = d2_p_oct;
		upd_info.upd_val.a_m_oct->no_srt = d2_nsrt;
		upd_info.upd_val.a_m_oct->srt = d2_p_srt;

		/*  loop to get object class abbreviation 	*/
		/* loop until break is selected		*/
		while (d20_005_get_obj(init, abbrv) == D2_NOERROR)
			{
			signed32	j;
	
			/* schema object class is not allowed to be modified. */
			if (strcmp(abbrv, S_OC_SCH) == 0)
				{
				d20_qomask[D20_Q1POS].d20_ioinfo =
						d20_errmess[D20_E66IDX];
  				d20_handlemask (d20_qomask) ;
				init = FALSE;
				continue;
				}

			if ((j=d20_024_search_obj_abbrv(abbrv,d2_noct,
				    (char *) d2_p_oct,D2_TOTAL_SCHEMA)) == -1)
				{
				/* obj. class abbrv. does not exist in Oct   */
				d20_qomask[D20_Q1POS].d20_ioinfo =
						d20_err_table[D20_E57IDX];
  				d20_handlemask (d20_qomask) ;
				init = FALSE;
				continue;
				}

			/* obj. class abbrv. found in Oct   */
			/* copy the Oct entry from Oct	*/
			/* to the structure to be modified.	  */
			memcpy((char *)&obj,(char *)(d2_p_oct + j),sizeof(Oct));

			/*  get new object class		*/
			/* loop until break is selected	      	*/
			while (d20_006_get_new_obj(FALSE,DS__M_OBJ,
							&obj) == D2_NOERROR)
				{
				/* check new obj. class and update Oct*/
		     		if (d20_032_update(&upd_info) == D2_NOERROR)
		  			{
					/* set modify flag          */
		    			modify = TRUE;
		    			break;
					}
				}
			break;	/* break from the get abbrv. loop */
			}
		break;

	case DS__AT_DISPLAY:   /*  display AT                       */
		d20_013_display_at(d2_nat, d2_p_at);
		break;

	case DS__A_ATTR:        /*  add attribute                     */
		/* set upd_info structure	*/

		upd_info.upd_mode = ADD_AT_ENTRY;
		upd_info.upd_val.a_m_at = &add_mod_at;
		upd_info.upd_val.a_m_at->at_entry = &attr;
		upd_info.upd_val.a_m_at->no_at = d2_nat;
		upd_info.upd_val.a_m_at->at = d2_p_at;

		/*  get attribute to be added         	*/
		/* loop until break is selected	      	*/
		while (d20_016_get_new_attr(init, DS__A_ATTR, &attr) == 
								D2_NOERROR)
			{/*  check new attribute  and update AT		*/
		     	if (d20_032_update(&upd_info) == D2_NOERROR)
		  		{/*  set modify flag                   */
				/*  break                             */
		    		modify = TRUE;
				
				d2_nat = upd_info.upd_val.a_m_at->no_at;
				d2_p_at = upd_info.upd_val.a_m_at->at ;

		    		break;
				}
		  	init = FALSE;
			}
		break;

	case DS__D_ATTR:        /*  delete attribute                  */
		/* set upd_info structure	*/

		upd_info.upd_mode = DEL_AT_ENTRY;
		upd_info.upd_val.d_at = &del_at;
		upd_info.upd_val.d_at->at_abbrv = abbrv;
		upd_info.upd_val.d_at->no_at = d2_nat;
		upd_info.upd_val.d_at->at = d2_p_at;
		upd_info.upd_val.d_at->no_oct = d2_noct;
		upd_info.upd_val.d_at->oct = d2_p_oct;
		upd_info.upd_val.d_at->no_srt = d2_nsrt;
		upd_info.upd_val.d_at->srt = d2_p_srt;

		/*  get attribute to be deleted       	*/
		/* loop until break is selected		*/
		while (d20_015_get_attr(init, abbrv) == D2_NOERROR)
			{/*  check attribute  and update AT		*/
		     	if (d20_032_update(&upd_info) == D2_NOERROR)
		  		{/*  set modify flag                   */
				/*  break                             */
		    		modify = TRUE;
				
				d2_nat = upd_info.upd_val.d_at->no_at;

		    		break;
				}
		  	init = FALSE;
			}
		break;

	case DS__M_ATTR:	/* modify an attribute 		*/
		/* set upd_info structure	*/

		upd_info.upd_mode = MOD_AT_ENTRY;
		upd_info.upd_val.a_m_at = &add_mod_at;
		upd_info.upd_val.a_m_at->at_entry = &attr;
		upd_info.upd_val.a_m_at->no_at = d2_nat;
		upd_info.upd_val.a_m_at->at = d2_p_at;
		upd_info.upd_val.a_m_at->no_srt = d2_nsrt;
		upd_info.upd_val.a_m_at->srt = d2_p_srt;

		/*  loop to get attribute abbreviation 	*/
		/* loop until break is selected		*/
		while (d20_015_get_attr(init, abbrv) == D2_NOERROR)
			{
			signed32	j;
	
			if ((j=d20_014_search_at_abbrv(abbrv,d2_nat,
				     (char *) d2_p_at, D2_TOTAL_SCHEMA)) == -1)
				{
				/* attribute abbrv. does not exist in At   */
				d20_qomask[D20_Q1POS].d20_ioinfo =
						d20_err_table[D20_E57IDX];
  				d20_handlemask (d20_qomask) ;
				init = FALSE;
				continue;
				}

			/* attribute abbrv. found in At   */
			/* copy the At entry from attribute table */
			/* to the structure to be modified.	  */
			memcpy((char *)&attr,(char *)(d2_p_at + j), sizeof(At));

			/*  get new attribute 		*/
			/* loop until break is selected	      	*/
			while (d20_016_get_new_attr(FALSE,DS__M_ATTR,
							&attr) == D2_NOERROR)
				{
				/* check new attribute and update AT */
		     		if (d20_032_update(&upd_info) == D2_NOERROR)
		  			{
					/* set modify flag          */
		    			modify = TRUE;
		    			break;
					}
				}
			break;	/* break from the get abbrv. loop */
			}

		break;

	default:                /*  otherwise                         */
				/*  break                             */
		break;
      }                         
    }                           
  }                             
}                               

}                               /*  d20_2_admin_scheme()              */


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_release()                                         */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        19.04.88                                              */
/*								      */
/* SYNTAX:      void  d20_release(srt, oct, at)                       */
/*		char   **srt;					      */
/*              char   **oct;                                         */
/*              char   **at;                                          */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function release memory of SRT, OCT and AT       */
/*              and will reset "init" flags.              	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		char   **srt;					      */
/*              char   **oct;                                         */
/*              char   **at;                                          */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		char   **srt;					      */
/*              char   **oct;                                         */
/*              char   **at;                                          */
/*								      */
/* RETURNVALUE:							      */
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
/*inoff ***************************************************************/

 static  void  d20_release(
   char **srt,
   char **oct,
   char **at)

{                               /*  d20_release()                     */

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

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if (*srt)
	free(*srt);
if (*oct)
	free(*oct);
if (*at)
	free(*at);
*srt = *oct = *at = (char *)NULL;

}                               /*  d20_release()                     */
