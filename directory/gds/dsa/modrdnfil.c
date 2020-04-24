/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: modrdnfil.c,v $
 * Revision 1.1.10.2  1996/02/18  18:15:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:02  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:50:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:40  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:46:16  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:37:00  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:53:11  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:27:56  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:13:52  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:25:33  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:37:29  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:24:26  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  15:56:03  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:26:29  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  14:58:27  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.4  1993/02/02  11:34:06  marrek
 * 	check whether new naming value already exists
 * 	remove SNI specific macros
 * 	[1993/02/01  15:38:55  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:44:06  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:22:22  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:33:29  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:23:49  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  02:52:00  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:32:38  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char modrdnfil_rcsid[] = "@(#)$RCSfile: modrdnfil.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:15:28 $";
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
/*                                                                    */
/* NAME         : modrdnfil.c                                         */
/*                                                                    */
/* AUTHOR       : Schreck, D AP 11                                    */
/* DATE         : 06.04.88                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : DS-Design-Spec.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : This module contains the functions used for         */
/*                DS_REPLACE_RDN.                                     */
/*                                                                    */
/* SYSTEM DEPENDENCIES: SINIX (--> C-ISAM)                            */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*   0.1    |06.04.88 |  Original                      | AS |         */
/*          |         |                                |    |         */
/*                                                                    */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module-identification                          */
/*                                                                    */
/**********************************************************************/



/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  external Includes    *****/

#include <dce/d27util.h>
#include <malgcfe.h>

/*****  internal Includes    *****/

#include <d26schema.h>
#include <d26isam.h>
#include <d26svcis.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_B03_change_main_entry                             */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        06.04.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_B03_change_main_entry (dn_info,      */
/*                                   last_rdn, del_old_rdn, obj_class,*/
/*                                   obj_idx, alias_flag, record,     */
/*                                   error, l_err)                    */
/*                                                                    */
/*              D26_dninfo      *dn_info;            ->               */
/*              Short           last_rdn;            ->               */
/*              Bool            del_old_rdn;         ->               */
/*              Long            obj_class;           ->               */
/*              Short           obj_idx;             ->               */
/*              Bool            alias_flag;          ->               */
/*              Char            *record;             <>               */
/*              Octet           **error;             <>               */
/*              Int             *l_err;              <>               */
/*                                                                    */
/* DESCRIPTION: This function changes the value of the last rdn in the*/
/*              main entry and updates the record in the database.    */
/*                                                                    */
/*              When errors occurs an error apdu and the length of    */
/*              error apdu is returned and the caller cancels the     */
/*              function.                                             */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      D26_dninfo        *dn_info          distinguished name infor- */
/*                                          mation of new entryname   */
/*                                                                    */
/*      Short             last_rdn          index of last rdn in      */
/*                                          'dn_info'                 */
/*                                                                    */
/*      Bool              del_old_rdn       flag indicating whether   */
/*                                          old RDN value should      */
/*                                          remained                  */
/*                            D2_TRUE       delete old value          */
/*                            D2_FALSE      remain old value (the     */
/*                                          does not belong to the    */
/*                                          namepart value)           */
/*                                                                    */
/*      Long              obj_class         index in OCT of entry     */
/*                                                                    */
/*      Short             obj_idx           index of entry in SRT     */
/*                                                                    */
/*      Bool              alias_flag        flag indicating whether   */
/*                                          entry is an alias object  */
/*                                                                    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      Octet            **error;           error message (contains   */
/*                                          the wrong attribute and/or*/
/*                                          return codes)             */
/*                                                                    */
/*      Int              *l_err;            length of error message   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      Short   D2_NO_ERR     no error occurred (main entry was       */
/*                            changed successfully)                   */
/*                                                                    */
/*              D26_ERROR     modification of main entry fails        */
/*                            (error apdu and l_err are supplied)     */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*     D26_srt       *d26_srt          pointer to SRT                 */
/*     D26_oct       *d26_oct          pointer to OCT                 */
/*     D26_at        *d26_at           pointer to AT                  */
/*     D26_fildes    *d26_ofildes      description for object files   */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        d26_C09_init_val                                            */
/*        d26_C11_copy_vals                                           */
/*        d26_u97_search_attr_val                                     */
/*                                                                    */
/*        d26_i15_get_record_pos                                      */
/*        d26_i16_insert_val                                          */
/*        d26_i59_get_valmemsize                                      */
/*                                                                    */
/*        d27_010_set_error_apdu                                      */
/*        isrewrite                                                   */
/*                                                                    */
/*        sprintf                                                     */
/*        memcpy                                                      */
/*                                                                    */
/* NOTES: Note the following restriction for the naming attributes:   */
/*             The naming attribute can be a single valued attribute  */
/*             or a multiple valued attribute with a limited number   */
/*             of values.                                             */
/*             The naming attribute cannot be a multiple valued       */
/*             attribute with a unlimited number of values.           */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_B03_change_main_entry
(
  D26_dninfo   *dn_info,
  signed16      last_rdn,
  boolean       del_old_rdn,
  signed32      obj_class,
  signed16      obj_idx,
  boolean       alias_flag,
  char         *record,
  byte         **error,
  signed32      *l_err
)

{                               /*  d26_B03_change_main_entry         */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* STATIC    ...    */
static char function[] = "d26_B03_change_main_entry";

/* AUTO      ...    */
 signed16         ret_val;      /* return value of a called function   */
 signed16         oct_idx;      /* index to OCT to determine           */
				/* file_nr for alias object            */
 D26_ocl_arc      *ocl_arc;     /* pointer to arc of object classes    */
 signed16         file_nr;      /* file number of changed object       */
 int              isfd;         /* C-isam filedescriptor of file of    */
				/* changed object                      */
 D26_at           *p_at_entry;  /* pointer to at entry of attr.        */
 D26_oct          *p_oct_entry; /* pointer to oct entry of object      */
 D26_nam_at       *p_nam_at;    /* pointer to naming attr. descr.      */
 D26_ava	  *p_ava;	/* pointer to ava in rdn               */
 D26_srt          *p_srt_entry; /* pointer to SRT entry of object      */
 char             *pos;         /* pointer to actual record position   */
 char             *anz_pos;     /* ptr to record position of no of vals*/
 char             *tmp_pos;     /* pointer to record position          */
 signed32         max_len;      /* total size of value in record       */
 signed16         anz_vals;     /* number of values of an attribute    */
/* additional variables for d26_u97_search_attr_val */
 boolean          found;
 signed16         val_no;
 signed16         norm_len;
 char             norm_val[D2_NP_LEN+1];
 unsigned16       rep;         /* value representation                */

/* FASTAUTO  ...    */
register D26_rdninfo   *p_rdninfo;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/


/************************/
/* initialize variables */
/************************/
/* determine filenumber */
p_oct_entry  = &d26_oct[obj_class];
if (alias_flag == TRUE)
{   
  /* filenr for alias is first ocl in ocl_arc of SRT */
      ocl_arc = (d26_srt + obj_idx)->d26s_ocl_arc;
      oct_idx = ocl_arc->d26s_oclass;
      p_oct_entry  = d26_oct + oct_idx;
      file_nr = p_oct_entry->d26c_filenr;
} else {
      file_nr = p_oct_entry->d26c_filenr;
}  /* alias_flag == D2_TRUE */

/* last rdn info */
p_rdninfo    = &dn_info->d26_rdn[last_rdn];
/* SRT entry and AT entry */
p_srt_entry  = &d26_srt[p_rdninfo->d26_r_idx];

/* change record index */
if ( obj_idx != p_rdninfo->d26_r_idx )
{   
      stint(p_rdninfo->d26_r_idx, record + LONGSIZE);
} 

for (p_nam_at = p_srt_entry->d26s_nam_at, p_ava = p_rdninfo->d26_r_ava; 
     p_nam_at->d26s_index != -1; p_nam_at++, p_ava++)
{ 
    p_at_entry = d26_at + p_ava->d26_a_idx;
    /* position of namepart value in record */
    pos = d26_i15_get_record_pos (record, 1, p_at_entry, p_oct_entry,
			          p_nam_at, FALSE);
    /* get number of values */
    anz_pos = d26_i15_get_record_pos (record, 1, p_at_entry, p_oct_entry,
				      p_nam_at, TRUE);
    anz_vals = ldint (anz_pos);

/********************************************************/
/* check whether new value exists as normal attr. value */
/********************************************************/
/* norm namepart value */
    norm_len = (signed16) p_ava->d26_a_len;
    d27_651_norm_T61String(p_ava->d26_a_val,(byte *)norm_val,norm_len);

    found = d26_u97_search_attr_val (record, p_at_entry, p_oct_entry,
				     p_nam_at, norm_len, norm_val, anz_vals,
				     (D2_name_string) NULL, &val_no, 
                                     &tmp_pos);
    if (found == TRUE)
    {    /* value already exists as normal attribute value */
     /* delete the normal attribute value              */
     if (val_no > 1)  /* value not naming one */
     {
         anz_vals = anz_vals - 1;
         stint (anz_vals, anz_pos);
         if (val_no > anz_vals)
         {    /* initialize value                       */
	      tmp_pos = d26_i15_get_record_pos (record, val_no, p_at_entry,
				    p_oct_entry, p_nam_at, FALSE);
	      d26_C09_init_val (&tmp_pos, p_at_entry);
         } else { /* copy the following values                  */
	      d26_C11_copy_vals (record, p_at_entry, p_oct_entry,
			         p_nam_at, val_no+1, anz_vals+1);
         } 
      }
    }  /* (found == D2_TRUE) */

/***************************************/
/* remain old namepart value if wished */
/***************************************/
    if (del_old_rdn == FALSE && !found)
    {    /* remain old namepart value */
         /* store new number of values */
         anz_vals = anz_vals + 1;
         stint (anz_vals, anz_pos);
         /* check whether maximum number of values is exceeded */
         if (anz_vals > p_at_entry->d26a_nr_recur)
         {    /* ERROR: max. number of values exceeded */
	      d27_010_set_error_apdu ((D23_rserror *) *error, D2_CALL_ERR,
				      D2_PAR_ERR, D2_NAMING_VIOLATION, l_err);

	      return (D26_ERROR);
         }  /* max. no. of values exceeded */
         /* copy old value to the end of list of values */
         tmp_pos = d26_i15_get_record_pos (record, anz_vals, p_at_entry,
				           p_oct_entry, p_nam_at, FALSE);
         max_len = d26_i59_get_valmemsize (p_at_entry);
         memcpy (tmp_pos, pos, (size_t) max_len);
    }  /* (del_old_rdn == D2_FALSE) */

/*************************************/
/* insert new naming value in record */
/*************************************/
  if (val_no != 1) /* old naming value != new one */
  {
    /* Get representation of name part */
    switch(*(p_ava->d26_a_val-1))
    {
      case D2_T61_REP:    rep = D2_T61;       break;
      case D2_PRTBL_REP:  rep = D2_PRINTABLE; break;
      case D2_NUM_REP:    rep = D2_NUMERIC;   break;
      case D2_IA5_REP:    rep = D2_IA5;       break;
      default:
	   d27_010_set_error_apdu ((D23_rserror *) *error, D2_CALL_ERR,
				   D2_PAR_ERR, D2_NAMING_VIOLATION, l_err);

	   return(D26_ERROR);
    }

    ret_val = d26_i16_insert_val (p_at_entry, rep, (char *) p_ava->d26_a_val,
			          p_ava->d26_a_len,
			          (D26_dns **) NULL, (D26_dns **) NULL,
			          &pos);

    if (ret_val != D2_NO_ERR) /* insert_val fails                 */
    {               /* C-isam error occurred                      */
         d27_010_set_error_apdu ((D23_rserror *) *error, D2_TEMP_ERR, 
                                    D2_REM_ERR, D2_NO_RESPONSE, l_err);
		    /* exit function                              */
         return (D26_ERROR);
    }               /* insert_val fails */
  }
} 	/* for all AVAs */

/******************/
/* rewrite record */
/******************/
                             /*  get filedescriptor for C-isam rewrite     */
isfd    = (d26_ofildes + file_nr)->d26_isfd;
                             /* */

if                      /*  rewrite fails                             */
   (D26_ISREWRITE(svc_c_sev_warning, isfd, record) < 0)
{                       
			/*  exit function                             */
     return (D26_ERROR);
}                       /*  rewrite fails */

D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_WRITE,function,1,0);
return (D2_NO_ERR);          /*  */
}                               /*  d26_B03_change_main_entry         */


