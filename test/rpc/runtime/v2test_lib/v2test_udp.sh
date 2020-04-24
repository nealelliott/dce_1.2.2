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
# $Log: v2test_udp.sh,v $
# Revision 1.1.7.2  1996/03/11  02:29:34  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:56  marty]
#
# Revision 1.1.7.1  1995/12/11  20:15:36  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:48:35  root]
# 
# Revision 1.1.5.1  1994/09/30  17:17:01  tom
# 	Use getip instead of host command.
# 	Run v2client out of current directory.
# 	[1994/09/30  16:53:43  tom]
# 
# Revision 1.1.3.3  1993/01/12  00:03:44  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:26:59  bbelch]
# 
# Revision 1.1.3.2  1992/09/29  21:24:41  devsrc
# 	SNI/SVR4 merge.  OT 5373
# 	[1992/09/11  15:49:14  weisman]
# 
# Revision 1.1  1992/01/19  03:41:54  devrcs
# 	Initial revision
# 
# $EndLog$
#

case $# in 
3) ;;
*) echo 'Usage: v2test_udp.sh server_host_name port_number client_program_directory' 1>&2 ; exit 1;;
esac

cd $3
IP=`getip $1`
set -x
echo "v2client 0a (Null test; non-idempotent)"
./v2client n "ncadg_ip_udp:${IP}[$2]" 100 100 400 0
echo "v2client 0b (Null test; idenpotent)"
./v2client n "ncadg_ip_udp:${IP}[$2]" 100 100 400 2
echo "v2client 0c (Null test; broadcast)"
./v2client n "ncadg_ip_udp:${IP}[$2]" 100 1 400 1
#
echo "v2client 1a (input test; non-idempotent)"
./v2client i "ncadg_ip_udp:${IP}[$2]" 100 10 400 0
echo "v2client 1b (input test; idempotent)"
./v2client i "ncadg_ip_udp:${IP}[$2]" 100 10 400 2
echo "v2client 1c (input test; broadcast)"
./v2client i "ncadg_ip_udp:${IP}[$2]" 100 1 400 1 
echo "v2client 1d (input test; non-idempotent)"
./v2client i "ncadg_ip_udp:${IP}[$2]" 20 10 4000 0
echo "v2client 1e (input test; idempotent)"
./v2client i "ncadg_ip_udp:${IP}[$2]" 20 10 4000 2
echo "v2client 1f (input test; non-idempotent)"
./v2client i "ncadg_ip_udp:${IP}[$2]"  5 3 100000 0
echo "v2client 1g (input test; idempotent)"
./v2client i "ncadg_ip_udp:${IP}[$2]" 5 3 100000 2
#
echo "v2client 2a (input test; non-idempotent)"
./v2client o "ncadg_ip_udp:${IP}[$2]" 100 10 400 0
echo "v2client 2b (input test; idempotent)"
./v2client o "ncadg_ip_udp:${IP}[$2]" 100 10 400 2
echo "v2client 2c (input test; broadcast)"
./v2client o "ncadg_ip_udp:${IP}[$2]" 100 1 400 1 
echo "v2client 2d (input test; non-idempotent)"
./v2client o "ncadg_ip_udp:${IP}[$2]" 20 10 4000 0
echo "v2client 2e (input test; idempotent)"
./v2client o "ncadg_ip_udp:${IP}[$2]" 20 10 4000 2
echo "v2client 2f (input test; non-idempotent)"
./v2client o "ncadg_ip_udp:${IP}[$2]"  5 3 100000 0
echo "v2client 2g (input test; idempotent)"
./v2client o "ncadg_ip_udp:${IP}[$2]" 5 3 100000 2
#
echo "v2client 3a (input test; non-idempotent)"
./v2client io "ncadg_ip_udp:${IP}[$2]" 100 10 400 0
echo "v2client 3b (input test; idempotent)"
./v2client io "ncadg_ip_udp:${IP}[$2]" 100 10 400 2
echo "v2client 3c (input test; broadcast)"
./v2client io "ncadg_ip_udp:${IP}[$2]" 100 1 400 1 
echo "v2client 3d (input test; non-idempotent)"
./v2client io "ncadg_ip_udp:${IP}[$2]" 20 10 4000 0
echo "v2client 3e (input test; idempotent)"
./v2client io "ncadg_ip_udp:${IP}[$2]" 20 10 4000 2
echo "v2client 3f (input test; non-idempotent)"
./v2client io "ncadg_ip_udp:${IP}[$2]"  5 3 100000 0
echo "v2client 3g (input test; idempotent)"
./v2client io "ncadg_ip_udp:${IP}[$2]" 5 3 100000 2
#
echo "v2client 4a (Null test; non-idempotent; maintain liveness)"
./v2client n "ncadg_ip_udp:${IP}[$2]" 1 10 400 8
echo "v2client 4b (Null test; idenpotent; maintain liveness)"
./v2client n "ncadg_ip_udp:${IP}[$2]" 1 10 400 10
echo "v2client 4c (Null test; broadcast; maintain liveness)"
./v2client n "ncadg_ip_udp:${IP}[$2]" 1 1 400 9
#
echo "v2client 5a (input test; non-idempotent; maintain liveness)"
./v2client io "ncadg_ip_udp:${IP}[$2]" 1 10 400 8
echo "v2client 5b (input test; idempotent; maintain liveness)"
./v2client io "ncadg_ip_udp:${IP}[$2]" 1 10 400 10
echo "v2client 5c (input test; broadcast; maintain liveness)"
./v2client io "ncadg_ip_udp:${IP}[$2]" 1 1 400 9 
