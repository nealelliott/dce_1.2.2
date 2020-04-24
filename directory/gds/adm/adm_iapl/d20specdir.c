/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20specdir.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:00  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:12:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:12  root]
 * 
 * Revision 1.1.8.3  1994/08/01  15:31:43  marrek
 * 	Bug fixes for OT 11295, 11456, 11458, 11471.
 * 	[1994/08/01  15:28:56  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:11  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:03:02  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:40  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:16:24  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:52:44  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:11:56  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:46:15  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:59:36  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:41:15  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:36:17  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:35:47  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:09:00  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20specdir.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:03 $";
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
/* NAME         : d20specdir.c                                        */
/*								      */
/* AUTHOR       : Volpers, D AP 11                                    */
/* DATE         : 10.05.88                                            */
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
/* SYSTEMABHAENIGKEIT:                                                */
/*                      COMMON                                        */
/*								      */
/* HISTORIE	:						      */
/*								      */
/* Vers.Nr. |  Datum  |	 Aenderungen		       | KZ | CR# FM# */
/*      0.1 | 10.05.88| Erstellung                     | ws |         */
/*								      */
/*datoff **************************************************************/

/**********************************************************************/
/*								      */
/*		       Modulidentifikation			      */
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

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d2info.h>
#include <d21iapl.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		   T Y P  -  D E F I N I T I O N E N		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		   D E K L A R A T I O N E N	  DATEN		      */
/*								      */
/**********************************************************************/
extern D20_omask d20_qomask[];
extern char      *d20_errmess[];

extern Ds_v2_info  info ;	/* common IAPL-interface parameter block */

/**********************************************************************/
/*								      */
/*		  D E K	L A R A	T I O N	E N	 FUNKTIONEN	      */
/*								      */
/**********************************************************************/


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNKTION                                            */
/*								      */
/* NAME       : d20_spec_call_dir                                     */
/*								      */
/* AUTHOR:      Volpers (D AP 11)                                     */
/* DATE:        10.05.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_spec_call_dir(p_env, dsa_typ)            */
/*                                                                    */
/*              char     *p_env;                                      */
/*              signed16  dsa_typ;                                    */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will call a cache update to enter       */
/*              a DSA as DEFAULT- or LOCAL-DSA to the cache           */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              char     *p_env;                                      */
/*              signed16  dsa_typ;                                    */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       No error occurred                             */
/*      -1      :       Error occurred                                */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* BEMERKUNGEN:							      */
/*								      */
/*inoff ***************************************************************/

 signed16 d20_spec_call_dir(
   char     *p_env,
   signed16  dsa_typ)
 
{                               /*  d20_spec_call_dir                 */

/**********************************************************************/
/*								      */
/*		   T Y P  -  D E F I N I T I O N E N		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

   signed16 ret_value = D2_NO_ERR;/* Return-Value                    */
   signed16 ret_value__ = D2_NO_ERR; /* Default_local return value */
   d2_ret_val ret;
   signed16 i;
   D2_ei_info    *reseinfo;
   D2_a_info     *resinfo;
   D2_a_value    *resvalue;
   D2_a_info     *atinfo;
   D2_a_value    *atvalue;
   char          val[D2_DNL_MAX+3];
   char          ent_normed[D2_DNL_MAX+1];
   Octet_string   aval;
   char   dsaname[D27_LFILE_NAME] ;  /* dsaname file                  */
   FILE   *fdcm ;             /*  file descriptor of dsaname file */
   signed16 nam_len;
   signed16 act_len;
   boolean	do_add;

static D2_pbhead head = { D21V022 } ;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if                           /* dsa_typ = local                     */
  (dsa_typ == D20_2_DSA_TYP || dsa_typ == D20_3_DSA_TYP)
{                            /* write dsaname file                  */
  sprintf (dsaname, "%s%s%s%s%d%s", dcelocal_path, D27_SERVER_DIR, D27_DSA_DIR,
				  D27_SUBDSA_DIR, info.dir_id, D27_DSANAME) ;
  if ((fdcm = fopen (dsaname, "w")) == (FILE *) NULL)
  {   
     return(D2_ERROR);
  } 
  nam_len = (signed16) (strlen((char *)(info.entry+1)) + 1);
  strcpy(ent_normed, (char *)(info.entry+1));
  d27_012_remove_blanks(ent_normed, nam_len, ent_normed);

  /* look for Principal Name in the attribute info.	*/
  atinfo = info.attrinfo;
  while (atinfo != (D2_a_info *)NULL)
	if (d20_cmp_objid(&(atinfo->d2_a_type), sizeof(D2_PR_NM),
			(Ob_id_string) D2_PR_NM))
		break;
	else
		atinfo = atinfo->d2_a_next;

  if (atinfo == (D2_a_info *)NULL)
	fprintf(fdcm,"%s",ent_normed);
  else
	{
	fprintf(fdcm,"%s\n",ent_normed);
	fprintf(fdcm,"%.*s", atinfo->d2_a_val->d2_a_v_len, 
			     atinfo->d2_a_val->d2_at_value.d2_prt_a_v);
	}

  fclose (fdcm) ;

  /* d27_007_norm_name(ent_normed, nam_len, ent_normed); */
} 
if                              /* dsa typ is client                  */
  (dsa_typ == D20_4_DSA_TYP)
{                               /* fill info structure for add object */
  atinfo = info.attrinfo;
  strcpy((char *)(info.entry + 1), D2_N_CLIENT);
  if                            /* no PSAP Address is entered         */
    ((atinfo->d2_a_type.d2_typ_len != (sizeof(D2_PSAP_ADDRESS) - 1)) || 
     (strncmp(D2_PSAP_ADDRESS, (char *)atinfo->d2_a_type.d2_type,
					 (sizeof(D2_PSAP_ADDRESS)- 1))))
  {                             /* error message                      */
    return(D20_NO_PSAP);
  } 
  ret_value = ds4_b11_call_directory(DS_ADD_OBJECT);
}
else
{                                /* save DSA-Name                     */
				 /* for attribute D2_SEE_ALSO         */
  strcpy(val,(char *)(info.entry + 1));
  nam_len = (signed16) strlen(val) + 1;
  /* d27_007_norm_name(val, nam_len, val); */
  act_len = d27_012_remove_blanks(val, nam_len, val);

  if                              /* dsa typ is default                 */
    (dsa_typ == D20_1_DSA_TYP || dsa_typ == D20_3_DSA_TYP)
  {                               /* read default dsa from cache        */
				  /* fill info structure for read       */

					    /* set object to search for */
    strcpy((char *)(info.entry + 1), D2_N_DEF_DSA);
    d20_166_set_all_attr();           /* to default DSA for d20_a0_read */

    if ((ret = d20_a0_read(&head)) != D2_NOERROR)
    /* fills the "info" structure if default dsa object exists */
    {   
/* add object if does not exist (used only for f i r s t default DSA to add) */
      atinfo = info.attrinfo;
      atinfo->d2_a_next = NULL;
      atinfo->d2_a_type.d2_type = (Ob_id_string)D2_SEE_ALSO;
      atinfo->d2_a_type.d2_typ_len = sizeof(D2_SEE_ALSO) - 1;

      atinfo->d2_a_no_val = 1; 
      atinfo->d2_a_val = atvalue = info.avalinfo ;
	strcpy((char *)info.avalues,val);
        atvalue->d2_a_rep = 
		(unsigned16) d27_401_syntax_to_rep((signed16)D2_DISTNAME);
	atvalue->d2_a_v_len = strlen(val) + 1 ;
	atvalue->d2_at_value.d2_oct_a_v = info.avalues;
      do_add = TRUE;
    }
    else
    {
      reseinfo = info.resentry ;
      resinfo = reseinfo->d2_ei_ai ;
      resvalue = resinfo->d2_a_val ;
      /* add attributes: used for subsequent adds of default dsas to existing
         default dsa object */
      atinfo = info.attrinfo;
      atinfo->d2_a_val = atvalue = info.avalinfo ;
        atvalue->d2_a_rep = 
		(unsigned16) d27_401_syntax_to_rep((signed16)D2_DISTNAME);
	atvalue->d2_at_value.d2_oct_a_v = aval = info.avalues ;
      atinfo->d2_a_next = NULL;
      atinfo->d2_a_type.d2_type = (Ob_id_string)D2_SEE_ALSO;
      atinfo->d2_a_type.d2_typ_len = sizeof(D2_SEE_ALSO) - 1;

      atinfo->d2_a_no_val = resinfo->d2_a_no_val + 1;

      for (i = 0, do_add = TRUE; i < resinfo->d2_a_no_val; i++)
      { 
	  if (d20_dsacmp(val, (char *)(resvalue->d2_at_value.d2_oct_a_v))
				      == TRUE)
	  {     /* DSA already exists */
		/* need not to add it to SEE_ALSO list */
	    do_add = FALSE;
	    break;
	  }
	  strcpy((char *)(aval), (char *)(resvalue->d2_at_value.d2_oct_a_v));
          atvalue->d2_a_rep = 
		(unsigned16) d27_401_syntax_to_rep((signed16)D2_DISTNAME);
	  atvalue->d2_a_v_len = resvalue->d2_a_v_len ;
	  atvalue->d2_at_value.d2_oct_a_v = aval ;

	  aval = atvalue->d2_at_value.d2_oct_a_v + atvalue->d2_a_v_len ;
	  resvalue++;
	  atvalue++;
      } 

      if (do_add)	/* add new DSA name */
      {
        strcpy((char *)aval,val);
        atvalue->d2_a_rep = 
		(unsigned16) d27_401_syntax_to_rep((signed16)D2_DISTNAME);
        atvalue->d2_a_v_len = strlen(val) + 1 ;
        atvalue->d2_at_value.d2_oct_a_v = aval;
      }
    } 

    if (do_add)
    {
      strcpy((char *)(info.entry + 1), D2_N_DEF_DSA);
      ret_value__ = ds4_b11_call_directory(DS_ADD_OBJECT);
    }
  }

  if                              /* dsa typ is local                    */
    (dsa_typ == D20_2_DSA_TYP || dsa_typ == D20_3_DSA_TYP)
  {                               /* fill info structure for D2_SEE_ALSO */
				  /* attribute                           */
    atinfo = info.attrinfo;
    atinfo->d2_a_next = NULL;
    atinfo->d2_a_type.d2_type = (Ob_id_string)D2_SEE_ALSO;
    atinfo->d2_a_type.d2_typ_len = sizeof(D2_SEE_ALSO) - 1;

    atinfo->d2_a_no_val = 1;
    atinfo->d2_a_val = atvalue = info.avalinfo ;
      atvalue->d2_a_rep = 
		(unsigned16) d27_401_syntax_to_rep((signed16)D2_DISTNAME);
      atvalue->d2_a_v_len = strlen(ent_normed) + 1 ;
      strcpy((char *)info.avalues, ent_normed);
      atvalue->d2_at_value.d2_oct_a_v = info.avalues;
      aval = atvalue->d2_at_value.d2_oct_a_v + atvalue->d2_a_v_len ;

    strcpy((char *)(info.entry + 1), D2_N_LOC_DSA);
    ret_value = ds4_b11_call_directory(DS_ADD_OBJECT);
  } 
} 

if  ( ( ret_value == 0 ) && (ret_value__ != 0) )
     ret_value = ret_value__;     /* make sure all adds were OK */
return(ret_value);

}                               /*  d20_spec_call_dir                 */


/**********************************************************************/
/*								      */
/* TYPE:        C-FUNKTION                                            */
/*								      */
/* NAME       : d20_delete_default_dsa                                */
/*								      */
/* AUTHOR:      Eros (CDS - Barcelona)                                */
/* DATE:        11.2.88                                               */
/*								      */
/* SYNTAX:      signed16 d20_delete_default_dsa(void)                 */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function deletes a default dsa from the cache    */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       No error occurred                             */
/*      -1      :       Error occurred                                */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* BEMERKUNGEN:							      */
/*								      */
/**********************************************************************/

 signed16 d20_delete_default_dsa(void)
{
   d2_ret_val    ret_value = D2_NOERROR;
   signed16 i;
   D2_ei_info    *reseinfo;
   D2_a_info     *resinfo;
   D2_a_value    *resvalue;
   D2_a_info     *atinfo;
   D2_a_value    *atvalue;
   char          val[D2_DNL_MAX+3];
   Octet_string   aval;
   Bool   found;
   signed16  nam_len;

static D2_pbhead head = { D21V022 } ;

/* 1] is D2_N_DEF_DSA extant?  (is there any default DSA at all?) */

				/* store DEFAULT-DSA                  */
     strcpy(val, (char *)(info.entry + 1));
     nam_len = (signed16) strlen(val) + 1;
     /* d27_007_norm_name(val, nam_len, val); */
     d27_012_remove_blanks(val, nam_len, val);

     strcpy((char *)(info.entry + 1), D2_N_DEF_DSA);
     d20_166_set_all_attr();
     if ((ret_value = d20_a0_read(&head)) != D2_NOERROR) /* get "info" stuff */
     {    d20_qomask[D20_Q1POS].d20_ioinfo =
				d20_errmess[map_errnum(D2_NFOUND)];
	  d20_handlemask (d20_qomask);
          return(-1);
     }

/* 2] to check if the DSA name the user selected is extant(i.e. D2_SEE_ALSO):*/
/* 2a] get the existing default DSA list from "info" structure.... */

     reseinfo = info.resentry;
     resinfo = reseinfo->d2_ei_ai;
     resvalue = resinfo->d2_a_val;
     atinfo = info.attrinfo;
     atinfo->d2_a_val = atvalue = info.avalinfo;
     atinfo->d2_a_next = NULL;
     atinfo->d2_a_type.d2_type = (Ob_id_string)D2_SEE_ALSO;
     atinfo->d2_a_type.d2_typ_len = sizeof(D2_SEE_ALSO) - 1;

     atvalue->d2_at_value.d2_oct_a_v = aval = info.avalues;
/* 2b] ... and see if any of the entries match the one the user entered */

     found = FALSE;
     atinfo->d2_a_no_val = resinfo->d2_a_no_val; 
     for (i=0; i < resinfo->d2_a_no_val; i++)
     {  if(d20_dsacmp((char *)(resvalue->d2_at_value.d2_oct_a_v), val) == TRUE)
	{   found = TRUE; /* the item is not copied to new list */
	    atinfo->d2_a_no_val--;
	}
	else
	{   /* copy the item to the new list */
	    strcpy((char *)aval, (char *)resvalue->d2_at_value.d2_oct_a_v);
      	    atvalue->d2_a_rep = 
		(unsigned16) d27_401_syntax_to_rep((signed16)D2_DISTNAME);
	    atvalue->d2_a_v_len = resvalue->d2_a_v_len;
	    atvalue->d2_at_value.d2_oct_a_v = aval;
	    aval = atvalue->d2_at_value.d2_oct_a_v + atvalue->d2_a_v_len;
	    atvalue++;
	}
	resvalue++;
     }

     if (!found)
     {    d20_qomask[D20_Q1POS].d20_ioinfo =
				d20_errmess[map_errnum(D2_NFOUND)];
	  d20_handlemask (d20_qomask);
          return(-1);
     }

/* 4] if no default dsa is now present, delete the whole object,
      otherwise re-write the default dsa list to the cache */

     if (atinfo->d2_a_no_val == 0)
     {    ret_value = ds4_b11_call_directory(DS_DELETE_OBJECT);
     }
     else
     {    ret_value = ds4_b11_call_directory(DS_ADD_OBJECT);
     }

     return(ret_value);
}
