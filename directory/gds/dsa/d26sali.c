/* 
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * HISTORY
 * $Log: d26sali.c,v $
 * Revision 1.1.8.2  1996/02/18  19:45:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:16  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:47:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:43  root]
 * 
 * Revision 1.1.6.3  1994/06/21  14:45:49  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:53:47  marrek]
 * 
 * Revision 1.1.6.2  1994/05/10  15:52:39  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:23:54  marrek]
 * 
 * Revision 1.1.6.1  1994/02/22  17:52:35  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:17:29  marrek]
 * 
 * Revision 1.1.4.3  1993/08/10  15:15:01  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.2  1993/08/10  14:35:36  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:16:40  marrek]
 * 
 * Revision 1.1.2.6  1993/02/18  10:23:27  marrek
 * 	fix for OT 7170
 * 	[1993/02/18  10:22:12  marrek]
 * 
 * Revision 1.1.2.5  1992/12/31  19:42:02  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:20:51  bbelch]
 * 
 * Revision 1.1.2.4  1992/12/17  23:43:57  treff
 * 	*** empty log message ***
 * 
 * Revision 1.1.2.3  1992/12/17  15:28:45  marrek
 * 	Null pointer was dereferenced in d26_u75_free_targets
 * 	[1992/12/16  13:58:04  marrek]
 * 
 * Revision 1.1.2.2  1992/11/27  20:19:25  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:38:10  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26sali.c,v $ $Revision: 1.1.8.2 $ $Date: 1996/02/18 19:45:53 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : d26sali.c       [d26sali]                           */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 21.07.92                                            */
/*                                                                    */
/* COMPONENT    : DSA, retrieval functions                            */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   The module contains the functions for searching   */
/*                  aliases in evaluation phase of search.            */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 21.07.92| birth                          | ek |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*                                                                    */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <stdlib.h>
#include <string.h>

/*****  external Includes    *****/

/*****  internal Includes    *****/

#include <d26schema.h>

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
/*                D E C L A R A T I O N S        DATA                 */
/*                                                                    */
/**********************************************************************/

/******** IMPORT ********/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/

static int d26_u74_cmp_target(const void *, const void *);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u69_fill_target()                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        15.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function compares the DN dname with the names    */
/*              contained in the target list. If it is shorter,       */
/*              it overwrites the other DN. If it is longer or equal, */
/*              it is ignored. If it is not comparable, it is appended*/
/*              to the list of target objects.                        */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      dname       D26_name_string normed DN of target object        */
/*      ename       D26_name_string exact DN of target object         */
/*      to_nr       Short           target object number              */
/*      subset      D2_subset       indicates subset to be searched   */
/*      target_root Void **         root of target object tree        */
/*      act_target  D26_target **   actual target object              */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*      target_root Void **         array of target objects           */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NULL                     error occurred                    */
/*      != D2_NULL  D26_target *    pointer to actual target object   */
/*                                                                    */
/*exoff ***************************************************************/

D26_target *d26_u69_fill_target(
    D2_name_string dname,
    D2_name_string ename,
    signed16       to_nr,
    D2_subset      subset,
    void           **target_root,
    D26_target     *act_target)

{                               /*  d26_u69_fill_target               */

  void **tsearch_result;    /* result returned by tsearch             */

if                              /*  memory for target object not yet  */
				/*  allocated                         */
  (act_target == NULL)
{                               /*  allocate new memory               */
  if                            /*  no more memory available          */
    ((act_target = (D26_target *)D27_CALLOC(svc_c_sev_warning,1,
     sizeof(D26_target))) == NULL)
  {                             /*  return error                      */
    return((D26_target *)NULL);
  }                             
}                               

if                              /*  no memory for normed target name  */
  ((act_target->d26_tdname = (D2_name_string)D27_REALLOC(svc_c_sev_warning,
   act_target->d26_tdname,strlen((char *)dname) + 1)) == NULL)
{                               /*  free target and return error      */
  free((void *)act_target);
  return((D26_target *)NULL);
}
else if                         /*  no memory for exact target name   */
       ((act_target->d26_tename = (D2_name_string)D27_REALLOC(
	svc_c_sev_warning,act_target->d26_tename,strlen((char *)ename) + 1))
	== NULL)
{                               /*  free target and return error      */
  free(act_target->d26_tdname);
  free((void *)act_target);
  return((D26_target *)NULL);
}
else
{                               /*  fill components of target object  */
  strcpy((char *)act_target->d26_tdname,(char *)dname);
  strcpy((char *)act_target->d26_tename,(char *)ename);
  act_target->d26_tbo_nr  = to_nr;
  act_target->d26_tsubset = subset;
  act_target->d26_tbase   = FALSE;
  while                         /*  comparable target object was      */
				/*  created before                    */
       (*(tsearch_result = D27_TSEARCH(svc_c_sev_warning,(void *)act_target,
	target_root,d26_u74_cmp_target)) != (void *)act_target)
  {                             /*  check if old target is longer     */
    if                          /*  old target is longer              */
      ((int)strlen((char *)((D26_target *)*tsearch_result)->d26_tdname) >
       (int)strlen((char *)act_target->d26_tdname))
    {                           /*  free memory for old target and    */
				/*  delete it from tree (overwriting  */
				/*  would destroy the ordering)       */
      tdelete(*tsearch_result,target_root,d26_u74_cmp_target);
      free(((D26_target *)*tsearch_result)->d26_tdname);
      free(((D26_target *)*tsearch_result)->d26_tename);
      free(*tsearch_result);
    }
    else
    {                           /*  mark actual target as invalid     */
      *act_target->d26_tdname = D2_EOS;
      break;
    }                           
  }                             
}                               

return(act_target);

}                               /*  d26_u69_fill_target               */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u73_use_alias                                     */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        20.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function dereferences an aliases from object     */
/*              record. If necessary it is put into the target object */
/*              tree. Then the break object of the aliased object is  */
/*              searched in the local database and the other          */
/*              components of the target object structure are filled  */
/*              from that.                                            */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      cmp_target  D26_target *    target object to compare          */
/*      subset      D2_subset       indicates subset to be searched   */
/*      filenr      Short           number of C-ISAM objects file     */
/*      target_root Void **         root of target object tree        */
/*      act_target  D26_target **   target object to insert           */
/*      message     String **       actual message pointer            */
/*      output      String *        pointer to beginning of message   */
/*      output_len  Int *           maximum length of result message  */
/*      remlen      Int *           remaining length for result data  */
/*      r_info      D23_rq_info *   information about required attr.  */
/*      f_info      D23_ft1_info *  information about filter attr.    */
/*      op_id       Short           indicates the calling function    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      target_root Void **         root of target object tree        */
/*      act_target  D26_target **   actual target object              */
/*      db_access   Bool *          indicates intermediate database   */
/*                                  access                            */
/*      cont_sa     Bool *          indicates whether search_aliases  */
/*                                  should continue                   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   Short           no error occurred                 */
/*      D26_ERROR                   error occurred                    */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u73_use_alias(
    D26_target   *cmp_target,
    D2_subset    subset,
    signed16     filenr,
    void         **target_root,
    D26_target   **act_target,
    boolean      *db_access,
    byte         **message,
    byte         **output,
    signed32     *output_len,
    signed32     *remlen,
    D23_rq_info  *r_info,
    D23_ft1_info *f_info,
    signed16     op_id,
    boolean      *cont_sa)

{                                /*  d26_u73_use_alias                */

  signed32       dnl_idx;   /* DN-List index of aliased object name   */
			    /* normed aliased object name             */
  byte           n_aliased_object[D2_DNL_MAX + 1];
			    /* exact aliased object name              */
  byte           e_aliased_object[D2_DNL_MAX + 1];
			    /* normed dereferenced object             */
  byte           deref_obj[D2_DNL_MAX + 1];
			    /* pointer to unresolved part of target   */
			    /* object                                 */
  D2_name_string unresolved;
  D26_dninfo     dninfo;    /* information about aliased object and   */
			    /* dereferenced object                    */
  signed16       ob_nr;     /* object number of dereferenced alias    */
  D26_target     hdl_target;  /* target object to be handled          */
  D26_target     *equi_target;/* equivalent target object             */

dnl_idx = d26_i47_get_al_references(filenr,d26_ob_record);
if                      /*  alias name is in DN-list                  */
  (d26_i33_deref_alias(dnl_idx,n_aliased_object,e_aliased_object,&dninfo)
   >= 0)
{                       /*  put aliased object into the target object */
			/*  tree                                      */
  ob_nr = dninfo.d26_rdn[dninfo.d26_rd_anz - 1].d26_r_idx;
  if                    /*  no more memory available                  */
    ((*act_target = d26_u69_fill_target((D2_name_string)n_aliased_object,
     (D2_name_string)e_aliased_object,ob_nr,
     subset == D2_ONE_LEVEL ? D2_BASE_OBJECT : D2_WHOLE_SUBTREE,
     target_root,*act_target)) == (D26_target *)NULL)
  {                     /*  return error                              */
    return(D26_ERROR);
  }
  else if             /*  actual target object was used             */
	 (*(*act_target)->d26_tdname != D2_EOS)
  {                     /*  set count of aliased RDNs                 */
    (*act_target)->d26_tal_rdns = dninfo.d26_rd_anz;
    if                  /*  information about aliased object could    */
			/*  be found                                  */
      ((unresolved = d26_u31_find_break_object(&dninfo,n_aliased_object,
       deref_obj,d26_ob_record,FALSE,D23_SEARCH)) != NULL)
    {                   /*  check whether any object is found         */
      (*act_target)->d26_tunresolved = (*act_target)->d26_tdname +
					   (unresolved - n_aliased_object);
      (*act_target)->d26_tmessage    = message;
      if                /*  any object was found                      */
	(dninfo.d26_rd_anz > 0)
      {                 /*  take object class, ACL and master         */
			/*  knowledge from current record             */
	d26_i36_read_ocl_acl_mk(&dninfo,d26_ob_record,
	    (*act_target)->d26_tob_cl,(*act_target)->d26_tacl,
	    &(*act_target)->d26_tmk);
      }
      else
      {                 /*  take first level master knowledge         */
	(*act_target)->d26_tmk = d26_u64_first_level_mk();
      }                 
      if                /*  alias could be dereferenced locally       */
	(*(*act_target)->d26_tunresolved == D2_EOS)
      {                 /*  enhance target object information         */
	(*act_target)->d26_toutput     = output;
	(*act_target)->d26_toutput_len = output_len;
	(*act_target)->d26_tremlen     = remlen;
	(*act_target)->d26_tr_info     = r_info;
	(*act_target)->d26_tf_info     = f_info;
	(*act_target)->d26_top_id      = op_id;
	(*act_target)->d26_tfilenr     = dninfo.d26_filenr;
	if              /*  memory for record can't be allocated      */
	  (((*act_target)->d26_trecord = (char *)D27_MALLOC(
	   svc_c_sev_warning,(unsigned)d26_oreclen)) == NULL)
	{               /*  log and return error                      */
	  return(D26_ERROR);
	}
	else
	{               /*  copy read record                          */
	  memcpy((*act_target)->d26_trecord,d26_ob_record,d26_oreclen);
	  hdl_target.d26_tbo_nr  = (*act_target)->d26_tbo_nr;
	  hdl_target.d26_tsubset = (*act_target)->d26_tsubset;
	  hdl_target.d26_tdname  = deref_obj;
	  *act_target = (D26_target *)NULL;
	  if            /*  whole subtree must be searched            */
	    (subset == D2_WHOLE_SUBTREE)
	  {             /*  search aliases                            */
	    if          /*  error from searching aliases              */
	      (d26_i50_search_aliases(&hdl_target,subset,deref_obj,&dninfo,
	       ob_nr,target_root,act_target,message,output,output_len,remlen,
	       r_info,f_info,op_id) != D2_NO_ERR)
	    {           /*  return error                              */
	      return(D26_ERROR);
	    }           
	  }             
	}               
      }
      else
      {                 /*  just reset actual target object pointer   */
	*act_target = (D26_target *)NULL;
      }                 
    }
    else
    {                   /*  set unresolved part to NULL               */
      (*act_target)->d26_tunresolved = NULL;
    }                   
    *db_access  = TRUE;
    equi_target = *(D26_target **)tfind((void *)cmp_target,target_root,
					d26_u74_cmp_target);
    if                  /*  equivalent target object has different DN */
      (strcmp((char *)equi_target->d26_tdname,(char *)cmp_target->d26_tdname)
       != 0)
    {                   /*  break search aliases                      */
      *cont_sa = FALSE;
    }                   
  }                     
}                       

return(D2_NO_ERR);

}                       /*  d26_u73_use_alias                         */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u74_cmp_target                                    */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        21.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function compares two target object structures.  */
/*              Two target objects are considered as equal, when      */
/*              one of the DNs of the target objects is equal or      */
/*              subordinate to the other. Otherwise strcmp is used    */
/*              for comparison.                                       */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      target1     Void *          first target object to compare    */
/*      target2     Void *          second target object to compare   */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*          -1  Int     first target object is less than second       */
/*           0          target objects are equal                      */
/*           1          first target object is greater than second    */
/*                                                                    */
/*inoff ***************************************************************/

static int d26_u74_cmp_target(
    const void *target1,
    const void *target2)

{                               /*  d26_u74_cmp_target                */

  D26_srt *srt1;                /*  SRT entry of first target object  */
  D26_srt *srt2;                /*  SRT entry of second target object */
  int     len1;                 /*  length of first target object     */
  int     len2;                 /*  length of second target object    */
  int     cmp;                  /*  result of comparison of           */
				/*  old and new length                */
  int     minlen;               /*  minimum length of target objects  */

srt1 = d26_srt + ((D26_target *)target1)->d26_tbo_nr;
srt2 = d26_srt + ((D26_target *)target2)->d26_tbo_nr;

len1 = strlen((char *)((D26_target *)target1)->d26_tdname);
len2 = strlen((char *)((D26_target *)target2)->d26_tdname);
minlen = len1 < len2 ? len1 : len2;

cmp  = strncmp((char *)((D26_target *)target1)->d26_tdname,
	       (char *)((D26_target *)target2)->d26_tdname,minlen);

if                      /*  one target object name is an initial      */
			/*  substring of the other                    */
  (cmp == 0)
{                       /*  compare lengths of target objects         */
  if                    /*  target objects are equal                  */
    (len1 == len2)
  {                     /*  compare subsets                           */
    if                  /*  first subset is smaller than second       */
      (((D26_target *)target1)->d26_tsubset
       < ((D26_target *)target2)->d26_tsubset)
    {                   /*  return SMALLER                            */
      return(-1);
    }
    else if             /*  first subset is greater than second       */
	   (((D26_target *)target1)->d26_tsubset >
	    ((D26_target *)target2)->d26_tsubset)
    {                   /*  return GREATER                            */
      return(1);
    }                   
  }
  else if               /*  first target is shorter than second       */
	 (len1 < len2)
  {                     /*  check whether second target is extension  */
			/*  of first                                  */
    if                  /*  second target is not extension of first   */
      (len1 != 0 && ((D26_target *)target2)->d26_tdname[len1] !=D2_RDN_SEP ||
       ((D26_target *)target1)->d26_tsubset != D2_WHOLE_SUBTREE &&
       (((D26_target *)target1)->d26_tsubset != D2_ONE_LEVEL ||
	srt2->d26s_par_nr != ((D26_target *)target1)->d26_tbo_nr))
    {                   /*  return SMALLER                            */
      return(-1);
    }                   
  }
  else
  {                     /*  check whether first  target is extension  */
			/*  of second                                 */
    if                  /*  first target is not extension of second   */
      (len2 != 0 && ((D26_target *)target1)->d26_tdname[len2] !=D2_RDN_SEP ||
       ((D26_target *)target2)->d26_tsubset != D2_WHOLE_SUBTREE &&
       (((D26_target *)target2)->d26_tsubset != D2_ONE_LEVEL ||
	srt1->d26s_par_nr != ((D26_target *)target2)->d26_tbo_nr))
    {                   /*  return GREATER                            */
      return(1);
    }                   
  }                     
}                       

return(cmp);

}                       /*  d26_u74_cmp_target                        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u75_free_targets                                  */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        22.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function frees all target object structures      */
/*              and deletes them from the tree                        */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      target_root Void **         root of target object tree        */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      target_root Void **         root of target object tree        */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*exoff ***************************************************************/

void d26_u75_free_targets(void **target_root)

{                               /*  d26_u75_free_targets              */

  D26_target *act_target;       /* actual target object to delete     */

for                     /*  all target objects in tree                */
   (; *target_root != (void *)NULL; )
{                       /*  delete the node from binary tree          */
  act_target = *(D26_target **)*target_root;
  tdelete((void *)act_target,target_root,d26_u74_cmp_target);
  if                    /*  record was allocated in target object     */
    (act_target->d26_trecord != (char *)NULL)
  {                     /*  free the record                           */
    free(act_target->d26_trecord);
  }                     
  free(act_target->d26_tdname);
  free(act_target->d26_tename);
  free(act_target);
}                       

}                       /*  d26_u75_free_targets                      */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u76_target_action                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        23.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function checks, whether the target object may   */
/*              be used for evaluation of a subtree, for a            */
/*              continuation reference, or must be ignored. It calls  */
/*              the appropriate functions to do this.                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

void d26_u76_target_action(
			/* IN    -  target object to handle           */
    const void *v_target,
    VISIT visit,        /* IN    -  kind of visit to the node         */
    const int level)    /* IN    -  level of node in tree             */

{                               /* d26_u76_target_action              */

  D26_target *target;       /* pointer to target object               */
			    /* name of target object                  */
  byte       dname[D2_DNL_MAX];
  D26_dninfo dninfo;        /* information about target object        */
  D2_name_string eunres;    /* exact unresolved part of target object */

  if                    /* target object is handled first time        */
    (visit == preorder || visit == leaf)
  {                     /* check whether name resolution was          */
			/* successful                                 */
    target = *(D26_target **)v_target;
    if                  /* unresolved part present, message pointer   */
			/* still valid, size and time limit not       */
			/* exceeded                                   */
      (target->d26_tunresolved != NULL &&
       *target->d26_tmessage != NULL)
    {                   /* check whether aliased object was           */
			/* dereferenced locally                       */
      dninfo.d26_rd_anz = 0;
      if                /* aliased object is dereferenced locally     */
	(*target->d26_tunresolved == D2_EOS &&
	 (target->d26_tmk == D26_OWNNAME ||
	  d26_serv_contr->d23_scopt & D2_LOCALSCOPE ||
	  target->d26_tbase == TRUE))
      {                 /* evaluate subtree                           */
	strcpy((char *)dname,(char *)target->d26_tdname);
	d26_u06_get_name(dname,&dninfo);
	dninfo.d26_filenr = target->d26_tfilenr;
	*target->d26_tmessage = d26_i44_write_set(*(target->d26_tmessage),
	  target->d26_toutput,target->d26_tsubset,dname,target->d26_tename,
	  &dninfo,target->d26_tal_rdns,target->d26_tbo_nr,target->d26_tob_cl,
	  target->d26_tacl,target->d26_tmk,target->d26_trecord,
	  target->d26_tr_info,target->d26_tf_info,target->d26_toutput_len,
	  target->d26_tremlen,target->d26_top_id);
      }
      else if           /* local scope not set                        */
	     (!(d26_serv_contr->d23_scopt & D2_LOCALSCOPE))
      {                 /* generate referral                          */
	if              /* complete target object is unresolved       */
	  (target->d26_tunresolved == target->d26_tdname)
	{               /* make referral with unresolved part only    */
	  eunres = target->d26_tename;
	  d26_u70_make_error_referrals(&dninfo,(D2_name_string)"",
	    target->d26_tal_rdns,(D2_name_string)"",target->d26_tunresolved,
	    eunres,(D23_av_value *)NULL,target->d26_tmk,
	    target->d26_tsubset == D2_BASE_OBJECT ? TRUE : FALSE);
	}
	else if         /* complete target object is resolved         */
	       (*target->d26_tunresolved == D2_EOS)
	{               /* make referral with resolved part only      */
	  eunres = target->d26_tename +
		   (target->d26_tunresolved - target->d26_tdname);
	  d26_u06_get_name(target->d26_tdname,&dninfo);
	  d26_u70_make_error_referrals(&dninfo,target->d26_tdname,
	    target->d26_tal_rdns,target->d26_tename,target->d26_tunresolved,
	    eunres,(D23_av_value *)NULL,target->d26_tmk,
	    target->d26_tsubset == D2_BASE_OBJECT ? TRUE : FALSE);
	}
	else
	{               /* make referral with both parts              */
	  *target->d26_tunresolved = D2_EOS;
	  eunres = target->d26_tename +
	    (target->d26_tunresolved - target->d26_tdname);
	  *eunres = D2_EOS;
	  d26_u06_get_name(target->d26_tdname,&dninfo);
	  d26_u70_make_error_referrals(&dninfo,target->d26_tdname,
	    target->d26_tal_rdns,target->d26_tename,
	    target->d26_tunresolved + 1,eunres + 1,(D23_av_value *)NULL,
	    target->d26_tmk,
	    target->d26_tsubset == D2_BASE_OBJECT ? TRUE : FALSE);
	  *target->d26_tunresolved = D2_RDN_SEP;
	  *eunres = D2_RDN_SEP;
	}
      }
    }
  }

}                       /* d26_u76_target_action                      */
