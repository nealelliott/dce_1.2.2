/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21addap.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:49  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:14:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:10  root]
 * 
 * Revision 1.1.8.4  1994/07/19  14:48:47  marrek
 * 	Bug fix in modify call.
 * 	[1994/07/19  11:54:57  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:16  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:09:19  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:47  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:06:31  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:44  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:23:13  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:31:19  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:20:41  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  17:47:59  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:02:10  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/18  08:25:09  marrek
 * 	Various bug fixes for mask handling found at SNI
 * 	[1992/12/17  16:24:03  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  14:18:50  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:33:03  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:46:59  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:12:22  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21addap.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:47 $";
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
/* NAME         : d21addap.o       [add saved objects]                */
/*								      */
/* AUTHOR       : Josep Pulpillo , CDS D 525                          */
/* DATE         : 28.07.89                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOK.-NR.	: DS-Design-Spez.				      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.	: <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*      0.1 | 28.07.89| Original                       | ws |         */
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
#include <string.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <dce/d27util.h>
#include <d2dump.h>
#include <d21.h>
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

extern char       *d20_beg1;    /* memory address of DNs of DSA1      */
extern D2_pbhead  *d20_head;

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

  
/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20t003_add_appnd()                                   */
/*								      */
/* AUTHOR:      Josep Pulpillo CDS D - 525                            */
/* DATE:        28.07.89                                              */
/*								      */
/* SYNTAX:      signed16 d21t003_add_appnd(fd, dn, at_mem, min_pos,   */
/*                                      max_pos, inherit, paren_obj,  */
/*                                      subtree, new_parent, bind_dsa,*/
/*					lenacl, pacl, at_val_area,    */
/*					use_flag)		      */
/*              int              fd;                                  */
/*              D20_offset      *dn;                                  */
/*              char            *at_mem;                              */
/*              signed32        *min_pos;                             */
/*              signed32        *max_pos;                             */
/*              signed32        inherit;                              */
/*              Name_string     parent_obj;                           */
/*              Name_string     subtree;                              */
/*              signed32        new_parent;                           */
/*		Name_string     bind_dsa;			      */
/*              signed16        lenacl;                               */
/*              char           *pacl;                                 */
/*		char	       *at_val_area;			      */
/*		signed16	use_flag;			      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will add a new entry to the DSA.        */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              int          fd;         file descriptor              */
/*              D20_offset  *dn;         offset of DN in file         */
/*              char        *at_mem;     memory area for attribute    */
/*                                       list                         */
/*              signed32    *min_pos;    minimum position of read     */
/*                                       attributes                   */
/*              signed32    *max_pos;    maximum position of read     */
/*                                       attributes                   */
/*              signed32     inherit;    Parameter specifying how MK  */
/*                                       and ACL attributes will be   */
/*                                       set for the appended entries */
/*              Name_string  parent_obj; DN of the parent object under*/
/*                                       which the subtree will be    */
/*                                       appended (not normalized)    */
/*					 when use_flag is APPEND_SUB, */
/*              			 DN of the parent object under*/
/*                                       which the shadow will be     */
/*                                       created (not normalized)     */
/*					 when use_flag is CREATE_SHD, */
/*              			 New DN to be added 	      */
/*					 when use_flag is CHANGE_NAM. */
/*					 or use_flag is CHANGE_MAS.   */
/*              Name_string  subtree;    DN of the object at the top  */
/*                                       of the subtree which will be */
/*                                       appended (normalized)        */
/*					 when use_flag is APPEND_SUB, */
/*					 New aliased object name,     */
/*					 when alias is being added    */
/*					 and use_flag is CHANGE_NAM.  */
/*              signed32     new_parent; The subtree has to be        */
/*                                       appended under the same      */
/*                                       parent or not                */
/*		Name_string  bind_dsa;			 	      */
/*              signed16     lenacl;     length of ACL of parent obj. */
/*              char        *pacl;       ACL of parent object         */
/*                                       (= ACL of object under which */
/*                                       the "append" is done         */
/*		char	    *at_val_area;pointer to area for attribute*/
/*				         values.		      */
/*		signed16     use_flag;   flag indicating if function  */
/*					 is called from create shadow */
/*					 or from        append subtree*/
/*					 or from        change name.  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              signed32        *min_pos;   new minimum position of   */
/*                                          read attributes           */
/*              signed32        *max_pos;   new maximum position of   */
/*                                          read attributes           */
/*								      */
/* RETURNVALUE:							      */
/*                 D2_NOERROR: operation successful                   */
/*                 D2_ERROR:   operation fails                        */
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

 signed16 d21t003_add_appnd(
   int          fd,
   D20_offset  *dn,
   char        *at_mem,
   signed32    *min_pos,
   signed32    *max_pos,
   signed32     inherit,
   Name_string  parent_obj,
   Name_string  subtree,
   signed32     new_parent,
   Name_string  bind_dsa,
   signed16     lenacl,
   char        *pacl,
   char        *at_val_area,
   signed16     use_flag)

{                               /*  d21t003_add_appnd()               */

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

        signed16   ins_type = D20__BEGIN | D20__CONTINUE;
        signed16   optype = D20__BEGIN;
        signed16   mod_mode;

        D2_a_type   type;
        signed16    *len;
        unsigned16  *rep;
        char        *val;
        char        *p_attr;
        char        *p_dn;

        d2_ret_val      ret_value = D2_NOERROR;

        D2_a_type       nam_type[NO_NAM_ATTR];
        D2_a_type       old_type;
        byte            t[NO_NAM_ATTR][D2_OBJ_IDL_MAX];
        signed16        anz;
        signed16        add_func;

        char           *p_mess;
        char           *p_max;
        D2_a_type       at_type;
        char            aliased_obj[D2_DNL_MAX + 2];
        char            norm_mk[D2_DNL_MAX];
        Bool            ignore_dnlist;

        char           new_dn[D2_DNL_MAX];
        String         p_new_als;
	char	      *sav_at_val_area;
	char           n_name[D2_DNL_MAX+1];
	char           norm_bind_dsa[D2_DNL_MAX+1];

	signed32       i, no_nam_attr;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/
sav_at_val_area = at_val_area;

for (i=0; i<NO_NAM_ATTR; i++)
	nam_type[i].d2_type = t[i];

if                              /*  errors on reading attributes      */
   (d20_119_read_attr(fd, dn->d20_file_off, d20_beg1 + dn->d20_mem_off,
		      dn->d20_attr_len, at_mem, min_pos, max_pos,
		      &p_dn, &p_attr))
{                               /*  return(D2_ERROR)                  */
  d20_head->d2_errvalue = D20_ADD_ERR;
  return(D2_ERROR);
}                               

switch ((int)use_flag)
	{
	case CHANGE_NAM:
		d21t045_build_dn((char *) subtree, (char *) parent_obj,
				 p_dn, n_name);
   		d20_162_set_name(n_name); /*  set DN         		*/
		break;
	case CHANGE_MAS:
   		d20_162_set_name(p_dn);	/*  set DN         		*/
		break;
	default:
   		if ((new_parent) && 
			((dn->d20_state & D21_REAL_OBJ) == D21_REAL_OBJ))
			{
			/*  The subtree will be appended      */
			/*  under a new parent                */

			if (d21t010_change_dn((Name_string) p_dn,
					parent_obj, subtree, new_dn)
							 == D2_ERROR)
				{
				/* Error building the new DN          */

				d20_head->d2_errvalue = D20_ADD_ERR;
				return(D2_ERROR);
				}

			d20_162_set_name(new_dn);    /*  set DN 	*/
			}
		else
			d20_162_set_name(p_dn);	/*  set DN              */

		break;
	}
				/*  get naming attribute              */
no_nam_attr = d20_121_get_nam_attr((Name_string) p_dn, nam_type);

/* norm name for bind dsa */
strcpy(norm_bind_dsa, (char *)bind_dsa);
d20_norm(norm_bind_dsa, strlen(norm_bind_dsa));

if (!strcmp(d20_beg1 + dn->d20_mem_off, norm_bind_dsa)) 
{   
  ignore_dnlist = TRUE;
}
else
{
  ignore_dnlist = FALSE;
} 

anz = 0;
add_func = D20_1_ADD_OBJ;
if (d20_insert_attr(D20__BEGIN,NULL,0,0,NULL, &at_val_area) == D2_ERROR)
	{
  	d20_head->d2_errvalue = D20_ADD_ERR;
  	return(D2_ERROR);
	} 

while                           /*  attributes found                  */
      (!d20_attr_from_message(optype, FALSE, p_attr, dn->d20_attr_len,
		   &type, &len, &rep, &val, &p_mess, &p_max, &at_type))
{                               /*  insert attribute in queue         */
    
   if                           /*  If NEW_ACL, ACL attribute is not  */
                                /*  inserted.                         */
      ((d20_match_rdn_type(&type, no_nam_attr, nam_type )!=0) &&
       (!new_parent || D2_ATTCMP(&type,D2_MASTER_KNOWLEDGE)!=0))
   {   
     anz++;
     if                         /*  max number of attributes reached  */
        (anz == D2_AT_MAX)
     {                          /*  break                             */
       break;
     }
else
{                          /*  fill ADD_OBJECT attribute block   */
				/*  or alias attribute block          */
       
       if (D2_ATTCMP(&type,D2_MASTER_KNOWLEDGE)==0)
       {   
	  /* norm name for master-knowledge attr. */
          strcpy(norm_mk, val);
	  d20_norm(norm_mk, strlen(norm_mk));
       } 
       if (D2_ATTCMP(&type,D2_DNLIST)==0 && ignore_dnlist == TRUE)
       {   
         optype = D20__CONTINUE;
         continue;
       }  
       if (inherit == NEW_ACL && D2_ATTCMP(&type,D2_ACL)==0)
       {   
 	 old_type.d2_type = type.d2_type;
 	 old_type.d2_typ_len = type.d2_typ_len;

	 if (d20_insert_attr(D20__CONTINUE,&type,lenacl,*rep,pacl,
						 &at_val_area) == D2_ERROR)
		{
  		d20_head->d2_errvalue = D20_ADD_ERR;
  		return(D2_ERROR);
		} 
       }
else if (D2_ATTCMP(&type,D2_ALIASED_OBJECT)!=0)
       {   
 	 old_type.d2_type = type.d2_type;
 	 old_type.d2_typ_len = type.d2_typ_len;

	 if (d20_insert_attr(D20__CONTINUE,&type,*len,*rep,val,&at_val_area) ==
								D2_ERROR)
		{
  		d20_head->d2_errvalue = D20_ADD_ERR;
  		return(D2_ERROR);
		} 
       }
else
{                        /* fill alias attribute block         */

	  if (use_flag == CHANGE_NAM)
		{
		/* Attribute value of the alias object*/
		d21t045_build_dn((char *) subtree, (char *) parent_obj,
				 val, n_name);
          	p_new_als=(String) n_name;    
		}
	  else
          	p_new_als=val;        /* Attribute value of the alias object*/

   	  add_func = D20_5_ALIAS;
	  aliased_obj[0] = D2_RDN_SEP ;
	  strcpy(&aliased_obj[1], p_new_als);
	  d20_ins_alias(aliased_obj);
       } 
     }                          
   } 
   optype = D20__CONTINUE;
}                               

if (add_func == D20_1_ADD_OBJ)
{   
  if (d20_insert_attr(D20__END,&type,*len,*rep,val,&at_val_area) == D2_ERROR)
	{
  	d20_head->d2_errvalue = D20_ADD_ERR;
  	return(D2_ERROR);
	} 
} 

if (add_func == D20_5_ALIAS)
	{
	/* serv. cntrls are ignored by DUA.	*/
	/* any valid serv. cntrls can be set.	*/
   	d20_168_set_serv_cntrls();
	}
else
	if (use_flag == CREATE_SHD)
		{
		/* function is called from shadow handling functions.	*/
		if (strcmp(norm_mk, (char *)norm_bind_dsa) == 0)
			/* master knowledge is same as that of bind dsa */
			/* object should already exist in the target    */
			/* dsa, so don't try to add this object.	*/
			return(D2_NOERROR);
		
   		d20_168_set_serv_cntrls();
		}
	else
		{
		if (new_parent == TRUE)
   			d20_172_set_master_serv_cntrls();
		else
			if (strcmp(norm_mk, (char *)norm_bind_dsa) == 0)
   				d20_172_set_master_serv_cntrls();
			else
   				d20_168_set_serv_cntrls();
		}

for ( ; ; )
{                               /*  ADD_OBJECT                        */
  if                            /*  ADD_OBJECT is successful          */
     ((ret_value = d20_call_iapl(add_func)) == D2_NOERROR)
  {                             
    at_val_area = sav_at_val_area;
    if                          /*  attributes still found            */
       (anz == D2_AT_MAX)
    {                           
      anz = 0;
      ins_type = D20__BEGIN | D20__CONTINUE;
      do                        
	{                       
	 mod_mode = (D27_ATTCMP(&old_type,&type)==0) ? D2_ADD_AV : D2_ADDATT;
	  if (d20_match_rdn_type(&type, no_nam_attr, nam_type)!=0)
	  {   
	    if                  /*  max number of attributes reached  */
	       (anz == D2_AT_MAX)
	    {                   /*  MODIFY_ENTRY                      */
	      if (d20_insert_modify(D20__END, mod_mode, &type, *len, *rep,
				(Octet_string) val, &type, *len, *rep,
				(Octet_string) val,&at_val_area) == D2_ERROR)
	      {
		d20_head->d2_errvalue = D20_MOD_ERR;
		return(D2_ERROR);
	      }

	      for ( ; ; )
	      { 
		if              /*  operation fails                   */
		   ((ret_value = d20_call_iapl(D20_3_MOD)) != D2_NOERROR)
		{   
		  if (d20_head->d2_errvalue == D2_TOO_BUSY)
		  {             /*  wait a few seconds and try again  */
		    sleep(5);
		    continue;
		  }
else
{
		    d20_head->d2_errvalue = D20_MOD_ERR;
		    break;
		  } 
		}
else
{
    		  at_val_area = sav_at_val_area;
		  break;
		}               
	      } 
	      if (ret_value != D2_NOERROR)
	      {   
		break;
	      } 
	      anz = 0;
	      ins_type = D20__BEGIN | D20__CONTINUE;
	    }                   
				/*  fill MODIFY-ENTRY attribute block */
	    anz++;
 	    old_type.d2_type = type.d2_type;
 	    old_type.d2_typ_len = type.d2_typ_len;

	    if (d20_insert_modify(ins_type, mod_mode, &type, *len, *rep,
				(Octet_string) val, &type, *len, *rep,
				(Octet_string) val,&at_val_area) == D2_ERROR)
		{
		d20_head->d2_errvalue = D20_MOD_ERR;
		return(D2_ERROR);
		}
	    ins_type = D20__CONTINUE;
	  } 
	}                       
      while                     /*  attributes found                  */
	    (!d20_attr_from_message(optype, FALSE, p_attr,
				    dn->d20_attr_len, &type, &len, &rep,
				    &val, &p_mess, &p_max, &at_type));
      if                        /*  attributes to be modified         */
	 (anz && ret_value == D2_NOERROR)
      {                         /*  MODIFY_ENTRY                      */
	if (d20_insert_modify(D20__END, mod_mode, &type, *len, *rep,
				(Octet_string) val, &type, *len, *rep,
				(Octet_string) val,&at_val_area) == D2_ERROR)
		{
		d20_head->d2_errvalue = D20_MOD_ERR;
		return(D2_ERROR);
		}

	for ( ; ; )
	{ 
	  ret_value = d20_call_iapl(D20_3_MOD);
	  if (ret_value)
	  {   
	    if (d20_head->d2_errvalue == D2_TOO_BUSY)
	    {                   /*  wait a few seconds and try again  */
	      sleep(5);
	      continue;
	    }
else
{
	      d20_head->d2_errvalue = D20_MOD_ERR;
	      break;
	    } 
	  }
else
{
    	    at_val_area = sav_at_val_area;
	    break;
	  } 
	} 
      }                         
    }                           
    break;
  }
else if (d20_head->d2_errvalue == D2_ALREADY_EXISTS)
  {   
    ret_value = D2_NOERROR;
    break;
  }
else if (d20_head->d2_errvalue == D2_TOO_BUSY)
  {                             /*  wait a few seconds and try again  */
    sleep(5);
    continue;
  }
else
{
    d20_head->d2_errvalue = D20_ADD_ERR;
    break;
  } 
}                               
				/* reset info structure               */
if (add_func == D20_5_ALIAS)
{   
   d20_res_alias();
} 

d20_169_reset_serv_cntrls();
				/*  return(ret_value)                 */
return((signed16) ret_value);

}                               /*  d21t003_add()                     */

 signed32 d20_match_rdn_type(
   D2_a_type	*type,
   signed32	no_nam_attr,
   D2_a_type	nam_type[])
{
signed32 i, ret_value;

/* check if type is one of the naming attributes type.	*/
for (i=0; i<no_nam_attr; i++)
	/* if match found.	*/
	if ((ret_value = D27_ATTCMP(type,&nam_type[i])) == 0)
		break;

return(ret_value);
}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_121_get_nam_attr()                                */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        20.04.88                                              */
/*								      */
/* SYNTAX:      signed32 d20_121_get_nam_attr(p_dn,type)              */
/*              D2_name_string  p_dn;                                 */
/*              D2_a_type       type[];     naming attribute          */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function determines naming attributes (in obj_id */
/*		format) of last RDN in a DN and returns no. of AVAs   */
/*		in last RDN.					      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D2_name_string  p_dn;       DN                        */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              D2_a_type      *type        naming attribute          */
/*								      */
/* RETURNVALUE:							      */
/*		No. of AVAs in last RDN.			      */
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

 signed32 d20_121_get_nam_attr(
   D2_name_string p_dn,
   D2_a_type      type[])

{                               /*  d20_121_get_nam_attr()            */

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

        D2_name_string  dn_ptr;
        D2_name_string  next_ava;
        signed32        i;
        Bool          found;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

dn_ptr = p_dn + strlen((char *)p_dn) - 1;
for (; dn_ptr >= p_dn; dn_ptr--)
  	if (*dn_ptr == D2_RDN_SEP)
		break;

dn_ptr++;

i = 0;
next_ava = dn_ptr;
found = TRUE;
do
	{
	if (found)
		{
		found = FALSE;
		d27_302_str_oid((char *)next_ava, &type[i++]);
		}

	if (*dn_ptr == D2_AVA_SEP)
		{
		found = TRUE;
		next_ava = dn_ptr + 1;
		}
	}
while (*dn_ptr++ != D2_EOS);

return(i);

}                               /*  d20_121_get_nam_attr()            */
