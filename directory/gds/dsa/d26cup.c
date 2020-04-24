/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26cup.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:04  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:43:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:25  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:52  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:19:57  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:05:21  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:09:45  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  13:44:44  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:03:25  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:22:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:18:40  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:36:37  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:05:21  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:46:27  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:53:54  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26cup.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:56 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYP          : MODUL                                               */
/*                                                                    */
/* NAME         : d26cup.c    [d27cup]                                */
/*                                                                    */
/* AUTOR        : E. Kraemer, D AP 11                                 */
/* DATUM        : 04.05.88                                            */
/*                                                                    */
/* KOMPONENTE   : CACHE update                                        */
/*                                                                    */
/* DOK.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* BESCHREIBUNG : the module contains the functions, which are used   */
/*                by client stub and local DSA to update the cache    */
/*                                                                    */
/* SYSTEMABHAENGIGKEIT:                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORIE     :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 04.05.88| birth                          | ek |         */
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
#include <d23ipc.h>
#include <d23apdu.h>

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

/******** IMPORT ********/

/******** EXPORT ********/

/******** LOCAL  ********/

			/* pointer to attribute type structure in     */
			/* client message                             */
static    byte       *d26_cl_type;

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/

static byte *d26_h02_find_tag(byte *message, signed16 op_id,
    D2_name_string deref_obj, D2_name_string base_object);
static void d26_h03_add_en(signed16 dir_id, D23_serv_ctrl *serv_ctr,
    D2_name_string deref_obj, byte *smessage, D23_inrqpb *input,
    signed16 memlen, byte **cmessage, signed16 *clen);
static signed16 d26_h04_add_comp_en(signed16 dir_id, D23_serv_ctrl *serv_ctr,
    D2_name_string deref_obj, D23_inrqpb *input, signed16 memlen,
    byte **cmessage, signed16 *clen);
static byte *d26_h10_check_buffer(signed16 len, signed16 *remlen,
    D23_inrqpb *input, signed16 memlen, byte *message);
static byte *d26_h11_write_add_head(signed16 dir_id, D23_serv_ctrl *serv_ctr,
    D23_inrqpb *input, signed16 memlen, byte *message, signed16 *remlen);
static byte *d26_h12_write_add_dn(D2_name_string deref_obj, byte **smessage,
    D23_inrqpb *input, signed16 memlen, byte *message, signed16 *remlen,
    boolean *ignore);
static byte *d26_h13_write_add_type(byte **smessage, D23_inrqpb *input,
    signed16 memlen, byte *message, signed16 *remlen, boolean *ignore);
static byte *d26_h14_write_add_avinfo(byte **smessage, D23_inrqpb *input,
    signed16 memlen, byte *message, signed16 *remlen, boolean ignore);
static byte *d26_h15_write_add_avvalue(byte **smessage, D23_inrqpb *input,
    signed16 memlen, byte *message, signed16 *remlen, boolean ignore);
static signed16 d26_h21_send_skip(byte **smessage, D23_inrqpb *input,
    signed16 memlen, byte **message, signed16 *remlen, unsigned16 tag,
    signed16 dir_id, D23_serv_ctrl *serv_ctr);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_h01_cache_update                                */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 04.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function writes all objects contained in result */
/*                into cache.                                         */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      result        String            complete IPC result message   */
/*      op_id         Short             indicates calling function    */
/*      dir_id        Ushort            directory id                  */
/*      serv_ctr      D23_serv_ctrl *   service controls              */
/*      base_object   D2_name_string *  base object of operation      */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*                                                                    */
/* RETURNVALUE     : None                                             */
/*                                                                    */
/*exoff ***************************************************************/

void d26_h01_cache_update(byte *result, signed16 op_id, signed16 dir_id,
    D23_serv_ctrl *serv_ctr, D2_name_string base_object)

{                /*  d26_h01_cache_update                             */

			    /* parameters for ipc bind                */
static  D23_bindpb  bindpb = {{D23_V02,0,0,0},D23_DCASID};

			    /* parameters for invoke                  */
static  D23_inrqpb  inrqpb = {{D23_V02,0,0,0},0,{0,0,0},D23_ADDOBJT};

			    /* parameters for unbind                  */
static  D23_ubndpb  ubndpb = {{D23_V02,0,0,0}};

			    /* memory length for cache invokes        */
static  signed16    memlen;

			    /* remaining memory length for invokes    */
static  signed16    cmemlen;

			    /* dereferenced base object of request    */
static  byte        deref_obj[D2_DNL_MAX + 1];

			    /* pointer to cache update message        */
byte *cmessage;

if              /*  cache update must not be performed                */
  (serv_ctr->d23_scopt & D2_DONT_STORE || (op_id != D23_READ &&
   op_id != D23_COMPARE && op_id != D23_LIST && op_id != D23_SEARCH))
{               /*  return immediately                                */
  return;
}
else if         /*  decoding of message allows no cache update        */
       ((result = d26_h02_find_tag(result,op_id,deref_obj,base_object))
	== (byte *)NULL)
{               /*  just return                                       */
  return;
}
else if         /*  error from bind                                   */
       (d23_bind(&bindpb) == D23_ERROR)
{               /*  just return                                       */
  return;
}
else
{               /*  set invoke and unbind parameters                  */
  ubndpb.d23_5assid  = inrqpb.d23_6assid = bindpb.d23_4assid;
  inrqpb.d23_6invinfo.d23_invid  = D23_UNUSED;
  inrqpb.d23_6invinfo.d23_contid = D23_SAC(D27_DAP_AC) |
    D23_SACM(D27_LOC_AC) | D23_STS(D27_PRIV_TS);
  inrqpb.d23_6invinfo.d23_usrid  = inrqpb.d23_6assid;
  inrqpb.d23_6rfidt  = cmessage = bindpb.d23_4refms;
  memlen             = cmemlen  = bindpb.d23_4lenms;
  switch(op_id)         /*  check calling function                    */
  {
    case D23_COMPARE:
      if                /*  error from handling compare result        */
	(d26_h04_add_comp_en(dir_id,serv_ctr,deref_obj,&inrqpb,memlen,
	 &cmessage,&cmemlen) != D2_NO_ERR)
      {                 /*  don't add the object                      */
	break;
      }                 
			/*  go into default case otherwise            */
    default:
      d26_h03_add_en(dir_id,serv_ctr,deref_obj,result,&inrqpb,memlen,
		     &cmessage,&cmemlen);
      break;
  }   
  d23_unbind(&ubndpb);
}               

}               /*  d26_h01_cache_update                              */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_h02_find_tag                                    */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 06.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function skips all information from the         */
/*                beginning of message, which is useless for cache.   */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      message       String            message to scan               */
/*      op_id         Short             indicates calling function    */
/*      base_object   D2_name_string    base object of operation      */
/*                                                                    */
/* OUTPUT-PARAMETER: None                                             */
/*      deref_obj     D2_name_string    dereferenced base object of   */
/*                                      operation                     */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      String   != D2_NULL     pointer to the interesting part of    */
/*                              the message.                          */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d26_h02_find_tag(byte *message, signed16 op_id,
    D2_name_string deref_obj, D2_name_string base_object)

{                /*  d26_h02_find_tag                                 */

  signed16 offset;      /* size to move message pointer               */
  Bool     complete;    /* indicates whether DN is completely decoded */

switch(op_id)
{
  case D23_COMPARE:
    message += sizeof(D23_rscompinfo);
    if(((D23_ds_name *)message)->d23_dstag == D23_NAMTAG)
    {   
      if(((D23_ds_name *)message)->d23_dscomplete == FALSE)
      {   
	return((byte *)NULL);
      }
      else
      {
	offset = ((D23_ds_name *)message)->d23_dsoff;
	message += sizeof(D23_ds_name);
	strcpy((char *)deref_obj,(char *)message);
	message += offset;
      } 
    }
    else
    {
      strcpy((char *)deref_obj,(char *)base_object);
    } 
    break;
  case D23_LIST:
    if(((D23_ds_name *)message)->d23_dstag == D23_BONTAG)
    {   
      if(((D23_ds_name *)message)->d23_dscomplete == FALSE)
      {   
	return((byte *)NULL);
      }
      else
      {
	offset = ((D23_ds_name *)message)->d23_dsoff;
	message += sizeof(D23_ds_name);
	strcpy((char *)deref_obj,(char *)message);
	message += offset;
      } 
    }
    else
    {
      strcpy((char *)deref_obj,(char *)base_object);
    } 
    break;
  case D23_SEARCH:
    if(((D23_ds_name *)message)->d23_dstag == D23_BONTAG)
    {   
      complete = ((D23_ds_name *)message)->d23_dscomplete;
      offset = ((D23_ds_name *)message)->d23_dsoff;
      message += sizeof(D23_ds_name) + offset;
      if(complete == FALSE)
      {   
	offset = ((D23_av_value *)message)->d23_avoff;
	message += sizeof(D23_av_value) + offset;
      } 
    } 
    deref_obj[0] = D2_EOS;
    break;
  case D23_READ:
    deref_obj[0] = D2_EOS;
    break;
}   

		/*  return message                                    */
return(message);

}               /*  d26_h02_find_tag                                  */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_h03_add_en                                      */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 09.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function reads an entry information from the    */
/*                smessage and performs the add object to cache.      */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      dir_id      Ushort              directory ID                  */
/*      serv_ctr    D23_serv_ctrl *     service controls              */
/*      deref_obj   D2_name_string      dereferenced base object of   */
/*                                      operation                     */
/*      smessage    String              message to scan               */
/*      input       D23_inrqpb *        parameters for invoke         */
/*      memlen      Ushort              length of memory for message  */
/*                                      to write                      */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      input       D23_inrqpb *        parameters for invoke         */
/*      cmessage    String *            message to write              */
/*      clen        Ushort *            maximum length of this message*/
/*                                                                    */
/* RETURNVALUE     : None                                             */
/*      >= 0        Short               remaining length of attribute */
/*                                      value expected from next call.*/
/*       < 0                            error occurred                */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_h03_add_en(signed16 dir_id, D23_serv_ctrl *serv_ctr,
    D2_name_string deref_obj, byte *smessage, D23_inrqpb *input,
    signed16 memlen, byte **cmessage, signed16 *clen)

{                /*  d26_h03_add_en                                   */

  unsigned16 tag;         /* scanned message tag                      */

			  /* indicates whether attributes must be     */
  boolean  ignore = FALSE;/* ignored                                  */

do
{               /*  check tag                                         */
  tag = *(unsigned16 *)smessage;
  switch(tag)
  {
    case D23_ENTTAG:
      d26_cl_type = NULL;
    case D23_SOITAG:
    case D23_OQTAG:
    case D23_CRSTAG:
      if(d26_h21_send_skip(&smessage,input,memlen,cmessage,clen,tag,dir_id,
	 serv_ctr) < 0)
      {   
	return;
      }
      else
      {
	ignore = FALSE;
      } 
      break;
    case D23_NAMTAG:
      if((*cmessage = d26_h12_write_add_dn(deref_obj,&smessage,input,memlen,
	 *cmessage,clen,&ignore)) == NULL)
      {   
	return;
      } 
      break;
    case D23_ATTTAG:
      if(d26_cl_type != NULL)
      {   
	*cmessage = d26_cl_type;
      } 
      if((*cmessage = d26_h13_write_add_type(&smessage,input,memlen,
	 *cmessage,clen,&ignore)) == NULL)
      {   
	return;
      } 
      break;
    case D23_AVTAG:
      d26_cl_type = NULL;
      if((*cmessage = d26_h14_write_add_avinfo(&smessage,input,memlen,
	 *cmessage,clen,ignore)) == NULL)
      {   
	return;
      } 
      break;
    default:
      if((*cmessage = d26_h15_write_add_avvalue(&smessage,input,memlen,
	 *cmessage,clen,ignore)) == NULL)
      {   
	return;
      } 
      break;
  }   
}               

while           /*  still useful information present                  */
     (tag != D23_OQTAG && tag != D23_CRSTAG);

return;

}               /*  d26_h03_add_en                                    */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_h04_add_comp_en                                 */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 09.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function performs the add object to cache for   */
/*                the compared object from compare.                   */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      dir_id      Ushort              directory ID                  */
/*      serv_ctr    D23_serv_ctrl *     service controls              */
/*      deref_obj   D2_name_string      dereferenced base object of   */
/*                                      operation                     */
/*      input       D23_inrqpb *        parameters for invoke         */
/*      memlen      Ushort              length of memory for message  */
/*                                      to write                      */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      input       D23_inrqpb *        parameters for invoke         */
/*      cmessage    String *            message to write              */
/*      clen        Ushort *            maximum length of this message*/
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*        0         Short               add to cache performed        */
/*                                      successfully                  */
/*      < 0                             error occurred                */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_h04_add_comp_en(signed16 dir_id, D23_serv_ctrl *serv_ctr,
    D2_name_string deref_obj, D23_inrqpb *input, signed16 memlen,
    byte **cmessage, signed16 *clen)

{                /*  d26_h04_add_comp_en                              */

  signed16  len;        /* length of part in current block            */
  signed16  offset;     /* offset to next apdu structure              */

if              /*  error from writing header for add message         */
  ((*cmessage = d26_h11_write_add_head(dir_id,serv_ctr,input,memlen,
   *cmessage,clen)) == NULL)
{   
  return(-1);
}
else
{               
  len    = strlen((char *)deref_obj) + 1;
  offset = D2_ALIGN(len);
  if                    /*  error from check buffer                   */
    ((*cmessage = d26_h10_check_buffer(sizeof(D23_ds_name) + offset,clen,
     input,memlen,*cmessage)) == NULL)
  {                     /*  return error                              */
    return(-1);
  }
  else
  {                     /*  write distinguished name header           */
    ((D23_ds_name *)*cmessage)->d23_dstag = D23_NAMTAG;
    ((D23_ds_name *)*cmessage)->d23_dscomplete = TRUE;
    ((D23_ds_name *)*cmessage)->d23_dslen = len;
    ((D23_ds_name *)*cmessage)->d23_dsoff = offset;
  }                     
  *clen     -= sizeof(D23_ds_name);
  *cmessage += sizeof(D23_ds_name);
  memcpy(*cmessage,deref_obj,len);
  *clen     -= offset;
  *cmessage += offset;
} 

return(D2_NO_ERR);

}               /*  d26_h04_add_comp_en                               */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_h10_check_buffer                                  */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        09.05.88                                              */
/*                                                                    */
/* DESCRIPTION: This function checks, whether the IPC buffer is large */
/*              enough, to contain len more bytes. If not, the buffer */
/*              will be written to IPC. A pointer for to write the    */
/*              message will be returned.                             */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      Ushort      len             count of bytes to write           */
/*      remlen      Ushort *        remaining length for invoke       */
/*                                  message                           */
/*      input       D23_inrqpb *    Parameters for invoke             */
/*      memlen      Ushort *        maximum length for invoke message */
/*      message     String          invoke message to write           */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      remlen      Ushort *        remaining length for invoke       */
/*                                  message                           */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      != D2_NULL  String          pointer to where the message may  */
/*                                  be written                        */
/*      D2_NULL                     in case of errors                 */
/*                                                                    */
/*exoff ***************************************************************/

static byte *d26_h10_check_buffer(signed16 len, signed16 *remlen,
    D23_inrqpb *input, signed16 memlen, byte *message)

{                               /*  d26_h10_check_buffer              */

if                      /*  block length too small                    */
  (memlen < len)
{                       /*  return error                              */
  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,svc_c_debug1,
    GDS_S_NOMEMORY,len));
  return(NULL);
}
else if                 /*  message block is full                     */
       (*remlen < len)
{                       /*  invoke IPC                                */
  input->d23_6imdat  = D23_MRDATA;
  input->d23_6lnidt  = memlen - *remlen;
  if                    /*  error from ipc invoke                     */
    (d23_invoke(input) == D23_ERROR)
  {                     /*  write error log                           */
    return(NULL);
  }
  else
  {                     /*  reset message and length                  */
    *remlen  = memlen;
    return(input->d23_6rfidt);
  }                     
}
else
{                       /*  return old message                        */
  return(message);
}                       

}                       /*  d26_h10_check_buffer                      */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_h11_write_add_head                              */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 09.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function reads an entry information from the    */
/*                smessage and performs the add object to cache.      */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      dir_id      Ushort              directory ID                  */
/*      serv_ctr    D23_serv_ctrl *     service controls              */
/*      input       D23_inrqpb *        parameters for invoke         */
/*      memlen      Ushort *            maximum length of this message*/
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      input       D23_inrqpb *        parameters for invoke         */
/*      message     String *            message to write              */
/*      remlen      Ushort *            maximum length of this message*/
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      != D2_NULL  String              pointer to byte after message */
/*                                      written.                      */
/*         D2_NULL                      error occurred                */
/*                                                                    */
/*inoff ***************************************************************/


static byte *d26_h11_write_add_head(signed16 dir_id, D23_serv_ctrl *serv_ctr,
    D23_inrqpb *input, signed16 memlen, byte *message, signed16 *remlen)

{                /*  d26_h11_write_add_head                           */

  signed16 len = 0;             /* length of data to write            */

len = sizeof(D23_inaddobjt);

if                      /*  error from check buffer                   */
  ((message = d26_h10_check_buffer(len,remlen,input,memlen,
				   message)) == NULL)
{                       /*  return error                              */
  return(NULL);
}
else
{                       /*  write add object header                   */
  ((D23_inaddobjt *)message)->d23_Mhdr.d23_versno = D23_APDUV22;
  ((D23_inaddobjt *)message)->d23_Mhdr.d23_dirid  = dir_id;
  *remlen -= len;
  message += len;

			/*  write common arguments                    */
  len = sizeof(D23_com_arg);
  if                    /*  error from check buffer                   */
    ((message = d26_h10_check_buffer(len,remlen,input,memlen,
				     message)) == NULL)
  {                     /*  return error                              */
    return(NULL);
  }
  else
  {                     /*  write common arguments                    */
    ((D23_com_arg *)message)->d23_cmtag = D23_CMTAG;
    ((D23_com_arg *)message)->d23_cmali = D2_NOT_DEFINED;
    ((D23_com_arg *)message)->d23_cmctr = *serv_ctr;
    ((D23_com_arg *)message)->d23_cmopr.d23_opnm_rs   = D2_NOT_STARTED;
    ((D23_com_arg *)message)->d23_cmopr.d23_op_nxtrdn = 0;
    *remlen -= len;
    message += len;
  }             
}               

		/*  return no error                                   */
return(message);

}               /*  d26_h11_write_add_head                            */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_h12_write_add_dn                                */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 09.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function reads an distinguished name from the   */
/*                smessage and writes it into the cmessage for add    */
/*                object to cache.                                    */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      deref_obj D2_name_string      dereferenced base object of     */
/*                                    operation                       */
/*      smessage  String *            server message to read          */
/*      input     D23_inrqpb *        parameters for invoke           */
/*      memlen    Ushort *            maximum length of this message  */
/*      ignore    Bool *              indicates that DN has to be     */
/*                                    ignored                         */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      smessage  String *            server message to read          */
/*      input     D23_inrqpb *        parameters for invoke           */
/*      message   String *            message to write                */
/*      remlen    Ushort *            maximum length of this message  */
/*      ignore    Bool *              indicates that DN has to be     */
/*                                    ignored                         */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      != D2_NULL           String             pointer to byte after */
/*                                              message written.      */
/*         D2_NULL                              error occurred        */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d26_h12_write_add_dn(D2_name_string deref_obj, byte **smessage,
    D23_inrqpb *input, signed16 memlen, byte *message, signed16 *remlen,
    boolean *ignore)

{                /*  d26_h12_write_add_dn                             */

  signed16 len = 0;             /* length of data to write            */
  signed16 s_offset;            /* offset to next data part in server */
				/* message                            */
  Bool     complete;            /* indicates whether DN is decoded    */
				/* completely                         */
  signed16 offset;              /* offset to next data part           */
  byte     cat_object[D2_DNL_MAX + 1];  /* memory to write DN         */
  D2_name_string object;        /* pointer to DN to write             */

s_offset = ((D23_ds_name *)*smessage)->d23_dsoff;
complete = ((D23_ds_name *)*smessage)->d23_dscomplete;
if                      /*  DN note decoded completely                */
  (complete == FALSE)
{                       /*  reset ignore flag and message pointer     */
  *ignore = TRUE;
  message = input->d23_6rfidt;
  *remlen = memlen;
}                       

if                      /*  DN has to be written into message         */
  (*ignore == FALSE)
{                       /*  check buffer                              */
  if                    /*  error from check buffer                   */
    ((message = d26_h10_check_buffer(sizeof(D23_ds_name),remlen,input,
				     memlen,message)) == NULL)
  {                     /*  return error                              */
    return(NULL);
  }
  else
  {                     /*  write distinguished name header           */
    if                  /*  list has called                           */
      (deref_obj[0] != D2_EOS)
    {                   /*  concatenate names                         */
      sprintf((char *)cat_object,"%s%c%s",deref_obj,D2_RDN_SEP,
	     (D23_ds_name *)*smessage + 1);
      ((D23_ds_name *)message)->d23_dstag = D23_NAMTAG;
      ((D23_ds_name *)message)->d23_dslen = len =
	strlen((char *)deref_obj) + 1;
      ((D23_ds_name *)message)->d23_dsoff = offset = D2_ALIGN(len);
      object = cat_object;
    }
    else
    {                   /*  copy name structure                       */
      *((D23_ds_name *)message) = *((D23_ds_name *)(*smessage));
      object = (D2_name_string)((D23_ds_name *)(*smessage) + 1);
      len    = ((D23_ds_name *)(*smessage))->d23_dslen;
      offset = s_offset;
    }                   
    *remlen   -= sizeof(D23_ds_name);
    message   += sizeof(D23_ds_name);
    *smessage += sizeof(D23_ds_name);

    if                  /*  error from check buffer                   */
      ((message = d26_h10_check_buffer(offset,remlen,input,memlen,message))
       == NULL)
    {                   /*  return error                              */
      return(NULL);
    }
    else
    {                   /*  write distinguished name into message     */
      memcpy(message,object,len);
      *remlen   -= offset;
      message   += offset;
      *smessage += s_offset;
    }           
  }             
}
else
{               /*  skip DN from server message                       */
  s_offset  += sizeof(D23_ds_name);
  *smessage += s_offset;
  if            /*  undecoded part is following                       */
    (complete == FALSE)
  {             /*  skip attribute value from server message          */
    s_offset = sizeof(D23_av_value) + ((D23_av_value *)*smessage)->d23_avoff;
    *smessage -= s_offset;
  }             
}               

		/*  return no error                                   */
return(message);

}               /*  d26_h12_write_add_dn                              */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_h13_write_add_type                             */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 09.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function reads an attribute type from the       */
/*                server message and writes it for add object into    */
/*                message for cache.                                  */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      smessage    String *            server message to read        */
/*      input       D23_inrqpb *        parameters for invoke         */
/*      memlen      Ushort *            maximum length of this message*/
/*      ignore      Bool *              indicates, whether attribute  */
/*                                      must be ignored               */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      smessage    String *            server message to read        */
/*      input       D23_inrqpb *        parameters for invoke         */
/*      message     String *            message to write              */
/*      remlen      Ushort *            maximum length of this message*/
/*      ignore      Bool *              indicates, whether attribute  */
/*                                      must be ignored               */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      != D2_NULL  String              pointer to byte after message */
/*                                      written.                      */
/*         D2_NULL                      error occurred                */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d26_h13_write_add_type(byte **smessage, D23_inrqpb *input,
    signed16 memlen, byte *message, signed16 *remlen, boolean *ignore)

{                 /*  d26_h13_write_add_type                          */

  signed16 len = 0;             /* length of data to write            */
  signed16 offset;              /* offset to next data part           */
				/* attribute type to compare with     */
  D2_a_type comp_type;          /* pseudo-attribute                   */

comp_type.d2_typ_len = ((D23_at_type *)*smessage)->d23_atlen;
comp_type.d2_type    = (Ob_id_string)(*smessage + sizeof(D23_at_type));

if                      /*  pseudo attribute is coming in             */
  (!D2_ATTCMP(&comp_type,D2_CACHE_ATTR))
{                       /*  reset ignore flag                         */
  *ignore = TRUE;
}                       

len = sizeof(D23_at_type) + ((D23_at_type *)*smessage)->d23_atoff;
if                      /*  attribute type has to be written          */
  (*ignore == FALSE)
{                       /*  check buffer                              */
  if                    /*  error from check buffer                   */
    ((message = d26_h10_check_buffer(len,remlen,input,memlen,
				     message)) == NULL)
  {                     /*  return error                              */
    return(NULL);
  }
  else
  {                     /*  write attribute type and set pointer to   */
			/*  type in client message                    */
    d26_cl_type = message;
    *((D23_at_type *)message) = *((D23_at_type *)(*smessage));
    offset = ((D23_at_type *)(*smessage))->d23_atoff;
    *smessage += sizeof(D23_at_type);
    message   += sizeof(D23_at_type);
    *remlen   -= sizeof(D23_at_type);

    memcpy(message,*smessage,comp_type.d2_typ_len);
    *smessage += offset;
    message   += offset;
    *remlen   -= offset;
  }             
}
else
{               /*  skip server message                               */
  *smessage += len;
}               

		/*  return no error                                   */
return(message);

}               /*  d26_h13_write_add_type                           */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_h14_write_add_avinfo                              */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 09.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function reads an attribute value information   */
/*                from the smessage                                   */
/*                and writes it for add object into message for cache */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      smessage    String *            server message to read        */
/*      input       D23_inrqpb *        parameters for invoke         */
/*      memlen      Ushort *            maximum length of this message*/
/*      ignore      Bool                indicates, whether attribute  */
/*                                      must be ignored               */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      smessage    String *            server message to read        */
/*      input       D23_inrqpb *        parameters for invoke         */
/*      message     String *            message to write              */
/*      remlen      Ushort *            maximum length of this message*/
/*      ignore      Bool                indicates, whether attribute  */
/*                                      must be ignored               */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      != D2_NULL  String              pointer to byte after message */
/*                                      written.                      */
/*         D2_NULL                      error occurred                */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d26_h14_write_add_avinfo(byte **smessage, D23_inrqpb *input,
    signed16 memlen, byte *message, signed16 *remlen, boolean ignore)

{                /*  d26_h14_write_add_avinfo                         */

  signed16 len = 0;             /* length of data to write            */

len = sizeof(D23_av_info);
if                      /*  attribute information has to be written   */
  (ignore == FALSE)
{                       /*  check buffer                              */
  if                    /*  error from check buffer                   */
    ((message = d26_h10_check_buffer(len,remlen,input,memlen,message))
     == NULL)
  {                     /*  return error                              */
    return(NULL);
  }
  else
  {                     /*  write attribute information               */
    *(D23_av_info *)message = *(D23_av_info *)*smessage;
    *remlen -= len;
    message += len;
  }             
}               

*smessage += len;

		/*  return no error                                   */
return(message);

}               /*  d26_h14_write_add_avinfo                          */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_h15_write_add_avvalue                             */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 28.11.90                                            */
/*                                                                    */
/* DESCRIPTION  : The Function reads an attribute value               */
/*                from the smessage                                   */
/*                and writes it for add object into message for cache */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      smessage    String *        server message to read            */
/*      input       D23_inrqpb *    parameters for invoke             */
/*      memlen      Ushort *        maximum length of this message    */
/*      ignore      Bool            indicates, whether attribute must */
/*                                  be ignored                        */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      smessage    String *        server message to read            */
/*      input       D23_inrqpb *    parameters for invoke             */
/*      message     String *        message to write                  */
/*      remlen      Ushort *        maximum length of this message    */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      != D2_NULL  String          pointer to byte after message     */
/*                                  written.                          */
/*         D2_NULL                  error occurred                    */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d26_h15_write_add_avvalue(byte **smessage, D23_inrqpb *input,
    signed16 memlen, byte *message, signed16 *remlen, boolean ignore)

{                /*  d26_h15_write_add_avvalue                        */

  signed16 len = 0;             /* length of data to write            */
  signed16 offset;              /* offset to next data part           */

len    = sizeof(D23_av_value);
offset = ((D23_av_value *)*smessage)->d23_avoff;

if                      /*  attribute information has to be written   */
  (ignore == FALSE && offset <= D2_ATL_MAX)
{                       /*  check buffer                              */
  if                    /*  error from check buffer                   */
    ((message = d26_h10_check_buffer(len,remlen,input,memlen,message))
      == NULL)
  {                     /*  return error                              */
    return(NULL);
  }
  else
  {                     /*  write attribute value structure           */
    *(D23_av_value *)message = *(D23_av_value *)*smessage;
    *remlen   -= len;
    message   += len;
    *smessage += len;
    len        = offset;
    while               /*  still data to write                       */
	 (len > 0)
    {                   /*  check remaining write buffer              */
      if                /*  write memory insufficient                 */
	(*remlen <= len)
      {                 /*  write only part of accessible value       */
	memcpy(message,*smessage,*remlen);
	input->d23_6imdat = D23_MRDATA;
	input->d23_6lnidt = memlen;
	if              /*  error from ipc invoke                     */
	  (d23_invoke(input) == D23_ERROR)
	{               /*  write error log                           */
	  return(NULL);
	}
	else
	{               /*  reset message and length                  */
	  len       -= *remlen;
	  *smessage += *remlen;
	  *remlen  = memlen;
	  message  = input->d23_6rfidt;
	}               
      }
      else
      {                 /*  write whole accessible value              */
	memcpy(message,*smessage,len);
	*remlen   -= len;
	message   += len;
	*smessage += len;
	len = 0;
      }                 
    }                   
  }             
}
else
{               /*  skip server message                               */
  *smessage += len + offset;
}               

		/*  return no error                                   */
return(message);

}               /* d26_h15_write_add_avvalue                          */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_h21_send_skip                                   */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 09.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function sends an invoke message to cache and   */
/*                skips part of smessage.                             */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      smessage    String *            server message to read        */
/*      input       D23_inrqpb *        parameters for invoke         */
/*      memlen      Ushort *            maximum length of message to  */
/*                                      write                         */
/*      message     String *            message to write              */
/*      remlen      Ushort *            remaining length of message to*/
/*                                      write                         */
/*      tag         Ushort              indicates incoming tag        */
/*      dir_id      Ushort              directory ID                  */
/*      serv_ctr    D23_serv_ctrl *     service controls              */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      smessage    String *            server message to read        */
/*      input       D23_inrqpb *        parameters for invoke         */
/*      message     String *            message to write              */
/*      remlen      Ushort *            maximum length of this message*/
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR   Short               no error occurred             */
/*         -  1                         error occurred                */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_h21_send_skip(byte **smessage, D23_inrqpb *input,
    signed16 memlen, byte **message, signed16 *remlen, unsigned16 tag,
    signed16 dir_id, D23_serv_ctrl *serv_ctr)

{                /*  d26_h21_send_skip                                */

			/* parameters for wait for result             */
  static D23_wtrspb d23_wtrspb = {{D23_V02,0,0,0},D23_INFIN};

  signed16 len;         /* length of data to skip                     */

if              /*  message has already been written                  */
  (*message != input->d23_6rfidt)
{               /*  set length of data to write                       */
  len = sizeof(D23_eom);
  if            /*  error from check buffer                           */
    ((*message = d26_h10_check_buffer(len,remlen,input,memlen,
				      *message)) == NULL)
  {             /*  return error                                      */
    return(-1);
  }
  else
  {             /*  write end of message tag                          */
    ((D23_eom *)(*message))->d23_eomtag = D23_EOMTAG;
    *remlen -= len;
    *message += len;
    input->d23_6imdat  = D23_LASTFG;
    input->d23_6lnidt  = memlen - *remlen;
    if          /*  error from invoke                                 */
      (d23_invoke(input) == D23_ERROR)
    {           /*  trace error                                       */
      return(-1);
    }
    else
    {           /*  wait for result                                   */
      if        /*  error from wait for result                        */
	(d23_waitrs(&d23_wtrspb) == D23_ERROR)
      {         /*  trace error                                       */
	return(-1);
      }
      input->d23_6invinfo.d23_invid  = D23_UNUSED;
      *remlen = memlen;
      *message = input->d23_6rfidt;
    }           
  }             
}               

switch(tag)
{
  case  D23_ENTTAG  :
    len = sizeof(D23_en_info);
    if((*message = d26_h11_write_add_head(dir_id,serv_ctr,input,memlen,
       *message,remlen)) == NULL)
    {   
      return(-1);
    } 
    break;
  case  D23_SOITAG  :
    len = sizeof(D23_subord_info);
    if((*message = d26_h11_write_add_head(dir_id,serv_ctr,input,memlen,
       *message,remlen)) == NULL)
    {   
      return(-1);
    } 
    break;
  case  D23_OQTAG   :
  case  D23_CRSTAG  :
    len = 0;
    break;
}   

*smessage += len;

		/*  return no error                                   */
return(D2_NO_ERR);

}               /*  d26_h21_send_skip                                 */
