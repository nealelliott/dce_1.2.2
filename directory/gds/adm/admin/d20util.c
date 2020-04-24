/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20util.c,v $
 * Revision 1.1.730.2  1996/02/18  19:41:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:15  marty]
 *
 * Revision 1.1.730.1  1995/12/08  15:18:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:17  root]
 * 
 * Revision 1.1.728.5  1994/08/01  15:31:51  marrek
 * 	Bug fixes for OT 11295, 11456, 11458, 11471.
 * 	[1994/08/01  15:30:29  marrek]
 * 
 * Revision 1.1.728.4  1994/06/21  14:43:45  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:24:50  marrek]
 * 
 * Revision 1.1.728.3  1994/05/10  15:49:54  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:13:28  marrek]
 * 
 * Revision 1.1.728.2  1994/03/23  15:06:39  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:12:11  keutel]
 * 
 * Revision 1.1.728.1  1994/02/22  15:59:06  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:33:20  marrek]
 * 
 * Revision 1.1.726.2  1993/10/14  16:46:38  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  16:45:24  keutel]
 * 
 * Revision 1.1.726.1  1993/10/13  17:29:54  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  07:36:14  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  10:28:54  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:34:45  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:09:35  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:05:33  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:20:27  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:15:14  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  20:22:27  melman
 * 	Fixes CR 4508 core dump in gdsditadm
 * 	[1992/07/01  20:21:53  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  22:02:37  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:17:36  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20util.c,v $ $Revision: 1.1.730.2 $ $Date: 1996/02/18 19:41:32 $";
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
/* NAME         : util20.c    [Administrations-Schnittstelle]         */
/*								      */
/* AUTHOR       : Volpers, D AP 11                                    */
/* DATE         : 01.02.88                                            */
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
/* SYSTEMABHAENGIGKEIT:                                               */
/*                      COMMON                                        */
/*								      */
/* HISTORY 	:						      */
/*								      */
/* Vers.Nr. |  Date   |	 Updates    		       | KZ | CR# FM# */
/*      0.1 | 21.08.87| Erstellung                     | ws |         */
/*								      */
/*datoff **************************************************************/

/**********************************************************************/
/*								      */
/*		       Modulidentification			      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*			  I N C	L U D E	S			      */
/*								      */
/**********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define D20_ROOT	0

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern D20_omask d20_qomask[];         /* Input fields for error mask */
extern char      *d20_errtab[];        /* Error mask                  */

extern char      *d20_err_table[];

extern	D20_at	   *d2_p_d20at;			      
extern	signed32    d2_nd20at;			     

extern  D20_srt    *d2_p_d20srt;       /* pointer to SRT              */
extern  signed32    d2_nd20srt ;        /* number of SRT elements      */
extern  Bool	    d20_printable;

static 	char	shift_arr[NAM_ATTR_OFFSET + 1] = "   ";
static 	char	extra_nam_attr[D20_62CNT][DS_NP_NM_LEN + NAM_ATTR_OFFSET + 1];

/**********************************************************************/
/*								      */
/*		  D E C L A R A T I O N S      FUNCTIONS	      */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : ds0_512_object_tree()                                 */
/*								      */
/* AUTHOR:      ORTH    (D AP 11)                                     */
/* DATE:        05.09.87                                              */
/*								      */
/* SYNTAX:      ds0_512_object_tree(flds, tree, obj_entry, no_np)     */
/*              String          flds[];                               */
/*              char            tree[];                               */
/*              Ds_obj_entry    *obj_entry;                           */
/*              signed16        *no_np;		total no. of AVAs.    */
/*								      */
/* DESCRIPTION:                                                       */
/*		This function sets the pointers of mask fields and    */
/*		object entry array to names of naming attributes and  */
/*		sets the pointers of object entry array to the values */
/*		of naming attributes.				      */
/*		This function returns in *no_np total no. of fields   */
/*		(AVAs) used to define the object name.		      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              char            tree[];         tree structure        */
/*              String          flds[];                               */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              Ds_obj_entry   *obj_entry;                            */
/*              signed16       *no_np;                                */
/*								      */
/* RETURNVALUE:							      */
/*              None                                                  */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*		D20_srt		*d2_p_d20srt;			      */
/*		signed32	d2_nd20srt;			      */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                          sscanf()                                  */
/*                          strlen()                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 void ds0_512_object_tree(
   D20_omask       *mask_ptr,
   char             tree[],
   Ds_obj_entry    *obj_entry,
   signed16        *no_np,
   signed16         mask)
 
{                               /*  ds0_512_object_tree               */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	C L A R	A T I O	N S  			      */
/*								      */
/**********************************************************************/

   signed16 j, i, k;
   signed32 nr;
   D20_srt *ptr;
   signed32 obj_len;
   signed32 extra_nam_attr_cnt = 0;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/


j=0;                            /*  initialize counter                */
				/*  create object name from tree      */
for (k = 0; 3 * k < (signed32)strlen(tree); k++)
	{
	/* rule number cannot be read from tree string.	*/
    	if (sscanf(tree + 3 * k,"%2d",&nr) <= 0)
  		/*  break loop                        */
    		break;

	/* look for the rule no. in SRT and use naming attributes.	*/
	ptr = d20_search_adm_srule(d2_nd20srt, d2_p_d20srt, nr);

	/*  fill object entry                 */
	for (i=0; i<ptr->no_nam_attr; i++)
		{
		if (i == 0)
			mask_ptr->d20_ioinfo = ptr->nam_attr[i]->at.att_name;
		else
			{
			strcpy(extra_nam_attr[extra_nam_attr_cnt], shift_arr);
			strcat(extra_nam_attr[extra_nam_attr_cnt],
						ptr->nam_attr[i]->at.att_name);
			mask_ptr->d20_ioinfo = 
					extra_nam_attr[extra_nam_attr_cnt++];
			}

		mask_ptr++->d20_fkt_id = D20_CSAY;
		obj_entry[j].obj_name = mask_ptr->d20_ioinfo;
		obj_len = (ptr->nam_attr[i]->at.att_upper > D20_ONM_VAL_FLD_LEN)
			? D20_ONM_VAL_FLD_LEN : ptr->nam_attr[i]->at.att_upper;
		sprintf(mask_ptr->d20_oformat, "%%-%ds", obj_len);
		mask_ptr->d20_iolen = obj_len;
/*
		*(mask_ptr->d20_ioinfo + obj_len) = D2_EOS;
*/
		mask_ptr++->d20_fkt_id = (mask == D20_MASK6) ? D20_CGET : 
								D20_CSAY ;
		obj_entry[j].obj_length = ptr->nam_attr[i]->at.att_upper;
		obj_entry[j].attr_type =  ptr->nam_attr[i]->at.att_obj_id;
		obj_entry[j].attr_id.d2_type = ptr->nam_attr[i]->obj_id.d2_type;
		obj_entry[j].attr_id.d2_typ_len = ptr->nam_attr[i]->obj_id.
								d2_typ_len;
		if ((obj_entry[j].rep = (signed16)d27_401_syntax_to_rep(
				(signed16)ptr->nam_attr[i]->at.att_syntax)) ==
				D2_T61_PR)
			if (d20_printable == TRUE)
				obj_entry[j].rep = D2_PRINTABLE;
			else
				obj_entry[j].rep = D2_T61;

		j++;
		}
	}

*no_np = j;

}                               /*  ds0_512_object_tree               */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : ds0_513_hexa                                          */
/*								      */
/* AUTHOR:      ORTH    (D AP 11)                                     */
/* DATE:        09.09.87                                              */
/*								      */
/* SYNTAX:      signed16 ds0_513_hexa(h_flds,j)                       */
/*                                                                    */
/*              String          h_flds[];                             */
/*              signed16        j;                                    */
/*								      */
/* DESCRIPTION: This function checks whether all characters in the    */
/*              string "h_flds" are hexadecimal characters and whether*/
/*              the length of the string is correct (incl. x' ')      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              String          h_flds[];                             */
/*              signed16        j;                                    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       no error                                      */
/*      -1      :       error occurred --> check error codes          */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                  isxdigit()                                        */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 signed16 ds0_513_hexa(
   String   h_flds,
   signed16 j)
 
{                               /*  ds0_513_hexa                      */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

   signed16 ret_value = D2_NOERROR;/* Return-Value                   */
   signed16 i;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if                              /*  length is not the length of a hexa-string */
   ((j-3)%2 != 0)
{                               /*  display error message             */
  d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E21IDX];

  d20_handlemask(d20_qomask);

  ret_value = 1;
}
else
{                               
  for                           /*  all char                          */
      (i=2; i<=j-2; i++)
  {                             
    if                          /*  input is not a hexadecimal character */
       (!isxdigit((int)h_flds[i]))
    {                           /*  display error message             */
				/*  break                             */
      d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E21IDX];

      d20_handlemask(d20_qomask);

      ret_value = 1;
      break;
    }                           
  }                             
}                               

return(ret_value);

}                               /*  ds0_513_hexa                      */

/*exon ****************************************************************/
/*								      */
/* TYP		: C-FUNCTION					      */
/*								      */
/* NAME		: ds4_004_octncpy  [octncpy]			      */
/*								      */
/* AUTOR	: Ernst	Kraemer, DAP 11				      */
/* DATUM	: 20.03.86					      */
/*								      */
/* SYNTAX	: ds4_004_octncpy(to,from,n)			      */
/*		  Octet_string to;				      */
/*		  Octet_string from;				      */
/*		  signed16 n;					      */
/*								      */
/* DESCRIPTION  : This function copy n character - including special  */
/*		  characters and \0 from 'from' to 'to'.         */
/*								      */
/*								      */
/* EINGABEPARAMETER:						      */
/*		  from	 Octet_string	 Source address		      */
/*		  n	 signed16	 Number of characters to copy */
/*								      */
/* AUSGABEPARAMETER:						      */
/*		  to	 Octet_string	 Output address      */
/*								      */
/* RETURNVALUE	: void				      */
/*								      */
/*exoff	***************************************************************/

 void ds4_004_octncpy(
   Octet_string to,
   Octet_string from,
   signed16 n)

{   		    /* 	Entry octncpy				      */
/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

register  Octet_string max_adr = to + n;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

for		    /* 	All characters from 'from'    */
   (; to < max_adr; )
{ 		    /* 	Copy character  			      */
  *to++	= *from++;
} 		    

}  		    /* 	End octncpy				      */


 signed16 d20_strlen(
   char *str)
{
signed16 len;

len = strlen(str);
#ifdef TRACE
fprintf(stderr,"LENGTH d20_strlen: %d\n",len);
fprintf(stderr,"string: %s\n",str);
#endif
if (len)
   { str += (len - 1);
     while ((*str == DS_DEFAULT_VALUE || *str == ' ') && len > 0)
       { str--;
	 len--;
       }
   }
#ifdef TRACE
fprintf(stderr,"NEW - LENGTH: %d\n",len);
#endif

return(len);
}

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d20_octncmp  [octncmp]                              */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 20.03.86                                            */
/*								      */
/* SYNTAX       : signed16 d20_octncmp(str1,str2,n)                   */
/*		  Octet_string str1;				      */
/*		  Octet_string str2;				      */
/*		  signed16 n;					      */
/*								      */
/* DESCRIPTION  : This function compares n characters (incl. '\0') of */
/*                "str1" and "str2".                                  */
/*                If both strings are equal, a "0" will be returned.  */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		  str1	 Octet_string	 String1		      */
/*		  str2	 Octet_string	 String2		      */
/*                n      signed16        number of characters         */
/*								      */
/* OUTPUT_PARAMETERS:   -                                             */
/*								      */
/* RETURNVALUE	   :						      */
/*                0:    Strings are equal                             */
/*               -1:    Strings are not equal                         */
/*								      */
/* STANDARD-HEADER: -                                                 */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*                  -                                                 */
/*								      */
/* GLOBAL DATA (updated) :                                            */
/*                  -                                                 */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*exoff	***************************************************************/

 signed16 d20_octncmp(
   Octet_string str1,
   Octet_string str2,
   signed16 n)

{   		    /* 	Entry octncmp				      */
/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

register  Octet_string max_adr = str2 + n;
      signed16        ret_value = D2_NO_ERR;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

                    
		    /*  Return-Value = 0                              */
                    

for                 /*  all characters                                */
   ( ; str2 < max_adr; )
{                   /*  compare characters                            */
  if                /*  characters are not equal                      */
     (*str1++ != *str2++)
  {                 /*  return-Value = -1                             */
		    /*  break                                         */
    ret_value =	-1;
    break;
  }                 
} 		    

     		    
		    /*  return (return value)                         */
    		    

return(ret_value);

}  		    /* 	End octncmp				      */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME  :      d20_gen_dnintf     (...)                              */
/*								      */
/* AUTHOR:      SANJAY , SIEMENS (INDIA) 			      */
/* DATE:        22.10.90                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_gen_dnintf (dn)                        */
/*								      */
/* DESCRIPTION: The function converts DN as stored as an attribute    */
/*	  	value to the form suitable for admin. interface .     */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              String    dn                the DN which will be      */
/*                                          changed                   */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*		String    dn		Converted DN.		      */
/*								      */
/* RETURNVALUE:							      */
/*		Always returns NO_ERROR, since input is expected to   */
/*		be correct.					      */
/*								      */
/* STANDARD-HEADER:                                                   */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*              None                                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*								      */
/*exoff ***************************************************************/

 d2_ret_val d20_gen_dnintf (
   String    dn)

{                               /*  d20_gen_dnintf                     */

/**********************************************************************/
/*								      */
/*                 T Y P E - D E F I N I T I O N S                    */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/


register  signed32  i;
char	  dnintf[D20_ATVAL_LEN * D20_T61_CONV_FACT + 1];
char	  *dest_ptr;
char	  *src_ptr;
char	  *begdn;
D20_at	  *at_ptr;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if (strlen(dn) == 0)
	return(D2_NOERROR);

/* dn already in displayble format.	*/
if (*dn == D2_ADM_RDN_SEP)
	return(D2_NOERROR);

dest_ptr = dnintf;
src_ptr = begdn = dn;


for (; *dn != D2_EOS;  dn++)
	{
  	if (*dn == D2_T61_REP || *dn == D2_PRTBL_REP ||
  		*dn == D2_NUM_REP || *dn == D2_IA5_REP)
		{
		/* get naming attribute abbrv. from schema and copy to target */
		/* string.						      */

		*dn = D2_EOS;
		for (at_ptr = d2_p_d20at, i = 0; i < d2_nd20at; at_ptr++,i++)
			if (strcmp(src_ptr,at_ptr->at.att_obj_id) == 0)
				break;

		strcpy(dest_ptr,at_ptr->at.att_abbrv);
		dest_ptr += strlen(dest_ptr);
		*dest_ptr = D2_ADM_TYPE_VAL_SEP;
		dest_ptr++;
		src_ptr = dn + 1;
		continue;
		}

	if (*dn == D2_AVA_SEP)
		{
		/* copy naming attribute value to target string.	*/
		*dn = D2_EOS;
		strcpy(dest_ptr, src_ptr);
		dest_ptr += strlen(dest_ptr);
		*dest_ptr = D2_ADM_AVA_SEP;
		dest_ptr++;
		src_ptr = dn + 1;
		continue;
		}
		
	if (*dn == D2_RDN_SEP)
		{
		/* copy naming attribute value to target string.	*/
		*dn = D2_EOS;
		strcpy(dest_ptr, src_ptr);
		dest_ptr += strlen(dest_ptr);
		*dest_ptr = D2_ADM_RDN_SEP;
		dest_ptr++;
		src_ptr = dn + 1;
		continue;
		}
	}

/* copy naming attribute value to target string.	*/
strcpy(dest_ptr,src_ptr);
dest_ptr += strlen(dest_ptr);
*dest_ptr = D2_EOS;

*begdn = D2_ADM_RDN_SEP;
strcpy(begdn + 1,dnintf);

return(D2_NOERROR);

}                               /*  d20_gen_dnintf                    */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME  :      d20_cmp_objid     (...)                               */
/*								      */
/* AUTHOR:      SANJAY , SIEMENS (INDIA) 			      */
/* DATE:        05.11.90                                              */
/*								      */
/* SYNTAX:      Bool  d20_cmp_objid (objid_ptr1, objid_len2,	      */
/*						 objid_str2) 	      */
/*								      */
/* DESCRIPTION: The function compares two object ids for their equality*/
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		D2_a_type	*objid_ptr1; pointer to first obj_id  */
/*		signed32	objid_len2; length of second obj_id   */
/*					    including EOS	      */
/*		Ob_id_string	objid_str2; pointer to string of sec. */
/*					     obj_id		      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		TRUE		if two obj_ids are equal	      */
/*		FALSE		if two obj_ids are not equal	      */
/*								      */
/* STANDARD-HEADER:                                                   */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*              None                                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*								      */
/*exoff ***************************************************************/

 Bool d20_cmp_objid (
   D2_a_type	*objid_ptr1,
   signed32	 objid_len2,
   Ob_id_string	 objid_str2)

{                               /*  d20_cmp_objid                     */

/**********************************************************************/
/*								      */
/*                 T Y P E - D E F I N I T I O N S                    */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

objid_len2 = objid_len2 - 1;  /* exclude EOS character */

if (objid_len2 == objid_ptr1->d2_typ_len)
	if (memcmp(objid_ptr1->d2_type,objid_str2,objid_len2) == 0)
		return(TRUE);
	else
		return(FALSE);
else
	return(FALSE);
}

 void d20_error(
   D2_pbhead *pbhead_ptr,
   signed16 retcode,
   signed16 errclass,
   signed16 errvalue)
{
pbhead_ptr->d2_retcode  = retcode;
pbhead_ptr->d2_errclass = errclass;
pbhead_ptr->d2_errvalue = errvalue;
}

 void d20_errcode(
   Errcode *errcode_ptr,
   char retcode,
   char errclass,
   signed16 errvalue)
{
errcode_ptr->retcode  = retcode;
errcode_ptr->errclass = errclass;
errcode_ptr->errvalue = errvalue;
}


/********************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME  :      d20_198_distname_syntax    (...)                      */
/*								      */
/* AUTHOR:      G. Rustemeyer, SNI DAP 113                            */
/* DATE:        19.08.91                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_198_distname_syntax (distname, rulenr) */
/*								      */
/* DESCRIPTION: The function checks the syntax of every AVA of the    */
/*              given distinguished name                              */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              char            *distname;   pointer to disting. name */
/*              signed32         rulenr;     actual struct.of dsaname */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*              D2_ERROR        if syntax-check was successful        */
/*              D2_NO_ERR       if syntax-check failed                */
/*								      */
/* STANDARD-HEADER:                                                   */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*              d27_609_compute_cntry_bitlist ()                      */
/*								      */
/* NOTES:                                                             */
/*								      */
/*								      */
/*exoff ***************************************************************/

 d2_ret_val d20_198_distname_syntax (
   char     *distname,
   signed32  rulenr)

{
   /*--------------------------------------------------------------------*/
   /*                 T Y P E - D E F I N I T I O N S                    */
   /*--------------------------------------------------------------------*/

   /*--------------------------------------------------------------------*/
   /*                  D E C L A R A T I O N S                           */
   /*--------------------------------------------------------------------*/
   D20_srt       *srtptr ;
   static Bool    d20_cntrylst_okay = FALSE;
   signed32       type_length, val_length;
   byte          *type_val_ptr;
   Bool           all_ava_done = FALSE;
   Bool		  type_val_sep;
   char           fname_cntry [D27_LFILE_NAME];
   FILE          *fp;
   signed32       d27_609_return;
   signed32	  i;

   /*--------------------------------------------------------------------*/
   /*                            C O D E                                 */
   /*--------------------------------------------------------------------*/

   /*-------------------------------------------------------------*/
   /* Assign country-bitlist, if necessary                        */
   /*-------------------------------------------------------------*/
   if ( d20_cntrylst_okay != TRUE )
      {
	sprintf(fname_cntry, "%s%s%s%s", dcelocal_path, D27_CLIENT_DIR,
			      D20_ADM, D20_COUNTRIES);
	 if ( (fp = fopen (fname_cntry, "r")) == NULL )
	    {
	       d20_qomask[D20_Q1POS].d20_ioinfo =
				       d20_errtab [D20_E41IDX];
	       d20_handlemask(d20_qomask);
	       return (D2_ERROR);
	    }

	 d27_609_return = d27_609_compute_cntry_bitlist (fp);

	 if ( fclose (fp) < 0 )
	    {
	       d20_qomask[D20_Q1POS].d20_ioinfo =
				       d20_errtab [D20_E41IDX];
	       d20_handlemask(d20_qomask);
	       return (D2_ERROR);
	    }

	 if ( d27_609_return != D2_NOERROR )
	    {
	       d20_qomask[D20_Q1POS].d20_ioinfo =
				       d20_errtab [D20_E62IDX];
	       d20_handlemask(d20_qomask);
	       return (D2_ERROR);
	    }

	 d20_cntrylst_okay = TRUE;
      }

   /*-------------------------------------------------------------*/
   /* set "srtptr" to srt-structure corresponding to rulenr       */
   /*-------------------------------------------------------------*/

   srtptr = d20_search_adm_srule(d2_nd20srt, d2_p_d20srt, rulenr);

   type_val_ptr = (byte *) (distname + strlen (distname) - 1);

   /*-------------------------------------------------------------*/
   /* scan distingueshed name and call syntax-check for each AVA  */
   /*-------------------------------------------------------------*/
   while (!all_ava_done)
      {
   	 type_length = 0;
   	 val_length = 0;
   	 type_val_sep = FALSE;

   	 /*-------------------------------------------------------------*/
   	 /* set "value" to the position before last AVA type and value  */
   	 /*-------------------------------------------------------------*/

   	 while (*type_val_ptr != D2_RDN_SEP && *type_val_ptr != D2_AVA_SEP)
      	    {
	  	if (*type_val_ptr == D2_T61_REP || *type_val_ptr == D2_PRTBL_REP
	  	  || *type_val_ptr == D2_NUM_REP || *type_val_ptr == D2_IA5_REP)
			type_val_sep = TRUE;
	  	else
			if (type_val_sep == TRUE)
				type_length++;
			else
				val_length++;

	 	 type_val_ptr --;
      	    }

	 for (i=0; i<srtptr->no_nam_attr; i++)
		if (strncmp(srtptr->nam_attr[i]->at.att_obj_id, 
			    (char *)(type_val_ptr + 1), type_length) == 0 &&
		   (strlen(srtptr->nam_attr[i]->at.att_obj_id) == type_length))
			break;

	 /* no naming attribute match.	*/
	 if (i == srtptr->no_nam_attr)
	    {
	       d20_qomask[D20_Q1POS].d20_ioinfo =
				       d20_errtab [D20_E62IDX];
	       d20_handlemask(d20_qomask);
	       return (D2_ERROR);
	    }

	 switch (srtptr->nam_attr[i]->at.att_syntax)
	    {
	       case D2_COUNTRY_STX:
		    if ( val_length != 2 )
		       {
			  d20_qomask[D20_Q1POS].d20_ioinfo =
						  d20_errtab [D20_E62IDX];
			  d20_handlemask(d20_qomask);
			  return (D2_ERROR);
		       }
		    else
		       if ((signed16) d27_608_is_country(
			      type_val_ptr + 1 + type_length + 1) == D2_ERROR)
			  {
			     d20_qomask[D20_Q1POS].d20_ioinfo =
						     d20_errtab [D20_E62IDX];
			     d20_handlemask(d20_qomask);
			     return (D2_ERROR);
			  }
		    break;
	       case D2_CEXACT:
	       case D2_CIGNORE:
		    if (d27_601_is_T61String (type_val_ptr + 1 + type_length +1,
						val_length)
			    == D2_ERROR)
		       {
			  d20_qomask[D20_Q1POS].d20_ioinfo =
						  d20_errtab [D20_E62IDX];
			  d20_handlemask(d20_qomask);
			  return (D2_ERROR);
		       }
		    break;
	       default:
		    {
		       d20_qomask[D20_Q1POS].d20_ioinfo =
					       d20_errtab [D20_E62IDX];
		       d20_handlemask(d20_qomask);
		       return (D2_ERROR);
		    }
	    }

	 if (type_val_ptr == (Octet_string)distname)
	    	all_ava_done = TRUE;
	 else
	 	if (*type_val_ptr == D2_RDN_SEP)
	 		srtptr = srtptr->par_rule;

	 type_val_ptr--;
      }

if (srtptr->par_rule != D20_ROOT)
    {
       d20_qomask[D20_Q1POS].d20_ioinfo =
			       d20_errtab [D20_E62IDX];
       d20_handlemask(d20_qomask);
       return (D2_ERROR);
    }

return (D2_NOERROR);

}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME  :      d20_cmp_structure (...)                               */
/*								      */
/* AUTHOR:      SANJAY , SIEMENS (INDIA) 			      */
/* DATE:        22.10.90                                              */
/*								      */
/* SYNTAX:      Bool  d20_cmp_structure (dn, tree)                    */
/*		char	*dn;					      */
/*		char	*tree;					      */
/*								      */
/* DESCRIPTION: This function checks if dn (in iapl format) fits into */
/*		tree structure.					      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		char	*dn;					      */
/*		char	*tree;					      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		TRUE		if dn fits into tree structure.	      */
/*		FALSE		otherwise.			      */
/*								      */
/* STANDARD-HEADER:                                                   */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*              None                                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*								      */
/*exoff ***************************************************************/

 Bool d20_cmp_structure (
   char	*dn,
   char	*tree)

{	
/**********************************************************************/
/*								      */
/*                 T Y P E - D E F I N I T I O N S                    */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

char	    *nam_attr_type[NO_NAM_ATTR];
char        tmp_dn[D2_DNL_MAX + 1];
char	    *tmp_dn_ptr = tmp_dn;
signed32    i, j, k, nr, no_nam_attr;
D20_srt	    *srt_ent;
signed32    tree_str_len;    

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* copy dn to work area.	*/
strcpy(tmp_dn, dn);
tree_str_len = strlen(tree);

/* process one RDN at a time	*/
for (k = 0; 3 * k < tree_str_len; k++)
	{
	/* get next rule no. from tree string.	*/
    	sscanf(tree + 3 * k,"%2d",&nr);

	/* look for the rule no. in SRT and use naming attributes.	*/
	srt_ent = d20_search_adm_srule(d2_nd20srt, d2_p_d20srt, nr);

	/* extract naming attribute types from next RDN.	*/
	no_nam_attr = 0;
	nam_attr_type[no_nam_attr++] = tmp_dn_ptr;

	while (*tmp_dn_ptr != D2_RDN_SEP && *tmp_dn_ptr != D2_EOS)
		{
	  	if (*tmp_dn_ptr == D2_T61_REP || *tmp_dn_ptr == D2_PRTBL_REP ||
	  	    *tmp_dn_ptr == D2_NUM_REP || *tmp_dn_ptr == D2_IA5_REP)
			/* terminate last naming attribute type.	*/
			*tmp_dn_ptr = D2_EOS;

		if (*tmp_dn_ptr == D2_AVA_SEP)
			nam_attr_type[no_nam_attr++] = tmp_dn_ptr + 1;

		tmp_dn_ptr++;
		}

	if ((*tmp_dn_ptr == D2_RDN_SEP) && (3*(k+1) > tree_str_len))
		/* case when dn has more RDNs than indicated by tree string. */
		return(FALSE);

	if ((*tmp_dn_ptr == D2_EOS) && (3*(k+1) < tree_str_len))
		/* case when dn has less RDNs than indicated by tree string. */
		return(FALSE);
		
	*tmp_dn_ptr = D2_EOS;
	tmp_dn_ptr++;

	if (srt_ent->no_nam_attr == no_nam_attr)
		{
		/* check if all naming attributes of srt_ent are present in */
		/* the RDN.						    */
		for (i = 0; i < srt_ent->no_nam_attr; i++)
			{
			for (j = 0; j < no_nam_attr; j++)
				if (strcmp(nam_attr_type[j], 
				    srt_ent->nam_attr[i]->at.att_obj_id) == 0)
					break;

			/* attribute not found.	*/
			if (j == no_nam_attr)
				break;
			}	
		
		/* some attribute has not matched.	*/
		if (i < srt_ent->no_nam_attr)
			return(FALSE);
		}
	else
		return(FALSE);
	}

return(TRUE);

}	/* end of d20_cmp_structure()	*/



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_dsacmp()                              */
/* Description         : Compare two DSA name                      */
/* Return Value        : TRUE  if they are equal                   */
/*                       FALSE if not                              */
/* Author              : Jean-Marc Bonnaudet, SNI BU BA NM 123     */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 d20_dsacmp( char *dsa1, char *dsa2 )
{
char     norm_dsa1[D2_DNL_MAX + 3], norm_dsa2[D2_DNL_MAX + 3];
int      dsa1_len, dsa2_len;
signed16 return_value = FALSE;

    dsa1_len = strlen( dsa1 );
    dsa2_len = strlen( dsa2 );

    if( dsa1_len == dsa2_len );
    {
	d27_007_norm_name( dsa1, dsa1_len, norm_dsa1 );
	d27_007_norm_name( dsa2, dsa2_len, norm_dsa2 );
	return_value = ( strncmp( norm_dsa1,
				  norm_dsa2,
				  dsa1_len ) == 0 ) ? TRUE : FALSE;
    }

    return( return_value );
}



/* End of d20util.c */
