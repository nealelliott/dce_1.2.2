/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27sort.c,v $
 * Revision 1.1.10.2  1996/02/18  18:23:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:54  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:16:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:41  root]
 * 
 * Revision 1.1.8.2  1994/06/21  14:48:35  marrek
 * 	June 1994 code submission.
 * 	[1994/06/20  13:03:59  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  19:22:00  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:55:44  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:39:59  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  13:14:47  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  22:06:12  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:49:22  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  19:45:48  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:52:47  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:44:40  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27sort.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:23:57 $";
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
/*                                                                    */
/* NAME         : d27sort.c <Utility-Functions for DIR-Interfaces>    */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 19.03.92                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    |17.12.87 |  Birth                         | ek |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <gds.h>

/*****  Operating System - Includes  *****/

#include <stdlib.h>
#include <string.h>

/*****  external Includes  *****/

#include <dce/d2dir.h>
#include <dce/d27util.h>

/*****  internal Includes  *****/

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define D27_UNBLANK 0   /* indicates beginning of string              */
#define D27_BLANK   1   /* indicates white space in string            */
			/* format string for UTC-time strings         */

#define D27_MINDIACRIT  0xc1    /* smallest diacritical character     */
#define D27_MAXDIACRIT  0xcf    /* largest diacritical character      */

#define D27_CAP_AE      0xe1    /* capital AE diphtong                */
#define D27_CAP_DSTROKE 0xe2    /* capital D with stroke              */
#define D27_CAP_HSTROKE 0xe4    /* capital H with stroke              */
#define D27_CAP_IJ      0xe6    /* capital IJ ligature                */
#define D27_CAP_LDOT    0xe7    /* capital L with middle dot          */
#define D27_CAP_LSTROKE 0xe8    /* capital L with stroke              */
#define D27_CAP_OSLASH  0xe9    /* capital O with slash               */
#define D27_CAP_OE      0xea    /* capital OE ligature                */
#define D27_CAP_THORN   0xec    /* capital thorn, icelandic           */
#define D27_CAP_TSTROKE 0xed    /* capital T with stroke              */
#define D27_CAP_ENG     0xee    /* capital eng, lapp                  */

#define D27_SM_DSTROKE  0xf2    /* small D with stroke                */
#define D27_SM_ETH      0xf3    /* small eth, icelandic               */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

typedef struct                          /* array of AVAs              */
	{
	  D2_name_string pr_type;       /* printable attribute type   */
	  signed16       len_of_type;   /* length of attribute type   */
	  signed16       len_of_ava;    /* length of AVA              */
	  D2_a_type      hex_type;      /* attribute type in HEX      */
	  char           ob_id[D2_OBJ_IDL_MAX]; /* memory for object  */
					/* identifier                 */
	} D27_ava;

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

/***   LOCAL                ****/

static signed32 d27_019_sort_rdn(signed16 no_ava, D27_ava *ava);
static int      d27_020_cmp_ava(const void *ava1, const void *ava2);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d27_007_norm_name                                     */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        11.12.87                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the normed value of the given    */
/*              inp_string, which is supposed to be a distinguished   */
/*              name.                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d27_007_norm_name(
    char     *inp,      /* IN    -  String to normalize               */
    signed16 length,    /* IN    -  length of string to normalize     */
    char     *out)      /* OUT   -  normalized string                 */

{                               /* d27_007_norm_name                  */

				/* separators between AVAs or RDNs    */
  static char ava_sep[] = {D2_RDN_SEP,D2_AVA_SEP,D2_EOS};
				/* separators between attribute type  */
				/* and value                          */
  static char tv_sep[]  = {D2_T61_REP,D2_PRTBL_REP,D2_NUM_REP,D2_IA5_REP,
    D2_EOS};

  byte      *rep;       /* pointer to representation                  */
  byte      *sep;       /* pointer to separator                       */
			/* loop pointer to input string               */
  register  byte *inp_string = (byte *)inp;
			/* loop pointer to output string              */
  register  byte *out_string = (byte *)out;

  while                 /* end of input string not reached            */
       (*inp_string != D2_EOS)
  {
			/* find type value separator                  */
    rep         = (byte *)strpbrk((char *)inp_string,tv_sep);
    length      = rep - inp_string;
			/* copy type                                  */
    memcpy(out_string,inp_string,length);
    inp_string += length + 1;
    out_string += length;
    sep         = (byte *)strpbrk((char *)inp_string,ava_sep);
    length = sep == NULL ? strlen((char *)inp_string) : sep - inp_string;
    switch(*rep)
    {
      case D2_T61_REP:
	*out_string++ = *rep;
	d27_651_norm_T61String(inp_string,out_string,length);
	break;
      case D2_PRTBL_REP:
	*out_string++ = D2_T61_REP;
	d27_031_norm_pr_string(inp_string,out_string,length);
	break;
      case D2_NUM_REP:
	*out_string++ = *rep;
	memcpy(inp_string,out_string,length);
	break;
      case D2_IA5_REP:
	*out_string++ = *rep;
	d27_031_norm_pr_string(inp_string,out_string,length);
	break;
    }
    inp_string += length;
    out_string += length;
    if                          /* AVA separator is following         */
      (sep != NULL)
    {                           /* copy separator                     */
      *out_string++ = *inp_string++;
    }
  }

  *out_string = D2_EOS;

  return;

}                               /* d27_007_norm_name                  */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d27_012_remove_blanks()                               */
/*                                                                    */
/* AUTHOR:      E. Kraemer, D AP 11                                   */
/* DATE:        20.07.88                                              */
/*                                                                    */
/* DESCRIPTION: This function removes starting and trailing blanks    */
/*              from inp_string as well as multiple blanks from       */
/*              the middle of the value.                              */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              String    inp               contains the string whose */
/*                                          blanks should be removed  */
/*                                                                    */
/*              Short     length            length of the inp_string  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              String    out               contains the string       */
/*                                          without forbidden blanks  */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              Ushort                      length of out_string      */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*              None                                                  */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d27_012_remove_blanks(
    char     *inp,      /* IN    -  String to remove blanks from      */
    signed16 length,    /* IN    -  length of string to remove blanks */
    char     *out)      /* OUT   -  String whith blanks removed       */

{                               /* d27_012_remove_blanks              */
/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

  signed16 mode = D27_BLANK;
  signed16 len  = 0;
  register signed16 i;
  register byte *inp_string = (byte *)inp;
  register byte *out_string = (byte *)out;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

  for                           /*  all characters of input string    */
     (i = 0; i < length; i++, inp_string++)
  {                             /*  check mode                        */
    switch(mode)
    {
      case D27_BLANK:
	if                      /*  separator encountered             */
	  (*inp_string == D2_AVA_SEP || *inp_string == D2_RDN_SEP ||
	   *inp_string == D2_EOS)
	{
	  if(len > 0)
	  {
	    if(*(out_string - 1) == D2_BLANK)
	    {
	      *(out_string - 1) = *inp_string;
	      break;
	    }
	  }
	  *out_string++ = *inp_string;
	  len++;
	  break;
	}
	else if                 /*  printable character encountered   */
	       (*inp_string != D2_BLANK)
	{
	  *out_string++ = *inp_string;
	  len++;
	  if                    /*  not representation character      */
	    (*inp_string != D2_T61_REP && *inp_string != D2_PRTBL_REP)
	  {
	    mode = D27_UNBLANK;
	  }
	}
	else
	{                       /*  check whether only  o n e  blank  */
				/*  is given in the Distinguished Name*/
	  if (len > 0)
	  {                     /* check whether attribute value      */
				/* begins                             */
	    if                  /* attribute value begins             */
	      (*(out_string - 1) == D2_T61_REP ||
	       *(out_string - 1) == D2_PRTBL_REP ||
	       *(out_string - 1) == D2_NUM_REP ||
	       *(out_string - 1) == D2_IA5_REP)
	    {                   /* check whether next character is    */
				/* delimiter                          */
	      if (i < length - 1)
	      {
		if
		  (*(inp_string + 1) == D2_AVA_SEP ||
		   *(inp_string + 1) == D2_RDN_SEP ||
		   *(inp_string + 1) == D2_EOS)
		{
				/* copy blank                         */
		  *out_string++ = *inp_string++;
				/* copy next character                */
		  *out_string++ = *inp_string;
		  len += 2;
		  i++;
		}
	      }
	    }
	  }
	}
	break;
      case D27_UNBLANK:
	*out_string++ = *inp_string;
	len++;
	if                      /* printable string ends              */
	  (*inp_string == D2_BLANK && (len == 1 ||
	   (signed16)*(inp_string - 1) <= D27_MINDIACRIT ||
	   (signed16)*(inp_string - 1) >= D27_MAXDIACRIT) ||
	   *inp_string == D2_T61_REP || *inp_string == D2_PRTBL_REP ||
	   *inp_string == D2_NUM_REP || *inp_string == D2_IA5_REP ||
	   *inp_string == D2_AVA_SEP || *inp_string == D2_RDN_SEP ||
	   *inp_string == D2_EOS)
	{
	  mode = D27_BLANK;
	}
	break;
    }
  }

  if                            /*  characters have been written      */
    (len > 0)
  {                             /*  check last character              */
    if                          /*  last character is blank           */
      (*(out_string - 1) == D2_BLANK && (len == 1 ||
       (signed16)*(out_string - 2) <= D27_MINDIACRIT ||
       (signed16)*(out_string - 2) >= D27_MAXDIACRIT))
    {                           /*  reduce length                     */
      len--;
    }
  }

  if                            /*  new length is zero && old length  */
				/*  is equal 1                        */
    (len == 0 && length == 1)
  {                             /*  at least one blank is given       */
    *out_string = D2_BLANK;
    len++;
  }

  return(len);

}                               /* d27_012_remove_blanks              */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d27_018_sort_name                                     */
/*                                                                    */
/* AUTHOR:      E. Kraemer, AP 11                                     */
/* DATE:        18.03.92                                              */
/*                                                                    */
/* DESCRIPTION: This function scans the distinguished name. If one    */
/*              of the RDNs consist of multiple AVAs, they are sorted */
/*              in the ordering of their attribute types.             */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   Short                   No error occurred         */
/*      D2_ERROR                            error occurred            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d27_018_sort_name(
			/* INOUT -  Distinguished name to be sorted   */
    D2_name_string dname)

{                               /* d27_018_sort_name                  */
/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

  D2_name_string rdn;                   /* pointer to beginning of RDN*/
  signed16       no_ava;                /* number of AVAs in one RDN  */
  D27_ava        ava[D2_NO_AVA];        /* array of AVAs              */

  register D2_name_string namep;        /* loop pointer to single     */
					/* character in name          */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

for                     /* all characters of the name                 */
   (namep = rdn = ava[0].pr_type = dname, no_ava = 0; ; namep++)
{                       /* check whether end of type is reached       */
  if                    /* end of type is reached                     */
    (*namep == D2_T61_REP || *namep == D2_PRTBL_REP ||
     *namep == D2_NUM_REP || *namep == D2_IA5_REP)
  {                     /* set attribute type length in AVA           */
    ava[no_ava].len_of_type = namep - ava[no_ava].pr_type;
  }                     /*                                            */
  if                    /* end of AVA is reached                      */
    (*namep == D2_AVA_SEP)
  {                     /* increment number of AVAs                   */
    ava[no_ava].len_of_ava = namep - ava[no_ava].pr_type;
    no_ava++;
    if                  /* number of AVAs exceeds limit               */
      (no_ava >= D2_NO_AVA)
    {                   /* return error                               */
      return(D2_ERROR);
    }
    else
    {                   /* set new AVA type pointer                   */
      ava[no_ava].pr_type = namep + 1;
    }                   /*                                            */
  }                     /*                                            */
  if                    /* end of RDN is reached                      */
    (*namep == D2_RDN_SEP || *namep == D2_EOS)
  {                     /* increment count of AVAs                    */
    ava[no_ava].len_of_ava = namep - ava[no_ava].pr_type;
    no_ava++;
    if                  /* more than one AVA is in RDN                */
      (no_ava > 1)
    {                   /* sort the RDN                               */
      if                /* RDN cannot be sorted                       */
	(d27_019_sort_rdn(no_ava,ava) == D2_ERROR)
      {                 /* return error                               */
	return(D2_ERROR);
      }                 /*                                            */
    }                   /*                                            */
    if                  /* end of name is not reached                 */
      (*namep == D2_RDN_SEP)
    {                   /* reset the numbers and pointers             */
      no_ava = 0;
      rdn = ava[0].pr_type = namep + 1;
    }
    else
    {                   /* end of name is reached                     */
      break;
    }                   /*                                            */
  }                     /*                                            */
}                       /*                                            */

return(D2_NO_ERR);

}                               /*  d27_018_sort_name                */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d27_019_sort_rdn                                      */
/*                                                                    */
/* AUTHOR:      E. Kraemer, AP 11                                     */
/* DATE:        18.03.92                                              */
/*                                                                    */
/* DESCRIPTION: This function converts the object identifier from     */
/*              one RDN into hex format. It sorts the AVAs then along */
/*              ascending object identifiers, copies the printed      */
/*              AVAs in this ordering to another piece of memory and  */
/*              recopies this sorted RDN to the old place.            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   signed32                No error occurred         */
/*      D2_ERROR                            error occurred            */
/*                                                                    */
/*exoff ***************************************************************/

static signed32 d27_019_sort_rdn(
    signed16 no_ava,    /* IN    -  Number of AVAs in RDN             */
    D27_ava  *ava)      /* INOUT -  Array of AVAs to be sorted        */
{                               /* d27_019_sort_rdn                   */
/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

  D2_name_string rdn;               /* pointer to beginning of RDN    */

  char sort_rdn[D2_DNL_MAX];        /* memory for sorted RDN          */
  char separator;                   /* separates type and value       */

  register char *sortp;             /* loop pointer to sorted RDN     */
  register signed16 i;              /* loop variable                  */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

rdn = ava[0].pr_type;

for                     /* all AVAs                                   */
   (i = 0; i < no_ava; i++)
{                       /* convert the object identifier              */
  ava[i].hex_type.d2_type = (Ob_id_string)ava[i].ob_id;
  separator = ava[i].pr_type[ava[i].len_of_type];
  ava[i].pr_type[ava[i].len_of_type] = D2_EOS;
  if                    /* object identifier cannot be converted      */
    (d27_302_str_oid((char *)ava[i].pr_type,&ava[i].hex_type) < 0)
  {                     /* return error                               */
    return(D2_ERROR);
  }                     /*                                            */
  ava[i].pr_type[ava[i].len_of_type] = separator;
}                       /*                                            */

qsort((void *)ava,no_ava,sizeof(D27_ava),d27_020_cmp_ava);

for                     /* all sorted AVAs                            */
   (i = 0, sortp = sort_rdn; i < no_ava; i++, sortp++)
{                       /* copy AVA to memory for sorted RDN          */
  memcpy(sortp,ava[i].pr_type,ava[i].len_of_ava);
  sortp += ava[i].len_of_ava;
  *sortp = D2_AVA_SEP;
}                       /*                                            */

memcpy(rdn,sort_rdn,sortp - sort_rdn - 1);

return(D2_NO_ERR);

}                               /* d27_019_sort_rdn                   */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d27_020_cmp_ava                                     */
/*                                                                    */
/* AUTHOR       : E. Kraemer, AP 11                                   */
/* DATE         : 19.03.92                                            */
/*                                                                    */
/* DESCRIPTION  : The function compares two AVAs using their octal    */
/*                object identifier.                                  */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      -1          Int     First AVA is "smaller" than second        */
/*       0          Int     Both  AVAs are equal                      */
/*      +1          Int     First AVA is "greater" than second        */
/*                                                                    */
/**********************************************************************/

static int d27_020_cmp_ava(
    const void *ava1,   /* IN    -  first AVA to compare              */
    const void *ava2)   /* IN    -  second AVA to compare             */

{                       /* d27_020_cmp_ava                            */

  int          ret_value;           /* return value                   */
  Ob_id_string ob_id;   /* temporary pointer to object identifier     */

/**********************************************************************/
/*                                                                    */
/*                    C O D E                                         */
/*                                                                    */
/**********************************************************************/

ret_value = (int)d27_303_cmp_obj_id(&((D27_ava *)ava1)->hex_type,
				    &((D27_ava *)ava2)->hex_type);

if                          /* AVAs must be exchanged                 */
  (ret_value > 0)
{                           /* exchange the pointer to the object ids */
  ob_id = ((D27_ava *)ava1)->hex_type.d2_type;
  ((D27_ava *)ava1)->hex_type.d2_type = ((D27_ava *)ava2)->hex_type.d2_type;
  ((D27_ava *)ava2)->hex_type.d2_type = ob_id;
}                           /*                                        */

return(ret_value);

}                       /* d27_020_cmp_ava                            */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_031_norm_pr_string                              */
/*                                                                    */
/* Description  : This function norms a printable string              */
/*                                                                    */
/* Return value    :                                                  */
/*   None                                                             */
/*                                                                    */
/**********************************************************************/

void d27_031_norm_pr_string(
    byte *in,           /* IN    -  string to norm                    */
    byte *out,          /* IN    -  normed string                     */
    signed16 len)       /* IN    -  length of string to norm          */

{                       /* d27_031_norm_pr_string                     */

			/* end of value to check                      */
  byte *endval = in + len;

  while                 /* still character to check                   */
       (in < endval)
  {                     /* norm the character                         */
    d27_681_norm_102_char(&in,&out);
  }

}                       /* d27_031_norm_pr_string                     */

