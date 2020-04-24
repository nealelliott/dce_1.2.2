#!/bin/sh -e
#/* Copyright (C) 1993 Transarc Corporation - All rights reserved. */
#/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/ravage/RCS/test_bash.sh,v 1.1 1993/04/29 13:50:42 ota Exp $ */

: ${BASH:=bash}				# allow caller to override

Fatal()
{
	echo "${prog_name}: $*" 1>&2
	rm $aggr
	exit 2
}
Bash() { $BASH "$@" || Fatal "bash failed doing '$@'"; }
bASH() { $BASH "$@" 2> /dev/null && Fatal "bash succeeded doing '$@'"; }

bASH /tmp/sdfj/fofoo 1 1 1

aggr=/tmp/bash.junk.$$
touch $aggr
chmod 444 $aggr
bASH $aggr 1 1 1
chmod 666 $aggr
bASH $aggr 1 0 1
bASH $aggr 1 1000000000 1
bASH $aggr 0x89012345 1 1
bASH $aggr 1 1 1 2
Bash $aggr 1 2 1 2
bASH $aggr 1 2 1 2 3
# internal limit of 16 values
Bash $aggr 1 100 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
bASH $aggr 1 100 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17

# value defaults to zero
Bash $aggr 1 1

Bash $aggr 2312 3 0
Bash $aggr 2312 3 0xff
Bash $aggr 2312 3 0x81
Bash $aggr 2312 3 0377
Bash $aggr 2312 3 012
Bash $aggr 2312 3 -1
Bash $aggr 2312 3 -127
Bash $aggr 2312 3 -128
Bash $aggr 2312 3 -129
Bash $aggr 2312 3 -255
Bash $aggr 2312 3 -256
bASH $aggr 2312 3 -257
Bash $aggr 2312 3 255
bASH $aggr 2312 3 256
bASH $aggr 2312 3 0x123
bASH $aggr 2312 3 01234

bASH $aggr 2312 3 -W 1
bASH $aggr 2312 3 -W8 1

Bash $aggr 2312 4 -W16 0 32 256 12388
Bash $aggr 2312 3 -W16 0x1234
Bash $aggr 2312 3 -W16 -1 -4
Bash $aggr 2312 3 -W16 -32768
bASH $aggr 2312 3 -W16 0x12345
bASH $aggr 2312 3 -W16 0223456

Bash $aggr 2312 3 -W32 0
Bash $aggr 2312 3 -W32 0x1234
Bash $aggr 2312 3 -W32 -1
Bash $aggr 2312 3 -W32 -32768 02222 0xf23
Bash $aggr 2312 3 -W32 0x12345 0x34565678
Bash $aggr 2312 3 -W32 0223456
bASH $aggr 2312 3 -W32 0xgaea 
bASH $aggr 2312 3 -W32 099
bASH $aggr 2312 3 -W32 123a

rm $aggr
