/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util_binding.c,v $
 * Revision 1.1  1996/10/07  16:40:43  wwang
 * Initial revision
 *
 * Revision 1.1  1996/10/01  17:07:12  wwang
 * Initial revision
 *
 * Revision 1.1.6.1  1996/01/26  23:41:48  wwang
 * 	Fix for OT 13289: "acl show" command will not handle a list of more than 6 elements
 * 	[1996/01/26  23:41:35  wwang]
 *
 * Revision 1.1.2.2  1994/08/24  19:20:41  pwang
 * 	Added dcp_binding_to_bytes() {OT#11575]
 * 	[1994/08/24  19:20:15  pwang]
 * 
 * Revision 1.1.2.1  1994/03/28  23:00:28  melman
 * 	Added rpcentry show and binding utilities
 * 	[1994/03/28  22:54:58  melman]
 * 
 * $EndLog$
 */
/*
 * MODULE: util_binding.c
 *
 * DESCRIPTION:
 *   This module contains routines to transform a binding between
 *   string syntax and tcl syntax
 */

/* INCLUDE FILES  */
#include <util_binding.h>

/* PROTOTYPES */
static int is_uuid_string(char *);


/*
 * FUNCTION: dcp_binding_string_to_tcl
 *
 * OVERVIEW:  Takes a string binding and returns a string of the
 *            string binding in tcl syntax.
 *            Calls malloc() to allocate the memory.
 *            On error rpc_status is set and returns NULL.
 *            Ignores network options.
 *
 * INPUTS:
 *  str                the string binding
 *
 * INPUT/OUTPUTS:
 *  rpc_status         status code
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  tcl                a string of the tcl syntax
 */
char *dcp_binding_string_to_tcl(char *str, unsigned32 *rpc_status)
{
    int   len;
    char *tcl;
    char *objuuid;
    char *protseq;
    char *netaddr;
    char *endpoint;
    char *netopts;                      /* ignored */

    rpc_string_binding_parse((unsigned char *)str,
                             (unsigned char **)&objuuid,
                             (unsigned char **)&protseq,
                             (unsigned char **)&netaddr,
                             (unsigned char **)&endpoint,
                             (unsigned char **)&netopts,
                             rpc_status);
    if (*rpc_status != rpc_s_ok) {
        return NULL;
    }
        
    len = strlen(objuuid) + strlen(protseq) +
          strlen(netaddr) + strlen(endpoint) + 4; /* 3 spaces + NULL */
    tcl = (char *)malloc(len);
    tcl[0] = '\0';                      /* make a NULL string */

    /* build up one at a time to avoid extra spaces */
    if (*objuuid) {
        strcat(tcl, objuuid);
    }
    if (*protseq) {
        if (*tcl) strcat(tcl, " ");
        strcat(tcl, protseq);
    }
    if (*netaddr) {
        if (*tcl) strcat(tcl, " ");
        strcat(tcl, netaddr);
    }
    if (*endpoint) {
        if (*tcl) strcat(tcl, " ");
        strcat(tcl, endpoint);
    }
    return tcl;
}

/*
 * FUNCTION: dcp_binding_tcl_to_string
 *
 * OVERVIEW:  Turns a binding in tcl syntax into a string binding.
 *            Accepts an argc, argv as returned by Tcl_SplitList()
 *            as the tcl syntax, and returns a string binding as a char *.
 *            if not 2 <= argc <= 4 then rpc_status is unchanged and
 *            NULL returned. 
 *            Use rpc_string_free() to free the allocated memory.
 *            On error rpc_status is set and returns NULL.
 *            Ignores network options.
 *
 * INPUTS:
 *  tcl                a string of the tcl syntax
 *
 * INPUT/OUTPUTS:
 *  rpc_status         status code
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  str                the string binding or NULL on error
 */
char *dcp_binding_tcl_to_string(int argc, char **argv, unsigned32 *rpc_status)
{
    char *str;
    char *objuuid=NULL;
    char *protseq=NULL;
    char *netaddr=NULL;
    char *endpoint=NULL;
    char *netopts=NULL;                 /* ignored */

    switch (argc) {
      case 2: 
        protseq = argv[0];
        netaddr = argv[1];
        break;
      case 3:
        if (is_uuid_string(argv[0])) {
            objuuid = argv[0];
            protseq = argv[1];
            netaddr = argv[2];
        } else {
            protseq = argv[0];
            netaddr = argv[1];
            endpoint = argv[2];
        }
        break;
      case 4: 
        objuuid = argv[0];
        protseq = argv[1];
        netaddr = argv[2];
        endpoint = argv[3];
        break;
      default:
	/* set the rpc_status */
	*rpc_status = rpc_s_invalid_binding;
        return NULL;
    }
    
    rpc_string_binding_compose((unsigned char *)objuuid,
                               (unsigned char *)protseq,
                               (unsigned char *)netaddr,
                               (unsigned char *)endpoint,
                               (unsigned char *)netopts,
                               (unsigned char **)&str,
                               rpc_status);
    if (*rpc_status != rpc_s_ok) {
        return NULL;
    }
    return str;
}

/*
 * FUNCTION: dcp_binding_to_bytes
 *
 * OVERVIEW:  Format a binding data into TCL binary format
 *
 * INPUTS:
 *  twr_t            *binding_data 
 *  char             *leading_space
 *
 * INPUT/OUTPUTS:
 *  error_status_t*  st
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  str                the string contains the binding data in binary  format
 */
char *dcp_binding_to_bytes(twr_t          *binding_data, 
                           char           *leading_space, 
                           error_status_t *st
)
{
    char *buf, *ptr;
    int  i, j, lines, leading, size, index;

    *st = error_status_ok;
    buf = NULL;

    /*
     * If NULL binding data, just return
     */
    if ((!binding_data->tower_length) || (!binding_data->tower_octet_string)) {
        return buf;
    }       

    /*
     * Allocate enough space for the tcl bytes bufing
     */
    lines = binding_data->tower_length / DCP_BINDING_MAX_BYTES_PER_LINE;
    if (binding_data->tower_length % DCP_BINDING_MAX_BYTES_PER_LINE) {
        lines++;
    }
    leading = strlen(leading_space);
    size = binding_data->tower_length * 2 + 
           lines * (DCP_BINDING_MAX_BYTES_PER_LINE - 1) + 
           lines * leading + (lines - 1) + 2 + lines;

    buf = (char *)malloc(size);
    if (!buf) {
        *st = mcp_s_no_memory;
        return buf;
    }
    memset(buf, 0, size);

   /*
    * Formating ...
    */
   index = 0;
   ptr = buf;
   for (i=0; i<lines; i++) {
       strcpy(ptr, leading_space);
       ptr += leading;
       if (i == 0) {
           *ptr = '{';
       }
       else {
           *ptr = ' ';
       }
       ptr++;
       for (j=0; j<DCP_BINDING_MAX_BYTES_PER_LINE; j++) {
           sprintf(ptr, "%2.2x", binding_data->tower_octet_string[index++]);
           ptr += 2;
           if (index == binding_data->tower_length) {
               break;
           }
           if (j < (DCP_BINDING_MAX_BYTES_PER_LINE - 1)) {
               *ptr++ = ' ';
           }
       }
       if (i ==  (lines - 1)) {
           *ptr++ = '}';
           *ptr = '\0';
       }
       else {
           *ptr++ = '\n';
       }
   }
   return buf;
}

/*
 * FUNCTION: is_uuid_string()
 *
 * OVERVIEW: returns TRUE if string is a valid UUID, FALSE if not
 * 
 *
 * INPUTS:
 *  str                 purported uuid string
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TRUE                a valid uuid string
 *  FALSE               not a valid uuid string
 */
static int is_uuid_string(char *str)
{
  uuid_t        uuid;
  unsigned32    status;

  uuid_from_string((unsigned char *)str, &uuid, &status);
  return (status==uuid_s_ok);
}
