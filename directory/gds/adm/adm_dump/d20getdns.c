/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20getdns.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:30  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:10:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:34  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:02:46  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:01:33  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:01  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:12:39  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:18:06  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:03:41  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:44:15  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:57:56  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:44:32  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:36:31  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20getdns.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:34 $";
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
/* NAME         : d20getdns.o  [get more DNs from DSA]                */
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
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d2dump.h>
#include <Trace.h>
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

extern char      *d20_beg1;             /* start address of DN-strings*/
extern char      *d20_beg2;             /* start address of DN-strings*/
extern char      *d20_object;           /* pointer to memory area for */
					/* DNs to be looked for       */
extern D2_pbhead *d20_head;             /* pointer to error codes of  */
					/* DS_SEARCH                  */

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_112_get_more_dns()                                */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        20.04.88                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_112_get_more_dns(pdn_off, dn_no,       */
/*					new_off, new_no, dn_exist)    */
/*              D20_offset **pdn_off;                                 */
/*              signed16     dn_no;                                   */
/*              D20_offset **new_off;                                 */
/*              signed16     new_no;                                  */
/*              Bool        *dn_exist;                                */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will look whether there are DNs (in the */
/*              unsorted list pointed to by "new_off") that are       */
/*              already contained in the sorted list pointed to by    */
/*              "pdn_off".                                            */
/*              If new DNs are detected, the new objects will be      */
/*              looked for.                                           */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D20_offset **pdn_off;   sorted list of DNs            */
/*              signed16     dn_no;     number of DNs in sorted list  */
/*              D20_offset **new_off;   new DNs to be looked for      */
/*              signed16     new_no;    number of new DNs             */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              Bool        *dn_exist;  a flag indicating whether     */
/*                                      new DNs were detected         */
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

 d2_ret_val d20_112_get_more_dns(
   D20_offset **pdn_off,
   signed16     dn_no,
   D20_offset **new_off,
   signed16     new_no,
   Bool        *dn_exist)

{                               /*  d20_112_get_more_dns()            */

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

 char         old_dn[D2_DNL_MAX];

 D20_offset **dn = pdn_off;
 D20_offset **dn_max = pdn_off + dn_no;
 D20_offset **new_max = new_off + new_no;
 D20_offset **act_new = new_off;

 int          ret_value;
 signed16     rdn_pos[D2_NP_MAX];
 signed16     rdn_anz;      /* number of RDNs                     */
 signed16     no;           /* number of RDNs found in sorted list*/
 char        *str;
 char         ch;
 Bool         name_found;
 Bool         obj_found;
 char        *last_handled = NULL;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

*dn_exist = FALSE;
strcpy(old_dn, d20_object);

while                           /*  not end of new DN list reached    */
      (act_new < new_max)
{                               

  trace((stderr,"112: new name: %s--- state: %ld",
	 d20_beg2 + (*act_new)->d20_mem_off, (*act_new)->d20_state));

  if                            /*  new DN not yet handled            */
     (((*act_new)->d20_state & D20_DONE) != D20_DONE)
  {                             
    str = d20_beg2 + (*act_new)->d20_mem_off;
    if                          /*  new DN is equal last handled      */
       (last_handled != NULL)
    {   
      if (!strcmp(last_handled, str))
      {                         /*  mark DN as handled                */
	(*act_new)->d20_state |= D20_DONE;
				/*  take next "new" DN                */
	act_new++;
	trace((stderr, "112: name found(1), state = D20_DONE"));
	continue;
      }                         
    } 
				/*  get RDNs of new DN                */
    d20_114_get_rdns(str, &rdn_anz, rdn_pos);
    trace((stderr, "112: name to be looked for: %s---", str));
    for                         /*  all RDNs of new DN                */
	(no = 0, dn = pdn_off; no < rdn_anz; no++)
    {                           /*  determine first part of new DN    */
      ch = *(str + rdn_pos[no]);
      *(str + rdn_pos[no]) = D2_EOS;
      for                       /*  who|e sorted list                 */
	  (name_found = FALSE ; dn < dn_max; )
      {                         
	trace((stderr, "112: name to be compared with: %s---",
	      d20_beg1 + (*dn)->d20_mem_off));
	if                      /*  first part of new DN < actual DN  */
	   ((ret_value = strcmp(str, d20_beg1 + (*dn)->d20_mem_off)) < 0)
	{                       /*  break                             */
	  break;
	}
        else if                 /*  new DN = actual DN in sorted list */
	      (!ret_value)
	{                       /*  break                             */
	  name_found = TRUE;
	  dn++;
	  trace((stderr, "112: name found: %s---", str));
	  break;
	}                       
				/*  take next DN in sorted list       */
	dn++;
      }                         
      *(str + rdn_pos[no]) = ch;
      if                        /*  object not found                  */
	 (name_found == FALSE)
      {                         /*  look for superiors in own list    */
	trace((stderr, "112: name not found: %s--- look in own list", str));
	d20_115_own_list(new_off, act_new - 1, &no, rdn_anz, rdn_pos, str);

	if                      /*  not all RDNs found in own list    */
	   (no < rdn_anz)
	{                       /*  check whether RDNs have been searched */
				/*  the time before                       */
	  if (last_handled != NULL)
	  {                     
	    for                 /*  all missing RDNs                  */
		( ; no < rdn_anz; no++)
	    {                   /*  check whether actual DN and last DN */
				/*  have the same ROOT                  */
	      ch = *(str + rdn_pos[no]);
	      *(str + rdn_pos[no]) = D2_EOS;
	      if (strncmp(last_handled, str, strlen(str)) != 0 ||
		  (*(last_handled + rdn_pos[no]) != D2_RDN_SEP &&
		   *(last_handled + rdn_pos[no]) != D2_EOS))
	      {                 /*  ROOT of both strings is not equal */
		*(str + rdn_pos[no]) = ch;
		break;
	      }
              else
              {                 /*  ROOT is equal up to RDN number "no" */
		*(str + rdn_pos[no]) = ch;
	      }                 
	    }                   
	  }                     
	}                       

				/*  look for new DN                   */
	if                      /*  operation fails                   */
	   (d20_113_search_dn(no, rdn_anz, rdn_pos, str, &obj_found))
	{                       
	  if (obj_found == TRUE)
	  {   
	    trace((stderr, "112: dn_exist set (1)"));
	    *dn_exist = TRUE;
	  } 
	  if                    /*  object was not found              */
	     (d20_head->d2_errvalue == D2_NFOUND)
	  {                     /*  ignore error                      */
				/*  set flag "new DN handled"         */
	    (*act_new)->d20_state |= D20_DONE;
	    trace((stderr, "112: state set (1)"));
				/*  break                             */
	    break;
	  }
          else
          {                     /*  return(D2_ERROR)                  */
	    d20_head->d2_errvalue = D20_SEARCH_ERR;
	    strcpy(d20_object, old_dn);
	    return(D2_ERROR);
	  }                     
	}
        else
        {                       /*  set flag "new DNs found"          */
	  *dn_exist = TRUE;
	  trace((stderr, "112: dn_exist set (2)"));
	}                       
				/*  set flag "new DN handled"         */
	(*act_new)->d20_state |= D20_DONE;
	trace((stderr, "112: state set (2)"));
				/*  break                             */
	break;
      }                         
    }                           
  }                             
				/*  take next "new" DN                */
  act_new++;
  last_handled = str;
}                               

strcpy(d20_object, old_dn);
				/*  return(D2_NOERROR)                */
return(D2_NOERROR);

}                               /*  d20_112_get_more_dns()            */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_113_search_dn()                                   */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        28.04.88                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_113_search_dn(no, max_no, rdn_pos, dn, */
/*                                      obj_found)                    */
/*              signed16    no;                                       */
/*              signed16    max_no;                                   */
/*              signed16   *rdn_pos;                                  */
/*              char    *dn;                                          */
/*              Bool    *obj_found;                                   */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will look for the new objects.          */
/*              The superior objects which already exist are          */
/*              referenced by "no".                                   */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed16    no;      number of RDNs already existing  */
/*              signed16    max_no;  total number of RDNs in DN       */
/*              signed16   *rdn_pos; positions of delimiters          */
/*              char    *dn;         pointer to DN                    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              Bool    *obj_found; a flag indicating whether objects */
/*                                  were found or not                 */
/*								      */
/* RETURNVALUE:							      */
/*              D2_NOERROR: no errors                                 */
/*              D2_ERROR:   error occurred on reading new objects     */
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

 d2_ret_val d20_113_search_dn(
   signed16  no,
   signed16  max_no,
   signed16 *rdn_pos,
   char     *dn,
   Bool     *obj_found)

{                               /*  d20_113_search_dn()               */

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

 d2_ret_val  ret_value = D2_NOERROR;
 char   ch;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

d20_162_set_name(dn);
if (*dn == D2_EOS)
{                               /* reset first character in INFO-     */
				/* structure                          */
  *(d20_object - 1) = D2_EOS;
} 
d20_set_subset(D2_BASE_OBJECT);
for                             /*  all new RDNs && no errors         */
    (*obj_found = FALSE ; no < max_no; no++)
{                               
  ch = *(d20_object + rdn_pos[no]);
  *(d20_object + rdn_pos[no]) = D2_EOS;

  trace((stderr,"113: SEARCH FOR: %s---", d20_object));
				/*  look for new object               */
  ret_value = d20_call_iapl(D20_0_SEARCH);

  *(d20_object + rdn_pos[no]) = ch;

  d20_167_set_file_mode();
  if (ret_value == D2_ERROR)
  {   
    break;
  }
  else
  {
    *obj_found = TRUE;
    trace((stderr,"113: obj_found set"));
  } 
} 

				/*  return(ret_value)                 */
return(ret_value);

}                               /*  d20_113_search_dn()               */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_114_get_rdns()                                    */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        20.04.88                                              */
/*								      */
/* SYNTAX:      void  d20_114_get_rdns(dn, no, pos)                   */
/*              char    *dn;                                          */
/*              signed16   *no;                                       */
/*              signed16   *pos;                                      */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will determine the number of RDNs and   */
/*              the positions of the delimiters of the RDNs in the    */
/*              given DN.                                             */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              char    *dn;        pointer to DN                     */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              signed16   *no;        number of RDNs                 */
/*              signed16   *pos;       positions of delimiters        */
/*								      */
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

 void d20_114_get_rdns(
   char     *dn,
   signed16 *no,
   signed16 *pos)

{                               /*  d20_114_get_rdns()                */

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

 char  *str = dn;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

				/*  determine number of RDNs and positions*/
*no = 0;
if (*dn)
{   
  for ( ; *dn; )
  { 
    if (*dn++ == D2_RDN_SEP)
    {   
	(*no)++;
	*pos++ = dn - 1 - str;
    } 
  } 
  (*no)++;
  *pos = dn - str;
} 

}                               /*  d20_114_get_rdns()                */


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_115_own_list()                                    */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        20.04.88                                              */
/*								      */
/* SYNTAX:      void  d20_115_own_list(first, max, no, max_no,        */
/*                                     rdn_pos, dn)                   */
/*              D20_offset  **first;                                  */
/*              D20_offset  **max;                                    */
/*              signed16     *no;                                     */
/*              signed16      max_no;                                 */
/*              signed16     *rdn_pos;                                */
/*              char         *dn;                                     */
/*								      */
/* DESCRIPTION:                                                       */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D20_offset  **first;    pointer to first list element */
/*              D20_offset  **max;      pointer to last element       */
/*              signed16     *no;       number of RDNs already        */
/*                                      handled                       */
/*              signed16      max_no;   total number of RDNs          */
/*              signed16     *rdn_pos;  positions of delimiters in    */
/*                                      RDN                           */
/*              char         *dn;       pointer to DN                 */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              signed16     *no;       number of RDNs already        */
/*                                      handled                       */
/*								      */
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
/*inoff ***************************************************************/

 void d20_115_own_list(
   D20_offset  **first,
   D20_offset  **max,
   signed16     *no,
   signed16      max_no,
   signed16     *rdn_pos,
   char         *dn)

{                               /*  d20_115_own_list()                */

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

 D20_offset **act = first;
 char         ch;
 int          ret_value;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

for                             /*  all RDNs of new DN                */
    ( ; *no < max_no && act <= max; )
{                               /*  determine first part of new DN    */
  ch = *(dn + rdn_pos[*no]);
  *(dn + rdn_pos[*no]) = D2_EOS;
  trace((stderr,"115: search for %s---", dn));
  for                           /*  who|e sorted list                 */
      ( ; act <= max; act++)
  {                             
    if                          /*  first part of new DN < actual DN  */
       ((ret_value = strcmp(dn, d20_beg2 + (*act)->d20_mem_off)) < 0)
    {                           /*  return                            */
      trace((stderr,"115: DN not found: %s---", dn));
      *(dn + rdn_pos[*no]) = ch;
      return;
    }
    else if                     /*  new DN = actual DN in sorted list */
	  (!ret_value)
    {                           /*  break                             */
      trace((stderr,"115: DN found: %s---", dn));
      *(dn + rdn_pos[*no]) = ch;
      (*no)++;
      break;
    }                           
				/*  take next DN in sorted list       */
  }                             
  if (ret_value)
  {   
    *(dn + rdn_pos[*no]) = ch;
  } 
}                               

}                               /*  d20_115_own_list()                */
