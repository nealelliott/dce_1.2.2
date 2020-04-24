/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: retreq.c,v $
 * Revision 1.1.10.2  1996/02/18  18:15:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:21  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:51:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:58  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:53:23  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:29:15  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:37:46  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:26:36  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  16:11:27  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:30:29  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:44:55  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:23:03  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:38:17  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:27:25  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  02:59:55  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:34:44  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: retreq.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:15:49 $";
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
/* NAME         : retreq.c        [retreq]                            */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 10.05.88                                            */
/*                                                                    */
/* COMPONENT    : DSA, retrieval functions                            */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   The module contains the utilities used by the     */
/*                  internal search function handling requested       */
/*                  attributes.                                       */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 10.05.88| birth                          | ek |         */
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
#include <search.h>

/*****  external Includes    *****/

#include <dce/d27util.h>

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
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/

static signed16 d26_u51_attr_idx_at(D26_attr_info *attr, signed16 ob_nr,
    signed32 *ob_cl, signed16 fct_tag);
static signed16 d26_u52_attr_idx_mess(D26_attr_info *attr, signed16 ob_nr,
    signed32 *ob_cl, signed16 cacheflag, D23_rq_info *r_info, signed16 *rv);
static void     d26_u53_nam_at_idx(D26_attr_info *attr, signed16 ob_nr);
static byte     *d26_u55_write_dnlist_attribute(signed16 filenr,
    D2_name_string object, byte *message, byte **output, D2_ret_val r_type,
    D26_at *at, signed32 *remlen, signed32 *output_len);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u43_no_req_attr_error                             */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        30.10.90                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the error message for read, when */
/*              the object holds none of the requested attributes.    */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*exoff ***************************************************************/

void d26_u43_no_req_attr_error(
    byte **message,     /* OUT   -  result message to write           */
			/* IN    -  distinguished name of object found*/
    D2_name_string dname,
    D23_rq_info *r_info,/* IN    -  information about requested attr. */
			/* IN    -  maximum length of result block    */
    signed32 *output_len,
    signed32 *remlen)   /* INOUT -  remaining length for result data  */

{                               /*  d26_u43_no_req_attr_error         */

  byte      *r_mess;        /* message pointer to requested attributes*/
  D2_a_type a_type;         /* requested attribute type               */
  signed32  meslen = 0;     /* message length                         */

  register signed16 i;      /* count of attributes                    */

  for                       /* all requested attributes               */
     (i = 1, r_mess = (byte *)(r_info + 1); i <= r_info->d23_rqnbr; i++,
      r_mess += sizeof(D23_at_type) + ((D23_at_type *)r_mess)->d23_atoff)
  {                         /* get attribute type from message        */
    a_type.d2_typ_len = ((D23_at_type *)r_mess)->d23_atlen;
    a_type.d2_type    = (Ob_id_string)((D23_at_type *)r_mess + 1);
    d26_u15_attr_error(message,dname,i,D2_ATTRIBUTE_MISSING,&a_type,0,0,0,
      (byte *)"",output_len,&meslen);
  }

  *remlen -= meslen;

}                       /*  d26_u43_no_req_attr_error                 */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u50_write_attributes                              */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        08.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the requested attributes   into  */
/*              message in IPC format.                                */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      record      String          Record to read data from          */
/*      message     String          actual message pointer            */
/*      output      String *        pointer to beginning of message   */
/*      dname       D2_name_string  normed DN of found object         */
/*      ename       D2_name_string  exact DN of found object          */
/*      r_info      D23_rq_info *   information about requested attr. */
/*      attr        D26_attr_info * array of indices (and a flag for  */
/*                                  read permission for each requested*/
/*                                  attribute in the AT (the last one */
/*                                  will be "-1")                     */
/*      ob_nr       Short           object number for object          */
/*      filenr      Short           file number containing the object */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of result message  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      recur       Bool *          indicates whether recurring       */
/*                                  attributes are supplied           */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           length of result message          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              String                      pointer to byte after     */
/*                                          written message.          */
/*                                                                    */
/*exoff ***************************************************************/

byte *d26_u50_write_attributes(char *record, byte *message, byte **output,
    D2_name_string dname, D2_name_string ename, D23_rq_info *r_info,
    boolean *recur, D26_attr_info *attr, signed16 ob_nr, signed16 filenr,
    signed32 *remlen, signed32 *output_len)

{                               /*  d26_u50_write_attributes          */

			    /* type of cache-pseudo-attribute         */
  static D2_a_type cache_type = {sizeof(D2_CACHE_ATTR) - 1,
				 (Ob_id_string)D2_CACHE_ATTR};

  D26_srt  *srt;            /* pointer to SRT                         */
  D26_at   *at;             /* pointer to AT                          */
  signed16 cmp_filenr;      /* number of object file to compare       */
  signed16 pos;             /* position of attribute in record        */

  register D26_ocl_arc *ocl_arc;/* loop pointer to object class arcs  */

*recur = FALSE;
srt    = d26_srt + ob_nr;

if                      /*  file number is undefined (Alias!)         */
  (filenr < 0)
{                       /*  search for least filenumber using SRT-    */
			/*  entry                                     */
  for                   /*  all assigned OCT-entries                  */
     (ocl_arc = srt->d26s_ocl_arc; ocl_arc->d26s_oclass != -1; ocl_arc++)
  {                     /*  compare file-numbers                      */
    cmp_filenr = d26_oct[ocl_arc->d26s_oclass].d26c_filenr;
    filenr = filenr < 0 || cmp_filenr < filenr ? cmp_filenr : filenr;
  }                     
}                       

for                     /*  all possible attributes                   */
   (; attr->d26_at_idx != D26_EOL; attr++)
{                       /*  check readability                         */
  if                    /*  attribute is readable                     */
    (attr->d26_read_ok == TRUE)
  {                     /*  calculate at pointer                      */
    at     = d26_at + attr->d26_at_idx;
    pos    = attr->d26_srt_idx != D26_IDX_NOT_DEF ?
		 srt->d26s_nam_at[attr->d26_nam_idx].d26s_position[filenr] :
		 at->d26a_field_pos[filenr];
    if                  /*  attribute is stored at object             */
      (at->d26a_filenr < 0)
    {                   /*  read attributes from record               */
      message = d26_i51_write_single_attribute(filenr,record,pos,ename,
		    message,output,r_info->d23_rqreq,at,remlen,output_len);
    }
    else if             /*  DN-List Attribute of own DSA is required  */
	   (attr->d26_at_idx == d26_dnl_idx &&
	    !strcmp((char *)dname,(char *)d26_myname))
    {                   /*  read Attribute from DN-List               */
      *recur  = TRUE;
      message = d26_u55_write_dnlist_attribute(filenr,ename,message,output,
			r_info->d23_rqreq,at,remlen,output_len);
    }
    else
    {                   /*  read attributes from attribute file       */
      *recur  = TRUE;
      message = d26_i52_write_recur_attribute(record,filenr,pos,ename,
		    message,output,r_info->d23_rqreq,at,remlen,output_len);
    }                   
  }
  else if               /*  Pseudo attribute occurred                 */
	 (attr->d26_at_idx == D26_IDX_NOT_DEF)
  {                     /*  write pseudo attribute type               */
    message = d26_u56_write_attribute_type(message,output,&cache_type,0,0,
					   remlen,output_len);
  }                     
  if                    /*  error from writing attribute              */
    (message == NULL)
  {                     /*  return error                              */
    return(NULL);
  }                     
}                       
			/*  return message pointer                    */
return(message);

}                       /*  d26_u50_write_attributes                  */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u51_attr_idx_at()                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        09.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function gets all at indices assigned to ob_nr  */
/*              into attr.                                            */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      ob_nr       Short           object number for object          */
/*      ob_cl       Long *          object class                      */
/*      fct_tag     Short           indicates the calling function    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      attr        D26_attr_info * array of indices (and a flag for  */
/*                                  read permission for each requested*/
/*                                  attribute in the AT (the last one */
/*                                  will be "-1")                     */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        Short           count of assigned indices         */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_u51_attr_idx_at(D26_attr_info *attr, signed16 ob_nr,
    signed32 *ob_cl, signed16 fct_tag)

{                               /*  d26_u51_attr_idx_at               */

  D26_oct    *oct;              /* pointer to OCT                     */
  D26_nam_at *nam_at;           /* pointer to naming attribute in SRT */

  register signed16 i,j,at_idx; /* loop variables                     */

for                     /*  all AT-entry usage indicators             */
   (j = 0; j < d26_noat; j++)
{                       /*  initialize the indicator                  */
  d26_ate_used[j] = FALSE;
}                       

for                     /*  all assigned object classes               */
   (j = 0; *ob_cl != D26_EOL; ob_cl++)
{                       /*  scan mandatory attributes                 */
  oct = d26_oct + *ob_cl;
  for                   /*  all mandatory attributes                  */
     (i = 0, at_idx = oct->d26c_mandatory[i]; at_idx >= 0;
      i++, at_idx = oct->d26c_mandatory[i])
  {                     /*  check whether Attribute is used already   */
    if                  /*  attribute is not yet used                 */
      (d26_ate_used[at_idx] == FALSE)
    {                   /*  assign AT-index                           */
      attr[j].d26_at_idx  = at_idx;
      d26_u53_nam_at_idx(attr + j,ob_nr);
      d26_ate_used[at_idx] = TRUE;
      j++;
    }                   
  }                     
  for                   /*  all optional  attributes                  */
     (i = 0, at_idx = oct->d26c_optional[i]; at_idx >= 0;
      i++, at_idx = oct->d26c_optional[i])
  {                     /*  check whether Attribute is used already   */
    if                  /*  attribute is not yet used                 */
      (d26_ate_used[at_idx] == FALSE)
    {                   /*  check for operational attribute           */
      if                /*  attribute is not operational or ADM       */
			/*  function was called                       */
	(at_idx != d26_mk_idx && at_idx != d26_acl_idx &&
	 at_idx != d26_dnl_idx &&
	 (at_idx != d26_pwd_idx || *ob_cl != d26_dsa_idx) ||
	 fct_tag > D2_ADM_OPID)
      {                 /*  assign AT-index                           */
	attr[j].d26_at_idx  = at_idx;
	d26_u53_nam_at_idx(attr + j,ob_nr);
	d26_ate_used[at_idx] = TRUE;
	j++;
      }                 
    }                   
  }                     
  if                    /*  object is alias                           */
    (*ob_cl == d26_ali_idx)
  {                     /*  set index of naming attribute             */
    for                 /*  all naming attributes of structure rule   */
       (i = 0, nam_at = d26_srt[ob_nr].d26s_nam_at; nam_at->d26s_index != -1;
	i++, nam_at++)
    {                   /*  check whether Attribute is used already   */
      at_idx = nam_at->d26s_index;
      if                /*  attribute is not yet used                 */
	(d26_ate_used[at_idx] == FALSE)
      {                 /*  assign SRT index and naming index in      */
			/*  attribute information structure           */
	attr[j].d26_at_idx   = at_idx;
	attr[j].d26_srt_idx  = ob_nr;
	attr[j].d26_nam_idx  = i;
	d26_ate_used[at_idx] = TRUE;
	j++;
      }                 
    }                   
  }                     
}                       

return(j);

}                               /*  d26_u51_attr_idx_at              */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u52_attr_idx_mess()                               */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        09.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function gets all at indices assigned to ob_nr  */
/*              into attr, that are ok for cacheflag.                 */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              al_flag     Bool            indicates, whether object */
/*                                          is alias.                 */
/*              ob_nr       Short           object number for object  */
/*              ob_cl       Long *          object classes of object  */
/*              cacheflag   Short           indicates, whether only   */
/*                                          cacheable, not cacheable  */
/*                                          or all attributes have    */
/*                                          to be taken.              */
/*              r_info      D23_rq_info *   information about required*/
/*                                          attributes                */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              attr        D26_attr_info * array of indices (and a   */
/*                                          flag for read permission  */
/*                                          for each requested        */
/*                                          attribute in the at (the */
/*                                          last one will be "-1")    */
/*              rv          Short *         return value of internal  */
/*                                          search                    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              Short       >= 0            count of assigned indices */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_u52_attr_idx_mess(D26_attr_info *attr, signed16 ob_nr,
    signed32 *ob_cl, signed16 cacheflag, D23_rq_info *r_info, signed16 *rv)

{                               /*  d26_u52_attr_idx_mess             */

  D23_at_type *ra_type;     /* pointer to requested attribute type in */
			    /* IPC-message                            */
  D2_a_type a_type;         /* attribute type                         */
  D26_nam_at *nam_at;       /* pointer to naming attribute in SRT     */
  signed16 idx;             /* index of AT-entry                      */

  register signed16 i,j;    /* loop variables                         */

for                     /*  all requested attributes                  */
   (i = 0, j = 0, ra_type = (D23_at_type *)(r_info + 1);
    i < r_info->d23_rqnbr; i++,
    ra_type = (D23_at_type *)((byte *)(ra_type + 1) + ra_type->d23_atoff))
{                       /*  search for index in at                   */
  a_type.d2_typ_len = ra_type->d23_atlen;
  a_type.d2_type    = (Ob_id_string)(ra_type + 1);
  if                    /*  attribute not in AT                       */
    ((idx = d26_u03_attr_search(&a_type)) < 0)
  {                     /*  set internal search return value          */
    *rv = D2_OBJ_CLS_VIOLATION;
  }
  else if               /*  cache flag may be ignored or cache flag   */
			/*  matches attribute                         */
	 (cacheflag == D2_IGNORE ||
	  cacheflag == (d26_at[idx].d26a_acl == D26_PUBLIC))
  {                     /*  check whether the attribute is in object  */
			/*  class                                     */
    if                  /*  object classes contain attribute          */
      (d26_u59_attr_in_obj_class(ob_cl,idx) == TRUE)
    {                   /*  check cache flag                          */
      attr[j].d26_at_idx  = idx;
      d26_u53_nam_at_idx(attr + j,ob_nr);
      j++;
    }
    else if             /*  object is alias                           */
	   (!D2_ATTCMP(&d26_oct[*ob_cl].d26c_ob_id,D2_OALIAS))
    {                   /*  check whether the attribute is naming     */
      for               /*  all naming attributes of structure rule   */
	 (nam_at = d26_srt[ob_nr].d26s_nam_at; nam_at->d26s_index != -1;
	  nam_at++)
      {                 /*  check whether it is the given one         */
	if              /*  attributes match                          */
	  (nam_at->d26s_index == idx)
	{               /*  assign SRT index and naming index in      */
			/*  attribute information structure           */
	  attr[j].d26_at_idx  = idx;
	  attr[j].d26_srt_idx = ob_nr;
	  attr[j].d26_nam_idx = i;
	  j++;
	  break;
	}               
      }                 
      if                /*  attribute is not naming                   */
	(nam_at->d26s_index == -1)
      {                 /*  set internal search return value          */
	*rv = D2_OBJ_CLS_VIOLATION;
      }                 
    }
    else
    {                   /*  set internal search return value          */
      *rv = D2_OBJ_CLS_VIOLATION;
    }                   
  }                     
}                       

return(j);

}                               /*  d26_u52_attr_idx_mess             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u53_nam_at_idx                                    */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        23.03.92                                              */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION: This function gets the SRT index and naming attribute */
/*              index of an attribute information.                    */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*exoff ***************************************************************/

static void d26_u53_nam_at_idx(
			/* INOUT -  pointer to attribute information  */
    D26_attr_info  *attr,
    signed16 ob_nr)     /* IN    -  object number of object           */

{                               /* d26_u53_nam_at_idx                 */

  register signed16 i;          /* loop variables                     */
  register D26_nam_at *nam_at;  /* loop pointer to naming attributes  */

attr->d26_srt_idx = D26_IDX_NOT_DEF;

for                     /*  all naming attributes of structure rule   */
   (i = 0, nam_at = d26_srt[ob_nr].d26s_nam_at; nam_at->d26s_index != -1;
    i++, nam_at++)
{                       /*  compare attribute indices                 */
  if                    /*  attribute is a naming one                 */
    (nam_at->d26s_index == attr->d26_at_idx)
  {                     /*  assign SRT index and naming index in      */
			/*  attribute information structure           */
    attr->d26_srt_idx = ob_nr;
    attr->d26_nam_idx = i;
    break;
  }                     
}                       

return;

}                               /*  d26_u53_nam_at_idx               */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u55_write_dnlist_attribute                        */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        22.09.89                                              */
/*                                                                    */
/* DESCRIPTION: This function writes a requested DNlist-attribute into*/
/*              message in IPC format.                                */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      filenr      Short           object file number                */
/*      object      D2_name_string  DN of read object                 */
/*      message     String          actual message pointer            */
/*      output      String *        pointer to beginning of message   */
/*      r_type      D2_ret_val      request type                      */
/*      at          D26_at *        pointer to AT-entry for attribute */
/*                                  to write                          */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of result message  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of result message  */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              String                      pointer to byte after     */
/*                                          written message.          */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d26_u55_write_dnlist_attribute(signed16 filenr,
    D2_name_string object, byte *message, byte **output, D2_ret_val r_type,
    D26_at *at, signed32 *remlen, signed32 *output_len)

{                               /*  d26_u55_write_dnlist_attribute    */

  signed16 pos;             /* position of attribute in record        */
  char     *record;         /* dummy-cisam-record                     */
  signed16 count;           /* count of DN-List-values                */

  register signed32 idx,idx1;   /* loop variables                     */

if                      /*  error from allocating memory for DN-List- */
			/*  indices                                   */
  ((record = (char *)D27_MALLOC(svc_c_sev_warning,
   at->d26a_field_pos[filenr] + sizeof(signed16) +
   sizeof(signed32) * (d26_maxidx_dnlist + 1))) == NULL)
{                       /*  set error apdu                            */
  d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,D2_REM_ERR,
			 D2_UNWILLING,output_len);
  return(NULL);
}
else
{                       /*  count DN-List-attribute values            */
  for                   /*  all DN-List entries                       */
     (idx = 0L, pos = at->d26a_field_pos[filenr] + sizeof(signed16),
      count = 0; idx <= d26_maxidx_dnlist; idx++)
  {                     /*  check interpretation and occurrency       */
    if                  /*  entry belongs to DN-List attribute        */
      (d26_dnlist[idx].d26_dnint != D2_COPY &&
       d26_dnlist[idx].d26_occurrency > 0L)
    {                   /*  compare with previous entries             */
      for               /*  all previous entries                      */
	 (idx1 = 0; idx1 < idx; idx1++)
      {                 /*  compare with current entry                */
	if              /*  entries match                             */
	  (d26_dnlist[idx1].d26_occurrency > 0L &&
	   d26_dnlist[idx1].d26_dnint != D2_COPY &&
	   d26_dnlist[idx1].d26_dnint != d26_dnlist[idx].d26_dnint &&
	   !strcmp((char *)d26_dnlist[idx1].d26_dnname,
	   (char *)d26_dnlist[idx].d26_dnname))
	{               /*  break loop                                */
	  break;
	}               
      }                 
      if                /*  no match was found                        */
	(idx1 == idx)
      {                 /*  put index into dummy record and count     */
	d26_i37_write_dummy_record(record,&pos,idx,sizeof(signed32));
	count++;
      }                 
    }                   
  }                     
  pos = at->d26a_field_pos[filenr];
  d26_i37_write_dummy_record(record,&pos,(signed32)count,sizeof(signed16));

  if                    /*  error from writing attribute              */
    ((message = d26_i51_write_single_attribute(filenr,record,
     at->d26a_field_pos[filenr],object,message,output,r_type,at,remlen,
     output_len)) == NULL)
  {                     /*  return error                              */
    free(record);
    return(NULL);
  }                     
  free(record);
}                       

			/*  return message pointer                    */
return(message);

}                       /*  d26_u55_write_dnlist_attribute            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u56_write_attribute_type                          */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        02.10.90                                              */
/*                                                                    */
/* DESCRIPTION: This function writes a requested attribute type and   */
/*              av_info structure into message in IPC-format.         */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      message     String          actual message pointer            */
/*      output      String *        pointer to beginning of message   */
/*      a_type      D2_a_type *     requested attribute type          */
/*      count       Short           count of attribute values         */
/*      repr        Short           ASN1-representation of attribute  */
/*                                  values                            */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of result message  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           length of result message          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              String                      pointer to byte after     */
/*                                          written message.          */
/*                                                                    */
/*exoff ***************************************************************/

byte *d26_u56_write_attribute_type(byte *message, byte **output,
    D2_a_type *a_type, signed16 count, signed16 repr, signed32 *remlen,
    signed32 *output_len)

{                               /*  d26_u56_write_attribute_type      */

  signed16 len;                 /* length of message to write         */
  signed16 offset;              /* offset for message pointer         */

len = sizeof(D23_at_type) + D2_ALIGN(a_type->d2_typ_len);
if                      /*  error from check buffer                   */
  ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
   == NULL)
{                       /*  return error                              */
  return(NULL);
}
else
{                       /*  write attribute type                      */
  ((D23_at_type *)message)->d23_atttag = D23_ATTTAG;
  ((D23_at_type *)message)->d23_atlen  = a_type->d2_typ_len;
  ((D23_at_type *)message)->d23_atoff  = offset =
					 D2_ALIGN(a_type->d2_typ_len);
  *remlen -= sizeof(D23_at_type);
  message += sizeof(D23_at_type);
  memcpy(message,a_type->d2_type,a_type->d2_typ_len);
  *remlen -= offset;
  message += offset;

  if                    /*  attribute values must be written          */
    (count > 0)
  {                     /*  check buffer again                        */
    len = sizeof(D23_av_info);
    if                  /*  error from check buffer                   */
      ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
       == NULL)
    {                   /*  return error                              */
      return(NULL);
    }
    else
    {                   /*  write attribute value information         */
      ((D23_av_info *)message)->d23_avtag  = D23_AVTAG;
      ((D23_av_info *)message)->d23_avnum  = (signed16)count;
      ((D23_av_info *)message)->d23_asyntx = (unsigned16)repr;
      *remlen -= len;
      message += len;
    }                   
  }                     
}                       
			/*  return message pointer                    */
return(message);

}                       /*  d26_u56_write_attribute_type              */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u65_get_superclasses                              */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        24.03.92                                              */
/*                                                                    */
/* DESCRIPTION: This function checks, whether ob_cl is in the list    */
/*              already. If not, it writes it into the list and       */
/*              calls itself recursively for all immediate            */
/*              superclasses.                                         */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      ob_cl       Long            object class to put into list     */
/*      ts_root     Void **         root for tsearch                  */
/*      ocl_list    Long **         list of object classes            */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      ts_root     Void **         root for tsearch                  */
/*      ocl_list    Long **         list of object classes            */
/*                                                                    */
/* RETURNVALUE: Void                                                  */
/*                                                                    */
/*exoff ***************************************************************/

void d26_u65_get_superclasses(signed32 ob_cl, void **ts_root,
    signed32 **ocl_list)

{                               /*  d26_u65_get_superclasses          */

  void **tsearch_result;    /* result returned by tsearch             */

  register signed16 *sup_cl;/* loop variable                          */

			/*  put object class into list                */
**ocl_list = ob_cl;

tsearch_result = D27_TSEARCH(svc_c_sev_warning,(void *)*ocl_list,ts_root,
  d26_u66_longcmp);
if                      /*  object class is new                       */
  (*tsearch_result == (void *)*ocl_list)
{                       /*  increment the object class list           */
  (*ocl_list)++;
  for                   /*  all superclasses                          */
     (sup_cl = d26_oct[ob_cl].d26c_sup_cl; *sup_cl != -1; sup_cl++)
  {                     /*  call function recursively                 */
    d26_u65_get_superclasses((signed32)*sup_cl,ts_root,ocl_list);
  }                     
}                       

}                       /*  d26_u65_get_superclasses                  */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_u66_longcmp()                                   */
/*                                                                    */
/* AUTHOR       : E. Kraemer, AP 11                                   */
/* DATE         : 24.03.92                                            */
/*                                                                    */
/* DESCRIPTION  : This functions compares two long integers l1 and l2.*/
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      l1      Void *          first integer to compare              */
/*      l2      Void *          second integer to compare             */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      -1      Int             first integer is smaller than second  */
/*       0                      both integers are equal               */
/*       1                      first integer is greater than second  */
/*                                                                    */
/**********************************************************************/

int d26_u66_longcmp(const void *l1, const void *l2)

{                           /*  d26_u66_longcmp                       */

if                          /*  first parameter is smaller than second*/
  (*(signed32 *)l1 < *(signed32 *)l2)
{                           /*  return -1                             */
  return(-1);
}
else if                     /*  first parameter is greater than second*/
     (*(signed32 *)l1 > *(signed32 *)l2)
{                           /*  return 1                              */
  return(1);
}
else
{                           /*  return 0                              */
  return(0);
}                           

}                           /*  end d26_u66_longcmp                   */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u71_make_attr_list                                */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        01.08.91                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the requested attributes into    */
/*              the attr_info structure array and sets the access     */
/*              permission flag.                                      */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      r_info      D23_rq_info *   information about required attr.  */
/*      dname       D2_name_string  distinguished name of found object*/
/*      acl         D26_acl *       Indices for acl on DN-list        */
/*      ob_nr       Short           object number for object          */
/*      ob_cl       Long *          object classes of object          */
/*      fct_tag     Short           indicates the calling function    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      attr        D26_attr_info * array of indices (and a flag for  */
/*                                  read permission for each requested*/
/*                                  attribute in the AT (the last one */
/*                                  will be "-1")                     */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      Short   D2_NO_ERR           all requested attributes are in   */
/*                                  the list.                         */
/*              D2_OBJ_CLS_VIOLATION    at least one requested        */
/*                                  attribute is not in the list.     */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u71_make_attr_list(D23_rq_info *r_info, D26_attr_info *attr,
    D2_name_string dname,  D26_acl *acl, signed16 ob_nr, signed32 *ob_cl,
    signed16 fct_tag)

{                               /*  d26_u71_make_attr_list            */

  D2_a_type   comp_type;    /* attribute type to compare with pseudo- */
			    /* attribute                              */
  D23_at_type *ra_type;     /* requested attribute type from message  */
  boolean     separate;     /* indicates whether cacheable and non-   */
			    /* cacheable attributes must be separated */
  signed16    ret_value = D2_NO_ERR;    /* return value               */

  register signed16 j;      /* count of attributes                    */

ra_type = (D23_at_type *)(r_info + 1);
if                      /*  all attributes required                   */
  (r_info->d23_rqnbr == D2_ALL_ATT_REQ)
{                       /*  reset service controls                    */
  d26_serv_contr->d23_scopt |= D2_DONT_STORE;
  separate = FALSE;
}
else if               /*  Attribute Type is following               */
       (ra_type->d23_atttag == D23_ATTTAG)
{                       /*  compare type                              */
  comp_type.d2_typ_len = ra_type->d23_atlen;
  comp_type.d2_type    = (Ob_id_string)(ra_type + 1);
  separate = D2_ATTCMP(&comp_type,D2_CACHE_ATTR) ? FALSE : TRUE;
}
else
{                       /*  don't separate attributes                 */
  separate = FALSE;
}                       

if                      /*  attributes must be separated              */
  (separate == TRUE)
{                       /*  look for cacheable attributes first       */
  j = d26_u52_attr_idx_mess(attr,ob_nr,ob_cl,TRUE,r_info,&ret_value);
  attr[j].d26_at_idx = attr[j].d26_srt_idx = D26_IDX_NOT_DEF;
  j++;
  j += d26_u52_attr_idx_mess(attr + j,ob_nr,ob_cl,FALSE,r_info,
			     &ret_value);
}
else
{                       /*  keep sequence of attributes               */
  if                    /*  all attributes required                   */
    (r_info->d23_rqnbr == D2_ALL_ATT_REQ)
  {                     /*  get all attributes from OCT and AT        */
    j = d26_u51_attr_idx_at(attr,ob_nr,ob_cl,fct_tag);
  }
  else
  {                     /*  get all attributes from message           */
    j = d26_u52_attr_idx_mess(attr,ob_nr,ob_cl,D2_IGNORE,r_info,&ret_value);
  }                     
}                       

attr[j].d26_at_idx = D26_EOL;

fct_tag = fct_tag > D2_ADM_OPID ? fct_tag - D2_ADM_OPID : fct_tag;
if                      /*  internal search was called by retrieval   */
			/*  function                                  */
  (fct_tag == D23_READ || fct_tag == D23_SEARCH)
{                       /*  check access rigths                       */
  d26_u05_acl_check(dname,D26_READ,attr,acl);
}
else
{                       /*  set all read flags as ok                  */
  for                   /*  all attributes to write                   */
     ( ; j >= 0; j--)
  {                     /*  set read flag ok                          */
    attr[j].d26_read_ok = TRUE;
  }                     
}                       

			/*  return                                    */
return(ret_value);

}                       /*  d26_u71_make_attr_list                    */
