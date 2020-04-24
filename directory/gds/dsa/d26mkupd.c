/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26mkupd.c,v $
 * Revision 1.1.10.2  1996/02/18  19:45:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:56  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:46:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:22  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:45:38  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:32:48  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:52:26  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/04  14:25:18  marrek]
 * 
 * 	Bug fixes in April 1994 submission.
 * 	[1994/05/03  16:41:17  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:22:42  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:09:29  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:21:24  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:06:12  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:15:19  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  14:21:55  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:13:09  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  19:41:16  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:20:13  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:50:33  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:56:49  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26mkupd.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:45:38 $";
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
/* NAME         : mkupdate                                            */
/*                                                                    */
/* AUTHOR       : Ufholz, D AP 11                                     */
/* DATE         : 28.04.88                                            */
/*                                                                    */
/* SYNTAX       mkupdate                                              */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  : The Function modifies the masterknowledge           */
/*                if db is read from disk                             */
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
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                    I N C L U D E S                                 */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>
#include <dce/dcesvcmsg.h>

#include <stdlib.h>
#include <errno.h>
#include <locale.h>

#include <dce/d27util.h>
#include <d26schema.h>
#include <d26isam.h>
#include <d26svcis.h>

/**********************************************************************/
/*                                                                    */
/*                    D E F I N T I O N S                             */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* AUTHOR       : Ufholz, D AP 11                                     */
/* DATE         : 28.04.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function modifies the masterknowledge           */
/*                if db is read from disk                             */
/*                                                                    */
/* RETURNVALUE     : -                                                */
/*                                                                    */
/*exoff ***************************************************************/

int main(
    int argc,           /* IN    -  count of command line parameters  */
    char *argv[])       /* IN    -  command line parameters           */

{   

			    /* type of master knowledge attribute     */
  static D2_a_type mk_type =  {sizeof(D2_MASTER_KNOWLEDGE) - 1,
			       (Ob_id_string)D2_MASTER_KNOWLEDGE};

  int      file_no;             /* C-ISAM-file-number                 */
  char     filename[D27_LFILE_NAME];    /* name of C-ISAM-file        */

  char     *res_name;           /* restored dsaname                   */
  char     *own_name;           /* own dsaname                        */
  signed32 res_dnlidx;          /* index of restored dsaname in dnlist*/
  signed32 own_dnlidx;          /* index of own dsaname in dnlist     */
  signed16 res_count = 0;       /* # of occurrency of restored dsaname*/
  signed16 own_count = 0;       /* # of occurrency of own dsaname     */

  int      isfdread;            /* ISAM-file descriptor of obj. files */


  signed16 mk_idx;              /* index of MK-attribute in AT        */
  signed16 mk_pos;              /* position of MK-attribute in record */
  signed32 mk;                  /* master-knowledge attribute in rec. */
  boolean  change;              /* indicates change of master-knowl.  */
				/* object identifiers for soundex     */
				/* algorithm                          */
  D2_obj_id soundex_obid;
				/* error status for serviceability    */
  error_status_t st;            /* functions                          */

  setlocale(LC_ALL, "");

  if                    /* no parameter given                         */
    (argc < 3)
  {                     /* print error and exit                       */
    fprintf(stderr,"usage: d26mkupd <inst-dir> <dir-id>\n");
    exit(D27_START_ERROR);
  }
  else if               /* installation directory too long            */
	 ((int)strlen(argv[1]) > D27_LINST_NAME)
  {                     /* give error note and exit                   */
    fprintf(stderr,"d26mkupd: Name of install directory too long\n");
    exit(D27_START_ERROR);
  }
  else
  {                     /* write filenames                            */
    sprintf(d26_chdb_name,"%s%s%s",dcelocal_path,D27_BIN_DIR,d26_chdb_prog);
    sprintf(d26_install,"%s%s%s%s%d",dcelocal_path,D27_SERVER_DIR,
      D27_DSA_DIR,D27_SUBDSA_DIR, atoi(argv[2]));
    sprintf(d26_logdir,"%s%s%s%s%s%d",dcelocal_path,D27_SERVER_DIR,
      D27_DCE_ADM,D27_DSA_DIR, D27_SUBDSA_DIR, atoi(argv[2]));
    if                  /* working directory cannot be changed        */
      (chdir(d26_logdir) < 0)
    {                   /* give error note and exit                   */
      fprintf(stderr,"Unable to chdir to %s, errno: %d\n",d26_logdir,errno);
      exit(D27_START_ERROR);
    }
    d27_001_svc_init(argc,argv,&st);
    if                  /* working directory cannot be changed        */
      (st != svc_s_ok)
    {                   /* give error note and exit                   */
      fprintf(stderr,"Unable to initialize serviceability, status: 0x%lx\n",
	st);
    }
    dce_svc_printf(GDS_S_ENTRY_MKUPD_MSG);

    d26_ulogging = FALSE;

    if                  /* scheme file cannot be read                 */
      (d26_mi9_read_scheme(d26_install,&d26_nosrt,&d26_srt,
       &d26_nam_at,&d26_name_pos,&d26_ocl_arc,&d26_nooct,&d26_oct,&d26_oct_idx,
       &d26_noat,&d26_at,&d26_at_pos,&d26_obid_str,&d26_nofile,&d26_ofildes,
       &d26_oreclen,&d26_ob_record,&d26_nafile,&d26_afildes,&d26_areclen,
       &d26_a_record,d27_country_bitlist,&soundex_obid) < 0)
    {                   /* exit error                                 */
      exit(1);
    }

    if                  /* restored dsaname doesn't exist             */
      ((res_name = getenv("DSANAME")) == NULL)
    {                   /* log error and exit                         */
      dce_svc_printf(GDS_S_ENVVAR_ERR_MSG,"DSANAME");
      exit(1);
    }

    if                  /* get own dsaname                            */
      ((own_name = getenv("DSANAMEOLD")) == NULL)
    {                   /* log error and exit                         */
      dce_svc_printf(GDS_S_ENVVAR_ERR_MSG,"DSANAMEOLD");
      exit(1);
    }

    DCE_SVC_LOG((GDS_S_DSANAME_READ_MSG));

    if                  /* error during opening dnlist file           */
      (d26_i01_upd_core_dnlist() == D26_ERROR)
    {                   /* log error and exit                         */
      exit(1);
    }

			/* initialize index of restored DSA-name      */
    res_dnlidx = D26_IDX_NOT_DEF;

			/* add new entry in dnlist for restored DSA   */
    if                  /* error from updating DN-List                */
      (d26_i02_upd_file_dnlist(D2_ADD_AV,(D2_name_string)NULL,D2_SOBJ,
       (D2_name_string)res_name,1,TRUE,&res_dnlidx) == D26_ERROR)
    {                   /* log error and exit                         */
      exit(1);
    }
			/* get index of own DSA-name in dnlist        */
    own_dnlidx = d26_u12_get_dnlidx(D2_SOBJ,(D2_name_string)own_name);

			/* determine AT index of masterknowledge attr.*/
    if                  /* Master-knowledge-attribute is not in schema*/
      ((mk_idx = d26_u03_attr_search(&mk_type)) >= 0)
    {                   /* search all object files                    */
      for               /* all object files                           */
	 (file_no = 0; file_no < d26_nofile; file_no++)
      {                 /* open file                                  */
	sprintf(filename,"%s%s%d",d26_install,D26_OBJECTS,file_no);
	if              /* file cannot be opened                      */
	  ((isfdread = D26_ISOPEN(svc_c_sev_fatal,filename,
	    ISINOUT + ISMANULOCK)) < 0)
	{               /* exit error                                 */
	  exit(1);
	}
	else
	{               /* log success and start query                */
	  mk_pos = d26_at[mk_idx].d26a_field_pos[file_no];
	  while         /* records can be read                        */
	       (D26_ISREAD(svc_c_route_nolog,isfdread,d26_ob_record,ISNEXT)
		>= 0)
	  {             /* read master-knowledge attribute            */
	    if          /* record contains master-knowledge           */
	      (ldint(d26_ob_record + mk_pos) > 0)
	    {           /* read master-knowledge                      */
	      mk = ldlong(d26_ob_record + mk_pos + INTSIZE);
	      change = FALSE;
	      if        /* old dsa was master                         */
		(mk == D26_OWNNAME)
	      {         /* reset master knowledge attribute by        */
			/* DN-list-index of restored DSA-name         */
		stlong(res_dnlidx,d26_ob_record + mk_pos + INTSIZE);
		res_count++;
		change = TRUE;
	      }
	      else if   /* own DSA was master                         */
		     (mk == own_dnlidx)
	      {         /* write -1 for own dsa name into record      */
		stlong(D26_OWNNAME,d26_ob_record + mk_pos + INTSIZE);
		own_count++;
		change = TRUE;
	      }
	      if        /* master-knowledge was changed               */
		(change == TRUE)
	      {         /* rewrite record                             */
		if      /* error from writing                         */
		  (D26_ISREWRITE(svc_c_sev_fatal,isfdread,d26_ob_record) < 0)
		{       /* exit error                                 */
		  exit(1);
		}
	      }
	    }
	  }

	  switch(iserrno)           /* check isread error             */
	  {
	    case EENDFILE:
	      break;
	    default:
	      dce_svc_printf(GDS_S_ISREAD_ERR_MSG,ISNEXT,iserrno);
	      exit(1);
	  }

	  if            /* error from closing file                    */
	    (D26_ISCLOSE(svc_c_sev_fatal,isfdread) < 0)
	  {             /* exit error                                 */
	    exit(1);
	  }

	  DCE_SVC_LOG((GDS_S_FILE_PROC_MSG,file_no));
	}
      }
    }

			/* log counts of changed references to DN-list*/
    DCE_SVC_LOG((GDS_S_REF_CHANGED_MSG,own_count,res_count));

    if                  /* own DSA-name was in DN-List before         */
      (own_dnlidx >= 0L)
    {                   /* delete DN-List-entry of own name           */
      if                /* error from updating DN-List                */
	(d26_i02_upd_file_dnlist(D2_DEL_AV,(D2_name_string)NULL,D2_SOBJ,
	 (D2_name_string)own_name,own_count, TRUE, &own_dnlidx) == D26_ERROR)
      {                 /* exit error                                 */
	exit(1);
      }
    }

    if                  /* more than one reference to the restored DSA*/
      (res_count > 1)
    {                   /* insert restored dsaname in DN-list         */
			/* according occurrency                       */
      if                /* error from updating DN-List                */
	(d26_i02_upd_file_dnlist(D2_ADD_AV,(D2_name_string)NULL,D2_SOBJ,
	 (D2_name_string)res_name,res_count - 1,TRUE,&res_dnlidx) == D26_ERROR)
      {                 /* exit error                                 */
	exit(1);
      }
    }
    else if             /* no reference to the restored dsaname       */
	   (res_count == 0)
    {                   /* delete restored DSA-name from DN-List      */
      if                /* error from updating DN-List                */
	(d26_i02_upd_file_dnlist(D2_DEL_AV,(D2_name_string)NULL,D2_SOBJ,
	 (D2_name_string)res_name,1,TRUE,&res_dnlidx) == D26_ERROR)
      {                 /* exit error                                 */
	exit(1);
      }
    }

    if                  /* error from closing dnlist file             */
      (d26_i06_close_dnlistfile() == D26_ERROR)
    {                   /* exit error                                 */
      exit(1);
    }
  }

  dce_svc_printf(GDS_S_EXIT_MKUPD_MSG);

  exit(0);

}               /*  main                                              */
