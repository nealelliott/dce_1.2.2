/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20restore.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:36  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:11:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:41  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:02:50  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:01:53  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:07  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:13:25  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:24:03  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:04:41  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:44:46  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:58:19  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/18  08:24:53  marrek
 * 	Various bug fixes for mask handling found at SNI
 * 	[1992/12/17  16:23:43  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:45:39  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:37:05  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20restore.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:39 $";
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
/* NAME         : d20restore.o   [restoring of subtrees]              */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 20.04.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOK.-NR.	: DS-Design-Spez.				      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*      0.1 | 20.04.88| Original                       | ws |         */
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
#include <stdlib.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d2dump.h>
#include <gdsext.h>
#include <d2info.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                 D E C L A R A T I O N S        DATA                */
/*								      */
/**********************************************************************/

extern D2_pbhead *d20_head;
extern Ds_v2_info info;

 char      *d20_beg1;             /* start address of DN-strings*/
 char      *d20_beg2;             /* start address of DN-strings*/

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

static  signed16 d20_handle_dns(int fd1, D20_offset **dn1, signed16 dn_no1, 
		char *at1_mem, int fd2, D20_offset **dn2, signed16 dn_no2,
		char *at2_mem, Name_string dst_dsa_name, char *at_val_area);

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_100_restore()                                     */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        20.04.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_100_restore(file1, fd1, name1,           */
/*                                    pdn1, dn1, dn_no1,              */
/*                                    attr1_len, file2 ,dst_dsa_name) */
/*              String       file1;                                   */
/*              int          fd1;                                     */
/*              char       **name1;                                   */
/*              D20_offset **pdn1;                                    */
/*              D20_offset **dn1;                                     */
/*              signed16     dn_no1;                                  */
/*              signed32     attr1_len;                               */
/*              String       file2;                                   */
/*		Name_string	dst_dsa_name;			      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              The dumped information of DSA1 is already in memory.  */
/*                  Afterwards the "search file of DSA2" (containing  */
/*              the information of DSA2) will be taken and the        */
/*              internally information (of DSA2) will be generated.   */
/*              Both internally informations will be checked against  */
/*              each other and the appropriate update function will   */
/*              be chosen.                                            */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              String       file1;    "search file " of DSA1         */
/*              int          fd1;      file descriptor of file1       */
/*              char       **name1;    pointer to memory area for DNs */
/*                                     of DSA1                        */
/*              D20_offset **pdn1;     pointer array of DN-offsets    */
/*              D20_offset **dn1;      offsets of DN (of DSA1) relativ*/
/*                                     to beginning of file or memory */
/*                                     area                           */
/*              signed16     dn_no1;   number of DNs found in DSA1    */
/*              signed32     attr1_len;     maximum length of         */
/*                                          attribute list for one DN */
/*              String       file2;    "search file" of DSA2          */
/*		Name_string  dst_name_string; target DSA name.	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred                                */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 signed16 d20_100_restore(
   String       file1,
   int          fd1,
   char        *name1,
   D20_offset **pdn1,
   D20_offset **dn1,
   signed16     dn_no1,
   signed32     attr1_len,
   String       file2,
   Name_string  dst_dsa_name)

{                               /*  d20_100_restore()                 */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

    D20_offset *dn2;        /* offsets of DN (of DSA2) relative   */
				/* to beginning of file or memory     */
				/* area                               */

    D20_offset  *pdn2;

    signed32     dn_no2;    /* number of DNs found in DSA2        */

    char        *name2;     /* pointer to memory area for DNs     */
    signed32     l_name2;   /* length of all DNs of DSA2          */

    int          fd2;       /* file descriptor of file2           */

    signed32     attr2_len; /* max. length of attribute list for  */
				/* DNs of DSA2                        */

    char        *at1_mem = NULL;  /* memory area for attribute */
					 /* list of DSA1              */
					 /* and for DN                */

    char        *at2_mem = NULL;  /* memory area for attribute */
					 /* list of DSA2              */
					 /* and for DN                */

    signed16    ret_value = D2_NOERROR;
char		*at_val_area;	/* pointer to memory area to store attribute */
			        /* values. (added because of complex attrs.) */

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/*  generate DN-list for DSA2         */
/*  operation fails                   */
if   (d20_105_gen_dn(TRUE, file2, &fd2, &name2, &l_name2, &pdn2, &dn2,
		   &dn_no2, &attr2_len))
	{
	/*  return(D2_ERROR)                  */
	d20_111_free((char *)pdn1, (char *)dn1, name1, (char *)NULL, 
		    (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL, 
		    (char *)NULL, (char *)NULL);
  	return(D2_ERROR);
	}
else
	{
  	/* set install flag for alias objects.	*/
  	if ((ret_value = d20_099_set_install_alias(info.bind_id,
				(D20_offset **) pdn2,dn_no2, name2, file2,
				attr2_len)) == D2_ERROR)
		{
		d20_111_free((char *)pdn1, (char *)dn1, name1, (char *)NULL,
			(char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL, 
		    	(char *)NULL, (char *)NULL);
    		return(ret_value);
		}

	/*  allocate memory for attribute lists*/
  	if ((at_val_area = malloc(attr1_len + attr2_len + 
			         (D2_AT_MAX * max_comp_attr() * 2))) == NULL)
		{
       		d20_head->d2_errvalue = D20_ALLOC_ERR;
       		ret_value = D2_ERROR;
       		}

  	if (attr1_len)
		{
    		attr1_len += D2_ALIGN(D2_DNL_MAX);

    		if (attr1_len < D20_BUFSIZ)
      			attr1_len = D20_BUFSIZ;

    		if ((at1_mem = malloc((size_t)attr1_len)) == NULL)
			{
      			d20_head->d2_errvalue = D20_ALLOC_ERR;
      			ret_value = D2_ERROR;
			}
		}

  	if (attr2_len && !ret_value)
		{
    		attr2_len += D2_ALIGN(D2_DNL_MAX);

    		if (attr2_len < D20_BUFSIZ)
      			attr2_len = D20_BUFSIZ;

    		if ((at2_mem = malloc((size_t)attr2_len)) == NULL)
			{
      			d20_head->d2_errvalue = D20_ALLOC_ERR;
      			ret_value = D2_ERROR;
			}
		}
	}

if                              /*  no errors occurred                */
   (!ret_value)
{                               
  d20_beg1 = name1;
  d20_beg2 = name2;
				/*  handling of DNs of both DSAs ok   */
  ret_value = d20_handle_dns(fd1, pdn1, dn_no1, at1_mem, fd2, (D20_offset **)pdn2,
				   dn_no2, at2_mem, dst_dsa_name,at_val_area);
} 

d20_111_free((char *)pdn1, (char *)dn1, name1, (char *)pdn2, (char *)dn2, name2,
	     (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL);

if (at_val_area)
     free(at_val_area);
if (at1_mem)
{   
  free(at1_mem);
} 
if (at2_mem)
{   
  free(at2_mem);
} 
close(fd1);
close(fd2);

				/*  return(ret_value)                 */
return(ret_value);

}                               /*  d20_100_restore()                 */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_101_compare_dn()                                  */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        20.04.88                                              */
/*								      */
/* SYNTAX:      signed32 d20_101_compare_dn(dn1, dn2)                 */
/*              D20_offset **dn1; 	  	                      */
/*              D20_offset **dn2;	                              */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will compare Distinguished Names pointed*/
/*              to by a pointer array.                                */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D20_offset **dn1;       pointer to DN1                */
/*              D20_offset **dn2;       pointer to DN2                */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*              -1:     DN1 < DN2                                     */
/*               0:     DN1 = DN2                                     */
/*               1:     DN1 > DN2                                     */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                      strcmp()                                      */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 signed32 d20_101_compare_dn(
   D20_offset **dn1,
   D20_offset **dn2)

{                               /*  d20_101_compare_dn()              */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

register int ret_value;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if ((ret_value = strcmp(d20_beg1 + (*dn1)->d20_mem_off,
			d20_beg2 + (*dn2)->d20_mem_off)) < 0)
{   
  return(-1);
}
else if (ret_value > 0)
{   
  return(1);
}
else
{
  return(0);
} 

}                               /*  d20_101_compare_dn()              */


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_handle_dns()                                      */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        20.04.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_handle_dns(fd1, dn1, dn_no1, at1_mem,    */
/*				   fd2, dn2, dn_no2, at2_mem,         */
/*				   dst_dsa_name, at_val_area)	      */
/*              int          fd1;                                     */
/*              D20_offset **dn1;                                     */
/*              signed16     dn_no1;                                  */
/*              char        *at1_mem;                                 */
/*              int          fd2;                                     */
/*              D20_offset **dn2;                                     */
/*              signed16     dn_no2;                                  */
/*              char        *at2_mem;                                 */
/*		Name_string dst_dsa_name;			      */
/*		char	   *at_val_area;			      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will compare Distinguished Names in both*/
/*              DSAs and will update the second DSA according to the  */
/*              results of the comparisions.                          */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              int          fd1;       file descriptor of file1(DSA1)*/
/*              D20_offset **dn1;       pointer list to DNs in DSA1   */
/*              signed16     dn_no1;    number of DNs in DSA1         */
/*              char        *at1_mem;   memory area for attribute list*/
/*                                      of DNs of DSA1                */
/*                                      and for DN                    */
/*              int          fd2;       file descriptor of file2(DSA2)*/
/*              D20_offset **dn2;       pointer list to DNs in DSA2   */
/*              signed16     dn_no2;    number of DNs in DSA2         */
/*              char        *at2_mem;   memory area for attribute list*/
/*                                      of DNs of DSA2                */
/*                                      and for DN                    */
/*		Name_string dst_dsa_name; Target DSA name.	      */
/*		char	*at_val_area;  pointer to area for attr.      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*              D2_NOERROR: operation is ok                           */
/*              D2_ERROR:   operation fails                           */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*inoff ***************************************************************/

 static  signed16 d20_handle_dns(
   int           fd1,
   D20_offset  **dn1,
   signed16      dn_no1,
   char         *at1_mem,
   int           fd2,
   D20_offset  **dn2,
   signed16      dn_no2,
   char         *at2_mem,
   Name_string   dst_dsa_name,
   char		*at_val_area)

{                               /*  d20_handle_dns()                  */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

	Bool	name_col;

 D20_offset **dn1_max = dn1 + dn_no1;
 D20_offset **dn2_max = dn2 + dn_no2;

 D20_offset **dn_sav = dn2;

 signed32      min1 = 0; /* minimum position of read attributes 1  */
 signed32      max1 = 0; /* maximum position of read attributes 1  */
 signed32      min2 = 0; /* minimum position of read attributes 2  */
 signed32      max2 = 0; /* maximum position of read attributes 2  */

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/*  DNs in both DSA exist             */
while      (dn1 < dn1_max && dn2 < dn2_max)
	{
	/*  compare DNs                       */
  	switch ((int) d20_101_compare_dn(dn1, dn2))
		{
    		case -1:                    
			/*  add new DN to DSA2                */
			/*  operation fails                   */
		   	if (d21t003_add_appnd(fd1, *dn1, at1_mem, &min1, &max1,
				      OLD_ACL, (Name_string)NULL,
				      (Name_string)NULL, FALSE,
				      dst_dsa_name,0, (char *)NULL,
				      at_val_area,CREATE_SHD))
				{
				/*  return(D2_ERROR)                  */
		  		return(D2_ERROR);
				}

				/*  take next DN from DSA1            */
				dn1++;
				break;

    		case 0:     
			/*  DN has to be installed            */
		   	if (((*dn1)->d20_state & D20_INSTALL) == D20_INSTALL)
				/*  modify entry                      */
		  		/*  operation fails                   */
		     		if (d21t004_modify_appnd(fd1, *dn1, at1_mem, 
				    &min1, &max1, fd2, *dn2, at2_mem, &min2, 
				    &max2, OLD_ACL, (Name_string)NULL, 
				    (Name_string)NULL, FALSE, 
				    (char *)NULL, 0, dst_dsa_name, 
				    TRUE, &name_col, at_val_area,CREATE_SHD))
		  			{
					/*  return(D2_ERROR)                  */
		    			return(D2_ERROR);
					}

			/*  mark DN2 as handled               */
			*dn2 = NULL;

			/*  take next DN from DSA1 and DSA2   */
			dn1++;
			dn2++;
			break;

    		case 1:                     
			/*  mark entry to be removed          */
			/*  take next DN from DSA2            */
			dn2++;
			break;
		}
	}

while                           /*  DNs in first DSA exist            */
      (dn1 < dn1_max)
{                               /*  add DN to second DSA              */
  if                            /*  operation fails                   */
     (d21t003_add_appnd(fd1, *dn1, at1_mem, &min1, &max1,
			      OLD_ACL, (Name_string)NULL,
			      (Name_string)NULL, FALSE,
			      dst_dsa_name,0, (char *)NULL,
			      at_val_area,CREATE_SHD))
  {                             /*  return(D2_ERROR)                  */
    return(D2_ERROR);
  }                             
				/*  take next DN from DSA1            */
  dn1++;
}                               

dn2 = dn2_max - 1;
while                           /*  DNs in second DSA exist           */
      (dn2 >= dn_sav)
{                               
  if                            /*  DN not yet handled                */
     (*dn2)
  {                             
    if                          /*  DN has to be removed              */
       (((*dn2)->d20_state & D20_INSTALL) == D20_INSTALL)
    {                           /*  remove DN in second DSA           */
      d20_162_set_name(d20_beg2 + (*dn2)->d20_mem_off);
      if                        /*  operation fails                   */
	 (d20_call_iapl(D20_2_DEL_OBJ))
      {                         /*  return(D2_ERROR)                  */
	d20_head->d2_errvalue = D20_DEL_ERR;
	return(D2_ERROR);
      }                         
    }                           
  }                             
				/*  take next DN from DSA2            */
  dn2--;
}                               

				/*  return(D2_NOERROR)                */
return(D2_NOERROR);

}                               /*  d20_handle_dns()                  */
