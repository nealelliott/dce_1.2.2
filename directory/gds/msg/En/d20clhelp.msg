$ 
$ @OSF_COPYRIGHT@
$ COPYRIGHT NOTICE
$ Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
$ ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
$ src directory for the full copyright text.
$ 
$ HISTORY
$ $Log: d20clhelp.msg,v $
$ Revision 1.1.4.2  1996/03/09  23:14:11  marty
$ 	Update OSF copyright year
$ 	[1996/03/09  22:31:30  marty]
$
$ Revision 1.1.4.1  1995/12/08  16:04:39  root
$ 	Submit OSF/DCE 1.2.1
$ 	[1995/12/08  15:15:50  root]
$ 
$ Revision 1.1.2.1  1994/08/08  13:21:02  marrek
$ 	Fix for OT11045.
$ 	[1994/08/08  13:19:05  marrek]
$ 
$ $EndLog$
$
$ $RCSfile: d20clhelp.msg,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:14:11 $
$
$set 1 Help Messages for the Command Line

$ If you change this help text, please change the default help
$ also in d20_disp_clhelp() (see adm/usradmin/d20param.c)
$ and check D20_CLHELP_LINE_NR (see h/d20_adm.h)

1 Usage:
2 gdssysadm -f<function> [-d<directory ID>] [-m<conf.mode>]
3           [-c<configuration type>] [-C<number of clients>]
4           [-s<number of servers>] [-u<delta update>] [-o<operation>]
5           [-M<data medium type>] [-n<filename>] [-k[<password>]]
6           [-F<formatting>] [-v<volume number>] [-D] [-p] [-X]
7           [-A<authentication mechanism>] [-h]
