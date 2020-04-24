/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_clerk.c,v $
 * Revision 1.1.11.2  1996/02/18  19:30:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:09  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:12:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:21  root]
 * 
 * Revision 1.1.8.5  1994/08/03  19:01:34  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:52:41  mccann]
 * 
 * Revision 1.1.8.4  1994/06/09  18:38:00  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:07  devsrc]
 * 
 * Revision 1.1.8.3  1994/06/08  20:17:54  peckham
 * 	Initialize serviceability and message catalogs.
 * 	[1994/05/31  18:56:23  peckham]
 * 
 * Revision 1.1.8.2  1994/05/12  21:11:11  peckham
 * 	Change _DNS_DEFAULT_STACK_ reference to _CDS_DEFAULT_STACK_
 * 	[1994/05/12  19:14:45  peckham]
 * 
 * Revision 1.1.8.1  1994/03/12  22:01:32  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:08:35  peckham]
 * 
 * Revision 1.1.6.4  1992/12/30  13:27:02  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:43:32  zeliff]
 * 
 * Revision 1.1.6.3  1992/12/15  15:54:51  tom
 * 	Bug 6409 - add setlocale call for i18n.
 * 	[1992/12/15  15:17:13  tom]
 * 
 * Revision 1.1.6.2  1992/09/29  19:12:20  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:12:01  weisman]
 * 
 * Revision 1.1.2.2  1992/03/22  21:56:14  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:24:02  weisman]
 * 
 * Revision 1.1  1992/01/19  15:24:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * gda_clerk.c
 *
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
/*
 * Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA USA
 * All Rights Reserved
 */

#include <gda.h>
#include <dce/assert.h>
#include <dce/cds_basic_types.h>
#include <dce/rpc.h>
#include <tower.h>
#include <dce/cds_clerkserver.h>
#include <dce/rpc_cvt.h>
#include <locale.h>

/*
 * prototypes..
 */

static void
reader (
	pthread_addr_t);

int 
main (int,
      char **);

static void 
dump_rp (
	 cds_ReplicaPointer_t *);

static int 
read_input (char  *,
            int   *,
            char  ***);

static int 
get_handle (char                  *,
            rpc_binding_handle_t  *);


char 	input_file[80];
int 	num_iterations = 1;
int 	forever = 0;

extern rpc_if_handle_t cds_clerkserver_v1_0_c_ifspec;


static void 
dump_rp (cds_ReplicaPointer_t *rp_p)
{
  unsigned char string[100];  
  char *binding_p = (char *)string;
  unsigned char *s;
  dns_opq_fname Iname;
  int len,cnt;
  int status;
  error_status_t uuid_status;
  unsigned char *tower_p;

  if ((rp_p->rp_type < 1) || (rp_p->rp_type > 3))
    printf("***INVALID REPLICA TYPE \n");

  /* Print Clearinghouse name */

  R_to_L_FullName(&rp_p->rp_chname, (FullName_t *)&Iname, 1);
  len = sizeof(string);
  status = dnsCvtCDSFullToStr(&Iname, string, &len);
  if (status != DNS_SUCCESS)
    printf("***INVALID CLEARINGHOUSE NAME\n ");
  else
    printf("    Clearinghouse name: %s \n", string);
  uuid_to_string((uuid_t *) &rp_p->rp_chname.fn_root, &s, &uuid_status);
  if (uuid_status)
	printf("***INVALID NS UUID\n ");
  else {
    printf("    NS UUID: %s\n",s);
    free((void *)s);
  }


  /* Print clearinghouse uuid */
  uuid_to_string((uuid_t *)&rp_p->rp_chid, &s, &uuid_status);
  if (uuid_status)
	printf("***INVALID CLEARINGHOUSE UUID\n ");
  else {
    printf("    Clearinghouse UUID: %s\n",s);
    free((void *)s);
  }

  printf("    Towers:\n");
  tower_p = rp_p->rp_towers;
  cnt = GETL16(tower_p);	/* number of towers */
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
  printf("       %d towers\n", cnt);
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
  for (/* cnt (as assigned above) */ ; cnt > 0; --cnt) 
  {
    len = GETL16(tower_p);      /* size of the tower */
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
    printf("       Tower[%d]: length=%d\n", cnt, len);
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
    if ( get_binding_string ( tower_p, binding_p ))
    {
	printf("        %s\n", string );
    }
    else 
    {
	/*
	 * if no protocol binding, dump hex data 
	 */
	for (/* len (as assigned above) */; len > 1; --len )
	{
	    printf("%02x-", *tower_p++);
	}
	
	printf("%02x\n", *tower_p++);	/* Last value without - */
	len = 0;			/* We're pointing to next tower */
    }
    tower_p += len;
  }
}


/* 
 * Send GDA a readattribute request and display results. 
 */

int 
main (int   argc,
      char  **argv)
{
  extern char	*optarg;	/* argument parameter */
  pthread_t main_thread;
  pthread_addr_t status;
  pthread_attr_t pattr;
  char svcname[128];
  char *myname_p;
  error_status_t st;
  int		c;

  /*
   * Get our filename, stripped of directories
   */
  if (myname_p = strrchr(argv[0], '/'))
    myname_p++;
  else
    myname_p = argv[0];

  setlocale(LC_ALL, "");

  (void)sprintf(svcname, "%s(%ld)", myname_p, (long)getpid());
  dce_svc_set_progname(svcname, &st);
  dce_assert(cds__svc_handle, (st == svc_s_ok));

  dce_msg_define_msg_table(cds_msg_table,
                           sizeof(cds_msg_table) / sizeof(cds_msg_table[0]),
                           &st);
  dce_assert(cds__svc_handle, (st == msg_s_ok));

  cds_ifspec = cds_clerkserver_v1_0_c_ifspec;

  strcpy(input_file, "./ gda_clerk_input");

  while ((c = getopt(argc, argv, "i:n:f")) != EOF) 
  {
    switch(c) {

    case 'i':
      /* input file */
      strcpy(input_file, optarg);
      break;

    case 'n':
      /* number of iterations */
      num_iterations = atoi(optarg);
      break;

    case 'f':
      /* number of iterations */
      forever = 1;
      break;

    default:
      /*
       * Give usage msg & exit.
       */
      printf("usage: %s [-i input_file] [-n number_iterations] [-f]\n",
	     argv[0]);
      exit(-1);
    }
  }

  pthread_attr_create(&pattr);
  pthread_attr_setstacksize(&pattr, _CDS_DEFAULT_STACK_);
  pthread_create(&main_thread, pattr,
		 (pthread_startroutine_t)reader, status);
  pthread_join(main_thread, &status);

  printf("GDA Clerk Normal Successful Exit.\n");
  exit(0);
}

#define ASK_QUESTIONS -2
#define USE_FILE_INPUT -3
#define MAX_NAME_SIZE 80

static int 
read_input (char  *binding_string,
            int   *num_names,
            char  ***names)
{
  int lineno;
  FILE *input;
  char line[80];
  int looper;

  lineno = 0;

  if((input = fopen(input_file, "r")) == (FILE *)NULL) {
    return(ASK_QUESTIONS);
  }
  
  lineno++;

  if(fscanf(input, "%[^\n]\n", line) != 1) {
    return(lineno);
  }

  if(sscanf(line, "%s", binding_string) != 1) {
    return(lineno);
  }

  lineno++;

  if(fscanf(input, "%[^\n]\n", line) != 1) {
    return(lineno);
  }

  if(sscanf(line, "%d", num_names) != 1) {
    return(lineno);
  }

  if((*names = (char **)malloc(sizeof(char *) * *num_names)) == (char **)NULL) {
    return(-1);
  }

  for(looper = 0; looper < *num_names; looper++) {

    if(((*names)[looper] = malloc(MAX_NAME_SIZE)) == (char *)NULL) {
      return(-1);
    }

    lineno++;

    if(fscanf(input, "%[^\n]\n", line) != 1) {
      return(lineno);
    }

    if(sscanf(line, "%s", (*names)[looper]) != 1) {
      return(lineno);
    }

    lineno++;
  }

  return(USE_FILE_INPUT);
}


static void 
reader (pthread_addr_t arg)
{
  static unsigned char attribute[] = {"\1\7dns$cts"};
  unsigned char name[100];
  unsigned char binding[100];
  int status;
  int len,i;
  cds_SetMember_t *member_p;
  cds_Timestamp_t context;
  cds_Name_t attname;
  cds_Progress_t Progress;
  error_status_t commStatus;
  cds_status_t  dnsStatus;
  cds_RA_value_t value;
  unsigned char wholeset;
  rpc_binding_handle_t  handle;
  dns_opq_fname Iname;
  char binding_string[80];
  int num_names;
  char **names;
  int looper;
  int operation_mode;


  if((operation_mode = read_input(binding_string, &num_names, &names)) >= -1) 
  {
      printf("GDA CLERK : Fatal Error At line %d on read of input file %s\n",
	     operation_mode, input_file);
      exit (1);
  }

  for(looper = 0; looper < num_iterations || forever; looper++) {
      /*
       * Get binding 
       */
      if(operation_mode == ASK_QUESTIONS) {
	  for (;;) {
	    printf("String Binding: ");
	    gets((char *)binding);
	    rpc_binding_from_string_binding(binding, &handle, &rpc_status);
	    if (rpc_status == rpc_s_ok) break;
	    printf("Invalid Binding %d\n", status);
	  }
      } else {
	rpc_binding_from_string_binding((unsigned char *)binding_string,
					&handle, &rpc_status);
	if (rpc_status != rpc_s_ok) {
	  printf("Bad binding, %s\n", binding_string);
	  exit(-1);
	}
      }

    /* Initialize progress record */
    Progress.pr_flags = 1;
    Progress.pr_timeout = (cds_Timeout_t *)NULL;
    Progress.pr_replicas_p = (cds_Set_t *)NULL;

    /* Set resolved name to null */
    memset(&Progress.pr_resolved, 0, sizeof(Progress.pr_resolved));
    Progress.pr_resolved.fn_length = 2;

    /* Get unresolved name */
    if(operation_mode == ASK_QUESTIONS) {
      for (;;) {
	printf("Name: ");
	gets((char *)name);
	len = sizeof(Iname);
	status=dnsCvtCDSFullToOpq(name, &Iname, &len);
	if (status == DNS_SUCCESS) break;
	printf("Invalid Name %d\n", status);
      }
    } else {
      len = sizeof(Iname);
      status=dnsCvtCDSFullToOpq((unsigned char *)names[looper%num_names],
				&Iname, &len);
      if (status != DNS_SUCCESS) 
      {
	  printf("GDA Clerk : Invalid Name %1$s, status = %2$d\n",
		 names[looper%num_names], 
		 status);
	  exit(-1);
      }
    }

    L_to_R_FullName((FullName_t *)&Iname, &Progress.pr_unresolved);


    Progress.pr_resolved.fn_length = 0;
    Progress.pr_replicas_p = (cds_Set_t *)NULL;

    attname.nm_length = sizeof(attribute)-1;
    memcpy(attname.nm_name, attribute, sizeof(attribute));

    memset(&context, 0, sizeof(context));
    value.returningToClerk = RA_none;

    commStatus = cds_ReadAttribute(handle, &Progress, 2,
				   &attname, &context, 1024,  0,
				   &value, &wholeset, &dnsStatus);

    if (commStatus == rpc_s_ok) {
      if (dnsStatus.er_status == PDU_Success
	  || dnsStatus.er_status == PDU_UnknownEntry
	  || dnsStatus.er_status == PDU_UnderSpecifiedName) {
	/* Print out returned Progress record */
	printf("Flags: ");
	if (Progress.pr_flags & 1)
	    printf("done ");
	if (Progress.pr_flags & 2)
	    printf("up ");
	if (Progress.pr_flags & 4)
	    printf("linked ");
	if (Progress.pr_flags & 8)
	    printf("hitlink ");
	if (Progress.pr_flags & 16)
	    printf("ignorestate");
      
	/* avoid dns* */
	Progress.pr_resolved.fn_name[Progress.pr_resolved.fn_length] = '\0';

	printf("\nResolvedName: %s\n", Progress.pr_resolved.fn_name);

	/* avoid dns* */
	Progress.pr_unresolved.fn_name[Progress.pr_unresolved.fn_length] = '\0';

	printf("UnResolvedName: %s\n", Progress.pr_unresolved.fn_name);

#ifdef SNI_SVR4
	{
	char* uuidstr;
	long status;
	/* The uuid field of the unresolved name is supposed to be the actual NS UUID */
	uuid_to_string(&Progress.pr_unresolved.fn_root, &uuidstr, &status);
	if (status)
		printf("INVALID unresolved name uuid\n");
	else
		printf("Unresolved name uuid: %s\n", uuidstr);
	}
#endif /* SNI_SVR4 */

	if (Progress.pr_replicas_p != (cds_Set_t *)NULL) {
	  if (Progress.pr_replicas_p->set_type != 3)
	      printf("Unexpected set_type %d\n", Progress.pr_replicas_p->set_type);
	  /* display each replica */
	  printf("Replicas: \n");
	  for (i=0; i<Progress.pr_replicas_p->set_length; ++i) {
	    member_p = &Progress.pr_replicas_p->set_members[i];
	    if (member_p->sm_flag != 1)
		printf("Bad set member flag for member %d\n", i);
	    if (memcmp(&member_p->sm_ts, &context, sizeof(context)) !=0)
		printf("Non-null member timestamp for member %d\n",i);
	    if (member_p->sm_value.av_valuetype != VT_ReplicaPointer)
		printf("Bad value type(%d) for member %d\n", 
		       member_p->sm_value.av_valuetype, i);
	    else {
	      dump_rp(member_p->sm_value.av_val.av_rp_p);
	    }
	  }
	} else
	    printf("No replica set returned\n");
      }
      if(dnsStatus.er_status != 0)
#ifdef SNI_SVR4
	{
	int convert_status;
	unsigned char buffer[132];

	printf("CDS error Status %d\n", dnsStatus.er_status);
	dce_error_inq_text(dnsStatus.er_status, buffer, &convert_status);
	if (convert_status == 0)
		printf("\"%s\"\n", buffer);
	else
		printf("Conversion error: \"%s\"\n", buffer);
	}
#else
	printf("CDS error Status %d\n", dnsStatus.er_status);
#endif /* SNI_SVR4 */
    } else
#ifdef SNI_SVR4
      {
      int convert_status;
      unsigned char buffer[132];

      printf("Rpc error Status %d:\n", commStatus);
      dce_error_inq_text(commStatus, buffer, &convert_status);
      if (convert_status == 0)
	      printf("\"%s\"\n", buffer);
      else
	      printf("Conversion error: \"%s\"\n", buffer);
      }
#else
      printf("Rpc error Status %d\n", commStatus);
#endif /* SNI_SVR4 */
}
