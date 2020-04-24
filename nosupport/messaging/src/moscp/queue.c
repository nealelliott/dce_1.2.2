/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * HISTORY
 * $Log: queue.c,v $
 * Revision 1.25  1996/12/09  10:02:01  salamone
 * Display names for secids
 *
 * Revision 1.24  96/12/06  11:00:33  11:00:33  salamone (Julie Salamone)
 * Support ACK datatype
 * 
 * Revision 1.23  96/12/05  15:51:32  15:51:32  salamone (Julie Salamone)
 * que catalog and create code clean-up
 * 
 * Revision 1.22  96/12/05  13:43:04  13:43:04  salamone (Julie Salamone)
 * que cat code clean-up
 * 
 * Revision 1.21  96/12/05  11:04:32  11:04:32  salamone (Julie Salamone)
 * que create and modify code clean-up
 * 
 * Revision 1.20  96/12/04  15:07:17  15:07:17  salamone (Julie Salamone)
 * Fix macros
 * 
 * Revision 1.19  96/12/03  11:10:40  11:10:40  salamone (Julie Salamone)
 * Rename tcl vars for MOS
 * 
 * Revision 1.18  96/12/03  10:43:34  10:43:34  salamone (Julie Salamone)
 * Fix headers
 * 
 * Revision 1.17  96/12/02  18:30:04  18:30:04  salamone (Julie Salamone)
 * Fix queue dequeue
 * 
 * Revision 1.16  96/12/02  17:02:59  17:02:59  salamone (Julie Salamone)
 * Fix queue remove
 * 
 * Revision 1.15  96/12/02  16:20:50  16:20:50  salamone (Julie Salamone)
 * Fix queue move; remove queue rename
 * 
 * Revision 1.14  96/11/29  13:08:24  13:08:24  salamone (Julie Salamone)
 * Fix queue enqueue
 * 
 * Revision 1.13  96/11/26  17:10:15  17:10:15  salamone (Julie Salamone)
 * Fix queue modify
 * 
 * Revision 1.12  96/11/26  15:56:16  15:56:16  salamone (Julie Salamone)
 * Fix queue create
 * 
 * Revision 1.11  96/11/25  17:25:20  17:25:20  salamone (Julie Salamone)
 * Fix queue list -still stinks
 * 
 * Revision 1.10  96/11/25  14:26:23  14:26:23  salamone (Julie Salamone)
 * Fix queue show
 * 
 * Revision 1.9  96/11/23  16:05:33  16:05:33  salamone (Julie Salamone)
 * Clean-up queue delete
 * 
 * Revision 1.8  96/11/23  14:48:47  14:48:47  salamone (Julie Salamone)
 * Clean-up queue catalog
 * 
 * Revision 1.7  96/11/22  18:05:46  18:05:46  salamone (Julie Salamone)
 * Phase I clean-up
 * 
 * Revision 1.6  96/10/17  17:56:42  17:56:42  wwang (Weidong Wang)
 * added "dequeue" support
 * 
 * Revision 1.5  1996/10/16  15:48:21  wwang
 * when reporting error, detach from resource if necessary.
 *
 * Revision 1.4  1996/10/11  15:22:55  wwang
 * *** empty log message ***
 *
 * Revision 1.3  1996/10/09  17:50:16  wwang
 * *** empty log message ***
 *
 * Revision 1.2  1996/10/09  15:08:05  wwang
 * *** empty log message ***
 *
 * Revision 1.1  1996/10/07  16:40:43  wwang
 * Initial revision
 *
 * Revision 1.3  1996/10/03  19:30:15  wwang
 * added one more option "-simplename" for queue catalog. By default,
 * name returned will be fully qualified name. Also added the code to
 * set default qmgr when the dcecp variable is set.
 *
 * Revision 1.2  1996/10/02  16:10:33  wwang
 * use a new sams file for queue error codes.
 *
 * Revision 1.1  1996/10/01  17:07:12  wwang
 * Initial revision
 *
 * Revision 1.2  1996/08/28  16:29:56  wwang
 * *** empty log message ***
 *
 * $EndLog$
 * ______________________________________________________________________
 */


#include <stdio.h>
#include <locale.h>
#include <setjmp.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>                     /* for UINT_MAX */
#include <ctype.h>

#include <dce/dce.h>

#ifndef NCK
#  define NCK
#endif
#include <dce/rpc.h>
#include <dce/stubbase.h>

#include <stdlib.h>
#include <string.h>
#include <dce/dce_error.h>

#include <dce/dce_msg.h>
#include <dcemcpmsg.h>
#include <moscp.h>
#include <mos_ack.h>
#include <dcemosmsg.h>
#include <queue.h>
#include <util_var.h>
#include <util_help.h>
#include <util_binding.h>
#include <tcl.h>
#include <dce/ep.h>
#include <dce/utc.h>
#include <util_var.h>

/*
 * DECLARATIONS
 */
#define DCP_CHECK_QUE_NO_ARG(type) {                                    \
  if(argc < 2){                                                         \
     char *tmp;                                                         \
     tmp = (char *)dce_sprintf(mcp_s_que_no_ ## type ## _name),         \
     Tcl_SetResult(interp, tmp, TCL_DYNAMIC);                           \
     return TCL_ERROR;                                                  \
  }                                                                     \
}                   

#define CHECK_MOS_ERROR(st) {                                           \
  if (st != error_status_ok) {                                          \
      Tcl_SetResult(interp, (char *)dce_sprintf(st), TCL_DYNAMIC);      \
      mos_done(&st);                                                    \
      return TCL_ERROR;                                                 \
  }                                                                     \
}

/*
 * Modifiable queue attributes via admin interface
 */
typedef struct dcp_que_attr_s_t {
  char *	annotation;
  char *	aliases;
  char *	persistent;
  char *	idle_timeout;
  char *	enq_opt;
  char *	deq_opt;
  int  		qmaxlength;
  int  		msgmaxsize;
} dcp_que_attr_t;

/*
 * PROTOTYPES
 */
void proc_braces(char *);
boolean32 start_with_brace(char *);
boolean32 all_spaces(char *);
int dcp_que_is_mtype_valid(Tcl_Interp *, char *, mos_msg_attr_msgtype_t *);
int dcp_que_is_mpersistence_valid(Tcl_Interp *, char *, mos_msg_attr_persistence_t *);
int dcp_que_is_mprotectlvl_valid(Tcl_Interp *, char *, unsigned32 *);
int dcp_que_is_abstime_valid(Tcl_Interp *, char *, utc_t **);
int dcp_que_is_reltime_valid(Tcl_Interp *, char *, utc_t **);
int dcp_que_is_qpersistence_valid(Tcl_Interp *, char *, mos_que_attr_persistence_t *);
int dcp_que_is_boolean_valid(Tcl_Interp *, boolean, char *, boolean *);
int dcp_que_show_attr(Tcl_Interp *, mos_handle_t);
int dcp_que_msg_show_attr(Tcl_Interp *, mos_handle_t, mos_msg_attr_t, mos_msg_buf_t, boolean32, boolean32);
int dcp_que_attach(Tcl_Interp *, char *, char *, mos_handle_t *);
int dcp_que_msg_peek(Tcl_Interp *, mos_handle_t, char *, boolean32, boolean32);
int dcp_que_attr_set(Tcl_Interp *, dcp_que_attr_t *, mos_que_attr_t);
void dcp_que_bytearray_show(Tcl_Interp *, mos_msg_buf_t, boolean32);
int dcp_que_ack_show(Tcl_Interp *, mos_msg_buf_t, boolean32);
int dcp_que_secid_show(Tcl_Interp *, sec_id_pa_t *);

static char buf[1024], buf2[1024];


void proc_braces(char *str)
{
  char *tmp = str;

  if (str == NULL) return;

  while (*tmp != '\0') {
    if (*tmp == '{' || *tmp == '}') *tmp = ' ';
    tmp++;
  }
}


boolean32 start_with_brace(char *str)
{
  char *tmp = str;

  if (str == NULL) return FALSE;

  while (isspace((int)(*tmp))) tmp++;

  if (*tmp == '{') 
    return TRUE;
  else
    return FALSE;
}


boolean32 all_spaces(char *str)
{
  char *ptr = str;

  if (ptr == NULL) 
    return TRUE;

  while (*ptr == ' ') ptr++;

  if (*ptr == '\0') 
    return TRUE;
  else
    return FALSE;
}


/*
 * FUNCTION:
 * dcp_que_mgr_attach()
 *
 * OVERVIEW:
 * This routine attaches to a queue manager.
 */
int
dcp_que_mgr_attach
    (Tcl_Interp *       interp,
     char *		sec_group,
     char *		name,
     mos_handle_t *	handle)	
{
  char *		qmgr; 
  char *                target;
  char *                qname;
  mos_rsrc_name_t 	qmgrname;
  error_status_t        st;

  /* when sec group is not specified, check for the variable */
  if (sec_group == NULL) {
    sec_group = Tcl_GetVar(interp, "mos_quemgr_group", 0);
  }
  
  /* determine if a relative name is given */
  if ((name == NULL) || (name[0] != '/')) {
      /* Relative name. Check if a default qmgr is set. */
      qmgr = Tcl_GetVar(interp, "mos_dflt_quemgr", 0);
      if (qmgr != NULL) {
        /* set the default queue manager here and get its handle back */
        mos_rsrc_dflt_quemgr_set((unsigned_char_t *)qmgr,
  			         (unsigned_char_t *)sec_group,
  			         handle,
			         &st);
      } else {
        /* we have to use the default set outside dcecp program */
        mos_rsrc_dflt_quemgr_get((unsigned_char_t *)sec_group,
			         qmgrname,
			         handle,
			         &st);
      }
   } else {
      /* Full name. Resolve it first */
      mos_name_resolution(name, &qmgr, &target, &qname, &st);
      if (target)
        free(target);
      if (qname)
        free(qname);
      /* attach to the queue manager */
      mos_rsrc_attach((unsigned_char_t *)qmgr,
	   	      (unsigned_char_t *)sec_group, 
		      handle,
		      &st);
      if (qmgr)
        free(qmgr);
   }

   CHECK_MOS_ERROR(st);
   return(TCL_OK);

} /* End of dcp_que_mgr_attach() routine */


/*
 * FUNCTION:
 * dcp_que_attach()
 *
 * OVERVIEW:
 * This routine attaches to a queue resource.
 */
int
dcp_que_attach
    (Tcl_Interp *       interp,
     char *		sec_group,
     char *		name,
     mos_handle_t *	handle)	
{
  char *		qmgr;
  mos_rsrc_name_t 	qmgrname,qname;
  error_status_t        st;

  /* when sec group is not specified, check for the variable */
  if (sec_group == NULL) {
    sec_group = Tcl_GetVar(interp, "mos_quemgr_group", 0);
  }

  /* 
   * If queue name is a simple name, use default queue manager specified
   * in TCL variable, "mos_dflt_quemgr". Otherwise, use default queue
   * manager determined by the API.
   */
  if (name[0] != '/') {
    qmgr = Tcl_GetVar(interp, "mos_dflt_quemgr", 0);
    if (qmgr != NULL) {
      sprintf((char *)qname, "%s/%s", qmgr, name);
    }
    else {
      mos_rsrc_dflt_quemgr_get((unsigned_char_t *)sec_group,
                               qmgrname,
                               NULL,
                               &st);
      CHECK_MOS_ERROR(st);
      sprintf((char *)qname, "%s/%s", qmgrname, name);
    }
  } else {
    sprintf((char *)qname, "%s", name);
  }

  /* attach to the queue first */
  mos_rsrc_attach(qname,
		  (unsigned_char_t *)sec_group, 
		  handle,
		  &st);
  CHECK_MOS_ERROR(st);

  return(TCL_OK);

} /* End of dcp_que_attach() routine */
   

/*
 * FUNCTION:
 * dcp_que_create()
 *
 * OVERVIEW:
 * This routine creates one queue and commits its queue attributes to
 * be managed by the specified queue manager.
 * If the queue name is NULL, the name is the queue ID.
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int 
dcp_que_create
   (ClientData 		clientData, 
    Tcl_Interp *	interp, 
    int 		argc, 
    char **		argv)
{
  dcp_que_attr_t 	in_qattr;
  char *		attr_list = NULL;
  char *		sec_group = NULL;
  int 			attr_argc = 0;
  char **		attr_argv = NULL;
  char ***		tmp_argv = NULL;
  int 			i;
  mos_handle_t 		qmgr_h, q_h;
  error_status_t 	st;
  mos_que_attr_t 	q_attr;
  char *                qmgr; 
  char *		target;
  char *	        queue_name;   
  mos_rsrc_name_t 	newqname, qmgrname, qname;

  dcp_ArgvInfo arg_table[] = {
    {"-qmaxlength", DCP_ARGV_INT,     NULL, NULL, mcp_t_que_maxlength_help},
    {"-qmsgmaxsize", DCP_ARGV_INT,     NULL, NULL, mcp_t_que_msgmaxsize_help},
    {"-qpersistence", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_persistent_help},
    {"-qallowenq", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_enqueue_help},
    {"-qallowdeq", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_dequeue_help},
    {"-qannotation", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_annotation_help},
    {"-qaliases", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_alias_help},
    {"-qidletimeout", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_ttl_help},
    {"-group", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_sgp_help},
    {"-attribute", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_cre_attr_help},
    {NULL,         DCP_ARGV_END,      NULL, NULL, 0}
  };
  
  dcp_ArgvInfo arg2_table[] = {
    {"qmaxlength", DCP_ARGV_INT,     NULL, NULL, mcp_t_que_maxlength_help},
    {"qmsgmaxsize", DCP_ARGV_INT,     NULL, NULL, mcp_t_que_msgmaxsize_help},
    {"qpersistence", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_persistent_help},
    {"qallowenq", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_enqueue_help},
    {"qallowdeq", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_dequeue_help},
    {"qannotation", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_annotation_help},
    {"qaliases", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_alias_help},
    {"qidletimeout", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_ttl_help},
    {"group", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_sgp_help},
    {"attribute", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_cre_attr_help},
    {NULL,         DCP_ARGV_END,      NULL, NULL, 0}
  };
  
  arg_table[0].dst = (char *)&in_qattr.qmaxlength;
  arg_table[1].dst = (char *)&in_qattr.msgmaxsize;
  arg_table[2].dst = (char *)&in_qattr.persistent;
  arg_table[3].dst = (char *)&in_qattr.enq_opt;
  arg_table[4].dst = (char *)&in_qattr.deq_opt;
  arg_table[5].dst = (char *)&in_qattr.annotation;
  arg_table[6].dst = (char *)&in_qattr.aliases;
  arg_table[7].dst = (char *)&in_qattr.idle_timeout;
  arg_table[8].dst = (char *)&sec_group;
  arg_table[9].dst = (char *)&attr_list;
  
  arg2_table[0].dst = (char *)&in_qattr.qmaxlength;
  arg2_table[1].dst = (char *)&in_qattr.msgmaxsize;
  arg2_table[2].dst = (char *)&in_qattr.persistent;
  arg2_table[3].dst = (char *)&in_qattr.enq_opt;
  arg2_table[4].dst = (char *)&in_qattr.deq_opt;
  arg2_table[5].dst = (char *)&in_qattr.annotation;
  arg2_table[6].dst = (char *)&in_qattr.aliases;
  arg2_table[7].dst = (char *)&in_qattr.idle_timeout;
  arg2_table[8].dst = (char *)&sec_group;
  
  /* Initialize que attr */
  in_qattr.annotation = NULL;
  in_qattr.aliases = NULL;
  in_qattr.qmaxlength = -1;
  in_qattr.msgmaxsize = -1;
  in_qattr.enq_opt = NULL;
  in_qattr.deq_opt = NULL;
  in_qattr.persistent = NULL;
  in_qattr.idle_timeout = NULL;

  if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
    return TCL_ERROR;

  /* Check to see if the user just wants help */
  DCP_CHECK_COMMAND_HELP();
  
  /* Check for extraneous arguments */
  DCP_CHECK_EXTRA_ARGS();
  
  /* Set variables */
  if (argv[1])
     DCP_GENERAL_SET_NAMELIST(argv[1]);
  
  /* if an sttribute list is given ... */
  if (attr_list != NULL) {
    int tmp_argc;
    
    /* see if the attr list starts with brace "{" */
    if (start_with_brace(attr_list)) {
      /* it has multiple attribute pairs. We need to split and 
	 process them one by one*/
      if (Tcl_SplitList(interp, attr_list, &attr_argc, &attr_argv)
	  != TCL_OK)
	return TCL_ERROR;
      
      /* allocate memory to hold address of each list */
      tmp_argv = (char ***)malloc(attr_argc * sizeof (char *));
      
      for (i = 0; i < attr_argc; i++) {
	/* process this pair. Split first then parse */
	if (Tcl_SplitList(interp, attr_argv[i], &tmp_argc, &(tmp_argv[i]))
	    != TCL_OK)
	  return TCL_ERROR;
	
	/* Now we process it with arg2 table */
	if (dcp_ParseArgv(interp, &tmp_argc, tmp_argv[i], arg2_table, 
			  DCP_ARGV_DONT_SKIP_FIRST_ARG) != TCL_OK)
	  return TCL_ERROR;
      }
    }
    else {
      /* the attr list does not start with brace, it has just one attr */
      if (Tcl_SplitList(interp, attr_list, &attr_argc, &attr_argv)
	  != TCL_OK)
	return TCL_ERROR;
      
      /* Now we process it with arg2 table */
      if (dcp_ParseArgv(interp, &attr_argc, attr_argv, arg2_table, 
			DCP_ARGV_DONT_SKIP_FIRST_ARG) != TCL_OK)
	return TCL_ERROR;
    }
  }
  
  /* Allocate a queue attr record */
  mos_que_attr_alloc(&q_attr, &st);
  CHECK_MOS_ERROR(st);
  
  /* Set specified queue attributes */
  if (dcp_que_attr_set(interp, &in_qattr, q_attr) != TCL_OK) {
     mos_que_attr_free(&q_attr, &st);
     return(TCL_ERROR);
  }

  /* Attach to queue manager */
  if (dcp_que_mgr_attach(interp, sec_group, argv[1], &qmgr_h) != TCL_OK) {
     mos_que_attr_free(&q_attr, &st);
     return(TCL_ERROR);
  }
  /* Determine queue name */
  if (argv[1]) {
     mos_name_resolution(argv[1], &qmgr, &target, &queue_name, &st);
     if (qmgr)
        free(qmgr);
     if (target)
        free(target);
     if (queue_name) {
        strcpy((char *)qname, queue_name);
        free(queue_name);
     } else {
        strcpy((char *)qname, argv[1]);
     }
  } else {
     /* A NULL queue name is OK, API generates qid as name */
     qname[0] = '\0';
  }
  
  /* Create the queue */
  mos_que_mgmt_create(qmgr_h, qname, q_attr, &q_h, &st);
  CHECK_MOS_ERROR(st);
  
  /* Display fully-qualified queue name and set result */
  mos_rsrc_name_get(q_h, NULL, NULL, qmgrname, qname, &st);
  sprintf((char *)newqname, "%s/%s", qmgrname, qname);
  DCP_SET_RESULT((char *)newqname);

  /* clean up */
  if (attr_list != NULL) {
    free((char *)attr_argv);
    if (tmp_argv != NULL) {
      for (i = 0; i < attr_argc; i++)
	free((char *)tmp_argv[i]);
      free((char *)tmp_argv);
    }
  }
  mos_que_attr_free(&q_attr, &st);
  mos_done(&st);
  
  return TCL_OK;

} /* End of dcp_que_create() routine */


/*
 * FUNCTION:
 * dcp_que_catalog()
 *
 * OVERVIEW:
 * This routine returns the names of all queues on the specified queue 
 * manager.
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int 
dcp_que_catalog
   (ClientData          clientData, 
    Tcl_Interp *        interp, 
    int                 argc, 
    char **             argv)
{
  boolean32 		simplename = FALSE;
  char *		sec_group = NULL;
  int 			i;
  mos_handle_t 		qmgr_h;
  char *		qmgr;
  error_status_t 	st;
  unsigned32 		num_qs = 1, num_returned, num_left;
  mos_rsrc_name_t       qs[1], *moreqs, qmgrname, qname;
  
  dcp_ArgvInfo arg_table[] = {
    {"-simplename", DCP_ARGV_CONSTANT, (char *)1, NULL, mcp_t_que_smplnm_help},
    {"-group", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_sgp_help},
    {NULL,         DCP_ARGV_END,      NULL, NULL, 0}
  };
  
  arg_table[0].dst = (char *)&simplename;
  arg_table[1].dst = (char *)&sec_group;
  
  if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
    return TCL_ERROR;
  
  /* Check to see if the user just wants help */
  DCP_CHECK_COMMAND_HELP();
  
  /* Check for extraneous arguments */
  DCP_CHECK_EXTRA_ARGS();
  if (argv[1] && (argv[1][0] != '/')) {
     DCP_SET_MISC_ERROR(mcp_s_que_bad_qmgr_name);
  }
  
  /* Set convenience variables */
  if (argv[1])
     DCP_GENERAL_SET_NAMELIST(argv[1]);
  
  /* Attach to queue manager */
  if (dcp_que_mgr_attach(interp, sec_group, argv[1], &qmgr_h) != TCL_OK) {
     return(TCL_ERROR);
  }

  /* Get the list of queues back - just simple names */
  mos_que_mgmt_quename_list(qmgr_h,
			    num_qs,
			    &num_returned,
			    qs,
			    &num_left,
			    &st);
  CHECK_MOS_ERROR(st);
  
  /*
   * Return successfully if no queues on qmgr
   * Otherwise, check if there are more than just 1 queue. If so, 
   * allocate more space and get them again.
   */
  if (num_returned == 0) {
    return TCL_OK;           
  }
  if (num_returned == num_qs && num_left > 0) {
    num_qs = num_returned + num_left;
    moreqs = (mos_rsrc_name_t *)malloc(num_qs * sizeof (mos_rsrc_name_t));
    if (moreqs == NULL) {
      DCP_SET_MISC_ERROR(mcp_s_no_memory);
    }
    mos_que_mgmt_quename_list(qmgr_h,
			      num_qs,
			      &num_returned,
			      moreqs,
			      &num_left,
			      &st);
    CHECK_MOS_ERROR(st);
  } else {
    moreqs = qs;
  }
  
  /* Show queue name list */
  mos_rsrc_name_get(qmgr_h, NULL, NULL, qmgrname, NULL, &st);
  for (i = 0; i < num_returned; i++) {
    if (simplename) {
      strcpy((char *)qname, (char *)moreqs[i]);
    } else {
      sprintf((char *)qname, "%s/%s", (char *)qmgrname, (char *)moreqs[i]);
    }
    Tcl_AppendResult(interp, (char *)qname, 0);
    if (i != (num_returned - 1))
       Tcl_AppendResult(interp, "\n", 0);
  }
  
  /* Clean up */
  if (moreqs != qs) 
    free((char *)moreqs);
  mos_done(&st);
  return TCL_OK;

} /* End of dcp_que_catalog() routine */


/*
 * FUNCTION:
 * dcp_que_delete()
 *
 * OVERVIEW:
 * This routine deletes the specified queue.
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int 
dcp_que_delete
   (ClientData 		clientData, 
    Tcl_Interp *	interp, 
    int 		argc, 
    char **		argv)
{
  boolean32 		del_opt = FALSE;
  char *		sec_group = NULL;
  int 			rc = TCL_OK;
  mos_handle_t 		q_h;
  error_status_t 	st;
  
  dcp_ArgvInfo arg_table[] = {
    {"-force", DCP_ARGV_CONSTANT, (char *)1, NULL, mcp_t_que_del_force_help},
    {"-group", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_sgp_help},
    {NULL,         DCP_ARGV_END,      NULL, NULL, 0}
  };
  
  arg_table[0].dst = (char *)&del_opt;
  arg_table[1].dst = (char *)&sec_group;
  
  if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
    return TCL_ERROR;
  
  /* Check to see if the user just wants help */
  DCP_CHECK_COMMAND_HELP();
  
  /* Check for extraneous arguments */
  DCP_CHECK_EXTRA_ARGS();
  DCP_CHECK_QUE_NO_ARG(queue);
  
  /* Set convenience variables */
  DCP_GENERAL_SET_NAMELIST(argv[1]);
  
  /* Attach to queue */
  rc = dcp_que_attach(interp, sec_group, argv[1], &q_h);
  if (rc != TCL_OK) {
     return(rc);
  }
  
  /* delete the queue */
  mos_que_mgmt_delete(del_opt ? mos_c_delop_force : mos_c_delop_preserve,
		      &q_h,   /* implicitly detached */
		      &st);
  CHECK_MOS_ERROR(st);
  
  /* Clean-up and return */
  mos_done(&st);
  return TCL_OK;

} /* End of dcp_que_delete() routine */


/*
 * FUNCTION:
 * dcp_que_dequeue()
 *
 * OVERVIEW:
 * This routine dequeues one message from the specified queue.
 * If no selection criteria specified, then the first message in the queue
 * is dequeued. If a selection criteria is specified, then only the first 
 * message that meets the selection criteria is dequeued.
 * Currently, only a mask_operator of "equal" is supported.   
 * Note that -mttr option takes no value.
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int 
dcp_que_dequeue
   (ClientData 			clientData, 
    Tcl_Interp *		interp, 
    int 			argc, 
    char **			argv)
{
  boolean32 		ttr = FALSE;
  char *		mid = NULL;
  char *		mtype = NULL;
  char *		dtype = NULL;
  char *		deqmode = NULL;
  char *		sender = NULL;
  int 			priority = -1;
  char *		protlvl = NULL;
  char *		sec_group = NULL;
  char *		attr_list = NULL;
  int 			attr_argc = 0;
  char **		attr_argv = NULL;
  char ***		tmp_argv = NULL;
  uuid_t 	        dtpuuid;
  uuid_t 		muuid;
  unsigned32 		status;
  int 			i;
  error_status_t 	st;
  mos_handle_t 		q_h;
  unsigned32 		plevel;
  mos_msg_attr_msgtype_t msgtype;
  mos_msg_selfilter_t 	sel_rec = NULL;
  unsigned32 		deq_flags = mos_c_deqmode_nowait;
  mos_msg_attr_t 	msg_attr;
  mos_msg_buf_t 	msg;
  int 			rc = TCL_OK;

  dcp_ArgvInfo arg_table[] = {
    {"-mid", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_srh_msg_help},
    {"-mtype", DCP_ARGV_STRING,     NULL, NULL, mcp_t_que_srh_mtp_help},
    {"-mpriority",     DCP_ARGV_INT   , NULL, NULL, mcp_t_que_srh_pri_help},
    {"-mdatatype", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_srh_dtp_help},
    {"-mprotectlvl", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_srh_plv_help},
    {"-msender", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_srh_snd_help},
    {"-mttr", DCP_ARGV_CONSTANT, (char *)1, NULL, mcp_t_que_lis_ttr_help},
    {"-deqmode", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_deq_wait_help},
    {"-group", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_sgp_help},
    {"-criteria",     DCP_ARGV_STRING, NULL, NULL, mcp_t_que_srh_att_help},
    {NULL,         DCP_ARGV_END,      NULL, NULL, 0}
  };
  
  dcp_ArgvInfo arg2_table[] = {
    {"mid", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_srh_msg_help},
    {"mtype", DCP_ARGV_STRING,     NULL, NULL, mcp_t_que_srh_mtp_help},
    {"mpriority",     DCP_ARGV_INT   , NULL, NULL, mcp_t_que_srh_pri_help},
    {"mdatatype", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_srh_dtp_help},
    {"mprotectlvl", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_srh_plv_help},
    {"msender", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_srh_snd_help},
    {"mttr", DCP_ARGV_CONSTANT, (char *)1, NULL, mcp_t_que_lis_ttr_help},
    {"deqmode", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_deq_wait_help},
    {"group", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_sgp_help},
    {NULL,         DCP_ARGV_END,      NULL, NULL, 0}
  };
  
  arg_table[0].dst = (char *)&mid;
  arg_table[1].dst = (char *)&mtype;
  arg_table[2].dst = (char *)&priority;
  arg_table[3].dst = (char *)&dtype;
  arg_table[4].dst = (char *)&protlvl;
  arg_table[5].dst = (char *)&sender;
  arg_table[6].dst = (char *)&ttr;
  arg_table[7].dst = (char *)&deqmode;
  arg_table[8].dst = (char *)&sec_group;
  arg_table[9].dst = (char *)&attr_list;
  
  arg2_table[0].dst = (char *)&mid;
  arg2_table[1].dst = (char *)&mtype;
  arg2_table[2].dst = (char *)&priority;
  arg2_table[3].dst = (char *)&dtype;
  arg2_table[4].dst = (char *)&protlvl;
  arg2_table[5].dst = (char *)&sender;
  arg2_table[6].dst = (char *)&ttr;
  arg2_table[7].dst = (char *)&deqmode;
  arg2_table[8].dst = (char *)&sec_group;
  
  if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
    return TCL_ERROR;
  
  /* Check to see if the user just wants help */
  DCP_CHECK_COMMAND_HELP();
  
  /* Check for extraneous arguments */
  DCP_CHECK_EXTRA_ARGS();
  DCP_CHECK_QUE_NO_ARG(queue);
  
  /* Set convenience variables */
  DCP_GENERAL_SET_NAMELIST(argv[1]);
  
  /* if an attribute list is given ... */
  if (attr_list != NULL) {
    int tmp_argc;
    
    /* see if the attr list starts with brace "{" */
    if (start_with_brace(attr_list)) {
      /* it has multiple attribute pairs. We need to split and 
	 process them one by one*/
      if (Tcl_SplitList(interp, attr_list, &attr_argc, &attr_argv)
	  != TCL_OK)
	return TCL_ERROR;
      
      /* allocate memory to hold address of each list */
      tmp_argv = (char ***)malloc(attr_argc * sizeof (char *));
      
      for (i = 0; i < attr_argc; i++) {
	/* process this pair. Split first then parse */
	if (Tcl_SplitList(interp, attr_argv[i], &tmp_argc, &(tmp_argv[i]))
	    != TCL_OK)
	  return TCL_ERROR;
	
	/* Now we process it with arg2 table */
	if (dcp_ParseArgv(interp, &tmp_argc, tmp_argv[i], arg2_table, 
			  DCP_ARGV_DONT_SKIP_FIRST_ARG) != TCL_OK)
	  return TCL_ERROR;
      }
    }
    else {
      /* the attr list does not start with brace, it has just one attr */
      if (Tcl_SplitList(interp, attr_list, &attr_argc, &attr_argv)
	  != TCL_OK)
	  return TCL_ERROR;
      
      /* Now we process it with arg2 table */
      if (dcp_ParseArgv(interp, &attr_argc, attr_argv, arg2_table, 
			DCP_ARGV_DONT_SKIP_FIRST_ARG) != TCL_OK)
	return TCL_ERROR;
    }
  }

  /* convert UUIDs */
  if (mid != NULL) {
    uuid_from_string((unsigned_char_t *)mid, &muuid, &status);
    if (status != uuid_s_ok) {
      DCP_SET_MISC_ERROR(mcp_s_que_bad_message_id);
    }
  }
  if (dtype != NULL) {
    uuid_from_string((unsigned_char_t *)dtype, &dtpuuid, &status);
    if (status != uuid_s_ok) {
      DCP_SET_MISC_ERROR(mcp_s_que_bad_data_type);
    }
  }
  
  /* Allocate a selection filter record */
  mos_msg_selfilter_alloc(&sel_rec, &st);
  CHECK_MOS_ERROR(st);

  /* Add each specified mask */
  if (mid != NULL) {
    mos_msg_selmask_add(mos_c_mask_type_msgid,
			&muuid,
			mos_c_mask_op_equal,
			sel_rec,
			&st);
    CHECK_MOS_ERROR(st);
  }
  
  if (dtype != NULL) {
    mos_msg_selmask_add(mos_c_mask_type_datatype,
			&dtpuuid,
			mos_c_mask_op_equal,
			sel_rec,
			&st);
    CHECK_MOS_ERROR(st);
  }
  
  if (mtype != NULL) {
    if (dcp_que_is_mtype_valid(interp, mtype, &msgtype) != TCL_OK) {
      return(TCL_ERROR);
    }
    mos_msg_selmask_add(mos_c_mask_type_msgtype,
			&msgtype,
			mos_c_mask_op_equal,
			sel_rec,
			&st);
    CHECK_MOS_ERROR(st);
  }
  
  if (priority != -1) {
    mos_msg_selmask_add(mos_c_mask_type_priority,
			&priority,
			mos_c_mask_op_equal,
			sel_rec,
			&st);
    CHECK_MOS_ERROR(st);
  }
  
  if (ttr) {
    mos_msg_selmask_add(mos_c_mask_type_ttr,
			NULL,
			mos_c_mask_op_none,
			sel_rec,
			&st);
    CHECK_MOS_ERROR(st);
  }
  
  if (sender != NULL) {
    mos_msg_selmask_add(mos_c_mask_type_sender_prin,
			sender,
			mos_c_mask_op_equal,
			sel_rec,
			&st);
    CHECK_MOS_ERROR(st);
  }
  
  if (protlvl != NULL) {
    if (dcp_que_is_mprotectlvl_valid(interp, protlvl, &plevel) != TCL_OK) {
       return(TCL_ERROR);
    }
    mos_msg_selmask_add(mos_c_mask_type_protectlvl,
			&plevel,
			mos_c_mask_op_equal,
			sel_rec,
			&st);
    CHECK_MOS_ERROR(st);
  }

  /* check the dequeue wait mode */
  if (deqmode != NULL) {
    if (!strcmp(deqmode, "deqwait"))
      deq_flags = mos_c_deqmode_wait;
    else if (!strcmp(deqmode, "nowaitdeq"))
      deq_flags = mos_c_deqmode_nowait;
    else
      DCP_SET_MISC_ERROR(mcp_s_que_bad_deq_wait_mode); 
  }

  /* Attach to queue */
  rc = dcp_que_attach(interp, sec_group, argv[1], &q_h);
  if (rc != TCL_OK) {
     return(rc);
  }
  
  /* dequeue a message according to the selection filter */
  mos_msg_dequeue(q_h,
		  deq_flags,
		  sel_rec,
		  &msg_attr,
		  &msg,
		  &st);
  CHECK_MOS_ERROR(st);
  
  /* Display dequeued message */
  rc = dcp_que_msg_show_attr(interp, q_h, msg_attr, msg, FALSE, TRUE);

  /* clean up */
  if (attr_list != NULL) {
    free((char *)attr_argv);
    if (tmp_argv != NULL) {
      for (i = 0; i < attr_argc; i++)
	free((char *)tmp_argv[i]);
      free((char *)tmp_argv);
    }
  }
  mos_msg_selfilter_free(&sel_rec, &st);
  mos_msg_attr_free(&msg_attr, &st);
  mos_msg_buf_free(&msg, &st);
  mos_done(&st);

  return(rc);

} /* End of dcp_que_dequeue() routine */


/*
 * FUNCTION:
 * dcp_que_show()
 *
 * OVERVIEW:
 * This routine returns the attributes of the specified queue.
 * If a message ID is NOT specified, show queue attributes.
 * If a message ID is specified, then:
 *   By default:  Show the message attributes
 *   -body        Show the message body
 *   -all         Show both message body and its message attributes
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int 
dcp_que_show
   (ClientData 		clientData, 
    Tcl_Interp *	interp, 
    int 		argc, 
    char **		argv)
{
  char *		mid = NULL;
  boolean32 		body = FALSE;
  boolean32 		all = FALSE;
  char *		sec_group = NULL;
  int 			rc = TCL_OK;
  mos_handle_t 		q_h;
  error_status_t 	st;
  
  dcp_ArgvInfo arg_table[] = {
    {"-mid",   DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_msgid_show_help},
    {"-body", DCP_ARGV_CONSTANT, (char *)1, NULL,mcp_t_que_shw_msgbody_help},
    {"-all", DCP_ARGV_CONSTANT, (char *)1, NULL,mcp_t_que_shw_msgall_help},
    {"-group", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_sgp_help},
    {NULL,         DCP_ARGV_END,      NULL, NULL, 0}
  };
  
  arg_table[0].dst = (char *)&mid;
  arg_table[1].dst = (char *)&body;
  arg_table[2].dst = (char *)&all;
  arg_table[3].dst = (char *)&sec_group;
  
  if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
    return TCL_ERROR;
  
  /* Check to see if the user just wants help */
  DCP_CHECK_COMMAND_HELP();
  
  /* Check for extraneous arguments */
  DCP_CHECK_EXTRA_ARGS();
  DCP_CHECK_QUE_NO_ARG(queue);
  
  /* Check options used correctly */
  if ((mid == NULL) && (body || all)) {
     DCP_SET_MISC_ERROR(mcp_s_opt_mid_req); 
  }
  if (body && all) {
     DCP_SET_MISC_ERROR(mcp_s_no_all_and_body); 
  }

  /* Set convenience variables */
  DCP_GENERAL_SET_NAMELIST(argv[1]);
  
  
  /* Attach to queue */
  rc = dcp_que_attach(interp, sec_group, argv[1], &q_h);
  if (rc != TCL_OK) {
     return(rc);
  }

  if (mid == NULL) {
     /* Show queue attributes */
     rc = dcp_que_show_attr(interp, q_h);
  }
  else {
     /* Show a message body and/or its attributes from the queue */
     rc = dcp_que_msg_peek(interp, q_h, mid, body, all); 	  
  }
  
  /* Clean-up and return */
  mos_done(&st);
  return(rc);
}


int 
dcp_que_msg_peek
   (Tcl_Interp *	interp,
    mos_handle_t 	q_h,
    char *              mid,
    boolean32           body,
    boolean32           all)
{
    mos_msg_selfilter_t selfilter;
    mos_msg_attr_t 	msg_attr = NULL;
    mos_msg_buf_t 	msg = NULL;
    uuid_t 		muuid;
    error_status_t      st;
    int                 rc = TCL_OK;

    /* convert mid to the uuid */
    uuid_from_string((unsigned_char_t *)mid, &muuid, &st);
    if (st != uuid_s_ok) {
      DCP_SET_MISC_ERROR(mcp_s_que_bad_message_id);
    }

    /* alloc selection filter */
    mos_msg_selfilter_alloc(&selfilter, &st);
    CHECK_MOS_ERROR(st);

    /* set the msgid as our selection */
    mos_msg_selmask_add(mos_c_mask_type_msgid,
			(mos_msg_selmask_value_t)&muuid,
			mos_c_mask_op_equal,
			selfilter,
			&st);
    CHECK_MOS_ERROR(st);

    if (body || all) {
      /* both message body & attributes */
      mos_msg_peek(q_h,
		   mos_c_deqmode_nowait, /* no wait on empty and no match */
		   selfilter,
		   &msg_attr,
		   &msg,
		   &st);
    }
    else {
      /* message attributes only */
      mos_msg_attr_peek(q_h,
			mos_c_deqmode_nowait, /* no wait when no match */
			selfilter,
			&msg_attr,
			&st);
    }
    CHECK_MOS_ERROR(st);

    /* free the selection filter */
    mos_msg_selfilter_free(&selfilter, &st);
    CHECK_MOS_ERROR(st);
  
    /* this is for message show */
    rc = dcp_que_msg_show_attr(interp, q_h, msg_attr, msg, body, all);

    /* clean up */
    mos_msg_attr_free(&msg_attr, &st);
    mos_msg_buf_free(&msg, &st);
    return(rc);

} /* End of dcp_que_msg_peek() routine */


int 
dcp_que_msg_show_attr
   (Tcl_Interp *	interp,
    mos_handle_t 	q_h,
    mos_msg_attr_t 	msg_attr,
    mos_msg_buf_t 	msg,
    boolean32 		body,
    boolean32 		all
   )
{
  error_status_t st;
  char *msg_hex_buf, *src_ptr, *des_ptr;
  int msg_hex_buf_len, i;
  uuid_t mid;
  unsigned_char_t *mid_string;
  utc_t enqtime, ttl, ttr;
  mos_msg_attr_msgtype_t msgtype;
  unsigned32 priority;
  mos_msg_attr_persistence_t persistency;
  unsigned32 notice_flags;
  mos_rsrc_name_t notice_dest;
  uuid_t dtype;
  unsigned_char_t *string_uuid;
  unsigned32 protect_level;
  sec_id_pa_t secid;
  sec_rgy_name_t notice_group;

  /* now we handle message attributes */
  if (!body) {

    /* msg id */
    mos_msg_attr_msgid_get(msg_attr, &mid, &st);
    CHECK_MOS_ERROR(st);
    uuid_to_string(&mid, &mid_string, &st);
    CHECK_MOS_ERROR(st);
    sprintf(buf, "{mid %s}\n", mid_string);
    Tcl_SetResult(interp, buf, TCL_VOLATILE);
    rpc_string_free(&mid_string, &st);

    /* enqueue time */
    mos_msg_attr_enqtime_get(msg_attr, &enqtime, &st);
    CHECK_MOS_ERROR(st);
    utc_ascgmtime(buf2, sizeof(buf2), &enqtime);
    sprintf(buf, "{menqtime %s}\n", buf2);
    Tcl_AppendResult(interp, buf, 0);

    /* message type */
    mos_msg_attr_msgtype_get(msg_attr, &msgtype, &st);
    CHECK_MOS_ERROR(st);
    switch (msgtype) {
    case mos_c_msg_attr_msgtype_data:
      sprintf(buf, "{mtype data}\n");
      break;
    case mos_c_msg_attr_msgtype_notice:
      sprintf(buf, "{mtype notice}\n");
    }
    Tcl_AppendResult(interp, buf, 0);

    /* priority */
    mos_msg_attr_priority_get(msg_attr, &priority, &st);
    CHECK_MOS_ERROR(st);
    sprintf(buf, "{mpriority %d}\n", priority);
    Tcl_AppendResult(interp, buf, 0);

    /* ttl/expiration */
    mos_msg_attr_ttl_get(msg_attr, &ttl, &st);
    if (st == mos_s_msg_attr_ttl_null) {
      /* not an error. It means that a message remains indefinitely 
	 until explicitly dequeued. */
      sprintf(buf, "{mexpiration forever}\n");
    }
    else {
      CHECK_MOS_ERROR(st);
      utc_ascgmtime(buf2, sizeof(buf2), &ttl);
      sprintf(buf, "{mexpiration %s}\n", buf2);
    }
    Tcl_AppendResult(interp, buf, 0);

    /* ttr/ receive time */
    mos_msg_attr_ttr_get(msg_attr, &ttr, &st);
    if (st == mos_s_msg_attr_ttr_null) {
      /* not an error. It means that a message is allowed to 
	 be retrieved now. */
      sprintf(buf, "{mreceivetime now}\n");
    }
    else {
      CHECK_MOS_ERROR(st);
      utc_ascgmtime(buf2, sizeof(buf2), &ttr);
      sprintf(buf, "{mreceivetime %s}\n", buf2);
    }
    Tcl_AppendResult(interp, buf, 0);

    /* persistency */
    mos_msg_attr_persistence_get(msg_attr, &persistency, &st);
    CHECK_MOS_ERROR(st);
    switch (persistency) {
    case mos_c_msg_attr_persistent:
      sprintf(buf, "{mpersistence persistent}\n");
      break;
    case mos_c_msg_attr_volatile:
      sprintf(buf, "{mpersistence volatile}\n");
    }
    Tcl_AppendResult(interp, buf, 0);

    /* notice */
    mos_msg_attr_notice_get(msg_attr, &notice_flags, 
			    notice_dest, notice_group, &st);
    CHECK_MOS_ERROR(st);
    if (notice_flags == mos_c_msg_attr_notice_none) {
      sprintf(buf, "{mopnotice none}\n");
      Tcl_AppendResult(interp, buf, 0);
    }
    else {
      strcpy(buf2, "{");
      if (notice_flags & mos_c_msg_attr_notice_enqueue)
	strcat(buf2, "enqueue ");
      if (notice_flags & mos_c_msg_attr_notice_dequeue)
	strcat(buf2, "dequeue ");
      if (buf2[strlen(buf2)-1] == ' ')
	buf2[strlen(buf2)-1] = '}';
      else
	strcat(buf2, "}");
      sprintf(buf, "{mopnotice %s}\n", buf2);
      Tcl_AppendResult(interp, buf, 0);

      /* display the notice destination too */
      sprintf(buf, "{mqnotice %s}\n", notice_dest);
      Tcl_AppendResult(interp, buf, 0);

      /* display the notice destination security group */
      if (notice_group[0] == '\0') {
          sprintf(buf, "{mgrpnotice {}}\n");
      } else {
          sprintf(buf, "{mgrpnotice %s}\n", notice_group);
      }
      Tcl_AppendResult(interp, buf, 0);
    }

    /* data type */
    mos_msg_attr_datatype_get(msg_attr, &dtype, &st);
    CHECK_MOS_ERROR(st);
    if (uuid_is_nil(&dtype, &st)) {
      sprintf(buf, "{mdatatype nil}\n");
    } else {
      uuid_to_string(&dtype, &string_uuid, &st);
      CHECK_MOS_ERROR(st);
      sprintf(buf, "{mdatatype %s}\n", string_uuid);
      rpc_string_free(&string_uuid, &st);
    }
    Tcl_AppendResult(interp, buf, 0);

    /* protection level */
    mos_msg_attr_protectlvl_get(msg_attr, &protect_level, &st);
    CHECK_MOS_ERROR(st);
    switch ((int)protect_level) {
      case mos_c_protect_level_default:
        sprintf(buf, "{mprotectlvl default}\n");  
        break;
      case mos_c_protect_level_none:
        sprintf(buf, "{mprotectlvl none}\n");
        break;
      case mos_c_protect_level_auth:
        sprintf(buf, "{mprotectlvl authn}\n");
        break;
      case mos_c_protect_level_integ:
        sprintf(buf, "{mprotectlvl integrity}\n");
        break;
      case mos_c_protect_level_privacy:
        sprintf(buf, "{mprotectlvl privacy}\n");
        break;
    }
    Tcl_AppendResult(interp, buf, 0);

    /* sec ids */
    mos_msg_attr_secid_get(msg_attr, &secid, &st);
    CHECK_MOS_ERROR(st);
    if (dcp_que_secid_show(interp, &secid) != TCL_OK) {
       return(TCL_ERROR);
    }
  } /* End of if (!body) */
    
  if (body || all) {
    /* Check message data type */
    mos_msg_attr_datatype_get(msg_attr, &dtype, &st);
    CHECK_MOS_ERROR(st);

    if (uuid_is_nil(&dtype, &st)) {     /* Show array of bytes. */
       dcp_que_bytearray_show(interp, msg, all);
    } else if (uuid_equal(&dtype, &mos_datatype_ack, &st)) { /* Show ACK */
       if (dcp_que_ack_show(interp, msg, all) != TCL_OK) {
          return(TCL_ERROR);
       }
    } else {
       /* this data has a non-nil data type. The message body is encoded. */
       /* set the buffer content into the variable */
       /* we need to change every byte into two hex number so that TCl can
          handle it. The length of the hex buffer will double */
       msg_hex_buf_len = msg->size * 2 + 1;
       msg_hex_buf = malloc(msg_hex_buf_len);
       if (msg_hex_buf == NULL) {
         DCP_SET_MISC_ERROR(mcp_s_no_memory);
       }
       /* put a '\0' at the end to make a string */
       msg_hex_buf[msg_hex_buf_len-1] = '\0';
    
       src_ptr = (char *)msg->data;
       des_ptr = msg_hex_buf;
       for (i = 0; i < msg->size; i++) {
         if (*src_ptr < 16)
	   sprintf(des_ptr, "0%x", *src_ptr);
         else
	   sprintf(des_ptr, "%x", *src_ptr);
         src_ptr ++;
         des_ptr += 2;
       }
    
       /* set this buffer to the variable mos_encoded_msg */
       Tcl_SetVar(interp, "mos_encoded_msg", msg_hex_buf, 0);
       free(msg_hex_buf);
    
       /* set the result */
       if (interp->result[0] != '\0')
         Tcl_AppendResult(interp, "\nCheck variable mos_encoded_msg for hexadecimal representation of the encoded message body", 0);
       else
         Tcl_SetResult(interp, "\nCheck variable mos_encoded_msg for hexadecimal representation of the encoded message body", TCL_VOLATILE);
  
    } /* End of application-specific message body */
  } /* End of show message body contents */

  return TCL_OK;

} 


void
dcp_que_bytearray_show
   (Tcl_Interp *		interp, 
    mos_msg_buf_t 		msg,
    boolean32                   all)
{
    if (all) {
      sprintf(buf, "\n{body ");
      Tcl_AppendResult(interp, buf, 0);
    }

    if (msg->size > 0) 
      sprintf(buf, "{%s}", (char *)msg->data);
    else
      sprintf(buf, "{}");
    Tcl_AppendResult(interp, buf, 0);

    if (all) {
      sprintf(buf, "}");
      Tcl_AppendResult(interp, buf, 0);
    }

} /* End of dcp_que_bytearray_show() routine */


int
dcp_que_secid_show
   (Tcl_Interp *		interp, 
    sec_id_pa_t * 		secid)
{
    sec_rgy_name_t      prin_name;        /* principal name */
    sec_rgy_name_t      realm_name;       /* realm name */
    sec_rgy_name_t      group_name;       /* group name */
    char *              cell_name;        /* Local cell name */
    sec_rgy_handle_t    rgy_handle;       /* Local registry binding */
    error_status_t      st;
    uuid_t              dummy_uuid;
    int			i, j;

    /*
     * Obtain local cell name
     */
    dce_cf_get_cell_name(&cell_name, &st);
    CHECK_MOS_ERROR(st);

    /*
     * Bind to local cell registry
     */
    sec_rgy_site_open(cell_name, &rgy_handle, &st);
    CHECK_MOS_ERROR(st);
    free(cell_name);    /* Got handle, free cell name */
    
    /* 
     * Show secid information 
     */
    sprintf(buf, "{msecid\n");
    Tcl_AppendResult(interp, buf, 0);

    /*
     * Obtain realm and principal names
     */
    sec_id_gen_name(rgy_handle,
                    &secid->realm.uuid,
                    &secid->principal.uuid,
                    NULL, 
                    realm_name, 
                    prin_name, 
                    &st);
    CHECK_MOS_ERROR(st);
    sprintf(buf, " {{realm %s}\n", realm_name);
    Tcl_AppendResult(interp, buf, 0);
    sprintf(buf, "  {principal %s}\n", prin_name);
    Tcl_AppendResult(interp, buf, 0);

    /* group names */
    sprintf(buf, "  {groups");
    Tcl_AppendResult(interp, buf, 0);
    for (i = 0; i < secid->num_groups; i++) {
      sec_rgy_pgo_id_to_name(rgy_handle,
                             sec_rgy_domain_group,
                             &secid->groups[i].uuid,
                             group_name, 
                             &st);
      CHECK_MOS_ERROR(st);
      sprintf(buf, " %s", group_name);
      Tcl_AppendResult(interp, buf, 0);
    }
    sprintf(buf, "}");
    Tcl_AppendResult(interp, buf, 0);

    /* foreign group names */
    if (secid->num_foreign_groupsets) {
       sprintf(buf, "\n  {fgroupsets\n");
       Tcl_AppendResult(interp, buf, 0);
       if (secid->num_foreign_groupsets > 1) {
          sprintf(buf, "   {");
          Tcl_AppendResult(interp, buf, 0);
       }
       for (i = 0; i < secid->num_foreign_groupsets; i++) {
         sec_id_gen_name(rgy_handle,
                         &secid->foreign_groupsets[i].realm.uuid,
                         &dummy_uuid,
                         NULL, 
                         realm_name, 
                         NULL, 
                         &st);
         CHECK_MOS_ERROR(st);
         if (i) {
            sprintf(buf, "    ");
            Tcl_AppendResult(interp, buf, 0);
         }
         sprintf(buf, "{{frealm %s}\n", realm_name);
         Tcl_AppendResult(interp, buf, 0);
         sprintf(buf, "     {fgroups");
         Tcl_AppendResult(interp, buf, 0);
         for (j = 0; j < secid->foreign_groupsets[i].num_groups; j++) {
            sec_rgy_pgo_id_to_name(rgy_handle,
                                   sec_rgy_domain_group,
                                   &secid->foreign_groupsets[i].groups[j].uuid,
                                   group_name, 
                                   &st);
            CHECK_MOS_ERROR(st);
            sprintf(buf, " %s", group_name);
            Tcl_AppendResult(interp, buf, 0);
         }
         sprintf(buf, "}");
         Tcl_AppendResult(interp, buf, 0);
       } /* End of loop thru each foreign group set */
       if (secid->num_foreign_groupsets > 1) {
          sprintf(buf, "}");
          Tcl_AppendResult(interp, buf, 0);
       }
    }

    sprintf(buf, "}}");
    Tcl_AppendResult(interp, buf, 0);
    return(TCL_OK);

} /* End of dcp_que_secid_show() routine */


int
dcp_que_ack_show
   (Tcl_Interp *		interp, 
    mos_msg_buf_t 		msg,
    boolean32                   all)
{
    mos_datatype_ack_t 		ack_msg_buf;
    unsigned_char_t *		string_uuid;
    error_status_t     		st;

    /* Register ACK, may already be registered */
    mos_api_datatype_register(&mos_datatype_ack,
                              (mos_msg_attr_convfn_t)mos_ack_convert,
                              &st);

    /* Decode ACK msgs */
    mos_msg_datatype_decode(&mos_datatype_ack,
                            msg,
                            &ack_msg_buf,
                            &st);
    CHECK_MOS_ERROR(st);

    if (all) {
      sprintf(buf, "\n{body\n {");
      Tcl_AppendResult(interp, buf, 0);
    }

    /* Show msg ID that requested delivery notification */
    uuid_to_string(&ack_msg_buf.msg_id, &string_uuid, &st);
    sprintf(buf, "{mid %s}\n", string_uuid);
    Tcl_AppendResult(interp, buf, 0);
    rpc_string_free(&string_uuid, &st);
    if (all) {
      sprintf(buf, "  ");
      Tcl_AppendResult(interp, buf, 0);
    }

    /* Show delivery notification for this notice msg */
    if (ack_msg_buf.notice_option & mos_c_msg_attr_notice_enqueue) {
      sprintf(buf, "{mopnotice enqueue}\n");
    } else {
      sprintf(buf, "{mopnotice dequeue}\n");
    }
    Tcl_AppendResult(interp, buf, 0);
    if (all) {
      sprintf(buf, "  ");
      Tcl_AppendResult(interp, buf, 0);
    }

    /* Show status of delivery notification */
    if (ack_msg_buf.status == 0) {
      sprintf(buf, "{status %d}", ack_msg_buf.status);
    } else {
      sprintf(buf, "{status {%s}}", 
                    (char *)dce_sprintf(ack_msg_buf.status));
    }
    Tcl_AppendResult(interp, buf, 0);
    if (all) {
      sprintf(buf, "}}");
      Tcl_AppendResult(interp, buf, 0);
    }

    return(TCL_OK);

} /* End of dcp_que_ack_show() routine */


int dcp_que_show_attr
   (Tcl_Interp *	interp, 
    mos_handle_t 	q_h)
{
  error_status_t st;
  unsigned32 num_qalias, num_left;
  mos_rsrc_name_t qalias[1], *qaliases;
  uuid_t qid;
  unsigned_char_t *string_uuid;
  unsigned32 qlen, qmaxlen, msgmaxsize;
  utc_t qctime, qatime;
  mos_que_attr_annotation_t annotation;
  mos_que_attr_persistence_t persistency;
  utc_t timeout;
  boolean allowenq, allowdeq;
  mos_que_attr_t q_attr;
  int i;

  mos_que_attr_peek(q_h,
		    &q_attr,
		    &st);
  CHECK_MOS_ERROR(st);
  
  /* get each attribute out of the attribute record and set to the 
     Tcl result */
  
  /* queue name and alias */
  mos_que_attr_alias_list(q_attr, 1, &num_qalias, qalias, &num_left, &st);
  CHECK_MOS_ERROR(st);
  if (num_left > 0) {
    num_qalias += num_left;
    qaliases = (mos_rsrc_name_t *)malloc(num_qalias *
					 sizeof(mos_rsrc_name_t));
    if (qaliases == NULL) {
      DCP_SET_MISC_ERROR(mcp_s_no_memory);
    }
    /* get back all */
    mos_que_attr_alias_list(q_attr, num_qalias, 
			    &num_qalias, qaliases, &num_left, &st);
    CHECK_MOS_ERROR(st);
  }
  else {
    qaliases = qalias;
  }
  /* display queue name */
  sprintf(buf, "{qname %s}\n", qaliases[0]);
  Tcl_SetResult(interp, buf, TCL_VOLATILE);

  /* display queue aliases */
  Tcl_AppendResult(interp, "{qaliases ", (char *)NULL);
  for (i = 1; i < num_qalias; i++) {
    sprintf(buf, "%s", qaliases[i]);
    Tcl_AppendResult(interp, buf, 0);
    if (i != (num_qalias - 1)) 
       Tcl_AppendResult(interp, "\n          ", (char *)NULL);
  }
  if (num_qalias > 1) {
     Tcl_AppendResult(interp, "}\n", (char *)NULL); 
     free((char *)qaliases);
  } else {
     Tcl_AppendResult(interp, "{}}\n", (char *)NULL); 
  }

  /* queue id */
  mos_que_attr_queid_get(q_attr, &qid, &st);
  CHECK_MOS_ERROR(st);
  uuid_to_string(&qid, &string_uuid, &st);
  CHECK_MOS_ERROR(st);
  sprintf(buf, "{qid %s}\n", string_uuid);
  Tcl_AppendResult(interp, buf, 0);
  rpc_string_free(&string_uuid, &st);
  
  /* queue current length */
  mos_que_attr_quecursize_get(q_attr, &qlen, &st);
  CHECK_MOS_ERROR(st);
  sprintf(buf, "{qlength %d}\n", qlen);
  Tcl_AppendResult(interp, buf, 0);
  
  /* queue max length */
  mos_que_attr_quemaxsize_get(q_attr, &qmaxlen, &st);
  CHECK_MOS_ERROR(st);
  if (qmaxlen) {
    sprintf(buf, "{qmaxlength %d}\n", qmaxlen);
  } else {
    sprintf(buf, "{qmaxlength unlimited}\n");
  }
  Tcl_AppendResult(interp, buf, 0);
  
  /* queue max msg size */
  mos_que_attr_msgmaxsize_get(q_attr, &msgmaxsize, &st);
  CHECK_MOS_ERROR(st);
  if (msgmaxsize) {
    sprintf(buf, "{qmaxmsgsize %d}\n", msgmaxsize);
  } else {
    sprintf(buf, "{qmaxmsgsize unlimited}\n");
  }
  Tcl_AppendResult(interp, buf, 0);
  
  /* queue creation time */
  mos_que_attr_createtime_get(q_attr, &qctime, &st);
  CHECK_MOS_ERROR(st);
  utc_ascgmtime(buf2, sizeof(buf2), &qctime);
  sprintf(buf, "{qcreatetime %s}\n", buf2);
  Tcl_AppendResult(interp, buf, 0);
  
  /* queue's last activity time */
  mos_que_attr_activetime_get(q_attr, &qatime, &st);
  CHECK_MOS_ERROR(st);
  utc_ascgmtime(buf2, sizeof(buf2), &qatime);
  sprintf(buf, "{qlastactivetime %s}\n", buf2);
  Tcl_AppendResult(interp, buf, 0);
  
  /* queue annotation */
  mos_que_attr_annotation_get(q_attr, annotation, &st);
  CHECK_MOS_ERROR(st);
  sprintf(buf, "{qannotation {%s}}\n", annotation);
  Tcl_AppendResult(interp, buf, 0);
  
  /* queue persistency */
  mos_que_attr_persistence_get(q_attr, &persistency, &st);
  CHECK_MOS_ERROR(st);
  switch (persistency) {
  case mos_c_que_attr_persistent:
    sprintf(buf, "{qpersistence persistent}\n");
    break;
  case mos_c_que_attr_volatile:
    sprintf(buf, "{qpersistence volatile}\n");
    break;
  case mos_c_que_attr_msgpersistence:
    sprintf(buf, "{qpersistence msgpersistence}\n");
  }
  Tcl_AppendResult(interp, buf, 0);
  
  /* timeout for idle */
  mos_que_attr_idletimeout_get(q_attr, &timeout, &st);
  if (st == mos_s_que_attr_idletimeout_null) {
    /* this is not an error. It means there is no timeout */
    sprintf(buf, "{qidletimeout forever}\n");
  }
  else {
    CHECK_MOS_ERROR(st);
    utc_ascreltime(buf2, sizeof(buf2), &timeout);
    sprintf(buf, "{qidletimeout %s}\n", buf2);
  }
  Tcl_AppendResult(interp, buf, 0);
  
  /* queue allow enqueue */
  mos_que_attr_allowenq_get(q_attr, &allowenq, &st);
  CHECK_MOS_ERROR(st);
  if (allowenq) {
     sprintf(buf, "{qallowenq yes}\n");
  } else {
     sprintf(buf, "{qallowenq no}\n");
  }
  Tcl_AppendResult(interp, buf, 0);
  
  /* queue allow dequeue */
  mos_que_attr_allowdeq_get(q_attr, &allowdeq, &st);
  CHECK_MOS_ERROR(st);
  if (allowdeq) {
     sprintf(buf, "{qallowdeq yes}");
  } else {
     sprintf(buf, "{qallowdeq no}");
  }
  Tcl_AppendResult(interp, buf, 0);
  
  /* free the q attr record */
  mos_que_attr_free(&q_attr, &st);
  CHECK_MOS_ERROR(st);
  
  return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_que_modify()
 *
 * OVERVIEW:
 * This routine commits queue attribute modifications on an existing queue.
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int 
dcp_que_modify
   (ClientData 			clientData, 
    Tcl_Interp *		interp, 
    int 			argc, 
    char **			argv)
{
  dcp_que_attr_t        in_qattr;
  char *                sec_group = NULL;
  char *                attr_list = NULL;
  int                   attr_argc = 0;
  char **               attr_argv = NULL;
  char ***              tmp_argv = NULL;
  int                   i;
  int                   rc = TCL_OK;
  error_status_t 	st;
  mos_handle_t 		q_h;
  mos_que_attr_t 	q_attr;

  dcp_ArgvInfo arg_table[] = {
    {"-qmaxlength",  DCP_ARGV_INT,     NULL, NULL, mcp_t_que_maxlength_help},
    {"-qmsgmaxsize", DCP_ARGV_INT,     NULL, NULL, mcp_t_que_msgmaxsize_help},
    {"-qpersistence", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_persistent_help},
    {"-qallowenq", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_enqueue_help},
    {"-qallowdeq", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_dequeue_help},
    {"-qannotation", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_annotation_help},
    {"-qaliases", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_addalias_help},
    {"-qidletimeout", DCP_ARGV_STRING , NULL, NULL, mcp_t_que_ttl_help},
    {"-group", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_sgp_help},
    {"-change", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_mod_attr_help},
    {NULL,         DCP_ARGV_END,      NULL, NULL, 0}
  };
    
  dcp_ArgvInfo arg2_table[] = {
    {"qmaxlength",  DCP_ARGV_INT,     NULL, NULL, mcp_t_que_maxlength_help},
    {"qmsgmaxsize", DCP_ARGV_INT,     NULL, NULL, mcp_t_que_msgmaxsize_help},
    {"qpersistence", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_persistent_help},
    {"qallowenq", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_enqueue_help},
    {"qallowdeq", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_dequeue_help},
    {"qannotation", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_annotation_help},
    {"qaliases", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_addalias_help},
    {"qidletimeout",  DCP_ARGV_STRING, NULL, NULL, mcp_t_que_ttl_help},
    {"group", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_sgp_help},
    {NULL,         DCP_ARGV_END,      NULL, NULL, 0}
  };
  
  arg_table[0].dst = (char *)&in_qattr.qmaxlength;
  arg_table[1].dst = (char *)&in_qattr.msgmaxsize;
  arg_table[2].dst = (char *)&in_qattr.persistent;
  arg_table[3].dst = (char *)&in_qattr.enq_opt;
  arg_table[4].dst = (char *)&in_qattr.deq_opt;
  arg_table[5].dst = (char *)&in_qattr.annotation;
  arg_table[6].dst = (char *)&in_qattr.aliases;
  arg_table[7].dst = (char *)&in_qattr.idle_timeout;
  arg_table[8].dst = (char *)&sec_group;
  arg_table[9].dst = (char *)&attr_list;
  
  arg2_table[0].dst = (char *)&in_qattr.qmaxlength;
  arg2_table[1].dst = (char *)&in_qattr.msgmaxsize;
  arg2_table[2].dst = (char *)&in_qattr.persistent;
  arg2_table[3].dst = (char *)&in_qattr.enq_opt;
  arg2_table[4].dst = (char *)&in_qattr.deq_opt;
  arg2_table[5].dst = (char *)&in_qattr.annotation;
  arg2_table[6].dst = (char *)&in_qattr.aliases;
  arg2_table[7].dst = (char *)&in_qattr.idle_timeout;
  arg2_table[8].dst = (char *)&sec_group;
  
  /* Initialize que attr */
  in_qattr.annotation = NULL;
  in_qattr.aliases = NULL;
  in_qattr.qmaxlength = -1;
  in_qattr.msgmaxsize = -1;
  in_qattr.enq_opt = NULL;
  in_qattr.deq_opt = NULL;
  in_qattr.persistent = NULL;
  in_qattr.idle_timeout = NULL;

  if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
    return TCL_ERROR;
  
  /* Check to see if the user just wants help */
  DCP_CHECK_COMMAND_HELP();
  
  /* Check for extraneous arguments */
  DCP_CHECK_EXTRA_ARGS();
  DCP_CHECK_QUE_NO_ARG(queue);
  
  /* Set variables */
  DCP_GENERAL_SET_NAMELIST(argv[1]);
  
  /* if an attribute list is given ... */
  if (attr_list != NULL) {
    int tmp_argc;
    
    /* see if the attr list starts with brace "{" */
    if (start_with_brace(attr_list)) {
      /* it has multiple attribute pairs. We need to split and 
	 process them one by one*/
      if (Tcl_SplitList(interp, attr_list, &attr_argc, &attr_argv)
	  != TCL_OK)
	return TCL_ERROR;
      
      /* allocate memory to hold address of each list */
      tmp_argv = (char ***)malloc(attr_argc * sizeof (char *));
      
      for (i = 0; i < attr_argc; i++) {
	/* process this pair. Split first then parse */
	if (Tcl_SplitList(interp, attr_argv[i], &tmp_argc, &(tmp_argv[i]))
	    != TCL_OK)
	  return TCL_ERROR;
	
	/* Now we process it with arg2 table */
	if (dcp_ParseArgv(interp, &tmp_argc, tmp_argv[i], arg2_table, 
			  DCP_ARGV_DONT_SKIP_FIRST_ARG) != TCL_OK)
	  return TCL_ERROR;
      }
    }
    else {
      /* the attr list does not start with brace, it has just one attr */
      if (Tcl_SplitList(interp, attr_list, &attr_argc, &attr_argv)
	  != TCL_OK)
	return TCL_ERROR;
      
      /* Now we process it with arg2 table */
      if (dcp_ParseArgv(interp, &attr_argc, attr_argv, arg2_table, 
			DCP_ARGV_DONT_SKIP_FIRST_ARG) != TCL_OK)
	return TCL_ERROR;
    }
  }
  
  /* Attach to queue */
  rc = dcp_que_attach(interp, sec_group, argv[1], &q_h);
  if (rc != TCL_OK) {
     return(rc);
  }

  /* Obtain the current queue attributes */
  mos_que_attr_peek(q_h,
		    &q_attr,
		    &st);
  CHECK_MOS_ERROR(st);
  
  /* Set specified queue attributes */
  if (dcp_que_attr_set(interp, &in_qattr, q_attr) != TCL_OK) {
     mos_que_attr_free(&q_attr, &st);
     return(TCL_ERROR);
  }

  /* Commit the new queue attributes */
  mos_que_attr_commit(q_h, q_attr, &st);
  CHECK_MOS_ERROR(st);

  /* Clean up */
  if (attr_list != NULL) {
    free((char *)attr_argv);
    if (tmp_argv != NULL) {
      for (i = 0; i < attr_argc; i++)
	free((char *)tmp_argv[i]);
      free((char *)tmp_argv);
    }
  }
  mos_done(&st);
  return TCL_OK;
  
} /* End of dcp_que_modify() routine */


/*
 * FUNCTION:
 * dcp_que_move()
 *
 * OVERVIEW:
 * This routine moves an entire queue or a queue member to another queue.
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int 
dcp_que_move
   (ClientData 			clientData, 
    Tcl_Interp *		interp, 
    int 			argc, 
    char **			argv)
{
  char *		new_name = NULL;
  boolean32 		referral = FALSE;
  char *		srcgroup = NULL; 
  char *		destgroup = NULL;
  char *		mid = NULL;
  uuid_t 		muuid;
  unsigned32 		status;
  int 			rc = TCL_OK;
  mos_handle_t          srcq_h, destq_h, qmgr_h;
  error_status_t 	st;
  unsigned32 		move_flag = mos_c_que_mv_none;
  mos_rsrc_name_t 	newqname, qmgrname;
  char *qmgr, *queue_name, *target;

  dcp_ArgvInfo arg_table[] = {
    {"-to",   DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_mov_name_help},
    {"-referral", DCP_ARGV_CONSTANT, (char *)1, NULL, mcp_t_que_mov_ref_help},
    {"-mid",   DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_msgid_move_help},
    {"-srcgroup", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_srcsgp_help},
    {"-destgroup", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_destsgp_help},
    {NULL,         DCP_ARGV_END,      NULL, NULL, 0}
  };
  
  arg_table[0].dst = (char *)&new_name;
  arg_table[1].dst = (char *)&referral;
  arg_table[2].dst = (char *)&mid;
  arg_table[3].dst = (char *)&srcgroup;
  arg_table[4].dst = (char *)&destgroup;
  
  if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
    return TCL_ERROR;
  
  /* Check to see if the user just wants help */
  DCP_CHECK_COMMAND_HELP();
  
  /* Check for extraneous arguments */
  DCP_CHECK_EXTRA_ARGS();
  DCP_CHECK_QUE_NO_ARG(queue);
  
  /* Set convenience variables */
  DCP_GENERAL_SET_NAMELIST(argv[1]);
  
  /* has to give a destination queue name */
  if (new_name == NULL) {
    DCP_SET_MISC_ERROR(mcp_s_que_no_dest_qname);
  }

  /* Referral allowed only on a queue move not message move operation. */
  if ((mid != NULL) && (referral))  {
    DCP_SET_MISC_ERROR(mcp_s_opt_referral);
  }

  /* convert mid to the uuid */
  if (mid != NULL) {
    uuid_from_string((unsigned_char_t *)mid, &muuid, &status);
    if (status != uuid_s_ok) {
      DCP_SET_MISC_ERROR(mcp_s_que_bad_message_id);
    }
  }
  
  /* Attach to source queue */
  rc = dcp_que_attach(interp, srcgroup, argv[1], &srcq_h);
  if (rc != TCL_OK) {
     return(rc);
  }
  
  if (mid != NULL) {

    /* Attach to destination queue. It must already exist. */
    rc = dcp_que_attach(interp, destgroup, new_name, &destq_h);
    if (rc != TCL_OK) {
        mos_done(&st);
        return(rc);
    }

    /* move a single message */
    mos_msg_move(srcq_h,     /* source queue */
		 destq_h,    /* destination queue */
		 &muuid,
		 &st);
    CHECK_MOS_ERROR(st);
    
  } else {
    
    /* when destination sec group is not specified, check for the variable */
    if (destgroup == NULL) {
      destgroup = Tcl_GetVar(interp, "mos_quemgr_group", 0);
    }

    /* 
     * On a queue move operation, the destination queue may not exist.
     * Obtain destination queue manager.
     */
    if (new_name[0] == '/') {
      /* the user specify a full name. resolve it first */
      mos_name_resolution(new_name, &qmgr, &target, &queue_name, &st);
      if (st != rpc_s_ok && st != rpc_s_partial_results)
        CHECK_MOS_ERROR(st);

      /* attach to the queue manager */
      mos_rsrc_attach((unsigned_char_t *)qmgr,
                      (unsigned_char_t *)destgroup,
                      &qmgr_h,
                      &st);
    } else {
      /* the user enters a relative name. Check if a default qmgr is set.
         If the var is not set, NULL will be returned */
      qmgr = Tcl_GetVar(interp, "mos_dflt_quemgr", 0);
      if (qmgr != NULL) {
        /* set the default queue manager here and get its handle back */
        mos_rsrc_dflt_quemgr_set((unsigned_char_t *)qmgr,
                                 (unsigned_char_t *)destgroup,
                                 &qmgr_h,
                                 &st);
      }
      else {
        /* we have to use the default set outside dcecp program */
        mos_rsrc_dflt_quemgr_get((unsigned_char_t *)destgroup,
                                 qmgrname,
                                 &qmgr_h,
                                 &st);
        qmgr = (char *)qmgrname;
      }
      queue_name = new_name;
    }
    CHECK_MOS_ERROR(st);
  
    /* move entire queue */
    if (referral) {
      move_flag = mos_c_que_mv_referral;
    }
    mos_que_mgmt_move(qmgr_h,    /* destination queue manager */
		      (unsigned_char_t *)queue_name,
		      move_flag,
                      &srcq_h, 	 /* move queue */
		      &st);
    CHECK_MOS_ERROR(st);
    
    /* make the new fully qualified name as result */
    sprintf((char *)newqname, "%s/%s", qmgr, queue_name);
    DCP_SET_RESULT((char *)newqname);
  }

  /* clean up */
  mos_done(&st);
  return TCL_OK;
  
} /* End of dcp_que_move() routine */


/*
 * FUNCTION:
 * dcp_que_list()
 *
 * OVERVIEW:
 * This routine returns a list of messages on the specified queue.
 * If no selection criteria specified, then all messages returned.
 * If a selection criteria is specified, then only a list of messages
 * that meet the selection criteria is returned.
 * Currently, only a mask_operator of "equal" is supported.   
 * Note that -mttr option takes no value.
 *
 * Needs to be implemented:
 * Selection criteria is specified as:
 *  TCL syntax:
 *    By default, a mask operator of "equal" is used:
 *      {<mask type> <mask value>}
 *    Otherwise:
 *      {<mask type> <mask value> <mask operator>}
 *  String syntax:
 *    By default, a mask operator of "equal" is used:
 *      <mask type>:<mask value>
 *    Otherwise:
 *      <mask type>:<mask value>:<mask operator>
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int 
dcp_que_list
   (ClientData 		clientData, 
    Tcl_Interp *	interp, 
    int 		argc, 
    char **		argv)
{
  char *mid = NULL;
  uuid_t muuid;
  int     		rc = TCL_OK;
  unsigned32 		status;
  boolean32 		ttr = FALSE;
  char *		mtype = NULL;
  char *		dtype = NULL;
  uuid_t 		dtpuuid;
  char *		sender = NULL;
  int 		        priority = -1;
  char *                prio_op = NULL;
  char *                protlvl = NULL;
  char *                sec_group = NULL;
  char *		attr_list = NULL;
  int 			attr_argc = 0;
  char **		attr_argv = NULL;
  char ***		tmp_argv = NULL;
  int 			i;
  error_status_t 	st;
  mos_handle_t 		q_h;
  uuid_t *uuids, oneuuid[1];
  unsigned32  		num_returned, num_left;
  unsigned32 		plevel;
  mos_msg_attr_msgtype_t msgtype;
  mos_msg_selfilter_t 	sel_rec = NULL;

  dcp_ArgvInfo arg_table[] = {
    {"-mid", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_srh_msg_help},
    {"-mtype", DCP_ARGV_STRING,     NULL, NULL, mcp_t_que_srh_mtp_help},
    {"-mpriority",     DCP_ARGV_INT   , NULL, NULL, mcp_t_que_srh_pri_help},
    {"-mdatatype", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_srh_dtp_help},
    {"-mprotectlvl", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_srh_plv_help},
    {"-msender", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_srh_snd_help},
    {"-mttr", DCP_ARGV_CONSTANT, (char *)1, NULL, mcp_t_que_lis_ttr_help},
    {"-group", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_sgp_help},
    {"-criteria",     DCP_ARGV_STRING, NULL, NULL, mcp_t_que_srh_att_help},
    {NULL,         DCP_ARGV_END,      NULL, NULL, 0}
  };
  
  dcp_ArgvInfo arg2_table[] = {
    {"mid", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_srh_msg_help},
    {"mtype", DCP_ARGV_STRING,     NULL, NULL, mcp_t_que_srh_mtp_help},
    {"mpriority",     DCP_ARGV_INT   , NULL, NULL, mcp_t_que_srh_pri_help},
    {"mdatatype", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_srh_dtp_help},
    {"mprotectlvl", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_srh_plv_help},
    {"msender", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_srh_snd_help},
    {"mttr", DCP_ARGV_CONSTANT, (char *)1, NULL, mcp_t_que_lis_ttr_help},
    {"group", DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_sgp_help},
    {NULL,         DCP_ARGV_END,      NULL, NULL, 0}
  };
  
  arg_table[0].dst = (char *)&mid;
  arg_table[1].dst = (char *)&mtype;
  arg_table[2].dst = (char *)&priority;
  arg_table[3].dst = (char *)&dtype;
  arg_table[4].dst = (char *)&protlvl;
  arg_table[5].dst = (char *)&sender;
  arg_table[6].dst = (char *)&ttr;
  arg_table[7].dst = (char *)&sec_group;
  arg_table[8].dst = (char *)&attr_list;
  
  arg2_table[0].dst = (char *)&mid;
  arg2_table[1].dst = (char *)&mtype;
  arg2_table[2].dst = (char *)&priority;
  arg2_table[3].dst = (char *)&dtype;
  arg2_table[4].dst = (char *)&protlvl;
  arg2_table[5].dst = (char *)&sender;
  arg2_table[6].dst = (char *)&ttr;
  arg2_table[7].dst = (char *)&sec_group;
  
  if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
    return TCL_ERROR;
  
  /* Check to see if the user just wants help */
  DCP_CHECK_COMMAND_HELP();
  
  /* Check for extraneous arguments */
  DCP_CHECK_EXTRA_ARGS();
  DCP_CHECK_QUE_NO_ARG(queue);
  
  /* Set convenience variables */
  DCP_GENERAL_SET_NAMELIST(argv[1]);
  
  /* if an sttribute list is given ... */
  if (attr_list != NULL) {
    int tmp_argc;
    
    /* see if the attr list starts with brace "{" */
    if (start_with_brace(attr_list)) {
      /* it has multiple attribute pairs. We need to split and 
	 process them one by one*/
      if (Tcl_SplitList(interp, attr_list, &attr_argc, &attr_argv)
	  != TCL_OK)
	return TCL_ERROR;
      
      /* allocate memory to hold address of each list */
      tmp_argv = (char ***)malloc(attr_argc * sizeof (char *));
      
      for (i = 0; i < attr_argc; i++) {
	/* process this pair. Split first then parse */
	if (Tcl_SplitList(interp, attr_argv[i], &tmp_argc, &(tmp_argv[i]))
	    != TCL_OK)
	  return TCL_ERROR;
	
	/* Now we process it with arg2 table */
	if (dcp_ParseArgv(interp, &tmp_argc, tmp_argv[i], arg2_table, 
			  DCP_ARGV_DONT_SKIP_FIRST_ARG) != TCL_OK)
	  return TCL_ERROR;
      }
    }
    else {
      /* the attr list does not start with brace, it has just one attr */
      if (Tcl_SplitList(interp, attr_list, &attr_argc, &attr_argv)
	  != TCL_OK)
	  return TCL_ERROR;
      
      /* Now we process it with arg2 table */
      if (dcp_ParseArgv(interp, &attr_argc, attr_argv, arg2_table, 
			DCP_ARGV_DONT_SKIP_FIRST_ARG) != TCL_OK)
	return TCL_ERROR;
    }
  }

  /* convert UUIDs */
  if (mid != NULL) {
    uuid_from_string((unsigned_char_t *)mid, &muuid, &status);
    if (status != uuid_s_ok) {
      DCP_SET_MISC_ERROR(mcp_s_que_bad_message_id);
    }
  }
  if (dtype != NULL) {
    uuid_from_string((unsigned_char_t *)dtype, &dtpuuid, &status);
    if (status != uuid_s_ok) {
      DCP_SET_MISC_ERROR(mcp_s_que_bad_data_type);
    }
  }
  
  /* allocate a selection filter record */
  mos_msg_selfilter_alloc(&sel_rec, &st);
  CHECK_MOS_ERROR(st);

  /* Add each selection mask */
  if (mid != NULL) {
    mos_msg_selmask_add(mos_c_mask_type_msgid,
			&muuid,
			mos_c_mask_op_equal,
			sel_rec,
			&st);
    CHECK_MOS_ERROR(st);
  }
  
  if (dtype != NULL) {
    mos_msg_selmask_add(mos_c_mask_type_datatype,
			&dtpuuid,
			mos_c_mask_op_equal,
			sel_rec,
			&st);
    CHECK_MOS_ERROR(st);
  }
  
  if (mtype != NULL) {
    if (dcp_que_is_mtype_valid(interp, mtype, &msgtype) != TCL_OK) {
      return(TCL_ERROR);
    }
    mos_msg_selmask_add(mos_c_mask_type_msgtype,
			&msgtype,
			mos_c_mask_op_equal,
			sel_rec,
			&st);
    CHECK_MOS_ERROR(st);
  }
  
  if (priority != -1) {
    mos_msg_selmask_add(mos_c_mask_type_priority,
			&priority,
			mos_c_mask_op_equal,
			sel_rec,
			&st);
    CHECK_MOS_ERROR(st);
  }
  
  if (ttr) {
    mos_msg_selmask_add(mos_c_mask_type_ttr,
			NULL,
			mos_c_mask_op_none,
			sel_rec,
			&st);
    CHECK_MOS_ERROR(st);
  }
  
  if (sender != NULL) {
    mos_msg_selmask_add(mos_c_mask_type_sender_prin,
			sender,
			mos_c_mask_op_equal,
			sel_rec,
			&st);
    CHECK_MOS_ERROR(st);
  }
  
  if (protlvl != NULL) {
    if (dcp_que_is_mprotectlvl_valid(interp, protlvl, &plevel) != TCL_OK) {
       return(TCL_ERROR);
    }
    mos_msg_selmask_add(mos_c_mask_type_protectlvl,
			&plevel,
			mos_c_mask_op_equal,
			sel_rec,
			&st);
    CHECK_MOS_ERROR(st);
  }
  
  /* attach to the queue */
  rc = dcp_que_attach(interp, sec_group, argv[1], &q_h);
  if (rc != TCL_OK) {
     return(rc);
  }
    
  /* Most likely there are more than one matching msgs, need to find
     out first */
  mos_que_mgmt_msgid_list(q_h, /* queue */
			  sel_rec,
			  1,
			  &num_returned,
			  oneuuid,
			  &num_left,
			  &st);
  CHECK_MOS_ERROR(st);
  
  if (num_returned == 1 && num_left > 0) {
    /* let us allocate enough space for them */
    uuids = (uuid_t *)malloc(sizeof(uuid_t) * (num_left+1));
    if (uuids == NULL) {
      DCP_SET_MISC_ERROR(mcp_s_no_memory);
    }
    mos_que_mgmt_msgid_list(q_h, /* queue */
			    sel_rec,
			    num_left+1,
			    &num_returned,
			    uuids,
			    &num_left,
			    &st);
    CHECK_MOS_ERROR(st);
  }      
  else if (num_returned == 1) {
    uuids = oneuuid;
  }

  if (num_returned > 0) {
    /* set the results */
    unsigned_char_t *string_uuid;
    unsigned32 status;

    uuid_to_string(&uuids[0], &string_uuid, &status);
    CHECK_MOS_ERROR(status);
    Tcl_SetResult(interp, (char *)string_uuid, TCL_VOLATILE);
    rpc_string_free(&string_uuid, &status);

    for (i = 1; i < num_returned; i++) {
      uuid_to_string(&uuids[i], &string_uuid, &status);
      CHECK_MOS_ERROR(status);
      Tcl_AppendResult(interp, "\n", 0);
      Tcl_AppendResult(interp, string_uuid, 0);
      rpc_string_free(&string_uuid, &status);
    }
  }

  /* clean up and return */
  if (attr_list != NULL) {
    free((char *)attr_argv);
    if (tmp_argv != NULL) {
      for (i = 0; i < attr_argc; i++)
	free((char *)tmp_argv[i]);
      free((char *)tmp_argv);
    }
  }
  mos_msg_selfilter_free(&sel_rec, &st);
  mos_done(&st);
  return TCL_OK;

} /* End of dcp_que_list() routine */


/*
 * FUNCTION:
 * dcp_que_enqueue()
 *
 * OVERVIEW:
 * This routine enqueues one message to a queue.
 * A NULL message body is allowed.
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int 
dcp_que_enqueue
   (ClientData 		clientData, 
    Tcl_Interp *	interp, 
    int 		argc, 
    char **		argv)
{
  char *		body = NULL;
  int  			priority = -1;
  char * 		persistent = NULL;
  char *		enq_mode = NULL;
  char *		prot_lvl = NULL;
  char *		sec_group = NULL;
  char *		notice_op = NULL;
  char *		notice_q = NULL;
  char *		notice_group = NULL;
  char *		mtype = NULL;
  char *		dtype = NULL;
  char *		attr_list = NULL;
  char *		exp = NULL;
  char *		ttr = NULL;
  utc_t * 		tout;
  unsigned32 		status;
  int 		        attr_argc = 0;
  char **		attr_argv = NULL;
  int 			i;
  char ***		tmp_argv = NULL;
  mos_handle_t 		q_h;
  error_status_t 	st;
  mos_msg_attr_t 	msg_attr;
  mos_datatype_bytearray_t   msg_buf;
  uuid_t 		mid;
  uuid_t 		dtpuuid;
  unsigned_char_t *	string_uuid;
  unsigned32 		mode_flags = mos_c_enqmode_async;
  unsigned32 		plevel;
  unsigned32 		attr_notice = 0;
  sec_rgy_name_t        not_group;
  int 			rc = TCL_OK;

  dcp_ArgvInfo arg_table[] = {
    {"-mpriority",   DCP_ARGV_INT,  NULL, NULL, mcp_t_que_add_pri_help},
    {"-mtype", DCP_ARGV_STRING,   NULL, NULL, mcp_t_que_add_mtp_help},
    {"-mdatatype", DCP_ARGV_STRING,   NULL, NULL, mcp_t_que_add_dtp_help},
    {"-mopnotice", DCP_ARGV_STRING,   NULL, NULL, mcp_t_que_add_ntc_help},
    {"-mqnotice", DCP_ARGV_STRING,   NULL, NULL, mcp_t_que_add_rply_help},
    {"-mgrpnotice", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_add_rpygp_help},
    {"-mpersistence", DCP_ARGV_STRING,NULL,NULL,mcp_t_que_add_pst_help},
    {"-mexpiration", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_add_exp_help},
    {"-mreceivetime",     DCP_ARGV_STRING, NULL, NULL, mcp_t_que_add_ttr_help},
    {"-mprotectlvl",     DCP_ARGV_STRING, NULL, NULL, mcp_t_que_add_prt_help},
    {"-body", DCP_ARGV_STRING,   NULL, NULL, mcp_t_que_add_bdy_help},
    {"-enqmode",     DCP_ARGV_STRING, NULL, NULL, mcp_t_que_add_wtm_help},
    {"-group",     DCP_ARGV_STRING, NULL, NULL, mcp_t_que_sgp_help},
    {"-attribute",     DCP_ARGV_STRING, NULL, NULL, mcp_t_que_add_att_help},
    {NULL,         DCP_ARGV_END,      NULL, NULL, 0}
  };
    
  dcp_ArgvInfo arg2_table[] = {
    {"mpriority",   DCP_ARGV_INT,  NULL, NULL, mcp_t_que_add_pri_help},
    {"mtype", DCP_ARGV_STRING,   NULL, NULL, mcp_t_que_add_mtp_help},
    {"mdatatype", DCP_ARGV_STRING,   NULL, NULL, mcp_t_que_add_dtp_help},
    {"mopnotice", DCP_ARGV_STRING,   NULL, NULL, mcp_t_que_add_ntc_help},
    {"mqnotice", DCP_ARGV_STRING,   NULL, NULL, mcp_t_que_add_rply_help},
    {"mgrpnotice", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_add_rpygp_help},
    {"mpersistence", DCP_ARGV_STRING,NULL, NULL,mcp_t_que_add_pst_help},
    {"mexpiration", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_add_exp_help},
    {"mreceivetime", DCP_ARGV_STRING, NULL, NULL, mcp_t_que_add_ttr_help},
    {"mprotectlvl",     DCP_ARGV_STRING, NULL, NULL, mcp_t_que_add_prt_help},
    {"body", DCP_ARGV_STRING,   NULL, NULL, mcp_t_que_add_bdy_help},
    {"enqmode",     DCP_ARGV_STRING, NULL, NULL, mcp_t_que_add_wtm_help},
    {"group",     DCP_ARGV_STRING, NULL, NULL, mcp_t_que_sgp_help},
    {NULL,         DCP_ARGV_END,      NULL, NULL, 0}
  };
    
  arg_table[0].dst = (char *)&priority;
  arg_table[1].dst = (char *)&mtype;
  arg_table[2].dst = (char *)&dtype;
  arg_table[3].dst = (char *)&notice_op;
  arg_table[4].dst = (char *)&notice_q;
  arg_table[5].dst = (char *)&notice_group;
  arg_table[6].dst = (char *)&persistent;
  arg_table[7].dst = (char *)&exp;
  arg_table[8].dst = (char *)&ttr;
  arg_table[9].dst = (char *)&prot_lvl;
  arg_table[10].dst = (char *)&body;
  arg_table[11].dst = (char *)&enq_mode;
  arg_table[12].dst = (char *)&sec_group;
  arg_table[13].dst = (char *)&attr_list;

  arg2_table[0].dst = (char *)&priority;
  arg2_table[1].dst = (char *)&mtype;
  arg2_table[2].dst = (char *)&dtype;
  arg2_table[3].dst = (char *)&notice_op;
  arg2_table[4].dst = (char *)&notice_q;
  arg2_table[5].dst = (char *)&notice_group;
  arg2_table[6].dst = (char *)&persistent;
  arg2_table[7].dst = (char *)&exp;
  arg2_table[8].dst = (char *)&ttr;
  arg2_table[9].dst = (char *)&prot_lvl;
  arg2_table[10].dst = (char *)&body;
  arg2_table[11].dst = (char *)&enq_mode;
  arg2_table[12].dst = (char *)&sec_group;

  if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
    return TCL_ERROR;
  
  /* Check to see if the user just wants help */
  DCP_CHECK_COMMAND_HELP();
  
  /* Check for extraneous arguments */
  DCP_CHECK_EXTRA_ARGS();
  DCP_CHECK_QUE_NO_ARG(queue);
  
  /* 
   * Determine if notice options and its dependent notice fields,
   * notice destination and notice security group name, are specified
   * properly.
   */
  if ((notice_op == NULL) && 
      ((notice_q != NULL) || (notice_group != NULL))) {
     DCP_SET_MISC_ERROR(mcp_s_que_no_noticeops);
  }

  /* Set convenience variables */
  DCP_GENERAL_SET_NAMELIST(argv[1]);
  
  /* if an sttribute list is given ... */
  if (attr_list != NULL) {
    int tmp_argc;
    
    /* see if the attr list starts with brace "{" */
    if (start_with_brace(attr_list)) {
      /* it has multiple attribute pairs. We need to split and 
	 process them one by one*/
      if (Tcl_SplitList(interp, attr_list, &attr_argc, &attr_argv)
	  != TCL_OK)
	return TCL_ERROR;
      
      /* allocate memory to hold address of each list */
      tmp_argv = (char ***)malloc(attr_argc * sizeof (char *));
      
      for (i = 0; i < attr_argc; i++) {
	/* process this pair. Split first then parse */
	if (Tcl_SplitList(interp, attr_argv[i], &tmp_argc, &(tmp_argv[i]))
	    != TCL_OK)
	  return TCL_ERROR;
	
	/* Now we process it with arg2 table */
	if (dcp_ParseArgv(interp, &tmp_argc, tmp_argv[i], arg2_table, 
			  DCP_ARGV_DONT_SKIP_FIRST_ARG) != TCL_OK)
	  return TCL_ERROR;
      }
    }
    else {
      /* the attr list does not start with brace, it has just one attr */
      if (Tcl_SplitList(interp, attr_list, &attr_argc, &attr_argv)
	  != TCL_OK)
	return TCL_ERROR;
      
      /* Now we process it with arg2 table */
      if (dcp_ParseArgv(interp, &attr_argc, attr_argv, arg2_table, 
			DCP_ARGV_DONT_SKIP_FIRST_ARG) != TCL_OK)
	return TCL_ERROR;
    }
  }

  /* Attach to specified queue */
  rc = dcp_que_attach(interp, sec_group, argv[1], &q_h);
  if (rc != TCL_OK) {
     return(rc);
  }

  /* 
   * Allocate a msg attribute structure 
   * Note that all message defaults are set implicitly by the API.
   */
  mos_msg_attr_alloc(&msg_attr, &st);
  CHECK_MOS_ERROR(st);

  /* If requested, set the attributes one by one */
  if (priority != -1) {
     mos_msg_attr_priority_set(priority,
   			       msg_attr,
			       &st);
     CHECK_MOS_ERROR(st);
  }

  if (mtype != NULL) {
    mos_msg_attr_msgtype_t msgtype;
    if (dcp_que_is_mtype_valid(interp, mtype, &msgtype) != TCL_OK) {
      return(TCL_ERROR);
    }
    mos_msg_attr_msgtype_set(msgtype, msg_attr, &st);
    CHECK_MOS_ERROR(st);
  }

  if (dtype != NULL) {
    uuid_from_string((unsigned_char_t *)dtype, &dtpuuid, &status);
    if (status != uuid_s_ok) {
      DCP_SET_MISC_ERROR(mcp_s_que_bad_data_type);
    }
    /* well, even you get that far, this is not supported yet. Sorry :-) */
    DCP_SET_MISC_ERROR(mcp_s_que_no_dtype_support);
  } else {
    if (body) {
      msg_buf.size = (strlen(body) + 1);
      msg_buf.data = (unsigned_char_t *)body;
    }
    else {
      msg_buf.size = 0;
      msg_buf.data = NULL;
    }
  }

  if (notice_op != NULL) {
    char *ptr;
    int i;

    /* this var may have multiple values. Replace braces with spaces first */
    proc_braces(notice_op);
		
    if ((ptr = strstr(notice_op, "none")) != NULL) {
      attr_notice |= mos_c_msg_attr_notice_none;
      for (i = 0; i < strlen("none"); i++, ptr++)
	*ptr = ' ';
    }
    if ((ptr = strstr(notice_op, "enqueue")) != NULL)  {
      attr_notice |= mos_c_msg_attr_notice_enqueue;
      for (i = 0; i < strlen("enqueue"); i++, ptr++)
	*ptr = ' ';
    }
    if ((ptr = strstr(notice_op, "dequeue")) != NULL) {
      attr_notice |= mos_c_msg_attr_notice_dequeue;
      for (i = 0; i < strlen("dequeue"); i++, ptr++)
	*ptr = ' ';
    }
    if (!all_spaces(notice_op)) {
      DCP_SET_MISC_ERROR(mcp_s_que_bad_notice);
    }

    if ((attr_notice != mos_c_msg_attr_notice_none) && 
        (notice_q == NULL)) {
      DCP_SET_MISC_ERROR(mcp_s_que_no_qnotice);
    }

    /* only when it indeed requires a notice */
    if (attr_notice != mos_c_msg_attr_notice_none) {
      if (notice_group == NULL)
	not_group[0] = '\0';
      else
	strncpy((char *)not_group, notice_group, sec_rgy_name_t_size);
      mos_msg_attr_notice_set(attr_notice,
			      (unsigned_char_t *)notice_q,
			      not_group,
			      msg_attr,
			      NULL,
			      &st);
      CHECK_MOS_ERROR(st);
    }
  }
  
  if (persistent != NULL) {
    mos_msg_attr_persistence_t pers;
    if (dcp_que_is_mpersistence_valid(interp, persistent, &pers) != TCL_OK) {
       return(TCL_ERROR);
    }
    mos_msg_attr_persistence_set(pers, msg_attr, &st);
    CHECK_MOS_ERROR(st);
  }
  
  if (exp != NULL) {
    if (dcp_que_is_abstime_valid(interp, exp, &tout) != TCL_OK) {
       return(TCL_ERROR);
    }
    mos_msg_attr_ttl_set(tout, msg_attr, &st);
    CHECK_MOS_ERROR(st);
  }
  
  if (ttr != NULL) {
    if (dcp_que_is_abstime_valid(interp, ttr, &tout) != TCL_OK) {
       return(TCL_ERROR);
    }
    mos_msg_attr_ttr_set(tout, msg_attr, &st);
    CHECK_MOS_ERROR(st);
  }
  
  if (prot_lvl != NULL) {
    if (dcp_que_is_mprotectlvl_valid(interp, prot_lvl, &plevel) != TCL_OK) {
       return(TCL_ERROR);
    }
    mos_msg_attr_protectlvl_set(plevel, msg_attr, &st);
    CHECK_MOS_ERROR(st);
  }

  if (enq_mode != NULL) {
    if (!strcmp(enq_mode, "async"))
      mode_flags |= mos_c_enqmode_async;
    if (!strcmp(enq_mode, "nowaitenq"))
      mode_flags |= mos_c_enqmode_nowait_enq;
    else if (!strcmp(enq_mode, "enqwait"))
      mode_flags |= mos_c_enqmode_wait_enq;
    else if (!strcmp(enq_mode, "deqwait")) 
      mode_flags |= mos_c_enqmode_wait_deq;
    else {
      DCP_SET_MISC_ERROR(mcp_s_que_bad_enq_mode);
    }
  } 

  /* now send the message (add it to the queue) */
  mos_msg_enqueue(q_h,
		  mode_flags,
		  msg_attr,
		  &msg_buf,
		  &mid,
		  &st);
  CHECK_MOS_ERROR(st);

  /* let us convert the mid into a string and set it as the result */
  uuid_to_string(&mid, &string_uuid, &st);
  CHECK_MOS_ERROR(st);
  Tcl_SetResult(interp, (char *)string_uuid, TCL_VOLATILE);
  rpc_string_free(&string_uuid, &st);

  /* clean up */
  if (attr_list != NULL) {
    free((char *)attr_argv);
    if (tmp_argv != NULL) {
      for (i = 0; i < attr_argc; i++)
	free((char *)tmp_argv[i]);
      free((char *)tmp_argv);
    }
  }
  mos_msg_attr_free(&msg_attr, &st);
  mos_done(&st);
  return(TCL_OK);

} /* End of dcp_que_enqueue() routine */


/*
 * FUNCTION:
 * dcp_que_remove()
 *
 * OVERVIEW:
 * This routine removes one message from the specified queue.
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int 
dcp_que_remove
   (ClientData 			clientData, 
    Tcl_Interp *		interp, 
    int 			argc, 
    char **			argv)
{
  char *		mid = NULL;
  char *		sec_group = NULL;
  int                   rc = TCL_OK;
  uuid_t 		muuid;
  unsigned32 		status;
  mos_handle_t 		q_h;
  error_status_t 	st;

  dcp_ArgvInfo arg_table[] = {
    {"-mid",   DCP_ARGV_STRING,  NULL, NULL, mcp_t_que_msgid_rem_help},
    {"-group",     DCP_ARGV_STRING, NULL, NULL, mcp_t_que_sgp_help},
    {NULL,         DCP_ARGV_END,      NULL, NULL, 0}
  };
  
  arg_table[0].dst = (char *)&mid;
  arg_table[1].dst = (char *)&sec_group;

  if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
    return TCL_ERROR;
  
  /* Check to see if the user just wants help */
  DCP_CHECK_COMMAND_HELP();
  
  /* Check for extraneous arguments */
  DCP_CHECK_EXTRA_ARGS();
  DCP_CHECK_QUE_NO_ARG(queue);
  
  /* Set convenience variables */
  DCP_GENERAL_SET_NAMELIST(argv[1]);
  
  /* message id has to be there */
  if (mid == NULL || mid[0] == '\0') {
    DCP_SET_MISC_ERROR(mcp_s_que_no_msg_id);
  }
  
  /* convert uuids */
  uuid_from_string((unsigned_char_t *)mid, &muuid, &status);
  if (status != uuid_s_ok) {
    DCP_SET_MISC_ERROR(mcp_s_que_bad_message_id);
  }

  /* Attach to queue */
  rc = dcp_que_attach(interp, sec_group, argv[1], &q_h);
  if (rc != TCL_OK) {
     return(rc);
  }

  /* delete the message */
  mos_msg_delete(q_h, &muuid, &st);
  CHECK_MOS_ERROR(st);

  mos_done(&st);
  return TCL_OK;
    
} /* End of dcp_que_remove() routine */


int
dcp_que_is_qpersistence_valid
   (Tcl_Interp *                        interp,
    char *				pers_string, 
    mos_que_attr_persistence_t *	pers)
{

    if (!strcmp(pers_string, "persistent")) {
      *pers = mos_c_que_attr_persistent;
    }
    else if (!strcmp(pers_string, "volatile")) {
      *pers = mos_c_que_attr_volatile;
    }
    else if (!strcmp(pers_string, "msgpersistence")) {
      *pers = mos_c_que_attr_msgpersistence;
    }
    else {
      DCP_SET_MISC_ERROR(mcp_s_que_bad_qpersistent_value);
    }

    return(TCL_OK);

} /* End of dcp_que_is_qpersistence_valid() routine */


int
dcp_que_is_mtype_valid
   (Tcl_Interp *                        interp,
    char *				mtype_string, 
    mos_msg_attr_msgtype_t *	        msg_type)
{

    if (!strcmp(mtype_string, "data")) {
      *msg_type = mos_c_msg_attr_msgtype_data;
    }
    else if (!strcmp(mtype_string, "notice")) {
      *msg_type = mos_c_msg_attr_msgtype_notice;
    }
    else {
      DCP_SET_MISC_ERROR(mcp_s_que_bad_message_type);
    }

    return(TCL_OK);

} /* End of dcp_que_is_msgtype_valid() routine */


int
dcp_que_is_mpersistence_valid
   (Tcl_Interp *                        interp,
    char *				pers_string, 
    mos_msg_attr_persistence_t *	pers)
{

    if (!strcmp(pers_string, "persistent")) {
      *pers = mos_c_msg_attr_persistent;
    }
    else if (!strcmp(pers_string, "volatile")) {
      *pers = mos_c_msg_attr_volatile;
    }
    else {
      DCP_SET_MISC_ERROR(mcp_s_que_bad_mpersistent_value);
    }

    return(TCL_OK);

} /* End of dcp_que_is_mpersistence_valid() routine */

int
dcp_que_is_mprotectlvl_valid
   (Tcl_Interp *                        interp,
    char *				mpl_string, 
    unsigned32 *	                mprotectlvl)
{
    if (!strcmp(mpl_string, "default"))
      *mprotectlvl = mos_c_protect_level_default;
    else if (!strcmp(mpl_string, "none"))
      *mprotectlvl = mos_c_protect_level_none;
    else if (!strcmp(mpl_string, "authn")) 
      *mprotectlvl = mos_c_protect_level_auth;
    else if (!strcmp(mpl_string, "integrity"))
      *mprotectlvl = mos_c_protect_level_integ;
    else if (!strcmp(mpl_string, "privacy"))
      *mprotectlvl = mos_c_protect_level_privacy;
    else {
      DCP_SET_MISC_ERROR(mcp_s_que_bad_protection_level);
    }
    return(TCL_OK);

} /* End of dcp_que_is_mprotectlvl_valid() routine */


int
dcp_que_is_boolean_valid
   (Tcl_Interp *                        interp,
    boolean				enqueue,
    char *				option, 
    boolean *				value)
{
    if (!strcmp(option, "yes")) {
      *value = TRUE;
    }
    else if (!strcmp(option, "no")) {
      *value = FALSE;
    }
    else {
      if (enqueue) {
         DCP_SET_MISC_ERROR(mcp_s_que_bad_enqueue_value);
      } else {
         DCP_SET_MISC_ERROR(mcp_s_que_bad_dequeue_value);
      }
    }

    return(TCL_OK);

} /* End of dcp_que_is_boolean_valid() routine */


int
dcp_que_is_reltime_valid
   (Tcl_Interp *                        interp,
    char *				time_string,
    utc_t **	                        tout)
{
    if (!strcmp(time_string, "0")) {
      *tout = NULL;       /* forever */
    } else {
      if ((utc_mkascreltime(*tout, time_string)) != 0) {
	DCP_SET_MISC_ERROR(mcp_s_utc_bad_rel_time);
      }
    }

    return(TCL_OK);

} /* End of dcp_que_is_reltime_valid() routine */


int
dcp_que_is_abstime_valid
   (Tcl_Interp *                        interp,
    char *				time_string, 
    utc_t **	                        tout)
{
    if (!strcmp(time_string, "0")) {
      *tout = NULL;       /* forever for expiration; now for ttr */
    } else {
      if ((utc_mkasctime(*tout, time_string)) != 0) {
	DCP_SET_MISC_ERROR(mcp_s_utc_bad_time);
      }
    }

    return(TCL_OK);

} /* End of dcp_que_is_abstime_valid() routine */


int
dcp_que_attr_set
   (Tcl_Interp *                        interp,
    dcp_que_attr_t *			iqattr, 
    mos_que_attr_t 	                oqattr)
{
  error_status_t	st;
  boolean 		allowenq, allowdeq;
  mos_que_attr_persistence_t pers;
  utc_t *		itout;
  int                   alias_argc;
  char                  **alias_argv;
  int			i;

  if (iqattr->qmaxlength != -1) {
    mos_que_attr_quemaxsize_set((unsigned32)iqattr->qmaxlength, oqattr, &st);
    CHECK_MOS_ERROR(st);
  }
  
  if (iqattr->msgmaxsize != -1) {
    mos_que_attr_msgmaxsize_set((unsigned32)iqattr->msgmaxsize, oqattr, &st);
    CHECK_MOS_ERROR(st);
  }

  if (iqattr->persistent != NULL) {
    if ((dcp_que_is_qpersistence_valid(interp, iqattr->persistent, &pers)) != TCL_OK) {
       return(TCL_ERROR);
    }
    mos_que_attr_persistence_set(pers, oqattr, &st);
    CHECK_MOS_ERROR(st);
  }
  
  if (iqattr->enq_opt != NULL) {
    if (dcp_que_is_boolean_valid(interp, TRUE, iqattr->enq_opt, &allowenq) != TCL_OK) {
       return(TCL_ERROR);
    }
    mos_que_attr_allowenq_set(allowenq, oqattr, &st);
    CHECK_MOS_ERROR(st);
  }
  
  if (iqattr->deq_opt != NULL) {
    if (dcp_que_is_boolean_valid(interp, FALSE, iqattr->deq_opt, &allowdeq) != TCL_OK){
       return(TCL_ERROR);
    }
    mos_que_attr_allowdeq_set(allowdeq, oqattr, &st);
    CHECK_MOS_ERROR(st);
  }
  
  if (iqattr->annotation != NULL) {
    mos_que_attr_annotation_set((unsigned_char_t *)iqattr->annotation, 
                                oqattr, 
                                &st);
    CHECK_MOS_ERROR(st);
  }
  
  if (iqattr->aliases != NULL) {
    if (Tcl_SplitList(interp, iqattr->aliases, &alias_argc, &alias_argv) != TCL_OK){
       return(TCL_ERROR);
    }
    /* reset the alias part in the attribute record */
    mos_que_attr_alias_reset(oqattr, &st);
    CHECK_MOS_ERROR(st);
    for (i = 0; i < alias_argc; i++) {
        mos_que_attr_alias_add(NULL, 
                               (unsigned_char_t *)alias_argv[i], 
                               oqattr, 
                               &st);
        CHECK_MOS_ERROR(st);
    }
    free((char *)alias_argv);
  }

  if (iqattr->idle_timeout != NULL) {
    if (dcp_que_is_reltime_valid(interp, iqattr->idle_timeout, &itout) != TCL_OK) {
       return(TCL_ERROR);
    }
    mos_que_attr_idletimeout_set(itout, oqattr, &st);
    CHECK_MOS_ERROR(st);
  }

  return(TCL_OK);

} /* End of dcp_que_attr_set() routine */
