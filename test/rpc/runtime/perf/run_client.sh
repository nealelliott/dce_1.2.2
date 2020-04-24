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
# $Log: run_client.sh,v $
# Revision 1.1.6.2  1996/03/11  02:29:21  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:50  marty]
#
# Revision 1.1.6.1  1995/12/11  20:15:18  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:48:11  root]
# 
# Revision 1.1.4.3  1994/08/10  21:04:17  tom
# 	Add a idempotent FP test (6) so previous maybe test (for CN)
# 	wont give false errors.
# 	[1994/08/10  21:04:05  tom]
# 
# Revision 1.1.4.2  1994/05/27  15:36:59  tatsu_s
# 	Added the back-to-back maybe call test.
# 	[1994/04/22  19:22:36  tatsu_s]
# 
# Revision 1.1.4.1  1994/04/06  20:05:25  tom
# 	Bug 10054 - Don't run DG only tests if testing CN.
# 	[1994/04/06  19:58:47  tom]
# 
# Revision 1.1.2.2  1993/01/13  19:57:37  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:25:31  bbelch]
# 
# Revision 1.1  1992/01/19  03:41:45  devrcs
# 	Initial revision
# 
# $EndLog$
#

#
# Unix Bourne shell script for running perf client tests
# The parameter passed in is a string binding.  

# Usage:
#           run_client.sh `string binding returned by server`
#
# An example run of this test involves starting the server:
#        ./server 1 ncadg_ip_udp
#              ==> returns binding, such as ncadg_ip_udp:129.35.130.133[2001]
#         run_client.sh ncadg_ip_udp:129.35.130.133[2001]
#

CLIENT=client
DEBUG=
PC=false

for ARG
do
    case $ARG in
    -d)
        DEBUG="-d"
        shift 
        ;;
    -pc)
        PC=true
        shift 
        ;;
    *) 
        break
        ;;
    esac
done

echo "Testing against $1..."

protocol=`expr "$1" : "\(.*\):"`

${CLIENT} ${DEBUG} 0 $1 3 400 y n                # Null call  
${CLIENT} ${DEBUG} 0 $1 3 400 y y                # Null idempotent call
${CLIENT} ${DEBUG} 1 $1 3 50 y n 3000            # Ins 
${CLIENT} ${DEBUG} 1 $1 3 50 y y 3000            # Ins, idempotent
${CLIENT} ${DEBUG} 2 $1 3 50 y n 3000            # Outs
${CLIENT} ${DEBUG} 2 $1 3 50 y y 3000            # Outs idempotent
if [ $PC != "true" ]
then
    ${CLIENT} ${DEBUG} 1 $1 3 3 y n 100000       # Ins (100K)
    ${CLIENT} ${DEBUG} 1 $1 3 3 y y 100000       # Ins, idempotent (100K)
    ${CLIENT} ${DEBUG} 2 $1 3 3 y n 100000       # Outs (100K)
    ${CLIENT} ${DEBUG} 2 $1 3 3 y y 100000       # Outs idempotent (100K)
fi

# Note: test 3 only works with UDP
if [ "$protocol" = "ncadg_ip_udp" ]
then
    ${CLIENT} ${DEBUG} 3 ncadg_ip_udp                # Broadcast
fi

${CLIENT} ${DEBUG} 4 $1 3 2                         # Maybe

# Note: test 5 only works with UDP
if [ "$protocol" = "ncadg_ip_udp" ]
then
    ${CLIENT} ${DEBUG} 5 ncadg_ip_udp                # Broadcast/maybe
fi
${CLIENT} ${DEBUG} 6 $1 3 100 y y                # Floating point
${CLIENT} ${DEBUG} 6 $1 3 100 y n                # Floating point
${CLIENT} ${DEBUG} 7 $1                          # Unregistered interface
${CLIENT} ${DEBUG} 8 $1 n                        # Forwarding
${CLIENT} ${DEBUG} 9 $1                          # Exception
${CLIENT} ${DEBUG} 10 $1 2 2 y n 60              # Slow call
${CLIENT} ${DEBUG} 10 $1 2 2 y y 60              # Slow idempotent call
