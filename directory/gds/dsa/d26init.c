/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26init.c,v $
 * Revision 1.1.10.2  1996/02/18  19:45:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:43  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:45:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:07  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:45:34  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:32:32  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:52:21  marrek
 * 	More bug fixes for April 1994 submission.
 * 	[1994/05/05  12:48:56  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/04  14:26:36  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:22:18  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:09:25  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:21:13  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:06:00  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:13:55  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  14:14:21  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:11:27  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:40:53  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:19:53  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:45:32  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:12:33  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:49:49  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:56:07  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26init.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:45:27 $";
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
/*                                                                    */
/* NAME         : d26init.c       [init]                              */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 24.09.90                                            */
/*                                                                    */
/* COMPONENT    : DIR-X, DSA, initialization                          */
/*                                                                    */
/* DESCRIPTION  :   The module contains the initialization  function  */
/*                  of the DSA.                                       */
/*                                                                    */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

/*****  external Includes    *****/

#include <dce/d27util.h>
#include <gds_sec.h>
#include <D00_gds_sec_protect.h>

/*****  internal Includes    *****/

#include <d26schema.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define D26_LEN_INPUT_STRING    20  /* maximum length of an input     */
				    /* string from configuration file */

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/

static void     d26_u29_read_configuration_data(void);
static signed16 d26_u77_read_routing_dsas(FILE *fp);
static signed16 d26_u78_convert_dist_name(D2_name_string to,
    D2_name_string from);


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_u09_init_dsa                                    */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 28.01.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function does everything, what DSA needs at     */
/*                beginning.                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       Short           No error occurred             */
/*         -1                           System error occurred         */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u09_init_dsa(
    signed16 dir_id)    /* IN    -  directory ID                      */

{                /*  d26_u09_init_dsa                                 */

  static boolean dsa_initiated = FALSE; /* indicates whether dsa is   */
					/* initiated already          */

  static boolean credentials_acquired = FALSE; /* indicates whether   */
			  /* security cred. have been acuired already */
			    /* structure for sigaction call           */
  static struct sigaction siginfo = {0};
  static char *mode = "r+"; /* open mode for dsaname file             */
			    /* parameter for fcntl                    */
  static struct flock lk_struct = { F_WRLCK, SEEK_SET, 0L, 0L, 0 };

				/* return value                       */
  signed16    ret_value = D2_NO_ERR;
				/* name of read system files          */
  char        filename[D27_LFILE_NAME];
  FILE        *fp;              /* Filepointer to DSA name  file      */
  int         lock = -1;        /* Filedescriptor of locked File      */
  int         l_errno;          /* local error number                 */
  D26_dninfo  dninfo;           /* information about scheme object    */

				/* process states :                   */
  char        pstate[3];        /* W3: wait for dsaname file          */
				/* W4: read dsaname and schema        */
				/* R1: ready for running              */

				/* parameter block for d23_set_pstate  */
  D23_pstatpb sp_pb = {{D23_V02,0,0,0},D23_UNUSED,0};

  int         data_version = 0; /* data version number                */

  signed32    auth_mech;	/* authentication mechanism	      */
  D2_sec_status sec_status;     /* status for security function       */

  if            /*  dsa is not initiated                              */
    (dsa_initiated == FALSE)
  {             /*  check data versions                               */
    sprintf(filename,"%s%s",d26_install,D27_DSA_D_VERS);
    if          /*  data versions do not match                        */
      (d26_u17_check_data_version(filename,&data_version)
       != D2_NO_ERR)
    {           /*  return error                                      */
      return(D26_ERROR);
    }
    else
    {           /*  set process state waiting (request for DSA-name)  */
      sprintf(pstate, "%c3", D23_WSTATE);
      sp_pb.d23_Ppstate = pstate;
      d23_set_pstate (&sp_pb);
      sprintf(filename,"%s%s",d26_install,D27_DSANAME);
      sigemptyset(&siginfo.sa_mask);
      siginfo.sa_handler = d26_m11_distcmd;
      if        /* signal routine cannot be activated                 */
	(D27_SIGACTION(svc_c_sev_fatal,SIGUSR1,&siginfo,
	 (struct sigaction *)NULL) < 0)
      {         /* return error                                       */
	return(D26_ERROR);
      }
      while     /*  error from opening DSA name file                  */
	   ((fp = fopen(filename,mode)) == NULL)
      {         /*  check error code                                  */
	switch(errno)
	{
	  case ENOENT:
	    sleep(D27_GT_DELAY);
#ifdef THREADSAFE
	    pthread_yield();
#endif
	  case EINTR :  /* never with threads                         */
	    d26_m11_distcmd(SIGUSR1);
	    continue;
	  default:
	    l_errno = errno;
	    dce_svc_printf(DCE_SVC(gds_svc_handle,"%s%s%d"),GDS_S_GENERAL,
	      svc_c_sev_fatal,GDS_S_FOPEN_ERR,filename,mode,l_errno);
	    return(D26_ERROR);
	}
      }
      siginfo.sa_handler = SIG_IGN;
      if        /* signal routine cannot be activated                 */
	(D27_SIGACTION(svc_c_sev_fatal,SIGUSR1,&siginfo,
	 (struct sigaction *)NULL) < 0)
      {         /* return error                                       */
	return(D26_ERROR);
      }
      sprintf(pstate,"%c4",D23_WSTATE);
      sp_pb.d23_Ppstate = pstate;
      d23_set_pstate (&sp_pb);
      lock = fileno(fp);
      lk_struct.l_type = F_WRLCK ;
      if        /*  error from locking file                           */
	(fcntl(lock,F_SETLKW,&lk_struct) == (-1))
      {         /*  set return code                                   */
	l_errno = errno;
	dce_svc_printf(DCE_SVC(gds_svc_handle,"%d%d"),GDS_S_GENERAL,
	  svc_c_sev_fatal,GDS_S_FCNTL_ERR,F_SETLKW,l_errno);
	ret_value = D26_ERROR;
      }
      else
      {         /*  get all global data                               */
	if      /*  error from reading DSA name                       */
	  (fscanf(fp, "%[^\n]", (char *)d26_myexact_name) != 1)
	{       /*  set return code                                   */
	  l_errno = errno;
	  dce_svc_printf(DCE_SVC(gds_svc_handle,"%s%s%d"),GDS_S_GENERAL,
	    svc_c_sev_fatal,GDS_S_FREAD_ERR,l_errno);
	  ret_value = D26_ERROR;
	}
	else if /* for DCE authentication Principal name has to be    */
		/* there too                                          */
	       (d26_auth_mech[D2_DCE_AUTH])
	{
	  if
	   (fscanf(fp,"\n%[^\n]",(char *)d26_mypr_name) != 1)
	  {     /*  set return code                                   */
	    dce_svc_printf(DCE_SVC(gds_svc_handle,"%s%s%d"),GDS_S_GENERAL,
	      svc_c_sev_fatal,GDS_S_FREAD_ERR,errno);
	    ret_value = D26_ERROR;
	  }
	}
	if /*  error from load scheme                            */
	       (ret_value == D26_ERROR || d26_mi1_call_read_scheme())
	{       /*  set return value                                  */
	  ret_value = D26_ERROR;
	}
	else if /*  error from open files                             */
	       (d26_mi3_open_files())
	{       /*  set ret_value                                     */
	  ret_value = D26_ERROR;
	}
	else if /*  error from reading attribute table for ASN1-coding*/
	       (d27_307_init_att(d26_install) != D2_NOERROR)
	{       /*  set return code                                   */
	  ret_value = D26_ERROR;
	}
	else
	{       /*  check schema object name                          */
	  dninfo.d26_rd_anz = 0;
	  strcpy((char *)d26_schema_name,D2_NAME_SCHEME);
	  d27_007_norm_name((char *)d26_schema_name,
	    (signed16)strlen((char *)d26_schema_name),
	    (char *)d26_schema_name);
	  if    /*  error from getting scheme name                    */
	    (d26_u06_get_name(d26_schema_name,&dninfo) != D2_NO_ERR)
	  {     /*  set return value                                  */
	    dce_svc_printf(GDS_S_ILL_SCHEMA_NAME_MSG,d26_schema_name);
	    ret_value = D26_ERROR;
	  }
	  else
	  {     /*  set DSA initialized flag                          */
	    dsa_initiated = TRUE;
	  }
	}
	d26_u29_read_configuration_data();
	lk_struct.l_type = F_UNLCK ;
	if      /*  dsaname file cannot be unlocked                   */
	  (fcntl(lock,F_SETLKW,&lk_struct) == (-1))
	{       /*  set return code                                   */
	  l_errno = errno;
	  dce_svc_printf(DCE_SVC(gds_svc_handle,"%d%d"),GDS_S_GENERAL,
	    svc_c_sev_fatal,GDS_S_FCNTL_ERR,F_SETLKW,l_errno);
	  ret_value = D26_ERROR;
	}
      }
      if        /*  error from closing DSA name file                  */
	(D27_FCLOSE(svc_c_sev_fatal,fp))
      {         /*  set return code                                   */
	ret_value = D26_ERROR;
      }
    }
    if          /*  error occurred                                    */
      (ret_value != D2_NO_ERR)
    {           /*  return error                                      */
      return(ret_value);
    }
    else
    {           /*  norm own DSA-name                                 */
      d27_007_norm_name((char *)d26_myexact_name,
	(signed16)strlen((char *)d26_myexact_name),(char *)d26_myname);
      sprintf(pstate,"%c1", D23_RSTATE);
      sp_pb.d23_Ppstate = pstate;
      d23_set_pstate(&sp_pb);
    }
  }

  if            /*  error from reading distinguished name list        */
    (d26_i01_upd_core_dnlist() == D26_ERROR)
  {             /*  return with error                                 */
    return(D26_ERROR);
  }


/* acquire credentials for every supported security mechanism		*/
  if (!credentials_acquired)
  {
#ifdef THREADSAFE
    if (D00_gds_sec_protect_init() != 0)
    {
      DCE_SVC_LOG((GDS_S_ALL_PURPOSE_MSG, 
	    "protection initialisation for gds_sec failed"));
    }
#endif

    for (auth_mech = 0; auth_mech < D2_MAX_AUTH_MECHS; auth_mech++)
    {
      if (d26_auth_mech[auth_mech])
      {
	if (dsa_acquire_cred(auth_mech,d26_myname,&sec_status) == D2_ERROR)
	{
	  dce_svc_printf(GDS_S_ACQU_CRED_ERR_MSG,auth_mech,
	    sec_status.logstring);
	  return (D26_ERROR);
	}
      }
    }
    credentials_acquired = TRUE;
  }

  d26_ftime = time(0);

  return(D2_NO_ERR);

}               /*  d26_u09_init_dsa                                  */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_u29_read_configuration_data                     */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 19.10.93                                            */
/*                                                                    */
/* DESCRIPTION  : The Function reads in the configuration data of the */
/*                DSA from the "conf" file into global variables      */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_u29_read_configuration_data(void)

{               /*  d26_u29_read_configuration_data                   */

				/* name of configuration file         */
  char filename[D27_LFILE_NAME];
  FILE *fp;                     /* Filepointer of configuration file  */
  int  var_count;               /* number of actually read variables  */
				/* memory for input from configuration*/
				/* file                               */
  char input_string[D26_LEN_INPUT_STRING];

  sprintf(filename,"%s/dsaconf",d26_install);
  if            /*  configuration file can be opened                  */
    ((fp = D27_FOPEN(svc_c_route_nolog,filename,"r")) != NULL)
  {             /*  read in data                                      */
    while       /*  declarator is following                           */
	 (fscanf(fp,"%s",input_string) > 0)
    {           /*  check the input string                            */
      if        /*  routing information is declared                   */
	(!strcmp(input_string,"ROUTING_INFO"))
      {         /*  read in routing information                       */
	while   /*  specificator is following                         */
	     (fscanf(fp,"%s",input_string) > 0)
	{       /*  check the input string                            */
	  if    /*  "routing" is specified                            */
	    (!strcmp(input_string,"ROUTING"))
	  {     /*  set routing indicator                             */
	    d26_routing = TRUE;
	  }
	  else if   /* "KNOWLEDGE" is specified                       */
		 (!strcmp(input_string,"KNOWLEDGE"))
	  {     /*  check whether duplicate specification             */
	    if  /*  knowledge priority set already                    */
	      (d26_knowledge_prio > 0)
	    {   /*  log error                                         */
	      dce_svc_printf(GDS_S_BAD_DSACONF_MSG,input_string);
	      break;
	    }
	    else
	    {   /*  set knowledge priority                            */
	      d26_knowledge_prio = 1;
	      if/*  routing priority set already                      */
		(d26_routing_prio > 0)
	      { /*  increment it                                      */
		d26_routing_prio++;
	      }
	    }
	  }
	  else if   /* "ROUTING_DSA" is specified                     */
		 (!strcmp(input_string,"ROUTING_DSAS"))
	  {     /*  check whether duplicate specification             */
	    if  /*  routing priority set already                      */
	      (d26_routing_prio > 0)
	    {   /*  log error                                         */
	      dce_svc_printf(GDS_S_BAD_DSACONF_MSG,input_string);
	      break;
	    }
	    else
	    {   /*  set routing priority                              */
	      d26_routing_prio = 1;
	      if/*  knowledge priority set already                    */
		(d26_knowledge_prio > 0)
	      { /*  increment it                                      */
		d26_knowledge_prio++;
	      }
	      if/*  routing DSAs cannot be read                       */
		(d26_u77_read_routing_dsas(fp) != D2_NO_ERR)
	      { /*  break loop                                        */
		break;
	      }
	    }
	  }
	  else
	  {     /*  no routing specification                          */
	    break;
	  }
	}
      }
      if        /*  chaining policy is declared                       */
	(!strcmp(input_string,"CHAINING_DSA"))
      {         /*  read in chaining policy                           */
	if      /*  chaining policy cannot be read in                 */
	  ((var_count = fscanf(fp,"%s",input_string)) < 1)
	{       /*  log error                                         */
	  D27_FPCHECK(svc_c_sev_warning,fp);
	  dce_svc_printf(DCE_SVC(gds_svc_handle,"%d%d"),GDS_S_GENERAL,
	    svc_c_sev_warning,GDS_S_INSUFF_INFO,1,var_count);
	  break;
	}
	else if /*  chaining policy is set to SINGLE_HOP              */
	       (!strcmp(input_string,"SINGLE_HOP"))
	{       /*  set chaining policy indicator                     */
	  d26_chaining_policy = D26_SINGLE_HOP;
	}
	else if /*  chaining policy is set to MULTI_HOP               */
	       (!strcmp(input_string,"MULTI_HOP"))
	{       /*  set chaining policy indicator                     */
	  d26_chaining_policy = D26_MULTI_HOP;
	}
	else
	{       /*  log error                                         */
	  dce_svc_printf(GDS_S_BAD_DSACONF_MSG,input_string);
	}
	continue;
      }
      else if   /*  dsp unbind delay is declared                      */
	     (!strcmp(input_string,"DSP_UNBIND_DELAY"))
      {         /*  read in dsp unbind delay                          */
	if      /*  chaining policy cannot be read in                 */
	  ((var_count = fscanf(fp,"%ld",&d26_dspunbind_delay)) < 1)
	{       /*  log error                                         */
	  D27_FPCHECK(svc_c_sev_warning,fp);
	  dce_svc_printf(DCE_SVC(gds_svc_handle,"%d%d"),GDS_S_GENERAL,
	    svc_c_sev_warning,GDS_S_INSUFF_INFO,1,var_count);
	  break;
	}
	else
	{       /*  convert DSP unbind delay in seconds               */
	  d26_dspunbind_delay *= 60L;
	}
      }
      else if   /*  end of file is reached                            */
	     (feof(fp) != 0)
      {         /*  break loop                                        */
	break;
      }
      else
      {         /*  log error                                         */
	dce_svc_printf(GDS_S_BAD_DSACONF_MSG,input_string);
	break;
      }
    }
    D27_FCLOSE(svc_c_route_nolog,fp);
  }

  if            /*  no routing priority given                         */
    (d26_knowledge_prio == 0 && d26_routing_prio == 0)
  {             /*  set knowledge priority by default                 */
    d26_knowledge_prio = 1;
  }

  DCE_SVC_LOG((GDS_S_DSACONF_MSG,d26_chaining_policy,d26_routing,
    d26_knowledge_prio,d26_routing_prio,d26_dspunbind_delay));

}               /*  d26_u29_read_configuration_data                   */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_u77_read_routing_dsas                           */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 19.10.93                                            */
/*                                                                    */
/* DESCRIPTION  : The Function reads in the routing dsa names from    */
/*                the configuration file                              */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      fp        FILE *        File pointer of configuration file    */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D26_ERROR   Short       error occurred                        */
/*      D2_NO_ERR               no error occurred                     */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_u77_read_routing_dsas(FILE *fp)

{               /*  d26_u77_read_routing_dsas                         */

				/* memory for input from configuration*/
				/* file                               */
  char        input_string[D2_DNL_MAX];
  D26_dninfo  dninfo;           /* information about routing dsa      */
  int         var_count;        /* number of actually read variables  */

  register    signed16 i;       /* loop variable                      */
  register    D26_acc_point *acp; /* pointer to access point          */

if              /*  number of routing DSAs cannot be read             */
  ((var_count = fscanf(fp,"%hd\n",&d26_nr_routing_dsas)) < 1)
{               /*  log error                                         */
  D27_FPCHECK(svc_c_sev_warning,fp);
  dce_svc_printf(DCE_SVC(gds_svc_handle,"%d%d"),GDS_S_GENERAL,
    svc_c_sev_warning,GDS_S_INSUFF_INFO,1,var_count);
  return(D26_ERROR);
}
else if         /*  no memory for routing access points               */
       ((d26_routing_access_points = (D26_acc_point *)D27_CALLOC(
	svc_c_sev_warning,d26_nr_routing_dsas,sizeof(D26_acc_point)))
	== NULL)
{               /* return error                                       */
  return(D26_ERROR);
}
else
{               /*  read in DSA names                                 */
  for           /*  all DSA-names                                     */
     (i = 0, acp = d26_routing_access_points; i < d26_nr_routing_dsas;
      i++, acp++)
  {             /*  read in the DSA-name                              */
    dninfo.d26_rd_anz = 0;
    if          /*  DSA-name cannot be read in                        */
      (D27_FGETS(svc_c_route_nolog,input_string,D2_DNL_MAX,fp) == NULL)
    {           /*  log error and return                              */
      d26_nr_routing_dsas = i;
      return(D26_ERROR);
    }
    else if     /*  no memory available for DSA-name                  */
	   ((acp->d26_dsa_ename = (D2_name_string)D27_MALLOC(
	    svc_c_sev_warning,strlen(input_string) + 1)) == NULL ||
	    (acp->d26_dsaname   = (D2_name_string)D27_MALLOC(
	    svc_c_sev_warning,strlen(input_string) + 1)) == NULL)
    {           /*  log error                                         */
      d26_nr_routing_dsas = i;
      return(D26_ERROR);
    }
    else if     /*  DSA name cannot be converted                      */
	   (d26_u78_convert_dist_name(acp->d26_dsa_ename,
	    (D2_name_string)input_string) < 0)
    {           /*  log error                                         */
      dce_svc_printf(GDS_S_BAD_DSACONF_MSG,input_string);
      d26_nr_routing_dsas = i;
      return(D26_ERROR);
    }
    else if     /*  wrong syntax of routing DSA name                  */
	   (d26_u06_get_name(acp->d26_dsa_ename,&dninfo) != D2_NO_ERR)
    {           /*  log error                                         */
      dce_svc_printf(GDS_S_BAD_DSACONF_MSG,input_string);
      d26_nr_routing_dsas = i;
      return(D26_ERROR);
    }
    else
    {           /*  norm DSA name                                     */
      d27_007_norm_name((char *)acp->d26_dsa_ename,
	strlen((char *)acp->d26_dsa_ename) + 1,(char *)acp->d26_dsaname);
    }           
  }             
}               

return(D2_NO_ERR);

}               /*  d26_u77_read_routing_dsas                         */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_u78_convert_dist_name                           */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 19.10.93                                            */
/*                                                                    */
/* DESCRIPTION  : The Function converts distinguished name from       */
/*                configuration file format to internal format        */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      from      D2_name_string    name to be converted              */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      to        D2_name_string    converted name                    */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D26_ERROR   Short           name could not be converted       */
/*      D2_NO_ERR                   no error occurred                 */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_u78_convert_dist_name(
    D2_name_string to,
    D2_name_string from)

{               /*  d26_u78_convert_dist_name                         */

			    /* object identifier in hex format        */
static byte      hex_ob_id[D2_OBJ_IDL_MAX];
static D2_a_type a_type = {0,hex_ob_id};

char     *pr_ob_id;         /* object identifier in printable format  */
signed16 at_idx;            /*AT index of attribute type              */

pr_ob_id = (char *)to;

from += strspn((char *)from," \t");

while           /*  end of string not reached                         */
     (*from != D2_EOS && *from != '\n')
{               /*  check character                                   */
  switch(*from)
  {
    case '/':
      *to = D2_RDN_SEP;
      pr_ob_id = (char *)to + 1;
      break;
    case ',':
      *to = D2_AVA_SEP;
      pr_ob_id = (char *)to + 1;
      break;
    case '=':
      *to = D2_EOS;
      if                /*  object identifier cannot be converted     */
	(d27_302_str_oid(pr_ob_id,&a_type) < 0)
      {                 /*  return error                              */
	return(D26_ERROR);
      }
      else if           /*  attribute type is not in AT               */
	     ((at_idx = d26_u03_attr_search(&a_type)) < 0)
      {                 /*  return error                              */
	return(D26_ERROR);
      }
      else
      {                 /*  check representation                      */
	switch(d26_at[at_idx].d26a_rep)
	{
	  case  D2_T61_PR:
	    *to = D2_T61_REP;
	    break;
	  case  D2_PRINTABLE:
	    *to = D2_PRTBL_REP;
	    break;
	  case  D2_NUMERIC:
	    *to = D2_NUM_REP;
	    break;
	  case  D2_IA5:
	    *to = D2_IA5_REP;
	    break;
	}   
      }                 
      break;
    default:
      *to = *from;
      break;
  }   
  from++;
  to++;
}               

*to = D2_EOS;

}               /*  d26_u78_convert_dist_name                         */
