/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20setinfo.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:57  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:12:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:09  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:08  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:02:54  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:38  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:15:24  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:50:25  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:11:27  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:46:09  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:59:27  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:40:45  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:36:00  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:35:27  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:08:48  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20setinfo.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:00 $";
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
/* NAME         : d20setinfo.c                                        */
/*								      */
/* AUTHOR       : Volpers,D AP 11                                     */
/* DATE         : 21.04.88                                            */
/*								      */
/* KOMPONENTE	: DS						      */
/*								      */
/* DOK.-NR.	: DS-Design-Spez.				      */
/*			.					      */
/*			.					      */
/*                                                                    */
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
/*      0.1 | 15.04.88| Erstellung                     | hv |         */
/*      0.2 | 31.01.89| Fehler in "copies update"      | WS |     57  */
/*          |         | (-->"set_name()"               |    |         */
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
#include <dce/d27util.h>
#include <d2info.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

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

extern  Ds_v2_info      info ;
			    /* common IAPL-interface parameter block */
extern  signed16        d20_bid;
				      /* bind-, directory identifier */
extern  Bool            d20_shadow ;          /* 'using shadow'-flag */

static  signed32        serv_cntrls;

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_160_set_info                                      */
/*								      */
/* AUTHOR:      Volpers   (D AP 11)                                   */
/* DATE:        21.04.88                                              */
/*								      */
/* SYNTAX:      void  d20_160_set_info(b_id, com_arg, file)           */
/*		signed16  b_id;					      */
/*		D2_c_arg *com_arg;				      */
/*		char     *file;					      */
/*								      */
/* DESCRIPTION: This function set the bind_id and serv_controls       */
/*              in the info structure                                 */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed16  b_id;	    bind identification		      */
/*		D2_c_arg *com_arg;  common arguments		      */
/*		char     *file;     file name			      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       no error                                      */
/*      -1      :       break is selected                             */
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

 void d20_160_set_info(
   signed16  b_id,
   D2_c_arg *com_arg,
   char     *file)

{                               /*  d20_160_set_info                  */

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

   info.bind_id = b_id ;
   info.serv_cntrl.d2_serv_cntrls = com_arg->d2_serv_cntrls;
   info.serv_cntrl.d2_priority = com_arg->d2_priority;
   info.serv_cntrl.d2_time_limit = com_arg->d2_time_limit;
   info.serv_cntrl.d2_size_limit = com_arg->d2_size_limit;
   info.filename = file ;

}                               /*  d20_160_set_info                  */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_161_reset_info                                    */
/*								      */
/* AUTHOR:      Volpers   (D AP 11)                                   */
/* DATE:        21.04.88                                              */
/*								      */
/* SYNTAX:      void d20_161_reset_info()                             */
/*                                                                    */
/*								      */
/* DESCRIPTION: This function reset the bind_id and serv_controls     */
/*              in the info structure                                 */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       no error                                      */
/*      -1      :       break is selected                             */
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

 void d20_161_reset_info(void)

{                               /*  d20_161_reset_info                */

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

   info.bind_id = d20_bid ;

   info.serv_cntrl.d2_serv_cntrls = (d20_shadow) ?
       D2_LOCALSCOPE | D2_NOREFERRAL | D2_USEDSA | D2_DREFALIAS
       | D2_DONT_STORE :
       D2_NOCACHE | D2_NOREFERRAL | D2_USEDSA | D2_DREFALIAS
       | D2_DONT_STORE ;

   info.serv_cntrl.d2_serv_cntrls |= D2_PREF_ADM_FUN;

   info.serv_cntrl.d2_priority = D2_PRIO_LOW ;
   info.serv_cntrl.d2_time_limit = D2_T_UNLIMITED ;
   info.serv_cntrl.d2_size_limit = D2_S_UNLIMITED ;
   info.filename = NULL ;


}                               /*  d20_161_reset_info                */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_162_set_name                                      */
/*								      */
/* AUTHOR:      Volpers   (D AP 11)                                   */
/* DATE:        02.05.88                                              */
/*								      */
/* SYNTAX:      void  d20_162_set_name(dn)                            */
/*              String         dn;                                    */
/*								      */
/* DESCRIPTION: This function set the entry information in the        */
/*              info structure                                        */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              String         dn;    DN of the object                */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       no error                                      */
/*      -1      :       break is selected                             */
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

 void d20_162_set_name(
   String dn)

{                               /*  d20_162_set_name                  */

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

strcpy((char *)(info.entry + 1), dn) ;
*(info.entry) = D2_RDN_SEP;

}                               /*  d20_162_set_name                  */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_164_get_subset                                    */
/*								      */
/* AUTHOR:      Volpers   (D AP 11)                                   */
/* DATE:        29.04.88                                              */
/*								      */
/* SYNTAX:      void  d20_164_get_subset(subset)                      */
/*              D2_subset       *subset ;                             */
/*								      */
/* DESCRIPTION: This function takes the subset                        */
/*                           D2_BASE_OBJECT   = single object search  */
/*                           D2_WHOLE_SUBTREE = whole subtree search  */
/*                                                                    */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       no error                                      */
/*      -1      :       break is selected                             */
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

 void d20_164_get_subset(
   D2_subset *subset)

{                               /*  d20_164_get_subset                */

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

     /* fill in given subset value into common parameter block */
      *subset = info.srchop ;


}                               /*  d20_164_get_subset                */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_166_set_all_attr                                  */
/*								      */
/* AUTHOR:      Volpers   (D AP 11)                                   */
/* DATE:        16.05.88                                              */
/*								      */
/* SYNTAX:      void  d20_166_set_all_attr()                          */
/*                                                                    */
/*								      */
/* DESCRIPTION: This function set the id in the info structure if     */
/*              all attributes are requested                          */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       no error                                      */
/*      -1      :       break is selected                             */
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

 void d20_166_set_all_attr(void)

{                               /*  d20_166_set_all_attr              */

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

 info.reqinfo.d2_r_no_at = D2_ALL_ATT_REQ ;

}                               /*  d20_166_set_all_attr              */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_167_set_file_mode()                               */
/*								      */
/* AUTHOR:      Volpers   (D AP 11)                                   */
/* DATE:        16.05.88                                              */
/*								      */
/* SYNTAX:      void  d20_167_set_file_mode()                         */
/*                                                                    */
/*								      */
/* DESCRIPTION:                                                       */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
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

 void d20_167_set_file_mode(void)

{                               /*  d20_167_set_file_mode()           */

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

info.serv_cntrl.d2_serv_cntrls |= D2_FILE_MODE;

}                               /*  d20_167_set_file_mode()           */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_168_set_serv_cntrls()                             */
/*								      */
/* AUTHOR:      Schmid    (D AP 11)                                   */
/* DATE:        16.06.88                                              */
/*								      */
/* SYNTAX:      void  d20_168_set_serv_cntrls()                       */
/*                                                                    */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will set the service controls for       */
/*              "local-scope" - search.                               */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
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

 void d20_168_set_serv_cntrls(void)

{                               /*  d20_168_set_serv_cntrls()         */

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

serv_cntrls = info.serv_cntrl.d2_serv_cntrls;
info.serv_cntrl.d2_serv_cntrls &= ~D2_NOCACHE;
info.serv_cntrl.d2_serv_cntrls |=  D2_LOCALSCOPE;

}                               /*  d20_168_set_serv_cntrls()         */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_169_reset_serv_cntrls()                           */
/*								      */
/* AUTHOR:      Schmid    (D AP 11)                                   */
/* DATE:        16.06.88                                              */
/*								      */
/* SYNTAX:      void  d20_169_reset_serv_cntrls()                     */
/*                                                                    */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will reset the service controls.        */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
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

 void d20_169_reset_serv_cntrls(void)

{                               /*  d20_169_reset_serv_cntrls()       */

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

info.serv_cntrl.d2_serv_cntrls = serv_cntrls;

}                               /*  d20_169_reset_serv_cntrls()       */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_170_set_req_attr(...)                             */
/*								      */
/* AUTHOR:      Schmid    (D AP 11)                                   */
/* DATE:        27.07.88                                              */
/*								      */
/* SYNTAX:      void  d20_170_set_req_attr(no, types)                 */
/*              signed16      no;                                     */
/*              D2_a_type *types;                                     */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will set the requested attribute types. */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed16   no;      number of attribute types         */
/*              D2_a_type *types;   attribute types                   */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
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

 void d20_170_set_req_attr(
   signed16   no,
   D2_a_type *types)

{                               /*  d20_170_set_req_info()            */

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

 signed16  i;
 D2_a_type *d20_typeptr;

info.reqinfo.d2_r_no_at = 0 ;
d20_typeptr = info.reqinfo.d2_r_type_at ;

for (i = 0; i < no; i++, types++)
{ 
  *d20_typeptr++ = *types;
  info.reqinfo.d2_r_no_at++ ;
} 

}                               /*  d20_170_set_req_info()            */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_172_set_master_serv_cntrls()                      */
/*								      */
/* AUTHOR:      Schmid    (D AP 11)                                   */
/* DATE:        03.08.89                                              */
/*								      */
/* SYNTAX:      void  d20_172_set_master_serv_cntrls()                */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will set the service controls for       */
/*              master update operation.                              */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
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

 void d20_172_set_master_serv_cntrls(void)

{                               /*  d20_172_set_master_serv_cntrls()  */

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

serv_cntrls = info.serv_cntrl.d2_serv_cntrls;
info.serv_cntrl.d2_serv_cntrls &= ~D2_LOCALSCOPE;
info.serv_cntrl.d2_serv_cntrls |= D2_NOCACHE;

}                               /*  d20_172_set_master_serv_cntrls()  */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_173_init_serv_cntrls()                            */
/*								      */
/* AUTHOR:      Schmid    (D AP 11)                                   */
/* DATE:        03.08.89                                              */
/*								      */
/* SYNTAX:      void  d20_173_init_serv_cntrls(s_cntrls)              */
/*              signed32  s_cntrls;				      */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will set the service controls.          */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed32  s_cntrls;     service controls to be set    */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
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

 void d20_173_init_serv_cntrls(
   signed32 s_cntrls)

{                               /*  d20_173_init_serv_cntrls()        */

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

info.serv_cntrl.d2_serv_cntrls = s_cntrls;

}                               /*  d20_173_init_serv_cntrls()        */
