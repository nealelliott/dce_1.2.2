/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: exec_mgmt_cmd_cds.c,v $
 * Revision 1.1.8.2  1996/02/18  19:29:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:57  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:09:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:30  root]
 * 
 * Revision 1.1.6.7  1994/08/03  19:00:51  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:49:57  mccann]
 * 
 * Revision 1.1.6.6  1994/06/23  18:29:33  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:33:39  mccann]
 * 
 * Revision 1.1.6.5  1994/06/09  18:36:52  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:38  devsrc]
 * 
 * Revision 1.1.6.4  1994/04/19  17:05:41  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:29:50  mccann]
 * 
 * Revision 1.1.6.3  1994/04/04  20:06:01  peckham
 * 	Fixes for sams file cleanup
 * 	[1994/04/04  18:06:10  peckham]
 * 
 * Revision 1.1.6.2  1994/03/22  19:59:57  griffin
 * 	HCell BL3 support: Add/remove cellname commands
 * 	[1994/03/22  17:47:44  griffin]
 * 
 * Revision 1.1.6.1  1994/03/12  21:59:01  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:01:14  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:05:15  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:36:04  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:48:47  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:21:51  weisman]
 * 
 * Revision 1.1  1992/01/19  15:21:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 */

#include <cdscp.h>
#include <cmdblk.h>
#include <filespec.h>
#include <logging.h>
#include <dce/cprpc_util.h>

/* this routine executes a parsed command */
/* the command block has pointers to everything else needed */

int					/* return 1 for EXIT, else 0 */
exec_cmd (
    struct cmdblk *cbp,			/* pointer to command block */
    struct op_blk *obp,			/* ptr to head of output list */
    struct logblk *lbp,			/* ptr to logging block */
    int           *scp)                 /* ptr to status code */
{
    int command_status=0;		/* result of executing command*/
    int completion_code=0;		/* tell caller to continue or not */
    struct entblk *ent_p = entitylist_pointer();
    struct attrblk *att_p = get_attrblk_ptr();
    char *fsp;		

    switch (cbp->func)
	{
	case NULLCMD:		/* Null Command always works */
	    command_status = DNS_SUCCESS;
	    break;
	case EXITCMD:		/* Tell caller to exit */
	    completion_code = TRUE;	
	    command_status = DNS_SUCCESS;
	    break;
        case HELPCMD:
            command_status = cdscp_help();
            break;
	case SHOREPCMD:		/* Show a directory at a specific clear. */
	    emit_reference_header(obp, ent_p->nxt,
		CDSCP_REFHDR_OPLINE_SHOW, "SHOW");
	    command_status = 
	      visit_attr_list(ent_p->nxt, att_p, cbp->envptr, cbp, obp);
	    break;
	case SHOENTCMD:		
	    /* Expand entry into list of entries and show attributes */
	    command_status =
		visit_entlist(ent_p, withlist_pointer(),
			      att_p, cbp->envptr, cbp, obp);
	    break;
        case SHOWDNS:           /* Show data for BIND-DNS entry */
        case SHOW500:           /* Show data for X500 GDS entry */
	    emit_reference_header(obp, ent_p->nxt,
		CDSCP_REFHDR_OPLINE_SHOW, "SHOW");
	    command_status = showcell(cbp->func, ent_p->nxt, cbp->envptr, obp);
	    break;
	case V1CRELNKCMD:	/* Create link */

	    command_status =
	      cresl(ent_p, att_p, cbp->envptr, cbp, obp);
	    break;
	  case V1CREDIRCMD:	/* Create directory */
	  case V1DELDIRCMD:	/* Delete directory */
	  case SKULKCMD:	/* Skulk */
	  case NEWEPOCHCMD:     /* set directory to new epoch */
	    command_status =
		do_dir_cmd((int)cbp->func, ent_p, att_p, 
			   cbp->envptr, cbp, obp);
	    break;
	case V1DELLNKCMD:	/* Delete link */
	    command_status =
		delsl(ent_p, att_p, cbp->envptr, cbp, obp);
	    break;
	case V1DELOBJCMD:	/* v1 delete object */
	    command_status =
		delobj(ent_p, att_p, cbp->envptr, cbp, obp);
	    break;

	case V1CREOBJCMD:	/* Create object */
	    command_status =
		creobj(ent_p, att_p, cbp->envptr, cbp, obp);
	    break;

	  case CREREPCMD:
	  case DELREPCMD:
	    command_status =
		do_replica_cmd((int)cbp->func, ent_p, att_p,
			       cbp->envptr, cbp, obp);
	    break;
	case SETENTCMD:		/* set entity */
	case ADDSETCMD:		/* add to a set */
	case REMSETCMD:		/* remove from a set */
	    command_status =  set_attr_line(ent_p->nxt, att_p->fwd, 
					    cbp->envptr, cbp);
	    break;
	case DIRDIRCMD:
	case DIROBJCMD:
	case DIRGRPCMD:
	case DIRCLHCMD:
	case DIRLNKCMD:
	    emit_reference_header(obp, ent_p->nxt,
		CDSCP_REFHDR_OPLINE_DIRECTORY, "LIST");
	    command_status = 
	      enum_things(ent_p->nxt, withlist_pointer(),
			     cbp->envptr, cbp, obp);
	    break;

	case CREKIDCMD:		/* create child pointer, after checks */
	    command_status = 
		crekid(ent_p, att_p, cbp->envptr, cbp, obp);
	    break;

	case DELKIDCMD:		/* delete child pointer, after checks */
	    command_status = delkid(ent_p, att_p, cbp->envptr,
				    cbp, obp);
	    break;

	case SETCFCMD:		/* set  the confidence to use on clerk calls */
	    command_status = 
		set_conf( cbp->envptr, cbp, obp);
	    break;

	case SHOCFCMD:		/* show the confidence to use on clerk calls */
	    command_status = 
		show_conf(cbp->envptr, cbp, obp);
	    break;

        case SETPFCCMD:		/* set cdscp preferred clearinghouse */
	    command_status = set_preferred_clh(cbp->envptr, cbp, obp);
	    break;

        case SHOPFCCMD:		/* show cdscp preferred clearinghouse */
	    command_status = show_preferred_clh(cbp->envptr, cbp, obp);
	    break;

        case SHOWSRV:
        case CRECLECMD:
        case DELCLECMD:
	case DISSRV:
	case CLEARCLE:
	    command_status = 
		cp_server_cmd((int)cbp->func,ent_p, att_p, 
			      cbp->envptr, obp);
	    
	    break;
	case SHOWCLK:
        case SHOWCLKCLE:    
        case SHOWCLKSRV:    
        case DISCLK:
     	case DEFCLKSRV:
	case CLEARCLKSRV:
	    command_status = 
	      cp_clerk_cmd((int)cbp->func, ent_p, att_p, cbp->envptr, obp);
	    break;
	case DMPCCHCMD:
	    command_status = dumpclerkcachecmd();
            if (command_status != DNS_SUCCESS)
            {
                mgmt_error(command_status);
            }
	    break;
/* New hierarchical cell commmands: */
        case ADDCELLPREFERCMD:
        case ADDCELLALIASCMD:
        case ADDCELLCHILDCMD:
        case REMCELLCMD:
	    command_status = cp_hiercell(cbp->func, ent_p->nxt, cbp->envptr, obp);
	    break;


	default:
	    dce_svc_printf(CDSCP_CMD_DFL_MSG, cbp->func);
	    command_status = DNS_SUCCESS;
	    break;
	}				/* end of switch on func */

    if ( command_status != DNS_SUCCESS ) {

	/* Report errors encountered during execution of the command,	*/
	/* then free the error reports. 				*/

	emit_error_report (get_err_blk_ptr(), obp);
	delete_all_err_list_elements (get_err_blk_ptr());
	}
    *scp = command_status;              /* let caller know if cmd worked */
    return (completion_code);		/* TRUE if EXIT cmd, else FALSE */
}
