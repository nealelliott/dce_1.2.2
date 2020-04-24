/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26trans.c,v $
 * Revision 1.1.731.2  1996/02/18  19:46:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:39  marty]
 *
 * Revision 1.1.731.1  1995/12/08  15:48:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:05  root]
 * 
 * Revision 1.1.729.6  1994/10/05  15:39:17  keutel
 * 	OT 12481: multiple routing
 * 	[1994/10/05  14:37:23  keutel]
 * 
 * Revision 1.1.729.5  1994/07/06  15:07:07  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:14:19  marrek]
 * 
 * Revision 1.1.729.4  1994/06/21  14:45:58  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:34:51  marrek]
 * 
 * Revision 1.1.729.3  1994/05/10  15:52:49  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/04  14:26:28  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:25:08  marrek]
 * 
 * Revision 1.1.729.2  1994/03/23  15:13:34  keutel
 * 	March 1994 code drop
 * 	[1994/03/22  14:14:31  keutel]
 * 
 * 	March 1994 code drop
 * 	[1994/03/21  16:23:13  keutel]
 * 
 * Revision 1.1.729.1  1994/02/22  18:39:24  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:19:55  marrek]
 * 
 * Revision 1.1.727.4  1993/11/08  15:58:01  keutel
 * 	CR 9390
 * 	[1993/11/08  15:55:52  keutel]
 * 
 * Revision 1.1.727.3  1993/10/29  15:11:13  keutel
 * 	CR 9261
 * 	[1993/10/29  14:07:38  keutel]
 * 
 * Revision 1.1.727.2  1993/10/14  17:16:45  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:12:44  keutel]
 * 
 * Revision 1.1.727.1  1993/10/13  17:31:15  keutel
 * 	October 1993 code drop
 * 	[1993/10/13  17:09:28  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  15:30:12  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:19:14  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  14:53:13  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.4  1993/02/02  11:28:18  marrek
 * 	Endless loop during transfer of Dist-name-attributes corrected.
 * 	[1993/02/01  15:07:47  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:42:27  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:21:17  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:22:37  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:17:16  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  16:52:31  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:21:14  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:52:34  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:58:12  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char d26trans_rcsid[] = "@(#)$RCSfile: d26trans.c,v $ $Revision: 1.1.731.2 $ $Date: 1996/02/18 19:46:14 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : d26trans                                            */
/*                                                                    */
/* AUTHOR       : Fischer, D AP 11                                    */
/* DATE         : 04.03.91                                            */
/*                                                                    */
/* SYNTAX       d26transfer <dir_old> <dir_new>                       */
/*                                                                    */
/* DESCRIPTION  : The Function modifies the db, if scheme is changed  */
/*                                                                    */
/* INPUT-PARAMETER: dir_old            name of directory with         */
/*                                     old data                       */
/*                  dir_new            name of directory with         */
/*                                     new data                       */
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
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module-identification                          */
/*                                                                    */
/**********************************************************************/


/**********************************************************************/
/*                                                                    */
/*                    I N C L U D E S                                 */
/*                                                                    */
/**********************************************************************/
#include <d26dsa.h>
#include <d26isam.h>
#include <d26svcis.h>

#include <dce/d27util.h>
#include <dce/dcesvcmsg.h>

#include <stdio.h>
#include <malloc.h>
#include <errno.h>
#include <locale.h>

/**********************************************************************/
/*                                                                    */
/*                    D E F I N E S                                   */
/*                                                                    */
/**********************************************************************/

#define D26_SCHEME          "/scheme"   /* name of used files         */
#define D26_DNLIST          "/dnlist"   /* name of used files         */

#define D26_OLD             0           /* identifier of old scheme   */
#define D26_NEW             1           /* identifier of new scheme   */
#define D26_NO_SCHEME       2           /* no. of schemas (old + new) */

/* Constants for function d26_t13_open_file */
#define D26_I18_FILE_CLOSED     -1
#define D26_I19_NO_FILETABLES    2       /* number of filetables      */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                    D E F I N T I O N S                             */
/*                                                                    */
/**********************************************************************/

/******** EXPORT ********/

				  /* name of directory of database    */
 char         d26_install[D26_NO_SCHEME][D27_LFILE_NAME];
				  /* Distinguished-Name-list in core  */
 D26_dndesc   *d26_dnlist[D26_NO_SCHEME] = {NULL,NULL};
 D26_srt      *d26_srt[D26_NO_SCHEME]; /* Structure Rule Table        */    
 D26_oct      *d26_oct[D26_NO_SCHEME]; /* Object Class Table          */
 D26_at       *d26_at[D26_NO_SCHEME];     /* Attribute Description Table      */
 D26_fildes   *d26t_ofildes[D26_NO_SCHEME];/* object file description         */
 D26_fildes   *d26t_afildes[D26_NO_SCHEME];/* attribut file description       */
 signed32     d26_nosrt[D26_NO_SCHEME];   /* maximum number of naming         */
					  /* attributes in SRT                */
 signed32     d26_nooct[D26_NO_SCHEME];   /* max. number of object classes    */
					  /* in OCT                           */
 signed32     d26_noat[D26_NO_SCHEME];    /* maximum number of attributes     */
					  /* in AT                            */
 signed32     d26_nofile[D26_NO_SCHEME];  /* number of object files           */
 signed32     d26_nafile[D26_NO_SCHEME];  /* number of attribute files        */

 boolean      d26_reset;      /* indicates that ISAM index must */
			      /* be reset                       */
/******** LOCAL  ********/

static signed32 d26_oreclen[D26_NO_SCHEME]; /* maximal record length (objects)*/
static signed32 d26_areclen[D26_NO_SCHEME]; /* maximal record length (attr.)  */
static String d26_ob_record[D26_NO_SCHEME]; /* record containing objects      */
static String d26_a_record[D26_NO_SCHEME];  /* record containing attribute val*/
				   /* 2D array of positions of naming */ 
static signed16 *d26_name_pos[D26_NO_SCHEME];
				   /* attrs in each object file        */
static signed16 *d26_at_pos[D26_NO_SCHEME]; /* 2D array of positions of other */
					    /* attrs in each object file      */
static  byte   *d26_obid_str[D26_NO_SCHEME];/* string which holds all octal   */
				            /* object ids for classes + attrs */
static  signed16 *d26_idx_str[D26_NO_SCHEME]; /*string of indices to attrs in */
					      /* the AT and classes in the OCT*/
static D26_nam_at *d26_nam_at[D26_NO_SCHEME]; /* naming attributes in SRT     */
					      /* in each object file          */
static D26_ocl_arc *d26_ocl_arc[D26_NO_SCHEME];/*arcs of Objectclasses in SRT */
static byte d26_cntr_bl_old[D27_CNTRY_BITLIST_LEN];
					/* country bitlist */
					/* object identifiers for     */
					/* soundex algorithms         */
static D2_obj_id d26_soundex_obid[D26_NO_SCHEME];

static   signed16        *srt_table;    /* mapping table of srt             */
static   signed16        *at_table;     /* mapping table of at              */

/* for optimization */
static   boolean      at_equal;          /* ATs differ only in indizes     */
static   boolean      at_idx_changed;    /* some AT indizes changed        */
static   signed16    *at_idxc_table;     /* contains changed indizes       */

	/* extern functions */
   /*** IMPORT from cisam     ***/
extern int iswrite(int isfd, String record);
extern int isread (int isfd, String record, register int mode);
extern int isstart (int isfd, D26_keydesc *keydesc, int keylen, String record, int mode);
extern int isuniqueid(int isfd, signed32 *rec_nr);

   /** IMPORT from d26tutil.c ***/
extern signed16 d26_t21_upd_core_dnlist(signed32 sch);
extern signed16 d26_t22_upd_file_dnlist (D2_attr_mod type, 
	D2_name_string old_dn, D2_dnint dnint, D2_name_string new_dn, 
	signed32 occurrency, boolean new_version, signed32 *dnlist_core_idx, 
	signed32 sch);
extern signed16 d26_t23_set_dnlistkey (signed16 key, signed32 sch);
extern signed16 d26_t24_get_updinfo (D26_update_info *upd_info, signed32 sch);
extern signed16 d26_t25_open_dnlistfile(signed32 sch);
extern signed16 d26_t26_close_dnlistfile(signed32 sch);
extern signed16 d26_t34_attr_search (signed32 sch, D2_a_type *attr_typ);
extern void d26_t35_select_key (signed32 sch, D26_keydesc *key, signed16 pos,
  	signed16 at_idx, signed16 filenr);
extern void  d26_t37_get_dninfo (String record, D26_dninfo *dninfo,
	D2_name_string *dname, signed16 np_count);
extern signed16 d26_t38_get_name (signed32 sch, D2_name_string dn, 
	D26_dninfo *dninfo);
extern signed16 d26_t42_add_index(signed16 at_idx, signed16 fnr, int isfd, 
	boolean add_idx);

	/* local functions */
static  void d26_t02_make_srt_table(void);
static  void d26_t03_make_at_table(void);
static  void d26_t04_transfer(D26_dninfo *, D2_name_string, signed16, signed32,
			    signed32);
static  void d26_t05_trans_attr(signed16, signed32 *, signed32, signed16, signed16, signed16);
static  void d26_t06_trans_value(signed32, signed16, char *, char *, signed16 *,
			       signed16 *, signed16);
static  signed32 d26_t08_read_file(signed16, D26_dninfo *, D2_name_string, signed16,
			    signed32, signed32);
static  signed32 d26_t12_write_new_record(String, int, String);
static  int d26_t13_open_file(signed16, signed16, signed16);
static  signed16 d26_t14_open_files(void);
static  signed16 d26_t15_close_files(void);
static  signed16 d26_t16_convert_record(D26_dninfo *, signed16, signed16,
	    D2_name_string, D2_name_string, signed32 *, signed32, signed16 *);
static  boolean  d26_t17_comp_srt(void);
static  boolean  d26_t18_comp_oct(void);
static  signed16 d26_t19_change_indizes(void);
static  signed16 d26_t20_copy_db(void);

/**********************************************************************/
/*                                                                    */
/*                    C O D E                                         */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : main                                                */
/*                                                                    */
/* AUTHOR       : Fischer, D AP 11                                    */
/* DATE         : 04.03.91                                            */
/*                                                                    */
/* DESCRIPTION  : The Function modifies the db, if scheme is changed  */
/*                                                                    */
/* INPUT-PARAMETER: dir_old            name of directory with         */
/*                                     old data                       */
/*                  dir_new            name of directory with         */
/*                                     new data                       */
/*                  dir_log            directory for log-files        */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*                                                                    */
/* RETURNVALUE     : -                                                */
/*                                                                    */
/* STANDARDHEADER  : -                                                */
/*                                                                    */
/* GLOBAL  DATA  (read only):   all                                   */
/*                                                                    */
/* GLOBAL  DATA  (changed):     all                                   */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*              d26_i10_upd_core_dnlist()                             */
/*                                                                    */
/* REMARKS      :  call: d26transfer <dir_old> <dir_new> <dir_log>    */
/*                                                                    */
/*exoff ***************************************************************/

int main
(
  int  argc,
  char *argv[]
)

{   
/**********************************************************************/
/*                                                                    */
/*                    D E C L A R A T I O N S                         */
/*                                                                    */
/**********************************************************************/

char    scheme_dir[D27_LFILE_NAME];     /* name of scheme directory   */
D26_dninfo dninfo;                      /* information about DNs of   */
					/* read objects               */
char    dname[D2_DNL_MAX + 1];          /* DN of read objects         */

error_status_t st;      /* error status for serviceability functions  */

/**********************************************************************/
/*                                                                    */
/*                    C O D E                                         */
/*                                                                    */
/**********************************************************************/

  setlocale(LC_ALL, "");

/* Check parameters */
if (argc <  4) {
    fprintf(stderr,"usage: %s <dir_old> <dir_new> <dir_log>\n",argv[0]);
    exit(1);
} else {
    strcpy(d26_install[D26_NEW],argv[2]);
    strcpy(d26_install[D26_OLD],argv[1]);
}

if                    /* working directory cannot be changed        */
  (chdir(argv[3]) < 0)
{                     /* give error note and exit                   */
  fprintf(stderr,"Unable to chdir to %s, errno: %d\n",argv[3],errno);
  fprintf(stderr,"usage: %s <dir_old> <dir_new> <dir_log>\n",argv[0]);
  exit(1);
}

d27_001_svc_init(argc,argv, &st);
if (st != svc_s_ok)
{
  fprintf(stderr,"Unable to initialize serviceability, status: 0x%lx\n",
	  st);
}
dce_svc_printf(GDS_S_ENTRY_TRANSFER_MSG);

/* Reading old and new scheme */
sprintf(scheme_dir,"%s",d26_install[D26_OLD]);
if (d26_mi9_read_scheme(scheme_dir,
      &d26_nosrt[D26_OLD], &d26_srt[D26_OLD],
      &d26_nam_at[D26_OLD], &d26_name_pos[D26_OLD], &d26_ocl_arc[D26_OLD],
      &d26_nooct[D26_OLD], &d26_oct[D26_OLD], &d26_idx_str[D26_OLD],
      &d26_noat[D26_OLD], &d26_at[D26_OLD], &d26_at_pos[D26_OLD],
      &d26_obid_str[D26_OLD], &d26_nofile[D26_OLD],
      &d26t_ofildes[D26_OLD], &d26_oreclen[D26_OLD], &d26_ob_record[D26_OLD],
      &d26_nafile[D26_OLD], &d26t_afildes[D26_OLD],
      &d26_areclen[D26_OLD], &d26_a_record[D26_OLD],
      d26_cntr_bl_old,&d26_soundex_obid[D26_OLD]) != D2_NO_ERR)
{
    exit(3);
}
DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, "old schema file read"));

sprintf(scheme_dir,"%s",d26_install[D26_NEW]);
if (d26_mi9_read_scheme(scheme_dir,
      &d26_nosrt[D26_NEW], &d26_srt[D26_NEW],
      &d26_nam_at[D26_NEW], &d26_name_pos[D26_NEW], &d26_ocl_arc[D26_NEW],
      &d26_nooct[D26_NEW], &d26_oct[D26_NEW], &d26_idx_str[D26_NEW],
      &d26_noat[D26_NEW], &d26_at[D26_NEW], &d26_at_pos[D26_NEW],
      &d26_obid_str[D26_NEW], &d26_nofile[D26_NEW],
      &d26t_ofildes[D26_NEW], &d26_oreclen[D26_NEW], &d26_ob_record[D26_NEW],
      &d26_nafile[D26_NEW], &d26t_afildes[D26_NEW],
      &d26_areclen[D26_NEW], &d26_a_record[D26_NEW],
      d27_country_bitlist,&d26_soundex_obid[D26_NEW]) != D2_NO_ERR)
{
    exit(4);
}
DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, "new schema file read"));

/* Reading old and new distinguished name list */
if (d26_t21_upd_core_dnlist(D26_OLD) == D2_ERROR) {
    exit(5);
}
DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, "old DN-List read"));

if (d26_t21_upd_core_dnlist(D26_NEW) == D2_ERROR) {
    exit(6);
}
DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, "new DN-List read"));

/* Making mapping tables */
d26_t02_make_srt_table();
DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG,"SRT mapping table ready"));
d26_t03_make_at_table();
DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, "AT mapping table ready"));

/********************/
/* open C-ISAM-files*/
/********************/

/* optimization */
if (at_equal && d26_t17_comp_srt() && d26_t18_comp_oct() &&
    !D27_ATTCMP(&d26_soundex_obid[D26_OLD],&d26_soundex_obid[D26_NEW]))
{
  DCE_SVC_LOG((GDS_S_TRF_SCHEMES_EQUAL_MSG));

  if (d26_t20_copy_db() != D2_NOERROR)
    exit(7);			/* SVC call in function	*/

  if (at_idx_changed)
  {
    if(d26_t14_open_files() != D2_NO_ERR)
      exit(8);
    else
      DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, "C-ISAM-files are opened"));

    if (d26_t19_change_indizes() == D2_ERROR)
       exit(9);

    /* Close old and new C-ISAM-files */
    d26_t15_close_files();
  }

} else {

  if(d26_t14_open_files() != D2_NO_ERR)
    exit(10);
  else
    DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, "C-ISAM-files are opened"));

  /********************/
  /* Transfer of data */
  /********************/
  /* The objects in the object files are searched beginning under root */
  dninfo.d26_rd_anz = 0;
  d26_t04_transfer(&dninfo,(D2_name_string) dname,0,D26_ROOT,D26_ROOT);
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,"d26_t04_transfer",D2_NOERROR));

  /* Closing old and new DN list file */
  if (d26_t26_close_dnlistfile(D26_OLD) == D2_ERROR) {
    DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, "error closing old dnlist file"));
  }
  if (d26_t26_close_dnlistfile(D26_NEW) == D2_ERROR) {
    DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, "error closing new dnlist file"));
  }
  /* Close old and new C-ISAM-files */
  d26_t15_close_files();
}

dce_svc_printf(GDS_S_EXIT_TRANSFER_MSG);

exit(0);

}      /* main */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_t02_make_srt_table                              */
/*                                                                    */
/* AUTHOR       : Fischer, D AP 11                                    */
/* DATE         : 04.03.91                                            */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  : The function makes a mapping table of the assigned  */
/*                old and new object numbers. An old and a new object */
/*                number are assigned, if they have the same sequence */
/*                of naming attributes leading to this object number. */
/*                A new object will only be in the mapping table, if  */
/*                it assigns to one of the old scheme. All old data,  */
/*                which belong to an object number without assigned   */
/*                new object number, will be lost.                    */
/*                The indices of the mapping table are corresponding  */
/*                to the indices of the old srt. The mapping table    */
/*                itself contains the indices of the new srt.         */
/*                                                                    */
/* INPUT-PARAMETER:  -                                                */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*                                                                    */
/* RETURNVALUE     : -                                                */
/*                                                                    */
/* STANDARDHEADER  : -                                                */
/*                                                                    */
/* GLOBAL  DATA  (read only):   d26_srt[D26_OLD]                     */
/*                              d26_srt[D26_NEW]                     */
/*                                                                    */
/* GLOBAL  DATA  (changed):     srt_table                             */
/*                                                                    */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS: calloc()                         */
/*                                                                    */
/* REMARKS      :  -                                                  */
/*                                                                    */
/*exoff ***************************************************************/

static  void d26_t02_make_srt_table(void)

{   
/**********************************************************************/
/*                                                                    */
/*                    D E C L A R A T I O N S                         */
/*                                                                    */
/**********************************************************************/

D26_srt     old_srt, new_srt;           /* old and new structure rule */
signed16    assigned;                   /* Flag: true, if an object of*/
					/* the old srt assigns to one */
					/* of the new srt             */

D26_nam_at  *old_nam_at;                /* naming attributes in old   */
D26_nam_at  *new_nam_at;                /* and new structure rules    */

					/* loop variables for old and */
					/* new SRT                    */
register signed32     i,j;
register D26_srt     *old_srtp, *new_srtp;

/**********************************************************************/
/*                                                                    */
/*                    C O D E                                         */
/*                                                                    */
/**********************************************************************/

/* Allocating memory for the mapping table */
if ((srt_table = 
    (signed16 *)D27_CALLOC(svc_c_sev_fatal,d26_nosrt[D26_OLD],
     sizeof(signed16))) == NULL) 
{
    exit(11);
}

/* Making mapping table */
for (i=0; i<d26_nosrt[D26_OLD]; i++) {
    old_srt = d26_srt[D26_OLD][i];
    for (j=0; j<d26_nosrt[D26_NEW]; j++) {
	new_srt = d26_srt[D26_NEW][j];
	assigned = FALSE;
	for(old_srtp = &old_srt, new_srtp = &new_srt;;
	    old_srtp = d26_srt[D26_OLD] + old_srtp->d26s_par_nr,
	    new_srtp = d26_srt[D26_NEW] + new_srtp->d26s_par_nr)
	{
	    for(old_nam_at = old_srtp->d26s_nam_at,
		new_nam_at = new_srtp->d26s_nam_at;
		old_nam_at->d26s_index > -1 && new_nam_at->d26s_index > -1;
		old_nam_at++, new_nam_at++) {
		if (D27_ATTCMP
		    (&d26_at[D26_OLD][old_nam_at->d26s_index].d26a_type,
		     &d26_at[D26_NEW][new_nam_at->d26s_index].d26a_type))
		{
		 break;
		}
	    }
	    if ((old_nam_at->d26s_index == new_nam_at->d26s_index) &&
		(new_nam_at->d26s_index == -1))
	     /* naming attributes of the both objects are equal */
	    {
	      if (old_srtp->d26s_par_nr == -1 && new_srtp->d26s_par_nr == -1)
	      {
		  /* parent of both objects is root */
		  assigned = TRUE;
		  break;
	      }
	      if (old_srtp->d26s_par_nr == -1 || new_srtp->d26s_par_nr == -1)
	      {
		  /* one parent is root, one is not root, */
		  /* therefore the objects do not assign  */
		  assigned = FALSE;
		  break;
	      }
	  } else  /* naming attributes not equal */
	  {
	      break;
	  }
	}
	if (assigned == TRUE) {
	    /* the object in the new srt assigns to the one in the old srt */
	    srt_table[i] = j;
	    break;  /* continue with next one in old srt */
	}
    }
    if (assigned == FALSE) {
	/* no objects of the new srt assign to the object of the old srt */
	srt_table[i] = -1;
	/* continue with next one in old srt */
    }
}

}   /* d26_t02_make_srt_table */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_t03_make_at_table                               */
/*                                                                    */
/* AUTHOR       : Fischer, D AP 11                                    */
/* DATE         : 04.03.91                                            */
/*                                                                    */
/* DESCRIPTION  : The function makes a mapping table of the assigned  */
/*                old and new at indices. An old and a new at entry   */
/*                are assigned, if they have the same attribute type. */
/*                An old attribute type will only be in the mapping   */
/*                table if it assigns to one of the new scheme.       */
/*                The indices of the mapping table are corresponding  */
/*                to the indices of the new at. The mapping table     */
/*                itself contains the indices of the old at.          */
/*                                                                    */
/* INPUT-PARAMETER:  -                                                */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*                                                                    */
/* RETURNVALUE     : -                                                */
/*                                                                    */
/* STANDARDHEADER  : -                                                */
/*                                                                    */
/* GLOBAL  DATA  (read only):   d26_at[D26_OLD]                       */
/*                              d26_at[D26_NEW]                       */
/*                                                                    */
/* GLOBAL  DATA  (changed):     at_table                              */
/*                              at_equal                              */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS: calloc()                         */
/*                                   d26_t34_attr_search()            */
/* REMARKS      :  -                                                  */
/*                                                                    */
/*exoff ***************************************************************/

static  void d26_t03_make_at_table(void)

{   
/**********************************************************************/
/*                                                                    */
/*                    D E C L A R A T I O N S                         */
/*                                                                    */
/**********************************************************************/

signed16   new_at_idx, old_at_idx;         /* index of attr in the at */
D26_at     *at_new, *at_old;

/**********************************************************************/
/*                                                                    */
/*                    C O D E                                         */
/*                                                                    */
/**********************************************************************/

/* Allocating memory for the mapping table */
if ((at_table = (signed16 *)D27_CALLOC(svc_c_sev_fatal, d26_noat[D26_NEW],
	sizeof(signed16))) == NULL) 
{
    exit(12);
}

/* allocating memory for index table */
if (d26_noat[D26_OLD] == d26_noat[D26_NEW] &&
    d26_nofile[D26_OLD] == d26_nofile[D26_NEW] &&
    d26_nafile[D26_OLD] == d26_nafile[D26_NEW])
{
  if ((at_idxc_table = (signed16 *)D27_CALLOC(svc_c_sev_fatal,d26_noat[D26_NEW],
	sizeof(signed16))) == NULL) {
      exit(13);
  }
  at_equal = TRUE;
  at_idx_changed = FALSE;
} else {
  at_equal = FALSE;
}

/* Making mapping table */
for (new_at_idx=0; new_at_idx<d26_noat[D26_NEW]; new_at_idx++) {
    /* Search new attribute type in the old at */
    if ((old_at_idx = d26_t34_attr_search(D26_OLD,
		      &d26_at[D26_NEW][new_at_idx].d26a_type)) < 0) {
	/* no attribute of the old at assign to one of the new at */
	/* therefore the attribute can not be transferred */
	at_table[new_at_idx] = -1;
    } else {
	/* Check if the syntax of the attribute is changed */
	if (d26_at[D26_NEW][new_at_idx].d26a_syntax !=
		    d26_at[D26_OLD][old_at_idx].d26a_syntax) {
	    /* The syntax of the two attributes is different */
	    /* therefore the attribute can not be transferred */
	    at_table[new_at_idx] = -1;
	} else {
	  /* The attribute in the old at assigns to the one in the new at */
	  at_table[new_at_idx] = old_at_idx;
	  /* check wether the ATs are equal                               */
	  if (at_equal && new_at_idx == old_at_idx)
	  {
	    at_old = d26_at[D26_OLD] + old_at_idx;
	    at_new = d26_at[D26_NEW] + new_at_idx;
	    if (at_old->d26a_ubound == at_new->d26a_ubound &&
		at_old->d26a_nr_recur == at_new->d26a_nr_recur &&
		at_old->d26a_phon == at_new->d26a_phon &&
		at_old->d26a_filenr == at_new->d26a_filenr &&
		!memcmp(at_old->d26a_field_pos,at_new->d26a_field_pos,
			d26_nofile[D26_OLD] * sizeof(signed16)))
	    {
	      if ((at_old->d26a_ind_level == 0 &&
		   at_new->d26a_ind_level > 0) ||
		  (at_old->d26a_ind_level > 0 &&
		   at_new->d26a_ind_level == 0)) /* a C-ISAM-index has to */
	      {                                  /* be added or deleted   */
		 at_idxc_table[new_at_idx] = TRUE;
		 at_idx_changed            = TRUE;
	      } else {
		 at_idxc_table[new_at_idx] = FALSE;
	      }
	    } else {
	      at_equal = FALSE;
	    }
	  } else {
	    at_equal = FALSE;
	  }
	}
    }
}

}   /* d26_t03_make_at_table */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_t04_transfer                                    */
/*                                                                    */
/* AUTHOR       : Fischer, D AP 11                                    */
/* DATE         : 04.03.91                                            */
/*                                                                    */
/* DESCRIPTION  : The function makes the transfer of the given object */
/*                and all its subordinates.                           */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*              dninfo      D26_dninfo *    information about DN of   */
/*                                          object to transfer        */
/*              dname       D2_name_string  DN of object to transfer  */
/*              np_count    signed16           count of significant      */
/*                                          nameparts                 */
/*              oldsupocl   signed16           superior objectclass      */
/*              newsupocl   Short           superior objectclass      */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*                                                                    */
/* RETURNVALUE     : -                                                */
/*                                                                    */
/* STANDARDHEADER  : -                                                */
/*                                                                    */
/* GLOBAL  DATA  (read only):   -                                     */
/*                                                                    */
/* GLOBAL  DATA  (changed):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                  d26_t08_read_file()                               */
/*                  d27_303_cmp_obj_id()                              */
/*                  d27_301_oid_str()                                 */
/*                                                                    */
/* REMARKS      :  -                                                  */
/*                                                                    */
/*exoff ***************************************************************/

static  void d26_t04_transfer
(
  D26_dninfo    *dninfo,   
  D2_name_string dname,  
  signed16       np_count, 
  signed32       oldsupocl,
  signed32       newsupocl
)
{   
/**********************************************************************/
/*                                                                    */
/*                    D E C L A R A T I O N S                         */
/*                                                                    */
/**********************************************************************/

signed16    parent;                 /* object number of given object  */
				    /* already searched files         */
signed16    filenr ;                /* current file number            */
D26_nam_at  *nam_at;                /* pointer to nam_at              */

char        log_comment[200];       /* comment for logging            */
String      log_ptr;                /* pointer to log-message         */

register    signed16       j,k;     /* loop variables                 */
				    /* indices to structure-rule- and */
				    /* object class tables            */
register    signed16    srt_idx, oct_idx, new_oct_idx;

register    D26_oct     *octp;      /* local ptr to OCT array         */
register    D26_srt     *srtp;      /* local ptr to SRT array         */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

parent = dninfo->d26_rd_anz == 0 ? D26_ROOT :
	 dninfo->d26_rdn[dninfo->d26_rd_anz - 1].d26_r_idx;
/* Go through the old srt and look for an object with the given parent */
for (srt_idx=0; srt_idx<d26_nosrt[D26_OLD]; srt_idx++)
{
    /* The parent of the object must be the given one           */
    if (d26_srt[D26_OLD][srt_idx].d26s_par_nr == parent)
    {
      if(srt_table[srt_idx] == -1)
      {
	sprintf(log_comment,"t04: cannot map name structure:");
	for(srtp = d26_srt[D26_OLD] + srt_idx,
	    log_ptr = log_comment + strlen(log_comment);
	    srtp >= d26_srt[D26_OLD];
	    srtp = d26_srt[D26_OLD] + srtp->d26s_par_nr,
	    log_ptr += strlen(log_ptr))
	{
	  *log_ptr++ = '/';
	  for (nam_at = srtp->d26s_nam_at; nam_at->d26s_index != -1;
	       nam_at++)
	  {
	    if (nam_at != srtp->d26s_nam_at)
	    {
	      log_ptr += strlen(log_ptr);
	      *log_ptr++ = ',';
	    }
	    d27_301_oid_str(&d26_at[D26_OLD][nam_at->d26s_index].d26a_type,
			  log_ptr);
	  }
	  log_ptr--;
	}
	DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
      }
      else
      {
	for (filenr = 1; filenr < d26_nofile[D26_OLD]; filenr++)
	{
	  /* Look for all file numbers in the oct */
	  for (j=0;
	      (oct_idx=d26_srt[D26_OLD][srt_idx].d26s_ocl_arc[j].d26s_oclass)
		 != -1;
	       j++)
	  {
	    /* The file number must exist in the oct (without objects0)*/
	    if (filenr == d26_oct[D26_OLD][oct_idx].d26c_filenr)
	    {
		  /* search for object class in new OCT                 */
		  for (k=0, new_oct_idx = -1, octp = d26_oct[D26_NEW];
		       k < d26_nooct[D26_NEW]; k++, octp++)
		  {
		    if (d27_303_cmp_obj_id(&octp->d26c_ob_id,
			&d26_oct[D26_OLD][oct_idx].d26c_ob_id) == 0)
		    {
		       new_oct_idx = k;
		       break;
		    }
		  }
		  if (new_oct_idx >= 0)
		  {
		     /* object class is found in the new oct */
		     /* search for children            */
		     dninfo->d26_filenr = filenr;

		      if(d26_t08_read_file(srt_idx,dninfo,dname,np_count,
					   oldsupocl,newsupocl)
			 != D2_NO_ERR)
		      {
			  exit(14);
		      }
		      d26_reset = TRUE;
		      break; /* continue with next filenumber */
		  }
	      } /* if */
	  } /* for */
	} /* for */
      } /* else */
    } /* if */
} /* for */

}    /* d26_t04_transfer */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_t05_trans_attr                                  */
/*                                                                    */
/* AUTHOR       : Fischer, D AP 11                                    */
/* DATE         : 04.03.91                                            */
/*                                                                    */
/* DESCRIPTION  : The function makes the transfer of one attribute.   */
/*                It transfers all attribute values of one attribute  */
/*                from the given old position (object file or attri-  */
/*                bute file) to the new position (object file or      */
/*                attribute file). This depends on the recurring no.  */
/*                If it >=1, the no. of values is limited and the     */
/*                values are in the object file,                      */
/*                if it <=0, the no. of values is unlimited and the   */
/*                values are in the attribute file.                   */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*                                                                    */
/* RETURNVALUE     : -                                                */
/*                                                                    */
/* STANDARDHEADER  : -                                                */
/*                                                                    */
/* GLOBAL  DATA  (read only):   -                                     */
/*                                                                    */
/* GLOBAL  DATA  (changed):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                  ldint()                                           */
/*                  stlong()                                          */
/*                  isstart()                                         */
/*                  isread()                                          */
/*                  d26_t06_trans_value()                             */
/*                  d26_t12_write_new_record()                        */
/*                  d26_t13_open_file()                               */
/*                  d26_i55_sel_rec_key()                             */
/*                  d27_301_oid_str()                                 */
/*                                                                    */
/* REMARKS      :  -                                                  */
/*                                                                    */
/*exoff ***************************************************************/

static  void d26_t05_trans_attr(
  signed16     old_filenr,
  signed32    *new_oct_idcs,
  signed32     rec_nr,
  signed16     at_idx,
  signed16     o_pos,
  signed16     n_pos)

{   

/**********************************************************************/
/*                                                                    */
/*                    D E C L A R A T I O N S                         */
/*                                                                    */
/**********************************************************************/

signed16       n_noval;                /* numbers of values to transfer  */
signed16       old_count;              /* count of old attribute values  */
signed16       new_count;              /* count of new attribute values  */
signed16       o_off,n_off;            /* offsets to next values         */
signed16       oo_recnr;               /* record number of old object    */
signed16       oa_filenr,na_filenr;    /* numbers of attribute files     */
signed16       count_pos;              /* position of value counter      */
signed16       syntax;                 /* syntax of attribute            */
				       /* length must be checked before  */

signed32       isfd[D26_NO_SCHEME];    /* ISAM-file descriptors of       */
				       /* attribute files                */
char        filename[D27_LFILE_NAME];  /* C-ISAM File-Name           */
D26_keydesc key;                       /* ISAM-key description           */
signed32         mode;                 /* ISAM read mode                 */

char        log_comment[200];          /* comment for logging            */

register    signed16       i,j;        /* loop variables                 */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

d27_301_oid_str(&d26_at[D26_NEW][at_idx].d26a_type,log_comment);

syntax      = d26_at[D26_NEW][at_idx].d26a_syntax;

old_count = ldint(d26_ob_record[D26_OLD] + o_pos);
n_noval = old_count < d26_at[D26_NEW][at_idx].d26a_nr_recur ||
	  d26_at[D26_NEW][at_idx].d26a_nr_recur == 0 ?
		 old_count : d26_at[D26_NEW][at_idx].d26a_nr_recur;
if                      /*  more old than new values                  */
  (old_count > n_noval)
{                       /*  log that                                  */
  sprintf(log_comment + strlen(log_comment),": %d values dropped",
	  old_count - n_noval);
  DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
}                       /*                                            */
count_pos = n_pos;

o_pos += INTSIZE;
n_pos += INTSIZE;

/* There are 4 cases:                                                   */
/*  1   old at object       new at object       record -> record        */
/*  2   old at object       new not at object   record -> attr file     */
/*  3   old not at object   new at object       attr file -> record     */
/*  4   old not at object   new not at object   attr file -> attr file  */

oa_filenr = d26_at[D26_OLD][at_table[at_idx]].d26a_filenr;
na_filenr = d26_at[D26_NEW][at_idx].d26a_filenr;
if (oa_filenr < 0)
{
    /* The old attribute is stored at object */
    if (na_filenr < 0)
    {
	/* Case1: both attributes are stored at object (record -> record) */
	/* Transfer the values of the attribute */
	for (i = 0, new_count = 0; i < n_noval; i++)
	{
	  d26_t06_trans_value(new_oct_idcs[i],at_idx,
	      d26_ob_record[D26_OLD] + o_pos,
	      d26_ob_record[D26_NEW] + n_pos,&o_off,&n_off,o_pos);
	  /* the new_oct_idcs-parameter is only used for type D2_OBJ_CLASS */
	  o_pos += o_off;
	  n_pos += n_off;
	  if (n_off != 0)
	  {
	    new_count++;
	  }
	}
    }
    else
    {
	/* Case2: The old attribute is stored at the object, the */
	/* new one not (record -> attribute file) */

	/* Open new attribute file */
	new_count = 0;
	if (old_count > 0 &&
	    (isfd[D26_NEW] = d26_t13_open_file(D26_NEW,D26_ATT,na_filenr))
	    >= 0)
	{
	  sprintf(filename, "%s%s%d", d26_install[D26_NEW],D26_ATTR,
		  na_filenr);
	/* Transfer the values of the attribute one by one */
	  for (i = 0; i < n_noval; i++)
	  {
	  /* transfer attribute value                               */
	  /* The memory for the new attribute record must be cleared*/
	    for (j = 0; j < d26t_afildes[D26_NEW][na_filenr].d26_rlen; j++)
	    {
	      d26_a_record[D26_NEW][j]='\0';
	    }
	    /* Write object file number and object unique id in record */
	    stint(d26_oct[D26_NEW][new_oct_idcs[0]].d26c_filenr,
		  d26_a_record[D26_NEW]);
	    stlong(rec_nr, d26_a_record[D26_NEW] + INTSIZE);
	    d26_t06_trans_value(new_oct_idcs[0],at_idx,
		  d26_ob_record[D26_OLD] + o_pos,
		  d26_a_record[D26_NEW] + INTSIZE + LONGSIZE,
		  &o_off,&n_off,o_pos);
	    o_pos += o_off;
	    /* Write new record in attribute file */
	    if (n_off > 0)
	    {
	      if(d26_t12_write_new_record(filename,isfd[D26_NEW],
		 d26_a_record[D26_NEW]) == D2_NO_ERR)
	      {
		new_count++;
	      }
	    }
	  } /* for */
	  d26t_afildes[D26_NEW][na_filenr].d26_in_use--;
      }
    }
}
else
{
    /* Case3: The old attribute is not stored at the object */
    /* Read the record from the old attribute file */
    new_count = 0;
    if (old_count > 0 &&
	(isfd[D26_OLD] = d26_t13_open_file(D26_OLD,D26_ATT,oa_filenr)) >= 0)
    {
      /* Transfer the values of the attribute one by one */
      oo_recnr = ldlong(d26_ob_record[D26_OLD]);
      d26_i55_sel_rec_key(&key,d26_at[D26_OLD] + at_table[at_idx],D2_EQUAL,
			  D23_READ);
      stint(old_filenr,d26_a_record[D26_OLD]);
      stlong(oo_recnr,d26_a_record[D26_OLD] + INTSIZE);
      if (isstart(isfd[D26_OLD],&key,INTSIZE + LONGSIZE,
	  d26_a_record[D26_OLD],ISEQUAL) < 0)
      {
	  /* hard error occurred */
	if (iserrno != ENOREC)
	{
	  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
	      svc_c_debug1,GDS_S_ISSTART_ERR, iserrno));
	}
	d26t_afildes[D26_OLD][oa_filenr].d26_in_use = FALSE;
      }
      else          /*  set query mode                            */
      {
	if            /*  new attribute is not stored at object   */
	  (na_filenr >= 0)
	{             /*  open the new attribute file             */
	  sprintf(filename, "%s%s%d", d26_install[D26_NEW],D26_ATTR,
		  na_filenr);
	  if ((isfd[D26_NEW] = d26_t13_open_file(D26_NEW,D26_ATT,
	      na_filenr)) < 0)
	  {
	    n_noval = 0;
	  }
	}
	mode      = ISCURR;

	for           /*  all new attribute values                  */
	   (i = 0, mode = ISCURR; i < old_count && new_count < n_noval;
	    i++, mode = ISNEXT)
	{             /*  read isam                                 */
	  if(isread(isfd[D26_OLD],d26_a_record[D26_OLD],mode) < 0)
	  {   
		      /*  check error                               */
	    switch(iserrno)
	    {
	      case  EENDFILE:   break;
	      default       :
		DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
		   svc_c_debug1,GDS_S_ISREAD_ERR, mode, iserrno));
		d26t_afildes[D26_OLD][oa_filenr].d26_in_use = FALSE;
		break;
	    }   
	  } else {      /*  compare key with record                   */
	    if          /*  record matches key                        */
	      (ldint(d26_a_record[D26_OLD]) == old_filenr &&
	       ldlong(d26_a_record[D26_OLD] + INTSIZE) == oo_recnr)
	    {           /*  convert data to new record                */
	    /* transfer attribute value                           */
	      if (na_filenr < 0)
	      {
	      /* Case3: only the new attribute is stored at the object*/
		d26_t06_trans_value(new_oct_idcs[0],at_idx,
		    d26_a_record[D26_OLD] + INTSIZE + LONGSIZE,
		    d26_ob_record[D26_NEW] + n_pos,&o_off,&n_off,o_pos);
		n_pos += n_off;
		if (n_off > 0)
		{
		  new_count++;
		}
	      }
	      else
	      {
	      /* Case4: old and new attribute are not stored at the */
	      /* object. The memory for the new attribute record    */
	      /* must be cleared.                                   */
		for (j = 0; j < d26t_afildes[D26_NEW][na_filenr].d26_rlen;
		     j++)
		{
		  d26_a_record[D26_NEW][j]='\0';
		}
	      /* Write object file number and object unique id in record */
		stint(d26_oct[D26_NEW][new_oct_idcs[0]].d26c_filenr,
		      d26_a_record[D26_NEW]);
		stlong(rec_nr, d26_a_record[D26_NEW] + INTSIZE);
		d26_t06_trans_value(new_oct_idcs[0],at_idx,
		    d26_a_record[D26_OLD] + INTSIZE + LONGSIZE,
		    d26_a_record[D26_NEW] + INTSIZE + LONGSIZE,
		    &o_off,&n_off,o_pos);
		/* Write new record in attribute file */
		  if (n_off > 0)
		  {
		    if(d26_t12_write_new_record(filename,isfd[D26_NEW],
		       d26_a_record[D26_NEW]) == D2_NO_ERR)
		    {
		      new_count++;
		    }
		  }
		}
	    }         /*                                            */
	  }           /*                                            */
	}             /*                                            */
	if            /*  new attribute is stored at object         */
	  (na_filenr >= 0)
	{             /*  reset the in use flag for the new attr.   */
		      /*  file                                      */
	  d26t_afildes[D26_NEW][na_filenr].d26_in_use--;
	}
      } /* else */
      d26t_afildes[D26_OLD][oa_filenr].d26_in_use--;
    }
}
stint(new_count, d26_ob_record[D26_NEW] + count_pos);

}    /* d26_t05_trans_attr */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_t06_trans_value                                 */
/*                                                                    */
/* AUTHOR       : Fischer, D AP 11                                    */
/* DATE         : 04.03.91                                            */
/*                                                                    */
/* DESCRIPTION  : The function makes the transfer of one attribute    */
/*                value. It checks all conditions for attribute values*/
/*                and tranfers one attribute value from the given     */
/*                old position to the new position. It returns the    */
/*                length of the tranferred value.                     */
/*                                                                    */
/* INPUT-PARAMETER:  at_idx     Short    at index of attribute        */
/*                   from_pos   char *   position in old record       */
/*                   to_pos     Char *   position in new record       */
/*                   o_pos      Short    position of attribute in old */
/*                                       record                       */
/*                                                                    */
/* OUTPUT-PARAMETER: f_off      Short *  offset to next old value     */
/*                   t_off      Short *  offset to next new value     */
/*                                       0 if no value is transferred */
/*                                                                    */
/* RETURNVALUE     : -                                                */
/*                                                                    */
/* STANDARDHEADER  : -                                                */
/*                                                                    */
/* GLOBAL  DATA  (read only):   -                                     */
/*                                                                    */
/* GLOBAL  DATA  (changed):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                  stint()                                           */
/*                  ldint()                                           */
/*                  stlong()                                          */
/*                  ldlong()                                          */
/*                  D2_ATTCMP()                                       */
/*                  d26_t22_upd_file_dnlist()                         */
/*                  d26_soundex()                                     */
/*                                                                    */
/* REMARKS      :  -                                                  */
/*                                                                    */
/*exoff ***************************************************************/

static  void d26_t06_trans_value(
  signed32   new_oct_idx,
  signed16   at_idx,
  char      *from_pos,
  char      *to_pos,
  signed16  *f_off,
  signed16  *t_off,
  signed16   o_pos)

{
/**********************************************************************/
/*                                                                    */
/*                    D E C L A R A T I O N S                         */
/*                                                                    */
/**********************************************************************/

signed16       len;            /* length of old attribute value          */
D26_at         *old_at;        /* pointer to old AT entry                */
D26_at         *new_at;        /* pointer to new AT entry                */
signed16       old_max_len;    /* max. length of value in old AT         */
signed16       new_max_len;    /* max. length of value in new AT         */
signed16       old_phon_len;   /* old length of phonetical value         */
signed16       new_phon_len;   /* old length of phonetical value         */
signed16       add_len;        /* length of internal added characters    */
signed16       rep_len;        /* length of representation (if there is) */

D26_dninfo     dninfo;         /* dninfo for checking validity of DNs    */
signed32       o_dnl_idx;      /* DN-list-indices                        */
long           n_dnl_idx;      /* DN-list-indices                        */
signed32       n_acl_idx[D2_NOIDX_ACL];

register    signed16 i,j,idx_cnt;  /* loop varaible                      */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

new_at = d26_at[D26_NEW] + at_idx;
old_at = d26_at[D26_OLD] + at_table[at_idx];
new_max_len = new_at->d26a_ubound;
old_max_len = old_at->d26a_ubound;
new_phon_len = new_at->d26a_phon == TRUE ? D26_LPHON : 0;
old_phon_len = old_at->d26a_phon == TRUE ? D26_LPHON : 0;
add_len  = new_at->d26a_rep == D2_T61_PR_LIST ? 2 * D26_MAXVALPARTS :
	    (new_at->d26a_rep == D2_TLXNR_STX ? D26_MAXVALPARTS : 1);
rep_len  = new_at->d26a_rep == D2_T61_PR ? INTSIZE : 0;

switch(new_at->d26a_syntax)
{
  case D2_CEXACT:
  case D2_PRINTABLE:
  case D2_TIME:
  case D2_TLXNR_STX:
	/* <value>'\0'  or  <phon-value><main parts of attr value>'\0' */
	*f_off = old_phon_len + rep_len + old_at->d26a_ubound + add_len;
	*t_off = new_phon_len + rep_len + new_at->d26a_ubound + add_len;
	if (new_at->d26a_phon == TRUE)
	{
	    /* If phonetical matching is allowed, first the phon value */
	    /* must be tranferred */
	    from_pos += old_phon_len;
	    len = strlen(from_pos + rep_len);
	    len = len < new_at->d26a_ubound ? len : new_at->d26a_ubound;
	    d26_soundex((Octet_string) from_pos, len, (Octet_string) to_pos);
	    to_pos += D26_LPHON;
	}
	if (rep_len > 0) /* representation has to be stored */
	{
	   stint(ldint(from_pos),to_pos);
	   to_pos += rep_len;
	   from_pos += rep_len;
	}
	len = (new_at->d26a_ubound > old_at->d26a_ubound ?
		old_at->d26a_ubound : new_at->d26a_ubound) + add_len;
	memcpy(to_pos,from_pos,len);
	to_pos[len - 1] = D2_EOS;
	break;

  case D2_CIGNORE:
  case D2_CIGN_LIST  :
  case D2_COUNTRY_STX:
  case D2_NUMERIC:
  case D2_TELNR_STX:
  case D2_POST_AD_STX:
  case D2_IA5:
	/*                 <normed value>'\0'<exact value>'\0' */
	/* or: <phon-value><normed value>'\0'<exact value>'\0' */
       *f_off = old_phon_len + rep_len + 2 * (old_at->d26a_ubound + add_len);
       *t_off = new_phon_len + rep_len + 2 * (new_at->d26a_ubound + add_len);
	if (new_at->d26a_phon == TRUE)
	{
	    /* If phonetical matching is allowed, first the phon value */
	    /* must be tranferred */
	    from_pos += old_phon_len;
	    len = strlen(from_pos + rep_len);
	    len = len < new_at->d26a_ubound ? len : new_at->d26a_ubound;
	    d26_soundex((Octet_string) from_pos, len, (Octet_string) to_pos);
	    to_pos += D26_LPHON;
	}
	if (rep_len > 0) /* representation has to be stored */
	{
	   stint(ldint(from_pos),to_pos);
	   to_pos += rep_len;
	   from_pos += rep_len;
	}
	/* 2 values must be transferred: the normed and the exact value */
	len = (new_at->d26a_ubound > old_at->d26a_ubound ?
		old_at->d26a_ubound : new_at->d26a_ubound) + add_len;
	memcpy(to_pos,from_pos,len);
	to_pos[len - 1] = D2_EOS;
	from_pos += old_at->d26a_ubound + add_len;
	to_pos   += new_at->d26a_ubound + add_len;
	memcpy(to_pos,from_pos,len);
	to_pos[len - 1] = D2_EOS;
	break;
  case D2_OBJ_IDENT:
	if(!D2_ATTCMP(&new_at->d26a_type,D2_OBJ_CLASS))
	{
	  /* determine new oct index */
	  /* Store new oct index in new record */
	  if (new_oct_idx != D26_IDX_NOT_DEF)
	  {
	    stlong((signed32)new_oct_idx,to_pos);
	    *t_off = LONGSIZE;
	  }
	  else
	  {
	    *t_off = 0;
	  }
	  *f_off = LONGSIZE;
	  break;
	}
		    /* handle as octet string otherwise       */
		    /*   1. check length of structured values */
  case  D2_MHS_DLSP_STX:
  case  D2_MHS_ORADR_STX:
  case  D2_MHS_ORNAME_STX:
	if (ldint(d26_ob_record[D26_OLD] + o_pos + 2 * old_max_len + INTSIZE)
	    > new_max_len) /* value too long */
	{
	  *f_off = 2 * (old_max_len + INTSIZE);
	  *t_off = 0;
	  break;
	}
  case  D2_GUIDE  :
  case  D2_TTXID_STX:
  case  D2_FAXNR_STX:
  case  D2_PSAP_STX:
	if (ldint(d26_ob_record[D26_OLD] + o_pos + old_max_len) >
	    new_max_len) /* value too long */
	{
	  *f_off = old_max_len + INTSIZE;
	  *t_off = 0;
	  break;
	}
  case  D2_ASN1:
  case  D2_ANY_STX:
  case  D2_OCTET  :
  case  D2_PASSWD_STX:
	*f_off = old_at->d26a_ubound + INTSIZE;
	*t_off = new_at->d26a_ubound + INTSIZE;
	len = new_at->d26a_ubound > old_at->d26a_ubound ?
		old_at->d26a_ubound : new_at->d26a_ubound;
	memcpy(to_pos,from_pos,len);
	to_pos   += new_at->d26a_ubound;
	from_pos += old_at->d26a_ubound;
	len = ldint(from_pos);
	from_pos += INTSIZE;
	len = len < new_at->d26a_ubound ? len : new_at->d26a_ubound;
	stint(len,to_pos);
	to_pos   += INTSIZE;
	if                  /* MHS attribute is transferred           */
	  (new_at->d26a_syntax == D2_MHS_DLSP_STX ||
	   new_at->d26a_syntax == D2_MHS_ORADR_STX ||
	   new_at->d26a_syntax == D2_MHS_ORNAME_STX)
	{                   /* do the same for the exact value        */
	  *f_off *= 2;
	  *t_off *= 2;
	  len = new_at->d26a_ubound > old_at->d26a_ubound ?
		old_at->d26a_ubound : new_at->d26a_ubound;
	  memcpy(to_pos,from_pos,len);
	  to_pos   += new_at->d26a_ubound;
	  from_pos += old_at->d26a_ubound;
	  len = ldint(from_pos);
	  len = len < new_at->d26a_ubound ? len : new_at->d26a_ubound;
	  stint(len,to_pos);
	}
	break;
  case D2_INTEGER:
  case D2_BOOLEAN:
	/* <value> */
	stlong(ldlong(from_pos), to_pos);
	*f_off = *t_off = LONGSIZE;
	break;
  case D2_PR_DM_STX:
  case D2_MHS_PR_DM_STX:
	/* <value1>...<value10> */
	for (i = 0; i < new_at->d26a_ubound; i++)
	{
	    stlong(ldlong(from_pos + i * LONGSIZE), (to_pos + i * LONGSIZE));
	}
	*f_off = *t_off = new_at->d26a_ubound * LONGSIZE;
	break;
  case D2_DISTNAME:
	/* <DN-index> */
	if((o_dnl_idx = ldlong(from_pos)) < 0)
	{
	  n_dnl_idx = o_dnl_idx;
	}
	else
	{
	  dninfo.d26_rd_anz = 0;
	  if (d26_t38_get_name(D26_NEW, (D2_name_string)
	       d26_dnlist[D26_OLD][o_dnl_idx].d26_dnname,
	       &dninfo) != D2_NO_ERR)
	  {
	    /* DN from DN-List is not valid in the new SRT */
	    *f_off = LONGSIZE;
	    *t_off = 0;
	    break;
	  }
	  n_dnl_idx = D26_IDX_NOT_DEF;
	  d26_t22_upd_file_dnlist(D2_ADD_AV,(D2_name_string)NULL,
	      d26_dnlist[D26_OLD][o_dnl_idx].d26_dnint,
	      (D2_name_string)d26_dnlist[D26_OLD][o_dnl_idx].d26_ename,
	      1,TRUE,&n_dnl_idx, D26_NEW);
	}
	stlong(n_dnl_idx, to_pos);
	*f_off = *t_off = LONGSIZE;
	break;
  case D2_ACCL:
	/* <index1>...<index20> */
	for (i = 0; i < D2_NOACCESS_RIGHTS; i++)
	{
	  for (j = 0, idx_cnt = 0; j < D2_NOIDX_ACL; j++)
	  {
	    n_acl_idx[j] = D26_EOL;
	    if((o_dnl_idx = ldlong(from_pos + (4*i+j) * LONGSIZE)) < 0)
	    {
	      n_acl_idx[idx_cnt] = o_dnl_idx; idx_cnt++;
	    }
	    else
	    {
	     dninfo.d26_rd_anz = 0;
	       if (d26_t38_get_name(D26_NEW, (D2_name_string)
		   d26_dnlist[D26_OLD][o_dnl_idx].d26_dnname,
		   &dninfo) == D2_NO_ERR)
	       {
		 /* name structure is valid */
		 d26_t22_upd_file_dnlist(D2_ADD_AV,(D2_name_string)NULL,
		    d26_dnlist[D26_OLD][o_dnl_idx].d26_dnint,
		    (D2_name_string)d26_dnlist[D26_OLD][o_dnl_idx].d26_ename,
		    1,TRUE,&n_acl_idx[idx_cnt], D26_NEW);
		 idx_cnt++;
	       }
	     }
	   }
	   if (n_acl_idx[0] == D26_EOL)
	   {
	     /* record may not be transferred because there are no valid */
	     /* access rights in this access class */
	     *t_off = 0; *f_off = i;
	     return;
	   }
	   for (j = 0; j < D2_NOIDX_ACL; j++)
	   {
	     stlong(n_acl_idx[j], (to_pos + (4*i+j) * LONGSIZE));
	   }
	 }
	 *f_off = *t_off = D2_ACLIDX_MAX * LONGSIZE;
	 break;
default:
	/* No value will be transferred */
	*f_off = *t_off = 0;
	break;
}

}    /* d26_t06_trans_value */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_t08_read_file                                   */
/*                                                                    */
/* AUTHOR       : Fischer, D AP 11                                    */
/* DATE         : 04.03.91                                            */
/*                                                                    */
/* DESCRIPTION  : The function opens an object file for reading (if   */
/*                it is not open) and starts a query to search for    */
/*                the children of the given object having the given   */
/*                object number.                                      */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*                   obnr    Short  object no. of actual object       */
/*                   dninfo  D26_dninfo *   information about the     */
/*                                  parent object.                    */
/*                   dname   D2_name_string DN of parent object       */
/*                   np_count Short count of significant nameparts    */
/*                   oldsupocl  Short  index of parent object class   */
/*                   newsupocl  Short  index of parent object class   */
/*                                                                    */
/* OUTPUT-PARAMETER: isfd    Int *  file descriptor of object file    */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*                                                                    */
/* STANDARDHEADER  : -                                                */
/*                                                                    */
/* GLOBAL  DATA  (read only):   -                                     */
/*                                                                    */
/* GLOBAL  DATA  (changed):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                ldint()                                             */
/*                ldlong()                                            */
/*                stint()                                             */
/*                stlong()                                            */
/*                isstart()                                           */
/*                isread()                                            */
/*                d26_t04_transfer()                                  */
/*                d26_t12_write_new_record()                          */
/*                d26_t13_open_file()                                 */
/*                d26_t16_convert_record()                            */
/*                d26_t35_select_key()                                */
/*                d26_u02_fill_field()                                */
/*                                                                    */
/* REMARKS      :  -                                                  */
/*                                                                    */
/*exoff ***************************************************************/

static  signed32 d26_t08_read_file(
  signed16     obnr,
  D26_dninfo  *dninfo,
  D2_name_string dname,
  signed16     np_count,
  signed32     oldsupocl,
  signed32     newsupocl)

{
/**********************************************************************/
/*                                                                    */
/*                    D E C L A R A T I O N S                         */
/*                                                                    */
/**********************************************************************/

signed16   file_no[D26_NO_SCHEME]; /* old object file number             */
int        isfd[D26_NO_SCHEME];/* C-Isam-File-descriptor                 */
signed32   mode;               /* ISAM read mode                         */
char       filename[D27_LFILE_NAME];   /* name of object file            */

signed16   s_idx;              /* SRT-index of RDN to use                */
signed16   pos;                /* position of naming attribute in record */
D26_keydesc key;               /* ISAM key structure                     */
signed16   len;                /* length of used index                   */
D26_rdninfo *rdn;              /* last RDN of object                     */

boolean    go_on;              /* indicates whether loop must be cont.   */
signed32   new_oct_idx;        /* index of new OCT-entry                 */
D2_name_string tail;           /* pointer to end of DN of object         */

signed32    rec_nr;            /* record number of object read           */
signed32    mk_idx;            /* DNL-index of master-knowledge attribute*/
signed16   ao_val;             /* validity for aliased object attribute  */
char    save[MAXKEYSIZE];      /* save key of record for reset           */
signed16   n_ava;              /* number of AVAs in last RDN             */
D26_ava *last_ava;             /* last AVA in last RDN                   */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

file_no[D26_OLD] = dninfo->d26_filenr;

if((isfd[D26_OLD] = d26_t13_open_file(D26_OLD,D26_OBJ,file_no[D26_OLD])) < 0)
{
	return(D2_ERROR);
}

    /* Set key to find the first record with the given object number */
    /* and namepart                                                  */
s_idx = np_count > 0 ? dninfo->d26_rdn[np_count - 1].d26_r_idx : obnr;
pos = d26_srt[D26_OLD][s_idx].d26s_nam_at[0].d26s_position[file_no[D26_OLD]];
d26_t35_select_key(D26_OLD, &key, pos,
	d26_srt[D26_OLD][s_idx].d26s_nam_at[0].d26s_index, file_no[D26_OLD]);
		  /*  fill key in record                        */
stint(obnr, d26_ob_record[D26_OLD] + key.k_part[0].kp_start);
		  /*  namepart of superior must be used         */

if(np_count > 0)
{
    rdn = dninfo->d26_rdn + np_count - 1;
    d26_u02_fill_field(D2_EOS,rdn->d26_r_ava[0].d26_a_len,
		     (char *) rdn->d26_r_ava[0].d26_a_val,
		     key.k_part[1].kp_leng,save);
    memcpy(d26_ob_record[D26_OLD] + key.k_part[1].kp_start,save,
		    key.k_part[1].kp_leng);
    len = key.k_part[1].kp_leng;
}
else
{
    len = 0;
}

if (isstart(isfd[D26_OLD],&key,INTSIZE + len,d26_ob_record[D26_OLD],ISEQUAL)
    < 0)
{
    /* no record found */
    if (iserrno == ENOREC)
    {
      d26t_ofildes[D26_OLD][file_no[D26_OLD]].d26_in_use--;
      return(D2_NO_ERR);
    }
    else
    {
    /* error from start query */
      DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
	svc_c_debug1,GDS_S_ISSTART_ERR, iserrno));
      return(D2_ERROR);
    }
}
else                /*  set query mode                            */
{
    mode      = ISCURR;
    go_on     = TRUE;
    d26_reset = FALSE;
    if                /*  base object is not root                   */
      (np_count > 0)
    {                 /*  set tail     to end of DN                 */
      n_ava = dninfo->d26_rdn[np_count-1].d26_r_nava;
      last_ava = dninfo->d26_rdn[np_count-1].d26_r_ava + n_ava - 1;
      tail = last_ava->d26_a_val + last_ava->d26_a_len;
      *tail++ = D2_RDN_SEP;
      *tail = '\0';
    } else {          /*  set name end to beginning of DN           */
      tail = dname;
    }                 /*                                            */
    while             /*  size and time limit not exceeded          */
	 (go_on == TRUE)
    {                 /*  read isam                                 */

		      /*  reading record */
      if (isread(isfd[D26_OLD],d26_ob_record[D26_OLD],mode) < 0)
      {
			/*  check error                               */
	switch(iserrno)
	{
	  case  EENDFILE:   go_on = FALSE;
			    break;
	  default       :
		    DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
		      svc_c_debug1,GDS_S_ISREAD_ERR, mode, iserrno));
			  d26t_ofildes[D26_OLD][file_no[D26_OLD]].d26_in_use--;
			    return(D2_ERROR);
	}
      } else {          /*  compare key with record                   */
	if              /*  record matches key                        */
	  (ldint(d26_ob_record[D26_OLD] + key.k_part[0].kp_start) == obnr &&
	   !memcmp(d26_ob_record[D26_OLD] + key.k_part[1].kp_start,
	   save,len))
	{               /*  convert data to new record                */
	  dninfo->d26_filenr = file_no[D26_OLD];
	  if            /*  record could be converted                 */
	    ((new_oct_idx = d26_t16_convert_record(dninfo,np_count,obnr,
	     dname,tail,&oldsupocl,newsupocl,&file_no[D26_NEW])) >= 0)
	  {             /*  open new object file                      */
	    DCE_SVC_LOG((GDS_S_TRF_REC_CONVERTED_MSG));
	    if((isfd[D26_NEW] = d26_t13_open_file(D26_NEW,D26_OBJ,
	       file_no[D26_NEW])) >= 0)
	    {
	      sprintf(filename,"%s%s%d",d26_install[D26_NEW],
				      D26_OBJECTS,file_no[D26_NEW]);
	      if(d26_t12_write_new_record(filename,isfd[D26_NEW],
		 d26_ob_record[D26_NEW]) == D2_NO_ERR)
			/*  save key from record                      */
	      {
		memcpy(save,
		    d26_ob_record[D26_OLD] + key.k_part[1].kp_start,
		    key.k_part[1].kp_leng);
		ao_val = ldint(d26_ob_record[D26_OLD] +
				key.k_part[2].kp_start);
		mk_idx = ldlong(d26_ob_record[D26_OLD] +
				key.k_part[3].kp_start);
		rec_nr = ldlong(d26_ob_record[D26_OLD]);

		d26_t04_transfer(dninfo,dname,np_count + 1,oldsupocl,new_oct_idx);
		if          /*  index was changed                 */
		  (d26_reset == TRUE)
		{           /*  reset key                             */
		  stint(obnr,d26_ob_record[D26_OLD] +key.k_part[0].kp_start);
		  memcpy(d26_ob_record[D26_OLD] +
			  key.k_part[1].kp_start,save,key.k_part[1].kp_leng);
		  stint(ao_val,d26_ob_record[D26_OLD] +
					      key.k_part[2].kp_start);
		  stlong(mk_idx,d26_ob_record[D26_OLD] +
					      key.k_part[3].kp_start);
		  stlong(rec_nr,d26_ob_record[D26_OLD]);
		  if    /*  error from start query                    */
		    (isstart(isfd[D26_OLD],&key,0,d26_ob_record[D26_OLD],
		     ISGREAT) < 0)
		  {     /*  check error number                        */
		    if  /*  serious error                             */
		      (iserrno != EENDFILE && iserrno != ENOREC)
		    {   /*  write error log                           */
		      DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
			svc_c_debug1,GDS_S_ISSTART_ERR, iserrno));
		    }   /*                                            */
		    go_on = FALSE;
		  }     /*                                            */
		  d26_reset = FALSE;
		}       /*                                            */
	      }
	    }
	  }             /*                                            */
	  mode = ISNEXT;
	} else {        /*  break loop                                */
	  go_on = FALSE;
	}               /*                                            */
      }                 /*                                            */
    }                   /*                                            */
    d26t_ofildes[D26_OLD][file_no[D26_OLD]].d26_in_use--;
}

return(D2_NO_ERR);

}           /*  d26_t08_read_file */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_t12_write_new_record                            */
/*                                                                    */
/* AUTHOR       : Fischer, D AP 11                                    */
/* DATE         : 04.03.91                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* INPUT-PARAMETER:  filename String name of object file              */
/*                   isfd    Int    file descriptor of attr.  file    */
/*                                  (=-1 if not open)                 */
/*                   record   String data to write                    */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*                                                                    */
/* STANDARDHEADER  : -                                                */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (changed):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                      iswrite()                                     */
/*                                                                    */
/* REMARKS      :                                                     */
/*                                                                    */
/*exoff ***************************************************************/

static  signed32 d26_t12_write_new_record(
  String    filename,
  int       isfd,
  String    record)

{
/**********************************************************************/
/*                                                                    */
/*                    D E C L A R A T I O N S                         */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/* Writing record in object file */
if (iswrite (isfd,record) < 0)
{
    /* error from writing record */
    DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, filename));
    DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
      svc_c_debug1,GDS_S_ISWRITE_ERR, iserrno));
    return(D2_ERROR);
}

return(D2_NOERROR);

}     /* d26_t12_write_new_record */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t16_convert_record                                */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        22.01.91                                              */
/*                                                                    */
/* DESCRIPTION: This function checks, whether all nameparts of the    */
/*              base object match. If so, it checks whether the new   */
/*              object class is defined. If so, it converts the data  */
/*              from the old record to the new one. Outsided attribute*/
/*              values are read, transferred and written into the     */
/*              attribute files as well as the DN-List-values.        */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              dninfo      D26_dninfo *    information about base    */
/*                                          object.                   */
/*              np_count    Short           count of nameparts in base*/
/*                                          object                    */
/*              ob_nr       Short           object number             */
/*              dname       D2_name_string  DN of the base object     */
/*              tail        D2_name_string  pointer to the end of the */
/*                                          DN of base object         */
/*              newsupocl   Short           sup. class of new object  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              oldsupocl   *Short          superior class            */
/*              new_filenr  *Short          new filenumber            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              D2_NO_ERR                   all data are converted.   */
/*              D26_ERROR                   error occurred.           */
/*                                                                    */
/* STANDARD-HEADER: None                                              */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*              d26_srt[]  D26_srt *       structure-rule-table       */
/*              d26_at[]   D26_at *        attribute table            */
/*              d26_ob_record String *      object record             */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*          ldlong()                                                  */
/*          ldint()                                                   */
/*          stlong()                                                  */
/*          stint()                                                   */
/*          isuniqueid()                                              */
/*          MA2_002_ERROR()                                           */
/*          d26_t05_trans_attr()                                      */
/*          d26_t13_open_file()                                       */
/*          d26_t34_attr_search()                                     */
/*          d26_t37_get_dninfo()                                      */
/*          d27_303_cmp_obj_id()                                      */
/*          d27_301_oid_str()                                         */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*inoff ***************************************************************/

static  signed16 d26_t16_convert_record(
  D26_dninfo     *dninfo,
  signed16       np_count,
  signed16       ob_nr,
  D2_name_string dname,
  D2_name_string tail,
  signed32      *oldsupocl,
  signed32       newsupocl,
  signed16      *new_filenr)

{                               /*  d26_t16_convert_record            */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* STATIC    ...    */
					/*attribute type of object class*/
static D2_a_type oc_type = {sizeof(D2_OBJ_CLASS) - 1,
			    (Ob_id_string)D2_OBJ_CLASS};
/* AUTO      ...    */

    signed16       filenr;     /* number of old ISAM file            */
    signed16       new_obj_no; /* object number of converted object  */

    D26_rdninfo *rdn;          /* pointer to RDN                 */
    D26_nam_at  *old_nam_at;   /* pointer to nam_at in SRT       */
    D26_nam_at  *new_nam_at;   /* pointer to nam_at in SRT       */

    String      np;            /* pointer to namepart in record  */

    signed32    new_oct_idx;   /* index of new OCT entry         */
    signed16    old_at_idx;    /* index in old at for object     */
			       /* class attribute                */
    signed16       new_at_idx; /* index in new at for attributes */
    signed16       old_max_len;/* maximum length of attribute    */
			       /* values in old AT               */
    signed16       new_max_len;/* maximum length of attribute    */
			       /* values in new AT               */
    signed16       syntax;     /* syntax of the attribute        */
    signed16       old_pos,new_pos;/* position of attribute in record*/
    signed16       oc_pos;         /* position of object class attr. */
    signed16       oc_count;       /* number of values of ocl. attr. */
    signed32       old_oct_idx;    /* oct index in old record        */
    signed32       ocl_tab[D26_OCL_VAL_COUNT];
    int            isfd;           /* C-ISAM-File descriptor         */
    int            isfd_a;         /* C-ISAM-File descriptor         */
    signed32       rec_nr;         /* C-Isam-record number           */
    boolean        alias_ocl;      /* Flag for alias-handling        */

    signed32       to_pos[D2_ACLIDX_MAX]; /* Dummies for checking    */
    signed16       t_off,f_off;
    signed32       oo_recnr;       /* old record number              */
    signed16       oa_filenr;      /* old attr. file number          */
    D26_keydesc key;               /* key for search in attr. file   */
    signed32       mode;           /* ISAM read mode                 */

    char       log_comment[200];   /* comment for logging        */
    signed16           phon_len;   /* length of phonetic value           */
    signed16       rep_len;        /* length of representation value     */

/* FASTAUTO  ...    */

register    signed16   i,j,k,m;    /* loop variables                     */
register    signed32   idx;        /*                                    */
register    signed16   old_count;  /* count of old attribute values      */
register    D26_srt    *srt;       /* pointer to srt entry               */
register    D26_oct    *octp;      /* local ptr to OCT array             */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

for (i = 0; i < D26_OCL_VAL_COUNT; i++) ocl_tab[i] = D26_IDX_NOT_DEF;

filenr = dninfo->d26_filenr;

for                     /*  all nameparts of base object              */
   (i = np_count - 1, rdn = dninfo->d26_rdn + i; i >= 0; i--, rdn--)
{                       /*  set name pointer                          */
  srt      = d26_srt[D26_OLD] + rdn->d26_r_idx;
  old_nam_at   = srt->d26s_nam_at;
  for                   /*  all AVAs of one namepart                  */
     (j = 0; old_nam_at[j].d26s_index > -1; j++)
  {
    rep_len = d26_at[D26_OLD][old_nam_at[j].d26s_index].d26a_rep
	      == D2_T61_PR ? INTSIZE : 0;
    phon_len = d26_at[D26_OLD][old_nam_at[j].d26s_index].d26a_phon == TRUE ?
							      D26_LPHON : 0;
    np = d26_ob_record[D26_OLD] + old_nam_at[j].d26s_position[filenr]
				+ INTSIZE + phon_len + rep_len;
    if                  /*  namepart does not match                   */
      (strncmp(np,(char *) rdn->d26_r_ava[j].d26_a_val,
	       rdn->d26_r_ava[j].d26_a_len) ||
       strlen(np) != rdn->d26_r_ava[j].d26_a_len)
    {                   /*  return don't use                          */
      return(D26_ERROR);
    }
  }
}

d26_t37_get_dninfo(d26_ob_record[D26_OLD],dninfo,&tail,np_count);
DCE_SVC_LOG((GDS_S_TRF_REC_FOUND_MSG, dname));

/* Determine the object class of the old object */
if ((old_at_idx = d26_t34_attr_search(D26_OLD, &oc_type)) < 0)
{    /* old at */
  DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, "Objectclass attribute not found"));
  return(D26_ERROR);
}
else
{
  oc_pos = d26_at[D26_OLD][old_at_idx].d26a_field_pos[filenr];

  if ((oc_count=ldint(d26_ob_record[D26_OLD] + oc_pos)) <= 0)
  {
  /* no object class existing */
    DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, "object class not found in record"));
    return(D26_ERROR);
  }
  else
  {
    old_oct_idx = ldlong(d26_ob_record[D26_OLD] + oc_pos + INTSIZE);
    *oldsupocl = old_oct_idx;
    alias_ocl = !D2_ATTCMP(&d26_oct[D26_OLD][old_oct_idx].d26c_ob_id,
			   D2_OALIAS);
  /* Search object class in new oct und get its index */
    new_oct_idx = -1L;
    for (i = 0, octp = d26_oct[D26_NEW]; i < d26_nooct[D26_NEW];
	 i++, octp++)
    {
      if (d27_303_cmp_obj_id(&octp->d26c_ob_id,
	  &d26_oct[D26_OLD][old_oct_idx].d26c_ob_id) == 0)
      {
	new_oct_idx = i;
      break;
      }
    }
    if (new_oct_idx == -1)
    {
      /* object class is not found in the new oct */
      /* record will not be transferred */
      strcpy(log_comment,"cannot map object class: ");
      d27_301_oid_str(&d26_oct[D26_OLD][old_oct_idx].d26c_ob_id,
		      log_comment + strlen(log_comment));
      DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
      return(D26_ERROR);
    }
    new_obj_no = srt_table[ob_nr];
    if (!alias_ocl)
    {
      /* Check if the found oct entry is among the object classes,    */
      /* wich are assigned to the new object number */
      for (i = 0,
	   idx = d26_srt[D26_NEW][new_obj_no].d26s_ocl_arc[0].d26s_oclass;
	   idx != -1;
	   i++, idx = d26_srt[D26_NEW][new_obj_no].d26s_ocl_arc[i].d26s_oclass)
      {
	if ((new_oct_idx == idx) &&
	    (d26_oct[D26_NEW][new_oct_idx].d26c_kind == D2_STRUCTURAL))
	{
	    /* Test wether the object has a correct parent in the new SRT*/
	    if (d26_srt[D26_NEW][new_obj_no].d26s_ocl_arc[i].d26s_supocl
		!= newsupocl)
	    {
		sprintf(log_comment,
		     "Object situated under unallowed parent:\n   %s",dname);
		DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
		return(D26_ERROR);
	    }
	  break;
	}
      }
      if (idx == -1)
      {
    /* Object class is not among the object classes which are assigned */
    /* to the new object number */
    /* record will not be transferred */
	strcpy(log_comment,"object class illegal for name structure: ");
	d27_301_oid_str(&d26_oct[D26_NEW][new_oct_idx].d26c_ob_id,
			log_comment + strlen(log_comment));
	DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
	return(D26_ERROR);
      }
    }
  /* Determine file number of new object file */
    if (alias_ocl)
    {
      /* Ocl.s in SRT-ocl_arc are sorted with ascending filenumbers */
      idx=d26_srt[D26_NEW][new_obj_no].d26s_ocl_arc[0].d26s_oclass;
      *new_filenr = d26_oct[D26_NEW][idx].d26c_filenr;
      strcpy(log_comment,"Object is an alias.");
      DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
    }
    else
    {
      *new_filenr = d26_oct[D26_NEW][new_oct_idx].d26c_filenr;
    }
    if((isfd = d26_t13_open_file(D26_NEW,D26_OBJ,*new_filenr)) < 0)
    {
  /* new object file cannot be opened, record may not be transferred */
      return(D26_ERROR);
    }
  /* Clear memory for new record  */
    for (i = 0; i < d26t_ofildes[D26_NEW][*new_filenr].d26_rlen; i++)
    {
      d26_ob_record[D26_NEW][i]='\0';
    }

    /* Check if all the mandatory attributes of the new object class are */
    /* present */
    /* if there are missing mandatory attributes of an auxiliary object- */
    /* class then only these aux.class will not be transferred           */

    for(k=0; k < oc_count; k++)   /* for all values of OCL-attribute */
    {
      if (k > 0) /* handle auxiliary objectclasses */
      {
	old_oct_idx = ldlong(d26_ob_record[D26_OLD] + oc_pos +
				     INTSIZE + k*LONGSIZE);
	new_oct_idx = -1;
	for (i = 0, octp = d26_oct[D26_NEW]; i < d26_nooct[D26_NEW];
	     i++, octp++)
	{
	  if (d27_303_cmp_obj_id(&octp->d26c_ob_id,
	      &d26_oct[D26_OLD][old_oct_idx].d26c_ob_id) == 0)
	  {
	    new_oct_idx  = i;
	    break;
	  }
	}
	if (new_oct_idx == -1)
	{
	  /* auxiliary object class is not found in the new oct */
	  strcpy(log_comment,"cannot map auxiliary object class: ");
	  d27_301_oid_str(&d26_oct[D26_OLD][old_oct_idx].d26c_ob_id,
			  log_comment + strlen(log_comment));
	  DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
	  continue;
	}
	else if (d26_oct[D26_NEW][new_oct_idx].d26c_kind != D2_AUXILIARY)
	{
	  /* auxiliary object class not of kind "AUXILIARY" in new OCT */
	  strcpy(log_comment,"cannot map auxiliary object class: ");
	  d27_301_oid_str(&d26_oct[D26_OLD][old_oct_idx].d26c_ob_id,
			  log_comment + strlen(log_comment));
	  DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
	  continue;
	}
	else
	{
	  /* check if aux. object class is among the allowed aux. Ocl.s */
	  /* of the structural object class                             */
	  for (i=0;(d26_oct[D26_NEW][ocl_tab[0]].d26c_aux_cl[i] != -1) &&
	       (d26_oct[D26_NEW][ocl_tab[0]].d26c_aux_cl[i] != new_oct_idx);
	       i++)
	  {}
	  if (d26_oct[D26_NEW][ocl_tab[0]].d26c_aux_cl[i] == -1)
	  {
	    strcpy(log_comment,
	   "auxiliary object class not assigned to structural object class: ");
	    d27_301_oid_str(&d26_oct[D26_OLD][old_oct_idx].d26c_ob_id,
			    log_comment + strlen(log_comment));
	    DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
	    continue;
	  }
	}
      }
      ocl_tab[k] = idx = new_oct_idx;

      for(i = 0, new_at_idx = d26_oct[D26_NEW][idx].d26c_mandatory[i];
	  new_at_idx != D26_ROOT;
	  i++, new_at_idx = d26_oct[D26_NEW][idx].d26c_mandatory[i])
      {
	if ((old_at_idx = at_table[new_at_idx]) == -1)
	{
	  /* this attribute does not exist in the old scheme */
	  if (k == 0) /* record will not be transferred */
	  {
	    strcpy(log_comment,"mandatory attribute missing in old schema: ");
	    d27_301_oid_str(&d26_at[D26_NEW][new_at_idx].d26a_type,
			    log_comment + strlen(log_comment));
	    DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
	    return(D2_ERROR);
	  }
	  else     /* attributes of aux. ocl. will not be transferred */
	  {
	    strcpy(log_comment,
	       "mandatory attribute of aux. ocl. missing in old schema: ");
	    d27_301_oid_str(&d26_at[D26_NEW][new_at_idx].d26a_type,
			    log_comment + strlen(log_comment));
	    DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
	    ocl_tab[k] = D26_IDX_NOT_DEF;
	    break;  /* continue with next auxiliary object class */
	  }
	}
	for (m = 0;
	    (d26_srt[D26_NEW][new_obj_no].d26s_nam_at[m].d26s_index > 0) &&
    (d26_srt[D26_NEW][new_obj_no].d26s_nam_at[m].d26s_index != new_at_idx);
	     m++)
	{}
	if (d26_srt[D26_NEW][new_obj_no].d26s_nam_at[m].d26s_index == -1)
	{               /* attribute is not naming (will be handled   */
			/* later)                                     */
	  old_pos = d26_at[D26_OLD][old_at_idx].d26a_field_pos[filenr];
	  if ((old_count = ldint(d26_ob_record[D26_OLD] + old_pos)) == 0)
	  {
	    /* mandatory attribute is not present in the record */
	    if (k > 0)
	    {
	      strcpy(log_comment,
	     "mandatory attribute of aux. ocl. missing in old object: ");
	      d27_301_oid_str(&d26_at[D26_NEW][new_at_idx].d26a_type,
			      log_comment + strlen(log_comment));
	      DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
	      ocl_tab[k] = D26_IDX_NOT_DEF;
	      break;
	    }
	    else
	    {
	      /* record will not be transferred */
	      strcpy(log_comment,"mandatory attribute missing in old object: ");
	      d27_301_oid_str(&d26_at[D26_NEW][new_at_idx].d26a_type,
			      log_comment + strlen(log_comment));
	      DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
	      return(D2_ERROR);
	    }
	  }
	  old_max_len = d26_at[D26_OLD][old_at_idx].d26a_ubound;
	  new_max_len = d26_at[D26_NEW][new_at_idx].d26a_ubound;
	  syntax      = d26_at[D26_OLD][old_at_idx].d26a_syntax;
	  j = 0;
	  if(new_max_len < old_max_len &&
	     (syntax == D2_GUIDE        || syntax == D2_TTXID_STX ||
	      syntax == D2_FAXNR_STX    || syntax == D2_PSAP_STX))
	  { /* new attribute length is shortened for a structured     */
	    /* attribute                                              */
	    for (j = 0, old_pos += INTSIZE; j < old_count;
		 j++, old_pos += old_max_len + INTSIZE)
	    {   /* all old attribute values                           */
	      if(ldint(d26_ob_record[D26_OLD] + old_pos + old_max_len) <=
		 new_max_len)
	      { /* attribute value must not be cut                    */
		break;
	      }
	    }
	  }
	  else if(new_max_len < old_max_len &&
	     (syntax == D2_MHS_DLSP_STX || syntax == D2_MHS_ORADR_STX ||
	      syntax == D2_MHS_ORNAME_STX))
	  { /* new attribute length is shortened for a MHS attribute  */
	    for (j = 0, old_pos += INTSIZE; j < old_count;
		 j++, old_pos += 2 * (old_max_len + INTSIZE))
	    {   /* all old attribute values                           */
	      if(ldint(d26_ob_record[D26_OLD] + old_pos + 2 * old_max_len +
		 INTSIZE) <= new_max_len)
	      { /* attribute value must not be cut                    */
		break;
	      }
	    }
	  }
	  else if (syntax == D2_DISTNAME)
	  {
	    /* check if name structure of attribute value is valid in the */
	    /* new SRT                                                    */
	    if ((oa_filenr=d26_at[D26_OLD][old_at_idx].d26a_filenr) > 0)
	    /* read values from attribute file */
	    {
	      j = old_count;
	      if ((isfd_a = d26_t13_open_file(D26_OLD,D26_ATT,oa_filenr))
		  >= 0)
	      {
		oo_recnr = ldlong(d26_ob_record[D26_OLD]);
		d26_i55_sel_rec_key(&key,d26_at[D26_OLD]+ old_at_idx,D2_EQUAL,
			    D23_READ);
		stint(filenr,d26_a_record[D26_OLD]);
		stlong(oo_recnr,d26_a_record[D26_OLD] + INTSIZE);
		if (isstart(isfd_a,&key,INTSIZE + LONGSIZE,
		    d26_a_record[D26_OLD],ISEQUAL) < 0)
		{
		  /* hard error occurred */
		  if (iserrno != ENOREC)
		  {
		    DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
		      svc_c_debug1,GDS_S_ISSTART_ERR, iserrno));
		  }
		  d26t_afildes[D26_OLD][oa_filenr].d26_in_use = FALSE;
		} else {
		  for           /*  all new attribute values                  */
		     (j = 0, mode = ISCURR; j < old_count;
		      j++, mode = ISNEXT)
		  {             /*  read isam                                 */
		    if(isread(isfd_a,d26_a_record[D26_OLD],mode) < 0)
		    {
				/*  check error                               */
		      switch(iserrno) {
			case  EENDFILE:   break;
			default       :
		    DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
		      svc_c_debug1,GDS_S_ISREAD_ERR, mode, iserrno));
			  d26t_afildes[D26_OLD][oa_filenr].d26_in_use = FALSE;
			  break;
		      }
		      if (j == 0) j = old_count; /* no values found */
		      break;
		    }
		    else        /*  compare key with record                   */
		    if          /*  record matches key                        */
		      (ldint(d26_a_record[D26_OLD]) == filenr &&
		       ldlong(d26_a_record[D26_OLD] + INTSIZE) == oo_recnr)
		    {           /*  convert data to new record                */
		      /* check attribute value                           */
		      d26_t06_trans_value(ocl_tab[k],new_at_idx,
			  d26_a_record[D26_OLD] + INTSIZE + LONGSIZE,
			  (char *) to_pos, &f_off,&t_off,old_pos);
		      if (t_off != 0)
		      {
			break;
		      }
		    }
		  } /* for all old values */
		}  /* else */
	      }  /* if isfd_a >= 0 */
	    } else
	    {           /* read values from object file */
	      for(j=0, old_pos += INTSIZE; j < old_count;
		  j++, old_pos += LONGSIZE)
	      {
		d26_t06_trans_value(ocl_tab[k],new_at_idx,
				    d26_ob_record[D26_OLD] + old_pos,
				    (char *) to_pos, &f_off, &t_off, old_pos);
		if (t_off != 0)
		{
		  break;
		}
	      }
	    }
	  } /* if syntax == D2_DISTNAME */

	  if (j == old_count)
	     /* no attribute value has a valid name structure */
	     /* or the allowed length                         */
	  {
	    if (k > 0)
	    {
	      strcpy(log_comment,
	      "no mandatory attribute value of aux. ocl. may be transferred: ");
	      d27_301_oid_str(&d26_at[D26_NEW][new_at_idx].d26a_type,
			      log_comment + strlen(log_comment));
	      DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
	      ocl_tab[k] = D26_IDX_NOT_DEF;
	      break;
	    }
	    else
	    {
	strcpy(log_comment,"no mandatory attribute value may be transferred: ");
	      d27_301_oid_str(&d26_at[D26_NEW][new_at_idx].d26a_type,
			      log_comment + strlen(log_comment));
	      DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
	      return(D2_ERROR);
	    }
	  } /* if j == old_count */
	} /* if attr. not naming */
      } /* for all mandatory attr. */

      /* check the ACL-attribute */
      for(new_at_idx=0;(new_at_idx < d26_noat[D26_NEW]) &&
			(D2_ATTCMP(&d26_at[D26_NEW][new_at_idx].d26a_type,
			D2_ACL)); new_at_idx++)
      {}
      if (new_at_idx < d26_noat[D26_NEW])
      {
	if ((old_at_idx = at_table[new_at_idx]) > -1)
	{
	  old_pos = d26_at[D26_OLD][old_at_idx].d26a_field_pos[filenr];
	  if (old_count = ldint(d26_ob_record[D26_OLD] + old_pos) == 1)
	  {
	    /* if there are no more valid access rights because of not */
	    /* valid name structures the record may not be transferred */
	    d26_t06_trans_value(ocl_tab[k],new_at_idx,
				d26_ob_record[D26_OLD] + old_pos + INTSIZE,
				(char *) to_pos, &f_off, &t_off,old_pos);
	    if (t_off == 0)
	    {
	      sprintf(log_comment,
"Record may not be transferred because there is no more valid entry\nin the ACL-right %d", f_off);
	      DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
	      return(D2_ERROR);
	    }
	  }
	}
      }
      if (ocl_tab[k] == D26_IDX_NOT_DEF) break;
    }

    /* check the length of naming attributes                   */
    for(i = ob_nr, srt = d26_srt[D26_OLD] + i; i != D26_ROOT;
	i = srt->d26s_par_nr, srt = d26_srt[D26_OLD] + i)
    {
      for (j = 0; srt->d26s_nam_at[j].d26s_index > D26_ROOT; j++)
      {
	old_pos = srt->d26s_nam_at[j].d26s_position[filenr];
	new_pos =
     d26_srt[D26_NEW][srt_table[i]].d26s_nam_at[j].d26s_position[*new_filenr];
	new_at_idx =
		    d26_srt[D26_NEW][srt_table[i]].d26s_nam_at[j].d26s_index;
	rep_len =
      d26_at[D26_OLD][srt->d26s_nam_at[j].d26s_index].d26a_rep == D2_T61_PR ?
							     INTSIZE : 0;
	phon_len =
       d26_at[D26_OLD][srt->d26s_nam_at[j].d26s_index].d26a_phon == TRUE ?
							     D26_LPHON : 0;
	if((int)strlen(d26_ob_record[D26_OLD] + old_pos + INTSIZE
	   + phon_len + rep_len) > d26_at[D26_NEW][new_at_idx].d26a_ubound)
	{
	    /* naming attribute would be cut off                */
	    /* record will not be transferred */
	    strcpy(log_comment,"naming attribute too long: ");
	    d27_301_oid_str(&d26_at[D26_NEW][new_at_idx].d26a_type,
			    log_comment + strlen(log_comment));
	    DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
	    return(D2_ERROR);
	}
      }
    }
  /* Setting unique C-ISAM identifier */
    if (isuniqueid(isfd,&rec_nr) < 0)
    {
  /* error from getting unique ID */
       DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
	  svc_c_debug1,GDS_S_ISUNIQUEID_ERR, iserrno));
       return(D26_ERROR);
    }
  /* Store record number and new object number in new record */
    stlong(rec_nr,d26_ob_record[D26_NEW]);
    stint(new_obj_no,d26_ob_record[D26_NEW] + LONGSIZE);

      /* store the naming attributes                          */
    for(i = ob_nr, srt = d26_srt[D26_OLD] + i; i != D26_ROOT;
	i = srt->d26s_par_nr, srt = d26_srt[D26_OLD] + i)
    {

      for (old_nam_at = srt->d26s_nam_at,
	   new_nam_at = d26_srt[D26_NEW][srt_table[i]].d26s_nam_at;
	   old_nam_at->d26s_index != -1;
	   old_nam_at++, new_nam_at++)
      {
	old_pos = old_nam_at->d26s_position[filenr];
	new_pos = new_nam_at->d26s_position[*new_filenr];
	new_at_idx = new_nam_at->d26s_index;
	/* Transfer the values of the attribute */
	d26_t05_trans_attr(filenr,ocl_tab,rec_nr,new_at_idx,old_pos,
			 new_pos);
      }
    }
    for (k = 0; k < oc_count; k++)
    {

      /* Store all other attributes into the new record   */
      idx = ocl_tab[k] != D26_IDX_NOT_DEF ? ocl_tab[k] : -1;
      if (idx != -1)
      {
      /* Store the mandatory attributes, whose AT-indices are */
      /* contained in the OCT */
	for(i = 0;
	    ((new_at_idx=d26_oct[D26_NEW][idx].d26c_mandatory[i]) != -1);
	    i++)
	{
	  for (m = 0;
	      (d26_srt[D26_NEW][new_obj_no].d26s_nam_at[m].d26s_index > 0) &&
      (d26_srt[D26_NEW][new_obj_no].d26s_nam_at[m].d26s_index != new_at_idx);
	       m++)
	  {}
	  if (d26_srt[D26_NEW][new_obj_no].d26s_nam_at[m].d26s_index == -1)
	  /* attribute is not a naming attribute */
	  {
	    old_pos = d26_at[D26_OLD][at_table[new_at_idx]].
				       d26a_field_pos[filenr];
	    new_pos = d26_at[D26_NEW][new_at_idx].
				       d26a_field_pos[*new_filenr];
	/* Transfer the values of the attribute */
	    d26_t05_trans_attr(filenr,ocl_tab,rec_nr,new_at_idx,old_pos,
			       new_pos);
	  }
	}
/* Store the optional attributes, whose AT-indices are contained in the OCT */
	  for(i = 0;
	      ((new_at_idx=d26_oct[D26_NEW][idx].d26c_optional[i]) != -1);
	      i++)
	  {
	  /* Check if the attribute exists in the old scheme and is not the*/
	  /* naming one                                                    */
	    for (m = 0;
	      (d26_srt[D26_NEW][new_obj_no].d26s_nam_at[m].d26s_index > 0) &&
      (d26_srt[D26_NEW][new_obj_no].d26s_nam_at[m].d26s_index != new_at_idx);
		 m++)
	    {}
	    if (at_table[new_at_idx] != -1 &&
		d26_srt[D26_NEW][new_obj_no].d26s_nam_at[m].d26s_index == -1)
	    {
	      /* this attribute exists in the old scheme and in the record */
	      old_pos = d26_at[D26_OLD][at_table[new_at_idx]].
						  d26a_field_pos[filenr];
	      new_pos = d26_at[D26_NEW][new_at_idx].
					 d26a_field_pos[*new_filenr];
	      /* Transfer the values of the attribute */
	      d26_t05_trans_attr(filenr,ocl_tab,rec_nr,new_at_idx,
			       old_pos,new_pos);
	    }
	  }
	}
      }
    }
  }

return(ocl_tab[0]);

}                       /*  d26_t16_convert_record                    */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_t13_open_file(...)                              */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 02.02.88                                            */
/*                                                                    */
/* DESCRIPTION  : This function will return the appropriate C-ISAM    */
/*                file descriptor.                                    */
/*                                                                    */
/*                First the function checks whether the file is       */
/*                already open.                                       */
/*                                                                    */
/*                When the file is open then the time of last         */
/*                use is set using the value of the global variable   */
/*                d26_ftime.                                          */
/*                                                                    */
/*                When the file isn't open the file which was Last    */
/*                Recently Used, is closed (when the times of more    */
/*                files are equal the file with the lowest file number*/
/*                (from AT  == index in file table) is closed) and the*/
/*                wished file is opened and time is set.              */
/*                (First the attribute files are examined. When a     */
/*                attribute file was found the object files aren't    */
/*                examined and the attribute file is closed).         */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                Short   sch       indicates old or new schema       */
/*                Short   file_nr;  internal file number of object    */
/*                                  entry or recurring attribute      */
/*                Short   oa_flag;  flag indicating whether object- or*/
/*                                  attr-file number should be used   */
/*                         D26_OBJ  object filenumber                 */
/*                         D26_ATT  attribute filenumber              */
/*                                                                    */
/* OUTPUT PARAMETERS: -                                               */
/*                                                                    */
/* RETURN VALUE :  >= 0:        C-ISAM file descriptor                */
/*                 D26_ERROR:   invalid file descriptor               */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                  D26_fildes  *d26t_ofildes;Description for object  */
/*                                           files                    */
/*                  D26_fildes  *d26t_afildes;Description for attribute*/
/*                                           files                    */
/*                                                                    */
/*                  Int         d26_nofile   number of objects files */
/*                                                                    */
/*                  Int         d26_nafile   number of attribute files*/
/*                                                                    */
/*                  Long        d26_ftime;   time set by last         */
/*                                           d26_u09_init_dsa         */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                  D26_fildes  *d26t_ofildes;Description for object  */
/*                                           files                    */
/*                  D26_fildes  *d26t_afildes;Description for attr.   */
/*                                           files                    */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                          sprintf()                                 */
/*                          isclose                                   */
/*                          isopen                                    */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*exoff ***************************************************************/

static  int    d26_t13_open_file(
  signed16   sch,
  signed16   oa_flag,
  signed16   file_nr)

{                                   /*  d26_t13_open_file()           */

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

				/* C-ISAM File-Name               */
 char            filename[D27_LFILE_NAME];
 int             fd;            /* file descriptor                */
 D26_fildes     *p_fd;          /* Pointer to file description of */
				/* of opened file                 */
 signed32        open_mode;     /* open mode for isopen           */

register signed32   i, j;           /* counter                            */
register D26_fildes *p_act,         /* ptr to actual entry in filetable   */
		    *p_close,       /* ptr to entry of file to close in   */
				    /* filetable                          */
		    *p_search;      /* ptr to begin of searched filetable */
register signed32   no_files;       /* number of files in act. filetable  */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

				    /*  get pointer to filedescription*/
				    /*  of file to open               */
p_fd = (oa_flag == D26_OBJ) ? d26t_ofildes[sch] : d26t_afildes[sch];
p_fd = p_fd + file_nr;

				    /*  fetch file descriptor         */
if                                  /*  file descriptor is invalid    */
   ((fd = p_fd->d26_isfd) < 0)
{                                   /*  search file to close               */

     /************************/
     /* search file to close */
     /************************/
     p_close = (D26_fildes *) NULL;

     for                            /*  filetables to examine and          */
				    /*  no file to close found             */
	 (j = 1;
	  ((j <= D26_I19_NO_FILETABLES)
	   && (p_close == (D26_fildes *) NULL));
	  j++)
     {                              /*  get filetable to examine           */
	switch ((int) j)            /* filetable to search                 */
	{
	case 1:                     /* first filetable to search           */
				    /* table of attribute files            */
		p_search = d26t_afildes[sch];
		no_files = d26_nafile[sch];
		break;
	case 2:                     /* second filetable to search          */
				    /* table of object files               */
		p_search = d26t_ofildes[sch];
		no_files = d26_nofile[sch];
		break;
	default:                    /* never occurs                        */
		break;
	}                           /* j */

				    /* */
				    /*  find file to close:                */
	for                         /*  all files in filetable             */
	    (i = 0; i < no_files; i++)
	{                           /*                                     */
	   p_act = p_search + i;    /*  get actual filetable entry         */
	   if                       /*  file not in use and file is open   */
	      ((p_act->d26_in_use == FALSE) && (p_act->d26_isfd >= 0))
	   {                        /*                                     */
		if                  /*  a file to close not yet found      */
		   (p_close == (D26_fildes *) NULL)
		{                   /*  set p_close = p_act                */
		     p_close = p_act;
		} else {            /*  compare p_close and p_act and set  */
				    /*  p_close                            */
		     if (p_act->d26_last_use < p_close->d26_last_use)
		     {              /* p_close used after p_act            */
			  p_close = p_act;
		     }              /* p_act->d26_last_use < ... */
		}                   /*  p_close == 0 */
	   }                        /*  file not in use ... */
	}                           /*  i = 0; ... */
     }                              /*  j = 1; ... */

     /***************************************/
     /* close unused file and open new file */
     /***************************************/
     if                             /*  file to close found                */
	(p_close != (D26_fildes *) NULL)
     {                              /*  close unused file                  */
	  if (D26_ISCLOSE (svc_c_sev_fatal,p_close->d26_isfd) < 0)
	  {    /* isclose fails          */
	       /* error trace and return */
	       if ((p_close >= d26t_afildes[sch])
		   && (p_close <= (d26t_afildes[sch] + d26_nafile[sch])))
	       {    /* attribute file */
		    sprintf (filename, "%s%s%d", d26_install[sch],D26_ATTR,
			     (p_close - d26t_afildes[sch]));
	       }
else
{ /* object file */
		    sprintf (filename, "%s%s%d",d26_install[sch],
			  D26_OBJECTS,(p_close - d26t_ofildes[sch]));
	       }

	       return (D26_ERROR);
	  }                         /* isclose fails */

				    /*  set filedescriptor of closed file  */
	  p_close->d26_isfd = D26_I18_FILE_CLOSED; /* */

				    /*  build filename of file to open     */
	  if (oa_flag == D26_OBJ)
	  {    /* object file */
	       sprintf(filename, "%s%s%d",
		       d26_install[sch], D26_OBJECTS, file_nr);
	  }
else
{ /* attribute file */
	       sprintf(filename,"%s%s%d",d26_install[sch],D26_ATTR,
		       file_nr);
	  }

	  if (sch == D26_OLD)                 /* get mode for open file   */
	    open_mode = ISINPUT + ISMANULOCK; /* read only                */
	  else
	    open_mode = ISINOUT + ISEXCLLOCK; /* read/write/excl. lock    */

				    /*  open new file                     */
	  if ((fd = D26_ISOPEN (svc_c_sev_fatal, filename, open_mode)) < 0)
	  {    /* isopen fails           */
	       /* error trace and return */
	       return (D26_ERROR);
	  }

	  p_fd->d26_isfd = fd;      /*  set filedescriptor in filetable    */
     } else {                       /*  error: no file to close found      */
				    /*  MA-LOG and MA_ERROR                */
	  dce_svc_printf(GDS_S_POOL_OPEN_OVFLW_MSG);
	  return(D26_ERROR);        /* */
     }                              /*  file to close found */
}                                   /*  fd < 0  */

/**************************************************/
/* set time of last use and return filedescritpor */
/**************************************************/

p_fd->d26_in_use++;                 /*  set file is used              */
p_fd->d26_last_use = time(0L);      /*  set time of last use          */
return(fd);                         /*  return(C-ISAM file descriptor)*/
}                                   /*  d26_t13_open_file()           */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t14_open_files()                                  */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        19.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function opens all required isam files.          */
/*                                                                    */
/* INPUT-PARAMETERS: None                                             */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Short   D2_NO_ERR        no error occurred                     */
/*             D26_ERROR        C-isam error occurred                 */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*              iserrno               ISAM error number               */
/*              d26_nofile            number of object files          */
/*              d26_nafile            number of attribute files       */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*              d26t_ofildes           object file descriptor table    */
/*              d26t_afildes           attribute file descriptor table */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        isopen()                                                    */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_t14_open_files(void)

{                               /*  d26_t14_open_files                */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* STATIC    ...    */

/* AUTO      ...    */
 static char *function = "d26_t14_open_files";
				    /* number of open isam files      */
 signed16 openfiles;
				    /* Isam filename                  */
 char  filename[D27_LFILE_NAME];
 signed16 ret_value = D2_NO_ERR;
 int      open_mode;                /* open mode for isopen           */

/* FASTAUTO  ...    */

register signed16    sch;  /* loop variable for schema                   */
register signed16    i;    /* loop variable                              */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

for                     /*  old and new schema                        */
   (sch = 0; sch < D26_I19_NO_FILETABLES; sch++)
{
  if (sch == D26_OLD)
    open_mode = ISINPUT + ISMANULOCK;    /* read only                 */
  else
    open_mode = ISINOUT + ISEXCLLOCK;    /* read/write/excl. lock     */

			/*  open object files                         */
  for                   /*  all object filenumbers                    */
     (i = 1, openfiles = 0;
      i < d26_nofile[sch] && openfiles < D26_MOPENFILES; i++)
  {                     /*  write file name                           */
    sprintf(filename,"%s%s%d",d26_install[sch],D26_OBJECTS,i);
    if                  /*  error from open isam                      */
      ((d26t_ofildes[sch][i].d26_isfd = D26_ISOPEN(svc_c_sev_fatal,filename,
       open_mode)) < 0)
    {                   /*  write error log                           */
      if (!(at_idx_changed && iserrno == ENOENT))
      {
	ret_value =  D26_ERROR;
      }
    } else {            /*  increment count of open isam files        */
      openfiles++;
    }                   /*                                            */
  }                     /*                                            */

  for                   /*  all at entries                            */
     (i = 3; i < d26_nafile[sch] && openfiles < D26_MOPENFILES; i++)
  {                     /*  write file name                           */
    sprintf(filename,"%s%s%d",d26_install[sch],D26_ATTR,i);
    if                  /*  error from open isam                      */
      ((d26t_afildes[sch][i].d26_isfd = D26_ISOPEN(svc_c_sev_fatal,filename,
       open_mode)) < 0)
    {                   /*  write error log                           */
      if (!(at_idx_changed && iserrno == ENOENT))
      {
	ret_value =  D26_ERROR;
      }
    } else {            /*  enhigher count of open isam files         */
      openfiles++;
    }                   /*                                            */
  }                     /*                                            */
}                       /*                                            */

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, ret_value));

return(ret_value);      /*                                            */

}                       /*  d26_t14_open_files                        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t15_close_files()                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        21.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function closes all open     isam files.         */
/*                                                                    */
/* INPUT-PARAMETERS: None                                             */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Short   D2_NO_ERR        no error occurred                     */
/*             D26_ERROR        C-isam error occurred                 */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*              iserrno               ISAM error number               */
/*              d26_nofile           number of object files          */
/*              d26_nafile           number of attribute files       */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*              d26t_ofildes          object file descriptor table    */
/*              d26t_afildes          attribute file descriptor table */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        isclose()                                                   */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_t15_close_files(void)

{                               /*  d26_t15_close_files               */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

signed16 ret_value = D2_NO_ERR;

register signed16    sch;  /* loop variable for schemas                  */
register signed16    i;    /* loop variable                              */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

for                     /*  old and new schema                        */
   (sch = 0; sch < D26_I19_NO_FILETABLES; sch++)
{                       /*  search for open files                     */
  for                   /*  all object filenumbers                    */
     (i = 0; i < d26_nofile[sch]; i++)
  {                     /*  check file descriptor                     */
    if                  /*  file is open                              */
      (d26t_ofildes[sch][i].d26_isfd >= 0)
    {                   /*  close file                                */
      if                /*  error from close file                     */
	(D26_ISCLOSE(svc_c_sev_fatal, d26t_ofildes[sch][i].d26_isfd) < 0)
      {                 /*  set return value                          */
	ret_value =  D26_ERROR;
      } else {          /*  set isam file descriptor -1               */
	d26t_ofildes[sch][i].d26_isfd = -1;
      }                 /*                                            */
    }                   /*                                            */
  }                     /*                                            */

  for                   /*  all attribute filenumbers                 */
     (i = 0; i < d26_nafile[sch]; i++)
  {                     /*  check file descriptor                     */
    if                  /*  file is open                              */
      (d26t_afildes[sch][i].d26_isfd >= 0)
    {                   /*  close file                                */
      if                /*  error from close file                     */
	(D26_ISCLOSE(svc_c_sev_fatal, d26t_afildes[sch][i].d26_isfd) < 0)
      {                 /*  set return value                          */
	ret_value =  D26_ERROR;
      } else {          /*  set isam file descriptor -1               */
	d26t_afildes[sch][i].d26_isfd = -1;
      }                 /*                                            */
    }                   /*                                            */
  }                     /*                                            */
}                       /*                                            */

return(ret_value);      /*                                            */

}                       /*  d26_t15_close_files                       */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t17_comp_srt()                                    */
/*                                                                    */
/* AUTHOR:      Loose, SNI BU BA KP 13                                */
/* DATE:        14.10.93                                              */
/*                                                                    */
/* DESCRIPTION: This function compares the old and new SRT            */
/*                                                                    */
/* INPUT-PARAMETERS: None                                             */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Bool    D2_TRUE          SRTs equal                            */
/*     Bool    D2_FALSE         SRTs not equal                        */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*              d26_srt                                               */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

static boolean d26_t17_comp_srt(void)

{                               /*  d26_t17_comp_srt */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

signed32          i;
D26_srt      *srt_old;
D26_srt      *srt_new;
D26_nam_at   *nat_old;
D26_nam_at   *nat_new;
boolean          ret_val = TRUE;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

if (d26_nosrt[D26_OLD] != d26_nosrt[D26_NEW])
  ret_val = FALSE;

for (i = 0, srt_old = d26_srt[D26_OLD], srt_new = d26_srt[D26_NEW];
     i < d26_nosrt[D26_OLD] && ret_val;
     i++, srt_old++, srt_new++)
{
  if (srt_old->d26s_par_nr == srt_new->d26s_par_nr)
  {
    for (nat_old = srt_old->d26s_nam_at,nat_new = srt_new->d26s_nam_at;
	 nat_old->d26s_index > -1 && nat_new->d26s_index > -1 && ret_val;
	 nat_old++, nat_new++)
    {
      if (nat_old->d26s_index != nat_new->d26s_index)
	ret_val = FALSE;
      /* indizes already checked while AT checking ! */
    }
    if (ret_val && nat_old->d26s_index != nat_new->d26s_index)
      ret_val = FALSE;
  } else {
    ret_val = FALSE;
  }
}

return(ret_val);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t18_comp_oct()                                    */
/*                                                                    */
/* AUTHOR:      Loose, SNI BU BA KP 13                                */
/* DATE:        14.10.93                                              */
/*                                                                    */
/* DESCRIPTION: This function compares the old and new SRT            */
/*                                                                    */
/* INPUT-PARAMETERS: None                                             */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Bool    D2_TRUE          SRTs equal                            */
/*     Bool    D2_FALSE         SRTs not equal                        */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*              d26_oct                                               */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

static boolean d26_t18_comp_oct(void)

{                               /*  d26_t18_comp_oct */

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

signed16     i,j;
D26_oct     *oct_old, *oct_new;
boolean      ret_val = TRUE;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

if (d26_nooct[D26_OLD] != d26_nooct[D26_NEW])
  ret_val = FALSE;

for (i = 0, oct_old = d26_oct[D26_OLD], oct_new = d26_oct[D26_NEW];
     i < d26_nooct[D26_OLD] && ret_val;
     i++, oct_old++, oct_new++)
{
  if (!D27_ATTCMP(&(oct_old->d26c_ob_id),&(oct_new->d26c_ob_id)) &&
      oct_old->d26c_filenr == oct_new->d26c_filenr)
  {
    for (j = 0;
	 oct_old->d26c_sup_cl[j] > -1 && oct_new->d26c_sup_cl[j] > -1 &&
	 ret_val; j++)
    {
      if (oct_old->d26c_sup_cl[j] != oct_new->d26c_sup_cl[j])
	ret_val = FALSE;
    }
    if (ret_val && oct_old->d26c_sup_cl[j] != oct_new->d26c_sup_cl[j])
      ret_val = FALSE;

    for (j = 0;
	 oct_old->d26c_aux_cl[j] > -1 && oct_new->d26c_aux_cl[j] > -1 &&
	 ret_val; j++)
    {
      if (oct_old->d26c_aux_cl[j] != oct_new->d26c_aux_cl[j])
	ret_val = FALSE;
    }
    if (ret_val && oct_old->d26c_aux_cl[j] != oct_new->d26c_aux_cl[j])
      ret_val = FALSE;

    for (j = 0;
	 oct_old->d26c_mandatory[j] > -1 && oct_new->d26c_mandatory[j] > -1 &&
	 ret_val; j++)
    {
      if (oct_old->d26c_mandatory[j] != oct_new->d26c_mandatory[j])
	ret_val = FALSE;
    }
    if (ret_val && oct_old->d26c_mandatory[j] != oct_new->d26c_mandatory[j])
      ret_val = FALSE;

    for (j = 0;
	 oct_old->d26c_optional[j] > -1 && oct_new->d26c_optional[j] > -1 &&
	 ret_val; j++)
    {
      if (oct_old->d26c_optional[j] != oct_new->d26c_optional[j])
	ret_val = FALSE;
    }
    if (ret_val && oct_old->d26c_optional[j] != oct_new->d26c_optional[j])
      ret_val = FALSE;
  } else {
    ret_val = FALSE;
  }
}

return (ret_val);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t19_change_indizes()                              */
/*                                                                    */
/* AUTHOR:      Loose, SNI BU BA KP 13                                */
/* DATE:        14.10.93                                              */
/*                                                                    */
/* DESCRIPTION: The function looks in the at_idxc_table for new       */
/*              indizes and calls the add_index-function.             */
/*                                                                    */
/* INPUT-PARAMETERS: None                                             */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     D2_ERROR                                                       */
/*     D2_NOERROR                                                     */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*              at_idxc_table                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_t19_change_indizes(void)

{                               /*  d26_t19_change_indizes */

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

signed16        i,j;            /* loop variables                     */
int             isfd;
D26_at         *at_old;
char          log_comment[200]; /* comment for logging              */
boolean       add_index;        /* if TRUE add index,               */
				/* if FALSE delete index            */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/* add C-ISAM-index only if index changed from 0 to <0 */
if (at_idx_changed)
{
  for (i = 0; i < d26_noat[D26_OLD]; i++)
  {
    if (at_idxc_table[i] == TRUE)
    {
      at_old = d26_at[D26_OLD]+i;
      add_index = d26_at[D26_OLD][i].d26a_ind_level <
		  d26_at[D26_NEW][i].d26a_ind_level;
      if (at_old->d26a_filenr >= 0)
      {
	if ((isfd = d26_t13_open_file(D26_NEW,D26_ATT,at_old->d26a_filenr))
	    == D26_ERROR)
	  return(D2_ERROR);
	sprintf(log_comment,"%s index in attr%d.",
			    add_index ? "Add" : "Delete",
			    at_old->d26a_filenr);
	DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
	if (d26_t42_add_index(i,at_old->d26a_filenr,isfd,add_index)
	    == D2_ERROR)
	  return(D2_ERROR);
	d26t_afildes[D26_NEW][at_old->d26a_filenr].d26_in_use--;
      } else {
	/* add index in every objectfile */
	for (j = 1; j < d26_nofile[D26_OLD]; j++)
	{
	  if (at_old->d26a_field_pos[j] >= 0)
	  {
	    if ((isfd = d26_t13_open_file(D26_NEW,D26_OBJ,j))
		 == D26_ERROR)
	      return(D2_ERROR);
	    sprintf(log_comment,"%s index in objects%d for at_idx %d.",
				add_index ? "Add" : "Delete",
				j,i);
	    DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
	    if (d26_t42_add_index(i,j,isfd,add_index) == D2_ERROR)
	      return(D2_ERROR);
	    d26t_ofildes[D26_NEW][j].d26_in_use--;
	  } /* if */
	} /* for */
      } /* if */
    } /* if */
  } /* for */
}

return(D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t20_copy_db()                                     */
/*                                                                    */
/* AUTHOR:      Loose, SNI BU BA KP 13                                */
/* DATE:        14.10.93                                              */
/*                                                                    */
/* DESCRIPTION: The function copies the whole DB to the NEW direct.   */
/*                                                                    */
/* INPUT-PARAMETERS: None                                             */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     D2_ERROR                                                       */
/*     D2_NOERROR                                                     */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*              d26_install                                           */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_t20_copy_db(void)

{                               /*  d26_t20_copy_db */

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

char  cp_command[20 + 3 * D27_LFILE_NAME];
char  rm_command[20 + 3 * D27_LFILE_NAME];
int   err;                          /* error from system call         */
boolean  *ofiles_to_copy;           /* array of ofile numbers         */
signed32 i,j;
signed16 file_nr;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

  /* remove all files except schema object */
  sprintf(rm_command,
   "ls %s/attr[!012].* %s/objects[!0].* | xargs -i rm {}",
    d26_install[D26_NEW],
    d26_install[D26_NEW]);

  if ((err = D27_SYSTEM(svc_c_sev_fatal, rm_command)) == -1)
  {
    return(D2_ERROR);
  }

  sprintf(rm_command,
   "ls %s/attr??*.* | xargs -i rm -f {}",
    d26_install[D26_NEW]);

  if ((err = D27_SYSTEM(svc_c_sev_fatal, rm_command)) == -1)
  {
    return(D2_ERROR);
  }

  sprintf(rm_command,
   "rm -f  %s/objects??*.*",
    d26_install[D26_NEW]);

  if ((err = D27_SYSTEM(svc_c_sev_fatal, rm_command)) == -1)
  {
    return(D2_ERROR);
  }

  sprintf(rm_command,"rm -f %s/dnlist.*",d26_install[D26_NEW]);
  if ((err = D27_SYSTEM(svc_c_sev_fatal, rm_command)) == -1)
  {
    return(D2_ERROR);
  }

  if ((ofiles_to_copy = D27_CALLOC(svc_c_sev_fatal,sizeof(boolean),
	d26_nofile[D26_NEW])) == NULL) {
    return(D2_ERROR);
  }
  for (i = 0; i < d26_nofile[D26_NEW]; i++)
    ofiles_to_copy[i] = FALSE;

  for (i = 0; i < d26_noat[D26_NEW]; i++)
  {
    if (at_idxc_table[i] == TRUE)
    {
      if ((file_nr = d26_at[D26_NEW][i].d26a_filenr) > 2)
      {
	sprintf(cp_command,"cp %s%s%d.* %s",
			   d26_install[D26_OLD],
			   D26_ATTR, file_nr,
			   d26_install[D26_NEW]);

	if ((err = D27_SYSTEM(svc_c_sev_fatal, cp_command)) == -1)
	{
	  return(D2_ERROR);
	}
      }
      else
      {
	/* check in which objectfiles the attribute occures */
	for (j = 1; j < d26_nofile[D26_NEW]; j++)
	{
	  if (d26_at[D26_NEW][i].d26a_field_pos[j] > 0)
	    ofiles_to_copy[j] = TRUE;
	}
      }
    }
  }
  for (i = 1; i < d26_nofile[D26_NEW]; i++)
  {
    if (ofiles_to_copy[i] == TRUE)
    {
      sprintf(cp_command,"cp %s%s%d.* %s",
		       d26_install[D26_OLD],D26_OBJECTS,i,d26_install[D26_NEW]);

      if ((err = D27_SYSTEM(svc_c_sev_fatal, cp_command)) == -1)
      {
	  return(D2_ERROR);
      }
    }
  }

return(D2_NOERROR);
}
