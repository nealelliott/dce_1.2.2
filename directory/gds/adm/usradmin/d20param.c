/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20param.c,v $
 * Revision 1.1.712.2  1996/02/18  19:42:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:14  marty]
 *
 * Revision 1.1.712.1  1995/12/08  15:21:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:02  root]
 * 
 * Revision 1.1.710.5  1994/08/08  13:20:57  marrek
 * 	Fix for OT11045.
 * 	[1994/08/08  13:18:49  marrek]
 * 
 * Revision 1.1.710.4  1994/06/21  14:43:58  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:25:33  marrek]
 * 
 * Revision 1.1.710.3  1994/05/10  15:50:22  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:14:48  marrek]
 * 
 * Revision 1.1.710.2  1994/03/23  15:07:07  keutel
 * 	March 1994 code drop
 * 	[1994/03/23  13:15:55  keutel]
 * 
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:18  keutel]
 * 
 * Revision 1.1.710.1  1994/02/22  18:04:17  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:39:08  marrek]
 * 
 * Revision 1.1.708.2  1993/10/14  16:46:41  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  16:45:35  keutel]
 * 
 * Revision 1.1.708.1  1993/10/13  17:30:01  keutel
 * 	October 1993 code drop (CR 9061)
 * 	[1993/10/13  11:38:32  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  11:07:28  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:42:40  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  10:08:53  marrek
 * 	correction of wrong length in filename
 * 	[1993/02/01  17:45:08  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:11:16  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:08:04  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/11  16:11:08  marrek
 * 	Configuration of dir-id '1' allowed
 * 	Use Macro MEDIUM_FILE instead of absolute value
 * 	[1992/12/11  13:25:54  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  15:41:37  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  09:15:02  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  16:51:55  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:20:10  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  20:02:26  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:46:18  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20param.c,v $ $Revision: 1.1.712.2 $ $Date: 1996/02/18 19:42:26 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20param.c                                          */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 04.10.89                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  : These functions analyse the parameters of gdsditadm */
/*                call.                                               */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*  0.1     |04.10.89 |  original                      |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <nl_types.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d20_adm.h>
#include <d20proto.h>

typedef struct
	{   signed32    f_idx;          /* index of input field in mask*/
	    signed32    f_no;           /* number of fields that       */
					/* should be set to D20_NO_OP  */
	    signed32    f_bits;         /* bit combination assigned to */
					/* identify this field         */
	} D20_f_info;

typedef struct
	{   D20_omask   *pmask;
	    signed32     f_no;          /* number of input fields      */
					/* in that mask                */
	    D20_f_info   f_info[D20_NOFIELDS];  /* field info:         */
						/*    -- index of field*/
						/*       in mask       */
						/*    -- number of     */
						/*       fields        */
	} D20_m_info;

extern signed32	m_dir_id ;		/* Directory identifier 	*/
extern signed32	m_dbtype ;		/* database system 		*/
extern signed32 m_authmech ;            /* authentication mechanism     */
extern signed32	m_conf_typ ;		/* configuration type 		*/
extern signed32	m_conf_mode ;		/* configuration mode 		*/
extern signed32	m_medium ;		/* storage medium (floppy/tape  */
					/* file)			*/
extern signed32	m_update ;		/* update-distribution 		*/
extern signed32	no_server ;		/* number of servers		*/
extern signed32	no_client ;		/* number of clients		*/
extern signed32	m_form ;		/* format storage medium	*/
extern signed32 m_opmode;		/* operation mode		*/
extern signed32 m_volumeno;		/* volume number		*/
extern Bool     m_print_err;		/* suppress error messages	*/
extern char     m_userkey[];
extern char     m_filename[];

					/* Masks */
extern D20_omask    confpar_mask[] ;
extern D20_omask    conf1_mask[] ;
extern D20_omask    conf1d_mask[] ;
extern D20_omask    conf2_mask[] ;
extern D20_omask    conf3_mask[] ;
extern D20_omask    conf_mask[] ;
extern D20_omask    dirsi_mask[] ;
extern D20_omask    dirsiA_mask[] ;
extern D20_omask    dirsiF_mask[] ;
extern D20_omask    direi_mask[] ;
extern D20_omask    direiA_mask[] ;
extern D20_omask    direiF_mask[] ;

static char tmpbuf[L_ZEILE] ;

static char commands[] =
		{ D20_ACT, D20_CONF, D20_DEACT, D20_STATE,
		  D20_RESTORE, D20_SAVE, D20_TR_OFF, D20_TR_ON, '\0' };

static signed32 cmd_field[] =        /* commands used in the main process */
		{ AK, KO, DE, DS, EI, SI, TE, TA, -1 };

				/* possible input combinations */
static signed32 params[] =
		{ D20C_PRINTERR | D20C_XON, /* activate */
					    /* configurate */
		  D20C_CONFTYPE | D20C_DIRID |
		  D20C_CONFMODE | D20C_PRINTERR |
		  D20C_NOSERVER | D20C_NOCLIENT | D20C_XON |
		  D20C_SUPP_MASK | D20C_AUTH_MECH,
		  D20C_PRINTERR | D20C_XON, /* deactivate */
					    /* state */
		  D20C_PRINTERR | D20C_XON | D20C_SUPP_MASK,
					    /* restore */
		  D20C_DIRID | D20C_USERKEY | D20C_MEDIUM | D20C_OPMODE |
		  D20C_PRINTERR | D20C_VOLUMENO | D20C_XON | D20C_FILENAME,
					    /* save */
		  D20C_DIRID | D20C_FORM_MEDIUM | D20C_USERKEY | D20C_MEDIUM |
		  D20C_OPMODE | D20C_PRINTERR | D20C_VOLUMENO | D20C_XON |
		  D20C_FILENAME,
		  D20C_PRINTERR | D20C_XON, /* trace off */
		  D20C_PRINTERR | D20C_XON  /* trace on */
		};

static signed32 conf_params[] =
		{                           /* create configuration */
		  D20C_CONFTYPE | D20C_DIRID |
		  D20C_CONFMODE | D20C_PRINTERR |
		  D20C_NOSERVER | D20C_NOCLIENT | D20C_XON |
		  D20C_AUTH_MECH,
					    /* delete configuration */
		  D20C_DIRID | D20C_CONFMODE | D20C_PRINTERR | D20C_XON,
					    /* display configuration */
		  D20C_CONFMODE | D20C_PRINTERR | D20C_XON | D20C_SUPP_MASK,
					    /* change configuration */
		  D20C_CONFTYPE | D20C_DIRID |
		  D20C_CONFMODE | D20C_PRINTERR |
		  D20C_NOSERVER | D20C_NOCLIENT | D20C_XON |
		  D20C_AUTH_MECH
		};

			    /* configuration parameters 
			     * ("-D" only for change configuration )
			     */
static signed32 config_par[] =
		{ 	    /* Client-System */
		  D20C_DIRID | D20C_CONFMODE |
		  D20C_PRINTERR | D20C_NOCLIENT | D20C_XON | D20C_SUPP_MASK,
			    /* Client-Server-System */
		  D20C_DIRID | D20C_CONFMODE | D20C_PRINTERR | D20C_NOSERVER |
		  D20C_NOCLIENT | D20C_XON | D20C_SUPP_MASK |
		  D20C_AUTH_MECH
		};

static signed32 save_params[] =
		{ 0,
					    /* op_mode = 1 */
		  D20C_DIRID | D20C_OPMODE | D20C_USERKEY | D20C_MEDIUM |
		  D20C_PRINTERR | D20C_XON,
					    /* op_mode = 2 */
		  D20C_DIRID | D20C_OPMODE | D20C_VOLUMENO | D20C_MEDIUM |
		  D20C_FORM_MEDIUM | D20C_PRINTERR | D20C_XON | D20C_FILENAME,
					    /* op_mode = 3 */
		  D20C_DIRID | D20C_OPMODE | D20C_PRINTERR | D20C_XON,
					    /* op_mode = 4 */
		  D20C_DIRID | D20C_OPMODE | D20C_PRINTERR | D20C_XON
		};

static signed32 restore_params[] =
		{ 0,
					    /* op_mode = 1 */
		  D20C_DIRID | D20C_OPMODE | D20C_USERKEY | D20C_VOLUMENO |
		  D20C_FILENAME | D20C_MEDIUM | D20C_PRINTERR | D20C_XON,
					    /* op_mode = 2 */
		  D20C_DIRID | D20C_OPMODE | D20C_VOLUMENO | D20C_FILENAME |
		  D20C_MEDIUM | D20C_PRINTERR | D20C_XON,
					    /* op_mode = 3 */
		  D20C_DIRID | D20C_OPMODE | D20C_PRINTERR | D20C_XON,
					    /* op_mode = 4 */
		  D20C_DIRID | D20C_OPMODE | D20C_PRINTERR | D20C_XON
		};

static D20_m_info m_info[] =
		  { { confpar_mask, 1,
		      { D20_P0CONFMODE, D20_N0CONFMODE, D20C_CONFMODE } },
		    { conf1_mask, 1,
		      { D20_P1DIRID, D20_N1DIRID, D20C_DIRID } },
		    { conf1d_mask, 1,
		      { D20_P1DIRID, D20_N1DIRID, D20C_DIRID } },
		    { conf2_mask, 2,
		      { { D20_P2CONFTYPE, D20_N2CONFTYPE, D20C_CONFTYPE },
		        { D20_P2CLIENT, D20_N2CLIENT, D20C_NOCLIENT } } },
		    { conf3_mask, 2,
		      { { D20_P3NOSERVER, D20_N3NOSERVER, D20C_NOSERVER },
			{ D20_P3AUTHMECH, D20_N3AUTHMECH, D20C_AUTH_MECH } } },
		    { dirsi_mask, 2,
		      { { D20_P4DIRID, D20_N4DIRID, D20C_DIRID },
			{ D20_P4MEDIUM, D20_N4MEDIUM, D20C_MEDIUM },
		      } },
		    { dirsiA_mask, 2,
		      { { D20_P6USERKEY, D20_N6USERKEY, D20C_USERKEY },
			{ D20_P6FORM_MEDIUM, D20_N6MEDIUM, D20C_FORM_MEDIUM }
		      } },
		    { dirsiF_mask, 2,
		      { { D20_P7FILENAME, D20_N7FILENAME, D20C_FILENAME },
			{ D20_P7USERKEY, D20_N7USERKEY, D20C_USERKEY }
		      } },
		    { direi_mask, 2,
		      { { D20_P5DIRID, D20_N5DIRID, D20C_DIRID },
			{ D20_P5MEDIUM, D20_N5MEDIUM, D20C_MEDIUM }
		      } },
		    { direiA_mask, 1,
		      { { D20_P8USERKEY, D20_N8USERKEY, D20C_USERKEY } 
		      } },
		    { direiF_mask, 2,
		      { { D20_P9FILENAME, D20_N9FILENAME, D20C_FILENAME },
			{ D20_P9USERKEY, D20_N9USERKEY, D20C_USERKEY } 
		      } }
		  };


/*exon ***************************************************************/
/*                                                                   */
/*   TYPE:    function                                               */
/*                                                                   */
/*   NAME:    d20_displ_mask()                                       */
/*                                                                   */
/*   SHORT DESCRIPTION:                                              */
/*      This functions sets all fields in the mask to D20_NO_OP if   */
/*      the input in this field is given as parameter to "gdsditadm".*/
/*                                                                   */
/*   INPUT-PARAMETER:                                                */
/*      act_params  = parameter combination of "gdsditadm" call      */
/*      pmask       = mask in which the fileds should be set to      */
/*                    D20_NO_OP                                      */
/*      sel_cmd     = selected command ( < 0, if "gdsditadm" called  */
/*                    without parameters)                            */
/*                                                                   */
/*   OUTPUT-PARAMETER:                                               */
/*      fkt_key     = fkt_key pressed in mask                        */
/*                                                                   */
/*   RETURN_VALUE:                                                   */
/*      D2_TRUE:        mask displayed                               */
/*      D2_ERROR:       mask not displayed                           */
/* ------------------------------------------------------------------*/
/*   AUTHOR: W.Schmid                               DATE: 03.10.89   */
/*exoff **************************************************************/

 Bool d20_displ_mask(
   signed32   act_params,
   D20_omask *pmask,
   signed32   sel_cmd,
   signed32  *fkt_key)
{
signed32 i_max;
signed32 idx;
signed32 idx_max;
Bool m_display = FALSE;
D20_m_info *p_info;
D20_m_info *p_maxinfo;
D20_f_info *f_info;
D20_f_info *f_maxinfo;

if (sel_cmd >= 0)
{   for (i_max = sizeof(m_info) / sizeof(D20_m_info),
	 p_info = &m_info[0], p_maxinfo = p_info + i_max;
	 p_info < p_maxinfo; p_info++)
    {
	if (pmask == p_info->pmask)
	{   break;
	}
    }

    for  (f_info = &p_info->f_info[0], f_maxinfo = f_info + p_info->f_no;
	  f_info < f_maxinfo; f_info++)
    {   if                          /* input to mask is given as parameter */
	   ((act_params & f_info->f_bits) != 0)
	{                           /* set all relevant mask fields to     */
				    /* D20_NO_OP                           */
	    for (idx = f_info->f_idx, idx_max = idx + f_info->f_no;
		 idx < idx_max; idx++)
	    {
		if (((pmask + idx)->d20_fkt_id & D20_NOERASE) == D20_NOERASE)
		{
		(pmask + idx)->d20_fkt_id = D20_NO_OP | D20_NOERASE;
		}
		else
		{
		(pmask + idx)->d20_fkt_id = D20_NO_OP;
		}
	    }
	}
	else
	{    m_display = TRUE;
	}
    }
}
else
{   m_display = TRUE;
}

if (m_display == TRUE)
{   *fkt_key = d20_handlemask(pmask);
}

return(m_display);
}


/*inon ***************************************************************/
/*                                                                   */
/*   TYPE:    function                                               */
/*                                                                   */
/*   NAME:    d20_get_str()                                          */
/*                                                                   */
/*   SHORT DESCRIPTION:                                              */
/*      This functions checks whether the given string is an         */
/*      alphanumerical string (for filename with '.' and '_') with a */
/*	given maximum length.					     */
/*                                                                   */
/*   INPUT-PARAMETER:                                                */
/*      str         = input string to be checked                     */
/*      name        = name of the array where the string has to be   */
/*		      stored					     */
/*      max_length  = maximum length of input string                 */
/*	param	    = given parameter				     */
/*                                                                   */
/*   OUTPUT-PARAMETER:                                               */
/*      None                                                         */
/*                                                                   */
/*   RETURN_VALUE:                                                   */
/*      D2_NOERROR:     input string is correct                      */
/*      otherwise:      exit(1)                                      */
/* ------------------------------------------------------------------*/
/*   AUTHOR: W.Schmid                               DATE: 03.10.89   */
/*inoff **************************************************************/

 signed32 d20_get_str(
   char     *str,
   char     *name,
   signed32  max_length,
   char  param)
{
signed32   length = 0;
char *sav_str = str - 2;

switch ((int ) param)
{   case D20_USERKEY:  	for ( ; *str != D2_EOS; str++, length++)
			{   if (length >= max_length || isalnum((int) *str) == 0)
    			    {   fprintf (stderr, D20_WR_SYNTAX, sav_str);
			        exit (1) ;
    			    }
			}
			break;
    case D20_FILENAME: 	if (is_allowed(str) == D2_ERROR)
    			{   fprintf (stderr, D20_WR_SYNTAX, sav_str);
			    exit (1) ;
    			}
			length=strlen(str);
			break;
}

if (length == 0)
{   *name = D2_EOS;
}
else
{   strcpy(name, sav_str + 2);
}

return(D2_NOERROR);
}


/*inon ***************************************************************/
/*                                                                   */
/*   TYPE:    function                                               */
/*                                                                   */
/*   NAME:    d20_get_val()                                          */
/*                                                                   */
/*   SHORT DESCRIPTION:                                              */
/*      This functions checks whether the given string is a numerical*/
/*      string. The value should be in the given boundaries.         */
/*                                                                   */
/*   INPUT-PARAMETER:                                                */
/*      str         = input string to be checked                     */
/*      min         = minimum integer value                          */
/*      max         = maximum integer value                          */
/*                                                                   */
/*   OUTPUT-PARAMETER:                                               */
/*      val         = integer value of input string                  */
/*                                                                   */
/*   RETURN_VALUE:                                                   */
/*      D2_NOERROR:     input string is correct                      */
/*      otherwise:      exit(1)                                      */
/* ------------------------------------------------------------------*/
/*   AUTHOR: W.Schmid                               DATE: 03.10.89   */
/*inoff **************************************************************/

 signed32 d20_get_val(
   char      *str,
   signed32   min,
   signed32   max,
   signed32  *val)
{
char *sav_str = str;

for ( ; *str != D2_EOS; str++)
{   if (isdigit((int) *str) == 0)
    {   fprintf (stderr, D20_WR_SYNTAX, sav_str - 2);
	exit (1) ;
    }
}

if (sav_str == str)
{   fprintf (stderr, D20_WR_SYNTAX, sav_str - 2) ;
    exit (1) ;
}

*val = atoi(sav_str);

if ( (max < 0 && *val < min) || ((max >= 0) && (*val < min || *val > max)) )
{   fprintf (stderr, D20_WR_SYNTAX, sav_str - 2) ;
    exit (1) ;
}

return(D2_NOERROR);
}



/*exon ***************************************************************/
/*                                                                   */
/*   TYPE:    function                                               */
/*                                                                   */
/*   NAME:    d20_get_params()                                       */
/*                                                                   */
/*   SHORT DESCRIPTION:                                              */
/*      This functions checks all parameters of "gdsditadm" and      */
/*      sets some global variables indicating which parameters are   */
/*      present.                                                     */
/*                                                                   */
/*   INPUT-PARAMETER:                                                */
/*      argc        = number of params to be checked                 */
/*      argv        = parameters of "gdsditadm"                      */
/*      dir         = installation path name to "config" file        */
/*                                                                   */
/*   OUTPUT-PARAMETER:                                               */
/*      cmd         = selected command                               */
/*      act_params  = bit field indicating which parameters are set  */
/*      all_params  = a flag indicatin whether all parameters for    */
/*                    the specific "gdsditadm" call are entered or   */
/*		      not (except "-p")                              */
/*                                                                   */
/*   RETURN_VALUE:                                                   */
/*      D2_NOERROR:     parameters are correct                       */
/*      otherwise:      exit(1)     --> wrong parameters             */
/*                      exit(9)     --> wrong function selection     */
/*                      EXIT3:  changing configuration with parameter*/
/*                               "-D":                               */
/*                              old configuration can't be read      */
/*                      EXIT6:  changing configuration with parameter*/
/*                               "-D":                               */
/*                              old configuration doesn't exist      */
/* ------------------------------------------------------------------*/
/*   AUTHOR: W.Schmid                               DATE: 03.10.89   */
/*exoff **************************************************************/

 signed32 d20_get_params(
   int       argc,
   char     *argv[],
   char     *dir,
   signed32 *cmd,
   signed32 *act_params,
   Bool     *all_params)
{
signed32   sel_cmd;          /* index of selected command */
char *pos;
char *str, *str1;
Bool  dsa_flag = FALSE;      /* indicates whether gdsdsa exists*/
struct stat statinfo;
char  ch[3];
char  cmdbuf[CMDBUF] ;
FILE *in_fd;

register signed32    i;

m_print_err = TRUE;
*act_params = 0;
*all_params = FALSE;
m_userkey[0] = D2_EOS;
m_filename[0] = D2_EOS;
m_conf_typ = -1;
m_volumeno = 1;

				/* evaluate all calling parameters */
if (argc <= 1)
{   *cmd = -1;
    return(D2_NOERROR);
}
else
{                               /* check whether first parameter is a */
				/* correct command                    */
    str = argv[1];
    if (*argv[1]++ != '-')
    {   fprintf (stderr, D20_WR_CODE, str) ;
	exit (9) ;
    }

    if (*argv[1]++ != D20_FUNC)
    {   fprintf (stderr, D20_WR_CODE, str) ;
	exit (9) ;
    }

    if ((pos = strchr(commands, *argv[1])) == (char *) 0)
    {   fprintf (stderr, D20_WR_CODE, str) ;
	exit (9) ;
    }
    else
    {   sel_cmd = pos - commands;
	*cmd = cmd_field[sel_cmd];
    }

    argv[1]++;
    if (*argv[1] != D2_EOS)
    {   fprintf (stderr, D20_WR_CODE, str) ;
	exit (9) ;
    }
}

				/* get all parameters for selected command */
for (i = 2; i < argc; i++)
{   str = argv[i];
    if (*argv[i]++ != '-')
    {   fprintf (stderr, D20_WR_SYNTAX, str) ;
	exit (1) ;
    }

    switch ((int ) *argv[i]++)
    {   case D20_CONFTYPE:      d20_get_val(argv[i], D20_1CONFTYPE,
					    D20_2CONFTYPE, &m_conf_typ);
				if ((params[sel_cmd] & D20C_CONFTYPE) == 0)
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}
				m_conf_typ--;
				sprintf (tmpbuf, DSAPATH, dcelocal_path, D27_BIN_DIR);
				if (dsa_flag == FALSE)
				      if (stat(tmpbuf, &statinfo) != 0)
					   {
					   if (m_conf_typ == CLTSRV_GLOBAL)
                                          	{
						fprintf (stderr, D20_WR_SYNTAX, str) ;
					        exit (1) ;
					   	}
				           }	
				      else
					   dsa_flag = TRUE;
				*act_params |= D20C_CONFTYPE;
				break ;
	case D20_DIRID:         d20_get_val(argv[i], D20_1DIRID, D20_2DIRID,
					    &m_dir_id);
				str1 = str ;
				if ((params[sel_cmd] & D20C_DIRID) == 0)
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}
				*act_params |= D20C_DIRID;
				break ;
	case D20_FORM_MEDIUM:   d20_get_val(argv[i], D20_1FORM_MEDIUM,
					    D20_2FORM_MEDIUM, &m_form);
				if ((params[sel_cmd] & D20C_FORM_MEDIUM)==0)
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}
				*act_params |= D20C_FORM_MEDIUM;
				break ;
	case D20_USERKEY:       d20_get_str(argv[i], m_userkey, D20_LKEY, D20_USERKEY);
				if ((params[sel_cmd] & D20C_USERKEY) == 0)
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}
				*act_params |= D20C_USERKEY;
				break ;
	case D20_CONFMODE:      d20_get_val(argv[i], D20_1CONFMODE,
					    D20_2CONFMODE, &m_conf_mode);
				if ((params[sel_cmd] & D20C_CONFMODE) == 0)
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}
				*act_params |= D20C_CONFMODE;
				break ;
	case D20_MEDIUM:        d20_get_val(argv[i], D20_1MEDIUM,
					    D20_2MEDIUM, &m_medium);
				if ((params[sel_cmd] & D20C_MEDIUM) == 0)
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}
				*act_params |= D20C_MEDIUM;
				break ;
	case D20_OPMODE:        d20_get_val(argv[i], D20_1OPMODE,
					    D20_2OPMODE, &m_opmode);
				if ((params[sel_cmd] & D20C_OPMODE) == 0)
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}
				*act_params |= D20C_OPMODE;
				break ;
	case D20_PRINTERR:      if (*argv[i] != D2_EOS)
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}
				*act_params |= D20C_PRINTERR;
				m_print_err = FALSE;
				break ;
	case D20_NOSERVER:      d20_get_val(argv[i], D20_1NOSERVER,
					    D20_2NOSERVER, &no_server);
				if ((params[sel_cmd] & D20C_NOSERVER) == 0)
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}
				*act_params |= D20C_NOSERVER;
				break ;
	case D20_NOCLIENT:      d20_get_val(argv[i], D20_1NOCLIENT,
					    D20_2NOCLIENT, &no_client);
				if ((params[sel_cmd] & D20C_NOCLIENT) == 0)
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}
				*act_params |= D20C_NOCLIENT;
				break ;
	case D20_VOLUMENO:      d20_get_val(argv[i], D20_1VOLUMENO,
					    D20_2VOLUMENO, &m_volumeno);
				if ((params[sel_cmd] & D20C_VOLUMENO) == 0)
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}
				*act_params |= D20C_VOLUMENO;
				break ;
	case D20_XON:           if (*argv[i] != D2_EOS)
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}
				*act_params |= D20C_XON;
				break ;
	case D20_SUPP_MASK:     if (*argv[i] != D2_EOS ||
				    (params[sel_cmd] & D20C_SUPP_MASK) == 0)
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}
				*act_params |= D20C_SUPP_MASK;
						/* suppress error messages */
				*act_params |= D20C_PRINTERR;
				m_print_err = FALSE;
				break ;

	case D20_FILENAME:      d20_get_str(argv[i], m_filename, D20_LFILE, D20_FILENAME);
				if ((params[sel_cmd] & D20C_FILENAME) == 0)
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}
				*act_params |= D20C_FILENAME;
				break ;

	case D20_AUTH_MECH:     /* Convert entry value in intern values */
				/* Not used at the moment. The intern	*/
				/* values are used.			*/

				/*
				switch(*argv[i])
				{
				    case '1':
					sprintf(argv[i], "%d", D2_ANONYMOUS);
					break;
				    case '2':
					sprintf(argv[i], "%d", D2_SIMPLE);
					break;
				    case '3':
					sprintf(argv[i], "%d", D2_STRONG);
					break;
				    case '4':
					sprintf(argv[i], "%d", D2_DCE_AUTH);
					break;
				    default:
					fprintf (stderr, D20_WR_PARAM, str) ;
					exit (1) ;
				}
				*/

				d20_get_val(argv[i], D2_ANONYMOUS,
					    D2_SIMPLE_STRONG, &m_authmech);

				/* For anonymous, no parameter is allowed ! */
				if (/* m_authmech != D2_ANONYMOUS && */
				    m_authmech != D2_SIMPLE     &&
				    m_authmech != D2_DCE_AUTH   &&
				    m_authmech != D2_STRONG     &&
				    m_authmech != D2_SIMPLE_DCE &&
				    m_authmech != D2_SIMPLE_STRONG )
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}

				if ((params[sel_cmd] & D20C_AUTH_MECH) == 0)
				{   fprintf (stderr, D20_WR_PARAM, str) ;
				    exit (1) ;
				}
				*act_params |= D20C_AUTH_MECH;
				break ;

	default:                fprintf (stderr, D20_WR_SYNTAX, str) ;
				exit (1) ;
    }
}

				/* check if all parameters are entered */
switch (*pos)
{ case D20_CONF:
    if ((*act_params & D20C_CONFMODE) == D20C_CONFMODE)
    {   if ((*act_params &
	      ~(D20C_PRINTERR | D20C_XON | D20C_SUPP_MASK)) ==
	    (conf_params[m_conf_mode-1] &
	      ~(D20C_PRINTERR | D20C_XON | D20C_SUPP_MASK)))
	{   *all_params = TRUE;
	}

	if (m_conf_mode == KDEL && (*act_params & D20C_DIRID) == D20C_DIRID && m_dir_id == D20_1DIRID)
	{   fprintf (stderr, D20_WR_PARAM, str1) ;
	    exit (1) ;
	}

				/* check if configuration mode is "change"
				 * and parameter "-D" is set
				 */
	if (m_conf_mode == KCHG &&
	    (*act_params & D20C_SUPP_MASK) == D20C_SUPP_MASK)
	{                       /* check whether DIR-ID is set */
	    if ((*act_params & D20C_DIRID) != D20C_DIRID)
	    {   fprintf(stderr, D20_WR_COMB);
		exit(1);
	    }

				/* take old configuration type */
	    sprintf (cmdbuf, PREPMASK, dir, m_dir_id, dir);
	    if ((in_fd = popen(cmdbuf,"r")) == NULL)
		   exit(EXIT2>>8);
	    if (fscanf(in_fd,"%d", &m_conf_typ) != 1)
		   exit(EXIT2>>8);
	    pclose(in_fd);

	    if (m_conf_typ == -1) {
		    /* no configuration information available */
		exit(EXIT6>>8);
	    }

				/* check whether the parameters are
				   valid for the configuration
				 */
	    if ((*act_params & ~config_par[m_conf_typ]) == 0)
	    {   *all_params = TRUE;
	    }
	    else
	    {   fprintf(stderr, D20_WR_COMB);
		exit(1);
	    }
	}
	else
	{                       /* check parameters depending on
				 * configuration type
				 */
	    if ((*act_params & D20C_CONFTYPE) == D20C_CONFTYPE)
		*all_params = ((*act_params | D20C_CONFTYPE | D20C_XON | D20C_PRINTERR | D20C_SUPP_MASK) &
			       config_par[m_conf_typ]) ==
			       config_par[m_conf_typ] ? TRUE : FALSE;
	}
    }

    if (*all_params == FALSE &&
	(*act_params & D20C_SUPP_MASK) == D20C_SUPP_MASK)
    {   sprintf(ch, "-%c", D20_SUPP_MASK);
	fprintf(stderr, D20_WR_PARAM, ch);
	exit(1);
    }

    if ((*act_params & D20C_CONFMODE) == D20C_CONFMODE &&
	m_conf_mode == KDIS)
    {   *all_params = ((*act_params & D20C_SUPP_MASK) == D20C_SUPP_MASK) ?
		      TRUE : FALSE;
    }
    break;

  case D20_SAVE:
    if ((*act_params & D20C_OPMODE) != 0)
    {   if (((*act_params & ~(D20C_PRINTERR | D20C_XON)) ==
	    (save_params[m_opmode] & ~(D20C_PRINTERR | D20C_XON))) ||
	    (m_medium != MEDIUM_FILE &&
	    (*act_params & ~(D20C_PRINTERR | D20C_XON | D20C_FILENAME)) ==
	    (save_params[m_opmode] & ~(D20C_PRINTERR | D20C_XON | D20C_FILENAME))) ||
	    (m_medium == MEDIUM_FILE &&
	    (*act_params & ~(D20C_PRINTERR | D20C_XON | D20C_FORM_MEDIUM | D20C_VOLUMENO)) ==
	    (save_params[m_opmode] & ~(D20C_PRINTERR | D20C_XON | D20C_FORM_MEDIUM | D20C_VOLUMENO))))

	{   *all_params = TRUE;
	}
	else
	{   fprintf (stderr, D20_WR_COMB) ;
	    exit (1) ;
	}

    }

    /* Volume number of data medium and format data medium only allowed for */
    /* save to disk/tape */
    if ((m_medium == MEDIUM_FILE) && ((*act_params & D20C_FORM_MEDIUM) == D20C_FORM_MEDIUM || (*act_params & D20C_VOLUMENO) == D20C_VOLUMENO))
    {   fprintf (stderr, D20_WR_COMB) ;
        exit (1) ;
    }
    /* Filename only allowed while save to file */
    if ((m_medium != MEDIUM_FILE) && (*act_params & D20C_FILENAME) == D20C_FILENAME)
    {   fprintf (stderr, D20_WR_COMB) ;
        exit (1) ;
    }
    break;

  case D20_RESTORE:
    if ((*act_params & D20C_OPMODE) != 0)
    {   if (((*act_params & ~(D20C_PRINTERR | D20C_XON)) ==
	    (restore_params[m_opmode] & ~(D20C_PRINTERR | D20C_XON))) ||
            (m_medium != MEDIUM_FILE &&
	    (*act_params & ~(D20C_PRINTERR | D20C_XON | D20C_FILENAME)) ==
	    (restore_params[m_opmode] & ~(D20C_PRINTERR | D20C_XON | D20C_FILENAME))) || 
            (m_medium == MEDIUM_FILE &&
	    (*act_params & ~(D20C_PRINTERR | D20C_XON | D20C_VOLUMENO)) ==
	    (restore_params[m_opmode] & ~(D20C_PRINTERR | D20C_XON | D20C_VOLUMENO))))
	{   *all_params = TRUE;
	}
	else
	{   fprintf (stderr, D20_WR_COMB) ;
	    exit (1) ;
	}
    }

    /* Volume number of data medium only allowed for restore from disk/tape */
    if ((m_medium == MEDIUM_FILE) && (*act_params & D20C_VOLUMENO) == D20C_VOLUMENO)
    {   fprintf (stderr, D20_WR_COMB) ;
	exit (1) ;
    }
    /* Filename only allowed while restore from file */
    if ((m_medium != MEDIUM_FILE) && (*act_params & D20C_FILENAME) == D20C_FILENAME)
    {   fprintf (stderr, D20_WR_COMB) ;
        exit (1) ;
    }
    break;

  case D20_STATE:
    *all_params = ((*act_params & D20C_SUPP_MASK) == D20C_SUPP_MASK) ?
		  TRUE : FALSE;
    break;

  default:
    if ((*act_params & ~(D20C_PRINTERR | D20C_XON))
	 == (params[sel_cmd] & ~(D20C_PRINTERR | D20C_XON)))
    {   *all_params = TRUE;
    }
    break;
}
return(D2_NOERROR);
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_disp_clhelp()                         */
/* Description         : Displays help if asked in the command     */
/*                       line option                               */
/* Return Value        : void                                      */
/* Author              : Jean-Marc Bonnaudet, SNI BU BA NM 123     */
/*                                                                 */
/*-----------------------------------------------------------------*/

void d20_disp_clhelp( void )
{
int     i;					/* Counter		*/
nl_catd nl_fd;		/* Native Language Message File Descriptor	*/
char    *help_message_ptr;			/* Pointer to a message	*/
char    *mess_file_name = "gdsclhelp.cat";	/* Message File Name	*/
char    *default_mess[] =
    { "Usage:",
      "gdssysadm -f<function> [-d<directory ID>]  [-m<conf.mode>]",
      "          [-c<configuration type>] [-C<number of clients>]",
      "          [-s<number of servers>] [-u<delta update>] [-o<operation>]",
      "          [-M<data medium type>] [-n<filename>] [-k[<password>]]",
      "          [-F<formatting>] [-v<volume number>] [-D] [-p] [-X]",
      "          [-A<authentication mechanism>] [-h]"
    };

    nl_fd = catopen(mess_file_name, 0);

    for(i=1; i<D20_CLHELP_LINE_NR; i++)
    {
	help_message_ptr = catgets(nl_fd, 1, i, default_mess[i-1]);
	puts(help_message_ptr);
    }

    if(nl_fd != (nl_catd)(-1))
	catclose(nl_fd);
    else
    {
	fprintf( stderr,
	    "\nError while opening NLS file (%s).\n", mess_file_name );
	fprintf( stderr,
	    "You should check the variables NLS_PATH and LANG.\n" );
    }

    exit(0);
}



/* End of d20param.c */
