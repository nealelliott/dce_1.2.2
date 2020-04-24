/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26wrut.c,v $
 * Revision 1.1.10.2  1996/02/18  18:15:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:37:44  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:49:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:22  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:46:08  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:35:58  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:53:01  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:26:27  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:13:39  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:24:17  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:40:43  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:22:05  marrek]
 * 
 * Revision 1.1.6.3  1993/08/16  12:31:45  marrek
 * 	Removing obsolete argument fct_tag in d26_u34_write_subordinate().
 * 	[1993/08/16  12:29:53  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  15:42:18  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:22:46  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  14:56:01  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.4  1993/02/02  11:31:45  marrek
 * 	Changes for OT6587
 * 	[1993/02/01  15:30:58  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:43:20  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:21:49  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:26:45  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:20:33  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:54:04  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:59:30  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26wrut.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:15:12 $";
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
/* NAME         : d26wrut.c       [d26wrut]                           */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 13.02.91                                            */
/*                                                                    */
/* COMPONENT    : DSA, retrieval functions                            */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   The module contains the functions, that write     */
/*                  IPC-structures into message.                      */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 13.02.91| birth                          | ek |         */
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
/*                D E C L A R A T I O N S        DATA                 */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** IMPORT ********/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u30_write_part_out_qual                           */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        04.10.90                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the partial outcome qualifier    */
/*              into message in IPC format.                           */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              char *      pointer to byte after written message.    */
/*                                                                    */
/*exoff ***************************************************************/

byte *d26_u30_write_part_out_qual(
    byte *message,      /* IN    -  actual message pointer            */
    byte **output,      /* INOUT -  pointer to beginning of message   */
    signed32 *remlen,   /* INOUT -  remaining length for message      */
			/* INOUT -  maximum length of result message  */
    signed32 *output_len)

{                       /*  d26_u30_write_part_out_qual               */

  signed16 limit_problem;   /* limit problem                          */
  signed16 len = 0;         /* message length to write                */

  if                    /* size limit is exceeded                     */
    (d26_serv_contr->d23_scslm != D2_S_UNLIMITED &&
     d26_entry_count > d26_serv_contr->d23_scslm)
  {                     /* set size limit problem                     */
    limit_problem = D2_SIZE_LIMIT_EX;
  }
  else if               /* time has run out                           */
	 (d26_time_ex == TRUE)
  {                     /* set time limit problem                     */
    limit_problem = D2_TIME_LIMIT_EX;
  }
  else if               /* too much referrals or database was closed  */
	 (d26_ref_count + 1 == D26_REF_BLOCKSIZE ||
	  d26_dbop_flag == D26_DB_NOOP)
  {                     /* set administrative limit exceeded          */
    limit_problem = D2_ADM_LIMIT_EX;
  }
  else
  {                     /* set no limit problem                       */
    limit_problem = D2_NO_LIMIT_EX;
  }

  if                    /* partial outcome qualifier must be written  */
    ((signed16)limit_problem != D2_NO_LIMIT_EX || d26_ref_present == TRUE)
  {                     /* do it                                      */
    len = sizeof(D23_part_oq);
    if                  /* error from check buffer                    */
      ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
       == NULL)
    {                   /* return error                               */
      return(NULL);
    }
    else
    {                   /* write partial outcome qualifier            */
      d26_start_p_o_q = message;
      ((D23_part_oq *)message)->d23_oqtag     = D23_OQTAG;
      ((D23_part_oq *)message)->d23_oq_uncrit = FALSE;
      ((D23_part_oq *)message)->d23_oq_limit  = limit_problem;
      *remlen -= len;
      message += len;
      if                /* referrals must be written                  */
	(d26_ref_present == TRUE)
      {                 /* do it                                      */
	if              /* error from writing referrals               */
	  ((message = d26_u48_write_referral_set(output,message,output_len,
	   remlen)) == NULL)
	{               /* return error                               */
	  return(NULL);
	}
      }
    }
  }

  return(message);

}                       /* d26_u30_write_part_out_qual               */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u34_write_subordinate                             */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        23.10.90                                              */
/*                                                                    */
/* DESCRIPTION: This function writes a subordinate structure from     */
/*              record into message in IPC format.                    */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      record      String          contains whole entry              */
/*      message     String          actual message pointer            */
/*      output      String *        pointer to beginning of message   */
/*      dninfo      D26_dninfo *    information about found object    */
/*      mk          Long            index of master DSA in DN-list    */
/*      ob_nr       Short           object number for object          */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of result message  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of result message  */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      != D2_NULL  String          pointer to byte after written     */
/*                                  message                           */
/*      D2_NULL                     in case of errors                 */
/*                                                                    */
/*exoff ***************************************************************/

byte *d26_u34_write_subordinate(char *record, byte *message, byte **output,
    D26_dninfo *dninfo, signed32 mk, signed16 ob_nr, signed32 *remlen,
    signed32 *output_len)

{                               /*  d26_u34_write_subordinate         */

  signed16 len;                 /* length of data to write            */

				/* memory for exact name              */
  byte     exact_dn[D2_DNL_MAX];
  signed16 dnlen;               /* length of exact name               */

len = sizeof(D23_subord_info);

if                      /*  error from check buffer                   */
  ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
   == NULL)
{                       /*  return error                              */
  return(NULL);
}
else
{                       /*  write ENT tag                             */
  ((D23_subord_info *)message)->d23_sotag = D23_SOITAG;
  ((D23_subord_info *)message)->d23_soali = d26_i47_get_al_references(
		      dninfo->d26_filenr,record) >= 0L ? TRUE : FALSE;
  ((D23_subord_info *)message)->d23_somas = mk == D26_OWNNAME ? TRUE :
								FALSE;

  *remlen -= len;
  message += len;

  dnlen = d26_i48_get_exact_name(record,dninfo,dninfo->d26_rd_anz - 1,
				 exact_dn);
  return(d26_u49_write_dist_name(message,output,exact_dn,dnlen,remlen,
	 output_len,D23_NAMTAG));
}                       

}                       /*  d26_u34_write_subordinate                 */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u37_write_entry                                   */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        08.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the entry from record      into  */
/*              message in IPC format.                                */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      record      String          contains whole entry              */
/*      output      String *        pointer to beginning of message   */
/*      dname       D2_name_string  normed DN of found object         */
/*      dninfo      D26_dninfo *    information about found object    */
/*      r_info      D23_rq_info *   information about required attr.  */
/*      attr        D26_attr_info * array of indices (and a flag for  */
/*                                  read permission for each requested*/
/*                                  attribute in the AT (the last one */
/*                                  will be "-1")                     */
/*      mk          Long            index of master DSA in DN-list    */
/*      ob_nr       Short           object number for object          */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of result message  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      message     String          result message to write           */
/*      output      String *        pointer to beginning of message   */
/*      recur       Bool *          indicates whether recurring       */
/*                                  attributes are supplied           */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of result message  */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              String   != D2_NULL         pointer to byte after     */
/*                                          written message.          */
/*                       D2_NULL            in case of errors         */
/*                                                                    */
/*exoff ***************************************************************/

byte *d26_u37_write_entry(char *record, byte *message, byte **output,
    D2_name_string dname, D26_dninfo *dninfo, D23_rq_info *r_info,
    boolean *recur, D26_attr_info *attr, signed32 mk, signed16 ob_nr,
    signed32 *remlen, signed32 *output_len)

{                               /*  d26_u37_write_entry               */

  signed16 len;                 /* length of data to write            */
				/* memory for exact name              */
  byte     exact_dn[D2_DNL_MAX];
  signed16 dnlen;               /* length of exact name               */

len = sizeof(D23_en_info);

if                      /*  error from check buffer                   */
  ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
   == NULL)
{                       /*  return error                              */
  return(NULL);
}
else
{                       /*  write ENT tag                             */
  ((D23_en_info *)message)->d23_entag   = D23_ENTTAG;
  ((D23_en_info *)message)->d23_enmas   = mk == D26_OWNNAME ? TRUE : FALSE;
  ((D23_en_info *)message)->d23_enmaxat =
    (signed16)((r_info->d23_rqnbr >= 0 ? r_info->d23_rqnbr : d26_noat) + 1);

  *remlen -= len;
  message += len;

  dnlen = d26_i48_get_exact_name(record,dninfo,0,exact_dn);
  if                    /*  error from write distinguished name       */
    ((message = d26_u49_write_dist_name(message,output,exact_dn,dnlen,remlen,
     output_len,D23_NAMTAG)) == NULL)
  {                     /*  return error                              */
    return(NULL);
  }
  else if               /*  error from write attributes               */
	 ((message = d26_u50_write_attributes(record,message,output,dname,
	  exact_dn,r_info,recur,attr,ob_nr,dninfo->d26_filenr,remlen,
	  output_len)) == NULL)
  {                     /*  return error                              */
    return(NULL);
  }
  else
  {                     /*  return message                            */
    return(message);
  }                     
}                       

}                       /*  d26_u37_write_entry                       */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u39_write_eom                                     */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        08.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the EOM tag into                 */
/*              message in IPC format.                                */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      message     String          actual message pointer            */
/*      remlen      Int *           remaining length for message      */
/*      output      String *        pointer to beginning of message   */
/*      output_len  Int *           maximum length of result message  */
/*      fct_tag     Short           indicates calling function        */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           length of completed result message*/
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              String                      pointer to byte after     */
/*                                          written message.          */
/*                                                                    */
/*exoff ***************************************************************/

byte *d26_u39_write_eom(byte *message, signed32 *remlen, byte **output,
    signed32 *output_len, signed16 fct_tag)

{                               /*  d26_u39_write_eom                 */

  signed16 len = 0;         /* message length to write                */

if                      /*  internal search was called by retrieval   */
			/*  function                                  */
  (fct_tag == D23_READ || fct_tag == D23_COMPARE || fct_tag == D23_LIST ||
   fct_tag == D23_SEARCH)
{                       /*  check buffer                              */
  len = sizeof(D23_com_res);
  if                    /*  error from check buffer                   */
    ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
     == NULL)
  {                     /*  return error                              */
    return(NULL);
  }
  else
  {                     /*  write commmon results                     */
    ((D23_com_res *)message)->d23_crtag     = D23_CRSTAG;
    ((D23_com_res *)message)->d23_ali_deref = d26_al_rdns > 0 &&
	  (d26_serv_contr->d23_scopt & D2_DREFALIAS) == 0L ?
	  TRUE : FALSE;
    *remlen -= len;
    message += len;
  }                     
}                       

len = sizeof(D23_eom);

if                      /*  error from check buffer                   */
  ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
   == NULL)
{                       /*  return error                              */
  return(NULL);
}
else
{                       /*  write EOM tag                             */
  ((D23_eom *)message)->d23_eomtag    = D23_EOMTAG;
  ((D23_eom *)message)->d23_eom_oqoff = d26_start_p_o_q == NULL ?
			0L : (signed32)(message - d26_start_p_o_q + len);
  *remlen -= len;
  return(message);
}                       

}                       /*  d26_u39_write_eom                         */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u40_check_buffer                                  */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        09.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function checks, whether the IPC buffer is large */
/*              enough, to contain len more bytes. If not, the buffer */
/*              will be written to IPC. A pointer for to write the    */
/*              message will be returned.                             */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              len         Short           count of bytes to write   */
/*              output_len  Int *           maximum length of result  */
/*              remlen      Int *           remaining length for      */
/*                                          result message            */
/*              output      String *        pointer to beginning of   */
/*                                          result message            */
/*              message     String          actual pointer to result  */
/*                                          message to write          */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              output_len  Int *           maximum length of result  */
/*              remlen      Int *           remaining length for      */
/*                                          result message            */
/*              output      String *        pointer to beginning of   */
/*                                          result message            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              String      > D2_NULL       pointer to where the      */
/*                                          message may be written    */
/*                          D2_NULL         in case of errors         */
/*                                                                    */
/*exoff ***************************************************************/

byte *d26_u40_check_buffer(signed16 len, signed32 *output_len,
    signed32 *remlen, byte **output, byte *message)

{                               /*  d26_u40_check_buffer              */

  signed32 poq_offset;          /* offset of partial outcome qualifier*/
				/* from the beginning of the message  */
  byte     *new_output;         /* holds address of reallocated output*/

if                      /*  block length too small                    */
  (D26_BLOCK_LEN < (signed32)len)
{                       /*  return error                              */
  return(NULL);
}
else if                 /*  message block is full                     */
       (*remlen < (signed32)len)
{                       /*  check IPC flag                            */
  poq_offset = d26_start_p_o_q - *output;
  if                    /*  error from reallocating                   */
    ((new_output = (byte *)D27_REALLOC(svc_c_sev_warning,*output,
     *output_len + D26_BLOCK_LEN)) == NULL)
  {                     /*  return error                              */
    return(NULL);
  }
  else
  {                     /*  reset message and length                  */
    *output      = new_output;
    *remlen     += D26_BLOCK_LEN;
    *output_len += D26_BLOCK_LEN;
    d26_start_p_o_q = d26_start_p_o_q == NULL ? NULL :
		      *output + poq_offset;
    return(*output + *output_len - *remlen);
  }                     
}
else
{                       /*  return old message                        */
  return(message);
}                       

}                       /*  d26_u40_check_buffer                      */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u44_write_child                                   */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        11.02.91                                              */
/*                                                                    */
/* DESCRIPTION: This function writes a subordinate of the base object */
/*              either in list or in search format into message, after*/
/*              filter attributes are checked.                        */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      message     String          actual message pointer            */
/*      output      String *        pointer to beginning of message   */
/*      dname       D2_name_string  DN of base object                 */
/*      dninfo      D26_dninfo *    information about base object     */
/*      np_count    Short           count of nameparts in base object */
/*      tail        D2_name_string  End of DN of base object          */
/*      ob_nr       Short           object number                     */
/*      r_info      D23_rq_info *   information about required attrs. */
/*      f_info      String          information about filter attrs.   */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of result message  */
/*      fct_tag     Short           indicates the calling function    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of result message  */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      != D2_NULL          String          pointer to byte after     */
/*                                          written message           */
/*         D2_NULL                          error occurred            */
/*                                                                    */
/*exoff ***************************************************************/

byte *d26_u44_write_child(byte *message, byte **output, D2_name_string dname,
    D26_dninfo *dninfo, signed16 np_count, D2_name_string tail,
    signed16 ob_nr, D23_rq_info *r_info, D23_ft1_info *f_info,
    signed32 *remlen, signed32 *output_len, signed16 fct_tag)

{                               /*  d26_u44_write_child               */

				/* object classes of read object      */
  signed32       ob_cl[D26_OCL_VAL_COUNT + 1];
				/* indices into DN list for ACL       */
  D26_acl        acl[D2_ACLIDX_MAX];
				/* array for filter attribute         */
  D26_attr_info  at_info[D2_AT_MAX + 1];    /* information            */
				/* index into DN list for master      */
  signed32       mk;            /* knowledge                          */
			    /* dummy space for write entry to write ..*/
  boolean        recur;         /* ... a recurring flag               */
  D23_fi1_info   *f_err_item;   /* ... a filter item                  */

d26_i34_get_dninfo(d26_ob_record,dninfo,&tail,np_count);
DCE_SVC_LOG((GDS_S_ENTRY_FOUND_MSG,dname));
d26_i36_read_ocl_acl_mk(dninfo,d26_ob_record,ob_cl,acl,&mk);
if                      /*  DSA is master or copies are allowed       */
  (mk == D26_OWNNAME || !(d26_serv_contr->d23_scopt & D2_NOCACHE))
{                       /*  get DN from record                        */
  switch(fct_tag > D2_ADM_OPID ? fct_tag - D2_ADM_OPID : fct_tag)
  {
    case D23_LIST:
      d26_entry_count++;
      if                /*  size limit not yet exceeded               */
	(d26_serv_contr->d23_scslm == D2_S_UNLIMITED ||
	 d26_entry_count <= d26_serv_contr->d23_scslm)
      {                 /*  write the entry into message              */
	if              /*  error from write subordinate              */
	  ((message = d26_u34_write_subordinate(d26_ob_record,message,
	   output,dninfo,mk,ob_nr,remlen,output_len)) == NULL)
	{               /*  return error                              */
	  return(NULL);
	}
      }
      break;
    case D23_SEARCH:
    case D23_REMOBJT:
    case D23_REPLRDN:
      d26_f20_calc_fat_info(ob_nr,ob_cl,f_info,at_info);
      if                    /*  no error from filter attribute check  */
	(d26_f19_filter_attr_check(d26_ob_record,dninfo->d26_filenr,ob_nr,
	 dname,at_info,acl,f_info,&f_err_item,fct_tag,FALSE)
	 == D2_NO_ERR)
      {                     /*  check whether aliases must be         */
			    /*  dereferenced                          */
	if                  /*  aliases must not be dereferenced or   */
			    /*  object is not alias                   */
	  (d26_search_arg->d23_Iali == FALSE || *ob_cl != d26_ali_idx)
	{                   /*  make attribute list                   */
	  d26_u71_make_attr_list(r_info,d26_attr,dname,acl,ob_nr,ob_cl,
				 fct_tag);
	  d26_entry_count++;
	  if            /*  size limit not yet exceeded               */
	    (d26_serv_contr->d23_scslm == D2_S_UNLIMITED ||
	     d26_entry_count <= d26_serv_contr->d23_scslm)
	  {             /*  write the entry into message              */
	    if          /*  error from write entry                    */
	      ((message = d26_u37_write_entry(d26_ob_record,message,output,
	       dname,dninfo,r_info,&recur,d26_attr,mk,ob_nr,remlen,
	       output_len)) == NULL)
	    {               /*  return error                          */
	      return(NULL);
	    }
	  }
	}
      }
      break;
  }   
}                       

return(message);

}                       /*  d26_u44_write_child                       */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u45_write_match_flag                              */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        09.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the match flag for compare into  */
/*              message in IPC format.                                */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      message     String          actual message pointer            */
/*      output      String          pointer to beginning of message   */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of message         */
/*      matchflag   Short           indicates whether filter has      */
/*                                  matched                           */
/*      mk          Long            index of master DSA in DN-list    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of message         */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              String                      pointer to byte after     */
/*                                          written message.          */
/*                       D2_NULL            in case of errors         */
/*                                                                    */
/*exoff ***************************************************************/

byte *d26_u45_write_match_flag(byte *message, byte **output,
    signed32 *remlen, signed32 *output_len, signed16 matchflag, signed32 mk)

{                               /*  d26_u45_write_match_flag          */

  signed16 len = 0;         /* length of data to write                */

len = sizeof(D23_rscompinfo);

if                      /*  error from check buffer                   */
  ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
   == NULL)
{                       /*  return error                              */
  return(NULL);
}
else
{                       /*  write matchflag                           */
  ((D23_rscompinfo *)message)->d23_Ltag    = D23_LTAG;
  ((D23_rscompinfo *)message)->d23_Lmatsgn = matchflag == D2_NO_ERR ?
					    TRUE : FALSE;
  ((D23_rscompinfo *)message)->d23_Lfrmen  = mk == D26_OWNNAME ?
					    TRUE : FALSE;
  *remlen -= len;
  message += len;
  return(message);
}                       

}                       /*  d26_u45_write_match_flag                  */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u49_write_dist_name                               */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        09.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the DN dname               into  */
/*              message in IPC format.                                */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      message     String          result message to write           */
/*      output      String *        Begin of result message           */
/*      dname       D2_name_string  DN of object found                */
/*      dnlen       Ushort          length of DN of object found      */
/*      remlen      Int *           remaining length for result       */
/*                                  message                           */
/*      output_len  Int *           maximum length of result          */
/*      dn_tag      Ushort          indicates kind of DN to write     */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        Begin of result message           */
/*      remlen      Int *           remaining length for result       */
/*                                  message                           */
/*      output_len  Int *           maximum length of result          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      != D2_NULL  String          pointer to byte after written     */
/*                                  message                           */
/*      D2_NULL                     in case of errors                 */
/*                                                                    */
/*exoff ***************************************************************/

byte *d26_u49_write_dist_name(byte *message, byte **output,
    D2_name_string dname, signed16 dnlen, signed32 *remlen,
    signed32 *output_len, unsigned16 dn_tag)

{                               /*  d26_u49_write_dist_name           */

  signed16 len = 0;         /* length of data to write                */

len = sizeof(D23_ds_name) + D2_ALIGN(dnlen);

if                      /*  error from check buffer                   */
  ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
   == NULL)
{                       /*  return error                              */
  return(NULL);
}
else
{                       /*  set pointer to head                       */
  ((D23_ds_name *)message)->d23_dstag = dn_tag;
  ((D23_ds_name *)message)->d23_dscomplete = TRUE;
  ((D23_ds_name *)message)->d23_dslen = dnlen;
  ((D23_ds_name *)message)->d23_dsoff = len = D2_ALIGN(dnlen);
  *remlen -= sizeof(D23_ds_name);
  message += sizeof(D23_ds_name);
  strcpy((char *)message,(char *)dname);
  message += len;
  *remlen -= len;
  return(message);
}                       

}                       /*  d26_u49_write_dist_name                   */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u54_write_chaining_results                        */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        21.03.90                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the chaining results into        */
/*              message in IPC format.                                */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      message     String          actual message pointer            */
/*      output      String *        pointer to beginning of message   */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of message         */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      output_len  Int *           maximum length of message         */
/*      remlen      Int *           remaining length for message      */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              String                      pointer to byte after     */
/*                                          written message.          */
/*                       D2_NULL            in case of errors         */
/*                                                                    */
/*exoff ***************************************************************/

byte *d26_u54_write_chaining_results(byte *message, byte **output,
    signed32 *remlen, signed32 *output_len)

{                               /*  d26_u54_write_chaining_results    */

  signed16 len = 0;         /* length of data to write                */

len = sizeof(D23_chain_res);

if                      /*  error from check buffer                   */
  ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
   == NULL)
{                       /*  return error                              */
  return(NULL);
}
else
{                       /*  write chaining result tag                 */
  ((D23_chain_res *)message)->d23_chrtag   = D23_CHRTAG;
  ((D23_chain_res *)message)->d23_max_cref = 0;
  *remlen -= len;
  message += len;
  return(message);
}                       

}                       /*  d26_u54_write_chaining_results            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u84_send_abandoned                                */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        31.01.94                                              */
/*                                                                    */
/* DESCRIPTION: This function sends the abandoned error for retrieval */
/*              operations.                                           */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   signed16            No error occurred             */
/*      D26_ERROR                       error occurred                */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u84_send_abandoned(
    signed16   op_id,   /* IN    -  operation ID                      */
    byte       **output,/* INOUT -  pointer to beginning of message   */
    signed32   *remlen, /* OUT   -  remaining length of message       */
			/* INOUT -  maximum length of message         */
    signed32   *output_len,
    D23_rsrqpb *rsrqpb) /* IN    -  parameters for d23_result         */

{                               /*  d26_u84_send_abandoned            */

  signed32 meslen;              /* length of written message          */

  d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,D2_REM_ERR,
    D2_ABANDONED,&meslen);
  *remlen = *output_len - meslen;

  if                    /* message cannot be sent                     */
    (d26_m01_ret_result(op_id,0,*output,*output_len - *remlen,rsrqpb,
     D23_RERROR) != D2_NO_ERR)
  {                     /* return error                               */
    return(D26_ERROR);
  }

  return(D2_NO_ERR);

}                       /* d26_u84_send_abandoned                     */
