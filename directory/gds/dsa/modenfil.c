/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: modenfil.c,v $
 * Revision 1.1.12.2  1996/02/18  18:15:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:37:53  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:50:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:33  root]
 * 
 * Revision 1.1.10.3  1994/05/10  15:53:08  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:27:33  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  15:13:49  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:25:14  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  18:41:26  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:23:45  marrek]
 * 
 * Revision 1.1.8.1  1993/10/29  15:11:29  keutel
 * 	CR 9261
 * 	[1993/10/29  14:05:11  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  15:50:59  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:25:21  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:43:50  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:22:10  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:31:55  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:22:43  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  02:48:29  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:32:19  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char modenfil_rcsid[] = "@(#)$RCSfile: modenfil.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 18:15:22 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : modenfil.c  <DS_MODIFY_ENTRY>                       */
/*                                                                    */
/* AUTHOR       : Schreck, D AP 11                                    */
/* DATE         : 01.02.88                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : DS-Design-Spec.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*                                                                    */
/* DESCRIPTION  : This module contains the functions used for         */
/*                DS_MODIFY_ENTRY.                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES: SINIX (--> C-ISAM)                            */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*  0.1     |26.02.88 |  original                      | as |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
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

				    /* begin of new acl index values in     */
				    /* array                                */
/* D26_C07_NEW_ACL   0                 defined in d26dsa.h                  */
				    /* new masterknowledge index value in   */
				    /* array                                */
/* D26_C09_NEW_MK    2 * D2_ACLIDX_MAX  defined in d26dsa.h                 */

#define D26_C03_ERR_COMM_LEN    100 /* length of comment for exit or error  */






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

                                    /* exact name for error logging   */
extern byte       exact_name[D2_DNL_MAX+1];

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_C04_change_main_entry                             */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        26.02.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_C04_change_main_entry (n_entryname,  */
/*                                   obj_class, obj_idx, alias_flag,  */
/*                                   no_of_attrs, mod_attr, attr_info,*/
/*                                   dnl_ptr, record, upd_dnl_file,   */
/*                                   key, del_dns, no_del_dns,        */
/*                                   no_mem_del_dns, error, l_err)    */
/*                                                                    */
/*              D2_name_string  n_entryname;         ->               */
/*              Long            obj_class;           ->               */
/*              Short           obj_idx;             ->               */
/*              Bool            alias_flag;          ->               */
/*              Short           no_of_attrs;         ->               */
/*              D26_md_info     *mod_attr;           ->               */
/*              D26_attr_info   *attr_info;          ->               */
/*              D26_dns         *dnl_ptr[];          ->               */
/*              Char            *record;             <>               */
/*              Bool            *upd_dnl_file;       <>               */
/*              Long            *key;                <-               */
/*              Long            **del_dns;           <-               */
/*              Short           *no_del_dns;         <-               */
/*              Short           *no_mem_del_dns;     <-               */
/*              D23_rserror     *error;              <-               */
/*              Int             *l_err;              <-               */
/*                                                                    */
/* DESCRIPTION: This function changes the values in the main entry,   */
/*              and updates the record in the database.               */
/*                                                                    */
/*              This function checks whether the last value of an     */
/*              attribute is deleted or the maximum number of values  */
/*              of an attribute is exceeded. Both cases results in    */
/*              an error and the function is canceled.                */
/*                                                                    */
/*              When errors occurs an error apdu and the length of    */
/*              error apdu is returned and the caller cancels the     */
/*              function.                                             */
/*                                                                    */
/*              When the function succeeds the real recurring         */
/*              attributes are changed later on.                      */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      D2_name_string    n_entryname       normed entryname (is      */
/*                                          required when old acl     */
/*                                          value is given in         */
/*                                          modifications and must be */
/*                                          checked of equality)      */
/*                                                                    */
/*      Long              obj_class         object class index of     */
/*                                          entry (index in OCT)      */
/*                                                                    */
/*      Short             obj_idx           index of entry in SRT     */
/*                                                                    */
/*      Bool              alias_flag        flag indicating whether   */
/*                                          entry is an alias object  */
/*                                                                    */
/*      Short             no_of attrs       number of modifications   */
/*                                                                    */
/*      D26_md_info       *mod_attr         informations about the    */
/*                                          received modifications    */
/*                                                                    */
/*      D26_attr_info     *attr_info        attribute informations    */
/*                                          (indices of attributes    */
/*                                          in the AT and SRT)        */
/*                                                                    */
/*      D26_dns           *dnl_ptr[]        pointer list pointing to  */
/*                                          ACL shorts and master dsa */
/*                                          short                     */
/*                                                                    */
/*      Char              *record           INPUT OUTPUT parameter    */
/*                                          -> old values of the main */
/*                                             entry                  */
/*                                          <- new values of the main */
/*                                             entry (the record is   */
/*                                             already written into   */
/*                                             the database           */
/*                                                                    */
/*      Bool              *upd_dnl_file     INPUT OUTPUT parameter    */
/*                                          is set to D2_TRUE when an */
/*                                          attribute (value) with    */
/*                                          dist. name syntax is      */
/*                                          replaced or deleted to    */
/*                                          indicate that the dn list */
/*                                          must be updated           */
/*                                          (this flag can be D2_TRUE */
/*                                          before this function is   */
/*                                          called)                   */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      Long              *key              C-isam key of changed     */
/*                                          main entry                */
/*                                                                    */
/*      Long              **del_dns         indices to dn list in core*/
/*                                          which occurrencies must   */
/*                                          be decreased by -1 (in    */
/*                                          file)                     */
/*                                                                    */
/*      Short             *no_del_dns       number of values in       */
/*                                          'del_dns'                 */
/*                                                                    */
/*      Short             *no_mem_del_dns   available memory for 'no_ */
/*                                          mem_del_dns' in 'del_dns' */
/*                                                                    */
/*      Bool               mod_schema       indicates wether scheme o.*/
/*                                          has to be changed in chdb */
/*                                                                    */
/*      D23_rserror      *error;            error message (contains   */
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
/*     D26_oct       *d26_oct          pointer to OCT                 */
/*     D26_srt       *d26_srt          pointer to SRT                 */
/*     D26_at        *d26_at           pointer to AT                  */
/*     D26_fildes    *d26_ofildes      description for object files   */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        d26_C09_init_val                                            */
/*        d26_C10_search_val                                          */
/*        d26_C11_copy_vals                                           */
/*        d26_i20_append_dnlidx                                       */
/*                                                                    */
/*        d26_i15_get_record_pos                                      */
/*        d26_i16_insert_val                                          */
/*                                                                    */
/*        d26_u15_attr_error                                          */
/*        d27_010_set_error_apdu                                      */
/*        d27_301_oid_str                                             */
/*                                                                    */
/*        stint                                                       */
/*        ldint                                                       */
/*        ldlong                                                      */
/*        isrewrite                                                   */
/*                                                                    */
/*        sprintf                                                     */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_C04_change_main_entry
(
  D2_name_string  n_entryname,
  signed32            obj_class,
  signed16           obj_idx,
  boolean            alias_flag,
  signed16           no_of_attrs,
  D26_md_info     *mod_attr,
  D26_attr_info   *attr_info,
  D26_dns         *dnl_ptr[],
  char            *record,
  boolean            *upd_dnl_file,
  signed32            *key,
  signed32            **del_dns,
  signed16           *no_del_dns,
  signed16           *no_mem_del_dns,
  D23_rserror     *error,
  signed32        *l_err
)

{                               /*  d26_C04_change_main_entry         */

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
static char  function[] = "d26_C04_change_main_entry";

/* AUTO      ...    */
 signed16            ret_val;      /* return value of a called function   */
 signed16            oct_idx;      /* index to OCT to determine           */
				   /* file_nr for alias object            */
 signed16            file_nr;      /* file number of changed object       */
 int                 isfd;         /* C-isam filedescriptor of file of    */
				   /* changed object                      */
 signed16            anz_vals;     /* number of values of an attribute    */
 signed16            val_no;       /* value number of a value of an attr. */
			           /* which should be modified            */
 signed32             dnl_idx;     /* index value to dn list in core      */
 D26_at           *p_at_entry;  /* pointer to at entry of attr.        */
 D26_nam_at       *p_nam_at;    /* pointer to naming attr. description */
 D26_ocl_arc      *ocl_arc;     /* pointer to arc of object classes    */
 D26_oct          *p_oct_entry; /* pointer to oct entry of attr.       */
 char             *pos;         /* pointer to actual record position   */
 char             file_name[D27_LFILE_NAME];
 char             *val;         /* attribute value                     */
 signed16           length;       /* attribute value length              */
 boolean             no_old_value; /* indicator whether an old attr. value*/
				    /* is given                            */

 signed32              apdu_len = 0; /* temp. variable for d26_u15_ call    */
 D2_a_type        type;         /* temp. variable attribute type       */
 unsigned16        rep;         /* value representation                */
 char             *record_ptr;  /* pointer to record (new in case of   */
				    /* schema modification)                */
 char             *sch_rec;     /* record for schema object            */

/* FASTAUTO  ...    */
register signed16          i, j;            /* counter                        */
register boolean           data_changed;    /* indicates whether data of main */
					 /* entry are changed              */
register D26_md_info    *p_mod_attr;     /* pointer to 'mod_attr'          */
register D26_attr_info  *p_attr_info;    /* pointer to 'attr_info'         */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

/*****************/
/* set variables */
/*****************/
data_changed = FALSE;
p_oct_entry  = &d26_oct[obj_class];
if (alias_flag == TRUE)
{   
			       /*  determine smallest filenr for */
			       /*  Alias object and correct oct  */
			       /*  entry (ocl_arcs are sorted by */
			       /*  ascending filenumber)         */
      ocl_arc = (d26_srt + obj_idx)->d26s_ocl_arc;
      oct_idx = ocl_arc->d26s_oclass;
      p_oct_entry  = d26_oct + oct_idx;
      file_nr = p_oct_entry->d26c_filenr;
} else {
      file_nr = p_oct_entry->d26c_filenr;
}  /* alias_flag == D2_TRUE */

if (d26_requestor.d26_sm_flag == D26_SM_PENDING)
{ /* schema has to be changed in chdb directory */
  /* open file objects0 in directory chdb       */
  sprintf(file_name, "%s%s%s%d", d26_install, D26_CHDB_DIR, D26_OBJECTS,
	  file_nr);
  if                      /*  isopen fails                              */
     ((isfd = d26_i23_open_file(file_nr,D26_OBJ)) == D26_ERROR)
  {                       
			  /*  set error code                            */
       d27_010_set_error_apdu (error, D2_TEMP_ERR, D2_REM_ERR,
				D2_UNWILLING, l_err);
			  /*  exit function                             */
       DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 1));
       return (D26_ERROR);
  }                       /*  isopen fails */

  if ((sch_rec = (char *) D27_CALLOC(svc_c_sev_warning,d26_oreclen, 
       sizeof(char))) == NULL)
  {
			     /*  set error apdu and length                 */
     d27_010_set_error_apdu (error, D2_TEMP_ERR, D2_REM_ERR,
			      D2_UNWILLING, l_err);
     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 2));
     return (D26_ERROR);     /*  exit function                             */
  }

  /* read first record of objects0 */
  stlong(1L,sch_rec);
  if (D26_ISREAD(svc_c_sev_warning,isfd, sch_rec, ISGTEQ) < 0)
  {                       
			  /*  set error code                            */
       d27_010_set_error_apdu (error, D2_TEMP_ERR, D2_REM_ERR,
				D2_UNWILLING, l_err);
			  /*  exit function                             */
       DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 3));
       return (D26_ERROR);
  }                       /*  isread fails */
  record_ptr = sch_rec;
} /* if schema is modified */
else
{
  record_ptr = record;
  isfd       = (d26_ofildes + file_nr)->d26_isfd;
}

/*****************************************************/
/* do modifications in main entry for all attributes */
/*****************************************************/
for                          /*  all attribute modifications               */
    (i = 0; i < no_of_attrs; i++)
{                            /*  set needed variables                      */
   p_mod_attr   = &mod_attr[i];
   p_attr_info  = &attr_info[i];
   p_at_entry   = &d26_at[p_attr_info->d26_at_idx];
   if (p_attr_info->d26_srt_idx != D26_IDX_NOT_DEF)
   {   
	p_nam_at = d26_srt[p_attr_info->d26_srt_idx].d26s_nam_at
			+ p_attr_info->d26_nam_idx;
   }
else
{
	p_nam_at  = (D26_nam_at *) NULL;
   } 
   /* get position of number of attr. values */
   pos = d26_i15_get_record_pos (record_ptr, 1, p_at_entry, p_oct_entry,
				 p_nam_at, TRUE);

   switch ((signed32) (p_mod_attr->d26_m)) {  /*  attribute modification*/
   case D2_ADDATT:           /*  add attribute                             */
	                     /*  store one value into occurrency           */
	stint (1, pos);
	data_changed = TRUE;
	                     
	if                   /*  not real recurring attribute              */
	   (p_at_entry->d26a_filenr < 0)
	{                    /*  insert new attribute value                */
	     pos = d26_i15_get_record_pos (record_ptr, 1, p_at_entry, 
					   p_oct_entry, p_nam_at, FALSE);
	     length = p_mod_attr->d26_mnew->d23_avlen;
	     val = ((char *) (p_mod_attr->d26_mnew)) + sizeof (D23_av_value);
	     rep = (p_mod_attr->d26_mnew)->d23_avrep;
	     ret_val = d26_i16_insert_val (p_at_entry, rep, val, length,
					   dnl_ptr+D26_C07_NEW_ACL,
					   dnl_ptr+D26_C09_NEW_MK, &pos);

	     if (ret_val != D2_NO_ERR) /* insert_val fails                 */
	     {               /* C-isam error occurred                      */
			     /* set error codes                            */
		  d27_010_set_error_apdu (error, D2_TEMP_ERR, D2_REM_ERR,
					   D2_NO_RESPONSE, l_err);
			     /* exit function                              */
      		  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
			D26_ERROR - 4));
		  return (D26_ERROR);
	     }               /* insert_val fails */
	}                    /*  not real recurring attribute */
	break;

   case D2_DELATT:           /*  delete attribute                          */
	                     /*  get number of old values                  */
	anz_vals = ldint (pos);
			     /*  set occurency to D2_NULL                  */
	stint (0, pos);
	data_changed = TRUE;
	                     /* */
	if                   /*  not real recurring attribute              */
	   (p_at_entry->d26a_filenr < 0)
	{                    /*  delete all old values                     */
	     for             /* all old values                             */
		 (j = 1; j <= anz_vals; j++)
	     {               /* get record position                        */
		pos = d26_i15_get_record_pos (record_ptr, j, p_at_entry,
					p_oct_entry, p_nam_at, FALSE);
			     /* append to 'del_dns'                        */
		ret_val = d26_i20_append_dnlidx (p_at_entry, pos,
						 upd_dnl_file, del_dns,
						 no_del_dns, no_mem_del_dns,
						 &dnl_idx, error,
						 (signed32 *) l_err);
		if (ret_val != D2_NO_ERR) /* append dnl idx fails */
		{               /* i20 fails              */
				/* exit function                           */
      		     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
			D26_ERROR - 5));
		     return (D26_ERROR);
		}               /* append dnl_idx fails */

			     /* initialize values                          */
		d26_C09_init_val (&pos, p_at_entry);
	     }               /* all old values */
	}                    /*  not real recurring attribute */
	break;

   case D2_DEL_AV:           /*  delete attribute value                    */
	                     /*  get number of old values                  */
	anz_vals = ldint (pos);
	                     /* */

	if (p_mod_attr->d26_mold != (D23_av_value *) NULL)
	{   
	     length = p_mod_attr->d26_mold->d23_avlen;
	     val = ((char *) (p_mod_attr->d26_mold))
		    + sizeof (D23_av_value);
	     no_old_value = FALSE;
	}
else
{
	     length = 0;
	     val = (char *) NULL;
	     no_old_value = TRUE;
	} 

	anz_vals = anz_vals - 1;
	if                   /*  last value should be deleted              */
	   (anz_vals <= 0)
	{                    /*  attribute error                           */
			     /*  set attribute error                       */
	     type = p_mod_attr->d26_mtype;
	     d26_u15_attr_error ((byte **) &error, exact_name, 1,
				 D2_CONSTRAINT_VIOLATION, &type,
				 p_at_entry->d26a_rep,
				 p_mod_attr->d26_mold->d23_avrep, length,
				 (byte *) val, l_err, &apdu_len);
	     *l_err = apdu_len;

			     /*  exit function                             */
      	     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 6));
	     return (D26_ERROR);
	}                    /*  last value ... */

			     /*  store new occurrency                      */
	stint (anz_vals, pos);
	data_changed = TRUE;
	if                   /*  not real recurring attribute              */
	   (p_at_entry->d26a_filenr < 0)
	{                    /*  search old value                          */
	     ret_val = d26_C10_search_val (record_ptr, p_at_entry, p_oct_entry,
			     p_nam_at, length, val,
			     anz_vals+1, no_old_value, n_entryname,
			     upd_dnl_file, del_dns,
			     no_del_dns, no_mem_del_dns,
			     &val_no, &pos, error, l_err);
	     if (ret_val != D2_NO_ERR) /* old value not found */
	     {               /* error old value not found                  */
			     /* exit function                              */
      		  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 7));
		  return (D26_ERROR);
	     }               /* old value not found */

			     /*  delete old value                          */
	     if (val_no > anz_vals)
	     {               /* initialize old value                       */
		  pos = d26_i15_get_record_pos (record_ptr, val_no, p_at_entry,
					p_oct_entry, p_nam_at, FALSE);
		  d26_C09_init_val (&pos, p_at_entry);
	     } else {        /* copy the following values                  */
		  d26_C11_copy_vals (record_ptr, p_at_entry, p_oct_entry,
				     p_nam_at, val_no+1, anz_vals+1);
	     } 
	}                    /*  not real recurring attribute */
	break;

   case D2_ADD_AV:           /*  add attribute value                       */
	                     /*  get number of old values                  */
	anz_vals = ldint (pos);
			     /*  store new occurrency                      */
	anz_vals = anz_vals + 1;
	stint (anz_vals, pos);
	data_changed = TRUE;
	                     /* */

	if                   /*  not real recurring attribute              */
	   (p_at_entry->d26a_filenr < 0)
	{                    /*                                            */
	     length = p_mod_attr->d26_mnew->d23_avlen;
	     val = ((char *) (p_mod_attr->d26_mnew)) + sizeof (D23_av_value);
	     rep = (p_mod_attr->d26_mnew)->d23_avrep;
	     if              /*  maximum number of values is exceeded      */
		(anz_vals > p_at_entry->d26a_nr_recur)
	     {               /*  attribute error                           */
		  type = p_mod_attr->d26_mtype;
		  d26_u15_attr_error ((byte **) &error, exact_name, 1,
				 D2_CONSTRAINT_VIOLATION, &type,
				 p_at_entry->d26a_rep, rep,
				 length, (byte *) val, l_err,
				 &apdu_len);
		  *l_err = apdu_len;

			     /*  exit function                             */
      		  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 8));
		  return (D26_ERROR);
	     }               /*  max. no. ... */

			     /*  store new attribute value                 */
	     pos = d26_i15_get_record_pos (record_ptr, anz_vals, p_at_entry,
					   p_oct_entry, p_nam_at, FALSE);
	     ret_val = d26_i16_insert_val (p_at_entry, rep, val, length,
					   dnl_ptr+D26_C07_NEW_ACL,
					   dnl_ptr+D26_C09_NEW_MK, &pos);

	     if (ret_val != D2_NO_ERR) /* insert_val fails                 */
	     {               /* C-isam error occurred                      */
			     /* set error codes                            */
		  d27_010_set_error_apdu (error, D2_TEMP_ERR, D2_REM_ERR,
					   D2_NO_RESPONSE, l_err);
			     /* exit function                              */
      		  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 9));
		  return (D26_ERROR);
	     }               /* insert_val fails */
	}                    /*  not real recurring attribute */
	break;

   case D2_REP_AV:           /*  replace attribute value                   */
	if                   /*  not real recurring attribute              */
	   (p_at_entry->d26a_filenr < 0)
	{                    /*                                            */
	     data_changed = TRUE;

			     /*  get number of old values                  */
	     anz_vals = ldint (pos);
			     /*  search old value                          */
	     if (p_mod_attr->d26_mold != (D23_av_value *) NULL)
	     {   
		  length = p_mod_attr->d26_mold->d23_avlen;
		  val = ((char *) (p_mod_attr->d26_mold)) +
			 sizeof (D23_av_value);
		  no_old_value = FALSE;
	     }
else
{
		  length = 0;
		  val = (char *) NULL;
		  no_old_value = TRUE;
	     } 

	     ret_val = d26_C10_search_val (record_ptr, p_at_entry, p_oct_entry,
			     p_nam_at, length, val,
			     anz_vals, no_old_value, n_entryname,
			     upd_dnl_file, del_dns,
			     no_del_dns, no_mem_del_dns,
			     &val_no, &pos, error, l_err);
	     if (ret_val != D2_NO_ERR) /* old value not found */
	     {               /* error old value not found                  */
			     /* exit function                              */
      		  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
			D26_ERROR - 10));
		  return (D26_ERROR);
	     }               /* old value not found */

			     /*  replace old value                         */
	     length = p_mod_attr->d26_mnew->d23_avlen;
	     val = ((char *) (p_mod_attr->d26_mnew)) + sizeof (D23_av_value);
	     rep = (p_mod_attr->d26_mnew)->d23_avrep;
	     ret_val = d26_i16_insert_val (p_at_entry, rep, val, length,
					   dnl_ptr+D26_C07_NEW_ACL,
					   dnl_ptr+D26_C09_NEW_MK, &pos);

	     if (ret_val != D2_NO_ERR) /* insert_val fails                 */
	     {               /* C-isam error occurred                      */
			     /* set error codes                            */
		  d27_010_set_error_apdu (error, D2_TEMP_ERR, D2_REM_ERR,
					   D2_NO_RESPONSE, l_err);
			     /* exit function                              */
      		  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
			D26_ERROR - 11));
		  return (D26_ERROR);
	     }               /* insert_val fails */
	}                    /*  not real recurring attribute */
	break;

   default:                  /* error: unknown modification (already       */
			     /* checked --> never occurs !)                */
	type = p_mod_attr->d26_mtype;
	d26_u15_attr_error ((byte **) &error, exact_name, 1,
			    D2_CONSTRAINT_VIOLATION, &type,
			    p_at_entry->d26a_rep, p_at_entry->d26a_rep,
			    (signed16) NULL, (byte *) NULL, l_err,
			    &apdu_len);
	*l_err = apdu_len;

			     /*  exit function                             */
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 12));
	return (D26_ERROR);
   }                         /*  attribute modification */
}                            /*  all attr. modifications */

/******************/
/* rewrite record */
/******************/
*key = ldlong (record_ptr);  /*  get key for update of recurring attributes*/

if (data_changed == TRUE)
{
  if                      /*  rewrite fails                             */
     (D26_ISREWRITE(svc_c_sev_warning, isfd, record_ptr) < 0) 
  {                       
			  /*  set error code                            */
       d27_010_set_error_apdu (error, D2_TEMP_ERR, D2_REM_ERR,
				D2_UNWILLING, l_err);
			  /*  exit function                             */
       DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 13));
       return (D26_ERROR);
  }                       /*  rewrite fails */
}                         /*  data changed  */

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_NO_ERR));
D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_WRITE,function,1,0);
return (D2_NO_ERR);          /*  */
}                               /*  d26_C04_change_main_entry         */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_C09_init_val                                      */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        29.02.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Void d26_C09_init_val (pos, p_at_entry)        */
/*                                                                    */
/*              char      **pos;          <>                          */
/*              D26_at    *p_at_entry;    ->                          */
/*                                                                    */
/* DESCRIPTION: This function initializes attribute values according  */
/*              to their syntax. The function is usefull when         */
/*              attribute values stored at the main entry are changed.*/
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      char       **pos             INPUT OUTPUT parameter           */
/*                                   -> record position of value which*/
/*                                      should be initialized         */
/*                                   <- record position of next value */
/*                                                                    */
/*      D26_at     *p_at_entry       pointer to at entry of attribute */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE: Void                                                  */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*       D26_at    *d26_at      pointer to AT                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        d26_u02_fill_field                                          */
/*        d26_i59_get_valmemsize                                      */
/*        stint                                                       */
/*        stlong                                                      */
/*                                                                    */
/* NOTES: is also called by d26_B03_change_main_entry (replace RDN)   */
/*                                                                    */
/*exoff ***************************************************************/

void d26_C09_init_val (char  **pos, 
		       D26_at *p_at_entry
		      )

{                               /*  d26_C09_init_val                  */

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

/* AUTO      ...    */
 signed32  max_len;

/* FASTAUTO  ...    */


/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

max_len = d26_i59_get_valmemsize (p_at_entry);
d26_u02_fill_field (D2_EOS, 0, (char *) NULL, (signed16) max_len, *pos);
*pos += max_len;

return;


}                               /*  d26_C09_init_val                  */



/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_C10_search_val                                    */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        01.03.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_C10_search_val (record, p_at_entry,  */
/*                                   p_oct_entry, p_nam_at,           */
/*                                   length, value, anz_vals,         */
/*                                   no_value, n_entry,               */
/*                                   upd_dnl_file, del_dns,           */
/*                                   no_del_dns, no_mem_del_dns,      */
/*                                   val_no, pos,                     */
/*                                   error, l_err)                    */
/*                                                                    */
/*              char             *record;            ->               */
/*              D26_at           *p_at_entry;        ->               */
/*              D26_oct          *p_oct_entry;       ->               */
/*              D26_srt          *p_nam_at;          ->               */
/*              Ushort           length;             ->               */
/*              Char             *value;             ->               */
/*              Short            anz_vals;           ->               */
/*              Bool             no_value;           ->               */
/*              D2_name_string   n_entry;            ->               */
/*              Bool             *upd_dnl_file;      <>               */
/*              Long             **del_dns;          <>               */
/*              Short            *no_del_dns;        <>               */
/*              Short            *no_mem_del_dns;    <>               */
/*              Short            *val_no;            <-               */
/*              Char             **pos;              <-               */
/*              D23_rserror      *error;             <-               */
/*              Int              *l_err;             <-               */
/*                                                                    */
/* DESCRIPTION: This function searches the given value in the main    */
/*              entry and returns the number of the value and the     */
/*              position of the value in the record. If no value is   */
/*              is given ('no_value == D2_TRUE') then the 'last value */
/*              is returned'.                                         */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      Char          *record             main entry                  */
/*                                                                    */
/*      D26_at        *p_at_entry         pointer to AT-entry  of     */
/*                                        attribute                   */
/*                                                                    */
/*      D26_oct       *p_oct_entry        pointer to OCT-entry of     */
/*                                        object                      */
/*                                                                    */
/*      D26_nam_at    *p_nam_at           pointer to naming attr.     */
/*                                        description                 */
/*                                                                    */
/*      Ushort        length              attribute value length      */
/*                                                                    */
/*      Char          *value              attribute value             */
/*                                                                    */
/*      Short         anz_vals            number of values of         */
/*                                        the given attribute in      */
/*                                        main entry                  */
/*                                                                    */
/*      Bool          no_value            indicates whether an        */
/*                                        attribute value is given    */
/*                             D2_TRUE    no attr. value is given     */
/*                             D2_FALSE   an attr. value is given     */
/*                                        the last value is 'returned'*/
/*                                                                    */
/*      D2_name_string   n_entry          normed entryname            */
/*                                                                    */
/*      Bool          *upd_dnl_file       INPUT OUTPUT parameter      */
/*                                        is set to D2_TRUE when an   */
/*                                        attribute (value) with      */
/*                                        dist. name syntax is        */
/*                                        replaced or deleted to      */
/*                                        indicate that the dn list   */
/*                                        must be updated             */
/*                                        (this flag can be D2_TRUE   */
/*                                        before this function is     */
/*                                        called)                     */
/*                                                                    */
/*      Long          **del_dns           INPUT OUTPUT parameter      */
/*                                        indices to dn list in core  */
/*                                        which occurrencies must     */
/*                                        be decreased by -1 (in      */
/*                                        file)                       */
/*                                                                    */
/*      Short         *no_del_dns         INPUT OUTPUT parameter      */
/*                                        number of values in         */
/*                                        'del_dns'                   */
/*                                                                    */
/*      Short         *no_mem_del_dns     INPUT OUTPUT parameter      */
/*                                        available memory for 'no_   */
/*                                        mem_del_dns' in 'del_dns'   */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      Short         *val_no             value number of found value */
/*                                                                    */
/*      Char          **pos               position of value in        */
/*                                        record                      */
/*                                                                    */
/*      D23_rserror   *error;             error message (contains     */
/*                                        the wrong attribute and/or  */
/*                                        return codes)               */
/*                                                                    */
/*      Int           *l_err;             length of error message     */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      Short   D2_NO_ERR     no error occurred (value was found)     */
/*                                                                    */
/*              D26_ERROR     value wasn't found or dnl idx couldn't  */
/*                            appended to list of 'del_dns'           */
/*                            (error apdu and l_err are supplied)     */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*     D26_at       *d26_at        pointer to AT                      */
/*     D26_oct      *d26_oct       pointer to OCT                     */
/*     D26_srt      *d26_srt       pointer to SRT                     */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        d26_i20_append_dnlidx                                       */
/*                                                                    */
/*        d26_u15_attr_error                                          */
/*        d26_u97_search_attr_val                                     */
/*        d26_i15_get_record_pos                                      */
/*        d27_301_oid_str                                             */
/*                                                                    */
/*        sprintf                                                     */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/


signed16 d26_C10_search_val 
(
  char             *record,
  D26_at           *p_at_entry,
  D26_oct          *p_oct_entry,
  D26_nam_at       *p_nam_at,
  signed16          length,
  char             *value,
  signed16          anz_vals,
  boolean           no_value,
  D2_name_string    n_entry,
  boolean          *upd_dnl_file,
  signed32        **del_dns,
  signed16         *no_del_dns,
  signed16         *no_mem_del_dns,
  signed16         *val_no,
  char            **pos,
  D23_rserror      *error,
  signed32         *l_err
)

{                               /*  d26_C10_search_val                */

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

/* AUTO      ...    */
 boolean       found = FALSE;
 signed16      ret_val;         /* return value of a called function   */
 signed32      dnl_idx;
 signed32      apdu_len = 0;    /* temp. variable for d26_u15_ call    */
 D2_a_type     type;            /* temp. variable attribute type       */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/************************/
/* initialize variables */
/************************/
*val_no = 0;
*pos = (char *) NULL;

/****************/
/* search value */
/****************/

if                           /*  no attr. value is  given                  */
   (no_value == TRUE)
{                            /*  return last value                         */
     if (anz_vals > 0)
     {   
	  found = TRUE;
	  *val_no = anz_vals;
	  *pos = d26_i15_get_record_pos (record, anz_vals, p_at_entry,
					 p_oct_entry, p_nam_at, FALSE);
     } 
} else {                     /*  search given value                        */
     found = d26_u97_search_attr_val (record, p_at_entry, p_oct_entry,
				      p_nam_at, length, value, anz_vals,
				      n_entry, val_no, pos);
}                            /*  no attr. value is given */

/*********************************/
/* check whether value was found */
/*********************************/

if                           /*  value was found                           */
   (found == TRUE)
{                            /*                                            */
			     /*  append to 'del_dns'                       */
     ret_val = d26_i20_append_dnlidx (p_at_entry, *pos,
				      upd_dnl_file, del_dns,
				      no_del_dns, no_mem_del_dns,
				      &dnl_idx, error, (signed32 *) l_err);
     if (ret_val != D2_NO_ERR) /* append dnl idx fails */
     {               /* i20 fails              */
	  *val_no = 0;
	  *pos = (char *) NULL;
		     /* exit function                           */
	  return (D26_ERROR);
     }               /* append dnl_idx fails */

} else {                     /*  attribute error: value not found          */
     *val_no = 0;
     *pos = (char *) NULL;
     type = p_at_entry->d26a_type;
     d26_u15_attr_error ((byte **) &error, n_entry, 1,
			 D2_ATTRIBUTE_MISSING, &type,
			 p_at_entry->d26a_rep, p_at_entry->d26a_rep,
			 length, (byte *) value, l_err,
			 &apdu_len);
     *l_err = apdu_len;

     return (D26_ERROR);     /*  exit function                             */
}                            /*  value found */

return (D2_NO_ERR);          /*  function exit                             */
}                               /*  d26_C10_search_val                */




/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_C11_copy_vals                                     */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        01.03.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Void d26_C11_copy_vals (record, p_at_entry,    */
/*                                             p_oct_entry,           */
/*                                             p_nam_at,              */
/*                                             from_val, upto_val)    */
/*                                                                    */
/*              Char        *record;       ->                         */
/*              D26_at      *p_at_entry;   ->                         */
/*              D26_oct     *p_oct_entry;  ->                         */
/*              D26_nam_at  *p_nam_at;     ->                         */
/*              Short       from_val;      ->                         */
/*              Short       upto_val;      ->                         */
/*                                                                    */
/* DESCRIPTION: This function copies all values from 'from_val' up to */
/*              'upto_val' to the position of value 'from_val' - 1.   */
/*              The last value ('upto_val') is initialized.           */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      Char           *record          record of main entry          */
/*                                                                    */
/*      D26_at         *p_at_entry      pointer to AT  entry of attr. */
/*                                                                    */
/*      D26_oct        *p_oct_entry     pointer to OCT entry of obj.  */
/*                                                                    */
/*      D26_nam_at     *p_nam_at        pointer to naming attr. descr.*/
/*                                                                    */
/*      Short          from_val         first value which should      */
/*                                      be copied                     */
/*                                                                    */
/*      Short          upto_val         last value which should be    */
/*                                      copied                        */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:  Void                                                 */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*        D26_at      *d26_at     pointer to AT                       */
/*        D26_oct     *d26_oct    pointer to OCT                      */
/*        D26_srt     *d26_srt    pointer to SRT                      */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        d26_i15_get_record_pos                                      */
/*                                                                    */
/* NOTES: is also called by d26_B03_change_main_entry (replace RDN)   */
/*                                                                    */
/*exoff ***************************************************************/

void d26_C11_copy_vals 
(
  char          *record,
  D26_at        *p_at_entry,
  D26_oct       *p_oct_entry,
  D26_nam_at    *p_nam_at,
  signed16       from_val,
  signed16       upto_val
)

{                               /*  d26_C11_copy_vals                 */

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

/* AUTO      ...    */
 char      *init_pos;

/* FASTAUTO  ...    */
register signed16    i;
register char     *to_pos;
register char     *from_pos;
register signed16    copy_length;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

                             /*  get record positions and copy_length      */
to_pos   = d26_i15_get_record_pos (record, (from_val - 1), p_at_entry,
				   p_oct_entry, p_nam_at, FALSE);
from_pos = d26_i15_get_record_pos (record, (from_val), p_at_entry,
				   p_oct_entry, p_nam_at, FALSE);
copy_length = from_pos - to_pos;
                             /* */

for                          /*  all values                                */
    (i = from_val; i <= upto_val; i++)
{                            /*  copy value                                */
   memcpy ((Octet_string) to_pos,(Octet_string) from_pos,
		    copy_length);
			     /*  get next positions                        */
   to_pos   = from_pos;
   from_pos = from_pos + copy_length;
}                            /*  all values */

                             /*  initialize last value                     */
init_pos = to_pos;
d26_C09_init_val (&init_pos, p_at_entry);
                             /*  */

return;
}                               /*  d26_C11_copy_vals                 */





/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_C13_change_rec_attrs                              */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        03.03.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_C13_change_rec_attrs (obj_class, key,*/
/*                                   no_of_attrs, mod_attr, attr_info)*/
/*                                                                    */
/*              Long            obj_class;          ->                */
/*              Long            key;                ->                */
/*              Short           no_of_attrs;        ->                */
/*              D26_md_info     *mod_attr;          ->                */
/*              D26_attr_info   *attr_info;         ->                */
/*                                                                    */
/* DESCRIPTION: This function modifies the real recurring values.     */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      Long              obj_class         index of object in OCT    */
/*                                                                    */
/*      Long              key               C-isam key of changed     */
/*                                          main entry                */
/*                                                                    */
/*      Short             no_of attrs       number of modifications   */
/*                                                                    */
/*      D26_md_info       *mod_attr         informations about the    */
/*                                          received modifications    */
/*                                                                    */
/*      D26_attr_info     *attr_info        attribute informations    */
/*                                          (indices of attributes    */
/*                                          in the AT and SRT)        */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Short     D2_NO_ERR   no error occurred                        */
/*                                                                    */
/*               D26_ERROR   any error occurred                       */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*     D26_at        *d26_at           pointer to AT                  */
/*     D26_oct       *d26_oct          pointer to OCT                 */
/*     D26_srt       *d26_srt          pointer to SRT                 */
/*     String        d26_a_record      record for attr. value         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        MA2_002_ERROR                                               */
/*                                                                    */
/*        d26_i02_upd_file_dnlist                                     */
/*        d26_i15_get_record_pos                                      */
/*        d26_i16_insert_val                                          */
/*        d26_i17_del_all_rec_vals                                    */
/*        d26_i18_read_rec_val                                        */
/*        d26_i19_del_rec_val                                         */
/*        d26_i20_append_dnlidx                                       */
/*        d26_i23_open_file                                           */
/*                                                                    */
/*        sprintf                                                     */
/*        stint                                                       */
/*        stlong                                                      */
/*        iswrite                                                     */
/*        isrewcurr                                                   */
/*        isrelease                                                   */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/


 signed16 d26_C13_change_rec_attrs
(
  signed32            obj_class,
  signed32            key,
  signed16           no_of_attrs,
  D26_md_info     *mod_attr,
  D26_attr_info   *attr_info
)

{                               /*  d26_C13_change_rec_attrs          */

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
static String    function   = "d26_C13_change_rec_attrs";

/* AUTO      ...    */
				/* return value of this function       */
 signed16         return_value = D2_NO_ERR;
 signed16         ret_val;      /* return value of a called function   */
				/* C-ISAM File-Name                    */
 signed16         file_nr;      /* file number of changed object       */
 int              isfd;         /* C-isam filedescriptor of file of    */
				/* changed object                      */
 char             *record;      /* file record                         */
 signed32         dnl_idx;      /* index value to dn list in core      */
 D26_at           *p_at_entry;  /* pointer to at  entry of attr.       */
 D26_oct          *p_oct_entry; /* pointer to oct entry of object      */
 D26_nam_at       *p_nam_at;    /* pointer to naming attr. descr.      */
 char             *val_pos;     /* pointer to attr. value porition in  */
				/* file record                         */
 char             *pos;         /* pointer to actual record position   */
 char             *val;         /* attribute value                     */
 char             *val_old;     /* attribute value                     */
 char             *val_new;     /* attribute value                     */
 signed16         length;       /* attribute value length              */
 boolean          no_old_value; /* indicator whether an old attr. value*/
				/* is given                            */
 unsigned16        rep;         /* value representation                */

/* FASTAUTO  ...    */
register signed16       i;               /* counter                        */
register D26_md_info    *p_mod_attr;     /* pointer to 'mod_attr'          */
register D26_attr_info  *p_attr_info;    /* pointer to 'attr_info'         */
register signed16       obj_filenr;      /* number of object file          */
register short          iw_count = 0;    /* number of iswrite-calls        */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/*****************/
/* set variables */
/*****************/
p_oct_entry  = &d26_oct[obj_class];
obj_filenr   = p_oct_entry->d26c_filenr;
p_nam_at     = (D26_nam_at *) NULL;

for                          /*  all attribute modifications               */
    (i = 0; i < no_of_attrs; i++)
{                            /*                                            */
   p_mod_attr   = &mod_attr[i];
   p_attr_info  = &attr_info[i];
   p_at_entry   = &d26_at[p_attr_info->d26_at_idx];

   if                        /*  real recurring attribute                  */
      (p_at_entry->d26a_filenr >= 0)
   {                         /*  open file                                 */
			     /* special handling for DNLIST-Attr and       */
			     /* master-info                                */
       if ((D2_ATTCMP (&(p_at_entry->d26a_type), D2_DNLIST) == 0)
	    && d26_ma_info == TRUE)
       {                     /* call update file dnlist                    */

	   val_old = ((char *) (p_mod_attr->d26_mold))+ sizeof (D23_av_value);
	   val_new = ((char *) (p_mod_attr->d26_mnew))+ sizeof (D23_av_value);

	   d26_i02_upd_file_dnlist (D2_REP_AV,
				   (D2_name_string) val_old,
				    D2_IGNORE,
				   (D2_name_string) val_new,
				    -1L, TRUE, &dnl_idx);
	   continue;
	} 

			     /* get C-ISAM file descriptor                 */
	file_nr = p_at_entry->d26a_filenr;
	isfd = d26_i23_open_file (file_nr, D26_ATT);
	if                   /* open file was ok                        */
	   (isfd < 0)
	{                    /* error from open file                    */
	   return_value = D26_ERROR;
	   continue;
	} else {
	     record = d26_a_record;
	     val_pos = d26_i15_get_record_pos (record, 1, p_at_entry,
					 p_oct_entry, p_nam_at, FALSE);
	     pos = val_pos;

	     switch ((signed32) (p_mod_attr->d26_m)) { /*  type of modif.*/
	     case D2_ADDATT:      /*  add attribute                        */
	     case D2_ADD_AV:      /*  add attribute value                  */
		                  /*  add record with new value            */
		                  /*  */
				  /* fill record                           */
		  stint (obj_filenr, record);
		  stlong (key, record + INTSIZE);
		  length = p_mod_attr->d26_mnew->d23_avlen;
		  val = ((char *) (p_mod_attr->d26_mnew)) +
			sizeof (D23_av_value);
		  rep = (p_mod_attr->d26_mnew)->d23_avrep;
		  ret_val = d26_i16_insert_val (p_at_entry, rep, val, length,
						(D26_dns **) NULL,
						(D26_dns **) NULL, &pos);

		  if (ret_val == D2_NO_ERR) /* insert_val successfull      */
		  {                         /* write C-ISAM record         */
		       if                   /* operation fails             */
			(D26_ISWRITE(svc_c_sev_warning,isfd, record) < 0)
		       {    
					    /* continue with next attribute*/
			    return_value = D26_ERROR;
			    continue;
		       }                     /* iswrite fails  */
		       iw_count++;

		  } else {        /* insert val fails                */
		       return_value = D26_ERROR;
		       continue;
		  }               /* insert_val fails */
		  break;

	     case D2_REP_AV:      /*  replace attr. value                  */
		                  /*  search old value and replace value   */
				  /*  and replace record                   */
		                  /*  */
		  if (p_mod_attr->d26_mold != (D23_av_value *) NULL)
		  {   
		       length = p_mod_attr->d26_mold->d23_avlen;
		       val = ((char *) (p_mod_attr->d26_mold)) +
			      sizeof (D23_av_value);
		       no_old_value = FALSE;
		  } else {
		       length = 0;
		       val = (char *) NULL;
		       no_old_value = TRUE;
		  } 

				  /* read record */
		  ret_val = d26_i18_read_rec_val (length, val, key, obj_filenr,
				    p_at_entry, no_old_value, TRUE,
				    ISEQUAL, isfd, D2_EQUAL, record);
		  if              /* record found                          */
		     (ret_val == D2_NO_ERR)
		  {               /* replace record                        */
		       ret_val = d26_i20_append_dnlidx (p_at_entry,
					 val_pos, (boolean *) NULL,
					 (signed32 **) NULL,
					 (signed16 *) NULL,
					 (signed16 *) NULL, &dnl_idx,
					 (D23_rserror *) NULL,
					 (signed32 *) NULL);

		       length = p_mod_attr->d26_mnew->d23_avlen;
		       val = ((char *) (p_mod_attr->d26_mnew)) +
			     sizeof (D23_av_value);
		       rep = (p_mod_attr->d26_mnew)->d23_avrep;
		       ret_val = d26_i16_insert_val (p_at_entry, rep,
						val, length,
						(D26_dns **) NULL,
						(D26_dns **) NULL, &pos);
		       if (ret_val == D2_NO_ERR) /* insert_val successfull */
		       {                         /* rewrite C-ISAM record  */
			    if                   /* operation fails        */
			       (D26_ISREWCURR(svc_c_sev_warning,isfd, record) 
				< 0)
			    {       
				 D26_ISRELEASE (svc_c_sev_warning,isfd);
				 return_value = D26_ERROR;
				 continue;
			    } else {             /* rewrite successfull    */
				 iw_count++;
				 D26_ISRELEASE (svc_c_sev_warning,isfd);
				 if (dnl_idx >= 0)
				 {   
				      d26_i02_upd_file_dnlist (D2_DEL_AV,
					      (D2_name_string) NULL,
					      D2_IGNORE,
					      (D2_name_string) NULL,
					      -1L, TRUE, &dnl_idx);
				 } 
			    }                     /* iswrite fails  */

		       } else {        /* insert val fails                */
			    return_value = D26_ERROR;
			    continue;
		       }               /* insert_val fails */

		  } else {        /* error: record not found               */
		       return_value = D26_ERROR;
		       continue;
		  }               /* record found */
		  break;

	     case D2_DEL_AV:      /*  delete  attr. value                  */
		                  /*  search old value and delete record   */
		                  /*  */
		  if (p_mod_attr->d26_mold != (D23_av_value *) NULL)
		  {   
		       length = p_mod_attr->d26_mold->d23_avlen;
		       val = ((char *) (p_mod_attr->d26_mold)) +
			      sizeof (D23_av_value);
		       no_old_value = FALSE;
		  } else {
		       length = 0;
		       val = (char *) NULL;
		       no_old_value = TRUE;
		  } 

				  /* read record */
		  ret_val = d26_i18_read_rec_val (length, val, key, obj_filenr,
				    p_at_entry, no_old_value, TRUE,
				    ISEQUAL, isfd, D2_EQUAL, record);
		  if (d26_i19_del_rec_val (ret_val, p_at_entry, isfd,
					   val_pos) != D2_NO_ERR)
		  {   
		       return_value = D26_ERROR;
		  } 
		  break;

	     case D2_DELATT:      /*  delete attribute                     */
		                  /*  search all values and delete the     */
				  /*  found records                        */
				  /*  ---> del_all_rec_vals ()             */
		                  /*  */
		  if (d26_i17_del_all_rec_vals (p_at_entry, key, obj_filenr,
						isfd, record) != D2_NO_ERR)
		  {   
		       return_value = D26_ERROR;
		  } 
		  break;

	     default:              /* error: unknown modification (already */
				   /* checked --> never occurs !)          */
		  continue;
	     }                    /*  type of modif.                       */
	}                    /* open file */
   }                         /*  real rec. attr. */
}                            /*  all attr. mods */

D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_WRITE,function,iw_count,0);

return (return_value);
}                               /*  d26_C13_change_rec_attrs          */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_C17_get_aliased_name                              */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        02.07.91                                              */
/*                                                                    */
/* SYNTAX:      EXPORT D2_name_string d26_C17_get_aliased_name (      */
/*                                   obj_class, obj_idx, at_idx,      */
/*                                   record, error, l_err)            */
/*                                                                    */
/*              Long            obj_class;           ->               */
/*              Short           obj_idx;             ->               */
/*              Short           at_idx;              ->               */
/*              char            *record;             ->               */
/*              D23_rserror     *error;              <-               */
/*              Int             *l_err;              <-               */
/*                                                                    */
/* DESCRIPTION: This function gets the aliased object name from       */
/*              the given record. The aliased object name is          */
/*              returned as return value of the function.             */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      Long              obj_class         object class index of     */
/*                                          entry (index in OCT)      */
/*                                                                    */
/*      Short             obj_idx           index of entry in SRT     */
/*                                                                    */
/*      Short             at_idx            index of aliased object   */
/*                                          name attribute in at      */
/*                                                                    */
/*      Char              *record           object record read by     */
/*                                          internal search (d26_u00) */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      D23_rserror      *error;            error message (contains   */
/*                                          the wrong attribute and/or*/
/*                                          return codes)             */
/*                                                                    */
/*      Int              *l_err;            length of error message   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_name_string   != D2_NULL   pointer to corresponding entry  */
/*                                    in distinguished name list in   */
/*                                    memory (the name is normed)     */
/*                                                                    */
/*                       == D2_NULL   there is no valid value stored  */
/*                                    in the record                   */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*     D26_oct       *d26_oct          pointer to OCT                 */
/*     D26_srt       *d26_srt          pointer to SRT                 */
/*     D26_at        *d26_at           pointer to AT                  */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        d26_i15_get_record_pos                                      */
/*        d26_u11_check_dn_index                                      */
/*        d27_010_set_error_apdu                                      */
/*        ldlong                                                      */
/*        sprintf                                                     */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

D2_name_string d26_C17_get_aliased_name 
(
  signed32         obj_class,
  signed16         obj_idx,
  signed16         at_idx,
  char            *record,
  D23_rserror     *error,
  signed32        *l_err
)

{                               /*  d26_C17_get_aliased_name          */

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

/* AUTO      ...    */
 D2_name_string   dn = (D2_name_string) NULL; /* return value */
 D26_dndesc       *ptr_dnlist_value;

 signed16         oct_idx;      /* index to OCT to determine           */
				/* file_nr for alias object            */
 signed16         file_nr;      /* file number of changed object       */
 signed32         dnl_idx;      /* index value to dn list in core      */
 D26_at           *p_at_entry;  /* pointer to at entry of attr.        */
 D26_nam_at       *p_nam_at;    /* pointer to naming attr. descr.      */
 D26_oct          *p_oct_entry; /* pointer to oct entry of attr.       */
 D26_ocl_arc      *ocl_arc;     /* pointer to arc of object classes    */
 char             *pos;         /* pointer to actual record position   */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/* determine smallest filenr for Alias object and correct oct entry */
p_oct_entry  = &d26_oct[obj_class];
ocl_arc = (d26_srt + obj_idx)->d26s_ocl_arc;
oct_idx = ocl_arc->d26s_oclass;
p_oct_entry  = d26_oct + oct_idx;
file_nr = p_oct_entry->d26c_filenr;
ocl_arc++;
for ( ; (oct_idx = ocl_arc->d26s_oclass) >= 0; ocl_arc++)
{ 
   if ((d26_oct + oct_idx)->d26c_filenr < file_nr)
   {   
	p_oct_entry  = d26_oct + oct_idx;
	file_nr = p_oct_entry->d26c_filenr;
   } 
} 

/* get value position */
p_at_entry   = &d26_at[at_idx];
p_nam_at     = (D26_nam_at *) NULL;
pos = d26_i15_get_record_pos (record, 1, p_at_entry, p_oct_entry,
			      p_nam_at, FALSE);

/* get value */
dnl_idx = ldlong (pos);

/* get distinguished name value */
d26_u11_check_dn_index (dnl_idx, &ptr_dnlist_value);
if (ptr_dnlist_value != (D26_dndesc *) NULL)
{    /* set dn */
     dn = ptr_dnlist_value->d26_dnname;
}
else
{ /* ERROR: no valid index in dnlist */
     d27_010_set_error_apdu (error, D2_TEMP_ERR, D2_REM_ERR, D2_NO_RESPONSE,
			     l_err);
} 

return (dn);
}                               /*  d26_C17_get_aliased_name         */






/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u97_search_attr_val                               */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        21.05.91                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Bool d26_u97_search_attr_val (record,          */
/*                                   p_at_entry,                      */
/*                                   p_oct_entry, p_nam_at,           */
/*                                   length, value, anz_vals,         */
/*                                   n_entry, val_no, pos)            */
/*                                                                    */
/*              Char             *record;            ->               */
/*              D26_at           *p_at_entry;        ->               */
/*              D26_oct          *p_oct_entry;       ->               */
/*              D26_nam_at       *p_nam_at;          ->               */
/*              Ushort           length;             ->               */
/*              Char             *value;             ->               */
/*              signed16            anz_vals;           ->               */
/*              D2_name_string   n_entry;            ->               */
/*              signed16            *val_no;            <-               */
/*              Char             **pos;              <-               */
/*                                                                    */
/* DESCRIPTION: This function searches the given value in the main    */
/*              entry and returns the number of the value and the     */
/*              position of the value in the record if the value was  */
/*              found.                                                */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      Char          *record             main entry                  */
/*                                                                    */
/*      D26_at        *p_at_entry         pointer to AT-entry  of     */
/*                                        attribute                   */
/*                                                                    */
/*      D26_oct       *p_oct_entry        pointer to OCT-entry of     */
/*                                        object                      */
/*                                                                    */
/*      D26_nam_at    *p_nam_at           pointer to naming attribute */
/*                                        description                 */
/*                                                                    */
/*      Ushort        length              attribute value length      */
/*                                                                    */
/*      Char          *value              attribute value             */
/*                                                                    */
/*      signed16         anz_vals            number of values of         */
/*                                        the given attribute in      */
/*                                        main entry                  */
/*                                                                    */
/*      D2_name_string   n_entry          normed entryname            */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      signed16         *val_no             value number of found value */
/*                                                                    */
/*      Char          **pos               position of value in        */
/*                                        record                      */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      Bool    D2_TRUE       value was found ('pos' and 'val_no' are */
/*                            supplied)                               */
/*                                                                    */
/*              D2_FALSE      value wasn't found ('pos' and 'val_no'  */
/*                            are not supplied)                       */
/*                            (error apdu and l_err are supplied)     */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*     D26_at       *d26_at        pointer to AT                      */
/*     D26_oct      *d26_oct       pointer to OCT                     */
/*     D26_srt      *d26_srt       pointer to SRT                     */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        d26_i15_get_record_pos                                      */
/*        d26_i38_cmp_equal                                           */
/*        d26_u18_scramble_string                                     */
/*                                                                    */
/* NOTES: is also called by d26_B03_change_main_entry (replace RDN)   */
/*                                                                    */
/*exoff ***************************************************************/


boolean d26_u97_search_attr_val (char            *record,
				 D26_at          *p_at_entry,
				 D26_oct         *p_oct_entry,
				 D26_nam_at      *p_nam_at,
				 signed16         length,
				 char            *value,
				 signed16         anz_vals,
				 D2_name_string   n_entry,
				 signed16        *val_no,
				 char           **pos
				)

{                               /*  d26_u97_search_attr_val                */

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

 boolean       found = FALSE;
 signed16      ret_val;          /* return value of a called function     */
 signed16      val_pos;

/* FASTAUTO  ...    */
register signed16      i;     /* counter */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/* initialize variables */
*val_no = 0;
*pos = (char *) NULL;

/* search given value */
*pos = d26_i15_get_record_pos (record, 1, p_at_entry,
			       p_oct_entry, p_nam_at, FALSE);

if (p_at_entry->d26a_syntax == D2_PASSWD_STX) /* encrypt value */
{
    d26_u18_scramble_string((Octet_string) value,(Octet_string) value,
			    (signed32) length);
}
for (i = 1; (i <= anz_vals) && (found == FALSE); i++)
{ 
   val_pos = 0;
   ret_val = d26_i38_cmp_equal((signed16)length,value,p_oct_entry->d26c_filenr,
			       *pos,n_entry,&val_pos,p_at_entry);
			/* get absolute value position                */
   *pos = *pos + val_pos;
   if                   /* value was found                            */
      (ret_val == D2_NO_ERR)
   {   
	found = TRUE;
	*val_no = i;
   } 
} 

if (p_at_entry->d26a_syntax == D2_PASSWD_STX) /* decrypt value */
{
    d26_u18_scramble_string((Octet_string) value,(Octet_string) value,
			    (signed32) length);
}
return (found);
}                               /*  d26_u97_search_attr_val                */
