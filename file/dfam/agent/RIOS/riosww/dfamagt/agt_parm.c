/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
 */
#ifdef HITACHI_REV_CHECK
static char *RCSID_agt_parm_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_parm.c,v 1.1.2.2 1996/03/09 20:44:11 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_parm.c,v $
 * Revision 1.1.2.2  1996/03/09  20:44:11  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:06  marty]
 *
 * Revision 1.1.2.1  1996/02/16  06:00:23  dce-sdc
 * 	First Submit
 * 	[1996/02/16  05:59:49  dce-sdc]
 * 
 * Revision 1.1.2.6  96/01/12  04:47:33  04:47:33  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:47:21  kishi]
 * 
 * Revision 1.1.2.5  1995/10/26  01:01:24  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:01:13  kishi]
 * 
 * Revision 1.1.2.4  1995/10/13  09:12:26  kishi
 * 	Add header comment.
 * 	[1995/10/13  09:12:14  kishi]
 * 
 * Revision 1.1.2.3  1995/09/23  09:51:57  kishi
 * 	Add header comment.
 * 	[1995/09/23  09:07:26  kishi]
 * 
 * Revision 1.1.2.2  1995/09/23  02:16:01  kishi
 * 	Replace NULL -> 0.
 * 	[1995/09/23  02:15:34  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:51:43  kishi
 * 	Initial revision
 * 	[1995/09/09  06:40:04  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netdb.h>
#include <syslog.h>


#include "agt_agent.h"
#include "agt_errid.h"
#include "agt_parm.h"
#include "agt_main.h"
#include "agt_ipc.h"
#include "agt_sck.h"
#include "agt_cmd.h"
#include "agt_trc.h"
#include "agt_msg.h"
#include "agt_wchd.h"
#include "agt_lib.h"
#include "agt_login.h"


static int agt__parm_to_opt_id(char *str);
static int agt__parm_to_cfg_opt_id(char *str);
static long agt__parm_get_parm_id(FILE *cf, char *buf);
static char *agt__parm_get_parm_opt(void);
static void agt__parm_cfg_error(long msgcode, char *optname,
				char *str, long paramid, long info);
static int agt__parm_get_logical_line(FILE *cf, char *buf);


static char *configfilename;


/**************************************************************************/
/* ID	= agt_parm_cmd_line_opt						  */
/* name = Analyze agent execution option				  */
/* func = Perform the following tasks:					  */
/*        1. analyze agent execution option				  */
/*        2. assemble address of unix domain socket			  */
/*        3. create directory path of unix domain socket		  */
/* i/o = 								  */
/*      argc  :int   :IN:number of parameters during agent execution      */
/*      argv[]:char *:IN:contents of agent execution parameters	          */
/* return = 1:execution request				 		  */
/* 	   	    2:stop request					  */
/*          3:abort request						  */	
/*          4:send message request					  */
/*          5:print trace information in standard output		  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_parm_cmd_line_opt(int argc, char *argv[])
{
    int optid;
    long i;
    long timeval;
    struct stat statbuf;
    int function = D_FUNC_STARTAGT;
    int level;

    int opt_msg = D_FALSE;
    int opt_to = D_FALSE;
    int opt_s = D_FALSE;
    int opt_a = D_FALSE;
    int opt_error = D_FALSE;
    int opt_error_full = D_FALSE;
    int opt_prmemtr = D_FALSE;

    int errmsg_done = D_FALSE;

    if (argc >= 2) {
        for (i = 1; i < argc; i++) {
            switch (optid = agt__parm_to_opt_id(argv[i])) {
	    case D_OPT_STOP:
		i++;
		if (i <argc) {
		    if (GetLong(argv[i], &timeval) == -2) {

			i--;
		    } else {
			if ((timeval < 1) || (D_CFG_31DAYS < timeval)) {
			    opt_error = D_TRUE;
			} else {
			    cmd.stoptime = timeval;
			}
		    }
	        }
		function = D_FUNC_STOPAGT;
		opt_s = D_TRUE;
		break;
	    case D_OPT_ABORT:
		function = D_FUNC_ABORTAGT;
		opt_a = D_TRUE;
		break;
	    case D_OPT_CFGFILE:
		opt_error_full = D_TRUE;
	        i++;
  	        if (i < argc) {
	            configfilename = argv[i];
	        } else {
		    opt_error = D_TRUE;
	        }
 	        break;
	    case D_OPT_DFAMDBG:
		opt_error_full = D_TRUE;
		msg.level++;
		break;
	    case D_OPT_DFAMFG:
		opt_error_full = D_TRUE;
		lmain.foreground = D_TRUE;
		break;
	    case D_OPT_MSG:
		opt_error_full = D_TRUE;
		i++;
		if (i >= argc) {
		    opt_error = D_TRUE;
	        } else {
		    cmd.mesg = argv[i];
		    function = D_FUNC_SNDMESG;
		    opt_msg = D_TRUE;
	        }
		break;
	    case D_OPT_TO:
		opt_error_full = D_TRUE;
		i++;
		if (i >= argc) {
		    opt_error = D_TRUE;
		} else {

		    if (cmd.dest == NULL) {
			cmd.dest = (unsigned long *)calloc((argc-i+1),
						sizeof(unsigned long));
		    }
		    if (cmd.dest == NULL) {
			switch (errno) {
			case ENOMEM:
			    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_20301, (argc-i+1), sizeof(unsigned long));
			    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
			    agt_exit(D_EXIT_OUTOFRSC);
			    break;
			default:
			    sprintf(msg.wkbuf, "%i,%i,%i,%i", D_EID_20302, errno, (argc-i+1), sizeof(unsigned long));
			    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
			    agt_exit(D_EXIT_UNKNOWN);
			    break;
			}
		    }
		    for (cmd.destnum = 0;
			 (i < argc) &&
			 (agt__parm_to_opt_id(argv[i]) == D_OPT_INVALID);
			 i++) {
		        if (GetIPaddr(argv[i], &(cmd.dest[cmd.destnum]))
			    == 0) {
			    cmd.destnum++;
			}
		    }
		    opt_to = D_TRUE;
	        }
		break;
	    case D_OPT_DFAMID:
		opt_error_full = D_TRUE;
		i++;
		if (i >= argc) {
		    opt_error = D_TRUE;
	        } else {
		    if ((strlen(argv[i]) == 1) && (isalnum(argv[i][0]))) {
			agt.agent_id = argv[i];
			break;
		    } else {
			fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02801, KDDS02801), "-dfaID");
			opt_error = D_TRUE;
			errmsg_done = D_TRUE;
		    }
	        }
		break;
	    case D_OPT_prmemtr:
		opt_error_full = D_TRUE;
		opt_prmemtr = D_TRUE;
		break;
	    case D_OPT_TRACEFLAG:
		opt_error_full = D_TRUE;
		i++;
		if (i < argc){
			level = atoi(argv[i]);
			if ((level >= 1) && (level <= 5))
				agt.traceflag = level;
		}
		break;
#ifdef _DEBUG
	    case D_OPT_waitsignal:
		opt_error_full = D_TRUE;
		debug.waitsignal = D_TRUE;
		break;
	    case D_OPT_loop_interval:
		opt_error_full = D_TRUE;
		i++;
		if (i <argc) {
		    if (GetLong(argv[i], (long *)&debug.loop_interval) != 0) {
			opt_error = D_TRUE;
		    }
		    if (debug.loop_interval < 0) {
			opt_error = D_TRUE;
		    }
	        }
		break;
	    case D_OPT_noexit:
		opt_error_full = D_TRUE;
		debug.noexit = D_TRUE;
		break;
#endif
	    case D_OPT_INVALID:
		opt_error = D_TRUE;
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i", D_EID_20303, optid);
		fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		agt_exit(D_EXIT_UNKNOWN);
		break;
	    }
        }
    }


    sprintf(sck.tmpl_adr.sun_path, "%s%s", D_UXSOC_PATH, agt.agent_id);
#ifdef _DEBUG
	syslog(LOG_ERR,"sck.tmpl_adr.sun_path = %s\n",sck.tmpl_adr.sun_path);
#endif


    if (stat(sck.tmpl_adr.sun_path, &statbuf) == -1) {
	switch (errno) {
	case ENOTDIR:
	case EACCES:
	    sprintf(msg.wkbuf, "%i,%i,%s", D_EID_20304, errno, sck.tmpl_adr.sun_path);
	    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    agt_exit(D_EXIT_UNKNOWN);
	    break;
	case ENOENT:
	    if (mkdir(sck.tmpl_adr.sun_path, S_IRWXU|S_IRWXG) == -1) {
		switch (errno) {
		case ENOSPC:
		    sprintf(msg.wkbuf, "%i,%s", D_EID_20305, sck.tmpl_adr.sun_path);
		    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02580, KDDS02580), "mkdir", strerror(errno), msg.wkbuf);
		    agt_exit(D_EXIT_OUTOFRSC);
		    break;
		default:
		    sprintf(msg.wkbuf, "%i,%i,%s", D_EID_20306, errno, sck.tmpl_adr.sun_path);
		    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		    agt_exit(D_EXIT_UNKNOWN);
		    break;
		}
	    }
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%s", D_EID_20307, errno, sck.tmpl_adr.sun_path);
	    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    agt_exit(D_EXIT_UNKNOWN);
	    break;
	}
    }



    strcat(sck.tmpl_adr.sun_path, "/");


    {
	struct servent *serventryp;
	char name[128];
	char *proto = "tcp";

	sprintf(name, "%s%s", D_SERVICE_ID, agt.agent_id);
	if ((serventryp = getservbyname(name, proto)) != NULL) {
	    sck.portnumber = serventryp->s_port;

	    if (sck.portnumber <= 5000) {
		fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02283, KDDS02283), sck.portnumber);
	    }
	}
    }


    if (opt_error != D_TRUE) {
        if ((opt_prmemtr == D_TRUE) &&
	    ((opt_a + opt_s + opt_msg + opt_to) == 0)) {
	    function = D_FUNC_PRMEMTR;
	    return function;
        }

        if ((opt_s + opt_a + opt_msg + opt_to) == 0) {
	    function = D_FUNC_STARTAGT;
	    return function;
        }

        if ((opt_s == D_TRUE) &&
	    ((opt_a + opt_msg + opt_to) == 0)) {
	    function = D_FUNC_STOPAGT;
	    return function;
        }

        if ((opt_a == D_TRUE) &&
	    ((opt_s + opt_msg + opt_to) == 0)) {
	    function = D_FUNC_ABORTAGT;
	    return function;
        }

        if (((opt_msg + opt_to) ==2) &&
	    ((opt_s + opt_a) == 0)) {
	    function = D_FUNC_SNDMESG;
	    return function;
        }
    }


    if (opt_error_full) {
	if (!errmsg_done) {
	    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02800, KDDS02800));
	}
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG20000, KDDS20000));
    } else {
	if (!errmsg_done) {
	    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02800, KDDS02800));
	}
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG20000, KDDS20000));
    }
    agt_exit(D_EXIT_INVPARM);
}


/**************************************************************************/
/* ID   = agt__parm_to_opt_id                                             */
/* name = Option analyzer		                                  */
/* func = Analyze parameters specified in the configuratin file		  */   
/* i/o =                                                                  */
/*	str:char *:IN:option information				  */
/* return =								  */
/*	D_CFG_INVALID:invalid option					  */
/*  D_CFG_HCI    :health check interval					  */
/*	D_CFG_HCT    :health check time out				  */
/*  D_CFG_TCPSDT :TCPShutdownTimeout				 	  */
/*	D_CFG_GW     :gateway name					  */
/*	D_CFG_AGTPORT:number of the board used by agent			  */
/*	D_CFG_IPCID  :file name used to create key for comunication       */
/*                    between processes					  */
/*	D_CFG_MEMTRSZ:memory trace size					  */
/*  D_CFG_EVTRSZ :event trace size					  */
/*	D_CFG_EVTRFL :number of files for event trace			  */
/*	D_CFG_PRTRSZ :process trace size				  */
/*	D_CFG_TCPRDT :wating timeout					  */
/*  D_CFG_TGTRT  :time to ckeck ticket validation			  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__parm_to_opt_id(char *str)
{
    long i;
    static struct {
        char *IDstr;
	long IDcode;
    } IDList[] = {
	{"-s", D_OPT_STOP},
	{"-a", D_OPT_ABORT},
	{"-ConfigFile", D_OPT_CFGFILE},
	{"-dfaDebug", D_OPT_DFAMDBG},
	{"-dfaFG", D_OPT_DFAMFG},
	{"-msg", D_OPT_MSG},
	{"-to", D_OPT_TO},
	{"-dfaID", D_OPT_DFAMID},
	{"-prmemtr", D_OPT_prmemtr},
	{"-trace", D_OPT_TRACEFLAG},
#ifdef _DEBUG
	{"-waitsignal", D_OPT_waitsignal},
	{"-loop_interval", D_OPT_loop_interval},
	{"-noexit", D_OPT_noexit},
#endif
	{"", D_OPT_INVALID}
    };


    for (i = 0; IDList[i].IDcode != D_OPT_INVALID; i++) {
	if (strcmp(IDList[i].IDstr, str) == 0) {
	    return IDList[i].IDcode;
        }
    }

    return D_OPT_INVALID;
}

/**************************************************************************/
/* ID	= agt_parm_config_file						  */
/* name = Analyzing configuration file					  */
/* fund = Open configuration file and analyzes its parameters. Then create*/
/*        directory name to be used by IPC				  */
/* io = none								  */
/* return = none							  */
/* note = Parameters not specified in the configuration file are set with */
/*        default data							  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_parm_config_file(void)
{
    FILE *cf = NULL;
    char wkbuf[D_CFG_MAXLEN+1];
    long numGW = 0;
    long lval;
    long id;
    char *opt;

    if (configfilename != NULL) {
	if ((cf = fopen(configfilename, "r")) == NULL) {
	    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02900, KDDS02900),  configfilename, strerror(errno));
	    agt_exit(D_EXIT_INVPARM);
        }
    }

    wkbuf[0] = '\0';

    if (cf != NULL) {
	while ((id = agt__parm_get_parm_id(cf, wkbuf)) != -1) {
	    if (id == D_CFG_GW) {
		for ( ; (opt = agt__parm_get_parm_opt()) != NULL; numGW++);
	    }
	    wkbuf[0] = '\0';
	}

	if (numGW > 0) {
	    sck.remoteip = (unsigned long *)calloc(numGW,
						   sizeof(unsigned long));
	    if (sck.remoteip == NULL) {
		switch (errno) {
		case ENOMEM:
		    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_20308, numGW, sizeof(unsigned long));
		    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
		    agt_exit(D_EXIT_OUTOFRSC);
		    break;
		default:
		    sprintf(msg.wkbuf, "%i,%i,%i,%i", D_EID_20309, errno, numGW, sizeof(unsigned long));
		    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		    agt_exit(D_EXIT_UNKNOWN);
		    break;
		}
	    }
	}


retry_fseek:
	if (fseek(cf, 0, SEEK_SET) == -1) {
	    switch (errno) {
	    case EINTR:
		goto retry_fseek;
	    default:
		sprintf(msg.wkbuf, "%i,%i", D_EID_20310, errno);
		fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		agt_exit(D_EXIT_UNKNOWN);
		break;
	    }
	}

	while ((id = agt__parm_get_parm_id(cf, wkbuf)) != -1) {
	    switch (id) {
	    case D_CFG_INVALID:
		fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02901, KDDS02901), wkbuf);
		agt_exit(D_EXIT_INVPARM);
		break;
	    case D_CFG_HCI:
		if ((opt = agt__parm_get_parm_opt()) == NULL) {
		    agt__parm_cfg_error(D_CFG_ERR_MISOPT, "HCInterval",
			     NULL, 0, 0);
		}
		switch (lval = GetLong(opt, (long *)&wchd.hcinterval)) {
		case 0:
		    if ((wchd.hcinterval < 15) ||
			(D_CFG_31DAYS < wchd.hcinterval)) {
			agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "HCInterval",
				 opt, 0, 0);
		    }
		    break;
		case -1:
		    agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "HCInterval",
			     opt, 0, 0);
		    break;
		case -2:
		    agt__parm_cfg_error(D_CFG_ERR_INVCHAR, "HCInterval",
			     opt, 0, 0);
		    break;
		default:
		    agt__parm_cfg_error(D_CFG_ERR_INTERNAL, "GetLong",
			     NULL, D_CFG_HCI, lval);
		    break;
		}
		break;
	    case D_CFG_HCT:
		if ((opt = agt__parm_get_parm_opt()) == NULL) {
		    agt__parm_cfg_error(D_CFG_ERR_MISOPT, "HCTimeout",
			     NULL, 0, 0);
		}
		switch (lval = GetLong(opt, (long *)&wchd.hctimeout)) {
		case 0:
		    if ((wchd.hctimeout < 15) ||
			(D_CFG_31DAYS < wchd.hctimeout)) {
		    agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "HCTimeout",
			     opt, 0, 0);
		    }
		    break;
		case -1:
		    agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "HCTimeout",
			     opt, 0, 0);
		    break;
		case -2:
		    agt__parm_cfg_error(D_CFG_ERR_INVCHAR, "HCTimeout",
			     opt, 0, 0);
		    break;
		default:
		    agt__parm_cfg_error(D_CFG_ERR_INTERNAL, "GetLong",
			     NULL, D_CFG_HCT, lval);
		    break;
		}
		break;
	    case D_CFG_TCPSDT:
		if ((opt = agt__parm_get_parm_opt()) == NULL) {
		    agt__parm_cfg_error(D_CFG_ERR_MISOPT, "TCPShutdownTimer",
			     NULL, 0, 0);
		}
		switch (lval = GetLong(opt, (long *)&sck.sdtimeout)) {
		case 0:
		    if ((sck.sdtimeout < 0) ||
			(D_CFG_31DAYS < sck.sdtimeout)) {
			agt__parm_cfg_error(D_CFG_ERR_OFRANGE,
				"TCPShutdownTimer", opt, 0, 0);
		    }
		    break;
		case -1:
		    agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "TCPShutdownTimer",
			     opt, 0, 0);
		    break;
		case -2:
		    agt__parm_cfg_error(D_CFG_ERR_INVCHAR, "TCPShutdownTimer",
			     opt, 0, 0);
		    break;
		default:
		    agt__parm_cfg_error(D_CFG_ERR_INTERNAL, "GetLong",
			     NULL, D_CFG_TCPSDT, lval);
		    break;
		}
		break;
	    case D_CFG_GW:
		for (;
		     ((opt = agt__parm_get_parm_opt()) != NULL) &&
		     (sck.numremote < numGW);) {
		    if (GetIPaddr(opt, &(sck.remoteip[sck.numremote])) == 0) {
			sck.numremote++;
		    } else {
			agt__parm_cfg_error(D_CFG_ERR_GATEWAY, "Gateways",
					    opt, 0, 0);
		    }
		}
		if (opt != NULL) {
		    sprintf(msg.wkbuf, "%i,%i", D_EID_20311, numGW);
		    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		    agt_exit(D_EXIT_UNKNOWN);
		}
		break;
	    case D_CFG_AGTPORT:
		if ((opt = agt__parm_get_parm_opt()) == NULL) {
		    agt__parm_cfg_error(D_CFG_ERR_MISOPT, "AgentPort",
			     NULL, 0, 0);
		}
		switch (lval = GetLong(opt, (long *)&sck.portnumber)) {
		case 0:
		    if ((sck.portnumber < 1) || (65535 < sck.portnumber)) {
			agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "AgentPort",
				 opt, 0, 0);
		    }
		    if (sck.portnumber <= 5000) {
			fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02283, KDDS02283), sck.portnumber);
		    }
		    break;
		case -1:
		    agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "AgentPort",
			     opt, 0, 0);
		    break;
		case -2:
		    agt__parm_cfg_error(D_CFG_ERR_INVCHAR, "AgentPort",
			     opt, 0, 0);
		    break;
		default:
		    agt__parm_cfg_error(D_CFG_ERR_INTERNAL, "AgentPort",
			     NULL, D_CFG_TCPSDT, lval);
		    break;
		}
		break;
	    case D_CFG_IPCID:
		if ((opt = agt__parm_get_parm_opt()) == NULL) {
		    agt__parm_cfg_error(D_CFG_ERR_MISOPT, "IPCId",
			     NULL, 0, 0);
		}
		agt.ipcpath = (char *)malloc(strlen(opt)+1);
		if (agt.ipcpath == NULL) {
		    switch (errno) {
		    case ENOMEM:
			sprintf(msg.wkbuf, "%i,%i", D_EID_20312, (strlen(opt)+1));
			fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
			agt_exit(D_EXIT_OUTOFRSC);
			break;
		    default:
			sprintf(msg.wkbuf, "%i,%i,%i", D_EID_20313, errno, (strlen(opt)+1));
			fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
			agt_exit(D_EXIT_UNKNOWN);
			break;
		    }
		} else {
		    sprintf(agt.ipcpath, "%s", opt);
		}
		break;
	    case D_CFG_MEMTRSZ:
		if ((opt = agt__parm_get_parm_opt()) == NULL) {
		    agt__parm_cfg_error(D_CFG_ERR_MISOPT, "MemTraceSize",
			     NULL, 0, 0);
		}
		switch (lval = GetLong(opt, (long *)&trc.memtrsize)) {
		case 0:
		    if ((trc.memtrsize < 0) ||
			(D_CFG_MEG < trc.memtrsize)) {
			agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "MemTraceSize",
				 opt, 0, 0);
		    }
		    trc.memtrsize *= D_CFG_KILO;
		    break;
		case -1:
		    agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "MemTraceSize",
			     opt, 0, 0);
		    break;
		case -2:
		    agt__parm_cfg_error(D_CFG_ERR_INVCHAR, "MemTraceSize",
			     opt, 0, 0);
		    break;
		default:
		    agt__parm_cfg_error(D_CFG_ERR_INTERNAL, "GetLong",
			     NULL, D_CFG_MEMTRSZ, lval);
		    break;
		}
		break;
	    case D_CFG_EVTRSZ:
		if ((opt = agt__parm_get_parm_opt()) == NULL) {
		    agt__parm_cfg_error(D_CFG_ERR_MISOPT, "EveTraceSize",
			     NULL, 0, 0);
		}
		switch (lval = GetLong(opt, (long *)&trc.evttrsize)) {
		case 0:
		    if ((trc.evttrsize < 32) ||
			(D_CFG_MEG < trc.evttrsize)) {
			agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "EveTraceSize",
				 opt, 0, 0);
		    }
		    trc.evttrsize *= D_CFG_KILO;
		    break;
		case -1:
		    agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "EveTraceSize",
			     opt, 0, 0);
		    break;
		case -2:
		    agt__parm_cfg_error(D_CFG_ERR_INVCHAR, "EveTraceSize",
			     opt, 0, 0);
		    break;
		default:
		    agt__parm_cfg_error(D_CFG_ERR_INTERNAL, "GetLong",
			     NULL, D_CFG_EVTRSZ, lval);
		    break;
		}
		break;
	    case D_CFG_EVTRFL:
		if ((opt = agt__parm_get_parm_opt()) == NULL) {
		    agt__parm_cfg_error(D_CFG_ERR_MISOPT, "EveTraceFiles",
			     NULL, 0, 0);
		}
		switch (lval = GetLong(opt, (long *)&trc.evttrfiles)) {
		case 0:
		    if ((trc.evttrfiles < 0) ||
			(trc.evttrfiles == 1) ||
			(D_CFG_KILO < trc.evttrfiles)) {
			agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "EveTraceFiles",
				 opt, 0, 0);
		    }
		    break;
		case -1:
		    agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "EveTraceFiles",
			     opt, 0, 0);
		    break;
		case -2:
		    agt__parm_cfg_error(D_CFG_ERR_INVCHAR, "EveTraceFiles",
			     opt, 0, 0);
		    break;
		default:
		    agt__parm_cfg_error(D_CFG_ERR_INTERNAL, "GetLong",
			     NULL, D_CFG_EVTRFL, lval);
		    break;
		}
		break;
	    case D_CFG_PRTRSZ:
		if ((opt = agt__parm_get_parm_opt()) == NULL) {
		    agt__parm_cfg_error(D_CFG_ERR_MISOPT, "ProcTraceSize",
			     NULL, 0, 0);
		}
		switch (lval = GetLong(opt, (long *)&trc.prctrsize)) {
		case 0:
		    if ((trc.prctrsize < 16) ||
			(D_CFG_MEG < trc.prctrsize)) {
			agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "ProcTraceSize",
				 opt, 0, 0);
		    }
		    trc.prctrsize *= D_CFG_KILO;
		    break;
		case -1:
		    agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "ProcTraceSize",
			     opt, 0, 0);
		    break;
		case -2:
		    agt__parm_cfg_error(D_CFG_ERR_INVCHAR, "ProcTraceSize",
			     opt, 0, 0);
		    break;
		default:
		    agt__parm_cfg_error(D_CFG_ERR_INTERNAL, "GetLong",
			     NULL, D_CFG_PRTRSZ, lval);
		    break;
		}
		break;

	    case D_CFG_TCPRDT:
		if ((opt = agt__parm_get_parm_opt()) == NULL) {
		    agt__parm_cfg_error(D_CFG_ERR_MISOPT, "TCPReadTimeout",
			     NULL, 0, 0);
		}
		switch (lval = GetLong(opt, (long *)&sck.readtimeout)) {
		case 0:
		    if ((sck.readtimeout < 0) ||
			(D_CFG_31DAYS < sck.readtimeout)) {
		    agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "TCPReadTimeout",
			     opt, 0, 0);
		    }
		    break;
		case -1:
		    agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "TCPReadTimeout",
			     opt, 0, 0);
		    break;
		case -2:
		    agt__parm_cfg_error(D_CFG_ERR_INVCHAR, "TCPReadTimeout",
			     opt, 0, 0);
		    break;
		default:
		    agt__parm_cfg_error(D_CFG_ERR_INTERNAL, "GetLong",
			     NULL, D_CFG_TCPRDT, lval);
		    break;
		}
		break;
	    case D_CFG_TGTRT:
		if ((opt = agt__parm_get_parm_opt()) == NULL) {
		    agt__parm_cfg_error(D_CFG_ERR_MISOPT, "TGTRefreshTime",
			     NULL, 0, 0);
		}
		switch (lval = GetLong(opt, (long *)&TGTRefreshTime)) {
		case 0:
		    if ((TGTRefreshTime < 10) ||
			(D_CFG_31DAYS < TGTRefreshTime)) {
		    agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "TGTRefreshTime",
			     opt, 0, 0);
		    }
		    break;
		case -1:
		    agt__parm_cfg_error(D_CFG_ERR_OFRANGE, "TGTRefreshTime",
			     opt, 0, 0);
		    break;
		case -2:
		    agt__parm_cfg_error(D_CFG_ERR_INVCHAR, "TGTRefreshTime",
			     opt, 0, 0);
		    break;
		default:
		    agt__parm_cfg_error(D_CFG_ERR_INTERNAL, "GetLong",
			     NULL, D_CFG_TGTRT, lval);
		    break;
		}
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i", D_EID_20314, id);
		fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		agt_exit(D_EXIT_UNKNOWN);
	    }

	    wkbuf[0] = '\0';
	}
    }


    if (cf != NULL) {
	fclose(cf);
    }



    if (agt.ipcpath == NULL) {
	int alloc_size;

	alloc_size = strlen(D_DFLT_IPCPATH) + 1;

	agt.ipcpath = (char *) malloc(alloc_size);
	if (agt.ipcpath == NULL) {
	    switch (errno) {
	    case ENOMEM:
		sprintf(msg.wkbuf, "%i,%i", D_EID_20315, alloc_size);
		fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
		agt_exit(D_EXIT_OUTOFRSC);
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i,%i", D_EID_20316, errno, alloc_size);
		fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		agt_exit(D_EXIT_UNKNOWN);
		break;
	    }
	} else {
	    sprintf(agt.ipcpath, "%s", D_DFLT_IPCPATH);
	}
    }


    if (strlen(agt.agent_id) > 0) {
	int alloc_size;
	char *wkp;

	alloc_size = strlen(agt.ipcpath) + strlen(agt.agent_id) + 1;


	wkp = (char *) malloc(alloc_size);
	if (wkp == NULL) {
	    switch (errno) {
	    case ENOMEM:
		sprintf(msg.wkbuf, "%i,%i", D_EID_20317, alloc_size);
		fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
		agt_exit(D_EXIT_OUTOFRSC);
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i,%i", D_EID_20318, errno, alloc_size);
		fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		agt_exit(D_EXIT_UNKNOWN);
		break;
	    }
	}
	sprintf(wkp, "%s%s", agt.ipcpath, agt.agent_id);

	free(agt.ipcpath);

	agt.ipcpath = wkp;
    }
}


/*****************************************************************************/
/* ID	= agt__parm_to_cfg_opt_id					     */
/*                                                                           */
/* name = Analysis of the configuration file option                          */
/* func = With comparing str and the configuration file option, checks if    */
/*        appropriate options are set.                                       */
/*                                                                           */
/* i/o    = str: char * :INP: configuration file option                      */
/*                                                                           */
/* return = D_CFG_HCI    : HCInterval option                                 */
/*          D_CFG_HCT    : HCTimeout option                                  */
/*          D_CFG_TCPSDT : TCPShutdownTimeout option                         */
/*          D_CFG_GW     : Gateways option                                   */
/*          D_CFG_AGTPORT: AgentPort option                                  */
/*          D_CFG_IPCID  : IPCId option                                      */
/*          D_CFG_MEMTRSZ: MemTraceSize option                               */
/*          D_CFG_EVTRSZ : EveTraceSize option                               */
/*          D_CFG_EVTRFL : EveTraceFiles option                              */
/*          D_CFG_PRTRSZ : ProcTraceSize option                              */
/*          D_CFG_TCPRDT : TCPReadTimeout option                             */
/*          D_CFG_TGTRT  : TGTRefreshTime option                             */
/*          D_CFG_INVALID: invalid option                                    */
/*                                                                           */
/* note   = none                                                             */
/* date   = October 12, 1995  by Tetsuya Ichikawa                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static int agt__parm_to_cfg_opt_id(char *str)
{
    long i;
    static struct {
        char *IDstr;
	long IDcode;
    } IDList[] = {
	{"HCInterval", D_CFG_HCI},
	{"HCTimeout", D_CFG_HCT},
	{"TCPShutdownTimeout", D_CFG_TCPSDT},
	{"Gateways", D_CFG_GW},
	{"AgentPort", D_CFG_AGTPORT},
	{"IPCId", D_CFG_IPCID},
	{"MemTraceSize", D_CFG_MEMTRSZ},
	{"EveTraceSize", D_CFG_EVTRSZ},
	{"EveTraceFiles", D_CFG_EVTRFL},
	{"ProcTraceSize", D_CFG_PRTRSZ},
	{"TCPReadTimeout", D_CFG_TCPRDT},
	{"TGTRefreshTime", D_CFG_TGTRT},
	{"", D_CFG_INVALID}
    };


    for (i = 0; IDList[i].IDcode != D_CFG_INVALID; i++) {
	if (strcmp(IDList[i].IDstr, str) == 0) {
	    return IDList[i].IDcode;
        }
    }

    return D_CFG_INVALID;
}


/**************************************************************************/
/* ID	= agt__parm_get_parm_id						  */
/* name = Get parameter identification number				  */
/* func = Get parameters one by one from configuration file		  */
/* i/o =								  */
/*	cf :FILE *:IN:pointer to configuration file structure		  */
/*  buf:char *:IN:pointer to buffer with information from configuration   */
/*	           file							  */
/* return =								  */
/*  D_CFG_INVALID:invalid option			                  */
/*  D_CFG_HCI    :health check interval time                        	  */
/*  D_CFG_HCT    :health check timeout			                  */
/*  D_CFG_TCPSDT :TCPShutdownTimeout                                 	  */
/*  D_CFG_GW     :gateway name	                                     	  */
/*	D_CFG_AGTPORT:number of the board used by agent			  */
/*	D_CFG_IPCID  :file name used to create key for comunication       */
/*                    between processes					  */
/*	D_CFG_MEMTRSZ:memory trace size					  */
/*  D_CFG_EVTRSZ :event trace size					  */
/*	D_CFG_EVTRFL :number of files for event trace			  */
/*	D_CFG_PRTRSZ :process trace size				  */
/*	D_CFG_TCPRDT :wating timeout					  */
/*  D_CFG_TGTRT  :time to ckeck ticket validation			  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static long agt__parm_get_parm_id(FILE *cf, char *buf)
{
    char *bp;

    if (buf[0] == '\0') {
	do {
	    if (agt__parm_get_logical_line(cf, buf) == 0) {
		return -1;
	    }
	} while ((bp = strtok(buf, D_CFG_FS)) == NULL);
    }

    return  agt__parm_to_cfg_opt_id(bp);
}


/**************************************************************************/
/* ID	= agt__parm_get_parm_opt					  */
/* name = Get parameter data						  */
/* func = Get data specified by the parameter				  */
/* i/o = none								  */
/* return = parameter data						  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static char *agt__parm_get_parm_opt(void)
{
    return strtok(NULL, D_CFG_FS);
}


/**************************************************************************/
/* ID	= agt__parm_cfg_error						  */
/* name = Print error message  						  */
/* func = If an invalid option is set in the configuration file, print    */
/*        the mesage and exit						  */
/* i/o =								  */
/*	msgcode:long  :IN:message code					  */
/*	optname:char *:IN:message information				  */
/*	str    :char *:IN:information to print with error message	  */
/*	paramid:long  :IN:option-ID					  */
/*	info   :long  :IN:information to print with error message	  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__parm_cfg_error(long msgcode, char *optname,
				char *str, long paramid, long info)
{
    switch (msgcode) {
    case D_CFG_ERR_MISOPT:
	 fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02902, KDDS02902), optname);
	 break;
     case D_CFG_ERR_OFRANGE:
	 fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02904, KDDS02904), optname);
	 break;
     case D_CFG_ERR_INVCHAR:
	 fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02903, KDDS02903), optname);
	 break;
     case D_CFG_ERR_GATEWAY:
	 fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02905, KDDS02905), optname, str);
	 break;
     case D_CFG_ERR_INTERNAL:
	 sprintf(msg.wkbuf, "%i,%s,%i,%i", D_EID_20319, str, paramid, info);
	 fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	 agt_exit(D_EXIT_UNKNOWN);
	 break;
     default:
	 sprintf(msg.wkbuf, "%i,%i,%s,%s,%i,%i", D_EID_20320, msgcode, str, optname, paramid, info);
	 fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	 agt_exit(D_EXIT_UNKNOWN);
	 break;
     }

    agt_exit(D_EXIT_INVPARM);
}


/**************************************************************************/
/* ID	= agt__parm_get_logical_line					  */
/* name = Read one line from the configuration file			  */
/* func = Read one line of information from configuration file skipping   */
/*        comment lines							  */
/* i/o =								  */
/*	cf :FILE *:IN :pointer to configuration file			  */
/*  buf:char *:OUT:line read from configuration file			  */
/* return = 								  */
/*	cnum:number of characters read in the line			  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__parm_get_logical_line(FILE *cf, char *buf)
{
    char c;
    long cnum;
    char *wp = buf;

    cnum = 0;
    c = fgetc(cf);
    do {
	if (c == EOF) {
	} else if (c == '#') {
	    do {
		c = fgetc(cf);
	    } while (! ((c == '\n') || (c != EOF)));
	} else if (c == '\n') {
	    if (cnum > 0) {
		if (*(wp-1) == '\\') {
		    wp--;
		    c = fgetc(cf);
		} else {
		    ;
		}
	    } else {
		c = fgetc(cf);
	    }
	} else {
	    *wp = c;
	    wp ++;
	    cnum++;
	    c = fgetc(cf);
	}
    } while (! ((cnum >= D_CFG_MAXLEN) || (c == EOF) ||
		((c == '\n') && (cnum > 0))));
    *wp = '\0';

    return ((c == EOF) && (cnum == 0)) ? 0 : cnum;
}
