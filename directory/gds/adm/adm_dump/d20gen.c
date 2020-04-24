/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20gen.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:26  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:10:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:30  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:02:44  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:01:21  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:51:35  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:12:13  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:09:31  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:03:04  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:43:54  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:57:46  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:43:58  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:36:16  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20gen.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:31 $";
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
/* NAME         : d20gen.o     [generation of DN-list]                */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 27.04.88                                            */
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
/*      0.1 | 27.04.88| Original                       | ws |         */
/*      0.2 | 12.12.88| Absturz bei dump & restore     | hv |     49  */
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
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23apdu.h>
#include <d2dump.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define NR_ADDR		10	/* Number of addresses to release */

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
extern int        d20_n_bytes;          /* number of bytes already    */
					/* read                       */
extern char d20_m_dsa[];    /* master DSA of objects that     */
				    /* shall be propagated later on   */
				    /* via "delta_updates"            */

static   signed32       d20_dn_state;

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

static  signed16 d20_108_build_offset(D20_dn_off *dn_str,
				   signed32 f_offset,
				   signed32 m_offset);
static  signed16 d20_109_check_memory(unsigned16 new_anz,
				   signed32      act_anz,
				   char       **dn_names,
				   signed32     *max_anz);
static  signed16 d20_110_check_dn(char        *dn_names,
			       signed32     f_offset,
			       signed32    *m_offset,
			       D20_dn_off  *dn_str,
			       unsigned16   dn_length);
static  signed16 d20_skip(int fd, unsigned16 *tag, signed32 *no_bytes);
 

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_106_get_dn_offsets()                              */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        20.04.88                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_106_get_dn_offsets(ignore, fd, ext_dn, */
/*                                           int_dn, dn, mk,          */
/*                                           no_bytes, attr_len       */
/*              Bool         ignore;                                  */
/*              int          fd;                                      */
/*              D20_dns     *ext_dn;                                  */
/*              D20_dns     *int_dn;                                  */
/*              D20_dn_off  *dn;                                      */
/*              D20_dn_off  *mk;                                      */
/*              signed32    *no_bytes;                                */
/*              signed32    *attr_len;                                */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function reads the search result message and     */
/*              determines all DNs in results. The DNs are copied in  */
/*              memory and a list of all DNs (aliased objects and     */
/*              DSA-names) that must be read additionally is created. */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              Bool         ignore;        a flag indicating whether */
/*                                          error on reading first tag*/
/*                                          (--> search file contains */
/*                                          nothing) can be ignored   */
/*              int          fd;            file descriptor for       */
/*                                          search result file        */
/*              D20_dns     *ext_dn;        external DNs (real objects*/
/*                                          and alias objects)        */
/*              D20_dns     *int_dn;        internal DNs (MK and      */
/*                                          alaised objects)          */
/*              D20_dn_off  *dn;            DN-offset structure       */
/*              D20_dn_off  *mk;            MK offset structure       */
/*              signed32    *no_bytes;      number of bytes read in   */
/*                                          search result file        */
/*              signed32    *attr_len;      max. length of longest    */
/*                                          attribute list            */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              see above: the pointers in the offset structures are  */
/*                         modified if memory must be reallocated     */
/*                                                                    */
/*              signed32    *no_bytes;      number of bytes read in   */
/*                                          search result file        */
/*              signed32    *attr_len;      max. length of longest    */
/*                                          attribute list            */
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

 d2_ret_val d20_106_get_dn_offsets(
   Bool         ignore,
   int          fd,
   D20_dns     *ext_dn,
   D20_dns     *int_dn,
   D20_dn_off  *dn,
   D20_dn_off  *mk,
   signed32    *no_bytes,
   signed32    *attr_len)
 
{                               /*  d20_106_get_dn_offsets()          */

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

     Bool         alias;
     signed32     f_offset;
     signed32     m_offset;

     signed32     at_l;

     unsigned16       dn_len;
     unsigned16       off;

     unsigned16       tag;
     D23_ds_name  ds_name;
     D23_at_type  at_type;
     D23_av_value av_value;
     D23_av_info  av_info;

     D2_a_type    a_type;
     byte         type[D2_OBJ_IDL_MAX];
     signed16     ret_value;
     signed16     i;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

d20_n_bytes = 0;
a_type.d2_type = (Ob_id_string) type;
if                              /*  error on reading first tag        */
   (d20_107_read(fd, (char *) &tag, sizeof(tag)))
{                               /*  return(err_code)                  */
  return((ignore == TRUE) ? D2_NOERROR : D2_ERROR);
}                               


for
    ( ; ; )
{                               /*  analyse message tag               */
  if                            /*  tag is ENT_TAG                    */
     ((tag = D2_LDSHORT(tag)) == D23_ENTTAG)
  {                             /*  skip entry information            */
    if                          /*  error occurred                    */
       (d20_107_read(fd, (char *)NULL, D23_S_EN - sizeof(tag)))
    {                           /*  return(D2_ERROR);                 */
      return(D2_ERROR);
    }
else if                         /*  error on reading next tag         */
	  (d20_107_read(fd, (char *) &tag, sizeof(tag)))
    {                           /*  return(D2_ERROR)                  */
      return(D2_ERROR);
    }
else if                         /*  tag is not NAM_TAG                */
	  ((tag = D2_LDSHORT(tag)) != D23_NAMTAG)
    {                           /*  return(D2_ERROR)                  */
      return(D2_ERROR);
    }
else if                         /*  error on reading DN-info          */
	  (d20_107_read(fd, (char *) &ds_name + sizeof(tag),
			D23_S_NM - sizeof(tag)))
    {                           /*  return(D2_ERROR)                  */
      return(D2_ERROR);
    }
else
{                           
      f_offset = (*no_bytes += D23_S_EN + D23_S_NM);
      m_offset = ext_dn->anz;
      if                        /*  DN can't be stored                */
	 (d20_109_check_memory(off = D2_LDSHORT(ds_name.d23_dsoff),
			       ext_dn->anz,&ext_dn->names, &ext_dn->max_anz))
      {                         /*  return(D2_ERROR)                  */
	return(D2_ERROR);
      }
else if                         /*  error on reading DN               */
	   (d20_107_read(fd, ext_dn->names + ext_dn->anz, off))
      {                         /*  return(D2_ERROR);                 */
	return(D2_ERROR);
      }
else
{                               /*  norm DN                           */
	d20_norm(ext_dn->names + ext_dn->anz,
		 dn_len = D2_LDSHORT(ds_name.d23_dslen));
	*no_bytes += off;
	ext_dn->anz += dn_len;
	alias = FALSE;
				    /* set flag that DN (that has just*/
				    /* been found) is not to be       */
				    /* installed on the shadow DSA    */
	d20_dn_state = D20_DONT_INSTALL;

	at_l = *no_bytes;
	if                      /*  error while reading attribute tag */
	   (d20_107_read(fd, (char *) &tag, sizeof(tag)))
	{                       /*  return(D2_ERROR)                  */
	  return(D2_ERROR);
	}                       

	for                     /*  EVER                              */
	    ( ; ; )
	{                       
	  if                    /*  tag is not ATT_TAG                */
	     ((tag = D2_LDSHORT(tag)) != D23_ATTTAG)
	  {                     /*  break                             */
	    break;
	  }
else if                         /*  error on reading attribute info   */
		(d20_107_read(fd, (char *) &at_type + sizeof(tag),
			      sizeof(at_type) - sizeof(tag)))
	  {                     /*  return(D2_ERROR)                  */
	    return(D2_ERROR);
	  }
else
{                     
	     off = D2_LDSHORT(at_type.d23_atoff);
	    *no_bytes += D23_S_ATT ;
	    a_type.d2_typ_len = at_type.d23_atlen;

	    if
	      (d20_107_read(fd,(char *) a_type.d2_type, at_type.d23_atlen))
	    {                   /*  return(D2_ERROR)                  */
	      return(D2_ERROR);
	    }                   

	    if
    (d20_107_read(fd,(char *) NULL, at_type.d23_atoff-at_type.d23_atlen))
	    {                   /*  return(D2_ERROR)                  */
	      return(D2_ERROR);
	    }
else
{
	      *no_bytes += off;
	    }                   

	    if                  /*  error while reading av_info_tag */
	      (d20_107_read(fd, (char *) &tag, sizeof(tag)))
	    {   
	      return(D2_ERROR);
	    }                       

	    if                  /*  tag is not AV_TAG                 */
	     ((tag = D2_LDSHORT(tag)) != D23_AVTAG)
	    {                   /*  att-type-only case                */
	      break;
	    }
else if                         /*  error on reading av_info          */
		(d20_107_read(fd, (char *) &av_info + sizeof(tag),
				       D23_S_AV - sizeof(tag)))
	    {                   /*  return(D2_ERROR)                  */
		return(D2_ERROR);
	    }
else
{                     
		*no_bytes += D23_S_AV;


	     if                 /*  error while reading tag           */
	       (d20_107_read(fd, (char *) &tag, sizeof(tag)))
	     {   
	       return(D2_ERROR);
	     }                       
	     i=0;
	     while ( (tag < D23_MINTAG) &&
		 (i < (signed16)(av_info.d23_avnum)) )
	      { 
		 av_value.d23_avlen = tag;

		 if
		   (d20_107_read(fd, (char *) &av_value + sizeof(tag),
					      D23_S_ATV - sizeof(tag)))
		 {    return(D2_ERROR);
		 }
else
{ off = av_value.d23_avoff;
		      *no_bytes += D23_S_ATV;
		 } 

		 if              /*  attribute is MASTER-KNOWLEDGE     */
		  (D2_ATTCMP(&a_type, D2_MASTER_KNOWLEDGE)==0)
		 {               
		  if             /*  Master-DSA can't be stored        */
		   (d20_109_check_memory(off, int_dn->anz, &int_dn->names,
					&int_dn->max_anz))
		  {              /*  return(D2_ERROR)                  */
		    return(D2_ERROR);
		  }
else if                          /*  error on reading Master-DSA       */
			(d20_107_read(fd, int_dn->names + int_dn->anz, off))
		  {              /*  return(D2_ERROR);                 */
		    return(D2_ERROR);
		  }
else
{                                /*  check whether DN has to be installed */
				 /*  on the shadow DSA                    */
		    if (strcmp(int_dn->names + int_dn->anz, d20_m_dsa) == 0)
		    {   
		      d20_dn_state = D20_INSTALL;
		    } 
		    if           /*  check of DSA failed               */
		    (d20_110_check_dn(int_dn->names, *no_bytes, &int_dn->anz,
					 mk, D2_LDSHORT(av_value.d23_avlen)))
		    {           /*  return(D2_ERROR)                   */
		      return(D2_ERROR);
		    }           
		  }             
		}
else if                         /*  error on reading attribute value  */
		      (d20_107_read(fd, (char *)NULL, off))
		{               /*  return(D2_ERROR);                 */
		  return(D2_ERROR);
		}
else
{
		 if             /*  attribute is ALIASED_OBJECT       */
		    (D2_ATTCMP(&a_type, D2_ALIASED_OBJECT)==0)
		 {                   
		    alias = TRUE;
		 } 
		}               
	       i++;
	      *no_bytes += off;
	      if                /*  error while reading tag           */
		(d20_107_read(fd, (char *) &tag, sizeof(tag)))
	      {   
		 return(D2_ERROR);
	      }                 
	   }                    


	  }                     
	 }                      
	}                       
	at_l = *no_bytes - at_l;
				/*  determine max. length of attr. lists */
	if (at_l > *attr_len)
	{   
	  *attr_len = at_l;
	} 

	if                      /*  offset of DN not built            */
	   (d20_108_build_offset(dn, f_offset, m_offset))
	{                       /*  return(D2_ERROR)                  */
	  return(D2_ERROR);
	}
else
{                               /*  store length of attribute list    */
	  (dn->off + dn->no - 1)->d20_attr_len = at_l;
	  if
	   (alias == TRUE)
	  {                     /*  set ALIAS-Flag                    */
	   (dn->off + dn->no - 1)->d20_state |= D21_ALIAS_OBJ;
	  } 
	}                       
      }                         
    }                           
  }
  else if                       /*  skipping of tag returns > 0       */
	((ret_value = d20_skip(fd, &tag, no_bytes)) > 0)
  {                             /*  return(D2_NOERROR)                */
    return(D2_NOERROR);
  }
  else if                       /*  ret_value < 0                     */
	(ret_value < 0)
  {                             /*  return(D2_ERROR)                  */
    return(D2_ERROR);
  }                             
}                               

}                               /*  d20_106_get_dn_offsets()          */


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_108_build_offset()                                */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        26.04.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_108_build_offset(dn_str, f_offset,       */
/*						m_offset)	      */
/*              D20_dn_off  *dn_str;                                  */
/*              signed32     f_offset;                                */
/*              signed32     m_offset;                                */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will reallocate memory for offset blocks*/
/*              (if necessary).                                       */
/*              Afterwars the file offset and memory offset of the    */
/*              DN will be stored.                                    */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D20_dn_off  *dn_str;    DN-structure containing OFFSET*/
/*              signed32     f_offset;  file offset                   */
/*              signed32     m_offset;  memory offset                 */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              D20_dn_off  *dn_str;    DN-structure containing OFFSET*/
/*                                      (actualized)                  */
/*                                                                    */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       reallocation (if necessary) failed            */
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

static  signed16 d20_108_build_offset(
   D20_dn_off  *dn_str,
   signed32     f_offset,
   signed32     m_offset)
 
{                               /*  d20_108_build_offset()            */

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

 D20_offset *ptr_dn;
 signed32    i;
 signed32    max_idx;
 signed32    base_offset;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if                              /*  number of entries exceeded        */
   (dn_str->no == dn_str->max_no)
{                               /*  reallocate offset blocks          */
#ifdef TRACE
  fprintf(stderr, "108: realloc\n");
  fflush(stderr);
#endif
  ptr_dn = dn_str->off;
  if                            /*  reallocation fails                */
     ((dn_str->off = (D20_offset *) realloc(dn_str->off,
		       (dn_str->max_no + D20_NO_OFF)*sizeof(D20_offset)))
		   == NULL)
  {                             /*  return(D2_ERROR)                  */
    d20_head->d2_errvalue = D20_ALLOC_ERR;
    return(D2_ERROR);
  }
  else if                       /*  reallocation fails                */
       ((dn_str->poff = (D20_offset **) realloc(dn_str->poff,
		      (dn_str->max_no + D20_NO_OFF)*sizeof(String)))
		      == NULL)
  {                             /*  return(D2_ERROR)                  */
    d20_head->d2_errvalue = D20_ALLOC_ERR;
    return(D2_ERROR);
  }
  else
  {                             /*  restore pointer                   */
    base_offset = (signed32) ((char *) dn_str->off - (char *) ptr_dn);
    for (i = 0, max_idx = dn_str->no; i < max_idx; i++)
    { 
      *(dn_str->poff + i) = (D20_offset *)
			    ((char *) *(dn_str->poff + i) + base_offset);
    } 
  }                             
  dn_str->max_no += D20_NO_OFF;
}                               

			      /*  fill offset blocks                */
(ptr_dn = (dn_str->off + dn_str->no))->d20_file_off = f_offset;
ptr_dn->d20_mem_off  = m_offset;
ptr_dn->d20_state = d20_dn_state;
*(dn_str->poff + dn_str->no) = ptr_dn;
(dn_str->no)++;

#ifdef TRACE
fprintf(stderr, "108: name inserted\n");
fflush(stderr);
#endif
				/*  return(D2_NOERROR)                */
return(D2_NOERROR);

}                               /*  d20_108_build_offset()            */


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_109_check_memory()                                */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        26.04.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_109_check_memory(new_anz, act_anz,       */
/*					dn_names, max_anz)            */
/*              unsigned16       new_anz;                             */
/*              signed32         act_anz;                             */
/*              char            **dn_names;                           */
/*              signed32         *max_anz;                            */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will reallocate memory for DNs (if      */
/*              necessary).                                           */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              unsigned16   new_anz;   length of new DN              */
/*              signed32     act_anz;   actual lenghts of all DNs     */
/*              char       **dn_names;  memory area for DNs           */
/*              signed32    *max_anz;   maximum length of all DNs     */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              char       **dn_names;  new memory area for DNs       */
/*                                      (if memory was reallocated)   */
/*              signed32    *max_anz;   new maximum length of all DNs */
/*                                      (if memory was reallocated)   */
/*                                                                    */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       reallocation (if necessary) failed            */
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

static  signed16 d20_109_check_memory(
   unsigned16   new_anz,
   signed32     act_anz,
   char       **dn_names,
   signed32    *max_anz)
 
{                               /*  d20_109_check_memory()            */

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


/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if                              /*  DN is too long                    */
   ((signed32)new_anz + act_anz > *max_anz)
{                               /*  reallocate DN-memory              */
  if                            /*  reallocation fails                */
     ((*dn_names = realloc(*dn_names, *max_anz + D20_MORE)) == NULL)
  {                             /*  return(D2_ERROR)                  */
    d20_head->d2_errvalue = D20_ALLOC_ERR;
    return(D2_ERROR);
  }
  else
  {                             /*  store new memory length           */
    *max_anz += D20_MORE;
  }                             
}                               

				/*  return(D2_NOERROR)                */
return(D2_NOERROR);

}                               /*  d20_109_check_memory()            */


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_110_check_dn()                                    */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        26.04.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_110_check_dn(dn_names, f_offset,m_offset,*/
/*                                     dn_str, dn_length)             */
/*              char        *dn_names;                                */
/*              signed32     f_offset;                                */
/*              signed32    *m_offset;                                */
/*              D20_dn_off  *dn_str;                                  */
/*              unsigned16   dn_length;                               */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will check whether the given DN was     */
/*              already entered in the DN list.                       */
/*              If not, the DN-Name will be entered. If necessary,    */
/*              memory for the DN-offsets will be reallocated.        */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              char        *dn_names;      memory area of DNs        */
/*              signed32     f_offset;      offset of DN in file      */
/*              signed32    *m_offset;      offset of DN in memory    */
/*              D20_dn_off  *dn_str;        DN-structure containing   */
/*                                          OFFSET information        */
/*              unsigned16   dn_length;     length of new DN          */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              signed32    *m_offset;      offset of next DN in      */
/*                                          memory                    */
/*              D20_dn_off  *dn_str;        DN-structure containing   */
/*                                          OFFSET information        */
/*                                          (actualized)              */
/*                                                                    */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       reallocation (if necessary) failed            */
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

static  signed16 d20_110_check_dn(
   char       *dn_names,
   signed32    f_offset,
   signed32   *m_offset,
   D20_dn_off *dn_str,
   unsigned16  dn_length)
 
{                               /*  d20_110_check_dn()                */

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

 signed32     i;
 char   *act_dn;
 signed16   ret_value;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

act_dn = dn_names + *m_offset;
				/*  norm name                         */
d20_norm(act_dn, dn_length);

#ifdef TRACE
fprintf(stderr,"110: name: %s--- length: %d\n", act_dn, dn_length);
fflush(stderr);
#endif

for                             /*  all DSAs in list                  */
    (i = 0; i < dn_str->no; i++)
{                               
  if                            /*  DSA found                         */
     (!strcmp(act_dn, dn_names + (dn_str->off + i)->d20_mem_off))
  {                             /*  return(D2_NOERROR)                */
#ifdef TRACE
    fprintf(stderr, "110: name already exists\n");
    fflush(stderr);
#endif
    return(D2_NOERROR);
  }                             
}                               

#ifdef TRACE
fprintf(stderr, "110: name not found\n");
fflush(stderr);
#endif

				/*  build offset of DSA               */
ret_value = d20_108_build_offset(dn_str, f_offset, *m_offset);
				/*  actualize length of internal DN-list*/
*m_offset += dn_length;

				/*  return(ret_value of operation)    */
return(ret_value);

}                               /*  d20_110_check_dn()                */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_111_free()                                        */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        26.04.88                                              */
/*								      */
/* SYNTAX:      void d20_111_free(ptr1, ptr2, ptr3, ptr4, ptr5, ptr6, */
/*                                ptr7, ptr8, ptr9, ptr10)            */
/*              char    *ptr1;                                        */
/*              char    *ptr2;                                        */
/*              char    *ptr3;                                        */
/*              char    *ptr4;                                        */
/*              char    *ptr5;                                        */
/*              char    *ptr6;                                        */
/*              char    *ptr7;                                        */
/*              char    *ptr8;                                        */
/*              char    *ptr9;                                        */
/*              char    *ptr10;                                       */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will release the allocated memory areas.*/
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:							      */
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

 void d20_111_free(
   char *ptr1,
   char *ptr2,
   char *ptr3,
   char *ptr4,
   char *ptr5,
   char *ptr6,
   char *ptr7,
   char *ptr8,
   char *ptr9,
   char *ptr10)

{                               /*  d20_111_free()                    */

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

 char  *adr[NR_ADDR];
 signed16  i;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

adr[0] = ptr1;
adr[1] = ptr2;
adr[2] = ptr3;
adr[3] = ptr4;
adr[4] = ptr5;
adr[5] = ptr6;
adr[6] = ptr7;
adr[7] = ptr8;
adr[8] = ptr9;
adr[9] = ptr10;

for (i = 0; i < NR_ADDR ; i++)
{ 
  if (adr[i])
  {   
    free(adr[i]);
  } 
} 

}                               /*  d20_111_free()                    */


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_skip()                                            */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        09.05.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_skip(fd, tag, no_bytes)                  */
/*              int             fd;                                   */
/*              signed16       *tag;                                  */
/*              signed32       *no_bytes;                             */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function skips the some of the message tags      */
/*              belonging to the search result message and not        */
/*              relevant for dump operation.                          */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              int          fd;            file descriptor for       */
/*                                          search result file        */
/*              signed16    *tag;           message tag               */
/*              signed32    *no_bytes;      number of bytes read in   */
/*                                          search result file        */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              signed16    *tag;           new message tag           */
/*              signed32    *no_bytes;      number of bytes read in   */
/*                                          search result file        */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*     -1:              error occurred                                */
/*      0:              No error occurred                             */
/*      1:              end of message reached                        */
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

static  signed16 d20_skip(
   int         fd,
   unsigned16 *tag,
   signed32   *no_bytes)
 
{	/*  d20_skip()                        */

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

     signed16	      ret_value = D2_ERROR;	
     unsigned16       off;

     D23_ds_name  ds_name;
     D23_av_value av_value;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if (*tag == D23_CHRTAG)
	{
   	if (d20_107_read(fd, (char *) NULL, D23_S_CHRES - sizeof(*tag)))
   		return(ret_value);

   	*no_bytes += D23_S_CHRES;
   	if (d20_107_read(fd, (char *) tag, sizeof(*tag)))
		return(ret_value);

   	if (*tag == D23_DMTAG)
     		{
     		if (d20_107_read(fd, (char *) NULL, D23_S_DM - sizeof(*tag)))
     			return(ret_value);

     		*no_bytes = D23_S_DM;
     		if (d20_107_read(fd, (char *) tag, sizeof(*tag)))
			return(ret_value);
     		}
	
    	if (*tag == D23_CREFTAG)
		{
     		if (d20_107_read(fd,(char *)NULL, D23_S_CREF - sizeof(*tag)))
     			return(ret_value);

     		*no_bytes += D23_S_CREF;
	    	if (d20_107_read(fd, (char *) tag, sizeof(*tag)))
	  		return(ret_value);

	    	if (*tag  != D23_NAMTAG)
	  		/*  return(D2_ERROR)              */
	    		return(ret_value);

	  	/*  error on reading DN-info      */
	    	if (d20_107_read(fd, (char *) &ds_name + sizeof(*tag),
			      D23_S_NM - sizeof(*tag)))
	  		/*  return(ret_value)              */
	     		return(ret_value);

	    	*no_bytes += D23_S_NM;
	      	if (d20_107_read(fd, (char *) NULL, ds_name.d23_dsoff))
	     		return(ret_value);

	     	*no_bytes += ds_name.d23_dsoff;

	    	if (d20_107_read(fd, (char *) tag, sizeof(*tag)))
	  		return(ret_value);

	 	/* read sequence of access info */
	    	while (*tag == D23_ACCTAG)
			{
	  		/*  error on reading Access-Info  */
	    		if (d20_107_read(fd, (char *) NULL,
			      		D23_S_ACC - sizeof(*tag)))
	  			/*  return(D2_ERROR)              */
	     			return(ret_value);

	    		*no_bytes += D23_S_ACC;

	    		if (d20_107_read(fd, (char *) tag, sizeof(*tag)))
	  			return(ret_value);

	    		if (*tag != D23_NAMTAG)
	  			/*  return(D2_ERROR)              */
	    			return(ret_value);

	  		/*  error on reading DN-info      */
	    		if (d20_107_read(fd, (char *) &ds_name + sizeof(*tag),
			      			D23_S_NM - sizeof(*tag)))
	  			/*  return(D2_ERROR)              */
	     			return(ret_value);

	    		*no_bytes += D23_S_NM;
	      		if (d20_107_read(fd,(char *)NULL, ds_name.d23_dsoff))
	     			return(ret_value);

	     		*no_bytes += ds_name.d23_dsoff;

	    		if (d20_107_read(fd, (char *) &av_value, D23_S_ATV))
	  			return(ret_value);

	  		*no_bytes += D23_S_ATV;
	      		if (d20_107_read(fd,(char *)NULL,av_value.d23_avoff))
	     			return(ret_value);

	     		*no_bytes += av_value.d23_avoff;
	    		if (d20_107_read(fd, (char *) tag, sizeof(*tag)))
	  			return(ret_value);

			} /* end of while loop.	*/

		}

	return(D2_NOERROR);
	}

/*  tag is NAM-TAG || D23_BONTAG      */
 if (*tag == D23_NAMTAG || *tag == D23_BONTAG)
	{
	/*  skip NAM_TAG                      */
  	/*  no error occurred                 */
     	if (!d20_107_read(fd, (char *) &ds_name + sizeof(*tag),
		       D23_S_NM - sizeof(*tag)))
  		{
		/*  read DN                           */
    		/*  no error occurred                 */
       		if (!d20_107_read(fd,(char *)NULL,ds_name.d23_dsoff))
    			/*  read next tag                     */
      			/*  no error occurred                 */
 			{
			if (!d20_107_read(fd,(char *)tag,sizeof(*tag)))
      				{
				/*  actualize number of bytes read    */
				*no_bytes += D23_S_NM + ds_name.d23_dsoff;
				ret_value = D2_NOERROR;
				}
			}
		}
	return(ret_value);
	}

/*  tag is EOM_TAG                    */
if (*tag == D23_EOMTAG)
	{
	/*  skip "EOM" info                   */
  	/*  no error on reading "EOM" info    */
     	if (!d20_107_read(fd, (char *)NULL, sizeof(D23_eom) - sizeof(*tag)))
  		{
		/*  actualize number of bytes read    */
    		*no_bytes += sizeof(D23_eom);

    		/*  error on reading next tag         */
       		if (d20_107_read(fd,(char *)tag, sizeof(*tag)))
      			ret_value = 1;
		else
      			ret_value = D2_NOERROR;
		}

	return(ret_value);
	}

/*  tag is OQ_TAG                     */
if (*tag == D23_OQTAG)
	{
	/*  skip  Part_oq Block               */
  	/*  no error on reading "EOM" info    */
     	if (!d20_107_read(fd, (char *)NULL, D23_S_POQ - sizeof(*tag)))
  		{
		/*  actualize number of bytes read    */
    		*no_bytes += D23_S_POQ;

    		/*  error on reading next tag         */
       		if (d20_107_read(fd, (char *) tag, sizeof(*tag)))
    			/*  return(ret_value);  */
      			return(ret_value);

    		/*  tag is referral tag               */
      		if (*tag == D23_REFTAG)
			{
    			/*  error on reading "REFERRAL" tag   */
      			if (d20_107_read(fd, (char *)NULL, 
					sizeof(D23_rf_info) - sizeof(*tag)))
    				/*  return(ret_value);                */
    				return(ret_value);

    			/*  error on reading next tag         */
      			if (d20_107_read(fd, (char *) tag, sizeof(*tag)))
    				/*  return(ret_value);                */
      				return(ret_value);

    			/*  tag is not NAM_TAG                */
       			if ((*tag = D2_LDSHORT(*tag)) != D23_NAMTAG)
    				/*  return(ret_value);                */
      				return(ret_value);

    			/*  error on reading DN-info          */
			if (d20_107_read(fd, (char *) &ds_name + sizeof(*tag),
		     				sizeof(ds_name) - sizeof(*tag)))
    				/*  return(ret_value);                */
      				return(ret_value);


    			/*  error on reading DN               */
       			if (d20_107_read(fd, (char *)NULL, 
					off = D2_LDSHORT(ds_name.d23_dsoff)))
    				/*  return(ret_value);                */
      				return(ret_value);

			/*  actualize of number of bytes read */
    			*no_bytes += sizeof(D23_rf_info) + sizeof(D23_ds_name) 
							 + off;

    			/*  error on reading next tag         */
      			if (d20_107_read(fd, (char *) tag, sizeof(*tag)))
    				/*  return(ret_value);                */
    				return(ret_value);

	 		/*  read sequence of access info */
	    		while (*tag == D23_ACCTAG)
				{
	  			/*  error on reading Access-Info  */
	    			if (d20_107_read(fd, (char *) NULL,
			      			D23_S_ACC - sizeof(*tag)))
	  				/*  return(D2_ERROR)              */
	     				return(ret_value);

	    			*no_bytes += D23_S_ACC;

	    			if (d20_107_read(fd,(char *)tag, sizeof(*tag)))
	  				return(ret_value);

	    			if (*tag != D23_NAMTAG)
	  				/*  return(D2_ERROR)              */
	    				return(ret_value);

	  			/*  error on reading DN-info      */
	    			if (d20_107_read(fd, (char *) &ds_name + 
					sizeof(*tag), D23_S_NM - sizeof(*tag)))
	  				/*  return(D2_ERROR)              */
	     				return(ret_value);

	    			*no_bytes += D23_S_NM;
	      			if (d20_107_read(fd, (char *) NULL, 
							ds_name.d23_dsoff))
	     				return(ret_value);

	     			*no_bytes += ds_name.d23_dsoff;
	     			if (d20_107_read(fd, (char *) &av_value, 
								D23_S_ATV))
	  				return(ret_value);

	   			*no_bytes += D23_S_ATV;
	       			if (d20_107_read(fd, (char *) NULL, 
							av_value.d23_avoff))
	      				return(ret_value);

	      			*no_bytes += av_value.d23_avoff;
	    			if (d20_107_read(fd,(char *)tag, sizeof(*tag)))
	  				return(ret_value);
				} /* end of while loop.	*/
			} /* end of REFTAG handling.	*/
		}

	return(D2_NOERROR);
	}

/*  tag is referral tag               */
if (*tag == D23_REFTAG)
	{
    	/*  error on reading "REFERRAL" tag   */
      	if (d20_107_read(fd,(char *)NULL,sizeof(D23_rf_info) - sizeof(*tag)))
    		/*  return(ret_value);                */
    		return(ret_value);

    	/*  error on reading next tag         */
      	if (d20_107_read(fd, (char *) tag, sizeof(*tag)))
    		/*  return(ret_value);                */
      		return(ret_value);

    	/*  tag is not NAM_TAG                */
       	if ((*tag = D2_LDSHORT(*tag)) != D23_NAMTAG)
    		/*  return(ret_value);                */
      		return(ret_value);

    	/*  error on reading DN-info          */
	if (d20_107_read(fd, (char *) &ds_name + sizeof(*tag),
	     				sizeof(ds_name) - sizeof(*tag)))
    		/*  return(ret_value);                */
      		return(ret_value);

    	/*  error on reading DN               */
       	if (d20_107_read(fd,(char *)NULL,off =D2_LDSHORT(ds_name.d23_dsoff)))
    		/*  return(ret_value);                */
      		return(ret_value);

	/*  actualize of number of bytes read */
    	*no_bytes += sizeof(D23_rf_info) + sizeof(D23_ds_name) + off;

    	/*  error on reading next tag         */
      	if (d20_107_read(fd, (char *) tag, sizeof(*tag)))
    		/*  return(ret_value);                */
    		return(ret_value);

	 /*  read sequence of access info */
	  while (*tag == D23_ACCTAG)
		{
	  	/*  error on reading Access-Info  */
	    	if (d20_107_read(fd, (char *) NULL,D23_S_ACC - sizeof(*tag)))
	  		/*  return(D2_ERROR)              */
	     		return(ret_value);

	    	*no_bytes += D23_S_ACC;

	    	if (d20_107_read(fd,(char *)tag, sizeof(*tag)))
	  		return(ret_value);

	    	if (*tag != D23_NAMTAG)
	  		/*  return(D2_ERROR)              */
	    		return(ret_value);

	  	/*  error on reading DN-info      */
	    	if (d20_107_read(fd, (char *) &ds_name + 
					sizeof(*tag), D23_S_NM - sizeof(*tag)))
	  		/*  return(D2_ERROR)              */
	     		return(ret_value);

	    	*no_bytes += D23_S_NM;
	      	if (d20_107_read(fd, (char *) NULL, ds_name.d23_dsoff))
	     		return(ret_value);

	     	*no_bytes += ds_name.d23_dsoff;
	     	if (d20_107_read(fd, (char *) &av_value, D23_S_ATV))
	  		return(ret_value);

	   	*no_bytes += D23_S_ATV;
	       	if (d20_107_read(fd, (char *) NULL, av_value.d23_avoff))
	      		return(ret_value);

	      	*no_bytes += av_value.d23_avoff;
	    	if (d20_107_read(fd,(char *)tag, sizeof(*tag)))
	  		return(ret_value);
		} /* end of while loop.	*/

	return(D2_NOERROR);
	} /* end of REFTAG handling.	*/

/*  tag is CRS_TAG                     */
if (*tag == D23_CRSTAG)
	{
	/*  skip  Common Result Block         */
  	/*  no error on reading               */
     	if (!d20_107_read(fd, (char *)NULL, D23_S_CRES - sizeof(*tag)))
  		{
		/*  actualize number of bytes read    */
    		*no_bytes += D23_S_CRES;

    		/*  error on reading next tag         */
       		if (d20_107_read(fd, (char *) tag, sizeof(*tag)))
      			return(ret_value);

    		/*  tag is NAM_TAG                */
       		if ((*tag = D2_LDSHORT(*tag)) == D23_NAMTAG)
    			{
			/*  skip name-block                   */
      			/*  error on reading DN-info          */
	  		if (d20_107_read(fd, (char *) &ds_name + sizeof(*tag),
		       				D23_S_NM - sizeof(*tag)))
				return(ret_value);

      			/*  error on reading DN               */
	 		if (!d20_107_read(fd, (char *)NULL, 
					off = D2_LDSHORT(ds_name.d23_dsoff)))
				return(ret_value);

		  	/*  actualize of number of bytes read */
      			*no_bytes += D23_S_NM + off;

			/*  error on reading next tag         */
			if (d20_107_read(fd, (char *) tag, sizeof(*tag)))
      				return(ret_value);
			
			}

       		if ((*tag = D2_LDSHORT(*tag)) == D23_SECTAG)
    			{
			/*  return(ret_value);                */
			if (d20_107_read(fd, (char *) NULL ,
		     			D23_S_SEP - sizeof(*tag)))
       				return(ret_value);

      			*no_bytes += D23_S_SEP;

      			/*  error on reading next tag         */
       			if (d20_107_read(fd, (char *) tag, sizeof(*tag)))
       				return(ret_value);

			}
		}

	return(D2_NOERROR);
	}

/*  errormessage from search          */
if (*tag == D2_CALL_ERR)             
	{
	/*  in case the new subtree was not   */
	/*  found */
    	if (d20_107_read(fd, (char *) NULL, D23_S_RSERR-sizeof(*tag)))
    		return(ret_value);

   	/*  error on reading next tag         */
    	if (d20_107_read(fd, (char *) tag, sizeof(*tag)))
   		/*  return(ret_value);                */
    		return(ret_value);

   	*no_bytes += D23_S_RSERR;
	return(D2_NOERROR);
	}

				/*  return(ret_value)                 */
return(ret_value);

}	/*  d20_skip()                        */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_norm()                                            */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        12.12.88                                              */
/*								      */
/* SYNTAX:      void  d20_norm(dn, len)                               */
/*              char        *dn;                                      */
/*              unsigned16   len;                                     */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will norm the given DN.                 */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              char        *dn;            DN                        */
/*              unsigned16   len;           length of DN              */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*     -1:              error occurred                                */
/*      0:              No error occurred                             */
/*      1:              end of message reached                        */
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

 void d20_norm(
   char       *dn,
   unsigned16  len)
 
{                               /*  d20_norm()                        */

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

register char   *max_dn;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

for (max_dn = dn + len; dn < max_dn; dn++)
{ 
	if (*dn == D2_PRTBL_REP) 
		*dn = D2_T61_REP;
	else
  		*dn = (D2_RG3 <= *dn && *dn <= D2_RG4) ?  
						*dn - (D2_RG3 - D2_RG1) : *dn;
} 

}                               /*  d20_norm()                        */
