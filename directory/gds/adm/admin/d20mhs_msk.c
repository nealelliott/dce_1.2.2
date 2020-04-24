/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20mhs_msk.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:49  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:17:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:59  root]
 * 
 * Revision 1.1.8.4  1994/09/06  15:23:04  keutel
 * 	OT 11932: displays O/R address type: Mnemonic
 * 	[1994/09/06  15:21:54  keutel]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:37  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:53:48  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:11:46  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:05:25  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:09:50  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:58:38  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:30:10  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:11:36  marrek
 * 	Function declarations are LOCAL.
 * 	[1993/08/09  12:23:33  marrek]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/30  08:30:34  marrek]
 * 
 * Revision 1.1.4.4  1993/01/29  08:36:09  marrek
 * 	noerase algo. for MHS masks changed.
 * 	[1993/01/28  10:34:57  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:08:52  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:04:35  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:14:17  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:10:38  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:54:12  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:33:02  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  19:36:49  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:48:10  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:33:53  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20mhs_msk.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:03 $";
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
/* NAME         : mhs_msk.c                                           */
/*								      */
/* AUTHOR       : S. Jain, SNI AP 11                                  */
/* DATE         : 21.03.1991                                          */
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
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*								      */
/* HISTORY 	:						      */
/*								      */
/* Vers.Nr. |  Date   |	 Updates    		       | KZ | CR# FM# */
/*      0.1 | 21.03.91| Erstellung                     | ws |         */
/*								      */
/*datoff **************************************************************/

/* Includes */

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>

#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d21iapl.h>
#include <d2spec_msk.h>
#include <cget.h>
#include <d2mhs_msk.h>
#include <d21spat.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define	FLEN	(D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT)

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

extern  char       d20_attr_name[] ;     /* attribute name in mask     */

extern  D20_omask  d20_qomask[];         /* input fields of error mask */
extern  char      *d20_errmess[];      /* error table                */

extern  char	  *d20_oraddifields[];
extern  char      *d20_moraddifields[] ;
extern  char      *d20_noraddifields[] ;
extern  char      *d20_soraddifields[] ;
extern  char      *d20_uoraddifields[] ;
extern  char      *d20_toraddifields[] ;
extern  char      *d20_ornameifields[] ;
extern  char      *d20_dlpermifields[] ;
extern  char      *d20_mogifields[] ;

extern  D20_omask  d20_oraddomask[];  
extern  D20_omask  d20_moraddomask[];
extern  D20_omask  d20_noraddomask[];
extern  D20_omask  d20_soraddomask[];
extern  D20_omask  d20_uoraddomask[];
extern  D20_omask  d20_toraddomask[];
extern  D20_omask  d20_ornameomask[];
extern  D20_omask  d20_7aomask[];
extern  D20_omask  d20_dlpermomask[];
extern  D20_omask  d20_mogomask[];

extern  Bool  	   d20_printable;

/* Functions */

/******** LOCAL  ********/
static signed32 d20_get_oradd_type(D2_OR_address *oradd);
static void d20_cp_to_oradd(signed32 oradd_type, D2_OR_address *oradd,
				Ds_property *prty);
static void d20_cp_from_oradd(	signed32 oradd_type,
				D2_OR_address *oradd,
				Ds_property *prty);
static void d20_cp_str_list_to_field(D2_str_list *str_list, 
				     char field[], unsigned16	fld_space);
static void d20_cp_field_to_str_list(char field[],
				    D2_str_list **str_list, unsigned16 rep);
static signed32 d20_oraddmask_valid(signed32 oradd_type);

static signed32 d20_ornamemask_valid(char dir_name[]);
static void d20_cp_to_orname(D2_OR_name *orname, char dir_name[]);
static void d20_cp_from_orname(D2_OR_name *orname);


/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_oradd()                                           */
/*								      */
/* AUTHOR:      Sanjay Jain DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: signed32 d20_oradd(op, attr_name, oradd, new_oradd,   */
/*						     mod_type)        */
/*              signed32   op;                                        */
/*              char      *attr_name                                  */
/*              D2_OR_address  *oradd;				      */
/*              D2_OR_address  *new_oradd;			      */
/*		signed16       *mod_type;			      */
/*								      */
/*		This function handles special masks of OR address.    */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed32   op;                                        */
/*              char      *attr_name                                  */
/*              D2_OR_address  *oradd;                                */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*              D2_OR_address  *oradd; In case of ADD-ATTRIBUTE/OBJ.  */
/*              D2_OR_address  *new_oradd; In case of MOD-ATTRIBUTE.  */
/*		signed16	*mod_type; In case of MOD-ATTRIBUTE.  */
/*								      */
/* RETURNVALUE:         fkt      funktionkey                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

signed32 d20_oradd(signed32 op, char *attr_name, D2_OR_address *oradd,
		D2_OR_address *new_oradd, signed16 *mod_type)
{
signed32           fkt, i;
D20_omask          sav_poraddomask[D20_M_SIZE];
D20_omask          sav_7aomask[D20_M_SIZE];
D20_omask	   *poraddomask;
Ds_property 	   prty;
signed32	   oradd_type;
signed32	   new_oradd_type;


strcpy(d20_attr_name, attr_name);

/* initialise all masks of OR address.	*/
for (i=DS_MORADD_CNTRY; i<= DS_MORADD_DV4; i++)
	*d20_moraddifields[i] = D2_EOS;

for (i=DS_NORADD_CNTRY; i<= DS_NORADD_DV4; i++)
	*d20_noraddifields[i] = D2_EOS;

for (i=DS_SORADD_CNTRY; i<= DS_SORADD_PSA; i++)
	*d20_soraddifields[i] = D2_EOS;

for (i=DS_UORADD_CNTRY; i<= DS_UORADD_PDSN; i++)
	*d20_uoraddifields[i] = D2_EOS;

for (i=DS_TORADD_ISDNN; i<= DS_TORADD_ISDNS; i++)
	*d20_toraddifields[i] = D2_EOS;

*(signed32 *)d20_toraddifields[DS_TORADD_PA] = FALSE;

for (i=DS_TORADD_X121; i<= DS_TORADD_TI; i++)
	*d20_toraddifields[i] = D2_EOS;

*(signed32 *)d20_toraddifields[DS_TORADD_TT] = FALSE;

for (i=DS_TORADD_DT1; i<= DS_TORADD_DV4; i++)
	*d20_toraddifields[i] = D2_EOS;

switch (op) 
	{
 	case DS_DELETE_ATTRIBUTE:
		fkt = FKTCR;
	       	break;

	case DS_DISPLAY_OBJECTS:
	case DS_DISP_OBJ:
		oradd_type = d20_get_oradd_type(oradd);
		switch (oradd_type)
			{
			case D2_MORADD:
				poraddomask = d20_moraddomask;
				break;
			case D2_NORADD:
				poraddomask = d20_noraddomask;
				break;
			case D2_SORADD:
				poraddomask = d20_soraddomask;
				break;
			case D2_UORADD:
				poraddomask = d20_uoraddomask;
				break;
			case D2_TORADD:
				poraddomask = d20_toraddomask;
				break;
			}

		d20_save_mask(poraddomask, sav_poraddomask);
	       	d20_cp_from_oradd(oradd_type, oradd, &prty);

		/* display particular OR address mask */
		fkt = d20_handlemask(poraddomask);

		if ((fkt == FKTCR) || (fkt == FKTMENU))
			if ((oradd_type == D2_TORADD) &&
		    	(*(signed32 *)(d20_toraddifields[DS_TORADD_PA]) != FALSE))
        		      fkt = ds0_520_handle_spec_attr(DS_DISP_OBJ,&prty);

	       	d20_restore_mask(poraddomask, sav_poraddomask);
	       	break;

	 case DS_MODIFY_ATTRIBUTE:
		{
		signed32	mod_pos, scd_pos;

		oradd_type = d20_get_oradd_type(oradd);
	       	d20_cp_from_oradd(oradd_type, oradd, &prty);

		switch (oradd_type)
		    {
		    case D2_MORADD:
			poraddomask = d20_moraddomask;
			mod_pos = DS_MORADD_MOD_POS;
			scd_pos = DS_MORADD_SCD_POS;
			break;
		    case D2_NORADD:
			poraddomask = d20_noraddomask;
			mod_pos = DS_NORADD_MOD_POS;
			scd_pos = DS_NORADD_SCD_POS;
			break;
		    case D2_SORADD:
			poraddomask = d20_soraddomask;
			mod_pos = DS_SORADD_MOD_POS;
			scd_pos = DS_SORADD_SCD_POS;
			break;
		    case D2_UORADD:
			poraddomask = d20_uoraddomask;
			mod_pos = DS_UORADD_MOD_POS;
			scd_pos = DS_UORADD_SCD_POS;
			break;
		    case D2_TORADD:
			poraddomask = d20_toraddomask;
			mod_pos = DS_TORADD_MOD_POS;
			scd_pos = DS_TORADD_SCD_POS;
			break;
		    }

		/* prepare masks to display old value.	*/

		d20_save_mask(poraddomask, sav_poraddomask);
		/* disable all input fields.	*/
		poraddomask[scd_pos - 1].d20_fkt_id = 
		poraddomask[scd_pos - 2].d20_fkt_id = 
		poraddomask[scd_pos - 3].d20_fkt_id =
		poraddomask[scd_pos - 4].d20_fkt_id = D20_NO_OP;

		if (mod_type != (signed16 *)NULL)
		    {
		    poraddomask[mod_pos].d20_fkt_id = D20_CSAY;
		    poraddomask[mod_pos + 1].d20_fkt_id = D20_CSAY;
		    *(signed32 *)poraddomask[mod_pos + 1].d20_ioinfo = D2_REP_AV;
		    }

		d20_oraddomask[0].d20_fkt_id = D20_NOERASE;
		d20_oraddomask[DS_ORADD_SCD_POS].d20_fkt_id = D20_FKTX;

		if ((oradd_type == D2_TORADD) &&
		    (*(signed32 *)(d20_toraddifields[DS_TORADD_PA]) != FALSE))
		    d20_oraddomask[DS_ORADD_F1_POS].d20_fkt_id = D20_FKTX;

	      	*(signed32 *)d20_oraddifields[DS_ORADD_TYPE] = oradd_type;

		d20_save_mask(d20_7aomask, sav_7aomask);
		/* enable scroll down key. */
    		d20_7aomask[D20_SP3POS].d20_fkt_id = D20_FKTX;

		/* display old value.	*/
	       	for (;;) 
		    {
		    fkt = d20_handlemask(poraddomask);

		    /* display OR address mask over particular OR */
		    /* address mask.			      */
		    fkt = d20_handlemask(d20_oraddomask);

		    if (fkt == FKTF1)
			{
        		fkt = ds0_520_handle_spec_attr(DS_DISP_OBJ, &prty);
			if (fkt == FKTCR || fkt == FKTMENU)
			    continue;
			}
	
		    break;
		    }

	       	d20_restore_mask(d20_7aomask, sav_7aomask);

		d20_oraddomask[0].d20_fkt_id = D20_CSAY;
		d20_oraddomask[DS_ORADD_SCD_POS].d20_fkt_id = 
		d20_oraddomask[DS_ORADD_F1_POS].d20_fkt_id = D20_NO_OP;

	       	d20_restore_mask(poraddomask, sav_poraddomask);

		if ((fkt == FKTCR) || (fkt == FKTMENU))
		    {
	      	    new_oradd_type = *(signed32 *)(d20_oraddifields[DS_ORADD_TYPE]);

		    switch (new_oradd_type)
			{
			case D2_MORADD:
			    poraddomask = d20_moraddomask;
			    mod_pos = DS_MORADD_MOD_POS;
			    scd_pos = DS_MORADD_SCD_POS;
			    break;
			case D2_NORADD:
			    poraddomask = d20_noraddomask;
			    mod_pos = DS_NORADD_MOD_POS;
			    scd_pos = DS_NORADD_SCD_POS;
			    break;
			case D2_SORADD:
			    poraddomask = d20_soraddomask;
			    mod_pos = DS_SORADD_MOD_POS;
			    scd_pos = DS_SORADD_SCD_POS;
			    break;
			case D2_UORADD:
			    poraddomask = d20_uoraddomask;
			    mod_pos = DS_UORADD_MOD_POS;
			    scd_pos = DS_UORADD_SCD_POS;
			    break;
			case D2_TORADD:
			    poraddomask = d20_toraddomask;
			    mod_pos = DS_TORADD_MOD_POS;
			    scd_pos = DS_TORADD_SCD_POS;
			    break;
			}

		    if (new_oradd_type == oradd_type)
			poraddomask[0].d20_fkt_id |= D20_NOERASE;

		    if (mod_type != (signed16 *)NULL)
			{
			poraddomask[mod_pos].d20_fkt_id = D20_CSAY;
			poraddomask[mod_pos + 1].d20_fkt_id = D20_CGET;
			*(signed32 *)poraddomask[mod_pos + 1].d20_ioinfo = D2_REP_AV;
			}

		    /* prepare masks for adding new value.	*/
		    poraddomask[scd_pos].d20_fkt_id = D20_FKTX;

		    /* if new oraddress type is TERMINAL and old 	   */
		    /* oraddress type was also TERMINAL and PSAP was   */
		    /* involved then enable F1 key to look at old PSAP */
		    /* while entering new TERMINAL type of oraddress.  */					
		    if ((new_oradd_type == D2_TORADD) &&
		    	(*(signed32 *)(d20_toraddifields[DS_TORADD_PA]) != FALSE))
			d20_toraddomask[DS_TORADD_F1_POS].d20_fkt_id = D20_FKTX;

		    /* enable scroll down key in PSAP mask. */
    		    d20_7aomask[D20_SP3POS].d20_fkt_id = D20_FKTX;

	       	    for (;;poraddomask[0].d20_fkt_id &= ~D20_NOERASE) 
			{
			fkt = d20_handlemask(poraddomask);

			if (fkt == FKTF1)
			    {
			    d20_save_mask(d20_7aomask, sav_7aomask);

        		    fkt = ds0_520_handle_spec_attr(DS_DISP_OBJ, &prty);

	       		    d20_restore_mask(d20_7aomask, sav_7aomask);

			    if (fkt == FKTCR || fkt == FKTMENU)
				continue;

			    break;
			    }
	
       			if (fkt == FKTCR || fkt == FKTMENU)
			    /* validate particular OR addr. mask. */
			    if (d20_oraddmask_valid(new_oradd_type) == TRUE)
				{
				/* check if PSAP address handling is */
				/* required.			     */
				/* if required handle PSAP.	     */
				if ((new_oradd_type == D2_TORADD) &&
				    (*(signed32 *)(d20_toraddifields[DS_TORADD_PA]) 
				    				!= FALSE))
					if (oradd->d2_presentation_addr !=
							(Psap_string)NULL)
	        		    		fkt = ds0_520_handle_spec_attr(
						DS_MODIFY_ATTRIBUTE,&prty);
					else
	        		    		fkt = ds0_520_handle_spec_attr(
						DS_ADD_ATTRIBUTE,&prty);
		
				if (fkt == FKTCR || fkt == FKTMENU)
				    {
				    if (mod_type != (signed16 *)NULL)
					*mod_type = *(signed32 *)poraddomask[
						    mod_pos + 1].d20_ioinfo;

		    		    d20_cp_to_oradd(new_oradd_type, new_oradd,
								&prty);
				    }
				}
			    else
				continue;	

			break;
			}	 /* end of for loop to get new value from mask*/

		    poraddomask[0].d20_fkt_id &= ~D20_NOERASE;

		    d20_toraddomask[DS_TORADD_F1_POS].d20_fkt_id = D20_NO_OP;

		    /* disenable scroll down key in PSAP mask. */
    		    d20_7aomask[D20_SP3POS].d20_fkt_id = D20_NO_OP;

		    poraddomask[mod_pos].d20_fkt_id = D20_NO_OP;
		    poraddomask[mod_pos + 1].d20_fkt_id = D20_NO_OP;
		    poraddomask[scd_pos].d20_fkt_id = D20_NO_OP;
		    } 	/* end of FKT == CR || FKT == MENU.	*/

		break;
		} 	/* end of modify attribute.	*/

	 case DS_ADD_OBJECT:
	 case DS_ADD_ATTRIBUTE:
	      	*(signed32 *)d20_oraddifields[DS_ORADD_TYPE] = D2_MORADD;

	      	fkt = d20_handlemask(d20_oraddomask);

		if ((fkt == FKTCR) || (fkt == FKTMENU))
			{
	      		oradd_type = *(signed32 *)(d20_oraddifields[DS_ORADD_TYPE]);

			switch (oradd_type)
				{
				case D2_MORADD:
					poraddomask = d20_moraddomask;
					break;
				case D2_NORADD:
					poraddomask = d20_noraddomask;
					break;
				case D2_SORADD:
					poraddomask = d20_soraddomask;
					break;
				case D2_UORADD:
					poraddomask = d20_uoraddomask;
					break;
				case D2_TORADD:
					poraddomask = d20_toraddomask;
					break;
				}

			poraddomask[0].d20_fkt_id |= D20_NOERASE;

			/* loop to get OR address from mask.	*/
	       		for (;;poraddomask[0].d20_fkt_id &= ~D20_NOERASE) 
				{      
				/* display particular OR address mask */
			     	fkt = d20_handlemask(poraddomask);

       				if (fkt == FKTCR || fkt == FKTMENU)
					/* validate particular OR addr. mask. */
					if (d20_oraddmask_valid(oradd_type) == 
								TRUE)
						{
					/* check if PSAP address handling is */
					/* required.			     */
					/* if required handle PSAP.	     */
						if ((oradd_type == D2_TORADD) &&
						    (*(signed32 *)(d20_toraddifields[
						    DS_TORADD_PA]) != FALSE))
	        					fkt = 
							ds0_520_handle_spec_attr
								(op,&prty);
		
       						if (fkt == FKTCR || 
								fkt == FKTMENU)
		    					d20_cp_to_oradd(
							oradd_type,oradd,&prty);
						}
					else
						continue;	
				break;
				} 	/* end of for loop.	*/

			poraddomask[0].d20_fkt_id &= ~D20_NOERASE;

			}	/* end of FKT == CR || FKT == MENU.	*/

	       	break;

	 default:
	       return(D2_ERROR);
    }

return(fkt);

}	/* end of  d20_oradd().		*/



/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_oradd_disp()                                      */
/*								      */
/* AUTHOR:      Sanjay Jain DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: signed32 d20_oradd_disp(object_counter, attr_name,    */
/*					oradd) 			      */
/*              signed32   object_counter;			      */
/*              char      *attr_name                                  */
/*              D2_OR_address  *oradd;				      */
/*								      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed32   object_counter;			      */
/*              char      *attr_name                                  */
/*              D2_OR_address  *oradd;                                */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:         fkt      funktionkey                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

signed32 d20_oradd_disp(signed32 object_counter,
	    char *attr_name,
	    D2_OR_address *oradd)
{
signed32 	oradd_type, fkt;


oradd_type = d20_get_oradd_type(oradd);

if (object_counter != 1)
	{
	switch(oradd_type)
		{
		/*  enable SCROLL_UP and F1 and       		*/
		/*  SCROLL_DOWN in OR address masks           	*/
			case D2_MORADD:
				d20_moraddomask[DS_MORADD_SCD_POS].d20_fkt_id = 
				d20_moraddomask[DS_MORADD_SCU_POS].d20_fkt_id = 
				d20_moraddomask[DS_MORADD_F1_POS].d20_fkt_id = 
								D20_FKTX; 
				break;
			case D2_NORADD:
			 	d20_noraddomask[DS_NORADD_SCD_POS].d20_fkt_id = 
			 	d20_noraddomask[DS_NORADD_SCU_POS].d20_fkt_id = 
			 	d20_noraddomask[DS_NORADD_F1_POS].d20_fkt_id = 
								D20_FKTX; 
				break;
			case D2_SORADD:
			 	d20_soraddomask[DS_SORADD_SCD_POS].d20_fkt_id = 
			 	d20_soraddomask[DS_SORADD_SCU_POS].d20_fkt_id = 
			 	d20_soraddomask[DS_SORADD_F1_POS].d20_fkt_id = 
								D20_FKTX; 
				break;
			case D2_UORADD:
			 	d20_uoraddomask[DS_UORADD_SCD_POS].d20_fkt_id = 
			 	d20_uoraddomask[DS_UORADD_SCU_POS].d20_fkt_id = 
			 	d20_uoraddomask[DS_UORADD_F1_POS].d20_fkt_id = 
								D20_FKTX; 
				break;
			case D2_TORADD:
			 	d20_toraddomask[DS_TORADD_SCD_POS].d20_fkt_id = 
			 	d20_toraddomask[DS_TORADD_SCU_POS].d20_fkt_id = 
			 	d20_toraddomask[DS_TORADD_F1_POS].d20_fkt_id = 
								D20_FKTX; 

				/*  enable SCROLL_UP and F1 and       */
				/*  SCROLL_DOWN in PSAP mask          */
    				d20_7aomask[D20_SP1POS].d20_fkt_id =
    				d20_7aomask[D20_SP2POS].d20_fkt_id =
    				d20_7aomask[D20_SP3POS].d20_fkt_id = D20_FKTX;
				break;
		}	/* end of switch	*/
	}
else
	{
	/*  enable SCROLL_UP */
    	switch(oradd_type)
		{
		case D2_MORADD:
			d20_moraddomask[DS_MORADD_SCU_POS].d20_fkt_id = 
								D20_FKTX; 
			break;
		case D2_NORADD:
		 	d20_noraddomask[DS_NORADD_SCU_POS].d20_fkt_id = 
								D20_FKTX; 
			break;
		case D2_SORADD:
		 	d20_soraddomask[DS_SORADD_SCU_POS].d20_fkt_id = 
								D20_FKTX; 
			break;
		case D2_UORADD:
		 	d20_uoraddomask[DS_UORADD_SCU_POS].d20_fkt_id = 
								D20_FKTX; 
			break;
		case D2_TORADD:
		 	d20_toraddomask[DS_TORADD_SCU_POS].d20_fkt_id = 
								D20_FKTX; 
			/*  enable SCROLL_UP */
    			d20_7aomask[D20_SP1POS].d20_fkt_id = D20_FKTX;
			break;
		}	/* end of switch	*/	
	}

fkt = d20_oradd(DS_DISPLAY_OBJECTS, attr_name, oradd, NULL, NULL);

switch(oradd_type)
	{
	/*  Disable SCROLL_UP and F1 and       		*/
	/*  SCROLL_DOWN in OR address mask           	*/
	case D2_MORADD:
		d20_moraddomask[DS_MORADD_SCD_POS].d20_fkt_id = 
		d20_moraddomask[DS_MORADD_SCU_POS].d20_fkt_id = 
		d20_moraddomask[DS_MORADD_F1_POS].d20_fkt_id = D20_NO_OP; 
		break;
	case D2_NORADD:
	 	d20_noraddomask[DS_NORADD_SCD_POS].d20_fkt_id = 
	 	d20_noraddomask[DS_NORADD_SCU_POS].d20_fkt_id = 
	 	d20_noraddomask[DS_NORADD_F1_POS].d20_fkt_id = D20_NO_OP; 
		break;
	case D2_SORADD:
	 	d20_soraddomask[DS_SORADD_SCD_POS].d20_fkt_id = 
	 	d20_soraddomask[DS_SORADD_SCU_POS].d20_fkt_id = 
	 	d20_soraddomask[DS_SORADD_F1_POS].d20_fkt_id = D20_NO_OP; 
		break;
	case D2_UORADD:
	 	d20_uoraddomask[DS_UORADD_SCD_POS].d20_fkt_id = 
	 	d20_uoraddomask[DS_UORADD_SCU_POS].d20_fkt_id = 
	 	d20_uoraddomask[DS_UORADD_F1_POS].d20_fkt_id = D20_NO_OP; 
		break;
	case D2_TORADD:
	 	d20_toraddomask[DS_TORADD_SCD_POS].d20_fkt_id = 
	 	d20_toraddomask[DS_TORADD_SCU_POS].d20_fkt_id = 
	 	d20_toraddomask[DS_TORADD_F1_POS].d20_fkt_id = D20_NO_OP; 

   		d20_7aomask[D20_SP1POS].d20_fkt_id =
    		d20_7aomask[D20_SP2POS].d20_fkt_id =
    		d20_7aomask[D20_SP3POS].d20_fkt_id = D20_NO_OP;
		break;
	}

return(fkt);

}	/* end of d20_oradd_disp()	*/



/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_oraddmask_valid()				      */
/*								      */
/* AUTHOR:      Sanjay Jain DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: signed32 d20_oraddmask_valid(oradd_type) 	      */
/*		signed32		oradd_type;		      */
/*								      */
/*		This function checks if mandatory components of OR    */
/*		addres (of particular type) are present, and inter-   */
/*		dependencies between some fields (e.g. Domain attr.)  */
/*		are O.K.					      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed32	oradd_type;			      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:         					      */
/*		D2_TRUE: if mask is valid.			      */
/*		D2_FALSE: if mask is invalid.			      */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

static  signed32 d20_oraddmask_valid(signed32 oradd_type)
{
/* check for mandatory components of a particular OR address type (may be */
/* they occur as a group, in which case one of the member from the group  */
/* must be present).							  */

/* check for interdependent components of a particular OR address type.   */
/* (e.g. either both domain type and value components should be present   */
/* or none.).								  */

switch (oradd_type)
	{
	case D2_MORADD:
		if ((d20_moraddifields[DS_MORADD_CNTRY][0] == D2_EOS) ||
		    (d20_moraddifields[DS_MORADD_ADMD][0] == D2_EOS))
			{
			/* display error mask.		*/
			d20_qomask[D20_Q1POS].d20_ioinfo=
					    d20_errmess[D20_E50IDX];
			d20_handlemask(d20_qomask);

			return(FALSE);
			}

		if (((d20_moraddifields[DS_MORADD_DT1][0] == D2_EOS) &&
		     (d20_moraddifields[DS_MORADD_DV1][0] != D2_EOS)) ||
		    ((d20_moraddifields[DS_MORADD_DT1][0] != D2_EOS) &&
		     (d20_moraddifields[DS_MORADD_DV1][0] == D2_EOS)) ||
		    ((d20_moraddifields[DS_MORADD_DT2][0] == D2_EOS) &&
		     (d20_moraddifields[DS_MORADD_DV2][0] != D2_EOS)) ||
		    ((d20_moraddifields[DS_MORADD_DT2][0] != D2_EOS) &&
		     (d20_moraddifields[DS_MORADD_DV2][0] == D2_EOS)) ||
		    ((d20_moraddifields[DS_MORADD_DT3][0] == D2_EOS) &&
		     (d20_moraddifields[DS_MORADD_DV3][0] != D2_EOS)) ||
		    ((d20_moraddifields[DS_MORADD_DT3][0] != D2_EOS) &&
		     (d20_moraddifields[DS_MORADD_DV3][0] == D2_EOS)) ||
		    ((d20_moraddifields[DS_MORADD_DT4][0] == D2_EOS) &&
		     (d20_moraddifields[DS_MORADD_DV4][0] != D2_EOS)) ||
		    ((d20_moraddifields[DS_MORADD_DT4][0] != D2_EOS) &&
		     (d20_moraddifields[DS_MORADD_DV4][0] == D2_EOS)))
			{
			/* display error mask.		*/
			d20_qomask[D20_Q1POS].d20_ioinfo=
					    d20_errmess[D20_E51IDX];
			d20_handlemask(d20_qomask);

			return(FALSE);
			}

		return(TRUE);
		
	case D2_NORADD:
		if ((d20_noraddifields[DS_NORADD_CNTRY][0] == D2_EOS) ||
		    (d20_noraddifields[DS_NORADD_ADMD][0] == D2_EOS) ||
		    (d20_noraddifields[DS_NORADD_NUI][0] == D2_EOS))
			{
			/* display error mask.		*/
			d20_qomask[D20_Q1POS].d20_ioinfo=
					    d20_errmess[D20_E50IDX];
			d20_handlemask(d20_qomask);

			return(FALSE);
			}

		if (((d20_noraddifields[DS_NORADD_DT1][0] == D2_EOS) &&
		     (d20_noraddifields[DS_NORADD_DV1][0] != D2_EOS)) ||
		    ((d20_noraddifields[DS_NORADD_DT1][0] != D2_EOS) &&
		     (d20_noraddifields[DS_NORADD_DV1][0] == D2_EOS)) ||
		    ((d20_noraddifields[DS_NORADD_DT2][0] == D2_EOS) &&
		     (d20_noraddifields[DS_NORADD_DV2][0] != D2_EOS)) ||
		    ((d20_noraddifields[DS_NORADD_DT2][0] != D2_EOS) &&
		     (d20_noraddifields[DS_NORADD_DV2][0] == D2_EOS)) ||
		    ((d20_noraddifields[DS_NORADD_DT3][0] == D2_EOS) &&
		     (d20_noraddifields[DS_NORADD_DV3][0] != D2_EOS)) ||
		    ((d20_noraddifields[DS_NORADD_DT3][0] != D2_EOS) &&
		     (d20_noraddifields[DS_NORADD_DV3][0] == D2_EOS)) ||
		    ((d20_noraddifields[DS_NORADD_DT4][0] == D2_EOS) &&
		     (d20_noraddifields[DS_NORADD_DV4][0] != D2_EOS)) ||
		    ((d20_noraddifields[DS_NORADD_DT4][0] != D2_EOS) &&
		     (d20_noraddifields[DS_NORADD_DV4][0] == D2_EOS)))
			{
			/* display error mask.		*/
			d20_qomask[D20_Q1POS].d20_ioinfo=
					    d20_errmess[D20_E51IDX];
			d20_handlemask(d20_qomask);

			return(FALSE);
			}

		return(TRUE);
		
	case D2_SORADD:
		if ((d20_soraddifields[DS_SORADD_CNTRY][0] == D2_EOS) ||
		    (d20_soraddifields[DS_SORADD_ADMD][0] == D2_EOS) ||
		    (d20_soraddifields[DS_SORADD_PCNTRY][0] == D2_EOS) ||
		    (d20_soraddifields[DS_SORADD_PCODE][0] == D2_EOS))
			{
			/* display error mask.		*/
			d20_qomask[D20_Q1POS].d20_ioinfo=
					    d20_errmess[D20_E50IDX];
			d20_handlemask(d20_qomask);

			return(FALSE);
			}

		return(TRUE);

	case D2_UORADD:
		if ((d20_uoraddifields[DS_UORADD_CNTRY][0] == D2_EOS) ||
		    (d20_uoraddifields[DS_UORADD_ADMD][0] == D2_EOS) ||
		    (d20_uoraddifields[DS_UORADD_PCNTRY][0] == D2_EOS) ||
		    (d20_uoraddifields[DS_UORADD_PCODE][0] == D2_EOS) ||
		    ((d20_uoraddifields[DS_UORADD_PAIF][0] == D2_EOS) &&
		     (d20_uoraddifields[DS_UORADD_PAIF + 1][0] == D2_EOS) &&
		     (d20_uoraddifields[DS_UORADD_PAIF + 2][0] == D2_EOS) &&
		     (d20_uoraddifields[DS_UORADD_PAIL1][0] == D2_EOS) &&
		     (d20_uoraddifields[DS_UORADD_PAIL2][0] == D2_EOS) &&
		     (d20_uoraddifields[DS_UORADD_PAIL3][0] == D2_EOS) &&
		     (d20_uoraddifields[DS_UORADD_PAIL4][0] == D2_EOS) &&
		     (d20_uoraddifields[DS_UORADD_PAIL5][0] == D2_EOS) &&
		     (d20_uoraddifields[DS_UORADD_PAIL6][0] == D2_EOS)))
			{
			/* display error mask.		*/
			d20_qomask[D20_Q1POS].d20_ioinfo=
					    d20_errmess[D20_E50IDX];
			d20_handlemask(d20_qomask);

			return(FALSE);
			}

		return(TRUE);

	case D2_TORADD:
		if ((d20_toraddifields[DS_TORADD_ISDNN][0] == D2_EOS) &&
		    (d20_toraddifields[DS_TORADD_ISDNS][0] == D2_EOS) &&
		    (*(signed32 *)d20_toraddifields[DS_TORADD_PA] == FALSE) &&
		    (d20_toraddifields[DS_TORADD_X121][0] == D2_EOS))
			{
			/* display error mask.		*/
			d20_qomask[D20_Q1POS].d20_ioinfo=
					    d20_errmess[D20_E52IDX];
			d20_handlemask(d20_qomask);

			return(FALSE);
			}

		if (((d20_toraddifields[DS_TORADD_DT1][0] == D2_EOS) &&
		     (d20_toraddifields[DS_TORADD_DV1][0] != D2_EOS)) ||
		    ((d20_toraddifields[DS_TORADD_DT1][0] != D2_EOS) &&
		     (d20_toraddifields[DS_TORADD_DV1][0] == D2_EOS)) ||
		    ((d20_toraddifields[DS_TORADD_DT2][0] == D2_EOS) &&
		     (d20_toraddifields[DS_TORADD_DV2][0] != D2_EOS)) ||
		    ((d20_toraddifields[DS_TORADD_DT2][0] != D2_EOS) &&
		     (d20_toraddifields[DS_TORADD_DV2][0] == D2_EOS)) ||
		    ((d20_toraddifields[DS_TORADD_DT3][0] == D2_EOS) &&
		     (d20_toraddifields[DS_TORADD_DV3][0] != D2_EOS)) ||
		    ((d20_toraddifields[DS_TORADD_DT3][0] != D2_EOS) &&
		     (d20_toraddifields[DS_TORADD_DV3][0] == D2_EOS)) ||
		    ((d20_toraddifields[DS_TORADD_DT4][0] == D2_EOS) &&
		     (d20_toraddifields[DS_TORADD_DV4][0] != D2_EOS)) ||
		    ((d20_toraddifields[DS_TORADD_DT4][0] != D2_EOS) &&
		     (d20_toraddifields[DS_TORADD_DV4][0] == D2_EOS)))
			{
			/* display error mask.		*/
			d20_qomask[D20_Q1POS].d20_ioinfo=
					    d20_errmess[D20_E51IDX];
			d20_handlemask(d20_qomask);

			return(FALSE);
			}

		return(TRUE);

	default:
		break;
	}
return(TRUE);
}



/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_cp_from_oradd()				      */
/*								      */
/* AUTHOR:      Sanjay Jain DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: d20_cp_from_oradd(oradd_type, oradd, prty)	      */
/*		signed32	oradd_type;			      */
/*		D2_OR_address	*oradd;			      	      */
/*		Ds_property	*prty;			      	      */
/*								      */
/*		This function copies information from OR address      */
/*		structure to the appropriate OR address mask, 	      */
/*		depending on oradd_type.			      */
/*		If presentation address is also present in OR address */
/*		structure, its value is returned via 'prty'.	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed32	oradd_type;			      */
/*		D2_OR_address	*oradd;			      	      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*		Ds_property	*prty;			      	      */
/*								      */
/* RETURNVALUE:         					      */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

static void d20_cp_from_oradd(	signed32 oradd_type,
				D2_OR_address *oradd,
				Ds_property *prty)
{
signed32	copy_len;

switch (oradd_type)
	{
	case D2_MORADD:
		d20_cp_str_list_to_field(oradd->d2_country_name,
		d20_moraddifields[DS_MORADD_CNTRY], D20_ORADD_CNT_FLD_LEN);

		d20_cp_str_list_to_field(oradd->d2_admd_name,
		d20_moraddifields[DS_MORADD_ADMD], D20_ORADD_ADMD_FLD_LEN);

		d20_cp_str_list_to_field(oradd->d2_prmd_name,
		d20_moraddifields[DS_MORADD_PRMD], D20_ORADD_PRMD_FLD_LEN);

		d20_cp_str_list_to_field(oradd->d2_org_name,
			d20_moraddifields[DS_MORADD_ORG], 
			D20_ORADD_ORG_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_org_unit_name1,
			d20_moraddifields[DS_MORADD_OU1], 
			D20_ORADD_OU1_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_org_unit_name2,
			d20_moraddifields[DS_MORADD_OU2], 
			D20_ORADD_OU2_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_org_unit_name3,
			d20_moraddifields[DS_MORADD_OU3], 
			D20_ORADD_OU3_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_org_unit_name4,
			d20_moraddifields[DS_MORADD_OU4], 
			D20_ORADD_OU4_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_common_name,
			d20_moraddifields[DS_MORADD_CN], 
			D20_ORADD_CN_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_given_name,
			d20_moraddifields[DS_MORADD_GN], 
			D20_ORADD_GN_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_initials,
			d20_moraddifields[DS_MORADD_INTLS], 
			D20_ORADD_INIT_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_surname,
			d20_moraddifields[DS_MORADD_SN], 
			D20_ORADD_SN_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_generation,
			d20_moraddifields[DS_MORADD_GEN], 
			D20_ORADD_GEN_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_type1,
			d20_moraddifields[DS_MORADD_DT1], 
			D20_ORADD_DT_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_type2,
			d20_moraddifields[DS_MORADD_DT2], 
			D20_ORADD_DT_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_type3,
			d20_moraddifields[DS_MORADD_DT3], 
			D20_ORADD_DT_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_type4,
			d20_moraddifields[DS_MORADD_DT4], 
			D20_ORADD_DT_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_value1,
			d20_moraddifields[DS_MORADD_DV1], 
			D20_ORADD_DV_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_value2,
			d20_moraddifields[DS_MORADD_DV2], 
			D20_ORADD_DV_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_value3,
			d20_moraddifields[DS_MORADD_DV3], 
			D20_ORADD_DV_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_value4,
			d20_moraddifields[DS_MORADD_DV4], 
			D20_ORADD_DV_FLD_LEN * D20_T61_CONV_FACT);

		break;

	case D2_NORADD:
		d20_cp_str_list_to_field(oradd->d2_country_name,
			d20_noraddifields[DS_NORADD_CNTRY], 
			D20_ORADD_CNT_FLD_LEN);
		d20_cp_str_list_to_field(oradd->d2_admd_name,
			d20_noraddifields[DS_NORADD_ADMD], 
			D20_ORADD_ADMD_FLD_LEN);
		if (oradd->d2_numeric_user_id != (Numeric_string)NULL)
			{
			strncpy(d20_noraddifields[DS_NORADD_NUI], 
		       		oradd->d2_numeric_user_id, 
				D20_ORADD_NUI_FLD_LEN);
			d20_noraddifields[DS_NORADD_NUI][D20_ORADD_NUI_FLD_LEN]
								= D2_EOS;
			}

		d20_cp_str_list_to_field(oradd->d2_prmd_name,
			d20_noraddifields[DS_NORADD_PRMD], 
			D20_ORADD_PRMD_FLD_LEN);

		d20_cp_str_list_to_field(oradd->d2_domain_type1,
			d20_noraddifields[DS_NORADD_DT1], 
			D20_ORADD_DT_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_type2,
			d20_noraddifields[DS_NORADD_DT2], 
			D20_ORADD_DT_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_type3,
			d20_noraddifields[DS_NORADD_DT3], 
			D20_ORADD_DT_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_type4,
			d20_noraddifields[DS_NORADD_DT4], 
			D20_ORADD_DT_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_value1,
			d20_noraddifields[DS_NORADD_DV1], 
			D20_ORADD_DV_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_value2,
			d20_noraddifields[DS_NORADD_DV2], 
			D20_ORADD_DV_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_value3,
			d20_noraddifields[DS_NORADD_DV3], 
			D20_ORADD_DV_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_value4,
			d20_noraddifields[DS_NORADD_DV4], 
			D20_ORADD_DV_FLD_LEN * D20_T61_CONV_FACT);

		break;

	case D2_SORADD:
		d20_cp_str_list_to_field(oradd->d2_country_name,
			d20_soraddifields[DS_SORADD_CNTRY], 
			D20_ORADD_CNT_FLD_LEN);
		d20_cp_str_list_to_field(oradd->d2_admd_name,
			d20_soraddifields[DS_SORADD_ADMD], 
			D20_ORADD_ADMD_FLD_LEN);

		d20_cp_str_list_to_field(oradd->d2_prmd_name,
			d20_soraddifields[DS_SORADD_PRMD], 
			D20_ORADD_PRMD_FLD_LEN);

		d20_cp_str_list_to_field(oradd->d2_post_country_name,
			d20_soraddifields[DS_SORADD_PCNTRY], 
			D20_ORADD_PCNT_FLD_LEN);

		d20_cp_str_list_to_field(oradd->d2_post_code,
			d20_soraddifields[DS_SORADD_PCODE], 
			D20_ORADD_PC_FLD_LEN);

		d20_cp_str_list_to_field(oradd->d2_post_addr_details,
			d20_soraddifields[DS_SORADD_PAD], 
			D20_ORADD_PAD_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_post_deliv_pt_name,
			d20_soraddifields[DS_SORADD_PDPN], 
			D20_ORADD_PDPN_FLD_LEN * D20_T61_CONV_FACT);

		if (oradd->d2_post_deliv_sys_name != (Print_string)NULL)
			{
			strncpy(d20_soraddifields[DS_SORADD_PDSN],
			        oradd->d2_post_deliv_sys_name, 
				D20_ORADD_PDSN_FLD_LEN * D20_T61_CONV_FACT);
			d20_soraddifields[DS_SORADD_PDSN]
			[D20_ORADD_PDSN_FLD_LEN * D20_T61_CONV_FACT] = D2_EOS;
			}

		d20_cp_str_list_to_field(oradd->d2_post_gen_deliv_addr,
			d20_soraddifields[DS_SORADD_PGDA], 
			D20_ORADD_PGDA_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_post_locale,
			d20_soraddifields[DS_SORADD_PL], 
			D20_ORADD_PL_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_post_office_box_nbr,
			d20_soraddifields[DS_SORADD_POBNR], 
			D20_ORADD_POFBN_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_post_office_name,
			d20_soraddifields[DS_SORADD_PON], 
			D20_ORADD_POFNA_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_post_office_nbr,
			d20_soraddifields[DS_SORADD_PONR], 
			D20_ORADD_POFNU_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_post_org_name,
			d20_soraddifields[DS_SORADD_PORGN], 
			D20_ORADD_PORGNA_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_post_patron_details,
			d20_soraddifields[DS_SORADD_PPD], 
			D20_ORADD_PPD_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_post_patron_name,
			d20_soraddifields[DS_SORADD_PPN], 
			D20_ORADD_PPN_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_post_street_addr,
			d20_soraddifields[DS_SORADD_PSA], 
			D20_ORADD_PSA_FLD_LEN * D20_T61_CONV_FACT);

		break;

	case D2_UORADD:
		{
		D2_str_list *str_list_ptr;
		signed32     pail_index;

		d20_cp_str_list_to_field(oradd->d2_country_name,
			d20_uoraddifields[DS_UORADD_CNTRY], 
			D20_ORADD_CNT_FLD_LEN);
		d20_cp_str_list_to_field(oradd->d2_admd_name,
			d20_uoraddifields[DS_UORADD_ADMD], 
			D20_ORADD_ADMD_FLD_LEN);
		d20_cp_str_list_to_field(oradd->d2_prmd_name,
			d20_uoraddifields[DS_UORADD_PRMD], 
			D20_ORADD_PRMD_FLD_LEN);

		d20_cp_str_list_to_field(oradd->d2_post_country_name,
			d20_uoraddifields[DS_UORADD_PCNTRY], 
			D20_ORADD_PCNT_FLD_LEN);
		d20_cp_str_list_to_field(oradd->d2_post_code,
			d20_uoraddifields[DS_UORADD_PCODE], 
			D20_ORADD_PC_FLD_LEN);
		if (oradd->d2_post_addr_in_full != (T61_string)NULL)
			{
			strncpy(d20_uoraddifields[DS_UORADD_PAIF], 
		       		(char *)oradd->d2_post_addr_in_full,
				D20_ORADD_PAIF1_FLD_LEN * D20_T61_CONV_FACT);

			d20_uoraddifields[DS_UORADD_PAIF]
			[D20_ORADD_PAIF1_FLD_LEN * D20_T61_CONV_FACT] = D2_EOS;

			if ((int)strlen((char *)oradd->d2_post_addr_in_full) >
				(D20_ORADD_PAIF1_FLD_LEN * D20_T61_CONV_FACT))
				{
				strncpy(d20_uoraddifields[DS_UORADD_PAIF + 1], 
				  (char *)oradd->d2_post_addr_in_full +
				  (D20_ORADD_PAIF1_FLD_LEN * D20_T61_CONV_FACT),
				   D20_ORADD_PAIF2_FLD_LEN * D20_T61_CONV_FACT);

				d20_uoraddifields[DS_UORADD_PAIF + 1]
				[D20_ORADD_PAIF2_FLD_LEN * D20_T61_CONV_FACT] =
									D2_EOS;
				}

			if ((int)strlen((char *)oradd->d2_post_addr_in_full) > 
				(D20_ORADD_PAIF1_FLD_LEN  * D20_T61_CONV_FACT) +
 				(D20_ORADD_PAIF2_FLD_LEN * D20_T61_CONV_FACT))
				{
				strncpy(d20_uoraddifields[DS_UORADD_PAIF + 2], 
				(char *)oradd->d2_post_addr_in_full +
				(D20_ORADD_PAIF1_FLD_LEN  * D20_T61_CONV_FACT)
				+ (D20_ORADD_PAIF2_FLD_LEN * D20_T61_CONV_FACT),
				D20_ORADD_PAIF3_FLD_LEN * D20_T61_CONV_FACT);

				d20_uoraddifields[DS_UORADD_PAIF + 2]
				[D20_ORADD_PAIF3_FLD_LEN * D20_T61_CONV_FACT] =
									D2_EOS;
				}
			}

		str_list_ptr = oradd->d2_post_addr_in_lines;
		pail_index = DS_UORADD_PAIL1;

		while (str_list_ptr != (D2_str_list *)NULL)
			{
			copy_len = str_list_ptr->d2_str.d2_size <= 
			(D20_ORADD_PAIL_FLD_LEN * D20_T61_CONV_FACT) ? 
			str_list_ptr->d2_str.d2_size :
			(D20_ORADD_PAIL_FLD_LEN * D20_T61_CONV_FACT);

			memcpy(d20_uoraddifields[pail_index], 
			str_list_ptr->d2_str.d2_value, copy_len);

			d20_uoraddifields[pail_index++][copy_len] = D2_EOS;

			str_list_ptr = str_list_ptr->d2_next;
			}

		if (oradd->d2_post_deliv_sys_name != (Print_string)NULL)
			{
			strncpy(d20_uoraddifields[DS_UORADD_PDSN],
			oradd->d2_post_deliv_sys_name,
			D20_ORADD_PDSN_FLD_LEN * D20_T61_CONV_FACT);

			d20_uoraddifields[DS_UORADD_PDSN]
			[D20_ORADD_PDSN_FLD_LEN * D20_T61_CONV_FACT] = D2_EOS;
			}

		break;
		}

	case D2_TORADD:
		if (oradd->d2_isdn_number != (Numeric_string)NULL)
			{
			strncpy(d20_toraddifields[DS_TORADD_ISDNN],
				oradd->d2_isdn_number, D20_ORADD_ISNO_FLD_LEN);

			d20_toraddifields[DS_TORADD_ISDNN]
			[D20_ORADD_ISNO_FLD_LEN] = D2_EOS;
			}

		if (oradd->d2_isdn_subaddress != (Numeric_string)NULL)
			{
			strncpy(d20_toraddifields[DS_TORADD_ISDNS],
			oradd->d2_isdn_subaddress, D20_ORADD_SUA_FLD_LEN);

			d20_toraddifields[DS_TORADD_ISDNS]
			[D20_ORADD_SUA_FLD_LEN] = D2_EOS;
			}

		if (oradd->d2_presentation_addr != (Psap_string)NULL)
			{
			*(signed32 *)d20_toraddifields[DS_TORADD_PA] = 1;
			prty->ds_value = oradd->d2_presentation_addr;
			prty->ds_length = 0;
			}

		if (oradd->d2_x121_addr != (Numeric_string)NULL)
			{
			strncpy(d20_toraddifields[DS_TORADD_X121],
				oradd->d2_x121_addr, D20_ORADD_X121_FLD_LEN);

			d20_toraddifields[DS_TORADD_X121]
			[D20_ORADD_X121_FLD_LEN] = D2_EOS;
			}

		d20_cp_str_list_to_field(oradd->d2_country_name,
			d20_toraddifields[DS_TORADD_CNTRY], 
			D20_ORADD_CNT_FLD_LEN);

		d20_cp_str_list_to_field(oradd->d2_admd_name,
			d20_toraddifields[DS_TORADD_ADMD], 
			D20_ORADD_ADMD_FLD_LEN);

		d20_cp_str_list_to_field(oradd->d2_prmd_name,
			d20_toraddifields[DS_TORADD_PRMD], 
			D20_ORADD_PRMD_FLD_LEN);

		if (oradd->d2_terminal_id != (Print_string)NULL)
			{
			strncpy(d20_toraddifields[DS_TORADD_TI],
			       oradd->d2_terminal_id, D20_ORADD_TI_FLD_LEN);

			d20_toraddifields[DS_TORADD_TI][D20_ORADD_TI_FLD_LEN] = 
									D2_EOS;
			}

		*(signed32 *)d20_toraddifields[DS_TORADD_TT] = 
			oradd->d2_terminal_type;

		d20_cp_str_list_to_field(oradd->d2_domain_type1,
			d20_toraddifields[DS_TORADD_DT1], 
			D20_ORADD_DT_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_type2,
			d20_toraddifields[DS_TORADD_DT2], 
			D20_ORADD_DT_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_type3,
			d20_toraddifields[DS_TORADD_DT3], 
			D20_ORADD_DT_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_type4,
			d20_toraddifields[DS_TORADD_DT4], 
			D20_ORADD_DT_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_value1,
			d20_toraddifields[DS_TORADD_DV1], 
			D20_ORADD_DV_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_value2,
			d20_toraddifields[DS_TORADD_DV2], 
			D20_ORADD_DV_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_value3,
			d20_toraddifields[DS_TORADD_DV3], 
			D20_ORADD_DV_FLD_LEN * D20_T61_CONV_FACT);

		d20_cp_str_list_to_field(oradd->d2_domain_value4,
			d20_toraddifields[DS_TORADD_DV4], 
			D20_ORADD_DV_FLD_LEN * D20_T61_CONV_FACT);

		break;

	default:
		break;
	}
}



/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_cp_str_list_to_field()			      */
/*								      */
/* AUTHOR:      Sanjay Jain DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: void d20_cp_str_list_to_field(str_list, field, 	      */
/*					      fld_space)	      */
/*		D2_str_list	*str_list;			      */
/*		char		field[];			      */
/*		unsigned16	fld_space;			      */
/*								      */
/*		This function copies information from D2_str_list     */
/*		type of structure to a filed.			      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		D2_str_list	*str_list;			      */
/*		char		field[];			      */
/*		unsigned16	fld_space;	space available for   */
/*						the field.	      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*		char		field[];			      */
/*								      */
/* RETURNVALUE:         					      */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

static void d20_cp_str_list_to_field(D2_str_list *str_list, char field[],
				     unsigned16 fld_space)
{
D2_str		source_str;
signed32	copy_len;

if (str_list != (D2_str_list *)NULL)
	{
	if ((str_list->d2_rep == D2_T61) ||
			(str_list->d2_next == (D2_str_list *)NULL))
		source_str = str_list->d2_str;
	else
		source_str = str_list->d2_next->d2_str;

	copy_len = (source_str.d2_size <= (int)(fld_space)) ?
						    source_str.d2_size : 
						    fld_space;
	 
	memcpy(field, source_str.d2_value, copy_len);
	field[copy_len] = D2_EOS;
	}

}


/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_cp_to_oradd()				      */
/*								      */
/* AUTHOR:      Sanjay Jain DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: d20_cp_to_oradd(oradd_type, oradd, prty)	      */
/*		signed32	oradd_type;			      */
/*		D2_OR_address	*oradd;			      	      */
/*		Ds_property	*prty;			      	      */
/*								      */
/*		This function generates the OR address struct from the*/
/*		mask information.				      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed32	oradd_type;			      */
/*		D2_OR_address	*oradd;			      	      */
/*		Ds_property	*prty;			      	      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:         					      */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

static  void d20_cp_to_oradd(signed32 oradd_type, D2_OR_address *oradd,
			    Ds_property *prty)
{ 
static	char	post_addr_in_full[D20_ORADD_PAIF1_FLD_LEN
				+ D20_ORADD_PAIF2_FLD_LEN
				+ D20_ORADD_PAIF3_FLD_LEN + 1];

switch (oradd_type)
	{
	case D2_MORADD:
		/* following two components are mandatory.	*/ 
		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_CNTRY], 
		&(oradd->d2_country_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_ADMD], 
		&(oradd->d2_admd_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_PRMD], 
		&(oradd->d2_prmd_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_ORG], 
		&(oradd->d2_org_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_OU1], 
		&(oradd->d2_org_unit_name1), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_OU2], 
		&(oradd->d2_org_unit_name2), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_OU3], 
		&(oradd->d2_org_unit_name3), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_OU4], 
		&(oradd->d2_org_unit_name4), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_CN], 
		&(oradd->d2_common_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_GN], 
		&(oradd->d2_given_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_INTLS], 
		&(oradd->d2_initials), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_SN], 
		&(oradd->d2_surname), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_GEN], 
		&(oradd->d2_generation), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_DT1], 
		&(oradd->d2_domain_type1), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_DV1], 
		&(oradd->d2_domain_value1), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_DT2], 
		&(oradd->d2_domain_type2), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_DV2], 
		&(oradd->d2_domain_value2), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_DT3], 
		&(oradd->d2_domain_type3), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_DV3], 
		&(oradd->d2_domain_value3), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_DT4], 
		&(oradd->d2_domain_type4), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_moraddifields[DS_MORADD_DV4], 
		&(oradd->d2_domain_value4), D2_PRINTABLE);

		oradd->d2_isdn_number = (Numeric_string)NULL;
		oradd->d2_isdn_subaddress = (Numeric_string)NULL;
		oradd->d2_numeric_user_id = (Numeric_string)NULL;
		oradd->d2_post_addr_details = (D2_str_list *)NULL;
		oradd->d2_post_addr_in_full = (T61_string)NULL;
		oradd->d2_post_code = (D2_str_list *)NULL;
		oradd->d2_post_country_name = (D2_str_list *)NULL;
		oradd->d2_post_deliv_pt_name = (D2_str_list *)NULL;
		oradd->d2_post_deliv_sys_name = (Print_string)NULL;
		oradd->d2_post_gen_deliv_addr = (D2_str_list *)NULL;
		oradd->d2_post_locale = (D2_str_list *)NULL;
		oradd->d2_post_office_box_nbr = (D2_str_list *)NULL;
		oradd->d2_post_office_name = (D2_str_list *)NULL;
		oradd->d2_post_office_nbr = (D2_str_list *)NULL;
		oradd->d2_post_org_name = (D2_str_list *)NULL;
		oradd->d2_post_patron_details = (D2_str_list *)NULL;
		oradd->d2_post_patron_name = (D2_str_list *)NULL;
		oradd->d2_post_street_addr = (D2_str_list *)NULL;
		oradd->d2_presentation_addr = (Psap_string)NULL;
		oradd->d2_terminal_id = (Print_string)NULL;
		oradd->d2_terminal_type = 0;
		oradd->d2_x121_addr = (Numeric_string)NULL;
		oradd->d2_post_addr_in_lines = (D2_str_list *)NULL;

		break;

	case D2_NORADD:
		/* following three components are mandatory.	*/ 
		d20_cp_field_to_str_list(d20_noraddifields[DS_NORADD_CNTRY], 
		&(oradd->d2_country_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_noraddifields[DS_NORADD_ADMD], 
		&(oradd->d2_admd_name), D2_PRINTABLE);

		strcpy(oradd->d2_numeric_user_id, 
				d20_noraddifields[DS_NORADD_NUI]);

		d20_cp_field_to_str_list(d20_noraddifields[DS_NORADD_PRMD], 
		&(oradd->d2_prmd_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_noraddifields[DS_NORADD_DT1], 
		&(oradd->d2_domain_type1), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_noraddifields[DS_NORADD_DV1], 
		&(oradd->d2_domain_value1), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_noraddifields[DS_NORADD_DT2], 
		&(oradd->d2_domain_type2), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_noraddifields[DS_NORADD_DV2], 
		&(oradd->d2_domain_value2), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_noraddifields[DS_NORADD_DT3], 
		&(oradd->d2_domain_type3), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_noraddifields[DS_NORADD_DV3], 
		&(oradd->d2_domain_value3), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_noraddifields[DS_NORADD_DT4], 
		&(oradd->d2_domain_type4), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_noraddifields[DS_NORADD_DV4], 
		&(oradd->d2_domain_value4), D2_PRINTABLE);

		oradd->d2_common_name = (D2_str_list *)NULL;
		oradd->d2_generation = (D2_str_list *)NULL;
		oradd->d2_given_name = (D2_str_list *)NULL;
		oradd->d2_initials = (D2_str_list *)NULL;
		oradd->d2_isdn_number = (Numeric_string)NULL;
		oradd->d2_isdn_subaddress = (Numeric_string)NULL;
		oradd->d2_org_name = (D2_str_list *)NULL;
		oradd->d2_org_unit_name1 = (D2_str_list *)NULL;
		oradd->d2_org_unit_name2 = (D2_str_list *)NULL;
		oradd->d2_org_unit_name3 = (D2_str_list *)NULL;
		oradd->d2_org_unit_name4 = (D2_str_list *)NULL;
		oradd->d2_post_addr_details = (D2_str_list *)NULL;
		oradd->d2_post_addr_in_full = (T61_string)NULL;
		oradd->d2_post_code = (D2_str_list *)NULL;
		oradd->d2_post_country_name = (D2_str_list *)NULL;
		oradd->d2_post_deliv_pt_name = (D2_str_list *)NULL;
		oradd->d2_post_deliv_sys_name = (Print_string)NULL;
		oradd->d2_post_gen_deliv_addr = (D2_str_list *)NULL;
		oradd->d2_post_locale = (D2_str_list *)NULL;
		oradd->d2_post_office_box_nbr = (D2_str_list *)NULL;
		oradd->d2_post_office_name = (D2_str_list *)NULL;
		oradd->d2_post_office_nbr = (D2_str_list *)NULL;
		oradd->d2_post_org_name = (D2_str_list *)NULL;
		oradd->d2_post_patron_details = (D2_str_list *)NULL;
		oradd->d2_post_patron_name = (D2_str_list *)NULL;
		oradd->d2_post_street_addr = (D2_str_list *)NULL;
		oradd->d2_presentation_addr = (Psap_string)NULL;
		oradd->d2_surname   = (D2_str_list *)NULL;
		oradd->d2_terminal_id = (Print_string)NULL;
		oradd->d2_terminal_type = 0;
		oradd->d2_x121_addr = (Numeric_string)NULL;
		oradd->d2_post_addr_in_lines = (D2_str_list *)NULL;

		break;

	case D2_SORADD:
		/* following four components are mandatory.	*/ 
		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_CNTRY], 
		&(oradd->d2_country_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_ADMD], 
		&(oradd->d2_admd_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_PCNTRY], 
		&(oradd->d2_post_country_name),D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_PCODE], 
		&(oradd->d2_post_code), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_PRMD], 
		&(oradd->d2_prmd_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_PAD], 
		&(oradd->d2_post_addr_details), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_PDPN], 
		&(oradd->d2_post_deliv_pt_name), D2_PRINTABLE);

		if (d20_soraddifields[DS_SORADD_PDSN][0] != D2_EOS)
			strcpy(oradd->d2_post_deliv_sys_name, 
				d20_soraddifields[DS_SORADD_PDSN]);
		else
			oradd->d2_post_deliv_sys_name = (Print_string)NULL;
			

		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_PGDA], 
		&(oradd->d2_post_gen_deliv_addr),D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_PL], 
		&(oradd->d2_post_locale), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_POBNR],
		&(oradd->d2_post_office_box_nbr),D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_PON], 
		&(oradd->d2_post_office_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_PONR], 
		&(oradd->d2_post_office_nbr), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_PORGN], 
		&(oradd->d2_post_org_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_PPD], 
		&(oradd->d2_post_patron_details), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_PPN], 
		&(oradd->d2_post_patron_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_soraddifields[DS_SORADD_PSA], 
		&(oradd->d2_post_street_addr), D2_PRINTABLE);

		oradd->d2_common_name = (D2_str_list *)NULL;
		oradd->d2_domain_type1 = (D2_str_list *)NULL;
		oradd->d2_domain_type2 = (D2_str_list *)NULL;
		oradd->d2_domain_type3 = (D2_str_list *)NULL;
		oradd->d2_domain_type4 = (D2_str_list *)NULL;
		oradd->d2_domain_value1 = (D2_str_list *)NULL;
		oradd->d2_domain_value2 = (D2_str_list *)NULL;
		oradd->d2_domain_value3 = (D2_str_list *)NULL;
		oradd->d2_domain_value4 = (D2_str_list *)NULL;
		oradd->d2_generation = (D2_str_list *)NULL;
		oradd->d2_given_name = (D2_str_list *)NULL;
		oradd->d2_initials = (D2_str_list *)NULL;
		oradd->d2_isdn_number = (Numeric_string)NULL;
		oradd->d2_isdn_subaddress = (Numeric_string)NULL;
		oradd->d2_numeric_user_id = (Numeric_string)NULL;
		oradd->d2_org_name = (D2_str_list *)NULL;
		oradd->d2_org_unit_name1 = (D2_str_list *)NULL;
		oradd->d2_org_unit_name2 = (D2_str_list *)NULL;
		oradd->d2_org_unit_name3 = (D2_str_list *)NULL;
		oradd->d2_org_unit_name4 = (D2_str_list *)NULL;
		oradd->d2_post_addr_in_full = (T61_string)NULL;
		oradd->d2_presentation_addr = (Psap_string)NULL;
		oradd->d2_surname   = (D2_str_list *)NULL;
		oradd->d2_terminal_id = (Print_string)NULL;
		oradd->d2_terminal_type = 0;
		oradd->d2_x121_addr = (Numeric_string)NULL;
		oradd->d2_post_addr_in_lines = (D2_str_list *)NULL;

		break;

	case D2_UORADD:
		{
		/* following four components are mandatory.	*/ 
		d20_cp_field_to_str_list(d20_uoraddifields[DS_UORADD_CNTRY], 
		&(oradd->d2_country_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_uoraddifields[DS_UORADD_ADMD], 
		&(oradd->d2_admd_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_uoraddifields[DS_UORADD_PCNTRY], 
		&(oradd->d2_post_country_name),D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_uoraddifields[DS_UORADD_PCODE], 
		&(oradd->d2_post_code), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_uoraddifields[DS_UORADD_PRMD], 
		&(oradd->d2_prmd_name), D2_PRINTABLE);

		if ((d20_uoraddifields[DS_UORADD_PAIF][0] != D2_EOS) ||
		    (d20_uoraddifields[DS_UORADD_PAIF + 1][0] != D2_EOS) ||
		    (d20_uoraddifields[DS_UORADD_PAIF + 2][0] != D2_EOS))
			{
			strcpy(post_addr_in_full,
					d20_uoraddifields[DS_UORADD_PAIF]);
			strcat(post_addr_in_full,
					d20_uoraddifields[DS_UORADD_PAIF + 1]);
			strcat(post_addr_in_full,
					d20_uoraddifields[DS_UORADD_PAIF + 2]);
			strcpy((char *)oradd->d2_post_addr_in_full,
							post_addr_in_full);
			}
		else
			oradd->d2_post_addr_in_full = (T61_string)NULL;

		if ((d20_uoraddifields[DS_UORADD_PAIL1][0] != D2_EOS) ||
		    (d20_uoraddifields[DS_UORADD_PAIL2][0] != D2_EOS) ||
		    (d20_uoraddifields[DS_UORADD_PAIL3][0] != D2_EOS) ||
		    (d20_uoraddifields[DS_UORADD_PAIL4][0] != D2_EOS) ||
		    (d20_uoraddifields[DS_UORADD_PAIL5][0] != D2_EOS) ||
		    (d20_uoraddifields[DS_UORADD_PAIL6][0] != D2_EOS))
			{	
			D2_str_list *str_list_ptr;
			D2_str_list *prev_str_list_ptr;
			signed32     pail_index;

			for (pail_index = 0,
			     str_list_ptr = oradd->d2_post_addr_in_lines;
			     pail_index < 6; 
			     pail_index++)
				if (d20_uoraddifields[DS_UORADD_PAIL1 + 
						pail_index][0] != D2_EOS)
					{
					str_list_ptr->d2_rep  = D2_PRINTABLE;

					str_list_ptr->d2_str.d2_size = 
					strlen(
					d20_uoraddifields[DS_UORADD_PAIL1 + 
					pail_index]);

					memcpy(str_list_ptr->d2_str.d2_value, 
					d20_uoraddifields[DS_UORADD_PAIL1 + 
					pail_index],
					strlen(
					d20_uoraddifields[DS_UORADD_PAIL1 + 
					pail_index]));

					prev_str_list_ptr = str_list_ptr;
			     		str_list_ptr = str_list_ptr->d2_next;
					}

			prev_str_list_ptr->d2_next = (D2_str_list *)NULL;

			}
		else
			oradd->d2_post_addr_in_lines = (D2_str_list *)NULL;


		if (d20_uoraddifields[DS_UORADD_PDSN][0] != D2_EOS)
			strcpy(oradd->d2_post_deliv_sys_name,
				d20_uoraddifields[DS_UORADD_PDSN]);
		else
			oradd->d2_post_deliv_sys_name = (Print_string)NULL;

		oradd->d2_common_name = (D2_str_list *)NULL;
		oradd->d2_domain_type1 = (D2_str_list *)NULL;
		oradd->d2_domain_type2 = (D2_str_list *)NULL;
		oradd->d2_domain_type3 = (D2_str_list *)NULL;
		oradd->d2_domain_type4 = (D2_str_list *)NULL;
		oradd->d2_domain_value1 = (D2_str_list *)NULL;
		oradd->d2_domain_value2 = (D2_str_list *)NULL;
		oradd->d2_domain_value3 = (D2_str_list *)NULL;
		oradd->d2_domain_value4 = (D2_str_list *)NULL;
		oradd->d2_generation = (D2_str_list *)NULL;
		oradd->d2_given_name = (D2_str_list *)NULL;
		oradd->d2_initials = (D2_str_list *)NULL;
		oradd->d2_isdn_number = (Numeric_string)NULL;
		oradd->d2_isdn_subaddress = (Numeric_string)NULL;
		oradd->d2_numeric_user_id = (Numeric_string)NULL;
		oradd->d2_org_name = (D2_str_list *)NULL;
		oradd->d2_org_unit_name1 = (D2_str_list *)NULL;
		oradd->d2_org_unit_name2 = (D2_str_list *)NULL;
		oradd->d2_org_unit_name3 = (D2_str_list *)NULL;
		oradd->d2_org_unit_name4 = (D2_str_list *)NULL;
		oradd->d2_post_addr_details = (D2_str_list *)NULL;
		oradd->d2_post_deliv_pt_name = (D2_str_list *)NULL;
		oradd->d2_post_gen_deliv_addr = (D2_str_list *)NULL;
		oradd->d2_post_locale = (D2_str_list *)NULL;
		oradd->d2_post_office_box_nbr = (D2_str_list *)NULL;
		oradd->d2_post_office_name = (D2_str_list *)NULL;
		oradd->d2_post_office_nbr = (D2_str_list *)NULL;
		oradd->d2_post_org_name = (D2_str_list *)NULL;
		oradd->d2_post_patron_details = (D2_str_list *)NULL;
		oradd->d2_post_patron_name = (D2_str_list *)NULL;
		oradd->d2_post_street_addr = (D2_str_list *)NULL;
		oradd->d2_presentation_addr = (Psap_string)NULL;
		oradd->d2_surname   = (D2_str_list *)NULL;
		oradd->d2_terminal_id = (Print_string)NULL;
		oradd->d2_terminal_type = 0;
		oradd->d2_x121_addr = (Numeric_string)NULL;

		break;
		}

	case D2_TORADD:
		if (d20_toraddifields[DS_TORADD_ISDNN][0] != D2_EOS)
			strcpy(oradd->d2_isdn_number,
				d20_toraddifields[DS_TORADD_ISDNN]);
		else
			oradd->d2_isdn_number = (Numeric_string)NULL;

		if (d20_toraddifields[DS_TORADD_ISDNS][0] != D2_EOS)
			strcpy(oradd->d2_isdn_subaddress,
				d20_toraddifields[DS_TORADD_ISDNS]);
		else
			oradd->d2_isdn_subaddress = (Numeric_string)NULL;

		if (*(signed32 *)d20_toraddifields[DS_TORADD_PA] != FALSE)
			memcpy(oradd->d2_presentation_addr, prty->ds_value,
							    prty->ds_length);
		else
			oradd->d2_presentation_addr = (Psap_string)NULL;

		if (d20_toraddifields[DS_TORADD_X121][0] != D2_EOS)
			strcpy(oradd->d2_x121_addr,
				d20_toraddifields[DS_TORADD_X121]);
		else
			oradd->d2_x121_addr = (Numeric_string)NULL;
			

		d20_cp_field_to_str_list(d20_toraddifields[DS_TORADD_CNTRY], 
		&(oradd->d2_country_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_toraddifields[DS_TORADD_ADMD], 
		&(oradd->d2_admd_name), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_toraddifields[DS_TORADD_PRMD], 
		&(oradd->d2_prmd_name), D2_PRINTABLE);

		if (d20_toraddifields[DS_TORADD_TI][0] != D2_EOS)
			strcpy(oradd->d2_terminal_id, 
					d20_toraddifields[DS_TORADD_TI]);
		else
			oradd->d2_terminal_id = (Print_string)NULL;

		oradd->d2_terminal_type = 
			*(signed32 *)d20_toraddifields[DS_TORADD_TT];

		d20_cp_field_to_str_list(d20_toraddifields[DS_TORADD_DT1], 
		&(oradd->d2_domain_type1), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_toraddifields[DS_TORADD_DV1], 
		&(oradd->d2_domain_value1), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_toraddifields[DS_TORADD_DT2], 
		&(oradd->d2_domain_type2), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_toraddifields[DS_TORADD_DV2], 
		&(oradd->d2_domain_value2), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_toraddifields[DS_TORADD_DT3], 
		&(oradd->d2_domain_type3), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_toraddifields[DS_TORADD_DV3], 
		&(oradd->d2_domain_value3), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_toraddifields[DS_TORADD_DT4], 
		&(oradd->d2_domain_type4), D2_PRINTABLE);

		d20_cp_field_to_str_list(d20_toraddifields[DS_TORADD_DV4], 
		&(oradd->d2_domain_value4), D2_PRINTABLE);

		oradd->d2_common_name = (D2_str_list *)NULL;
		oradd->d2_generation = (D2_str_list *)NULL;
		oradd->d2_given_name = (D2_str_list *)NULL;
		oradd->d2_initials = (D2_str_list *)NULL;
		oradd->d2_numeric_user_id = (Numeric_string)NULL;
		oradd->d2_org_name = (D2_str_list *)NULL;
		oradd->d2_org_unit_name1 = (D2_str_list *)NULL;
		oradd->d2_org_unit_name2 = (D2_str_list *)NULL;
		oradd->d2_org_unit_name3 = (D2_str_list *)NULL;
		oradd->d2_org_unit_name4 = (D2_str_list *)NULL;
		oradd->d2_post_addr_details = (D2_str_list *)NULL;
		oradd->d2_post_addr_in_full = (T61_string)NULL;
		oradd->d2_post_code = (D2_str_list *)NULL;
		oradd->d2_post_country_name = (D2_str_list *)NULL;
		oradd->d2_post_deliv_pt_name = (D2_str_list *)NULL;
		oradd->d2_post_deliv_sys_name = (Print_string)NULL;
		oradd->d2_post_gen_deliv_addr = (D2_str_list *)NULL;
		oradd->d2_post_locale = (D2_str_list *)NULL;
		oradd->d2_post_office_box_nbr = (D2_str_list *)NULL;
		oradd->d2_post_office_name = (D2_str_list *)NULL;
		oradd->d2_post_office_nbr = (D2_str_list *)NULL;
		oradd->d2_post_org_name = (D2_str_list *)NULL;
		oradd->d2_post_patron_details = (D2_str_list *)NULL;
		oradd->d2_post_patron_name = (D2_str_list *)NULL;
		oradd->d2_post_street_addr = (D2_str_list *)NULL;
		oradd->d2_surname   = (D2_str_list *)NULL;
		oradd->d2_post_addr_in_lines = (D2_str_list *)NULL;

		break;

	default:
		break;
	}
}


/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_cp_field_to_str_list()			      */
/*								      */
/* AUTHOR:      Sanjay Jain DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: void d20_cp_field_to_str_list(field, str_list, rep)   */
/*		char		field[];			      */
/*		D2_str_list	**str_list;			      */
/*		usigned16	rep;				      */
/*								      */
/*		This function copies information from a mask field to */
/*		D2_str_list type of structure.			      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		char		field[];			      */
/*		D2_str_list	**str_list;			      */
/*		unsigned16	rep;				      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*		D2_str_list	**str_list;			      */
/*								      */
/* RETURNVALUE:         					      */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

static void d20_cp_field_to_str_list(char field[],
				    D2_str_list **str_list, unsigned16 rep)
{
    if (field[0] != D2_EOS)
	{
	(*str_list)->d2_next = (D2_str_list *)NULL;
	(*str_list)->d2_rep  = rep;
	(*str_list)->d2_str.d2_size = strlen(field);
	memcpy((*str_list)->d2_str.d2_value, field, strlen(field));
	}
    else
	*str_list = (D2_str_list *)NULL;

}


/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_get_oradd_type()				      */
/*								      */
/* AUTHOR:      Sanjay Jain DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: signed32 d20_get_oradd_type(oradd) 		      */
/*		D2_OR_addres	*oradd;				      */
/*								      */
/*		This function analyses the oradd and determines the   */
/*		type of the OR address involved.		      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		D2_OR_address	*oradd;				      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:         					      */
/*		D2_MORADD 	if type is mnemonic.		      */
/*		D2_NORADD 	if type is numeric.		      */
/*		D2_SORADD 	if type is structured postal.	      */
/*		D2_UORADD 	if type is unstructured postal.       */
/*		D2_TORADD 	if type is terminal.		      */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

static signed32 d20_get_oradd_type(D2_OR_address *oradd)
{
if ((oradd->d2_post_addr_in_full != (T61_string)NULL) ||
    (oradd->d2_post_addr_in_lines != (D2_str_list *)NULL))
	return(D2_UORADD);

if (oradd->d2_post_code != (D2_str_list *)NULL)
	return(D2_SORADD);

if (oradd->d2_numeric_user_id != (Numeric_string)NULL)
	return(D2_NORADD);

if ((oradd->d2_isdn_number != (Numeric_string)NULL) ||
    (oradd->d2_isdn_subaddress != (Numeric_string)NULL) ||
    (oradd->d2_presentation_addr != (Psap_string)NULL) ||
    (oradd->d2_x121_addr != (Numeric_string)NULL))
	return(D2_TORADD);

return(D2_MORADD);
}


/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_orname()                                          */
/*								      */
/* AUTHOR:      Sanjay Jain DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: signed32 d20_orname(object-counter, op, attr_name,    */
/*				orname,	new_orname, mod_type)	      */
/*		signed32   object_counter;			      */	
/*              signed32   op;                                        */
/*              char      *attr_name                                  */
/*		D2_OR_name     *orname;				      */
/*		D2_OR_name     *new_orname;			      */
/*		signed16     	*mod_type;			      */
/*								      */
/*		This function handles special mask of OR name.        */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed32   object_counter;			      */	
/*              signed32   op;                                        */
/*              char      *attr_name                                  */
/*		D2_OR_name     *orname;				      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*		D2_OR_name     *orname;				      */
/*		D2_OR_name     *new_orname;			      */
/*		signed16     	*mod_type;			      */
/*								      */
/* RETURNVALUE:         fkt      funktionkey                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

signed32 d20_orname( signed32		object_counter,
		     signed32         	op,
		     char       	*attr_name,
		     D2_OR_name	*orname,
		     D2_OR_name	*new_orname,
		     signed16		*mod_type)
{
signed32           fkt;
D20_omask          sav_ornameomask[D20_M_SIZE];
D20_omask          sav_7aomask[D20_M_SIZE];
static 	char   	   dir_name[D2_DNL_MAX + 1];


strcpy(d20_attr_name, attr_name);

/* initialise OR name mask.	*/
*d20_ornameifields[DS_ORN_DIRN1] = D2_EOS;
*d20_ornameifields[DS_ORN_DIRN2] = D2_EOS;
*(signed32 *)d20_ornameifields[DS_ORN_ORA] = FALSE;

switch (op) 
	{
 	case DS_DELETE_ATTRIBUTE:
		fkt = FKTCR;
	       	break;

	case DS_DISPLAY_OBJECTS:
	case DS_DISP_OBJ:

		/* if more than one Object is being handled, enable */
		/* 'Cursor Up', 'Cursor Down' and 'F1' keys, else   */
		/* enable only 'Cursor Up' key.			    */
		if (object_counter != 1)
			{
			d20_ornameomask[DS_ORN_SCD_POS].d20_fkt_id =
			d20_ornameomask[DS_ORN_SCU_POS].d20_fkt_id =
			d20_ornameomask[DS_ORN_F1_POS].d20_fkt_id  = D20_FKTX;
			}
		else
			d20_ornameomask[DS_ORN_SCU_POS].d20_fkt_id = D20_FKTX;
			
		d20_save_mask(d20_ornameomask, sav_ornameomask);

	       	d20_cp_from_orname(orname);

		/* display OR name mask */
		fkt = d20_handlemask(d20_ornameomask);

		if ((fkt == FKTCR) || (fkt == FKTMENU))
			if ((*(signed32 *)d20_ornameifields[DS_ORN_ORA]) != FALSE)
        			fkt = d20_oradd_disp(object_counter, 
				      attr_name, orname->d2_or_address);

	       	d20_restore_mask(d20_ornameomask, sav_ornameomask);

		d20_ornameomask[DS_ORN_SCD_POS].d20_fkt_id =
		d20_ornameomask[DS_ORN_SCU_POS].d20_fkt_id =
		d20_ornameomask[DS_ORN_F1_POS].d20_fkt_id  = D20_NO_OP;

	       	break;

	 case DS_MODIFY_ATTRIBUTE:
	       	d20_cp_from_orname(orname);

		d20_ornameomask[DS_ORN_MOD_POS].d20_fkt_id = D20_CSAY;
		d20_ornameomask[DS_ORN_MOD_POS + 1].d20_fkt_id = D20_CGET;
		*(signed32 *)d20_ornameomask[DS_ORN_MOD_POS + 1].d20_ioinfo = 
								D2_REP_AV;

		d20_ornameomask[DS_ORN_SCD_POS].d20_fkt_id = D20_FKTX;
		if (*(signed32 *)d20_ornameifields[DS_ORN_ORA] != FALSE)
		    d20_ornameomask[DS_ORN_F1_POS].d20_fkt_id = D20_FKTX;

		for (;;d20_ornameomask[0].d20_fkt_id &= ~D20_NOERASE)
		    {
		    fkt = d20_handlemask(d20_ornameomask);

		    if (fkt == FKTF1)
			{
			d20_moraddomask[DS_MORADD_SCD_POS].d20_fkt_id =
			d20_noraddomask[DS_NORADD_SCD_POS].d20_fkt_id =
			d20_soraddomask[DS_SORADD_SCD_POS].d20_fkt_id =
			d20_uoraddomask[DS_UORADD_SCD_POS].d20_fkt_id =
			d20_toraddomask[DS_TORADD_SCD_POS].d20_fkt_id =D20_FKTX;

			d20_save_mask(d20_7aomask, sav_7aomask);
			/* enable scroll down key in */
			/* PSAP mask.		     */
			d20_7aomask[D20_SP3POS].d20_fkt_id = D20_FKTX;

			fkt = d20_oradd(DS_DISP_OBJ, attr_name,
					orname->d2_or_address, NULL,NULL);

			d20_moraddomask[DS_MORADD_SCD_POS].d20_fkt_id =
			d20_noraddomask[DS_NORADD_SCD_POS].d20_fkt_id =
			d20_soraddomask[DS_SORADD_SCD_POS].d20_fkt_id =
			d20_uoraddomask[DS_UORADD_SCD_POS].d20_fkt_id =
			d20_toraddomask[DS_TORADD_SCD_POS].d20_fkt_id =
								D20_NO_OP;

       			d20_restore_mask(d20_7aomask, sav_7aomask);

			if (fkt == FKTCR || fkt == FKTMENU)
			    continue;

			break;
			}

		    if (fkt == FKTCR || fkt == FKTMENU)
			if (d20_ornamemask_valid(dir_name) == TRUE)
			    {
			    if (*(signed32 *)d20_ornameifields[DS_ORN_ORA] != 
								FALSE)
				if (orname->d2_or_address !=
						(D2_OR_address *)NULL)
				    fkt = d20_oradd(DS_MODIFY_ATTRIBUTE, 
					  attr_name, orname->d2_or_address,
					  new_orname->d2_or_address, NULL);
				else
				    fkt = d20_oradd(DS_ADD_ATTRIBUTE, attr_name,
					  new_orname->d2_or_address, NULL,
					  NULL);
						

			    if (fkt == FKTCR || fkt == FKTMENU)
				{
				*mod_type = *(signed32 *)d20_ornameomask[
						DS_ORN_MOD_POS + 1].d20_ioinfo;
				d20_cp_to_orname(new_orname, dir_name);
				}
			    }
			else
			    continue;

		    break;
		    }	/* end of for loop to display old value and get new
			   value.	*/

		d20_ornameomask[0].d20_fkt_id &= ~D20_NOERASE;

		d20_ornameomask[DS_ORN_MOD_POS].d20_fkt_id = D20_NO_OP;
		d20_ornameomask[DS_ORN_MOD_POS + 1].d20_fkt_id = D20_NO_OP;
		d20_ornameomask[DS_ORN_SCD_POS].d20_fkt_id = D20_NO_OP;
		d20_ornameomask[DS_ORN_F1_POS].d20_fkt_id = D20_NO_OP;

		break;

	 case DS_ADD_OBJECT:
	 case DS_ADD_ATTRIBUTE:
		for (;;d20_ornameomask[0].d20_fkt_id &= ~D20_NOERASE)
			{
		      	fkt = d20_handlemask(d20_ornameomask);

			if (fkt == FKTCR || fkt == FKTMENU)
				if (d20_ornamemask_valid(dir_name) == TRUE)
					{
					if (*(signed32 *)d20_ornameifields[
						DS_ORN_ORA] != FALSE)
						fkt = d20_oradd(op, attr_name,
							orname->d2_or_address,
							NULL, NULL);
							/* This two last   */
							/* parameters were */
							/* failing !   JMB */

					if (fkt == FKTCR || fkt == FKTMENU)
						d20_cp_to_orname(orname, 
						dir_name);
					}
				else
					continue;

			break;
			}

		d20_ornameomask[0].d20_fkt_id &= ~D20_NOERASE;

	       	break;

	 default:
	       return(D2_ERROR);
    }

return(fkt);

}	/* end of  d20_orname().		*/


/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_ornamemask_valid()                                */
/*								      */
/* AUTHOR:      Sanjay Jain DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: signed32 d20_ornamemask_valid(dir_name)		      */
/*			char	dir_name[];			      */
/*								      */
/*		This function checks the validity of or_name mask.    */
/*		If directory name is given in or_name mask, then it   */
/*		is returned in dir_name after converting it from      */
/*		mask format to iapl format.			      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*		char	dir_name[];				      */
/*								      */
/* RETURNVALUE:         					      */
/*		D2_TRUE		If mask is valid.		      */
/*		D2_FALSE	If mask is invalid.		      */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

static signed32 d20_ornamemask_valid(char dir_name[])
{
if ((d20_ornameifields[DS_ORN_DIRN1][0] == D2_EOS) &&
    (d20_ornameifields[DS_ORN_DIRN2][0] == D2_EOS) &&
    (*(signed32 *)d20_ornameifields[DS_ORN_ORA] == FALSE))
	{
	/* display error mask.		*/
	/* at least one of the components must be entered.	*/
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errmess[D20_E53IDX];
	d20_handlemask(d20_qomask);

	return(FALSE);
	}

if ((d20_ornameifields[DS_ORN_DIRN1][0] != D2_EOS) ||
    (d20_ornameifields[DS_ORN_DIRN2][0] != D2_EOS))
	{
	/* directory name is specified, check for its correctness   */
	/* and if correct generate new directory name with Obj. Ids */
	/* of naming attribute.					    */

	strcpy(dir_name, d20_ornameifields[DS_ORN_DIRN1]);
	strcat(dir_name, d20_ornameifields[DS_ORN_DIRN2]);

	if (ds0_508_check_name(dir_name) != D2_NO_ERR)
		{
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_errmess[D20_E54IDX];
		d20_handlemask(d20_qomask);
		return(FALSE);
		}
	}

return(TRUE);
}	/* end of d20_ornamemask_valid().	*/


/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_cp_to_orname()                                    */
/*								      */
/* AUTHOR:      Sanjay Jain DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: d20_cp_to_orname(orname, dir_name)		      */
/*		D2_OR_name     *orname;				      */
/*		char		dir_name[];			      */
/*								      */
/*		This function copies dir_name to orname and sets the  */
/*		or_address pointer to NULL if or_address field in mask*/
/*		is false.					      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		char		dir_name[];			      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*		D2_OR_name     *orname;				      */
/*								      */
/* RETURNVALUE:         					      */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

static void d20_cp_to_orname(D2_OR_name *orname, char dir_name[])
{
    if ((d20_ornameifields[DS_ORN_DIRN1][0] != D2_EOS) ||
	(d20_ornameifields[DS_ORN_DIRN2][0] != D2_EOS))
	    strcpy((char *)orname->d2_dir_name, (char *)dir_name);
    else
	    orname->d2_dir_name = (D2_name_string)NULL;

    if (*(signed32 *)d20_ornameifields[DS_ORN_ORA] == FALSE)
	    orname->d2_or_address = (D2_OR_address *)NULL;
} 	/* end of d20_cp_to_orname().	*/


/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_cp_from_orname()                                  */
/*								      */
/* AUTHOR:      Sanjay Jain DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: d20_cp_from_orname(orname) 			      */
/*		D2_OR_name     *orname;				      */
/*								      */
/*		This function copies information from orname structure*/
/*		to orname_mask.					      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		D2_OR_name     *orname;				      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE: 						      */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

static void d20_cp_from_orname(D2_OR_name *orname)
{
char	 dist_name[D2_DNL_MAX + 1];
signed32 dist_name_len;

if (orname->d2_dir_name != (D2_name_string)NULL)
	{
	strcpy(dist_name, (char *)orname->d2_dir_name);
	d20_gen_dnintf(dist_name);
	dist_name_len = strlen(dist_name) + 1  /* including EOS */;

	strncpy(d20_ornameifields[DS_ORN_DIRN1], dist_name, FLEN);

	/* length of dir name > FLEN         */
	if (dist_name_len > FLEN)
		strncpy(d20_ornameifields[DS_ORN_DIRN2], dist_name + FLEN, 
									FLEN);
	else
		*d20_ornameifields[DS_ORN_DIRN2] = D2_EOS;
	}

if (orname->d2_or_address == (D2_OR_address *)NULL)
	*(signed32 *)d20_ornameifields[DS_ORN_ORA] = FALSE;
else
	*(signed32 *)d20_ornameifields[DS_ORN_ORA] = 1;
} 	/* end of d20_cp_from_orname()		*/


/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_dlsp()                                            */
/*								      */
/* AUTHOR:      Sanjay Jain DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: signed32 d20_dlsp(object_counter, op, attr_name, dlsp,*/
/*			     new_dlsp, orname, mog_space_ptr,mod_type)*/
/*			signed32		object_counter;	      */
/*			signed32        	op;	      	      */
/*			char       		*attr_name;	      */
/*			D2_dl_submit_perm	*dlsp;	      	      */
/*			D2_dl_submit_perm	*new_dlsp;	      */
/*			D2_OR_name		*orname;	      */
/*			char			*mog_space_ptr;	      */
/*			signed16		*mod_type;	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*			signed32		object_counter;	      */
/*			signed32       		op;	      	      */
/*			char       		*attr_name;	      */
/*			D2_dl_submit_perm	*dlsp;	      	      */
/*			D2_OR_name		*orname;	      */
/*			char			*mog_space_ptr;	      */
/*			signed16		*mod_type;	      */
/*								      */
/*			This function handles DL-Submit-Permission    */
/*			syntax mask.				      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*			D2_dl_submit_perm	*dlsp;	      	      */
/*			D2_OR_name		*orname;	      */
/*			D2_dl_submit_perm	*new_dlsp;	      */
/*			char			*mog_space_ptr;	      */
/*			signed16     		*mod_type;	      */
/*								      */
/* RETURNVALUE:         fkt      funktionkey                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

signed32 d20_dlsp(	signed32			object_counter,
			signed32         		op,
			char       		*attr_name,
			D2_dl_submit_perm	*dlsp,
			D2_dl_submit_perm	*new_dlsp,
			D2_OR_name		*orname,
			char			*mog_space_ptr,
			signed16		*mod_type)
{
signed32           fkt;
D20_omask          sav_mogomask[D20_M_SIZE];
D20_omask          sav_ornameomask[D20_M_SIZE];
D20_omask          sav_7aomask[D20_M_SIZE];
static 	char   	   mog_name[D2_DNL_MAX + 1];
signed32	   mog_name_len;

strcpy(d20_attr_name, attr_name);

/* initialise Member Of Group mask. 	*/
*d20_mogifields[DS_MOG_MOG1] = D2_EOS;
*d20_mogifields[DS_MOG_MOG2] = D2_EOS;

switch (op) 
	{
 	case DS_DELETE_ATTRIBUTE:
		fkt = FKTCR;
	       	break;

	case DS_DISPLAY_OBJECTS:
	case DS_DISP_OBJ:
		switch (dlsp->d2_perm_type)
			{
			case D21_DLSP_INDIV:
			case D21_DLSP_MEM_DL:
			case D21_DLSP_PATT:
				d20_ornameomask[DS_ORN_DLST_POS].d20_fkt_id =
				d20_ornameomask[DS_ORN_DLST_POS + 1].d20_fkt_id
						 = D20_CSAY;

		                *(signed32 *)d20_ornameifields[DS_ORN_DLST] = 
							dlsp->d2_perm_type;
				fkt = d20_orname(object_counter, op, attr_name,
						 dlsp->d2_choice.d2_individual,
						 NULL, NULL);

				d20_ornameomask[DS_ORN_DLST_POS].d20_fkt_id =
				d20_ornameomask[DS_ORN_DLST_POS + 1].d20_fkt_id
						 = D20_NO_OP;

				break;
			case D21_DLSP_MEM_G:
				d20_save_mask(d20_mogomask, sav_mogomask);
				if (object_counter != 1)
					{
				       d20_mogomask[DS_MOG_SCD_POS].d20_fkt_id =
				       d20_mogomask[DS_MOG_SCU_POS].d20_fkt_id =
				       d20_mogomask[DS_MOG_F1_POS].d20_fkt_id = 
								D20_FKTX; 
					}
				else
				       d20_mogomask[DS_MOG_SCU_POS].d20_fkt_id =
								D20_FKTX; 

				strcpy(mog_name, 
				  (char *)dlsp->d2_choice.d2_member_of_group);

				d20_gen_dnintf(mog_name);

				/* including EOS */;
				mog_name_len = strlen(mog_name) + 1;

				strncpy(d20_mogifields[DS_MOG_MOG1], mog_name, 
					FLEN);

				/* length of mog_name > FLEN         */
				if (mog_name_len > FLEN)
					strncpy(d20_mogifields[DS_MOG_MOG2], 
						mog_name + FLEN, FLEN);
				else
					d20_mogifields[DS_MOG_MOG2][0] = D2_EOS;

				fkt = d20_handlemask(d20_mogomask);

	       			d20_restore_mask(d20_mogomask, sav_mogomask);

				break;
			}

	       	break;

	 case DS_MODIFY_ATTRIBUTE:
		switch (dlsp->d2_perm_type)
		    {
		    case D21_DLSP_INDIV:
		    case D21_DLSP_MEM_DL:
		    case D21_DLSP_PATT:
	       	       	d20_cp_from_orname(dlsp->d2_choice.d2_individual);

			d20_save_mask(d20_ornameomask, sav_ornameomask);
			d20_ornameomask[DS_ORN_SCD_POS - 1].d20_fkt_id =
			d20_ornameomask[DS_ORN_SCD_POS - 2].d20_fkt_id =
			d20_ornameomask[DS_ORN_SCD_POS - 3].d20_fkt_id =
			d20_ornameomask[DS_ORN_SCD_POS - 4].d20_fkt_id =
								D20_NO_OP;

			d20_ornameomask[DS_ORN_MOD_POS].d20_fkt_id = D20_CSAY;
			d20_ornameomask[DS_ORN_MOD_POS + 1].d20_fkt_id =
								D20_CSAY;
			*(signed32 *)d20_ornameomask[DS_ORN_MOD_POS + 1].d20_ioinfo 
								= D2_REP_AV;

			d20_dlpermomask[0].d20_fkt_id = D20_NOERASE;
			d20_dlpermomask[DS_DLPERM_SCD_POS].d20_fkt_id =D20_FKTX;

			if (*(signed32 *)d20_ornameifields[DS_ORN_ORA] != FALSE)
			    d20_dlpermomask[DS_DLPERM_F1_POS].d20_fkt_id = 
								D20_FKTX;

			*(signed32 *)d20_dlpermifields[DS_DLPERM_TYPES] = 
							dlsp->d2_perm_type;
				
			d20_moraddomask[DS_MORADD_SCD_POS].d20_fkt_id =
			d20_noraddomask[DS_NORADD_SCD_POS].d20_fkt_id =
			d20_soraddomask[DS_SORADD_SCD_POS].d20_fkt_id =
			d20_uoraddomask[DS_UORADD_SCD_POS].d20_fkt_id =
			d20_toraddomask[DS_TORADD_SCD_POS].d20_fkt_id =D20_FKTX;

			d20_save_mask(d20_7aomask, sav_7aomask);
				/* enable scroll down key in */
				/* PSAP mask.		     */
			d20_7aomask[D20_SP3POS].d20_fkt_id = D20_FKTX;

			for (;;)
			    {
		      	    fkt = d20_handlemask(d20_ornameomask);

		      	    fkt = d20_handlemask(d20_dlpermomask);

			    if (fkt == FKTF1)
				{
				fkt = d20_oradd(DS_DISP_OBJ, attr_name, 
				      dlsp->d2_choice.d2_individual->
				      d2_or_address, NULL, NULL);

				if (fkt == FKTCR || fkt == FKTMENU)
				    continue;
				}

			    break;
			    }

			d20_moraddomask[DS_MORADD_SCD_POS].d20_fkt_id =
			d20_noraddomask[DS_NORADD_SCD_POS].d20_fkt_id =
			d20_soraddomask[DS_SORADD_SCD_POS].d20_fkt_id =
			d20_uoraddomask[DS_UORADD_SCD_POS].d20_fkt_id =
			d20_toraddomask[DS_TORADD_SCD_POS].d20_fkt_id =
								D20_NO_OP;

	       		d20_restore_mask(d20_7aomask, sav_7aomask);

			d20_dlpermomask[0].d20_fkt_id = D20_CSAY;
			d20_dlpermomask[DS_DLPERM_SCD_POS].d20_fkt_id = 
			d20_dlpermomask[DS_DLPERM_F1_POS].d20_fkt_id = 
								D20_NO_OP; 

			d20_restore_mask(d20_ornameomask, sav_ornameomask);

			if (fkt == FKTCR || fkt == FKTMENU)
			    {
			    new_dlsp->d2_perm_type = *(signed32 *)d20_dlpermifields[
							     DS_DLPERM_TYPES];

			    if (new_dlsp->d2_perm_type != D21_DLSP_MEM_G)
				{
				/* any one of the OR name types can */
				/* be set.			    */
				d20_ornameomask[0].d20_fkt_id |= D20_NOERASE;

				d20_ornameomask[DS_ORN_DLST_POS].d20_fkt_id =
				d20_ornameomask[DS_ORN_DLST_POS + 1].d20_fkt_id
						 		= D20_CSAY;

		                *(signed32 *)d20_ornameifields[DS_ORN_DLST] = 
		                *(signed32 *)d20_dlpermifields[ DS_DLPERM_TYPES];

				new_dlsp->d2_choice.d2_individual = orname;
				fkt = d20_orname(0, DS_MODIFY_ATTRIBUTE,
						attr_name, dlsp->d2_choice.
						d2_individual, orname, 
						mod_type);

				d20_ornameomask[0].d20_fkt_id &= ~D20_NOERASE;

				d20_ornameomask[DS_ORN_DLST_POS].d20_fkt_id =
				d20_ornameomask[DS_ORN_DLST_POS + 1].d20_fkt_id
						 		= D20_NO_OP;
				}
			    else
				{
				/* member of group case */
				d20_mogomask[DS_MOG_MOD_POS].d20_fkt_id = 
								D20_CSAY;
				d20_mogomask[DS_MOG_MOD_POS + 1].d20_fkt_id = 
								D20_CGET;
				*(signed32 *)d20_mogomask[DS_MOG_MOD_POS + 1].
							d20_ioinfo = D2_REP_AV;

				d20_mogomask[DS_MOG_SCD_POS].d20_fkt_id = 
								D20_FKTX;

				for (;;)
				    {
				    fkt = d20_handlemask(d20_mogomask);

				    if (fkt == FKTCR || fkt == FKTMENU)
					{
					if ((*d20_mogifields[DS_MOG_MOG1] == 
					     D2_EOS) &&
					     (*d20_mogifields[DS_MOG_MOG2] == 
					     D2_EOS)) 
					    {
					    d20_qomask[D20_Q1POS].d20_ioinfo = 
					    d20_errmess[D20_E53IDX];
					    d20_handlemask(d20_qomask);
					    continue;
					    }

					strcpy(mog_name, 
						d20_mogifields[DS_MOG_MOG1]);
					strcat(mog_name, 
						d20_mogifields[DS_MOG_MOG2]);

					if (ds0_508_check_name(mog_name) != 
								D2_NO_ERR)
					    {
					    d20_qomask[D20_Q1POS].d20_ioinfo = 
					    d20_errmess[D20_E54IDX];
					    d20_handlemask(d20_qomask);
					    continue;
					    }

					strcpy(mog_space_ptr, mog_name);
					new_dlsp->d2_choice.d2_member_of_group =
						(D2_name_string)mog_space_ptr;
					*mod_type = *(signed32 *)d20_mogomask[
					DS_MOG_MOD_POS+1].d20_ioinfo;
					}

				    break;
				    } /* end of for loop.	*/
				d20_mogomask[DS_MOG_MOD_POS].d20_fkt_id = 
								D20_NO_OP;
				d20_mogomask[DS_MOG_MOD_POS + 1].d20_fkt_id = 
								D20_NO_OP;
				d20_mogomask[DS_MOG_SCD_POS].d20_fkt_id = 
								D20_NO_OP;
				} /* end of member of group case.	*/
			    } /* FKT ==CR || FKT == MENU	*/

			break;

		    case D21_DLSP_MEM_G:
			strcpy(mog_name,
			    (char *) dlsp->d2_choice.d2_member_of_group);

			d20_gen_dnintf(mog_name);

			/* including EOS */;
			mog_name_len = strlen(mog_name) + 1;

			strncpy(d20_mogifields[DS_MOG_MOG1], mog_name, FLEN);

			/* length of mog_name > FLEN         */
			if (mog_name_len > FLEN)
			    strncpy(d20_mogifields[DS_MOG_MOG2], 
							mog_name + FLEN, FLEN);
			else
			    d20_mogifields[DS_MOG_MOG2][0] = D2_EOS;

			d20_save_mask(d20_mogomask, sav_mogomask);
			d20_mogomask[DS_MOG_SCD_POS - 1].d20_fkt_id =
			d20_mogomask[DS_MOG_SCD_POS - 2].d20_fkt_id =
			d20_mogomask[DS_MOG_SCD_POS - 3].d20_fkt_id =
			d20_mogomask[DS_MOG_SCD_POS - 4].d20_fkt_id = D20_NO_OP;

			d20_mogomask[DS_MOG_MOD_POS].d20_fkt_id = D20_CSAY;
			d20_mogomask[DS_MOG_MOD_POS + 1].d20_fkt_id = D20_CSAY;
			*(signed32 *)d20_mogomask[DS_MOG_MOD_POS + 1].d20_ioinfo = 
								D2_REP_AV;

			d20_dlpermomask[0].d20_fkt_id = D20_NOERASE;
			d20_dlpermomask[DS_DLPERM_SCD_POS].d20_fkt_id = 
								D20_FKTX;

			*(signed32 *)d20_dlpermifields[DS_DLPERM_TYPES] = 
							dlsp->d2_perm_type;
				
		      	fkt = d20_handlemask(d20_mogomask);

		      	fkt = d20_handlemask(d20_dlpermomask);

			d20_dlpermomask[0].d20_fkt_id = D20_CSAY;
			d20_dlpermomask[DS_DLPERM_SCD_POS].d20_fkt_id = 
								D20_NO_OP;

			d20_restore_mask(d20_mogomask, sav_mogomask);

			if (fkt == FKTCR || fkt == FKTMENU)
			    {
			    new_dlsp->d2_perm_type = *(signed32 *)d20_dlpermifields[
							DS_DLPERM_TYPES];

			    if (new_dlsp->d2_perm_type != D21_DLSP_MEM_G)
				{
				/* any one of the OR name types can */
				/* be set.			    */
				d20_ornameomask[DS_ORN_DLST_POS].d20_fkt_id =
				d20_ornameomask[DS_ORN_DLST_POS + 1].d20_fkt_id
						 		= D20_CSAY;

                		*(signed32 *)d20_ornameifields[DS_ORN_DLST] = 
		                *(signed32 *)d20_dlpermifields[DS_DLPERM_TYPES];

				d20_ornameomask[DS_ORN_MOD_POS].d20_fkt_id = 
								D20_CSAY;
				d20_ornameomask[DS_ORN_MOD_POS + 1].d20_fkt_id 
								= D20_CGET;
				*(signed32 *)d20_ornameomask[DS_ORN_MOD_POS + 1].
							d20_ioinfo = D2_REP_AV;

				/* enable Scroll Down keys in */
				/* Masks involved.	      */
				d20_ornameomask[DS_ORN_SCD_POS].d20_fkt_id = 
				d20_oraddomask[DS_ORADD_SCD_POS].d20_fkt_id =
				d20_moraddomask[DS_MORADD_SCD_POS].d20_fkt_id =
				d20_noraddomask[DS_NORADD_SCD_POS].d20_fkt_id =
				d20_soraddomask[DS_SORADD_SCD_POS].d20_fkt_id =
				d20_uoraddomask[DS_UORADD_SCD_POS].d20_fkt_id =
				d20_toraddomask[DS_TORADD_SCD_POS].d20_fkt_id =
				d20_7aomask[D20_SP3POS].d20_fkt_id = D20_FKTX;

				new_dlsp->d2_choice.d2_individual = orname;
				fkt = d20_orname(0, DS_ADD_ATTRIBUTE, attr_name,
						 orname, NULL, NULL);

				/* disable Scrol Down keys in */
				/* Masks involved.	      */
				d20_ornameomask[DS_ORN_SCD_POS].d20_fkt_id = 
				d20_oraddomask[DS_ORADD_SCD_POS].d20_fkt_id =
				d20_moraddomask[DS_MORADD_SCD_POS].d20_fkt_id =
				d20_noraddomask[DS_NORADD_SCD_POS].d20_fkt_id =
				d20_soraddomask[DS_SORADD_SCD_POS].d20_fkt_id =
				d20_uoraddomask[DS_UORADD_SCD_POS].d20_fkt_id =
				d20_toraddomask[DS_TORADD_SCD_POS].d20_fkt_id =
				d20_7aomask[D20_SP3POS].d20_fkt_id = D20_NO_OP;

				d20_ornameomask[DS_ORN_MOD_POS].d20_fkt_id = 
				d20_ornameomask[DS_ORN_MOD_POS + 1].d20_fkt_id =
								 D20_NO_OP;

				d20_ornameomask[DS_ORN_DLST_POS].d20_fkt_id =
				d20_ornameomask[DS_ORN_DLST_POS + 1].d20_fkt_id
						 		= D20_NO_OP;

				*mod_type = *(signed32 *)d20_ornameomask[
					    DS_ORN_MOD_POS + 1].d20_ioinfo;
				} /* end of orname syntax handling.	*/
			    else
				{
				/* member of group case */
				d20_mogomask[0].d20_fkt_id |= D20_NOERASE;

				d20_mogomask[DS_MOG_MOD_POS].d20_fkt_id = 
								D20_CSAY;
				d20_mogomask[DS_MOG_MOD_POS + 1].d20_fkt_id = 
								D20_CGET;
				*(signed32 *)d20_mogomask[DS_MOG_MOD_POS + 1].
							d20_ioinfo = D2_REP_AV;

				d20_mogomask[DS_MOG_SCD_POS].d20_fkt_id = 
								D20_FKTX;

				for (;;
				     d20_mogomask[0].d20_fkt_id &= ~D20_NOERASE)
				    {
				    fkt = d20_handlemask(d20_mogomask);

				    if (fkt == FKTCR || fkt == FKTMENU)
					{
					if ((*d20_mogifields[DS_MOG_MOG1] == 
					     D2_EOS) &&
					     (*d20_mogifields[DS_MOG_MOG2] == 
					     D2_EOS)) 
					    {
					    d20_qomask[D20_Q1POS].d20_ioinfo = 
					    d20_errmess[D20_E53IDX];
					    d20_handlemask(d20_qomask);
					    continue;
					    }

					strcpy(mog_name, 
						d20_mogifields[DS_MOG_MOG1]);
					strcat(mog_name, 
						d20_mogifields[DS_MOG_MOG2]);

					if (ds0_508_check_name(mog_name) != 
								D2_NO_ERR)
					    {
					    d20_qomask[D20_Q1POS].d20_ioinfo = 
					    d20_errmess[D20_E54IDX];
					    d20_handlemask(d20_qomask);
					    continue;
					    }

					strcpy(mog_space_ptr, mog_name);
					new_dlsp->d2_choice.d2_member_of_group =
						(D2_name_string)mog_space_ptr;
					*mod_type = *(signed32 *)d20_mogomask[
					DS_MOG_MOD_POS+1].d20_ioinfo;
					}

				    break;
				    } /* end of for loop.	*/

				d20_mogomask[0].d20_fkt_id &= ~D20_NOERASE;

				d20_mogomask[DS_MOG_MOD_POS].d20_fkt_id = 
								D20_NO_OP;
				d20_mogomask[DS_MOG_MOD_POS + 1].d20_fkt_id = 
								D20_NO_OP;
				d20_mogomask[DS_MOG_SCD_POS].d20_fkt_id = 
								D20_NO_OP;
				} /* member of group selected. */
			    } /* end of FKT == CR || FKT == MENU.	*/
			break; /* break from DLSP_MEM_G case	*/
			} /* end of switch	*/

		break; /* break from MODIFY_ATTRIBUTE case	*/

	 case DS_ADD_OBJECT:
	 case DS_ADD_ATTRIBUTE:
		*(signed32 *)d20_dlpermifields[DS_DLPERM_TYPES] = D21_DLSP_INDIV;
		fkt = d20_handlemask(d20_dlpermomask);

		if (fkt == FKTCR || fkt == FKTMENU)
		    {
		    switch (*(signed32 *)d20_dlpermifields[DS_DLPERM_TYPES])
			{
			case D21_DLSP_INDIV:
			case D21_DLSP_MEM_DL:
			case D21_DLSP_PATT:
			    d20_ornameomask[0].d20_fkt_id |= D20_NOERASE;
			    d20_ornameomask[DS_ORN_DLST_POS].d20_fkt_id =
			    d20_ornameomask[DS_ORN_DLST_POS + 1].d20_fkt_id
						 		= D20_CSAY;

		            *(signed32 *)d20_ornameifields[DS_ORN_DLST] = 
		            *(signed32 *)d20_dlpermifields[DS_DLPERM_TYPES];
			    fkt = d20_orname(object_counter, op, attr_name,
					     orname, NULL, NULL);

			    d20_ornameomask[0].d20_fkt_id &= ~D20_NOERASE;
			    d20_ornameomask[DS_ORN_DLST_POS].d20_fkt_id =
			    d20_ornameomask[DS_ORN_DLST_POS + 1].d20_fkt_id
						 		= D20_NO_OP;

			    if ((fkt == FKTCR) || (fkt == FKTMENU))
				{
				dlsp->d2_perm_type = 
			        *(signed32 *)d20_dlpermifields[DS_DLPERM_TYPES];
				/* any one of the OR name types can */
				/* be set.			    */
				dlsp->d2_choice.d2_individual = orname;
				}

			    break;

			case D21_DLSP_MEM_G:
			    d20_mogomask[0].d20_fkt_id |= D20_NOERASE;

			    for (;;d20_mogomask[0].d20_fkt_id &= ~D20_NOERASE)
				{
				fkt = d20_handlemask(d20_mogomask);

				if (fkt == FKTCR || fkt == FKTMENU)
				    {
				    if ((*d20_mogifields[DS_MOG_MOG1] == D2_EOS)
					 && (*d20_mogifields[DS_MOG_MOG2] == 
								D2_EOS))
					{
					d20_qomask[D20_Q1POS].d20_ioinfo = 
					d20_errmess[D20_E53IDX];
					d20_handlemask(d20_qomask);
					continue;
					}

				    strcpy(mog_name, d20_mogifields[
								DS_MOG_MOG1]);
				    strcat(mog_name, d20_mogifields[
								DS_MOG_MOG2]);

				    if (ds0_508_check_name(mog_name) != 
								D2_NO_ERR)
				        {
				        d20_qomask[D20_Q1POS].d20_ioinfo = 
				        d20_errmess[D20_E54IDX];
				        d20_handlemask(d20_qomask);
				        continue;
				        }

				    dlsp->d2_perm_type = D21_DLSP_MEM_G;
				    strcpy(mog_space_ptr, mog_name);
				    dlsp->d2_choice.d2_member_of_group = 
						(D2_name_string)mog_space_ptr;
				    }

				break;
				}	/* end of for loop.	*/

			    d20_mogomask[0].d20_fkt_id &= ~D20_NOERASE;

			    break; /* break from member of group case.	*/
			}	/* end of switch.	*/
		    }	/* end of fkt == CR || fkt == MENU	*/

	       	break;

	 default:
	       return(D2_ERROR);
    }

return(fkt);

}	/* end of  d20_dlsp().		*/

