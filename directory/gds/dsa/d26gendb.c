/* 
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * HISTORY
 * $Log: d26gendb.c,v $
 * Revision 1.1.722.2  1996/02/18  19:45:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:29  marty]
 *
 * Revision 1.1.722.1  1995/12/08  15:44:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:54  root]
 * 
 * Revision 1.1.720.5  1994/10/05  15:39:02  keutel
 * 	OT 12481: multiple routing
 * 	[1994/10/05  14:36:04  keutel]
 * 
 * Revision 1.1.720.4  1994/06/21  14:45:25  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:53:17  marrek]
 * 
 * Revision 1.1.720.3  1994/05/10  15:52:12  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/04  14:25:03  marrek]
 * 
 * 	Bug fixes in April 1994 submission.
 * 	[1994/05/03  16:40:59  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:21:44  marrek]
 * 
 * Revision 1.1.720.2  1994/03/23  15:09:20  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:21:03  keutel]
 * 
 * Revision 1.1.720.1  1994/02/22  16:05:45  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:12:55  marrek]
 * 
 * Revision 1.1.718.2  1993/10/14  17:16:26  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:09:56  keutel]
 * 
 * Revision 1.1.718.1  1993/10/13  17:30:39  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:27:28  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  14:06:50  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:09:19  marrek]
 * 
 * Revision 1.1.4.7  1993/02/02  14:47:12  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.6  1993/02/02  11:20:15  marrek
 * 	Telephone number component of FAX number is checked
 * 	[1993/02/01  14:54:05  marrek]
 * 
 * Revision 1.1.4.5  1992/12/31  19:40:14  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:19:35  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/17  23:12:47  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:08:07  tom]
 * 
 * Revision 1.1.4.3  1992/11/27  19:44:17  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:11:26  marrek]
 * 
 * Revision 1.1.4.2  1992/09/22  08:23:25  marrek
 * 	Rename abort to abort_msg (Bug #4935).
 * 	[1992/09/22  08:16:04  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:56:05  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:35:13  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:49:17  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:55:12  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26gendb.c,v $ $Revision: 1.1.722.2 $ $Date: 1996/02/18 19:45:16 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/**********************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : d26gendb.c                                          */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 21.04.88                                            */
/* UPDATE       : 15.11.90  by Melissa        (design E. Kraemer)     */
/*                                                                    */
/* SYNTAX       : gendb <dir> <dirlog>                                */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  : In the specified directory the schema information   */
/*                is read from the database.                          */
/*                The information consists of rules for generating    */
/*                valid distinguished names (SRT), the relationship   */
/*                between object classes and which mandatory and      */
/*                optional attributes exist in each class (OCT), and  */
/*                the characteristics of each attribute (AT).         */
/*                                                                    */
/*                Gendb creates an empty database to be used by the   */
/*                DSA for storing object data.                        */
/*                Finally, a binary file called "scheme" is generated */
/*                which holds the SRT, OCT, AT and other information. */
/*                This file will be read by the DSA at boot time.     */
/*                                                                    */
/* INPUT-PARAMETER:     dir  -  relative or full directory name       */
/*                      dirlog  pathname for log-files                */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*                                                                    */
/* RETURNVALUE     : -                                                */
/*                                                                    */
/* STANDARDHEADER  : -                                                */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (changed):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                                                                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                    I N C L U D E S                                 */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>
#include <dce/dcesvcmsg.h>

/*****  Operating-System-Includes  *****/

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <locale.h>

/*****  external Includes    *****/

#include <d2shm.h>              /* Scheme definitions                 */
#include <dce/d27util.h>

/*****  internal Includes    *****/

#include <d26schema.h>
#include <d26dnlis.h>
#include <d26gendb.h>

/**********************************************************************/
/*                                                                    */
/*                    D E F I N E S                                   */
/*                                                                    */
/**********************************************************************/

#define D26_TIME_LBOUND 11      /* upper bound ...                    */
#define D26_TIME_UBOUND (D26_TLEN - 1)  /* lower bound ...            */
				/* ... of time stamp attribute        */

/**********************************************************************/
/*                                                                    */
/*                    T Y P E D E F S                                 */
/*                                                                    */
/**********************************************************************/

typedef struct                  /* File number sorting element       */
	{
	    signed16 seq_nr;
	    signed16 value;
	} Fnr_elem;

/**********************************************************************/
/*                                                                    */
/*                    D E C L A R A T I O N S    DATA                 */
/*                                                                    */
/**********************************************************************/

/******** EXPORT ********/

char    dir_fname[D27_LINST_NAME];  /* Name of directory for database */

signed16 nr_adm_srt;                /* Number of admin_srt entries    */

signed16 nr_oc_indices;             /* No of superclass and auxiliary */
				    /* class indices                  */
signed16 nr_oct_indices;            /* No of indices contained in OCT */

signed16 nr_na_indices;             /* No of SRT naming attr.-indices */
signed16 nr_cl_indices;             /* No of object class arcs        */

signed16 *level_pos;                /* array of shorts which keeps the*/
				    /* position of every attribute in */
				    /* every level in the name in     */
				    /* every object file              */

signed16 *d26_name_pos;             /* Array of positions of naming   */
				    /* attributes in each object file */
signed16 *d26_at_pos;               /* array of positions of other    */
				    /* attributes in each object file */
Admin_srt *admin_srt;           /* Contents of SRT info in attr0 file */
Admin_oct *admin_oct;           /* Contents of OCT info in attr1 file */
Admin_at  *admin_at;            /* Contents of AT info in attr2 file  */

/******** LOCAL  ********/

static signed16  nr_at_indices;     /* No of OCT mand + opt AT indices*/

			/* An array with space for a octal object id  */
			/* string for each OCT entry.                 */
static byte      *oct_oids;
			/* An array with space for a octal object id  */
			/* string for each AT entry.                  */
static byte      *at_oids;

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/

static signed16   d26_g01_strip_oct_at(void);
static signed16   d26_g02_proc_oct_oids(void);
static signed16   d26_g03_setup_oct(void);
static signed16   d26_g04_proc_at_oids(void);
static signed16   d26_g05_proc_srt_names(void);
static signed16   d26_g06_setup_srt(void);
static signed16   d26_g07_setup_at(signed16 max_len);
static signed16   d26_g08_calc_class_idx(void);
static signed16   d26_g09_calc_attr_idx(void);
static signed16   d26_g10_write_attr_table(void);
static void       d26_g11_set_oct_usage(Admin_oct *adm_octp);
static signed16   d26_g12_calc_total_atidx(Admin_oct *adm_octp);
static signed16   d26_g13_atacr_to_idx(char *acronym);
static signed16   d26_g14_ocacr_to_idx(char *acronym);
static signed16   d26_g15_mand_list(Admin_oct *adm_octp, void **idx_root,
    signed16 **mand_list);
static signed16   d26_g16_opt_list(Admin_oct *adm_octp, void **idx_root,
    signed16 **opt_list);

static int        d26_g21_cmp_srt(const void *srt1, const void *srt2);
static int        d26_g22_cmp_oct(const void *oct1, const void *oct2);
static int        d26_g23_cmp_at(const void *at1, const void *at2);
static int        d26_g24_cmp_filenr(const void *fnr1, const void *fnr2);
static int        d26_g25_cmp_naming_types(const void *acr1,
    const void *acr2);
static int        d26_g26_cmp_ocl_arcs(const void *arc1, const void *arc2);
static int        d26_g27_cmp_idx(const void *idx1, const void *idx2);

static boolean    d26_g31_cmp_name_str(Admin_srt *adm_srtp1,
    Admin_srt *adm_srtp2);

static signed16   d26_g41_read_srt_entry(char *srt_value, signed16 srt_idx);
static signed16   d26_g42_read_oct_entry(char *oct_value, signed16 oct_idx);
static signed16   d26_g43_read_at_entry(char *at_value, signed16 at_idx);

static char *d26_g51_skip_table_elements(char *tab_entry, signed16 elements);

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : main                                                */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 11.04.88                                            */
/* UPDATE       : 10.10.90  by Melissa                                */
/*                                                                    */
/* DESCRIPTION  : The program generates an empty database as specified*/
/*                in the schema object.                               */
/*                                                                    */
/* RETURNVALUE     : -                                                */
/*                                                                    */
/**********************************************************************/

int main(
    int argc,           /* IN    -  count of command line parameters  */
    char *argv[])       /* IN    -  command line parameters           */

{                       /* main                                       */

			/* attribute type of DN-List attribute        */
  static D2_a_type oid = {sizeof(D2_DNLIST) - 1,(Ob_id_string)D2_DNLIST};

  FILE     *fp;         /* filepointer for general use                */
			/* filename for general use                   */
  char     filename[D27_LFILE_NAME];

  int      data_version;/* data version number                        */
			/* maximum length of one attribute value      */
  short    max_attr_val_len;

  long     recnum;         /* record number of schema object             */
			/* timestamp of schema object                 */
  char     schema_time[D26_TLEN];

  Admin_at *dnlatp;     /* pointer to DN-list entry in administrated  */
			/* attribute table                            */

  error_status_t st;    /* error status for serviceability functions  */

  setlocale(LC_ALL, "");
  if                    /* parameter is missing                       */
    (argc < 3)
  {                     /* display usage message and exit             */
    fprintf(stderr,"usage: %s <directory-name> <logfile directory>\n",
      argv[0]);
    exit(1);
  }
  else if               /* working directory name is too long         */
	 ((int)strlen(argv[1]) > D27_LINST_NAME)
  {                     /* display error message and exit             */
    fprintf(stderr,"directory name is longer than %d characters\n",
      D27_LINST_NAME);
    exit(1);
  }

  (void)strcpy(dir_fname,argv[1]);
  if                    /* working directory cannot be changed        */
    (chdir(argv[2]) < 0)
  {                     /* give error note and exit                   */
    fprintf(stderr,"Unable to chdir to %s, errno: %d\n",argv[2],errno);
    exit(1);
  }

  d27_001_svc_init(argc,argv,&st);
  if                    /* serviceability cannot be initialized       */
    (st != svc_s_ok)
  {                     /* give error note                            */
    fprintf(stderr,"Unable to initialize serviceability, status: 0x%lx\n",
      st);
  }
  dce_svc_printf(GDS_S_ENTRY_GENDB_MSG);

  sprintf(filename,"%s%s",dir_fname,D27_DSA_D_VERS);
  if                    /* data versions do not match                */
    (d26_u17_check_data_version(filename,&data_version) != D2_NO_ERR)
  {                     /* error exit                                */
    dce_svc_printf(DCE_SVC(gds_svc_handle,""),GDS_S_GENERAL,svc_c_sev_error,
       GDS_S_EXIT_GENDB);
    exit(1);
  }
			/* Open file "conf"                          */
  sprintf(filename,"%s%s",dir_fname,D27_DSA_CONF);
  if                    /* configuration file cannot be opened       */
    ((fp = D27_FOPEN(svc_c_sev_fatal,filename,"r")) == NULL)
  {                     /* error exit                                */
    exit(1);
  }

  if                    /* configuration data cannot be read          */
    (d26_gi1_read_configuration_data(fp,&max_attr_val_len) != D2_NOERROR)
  {                     /* error exit                                 */
    exit(1);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_CONF_MAX_AVLEN_MSG,max_attr_val_len));
  }

			/* function d27_609_compute_cntry_bitlist     */
			/* (Assign d27_country_bitlist from file      */
			/* conf)                                      */

  if                    /* country bitlist cannot be read             */
    (d27_609_compute_cntry_bitlist(fp) != D2_NOERROR)
  {                     /* error exit                                 */
    dce_svc_printf(GDS_S_CBL_ERR_MSG);
    exit(1);
  }
  else                  /* log success                                */
  {
    DCE_SVC_LOG((GDS_S_CBL_READ_MSG));
  }

  if                    /* configuration file cannot be closed        */
    (D27_FCLOSE(svc_c_sev_fatal,fp) < 0)
  {                     /* error exit                                 */
    exit(1);
  }

  if                    /* schema object cannot be read               */
    (d26_gi2_read_schema_object(&recnum,schema_time) != D2_NOERROR)
  {                     /* error exit                                 */
    exit(1);
  }
  DCE_SVC_LOG((GDS_S_SCHEMA_READ_MSG));

  if                    /* memory for SRT cannot be allocated         */
    ((admin_srt = (Admin_srt *)D27_CALLOC(svc_c_sev_fatal,nr_adm_srt,
     sizeof(Admin_srt))) == NULL ||
     (d26_srt = (D26_srt *)D27_CALLOC(svc_c_sev_fatal,nr_adm_srt,
      sizeof(D26_srt))) == NULL)
  {                     /* error exit                                 */
    exit(1);
  }

  if                    /* SRT attribute cannot be read               */
    (d26_gi3_read_attribute_table(recnum,0,nr_adm_srt,d26_g41_read_srt_entry)
     != D2_NOERROR)
  {                     /* error exit                                 */
    exit(1);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_SRT_READ_MSG));
  }

  if                    /* memory for OCT cannot be allocated         */
    ((admin_oct = (Admin_oct *)D27_CALLOC(svc_c_sev_fatal,d26_nooct,
     sizeof(Admin_oct))) == NULL ||
     (d26_oct = (D26_oct *)D27_CALLOC(svc_c_sev_fatal,d26_nooct,
     sizeof(D26_oct))) == NULL ||
     (oct_oids = (byte *)D27_CALLOC(svc_c_sev_fatal,d26_nooct,
      D2_OBJ_IDL_MAX + 1)) == NULL)
  {                     /* error exit                                 */
    exit(1);
  }

  if                    /* OCT attribute cannot be read               */
    (d26_gi3_read_attribute_table(recnum,1,(signed16)d26_nooct,
     d26_g42_read_oct_entry) != D2_NOERROR)
  {                     /* error exit                                 */
    exit(1);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_OCT_READ_MSG));
  }

  if                    /* memory for AT cannot be read              */
    ((admin_at = (Admin_at *)D27_CALLOC(svc_c_sev_fatal,d26_noat,
      sizeof(Admin_at))) == NULL ||
     (d26_at = (D26_at *)D27_CALLOC(svc_c_sev_fatal,d26_noat,sizeof(D26_at)))
      == NULL ||
     (at_oids = (byte *)D27_CALLOC(svc_c_sev_fatal,d26_noat,
      D2_OBJ_IDL_MAX + 1)) == NULL)
  {                     /* error exit                                 */
    exit(1);
  }

  if                    /* AT attribute cannot be read                */
    (d26_gi3_read_attribute_table(recnum,2,(signed16)d26_noat - 1,
     d26_g43_read_at_entry) != D2_NOERROR)
  {                     /* error exit                                 */
    exit(1);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_AT_READ_MSG));
			  /* Create an extra attribute, (DN list)       */
    dnlatp = admin_at + d26_noat - 1;
    strcpy(dnlatp->acronym,"DNL");
    d27_301_oid_str(&oid,dnlatp->obj_id);
    (void)strcpy(dnlatp->name,"Distinguished-Name-List");
    dnlatp->lbound      = 1;
    dnlatp->ubound      = D2_DNL_MAX;
    dnlatp->nr_recur    = 0;
    dnlatp->syntax      = D2_DISTNAME;
    dnlatp->phon        = FALSE;
    dnlatp->acl         = D26_PUBLIC;
    dnlatp->index_level = 0;
  }

  if                    /* error detected during stripping admin_oct  */
			/* and admin_at                               */
    (d26_g01_strip_oct_at() != D2_NO_ERR)
  {                     /* error exit                                 */
    exit(1);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_OCT_AT_STRIPPED_MSG));
  }

			/* Convert printable object ids to octal.     */
			/* Sort 'd26_oct' in ascending object id      */
			/* order. Link 'admin_oct' and 'd26_oct'      */
			/* entries.                                   */

  if                    /* error in processing the admin_oct          */
    (d26_g02_proc_oct_oids() != D2_NO_ERR)
  {                     /* error exit                                 */
    exit(1);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_OCT_PROCESSED_MSG));
  }

			/* calculate totals of OCT indices, remove    */
			/* the gaps from the file numbers and reset   */
			/* in the OCT.                                */

  if                    /* error from setting up OCT                  */
    (d26_g03_setup_oct() != D2_NO_ERR)
  {                     /* error exit                                 */
    exit(1);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_OCT_SETUP_MSG,d26_nofile));
  }


			/* Convert printable object ids to octal.     */
			/* Sort 'AT' in ascending object id order.    */
			/* Link 'admin_at' and 'AT' entries.          */

  if                    /* error from processing Admin_at             */
    (d26_g04_proc_at_oids() != D2_NO_ERR)
  {                     /* error exit                                 */
    exit(1);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_AT_PROCESSED_MSG));
  }

			/* Convert naming attribute acronyms to AT-   */
			/* indices and write them into the SRT.       */
			/* Sort d26_srt in ascending naming index     */
			/* order.                                     */
			/* Link admin_srt and d26_srt entries.        */

  if                    /* error from processing admin_srt            */
    (d26_g05_proc_srt_names() != D2_NO_ERR)
  {                     /* error exit                                 */
    exit(1);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_SRT_PROCESSED_MSG));
  }

			/* calculate the parent object numbers in SRT */
  if                    /* error from setting up SRT                  */
    (d26_g06_setup_srt() != D2_NO_ERR)
  {                     /* error exit                                 */
    exit(1);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_SRT_SETUP_MSG));
  }

			/* copy or calculate the content of the AT    */
			/* entries from admin_at entries. Decide if   */
			/* attributes are held in objects or attrib.  */
			/* file. Must be called after                 */
			/* d26_g05_proc_srt_names                     */

  if                    /* error from setting up AT                   */
    (d26_g07_setup_at(max_attr_val_len) != D2_NO_ERR)
  {                     /* error exit                                 */
    exit(1);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_AT_SETUP_MSG));
  }

  if                    /* error from allocating memory for object    */
			/* class arcs                                 */
    ((d26_ocl_arc = (D26_ocl_arc *)D27_CALLOC(svc_c_sev_fatal,
     (size_t)(2 * nr_adm_srt),sizeof(D26_ocl_arc))) == NULL)
  {                     /* error exit                                 */
    exit(1);
  }

			/* Set the object class arcs in SRT from the  */
			/* acronyms in admin_srt.                     */

  if                    /* error from setting up object class arcs    */
    (d26_g08_calc_class_idx() != D2_NO_ERR)
  {                     /* error exit                                 */
    exit(1);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_SRT_OCLARCS_READY_MSG));
  }

  if                    /* not enough memory for list of indices      */
    ((d26_oct_idx = (signed16 *)D27_CALLOC(svc_c_sev_fatal,
     (size_t)(nr_at_indices + nr_oc_indices),sizeof(signed16))) == NULL)
  {                     /* error exit                                 */
    exit(1);
  }

			/* Replace list of acronyms for superclasses */
			/* for auxiliary object classes, for mand.   */
			/* and optional attributes with list of      */
			/* indices to OCT and AT respectively.       */

  if                    /* error from calculating OCT and AT indices */
    (d26_g09_calc_attr_idx() != D2_NO_ERR)
  {                     /* error exit                                 */
    exit(1);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_OCT_INDICES_READY_MSG));
  }

  if                    /* not enough memory for attribute lengths,   */
			/* position arrays and file descriptors       */
    ((d26_name_pos = (signed16 *)D27_CALLOC(svc_c_sev_fatal,
      nr_na_indices * d26_nofile,sizeof(signed16))) == NULL ||
     (d26_at_pos   = (signed16 *)D27_CALLOC(svc_c_sev_fatal,
      d26_noat * d26_nofile,sizeof(signed16))) == NULL ||
     (level_pos = (signed16 *)D27_CALLOC(svc_c_sev_fatal,
      D2_NP_MAX * d26_noat,sizeof(signed16))) == NULL ||
     d26_gi6_fildes_alloc() != D2_NOERROR)
  {                     /* error exit                                 */
    exit(1);
  }

  if                    /* database cannot be created                 */
    (d26_gi8_create_database(schema_time) != D2_NOERROR)
  {                     /* error exit                                 */
    exit(1);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_DB_GENERATED_MSG));
  }

  if                    /* attribute mapping table cannot be generated*/
    (d26_g10_write_attr_table() != D2_NO_ERR)
  {                     /* error exit                                 */
    exit(1);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_ASN1_GENERATED_MSG));
  }

  if (admin_oct != NULL)    free(admin_oct);
  if (d26_oct != NULL)      free(d26_oct);
  if (oct_oids != NULL)     free(oct_oids);
  if (admin_at != NULL)     free(admin_at);
  if (d26_at != NULL)       free(d26_at);
  if (at_oids != NULL)      free(at_oids);
  if (admin_srt != NULL)    free(admin_srt);
  if (d26_srt != NULL)      free(d26_srt);
  if (d26_nam_at != NULL)   free(d26_nam_at);
  if (d26_ocl_arc != NULL)  free(d26_ocl_arc);
  if (d26_oct_idx != NULL)  free(d26_oct_idx);
  if (d26_name_pos != NULL) free(d26_name_pos);
  if (d26_at_pos != NULL)   free(d26_at_pos);
  if (level_pos != NULL)    free(level_pos);

  d26_gi7_fildes_free();

  dce_svc_printf(GDS_S_EXIT_GENDB_MSG);

  exit(0);

}                       /* main                                       */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g01_strip_oct_at                                */
/*                                                                    */
/* AUTHOR       : E. Kraemer, DAP 11                                  */
/* DATE         : 14.12.90                                            */
/*                                                                    */
/* DESCRIPTION  : Check, whether all OCT-entries are needed for the   */
/*                objects admitted by SRT. The superfluos ones are    */
/*                stripped. All AT-entries, which are not mentioned   */
/*                in OCT or SRT are stripped as well.                 */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short           no error occurred             */
/*      D26_ERROR                       error occurred                */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g01_strip_oct_at(void)

{                       /* d26_g01_strip_oct_at                       */

			    /* attribute type for DN-List attribute   */
  static D2_a_type dnl = {sizeof(D2_DNLIST) - 1,(Ob_id_string)D2_DNLIST};

			    /* attribute type for DN-List attribute   */
  static D2_obj_id ali = {sizeof(D2_OALIAS) - 1,(Ob_id_string)D2_OALIAS};

  signed16  i,j,k,l;        /* loop variables                         */
  Admin_srt *srtp;          /* Temp ptr to admin_srt array            */
  Admin_oct *octp,*octp1;   /* Temp ptr to admin_oct array            */
  Admin_at  *atp,*atp1;     /* Temp ptr to admin_at array             */

			    /* object idendifier in printable format  */
  char      pr_ob_id[D2_OBJ_IDL_MAX + 1];

  for                   /* all SRT-entries                            */
     (i = 0, srtp = admin_srt; i < nr_adm_srt; i++, srtp++)
  {                     /* compare object class acronym with all      */
			/* admin_oct-entries                          */
    for                 /* all OCT entries                            */
       (j = 0, octp = admin_oct; j < d26_nooct; j++, octp++)
    {                   /* check object class match                   */
      if                /* object class matches                       */
	(!strcmp(srtp->class_anym,octp->acronym))
      {                 /* check object class kind                    */
	if              /* object class is not structural             */
	  (octp->kind != D2_STRUCTURAL)
	{               /* log error and return                       */
	  dce_svc_printf(GDS_S_SRT_NSTR_OC_ERR_MSG,srtp->class_anym);
	  return(D26_ERROR);
	}
			/* set the used flag 'TRUE' for the object    */
			/* class and its superclasses                 */
	d26_g11_set_oct_usage(octp);
	for             /* all auxiliary object classes               */
	   (k = 0; k < octp->nr_aux_cl; k++)
	{               /* search for assigned entry in OCT           */
	  for           /* all other object classes                   */
	     (l = 0, octp1 = admin_oct; l < d26_nooct; l++, octp1++)
	  {             /* check whether acronyms match               */
	    if          /* acronyms match                             */
	      (!strcmp(octp->aux_anym[k],octp1->acronym))
	    {           /* check kind of object class                 */
	      if        /* object class is not auxiliary              */
		(octp1->kind != D2_AUXILIARY)
	      {         /* log error and return                       */
		dce_svc_printf(GDS_S_OCT_NAUX_OC_ERR_MSG,octp->aux_anym[k]);
		return(D26_ERROR);
	      }
			/* set the used flag 'TRUE' for the auxiliary */
			/* object class and its superclasses          */
	      d26_g11_set_oct_usage(octp1);
	      break;
	    }
	  }
	  if            /* auxiliary object class not found           */
	    (l == d26_nooct)
	  {             /* log error and return                       */
	    dce_svc_printf(GDS_S_OCT_NAUX_OC_ERR_MSG,octp->aux_anym[k]);
	    return(D26_ERROR);
	  }
	}
      }
    }
  }
			/* convert Alias-object class into printable  */
			/* format                                     */
  d27_301_oid_str(&ali,pr_ob_id);

			/* scan OCT and remove unused entries         */
  for                   /* all OCT entries                            */
     (i = 0, octp = admin_oct; i < d26_nooct; )
  {                     /* check the object class                     */
    if                  /* alias object class encountered             */
      (!strcmp(pr_ob_id,octp->obj_id))
    {                   /* set 'used' flag for alias                  */
      octp->used = TRUE;
    }
			/* check 'used' flag                          */
    if                  /* entry is used                              */
      (octp->used == TRUE)
    {                   /* just continue                              */
      i++;
      octp++;
    }
    else
    {                   /* remove the entry                           */
      for               /* all following OCT entries                  */
	 (j = i + 1, octp1 = octp; j < d26_nooct; j++, octp1++)
      {                 /* overwrite previous entry with it           */
	*octp1 = *(octp1 + 1);
      }
      d26_nooct--;
    }
  }
			/* scan OCT and check assigned attributes     */
  for                   /* all OCT entries                            */
     (i = 0, octp = admin_oct; i < d26_nooct; i++, octp++)
  {                     /* check mandatory attributes                 */
    for                 /* all mandatory attributes                   */
       (j = 0; j < octp->nr_mand; j++)
    {                   /* search for it in AT                        */
      for               /* all AT entries                             */
	 (k = 0, atp = admin_at; k < d26_noat; k++, atp++)
      {                 /* compare the acronyms                       */
	if              /* acronyms match                             */
	  (!strcmp(atp->acronym,octp->mand_anym[j]))
	{               /* set used flag to TRUE                      */
	  atp->used = TRUE;
	}
      }
    }
			/* check optional  attributes                 */
    for                 /* all optional attributes                    */
       (j = 0; j < octp->nr_opt; j++)
    {                   /* search for it in AT                        */
      for               /* all AT entries                             */
	 (k = 0, atp = admin_at; k < d26_noat; k++, atp++)
      {                 /* compare the acronyms                       */
	if              /* acronyms match                             */
	  (!strcmp(atp->acronym,octp->opt_anym[j]))
	{               /* set used flag to TRUE                      */
	  atp->used = TRUE;
	}
      }
    }
  }
			/* convert DN-List-type into printable format */
  d27_301_oid_str(&dnl,pr_ob_id);

			/* scan AT  and remove unused entries         */
  for                   /* all AT entries                             */
     (i = 0, atp = admin_at; i < d26_noat; )
  {                     /* check attribute type                       */
    if                  /* entry belongs to DN-List-attribute         */
      (!strcmp(pr_ob_id,atp->obj_id))
    {                   /* set used flag to TRUE                      */
      atp->used = TRUE;
    }
			/* check 'used' flag                          */
    if                  /* AT entry is used                           */
     (atp->used == TRUE)
    {                   /* just continue                              */
      i++;
      atp++;
    }
    else
    {                   /* remove the entry                           */
      for               /* all following AT entries                   */
	 (j = i + 1, atp1 = atp; j < d26_noat; j++, atp1++)
      {                 /* overwrite the previous entry with it       */
	*atp1 = *(atp1 + 1);
      }
      d26_noat--;
    }
  }

  return(D2_NO_ERR);

}                       /* d26_g01_strip_oct_at                       */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g02_proc_oct_oids                               */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : For each admin_oct entry, convert the object id from*/
/*                printable dotted form to an octal string which is   */
/*                stored in the d26_oct. Sort the d26_oct in ascending*/
/*                object id order. Link admin_oct entries to the      */
/*                corresponding entries in d26_oct.                   */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short           no error occurred             */
/*      D26_ERROR                       error occurred                */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g02_proc_oct_oids(void)

{                       /* d26_g02_proc_oct_oids                      */

Admin_oct *adm_octp;    /* Temp pointer to 'admin_oct' array          */
D26_oct   *octp;        /* Temp pointer to 'd26_oct' array            */
signed16  i;            /* loop variable                              */

for                     /* all OCT entries                            */
   (i = 0, octp = d26_oct, adm_octp = admin_oct; i < d26_nooct;
    i++, octp++, adm_octp++)
{                       /* link the OCT entry to administrate OCT     */
			/* After sorting, 'd26c_kind'   field will be */
			/* used to determine the index which links    */
			/* 'admin_oct' entries to 'd26_oct' entries.  */
  octp->d26c_kind = i;

  if                    /* Object class has no object identifier      */
    (strcmp(adm_octp->obj_id,"---") == 0)
  {                     /* set length to 0                            */
      octp->d26c_ob_id.d2_typ_len = 0;
  }
  else
  {                     /* set object identifier pointer to container */
    octp->d26c_ob_id.d2_type = oct_oids + (i * (D2_OBJ_IDL_MAX + 1));
    if                  /* object identifier cannot be converted      */
      (d27_302_str_oid(adm_octp->obj_id,&octp->d26c_ob_id) != D2_NO_ERR)
    {                   /* log error and return                       */
      return(D26_ERROR);
    }
  }
}
			/* Sort 'd26_oct' array in ascending obj id   */
			/* order                                      */
qsort((char *)d26_oct,d26_nooct,sizeof(D26_oct),d26_g22_cmp_oct);

for                     /* all OCT entries                            */
   (i = 0, octp = d26_oct; i < d26_nooct; i++, octp++)
{                       /* link appropriate administrated OCT entry   */
			/* to it                                      */
    admin_oct[octp->d26c_kind].link = i;
}

return(D2_NO_ERR);

}                       /* d26_g02_proc_oct_oids                      */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g03_setup_oct                                   */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : For each entry in admin_oct calculate the total     */
/*                number of its immediate superclasses, its auxiliary */
/*                object classes, the mandatory and optional          */
/*                attributes of the object class and all its          */
/*                superclasses and write them into global variables.  */
/*                Collect all the file numbers from admin_oct         */
/*                and sort them in ascending order. File number gaps  */
/*                are got rid of and the new file numbers are recorded*/
/*                in the current OCT entry. Global variable d26_nofile*/
/*                holds the maximum file number.                      */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      None                                                          */
/*                                                                    */
/* OUTPUT-PARAMETER:  -                                               */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short           no error occurred             */
/*      D26_ERROR                       error occurred                */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g03_setup_oct(void)

{                       /* d26_g03_setup_oct                          */

Fnr_elem  *filenr_list; /* For sorting and filtering gaps in file nrs */
signed16   prev_value;  /* Used when processing filenr_list           */

register   Admin_oct *adm_octp;
register   signed16   i;

if                      /* no more memory for file number list        */
  ((filenr_list = (Fnr_elem *)D27_CALLOC(svc_c_sev_fatal,d26_nooct,
   sizeof(Fnr_elem))) == NULL)
{                       /* log error and return                       */
  return(D26_ERROR);
}
else
{                       /* fill list of file numbers                  */

			/* nr_at_indices starts at 1 because later an */
			/* extra opt attr will be added to DSA class. */
			/* This attr will be D2_DNLIST.               */

  for                   /* all OCT-entries                            */
     (i = 0, adm_octp = admin_oct, nr_at_indices = 1, nr_oc_indices = 0;
      i < d26_nooct; i++, adm_octp++)
  {                     /* Keep running total of OCT-indices.         */
			/* Terminators = 2.                           */
    nr_oc_indices += adm_octp->nr_sup_cl + adm_octp->nr_aux_cl + 2;

			/* Keep running total of mandatory + optional */
			/* attrs for global use. Terminators = 2.     */
    nr_at_indices += d26_g12_calc_total_atidx(adm_octp) + 2;

			/* Fill local array with file_nr from         */
			/* admin_oct                                  */
    filenr_list[i].seq_nr = i;
    filenr_list[i].value  = (signed16)adm_octp->file_nr;
  }
			/* Sort local array in acsending file nr order*/
  qsort((char *)filenr_list,d26_nooct,sizeof(Fnr_elem),d26_g24_cmp_filenr);

			/* Get rid of gaps in filenrs and ensure that */
			/* file nrs start with -1. Put new nrs in OCT.*/

  for                   /* all elements of file number list           */
     (i = 0, d26_nofile = 0, prev_value = -1; i < d26_nooct;
      prev_value = filenr_list[i++].value)
  {                     /* compare subsequent file numbers            */
    if                  /* file numbers are different                 */
      (filenr_list[i].value != prev_value)
    {                   /* increment number of object files           */
	d26_nofile++;
    }
    d26_oct[admin_oct[filenr_list[i].seq_nr].link].d26c_filenr =
	    d26_nofile - 1;
  }

  free(filenr_list);
  return(D2_NO_ERR);
}

}                       /* d26_g03_setup_oct                          */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g04_proc_at_oids                                */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : For each admin_at entry, convert the object id from */
/*                printable dotted form to an octal string which is   */
/*                stored in the AT. Sort the AT in ascending object   */
/*                id order. Link admin_at entries to the corresponding*/
/*                entries in AT.                                      */
/*                                                                    */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      None                                                          */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short           no error occurred             */
/*      D26_ERROR                       error occurred                */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g04_proc_at_oids(void)

{                       /* d26_g04_proc_at_oids                       */

register Admin_at  *adm_atp;    /* Temp pointer to 'admin_at' array   */
register D26_at    *atp;/* Temp pointer to 'd26_at' array             */
register signed16  i;   /* loop variable                              */

for                     /* all AT entries                             */
   (i = 0, atp = d26_at, adm_atp = admin_at; i < d26_noat;
    i++, atp++, adm_atp++)
{                       /* link AT entry to Admin_at entry            */
			/* After sorting, 'd26a_lbound' field will be */
			/* used to determine the index which links    */
			/* 'admin_at' entries to 'AT' entries.        */
  atp->d26a_lbound = i;

  atp->d26a_type.d2_type = at_oids + (i * (D2_OBJ_IDL_MAX + 1));

  if                    /* object identifier cannot be converted      */
    (d27_302_str_oid(adm_atp->obj_id, &(atp->d26a_type)) != D2_NO_ERR)
  {                     /* log error and return                       */
    return(D26_ERROR);
  }
}
			/* Sort 'AT' array in ascending obj id order  */
qsort((char *)d26_at,d26_noat,sizeof(D26_at),d26_g23_cmp_at);

for                     /* all AT entries                             */
   (i=0, atp = d26_at; i < d26_noat; i++, atp++)
{                       /* link appropriate Admin_at entry to it      */
  admin_at[atp->d26a_lbound].link = i;
}

return(D2_NO_ERR);

}                       /* d26_g04_proc_at_oids                       */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g05_proc_srt_names                              */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : For each admin_srt entry sort the naming attributes.*/
/*                allocate memory for the naming attribute structures */
/*                in d26_srt. For admin_srt, which represent the same */
/*                name structure search for the naming attribute      */
/*                acronyms in admin_at. Record the links (index to AT)*/
/*                in the naming attribute structures in the SRT. Sort */
/*                the SRT in ascending naming index order. Link       */
/*                the entries to the corresponding entries in d26_srt.*/
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      None                                                          */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short           no error occurred             */
/*      D26_ERROR                       error occurred                */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g05_proc_srt_names(void)

{                       /* d26_g05_proc_srt_names                     */

			/* loop pointers to administrated SRT         */
register Admin_srt *adm_srtp1, *adm_srtp2;
			/* loop pointer to naming attribute structures*/
register D26_nam_at *natp;
register D26_srt *srtp; /* loop pointer to SRT                        */
register signed16 i,j,k;/* loop variables                             */

for                     /* all admin_srt entries                      */
   (i = nr_na_indices = 0, adm_srtp1 = admin_srt; i < nr_adm_srt;
    i++, adm_srtp1++)
{                       /* sort the naming attributes                 */
  qsort((char *)adm_srtp1->name_attr[0],adm_srtp1->nr_nattr,D2_L_ACRONYM + 1,
	d26_g25_cmp_naming_types);
  nr_na_indices += adm_srtp1->nr_nattr + 1;
}

if                      /* not enough menory for naming attribute     */
			/* structures                                 */
  ((d26_nam_at = (D26_nam_at *)D27_CALLOC(svc_c_sev_fatal,
   (size_t)nr_na_indices,sizeof(D26_nam_at))) == NULL)
{                       /* log error and return                       */
  return(D26_ERROR);
}
else
{                       /* link SRT and Admin_srt                     */
  for                   /* all admin_srt entries                      */
     (i = d26_nosrt = 0, srtp = d26_srt, adm_srtp1 = admin_srt,
      natp = d26_nam_at; i < nr_adm_srt; i++, adm_srtp1++)
  {                     /* compare name structures for all previous   */
			/* SRT entries                                */
    for                 /* all previous admin_srt entries             */
       (j = 0, adm_srtp2 = admin_srt; j < i; j++, adm_srtp2++)
    {                   /* compare name structures                    */
      if                /* name structures are equal                  */
	(d26_g31_cmp_name_str(adm_srtp1,adm_srtp2) == FALSE)
      {                 /* set prelink                                */
	adm_srtp1->prelink = adm_srtp2->prelink;
	break;
      }
    }
    if                  /* name structure is new                      */
      (j == i)
    {                   /* set prelink and relink                     */
      adm_srtp1->prelink = srtp->d26s_par_nr = d26_nosrt++;
      srtp->d26s_nam_at = natp;
      for               /* all naming attributes of the structure rule*/
	 (k = 0; k < adm_srtp1->nr_nattr; k++, natp++)
      {
	if              /* naming attribute not in AT                 */
	  ((natp->d26s_index = d26_g13_atacr_to_idx(
	    adm_srtp1->name_attr[k])) < 0)
	{               /* log error and return                       */
	  dce_svc_printf(GDS_S_SRT_NNAM_AT_ERR_MSG,adm_srtp1->name_attr[k]);
	  return(D26_ERROR);
	}
      }
      natp++->d26s_index = -1;
      srtp++;
    }
  }

  nr_na_indices = natp - d26_nam_at;

			/* Sort 'srt' in ascending naming index order */
  qsort((char *)d26_srt,d26_nosrt,sizeof(D26_srt),d26_g21_cmp_srt);

			/* Link admin_srt to correct d26_srt entries  */
  for                   /* all SRT entries                            */
     (i = 0, srtp = d26_srt; i < d26_nosrt; i++, srtp++)
  {                     /* search for links in Admin_srt              */
    for                 /* all admin_srt entries                      */
       (j = 0, adm_srtp1 = admin_srt; j < nr_adm_srt; j++, adm_srtp1++)
    {                   /* compare links                              */
      if                /* match is found                             */
	(srtp->d26s_par_nr == adm_srtp1->prelink)
      {                 /* reset link                                 */
	adm_srtp1->link = i;
      }
    }
  }

  return(D2_NO_ERR);
}

}                       /* d26_g05_proc_srt_names                     */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g06_setup_srt                                   */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : For each entry in admin_srt, work out the d26_srt   */
/*                index for the parent and record this in the current */
/*                d26_srt-entry. Keep a running total of all the      */
/*                object class arcs in admin_srt. This is held in the */
/*                global variable nr_cl_indices.                      */
/*                                                                    */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      None                                                          */
/*                                                                    */
/* OUTPUT-PARAMETER:  -                                               */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short           no error occurred             */
/*      D26_ERROR                       error occurred                */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g06_setup_srt(void)

{                       /* d26_g06_setup_srt                          */

Admin_srt *adm_srtp, *adm_srtp_loop;    /* loop pointers to admin_srt */
D26_srt   *srtp;                        /* pointer to SRT             */
signed16   i,j;

for                     /* All SRT entries                            */
   (i = 0, srtp = d26_srt; i < d26_nosrt; i++, srtp++)
{                       /* initialize parent object number            */
  srtp->d26s_par_nr = -2;
}

for (i = 0, adm_srtp = admin_srt; i < nr_adm_srt; i++, adm_srtp++)
{
  srtp = d26_srt + adm_srtp->link;
  if                    /* parent object number not yet set           */
    (srtp->d26s_par_nr == -2)
  {                     /* check whether root is parent               */
    if                  /* root is parent                             */
      (adm_srtp->par_ob_nr == 0)
    {                   /* set root parent in SRT entry               */
      srtp->d26s_par_nr = D26_ROOT;
    }
    else
    {                   /* search for parent entry of admin_srt       */
      for               /* all admin_srt entries                      */
	 (j = 0, adm_srtp_loop = admin_srt; j < nr_adm_srt;
	  j++, adm_srtp_loop++)
      {                 /* compare object number with parent number   */
	if              /* parent entry is found                      */
	  (adm_srtp->par_ob_nr == adm_srtp_loop->ob_nr)
	{               /* set parent number                          */
	  srtp->d26s_par_nr = adm_srtp_loop->link;
	  break;
	}
      }
      if                /* no parent entry found                      */
	(j == nr_adm_srt)
      {                 /* log error and return                       */
	dce_svc_printf(GDS_S_SRT_SUP_MISSING_MSG,adm_srtp->par_ob_nr);
	return(D26_ERROR);
      }
    }
  }
}

return(D2_NO_ERR);

}                       /* d26_g06_setup_srt                          */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g07_setup_at                                    */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : For each entry in admin_at, copy most of the fields */
/*                directly to the corresponding AT entry. Work out    */
/*                the ASN.1 representation based on each entry syntax.*/
/*                Set up 'cache' and 'importance' fields according to */
/*                the rules from the design spec. Determine if the    */
/*                attr value will be held in an 'object' or an 'attr' */
/*                file. Set up total number of attr files required in */
/*                the global variable d26_nafile.                     */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      max_len         short       maximum length of attribute       */
/*                                  values to be stored at object     */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short           no error occurred             */
/*      D26_ERROR                       error occurred                */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g07_setup_at(
    signed16 max_len)

{                       /* d26_g07_setup_at                           */

D26_at    *atp;         /* pointer to AT                              */

			/* loop pointer to administrated AT           */
register Admin_at *adm_atp;
register D26_srt *srtp; /* loop pointer to SRT                        */
			/* loop pointer to naming attribute structures*/
register D26_nam_at *nam_at;
register signed16 i,j;  /* loop variables                             */

for                     /* all entries of administrated AT            */
   (i = 0, adm_atp = admin_at, d26_nafile = 3; i < d26_noat; i++, adm_atp++)
{                       /* link AT entry to administrated AT          */
  atp = d26_at + adm_atp->link;

  atp->d26a_syntax    = (signed16)adm_atp->syntax;
  switch((int)atp->d26a_syntax)
  {
    case D2_TIME:       /* attribute syntax is UTC time               */
      atp->d26a_lbound = (signed16)D26_TIME_LBOUND;
      atp->d26a_ubound = (signed16)D26_TIME_UBOUND;
      break;
    case D2_FAXNR_STX:  /* attribute syntax is FAX number             */
      atp->d26a_ubound = 2 * sizeof(signed16) +
			 D2_ALIGN(adm_atp->ubound) + sizeof(long);
      atp->d26a_lbound = 2 * sizeof(signed16) + (signed16)adm_atp->lbound;
      break;
    default:
      atp->d26a_lbound = (signed16)adm_atp->lbound;
      atp->d26a_ubound = (signed16)adm_atp->ubound;
      break;
  }

  atp->d26a_nr_recur  = (signed16)adm_atp->nr_recur;
  atp->d26a_phon      = (atp->d26a_syntax == D2_CEXACT ||
    atp->d26a_syntax == D2_CIGNORE || atp->d26a_syntax == D2_PRINTABLE ||
    atp->d26a_syntax == D2_IA5 || atp->d26a_syntax == D2_CIGN_LIST ||
    atp->d26a_syntax == D2_COUNTRY_STX ||
    atp->d26a_syntax == D2_POST_AD_STX) ? (signed16)adm_atp->phon : FALSE;
  atp->d26a_acl       = (signed16)adm_atp->acl;
  atp->d26a_ind_level = (signed16)adm_atp->index_level;

  if                    /* no matching allowed for attribute          */
    (atp->d26a_syntax == D2_ANY_STX || atp->d26a_syntax == D2_GUIDE   ||
     atp->d26a_syntax == D2_CERTIFICATE ||
     atp->d26a_syntax == D2_CERT_PAIR || atp->d26a_syntax == D2_CERT_LIST ||
     atp->d26a_syntax == D2_PR_DM_STX || atp->d26a_syntax == D2_ASN1)
  {                     /* reset index level                          */
    atp->d26a_ind_level = 0;
  }
			/* Set up ASN.1 representation                */
  if                    /* syntax not known                           */
    ((atp->d26a_rep = d27_401_syntax_to_rep(atp->d26a_syntax)) == D2_ERROR)
  {                     /* log error and log                          */
    dce_svc_printf(GDS_S_ATSYNTAX_UNKNOWN_MSG,atp->d26a_syntax);
    return(D26_ERROR);
  }

  if                    /* object class attribute                     */
    (D2_ATTCMP(&atp->d26a_type,D2_OBJ_CLASS) == 0)
  {                     /* set occurrency fixed                       */
    atp->d26a_nr_recur = D26_OCL_VAL_COUNT;
  }

  if                    /* The following attributes may not recur     */
     ((D2_ATTCMP(&atp->d26a_type,D2_ALIASED_OBJECT) == 0) ||
      (D2_ATTCMP(&atp->d26a_type,D2_COUNTRY) == 0) ||
      (D2_ATTCMP(&atp->d26a_type,D2_PSAP_ADDRESS) == 0) ||
      (D2_ATTCMP(&atp->d26a_type,D2_ACL) == 0) ||
      (D2_ATTCMP(&atp->d26a_type,D2_MASTER_KNOWLEDGE) == 0) ||
      (D2_ATTCMP(&atp->d26a_type,D2_PREF_DEV_METHOD) == 0))
  {                     /* set them to single valued                  */
    atp->d26a_nr_recur = 1;
  }

  if                    /* attribute is ACL                           */
    (D2_ATTCMP(&atp->d26a_type, D2_ACL) == 0)
  {                     /* don't allow deletion                       */
    atp->d26a_importance = D26_NODELETE;
  }
  else if               /* attribute is master knowledge              */
	 (D2_ATTCMP(&atp->d26a_type,D2_MASTER_KNOWLEDGE) == 0)
  {                     /* don't allow deletion and assign index      */
    atp->d26a_importance = D26_NODELETE;
    d26_mk_idx = adm_atp->link;
  }
  else if               /* attribute is object class                  */
	 (D2_ATTCMP(&atp->d26a_type,D2_OBJ_CLASS) == 0)
  {                     /* don't allow change                         */
    atp->d26a_importance = D26_NOCHANGE;
  }
  else
  {                     /* allow any change                           */
    atp->d26a_importance = D26_DOANY;
    if                  /* attribute is aliased object                */
      (D2_ATTCMP(&atp->d26a_type,D2_ALIASED_OBJECT) == 0)
    {                   /* assign index                               */
      d26_alob_idx = adm_atp->link;
    }
    else if             /* attribute is DN-list attribute             */
      (D2_ATTCMP(&atp->d26a_type,D2_DNLIST) == 0)
    {                   /* assign index                               */
      d26_dnl_idx = adm_atp->link;
    }
  }

  if                    /* attribute is SRT                           */
    (D2_ATTCMP(&atp->d26a_type,D2_SRT) == 0)
  {                     /* assign attribute file number 0             */
    atp->d26a_filenr = 0;
    atp->d26a_ind_level = 0;
    continue;
  }
  if                    /* attribute is OCT                           */
    (D2_ATTCMP(&atp->d26a_type,D2_OCT) == 0)
  {                     /* assign attribute file number 1             */
    atp->d26a_filenr = 1;
    atp->d26a_ind_level = 0;
    continue;
  }
  if                    /* attribute is AT                            */
    (D2_ATTCMP(&atp->d26a_type,D2_AT) == 0)
  {                     /* assign attribute file number 2             */
    atp->d26a_filenr = 2;
    atp->d26a_ind_level = 0;
    continue;
  }
			/* If the current attr is a naming attr, then */
			/* make sure its value is in the object file. */

  for                   /* all SRT entries                            */
     (j = 0, srtp = d26_srt; j < d26_nosrt; j++, srtp++)
  {                     /* scan the nameing attributes                */
    for                 /* all naming attributes of the SRT entries   */
       (nam_at = srtp->d26s_nam_at; nam_at->d26s_index != -1; nam_at++)
    {                   /* compare with actual AT entry               */
      if                /* naming attribute is the actual one         */
	(nam_at->d26s_index == adm_atp->link)
      {                 /* check whether attribute is recurring       */
	if              /* attribute is truly recurring               */
	  (atp->d26a_nr_recur == 0)
	{               /* log error                                  */
	  dce_svc_printf(GDS_S_NAMAT_RECUR_MSG,adm_atp->acronym);
	  return(D26_ERROR);
	}
	else
	{               /* set invalid attribute file number          */
	  atp->d26a_filenr = -1;
	  break;
	}
      }
    }
    if                  /* current attribute was found as naming      */
      (nam_at->d26s_index != -1)
    {                   /* break the loop                             */
      break;
    }
  }

  if                    /* attribute is not naming                    */
    (j == d26_nosrt)
  {                     /* Unlimited recurring attributes and long    */
			/* attribute values will later be stored in   */
			/* files called attr<filenr>.                 */
			/* Attrs with ACCL, DISTNAME syntax           */
			/* and type D2_OBJ_CLASS can never be too     */
			/* long because they hold only 4 bytes.       */

    if                  /* attribute is truly recurring or too long   */
      (atp->d26a_nr_recur == 0 ||
       (atp->d26a_ubound > max_len && atp->d26a_syntax != D2_PSAP_STX &&
	atp->d26a_syntax != D2_ACCL && atp->d26a_syntax != D2_DISTNAME &&
	D2_ATTCMP(&atp->d26a_type,D2_OBJ_CLASS) != 0))
    {                   /* set attribute file number                  */
      atp->d26a_filenr = d26_nafile++;
    }
    else
    {                   /* set invalid attribute file number          */
      atp->d26a_filenr = -1;
    }
  }
}

return(D2_NO_ERR);

}                       /* d26_g07_setup_at                           */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g08_calc_class_idx                              */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : For each admin_srt entry, search for all the class  */
/*                acronyms in admin_oct. Store all link numbers       */
/*                (indices to OCT) in an array in the current SRT     */
/*                entry. Sort the array ascending index order and     */
/*                terminate it with -1.                               */
/*                                                                    */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      None                                                          */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short           no error occurred             */
/*      D26_ERROR                       error occurred                */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g08_calc_class_idx(void)

{                       /* d26_g08_calc_class_idx                     */

register D26_srt *srtp; /* loop pointer to SRT                        */
			/* loop pointer to object class arcs array    */
register D26_ocl_arc *srt_oa_p;
			/* loop pointers to administrated SRT         */
register Admin_srt *adm_srtp, *adm_srtp_loop;
register signed16 i,j,k;/* loop variables                             */

			/* For each SRT entry, set d26s_oclass to     */
			/* point to an array of ascending link nrs    */
			/* terminated by -1. The link numbers indicate*/
			/* entries in 'd26_oct' for each class listed.*/

for                     /* all SRT entries                            */
   (i = 0, srt_oa_p = d26_ocl_arc, srtp = d26_srt; i < d26_nosrt;
    i++, srtp++)
{                       /* set object class arc pointer               */
  srtp->d26s_ocl_arc = srt_oa_p;

  for                   /* all admin_srt entries                      */
     (j = 0, adm_srtp = admin_srt; j < nr_adm_srt; j++, adm_srtp++)
  {                     /* check, whether admin_srt entry is assigned */
			/* to SRT entry                               */
    if                  /* entries are linked                         */
      (adm_srtp->link == i)
    {                   /* write object class index into arc          */
      if                /* object class not found                     */
	((srt_oa_p->d26s_oclass = d26_g14_ocacr_to_idx(adm_srtp->class_anym))
	 < 0)
      {                 /* log error and return                       */
	dce_svc_printf(GDS_S_SRT_OC_INVALID_MSG,adm_srtp->class_anym);
	return(D26_ERROR);
      }
      else
      {                 /* check whether structure rule is subordinate*/
			/* to root                                    */
	if              /* structure rule is subordinate to root      */
	  (adm_srtp->par_ob_nr == 0)
	{               /* write "root" into object class arc         */
	  srt_oa_p->d26s_supocl = D26_ROOT;
	}
	else
	{               /* search for superior structure rule         */
	  for           /* all Admin_srt entries                      */
	     (k = 0, adm_srtp_loop = admin_srt; k < nr_adm_srt;
	      k++, adm_srtp_loop++)
	  {             /* compare object numbers                     */
	    if          /* parent entry is found                      */
	      (adm_srtp_loop->ob_nr == adm_srtp->par_ob_nr)
	    {           /* write superior class index                 */
	      if        /* superior object class not found            */
		((srt_oa_p->d26s_supocl = d26_g14_ocacr_to_idx(
		 adm_srtp_loop->class_anym)) < 0)
	      {         /* log error                                  */
		dce_svc_printf(GDS_S_SRT_OC_INVALID_MSG,
		  adm_srtp->class_anym);
		return(D26_ERROR);
	      }
	      break;
	    }
	  }
	  if            /* parent entry not found                     */
	    (k == nr_adm_srt)
	  {             /* log error                                  */
	    dce_svc_printf(GDS_S_SRT_SUP_MISSING_MSG,adm_srtp->par_ob_nr);
	    return(D26_ERROR);
	  }
	}
      }
			/* increment pointer to object class arcs     */
      srt_oa_p++;
    }
  }

  qsort((char *)srtp->d26s_ocl_arc,srt_oa_p - srtp->d26s_ocl_arc,
	sizeof(D26_ocl_arc),d26_g26_cmp_ocl_arcs);

			/* set end of list                            */
  srt_oa_p->d26s_oclass = -1;
			/* increment pointer to object class arcs     */
  srt_oa_p++;
}

nr_cl_indices = srt_oa_p - d26_ocl_arc;

#ifdef DESCRIBE
{
    D26_srt *srtp;

    fprintf(stderr, "SRT ............\n\n");
    for (i = 0, srtp = d26_srt; i < d26_nosrt; i++, srtp++)
    {
	for (j = 0, adm_srtp = admin_srt; j < nr_adm_srt; j++, adm_srtp++)
	{
	    if (i == adm_srtp->link)
		break;
	}
	fprintf(stderr, "%2d: parent %2d names: ",i, srtp->d26s_par_nr);
	for (j = 0; j < adm_srtp->nr_nattr; j++)
	{
	    fprintf(stderr, "%3s %d : ", adm_srtp->name_attr[j],
		    srtp->d26s_nam_at[j].d26s_index);
	}
	fprintf(stderr, "class_arcs: ");
	for (j=0, srt_oa_p = srtp->d26s_ocl_arc; srt_oa_p->d26s_oclass != -1;
	     j++, srt_oa_p++)
	{
	    fprintf(stderr, "%2d %2d : ",srt_oa_p->d26s_oclass,
		    srt_oa_p->d26s_supocl);
	}
	fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
}
#endif

return(D2_NO_ERR);

}                       /* d26_g08_calc_class_idx                     */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g09_calc_attr_idx                               */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : For each admin_oct entry, search for all the        */
/*                mandatory attribute acronyms in admin_at. Store all */
/*                the link numbers (indices to AT), in an array in the*/
/*                current d26_oct entry. Sort the array ascending     */
/*                index order and terminate it with -1. Do the same   */
/*                for all the optional attributes.                    */
/*                                                                    */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      None                                                          */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short           no error occurred             */
/*      D26_ERROR                       error occurred                */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g09_calc_attr_idx(void)

{                       /* d26_g09_calc_attr_idx                      */

void    *idx_root;      /* root of indices in tsearch tree            */
short   *oct_idx_p;     /* loop pointer to OCT index arrax            */
			/* loop pointer to administrated OCT          */
register Admin_oct *adm_octp;
register signed16 i,j;  /* loop variables                             */

			/* For each OCT entry, set d26c_sup_cl,       */
			/* d26c_aux_cl, d26c_mandatory and            */
			/* d26c_optional to point to arrays of acsend-*/
			/* ing link nrs terminated by -1. The link nrs*/
			/* indicate entries in 'AT' for each attrib-  */
			/* ute listed in the OCT entry.               */

for                     /* all admin_oct entries                      */
   (i = 0, oct_idx_p = d26_oct_idx, adm_octp = admin_oct; i < d26_nooct;
    i++, adm_octp++)
{                       /* set superclass pointer in OCT entry        */
  d26_oct[adm_octp->link].d26c_sup_cl = oct_idx_p;

			/* For each superclass search for acronym in  */
			/* in 'admin_oct' and record appropriate link */
  for                   /* all superclasses                           */
     (j = 0; j < adm_octp->nr_sup_cl; j++)
  {                     /* calculate index of superclass              */
    if                  /* superclass not in OCT                      */
      ((*oct_idx_p++ = d26_g14_ocacr_to_idx(adm_octp->super_anym[j])) < 0)
    {                   /* log error and return                       */
      dce_svc_printf(GDS_S_OCT_SUP_MISSING_MSG,adm_octp->super_anym[j]);
      return(D26_ERROR);
    }
  }
  qsort((char *)d26_oct[adm_octp->link].d26c_sup_cl,j,sizeof(signed16),
	d26_g27_cmp_idx);
  *oct_idx_p++ = -1;    /* Terminate list                             */

			/* set auxiliary class pointer in OCT entry   */
  d26_oct[adm_octp->link].d26c_aux_cl = oct_idx_p;
			/* For each auxiliary object class search     */
			/* for acronym in 'admin_at' and record       */
			/* appropriate link                           */
  for                   /* all auxiliary object classes               */
     (j = 0; j < adm_octp->nr_aux_cl; j++)
  {                     /* calculate index of auxiliary object class  */
    if                  /* auxiliary object class not in OCT          */
      ((*oct_idx_p++ = d26_g14_ocacr_to_idx(adm_octp->aux_anym[j])) < 0)
    {                   /* log error                                  */
      dce_svc_printf(GDS_S_OCT_NAUX_OC_ERR_MSG,adm_octp->aux_anym[j]);
      return(D26_ERROR);
    }
  }
  qsort((char *)d26_oct[adm_octp->link].d26c_aux_cl,j,sizeof(signed16),
	d26_g27_cmp_idx);
  *oct_idx_p++ = -1;    /* Terminate list                             */
}

for                     /* all admin_oct entries                      */
   (i = 0, adm_octp = admin_oct; i < d26_nooct;
    i++, oct_idx_p++, adm_octp++)
{                       /* set mandatory attribute pointer in OCT     */
  idx_root = (void *)NULL;
  d26_oct[adm_octp->link].d26c_mandatory = oct_idx_p;

  if                    /* list of mandatory attributes cannot be     */
			/* generated                                  */
    (d26_g15_mand_list(adm_octp,&idx_root,&oct_idx_p) != D2_NO_ERR)
  {
    return(D26_ERROR);
  }
  *oct_idx_p++ = -1;    /* Set terminator flag                        */

			/* set optional attribute pointer in OCT      */
  d26_oct[adm_octp->link].d26c_optional = oct_idx_p;

  if                    /* list of optional  attributes cannot be     */
			/* generated                                  */
    (d26_g16_opt_list(adm_octp,&idx_root,&oct_idx_p) != D2_NO_ERR)
  {
    return(D26_ERROR);
  }
  *oct_idx_p++ = -1;    /* Set terminator flag                        */

  for                   /* all allocated mandatory indices            */
     (j = 0, oct_idx_p = d26_oct[adm_octp->link].d26c_mandatory;
      *oct_idx_p != -1; j++, oct_idx_p++)
  {                     /* delete node from tree                      */
    tdelete(oct_idx_p,&idx_root,d26_g27_cmp_idx);
  }
  qsort((char *)d26_oct[adm_octp->link].d26c_mandatory,j,sizeof(signed16),
	d26_g27_cmp_idx);

  for                   /* all allocated optional indices             */
     (j = 0, oct_idx_p = d26_oct[adm_octp->link].d26c_optional;
      *oct_idx_p != -1; j++, oct_idx_p++)
  {                     /* delete node from tree                      */
    tdelete(oct_idx_p,&idx_root,d26_g27_cmp_idx);
  }
  qsort((char *)d26_oct[adm_octp->link].d26c_optional,j,sizeof(signed16),
	d26_g27_cmp_idx);
}

nr_oct_indices = oct_idx_p - d26_oct_idx;

for                     /* all admin_oct entries                      */
   (i = 0, adm_octp = admin_oct; i < d26_nooct; i++, adm_octp++)
{                       /* set correct object class kind              */
  d26_oct[adm_octp->link].d26c_kind = adm_octp->kind;
}

#ifdef DESCRIBE
{
    D26_oct *octp;

    fprintf(stderr, "\fOCT ............\n\n");
    for (i=0, octp = d26_oct; i < d26_nooct; i++, octp++)
    {
	for (j=0, adm_octp = admin_oct; j < d26_nooct; j++, adm_octp++)
	{
	    if (i == adm_octp->link)
		break;
	}
	fprintf(stderr, "%2d: %3s file %2d kind: %2d object id %s\n",
	    i, adm_octp->acronym, octp->d26c_filenr,octp->d26c_kind,
	    adm_octp->obj_id);
	fprintf(stderr, "\tsup_cl: ");
	for (j=0; octp->d26c_sup_cl[j] != -1; j++)
	    fprintf(stderr, "%2d ", octp->d26c_sup_cl[j]);
	fprintf(stderr, "\n\taux_cl: ");
	for (j=0; octp->d26c_aux_cl[j] != -1; j++)
	    fprintf(stderr, "%2d ", octp->d26c_aux_cl[j]);
	fprintf(stderr, "\n\tmand:");
	for (j=0; octp->d26c_mandatory[j] != -1; j++)
	    fprintf(stderr, "%2d ", octp->d26c_mandatory[j]);
	fprintf(stderr, "\n\topt: ");
	for (j=0; octp->d26c_optional[j] != -1; j++)
	    fprintf(stderr, "%2d ", octp->d26c_optional[j]);
	fprintf(stderr, "\n");
	if ((i%20) == 19)
	    fprintf(stderr, "\f\n\n");
    }
    fprintf(stderr, "\n");
}
#endif

return(D2_NO_ERR);

}                       /* d26_g09_calc_attr_idx                      */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g10_write_attr_table                            */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 03.13.92                                            */
/*                                                                    */
/* DESCRIPTION  : Create a text file with Object ids and ASN.1        */
/*                representation for all attributes. This file will   */
/*                be read in for the ASN.1 encoding                   */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      None                                                          */
/*                                                                    */
/* OUTPUT-PARAMETER:  -                                               */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short           no error occurred             */
/*      D26_ERROR                       error occurred                */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g10_write_attr_table(void)

{                       /* d26_g10_write_attr_table                   */

			/* name of asn1_attr file                     */
char    fname[D27_LFILE_NAME];
FILE    *fp;            /* file pointer for asn1_attr file            */

			/* memory for printable Object identifier     */
char    s[D2_OBJ_IDL_MAX + 1];
			/* return value                               */
signed16 ret_value = D2_NO_ERR;

register D26_at *atp;   /* loop pointer to AT                         */
register signed16 i;    /* loop variable                              */

sprintf(fname,"%s%s",dir_fname,D27_ASN1_ATTR);

if                      /* attribute table cannot be opened           */
  ((fp = D27_FOPEN(svc_c_sev_fatal,fname,"w")) == NULL)
{                       /* log the error and return                   */
  ret_value = D26_ERROR;
}
else
{                       /* write count of attributes first            */
  if                    /* count of attributes cannot be written      */
    (d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%d\n",d26_noat) < 0)
  {                     /* log error and set return value             */
    ret_value = D26_ERROR;
  }

  for                   /* all AT entries                             */
     (i = 0, atp = d26_at; i < d26_noat; i++, atp++)
  {                     /* convert the object identifier              */
    if                  /* mapping line cannot be written to the file */
      (d27_301_oid_str(&atp->d26a_type,s) != D2_NO_ERR ||
       d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%-28s %5d\n",s,
       atp->d26a_rep) < 0)
    {
      break;
    }
  }
  if                    /* ASN1_attr file is incomplete               */
    (i < d26_noat)
  {                     /* set return value                           */
    ret_value = D26_ERROR;
  }

  if                    /* file cannot be closed                      */
    (D27_FCLOSE(svc_c_sev_fatal,fp) < 0)
  {                     /* log the error and set the return value     */
    ret_value = D26_ERROR;
  }
}

return(ret_value);

}                       /* d26_g10_write_attr_table                   */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g11_set_oct_usage                               */
/*                                                                    */
/* AUTHOR       : E. Kraemer, DAP 11                                  */
/* DATE         : 06.03.92                                            */
/*                                                                    */
/* DESCRIPTION  : The function sets the usage flag for the OCT-entry. */
/*                Then it calls itself recursively for all its        */
/*                superclasses and auxiliary object classes.          */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      adm_octp        Admin_oct * pointer to administrated OCT      */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      adm_octp        Admin_oct * pointer to administrated OCT      */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

static void d26_g11_set_oct_usage(
    Admin_oct *adm_octp)

{                       /* d26_g11_set_oct_usage                      */

register signed16   i,j;   /* loop variables                          */
register Admin_oct *adm_octp_loop; /* loop pointer to admin_oct array */

if(adm_octp->used != TRUE)
{                       /* OCT-entry is not yet tagged as 'used'      */
  adm_octp->used = TRUE;

  for                   /* all superclasses                           */
     (i = 0; i < adm_octp->nr_sup_cl; i++)
  {
    for                 /* all OCT entries                            */
       (j = 0, adm_octp_loop = admin_oct; j < d26_nooct;
	j++, adm_octp_loop++)
    {
      if                /* acronyms match                             */
	(!strcmp(adm_octp->super_anym[i],adm_octp_loop->acronym))
      {                 /* call function for superclass               */
	d26_g11_set_oct_usage(adm_octp_loop);
      }
    }
  }
}

}                       /* d26_g11_set_oct_usage                      */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g12_calc_total_atidx                            */
/*                                                                    */
/* AUTHOR       : E. Kraemer, DAP 11                                  */
/* DATE         : 06.03.92                                            */
/*                                                                    */
/* DESCRIPTION  : The function calculates the total count of all      */
/*                AT-indices for the optional and mandatory attributes*/
/*                for the object class and all its superclasses.      */
/*                Nothing is merged, as the number is used only for   */
/*                calloc.                                             */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      adm_octp        Admin_oct * pointer to administrated OCT      */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      > 0             short       count of indices contained in OCT */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g12_calc_total_atidx(
    Admin_oct *adm_octp)

{                       /* d26_g12_calc_total_atidx                   */

signed16 total;         /* total number of mandatory and optional     */
			/* attributes                                 */

register Admin_oct *adm_octp_loop;  /* loop pointer to admin_oct array*/
register signed16 i,j;  /* loop variables                             */

total = adm_octp->nr_mand + adm_octp->nr_opt;

for                     /* all superclasses                           */
   (i = 0; i < adm_octp->nr_sup_cl; i++)
{                       /* search for superclass entry                */
  for                   /* all OCT entries                            */
     (j = 0, adm_octp_loop = admin_oct; j < d26_nooct; j++, adm_octp_loop++)
  {                     /* compare acronyms                           */
    if                  /* acronyms match                             */
      (!strcmp(adm_octp->super_anym[i],adm_octp_loop->acronym))
    {                   /* call function for superclass               */
      total += d26_g12_calc_total_atidx(adm_octp_loop);
    }
  }
}

return(total);

}                       /* d26_g12_calc_total_atidx                   */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g13_atacr_to_idx                                */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : Searches the admin_at for an entry with the given   */
/*                acronym and returns the assigned AT-index.          */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      acronym     char *      Acronym of the attribute              */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      >= 0            Short       AT-index of the attribute         */
/*      -1                          No entry in AT found              */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g13_atacr_to_idx(
    char *acronym)

{                       /* d26_g13_atacr_to_idx                       */

			/* loop pointer to administrated AT           */
register Admin_at *adm_atp;
register signed32 i;    /* loop variable                              */

for                     /* all administrated AT entries               */
   (i = 0, adm_atp = admin_at; i < d26_noat; i++, adm_atp++)
{
  if                    /* Acronyms match                             */
    (!strcmp(acronym,adm_atp->acronym))
  {                     /* break loop                                 */
    break;
  }
}

if                      /* no AT entry was found                      */
  (i == d26_noat)
{                       /* return error                               */
  return(-1);
}
else
{                       /* return index of AT entry                   */
  return(adm_atp->link);
}

}       /* end d26_g13_atacr_to_idx                                   */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g14_ocacr_to_idx                                */
/*                                                                    */
/* AUTHOR       : Ernst Kraemer, AP 11                                */
/* DATE         : 10.03.92                                            */
/*                                                                    */
/* DESCRIPTION  : Searches the admin_oct for an entry with the given  */
/*                acronym and returns the assigned OCT-index.         */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      acronym     char *      Acronym of the object class           */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      >= 0            Short       OCT-index of the object class     */
/*      -1                          No entry in OCT found             */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g14_ocacr_to_idx(
    char *acronym)

{                       /* d26_g14_ocacr_to_idx                       */

			/* loop pointer to administrated OCT          */
register Admin_oct *adm_octp;
register signed16 i;    /* loop variable                              */

for                     /* all entries of administrated OCT           */
   (i = 0, adm_octp = admin_oct; i < d26_nooct; i++, adm_octp++)
{                       /* check whether acronyms match               */
  if                    /* Acronyms match                             */
    (!strcmp(acronym,adm_octp->acronym))
  {                     /* break loop                                 */
    break;
  }
}

if                      /* no OCT entry was found                     */
  (i == d26_nooct)
{                       /* return error                               */
  return(-1);
}
else
{                       /* return index of OCT entry                  */
  return(adm_octp->link);
}

}                       /* d26_g14_ocacr_to_idx                       */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g15_mand_list()                                 */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 10.03.92                                            */
/*                                                                    */
/* DESCRIPTION  : The function converts all the mandatory attributes  */
/*                of the OCT entry to indices and writes them into    */
/*                the list of mandatory attributes. The function calls*/
/*                itself then recursively for all superclasses        */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      adm_octp        Admin_oct *     pointer to Admin OCT entry    */
/*      idx_root        short **        root of mandatory attributes  */
/*      mand_list       short **        list of mandatory attributes  */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      mand_list       short **        list of mandatory attributes  */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short           no error occurred             */
/*      D26_ERROR                       error occurred                */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g15_mand_list(
    Admin_oct *adm_octp,
    void **idx_root,
    signed16 **mand_list)

{                       /* d26_g15_mand_list                          */

D26_oct  *octp;         /* pointer to OCT                             */
void     **tsearch_result;  /* result returned by tsearch             */

register signed16 j;    /* loop variable                              */

			/* For each mandatory attribute search for    */
			/* acronym in 'admin_at' and record           */
			/* appropriate link                           */

for                     /* all mandatory attributes                   */
   (j = 0; j < adm_octp->nr_mand; j++)
{                       /* calculate AT index of attribute            */
  if                    /* attribute not in AT                        */
    ((**mand_list = d26_g13_atacr_to_idx(adm_octp->mand_anym[j])) < 0)
  {                     /* log error and return                       */
    dce_svc_printf(GDS_S_OCT_AT_INVALID_MSG,adm_octp->mand_anym[j]);
    return(D26_ERROR);
  }
  else
  {                     /* put new element into binary tree           */
    tsearch_result = D27_TSEARCH(svc_c_sev_fatal,(void *)*mand_list,idx_root,
      d26_g27_cmp_idx);
    if                  /* attribute is new                           */
      (*tsearch_result == (void *)*mand_list)
    {                   /* increment pointer to mandatory attributes  */
      (*mand_list)++;
    }
  }
}

for                     /* all superclasses                           */
   (j = 0; d26_oct[adm_octp->link].d26c_sup_cl[j] != D26_ROOT; j++)
{                       /* call function recursively                  */
  octp = d26_oct + d26_oct[adm_octp->link].d26c_sup_cl[j];
  if                    /* mandatory list cannot be generated         */
    (d26_g15_mand_list(admin_oct + octp->d26c_kind,idx_root,mand_list)
     != D2_NO_ERR)
  {                     /* return error                               */
    return(D26_ERROR);
  }
}

return(D2_NO_ERR);

}                       /* d26_g15_mand_list                          */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g16_opt_list                                    */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 11.03.92                                            */
/*                                                                    */
/* DESCRIPTION  : The function converts all the optional  attributes  */
/*                of the OCT entry to indices and writes them into    */
/*                the list of optional attributes. The function calls */
/*                itself then recursively for all superclasses.       */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      adm_octp        Admin_oct *     pointer to Admin OCT entry    */
/*      idx_root        short **        root of indices               */
/*      opt_list        short **        list of optional  attributes  */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      opt_list        short **        list of optional  attributes  */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short           no error occurred             */
/*      D26_ERROR                       error occurred                */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g16_opt_list(
    Admin_oct *adm_octp,
    void **idx_root,
    signed16 **opt_list)

{                       /* d26_g16_opt_list                           */

D26_oct  *octp;         /* pointer to OCT                             */
void     **tsearch_result;  /* result returned by tsearch             */

register signed16 j;     /* loop variables                            */

			/* For each optional attribute search for     */
			/* acronym in 'admin_at' and record           */
			/* appropriate link                           */

for                     /* all optional attributes                    */
   (j = 0; j < adm_octp->nr_opt; j++)
{                       /* calculate AT index of attribute            */
  if                    /* attribute not in AT                        */
    ((**opt_list = d26_g13_atacr_to_idx(adm_octp->opt_anym[j])) < 0)
  {                     /* log error                                  */
    dce_svc_printf(GDS_S_OCT_AT_INVALID_MSG,adm_octp->opt_anym[j]);
    return(D26_ERROR);
  }
  else
  {                     /* look whether the attribute is in the list  */
			/* already                                    */
    tsearch_result = D27_TSEARCH(svc_c_sev_fatal,(void *)*opt_list,idx_root,
      d26_g27_cmp_idx);
    if                  /* attribute is new                           */
      (*tsearch_result == (void *)*opt_list)
    {                   /* put index into the binary tree             */
      (*opt_list)++;
    }
  }
}

for                     /* all superclasses                           */
   (j = 0; d26_oct[adm_octp->link].d26c_sup_cl[j] != D26_ROOT; j++)
{                       /* call function recursively                  */
  octp = d26_oct + d26_oct[adm_octp->link].d26c_sup_cl[j];
  if                    /* optional list cannot be generated          */
    (d26_g16_opt_list(admin_oct + octp->d26c_kind,idx_root,opt_list)
     != D2_NO_ERR)
  {                     /* return error                               */
    return(D26_ERROR);
  }
}
			/* If DSA object, then add the index of the   */
			/* DN-list attribute to the optional attrib.  */
if                      /* DSA object class encountered               */
  (D2_ATTCMP(&d26_oct[adm_octp->link].d26c_ob_id,D2_ODSA) == 0)
{                       /* insert index of DN-list attribute into     */
			/* optional attributes array                  */
  **opt_list = d26_dnl_idx;
			/* put index into binary tree                 */
  tsearch_result = D27_TSEARCH(svc_c_sev_fatal,(void *)*opt_list,idx_root,
    d26_g27_cmp_idx);
  if                    /* attribute is new                           */
    (*tsearch_result == (void *)*opt_list)
  {                     /* increment optional attributes pointer      */
    (*opt_list)++;
  }
}

return(D2_NO_ERR);

}                       /* d26_g16_opt_list                           */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g21_cmp_srt                                     */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : Used by qsort to compare two srt name index fields. */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      srt1            void *      first SRT entry to compare        */
/*      srt2            void *      second SRT entry to compare       */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      -1              int         first SRT entry before second     */
/*       0                          SRT entries are equal             */
/*      +1                          first SRT entry after second      */
/*                                                                    */
/**********************************************************************/

static int d26_g21_cmp_srt(
    const void *srt1,
    const void *srt2)

{                       /* d26_g21_cmp_srt                            */

register D26_nam_at *nam_at1,*nam_at2;  /* loop variables             */

for                     /* all naming attributes in list              */
   (nam_at1 = ((D26_srt *)srt1)->d26s_nam_at,
    nam_at2 = ((D26_srt *)srt2)->d26s_nam_at; ; nam_at1++, nam_at2++)
{                       /* compare AT-indices                         */
  if                    /* first AT-index is smaller                  */
    (nam_at1->d26s_index < nam_at2->d26s_index)
  {                     /* return 'smaller'                           */
    return(-1);
  }
  else if               /* second index is smaller                    */
	 (nam_at1->d26s_index > nam_at2->d26s_index)
  {                     /* return ' greater'                          */
    return(1);
  }
  else if               /* both lists are terminated                  */
	 (nam_at1->d26s_index == -1)
  {                     /* return 'equal'                             */
    return(0);
  }
}

}                       /* d26_g21_cmp_srt                            */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g22_cmp_oct                                     */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : Used by qsort to compare two OCT octal object ids.  */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      oct1            void *      first OCT entry to compare        */
/*      oct2            void *      second OCT entry to compare       */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      -1              int         first OCT entry before second     */
/*       0                          OCT entries are equal             */
/*      +1                          first OCT entry after second      */
/*                                                                    */
/**********************************************************************/

static int d26_g22_cmp_oct(
    const void *oct1,
    const void *oct2)

{                       /* d26_g22_cmp_oct                            */

return(d27_303_cmp_obj_id(&((D26_oct *)oct1)->d26c_ob_id,
			  &((D26_oct *)oct2)->d26c_ob_id));

}                       /* d26_g22_cmp_oct                            */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g23_cmp_at                                      */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : Used by qsort to compare two AT octal object ids.   */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      at1             void *      first AT entry to compare         */
/*      at2             void *      second AT entry to compare        */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      -1              int         first SRT entry before second     */
/*       0                          SRT entries are equal             */
/*      +1                          first SRT entry after second      */
/*                                                                    */
/**********************************************************************/

static int d26_g23_cmp_at(
    const void *at1,
    const void *at2)

{                       /* d26_g23_cmp_at                             */

return(d27_303_cmp_obj_id(&((D26_at *)at1)->d26a_type,
			  &((D26_at *)at2)->d26a_type));
}                       /* d26_g23_cmp_at                             */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g24_cmp_filenr                                  */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : Used by qsort to compare file numbers from two      */
/*                Fnr_elem structures.                                */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      fnr1            void *      first file number to compare      */
/*      fnr2            void *      second file number to compare     */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      -1              int         first file number before second   */
/*       0                          SRT entries are equal             */
/*      +1                          first file number after second    */
/*                                                                    */
/**********************************************************************/

static int d26_g24_cmp_filenr(const void *fnr1,
    const void *fnr2)

{                       /* d26_g24_cmp_filenr                         */
if                      /* file numbers are equal                     */
  (((Fnr_elem *)fnr1)->value == ((Fnr_elem *)fnr2)->value)
{                       /* return 'equal'                             */
  return(0);
}
else if                 /* first file number smaller than second      */
       (((Fnr_elem *)fnr1)->value < ((Fnr_elem *)fnr2)->value)
{                       /* return 'smaller'                           */
  return(-1);
}
else
{                       /* return 'greater'                           */
  return(1);
}

}                       /* d26_g24_cmp_filenr                         */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g25_cmp_naming_types                            */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : Used by qsort to compare two naming acronyms.       */
/*                                                                    */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      acr1            void *      first acronym to compare          */
/*      acr2            void *      second acronym to compare         */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      -1              int         first acronym before second       */
/*       0                          acronyms are equal                */
/*      +1                          first acronym after second        */
/*                                                                    */
/**********************************************************************/

static int d26_g25_cmp_naming_types(
    const void *acr1,
    const void *acr2)

{                       /* d26_g25_cmp_naming_types                   */

			/* loop pointers to administrated AT          */
register Admin_at *adm_atp1,*adm_atp2;
register signed32 i;    /* loop variable                              */

for                     /* all Admin_at entries                       */
   (i = 0, adm_atp1 = admin_at; i < d26_noat; i++, adm_atp1++)
{                       /* check acronyms                             */
  if                    /* acronyms match                             */
    (!strcmp((char *)acr1,adm_atp1->acronym))
  {                     /* break loop                                 */
    break;
  }
}

if                      /* no AT entry was found                      */
  (i == d26_noat)
{                       /* return 'first acronym smaller'             */
  return(-1);
}

for                     /* all Admin_at entries                       */
   (i = 0, adm_atp2 = admin_at; i < d26_noat; i++, adm_atp2++)
{                       /* compare acronyms                           */
  if                    /* acronyms match                             */
    (!strcmp((char *)acr2,adm_atp2->acronym))
  {                     /* break loop                                 */
    break;
  }
}

if                      /* no AT entry was found                      */
  (i == d26_noat)
{                       /* return 'second acronym smaller'            */
  return(1);
}

if                      /* first AT entry before second               */
  (adm_atp1->link < adm_atp2->link)
{                       /* return 'first acronym smaller'             */
  return(-1);
}
else if                 /* second AT entry before first               */
       (adm_atp2->link < adm_atp1->link)
{                       /* return 'second acronym smaller'            */
  return(1);
}
else
{                       /* return equality                            */
  return(0);
}

}                       /* d26_g25_cmp_naming_types                   */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g26_cmp_ocl_arcs                                */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 12.12.90                                            */
/*                                                                    */
/* DESCRIPTION  : Used by qsort to compare two object class arcs.     */
/*                An arc is smaller than the other, when the          */
/*                filenumber of the assigned OCT-entry is smaller     */
/*                than the other ones'.                               */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      arc1            void *      first object class arc to compare */
/*      arc2            void *      second object class arc to compare*/
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      -1              int         first obj. class arc before second*/
/*       0                          object class arcs are equal       */
/*      +1                          first obj. class arc after second */
/*                                                                    */
/**********************************************************************/

static int d26_g26_cmp_ocl_arcs(
    const void *arc1,
    const void *arc2)

{

signed16 filenr1, filenr2;  /* file number of OCT entries             */

filenr1 = d26_oct[((D26_ocl_arc *)arc1)->d26s_oclass].d26c_filenr;
filenr2 = d26_oct[((D26_ocl_arc *)arc2)->d26s_oclass].d26c_filenr;

if                      /* first file number is smaller               */
  (filenr1 < filenr2)
{                       /* return 'first arc smaller'                 */
  return(-1);
}
else if                 /* file numbers are equal                     */
       (filenr1 == filenr2)
{                       /* return 'equal arcs'                        */
  return(0);
}
else                    /* return 'first arc greater'                 */
{
  return(1);
}

}                       /* d26_g26_cmp_ocl_arcs                       */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g27_cmp_idx                                     */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : Used by qsort to compare two indices (shorts).      */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      idx1            void *      first index to compare            */
/*      idx2            void *      second index to compare           */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      -1              int         first index before second         */
/*       0                          indices are equal                 */
/*      +1                          first index after second          */
/*                                                                    */
/**********************************************************************/

static int d26_g27_cmp_idx(
    const void *idx1,
    const void *idx2)

{

if                      /* indices are equal                          */
  (*(short *)idx1 == *(short *)idx2)
{                       /* return 'equal'                             */
  return(0);
}
else if                 /* first index is smaller                     */
       (*(short *)idx1 < *(short *)idx2)
{                       /* return 'smaller'                           */
  return(-1);
}
else
{                       /* return 'greater'                           */
  return(1);
}

}                       /* d26_g27_cmp_idx                            */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g31_cmp_name_str                                */
/*                                                                    */
/* AUTHOR       : E. Kraemer, DAP 11                                  */
/* DATE         : 09.03.92                                            */
/*                                                                    */
/* DESCRIPTION  : The function compares the naming attributes of the  */
/*                two SRT entries. If they are different, it returns  */
/*                D2_TRUE, otherwise it calls itself recursively      */
/*                with the superior structure rules.                  */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      adm_srtp1       Admin_srt * pointer to first ...              */
/*      adm_srtp2       Admin_srt * pointer to second ...             */
/*                                  ... administrated SRT entry       */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_TRUE     Short       name structures are different         */
/*      D2_FALSE    Short       name structures are equal             */
/*                                                                    */
/**********************************************************************/

static boolean d26_g31_cmp_name_str(
    Admin_srt *adm_srtp1,
    Admin_srt *adm_srtp2)

{                       /* d26_g31_cmp_name_str                       */

			/* loop pointers to administrated SRT         */
register Admin_srt *adm_srtp_loop1;
register Admin_srt *adm_srtp_loop2;

register signed16 i;    /* loop variables                             */

if                      /* counts of naming attributes differ         */
  (adm_srtp1->nr_nattr != adm_srtp2->nr_nattr)
{                       /* return 'name structures differ'            */
  return(TRUE);
}

for                     /* all naming acronyms                        */
   (i = 0; i < adm_srtp1->nr_nattr; i++)
{                       /* compare them                               */
  if                    /* acronyms differ                            */
    (strcmp(adm_srtp1->name_attr[i],adm_srtp2->name_attr[i]))
  {                     /* return 'name structures differ'            */
    return(TRUE);
  }
}

if                      /* both structure rules are subordinate to    */
			/* root                                       */
  (adm_srtp1->par_ob_nr == 0 && adm_srtp2->par_ob_nr == 0)
{                       /* return 'name structures equal'             */
  return(FALSE);
}
else if                 /* one structure rule is subordinate to root  */
       (adm_srtp1->par_ob_nr == 0 || adm_srtp2->par_ob_nr == 0)
{                       /* return 'name structures differ'            */
  return(TRUE);
}

for                     /* all Admin_srt entries                      */
   (i = 0, adm_srtp_loop1 = admin_srt; i < nr_adm_srt; i++, adm_srtp_loop1++)
{
  if                    /* object number and parent object numbers    */
			/* match                                      */
    (adm_srtp_loop1->ob_nr == adm_srtp1->par_ob_nr)
  {                     /* break loop                                 */
    break;
  }
}

if                      /* parent entry not found                     */
  (i == nr_adm_srt)
{                       /* name structures differ (not ok!)           */
			/* log error, return 'name structures differ' */
  dce_svc_printf(GDS_S_SRT_SUP_MISSING_MSG,adm_srtp1->par_ob_nr);
  return(TRUE);
}

for                     /* all Admin_srt entries                      */
   (i = 0, adm_srtp_loop2 = admin_srt; i < nr_adm_srt; i++, adm_srtp_loop2++)
{                       /* compare object number with parent number   */
  if                    /* object number and parent object numbers    */
			/* match                                      */
    (adm_srtp_loop2->ob_nr == adm_srtp2->par_ob_nr)
  {                     /* break loop                                 */
    break;
  }
}

if                      /* parent entry not found                     */
  (i == nr_adm_srt)
{                       /* name structures differ (not ok!)           */
			/* log error, return 'name structures differ' */
  dce_svc_printf(GDS_S_SRT_SUP_MISSING_MSG,adm_srtp2->par_ob_nr);
  return(TRUE);
}

return(d26_g31_cmp_name_str(adm_srtp_loop1,adm_srtp_loop2));

}                       /* d26_g31_cmp_name_str                       */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g41_read_srt_entry                              */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 03.02.93                                            */
/*                                                                    */
/* DESCRIPTION  : From known positions in the supplied srt_value the  */
/*                the SRT entry is read into an entry of the          */
/*                administrated SRT array.                            */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      srt_value       char *      SRT value from schema object      */
/*      srt_idx         short       index of SRT entry to be read     */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short       no error occurred                 */
/*      D2_ERROR                    insufficient information in       */
/*                                  SRT entry                         */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g41_read_srt_entry(
    char *srt_value,
    signed16 srt_idx)

{                       /* d26_g41_read_srt_entry                     */

Admin_srt *srtp;                    /* pointer to administrated SRT   */
char *srtv;                         /* pointer to unchanged SRT value */

register signed16 k;                /* loop variable                  */

srtp = admin_srt + srt_idx;
srtv = srt_value;
DCE_SVC_LOG((GDS_S_SRTENTR_READ_MSG,srt_value));

			/* Read the first 2 elements into admin_srt   */
sscanf(srt_value,"%d %d",&srtp->ob_nr,&srtp->par_ob_nr);

			/* Skip the first two elements                */
srt_value = d26_g51_skip_table_elements(srt_value,2);

for                     /* all acronyms of naming attributes          */
   (k = 0; *srt_value != D2_COLON && k < D2_NO_AVA;
    srt_value = d26_g51_skip_table_elements(srt_value,1), k++)
{                       /* read in the acronym                        */
  sscanf(srt_value,"%s",srtp->name_attr[k]);
}
srtp->nr_nattr = k;

if                      /* too much acronyms                          */
  (*srt_value != D2_COLON)
{                       /* log error and break                        */
  dce_svc_printf(GDS_S_BADSRT_ENTRY_MSG,srtv);
  return(D26_ERROR);
}
else
{                       /* skip the colon and read in object class    */
			/* acronym                                    */
  srt_value = d26_g51_skip_table_elements(srt_value,1);
  sscanf(srt_value,"%s",srtp->class_anym);
  srtp->link = -1;
}

return(D2_NO_ERR);

}                       /* d26_g41_read_srt_entry                     */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g42_read_oct_entry                              */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 05.02.93                                            */
/*                                                                    */
/* DESCRIPTION  : From known positions in the supplied oct_value      */
/*                the OCT entry is read into an entry of the          */
/*                administrated OCT array.                            */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      oct_value       char *      OCT entry from schema object      */
/*      oct_idx         short       index of OCT entry to be read     */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short       no error occurred                 */
/*      D2_ERROR                    insufficient information in       */
/*                                  OCT entry                         */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g42_read_oct_entry(
    char *oct_value,
    signed16 oct_idx)

{                       /* d26_g42_read_oct_entry                     */

char *octv;                         /* pointer to unchanged OCT value */
Admin_oct *octp;                    /* pointer to administrated OCT   */

register signed16 k;                /* loop variable                  */

octp = admin_oct + oct_idx;
octv = oct_value;
DCE_SVC_LOG((GDS_S_OCTENTR_READ_MSG,oct_value));

octp->used = FALSE;

			/* Read the first 5 elements into admin_oct   */
sscanf(oct_value,"%s %s %s %d %d",octp->acronym,octp->obj_id,octp->name,
       &octp->kind,&octp->file_nr);

			/* Skip the first 5 elements                  */
oct_value = d26_g51_skip_table_elements(oct_value,5);

			/* Process superclass acronyms                */
for                     /* all acronyms of superclasses               */
   (k = 0; k < D2_NO_SUPCL && *oct_value != D2_COLON;
    oct_value = d26_g51_skip_table_elements(oct_value,1), k++)
{                       /* read in the acronym                        */
  sscanf(oct_value,"%s",octp->super_anym[k]);
}
octp->nr_sup_cl = k;

if                      /* too much acronyms                          */
  (*oct_value != D2_COLON)
{                       /* log error and break                        */
  dce_svc_printf(GDS_S_BADOCT_ENTRY_MSG,octv);
  return(D26_ERROR);
}
else
{                       /* skip the colon                             */
  oct_value = d26_g51_skip_table_elements(oct_value,1);

			/* Process auxiliary class acronyms           */
  for                   /* all acronyms of auxiliary object classes   */
     (k = 0; k < D2_NO_AUXCL && *oct_value != D2_COLON;
      oct_value = d26_g51_skip_table_elements(oct_value,1), k++)
  {                     /* read in the acronym                        */
    sscanf(oct_value,"%s",octp->aux_anym[k]);
  }
  octp->nr_aux_cl = k;

  if                    /* too much acronyms                          */
    (*oct_value != D2_COLON)
  {                     /* log error and break                        */
    dce_svc_printf(GDS_S_BADOCT_ENTRY_MSG,octv);
    return(D26_ERROR);
  }
  else
  {                     /* skip the colon                             */
    oct_value = d26_g51_skip_table_elements(oct_value,1);

			/* Process mandatory attribute acronyms       */
    for                 /* all acronyms of mandatory attributes       */
       (k = 0; k < D2_NO_MAND && *oct_value != D2_COLON;
	oct_value = d26_g51_skip_table_elements(oct_value,1), k++)
    {                   /* read in the acronym                        */
      sscanf(oct_value,"%s",octp->mand_anym[k]);
    }
    octp->nr_mand = k;

    if                  /* too much acronyms                          */
      (*oct_value != D2_COLON)
    {                   /* log error and break                        */
      dce_svc_printf(GDS_S_BADOCT_ENTRY_MSG,octv);
      return(D26_ERROR);
    }
    else
    {                   /* skip the colon                             */
      oct_value = d26_g51_skip_table_elements(oct_value,1);

			/* Process optional attribute acronyms        */
      for               /* all acronyms of optional attributes        */
	 (k = 0; k < D2_NO_OPT && *oct_value != D2_EOS;
	  oct_value = d26_g51_skip_table_elements(oct_value,1), k++)
      {                 /* read in the acronym                        */
	sscanf(oct_value,"%s",octp->opt_anym[k]);
      }
      octp->nr_opt = k;

      if                /* too much acronyms                          */
	(*oct_value != D2_EOS)
      {                 /* log error and return                       */
	dce_svc_printf(GDS_S_BADOCT_ENTRY_MSG,octv);
	return(D26_ERROR);
      }
    }
  }
}

return(D2_NO_ERR);

}                       /* d26_g42_read_oct_entry                     */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g43_read_at_entry                               */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 05.02.93                                            */
/*                                                                    */
/* DESCRIPTION  : From known positions in the supplied at_value       */
/*                the AT entry is read into an entry of the           */
/*                administrated AT array.                             */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      at_value        char *      AT entry from schema object       */
/*      at_idx          short       index of AT entry to be read      */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short       no error occurred                 */
/*      D2_ERROR                    insufficient information in       */
/*                                  AT entry                          */
/*                                                                    */
/**********************************************************************/

static signed16 d26_g43_read_at_entry(
    char     *at_value,
    signed16 at_idx)

{                       /* d26_g43_read_at_entry                      */

char *atv;                          /* pointer to unchanged AT value  */
Admin_at *atp;                      /* pointer to administrated AT    */

atp = admin_at + at_idx;
atv = at_value;
DCE_SVC_LOG((GDS_S_ATENTR_READ_MSG,at_value));
atp->used = FALSE;

			/* Read all the elements into admin_at        */
sscanf(at_value,"%s %s %s %d %d %d %d %d %d %d",atp->acronym,atp->obj_id,
       atp->name,&atp->lbound,&atp->ubound,&atp->nr_recur,&atp->syntax,
       &atp->phon,&atp->acl,&atp->index_level);

return(D2_NO_ERR);

}                       /* d26_g43_read_at_entry                      */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_g51_skip_table_elements()                       */
/*                                                                    */
/* AUTHOR       : Kraemer, AP 11                                      */
/* DATE         : 01.06.92                                            */
/*                                                                    */
/* DESCRIPTION  :   The function skips <elements> elements of the     */
/*                  tab entry.                                        */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      tab_entry       char *  entry of a schema table (SRT,OCT,AT)  */
/*      elements        short   count of elements to be skipped       */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      != D2_NULL      char *      pointer to next element to be     */
/*                                  scanned                           */
/**********************************************************************/

static char *d26_g51_skip_table_elements(
    char *tab_entry,    /* IN    -  entry of a schema table           */
    signed16 elements)  /* IN    -  count of elements to be skipped   */

{                       /*  d26_g51_skip_table_elements               */

signed16 el_count;      /* count of skipped elements                  */
char     *trailer;      /* pointer to the remaining part of the table */
			/* entry                                      */

for                     /*  all characters of the table entry         */
   (trailer = tab_entry, el_count = 0; el_count < elements; )
{                       /*  check whether separator encountered       */
  if                    /*  separator encountered                     */
    (*trailer == D2_BLANK)
  {                     /*  increment count of skipped elements       */
    el_count++;
    while               /*  separators following                      */
	 (*trailer == D2_BLANK)
    {                   /*  increment trailer                         */
      trailer++;
    }                   
  }
  else if               /*  end of string encountered                 */
	 (*trailer == D2_EOS)
  {                     /*  break loop                                */
    break;
  }
else
{                     /*  increment trailer                         */
    trailer++;
  }                     
}                       

return(trailer);

}                       /*  d26_g51_skip_table_elements               */
