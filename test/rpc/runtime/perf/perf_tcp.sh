#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: perf_tcp.sh,v $
# Revision 1.1.9.2  1996/03/11  02:29:03  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:35  marty]
#
# Revision 1.1.9.1  1995/12/11  20:15:05  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:47:58  root]
# 
# Revision 1.1.7.2  1994/05/27  15:36:57  tatsu_s
# 	Added the back-to-back maybe call test.
# 	[1994/04/22  19:22:19  tatsu_s]
# 
# Revision 1.1.7.1  1994/04/06  20:05:21  tom
# 	Bug 10054 - comment out unsupported test 12.
# 	For test 15[cd], increase cancel timeout from 1 to 5 seconds.
# 	[1994/04/06  19:58:40  tom]
# 
# Revision 1.1.5.1  1993/10/15  21:03:06  ohara
# 	make geting the IP address more portable
# 	[1993/10/06  18:09:06  ohara]
# 
# Revision 1.1.3.2  1993/01/13  19:56:51  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:24:42  bbelch]
# 
# Revision 1.1  1992/01/19  03:41:22  devrcs
# 	Initial revision
# 
# $EndLog$
#

case $# in 
2) ;;
*) echo 'Usage: perf_tcp.sh server_host_name client_program_directory' 1>&2 ; exit 1;;
esac

cd $2
##IP=`host $1 | awk '{print $3}'`
# Use the dce getip program, and if that doesn't work, try the
# basic approach of greping /etc/hosts.  This combination
# should cover most any platform.
if [ -x /opt/dcelocal/bin/getip ]; then
        IP=`getip $1`
else
        IP=`grep $1 /etc/hosts | awk '{print $1}'`
fi

echo "client 0a"
client 0 "ncacn_ip_tcp:${IP}[2001]" 3 40 y y
echo "client 0b"
client 0 "ncacn_ip_tcp:${IP}[2001]" 3 40 y n
echo "client 0c"
client 0 "ncacn_ip_tcp:${IP}[2001]" 3 40 n y
echo "client 0d"
client 0 "ncacn_ip_tcp:${IP}[2001]" 3 40 n n
echo "client 1a"
client 1 "ncacn_ip_tcp:${IP}[2001]" 3 40 y y 400
echo "client 1b"
client 1 "ncacn_ip_tcp:${IP}[2001]" 3 40 y n 400
echo "client 1c"
client 1 "ncacn_ip_tcp:${IP}[2001]" 3 10 y y 4000
echo "client 1d"
client 1 "ncacn_ip_tcp:${IP}[2001]" 3 10 y n 4000
#echo "client 1e"
#client 1 "ncacn_ip_tcp:${IP}[2001]" 3 2 y y 100000
#echo "client 1f"
#client 1 "ncacn_ip_tcp:${IP}[2001]" 3 2 y n 100000
echo "client 2a"
client 2 "ncacn_ip_tcp:${IP}[2001]" 3 100 y y 400
echo "client 2b"
client 2 "ncacn_ip_tcp:${IP}[2001]" 3 100 y n 400
echo "client 2c"
client 2 "ncacn_ip_tcp:${IP}[2001]" 3 10 y y 4000
echo "client 2d"
client 2 "ncacn_ip_tcp:${IP}[2001]" 3 10 y n 4000
#echo "client 2e"
#client 2 "ncacn_ip_tcp:${IP}[2001]" 3 2 y y 100000
#echo "client 2f"
#client 2 "ncacn_ip_tcp:${IP}[2001]" 3 2 y n 100000
#echo "client 3"
#client 3 "ncacn_ip_tcp"
echo "client 4"
client 4 "ncacn_ip_tcp:${IP}[2001]" 3 2
#echo "client 5"
#client 5 "ncacn_ip_tcp"
echo "client 6a"
client 6 "ncacn_ip_tcp:${IP}[2001]" 3 100 y y
echo "client 6b"
client 6 "ncacn_ip_tcp:${IP}[2001]" 3 100 y n
echo "client 8"
client 8 "ncacn_ip_tcp:${IP}[2001]" y
echo "client 7"
client 7 "ncacn_ip_tcp:${IP}[2001]"
echo "client 9"
client 9 "ncacn_ip_tcp:${IP}[2001]"
echo "client 10a"
client 10 "ncacn_ip_tcp:${IP}[2001]" 4 3 y y 2
echo "client 10b"
client 10 "ncacn_ip_tcp:${IP}[2001]" 2 3 y n 2
echo "client 10c"
client 10 "ncacn_ip_tcp:${IP}[2001]" 4 3 y y 2 1
echo "client 10d"
client 10 "ncacn_ip_tcp:${IP}[2001]" 2 3 y n 2 1
echo "client 10e"
client 10 "ncacn_ip_tcp:${IP}[2001]" 4 3 y y 2 2
echo "client 10f"
client 10 "ncacn_ip_tcp:${IP}[2001]" 2 3 y n 2 2 
# These test are unsupported and will fail
#echo "client 12a"
#client 12 "ncacn_ip_tcp:${IP}[2001]" 2 10 y
#echo "client 12b"
#client 12 "ncacn_ip_tcp:${IP}[2001]" 2 10 n
echo "client 13"
client 13 "ncacn_ip_tcp:${IP}[2001]"
echo "client 14a"
client 14 "ncacn_ip_tcp:${IP}[2001]" 4 n 1 
echo "client 14b"
client 14 "ncacn_ip_tcp:${IP}[2001]" 4 y 1 
echo "client 15a"
client 15 "ncacn_ip_tcp:${IP}[2001]" 2 y 1 
echo "client 15b"
client 15 "ncacn_ip_tcp:${IP}[2001]" 2 n 1 
echo "client 15c"
client 15 "ncacn_ip_tcp:${IP}[2001]" 2 y 1 5
echo "client 15d"
client 15 "ncacn_ip_tcp:${IP}[2001]" 2 n 1 5 

