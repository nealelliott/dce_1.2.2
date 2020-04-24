/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21modap.c,v $
 * Revision 1.1.10.2  1996/02/18  19:40:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:08  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:15:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:26  root]
 * 
 * Revision 1.1.8.4  1994/07/19  14:48:49  marrek
 * 	Increment attribute counter for REP_AV twice.
 * 	[1994/07/19  11:55:56  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:21  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:09:51  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:58  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:07:39  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:58:04  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:25:33  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:45:06  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:24:07  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  17:48:41  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:03:03  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/18  08:26:40  marrek
 * 	Various bug fixes for mask handling found at SNI
 * 	[1992/12/17  16:26:03  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  14:23:47  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:37:01  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:49:27  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:13:45  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21modap.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:40:02 $";
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
/* NAME         : d21modap.o   [modify saved objects]                 */
/*								      */
/* AUTHOR       : Josep Pulpillo CDS D - 525                          */
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
extern char       *d20_beg2;    /* memory address of DNs of DSA2      */
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
/* NAME       : d21t004_modify_appnd()                                */
/*								      */
/* AUTHOR:      Josep Pulpillo CDS D - 525                            */
/* DATE:        28.07.89                                              */
/*								      */
/* SYNTAX:      signed16 d21t004_modify_appnd(fd1, dn1, at1_mem, min1,*/
/*                            max1, fd2, dn2, at2_mem, min2, max2,    */
/*                            inherit, parent_obj, subtree,           */
/*                            new_parent, , pacl, lenacl,bind_dsa,    */
/*			      overwrite, name_col,  at_val_area,      */
/*			      use_flag);			      */
/*              int          fd1;                                     */
/*              D20_offset  *dn1;                                     */
/*              char        *at1_mem;                                 */
/*              signed32    *min1;                                    */
/*              signed32    *max1;                                    */
/*              int          fd2;                                     */
/*              D20_offset  *dn2;                                     */
/*              char        *at2_mem;                                 */
/*              signed32    *min2;                                    */
/*              signed32    *max2;                                    */
/*              signed32     inherit;                                 */
/*              Name_string  parent_obj;                              */
/*              Name_string  subtree;                                 */
/*              signed32     new_parent;                              */
/*              char        *pacl;                                    */
/*              signed16     lenacl;                                  */
/*		Name_string  bind_dsa;				      */
/*              Bool         overwrite;                               */
/*              Bool        *name_col;                                */
/*		char	    *at_val_area;			      */
/*		signed16     use_flag;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will modify the specified entry.        */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              int          fd1;        file descriptor of file1     */
/*              D20_offset  *dn1;        offset of DN (DSA1)          */
/*              char        *at1_mem;    memory area for attribute    */
/*                                       list of DN1                  */
/*              signed32    *min1;       minimum position of read     */
/*                                       attributes 1                 */
/*              signed32    *max1;       maximum position of read     */
/*                                       attributes 1                 */
/*              int          fd2;        file descriptor of file2     */
/*              D20_offset  *dn2;        offset of DN (DSA2)          */
/*              char        *at2_mem;    memory area for attribute    */
/*                                       list of DN2                  */
/*              signed32    *min2;       minimum position of read     */
/*                                       attributes 2                 */
/*              signed32    *max2;       maximum position of read     */
/*                                       attributes 2                 */
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
/*              Name_string  subtree;    DN of the object at the top  */
/*                                       of the subtree which will be */
/*                                       appended (normalized)        */
/*              signed32     new_parent; The subtree has to be        */
/*                                       appended under the same      */
/*                                       parent or not                */
/*              char        *pacl;       Memory area containing the   */
/*                                       ACL attribute of the parent  */
/*                                       object of the subtree        */
/*              signed16     lenacl;     Length of the ACL attribute  */
/*		Name_string  bind_dsa;				      */
/*              Bool         overwrite;  overwrite entry or not       */
/*		char	    *at_val_area;pointer to memory            */
/*					  area for attribute values.  */
/*		signed16     use_flag;  flag indicating if function   */
/*					is called from create shadow  */
/*					or from        append subtree */
/*					or from        change name    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              signed32        *min1;      new minimum position of   */
/*                                          read attributes 1         */
/*              signed32        *max1;      new maximum position of   */
/*                                          read attributes 1         */
/*              signed32        *min2;      new minimum position of   */
/*                                          read attributes 2         */
/*              signed32        *max2;      new maximum position of   */
/*                                          read attributes 2         */
/*              Bool            *name_col;  indicates naming collision*/
/*                                                                    */
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

 signed16 d21t004_modify_appnd(
   int          fd1,
   D20_offset  *dn1,
   char        *at1_mem,
   signed32    *min1,
   signed32    *max1,
   int          fd2,
   D20_offset  *dn2,
   char        *at2_mem,
   signed32    *min2,
   signed32    *max2,
   signed32     inherit,
   Name_string  parent_obj,
   Name_string  subtree,
   signed32     new_parent,
   char        *pacl,
   signed16     lenacl,
   Name_string  bind_dsa,
   Bool         overwrite,
   Bool        *name_col,
   char	       *at_val_area,
   signed16     use_flag)

{                               /*  d21t004_modify_appnd()            */

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

	Bool       obj_al = FALSE;  

        signed16   ins_type = D20__BEGIN | D20__CONTINUE ;
        signed16   optype = D20__BEGIN;
        signed16   opmode,i;

        char    *p_attr1;
        char    *p_attr2;
        char    *p_dn1;
        char    *p_dn2;

        D2_a_type   nam_type[NO_NAM_ATTR];
        byte        t[NO_NAM_ATTR][D2_OBJ_IDL_MAX];

        D2_a_type   time_stamp; 

        D2_a_type   type1;
        signed16   *len1;
        unsigned16 *rep1;
        char       *val1;

        D2_a_type   type2;
        signed16   *len2;
        unsigned16 *rep2;
        char       *val2;
				   /* only for delete              */
        D2_a_type   type3;
        signed16   *len3;
        unsigned16 *rep3;
        char       *val3;

        char       *pmess1;
        char       *pmax1;
        D2_a_type   at_type1;

        char       *pmess2;
        char       *pmax2;
        D2_a_type   at_type2;

        char       *pmess3;
        char       *pmax3;
        D2_a_type   at_type3;

        Bool        do_update;
        Bool        attr_typ_exist;
        Bool        attr_exist;

        D2_a_type   old_type;
        Bool        first_attr = TRUE;

        char        new_dn1[D2_DNL_MAX];

        Bool        modify_called = FALSE;

				/* flag indicating whether scheme     */
				/* object is to be modified           */
        Bool        scheme_mod = FALSE;
        signed16    s_opmode;
				/* a flag indicating whether time stamp */
				/* of scheme object was found or not    */
        Bool        t_stamp = FALSE;
				/* length of time stamp value in list1*/
        signed16    s_len1 = 0;
				/* length of time stamp value in list2*/
        signed16    s_len2 = 0;
				/* time stamp value in list1          */
        unsigned16  s_rep1 = 0;
        unsigned16  s_rep2 = 0;
        char       *s_val1 = (char *) NULL;
				/* time stamp value in list2          */
        char       *s_val2 = (char *) NULL;
        Bool        ignore_dnlist;

register d2_ret_val ret_value = D2_NOERROR;
	char	   *sav_at_val_area;

	signed32    no_nam_attr;
	char        n_name[D2_DNL_MAX+1];
	char        norm_scheme[D2_DNL_MAX+1];
	char        norm_bind_dsa[D2_DNL_MAX+1];
        char        norm_mk[D2_DNL_MAX+1];

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

sav_at_val_area = at_val_area;

time_stamp.d2_typ_len = sizeof(D2_TIME_STAMP)-1;
time_stamp.d2_type = (Ob_id_string) D2_TIME_STAMP;

for (i=0; i<NO_NAM_ATTR; i++)
	nam_type[i].d2_type = t[i];


/*  errors on reading attributes of object of DSA1     	*/
/*  from saved file                   			*/
if (d20_119_read_attr(fd1, dn1->d20_file_off, d20_beg1+dn1->d20_mem_off,
		      dn1->d20_attr_len, at1_mem, min1, max1, &p_dn1,
		      &p_attr1))
	{/*  return(D2_ERROR)                     */
  	d20_head->d2_errvalue = D20_MOD_ERR;
  	return(D2_ERROR);
	}

/*  errors on reading attributes of object of DSA1     	*/
/*  from saved file                   			*/
if (d20_119_read_attr(fd2, dn2->d20_file_off, d20_beg2 + dn2->d20_mem_off,
		      dn2->d20_attr_len, at2_mem, min2, max2, &p_dn2,
		      &p_attr2))
	{/*  return(D2_ERROR)                     */
  	d20_head->d2_errvalue = D20_MOD_ERR;
  	return(D2_ERROR);
	}

strcpy(new_dn1, p_dn1);
d20_norm(new_dn1, strlen(p_dn1));

strcpy(norm_scheme, D2_NAME_SCHEME);
d20_norm(norm_scheme, strlen(norm_scheme));

/*  scheme object is to be handled    */
if (strcmp(new_dn1, norm_scheme) == 0)
  	scheme_mod = TRUE;

if (use_flag == CHANGE_NAM)
   	d20_162_set_name(p_dn2);	/*  set DN			*/
else
	/*  The object belongs to a different */
	/*  parent object                     */
	if ((new_parent) && ((dn1->d20_state & D21_REAL_OBJ) == D21_REAL_OBJ))
		{
   		/* Error building the new DN          			*/
      		if (d21t010_change_dn((Name_string) p_dn1, parent_obj, subtree,
					new_dn1) == D2_ERROR)
			{
      			d20_head->d2_errvalue = D20_MOD_ERR;
      			return(D2_ERROR);
			}

   		d20_162_set_name(new_dn1);   /*  set DN        		*/
		}
	else
   		d20_162_set_name(p_dn1);     /*  set DN 		*/

if ((((dn1)->d20_state & D21_ALIAS_OBJ)==D21_ALIAS_OBJ) ||
    (((dn2)->d20_state & D21_ALIAS_OBJ)==D21_ALIAS_OBJ))
	obj_al = TRUE;

if (obj_al)
	{/*  The function is handling alias    */

   	while (!d20_attr_from_message(optype, TRUE, p_attr1, 
				      dn1->d20_attr_len, &type1, &len1, &rep1, 
				      &val1, &pmess1, &pmax1, &at_type1))
   		{ /*  get aliased object                */
     		if (D2_ATTCMP(&type1,D2_ALIASED_OBJECT)==0)
			{
			if (use_flag == CHANGE_NAM)
				{
				d21t045_build_dn((char *) subtree,
					(char *) parent_obj, val1, n_name);
				val1 = (char *)n_name;
				*len1 = strlen(n_name) + 1;
				}
       			break;
			}
    		else 
       			optype = D20__CONTINUE;
   		} 

   	optype = D20__BEGIN;
   	while (!d20_attr_from_message(optype, TRUE, p_attr2, 
				      dn2->d20_attr_len, &type2, &len2, &rep2, 
				      &val2, &pmess2, &pmax2, &at_type2))
   		{/*  get aliased object                */
     		if (D2_ATTCMP(&type2,D2_ALIASED_OBJECT)==0)
       			break;
		else
       			optype = D20__CONTINUE;
		}

   	do_update = FALSE;
  	if ((D27_ATTCMP(&type1,&type2)==0) &&
	    (D2_ATTCMP(&type1,D2_ALIASED_OBJECT)==0))
		{
      		if (strcmp(val1, val2))
	 		do_update = TRUE;
     		else 
	 		*name_col = FALSE;
		}
	else
     		do_update = TRUE;

   	if (do_update == TRUE)
		{
	 	if (overwrite == TRUE)
			{
			if (use_flag == CREATE_SHD)
				/* function is called from shadow handling */
				/* functions.				   */
   				d20_168_set_serv_cntrls();
			else
	   			d20_172_set_master_serv_cntrls();

	   		for ( ; ; )
	  			{ 
	     			if (d20_call_iapl(D20_2_DEL_OBJ))
					{
	       				if (d20_head->d2_errvalue ==D2_TOO_BUSY)
	       					{
					/*  wait a few seconds and try again  */
		 				sleep(5);
		 				continue;
						}
		 			else
						{
						d20_head->d2_errvalue = 
								D20_DEL_ERR;
		 				return(D2_ERROR);
						}
					}
				else
	       				break;
				}

			d20_169_reset_serv_cntrls();

	   		return(d21t003_add_appnd(fd1, dn1, at1_mem, min1, max1,
						 inherit, parent_obj, subtree, 
						 new_parent, bind_dsa, lenacl, 
						 pacl,at_val_area,use_flag));
			}
		else 
	   		*name_col = TRUE;
		}

   	return(D2_NOERROR);
	}

/*  get naming attribute              */
no_nam_attr = d20_121_get_nam_attr((D2_name_string) p_dn1, nam_type);

/* norm name for bind dsa */
strcpy(norm_bind_dsa, (char *)bind_dsa);
d20_norm(norm_bind_dsa, strlen(norm_bind_dsa));

if (!strcmp(d20_beg1 + dn1->d20_mem_off, norm_bind_dsa)) 
  	ignore_dnlist = TRUE;
else
  	ignore_dnlist = FALSE;

/*  initialize counter                */
for ( ; ret_value == D2_NOERROR; )
	{
  	i = 0;

	/* read attr. of object1 (of DSA1).	*/
  	while (!d20_attr_from_message(optype,TRUE, p_attr1, 
				      dn1->d20_attr_len, &type1, &len1, &rep1, 
				      &val1, &pmess1, &pmax1, &at_type1))
  		{/*  look in restore file              */
    		attr_exist = FALSE;
    		attr_typ_exist = FALSE;
    		optype = D20__BEGIN;

		/* skip the naming attribute.		*/
    		if (d20_match_rdn_type(&type1,no_nam_attr,nam_type)==0)
			{
      			first_attr = FALSE;
      			optype = D20__CONTINUE;
      			old_type.d2_type = type1.d2_type;
      			old_type.d2_typ_len = type1.d2_typ_len;
      			continue;
			}

		/* read attr. of object2 (of DSA2).	*/
    		while (!d20_attr_from_message(optype, TRUE, p_attr2, 
					      dn2->d20_attr_len, &type2, &len2,
					      &rep2, &val2, &pmess2, &pmax2, 
					      &at_type2))
    			{ /*  insert attribute in queue         */
      			optype = D20__CONTINUE;

			/* skip naming attribute.	*/
      			if (d20_match_rdn_type(&type2,no_nam_attr, nam_type)==0)
				{
				D2_STSHORT(*len2, D20_HDLD);
				continue;
				}

        		/* attribute types equal              */
      			if (D27_ATTCMP(&type1,&type2)==0)
      				{
				/* compare value and set len to "-1"  */
				/* as sign that the attribute has     */
				/* been found                         */

				attr_typ_exist = TRUE;

 				/* is the MK attribute                */
				if (D2_ATTCMP(&type1,D2_MASTER_KNOWLEDGE)==0)
					{
					/* norm name for master-knowledge attr*/
					strcpy(norm_mk, val2);
					d20_norm(norm_mk, strlen(norm_mk));
					/* MK attribute must not be modified  */
	   				attr_exist = TRUE;
					}
				else
					if (D2_ATTCMP(&type1,D2_DNLIST)==0 && 
					    ignore_dnlist == TRUE)
						{
          					optype = D20__CONTINUE;
	  					attr_exist = TRUE;
	  					break;
						}
					else
						/* Is the ACL attribute */
	   					if (D2_ATTCMP(&type1,D2_ACL)==0
						    && inherit)
	   						{
					/* ACL is really different            */
	      						if (*len2 != lenacl || 
							    memcmp(
							    (Octet_string) val2,
					   		    (Octet_string) pacl,
							    lenacl) != 0)
	  							{ 
	     							attr_exist = 
								FALSE; 
	     							val1 = pacl;
	     							*rep1 = D2_ACCL;
	     							*len1 = lenacl;
								}
	   						else
	     							attr_exist = 
									TRUE;
	  						} 
						else
							{
						/* is normal attribute.	*/
	   						if (*len1 == *len2 &&
							   memcmp(
							   (Octet_string) val1, 
							   (Octet_string) val2,
							   *len1) == 0)
	      							attr_exist = 
									TRUE;
	  						else 
	      							attr_exist = 
								FALSE;
	  						} 
				break; /* matching type found break.	*/
				}
			}

		/* particular attribute does not exist.	*/
    		if (attr_exist == FALSE)
    			{ /* update                            */

      			/* attribute type already there      */
			if (attr_typ_exist == TRUE)
     				{ 
				/* attribute is time stamp of scheme object*/
	   			if (scheme_mod == TRUE && 
				    D2_ATTCMP(&type2,D2_TIME_STAMP)==0)
					{
					/* store old and new attribute value  */
	  				s_opmode = D2_REP_AV;
	  				s_len1 = *len1;
	  				s_val1 = val1;
	  				s_rep1 = *rep1;
	  				s_len2 = *len2;
	  				s_rep2 = *rep2;
	  				s_val2 = val2;
	  				t_stamp = TRUE;
	  				i--;
					}
				else
					{
				/* put attribute in modify attribute block */
	  				opmode = D2_REP_AV;
					i++; /* REP_AV has two values ! */

	  				if (d20_insert_modify(ins_type, opmode,
					    &type2, *len2, *rep2,
					    (Name_string) val2, &type1, *len1,
					    *rep1, (Name_string) val1,
					    &at_val_area) == D2_ERROR)
						{
  						d20_head->d2_errvalue = 
								D20_MOD_ERR;
  						return(D2_ERROR);
						}

	  				ins_type = D20__CONTINUE ;
					}
				} /* end of type exists.	*/
      			else
				{ /* type does not exist.	*/
				if (first_attr == TRUE)
	  				opmode = D2_ADDATT;
				else
	  				if (D27_ATTCMP(&type1,&old_type)==0)
	  					opmode = D2_ADD_AV;
	  				else
						opmode = D2_ADDATT;

				/* attribute is time stamp of scheme object*/
	   			if (scheme_mod == TRUE && 
				    D2_ATTCMP(&type2,D2_TIME_STAMP)==0)
					{
					/* store old and new attribute value  */
	  				s_opmode = opmode;
	  				s_len1 = s_len2 = *len1;
	  				s_rep1 = s_rep2 = *rep1;
	  				s_val1 = s_val2 = val1;
	  				t_stamp = TRUE;
	  				i--;
					}
				else
					{
				  /* put attribute in modify attribute block */
	  				if (d20_insert_modify(ins_type, opmode, 
					    &type1, *len1, *rep1,
					    (Name_string) val1, &type1, *len1,
					    *rep1, (Name_string) val1,
					    &at_val_area) == D2_ERROR)
						{
  						d20_head->d2_errvalue = 
								D20_MOD_ERR;
  						return(D2_ERROR);
						}
	  				ins_type = D20__CONTINUE ;
					}
				} /* end of type does not exist.	*/

			i++;
			} /* end of attribute does not exist.	*/

     		if (D27_ATTCMP(&type1,&type2)==0)
			D2_STSHORT(*len2, D20_HDLD);

    		first_attr = FALSE;
    		old_type.d2_type = type1.d2_type;
    		old_type.d2_typ_len = type1.d2_typ_len;

    		optype = D20__CONTINUE;

		if (i + 1 >= D2_AT_MAX)
			{
			i = D2_AT_MAX;
			break;
			}

		} /* end of main while loop.	*/

	/* delete all attributes in the from  */
	/* the restore DSA which are not handled */
  	optype = D20__BEGIN;

  	/*  attributes in restore file found  */
     	while (i < D2_AT_MAX && !d20_attr_from_message(optype, TRUE, p_attr2,
				 dn2->d20_attr_len, &type2, &len2, &rep2, &val2,
				 &pmess2, &pmax2, &at_type2))
		{
  		/*  insert attribute in queue         */
		/*  check whether delete attribute    */
		/*  or delete attribute value is needed */

     		if (d20_match_rdn_type(&type2, no_nam_attr, nam_type)==0)
			{
      			D2_STSHORT(*len2, D20_HDLD);
      			continue;
			}

    		optype = D20__BEGIN;
     		while (!d20_attr_from_message(optype, FALSE, p_attr2, 
			dn2->d20_attr_len, &type3, &len3, &rep3, &val3, &pmess3,
			&pmax3, &at_type3))
			{
      			optype = D20__CONTINUE;

      			if (D27_ATTCMP(&type2,&type3)==0)
      				{ /* checkl whether it is handled      */
				if (*len3 == D20_HDLD)
					{ /* del attribute value              */
	  				opmode = D2_DEL_AV;

	  				if (d20_match_rdn_type(&type2,
						no_nam_attr, nam_type)!=0)
						{
	    			/* attribute is time stamp of scheme object*/
	       					if (scheme_mod == TRUE && 
						    D2_ATTCMP(&type2,
							D2_TIME_STAMP)==0)
	    						{
					/* store old and new attribute value  */
	      						s_opmode = opmode;
	      						s_len1 = s_len2 = *len2;
	      						s_rep1 = s_rep2 = *rep2;
	      						s_val1 = s_val2 = val2;
	      						t_stamp = TRUE;
	      						i--;
							}
	    			/* put attribute in modify attribute block */
	      					else
							{
							if (d20_insert_modify(
							    ins_type, opmode, 
							    &type2,*len2, *rep2,
							    (Name_string) val2,
							    &type2, *len2,*rep2,
							    (Name_string) val2,
							    &at_val_area) 
								== D2_ERROR)
								{
  								d20_head->
								d2_errvalue =
								D20_MOD_ERR;
  								return(D2_ERROR);
								}
	      						ins_type =D20__CONTINUE;
							}
						}

	  				D2_STSHORT(*len2, D20_HDLD);
	  				break;
					}
				else
					{/* del attribute                     */
	  				opmode = D2_DELATT;

	  				if (d20_match_rdn_type(&type2,
						no_nam_attr, nam_type)!=0)
						{
	    			/* attribute is time stamp of scheme object*/
	       					if (scheme_mod == TRUE && 
						    D2_ATTCMP(&type2,
						    D2_TIME_STAMP)==0)
	    						{
					/* store old and new attribute value  */
	      						s_opmode = opmode;
	      						s_len1 = s_len2 = *len2;
	      						s_rep1 = s_rep2 = *rep2;
	      						s_val1 = s_val2 = val2;
	      						t_stamp = TRUE;
	      						i--;
							}
						else
	    						{
				/* put attribute in modify attribute block */
	      						if (d20_insert_modify(
							    ins_type, opmode, 
							    &type2,*len2, *rep2,
							    (Name_string) val2,
							    &type2, *len2,*rep2,
							    (Name_string) val2,
							    &at_val_area)
								== D2_ERROR)
								{
  								d20_head->
								d2_errvalue = 
								D20_MOD_ERR;
  								return(D2_ERROR);
								}
	
		      					ins_type =D20__CONTINUE;
							}
						}
	
		  			D2_STSHORT(*len2, D20_HDLD);
	
		  			/* check whether the attribute is    */
					/* recurring                         */
		    			while (!d20_attr_from_message(optype, 
						FALSE, p_attr2, 
						dn2->d20_attr_len, &type3, 
						&len3, &rep3, &val3, &pmess3, 
						&pmax3, &at_type3))
						{
		    				if (D27_ATTCMP(&type2,&type3)
						    == 0)
		    			/* the attribute is recurring        */
		      					D2_STSHORT(*len3, 
								D20_HDLD);
		    				else
		      					break;
						}

					}

				}

			} /* end of while loop.	*/

    		optype = D20__CONTINUE;
    		if (++i == D2_AT_MAX)
    			/* i = max number o attributes       */
       			break;
	
		} /* end of while loop.	*/

  	if (i != 0 || t_stamp == TRUE)
		{
    		if (overwrite == TRUE)
			{
      			/* time stamp of scheme object not found */
		        /* or (modify attribute list is full and */
		        /*     time stamp found)                 */
	 		if (t_stamp == FALSE || (i == D2_AT_MAX && 
							t_stamp == TRUE))
				{
      				/* terminate list                      */
				ins_type = D20__END ;

				if (d20_insert_modify(ins_type, opmode, &type2,
				    *len2, *rep2, (Name_string) val2, &type2,
				    *len2, *rep2, (Name_string) val2,
				    &at_val_area) == D2_ERROR)
					{
  					d20_head->d2_errvalue = D20_MOD_ERR;
  					return(D2_ERROR);
					}
				}
      			else
				{
				/* put time stamp in modify attribute queue */
				ins_type |= D20__END;

				if (d20_insert_modify(ins_type, 
				    s_opmode, &time_stamp, s_len2, 
				    s_rep2, (Name_string) s_val2, &time_stamp, 
				    s_len1, s_rep1, (Name_string) s_val1, 
			   	    &at_val_area) == D2_ERROR)
					{
  					d20_head->d2_errvalue = D20_MOD_ERR;
  					return(D2_ERROR);
					}
				}

      			if (strcmp(norm_mk, (char *)norm_bind_dsa) == 0)
	 			d20_172_set_master_serv_cntrls();
	 		else
				d20_168_set_serv_cntrls();

      			modify_called = TRUE;

      			for ( ; ; )
				{
				ret_value = d20_call_iapl(D20_3_MOD);
				if (ret_value)
					{
	  				if (d20_head->d2_errvalue == 
							D2_TOO_BUSY)
	  					{
					/*  wait a few seconds and try again  */
	    					sleep(5);
	    					continue;
						}
	  				else
						{
	    					d20_head->d2_errvalue =
							 D20_MOD_ERR;
	    					break;
						}
					}
				else
					{
	  				at_val_area = sav_at_val_area;
	  				break;
					}
				}

			d20_169_reset_serv_cntrls();

      			ins_type = D20__BEGIN | D20__CONTINUE ;
      			optype = D20__CONTINUE;
   			}
		else 
      			break;
		}

  	if (i < D2_AT_MAX)
    		break;

	}

if (i == 0 && modify_called == FALSE && t_stamp == FALSE)
  	*name_col = FALSE;


return(ret_value);

}                               /*  d21t004_modify_appnd()                  */


