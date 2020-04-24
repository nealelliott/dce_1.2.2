/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20store.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:32  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:13:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:48  root]
 * 
 * Revision 1.1.8.4  1994/07/06  15:06:21  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:11:02  marrek]
 * 
 * Revision 1.1.8.3  1994/06/21  14:43:17  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  13:08:06  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:31  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:05:24  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:20  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:20:38  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:16:11  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:17:22  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:20  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:01:11  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:56:19  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:03:37  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:42:53  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:11:08  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20store.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:31 $";
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
/* NAME         : d20store.o  [store scheme]                          */
/*								      */
/* AUTHOR       : Sanjay, D AP 11                                     */
/* DATE         : 29.01.91					      */
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
/*      0.1 | 04.07.88| Original                       | ws |         */
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
#include <string.h>
#include <time.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d21iapl.h>
#include <d2shm.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define D20_T_STRING    "%02d%02d%02d%02d%02d%02dZ"
#define D2_SCH_AT_SIZE  ((D2_AT_MAX) * (((D2_L_SRT_ATT > D2_L_OCT_ATT) ? D2_L_SRT_ATT : D2_L_OCT_ATT) > D2_L_AT_ATT ? ((D2_L_SRT_ATT > D2_L_OCT_ATT) ? D2_L_SRT_ATT : D2_L_OCT_ATT) : D2_L_AT_ATT))

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

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

static signed32 d21_del_attrs(D21_pC  *pb);
static signed32 d21_add_srt(signed32 *no_modinf, D21_pC *pb, signed32 no_srt,
   			Srt *srt);
static	signed32 d21_add_oct(signed32 *no_modinf, D21_pC *pb, signed32 no_oct,
   			Oct *oct);
static	signed32 d21_add_at(signed32 *no_modinf, D21_pC *pb, signed32 no_at,
   			At *at);
static signed32 d21_add_tst(signed32 *no_modinf, D21_pC *pb);
static char *d20_get_utc_time(void);
static void d21_store_schema(FILE *fp, signed32 no_srt, Srt *srt,
			signed32 no_oct, Oct *oct, signed32 no_at, At *at);

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21_22_stsh()                                  	      */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        29.01.91					      */
/*								      */
/* SYNTAX:      signed32 d21_22_stsh(bind_id,ret_code,version,        */
/*				     file_name,no_srt,srt,no_oct,oct, */
/*				     no_at,at)			      */
/*								      */
/*		signed16  bind_id;				      */
/*		Errcode  *ret_code;				      */
/*		signed16  version;				      */
/*		char	 *file_name;				      */
/*		signed32  no_srt;			      	      */
/*		Srt	 *srt;				      	      */
/*		signed32  no_oct;			              */
/*		Oct	 *oct;				      	      */
/*		signed32 no_at;				      	      */
/*		At       *at;				      	      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will store the new scheme in the DSA.   */
/*              If the operation fails the new scheme is stored in a  */
/*              local file.                                           */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed16  bind_id;				      */
/*		Errcode  *ret_code;				      */
/*		signed16  version;				      */
/*		char	 *file_name;				      */
/*		signed32  no_srt;			      	      */
/*		Srt	 *srt;				      	      */
/*		signed32  no_oct;			      	      */
/*		Oct	 *oct;				      	      */
/*		signed32  no_at;			      	      */
/*		At       *at;				      	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred                                */
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

 signed32 d21_22_stsh(
   signed16  bind_id,
   Errcode  *ret_code,
   signed16  version,
   char     *file_name,
   signed32  no_srt,
   Srt      *srt,
   signed32  no_oct,
   Oct      *oct,
   signed32  no_at,
   At       *at)

{			/*  d21_22_stsh()              */

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

	D2_pbhead	head;
	D21_pC		pb ;
        D2_c_arg	c_arg;
 	signed32     	ret_value = D2_NOERROR;

	/* modify attribute information area */
        D2_a_modinf     d21_minfo[D2_AT_MAX] ;	

	/* attribute value reference area */
        D2_a_value  	d21_aval[D2_AT_MAX] ;       

	/* attribute value area */
 	char	   	d21_values[D2_SCH_AT_SIZE + 1] ;	

	/* keeps the count of attribute values entered so far in mod. info. */ 
	/* structure for modification.					    */
	signed32		no_modinf; 

        FILE            *fp;

/*
static char        *f_name = "d21_22_stsh";
static char        *f_entry = "ENTRY";
static char        *f_exit = "EXIT";
*/

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

d20_errcode(ret_code, D2_NOERROR, D2_NOERROR, D2_NOERROR);
d20_error(&head, D2_NOERROR, D2_NOERROR, D2_NOERROR);
/* d21_l03_trace_err(bind_id, f_name, f_entry, &head); */

/*  version is wrong                  */
if (version != DS_V03)
	{/*  set error values                  */
	/*  return(D2_ERROR)                  */
  	d20_errcode(ret_code, CALLERR, ILLVERS, DS_NOT_SPEC);
  	d20_error(&head, CALLERR, ILLVERS, DS_NOT_SPEC);
  	/* d21_l03_trace_err(bind_id, f_name, f_exit, &head); */
  	return(D2_ERROR);
	}

/* initialize the parameter block */
head.d2_version = D21V022;
pb.d21_Cpbhead = &head;

c_arg.d2_extension = (D2_extension *)NULL;
c_arg.d2_op_progress.d2_nr_phase = D2_NOT_STARTED;
c_arg.d2_op_progress.d2_next_rdn = 0;
c_arg.d2_al_rdn = 0;
c_arg.d2_serv_cntrls = D2_NOCACHE | D2_NOREFERRAL | D2_USEDSA | 
		       D2_DREFALIAS | D2_DONT_STORE | D2_PREF_ADM_FUN;
c_arg.d2_priority = D2_PRIO_LOW;
c_arg.d2_time_limit = D2_T_UNLIMITED;
c_arg.d2_size_limit = D2_S_UNLIMITED;
c_arg.d2_scope_ref = D2_WORLD_SCOPE; 

pb.d21_Cc_a = &c_arg;

pb.d21_Cbind_id = bind_id;
pb.d21_Centry = (D2_name_string)D2_NAME_SCHEME;

no_modinf = 0;
pb.d21_Cmod = d21_minfo;
pb.d21_Cmod->d2_m_attr.d2_a_val = d21_aval;
pb.d21_Cmod->d2_m_attr.d2_a_val->d2_at_value.d2_oct_a_v = 
						(Octet_string)d21_values;
pb.d21_Cerr = (D2_error *)NULL;

/* delete SRT, OCT, AT and TIMESTAMP attributes of the schema object	*/
if ((ret_value = d21_del_attrs(&pb)) == D2_NOERROR)
	/* fill modify structure with SRT entries	*/
	if ((ret_value = d21_add_srt(&no_modinf,&pb,no_srt,srt)) == D2_NOERROR)
		/* fill modify structure with OCT entries	*/
		if ((ret_value = d21_add_oct(&no_modinf,&pb,no_oct,oct)) == 
								D2_NOERROR)
			/* fill modify structure with AT entries	*/
			if ((ret_value = d21_add_at(&no_modinf,&pb,no_at,at)) 
								== D2_NOERROR)
				/* fill modify structure with TIMESTAMP */
				ret_value = d21_add_tst(&no_modinf,&pb);

if (ret_value == D2_ERROR)
	{/*  scheme update fails               */
	/* free space of error information     */
	if (pb.d21_Cerr != (D2_error *)NULL)
		free((char *)pb.d21_Cerr);	

	/*  set error values                  */
  	d20_errcode(ret_code, D2_TEMP_ERR, D2_REM_ERR, DS_NO_RESPONSE);
  	d20_error(&head, D2_TEMP_ERR, D2_REM_ERR, DS_NO_RESPONSE);

	/* store schema in a file	*/
     	if ((fp = fopen(file_name, "w")) == (FILE *)NULL)
  		{/*  open of local scheme file failed   */
  		 /*  set error values                   */
  		d20_errcode(ret_code, TEMPERR, SYSERR, DS_NO_FILE);
  		d20_error(&head, TEMPERR, SYSERR, DS_NO_FILE);
  		}
	else
		{/*  store scheme in local file         */
		d21_store_schema(fp,no_srt, srt, no_oct, oct, no_at, at);
    		fclose(fp);
		}
	}

/* d21_l03_trace_err(bind_id, f_name, f_exit, &head); */
return(ret_value);

}			/*  d21_22_stsh()              */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21_del_attrs()                                       */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        29.01.91					      */
/*								      */
/* SYNTAX:      signed32 d21_del_attrs(pb)		      	      */
/*								      */
/*		D21_pC  *pb;					      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function deletes TIME_STAMP, SRT, OCT and AT     */
/*		attributes of the schema object in DSA.		      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		D21_pC  *pb;					      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred                                */
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

 static signed32 d21_del_attrs(
   D21_pC  *pb)

{

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

	D2_a_modinf      *d21_miptr ;

/***********************************************************************/
/*								       */
/*			      C	O D E				       */
/*								       */
/***********************************************************************/

/* initialize mod. info. pointer	*/
d21_miptr = pb->d21_Cmod;

/* fill mod. info. structure with TIMESTAMP */
d21_miptr->d2_m_next = d21_miptr + 1;
d21_miptr->d2_m_mod = D2_DELATT;

d21_miptr->d2_m_attr.d2_a_next 	= (D2_a_info *)NULL;
d21_miptr->d2_m_attr.d2_a_type.d2_type 	= (Ob_id_string)D2_TIME_STAMP;
d21_miptr->d2_m_attr.d2_a_type.d2_typ_len = sizeof(D2_TIME_STAMP) - 1;
d21_miptr->d2_m_attr.d2_a_no_val = 0; 

d21_miptr++;

/* fill mod. info. structure with SRT attribute */
d21_miptr->d2_m_next = d21_miptr + 1;
d21_miptr->d2_m_mod = D2_DELATT;

d21_miptr->d2_m_attr.d2_a_next	= (D2_a_info *)NULL;
d21_miptr->d2_m_attr.d2_a_type.d2_type = (Ob_id_string)D2_SRT;
d21_miptr->d2_m_attr.d2_a_type.d2_typ_len = sizeof(D2_SRT) - 1;
d21_miptr->d2_m_attr.d2_a_no_val = 0; 

d21_miptr++;

/* fill mod. info. structure with OCT attribute */
d21_miptr->d2_m_next = d21_miptr + 1;
d21_miptr->d2_m_mod = D2_DELATT;

d21_miptr->d2_m_attr.d2_a_next 	= (D2_a_info *)NULL;
d21_miptr->d2_m_attr.d2_a_type.d2_type 	= (Ob_id_string)D2_OCT;
d21_miptr->d2_m_attr.d2_a_type.d2_typ_len = sizeof(D2_OCT) - 1;
d21_miptr->d2_m_attr.d2_a_no_val = 0; 

d21_miptr++;

/* fill mod. info. structure with AT attribute */
d21_miptr->d2_m_next = (D2_a_modinf *)NULL;
d21_miptr->d2_m_mod = D2_DELATT;

d21_miptr->d2_m_attr.d2_a_next = (D2_a_info *)NULL;
d21_miptr->d2_m_attr.d2_a_type.d2_type 	= (Ob_id_string)D2_AT;
d21_miptr->d2_m_attr.d2_a_type.d2_typ_len = sizeof(D2_AT) - 1;
d21_miptr->d2_m_attr.d2_a_no_val = 0; 

/* send the modify request to delete attributes of schema object.	*/
return((signed32)d21_C_modify_attribute(pb)) ;

}			/* end of d21_del_attrs()	*/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21_add_srt()                                         */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        29.01.91					      */
/*								      */
/* SYNTAX:      signed32 d21_add_srt(no_modinf,pb,no_srt,srt)	      */
/*								      */
/*		signed32 *no_modinf;				      */
/*		D21_pC   *pb;					      */
/*		signed32  no_srt;			      	      */
/*		Srt	 *srt;				      	      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will add SRT attribute in the schema    */
/*		object in the DSA.   				      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed32 *no_modinf;				      */
/*		D21_pC   *pb;					      */
/*		signed32  no_srt;			      	      */
/*		Srt	 *srt;				      	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		signed32 *no_modinf;				      */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred                                */
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

 static signed32 d21_add_srt(
   signed32 *no_modinf,
   D21_pC   *pb,
   signed32  no_srt,
   Srt      *srt)

{

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

D2_a_modinf      *d21_miptr ;
D2_a_value       *d21_avptr ;
Octet_string     d21_valptr ;
Octet_string     run_ptr;
Srt		 *srt_ent;
signed32	 counter,i;
signed16	 mod_type;

/***********************************************************************/
/*								       */
/*			      C	O D E				       */
/*								       */
/***********************************************************************/

/* initialize pointers to fill mod. info. structure */
d21_miptr 	= pb->d21_Cmod + *no_modinf;
d21_avptr 	= pb->d21_Cmod->d2_m_attr.d2_a_val + *no_modinf;
if (*no_modinf == 0)
	d21_valptr = pb->d21_Cmod->d2_m_attr.d2_a_val->d2_at_value.d2_oct_a_v;
else
	d21_valptr = (d21_avptr - 1)->d2_at_value.d2_oct_a_v +
		     strlen((char *)(d21_avptr - 1)->d2_at_value.d2_oct_a_v) ;

/* for all the SRT entries fill modify structure */
/* for first modinfo., kind of modification is 'Add a new attribute' */
/* for rest of modinfo., kind of modification is 'Add a new value'   */
/* if no. of modifications reach max. limit, send the modify request */

for (counter = 0,mod_type = D2_ADDATT, srt_ent = srt; counter < no_srt; 
			       counter++,d21_miptr++,srt_ent++,(*no_modinf)++)
	{
	/* no. of modifications reached max. limit. Send the modify request. */
	if (*no_modinf == D2_AT_MAX)
		{
		(d21_miptr - 1)->d2_m_next = (D2_a_modinf *)NULL;
		if (d21_C_modify_attribute(pb) == D2_ERROR)
			return(D2_ERROR);

		*no_modinf = 0;

		/* initialize pointers to fill mod. info. structure */
		d21_miptr = pb->d21_Cmod ;
		d21_avptr = pb->d21_Cmod->d2_m_attr.d2_a_val ;
		d21_valptr = 
		pb->d21_Cmod->d2_m_attr.d2_a_val->d2_at_value.d2_oct_a_v ;
		}
	
	/* fill next mod. info structure.	*/
        d21_miptr->d2_m_next = d21_miptr + 1 ;
        d21_miptr->d2_m_mod = mod_type ;

        d21_miptr->d2_m_attr.d2_a_next	= (D2_a_info *)NULL;
        d21_miptr->d2_m_attr.d2_a_type.d2_type = (Ob_id_string)D2_SRT;
        d21_miptr->d2_m_attr.d2_a_type.d2_typ_len = sizeof(D2_SRT) - 1;
	d21_miptr->d2_m_attr.d2_a_no_val = 1;
	d21_miptr->d2_m_attr.d2_a_val = d21_avptr ;

	d21_avptr->d2_a_rep = (unsigned16)d27_401_syntax_to_rep(
				(signed16)D2_PRINTABLE);
	d21_avptr->d2_at_value.d2_oct_a_v = d21_valptr ;

	/* store the SRT entry value */
	sprintf((char *)d21_valptr, D2_FORMAT_SRT, srt_ent->rule_nr,
						   srt_ent->par_rule_nr);

	run_ptr = d21_valptr + strlen((char *)d21_valptr);

	/* store naming attributes.	*/
	for (i = 0; i < srt_ent->no_nam_attr; i++)
		{
		sprintf((char *)run_ptr," %s",srt_ent->nam_attr[i]);
		run_ptr += strlen((char *)run_ptr);
		}

	sprintf((char *)run_ptr," %s",D2_ATT_SEPRATOR);
	run_ptr += strlen((char *)run_ptr);

	/* store object class for a structure rule */
	sprintf((char *)run_ptr," %s",srt_ent->obj_class);

	d21_avptr->d2_a_v_len = strlen((char *)d21_valptr);
	d21_avptr++;
	d21_valptr += strlen((char *)d21_valptr);
	mod_type = D2_ADD_AV;
	}

return(D2_NOERROR);

}			/* end of d21_add_srt()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21_add_oct()                                         */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        29.01.91					      */
/*								      */
/* SYNTAX:      signed32 d21_add_oct(no_modinf,pb,no_oct,oct)	      */
/*								      */
/*		signed32 *no_modinf;				      */
/*		D21_pC   *pb;					      */
/*		signed32  no_oct;			      	      */
/*		Oct	 *oct;				      	      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will add OCT attribute in the schema    */
/*		object in the DSA.   				      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed32 *no_modinf;				      */
/*		D21_pC   *pb;					      */
/*		signed32  no_oct;			      	      */
/*		Oct	 *oct;				      	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		signed32 *no_modinf;				      */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred                                */
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

 static	signed32 d21_add_oct(
   signed32 *no_modinf,
   D21_pC   *pb,
   signed32  no_oct,
   Oct      *oct)

{

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

D2_a_modinf      *d21_miptr ;
D2_a_value       *d21_avptr ;
Octet_string     d21_valptr ;
Octet_string     run_ptr;
Oct		 *oct_ent;
signed32	 counter,i;
signed16	 mod_type;

/***********************************************************************/
/*								       */
/*			      C	O D E				       */
/*								       */
/***********************************************************************/

/* initialize pointers to fill mod. info. structure */
d21_miptr 	= pb->d21_Cmod + *no_modinf;
d21_avptr 	= pb->d21_Cmod->d2_m_attr.d2_a_val + *no_modinf;
if (*no_modinf == 0)
	d21_valptr = pb->d21_Cmod->d2_m_attr.d2_a_val->d2_at_value.d2_oct_a_v;
else
	d21_valptr = (d21_avptr - 1)->d2_at_value.d2_oct_a_v +
		     strlen((char *)(d21_avptr - 1)->d2_at_value.d2_oct_a_v);

/* for all the OCT entries fill modify structure */
/* for first modinfo., kind of modification is 'Add a new attribute' */
/* for rest of modinfo., kind of modification is 'Add a new value'   */
/* if no. of modifications reach max. limit, send the modify request */

for (counter = 0,mod_type = D2_ADDATT, oct_ent = oct; counter < no_oct; 
			      counter++,d21_miptr++,oct_ent++,(*no_modinf)++)
	{
	/* no. of modifications reached max. limit. Send the modify request */
	if (*no_modinf == D2_AT_MAX)
		{
		(d21_miptr - 1)->d2_m_next = (D2_a_modinf *)NULL;
		if (d21_C_modify_attribute(pb) == D2_ERROR)
			return(D2_ERROR);

		*no_modinf = 0;

		/* initialize pointers to fill mod. info. structure */
		d21_miptr = pb->d21_Cmod ;
		d21_avptr = pb->d21_Cmod->d2_m_attr.d2_a_val ;
		d21_valptr = 
		pb->d21_Cmod->d2_m_attr.d2_a_val->d2_at_value.d2_oct_a_v ;
		}

	/* fill next mod. info structure.	*/
        d21_miptr->d2_m_next = d21_miptr + 1 ;
        d21_miptr->d2_m_mod = mod_type ;

        d21_miptr->d2_m_attr.d2_a_next = (D2_a_info *)NULL;
        d21_miptr->d2_m_attr.d2_a_type.d2_type = (Ob_id_string)D2_OCT;
        d21_miptr->d2_m_attr.d2_a_type.d2_typ_len = sizeof(D2_OCT) - 1;
	d21_miptr->d2_m_attr.d2_a_no_val = 1;
	d21_miptr->d2_m_attr.d2_a_val = d21_avptr ;

	d21_avptr->d2_a_rep = (unsigned16)d27_401_syntax_to_rep(
				(signed16)D2_PRINTABLE);
	d21_avptr->d2_at_value.d2_oct_a_v = d21_valptr ;

	/* store the OCT entry value */
	sprintf((char *)d21_valptr,D2_FORMAT_OCT,oct_ent->obj_abbrv,
					  oct_ent->obj_id,
					  oct_ent->obj_name,
					  oct_ent->obj_class_kind,
					  oct_ent->obj_file_nr);

	run_ptr = d21_valptr + strlen((char *)d21_valptr);

	/* store super classes.		*/

	if (oct_ent->no_sup_class == 0)
		{
		sprintf((char *)run_ptr," ");
		run_ptr += strlen((char *)run_ptr);
		}
	else
		{
		for (i = 0; i < oct_ent->no_sup_class; i++)
			{
			sprintf((char *)run_ptr," %s",oct_ent->sup_class[i]);
			run_ptr += strlen((char *)run_ptr);
			}
		}

	sprintf((char *)run_ptr," %s",D2_ATT_SEPRATOR);
	run_ptr += strlen((char *)run_ptr);

	/* store auxiliary classes.		*/
	if (oct_ent->no_aux_class == 0)
		{
		sprintf((char *)run_ptr," ");
		run_ptr += strlen((char *)run_ptr);
		}
	else
		{
		for (i = 0; i < oct_ent->no_aux_class; i++)
			{
			sprintf((char *)run_ptr," %s",oct_ent->aux_class[i]);
			run_ptr += strlen((char *)run_ptr);
			}
		}

	sprintf((char *)run_ptr," %s",D2_ATT_SEPRATOR);
	run_ptr += strlen((char *)run_ptr);

	/* store mandatory attributes for an object class 	*/
	if (oct_ent->no_must_attr == 0)
		{
		sprintf((char *)run_ptr," ");
		run_ptr += strlen((char *)run_ptr);
		}
	else
		{
		for (i = 0; i < oct_ent->no_must_attr; i++)
			{
			sprintf((char *)run_ptr," %s",oct_ent->must_attr[i]);
			run_ptr += strlen((char *)run_ptr);
			} 
		}

	sprintf((char *)run_ptr," %s",D2_ATT_SEPRATOR);
	run_ptr += strlen((char *)run_ptr);

	/* store optional attributes for an object class	*/
	if (oct_ent->no_may_attr == 0)
		{
		sprintf((char *)run_ptr," ");
		run_ptr += strlen((char *)run_ptr);
		}
	else
		{
		for (i = 0; i < oct_ent->no_may_attr; i++)
			{
			sprintf((char *)run_ptr," %s",oct_ent->may_attr[i]);
			run_ptr += strlen((char *)run_ptr);
			} 
		}

	d21_avptr->d2_a_v_len = strlen((char *)d21_valptr);
	d21_avptr++;
	d21_valptr += strlen((char *)d21_valptr);
	mod_type = D2_ADD_AV;
	}

return(D2_NOERROR);

}			/* end of d21_add_oct()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21_add_at()                                          */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        29.01.91					      */
/*								      */
/* SYNTAX:      signed32 d21_add_at(no_modinf,pb,no_at,at)	      */
/*								      */
/*		signed32 *no_modinf;				      */
/*		D21_pC   *pb;					      */
/*		signed32  no_at;			      	      */
/*		At	 *at;				      	      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will add AT attribute in the schema     */
/*		object in the DSA.   				      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed32 *no_modinf;				      */
/*		D21_pC   *pb;					      */
/*		signed32  no_at;			      	      */
/*		At	 *at;				      	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		signed32 *no_modinf;				      */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred                                */
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

 static	signed32 d21_add_at(
   signed32 *no_modinf,
   D21_pC   *pb,
   signed32  no_at,
   At       *at)

{

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

D2_a_modinf      *d21_miptr ;
D2_a_value       *d21_avptr ;
Octet_string     d21_valptr ;
At		 *at_ent;
signed32	 counter;
signed16	 mod_type;

/***********************************************************************/
/*								       */
/*			      C	O D E				       */
/*								       */
/***********************************************************************/

/* initialize pointers to fill mod. info. structure */
d21_miptr 	= pb->d21_Cmod + *no_modinf;
d21_avptr 	= pb->d21_Cmod->d2_m_attr.d2_a_val + *no_modinf;
if (*no_modinf == 0)
	d21_valptr = pb->d21_Cmod->d2_m_attr.d2_a_val->d2_at_value.d2_oct_a_v;
else
	d21_valptr = (d21_avptr - 1)->d2_at_value.d2_oct_a_v +
		     strlen((char *)(d21_avptr - 1)->d2_at_value.d2_oct_a_v);

/* for all the AT entries fill modify structure */
/* for first modinfo., kind of modification is 'Add a new attribute' */
/* for rest of modinfo., kind of modification is 'Add a new value'   */
/* if no. of modifications reach max. limit, send the modify request */

for (counter = 0,mod_type = D2_ADDATT, at_ent = at; counter < no_at; 
			counter++,d21_miptr++,at_ent++,(*no_modinf)++)
	{
	/* no. of modifications reached max. limit. Send the modify request */
	if (*no_modinf == D2_AT_MAX)
		{
		(d21_miptr - 1)->d2_m_next = (D2_a_modinf *)NULL;
		if (d21_C_modify_attribute(pb) == D2_ERROR)
			return(D2_ERROR);

		*no_modinf = 0;

		/* initialize pointers to fill mod. info. structure */
		d21_miptr = pb->d21_Cmod ;
		d21_avptr = pb->d21_Cmod->d2_m_attr.d2_a_val ;
		d21_valptr = 
		pb->d21_Cmod->d2_m_attr.d2_a_val->d2_at_value.d2_oct_a_v ;
		}

	/* fill next mod. info. structure.	*/
        d21_miptr->d2_m_next = d21_miptr + 1 ;
        d21_miptr->d2_m_mod = mod_type ;

        d21_miptr->d2_m_attr.d2_a_next = (D2_a_info *)NULL;
        d21_miptr->d2_m_attr.d2_a_type.d2_type = (Ob_id_string)D2_AT;
        d21_miptr->d2_m_attr.d2_a_type.d2_typ_len = sizeof(D2_AT) - 1;
	d21_miptr->d2_m_attr.d2_a_no_val = 1;
	d21_miptr->d2_m_attr.d2_a_val = d21_avptr ;

	d21_avptr->d2_a_rep = (unsigned16)d27_401_syntax_to_rep(
				(signed16)D2_PRINTABLE);
	d21_avptr->d2_at_value.d2_oct_a_v = d21_valptr ;

	/* store the AT entry value */
	sprintf((char *)d21_valptr, D2_FORMAT_AT, 
					at_ent->att_abbrv,at_ent->att_obj_id,
					 at_ent->att_name,at_ent->att_lower,
					 at_ent->att_upper,at_ent->att_no_recur,
					 at_ent->att_syntax,at_ent->att_phon,
					 at_ent->att_acl, at_ent->att_ind_lev);

	d21_avptr->d2_a_v_len = strlen((char *)d21_valptr);
	d21_avptr++;
	d21_valptr += strlen((char *)d21_valptr);
	mod_type = D2_ADD_AV;
	}

return(D2_NOERROR);

}			/* end of d21_add_at()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21_add_tst()                                         */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        29.01.91					      */
/*								      */
/* SYNTAX:      signed32 d21_add_tst(no_modinf,pb)	      	      */
/*								      */
/*		signed32 *no_modinf;				      */
/*		D21_pC   *pb;					      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will add TIME_STAMP  attribute in the   */
/*		schema object in the DSA.  			      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed32 *no_modinf;				      */
/*		D21_pC   *pb;					      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred                                */
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

 static signed32 d21_add_tst(
   signed32 *no_modinf,
   D21_pC   *pb)

{

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

D2_a_modinf      *d21_miptr ;
D2_a_value       *d21_avptr ;
Octet_string     d21_valptr ;

/***********************************************************************/
/*								       */
/*			      C	O D E				       */
/*								       */
/***********************************************************************/

/* initialize pointers to fill mod. info. structure */
d21_miptr 	= pb->d21_Cmod + *no_modinf;
d21_avptr 	= pb->d21_Cmod->d2_m_attr.d2_a_val + *no_modinf;
if (*no_modinf == 0)
	d21_valptr = pb->d21_Cmod->d2_m_attr.d2_a_val->d2_at_value.d2_oct_a_v;
else
	d21_valptr = (d21_avptr - 1)->d2_at_value.d2_oct_a_v +
		     strlen((char *)(d21_avptr - 1)->d2_at_value.d2_oct_a_v);

/* no. of modifications reached max. limit. Send the modify request */
if (*no_modinf == D2_AT_MAX)
	{
	(d21_miptr - 1)->d2_m_next = (D2_a_modinf *)NULL;
	if (d21_C_modify_attribute(pb) == D2_ERROR)
		return(D2_ERROR);

	*no_modinf = 0;

	/* initialize pointers to fill mod. info. structure */
	d21_miptr = pb->d21_Cmod ;
	d21_avptr = pb->d21_Cmod->d2_m_attr.d2_a_val ;
	d21_valptr = 
	pb->d21_Cmod->d2_m_attr.d2_a_val->d2_at_value.d2_oct_a_v ;
	}
	
/* fill mod. info. structure with TIMESTAMP attribute 	*/
d21_miptr->d2_m_next = (D2_a_modinf *)NULL;
d21_miptr->d2_m_mod = D2_ADDATT ;

d21_miptr->d2_m_attr.d2_a_next	= (D2_a_info *)NULL;
d21_miptr->d2_m_attr.d2_a_type.d2_type = (Ob_id_string)D2_TIME_STAMP;
d21_miptr->d2_m_attr.d2_a_type.d2_typ_len = sizeof(D2_TIME_STAMP) - 1;
d21_miptr->d2_m_attr.d2_a_no_val = 1;
d21_miptr->d2_m_attr.d2_a_val = d21_avptr ;

d21_avptr->d2_a_rep = (unsigned16)d27_401_syntax_to_rep((signed16)D2_TIME);
d21_avptr->d2_at_value.d2_oct_a_v = d21_valptr ;
sprintf((char *)d21_valptr,"%s",d20_get_utc_time());
d21_avptr->d2_a_v_len = strlen((char *)d21_valptr);

/* send modify request to add attributes.	*/
return((signed32)d21_C_modify_attribute(pb)) ;

}			/* end of d21_add_tst()		*/

/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_get_utc_time()                                    */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        14.04.88                                              */
/*								      */
/* SYNTAX:      char *d20_get_utc_time()                              */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will determine the UTC-time.            */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*      != NULL:     pointer to UTC-time string                       */
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

 static char *d20_get_utc_time(void)

{                               /*  d20_get_utc_time()                */

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

static  char        utc_time[D20_SUTCTIME];

    time_t      t_stamp;
    struct tm  *t;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

t_stamp = time((time_t *) 0L);
t = gmtime(&t_stamp);
sprintf(utc_time, D20_T_STRING, t->tm_year, t->tm_mon + 1, t->tm_mday,
		  (t->tm_hour == 24) ? 0 : t->tm_hour, t->tm_min, t->tm_sec);

				/*  return(UTC-time)                  */
return(utc_time);

}                               /*  d20_get_utc_time()                */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21_store_schema()                                    */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        29.01.91					      */
/*								      */
/* SYNTAX:      void d21_store_schema(fp, no_srt, srt, no_oct, oct,   */
/*					no_at, at)		      */
/*								      */
/*		FILE	 *fp;					      */
/*		signed32  no_srt;				      */
/*		Srt	 *srt;					      */
/*		signed32  no_oct;				      */
/*		Oct	 *oct;					      */
/*		signed32  no_at;				      */
/*		At	 *at;					      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function stores the DSA schema in a file.        */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		FILE	 *fp;					      */
/*		signed32  no_srt;				      */
/*		Srt	 *srt;					      */
/*		signed32  no_oct;				      */
/*		Oct	 *oct;					      */
/*		signed32  no_at;				      */
/*		At	 *at;					      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
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
/*exoff ***************************************************************/

 static void d21_store_schema(
   FILE     *fp,
   signed32  no_srt,
   Srt      *srt,
   signed32  no_oct,
   Oct      *oct,
   signed32  no_at,
   At       *at)

{

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

Srt	*srt_ent = srt;
Oct	*oct_ent = oct;
At	*at_ent = at;
signed32 i,j;

/***********************************************************************/
/*								       */
/*			      C	O D E				       */
/*								       */
/***********************************************************************/

/**************** write AT in file ********************/

/* write AT header */
fprintf(fp,DSAAT_HEAD,no_at);

/* for all attribute entries */
for (i=0;i<no_at;i++,at_ent++)
	fprintf(fp,DSAAT_FORMAT, at_ent->att_abbrv,at_ent->att_obj_id,
				 at_ent->att_name, at_ent->att_lower,
				 at_ent->att_upper,at_ent->att_no_recur,
				 at_ent->att_syntax,at_ent->att_acl,
				 at_ent->att_ind_lev, at_ent->att_phon);

/**************** write OCT in ********************/

/* write OCT header */
fprintf(fp,DSAOCT_HEAD,no_oct);

/* for all object class entries */
for (i=0;i<no_oct;i++,oct_ent++)
	{
	fprintf(fp,DSAOCT_FORMAT,oct_ent->obj_abbrv,
				 oct_ent->obj_id,
				 oct_ent->obj_name,
				 oct_ent->obj_class_kind,
				 oct_ent->obj_file_nr,
				 oct_ent->no_sup_class,
				 oct_ent->no_aux_class,
				 oct_ent->no_must_attr,
				 oct_ent->no_may_attr);

	/* write super classes	*/
	fprintf(fp,"Sup.-classes: ");

	for (j=0;j<oct_ent->no_sup_class;j++)
		fprintf(fp,"%s ",oct_ent->sup_class[j]);

	/* write auxiliary object classes	*/
	fprintf(fp,"Aux.-classes: ");

	for (j=0;j<oct_ent->no_aux_class;j++)
		fprintf(fp,"%s ",oct_ent->aux_class[j]);

	/* write mandatory attributes */
	fprintf(fp,"Mand: ");

	for (j=0;j<oct_ent->no_must_attr;j++)
		fprintf(fp,"%s ",oct_ent->must_attr[j]);

	/* write optional attributes */
	fprintf(fp,"Opt: ");

	for (j=0;j<oct_ent->no_may_attr;j++)
		fprintf(fp,"%s ",oct_ent->may_attr[j]);

	fprintf(fp,"\n");
	}

/**************** write SRT in file ********************/

/* write SRT header */
fprintf(fp,DSASRT_HEAD,no_srt);

/* for all structural rule entries */
for (i=0;i<no_srt;i++,srt_ent++)
	{
	fprintf(fp,DSASRT_FORMAT,srt_ent->rule_nr,
				 srt_ent->par_rule_nr,
				 srt_ent->no_nam_attr);

	/* write naming attributes */
	fprintf(fp,"Nam-attrs: ");

	for (j=0;j<srt_ent->no_nam_attr;j++)
		fprintf(fp,"%s ",srt_ent->nam_attr[j]);

	/* write object class */
	fprintf(fp,"Obj_Class: ");

	fprintf(fp,"%s ",srt_ent->obj_class);

	fprintf(fp,"\n");
	}

}		/* end of d21_store_scheme()		*/
