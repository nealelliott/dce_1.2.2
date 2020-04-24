#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: makeasm.sh,v $
# Revision 1.1.12.2  1996/03/09  23:13:16  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:29  marty]
#
# Revision 1.1.12.1  1995/12/08  15:30:39  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:55:11  root]
# 
# Revision 1.1.10.1  1994/02/22  16:04:37  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  10:05:14  marrek]
# 
# Revision 1.1.8.1  1993/10/28  13:33:00  keutel
# 	CR 9279
# 	[1993/10/28  13:24:27  keutel]
# 
# Revision 1.1.6.2  1993/08/11  13:39:00  marrek
# 	July 1993 code drop.
# 	[1993/07/30  14:08:41  marrek]
# 
# Revision 1.1.4.3  1992/12/31  18:57:40  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:16:49  bbelch]
# 
# Revision 1.1.4.2  1992/10/05  16:48:14  marrek
# 	Insert #!/bin/sh .
# 	[1992/10/05  16:47:44  marrek]
# 
# Revision 1.1.2.2  1992/06/01  23:40:51  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  20:42:15  zeliff]
# 
# $EndLog$
#
# $RCSfile: makeasm.sh,v $ $Revision: 1.1.12.2 $ $Date: 1996/03/09 23:13:16 $
#
#****************************************************************************
#                                                                           *
#        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
#                             ALL RIGHTS RESERVED                           *
#                                                                           *
#****************************************************************************

#
# shell script to create an assembler file
# depending on Machine and/or compiler
#
# call format:  makeasm $(CC) $(CFLAGS)
# produces isasmb.s or isasmb.c from asmb.?

CC=$1

shift

machine=c

rm -f isasmb.asm isasmb.c

#for a
#do
#    case $a in
#        -DM16S30)
#                machine=s
#                cat asmb.32kMX2 >isasmb.s;;
#        -DM32S50)
#                machine=s
#                cat asmb.32000 >isasmb.s;;
#        -DISAM_LOCK)
#                machine=s
#                cat asmb.ISAM_LOCK >isasmb.s;;
#        -DWX200)
#                machine=s
#                cat asmb.wx200 >isasmb.asm;;
#    esac
#done

if [ $machine = c ]
then
    cat asmb.c >isasmb.c
    $CC $* -S isasmb.c
fi

