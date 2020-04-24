/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26filis.c,v $
 * Revision 1.1.10.2  1996/02/18  19:45:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:25  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:44:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:51  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:52:10  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:21:18  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:05:41  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:12:00  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  14:01:22  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:08:01  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  14:46:20  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.4  1993/02/02  11:19:00  marrek
 * 	Introduce D2_IA5
 * 	[1993/02/01  14:49:06  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:40:05  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:19:24  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:42:35  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:09:47  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:48:28  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:54:50  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26filis.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:45:13 $";
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
/* NAME         : d26filis.c     [filis]                              */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 09.04.89                                            */
/*                                                                    */
/* COMPONENT    : DSA, retrieval functions                            */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   The module contains functions to compare filter   */
/*                  items in Syntax of Version 2.1A.                  */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 09.04.89| birth                          | ek |         */
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

/*****  internal Includes    *****/

#include <d26schema.h>
#include <d26isam.h>

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

/******** IMPORT ********/

/******** LOCAL  ********/

static signed16 d26_i40_cmp_ordering(signed16 len, byte *value,
    signed16 rule, char *record, signed16 *pos, D26_at *at);
static signed16 d26_i82_cmp_substr(D23_fs1_info *f_substr, signed16 ofilenr,
    char *record, D2_name_string dname, signed16 *pos, D26_at *at);

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_i40_cmp_ordering                                */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 04.01.91                                            */
/*                                                                    */
/* DESCRIPTION  : This function will compare the filter and attribute */
/*                values from message and record using                */
/*                ordering rule.                                      */
/*                                                                    */
/* RETURN VALUE :   D2_NO_ERR:              object matches filter     */
/*                  D2_NFOUND               object doesn't match      */
/*                                          filter                    */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_i40_cmp_ordering(
    signed16 len,       /* IN    -  length of filter value            */
    byte     *value,    /* IN    -  filter value                      */
    signed16 rule,      /* IN    -  either D2_GTEQ or D2_LTEQ         */
    char     *record,   /* IN    -  record containing object          */
    signed16 *pos,      /* INOUT -  position of value in record       */
    D26_at   *at)       /* IN    -  AT entry of filter attribute      */

{                                   /*  d26_i40_cmp_ordering          */

  signed32 comp_val;            /* value in record                    */
  signed16 comp_len;            /* length of value in record          */
  int      comp;                /* indicates wheter value in record   */
				/* is smaller, equal or greater than  */
				/* filter value                       */
  char     normed_time[D26_TLEN];

  switch(at->d26a_syntax)
  {
    case  D2_INTEGER  :
      comp_val = ldlong(record + *pos);
      if                /* values do not match for matching rule      */
	(comp_val < *(signed32 *)value && rule == D2_LTEQ ||
	 comp_val > *(signed32 *)value && rule == D2_GTEQ)
      {                 /* return no match                            */
	*pos += LONGSIZE;
	return(D2_NFOUND);
      }
      break;
    case  D2_OCTET    :
      comp_len = ldint(record + *pos + at->d26a_ubound);
      comp_len = comp_len < len ? comp_len : len;
      if                /* values do not match for 'less'             */
	((comp = memcmp(record + *pos,value,comp_len)) < 0 &&
	 rule == D2_LTEQ)
      {                 /* return no match                            */
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      else if           /* values do not match for 'greater'          */
	     (comp > 0 && rule == D2_GTEQ)
      {                 /* return no match                            */
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      else if           /* values do not match for matching rule      */
	     (comp_len < len && rule == D2_LTEQ ||
	     comp_len > len && rule == D2_GTEQ)
      {                 /* return no match                            */
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      break;
    case  D2_TIME     :
      d26_us9_norm_utc_time((char *)value,normed_time);
      if                /* values do not match for 'less'             */
	((comp = strcmp(record + *pos,normed_time)) < 0 && rule == D2_LTEQ)
      {                 /* return no match                            */
	 *pos += at->d26a_ubound + 1;
	 return(D2_NFOUND);
      }
      else if           /* values do not match for 'greater'          */
	     (comp > 0 && rule == D2_GTEQ)
      {                 /* return no match                            */
	 *pos += at->d26a_ubound + 1;
	 return(D2_NFOUND);
      }
      break;
    default           :
      return(D2_NFOUND);
  }

			/*  return no error                           */
  return(D2_NO_ERR);

}                                   /*  d26_i40_cmp_ordering          */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_i79_cmp_sg_item()                               */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 09.04.89                                            */
/*                                                                    */
/* DESCRIPTION  : This function will look whether the filter item     */
/*                matches a value in record for attributes with       */
/*                restricted count of values.                         */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*      f_item      D23_fi1_info *  information about filter attribute*/
/*      filenr      Short           number of object file             */
/*      record      String          record containing object          */
/*      dname       D2_name_string  distinguished name of object found*/
/*      at          D26_at *        description of filter attribute   */
/*      pos         Short           position of attribute in record   */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*      None                                                          */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR               Short   object matches filter item    */
/*      D26_NOFILTERMATCH               filter doesn't match attribute*/
/*                                      on object                     */
/*      D2_ATTRIBUTE_MISSING            filter attribute not present  */
/*                                      on object                     */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i79_cmp_sg_item(D23_fi1_info *f_item, signed16 filenr,
    char *record, D2_name_string dname, D26_at *at, signed16 pos)

{                                   /*  d26_i79_cmp_sg_item           */

  signed16     ret_value;       /* return value                       */
  signed16     count;           /* count of attribute values in record*/
  D23_at_type  *f_type;         /* type of filter attribute           */
  D23_av_value *f_value;        /* filter value                       */
  D23_fs1_info *f_substr;       /* filter substrings                  */

  register signed16 i;          /* loop variable                      */

ret_value = D2_ATTRIBUTE_MISSING;
count     = ldint(record + pos);
pos      += INTSIZE;

f_type = (D23_at_type *)(f_item + 1);
for                                 /*  all values in record          */
   (i = 0; i < count; i++)
{                                   /*  compare single value          */
  ret_value = D26_NOFILTERMATCH;
  switch(f_item->d23_fi1match)
  {
    case  D2_APPROX_MATCH :
      if                            /*  attribute admits phonetical   */
				    /*  matching                      */
	(at->d26a_phon == TRUE)
      {                             /*  set filter value              */
	f_value = (D23_av_value *)((byte *)(f_type + 1) + f_type->d23_atoff);
	if(d26_i42_cmp_phon(f_value->d23_avlen,(byte *)(f_value + 1),record,
	   &pos,at) == D2_NO_ERR)
	{   
	  return(D2_NO_ERR);
	} 
	break;
      }                             /*                                */
	    /* compare for equality otherwise                         */
    case  D2_EQUAL        :
      f_value = (D23_av_value *)((byte *)(f_type + 1) + f_type->d23_atoff);
      if(d26_i38_cmp_equal(f_value->d23_avlen,(char *)(f_value + 1),filenr,
	 record,dname,&pos,at) == D2_NO_ERR)
      {   
	return(D2_NO_ERR);
      } 
      break;
    case  D2_SUBSTRINGS   :
      f_substr = (D23_fs1_info *)((byte *)(f_type + 1) + f_type->d23_atoff);
      if(d26_i82_cmp_substr(f_substr,filenr,record,dname,&pos,at)
	 == D2_NO_ERR)
      {   
	return(D2_NO_ERR);
      } 
      break;
    case  D2_GTEQ         :
    case  D2_LTEQ         :
      f_value = (D23_av_value *)((byte *)(f_type + 1) + f_type->d23_atoff);
      if(d26_i40_cmp_ordering(f_value->d23_avlen,(byte *)(f_value + 1),
	 f_item->d23_fi1match,record,&pos,at) == D2_NO_ERR)
      {   
	return(D2_NO_ERR);
      } 
      break;
    case  D2_PRESENT      :
      return(D2_NO_ERR);
    default               :
      break;
  }
}

				    /*  return value                  */
return(ret_value);

}                                   /*  d26_i79_cmp_sg_item           */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_i81_cmp_rec_item()                              */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 09.04.89                                            */
/*                                                                    */
/* DESCRIPTION  : This function will look whether the recurring filter*/
/*                item in V21A-syntax matches the attribute from      */
/*                message matches the one in record.                  */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*      f_item      D23_fi1_info *  information about filter attribute*/
/*      file_nr     Short           number of object file             */
/*      record      String          ISAM-record containing object     */
/*      dname       D2_name_string  distinguished name of object found*/
/*      at          D26_at *        description of filter attribute   */
/*      pos         Short           position of attribute in record   */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*      None                                                          */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR               Short   object matches filter item    */
/*      D26_NOFILTERMATCH               filter doesn't match attribute*/
/*                                      on object                     */
/*      D2_ATTRIBUTE_MISSING            attribute not present on      */
/*                                      object                        */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i81_cmp_rec_item(D23_fi1_info *f_item, signed16 file_nr,
    char *record, D2_name_string dname, D26_at *at, signed16 pos)

{                                   /*  d26_i81_cmp_rec_item          */

  signed32     key;             /* serialkey of read object           */
  signed16     count;           /* count of attr. values in record    */
  int          isfd;            /* ISAM File descriptor               */
  int          mode = ISEQUAL;  /* ISAM read mode                     */

  D23_at_type  *f_type;         /* type of filter attribute           */
  D23_av_value *f_value;        /* filter value                       */
  D23_fs1_info *f_substr;       /* filter substrings                  */
  signed16     len;             /* length of initial substring        */
  char         *value;          /* initial substring value            */

  register signed16 i;          /* loop variable                      */

key       = ldlong(record);
count     = ldint(record + pos);

if                              /*  no attribute value is given       */
  (count <= 0)
{                               /*  return no match                   */
  return(D2_ATTRIBUTE_MISSING);
}
else if                         /*  attribute must only exist         */
       (f_item->d23_fi1match == D2_PRESENT)
{                               /*  return match                      */
  return(D2_NO_ERR);
}
else if                         /*  error from open file              */
       ((isfd = d26_i23_open_file(at->d26a_filenr,D26_ATT)) < 0)
{                               /*  return not found                  */
  return(D26_NOFILTERMATCH);
}
else
{                               /*  check matching rule               */
  f_type = (D23_at_type *)(f_item + 1);
  switch(f_item->d23_fi1match)
  {
    case    D2_APPROX_MATCH:
      if                        /*  attribute admits phonetical match */
	(at->d26a_phon == TRUE)
      {                         /*  compare phonetical strings        */
	f_value = (D23_av_value *)((byte *)(f_type + 1) + f_type->d23_atoff);
	if(d26_i18_read_rec_val(f_value->d23_avlen,(char *)(f_value + 1),key,
	   file_nr,at,FALSE,FALSE,ISEQUAL,isfd,f_item->d23_fi1match,
	   d26_a_record) == D2_NO_ERR)
	{   
	  return(D2_NO_ERR);
	} 
	break;
      }                         /*                                    */
	    /* compare for equality otherwise                         */
    case    D2_EQUAL:
      f_value = (D23_av_value *)((byte *)(f_type + 1) + f_type->d23_atoff);
      if(d26_i18_read_rec_val(f_value->d23_avlen,(char *)(f_value + 1), key,
	 file_nr,at,FALSE,FALSE,ISEQUAL,isfd,f_item->d23_fi1match,
	 d26_a_record) == D2_NO_ERR)
      {   
	return(D2_NO_ERR);
      } 
      break;
    case    D2_SUBSTRINGS:
      f_substr = (D23_fs1_info *)((byte *)(f_type + 1) + f_type->d23_atoff);
      if(f_substr->d23_fs1position == D2_INITIAL)
      {   
	len   = f_substr->d23_fs1len;
	value = (char *)(f_substr + 1);
      }
      else
      {
	len   = 0;
	value = (char *)NULL;
      } 
      for(i = 0; i < count; i++, mode = ISNEXT)
      { 
	if(d26_i18_read_rec_val(len,value,key,file_nr,at,TRUE,FALSE,
	   mode,isfd,f_item->d23_fi1match,d26_a_record) == D2_NO_ERR)
	{   
	  pos = INTSIZE + LONGSIZE;
	  if(d26_i82_cmp_substr(f_substr,D26_IDX_NOT_DEF,d26_a_record,dname,
	     &pos,at) == D2_NO_ERR)
	  {   
	    return(D2_NO_ERR);
	  } 
	}
	else
	{
	  break;
	} 
      } 
      break;
    case    D2_GTEQ:
    case    D2_LTEQ:
      f_value = (D23_av_value *)((byte *)(f_type + 1) + f_type->d23_atoff);
      if(d26_i18_read_rec_val(f_value->d23_avlen,(char *)(f_value + 1),key,
	 file_nr,at,FALSE,FALSE,ISGTEQ,isfd,f_item->d23_fi1match,
	 d26_a_record) == D2_NO_ERR)
      {   
	return(D2_NO_ERR);
      } 
      break;
  }   
}

				    /*  return value                  */
return(D26_NOFILTERMATCH);

}                                   /*  d26_i81_cmp_rec_item          */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_i82_cmp_substr()                                */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 02.03.89                                            */
/*                                                                    */
/* DESCRIPTION  : This function will compare the filter and attribute */
/*                values from message and record using                */
/*                substring rule.                                     */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*      f_substr    D23_fs1_info *  pointer to filter substrings      */
/*      ofilenr     Short           number of object file             */
/*      record      String          C-ISAM record containing object   */
/*      dname       D2_name_string  distinguished name of object found*/
/*      pos         Short *         position of value in record       */
/*      at          D26_at *        description of filter attribute   */
/*                                                                    */
/* OUTPUT PARAMETERS:   None                                          */
/*                                                                    */
/* RETURN VALUE :   D2_NO_ERR:              object matches filter     */
/*                  D2_NFOUND               object doesn't match      */
/*                                          filter                    */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_i82_cmp_substr(D23_fs1_info *f_substr, signed16 ofilenr,
    char *record, D2_name_string dname, signed16 *pos, D26_at *at)

{                                   /*  d26_i82_cmp_substr            */

  signed16 atlen;               /* length of attribute value          */
  signed16 add_len;             /* length of internal added characters*/
  signed16 phon_len;            /* length of phonetical value         */
  signed16 rep_len;             /* length of representation           */
				/* memory to write ...                */
  byte     dn_val[D2_DNL_MAX + 1];  /* ... DN attribute value         */
  byte     acl[D2_ACLIDX_MAX * (1 + D2_DNL_MAX)];  /* ... acl         */
  byte     *comp_val;           /* attribute value to compare         */

  register signed16 len;        /* length of part of attribute value  */

phon_len = at->d26a_phon == TRUE ? D26_LPHON : 0;
rep_len  = at->d26a_rep == D2_T61_PR ? INTSIZE : 0;

switch(at->d26a_syntax)
{
  case  D2_CEXACT   :
  case  D2_PRINTABLE:
    comp_val = (byte *)record + *pos + phon_len + rep_len;
    atlen    = strlen((char *)comp_val);
    if(d26_f14_multigrep(comp_val,atlen,f_substr))
    {   
      *pos += phon_len + rep_len + at->d26a_ubound + 1;
      return(D2_NFOUND);
    } 
    break;
  case  D2_CIGNORE    :
  case  D2_IA5        :
  case  D2_CIGN_LIST  :
  case  D2_NUMERIC    :
  case  D2_TELNR_STX  :
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
    comp_val = (byte *)record + *pos + phon_len + rep_len;
    for(atlen = 0; atlen < at->d26a_ubound + add_len;
	atlen += len + 1, comp_val += len + 1)
    { 
      if((len = strlen((char *)comp_val)) == 0)
      {   
	break;
      } 
    } 
    atlen = atlen > 0 ? atlen - 1 : atlen;
    comp_val = (byte *)record + *pos + phon_len + rep_len;
    if(d26_f14_multigrep(comp_val,atlen,f_substr))
    {   
      *pos += phon_len + rep_len + 2 * (at->d26a_ubound + add_len);
      return(D2_NFOUND);
    } 
    break;
  case  D2_OCTET    :
    comp_val = (byte *)record + *pos;
    atlen    = ldint(record + *pos + at->d26a_ubound);
    if(d26_f14_multigrep(comp_val,atlen,f_substr))
    {   
      *pos += at->d26a_ubound + INTSIZE;
      return(D2_NFOUND);
    } 
    break;
  case  D2_DISTNAME:
    comp_val = dn_val;
    atlen    = d26_i56_atdnlen(record,pos,at,&comp_val,FALSE);
    if(comp_val == NULL)
    {   
      return(D2_NFOUND);
    }
    else if(d26_f14_multigrep(comp_val,atlen,f_substr))
    {   
      return(D2_NFOUND);
    } 
    break;
  case  D2_ACCL     :
    comp_val = acl;
    atlen = d26_i57_atacllen(dname,record,pos,comp_val,FALSE);
    if(d26_f14_multigrep(comp_val,atlen,f_substr))
    {   
      return(D2_NFOUND);
    } 
    break;
  default           :
    return(D2_NFOUND);
}   

			/*  return no error                           */
return(D2_NO_ERR);

}                                   /*  d26_i82_cmp_substr            */
