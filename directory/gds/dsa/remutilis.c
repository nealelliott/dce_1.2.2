/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: remutilis.c,v $
 * Revision 1.1.10.2  1996/02/18  18:15:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:09  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:50:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:47  root]
 * 
 * Revision 1.1.8.4  1994/07/06  15:07:15  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:15:06  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:53:16  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:28:19  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:13:55  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:25:52  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:37:37  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:25:11  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  16:01:14  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:27:38  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:44:22  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:22:40  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:34:53  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:24:53  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  02:56:18  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:32:57  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char remutilis_rcsid[] = "@(#)$RCSfile: remutilis.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:15:37 $";
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
/* NAME         : d26isrem.c    <DS_REMOVE_OBJECT>                    */
/*                                                                    */
/* AUTHOR       : Volpers,  D AP 11                                   */
/* DATE         : 21.03.88                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM DEPENDENCIES: SINIX (--> C-ISAM)                            */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    |21.03.88 |  Original                      | HV |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/


/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

/*****  Operating System - Includes  *****/
#include <isam.h>

/*****  external Includes  *****/

/*****  internal Includes  *****/

#include <d26dsa.h>
#include <dce/d27util.h>
#include <d26isam.h>
#include <d26svcis.h>
#include <d26schema.h>
#include <malgcfe.h>

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
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_850_delete_obj(...)                             */
/*                                                                    */
/* AUTHOR       : Volpers,D AP 11                                     */
/* DATE         : 17.03.88                                            */
/*                                                                    */
/* SYNTAX       : Short d26_850_delete_obj(record, oclass,al_flag,    */
/*                                obj_idx, a_out, a_len, p_oct_entry) */
/*                String         record;                              */
/*                Long            oclass;                             */
/*                Bool            al_flag;                            */
/*                Short           obj_idx;                            */
/*                Char           *a_out;                              */
/*                Ushort         *a_len;                              */
/*                D26_oct        **p_oct_entry;                       */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                String         record;    record which will be      */
/*                                          deleted                   */
/*                Long           oclass ;   Index in oct              */
/*                Bool           al_flag;   alias flag                */
/*                Short          obj_idx;   index in SRT              */
/*                Ushort        *a_len;     maximum length of         */
/*                                          REMOBJ-Result-Message     */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                Char          *a_out;     REM_OBJECT-Result-Message */
/*                Ushort        *a_len;     length of REMOBJ          */
/*                                          Result-Message            */
/*                D26_oct       **p_oct_entry;                        */
/*                                                                    */
/* RETURN VALUE :  = D26_ERROR              if is_delete failed       */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                isdelete()                                          */
/*                d27_010_set_error_apdu                              */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*inoff ***************************************************************/

extern int isdelete(int isfd, String record);

signed16 d26_850_delete_obj
(
  String          record,
  signed32        oclass,
  boolean         al_flag,
  signed16        obj_idx,
  byte           *a_out,
  signed32       *a_len,
  D26_oct        **p_oct_entry
)

{                                   /*  d26_850_delete_obj()          */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

 static char function[] = "d26_850_delete_obj";
 signed16        ret_value = D2_NO_ERR;   /* return value         */
 signed16        file_nr;      /* file number of changed object   */
 int             isfd;         /* C-isam filedescriptor of file   */
			       /* which will be deleted           */
 signed16        oct_idx;
 D26_ocl_arc    *ocl_arc;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

				   /* get filedescriptor for isdelete */
if (al_flag == TRUE)
    {
				    /*  determine smallest filenr for */
				    /*  Alias object                  */
       ocl_arc = (d26_srt + obj_idx)->d26s_ocl_arc;
       oct_idx = ocl_arc->d26s_oclass;
       *p_oct_entry = d26_oct + oct_idx;

       file_nr = (*p_oct_entry)->d26c_filenr;
       ocl_arc++;
       for ( ; (oct_idx = ocl_arc->d26s_oclass) >= 0; ocl_arc++)
       { 
	 if ((d26_oct + oct_idx)->d26c_filenr < file_nr)
	 {   
	   *p_oct_entry = d26_oct + oct_idx;
	   file_nr = (*p_oct_entry)->d26c_filenr;
	 } 
       } 
    }else{
      *p_oct_entry = d26_oct + oclass;
      file_nr = d26_oct[oclass].d26c_filenr;
    }
isfd    = (d26_ofildes + file_nr)->d26_isfd;

if                                 /* delete record                   */
/*  (D26_ISDELETE(svc_c_sev_warning, isfd, record) < 0) */
  (isdelete(isfd, record) < 0)
{                                  /* MA_ERROR                        */
  DCE_SVC_LOG((DCE_SVC(gds_svc_handle, "%d"),GDS_S_GENERAL, svc_c_debug1,
	GDS_S_ISDELETE_ERR, iserrno));
  d27_010_set_error_apdu((D23_rserror *)a_out,D2_TEMP_ERR,D2_REM_ERR,
			 D2_UNWILLING,a_len);
  return(D26_ERROR);
} 
D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_DELETE,function,1,0);

return(ret_value);

}                                   /*  d26_850_delete_obj()         */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_851_del_rec   (...)                             */
/*                                                                    */
/* AUTHOR       : Volpers,D AP 11                                     */
/* DATE         : 29.03.88                                            */
/*                                                                    */
/* SYNTAX       : Short d26_851_del_rec(record, at_entry,oclass)      */
/*                                                                    */
/*                String         record;                              */
/*                D26_at        *at_entry;                            */
/*                Short          oclass;                              */
/*                                                                    */
/* DESCRIPTION: this function delete the recurring attributes if the  */
/*              function remove_object is called                      */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                String         record;    record which will be      */
/*                                          deleted                   */
/*                D26_at        *at_entry;    entry in at             */
/*                Long           oclass;    Index in oct              */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE :  = D26_ERROR              if SEARCH_Result-Message  */
/*                                          is too long               */
/*                 = err                    otherwise                 */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                  d26_i23_open_files()                              */
/*                  d26_i17_del_all_rec_vals()                        */
/* REMARKS :                                                          */
/*                                                                    */
/*inoff ***************************************************************/

signed16 d26_851_del_rec
(
  String          record,
  D26_at         *at_entry,
  signed32        oclass
)

{                                   /*  d26_851_del_rec()             */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

 static char function[] = "d26_851_del_rec";
 signed16        ret_value = D2_NO_ERR;   /* return value         */
 signed32        key;          /* record key                      */
 signed16        file_nr;      /* file number of deleted object   */
 int             isfd;         /* C-isam file descriptor          */
 signed16        anzval;       /* number of values of an attr     */
 char            *pos;         /* pointer to actual rec position  */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

pos = d26_i15_get_record_pos(record, 1, at_entry,&d26_oct[oclass],
			     (D26_nam_at *)NULL,TRUE);
anzval = ldint(pos);

if (anzval > 0)                    /* a rec attribute existed         */
{   
  key = ldlong (record);
				   /* get C_Isam file descriptor      */
  file_nr = at_entry->d26a_filenr;
  if                               /* open file                       */
    ((isfd = d26_i23_open_file (file_nr, D26_ATT)) < 0)
  {                                /* error                           */
    ret_value = D26_ERROR;
  }
else
{
				   /* delete attribute               */
    if
      (d26_i17_del_all_rec_vals (at_entry, key,
		       d26_oct[oclass].d26c_filenr,isfd, d26_a_record))
    {   
      ret_value = D26_ERROR;
    } 
    D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_DELETE,function,1,0);
  } 
} 

}                                   /*  d26_851_del_rec()             */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_852_upd_dnlist(...)                             */
/*                                                                    */
/* AUTHOR       : Volpers,D AP 11                                     */
/* DATE         : 29.03.88                                            */
/*                                                                    */
/* SYNTAX       : Short d26_852_upd_dnlist(record, at_entry,oclass,   */
/*                                         p_oct_entry, upd_time)     */
/*                String         record;                              */
/*                D26_at        *at_entry;                            */
/*                Long          oclass;                               */
/*                D26_oct        *p_oct_entry;                        */
/*                Bool           *upd_time;                           */
/*                                                                    */
/* DESCRIPTION: this function make the update of the DN list if the   */
/*              function remove_object is called                      */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                String         record;    record which will be      */
/*                                          deleted                   */
/*                D26_at        *at_entry; entry in at                */
/*                Long          oclass;                               */
/*                D26_oct        *p_oct_entry;                        */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                Bool          *upd_time;  is set to D2_TRUE when    */
/*                                          an update to dnlist in    */
/*                                          file was done             */
/*                                          NOTE that the variable    */
/*                                          must be initialised with  */
/*                                          D2_FALSE                  */
/*                                                                    */
/* RETURN VALUE :  = D26_ERROR              if SEARCH_Result-Message  */
/*                                          is too long               */
/*                 = err                    otherwise                 */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                  d26_i15_get_recor_pos()                           */
/*                  d26_i02_upd_file_dnlist()                         */
/* REMARKS :                                                          */
/*                                                                    */
/*inoff ***************************************************************/

signed16 d26_852_upd_dnlist
(
  String          record,
  D26_at         *at_entry,
  signed32        oclass,
  D26_oct        *p_oct_entry,
  boolean        *upd_time
)

{                                   /*  d26_852_upd_dnlist()          */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

 static char function[] = "d26_852_upd_dnlist";
 signed16           ret_value = D2_NO_ERR;   /* return value         */
 signed16           i;
 signed16           anz_vals;     /* number of values of an attr     */
 char               *pos;         /* pointer to actual rec position  */
 signed32           dnl_idx;      /* index value to dn list in core  */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

				   /* set needed variables            */


pos = d26_i15_get_record_pos(record, 1, at_entry,p_oct_entry,
			     (D26_nam_at *)NULL,TRUE);

anz_vals = ldint(pos);
				   /* all old values                  */
for (i = 1; i <= anz_vals; i++)
{                                  /* get record position             */
  pos = d26_i15_get_record_pos(record, i, at_entry,p_oct_entry,
			       (D26_nam_at *)NULL,FALSE);
  dnl_idx = ldlong(pos);

  if (dnl_idx >= 0)
  {   
    *upd_time = TRUE;
    if
      (d26_i02_upd_file_dnlist (D2_DEL_AV,(D2_name_string)NULL, D2_IGNORE,
			       (D2_name_string)NULL, 1,FALSE,&dnl_idx))
    {   
      ret_value = D26_ERROR;
    } 
  } 
} 
D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_DELETE,function,1,0);

}                                   /*  d26_852_upd_dnlist()          */
