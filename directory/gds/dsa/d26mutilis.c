/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26mutilis.c,v $
 * Revision 1.1.12.2  1996/02/18  19:45:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:00  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:46:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:27  root]
 * 
 * Revision 1.1.10.5  1994/10/05  15:39:14  keutel
 * 	OT 12481: multiple routing
 * 	[1994/10/05  14:36:52  keutel]
 * 
 * Revision 1.1.10.4  1994/06/21  14:45:41  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:33:01  marrek]
 * 
 * Revision 1.1.10.3  1994/05/10  15:52:30  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:22:56  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  15:09:32  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:21:38  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  16:06:17  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:15:51  marrek]
 * 
 * Revision 1.1.8.1  1993/10/29  15:11:11  keutel
 * 	CR 9261
 * 	[1993/10/29  14:05:22  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  14:25:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:14:04  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:41:24  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:20:23  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:48:16  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:14:22  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:51:03  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:57:10  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26mutilis.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 19:45:41 $";
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
/* NAME         : d26mutilis.c       [mutilis]                        */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 27.12.88                                            */
/*                                                                    */
/* COMPONENT    : DSA, retrieval functions                            */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   The module contains the CISAM utilities used by   */
/*                  the DSA main function.                            */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 27.12.88| birth                          | ek |         */
/*          | 02.12.90| Update for new scheme format   | mcj|         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <isam.h>

/*****  external Includes    *****/

#include <dce/d27util.h>
#include <d26svcis.h>

/*****  internal Includes    *****/

#include <d26schema.h>
#include <d26isam.h>

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
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_mi1_call_read_scheme                            */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 21.12.87                                            */
/*                                                                    */
/* DESCRIPTION  : The Function calls another function to read in the  */
/*                internal schema into global variables. This         */
/*                intermediate function is necessary only, that the   */
/*                interface of this called function could be changed, */
/*                to enable the transfer program to use it too.       */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       signed16        No error occurred             */
/*         -1                           System error occurred         */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_mi1_call_read_scheme(void)

{                /*  d26_mi1_call_read_scheme                         */
			    /* type of object class attribute         */
  static D2_a_type obcl_type = {sizeof(D2_OBJ_CLASS) - 1,
			       (Ob_id_string)D2_OBJ_CLASS};
			    /* type of aliased object attribute       */
  static D2_a_type alob_type = {sizeof(D2_ALIASED_OBJECT) - 1,
			       (Ob_id_string)D2_ALIASED_OBJECT};
			    /* type of master knowledge attribute     */
  static D2_a_type mk_type =   {sizeof(D2_MASTER_KNOWLEDGE) - 1,
			       (Ob_id_string)D2_MASTER_KNOWLEDGE};
			    /* type of ACL attribute                  */
  static D2_a_type acl_type =  {sizeof(D2_ACL) - 1,(Ob_id_string)D2_ACL};
			    /* type of User-Password attribute        */
  static D2_a_type pwd_type =  {sizeof(D2_PWD) - 1,(Ob_id_string)D2_PWD};
			    /* type of PSAP-address attribute         */
  static D2_a_type psap_type = {sizeof(D2_PSAP_ADDRESS) - 1,
			      (Ob_id_string)D2_PSAP_ADDRESS};
			    /* type of DN-List-attribute              */
  static D2_a_type dnl_type =  {sizeof(D2_DNLIST) - 1,
			       (Ob_id_string)D2_DNLIST};
			    /* type of Auth.-Method-attribute         */
  static D2_a_type am_type =  {sizeof(D2_AUTH_MECH) -1,
			       (Ob_id_string)D2_AUTH_MECH};
			    /* type of Principal-Name-attribute       */
  static D2_a_type pn_type =  {sizeof(D2_PR_NM) -1,
			       (Ob_id_string)D2_PR_NM};

			    /* object identifier of alias class       */
  static D2_obj_id al_class  = {sizeof(D2_OALIAS) - 1,
			       (Ob_id_string)D2_OALIAS};
			    /* object identifier of DSA class         */
  static D2_obj_id dsa_class = {sizeof(D2_ODSA) - 1,(Ob_id_string)D2_ODSA};

  D2_obj_id soundex_obid;   /* identifiers for soundex algorithms     */
  D2_obj_id *linked_soundex_obid;
			    /* identifiers in printable format        */
  char      pr_se_obid[D2_OBJ_IDL_MAX];
  char      pr_lse_obid[D2_OBJ_IDL_MAX];



  if            /* error from reading scheme file                     */
    (d26_mi9_read_scheme(d26_install,&d26_nosrt,&d26_srt,&d26_nam_at,
     &d26_name_pos,&d26_ocl_arc,&d26_nooct,&d26_oct,&d26_oct_idx,&d26_noat,
     &d26_at,&d26_at_pos,&d26_obid_str,&d26_nofile,&d26_ofildes,&d26_oreclen,
     &d26_ob_record,&d26_nafile,&d26_afildes,&d26_areclen,&d26_a_record,
     d27_country_bitlist,&soundex_obid) < 0)
  {             /* return error                                       */
    return(D26_ERROR);
  }
  else if       /* no memory for attribute informations and superior  */
		/* node record available                              */
	 ((d26_attr = (D26_attr_info *)D27_CALLOC(svc_c_sev_fatal,
	   (unsigned)((d26_noat > D2_AT_MAX ? d26_noat : D2_AT_MAX) + 1),
	   sizeof(D26_attr_info))) == NULL ||
	  (d26_ate_used = (boolean *)D27_CALLOC(svc_c_sev_fatal,d26_noat,
	   sizeof(boolean))) == NULL ||
	  (d26_ofile_searched = (boolean *)D27_CALLOC(svc_c_sev_fatal,
	   d26_nofile,sizeof(boolean))) == NULL ||
	  (d26_sn_record = (char *)D27_MALLOC(svc_c_sev_fatal,d26_oreclen))
	   == NULL)
  {             /* return error                                       */
    return(D26_ERROR);
  }
  else if       /* operational attribute or object class not known    */
	 ((d26_obcl_idx = d26_u03_attr_search(&obcl_type)) < 0      ||
	  (d26_alob_idx = d26_u03_attr_search(&alob_type)) < 0      ||
	  (d26_mk_idx   = d26_u03_attr_search(&mk_type)) < 0        ||
	  (d26_acl_idx  = d26_u03_attr_search(&acl_type)) < 0       ||
	  (d26_pwd_idx  = d26_u03_attr_search(&pwd_type)) < 0       ||
	  (d26_psap_idx = d26_u03_attr_search(&psap_type)) < 0      ||
	  (d26_dnl_idx  = d26_u03_attr_search(&dnl_type)) < 0       ||
	  (d26_am_idx   = d26_u03_attr_search(&am_type)) < 0        ||
	  (d26_pn_idx   = d26_u03_attr_search(&pn_type)) < 0        ||
	  (d26_ali_idx  = d26_u14_get_obj_class_idx(&al_class)) < 0 ||
	  (d26_dsa_idx  = d26_u14_get_obj_class_idx(&dsa_class)) < 0)
  {             /* return error                                       */
    dce_svc_printf(GDS_S_OPAT_MISSING_MSG);
    return(D26_ERROR);
  }
  else
  {             /* check object identifiers for soundex algorithms    */
    linked_soundex_obid = d26_soundex(NULL,0,NULL);
    if          /* soundex algorithms differ                          */
      (D27_ATTCMP(linked_soundex_obid,&soundex_obid))
    {           /* issue a warning                                    */
      d27_301_oid_str(linked_soundex_obid,pr_lse_obid);
      d27_301_oid_str(&soundex_obid,pr_se_obid);
      dce_svc_printf(GDS_S_BAD_SOUNDEX_MSG,pr_lse_obid,pr_se_obid);
    }
  }
		/* return no error                                    */
  return(D2_NO_ERR);

}               /* d26_mi1_call_read_scheme                           */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_mi2_free                                          */
/*                                                                    */
/* AUTHOR:      Kraemer D AP 11                                       */
/* DATE:        27.12.88                                              */
/*                                                                    */
/* DESCRIPTION: The function frees all memory CISAM specific memory   */
/*              allocated before.                                     */
/*                                                                    */
/* RETURN VALUE :       None                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d26_mi2_free(void)

{                       /* d26_mi2_free                               */

  if                    /* memory for object file description alloc.  */
    (d26_ofildes != NULL)
  {                     /* check records                              */
    free(d26_ofildes);
  }
  if                    /* memory for object record is allocated      */
    (d26_ob_record != NULL)
  {                     /* free memory                                */
    free(d26_ob_record);
  }
  if                    /* memory for object record is allocated      */
    (d26_sn_record != NULL)
  {                     /* free memory                                */
    free(d26_sn_record);
  }
  if                    /* memory for attribute file description all. */
    (d26_afildes != NULL)
  {                     /* free memory                                */
    free(d26_afildes);
  }
  if                    /* memory for attribute record is allocated   */
    (d26_a_record != NULL)
  {                     /* free memory                                */
    free(d26_a_record);
  }

}                       /* d26_mi2_free                               */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_mi3_open_files                                    */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        19.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function opens all required isam files.          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     signed16  D2_NO_ERR      no error occurred                     */
/*               D26_ERROR      C-isam error occurred                 */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_mi3_open_files(void)

{                       /* d26_mi3_open_files                         */

  signed16 openfiles;   /* number of open isam files                  */
			/* Isam filename                              */
  char     filename[D27_LFILE_NAME];
  signed16 ret_value = D2_NO_ERR;

  register signed16 i;  /* loop variable                              */

  for                   /* all object filenumbers                     */
     (i = openfiles = 0; i < d26_nofile && openfiles < D26_MOPENFILES; i++)
  {                     /* write file name                            */
    sprintf(filename,"%s%s%d",d26_install,D26_OBJECTS,i);
    if                  /* file is needed                             */
      (d26_ofildes[i].d26_rlen > 0)
    {                   /* try to open ISAM file                      */
      if                /* error from open isam                       */
	((d26_ofildes[i].d26_isfd = D26_ISOPEN(svc_c_sev_fatal,filename,
	 ISINOUT + ISMANULOCK)) < 0)
      {                 /* write error log                            */
	ret_value =  D26_ERROR;
      }
      else
      {                 /* increment count of open isam files         */
	openfiles++;
      }
    }
  }

  for                   /* all attribute files                        */
     (i = 0; i < d26_nafile && openfiles < D26_MOPENFILES; i++)
  {                     /* write file name                            */
    sprintf(filename,"%s%s%d",d26_install,D26_ATTR,i);
    if                  /* error from open isam                       */
    ((d26_afildes[i].d26_isfd = D26_ISOPEN(svc_c_sev_fatal,filename,
     ISINOUT + ISMANULOCK)) < 0)
    {                   /* write error log                            */
      ret_value =  D26_ERROR;
    }
    else
    {                   /*  enhigher count of open isam files         */
      openfiles++;
    }
  }

  return(ret_value);

}                       /* d26_mi3_open_files                         */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_mi4_close_files()                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        21.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function closes all open isam files.             */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Short   D2_NO_ERR        no error occurred                     */
/*             D26_ERROR        C-isam error occurred                 */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_mi4_close_files(void)

{                       /* d26_mi4_close_files                        */

  signed16 ret_value = D2_NO_ERR;

  register signed16 i;  /* loop variable                              */

  for                   /* all object filenumbers                     */
     (i = 0; i < d26_nofile; i++)
  {                     /* check file descriptor                      */
    if                  /* file is open                               */
      (d26_ofildes[i].d26_isfd >= 0)
    {                   /* close file                                 */
      if                /* error from close file                      */
	(D26_ISCLOSE(svc_c_sev_warning,d26_ofildes[i].d26_isfd) < 0)
      {                 /* set return value                           */
	ret_value =  D26_ERROR;
      }
      else
      {                 /* set isam file descriptor -1                */
	d26_ofildes[i].d26_isfd = -1;
      }
    }
  }

  for                   /* all attribute filenumbers                  */
     (i = 0; i < d26_nafile; i++)
  {                     /* check file descriptor                      */
    if                  /* file is open                               */
      (d26_afildes[i].d26_isfd >= 0)
    {                   /* close file                                 */
      if                /* error from close file                      */
	(D26_ISCLOSE(svc_c_sev_warning,d26_afildes[i].d26_isfd) < 0)
      {                 /* set return value                           */
	ret_value =  D26_ERROR;
      }
      else
      {                 /* set isam file descriptor -1                */
	d26_afildes[i].d26_isfd = -1;
      }
    }
  }

  return(ret_value);

}                       /* d26_mi4_close_files                        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_mi6_save_schema_obj                               */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        05.11.91                                              */
/*                                                                    */
/* DESCRIPTION: This function copies all C-Isam-files, that contain   */
/*              the schema object into chdb.                          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     signed16  D2_NO_ERR      no error occurred                     */
/*               D26_ERROR      any system error occurred             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_mi6_save_schema_obj(void)

{                       /* d26_mi6_save_schema_obj                    */

			/* name of chdb directory                     */
  char filename[D27_LFILE_NAME];
			/* space for copy command                     */
  char cp_command[5 + 5 * D27_LFILE_NAME];
  int   err;            /* error from system call                     */

  sprintf(filename,"%s%s",d26_install,D26_CHDB_DIR);
  sprintf(cp_command,"cp %s%s0.* %s%s[012].* %s%s %s%s %s",
	  d26_install,D26_OBJECTS,
	  d26_install,D26_ATTR,
	  d26_install,D27_DSA_CONF,
	  d26_install,D27_DSA_D_VERS,
	  filename);

  if                    /* copy command cannot be executed            */
     ((err = D27_SYSTEM(svc_c_sev_warning,cp_command)) != 0)
  {                     /* log error and return                       */
    return(D26_ERROR);
  }

  return(D2_NO_ERR);

}                       /* d26_mi6_save_schema_obj                    */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_mi7_clean_chdb                                    */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        05.11.91                                              */
/*                                                                    */
/* DESCRIPTION: This function cleans the chdb directory if there was  */
/*              an error during the schema modification.              */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   signed16    no error occurred                     */
/*      D26_ERROR               any system error occurred             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_mi7_clean_chdb(void)

{                       /* d26_mi7_clean_chdb                         */

			/* name of chdb directory                     */
  char filename[D27_LFILE_NAME];
			/* space for copy command                     */
  char  rm_command[20 + 2 * D27_LFILE_NAME];
  int   err;            /* error from system call                     */

  sprintf(filename,"%s%s",d26_install,D26_CHDB_DIR);
  sprintf(rm_command,"ls %s | xargs -i rm %s/{}",filename,filename);

  if                    /* copy command cannot be executed            */
    ((err = D27_SYSTEM(svc_c_sev_warning,rm_command)) != 0)
  {                     /* log error and return                       */
    return(D26_ERROR);
  }

  return(D2_NO_ERR);

}                       /* d26_mi7_clean_chdb                         */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_mi8_remove_saved_schema                           */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        02.12.91                                              */
/*                                                                    */
/* DESCRIPTION: This function removes all files and the directory of  */
/*              the saved schema object.                              */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     signed16  D2_NO_ERR      no error occurred                     */
/*               D26_ERROR      any system error occurred             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_mi8_remove_saved_schema(void)

{                       /* d26_mi8_remove_saved_schema                */
			/* space for copy command                     */
  char rm_command[5 + 3 * D27_LFILE_NAME];
  int  err;             /* error from system call                     */

  sprintf(rm_command,"rm -r %s/%s",d26_install,D26_CHDB_DIR);

  if                    /* remove command cannot be executed          */
    ((err = D27_SYSTEM(svc_c_sev_warning,rm_command)) != 0)
  {                     /* log error and return                       */
    return(D26_ERROR);
  }

  return(D2_NO_ERR);

}                       /* d26_mi8_remove_saved_schema                */
