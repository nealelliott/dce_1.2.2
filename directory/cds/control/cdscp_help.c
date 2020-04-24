/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdscp_help.c,v $
 * Revision 1.1.8.2  1996/02/18  19:27:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:57  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:06:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:24  root]
 * 
 * Revision 1.1.6.4  1994/08/03  19:00:14  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:36:19  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  16:08:24  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:53:10  devsrc]
 * 
 * Revision 1.1.6.2  1994/05/06  17:42:32  jd
 * 	The help text string wss broken into two strings to make them shorter.
 * 	Put both of them out.
 * 	[1994/05/06  17:35:56  jd]
 * 
 * Revision 1.1.6.1  1994/03/12  21:57:36  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:56:19  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:01:22  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:31:00  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:45:24  weisman
 * 		New file from Digital (post 1.0)
 * 	[1992/03/22  20:54:56  weisman]
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1992-1994. ALL RIGHTS RESERVED.
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
 *
 * DESCRIPTION OF MODULE:
 *
 *  This module supports the 'cdscp' help function.
 *
 *
 */

#include <cdscp.h>

/*
 * This is the help text.
 *
 * For now, we are including the help text right here along with the
 * code.  Eventually, I18N considerations will probably force us to
 * create yet-another-text-file containing the help text.
 *
 */

static char cdscp_help_text[] = {
"\n\
usage: cdscp operation [options] [args]\n\
\n\
add     directory\n\
add     object\n\
clear   cached server\n\
clear   clearinghouse\n\
create  child\n\
create  clearinghouse\n\
create  directory\n\
create  link\n\
create  object\n\
create  replica\n\
define  cached server\n\
delete  child\n\
delete  clearinghouse\n\
delete  directory\n\
delete  link\n\
delete  object\n\
delete  replica\n\
delete  subtree\n\
disable clerk\n\
disable server\n\
dump    clerk cache\n\
dump    subtree\n\
list    child\n\
list    clearinghouse\n\
list    directory\n\
list    link\n\
list    object\n\
merge   file\n\
merge   subtree\n\
recreate directory\n\
recreate link\n\
recreate object\n\
replace subtree\n\
replace object\n\
replace link\n\
remove  directory\n\
remove  link\n\
remove  object\n\
set     cdscp confidence\n\
set     cdscp preferred clearinghouse\n\
set     directory\n\
set     directory to new epoch\n\
set     directory to skulk\n\
set     link\n\
set     object\n\
show    cached clearinghouse\n\
show    cached server\n\
show    cdscp confidence\n\
show    cdscp preferred clearinghouse\n\
show    cell\n\
show    child\n\
show    clearinghouse\n\
show    clerk\n\
show    directory\n\
show    link\n\
show    object\n\
show    replica\n\
show    server\n"
};

/*
 * This is the help routine.
 *
 * We simply spit out the help text in a no-frills fashion.
 * (ie. nothing fancy like 'more')
 */

dns_status_t
cdscp_help (void)
{
    /*
     * It turns out that one string is pretty big and blows the lex buffer
     * on at least one reference platform. Break it up.
     */
    cds_fprintf (stdout, CDSCP_HELP_TEXT1, cdscp_help_text);
    cds_fprintf (stdout, CDSCP_HELP_TEXT2, cdscp_help_text);
    return (DNS_SUCCESS);
}
