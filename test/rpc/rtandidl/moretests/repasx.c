/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: repasx.c,v $
 * Revision 1.1.4.2  1996/02/17  23:21:55  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:00  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:09:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:45  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:27:38  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:43:47  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:39:30  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:12:46  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:36:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/



/* extern char *malloc();  */
ndr_boolean from_local_rep = 0;
ndr_boolean to_local_rep = 0;
ndr_boolean free_net_rep = 0;
ndr_boolean free_local_rep = 0;

repastest_t2 net_buf;
repastest_t1 loc_buf;

/***************************************************************************/
/* In the routines below, one would normally malloc a buffer for use in    */
/* storing the converted data.  However, the test is simply to see that the*/
/* routine is in fact called, not necessarily what it does.  By using a    */
/* known buffer for each conversion, I avoid significant overhead in malloc*/
/* and free operations which would normally total about 1000 per test.     */
/***************************************************************************/


void repastest_t2_from_local(from_local, to_net)
repastest_t1 *from_local;
repastest_t2 **to_net;
{
    /***********************************************************************/
    /* Convert from the local representation to the network representation */
    /* Don't bother doing a malloc, just assign the net_buf buffer.
    /***********************************************************************/
    net_buf.f1 = net_buf.f2 = from_local->f1/2;
    *to_net = &net_buf;
    from_local_rep = 1;
    
}

void repastest_t2_free_inst(to_net)
repastest_t2 *to_net;
{
    /***********************************************************************/
    /* After shipping the data over the net, get rid of the network buffer */
    /* Don't have to free the net_buf buffer.
    /***********************************************************************/
    free_net_rep = 1;
}

void repastest_t2_to_local(from_net, to_local)
repastest_t2 *from_net;
repastest_t1 *to_local;
{
    /***********************************************************************/
    /* Convert from the network representation to the local representation */
    /* Don't bother doing a malloc, just assign the loc_buf buffer.
    /***********************************************************************/
    to_local->f1 = from_net->f1 + from_net->f2;
/*	to_local = &loc_buf; */
    to_local_rep = 1;
}
                  
/* for callee to free local type */
void repastest_t2_free_local(to_local)
repastest_t1 *to_local;
{
    /***********************************************************************/
    /* After marshalling the data into the appropriate buffer, get rid of  */
    /* the intermediate buffer, used to store the local representation.    */
    /* Don't have to free the loc_buf buffer.
    /***********************************************************************/
    free_local_rep = 1;
}

