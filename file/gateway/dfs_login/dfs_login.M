.\" 
.\" @OSF_COPYRIGHT@
.\" COPYRIGHT NOTICE
.\" Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
.\" ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
.\" the full copyright text.
.\" 
.\" HISTORY
.\" $Log: dfs_login.M,v $
NONERevision 1.1.6.1  1996/10/02  17:49:30  damon
NONE	Newest DFS from Transarc
NONE
NONERevision 1.1.5.2  1996/10/01  18:39:05  damon
NONE	Newest DFS from Transarc
NONE
.\" Revision /main/HPDCE02/1  1994/11/17  15:30 UTC  mgm
.\" 	fix leader in history
.\" 
.\" Revision 1.1.2.1  1994/07/26  19:12:47  mckeen
.\"	Expanded copyrights
.\"	Initial Version
.\"
.\" Revision 1.1.1.3  1994/07/26  18:57:34  mckeen
.\"	Expanded copyrights
.\"
.\" Revision 1.1.1.2  1994/07/26  16:08:04  mckeen
.\"	Initial Version
.\"
.\" $EndLog$
.\" 
.TH dlog 1 "" "HP DCE" ""
.ds )H Hewlett-Packard Company
.ds ]W HP DCE/9000 Version 1.2
.SH NAME
dfs_login, dfs_logout \- Remote authentication to a DCE/DFS Cell
.SH SYNOPSIS
.C dfs_login
.RC [ -h host ]
.RC [ -l [hh][:mm] ]
.RI [ \|dce-principal \|]
.RI [ \|dce-password \|]
.PP
.C dfs_logout
.RC [ -h host ]
.RI [ \|dce-principal \|]
.SH DESCRIPTION
.C dfs_login 
allows users on NFS client machines to establish DCE credentials
on the NFS exporter machine in order to gain access to the DFS.
A user needs to have a DCE account in the Cell that they want to
access through the NFS mounted DFS (/.../<cell>) path.

The -h argument is used to override the host that is being used as
the exporter running the dfsgwd server.  The default host is
obtained from the host that exported the local NFS mount of
"/..." .

The -l argument is used to create a Kerberos ticket with a lifetime
of the specified number of hours and minutes. The default lifetime
is 8 hours.

The dce-principal will be the currently logged in user name unless
an alternate is given as an argument.

The user will be prompted for a password unless one is supplied
as an argument.
.PP
.C dfs_logout 
will destroy the authenticated mappings created by dfs_login.
.PP
Both of these commands require a working Kerberos5 environment on
the host that is attempting to mount the DFS locally. See the 
.I HP DCE/9000 Release Notes
for more information on setting up this
facility.
.SH FILES
/krb5/krb.conf and /krb5/krb.realms
.SH VARIABLES
The DLOGSERVICE environment variable can be set to change the service
name to something other than the default of "dlog".  This
service name is used to lookup both the UDP port to find the
dfsgwd server and the name of the DCE Security registry service.
.SH SEE ALSO
dfsgw(8),
dfsgwd(8)


