/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20errfile.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:29  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:19:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:29  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:46  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:12:42  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:24  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:34:52  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:39:46  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:36:50  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:10:06  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:06:19  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  14:56:56  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:10:20  marrek]
 * 
 * Revision 1.1.4.2  1992/10/12  21:08:16  sommerfeld
 * 	Correct previous fix to match what SNI wanted here.
 * 	[1992/10/09  20:02:02  sommerfeld]
 * 
 * 	HPUX struct flock has fewer fields than SVR4 struct flock.
 * 	Since C guarantees that uninitialized fields will be zeroed, we
 * 	remove the extra zeros from the end of the initalizer list.
 * 	[1992/09/30  21:27:01  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/06/01  19:59:05  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:42:43  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20errfile.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:45 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d20errfile.c                                        */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 05.01.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d23apdu.h>
#include <cget.h>
#include <d2delupd.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define D20_LABEL_LEN       14
#define D20_CONTENTS_LEN    16
#define D20_LINE_LEN        80
#define D20_LINES_PER_ENTRY 11

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

typedef struct {
    char    tstamp [D20_LABEL_LEN + D20_CONTENTS_LEN];
    char    etype  [D20_LABEL_LEN + D20_CONTENTS_LEN];
    char    dirid  [D20_LABEL_LEN + 3];
    char    dname  [D20_LABEL_LEN + D2_DNL_MAX];
    char    oname  [D20_LABEL_LEN + D2_DNL_MAX];
    char    opcode [D20_LABEL_LEN + D20_CONTENTS_LEN];
    char    etime  [D20_LABEL_LEN + D20_CONTENTS_LEN];
    char    ecode  [D20_LABEL_LEN + D20_CONTENTS_LEN];
    char    ecount [D20_LABEL_LEN + D20_CONTENTS_LEN];
    char    action [D20_LABEL_LEN + 3];
    char    line   [D20_LINE_LEN  + 1];
} Shdupd_err_entry;

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern D20_omask  d20_qomask[];
extern D20_omask  d20_E3omask[];
extern char      *d20_err_table[];

/* LOCAL FUNCTIONS */

static d2_ret_val d20_get_err_entry ( Shdupd_err_entry * , FILE * );
static d2_ret_val d20_put_err_entry ( Shdupd_err_entry * , FILE * );


/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d20_C04_show_error_file()                              */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function locks the error message file from the           */
/*      incremental shadowing operations and displays/updates         */
/*      its contents.                                                 */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      path    = path of directory installation                      */
/*      dir_id  = directory identifier                                */
/*      op      = operation: DS__DISP_UPD_ERR  or  DS__UPD_ERR        */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                    None                                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR  = The function has been performed successfully.   */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d20_C04_show_error_file(
  char * path,
  signed16 dir_id,
  signed16 op)
{
char             lock_file_name[D27_LFILE_NAME];
char             err_file_name[D27_LFILE_NAME];
char             tmp_file_name[D27_LFILE_NAME];
char             mask_oname[D2_DNL_MAX];
char             mask_dname[D2_DNL_MAX];
FILE            *lockfp, *errfp, *tmpfp;
Shdupd_err_entry entry;
static   struct flock arg ={F_WRLCK,SEEK_SET,0L,0L}; /* variables for fcntl */
register signed32     flush = 0;
register signed32     next = 0;
register signed32     mod = 0;
register signed32     upd = ((op==DS__UPD_ERR)?1:0);

sprintf(err_file_name, "%s%s%s%d%s", path, D27_DSA_DIR, D27_SUBDSA_DIR,
				                          dir_id, D21_UPD_ERR);
sprintf(lock_file_name, "%s%s%s%d%s", path, D27_DSA_DIR, D27_SUBDSA_DIR,
				                            dir_id, D2_LOGDAE);
if (upd)
    sprintf(tmp_file_name, "%s%s%s%d%s.tmp", path, D27_DSA_DIR, D27_SUBDSA_DIR,
                                                          dir_id, D21_UPD_ERR);

umask(0);
				/* open daemon log file               */
if ((lockfp = (FILE *) fopen (lock_file_name, "a")) == (FILE *) NULL) {
    if (errno == ENOENT)
        d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E107IDX];
    else
        d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E106IDX];
    d20_handlemask(d20_qomask);
    return(D2_NOERROR);
}

                                /* lock daemon log file               */
if (fcntl (fileno (lockfp), F_SETLKW, &arg) == (-1)) {
    if (errno == EACCES || errno == EAGAIN)
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E23IDX];
    else
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E24IDX];
    d20_handlemask(d20_qomask);
    fclose(lockfp);
    return(D2_NOERROR);
}
	
				/* open error file                    */
if ((errfp = fopen(err_file_name, "r+")) == (FILE *) NULL) {
    if (errno == ENOENT)
        d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E22IDX];
    else
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E108IDX];
    d20_handlemask(d20_qomask);
    fclose(lockfp);
    return(D2_NOERROR);
}

if (upd) {
    if ((tmpfp = fopen(tmp_file_name, "w+")) == (FILE *) NULL) {
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E109IDX];
	d20_handlemask(d20_qomask);
	fclose(lockfp);
	fclose(errfp);
	return(D2_NOERROR);
    }
}

while (d20_get_err_entry (&entry, errfp) == D2_NOERROR) {
    if ((dir_id == atoi(&entry.dirid[D20_LABEL_LEN-1])) && (!flush)) {
        strcpy(mask_oname, &entry.oname[D20_LABEL_LEN-1]);
        d20_gen_dnintf(mask_oname);
        d20_C08_copy_name(mask_oname, 
                          d20_E3omask[DS_E3_OBJ_NAME].d20_ioinfo,
                          d20_E3omask[DS_E3_OBJ_NAME+1].d20_ioinfo);

        strcpy(mask_dname, &entry.dname[D20_LABEL_LEN-1]);
        d20_gen_dnintf(mask_dname);
        d20_C08_copy_name(mask_dname, 
                          d20_E3omask[DS_E3_DSA].d20_ioinfo,
                          d20_E3omask[DS_E3_DSA+1].d20_ioinfo);

        if (strcmp(&entry.opcode[D20_LABEL_LEN-1], D2_A_OBJ) == 0)  
            *(signed32 *)d20_E3omask[DS_E3_OP].d20_ioinfo = D23_ADDOBJT;
        else if (strcmp(&entry.opcode[D20_LABEL_LEN-1], D2_R_OBJ) == 0)
                 *(signed32 *)d20_E3omask[DS_E3_OP].d20_ioinfo = D23_REMOBJT;
             else if (strcmp(&entry.opcode[D20_LABEL_LEN-1], D2_M_OBJ) == 0)
                      *(signed32 *)d20_E3omask[DS_E3_OP].d20_ioinfo = D23_MODENTR;
                  else if (strcmp(&entry.opcode[D20_LABEL_LEN-1],D2_M_RDN) == 0)
                           *(signed32 *)d20_E3omask[DS_E3_OP].d20_ioinfo=D23_REPLRDN;
                       else {
	                   d20_qomask[D20_Q1POS].d20_ioinfo = 
                                                     d20_err_table[D20_E110IDX];
	                   d20_handlemask(d20_qomask);
	                   fclose(lockfp);
	                   fclose(errfp);
	                   return(D2_NOERROR);
                       }
        strcpy(d20_E3omask[DS_E3_ERR_TIME].d20_ioinfo, 
               &entry.etime[D20_LABEL_LEN-1]);
        strcpy(d20_E3omask[DS_E3_ERR_COUNT].d20_ioinfo, 
               &entry.ecount[D20_LABEL_LEN-1]);
        strcpy(d20_E3omask[DS_E3_ERR_CODE].d20_ioinfo, 
               &entry.ecode[D20_LABEL_LEN-1]);

        if (upd) {
            d20_E3omask[DS_E3_UPD_ERR-1].d20_fkt_id = (signed16) D20_CSAY;
            d20_E3omask[DS_E3_UPD_ERR].d20_fkt_id = (signed16) D20_CGET;
            *(signed32 *)d20_E3omask[DS_E3_UPD_ERR].d20_ioinfo = FALSE;
        }
        else {
            d20_E3omask[DS_E3_UPD_ERR-1].d20_fkt_id = (signed16) D20_NO_OP;
            d20_E3omask[DS_E3_UPD_ERR].d20_fkt_id = (signed16) D20_NO_OP;
        }

	switch ((int) d20_handlemask(d20_E3omask)) {
            case FKTCR:
            case FKTMENU:
            case FKTSCD:
                next++;
                break;

            case FKTDEL:
                if ((upd) && (mod)) {
                    flush++;
	            d20_qomask[D20_Q1POS].d20_ioinfo =
                                                     d20_err_table[D20_E111IDX];
	            d20_handlemask(d20_qomask);
                }
                else {
                    if (upd) {
	                fclose(tmpfp);
                        if (unlink (tmp_file_name) == -1) {
                            d20_qomask[D20_Q1POS].d20_ioinfo =
                                                     d20_err_table[D20_E113IDX];
	                    d20_handlemask(d20_qomask);
                        }
                    }
	            fclose(lockfp);
	            fclose(errfp);
	            return(D2_NOERROR);
                }
                break;
        }
    }

    if (upd) {  /* write entry to tmp file */
        if (*(signed32 *)d20_E3omask[DS_E3_UPD_ERR].d20_ioinfo != FALSE) {
            entry.action[D20_LABEL_LEN-1] = '0';
            mod++;
        }
        if (d20_put_err_entry (&entry, tmpfp) == D2_ERROR) {
	    d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E112IDX];
	    d20_handlemask(d20_qomask);
	    fclose(lockfp);
	    fclose(errfp);
            fclose(tmpfp);
            if (unlink (tmp_file_name) == -1) {
                d20_qomask[D20_Q1POS].d20_ioinfo=d20_err_table[D20_E113IDX];
	        d20_handlemask(d20_qomask);
            }
	    return(D2_NOERROR);
        }
    }
}
if (next) {
    d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E114IDX];
    d20_handlemask(d20_qomask);
}

fclose(lockfp);
fclose(errfp);

if (upd) {
    fclose(tmpfp);
    if (mod) {
        if (unlink (err_file_name) == -1) {
            d20_qomask[D20_Q1POS].d20_ioinfo=d20_err_table[D20_E115IDX];
            d20_handlemask(d20_qomask);
            return(D2_NOERROR);
        }
        if (link (tmp_file_name, err_file_name) == -1) {
            if (errno == ENOENT)
                d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E22IDX];
            else
                d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E115IDX];
            d20_handlemask(d20_qomask);
            return(D2_NOERROR);
        }
    }
    if (unlink (tmp_file_name) == -1) {
        d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E113IDX];
        d20_handlemask(d20_qomask);
    }
}
	
return(D2_NOERROR);
} /* end of d20_C04_show_error_file */

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d20_get_err_entry()                                    */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function reads an entry from the shadowing job           */
/*      error message file.                                           */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      entry   = structure to hold the new entry                     */
/*      fp      = file to be read from                                */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                    None                                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR  = The function has been performed successfully.   */
/*      D2_ERROR  = Some error					      */
/*                                                                    */
/*exoff ***************************************************************/

static d2_ret_val d20_get_err_entry(
  Shdupd_err_entry * entry,
  FILE * fp)
{
    if (fgets(entry->tstamp,D20_LABEL_LEN + D20_CONTENTS_LEN, fp) == NULL)
        return (D2_ERROR);
    else
        entry->tstamp[strlen(entry->tstamp)-1] = 0;

    if (fgets(entry->etype, D20_LABEL_LEN + D20_CONTENTS_LEN, fp) == NULL)
        return (D2_ERROR);
    else
        entry->etype[strlen(entry->etype)-1] = 0;

    if (fgets(entry->dirid, D20_LABEL_LEN + 3, fp) == NULL)
        return (D2_ERROR);
    else
        entry->dirid[strlen(entry->dirid)-1] = 0;

    if (fgets(entry->dname, D20_LABEL_LEN + D2_DNL_MAX, fp) == NULL)
        return (D2_ERROR);
    else
        entry->dname[strlen(entry->dname)-1] = 0;

    if (fgets(entry->oname, D20_LABEL_LEN + D2_DNL_MAX, fp) == NULL)
        return (D2_ERROR);
    else
        entry->oname[strlen(entry->oname)-1] = 0;

    if (fgets(entry->opcode,D20_LABEL_LEN + D20_CONTENTS_LEN, fp) == NULL)
        return (D2_ERROR);
    else
        entry->opcode[strlen(entry->opcode)-1] = 0;

    if (fgets(entry->etime, D20_LABEL_LEN + D20_CONTENTS_LEN, fp) == NULL)
        return (D2_ERROR);
    else
        entry->etime[strlen(entry->etime)-1] = 0;

    if (fgets(entry->ecode, D20_LABEL_LEN + D20_CONTENTS_LEN, fp) == NULL)
        return (D2_ERROR);
    else
        entry->ecode[strlen(entry->ecode)-1] = 0;

    if (fgets(entry->ecount,D20_LABEL_LEN + D20_CONTENTS_LEN, fp) == NULL)
        return (D2_ERROR);
    else
        entry->ecount[strlen(entry->ecount)-1] = 0;

    if (fgets(entry->action,D20_LABEL_LEN + 3, fp) == NULL)
        return (D2_ERROR);
    else
        entry->action[strlen(entry->action)-1] = 0;

    if (fgets(entry->line,  D20_LINE_LEN  + 1, fp) == NULL)
        return (D2_ERROR);
    else
        entry->line[strlen(entry->line)-1] = 0;

    return (D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d20_put_err_entry()                                    */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function writes an entry to the shadowing job            */
/*      error message file.                                           */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      entry   = structure which holds the new entry                 */
/*      fp      = file to be written to                               */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                    None                                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR  = The function has been performed successfully.   */
/*      D2_ERROR  = Some error					      */
/*                                                                    */
/*exoff ***************************************************************/

static d2_ret_val d20_put_err_entry(
  Shdupd_err_entry * entry,
  FILE * fp)
{
    entry->tstamp[strlen(entry->tstamp)] = '\n';
    if (fputs(entry->tstamp,fp) == 0)
        return (D2_ERROR);

    entry->etype[strlen(entry->etype)] = '\n';
    if (fputs(entry->etype,fp) == 0)
        return (D2_ERROR);

    entry->dirid[strlen(entry->dirid)] = '\n';
    if (fputs(entry->dirid,fp) == 0)
        return (D2_ERROR);

    entry->dname[strlen(entry->dname)] = '\n';
    if (fputs(entry->dname,fp) == 0)
        return (D2_ERROR);

    entry->oname[strlen(entry->oname)] = '\n';
    if (fputs(entry->oname,fp) == 0)
        return (D2_ERROR);

    entry->opcode[strlen(entry->opcode)] = '\n';
    if (fputs(entry->opcode,fp) == 0)
        return (D2_ERROR);

    entry->etime[strlen(entry->etime)] = '\n';
    if (fputs(entry->etime,fp) == 0)
        return (D2_ERROR);

    entry->ecode[strlen(entry->ecode)] = '\n';
    if (fputs(entry->ecode,fp) == 0)
        return (D2_ERROR);

    entry->ecount[strlen(entry->ecount)] = '\n';
    if (fputs(entry->ecount,fp) == 0)
        return (D2_ERROR);

    entry->action[strlen(entry->action)] = '\n';
    if (fputs(entry->action,fp) == 0)
        return (D2_ERROR);

    entry->line[strlen(entry->line)] = '\n';
    if (fputs(entry->line,fp) == 0)
        return (D2_ERROR);

    return (D2_NOERROR);
}
