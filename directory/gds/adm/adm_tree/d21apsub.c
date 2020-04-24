/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21apsub.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:50  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:14:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:12  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:43:20  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  12:08:54  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:48  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:06:36  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:46  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:23:23  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:33:28  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:21:13  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:48:06  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:02:19  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:19:33  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:33:41  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:47:24  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:12:34  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21apsub.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:48 $";
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
/* NAME         : d21apsub.o                                          */
/*								      */
/* AUTHOR       : Serra & Pulpillo, D 525 (CDS)                       */
/* DATE         : 11.07.89                                            */
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
/*      0.1 | 11.07.89| Erstellung                     | hv |         */
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
#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <dce/d27util.h>
#include <d2delupd.h>
#include <d2dump.h>
#include <d21.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define		SIZE_INHOVR	30	/* size of loggingstring for inherit
					   and overwrite */

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

extern signed16   d20_bid;              /* global bind-ID             */
extern D2_pbhead *d20_head;

extern char      *d20_object;

/**********************************************************************/
/*								      */
/*		  D E C L A R A T I O N S      FUNCTIONS	      */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21_12_appnd()                                         */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will append a subtree or an object, which was   */
/*      previously saved by the DS_SAVE_SUBTREE function in a local   */
/*      file, to a (new) parent on the bind DSA.                      */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      signed16      bind_id;      bind identification               */
/*      signed16      version;      version of interface              */
/*      String        file_name;    file containing the saved objects */
/*      Name_string   parent_name;  name of the object under which    */
/*                                  the subtree or the object will    */
/*                                  be appended                       */
/*      signed32      inherit;      parameter specifying how the MK   */
/*                                  and the ACL attributes will be    */
/*                                  set for the appended entries      */
/*      Bool          overwrite;    a flag indicating whether existing*/
/*                                  objects will be overwritten or not*/
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Name_string **old_objects;  list of existing objects with     */
/*                                  the same name as objects to be    */
/*                                  appended                          */
/*      Errcode      *return_code;  return code of operation          */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR  = The function has been performed successfully.   */
/*      D2_ERROR    = function fails                                  */
/*                                                                    */
/*exoff ***************************************************************/

 d2_ret_val d21_12_appnd(
   signed16      bind_id,
   Errcode      *return_code,
   signed16      version,
   String        file_name,
   Name_string   parent_name,
   signed32      inherit,
   Bool          overwrite,
   Name_string **old_objects)

{
   D2_pbhead        head;
   D2_c_arg         comm_arg;
   char             file2[D20_FILE_MAX];    /* file name for search   */
   D20_delupd_info  del_info;   /* information to be used for "delta- */
				/* updates"                           */
   signed32         val[D20_NOVAL];
   d2_ret_val       ret_value = D2_NOERROR;

   D20_offset      *dn1;        /* offsets of DN relatives            */
				/* to beginning of files or memory    */
				/* areas                              */
   D20_offset   *pdn1;      
   signed32      dn_no1;        /* number of DNs found in both DSAs   */
   char         *name1;         /* pointer to memory area for DNs     */
   int           fd1;           /* file descriptor of file1 and file2 */
   signed32      l_name1;            /* length of dnlist              */
   signed32      attr1_len;          /* length of attributes          */
   char         *p_name;
   char          norm_par_nam[D2_DNL_MAX];
   char          new_subtree[D2_DNL_MAX];   /* new DN name            */
   String        new_sub;
   signed32      new_parent;
   signed16      save_bid;
   char          inh_ovr[D20_LOG_INHOVR];
   signed16      dir_id;

   /*
   d21d009_errlog("ENTRY d21_12_appnd", NULL);
   */

   /*
   d27_009_trace_dn("  New Parent Name:", (char *)parent_name);
   */
   sprintf(inh_ovr, "%d %d", inherit, overwrite);
   /*
   d27_009_trace_dn("  Inherit/Overwrite:", inh_ovr);
   */

   *old_objects = NULL;
			       /* check version, bind-id */
   if (d21t009_param_check(bind_id, return_code, version,&dir_id)
                           == D2_ERROR) {
      /*
      d21d009_errlog("EXIT  d21_12_appnd", return_code);
      */
      return(D2_ERROR);
   }

   d20_head =  &head;
   head.d2_version = DS_V02;
   head.d2_retcode = head.d2_errclass = head.d2_errvalue = D2_NO_ERR;

   if (d20_120_read_delupd_info(file_name, val, &del_info) == D2_ERROR) {
      return_code->retcode = d20_head->d2_retcode % 16 + '0';
      return_code->errclass = d20_head->d2_errclass % 16 + '0';
      return_code->errvalue = d20_head->d2_errvalue;
      /*
      d21d009_errlog("EXIT  d21_12_appnd", return_code);
      */
      return(D2_ERROR);
   }

   save_bid = d20_bid;
   d20_bid = bind_id;
   comm_arg.d2_serv_cntrls = D2_NOREFERRAL | D2_USEDSA | D2_DREFALIAS |
                             D2_LOCALSCOPE | D2_DONT_STORE | D2_PREF_ADM_FUN;
   comm_arg.d2_priority = D2_PRIO_LOW;
   comm_arg.d2_time_limit = D2_T_UNLIMITED;
   comm_arg.d2_size_limit = D2_S_UNLIMITED;
   
   d20_init();

				   /*  read dump file                 */
   if (d20_116_read_dump_file(file_name, &name1, &pdn1, &dn1, &dn_no1,
			      &attr1_len, &del_info)) {
                                   /*  operation fails                */
      return_code->retcode = PERMERR;
      return_code->errclass = SYSERR;
      return_code->errvalue = d20_head->d2_errvalue;
      d20_bid = save_bid;
      /*
      d21d009_errlog("EXIT  d21_12_appnd", return_code);
      */
      return(D2_ERROR);
   }
   else {                          /*  open SEARCH-file of DSA1       */
      if ((fd1 = open(file_name, O_RDONLY)) == -1) {
                                   /*  open fails                     */
	 if (errno == ENOENT || errno == EACCES) {
	    return_code->errvalue = D20_NOFILE;
	 }
	 else {
	    return_code->errvalue = D20_FILE_ERR;
	 }
         return_code->retcode = PERMERR;
         return_code->errclass = SYSERR;
         d20_bid = save_bid;
	 /*
	 d21d009_errlog("EXIT  d21_12_appnd", return_code);
	 */
         return(D2_ERROR);
      }
   }
   sprintf(file2, "%s%d", D20_SE2, getpid());
   p_name = del_info.d20_obj;

   if (parent_name == NULL)
   {    *(d20_object - 1) = D2_EOS;             /* ROOT */
	norm_par_nam[0] = D2_EOS;
	new_sub = p_name;
	new_parent = D21_OLD_PARENT;
   }
   else
   {    strcpy(norm_par_nam, (char *)parent_name);
	d20_norm(norm_par_nam, strlen(norm_par_nam));
	d20_162_set_name((String) parent_name);
	d20_set_subset(D2_BASE_OBJECT);
	d20_168_set_serv_cntrls();
						/* look for parent name */
	if (d20_call_iapl(D20_0_SEARCH)) {
	    d20_169_reset_serv_cntrls();
	    if (d20_head->d2_errvalue == D2_NFOUND) {
		return_code->retcode = CALLERR;
		return_code->errclass = PARERR;
		return_code->errvalue = DS_NFOUND;
	    }
	    else {
		return_code->retcode = PERMERR;
		return_code->errclass = SYSERR;
		return_code->errvalue = DS_SEARCH_ERR;
	    }
            d20_bid = save_bid;
	    /*
	    d21d009_errlog("EXIT  d21_12_appnd", return_code);
	    */
	    return(D2_ERROR);
	}
	d20_169_reset_serv_cntrls();

	if ((new_parent = d21t006_new_parent(norm_par_nam, p_name)) != 0) {
	    if (d21t010_change_dn((Name_string) p_name,
				  (Name_string) norm_par_nam, 
				  (Name_string) p_name,
				  new_subtree) == D2_ERROR) {
	       return_code->retcode = PERMERR;
	       return_code->errclass = SYSERR;
	       return_code->errvalue = DS_SEARCH_ERR;
               d20_bid = save_bid;
	       /*
	       d21d009_errlog("EXIT  d21_12_appnd", return_code);
	       */
	       return(D2_ERROR);
	    }
	    else
	    {   new_sub = new_subtree;
	    }
	}
	else {
	    new_sub = p_name;
	}
   }

   if (d20_165_call_search(bind_id, del_info.d20_sobj, &comm_arg,
			   (D2_name_string) new_sub, file2, TRUE, &fd1,
			   &name1, &l_name1, &pdn1, &dn1, &dn_no1,
			   &attr1_len) != D2_NOERROR) {
      return_code->errvalue = d20_head->d2_errvalue;
      ret_value = D2_ERROR;
   }

   if (!ret_value) {
      if (d21t000_restore_appnd(fd1, (D20_offset **) pdn1, (D20_offset **) dn1,
	     dn_no1, name1, attr1_len, file2, inherit, overwrite, new_parent,
	     old_objects, (Name_string) norm_par_nam, (Name_string) p_name,
	     bind_id)) {
	 return_code->errvalue = d20_head->d2_errvalue;
         ret_value = D2_ERROR;
      }      
   }
   unlink(file2);
   
   if (ret_value) {
      return_code->retcode = PERMERR;
      return_code->errclass = SYSERR;
   }
   d20_bid = save_bid;
   /*
   d21d009_errlog("EXIT  d21_12_appnd", return_code);
   */
   return(ret_value);
}
