/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: retreqis.c,v $
 * Revision 1.1.737.2  1996/02/18  18:15:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:25  marty]
 *
 * Revision 1.1.737.1  1995/12/08  15:51:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:01  root]
 * 
 * Revision 1.1.735.3  1994/05/10  15:53:25  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:29:27  marrek]
 * 
 * Revision 1.1.735.2  1994/03/23  15:14:03  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:26:40  keutel]
 * 
 * Revision 1.1.735.1  1994/02/22  17:37:49  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:26:55  marrek]
 * 
 * Revision 1.1.733.2  1993/10/14  17:16:58  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:15:14  keutel]
 * 
 * Revision 1.1.733.1  1993/10/13  17:31:41  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:31:08  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  16:13:42  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:31:06  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  15:00:13  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.4  1993/02/02  11:35:51  marrek
 * 	Change type of ob_key to Long.
 * 	introduce D2_IA5
 * 	[1993/02/01  15:44:43  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:45:03  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:23:09  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:39:11  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:28:02  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  03:00:11  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:34:53  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: retreqis.c,v $ $Revision: 1.1.737.2 $ $Date: 1996/02/18 18:15:54 $";
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
/* NAME         : retreqis.c      [retreqis]                          */
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
/* DESCRIPTION  :   The module contains the ISAM utilities used by    */
/*                  the internal search function for reading requested*/
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

/******** LOCAL  ********/

static signed16 d26_i45_expand_obj_class(char *record, signed16 pos,
	signed16 count, signed32 *ocl_expand);
static byte *d26_i53_write_attr_val(signed16 ofilenr, char *record,
    D2_name_string object, signed16 *pos, byte *message, byte **output,
    D26_at *at, signed32 *remlen, signed32 *output_len);
static byte *d26_i54_spool_value(char *value, signed16 vlen, byte *message,
    byte **output, signed32 *remlen, signed32 *output_len);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i37_write_dummy_record                            */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        25.09.89                                              */
/*                                                                    */
/* DESCRIPTION: This function writes a DN-List index into a dummy     */
/*              record at a predefined position.                      */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d26_i37_write_dummy_record(
    char *record,       /* OUT   -  dummy record to write             */
    signed16 *pos,      /* INOUT -  position in record                */
    signed32 idx,       /* IN    -  DN-List index to write            */
    signed16 size)      /* IN    -  size of variable to store         */

{                               /*  d26_i37_write_dummy_record        */

  if                    /* short integer has to be stored             */
    (size == sizeof(signed16))
  {                     /* store short integer                        */
    stint((signed16)idx,record + *pos);
    *pos += INTSIZE;
  }
  else
  {                     /* store long integer                         */
    stlong(idx,record + *pos);
    *pos += LONGSIZE;
  }

}                       /*  d26_i37_write_dummy_record                */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i45_expand_obj_class                              */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        24.03.92                                              */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION: This function expands the list of object classes from */
/*              record by all the superclasses and puts them into     */
/*              ocl_expand.                                           */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      > 0         Short           count of object classes in list   */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_i45_expand_obj_class(
    char *record,       /* IN    -  record containing object classes  */
    signed16 pos,       /* IN    -  position of first object class    */
    signed16 count,     /* IN    -  count of object classes           */
			/* OUT   -  expanded object classes           */
    signed32 *ocl_expand)

{                               /*  d26_i45_expand_obj_class          */

  signed32 ob_cl;       /* object class value                         */
  signed32 *listp;      /* pointer to object class list               */
  void     *ts_root;    /* root for tsearch                           */

  register signed16 i;  /* loop variable                              */

ts_root = (void *)NULL;
listp   = ocl_expand;

for                     /*  all object classes in record              */
   (i = 0; i < count; i++)
{                       /*  read object class                         */
  ob_cl = ldlong(record + pos);
  pos += LONGSIZE;
  d26_u65_get_superclasses(ob_cl,&ts_root,&listp);
}

count = (signed16)(listp - ocl_expand);

for                     /* all allocated object classes               */
   (i = 0, listp = ocl_expand; i < count; i++, listp++)
{                       /*  delete node from tree                     */
  tdelete((void *)listp,&ts_root,d26_u66_longcmp);
}

return(count);

}                       /*  d26_i45_expand_obj_class                  */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i51_write_single_attribute                        */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        24.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function writes a single requested attribute into*/
/*              message in IPC format.                                */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      filenr      Short           file number containing the object */
/*      record      String          Record to read data from          */
/*      pos         Short           position of attribute in record   */
/*      object      D2_name_string  exact DN of read object           */
/*      message     String          actual message pointer            */
/*      output      String *        pointer to beginning of message   */
/*      r_type      D2_ret_val      request type                      */
/*      at          D26_at *        pointer to AT entry for attribute */
/*                                  to write                          */
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

byte *d26_i51_write_single_attribute(signed16 filenr, char *record,
    signed16 pos, D2_name_string object, byte *message, byte **output,
    D2_ret_val r_type, D26_at *at, signed32 *remlen, signed32 *output_len)

{                               /*  d26_i51_write_single_attribute    */

  signed16 count;           /* count of attribute values              */
			    /* record for object class values         */
  signed32 *ocl_expand = NULL;

  register signed16 idx;    /* loop variable                          */

count = ldint(record + pos);
pos  += INTSIZE;

if                      /*  attribute values exist                    */
  (count > 0)
{                       /*  reset counter of values                   */
  if                    /*  attribute is object class                 */
    (!D2_ATTCMP(&at->d26a_type,D2_OBJ_CLASS))
  {                     /*  allocate memory for object class record   */
    if                  /*  no more memory available                  */
      ((ocl_expand = (signed32 *)calloc(d26_nooct,sizeof(signed32)))
       == NULL)
    {                   /*  set error apdu                            */
      d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,D2_REM_ERR,
			     D2_UNWILLING,output_len);
      return(NULL);
    } else {            /*  expand object classes into that array     */
      count = d26_i45_expand_obj_class(record,pos,count,ocl_expand);
      record = (char *)ocl_expand;
      pos = 0;
    }                   /*                                            */
  }                     /*                                            */
  count = r_type == D2_R_TYP ? 0 : count;
  if                    /*  attribute type could be written           */
    ((message = d26_u56_write_attribute_type(message,output,&at->d26a_type,
     count,at->d26a_rep,remlen,output_len)) != NULL)
  {                     /*  write attribute values                    */
    for                 /*  all values found                          */
       (idx = 0; idx < count; idx++)
    {                   /*  write attribute value                     */
      if                /*  error from writing value                  */
	((message = d26_i53_write_attr_val(filenr,record,object,&pos,message,
	 output,at,remlen,output_len)) == NULL)
      {                 /*  return error                              */
	break;
      }                 /*                                            */
    }                   /*                                            */
  }                     /*                                            */
  if                    /*  memory for expanded object class allocated*/
    (ocl_expand != (signed32 *)NULL)
  {                     /*  free memory                               */
    free(ocl_expand);
  }                     /*                                            */
}                       /*                                            */

			/*  return message pointer                    */
return(message);

}                       /*  d26_i51_write_single_attribute            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i52_write_recur_attribute                         */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        24.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function writes a recurring requested attribute  */
/*              into message in IPC format.                           */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      record      String          Record to read data from          */
/*      filenr      Short           number of object file             */
/*      pos         Short           position of attribute in record   */
/*      object      D2_name_string  exact DN of read object           */
/*      message     String          actual message pointer            */
/*      output      String *        pointer to beginning of message   */
/*      r_type      D2_ret_val      request type                      */
/*      at          D26_at *        pointer to AT entry for attribute */
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
/*exoff ***************************************************************/

byte *d26_i52_write_recur_attribute(char *record, signed16 filenr,
    signed16 pos, D2_name_string object, byte *message, byte **output,
    D2_ret_val r_type, D26_at *at, signed32 *remlen, signed32 *output_len)

{                               /*  d26_i52_write_recur_attribute     */

			    /* function identifier for traces         */
  static String function = "i52";

  signed16 count;           /* count of values                        */
  signed32 ob_key;          /* isam key of record                     */
  signed16 at_filenr;       /* number of recurring attribute file     */
  int      isfd;            /* Isam file descriptor                   */
  int      mode;            /* ISAM read mode                         */
  D26_keydesc key;          /* ISAM key structure                     */

  signed32 is_count = 0;    /* counts calls of isstart                */
  signed32 ir_count = 0;    /* counts calls of isread                 */

  register signed16 i;      /* loop variable                          */

count = ldint(record + pos);

if                      /*  attribute values exist                    */
  (count > 0)
{                       /*  reset counter of values                   */
  count = r_type == D2_R_TYP ? 0 : count;
  if                    /*  error from writing attribute type         */
    ((message = d26_u56_write_attribute_type(message,output,&at->d26a_type,
     count,at->d26a_rep,remlen,output_len)) == NULL)
  {                     /*  return error                              */
    return(NULL);
  }
  else if               /*  values must be written                    */
	 (count > 0)
  {                     /*  open c-isam file                          */
    at_filenr = at->d26a_filenr;
    if                              /*  error from open file          */
      ((isfd = d26_i23_open_file(at_filenr,D26_ATT)) < 0)
    {                               /*  return message                */
      return(message);
    }
    else
    {                   /*  select key                                */
      d26_i55_sel_rec_key(&key,at,D2_EQUAL,D23_READ);
			/*  fill key in record                        */
      ob_key = ldlong(record);
      stint(filenr,d26_a_record);
      stlong(ob_key,d26_a_record + INTSIZE);
      if                /*  attribute file is in use already          */
	(d26_afildes[at_filenr].d26_in_use > 0)
      {                 /*  set the reset flag                        */
	d26_reset = TRUE;
      }                 /*                                            */
      is_count++;
      d26_dbacc_count++;
      if                /*  error from start query                    */
	(D26_ISSTART(svc_c_route_nolog,isfd,&key,INTSIZE + LONGSIZE,
	 d26_a_record,ISEQUAL) < 0)
      {                 /*  check error number                        */
	D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,is_count,
	  ir_count);
	if              /*  just no match to key                      */
	  (iserrno == ENOREC)
	{               /*  return without error                      */
	  return(message);
	}
	else
	{        /*  write error message                       */
	  dce_svc_printf(GDS_S_ISSTART_ERR_MSG,iserrno);
	  return(message);
	}
      }
      else
      {                 /*  set query mode                            */
	for             /*  all attribute values                      */
	   (i = 0, mode = ISCURR + ISWAIT; i < count; i++)
	{               /*  write attribute value                     */
	  pos = INTSIZE + LONGSIZE;
	  ir_count++;
	  d26_dbacc_count++;
	  if            /*  error from read isam                      */
	    (D26_ISREAD(svc_c_route_nolog,isfd,d26_a_record,mode) < 0)
	  {             /*  check error                               */
	    D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,
	      is_count,ir_count);
	    switch(iserrno)
	    {
	      case  EENDFILE:
		return(message);
	      default:
		dce_svc_printf(GDS_S_ISREAD_ERR_MSG,mode,iserrno);
		return(message);
	    }   
	  }
	  else if       /*  other object number and key found         */
		 (filenr != ldint(d26_a_record)
		  || ob_key != ldlong(d26_a_record + INTSIZE))
	  {             /*  return message                            */
	    D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,
	      is_count,ir_count);
	    return(message);
	  }
	  else if       /*  error from writing attribute value        */
		 ((message = d26_i53_write_attr_val(D26_IDX_NOT_DEF,
		  d26_a_record,object,&pos,message,output,at,remlen,
		  output_len)) == NULL)
	  {             /*  return error                              */
	    D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,
	      is_count,ir_count);
	    return(NULL);
	  }
	  else
	  {             /*  reset mode                                */
	    mode = ISNEXT + ISWAIT;
	  }
	}
	D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,is_count,
	  ir_count);
      }
    }
  }
}

			/*  return message pointer                    */
return(message);

}                       /*  d26_i52_write_recur_attribute             */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i53_write_attr_val                                */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        24.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function writes a requested attribute value into */
/*              message in IPC format.                                */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      ofilenr     Short           object file number                */
/*      record      String          Record to read data from          */
/*      object      D2_name_string  exact DN of read object           */
/*      pos         Short *         Position of value in record       */
/*      message     String          actual message pointer            */
/*      output      String *        pointer to beginning of message   */
/*      at          D26_at *        pointer to AT entry for attribute */
/*                                  to write                          */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of result message  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      pos         Short *         Position of next value in record  */
/*      output      String *        pointer to beginning of message   */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           length of result message          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              String                      pointer to byte after     */
/*                                          written message.          */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d26_i53_write_attr_val(signed16 ofilenr, char *record,
    D2_name_string object, signed16 *pos, byte *message, byte **output,
    D26_at *at, signed32 *remlen, signed32 *output_len)

{                               /*  d26_i53_write_attr_val            */

  signed16 len;             /* length of message to write             */
  signed16 atlen;           /* length of attribute value              */
			    /* memory to write ...                    */
  signed16 add_len;         /* length of internal added characters    */
  signed16 phon_len;        /* length of phonetical value             */
  char     *value;          /* attribute value to write               */
  char     *val1,*val2;     /* help pointers to attribute values      */
  signed16 rep;             /* representation of attribute            */
  D26_oct  *oct;            /* pointer to OCT-entry                   */

			    /* memory for converted attribute value   */
  signed32 convert_val[D26_MAXVALLEN / sizeof(signed32)];

  register signed16 p_len;  /* length of part of attribute value      */

phon_len = at->d26a_phon == TRUE ? D26_LPHON : 0;
rep      = at->d26a_rep;

switch(at->d26a_syntax)
{
  case D2_CIGNORE:
  case D2_IA5:
  case D2_CIGN_LIST:
  case D2_NUMERIC:
  case D2_TELNR_STX:
  case D2_POST_AD_STX:
  case D2_COUNTRY_STX:
    if                  /* attribute is a sequence of choice T61-prtbl*/
      (at->d26a_rep == D2_T61_PR_LIST)
    {                   /* set additional length                      */
      add_len = 2 * D26_MAXVALPARTS;
    }
    else if             /* attribute is common name of schema object  */
	   (ofilenr == 0)
    {                   /* set additional length                      */
      add_len = D26_CNL - at->d26a_ubound + 1;
      phon_len = D26_LPHON;
    }
    else
    {                   /* set additional length to 1                 */
      add_len = 1;
    }
    *pos += phon_len;
    if                  /* representation is a choice                 */
      (rep == D2_T61_PR)
    {                   /* read it from record                        */
      rep = ldint(record + *pos);
      *pos += INTSIZE;
    }                   /*                                            */
    *pos += at->d26a_ubound + add_len;
    value = record + *pos;
    for(atlen = 0; atlen < at->d26a_ubound + add_len;
	atlen += p_len + 1, value += p_len + 1)
    { 
      if((p_len = strlen(value)) == 0)
      {   
	break;
      } 
    } 
    if                  /* attribute length is positive               */
      (atlen > 0)
    {                   /* reduce it                                  */
      atlen--;
    }
    value = record + *pos;
    *pos += at->d26a_ubound + add_len;
    break;
  case  D2_CEXACT :
  case  D2_PRINTABLE:
  case  D2_TIME     :
  case  D2_TLXNR_STX:
    add_len  = (at->d26a_syntax == D2_TLXNR_STX) ? D26_MAXVALPARTS : 1;
    *pos += phon_len;
    if                  /* representation is a choice                 */
      (rep == D2_T61_PR)
    {                   /* read it from record                        */
      rep = ldint(record + *pos);
      *pos += INTSIZE;
    }
    value = record + *pos;
    for(atlen = 0; atlen < at->d26a_ubound + add_len;
	atlen += p_len + 1, value += p_len + 1)
    { 
      if((p_len = strlen(value)) == 0)
      {   
	break;
      } 
    } 
    if                  /* attribute length is positive               */
      (atlen > 0)
    {                   /* reduce it                                  */
      atlen--;
    }
    value = record + *pos;
    *pos += at->d26a_ubound + add_len;
    break;
  case  D2_MHS_DLSP_STX:
    *pos += at->d26a_ubound + INTSIZE;
    value = (char *)convert_val;
    atlen = d26_im13_ld_dl_sub_perm(value,record + *pos,
				    ldint(record + *pos + at->d26a_ubound));
    *pos += at->d26a_ubound + INTSIZE;
    break;
  case  D2_MHS_ORADR_STX:
    *pos += at->d26a_ubound + INTSIZE;
    value = (char *)convert_val;
    atlen = d26_im11_ld_or_address(value,record + *pos,
				   ldint(record + *pos + at->d26a_ubound));
    *pos += at->d26a_ubound + INTSIZE;
    break;
  case  D2_MHS_ORNAME_STX:
    *pos += at->d26a_ubound + INTSIZE;
    value = (char *)convert_val;
    atlen = d26_im12_ld_or_name(value,record + *pos,
				ldint(record + *pos + at->d26a_ubound));
    *pos += at->d26a_ubound + INTSIZE;
    break;
  case  D2_OBJ_IDENT:
    if(!D2_ATTCMP(&at->d26a_type,D2_OBJ_CLASS))
    {   
      oct = d26_oct + ((signed32 *)record)[*pos];
      (*pos)++;
      if(oct->d26c_ob_id.d2_typ_len == 0)
      {   
	return(message);
      }
      else
      {
	atlen = (signed16)oct->d26c_ob_id.d2_typ_len;
	value = (char *)oct->d26c_ob_id.d2_type;
      } 
      break;
    } 
		    /* handle as octet string otherwise     */
  case  D2_ANY_STX:
  case  D2_OCTET:
  case  D2_PASSWD_STX:
  case  D2_ASN1:
    value = record + *pos;
    *pos += at->d26a_ubound;
    atlen = ldint(record + *pos);
    *pos += INTSIZE;
    if                      /* attribute has password syntax          */
      (at->d26a_syntax == D2_PASSWD_STX)
    {                       /* descramble it                          */
      d26_u18_scramble_string((byte *)value,(byte *)value,atlen);
    }                       /*                                        */
    break;
  case  D2_GUIDE  :
    value = (char *)convert_val;
    atlen = d26_is11_ld_search_guide(value,record + *pos);
    *pos += at->d26a_ubound + INTSIZE;
    break;
  case  D2_TTXID_STX:
    value = (char *)convert_val;
    atlen = d26_is12_ld_ttx_term_id(value,record + *pos,
				    ldint(record + *pos + at->d26a_ubound));
    *pos += at->d26a_ubound + INTSIZE;
    break;
  case  D2_FAXNR_STX:
    value = (char *)convert_val;
    atlen = d26_is13_ld_fax_number(value,record + *pos,
				   ldint(record + *pos + at->d26a_ubound));
    *pos += at->d26a_ubound + INTSIZE;
    break;
  case  D2_PSAP_STX:
    value = (char *)convert_val;
    atlen = d26_is14_ld_psap_addr(value,record + *pos,
				  ldint(record + *pos + at->d26a_ubound));
    *pos += at->d26a_ubound + INTSIZE;
    break;
  case  D2_BOOLEAN:
    value = (char *)convert_val;
    *(Bool *)value = (Bool)ldlong(record + *pos);
    atlen = sizeof(Bool);
    *pos += LONGSIZE;
    break;
  case  D2_INTEGER:
    value = (char *)convert_val;
    *(signed32 *)value = ldlong(record + *pos);
    atlen = sizeof(signed32);
    *pos += LONGSIZE;
    break;
  case  D2_PR_DM_STX:
  case  D2_MHS_PR_DM_STX:
    for(value = (char *)convert_val, atlen = 0;
	atlen < at->d26a_ubound * LONGSIZE;
	value += LONGSIZE, atlen += LONGSIZE, *pos += LONGSIZE)
    { 
      if((*(signed32 *)value = ldlong(record + *pos)) == -1L)
      {   
	break;
      } 
    } 
    value = (char *)convert_val;
    *pos += at->d26a_ubound * LONGSIZE;
    break;
  case  D2_DISTNAME:
    atlen = d26_i56_atdnlen(record,pos,at,(D2_name_string *)&value,TRUE);
    break;
  case  D2_ACCL   :
    value = (char *)convert_val;
    atlen = d26_i57_atacllen(object,record,pos,(D2_name_string)value,TRUE);
    break;
  case  D2_CERTIFICATE:
    value = val1 = (char *)convert_val;
    val2 = record + *pos;
    d26_is31_ld_certificate(&val1,&val2);
    *pos += at->d26a_ubound;
    atlen = ldint(record + *pos);
    *pos += INTSIZE;
    break;
  case  D2_CERT_PAIR:
    value = val1 = (char *)convert_val;
    val2 = record + *pos;
    d26_is32_ld_cert_pair(&val1,&val2);
    *pos += at->d26a_ubound;
    atlen = ldint(record + *pos);
    *pos += INTSIZE;
    break;
  case  D2_CERT_LIST:
    value = val1 = (char *)convert_val;
    val2 = record + *pos;
    d26_is33_ld_cert_list(&val1,&val2);
    *pos += at->d26a_ubound;
    atlen = ldint(record + *pos);
    *pos += INTSIZE;
    break;
}   

len = sizeof(D23_av_value);
if                      /*  buffer is sufficient                      */
  ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
   != NULL)
{                       /*  write ATR tag                             */
  ((D23_av_value *)message)->d23_avlen = atlen;
  ((D23_av_value *)message)->d23_avoff = D2_ALIGN(atlen);
  ((D23_av_value *)message)->d23_avrep = (unsigned16)rep;
  *remlen -= len;
  message += len;
  message = d26_i54_spool_value(value,atlen,message,output,remlen,
				output_len);
}

			/*  return message pointer                    */
return(message);

}                       /*  d26_i53_write_attr_val                    */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i54_spool_value                                   */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        24.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function writes a requested attribute value into */
/*              message in IPC format.                                */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      value       String          value to write                    */
/*      vlen        Short           length of value                   */
/*      message     String          actual message pointer            */
/*      output      String *        pointer to beginning of message   */
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

static byte *d26_i54_spool_value(char *value, signed16 vlen, byte *message,
    byte **output, signed32 *remlen, signed32 *output_len)

{                               /*  d26_i54_spool_value               */

  signed32 len;             /* length of message to write             */
  signed32 poq_offset;      /* offset of partial outcome qualifier    */
			    /* from the beginning of the message      */
  signed32 block_count;     /* count of blocks to allocate            */
  byte     *new_output;     /* holds address of reallocated output    */

len = D2_ALIGN(vlen);
block_count = (len - *remlen + D26_BLOCK_LEN - 1) / D26_BLOCK_LEN;

if                      /*  attribute value does not fit into actual  */
			/*  block                                     */
  (block_count > 0)
{                       /*  check calling function                    */
  poq_offset = d26_start_p_o_q - *output;
  if                    /*  error from reallocating                   */
    ((new_output = (byte *)D27_REALLOC(svc_c_sev_warning,*output,
     *output_len + block_count * D26_BLOCK_LEN)) == NULL)
  {                     /*  set error apdu                            */
    d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,
			   D2_REM_ERR,D2_UNWILLING,output_len);
    return(NULL);
  }
  else
  {                     /*  reset message and length                  */
    *output         = new_output;
    *remlen        += block_count * D26_BLOCK_LEN;
    *output_len    += block_count * D26_BLOCK_LEN;
    message         = new_output + *output_len - *remlen;
    d26_start_p_o_q = d26_start_p_o_q == NULL ? NULL :
		      *output + poq_offset;
  }
}

memcpy(message,value,vlen);
message += len;
*remlen -= len;

			/*  return message pointer                    */
return(message);

}                       /*  d26_i54_spool_value                       */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i56_atdnlen                                       */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        04.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function sets dname to point to the DN described */
/*              by record, position and attribute type.               */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      record      String *        Record to read data from          */
/*      pos         Short *         Position of value in record       */
/*      at          D26_at *        pointer to AT-entry               */
/*      exact       Bool            Indicates whether exact value is  */
/*                                  required                          */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      dname       D2_name_string *pointer to DN                     */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              Short                       length of DN-attribute    */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i56_atdnlen(char *record, signed16 *pos, D26_at *at,
    D2_name_string *dname, boolean exact)

{                               /*  d26_i56_atdnlen                   */

  signed32 dnidx;           /* index of DN in table                   */
  D26_dndesc *dndesc;       /* pointer to DN description              */

dnidx = ldlong(record + *pos);
*pos += LONGSIZE;

if                          /*  Master-knowledge attribute is required*/
			    /*  and index denotes own DSA-name        */
  (!D2_ATTCMP(&at->d26a_type,D2_MASTER_KNOWLEDGE) && dnidx == D26_OWNNAME)
{                           /*  set name pointer to own DSA-name      */
  *dname = (D2_name_string)(exact == TRUE ? d26_myexact_name :
					       d26_myname);
}
else
{                           /*  get DN-List-entry                     */
  d26_u11_check_dn_index(dnidx,&dndesc);
  if                        /*  DN-index is invalid                   */
    (dndesc == NULL)
  {                         /*  set name pointer to NULL              */
    *dname = NULL;
  }
  else
  {                         /*  check occurrrency                     */
    if                      /*  no references to that entry           */
      (dndesc->d26_occurrency == 0)
    {                       /*  set name pointer to NULL              */
      *dname = NULL;
    }
    else
    {                       /*  set name pointer to DN-list-entry     */
      *dname = exact == TRUE ? dndesc->d26_ename : dndesc->d26_dnname;
    }
  }
}

				/*  return attribute length           */
return(*dname == NULL ? 0 : strlen((char *)*dname) + 1);

}                               /*  d26_i56_atdnlen                   */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i57_atacllen                                       */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        08.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the acl message part to send on  */
/*              ipc. It returns the length of this value.             */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      object      D2_name_string  exact DN of object read already   */
/*      record      String *        Record to read data from.         */
/*      pos         Short *         Position of value in record       */
/*      exact       Bool            Indicates whether exact value is  */
/*                                  required                          */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      acl         String          where the acl value has to be     */
/*                                  written to                        */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >0          Short           length of ACL-attribute           */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i57_atacllen(D2_name_string object, char *record, signed16 *pos,
    byte *acl, boolean exact)

{                               /*  d26_i57_atacllen                  */

  D26_dndesc *dndesc;       /* pointer to DN description              */
  signed32   idx;           /* DN-list index read from record         */
  signed16   len;           /* length of attribute value              */
  byte       *begin;        /* begin of acl value                     */

  register signed16 i;      /* loop variable                          */

begin = acl;

for                             /*  all ACL Items                     */
   (i = 0; i < D2_ACLIDX_MAX; i++, *pos += LONGSIZE)
{                               /*  read DNL index from record        */
  if                            /*  DNL-Index not negative            */
    ((idx = ldlong(record + *pos)) >= 0)
  {                             /*  take DN from DN list              */
    d26_u11_check_dn_index(idx,&dndesc);
    if                          /*  DNL Index ok                      */
      (dndesc != NULL)
    {                           /*  write DN into acl                 */
      *acl++ = dndesc->d26_dnint;
      if                        /* exact name is required             */
	(exact == TRUE)
      {                         /* copy exact name                    */
	strcpy((char *)acl,(char *)dndesc->d26_ename);
      }
      else
      {                         /* copy normed name                   */
	strcpy((char *)acl,(char *)dndesc->d26_dnname);
      }
      acl += strlen((char *)acl) + 1;
    }
    else
    {                           /*  write DN invalid into acl         */
      *acl++ = D2_IGNORE;
      *acl++ = D2_EOS;
    }
  }
  else
  {                             /*  take DN from object               */
    if                          /*  object itself has to be taken     */
      (idx == D26_OWNNAME)
    {                           /*  write "single object"             */
      *acl++ = D2_SOBJ;
      strcpy((char *)acl,(char *)object);
      acl += strlen((char *)acl) + 1;
    }
    else if                     /*  no DN is given                    */
	   (idx == D26_EOL)
    {                           /*  write DN invalid into acl         */
      *acl++ = D2_IGNORE;
      *acl++ = D2_EOS;
    }
    else
    {                           /*  calculate length of ancestor      */
      len = d26_u08_calc_len(object,(signed16)idx);
      *acl++ = D2_SUBTREE;
      sprintf((char *)acl,"%*.*s",len,len,object);
      acl += len + 1;
    }
  }
}

return(acl - begin);

}                               /*  d26_i57_atacllen                  */
