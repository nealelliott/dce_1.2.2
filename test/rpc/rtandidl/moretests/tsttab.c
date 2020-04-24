/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsttab.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:43  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:27  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:12:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:06  root]
 * 
 * Revision 1.1.2.2  1993/01/13  19:49:55  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:16:46  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  %a%copyright(,**  )
**
**  NAME
**
**      tsttab.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Tests for the Server Object services of the RPC Communications 
**  Component
**
**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**  02-feb-91 burton    add comtest_object_xfr
**  15-oct-90 burton    original
**
**  %a%private_end  
**
*/

#include <tstdriver.h>

 int comtest_bind_from_str_bind ();
 int comtest_bind_to_str_bind ();
 int comtest_bind_str_binding_comp ();
 int comtest_bind_str_binding_parse ();
 int comtest_bind_free ();
 int comtest_bind_copy ();
 int comtest_bind_vector_free ();
 int comtest_bind_reset ();
 int comtest_bind_inq_object ();
 int comtest_bind_set_object ();
 int comtest_bind_inq_client ();
 int comtest_if_inq_id ();
 int comtest_if_id_vector_free ();
 int comtest_net_inq_protseq ();
 int comtest_net_is_protseq_valid ();
 int comtest_client_xfr ();
 int comtest_object_xfr ();
 int comtest_uuid_vers2 ();


struct  cmd  cmdtab[] = {
{ "comtest_bind_from_str_bind",      comtest_bind_from_str_bind      },
{ "comtest_bind_to_str_bind",        comtest_bind_to_str_bind        },
{ "comtest_bind_str_binding_comp",   comtest_bind_str_binding_comp   },
{ "comtest_bind_str_binding_parse",  comtest_bind_str_binding_parse  },
{ "comtest_bind_free",               comtest_bind_free               },
{ "comtest_bind_copy",               comtest_bind_copy               },
{ "comtest_bind_vector_free",        comtest_bind_vector_free        },
{ "comtest_bind_reset",              comtest_bind_reset              },
{ "comtest_bind_inq_object",         comtest_bind_inq_object         },
{ "comtest_bind_set_object",         comtest_bind_set_object         },
{ "comtest_bind_inq_client",         comtest_bind_inq_client         },
{ "comtest_if_inq_id",               comtest_if_inq_id               },
{ "comtest_if_id_vector_free",       comtest_if_id_vector_free       },
{ "comtest_net_inq_protseq",         comtest_net_inq_protseq         },
{ "comtest_net_is_protseq_valid",    comtest_net_is_protseq_valid    },
{ "comtest_client_xfr",              comtest_client_xfr              },
{ "comtest_object_xfr",              comtest_object_xfr              },
{ "comtest_uuid_vers2",              comtest_uuid_vers2              },
{ 0 },
};

char  *tstscript  =  "./testscript";   /*  Default input testscript file */
